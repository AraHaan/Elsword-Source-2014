#include "stdafx.h"
#include ".\x2room.h"

#ifdef HEAP_BROKEN_BY_ROOM
CX2Room::ROOM_PACKET_DATA CX2Room::RoomPacketData;
#endif // HEAP_BROKEN_BY_ROOM


CX2Room::CX2Room()
{
	g_pX2Room		= this;
	m_RoomType		= RT_INVALID;

	m_RoomUID		= 0;
	m_RoomState		= RS_INIT;

	m_fElapsedTime	= 0.0f;

	m_bPublic		= true;
	m_bIntrudeGame	= false;

	m_bBan			= false;
	m_fPingTestTime	= 0.0f;

	m_RelayServerPort = 0;

	m_bNotEnoughSpirit = false;

	m_bRecievePitInNot = true;

	m_wstrCenterServerIP = L"";

	m_iNumOfPlayers = 0;

	g_pData->GetGameUDP()->ClearPeer();
	g_pData->GetGameUDP()->ClearRecvBuffer();

	if( g_pData->GetMyUser()->GetSelectUnit() != NULL )
		g_pData->GetGameUDP()->SetMyUID( g_pData->GetMyUser()->GetSelectUnit()->GetUID() );
	
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	m_bCheckGameStart	= false;
#endif//UDP_CAN_NOT_SEND_USER_KICK
	
	//{{ kimhc // 2012-10-16 // ���� ���� ������ ȣ��Ʈ�� �����ϴ� �ڵ�
#ifdef	SERV_CHOOSE_FASTEST_HOST
	m_dwAveragePingTimeForGameEdit = 0;
#endif	SERV_CHOOSE_FASTEST_HOST
	//}} kimhc // 2012-10-16 // ���� ���� ������ ȣ��Ʈ�� �����ϴ� �ڵ�

#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    m_bSkipPingSendCheck = false;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	m_bForceHostForGameEdit = false;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
}

