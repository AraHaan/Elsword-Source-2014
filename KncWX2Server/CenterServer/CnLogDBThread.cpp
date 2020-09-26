#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "CnLogDBThread.h"
#include "CnServer.h"
#include "ActorManager.h"
#include "NetError.h"
#include "PvPRoom.h"
#include "DungeonRoom.h"
#include "CnRoomManager.h"

#define CLASS_TYPE KCnLogDBThread
ImplPfID( KCnLogDBThread, PI_CN_LOG_DB );

IMPL_PROFILER_DUMP( KCnLogDBThread )
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

KCnLogDBThread::~KCnLogDBThread(void)
{
}

void KCnLogDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {
       _CASE( DBE_SERVER_ON_REQ, KNetAddress );
	   //{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	   _CASE( DBE_VERIFY_SERVER_CONNECT_REQ, KECN_VERIFY_SERVER_CONNECT_REQ );
#else
	   _CASE( DBE_VERIFY_SERVER_CONNECT_REQ, KNetAddress );
#endif SERV_INTEGRATION
	   //}}       
       //_CASE( DBE_UPDATE_SERVER_INFO_REQ, KServerInfo );
        CASE_NOPARAM( DBE_SERVER_OFF_NOT );
        CASE( ELOG_CHAT_LOG_NOT );
		CASE( ELOG_INSERT_TRADE_LOG_NOT );
		CASE( ELOG_INSERT_PERSONAL_SHOP_LOG_NOT );
	   _CASE( ELOG_STAT_RES_STONE, KStatisticsData );
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
	   CASE_NOPARAM( DBE_EVENT_UPDATE_REQ );
#endif //SERV_ADD_EVENT_DB
		
		//{{ 2008. 3. 4  ������  ����� �α�
		CASE( ELOG_INSERT_ABUSER_LOG_NOT );
		//}}
		//{{ 2008. 10. 31  ������	����� ����Ʈ
		CASE( DBE_ABUSER_LIST_REQ );
		//}}
		//{{ 2009. 5. 23  ������	�ð����
		CASE_NOPARAM( DBE_GET_TIME_DROP_INFO_REQ );
		CASE( DBE_CHECK_TIME_DROP_RESULT_REQ );
		//}}
		//{{ 2009. 7. 27  ������	��ϸ� Ŭ���� �α�
		CASE( ELOG_HENIR_DUNGEON_CLEAR_NOT );
		//}}
		//{{ 2010. 11. 8	������	���� �α� DB
#ifdef SERV_DUNGEON_LOG_DB
	   _CASE( DBE_DB_LOG_DUNGEON_NOT, KE_LOCAL_LOG_DUNGEON_NOT );
#endif SERV_DUNGEON_LOG_DB
		//}}
	   //{{ 2010. 11. 20	������	Ư�� �ð� ���� ����
#ifdef SERV_TIME_DROP_MONSTER_EVENT
	   CASE_NOPARAM( DBE_GET_DUNGEON_EVENT_INFO_REQ );
	   CASE( DBE_UPDATE_EVENT_MONSTER_KILL_INFO_NOT );
#endif SERV_TIME_DROP_MONSTER_EVENT
	   //}}
	   //{{ 2010. 12. 8	������	���� ���� ��� ���
#ifdef SERV_APPROPRIATE_LEVEL_STAT
	   CASE( ELOG_APPROPRIATE_LEVEL_STATISTICS_NOT );
#endif SERV_APPROPRIATE_LEVEL_STAT
	   //}}
	   //{{ 2011. 01. 26	������	���� Ŭ���� Ÿ�� ������ üũ
#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
	   CASE( DBE_REG_DUNGEON_CLEAR_TIME_HACK_USER_NOT );
#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
	   //}}
	   //{{ 2011. 03. 17	��μ�	���� �÷��� �α� ��û���� 
#ifdef SERV_DUNGEON_TEMP_LOG_DB
	   CASE( DBE_DB_TEMP_LOG_DUNGEON_NOT );
#endif SERV_DUNGEON_TEMP_LOG_DB
	   //}}
	   //{{ 2011. 06. 13	������	�������� �α�
#ifdef SERV_DUNGEON_STAGE_LOG
	   CASE( ELOG_DUNGEON_STAGE_LOG_NOT );
#endif SERV_DUNGEON_STAGE_LOG
	   //}}
	   //{{ 2011. 07. 27	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	   CASE( DBE_PVP_MATCH_RESULT_LOG_NOT );
#endif SERV_PVP_NEW_SYSTEM
	   //}}
	   //{{ 2012. 10. 16	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	   CASE( ELOG_COLLECTION_OF_RELAY_AND_P2P_INFO_NOT );
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	   //}}
	   //{{ 2012. 09. 05	�ڼ���	������ �α� �߰�
#ifdef UDP_CAN_NOT_SEND_USER_KICK_LOG
	   CASE( DBE_UDP_CHECK_KICK_USER_NOT );
#endif UDP_CAN_NOT_SEND_USER_KICK_LOG
	   //}}
	   //{{ 2012. 10. 18	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	   CASE( ELOG_BATTLE_FIELD_LEAVE_LOG_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	   //}}
	   //{{ 2012. 12. 12	������	���â ��Ż ���� �α�
#ifdef SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
	   CASE( DBE_DUNGEON_RESULT_LEAVE_LOG_NOT );
#endif SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
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
	  
#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
	   CASE_NOPARAM( DBE_CHECK_EVENT_UPDATE_REQ );
#endif //SERV_ADD_EVENT_DB
#endif SERV_REFRESH_EVENT_USING_RELEASE_TICK  
	   
	   //{{ 2013. 2. 26	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	   CASE( DBE_UDP_CHECK_KICK_USER_NOT );
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	   //}}
	   //{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	   CASE( DBE_WRITE_RELATIONSHIP_INFO_LOG_REQ );
#endif SERV_RELATIONSHIP_SYSTEM
	   //}

#ifdef SERV_DUNGEON_STAGE_LOAD_LOG// �۾���¥: 2013-05-15	// �ڼ���
	   CASE( DBE_DUNGEON_STAGE_LOAD_LOG_NOT );
#endif // SERV_DUNGEON_STAGE_LOAD_LOG

#ifdef SERV_HENIR_CLEAR_TIME_HACK_USER_CHECK
       CASE( ELOG_HENIR_HACKING_LOG_NOT );
#endif // SERV_HENIR_CLEAR_TIME_HACK_USER_CHECK

       _CASE( ELOG_GEN_FIELD_MIDDLE_BOSS_STAT_NOT, KFieldBossGenKillStat );
       _CASE( ELOG_KILL_FIELD_MIDDLE_BOSS_STAT_NOT, KFieldBossGenKillStat );

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-12-03	// �ڼ���
	   CASE( DBE_BOSS_FIELD_LOG_NOT );
#endif // SERV_BATTLE_FIELD_BOSS

    default:
        START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

void KCnLogDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KCnLogDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
    SendToUser( nTo, usEventID, char() );
}

void KCnLogDBThread::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID )
{
    SendToGSUser( nGS, nTo, usEventID, char() );
}

IMPL_ON_FUNC( ELOG_CHAT_LOG_NOT )
{
    // injection ���� ���ɼ� ����.
    KODBC::RemoveInvalidMark( kPacket_.m_strMsg );

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_ChatLog_INS", L"%d, %d, %d, %d, %d, N\'%s\'",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_nRoomUID
		% kPacket_.m_nCharUID
		% kPacket_.m_nUserUID
		% (int)kPacket_.m_cChatType
		% kPacket_.m_strMsg );
#else //SERV_RENEWAL_SP
    DO_QUERY( L"exec dbo.lup_insert_chat_log", L"%d, %d, %d, %d, N\'%s\'",
        % kPacket_.m_nRoomUID
        % kPacket_.m_nCharUID
        % kPacket_.m_nUserUID
        % (int)kPacket_.m_cChatType
        % kPacket_.m_strMsg );
#endif //SERV_RENEWAL_SP

end_proc:
    __noop; // doing nothing.
}

_IMPL_ON_FUNC( DBE_SERVER_ON_REQ, KNetAddress )
{
    KDBE_CENTER_SERVER_ON_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	//{{ 2013. 01. 17	������	������ ��� �缳IP ����
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_SEL_ByPublicIP", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% KBaseServer::ESC_CENTER 
		% kPacket_.m_wstrIP 
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_By_PublicIP_SEL", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% KBaseServer::ESC_CENTER 
		% kPacket_.m_wstrIP 
		);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_GET_verify_PublicIP", L"%d, N\'%s\'", 
		% KBaseServer::ESC_CENTER 
		% kPacket_.m_wstrIP 
		);
#endif //SERV_RENEWAL_SP
#else //SERV_PRIVATE_IP_SERVER_NETWORKING
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_SEL", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID()
		% KBaseServer::ESC_CENTER 
		% kPacket_.m_wstrIP 
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_Verify_On_SEL", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID()
		% KBaseServer::ESC_CENTER 
		% kPacket_.m_wstrIP 
		);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_verify_server_on", L"%d, N\'%s\'", 
		% KBaseServer::ESC_CENTER 
		% kPacket_.m_wstrIP 
		);
#endif //SERV_RENEWAL_SP
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}	

    if( !m_kODBC.BeginFetch() )
    {
        START_LOG( cerr, L"���� ���� ���� ����." )
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
		>> kPacket.m_kServerInfo.m_bOn
		>> kPacket.m_kServerInfo.m_cServerRollType );
#else
	//{{ 2011. 02. 23	������	������ ���� Ÿ��
	//#ifdef SERV_SERVER_ROLL_TYPE
	FETCH_DATA( kPacket.m_kServerInfo.m_iUID 
		>> kPacket.m_kServerInfo.m_wstrName
		>> kPacket.m_kServerInfo.m_iServerGroupID
		>> kPacket.m_kServerInfo.m_iServerClass
		>> kPacket.m_kServerInfo.m_wstrIP
		>> kPacket.m_kServerInfo.m_usMasterPort
		>> kPacket.m_kServerInfo.m_usNCUDPPort
		>> kPacket.m_kServerInfo.m_nMaxUser
		>> kPacket.m_kServerInfo.m_nCurrentUser
		>> kPacket.m_kServerInfo.m_bOn
		>> kPacket.m_kServerInfo.m_cServerRollType );
	//#else
	//	FETCH_DATA( kPacket.m_kServerInfo.m_iUID 
	//		>> kPacket.m_kServerInfo.m_wstrName
	//		>> kPacket.m_kServerInfo.m_iServerGroupID
	//		>> kPacket.m_kServerInfo.m_iServerClass
	//		>> kPacket.m_kServerInfo.m_wstrIP
	//		>> kPacket.m_kServerInfo.m_usMasterPort
	//		>> kPacket.m_kServerInfo.m_usNCUDPPort
	//		>> kPacket.m_kServerInfo.m_nMaxUser
	//		>> kPacket.m_kServerInfo.m_nCurrentUser
	//		>> kPacket.m_kServerInfo.m_bOn );
	//#endif SERV_SERVER_ROLL_TYPE
	//}}    
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}	

    m_kODBC.EndFetch();

	//{{ 2011. 07. 22	������	��� ���� SP���
#ifdef SERV_ALL_DB_QUERY_USE_SP
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LCenterProxy_SEL_ByServerUID", L"%d, %d",	% KBaseServer::GetKObj()->GetServerGroupID() 
																		% kPacket.m_kServerInfo.m_iUID	);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LCenterProxy_By_ServerUID_SEL", L"%d, %d",	% KBaseServer::GetKObj()->GetServerGroupID() 
																		% kPacket.m_kServerInfo.m_iUID	);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_get_center_proxy_id_by_serveruid", L"%d", % kPacket.m_kServerInfo.m_iUID );
#endif //SERV_RENEWAL_SP
#else
	// ProxyID�� �޾ƿ´�.
	//DO_QUERY_NO_PROFILE( L"SELECT ProxyID FROM dbo.LCenterProxy(NOLOCK)", L"WHERE ServerUID = %d", % kPacket.m_kServerInfo.m_iUID );
#endif SERV_ALL_DB_QUERY_USE_SP
	//}}	
	if( !m_kODBC.BeginFetch() )
	{
		START_LOG( cerr, L"�� �������� ���ʹ�." )
			<< BUILD_LOG( kPacket.m_kServerInfo.m_iUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_VERIFY_13;
		goto end_proc;
	}

	FETCH_DATA( kPacket.m_iProxyID );
	m_kODBC.EndFetch();

	//{{ 2010. 11. 1	������	���� ����Ʈ ����
#ifdef SERV_SQUARE_LIST_SERV_UID
	//���� ���� �����͸� �޾ƿ´�.
	DO_QUERY_NO_PROFILE( L"SELECT Type, Name, SortIndex FROM dbo.LSquareTable(NOLOCK)", L"WHERE ServerUID = %d ", % KBaseServer::GetKObj()->GetUID() );
	while( m_kODBC.Fetch() )
	{
		KSquareCreateInfo kSquareCreateInfo;

		FETCH_DATA( kSquareCreateInfo.m_iSquareType
			>> kSquareCreateInfo.m_wstrSquareName
			>> kSquareCreateInfo.m_cSortIndex );

		kPacket.m_vecSquareList.push_back( kSquareCreateInfo );
	}
#else
	//{{ 2011. 07. 22	������	��� ���� SP���
#ifdef SERV_ALL_DB_QUERY_USE_SP
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LSquareTable_SEL", L"%d, N\'%s\'", % KBaseServer::GetKObj()->GetServerGroupID() 
															  % kPacket_.m_wstrIP );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_get_square_list", L"N\'%s\'", % kPacket_.m_wstrIP );
#endif //SERV_RENEWAL_SP
#else
	//���� ���� �����͸� �޾ƿ´�.
	//DO_QUERY_NO_PROFILE( L"SELECT Type, Name FROM dbo.LSquareTable(NOLOCK)", L"WHERE IP = N\'%s\' ", % kPacket_.m_wstrIP );
#endif SERV_ALL_DB_QUERY_USE_SP
	//}}	
	while( m_kODBC.Fetch() )
	{
		KSquareCreateInfo kSquareCreateInfo;

		FETCH_DATA( kSquareCreateInfo.m_iSquareType
			>> kSquareCreateInfo.m_wstrSquareName );

		kPacket.m_vecSquareList.push_back( kSquareCreateInfo );
	}
#endif SERV_SQUARE_LIST_SERV_UID
	//}}	

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
	kPacket.m_iLocalServerGroupID = kPacket_.m_iServerGroupID;

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
	DO_QUERY( L"exec dbo.P_LServerList_GET_verify_connect", L"%d, N\'%s\', %d", % KBaseServer::ESC_GAME % kPacket_.m_kNetAddress.m_wstrIP % kPacket_.m_kNetAddress.m_usPort );
#else
	DO_QUERY( L"exec dbo.lup_verify_server_connect", L"%d, N\'%s\', %d", % KBaseServer::ESC_GAME % kPacket_.m_kNetAddress.m_wstrIP % kPacket_.m_kNetAddress.m_usPort );
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		//{{ 2010. 04. 22  ������	������ ��ü ����
		FETCH_DATA( kPacket.m_nUID 
			>> kPacket.m_iDBRegServerGroupID
			>> kPacket.m_strName
			>> kPacket.m_sMaxNum );
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

//_IMPL_ON_FUNC( DBE_UPDATE_SERVER_INFO_REQ, KServerInfo )
//{
//    KServerList kPacket;
//    WriteServerInfoToDB( kPacket_, kPacket );
//
//    SendToServer( DBE_UPDATE_SERVER_INFO_ACK, kPacket );
//}

//void KCnLogDBThread::WriteServerInfoToDB( IN KServerInfo kPacket_, OUT KServerList& kPacket )
//{
//    // ������ DB�� ���´�.
//    int iOK;
//    DO_QUERY( ( L"exec dbo.lup_update_concurrent_user %d, %d" ) % kPacket_.m_iUID % kPacket_.m_nCurrentUser );	// �ּ� ���� �� P_LServerList_CCU_UPD ����ؾ���
//    if( m_kODBC.BeginFetch() )
//    {
//        FETCH_DATA( iOK );
//        m_kODBC.EndFetch();
//    }
//
//    // DB�κ��� ���� ����Ʈ �����͸� �޾ƿ´�
//    DO_QUERY( ( L"exec dbo.lup_get_server_list" ) ); // �ּ������� P_LServerList_SEL �̰� ����ؾ���
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
    DO_QUERY( L"exec dbo.lup_set_server_enable_flag", L"%d, 0", % GetKCnServer()->GetUID() );
#endif //SERV_RENEWAL_SP

    int iOK;
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );
        m_kODBC.EndFetch();

        if( iOK != NetError::NET_OK )
        {
            START_LOG( cerr, L"���� ���� ǥ�� ����." )
                << BUILD_LOG( GetKCnServer()->GetUID() )
                << END_LOG;

            iOK = NetError::ERR_VERIFY_08;
        }
    }

end_proc:
    return;
}

IMPL_ON_FUNC( ELOG_INSERT_TRADE_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LTrade_INS", L"%d, %d, %d, %d, %d, %d, %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iFromUserUID
		% kPacket_.m_iFromUnitUID
		% kPacket_.m_iToUserUID
		% kPacket_.m_iToUnitUID
		% kPacket_.m_iItemUID
		% kPacket_.m_iItemID
		% kPacket_.m_iQuantity );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_trade_log", L"%d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iFromUserUID
		% kPacket_.m_iFromUnitUID
		% kPacket_.m_iToUserUID
		% kPacket_.m_iToUnitUID
		% kPacket_.m_iItemUID
		% kPacket_.m_iItemID
		% kPacket_.m_iQuantity );
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.m_iFromUserUID )
		<< BUILD_LOG( kPacket_.m_iFromUnitUID )
		<< BUILD_LOG( kPacket_.m_iToUserUID )
		<< BUILD_LOG( kPacket_.m_iToUnitUID )
		<< BUILD_LOG( kPacket_.m_iItemUID )
		<< BUILD_LOG( kPacket_.m_iItemID )
		<< BUILD_LOG( kPacket_.m_iQuantity )
		<< END_LOG;
	return;
}

//{{ 2008. 2. 26  ������  ���λ��� �α�
IMPL_ON_FUNC( ELOG_INSERT_PERSONAL_SHOP_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_STrade_INS", L"%d, %d, %d, %d, %d, %d, %d, %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iFromUnitUID
		% kPacket_.m_iToUnitUID
		% kPacket_.m_iItemID
		% kPacket_.m_iItemUID
		% kPacket_.m_iQuantity
		% kPacket_.m_iED
		% kPacket_.m_bIsSquare
		% (int)kPacket_.m_cPShopType );

#else //SERV_RENEWAL_SP
	//{{ 2011. 05. 11	������	�븮����
#ifdef SERV_PSHOP_AGENCY
	DO_QUERY( L"exec dbo.lup_insert_store_log", L"%d, %d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iFromUnitUID
		% kPacket_.m_iToUnitUID
		% kPacket_.m_iItemID
		% kPacket_.m_iItemUID
		% kPacket_.m_iQuantity
		% kPacket_.m_iED
		% kPacket_.m_bIsSquare
		% (int)kPacket_.m_cPShopType );
#else
	DO_QUERY( L"exec dbo.lup_insert_store_log", L"%d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iFromUnitUID
		% kPacket_.m_iToUnitUID
		% kPacket_.m_iItemID
		% kPacket_.m_iItemUID
		% kPacket_.m_iQuantity
		% kPacket_.m_iED
		% kPacket_.m_bIsSquare );
#endif SERV_PSHOP_AGENCY
	//}}
#endif //SERV_RENEWAL_SP

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
		//{{ 2011. 05. 11	������	�븮����
#ifdef SERV_PSHOP_AGENCY
		<< BUILD_LOGc( kPacket_.m_cPShopType )
#endif SERV_PSHOP_AGENCY
		//}}
		<< END_LOG;
	return;
	//}}
}
//}}

//{{ 2007. 12. 25  ������  ��Ȱ�� ���
_IMPL_ON_FUNC( ELOG_STAT_RES_STONE, KStatisticsData )
{
    int iOK = NetError::ERR_ODBC_01;

    KStatisticsData::iterator mit;
    for( mit = kPacket_.begin(); mit != kPacket_.end(); ++mit )
    {
        if( mit->first.m_vecIntKey.empty() )
        {
            START_LOG( cerr, L"���Ű���� �����." )
                << END_LOG;

            continue;
        }        
		__int64 aiCount[KStatistics::eSIColResStone_Max];
        std::vector< __int64 >::iterator vit = mit->second.begin();
        for( int iIndex = 0; iIndex < KStatistics::eSIColResStone_Max; iIndex++ )
        {
            if( vit == mit->second.end() )
            {
                START_LOG( cwarn, L"ī��Ʈ �����Ͱ� �����." )                    
                    << BUILD_LOG( iIndex )
                    << END_LOG;

                aiCount[iIndex] = 0;
            }
            else
            {
                aiCount[iIndex] = *vit;
                vit++;
            }
        }

#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_StatsStoneCnt_MER", L"%d, %d, %d",
			% KBaseServer::GetKObj()->GetServerGroupID()
			% aiCount[KStatistics::eSIColResStone_PlayerCount]
			% aiCount[KStatistics::eSIColResStone_UseCount]
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_StatsStoneCnt_SET", L"%d, %d, %d",
			% KBaseServer::GetKObj()->GetServerGroupID()
			% aiCount[KStatistics::eSIColResStone_PlayerCount]
			% aiCount[KStatistics::eSIColResStone_UseCount]
			);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.StatsStoneCntUpdate", L"%d, %d",
			% aiCount[KStatistics::eSIColResStone_PlayerCount]
			% aiCount[KStatistics::eSIColResStone_UseCount]
			);
#endif //SERV_RENEWAL_SP

        if( m_kODBC.BeginFetch() )
        {
            FETCH_DATA( iOK );

            m_kODBC.EndFetch();
        }
    }

end_proc:
    LOG_SUCCESS( iOK == NetError::NET_OK )
        << BUILD_LOG( iOK )
        << BUILD_LOG( kPacket_.size() )
        << END_LOG;	
}
//}}

