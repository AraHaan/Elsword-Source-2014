#include "GSUser.h"
#include ".\UserAbuserManager.h"
#include "AbuserLog/KAbuserLogManager.h"
//{{ 2011. 09. 08	������	������ ��Ŷ ����͸�
#ifdef SERV_AUTO_HACK_PACKET_MORNITOING
	#include "GameSysVal/GameSysVal.h"
#endif SERV_AUTO_HACK_PACKET_MORNITOING
//}}

//{{ 2012. 10. 15	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}

#ifdef SERV_HACKING_USER_CHECK_COUNT// �۾���¥: 2013-06-01	// �ڼ���
	#include "Enum/Enum.h"
#endif // SERV_HACKING_USER_CHECK_COUNT

//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
KUserAbuserManager::KUserAbuserManager(void)
{
	Clear();
}

KUserAbuserManager::~KUserAbuserManager(void)
{
}

void KUserAbuserManager::Clear( bool bOnlyCharAbuser /*= false*/ )
{
	if( !bOnlyCharAbuser )
	{
		for( int i = 0; i < AT_MAX; ++i )
		{
			m_bIsAccountAbuser[i] = false;
		}

		//{{ 2011. 03. 02	������	���� �ŷ� ����
#ifdef SERV_HACK_USER_TRADE_BLOCK
		m_bIsTradeBlock = false;
#endif SERV_HACK_USER_TRADE_BLOCK
		//}}
#ifdef SERV_NEW_UNIT_TRADE_LIMIT
		m_bIsTradeBlockUnit = false;
#endif SERV_NEW_UNIT_TRADE_LIMIT
	}

	for( int i = 0; i < CAT_MAX; ++i )
	{
		m_bIsCharAbuser[i] = false;
	}

	//{{ 2010. 9. 8	������	��Ʈ��ũ Ʈ���� ���� ���� ����͸�
#ifdef SERV_TRAFFIC_USER_MONITORING
	m_mapTrafficEventCount.clear();
#endif SERV_TRAFFIC_USER_MONITORING
	//}}
	//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	m_iGetItemCount = 0;
	m_iGetItemCountMin = INT_MAX;
	m_iGetItemCountMax = INT_MAX;
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}
#ifdef SERV_AUTO_HACK_CHECK_LEVEL_FIX
	m_iLastLevelUpUID = 0;
#endif // SERV_AUTO_HACK_CHECK_LEVEL_FIX
}

void KUserAbuserManager::InitAccountAbuser( IN KGSUserPtr spUser )
{
	m_bIsAccountAbuser[AT_MORNITORING_ABUSER] = SiKAbuserLogManager()->IsMornitoringAbuser( spUser->GetUID() );	

	//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	m_iGetItemCountMin = SiKAbuserLogManager()->GetItemCountMin();
	m_iGetItemCountMax = SiKAbuserLogManager()->GetItemCountMax();
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}

	//if( m_bIsAccountAbuser[AT_MORNITORING_ABUSER] )
	//{
	//	std::wstring wstrIPAddr = KncUtil::toWideString( GetIPStr(), strlen( GetIPStr() ) );

	//	KDBE_SEND_PHONE_MSG_NOT kPacketNot;
	//	kPacketNot.m_wstrSMSMessage  = L"����� ���� [";
	//	kPacketNot.m_wstrSMSMessage += GetName();
	//	kPacketNot.m_wstrSMSMessage += L"]�� �����Ͽ����ϴ�! : ";
	//	kPacketNot.m_wstrSMSMessage += wstrIPAddr;

	//	SiKAbuserLogManager()->GetAbuserNotifyPhoneNumList( kPacketNot.m_wstrPhoneNum );

	//	SendToSMSDB( DBE_SEND_PHONE_MSG_NOT, kPacketNot );
	//}
}