CX2Room::~CX2Room(void)
{
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	m_bCheckGameStart	= false;
#endif//UDP_CAN_NOT_SEND_USER_KICK
	ClearSlotDataList();
	ClearObserverSlotDataList();	

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	g_pData->GetGameUDP()->DisconnectToRelay();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	g_pData->GetGameUDP()->ClearPeer();
	g_pData->GetGameUDP()->ClearRecvBuffer();

#ifdef	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	m_mapGamePlayNetworkInfo.clear();
#endif	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	g_pX2Room = NULL;

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    if ( g_pData != NULL && g_pData->GetGameUDP() != NULL && g_pMain != NULL )
        g_pData->GetGameUDP()->SetForceConnectMode( g_pMain->GetUDPMode( CX2Game::GT_NONE ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

}

void CX2Room::ConnectRelayServer( const WCHAR* pIP, int port )
{
	//CKTDXThread::CLocker locker( g_pMain->GetUnitLoader()->GetLock() );

	SlotData* pSlotData = GetMySlot();
	if( NULL != pSlotData &&
		NULL != pSlotData->m_pUnit )
	{
		pSlotData->m_pUnit->GetUID();
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        g_pData->GetGameUDP()->SetMyUIDAndRelayIPAddressAndPort( pSlotData->m_pUnit->GetUID(), 
            CKTDNUDP::ConvertIPToAddress( pIP ), port );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		g_pData->GetGameUDP()->SetMyUIDAndRelayIpAndPort( pSlotData->m_pUnit->GetUID(), pIP, port );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        g_pData->GetGameUDP()->DisconnectToRelay();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		g_pData->GetGameUDP()->ConnectTestToRelay();
	}
}

HRESULT CX2Room::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();
	m_fElapsedTime = fElapsedTime;

	SlotData* pMySlotData = GetMySlot();
	if( pMySlotData != NULL &&
		pMySlotData->m_SlotState == SS_WAIT )
	{
		// note!! �������ΰ�쿡�� ���Ӿȿ��� unitviewer�� �������� �ʵ��� �Ѵ�
		if( true != pMySlotData->m_bObserver ||
			( CX2Main::XS_DUNGEON_GAME != g_pMain->GetNowStateID() &&
			CX2Main::XS_PVP_GAME != g_pMain->GetNowStateID() &&
			CX2Main::XS_TRAINING_GAME != g_pMain->GetNowStateID() ) )
		{
			for( int i = 0; i < (int)m_SlotDataList.size(); i++ )
			{
				SlotData* pSlotData = m_SlotDataList[i];
				if( pSlotData != NULL )
				{
					if( pSlotData->m_SlotState == SS_WAIT
						|| pSlotData->m_SlotState == SS_LOADING
						|| pSlotData->m_SlotState == SS_PLAY )
					{
						if( pSlotData->m_pUnitViewer == NULL )
							pSlotData->MakeUnitViewer();
						else if ( pMySlotData->m_bPitIn == false )
							pSlotData->m_pUnitViewer->SetShowObject( true );
					}
				}
			}
		}
	}

	for( int i=0; i<(int)m_SlotDataList.size(); i++ )
	{
		CX2Room::SlotData* pSlotData = GetSlotData( i );
		if( NULL == pSlotData )
			continue;

		if( pSlotData->m_fElapsedTimeAfterJoinRoom < 5.f )
		{
			switch( pSlotData->m_SlotState )
			{
			case CX2Room::SS_EMPTY:
			case CX2Room::SS_CLOSE:
				{
				} break;
			case CX2Room::SS_WAIT:
			case CX2Room::SS_LOADING:
			case CX2Room::SS_PLAY:
				{
					pSlotData->m_fElapsedTimeAfterJoinRoom += fElapsedTime;
				} break;
			}
		}
	}

	
	if ( GetRoomState() != CX2Room::RS_PLAY && GetRoomState() != CX2Room::RS_LOADING )
	{
		CX2Room::ROOM_TYPE roomType = GetRoomType();

		int otherUserNum = 0;
		bool bCheckAllReadyExceptMe = true;
		bool bCheckAllReadyExceptMeAndOpponentToTrade = true;

		int bRedTeamNum = 0;
		int bBlueTeamNum = 0;

		bool bCheckPlayWarning = false;

		for( int i = 0; i < (int)m_SlotDataList.size(); i++ )
		{
			CX2Room::SlotData* pSlotData = GetSlotData( i );
			if( NULL == pSlotData )
				continue;

			switch( pSlotData->m_SlotState )
			{
			case CX2Room::SS_EMPTY:
			case CX2Room::SS_CLOSE:
				{

				} 
				break;
			case CX2Room::SS_WAIT:
				{
					if ( pSlotData->m_pUnit != NULL && 
						pSlotData->m_pUnit->GetUID() != g_pData->GetMyUser()->GetSelectUnit()->GetUID() )
					{
						otherUserNum++;

						if ( pSlotData->GetIsHost() == false && pSlotData->GetIsReady() == false )
						{
							bCheckAllReadyExceptMe = false;
						}
						else if ( pSlotData->GetIsHost() == true && pSlotData->m_bPitIn == true )
							bCheckAllReadyExceptMe = false;


						if ( pSlotData->GetIsReady() == false && pSlotData->m_bTrade == false )
						{
							bCheckAllReadyExceptMeAndOpponentToTrade = false;
						}
					}

					if ( (CX2Room::TEAM_NUM)pSlotData->m_TeamNum == CX2Room::TN_RED )
					{
						bRedTeamNum++;
					}
					else if ( (CX2Room::TEAM_NUM)pSlotData->m_TeamNum == CX2Room::TN_BLUE )
					{
						bBlueTeamNum++;
					}
				}
				break;
			}
		}

		bool bCheckBalance = true;

		if ( roomType == CX2Room::RT_PVP )
		{
			CX2PVPRoom* pPVPRoom = (CX2PVPRoom*)this;

			if ( pPVPRoom->GetPVPGameType() == CX2PVPRoom::PGT_TEAM )
			{
				if ( bRedTeamNum != bBlueTeamNum )
				{
					bCheckBalance = false;
				}
			}
		}
	}

	NetworkProcess();

	return S_OK;
}

void CX2Room::NetworkProcess()
{
	KTDXPROFILE();
	vector<UidType> slotUIDList;
	//�� �������� �Ǿ�� ������Ų��.
	bool bAddNewPeer = false;
	for( int i = 0; i < (int)m_SlotDataList.size(); i++ )
	{
		//���Կ� �ִ� ������ �Ǿ ����Ѵ�
		SlotData* pSlotData = GetSlotData( i );

#ifdef SERV_PVP_NEW_SYSTEM
		if( pSlotData->m_bNpc == true )
			continue;
#endif

		if( pSlotData != NULL && pSlotData->m_pUnit != NULL )
		{
			if( pSlotData->GetSlotState() != SS_EMPTY && pSlotData->GetSlotState() != SS_CLOSE )
			{
				slotUIDList.push_back( pSlotData->m_UnitUID );
				const CKTDNUDP::Peer* pPeer = g_pData->GetGameUDP()->GetPeer( pSlotData->m_UnitUID );
				if( pPeer == NULL )
				{
					bAddNewPeer = true;


					g_pData->GetGameUDP()->AddPeer( pSlotData->m_UnitUID, 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                        CKTDNUDP::ConvertIPToAddress( pSlotData->m_pUnit->GetUnitData().m_IP.c_str() ), 
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//                        pSlotData->m_pUnit->GetUnitData().m_IP.c_str(), 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                        pSlotData->m_pUnit->GetUnitData().m_Port
					//{{ 2013. 1. 9	�ڼ���	Merge ����IP ���� ���н� ����IP�� �õ�( ������ )
//#ifdef  SERV_KTDX_RETRY_USING_INTERNAL_IP
                        , 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                        CKTDNUDP::ConvertIPToAddress( pSlotData->m_pUnit->GetUnitData().m_InternalIP.c_str() ), 
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//                        pSlotData->m_pUnit->GetUnitData().m_InternalIP.c_str(), 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                        pSlotData->m_pUnit->GetUnitData().m_InternalPort
//#endif  SERV_KTDX_RETRY_USING_INTERNAL_IP
                        );

					//}}
					
#ifdef	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
					InsertGamePlayNetworkInfo( pSlotData );
#endif	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
				}
#ifdef	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
				else
				{
					UpdateGamePlayNetworkInfo( pPeer );
				}
#endif	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
			}			
		}
	}

	for( int i = 0; i < (int)m_ObserverSlotDataList.size(); i++ )
	{
		//���Կ� �ִ� ������ �Ǿ ����Ѵ�
		SlotData* pSlotData = GetObserverSlotData( i );
		if( pSlotData != NULL && pSlotData->m_pUnit != NULL )
		{
			if( pSlotData->GetSlotState() != SS_EMPTY && pSlotData->GetSlotState() != SS_CLOSE )
			{
				slotUIDList.push_back( pSlotData->m_UnitUID );
				const CKTDNUDP::Peer* pPeer = g_pData->GetGameUDP()->GetPeer( pSlotData->m_UnitUID );
				if( pPeer == NULL )
				{
					bAddNewPeer = true;
					//{{ 2013. 1. 9	�ڼ���	Merge ����IP ���� ���н� ����IP�� �õ�( ������ )

					g_pData->GetGameUDP()->AddPeer( pSlotData->m_UnitUID, 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                        CKTDNUDP::ConvertIPToAddress( pSlotData->m_pUnit->GetUnitData().m_IP.c_str() ),
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//                        pSlotData->m_pUnit->GetUnitData().m_IP.c_str(), 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                        pSlotData->m_pUnit->GetUnitData().m_Port
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
                        , 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                        CKTDNUDP::ConvertIPToAddress( pSlotData->m_pUnit->GetUnitData().m_InternalIP.c_str() ),
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//                        pSlotData->m_pUnit->GetUnitData().m_InternalIP.c_str(), 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                        pSlotData->m_pUnit->GetUnitData().m_InternalPort 
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
                        );

					//}}
				}
			}			
		}
	}

	//���Կ� ���� ������ �Ǿ�� �����Ѵ�
	g_pData->GetGameUDP()->RemoveOtherPeer( slotUIDList );

	if( bAddNewPeer == true )	/// ���⼭�� ���ο� ������ ���� ��쿡 �׽�Ʈ
	{
		g_pData->GetGameUDP()->ConnectTestToPeer();	
	}

	//�� �׽�Ʈ�� �Ѵ�.

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    m_fPingTestTime += g_pKTDXApp->GetElapsedTime();
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	m_fPingTestTime += m_fElapsedTime;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	if( m_fPingTestTime > 5.0f 
		
		//{{ kimhc // 2012-10-16 // ���� ���� ������ ȣ��Ʈ�� �����ϴ� �ڵ�
#ifdef	SERV_CHOOSE_FASTEST_HOST
		&& g_pData->GetGameUDP()->ConnectTestResult() /// KTDNUDP�� ConnectTestToPeer�� ���� ������ �� �޾�����
#endif	SERV_CHOOSE_FASTEST_HOST
		//}} kimhc // 2012-10-16 // ���� ���� ������ ȣ��Ʈ�� �����ϴ� �ڵ�
		)
	{
		m_fPingTestTime = 0.0f;

		//{{ kimhc // 2012-10-16 // ���� ���� ������ ȣ��Ʈ�� �����ϴ� �ڵ�
#ifdef	SERV_CHOOSE_FASTEST_HOST
		DWORD dwAveragePingTime = 0;
		if ( GetAverageTestPingTimeToOthers( dwAveragePingTime ) )
			Send_EGS_UDP_PING_SCORE_NOT( dwAveragePingTime );
#endif	SERV_CHOOSE_FASTEST_HOST
		//}} kimhc // 2012-10-16 // ���� ���� ������ ȣ��Ʈ�� �����ϴ� �ڵ�

		TestPing();
	}

	//��Ŷ �ڵ鸵�� �Ѵ�.
	P2PPacketHandler();
}

void CX2Room::P2PPacketHandler()
{
	KTDXPROFILE();

	CKTDNUDP::RecvData* pRecvData = g_pData->GetGameUDP()->PopRecvData();
	while( pRecvData != NULL )
	{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
		P2PPacketHandler( pRecvData );
		SAFE_DELETE( pRecvData );
		pRecvData = g_pData->GetGameUDP()->PopRecvData();		
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		bool bRetVal = false;
//		if( g_pX2Game != NULL )
//		{
//			bRetVal = g_pX2Game->P2PPacketHandler( pRecvData );
//		}
//
//		if( bRetVal == false )
//		{
//			switch( pRecvData->m_ID )
//			{
//			case XPT_PING_TEST_REQ:
//				{
//					KSerBuffer ksBuff;
//					ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
//					KXPT_PING_TEST_REQ kKXPT_PING_TEST_REQ;
//					DeSerialize( &ksBuff, &kKXPT_PING_TEST_REQ );
//
//					KXPT_PING_TEST_ACK kKXPT_PING_TEST_ACK;
//					kKXPT_PING_TEST_ACK.m_UnitUID	=  GetMySlot()->m_pUnit->GetUID();
//					kKXPT_PING_TEST_ACK.m_SendTime	=  kKXPT_PING_TEST_REQ.m_SendTime;
//
//					KSerBuffer buff;
//					Serialize( &buff, &kKXPT_PING_TEST_ACK );
//					g_pData->GetGameUDP()->Send( kKXPT_PING_TEST_REQ.m_UnitUID, XPT_PING_TEST_ACK, (char*)buff.GetData(), buff.GetLength() );
//				}
//				break;
//
//			case XPT_PING_TEST_ACK:
//				{
//					KSerBuffer ksBuff;
//					ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
//					KXPT_PING_TEST_ACK kKXPT_PING_TEST_ACK;
//					DeSerialize( &ksBuff, &kKXPT_PING_TEST_ACK );
//
//					SlotData* pSlotData = GetSlotDataByUnitUID( kKXPT_PING_TEST_ACK.m_UnitUID );
//					if( pSlotData != NULL )
//					{
//                      LONG    lTimeDiff = (LONG) ( timeGetTime() - kKXPT_PING_TEST_ACK.m_SendTime );
//                        DWORD   dwPingTime = ( lTimeDiff >= 0 ) ? lTimeDiff : 0;
//                        if ( dwPingTime > 5000 )
//                            dwPingTime = 5000;
//				        pSlotData->m_PingTime = dwPingTime
//#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
//                        float   fInvWeight = 1.f/(float) ( pSlotData->m_dwPingCount + 1 );
//                        float   fCutPingTime = __min(pSlotData->m_PingTime,2000) * 0.001f;
//                        pSlotData->m_fAvgPingTime = ( pSlotData->m_dwPingCount * pSlotData->m_fAvgPingTime + fCutPingTime ) * fInvWeight;
//#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
//                        pSlotData->m_fAvgPingTimeSqr = ( pSlotData->m_dwPingCount * pSlotData->m_fAvgPingTimeSqr + fCutPingTime * fCutPingTime ) * fInvWeight;
//                        float fPingStd = pSlotData->m_fAvgPingTimeSqr - pSlotData->m_fAvgPingTime * pSlotData->m_fAvgPingTime;
//                        pSlotData->m_fAvgPingStd = ( fPingStd <= 0.f ) ? 0.f : sqrtf( fPingStd );
//#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
//                        if ( pSlotData->m_dwPingCount < 12 )
//                            pSlotData->m_dwPingCount++;
//#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
//					}
//				}
//				break;
//			}
//		}		
//		SAFE_DELETE( pRecvData );
//		pRecvData = g_pData->GetGameUDP()->PopRecvData();
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK			
	}

#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    SetSkipPingSendCheck( false );
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
}

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
bool    CX2Room::P2PPacketHandler( CKTDNUDP::RecvData* pRecvData )
{
	KTDXPROFILE();

	if( NULL == pRecvData )
		return false;

	if( g_pX2Game != NULL )
	{
		bool bRetVal = g_pX2Game->P2PPacketHandler( pRecvData );
        if ( bRetVal == true )
            return true;
	}//if

	switch( pRecvData->m_ID )
    {
    case XPT_PORT_CHECK_REQ:
        return true;
    case XPT_PORT_CHECK_ACK:
        return true;
	case XPT_PING_TEST_REQ:
		{
            if ( pRecvData->m_Size != sizeof(KXPT_PING_TEST_REQ) )
                return false;
            const KXPT_PING_TEST_REQ* pkReq = (const KXPT_PING_TEST_REQ*) pRecvData->m_pRecvBuffer;
			KXPT_PING_TEST_ACK  kKXPT_PING_TEST_ACK;
			kKXPT_PING_TEST_ACK.m_UnitUID	=  GetMySlot()->m_pUnit->GetUID();
			kKXPT_PING_TEST_ACK.m_SendTime	= pkReq->m_SendTime;
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
			kKXPT_PING_TEST_ACK.m_bRelay = pkReq->m_bRelay;
			kKXPT_PING_TEST_ACK.m_uiSendCounter	= pkReq->m_uiSendCounter;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
            kKXPT_PING_TEST_ACK.m_bIgnoreAck = GetSkipPingSendCheck();
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
            g_pData->GetGameUDP()->SendToPeer( pkReq->m_UnitUID, XPT_PING_TEST_ACK, (char*) &kKXPT_PING_TEST_ACK, sizeof(KXPT_PING_TEST_ACK) );
		} 
        return true;
    case XPT_PING_TEST_ACK:
        {
            if ( pRecvData->m_Size != sizeof(KXPT_PING_TEST_ACK) )
                return false;
            const KXPT_PING_TEST_ACK* pkAck = (const KXPT_PING_TEST_ACK*) pRecvData->m_pRecvBuffer;
			CX2Room::SlotData* pSlotData = GetSlotDataByUnitUID( pkAck->m_UnitUID );
			if( pSlotData != NULL )
			{
                DWORD   dwCurrTime = timeGetTime();
                LONG    lTimeDiff = (LONG) ( dwCurrTime - pkAck->m_SendTime );
                DWORD   dwPingTime = ( lTimeDiff >= 0 ) ? lTimeDiff : 0;
                if ( dwPingTime > 5000 )
                    dwPingTime = 5000;
                bool    bValidPingTime = false;
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
                switch( pSlotData->GetSlotState() )
                {
                case CX2Room::SS_CLOSE:
                case CX2Room::SS_EMPTY:
                    break;
                default:
                    if ( pSlotData->m_uPingSendCount > 0 )
                    {
                        unsigned u = 0;
                        unsigned uMax = pSlotData->m_uPingSendCount;
                        unsigned uBase = 0;
                        if ( uMax > ARRAY_SIZE( pSlotData->m_adwPingSendTime ) )
                        {
                            uBase = uMax - ARRAY_SIZE( pSlotData->m_adwPingSendTime );
                            uMax = ARRAY_SIZE( pSlotData->m_adwPingSendTime );
                        }
                        for( ; u < uMax; u++ )
                        {
                            if ( pSlotData->m_adwPingSendTime[u] == pkAck->m_SendTime )
                                break;
                        }
                        if ( u < uMax )
                        {
                            for( unsigned v = u + 1; v < uMax; v++ )
                            {
                                pSlotData->m_adwPingSendTime[ v - 1 ] = pSlotData->m_adwPingSendTime[v];
                            }
                            if (GetSkipPingSendCheck() == false && pkAck->m_bIgnoreAck == false )
                            {
                                lTimeDiff = (LONG) ( dwCurrTime - pkAck->m_SendTime );
                                pSlotData->m_PingTime = (lTimeDiff > 0 ) ? lTimeDiff * ( uBase + 1 ) : 0;
                                if ( pSlotData->m_PingTime > 5000 )
                                    pSlotData->m_PingTime = 5000;
                                bValidPingTime = true;
                            }
                            pSlotData->m_uPingSendCount -= ( uBase + 1 );
                        }
                    }
                    break;
                }//switch
#else   SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
                bValidPingTime = true;
				pSlotData->m_PingTime = dwPingTime;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
	
				
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
                if ( bValidPingTime == true )
                {
                    float   fInvWeight = 1.f/(float) ( pSlotData->m_dwPingCount + 1 );
                    float   fCutPingTime = __min(pSlotData->m_PingTime,2000) * 0.001f;
                    pSlotData->m_fAvgPingTime = ( pSlotData->m_dwPingCount * pSlotData->m_fAvgPingTime + fCutPingTime ) * fInvWeight;
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
                    pSlotData->m_fAvgPingTimeSqr = ( pSlotData->m_dwPingCount * pSlotData->m_fAvgPingTimeSqr + fCutPingTime * fCutPingTime ) * fInvWeight;
                    float fPingStd = pSlotData->m_fAvgPingTimeSqr - pSlotData->m_fAvgPingTime * pSlotData->m_fAvgPingTime;
                    pSlotData->m_fAvgPingStd = ( fPingStd <= 0.f ) ? 0.f : sqrtf( fPingStd );
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
                    if ( pSlotData->m_dwPingCount < 12 )
                        pSlotData->m_dwPingCount++;
                }
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
			}
        }
        return true;
    }//switch


    return false;
}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

void CX2Room::TestPing()
{
	KXPT_PING_TEST_REQ kKXPT_PING_TEST_REQ;
	if( GetMySlot() == NULL )
		return;

    DWORD   dwSendTime = timeGetTime();
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    for( int iSlotIndex = 0; iSlotIndex < GetSlotNum(); ++iSlotIndex )
    {
        SlotData* pSlotData = GetSlotData( iSlotIndex );
        if ( pSlotData == NULL || pSlotData->m_SlotState == CX2Room::SS_CLOSE || pSlotData->m_SlotState == CX2Room::SS_EMPTY )
            continue;
        if ( pSlotData->m_uPingSendCount < ARRAY_SIZE(pSlotData->m_adwPingSendTime) )
        {
            pSlotData->m_adwPingSendTime[pSlotData->m_uPingSendCount] = dwSendTime;
        }
        else
        {
            for( unsigned u = 0; u < ARRAY_SIZE(pSlotData->m_adwPingSendTime) - 1; u++ )
            {
                pSlotData->m_adwPingSendTime[u] = pSlotData->m_adwPingSendTime[u+1];
            }
            pSlotData->m_adwPingSendTime[ARRAY_SIZE(pSlotData->m_adwPingSendTime) - 1] = dwSendTime;
        }
        pSlotData->m_uPingSendCount++;
    }
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE

	kKXPT_PING_TEST_REQ.m_UnitUID	=  GetMySlot()->m_pUnit->GetUID();
	kKXPT_PING_TEST_REQ.m_SendTime	=  dwSendTime;


#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	std::map<__int64,CKTDNUDP::Peer>::const_iterator iter;
	const std::map<__int64,CKTDNUDP::Peer>& mapPeer = g_pData->GetGameUDP()->GetPeerMap();
    UidType MyUID = g_pData->GetGameUDP()->GetMyUID();
	for( iter = mapPeer.begin(); iter != mapPeer.end(); iter++ )
	{
		const CKTDNUDP::Peer* pPeer = &iter->second;

		if( pPeer->m_UID != MyUID )
		{
			kKXPT_PING_TEST_REQ.m_bRelay = pPeer->GetUseRelay();
			kKXPT_PING_TEST_REQ.m_uiSendCounter = KOGGamePerformanceCheck::GetInstance()->GetUdpPingCheckMgr()->SendPingCheckPacket();

			//��ȿ�� SendCounter�� ������ ��쿡��...
			if( !kKXPT_PING_TEST_REQ.m_bRelay )
			{
				KOGGamePerformanceCheck::GetInstance()->GetP2PPingCheckMgr()->SendPingCheckPacket( kKXPT_PING_TEST_REQ.m_uiSendCounter );
			}
			else
			{
				KOGGamePerformanceCheck::GetInstance()->GetRelayPingCheckMgr()->SendPingCheckPacket( kKXPT_PING_TEST_REQ.m_uiSendCounter );
			}
            g_pData->GetGameUDP()->SendToPeer( pPeer->m_UID, XPT_PING_TEST_REQ, (char*)&kKXPT_PING_TEST_REQ, sizeof(KXPT_PING_TEST_REQ) );
		}
	}



//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//	map<__int64,CKTDNUDP::Peer>::const_iterator iter;
//	const map<__int64,CKTDNUDP::Peer>& mapPeer = g_pData->GetGameUDP()->GetPeerMap();
//	for( iter = mapPeer.begin(); iter != mapPeer.end(); iter++ )
//	{
//		const CKTDNUDP::Peer* pPeer = &iter->second;
//
//		if( pPeer != g_pData->GetGameUDP()->GetMyPeer() )
//		{
//			kKXPT_PING_TEST_REQ.m_bRelay = pPeer->GetUseRelay();
//			kKXPT_PING_TEST_REQ.m_uiSendCounter = KOGGamePerformanceCheck::GetInstance()->GetUdpPingCheckMgr()->SendPingCheckPacket();
//
//			//��ȿ�� SendCounter�� ������ ��쿡��...
//			if( !kKXPT_PING_TEST_REQ.m_bRelay )
//			{
//				KOGGamePerformanceCheck::GetInstance()->GetP2PPingCheckMgr()->SendPingCheckPacket( kKXPT_PING_TEST_REQ.m_uiSendCounter );
//			}
//			else
//			{
//				KOGGamePerformanceCheck::GetInstance()->GetRelayPingCheckMgr()->SendPingCheckPacket( kKXPT_PING_TEST_REQ.m_uiSendCounter );
//			}
//
//			KSerBuffer buff;
//			Serialize( &buff, &kKXPT_PING_TEST_REQ );
//			g_pData->GetGameUDP()->Send( pPeer->m_UID, XPT_PING_TEST_REQ, (char*)buff.GetData(), buff.GetLength() );
//		}
//	}
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#else//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    g_pData->GetGameUDP()->BroadCast( XPT_PING_TEST_REQ, (char*)&kKXPT_PING_TEST_REQ, sizeof(KXPT_PING_TEST_REQ) );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	KSerBuffer buff;
//	Serialize( &buff, &kKXPT_PING_TEST_REQ );
//	g_pData->GetGameUDP()->BroadCast( XPT_PING_TEST_REQ, (char*)buff.GetData(), buff.GetLength() );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
}

CX2Room::SlotData* CX2Room::GetSlotData( int index )
{ 
	if ( index < 0 || index >= (int)m_SlotDataList.size() )
	{
		return NULL;
	}

	return m_SlotDataList[index];
}

CX2Room::SlotData* CX2Room::GetObserverSlotData( int index )
{ 
	if ( index < 0 || index >= (int)m_ObserverSlotDataList.size() )
	{
		return NULL;
	}

	return m_ObserverSlotDataList[index];
}

CX2Room::SlotData* CX2Room::GetObserverSlotDataByUnitUID( UidType unitUID )
{
	KTDXPROFILE();

	for( int i = 0; i < (int)m_ObserverSlotDataList.size(); i++ )
	{
		SlotData* pSlotData = m_ObserverSlotDataList[i];
		if ( pSlotData != NULL )
		{
			if ( pSlotData->m_UnitUID == unitUID )
			{
				return pSlotData;
			}
		}
	}

	return NULL;
}

CX2Room::SlotData* CX2Room::GetObserverSlotDataByNickName( const WCHAR* pNickName )
{
	if ( pNickName == NULL )
		return NULL;

	for( int i = 0; i < (int)m_ObserverSlotDataList.size(); i++ )
	{
		SlotData* pSlotData = m_ObserverSlotDataList[i];
		if ( pSlotData != NULL && pSlotData->m_pUnit != NULL )
		{
			if ( wcscmp( pNickName, pSlotData->m_pUnit->GetNickName() ) == 0 ) 
			{
				return pSlotData;
			}
		}
	}

	return NULL;
}

CX2Room::SlotData* CX2Room::GetSlotDataByUnitUID( UidType unitUID )
{
	KTDXPROFILE();

	for( int i = 0; i < (int)m_SlotDataList.size(); i++ )
	{
		SlotData* pSlotData = m_SlotDataList[i];
		if ( pSlotData != NULL )
		{
			if ( pSlotData->m_UnitUID == unitUID )
			{
				return pSlotData;
			}
		}
	}

	return NULL;
}

CX2Room::SlotData* CX2Room::GetSlotDataByNickName( const WCHAR* pNickName )
{
	if ( pNickName == NULL )
		return NULL;

	for( int i = 0; i < (int)m_SlotDataList.size(); i++ )
	{
		SlotData* pSlotData = m_SlotDataList[i];
		if ( pSlotData != NULL && pSlotData->m_pUnit != NULL )
		{
			if ( wcscmp( pNickName, pSlotData->m_pUnit->GetNickName() ) == 0 ) 
			{
				return pSlotData;
			}
		}
	}

	return NULL;
}


#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
void    CX2Room::ResetSlotPingSendsAll()
{
	for( int i = 0; i < (int)m_SlotDataList.size(); i++ )
	{
		SlotData* pSlotData = m_SlotDataList[i];
		if ( pSlotData != NULL && pSlotData->GetSlotState() != SS_EMPTY && pSlotData->GetSlotState() != SS_CLOSE )
		{
            pSlotData->m_uPingSendCount = 0;
            ZeroMemory( pSlotData->m_adwPingSendTime, sizeof(pSlotData->m_adwPingSendTime) );
		}
	}
    m_bSkipPingSendCheck = true;
}

bool    CX2Room::ResetSlotPingSendByUnitUID( UidType unitUID )
{
    CX2Room::SlotData* pSlotData = GetSlotDataByUnitUID( unitUID );
    if ( pSlotData != NULL )
    {
        ZeroMemory( pSlotData->m_adwPingSendTime, sizeof(pSlotData->m_adwPingSendTime) );
        return true;
    }
    return false;
}
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE


int	CX2Room::GetIndexByUnitUID( UidType unitUid )
{
	for( int i = 0; i < (int)m_SlotDataList.size(); i++ )
	{
		SlotData* pSlotData = m_SlotDataList[i];
		if ( pSlotData != NULL )
		{
			if ( pSlotData->m_UnitUID == unitUid )
			{
				return i;
			}
		}
	}

	return -1;
}

int CX2Room::GetUserNum()
{
	int retVal = 0;
	for( int i = 0; i < (int)m_SlotDataList.size(); i++ )
	{
		SlotData* pSlotData = m_SlotDataList[i];
		if( pSlotData != NULL && pSlotData->GetSlotState() != SS_EMPTY && pSlotData->GetSlotState() != SS_CLOSE )
		{
			retVal++;
		}
	}
	return retVal;
}

int CX2Room::GetTeamNum( int teamNum, bool bGame )
{
	int retVal = 0;
	for( int i = 0; i < (int)m_SlotDataList.size(); i++ )
	{
		SlotData* pSlotData = m_SlotDataList[i];
		if( pSlotData != NULL && pSlotData->GetSlotState() != SS_EMPTY && pSlotData->GetSlotState() != SS_CLOSE )
		{
			if( pSlotData->GetTeamNum() == teamNum )
			{
				if( bGame == true )
				{
					if( pSlotData->GetSlotState() == SS_PLAY )
						retVal++;
				}
				else
				{
					retVal++;
				}
			}			
		}
	}
	return retVal;
}

CX2Room::SlotData* CX2Room::GetHostSlot()
{
	for( int i = 0; i < (int)m_SlotDataList.size(); i++ )
	{
		SlotData* pSlotData = m_SlotDataList[i];
		if ( pSlotData != NULL )
		{
			if( pSlotData->m_SlotState == SS_WAIT 
				|| pSlotData->m_SlotState == SS_LOADING
				|| pSlotData->m_SlotState == SS_PLAY )
			{
				if ( pSlotData->m_bHost == true  )
				{
					return pSlotData;
				}
			}
		}
	}

	return NULL;
}

CX2Room::SlotData* CX2Room::GetMySlot()
{
	for( int i = 0; i < (int)m_SlotDataList.size(); i++ )
	{
		SlotData* pSlotData = m_SlotDataList[i];
		if ( pSlotData != NULL )
		{
			if( pSlotData->m_SlotState == SS_WAIT 
				|| pSlotData->m_SlotState == SS_LOADING
				|| pSlotData->m_SlotState == SS_PLAY )
			{
				if ( pSlotData->m_bMySlot == true  )
				{
					return pSlotData;
				}
			}
		}
	}

	for( int i = 0; i < (int)m_ObserverSlotDataList.size(); i++ )
	{
		SlotData* pSlotData = m_ObserverSlotDataList[i];
		if ( pSlotData != NULL )
		{
			if( pSlotData->m_SlotState == SS_WAIT 
				|| pSlotData->m_SlotState == SS_LOADING
				|| pSlotData->m_SlotState == SS_PLAY )
			{
				if ( pSlotData->m_bMySlot == true  )
				{
					return pSlotData;
				}
			}
		}
	}

	return NULL;
}


void CX2Room::Set_KRoomInfo( const KRoomInfo& kRoomInfo_ )
{
	m_RoomType		= (CX2Room::ROOM_TYPE)kRoomInfo_.m_RoomType;
	m_RoomUID		= kRoomInfo_.m_RoomUID;
	m_RoomName		= kRoomInfo_.m_RoomName;
	m_RoomState		= (CX2Room::ROOM_STATE)kRoomInfo_.m_RoomState;
	m_bPublic		= kRoomInfo_.m_bPublic;
	m_RelayServerIP		= kRoomInfo_.m_wstrUDPRelayIP;
	m_RelayServerPort	= kRoomInfo_.m_usUDPRelayPort;
}

void CX2Room::Set_KRoomSlotInfo( const KRoomSlotInfo& kRoomSlotInfo_ )
{
	SlotData* pSlotData 
		= ( kRoomSlotInfo_.m_kRoomUserInfo.m_bIsObserver == true ? 
		GetObserverSlotData( kRoomSlotInfo_.m_Index ) : GetSlotData( kRoomSlotInfo_.m_Index ) );
	
	ASSERT( NULL != pSlotData );
	if( NULL != pSlotData )
	{
		if ( pSlotData->Set_KRoomSlotInfo( kRoomSlotInfo_ ) )
				CheckMakeUnitViewer( pSlotData );
	}
}

void CX2Room::Set_KRoomSlotInfoList( const std::vector<KRoomSlotInfo>& vecSlot_ )
{
	const int iAddSlot = static_cast<int>( vecSlot_.size() - m_SlotDataList.size() );

	// ���ο� ������ ũ�Ⱑ �� ũ�� ����� �ø�
	// �̺κ��� MatcingMySlotData() ������ ���� �Ǿ�� ��
	if ( iAddSlot > 0 )
	{
		for ( int i = 0; i < iAddSlot; ++i )
		{
			SlotData* pSlotData = new SlotData();
			m_SlotDataList.push_back( pSlotData );
		}
	}

#ifdef FIX_OBSERVER_MODE_CRASH		/// �������� ����
	if (NULL != GetMySlot() && false == GetMySlot()->m_bObserver )
		MatchingMySlotData( vecSlot_ );
#endif FIX_OBSERVER_MODE_CRASH

	for ( UINT uiIndex = 0; uiIndex < m_SlotDataList.size(); ++uiIndex )
	{
		SlotData* pSlotData	= m_SlotDataList[uiIndex];
		if ( uiIndex < vecSlot_.size() )
		{
			const KRoomSlotInfo& kRoomSlotInfo = vecSlot_[uiIndex];
			if ( pSlotData->Set_KRoomSlotInfo( kRoomSlotInfo ) )
				CheckMakeUnitViewer( pSlotData );	
		}
		else // ������ ������ �̻��̸�
		{
			pSlotData->Set_KRoomSlotInfoInCloseOrEmptyRoomstate();	// �󽽷����� ó��
			SAFE_DELETE( pSlotData );
		}
	}

	// ���ο� ������ ũ�Ⱑ ������ ����� ����
	if ( vecSlot_.size() < m_SlotDataList.size() )
		m_SlotDataList.resize( vecSlot_.size() );
	
	
// 	int addSlot = (int)(vecSlot.size() - m_SlotDataList.size());
// 	if( addSlot > 0 )
// 	{
// 		for( int i = 0; i < addSlot; i++ )
// 		{
// 			SlotData* pSlotData = new SlotData();
// 			m_SlotDataList.push_back( pSlotData );
// 		}
// 	}
// 
// 	for( int i = 0; i < (int)vecSlot.size(); i++ )
// 	{
// 		const KRoomSlotInfo* pKRoomSlotInfo	= &vecSlot[i];
// 		SlotData* pSlotData				= m_SlotDataList[i];
// 
// 		pSlotData->Set_KRoomSlotInfo( *pKRoomSlotInfo );
// 	}
}

void CX2Room::Set_KRoomObserverSlotInfoList( std::vector<KRoomSlotInfo>& vecObserverSlot_ )
{
	int addSlot = (int)(vecObserverSlot_.size() - m_ObserverSlotDataList.size());
	if( addSlot > 0 )
	{
		for( int i = 0; i < addSlot; i++ )
		{
			SlotData* pSlotData = new SlotData();
			m_ObserverSlotDataList.push_back( pSlotData );
		}
	}

	for( int i = 0; i < (int)vecObserverSlot_.size(); i++ )
	{
		KRoomSlotInfo* pKRoomSlotInfo	= &vecObserverSlot_[i];
		SlotData* pSlotData				= m_ObserverSlotDataList[i];

		pSlotData->Set_KRoomSlotInfo( *pKRoomSlotInfo );
		// �������� CheckMakeUnitViewer�� ���� ����
	}
}

#ifdef SERV_PVP_NEW_SYSTEM
void CX2Room::Set_KRoomSlotInfoNpc( std::map< UidType, KPvpNpcInfo > &mapNpcInfo )
{
	if( mapNpcInfo.empty() == true )
		return;

	for(UINT i=0; i<m_SlotDataList.size(); ++i)
	{
		SlotData *pSlotData = m_SlotDataList[i];
		if( pSlotData != NULL && pSlotData->m_bNpc == true )
		{
			std::map< UidType, KPvpNpcInfo >::iterator iter = mapNpcInfo.find( pSlotData->m_iNpcUid );
			if( iter != mapNpcInfo.end() )
			{
				KPvpNpcInfo kNpcInfo = (KPvpNpcInfo)iter->second;
				pSlotData->m_iNpcId = kNpcInfo.m_iNpcID;
			}
		}
	}
}

void CX2Room::DeleteNpcSlot()
{
	for(UINT i=0; i<m_SlotDataList.size(); ++i)
	{
		SlotData *pSlotData = m_SlotDataList[i];
		if(pSlotData->m_bNpc == true )
		{
			RoomNpcSlot roomNpcSlot;
			roomNpcSlot.m_iNpcId = pSlotData->m_iNpcId;
			roomNpcSlot.m_iNpcUid = pSlotData->m_iNpcUid;			
			roomNpcSlot.m_iRating = pSlotData->m_iNpcRating;
#ifdef PVP_SEASON2
			roomNpcSlot.m_cRank = pSlotData->m_cRank;
#endif
			roomNpcSlot.m_wstrNpcName = pSlotData->m_pUnit->GetUnitData().m_NickName;
			roomNpcSlot.m_iLevel = pSlotData->m_pUnit->GetUnitData().m_Level;

			// ���� NPC Stat�� RoomSlot�� ����� Stat������ ����Ѵ�.
			roomNpcSlot.m_fBaseHP		= pSlotData->m_pUnit->GetUnitData().m_GameStat.m_fBaseHP;
			roomNpcSlot.m_fAtkPhysic	= pSlotData->m_pUnit->GetUnitData().m_GameStat.m_fAtkPhysic;
			roomNpcSlot.m_fAtkMagic		= pSlotData->m_pUnit->GetUnitData().m_GameStat.m_fAtkMagic;
			roomNpcSlot.m_fDefPhysic	= pSlotData->m_pUnit->GetUnitData().m_GameStat.m_fDefPhysic;
			roomNpcSlot.m_fDefMagic		= pSlotData->m_pUnit->GetUnitData().m_GameStat.m_fDefMagic;

			m_vecNpcSlot.push_back( roomNpcSlot );
			SAFE_DELETE( pSlotData );
			m_SlotDataList.erase( m_SlotDataList.begin() + i );
			--i;
		}
	}

	SetNumOfPlayers( GetUserNum() );
}

CX2Room::RoomNpcSlot* CX2Room::GetNpcSlotData(UidType iUid)
{
	for(UINT i=0; i<m_vecNpcSlot.size(); ++i)
	{
		if( iUid == m_vecNpcSlot[i].m_iNpcUid )
			return &m_vecNpcSlot[i];
	}

	return NULL;
}

CX2Room::RoomNpcSlot* CX2Room::GetNpcSlotDataByIndex( int iIndex_ )
{
	if( static_cast<int>(m_vecNpcSlot.size()) > iIndex_ ) 
	{
		return &m_vecNpcSlot[iIndex_];
	}
	return NULL;
}

bool CX2Room::IsNpcSlot(UidType iNpcUid)
{
	for(UINT i=0; i<m_vecNpcSlot.size(); ++i)
	{
		if( iNpcUid == m_vecNpcSlot[i].m_iNpcUid )
		{
			return true;
		}
	}

	return false;
}
#endif







bool CX2Room::Handler_EGS_JOIN_ROOM_NOT( KEGS_JOIN_ROOM_NOT& kEGS_JOIN_ROOM_NOT )
{
	if( kEGS_JOIN_ROOM_NOT.m_JoinSlot.m_kRoomUserInfo.m_nUnitUID == GetMySlot()->m_UnitUID )
		return false;

	Set_KRoomSlotInfo( kEGS_JOIN_ROOM_NOT.m_JoinSlot );


	SlotData* pSlotData = GetSlotData( kEGS_JOIN_ROOM_NOT.m_JoinSlot.m_Index );
	if( pSlotData != NULL )
	{
		pSlotData->m_fElapsedTimeAfterJoinRoom = 0.f;
	}
	return true;
}

bool CX2Room::Handler_EGS_LEAVE_ROOM_REQ( int leaveRoomReason /* = NetError::NOT_LEAVE_ROOM_REASON_00 */ )
{
#ifdef DISABLE_REDUDANT_PACKET_TEST
	if( true == g_pMain->IsWaitingServerPacket( EGS_LEAVE_ROOM_ACK ) )
	{
		return false;
	}
#endif DISABLE_REDUDANT_PACKET_TEST


	KEGS_LEAVE_ROOM_REQ kPacket;
	kPacket.m_iReason = leaveRoomReason;

#ifdef	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	// �ʿ� ������ �ִ� GamePlayNetworkInfo �� vector�� ��´�.
	MoveGamePlayNetworkInfoFromMapToVector( kPacket.m_kGamePlayNetworkInfo.m_vecGamePlayNetworkInfo );
#endif	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO

	g_pData->GetServerProtocol()->SendPacket( EGS_LEAVE_ROOM_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_LEAVE_ROOM_ACK );
	return true;
}

bool CX2Room::Handler_EGS_LEAVE_ROOM_ACK( KEGS_LEAVE_ROOM_ACK& kEGS_LEAVE_ROOM_ACK )
{
	g_pMain->DeleteServerPacket( EGS_LEAVE_ROOM_ACK );

	return g_pMain->IsValidPacket( kEGS_LEAVE_ROOM_ACK.m_iOK );
	
}

bool CX2Room::Handler_EGS_LEAVE_ROOM_NOT( KEGS_LEAVE_ROOM_NOT& kEGS_LEAVE_ROOM_NOT )
{
	if ( kEGS_LEAVE_ROOM_NOT.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_04 )
	{
		CX2Room::SlotData* pSlotData = GetSlotDataByUnitUID( kEGS_LEAVE_ROOM_NOT.m_iUnitUID );
		if ( pSlotData != NULL && pSlotData->m_pUnit != NULL )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_REPLACED_STRING( ( STR_ID_463, "S", pSlotData->m_pUnit->GetNickName() ) ), g_pMain->GetNowState() );
		}
	}

	SetRoomState( (ROOM_STATE)kEGS_LEAVE_ROOM_NOT.m_cRoomState );
	Set_KRoomSlotInfoList( kEGS_LEAVE_ROOM_NOT.m_vecSlot );
	Set_KRoomObserverSlotInfoList( kEGS_LEAVE_ROOM_NOT.m_vecObserverSlot );
	return true;
}

