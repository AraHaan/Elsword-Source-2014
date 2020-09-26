#include "CnProxy.h"
#include "NetError.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "GSUser.h"
#include "ActorManager.h"
#include "ProxyManager.h"
#include "KncSend.h"
#include "GameServer.h"
#include "DBLayer.h"
#include "RoomListManager.h"
//{{ 2011. 12. 05	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewPartyListManager.h"
#else
	#include "PartyListManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#ifdef DEF_TRADE_BOARD
#include "PersonalShopItemList.h"
#endif DEF_TRADE_BOARD

//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}
//{{ 2011. 12. 14	������	��Ŷ ó�� ���� �ð� üũ
#ifdef SERV_PACKET_PROCESSING_LATENCY_CHECK
	#include "PacketMornitoring/PacketMornitoring.h"
#endif SERV_PACKET_PROCESSING_LATENCY_CHECK
//}}
//{{ 2012. 11. 20	������		��Ʋ�ʵ� �� ���� ��� �߰�
#ifdef SERV_BATTLE_FIELD_ROOM_CLEAR_AT_CN_PROXY_DIE
	#include "BattleFieldListManager.h"
#endif SERV_BATTLE_FIELD_ROOM_CLEAR_AT_CN_PROXY_DIE
//}}

NiImplementRTTI( KCnProxy, KActorProxy );
//ImplementProxy( KCnProxy );
ImplPfID( KCnProxy, PI_CN_PROXY );

#define CLASS_TYPE KCnProxy

KCnProxy::KCnProxy(void)
//{{ 2010. 04. 16  ������	������ ��ü ����
#ifdef SERV_INTEGRATION
:
m_iCnServerGroupID( -1 ),
m_iGsServerGroupID( -1 )
#endif SERV_INTEGRATION
//}}
{
}

KCnProxy::~KCnProxy(void)
{
}

ImplToStringW( KCnProxy )
{
    return START_TOSTRING_PARENTW( KActorProxy )
        << TOSTRINGW( KncUtil::toWideString(GetIPStr()) )
        << TOSTRINGW( GetPort() )
        << TOSTRINGWb( IsConnected() );
}

// Call to ProcessEventQueue (Client)
void KCnProxy::ProcessEvent( const KEventPtr& spEvent_ )
{
    if( RoutePacket( spEvent_ ) )
    {
        return;
    }

    switch( spEvent_->m_usEventID )
    {
	//{{ 2011. 07. 19	������	������ ���� �ڵ� �����丵
	//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING	
	CASE( ECN_VERIFY_SERVER_CONNECT_ACK );
	//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
	//}}

	//{{ 2010. 01. 27  ������	�� ����
#ifdef SERV_ROOM_COUNT
	CASE( ERM_ROOM_COUNT_INFO_NOT );
#endif SERV_ROOM_COUNT
	//}}

    CASE_NOPARAM( E_RESERVE_DESTROY );

    default:
        START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ��� - " << spEvent_->GetIDStr() );
    }
}

