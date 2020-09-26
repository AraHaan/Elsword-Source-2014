#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "LoginLogDBThread.h"
#include "LoginServer.h"
#include "ActorManager.h"
#include "NetError.h"
#include "Enum/Enum.h"

#define CLASS_TYPE KLoginLogDBThread
ImplPfID( KLoginLogDBThread, PI_LOGIN_LOG_DB );

IMPL_PROFILER_DUMP( KLoginLogDBThread )
{
	for( unsigned int ui = 0; ui < vecDump.size(); ++ui )
	{
		unsigned int iAvg = 0;
		if( vecDump[ui].m_iQueryCount > 0 )	iAvg = vecDump[ui].m_iTotalTime / vecDump[ui].m_iQueryCount;		

#ifdef SERV_RENEWAL_SP
		DO_QUERY_NO_PROFILE( L"exec dbo.P_QueryStats_INS", L"%d, N\'%s\', %d, %d, %d, %d, %d, %d",
			% KBaseServer::GetKObj()->GetServerGroupID()
			% vecDump[ui].m_wstrQuery
			% vecDump[ui].m_iMinTime
			% iAvg
			% vecDump[ui].m_iMaxTime
			% vecDump[ui].m_iOver1Sec
			% vecDump[ui].m_iQueryCount
			% vecDump[ui].m_iQueryFail
			);
#else //SERV_RENEWAL_SP
		DO_QUERY_NO_PROFILE( L"exec dbo.lup_insert_querystats", L"N\'%s\', %d, %d, %d, %d, %d, %d",
			% vecDump[ui].m_wstrQuery
			% vecDump[ui].m_iMinTime
			% iAvg
			% vecDump[ui].m_iMaxTime
			% vecDump[ui].m_iOver1Sec
			% vecDump[ui].m_iQueryCount
			% vecDump[ui].m_iQueryFail
			);
#endif //SERV_RENEWAL_SP

		continue;

end_proc:
		START_LOG( cerr, vecDump[ui].m_wstrQuery )
			<< BUILD_LOG( vecDump[ui].m_iMinTime )
			<< BUILD_LOG( vecDump[ui].m_iMaxTime )
			<< BUILD_LOG( vecDump[ui].m_iTotalTime )
			<< BUILD_LOG( vecDump[ui].m_iQueryCount )
			<< BUILD_LOG( iAvg )
			<< BUILD_LOG( vecDump[ui].m_iOver1Sec )
			<< BUILD_LOG( vecDump[ui].m_iQueryFail )
			<< END_LOG;
	}
}

KLoginLogDBThread::~KLoginLogDBThread(void)
{
}

void KLoginLogDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {
       _CASE( DBE_SERVER_ON_REQ, KNetAddress );
	   //{{ 2011. 07. 19	������	������ ���� �ڵ� �����丵
	   //#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING	   
	   //{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	   _CASE( DBE_VERIFY_SERVER_CONNECT_REQ, KECN_VERIFY_SERVER_CONNECT_REQ );
#else
	   _CASE( DBE_VERIFY_SERVER_CONNECT_REQ, KNetAddress );
#endif SERV_INTEGRATION
	   //}}
	   //#endif SERV_SERVER_CONNECT_CODE_REFACTORING
	   //}}
	    //{{ 2008. 5. 27  ������  ü��ID ����
	    CASE( DBE_UPDATE_SERVER_INFO_REQ );
	    //}}       
		//{{ 2009. 7. 6  ������		��ŷ ����		
		CASE_NOPARAM( DBE_GET_HENIR_RANKING_INFO_REQ );
		CASE( DBE_DELETE_RANKING_INFO_NOT );
		CASE( DBE_BACKUP_RANKING_INFO_NOT );
		//}}
		//{{ 2010. 06. 01  ������	���� ó�� ����
#ifdef SERV_CCU_NEW
		CASE( DBE_UPDATE_CCU_INFO_NOT );
#endif SERV_CCU_NEW
		//}}
		//{{ 2010. 10. 13	������	DLL Manager
#ifdef SERV_DLL_MANAGER
		CASE_NOPARAM( DBE_GET_MODULE_INFO_LIST_REQ );
	   _CASE( DBE_UPDATE_MODULE_INFO_LIST_REQ, KDBE_GET_MODULE_INFO_LIST_ACK );
#endif SERV_DLL_MANAGER
		//}}
		//{{ 2010. 12. 20	������	Ư�� �ð� ť�� ����
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
		CASE_NOPARAM( DBE_GET_TIME_RANDOM_ITEM_EVENT_INFO_REQ );
		CASE( DBE_UPDATE_TIME_RANDOM_ITEM_EVENT_RESULT_NOT );
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
		//}}
		//{{ 2011. 05. 11	������	�븮����
#ifdef SERV_PSHOP_AGENCY
		CASE( ELOG_INSERT_PERSONAL_SHOP_LOG_NOT );
#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2013. 02. 13	������	Tick�����ս� DB�α� ���
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
		CASE( DBE_TICK_PERFORMANCE_LOG_NOT );
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
		//}}
		//{{ 2013. 02. 21	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
		CASE( DBE_LOG_STATISTICS_INFO_NOT );
#endif SERV_LOG_SYSTEM_NEW
		//}}

        CASE_NOPARAM( DBE_SERVER_OFF_NOT );
		
    default:
        START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

void KLoginLogDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KLoginLogDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
    SendToUser( nTo, usEventID, char() );
}

void KLoginLogDBThread::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID )
{
    SendToGSUser( nGS, nTo, usEventID, char() );
}

_IMPL_ON_FUNC( DBE_SERVER_ON_REQ, KNetAddress )
{
    KDBE_LOGIN_SERVER_ON_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	//{{ 2013. 01. 17	������	������ ��� �缳IP ����
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_SEL_VerifyPublicIP", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% KBaseServer::ESC_LOGIN 
		% kPacket_.m_wstrIP 
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_By_PublicIP_SEL", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% KBaseServer::ESC_LOGIN 
		% kPacket_.m_wstrIP 
		);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_GET_verify_PublicIP", L"%d, N\'%s\'", 
		% KBaseServer::ESC_LOGIN 
		% kPacket_.m_wstrIP 
		);
#endif //SERV_RENEWAL_SP
#else //SERV_PRIVATE_IP_SERVER_NETWORKING
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_SEL", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID()
		% KBaseServer::ESC_LOGIN 
		% kPacket_.m_wstrIP 
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_Verify_On_SEL", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID()
		% KBaseServer::ESC_LOGIN 
		% kPacket_.m_wstrIP 
		);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_verify_server_on", L"%d, N\'%s\'", 
		% KBaseServer::ESC_LOGIN 
		% kPacket_.m_wstrIP 
		);
#endif //SERV_RENEWAL_SP
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}	

    if( !m_kODBC.BeginFetch() )
    {
        START_LOG( cerr, L"�α� ���� ���� ����." )
            << BUILD_LOG( kPacket_.m_wstrIP )
			<< END_LOG;

        kPacket.m_iOK = NetError::ERR_VERIFY_02;
        goto end_proc;
    }

	//{{ 2013. 01. 17	������	������ ��� �缳IP ����
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
	FETCH_DATA( kPacket.m_kServerInfo.m_iUID 
		>> kPacket.m_kServerInfo.m_wstrName
		>> kPacket.m_kServerInfo.m_iServerGroupID
		>> kPacket.m_kServerInfo.m_iServerClass
		>> kPacket.m_kServerInfo.m_wstrPublicIP
		>> kPacket.m_kServerInfo.m_wstrPrivateIP
		>> kPacket.m_kServerInfo.m_usMasterPort
		>> kPacket.m_kServerInfo.m_usNCUDPPort
		>> kPacket.m_kServerInfo.m_nMaxUser
		>> kPacket.m_kServerInfo.m_nCurrentUser
		>> kPacket.m_kServerInfo.m_bOn );