bool CX2Room::Handler_EGS_LEAVE_GAME_REQ()
{
	g_pData->GetServerProtocol()->SendID( EGS_LEAVE_GAME_REQ );
	g_pMain->AddServerPacket( EGS_LEAVE_GAME_ACK );
	return true;
}

bool CX2Room::Handler_EGS_LEAVE_GAME_ACK( KEGS_LEAVE_GAME_ACK& kEGS_LEAVE_GAME_ACK )
{
	if( g_pMain->DeleteServerPacket( EGS_LEAVE_GAME_ACK ) == true )
	{
		return g_pMain->IsValidPacket( kEGS_LEAVE_GAME_ACK.m_iOK );
	}
	return false;
}

bool CX2Room::Handler_EGS_LEAVE_GAME_NOT( KEGS_LEAVE_GAME_NOT& kEGS_LEAVE_GAME_NOT )
{
	SetRoomState( (ROOM_STATE)kEGS_LEAVE_GAME_NOT.m_cRoomState );
	Set_KRoomSlotInfoList( kEGS_LEAVE_GAME_NOT.m_vecSlot );
	Set_KRoomObserverSlotInfoList( kEGS_LEAVE_GAME_NOT.m_vecObserverSlot );
	return true;
}

bool CX2Room::Handler_EGS_CHANGE_TEAM_REQ( CX2Room::TEAM_NUM teamNum )
{
	CX2PVPRoom::SlotData* pSlotData = GetMySlot();

	if( pSlotData == NULL )
		return false;

	if ( pSlotData->m_TeamNum == teamNum )
		return false;

	if ( pSlotData->m_bObserver == true )
		return false;

	KEGS_CHANGE_TEAM_REQ kEGS_CHANGE_TEAM_REQ;
	kEGS_CHANGE_TEAM_REQ.m_TeamNum = teamNum;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_TEAM_REQ, kEGS_CHANGE_TEAM_REQ );
	g_pMain->AddServerPacket( EGS_CHANGE_TEAM_ACK );

	return true;
}

