#include ".\square.h"

#include "Room/BaseRoomFSM.h"
#include "NetError.h"
#include "x2data/XSLRoom.h"

#define CLASS_TYPE KSquare

NiImplementRTTI( KSquare, KCnBaseRoom );

ImplToStringW( KSquare )
{
	return START_TOSTRING_PARENTW( KCnBaseRoom );
}

KSquare::KSquare(void)
:
m_bIsDefault(false)
//{{ 2010. 11. 1	������	���� ����Ʈ ����
#ifdef SERV_SQUARE_LIST_SERV_UID
, m_cSortIndex( 0 )
#endif SERV_SQUARE_LIST_SERV_UID
//}}
{
	m_kSquareUserManager.Init( SQUARE_MAX_SLOT );
}

KSquare::~KSquare(void)
{
}

void KSquare::Tick()
{
	KCnBaseRoom::Tick();

	switch( GetStateID() )
	{
	case KBaseRoomFSM::S_INIT:
		break;

	case KBaseRoomFSM::S_WAIT:
		break;

	case KBaseRoomFSM::S_CLOSE:
		m_kSquareUserManager.ZU_CheckEnd();
		CloseRoom();
		//{{ 2010. 03. 14  ������	�� �й� ��� ����
#ifdef SERV_ROOM_COUNT
		SendRoomCountInfo( CXSLRoom::RT_SQUARE, false );
#endif SERV_ROOM_COUNT
		//}}
		break;

	default:
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( GetStateID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		break;
	}

	std::vector<UidType> vecZombieUnit;
	if( m_kSquareUserManager.ZU_Tick( vecZombieUnit ) == true )
	{
		DeleteZombie( vecZombieUnit );

		START_LOG( cout2, L"���� �������� ����.!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( vecZombieUnit.size() )
			<< END_LOG;
	}
}

void KSquare::ProcessEvent( const KEventPtr& spEvent_ )
{
	m_kSquareUserManager.ZU_Refresh( spEvent_->m_anTrace[0], spEvent_->m_usEventID ); //080221.hoons : ���� ��������üũ.
	DWORD dwElapTime = ::GetTickCount();  // 040428. ��Ŷ ó�� �ҿ�ð��� ���Ѵ�.

	switch( spEvent_->m_usEventID )
	{
	CASE( ERM_OPEN_SQUARE_NOT );
	CASE( ERM_JOIN_SQUARE_REQ );
   _CASE( ERM_SQUARE_UNIT_SYNC_DATA_REQ, KEGS_SQUARE_UNIT_SYNC_DATA_REQ );
   _CASE( ERM_LEAVE_SQUARE_REQ, KEGS_LEAVE_SQUARE_REQ );
   _CASE( ERM_CHAT_REQ, KEGS_CHAT_REQ );
   _CASE( ERM_CHECK_INVALID_USER_NOT, UidType );
    CASE( ERM_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT );

    CASE( ERM_REQUEST_TRADE_NOT );
    CASE( ERM_REQUEST_TRADE_REPLY_NOT );

   _CASE( ERM_SQUARE_LIST_INFO_NOT, UidType );

    CASE_NOPARAM( ERM_CHECK_OPEN_PERSONAL_SHOP_REQ );
   _CASE( ERM_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT, KSquarePersonalShopInfo );
   _CASE( ERM_CHECK_JOIN_PERSONAL_SHOP_REQ, KEGS_JOIN_PERSONAL_SHOP_REQ );

    CASE_NOPARAM( ERM_TRADE_START_NOT );
    CASE_NOPARAM( ERM_TRADE_FINISH_NOT );

	//{{ 2009. 9. 23  ������	1������ĳ����
   _CASE( ERM_CHANGE_SQUARE_USER_INFO_NOT, KSquareUserInfo );
	//}}
   //{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
   CASE( ERM_SQUARE_UNIT_REFRESH_ED_NOT );
#endif SERV_ED_MONITORING_IN_GAME
   //}}

	default:
		START_LOG( cerr, L"���� ��Ŷ�� ���忡 ���ƿ´� ;��;" )
			<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
			<< END_LOG;
		break;
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG_WITH_NAME( cwarn )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}
}

void KSquare::BroadCastID( u_short usEventID )
{
	BroadCast( usEventID, char() );
}

void KSquare::_BroadCast( const KEvent& kEvent_ )
{
    std::map< UidType, std::set< UidType > > mapUserList;
    m_kSquareUserManager.GetUserList( mapUserList );

    // GameServer�� ���� loop
    std::map< UidType, std::set< UidType > >::iterator mit;
    for( mit = mapUserList.begin(); mit != mapUserList.end(); ++mit )
    {
        KEventPtr spEvent( kEvent_.Clone() );
        spEvent->m_anTrace[0] = mit->first;
        spEvent->m_anTrace[1] = -1;

        // User ������ ���� loop
        std::set< UidType >::iterator sit;
        for( sit = mit->second.begin(); sit != mit->second.end(); ++sit )
        {
            LIF( spEvent->m_kDestPerformer.AddUID( *sit ) );
        }

        KncSend( PI_CN_ROOM, GetUID(), spEvent );
    }
}

void KSquare::SendSquareListInfo( int iCode )
{
	KERM_SQUARE_LIST_INFO_NOT kPacket;
	kPacket.m_iCode = iCode;
	GetSquareInfo( kPacket.m_kSquareInfo );

	//{{ 2010. 05. 12  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	BroadCastSameGroupGS( ERM_SQUARE_LIST_INFO_NOT, kPacket );
#else
	BroadCastAllGSServer( ERM_SQUARE_LIST_INFO_NOT, kPacket );
#endif SERV_INTEGRATION
	//}}
}

void KSquare::GetSquareInfo( KSquareInfo& kSquareInfo )
{
	kSquareInfo.m_cSquareType		= m_cSquareType;
	//{{ 2010. 11. 1	������	���� ����Ʈ ����
#ifdef SERV_SQUARE_LIST_SERV_UID
	kSquareInfo.m_cSortIndex		= m_cSortIndex;
#endif SERV_SQUARE_LIST_SERV_UID
	//}}
	kSquareInfo.m_iSquareUID		= GetUID();
	kSquareInfo.m_JoinSlot			= static_cast<char>(m_kSquareUserManager.GetNumMember());
	kSquareInfo.m_MaxSlot			= static_cast<char>(m_kSquareUserManager.GetNumMaxUser());
	kSquareInfo.m_wstrRoomName		= GetName();
	kSquareInfo.m_RelayIP			= m_wstrUDPRelayIP;
	kSquareInfo.m_Port				= static_cast<int>(m_usUDPRelayPort);
}

void KSquare::DeleteZombie( std::vector<UidType>& vecZombieUnit )
{
	START_LOG( cout2, L"������ ���� ������������.!" )
		<< BUILD_LOG( vecZombieUnit.size() )
		<< END_LOG;

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecZombieUnit )
	{
		KSquareUserPtr spUser = m_kSquareUserManager.GetUser( iUnitUID );
		if( spUser == NULL )
		{
			START_LOG( cerr, L"���� �������� ������ �̻�.!" )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		KERM_CHECK_INVALID_ZOMBIE_USER_NOT kPacket;
		kPacket.m_iRoomUID = GetUID();
		SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_CHECK_INVALID_ZOMBIE_USER_NOT, kPacket );

		if( m_kSquareUserManager.LeaveSquare( iUnitUID ) == false )
		{
			START_LOG( cerr, L"�������� ���� ������ ����." )
				<< END_LOG;
			continue;
		}

		if( IsEmpty() == false )
		{
			KEGS_LEAVE_SQUARE_NOT kNot;
			kNot.m_iUnitUID		= spUser->GetCID();
			kNot.m_iReason		= NetError::NOT_LEAVE_ROOM_REASON_02;
			BroadCast( ERM_LEAVE_SQUARE_NOT, kNot );
		}
	}

	//TO DO..
	//���� �⺻�������� �ƴҰ�� ��� ������ �������� close square ��ɱ���
	if( m_bIsDefault != false )
	{
	}

	// ��� ���� ������ ���� ����Ʈ�� �����϶�� ������.
	SendSquareListInfo( NetError::ERR_ROOM_15 );
}

IMPL_ON_FUNC( ERM_OPEN_SQUARE_NOT )
{
	if( GetStateID() != KBaseRoomFSM::S_INIT )
	{
		START_LOG( cerr, L"������ �ʱ� ���°� �ƴ�." )
			<< BUILD_LOG( GetStateID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;

		return;
	}

	//{{ 2010. 03. 14  ������	�� �й� ��� ����
#ifdef SERV_ROOM_COUNT
	SendRoomCountInfo( CXSLRoom::RT_SQUARE, true );
#endif SERV_ROOM_COUNT
	//}}

	SetName( kPacket_.m_wstrSquareName );
	SetType( ( char )kPacket_.m_iSquareType );
	//{{ 2010. 11. 1	������	���� ����Ʈ ����
#ifdef SERV_SQUARE_LIST_SERV_UID
	SetSortIndex( kPacket_.m_cSortIndex );
#endif SERV_SQUARE_LIST_SERV_UID
	//}}
	SetDefault( true );
	
	StateTransition( KBaseRoomFSM::I_TO_WAIT );

	SendSquareListInfo( NetError::ERR_ROOM_14 );

	m_kSquareUserManager.ZU_CheckStart();
}

IMPL_ON_FUNC( ERM_JOIN_SQUARE_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KBaseRoomFSM::S_WAIT ), ERM_JOIN_SQUARE_ACK, KEGS_JOIN_SQUARE_ACK );

	if( m_kSquareUserManager.EnterSquare( kPacket_ ) == true )
	{
		kPacket.m_iOK	= NetError::NET_OK;
		GetSquareInfo( kPacket.m_kSquareInfo );
		m_kSquareUserManager.GetUserInfoList( kPacket.m_vecUserInfo );

		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_SQUARE_ACK, kPacket );

		KEGS_JOIN_SQUARE_NOT kNot;
		kNot.m_kUserInfo = kPacket_;

		BroadCast( ERM_JOIN_SQUARE_NOT, kNot );

		// ��� ���� ������ ���� ����Ʈ�� �����϶�� ������.
		SendSquareListInfo( NetError::ERR_ROOM_15 );

		return;
	}


	kPacket.m_iOK	= NetError::ERR_SQUARE_01;

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_SQUARE_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_SQUARE_UNIT_SYNC_DATA_REQ, KEGS_SQUARE_UNIT_SYNC_DATA_REQ )
{
	VERIFY_STATE( ( 1, KBaseRoomFSM::S_WAIT ) );

	//{{ 2010. 8. 2	������	���� ��ũ ����ȭ
#ifdef SERV_SQUARE_SYNC
	if( m_kSquareUserManager.CompareSquareSyncPacket( FIRST_SENDER_UID, kPacket_ ) == false )
	{
		START_LOG_WITH_NAME( cwarn )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;        
	}
#endif SERV_SQUARE_SYNC
	//}}

    // 070730. jseop.
    // Ŭ���̾�Ʈ�� ERM_SQUARE_UNIT_SYNC_DATA_REQ�� ���� ���� ���忡�� ������ SetSyncPos �Լ��� false�� �����Ѵ�.
    // cerr -> cwarn
	if( m_kSquareUserManager.SetSyncPos( FIRST_SENDER_UID, kPacket_.m_vPos ) == false )
	{
		START_LOG_WITH_NAME( cwarn )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	//std::vector<KSquareUserPtr>	vecSquareUser;
	//if( m_kSquareUserManager.GetSortSyncPos( FIRST_SENDER_UID, vecSquareUser ) == false )
	//{
	//	START_LOG_WITH_NAME( cerr )
	//		<< BUILD_LOG( FIRST_SENDER_UID )
	//		<< END_LOG;
	//	return;
	//}

	KEGS_SQUARE_UNIT_SYNC_DATA_NOT kNot;
	kNot.m_bIsRight				= kPacket_.m_bIsRight;
	kNot.m_LastTouchLineIndex	= kPacket_.m_LastTouchLineIndex;
	kNot.m_StateChangeNum		= kPacket_.m_StateChangeNum;
	kNot.m_StateID				= kPacket_.m_StateID;
	kNot.m_UnitUID				= FIRST_SENDER_UID;
	kNot.m_vPos					= kPacket_.m_vPos;

	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
	kNot.m_iED					= m_kSquareUserManager.GetUserED( FIRST_SENDER_UID );
#endif SERV_ED_MONITORING_IN_GAME
	//}}


	//for( int i = 0; i < (int)vecSquareUser.size(); ++i )
	//{
	//	SendToGSCharacter( vecSquareUser[i]->GetGSUID(), vecSquareUser[i]->GetCID(), ERM_SQUARE_UNIT_SYNC_DATA_NOT, kNot );
	//}

	BroadCast( ERM_SQUARE_UNIT_SYNC_DATA_NOT, kNot );
}

_IMPL_ON_FUNC( ERM_LEAVE_SQUARE_REQ, KEGS_LEAVE_SQUARE_REQ )
{
	// �ǵ����� Leave ���� ������ ������ �� �� �� �� Leave �ϰ� �Ǵ� ��찡 �ִ�.
	//KEGS_LEAVE_SQUARE_ACK kPacket;
	//kPacket.m_iOK	= NetError::NET_OK;
	//if( GetStateID() != KBaseRoomFSM::S_WAIT )
	//{
	//	kPacket.m_iOK = NetError::ERR_ROOM_09;
	//	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_SQUARE_ACK, kPacket );
	//	return;
	//}

    VERIFY_ROOM_STATE( ( 1, KBaseRoomFSM::S_WAIT ), ERM_LEAVE_SQUARE_ACK, KEGS_LEAVE_SQUARE_ACK );
    kPacket.m_iOK	= NetError::NET_OK;

	if( m_kSquareUserManager.LeaveSquare( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"���� ������ ����." );
		kPacket.m_iOK = NetError::ERR_SQUARE_02;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_SQUARE_ACK, kPacket );
		return;
	}

	//������ ���Ƿ� ������쿡�� ack�� �����ش�.
	if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_00 )
	{
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_SQUARE_ACK, kPacket );
	}

	if( IsEmpty() == false )
	{
		KEGS_LEAVE_SQUARE_NOT kNot;
		kNot.m_iUnitUID		= FIRST_SENDER_UID;
		kNot.m_iReason		= kPacket_.m_iReason;

		BroadCast( ERM_LEAVE_SQUARE_NOT, kNot );
	}

	//TO DO..
	//���� �⺻�������� �ƴҰ�� ��� ������ �������� close square ��ɱ���
	if( m_bIsDefault != false )
	{
	}

	// ��� ���� ������ ���� ����Ʈ�� �����϶�� ������.
	SendSquareListInfo( NetError::ERR_ROOM_15 );
}

