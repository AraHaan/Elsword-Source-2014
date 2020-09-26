#include "ServerDefine.h"
//{{ 2013. 2. 6	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
#include <WinSock2.h>	// windows.h redefinition ���� ��ó��
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}
#include "CnRoomManager.h"

#include "Room/BaseRoomFSM.h"
#include "Room/RoomFSM.h"
#include "Room/TradeFSM.h"
#include "Room/PersonalShopFSM.h"

#include "PvPRoom.h"
#include "DungeonRoom.h"
#include "TrainingCenterRoom.h"
#include "Square.h"
#include "Trade.h"
#include "PersonalShop.h"
//{{ 2011. 10. 27	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "BattleFieldRoom.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#include "NetError.h"

#include "CnServer.h"

#include "socket/NetCommon.h"		// ������ ����(����� �� ����) IP
#include "CnNetLayer.h"				// ������ ����(����� �� ����) Port
#include "AbuserLog/KAbuserLogManager.h"

//{{ 2011. 12. 05	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewCnPartyManager.h"
#else
	#include "CnPartyManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
	#include "GameEvent/GameEventManager.h"
#endif SERV_EVENT_SCRIPT_REFRESH
//}}

//{{ 2011. 01. 26	������	���� Ŭ���� Ÿ�� ������ üũ
#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
	#include "CnHackingCheckManager.h"
#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
//}}

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#include "EventDataRefresh/ReleaseTickManager.h"
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}

//{{ 2012. 04. 18	������	�� �Ŵ��� �����ս� ���
#ifdef SERV_ROOM_MANAGER_PARALLEL_FOR_EACH
	#include <tbb/task_scheduler_init.h>
	#include <tbb/parallel_for_each.h>
	#include <boost/bind.hpp>
	#include <functional>
#endif SERV_ROOM_MANAGER_PARALLEL_FOR_EACH
//}}

//{{ 2013. 2. 5	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	#include "InteriorUdpSession.h"
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	#include "WeddingHall.h"
#endif SERV_RELATIONSHIP_SYSTEM
//}

NiImplementRTTI( KCnRoomManager, KRoomManager );

ImplToStringW( KCnRoomManager )
{
	stm_ << L"--------------- Room Manager -------------------" << std::endl;
	stm_ << TOSTRINGW( GetNumEmptyRoom( CXSLRoom::RT_PVP ) );
	stm_ << TOSTRINGW( GetNumEmptyRoom( CXSLRoom::RT_DUNGEON ) );
	stm_ << TOSTRINGW( GetNumEmptyRoom( CXSLRoom::RT_SQUARE ) );
	stm_ << TOSTRINGW( GetNumEmptyRoom( CXSLRoom::RT_TRADE ) );
	stm_ << TOSTRINGW( GetNumEmptyRoom( CXSLRoom::RT_TRAININGCENTER ) );
	stm_ << TOSTRINGW( GetNumEmptyRoom( CXSLRoom::RT_PERSONAL_SHOP ) );
	//{{ 2011. 10. 27	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	stm_ << TOSTRINGW( GetNumEmptyRoom( CXSLRoom::RT_BATTLE_FIELD ) );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	stm_ << TOSTRINGW( GetNumEmptyRoom( CXSLRoom::RT_WEDDING_HALL ) );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

	KTickDumpInfo kInfo;
	m_kRoomManagerTickDump.GetDump( kInfo );

	stm_ << L"-------------- Tick Dump --------------------" << std::endl;
	stm_ << L"MinTick : "		 << kInfo.m_dwMinTick << std::endl;
	stm_ << L"MinTickRegDate : " << kInfo.m_wstrMinTickRegDate << std::endl;
	stm_ << L"MaxTick : "		 << kInfo.m_dwMaxTick << std::endl;
	stm_ << L"MaxTickRegDate : " << kInfo.m_wstrMaxTickRegDate << std::endl;
	stm_ << L"AverageTick : "	 << kInfo.m_iAverageTickCount << std::endl;
	stm_ << L"TotalTickCount : " << kInfo.m_iTotalTickCount << std::endl;
	stm_ << L"PassedTickCount : " << kInfo.m_iPassedTickCount << std::endl;

	//{{ 2010. 8. 29	������	�̺�Ʈ ť ������ ����
#ifdef SERV_MAX_QUEUE_SIZE_DUMP
	stm_ << L"-------------- RoomManager Max Queue Size Dump --------------------" << std::endl;
	m_kRMEventProcess.ToString( stm_ );

	stm_ << L"-------------- Room Max Queue Size Dump --------------------" << std::endl;

	UidType iRoomUID = 0;
	KPerformer::KMaxQueueSize kMaxQueueSizeDump;
	for( int iRoomType = 0; iRoomType < CXSLRoom::RT_TOTAL_NUM; ++iRoomType )
	{
		std::map< UidType, KCnBaseRoomPtr >::const_iterator mit;
		for( mit = m_mapRoomList[iRoomType].begin(); mit != m_mapRoomList[iRoomType].end(); ++mit )
		{
			// ����Ʈ �����ͷ� ����
			KCnBaseRoomPtr spRoom = mit->second;
			if( spRoom == NULL )
				continue;

            KPerformer::KMaxQueueSize kMaxQueueSize = spRoom->GetMaxQueueSize();
			if( kMaxQueueSize.m_QueueSize > kMaxQueueSizeDump.m_QueueSize )
			{
                kMaxQueueSizeDump = kMaxQueueSize;
				iRoomUID = spRoom->GetUID();
			}
		}
	}

	stm_ << TOSTRINGW( iRoomUID );
	stm_ << TOSTRINGW( CXSLRoom::GetRoomType( iRoomUID ) );
	stm_ << TOSTRINGW( kMaxQueueSizeDump.m_QueueSize );
	stm_ << TOSTRINGW( kMaxQueueSizeDump.GetMaxQueueSizeRegDate() );
    
#endif SERV_MAX_QUEUE_SIZE_DUMP
	//}}

	//{{ 2011. 03. 25	������	���� ���� üũ
#ifdef SERV_ROOM_TICK_DUMP
	{
		for( int iRoomType = 0; iRoomType < CXSLRoom::RT_TOTAL_NUM; ++iRoomType )
		{
			KTickDumpInfo kInfo;
			m_kRoomTickDump.GetDump( iRoomType, kInfo );

			stm_ << L"-------------- Tick Dump --------------------" << std::endl;
			stm_ << L"RoomType : "		 << iRoomType << std::endl;
			stm_ << L"MaxTick : "		 << kInfo.m_dwMaxTick << std::endl;
			stm_ << L"MaxTickRegDate : " << kInfo.m_wstrMaxTickRegDate << std::endl;
			stm_ << L"AverageTick : "	 << kInfo.m_iAverageTickCount << std::endl;
		}
	}
#endif SERV_ROOM_TICK_DUMP
	//}}

	return stm_;
}

