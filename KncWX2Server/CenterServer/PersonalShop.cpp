#include ".\PersonalShop.h"

#include "Room/PersonalShopFSM.h"
#include "NetError.h"
#include "X2Data/XSLRoom.h"
#include "X2data/XSLItem.h"
//{{ 2010. 05. 18  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	#include "CnServer.h"
#endif SERV_INTEGRATION
//}}
//{{ 2011. 05. 11	������	�븮����
#ifdef SERV_PSHOP_AGENCY
	#include "Enum/Enum.h"
#endif SERV_PSHOP_AGENCY
//}}

#define CLASS_TYPE KPersonalShop

NiImplementRTTI( KPersonalShop, KCnBaseRoom );

ImplToStringW( KPersonalShop )
{
	return START_TOSTRING_PARENTW( KCnBaseRoom );
}

KPersonalShop::KPersonalShop(void)
{
	m_kPersonalShopUserManager.Init( 8 );

	m_wstrPersonalShopName.clear();

#ifdef SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = SEnum::AST_NONE;
#else //SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = CXSLSquareUnit::PST_NONE;
#endif //SERV_UPGRADE_TRADE_SYSTEM
}

KPersonalShop::~KPersonalShop(void)
{
}

void KPersonalShop::CloseRoom()
{
	m_kPersonalShopUserManager.Reset();

	m_vecSellItemInfo.clear();
	m_wstrPersonalShopName.clear();

	KCnBaseRoom::CloseRoom();
	return;
}

void KPersonalShop::Tick()
{
	KCnBaseRoom::Tick();

	switch( GetStateID() )
	{
	case KPersonalShopFSM::S_INIT:
		break;

	case KPersonalShopFSM::S_CLOSE:
		{
			CloseRoom();
			//{{ 2010. 03. 14  ������	�� �й� ��� ����
#ifdef SERV_ROOM_COUNT
			SendRoomCountInfo( CXSLRoom::RT_PERSONAL_SHOP, false );
#endif SERV_ROOM_COUNT
			//}}
		}
		break;

	case KPersonalShopFSM::S_WAIT:
		// ���������� �ϰ� �����ð��ȿ� ��ǰ����� ���� ������� �ݾƹ�����..
		if( m_kPersonalShopUserManager.GetNumMember() <= 1 )
		{
			if( m_kTimer[TM_WAIT_OTHER].elapsed() >= 1800.0 )
			{
				UidType iHostUID = 0;
				if( m_kPersonalShopUserManager.GetHostUID( iHostUID ) == false )
				{
					START_LOG( cerr, L"not take host uid.!" )
						<< END_LOG;
				}
				BreakPersonalShop( ( int )NetError::NOT_LEAVE_ROOM_REASON_17, iHostUID );
			}
		}
		break;

	case KPersonalShopFSM::S_SHOP:
		break;

	default:
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( GetStateID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		break;
	}
}

bool KPersonalShop::IsEmpty()
{
	if( m_kPersonalShopUserManager.GetNumMember() < 0 )
	{
		START_LOG( cerr, L"���� ���� �ο� ���� �̻���." )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
			<< END_LOG;
	}

	return ( m_kPersonalShopUserManager.GetNumMember() <= 0 );
}

bool KPersonalShop::IsFull()
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

void KPersonalShop::BreakPersonalShop( int iReason, UidType iHostUID )
{
	KERM_BREAK_PERSONAL_SHOP_NOT kPacketNot;
	kPacketNot.m_iReason = iReason;
	kPacketNot.m_iHostUID = iHostUID;
	BroadCast( ERM_BREAK_PERSONAL_SHOP_NOT, kPacketNot );

	StateTransition( KPersonalShopFSM::I_TO_CLOSE );

#ifdef DEF_TRADE_BOARD
	SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, m_vecSellItemInfo );
#endif DEF_TRADE_BOARD
}

void KPersonalShop::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();  // 040428. ��Ŷ ó�� �ҿ�ð��� ���Ѵ�.

	switch( spEvent_->m_usEventID )
	{
   _CASE( ERM_CHECK_INVALID_USER_NOT, UidType );
	CASE( ERM_OPEN_PERSONAL_SHOP_REQ );
	CASE( ERM_REG_PERSONAL_SHOP_ITEM_REQ );
	CASE( ERM_JOIN_PERSONAL_SHOP_REQ );
   _CASE( ERM_BREAK_PERSONAL_SHOP_REQ, KEGS_BREAK_PERSONAL_SHOP_REQ );
    CASE( ERM_LEAVE_PERSONAL_SHOP_REQ );
	CASE( ERM_BUY_PERSONAL_SHOP_ITEM_REQ );
	CASE( ERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_ACK );
	//{{ 2012. 05. 29	��μ�       ���� ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	CASE( DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_ACK );
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	//}}
	default:
		//{{ 2011. 04. 06	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
		ProcessNativeEvent( spEvent_ );
#else
		START_LOG( cerr, L"�ڵ鷯�� ���ǵ��� �ʾ���." )
			<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
			<< END_LOG;
#endif SERV_PSHOP_AGENCY
		//}}		
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

void KPersonalShop::BroadCastID( u_short usEventID )
{
	BroadCast( usEventID, char() );
}

void KPersonalShop::_BroadCast( const KEvent& kEvent_ )
{
	std::map< UidType, std::set< UidType > > mapUserList;
	m_kPersonalShopUserManager.GetUserList( mapUserList );

	// GameServer�� ���� loop
	std::map< UidType, std::set< UidType > >::iterator mit;
	for( mit = mapUserList.begin(); mit != mapUserList.end(); mit++ )
	{
		KEventPtr spEvent( kEvent_.Clone() );
		spEvent->m_anTrace[0] = mit->first;
		spEvent->m_anTrace[1] = -1;

		// User ������ ���� loop
		std::set< UidType >::iterator sit;
		for( sit = mit->second.begin(); sit != mit->second.end(); sit++ )
		{
			LIF( spEvent->m_kDestPerformer.AddUID( *sit ) );
		}

		KncSend( PI_CN_ROOM, GetUID(), spEvent );
	}
}

#ifdef DEF_TRADE_BOARD
void KPersonalShop::SendPShopListInfo( int _iCode, std::vector<KSellPersonalShopItemInfo> & _veckInfo )
{
	KERM_PERSONAL_SHOP_INFO_NOT kNot;
	kNot.m_iPSUID = GetUID();
	kNot.m_iCode = _iCode;
	kNot.m_vecItemInfo = _veckInfo;

	//������ �߰� �ϰܿ츸 �Ǹ��� �г����� �־���.
	if( _iCode == NetError::ERR_PERSONAL_SHOP_26 )
		m_kPersonalShopUserManager.GetHostNickName( kNot.m_wstrSellerNickName );

	//{{ 2010. 05. 11  ������	�������� ������ ����
#ifdef SERV_INTEGRATION
	BroadCastSameGroupGS( ERM_PERSONAL_SHOP_INFO_NOT, kNot );
#else
	BroadCastAllGSServer( ERM_PERSONAL_SHOP_INFO_NOT, kNot );
#endif SERV_INTEGRATION
	//}}

	START_LOG( clog, L"�������� ������ ����.!" )
		<< BUILD_LOG( kNot.m_vecItemInfo.size() )
		<< BUILD_LOG( NetError::GetErrStr(_iCode) )
		;
}
void KPersonalShop::SendPShopListInfo( int _iCode, KSellPersonalShopItemInfo & kInfo )
{
	std::vector<KSellPersonalShopItemInfo> veckInfo;
	veckInfo.push_back(kInfo);

	SendPShopListInfo(_iCode, veckInfo);
}
#endif DEF_TRADE_BOARD

//���Ӽ����� �׾�����..ó��
_IMPL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType )
{
	std::vector<UidType>	vecDelUserUID;
	int iCount = m_kPersonalShopUserManager.DeleteUserByGsUID( kPacket_, vecDelUserUID );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_ )
		<< BUILD_LOG( iCount )
		<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() );

	//������Ƽ���� ������ ������ ������ ����.
	if( iCount == 0 )
		return;

	//���� �� ���ְ�
	std::vector< UidType >::iterator vit;
	for( vit = vecDelUserUID.begin(); vit != vecDelUserUID.end(); vit++ )
	{
		if( m_kPersonalShopUserManager.IsHost(*vit) == true )
		{
			BreakPersonalShop( ( int )NetError::NOT_LEAVE_ROOM_REASON_09, *vit );
			return;
		}

		m_kPersonalShopUserManager.LeavePersonalShop( *vit );
	}
}