void KUserAbuserManager::InitCharAbuser( IN KGSUserPtr spUser )
{
	if( spUser == NULL )
	{
		START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
			<< END_LOG;
		return;
	}

	m_bIsCharAbuser[CAT_ITEM_ABUSER]	= SiKAbuserLogManager()->IsItemAbuser( spUser->GetCharUID() );
	m_bIsCharAbuser[CAT_ENCHANT_ABUSER] = SiKAbuserLogManager()->IsEnchantAbuser( spUser->GetCharUID() );
	m_bIsCharAbuser[CAT_ED_ABUSER]		= SiKAbuserLogManager()->IsEDAbuser( spUser->GetCharUID() );
	//{{ 2010. 9. 8	������	��Ʈ��ũ Ʈ���� ���� ���� ����͸�
#ifdef SERV_TRAFFIC_USER_MONITORING
	m_bIsCharAbuser[CAT_TRAFFIC_ABUSER] = SiKAbuserLogManager()->IsTrafficAbuser( spUser->GetCharUID() );
#endif SERV_TRAFFIC_USER_MONITORING
	//}}	
}

void KUserAbuserManager::RegEDAbuser( IN KGSUserPtr spUser )
{
	if( spUser == NULL )
	{
		START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
			<< END_LOG;
		return;
	}

	if( spUser->GetAuthLevel() > SEnum::UAL_NORMAL )
	{
		// ��ڴ� ���ش�~
		return;
	}
#ifndef SERV_PREVENT_ABUSER_LOG
	if( m_bIsCharAbuser[CAT_ED_ABUSER] == false )
	{
		// �������� �� ED�������!
		m_bIsCharAbuser[CAT_ED_ABUSER] = true;

		START_LOG( cout, L"[�˸�] ED ���� ����ڷ� ��ϵǾ����ϴ�!" )
			<< BUILD_LOG( spUser->GetName() )
			<< BUILD_LOG( spUser->GetCharName() );

		// ED ������� ���!
		KDBE_REG_ABUSER_AUTOMATIC_NOT kPacketNot;
		kPacketNot.AddAbuserInfo( KAbuserLogManager::AT_ED_ABUSER, spUser->GetCharUID() );
		spUser->SendToLogDB( DBE_REG_ABUSER_AUTOMATIC_NOT, kPacketNot );
	}
#endif SERV_PREVENT_ABUSER_LOG
}

void KUserAbuserManager::RegItemAbuser( IN KGSUserPtr spUser )
{
	if( spUser == NULL )
	{
		START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
			<< END_LOG;
		return;
	}

	if( spUser->GetAuthLevel() > SEnum::UAL_NORMAL )
	{
		// ��ڴ� ���ش�~
		return;
	}
#ifndef SERV_PREVENT_ABUSER_LOG
	if( m_bIsCharAbuser[CAT_ITEM_ABUSER] == false )
	{
		// �������� �� �����۾������!
		m_bIsCharAbuser[CAT_ITEM_ABUSER] = true;

		START_LOG( cout, L"[�˸�] ������ ���� ����ڷ� ��ϵǾ����ϴ�!" )
			<< BUILD_LOG( spUser->GetName() )
			<< BUILD_LOG( spUser->GetCharName() );

		// ������ ������� ���!
		KDBE_REG_ABUSER_AUTOMATIC_NOT kPacketNot;
		kPacketNot.AddAbuserInfo( KAbuserLogManager::AT_ITEM_ABUSER, spUser->GetCharUID() );
		spUser->SendToLogDB( DBE_REG_ABUSER_AUTOMATIC_NOT, kPacketNot );
	}
#endif SERV_PREVENT_ABUSER_LOG
}

