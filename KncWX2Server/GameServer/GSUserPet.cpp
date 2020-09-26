#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"

#include "odbc/Odbc.h"

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
	#include "X2Data/XSLRidingPetManager.h"
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_STRING_FILTER_USING_DB
#include "StringFilterManager.h"
#endif //SERV_STRING_FILTER_USING_DB

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp ���� ������ �˴ϴ�!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

//{{ 2010. 7. 21  ������	�� �ý���
#ifdef SERV_PET_SYSTEM
IMPL_ON_FUNC( EGS_CREATE_PET_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CREATE_PET_REQ, EGS_CREATE_PET_ACK );

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrPetName ) == false )
	{
		KEGS_CREATE_PET_ACK kAck;
		kAck.m_iOK = NetError::ERR_STRING_FILTER_01;
		SendPacket( EGS_CREATE_PET_ACK, kAck );
		return;
	}
#endif //SERV_STRING_FILTER_USING_DB

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_CREATE_PET_ACK kAck;

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
			SendPacket( EGS_CREATE_PET_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_CREATE_PET_ACK kPacket;
	KDBE_CREATE_PET_REQ kPacketToDB;

	// 1. �� �̸� �˻�
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrPetName ) == true )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_CREATE_PET_ACK, kPacket );
		return;
	}

	// 2. ������ ���� �������� �����ϴ��� üũ!
	if( m_kInventory.IsExist( kPacket_.m_iItemUID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ITEM_04;
		SendPacket( EGS_CREATE_PET_ACK, kPacket );
		return;
	}

	// 3. ���� �����ϴ� �������� �´��� Ȯ��!
	const int iItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );

	// 4. ������ �̸��� valid���� �˻� ( �� �� �̸��̶� Ÿ ���� �� �̸��̶� ���� �� ���� )
	if( m_kUserPetManager.Handler_EGS_CREATE_PET_REQ( kPacket_, iItemID, kPacketToDB ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_CREATE_PET_ACK, kPacket );
		return;
	}

	// 5. ������ ����!
	KInventoryItemInfo kInvenotyItem;
	if( m_kInventory.DeleteItem( kPacket_.m_iItemUID, kInvenotyItem, KDeletedItemInfo::DR_PET_CREATE_ITEM ) == false )
	{
		START_LOG( cerr, L"�������� ���� �ϴ��� �˻縦 �ߴµ� ���� ���� �߳�? ���� �� ���� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< END_LOG;

		// �����صξ��� �� ������ ������!
		m_kUserPetManager.ClearReservedPet();

		kPacket.m_iOK = NetError::ERR_PET_00;
		SendPacket( EGS_CREATE_PET_ACK, kPacket );
		return;
	}

	// ������ ���
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_USE, 1 );
		//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	// 6. DB�� �� �����Ϸ� ���ô�!
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_vecUpdatedInventorySlot.push_back( kInvenotyItem );
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
	SendToGameDB( DBE_CREATE_PET_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_CREATE_PET_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_CREATE_PET_ACK kPacket;

	// ������ �ѹ� üũ
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );

	// �� ���� ��� ó��
	if( m_kUserPetManager.Handler_DBE_CREATE_PET_ACK( kPacket_ ) == false )
	{
		START_LOG( cerr, L"�� ���� ����! ������� �Դµ� ���� ���д� ���� �� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_CREATE_PET_ACK, kPacket );
		return;
	}

#ifdef SERV_FREE_AUTO_LOOTING
	if( kPacket_.m_kPetInfo.m_bFreeAutoLooting == true )
	{
		SetAutoLootingPet( kPacket_.m_kPetInfo.m_iPetUID );
	}
#endif SERV_FREE_AUTO_LOOTING

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_kCreatedPetInfo = kPacket_.m_kPetInfo;
	kPacket.m_vecKInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_CREATE_PET_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_GET_PET_LIST_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_PET_LIST_ACK );

	KEGS_GET_PET_LIST_ACK kPacket;

	// �� ����Ʈ ��û�ϱ�
	m_kUserPetManager.Handler_EGS_GET_PET_LIST_REQ( kPacket_, kPacket );

	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_GET_PET_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_SUMMON_PET_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SUMMON_PET_REQ, EGS_SUMMON_PET_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_SUMMON_PET_ACK kAck;

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
			SendPacket( EGS_SUMMON_PET_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE  &&  GetRoomUID() > 0 )
			{
				START_LOG( cerr, L"�ŷ� ���忡���� �� ��ȯ�� �� �� �����ϴ�!" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_iSummonPetUID )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_PET_05;
				SendPacket( EGS_SUMMON_PET_ACK, kAck );
				return;
			}
		}
		else if( GetStateID() == KGSFSM::S_FIELD_MAP )
		{
			if( GetRoomListID() != 0 )
			{
				START_LOG( cerr, L"���� �κ񿡼��� �� ��ȯ�� �� �� �����ϴ�!" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_iSummonPetUID )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_PET_05;
				SendPacket( EGS_SUMMON_PET_ACK, kAck );
				return;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_SUMMON_PET_ACK kPacket;
	KDBE_SUMMON_PET_REQ kPacketToDB;

	// �� ��ȯ ��û
	if( m_kUserPetManager.Handler_EGS_SUMMON_PET_REQ( GetThisPtr<KGSUser>(), kPacket_, kPacketToDB ) == false )
	{
		if( kPacket_.m_iSummonPetUID == 0 )
		{
			START_LOG( cerr, L"�� ��ȯ ���� ��û ����!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< BUILD_LOG( GetRoomListID() )
				<< BUILD_LOG( kPacket_.m_iSummonPetUID )
				<< BUILD_LOG( m_kUserPetManager.GetSummonCount() )
				<< BUILD_LOG( m_kUserPetManager.GetLastSummonedPetUID() )
				<< END_LOG;
		}
		else
		{
			START_LOG( cerr, L"�� ��ȯ ��û ����!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< BUILD_LOG( GetRoomListID() )
				<< BUILD_LOG( kPacket_.m_iSummonPetUID )
				<< BUILD_LOG( m_kUserPetManager.GetSummonCount() )
				<< BUILD_LOG( m_kUserPetManager.GetLastSummonedPetUID() )
				<< END_LOG;
		}

		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_SUMMON_PET_ACK, kPacket );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// �� �κ��丮 ó��!

	// �� �κ��丮�� ���� ���� DB�� ������Ʈ�� �κ��丮�� ���� ������ ����!
	m_kInventory.FlushEnduranceChange( kPacketToDB.m_kItemEnduranceUpdate );
	m_kInventory.FlushPositionChange( kPacketToDB.m_kItemPositionUpdate );
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );

	// �� �κ��丮�� ���� �� ������ ����� ������ ������ ����!
	std::map< UidType, KInventoryItemInfo > mapRemovedPetItem;
	m_kInventory.RemovePetInventory( mapRemovedPetItem );

	// ���������� ó���ߴ��� Ȯ��!
	if( kPacketToDB.m_iBeforeSummonPetUID == 0 )
	{
		if( mapRemovedPetItem.size() > 0 )
		{
			START_LOG( cerr, L"��ȯ�� ���� �����µ� �� �κ��丮�� �������� �־���? ���� �Ͼ���� �ȵǴ� ����!!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( mapRemovedPetItem.size() )
				<< END_LOG;
		}
	}
	else
	{
		// �ش� �꿡 �ֽ� �κ��丮 ������ ��������! -> �̰� �ܺη� �����Ű�°� ����Ģ�ϱ�.. ����� ã�ƺ���..
		if( m_kUserPetManager.UpdatePetInventory( kPacketToDB.m_iBeforeSummonPetUID, mapRemovedPetItem ) == false )
		{
			START_LOG( cerr, L"�� �κ��丮 ���� ������Ʈ ����!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacketToDB.m_iBeforeSummonPetUID )
				<< BUILD_LOG( mapRemovedPetItem.size() )
				<< END_LOG;
		}
	}	

	// �� ��ȯ �����ϸ� DB�� ��ȯ ��� ������Ʈ �Ϸ� ����!
	kPacketToDB.m_iUnitUID = GetCharUID();
	SendToGameDB( DBE_SUMMON_PET_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_SUMMON_PET_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
//#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SUMMON_PET_ACK );
//#else
//	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
//#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_SUMMON_PET_ACK kPacket;
	std::map< UidType, KInventoryItemInfo > mapPetItem;

	// �κ� ���� DB������Ʈ ��� ó��
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.RollBackInitEndurance( kPacket_.m_kItemEnduranceUpdate );
	m_kInventory.PositionUpdateComplete( kPacket_.m_kItemPositionUpdate );

	// �� ��ȯ ��� ó��!
	int iPetCategorySlotSize = 0;
	if( m_kUserPetManager.Handler_DBE_SUMMON_PET_ACK( kPacket_, kPacket.m_kSummonedPetInfo, iPetCategorySlotSize, mapPetItem ) == false )
	{
		START_LOG( cerr, L"�� ��ȯ ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< BUILD_LOG( kPacket_.m_iSummonPetUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_SUMMON_PET_ACK, kPacket );
		return;
	}

	// �� ��ȯ�ϴ� ��쿡�� �� �κ��丮 �ʱ�ȭ ����!
	if( kPacket.m_kSummonedPetInfo.m_iPetUID > 0 )
	{
		m_kInventory.InitPetInventory( kPacket.m_kSummonedPetInfo.m_iPetUID, iPetCategorySlotSize, mapPetItem, kPacket.m_vecPetInventorySlotInfo );
	}

	// �� ��ȯ�� ���õ� ��� ó���� �� ������ Ŭ���̾�Ʈ�� ��� �뺸!
	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_SUMMON_PET_ACK, kPacket );

	// �� �κ��丮�� �ʱ�ȭ �ɶ����� �Ⱓ�� ������ �˻縦 ����!
	if( kPacket.m_kSummonedPetInfo.m_iPetUID > 0 )
	{
		ExpireItem( true );
	}

	//////////////////////////////////////////////////////////////////////////
	// �� ��ȯ ����� Ŭ�󿡵� �˷��ְ� �ʵ� �Ǵ� ��ȿ� �ִ� �ֺ� �������Ե� �˸���!
	//{{ 2008. 12. 3  ������	�ʵ忡�� �Ⱓ�ٵ� ������ ����
	if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		if( GetFieldUID() <= 0 )
		{
			KEGS_SUMMON_PET_NOT kPacketNot;
			kPacketNot.m_iUnitUID = GetCharUID();
			if( kPacket_.m_iSummonPetUID != 0 )
			{
				kPacketNot.m_vecPetInfo.push_back( kPacket.m_kSummonedPetInfo );
			}
			SendPacket( ERM_SUMMON_PET_NOT, kPacketNot );
		}
		else
		{
			KERM_SUMMON_PET_NOT kPacketNot;
			kPacketNot.m_iUnitUID = GetCharUID();
			if( kPacket_.m_iSummonPetUID != 0 )
			{
				kPacketNot.m_vecPetInfo.push_back( kPacket.m_kSummonedPetInfo );
			}
			SendToGSField( ERM_SUMMON_PET_NOT, kPacketNot );
		}
	}
	//}}
	// ���� Ÿ��Ʋ�� �뿡�� �ٲ����� �� ����鿡�� �˷��ش�.
	else if( GetStateID() == KGSFSM::S_ROOM )
	{
		if( GetRoomUID() <= 0 )
		{
			START_LOG( cerr, L"�� UID �̻�." )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			return;
		}

		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			//{{ 2012. 02. 13	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			{
				KERM_SUMMON_PET_NOT kPacketNot;
				kPacketNot.m_iUnitUID = GetCharUID();
				if( kPacket_.m_iSummonPetUID != 0 )
				{
					kPacketNot.m_vecPetInfo.push_back( kPacket.m_kSummonedPetInfo );
				}
				SendToCnRoom( ERM_SUMMON_PET_NOT, kPacketNot );
			}
			break;

		case CXSLRoom::RT_SQUARE:
			// �����̸� �Ⱥ�����!
			break;

		default:
			{
				START_LOG( cerr, L"�̻��� �� Ÿ��." )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
			}
			break;
		}
	}
}

IMPL_ON_FUNC( ERM_SUMMON_PET_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_SUMMON_PET_NOT kPacket;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_vecPetInfo = kPacket_.m_vecPetInfo;
	SendPacket( EGS_SUMMON_PET_NOT, kPacket );
}

IMPL_ON_FUNC( EGS_COMMANDS_FOR_PETS_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_COMMANDS_FOR_PETS_ACK );

	// ������ ��ɿ� ���� ���� �ൿ ����
	// ������ ����� ä������ �Էµ����� enum������ �����Ͽ� ��������� ����� �����Ѵ�.
	KEGS_COMMANDS_FOR_PETS_ACK kPacket;
	CXSLPetManager::PET_ACTION_COMMAND ePetActionResult;

	if( m_kUserPetManager.Handler_EGS_COMMANDS_FOR_PETS_REQ( GetThisPtr<KGSUser>(), kPacket_, ePetActionResult ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_COMMANDS_FOR_PETS_ACK, kPacket );

		// ���� ��ȯ���� �ʾҴµ� �� ����� �Դ�. ������ ��ȯ���� ��Ű��.
		if( kPacket.m_iOK == NetError::ERR_PET_06 )
		{
			START_LOG( cout, L"���� ��ȯ���� �ʾҴµ� �� ����� �Դ�. ������ ��ȯ���� ��Ű��!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() );
#else
				<< BUILD_LOG( GetCharUID() );
#endif SERV_PRIVACY_AGREEMENT
			KEGS_SUMMON_PET_NOT kPacketNot;
			kPacketNot.m_iUnitUID = GetCharUID();
			kPacketNot.m_vecPetInfo.clear();
			SendPacket( EGS_SUMMON_PET_NOT, kPacketNot );
		}
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_COMMANDS_FOR_PETS_ACK, kPacket );

	// �� �׼��� ���� ������ �ֺ� �����鿡�� ������!
	SendPetAction( ePetActionResult );
}

IMPL_ON_FUNC( ERM_PET_ACTION_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PET_ACTION_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_DECREASE_PET_SATIETY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT kNot;

	// �� ������ ���� üũ!	

	//{{ 2012. 04. 26	�ڼ���	�� �峭��
#ifdef SERV_PET_TOY
	if( m_kUserPetManager.DecreaseSatietyOnPvpRoom( kPacket_.m_cTeamNumMember, m_kInventory.CheckPetToyItem(), kNot ) == true )
#else
	if( m_kUserPetManager.DecreaseSatietyOnPvpRoom( kPacket_.m_cTeamNumMember, kNot ) == true )
#endif SERV_PET_TOY
		//}}
	{
		kNot.m_iUnitUID = GetCharUID();
		SendToCnRoom( ERM_CHANGED_PET_SATIETY_INTIMACY_NOT, kNot );

		// �ڵ� ���� ��� üũ!
		CheckAutoFeed();

		//{{ 2010. 10. 18	������	�� ���� ��ų
#ifdef SERV_PET_AURA_SKILL
		if( m_kUserPetManager.IsSummonedPetHungry() == true )
		{
			if( GetRoomUID() <= 0 )
			{
				START_LOG( cerr, L"�� UID �̻�." )
					<< BUILD_LOG( GetRoomUID() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
				return;
			}

			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_PVP:
			case CXSLRoom::RT_DUNGEON:
			case CXSLRoom::RT_TRAININGCENTER:
				{
					KERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT kPacketNot;
					//{{ 2010. 12. 23	������	��� ���� ��Ŷ Ÿ��
#ifdef SERV_CHANGE_EQUIPPED_PACKET_REFAC
					kPacketNot.m_cChangeEquippedPacketType = KEGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT::CEPT_CHANGE_PET;
#endif SERV_CHANGE_EQUIPPED_PACKET_REFAC
					//}}
					kPacketNot.m_UnitUID = GetCharUID();
					GetRoomUserInfo( kPacketNot.m_kRoomUserInfo );
					SendToCnRoom( ERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT, kPacketNot );
				}
				break;

			case CXSLRoom::RT_SQUARE:
				break;

			default:
				{
					START_LOG( cerr, L"�̻��� �� Ÿ��." )
						<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;
				}
				break;
			}            
		}
#endif SERV_PET_AURA_SKILL
		//}}
	}
}

_IMPL_ON_FUNC( ERM_CHANGED_PET_SATIETY_NOT, KEGS_CHANGED_PET_SATIETY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGED_PET_SATIETY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGED_PET_INTIMACY_NOT, KEGS_CHANGED_PET_INTIMACY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGED_PET_INTIMACY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGED_PET_SATIETY_INTIMACY_NOT, KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGED_PET_SATIETY_INTIMACY_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_FEED_PETS_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_FEED_PETS_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_FEED_PETS_ACK kAck;

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
			SendPacket( EGS_FEED_PETS_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		// �� ���� ���� ����
		if( GetStateID() == KGSFSM::S_ROOM )
		{
			if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE  &&  GetRoomUID() > 0 )
			{
				START_LOG( cerr, L"�ŷ� ���忡���� �� ���̸� �� �� �����ϴ�!" )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_PET_05;
				SendPacket( EGS_FEED_PETS_ACK, kAck );
				return;
			}
		}
		else if( GetStateID() == KGSFSM::S_FIELD_MAP )
		{
			if( GetRoomListID() != 0 )
			{
				START_LOG( cerr, L"���� �κ񿡼��� �� ���̸� �� �� �����ϴ�!" )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_PET_05;
				SendPacket( EGS_FEED_PETS_ACK, kAck );
				return;
			}
		}

		// ���̸� �ַ��� �ϴ� �������� ������ �����ϴ� ���������� üũ!
		if( m_kInventory.IsExist( kPacket_.m_iItemUID ) == false )
		{
			START_LOG( cerr, L"�������� �ʴ� �������� ���̷� �ַ��� �ߴ�!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_ITEM_04;
			SendPacket( EGS_FEED_PETS_ACK, kAck );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_FEED_PETS_ACK kPacket;
	KEGS_CHANGED_PET_SATIETY_NOT kNotSatiety;
	KEGS_CHANGED_PET_INTIMACY_NOT kNotIntimacy;

	// �� ���� ItemID���
	const int iFeedItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );
	// �� �����ֱ� üũ!
	bool bIsFirstFeed = false;

	//{{ 2011. 05. 23  ��μ�	ť�ǿ� ���� ģ�е� ����
#ifdef SERV_ADD_KEWPIEL_JELLY
	int	iAddIntimacy = 0;
	if( m_kUserPetManager.Handler_EGS_FEED_PETS_REQ( GetLevel(), iFeedItemID, bIsFirstFeed, kNotSatiety, kNotIntimacy, iAddIntimacy ) == false )
#else
	if( m_kUserPetManager.Handler_EGS_FEED_PETS_REQ( GetLevel(), iFeedItemID, bIsFirstFeed, kNotSatiety, kNotIntimacy ) == false )
#endif SERV_ADD_KEWPIEL_JELLY
	//}}
	{
#ifdef SERV_EVENT_PET_INVENTORY
//���� ���̱Ⱑ ���� �߰ų� �̹� �Ծ��� ���̴� �ȸ����ϱ� üũ �ؼ� �˷�����
//�̺�Ʈ �� ���̰� ���� �Ѱ��� üũ�ؼ� �ٸ� �ݿ����� �߰��� ����.
#endif SERV_EVENT_PET_INVENTORY
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_FEED_PETS_ACK, kPacket );

		// ��ҷ��� �ȸԴ°��� üũ�ؼ� �׷��ٸ� �ֺ� �����鿡�� �׼��� ������!
		if( kPacket.m_iOK == NetError::ERR_PET_10 )
		{
			SendPetAction( CXSLPetManager::PAC_STUFFED );
		}
		return;
	}

	// �ش� ������ ����!
	if( m_kInventory.FeedPetInInventory( kPacket_.m_iItemUID, kPacket.m_vecInventorySlotInfo ) == false )
	{
		START_LOG( cerr, L"������ �������� �ִ��� üũ �ߴµ� ���� ������ ���� �����ߴ�? ���� �Ͼ�� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PET_11;
		SendPacket( EGS_FEED_PETS_ACK, kPacket );
		return;
	}
#ifdef SERV_EVENT_PET_INVENTORY
	///������� �Դ� �׷� ���� ���̱� �����̴�. �ٵ� �̺�Ʈ ���̴� ģ�е��� ���� ���� ���� ������ �����
	//�κ��丮�� Ȱ��ȭ ��Ű�� ���� �̱� ������ ���⼭ ���� ID�� ���ǹ� �ɰ� ���� ��Ų��.
	//��Ŷ���� ���̸� �Ծ��ٴ� ������ �����ش�. �� �� DB�� ������ ������Ʈ �Ѵ�.
	if( iFeedItemID == 141000440 )
	{
		// ��ȯ�� �� �������!
		KUserPetPtr spSummonedPet = m_kUserPetManager.GetSummonedPet();
		if( spSummonedPet == NULL )
		{
			START_LOG( cerr, L"��ȯ ���� PetUID���� �ִµ� �� ������ ����?" )
				<< BUILD_LOG( m_kUserPetManager.GetSummonedPet() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PET_11;
			SendPacket( EGS_FEED_PETS_ACK, kPacket );
			return;
		}
		//���⼭ DB�� �� ������ ����.
		KDBE_EVENT_PET_EVENT_FOOD_EAT_REQ kPacketDB;
		kPacketDB.m_bEventFoodEat = spSummonedPet->IsEventFoodEat();
		kPacketDB.m_iPetID		  = spSummonedPet->GetPetID();
		kPacketDB.m_iPetUID		  = spSummonedPet->GetPetUID();
		kPacketDB.m_wstrPetName	  = spSummonedPet->GetPetName();
		kPacketDB.m_iUnitUID      = GetCharUID();
		kPacketDB.m_vecInventorySlotInfo = kPacket.m_vecInventorySlotInfo;

		SendToGameDB( DBE_EVENT_PET_EVENT_FOOD_EAT_REQ, kPacketDB );
		return;
	}	
#endif SERV_EVENT_PET_INVENTORY
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_sSatiety = kNotSatiety.m_sCurrentSatiety;
	SendPacket( EGS_FEED_PETS_ACK, kPacket );

	//{{ 2011. 05. 23  ��μ�	ť�ǿ� ���� ģ�е� ����
#ifdef SERV_ADD_KEWPIEL_JELLY
	if( iAddIntimacy != 0 )
	{
		// ��ȯ�� �� �������!
		KUserPetPtr spSummonedPet = m_kUserPetManager.GetSummonedPet();
		if( spSummonedPet == NULL )
		{
			START_LOG( cerr, L"��ȯ ���� PetUID���� �ִµ� �� ������ ����?" )
				<< BUILD_LOG( m_kUserPetManager.GetSummonedPet() )
				<< END_LOG;
			return;
		}

		KEGS_EAT_SPECIAL_FEED_NOT kPacketNot;
		kPacketNot.m_iUnitUID = GetCharUID();
		kPacketNot.m_iPetUID = m_kUserPetManager.GetSummonedPetUID();
		kPacketNot.m_sPercent = static_cast<short>(iAddIntimacy);
		kPacketNot.m_iCurrentIntimacy = spSummonedPet->GetIntimacy();
		SendPacket( EGS_EAT_SPECIAL_FEED_NOT, kPacketNot );
	}
#endif SERV_ADD_KEWPIEL_JELLY
	//}}

	// ù���̶�� ù ���� ��Ŷ�� ������!
	if( bIsFirstFeed == true )
	{
		KEGS_FIRST_FEED_PETS_NOT kPacketNot;
		kPacketNot.m_bAutoFeed = false;
		SendPacket( EGS_FIRST_FEED_PETS_NOT, kPacketNot );
	}

	// ���� ������ �ֺ� �����鿡�� �׼� ������ ������!
	SendPetAction( CXSLPetManager::PAC_EAT );

	//////////////////////////////////////////////////////////////////////////
	// ���̶�� �������� ģ�е��� �˸���!
	if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		// ù ���� �ֱ� ��� ģ�е� ������ Ŭ���̾�Ʈ�� ����!
		if( bIsFirstFeed )
		{
			kNotIntimacy.m_iUnitUID = GetCharUID();
			SendPacket( EGS_CHANGED_PET_INTIMACY_NOT, kNotIntimacy );
		}
	}
	else if( GetStateID() == KGSFSM::S_ROOM )
	{
		if( GetRoomUID() <= 0 )
		{
			START_LOG( cerr, L"�� UID �̻�." )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			return;
		}

		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			//{{ 2012. 10. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			{
				kNotSatiety.m_iUnitUID = GetCharUID();
				SendToCnRoom( ERM_CHANGED_PET_SATIETY_NOT, kNotSatiety );

				// ù ���� �ֱ� ��� ģ�е� ������ ��ε�ĳ����!
				if( bIsFirstFeed )
				{
					kNotIntimacy.m_iUnitUID = GetCharUID();
					SendToCnRoom( ERM_CHANGED_PET_INTIMACY_NOT, kNotIntimacy );
				}
			}
			break;

		case CXSLRoom::RT_SQUARE:
			// �����̸� �Ⱥ�����!
			break;

		default:
			{
				START_LOG( cerr, L"�̻��� �� Ÿ��." )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
			}
			break;
		}
	}

	//{{ 2011. 05. 16  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iFeedItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� �������Դϴ�!" )
			<< BUILD_LOG( iFeedItemID )
			<< END_LOG;

		return;
	}
	
	m_kUserTitleManager.GivePetFeed( pItemTemplet->m_ItemGrade, GetThisPtr<KGSUser>() );

#endif SERV_ADD_TITLE_CONDITION
	//}}
}

