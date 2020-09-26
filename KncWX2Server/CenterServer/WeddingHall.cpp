#include "WeddingHall.h"
#include "WeddingHallUserManager.h"
#include "NetError.h"
#include "Room/RoomFSM.h"
#include "X2Data/XSLRoom.h"
#include "CnRoomManager.h"

#include "X2Data/XSLUnitManager.h"
#include "x2data/XSLItemManager.h"

#include "GameSysVal/GameSysVal.h"

//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
#include "Socket/NetCommon.h"		// ������ ����(����� �� ����) IP
#include "CnNetLayer.h"				// ������ ����(����� �� ����) Port
#endif SERV_CHOOSE_FASTEST_HOST
//}

//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	#include "X2Data/XSLBuffManager.h"
#endif SERV_SERVER_BUFF_SYSTEM
//}

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
NiImplementRTTI( KWeddingHall, KRoom );

#define CLASS_TYPE KWeddingHall

KWeddingHall::KWeddingHall()
{

	m_spUserManager = KWeddingHallUserManagerPtr( new KWeddingHallUserManager );
	SetRoomUserManager( m_spUserManager );


	m_spUserManager->Init( CXSLRoom::RT_WEDDING_HALL, SEnum::WEDDING_HALL_MAX_NUM );

	m_bEndPlay				= false;
	m_cGetItemType			= CXSLRoom::DGIT_RANDOM;
	m_cRoomType				= CXSLRoom::RT_WEDDING_HALL;
}

KWeddingHall::~KWeddingHall()
{
}

ImplToStringW( KWeddingHall )
{
    return START_TOSTRING_PARENTW( KRoom );
}

void KWeddingHall::Tick()
{
    KRoom::Tick();

	if( GetStateID() == KRoomFSM::S_PLAY )
	{
		//////////////////////////////////////////////////////////////////////////
		// ��Ƽ�� Ÿ�̸� üũ
        CheckContinueTimer();
		//////////////////////////////////////////////////////////////////////////

	}
	else if( GetStateID() == KRoomFSM::S_WAIT )
	{
        if( m_tTimer[TE_WAIT_TIMER].elapsed() > 10.0 )
		{
			// ��ȥ������ ��� �ð��� 10�ʸ� ������ ���� �Ҹ� ��Ų��!
			StateTransition( KRoomFSM::I_TO_CLOSE );
		}
	}
}

void KWeddingHall::_BroadCast_RejectUser( int iFlag_, const KEvent& kEvent_ , const UidType iRejectUserUID )
{
	std::map< UidType, std::set<UidType> > mapUserList;
	m_spRoomUserManager->GetUserList( iFlag_, mapUserList );
	m_spRoomUserManager->GetUserList( KRoomUserManager::UL_ALL, mapUserList, KRoomUserManager::UT_OBSERVER );

	// GameServer�� ���� loop
	std::map< UidType, std::set<UidType> >::iterator mit;
	for( mit = mapUserList.begin(); mit != mapUserList.end(); mit++ )
	{
		if( iRejectUserUID == mit->first )
			continue;

		KEventPtr spEvent( kEvent_.Clone() );
		spEvent->m_anTrace[0] = mit->first;
		spEvent->m_anTrace[1] = -1;

		// User ������ ���� loop
		std::set<UidType>::iterator sit;
		for( sit = mit->second.begin(); sit != mit->second.end(); sit++ )
		{
			LIF( spEvent->m_kDestPerformer.AddUID( *sit ) );
		}

		KncSend( PI_CN_ROOM, GetUID(), spEvent );
	}
}

void KWeddingHall::SetRoomInfo( const KRoomInfo& kInfo )
{
    m_cRoomType			= kInfo.m_RoomType;
    m_uiRoomListID		= kInfo.m_uiRoomListID;
    m_bPublic			= kInfo.m_bPublic;
    m_bTeamBalance		= false;
    m_wstrPassword		= kInfo.m_wstrPassword;
    m_cGameType			= CXSLRoom::PGT_TEAM;
    m_cWinningNumKill	= 0;
    m_sWorldID			= kInfo.m_WorldID;
    m_cDifficultyLevel	= kInfo.m_DifficultyLevel;
    m_iDungeonID		= kInfo.m_iDungeonID;
	m_cDungeonMode		= kInfo.m_cDungeonMode;

	if( kInfo.m_cGetItemType <= CXSLRoom::DGIT_NONE || kInfo.m_cGetItemType >= CXSLRoom::DGIT_END )
	{
		START_LOG( cwarn, L"�������� ������ ����Ÿ���� �߸��Ǿ���.!" )
			<< BUILD_LOGc( m_cGetItemType )
			<< BUILD_LOGc( kInfo.m_cGetItemType )
			<< BUILD_LOGc( kInfo.m_iDungeonID )
			<< BUILD_LOGc( kInfo.m_DifficultyLevel )
			<< END_LOG;

		m_cGetItemType		= CXSLRoom::DGIT_RANDOM;		
	}
	else
	{
		m_cGetItemType		= kInfo.m_cGetItemType;
	}

	SetName( kInfo.m_RoomName );
	SetPlayTimeLimit( kInfo.m_fPlayTime );

	m_spUserManager->SetMaxSlot( SEnum::WEDDING_HALL_MAX_NUM );
}