//{{ 2011. 09. 08	������	������ ��Ŷ ����͸�
#ifdef SERV_AUTO_HACK_PACKET_MORNITOING
void KUserAbuserManager::RegPacketMornitoring( IN KGSUserPtr spUser )
{
	if( spUser == NULL )
	{
		START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
			<< END_LOG;
		return;
	}

	if( spUser->GetAuthLevel() > SEnum::UAL_NORMAL )
	{
		// ��ڴ� ���ش�~
		return;
	}
#ifndef SERV_PREVENT_ABUSER_LOG
	if( m_bIsAccountAbuser[AT_MORNITORING_ABUSER] == false )
	{
		// �������� �� ��Ŷ ���ô�!
		m_bIsAccountAbuser[AT_MORNITORING_ABUSER] = true;

		START_LOG( cout, L"[�˸�] ��Ŷ ���� ����ڷ� ��ϵǾ����ϴ�!" )
			<< BUILD_LOG( spUser->GetName() )
			<< BUILD_LOG( spUser->GetCharName() );

		// ������ ������� ���!
		KDBE_REG_ABUSER_AUTOMATIC_NOT kPacketNot;
		kPacketNot.AddAbuserInfo( KAbuserLogManager::AT_ABUSER_MONITORING, spUser->GetUID() );
		spUser->SendToLogDB( DBE_REG_ABUSER_AUTOMATIC_NOT, kPacketNot );
	}
#endif SERV_PREVENT_ABUSER_LOG
}
#endif SERV_AUTO_HACK_PACKET_MORNITOING
//}}

//{{ 2013. 05. 29	������	��Ŷ ����͸� �ټ��� �׸� �߰�
#ifdef SERV_PACKET_MORNITORING_SPIRIT_COL
void KUserAbuserManager::CheckMornitoringAbuser( IN const UidType iUserUID, IN const UidType iUnitUID, IN const int iED, IN const int iSpirit, IN const unsigned short usEventID )
#else
void KUserAbuserManager::CheckMornitoringAbuser( IN const UidType iUserUID, IN const UidType iUnitUID, IN const int iED, IN const unsigned short usEventID )
#endif SERV_PACKET_MORNITORING_SPIRIT_COL
//}}
{
    if( m_bIsAccountAbuser[AT_MORNITORING_ABUSER] == false )
		return;

	//{{ 2011. 09. 08	������	������ ��Ŷ ����͸�
#ifdef SERV_AUTO_HACK_PACKET_MORNITOING
	if( SiKGameSysVal()->IsHackUserPacketMornitor() == false )
		return;
#endif SERV_AUTO_HACK_PACKET_MORNITOING
	//}}

	//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	// �ش� ������ ó���� eventid�� �ؽ�Ʈ ���Ϸ� ������!
	//{{ 2013. 05. 29	������	��Ŷ ����͸� �ټ��� �׸� �߰�
#ifdef SERV_PACKET_MORNITORING_SPIRIT_COL
	SiKAbuserLogManager()->InsertAbuserEventLog( iUserUID, iUnitUID, iED, m_iGetItemCount, iSpirit, usEventID );
#else
	SiKAbuserLogManager()->InsertAbuserEventLog( iUserUID, iUnitUID, iED, m_iGetItemCount, usEventID );
#endif SERV_PACKET_MORNITORING_SPIRIT_COL
	//}}	
#else
	if( CheckAbuserEvent( usEventID ) == false )
		return;

	// �ش� ������ ó���� eventid�� �ؽ�Ʈ ���Ϸ� ������!
	SiKAbuserLogManager()->InsertAbuserEventLog( iUnitUID, iED, 0, usEventID );
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}
}

void KUserAbuserManager::CheckItemAbuser( IN KGSUserPtr spUser, IN const int iGetItemState, IN const int iItemID, IN const int iQuantity )
{
	std::map< int, int > mapGetItem;
	mapGetItem.insert( std::make_pair( iItemID, iQuantity ) );
    CheckItemAbuser( spUser, iGetItemState, mapGetItem );
}

void KUserAbuserManager::CheckItemAbuser( IN KGSUserPtr spUser, IN const int iGetItemState, IN const std::map< int, int >& mapGetItemInfo )
{
	if( spUser == NULL )
	{
		START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
			<< END_LOG;
		return;
	}

	if( m_bIsCharAbuser[CAT_ITEM_ABUSER] == false )
		return;

	KELOG_ITEM_ABUSER_LOG_NOT kPacketToLog;
	std::map< int, int >::const_iterator mit;
	for( mit = mapGetItemInfo.begin(); mit != mapGetItemInfo.end(); ++mit )
	{
		KItemAbuserLogInfo kAbuserLogInfo;
		kAbuserLogInfo.m_iRewardState = iGetItemState;
		kAbuserLogInfo.m_iItemID	  = mit->first;
		kAbuserLogInfo.m_iQuantity	  = mit->second;

		kPacketToLog.m_vecItemAbuserLog.push_back( kAbuserLogInfo );
	}

	kPacketToLog.m_iUnitUID = spUser->GetCharUID();
	spUser->SendToLogDB( ELOG_ITEM_ABUSER_LOG_NOT, kPacketToLog );
}