IMPL_ON_FUNC( ERM_OPEN_PERSONAL_SHOP_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_INIT ), ERM_OPEN_PERSONAL_SHOP_ACK, KERM_OPEN_PERSONAL_SHOP_ACK );

	//{{ 2010. 03. 14  ������	�� �й� ��� ����
#ifdef SERV_ROOM_COUNT
	SendRoomCountInfo( CXSLRoom::RT_PERSONAL_SHOP, true );
#endif SERV_ROOM_COUNT
	//}}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//////////////////////////////////////////////////////////////////////////
	// ���� �������� ���Ϳ� �ŷ����� �����Ѱ��� Ȯ��!
	if( KBaseServer::GetKObj()->GetServerGroupID() != KncUid::ExtractServerGroupID( FIRST_SENDER_UID ) )
	{
		START_LOG( cerr2, L"�ٸ� �������� ������ ���� ���λ������� �����Ͽ����ϴ�! �̷��� �ŷ��α� �̻��ϰ� ���ƿ�!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( KBaseServer::GetKObj()->GetServerGroupID() )
			<< BUILD_LOG( KncUid::ExtractServerGroupID( FIRST_SENDER_UID ) )
			<< END_LOG;
		// �α׸� ����!
	}
	//////////////////////////////////////////////////////////////////////////	
#endif DEPRECATED_SERVER_GROUP_MASK

	// ��ǰ����� ��ٸ��� ���� Ÿ�̸� ����.(�ʱ� 5��)
	m_kTimer[TM_WAIT_OTHER].restart();

	//���� �������̶� wait�� ����..
	// �� ���¸� wait �� �ٲٰ� ��ǰ����� �ö����� ��ٸ���.
	StateTransition( KPersonalShopFSM::I_TO_WAIT );

	//////////////////////////////////////////////////////////////////////////

	// �����̹Ƿ� ���� ������ �ʱ�ȭ �Ѵ�.
	m_kPersonalShopUserManager.Reset();

	KPersonalShopUserInfo kInfo;

	// ������ �����Ҽ��ִ� ����� �ִ��� �˻��Ѵ�.
	if( kPacket_.m_iED < PS_OPENING_ED )
	{
		START_LOG( clog, L"���� ��������� ������ ������ ��������.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iED )
			<< BUILD_LOG( PS_OPENING_ED );

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_10;

		StateTransition( KPersonalShopFSM::I_TO_CLOSE );

		goto end_proc;
	}

	// ���� ��û�ڸ� �ִ´�.
	kInfo.m_iUserUID = kPacket_.m_iUserUID;
	kInfo.m_wstrNickName = kPacket_.m_wstrNickName;
	kInfo.m_iUnitUID = FIRST_SENDER_UID;
	kInfo.m_iGSUID = LAST_SENDER_UID;
#ifdef DEF_TRADE_BOARD
	kInfo.m_bIsSquare = true;
#endif DEF_TRADE_BOARD

	if( !m_kPersonalShopUserManager.EnterPersonalShop( kInfo ) )
	{
		START_LOG( cerr, L"���λ��� ���� �߰� ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_00;
		
		StateTransition( KPersonalShopFSM::I_TO_CLOSE );
		
		goto end_proc;
	}

	// ack�� ������.
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iPersonalShopUID = GetUID();

	kPacket.m_cPersonalShopType = kPacket_.m_cPersonalShopType;
#ifdef SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = static_cast<SEnum::AGENCY_SHOP_TYPE>(kPacket_.m_cPersonalShopType);
#else //SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = static_cast<CXSLSquareUnit::PERSONAL_SHOP_TYPE>(kPacket_.m_cPersonalShopType);
#endif //SERV_UPGRADE_TRADE_SYSTEM

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_PERSONAL_SHOP_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_REG_PERSONAL_SHOP_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_WAIT ), ERM_REG_PERSONAL_SHOP_ITEM_ACK, KERM_REG_PERSONAL_SHOP_ITEM_ACK );

	std::vector< KSellPersonalShopItemInfo >::const_iterator vit;

	//���� �����̸� ����.
	m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;

	kPacket.m_iOK = NetError::NET_OK;

	//�����ڰ� �ƴҰ��..
	if( m_kPersonalShopUserManager.IsHost(FIRST_SENDER_UID) == false )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_08;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_REG_PERSONAL_SHOP_ITEM_ACK, kPacket );

		UidType iHostUID = 0;
		m_kPersonalShopUserManager.GetHostUID( iHostUID );
		START_LOG( cerr, L"���λ��� �����ڰ� �ƴѵ� �ǸŹ�ǰ�� ��� �Ϸ�����.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( iHostUID )
			<< END_LOG;

		goto end_proc;
	}

	//{{ 2008. 9. 11  ������	���λ��� ����ó��
	for( vit = kPacket_.m_vecSellItemInfo.begin(); vit != kPacket_.m_vecSellItemInfo.end(); ++vit )
	{
		__int64 iPricePerOne  = static_cast<__int64>(vit->m_iPricePerOne);
		__int64 iItemQuantity = static_cast<__int64>(vit->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity);
		__int64 iCheckTotalPrice = iPricePerOne * iItemQuantity;

		if( iCheckTotalPrice < 0  ||  iCheckTotalPrice > 2000000000 )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_23;

			START_LOG( cout, L"�Ǹ� �׼����� �Ѱ�ġ�� �Ѿ��. ���������� �ǽɵ�.." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( iPricePerOne )
				<< BUILD_LOG( iItemQuantity )
				<< BUILD_LOG( iCheckTotalPrice );

			goto end_proc;
		}
	}
	//}}

	//��ǰ�� ��ϵǾ� �ִµ� ���� wait�̰� �����Ŷ�� �ٽÿ��� �����޼����� ������ shop ���·� �ٲپ��ش�.
	if( m_vecSellItemInfo.size() > 0 )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_07;

		START_LOG( cerr, L"���λ��� ��ǰ�� ��ϵǾ��ִµ� �ٽ� �����Ŷ�� ���ƿ�.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_vecSellItemInfo.size() )
			<< BUILD_LOG( kPacket_.m_vecSellItemInfo.size() )
			<< END_LOG;

		StateTransition( KPersonalShopFSM::I_TO_SHOP );

		goto end_proc;
	}

	//����� �ӽ÷� 3�������� �����ϰ� ���� ����ȭ �𵨿� ���� �ǸŰ����� �޶�����.
	int iSellItemNum = 3;