void KWeddingHall::GetRoomInfo( KRoomInfo& kInfo )
{
	KRoom::GetRoomInfo( kInfo );
  	kInfo.m_MaxSlot = m_spUserManager->GetMaxSlot(); // �ش� ��ȥ���常�� �ִ� ���� ������ ��������!
}

void KWeddingHall::ProcessNativeEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {
	CASE( ERM_OPEN_WEDDING_HALL_REQ );
	CASE( ERM_JOIN_WEDDING_HALL_REQ );
	CASE( ERM_LEAVE_ROOM_REQ );
	_CASE( ERM_LEAVE_WEDDING_HALL_REQ, KEGS_LEAVE_WEDDING_HALL_REQ );
	CASE_NOPARAM( ERM_CHECK_START_WEDDING_REQ );
	_CASE( ERM_START_WEDDING_ASK_NOT, KEGS_START_WEDDING_ASK_ACK );
	CASE( DBE_WEDDING_COMPLETE_ACK );
	_CASE( ERM_SQUARE_UNIT_SYNC_DATA_REQ, KEGS_SQUARE_UNIT_SYNC_DATA_REQ );
	_CASE( ERM_CHECK_INVALID_USER_NOT, UidType );
    default:
        START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ��� - " << spEvent_->GetIDStr() )
            << BUILD_LOG( spEvent_->m_usEventID );
    }
}

void KWeddingHall::StartGame()
{
    KRoom::StartGame();

	// ��Ʋ�ʵ�� ���½ð�üũ�� ���� �ʽ��ϴ�.
	EnableRemainingPlaytime( false );
	
	const int iPlayerCount = m_spUserManager->GetNumMember();
    
    const int iPlayingUserCount = m_spUserManager->GetNumPlaying();
    if( iPlayingUserCount < 1 || iPlayingUserCount > m_spUserManager->GetMaxSlot() )
    {
        START_LOG( cerr, L"�÷��� ���� ���� ���� �̻���." )
            << BUILD_LOG( iPlayingUserCount )
            << END_LOG;
    }
}

void KWeddingHall::StartPlay()
{
    KRoom::StartPlay();
}

void KWeddingHall::EndPlay()
{
	m_bEndPlay = false;
    KRoom::EndPlay();
}

void KWeddingHall::EndGame()
{
    KRoom::EndGame();
}

int KWeddingHall::DecideWinTeam()
{
	// ��Ʋ�ʵ�� ���а����� ����! ���� ȣ��ȴٰ� �ص� ���Ͱ� �¸��Ѱ����� ó������!
	return CXSLRoom::TN_MONSTER;
}

int KWeddingHall::GetPlayMode()
{
    return m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
}