void KCnProxy::OnDestroy()
{
    KActorProxy::OnDestroy();

	//{{ 2010. 05. 17  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	// ���� ������ CnProxy��� ����Ʈ���� ����
	SiKProxyManager()->DelSameServerGroupProxyID( GetProxyID() );
#endif SERV_INTEGRATION
	//}}

	SiKProxyManager()->DestroyProxy( KProxyManager::PT_CENTER, GetProxyID() );

	if( GetUID() != 0 )
	{
		// center server�� ���� ��� - �ش� ���Ϳ� ����ִ� �� / ���� ���� ���� ��Ͽ��� ��� ����. 

		UidType iProxyID = static_cast<UidType>(GetProxyID());
		int nDelRoomCount	= SiKRoomListManager()->DelRoomInfo( iProxyID );//GetUID() );
		int nDelSquareCount = SiKRoomListManager()->DelSquareInfo( iProxyID );//GetUID() );

#ifdef DEF_TRADE_BOARD
		SiKPersonalShopItemList()->DelItemInfo(iProxyID);
#endif DEF_TRADE_BOARD

		//{{ 2010. 03. 09  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
		int nDelPartyCount = SiKPartyListManager()->DelPartyInfo( iProxyID );
#endif SERV_CHANNEL_PARTY
		//}}

		//{{ 2012. 11. 20	������		��Ʋ�ʵ� �� ���� ��� �߰�
#ifdef SERV_BATTLE_FIELD_ROOM_CLEAR_AT_CN_PROXY_DIE
		SiKBattleFieldListManager()->DeleteBattleFieldRoomInfo( iProxyID );
#endif SERV_BATTLE_FIELD_ROOM_CLEAR_AT_CN_PROXY_DIE
		//}}

		// ���� ���� �������� �����ϰ� �ִ� �������� ������Ѿ� �Ѵ�. (��ο��� �̺�Ʈ ����.)
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_USER, NULL, ERM_CHECK_INVALID_ROOM_NOT, iProxyID );
		KActorManager::GetKObj()->QueueingToAll( spEvent );

		START_LOG( clog2, L"���ͼ����� ���B��. +��+/ ���õ� �߱�..;��;" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( nDelRoomCount )
			<< BUILD_LOG( nDelSquareCount )
			//{{ 2010. 03. 09  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
			<< BUILD_LOG( nDelPartyCount )
#endif SERV_CHANNEL_PARTY
			//}}
			<< END_LOG
			;
	}

	//{{ 2009. 1. 30  ������	��ü �Ҹ� �α�
	if( GetDisconnectReason() == KStatistics::eSIColDR_Server_Shutdown )
	{
		START_LOG( cout, L"���� �˴ٿ����� ���� CnProxy��ü �Ҹ�" )
			<< BUILD_LOG( GetProxyID() );
	}
	else
	{
		START_LOG( cout2, L"���������� �������� ���� CnProxy��ü �Ҹ�" )
			<< BUILD_LOG( GetProxyID() )
			<< BUILD_LOG( GetDisconnectReason() )
			<< dbg::tab << L"���� ���� : " << KStatistics::GetDissconnectReasonString( GetDisconnectReason() ) << dbg::endl;

		//{{ 2010. 10. 11	������	������ ���� ���� �α�
#ifdef SERV_SERVER_DISCONNECT_LOG
		{
			CTime kRegDate = CTime::GetCurrentTime();
			KE_LOCAL_LOG_SERVER_DISCONNECT_NOT kNot;
			kNot.m_cLogType				= KE_LOCAL_LOG_SERVER_DISCONNECT_NOT::SDLT_DISCONNECT;
			kNot.m_wstrSourceName		= boost::str( boost::wformat( L"%d" ) % KBaseServer::GetKObj()->GetUID() );
			kNot.m_wstrDestinationName	= boost::str( boost::wformat( L"%d" ) % GetProxyID() );
			kNot.m_wstrDestinationIP	= KncUtil::toWideString( GetIPStr() );
			kNot.m_wstrReason			= KStatistics::GetDissconnectReasonString( GetDisconnectReason() );
			kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
			KSIManager.QueueingEvent( E_LOCAL_LOG_SERVER_DISCONNECT_NOT, kNot );
		}		
#endif SERV_SERVER_DISCONNECT_LOG
		//}}

		//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
		{
			KE_DISCONNECT_SERVER_REPORT_NOT kNot;
			kNot.m_cType = KMornitoringManager::ET_DISCONNECT_GAME_CN_PROXY;
			KEventPtr spEvent( new KEvent );
			spEvent->SetData( 0, NULL, E_DISCONNECT_SERVER_REPORT_NOT, kNot );
			KBaseServer::GetKObj()->QueueingEvent( spEvent );
		}
#endif SERV_MORNITORING
		//}}
	}
	//}}
}