IMPL_ON_FUNC( EGS_SET_AUTO_FEED_PETS_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SET_AUTO_FEED_PETS_ACK );

	KEGS_SET_AUTO_FEED_PETS_ACK kPacket;

	// �ڵ� ���� ����
	if( m_kUserPetManager.Handler_EGS_SET_AUTO_FEED_PETS_REQ( kPacket_ ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_SET_AUTO_FEED_PETS_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_bAutoFeed = kPacket_.m_bAutoFeed;
	SendPacket( EGS_SET_AUTO_FEED_PETS_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_PET_EVOLUTION_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_PET_EVOLUTION_ACK );

	KEGS_PET_EVOLUTION_ACK kPacket;
	int iPetCategorySlotSize = 0;
	bool bIsSummonedPet = false;

	//////////////////////////////////////////////////////////////////////////
	// �� ���� ���� ����
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE  &&  GetRoomUID() > 0 )
		{
			START_LOG( cerr, L"�ŷ� ���忡���� �� ��ȭ�� �� �� �����ϴ�!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PET_05;
			SendPacket( EGS_PET_EVOLUTION_ACK, kPacket );
			return;
		}
	}
	else if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		if( GetRoomListID() != 0 )
		{
			START_LOG( cerr, L"���� �κ񿡼��� �� ��ȭ�� �� �� �����ϴ�!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PET_05;
			SendPacket( EGS_PET_EVOLUTION_ACK, kPacket );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	// �� ��ȭ
	if( m_kUserPetManager.Handler_EGS_PET_EVOLUTION_REQ( kPacket_.m_iPetUID, kPacket, iPetCategorySlotSize, bIsSummonedPet ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_PET_EVOLUTION_ACK, kPacket );
		return;
	}

	// �� ��ȭ ����� Ŭ���̾�Ʈ���� �˷�����!
	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_PET_EVOLUTION_ACK, kPacket );

	// ��ȯ�� ���� ��ȭ�Ȱ� �ƴ϶�� ���⼭ ����!
	if( bIsSummonedPet == false )
		return;

	// �� ��ȭ ���� �ߴٸ� �κ��丮 ũ�� ��������!
	m_kInventory.ResetPetInventorySize( iPetCategorySlotSize );

#ifdef SERV_EVENT_VC
	// �뺴 �Ƿ簡 ��ȭ�ߴٸ� �������� ������ ����
	if( kPacket.m_kEvolutionResultPetInfo.m_iPetID == CXSLPetManager::PUI_PET_MERCENARY_PPORU_EVENT_INT )
	{
		// ��ü ��ȭ ����
		if( kPacket.m_kEvolutionResultPetInfo.m_cEvolutionStep == 2 )
		{
			// �̺�Ʈ ������ ����!
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			kPacketToDB.m_iRewardID	   = 51417, //��� ������ ���� ť��		
			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}
		else if( kPacket.m_kEvolutionResultPetInfo.m_cEvolutionStep == 3 )
		{
			// �̺�Ʈ ������ ����!
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			kPacketToDB.m_iRewardID	   = 51418, //�Ƹ����� �÷���� ���� ��ȯ��	
			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}
	}
#endif //SERV_EVENT_VC

	//////////////////////////////////////////////////////////////////////////
	// �ʵ��̰ų� ���̸� �˸���!
	if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		if( GetFieldUID() <= 0 )
		{
			KEGS_PET_EVOLUTION_NOT kPacketNot;
			kPacketNot.m_iUnitUID = GetCharUID();
			kPacketNot.m_kPetInfo = kPacket.m_kEvolutionResultPetInfo;
			SendPacket( EGS_PET_EVOLUTION_NOT, kPacketNot );
		}
		else
		{
			KERM_PET_EVOLUTION_NOT kPacketNot;
			kPacketNot.m_iUnitUID = GetCharUID();
			kPacketNot.m_kEvolutionPetInfo = kPacket.m_kEvolutionResultPetInfo;
			SendToGSField( ERM_PET_EVOLUTION_NOT, kPacketNot );
		}		
	}
	//}}
	// ���� Ÿ��Ʋ�� �뿡�� �ٲ����� �� ����鿡�� �˷��ش�.
	else if( GetStateID() == KGSFSM::S_ROOM )
	{
		if( GetRoomUID() <= 0 )
		{
			START_LOG( cerr, L"�� UID �̻�." )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			return;
		}

		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			//{{ 2012. 11. 08	�ڼ���	��Ʋ�ʵ� : �ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			{
				KERM_PET_EVOLUTION_NOT kPacketNot;
				kPacketNot.m_iUnitUID = GetCharUID();
				kPacketNot.m_kEvolutionPetInfo = kPacket.m_kEvolutionResultPetInfo;
				SendToCnRoom( ERM_PET_EVOLUTION_NOT, kPacketNot );
			}
			break;

		case CXSLRoom::RT_SQUARE:
			// �����̸� �Ⱥ�����!
			break;

		default:
			{
				START_LOG( cerr, L"�̻��� �� Ÿ��." )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
			}
			break;
		}
	}
}

