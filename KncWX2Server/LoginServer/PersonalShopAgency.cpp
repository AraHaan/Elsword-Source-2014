#include ".\PersonalShopAgency.h"

#include "Room/PersonalShopFSM.h"
#include "NetError.h"
#include "Enum/Enum.h"
#include "X2Data/XSLRoom.h"
#include "X2data/XSLItem.h"
////{{ 2010. 05. 18  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	#include "LoginServer.h"
#endif SERV_INTEGRATION
////}}
#include "LoginRoomManager.h"

#include "X2Data/XSLSquareUnit.h"

//{{ 2011. 04. 13	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY


#define CLASS_TYPE KPersonalShopAgency

NiImplementRTTI( KPersonalShopAgency, KLoginBaseRoom );

ImplToStringW( KPersonalShopAgency )
{
	return START_TOSTRING_PARENTW( KLoginBaseRoom );
}

KPersonalShopAgency::KPersonalShopAgency(void)
{
	m_kPersonalShopUserManager.Init( 8 );
	m_kPersonalShopItemManager.Clear();
	m_wstrPersonalShopName.clear();

#ifdef SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = SEnum::AST_NONE;
#else //SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = CXSLSquareUnit::PST_NONE;
#endif //SERV_UPGRADE_TRADE_SYSTEM
}

KPersonalShopAgency::~KPersonalShopAgency(void)
{
}

void KPersonalShopAgency::CloseRoom()
{
	const UidType iHostUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
	if( iHostUnitUID != 0 )
	{
		// �븮���� �ذ�!
		m_kPersonalShopUserManager.DeletePersonalShopAgency( iHostUnitUID );

		// �븮���� ������ ����Ʈ���� �����մϴ�.
		GetKLoginRoomManager()->DelPShopOwner( iHostUnitUID );
	}

	m_kPersonalShopUserManager.Reset();
	m_kPersonalShopItemManager.Clear();
	m_wstrPersonalShopName.clear();	

	KLoginBaseRoom::CloseRoom();
}

void KPersonalShopAgency::Tick()
{
	KLoginBaseRoom::Tick();

	switch( GetStateID() )
	{
	case KPersonalShopFSM::S_INIT:
		break;

	case KPersonalShopFSM::S_CLOSE:
		{
			CloseRoom();
		}
		break;

	case KPersonalShopFSM::S_WAIT:
		// ���������� �ϰ� �����ð��ȿ� ��ǰ����� ���� ������� �ݾƹ�����..
		if( m_kPersonalShopUserManager.IsExistPShopAgency() == true )
		{
			if( m_kTimer[TM_WAIT_OTHER].elapsed() >= 1800.0 )
			{
                m_kTimer[TM_WAIT_OTHER].restart();
				const UidType iHostUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
				const UidType iHostGSUID = m_kPersonalShopUserManager.GetPShopAgencyHostGSUID();
				BreakPersonalShop( ( int )NetError::NOT_LEAVE_ROOM_REASON_17, iHostGSUID, iHostUnitUID );
			}
		}
		break;

	case KPersonalShopFSM::S_SHOP:
		{
			if( m_kTimer[TM_CHECK_EXPIRATION].elapsed() >= 60.0 )
			{
				m_kTimer[TM_CHECK_EXPIRATION].restart();

				CTime tCurTime = CTime::GetCurrentTime();

				// �븮���� �Ⱓ ���ᰡ �Ǹ� �ڵ����� STOP_SALEó��!
				if( tCurTime > m_tAgencyExpirationDate )
				{
					// �Ǹ� ���� ���·� �̵�!
					StateTransition( KPersonalShopFSM::I_TO_STOP_SALE );

					// DB�� �Ǹ� ���� ������Ʈ �Ϸ� ����!
					KDBE_STOP_SALE_PSHOP_AGENCY_REQ kPacketToDB;
					kPacketToDB.m_iUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
					kPacketToDB.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_21;
					SendToGameDB( DBE_STOP_SALE_PSHOP_AGENCY_REQ, kPacketToDB );

					// �����ڿ��� �Ǹ� ���� �˷��ֱ����� ����!
					SendPShopAgencyMessage( KEGS_PSHOP_AGENCY_MESSAGE_NOT::PAMT_EXPIRATION );
					break;
				}

				// �������� 24�ð��� ������ �ڵ����� STOP_SALEó��!
				if( tCurTime > m_tAgencyOpenOver24Hours )
				{
					// �Ǹ� ���� ���·� �̵�!
					StateTransition( KPersonalShopFSM::I_TO_STOP_SALE );

					// DB�� �Ǹ� ���� ������Ʈ �Ϸ� ����!
					KDBE_STOP_SALE_PSHOP_AGENCY_REQ kPacketToDB;
					kPacketToDB.m_iUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
					kPacketToDB.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_21;
					SendToGameDB( DBE_STOP_SALE_PSHOP_AGENCY_REQ, kPacketToDB );

					// �����ڿ��� �Ǹ� ���� �˷��ֱ����� ����!
					SendPShopAgencyMessage( KEGS_PSHOP_AGENCY_MESSAGE_NOT::PAMT_OVER_24_HOURS );
					break;
				}
			}

			//{{ 2011. 05. 13	������	�븮 ���� ���� ���� ���� �ذ�
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
			CheckZombieUser();
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
			//}}
		}
		break;

	case KPersonalShopFSM::S_STOP_SALE:
		{
			//{{ 2011. 05. 13	������	�븮 ���� ���� ���� ���� �ذ�
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
			CheckZombieUser();
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
			//}}
		}		
		break;

	default:
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( GetStateID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		break;
	}
}

void KPersonalShopAgency::SendPShopAgencyMessage( IN const KEGS_PSHOP_AGENCY_MESSAGE_NOT::PSHOP_AGENCY_MESSAGE_TYPE eType, IN const int iItemID /*= 0*/, IN const int iQuantity /*= 0*/, IN const int iSellED /*= 0*/ )
{
	KEGS_PSHOP_AGENCY_MESSAGE_NOT kPacketNot;
	kPacketNot.m_cMessageType = eType;
	kPacketNot.m_iHostUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
	kPacketNot.m_iSellItemID = iItemID;
	kPacketNot.m_iSellQuantity = iQuantity;
	kPacketNot.m_iSellED = iSellED;
	SendToServer( ERM_PSHOP_AGENCY_MESSAGE_NOT, kPacketNot );
}

void KPersonalShopAgency::SendToGameDB( unsigned short usEventID )
{
	SendToGameDB( usEventID, char() );
}

bool KPersonalShopAgency::IsEmpty()
{
	if( m_kPersonalShopUserManager.GetNumMember() < 0 )
	{
		START_LOG( cerr, L"���� ���� �ο� ���� �̻���." )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
			<< END_LOG;
	}

	return ( m_kPersonalShopUserManager.GetNumMember() <= 0 );
}

bool KPersonalShopAgency::IsFull()
{
	if( m_kPersonalShopUserManager.GetNumMember() > m_kPersonalShopUserManager.GetMaxMember() )
	{
		START_LOG( cerr, L"���λ��� �ο� ���� �̻���." )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetMaxMember() )
			<< END_LOG;
	}

	return ( m_kPersonalShopUserManager.GetNumMember() >= m_kPersonalShopUserManager.GetMaxMember() );
}

//{{ 2011. 05. 13	������	�븮 ���� ���� ���� ���� �ذ�
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
void KPersonalShopAgency::CheckZombieUser()
{
	// 3�и��� �ѹ��� üũ!
	if( m_kTimer[TM_CHECK_ZOMBIE_USER].elapsed() < 60.0 )
		return;

	m_kTimer[TM_CHECK_ZOMBIE_USER].restart();
	
	// ���� �ο��� ������ �н�!
	if( m_kPersonalShopUserManager.GetNumMember() == 0 )
		return;

	// ������ �ִ� ��� �����鿡�� üũ ��Ŷ ������!
	KERM_CHECK_ZOMBIE_USER_REQ kPacket;
	kPacket.m_iRoomUID = GetUID();
	BroadCast( ERM_CHECK_ZOMBIE_USER_REQ, kPacket );
}
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
//}}