void KWeddingHall::HandleTimeOutUser( std::map< UidType, UidType >& mapUserCIDNGS )
{
    if( mapUserCIDNGS.empty() )
    {
        return;
    }

    std::map< UidType, UidType >::iterator mit;
    for( mit = mapUserCIDNGS.begin(); mit != mapUserCIDNGS.end(); ++mit )
    {
		KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mit->first );
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"�ش� ������ �������� �ʽ��ϴ�!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		const UidType iUnitUID = mit->first;
		const UidType iPartyUID = spRoomUser->GetPartyUID();

		if( !m_spUserManager->LeaveRoom( mit->first ) )
		{
			START_LOG( cerr, L"�� ������ ����." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
		}

        // ���� �������� ��Ŷ�� ������.
        SendToGSCharacter( mit->second, mit->first, ERM_GAME_LOADING_TIME_OUT_NOT );

		// ���� ��� �����鿡�� ���� ������ ������.
		KEGS_LEAVE_WEDDING_HALL_NOT kPacketNot;
		kPacketNot.m_cRoomState = GetStateID();
		kPacketNot.m_iUnitUID = iUnitUID;
//		GetRoomSlotInfo( kPacketNot.m_vecSlot );
		BroadCast( ERM_LEAVE_WEDDING_HALL_NOT, kPacketNot );
    }    
	
	if( GetStateID() == KRoomFSM::S_LOAD  ||  GetStateID() == KRoomFSM::S_RESULT )
	{
		START_LOG( cerr, L"��Ʋ�ʵ忡���� ������� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( GetStateIDString() )
			<< BUILD_LOG( mapUserCIDNGS.size() )
			<< END_LOG;
	}

    // �濡�� ��� ������ �������� ���� �ϴ� WAIT���·� �̵���Ų��!
    if( IsEmpty() )
    {
		StateTransition( KRoomFSM::I_TO_RESULT );
        StateTransition( KRoomFSM::I_TO_WAIT );
		m_tTimer[TE_WAIT_TIMER].restart();
    }
}

void KWeddingHall::DeleteZombie( std::vector<UidType>& vecZombieUnit )
{
	START_LOG( cout2, L"[�˸�] ��Ʋ�ʵ� ���� ���� ����!" )
		<< BUILD_LOG( vecZombieUnit.size() );

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecZombieUnit )
	{
		KRoomUserPtr spUser = m_spUserManager->GetUser( iUnitUID );
		if( IS_NOT_NULL( spUser ) )
		{
			START_LOG( cout2, L"[�˸�] ��Ʋ�ʵ� ���� ���� ����!" )
				<< BUILD_LOG( spUser->GetNickName() )
				<< BUILD_LOG( spUser->IsHost() );

			KERM_CHECK_INVALID_ZOMBIE_USER_NOT kPacket;
			kPacket.m_iRoomUID = GetUID();
			SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_CHECK_INVALID_ZOMBIE_USER_NOT, kPacket );
		}
		else
		{
			START_LOG( cerr, L"������ ������ �̻�.!" )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
		}

		if( m_spUserManager->LeaveRoom( iUnitUID ) == false )
		{
			START_LOG( cerr, L"���� ���� �� ������ ����." )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( vecZombieUnit.size() )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		// ��Ʋ�ʵ� ��Ż�� �����ߴٸ� �� ������ �ѹ� ��������
		SendRoomListInfo( KERM_WEDDING_HALL_INFO_NOT::IT_UPDATE );
	}

	// �濡�� ��� ������ �������� ���� �ݴ´�.
	if( IsEmpty() == true )
	{
		StateTransition( KRoomFSM::I_TO_CLOSE );
	}
	else
	{
		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecZombieUnit )
		{
			// ���� ��� �����鿡�� ���� ������ ������.
			KEGS_LEAVE_WEDDING_HALL_NOT kPacketNot;
			kPacketNot.m_iReason = 0;
			kPacketNot.m_cRoomState = GetStateID();
			kPacketNot.m_iUnitUID = iUnitUID;
//			GetRoomSlotInfo( kPacketNot.m_vecSlot );
			BroadCast( ERM_LEAVE_WEDDING_HALL_NOT, kPacketNot );
		}
	}
}

void KWeddingHall::CloseRoom()
{
	// ���� ������ �����Ұ͵��� ��������.
	
	// ��Ʋ�ʵ� ���� ������ ����üũ�� ����
	m_spUserManager->ZU_CheckEnd();

	// ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
	KWeddingHallInfo kHallInfo;
	GetWeddingHallInfo( kHallInfo );
	kHallInfo.m_iRoomUID = 0;
	SetWeddingHallInfo( kHallInfo );
	SendRoomListInfo( KERM_WEDDING_HALL_INFO_NOT::IT_UPDATE );

	//������� ������ �ȵǾ� ������ ���ϴ� �����
	if( GetStateID() == KRoomFSM::S_INIT )
	{
		StateTransition( KRoomFSM::I_TO_WAIT );
	}

	StateTransition( KRoomFSM::I_TO_CLOSE );

	//{{ 2013. 01. 07	������	�濡�� �ʵ�� ���ƿ��� ���ϴ� ���� ����
#ifdef SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
	// �ʱ�ȭ �ϱ� ���� �˻� �ѹ���~
	if( m_spUserManager->GetNumMember() != 0 )
	{
		START_LOG( cout, L"[���] ��ȥ������ �������� �ϴµ� �����Ǿ��־���� ������ ���� ����ִ�! �̷��� ���װ�.." )
			<< BUILD_LOG( m_spUserManager->GetNumMember() )
			<< END_LOG;
	}

	// ���� �ݱ� ���� �ʱ�ȭ �ѹ� ������!
	m_spUserManager->Reset();
#endif SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
	//}}

	KCnBaseRoom::CloseRoom();
}

void KWeddingHall::CheckLoadingSuccess()
{
	// �ƹ��͵� ���� �ʽ��ϴ�. �ε����¸� ���� �ʱ� ����!
}

void KWeddingHall::CheckContinueTimer()
{
    // ��Ƽ�� Ÿ�̸Ӹ� üũ�Ͽ� 9�ʰ� ������ 
}

void KWeddingHall::SendRoomListInfo( IN const int iCode )
{
	KERM_WEDDING_HALL_INFO_NOT kPacket;
	kPacket.m_cType = static_cast<char>(iCode);
	GetWeddingHallInfo( kPacket.m_kHallInfo );
	GetUnitList( kPacket.m_vecJoinUnitList );
	BroadCastAllGSServer( ERM_WEDDING_HALL_INFO_NOT, kPacket );
}

