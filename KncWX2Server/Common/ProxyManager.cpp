#include "ProxyManager.h"

#include "Socket/NetCommon.h"
#include "NetLayer.h"
#include "BaseServer.h"
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

//{{ 2010. 01. 27  ������	���������
// Ư�� �������� �������̴�.. �ذ����� ã��!
#include "X2Data/XSLRoom.h"
//}}
//{{ 2010. 02. 17  ������	ä�� ��Ƽ ����
#ifdef SERV_CHANNEL_PARTY
	#include "X2Data/XSLPartyManager.h"
#endif SERV_CHANNEL_PARTY
//}}

ImplementSingleton( KProxyManager );

ImplToStringW( KProxyManager )
{
	START_TOSTRING2W( KProxyManager );

	//{{ 2009. 11. 23  ������	���������ӱ�������
	{
		KLocker lock( m_csProxy );

		//{{ 2010. 8. 31	������	�̺�Ʈ ť �ִ� ũ�� ����
#ifdef SERV_MAX_QUEUE_SIZE_DUMP
		UidType iUID = 0;
		KPerformer::KMaxQueueSize kMaxQueueSizeDump;
#endif SERV_MAX_QUEUE_SIZE_DUMP
		//}}

		for( int i = 0; i < PT_NUM; ++i )
		{
			int nAliveProxyCount = 0;
			std::map< int, KActorProxyPtr >::const_iterator mit;
			for( mit = m_mapProxy[i].begin(); mit != m_mapProxy[i].end(); ++mit )
			{
				KActorProxyPtr spProxy = mit->second;
				if( spProxy == NULL )
					continue;

				nAliveProxyCount++;

				//{{ 2010. 8. 31	������	�̺�Ʈ ť �ִ� ũ�� ����
#ifdef SERV_MAX_QUEUE_SIZE_DUMP
				KPerformer::KMaxQueueSize kMaxQueueSize = spProxy->GetMaxQueueSize();
				if( kMaxQueueSize.m_QueueSize > kMaxQueueSizeDump.m_QueueSize )
				{
					kMaxQueueSizeDump = kMaxQueueSize;
					iUID = spProxy->GetUID();
				}
#endif SERV_MAX_QUEUE_SIZE_DUMP
				//}}
			}

			stm_ << TOSTRINGW( m_vecAddress[i].size() )
				<< TOSTRINGW( m_mapProxy[i].size() )
				<< TOSTRINGW( nAliveProxyCount );
		}
	
		//{{ 2010. 8. 31	������	�̺�Ʈ ť �ִ� ũ�� ����
#ifdef SERV_MAX_QUEUE_SIZE_DUMP
		stm_ << TOSTRINGW( iUID );
		stm_ << TOSTRINGW( kMaxQueueSizeDump.m_QueueSize );
		stm_ << TOSTRINGW( kMaxQueueSizeDump.GetMaxQueueSizeRegDate() );
#endif SERV_MAX_QUEUE_SIZE_DUMP
		//}}
	}
	//}}

	return stm_;
}

DWORD KProxyManager::ms_dwReconnectGap( 1000 * 60 * 2 );  // 2��.

KProxyManager::KProxyManager(void)
{
    m_dwReconnectTick = ::GetTickCount();
	m_fnCreateProxy = NULL;	
}

KProxyManager::~KProxyManager(void)
{
}

void KProxyManager::SetProxyBuilder( PFNCreateProxy fnBuilder )
{
	m_fnCreateProxy = fnBuilder;
}

void KProxyManager::AddRemoteAddress( int iProxyType_, int iID_, const WCHAR* wIP_, unsigned short usPort_ )
{
	if( iProxyType_ < 0 || iProxyType_ >= PT_NUM )
	{
		START_LOG( cerr, L"���Ͻ� Ÿ���� �̻���." )
			<< BUILD_LOG( iProxyType_ )
			<< END_LOG;

		return;
	}

    if( iID_ < 0 || iID_ > ( int )m_vecAddress[iProxyType_].size() )
    {
        START_LOG( cerr, L"���Ͻ� ���̵� �߸��Ǿ���." )
			<< BUILD_LOG( iProxyType_ )
            << BUILD_LOG( iID_ )
            << BUILD_LOG( ( int )m_vecAddress[iProxyType_].size() )
            << END_LOG;

        return;
    }

	KNetAddress kAddr;
	kAddr.m_wstrIP  = wIP_;
    kAddr.m_usPort  = usPort_;

    if( iID_ == ( int )m_vecAddress[iProxyType_].size() )
    {
        m_vecAddress[iProxyType_].push_back( kAddr );
    }
    else
    {
        START_LOG( cerr, L"���Ͻ� �ּҸ� ���." )
			<< BUILD_LOG( iProxyType_ )
            << BUILD_LOG( iID_ )
            << BUILD_LOG( m_vecAddress[iProxyType_][iID_].m_wstrIP )
            << BUILD_LOG( m_vecAddress[iProxyType_][iID_].m_usPort )
            << BUILD_LOG( kAddr.m_wstrIP )
            << BUILD_LOG( kAddr.m_usPort )
            << END_LOG;

        m_vecAddress[iProxyType_][iID_] = kAddr;
    }
}