//{{ 2011. 07. 19	������	������ ���� �ڵ� �����丵
//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING	
IMPL_ON_FUNC( ECN_VERIFY_SERVER_CONNECT_ACK )
{
    LOG_SUCCESS( kPacket_.m_nOK == NetError::NET_OK )
        << BUILD_LOG( kPacket_.m_nOK );

    m_nAckOK = kPacket_.m_nOK;
    if( m_nAckOK == NetError::NET_OK )
    {
        KBaseServer::GetKObj()->SetUID( kPacket_.m_nUID );

		SetUID( kPacket_.m_iServerUID );

		//{{ 2010. 04. 16  ������	������ ��ü ����
#ifdef SERV_INTEGRATION
		m_iCnServerGroupID = kPacket_.m_iDBRegServerGroupID;
		m_iGsServerGroupID = GetKGameServer()->GetServerGroupID();

		if( IsSameServerGroup() )
		{
			START_LOG( cout2, L"���� ������ ���ͼ����� ���������� �����Ͽ����ϴ�!" )
				<< BUILD_LOG( kPacket_.m_iServerUID )
				<< BUILD_LOG( m_iGsServerGroupID )
				<< BUILD_LOG( m_iCnServerGroupID );

			// ���� ������ CnProxy��� ����Ʈ�� �߰�
			SiKProxyManager()->AddSameServerGroupProxyID( GetProxyID() );
		}
		else
		{
			START_LOG( cout2, L"�ٸ� ������ ���ͼ����� ���������� �����Ͽ����ϴ�!" )
				<< BUILD_LOG( kPacket_.m_iServerUID )
				<< BUILD_LOG( m_iGsServerGroupID )
				<< BUILD_LOG( m_iCnServerGroupID );
		}
#else
		//{{ 2009. 3. 25  ������	�α�
		START_LOG( cout2, L"���ͼ����� ���������� �����Ͽ����ϴ�!" )
			<< BUILD_LOG( kPacket_.m_iServerUID );
		//}}
#endif SERV_INTEGRATION
		//}}
    }

    SetEvent( m_spEventAck.get() );
}
//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
//}}

//{{ 2010. 01. 27  ������	�� ����
#ifdef SERV_ROOM_COUNT

IMPL_ON_FUNC( ERM_ROOM_COUNT_INFO_NOT )
{
	// ������ ������ ������ ������ ���Ѱ��� Load������ �Ѵ�.
	// Load���� ���� ���� ���ͼ����� �������� ������� �ϰԵ�.
	SetLoad_NEW( static_cast<int>(kPacket_.m_cRoomType), kPacket_.m_bIsOpen );
}

#endif SERV_ROOM_COUNT
//}}

IMPL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY )
{
    m_bDestroyReserved = true;
}

