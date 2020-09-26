#include "LoginRoomManager.h"

#include "Room/BaseRoomFSM.h"
#include "Room/RoomFSM.h"
#include "Room/TradeFSM.h"
#include "Room/PersonalShopFSM.h"

//{{ 2011. 04. 06	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY	
	#include "PersonalShopAgency.h"
#endif SERV_PSHOP_AGENCY
//}}
#include "NetError.h"
#include "Enum/Enum.h"

#include "LoginServer.h"

#include "socket/NetCommon.h"		// ������ ����(����� �� ����) IP
#include "LoginNetLayer.h"				// ������ ����(����� �� ����) Port

NiImplementRTTI( KLoginRoomManager, KRoomManager );

ImplToStringW( KLoginRoomManager )
{
	stm_ << L"--------------- Personal Shop Agency Manager -------------------" << std::endl;
	stm_ << TOSTRINGW( GetNumEmptyRoom() );
	stm_ << TOSTRINGW( m_mapRoomList.size() );
	stm_ << TOSTRINGW( m_vecRoomList.size() );
	stm_ << TOSTRINGW( m_dwNextRoomNumber );
	stm_ << TOSTRINGW( m_mapPShopOwnerList.size() );
	//}}

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
	BOOST_TEST_FOREACH( KLoginBaseRoomPtr, spRoom, m_vecRoomList )
	{
		if( spRoom == NULL )
			continue;

        KPerformer::KMaxQueueSize kMaxQueueSize = spRoom->GetMaxQueueSize();
		if( kMaxQueueSize.m_QueueSize > kMaxQueueSizeDump.m_QueueSize )
		{
            kMaxQueueSizeDump = kMaxQueueSize;
			iRoomUID = spRoom->GetUID();
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
		KTickDumpInfo kInfo;
		m_kRoomTickDump.GetDump( CXSLRoom::RT_PSHOP_AGENCY, kInfo );

		stm_ << L"-------------- Tick Dump --------------------" << std::endl;		
		stm_ << L"MaxTick : "		 << kInfo.m_dwMaxTick << std::endl;
		stm_ << L"MaxTickRegDate : " << kInfo.m_wstrMaxTickRegDate << std::endl;
		stm_ << L"AverageTick : "	 << kInfo.m_iAverageTickCount << std::endl;
	}
#endif SERV_ROOM_TICK_DUMP
	//}}

	return stm_;
}

KLoginRoomManager::KLoginRoomManager()
:
m_dfZUTickTime( 0.0 ),
m_dwNextRoomNumber( 1 )
{
}

KLoginRoomManager::~KLoginRoomManager()
{
}

void KLoginRoomManager::Run()
{
	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 20 );       // 0.02s ����

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Tick();

		else std::cout << "*** WaitForSingleObject() - return : " << ret << std::endl;

	}

	return;
}

bool KLoginRoomManager::Init()
{
	return KThread::Begin();
}

bool KLoginRoomManager::CreateEmptyRoom()
{
	KLocker lock( m_csEmptyRoomList );

	KNetAddress kAddress;
	kAddress.m_wstrIP = NetCommon::GetLocalIPW();
	kAddress.m_usPort = KNetLayer::GetKObj()->GetNCUDPPort();

	if( m_spRoomFSM == NULL )
	{
		m_spRoomFSM = CreateRoomFSM();
	}

	UidType iRoomUID = 0;
	KLoginBaseRoomPtr spRoom( CreateRoom() );
	if( spRoom == NULL )
	{
		START_LOG( cerr, L"�� ���� ����!" )
			<< END_LOG;
        return false;
	}

	KncUid::SetPureUID( iRoomUID, static_cast<UidType>(m_dwNextRoomNumber) );
	KncUid::SetServerID( iRoomUID, SEnum::PSHOP_AGENCY_PROXY_ID );
	//{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	KncUid::SetServerGroupID( iRoomUID, GetKLoginServer()->GetServerGroupID() );
#endif SERV_INTEGRATION
	//}}
	KncUid::SetReservedID( iRoomUID, CXSLRoom::RT_PSHOP_AGENCY );
	spRoom->SetUID( iRoomUID );
	spRoom->SetFSM( m_spRoomFSM );
	spRoom->SetUDPRelayInfo( kAddress );

	// ���⼭�� map�� key�ߺ� �˻縦 �� �ʿ䰡 ���� �� ����.
	m_mapRoomList[iRoomUID] = spRoom;
	m_vecRoomList.push_back( spRoom );
	m_mapEmptyRoomList[iRoomUID] = iRoomUID;

	// �������� ������ �� ����
	++m_dwNextRoomNumber;
	return true;
}