#ifdef SERV_UPGRADE_TRADE_SYSTEM
	if( m_PersonalShopType == SEnum::AST_PREMIUM )
#else //SERV_UPGRADE_TRADE_SYSTEM
	if( m_PersonalShopType == CXSLSquareUnit::PST_PREMIUM )
#endif //SERV_UPGRADE_TRADE_SYSTEM
		iSellItemNum = 9;

	if( static_cast<int>(kPacket_.m_vecSellItemInfo.size()) > iSellItemNum )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_09;

		goto end_proc;
	}

	kPacket.m_iOpeningED = PS_OPENING_ED;	//������� ������ �ӽ÷� �������..
	kPacket.m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
	kPacket.m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;

	//����üũ ������ �������� ��ǰ�� ��� ��Ų��.
	m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;


	//��ǰ����� �������� �Ǹ� ���·� �ٲ۴�.
	StateTransition( KPersonalShopFSM::I_TO_SHOP );

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_REG_PERSONAL_SHOP_ITEM_ACK, kPacket );

#ifdef DEF_TRADE_BOARD
	if( kPacket.m_iOK == NetError::NET_OK )
	{
		//������ ����� �Ǿ����� ����Ʈ�� �߰�.
		SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_26, m_vecSellItemInfo );
	}
#endif DEF_TRADE_BOARD
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
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_32;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_PERSONAL_SHOP_ACK, kPacket );
		}
		return;
	}
#else
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_SHOP ), ERM_JOIN_PERSONAL_SHOP_ACK, KERM_JOIN_PERSONAL_SHOP_ACK );
#endif

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( m_kPersonalShopUserManager.IsHostUserSameServerGroup( kPacket_.m_iUnitUID ) == false )
	{
		START_LOG( cerr, L"���λ������� �ش� �������� ������ ���� �� �ֽ��ϴ�." )
			<< BUILD_LOG( GetKCnServer()->GetServerGroupID() )
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

	if( !m_kPersonalShopUserManager.EnterPersonalShop( kInfo ) )
	{
		START_LOG( cwarn, L"���λ��� ���� �߰� ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
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
	kPacket.m_bIsPShopAgency = false;
#endif SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	//}}

	if( m_kPersonalShopUserManager.GetHostNickName( kPacket.m_wstrNickName ) == false )
	{
		START_LOG( cerr, L"ȣ��Ʈ �г��� ��� ����.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}

	kPacket.m_vecSellItemInfo = m_vecSellItemInfo;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_PERSONAL_SHOP_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_BREAK_PERSONAL_SHOP_REQ, KEGS_BREAK_PERSONAL_SHOP_REQ )
{
	VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_WAIT, KPersonalShopFSM::S_SHOP ), ERM_BREAK_PERSONAL_SHOP_ACK, KEGS_BREAK_PERSONAL_SHOP_ACK );

	bool bIsHost = m_kPersonalShopUserManager.IsHost( FIRST_SENDER_UID );

	if( !m_kPersonalShopUserManager.LeavePersonalShop( FIRST_SENDER_UID ) )
	{
		START_LOG( cerr, L"���λ��� ������ ����." )
			<< END_LOG;

		if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_01 )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_12;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BREAK_PERSONAL_SHOP_ACK, kPacket );
		}
	}
	else
	{
		if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_01 )
		{
			kPacket.m_iOK = NetError::NET_OK;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BREAK_PERSONAL_SHOP_ACK, kPacket );
		}
	}

	//ȣ��Ʈ�� �������� �����Ƿ� ������ �����Ų��.
	if( bIsHost == true )
		BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_21, FIRST_SENDER_UID );
}

IMPL_ON_FUNC( ERM_LEAVE_PERSONAL_SHOP_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_SHOP ), ERM_LEAVE_PERSONAL_SHOP_ACK, KEGS_LEAVE_PERSONAL_SHOP_ACK );

	kPacket.m_iOK = NetError::NET_OK;

	if( m_kPersonalShopUserManager.IsHost( FIRST_SENDER_UID ) == true )
	{
		START_LOG( cerr, L"�����ڰ� LEAVE �� ����.!�̷�.." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_21, FIRST_SENDER_UID );
		return;
	}

	if( !m_kPersonalShopUserManager.LeavePersonalShop( FIRST_SENDER_UID ) )
	{
		START_LOG( cerr, L"���λ��� ������ ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_01 )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_12;
		}
	}

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_PERSONAL_SHOP_ACK, kPacket );

	//���� ����� ���ٸ� ���� �ݴ´�.
	if( m_kPersonalShopUserManager.GetNumMember() <= 0 )
	{
		StateTransition( KPersonalShopFSM::I_TO_CLOSE );

		START_LOG( cerr, L"���λ��� leave ó���ε� ����� ����.!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_wstrPersonalShopName )
			<< BUILD_LOG( m_vecSellItemInfo.size() )
			<< END_LOG;

#ifdef DEF_TRADE_BOARD
		SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, m_vecSellItemInfo );
#endif DEF_TRADE_BOARD
	}
}

