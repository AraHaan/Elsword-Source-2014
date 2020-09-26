#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"

#include "Enum/Enum.h"
//{{ 2011. 02. 06	������	�ŷ� ���� ���
#ifdef SERV_BLOCK_TRADE
	#include "GameSysVal/GameSysVal.h"
#endif SERV_BLOCK_TRADE
//}}


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp ���� ������ �˴ϴ�!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC( EGS_REQUEST_TRADE_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_REQUEST_TRADE_ACK kAck;

	//{{ 2011. 02. 06	������	�ŷ� ���� ���
#ifdef SERV_BLOCK_TRADE
	if( SiKGameSysVal()->IsBlockPersonalTrade() == true )
	{
		kAck.m_iOK = NetError::ERR_BLOCK_TRADE_00;
		SendPacket( EGS_REQUEST_TRADE_ACK, kAck );
		return;
	}
#endif SERV_BLOCK_TRADE
	//}}

	//{{ 2011. 03. 02	������	���� �ŷ� ����
#ifdef SERV_HACK_USER_TRADE_BLOCK
	if( m_kUserAbuserManager.IsTradeBlockUser() == true )
	{
#ifdef SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kAck.m_iOK = NetError::ERR_BLOCK_TRADE_07;
#else // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kAck.m_iOK = NetError::ERR_TRADE_00;
#endif // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		SendPacket( EGS_REQUEST_TRADE_ACK, kAck );
		return;
	}
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	if( m_kUserAbuserManager.IsTradeBlockUnit() == true )
	{
		kAck.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_02;
		SendPacket( EGS_REQUEST_TRADE_ACK, kAck );
		return;
	}
#endif SERV_NEW_UNIT_TRADE_LIMIT

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		kAck.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_REQUEST_TRADE_ACK, kAck );
		return;
	}

	if( kPacket_.m_iUnitUID == GetCharUID() )
	{
		kAck.m_iOK = NetError::ERR_TRADE_13;
		SendPacket( EGS_REQUEST_TRADE_ACK, kAck );
		return;
	}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 12  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( IsSameServerGroupUID( kPacket_.m_iUnitUID ) == false )
	{
		kAck.m_iOK = NetError::ERR_DIF_SERVER_GROUP_00;
		SendPacket( EGS_REQUEST_TRADE_ACK, kAck );
		return;
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	kAck.m_iOK = NetError::NET_OK;
	kAck.m_iUnitUID = kPacket_.m_iUnitUID;	
	//kAck.m_wstrNickName = spUser->GetCharName();
	SendPacket( EGS_REQUEST_TRADE_ACK, kAck );

	KERM_REQUEST_TRADE_NOT kNot;
	kNot.m_iSenderUID		= GetCharUID();
	kNot.m_wstrSenderNickName = GetCharName();
	kNot.m_iReceiverUID		= kPacket_.m_iUnitUID;

	if( GetStateID() == KGSFSM::S_ROOM )
	{
		SendToCnRoom( ERM_REQUEST_TRADE_NOT, kNot );
	}
	else
	{
		SendToGSField( ERM_REQUEST_TRADE_NOT, kNot );
	}	
}

IMPL_ON_FUNC( ERM_REQUEST_TRADE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2009. 3. 24  ������	GM����
	if( GetAuthLevel() == SEnum::UAL_GM )
	{
		KERM_REQUEST_TRADE_REPLY_NOT kPacketNot;
		kPacketNot.m_iReceiverUID = kPacket_.m_iSenderUID;
		kPacketNot.m_iOK = NetError::ERR_GM_CHAR_DENY_00; // ���ΰŷ� ������ ó��..

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			SendToCnRoom( ERM_REQUEST_TRADE_REPLY_NOT, kPacketNot );
		}
		else
		{
			SendToGSField( ERM_REQUEST_TRADE_REPLY_NOT, kPacketNot );
		}
		return;
	}
	//}}

	//{{ 2011. 03. 02	������	���� �ŷ� ����
#ifdef SERV_HACK_USER_TRADE_BLOCK
	if( m_kUserAbuserManager.IsTradeBlockUser() == true )
	{
		KERM_REQUEST_TRADE_REPLY_NOT kPacketNot;
		kPacketNot.m_iReceiverUID = kPacket_.m_iSenderUID;
		kPacketNot.m_iOK = NetError::NOT_REQUEST_TRADE_REPLY_02; // ���ΰŷ� ������ ó��..

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			SendToCnRoom( ERM_REQUEST_TRADE_REPLY_NOT, kPacketNot );
		}
		else
		{
			SendToGSField( ERM_REQUEST_TRADE_REPLY_NOT, kPacketNot );
		}
		return;
	}
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	if( m_kUserAbuserManager.IsTradeBlockUnit() == true )
	{
		KERM_REQUEST_TRADE_REPLY_NOT kPacketNot;
		kPacketNot.m_iReceiverUID = kPacket_.m_iSenderUID;
		kPacketNot.m_iOK = NetError::NOT_REQUEST_TRADE_REPLY_02; // ���ΰŷ� ������ ó��..

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			SendToCnRoom( ERM_REQUEST_TRADE_REPLY_NOT, kPacketNot );
		}
		else
		{
			SendToGSField( ERM_REQUEST_TRADE_REPLY_NOT, kPacketNot );
		}
		return;
	}
#endif SERV_NEW_UNIT_TRADE_LIMIT

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		KERM_REQUEST_TRADE_REPLY_NOT kPacketNot;
		kPacketNot.m_iReceiverUID = kPacket_.m_iSenderUID;
		kPacketNot.m_iOK = NetError::NOT_REQUEST_TRADE_REPLY_02; // ���ΰŷ� ������ ó��..

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			SendToCnRoom( ERM_REQUEST_TRADE_REPLY_NOT, kPacketNot );
		}
		else
		{
			SendToGSField( ERM_REQUEST_TRADE_REPLY_NOT, kPacketNot );
		}
		return;
	}

	//{{ 2009. 5. 12  ������	���ΰŷ� �ź�
	if( m_kUserGameOption.IsDenyTrade() )
	{
		KERM_REQUEST_TRADE_REPLY_NOT kPacketNot;
		kPacketNot.m_iReceiverUID = kPacket_.m_iSenderUID;
		kPacketNot.m_iOK = NetError::NOT_REQUEST_TRADE_REPLY_02; // ���ΰŷ� ������ ó��..

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			SendToCnRoom( ERM_REQUEST_TRADE_REPLY_NOT, kPacketNot );
		}
		else
		{
			SendToGSField( ERM_REQUEST_TRADE_REPLY_NOT, kPacketNot );
		}
		return;
	}
	//}}

	// �̹� �ŷ� ���� ���.
	if( GetTradeUID() != 0  ||  GetPersonalShopUID() != 0 )
	{
		KERM_REQUEST_TRADE_REPLY_NOT kPacketNot;
		kPacketNot.m_iReceiverUID = kPacket_.m_iSenderUID;
		kPacketNot.m_iOK = NetError::NOT_REQUEST_TRADE_REPLY_01;

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			SendToCnRoom( ERM_REQUEST_TRADE_REPLY_NOT, kPacketNot );
		}
		else
		{
			SendToGSField( ERM_REQUEST_TRADE_REPLY_NOT, kPacketNot );
		}
		return;
	}

	KEGS_REQUEST_TRADE_NOT kPacket;
	kPacket.m_iSenderUID                = kPacket_.m_iSenderUID;
	kPacket.m_wstrSenderNickName        = kPacket_.m_wstrSenderNickName;

	SendPacket( EGS_REQUEST_TRADE_NOT, kPacket );
}