void KWeddingHall::OnLeaveRoom( IN const UidType iLAST_SENDER_UID, IN const UidType iFIRST_SENDER_UID, IN const KERM_LEAVE_ROOM_REQ& kPacket_, IN const u_short usEventIDAck )
{
	KRoom::OnLeaveRoom( iLAST_SENDER_UID, iFIRST_SENDER_UID, kPacket_, usEventIDAck );

	//=== ������ ��Ż ���ó�� ===
	START_LOG( clog, L"=== WEDDING HALL : ��ȥ������ ��Ż�Ѵٰ� ���Ƽ�� ���� �ʽ��ϴ�. ===" )
		<< BUILD_LOG( iFIRST_SENDER_UID )
		<< END_LOG;	

	KEGS_LEAVE_ROOM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
	
	// Ŭ�� �� ������ �ְ� ���� �Ǹ� ������ ���� �Ѵ�.
	kPacket.m_iReason = kPacket_.m_iReason;
	
	KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iFIRST_SENDER_UID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"������ ã�� �� �����ϴ�!" )
			<< BUILD_LOG( iFIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	// ���� ���� ����.
	if( m_spUserManager->LeaveRoom( iFIRST_SENDER_UID ) == false )
	{
		// ���Կ��� ������ ����.
		kPacket.m_iOK = NetError::ERR_SLOT_04;
		if( kPacket_.m_bSendAck )
		{
			SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, usEventIDAck, kPacket );
		}
		return;
	}
	else
	{
		// ��Ʋ�ʵ忡�� ��Ż�ϴ°� �����ߴٸ� �ӽ� �κ��丮�� ����������!
		SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, ERM_CLEAR_TEMP_INVENTORY_NOT );

		if( kPacket_.m_bSendAck )
		{
			SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, usEventIDAck, kPacket );
		}

		// ��Ʋ�ʵ� ��Ż�� �����ߴٸ� �� ������ �ѹ� ��������
		SendRoomListInfo( KERM_WEDDING_HALL_INFO_NOT::IT_UPDATE );
	}

	if( IsEmpty() == false )
	{
		// ���� ��� �����鿡�� ���� ������ ������.
		KEGS_LEAVE_WEDDING_HALL_NOT kPacketNot;
		kPacketNot.m_iReason = usEventIDAck; // ���� �Ǿ����� ACK��ŶŸ���� ����! ���߿� �����ؼ� ó���Ҽ� �ֵ���
		kPacketNot.m_cRoomState = GetStateID();
		kPacketNot.m_iUnitUID = iFIRST_SENDER_UID;
//		GetRoomSlotInfo( kPacketNot.m_vecSlot );
		BroadCast( ERM_LEAVE_WEDDING_HALL_NOT, kPacketNot );
	}

	if( GetStateID() == KRoomFSM::S_LOAD )
	{
		START_LOG( cerr, L"��ȥ���忡���� �ε� ���¸� ���� �ʽ��ϴ�!" )
			<< BUILD_LOG( iFIRST_SENDER_UID )
			<< END_LOG;
	}

	// �濡�� ��� ������ �������� �ϴ� WAIT���·� �̵���Ų��!
	if( IsEmpty() == true )
	{
		StateTransition( KRoomFSM::I_TO_RESULT );
		StateTransition( KRoomFSM::I_TO_WAIT );
		m_tTimer[TE_WAIT_TIMER].restart();
	}
}

void KWeddingHall::SetWeddingHallInfo( IN const KWeddingHallInfo& kInfo )
{
	m_kHallInfo = kInfo;
}

void KWeddingHall::GetWeddingHallInfo( OUT KWeddingHallInfo& kInfo )
{
	kInfo = m_kHallInfo;
}

void KWeddingHall::GetWeddingHallSlotInfo( std::vector< KRoomSlotInfo >& vecSlot, KRoomUserManager::USERLIST_TYPE eType )
{
	std::vector< KRoomSlotInfo > vecTempSlot;
	GetRoomSlotInfo( vecTempSlot, eType );

	BOOST_TEST_FOREACH( KRoomSlotInfo&, kRoomInfo, vecTempSlot )
	{
		if( kRoomInfo.m_kRoomUserInfo.m_nUnitUID > 0)
		{
			vecSlot.push_back( kRoomInfo );
		}
	}
}

void KWeddingHall::GetUnitList( OUT std::vector< UidType >& vecUidList )
{
	m_spUserManager->GetRoomUserList( vecUidList );
}