IMPL_ON_FUNC( ERM_BUY_PERSONAL_SHOP_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_SHOP ), ERM_BUY_PERSONAL_SHOP_ITEM_ACK, KEGS_BUY_PERSONAL_SHOP_ITEM_ACK );

	kPacket.m_iOK = NetError::ERR_UNKNOWN;

	// ����� �������� ã�´�.
	u_int uiSellItemIndex = 0;
	for( ; uiSellItemIndex < m_vecSellItemInfo.size(); ++uiSellItemIndex )
	{
		if( m_vecSellItemInfo[uiSellItemIndex].m_kInventoryItemInfo.m_iItemUID == kPacket_.m_iBuyItemUID )
			break;
	}

	int iTotalPrice = 0;

	// �������� �ִٸ�..
	if( uiSellItemIndex < m_vecSellItemInfo.size() )
	{
		// 1. �ŷ� ������ ī�װ��� ����������?
		if( !CXSLInventory::IsTradePossibleCategory( m_vecSellItemInfo[uiSellItemIndex].m_kInventoryItemInfo.m_cSlotCategory ) )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_17;

			START_LOG( cerr, L"�Ǹ����� �������� �ŷ������� ī�װ��� �ƴϴ�." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOGc( m_vecSellItemInfo[uiSellItemIndex].m_kInventoryItemInfo.m_cSlotCategory )
				<< END_LOG;

			goto end_proc;
		}		

		// 1. ���� �˻�..
		if( m_vecSellItemInfo[uiSellItemIndex].m_kInventoryItemInfo.m_kItemInfo.m_iQuantity < kPacket_.m_iBuyQuantity )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_14;

			START_LOG( cerr, L"�ǸŹ�ǰ�� ������." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( m_vecSellItemInfo[uiSellItemIndex].m_kInventoryItemInfo.m_kItemInfo.m_iQuantity )
				<< BUILD_LOG( kPacket_.m_iBuyQuantity )
				<< END_LOG;

			goto end_proc;
		}

		// 2. ���� �׼��� �Ѱ�ġ �˻�
		__int64 iPricePerOne  = static_cast<__int64>(m_vecSellItemInfo[uiSellItemIndex].TotalPricePerOne());
		__int64 iItemQuantity = static_cast<__int64>(kPacket_.m_iBuyQuantity);
		__int64 iCheckTotalPrice = iPricePerOne * iItemQuantity;

		if( iCheckTotalPrice < 0  ||  iCheckTotalPrice > 2000000000 )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_24;

			START_LOG( cout, L"���� �׼����� �Ѱ�ġ�� �Ѿ��. ���������� �ǽɵ�.." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( iPricePerOne )
				<< BUILD_LOG( iItemQuantity )
				<< BUILD_LOG( iCheckTotalPrice );

			goto end_proc;
		}

		// 3. ���� �˻�..
		iTotalPrice = (m_vecSellItemInfo[uiSellItemIndex].TotalPricePerOne() * kPacket_.m_iBuyQuantity);
#ifdef DEF_TRADE_BOARD
		if( false == m_kPersonalShopUserManager.IsSquare(FIRST_SENDER_UID) )
		{
			// �ŷ��Խ����� ���ؼ� ������ �����Ḧ �� ���δ�.
			iTotalPrice += static_cast<int>(iTotalPrice * TD_COMMISSION);
		}
#endif DEF_TRADE_BOARD

		// 4. �����ڰ� ��ǰ�� ED�� �ִ��� �˻�.
		if( iTotalPrice > kPacket_.m_iBuyUnitNowED )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_16;

			START_LOG( clog, L"ED�� �����Ͽ� ��ǰ�� �����Ҽ� ����.!" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( iTotalPrice )
				<< BUILD_LOG( kPacket_.m_iBuyUnitNowED )
				;

			goto end_proc;
		}

		// 5. ���ż������� ED���� ���Ͽ� �ӽ� ����
		kPacket_.m_iEDTemp = kPacket_.m_iBuyUnitNowED - iTotalPrice; //( m_vecSellItemInfo[i].TotalPricePerOne() * kPacket_.m_iBuyQuantity ) );

		// 6. �ѹ��� ���Ű����� �������� �ִ� 1������
		if( kPacket_.m_vecItemInfoTemp.size() > 1 )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_05;

			START_LOG( cerr, L"�ѹ��� ���Ű����� ������ ������ ���������ε�? �Ͼ�� ���� ����!" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_vecItemInfoTemp.size() )
				<< END_LOG;

			goto end_proc;
		}
		else if( kPacket_.m_vecItemInfoTemp.size() == 1 )
		{
			//{{ 2009. 2. 12  ������	���λ��� ED���� ����
			std::vector< KItemInfo >::iterator vit = kPacket_.m_vecItemInfoTemp.begin();
			for( ; vit != kPacket_.m_vecItemInfoTemp.end(); ++vit )
			{
				if( vit->m_iItemID == m_vecSellItemInfo[uiSellItemIndex].m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
				{
					// GSUser�Ҹ�� ���� ������Ʈ������� DB�� �� ������ ���� ����
					*vit = m_vecSellItemInfo[uiSellItemIndex].m_kInventoryItemInfo.m_kItemInfo;

					// ������ ����
					vit->m_iQuantity = kPacket_.m_iBuyQuantity;
					break;
				}
			}

			if( vit == kPacket_.m_vecItemInfoTemp.end() )
			{
				kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_05;

				START_LOG( cerr, L"������ �������� ����." )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( kPacket_.m_iBuyQuantity )
					<< BUILD_LOG( kPacket_.m_iBuyItemUID )
					<< END_LOG;

				goto end_proc;
			}
			//}}
		}
	}
	// ���ٸ�.
	else
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_15;

		START_LOG( cerr, L"�ǸŹ�ǰ ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iBuyQuantity )
			<< BUILD_LOG( kPacket_.m_iBuyItemUID )
			<< END_LOG;

		goto end_proc;
	}

	//{{ 2009. 2. 11  ������	���λ��� ED���� ����
	if( !m_kPersonalShopUserManager.SetUpdateTemp( FIRST_SENDER_UID, kPacket_.m_kItemQuantityUpdateTemp, kPacket_.m_vecItemInfoTemp, kPacket_.m_iEDTemp ) )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_05;

		START_LOG( cerr, L"������ �������� ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iBuyQuantity )
			<< BUILD_LOG( kPacket_.m_iBuyItemUID )
			<< END_LOG;

		goto end_proc;
	}
	//}}

	//��� ���Ż��� üũ�� �������� �Ǹ��� gsuser�� ���� �ǸŹ�ǰ�� �ִ��� üũ�ϰ� �´�.
	UidType iHostUID = 0;
	UidType iHostGSUID = 0;

	if( m_kPersonalShopUserManager.GetHostUID( iHostUID ) == true )
	{
		if( m_kPersonalShopUserManager.GetPersonalShopUserGS( iHostUID, iHostGSUID ) == true )
		{
			KERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_REQ kReq;
			kReq.m_kSellItemInfo = m_vecSellItemInfo[uiSellItemIndex].m_kInventoryItemInfo;
			kReq.m_iBuyUnitUID	 = FIRST_SENDER_UID;
			kReq.m_iBuyQuantity	 = kPacket_.m_iBuyQuantity;
			kReq.m_iTotalPrice	 = iTotalPrice;

            SendToGSCharacter( iHostGSUID, iHostUID, ERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_REQ, kReq );
			return;
		}
		else
		{
			START_LOG( cerr, L"���λ��� �Ǹ��� GSUID ��� ����.!" )
				<< BUILD_LOG( iHostUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_20;

			goto end_proc;
		}
	}
	else
	{
		START_LOG( cerr, L"���λ��� �Ǹ��� UID ��� ����.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_wstrPersonalShopName )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_20;

		goto end_proc;
	}

	return;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );
}