#else
	FETCH_DATA( kPacket.m_kServerInfo.m_iUID 
		>> kPacket.m_kServerInfo.m_wstrName
		>> kPacket.m_kServerInfo.m_iServerGroupID
		>> kPacket.m_kServerInfo.m_iServerClass
		>> kPacket.m_kServerInfo.m_wstrIP
		>> kPacket.m_kServerInfo.m_usMasterPort
		>> kPacket.m_kServerInfo.m_usNCUDPPort
		>> kPacket.m_kServerInfo.m_nMaxUser
		>> kPacket.m_kServerInfo.m_nCurrentUser
		>> kPacket.m_kServerInfo.m_bOn );
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}    

    m_kODBC.EndFetch();

	//{{ 2011. 07. 22	������	��� ���� SP���
#ifdef SERV_ALL_DB_QUERY_USE_SP
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LLoginProxy_SEL_ByServerUID", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID()
																	 % kPacket.m_kServerInfo.m_iUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LLoginProxy_By_ServerUID_SEL", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID()
																	 % kPacket.m_kServerInfo.m_iUID );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_get_login_proxy_id_by_serveruid", L"%d", % kPacket.m_kServerInfo.m_iUID );
#endif //SERV_RENEWAL_SP
#else
	// ProxyID�� �޾ƿ´�.
	//DO_QUERY_NO_PROFILE( L"SELECT ProxyID FROM dbo.LLoginProxy(NOLOCK)", L"WHERE ServerUID = %d", % kPacket.m_kServerInfo.m_iUID );
#endif SERV_ALL_DB_QUERY_USE_SP
	//}}	
	if( !m_kODBC.BeginFetch() )
	{
		START_LOG( cerr, L"�� �������� �α� ������." )
			<< BUILD_LOG( kPacket.m_kServerInfo.m_iUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_VERIFY_13;
		goto end_proc;
	}

	FETCH_DATA( kPacket.m_iProxyID );
	m_kODBC.EndFetch();

    //������ �����ٰ� ǥ���Ѵ�.
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_UPD_Enable", L"%d, %d, 1", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket.m_kServerInfo.m_iUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_UPD", L"%d, %d, 1", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket.m_kServerInfo.m_iUID );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
    DO_QUERY( L"exec dbo.lup_set_server_enable_flag", L"%d, 1", % kPacket.m_kServerInfo.m_iUID );
#endif //SERV_RENEWAL_SP

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK );
        m_kODBC.EndFetch();

        if( kPacket.m_iOK != NetError::NET_OK )
        {
            START_LOG( cerr, L"���� ���� ǥ�� ����." )
                << BUILD_LOG( kPacket.m_kServerInfo.m_iUID )
                << END_LOG;

            kPacket.m_iOK = NetError::ERR_VERIFY_08;
        }
    }

end_proc:
    //��� ������
    SendToServer( DBE_SERVER_ON_ACK, kPacket );
}

//{{ 2011. 07. 19	������	������ ���� �ڵ� �����丵
//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
//{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION

_IMPL_ON_FUNC( DBE_VERIFY_SERVER_CONNECT_REQ, KECN_VERIFY_SERVER_CONNECT_REQ )
{
	KECN_VERIFY_SERVER_CONNECT_ACK kPacket;
	kPacket.m_nOK = NetError::ERR_ODBC_01;
	kPacket.m_iServerUID = KBaseServer::GetKObj()->GetUID();

#ifdef SERV_RENEWAL_SP
//////////////////////////////////////////////////////////////////////////
	int iServerSetID = KBaseServer::GetKObj()->GetServerGroupID();
	bool bUsePrivateIP = false;
	int iServerType = (int)KBaseServer::ESC_GAME;	// �������� Login Server���� ���� ������ �ٴ´�

#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
	bUsePrivateIP = true;
#endif // SERV_PRIVATE_IP_SERVER_NETWORKING

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	iServerType = 0;	// �߱��� ��� Login Server�� ChannelServer, GameServer �� ������ ����.
#endif // SERV_FROM_CHANNEL_TO_LOGIN_PROXY

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_PROFILE( L"exec dbo.P_LServerList_SEL_VerifyConnect", L"%d, %d, N\'%s\', %d, %d", 
		% iServerSetID
		% iServerType
		% kPacket_.m_kNetAddress.m_wstrIP 
		% kPacket_.m_kNetAddress.m_usPort
		% (int)bUsePrivateIP );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_PROFILE( L"exec dbo.P_LServerList_Verify_Connect_SEL", L"%d, %d, N\'%s\', %d, %d", 
		% iServerSetID
		% iServerType
		% kPacket_.m_kNetAddress.m_wstrIP 
		% kPacket_.m_kNetAddress.m_usPort
		% (int)bUsePrivateIP );
#endif //SERV_ALL_RENEWAL_SP
//////////////////////////////////////////////////////////////////////////
#else //SERV_RENEWAL_SP
	//{{ 2013. 01. 21	������	������ ��� �缳IP ����
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_PROFILE( L"exec dbo.P_LServerList_SEL_VerifyConnect", L"%d, N\'%s\', %d", % KBaseServer::ESC_GAME % kPacket_.m_kNetAddress.m_wstrIP % kPacket_.m_kNetAddress.m_usPort );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_PROFILE( L"exec dbo.P_LServerList_GET_verify_connect", L"%d, N\'%s\', %d", % KBaseServer::ESC_GAME % kPacket_.m_kNetAddress.m_wstrIP % kPacket_.m_kNetAddress.m_usPort );
#endif //SERV_ALL_RENEWAL_SP
#else
	DO_QUERY_NO_PROFILE( L"exec dbo.lup_verify_server_connect", L"%d, N\'%s\', %d", % KBaseServer::ESC_GAME % kPacket_.m_kNetAddress.m_wstrIP % kPacket_.m_kNetAddress.m_usPort );
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		//{{ 2010. 04. 22  ������	������ ��ü ����
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
		FETCH_DATA( kPacket.m_nUID
			>> kPacket.m_iDBRegServerGroupID
			>> kPacket.m_strName
			>> kPacket.m_sMaxNum
			>> kPacket.m_iServerType );
#else
		FETCH_DATA( kPacket.m_nUID
			>> kPacket.m_iDBRegServerGroupID
			>> kPacket.m_strName
			>> kPacket.m_sMaxNum );
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY
		//}}

		m_kODBC.EndFetch();

		kPacket.m_nOK = NetError::NET_OK;
	}
	else
	{
		START_LOG( cerr, LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_kNetAddress.m_wstrIP )
			<< BUILD_LOG( kPacket_.m_kNetAddress.m_usPort );
	}

#ifdef	SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	switch(kPacket.m_iServerType)
	{
	case KBaseServer::ESC_GAME:
	case KBaseServer::ESC_CHANNEL:
		kPacket.m_nOK = NetError::NET_OK;
		break;
	default:
		kPacket.m_nOK = NetError::ERR_ODBC_01;

		START_LOG( cerr, L"��ȿ���� ���� ���� Ÿ��" )
			<< BUILD_LOG( kPacket.m_iServerType );
		break;
	}
#endif	SERV_FROM_CHANNEL_TO_LOGIN_PROXY

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_VERIFY_SERVER_CONNECT_ACK, kPacket );
}

#else