IMPL_ON_FUNC( ERM_PET_EVOLUTION_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_PET_EVOLUTION_NOT kPacket;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_kPetInfo = kPacket_.m_kEvolutionPetInfo;
	SendPacket( EGS_PET_EVOLUTION_NOT, kPacket );
}

//{{ 2012. 02. 22	�ڼ���	�� �̸� �����
#ifdef SERV_PET_CHANGE_NAME
IMPL_ON_FUNC( EGS_CHANGE_PET_NAME_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_PET_NAME_REQ, EGS_CHANGE_PET_NAME_ACK );

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrPetName ) == false )
	{
		KEGS_CHANGE_PET_NAME_ACK kAck;
		kAck.m_iOK = NetError::ERR_STRING_FILTER_01;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kAck );
		return;
	}
#endif //SERV_STRING_FILTER_USING_DB

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_CHANGE_PET_NAME_ACK kAck;

		//{{ 2012. 03. 29	������	Inventory Lock ���
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"�κ��丮 ���� �ɷ��ִ� �����Դϴ�!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_CHANGE_PET_NAME_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_CHANGE_PET_NAME_ACK kPacket;
	KDBE_CHANGE_PET_NAME_REQ kPacketToDB;

	// 0. ��ȯ�Ǿ� �ִ� ���� �����ϴ��� �˻�
	const UidType iSummonedPetUID = m_kUserPetManager.GetSummonedPetUID();
	if( iSummonedPetUID == 0 )
	{
		kPacket.m_iOK = NetError::ERR_PET_06;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}

	// 1. �ش� �������� �����ϴ��� üũ!
	if( m_kInventory.IsExist( kPacket_.m_iItemUID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ITEM_04;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}

	const int iItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );
	// 2. ����� �������� �� �̸� ����� �������� �´°�?
	if( CXSLItem::IsPetNameChangeCashItem( iItemID ) == false )
	{
		START_LOG( cerr, L"���̸� ���� �����۵� �ƴϸ鼭 ����Ϸ��� �ߴ�!" )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PET_18;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}

	// 3-1. �� �̸� ��ȿ �˻�
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrPetName ) == true )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}

	// 3-2. �� �̸� ���� ���� üũ
	std::string strPetName = KncUtil::toNarrowString( kPacket_.m_wstrPetName );
	if( strPetName.size() > 12 )
	{
		kPacket.m_iOK = NetError::ERR_PET_02;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}

	// 3-3. �� �̸� �ߺ� üũ
	if( m_kUserPetManager.IsExistPet( kPacket_.m_wstrPetName ) == true )
	{
		kPacket.m_iOK = NetError::ERR_PET_01;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}

	// 4. DB�� ���� ��Ŷ ����
	if( m_kUserPetManager.GetPetInfo( iSummonedPetUID, kPacketToDB.m_kPetInfo ) == false )
	{
		kPacket.m_iOK = NetError::ERR_PET_19;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}
	kPacketToDB.m_iItemUID = kPacket_.m_iItemUID;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_kPetInfo.m_wstrPetName = kPacket_.m_wstrPetName;

	// 5. ������ ����!
	KInventoryItemInfo kInvenotyItem;
	if( m_kInventory.DeleteItem( kPacket_.m_iItemUID, kInvenotyItem, KDeletedItemInfo::DR_PET_NAME_CHANGE ) == false )
	{
		START_LOG( cerr, L"�������� ���� �ϴ��� �˻縦 �ߴµ� ���� ���� �߳�? ���� �� ���� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PET_19;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}
	kPacketToDB.m_vecUpdatedInventorySlot.push_back( kInvenotyItem );

	// ������ ���
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_USE, 1 );
		//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	// 6. DB�� �� �̸� �����Ϸ� ���ô�!
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );

	SendToGameDB( DBE_CHANGE_PET_NAME_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_CHANGE_PET_NAME_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_CHANGE_PET_NAME_ACK kPacket;

	// ������ �ѹ� üũ
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}

	// Ȥ�ó� �ؼ� �˻��غ�
	if( m_kUserPetManager.IsExistPet( kPacket_.m_kPetInfo.m_iPetUID ) == false )
	{
		START_LOG( cerr, L"�������� �ʴ� Pet�� �̸��� ���� �Ǿ���? ���� �� ���� ����!" )
			<< BUILD_LOG( kPacket_.m_kPetInfo.m_iPetUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			<< BUILD_LOGc( kPacket_.m_kPetInfo.m_iPetID )
#else //SERV_PETID_DATA_TYPE_CHANGE
			<< BUILD_LOGc( kPacket_.m_kPetInfo.m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
			<< BUILD_LOG( kPacket_.m_kPetInfo.m_wstrPetName )
			<< BUILD_LOGc( kPacket_.m_kPetInfo.m_cEvolutionStep )
			<< END_LOG;

		// �׳� �Ѿ� ����!
	}
	else
	{
		// �� �̸� ����!
		m_kUserPetManager.GetPet( kPacket_.m_kPetInfo.m_iPetUID )->SetPetName( kPacket_.m_kPetInfo.m_wstrPetName );
	}

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_kChangedPetInfo = kPacket_.m_kPetInfo;
	kPacket.m_vecKInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
}

#endif SERV_PET_CHANGE_NAME
//}}
#ifdef SERV_EVENT_PET_INVENTORY
IMPL_ON_FUNC( DBE_EVENT_PET_EVENT_FOOD_EAT_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	KEGS_FEED_PETS_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		SendPacket( EGS_FEED_PETS_ACK, kPacket );
	}
	else
	{
		kPacket.m_EventFoodEat = kPacket_.m_bEventFoodEat;
		kPacket.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;
		SendPacket( EGS_FEED_PETS_ACK, kPacket );
	}
}
#endif SERV_EVENT_PET_INVENTORY