bool KWeddingHall::CheckWeddingHallInfo( KWeddingHallInfo& kWeddingInfo )
{
	if( m_kHallInfo.m_iWeddingUID != kWeddingInfo.m_iWeddingUID )
	{
		 START_LOG( cerr, L"��ȥ���� UID ���� �ٸ��ϴ�." )
			 << BUILD_LOG( m_kHallInfo.m_iWeddingUID )
			 << BUILD_LOG( kWeddingInfo.m_iWeddingUID )
			 << END_LOG;

		 return false;
	}

	if( m_kHallInfo.m_bSuccess == true )
	{
		START_LOG( cerr, L"��ȥ���� �Ϸ�� ���Դϴ�." )
			<< BUILD_LOG( m_kHallInfo.m_iWeddingUID )
			<< BUILD_LOG( m_kHallInfo.m_bSuccess )
			<< END_LOG;

		return false;
	}

	return true;
}

bool KWeddingHall::IsGroomOrBride( IN UidType iUnitUID )
{
	if( m_kHallInfo.m_iGroom == iUnitUID )
	{
		return true;
	}

	if( m_kHallInfo.m_iBride == iUnitUID )
	{
		return true;
	}

	return false;
}

IMPL_ON_FUNC( ERM_OPEN_WEDDING_HALL_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_INIT ), ERM_OPEN_WEDDING_HALL_ACK, KERM_OPEN_WEDDING_HALL_ACK );

	//{{ 2010. 03. 14  ������	�� �й� ��� ����
	SendRoomCountInfo( CXSLRoom::RT_WEDDING_HALL, true );

    // ��ȥ���� �ɼ� ���� ����.
	kPacket_.m_kWeddingInfo.m_iRoomUID = GetUID();
	SetWeddingHallInfo( kPacket_.m_kWeddingInfo );

    // �� �����̹Ƿ� ���� ������ �ʱ�ȭ �Ѵ�.
    m_spUserManager->Reset();
    m_spUserManager->AssignTeam( -1 );

    // ���Կ� ���� ��û�ڸ� �ִ´�.
    if( m_spUserManager->EnterRoom( kPacket_.m_kRoomUserInfo, false ) == false )
    {
        START_LOG( cerr, L"�� ���� ����. ��Ʋ�ʵ带 �����ϴ°��ε� ���⼭ ���а� ����! ���� �Ͼ���� �ȵǴ� ����!" )
            << BUILD_LOG( FIRST_SENDER_UID )
            << BUILD_LOG( GetUID() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
            << END_LOG;

        kPacket.m_iOK = NetError::GetLastError();
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_WEDDING_HALL_ACK, kPacket );
        return;
    }

    // �� ���¸� wait �� �ٲ۴�.
    StateTransition( KRoomFSM::I_TO_WAIT );
	m_tTimer[TE_WAIT_TIMER].restart();
    m_kTimer[TM_WAIT].restart();

	//����������¸� ����� �ٲ۴�.
	m_spUserManager->SetAllReady( true );

	//gm �̻��̸� ���ͼ����� ip�� �־�����.
	if( m_spUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == true )
	{
		kPacket.m_wstrCNIP = NetCommon::GetLocalIPW();
	}

	// ���� �ʱ�ȭ.
	StartGame();

	// ������ �� �ε� ���� ���� ��ȥ���忡���� �ٷ� ������ �����Ѵ�!
	StartPlay();

    // �� ���� ��û�ڿ��� ack�� ������.
    kPacket.m_iOK = NetError::NET_OK;
	GetRoomInfo( kPacket.m_RoomInfo );
	GetWeddingHallSlotInfo( kPacket.m_vecSlot );
	GetWeddingHallInfo( kPacket.m_kWeddingInfo );

	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
	kPacket.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
	kPacket.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();

    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_WEDDING_HALL_ACK, kPacket );

	//������� �������� üũ����.
	m_spUserManager->ZU_CheckStart();

	// �� ���� ����
	SendRoomListInfo( KERM_WEDDING_HALL_INFO_NOT::IT_UPDATE );

	//////////////////////////////////////////////////////////////////////////	
	// [����!!!] �ݵ�� �渮��Ʈ�� ���� ���� ���Ŀ� �� ��Ŷ�� ������ �Ѵ�! ������ ���͸� ���� ��Ŷ�� �ٽ� ���!
	if( kPacket_.m_kReturnToFieldInfo.m_iRoomUID != 0 )
	{
		KERM_COMPLETE_RETURN_TO_FIELD_NOT kPacketToGS;
		kPacketToGS.m_kReturnToFieldInfo = kPacket_.m_kReturnToFieldInfo;
		SendToGSServer( LAST_SENDER_UID, ERM_COMPLETE_RETURN_TO_FIELD_NOT, kPacketToGS );
	}
}