//{{ 2012. 05. 29	��μ�       ���� ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
IMPL_ON_FUNC( ERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_ACK )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_SHOP ), ERM_BUY_PERSONAL_SHOP_ITEM_ACK, KEGS_BUY_PERSONAL_SHOP_ITEM_ACK );

	std::vector< KSellPersonalShopItemInfo >::iterator vitItem;

	UidType iBuyUnitGSUID = 0;
	UidType iBuyUnitUID = kPacket_.m_iBuyUnitUID;
	if( m_kPersonalShopUserManager.GetPersonalShopUserGS( iBuyUnitUID, iBuyUnitGSUID ) == false )
	{
		START_LOG( cerr, L"������ ���� UID ã�� ����.!" )
			<< BUILD_LOG( iBuyUnitUID )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetMaxMember() )
			<< END_LOG;

		return;
	}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"���λ��� ���Ž���.!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( iBuyUnitUID )
			<< END_LOG
			;

		kPacket.m_iOK = kPacket_.m_iOK;

		goto end_proc;
	}

	UidType iHostUID = 0;
	UidType iHostGSUID = 0;
	if( m_kPersonalShopUserManager.GetHostUID( iHostUID ) == true )
	{
		if( m_kPersonalShopUserManager.GetPersonalShopUserGS( iHostUID, iHostGSUID ) == false )
		{
			START_LOG( cerr, L"���λ��� �Ǹ��� GSUID ��� ����.!" )
				<< BUILD_LOG( iHostUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_20;

			goto end_proc;
		}
	}
	else
	{
		START_LOG( cerr, L"���λ��� �Ǹ��� UID ��� ����.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_wstrPersonalShopName )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_20;

		goto end_proc;
	}

	//�ȷ��� �������� ã�´�.
	for( vitItem = m_vecSellItemInfo.begin(); vitItem != m_vecSellItemInfo.end(); ++vitItem )
	{
		if( vitItem->m_kInventoryItemInfo.m_iItemUID == kPacket_.m_iBuyItemUID )
			break;
	}

	// �������� �ִٸ�..
	if( vitItem != m_vecSellItemInfo.end() )
	{
		//���� �˻�..
		if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity < kPacket_.m_iBuyQuantity )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_14;

			START_LOG( cwarn, L"�ǸŹ�ǰ�� ������." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity )
				<< BUILD_LOG( kPacket_.m_iBuyQuantity )
				<< END_LOG;

			goto end_proc;
		}

		//�Ǹ��� & ������ �������� ������Ʈ ���� �����ֱ�.
		if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_cUsageType != CXSLItem::PT_QUANTITY )
		{
			if( kPacket_.m_iBuyQuantity > 1 )
			{
				kPacket_.m_iBuyQuantity = 1;

				START_LOG( cerr, L"������ �������� �ƴѵ� ��� ������ 1�̻���." )
					<< BUILD_LOG( m_wstrPersonalShopName )
					<< BUILD_LOG( iHostUID )
					<< BUILD_LOG( iBuyUnitUID )
					<< BUILD_LOGc( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_cUsageType )
					<< BUILD_LOG( kPacket_.m_iBuyQuantity )
					<< END_LOG;
			}
		}

		KItemQuantityUpdate kBuyUserUpdateTemp;
		std::vector< KItemInfo > vecBuyUserUpdateTemp;
		int iBuyUserEDTemp = 0;

		if( !m_kPersonalShopUserManager.GetUpdateTemp( iBuyUnitUID, kBuyUserUpdateTemp, vecBuyUserUpdateTemp, iBuyUserEDTemp ) )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_25;

			START_LOG( cerr, L"������ UID ã�� ����.!" )
				<< BUILD_LOG( iBuyUnitUID )
				<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
				<< BUILD_LOG( m_kPersonalShopUserManager.GetMaxMember() )
				<< END_LOG;

			goto end_proc;
		}

		//////////////////////////////////////////////////////////////////////////
		// DB �� ���� �ؾ� �� ������ ����
		KDBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_REQ kCompleteReq;
		kCompleteReq.m_iSellUnitUID = iHostUID;
		kCompleteReq.m_iSellUnitGSUID = iHostGSUID;
		kCompleteReq.m_iSellUnitEDIN = vitItem->m_iPricePerOne * kPacket_.m_iBuyQuantity;
		kCompleteReq.m_kSellItemInfo = vitItem->m_kInventoryItemInfo;
		kCompleteReq.m_kSellItemInfo.m_kItemInfo.m_iQuantity = kPacket_.m_iBuyQuantity;
		//{{ 2010. 03. 31  ������	�ŷ��Խ���
#ifdef DEF_TRADE_BOARD
		kCompleteReq.m_iBuyCommissionED = 0; // �ʱ�ȭ
		kCompleteReq.m_bBuyUnitIsSquare = m_kPersonalShopUserManager.IsSquare( iBuyUnitUID ); // �������� ������ġ ���
#endif DEF_TRADE_BOARD
		//}}

		//{{ 2009. 3. 24  ������	������
#ifdef SERV_UPGRADE_TRADE_SYSTEM
		if( m_PersonalShopType != SEnum::AST_PREMIUM )
#else //SERV_UPGRADE_TRADE_SYSTEM
		if( m_PersonalShopType != CXSLSquareUnit::PST_PREMIUM )
#endif //SERV_UPGRADE_TRADE_SYSTEM
		{
			// [����] �����̾� ������ �ƴ϶�� �ǸŰ��� 10%�� ������� ���� ����
			kCompleteReq.m_iSellCommissionED = static_cast<int>(kCompleteReq.m_iSellUnitEDIN * TD_HOST_COMMISSION);
			kCompleteReq.m_iSellUnitEDIN -= kCompleteReq.m_iSellCommissionED;
		}
		//}}

		kCompleteReq.m_iBuyUnitUID = iBuyUnitUID;
		kCompleteReq.m_iBuyUnitGSUID = iBuyUnitGSUID;
		kCompleteReq.m_iBuyUnitEDOUT = vitItem->TotalPricePerOne() * kPacket_.m_iBuyQuantity;
		kCompleteReq.m_kBuyItemInfo = vitItem->m_kInventoryItemInfo;
		kCompleteReq.m_kBuyItemInfo.m_kItemInfo.m_iQuantity = kPacket_.m_iBuyQuantity;
#ifdef DEF_TRADE_BOARD
		// �ŷ��Խ��� ������
		if( false == m_kPersonalShopUserManager.IsSquare(iBuyUnitUID) )
		{
			kCompleteReq.m_iBuyCommissionED = static_cast<int>(kCompleteReq.m_iBuyUnitEDOUT * TD_COMMISSION);
			kCompleteReq.m_iBuyUnitEDOUT += kCompleteReq.m_iBuyCommissionED;
		}
#endif DEF_TRADE_BOARD

		kCompleteReq.m_iBuyUnitEDOUT = (-1) * kCompleteReq.m_iBuyUnitEDOUT; // ��ȭ���� �ǹǷ� ������ ED -> ���̳ʽ��� ó��

		//kCompleteReq.m_kSellItemQuantityUpdateTemp = kPacket_.m_kItemQuantityUpdateTemp;
		//kCompleteReq.m_vecSellItemInfoTemp = kPacket_.m_vecItemInfoTemp;
		
		//kCompleteReq.m_kBuyItemQuantityUpdateTemp = kBuyUserUpdateTemp;
		//kCompleteReq.m_vecBuyItemInfoTemp = vecBuyUserUpdateTemp;
		
		// DB �� ����
        SendToGameDB( DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_REQ, kCompleteReq );

		// Ŭ���̾�Ʈ�� OK�� ���� - �����ŷ� ������ ���� ������Ʈ �Ǿ�� ���׸� ������ �� �ִ�!
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( iBuyUnitGSUID, iBuyUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );

		//�Ǹž����� ����ó�� & �������� �����鿡�� �ǸŻ�ǰ ���� ������Ʈ ó��.
		vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity -= kPacket_.m_iBuyQuantity;

		if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity <= 0 )
		{
			vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity = 0;
		}
	}
	// ���ٸ�.
	else
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_15;

		START_LOG( cwarn, L"�ǸŹ�ǰ ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			//<< BUILD_LOG( m_kPersonalShopUserManager.GetHostUID() )			
			<< BUILD_LOG( kPacket_.m_iBuyItemUID )
			<< BUILD_LOG( kPacket_.m_iBuyQuantity )
			<< END_LOG;

		goto end_proc;
	}

end_proc:
	SendToGSCharacter( iBuyUnitGSUID, iBuyUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_ACK )
{
	// ���� ������ ������ ���� ����� �Ѵ�.
	std::vector< KSellPersonalShopItemInfo >::iterator vitItem;
	//�ȷ��� �������� ã�´�.
	for( vitItem = m_vecSellItemInfo.begin(); vitItem != m_vecSellItemInfo.end(); ++vitItem )
	{
		if( vitItem->m_kInventoryItemInfo.m_iItemUID == kPacket_.m_kResult.m_kSellItemInfo.m_iItemUID )
			break;
	}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"���λ��� ���Ž���.!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( kPacket_.m_kResult.m_iBuyUnitUID )
			<< BUILD_LOG( kPacket_.m_kResult.m_iSellUnitUID )
			<< END_LOG;

		KEGS_BUY_PERSONAL_SHOP_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_25;
		SendToGSCharacter( kPacket_.m_kResult.m_iSellUnitGSUID, kPacket_.m_kResult.m_iSellUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );

		// �������� �ִٸ�..
		if( vitItem != m_vecSellItemInfo.end() )
		{
			vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity += kPacket_.m_kResult.m_kSellItemInfo.m_kItemInfo.m_iQuantity;
		}
		return;
	}

	// �������� �ִٸ�..
	if( vitItem != m_vecSellItemInfo.end() )
	{
		KEGS_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT kUpdateNot;
		kUpdateNot.m_iItemUID = vitItem->m_kInventoryItemInfo.m_iItemUID;
		kUpdateNot.m_iQuantity = vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity;
		BroadCast( ERM_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT, kUpdateNot );

		//{{ 2008. 2. 26  ������  ���λ��� �α�
		int iItemID = vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iItemID;
		//}}		

		//�Ǹ��� �����ۼ����� ������ �������ش�.
		if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_QUANTITY )
		{
			if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity == 0 )
			{
#ifdef DEF_TRADE_BOARD
				SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, *vitItem );
#endif DEF_TRADE_BOARD

				m_vecSellItemInfo.erase( vitItem );
			}

#ifdef DEF_TRADE_BOARD
			SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_27, *vitItem );
#endif DEF_TRADE_BOARD
		}
		else
		{
#ifdef DEF_TRADE_BOARD
			SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, *vitItem );