#ifdef SERV_PERIOD_PET
_IMPL_ON_FUNC( DBE_RELEASE_PET_ACK, KEGS_RELEASE_PET_ACK )
{
	int iPetID = 0;
	bool bExpire = m_kUserPetManager.IsExpirePet( kPacket_.m_iPetUID, iPetID );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( m_kUserPetManager.ReleasePet( kPacket_.m_iPetUID ) == false )
		{
			// ������ ��ȯ ����
			KEGS_SUMMON_PET_REQ kPacketReq;
			KDBE_SUMMON_PET_REQ kDbePacketReq;
			kPacketReq.m_iSummonPetUID = 0;
			m_kUserPetManager.Handler_EGS_SUMMON_PET_REQ( GetThisPtr<KGSUser>(), kPacketReq, kDbePacketReq );

			// ���̵� �� ��ȯ ����� Ŭ�󿡵� �˷��ְ� �ʵ� �Ǵ� ��ȿ� �ִ� �ֺ� �������Ե� �˸���!
			if( ( GetStateID() == KGSFSM::S_FIELD_MAP ) &&
				( 0 < GetFieldUID() )
				)
			{
				KEGS_SUMMON_PET_NOT kPacketNot;
				kPacketNot.m_iUnitUID		= GetCharUID();
				SendToGSField( ERM_SUMMON_PET_NOT, kPacketNot );
			}

			if( GetStateID() == KGSFSM::S_ROOM && 0 < GetRoomUID() )
			{
				switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
				{
				case CXSLRoom::RT_PVP:
				case CXSLRoom::RT_DUNGEON:
				case CXSLRoom::RT_TRAININGCENTER:
					//{{ 2012. 02. 13	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
				case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
					//}}
					{
						KEGS_SUMMON_PET_NOT kPacketNot;
						kPacketNot.m_iUnitUID		= GetCharUID();
						SendToCnRoom( ERM_SUMMON_PET_NOT, kPacketNot );
					}
					break;
				}
			}

			// �𼭸� �� �ٽ� ���� �õ�
			m_kUserPetManager.ReleasePet( kPacket_.m_iPetUID );
		}

		//if( bExpire )
		{
			// ������ ����� ���� Release ó�� �Ϸ�
			KEGS_RELEASE_PET_NOT kPacket;
			kPacket.m_iPetUID	= kPacket_.m_iPetUID;
			kPacket.m_iUnitUID	= GetCharUID();
			kPacket.m_iPetID	= iPetID;
			SendPacket( EGS_RELEASE_PET_NOT, kPacket );
		}
		//else
		//{
		//	// �����ֱ�� ���� Release ó�� �Ϸ�
		//	SendPacket( EGS_RELEASE_RIDING_PET_ACK, kPacket_ );
		//}
	}
	else
	{
		//if( bExpire == false )
		//{
		//	// �����ֱ�� ���� Release ó�� ����
		//	SendPacket( EGS_RELEASE_RIDING_PET_ACK, kPacket_ );
		//}
	}
}
#endif SERV_PERIOD_PET