IMPL_ON_FUNC( ERM_JOIN_WEDDING_HALL_REQ )
{
	KERM_JOIN_WEDDING_HALL_ACK kPacket;
	
	if( GetStateID() != KRoomFSM::S_PLAY  &&  GetStateID() != KRoomFSM::S_WAIT )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_09;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_WEDDING_HALL_ACK, kPacket );
		return;
	}

	// ����� ã�� �Դ��� ����!
	if( CheckWeddingHallInfo( kPacket_.m_kWeddingInfo ) == false )
	{
		START_LOG( cerr, L"��ȥ���忡 �����ߴ�! �� ���״�!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetWeddingHallUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_37;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_WEDDING_HALL_ACK, kPacket );
		return;
	}

	// ��Ʋ�ʵ忡 ���� ó�� �Ѵ�!
	if( m_spUserManager->EnterRoom( kPacket_.m_kRoomUserInfo, false ) == false )
	{
		START_LOG( cerr, L"��ȥ���� ���� ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_WEDDING_HALL_ACK, kPacket );
		return;
	}

	if( m_spUserManager->SetReady( FIRST_SENDER_UID, true ) == false )
	{
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_37;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_WEDDING_HALL_ACK, kPacket );
		return;
	}

	if( m_spUserManager->StartGame( FIRST_SENDER_UID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_37;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_WEDDING_HALL_ACK, kPacket );
		return;
	}

	// �ش� ������ �ٷ� ���� ���� ó�� ����!
	if( m_spUserManager->StartPlay( kPacket_.m_kRoomUserInfo.m_nUnitUID ) == false )
	{
		START_LOG( cerr, L"���� ���� ó�� ����!" )
			<< BUILD_LOG( kPacket_.m_kRoomUserInfo.m_nUnitUID )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_37;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_WEDDING_HALL_ACK, kPacket );
		return;
	}

	// �� ���� ��ȭ.
	if( GetStateID() == KRoomFSM::S_WAIT )
	{
		StateTransition( KRoomFSM::I_TO_LOAD );
		StateTransition( KRoomFSM::I_TO_PLAY );
	}

	// ������ �������� ack�� ������.
	kPacket.m_iOK = NetError::NET_OK;
	GetRoomInfo( kPacket.m_RoomInfo );
	GetWeddingHallInfo( kPacket.m_kWeddingInfo );

	KRoomSlotInfo kInfoMy;
	if( m_spRoomUserManager->GetRoomSlotInfo( FIRST_SENDER_UID, kInfoMy ) == true )
	{
		kPacket.m_vecSlot.push_back( kInfoMy );
	}

	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
	kPacket.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
	kPacket.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_WEDDING_HALL_ACK, kPacket );

	// ������ �������� ��ȥ���忡 �ִ� ��� ������ ������ �ϳ��� ������
	std::vector< KRoomSlotInfo > vecTempSlot;
	GetWeddingHallSlotInfo( vecTempSlot );

	BOOST_TEST_FOREACH( KRoomSlotInfo&, kInfo, vecTempSlot )
	{
		if( kInfo.m_kRoomUserInfo.m_nUnitUID > 0 &&  kInfo.m_kRoomUserInfo.m_nUnitUID != FIRST_SENDER_UID )
		{
			KEGS_JOIN_WEDDING_HALL_NOT kPacketList;
			kPacketList.m_StartPosIndex = 0;
			kPacketList.m_JoinSlot = kInfo;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_WEDDING_HALL_NOT, kPacketList );
		}
	}

	// ���� ��� �������� NOT�� ������.
	KEGS_JOIN_WEDDING_HALL_NOT kPacketNot;
	//080425.hoons.������ ���º� ���������� �����ؼ� �����ش�.
	m_spUserManager->KRoomUserManager::GetRoomSlotInfo( FIRST_SENDER_UID, kPacketNot.m_JoinSlot, KRoomUserManager::UT_GAME );
	kPacketNot.m_StartPosIndex = 0;
	BroadCast_RejectUser( ERM_JOIN_WEDDING_HALL_NOT, kPacketNot, FIRST_SENDER_UID );

	// ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
	SendRoomListInfo( KERM_WEDDING_HALL_INFO_NOT::IT_UPDATE );

	//////////////////////////////////////////////////////////////////////////	
	// [����!!!] �ݵ�� �渮��Ʈ�� ���� ���� ���Ŀ� �� ��Ŷ�� ������ �Ѵ�! ������ ���͸� ���� ��Ŷ�� �ٽ� ���!
	if( kPacket_.m_kReturnToFieldInfo.m_iRoomUID != 0 )
	{
		KERM_COMPLETE_RETURN_TO_FIELD_NOT kPacketToGS;
		kPacketToGS.m_kReturnToFieldInfo = kPacket_.m_kReturnToFieldInfo;
		SendToGSServer( LAST_SENDER_UID, ERM_COMPLETE_RETURN_TO_FIELD_NOT, kPacketToGS );
	}
}

IMPL_ON_FUNC( ERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_ROOM_ACK );
}