int KProxyManager::GetRemoteAddrNum( int iProxyType )
{
	if( iProxyType < 0 || iProxyType >= PT_NUM )
	{
		START_LOG( cerr, L"���Ͻ� Ÿ���� �̻���." )
			<< BUILD_LOG( iProxyType )
			<< END_LOG;

		return 0;
	}

	return m_vecAddress[iProxyType].size();
}

//{{ 2009. 11. 23  ������	���������ӱ�������
void KProxyManager::ConnectAll()
{
	// �� �Լ��� ������ ���� Thread�� ȣ���ϵ��� �ؾ����ٵ�..

	for( int i = 0; i < PT_NUM; ++i )
	{
		for( int j = 0; j < ( int )m_vecAddress[i].size(); ++j )
		{
			Connect( i, j );
		}
	}

	//{{ 2009. 11. 23  ������	���������ӱ�������
	m_spThreadKeepConnect = boost::shared_ptr< KTThread< KProxyManager > >( new KTThread< KProxyManager >( *this, &KProxyManager::KeepConnectionThread, 10000 ) );

	if( m_spThreadKeepConnect )
	{
        m_spThreadKeepConnect->Begin();

		START_LOG( cout, L"ProxyManager ������ Thread ����!" );
	}
	else
	{
		START_LOG( cerr, L"ProxyManager ������ Thread ���� ����!" );
	}
	//}}
}
//}}

void KProxyManager::Tick()
{
	//{{ 2011. 12. 14	������	SimLayer Tick�����ս� üũ
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	BEGIN_CHECK_TICK_LATENCY;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

	//{{ 2009. 11. 23  ������	���������ӱ�������
	KLocker lock( m_csProxy );
	//}}

	std::map< int, KActorProxyPtr >::iterator mit;
	
	for( int i = 0; i < PT_NUM; ++i )
	{
		for( mit = m_mapProxy[i].begin(); mit != m_mapProxy[i].end(); ++mit )
		{
			if( mit->second )
			{
				mit->second->Tick();
			}
		}
	}

	// ��ü ������ ����� uid�� ������ ã�Ƽ� �����. (��ü���� �����, map���� �״�� ���ܵд�.)
	for( int i = 0; i < PT_NUM; ++i )
	{
		std::vector< int >::iterator vit;
		for( vit = m_vecDestroyList[i].begin(); vit != m_vecDestroyList[i].end(); ++vit )
		{
			mit = m_mapProxy[i].find( *vit );
			if( mit == m_mapProxy[i].end() )
			{
				START_LOG( cerr, L"�����ϰ��� �ϴ� proxy�� ã�� �� ����" )
					<< BUILD_LOG( *vit )
					<< BUILD_LOG( m_mapProxy[i].size() )
					<< END_LOG;
			}
			else
			{
				mit->second.reset();
			}
		}
		m_vecDestroyList[i].clear();
	}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 11. 23  ������	���������ӱ�������
	// ������ ���� ������ �־ �����Ǵ� �κ�.. m_mapProxy <-- �̳༮�� ������!

	// ���� �ֱ�� ���ͼ������� ���� ���� üũ.
	//if( GetTickCount() - m_dwReconnectTick >= ms_dwReconnectGap )
	//{
	//	START_LOG( clog, L"Proxy ���ӻ��� üũ" );

	//	for( i = 0; i < PT_NUM; ++i )
	//	{
	//		for( mit = m_mapProxy[i].begin(); mit != m_mapProxy[i].end(); ++mit )
	//		{
	//			if( !mit->second )
	//			{
	//				Connect( i, mit->first );
	//			}
	//		}
	//	}

	//	m_dwReconnectTick = ::GetTickCount();
	//}
	//}}
	//////////////////////////////////////////////////////////////////////////	

	//{{ 2011. 12. 14	������	SimLayer Tick�����ս� üũ
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	END_CHECK_TICK_LATENCY;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}
}

bool KProxyManager::Shutdown( float fWaitTime )
{
	//{{ 2011. 10. 24	������	ProxyManager ���� ����
//#ifdef SERV_PROXY_MANAGER_BUG_FIX

//{{ robobeg : 2013-08-08
    // ���Ӽ��� ����� ũ������ �����ϱ� ���� ����
	if ( m_spThreadKeepConnect )
		m_spThreadKeepConnect->End( INFINITE );
//}} robobeg : 2013-08-08
	{
		KLocker lock( m_csProxy );

//{{ robobeg : 2013-08-08
		//{{ 2009. 11. 23  ������	���������ӱ�������
		//m_spThreadKeepConnect->End( 3000 ); // �ϴ� ������ Thread���� ���δ�.
		//}}
//}} robobeg : 2013-08-08

		for( int i = 0; i < PT_NUM; ++i )
		{
			std::map< int, KActorProxyPtr >::iterator mit;
			for( mit = m_mapProxy[i].begin(); mit != m_mapProxy[i].end(); ++mit )
			{
				if( mit->second )
				{
					mit->second->SetDisconnectReason( KStatistics::eSIColDR_Server_Shutdown );
					mit->second->ReserveDestroy();
				}
			}
		}
	}	
//#endif SERV_PROXY_MANAGER_BUG_FIX
	//}}

	boost::timer timer;
	bool bShutdownFinished = ShutdownFinished();

	while( timer.elapsed() < fWaitTime && !bShutdownFinished )
	{
		Sleep( 500 );

		bShutdownFinished = ShutdownFinished();
	}

	LOG_SUCCESS( bShutdownFinished )
		<< BUILD_LOG( timer.elapsed() );

	return bShutdownFinished;
}