void KUserAbuserManager::CheckEDAbuser( IN KGSUserPtr spUser, IN const int iGetEDState, IN const int iBeforeED )
{
	if( spUser == NULL )
	{
		START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
			<< END_LOG;
		return;
	}
    
	if( m_bIsCharAbuser[CAT_ED_ABUSER] == false )
		return;

	KELOG_ED_ABUSER_LOG_NOT kPacketEDLog;
	kPacketEDLog.m_iUnitUID  = spUser->GetCharUID();
	kPacketEDLog.m_iState	 = iGetEDState;
	kPacketEDLog.m_iBeforeED = iBeforeED;
	kPacketEDLog.m_iAfterED	 = spUser->GetED();
	kPacketEDLog.m_iED		 = kPacketEDLog.m_iAfterED - iBeforeED;
	spUser->SendToLogDB( ELOG_ED_ABUSER_LOG_NOT, kPacketEDLog );
}

//{{ 2010. 9. 8	������	��Ʈ��ũ Ʈ���� ���� ���� ����͸�
#ifdef SERV_TRAFFIC_USER_MONITORING

void KUserAbuserManager::CheckTrafficAbuser( IN unsigned short usEventID )
{
	if( IsTrafficAbuser() == false )
		return;

	std::map< unsigned short, int >::iterator mit;
	mit = m_mapTrafficEventCount.find( usEventID );
	if( mit == m_mapTrafficEventCount.end() )
	{
		m_mapTrafficEventCount.insert( std::make_pair( usEventID, 1 ) );
	}
	else
	{
		++mit->second;
	}
}

void KUserAbuserManager::GetTrafficEventCount( OUT std::map< unsigned short, int >& mapTrafficEventCount ) const
{
	mapTrafficEventCount.clear();
    mapTrafficEventCount = m_mapTrafficEventCount;
}

#endif SERV_TRAFFIC_USER_MONITORING
//}}