#endif SERV_PET_SYSTEM
//}}

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-20
IMPL_ON_FUNC( EGS_GET_RIDING_PET_LIST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_GET_RIDING_PET_LIST_ACK kPacket;
	kPacket.m_iOK = m_kUserRidingPetManager.GetUserRidingPetList( kPacket_.m_byteViewPage, kPacket.m_vecRidingPetList );
	
	switch( kPacket.m_iOK )
	{
	case KUserRidingPetManager::RIDING_PET_SUCCEED:
		kPacket.m_iOK = NetError::NET_OK;
		SendPacket( EGS_GET_RIDING_PET_LIST_ACK, kPacket );
		break;

	case KUserRidingPetManager::RIDING_PET_NOT_INITED:
		{
			KDBE_GET_RIDING_PET_LIST_REQ KPacketToDB;
			if( m_kAfterWorkStorage.InsertData( PI_GS_USER, GetUID(), NULL, EGS_GET_RIDING_PET_LIST_REQ, kPacket_, KPacketToDB.m_iAfterWorkStorageKey ) != KAfterWorkStorage::AWS_SUCCEED )
			{
				START_LOG( cerr, L"Riding Pet System: Key���� ���� �� �����ϴ�." )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_UNKNOWN;
				SendPacket( EGS_GET_RIDING_PET_LIST_ACK, kPacket );
			}
			else
			{
				KPacketToDB.m_iUnitUID = GetCharUID();
				SendToGameDB( DBE_GET_RIDING_PET_LIST_REQ, KPacketToDB );
			}
		}
		break;

	default:
		START_LOG( cerr, L"Riding Pet System: Unhandled Result" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_GET_RIDING_PET_LIST_ACK, kPacket );
		break;
	};
}