void KProxyManager::DestroyProxy( int iProxyType, int iProxyID )
{
	START_LOG( cerr, L"�� ��������." )
		<< BUILD_LOG( iProxyType )
		<< BUILD_LOG( iProxyID )
		<< END_LOG;

	if( iProxyType < 0 || iProxyType >= PT_NUM )
	{
		START_LOG( cerr, L"���Ͻ� Ÿ���� �̻���." )
			<< BUILD_LOG( iProxyType )
			<< END_LOG;

		return;
	}

	m_vecDestroyList[iProxyType].push_back( iProxyID );
}

void KProxyManager::SetRoomServerLoad( int iProxyID, bool bIncrease )
{
	KActorProxyPtr spProxy = Get( PT_CENTER, iProxyID );

	if( spProxy )
	{
		int iLoad = spProxy->GetLoad();
		if( bIncrease )
		{
			spProxy->SetLoad( iLoad + 1 );
		}
		else
		{
			spProxy->SetLoad( iLoad - 1 );
		}
	}
}

bool KProxyManager::SendPacket( UidType nFrom, KEvent& kEvent_ )
{
	KActorProxyPtr spProxy;

	switch( GetPerformerClass( kEvent_.m_kDestPerformer.m_dwPerformerID ) )
	{
	case PC_USER:
	case PC_SERVER:
	case PC_ACCOUNT_DB:
	case PC_LOG_DB:
    case PC_NX_AUTH:
#ifdef SERV_GLOBAL_AUTH
	case PC_PUBLISHER_AUTH:
#endif // SERV_GLOBAL_AUTH
#ifdef SERV_GLOBAL_BILLING
	case PC_KOG_BILLING_DB:
	case PC_KOG_BILLING_MANAGER:
	case PC_PUBLISHER_BILLING:
#endif // SERV_GLOBAL_BILLING
#ifdef SERV_COUNTRY_CN
	case PC_GIANT_AUTH:
	case PC_GIANT_ROLEREG:
	case PC_GIANT_INFO:
	case PC_GIANT_BILLING:
#endif // SERV_COUNTRY_CN
		//{{ 2011. 07. 04	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	case PC_MATCH_MAKING:
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ 2012. 02. 16	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	case PC_AUTO_PARTY:
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
#ifdef SERV_ID_NETMARBLE_PCBANG
	case PC_ID_PCBANG_AUTH_DB:
#endif //SERV_ID_NETMARBLE_PCBANG
#ifdef SERV_ADD_EVENT_DB
	case PC_EVENT_DB:
#endif //SERV_ADD_EVENT_DB
#ifdef SERV_ADD_SCRIPT_DB
	case PC_SCRIPT_DB:
#endif //SERV_ADD_SCRIPT_DB
		switch( GetServerClass( kEvent_.m_kDestPerformer.m_dwPerformerID ) )
		{
		case SC_CENTER:
			spProxy = GetCnProxy( nFrom );
			break;
		case SC_LOGIN:
			spProxy = GetLoginProxy();
			break;
			//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
		case SC_GLOBAL:
			spProxy = GetGlobalProxy();
			break;
#endif SERV_INSERT_GLOBAL_SERVER
			//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
		default:
			START_LOG( cerr, L"��Ŷ�� ��ǥ ������ �̻���." )
				<< BUILD_LOG( KEvent::GetIDStr( kEvent_.m_usEventID ) )
				<< BUILD_LOG( kEvent_.m_kDestPerformer.m_dwPerformerID )
				<< BUILD_LOG( kEvent_.m_kDestPerformer.GetUIDListSize() )
				<< END_LOG;
			break;
		}
		break;

	case PC_ROOM:
		//{{ 2011. 04. 29	������	�븮����
#ifdef SERV_PSHOP_AGENCY
		switch( GetServerClass( kEvent_.m_kDestPerformer.m_dwPerformerID ) )
		{
		case SC_CENTER:
			spProxy = GetRoomProxy( kEvent_.m_kDestPerformer.GetFirstUID(), kEvent_.m_usEventID );
			break;
		case SC_LOGIN:
			spProxy = GetLoginProxy();
			break;
		default:
			START_LOG( cerr, L"��Ŷ�� ��ǥ ������ �̻���." )
				<< BUILD_LOG( KEvent::GetIDStr( kEvent_.m_usEventID ) )
				<< BUILD_LOG( kEvent_.m_kDestPerformer.m_dwPerformerID )
				<< BUILD_LOG( kEvent_.m_kDestPerformer.GetUIDListSize() )
				<< END_LOG;
			break;
		}
#else
		spProxy = GetRoomProxy( kEvent_.m_kDestPerformer.GetFirstUID(), kEvent_.m_usEventID );
#endif SERV_PSHOP_AGENCY
		//}}
		break;

		//{{ 2010. 02. 16  ������	ä����Ƽ����
#ifdef SERV_CHANNEL_PARTY
	case PC_PARTY:
		spProxy = GetPartyProxy( kEvent_.m_kDestPerformer.GetFirstUID() );
		break;
#endif SERV_CHANNEL_PARTY
		//}}

	default:
		START_LOG( cerr, L"��Ŷ�� ��ǥ ������ �̻���." )
			<< BUILD_LOG( KEvent::GetIDStr( kEvent_.m_usEventID ) )
			<< BUILD_LOG( kEvent_.m_kDestPerformer.m_dwPerformerID )
			<< BUILD_LOG( kEvent_.m_kDestPerformer.GetUIDListSize() )
			<< END_LOG;
		return false;
	}

	if( !spProxy )
	{
		START_LOG( cerr, L"Proxy �����͸� ã�� �� ����" )
			<< BUILD_LOG( kEvent_.m_kDestPerformer.m_dwPerformerID )
			<< BUILD_LOG( kEvent_.m_kDestPerformer.GetUIDListSize() )
			<< BUILD_LOG( kEvent_.m_usEventID )
			<< BUILD_LOG( kEvent_.GetIDStr() )
			<< END_LOG;

		//{{ 2009. 11. 23  ������	���������ӱ�������
		// ������ �õ��� �ϸ� �ȵ˴ϴ�. �ֳĸ� ���� ���� Thread�� �� �Լ��� ȣ���ϱ� ����������..

		//LIF( Connect( iProxyType, iProxyID ) );   // ��ȿ�� ������ �߰��ϸ� ������ �õ�.
		//}}
		return false;
	}

	return spProxy->SendPacket( kEvent_ );
}