//{{ 2007. 8. 20  ������  ä�� ����
_IMPL_ON_FUNC( ERM_CHAT_REQ, KEGS_CHAT_REQ )
{
	VERIFY_ROOM_STATE_CHAT_ONLY( ( 1, KBaseRoomFSM::S_WAIT ), ERM_CHAT_ACK, KERM_CHAT_ACK );

    if( kPacket_.m_cChatPacketType != KEGS_CHAT_REQ::CPT_TOTAL )
	{
		START_LOG( cerr, L"�Ϲ�ä�� ��ŶŸ���� �ƴѵ� ����Գ�?" )
			<< BUILD_LOGc( kPacket_.m_cChatPacketType )
			<< END_LOG;
		return;
	}
	
	// ���� ������� ack�� ������.
	kPacket.m_iOK				= NetError::NET_OK;
	kPacket.m_cChatPacketType	= kPacket_.m_cChatPacketType;

	// ��� �������� ä�� ������ ������.
	KEGS_CHAT_NOT kPacketBR;	
	kPacketBR.m_SenderUnitUID	= FIRST_SENDER_UID;
	kPacketBR.m_RecieverUnitUID	= kPacket_.m_ToUnitUID;
	kPacketBR.m_cChatPacketType = kPacket_.m_cChatPacketType;
	kPacketBR.m_cRoomChatType	= kPacket_.m_cRoomChatType;
	kPacketBR.m_wstrMsg			= kPacket_.m_wstrMsg;	

	if ( (CXSLRoom::ROOM_CHAT_TYPE)kPacketBR.m_cRoomChatType == CXSLRoom::RCT_ALL )
	{
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHAT_ACK, kPacket );
		BroadCast( ERM_CHAT_NOT, kPacketBR );
	}
	else if ( (CXSLRoom::ROOM_CHAT_TYPE)kPacketBR.m_cRoomChatType == CXSLRoom::RCT_WHISPER )
	{
		UidType nGS;
		if( m_kSquareUserManager.GetSquareUserGS( kPacket_.m_ToUnitUID, nGS ) )
		{
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHAT_ACK, kPacket );
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHAT_NOT, kPacketBR );
			SendToGSCharacter( nGS, kPacket_.m_ToUnitUID, ERM_CHAT_NOT, kPacketBR );
		}
		else
		{
			kPacket.m_iOK = NetError::ERR_ROOM_23;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHAT_ACK, kPacket );
		}		
	}
	else
	{
		START_LOG( cerr, L"�̻��� ä�� Ÿ��." )
			<< BUILD_LOGc( kPacketBR.m_cRoomChatType )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ROOM_24;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHAT_ACK, kPacket );
	}
}
//}}