void KLoginRoomManager::Tick()
{
	//////////////////////////////////////////////////////////////////////////
	//{{ 2011. 03. 24	������	���� ���� üũ
#ifdef SERV_ROOM_TICK_DUMP
	const DWORD dwLastTickCount = ::GetTickCount();
#endif SERV_ROOM_TICK_DUMP
	//}}		
	//////////////////////////////////////////////////////////////////////////		

	BOOST_TEST_FOREACH( KLoginBaseRoomPtr, spRoom, m_vecRoomList )
	{
		if( spRoom == NULL )
		{
			START_LOG( cerr, L"�� ������ �̻�." )
				<< END_LOG;
			continue;
		}

		spRoom->Tick();
	}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2011. 03. 24	������	���� ���� üũ
#ifdef SERV_ROOM_TICK_DUMP
	const DWORD dwRoomTypeTick = ::GetTickCount() - dwLastTickCount;
	m_kRoomTickDump.CheckOneTickLatency( CXSLRoom::RT_PSHOP_AGENCY, dwRoomTypeTick );
#endif SERV_ROOM_TICK_DUMP
	//}}
	//////////////////////////////////////////////////////////////////////////

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
}

#include <KncLua.h>
#include <lua_tinker.h>

void KLoginRoomManager::RegToLua()
{
	//���ͼ����� ���ð��� ������(Set~~Func)�� config_cn.lua �� ���� �޴´�.
	lua_tinker::class_add<KLoginRoomManager>( g_pLua, "KLoginRoomManager" );
	lua_tinker::class_def<KLoginRoomManager>( g_pLua, "dump",						&KLoginRoomManager::Dump );
	lua_tinker::class_def<KLoginRoomManager>( g_pLua, "SetZUTickTime",				&KLoginRoomManager::SetZUTickTime );

	lua_tinker::decl( g_pLua, "RoomManager", this );
}

void KLoginRoomManager::ShutDown()
{
	// �α� �����
	std::wstringstream wstrDump;
	ToString( wstrDump );
	dbg::cout << wstrDump.str() << dbg::endl;

	KRoomManager::ShutDown();
}

DWORD KLoginRoomManager::GetNumEmptyRoom() const
{
	KLocker lock( m_csEmptyRoomList );

	return ( DWORD )m_mapEmptyRoomList.size();
}

UidType KLoginRoomManager::OpenRoom()
{
	KLocker lock( m_csEmptyRoomList );

	if( m_mapEmptyRoomList.empty() )
	{
		// ����� �����Ͽ� ���� ���� ����ϴ�!
		LIF( CreateEmptyRoom() );
	}

	std::map< UidType, UidType >::iterator mit;
	for( mit = m_mapEmptyRoomList.begin(); mit != m_mapEmptyRoomList.end(); ++mit )
	{
		KLoginBaseRoomPtr spRoom = Get( mit->first );
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

	if( mit == m_mapEmptyRoomList.end() )
	{
		START_LOG( cerr, L"�� ���� ã�� �� ����." )
			<< END_LOG;

		return 0;
	}

	UidType iRoomUID = mit->first;
	m_mapEmptyRoomList.erase( mit );

	return iRoomUID;
}

bool KLoginRoomManager::CloseRoom( UidType iRoomUID )
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

	KLocker lock( m_csEmptyRoomList );

	std::map< UidType, UidType >::iterator mit;
	mit = m_mapEmptyRoomList.find( iRoomUID );
	if( mit != m_mapEmptyRoomList.end() )
	{
		START_LOG( cerr, L"�� �渮��Ʈ�� �������� ID�� ���� �̹� ����." )
			<< BUILD_LOG( iRoomUID )
			<< END_LOG;

		return false;
	}

	KLoginBaseRoomPtr spRoom = Get( iRoomUID );
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
	m_mapEmptyRoomList.insert( std::make_pair( iRoomUID, iRoomUID ) );

	return true;
}

//{{ 2010. 06. 17  ������	������ ����
void KLoginRoomManager::QueueingEventToRoomManager( const KEventPtr& spEvent_ )
{
	m_kRMEventProcess.QueueingEvent( spEvent_ );
}
//}}

void KLoginRoomManager::QueueingEventToRoomByRoomUID( IN const UidType iRoomUID, IN const KEventPtr& spEvent_ )
{
	KLoginBaseRoomPtr spRoom = Get( iRoomUID );
	if( spRoom == NULL )
	{
		START_LOG( cerr, L"���� ���� �ʴ� ���Դϴ�!" )
			<< BUILD_LOG( iRoomUID )
			<< END_LOG;
		return;
	}

	spRoom->QueueingEvent( spEvent_ );
}