//int KProxyManager::BroadCast( int iProxyType_, KEvent& kEvent_ )
//{
//	// �� �Լ��� ����� ���ǰ� ���� ����
//
//	//{{ 2009. 11. 23  ������	���������ӱ������� - ���� ���� �Լ��� ���߰ڴ�
//	KLocker lock( m_csProxy );
//	//}}
//
//	if( iProxyType_ < 0 || iProxyType_ >= PT_NUM )
//	{
//		START_LOG( cerr, L"���Ͻ� Ÿ���� �̻���." )
//			<< BUILD_LOG( iProxyType_ )
//			<< END_LOG;
//
//		return 0;
//	}
//
//	std::map< int, KActorProxyPtr >::iterator mit;
//
//	int nCount = 0;
//
//	for( mit = m_mapProxy[iProxyType_].begin(); mit != m_mapProxy[iProxyType_].end(); ++mit )
//	{
//		if( mit->second == NULL )
//		{
//			START_LOG( cerr, L"Proxy �����͸� ã�� �� ����" )
//				<< BUILD_LOG( kEvent_.m_kDestPerformer.m_dwPerformerID )
//				<< BUILD_LOG( kEvent_.m_kDestPerformer.GetUIDListSize() )
//				<< BUILD_LOG( kEvent_.m_usEventID )
//				<< BUILD_LOG( kEvent_.GetIDStr() )
//				<< END_LOG;
//
//			//{{ 2009. 11. 23  ������	���������ӱ�������
//			// �� �Լ������� Connect()�� ȣ���ϸ� �ȵ˴ϴ�. �ֳĸ� ���ӷ��� Thread�� �� �Լ��� ȣ���ϱ�  ����������..
//
//			//LIF( Connect( iProxyType_, mit->first ) );   // ��ȿ�� ������ �߰��ϸ� ������ �õ�.
//			//}}
//		}
//		else
//		{
//			mit->second->SendPacket( kEvent_ );
//			nCount++;
//		}
//	}
//
//	return nCount;
//}

//{{ 2009. 11. 23  ������	���������ӱ�������
void KProxyManager::GetDisconnectedProxyList( OUT std::vector< KProxyInfo >& vecDisconnProxy )
{
	KLocker lock( m_csProxy );

	vecDisconnProxy.clear();

	for( int i = 0; i < PT_NUM; ++i )
	{
		std::map< int, KActorProxyPtr >::iterator mit;
		for( mit = m_mapProxy[i].begin(); mit != m_mapProxy[i].end(); ++mit )
		{
			if( !mit->second )
			{
				vecDisconnProxy.push_back( KProxyInfo( i, mit->first ) );
			}
		}
	}
}
//}}

//{{ 2010. 06. 16  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT

void KProxyManager::BroadCastToCnServer( KEvent& kEvent_ )
{
	KLocker lock( m_csProxy );

	std::map< int, KActorProxyPtr >::iterator mit;
	for( mit = m_mapProxy[PT_CENTER].begin(); mit != m_mapProxy[PT_CENTER].end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		mit->second->SendPacket( kEvent_ );
	}
}