_IMPL_ON_FUNC( DBE_VERIFY_SERVER_CONNECT_REQ, KNetAddress )
{
	KECN_VERIFY_SERVER_CONNECT_ACK kPacket;
	kPacket.m_nOK = NetError::ERR_ODBC_01;
	kPacket.m_iServerUID = KBaseServer::GetKObj()->GetUID();

	DO_QUERY_NO_PROFILE( L"SELECT UID, ServerSetID, Name, MaxConcurrentUser FROM dbo.LServerList(NOLOCK)", L"WHERE ServerType = 3 AND IP = N\'%s\' AND Port1 = %d",
		% kPacket_.m_wstrIP
		% kPacket_.m_usPort );

	if( m_kODBC.BeginFetch() )
	{	
		FETCH_DATA( kPacket.m_nUID 
			>> kPacket.m_sGroupID
			>> kPacket.m_strName
			>> kPacket.m_sMaxNum );

		m_kODBC.EndFetch();

		kPacket.m_nOK = NetError::NET_OK;
	}
	else
	{
		START_LOG( cerr, LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_wstrIP )
			<< BUILD_LOG( kPacket_.m_usPort );
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_VERIFY_SERVER_CONNECT_ACK, kPacket );
}


#endif SERV_INTEGRATION
//}}
//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
//}}

//{{ 2008. 5. 27  ������  ü��ID ����
IMPL_ON_FUNC( DBE_UPDATE_SERVER_INFO_REQ )
{
	// ������ DB�� ���´�.
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatCCUMinuteGuestUser_INS", L"%d, N\'%s\', %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_wstrRegDate.c_str() 
		% kPacket_.m_iServerUID 
		% kPacket_.m_iConcurrentUser 
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_update_concurrent_guestuser", L"N\'%s\', %d, %d", 
		% kPacket_.m_wstrRegDate.c_str() 
		% kPacket_.m_iServerUID 
		% kPacket_.m_iConcurrentUser 
		);
#endif //SERV_RENEWAL_SP

	// ��ȯ���� ���� stored procedure��

end_proc:
	SendToServer( DBE_UPDATE_SERVER_INFO_ACK );
}

//void KLoginLogDBThread::WriteServerInfoToDB( IN KServerInfo kPacket_, OUT KServerList& kPacket )
//{
//    // ������ DB�� ���´�.
//    int iOK;    
//	DO_QUERY( ( L"exec dbo.lup_update_concurrent_user %d, %d" ) % kPacket_.m_iUID % kPacket_.m_iConcurrentUser ); // �ּ� ������ P_LServerList_CCU_UPD ����ؾ���
//    if( m_kODBC.BeginFetch() )
//    {
//        FETCH_DATA( iOK );
//        m_kODBC.EndFetch();
//    }
//
//    // DB�κ��� ���� ����Ʈ �����͸� �޾ƿ´�
//    DO_QUERY( ( L"exec dbo.lup_get_server_list" ) );  // �ּ������� P_LServerList_SEL �̰� ����ؾ���
//
//    while( m_kODBC.Fetch() )
//    {
//        KServerInfo kServerInfo;
//
//        FETCH_DATA( kServerInfo.m_iUID
//            >> kServerInfo.m_wstrName
//            >> kServerInfo.m_iServerGroupID
//            >> kServerInfo.m_iServerClass
//            >> kServerInfo.m_wstrIP
//            >> kServerInfo.m_usMasterPort
//            >> kServerInfo.m_usNCUDPPort
//            >> kServerInfo.m_nMaxUser
//            >> kServerInfo.m_nCurrentUser
//            >> kServerInfo.m_bOn );
//
//
//        std::pair< KServerList::iterator, bool > pairIB;
//        std::vector< KServerInfo > vecServerList;
//        pairIB = kPacket.insert( std::make_pair( kServerInfo.m_iServerGroupID, vecServerList ) );
//        pairIB.first->second.push_back( kServerInfo );
//    }
//
//end_proc:
//    return;
//}