#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
IMPL_ON_FUNC_NOPARAM( DBE_EVENT_UPDATE_REQ )
{
	KDBE_EVENT_UPDATE_ACK kPacket;

	// DB�κ��� �̺�Ʈ ����Ʈ �����͸� �޾ƿ´�
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_EventInfo_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.lup_get_event" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		KEventInfo kEventInfo;

		FETCH_DATA( kEventInfo.m_iEventUID
			>> kEventInfo.m_wstrEventName
			>> kEventInfo.m_iEventType
			>> kEventInfo.m_wstrBeginDate
			>> kEventInfo.m_wstrEndDate
			>> kEventInfo.m_iScriptID
//#ifdef SERV_ALLOW_EVENT_ERASE
			>> kEventInfo.m_bOn
//#else SERV_ALLOW_EVENT_ERASE
//			>> kEventInfo.m_bEnable
//#endif SERV_ALLOW_EVENT_ERASE
			);

#ifdef SERV_ALLOW_EVENT_ERASE
		kEventInfo.m_bEnable = true;
#endif SERV_ALLOW_EVENT_ERASE

		kPacket.m_vecEventList.push_back( kEventInfo );
	}

end_proc:
	//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_SERVER, NULL, DBE_EVENT_UPDATE_ACK, kPacket );
	GetKCnRoomManager()->QueueingEventToRoomManager( spEvent );
#else
	SendToServer( DBE_EVENT_UPDATE_ACK, kPacket );
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
}
#endif //SERV_ADD_EVENT_DB

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
IMPL_ON_FUNC_NOPARAM( DBE_RELEASE_TICK_UPDATE_REQ )
{
	int								releaseTick = -1;
	KDBE_RELEASE_TICK_UPDATE_ACK	AckPacket;
	int iReleaseTickType = 0;
	int iReleaseTick = 0;

	// ReleaseTick ���
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_ReleaseTick_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.lup_get_release_tick" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		FETCH_DATA( iReleaseTickType
			>> iReleaseTick );
		AckPacket.m_mapReleaseTick.insert( std::make_pair( iReleaseTickType, releaseTick ) );
	}

end_proc:
	SendToServer( DBE_RELEASE_TICK_UPDATE_ACK, AckPacket );
}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}

//{{ 2008. 3. 4  ������  ����� �α�
IMPL_ON_FUNC( ELOG_INSERT_ABUSER_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsAbusingLog_INS", L"%d, %d, %d, N\'%s\', %d, %d, %d, %d, N\'%s\', %d, %d, "
		L"%d, %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iUserUID				// @iUserUID		bigint
		% kPacket_.m_iUnitUID				// @iUnitUID		bigint
		% kPacket_.m_wstrIP					// @strIP			nvarchar(15)
		% kPacket_.m_iGameType				// @iVsType			tinyint
		% (int)kPacket_.m_fPlayTimeLimit	// @iVsSetTime		smallint
		% kPacket_.m_bPublic				// @bIsSecret		bit
		% kPacket_.m_iPlayWithMember		// @iPlayerCnt		tinyint
		% kPacket_.m_wstrEndGameTime		// @sdtVsEndTime	smalldatetime
		% kPacket_.m_iPlayTime				// @iPlayTime		int
		% kPacket_.m_iRewardVP				// @iGetVP			int
		% kPacket_.m_iRewardEXP				// @iGetExp			int
		% kPacket_.m_iKillCount				// @iKillCnt		tinyint
		% kPacket_.m_iDeathCount			// @iDeathCnt		tinyint
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.StatsAbusingLogInsert", L"%d, %d, N\'%s\', %d, %d, %d, %d, N\'%s\', %d, %d, "
		L"%d, %d, %d",
		% kPacket_.m_iUserUID				// @iUserUID		bigint
		% kPacket_.m_iUnitUID				// @iUnitUID		bigint
		% kPacket_.m_wstrIP					// @strIP			nvarchar(15)
		% kPacket_.m_iGameType				// @iVsType			tinyint
		% (int)kPacket_.m_fPlayTimeLimit	// @iVsSetTime		smallint
		% kPacket_.m_bPublic				// @bIsSecret		bit
		% kPacket_.m_iPlayWithMember		// @iPlayerCnt		tinyint
		% kPacket_.m_wstrEndGameTime		// @sdtVsEndTime	smalldatetime
		% kPacket_.m_iPlayTime				// @iPlayTime		int
		% kPacket_.m_iRewardVP				// @iGetVP			int
		% kPacket_.m_iRewardEXP				// @iGetExp			int
		% kPacket_.m_iKillCount				// @iKillCnt		tinyint
		% kPacket_.m_iDeathCount			// @iDeathCnt		tinyint
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
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_wstrIP )
		<< BUILD_LOG( kPacket_.m_iGameType )
		<< BUILD_LOG( kPacket_.m_fPlayTimeLimit )
		<< BUILD_LOG( kPacket_.m_bPublic )
		<< BUILD_LOG( kPacket_.m_iPlayWithMember )
		<< BUILD_LOG( kPacket_.m_wstrEndGameTime )
		<< BUILD_LOG( kPacket_.m_iPlayTime )
		<< BUILD_LOG( kPacket_.m_iRewardVP )
		<< BUILD_LOG( kPacket_.m_iRewardEXP )
		<< BUILD_LOG( kPacket_.m_iKillCount )
		<< BUILD_LOG( kPacket_.m_iDeathCount )
		<< END_LOG;
}
//}}

//{{ 2008. 10. 31  ������	����� ����Ʈ
//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
IMPL_ON_FUNC( DBE_ABUSER_LIST_REQ )
{
	KDBE_ABUSER_LIST_ACK ackPacket;

	// Abuser List ���
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_AbuserList_SEL", L"%d, %d",	% KBaseServer::GetKObj()->GetServerGroupID()
														% kPacket_.m_iRtt );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_get_abuser_list", L"%d", % kPacket_.m_iRtt );
#endif //SERV_RENEWAL_SP

	ackPacket.m_iRtt = kPacket_.m_iRtt;

	while( m_kODBC.Fetch() )
	{
		UidType iUnitUID = 0;

		FETCH_DATA( iUnitUID );

		if( iUnitUID <= 0 )
			continue;

		ackPacket.m_vecAbuserList.push_back( iUnitUID );
	}

end_proc:
	SendToServer( DBE_ABUSER_LIST_ACK, ackPacket );
}
#else
IMPL_ON_FUNC( DBE_ABUSER_LIST_REQ )
{
	KDBE_ABUSER_LIST_ACK kPacket;
	std::map< int, int >::const_iterator mit;	
	std::map< int, KAbuserList >::iterator mitAL;

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

		mit = kPacket_.m_mapReleaseTick.find( iAbuserType );
		if( mit != kPacket_.m_mapReleaseTick.end() )
		{
			// DB�� ����� ReleaseTick�� �� ũ�ٸ�..
			if( iReleaseTick > mit->second )
			{
				KAbuserList kAbuserInfo;
				kAbuserInfo.m_iReleaseTick = iReleaseTick;
				kPacket.m_mapAbuserList.insert( std::make_pair( iAbuserType, kAbuserInfo ) );
			}
		}
		else
		{
			START_LOG( clog, L"Abuser List�ʹ� ������� ReleaseTick�Դϴ�." )
				<< BUILD_LOG( iAbuserType );
		}
	}

	// Abuser List ���
	for( mitAL = kPacket.m_mapAbuserList.begin(); mitAL != kPacket.m_mapAbuserList.end(); ++mitAL )
	{
#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_AbuserList_SEL", L"%d, %d",	% KBaseServer::GetKObj()->GetServerGroupID()
															% mitAL->first );
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.lup_get_abuser_list", L"%d", % mitAL->first );
#endif //SERV_RENEWAL_SP

		while( m_kODBC.Fetch() )
		{
			UidType iUnitUID = 0;

			FETCH_DATA( iUnitUID );

			if( iUnitUID <= 0 )
				continue;

			mitAL->second.m_vecAbuserList.push_back( iUnitUID );
		}
	}

end_proc:
	//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
#ifdef SERV_EVENT_SCRIPT_REFRESH
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_SERVER, NULL, DBE_ABUSER_LIST_ACK, kPacket );
	GetKCnRoomManager()->QueueingEventToRoomManager( spEvent );
#else
    SendToServer( DBE_ABUSER_LIST_ACK, kPacket );
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}	
}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
//}}

//{{ 2009. 5. 23  ������	�ð����
IMPL_ON_FUNC_NOPARAM( DBE_GET_TIME_DROP_INFO_REQ )
{
	KDBE_GET_TIME_DROP_INFO_ACK kPacket;

	// Release Tick ���
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_DropEvent_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.lup_get_drop_event" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		KTimeDropInfo kInfo;
		bool bIsDrop = false;

		FETCH_DATA( kInfo.m_iTimeDropID
			>> kInfo.m_iDropItemID
			>> kInfo.m_wstrDropTime
			>> kInfo.m_wstrServerIP
			>> bIsDrop
			>> kInfo.m_bNoticeMsg );

		// �̹� ��ӵ� �������� ���� �ʴ´�
		if( bIsDrop == true )
			continue;

		kPacket.m_vecTimeDropInfo.push_back( kInfo );
	}

end_proc:
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_SERVER, NULL, DBE_GET_TIME_DROP_INFO_ACK, kPacket );
	GetKCnRoomManager()->QueueingEventToRoomManager( spEvent );
}

IMPL_ON_FUNC( DBE_CHECK_TIME_DROP_RESULT_REQ )
{
	KDBE_CHECK_TIME_DROP_RESULT_ACK kPacket;
	kPacket.m_iOK			= NetError::ERR_ODBC_01;
	kPacket.m_iGSUID		= kPacket_.m_iGSUID;
	kPacket.m_iUserUID		= kPacket_.m_iUserUID;
	kPacket.m_iUnitUID		= kPacket_.m_iUnitUID;
	kPacket.m_wstrNickName	= kPacket_.m_wstrNickName;
	kPacket.m_iTimeDropID	= kPacket_.m_iTimeDropID;
	kPacket.m_iRewardItemID = kPacket_.m_iRewardItemID;
	kPacket.m_wstrRewardItemName = kPacket_.m_wstrRewardItemName;
	kPacket.m_iDropItemUID	= kPacket_.m_iDropItemUID;
	
	// ���� �޾Ƶ� �Ǵ� �������� �˻�
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_DropEvent_UPD", L"%d, %d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket_.m_iTimeDropID % kPacket_.m_iUserUID 
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_update_drop_event", L"%d, %d", % kPacket_.m_iTimeDropID % kPacket_.m_iUserUID );
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_TIME_DROP_EVENT_00;

		START_LOG( cerr, L"�ð� ��� ���� üũ ���� ����!" )
			<< BUILD_LOG( kPacket_.m_iTimeDropID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< END_LOG;

		goto end_proc;
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		// �� ������ ������ ������ �ִ� ������ �ƴϴ�!
		kPacket.m_iOK = NetError::ERR_TIME_DROP_EVENT_01;
	}

end_proc:
	if( kPacket.m_iOK == NetError::NET_OK )
	{
		// �����ߴٸ� ���� ó���Ϸ� ����!
		SendToServer( DBE_CHECK_TIME_DROP_RESULT_ACK, kPacket );
	}
	else
	{
		// ���� �ߴٸ� �ѹ��Ϸ� ����!
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CN_SERVER, NULL, DBE_CHECK_TIME_DROP_RESULT_ACK, kPacket );
		GetKCnRoomManager()->QueueingEventToRoomManager( spEvent );
	}	
}
//}}