bool CX2Room::Handler_EGS_CHANGE_TEAM_ACK( KEGS_CHANGE_TEAM_ACK& kEGS_CHANGE_TEAM_ACK )
{
	if( g_pMain->DeleteServerPacket( EGS_CHANGE_TEAM_ACK ) == true )
	{
		return g_pMain->IsValidPacket( kEGS_CHANGE_TEAM_ACK.m_iOK );
	}
	return false;
}

bool CX2Room::Handler_EGS_CHANGE_TEAM_NOT( KEGS_CHANGE_TEAM_NOT& kEGS_CHANGE_TEAM_NOT )
{
	CX2PVPRoom::SlotData* pSlotDataMy	= GetSlotDataByUnitUID( kEGS_CHANGE_TEAM_NOT.m_UnitUID );
	CX2PVPRoom::SlotData* pSlotData		= GetSlotData( kEGS_CHANGE_TEAM_NOT.m_SwapSlotIndex );
	if( pSlotDataMy == NULL || pSlotData == NULL )
		return false;

	pSlotDataMy->m_TeamNum					= kEGS_CHANGE_TEAM_NOT.m_TeamNum;
	m_SlotDataList[pSlotDataMy->m_Index]	= pSlotData;
	m_SlotDataList[pSlotData->m_Index]		= pSlotDataMy;

	pSlotData->m_Index		= pSlotDataMy->m_Index;
	pSlotDataMy->m_Index	= kEGS_CHANGE_TEAM_NOT.m_SwapSlotIndex;

	pSlotData->m_UnitUID	= 0;
	pSlotData->m_SlotState	= SS_EMPTY;

	return true;
}

bool CX2Room::Handler_EGS_CHANGE_READY_REQ()
{

#ifdef DISABLE_REDUDANT_PACKET_TEST
	if( true == g_pMain->IsWaitingServerPacket( EGS_CHANGE_READY_ACK ) )
	{
		return false;
	}
#endif DISABLE_REDUDANT_PACKET_TEST


	CX2PVPRoom::SlotData* pSlotData = GetMySlot();

	if( pSlotData == NULL )
		return false;

	KEGS_CHANGE_READY_REQ kEGS_CHANGE_READY_REQ;
	kEGS_CHANGE_READY_REQ.m_bReady = !pSlotData->m_bReady;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_READY_REQ, kEGS_CHANGE_READY_REQ );
	g_pMain->AddServerPacket( EGS_CHANGE_READY_ACK, 5.f );

	return true;
}

bool CX2Room::Handler_EGS_CHANGE_READY_ACK( KEGS_CHANGE_READY_ACK& kEGS_CHANGE_READY_ACK )
{
	if( g_pMain->DeleteServerPacket( EGS_CHANGE_READY_ACK ) == true )
	{
		return g_pMain->IsValidPacket( kEGS_CHANGE_READY_ACK.m_iOK );
	}
	return false;
}