void KProxyManager::BroadCastToCnServerEachServerGroup( KEvent& kEvent_ )
{
	KLocker lock( m_csProxy );

	std::set< int > setCheckServerGroup;

	std::map< int, KActorProxyPtr >::iterator mit;
	for( mit = m_mapProxy[PT_CENTER].begin(); mit != m_mapProxy[PT_CENTER].end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		const int iCnServerGroup = mit->second->GetActorServerGroup();
		if( iCnServerGroup == -1 )
			continue;

		if( setCheckServerGroup.find( iCnServerGroup ) != setCheckServerGroup.end() )
			continue;

		setCheckServerGroup.insert( iCnServerGroup );

		mit->second->SendPacket( kEvent_ );
	}
}

#endif SERV_REALTIME_SCRIPT
//}}

bool KProxyManager::ShutdownFinished()
{
	//{{ 2009. 11. 23  ������	���������ӱ�������	
	KLocker lock( m_csProxy );
	//}}
	
	for( int i = 0; i < PT_NUM; ++i )
	{
		std::map< int, KActorProxyPtr >::iterator mit;
		for( mit = m_mapProxy[i].begin(); mit != m_mapProxy[i].end(); ++mit )
		{
			// ���� ����ִ� �����Ͱ� �ִٸ� false.
			if( mit->second )
			{
				return false;
			}
		}
	}

	return true;
}

bool KProxyManager::Connect( int iProxyType, int iID )
{
	//{{ 2009. 11. 23  ������	���������ӱ�������
	// mapProxy�� �ֱ� �������� ����ȭ�� ��ȣ�����ʾƵ� �������ϴ�.	
	//}}

	if( iProxyType < 0 || iProxyType >= PT_NUM )
	{
		START_LOG( cerr, L"���Ͻ� Ÿ���� �̻���." )
			<< BUILD_LOG( iProxyType )
			<< END_LOG;

		return false;
	}

    if( iID < 0 || iID >= ( int )m_vecAddress[iProxyType].size() )
    {
        START_LOG( cerr, L"���Ͻ� ���̵� �߸��Ǿ���." )
            << BUILD_LOG( iID )
            << BUILD_LOG( m_vecAddress[iProxyType].size() )
            << END_LOG;

        return false;
    }

    KActorProxyPtr spProxy( m_fnCreateProxy( iProxyType ) );

    spProxy->Init( true );
    spProxy->SetProxyID( iID );

	//{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	//{{ 2011. 07. 19	������	������ ���� �ڵ� �����丵
	//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING	
	KECN_VERIFY_SERVER_CONNECT_REQ kPacket;
	//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
	//}}
	kPacket.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
	//{{ 2013. 01. 21	������	������ ��� �缳IP ����
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
	kPacket.m_kNetAddress.m_wstrIP = KBaseServer::GetKObj()->GetPrivateIP();	// ���� ���Ӽ����� �缳IP�� �Ѱ��ش�!
#else
	kPacket.m_kNetAddress.m_wstrIP = NetCommon::GetLocalIPW();
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}	
	kPacket.m_kNetAddress.m_usPort = KNetLayer::GetKObj()->m_usPortMaster;
#else
	KNetAddress kPacket;
	kPacket.m_wstrIP    = NetCommon::GetLocalIPW();
	kPacket.m_usPort    = KNetLayer::GetKObj()->m_usPortMaster;
#endif SERV_INTEGRATION
	//}}

	//{{ 2011. 07. 19	������	������ ���� �ڵ� �����丵
	//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING	
    KEvent kEvent;
	switch( iProxyType )
	{
	case PT_CENTER:
		kEvent.SetData( PI_CN_USER, NULL, ECN_VERIFY_SERVER_CONNECT_REQ, kPacket );
		break;
	case PT_LOGIN:
		kEvent.SetData( PI_LOGIN_USER, NULL, ELG_VERIFY_SERVER_CONNECT_REQ, kPacket );
		break;
		//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	case PT_GLOBAL:
		kEvent.SetData( PI_GLOBAL_USER, NULL, EGB_VERIFY_SERVER_CONNECT_REQ, kPacket );
		break;
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
	default:
		START_LOG( cerr, L"���Ͻ� Ÿ���� �̻���." )
			<< BUILD_LOG( iProxyType )
			<< END_LOG;
		return false;
	}
	//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
	//}}

	KNetAddress kAddr = m_vecAddress[iProxyType][( int )iID];

    int nRet = spProxy->ConnectNAuth( kAddr.m_wstrIP.c_str(), kAddr.m_usPort, kEvent, 5000 );

    LOG_SUCCESS( nRet == 0 )
        << BUILD_LOG( nRet )
		<< BUILD_LOG( iProxyType )
        << BUILD_LOG( iID )
        << BUILD_LOG( m_vecAddress[iProxyType][( int )iID].m_wstrIP )
        << BUILD_LOG( m_vecAddress[iProxyType][( int )iID].m_usPort );

    if( nRet != 0 )
    {
        spProxy.reset();
    }

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 11. 23  ������	���������ӱ�������
	// ���⵵ ����ȭ �Ǿ�� �Ѵ�! m_mapProxy�� ���̴°��� �α׸� ����ȭ �ؾ��ҵ�

	{
		KLocker lock( m_csProxy );
        
		m_mapProxy[iProxyType][iID] = spProxy;
	}

	//}}
	//////////////////////////////////////////////////////////////////////////

    return ( nRet == 0 );
}