IMPL_ON_FUNC( EGS_REQUEST_TRADE_REPLY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KERM_REQUEST_TRADE_REPLY_NOT kPacket;

	kPacket.m_iReceiverUID	= kPacket_.m_iReceiverUID;

	if( kPacket_.m_bAccept )
	{
		kPacket.m_iOK = NetError::NOT_REQUEST_TRADE_REPLY_00;
	}
	else
	{
		kPacket.m_iOK = NetError::NOT_REQUEST_TRADE_REPLY_02;
	}

	kPacket.m_iMemberUID				= GetCharUID();
	kPacket.m_iMemberGSUID				= KBaseServer::GetKObj()->GetUID();

	if( GetStateID() == KGSFSM::S_ROOM )
	{
		SendToCnRoom( ERM_REQUEST_TRADE_REPLY_NOT, kPacket );
	}
	else
	{
		SendToGSField( ERM_REQUEST_TRADE_REPLY_NOT, kPacket );
	}
}

IMPL_ON_FUNC( ERM_REQUEST_TRADE_REPLY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NOT_REQUEST_TRADE_REPLY_00 )
	{
		KERM_OPEN_TRADE_REQ kPacket;
		kPacket.m_iUserUID = GetUID();
		kPacket.m_wstrSenderNickName = GetCharName();
		kPacket.m_iMemberUID	= kPacket_.m_iMemberUID;
		kPacket.m_iMemberGSUID	= kPacket_.m_iMemberGSUID;
		//{{ 2009. 4. 28  ������	�ִ�ED����
		kPacket.m_iED = GetED();
		//}}
		//{{ 2011. 06. 21	������	�ŷ��� ���� ���� ����
#ifdef SERV_OPEN_TRADE_ROOM_BUG_FIX
		_SendToCnRoom( 0, ERM_OPEN_TRADE_REQ, kPacket );
#else
		SendToCnRoom( ERM_OPEN_TRADE_REQ, kPacket );
#endif SERV_OPEN_TRADE_ROOM_BUG_FIX
		//}}
	}
	else
	{
		KEGS_REQUEST_TRADE_DENY_NOT kPacketNot;
		kPacketNot.m_iOK = kPacket_.m_iOK;
		kPacketNot.m_iUnitUID = kPacket_.m_iMemberUID;
		SendPacket( EGS_REQUEST_TRADE_DENY_NOT, kPacketNot );
	}
}

IMPL_ON_FUNC( ERM_OPEN_TRADE_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"���� �ŷ� ���� ����.!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			SendToCnRoom( ERM_TRADE_FINISH_NOT );
		}
		//else
		//{
		//	SendToGSField( ERM_TRADE_FINISH_NOT );
		//}
		return;
	}

	m_iTradeUID = kPacket_.m_iTradeUID;

	KEGS_CREATE_TRADE_NOT kPacketNot;
	kPacketNot.m_iTradeUID = kPacket_.m_iTradeUID;
	SendPacket( EGS_CREATE_TRADE_NOT, kPacketNot );
}

IMPL_ON_FUNC( ERM_ACCEPT_JOIN_TRADE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetTradeUID() != 0 )
	{
		START_LOG( cerr, L"�̹� �ٸ� �ŷ��� ���� ����." )
			<< BUILD_LOG( GetTradeUID() )
			<< END_LOG;

		return;
	}

	KERM_JOIN_TRADE_REQ kPacket;
	kPacket.m_kMemberInfo.m_iGSUID	= KBaseServer::GetKObj()->GetUID();
	kPacket.m_kMemberInfo.m_iUserUID = GetUID();
	kPacket.m_kMemberInfo.m_iUnitUID = GetCharUID();
	kPacket.m_kMemberInfo.m_wstrNickName = GetCharName();
	//{{ 2009. 4. 28  ������	�ִ�ED����
	kPacket.m_kMemberInfo.m_iED = GetED();	
	//}}

	_SendToCnRoom( kPacket_, ERM_JOIN_TRADE_REQ, kPacket );
}

IMPL_ON_FUNC( ERM_JOIN_TRADE_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"���� �ŷ� ���� ����." )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		return;
	}

	if( GetStateID() == KGSFSM::S_ROOM )
	{
		SendToCnRoom( ERM_TRADE_START_NOT );
	}
	//else
	//{
	//	SendToGSField( ERM_TRADE_START_NOT );
	//}

	m_iTradeUID = kPacket_.m_iTradeUID;
	KEGS_TRADE_USER_INFO_NOT kPacket;
	kPacket.m_vecTradeUserInfo = kPacket_.m_vecTradeUserInfo;
	SendPacket( EGS_TRADE_USER_INFO_NOT, kPacket );
}