KCnRoomManager::KCnRoomManager()
:
//{{ 2012. 09. 08	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
m_dfBattleFieldZUTickTime( 20.0 ),
m_dfBattleFIeldZUEchoTime( 10.0 ),
#endif SERV_BATTLE_FIELD_SYSTEM
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
m_dfBattleFieldZombieAlertTickTime( 20.0 ),
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
//}}
m_dfZUTickTime( 0.0 )
{
	int i;
	for( i = 0; i < CXSLRoom::RT_TOTAL_NUM; i++ )
	{
		m_dwNumRoom[i] = 0;
	}
}

KCnRoomManager::~KCnRoomManager()
{
}

void KCnRoomManager::Run()
{
	DWORD ret;

	//{{ 2012. 04. 18	������	�� �Ŵ��� �����ս� ���
#ifdef SERV_ROOM_MANAGER_PARALLEL_FOR_EACH
	tbb::task_scheduler_init tbbInit; // [����] ����Ǵ� �����忡�� �ʱ�ȭ �ؾ���!
#endif SERV_ROOM_MANAGER_PARALLEL_FOR_EACH
	//}}

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 20 );       // 0.02s ����

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Tick();

		else std::cout << "*** WaitForSingleObject() - return : " << ret << std::endl;

	}

	return;
}

bool KCnRoomManager::Init()
{
	//{{ 2011. 05. 11	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	if( GetKCnServer()->GetProxyID() >= SEnum::PSHOP_AGENCY_PROXY_ID )
	{
        START_LOG( cerr, L"�븮 ������ ���� ProxyID���� ħ���Ͽ����ϴ�! ������ �ֽ��ϴ�! �� �ذ��ϼ���!!!!!" )
			<< BUILD_LOG( GetKCnServer()->GetProxyID() )
			<< BUILD_LOG( SEnum::PSHOP_AGENCY_PROXY_ID )
			<< END_LOG;
		return false;
	}
#endif SERV_PSHOP_AGENCY
	//}}

	KNetAddress kAddress;
	kAddress.m_wstrIP = NetCommon::GetLocalIPW();
	kAddress.m_usPort = KNetLayer::GetKObj()->GetNCUDPPort();

	int iRoomType;
	for( iRoomType = 0; iRoomType < CXSLRoom::RT_TOTAL_NUM; ++iRoomType )
	{
		//{{ 2011. 05. 11	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
		if( iRoomType == CXSLRoom::RT_PSHOP_AGENCY )
			continue;
#endif SERV_PSHOP_AGENCY
		//}}

		m_spRoomFSM[iRoomType] = CreateRoomFSM( iRoomType );

		UidType iIndex;
		for( iIndex = 1; iIndex <= m_dwNumRoom[iRoomType]; iIndex++ )
		{
			UidType iRoomUID = 0;
			KCnBaseRoomPtr spRoom( CreateRoom( iRoomType ) );

			KncUid::SetPureUID( iRoomUID, static_cast<UidType>(iIndex) );
			KncUid::SetServerID( iRoomUID, GetKCnServer()->GetProxyID() );
			//{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
			KncUid::SetServerGroupID( iRoomUID, GetKCnServer()->GetServerGroupID() );
#endif SERV_INTEGRATION
			//}}
			KncUid::SetReservedID( iRoomUID, iRoomType );
			spRoom->SetUID( iRoomUID );
			spRoom->SetFSM( m_spRoomFSM[iRoomType] );
			spRoom->SetUDPRelayInfo( kAddress );

			// ���⼭�� map�� key�ߺ� �˻縦 �� �ʿ䰡 ���� �� ����.
			m_mapRoomList[iRoomType][iRoomUID] = spRoom;
			//{{ 2012. 04. 18	������	�� �Ŵ��� �����ս� ���
#ifdef SERV_ROOM_MANAGER_PARALLEL_FOR_EACH
			m_vecRoomList[iRoomType].push_back( spRoom );
#endif SERV_ROOM_MANAGER_PARALLEL_FOR_EACH
			//}}
			m_mapEmptyRoomList[iRoomType][iRoomUID] = iRoomUID;
		}
	}

	// ������ ���õ� ���� ������ ������ �����Ѵ�.
	BOOST_TEST_FOREACH( const KSquareCreateInfo&, kInfo, m_vecSquareCreateInfo )
	{
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CN_ROOM, NULL, ERM_OPEN_SQUARE_NOT, kInfo );
		spEvent->m_kDestPerformer.AddUID( 0 );
		QueueingEventToRoom( spEvent );
	}

	return KThread::Begin();
}