bool KPersonalShopAgency::BreakPersonalShop( IN const int iReason, IN const UidType iHostGSUID, IN const UidType iHostUID )
{
	// 1. �Ǹ����� �������� �ִ��� Ȯ��
	if( m_kPersonalShopItemManager.IsEmptySellItemList() == false )
	{
		START_LOG( cerr, L"�Ǹ����� �������� �ִµ� �� �������� ����? ���״�!!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_kPersonalShopItemManager.GetSellItemCount() )
			<< END_LOG;
		return false;
	}

	// 2. ��¥ HostUID�� �´��� Ȯ������!
	const UidType iCheckHostUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
	if( iCheckHostUID != iHostUID )
	{
		START_LOG( cerr, L"������ UnitUID�� �ٸ��� ��û�� ���Դ�! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( iCheckHostUID )
			<< BUILD_LOG( iHostUID )
			<< END_LOG;
		return false;
	}

	// 3. ��ȿ� �ִ� �����鿡�� �˸�
	KERM_BREAK_PSHOP_AGENCY_NOT kPacketNot;
	kPacketNot.m_iReason = iReason;
	kPacketNot.m_iHostGSUID = iHostGSUID;
	kPacketNot.m_iHostUID = iHostUID;
	BroadCast( ERM_BREAK_PSHOP_AGENCY_NOT, kPacketNot );

#ifdef DEF_TRADE_BOARD
	// 4. �Խ��ǿ��� ���� ����
	std::vector< KSellPersonalShopItemInfo > vecSellItemList;
	m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( vecSellItemList );
	SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, vecSellItemList );
#endif DEF_TRADE_BOARD

	// 5. DB���� ������Ʈ �Ϸ� ����!
	SendToGameDB( DBE_BREAK_PSHOP_AGENCY_REQ, kPacketNot );

	// 6. ���� ���� ����
	StateTransition( KPersonalShopFSM::I_TO_CLOSE );
	return true;
}

void KPersonalShopAgency::BroadCastID( u_short usEventID )
{
	BroadCast( usEventID, char() );
}

void KPersonalShopAgency::_BroadCast( const KEvent& kEvent_ )
{
	std::map< UidType, std::set< UidType > > mapUserList;
	m_kPersonalShopUserManager.GetUserList( mapUserList );

	// GameServer�� ���� loop
	std::map< UidType, std::set< UidType > >::iterator mit;
	for( mit = mapUserList.begin(); mit != mapUserList.end(); ++mit )
	{
		KEventPtr spEvent( kEvent_.Clone() );
		spEvent->m_anTrace[0] = mit->first;
		spEvent->m_anTrace[1] = -1;

		// User ������ ���� loop
		BOOST_TEST_FOREACH( const UidType, iMemberUnitUID, mit->second )
		{
			LIF( spEvent->m_kDestPerformer.AddUID( iMemberUnitUID ) );
		}

		KncSend( PI_LOGIN_ROOM, GetUID(), spEvent );
	}
}

void KPersonalShopAgency::_BroadCastWithOutOneChar( const KEvent& kEvent_, const UidType iUnitUID )
{
	std::map< UidType, std::set< UidType > > mapUserList;
	m_kPersonalShopUserManager.GetUserList( mapUserList );

	// GameServer�� ���� loop
	std::map< UidType, std::set< UidType > >::iterator mit;
	for( mit = mapUserList.begin(); mit != mapUserList.end(); ++mit )
	{
		KEventPtr spEvent( kEvent_.Clone() );
		spEvent->m_anTrace[0] = mit->first;
		spEvent->m_anTrace[1] = -1;

		// User ������ ���� loop
		BOOST_TEST_FOREACH( const UidType, iMemberUnitUID, mit->second )
		{
			if( iMemberUnitUID == iUnitUID )
				continue;

			LIF( spEvent->m_kDestPerformer.AddUID( iMemberUnitUID ) );
		}

		KncSend( PI_LOGIN_ROOM, GetUID(), spEvent );
	}
}

#ifdef DEF_TRADE_BOARD
void KPersonalShopAgency::SendPShopListInfo( IN const int _iCode, IN const std::vector<KSellPersonalShopItemInfo> & _veckInfo, IN const UidType iGSUID /*= 0*/ )
{
	const UidType iHostUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
	LIF( iHostUnitUID != 0 );

	KERM_PERSONAL_SHOP_INFO_NOT kNot;
	kNot.m_iPSUID = GetUID();
	kNot.m_iHostUnitUID = iHostUnitUID;
	kNot.m_iCode = _iCode;
	kNot.m_vecItemInfo = _veckInfo;

	//������ �߰� �ϰܿ츸 �Ǹ��� �г����� �־���.
	if( _iCode == NetError::ERR_PERSONAL_SHOP_26 )
		m_kPersonalShopUserManager.GetPShopAgencyHostNickName( kNot.m_wstrSellerNickName );

	if( iGSUID == 0 )
	{
		//{{ 2010. 05. 11  ������	�������� ������ ����
#ifdef SERV_INTEGRATION
		BroadCastSameGroupGS( ERM_PERSONAL_SHOP_INFO_NOT, kNot );
#else
		SendToGSServer( 0, ERM_PERSONAL_SHOP_INFO_NOT, kNot );
#endif SERV_INTEGRATION
		//}}
	}
	else
	{
		SendToGSServer( iGSUID, ERM_PERSONAL_SHOP_INFO_NOT, kNot );
	}

	START_LOG( clog, L"�������� ������ ����.!" )
		<< BUILD_LOG( kNot.m_vecItemInfo.size() )
		<< BUILD_LOG( NetError::GetErrStr(_iCode) )
		;
}
void KPersonalShopAgency::SendPShopListInfo( IN const int _iCode, IN const KSellPersonalShopItemInfo& kInfo, IN const UidType iGSUID /*= 0*/ )
{
	std::vector< KSellPersonalShopItemInfo > veckInfo;
	veckInfo.push_back( kInfo );
	SendPShopListInfo( _iCode, veckInfo, iGSUID );
}
#endif DEF_TRADE_BOARD


void KPersonalShopAgency::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();  // 040428. ��Ŷ ó�� �ҿ�ð��� ���Ѵ�.

	switch( spEvent_->m_usEventID )
	{
   _CASE( ERM_CHECK_INVALID_USER_NOT, UidType );

	CASE( ERM_LOAD_PSHOP_BOARD_INFO_NOT );

	CASE( ERM_OPEN_PSHOP_AGENCY_REQ );
   _CASE( DBE_OPEN_PSHOP_AGENCY_ACK, KERM_OPEN_PERSONAL_SHOP_ACK );

	CASE( ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT );

	CASE_NOPARAM( ERM_CHECK_OPEN_PSHOP_AGENCY_REQ );

	//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	_CASE( ERM_CHECK_FOR_REG_PSHOP_AGENCY_ITEM_REQ, KDBE_PREPARE_REG_PSHOP_AGENCY_ITEM_REQ );
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}

   _CASE( ERM_REG_PSHOP_AGENCY_ITEM_REQ, KERM_REG_PERSONAL_SHOP_ITEM_REQ );
   _CASE( DBE_REG_PSHOP_AGENCY_ITEM_ACK, KERM_REG_PERSONAL_SHOP_ITEM_ACK );

   _CASE( ERM_JOIN_MY_PSHOP_AGENCY_REQ, KERM_JOIN_PERSONAL_SHOP_REQ );
	CASE( ERM_JOIN_PERSONAL_SHOP_REQ );

   _CASE( ERM_LEAVE_PSHOP_AGENCY_REQ, KERM_LEAVE_PERSONAL_SHOP_REQ );
	CASE( ERM_LEAVE_PERSONAL_SHOP_REQ );

	CASE( ERM_BUY_PERSONAL_SHOP_ITEM_REQ );
   _CASE( DBE_BUY_PSHOP_AGENCY_ITEM_ACK, KDBE_BUY_PSHOP_AGENCY_ITEM_REQ );	

	CASE_NOPARAM( ERM_STOP_SALE_PSHOP_AGENCY_REQ );
	CASE( DBE_STOP_SALE_PSHOP_AGENCY_ACK );

	CASE( ERM_UPDATE_PERIOD_PSHOP_AGENCY_NOT );

   _CASE( ERM_PICK_UP_FROM_PSHOP_AGENCY_REQ, KEGS_PICK_UP_FROM_PSHOP_AGENCY_REQ );
	CASE( ERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_ACK );
	CASE( DBE_PICK_UP_FROM_PSHOP_AGENCY_ACK );
	//{{ 2011. 05. 13	������	�븮 ���� ���� ���� ���� �ذ�
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	CASE( ERM_CHECK_ZOMBIE_USER_ACK );
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	//}}

    CASE( ERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ );

	default:
		START_LOG( cerr, L"�ڵ鷯�� ���ǵ��� �ʾ���." )
			<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
			<< END_LOG;
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

//���Ӽ����� �׾�����..ó��
_IMPL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType )
{
	std::vector< UidType > vecDelUserUID;
	int iCount = m_kPersonalShopUserManager.DeleteUserByGsUID( kPacket_, vecDelUserUID );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_ )
		<< BUILD_LOG( iCount )
		<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() );

	//������Ƽ���� ������ ������ ������ ����.
	if( iCount == 0 )
		return;

	//���� �� ���ְ�
	BOOST_TEST_FOREACH( const UidType, iDelUnitUID, vecDelUserUID )
	{
		m_kPersonalShopUserManager.LeavePersonalShop( iDelUnitUID );
	}
}

IMPL_ON_FUNC( ERM_LOAD_PSHOP_BOARD_INFO_NOT )
{
	if( GetStateID() == KPersonalShopFSM::S_INIT  || 
		GetStateID() == KPersonalShopFSM::S_WAIT  || 
		GetStateID() == KPersonalShopFSM::S_STOP_SALE )
	{
		return;
	}

	// �Ǹ����� �������� �ִ��� Ȯ��!
	if( m_kPersonalShopItemManager.IsEmptySellItemList() == true )
	{
		START_LOG( cerr, L"�Ǹ����� �������� ���µ� �� ���°� �̻��ϱ�?" )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}	

	std::vector< KSellPersonalShopItemInfo > vecSellItemList;
	m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( vecSellItemList );
	SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_26, vecSellItemList, kPacket_.m_iGSUID );
}

IMPL_ON_FUNC( ERM_OPEN_PSHOP_AGENCY_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_INIT ), ERM_OPEN_PSHOP_AGENCY_ACK, KERM_OPEN_PERSONAL_SHOP_ACK );

	// ��ǰ����� ��ٸ��� ���� Ÿ�̸� ����.(�ʱ� 5��)
	m_kTimer[TM_WAIT_OTHER].restart();

	// �Ⱓ ���� üũ�� ���� Ÿ�̸�!
	m_kTimer[TM_CHECK_EXPIRATION].restart();

	//{{ 2011. 05. 13	������	�븮 ���� ���� ���� ���� �ذ�
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	m_kTimer[TM_CHECK_ZOMBIE_USER].restart();
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	//}}

	//���� �������̶� wait�� ����..
	// �� ���¸� wait �� �ٲٰ� ��ǰ����� �ö����� ��ٸ���.
	StateTransition( KPersonalShopFSM::I_TO_WAIT );

	//////////////////////////////////////////////////////////////////////////
	// �����̹Ƿ� ���� ������ �ʱ�ȭ �Ѵ�.
	m_kPersonalShopUserManager.Reset();	

	KPersonalShopUserInfo kInfo;
	kInfo.m_iUserUID = kPacket_.m_iUserUID;
	kInfo.m_wstrNickName = kPacket_.m_wstrNickName;
	kInfo.m_iUnitUID = FIRST_SENDER_UID;
	kInfo.m_iGSUID = LAST_SENDER_UID;