//{{ 2009. 7. 6  ������		��ŷ ����
IMPL_ON_FUNC_NOPARAM( DBE_GET_HENIR_RANKING_INFO_REQ )
{
	KDBE_GET_HENIR_RANKING_INFO_ACK kPacket;	

	//////////////////////////////////////////////////////////////////////////
	// �ϰ� ��ϸ� �ð� ��ŷ

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Rank_SpaceTime_Day_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.lup_get_rank_spacetime_day" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		CTime tRegDate;
		std::wstring wstrRegDate;
		KHenirRankingInfo kInfo;

		FETCH_DATA( kInfo.m_iRank
			>> kInfo.m_iUnitUID
			>> kInfo.m_wstrNickName
			>> kInfo.m_cUnitClass
			>> kInfo.m_ucLevel
			>> kInfo.m_iStageCount
			>> kInfo.m_ulPlayTime
			>> wstrRegDate
			);

		// RegDate ��ȯ
		KncUtil::ConvertStringToCTime( wstrRegDate, tRegDate );
		kInfo.m_tRegDate = tRegDate.GetTime();

		// RankingInfo ����
		std::map< int , std::vector< KHenirRankingInfo > >::iterator mit;
		mit = kPacket.m_mapHenirRanking.find( SEnum::RT_DAY_RANKING );
		if( mit == kPacket.m_mapHenirRanking.end() )
		{
			std::vector< KHenirRankingInfo > vecRanking;
			vecRanking.push_back( kInfo );
			kPacket.m_mapHenirRanking.insert( std::make_pair( SEnum::RT_DAY_RANKING, vecRanking ) );
		}
		else
		{
			mit->second.push_back( kInfo );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// �ְ� ��ϸ� �ð� ��ŷ

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Rank_SpaceTime_Week_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.lup_get_rank_spacetime_week" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		CTime tRegDate;
		std::wstring wstrRegDate;
		KHenirRankingInfo kInfo;

		FETCH_DATA( kInfo.m_iRank
			>> kInfo.m_iUnitUID
			>> kInfo.m_wstrNickName
			>> kInfo.m_cUnitClass
			>> kInfo.m_ucLevel
			>> kInfo.m_iStageCount
			>> kInfo.m_ulPlayTime
			>> wstrRegDate
			);

		// RegDate ��ȯ
		KncUtil::ConvertStringToCTime( wstrRegDate, tRegDate );
		kInfo.m_tRegDate = tRegDate.GetTime();

		// RankingInfo ����
		std::map< int , std::vector< KHenirRankingInfo > >::iterator mit;
		mit = kPacket.m_mapHenirRanking.find( SEnum::RT_WEEK_RANKING );
		if( mit == kPacket.m_mapHenirRanking.end() )
		{
			std::vector< KHenirRankingInfo > vecRanking;
			vecRanking.push_back( kInfo );
			kPacket.m_mapHenirRanking.insert( std::make_pair( SEnum::RT_WEEK_RANKING, vecRanking ) );
		}
		else
		{
			mit->second.push_back( kInfo );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� ��ϸ� �ð� ��ŷ

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Rank_SpaceTime_Month_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.lup_get_rank_spacetime_month" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		CTime tRegDate;
		std::wstring wstrRegDate;
		KHenirRankingInfo kInfo;

		FETCH_DATA( kInfo.m_iRank
			>> kInfo.m_iUnitUID
			>> kInfo.m_wstrNickName
			>> kInfo.m_cUnitClass
			>> kInfo.m_ucLevel
			>> kInfo.m_iStageCount
			>> kInfo.m_ulPlayTime
			>> wstrRegDate
			);

		// RegDate ��ȯ
		KncUtil::ConvertStringToCTime( wstrRegDate, tRegDate );
		kInfo.m_tRegDate = tRegDate.GetTime();

		// RankingInfo ����
		std::map< int , std::vector< KHenirRankingInfo > >::iterator mit;
		mit = kPacket.m_mapHenirRanking.find( SEnum::RT_MONTH_RANKING );
		if( mit == kPacket.m_mapHenirRanking.end() )
		{
			std::vector< KHenirRankingInfo > vecRanking;
			vecRanking.push_back( kInfo );
			kPacket.m_mapHenirRanking.insert( std::make_pair( SEnum::RT_MONTH_RANKING, vecRanking ) );
		}
		else
		{
			mit->second.push_back( kInfo );
		}
	}

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-17	// �ڼ���
	//////////////////////////////////////////////////////////////////////////
	// ���� ��ϸ� �ð� ��ŷ

	DO_QUERY_NO_ARG( L"exec dbo.P_Rank_SpaceTime_Hero_Month_SEL" );

	while( m_kODBC.Fetch() )
	{
		CTime tRegDate;
		std::wstring wstrRegDate;
		KHenirRankingInfo kInfo;

		FETCH_DATA( kInfo.m_iRank
			>> kInfo.m_iUnitUID
			>> kInfo.m_wstrNickName
			>> kInfo.m_cUnitClass
			>> kInfo.m_ucLevel
			>> kInfo.m_iStageCount
			>> kInfo.m_ulPlayTime
			>> wstrRegDate
			);

		// RegDate ��ȯ
		KncUtil::ConvertStringToCTime( wstrRegDate, tRegDate );
		kInfo.m_tRegDate = tRegDate.GetTime();

		//kInfo.m_bytePlayStartedUserNum = 1;	// ������� �ʱ� ������ ���� ����� �ʿ� ����.

		// RankingInfo ����
		std::map< int , std::vector< KHenirRankingInfo > >::iterator mit;
		mit = kPacket.m_mapHenirRanking.find( SEnum::RT_HERO_RANKING );
		if( mit == kPacket.m_mapHenirRanking.end() )
		{
			std::vector< KHenirRankingInfo > vecRanking;
			vecRanking.push_back( kInfo );
			kPacket.m_mapHenirRanking.insert( std::make_pair( SEnum::RT_HERO_RANKING, vecRanking ) );
		}
		else
		{
			mit->second.push_back( kInfo );
		}
	}
#endif // SERV_HENIR_RENEWAL_2013

end_proc:
	SendToServer( DBE_GET_HENIR_RANKING_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_DELETE_RANKING_INFO_NOT )
{
	// �ش� Ÿ���� ��ŷ�� �ʱ�ȭ
	switch( kPacket_.m_iRankingType )
	{
	case SEnum::RT_DAY_RANKING:
		{
			// �ϰ� ����
			//{{ 2011. 09. 07	������	��ϸ� �ð� ��ŷ ��� ������Ʈ
#ifdef SERV_HENIR_RANKING_BACKUP_UPDATE
#else
#ifdef SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_Rank_SpaceTime_Day_DEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
			DO_QUERY_NO_ARG( L"exec dbo.lup_delete_rank_spacetime_day" );
#endif //SERV_RENEWAL_SP
#endif SERV_HENIR_RANKING_BACKUP_UPDATE
			//}}

			START_LOG( cout, L"��ϸ� �ð� ���� ��ŷ DB���� ���� �Ϸ�!" );
		}
		break;

	case SEnum::RT_WEEK_RANKING:
		{
			// �ְ� ����
			//{{ 2011. 09. 07	������	��ϸ� �ð� ��ŷ ��� ������Ʈ
#ifdef SERV_HENIR_RANKING_BACKUP_UPDATE
#else
#ifdef SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_Rank_SpaceTime_Week_DEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
			DO_QUERY_NO_ARG( L"exec dbo.lup_delete_rank_spacetime_week" );
#endif //SERV_RENEWAL_SP
#endif SERV_HENIR_RANKING_BACKUP_UPDATE
			//}}			

            START_LOG( cout, L"��ϸ� �ð� �ְ� ��ŷ DB���� ���� �Ϸ�!" );
		}
		break;

	case SEnum::RT_MONTH_RANKING:
		{
			// ���� ����
			//{{ 2011. 09. 07	������	��ϸ� �ð� ��ŷ ��� ������Ʈ
#ifdef SERV_HENIR_RANKING_BACKUP_UPDATE
#else
#ifdef SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_Rank_SpaceTime_Month_DEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
			DO_QUERY_NO_ARG( L"exec dbo.lup_delete_rank_spacetime_month" );
#endif //SERV_RENEWAL_SP
#endif SERV_HENIR_RANKING_BACKUP_UPDATE
			//}}

			START_LOG( cout, L"��ϸ� �ð� ���� ��ŷ DB���� ���� �Ϸ�!" );
		}
		break;

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-17	// �ڼ���
	case SEnum::RT_HERO_RANKING:
		{
			// ���� ��ŷ ����
			//{{ 2011. 09. 07	������	��ϸ� �ð� ��ŷ ��� ������Ʈ
#ifdef SERV_HENIR_RANKING_BACKUP_UPDATE
#else
			DO_QUERY_NO_ARG( L"exec dbo.P_Rank_SpaceTime_Hero_Month_DEL" );
#endif SERV_HENIR_RANKING_BACKUP_UPDATE
			//}}

			START_LOG( cout, L"��ϸ� �ð� ���� ��ŷ DB���� ���� �Ϸ�!" );
		}
		break;
#endif // SERV_HENIR_RENEWAL_2013

	default:
		{
			START_LOG( cerr, L"DB���̺��� ���� ��ŷŸ���� �ʱ�ȭ�Ϸ� �Ͽ����ϴ�" )
				<< BUILD_LOG( kPacket_.m_iRankingType )
				<< END_LOG;
		}
	}

	//{{ 2011. 09. 07	������	��ϸ� �ð� ��ŷ ��� ������Ʈ
#ifdef SERV_HENIR_RANKING_BACKUP_UPDATE
#else
end_proc:
	return;
#endif SERV_HENIR_RANKING_BACKUP_UPDATE
	//}}
}

IMPL_ON_FUNC( DBE_BACKUP_RANKING_INFO_NOT )
{
	int iOK = NetError::ERR_ODBC_01;
	//{{ 2011. 09. 07	������	��ϸ� �ð� ��ŷ ��� ������Ʈ
#if defined (SERV_HENIR_RANKING_BACKUP_UPDATE) || defined(SERV_RENEWAL_SP)
	const CTime tCurTime = CTime::GetCurrentTime();
	const std::wstring wstrCurDate = ( CStringW )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
#endif SERV_HENIR_RANKING_BACKUP_UPDATE //SERV_RENEWAL_SP
	//}}

	std::map< int, std::vector< KHenirRankingInfo > >::const_iterator mit;
	for( mit = kPacket_.m_mapHenirRanking.begin(); mit != kPacket_.m_mapHenirRanking.end(); ++mit )
	{
		const int iRankingType = mit->first;
		const std::vector< KHenirRankingInfo >& vecRanking = mit->second;

		switch( iRankingType )
		{
		case SEnum::RT_DAY_RANKING:
			{
				// �ϰ� ����				
				//{{ 2011. 09. 07	������	��ϸ� �ð� ��ŷ ��� ������Ʈ
#ifdef SERV_HENIR_RANKING_BACKUP_UPDATE
#else
#ifdef SERV_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_Rank_SpaceTime_Day_DEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
				DO_QUERY_NO_ARG( L"exec dbo.lup_delete_rank_spacetime_day" );
#endif //SERV_RENEWAL_SP
#endif SERV_HENIR_RANKING_BACKUP_UPDATE
				//}}
			}
			break;

		case SEnum::RT_WEEK_RANKING:
			{
				// �ְ� ����
				//{{ 2011. 09. 07	������	��ϸ� �ð� ��ŷ ��� ������Ʈ
#ifdef SERV_HENIR_RANKING_BACKUP_UPDATE
#else
#ifdef SERV_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_Rank_SpaceTime_Week_DEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
				DO_QUERY_NO_ARG( L"exec dbo.lup_delete_rank_spacetime_week" );
#endif //SERV_RENEWAL_SP
#endif SERV_HENIR_RANKING_BACKUP_UPDATE
				//}}				
			}
			break;

		case SEnum::RT_MONTH_RANKING:
			{
				// ���� ����
				//{{ 2011. 09. 07	������	��ϸ� �ð� ��ŷ ��� ������Ʈ
#ifdef SERV_HENIR_RANKING_BACKUP_UPDATE
#else
#ifdef SERV_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_Rank_SpaceTime_Month_DEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
				DO_QUERY_NO_ARG( L"exec dbo.lup_delete_rank_spacetime_month" );
#endif //SERV_RENEWAL_SP
#endif SERV_HENIR_RANKING_BACKUP_UPDATE
				//}}				
			}
			break;

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-17	// �ڼ���
		case SEnum::RT_HERO_RANKING:
			{
				// ���� ��ŷ ����
				//{{ 2011. 09. 07	������	��ϸ� �ð� ��ŷ ��� ������Ʈ
#ifdef SERV_HENIR_RANKING_BACKUP_UPDATE
#else
				DO_QUERY_NO_ARG( L"exec dbo.P_Rank_SpaceTime_Hero_Month_DEL" );
#endif SERV_HENIR_RANKING_BACKUP_UPDATE
				//}}				
			}
			break;
#endif // SERV_HENIR_RENEWAL_2013

		default:
			{
				START_LOG( cerr, L"DB���̺��� ���� ��ŷŸ���� �ʱ�ȭ�Ϸ� �Ͽ����ϴ�" )
					<< BUILD_LOG( iRankingType )
					<< END_LOG;

				continue;
			}
		}

		std::vector< KHenirRankingInfo >::const_iterator vit;
		for( vit = vecRanking.begin(); vit != vecRanking.end(); ++vit )
		{
			const KHenirRankingInfo& kRankingInfo = *vit;

			// RegDate ��ȯ			
			CTime tRegDate = CTime( kRankingInfo.m_tRegDate );
			const std::wstring wstrRegDate = ( CStringW )( tRegDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );

			// DB Update
			switch( iRankingType )
			{
			case SEnum::RT_DAY_RANKING:
				{
#ifdef SERV_RENEWAL_SP
					DO_QUERY( L"exec dbo.P_Rank_SpaceTime_Day_INS", L"%d, %d, %d, N\'%s\', %d, %d, %d, %d, N\'%s\', N\'%s\'",
						% KBaseServer::GetKObj()->GetServerGroupID()
						% kRankingInfo.m_iRank
						% kRankingInfo.m_iUnitUID
						% kRankingInfo.m_wstrNickName
						% (int)kRankingInfo.m_cUnitClass
						% (int)kRankingInfo.m_ucLevel
						% kRankingInfo.m_iStageCount
						% kRankingInfo.m_ulPlayTime
						% wstrRegDate
						% wstrCurDate
						);
#else //SERV_RENEWAL_SP
					//{{ 2011. 09. 07	������	��ϸ� �ð� ��ŷ ��� ������Ʈ
#ifdef SERV_HENIR_RANKING_BACKUP_UPDATE
					DO_QUERY( L"exec dbo.lup_insert_rank_spacetime_day", L"%d, %d, N\'%s\', %d, %d, %d, %d, N\'%s\', N\'%s\'",
						% kRankingInfo.m_iRank
						% kRankingInfo.m_iUnitUID
						% kRankingInfo.m_wstrNickName
						% (int)kRankingInfo.m_cUnitClass
						% (int)kRankingInfo.m_ucLevel
						% kRankingInfo.m_iStageCount
						% kRankingInfo.m_ulPlayTime
						% wstrRegDate
						% wstrCurDate
						);
#else
					DO_QUERY( L"exec dbo.lup_insert_rank_spacetime_day", L"%d, %d, N\'%s\', %d, %d, %d, %d, N\'%s\'",
						% kRankingInfo.m_iRank
						% kRankingInfo.m_iUnitUID
						% kRankingInfo.m_wstrNickName
						% (int)kRankingInfo.m_cUnitClass
						% (int)kRankingInfo.m_ucLevel
						% kRankingInfo.m_iStageCount
						% kRankingInfo.m_ulPlayTime
						% wstrRegDate
						);
#endif SERV_HENIR_RANKING_BACKUP_UPDATE
					//}}					
#endif //SERV_RENEWAL_SP
				}
				break;

			case SEnum::RT_WEEK_RANKING:
				{
#ifdef SERV_RENEWAL_SP
					DO_QUERY( L"exec dbo.P_Rank_SpaceTime_Week_INS", L"%d, %d, %d, N\'%s\', %d, %d, %d, %d, N\'%s\', N\'%s\'",
						% KBaseServer::GetKObj()->GetServerGroupID()
						% kRankingInfo.m_iRank
						% kRankingInfo.m_iUnitUID
						% kRankingInfo.m_wstrNickName
						% (int)kRankingInfo.m_cUnitClass
						% (int)kRankingInfo.m_ucLevel
						% kRankingInfo.m_iStageCount
						% kRankingInfo.m_ulPlayTime
						% wstrRegDate
						% wstrCurDate
						);
#else //SERV_RENEWAL_SP
					//{{ 2011. 09. 07	������	��ϸ� �ð� ��ŷ ��� ������Ʈ
#ifdef SERV_HENIR_RANKING_BACKUP_UPDATE
					DO_QUERY( L"exec dbo.lup_insert_rank_spacetime_week", L"%d, %d, N\'%s\', %d, %d, %d, %d, N\'%s\', N\'%s\'",
						% kRankingInfo.m_iRank
						% kRankingInfo.m_iUnitUID
						% kRankingInfo.m_wstrNickName
						% (int)kRankingInfo.m_cUnitClass
						% (int)kRankingInfo.m_ucLevel
						% kRankingInfo.m_iStageCount
						% kRankingInfo.m_ulPlayTime
						% wstrRegDate
						% wstrCurDate
						);
#else
					DO_QUERY( L"exec dbo.lup_insert_rank_spacetime_week", L"%d, %d, N\'%s\', %d, %d, %d, %d, N\'%s\'",
						% kRankingInfo.m_iRank
						% kRankingInfo.m_iUnitUID
						% kRankingInfo.m_wstrNickName
						% (int)kRankingInfo.m_cUnitClass
						% (int)kRankingInfo.m_ucLevel
						% kRankingInfo.m_iStageCount
						% kRankingInfo.m_ulPlayTime
						% wstrRegDate
						);
#endif SERV_HENIR_RANKING_BACKUP_UPDATE
					//}}
#endif //SERV_RENEWAL_SP
				}
				break;

			case SEnum::RT_MONTH_RANKING:
				{
#ifdef SERV_RENEWAL_SP
					DO_QUERY( L"exec dbo.P_Rank_SpaceTime_Month_INS", L"%d, %d, %d, N\'%s\', %d, %d, %d, %d, N\'%s\', N\'%s\'",
						% KBaseServer::GetKObj()->GetServerGroupID()
						% kRankingInfo.m_iRank
						% kRankingInfo.m_iUnitUID
						% kRankingInfo.m_wstrNickName
						% (int)kRankingInfo.m_cUnitClass
						% (int)kRankingInfo.m_ucLevel
						% kRankingInfo.m_iStageCount
						% kRankingInfo.m_ulPlayTime
						% wstrRegDate
						% wstrCurDate
						);
#else //SERV_RENEWAL_SP
					//{{ 2011. 09. 07	������	��ϸ� �ð� ��ŷ ��� ������Ʈ
#ifdef SERV_HENIR_RANKING_BACKUP_UPDATE
					DO_QUERY( L"exec dbo.lup_insert_rank_spacetime_month", L"%d, %d, N\'%s\', %d, %d, %d, %d, N\'%s\', N\'%s\'",
						% kRankingInfo.m_iRank
						% kRankingInfo.m_iUnitUID
						% kRankingInfo.m_wstrNickName
						% (int)kRankingInfo.m_cUnitClass
						% (int)kRankingInfo.m_ucLevel
						% kRankingInfo.m_iStageCount
						% kRankingInfo.m_ulPlayTime
						% wstrRegDate
						% wstrCurDate
						);
#else
					DO_QUERY( L"exec dbo.lup_insert_rank_spacetime_month", L"%d, %d, N\'%s\', %d, %d, %d, %d, N\'%s\'",
						% kRankingInfo.m_iRank
						% kRankingInfo.m_iUnitUID
						% kRankingInfo.m_wstrNickName
						% (int)kRankingInfo.m_cUnitClass
						% (int)kRankingInfo.m_ucLevel
						% kRankingInfo.m_iStageCount
						% kRankingInfo.m_ulPlayTime
						% wstrRegDate
						);
#endif SERV_HENIR_RANKING_BACKUP_UPDATE
					//}}					
#endif //SERV_RENEWAL_SP
				}
				break;

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-17	// �ڼ���
			case SEnum::RT_HERO_RANKING:
				{
					DO_QUERY( L"exec dbo.P_Rank_SpaceTime_Hero_Month_INT", L"%d, %d, N\'%s\', %d, %d, %d, %d, N\'%s\'",
						% kRankingInfo.m_iRank
						% kRankingInfo.m_iUnitUID
						% kRankingInfo.m_wstrNickName
						% (int)kRankingInfo.m_cUnitClass
						% (int)kRankingInfo.m_ucLevel
						% kRankingInfo.m_iStageCount
						% kRankingInfo.m_ulPlayTime
						% wstrRegDate
						);
				}
				break;
#endif // SERV_HENIR_RENEWAL_2013
				
			default:
				{
					START_LOG( cerr, L"�̻��� ��ŷŸ�� �Դϴ�" )
						<< BUILD_LOG( iRankingType )
						<< END_LOG;
					continue;
				}
			}

			//if( m_kODBC.BeginFetch() )
			//{
			//	FETCH_DATA( iOK );
			//	m_kODBC.EndFetch();

			//	if( iOK != NetError::NET_OK )
			//	{
			//		START_LOG( cerr, L"��ϸ��� �ð� ��ŷ ���� ������Ʈ ����" )
			//			<< BUILD_LOG( mit->first )
			//			<< BUILD_LOG( kRankingInfo.m_iRank )
			//			<< BUILD_LOG( kRankingInfo.m_iUnitUID )
			//			<< BUILD_LOG( kRankingInfo.m_wstrNickName )
			//			<< BUILD_LOG( (int)kRankingInfo.m_cUnitClass )
			//			<< BUILD_LOG( (int)kRankingInfo.m_ucLevel )
			//			<< BUILD_LOG( kRankingInfo.m_iStageCount )
			//			<< BUILD_LOG( kRankingInfo.m_ulRemainPlayTime )
			//			<< BUILD_LOG( wstrRegDate )
			//			<< END_LOG;
			//	}
			//}
		}

		START_LOG( cout, L"��ϸ��� �ð� ��ŷ ���� DB��� �Ϸ�!" )
			<< BUILD_LOG( iRankingType )
			<< BUILD_LOG( vecRanking.size() );
	}

end_proc:
	return;
}
//}}

//{{ 2010. 06. 01  ������	���� ó�� ����
#ifdef SERV_CCU_NEW

IMPL_ON_FUNC( DBE_UPDATE_CCU_INFO_NOT )
{
	for( u_int ui = 0; ui < kPacket_.m_vecCCUInfo.size(); ++ui )
	{
		const KUpdateCCUInfo& kInfo = kPacket_.m_vecCCUInfo[ui];

		// ������ DB�� ���´�.
		int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_StatCCUMinute_INS", L"%d, N\'%s\', %d, %d", 
			% KBaseServer::GetKObj()->GetServerGroupID() 
			% kPacket_.m_wstrRegDate.c_str()
			% kInfo.m_iServerUID
			% kInfo.m_iConcurrentUser
			);
#else //SERV_ALL_RENEWAL_SP
		// ����UID�� 0�̸� ��ü����
		if( kInfo.m_iServerUID == 0 )
		{
#ifdef SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_StatCCUMinute_INS", L"%d, N\'%s\', %d, %d", 
				% KBaseServer::GetKObj()->GetServerGroupID() 
				% kPacket_.m_wstrRegDate.c_str()
				% kInfo.m_iServerUID
				% kInfo.m_iConcurrentUser
				);
#else //SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.lup_insert_ccu_minute", L"N\'%s\', %d, %d",
				% kPacket_.m_wstrRegDate.c_str()
				% kInfo.m_iServerUID
				% kInfo.m_iConcurrentUser
				);
#endif //SERV_RENEWAL_SP
		}
		else if( kInfo.m_iServerUID == KBaseServer::GetKObj()->GetUID() )
		{
#ifdef SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_StatCCUMinuteGuestUser_INS", L"%d, N\'%s\', %d, %d", 
				% KBaseServer::GetKObj()->GetServerGroupID() 
				% kPacket_.m_wstrRegDate.c_str()
				% kInfo.m_iServerUID
				% kInfo.m_iConcurrentUser
				);
#else //SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.lup_update_concurrent_guestuser", L"N\'%s\', %d, %d",
				% kPacket_.m_wstrRegDate.c_str()
				% kInfo.m_iServerUID
				% kInfo.m_iConcurrentUser
				);