//{{ 2009. 7. 27  ������	��ϸ� Ŭ���� �α�
IMPL_ON_FUNC( ELOG_HENIR_DUNGEON_CLEAR_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsTimeSpaceTop100_INS", L"%d, %d, %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% kPacket_.m_iPlayTime );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_timespacetop100", L"%d, %d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% kPacket_.m_iPlayTime );
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_iPlayTime )
		<< END_LOG;
}
//}}

//{{ 2010. 11. 8	������	���� �α� DB
#ifdef SERV_DUNGEON_LOG_DB
_IMPL_ON_FUNC( DBE_DB_LOG_DUNGEON_NOT, KE_LOCAL_LOG_DUNGEON_NOT )
{
	const std::wstring wstrIP = NetCommon::GetLocalIPW();

	int iOK = NetError::ERR_ODBC_01;
		
	int iAPoint = 0; // PVE���� �߰������� SP�� �������� �ʱ� ���� ������ ���⵵�� ��.

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LDungeonLog_INS",
		L"%d, "
		L"N\'%s\', %d, %d, N\'%s\', %d, %d, %d, %d, %d, %d, "
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
		L"%d, %d, %d, %d, %d, %d, N\'%s\', %d, %d, %d, "
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
		L"%d, %d, N\'%s\',%d, %d, %d, %d, %d, %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% wstrIP
		% kPacket_.m_iDungeonID
		% kPacket_.m_nUnitUID
		% kPacket_.m_wstrNickName
		% kPacket_.m_iDifficultyLevel
		% kPacket_.m_iIsChallenge
		% kPacket_.m_iUnitClass
		% kPacket_.m_iClear
		% kPacket_.m_iStartNumMember
		% kPacket_.m_iEndNumMember							// 10��
		% static_cast<int>(kPacket_.m_ucLevel)
		% kPacket_.m_iPlayTime
		% kPacket_.m_iEXP
		% kPacket_.m_iED
		% static_cast<int>(kPacket_.m_cComboRank)
		% kPacket_.m_iComboScore
		% static_cast<int>(kPacket_.m_cTechnicalRank)
		% kPacket_.m_iTechnicalScore
		% static_cast<int>(kPacket_.m_cTimeRank)
		% static_cast<int>(kPacket_.m_cDamagedRank)			// 20��
		% kPacket_.m_iDamageNum
		% static_cast<int>(kPacket_.m_cTotalRank)
		% kPacket_.m_iRessurectionStoneCount
		% kPacket_.m_iPassedStageCount
		% kPacket_.m_iPassedSubStageCount
		% kPacket_.m_bIsWithPet
		% kPacket_.m_wstrRegDate
		% kPacket_.m_sNpcDieCount
		% kPacket_.m_iGivenDamage
		% kPacket_.m_iAttackDamage							// 30��
		% kPacket_.m_sUsingSkillKind
		% kPacket_.m_iTotalEXP
		% kPacket_.m_iPetID
		% static_cast<int>(kPacket_.m_cPetEvoStep)
		% kPacket_.m_iDashCount
		% kPacket_.m_iEndHP
		% kPacket_.m_iStartContinue
		% kPacket_.m_iContinueUseStage
		% kPacket_.m_iContinueUseSub
		% kPacket_.m_iFrame									// 40��
		% kPacket_.m_iBaseHP
		% kPacket_.m_iRecovHP
		% kPacket_.m_wstrChannelIP
		% kPacket_.m_iUserUID
		% static_cast<int>(kPacket_.m_cAutoPartyPlay)
		% kPacket_.m_iAutoPartyWaitTime
		% kPacket_.m_iLeaveReason
		% static_cast<int>(kPacket_.m_cRoomState)
		% kPacket_.m_bSkillSlotType
		% iAPoint
		);
#else //SERV_RENEWAL_SP
	//{{ 2012. 12. 12  ��ų ���� ��� Ÿ�� �α� - ��μ�
#ifdef SERV_USE_SKILL_SLOT_TYPE_LOG
	//{{ 2012. 12. 12	������	��Ʋ�ʵ� �ý���
	//#ifdef SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
	DO_QUERY( L"exec dbo.lup_insert_Dungeon_Log", 
		L"N\'%s\', %d, %d, N\'%s\', %d, %d, %d, %d, %d, %d, "
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
		L"%d, %d, %d, %d, %d, %d, N\'%s\', %d, %d, %d, "
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
		L"%d, %d, N\'%s\',%d, %d, %d, %d, %d, %d, %d",
		% wstrIP
		% kPacket_.m_iDungeonID
		% kPacket_.m_nUnitUID
		% kPacket_.m_wstrNickName
		% kPacket_.m_iDifficultyLevel
		% kPacket_.m_iIsChallenge
		% kPacket_.m_iUnitClass
		% kPacket_.m_iClear
		% kPacket_.m_iStartNumMember
		% kPacket_.m_iEndNumMember							// 10��
		% static_cast<int>(kPacket_.m_ucLevel)
		% kPacket_.m_iPlayTime
		% kPacket_.m_iEXP
		% kPacket_.m_iED
		% static_cast<int>(kPacket_.m_cComboRank)
		% kPacket_.m_iComboScore
		% static_cast<int>(kPacket_.m_cTechnicalRank)
		% kPacket_.m_iTechnicalScore
		% static_cast<int>(kPacket_.m_cTimeRank)
		% static_cast<int>(kPacket_.m_cDamagedRank)			// 20��
		% kPacket_.m_iDamageNum
		% static_cast<int>(kPacket_.m_cTotalRank)
		% kPacket_.m_iRessurectionStoneCount
		% kPacket_.m_iPassedStageCount
		% kPacket_.m_iPassedSubStageCount
		% kPacket_.m_bIsWithPet
		% kPacket_.m_wstrRegDate
		% kPacket_.m_sNpcDieCount
		% kPacket_.m_iGivenDamage
		% kPacket_.m_iAttackDamage							// 30��
		% kPacket_.m_sUsingSkillKind
		% kPacket_.m_iTotalEXP
		% kPacket_.m_iPetID
		% static_cast<int>(kPacket_.m_cPetEvoStep)
		% kPacket_.m_iDashCount
		% kPacket_.m_iEndHP
		% kPacket_.m_iStartContinue
		% kPacket_.m_iContinueUseStage
		% kPacket_.m_iContinueUseSub
		% kPacket_.m_iFrame									// 40��
		% kPacket_.m_iBaseHP
		% kPacket_.m_iRecovHP
		% kPacket_.m_wstrChannelIP
		% kPacket_.m_iUserUID
		% static_cast<int>(kPacket_.m_cAutoPartyPlay)
		% kPacket_.m_iAutoPartyWaitTime
		% kPacket_.m_iLeaveReason
		% static_cast<int>(kPacket_.m_cRoomState)
		% kPacket_.m_bSkillSlotType
		% iAPoint
		);
#else //SERV_PETID_DATA_TYPE_CHANGE
	DO_QUERY( L"exec dbo.lup_insert_Dungeon_Log", 
		L"N\'%s\', %d, %d, N\'%s\', %d, %d, %d, %d, %d, %d, "
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
		L"%d, %d, %d, %d, %d, %d, N\'%s\', %d, %d, %d, "
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
		L"%d, %d, N\'%s\',%d, %d, %d, %d, %d, %d",
		% wstrIP
		% kPacket_.m_iDungeonID
		% kPacket_.m_nUnitUID
		% kPacket_.m_wstrNickName
		% kPacket_.m_iDifficultyLevel
		% kPacket_.m_iIsChallenge
		% kPacket_.m_iUnitClass
		% kPacket_.m_iClear
		% kPacket_.m_iStartNumMember
		% kPacket_.m_iEndNumMember							// 10��
		% static_cast<int>(kPacket_.m_ucLevel)
		% kPacket_.m_iPlayTime
		% kPacket_.m_iEXP
		% kPacket_.m_iED
		% static_cast<int>(kPacket_.m_cComboRank)
		% kPacket_.m_iComboScore
		% static_cast<int>(kPacket_.m_cTechnicalRank)
		% kPacket_.m_iTechnicalScore
		% static_cast<int>(kPacket_.m_cTimeRank)
		% static_cast<int>(kPacket_.m_cDamagedRank)			// 20��
		% kPacket_.m_iDamageNum
		% static_cast<int>(kPacket_.m_cTotalRank)
		% kPacket_.m_iRessurectionStoneCount
		% kPacket_.m_iPassedStageCount
		% kPacket_.m_iPassedSubStageCount
		% kPacket_.m_bIsWithPet
		% kPacket_.m_wstrRegDate
		% kPacket_.m_sNpcDieCount
		% kPacket_.m_iGivenDamage
		% kPacket_.m_iAttackDamage							// 30��
		% kPacket_.m_sUsingSkillKind
		% kPacket_.m_iTotalEXP
		% static_cast<int>(kPacket_.m_cPetID)
		% static_cast<int>(kPacket_.m_cPetEvoStep)
		% kPacket_.m_iDashCount
		% kPacket_.m_iEndHP
		% kPacket_.m_iStartContinue
		% kPacket_.m_iContinueUseStage
		% kPacket_.m_iContinueUseSub
		% kPacket_.m_iFrame									// 40��
		% kPacket_.m_iBaseHP
		% kPacket_.m_iRecovHP
		% kPacket_.m_wstrChannelIP
		% kPacket_.m_iUserUID
		% static_cast<int>(kPacket_.m_cAutoPartyPlay)
		% kPacket_.m_iAutoPartyWaitTime
		% kPacket_.m_iLeaveReason
		% static_cast<int>(kPacket_.m_cRoomState)
		% kPacket_.m_bSkillSlotType
		);
#endif //SERV_PETID_DATA_TYPE_CHANGE
	//#endif SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
	//}}