#ifdef DEF_TRADE_BOARD
	kInfo.m_bIsSquare = false;
#endif DEF_TRADE_BOARD

	// �븮���� ����!
	if( m_kPersonalShopUserManager.CreatePersonalShopAgency( kInfo ) == false )
	{
		START_LOG( cerr, L"�븮 ���� ��� ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_44;
		StateTransition( KPersonalShopFSM::I_TO_CLOSE );
		goto end_proc;
	}

	// ���λ��� ���� ����!	
//{{ 2011. 05. 13	������	�븮 ���� ���� ���� ���� �ذ�
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	if( m_kPersonalShopUserManager.EnterPShopAgencyOwner( kInfo ) == false )
#else
	if( m_kPersonalShopUserManager.EnterPersonalShop( kInfo ) == false )
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
//}}
	{
		START_LOG( cerr, L"�븮���� ������ ���� �߰� ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_44;
		StateTransition( KPersonalShopFSM::I_TO_CLOSE );
		goto end_proc;
	}	

	// ack�� ������.
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iPersonalShopUID = GetUID();

	// ���� Ÿ�� ����
	kPacket.m_cPersonalShopType = kPacket_.m_cPersonalShopType;
#ifdef SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = static_cast<SEnum::AGENCY_SHOP_TYPE>(kPacket_.m_cPersonalShopType);
#else //SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = static_cast<CXSLSquareUnit::PERSONAL_SHOP_TYPE>(kPacket_.m_cPersonalShopType);
#endif //SERV_UPGRADE_TRADE_SYSTEM

	// ���� ���� �ð� ����
	m_tAgencyOpenDate = CTime::GetCurrentTime();
	kPacket.m_wstrAgencyOpenDate = (const std::wstring)m_tAgencyOpenDate.Format( _T("%Y-%m-%d %H:%M:%S") );

	// ���� ���� �ð� 24�ð� ���� ����
	m_tAgencyOpenOver24Hours = m_tAgencyOpenDate + CTimeSpan( 1, 0, 0, 0 );

	// �븮���� ������ ����
	m_wstrAgencyExpirationDate = kPacket_.m_wstrAgencyExpirationDate;
	if( KncUtil::ConvertStringToCTime( m_wstrAgencyExpirationDate, m_tAgencyExpirationDate ) == false )
	{
		START_LOG( cerr, L"�븮 ���� ������ ���� ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_44;
		StateTransition( KPersonalShopFSM::I_TO_CLOSE );
		goto end_proc;
	}

	kPacket.m_iGSUID = LAST_SENDER_UID;
	kPacket.m_iUserUID = kInfo.m_iUserUID;
	kPacket.m_iUnitUID = FIRST_SENDER_UID;
	SendToGameDB( DBE_OPEN_PSHOP_AGENCY_REQ, kPacket );
	return;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_PSHOP_AGENCY_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_OPEN_PSHOP_AGENCY_ACK, KERM_OPEN_PERSONAL_SHOP_ACK )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_WAIT ), ERM_OPEN_PSHOP_AGENCY_ACK, KERM_OPEN_PERSONAL_SHOP_ACK );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// �븮���� ������ ����Ʈ�� ������ ������ �߰��Ѵ�!
		GetKLoginRoomManager()->AddPShopOwner( kPacket_.m_iUnitUID, GetUID() );
		
		SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, ERM_OPEN_PSHOP_AGENCY_ACK, kPacket_ );
		return;
	}

	// �ٽ� ��Ż!
	if( m_kPersonalShopUserManager.LeavePersonalShop( kPacket_.m_iUnitUID ) == false )
	{
		START_LOG( cerr, L"�븮���� ������ ����." )
			<< END_LOG;
	}

	// ���� ��Ŷ
	kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_44;
	SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, ERM_OPEN_PSHOP_AGENCY_ACK, kPacket );

	// �� �ݱ�
	BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_21, kPacket_.m_iGSUID, kPacket_.m_iUnitUID );
}

IMPL_ON_FUNC( ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT )
{
	if( GetStateID() != KPersonalShopFSM::S_INIT )
	{
		START_LOG( cerr, L"�� ���°� �̻��մϴ�! �븮������ ������ �� �����ϴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetStateID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
        return;
	}

	// ����Ϸ��� ��ǰ�� �ִ��� üũ�Ѵ�!
	if( kPacket_.m_vecSellItemInfo.empty() == true )
	{
		START_LOG( cerr, L"������ ���� ������ �����Ϸ��µ� ��ϵ� ��ǰ�� ����! ���� �Ͼ���� �ȵǴ¿���!" )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< BUILD_LOG( kPacket_.m_cPersonalShopType )
			<< BUILD_LOG( kPacket_.m_wstrAgencyExpirationDate )
			<< BUILD_LOG( kPacket_.m_wstrAgencyOpenDate )
			<< BUILD_LOG( kPacket_.m_wstrPersonalShopName )
			<< BUILD_LOG( kPacket_.m_bOnSale );

		KERM_BREAK_PSHOP_AGENCY_NOT kPacketNot;
		kPacketNot.m_iHostUID = kPacket_.m_iUnitUID;
		SendToGameDB( DBE_BREAK_PSHOP_AGENCY_REQ, kPacketNot );

		// ������ �Ǹ� �������·� ������!
		StateTransition( KPersonalShopFSM::I_TO_CLOSE );
		return;
	}

	// ��ǰ����� ��ٸ��� ���� Ÿ�̸� ����.(�ʱ� 5��)
	m_kTimer[TM_WAIT_OTHER].restart();

	// �Ⱓ ���� üũ�� ���� Ÿ�̸�
	m_kTimer[TM_CHECK_EXPIRATION].restart();

	//{{ 2011. 05. 13	������	�븮 ���� ���� ���� ���� �ذ�
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	m_kTimer[TM_CHECK_ZOMBIE_USER].restart();
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	//}}

	//���� �������̶� wait�� ����..
	// �� ���¸� wait �� �ٲٰ� ��ǰ����� �ö����� ��ٸ���.
	StateTransition( KPersonalShopFSM::I_TO_WAIT );

	//////////////////////////////////////////////////////////////////////////

	// �����̹Ƿ� ���� ������ �ʱ�ȭ �Ѵ�.
	m_kPersonalShopUserManager.Reset();	

	KPersonalShopUserInfo kInfo;
	kInfo.m_iUserUID = kPacket_.m_iUserUID;
	kInfo.m_wstrNickName = kPacket_.m_wstrNickName;
	kInfo.m_iUnitUID = kPacket_.m_iUnitUID;

	// �븮���� ���� �߰�!
	if( m_kPersonalShopUserManager.CreatePersonalShopAgency( kInfo ) == false )
	{
		START_LOG( cerr, L"�븮 ���� ���� �߰� ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< END_LOG;

		StateTransition( KPersonalShopFSM::I_TO_CLOSE );
		goto end_proc;
	}

	// ���� Ÿ�� ����
#ifdef SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = static_cast<SEnum::AGENCY_SHOP_TYPE>(kPacket_.m_cPersonalShopType);
#else //SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = static_cast<CXSLSquareUnit::PERSONAL_SHOP_TYPE>(kPacket_.m_cPersonalShopType);
#endif //SERV_UPGRADE_TRADE_SYSTEM

	// ���� ���� �ð� ����
	if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrAgencyOpenDate, m_tAgencyOpenDate ) == false )
	{
		START_LOG( cerr, L"�븮 ���� �����ð� ���� ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_wstrAgencyOpenDate )
			<< END_LOG;

		StateTransition( KPersonalShopFSM::I_TO_CLOSE );
		goto end_proc;
	}

	// ���� ���� �ð� 24�ð� ���� ����
	m_tAgencyOpenOver24Hours = m_tAgencyOpenDate + CTimeSpan( 1, 0, 0, 0 );

	// �븮���� ������ ����
	m_wstrAgencyExpirationDate = kPacket_.m_wstrAgencyExpirationDate;
	if( KncUtil::ConvertStringToCTime( m_wstrAgencyExpirationDate, m_tAgencyExpirationDate ) == false )
	{
		START_LOG( cerr, L"�븮 ���� ������ ���� ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( m_wstrAgencyExpirationDate )
			<< END_LOG;

		StateTransition( KPersonalShopFSM::I_TO_CLOSE );
		goto end_proc;
	}

	//���� �����̸� ����.
	m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;

	// ������ ������ ������Ʈ �Ѵ�!
	m_kPersonalShopItemManager.UpdateSellItemBackupDataList( kPacket_.m_vecSellItemInfo );

	// ��ǰ����� �������� �Ǹ� ���·� �ٲ۴�.
	StateTransition( KPersonalShopFSM::I_TO_SHOP );

	// �븮���� ������ ����Ʈ�� ������ ������ �߰��Ѵ�!
	GetKLoginRoomManager()->AddPShopOwner( kPacket_.m_iUnitUID, GetUID() );

	// ���� �Ǹ����� ���¶�� ���¸� �ٲ۴�!
	if( kPacket_.m_bOnSale )
	{
#ifdef DEF_TRADE_BOARD
		//������ ����� �Ǿ����� ����Ʈ�� �߰�.
		std::vector< KSellPersonalShopItemInfo > vecSellItemList;
		m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( vecSellItemList );
		SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_26, vecSellItemList );		
#endif DEF_TRADE_BOARD
	}
	else
	{
		StateTransition( KPersonalShopFSM::I_TO_STOP_SALE );
	}

	return;

end_proc:
	START_LOG( cerr, L"�������� �ڵ����� �븮 ���� ���� ����!" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< END_LOG;
	return;
}

IMPL_ON_FUNC_NOPARAM( ERM_CHECK_OPEN_PSHOP_AGENCY_REQ )
{
	VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_CHECK_OPEN_PSHOP_AGENCY_ACK, KERM_CHECK_OPEN_PSHOP_AGENCY_ACK );

	// ���� ��ǰ�� �� �ȷȴ��� üũ!
	if( GetStateID() == KPersonalShopFSM::S_SHOP )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_37;
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_55;
	}
 
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHECK_OPEN_PSHOP_AGENCY_ACK, kPacket );
}