IMPL_ON_FUNC( EGS_SUMMON_RIDING_PET_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_SUMMON_RIDING_PET_ACK kPacket;

	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			//{{ 2012. 02. 13	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			break;

		default:
			if( kPacket_.m_bNeedAck == true )
			{
				kPacket.m_iOK = NetError::ERR_RIDING_PET_12;	// ���̵� ���� ��ȯ�� �� ���� �����Դϴ�.
				SendPacket( EGS_SUMMON_RIDING_PET_ACK, kPacket );
				return;
			}
			break;
		}
	}

	kPacket.m_iOK = m_kUserRidingPetManager.SummonRidingPet( kPacket_.m_iRidingPetUID, kPacket.m_kRidingPetInfo );

	switch( kPacket.m_iOK )
	{
	case KUserRidingPetManager::RIDING_PET_SUCCEED:
	case KUserRidingPetManager::RIDING_PET_ALREADY_SUMMONED:
		kPacket.m_iOK = NetError::NET_OK;
		break;

	case KUserRidingPetManager::RIDING_PET_NOT_INITED:
		{
			KDBE_GET_RIDING_PET_LIST_REQ KPacketToDB;
			if( m_kAfterWorkStorage.InsertData( PI_GS_USER, GetUID(), NULL, EGS_SUMMON_RIDING_PET_REQ, kPacket_, KPacketToDB.m_iAfterWorkStorageKey ) != KAfterWorkStorage::AWS_SUCCEED )
			{
				START_LOG( cerr, L"Riding Pet System: Key���� ���� �� �����ϴ�." )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_UNKNOWN;
			}
			else
			{
				KPacketToDB.m_iUnitUID = GetCharUID();
				SendToGameDB( DBE_GET_RIDING_PET_LIST_REQ, KPacketToDB );
				return;
			}
		}
		break;

	case KUserRidingPetManager::RIDING_PET_NOT_EXIST:
		kPacket.m_iOK = NetError::ERR_RIDING_PET_06;	// �������� �ʴ� ���̵� ���Դϴ�.
		break;

	case KUserRidingPetManager::RIDING_PET_SOMETHING_SUMMONED:
		kPacket.m_iOK = NetError::ERR_RIDING_PET_07;	// �̹� �ٸ� ���̵� ���� ��ȯ�Ǿ� �ֽ��ϴ�.
		break;

	case KUserRidingPetManager::RIDING_PET_EMPTY:
		kPacket.m_iOK = NetError::ERR_RIDING_PET_13;	// ���� ���� ���̵� ���� �����ϴ�.
		break;

	default:
		START_LOG( cerr, L"Riding Pet System: Unhandled Result" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		break;
	}

	if( kPacket_.m_bNeedAck == true )
	{
		SendPacket( EGS_SUMMON_RIDING_PET_ACK, kPacket );
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		return;
	}

	// ���̵� �� ��ȯ ����� Ŭ�󿡵� �˷��ְ� �ʵ� �Ǵ� ��ȿ� �ִ� �ֺ� �������Ե� �˸���!
	if( ( GetStateID() == KGSFSM::S_FIELD_MAP ) &&
		( 0 < GetFieldUID() )
		)
	{
		KEGS_SUMMON_RIDING_PET_NOT kPacketNot;
		kPacketNot.m_iUnitUID		= GetCharUID();
		kPacketNot.m_iRidingPetUID	= kPacket.m_kRidingPetInfo.m_iRidingPetUID;
		kPacketNot.m_usRidingPetID	= kPacket.m_kRidingPetInfo.m_usRindingPetID;
		SendToGSField( ERM_SUMMON_RIDING_PET_NOT, kPacketNot );
	}
	
	if( ( GetStateID() == KGSFSM::S_ROOM ) &&
		( 0 < GetRoomUID() )
		)
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			//{{ 2012. 02. 13	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			{
				KEGS_SUMMON_RIDING_PET_NOT kPacketNot;
				kPacketNot.m_iUnitUID		= GetCharUID();
				kPacketNot.m_iRidingPetUID	= kPacket.m_kRidingPetInfo.m_iRidingPetUID;
				kPacketNot.m_usRidingPetID	= kPacket.m_kRidingPetInfo.m_usRindingPetID;
				SendToCnRoom( ERM_SUMMON_RIDING_PET_NOT, kPacketNot );
			}
			break;
		}
	}
}

IMPL_ON_FUNC( EGS_UNSUMMON_RIDING_PET_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_UNSUMMON_RIDING_PET_ACK kPacket;
	kPacket.m_iOK = m_kUserRidingPetManager.UnSummonRidingPet( kPacket_.m_iRidingPetUID, kPacket_.m_fStamina );

	switch( kPacket.m_iOK )
	{
	case KUserRidingPetManager::RIDING_PET_SUCCEED:
		kPacket.m_iOK			= NetError::NET_OK;
		kPacket.m_iRidingPetUID	= kPacket_.m_iRidingPetUID;
		SendPacket( EGS_UNSUMMON_RIDING_PET_ACK, kPacket );
		break;

	case KUserRidingPetManager::RIDING_PET_NOT_INITED:
		{
			KDBE_GET_RIDING_PET_LIST_REQ KPacketToDB;
			if( m_kAfterWorkStorage.InsertData( PI_GS_USER, GetUID(), NULL, EGS_UNSUMMON_RIDING_PET_REQ, kPacket_, KPacketToDB.m_iAfterWorkStorageKey ) != KAfterWorkStorage::AWS_SUCCEED )
			{
				START_LOG( cerr, L"Riding Pet System: Key���� ���� �� �����ϴ�." )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_UNKNOWN;
				SendPacket( EGS_UNSUMMON_RIDING_PET_ACK, kPacket );
			}
			else
			{
				KPacketToDB.m_iUnitUID = GetCharUID();
				SendToGameDB( DBE_GET_RIDING_PET_LIST_REQ, KPacketToDB );
			}
		}
		break;

	case KUserRidingPetManager::RIDING_PET_ALREADY_UNSUMMONED:
		kPacket.m_iOK = NetError::ERR_RIDING_PET_08;	// ��ȯ�Ǿ� �ִ� ���̵� ���� �����ϴ�.
		SendPacket( EGS_SUMMON_RIDING_PET_ACK, kPacket );
		break;

	case KUserRidingPetManager::RIDING_PET_NOT_SUMMONED:
		kPacket.m_iOK = NetError::ERR_RIDING_PET_09;	// �ش� ���� ��ȯ�Ǿ� ���� �ʽ��ϴ�.
		SendPacket( EGS_SUMMON_RIDING_PET_ACK, kPacket );
		break;

	default:
		START_LOG( cerr, L"Riding Pet System: Unhandled Result" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_UNSUMMON_RIDING_PET_ACK, kPacket );
		break;
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		return;
	}

	// ���̵� �� ��ȯ ����� Ŭ�󿡵� �˷��ְ� �ʵ� �Ǵ� ��ȿ� �ִ� �ֺ� �������Ե� �˸���!
	if( ( GetStateID() == KGSFSM::S_FIELD_MAP ) &&
		( 0 < GetFieldUID() )
		)
	{
		KEGS_UNSUMMON_RIDING_PET_NOT kPacketNot;
		kPacketNot.m_iUnitUID		= GetCharUID();
		SendToGSField( ERM_UNSUMMON_RIDING_PET_NOT, kPacketNot );
	}

	if( ( GetStateID() == KGSFSM::S_ROOM ) &&
		( 0 < GetRoomUID() )
		)
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			//{{ 2012. 02. 13	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			{
				KEGS_UNSUMMON_RIDING_PET_NOT kPacketNot;
				kPacketNot.m_iUnitUID		= GetCharUID();
				SendToCnRoom( ERM_UNSUMMON_RIDING_PET_NOT, kPacketNot );
			}
			break;
		}
	}
}