#endif DEF_TRADE_BOARD

			m_vecSellItemInfo.erase( vitItem );
		}

		//��� ��ǰ�� �ȷ������ ���λ��� �ݾ��ֱ�.
		if( m_vecSellItemInfo.empty() == true )
		{
			BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_19, kPacket_.m_kResult.m_iSellUnitUID );
		}

		//{{ 2008. 2. 26  ������  ���λ��� �α�
		KELOG_INSERT_PERSONAL_SHOP_LOG_NOT kPacketLog;
		//{{ 2011. 05. 11	������	�븮����
#ifdef SERV_PSHOP_AGENCY
		kPacketLog.m_cPShopType	  = SEnum::PST_PERSONAL_SHOP;
#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2010. 03. 31  ������	�ŷ��Խ���
		kPacketLog.m_bIsSquare	  = kPacket_.m_kResult.m_bBuyUnitIsSquare;
		//}}
		kPacketLog.m_iFromUnitUID = kPacket_.m_kResult.m_iSellUnitUID;
		kPacketLog.m_iToUnitUID	  = kPacket_.m_kResult.m_iBuyUnitUID;
		kPacketLog.m_iItemUID	  = kUpdateNot.m_iItemUID;
		kPacketLog.m_iItemID	  = iItemID;
		kPacketLog.m_iQuantity	  = kPacket_.m_kResult.m_kSellItemInfo.m_kItemInfo.m_iQuantity;
		kPacketLog.m_iED		  = kPacket_.m_kResult.m_iSellUnitEDIN;
		SendToLogDB( ELOG_INSERT_PERSONAL_SHOP_LOG_NOT, kPacketLog );
		//}}
	}

	// �Ǹ��ڿ��� �̺�Ʈ ����
	SendToGSCharacter( kPacket_.m_kResult.m_iSellUnitGSUID, kPacket_.m_kResult.m_iSellUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, kPacket_.m_kResult );

	// �����ڿ��� �̺�Ʈ ����
	SendToGSCharacter( kPacket_.m_kResult.m_iBuyUnitGSUID, kPacket_.m_kResult.m_iBuyUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, kPacket_.m_kResult );
}

#else
IMPL_ON_FUNC( ERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_ACK )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_SHOP ), ERM_BUY_PERSONAL_SHOP_ITEM_ACK, KEGS_BUY_PERSONAL_SHOP_ITEM_ACK );

	std::vector< KSellPersonalShopItemInfo >::iterator vitItem;

	UidType iBuyUnitGSUID = 0;
	UidType iBuyUnitUID = kPacket_.m_iBuyUnitUID;
	if( m_kPersonalShopUserManager.GetPersonalShopUserGS( iBuyUnitUID, iBuyUnitGSUID ) == false )
	{
		START_LOG( cerr, L"������ ���� UID ã�� ����.!" )
			<< BUILD_LOG( iBuyUnitUID )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetMaxMember() )
			<< END_LOG;

		return;
	}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"���λ��� ���Ž���.!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( iBuyUnitUID )
			<< END_LOG
			;

		kPacket.m_iOK = kPacket_.m_iOK;

		goto end_proc;
	}

	UidType iHostUID = 0;
	UidType iHostGSUID = 0;
	if( m_kPersonalShopUserManager.GetHostUID( iHostUID ) == true )
	{
		if( m_kPersonalShopUserManager.GetPersonalShopUserGS( iHostUID, iHostGSUID ) == false )
		{
			START_LOG( cerr, L"���λ��� �Ǹ��� GSUID ��� ����.!" )
				<< BUILD_LOG( iHostUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_20;

			goto end_proc;
		}
	}
	else
	{
		START_LOG( cerr, L"���λ��� �Ǹ��� UID ��� ����.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_wstrPersonalShopName )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_20;

		goto end_proc;
	}

	//�ȷ��� �������� ã�´�.
	for( vitItem = m_vecSellItemInfo.begin(); vitItem != m_vecSellItemInfo.end(); ++vitItem )
	{
		if( vitItem->m_kInventoryItemInfo.m_iItemUID == kPacket_.m_iBuyItemUID )
			break;
	}

	// �������� �ִٸ�..
	if( vitItem != m_vecSellItemInfo.end() )
	{
		//���� �˻�..
		if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity < kPacket_.m_iBuyQuantity )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_14;

			START_LOG( cwarn, L"�ǸŹ�ǰ�� ������." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity )
				<< BUILD_LOG( kPacket_.m_iBuyQuantity )
				<< END_LOG;

			goto end_proc;
		}

		//�Ǹ��� & ������ �������� ������Ʈ ���� �����ֱ�.
		if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_cUsageType != CXSLItem::PT_QUANTITY )
		{
			if( kPacket_.m_iBuyQuantity > 1 )
			{
				kPacket_.m_iBuyQuantity = 1;

				START_LOG( cerr, L"������ �������� �ƴѵ� ��� ������ 1�̻���." )
					<< BUILD_LOG( m_wstrPersonalShopName )
					<< BUILD_LOG( iHostUID )
					<< BUILD_LOG( iBuyUnitUID )
					<< BUILD_LOGc( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_cUsageType )
					<< BUILD_LOG( kPacket_.m_iBuyQuantity )
					<< END_LOG;
			}
		}

		KItemQuantityUpdate kBuyUserUpdateTemp;
		std::vector< KItemInfo > vecBuyUserUpdateTemp;
		int iBuyUserEDTemp = 0;

		if( !m_kPersonalShopUserManager.GetUpdateTemp( iBuyUnitUID, kBuyUserUpdateTemp, vecBuyUserUpdateTemp, iBuyUserEDTemp ) )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_25;

			START_LOG( cerr, L"������ UID ã�� ����.!" )
				<< BUILD_LOG( iBuyUnitUID )
				<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
				<< BUILD_LOG( m_kPersonalShopUserManager.GetMaxMember() )
				<< END_LOG;

			goto end_proc;
		}

		//#1. �Ǹ��� ��ǰ������ �Ǹűݾ� ����
		KERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT kCompleteNot;
		kCompleteNot.m_iSellUnitUID = iHostUID;
		kCompleteNot.m_iSellUnitEDIN = vitItem->m_iPricePerOne * kPacket_.m_iBuyQuantity;
		kCompleteNot.m_kSellIntemInfoOUT = vitItem->m_kInventoryItemInfo;
		kCompleteNot.m_kSellIntemInfoOUT.m_kItemInfo.m_iQuantity = kPacket_.m_iBuyQuantity;
		kCompleteNot.m_iSellCommissionED = 0; // �ʱ�ȭ
		//{{ 2010. 03. 31  ������	�ŷ��Խ���
#ifdef DEF_TRADE_BOARD
		kCompleteNot.m_iPriceCommissionED = 0; // �ʱ�ȭ
		kCompleteNot.m_bIsSquare = m_kPersonalShopUserManager.IsSquare( iBuyUnitUID ); // �������� ������ġ ���
#endif DEF_TRADE_BOARD
		//}}

		//{{ 2009. 3. 24  ������	������
		if( m_PersonalShopType != SEnum::AST_PREMIUM )
		{
			// [����] �����̾� ������ �ƴ϶�� �ǸŰ��� 10%�� ������� ���� ����
			kCompleteNot.m_iSellCommissionED = static_cast<int>(kCompleteNot.m_iSellUnitEDIN * TD_HOST_COMMISSION);
			kCompleteNot.m_iSellUnitEDIN -= kCompleteNot.m_iSellCommissionED;

			// iEDTemp�� ���а��� �ƴ϶� �ǰ���
			kPacket_.m_iEDTemp -= kCompleteNot.m_iSellCommissionED;
		}
		//}}

		//#2. ������ ��ǰ������ ���űݾ� ����.
		kCompleteNot.m_iPriceUnitUID = iBuyUnitUID;
		kCompleteNot.m_iPriceUnitEDOUT = vitItem->TotalPricePerOne() * kPacket_.m_iBuyQuantity;
#ifdef DEF_TRADE_BOARD
		// �ŷ��Խ��� ������
		if( false == m_kPersonalShopUserManager.IsSquare(iBuyUnitUID) )
		{
			kCompleteNot.m_iPriceCommissionED = static_cast<int>(kCompleteNot.m_iPriceUnitEDOUT * TD_COMMISSION);
			kCompleteNot.m_iPriceUnitEDOUT += kCompleteNot.m_iPriceCommissionED;
		}
#endif DEF_TRADE_BOARD
		kCompleteNot.m_kPriceIntemInfoIN = vitItem->m_kInventoryItemInfo;
		kCompleteNot.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity = kPacket_.m_iBuyQuantity;

		//#3. �Ǹ��ڿ��� �̺�Ʈ ����
		kCompleteNot.m_kItemQuantityUpdateTemp = kPacket_.m_kItemQuantityUpdateTemp;
		kCompleteNot.m_vecItemInfoTemp = kPacket_.m_vecItemInfoTemp;
		kCompleteNot.m_iEDTemp = kPacket_.m_iEDTemp;
		SendToGSCharacter( iHostGSUID, iHostUID, ERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, kCompleteNot );

		//#4. �����ڿ��� �̺�Ʈ ����
		kCompleteNot.m_kItemQuantityUpdateTemp = kBuyUserUpdateTemp;
		kCompleteNot.m_vecItemInfoTemp = vecBuyUserUpdateTemp;
		kCompleteNot.m_iEDTemp = iBuyUserEDTemp;
		SendToGSCharacter( iBuyUnitGSUID, iBuyUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, kCompleteNot );

		//#5. Ŭ���̾�Ʈ�� OK�� ���� - �����ŷ� ������ ���� ������Ʈ �Ǿ�� ���׸� ������ �� �ִ�!
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( iBuyUnitGSUID, iBuyUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );


		//�Ǹž����� ����ó�� & �������� �����鿡�� �ǸŻ�ǰ ���� ������Ʈ ó��.
		vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity -= kPacket_.m_iBuyQuantity;

		if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity <= 0 )
		{
			//LIF( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity < 0 );

			vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity = 0;
		}

		KEGS_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT kUpdateNot;
		kUpdateNot.m_iItemUID = vitItem->m_kInventoryItemInfo.m_iItemUID;
		kUpdateNot.m_iQuantity = vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity;
		BroadCast( ERM_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT, kUpdateNot );

		//{{ 2008. 2. 26  ������  ���λ��� �α�
		int iItemID = vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iItemID;
		//}}		

		//�Ǹ��� �����ۼ����� ������ �������ش�.
		if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_QUANTITY )
		{
			if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity == 0 )
			{
#ifdef DEF_TRADE_BOARD
				SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, *vitItem );
#endif DEF_TRADE_BOARD

				m_vecSellItemInfo.erase( vitItem );
			}

#ifdef DEF_TRADE_BOARD
			SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_27, *vitItem );
#endif DEF_TRADE_BOARD
		}
		else
		{
#ifdef DEF_TRADE_BOARD
			SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, *vitItem );
#endif DEF_TRADE_BOARD

			m_vecSellItemInfo.erase( vitItem );
		}

		//��� ��ǰ�� �ȷ������ ���λ��� �ݾ��ֱ�.
		if( m_vecSellItemInfo.empty() == true )
		{
			BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_19, iHostUID );
		}

		//{{ 2008. 2. 26  ������  ���λ��� �α�
		KELOG_INSERT_PERSONAL_SHOP_LOG_NOT kPacketLog;
		//{{ 2011. 05. 11	������	�븮����
#ifdef SERV_PSHOP_AGENCY
		kPacketLog.m_cPShopType	  = SEnum::PST_PERSONAL_SHOP;
#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2010. 03. 31  ������	�ŷ��Խ���
		kPacketLog.m_bIsSquare	  = kCompleteNot.m_bIsSquare;
		//}}
		kPacketLog.m_iFromUnitUID = iHostUID;
		kPacketLog.m_iToUnitUID	  = iBuyUnitUID;
		kPacketLog.m_iItemUID	  = kUpdateNot.m_iItemUID;
		kPacketLog.m_iItemID	  = iItemID;
		kPacketLog.m_iQuantity	  = kPacket_.m_iBuyQuantity;
		kPacketLog.m_iED		  = kCompleteNot.m_iSellUnitEDIN;
		SendToLogDB( ELOG_INSERT_PERSONAL_SHOP_LOG_NOT, kPacketLog );
		//}}

		return;
	}
	// ���ٸ�.
	else
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_15;

		START_LOG( cwarn, L"�ǸŹ�ǰ ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			//<< BUILD_LOG( m_kPersonalShopUserManager.GetHostUID() )			
			<< BUILD_LOG( kPacket_.m_iBuyItemUID )
			<< BUILD_LOG( kPacket_.m_iBuyQuantity )
			<< END_LOG;

		goto end_proc;
	}


end_proc:
	SendToGSCharacter( iBuyUnitGSUID, iBuyUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );
}
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
//}}