void KCnRoomManager::Tick()
{
	for( int iRoomType = 0; iRoomType < CXSLRoom::RT_TOTAL_NUM; ++iRoomType )
	{
		//////////////////////////////////////////////////////////////////////////
		//{{ 2011. 03. 24	������	���� ���� üũ
#ifdef SERV_ROOM_TICK_DUMP
		const DWORD dwLastTickCount = ::GetTickCount();
#endif SERV_ROOM_TICK_DUMP
		//}}		
		//////////////////////////////////////////////////////////////////////////		

		//{{ 2012. 04. 18	������	�� �Ŵ��� �����ս� ���
#ifdef SERV_ROOM_MANAGER_PARALLEL_FOR_EACH
		tbb::parallel_for_each( m_vecRoomList[iRoomType].begin(), m_vecRoomList[iRoomType].end(), boost::bind( &KCnBaseRoom::Tick, _1 ) );
		//std::for_each( m_vecRoomList[iRoomType].begin(), m_vecRoomList[iRoomType].end(), boost::bind( &KCnBaseRoom::Tick, _1 ) );
		//std::for_each( m_vecRoomList[iRoomType].begin(), m_vecRoomList[iRoomType].end(), std::mem_fun( &KCnBaseRoom::Tick ) ); // ����Ʈ �����Ͱ� ��� �����̳ʸ� ����� ���Ѵ�.
#else
		std::map< UidType, KCnBaseRoomPtr >::iterator mit;
		for( mit = m_mapRoomList[iRoomType].begin(); mit != m_mapRoomList[iRoomType].end(); ++mit )
		{
			if( mit->second )
			{
				mit->second->Tick();
			}
			else
			{
				START_LOG( cerr, L"�� ������ �̻�." )
					<< END_LOG;
			}
		}
#endif SERV_ROOM_MANAGER_PARALLEL_FOR_EACH
		//}}

		//////////////////////////////////////////////////////////////////////////
		//{{ 2011. 03. 24	������	���� ���� üũ
#ifdef SERV_ROOM_TICK_DUMP
		const DWORD dwRoomTypeTick = ::GetTickCount() - dwLastTickCount;
		m_kRoomTickDump.CheckOneTickLatency( iRoomType, dwRoomTypeTick );
#endif SERV_ROOM_TICK_DUMP
		//}}
		//////////////////////////////////////////////////////////////////////////
	}

	//{{ 2010. 02. 17  ������	ä����Ƽ����
#ifdef SERV_CHANNEL_PARTY
	SiKCnPartyManager()->Tick();
#endif SERV_CHANNEL_PARTY
	//}}

	//{{ 2010. 03. 08  ������	TickDumper
#ifdef SERV_ROOM_TICK
	m_kRoomManagerTickDump.CheckOneTickLatency();
#endif SERV_ROOM_TICK
	//}}
	
	//{{ 2010. 06. 17  ������	��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_REALTIME_SCRIPT
	m_kRMEventProcess.Tick();
#endif SERV_REALTIME_SCRIPT
	//}}

	//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
	// �̺�Ʈ �Ŵ���
	SiKGameEventManager()->Tick();
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}

	//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	SiKReleaseTickManager()->Tick();
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}

	//{{ 2008. 10. 31  ������	����� ����Ʈ ����
	SiKAbuserLogManager()->Tick();
	//}}

	//{{ 2011. 01. 26	������	���� Ŭ���� Ÿ�� ������ üũ
#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
	SiKCnHackingCheckManager()->Tick();
#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
	//}}
	//{{ 2013. 2. 5	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	KInteriorUdpSession::GetKObj()->SendAndFlush();
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}
}

#include <KncLua.h>
#include <lua_tinker.h>