//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
_IMPL_ON_FUNC( ERM_CHECK_FOR_REG_PSHOP_AGENCY_ITEM_REQ, KDBE_PREPARE_REG_PSHOP_AGENCY_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_WAIT ), ERM_CHECK_FOR_REG_PSHOP_AGENCY_ITEM_ACK, KEGS_REG_PSHOP_AGENCY_ITEM_ACK );
    
	//�����ڰ� �ƴҰ��..
	if( m_kPersonalShopUserManager.IsPShopAgencyHost( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"���λ��� �����ڰ� �ƴѵ� �ǸŹ�ǰ�� ��� �Ϸ�����.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_08;
		goto end_proc;
	}

	//{{ 2008. 9. 11  ������	���λ��� ����ó��
	BOOST_TEST_FOREACH( const KSellPersonalShopItemInfo&, kSellPShopItemInfo, kPacket_.m_vecSellItemInfo )
	{
		__int64 iPricePerOne  = static_cast<__int64>(kSellPShopItemInfo.m_iPricePerOne);
		__int64 iItemQuantity = static_cast<__int64>(kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity);
		__int64 iCheckTotalPrice = iPricePerOne * iItemQuantity;

		if( iCheckTotalPrice < 0  ||  iCheckTotalPrice > SEnum::UI_MAX_ED_CHAR_HAVE )
		{
			START_LOG( cout, L"�Ǹ� �׼� �� ���� ĳ���Ͱ� ���� ������ ED�Ѱ�ġ�� �Ѿ��. ���������� �ǽɵ�.." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( iPricePerOne )
				<< BUILD_LOG( iItemQuantity )
				<< BUILD_LOG( iCheckTotalPrice );

			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_23;
			goto end_proc;
		}
	}
	//}}

	//��ǰ�� ��ϵǾ� �ִµ� ���� wait�̰� �����Ŷ�� �ٽÿ��� �����޼����� ������ shop ���·� �ٲپ��ش�.
	if( m_kPersonalShopItemManager.IsEmptySellItemList() == false )
	{
		START_LOG( cerr, L"���λ��� ��ǰ�� ��ϵǾ��ִµ� �ٽ� �����Ŷ�� ���ƿ�.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPersonalShopItemManager.GetSellItemCount() )
			<< BUILD_LOG( kPacket_.m_vecSellItemInfo.size() )
			<< END_LOG;

		StateTransition( KPersonalShopFSM::I_TO_SHOP );
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_07;
		goto end_proc;
	}

	// �븮������ �ִ� 9������ ��ǰ ����� ����
	if( static_cast<int>(kPacket_.m_vecSellItemInfo.size()) > SEnum::PAE_SELL_ITEM_LIMIT_NUM )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_09;
		goto end_proc;
	}

	// �˻� �Ϸ������� �ٽ� ���ư���!
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
	kPacket.m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHECK_FOR_REG_PSHOP_AGENCY_ITEM_ACK, kPacket );
}
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
//}}

_IMPL_ON_FUNC( ERM_REG_PSHOP_AGENCY_ITEM_REQ, KERM_REG_PERSONAL_SHOP_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_WAIT ), ERM_REG_PSHOP_AGENCY_ITEM_ACK, KERM_REG_PERSONAL_SHOP_ITEM_ACK );

	//�����ڰ� �ƴҰ��..
	if( m_kPersonalShopUserManager.IsPShopAgencyHost( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"���λ��� �����ڰ� �ƴѵ� �ǸŹ�ǰ�� ��� �Ϸ�����.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_08;
		goto end_proc;
	}

	kPacket.m_iOK = NetError::NET_OK;

	//���� �����̸� ����.
	m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
	
	//{{ 2008. 9. 11  ������	���λ��� ����ó��
	BOOST_TEST_FOREACH( const KSellPersonalShopItemInfo&, kSellPShopItemInfo, kPacket_.m_vecSellItemInfo )
	{
		__int64 iPricePerOne  = static_cast<__int64>(kSellPShopItemInfo.m_iPricePerOne);
		__int64 iItemQuantity = static_cast<__int64>(kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity);
		__int64 iCheckTotalPrice = iPricePerOne * iItemQuantity;

		if( iCheckTotalPrice < 0  ||  iCheckTotalPrice > SEnum::UI_MAX_ED_CHAR_HAVE )
		{
			START_LOG( cout, L"�Ǹ� �׼� �� ���� ĳ���Ͱ� ���� ������ ED�Ѱ�ġ�� �Ѿ��. ���������� �ǽɵ�.." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( iPricePerOne )
				<< BUILD_LOG( iItemQuantity )
				<< BUILD_LOG( iCheckTotalPrice );

			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_23;
			goto end_proc;
		}
	}
	//}}

	//��ǰ�� ��ϵǾ� �ִµ� ���� wait�̰� �����Ŷ�� �ٽÿ��� �����޼����� ������ shop ���·� �ٲپ��ش�.
	if( m_kPersonalShopItemManager.IsEmptySellItemList() == false )
	{
		START_LOG( cerr, L"���λ��� ��ǰ�� ��ϵǾ��ִµ� �ٽ� �����Ŷ�� ���ƿ�! �̹� ù��°�� �˻縦 ���ƴ�! ���� ȣ��Ǿ �ȵǴ� ����!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPersonalShopItemManager.GetSellItemCount() )
			<< BUILD_LOG( kPacket_.m_vecSellItemInfo.size() )
			<< END_LOG;

		StateTransition( KPersonalShopFSM::I_TO_SHOP );
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_07;
		goto end_proc;
	}

	// �븮������ �ִ� 9������ ��ǰ ����� ���� --> 15���� ����. 2013.10
	//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	if( static_cast<int>(kPacket_.m_vecSellItemInfo.size()) > SEnum::PAE_SELL_ITEM_LIMIT_NUM )
#else
	if( static_cast<int>(kPacket_.m_vecSellItemInfo.size()) > PSHOP_AGENCY_ENUM::PAE_SELL_ITEM_LIMIT_NUM )
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}
	{
		START_LOG( cerr, L"�븮���� ��ǰ ��� ���� ������ �ʰ��ߴ�! �������� �˻縦 �����Ƿ� ���� ȣ��Ǿ �ȵǴ� ����!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPersonalShopItemManager.GetSellItemCount() )
			<< BUILD_LOG( kPacket_.m_vecSellItemInfo.size() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_09;
		goto end_proc;
	}
	
	kPacket.m_iOpeningED = 0;	// �븮������ ������� ����..
	kPacket.m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
	kPacket.m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;

	//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
#else
	//����üũ ������ �������� ��ǰ�� ��� ��Ų��.

	m_kPersonalShopItemManager.UpdateSellItemList( kPacket_.m_vecSellItemInfo );
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}

	// DB�� ����Ϸ� ����!
	kPacket.m_iGSUID = LAST_SENDER_UID;
	kPacket.m_iUnitUID = FIRST_SENDER_UID;
	SendToGameDB( DBE_REG_PSHOP_AGENCY_ITEM_REQ, kPacket );
	return;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_REG_PSHOP_AGENCY_ITEM_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_REG_PSHOP_AGENCY_ITEM_ACK, KERM_REG_PERSONAL_SHOP_ITEM_ACK )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_WAIT ), ERM_REG_PSHOP_AGENCY_ITEM_ACK, KERM_REG_PERSONAL_SHOP_ITEM_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
        kPacket_.m_iOK = NetError::ERR_PERSONAL_SHOP_45;

		START_LOG( cerr, L"�븮 ���� ��ǰ DB����� �����Ͽ����ϴ�!" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		// �ѹ�ó��
		m_wstrPersonalShopName = L"";
		m_kPersonalShopItemManager.Clear();
		goto end_proc;
	}

	// ������ ��Ͽ� �����ߴٸ� �븮 ������ ������!
	if( m_kPersonalShopUserManager.LeavePersonalShop( kPacket_.m_iUnitUID ) == false )
	{
		START_LOG( cerr, L"������ ���µ� ���λ��� ������ ��û�� ���Դ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}

	// ��� �����صξ��� ������ ������ �ʱ�ȭ �Ѵ�.
	m_kPersonalShopItemManager.Clear();

	// ���� ���� ������ ������ ������Ʈ �Ѵ�!
	//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	m_kPersonalShopItemManager.UpdateSellItemList( kPacket_.m_vecAddCompleteItemInfo );
#else
	m_kPersonalShopItemManager.UpdateSellItemList( kPacket_.m_vecSellItemInfo );
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}

	// ��ǰ����� �������� �Ǹ� ���·� �ٲ۴�.
	StateTransition( KPersonalShopFSM::I_TO_SHOP );

end_proc:
	SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, ERM_REG_PSHOP_AGENCY_ITEM_ACK, kPacket_ );

#ifdef DEF_TRADE_BOARD
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		//������ ����� �Ǿ����� ����Ʈ�� �߰�.
		std::vector< KSellPersonalShopItemInfo > vecSellItemList;
		m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( vecSellItemList );
		SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_26, vecSellItemList );
	}
#endif DEF_TRADE_BOARD    
}