bool KUserAbuserManager::CheckAbuserEvent( unsigned short usEventID )
{
	switch( usEventID )
	{
	case E_KICK_USER_NOT:
	case EGS_CREATE_UNIT_REQ:
	case EGS_DELETE_UNIT_REQ:	
	case EGS_SELECT_UNIT_REQ:
	case EGS_ENTER_EQUIP_SHOP_REQ:
	case EGS_ENTER_NPC_HOUSE_REQ:
	case EGS_OPEN_PVP_ROOM_LIST_REQ:	
	case EGS_CHANGE_PVP_ROOM_LIST_REQ:
	case EGS_CREATE_ROOM_REQ:
	case EGS_JOIN_ROOM_REQ:
	case EGS_QUICK_JOIN_REQ:
	case EGS_LEAVE_ROOM_REQ:
	case EGS_STATE_CHANGE_GAME_START_REQ:
	case EGS_END_GAME_REQ:
	case EGS_STATE_CHANGE_RESULT_REQ:
	case EGS_LEAVE_GAME_REQ:
	case EGS_STATE_CHANGE_GAME_INTRUDE_REQ:
	case EGS_INTRUDE_START_REQ:
	case ERM_END_GAME_DUNGEON_RESULT_DATA_NOT:
	case ERM_END_GAME_PVP_RESULT_DATA_NOT:
	case ERM_UPDATE_PVP_UNIT_INFO_NOT:
	case ERM_UPDATE_DUNGEON_UNIT_INFO_NOT:
	case EGS_CLIENT_QUIT_REQ:
	case EGS_BUY_ED_ITEM_REQ:
	case DBE_BUY_ED_ITEM_ACK:
	case EGS_SELL_ED_ITEM_REQ:
	case DBE_INSERT_ITEM_ACK:
	case EGS_RESULT_SUCCESS_REQ:
	case EGS_ADMIN_CHANGE_ED_REQ:
	case EGS_ADMIN_CHANGE_ED_ACK:	
	case EGS_JOIN_SQUARE_REQ:
	case ERM_JOIN_SQUARE_ACK:
	case EGS_LEAVE_SQUARE_REQ:
	case ERM_LEAVE_SQUARE_ACK:
	case EGS_REQUEST_TRADE_REQ:
	case ERM_REQUEST_TRADE_NOT:
	case EGS_REQUEST_TRADE_REPLY_NOT:
	case ERM_REQUEST_TRADE_REPLY_NOT:
	case ERM_OPEN_TRADE_ACK:
	case ERM_ACCEPT_JOIN_TRADE_NOT:
	case ERM_JOIN_TRADE_ACK:
	case ERM_JOIN_TRADE_NOT:
	case EGS_BREAK_TRADE_REQ:
	case ERM_BREAK_TRADE_REQ:
	case ERM_BREAK_TRADE_ACK:
	case ERM_BREAK_TRADE_NOT:
	case EGS_UPDATE_TRADE_ITEM_REQ:
	case ERM_UPDATE_TRADE_ITEM_ACK:
	case ERM_UPDATE_TRADE_ITEM_NOT:
	case EGS_UPDATE_TRADE_ED_REQ:
	case ERM_UPDATE_TRADE_ED_ACK:
	case ERM_UPDATE_TRADE_ED_NOT:
	case EGS_OFFER_TRADE_REQ:
	case ERM_OFFER_TRADE_ACK:
	case ERM_OFFER_TRADE_NOT:
	case EGS_APPROVE_TRADE_REQ:
	case ERM_APPROVE_TRADE_ACK:
	case ERM_APPROVE_TRADE_NOT:
	case ERM_CONFIRM_TRADE_REQ:
	case ERM_CONFIRM_TRADE_NOT:	
	case DBE_INSERT_TRADE_ITEM_ACK:	
	case EGS_CREATE_TC_ROOM_REQ:
	case EGS_END_TC_GAME_REQ:
	case EGS_OPEN_RANDOM_ITEM_REQ:
	case DBE_OPEN_RANDOM_ITEM_ACK:
	case EGS_ITEM_MANUFACTURE_REQ:
	case DBE_ITEM_MANUFACTURE_ACK:
	case ERM_CHECK_INVALID_ZOMBIE_USER_NOT:
	case EGS_CREATE_PERSONAL_SHOP_REQ:
	case ERM_CHECK_OPEN_PERSONAL_SHOP_ACK:
	case ERM_OPEN_PERSONAL_SHOP_ACK:
	case EGS_REG_PERSONAL_SHOP_ITEM_REQ:
	case ERM_REG_PERSONAL_SHOP_ITEM_ACK:
	case EGS_JOIN_PERSONAL_SHOP_REQ:
	case ERM_CHECK_JOIN_PERSONAL_SHOP_ACK:
	case ERM_JOIN_PERSONAL_SHOP_ACK:
	case EGS_BREAK_PERSONAL_SHOP_REQ:
	case ERM_BREAK_PERSONAL_SHOP_ACK:
	case ERM_BREAK_PERSONAL_SHOP_NOT:
	case EGS_LEAVE_PERSONAL_SHOP_REQ:
	case ERM_LEAVE_PERSONAL_SHOP_ACK:
	case EGS_BUY_PERSONAL_SHOP_ITEM_REQ:
	case ERM_BUY_PERSONAL_SHOP_ITEM_ACK:
	case EGS_ADMIN_CHANGE_UNIT_CLASS_REQ:
	case DBE_CHANGE_UNIT_CLASS_ACK:
	case ERM_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT:
	case ERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_REQ:
	case ERM_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT:
	case ERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT:
	case DBE_INSERT_BUY_PERSONAL_SHOP_ITEM_ACK:
	case ERM_CHANGE_TRADE_NOT:
	case EGS_RESOLVE_ITEM_REQ:
	case DBE_RESOLVE_ITEM_ACK:
	case EGS_ENCHANT_ITEM_REQ:
	case DBE_ENCHANT_ITEM_ACK:
	case EGS_SOCKET_ITEM_REQ:
	case DBE_SOCKET_ITEM_ACK:
	case EGS_GET_POST_LETTER_LIST_REQ:
	case DBE_GET_POST_LETTER_LIST_ACK:
	case EGS_SEND_LETTER_REQ:
	case DBE_PREPARE_INSERT_LETTER_TO_POST_ACK:
	case DBE_INSERT_LETTER_TO_POST_ACK:
	case ELG_FIND_USER_SEND_LETTER_NOT:
	case EGS_READ_LETTER_REQ:
	case EGS_GET_ITEM_FROM_LETTER_REQ:
	case DBE_GET_ITEM_FROM_LETTER_ACK:
	case EGS_DELETE_LETTER_REQ:
	case EGS_AGREE_HACK_USER_REQ:
	case DBE_AGREE_HACK_USER_ACK:
	case EGS_REPORT_HACK_USER_NOT:
	case EGS_NPROTECT_HACKING_USER_NOT:
	case EGS_ATTRIB_ENCHANT_ITEM_REQ:
	case DBE_ATTRIB_ENCHANT_ITEM_ACK:
		return true;
	}

	return false;
}