void KCnRoomManager::RegToLua()
{
	//���ͼ����� ���ð��� ������(Set~~Func)�� config_cn.lua �� ���� �޴´�.
	lua_tinker::class_add<KCnRoomManager>( g_pLua, "KCnRoomManager" );
	lua_tinker::class_def<KCnRoomManager>( g_pLua, "dump",						&KCnRoomManager::Dump );
	lua_tinker::class_def<KCnRoomManager>( g_pLua, "SetNumPvPRoom",				&KCnRoomManager::SetNumPvPRoom );
	lua_tinker::class_def<KCnRoomManager>( g_pLua, "SetNumDungeonRoom",			&KCnRoomManager::SetNumDungeonRoom );
	lua_tinker::class_def<KCnRoomManager>( g_pLua, "SetNumSquare",				&KCnRoomManager::SetNumSquare );	
	lua_tinker::class_def<KCnRoomManager>( g_pLua, "SetNumTrade",				&KCnRoomManager::SetNumTrade );
	lua_tinker::class_def<KCnRoomManager>( g_pLua, "SetNumTrainingCenterRoom",	&KCnRoomManager::SetNumTrainingCenterRoom );
	lua_tinker::class_def<KCnRoomManager>( g_pLua, "SetNumPersonalShop",		&KCnRoomManager::SetNumPersonalShop );
	//{{ 2011. 10. 27	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	lua_tinker::class_def<KCnRoomManager>( g_pLua, "SetNumBattleField",			&KCnRoomManager::SetNumBattleField );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	lua_tinker::class_def<KCnRoomManager>( g_pLua, "SetNumWeddingHall",			&KCnRoomManager::SetNumWeddingHall );
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	lua_tinker::class_def<KCnRoomManager>( g_pLua, "OpenSquare",				&KCnRoomManager::OpenSquare_Lua );
	lua_tinker::class_def<KCnRoomManager>( g_pLua, "SetZUTickTime",				&KCnRoomManager::SetZUTickTime );
	//{{ 2011. 12. 12	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	lua_tinker::class_def<KCnRoomManager>( g_pLua, "SetBattleFieldZUTickTime",	&KCnRoomManager::SetBattleFieldZUTickTime );
	lua_tinker::class_def<KCnRoomManager>( g_pLua, "dumpMonster",				&KCnRoomManager::DumpRoomMonster_LUA );
	lua_tinker::class_def<KCnRoomManager>( g_pLua, "dumpBattleField",			&KCnRoomManager::DumpBattleField_LUA );
#endif SERV_BATTLE_FIELD_SYSTEM
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    lua_tinker::class_def<KCnRoomManager>( g_pLua, "SetBattleFieldZombieAlertTickTime",	&KCnRoomManager::SetBattleFieldZombieAlertTickTime );
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
	//}}
	//{{ 2012. 11. 20	������		���� ��Ʋ�ʵ� üũ �� ����
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	lua_tinker::class_def<KCnRoomManager>( g_pLua, "KillZombieUser",			&KCnRoomManager::KillZombieUser_LUA );
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	//}}

	lua_tinker::decl( g_pLua, "RoomManager", this );
}

void KCnRoomManager::ShutDown()
{
	// �α� �����
	std::wstringstream wstrDump;
	ToString( wstrDump );
	dbg::cout << wstrDump.str() << dbg::endl;

	KRoomManager::ShutDown();
}

void KCnRoomManager::SetNumRoom( int iRoomType, DWORD dwNumRoom )
{
	if( iRoomType < 0 || iRoomType >= CXSLRoom::RT_TOTAL_NUM )
	{
		START_LOG( cerr, L"�� Ÿ���� �̻���." )
			<< BUILD_LOG( iRoomType )
			<< BUILD_LOG( ( int )CXSLRoom::RT_TOTAL_NUM )
			<< END_LOG;

		return;
	}

	m_dwNumRoom[iRoomType] = dwNumRoom;
}

void KCnRoomManager::SetNumPvPRoom( DWORD dwNumRoom )
{
	SetNumRoom( CXSLRoom::RT_PVP, dwNumRoom );
}

void KCnRoomManager::SetNumDungeonRoom( DWORD dwNumRoom )
{
	SetNumRoom( CXSLRoom::RT_DUNGEON, dwNumRoom );
}

void KCnRoomManager::SetNumSquare( DWORD dwNumRoom )
{
	SetNumRoom( CXSLRoom::RT_SQUARE, dwNumRoom );
}

void KCnRoomManager::SetNumTrade( DWORD dwNumRoom )
{
	SetNumRoom( CXSLRoom::RT_TRADE, dwNumRoom );
}

void KCnRoomManager::SetNumTrainingCenterRoom( DWORD dwNumRoom )
{
	SetNumRoom( CXSLRoom::RT_TRAININGCENTER, dwNumRoom );
}

void KCnRoomManager::SetNumPersonalShop( DWORD dwNumRoom )
{
	SetNumRoom( CXSLRoom::RT_PERSONAL_SHOP, dwNumRoom );
}