bool KCnProxy::RoutePacket( const KEventPtr& spEvent_ )
{
	//{{ 2011. 12. 14	������	��Ŷ ó�� ���� �ð� üũ
#ifdef SERV_PACKET_PROCESSING_LATENCY_CHECK
	SiKPacketMornitoring()->CheckCnProxyPacketProcessingCount( spEvent_->m_usEventID );
#endif SERV_PACKET_PROCESSING_LATENCY_CHECK
	//}}

    //START_LOG( clog, L"�����" )
    //    << BUILD_LOG( pkEvent_->m_kDestPerformer.m_dwPerformerID )
    //    << BUILD_LOG( pkEvent_->m_kDestPerformer.m_iUID )
    //    << BUILD_LOG( pkEvent_->GetLastSenderUID() )
    //    << BUILD_LOG( pkEvent_->m_usEventID )
    //    << BUILD_LOG( pkEvent_->GetIDStr() );

    if( spEvent_->m_kDestPerformer.m_dwPerformerID != PI_CN_PROXY )
    {
        int iEval = CompareServerLevel( spEvent_->m_kDestPerformer.m_dwPerformerID, PI_CN_PROXY );
        if( iEval < 0 )
        {
            _JIF( !spEvent_->IsEmptyTrace(), return false );
            KActorManager::GetKObj()->QueueingTo( spEvent_->GetLastSenderUID(), spEvent_ );
            return true;
        }
        else if( iEval > 0 )
        {
            START_LOG( cerr, L"���� ������ ���� �̺�Ʈ�� ���� �ȵȴ�." )
                << BUILD_LOG( spEvent_->m_kDestPerformer.m_dwPerformerID )
                << BUILD_LOG( spEvent_->m_usEventID )
                << BUILD_LOG( spEvent_->GetIDStr() )
                << END_LOG;
            return true;
        }
        else
        {			
            switch( GetPerformerClass( spEvent_->m_kDestPerformer.m_dwPerformerID ) )
            {
            case PC_USER:
                KActorManager::GetKObj()->MultiQueueing( spEvent_ );
                return true;
            case PC_CHARACTER:
				{
					//080711.hoons.
					//�켱 1�������� ���ΰŷ� ��ó���� ���Ǳ� ������ ĳ���ͺκи��� ���..
					std::vector<UidType> vecUID;
					int iNotExistUserCnt = KActorManager::GetKObj()->MultiQueueingByCID( spEvent_, vecUID );
					if( iNotExistUserCnt > 0 )
					{
						ProcbyNotExistUser( spEvent_, vecUID );

						START_LOG( cwarn, L"��Ŷ���� ������ ����." )
							<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
							<< BUILD_LOG( iNotExistUserCnt )
							<< END_LOG;
					}
				}
                return true;
            case PC_SERVER:
                KBaseServer::GetKObj()->QueueingEvent( spEvent_ );
                return true;
            case PC_GAME_DB:
                SiKDBLayer()->QueueingEvent( spEvent_ );
                return true;
            default:
                START_LOG( cerr, L"�������� �̻��� ��Ŷ." )
                    << BUILD_LOG( spEvent_->m_kDestPerformer.m_dwPerformerID )
                    << END_LOG;
                return true;
            }
        }
    }

    return false;
}