#endif SERV_USER_ABUSER_MANAGER
//}}

//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
void KUserAbuserManager::IncreaseGetItemCount( IN KGSUserPtr spUser, IN int iCount )
{
	m_iGetItemCount += iCount;

	// ȹ���� ������ ������ 5000���� ������ ��ŷ������ �����Ͽ� ����͸� �ǽ�!
	if( m_bIsAccountAbuser[AT_MORNITORING_ABUSER] == false )
	{
		if( GetItemCountMin() <= m_iGetItemCount  &&  m_iGetItemCount < GetItemCountMax() )
		{
			//m_bIsAccountAbuser[AT_MORNITORING_ABUSER] = true;

			START_LOG( cout, L"������ ȹ�� ī��Ʈ�� 5000���� �Ѿ ��ŷ �ǽ� ������ üũ��!" )
				<< BUILD_LOG( spUser->GetName() )
				<< BUILD_LOG( spUser->GetUID() )
				<< BUILD_LOGc( spUser->GetAuthLevel() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetED() )
				<< BUILD_LOGc( spUser->GetLevel() )
				<< BUILD_LOG( spUser->GetEXP() )
				<< BUILD_LOG( spUser->GetStateIDString() )
				<< BUILD_LOG( spUser->GetRoomUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( spUser->GetRoomUID() ) )
				<< BUILD_LOG( GetItemCountMin() )
				<< BUILD_LOG( GetItemCountMax() )
				<< BUILD_LOG( m_iGetItemCount );

			KDBE_SEND_PHONE_MSG_NOT kPacketNot;
			//{{ 2012. 10. 15	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
			SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_AUTO_HACK_CHECK_GET_ITEM, kPacketNot.m_vecPhoneNum );
#else
			kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-4118-3867" ) );		// ��âȣ �����
			kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-8584-3123" ) );		// ����ö
			kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-4342-0365" ) );		//
			kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-7753-3789" ) );		//
			kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-8421-1075" ) );		// ������
			kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-9317-0790" ) );		// Ȳ����
#endif SERV_SMS_TOTAL_MANAGER
			//}}
			kPacketNot.m_wstrSMSMessage = boost::str( boost::wformat( L"�۾��� �߰�! : UserUID(%d), UnitUID(%d), NickName(%s)" ) % spUser->GetUID() % spUser->GetCharUID() % spUser->GetCharName() );
			spUser->SendToSMSDB( DBE_SEND_PHONE_MSG_NOT, kPacketNot );

			//{{ 2011. 09. 20	��μ�	������ 5000�� ����� �ڵ� ���� ��
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM_AUTO_BLOCK
			if( spUser != NULL )
			{
				//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_GET_ITEM_COUNT;
#else
				KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_GET_ITEM_COUNT;
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				//}}

				//spUser->HackUserRegRejectedUser( kNot.m_cHackingCheckType );
				spUser->HackUserRegMonitorsUser( kNot.m_cHackingCheckType );
				spUser->HackUserKick( kNot );
			}
#endif SERV_AUTO_HACK_CHECK_GET_ITEM_AUTO_BLOCK
			//}}
		}
	}
	else
	{
		// ���� ī��Ʈ�� 10000���� ������ �α׸� ������ ����
		if( GetItemCountMax() <= m_iGetItemCount )
		{
			m_bIsAccountAbuser[AT_MORNITORING_ABUSER] = false;
		}
	}
}
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
//}}