#endif //SERV_RENEWAL_SP
		}
		else
		{
#ifdef SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_LServerList_CCU_UPD", L"%d, N\'%s\', %d, %d, %d, %d", 
				% KBaseServer::GetKObj()->GetServerGroupID() 
				% kPacket_.m_wstrRegDate.c_str()
				% kInfo.m_iServerUID
				% kInfo.m_iConcurrentUser
				% kInfo.m_iCurPartyCount
				% kInfo.m_iPlayGamePartyCount
				);
#else //SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.lup_update_concurrent_user", L"N\'%s\', %d, %d, %d, %d",
				% kPacket_.m_wstrRegDate.c_str()
				% kInfo.m_iServerUID
				% kInfo.m_iConcurrentUser
				% kInfo.m_iCurPartyCount
				% kInfo.m_iPlayGamePartyCount
				);
#endif //SERV_RENEWAL_SP
		}		
#endif //SERV_ALL_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

#ifdef SERV_CHANNELING_USER_MANAGER
		if( !kInfo.m_mapChannelingUserList.empty() )
		{
			int iChannelingOK = NetError::ERR_ODBC_01;

			std::map<int,int>::const_iterator mitCUL;
			for( mitCUL = kInfo.m_mapChannelingUserList.begin(); mitCUL != kInfo.m_mapChannelingUserList.end(); ++mitCUL )
			{
				// ����UID�� 0�̸� ��ü ä�θ� ���� kInfo.m_iServerUID == 0
#ifdef SERV_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_StatCCUMinuteChannel_INS", L"%d, N\'%s\', %d, %d, %d", 
					% KBaseServer::GetKObj()->GetServerGroupID() 
					% kPacket_.m_wstrRegDate.c_str()
					% kInfo.m_iServerUID
					% mitCUL->second	// ä�θ� �ڵ�
					% mitCUL->first		// ����
					);
#else //SERV_RENEWAL_SP
				DO_QUERY( L"exec dbo.lup_insert_StatCCUMinuteChannel", L"N\'%s\', %d, %d, %d",
					% kPacket_.m_wstrRegDate.c_str()
					% kInfo.m_iServerUID
					% mitCUL->second	// ä�θ� �ڵ�
					% mitCUL->first		// ����
					);
#endif //SERV_RENEWAL_SP

				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iChannelingOK );
					m_kODBC.EndFetch();
				}	
			}
		}