IMPL_ON_FUNC( ERM_UPDATE_PERIOD_PSHOP_AGENCY_NOT )
{
	switch( GetStateID() )
	{
	case KPersonalShopFSM::S_INIT:
	case KPersonalShopFSM::S_CLOSE:
	case KPersonalShopFSM::S_STOP_SALE:
		{
			START_LOG( cerr, L"�̹� ���� �����ִµ� �븮���� �Ⱓ�� �����Ϸ��� �ϳ�?" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( GetStateIDString() )
				<< END_LOG;
		}
		return;
	}
    
    // �Ⱓ ���� ������Ʈ!	
	if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrAgencyExpirationDate, m_tAgencyExpirationDate ) == false )
	{
		START_LOG( cerr, L"�븮 ���� ������ ���� ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_wstrAgencyExpirationDate )
			<< BUILD_LOG( m_wstrAgencyExpirationDate )
			<< END_LOG;
		return;
	}

	m_wstrAgencyExpirationDate = kPacket_.m_wstrAgencyExpirationDate;
}

_IMPL_ON_FUNC( ERM_JOIN_MY_PSHOP_AGENCY_REQ, KERM_JOIN_PERSONAL_SHOP_REQ )
{
	KEGS_JOIN_MY_PSHOP_AGENCY_ACK kPacket;
	kPacket.m_cJoinType = kPacket_.m_cJoinType;
#ifdef SERV_UPGRADE_TRADE_SYSTEM
    kPacket.m_iUsedItemID = kPacket_.m_iUsedItemID;
#endif //SERV_UPGRADE_TRADE_SYSTEM

	switch( kPacket_.m_cJoinType )
	{
	case KEGS_JOIN_MY_PSHOP_AGENCY_REQ::JT_SHOW_MY_ITEM_LIST:
		{
			if( GetStateID() == KPersonalShopFSM::S_STOP_SALE )
			{
				kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_41;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
				return;
			}
		}
		break;

	case KEGS_JOIN_MY_PSHOP_AGENCY_REQ::JT_FOR_PICK_UP:
		//{{ 2011. 05. 23	������	�븮 ���� ����
#ifdef SERV_PSHOP_AGENCY_NEW
#else
		{
			if( GetStateID() == KPersonalShopFSM::S_SHOP )
			{
				kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_42;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
				return;
			}
		}
#endif SERV_PSHOP_AGENCY_NEW
		//}}		
		break;

	default:
		{
			START_LOG( cerr, L"�̻��� Ÿ������ �����û�� ���Խ��ϴ�!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( kPacket_.m_wstrNickName )
				<< END_LOG;
		}
		return;
	}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( m_kPersonalShopUserManager.IsPShopAgencyHostUserSameServerGroup( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"���λ������� �ش� �������� ������ ���� �� �ֽ��ϴ�." )
			<< BUILD_LOG( GetKLoginServer()->GetServerGroupID() )
			<< BUILD_LOG( KncUid::ExtractServerGroupID( FIRST_SENDER_UID ) )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_DIF_SERVER_GROUP_00;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
		return;
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	// ���� ������ ����Ʈ ��û�� ������ �����ڰ� �ƴ϶�� ���� ��Ų��!
	if( m_kPersonalShopUserManager.IsPShopAgencyHost( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"�븮 ���� �����ڸ� �̿� ���� �մϴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetPShopAgencyHostGSUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_46;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	//������ ������ �����Ѵ�.
	KPersonalShopUserInfo kInfo;
	kInfo.m_iUserUID = kPacket_.m_iUserUID;
	kInfo.m_wstrNickName = kPacket_.m_wstrNickName;
	kInfo.m_iUnitUID = FIRST_SENDER_UID;
	kInfo.m_iGSUID = LAST_SENDER_UID;
#ifdef DEF_TRADE_BOARD
	kInfo.m_bIsSquare = kPacket_.m_bIsSquare;
#endif DEF_TRADE_BOARD

	// ���� join�� �õ��ϴ� ������ �����ڶ�� Enter��Ű�� �ʰ� ��������!
	//{{ 2011. 05. 13	������	�븮 ���� ���� ���� ���� �ذ�
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	if( m_kPersonalShopUserManager.EnterPShopAgencyOwner( kInfo ) == false )
#else
	if( m_kPersonalShopUserManager.EnterPersonalShop( kInfo ) == false )
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	//}}
	{
		START_LOG( cerr, L"�븮���� ������ ���� ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID() )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetMaxMember() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	// ������ �г��� ���
	if( m_kPersonalShopUserManager.GetPShopAgencyHostNickName( kPacket.m_wstrNickName ) == false )
	{
		START_LOG( cerr, L"ȣ��Ʈ �г��� ��� ����.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_wstrPersonalShopName = m_wstrPersonalShopName;
	//{{ 2011. 05. 23	������	�븮 ���� ����
#ifdef SERV_PSHOP_AGENCY_NEW
	if( kPacket_.m_cJoinType == KEGS_JOIN_MY_PSHOP_AGENCY_REQ::JT_SHOW_MY_ITEM_LIST )
	{
		// �Ǹ�����UI������ �Ǹ����� ��ǰ�� �����ش�.
		m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( kPacket.m_vecSellItemInfo, KPersonalShopItemManager::ILT_ONLY_ON_SALE_ITEMS );
	}
	else
	{
		if( GetStateID() == KPersonalShopFSM::S_SHOP )
		{
			// �Ǹ����� ��ǰ�ޱ�UI������ ���ǵ� ��ǰ�� �����ش�.
			m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( kPacket.m_vecSellItemInfo, KPersonalShopItemManager::ILT_ONLY_SOLD_OUT_ITEMS );
		}
		else
		{
			// �Ǹ� ������ ���¿����� ��ǰ�ޱ�UI������ ��� ��ǰ�� �� �����ش�.
			m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( kPacket.m_vecSellItemInfo, KPersonalShopItemManager::ILT_ALL_ITEMS );	
		}		
	}

	if( kPacket.m_vecSellItemInfo.empty() == true )
	{
		if( m_kPersonalShopUserManager.LeavePersonalShop( FIRST_SENDER_UID ) == false )
		{
			START_LOG( cerr, L"�븮���� ������ ����." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
		}

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_52;
        BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_21, LAST_SENDER_UID, FIRST_SENDER_UID );

	}
#else
	if( kPacket_.m_cJoinType == KEGS_JOIN_MY_PSHOP_AGENCY_REQ::JT_SHOW_MY_ITEM_LIST )
	{
		m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( kPacket.m_vecSellItemInfo, false ); // �Ǹ�����UI������ ���ǵ� ��ǰ�� �����Ѵ�.
	}
	else
	{	
		m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( kPacket.m_vecSellItemInfo, true );	// ��ǰ�ޱ�UI������ ��� ��ǰ�� �� �����ش�.
	}
#endif SERV_PSHOP_AGENCY_NEW
	//}}
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_JOIN_PERSONAL_SHOP_REQ )
{
#ifdef DEF_TRADE_BOARD
	KERM_JOIN_PERSONAL_SHOP_ACK kPacket;
	switch( GetStateID() )
	{
	case KPersonalShopFSM::S_INIT:
	case KPersonalShopFSM::S_CLOSE:
	case KPersonalShopFSM::S_WAIT:
	case KPersonalShopFSM::S_STOP_SALE:
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_32;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_PERSONAL_SHOP_ACK, kPacket );
		}
		return;
	}
#else
	//VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_JOIN_PERSONAL_SHOP_ACK, KERM_JOIN_PERSONAL_SHOP_ACK );
#endif

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( m_kPersonalShopUserManager.IsPShopAgencyHostUserSameServerGroup( kPacket_.m_iUnitUID ) == false )
	{
		START_LOG( cerr, L"���λ������� �ش� �������� ������ ���� �� �ֽ��ϴ�." )
			<< BUILD_LOG( GetKLoginServer()->GetServerGroupID() )
			<< BUILD_LOG( KncUid::ExtractServerGroupID( kPacket_.m_iUnitUID ) )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_DIF_SERVER_GROUP_00;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_PERSONAL_SHOP_ACK, kPacket );
		return;
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	kPacket.m_iOK = NetError::NET_OK;

	//������ ������ �����Ѵ�.
	KPersonalShopUserInfo kInfo;
	kInfo.m_iUserUID = kPacket_.m_iUserUID;
	kInfo.m_wstrNickName = kPacket_.m_wstrNickName;
	kInfo.m_iUnitUID = FIRST_SENDER_UID;
	kInfo.m_iGSUID = LAST_SENDER_UID;
#ifdef DEF_TRADE_BOARD
	kInfo.m_bIsSquare = kPacket_.m_bIsSquare;
#endif DEF_TRADE_BOARD

	// ���� join�� �õ��ϴ� ������ �����ڶ�� Enter��Ű�� �ʰ� ��������!
	//{{ 2011. 05. 13	������	�븮 ���� ���� ���� ���� �ذ�
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	if( m_kPersonalShopUserManager.EnterPShopAgencyCustomer( kInfo ) == false )
#else
	if( m_kPersonalShopUserManager.EnterPersonalShop( kInfo ) == false )
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	//}}
	{
		START_LOG( cwarn, L"���λ��� ���� �߰� ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID() )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetMaxMember() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		goto end_proc;
	}	

	kPacket.m_iPersonalShopUID = GetUID();
	kPacket.m_wstrPersonalShopName = m_wstrPersonalShopName;
	//{{ 2011. 07. 19	������	�븮���� ���� ������ ���� �̺�Ʈ
#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	kPacket.m_bIsPShopAgency = true;
#endif SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	//}}

	if( m_kPersonalShopUserManager.GetPShopAgencyHostNickName( kPacket.m_wstrNickName ) == false )
	{
		START_LOG( cerr, L"ȣ��Ʈ �г��� ��� ����.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}

	m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( kPacket.m_vecSellItemInfo );

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_PERSONAL_SHOP_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_LEAVE_PSHOP_AGENCY_REQ, KERM_LEAVE_PERSONAL_SHOP_REQ )
{
	VERIFY_ROOM_STATE( ( 3, KPersonalShopFSM::S_WAIT, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_LEAVE_PSHOP_AGENCY_ACK, KEGS_LEAVE_PSHOP_AGENCY_ACK );

	// ������ ���� ��Ż ó��
	if( m_kPersonalShopUserManager.LeavePersonalShop( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"������ ���µ� ���λ��� ������ ��û�� ���Դ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_01 )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_12;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_PSHOP_AGENCY_ACK, kPacket );
		}
	}
	else
	{
		if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_01 )
		{
			kPacket.m_iOK = NetError::NET_OK;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_PSHOP_AGENCY_ACK, kPacket );
		}
	}

	// ��Ż�� ������ ���������� Ȯ��!
	const bool bIsHost = m_kPersonalShopUserManager.IsPShopAgencyHost( FIRST_SENDER_UID );
	if( bIsHost )
	{
		// �Ǹ����� ��ǰ�� �ϳ��� ���ٸ� ���� �ݴ´�!
		if( m_kPersonalShopItemManager.IsEmptySellItemList() == true )
		{
			BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_21, LAST_SENDER_UID, FIRST_SENDER_UID );
		}
	}
}

IMPL_ON_FUNC( ERM_LEAVE_PERSONAL_SHOP_REQ )
{
	VERIFY_ROOM_STATE( ( 3, KPersonalShopFSM::S_WAIT, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_LEAVE_PERSONAL_SHOP_ACK, KEGS_LEAVE_PERSONAL_SHOP_ACK );

	// ������ ���� ��Ż ó��
	if( m_kPersonalShopUserManager.LeavePersonalShop( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"������ ���µ� ���λ��� ������ ��û�� ���Դ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_01 )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_12;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_PERSONAL_SHOP_ACK, kPacket );
		}
	}
	else
	{
		if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_01 )
		{
			kPacket.m_iOK = NetError::NET_OK;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_PERSONAL_SHOP_ACK, kPacket );
		}
	}

	if( m_kPersonalShopUserManager.IsPShopAgencyHost( FIRST_SENDER_UID ) == true )
	{
		// ȣ��Ʈ�� �������� �ϴ� ���
		START_LOG( cerr, L"�����ڰ� �� ��Ŷ�� �� �! ���״�! ��·�� üũ�� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( ERM_BUY_PERSONAL_SHOP_ITEM_REQ )
{
	// �Ǹ� ���� ���¶�� ���� ���� ������ ������!
	if( GetStateID() == KPersonalShopFSM::S_STOP_SALE )
	{
		KEGS_BUY_PERSONAL_SHOP_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_PERSONAL_SHOP_36;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacketAck );
		return;
	}

	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_SHOP ), ERM_BUY_PERSONAL_SHOP_ITEM_ACK, KEGS_BUY_PERSONAL_SHOP_ITEM_ACK );

	// ȣ��Ʈ ���� ���
	const UidType iHostUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
	if( iHostUnitUID == 0 )
	{
		START_LOG( cerr, L"ȣ��Ʈ ������ �����ϴ�. �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iBuyQuantity )
			<< BUILD_LOG( kPacket_.m_iBuyItemUID )
			<< END_LOG;

		KEGS_BUY_PERSONAL_SHOP_ITEM_ACK kPacketAck;
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_20;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacketAck );
		return;
	}

	if( iHostUnitUID == 0 )
	{
		START_LOG( cerr, L"ȣ��Ʈ ������ �����ϴ�. �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iBuyQuantity )
			<< BUILD_LOG( kPacket_.m_iBuyItemUID )
			<< END_LOG;

		KEGS_BUY_PERSONAL_SHOP_ITEM_ACK kPacketAck;
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_20;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacketAck );
		return;
	}

	// ���� �� ó��
	KDBE_BUY_PSHOP_AGENCY_ITEM_REQ kPacketToDB;

	if( m_kPersonalShopItemManager.PrepareForBuyItem( iHostUnitUID, m_PersonalShopType, kPacket_, kPacketToDB ) == false )
	{
		START_LOG( cwarn, L"���� ����!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iBuyQuantity )
			<< BUILD_LOG( kPacket_.m_iBuyItemUID )
			<< BUILD_LOG( iHostUnitUID )
			<< BUILD_LOG( m_PersonalShopType )
			<< END_LOG;

		KEGS_BUY_PERSONAL_SHOP_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacketAck );
		return;
	}	
	
	kPacketToDB.m_iHostUnitUID = iHostUnitUID;
	kPacketToDB.m_iPriceGSUID = LAST_SENDER_UID;
	kPacketToDB.m_iPriceUnitUID = FIRST_SENDER_UID;

	//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	kPacketToDB.m_kSellItemDBUpdate = kPacketToDB.m_kPriceIntemInfoIN;

	// ���� ���̹Ƿ� ���̳ʽ� ������ ��ȯ
	kPacketToDB.m_iPriceUnitEDOUT = (-1) * kPacketToDB.m_iPriceUnitEDOUT;
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}

	//////////////////////////////////////////////////////////////////////////	
	//��� ���Ż��� üũ�� �������� �븮 ���� DB�� ���� ���� DB������ ������!
	SendToGameDB( DBE_BUY_PSHOP_AGENCY_ITEM_REQ, kPacketToDB );
}

_IMPL_ON_FUNC( DBE_BUY_PSHOP_AGENCY_ITEM_ACK, KDBE_BUY_PSHOP_AGENCY_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_BUY_PERSONAL_SHOP_ITEM_ACK, KEGS_BUY_PERSONAL_SHOP_ITEM_ACK );

	// ������ ��쿡 ���� ����ó��
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"DB���� ���� ��û ó���� �����Ͽ����ϴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		// �ѹ� ó��
		//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		m_kPersonalShopItemManager.Confirm( kPacket_.m_kSellItemDBUpdate.m_iItemUID, false );
#else
		m_kPersonalShopItemManager.Confirm( kPacket_.m_kPriceIntemInfoIN.m_iItemUID, false );
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}

		// ���� ��ȯ
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_15;
		SendToGSCharacter( kPacket_.m_iPriceGSUID, kPacket_.m_iPriceUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );
		return;
	}

	// ���� ������ ���� ����!
	//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	if( m_kPersonalShopItemManager.Confirm( kPacket_.m_kSellItemDBUpdate.m_iItemUID, true ) == false )
	{
		START_LOG( cerr, L"������ �����ߴ�! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_kSellItemDBUpdate.m_iItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_15;
		SendToGSCharacter( kPacket_.m_iPriceGSUID, kPacket_.m_iPriceUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );
		return;
	}
#else
	if( m_kPersonalShopItemManager.Confirm( kPacket_.m_kPriceIntemInfoIN.m_iItemUID, true ) == false )
	{
		START_LOG( cerr, L"������ �����ߴ�! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_kPriceIntemInfoIN.m_iItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_15;
		SendToGSCharacter( kPacket_.m_iPriceGSUID, kPacket_.m_iPriceUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );
		return;
	}
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}

	//////////////////////////////////////////////////////////////////////////
	// �ȸ� ���� ����Ʈ �ű��	

	// 1. �����ڿ��� �̺�Ʈ ����
	{
		KERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT kCompleteNot;
		kCompleteNot.m_iSellUnitUID			= 0; // ���� �ʿ䰡 ����!
		kCompleteNot.m_iSellUnitEDIN		= 0; // ���� �ʿ䰡 ����!
		kCompleteNot.m_iSellCommissionED	= 0; // ���� �ʿ䰡 ����!
		kCompleteNot.m_kSellIntemInfoOUT;		 // ���� �ʿ䰡 ����!

		kCompleteNot.m_iPriceUnitUID		= kPacket_.m_iPriceUnitUID;
		kCompleteNot.m_iPriceUnitEDOUT		= kPacket_.m_iPriceUnitEDOUT;
		//{{ 2010. 03. 31  ������	�ŷ��Խ���
		kCompleteNot.m_iPriceCommissionED	= kPacket_.m_iPriceCommissionED;
		//}}
		kCompleteNot.m_kPriceIntemInfoIN	= kPacket_.m_kPriceIntemInfoIN;
		//{{ 2010. 03. 31  ������	�ŷ��Խ���
#ifdef DEF_TRADE_BOARD
		kCompleteNot.m_bIsSquare = false;
#endif DEF_TRADE_BOARD
		//}}
		// GSUser��ü �Ҹ�� ���� DB������Ʈ�� ����
		kCompleteNot.m_kItemQuantityUpdateTemp  = kPacket_.m_kItemQuantityUpdateTemp;
		kCompleteNot.m_vecItemInfoTemp			= kPacket_.m_vecItemInfoTemp;
		kCompleteNot.m_iEDTemp					= kPacket_.m_iEDTemp;
		SendToGSCharacter( kPacket_.m_iPriceGSUID, kPacket_.m_iPriceUnitUID, ERM_BUY_PSHOP_AGENCY_ITEM_COMPLETE_NOT, kCompleteNot );

		//#5. Ŭ���̾�Ʈ�� OK�� ���� - �����ŷ� ������ ���� ������Ʈ �Ǿ�� ���׸� ������ �� �ִ�!
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( kPacket_.m_iPriceGSUID, kPacket_.m_iPriceUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );
	}	

	// 2. ������ �����ִ� �����鿡�� ����� ���� ���� ����
	{
		const UidType iHostUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
		LIF( iHostUnitUID != 0 );

		KSellPersonalShopItemInfo kSellItemInfo;
		//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		if( m_kPersonalShopItemManager.GetSellPersonalShopItemInfo( kPacket_.m_kSellItemDBUpdate.m_iItemUID, kSellItemInfo ) == false )
#else
		if( m_kPersonalShopItemManager.GetSellPersonalShopItemInfo( kPacket_.m_kPriceIntemInfoIN.m_iItemUID, kSellItemInfo ) == false )
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}
		{
			START_LOG( cerr, L"������ ������ �������� �ʽ��ϴ�! �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( kPacket_.m_kPriceIntemInfoIN.m_iItemUID )
				<< END_LOG;
		}

		// ���� UI�� ���� ������ ���� ����!
		KEGS_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT kUpdateNot;
		kUpdateNot.m_iItemUID = kSellItemInfo.m_kInventoryItemInfo.m_iItemUID;
		kUpdateNot.m_iQuantity = kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity;
		BroadCast( ERM_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT, kUpdateNot );
		//BroadCastWithOutOneChar( ERM_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT, kUpdateNot, iHostUnitUID );

		// �����ڿ��� �Ǹ� ���� �˷��ֱ����� ����!
		if( m_kPersonalShopItemManager.IsAllItemSoldOut() == true )
		{
			SendPShopAgencyMessage( KEGS_PSHOP_AGENCY_MESSAGE_NOT::PAMT_SOLD_OUT, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iItemID, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity, kPacket_.m_iSellUnitEDIn );
		}
		
		SendPShopAgencyMessage( KEGS_PSHOP_AGENCY_MESSAGE_NOT::PAMT_SELL_ITEM, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iItemID, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity, kPacket_.m_iSellUnitEDIn );

		// �����ڿ��� ���� ����!
		{
			CStringW cwstrItemID;
			cwstrItemID.Format( L"%d", kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iItemID );

			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = iHostUnitUID;
			kPacketToDB.m_iToUnitUID = iHostUnitUID;
			kPacketToDB.m_iRewardType = KPostItemInfo::LT_SELL_ITEM_PSHOP;
			kPacketToDB.m_wstrMessage = cwstrItemID.GetBuffer();
			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}

		//�Ǹ��� �����ۼ����� ������ �������ش�.
		if( kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_QUANTITY )
		{
			if( kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity == 0 )
			{
#ifdef DEF_TRADE_BOARD
				SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, kSellItemInfo );
#endif DEF_TRADE_BOARD
			}

#ifdef DEF_TRADE_BOARD
			SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_27, kSellItemInfo );
#endif DEF_TRADE_BOARD
		}
		else
		{
#ifdef DEF_TRADE_BOARD
			SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, kSellItemInfo );
#endif DEF_TRADE_BOARD
		}

		//{{ 2008. 2. 26  ������  ���λ��� �α�
		KELOG_INSERT_PERSONAL_SHOP_LOG_NOT kPacketLog;
		kPacketLog.m_cPShopType	  = SEnum::PST_PSHOP_AGENCY;
		kPacketLog.m_bIsSquare	  = false;
		kPacketLog.m_iFromUnitUID = iHostUnitUID;
		kPacketLog.m_iToUnitUID	  = kPacket_.m_iPriceUnitUID;
		kPacketLog.m_iItemUID	  = kUpdateNot.m_iItemUID;
		kPacketLog.m_iItemID	  = kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID;
		kPacketLog.m_iQuantity	  = kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity;
		kPacketLog.m_iED		  = kPacket_.m_iSellUnitEDIn;
		SendToLogDB( ELOG_INSERT_PERSONAL_SHOP_LOG_NOT, kPacketLog );
		//}}
	}

	// 3. ��� ��ǰ�� �ȷ������ ���λ��� �ݾ��ֱ�.
	if( m_kPersonalShopItemManager.IsAllItemSoldOut() == true )
	{
		// ��� �����۵��� �� �ȷȴ�! �ڵ����� �Ǹ����� ���·� ����!
		StateTransition( KPersonalShopFSM::I_TO_STOP_SALE );

		// DB�� �Ǹ� ���� ������Ʈ �Ϸ� ����!
		KDBE_STOP_SALE_PSHOP_AGENCY_REQ kPacketToDB;
		kPacketToDB.m_iGSUID = m_kPersonalShopUserManager.GetPShopAgencyHostGSUID();
		kPacketToDB.m_iUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
		kPacketToDB.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_26;
		SendToGameDB( DBE_STOP_SALE_PSHOP_AGENCY_REQ, kPacketToDB );
	}
}