_IMPL_ON_FUNC( ERM_JOIN_TRADE_NOT, KEGS_JOIN_TRADE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// �ڱ� �ڽ��� JOIN�� ACK�� ����.
	if( kPacket_.m_kTradeUserInfo.m_iUnitUID == GetCharUID() )
	{
		return;
	}

	SendPacket( EGS_JOIN_TRADE_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_BREAK_TRADE_REQ )
{
	if( GetStateID() == KGSFSM::S_INIT ||
		GetStateID() == KGSFSM::S_UID_UPDATED ||
		GetStateID() == KGSFSM::S_LOGINED ||
		GetStateID() == KGSFSM::S_SERVER_SELECT )
	{
		START_LOG( cerr, L"�߸��� ����!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

	if( GetTradeUID() == 0 )
	{
		START_LOG( cerr, L"���� �ŷ� UID �̻�.!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_BREAK_TRADE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_TRADE_01;
		SendPacket( EGS_BREAK_TRADE_ACK, kPacket );

		KEGS_BREAK_TRADE_NOT kPacketNot;
		kPacketNot.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
		SendPacket( EGS_BREAK_TRADE_NOT, kPacketNot );
		return;
	}

	_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_BREAK_TRADE_REQ, KEGS_BREAK_TRADE_REQ )
{
	// ���´� ���߿� ����.

	if( GetTradeUID() == 0 )
	{
		START_LOG( cerr, L"���� �ŷ� UID �̻�.!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		return;
	}

	_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_BREAK_TRADE_ACK, KEGS_BREAK_TRADE_ACK )
{
	if( GetStateID() == KGSFSM::S_INIT ||
		GetStateID() == KGSFSM::S_UID_UPDATED ||
		GetStateID() == KGSFSM::S_LOGINED ||
		GetStateID() == KGSFSM::S_SERVER_SELECT )
	{
		START_LOG( cerr, L"�߸��� ����!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_iTradeUID = 0;

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			SendToCnRoom( ERM_TRADE_FINISH_NOT );
		}
		//else
		//{
		//	SendToGSField( ERM_TRADE_FINISH_NOT );
		//}
	}

	SendPacket( EGS_BREAK_TRADE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_BREAK_TRADE_NOT, KEGS_BREAK_TRADE_NOT )
{
	if( GetStateID() == KGSFSM::S_INIT ||
		GetStateID() == KGSFSM::S_UID_UPDATED ||
		GetStateID() == KGSFSM::S_LOGINED ||
		GetStateID() == KGSFSM::S_SERVER_SELECT )
	{
		START_LOG( cerr, L"�߸��� ����!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

	m_iTradeUID = 0;

	if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_00 )
	{
		kPacket_.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
	}

	if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_06 )
	{
		SendPacket( EGS_BREAK_TRADE_NOT, kPacket_ );
	}

	if( GetStateID() == KGSFSM::S_ROOM )
	{
		SendToCnRoom( ERM_TRADE_FINISH_NOT );
	}
	//else
	//{
	//	SendToGSField( ERM_TRADE_FINISH_NOT );
	//}
}

IMPL_ON_FUNC( EGS_UPDATE_TRADE_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_UPDATE_TRADE_ITEM_REQ, EGS_UPDATE_TRADE_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_UPDATE_TRADE_ITEM_ACK kAck;

		//{{ 2012. 03. 29	������	Inventory Lock ���
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"�κ��丮 ���� �ɷ��ִ� �����Դϴ�!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );


			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_UPDATE_TRADE_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		//{{ 2011. 02. 06	������	�ŷ� ���� ���
#ifdef SERV_BLOCK_TRADE
		if( SiKGameSysVal()->IsBlockPersonalTrade() == true )
		{
			kAck.m_iOK = NetError::ERR_BLOCK_TRADE_00;
			SendPacket( EGS_UPDATE_TRADE_ITEM_ACK, kAck );
			return;
		}
#endif SERV_BLOCK_TRADE
		//}}

		//{{ 2011. 03. 02	������	���� �ŷ� ����
#ifdef SERV_HACK_USER_TRADE_BLOCK
		if( m_kUserAbuserManager.IsTradeBlockUser() == true )
		{
#ifdef SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			kAck.m_iOK = NetError::ERR_BLOCK_TRADE_07;
#else // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			kAck.m_iOK = NetError::ERR_TRADE_00;
#endif // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			SendPacket( EGS_UPDATE_TRADE_ITEM_ACK, kAck );
			return;
		}
#endif SERV_HACK_USER_TRADE_BLOCK
		//}}
		
#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	if( m_kUserAbuserManager.IsTradeBlockUnit() == true )
	{
		kAck.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_02;
		SendPacket( EGS_UPDATE_TRADE_ITEM_ACK, kAck );
		return;
	}
#endif SERV_NEW_UNIT_TRADE_LIMIT

		if( GetTradeUID() == 0 )
		{
			START_LOG( cerr, L"���� ���ΰŷ����� ���°� �ƴѵ� ���ΰŷ� ������ ������Ʈ ��Ŷ�� �Դ�." )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_TRADE_02;
			SendPacket( EGS_UPDATE_TRADE_ITEM_ACK, kAck );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_UPDATE_TRADE_ITEM_ACK kPacketAck;
	KERM_UPDATE_TRADE_ITEM_REQ kPacket;

	if( !kPacket_.m_bDelete )
	{
		kPacketAck.m_iOK = m_kInventory.CheckItemForTrade( kPacket_.m_iItemUID, kPacket_.m_iQuantity, kPacket.m_kInventoryItemInfo );
		if( kPacketAck.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"����Ϸ��� ������ �̻�" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( kPacketAck.m_iOK )
				<< BUILD_LOG( NetError::GetErrStr( kPacketAck.m_iOK ) )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< BUILD_LOG( m_kInventory.GetItemID( kPacket_.m_iItemUID ) )
				<< BUILD_LOG( kPacket_.m_iQuantity )
				<< END_LOG;

			SendPacket( EGS_UPDATE_TRADE_ITEM_ACK, kPacketAck );
			return;
		}

		kPacket.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity = kPacket_.m_iQuantity;
	}
	else
	{
		kPacket.m_kInventoryItemInfo.m_iItemUID = kPacket_.m_iItemUID;
	}

	kPacket.m_bDelete = kPacket_.m_bDelete;
	_SendToCnRoom( GetTradeUID(), ERM_UPDATE_TRADE_ITEM_REQ, kPacket );
}

_IMPL_ON_FUNC( ERM_UPDATE_TRADE_ITEM_ACK, KEGS_UPDATE_TRADE_ITEM_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_UPDATE_TRADE_ITEM_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	SendPacket( EGS_UPDATE_TRADE_ITEM_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_UPDATE_TRADE_ITEM_NOT, KEGS_UPDATE_TRADE_ITEM_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_UPDATE_TRADE_ITEM_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_UPDATE_TRADE_ED_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_UPDATE_TRADE_ED_REQ, EGS_UPDATE_TRADE_ED_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_UPDATE_TRADE_ED_ACK kAck;

		//{{ 2012. 03. 29	������	Inventory Lock ���
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"�κ��丮 ���� �ɷ��ִ� �����Դϴ�!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_UPDATE_TRADE_ED_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		//{{ 2011. 02. 06	������	�ŷ� ���� ���
#ifdef SERV_BLOCK_TRADE
		if( SiKGameSysVal()->IsBlockPersonalTrade() == true )
		{
			kAck.m_iOK = NetError::ERR_BLOCK_TRADE_00;
			SendPacket( EGS_UPDATE_TRADE_ED_ACK, kAck );
			return;
		}
#endif SERV_BLOCK_TRADE
		//}}

		//{{ 2011. 03. 02	������	���� �ŷ� ����
#ifdef SERV_HACK_USER_TRADE_BLOCK
		if( m_kUserAbuserManager.IsTradeBlockUser() == true )
		{
#ifdef SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			kAck.m_iOK = NetError::ERR_BLOCK_TRADE_07;
#else // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			kAck.m_iOK = NetError::ERR_TRADE_00;
#endif // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			SendPacket( EGS_UPDATE_TRADE_ED_ACK, kAck );
			return;
		}
#endif SERV_HACK_USER_TRADE_BLOCK
		//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	if( m_kUserAbuserManager.IsTradeBlockUnit() == true )
	{
		kAck.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_02;
		SendPacket( EGS_UPDATE_TRADE_ED_ACK, kAck );
		return;
	}
#endif SERV_NEW_UNIT_TRADE_LIMIT

		//{{ 2011. 04. 11	������	�ŷ� ����
#ifdef SERV_MAX_ED_LIMIT
		if( kPacket_.m_iED > SEnum::UI_MAX_TRADE_ED )
		{
			START_LOG( cerr, L"�ִ� �ŷ� ���� �׼��� �ʰ��Ͽ����ϴ�! ��ŷ�ΰ�?" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#else
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( kPacket_.m_iED )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_TRADE_04;
			SendPacket( EGS_UPDATE_TRADE_ED_ACK, kAck );
			return;
		}
#endif SERV_MAX_ED_LIMIT
		//}}

		//{{ 2010. 03. 17  ������	���� ���� ����
		if( GetTradeUID() == 0 )
		{
			START_LOG( cerr, L"���ΰŷ����� ���°� �ƴѵ� ���ΰŷ� �̺�Ʈ�� ����?" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_TRADE_00;
			SendPacket( EGS_UPDATE_TRADE_ED_ACK, kAck );
			return;
		}
		//}}

		if( kPacket_.m_iED < 0 || kPacket_.m_iED > GetED() )
		{
			START_LOG( cerr, L"�̰�.. ��ŷ�ΰ�?" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iED )
				<< BUILD_LOG( GetED() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_TRADE_04;
			SendPacket( EGS_UPDATE_TRADE_ED_ACK, kAck );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	KERM_UPDATE_TRADE_ED_REQ kPacket;
	kPacket = kPacket_.m_iED;
	_SendToCnRoom( GetTradeUID(), ERM_UPDATE_TRADE_ED_REQ, kPacket );
}

_IMPL_ON_FUNC( ERM_UPDATE_TRADE_ED_ACK, KEGS_UPDATE_TRADE_ED_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_UPDATE_TRADE_ED_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	SendPacket( EGS_UPDATE_TRADE_ED_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_UPDATE_TRADE_ED_NOT, KEGS_UPDATE_TRADE_ED_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_UPDATE_TRADE_ED_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_OFFER_TRADE_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2010. 03. 17  ������	���� ���� ����
	if( GetTradeUID() == 0 )
	{
		START_LOG( cerr, L"���ΰŷ����� �ƴѵ� ���ΰŷ� �̺�Ʈ�� ����?" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return;
	}
	//}}

	_SendToCnRoom( GetTradeUID(), ERM_OFFER_TRADE_REQ, char() );
}

_IMPL_ON_FUNC( ERM_OFFER_TRADE_ACK, KEGS_OFFER_TRADE_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_OFFER_TRADE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_OFFER_TRADE_NOT, KEGS_OFFER_TRADE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_OFFER_TRADE_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_APPROVE_TRADE_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_APPROVE_TRADE_REQ, EGS_APPROVE_TRADE_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		//{{ 2012. 03. 29	������	Inventory Lock ���
#ifdef SERV_INVENTORY_LOCK
		KEGS_APPROVE_TRADE_ACK kAck;
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"�κ��丮 ���� �ɷ��ִ� �����Դϴ�!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_APPROVE_TRADE_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	//{{ 2012. 05. 08	��μ�       ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
	WriteUnitInfoToDB( false, ERM_APPROVE_TRADE_REQ );

	//{{ 2012. 09. 22	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	BingoEventDBWrite();
#endif SERV_EVENT_BINGO
	//}}
#else
	_SendToCnRoom( GetTradeUID(), ERM_APPROVE_TRADE_REQ, char() );
#endif SERV_TRADE_LOGIC_CHANGE_TRADE
	//}}
}

_IMPL_ON_FUNC( ERM_APPROVE_TRADE_ACK, KEGS_APPROVE_TRADE_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_APPROVE_TRADE_ACK );

	SendPacket( EGS_APPROVE_TRADE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_APPROVE_TRADE_NOT, KEGS_APPROVE_TRADE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_APPROVE_TRADE_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_CONFIRM_TRADE_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KERM_CONFIRM_TRADE_ACK kPacketAck;

	if( m_kInventory.CheckTradeItemOut( kPacket_.m_vecItemOut, kPacketAck.m_kItemQuantityUpdateTemp, CXSLRoom::RT_TRADE ) == false )
	{
		START_LOG( cerr, L"���ΰŷ� confirm üũ ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::GetLastError();
		_SendToCnRoom( GetTradeUID(), ERM_CONFIRM_TRADE_ACK, kPacketAck );

#ifdef	SERV_SHARING_BANK_TEST
		//	�ŷ��� �����°͸� üũ�ϸ� �ɱ�?
		if( kPacketAck.m_iOK == NetError::ERR_SHARE_BANK_02 )
		{
			//	��� �����
			CTime tCurTime = CTime::GetCurrentTime();

			KELOG_ITEM_TRADE_ERROR_NOT kErrorNot;
			kErrorNot.m_iUnitUID = GetCharUID();
			kErrorNot.m_iType = SEnum::TET_TRADE;
			kErrorNot.m_wsrtRegDate = ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );

			SendToLogDB( ELOG_ITEM_TRADE_ERROR_NOT, kErrorNot );
		}
#endif	SERV_SHARING_BANK_TEST
		return;
	}

	if( kPacket_.m_iEDOut > GetED() )
	{
		START_LOG( cerr, L"�̰�.. ��ŷ�ΰ�?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iEDOut )
			<< BUILD_LOG( GetED() )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_TRADE_06;
		_SendToCnRoom( GetTradeUID(), ERM_CONFIRM_TRADE_ACK, kPacketAck );
		return;
	}

	if( m_kInventory.CheckTradeItemIn( kPacket_.m_vecItemIn, kPacketAck.m_kItemQuantityUpdateTemp, kPacketAck.m_vecItemInfoTemp, CXSLRoom::RT_TRADE ) == false )
	{
		kPacketAck.m_iOK = NetError::GetLastError();
		_SendToCnRoom( GetTradeUID(), ERM_CONFIRM_TRADE_ACK, kPacketAck );
		return;
	}

	// ������ ���������� ED�� ���ҽ�Ų���� ���� ED���� Temp�� ����
	kPacketAck.m_iEDTemp = ( GetED() - kPacket_.m_iEDOut + kPacket_.m_iEDIn );

	kPacketAck.m_iOK = NetError::NET_OK;
	_SendToCnRoom( GetTradeUID(), ERM_CONFIRM_TRADE_ACK, kPacketAck );
	return;
}

//{{ 2012. 05. 08	��μ�       ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
IMPL_ON_FUNC( ERM_CONFIRM_TRADE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KERM_DB_UPDATE_TRADE_RESULT_NOT kPacketToCnTrade;
	kPacketToCnTrade.m_iUnitUID = GetCharUID();

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( IsSameServerGroupUID( kPacket_.m_iTradeUID ) == false )
	{
		START_LOG( cerr, L"�ٸ� �������� TradeUID�ε� �̰����� ��� ����? ���� �Ͼ�� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iTradeUID )
			<< BUILD_LOG( KncUid::ExtractServerGroupID( kPacket_.m_iTradeUID ) )
			<< BUILD_LOG( KncUid::ExtractServerGroupID( GetCharUID() ) )
			<< END_LOG;

		// �α׸� ���.
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
	if( SiKGameSysVal()->GetLetterAndTradeMonitoringIsOn() == true )
	{
		// �ŷ� �� �� �����۰� ED�� ���� ���� ED �� ���� ��
		if( kPacket_.m_iEDOut <= 0 && kPacket_.m_vecItemOut.empty() && kPacket_.m_iEDIn > 0  )
		{
			if( m_kUserHackingManager.AddTradeMonitoringList( kPacket_.m_iOtherCID ) == true )
			{
				KDBE_WRITE_LETTER_AND_TRADE_MOINTORING_REQ PacketReq;
				CTime tCurTime = CTime::GetCurrentTime();

				PacketReq.m_iRecvUnitUID = GetCharUID();
				PacketReq.m_wstrRegDate = ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
				PacketReq.m_cType = 1;

				SendToLogDB( DBE_WRITE_LETTER_AND_TRADE_MOINTORING_REQ, PacketReq );
			}
		}
	}
#endif SERV_MONITORING_LETTER_AND_TRADE
	//}}

	//////////////////////////////////////////////////////////////////////////
	// ED�ŷ�
	{
		//{{ 2012. 10. 02	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
#else
		// ED �����
		if( kPacket_.m_iEDOut != 0 )
		{
			//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
//#ifdef SERV_USER_ABUSER_MANAGER
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_OUT_TRADE, GetED() - kPacket_.m_iEDOut );
//#endif SERV_USER_ABUSER_MANAGER
			//}}
		}

		if( kPacket_.m_iEDIn != 0 )
		{
			//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
//#ifdef SERV_USER_ABUSER_MANAGER			
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_IN_TRADE, GetED() - kPacket_.m_iEDOut + kPacket_.m_iEDIn );
//#endif SERV_USER_ABUSER_MANAGER
			//}}
		}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}

		// ED ���� ���� ����ȭ ���� Ȯ�� ����
		//{{ 2012. 10. 02	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		if( m_kEDManager.GetChangeValue() != 0 )
#else
		if( m_iED.GetChangeValue() != 0 )
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}		
		{
			START_LOG( cerr, L"���� �ŷ� ������ ����-DB ����ȭ�� ���� �ʾҴ�." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
				//{{ 2012. 10. 02	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
				<< BUILD_LOG( m_kEDManager.GetChangeValue() )
#else
				<< BUILD_LOG( m_iED.GetChangeValue() )
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
				//}}				
				<< END_LOG;
		}

		// DB������Ʈ�� ED��ȭ�� ����
		kPacketToCnTrade.m_iChangedED = ( kPacket_.m_iEDIn - kPacket_.m_iEDOut );
		//{{ 2012. 10. 02	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		SetED( GetED() - kPacket_.m_iEDOut, KUserEDManager::ER_SET_ED_OUT_TRADE );
		SetED( GetED() + kPacket_.m_iEDIn, KUserEDManager::ER_SET_ED_IN_TRADE );
#else
		m_iED.SetValue( GetED() + kPacketToCnTrade.m_iChangedED );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}

		
		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( 0 );
			KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PItemTrade, kPacket_.m_iEDIn );
			KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MItemTrade, kPacket_.m_iEDOut );
		}

		{
			// ���� ���
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_TradeCount, 1 );
		}
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ������ ������
	BOOST_TEST_FOREACH( const KInventoryItemInfo&, kOutItemInfo, kPacket_.m_vecItemOut )
	{
		int iCategory;
		int iSlotID;
		LIF( m_kInventory.GetCategorySlot( kOutItemInfo.m_iItemUID, iCategory, iSlotID ) );
		LIF( static_cast<int>(kOutItemInfo.m_cSlotCategory) == iCategory );
		LIF( static_cast<int>(kOutItemInfo.m_sSlotID) == iSlotID );

		KInventoryItemInfo kInfo;
		KInventoryItemInfo kInfoUpdate;
		if( kOutItemInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_QUANTITY )
		{
			int iDecreased;
			KInventoryItemInfo kBeforQuantityInfo;
			m_kInventory.GetInventoryItemInfo( kOutItemInfo.m_iItemUID, kBeforQuantityInfo );

			m_kInventory.DecreaseQuantity( kOutItemInfo.m_iItemUID, kOutItemInfo.m_kItemInfo.m_iQuantity, iDecreased, KDeletedItemInfo::DR_TRADE );
			LIF( kOutItemInfo.m_kItemInfo.m_iQuantity == iDecreased );
			m_kInventory.GetInventorySlotInfo( iCategory, iSlotID, kInfo );

			// ������ �������� ���� �Ǹ� ������ ������ �ʱ�ȭ �Ǳ� ������ �����۾��̵� ������ �д�.
			kInfo.m_kItemInfo.m_iItemID = kBeforQuantityInfo.m_kItemInfo.m_iItemID;
			kInfo.m_kItemInfo.m_cUsageType = kBeforQuantityInfo.m_kItemInfo.m_cUsageType;
			kInfo.m_kItemInfo.m_iQuantity = kBeforQuantityInfo.m_kItemInfo.m_iQuantity - iDecreased;

			kInfoUpdate = kInfo;
			kInfoUpdate.m_kItemInfo.m_iQuantity = iDecreased;
		}
		else
		{
			KInventoryItemInfo kBeforDeleteInfo;
			KInventoryItemInfo kTempInfo;
			m_kInventory.GetInventoryItemInfo( kOutItemInfo.m_iItemUID, kBeforDeleteInfo );
			m_kInventory.DeleteItem( kOutItemInfo.m_iItemUID, kTempInfo, KDeletedItemInfo::DR_TRADE );

			kInfoUpdate = kBeforDeleteInfo;
			kInfo = kBeforDeleteInfo;
			kInfo.m_kItemInfo.m_iQuantity = 0;
		}

		// �κ��丮 ���� ����
		kInfoUpdate.m_iItemUID = kOutItemInfo.m_iItemUID;
		kPacketToCnTrade.m_vecUpdated.push_back( kInfoUpdate );
		
		kInfo.m_iItemUID = kOutItemInfo.m_iItemUID;
		kPacketToCnTrade.m_vecUpdatedInventorySlot.push_back( kInfo );

		// ���� ��� ���ΰŷ� �Ǹ� ī��Ʈ
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Trade_Sell_Item, kOutItemInfo.m_kItemInfo.m_iQuantity );

		// ������� �ŷ� ������
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( kOutItemInfo.m_kItemInfo.m_iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_TRADE, kOutItemInfo.m_kItemInfo.m_iQuantity );
		//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_TRADE, kOutItemInfo.m_kItemInfo.m_iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	// ������ ���������� ���� �κ��丮 ���� ����
	{
		m_kInventory.FlushQuantityChange( kPacketToCnTrade.m_kItemQuantityUpdate_OUT.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketToCnTrade.m_kItemQuantityUpdate_OUT.m_vecDeleted );
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ������ ������
	if( !kPacket_.m_vecItemIn.empty() )
	{
		for( u_int ui = 0; ui < kPacket_.m_vecItemIn.size(); ++ui )
		{
			// ���� ��� ���ΰŷ� ���� ī��Ʈ
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Trade_Buy_Item, kPacket_.m_vecItemIn[ui].m_kItemInfo.m_iQuantity );
		}

		//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
		// ������ �����
		std::map<int, int> mapInsertedItem;
		std::vector<KInventoryItemInfo>::iterator vit = kPacket_.m_vecItemIn.begin();
		for( ; vit != kPacket_.m_vecItemIn.end() ; ++vit )
		{
			mapInsertedItem.insert(std::make_pair( vit->m_kItemInfo.m_iItemID, vit->m_kItemInfo.m_iQuantity ));
		}
		
		m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_TRADE, mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
		//}}
	}

	//////////////////////////////////////////////////////////////////////////
	// DB������Ʈ�� ���� ���� ����
	{
		_SendToCnRoom( GetTradeUID(), ERM_DB_UPDATE_TRADE_RESULT_NOT, kPacketToCnTrade );

		m_kInventory.Lock( KInventoryLocker::LT_TIME_OUT, 60.0 );
	}
	//////////////////////////////////////////////////////////////////////////
}

_IMPL_ON_FUNC( ERM_DB_UPDATE_TRADE_COMPLETE_NOT, KERM_DB_UPDATE_TRADE_RESULT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//////////////////////////////////////////////////////////////////////////
	// ED��� ó��
	{
		// �κ��丮 ���!
		m_kInventory.Unlock();

		//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
		if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE )
		{
			// ��ȭ�� ED ������ SQUARE �� ���� ���� ����.
			KERM_SQUARE_UNIT_REFRESH_ED_NOT kReq;
			kReq.m_iUnitUID = GetCharUID();
			kReq.m_iED = GetED();
			SendToCnRoom( ERM_SQUARE_UNIT_REFRESH_ED_NOT, kReq );
		}
#endif SERV_ED_MONITORING_IN_GAME
	}
	//////////////////////////////////////////////////////////////////////////
	KDBE_TRADE_COMPLETE_REQ kPacketReq;
	std::vector< KInventoryItemInfo >	vecInventorySlotInfo;
	std::map< UidType, KInventoryItemInfo > mapItemInfo;
	std::map< UidType, KInventoryItemInfo >::iterator mit;
	std::map< UidType, KInventoryItemInfo >::iterator mitTemp;

	std::map< UidType, KItemInfo > mapInsertItemInfo;

	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_iED = GetED();

	kPacketReq.m_vecInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;

	// ������ ������ �̹Ƿ� ������ UID �� 0���� �ϸ� Ŭ�󿡼� ���� ó���ȴ�.
	BOOST_TEST_FOREACH( KInventoryItemInfo&, kInsertItemInfo, kPacketReq.m_vecInventorySlotInfo )
	{
		// Ŭ���̾�Ʈ���� ������ ���� ���� uid �� ������
		if( kInsertItemInfo.m_kItemInfo.m_iQuantity <= 0 )
		{
			kInsertItemInfo.m_iItemUID  = 0;
		}
	}

	// ��� ���� �������� �ִ�.
	if( kPacket_.m_vecInsertDBItem.empty() == false )
	{
		// ���� �߰��� ������ �� �ߺ��Ǵ� �κ��丮 ����� ��������
		m_kInventory.FixUpInventoryItem( kPacket_.m_vecInsertDBItem, kPacketReq.m_kQuantityUpdate , kPacketReq.m_vecInventorySlotInfo );

		// �ŷ��� ���Ͽ� �߰��� �������� ���� �߰� ����
		// ������ ��� �� item
		BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInsertItemInfo, kPacket_.m_vecInsertDBItem )
		{
			KItemInfo kInfo = kInsertItemInfo.m_kItemInfo;

			if( kInsertItemInfo.m_iItemUID <= 0 )
				continue;

			if( kInfo.m_iQuantity <= 0 )
				continue;

			mapInsertItemInfo.insert( std::make_pair( kInsertItemInfo.m_iItemUID, kInfo ) );
		}
		std::vector< KInventoryItemInfo >	vecInsertInventoryTemp;

		if( m_kInventory.InsertItem( mapInsertItemInfo, vecInsertInventoryTemp ) == false )
		{
			BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInsertItemInfo, vecInsertInventoryTemp )
			{
				START_LOG( cerr, L"�߰��� �����ߴٰ�?!" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kInsertItemInfo.m_iItemUID )
					<< BUILD_LOG( kInsertItemInfo.m_sSlotID )
					<< BUILD_LOG( kInsertItemInfo.m_kItemInfo.m_iQuantity )
					<< END_LOG;
			}
		}

		// �߰��� �������� ���� ��Ͽ� �߰��Ѵ�.
		BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInsertItemInfo, vecInsertInventoryTemp )
		{
			BOOST_TEST_FOREACH( KInventoryItemInfo&, kTempInfo, kPacketReq.m_vecInventorySlotInfo )
			{
				if( kInsertItemInfo.m_iItemUID <= 0 )
					continue;

				if( kInsertItemInfo.m_iItemUID == kTempInfo.m_iItemUID )
				{
					kTempInfo.m_sSlotID = kInsertItemInfo.m_sSlotID;
					break;
				}
			}

			kPacketReq.m_vecInventorySlotInfo.push_back( kInsertItemInfo );
		}

		// �ߺ� ����,�߰� ����
		KItemQuantityUpdate kQuantityUpdate;
		m_kInventory.FlushQuantityChange( kQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kQuantityUpdate.m_vecDeleted );
	}

	SendToGameDB( DBE_TRADE_COMPLETE_REQ, kPacketReq );
}

IMPL_ON_FUNC( DBE_TRADE_COMPLETE_ACK )
{
	KEGS_TRADE_COMPLETE_NOT kPacketNot;
	//m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	//m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	kPacketNot.m_iED = kPacket_.m_iED;
	kPacketNot.m_vecInventorySlotInfo.insert( kPacketNot.m_vecInventorySlotInfo.begin(), kPacket_.m_vecInventorySlotInfo.begin(), kPacket_.m_vecInventorySlotInfo.end() );
	SendPacket( EGS_TRADE_COMPLETE_NOT, kPacketNot );
}
#else

//{{ 2012. 03. 28	������	���ΰŷ� DDOS ���
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_TRADE_DDOS_DEFENCE
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( ERM_CONFIRM_TRADE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KERM_DB_UPDATE_TRADE_RESULT_NOT kPacketToCnTrade;

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( IsSameServerGroupUID( kPacket_.m_iTradeUID ) == false )
	{
		START_LOG( cerr, L"�ٸ� �������� TradeUID�ε� �̰����� ��� ����? ���� �Ͼ�� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iTradeUID )
			<< BUILD_LOG( KncUid::ExtractServerGroupID( kPacket_.m_iTradeUID ) )
			<< BUILD_LOG( KncUid::ExtractServerGroupID( GetCharUID() ) )
			<< END_LOG;

		// �α׸� ���.
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
	if( SiKGameSysVal()->GetLetterAndTradeMonitoringIsOn() == true )
	{
		// �ŷ� �� �� �����۰� ED�� ���� ���� ED �� ���� ��
		if( kPacket_.m_iEDOut <= 0 && kPacket_.m_vecItemOut.empty() && kPacket_.m_iEDIn > 0  )
		{
			if( m_kUserHackingManager.AddTradeMonitoringList( kPacket_.m_iOtherCID ) == true )
			{
				KDBE_WRITE_LETTER_AND_TRADE_MOINTORING_REQ PacketReq;
				CTime tCurTime = CTime::GetCurrentTime();

				PacketReq.m_iRecvUnitUID = GetCharUID();
				PacketReq.m_wstrRegDate = ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
				PacketReq.m_cType = 1;

				SendToLogDB( DBE_WRITE_LETTER_AND_TRADE_MOINTORING_REQ, PacketReq );
			}
		}
	}
#endif SERV_MONITORING_LETTER_AND_TRADE
	//}}

	//////////////////////////////////////////////////////////////////////////
	// ED�ŷ�
	{
		int iBeforeED = m_iED;
		
		m_iED -= kPacket_.m_iEDOut;
		
		// ED �����
		if( kPacket_.m_iEDOut != 0 )
		{
			//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
		#ifdef SERV_USER_ABUSER_MANAGER
			// ED �����
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_OUT_TRADE, iBeforeED );
		#endif SERV_USER_ABUSER_MANAGER
			//}}
		}
		
		iBeforeED = m_iED;
		
		m_iED += kPacket_.m_iEDIn;	
		
		if( kPacket_.m_iEDIn != 0 )
		{
			//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
		#ifdef SERV_USER_ABUSER_MANAGER
			// ED �����
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_IN_TRADE, iBeforeED );
		#endif SERV_USER_ABUSER_MANAGER
			//}}
		}

		// DB������Ʈ�� ED���а��� �������! ( �ŷ��� ���а� + �ŷ��߰��� - �ŷ����ҷ� )
		// ���⼭ ���а��� DB�� ������Ʈ �Ϸ� ���ϱ� WriteUnitInfoToDB()������ ED���а��� DB�� ������Ʈ �ϸ� �ȵȴ�!
		kPacketToCnTrade.m_iChangedED = m_iED.GetChangeValue();

		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( 0 );
			KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PItemTrade, kPacket_.m_iEDIn );
			KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MItemTrade, kPacket_.m_iEDOut );
		}

		{
			// ���� ���
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_TradeCount, 1 );
		}
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ������ ������
	BOOST_TEST_FOREACH( const KInventoryItemInfo&, kOutItemInfo, kPacket_.m_vecItemOut )
	{
		int iCategory;
		int iSlotID;
		LIF( m_kInventory.GetCategorySlot( kOutItemInfo.m_iItemUID, iCategory, iSlotID ) );
		LIF( static_cast<int>(kOutItemInfo.m_cSlotCategory) == iCategory );
		LIF( static_cast<int>(kOutItemInfo.m_sSlotID) == iSlotID );

		KInventoryItemInfo kInfo;
		if( kOutItemInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_QUANTITY )
		{
			int iDecreased;
			m_kInventory.DecreaseQuantity( kOutItemInfo.m_iItemUID, kOutItemInfo.m_kItemInfo.m_iQuantity, iDecreased, KDeletedItemInfo::DR_TRADE );
			LIF( kOutItemInfo.m_kItemInfo.m_iQuantity == iDecreased );
			m_kInventory.GetInventorySlotInfo( iCategory, iSlotID, kInfo );
		}
		else
		{
			m_kInventory.DeleteItem( kOutItemInfo.m_iItemUID, kInfo, KDeletedItemInfo::DR_TRADE );
		}

		// �κ��丮 ���� ����
		kPacketToCnTrade.m_vecUpdatedInventorySlot.push_back( kInfo );

		// ���� ��� ���ΰŷ� �Ǹ� ī��Ʈ
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Trade_Sell_Item, kOutItemInfo.m_kItemInfo.m_iQuantity );

		// ������� �ŷ� ������
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( kOutItemInfo.m_kItemInfo.m_iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_TRADE, kOutItemInfo.m_kItemInfo.m_iQuantity );
		//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_TRADE, kOutItemInfo.m_kItemInfo.m_iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ������ ������
	if( !kPacket_.m_vecItemIn.empty() )
	{
		// 10�� ���ΰŷ��� �����ϵ��� ����
		std::vector< KInventoryItemInfo > vecUpdatedInventorySlot;
		m_kInventory.PrepareInsertForTrade( kPacket_.m_vecItemIn, kPacketToCnTrade.m_mapInsertedItem, vecUpdatedInventorySlot, kPacketToCnTrade.m_vecItemInfo );
		kPacketToCnTrade.m_vecUpdatedInventorySlot.insert( kPacketToCnTrade.m_vecUpdatedInventorySlot.begin(), vecUpdatedInventorySlot.begin(), vecUpdatedInventorySlot.end() );

		for( u_int ui = 0; ui < kPacket_.m_vecItemIn.size(); ++ui )
		{
			// ���� ��� ���ΰŷ� ���� ī��Ʈ
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Trade_Buy_Item, kPacket_.m_vecItemIn[ui].m_kItemInfo.m_iQuantity );
		}

		//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
		// ������ �����
		m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_TRADE, kPacketToCnTrade.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
		//}}
	}

	//////////////////////////////////////////////////////////////////////////
	// DB������Ʈ�� ���� ���� ����
	{
		kPacketToCnTrade.m_iUnitUID = GetCharUID();
		m_kInventory.FlushQuantityChange( kPacketToCnTrade.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketToCnTrade.m_kItemQuantityUpdate.m_vecDeleted );
		_SendToCnRoom( GetTradeUID(), ERM_DB_UPDATE_TRADE_RESULT_NOT, kPacketToCnTrade );

		// �κ��丮 ��!
		m_kInventory.Lock( KInventoryLocker::LT_TIME_OUT );
	}
	//////////////////////////////////////////////////////////////////////////
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

//IMPL_ON_FUNC( ERM_CONFIRM_TRADE_NOT )
//{
//	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
//
//	//{{ 2010. 05. 18  ������	���� ���� ������ ����
//#ifdef SERV_INTEGRATION
//	if( IsSameServerGroupUID( kPacket_.m_iTradeUID ) == false )
//	{
//		START_LOG( cerr, L"�ٸ� �������� TradeUID�ε� �̰����� ��� ����? ���� �Ͼ�� �ȵǴ� ����!" )
//			<< BUILD_LOG( GetCharUID() )
//			<< BUILD_LOG( GetCharName() )
//			<< BUILD_LOG( kPacket_.m_iTradeUID )
//			<< BUILD_LOG( KncUid::ExtractServerGroupID( kPacket_.m_iTradeUID ) )
//			<< BUILD_LOG( KncUid::ExtractServerGroupID( GetCharUID() ) )
//			<< END_LOG;
//
//		// �α׸� ���.
//	}
//#endif SERV_INTEGRATION
//	//}}
//
//	//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
//#ifdef SERV_MONITORING_LETTER_AND_TRADE
//	if( SiKGameSysVal()->GetLetterAndTradeMonitoringIsOn() == true )
//	{
//		// �ŷ� �� �� �����۰� ED�� ���� ���� ED �� ���� ��
//		if( kPacket_.m_iEDOut <= 0 && kPacket_.m_vecItemOut.empty() && kPacket_.m_iEDIn > 0  )
//		{
//			if( m_kUserHackingManager.AddTradeMonitoringList( kPacket_.m_iOtherCID ) == true )
//			{
//				KDBE_WRITE_LETTER_AND_TRADE_MOINTORING_REQ PacketReq;
//				CTime tCurTime = CTime::GetCurrentTime();
//
//				PacketReq.m_iRecvUnitUID = GetCharUID();
//				PacketReq.m_wstrRegDate = ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
//				PacketReq.m_cType = 1;
//
//				SendToLogDB( DBE_WRITE_LETTER_AND_TRADE_MOINTORING_REQ, PacketReq );
//			}
//		}
//	}
//#endif SERV_MONITORING_LETTER_AND_TRADE
//	//}}
//
//	std::vector< KInventoryItemInfo > vecUpdatedSlot;
//	BOOST_TEST_FOREACH( const KInventoryItemInfo&, kOutItemInfo, kPacket_.m_vecItemOut )
//	{
//		int iCategory;
//		int iSlotID;
//		LIF( m_kInventory.GetCategorySlot( kOutItemInfo.m_iItemUID, iCategory, iSlotID ) );
//		LIF( static_cast<int>(kOutItemInfo.m_cSlotCategory) == iCategory );
//		//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
//		//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
//		LIF( static_cast<int>(kOutItemInfo.m_sSlotID) == iSlotID );
//		//#else
//		//		LIF( static_cast<int>(vit->m_cSlotID) == iSlotID );
//		//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
//		//}}
//
//		KInventoryItemInfo kInfo;
//		if( kOutItemInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_QUANTITY )
//		{
//			int iDecreased;
//			m_kInventory.DecreaseQuantity( kOutItemInfo.m_iItemUID, kOutItemInfo.m_kItemInfo.m_iQuantity, iDecreased, KDeletedItemInfo::DR_TRADE );
//			LIF( kOutItemInfo.m_kItemInfo.m_iQuantity == iDecreased );
//			m_kInventory.GetInventorySlotInfo( iCategory, iSlotID, kInfo );
//		}
//		else
//		{
//			m_kInventory.DeleteItem( kOutItemInfo.m_iItemUID, kInfo, KDeletedItemInfo::DR_TRADE );
//		}
//		vecUpdatedSlot.push_back( kInfo );
//
//		// ���� ��� ���ΰŷ� �Ǹ� ī��Ʈ
//		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Trade_Sell_Item, kOutItemInfo.m_kItemInfo.m_iQuantity );
//
//		// ������� �ŷ� ������
//		KStatisticsKey kKey;
//		kKey.m_vecIntKey.push_back( kOutItemInfo.m_kItemInfo.m_iItemID );
//		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_TRADE, kOutItemInfo.m_kItemInfo.m_iQuantity );
//		//{{ 2011. 04. 13	������	������ ��� DB���
//#ifdef SERV_ITEM_STATISTICS_TO_DB
//		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_TRADE, kOutItemInfo.m_kItemInfo.m_iQuantity );
//#endif SERV_ITEM_STATISTICS_TO_DB
//		//}}
//	}
//	//////////////////////////////////////////////////////////////////////////
//
//	//////////////////////////////////////////////////////////////////////////
//	// ED�ŷ�
//
//	int iBeforeED = m_iED;
//
//	m_iED -= kPacket_.m_iEDOut;
//
//	// ED �����
//	if( kPacket_.m_iEDOut != 0 )
//	{
//		//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
//#ifdef SERV_USER_ABUSER_MANAGER
//		// ED �����
//		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_OUT_TRADE, iBeforeED );
//#endif SERV_USER_ABUSER_MANAGER
//		//}}
//	}
//
//	iBeforeED = m_iED;
//
//	m_iED += kPacket_.m_iEDIn;	
//
//	if( kPacket_.m_iEDIn != 0 )
//	{
//		//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
//#ifdef SERV_USER_ABUSER_MANAGER
//		// ED �����
//		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_IN_TRADE, iBeforeED );
//#endif SERV_USER_ABUSER_MANAGER
//		//}}
//	}
//
//	{
//		KStatisticsKey kKey;
//		kKey.m_vecIntKey.push_back( 0 );
//		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PItemTrade, kPacket_.m_iEDIn );
//		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MItemTrade, kPacket_.m_iEDOut );
//	}
//
//	{
//		// ���� ���
//		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_TradeCount, 1 );
//	}
//
//	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
//#ifdef SERV_ED_MONITORING_IN_GAME
//	if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE )
//	{
//		// ��ȭ�� ED ������ SQUARE �� ���� ���� ����.
//		KERM_SQUARE_UNIT_REFRESH_ED_NOT kReq;
//		kReq.m_iUnitUID = GetCharUID();
//		kReq.m_iED = m_iED;
//
//		SendToCnRoom( ERM_SQUARE_UNIT_REFRESH_ED_NOT, kReq );
//	}
//#endif SERV_ED_MONITORING_IN_GAME
//	//}}
//	//////////////////////////////////////////////////////////////////////////
//
//	//////////////////////////////////////////////////////////////////////////
//	// ������ ������
//	if( !kPacket_.m_vecItemIn.empty() )
//	{
//		KDBE_INSERT_TRADE_ITEM_REQ kPacketReq;
//
//		// 10�� ���ΰŷ��� �����ϵ��� ����
//		m_kInventory.PrepareInsertForTrade( kPacket_.m_vecItemIn, kPacketReq.m_mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo );
//		kPacketReq.m_vecUpdatedInventorySlot.insert( kPacketReq.m_vecUpdatedInventorySlot.begin(), vecUpdatedSlot.begin(), vecUpdatedSlot.end() );
//
//		if( !kPacketReq.m_vecItemInfo.empty() )
//		{
//			kPacketReq.m_iUnitUID = GetCharUID();
//			m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
//			m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
//			SendToGameDB( DBE_INSERT_TRADE_ITEM_REQ, kPacketReq );
//
//			START_LOG_WITH_NAME( clog2 )
//				<< BUILD_LOG( ( int )kPacketReq.m_vecUpdatedInventorySlot.size() );
//		}
//		else
//		{
//			KEGS_TRADE_COMPLETE_NOT kPacketNot;
//			kPacketNot.m_iED = GetED();
//			kPacketNot.m_vecInventorySlotInfo = kPacketReq.m_vecUpdatedInventorySlot;
//			SendPacket( EGS_TRADE_COMPLETE_NOT, kPacketNot );
//
//			START_LOG_WITH_NAME( clog2 )
//				<< BUILD_LOG( ( int )kPacketNot.m_vecInventorySlotInfo.size() );
//		}
//
//		KELOG_ITEM_ABUSER_LOG_NOT kPacketToLog;
//
//		for( u_int ui = 0; ui < kPacket_.m_vecItemIn.size(); ++ui )
//		{
//			// ���� ��� ���ΰŷ� ���� ī��Ʈ
//			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Trade_Buy_Item, kPacket_.m_vecItemIn[ui].m_kItemInfo.m_iQuantity );
//		}
//
//		//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
//#ifdef SERV_USER_ABUSER_MANAGER
//		// ������ �����
//		m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_TRADE, kPacketReq.m_mapInsertedItem );
//#endif SERV_USER_ABUSER_MANAGER
//		//}}
//	}
//	else	
//	{
//		KEGS_TRADE_COMPLETE_NOT kPacketNot;
//		kPacketNot.m_iED = GetED();
//		kPacketNot.m_vecInventorySlotInfo = vecUpdatedSlot;
//		SendPacket( EGS_TRADE_COMPLETE_NOT, kPacketNot );
//
//		START_LOG_WITH_NAME( clog2 )
//			<< BUILD_LOG( ( int )kPacketNot.m_vecInventorySlotInfo.size() );
//	}
//}

//////////////////////////////////////////////////////////////////////////
#endif SERV_TRADE_DDOS_DEFENCE
//////////////////////////////////////////////////////////////////////////
//}}

//{{ 2012. 03. 28	������	���ΰŷ� DDOS ���
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_TRADE_DDOS_DEFENCE
//////////////////////////////////////////////////////////////////////////
IMPL_ON_FUNC( ERM_DB_UPDATE_TRADE_COMPLETE_NOT )
{
    VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//////////////////////////////////////////////////////////////////////////
	// ED��� ó��
	{
		// �κ��丮 ���!
		m_kInventory.Unlock();

		// DB�� ���ٿ� ��ŭ SetValue������!DB�� ������Ʈ�� ���� �̹Ƿ� �ٷ� SetData������!
		if( m_iED.GetChangeValue() != kPacket_.m_iChangedED )
		{
			START_LOG( cout, L"[�˸�] ���ΰŷ��� ���� Lock�� �� ���̿� ED�� ��ȭ�� �Ͼ��! �̰� �� �������ΰ�?" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetED() )
				<< BUILD_LOG( m_iED.GetChangeValue() )
				<< BUILD_LOG( kPacket_.m_iChangedED );
		}

		m_iED.SetValue( GetED() );
		//m_iED.AdjustInitValue( m_iED.GetInitValue() - kPacket_.m_iED );

		//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
		if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE )
		{
			// ��ȭ�� ED ������ SQUARE �� ���� ���� ����.
			KERM_SQUARE_UNIT_REFRESH_ED_NOT kReq;
			kReq.m_iUnitUID = GetCharUID();
			kReq.m_iED = GetED();
			SendToCnRoom( ERM_SQUARE_UNIT_REFRESH_ED_NOT, kReq );
		}
#endif SERV_ED_MONITORING_IN_GAME
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_TRADE_COMPLETE_NOT kPacketNot;
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacketNot.m_vecInventorySlotInfo );
	kPacketNot.m_iED = GetED();
	kPacketNot.m_vecInventorySlotInfo.insert( kPacketNot.m_vecInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );
	SendPacket( EGS_TRADE_COMPLETE_NOT, kPacketNot );

	START_LOG_WITH_NAME( clog2 )
		<< BUILD_LOG( ( int )kPacketNot.m_vecInventorySlotInfo.size() );
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( DBE_INSERT_TRADE_ITEM_ACK )
{
	// TODO : ���� ���ƾ� �ϳ�..

	KEGS_TRADE_COMPLETE_NOT kPacketNot;

	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacketNot.m_vecInventorySlotInfo );

	kPacketNot.m_iED = GetED();
	kPacketNot.m_vecInventorySlotInfo.insert( kPacketNot.m_vecInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );
	SendPacket( EGS_TRADE_COMPLETE_NOT, kPacketNot );

	START_LOG_WITH_NAME( clog2 )
		<< BUILD_LOG( ( int )kPacketNot.m_vecInventorySlotInfo.size() );
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_TRADE_DDOS_DEFENCE
//////////////////////////////////////////////////////////////////////////
//}}

#endif SERV_TRADE_LOGIC_CHANGE_TRADE
//}}

_IMPL_ON_FUNC( ERM_CHANGE_TRADE_NOT, KEGS_CHANGE_TRADE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_TRADE_NOT, kPacket_ );
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