#endif SERV_CHANNELING_USER_MANAGER

		//if( iOK != NetError::NET_OK )
		//{
		//	START_LOG( cerr, L"���� ���� ��Ͽ� �����Ͽ����ϴ�!" )
		//		<< BUILD_LOG( kPacket_.m_wstrRegDate )
		//		<< BUILD_LOG( kInfo.m_iServerUID )
		//		<< BUILD_LOG( kInfo.m_iConcurrentUser )
		//		<< BUILD_LOG( kInfo.m_iCurPartyCount )
		//		<< BUILD_LOG( kInfo.m_iPlayGamePartyCount )
		//		<< END_LOG;
		//}

end_proc:
		continue;
	}
}

#endif SERV_CCU_NEW
//}}

//{{ 2010. 10. 13	������	DLL Manager
#ifdef SERV_DLL_MANAGER

IMPL_ON_FUNC_NOPARAM( DBE_GET_MODULE_INFO_LIST_REQ )
{
	KDBE_GET_MODULE_INFO_LIST_ACK kPacket;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LModuleTable_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.lup_get_module_log" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		KModuleInfo kInfo;

		//{{  2012. 1. 30	��μ�		�ѹ��� ���(��ŷ ���μ��� �˻�)
#ifdef SERV_HACKING_DLL_CHECK
		FETCH_DATA( kInfo.m_iModuleUID
			>> kInfo.m_wstrModuleName
			>> kInfo.m_iCount
			);
#else
		FETCH_DATA( kInfo.m_iModuleUID
			>> kInfo.m_wstrModuleName
			>> kInfo.m_iCount
			>> kInfo.m_bAllow
			>> kInfo.m_iFlag			
			);
#endif SERV_HACKING_DLL_CHECK
		//}}

		kPacket.m_vecModuleList.push_back( kInfo );
	}


	//{{  2012. 1. 30	��μ�		�ѹ��� ���(��ŷ ���μ��� �˻�)