#else
	//{{ 2012. 12. 12	������	��Ʋ�ʵ� �ý���
	//#ifdef SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
	DO_QUERY( L"exec dbo.lup_insert_Dungeon_Log", 
		L"N\'%s\', %d, %d, N\'%s\', %d, %d, %d, %d, %d, %d, "
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
		L"%d, %d, %d, %d, %d, %d, N\'%s\', %d, %d, %d, "
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
		L"%d, %d, N\'%s\',%d, %d, %d, %d, %d",
		% wstrIP
		% kPacket_.m_iDungeonID
		% kPacket_.m_nUnitUID
		% kPacket_.m_wstrNickName
		% kPacket_.m_iDifficultyLevel
		% kPacket_.m_iIsChallenge
		% kPacket_.m_iUnitClass
		% kPacket_.m_iClear
		% kPacket_.m_iStartNumMember
		% kPacket_.m_iEndNumMember							// 10��
		% static_cast<int>(kPacket_.m_ucLevel)
		% kPacket_.m_iPlayTime
		% kPacket_.m_iEXP
		% kPacket_.m_iED
		% static_cast<int>(kPacket_.m_cComboRank)
		% kPacket_.m_iComboScore
		% static_cast<int>(kPacket_.m_cTechnicalRank)
		% kPacket_.m_iTechnicalScore
		% static_cast<int>(kPacket_.m_cTimeRank)
		% static_cast<int>(kPacket_.m_cDamagedRank)			// 20��
		% kPacket_.m_iDamageNum
		% static_cast<int>(kPacket_.m_cTotalRank)
		% kPacket_.m_iRessurectionStoneCount
		% kPacket_.m_iPassedStageCount
		% kPacket_.m_iPassedSubStageCount
		% kPacket_.m_bIsWithPet
		% kPacket_.m_wstrRegDate
		% kPacket_.m_sNpcDieCount
		% kPacket_.m_iGivenDamage
		% kPacket_.m_iAttackDamage							// 30��
		% kPacket_.m_sUsingSkillKind
		% kPacket_.m_iTotalEXP
		% static_cast<int>(kPacket_.m_cPetID)
		% static_cast<int>(kPacket_.m_cPetEvoStep)
		% kPacket_.m_iDashCount
		% kPacket_.m_iEndHP
		% kPacket_.m_iStartContinue
		% kPacket_.m_iContinueUseStage
		% kPacket_.m_iContinueUseSub
		% kPacket_.m_iFrame									// 40��
		% kPacket_.m_iBaseHP
		% kPacket_.m_iRecovHP
		% kPacket_.m_wstrChannelIP
		% kPacket_.m_iUserUID
		% static_cast<int>(kPacket_.m_cAutoPartyPlay)
		% kPacket_.m_iAutoPartyWaitTime
		% kPacket_.m_iLeaveReason
		% static_cast<int>(kPacket_.m_cRoomState)
		);
	//#endif SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
	//}}
#endif SERV_USE_SKILL_SLOT_TYPE_LOG
	//}}
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( wstrIP )
		<< BUILD_LOG( kPacket_.m_iDungeonID )
		<< BUILD_LOG( kPacket_.m_nUnitUID )
		<< BUILD_LOG( kPacket_.m_wstrNickName )
		<< BUILD_LOG( kPacket_.m_iDifficultyLevel )
		<< BUILD_LOG( kPacket_.m_iIsChallenge )
		<< BUILD_LOG( kPacket_.m_iUnitClass )
		<< BUILD_LOG( kPacket_.m_iClear )
		<< BUILD_LOG( kPacket_.m_iStartNumMember )
		<< BUILD_LOG( kPacket_.m_iEndNumMember )
		<< BUILD_LOGc( kPacket_.m_ucLevel )
		<< BUILD_LOG( kPacket_.m_iPlayTime )
		<< BUILD_LOG( kPacket_.m_iEXP )
		<< BUILD_LOG( kPacket_.m_iED )
		<< BUILD_LOGc( kPacket_.m_cComboRank )
		<< BUILD_LOG( kPacket_.m_iComboScore )
		<< BUILD_LOGc( kPacket_.m_cTechnicalRank )
		<< BUILD_LOG( kPacket_.m_iTechnicalScore )
		<< BUILD_LOGc( kPacket_.m_cTimeRank )
		<< BUILD_LOGc( kPacket_.m_cDamagedRank )
		<< BUILD_LOG( kPacket_.m_iDamageNum )
		<< BUILD_LOGc( kPacket_.m_cTotalRank )
		<< BUILD_LOG( kPacket_.m_iRessurectionStoneCount )
		<< BUILD_LOG( kPacket_.m_iPassedStageCount )
		<< BUILD_LOG( kPacket_.m_iPassedSubStageCount )
		<< BUILD_LOG( kPacket_.m_bIsWithPet )
		<< BUILD_LOG( kPacket_.m_wstrRegDate )
		<< BUILD_LOG( kPacket_.m_sNpcDieCount )
		<< BUILD_LOG( kPacket_.m_iGivenDamage )
		<< BUILD_LOG( kPacket_.m_iAttackDamage )
		<< BUILD_LOG( kPacket_.m_sUsingSkillKind )
		<< BUILD_LOG( kPacket_.m_iTotalEXP )
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
		<< BUILD_LOGc( kPacket_.m_iPetID )
#else //SERV_PETID_DATA_TYPE_CHANGE
		<< BUILD_LOGc( kPacket_.m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
		<< BUILD_LOGc( kPacket_.m_cPetEvoStep )
		<< BUILD_LOG( kPacket_.m_iDashCount )
		<< BUILD_LOG( kPacket_.m_iEndHP )
		<< BUILD_LOG( kPacket_.m_iStartContinue )
		<< BUILD_LOG( kPacket_.m_iContinueUseStage )
		<< BUILD_LOG( kPacket_.m_iContinueUseSub )
		<< BUILD_LOG( kPacket_.m_iFrame )
		<< BUILD_LOG( kPacket_.m_iBaseHP )
		<< BUILD_LOG( kPacket_.m_iRecovHP )
		<< BUILD_LOG( kPacket_.m_wstrChannelIP )
		//{{ 2012. 11. 6	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		<< BUILD_LOGc( kPacket_.m_cAutoPartyPlay )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2012. 12. 12	������	��Ʋ�ʵ� �ý���
#ifdef SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
		<< BUILD_LOG( kPacket_.m_iLeaveReason )
		<< BUILD_LOGc( kPacket_.m_cRoomState )
#endif SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
		//}}
		//{{ 2012. 12. 12  ��ų ���� ��� Ÿ�� �α� - ��μ�
#ifdef SERV_USE_SKILL_SLOT_TYPE_LOG
		<< BUILD_LOG( kPacket_.m_bSkillSlotType )
#endif SERV_USE_SKILL_SLOT_TYPE_LOG
		<< BUILD_LOG( iAPoint )
		//}}
		<< END_LOG;
}
#endif SERV_DUNGEON_LOG_DB
//}}

//{{ 2010. 11. 20	������	Ư�� �ð� ���� ����
#ifdef SERV_TIME_DROP_MONSTER_EVENT

IMPL_ON_FUNC_NOPARAM( DBE_GET_DUNGEON_EVENT_INFO_REQ )
{
	KDBE_GET_DUNGEON_EVENT_INFO_ACK kPacket;

	// Ư�� �ð� ���� ���� �̺�Ʈ ���� ���
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LExpo_Event_SEL", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() 
														% KBaseServer::GetKObj()->GetUID() );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_get_expo_event", L"%d", % KBaseServer::GetKObj()->GetUID() );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		KDungeonDropEventNpcInfo kInfo;
		UidType iUnitUID = 0;
		int iServerUID = 0;
		std::wstring wstrDropDate;
		std::wstring wstrDieDate;

		FETCH_DATA( kInfo.m_iEventUID
			>> kInfo.m_iNpcID
			>> wstrDropDate
			);

		CTime tDropDate;
		LIF( KncUtil::ConvertStringToCTime( wstrDropDate, tDropDate ) );
		kInfo.m_tDropDate = tDropDate.GetTime();

		kPacket.m_vecEventInfo.push_back( kInfo );
	}

end_proc:
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_SERVER, NULL, DBE_GET_DUNGEON_EVENT_INFO_ACK, kPacket );
	GetKCnRoomManager()->QueueingEventToRoomManager( spEvent );
}

IMPL_ON_FUNC( DBE_UPDATE_EVENT_MONSTER_KILL_INFO_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	CTime tDieDate = CTime( kPacket_.m_tNpcDieDate );
	const std::wstring wstrDieDate = (const wchar_t*)tDieDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LExpo_Event_UPD", L"%d, %d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iEventUID
		% kPacket_.m_iKillUnitUID
		% wstrDieDate
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_update_expo_event", L"%d, %d, N\'%s\'", 
		% kPacket_.m_iEventUID
		% kPacket_.m_iKillUnitUID
		% wstrDieDate
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
		<< BUILD_LOG( kPacket_.m_iEventUID )
		<< BUILD_LOG( kPacket_.m_iKillUnitUID )
		<< BUILD_LOG( wstrDieDate )
		<< END_LOG;
}

#endif SERV_TIME_DROP_MONSTER_EVENT
//}}

//{{ 2010. 12. 8	������	���� ���� ��� ���
#ifdef SERV_APPROPRIATE_LEVEL_STAT
IMPL_ON_FUNC( ELOG_APPROPRIATE_LEVEL_STATISTICS_NOT )
{
	CTime tDungeonStartTime = CTime( kPacket_.m_tDungeonStartTime );
	const std::wstring wstrDungeonStartTime = (const wchar_t*)tDungeonStartTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

	BOOST_TEST_FOREACH( const KUserAppropriateLevelInfo&, kInfo, kPacket_.m_vecUserAppropriateLevel )
	{
		int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LEquipmentLog_INS", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\'", 
			% KBaseServer::GetKObj()->GetServerGroupID()
			% kInfo.m_iUnitUID
			% static_cast<int>(kInfo.m_ucLevel)
			% kPacket_.m_iDungeonID
			% static_cast<int>(kPacket_.m_cDifficultyLevel)
			% kInfo.m_iWeaponLevel
			% kInfo.m_iWeaponGrade
			% kInfo.m_iStartedAtkPhysic
			% kInfo.m_iStartedAtkMagic
			% wstrDungeonStartTime
			);
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.lup_insert_Equipment_Log", L"%d, %d, %d, %d, %d, %d, %d, %d, N\'%s\'", 
			% kInfo.m_iUnitUID
			% static_cast<int>(kInfo.m_ucLevel)
			% kPacket_.m_iDungeonID
			% static_cast<int>(kPacket_.m_cDifficultyLevel)
			% kInfo.m_iWeaponLevel
			% kInfo.m_iWeaponGrade
			% kInfo.m_iStartedAtkPhysic
			% kInfo.m_iStartedAtkMagic
			% wstrDungeonStartTime
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
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< BUILD_LOGc( kInfo.m_ucLevel )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_cDifficultyLevel )
			<< BUILD_LOG( kInfo.m_iWeaponLevel )
			<< BUILD_LOG( kInfo.m_iWeaponGrade )
			<< BUILD_LOG( kInfo.m_iStartedAtkPhysic )
			<< BUILD_LOG( kInfo.m_iStartedAtkMagic )
			<< BUILD_LOG( wstrDungeonStartTime )
			<< END_LOG;
	}
}
#endif SERV_APPROPRIATE_LEVEL_STAT
//}}

//{{ 2011. 01. 26	������	���� Ŭ���� Ÿ�� ������ üũ
#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
IMPL_ON_FUNC( DBE_REG_DUNGEON_CLEAR_TIME_HACK_USER_NOT )
{
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LDungeonTimeHackUser_INS", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrRegDate
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_DGT_HackUser", L"%d, N\'%s\'", 
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrRegDate
		);
#endif //SERV_RENEWAL_SP

end_proc:
	return;
}
#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
//}}