bool CX2Room::Handler_EGS_CHANGE_READY_NOT( KEGS_CHANGE_READY_NOT& kEGS_CHANGE_READY_NOT )
{
	CX2PVPRoom::SlotData* pSlotData = GetSlotDataByUnitUID( kEGS_CHANGE_READY_NOT.m_UnitUID );
	if( pSlotData == NULL )
		return false;

	pSlotData->m_bReady = kEGS_CHANGE_READY_NOT.m_bReady;
	return true;
}


bool CX2Room::Handler_EGS_CHANGE_PITIN_REQ()
{
	CX2PVPRoom::SlotData* pSlotData = GetMySlot();

	if( pSlotData == NULL )
		return false;

	KEGS_CHANGE_PITIN_REQ kEGS_CHANGE_PITIN_REQ;
	kEGS_CHANGE_PITIN_REQ.m_bPitIn = !pSlotData->m_bPitIn;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_PITIN_REQ, kEGS_CHANGE_PITIN_REQ );
	g_pMain->AddServerPacket( EGS_CHANGE_PITIN_ACK );

	m_bRecievePitInNot = false;

	return true;
}
bool CX2Room::Handler_EGS_CHANGE_PITIN_ACK( KEGS_CHANGE_PITIN_ACK& kEGS_CHANGE_PITIN_ACK )
{
	if( g_pMain->DeleteServerPacket( EGS_CHANGE_PITIN_ACK ) == true )
	{
		return g_pMain->IsValidPacket( kEGS_CHANGE_PITIN_ACK.m_iOK );
	}
	return false;
}
bool CX2Room::Handler_EGS_CHANGE_PITIN_NOT( KEGS_CHANGE_PITIN_NOT& kEGS_CHANGE_PITIN_NOT )
{
	CX2PVPRoom::SlotData* pSlotData = GetSlotDataByUnitUID( kEGS_CHANGE_PITIN_NOT.m_UnitUID );
	if( pSlotData == NULL )
		return false;

	pSlotData->m_bPitIn = kEGS_CHANGE_PITIN_NOT.m_bPitIn;

	CX2Room::SlotData* pMySlotData = GetMySlot();
	if ( pMySlotData->m_pUnit != NULL && pMySlotData->m_pUnit->GetUID() == kEGS_CHANGE_PITIN_NOT.m_UnitUID )
	{
		m_bRecievePitInNot = true;
	}

	return true;
}


bool CX2Room::Handler_EGS_CHANGE_SLOT_OPEN_REQ( int slotIndex )
{

#ifdef DISABLE_REDUDANT_PACKET_TEST
	if( true == g_pMain->IsWaitingServerPacket( EGS_CHANGE_SLOT_OPEN_ACK ) )
	{
		return false;
	}
#endif DISABLE_REDUDANT_PACKET_TEST


	SlotData* pMySlotData = GetMySlot();

	if( pMySlotData == NULL )
		return false;

	if ( RT_PVP == m_RoomType )
	{
		CX2PVPRoom* pPVPRoom = (CX2PVPRoom*)this;

		if( pPVPRoom->IsRoomMaster( pMySlotData ) == false )
			return false;
	}
	else
	{
		if ( pMySlotData->m_bHost == false )
			return false;
	}

	

	SlotData* pSlotData = GetSlotData( slotIndex );
	if( pSlotData == NULL )
		return false;

	if( pSlotData->m_SlotState != SS_EMPTY && pSlotData->m_SlotState != SS_CLOSE )
		return false;

	KEGS_CHANGE_SLOT_OPEN_REQ kEGS_CHANGE_SLOT_OPEN_REQ;
	kEGS_CHANGE_SLOT_OPEN_REQ.m_SlotIndex = slotIndex;
	if( pSlotData->m_SlotState == SS_EMPTY )
		kEGS_CHANGE_SLOT_OPEN_REQ.m_SlotState = SS_CLOSE;
	if( pSlotData->m_SlotState == SS_CLOSE )
		kEGS_CHANGE_SLOT_OPEN_REQ.m_SlotState = SS_EMPTY;

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_SLOT_OPEN_REQ, kEGS_CHANGE_SLOT_OPEN_REQ );
	g_pMain->AddServerPacket( EGS_CHANGE_SLOT_OPEN_ACK );

	return true;
}

bool CX2Room::Handler_EGS_CHANGE_SLOT_OPEN_ACK( KEGS_CHANGE_SLOT_OPEN_ACK& kEGS_CHANGE_SLOT_OPEN_ACK )
{
	if( g_pMain->DeleteServerPacket( EGS_CHANGE_SLOT_OPEN_ACK ) == true )
	{
		return g_pMain->IsValidPacket( kEGS_CHANGE_SLOT_OPEN_ACK.m_iOK );
	}
	return false;
}

bool CX2Room::Handler_EGS_CHANGE_SLOT_OPEN_NOT( KEGS_CHANGE_SLOT_OPEN_NOT& kEGS_CHANGE_SLOT_OPEN_NOT )
{
	SlotData* pSlotData = GetSlotData( kEGS_CHANGE_SLOT_OPEN_NOT.m_SlotIndex );

	if( pSlotData == NULL )
		return false;

	pSlotData->m_SlotState = (SLOT_STATE)kEGS_CHANGE_SLOT_OPEN_NOT.m_SlotState;
	return true;
}




bool CX2Room::Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( KEGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT& kEGS_CHANGE_EQUIPPED_ITEM_NOT )
{
	CX2Room::SlotData* pSlotData = GetSlotDataByUnitUID( kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_UnitUID );
	if( pSlotData == NULL || pSlotData->m_pUnit == NULL || pSlotData->m_pUnitViewer == NULL )
		return false;


	ASSERT( pSlotData->m_pUnit->GetUID() == kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_UnitUID );
	if( pSlotData->m_pUnit->GetUID() != kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_UnitUID )
		return false;

//#ifdef TITLE_SYSTEM
    if(kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_vecInventorySlotInfo.size() == 0)
    {
#ifdef SERV_TITLE_DATA_SIZE
		pSlotData->m_pUnit->SetTitleId(kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_iEquippedTitleID);
		pSlotData->m_pUnitViewer->GetUnit()->SetTitleId(kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_iEquippedTitleID);
#else
        pSlotData->m_pUnit->SetTitleId(kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_sEquippedTitleID);
        pSlotData->m_pUnitViewer->GetUnit()->SetTitleId(kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_sEquippedTitleID);
#endif
        pSlotData->m_pUnitViewer->UpdateTitle();

        return true;
    }
//#endif




	pSlotData->m_pUnit->AccessUnitData().m_GameStat.SetKStat( kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_kGameStat );



	// �� �����̸�
	if ( g_pData->GetMyUser()->GetSelectUnit()->GetUID() == kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_UnitUID )
	{
#ifdef FIX_WRONG_EQUIP
		// EGS_CHANGE_INVENTORY_SLOT_ACK ��Ŷ�� �����ϱ� ���� �� ��Ŷ�� �����ϸ�
		// ���ŵ��� ���� �� �κ��丮�� �̿��Ͽ� �����ϱ� ������ ������Ʈ�� �� ��
		// �׷��Ƿ� ���⼭ ó������ �ʰ� EGS_CHANGE_INVENTORY_SLOT_ACK ���� ó���ϵ��� ����
#else FIX_WRONG_EQUIP
		if ( pSlotData->m_pUnitViewer != NULL )
			pSlotData->m_pUnitViewer->UpdateEqip( true );
#endif FIX_WRONG_EQUIP

		return true;
	}



	// �ٸ� ����� �����̸�
	CX2UnitViewerUI* pUnitViewerUIWatch = NULL; 
	if( true ==  g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_WATCH) &&
		g_pData->GetUIManager()->GetUIWatch()->GetTargetUnitUID() == kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_UnitUID )
	{
		pUnitViewerUIWatch = g_pData->GetUIManager()->GetUIWatch()->GetUnitViewerUI();
	}

	StateLog( pSlotData->m_pUnit->GetNickName() );

	for( int i = 0; i < (int)kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_vecInventorySlotInfo.size(); i++ )
	{
		KInventoryItemInfo& kInventorySlotInfo = kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_vecInventorySlotInfo[i];

		if( kInventorySlotInfo.m_cSlotCategory == CX2Inventory::ST_E_EQUIP || 
			kInventorySlotInfo.m_cSlotCategory == CX2Inventory::ST_EQUIP ||
			kInventorySlotInfo.m_cSlotCategory == CX2Inventory::ST_ACCESSORY ||
			kInventorySlotInfo.m_cSlotCategory == CX2Inventory::ST_AVARTA )
		{
			CX2Item* pItem = pSlotData->m_pUnit->GetInventory().GetItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID );
			if( pItem != NULL )
			{
				wstring wstrLog = L"RemoveItemName: ";
				wstrLog += pItem->GetFullName();
				StateLog( wstrLog.c_str() );

				pSlotData->m_pUnit->RemoveEqip( pItem->GetUID() );
				pSlotData->m_pUnitViewer->RemoveEqip( pItem->GetUID() );

				if( NULL != pUnitViewerUIWatch )
				{
					pUnitViewerUIWatch->RemoveEqip( pItem->GetUID() );
				}
			}
		}
	}

	for( int i = 0; i < (int)kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_vecInventorySlotInfo.size(); i++ )
	{
		KInventoryItemInfo& kInventorySlotInfo = kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_vecInventorySlotInfo[i];
		pSlotData->m_pUnit->AccessInventory().RemoveItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID );
	}

	for( int i = 0; i < (int)kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_vecInventorySlotInfo.size(); i++ )
	{
		KInventoryItemInfo& kInventorySlotInfo = kEGS_CHANGE_EQUIPPED_ITEM_NOT.m_vecInventorySlotInfo[i];
		if( kInventorySlotInfo.m_iItemUID <= 0 )
			continue;

        {
		    CX2Item::ItemData kItemData( kInventorySlotInfo );
		    pSlotData->m_pUnit->AccessInventory().AddItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID, kItemData );
        }

		if( kInventorySlotInfo.m_cSlotCategory == CX2Inventory::ST_E_EQUIP )
		{
			CX2Item* pItem = pSlotData->m_pUnit->GetInventory().GetItem( (CX2Inventory::SORT_TYPE)kInventorySlotInfo.m_cSlotCategory, kInventorySlotInfo.m_sSlotID );
			if( pItem != NULL )
			{
				if( true == pSlotData->m_pUnit->AddEqip( pItem->GetUID() ) )
				{
					wstring wstrLog = L"AddItemName: ";
					wstrLog += pItem->GetFullName();
					StateLog( wstrLog.c_str() );

					pSlotData->m_pUnitViewer->AddEqip( pItem->GetUID() );

					if( NULL != pUnitViewerUIWatch )
					{
						pUnitViewerUIWatch->AddEqip( pItem->GetUID() );
					}
				}
			}
		}
	}



	if( pUnitViewerUIWatch != NULL )
	{
		g_pData->GetUIManager()->GetUIWatch()->ResetEquipUI();

		// note!! ���ĳ����������â���� Īȣ�� �ٲ���� �� �������ִ� �κ� �۾��ؾ���
	}



	return true;
}


bool CX2Room::Handler_EGS_BAN_USER_REQ( UidType unitUID )
{
	KEGS_BAN_USER_REQ kPacket;
	kPacket.m_iUnitUID = unitUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_BAN_USER_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_BAN_USER_ACK );

	return true;
}

bool CX2Room::Handler_EGS_BAN_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KPacketOK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_BAN_USER_ACK ) == true )
	{
		return g_pMain->IsValidPacket( kEvent.m_iOK );
	}

	return false;
}