//{{ 2011. 10. 27	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnRoomManager::SetNumBattleField( DWORD dwNumRoom )
{
	SetNumRoom( CXSLRoom::RT_BATTLE_FIELD, dwNumRoom );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
void KCnRoomManager::SetNumWeddingHall( DWORD dwNumRoom )
{
	SetNumRoom( CXSLRoom::RT_WEDDING_HALL, dwNumRoom );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}

void KCnRoomManager::PrepareSquare( std::vector< KSquareCreateInfo >& vecSquareCreateInfo )
{
	m_vecSquareCreateInfo = vecSquareCreateInfo;
}

DWORD KCnRoomManager::GetNumEmptyRoom( int iRoomType ) const
{
	if( iRoomType < 0 || iRoomType >= CXSLRoom::RT_TOTAL_NUM )
	{
		START_LOG( cerr, L"�� Ÿ���� �̻���." )
			<< BUILD_LOG( iRoomType )
			<< BUILD_LOG( ( int )CXSLRoom::RT_TOTAL_NUM )
			<< END_LOG;

		return 0;
	}

	KLocker lock( m_csEmptyRoomList[iRoomType] );

	return ( DWORD )m_mapEmptyRoomList[iRoomType].size();
}

UidType KCnRoomManager::OpenRoom( CXSLRoom::ROOM_TYPE eRoomType )
{
	KLocker lock( m_csEmptyRoomList[eRoomType] );

	if( m_mapEmptyRoomList[eRoomType].empty() )
	{
		START_LOG( cerr, L"�� ���� ����." )
			<< BUILD_LOG( eRoomType )
			<< END_LOG;

		return 0;
	}

	std::map< UidType, UidType >::iterator mit;
	for( mit = m_mapEmptyRoomList[eRoomType].begin(); mit != m_mapEmptyRoomList[eRoomType].end(); ++mit )
	{
		KCnBaseRoomPtr spRoom = Get( mit->first );
		if( spRoom == NULL )
		{
			START_LOG( cerr, L"�� �� �ε����� �̻���." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
		}
		else
		{
			// ��� �� FSM ���� INIT, CLOSE, WAIT �� ������ �̵��� ���� ����. 
			if( spRoom->GetStateID() == KBaseRoomFSM::S_INIT )
			{
				break;
			}
			else
			{
				START_LOG( cerr, L"�� �� ����Ʈ�� ������ �� ���°� �̻���." )
					<< BUILD_LOG( spRoom->GetStateID() )
					<< BUILD_LOG( spRoom->GetStateIDString() )
					<< END_LOG;
			}
		}
	}

	if( mit == m_mapEmptyRoomList[eRoomType].end() )
	{
		START_LOG( cerr, L"�� ���� ã�� �� ����." )
			<< END_LOG;

		return 0;
	}

	UidType iRoomUID = mit->first;
	m_mapEmptyRoomList[eRoomType].erase( mit );

	return iRoomUID;
}

bool KCnRoomManager::CloseRoom( UidType iRoomUID )
{
	const CXSLRoom::ROOM_TYPE eRoomType = ( CXSLRoom::ROOM_TYPE )KncUid::ExtractReservedID( iRoomUID );
	if( eRoomType < 0 || eRoomType >= CXSLRoom::RT_TOTAL_NUM )
	{
		START_LOG( cerr, L"�� Ÿ���� �̻���." )
			<< BUILD_LOG( eRoomType )
			<< BUILD_LOG( ( int )CXSLRoom::RT_TOTAL_NUM )
			<< END_LOG;

		return false;
	}

	KLocker lock( m_csEmptyRoomList[eRoomType] );

	std::map< UidType, UidType >::iterator mit;
	mit = m_mapEmptyRoomList[eRoomType].find( iRoomUID );
	if( mit != m_mapEmptyRoomList[eRoomType].end() )
	{
		START_LOG( cerr, L"�� �渮��Ʈ�� �������� ID�� ���� �̹� ����." )
			<< BUILD_LOG( iRoomUID )
			<< END_LOG;

		return false;
	}

	KCnBaseRoomPtr spRoom = Get( iRoomUID );
	if( !spRoom )
	{
		START_LOG( cerr, L"�ش� UID�� ���� ����." )
			<< BUILD_LOG( iRoomUID )
			<< END_LOG;

		return false;
	}

	// ��� �� FSM ���� INIT, CLOSE, WAIT �� ������ �̵��� ���� ����.
	if( spRoom->GetStateID() != KBaseRoomFSM::S_CLOSE )
	{
		START_LOG( cerr, L"�� ���� �̻�." )
			<< BUILD_LOG( spRoom->GetStateID() )
			<< BUILD_LOG( iRoomUID )
			<< END_LOG;

		return false;
	}

	// ��� �� FSM ���� INIT, CLOSE, WAIT �� ������ �̵��� ���� ����.
	spRoom->StateTransition( KBaseRoomFSM::I_TO_INIT );
	m_mapEmptyRoomList[eRoomType].insert( std::make_pair( iRoomUID, iRoomUID ) );

	return true;
}

//{{ 2010. 06. 17  ������	������ ����
void KCnRoomManager::QueueingEventToRoomManager( const KEventPtr& spEvent_ )
{
	m_kRMEventProcess.QueueingEvent( spEvent_ );
}
//}}

void KCnRoomManager::QueueingEventToRoom( const KEventPtr& spEvent_ )
{
	UidType iRoomUID = 0;
	switch( spEvent_->m_usEventID )
	{
	case ERM_OPEN_PVP_ROOM_REQ:
		//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	case ERM_OPEN_PVP_ROOM_FOR_MATCH_REQ:
#endif SERV_PVP_NEW_SYSTEM
		//}}
		iRoomUID = OpenRoom( CXSLRoom::RT_PVP );
		break;
	case ERM_OPEN_DUNGEON_ROOM_REQ:
	case ERM_PARTY_OPEN_DUNGEON_ROOM_REQ:
		//{{ 2011. 12. 09	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	case ERM_SINGLE_OPEN_DUNGEON_ROOM_REQ:
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		iRoomUID = OpenRoom( CXSLRoom::RT_DUNGEON );
		break;
	case ERM_OPEN_SQUARE_NOT:
		iRoomUID = OpenRoom( CXSLRoom::RT_SQUARE );
		break;
	case ERM_OPEN_TRADE_REQ:
		iRoomUID = OpenRoom( CXSLRoom::RT_TRADE );
		break;
	case ERM_OPEN_TC_ROOM_REQ:
		iRoomUID = OpenRoom( CXSLRoom::RT_TRAININGCENTER );
		break;
	case ERM_OPEN_PERSONAL_SHOP_REQ:
		iRoomUID = OpenRoom( CXSLRoom::RT_PERSONAL_SHOP );
		break;
		//{{ 2011. 10. 27	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	case ERM_OPEN_BATTLE_FIELD_REQ:
		iRoomUID = OpenRoom( CXSLRoom::RT_BATTLE_FIELD );
		break;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	case ERM_OPEN_WEDDING_HALL_REQ:
		iRoomUID = OpenRoom( CXSLRoom::RT_WEDDING_HALL );
		break;
#endif SERV_RELATIONSHIP_SYSTEM
		//}
	default:
		// �� �ϳ����� �̺�Ʈ�� ���޵ȴٰ� ������. GetFirstUID
		iRoomUID = spEvent_->m_kDestPerformer.GetFirstUID();
		break;
	}

	if( iRoomUID <= 0 )
	{
		switch( spEvent_->m_usEventID )
		{
		case ERM_OPEN_PVP_ROOM_REQ:
		case ERM_OPEN_DUNGEON_ROOM_REQ:
		case ERM_PARTY_OPEN_DUNGEON_ROOM_REQ:
			{
				KEGS_CREATE_ROOM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_ROOM_00;
				SendToGSCharacter( spEvent_->GetLastSenderUID(), spEvent_->GetFirstSenderUID(), ERM_OPEN_ROOM_ACK, kPacket );
				return;
			}

			//{{ 2011. 12. 09	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case ERM_SINGLE_OPEN_DUNGEON_ROOM_REQ:
			{
				KEGS_CREATE_ROOM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_ROOM_00;
				SendToGSCharacter( spEvent_->GetLastSenderUID(), spEvent_->GetFirstSenderUID(), ERM_SINGLE_OPEN_DUNGEON_ROOM_ACK, kPacket );
				return;
			}
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}

			//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		case ERM_OPEN_PVP_ROOM_FOR_MATCH_REQ:
			{
				KEGB_OPEN_PVP_ROOM_FOR_MATCH_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_ROOM_00;
				SendToGSCharacter( spEvent_->GetLastSenderUID(), spEvent_->GetFirstSenderUID(), ERM_OPEN_PVP_ROOM_FOR_MATCH_ACK, kPacket );
				return;
			}			
#endif SERV_PVP_NEW_SYSTEM
			//}}

		case ERM_OPEN_TC_ROOM_REQ:
			{
				KEGS_CREATE_TC_ROOM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_ROOM_00;
				SendToGSCharacter( spEvent_->GetLastSenderUID(), spEvent_->GetFirstSenderUID(), ERM_OPEN_TC_ROOM_ACK, kPacket );
				return;
			}

		case ERM_OPEN_SQUARE_NOT:
			{
				START_LOG( cerr, L"���� ���� ����." )
					<< BUILD_LOG( iRoomUID )
					<< END_LOG;
				return;
			}
		case ERM_OPEN_TRADE_REQ:
			{
				KERM_OPEN_TRADE_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_TRADE_00;
				SendToGSCharacter( spEvent_->GetLastSenderUID(), spEvent_->GetFirstSenderUID(), ERM_OPEN_TRADE_ACK, kPacket );
				return;
			}

		case ERM_OPEN_PERSONAL_SHOP_REQ:
			{
				KERM_OPEN_PERSONAL_SHOP_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_00;
				SendToGSCharacter( spEvent_->GetLastSenderUID(), spEvent_->GetFirstSenderUID(), ERM_OPEN_PERSONAL_SHOP_ACK, kPacket );
				return;
			}

			//{{ 2011. 10. 27	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case ERM_OPEN_BATTLE_FIELD_REQ:
			{
				KERM_OPEN_BATTLE_FIELD_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_ROOM_00;
				SendToGSCharacter( spEvent_->GetLastSenderUID(), spEvent_->GetFirstSenderUID(), ERM_OPEN_BATTLE_FIELD_ACK, kPacket );
				return;
			}
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
			case ERM_OPEN_WEDDING_HALL_REQ:
				{
					KERM_OPEN_WEDDING_HALL_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_ROOM_00;
					SendToGSCharacter( spEvent_->GetLastSenderUID(), spEvent_->GetFirstSenderUID(), ERM_OPEN_WEDDING_HALL_ACK, kPacket );
					return;
				}
#endif SERV_RELATIONSHIP_SYSTEM
			//}
		default:
			START_LOG( cerr, L"�̺�Ʈ�� ��ǥ �� UID�� �̻���." )
				<< BUILD_LOG( iRoomUID )
				<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
				<< END_LOG;
			return;
		}
	}

	KCnBaseRoomPtr spRoom = Get( iRoomUID );
	if( !spRoom )
	{
		START_LOG( cwarn, L"�� �˻� ����." )
			<< BUILD_LOG( iRoomUID )
			<< END_LOG;

		return;
	}

	spRoom->QueueingEvent( spEvent_ );
}

void KCnRoomManager::QueueingToAll( int iRoomType, const KEventPtr& spEvent_ )
{
	// �� �濡�� ť������ �ʴ´�.
	std::map< UidType, KCnBaseRoomPtr >::iterator mitRoom;
	std::map< UidType, UidType >::iterator mitEmptyRoom;

	KLocker lock( m_csEmptyRoomList[iRoomType] );
	for( mitRoom = m_mapRoomList[iRoomType].begin(); mitRoom != m_mapRoomList[iRoomType].end(); mitRoom++ )
	{
		mitEmptyRoom = m_mapEmptyRoomList[iRoomType].find( mitRoom->first );
		if( mitEmptyRoom == m_mapEmptyRoomList[iRoomType].end() )
		{
			mitRoom->second->QueueingEvent( spEvent_ );
		}
	}
}

void KCnRoomManager::QueueingToAll( const KEventPtr& spEvent_ )
{
	// ���� ���� �ִ� ��� �濡 �̺�Ʈ�� ť���Ѵ�. �� �濡�� ť������ �ʴ´�.
	int i;
	for( i = 0; i < CXSLRoom::RT_TOTAL_NUM; i++ )
	{
		QueueingToAll( i, spEvent_ );
	}
}

void KCnRoomManager::OpenSquare_Lua( int iSquareType, char* szSquareName )
{
	if( iSquareType <= CXSLSquareGame::ST_NONE || iSquareType >= CXSLSquareGame::ST_END )
	{
		START_LOG( cout2, L"���� Ÿ���� �߸��Ǿ���.!" )
			<< BUILD_LOG( iSquareType )
			<< END_LOG;

		return;
	}

	if( strlen(szSquareName) <= 1 )
	{
		START_LOG( cout2, L"�����̸��� ª���ϴ�." )
			<< BUILD_LOG( szSquareName )
			<< END_LOG;

		return;
	}


	KSquareCreateInfo kInfo;
	kInfo.m_iSquareType		= iSquareType;
	kInfo.m_wstrSquareName	= KncUtil::toWideString( szSquareName, strlen(szSquareName) );

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_ROOM, NULL, ERM_OPEN_SQUARE_NOT, kInfo );
	spEvent->m_kDestPerformer.AddUID( 0 );
	QueueingEventToRoom( spEvent );

	START_LOG( cout2, L"::: ��� ���� ����.!! :::" )
		<< BUILD_LOG( iSquareType )
		<< BUILD_LOG( szSquareName )
		;
}