//{{ 2011. 03. 17	��μ�	���� �÷��� �α� ��û���� 
#ifdef SERV_DUNGEON_TEMP_LOG_DB
IMPL_ON_FUNC( DBE_DB_TEMP_LOG_DUNGEON_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LDungeonTempLog_INS", 
		L"%d, "
		L"%d, %d, N\'%s\', %d, %d, %d, %d, %d, %d, %d, N\'%s\',  "		// 11��
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "				// 10��
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "				// 10��
		L"%d, N\'%s\', %d",		// 3��
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iDungeonID
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		% kPacket_.m_iUserUID
		% kPacket_.m_iDifficulty
		% kPacket_.m_bIsChallenge
		% kPacket_.m_iUnitClass
		% kPacket_.m_bIsClear
		% kPacket_.m_iStartMember
		% kPacket_.m_iEndMember									// 10
		% static_cast<int>(kPacket_.m_ucLv)
		% kPacket_.m_iPlayTime
		% static_cast<int>(kPacket_.m_cComboRank)
		% kPacket_.m_iComboScore
		% static_cast<int>(kPacket_.m_cTechnicalRank)
		% kPacket_.m_iTechnScore
		% static_cast<int>(kPacket_.m_cTimeRank)
		% static_cast<int>(kPacket_.m_cDamageRank)
		% static_cast<int>(kPacket_.m_cTotalRank)
		% kPacket_.m_iPassStage									// 20
		% kPacket_.m_iPassSub
		% kPacket_.m_iNormKill
		% kPacket_.m_iLoEliteKill
		% kPacket_.m_iHiEliteKill
		% kPacket_.m_iMidBossKill
		% kPacket_.m_iBossKill
		% kPacket_.m_iAttkedDamage
		% kPacket_.m_iRecovHP
		% kPacket_.m_iBaseHP
		% kPacket_.m_iGivenDamage								// 30
		% kPacket_.m_iPartyDamage
		% kPacket_.m_iSkillUsedCount
		% kPacket_.m_wstrRegDate
		% kPacket_.m_bComeBackParty
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_Dungeon_Temp_Log",
		L"%d, %d, N\'%s\', %d, %d, %d, %d, %d, %d, %d, N\'%s\',  "		// 11��
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "				// 10��
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "				// 10��
		L"%d, N\'%s\', %d",		// 3��
		% kPacket_.m_iDungeonID
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		% kPacket_.m_iUserUID
		% kPacket_.m_iDifficulty
		% kPacket_.m_bIsChallenge
		% kPacket_.m_iUnitClass
		% kPacket_.m_bIsClear
		% kPacket_.m_iStartMember
		% kPacket_.m_iEndMember									// 10
		% static_cast<int>(kPacket_.m_ucLv)
		% kPacket_.m_iPlayTime
		% static_cast<int>(kPacket_.m_cComboRank)
		% kPacket_.m_iComboScore
		% static_cast<int>(kPacket_.m_cTechnicalRank)
		% kPacket_.m_iTechnScore
		% static_cast<int>(kPacket_.m_cTimeRank)
		% static_cast<int>(kPacket_.m_cDamageRank)
		% static_cast<int>(kPacket_.m_cTotalRank)
		% kPacket_.m_iPassStage									// 20
		% kPacket_.m_iPassSub
		% kPacket_.m_iNormKill
		% kPacket_.m_iLoEliteKill
		% kPacket_.m_iHiEliteKill
		% kPacket_.m_iMidBossKill
		% kPacket_.m_iBossKill
		% kPacket_.m_iAttkedDamage
		% kPacket_.m_iRecovHP
		% kPacket_.m_iBaseHP
		% kPacket_.m_iGivenDamage								// 30
		% kPacket_.m_iPartyDamage
		% kPacket_.m_iSkillUsedCount
		% kPacket_.m_wstrRegDate
		% kPacket_.m_bComeBackParty
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
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iDifficulty )
			<< BUILD_LOG( kPacket_.m_bIsChallenge )
			<< BUILD_LOG( kPacket_.m_iUnitClass )
			<< BUILD_LOG( kPacket_.m_bIsClear )
			<< BUILD_LOG( kPacket_.m_iStartMember )
			<< BUILD_LOG( kPacket_.m_iEndMember )
			<< BUILD_LOGc( kPacket_.m_ucLv )
			<< BUILD_LOG( kPacket_.m_iPlayTime )
			<< BUILD_LOGc( kPacket_.m_cComboRank )
			<< BUILD_LOG( kPacket_.m_iComboScore )
			<< BUILD_LOGc( kPacket_.m_cTechnicalRank )
			<< BUILD_LOG( kPacket_.m_iTechnScore )
			<< BUILD_LOGc( kPacket_.m_cTimeRank )
			<< BUILD_LOGc( kPacket_.m_cDamageRank )
			<< BUILD_LOGc( kPacket_.m_cTotalRank )
			<< BUILD_LOG( kPacket_.m_iPassStage )
			<< BUILD_LOG( kPacket_.m_iPassSub )
			<< BUILD_LOG( kPacket_.m_iNormKill )
			<< BUILD_LOG( kPacket_.m_iLoEliteKill )
			<< BUILD_LOG( kPacket_.m_iHiEliteKill )
			<< BUILD_LOG( kPacket_.m_iMidBossKill )
			<< BUILD_LOG( kPacket_.m_iBossKill )
			<< BUILD_LOG( kPacket_.m_iAttkedDamage )
			<< BUILD_LOG( kPacket_.m_iRecovHP )
			<< BUILD_LOG( kPacket_.m_iBaseHP )
			<< BUILD_LOG( kPacket_.m_iGivenDamage )
			<< BUILD_LOG( kPacket_.m_iPartyDamage )
			<< BUILD_LOG( kPacket_.m_iSkillUsedCount )
			<< BUILD_LOG( kPacket_.m_wstrRegDate )
			<< BUILD_LOG( kPacket_.m_bComeBackParty )
			<< END_LOG;
}
#endif SERV_DUNGEON_TEMP_LOG_DB
//}}

//{{ 2011. 06. 13	������	�������� �α�
#ifdef SERV_DUNGEON_STAGE_LOG
IMPL_ON_FUNC( ELOG_DUNGEON_STAGE_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LDungeonStageLog_INS", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iDungeonID
		% kPacket_.m_iStageID
		% kPacket_.m_iUnitUID
		% (int)kPacket_.m_cLevel
		% (int)kPacket_.m_cDifficulty
		% kPacket_.m_bIsChallenge
		% (int)kPacket_.m_cUnitClass
		% kPacket_.m_iStagePlayTime
		% kPacket_.m_iStageDamage
		% kPacket_.m_iResurrectionCount
		% kPacket_.m_wstrRegDate
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_dungeon_stage", 
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\'", 
		% kPacket_.m_iDungeonID
		% kPacket_.m_iStageID
		% kPacket_.m_iUnitUID
		% (int)kPacket_.m_cLevel
		% (int)kPacket_.m_cDifficulty
		% kPacket_.m_bIsChallenge
		% (int)kPacket_.m_cUnitClass
		% kPacket_.m_iStagePlayTime
		% kPacket_.m_iStageDamage
		% kPacket_.m_iResurrectionCount
		% kPacket_.m_wstrRegDate
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
		<< BUILD_LOG( kPacket_.m_iDungeonID )
		<< BUILD_LOG( kPacket_.m_iStageID )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOGc( kPacket_.m_cLevel )
		<< BUILD_LOGc( kPacket_.m_cDifficulty )
		<< BUILD_LOG( kPacket_.m_bIsChallenge )
		<< BUILD_LOGc( kPacket_.m_cUnitClass )
		<< BUILD_LOG( kPacket_.m_iStagePlayTime )
		<< BUILD_LOG( kPacket_.m_iStageDamage )
		<< BUILD_LOG( kPacket_.m_iResurrectionCount )
		<< BUILD_LOG( kPacket_.m_wstrRegDate )
		<< END_LOG;
}
#endif SERV_DUNGEON_STAGE_LOG
//}}

//{{ 2011. 07. 27	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
IMPL_ON_FUNC( DBE_PVP_MATCH_RESULT_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;
	UidType iArenaLogUID = 0;

	//////////////////////////////////////////////////////////////////////////
	// �α� ����� ���� �غ� �۾�

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LArenaUIDCreate_SEL", L"%d",	% KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.lup_get_ArenaUID" );
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK
			>> iArenaLogUID );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"�α� ����� ���� ArenaUID�� ��� ����!" )
			<< BUILD_LOG( iOK )
			<< END_LOG;
		goto end_proc;
	}


	//////////////////////////////////////////////////////////////////////////
	// �α� ���!

	BOOST_TEST_FOREACH( const KPvpMatchUserResultLog&, kUserLog, kPacket_.m_vecPvpMatchUserLog )
	{
		iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LUnitPVPLog_INS", 
			L"%d, "
			L"%d, %d, %d, %d, N\'%s\', %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, N\'%s\', %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, N\'%s\'",
			% KBaseServer::GetKObj()->GetServerGroupID()
			% iArenaLogUID
			% (int)kPacket_.m_cMatchType
			% (int)kPacket_.m_cNpcPvpType
			% kUserLog.m_iUnitUID
			% kUserLog.m_wstrNickName
			% (int)kUserLog.m_ucLevel
			% (int)kUserLog.m_cUnitClass
			% kUserLog.m_iRating
			% (int)kUserLog.m_cPvpRank
			% kUserLog.m_iRPoint
			% kUserLog.m_iAPoint
			% kUserLog.m_iOfficialMatchCnt
			% (int)kUserLog.m_cTeam
			% (int)kUserLog.m_bPartyApp
			% kUserLog.m_iWaitTime
			% kUserLog.m_iWin
			% (int)kUserLog.m_cEndType
			% kPacket_.m_iPlayTime
			% kUserLog.m_iKill
			% kUserLog.m_iAssist
			% kUserLog.m_iDeath
			% kUserLog.m_iNetRatingInc
			% kUserLog.m_iNetRPInc
			% kUserLog.m_iNetAPInc
			% kUserLog.m_iEXPearned
			% kPacket_.m_wstrRegDate
			);
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.lup_insert_unitpvp_log", 
			L"%d, %d, %d, %d, N\'%s\', %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, N\'%s\', %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, N\'%s\'",
			% iArenaLogUID
			% (int)kPacket_.m_cMatchType
			% (int)kPacket_.m_cNpcPvpType
			% kUserLog.m_iUnitUID
			% kUserLog.m_wstrNickName
			% (int)kUserLog.m_ucLevel
			% (int)kUserLog.m_cUnitClass
			% kUserLog.m_iRating
			% (int)kUserLog.m_cPvpRank
			% kUserLog.m_iRPoint
			% kUserLog.m_iAPoint
			% kUserLog.m_iOfficialMatchCnt
			% (int)kUserLog.m_cTeam
			% (int)kUserLog.m_bPartyApp
			% kUserLog.m_iWaitTime
			% kUserLog.m_iWin
			% (int)kUserLog.m_cEndType
			% kPacket_.m_iPlayTime
			% kUserLog.m_iKill
			% kUserLog.m_iAssist
			% kUserLog.m_iDeath
			% kUserLog.m_iNetRatingInc
			% kUserLog.m_iNetRPInc
			% kUserLog.m_iNetAPInc
			% kUserLog.m_iEXPearned
			% kPacket_.m_wstrRegDate
			);
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"���� ��Ī ���� �α� ��� ����!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( iArenaLogUID )
				<< BUILD_LOGc( kPacket_.m_cMatchType )
				<< BUILD_LOGc( kPacket_.m_cNpcPvpType )
				<< BUILD_LOG( kUserLog.m_iUnitUID )
				<< BUILD_LOG( kUserLog.m_wstrNickName )
				<< BUILD_LOGc( kUserLog.m_ucLevel )
				<< BUILD_LOGc( kUserLog.m_cUnitClass )
				<< BUILD_LOG( kUserLog.m_iRating )
				<< BUILD_LOGc( kUserLog.m_cPvpRank )
				<< BUILD_LOG( kUserLog.m_iRPoint )
				<< BUILD_LOG( kUserLog.m_iAPoint )
				<< BUILD_LOG( kUserLog.m_iOfficialMatchCnt )
				<< BUILD_LOGc( kUserLog.m_cTeam )
				<< BUILD_LOGc( kUserLog.m_bPartyApp )
				<< BUILD_LOG( kUserLog.m_iWaitTime )
				<< BUILD_LOG( kUserLog.m_iWin )
				<< BUILD_LOGc( kUserLog.m_cEndType )
				<< BUILD_LOG( kPacket_.m_iPlayTime )
				<< BUILD_LOG( kUserLog.m_iKill )
				<< BUILD_LOG( kUserLog.m_iAssist )
				<< BUILD_LOG( kUserLog.m_iDeath )
				<< BUILD_LOG( kUserLog.m_iNetRatingInc )
				<< BUILD_LOG( kUserLog.m_iNetRPInc )
				<< BUILD_LOG( kUserLog.m_iNetAPInc )
				<< BUILD_LOG( kUserLog.m_iEXPearned )
				<< BUILD_LOG( kPacket_.m_wstrRegDate )
				<< END_LOG;
		}
	}

	return;