_IMPL_ON_FUNC( ERM_LEAVE_WEDDING_HALL_REQ, KEGS_LEAVE_WEDDING_HALL_REQ )
{
	KERM_LEAVE_ROOM_REQ kPacket;
	kPacket.m_iReason = kPacket_.m_iReason;
#ifndef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	kPacket.m_kBattleFieldJoinInfo.m_iBattleFieldID = kPacket_.m_iBattleFieldID;
	kPacket.m_kGamePlayNetworkInfo = kPacket_.m_kGamePlayNetworkInfo;
	kPacket.m_kResultInfo = kPacket_.m_kResultInfo;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket, ERM_LEAVE_WEDDING_HALL_ACK );
}

IMPL_ON_FUNC_NOPARAM( ERM_CHECK_START_WEDDING_REQ )
{
	KERM_CHECK_START_WEDDING_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	if( IsGroomOrBride( FIRST_SENDER_UID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_41;
	}

	// ���������
	bool bSendSucess = false;
	if( IsGroom( FIRST_SENDER_UID ) == true )
	{
		KRoomUserPtr spUser = m_spUserManager->GetUser( m_kHallInfo.m_iBride );
		if( spUser != NULL )
		{
			SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_CHECK_START_WEDDING_NOT, char() );
			bSendSucess = true;
		}
	}

	if( IsBride( FIRST_SENDER_UID ) == true )
	{
		KRoomUserPtr spUser = m_spUserManager->GetUser( m_kHallInfo.m_iGroom );
		if( spUser != NULL )
		{
			SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_CHECK_START_WEDDING_NOT, char() );
			bSendSucess = true;
		}
	}

	if( bSendSucess == false )
	{
		// ���濡�� ��û�� ������ ����
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_42;
	}

	// ��û�ڿ��� ������
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHECK_START_WEDDING_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_START_WEDDING_ASK_NOT, KEGS_START_WEDDING_ASK_ACK )
{
	KEGS_START_WEDDING_AGREE_NOT kPacket;
	kPacket.m_bAgree = kPacket_.m_bAgree;

	if( IsGroom( FIRST_SENDER_UID ) == true )
	{
		KRoomUserPtr spUser = m_spUserManager->GetUser( m_kHallInfo.m_iBride );
		if( spUser != NULL )
		{
			SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_START_WEDDING_AGREE_NOT, kPacket );
		}
	}
	else if( IsBride( FIRST_SENDER_UID ) == true )
	{
		KRoomUserPtr spUser = m_spUserManager->GetUser( m_kHallInfo.m_iGroom );
		if( spUser != NULL )
		{
			SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_START_WEDDING_AGREE_NOT, kPacket );
		}
	}

	// ������ �ݴ� �ߴٸ�
	if( kPacket_.m_bAgree == false )
	{
		return;	
	}
	
	// ���۰� ���ÿ� DB ����� �Ǿ�� �Ѵ�
	KDBE_WEDDING_COMPLETE_REQ kPacketDB;
	kPacketDB.m_iWeddingUID = m_kHallInfo.m_iWeddingUID;
	kPacketDB.m_iGroom = m_kHallInfo.m_iGroom;
	kPacketDB.m_iBride = m_kHallInfo.m_iBride;
	CTime tCurr = CTime::GetCurrentTime();
	kPacketDB.m_wstrWeddingDate	 = (const wchar_t*)tCurr.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
	SendToGameDB( DBE_WEDDING_COMPLETE_REQ, kPacketDB );
}