IMPL_ON_FUNC( EGS_CREATE_RIDING_PET_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	int iOK = m_kUserRidingPetManager.RidingPetCreateCheck();

	switch( iOK )
	{
	case KUserRidingPetManager::RIDING_PET_SUCCEED:
		{
			//////////////////////////////////////////////////////////////////////////
			// ����ó��
			{
				KEGS_CREATE_RIDING_PET_ACK kAck;

				//{{ 2012. 03. 29	������	Inventory Lock ���
#ifdef SERV_INVENTORY_LOCK
				if( m_kInventory.IsLocked() == true )
				{
					START_LOG( cout, L"�κ��丮 ���� �ɷ��ִ� �����Դϴ�!" )
						<< BUILD_LOG( GetUID() )
						<< BUILD_LOG( GetName() )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( GetCharName() );

					kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
					SendPacket( EGS_CREATE_RIDING_PET_ACK, kAck );
					return;
				}
#endif SERV_INVENTORY_LOCK
				//}}
			}
			//////////////////////////////////////////////////////////////////////////

			// �ش� �������� �����ϴ��� üũ!
			if( m_kInventory.IsExist( kPacket_.m_iItemUID ) == false )
			{
				KEGS_CREATE_RIDING_PET_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_ITEM_04;
				SendPacket( EGS_CREATE_RIDING_PET_ACK, kPacket );
				return;
			}

			// ���̵� ���� �����ϴ� �������� �´��� Ȯ��!
			KDBE_CREATE_RIDING_PET_REQ kPacketToDB;
			const int iItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );
			SiCXSLRidingPetManager()->GetRidingPetID( iItemID, kPacketToDB.m_usCreatePetID, kPacketToDB.m_sPeriod );

			if( kPacketToDB.m_usCreatePetID == CXSLPetManager::RPI_NONE )
			{
				KEGS_CREATE_RIDING_PET_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_RIDING_PET_01;
				SendPacket( EGS_CREATE_RIDING_PET_ACK, kPacket );
				return;
			}

			// ������ ����!
			KInventoryItemInfo kInvenotyItem;
			if( m_kInventory.DeleteItem( kPacket_.m_iItemUID, kInvenotyItem, KDeletedItemInfo::DR_RIDING_PET_CREATE_ITEM ) == false )
			{
				START_LOG( cerr, L"�������� ���� �ϴ��� �˻縦 �ߴµ� ���� ���� �߳�? ���� �� ���� ����!" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_iItemUID )
					<< END_LOG;

				KEGS_CREATE_RIDING_PET_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_RIDING_PET_02;
				SendPacket( EGS_CREATE_RIDING_PET_ACK, kPacket );
				return;
			}

			// ������ ���
			{
				KStatisticsKey kKey;
				kKey.m_vecIntKey.push_back( iItemID );
				KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_USE, 1 );
				//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
				KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
				//}}
			}

			// DB�� ���̵� �� �����Ϸ� ���ô�!
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_vecUpdatedInventorySlot.push_back( kInvenotyItem );
			m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
			m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
			SendToGameDB( DBE_CREATE_RIDING_PET_REQ, kPacketToDB );
		}
		break;

	case KUserRidingPetManager::RIDING_PET_NOT_INITED:
		{
			KDBE_GET_RIDING_PET_LIST_REQ KPacketToDB;
			if( m_kAfterWorkStorage.InsertData( PI_GS_USER, GetUID(), NULL, EGS_CREATE_RIDING_PET_REQ, kPacket_, KPacketToDB.m_iAfterWorkStorageKey ) != KAfterWorkStorage::AWS_SUCCEED )
			{
				START_LOG( cerr, L"Riding Pet System: Key���� ���� �� �����ϴ�." )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				KEGS_CREATE_RIDING_PET_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_UNKNOWN;
				SendPacket( EGS_CREATE_RIDING_PET_ACK, kPacket );
			}
			else
			{
				KPacketToDB.m_iUnitUID = GetCharUID();
				SendToGameDB( DBE_GET_RIDING_PET_LIST_REQ, KPacketToDB );
			}
		}
		break;

	case KUserRidingPetManager::RIDING_PET_CREATE_COUNT_LIMITED:
		{
			KEGS_CREATE_RIDING_PET_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_RIDING_PET_03;
			SendPacket( EGS_CREATE_RIDING_PET_ACK, kPacket );
		}
		break;

	default:
		{
			START_LOG( cerr, L"Riding Pet System: Unhandled Result" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			KEGS_CREATE_RIDING_PET_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			SendPacket( EGS_CREATE_RIDING_PET_ACK, kPacket );
		}
		break;
	};
}

IMPL_ON_FUNC( DBE_GET_RIDING_PET_LIST_ACK )
{
	bool bSuccess = false;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( m_kUserRidingPetManager.Init( kPacket_.m_vecRidingPetList ) != KUserRidingPetManager::RIDING_PET_SUCCEED )
		{
			START_LOG( cerr, L"Riding Pet System: ���� �ʱ�ȭ ����" )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;
		}
		else
		{
			CheckRidingPetProcess();
			bSuccess = true;
		}
	}

	if( bSuccess == true )
	{
		if( kPacket_.m_iAfterWorkStorageKey != 0 )
		{
			KEventPtr spEvent = KEventPtr();
			if( ( m_kAfterWorkStorage.RetrieveData( kPacket_.m_iAfterWorkStorageKey, spEvent ) != KAfterWorkStorage::AWS_SUCCEED )
				|| ( spEvent == NULL )
				)
			{
				START_LOG( cerr, L"After Work Storage: �������� ����" )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
				return;
			}

			QueueingEvent( spEvent );
		}
	}
	else
	{
		if( kPacket_.m_iAfterWorkStorageKey == 0 )
		{
			KEGS_GET_RIDING_PET_LIST_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_RIDING_PET_04;
			SendPacket( EGS_GET_RIDING_PET_LIST_ACK, kPacket );
		}
		else
		{
			KEventPtr spEvent = KEventPtr();
			if( ( m_kAfterWorkStorage.RetrieveData( kPacket_.m_iAfterWorkStorageKey, spEvent ) != KAfterWorkStorage::AWS_SUCCEED )
				|| ( spEvent == NULL )
				)
			{
				START_LOG( cerr, L"After Work Storage: �������� ����" )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
				return;
			}
		}
	}
}