IMPL_ON_FUNC_NOPARAM( ERM_STOP_SALE_PSHOP_AGENCY_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_SHOP ), ERM_STOP_SALE_PSHOP_AGENCY_ACK, KEGS_STOP_SALE_PSHOP_AGENCY_ACK );

	const bool bIsHost = m_kPersonalShopUserManager.IsPShopAgencyHost( FIRST_SENDER_UID );
	if( bIsHost == false )
	{
		START_LOG( cerr, L"�����ڵ� �ƴϸ鼭 �Ǹ� ������ �ϳ�?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_46;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STOP_SALE_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	// �Ǹ� ���� ���·� �̵�!
	StateTransition( KPersonalShopFSM::I_TO_STOP_SALE );

	// DB�� �Ǹ� ���� ������Ʈ �Ϸ� ����!
	KDBE_STOP_SALE_PSHOP_AGENCY_REQ kPacketToDB;
	kPacketToDB.m_iGSUID = LAST_SENDER_UID;
	kPacketToDB.m_iUnitUID = FIRST_SENDER_UID;
	kPacketToDB.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_21;
	SendToGameDB( DBE_STOP_SALE_PSHOP_AGENCY_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_STOP_SALE_PSHOP_AGENCY_ACK )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_STOP_SALE ), ERM_STOP_SALE_PSHOP_AGENCY_ACK, KEGS_STOP_SALE_PSHOP_AGENCY_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"�븮���� �Ǹ� ���� ���� ȣ�� ����! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		// �ϴ� �ٽ� ���¸� ���� ��Ų��!
		StateTransition( KPersonalShopFSM::I_TO_SHOP );

		kPacket.m_iOK = kPacket_.m_iOK;
		if( kPacket_.m_iGSUID != 0 )
		{
			SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, ERM_STOP_SALE_PSHOP_AGENCY_ACK, kPacket );
		}		
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	if( kPacket_.m_iGSUID != 0 )
	{
		SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, ERM_STOP_SALE_PSHOP_AGENCY_ACK, kPacket );
	}	

	// �Ǹ� ������ �����ߴٸ� ���� �������� �������� �����ڸ� �����ϰ� ��� Break�� ������.
	KERM_BREAK_PERSONAL_SHOP_NOT kPacketNot;
	kPacketNot.m_iReason = kPacket_.m_iReason;
	kPacketNot.m_iHostUID = kPacket_.m_iUnitUID;
	BroadCastWithOutOneChar( ERM_BREAK_PERSONAL_SHOP_NOT, kPacketNot, kPacket_.m_iUnitUID );

	// �Ǹ������� �����ߴٸ� �ŷ��Խ��ǿ����� ��������!
	std::vector< KSellPersonalShopItemInfo > vecSellItemInfo;
	//{{ 2011. 05. 23	������	�븮 ���� ����
#ifdef SERV_PSHOP_AGENCY_NEW
	m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( vecSellItemInfo, KPersonalShopItemManager::ILT_ALL_ITEMS );
#else
	m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( vecSellItemInfo, true );
#endif SERV_PSHOP_AGENCY_NEW
	//}}	
	SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, vecSellItemInfo );

	// ���� �����ϱ�!
	KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
	kPacketToDB.m_iFromUnitUID = kPacket_.m_iUnitUID;
	kPacketToDB.m_iToUnitUID = kPacket_.m_iUnitUID;
	kPacketToDB.m_iRewardType = KPostItemInfo::LT_STOP_SALE_PSHOP;
	SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
}

_IMPL_ON_FUNC( ERM_PICK_UP_FROM_PSHOP_AGENCY_REQ, KEGS_PICK_UP_FROM_PSHOP_AGENCY_REQ )
{
	//{{ 2011. 05. 23	������	�븮 ���� ����
#ifdef SERV_PSHOP_AGENCY_NEW
	VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#else
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#endif SERV_PSHOP_AGENCY_NEW
	//}}

	// �븮 ���� ���������� üũ!
	const bool bIsHost = m_kPersonalShopUserManager.IsPShopAgencyHost( FIRST_SENDER_UID );
	if( bIsHost == false )
	{
		START_LOG( cerr, L"�����ڵ� �ƴϸ鼭 ��ǰ �������⸦ �ϳ�?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_46;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	// ��ǰ �������� ��û
	KERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_REQ kPacketToGS;

	if( m_kPersonalShopItemManager.PrepareForPickUp( kPacket_.m_vecPickUpItemList, kPacketToGS ) == false )
	{
		START_LOG( cerr, L"�븮���� ��ǰ �������� �غ� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_vecPickUpItemList.size() )
            << BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	// ��ǰ �������� ���� �κ��丮 ���� üũ�� �Ѵ�!
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_REQ, kPacketToGS );
}

//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
IMPL_ON_FUNC( ERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_ACK )
{
	//{{ 2011. 05. 23	������	�븮 ���� ����
#ifdef SERV_PSHOP_AGENCY_NEW
	VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#else
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#endif SERV_PSHOP_AGENCY_NEW
	//}}	

	KDBE_PICK_UP_FROM_PSHOP_AGENCY_REQ kPacketToDB;

	// pick up
	if( m_kPersonalShopItemManager.PickUp( kPacket_.m_vecPickUpItemList, kPacketToDB ) == false )
	{
		START_LOG( cerr, L"�븮���� ��ǰ �������� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_vecPickUpItemList.size() )
			<< BUILD_LOG( kPacketToDB.m_vecPickUpItemList.size() )
            << BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	kPacketToDB.m_iGSUID = LAST_SENDER_UID;
	kPacketToDB.m_iUnitUID = FIRST_SENDER_UID;

	BOOST_TEST_FOREACH( const KSellPersonalShopItemInfo&, kItem, kPacketToDB.m_vecPickUpItemList )
	{
#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		if ( kPacket_.m_cShopType <= SEnum::AST_NORMAL ) 
#else // SERV_UPGRADE_TRADE_SYSTEM
		if ( kPacket_.m_cShopType <= CXSLSquareUnit::PST_NORMAL ) 
#endif // SERV_UPGRADE_TRADE_SYSTEM
        {
            kPacketToDB.m_iSellItemTotalED += ( kItem.m_iTotalSellEDIn * 0.9 );
        }
        else 
        {
            kPacketToDB.m_iSellItemTotalED += kItem.m_iTotalSellEDIn;
        }
        
	}

	SendToGameDB( DBE_PICK_UP_FROM_PSHOP_AGENCY_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_PICK_UP_FROM_PSHOP_AGENCY_ACK )
{
	//{{ 2011. 05. 23	������	�븮 ���� ����
#ifdef SERV_PSHOP_AGENCY_NEW
	VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#else
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#endif SERV_PSHOP_AGENCY_NEW
	//}}	


	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// pick up fail
		m_kPersonalShopItemManager.UnPickUp( kPacket_.m_vecPickUpFailList );

		SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	std::vector<UidType> vecPickUpItemList;
	BOOST_TEST_FOREACH( const KSellPersonalShopItemInfo&, kItem, kPacket_.m_vecPickUpItemList )
	{
		vecPickUpItemList.push_back( kItem.m_kInventoryItemInfo.m_iItemUID );
	}

	// pick up complete
	KPersonalShopItemManager::ITEM_LIST_TYPE eType;
	if( GetStateID() == KPersonalShopFSM::S_SHOP )
	{
		// ���� ������ �Ǹ����� ���¶�� ���ǵ� ������ ������ ����!
		eType = KPersonalShopItemManager::ILT_ONLY_SOLD_OUT_ITEMS;
	}
	else
	{
		// ���� ������ �Ǹ��ߴ��� ���¶�� ��� ������ ������ ����!
		eType = KPersonalShopItemManager::ILT_ALL_ITEMS;
	}

	m_kPersonalShopItemManager.PickUpComplete( vecPickUpItemList, kPacket, eType );

	BOOST_TEST_FOREACH( KSellPersonalShopItemInfo&, kItem, kPacket_.m_vecPickUpItemList )
	{
		if( kItem.m_iInventoryItemUID != 0  )
		{
			kItem.m_kInventoryItemInfo.m_iItemUID = kItem.m_iInventoryItemUID;
			kPacket.m_vecPickUpSuccessItemList.push_back( kItem );
		}
	}

	kPacket.m_iTotalSellEDIn = kPacket_.m_iSellItemTotalED;

	// Pick Up ����� GSUser���� ������!
	kPacket.m_iOK = NetError::NET_OK;

    if ( m_kPersonalShopItemManager.IsEmptySellItemList() == true ) 
    {
        kPacket.m_bRemainSellItem = false;
    }
    else 
    {
        kPacket.m_bRemainSellItem = true;
    }

	SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );

	// �Ǹ����� ��ǰ�� �ϳ��� ���ٸ� ���� �ݴ´�!
	if( m_kPersonalShopItemManager.IsEmptySellItemList() == true )
	{
		BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_21, kPacket_.m_iGSUID, kPacket_.m_iUnitUID );
	}
	//{{ 2011. 05. 24	������	�븮 ���� ����
#ifdef SERV_PSHOP_AGENCY_NEW
	else
	{
		// ���ǵ� �������� �� ���ҳ� ã�ƺ���!
		if( m_kPersonalShopItemManager.IsAllItemOnSale() == true )
		{
			// �� �ȳ������� UI�� �ڵ����� �����Ƿ� �濡���� ��Żó�� ����!
			if( m_kPersonalShopUserManager.LeavePersonalShop( kPacket_.m_iUnitUID ) == false )
			{
				START_LOG( cerr, L"�븮���� ������ ����." )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< END_LOG;
			}
		}
	}
#endif SERV_PSHOP_AGENCY_NEW
	//}}
}
#else
IMPL_ON_FUNC( ERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_ACK )
{
	//{{ 2011. 05. 23	������	�븮 ���� ����
#ifdef SERV_PSHOP_AGENCY_NEW
	VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#else
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#endif SERV_PSHOP_AGENCY_NEW
	//}}	

	KDBE_PICK_UP_FROM_PSHOP_AGENCY_REQ kPacketToDB;

	// pick up
	if( m_kPersonalShopItemManager.PickUp( kPacket_.m_vecPickUpItemList, kPacketToDB ) == false )
	{
		START_LOG( cerr, L"�븮���� ��ǰ �������� �غ� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_vecPickUpItemList.size() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	kPacketToDB.m_iGSUID = LAST_SENDER_UID;
	kPacketToDB.m_iUnitUID = FIRST_SENDER_UID;
	SendToGameDB( DBE_PICK_UP_FROM_PSHOP_AGENCY_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_PICK_UP_FROM_PSHOP_AGENCY_ACK )
{
	//{{ 2011. 05. 23	������	�븮 ���� ����
#ifdef SERV_PSHOP_AGENCY_NEW
	VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#else
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#endif SERV_PSHOP_AGENCY_NEW
	//}}	

	// ������� ������ �����ѰŴ�!

	// pick up fail
	m_kPersonalShopItemManager.UnPickUp( kPacket_.m_vecPickUpFailList );

	// pick up complete
	//{{ 2011. 05. 23	������	�븮 ���� ����
#ifdef SERV_PSHOP_AGENCY_NEW
	KPersonalShopItemManager::ITEM_LIST_TYPE eType;
	if( GetStateID() == KPersonalShopFSM::S_SHOP )
	{
		// ���� ������ �Ǹ����� ���¶�� ���ǵ� ������ ������ ����!
		eType = KPersonalShopItemManager::ILT_ONLY_SOLD_OUT_ITEMS;
	}
	else
	{
		// ���� ������ �Ǹ��ߴ��� ���¶�� ��� ������ ������ ����!
		eType = KPersonalShopItemManager::ILT_ALL_ITEMS;
	}

	m_kPersonalShopItemManager.PickUpComplete( kPacket_.m_vecPickUpItemList, kPacket, eType );
#else
	m_kPersonalShopItemManager.PickUpComplete( kPacket_.m_vecPickUpItemList, kPacket );
#endif SERV_PSHOP_AGENCY_NEW
	//}}	

	// Pick Up ����� GSUser���� ������!
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );

	// �Ǹ����� ��ǰ�� �ϳ��� ���ٸ� ���� �ݴ´�!
	if( m_kPersonalShopItemManager.IsEmptySellItemList() == true )
	{
		BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_21, kPacket_.m_iGSUID, kPacket_.m_iUnitUID );
	}
	//{{ 2011. 05. 24	������	�븮 ���� ����
#ifdef SERV_PSHOP_AGENCY_NEW
	else
	{
		// ���ǵ� �������� �� ���ҳ� ã�ƺ���!
		if( m_kPersonalShopItemManager.IsAllItemOnSale() == true )
		{
			// �� �ȳ������� UI�� �ڵ����� �����Ƿ� �濡���� ��Żó�� ����!
			if( m_kPersonalShopUserManager.LeavePersonalShop( kPacket_.m_iUnitUID ) == false )
			{
				START_LOG( cerr, L"�븮���� ������ ����." )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< END_LOG;
			}
		}
	}
#endif SERV_PSHOP_AGENCY_NEW
	//}}
}
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
//}}


#endif SERV_PSHOP_AGENCY
//}}

//{{ 2011. 05. 13	������	�븮 ���� ���� ���� ���� �ذ�
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
IMPL_ON_FUNC( ERM_CHECK_ZOMBIE_USER_ACK )
{
	switch( kPacket_.m_cZombieCheckType )
	{
	case KERM_CHECK_ZOMBIE_USER_ACK::ZCT_CONNECT_ZOMBIE:
	case KERM_CHECK_ZOMBIE_USER_ACK::ZCT_DISCONNECT_ZOMBIE:
	case KERM_CHECK_ZOMBIE_USER_ACK::ZCT_JOIN_DIF_ROOM:
		{
			// �ش� ĳ���Ͱ� �����ִ��� �˻�!
			if( m_kPersonalShopUserManager.IsExistUnitUID( FIRST_SENDER_UID ) == true )
			{
				m_kPersonalShopUserManager.LeavePersonalShop( FIRST_SENDER_UID );

				START_LOG( cout2, L"���� ������ ���� �մϴ�!" )
					<< BUILD_LOGc( kPacket_.m_cZombieCheckType )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( GetStateIDString() )
					<< BUILD_LOG( m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID() )
					<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
					<< BUILD_LOG( m_kPersonalShopItemManager.GetSellItemCount() )
					<< BUILD_LOG( m_kPersonalShopItemManager.IsAllItemSoldOut() )
					<< BUILD_LOG( m_kPersonalShopItemManager.IsAllItemOnSale() )
					<< BUILD_LOG( FIRST_SENDER_UID );
			}
		}
		break;
	}
}
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
//}}


IMPL_ON_FUNC( ERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ )
{
    std::vector< KSellPShopItemBackupData > vecMissingSellItemInfo;
    BOOST_TEST_FOREACH( KSellPShopItemBackupData&, kItem, kPacket_.m_vecSellItemInfo )
    {
        if( false == m_kPersonalShopItemManager.IsExistItem( kItem.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID ) )
        {
            vecMissingSellItemInfo.push_back( kItem );
        }
        
    }
    if ( !vecMissingSellItemInfo.empty() ) 
    {
        START_LOG( cout, L"�븮������ ��ϵ� �������� DB �����۰� �ٸ� ��� �߻��Ͽ� �߰�" )
            << BUILD_LOG( FIRST_SENDER_UID )
            << BUILD_LOG( LAST_SENDER_UID )
            << BUILD_LOG( vecMissingSellItemInfo.size() )
            << BUILD_LOG( m_kPersonalShopItemManager.GetSellItemCount() );
            
        m_kPersonalShopItemManager.UpdateSellItemBackupDataList( vecMissingSellItemInfo );
    }
}