end_proc:
	BOOST_TEST_FOREACH( const KPvpMatchUserResultLog&, kUserLog, kPacket_.m_vecPvpMatchUserLog )
	{
		START_LOG( cerr, L"���� ��Ī ���� �α� ��� ����!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( iArenaLogUID )
			<< BUILD_LOGc( kPacket_.m_cMatchType )
			<< BUILD_LOGc( kPacket_.m_cNpcPvpType )
			<< BUILD_LOG( kUserLog.m_iUnitUID )
			<< BUILD_LOG( kUserLog.m_wstrNickName )
			<< BUILD_LOGc( kUserLog.m_ucLevel )
			<< BUILD_LOGc( kUserLog.m_cUnitClass )
			<< BUILD_LOG( kUserLog.m_iRating )
			<< BUILD_LOGc( kUserLog.m_cPvpRank )
			<< BUILD_LOG( kUserLog.m_iRPoint )
			<< BUILD_LOG( kUserLog.m_iAPoint )
			<< BUILD_LOG( kUserLog.m_iOfficialMatchCnt )
			<< BUILD_LOGc( kUserLog.m_cTeam )
			<< BUILD_LOGc( kUserLog.m_bPartyApp )
			<< BUILD_LOG( kUserLog.m_iWaitTime )
			<< BUILD_LOG( kUserLog.m_iWin )
			<< BUILD_LOGc( kUserLog.m_cEndType )
			<< BUILD_LOG( kPacket_.m_iPlayTime )
			<< BUILD_LOG( kUserLog.m_iKill )
			<< BUILD_LOG( kUserLog.m_iAssist )
			<< BUILD_LOG( kUserLog.m_iDeath )
			<< BUILD_LOG( kUserLog.m_iNetRatingInc )
			<< BUILD_LOG( kUserLog.m_iNetRPInc )
			<< BUILD_LOG( kUserLog.m_iNetAPInc )
			<< BUILD_LOG( kUserLog.m_iEXPearned )
			<< BUILD_LOG( kPacket_.m_wstrRegDate )
			<< END_LOG;
	}
}
#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2012. 10. 16	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
IMPL_ON_FUNC( ELOG_COLLECTION_OF_RELAY_AND_P2P_INFO_NOT )
{
	BOOST_TEST_FOREACH( const KDBUpdateGamePlayNetworkInfo&, kInfo, kPacket_.m_vecDBUpdateInfo )
	{
		int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LP2P_RelayLog_INS ", 
			L"%d, %d, %d, %d, %d, N\'%s\', N\'%s\'",
			% KBaseServer::GetKObj()->GetServerGroupID()
			% kInfo.m_iGameType
			% kInfo.m_iHolePunchingTryCount
			% kInfo.m_iP2PPlayTime
			% kInfo.m_iRelayPlayTime
			% kInfo.m_wstrRelayServerIP
			% kInfo.m_wstrRegDate
			);
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LP2P_RelayLog_INT ", 
			L"%d, %d, %d, %d, N\'%s\', N\'%s\'",
			% kInfo.m_iGameType
			% kInfo.m_iHolePunchingTryCount
			% kInfo.m_iP2PPlayTime
			% kInfo.m_iRelayPlayTime
			% kInfo.m_wstrRelayServerIP
			% kInfo.m_wstrRegDate
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
			START_LOG( cerr, L"P2P ��Ʈ��ũ �÷��� ���� �α� ��� ����!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kInfo.m_iGameType )
				<< BUILD_LOG( kInfo.m_iHolePunchingTryCount )
				<< BUILD_LOG( kInfo.m_iP2PPlayTime )
				<< BUILD_LOG( kInfo.m_iRelayPlayTime )
				<< BUILD_LOG( kInfo.m_wstrRelayServerIP )
				<< BUILD_LOG( kInfo.m_wstrRegDate )
				<< END_LOG;
		}
	}
}
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
//}}

//{{ 2012. 09. 05	�ڼ���	������ �α� �߰�
#ifdef UDP_CAN_NOT_SEND_USER_KICK_LOG
IMPL_ON_FUNC( DBE_UDP_CHECK_KICK_USER_NOT )
{
	int iOK;
	CTime tCurrentTime = CTime::GetCurrentTime();
	const std::wstring wstrRegTime = tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

	BOOST_TEST_FOREACH( KLanBugRecord, kLanBugRecord, kPacket_.m_vecLanBugRecord )
	{
		iOK = NetError::ERR_ODBC_01;
		DO_QUERY( L"exec dbo.P_LAccusationLog_INT", 
			L"%d, %d, %d, %d, %d, %d, %d, %d, N\'%s\'",
			% kPacket_.m_iRoomUID
			% kPacket_.m_iPVPChannelClass
			% kPacket_.m_iMatchType
			% kLanBugRecord.m_iMyUnitUID
			% kLanBugRecord.m_iAccusationUnitUID
			% kLanBugRecord.m_byteNum
			% kLanBugRecord.m_bNowKickOut
			% kLanBugRecord.m_iMyUnitWin
			% wstrRegTime
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"���� �� �α� ��� ����" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kPacket_.m_iRoomUID )
				<< BUILD_LOG( kPacket_.m_iPVPChannelClass )
				<< BUILD_LOG( kPacket_.m_iMatchType )
				<< BUILD_LOG( kLanBugRecord.m_iMyUnitUID )
				<< BUILD_LOG( kLanBugRecord.m_iAccusationUnitUID )
				<< BUILD_LOG( kLanBugRecord.m_byteNum )
				<< BUILD_LOG( kLanBugRecord.m_bNowKickOut )
				<< BUILD_LOG( kLanBugRecord.m_iMyUnitWin )
				<< BUILD_LOG( wstrRegTime )
				<< END_LOG;
		}
	}

end_proc:
	return;
}
#endif UDP_CAN_NOT_SEND_USER_KICK_LOG
//}}

//{{ 2012. 10. 18	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
//{{ 2012. 11. 20	�ڼ���	Field Frame �α� �߰�
#ifdef SERV_FIELD_FRAME_LOG
IMPL_ON_FUNC( ELOG_BATTLE_FIELD_LEAVE_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LFieldLog_INS", 
		L"%d, "
		L"N\'%s\', %d, %d, %d, %d, N\'%s\', %d, %d, %d, %d, "
		L"%d, N\'%s\', %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_wstrServerIP
		% kPacket_.m_iBattleFieldID
		% kPacket_.m_iRoomUID
		% static_cast<int>(kPacket_.m_cLeaveType)
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		% static_cast<int>(kPacket_.m_cUnitClass)
		% static_cast<int>(kPacket_.m_ucLevel)
		% kPacket_.m_iPlayTime
		% kPacket_.m_iGetEXP
		% kPacket_.m_iGetED
		% kPacket_.m_wstrRegDate
		% kPacket_.m_iFieldFrame
//#ifdef SERV_ADD_FIELD_LOG_COLUMN
		% kPacket_.m_sNpcDieCount
//#endif SERV_ADD_FIELD_LOG_COLUMN
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LFieldLog_INT", 
		L"N\'%s\', %d, %d, %d, %d, N\'%s\', %d, %d, %d, %d, "
		L"%d, N\'%s\', %d",
		% kPacket_.m_wstrServerIP
		% kPacket_.m_iBattleFieldID
		% kPacket_.m_iRoomUID
		% static_cast<int>(kPacket_.m_cLeaveType)
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		% static_cast<int>(kPacket_.m_cUnitClass)
		% static_cast<int>(kPacket_.m_ucLevel)
		% kPacket_.m_iPlayTime
		% kPacket_.m_iGetEXP
		% kPacket_.m_iGetED
		% kPacket_.m_wstrRegDate
		% kPacket_.m_iFieldFrame
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
		START_LOG( cerr, L"��Ʋ�ʵ� ��Ż �α� ��� ����!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_wstrServerIP )
			<< BUILD_LOG( kPacket_.m_iBattleFieldID )
			<< BUILD_LOG( kPacket_.m_iRoomUID )
			<< BUILD_LOGc( kPacket_.m_cLeaveType )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< BUILD_LOGc( kPacket_.m_cUnitClass )
			<< BUILD_LOGc( kPacket_.m_ucLevel )
			<< BUILD_LOG( kPacket_.m_iPlayTime )
			<< BUILD_LOG( kPacket_.m_iGetEXP )
			<< BUILD_LOG( kPacket_.m_iGetED )
			<< BUILD_LOG( kPacket_.m_wstrRegDate )
			<< BUILD_LOG( kPacket_.m_iFieldFrame )
#ifdef SERV_ADD_FIELD_LOG_COLUMN
			<< BUILD_LOG( kPacket_.m_sNpcDieCount )
#endif SERV_ADD_FIELD_LOG_COLUMN
			<< END_LOG;
	}
}
#else
IMPL_ON_FUNC( ELOG_BATTLE_FIELD_LEAVE_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LFieldLog_INS",
		L"%d, "
		L"N\'%s\', %d, %d, %d, %d, N\'%s\', %d, %d, %d, %d, "
		L"%d, N\'%s\'",
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_wstrServerIP
		% kPacket_.m_iBattleFieldID
		% kPacket_.m_iRoomUID
		% static_cast<int>(kPacket_.m_cLeaveType)
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		% static_cast<int>(kPacket_.m_cUnitClass)
		% static_cast<int>(kPacket_.m_ucLevel)
		% kPacket_.m_iPlayTime
		% kPacket_.m_iGetEXP
		% kPacket_.m_iGetED
		% kPacket_.m_wstrRegDate
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LFieldLog_INT", 
		L"N\'%s\', %d, %d, %d, %d, N\'%s\', %d, %d, %d, %d, "
		L"%d, N\'%s\'",
		% kPacket_.m_wstrServerIP
		% kPacket_.m_iBattleFieldID
		% kPacket_.m_iRoomUID
		% static_cast<int>(kPacket_.m_cLeaveType)
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		% static_cast<int>(kPacket_.m_cUnitClass)
		% static_cast<int>(kPacket_.m_ucLevel)
		% kPacket_.m_iPlayTime
		% kPacket_.m_iGetEXP
		% kPacket_.m_iGetED
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
		START_LOG( cerr, L"��Ʋ�ʵ� ��Ż �α� ��� ����!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_wstrServerIP )
			<< BUILD_LOG( kPacket_.m_iBattleFieldID )
			<< BUILD_LOG( kPacket_.m_iRoomUID )
			<< BUILD_LOGc( kPacket_.m_cLeaveType )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< BUILD_LOGc( kPacket_.m_cUnitClass )
			<< BUILD_LOGc( kPacket_.m_ucLevel )
			<< BUILD_LOG( kPacket_.m_iPlayTime )
			<< BUILD_LOG( kPacket_.m_iGetEXP )
			<< BUILD_LOG( kPacket_.m_iGetED )
			<< BUILD_LOG( kPacket_.m_wstrRegDate )
			<< END_LOG;
	}
}
#endif SERV_FIELD_FRAME_LOG
//}}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 12. 12	������	���â ��Ż ���� �α�
#ifdef SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
IMPL_ON_FUNC( DBE_DUNGEON_RESULT_LEAVE_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LDungeonLeaveLog_INS", L"%d, %d, N\'%s\', N\'%s\', N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrLeaveReason
		% kPacket_.m_wstrRoomState
		% kPacket_.m_wstrRegDate
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LDungeonLeaveLog_INT", 
		L"%d, N\'%s\', N\'%s\', N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrLeaveReason
		% kPacket_.m_wstrRoomState
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
		START_LOG( cerr, L"���� ���â ��Ż �α� ��� ����!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrLeaveReason )
			<< BUILD_LOG( kPacket_.m_wstrRoomState )
			<< BUILD_LOG( kPacket_.m_wstrRegDate )
			<< END_LOG;
	}
}
#endif SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
//}}

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