KActorProxyPtr KProxyManager::Get( int iProxyType, int iProxyID )
{
	//{{ 2009. 11. 23  ������	���������ӱ�������
	KLocker lock( m_csProxy );
	//}}

	if( iProxyType < 0 || iProxyType >= PT_NUM )
	{
		START_LOG( cerr, L"���Ͻ� Ÿ���� �̻���." )
			<< BUILD_LOG( iProxyType )
			<< END_LOG;

		return KActorProxyPtr();
	}

	if( iProxyID < 0 || iProxyID >= ( int )m_vecAddress[iProxyType].size() )
	{
		START_LOG( cerr, L"���Ͻ� ���̵� �߸��Ǿ���." )
			<< BUILD_LOG( iProxyID )
			<< BUILD_LOG( m_vecAddress[iProxyType].size() )
			<< END_LOG;

		return KActorProxyPtr();
	}

	std::map< int, KActorProxyPtr >::iterator mit;
	mit = m_mapProxy[iProxyType].find( iProxyID );

	if( mit == m_mapProxy[iProxyType].end() )
	{
		return KActorProxyPtr();
	}
	else
	{
		if( mit->second )
		{
			return mit->second;
		}
		else
		{
			return KActorProxyPtr();
		}
	}
}

//{{ 2010. 05. 17  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION

int KProxyManager::GetCnProxyID( UidType nUserUID_ )
{
	//{{ 2009. 11. 23  ������	���������ӱ�������
	KLocker lock( m_csProxy );
	//}}

	//////////////////////////////////////////////////////////////////////////
	// [����] nUserUID�� m_vecSameServerGroupProxy�� size()�� ���� �������� index�� �Ͽ� �����ϰ� ProxyID�� ��´�!
	//////////////////////////////////////////////////////////////////////////

	// �׷��� ����ִٸ� ����ó��!
	if( m_vecSameServerGroupProxy.empty() ) return -1;

	const u_int uiIndex = ( u_int )( nUserUID_ % (UidType)m_vecSameServerGroupProxy.size() );
	if( uiIndex >= m_vecSameServerGroupProxy.size() )
		return -1;

	return m_vecSameServerGroupProxy[uiIndex];
}

#else

int KProxyManager::GetCnProxyID( UidType nUserUID_ )
{
	//{{ 2009. 11. 23  ������	���������ӱ�������
	KLocker lock( m_csProxy );
	//}}

	//////////////////////////////////////////////////////////////////////////
	// [����] nUserUID�� m_mapProxy�� size()�� ���� �������� index�� �Ͽ� �����ϰ� ProxyID�� ��´�!
	//////////////////////////////////////////////////////////////////////////

	if( m_mapProxy[PT_CENTER].empty() ) return -1;

	return ( int )( nUserUID_ % (UidType)m_mapProxy[PT_CENTER].size() );
}

#endif SERV_INTEGRATION
//}}

KActorProxyPtr KProxyManager::GetCnProxy( UidType nUserUID_ )
{
	//{{ 2009. 11. 23  ������	���������ӱ�������
	KLocker lock( m_csProxy );
	//}}

    int iCnProxyID = GetCnProxyID( nUserUID_ );

    std::map< int, KActorProxyPtr >::iterator mit;
    mit = m_mapProxy[PT_CENTER].find( iCnProxyID );

    if( mit == m_mapProxy[PT_CENTER].end() )
        return KActorProxyPtr();

	//{{ 2010. 05. 17  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( mit->second == NULL )
	{
		START_LOG( cerr, L"CnProxy�����Ͱ��� NULL�̴�! ���ͼ����� ����� ���������� Ȯ���� �ʿ���!" )
			<< BUILD_LOG( nUserUID_ )
			<< BUILD_LOG( iCnProxyID )
			<< END_LOG;

		return KActorProxyPtr();
	}

	if( mit->second->IsSameServerGroup() == false )
	{
		START_LOG( cerr, L"���� �������� ���ͷθ� ��Ŷ�� ������ �� �ִ�!" )
			<< BUILD_LOG( nUserUID_ )
			<< BUILD_LOG( iCnProxyID )
			<< END_LOG;

		return KActorProxyPtr();
	}
#endif SERV_INTEGRATION
	//}}

    return mit->second;
}

//{{ 2010. 01. 27  ������	���������
// Ư�� �������� �������̴�.. �ذ����� ã��!
// GetTypeByEventID()�Լ��� �Լ������ͷ� ����ؼ� ���°� ���?
#ifdef SERV_ROOM_COUNT