#ifdef SERV_HACKING_DLL_CHECK
	{
		std::map< int, int >::const_iterator mit;

		// Release Tick ���
#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_ReleaseTick_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
		DO_QUERY_NO_ARG( L"exec dbo.lup_get_release_tick" );
#endif //SERV_RENEWAL_SP

		while( m_kODBC.Fetch() )
		{
			int iAbuserType = 0;
			int iReleaseTick = 0;

			FETCH_DATA( iAbuserType
				>> iReleaseTick );

			kPacket.m_mapReleaseTick.insert( std::make_pair( iAbuserType, iReleaseTick ) );
		}
	}
#endif SERV_HACKING_DLL_CHECK
	//}}

end_proc:
	SendToServer( DBE_GET_MODULE_INFO_LIST_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_UPDATE_MODULE_INFO_LIST_REQ, KDBE_GET_MODULE_INFO_LIST_ACK )
{
	KDBE_GET_MODULE_INFO_LIST_ACK kPacket;

	BOOST_TEST_FOREACH( const KModuleInfo&, kInfo, kPacket_.m_vecModuleList )
	{
		int iOK = NetError::ERR_ODBC_01;

		// ���UID�� 0�̸� ���� insert�Ǵ� ����.
		if( kInfo.m_iModuleUID == 0 )
		{
			KModuleInfo kNewModuleInfo = kInfo;

			//{{  2012. 1. 30	��μ�		�ѹ��� ���(��ŷ ���μ��� �˻�)
#ifdef SERV_HACKING_DLL_CHECK
#ifdef SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_LModuleTable_INS", L"%d, N\'%s\', %d, %d, %d", 
				% KBaseServer::GetKObj()->GetServerGroupID() 
				% kNewModuleInfo.m_wstrModuleName
				% kNewModuleInfo.m_iCount
				% kNewModuleInfo.m_iAllow
				% kNewModuleInfo.m_iFlag
				);
#else //SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.lup_insert_module_log", L"N\'%s\', %d, %d, %d", 
				% kNewModuleInfo.m_wstrModuleName
				% kNewModuleInfo.m_iCount
				% kNewModuleInfo.m_iAllow
				% kNewModuleInfo.m_iFlag
				);
#endif //SERV_RENEWAL_SP
#else
#ifdef SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_LModuleTable_INS", L"%d, N\'%s\', %d, %d, %d", 
				% KBaseServer::GetKObj()->GetServerGroupID() 
				% kNewModuleInfo.m_wstrModuleName
				% kNewModuleInfo.m_iCount
				% kNewModuleInfo.m_bAllow
				% kNewModuleInfo.m_iFlag
				);
#else //SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.lup_insert_module_log", L"N\'%s\', %d, %d, %d", 
				% kNewModuleInfo.m_wstrModuleName
				% kNewModuleInfo.m_iCount
				% kNewModuleInfo.m_bAllow
				% kNewModuleInfo.m_iFlag
				);
#endif //SERV_RENEWAL_SP
#endif SERV_HACKING_DLL_CHECK
			//}}

			
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK
					>> kNewModuleInfo.m_iModuleUID );

				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				//{{  2012. 1. 30	��μ�		�ѹ��� ���(��ŷ ���μ��� �˻�)
#ifdef SERV_HACKING_DLL_CHECK
				START_LOG( cerr, L"��� ���� insert����!" )
					<< BUILD_LOG( kNewModuleInfo.m_wstrModuleName )
					<< BUILD_LOG( kNewModuleInfo.m_iCount )
					<< BUILD_LOG( kNewModuleInfo.m_iAllow )
					<< BUILD_LOG( kNewModuleInfo.m_iFlag )
					<< END_LOG;
#else
				START_LOG( cerr, L"��� ���� insert����!" )
					<< BUILD_LOG( kNewModuleInfo.m_wstrModuleName )
					<< BUILD_LOG( kNewModuleInfo.m_iCount )
					<< BUILD_LOG( kNewModuleInfo.m_bAllow )
					<< BUILD_LOG( kNewModuleInfo.m_iFlag )
					<< END_LOG;
#endif SERV_HACKING_DLL_CHECK
				//}}
			}
			else
			{
				kPacket.m_vecModuleList.push_back( kNewModuleInfo );
			}
		}
		// ���UID�� 0�� �ƴϸ� update�Ǵ� ����.
		else
		{
#ifdef SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_LModuleTable_UPD", L"%d, %d, %d", 
				% KBaseServer::GetKObj()->GetServerGroupID() 
				% kInfo.m_iModuleUID 
				% kInfo.m_iCount 
				);
#else //SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.lup_update_module_log", L"%d, %d", 
				% kInfo.m_iModuleUID 
				% kInfo.m_iCount 
				);
#endif //SERV_RENEWAL_SP

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"��� ���� update����!" )
					<< BUILD_LOG( kInfo.m_iModuleUID )
					<< BUILD_LOG( kInfo.m_iCount )
					<< END_LOG;
			}
		}
	}

end_proc:
	SendToServer( DBE_UPDATE_MODULE_INFO_LIST_ACK, kPacket );
}

#endif SERV_DLL_MANAGER
//}}

//{{ 2010. 12. 20	������	Ư�� �ð� ť�� ����
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT

IMPL_ON_FUNC_NOPARAM( DBE_GET_TIME_RANDOM_ITEM_EVENT_INFO_REQ )
{
	KDBE_GET_TIME_RANDOM_ITEM_EVENT_INFO_ACK kPacket;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LEvent_Christmas_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.lup_get_Christmas_event" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		KTimeOpenRandomItemEventInfo kInfo;
		std::wstring wstrUpdateTime;
		UidType iUserUID = 0;
		UidType iUnitUID = 0;

		//{{ 2011. 12.13    ��μ�	���� �̺�Ʈ ��� �߰� (�ݺ� ����)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
		FETCH_DATA( kInfo.m_iID
			>> kInfo.m_iItemID
			>> kInfo.m_wstrDropTime
			>> wstrUpdateTime
			>> iUserUID
			>> iUnitUID
			>> kInfo.m_wstrDropEndTime
			>> kInfo.m_iOpenItemID );
#else
		//{{ 2011. 07. 04    ��μ�    ���̹� ���θ��
		// #ifdef SERV_NAVER_PROMOTIONS
		// 		FETCH_DATA( kInfo.m_iID
		// 			>> kInfo.m_iItemID
		// 			>> kInfo.m_wstrDropTime
		// 			>> wstrUpdateTime
		// 			>> iUserUID
		// 			>> iUnitUID
		// 			>> kInfo.m_wstrDropEndTime );
		// #else
		// 		FETCH_DATA( kInfo.m_iID
		// 			>> kInfo.m_iItemID
		// 			>> kInfo.m_wstrDropTime
		// 			>> wstrUpdateTime
		// 			>> iUserUID
		// 			>> iUnitUID
		// 			);
		// #endif SERV_NAVER_PROMOTIONS
		//}} 
#endif SERV_RANDOM_CUBE_GOLD_REWARD
		//}}
			
		if( iUserUID != 0 )
		{
			// �̹� ���� ���� ������� ���� ��������!
			kPacket.m_setGetRewardUserUIDList.insert( iUserUID );
			continue;
		}

		kPacket.m_vecTimeOpenRandomItemInfo.push_back( kInfo );
	}

end_proc:
	SendToServer( DBE_GET_TIME_RANDOM_ITEM_EVENT_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_UPDATE_TIME_RANDOM_ITEM_EVENT_RESULT_NOT )
{
	int iOK = NetError::NET_OK;

	CTime tUpdateTime = CTime( kPacket_.m_kDBUpdateInfo.m_tUpdateTime );
	const std::wstring wstrUpdateTime = ( std::wstring )( tUpdateTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LEvent_Christmas_UPD", L"%d, %d, %d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_kDBUpdateInfo.m_iID
		% kPacket_.m_kDBUpdateInfo.m_iGetUserUID
		% kPacket_.m_kDBUpdateInfo.m_iGetUnitUID
		% wstrUpdateTime
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_update_Christmas_event", L"%d, %d, %d, N\'%s\'",
		% kPacket_.m_kDBUpdateInfo.m_iID
		% kPacket_.m_kDBUpdateInfo.m_iGetUserUID
		% kPacket_.m_kDBUpdateInfo.m_iGetUnitUID
		% wstrUpdateTime
		);
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.m_kDBUpdateInfo.m_iID )
		<< BUILD_LOG( kPacket_.m_kDBUpdateInfo.m_iGetUserUID )
		<< BUILD_LOG( kPacket_.m_kDBUpdateInfo.m_iGetUnitUID )
		<< BUILD_LOG( wstrUpdateTime )
		<< END_LOG;
}

#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
//}}

//{{ 2011. 05. 11	������	�븮����
#ifdef SERV_PSHOP_AGENCY
IMPL_ON_FUNC( ELOG_INSERT_PERSONAL_SHOP_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.lup_insert_store_log", L"%d, %d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iFromUnitUID
		% kPacket_.m_iToUnitUID
		% kPacket_.m_iItemID
		% kPacket_.m_iItemUID
		% kPacket_.m_iQuantity
		% kPacket_.m_iED
		% kPacket_.m_bIsSquare
		% (int)kPacket_.m_cPShopType );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.m_iFromUnitUID )
		<< BUILD_LOG( kPacket_.m_iToUnitUID )
		<< BUILD_LOG( kPacket_.m_iItemID )
		<< BUILD_LOG( kPacket_.m_iItemUID )
		<< BUILD_LOG( kPacket_.m_iQuantity )
		<< BUILD_LOG( kPacket_.m_iED )
		<< BUILD_LOG( kPacket_.m_bIsSquare )		
		<< BUILD_LOGc( kPacket_.m_cPShopType )
		<< END_LOG;
}
#endif SERV_PSHOP_AGENCY
//}}

IMPL_ON_FUNC_NOPARAM( DBE_SERVER_OFF_NOT )
{
    //���� Enable�� ����
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_UPD_Enable", L"%d, %d, 0", % KBaseServer::GetKObj()->GetServerGroupID() % KBaseServer::GetKObj()->GetUID() );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_UPD", L"%d, %d, 0", % KBaseServer::GetKObj()->GetServerGroupID() % KBaseServer::GetKObj()->GetUID() );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
    DO_QUERY( L"exec dbo.lup_set_server_enable_flag", L"%d, 0", % GetKLoginServer()->GetUID() );
#endif //SERV_RENEWAL_SP

    int iOK;
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );
        m_kODBC.EndFetch();

        if( iOK != NetError::NET_OK )
        {
            START_LOG( cerr, L"���� ���� ǥ�� ����." )
                << BUILD_LOG( GetKLoginServer()->GetUID() )
                << END_LOG;

            iOK = NetError::ERR_VERIFY_08;
        }
    }

end_proc:
    return;
}

//{{ 2013. 02. 13	������	Tick�����ս� DB�α� ���
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
IMPL_ON_FUNC( DBE_TICK_PERFORMANCE_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	// SimLayer Tick �����ս� ���
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerSimLayerTickLog_INS", L"%d, %d, %d, %d, N\'%s\', N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iServerUID
		% kPacket_.m_iServerType
		% kPacket_.m_dwMaxTickCount
		% kPacket_.m_wstrMaxTickRegDate
		% kPacket_.m_wstrRegDate
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerSimLayerTickLog_INS", L"%d, %d, %d, N\'%s\', N\'%s\'", 
		% kPacket_.m_iServerUID
		% kPacket_.m_iServerType
		% kPacket_.m_dwMaxTickCount
		% kPacket_.m_wstrMaxTickRegDate
		% kPacket_.m_wstrRegDate
		);
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"SimLayer Tick �����ս� �α� ��� ����!" )
			<< BUILD_LOG( kPacket_.m_iServerUID )
			<< BUILD_LOG( kPacket_.m_iServerType )
			<< BUILD_LOG( kPacket_.m_dwMaxTickCount )
			<< BUILD_LOG( kPacket_.m_wstrMaxTickRegDate )
			<< BUILD_LOG( kPacket_.m_wstrRegDate )
			<< END_LOG;
	}
}
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
//}}

//{{ 2013. 02. 21	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
IMPL_ON_FUNC( DBE_LOG_STATISTICS_INFO_NOT )
{
	CTime tCurTime = CTime::GetCurrentTime();
	const std::wstring wstrCurTime = ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );	
	const UidType iServerUID = KBaseServer::GetKObj()->GetUID();

	BOOST_TEST_FOREACH( const KLogStatInfo&, kInfo, kPacket_.m_vecLogStat )
	{
		int iOK = NetError::ERR_ODBC_01;

		// ���� ���� �α� ���
		DO_QUERY( L"exec dbo.P_LServer_ErrorLog_INS", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, N\'%s\'", 
			% iServerUID
			% kInfo.m_wstrFunctionName
			% kInfo.m_wstrFileName
			% kInfo.m_wstrLineNum
			% kInfo.m_iCount
			% wstrCurTime
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}
end_proc:
		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"Log Statistics ���� ��� ����!" )
				<< BUILD_LOG( iServerUID )
				<< BUILD_LOG( kInfo.m_wstrFunctionName )
				<< BUILD_LOG( kInfo.m_wstrFileName )
				<< BUILD_LOG( kInfo.m_wstrLineNum )
				<< BUILD_LOG( kInfo.m_iCount )
				<< BUILD_LOG( wstrCurTime )
				<< END_LOG;
		}
	}	
}
#endif SERV_LOG_SYSTEM_NEW
//}}