//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_LEVEL
bool KUserAbuserManager::IsAutoHackLevelUpUser( IN u_char ucCurrentCharLevel
#ifdef SERV_AUTO_HACK_CHECK_LEVEL_FIX
											   , UidType iCurrentCharUID
#endif // SERV_AUTO_HACK_CHECK_LEVEL_FIX
											   )
{
	// 7���� �̻� üũ�Ѵ�!
	if( ucCurrentCharLevel <= AE_AUTO_HACK_CHECK_LEVEL )
		return false;

	CTime tCurTime = CTime::GetCurrentTime();
	CTime tLevelUpTime = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), tCurTime.GetHour(), tCurTime.GetMinute(), 0 );

	if( tLevelUpTime == m_tLastLevelUpTime 
#ifdef SERV_AUTO_HACK_CHECK_LEVEL_FIX
		&& iCurrentCharUID == m_iLastLevelUpUID
#endif // SERV_AUTO_HACK_CHECK_LEVEL_FIX
		)
	{
		std::wstring wstrLevelUpTime = (const wchar_t*)m_tLastLevelUpTime.Format(_T("%Y-%m-%d %H:%M:%S"));

		START_LOG( cout, L"������ �ǽ� ���� �߰�! ĳ���Ͱ� �������� �ð� ������ 1�� �̸��Դϴ�!" )
			<< BUILD_LOG( wstrLevelUpTime );
		return true;
	}

	m_tLastLevelUpTime = tLevelUpTime;
#ifdef SERV_AUTO_HACK_CHECK_LEVEL_FIX
	m_iLastLevelUpUID = iCurrentCharUID;
#endif // SERV_AUTO_HACK_CHECK_LEVEL_FIX
	return false;
}
#endif SERV_AUTO_HACK_CHECK_LEVEL
//}}