int KProxyManager::GetTypeByEventID( u_short usEventID )
{
	switch( usEventID )
	{
	case ERM_OPEN_PVP_ROOM_REQ:
		//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	case ERM_OPEN_PVP_ROOM_FOR_MATCH_REQ:
#endif SERV_PVP_NEW_SYSTEM
		//}}
		return CXSLRoom::RT_PVP;

	case ERM_OPEN_DUNGEON_ROOM_REQ:
	case ERM_PARTY_OPEN_DUNGEON_ROOM_REQ:
		//{{ 2011. 12. 09	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	case ERM_SINGLE_OPEN_DUNGEON_ROOM_REQ:
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		return CXSLRoom::RT_DUNGEON;

	case ERM_OPEN_TRADE_REQ:
		return CXSLRoom::RT_TRADE;

	case ERM_OPEN_TC_ROOM_REQ:
		return CXSLRoom::RT_TRAININGCENTER;

	case ERM_OPEN_PERSONAL_SHOP_REQ:
		return CXSLRoom::RT_PERSONAL_SHOP;
		//{{ 2011. 04. 11	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	case ERM_OPEN_PSHOP_AGENCY_REQ:
		return CXSLRoom::RT_PSHOP_AGENCY;
#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	case ERM_OPEN_BATTLE_FIELD_REQ:
		return CXSLRoom::RT_BATTLE_FIELD;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	case ERM_OPEN_WEDDING_HALL_REQ:
		return CXSLRoom::RT_WEDDING_HALL;
#endif SERV_RELATIONSHIP_SYSTEM
		//}
	}
	
	START_LOG( cerr, L"��� ���� ���� ����� ��Ŷ�Դϴ�." )
		<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
		<< END_LOG;

	// default roomtype
	return CXSLRoom::RT_PVP;
}

int KProxyManager::GetRoomProxyID_NEW( UidType nRoomUID, u_short usEventID )
{
	//{{ 2009. 11. 23  ������	���������ӱ�������
	KLocker lock( m_csProxy );
	//}}

	if( nRoomUID == 0 )
	{
		const int iType = GetTypeByEventID( usEventID );

		std::map< int, KActorProxyPtr >::iterator mit;
		int iMinLoad = INT_MAX;
		int iMinLoadProxyID = -1;

		for( mit = m_mapProxy[PT_CENTER].begin(); mit != m_mapProxy[PT_CENTER].end(); ++mit )
		{
			if( mit->second == NULL )
				continue;

			//{{ 2010. 05. 10  ������	�������� ������ ����
#ifdef SERV_INTEGRATION
			if( mit->second->IsSameServerGroup() == false )
				continue;
#endif SERV_INTEGRATION
			//}}
			
			int iLoad = mit->second->GetLoad_NEW( iType );
			if( iMinLoad > iLoad )
			{
				iMinLoad = iLoad;
				iMinLoadProxyID = mit->first;
			}
		}

		return iMinLoadProxyID;
	}
	else
	{
		return ( int )KncUid::ExtractServerID( nRoomUID );
	}
}

#endif SERV_ROOM_COUNT
//}}

//{{ 2010. 02. 16  ������	ä����Ƽ����
#ifdef SERV_CHANNEL_PARTY

int KProxyManager::GetPartyProxyID( UidType nPartyUID )
{
	//{{ 2009. 11. 23  ������	���������ӱ�������
	KLocker lock( m_csProxy );
	//}}

	if( nPartyUID == 0 )
	{
		std::map< int, KActorProxyPtr >::iterator mit;
		int iMinLoad = INT_MAX;
		int iMinLoadProxyID = -1;

		for( mit = m_mapProxy[PT_CENTER].begin(); mit != m_mapProxy[PT_CENTER].end(); ++mit )
		{
			if( mit->second == NULL )
				continue;

			//{{ 2010. 05. 10  ������	�������� ������ ����
#ifdef SERV_INTEGRATION
			if( mit->second->IsSameServerGroup() == false )
				continue;
#endif SERV_INTEGRATION
			//}}
			
			int iLoad = mit->second->GetLoad_NEW( CXSLPartyManager::PE_PARTY_COUNT_TYPE );
			if( iMinLoad > iLoad )
			{
				iMinLoad = iLoad;
				iMinLoadProxyID = mit->first;
			}			
		}

		return iMinLoadProxyID;
	}
	else
	{
		return ( int )KncUid::ExtractServerID( nPartyUID );
	}
}

#endif SERV_CHANNEL_PARTY
//}}

int KProxyManager::GetRoomProxyID( UidType nRoomUID )
{
	//{{ 2009. 11. 23  ������	���������ӱ�������
	KLocker lock( m_csProxy );
	//}}

    if( nRoomUID == 0 )
    {
        std::map< int, KActorProxyPtr >::iterator mit;
        int iMinLoad = INT_MAX;
        int iMinLoadProxyID = -1;

        for( mit = m_mapProxy[PT_CENTER].begin(); mit != m_mapProxy[PT_CENTER].end(); ++mit )
        {
            if( mit->second != NULL )
            {
                int iLoad = mit->second->GetLoad();
                if( iMinLoad > iLoad )
                {
                    iMinLoad = iLoad;
                    iMinLoadProxyID = mit->first;
                }
            }
        }

        return iMinLoadProxyID;
    }
    else
    {
		return ( int )KncUid::ExtractServerID( nRoomUID );
    }
}