_IMPL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType )
{
	std::vector<UidType>	vecDelUserUID;
	int iCount = m_kSquareUserManager.DeleteUserByGsUID( kPacket_, vecDelUserUID );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_ )
		<< BUILD_LOG( iCount )
		<< BUILD_LOG( m_kSquareUserManager.GetNumMember() );

	//����濡�� ������ ������ ������ ����.
	if( iCount == 0 )
		return;

	//������ �������� �� ����
	for( int i = 0; i < (int)vecDelUserUID.size(); i++ )
	{
		m_kSquareUserManager.LeaveSquare( vecDelUserUID[i] );
	}

	//���� �������� ������ ���� ���� ������ �˸���.
	for( int i = 0; i < (int)vecDelUserUID.size(); i++ )
	{
		if( IsEmpty() == false )
		{
			KEGS_LEAVE_SQUARE_NOT kNot;
			kNot.m_iUnitUID		= vecDelUserUID[i];
			kNot.m_iReason		= NetError::NOT_LEAVE_ROOM_REASON_01;

			BroadCast( ERM_LEAVE_SQUARE_NOT, kNot );
		}
	}
}

IMPL_ON_FUNC( ERM_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT )
{
	VERIFY_STATE( ( 1, KBaseRoomFSM::S_WAIT ) );

	//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
	m_kSquareUserManager.SetUserEquippedTitle( FIRST_SENDER_UID, kPacket_.m_iEquippedTitleID );
#else
	m_kSquareUserManager.SetUserEquippedTitle( FIRST_SENDER_UID, kPacket_.m_sEquippedTitleID );
#endif SERV_TITLE_DATA_SIZE
	//}}
	m_kSquareUserManager.SetUserEquipItem( FIRST_SENDER_UID, kPacket_.m_vecInventorySlotInfo );
	BroadCast( ERM_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_SQUARE_LIST_INFO_NOT, UidType )
{
	VERIFY_STATE( ( 1, KBaseRoomFSM::S_WAIT ) );

	//KERM_SQUARE_LIST_INFO_NOT kPacket;
	//kPacket.m_iCode = NetError::ERR_ROOM_14;
	//GetSquareInfo( kPacket.m_kSquareInfo );
	//SendToGSServer( kPacket_, ERM_SQUARE_LIST_INFO_NOT, kPacket );
	SendSquareListInfo( NetError::ERR_ROOM_14 );
}

IMPL_ON_FUNC( ERM_REQUEST_TRADE_NOT )
{
	VERIFY_STATE( ( 1, KBaseRoomFSM::S_WAIT ) );

	UidType iGSUID;
	if( !m_kSquareUserManager.GetSquareUserGS( kPacket_.m_iReceiverUID, iGSUID ) )
	{
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( kPacket_.m_iSenderUID )
			<< BUILD_LOG( kPacket_.m_iReceiverUID )
			<< BUILD_LOG( kPacket_.m_wstrSenderNickName )
			<< END_LOG;
		return;
	}

	//�ŷ�����ڰ� ���λ��� �������̸� �ŷ��� �������� �ʴ´�.
	if( m_kSquareUserManager.GetPersonalShopState( kPacket_.m_iReceiverUID ) > CXSLSquareUnit::PSS_NONE )
	{
		START_LOG( cwarn, L"���λ��� �������� �������� ���ΰŷ� ��û�� ��.!" )
			<< BUILD_LOG( kPacket_.m_iSenderUID )
			<< BUILD_LOG( kPacket_.m_iReceiverUID )
			<< BUILD_LOG( kPacket_.m_wstrSenderNickName )
			<< END_LOG;
		return;

	}

	SendToGSCharacter( iGSUID, kPacket_.m_iReceiverUID, ERM_REQUEST_TRADE_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_REQUEST_TRADE_REPLY_NOT )
{
	VERIFY_STATE( ( 1, KBaseRoomFSM::S_WAIT ) );

	UidType iGSUID;
	if( !m_kSquareUserManager.GetSquareUserGS( kPacket_.m_iReceiverUID, iGSUID ) )
	{
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( kPacket_.m_iReceiverUID )
			<< END_LOG;
		return;
	}

	SendToGSCharacter( iGSUID, kPacket_.m_iReceiverUID, ERM_REQUEST_TRADE_REPLY_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( ERM_CHECK_OPEN_PERSONAL_SHOP_REQ )
{
	VERIFY_STATE( ( 1, KBaseRoomFSM::S_WAIT ) );

	KERM_CHECK_OPEN_PERSONAL_SHOP_ACK kPacket;
	KSquarePersonalShopInfo kInfo;

	kPacket.m_iOK = NetError::NET_OK;

	//���������� �Ҽ��ִ���..
	//if( m_kSquareUserManager.IsCreatePersonalShop() == false )
	//{
	//	kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_02;

	//	goto end_proc;
	//}

	if( m_kSquareUserManager.GetPersonalShopState( FIRST_SENDER_UID ) != CXSLSquareUnit::PSS_NONE )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_22;

		START_LOG( cerr, L"�������� üũ�� �ٽ� üũ ��Ŷ�̿�.!" )
			<< BUILD_LOG( FIRST_SENDER_UID );

		goto end_proc;
	}

	//�ȴٸ� ���� ����üũ..	
	kInfo.m_cUpdateType = KSquarePersonalShopInfo::UT_STATE_NAME;
	kInfo.m_cPersonalShopState = CXSLSquareUnit::PSS_WAIT;

	if( m_kSquareUserManager.SetPersonalShopData( FIRST_SENDER_UID, kInfo ) == false )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_03;

		START_LOG( cerr, L"���������� ã�� ���� ���λ��� ���۽���.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		goto end_proc;
	}

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHECK_OPEN_PERSONAL_SHOP_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT, KSquarePersonalShopInfo )
{
	VERIFY_STATE( ( 1, KBaseRoomFSM::S_WAIT ) );

	KEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT kNot;

	if( m_kSquareUserManager.SetPersonalShopData( FIRST_SENDER_UID, kPacket_, kNot ) == false )
	{
		START_LOG( cerr, L"���������� ã�� ���� ���λ��� ���۽���.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOGc( kPacket_.m_cPersonalShopState )
			<< BUILD_LOG( kPacket_.m_iPersonalShopUID )
			<< BUILD_LOG( kPacket_.m_wstrPersonalShopName )
			<< END_LOG;
	}
	else
	{
		//���������鿡�� �˷��ش�.
		//KEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT kNot;
		//kNot.m_iUnitUID = FIRST_SENDER_UID;
		//kNot.m_cPersonalShopState = kPacket_.m_cPersonalShopState;

		//switch( kPacket_.m_cUpdateType )
		//{

		//}
		//kNot.m_cPersonalShopType  = kPacket_.m_cPersonalShopType;

		//if( kNot.m_cPersonalShopState == CXSLSquareUnit::PSS_SHOP )
		//{
		//	kNot.m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
		//}

		BroadCast( ERM_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT, kNot );

		return;
	}

	//To do..
	//���� �ش������� ��ã�� ��찡 ����� gsuser�� ����Ͽ� ��ġ������ ����ؾ��Ѵ�.
}

_IMPL_ON_FUNC( ERM_CHECK_JOIN_PERSONAL_SHOP_REQ, KEGS_JOIN_PERSONAL_SHOP_REQ )
{
	VERIFY_STATE( ( 1, KBaseRoomFSM::S_WAIT ) );

	KERM_CHECK_JOIN_PERSONAL_SHOP_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iPersonalShopUID = 0;

	char cShopState = m_kSquareUserManager.GetPersonalShopState( kPacket_.m_iUnitUID );
	if( cShopState == CXSLSquareUnit::PSS_SHOP )
	{
		kPacket.m_iPersonalShopUID = m_kSquareUserManager.GetPersonalShopUID( kPacket_.m_iUnitUID );

		if( kPacket.m_iPersonalShopUID != 0 )
		{
			goto end_proc;
		}
		else
		{
			START_LOG( cerr, L"�����������ε� ���� UID�� �߸��Ǿ���.!" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOGc( cShopState )
				<< END_LOG;
		}
	}
	else
	{
		START_LOG( cerr, L"�����غ� �ȵǾ��ִµ� ���� ��û�̿�.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOGc( cShopState )
			<< END_LOG;
	}

	kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_11;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHECK_JOIN_PERSONAL_SHOP_ACK, kPacket );
}

IMPL_ON_FUNC_NOPARAM( ERM_TRADE_START_NOT )
{

}

IMPL_ON_FUNC_NOPARAM( ERM_TRADE_FINISH_NOT )
{

}

//{{ 2009. 9. 23  ������	1������ĳ����
_IMPL_ON_FUNC( ERM_CHANGE_SQUARE_USER_INFO_NOT, KSquareUserInfo )
{
	VERIFY_STATE( ( 1, KBaseRoomFSM::S_WAIT ) );

	m_kSquareUserManager.SetSquareUserInfo( kPacket_ );

	// ������
	BroadCast( ERM_CHANGE_SQUARE_USER_INFO_NOT, kPacket_ );
}
//}}

//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
IMPL_ON_FUNC( ERM_SQUARE_UNIT_REFRESH_ED_NOT )
{
	VERIFY_STATE( ( 1, KBaseRoomFSM::S_WAIT ) );

	KSquareUserPtr spUser = m_kSquareUserManager.GetUser( kPacket_.m_iUnitUID );
	if( !spUser )
	{
		START_LOG( clog, L"���忡 �������� �ʴ� �����Դϴ�." )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		return;
	}

	spUser->SetED( kPacket_.m_iED );
}
#endif SERV_ED_MONITORING_IN_GAME
//}}