IMPL_ON_FUNC( DBE_CREATE_RIDING_PET_ACK )
{
	KEGS_CREATE_RIDING_PET_ACK kPacket;
	kPacket.m_iOK					= NetError::NET_OK;
	kPacket.m_kCreatedPetInfo		= kPacket_.m_kCreatedPetInfo;
	kPacket.m_vecKInventorySlotInfo	= kPacket_.m_vecUpdatedInventorySlot;

	// ������ �ѹ� üũ
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );

	// �� ���� ��� ó��
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		const int iOK = m_kUserRidingPetManager.AddRidingPet( kPacket_.m_kCreatedPetInfo, true );
		if( iOK != KUserRidingPetManager::RIDING_PET_SUCCEED )
		{
			START_LOG( cerr, L"Riding Pet System: �ʱ�ȭ ���� ���� KUserRidingPetManager�� ������ ���̵� �� ������ �����Ϸ� �Ͽ����ϴ�." )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_UNKNOWN;
		}

		KUserRidingPetPtr spInfo = KUserRidingPetPtr();
		if( m_kUserRidingPetManager.GetRidingPet( kPacket.m_kCreatedPetInfo.m_iRidingPetUID, spInfo ) == KUserRidingPetManager::RIDING_PET_SUCCEED )
		{
			spInfo->GetData( kPacket.m_kCreatedPetInfo );
		}
	}

	SendPacket( EGS_CREATE_RIDING_PET_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_RELEASE_RIDING_PET_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	const int iOK = m_kUserRidingPetManager.ReleaseRidingPetCheck( kPacket_.m_iRidingPetUID );

	switch( iOK )
	{
	case KUserRidingPetManager::RIDING_PET_SUCCEED:
		{
			KDBE_RELEASE_RIDING_PET_REQ kPacketToDB;
			kPacketToDB.m_iUnitUID		= GetCharUID();
			kPacketToDB.m_iRidingPetUID	= kPacket_.m_iRidingPetUID;
#ifdef SERV_EVENT_RIDING_WITH_SUB_QUEST
			// �̺�Ʈ�� ���̰�, ����Ʈ ������
			if(kPacket_.m_bCEventQuest == true) 
			{
				kPacketToDB.m_bEventQuest = true;
			}
			else
			{
				kPacketToDB.m_bEventQuest = false;
			}
#endif //SERV_EVENT_RIDING_WITH_SUB_QUEST

			SendToGameDB( DBE_RELEASE_RIDING_PET_REQ, kPacketToDB );
		}
		break;

	case KUserRidingPetManager::RIDING_PET_NOT_INITED:
		{
			KDBE_GET_RIDING_PET_LIST_REQ KPacketToDB;
			if( m_kAfterWorkStorage.InsertData( PI_GS_USER, GetUID(), NULL, EGS_RELEASE_RIDING_PET_REQ, kPacket_, KPacketToDB.m_iAfterWorkStorageKey ) != KAfterWorkStorage::AWS_SUCCEED )
			{
				START_LOG( cerr, L"Riding Pet System: Key���� ���� �� �����ϴ�." )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				KEGS_RELEASE_RIDING_PET_ACK kPacket;
				kPacket.m_iOK			= NetError::ERR_UNKNOWN;
				SendPacket( EGS_RELEASE_RIDING_PET_ACK, kPacket );
			}
			else
			{
				KPacketToDB.m_iUnitUID = GetCharUID();
				SendToGameDB( DBE_GET_RIDING_PET_LIST_REQ, KPacketToDB );
			}
		}
		break;

	case KUserRidingPetManager::RIDING_PET_ALREADY_SUMMONED:
		{
			KEGS_RELEASE_RIDING_PET_ACK kPacket;
			kPacket.m_iOK			= NetError::ERR_RIDING_PET_10;	// ��ȯ�Ǿ� �ִ� ���̵� ���� ������ �� �����ϴ�.
			SendPacket( EGS_RELEASE_RIDING_PET_ACK, kPacket );
		}
		break;

	case KUserRidingPetManager::RIDING_PET_NOT_EXIST:
		{
			KEGS_RELEASE_RIDING_PET_ACK kPacket;
			kPacket.m_iOK			= NetError::ERR_RIDING_PET_06;	// �������� �ʴ� ���̵� ���Դϴ�.
			SendPacket( EGS_RELEASE_RIDING_PET_ACK, kPacket );
		}
		break;

	default:
		{
			START_LOG( cerr, L"Riding Pet System: Unhandled Result" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			KEGS_RELEASE_RIDING_PET_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			SendPacket( EGS_RELEASE_RIDING_PET_ACK, kPacket );
		}
		break;
	};
}

_IMPL_ON_FUNC( DBE_RELEASE_RIDING_PET_ACK, KEGS_RELEASE_RIDING_PET_ACK )
{
	USHORT		usRidingPetID	= 0;
	const int	iResult			= m_kUserRidingPetManager.IsExpirationRidingPet( kPacket_.m_iRidingPetUID, usRidingPetID );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		const int iOK = m_kUserRidingPetManager.ReleaseRidingPet( kPacket_.m_iRidingPetUID );
		switch( iOK )
		{
		case KUserRidingPetManager::RIDING_PET_SUCCEED:
			break;

		case KUserRidingPetManager::RIDING_PET_ALREADY_SUMMONED:
			{
				// ������ ��ȯ ����
				m_kUserRidingPetManager.UnSummonRidingPet( kPacket_.m_iRidingPetUID, 0 );

				// ���̵� �� ��ȯ ����� Ŭ�󿡵� �˷��ְ� �ʵ� �Ǵ� ��ȿ� �ִ� �ֺ� �������Ե� �˸���!
				if( ( GetStateID() == KGSFSM::S_FIELD_MAP ) &&
					( 0 < GetFieldUID() )
					)
				{
					KEGS_UNSUMMON_RIDING_PET_NOT kPacketNot;
					kPacketNot.m_iUnitUID		= GetCharUID();
					SendToGSField( ERM_UNSUMMON_RIDING_PET_NOT, kPacketNot );
				}

				if( ( GetStateID() == KGSFSM::S_ROOM ) &&
					( 0 < GetRoomUID() )
					)
				{
					switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
					{
					case CXSLRoom::RT_PVP:
					case CXSLRoom::RT_DUNGEON:
					case CXSLRoom::RT_TRAININGCENTER:
						//{{ 2012. 02. 13	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
					case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
						//}}
						{
							KEGS_UNSUMMON_RIDING_PET_NOT kPacketNot;
							kPacketNot.m_iUnitUID		= GetCharUID();
							SendToCnRoom( ERM_UNSUMMON_RIDING_PET_NOT, kPacketNot );
						}
						break;
					}
				}

				// �ٽ� ���� �õ�
				m_kUserRidingPetManager.ReleaseRidingPet( kPacket_.m_iRidingPetUID );
			}
			break;

		default:
			START_LOG( cerr, L"Riding Pet System: Unhandled Result" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( usRidingPetID )
				<< BUILD_LOG( kPacket_.m_iRidingPetUID )
				<< END_LOG;
			break;
		}

		if( iResult == KUserRidingPetManager::RIDING_PET_SUCCEED )
		{
			// ������ ����� ���� Release ó�� �Ϸ�
			KEGS_RELEASE_RIDING_PET_NOT kPacket;
			kPacket.m_iRidingPetUID		= kPacket_.m_iRidingPetUID;
			kPacket.m_usRindingPetID	= usRidingPetID;
			SendPacket( EGS_RELEASE_RIDING_PET_NOT, kPacket );
		}
		else
		{
			// �����ֱ�� ���� Release ó�� �Ϸ�
			SendPacket( EGS_RELEASE_RIDING_PET_ACK, kPacket_ );
		}
	}
	else
	{
		if( iResult != KUserRidingPetManager::RIDING_PET_SUCCEED )
		{
			// �����ֱ�� ���� Release ó�� ����
			SendPacket( EGS_RELEASE_RIDING_PET_ACK, kPacket_ );
		}
	}
}

_IMPL_ON_FUNC( ERM_SUMMON_RIDING_PET_NOT, KEGS_SUMMON_RIDING_PET_NOT )
{
	SendPacket( EGS_SUMMON_RIDING_PET_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_UNSUMMON_RIDING_PET_NOT, KEGS_UNSUMMON_RIDING_PET_NOT )
{
	SendPacket( EGS_UNSUMMON_RIDING_PET_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_UPDATE_RIDING_PET_NOT )
{
	const int iOK = m_kUserRidingPetManager.UpdateSummonedRidingPetInfo( kPacket_.m_iRidingPetUID, kPacket_.m_fStamina );

	if( iOK != KUserRidingPetManager::RIDING_PET_SUCCEED )
	{
		START_LOG( cwarn, L"Riding Pet System: ���̵� �� ���׹̳� �� ���� ����" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iRidingPetUID )
			<< BUILD_LOG( kPacket_.m_fStamina )
			<< END_LOG;
	}
}
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_EVENT_VC
IMPL_ON_FUNC( EGS_USE_INTIMACY_UP_ITEM_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_USE_INTIMACY_UP_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_USE_INTIMACY_UP_ITEM_ACK kAck;

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
			SendPacket( EGS_USE_INTIMACY_UP_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		// �� ���� ���� ����
		if( GetStateID() == KGSFSM::S_ROOM )
		{
			if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE  &&  GetRoomUID() > 0 )
			{
				START_LOG( cerr, L"�ŷ� ���忡���� ģ�� ��¾������� �� �� �����ϴ�!" )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_PET_05;
				SendPacket( EGS_USE_INTIMACY_UP_ITEM_ACK, kAck );
				return;
			}
		}
		else if( GetStateID() == KGSFSM::S_FIELD_MAP )
		{
			if( GetRoomListID() != 0 )
			{
				START_LOG( cerr, L"���� �κ񿡼��� ģ�� ��¾������� �� �� �����ϴ�!" )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_PET_05;
				SendPacket( EGS_USE_INTIMACY_UP_ITEM_ACK, kAck );
				return;
			}
		}

		// ���̸� �ַ��� �ϴ� �������� ������ �����ϴ� ���������� üũ!
		if( m_kInventory.IsExist( kPacket_.m_iItemUID ) == false )
		{
			START_LOG( cerr, L"�������� �ʴ� �������� ģ�� ��¾��������� �ַ��� �ߴ�!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_ITEM_04;
			SendPacket( EGS_USE_INTIMACY_UP_ITEM_ACK, kAck );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_USE_INTIMACY_UP_ITEM_ACK kPacket;
	KEGS_CHANGED_PET_INTIMACY_NOT kNotIntimacy;

	// �� ���� ItemID���
	const int iFeedItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );

	if( m_kUserPetManager.Handler_EGS_USE_INTIMACY_UP_ITEM_REQ( kNotIntimacy, kPacket.m_iUpPercent ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_USE_INTIMACY_UP_ITEM_ACK, kPacket );

		return;
	}

	// �ش� ������ ����!
	if( m_kInventory.FeedPetInInventory( kPacket_.m_iItemUID, kPacket.m_vecInventorySlotInfo ) == false )
	{
		START_LOG( cerr, L"������ �������� �ִ��� üũ �ߴµ� ���� ������ ���� �����ߴ�? ���� �Ͼ�� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PET_11;
		SendPacket( EGS_USE_INTIMACY_UP_ITEM_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_USE_INTIMACY_UP_ITEM_ACK, kPacket );

	// ���� ������ �ֺ� �����鿡�� �׼� ������ ������!
	SendPetAction( CXSLPetManager::PAC_EAT );

	//////////////////////////////////////////////////////////////////////////
	// ���̶�� �������� ģ�е��� �˸���!
	if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		kNotIntimacy.m_iUnitUID = GetCharUID();
		SendPacket( EGS_CHANGED_PET_INTIMACY_NOT, kNotIntimacy );

	}
	else if( GetStateID() == KGSFSM::S_ROOM )
	{
		if( GetRoomUID() <= 0 )
		{
			START_LOG( cerr, L"�� UID �̻�." )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			return;
		}

		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			//{{ 2012. 10. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			{
				kNotIntimacy.m_iUnitUID = GetCharUID();
				SendToCnRoom( ERM_CHANGED_PET_INTIMACY_NOT, kNotIntimacy );
			}
			break;

		case CXSLRoom::RT_SQUARE:
			// �����̸� �Ⱥ�����!
			break;

		default:
			{
				START_LOG( cerr, L"�̻��� �� Ÿ��." )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
			}
			break;
		}
	}
}
#endif //SERV_EVENT_VC
//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////