#ifdef SERV_HACKING_USER_CHECK_COUNT// �۾���¥: 2013-05-30	// �ڼ���
int KUserAbuserManager::IncreaseCheckCount( IN const byte byteType, OUT byte& byteResult, OUT KAbuserLogManager::KHackingUserCheckInfo& kInfo )
{
	if( SiKAbuserLogManager()->GetHackingUserCheckInfo( byteType, kInfo ) == false )
	{
		return HUCR_FAILED;
	}

	TYPE_HACKING_USER_CHECK_COUNT::iterator it = m_mapHackingUserCheckCount.find( byteType );
	if( it == m_mapHackingUserCheckCount.end() )
	{
		return HUCR_NOT_INITED;
	}

	if( kInfo.m_bResetWeekly == true )
	{
		// ī��Ʈ �ʱ�ȭ üũ
		if( it->second.m_wstrLastResetDate.empty() == true )
		{
			CTime tCurrentTime				= CTime::GetCurrentTime();
			it->second.m_wstrLastResetDate	= tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
		}
		else
		{
			CTime tLastResetDate;
			if( KncUtil::ConvertStringToCTime( it->second.m_wstrLastResetDate, tLastResetDate ) == false )
			{
				START_LOG( cerr, L"�� ī��Ʈ m_wstrLastResetDate �ð� ��ȯ ����" )
					<< BUILD_LOG( it->second.m_wstrLastResetDate )
					<< END_LOG;
			}
			else
			{
				// ������ �ʱ�ȭ �ð����κ��� 1���� �� ����� ���� 6�ø� ����
				int iDayOfWeek			= tLastResetDate.GetDayOfWeek();
				int iNextResetDayTerm	= 5 - iDayOfWeek;
				if( iNextResetDayTerm <= 0 )
				{
					iNextResetDayTerm += 7;
				}

				CTimeSpan tNextResetDayTerm( iNextResetDayTerm, 0, 0, 0 );
				CTime tResetTime	= CTime( tLastResetDate.GetYear(), tLastResetDate.GetMonth(), tLastResetDate.GetDay(), 6, 0, 0 ) + tNextResetDayTerm;
				CTime tCurrentTime	= CTime::GetCurrentTime();

				if( tResetTime < tCurrentTime )
				{
					// ī��Ʈ �ʱ�ȭ ����
					it->second.m_byteBlockCheckCount	= 0;
					it->second.m_wstrLastResetDate		= tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
				}
			}
		}
	}

	// ���� ó��
	const byte byteBlockCheckCount = it->second.m_byteBlockCheckCount;
	it->second.m_byteBlockCheckCount += 1;

	byteResult = 0;
	if( ( 0 < kInfo.m_bytePacketMornitoring )	&& ( kInfo.m_bytePacketMornitoring	<= it->second.m_byteBlockCheckCount ) )	{	SEnum::AddFlag( byteResult, KAbuserLogManager::HUCRT_PACKETMORNITORING );	}
	if( ( 0 < kInfo.m_byteAccountBlock )		&& ( kInfo.m_byteAccountBlock		<= it->second.m_byteBlockCheckCount ) )	{	SEnum::AddFlag( byteResult, KAbuserLogManager::HUCRT_ACCOUNTBLOCK );	}
	if( ( 0 < kInfo.m_byteKick )				&& ( kInfo.m_byteKick				<= it->second.m_byteBlockCheckCount ) )	{	SEnum::AddFlag( byteResult, KAbuserLogManager::HUCRT_KICK );	}
	if( ( 0 < kInfo.m_byteReset )				&& ( kInfo.m_byteReset				<= it->second.m_byteBlockCheckCount ) )	{	it->second.m_byteBlockCheckCount = 0;	}

	if( byteBlockCheckCount != it->second.m_byteBlockCheckCount )
	{
		it->second.m_bChanged = true;
	}

	return HUCR_SUCCEED;
}

void KUserAbuserManager::InitCheckCount( IN const byte byteType, IN const byte byteBlockCheckCount, IN const std::wstring& wstrLastResetDate )
{
	TYPE_HACKING_USER_CHECK_COUNT::iterator it = m_mapHackingUserCheckCount.find( byteType );
	if( it != m_mapHackingUserCheckCount.end() )
	{
		return;
	}

	KHackingUserCheckCount kData;
	kData.m_bChanged			= false;
	kData.m_byteBlockCheckCount	= byteBlockCheckCount;
	kData.m_wstrLastResetDate	= wstrLastResetDate;
	m_mapHackingUserCheckCount.insert( TYPE_HACKING_USER_CHECK_COUNT::value_type( byteType, kData ) );
}

void KUserAbuserManager::GetDBUpdateInfo( OUT std::map<byte, KHackingUserCheckCountDB>& mapHackingUserCheckCountDB ) const
{
	KHackingUserCheckCountDB kInfo;
	
	TYPE_HACKING_USER_CHECK_COUNT::const_iterator	it;
	for( it = m_mapHackingUserCheckCount.begin(); it != m_mapHackingUserCheckCount.end(); ++it )
	{
		if( it->second.m_bChanged == false )
		{
			continue;
		}

		kInfo.m_byteBlockCheckCount	= it->second.m_byteBlockCheckCount;
		kInfo.m_wstrLastResetDate	= it->second.m_wstrLastResetDate;
		mapHackingUserCheckCountDB.insert( std::map<byte, KHackingUserCheckCountDB>::value_type( it->first, kInfo ) );
	}
}
#endif // SERV_HACKING_USER_CHECK_COUNT