void KCnProxy::ProcbyNotExistUser( const KEventPtr& spEvent_, std::vector<UidType>& vecUID )
{
	switch( spEvent_->m_usEventID )
	{
		//{{ 2009. 2. 10  ������	���ΰŷ� ���� ����		
	case ERM_CONFIRM_TRADE_NOT:
		{
			for( int i = 0; i < (int)vecUID.size(); ++i )
			{
				KSerializer ks;
				ks.BeginReading( &spEvent_->m_kbuff );
				KERM_CONFIRM_TRADE_NOT kData_;
				if( !ks.Get(kData_) )
				{
					START_LOG( cerr, L"deserialze failed." )
						<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
						<< BUILD_LOG( vecUID[i] )
						<< END_LOG;
				}
				else
				{
					KDBE_INSERT_TRADE_ITEM_BY_SERVER_NOT kPacketToDB;
					kPacketToDB.m_iUnitUID = vecUID[i];
					kPacketToDB.m_kItemQuantityUpdate = kData_.m_kItemQuantityUpdateTemp;
					kPacketToDB.m_vecItemInfo = kData_.m_vecItemInfoTemp;
					kPacketToDB.m_iED = kData_.m_iEDTemp;

					UidType anTrace[2] = { vecUID[i], -1 };
					KncSend( PI_GS_USER, vecUID[i], PI_GS_GAME_DB, 0, anTrace, DBE_INSERT_TRADE_ITEM_BY_SERVER_NOT, kPacketToDB );

					spEvent_->m_kbuff.Reset();
				}
			}
		}
		break;

	case ERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT:
		{
			for( int i = 0; i < (int)vecUID.size(); ++i )
			{
				KSerializer ks;
				ks.BeginReading( &spEvent_->m_kbuff );
				KERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT kData_;
				if( !ks.Get(kData_) )
				{
					START_LOG( cerr, L"deserialze failed." )
						<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
						<< BUILD_LOG( vecUID[i] )
						<< END_LOG;
				}
				else
				{
					KDBE_INSERT_TRADE_ITEM_BY_SERVER_NOT kPacketToDB;
					kPacketToDB.m_iUnitUID = vecUID[i];
					kPacketToDB.m_kItemQuantityUpdate = kData_.m_kItemQuantityUpdateTemp;
					kPacketToDB.m_vecItemInfo = kData_.m_vecItemInfoTemp;
					kPacketToDB.m_iED = kData_.m_iEDTemp;

					UidType anTrace[2] = { vecUID[i], -1 };
					KncSend( PI_GS_USER, vecUID[i], PI_GS_GAME_DB, 0, anTrace, DBE_INSERT_TRADE_ITEM_BY_SERVER_NOT, kPacketToDB );

					spEvent_->m_kbuff.Reset();
				}
			}
		}
		break;
		//}}

		//{{ 2011. 08. 03	������	���� ���� ���ῡ ���� ����ó��
#ifdef SERV_CLIENT_QUIT_PVP_BUG_PLAY_FIX
	case ERM_UPDATE_PVP_UNIT_INFO_NOT:
		{
			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUID )
			{
				KSerializer ks;
				ks.BeginReading( &spEvent_->m_kbuff );
				KERM_UPDATE_PVP_UNIT_INFO_NOT kData_;
				if( !ks.Get(kData_) )
				{
					START_LOG( cerr, L"deserialze failed." )
						<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
						<< BUILD_LOG( iUnitUID )
						<< END_LOG;
				}
				else
				{
					// ���� ������ �ƴϸ� ���� ������Ʈ�� �ʿ䰡 ����.
					if( kData_.m_bIsOfficialMatch == true )
					{
						const int iNewRating = kData_.m_iCurrentRating + kData_.m_iRating;

						//{{ 2012. 06. 19	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
#else
						const CXSLUnit::PVP_EMBLEM eEmblem = CXSLUnit::GetPVPEmblem( iNewRating );
#endif SERV_2012_PVP_SEASON2
						//}}

						int iChangeMaxRating = 0;
						if( iNewRating > kData_.m_iCurrentMaxRating )
						{
							iChangeMaxRating = iNewRating - kData_.m_iCurrentMaxRating;
						}

						// RPoint���� 0���� �۾��� �� �����Ƿ� üũ����!
						if( ( kData_.m_iRPoint + kData_.m_iCurrentRPoint ) < 0 )
						{
							kData_.m_iRPoint = -kData_.m_iCurrentRPoint;
						}

						// APoint���� 0���� �۾��� �� �����Ƿ� üũ����!
						if( ( kData_.m_iAPoint + kData_.m_iCurrentAPoint ) < 0 )
						{
							kData_.m_iAPoint = -kData_.m_iCurrentAPoint;
						}

						KDBE_QUIT_USER_PVP_RESULT_UPDATE_NOT kPacket;
						kPacket.m_iUnitUID			= iUnitUID;
						kPacket.m_iOfficialMatchCnt = ( kData_.m_bIsOfficialMatch ? 1 : 0 );
						kPacket.m_iRating			= kData_.m_iRating;
						kPacket.m_iMaxRating		= iChangeMaxRating;
						kPacket.m_iRPoint			= kData_.m_iRPoint;
						kPacket.m_iAPoint			= kData_.m_iAPoint;
						kPacket.m_bIsWinBeforeMatch	= ( kData_.m_iWin == 1 );

						//{{ 2012. 06. 19	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
						kPacket.m_cEmblemEnum		= static_cast<char>( CXSLUnit::ComputeNewPVPRank( static_cast<CXSLUnit::PVP_RANK>( kData_.m_cRankForServer ), iNewRating ) );
						kPacket.m_fKFactor			= kData_.m_fKFactorIncrement;

						if( kPacket.m_cEmblemEnum == CXSLUnit::PVPRANK_NONE )
						{
							START_LOG( cerr, L"PVPRANK_NONE ��ũ!! �߻��ؼ� �ȵȴ�. m_bIsWinBeforeMatch�� ������ ������ ���� ���� ��Ű�� �ʴ´�!!")
								<< BUILD_LOG( iUnitUID )
								<< END_LOG;
							kPacket.m_iOfficialMatchCnt = 0;
							kPacket.m_iRating			= 0;
							kPacket.m_iMaxRating		= 0;
							kPacket.m_iRPoint			= 0;
							kPacket.m_iAPoint			= 0;
							kPacket.m_cEmblemEnum		= kData_.m_cRankForServer;
							kPacket.m_fKFactor			= 0.0;
						}
#else
						kPacket.m_cEmblemEnum		= static_cast<char>(CXSLUnit::PvpEmblemToPvpRank( eEmblem ));
#endif SERV_2012_PVP_SEASON2
						//}}
						kPacket.m_iWin				= kData_.m_iWin;
						kPacket.m_iLose				= kData_.m_iLose;

						UidType anTrace[2] = { iUnitUID, -1 };
						KncSend( PI_GS_USER, iUnitUID, PI_GS_GAME_DB, 0, anTrace, DBE_QUIT_USER_PVP_RESULT_UPDATE_NOT, kPacket );

						START_LOG( cout, L"���� ��� ��Ŷ�� ���� GSUser�� ��� �ڵ����� DB�� ��� ������Ʈ �մϴ�!" )
							<< BUILD_LOG( kPacket.m_iUnitUID )
							<< BUILD_LOG( kData_.m_iCurrentRating )
							<< BUILD_LOG( kData_.m_iCurrentMaxRating )
							<< BUILD_LOG( kData_.m_iCurrentRPoint )
							<< BUILD_LOG( kData_.m_iCurrentAPoint )
							<< BUILD_LOG( kPacket.m_iOfficialMatchCnt )
							<< BUILD_LOG( kPacket.m_iRating )
							<< BUILD_LOG( kPacket.m_iMaxRating )
							<< BUILD_LOG( kData_.m_iRPoint )
							<< BUILD_LOG( kPacket.m_iRPoint )
							<< BUILD_LOG( kData_.m_iAPoint )
							<< BUILD_LOG( kPacket.m_iAPoint )
							<< BUILD_LOG( kPacket.m_bIsWinBeforeMatch )
							<< BUILD_LOGc( kPacket.m_cEmblemEnum )
							<< BUILD_LOG( kPacket.m_iWin )
							<< BUILD_LOG( kPacket.m_iLose );
					}

					spEvent_->m_kbuff.Reset();
				}
			}
		}
		break;
#endif SERV_CLIENT_QUIT_PVP_BUG_PLAY_FIX
		//}}

		//{{ 2012. 05. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	case ERM_GAME_START_TIME_COUNT_NOT:
		{
			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUID )
			{
				KSerializer ks;
				ks.BeginReading( &spEvent_->m_kbuff );
				KEGS_GAME_START_TIME_COUNT_NOT kData_;
				if( !ks.Get(kData_) )
				{
					START_LOG( cerr, L"deserialze failed." )
						<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
						<< BUILD_LOG( iUnitUID )
						<< END_LOG;
				}
				else
				{
					START_LOG( cout, L"���� ���� Ÿ�̸Ӹ� �޾ƾ��� ������ ����! �����Ƴ�? �׷��� �濡���� ��Ż ó������!" )
						<< BUILD_LOG( iUnitUID );

					// �濡�� ��Ż ��û!
					KERM_LEAVE_ROOM_REQ kPacket;
					kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_01;

					UidType anTrace[2] = { iUnitUID, -1 };
					KncSend( PI_GS_USER, iUnitUID, PI_CN_ROOM, kData_.m_iRoomUID, anTrace, ERM_LEAVE_ROOM_REQ, kPacket );

					spEvent_->m_kbuff.Reset();
				}
			}
		}
		break;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}
}