//{{ oasis907 : ����� [2009.11.19] //
// ���� �Ϸ� (6. �����߿� ��ų ������ ����Ǿ������ ����� �����鿡�� ����� ��ų ������ ��ε�ĳ������)
bool CX2Room::Handler_EGS_CHANGE_SKILL_INFO_IN_ROOM_NOT( KEGS_CHANGE_SKILL_INFO_IN_ROOM_NOT& kEvent_ )
{
	if(g_pX2Game == NULL)
		return false;

// 	CX2Room::SlotData* pSlotData = GetSlotDataByUnitUID( kEvent_.m_iUnitUID );
// 	if( pSlotData == NULL || pSlotData->m_pUnit == NULL)
// 		return false;
	
	//{{ oasis907 : ����� [2009.12.10] //
	CX2GUUser *pUser = g_pX2Game->GetUserUnitByUID( kEvent_.m_iUnitUID );

	if( pUser == NULL )
		return false;

	pUser->SetGameStat( kEvent_.m_kGameStat );
	if ( NULL == pUser->GetUnit() )
		return false;

	CX2Unit::UnitData& kUnitData = pUser->GetUnit()->AccessUnitData();//pSlotData->m_pUnit->GetUnitData();

	kUnitData.m_GameStat.SetKStat( kEvent_.m_kGameStat );
	kUnitData.m_byMemberShipGrade = kEvent_.m_ucMemberShipGrade;
	CX2UserSkillTree& userSkillTree = kUnitData.m_UserSkillTree;
	
	// �� �����̸� - ��ų ���� NOT���� ��� ��ų ������ �̹� ó���� ����
	if ( g_pData->GetMyUser()->GetSelectUnit()->GetUID() != kEvent_.m_iUnitUID )
	{
		userSkillTree.ConstructGuildSkillTreeNotMyUnit( kEvent_.m_UnitSkillData.m_vecGuildPassiveSkill );
		userSkillTree.ConstructSkillTreeNotMyUnit( kEvent_.m_UnitSkillData.m_vecPassiveSkill, kEvent_.m_UnitSkillData.m_aEquippedSkill, kEvent_.m_UnitSkillData.m_aEquippedSkillSlotB );

		for ( int i = 0; i < EQUIPPED_SKILL_SLOT_COUNT; ++i )
		{
			const KSkillData& skillDataInSlotA = kEvent_.m_UnitSkillData.m_aEquippedSkill[i];
			// Slot A
			if ( userSkillTree.IsChangedSkillSlot( i, false, skillDataInSlotA ) )
			{
				userSkillTree.SetEquippedSkill( i, false, skillDataInSlotA );
				pUser->ChangeEquippedSkillState( kUnitData, 
					static_cast<CX2SkillTree::SKILL_ID>( skillDataInSlotA.m_iSkillID ), 
					i, false );
			}
			const KSkillData& skillDataInSlotB = kEvent_.m_UnitSkillData.m_aEquippedSkillSlotB[i];
			// Slot B
			if ( userSkillTree.IsChangedSkillSlot( i, true, skillDataInSlotB ) )
			{
				userSkillTree.SetEquippedSkill( i, true, skillDataInSlotB );
				pUser->ChangeEquippedSkillState( kUnitData, 
					static_cast<CX2SkillTree::SKILL_ID>( skillDataInSlotB.m_iSkillID ),
					i, true );
			}
		}
	}

	pUser->UpdateSocketDataAndEnchantData();
	pUser->UpdatePassiveAndActiveSkillState();
	

	return true;
}
//}} oasis907 : ����� [2009.11.19] //


void CX2Room::PlayMotionByID( ROOM_MOTION motionID, CX2UnitViewerUI* pUnitViewerUI )
{
	if ( pUnitViewerUI == NULL )
		return;

	switch( motionID )
	{
	case RM_WAIT1:
		//motionName = L"LobbyWait1";
		pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT1 );
		break;

	case RM_WAIT2:
		//motionName = L"LobbyWait2";
		pUnitViewerUI->PlayByMotionType( CX2UnitViewerUI::UVVMT_WAIT2 );
		break;
	}
}

int	CX2Room::GetActiveObserverSlotNum()
{
	int activeObserverNum = 0;
	for( int i = 0; i <  GetObserverSlotNum(); i++ )
	{
		SlotData* pSlotData = GetObserverSlotData( i );

		if( pSlotData != NULL 
			&& pSlotData->m_SlotState != CX2Room::SS_CLOSE
			&& pSlotData->m_SlotState != CX2Room::SS_EMPTY )
		{
			activeObserverNum++;
		}
	}

	return activeObserverNum;
}

#ifdef	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
/** @function : InsertNewPeerToGamePlayNetworkInfo
	@brief : ���ο� Peer ������ GamePlayNetworInfo �ʿ� insert �ϴ� �Լ�
	@param : ���ο� ���� ����Ÿ(pSlotData_)
*/
void CX2Room::InsertGamePlayNetworkInfo( const SlotData* pSlotData_ )
{
	KGamePlayNetworkInfo kGamePlayNetworkInfo;
	kGamePlayNetworkInfo.m_iOpponentUnitUID = pSlotData_->m_UnitUID;
	
	/// ������ �����ϴ� ��쿡�� �׳� false Piar�� ���� �� ����
	m_mapGamePlayNetworkInfo.insert( make_pair(pSlotData_->m_UnitUID, kGamePlayNetworkInfo) );	
}

/** @function : UpdateGamePlayNetworkInfo
	@brief : ������ �Ǵ� p2p �ð��� ����
	@param : Peer ����
*/
void CX2Room::UpdateGamePlayNetworkInfo( const CKTDNUDP::Peer* pPeer_ )
{
	/// map �ȿ� peer ������ �ְ�, p2p �Ǵ� ������ ������ �����Ǿ��ٸ�
	if ( !m_mapGamePlayNetworkInfo.empty() 
		&& ( pPeer_->GetP2PConnected() || pPeer_->GetUseRelay() ) )
	{
		map<UidType, KGamePlayNetworkInfo>::iterator mItr
			= m_mapGamePlayNetworkInfo.find( pPeer_->m_UID );
		if ( mItr != m_mapGamePlayNetworkInfo.end() )
		{
			/// p2p�� �α� ���� ��� Ȧ��Ī�� �õ��ߴ°�
			mItr->second.m_iHolePunchingTryCount = pPeer_->m_ConnectTestCount;
			const float& fPlayElapsedTime = g_pKTDXApp->GetElapsedTime();
			
			if ( pPeer_->GetUseRelay() )	/// �����̸� ����ϸ�
				mItr->second.m_fRelayPlayTime += fPlayElapsedTime;
			else
				mItr->second.m_fP2PPlayTime += fPlayElapsedTime;
		}
	}
}

/** @function : MoveGamePlayNetworkInfoFromMapToVector
	@brief : �ʿ� �ִ� GamePlayNetworkInfo�� ���ڷ� ���޵� vector�� insert��
	@param : insert �� vector
*/
void CX2Room::MoveGamePlayNetworkInfoFromMapToVector( OUT vector<KGamePlayNetworkInfo>& vecGamePlayNetworkInfo_ )
{
	map<UidType,KGamePlayNetworkInfo>::const_iterator mItr 
		= m_mapGamePlayNetworkInfo.begin();

	while ( m_mapGamePlayNetworkInfo.end() != mItr )
	{
		vecGamePlayNetworkInfo_.push_back( mItr->second );
		++mItr;
	}
}

#endif	SERV_COLLECTION_OF_RELAY_AND_P2P_INFO

void CX2Room::MatchingMySlotData( const std::vector<KRoomSlotInfo>& vecSlot_ )
{
	const UidType UserUidOfMine = g_pData->GetMyUser()->GetUID();

	// vecSlot���� ���� �ڽŰ��� ã�� �Լ�
	auto FunctionFindMySlotInfo = [UserUidOfMine]( const KRoomSlotInfo& kRoomSlotInfo_ ) {
		return ( kRoomSlotInfo_.m_kRoomUserInfo.m_iOwnerUserUID == UserUidOfMine );			
	};

	std::vector<KRoomSlotInfo>::const_iterator cvItrMySlotInfo 
		= find_if( vecSlot_.begin(), vecSlot_.end(), FunctionFindMySlotInfo );

	ASSERT( cvItrMySlotInfo != vecSlot_.end() );
	if ( cvItrMySlotInfo != vecSlot_.end() )
	{
		// ���ο� ���� ������ �ִ� �ڽ��� �����Ϳ� 
		// ������ ���� ������ �ִ� �ڽ��� �����Ͱ� ���� �ε����� ���� ������ ������ �������� �ε����� ����
		if ( false == m_SlotDataList[cvItrMySlotInfo->m_Index]->m_bMySlot )
		{
			for ( UINT uiIndex = 0; uiIndex < m_SlotDataList.size(); ++uiIndex )
			{
				// ������ ���� ������ �ڽ��� �����Ͱ� ������
				if ( m_SlotDataList[uiIndex]->m_UnitUID == cvItrMySlotInfo->m_kRoomUserInfo.m_nUnitUID )
				{
					const UINT uiMySlotInfoIndex = static_cast<UINT>( cvItrMySlotInfo->m_Index );
					ASSERT( uiMySlotInfoIndex < m_SlotDataList.size() );

					// ���ο� ���� ������ index ��ġ�� �����ϰ� �����.
					SlotData* pMySlotData = m_SlotDataList[uiIndex];

					m_SlotDataList[uiIndex] = m_SlotDataList[uiMySlotInfoIndex];
					m_SlotDataList[uiMySlotInfoIndex] = pMySlotData;
					
					m_SlotDataList[uiMySlotInfoIndex]->m_Index = static_cast<int>( uiIndex );
					m_SlotDataList[uiIndex]->m_Index = uiMySlotInfoIndex;

					if ( NULL != g_pX2Game )
						g_pX2Game->ChangeUserUnitIndex( uiIndex, uiMySlotInfoIndex );
					break;
				}
			}
		}
	}
}

void CX2Room::CheckMakeUnitViewer( SlotData* pSlotData_ )
{
	switch ( GetRoomType() )
	{
	case RT_DUNGEON:
	case RT_PVP:
	case RT_TRAININGCENTER:
		pSlotData_->CheckMakeUnitViewer();
		break;

	default:
		break;
	}			
}

//{{ kimhc // 2012-10-16 // ���� ���� ������ ȣ��Ʈ�� �����ϴ� �ڵ�
#ifdef	SERV_CHOOSE_FASTEST_HOST
/** @function : GetAverageTestPingTimeToOthers
	@brief : �ٸ� �÷��̾����� Ping �ð� ��� ���� ���ϴ� �Լ�
	@param : ���� ��հ�(& dwAveragePingTime_)
	@return : ��հ��� ���Դ��� ���η� ������ �־����� true, �������� false
*/
bool CX2Room::GetAverageTestPingTimeToOthers( OUT DWORD& dwAveragePingTime_ )
{
	UINT uiSumOfPlayerNumber = 0;	/// �÷��̾�� �Ǵܵ� ���� ��
	UINT uiSumOfPingTime = 0;		/// �÷��̾����� �� �ð�

	BOOST_FOREACH( const SlotData* pSlotData, m_SlotDataList )
	{
		if ( NULL != pSlotData )
		{
			/// ���� ���°� 
			switch ( pSlotData->GetSlotState() )
			{
			case SS_EMPTY:
			case SS_CLOSE:
				break;

			default:	/// ���� ���µ��� �ƴϸ� ���� ��
				{
					if ( !pSlotData->IsMyUnit() && false == pSlotData->m_bNpc )
					{
						/// pingTime �� �ʱ�ȭ �� ���� ���� ���� ���� TestPingReq�� ���� ������ ���� ���� ����
						if ( pSlotData->m_PingTime != PING_TIME_TO_BE_INITIALIZED )
						{
							++uiSumOfPlayerNumber;
							uiSumOfPingTime += pSlotData->m_PingTime;
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
                            if ( pSlotData->m_dwPingCount != 0 )
                                uiSumOfPingTime += (unsigned) ( pSlotData->m_fAvgPingStd * 1000 );
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
						}
						else	/// ���� TestPingReq�� ���� ������ ���� ���� ������ �ִٸ�
						{
							return false;
						}
					}					
				} break;
			}	/// switch		
		}	// if
	}	/// BOOST

	/// TestPingReq�� ���� ������ �� ������ �Ѹ��̶� �ִٸ�
	if ( 0 < uiSumOfPlayerNumber )
	{
		dwAveragePingTime_ = static_cast<DWORD>( uiSumOfPingTime / uiSumOfPlayerNumber );
		return true;
	}
	else	/// �Ѹ� ���ٸ�(RoomUserInfo�� �ڽŹۿ� ���ų�, TestPingReq�� ���� ������ ���� ���� ���)
		return false;
}

/** @function : Send_EGS_UDP_PING_SCORE_NOT
	@brief : ��� �� �ð��� ������ �����ϴ� �Լ�
	@param : ��� �� �ð�(dwAveragePingTime_)
*/
void CX2Room::Send_EGS_UDP_PING_SCORE_NOT( const DWORD dwAveragePingTime_ )
{
	KEGS_UDP_PING_SCORE_NOT kPacket;

	/// GameEdit���� ������ �׽�Ʈ �ΰ��� ������ �� ���� ���
#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	kPacket.m_bForceHost = m_bForceHostForGameEdit;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	kPacket.m_dwPingScore = ( 0 != GetAveragePingTimeForGameEdit() ) ? GetAveragePingTimeForGameEdit() : dwAveragePingTime_;
	
	g_pData->GetServerProtocol()->SendPacket( EGS_UDP_PING_SCORE_NOT, kPacket );
}

#endif	SERV_CHOOSE_FASTEST_HOST
//}} kimhc // 2012-10-16 // ���� ���� ������ ȣ��Ʈ�� �����ϴ� �ڵ�


#ifdef HEAP_BROKEN_BY_ROOM
/** @function : ApplyRoomPacketData
	@brief : static ����, RoomPacketData �� ���õ� �������� ����� ���� ��Ų��
*/
/*virtual*/ void CX2Room::ApplyRoomPacketData()
{
	RoomPacketData.m_bSetNewData = false;	/// ���� �ǹǷ� �� �����Ͱ� �ƴ�
	Set_KRoomInfo( RoomPacketData.m_RoomInfo );
	
	if ( !RoomPacketData.m_vecSlot.empty() )
		Set_KRoomSlotInfoList( RoomPacketData.m_vecSlot );

	if ( !RoomPacketData.m_vecObserverSlot.empty() )
		Set_KRoomObserverSlotInfoList( RoomPacketData.m_vecObserverSlot );

	ConnectRelayServer( RoomPacketData.m_RoomInfo.m_wstrUDPRelayIP.c_str(), 
		RoomPacketData.m_RoomInfo.m_usUDPRelayPort );
	SetCenterServerIP( RoomPacketData.m_wstrCNIP.c_str() );
		
}

/*static*/ void CX2Room::InitializeRoomPacketData()
{
	RoomPacketData.Initialize();
}

/*static*/ void CX2Room::SetRoomPacketData( const KRoomInfo& roomInfo_, const vector<KRoomSlotInfo>& vecSlot_, const vector<KRoomSlotInfo>& vecObserverSlot_, const wstring& wstrCNIP_ )
{
	RoomPacketData.m_bSetNewData = true;
	RoomPacketData.m_RoomInfo = roomInfo_;

	if ( !vecSlot_.empty() )
	{
		RoomPacketData.m_vecSlot.reserve( vecSlot_.size() );
		RoomPacketData.m_vecSlot = vecSlot_;
	}

	if ( !vecObserverSlot_.empty() )
	{
		RoomPacketData.m_vecObserverSlot.reserve( vecObserverSlot_.size() );
		RoomPacketData.m_vecObserverSlot = vecObserverSlot_;
	}

	RoomPacketData.m_wstrCNIP = wstrCNIP_;
}

/*static*/ void CX2Room::SetPvpNpcSlotInfo( const map<UidType, KPvpNpcInfo>& mapPvpNpcSlotInfo_ )
{
	if ( !mapPvpNpcSlotInfo_.empty() )
		RoomPacketData.m_mapPvpNpcInfo = mapPvpNpcSlotInfo_;
}

/*static*/ void CX2Room::SetOfficialPvp( const bool bOfficialPvp_ )
{
	RoomPacketData.m_bOfficialPvp = bOfficialPvp_;
}
#endif // HEAP_BROKEN_BY_ROOM

#ifdef SHOW_UDP_NETWORK_INFO_IN_ROOM
wstring CX2Room::GetStringNetworkInfo( const SlotData* pSlotData_ )
{
	if ( NULL != pSlotData_ )
	{
		const CKTDNUDP::Peer* pPeer = g_pData->GetGameUDP()->GetPeer( pSlotData_->m_UnitUID );

		if ( NULL != pPeer )
		{
			wstringstream wstrNetworkInfo;
			//{{ 2013. 1. 11	�ڼ���	Merge ����IP ���� ���н� ����IP�� �õ�( ������ )
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#ifdef  SERV_KTDX_RETRY_USING_INTERNAL_IP
			if( pPeer->GetUseRelay() == true )
			{
				wstrNetworkInfo << L" ������ ���� / ";
			}

			else if( pPeer->m_eP2PConnectType == CKTDNUDP::P2PCONNECT_INTERNAL )
			{
				wstrNetworkInfo << L" P2P ����( Internal IP ) / ";
			}
			else
			{
				wstrNetworkInfo << L" P2P ����( Public IP ) / ";
			}
//#else   SERV_KTDX_RETRY_USING_INTERNAL_IP
//			wstrNetworkInfo << ( pPeer->m_bUseRelay ? L" ������ ���� / " : L" P2P ���� / " );
//#endif  SERV_KTDX_RETRY_USING_INTERNAL_IP
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			wstrNetworkInfo << ( pPeer->GetUseRelay() ? L" ������ ���� / " : L" P2P ���� / " );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
			//}}
			wstrNetworkInfo << L"P2P �õ� Ƚ��: " << pPeer->m_ConnectTestCount;
			wstrNetworkInfo << L" / ������Ŷ ���� Ƚ��: " << pSlotData_->m_uiUserPacketCountToReceive;
			wstrNetworkInfo << L" / IP: " 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                << CKTDNUDP::ConvertAddressToIP( pPeer->m_IPAddress )
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//                << pPeer->m_IP 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                << L" / Port: " << pPeer->m_Port
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                << L" / AvgPing: " << (int)(pSlotData_->m_fAvgPingTime * 1000)
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
                << L"(" << (int)(pSlotData_->m_fAvgPingStd * 1000) << L")"
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				;
			return wstrNetworkInfo.str();
		}
		else
			return L"";
	}
	else
		return L"";
}
#endif // SHOW_UDP_NETWORK_INFO_IN_ROOM

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void CX2Room::SlotData::Free()
{
	//if( g_pMain != NULL )
		//CKTDXThread::CLocker locker( g_pMain->GetUnitLoader()->GetLock() );
/*
	if( m_bMySlot == false )
	{
		SAFE_DELETE( m_pUnit );
	}
*/
	if ( m_pUnit != NULL )
	{
		if ( g_pData->GetMyUser()->IsMyUnitByUnitUID( m_pUnit->GetUID() ) == false )
		{
#ifdef GUUSER_UNIT_POINTER_CRASH_FIX
			if(g_pX2Game != NULL)
			{
				CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID(m_pUnit->GetUID());
				if(pGUUser != NULL)
				{
					pGUUser->SetUnit(NULL);
					g_pX2Game->DeleteUserUnitByData(pGUUser);
				}
			}
#endif GUUSER_UNIT_POINTER_CRASH_FIX
			SAFE_DELETE( m_pUnit );
#ifdef TAG_MATCH_TEST
			SAFE_DELETE( m_pSubUnit );
#endif TAG_MATCH_TEST
		}
	}

	SAFE_DELETE_KTDGOBJECT( m_pUnitViewer );

	m_Index			= 0;
	m_SlotState		= SS_CLOSE;
	m_bHost			= false;
	m_bMySlot		= false;
	m_bReady		= false;
	m_bPitIn		= false;
	m_bTrade		= false;
	m_TeamNum		= TN_RED;
	m_UnitUID		= 0;
	m_PingTime		= PING_TIME_TO_BE_INITIALIZED;
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    ZeroMemory( m_adwPingSendTime, sizeof(m_adwPingSendTime) );
    m_uPingSendCount = 0;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
    m_fAvgPingTime = 0.f;
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    m_fAvgPingTimeSqr = 0.f;
    m_fAvgPingStd = 0.f;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    m_dwPingCount = 0;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

	m_bObserver		= false;

	m_pUnit			= NULL;
#ifdef TAG_MATCH_TEST
	m_pSubUnit		= NULL;
#endif TAG_MATCH_TEST


//#ifdef PVP_BOSS_COMBAT_TEST
//	m_bIsBoss		= false;
//#endif PVP_BOSS_COMBAT_TEST
//

#ifdef SHOW_UDP_NETWORK_INFO_IN_ROOM
	m_uiUserPacketCountToReceive = 0;		/// ���� ���� ��Ŷ ��
#endif // SHOW_UDP_NETWORK_INFO_IN_ROOM

}

bool CX2Room::SlotData::Set_KRoomSlotInfoOfMine( const KRoomSlotInfo& kRoomSlotInfo_ )
{
	bool bRemakeUnitViewer = false;
	m_bMySlot = true;

	if( NULL == m_pUnit )	// ������ �ƹ��͵� ���� �����̸�
	{
		// �� ������ ����
		m_pUnit = g_pData->GetMyUser()->GetUnitByUID( kRoomSlotInfo_.m_kRoomUserInfo.m_nUnitUID );

		// m_pUnit�� NULL �̾����� g_pX2Game�� ������, �ƴϸ� �־����� �� ������ ������� ��
		//ASSERT( NULL == g_pX2Game || NULL == g_pX2Game->GetUserUnitByUID( m_pUnit->GetUID() ) );

#ifdef GUUSER_UNIT_POINTER_CRASH_FIX
		if ( NULL != g_pX2Game ) // ���� ��ü�� ������ ������ü�� ���� ����
		{
			CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID( m_pUnit->GetUID() );
			if(pGUUser != NULL)
			{
				pGUUser->SetUnit( m_pUnit );
				//StateLog( L"m_pUnit�� NULL �̾��µ�, GUUser ��ü�� ����" );
			}
		}
#endif GUUSER_UNIT_POINTER_CRASH_FIX
	}
	else	// ������ ���� �ִ� �����̸�
	{
		// ���ο� ������ ���� �ε� ������ ������ �ٸ� ���� ������ �Ǿ� ������
		ASSERT( m_pUnit->GetUID() == kRoomSlotInfo_.m_kRoomUserInfo.m_nUnitUID );
		if ( m_pUnit->GetUID() != kRoomSlotInfo_.m_kRoomUserInfo.m_nUnitUID )
		{
			const UidType uidToBeDeleted = m_pUnit->GetUID();

			// note!! ���� �ȿ� �ִ� �ٸ� ������ slotdata�� ����� �Ǵ� ��찡 �ƴ��� Ȯ�� �ʿ�
			SAFE_DELETE( m_pUnit );
			StateLog( L"overwrite non-empty slotdata - my" );					

			// ���� ����
			m_pUnit = g_pData->GetMyUser()->GetUnitByUID( kRoomSlotInfo_.m_kRoomUserInfo.m_nUnitUID );

#ifdef GUUSER_UNIT_POINTER_CRASH_FIX
			if(g_pX2Game != NULL)
			{
				// �ٸ� ���� �� �����
				g_pX2Game->DeleteUserUnitByUID( uidToBeDeleted );	

				// ������ ���ӿ� �� ���� ��ü�� �־��ٸ� m_pUnit ����
				CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID( m_pUnit->GetUID() );
				if(pGUUser != NULL)
				{
					pGUUser->SetUnit(m_pUnit);
				}
			}
#endif GUUSER_UNIT_POINTER_CRASH_FIX
			bRemakeUnitViewer = true;
		}
	}
	
	if ( NULL != m_pUnit )
		m_pUnit->AccessUnitData().SetKRoomUserInfo( kRoomSlotInfo_.m_kRoomUserInfo );

#ifdef RIDING_SYSTEM
	SetOrClearRidingPetInfo( m_pUnit, kRoomSlotInfo_ );
#endif // RIDING_SYSTEM

	return bRemakeUnitViewer;
}

bool CX2Room::SlotData::Set_KRoomSlotInfoOfOthers( const KRoomSlotInfo& kRoomSlotInfo_ )
{
	bool bRemakeUnitViewer = false;
	m_bMySlot = false;

	if ( NULL == m_pUnit )	// ������ ������ ��� ����
	{
		m_pUnit = new CX2Unit( kRoomSlotInfo_.m_kRoomUserInfo );	// ���� ����

#ifdef GUUSER_UNIT_POINTER_CRASH_FIX
		if (g_pX2Game != NULL)
		{
			CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID(m_pUnit->GetUID());
			if (pGUUser != NULL)
			{
				pGUUser->SetUnit(m_pUnit);
			}
		}
#endif GUUSER_UNIT_POINTER_CRASH_FIX
	}
	else
	{
		// ���� ������ Unit�� ���ο� ������ Unit�� �ٸ���
		if ( m_pUnit->GetUID() != kRoomSlotInfo_.m_kRoomUserInfo.m_nUnitUID )
		{
			//{{ dmlee 2009.4.15 �����濡�� ���� ���� �����
			// note!! ���� �ȿ� �ִ� �ٸ� ������ slotdata�� ����� �Ǵ� ��찡 �ƴ��� Ȯ�� �ʿ�

			const UidType uidToBeDeleted = m_pUnit->GetUID();

			if ( m_pUnit->GetOwnerUserUID() != g_pData->GetMyUser()->GetUID() )
			{
				SAFE_DELETE( m_pUnit );
			}
			else
				m_pUnit = NULL;

			m_pUnit = new CX2Unit( kRoomSlotInfo_.m_kRoomUserInfo );	// �ٽ� ����

#ifdef GUUSER_UNIT_POINTER_CRASH_FIX
			if ( NULL != g_pX2Game )
			{
				g_pX2Game->DeleteUserUnitByUID( uidToBeDeleted );

				CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID( m_pUnit->GetUID() );
				if(pGUUser != NULL)
				{
					pGUUser->SetUnit(m_pUnit);
				}
			}
#endif GUUSER_UNIT_POINTER_CRASH_FIX
			bRemakeUnitViewer = true;
		}
		else	// ������ ���԰� ���ο� ������ Unit�� ���ٸ� ������Ʈ��
		{
			m_pUnit->AccessUnitData().SetKRoomUserInfo( kRoomSlotInfo_.m_kRoomUserInfo );
		}
	}

#ifdef SERV_PVP_NEW_SYSTEM
	if( m_bNpc == false && NULL != m_pUnit )
	{
		m_pUnit->ResetEqip();

#ifdef RIDING_SYSTEM
		SetOrClearRidingPetInfo( m_pUnit, kRoomSlotInfo_ );
#endif // RIDING_SYSTEM

	}
#else //SERV_PVP_NEW_SYSTEM
	m_pUnit->ResetEqip();
#endif //SERV_PVP_NEW_SYSTEM

	return bRemakeUnitViewer;
}