void KLoginRoomManager::QueueingEventToRoom( const KEventPtr& spEvent_ )
{
	UidType iRoomUID = 0;
	switch( spEvent_->m_usEventID )
	{
	case ERM_OPEN_PSHOP_AGENCY_REQ:
	case ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT:
	case ERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ:
		QueueingEventToRoomManager( spEvent_ ); // SimLayer���� ó���ϱ� ���� LoginRMEventProcess���� �����ϴ�.
		return;

	default:
		// �� �ϳ����� �̺�Ʈ�� ���޵ȴٰ� ������. GetFirstUID
		iRoomUID = spEvent_->m_kDestPerformer.GetFirstUID();
		break;
	}

	if( iRoomUID <= 0 )
	{
		switch( spEvent_->m_usEventID )
		{
		case ERM_OPEN_PSHOP_AGENCY_REQ:
			{
				KERM_OPEN_PERSONAL_SHOP_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_00;
				SendToGSCharacter( spEvent_->GetLastSenderUID(), spEvent_->GetFirstSenderUID(), ERM_OPEN_PSHOP_AGENCY_ACK, kPacket );
				return;
			}
			break;
		case ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT:
			{
				START_LOG( cerr, L"���� ������ �븮���� ���� �����ϴ�! �Ͼ���� �ȵǴ� ����!" )
					<< END_LOG;

			}
			break;
		default:
			START_LOG( cerr, L"�̺�Ʈ�� ��ǥ �� UID�� �̻���." )
				<< BUILD_LOG( iRoomUID )
				<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
				<< END_LOG;
			return;
		}
	}

	KLoginBaseRoomPtr spRoom = Get( iRoomUID );
	if( spRoom == NULL )
	{
		START_LOG( cwarn, L"�� �˻� ����." )
			<< BUILD_LOG( iRoomUID )
			<< END_LOG;
		return;
	}

	spRoom->QueueingEvent( spEvent_ );
}

void KLoginRoomManager::QueueingToAll( int iRoomType, const KEventPtr& spEvent_ )
{
	// �� �濡�� ť������ �ʴ´�.
	std::map< UidType, KLoginBaseRoomPtr >::iterator mitRoom;
	std::map< UidType, UidType >::iterator mitEmptyRoom;

	BOOST_TEST_FOREACH( KLoginBaseRoomPtr, spRoom, m_vecRoomList )
	{
		if( spRoom == NULL )
			continue;

		mitEmptyRoom = m_mapEmptyRoomList.find( spRoom->GetUID() );
		if( mitEmptyRoom == m_mapEmptyRoomList.end() )
		{
			spRoom->QueueingEvent( spEvent_ );
		}
	}
}

void KLoginRoomManager::QueueingToAll( const KEventPtr& spEvent_ )
{
	// ���� ���� �ִ� ��� �濡 �̺�Ʈ�� ť���Ѵ�. �� �濡�� ť������ �ʴ´�.	
	QueueingToAll( CXSLRoom::RT_PSHOP_AGENCY, spEvent_ );
}

void KLoginRoomManager::SetZUTickTime( double dfTickTime )
{
	m_dfZUTickTime = dfTickTime;

	START_LOG( cout2, L"Zombie User üũ �ð�����.!" )
		<< BUILD_LOG( m_dfZUTickTime );
}

double KLoginRoomManager::GetZUTickTime()
{
	if( m_dfZUTickTime <= 0.0 || m_dfZUTickTime >= 600.0 )
	{
		return 180.0;
	}

	return m_dfZUTickTime;
}

bool KLoginRoomManager::AddPShopOwner( IN const UidType iUnitUID, IN const UidType iRoomUID )
{
	UidType iCheckRoomUID = 0;
	if( GetPShopAgencyUIDByUnitUID( iUnitUID, iCheckRoomUID ) == true )
	{
		START_LOG( cerr, L"�̹� ��ϵǾ� �ִ� �����Դϴ�. ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iCheckRoomUID )
			<< BUILD_LOG( iRoomUID )
			<< END_LOG;
		return false;
	}
    
	m_mapPShopOwnerList.insert( std::make_pair( iUnitUID, iRoomUID ) );
	return true;
}

void KLoginRoomManager::DelPShopOwner( IN const UidType iUnitUID )
{
	UidType iCheckRoomUID = 0;
	if( GetPShopAgencyUIDByUnitUID( iUnitUID, iCheckRoomUID ) == false )
	{
		START_LOG( cerr, L"��ϵǾ� �ִ� ������ �ƴմϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return;
	}
    
	m_mapPShopOwnerList.erase( iUnitUID );
}

bool KLoginRoomManager::GetPShopAgencyUIDByUnitUID( IN const UidType iUnitUID, OUT UidType& iRoomUID )
{
	std::map< UidType, UidType >::const_iterator mit;
	mit = m_mapPShopOwnerList.find( iUnitUID );
	if( mit == m_mapPShopOwnerList.end() )
	{
		return false;
	}

	iRoomUID = mit->second;
	return true;
}

void KLoginRoomManager::SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID )
{
	SendToGSCharacter( nGS, nTo, usEventID, char() );
}

FSMclassPtr KLoginRoomManager::CreateRoomFSM()
{
	return boost::shared_ptr< KPersonalShopFSM >( new KPersonalShopFSM );
}

KLoginBaseRoomPtr KLoginRoomManager::CreateRoom()
{
	return KPersonalShopAgencyPtr( new KPersonalShopAgency );
}

KLoginBaseRoomPtr KLoginRoomManager::Get( UidType iRoomUID )
{
	std::map< UidType, KLoginBaseRoomPtr >::iterator mit;
	mit = m_mapRoomList.find( iRoomUID );
	if( mit == m_mapRoomList.end() )
	{
		return KLoginBaseRoomPtr();
	}

	return mit->second;
}