void KCnRoomManager::SetZUTickTime( double dfTickTime )
{
	m_dfZUTickTime = dfTickTime;

	START_LOG( cout2, L"Zombie User üũ �ð�����.!" )
		<< BUILD_LOG( m_dfZUTickTime );
}

double KCnRoomManager::GetZUTickTime()
{
	if( m_dfZUTickTime <= 0.0 || m_dfZUTickTime >= 600.0 )
	{
		return 180.0;
	}

	return m_dfZUTickTime;
}

//{{ 2012. 09. 08	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnRoomManager::SetBattleFieldZUTickTime( double dfTickTime )
{
	if( dfTickTime <= 0.0 || dfTickTime >= 60.0 )
	{
		START_LOG( cerr, L"��Ʋ�ʵ� ���� ���� üũ Ÿ���� �� ������ �ʹ� ũ��! �����Ѵ�!" )
			<< BUILD_LOG( dfTickTime )
			<< END_LOG;

		dfTickTime = 30.0;
	}

	m_dfBattleFieldZUTickTime = dfTickTime;
	m_dfBattleFIeldZUEchoTime = dfTickTime / 2;

	START_LOG( cout2, L"BattleField Zombie User üũ �ð�����.!" )
		<< BUILD_LOG( m_dfBattleFieldZUTickTime )
		<< BUILD_LOG( m_dfBattleFIeldZUEchoTime );
}