KActorProxyPtr KProxyManager::GetRoomProxy( UidType nRoomUID, u_short usEventID )
{
	//{{ 2009. 11. 23  ������	���������ӱ�������
	KLocker lock( m_csProxy );
	//}}

	//{{ 2010. 01. 27  ������	���������
#ifdef SERV_ROOM_COUNT
	int nProxyID = GetRoomProxyID_NEW( nRoomUID, usEventID );
#else
	int nProxyID = GetRoomProxyID( nRoomUID );
#endif SERV_ROOM_COUNT
	//}}

    std::map< int, KActorProxyPtr >::iterator mit;
    mit = m_mapProxy[PT_CENTER].find( nProxyID );

    if( mit == m_mapProxy[PT_CENTER].end() )
        return KActorProxyPtr();

    return mit->second;
}

//{{ 2010. 02. 16  ������	ä����Ƽ����
#ifdef SERV_CHANNEL_PARTY

KActorProxyPtr KProxyManager::GetPartyProxy( UidType nPartyUID )
{
	//{{ 2009. 11. 23  ������	���������ӱ�������
	KLocker lock( m_csProxy );
	//}}

	int nProxyID = GetPartyProxyID( nPartyUID );

	std::map< int, KActorProxyPtr >::iterator mit;
	mit = m_mapProxy[PT_CENTER].find( nProxyID );

	if( mit == m_mapProxy[PT_CENTER].end() )
		return KActorProxyPtr();

	return mit->second;
}

#endif SERV_CHANNEL_PARTY
//}}

int KProxyManager::GetLoginProxyID()
{
	//{{ 2009. 11. 23  ������	���������ӱ�������
	KLocker lock( m_csProxy );
	//}}

	// �ϴ� �α� ������ �ϳ���� �����Ѵ�.
	if( m_mapProxy[PT_LOGIN].empty() )
	{
		return -1;
	}

	return m_mapProxy[PT_LOGIN].begin()->first;
}

KActorProxyPtr KProxyManager::GetLoginProxy()
{
	//{{ 2009. 11. 23  ������	���������ӱ�������
	KLocker lock( m_csProxy );
	//}}

	// �ϴ� �α� ������ �ϳ���� �����Ѵ�.
	if( m_mapProxy[PT_LOGIN].empty() )
	{
		return KActorProxyPtr();
	}
	else
	{
		if( m_mapProxy[PT_LOGIN].begin()->second )
		{
			return m_mapProxy[PT_LOGIN].begin()->second;
		}
		else
		{
			return KActorProxyPtr();
		}
	}
}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
int KProxyManager::GetGlobalProxyID()
{
	//{{ 2009. 11. 23  ������	���������ӱ�������
	KLocker lock( m_csProxy );
	//}}

	// �ϴ� �α� ������ �ϳ���� �����Ѵ�.
	if( m_mapProxy[PT_GLOBAL].empty() )
	{
		return -1;
	}

	return m_mapProxy[PT_GLOBAL].begin()->first;
}

KActorProxyPtr KProxyManager::GetGlobalProxy()
{
	//{{ 2009. 11. 23  ������	���������ӱ�������
	KLocker lock( m_csProxy );
	//}}

	// �ϴ� �α� ������ �ϳ���� �����Ѵ�.
	if( m_mapProxy[PT_GLOBAL].empty() )
	{
		return KActorProxyPtr();
	}
	else
	{
		if( m_mapProxy[PT_GLOBAL].begin()->second )
		{
			return m_mapProxy[PT_GLOBAL].begin()->second;
		}
		else
		{
			return KActorProxyPtr();
		}
	}
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

//{{ 2010. 05. 17  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION

void KProxyManager::AddSameServerGroupProxyID( int iProxyID )
{
	//{{ 2009. 11. 23  ������	���������ӱ�������
	KLocker lock( m_csProxy );
	//}}

	// vector�� �ش� ProxyID�� ������ push_back�Ѵ�.
	if( std::find( m_vecSameServerGroupProxy.begin(), m_vecSameServerGroupProxy.end(), iProxyID ) == m_vecSameServerGroupProxy.end() )
	{
		m_vecSameServerGroupProxy.push_back( iProxyID );
	}
}

void KProxyManager::DelSameServerGroupProxyID( int iProxyID )
{
	//{{ 2009. 11. 23  ������	���������ӱ�������
	KLocker lock( m_csProxy );
	//}}

	std::vector< int >::iterator vit;
	vit = std::find( m_vecSameServerGroupProxy.begin(), m_vecSameServerGroupProxy.end(), iProxyID );

	if( vit != m_vecSameServerGroupProxy.end() )
	{
		m_vecSameServerGroupProxy.erase( vit );
	}
}

#endif SERV_INTEGRATION
//}}

//{{ 2009. 11. 23  ������	���������ӱ�������
void KProxyManager::KeepConnectionThread()
{
	std::vector< KProxyInfo > vecDisconnectedProxyList;
	GetDisconnectedProxyList( vecDisconnectedProxyList );

	for( u_int ui = 0; ui < vecDisconnectedProxyList.size(); ++ui )
	{
		Connect( vecDisconnectedProxyList[ui].m_iProxyType, vecDisconnectedProxyList[ui].m_iProxyID );
	}
}
//}}