#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
IMPL_ON_FUNC_NOPARAM( DBE_CHECK_EVENT_UPDATE_REQ )
{
	KDBE_CHECK_EVENT_UPDATE_ACK		AckPacket;
	int iReleaseTickType = 0;
	int iReleaseTick = 0;

	// ReleaseTick ���
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_ReleaseTick_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.lup_get_release_tick" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		FETCH_DATA( iReleaseTickType
			>> iReleaseTick );
		AckPacket.m_mapReleaseTick.insert( std::make_pair( iReleaseTickType, iReleaseTick ) );
	}

end_proc:
	SendToServer( DBE_CHECK_EVENT_UPDATE_ACK, AckPacket );
}
#endif //SERV_ADD_EVENT_DB
#endif SERV_REFRESH_EVENT_USING_RELEASE_TICK

//{{ 2013. 2. 26	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
IMPL_ON_FUNC( DBE_UDP_CHECK_KICK_USER_NOT )
{
	CTime tCurrentTime = CTime::GetCurrentTime();
	const std::wstring wstrRegTime = tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

	DO_QUERY( L"exec dbo.P_LAccusationLog_INT", 
		L"%d, %d, %d, %d, N\'%s\'",
		% kPacket_.m_iRoomUID
		% kPacket_.m_iPVPChannelClass
		% kPacket_.m_iMatchType
		% kPacket_.m_iUnitUID
		% wstrRegTime
		);

	int iOK = NetError::ERR_ODBC_01;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"���� �� �α� ��� ����" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iRoomUID )
			<< BUILD_LOG( kPacket_.m_iPVPChannelClass )
			<< BUILD_LOG( kPacket_.m_iMatchType )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( wstrRegTime )
			<< END_LOG;
	}

end_proc:
	return;
}
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
IMPL_ON_FUNC( DBE_WRITE_RELATIONSHIP_INFO_LOG_REQ )
{
	KDBE_WRITE_RELATIONSHIP_INFO_LOG_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.P_LCoupleSystemLog_INT", L"%d, %d, %d, %d, %d, N\'%s\'", 
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% static_cast<int>(kPacket_.m_ucLevel)
		% static_cast<int>(kPacket_.m_cUnitClass)
		% static_cast<int>(kPacket_.m_cCoupleType)
		% kPacket_.m_wstrRegDate	);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}
end_proc:;
	// ���� �ʿ䰡 ����
	//SendToServer( DBE_WRITE_RELATIONSHIP_INFO_LOG_ACK, kPacket );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef SERV_DUNGEON_STAGE_LOAD_LOG// �۾���¥: 2013-05-15	// �ڼ���
IMPL_ON_FUNC( DBE_DUNGEON_STAGE_LOAD_LOG_NOT )
{
	int iOK;

	BOOST_TEST_FOREACH( const DungonStageLoadLog&, sData, kPacket_.m_vecDungonStageLoadLog )
	{
		DO_QUERY( L"exec dbo.P_LSameDungeonStageLog_INT",
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d",
			% sData.m_iUnitUID
			% kPacket_.m_iRoomUID
			% static_cast<unsigned char>( sData.m_cNormalStage )
			% static_cast<unsigned char>( sData.m_cSecretStage )
			% static_cast<unsigned char>( sData.m_cEliteStage )
			% static_cast<unsigned char>( sData.m_cClearConditionCheckCount )
			% sData.m_iDungeonID
			% sData.m_iNowStageID
			% sData.m_iReqStageID
			% sData.m_bHost
			% static_cast<unsigned char>( sData.m_cMemberNum )
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}
	}

end_proc:
	return;
}
#endif // SERV_DUNGEON_STAGE_LOAD_LOG


#ifdef SERV_HENIR_CLEAR_TIME_HACK_USER_CHECK // 2013-09-10 �����
IMPL_ON_FUNC( ELOG_HENIR_HACKING_LOG_NOT )
{
    /*���ν��� ��: [dbo].[P_LHenirHackUserLog_INS]

    ���� ����:
               @UserUID     bigint
        ,      @UnitUID     bigint
        ,      @StartMember tinyint
        ,      @PlayTime    int
        ,      @RegDate     smalldatetime

        ��ȯ ��: Result (�μ�Ʈ ���� �� -1 ��ȯ)*/
    std::wstring wstrTime = (LPCTSTR)CTime::GetCurrentTime().Format( _T("%y%m%d %H:%M:%S") );
    BOOST_TEST_FOREACH( const KHenirHackUserData&, sData, kPacket_.m_vecHackUser )
    {
        DO_QUERY( L"exec dbo.P_LHenirHackUserLog_INS",
            L"%d, %d, %d, %d, N\'%s\' ",
            % sData.m_iUserUID
            % sData.m_iUnitUID
            % sData.m_iMemberNum
            % sData.m_iPlayTime
            % wstrTime );

        int iOK = NetError::ERR_ODBC_01;
        if( m_kODBC.BeginFetch() )
        {
            FETCH_DATA( iOK );
            m_kODBC.EndFetch();
        }
    }

end_proc:
    return;
}
#endif // SERV_HENIR_CLEAR_TIME_HACK_USER_CHECK

_IMPL_ON_FUNC( ELOG_GEN_FIELD_MIDDLE_BOSS_STAT_NOT, KFieldBossGenKillStat )
{
    DO_QUERY( L"exec dbo.P_StatsFieldBossSpawn_INS",
        L"%d, %d",
        % kPacket_.m_nFieldID 
        % kPacket_.m_nSpawnID
        );

    int iOK = NetError::ERR_ODBC_01;
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );
        m_kODBC.EndFetch();
       
    }
end_proc:
    LOG_SUCCESS( iOK == 0 )
        << BUILD_LOG( kPacket_.m_nFieldID )
        << BUILD_LOG( kPacket_.m_nSpawnID )
        << END_LOG;

    return;
}

_IMPL_ON_FUNC( ELOG_KILL_FIELD_MIDDLE_BOSS_STAT_NOT, KFieldBossGenKillStat )
{
    DO_QUERY( L"exec dbo.P_StatsFieldBossKill_INS",
        L"%d, %d, %d",
        % kPacket_.m_nFieldID 
        % kPacket_.m_nSpawnID
        % kPacket_.m_nNumUser
        );

    int iOK = NetError::ERR_ODBC_01;
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );
        m_kODBC.EndFetch();

    }
end_proc:
    LOG_SUCCESS( iOK == 0 )
        << BUILD_LOG( kPacket_.m_nFieldID )
        << BUILD_LOG( kPacket_.m_nSpawnID )
        << END_LOG;

    return;
}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-12-03	// �ڼ���
IMPL_ON_FUNC( DBE_BOSS_FIELD_LOG_NOT )
{
	// �濡 ���� �α� ����
	DO_QUERY( L"exec dbo.P_LBossFieldLog_INS",
		L"%d, %d, N\'%s\', %d, %d, %d",
		% kPacket_.m_kFieldLog.m_iBossFieldID
		% kPacket_.m_kFieldLog.m_iPortalMapID
		% kPacket_.m_kFieldLog.m_wstrPortalOpenTime
		% kPacket_.m_kFieldLog.m_bSucceed
		% kPacket_.m_kFieldLog.m_iPlayTime
		% kPacket_.m_kFieldLog.m_byteRemainUserCount
		);

	int iOK = NetError::ERR_ODBC_01;
	int iLogUID;

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK
			>> iLogUID
			);
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		LOG_SUCCESS( iOK == NetError::NET_OK )
			<< END_LOG;

		goto end_proc;
	}

	// �濡 �־��� �������� �α� ����
	for( KDBE_BOSS_FIELD_LOG_NOT::TYPE_BOSS_FIELD_USER_LOG::const_iterator it = kPacket_.m_mapUserLog.begin(); it != kPacket_.m_mapUserLog.end(); ++it )
	{
		const std::vector<KBossFieldUserLog>& vecUserLog = it->second;

		BOOST_TEST_FOREACH( const KBossFieldUserLog&, kData, vecUserLog )
		{
			DO_QUERY( L"exec dbo.P_LBossFieldLog_Unit_INS",
				L"%d, %d, N\'%s\', %d, %d, %d, %d, %d, %d, %d, "
				L"%d, %d",
				% iLogUID
				% kData.m_iUnitUID
				% kData.m_wstrNickName
				% kData.m_byteLevel
				% kData.m_byteClass
				% kData.m_byteCompletionType
				% kData.m_byteContributionRank
				% kData.m_iEXP
				% kData.m_iED
				% kData.m_iGivenDamage
				% kData.m_iAttackDamage
				% kData.m_sNumResurrectionStone
				);

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}
		}
	}

end_proc:
	return;
}
#endif // SERV_BATTLE_FIELD_BOSS