#endif SERV_BATTLE_FIELD_SYSTEM
//}}


#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
void KCnRoomManager::SetBattleFieldZombieAlertTickTime( double dfTickTime )
{
	if( dfTickTime <= 20.0 || dfTickTime >= 60.0 )
	{
		START_LOG( cerr, L"��Ʋ�ʵ� ���� ���� üũ Ÿ���� �� ������ �ʹ� ũ��! �����Ѵ�!" )
			<< BUILD_LOG( dfTickTime )
			<< END_LOG;

		dfTickTime = 20.0;
	}

	m_dfBattleFieldZombieAlertTickTime = dfTickTime;

	START_LOG( cout2, L"BattleField Zombie Alert üũ �ð�����.!" )
		<< BUILD_LOG( m_dfBattleFieldZombieAlertTickTime );
}
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

//{{ 2011. 12. 12	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnRoomManager::DumpRoomMonster_LUA( const char* pUserNickName, int iNpcUID )
{
	START_LOG( cout, L"�� ���� ã�� ���!" )
		<< BUILD_LOG( pUserNickName )
		<< BUILD_LOG( iNpcUID );

	KERM_DUMP_ROOM_MONSTER_NOT kNot;
	kNot.m_wstrNickName = KncUtil::toWideString( pUserNickName );
	kNot.m_iNpcUID = iNpcUID;

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_NULL, NULL, ERM_DUMP_ROOM_MONSTER_NOT, kNot );    
	QueueingToAll( CXSLRoom::RT_BATTLE_FIELD, spEvent->Clone() );
}