IMPL_ON_FUNC( DBE_WEDDING_COMPLETE_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KERM_START_WEDDING_NOT kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		KRoomUserPtr spBride = m_spUserManager->GetUser( m_kHallInfo.m_iBride );
		if( spBride != NULL )
		{
			SendToGSCharacter( spBride->GetGSUID(), spBride->GetCID(), ERM_START_WEDDING_NOT, kPacket );
		}

		KRoomUserPtr spGroom = m_spUserManager->GetUser( m_kHallInfo.m_iGroom );
		if( spGroom != NULL )
		{
			SendToGSCharacter( spGroom->GetGSUID(), spGroom->GetCID(), ERM_START_WEDDING_NOT, kPacket );
		}

		return;
	}

	// ��� �������� ������ �˸��� - ���ÿ� ���� ��Ƽ� ������
	{
		KERM_START_WEDDING_NOT kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		kPacket.m_iGroom = m_kHallInfo.m_iGroom;
		kPacket.m_iBride = m_kHallInfo.m_iBride;
		SiCXSLItemManager()->GetWeddingCompleteRewardItem( kPacket.m_mapRewardItem );
		BroadCast( ERM_START_WEDDING_NOT, kPacket );
	}
	
	// �ش� ��ȥ���� �Ϸ� �Ǿ��ٰ� �˷��ش�
	{
		m_kHallInfo.m_bSuccess = true;

		// ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
		SendRoomListInfo( KERM_WEDDING_HALL_INFO_NOT::IT_DELETE );

// [�����һ���]
// 		KRoomUserPtr spHost = m_spUserManager->GetHostUser();
// 		if( spHost != NULL )
// 		{
// 			SendToGSCharacter( spHost->GetGSUID(), spHost->GetCID(), ERM_START_WEDDING_NOT, kPacket );
// 		}
	}

	// ��ȥ �α� ���
	{
		KRoomUserPtr spBride = m_spUserManager->GetUser( m_kHallInfo.m_iBride );
		if( spBride != NULL )
		{
			// �α� ���(��)
			KDBE_WRITE_RELATIONSHIP_INFO_LOG_REQ kLog;
			kLog.m_iUserUID = spBride->GetUserUID();
			kLog.m_iUnitUID = spBride->GetCID();
			kLog.m_ucLevel = spBride->GetLevel();
			kLog.m_cUnitClass = spBride->GetUnitClass_LUA();
			kLog.m_cCoupleType = 2;
			kLog.m_wstrRegDate = kPacket_.m_wstrWeddingDate;
			SendToLogDB( DBE_WRITE_RELATIONSHIP_INFO_LOG_REQ, kLog );

		}

		KRoomUserPtr spGroom = m_spUserManager->GetUser( m_kHallInfo.m_iGroom );
		if( spGroom != NULL )
		{
			// �α� ���(��)
			KDBE_WRITE_RELATIONSHIP_INFO_LOG_REQ kLog;
			kLog.m_iUserUID = spGroom->GetUserUID();
			kLog.m_iUnitUID = spGroom->GetCID();
			kLog.m_ucLevel = spGroom->GetLevel();
			kLog.m_cUnitClass = spGroom->GetUnitClass_LUA();
			kLog.m_cCoupleType = 2;
			kLog.m_wstrRegDate = kPacket_.m_wstrWeddingDate;
			SendToLogDB( DBE_WRITE_RELATIONSHIP_INFO_LOG_REQ, kLog );
		}
	}
}

_IMPL_ON_FUNC( ERM_SQUARE_UNIT_SYNC_DATA_REQ, KEGS_SQUARE_UNIT_SYNC_DATA_REQ )
{
	// �ƹ��͵� ���� ����
}

_IMPL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType )
{
	std::vector<KRoomUserManager::KLeaveUserInfo>	vecDelUserUID;
	const int iCount = m_spUserManager->DeleteUserByGsUID( kPacket_, vecDelUserUID );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_ )
		<< BUILD_LOG( iCount )
		<< BUILD_LOG( m_spUserManager->GetNumMember() );

	//����濡�� ������ ������ ������ ����.
	if( iCount == 0 )
		return;

	BOOST_TEST_FOREACH( const KRoomUserManager::KLeaveUserInfo&, kInfo, vecDelUserUID )
	{
		m_spUserManager->LeaveRoom( kInfo.m_iUnitUID );
	}

	BOOST_TEST_FOREACH( const KRoomUserManager::KLeaveUserInfo&, kInfo, vecDelUserUID )
	{
		if( IsEmpty() == false )
		{
			// ���� ��� �����鿡�� ���� ������ ������.
			KEGS_LEAVE_WEDDING_HALL_NOT kPacketNot;
			kPacketNot.m_cRoomState		= GetStateID();
			kPacketNot.m_iUnitUID		= kInfo.m_iUnitUID;
//			GetRoomSlotInfo( kPacketNot.m_vecSlot );
			BroadCast( ERM_LEAVE_WEDDING_HALL_NOT, kPacketNot );
		}
	}

	if( GetStateID() == KRoomFSM::S_LOAD  ||  GetStateID() == KRoomFSM::S_RESULT )
	{
		START_LOG( cerr, L"��ȥ������ ������� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( GetStateIDString() )
			<< BUILD_LOG( iCount )
			<< END_LOG;
	}

	// �濡�� ��� ������ �������� �ϴ� WAIT���·� �ٲ۴�.
	if( IsEmpty() == true )
	{
		StateTransition( KRoomFSM::I_TO_RESULT );
		StateTransition( KRoomFSM::I_TO_WAIT );
		m_tTimer[TE_WAIT_TIMER].restart();

		// ��Ʋ�ʵ� ��Ż�� �����ߴٸ� �� ������ �ѹ� ��������
		SendRoomListInfo( KERM_WEDDING_HALL_INFO_NOT::IT_DELETE );
	}
	else
	{
		// ��Ʋ�ʵ� ��Ż�� �����ߴٸ� �� ������ �ѹ� ��������
		SendRoomListInfo( KERM_WEDDING_HALL_INFO_NOT::IT_UPDATE );
	}
}
#endif SERV_RELATIONSHIP_SYSTEM
//}