void CX2Room::SlotData::Set_KRoomSlotInfoInCloseOrEmptyRoomstate()
{
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
	m_PingTime		= PING_TIME_TO_BE_INITIALIZED;
    ZeroMemory( m_adwPingSendTime, sizeof(m_adwPingSendTime) );
    m_uPingSendCount = 0;
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
    m_fAvgPingTime  = 0.f;
    m_fAvgPingTimeSqr = 0.f;
    m_fAvgPingStd = 0.f;
    m_dwPingCount   = 0;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE

	m_bMySlot = false;
	if ( NULL != m_pUnit )	// ������ ���� �ִ� �����̸�
	{
		if ( NULL != g_pX2Game )	// ������ ��� �ִ� ��� ���ӿ��� ������
			g_pX2Game->DeleteUserUnitByUID( m_pUnit->GetUID() );

		// �� m_pUnit�� �ƴ϶�� ����
		if ( m_pUnit->GetOwnerUserUID() != g_pData->GetMyUser()->GetUID() )
		{
			SAFE_DELETE( m_pUnit );
		}
 		else	// ������� NULL ����
 			m_pUnit = NULL;

		SAFE_DELETE_KTDGOBJECT( m_pUnitViewer );
	}
}


bool CX2Room::SlotData::Set_KRoomSlotInfo( const KRoomSlotInfo& kRoomSlotInfo )
{
	//CKTDXThread::CLocker locker( g_pMain->GetUnitLoader()->GetLock() );

#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    UidType  oldUnitUID = m_UnitUID;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE

	m_Index			= kRoomSlotInfo.m_Index;
	m_SlotState		= (CX2Room::SLOT_STATE)kRoomSlotInfo.m_SlotState;
	m_bHost			= kRoomSlotInfo.m_bHost;	
	m_bReady		= kRoomSlotInfo.m_bReady;
	m_TeamNum		= kRoomSlotInfo.m_TeamNum;
	m_UnitUID		= kRoomSlotInfo.m_kRoomUserInfo.m_nUnitUID;

	m_bPitIn		= kRoomSlotInfo.m_bPitIn;
	m_bTrade		= kRoomSlotInfo.m_bTrade;
	m_bObserver		= kRoomSlotInfo.m_kRoomUserInfo.m_bIsObserver;

#ifdef ADDED_RELATIONSHIP_SYSTEM
	m_RelationshipTargetUID = kRoomSlotInfo.m_kRoomUserInfo.m_iLoverUnitUID;
#endif // ADDED_RELATIONSHIP_SYSTEM


//#ifdef PVP_BOSS_COMBAT_TEST
//	m_bIsBoss		= kRoomSlotInfo.m_bIsBoss;
//#endif PVP_BOSS_COMBAT_TEST

#ifdef PVP_SEASON2
	m_cRank			= kRoomSlotInfo.m_kRoomUserInfo.m_cRank;
#endif

#ifdef SERV_PVP_NEW_SYSTEM
	m_bNpc			= kRoomSlotInfo.m_kRoomUserInfo.m_bIsPvpNpc;
	if ( true == m_bNpc )
	{
		SAFE_DELETE_KTDGOBJECT( m_pUnitViewer );

		m_iNpcUid		= kRoomSlotInfo.m_kRoomUserInfo.m_nUnitUID;
		m_wstrNpcName	= kRoomSlotInfo.m_kRoomUserInfo.m_wstrNickName;
		m_iNpcRating	= kRoomSlotInfo.m_kRoomUserInfo.m_iRating;
	}
#endif

#ifdef NO_P2P_NO_GAME
	g_pX2Room->SetNumOfPlayers( g_pX2Room->GetUserNum() );
#endif NO_P2P_NO_GAME


	// ���ο� ������ ����ִ� ����
	if ( m_SlotState == SS_CLOSE || m_SlotState == SS_EMPTY )
	{
		Set_KRoomSlotInfoInCloseOrEmptyRoomstate();
		return false;
	}
	else // ���ο� ������ ����ִ� ���°� �ƴ϶��
	{
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
        if ( oldUnitUID != m_UnitUID )
        {
				m_PingTime		= PING_TIME_TO_BE_INITIALIZED;
                ZeroMemory( m_adwPingSendTime, sizeof(m_adwPingSendTime) );
                m_uPingSendCount = 0;
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
                m_fAvgPingTime  = 0.f;
                m_fAvgPingTimeSqr = 0.f;
                m_fAvgPingStd = 0.f;
                m_dwPingCount   = 0;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        }
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE

		bool bRemakeUnitViewer = false;

		// ���ο� ������ �� ���̶��
		if ( kRoomSlotInfo.m_kRoomUserInfo.m_iOwnerUserUID == g_pData->GetMyUser()->GetUID() )
		{
			bRemakeUnitViewer = Set_KRoomSlotInfoOfMine( kRoomSlotInfo );
		}
		else	// ���ο� ������ �� ���� �ƴҶ�
		{
			bRemakeUnitViewer = Set_KRoomSlotInfoOfOthers( kRoomSlotInfo );
		}

		if ( true == m_bObserver )
		{
			SAFE_DELETE_KTDGOBJECT( m_pUnitViewer );
			bRemakeUnitViewer = false;
		}

		return bRemakeUnitViewer;
	}
}

void CX2Room::SlotData::MakeUnitViewer()
{
	//CKTDXThread::CLocker locker( g_pMain->GetUnitLoader()->GetLock() );

	if( m_pUnit != NULL )
	{
		CX2UnitViewerUI::DeleteKTDGObject( m_pUnitViewer );
		m_pUnitViewer = NULL;

		m_pUnitViewer = CX2UnitViewerUI::CreateUnitViewerUI();
#ifdef SEPARATION_MOTION
		m_pUnitViewer->SetUnit( m_pUnit, CX2UnitViewerUI::UVS_ROOM ); 
#else
		m_pUnitViewer->SetUnit( m_pUnit );
#endif
		m_pUnitViewer->GetXSkinAnim()->Reset();
		m_pUnitViewer->PlayByMotionType( CX2UnitViewerUI::UVUMT_WAIT );
		m_pUnitViewer->GetXSkinAnim()->Play( CKTDGXSkinAnim::XAP_LOOP );
		m_pUnitViewer->GetXSkinAnim()->SetApplyMotionOffset( false );

		g_pKTDXApp->GetDGManager()->AddObjectChain( m_pUnitViewer );

#if defined(WORLD_TOOL) || defined(X2TOOL)
		return;
#endif

#ifdef SERV_PET_SYSTEM
		if( m_pUnit->GetPetInfo() != NULL )
		{
			KPetInfo *pPetInfo = m_pUnit->GetPetInfo();
			CX2PET::PetInfo petInfo;

			petInfo.m_PetUid = pPetInfo->m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			petInfo.m_PetId = pPetInfo->m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
			petInfo.m_PetId = pPetInfo->m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
			petInfo.m_Evolution_Step = pPetInfo->m_cEvolutionStep;
			petInfo.m_wstrPetName = pPetInfo->m_wstrPetName;		
			petInfo.m_Intimacy = (float)pPetInfo->m_iIntimacy;
			petInfo.m_Satiety = pPetInfo->m_sSatiety;
			petInfo.m_Emotion = (int)pPetInfo->m_sEmotion;
			petInfo.m_Extroversion = (int)pPetInfo->m_sExtroversion;	
#ifdef PET_DROP_ITEM_PICKUP
			petInfo.m_bIsDropItemPickup = pPetInfo->m_bAutoLooting;
#endif //PET_DROP_ITEM_PICKUP
#ifdef SERV_PET_SYSTEM_EX1
			petInfo.m_bAlwaysMaxSatiety = pPetInfo->m_bAlwaysMaxSatiety;
#endif //SERV_PET_SYSTEM_EX1

			petInfo.m_bSummon = true;
#ifdef SERV_EVENT_PET_INVENTORY
			petInfo.m_bEventFoodEat		= pPetInfo->m_bEventFoodEat;
			petInfo.m_bIsEventPetID		= pPetInfo->m_bIsEventPetID;
#endif SERV_EVENT_PET_INVENTORY

			g_pData->GetPetManager()->CreatePet( m_pUnit->GetUID(), petInfo );
		}
		else
		{
			g_pData->GetPetManager()->RemovePet( m_pUnit->GetUID() );
		}
#endif
	}

}

bool CX2Room::SlotData::IsMyUnit() const
{
	//CKTDXThread::CLocker locker( g_pMain->GetUnitLoader()->GetLock() );
	if ( m_pUnit != NULL )
	{
		if ( g_pData->GetMyUser()->IsMyUnitByUnitUID( m_pUnit->GetUnitData().m_UnitUID ) == true )
		{
			return true;
		}
	}


	return false;
}

void CX2Room::SlotData::CheckMakeUnitViewer()
{
	if( NULL == m_pUnitViewer || m_pUnitViewer->GetUnit() != m_pUnit )
	{
		MakeUnitViewer();
	}
	else
	{
		m_pUnitViewer->UpdateEqip( false ); // ������ �������� ���ư��� ��� ��ü�ϰ� �ٽ� �����ϴ� ��� ������ ��ü update �ϵ��� ����
	}

	// note!! ������ setshowobject(false)�� ȣ�����ֱ� ���ؼ� true, false �������� ȣ������. 
	// fix!! �Ƹ� ��򰡿��� unitviewer�� setshowobject(false)�Ǿ����� ������ object���� setshowobject(true)�Ǿ��ִ� �� ����. ã�Ƽ� �����ؾ���
	m_pUnitViewer->SetShowObject( true );
	m_pUnitViewer->SetShowObject( false );
}

#ifdef RIDING_SYSTEM
void CX2Room::SlotData::SetOrClearRidingPetInfo( CX2Unit* pUnit_, const KRoomSlotInfo& kRoomSlotInfo_ )
{
	/// ���� ���̵� ���� Ÿ�� ������
	if ( NULL != pUnit_ )
	{
		if ( kRoomSlotInfo_.m_kRoomUserInfo.m_iRidingPetUID > 0 )
		{
			KRidingPetInfo kRidingPetInfo;
			kRidingPetInfo.m_iRidingPetUID = kRoomSlotInfo_.m_kRoomUserInfo.m_iRidingPetUID;
			kRidingPetInfo.m_usRindingPetID = kRoomSlotInfo_.m_kRoomUserInfo.m_usRidingPetID;
			pUnit_->SetFullRidingPetInfo( kRidingPetInfo );			
		}
		else
			pUnit_->ClearRidingPetInfo();
	}
}
#endif // RIDING_SYSTEM

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-11

/** @function	: Handler_EGS_DECISION_ENTER_DEFENCE_DUNGEON_NOT
	@brief		: �� �ȿ� �ִ� ������ ���� �ɷ�ġ ���� ��Ŷ ó��
	@param		: �̺�Ʈ �޽���
	@return		: ó�� ����
*/
bool CX2Room::Handler_EGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT( KEGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT& kEvent_ )
{
	if(g_pX2Game == NULL)
		return false;
	CX2GUUser *pUser = g_pX2Game->GetUserUnitByUID( kEvent_.m_iUnitUID );

	if( pUser == NULL )
		return false;

	pUser->SetGameStat( kEvent_.m_kGameStat );
	if ( NULL == pUser->GetUnit() )
		return false;

	CX2Unit::UnitData& kUnitData = pUser->GetUnit()->AccessUnitData();//pSlotData->m_pUnit->GetUnitData();

	kUnitData.m_GameStat.SetKStat( kEvent_.m_kGameStat );

	pUser->SetMaxHp( static_cast< const float >( kEvent_.m_kGameStat.m_iBaseHP ) );
	if ( pUser->GetNowHp() > pUser->GetMaxHp() )
	{
		pUser->SetNowHp( pUser->GetMaxHp() );
	}

	pUser->UpdateSocketDataAndEnchantData();
	pUser->UpdatePassiveAndActiveSkillState();
	
	return true;
}

#endif // SERV_NEW_DEFENCE_DUNGEON