void KCnRoomManager::DumpBattleField_LUA( const char* pUserNickName )
{
	START_LOG( cout, L"��Ʋ�ʵ� ����!" )
		<< BUILD_LOG( pUserNickName );

	KERM_DUMP_BATTLE_FIELD_NOT kNot;
	kNot.m_wstrNickName = KncUtil::toWideString( pUserNickName );

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_NULL, NULL, ERM_DUMP_BATTLE_FIELD_NOT, kNot );    
	QueueingToAll( CXSLRoom::RT_BATTLE_FIELD, spEvent->Clone() );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 11. 20	������		���� ��Ʋ�ʵ� üũ �� ����
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
void KCnRoomManager::KillZombieUser_LUA( const char* pUserNickName )
{
	START_LOG( cout, L"���� ���� ų!" )
		<< BUILD_LOG( pUserNickName );

	KERM_KILL_ZOMBIE_USER_IN_BATTLE_FIELD_NOT kNot;
	kNot.m_wstrNickName = KncUtil::toWideString( pUserNickName );

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_NULL, NULL, ERM_KILL_ZOMBIE_USER_IN_BATTLE_FIELD_NOT, kNot );
	QueueingToAll( CXSLRoom::RT_BATTLE_FIELD, spEvent->Clone() );
}
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
//}}

void KCnRoomManager::SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID )
{
	SendToGSCharacter( nGS, nTo, usEventID, char() );
}


FSMclassPtr KCnRoomManager::CreateRoomFSM( int iRoomType )
{
	switch( iRoomType )
	{
	case CXSLRoom::RT_PVP:				return boost::shared_ptr< KRoomFSM >( new KRoomFSM );
	case CXSLRoom::RT_DUNGEON:			return boost::shared_ptr< KRoomFSM >( new KRoomFSM );
	case CXSLRoom::RT_TRAININGCENTER:	return boost::shared_ptr< KRoomFSM >( new KRoomFSM );
	case CXSLRoom::RT_SQUARE:			return boost::shared_ptr< KBaseRoomFSM >( new KBaseRoomFSM );
	case CXSLRoom::RT_TRADE:			return boost::shared_ptr< KTradeFSM >( new KTradeFSM );
	case CXSLRoom::RT_PERSONAL_SHOP:	return boost::shared_ptr< KPersonalShopFSM >( new KPersonalShopFSM );
		//{{ 2011. 10. 27	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	case CXSLRoom::RT_BATTLE_FIELD:		return boost::shared_ptr< KRoomFSM >( new KRoomFSM );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	case CXSLRoom::RT_WEDDING_HALL:		return boost::shared_ptr< KRoomFSM >( new KRoomFSM );
#endif SERV_RELATIONSHIP_SYSTEM
		//}

	default:
		START_LOG( cerr, L"�� Ÿ���� �̻���." )
			<< BUILD_LOG( iRoomType )
			<< END_LOG;
	}

	return FSMclassPtr();
}

KCnBaseRoomPtr KCnRoomManager::CreateRoom( int iRoomType )
{
	// TODO : ����, ��Ƽ ó��.

	switch( iRoomType )
	{
	case CXSLRoom::RT_PVP:				return KPvPRoomPtr( new KPvPRoom );
	case CXSLRoom::RT_DUNGEON:			return KDungeonRoomPtr( new KDungeonRoom );
	case CXSLRoom::RT_TRAININGCENTER:	return KTrainingCenterRoomPtr( new KTrainingCenterRoom );
	case CXSLRoom::RT_SQUARE:			return KSquarePtr( new KSquare );
	case CXSLRoom::RT_TRADE:			return KTradePtr( new KTrade );			
	case CXSLRoom::RT_PERSONAL_SHOP:	return KPersonalShopPtr( new KPersonalShop );
		//{{ 2011. 10. 27	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	case CXSLRoom::RT_BATTLE_FIELD:		return KBattleFieldRoomPtr( new KBattleFieldRoom );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	case CXSLRoom::RT_WEDDING_HALL:		return KWeddingHallPtr( new KWeddingHall );
#endif SERV_RELATIONSHIP_SYSTEM
		//}

	default:
		START_LOG( cerr, L"�� Ÿ���� �̻���." ) << BUILD_LOG( iRoomType ) << END_LOG;
	}

	return KCnBaseRoomPtr();
}

KCnBaseRoomPtr KCnRoomManager::Get( UidType iRoomUID )
{
	int iRoomType = ( int )KncUid::ExtractReservedID( iRoomUID );
	if( iRoomType < 0 || iRoomType >= CXSLRoom::RT_TOTAL_NUM )
	{
		START_LOG( cerr, L"�� Ÿ���� �̻���." )
			<< BUILD_LOG( iRoomUID )
			<< BUILD_LOG( iRoomType )
			<< END_LOG;

		return KCnBaseRoomPtr();
	}

	std::map< UidType, KCnBaseRoomPtr >::iterator mit;
	mit = m_mapRoomList[iRoomType].find( iRoomUID );
	if( mit == m_mapRoomList[iRoomType].end() )
	{
		return KCnBaseRoomPtr();
	}

	return mit->second;
}
