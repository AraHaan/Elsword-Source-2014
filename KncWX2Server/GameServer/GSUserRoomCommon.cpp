#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
#include "TrainingCenterTable.h"
#include "GameSysVal/GameSysVal.h"
//{{ 2008. 11. 24  ������	�� ����Ʈ
#include "RoomListManager.h"
//}}

//{{ 2011. 12. 05	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewPartyListManager.h"
#else
	#include "PartyListManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 05. 2	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	#include "GSWorldMissionManager.h"
	#include "../Common/X2Data/XSLBuffManager.h"
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}
//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	#include "X2Data/XSLEnchantItemManager.h"
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}

#ifdef SERV_GLOBAL_MISSION_MANAGER
#include "GSGlobalMissionManager.h"
#endif SERV_GLOBAL_MISSION_MANAGER

#include "Enum/Enum.h"
#include "GSWorldMissionManager.h"

#ifdef SERV_STRING_FILTER_USING_DB
#include "StringFilterManager.h"
#endif //SERV_STRING_FILTER_USING_DB

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp ���� ������ �˴ϴ�!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC( EGS_CREATE_ROOM_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
//#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CREATE_ROOM_REQ, EGS_CREATE_ROOM_ACK );
//#else
//	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
//#endif SERV_REPEAT_FILTER_REFAC
	//}}    

#ifdef SERV_STRING_FILTER_USING_DB
	kPacket_.m_RoomInfo.m_RoomName = SiKStringFilterManager()->FilteringChatString( kPacket_.m_RoomInfo.m_RoomName.c_str(), L'��' );
#endif //SERV_STRING_FILTER_USING_DB

	KEGS_CREATE_ROOM_ACK kPacketAck;
	u_short usEventID;

	KERM_OPEN_ROOM_REQ kPacket;

	kPacket_.m_RoomInfo.m_uiRoomListID = GetRoomListID();

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		if( GetLevel() > SiKGameSysVal()->GetGuestUserLimitLevel() )
		{
			START_LOG( cerr, L"Ŭ���̾�Ʈ���� ü��ID�� ����20�Ǿ����� ������ ĳ���������� �̵��������ٵ� ���� ������ �ϰ��ֳ�?" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_GUEST_USER_02;
			SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
			return;
		}
	}

	// [����] Ư�������� ���(��:�ʽ��ڽ�) ��Ƽ�� ������ �ʾƵ� ������ �� �ְ� ������ �ұ�?
	if( CXSLDungeon::IsSinglePlayDungeon( kPacket_.m_RoomInfo.m_iDungeonID ) )
	{
		START_LOG( cerr, L"Ʃ�丮�� ������ �� ��Ŷ���� �����Ϸ� �ϸ�ȵǴµ�?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_RoomInfo.m_iDungeonID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_ROOM_30;
		SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
		return;
	}

	switch( kPacket_.m_RoomInfo.m_RoomType )
	{
	case CXSLRoom::RT_PVP:
		{
			usEventID = ERM_OPEN_PVP_ROOM_REQ;

			//{{ 2010. 12. 7	������	�ٸ� ä�� üũ
#ifdef SERV_WRONG_PVP_CHANNEL_JOIN_FIX
			if( GetPVPChannelClass( kPacket_.m_RoomInfo.m_iPVPChannelClass ) == false )
			{
				kPacketAck.m_iOK = NetError::ERR_ROOM_30;
				SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
				return;
			}
#else
			LIF( GetPVPChannelClass( kPacket_.m_RoomInfo.m_iPVPChannelClass ) );
#endif SERV_WRONG_PVP_CHANNEL_JOIN_FIX
			//}}

			//{{ 2010. 7. 29  ������	���� �ڵ� ����
			if( kPacket_.m_RoomInfo.m_iPVPChannelClass == KPVPChannelInfo::PCC_NONE )
			{
				START_LOG( cerr, L"���� ä�� ������ �̻��մϴ�! ���� �� ���� ����!" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetRoomListID() )
					<< BUILD_LOG( GetRoomUID() )
					<< BUILD_LOG( GetStateIDString() )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_ROOM_30;
				SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
				return;
			}
			//}}

			//{{ 2009. 10. 22  ������	����ų�� �˻� - ����ä���� ų���� ���� �������ᰡ �����Ƿ� ����
			if( kPacket_.m_RoomInfo.m_iPVPChannelClass != KPVPChannelInfo::PCC_PLAY )
			{
				if( CXSLRoom::IsValidWinKillNum( kPacket_.m_RoomInfo.m_PVPGameType, kPacket_.m_RoomInfo.m_WinKillNum ) == false )
				{
					START_LOG( cerr, L"������ ������ ų�������� �̻���." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOGc( kPacket_.m_RoomInfo.m_PVPGameType )
						<< BUILD_LOGc( kPacket_.m_RoomInfo.m_WinKillNum )
						<< END_LOG;

					kPacketAck.m_iOK = NetError::ERR_ROOM_47;
					SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
					return;
				}

				// �÷��� �ð� �˻�
				if( CXSLRoom::IsValidPvpPlayTime( kPacket_.m_RoomInfo.m_PVPGameType, static_cast<int>(kPacket_.m_RoomInfo.m_fPlayTime) ) == false )
				{
					START_LOG( cerr, L"������ ������ �÷��� Ÿ�� ������ �̻���." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOGc( kPacket_.m_RoomInfo.m_PVPGameType )
						<< BUILD_LOG( kPacket_.m_RoomInfo.m_fPlayTime )
						<< END_LOG;

					kPacketAck.m_iOK = NetError::ERR_ROOM_30;
					SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
					return;
				}
			}
			//}}

			//////////////////////////////////////////////////////////////////////////
			// [�̺�Ʈ] (2010-07-29 ~ 2010-08-11) ��ȸä�� ���� ����
#ifdef SERV_TOURNAMENT_CONDITION
			if( kPacket_.m_RoomInfo.m_iPVPChannelClass == KPVPChannelInfo::PCC_TOURNAMENT )
			{				
#ifdef SERV_MAIN_TOURNAMENT
				// �������� Ƽ�� ������ ��ȸä�� ����.
				if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == false )
				{
					kPacketAck.m_iOK = NetError::ERR_ROOM_50;
					SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
					return;
				}
#else
				// �������� Ƽ�� ������ ��ȳä�� ����.
				if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == true )
				{
					kPacketAck.m_iOK = NetError::ERR_ROOM_39;
					SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
					return;
				}
#endif SERV_MAIN_TOURNAMENT
			}
#endif SERV_TOURNAMENT_CONDITION
			//////////////////////////////////////////////////////////////////////////
		}
		break;

	case CXSLRoom::RT_DUNGEON:
		{
			START_LOG( cerr, L"�߸��� �� ���� ��Ŷ�Դϴ�." )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_ROOM_46;
			SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
			return;
		}
		break;

	default: //��Ÿ���� �̻��� ���
		{
			START_LOG( cerr, L"�߸��� �� Ÿ�� ����.!" )
				<< BUILD_LOGc( kPacket_.m_RoomInfo.m_RoomType )
				<< END_LOG;

			KEGS_CREATE_ROOM_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_ROOM_00;
			SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
		}
		return;
	}

	kPacket.m_bQuickJoin = false;
	kPacket.m_kRoomInfo = kPacket_.m_RoomInfo;

	GetRoomUserInfo( kPacket.m_kRoomUserInfo, kPacket_.m_RoomInfo.m_RoomType );

	//������ ���� �� ����� üũ
	if( kPacket_.m_bObserver == true )
	{
		if( GetAuthLevel() >= SEnum::UAL_SUPERIOR )
		{
			kPacket.m_kRoomUserInfo.m_bIsObserver = true;
		}
		else
		{
			START_LOG( cerr, L"Ư�� ���� ������ ������ ������ �� ����� ��û" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( KBaseServer::GetKObj()->GetUID() )
				<< END_LOG;

			KEGS_CREATE_ROOM_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_ROOM_35;
			SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );

			return;
		}
	}

	START_LOG( clog2, L"���� ���, ���� ����" )
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( KBaseServer::GetKObj()->GetUID() )		
		<< BUILD_LOGc( kPacket_.m_RoomInfo.m_PVPGameType )
		<< BUILD_LOG( kPacket_.m_RoomInfo.m_iDungeonID );

	SendToCnRoom( usEventID, kPacket );
}

//{{ 2008. 12. 4  ������
IMPL_ON_FUNC( EGS_CREATE_TUTORIAL_ROOM_REQ )
{
	//{{ 2010. 9. 7	������	�ߺ� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP ), EGS_CREATE_TUTORIAL_ROOM_REQ, EGS_CREATE_TUTORIAL_ROOM_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}	

	KEGS_CREATE_TUTORIAL_ROOM_ACK kPacketAck;
	u_short usEventID;

	KERM_OPEN_ROOM_REQ kPacket;

	kPacket_.m_RoomInfo.m_uiRoomListID = GetRoomListID();

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		if( GetLevel() > SiKGameSysVal()->GetGuestUserLimitLevel() )
		{
			START_LOG( cerr, L"Ŭ���̾�Ʈ���� ü��ID�� ����20�Ǿ����� ������ ĳ���������� �̵��������ٵ� ���� ������ �ϰ��ֳ�?" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_GUEST_USER_02;
			SendPacket( EGS_CREATE_TUTORIAL_ROOM_ACK, kPacketAck );
			return;
		}
	}

	// [����] Ư�������� ���(��:�ʽ��ڽ�) ��Ƽ�� ������ �ʾƵ� ������ �� �ְ� ������ �ұ�?
	if( !CXSLDungeon::IsSinglePlayDungeon( kPacket_.m_RoomInfo.m_iDungeonID ) )
	{
		START_LOG( cerr, L"�Ϲ� ������ �� ��Ŷ���� �����Ϸ� �ϸ�ȵǴµ�?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_RoomInfo.m_iDungeonID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_ROOM_30;
		SendPacket( EGS_CREATE_TUTORIAL_ROOM_ACK, kPacketAck );
		return;
	}

	switch( kPacket_.m_RoomInfo.m_RoomType )
	{
	case CXSLRoom::RT_DUNGEON:
		{
			usEventID = ERM_OPEN_DUNGEON_ROOM_REQ;

			//��� �̻��� �ƴϸ� üũ�Ѵ�.
			if( GetAuthLevel() >= SEnum::UAL_GM )
			{
				START_LOG( cout, L"��� ������ ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCharUID() )
#else
					<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( kPacket_.m_RoomInfo.m_iDungeonID )
					;
			}

#ifdef SERV_FIX_TUTORIAL_DUNGEON_GAME_INFO// �۾���¥: 2013-05-15	// �ڼ���
			// ���� �÷��� ������ ����
			m_kUserDungeonManager.SetDungeonGameInfo( kPacket_.m_RoomInfo.m_iDungeonID, kPacket_.m_RoomInfo.m_DifficultyLevel, kPacket_.m_RoomInfo.m_cDungeonMode );
#endif // SERV_FIX_TUTORIAL_DUNGEON_GAME_INFO
		}
		break;

	default: //��Ÿ���� �̻��� ���
		{
			START_LOG( cerr, L"�߸��� �� Ÿ�� ����.!" )
				<< BUILD_LOG( kPacket_.m_RoomInfo.m_RoomType )
				<< END_LOG;

			KEGS_CREATE_TUTORIAL_ROOM_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_ROOM_00;
			SendPacket( EGS_CREATE_TUTORIAL_ROOM_ACK, kPacketAck );
		}
		return;
	}

	kPacket.m_bQuickJoin = false;
	kPacket.m_kRoomInfo = kPacket_.m_RoomInfo;

	GetRoomUserInfo( kPacket.m_kRoomUserInfo, kPacket_.m_RoomInfo.m_RoomType );

	START_LOG( clog2, L"���� ���, ���� ����" )
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( KBaseServer::GetKObj()->GetUID() )
		<< BUILD_LOG( kPacket_.m_RoomInfo.m_iDungeonID );

	// �� �������� �������� FSM���� �ٲ���´�.
	if( GetStateID() == KGSFSM::S_SERVER_SELECT )
	{
		StateTransition( KGSFSM::I_TO_FIELD_MAP );
	}

	SendToCnRoom( usEventID, kPacket );
}
//}}

_IMPL_ON_FUNC( ERM_OPEN_ROOM_ACK, KEGS_CREATE_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetRoomUID( kPacket_.m_RoomInfo.m_RoomUID );

		if( GetRoomUID() != 0 )
		{
			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_PVP:
				//SendUpdateUnitState( CXSLUnit::CUS_PVP_WAIT );
				//SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_01 );
				break;

			case CXSLRoom::RT_DUNGEON:
				{
					//if( !DeleteRequiredItemToEnterDungeonRoom( kPacket_.m_RoomInfo.m_iDungeonID + static_cast<int>( kPacket_.m_RoomInfo.m_DifficultyLevel ), kPacket_.m_vecInventorySlotInfo ) )
					//{
					//    // �� ���� ���� ó�� - ���ͼ����� ERM_LEAVE_ROOM_REQ�� ������ ����Ż ó�� ��
					//    // ���ƿ��� ACK�� KGSFSM�� �ɷ��� Ŭ���̾�Ʈ�� ���۵��� �ʴ´�.
					//    KERM_LEAVE_ROOM_REQ kPacketReq;
					//    kPacketReq.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_13;
					//    SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacketReq );

					//    // �� ���� ���� �ڵ�
					//    kPacket_.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_01;
					//    goto end_proc;
					//}
				}

				//SendUpdateUnitState( CXSLUnit::CUS_DUNGEON_WAIT );
				break;
			}
		}

		//{{ 2007. 11. 7  ������  ����ó���� ���� �ҽ��ڵ� ��ġ�̵�
		StateTransition( KGSFSM::I_TO_ROOM );
		m_kTimer[TM_ROOM].restart();
		//}}

		// �ʵ忡�� ��Ż
		SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );

		if( CXSLDungeon::IsSinglePlayDungeon( kPacket_.m_RoomInfo.m_iDungeonID )  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
		{
			// Ʃ�丮�� ������ ��� ��Ƽ���� ��Ż��Ŵ
			SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_01 );
		}

		//{{ 2009. 10. 8  ������	���
#ifdef GUILD_TEST
		SendUpdateGuildMemberInfo();
#endif GUILD_TEST
		//}}
	}
	//else
	//{
	//	LOG_SUCCESS( kPacket_.m_iOK == NetError::ERR_ROOM_05 )
	//		<< BUILD_LOG( L"����� ����.!" )
	//		<< BUILD_LOG( kPacket_.m_RoomInfo.m_RoomType )
	//		<< BUILD_LOG( GetCharUID() )
	//		<< BUILD_LOG( GetCharName() )
	//		<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
	//		<< END_LOG;
	//}

	//end_proc:
	if( CXSLDungeon::IsSinglePlayDungeon( kPacket_.m_RoomInfo.m_iDungeonID )  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
	{
		KEGS_CREATE_TUTORIAL_ROOM_ACK kPacketAck;
		kPacketAck.m_iOK			 = kPacket_.m_iOK;
		kPacketAck.m_RoomInfo		 = kPacket_.m_RoomInfo;
		kPacketAck.m_vecSlot		 = kPacket_.m_vecSlot;
		kPacketAck.m_wstrCNIP		 = kPacket_.m_wstrCNIP;
		SendPacket( EGS_CREATE_TUTORIAL_ROOM_ACK, kPacketAck );
	}
	else
	{
		SendPacket( EGS_CREATE_ROOM_ACK, kPacket_ );
	}    
}

IMPL_ON_FUNC( EGS_JOIN_ROOM_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_JOIN_ROOM_REQ, EGS_JOIN_ROOM_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	KEGS_JOIN_ROOM_ACK kPacketAck;
	KERM_JOIN_ROOM_REQ kPacket;

	KRoomInfoPtr spRoomInfo = SiKRoomListManager()->GetRoomInfo( kPacket_.m_iRoomUID );
	if( !spRoomInfo )
	{
		// �ð� ���̷� ���� ������ ����̴�.
		kPacketAck.m_iOK = NetError::ERR_ROOM_16;
		SendPacket( EGS_JOIN_ROOM_ACK, kPacketAck );

		START_LOG( cwarn, L"���� �����Ǿ� join�� �ȵǴ� ���.!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacketAck.m_iOK ) )
			<< END_LOG;

		return;
	}

	// �߸��� ä���� �濡 ���� ���ϵ��� ����ó��
	if( spRoomInfo->m_uiRoomListID != GetRoomListID() )
	{
		// [����] �ð� ���̷� Ŭ���̾�Ʈ�� �渮��Ʈ�� ������ ���� �����־� �Ͼ�� ����
		kPacketAck.m_iOK = NetError::ERR_ROOM_16;
		SendPacket( EGS_JOIN_ROOM_ACK, kPacketAck );
		return;
	}

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// �۾���¥: 2013-10-08	// �ڼ���
	if( ( m_kUserCheatManager.IsObserverMode() == false ) && ( GetRoomListID() == SiKRoomListManager()->GetPvpOffcialRoomListID() ) )
	{
		kPacketAck.m_iOK = NetError::ERR_ROOM_53;	// ���� �������� ������ �� �����ϴ�.
		SendPacket( EGS_JOIN_ROOM_ACK, kPacketAck );
		return;
	}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM

	// ü��ID ��� ����
	if( IsGuestUser() )
	{
		if( GetLevel() > SiKGameSysVal()->GetGuestUserLimitLevel() )
		{
			START_LOG( cerr, L"Ŭ���̾�Ʈ���� ü��ID�� ����20�Ǿ����� ������ ĳ���������� �̵��������ٵ� ���� ������ �ϰ��ֳ�?" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_GUEST_USER_02;
			SendPacket( EGS_JOIN_ROOM_ACK, kPacketAck );
			return;
		}
	}

	//:: ������ �����Ұ��.
	switch( CXSLRoom::GetRoomType( kPacket_.m_iRoomUID ) )
	{
	case CXSLRoom::RT_PVP:
		{
			//{{ 2010. 12. 7	������	�ٸ� ä�� üũ
#ifdef SERV_WRONG_PVP_CHANNEL_JOIN_FIX
			int iPVPChannelClass = 0;
			if( GetPVPChannelClass( iPVPChannelClass ) == false )
			{
				// [����] �ð� ���̷� Ŭ���̾�Ʈ�� �渮��Ʈ�� ������ ���� �����־� �Ͼ�� ����
				kPacketAck.m_iOK = NetError::ERR_ROOM_16;
				SendPacket( EGS_JOIN_ROOM_ACK, kPacketAck );
				return;
			}
#else
			int iPVPChannelClass = 0;
			LIF( GetPVPChannelClass( iPVPChannelClass ) );
#endif SERV_WRONG_PVP_CHANNEL_JOIN_FIX
			//}}

			//////////////////////////////////////////////////////////////////////////
			// [�̺�Ʈ] (2010-07-29 ~ 2010-08-11) ��ȸä�� ���� ����
#ifdef SERV_TOURNAMENT_CONDITION
			if( iPVPChannelClass == KPVPChannelInfo::PCC_TOURNAMENT )
			{
#ifdef SERV_MAIN_TOURNAMENT
				// �������� Ƽ�� ������ ��ȸä�� ����.
				if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == false )
				{
					kPacketAck.m_iOK = NetError::ERR_ROOM_50;
					SendPacket( EGS_JOIN_ROOM_ACK, kPacketAck );
					return;
				}
#else
				// �������� Ƽ�� ������ ��ȸä�� ����.
				if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == true )
				{
					kPacketAck.m_iOK = NetError::ERR_ROOM_39;
					SendPacket( EGS_JOIN_ROOM_ACK, kPacketAck );
					return;
				}
#endif SERV_MAIN_TOURNAMENT				
			}
#endif SERV_TOURNAMENT_CONDITION
			//////////////////////////////////////////////////////////////////////////
		}
		break;

	case CXSLRoom::RT_DUNGEON:
		{
			START_LOG( cerr, L"�߸��� �� ���� ��Ŷ�Դϴ�." )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_ROOM_46;
			SendPacket( EGS_JOIN_ROOM_ACK, kPacketAck );
			return;
		}
		break;

	default:
		break;
	}

	kPacket.m_bQuickJoin = false;
	kPacket.m_wstrPassword = kPacket_.m_wstrPassword;
#ifdef SERV_NEW_PVPROOM_PROCESS
	kPacket.m_bInvitation = kPacket_.m_bInvitation;
#endif SERV_NEW_PVPROOM_PROCESS

	GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::GetRoomType( kPacket_.m_iRoomUID ) );

	//{{ 2012. 12. 5	������		������ ���� ���� ���
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
	if( GetAuthLevel() >= SEnum::UAL_GM  &&  m_kUserCheatManager.IsObserverMode() == true )
	{
		START_LOG( cout, L"��ڰ� ������ ���� �濡 �����մϴ�!" )
			<< BUILD_LOGc( GetAuthLevel() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() );

		kPacket.m_kRoomUserInfo.m_bIsObserver = true;
	}
#else
	kPacket.m_kRoomUserInfo.m_bIsObserver = kPacket_.m_bIsObserver;
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
	//}}

	_SendToCnRoom( kPacket_.m_iRoomUID, ERM_JOIN_ROOM_REQ, kPacket );
}

_IMPL_ON_FUNC( ERM_JOIN_ROOM_ACK, KEGS_JOIN_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetRoomUID( kPacket_.m_RoomInfo.m_RoomUID );

		if( GetRoomUID() != 0 )
		{
			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_PVP:
				//SendUpdateUnitState( CXSLUnit::CUS_PVP_WAIT );
				break;

			case CXSLRoom::RT_DUNGEON:
				{
					//if( !DeleteRequiredItemToEnterDungeonRoom( kPacket_.m_RoomInfo.m_iDungeonID + static_cast<int>( kPacket_.m_RoomInfo.m_DifficultyLevel ), kPacket_.m_vecInventorySlotInfo ) )
					//{
					//    // �� ���� ���� ó�� - ���ͼ����� ERM_LEAVE_ROOM_REQ�� ������ ����Ż ó�� ��
					//    // ���ƿ��� ACK�� KGSFSM�� �ɷ��� Ŭ���̾�Ʈ�� ���۵��� �ʴ´�.
					//    KERM_LEAVE_ROOM_REQ kPacketReq;
					//    kPacketReq.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_13;
					//    SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacketReq );

					//    // �� ���� ���� �ڵ�
					//    kPacket_.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_01;
					//    goto end_proc;
					//}
				}
				//SendUpdateUnitState( CXSLUnit::CUS_DUNGEON_WAIT );
				break;
			}
		}

		//{{ 2007. 11. 7  ������  ����ó���� ���� �ҽ��ڵ� ��ġ�̵�		
		StateTransition( KGSFSM::I_TO_ROOM );
		m_kTimer[TM_ROOM].restart();
		//}}

		// �ʵ忡�� ��Ż
		SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );

		//{{ 2009. 10. 8  ������	���
#ifdef GUILD_TEST
		SendUpdateGuildMemberInfo();
#endif GUILD_TEST
		//}}
	}
	else
	{
		START_LOG_WITH_NAME( clog )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( kPacket_.m_RoomInfo.m_wstrPassword );
	}

	//end_proc:
	SendPacket( EGS_JOIN_ROOM_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_JOIN_ROOM_NOT, KEGS_JOIN_ROOM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_JOIN_ROOM_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_QUICK_JOIN_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_QUICK_JOIN_REQ, EGS_QUICK_JOIN_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	UidType iRoomUID = SiKRoomListManager()->GetWaitingRoomUID( GetRoomListID(), kPacket_ );

	KEGS_QUICK_JOIN_ACK kPacketAck;

	if( iRoomUID > 0 )
	{
		KRoomInfoPtr spRoomInfo = SiKRoomListManager()->GetRoomInfo( iRoomUID );
		if( !spRoomInfo )
		{
			// �ð� ���̷� ���� ������ ����̴�.
			kPacketAck.m_iOK = NetError::ERR_ROOM_16;
			SendPacket( EGS_QUICK_JOIN_ACK, kPacketAck );

			START_LOG( cerr, L"���� �����Ǿ� join�� �ȵǴ� ���.!" )
				<< BUILD_LOG( NetError::GetErrStr( kPacketAck.m_iOK ) )
				<< END_LOG
				;

			return;
		}

		// ü��ID ��� ����
		if( IsGuestUser() )
		{
			if( GetLevel() > SiKGameSysVal()->GetGuestUserLimitLevel() )
			{
				START_LOG( cerr, L"Ŭ���̾�Ʈ���� ü��ID�� ����20�Ǿ����� ������ ĳ���������� �̵��������ٵ� ���� ������ �ϰ��ֳ�?" )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_GUEST_USER_02;
				SendPacket( EGS_QUICK_JOIN_ACK, kPacketAck );
				return;
			}
		}

		KERM_JOIN_ROOM_REQ kPacket;

		switch( CXSLRoom::GetRoomType( iRoomUID ) )
		{
		case CXSLRoom::RT_PVP:
			{
#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// �۾���¥: 2013-10-08	// �ڼ���
				if( ( m_kUserCheatManager.IsObserverMode() == false ) && ( GetRoomListID() == SiKRoomListManager()->GetPvpOffcialRoomListID() ) )
				{
					kPacketAck.m_iOK = NetError::ERR_ROOM_53;	// ���� �������� ������ �� �����ϴ�.
					SendPacket( EGS_QUICK_JOIN_ACK, kPacketAck );
					return;
				}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM

				//////////////////////////////////////////////////////////////////////////
				// [�̺�Ʈ] (2010-07-29 ~ 2010-08-11) ��ȸä�� ���� ����
#ifdef SERV_TOURNAMENT_CONDITION
				int iPVPChannelClass = 0;
				LIF( GetPVPChannelClass( iPVPChannelClass ) );
				if( iPVPChannelClass == KPVPChannelInfo::PCC_TOURNAMENT )
				{
#ifdef SERV_MAIN_TOURNAMENT
					// �������� Ƽ�� ������ ��ȸä�� ����.
					if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == false )
					{
						kPacketAck.m_iOK = NetError::ERR_ROOM_50;
						SendPacket( EGS_QUICK_JOIN_ACK, kPacketAck );
						return;
					}
#else
					// �������� Ƽ�� ������ ��ȸä�� ����.
					if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == true )
					{
						kPacketAck.m_iOK = NetError::ERR_ROOM_39;
						SendPacket( EGS_QUICK_JOIN_ACK, kPacketAck );
						return;
					}
#endif SERV_MAIN_TOURNAMENT
				}
#endif SERV_TOURNAMENT_CONDITION
				//////////////////////////////////////////////////////////////////////////
			}
			break;

		case CXSLRoom::RT_DUNGEON:
			{
				START_LOG( cerr, L"�߸��� ������ ��Ŷ�Դϴ�." )
#ifndef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_ROOM_46;
				SendPacket( EGS_QUICK_JOIN_ACK, kPacketAck );
				return;
			}
			break;

		default:
			break;
		}

		kPacket.m_bQuickJoin = true;
#ifdef SERV_NEW_PVPROOM_PROCESS
		kPacket.m_bInvitation = false;
#endif SERV_NEW_PVPROOM_PROCESS
		GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::GetRoomType( iRoomUID ) );

		_SendToCnRoom( iRoomUID, ERM_JOIN_ROOM_REQ, kPacket );
	}
	else
	{
		kPacketAck.m_iOK = NetError::ERR_ROOM_07;
		SendPacket( EGS_QUICK_JOIN_ACK, kPacketAck );
	}
}

_IMPL_ON_FUNC( ERM_QUICK_JOIN_ACK, KEGS_JOIN_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetRoomUID( kPacket_.m_RoomInfo.m_RoomUID );

		switch(	CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_DUNGEON:
			{
				//if( !DeleteRequiredItemToEnterDungeonRoom( kPacket_.m_RoomInfo.m_iDungeonID + static_cast<int>( kPacket_.m_RoomInfo.m_DifficultyLevel ), kPacket_.m_vecInventorySlotInfo ) )
				//{
				//    // �� ���� ���� ó�� - ���ͼ����� ERM_LEAVE_ROOM_REQ�� ������ ����Ż ó�� ��
				//    // ���ƿ��� ACK�� KGSFSM�� �ɷ��� Ŭ���̾�Ʈ�� ���۵��� �ʴ´�.
				//    KERM_LEAVE_ROOM_REQ kPacketReq;
				//    kPacketReq.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_13;
				//    SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacketReq );

				//    // �� ���� ���� �ڵ�
				//    kPacket_.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_01;
				//    goto end_proc;
				//}
			}
			break;

		default:
			break;
		}

		StateTransition( KGSFSM::I_TO_ROOM );
		m_kTimer[TM_ROOM].restart();

		// �ʵ忡�� ��Ż
		SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );

		//{{ 2009. 10. 8  ������	���
#ifdef GUILD_TEST
		SendUpdateGuildMemberInfo();
#endif GUILD_TEST
		//}}
	}
	else
	{
		START_LOG_WITH_NAME( clog )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( kPacket_.m_RoomInfo.m_wstrPassword );
	}

	//end_proc:
	SendPacket( EGS_QUICK_JOIN_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_LEAVE_ROOM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_LEAVE_ROOM_REQ, EGS_LEAVE_ROOM_ACK );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( GetRoomUID() );

	u_short usEventID;

	//�Ʒü��� ��赥���͸� �ޱ����� ��Ŷ�� ��ɺ��� ��Ȱ��.
	switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
	{
	case CXSLRoom::RT_TRAININGCENTER:
		usEventID = ERM_LEAVE_TC_ROOM_REQ;
		break;

		//{{ 2010. 03. 17  ������	���� ���� ����		
	case CXSLRoom::RT_SQUARE:
		{
			START_LOG( cerr, L"�� ��Ż�ε� ������ ���忡 �ֳ�? �Ͼ���� �ȵǴ� ����" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			KEGS_LEAVE_SQUARE_REQ kPacket;
			kPacket.m_iReason = kPacket_.m_iReason;
			SendToCnRoom( ERM_LEAVE_SQUARE_REQ, kPacket );
			return;
		}
		break;
		//}}

	default:
		usEventID = ERM_LEAVE_ROOM_REQ;
		break;
	}

	KERM_LEAVE_ROOM_REQ kPacket;
	kPacket.m_iReason = kPacket_.m_iReason;
	//{{ 2012. 04. 17	������	���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	kPacket.m_kResultInfo = kPacket_.m_kResultInfo;
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}
	//{{ 2012. 10. 16	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	kPacket.m_kGamePlayNetworkInfo = kPacket_.m_kGamePlayNetworkInfo;
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	//}}

	SendToCnRoom( usEventID, kPacket );
}

//{{ 2011. 02. 21  ��μ�	Ư�� ���� ���� ������ ���� ����
//#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
_IMPL_ON_FUNC( ERM_LEAVE_ROOM_ACK, KEGS_LEAVE_ROOM_ACK )
//#else
//_IMPL_ON_FUNC( ERM_LEAVE_ROOM_ACK, KPacketOK )
//#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
//}}
{
	//{{ 2010. 03. 17  ������	���� ���� ����
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_LEAVE_ROOM_ACK );
	//}}

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.20 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
	m_bTimeControlItemCheckDungeonPlay = false;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	// �����濡�� ������� ��Ƽ Ż��.
	const CXSLRoom::ROOM_TYPE eRoomType = CXSLRoom::GetRoomType( GetRoomUID() );

	//{{ 2012. 04. 16	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	std::vector< int > vecDeleteBuff;
	std::vector< KBuffInfo > vecActivateBuff;

	switch( eRoomType )
	{
#ifdef SERV_FIX_REVENGE_BUFF// �۾���¥: 2013-08-09	// �ڼ���
	case CXSLRoom::RT_PVP:
		m_kUserBuffManager.OnLeavePVPRoom( GetThisPtr<KGSUser>(), vecActivateBuff, vecDeleteBuff );
		break;
#endif // SERV_FIX_REVENGE_BUFF

	case CXSLRoom::RT_DUNGEON:
		{
			//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
			bool bBadAttitudeUser = false;

			if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_34 )
			{
				bBadAttitudeUser = true;
				kPacket_.m_iReason = NetError::NET_OK;
			}

			m_kUserBuffManager.OnLeaveDungeonRoom( GetThisPtr<KGSUser>(), vecActivateBuff, vecDeleteBuff, bBadAttitudeUser );
#else
			m_kUserBuffManager.OnLeaveDungeonRoom( GetThisPtr<KGSUser>(), vecDeleteBuff );

			if( vecDeleteBuff.empty() == false )
			{
				// �α��� ���� ���� �Ŵ����� ������Ʈ
				KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
				kReq.m_iUnitUID = GetCharUID();
				kReq.m_vecDeActivateBuff = vecDeleteBuff;
				SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
			}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
			//}
		}
		break;

	case CXSLRoom::RT_BATTLE_FIELD:
		m_kUserBuffManager.OnLeaveBattleField( GetThisPtr<KGSUser>(), vecActivateBuff, vecDeleteBuff );
		break;
	}

	if( vecDeleteBuff.empty() == false || vecActivateBuff.empty() == false )
	{
		// �α��� ���� ���� �Ŵ����� ������Ʈ
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
		kReq.m_iUnitUID = GetCharUID();
		kReq.m_vecDeActivateBuff = vecDeleteBuff;
		kReq.m_vecActivateBuff = vecActivateBuff;
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	bool bIsSuccess = false;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		bIsSuccess = true;
	}
	else
	{
		if( GetRoomUID() != 0  &&  kPacket_.m_iOK == NetError::ERR_ROOM_34 )
			bIsSuccess = true;
	}

	if( bIsSuccess == true )
	{
		SetRoomUID( 0 );
		m_kUserDungeonManager.SetDungeonGameInfo( 0, 0, 0 );

		StateTransition( KGSFSM::I_TO_FIELD_MAP );

		//SendUpdateUnitState( CXSLUnit::CUS_ONLINE );

		if( GetTradeUID() != 0 )
		{
			KEGS_BREAK_TRADE_REQ kPacket;
			kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
			_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
		}
	}

	//{{ 2013. 2. 7	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	if( ( eRoomType == CXSLRoom::RT_PVP ) ||
		( eRoomType == CXSLRoom::RT_DUNGEON )
		)
#else
	if( eRoomType == CXSLRoom::RT_DUNGEON )
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
		//}}
	{
		//{{ 2011. 02. 21  ��μ�	Ư�� ���� ���� ������ ���� ����
//#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
		if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_22 )
		{
			SendLeaveParty( NetError::NOT_LEAVE_ROOM_REASON_22 );
		}
		else
		{
			SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_02 );
		}
//#else
//		SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_02 );
//#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
		//}}
	}

	SendPacket( EGS_LEAVE_ROOM_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_LEAVE_ROOM_NOT, KEGS_LEAVE_ROOM_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_LEAVE_ROOM_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_BAN_USER_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
//#ifdef SERV_REPEAT_FILTER_REFAC
	_VERIFY_STATE_REPEAT_FILTER( cerr, ( 1, KGSFSM::S_ROOM ), EGS_BAN_USER_REQ, EGS_BAN_USER_ACK, KPacketOK );
//#else
//	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
//#endif SERV_REPEAT_FILTER_REFAC
	//}}

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iUnitUID );

	if( kPacket_.m_iUnitUID != GetCharUID() )
	{
		SendToCnRoom( ERM_BAN_USER_REQ, kPacket_ );
	}
	//�ڱ��ڽ��� ���� ��ų�� ����.
	else
	{
		KPacketOK kPacket;
		kPacket.m_iOK = NetError::ERR_ROOM_26;
		SendPacket( EGS_BAN_USER_ACK, kPacket );
	}
}

_IMPL_ON_FUNC( ERM_BAN_USER_ACK, KPacketOK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	SendPacket( EGS_BAN_USER_ACK, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( ERM_BAN_USER_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	//{{ 2010. 8. 18	������	�濡�� ����Ǿ������� �ӽ��κ� ����
	if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
	{
		ClearTempInventory();
	}
	//}}

	if( GetTradeUID() != 0 )
	{
		KEGS_BREAK_TRADE_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_20;
		_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
	}

	SetRoomUID( 0 );

	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	SendID( EGS_BAN_USER_NOT );
}

IMPL_ON_FUNC( EGS_STATE_CHANGE_GAME_START_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_STATE_CHANGE_GAME_START_REQ, EGS_STATE_CHANGE_GAME_START_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	START_LOG_WITH_NAME( clog );

	KERM_GAME_START_REQ kPacket;
	kPacket.m_sWorldID = 0;

	if( GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_DUNGEON:
			{
				// �ټ��� �˻� - �� �ڵ鷯�� ����ϴ� �����̶�� Ʃ�丮���ΰ�..
				//if( CheckingSpirit( 0 ) == false )
				//{
				//	KEGS_STATE_CHANGE_GAME_START_ACK kPacketAck;
				//	kPacketAck.m_iOK = NetError::ERR_ROOM_27;
				//	SendPacket( EGS_STATE_CHANGE_GAME_START_ACK, kPacketAck );
				//	return;
				//}
			}
			break;

		case CXSLRoom::RT_PVP:
			{
				kPacket.m_sWorldID = kPacket_.m_sWorldID;

				//{{ 2010. 7. 28  ������	��ȸ ä�� ���� ó��
#ifdef SERV_TOURNAMENT_CONDITION
				int iPvpChannelClass = 0;
				LIF( GetPVPChannelClass( iPvpChannelClass ) );
				if( iPvpChannelClass == KPVPChannelInfo::PCC_NONE )
				{
					START_LOG( cerr, L"�渮��Ʈ���� ���� ã�� �� �����ϴ�." )
						<< BUILD_LOG( GetRoomUID() )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;

					KEGS_STATE_CHANGE_GAME_START_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_ROOM_31;
					SendPacket( EGS_STATE_CHANGE_GAME_START_ACK, kPacket );
					return;
				}
				else if( iPvpChannelClass == KPVPChannelInfo::PCC_TOURNAMENT )
				{
#ifdef SERV_MAIN_TOURNAMENT
					// �������� Ƽ�� ������ ��ȸä�� ����.
					if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == false )
					{
						KEGS_STATE_CHANGE_GAME_START_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_ROOM_50;
						SendPacket( EGS_STATE_CHANGE_GAME_START_ACK, kPacket );
						return;
					}
#else
					// �������� Ƽ�� ������ ��ȸä�� ����.
					if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == true )
					{
						KEGS_STATE_CHANGE_GAME_START_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_ROOM_39;
						SendPacket( EGS_STATE_CHANGE_GAME_START_ACK, kPacket );
						return;
					}
#endif SERV_MAIN_TOURNAMENT
				}
#endif SERV_TOURNAMENT_CONDITION
				//}}
			}
			break;

		default:
			break;
		}
	}
	else
	{
		START_LOG( cerr, L"�濡 �������ִµ� RoomUID�� ����?" )
			<< BUILD_LOG( GetRoomUID() )
			<< END_LOG;
		return;
	}

	SendToCnRoom( ERM_GAME_START_REQ, kPacket );
}

IMPL_ON_FUNC( EGS_GAME_LOADING_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iLoadingProgress );

	if( kPacket_.m_iLoadingProgress >= 100 )		// �ε��� �Ϸ� �� ���� - ���� ���� ����
	{
	//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
		kPacket_.m_iUnitLevel = static_cast<int>( GetLevel() );
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	//}}

#ifdef SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS_TEMP// �۾���¥: 2013-06-12	// �ڼ���
		SendUpdateBuffInfo();
#endif // SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS_TEMP
	}

	//{{ 2011. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		SendToCnRoom( ERM_BATTLE_FIELD_GAME_LOADING_REQ, kPacket_ );
	}
	else
	{
		SendToCnRoom( ERM_GAME_LOADING_REQ, kPacket_ );
	}
#else
	SendToCnRoom( ERM_GAME_LOADING_REQ, kPacket_ );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
}

IMPL_ON_FUNC( ERM_GAME_START_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_STATE_CHANGE_GAME_START_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	SendPacket( EGS_STATE_CHANGE_GAME_START_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_GAME_START_NOT )//, KEGS_STATE_CHANGE_GAME_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_STATE_CHANGE_GAME_START_NOT kPacket;
	kPacket.m_cRoomState	= kPacket_.m_cRoomState;
	kPacket.m_iGameType		= kPacket_.m_iGameType;	
	kPacket.m_vecSlot		= kPacket_.m_vecSlot;
	kPacket.m_sWorldID		= kPacket_.m_sWorldID;

	if( GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
			{
				//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
				m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_PVP_PLAY_COUNT );

				const int iPvpPlayCount = m_kUserStatistics.GetCharacterCount( KUserStatistics::CGCT_PVP_PLAY_COUNT );
				if( iPvpPlayCount == 1 )
				{
					WriteCharacterLogToDB( KELOG_USER_CHARACTER_LOG_NOT::LT_FIRST_PVP );
				}
#endif SERV_CHAR_LOG
				//}}

				//SendUpdateUnitState( CXSLUnit::CUS_PVP_PLAY );
			}			
			break;

		case CXSLRoom::RT_DUNGEON:
			{
#ifdef SERV_EVENT_CHECK_POWER
				IF_EVENT_ENABLED( CEI_CHECK_POWER )
				{
					UpdateCheckPowerScore( kPacket_.m_iDungeonID, CXSLDungeon::RT_F, 1, false, CXSLDungeon::DL_NORMAL, false, false );
				}
#endif SERV_EVENT_CHECK_POWER

				//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
				m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_DUNGEON_PLAY_COUNT );

				const int iDungeonPlayCount = m_kUserStatistics.GetCharacterCount( KUserStatistics::CGCT_DUNGEON_PLAY_COUNT );
				if( iDungeonPlayCount == 1 )
				{
					WriteCharacterLogToDBWithDungeonID( KELOG_USER_CHARACTER_LOG_NOT::LT_FIRST_DUNGEON_PLAY, kPacket_.m_iDungeonID );
				}
#endif SERV_CHAR_LOG
				//}}

				//if( !DeleteRequiredItemToEnterDungeonRoom( kPacket_.m_iDungeonID, kPacket.m_vecInventorySlotInfo ) )
				//if( !CheckRequiredItemToEnterDungeonRoom( kPacket_.m_iDungeonID ) )
				//{
				//	//�켱 ���ӽ��۽� �������� ������ ������ �α׸� ���ܺ���..
				//	START_LOG( clog, L"�������� ���۽� �ʿ�������� ����.!" )
				//		<< BUILD_LOG( GetCharName() )
				//		<< BUILD_LOG( kPacket_.m_iDungeonID )
				//		<< END_LOG;

				//    // �� ���� ���� ó�� - ���ͼ����� ERM_LEAVE_ROOM_REQ�� ������ ����Ż ó�� ��
				//    // ���ƿ��� ACK�� KGSFSM�� �ɷ��� Ŭ���̾�Ʈ�� ���۵��� �ʴ´�.
				//    KERM_LEAVE_ROOM_REQ kPacketReq;
				//    kPacketReq.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_22;
				//    SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacketReq );

				//    return;
				//}

				//SendUpdateUnitState( CXSLUnit::CUS_DUNGEON_PLAY );
			}
			break;
		}
	}

	// �ټ��� ����
	//{{ 2012. 03. 20	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	kPacket.m_iSpirit = m_kUserSpiritManager.GetSpirit();
#else
	kPacket.m_iSpirit = m_iSpirit;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	SendPacket( EGS_STATE_CHANGE_GAME_START_NOT, kPacket );
}

_IMPL_ON_FUNC( ERM_GAME_LOADING_ACK, KPacketOK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_GAME_LOADING_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_GAME_LOADING_NOT, KEGS_GAME_LOADING_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_GAME_LOADING_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( ERM_GAME_LOADING_TIME_OUT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	//{{ 2010. 11. 23	������	�� �ε� ���� ����
#ifdef SERV_ROOM_LOADING_BUG_FIX
	// �����濡�� ������� ��Ƽ Ż��.
	CXSLRoom::ROOM_TYPE eRoomType = CXSLRoom::GetRoomType( GetRoomUID() );

	if( eRoomType == CXSLRoom::RT_DUNGEON )
	{
		SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_02 );
	}
#endif SERV_ROOM_LOADING_BUG_FIX
	//}}

	//{{ 2010. 12. 30	������	����Ż ����ó��
#ifdef SERV_LEAVE_ROOM_BUG_FIX
	SetRoomUID( 0 );
#endif SERV_LEAVE_ROOM_BUG_FIX
	//}}

	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	SendID( EGS_GAME_LOADING_TIME_OUT_NOT );
}

IMPL_ON_FUNC_NOPARAM( ERM_GAME_LOADING_ALL_UNIT_OK_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendID( EGS_GAME_LOADING_ALL_UNIT_OK_NOT );
}

_IMPL_ON_FUNC( ERM_PLAY_START_NOT, KEGS_PLAY_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
	if( GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_DUNGEON:
			{
				m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_DUNGEON_PLAY_COUNT );

				const int iDungeonPlayCount = m_kUserStatistics.GetCharacterCount( KUserStatistics::CGCT_DUNGEON_PLAY_COUNT );
				if( iDungeonPlayCount == 1 )
				{
					WriteCharacterLogToDBWithDungeonID( KELOG_USER_CHARACTER_LOG_NOT::LT_FIRST_DUNGEON_PLAY, kPacket_.m_iDungeonID );
				}

				//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
				// �������� �˷�����
				KEGS_HENIR_REWARD_COUNT_NOT	kPacket;
				m_kUserDungeonManager.GetHenirRewardCountInfo( kPacket );

				SendPacket( EGS_HENIR_REWARD_COUNT_NOT, kPacket );

				START_LOG( clog, L"�������� ���� ȹ�� Ƚ�� ����!!" )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardEventUnLimited() )
					<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardNormalCount() )
					<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardPremiumCount() )
					<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardEventCount() )
					<< END_LOG;
#endif SERV_NEW_HENIR_TEST
				//}}

#ifdef SERV_COEXISTENCE_FESTIVAL// �۾���¥: 2013-05-14	// �ڼ���
				if( ( CXSLDungeon::IsTutorialDungeon( GetDungeonIDAndDungeonDifficulty() ) == false ) &&
					( CXSLDungeon::IsHenirDungeon( GetDungeonIDAndDungeonDifficulty() ) == false )
					)
				{
					// ��Ʋ �ʵ� ���� ���������Ƿ� ������ ����.
					std::vector< KBuffInfo > vecActivateBuff;
					ActivateBuff( CXSLBuffManager::BTI_FESTIVAL_EVENT_BUFF, vecActivateBuff );

					if( vecActivateBuff.empty() == false )
					{
						// �α��� ���� ���� �Ŵ����� ������Ʈ
						KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
						kReq.m_iUnitUID = GetCharUID();
						kReq.m_vecActivateBuff = vecActivateBuff;
						SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
					}
				}
#endif // SERV_COEXISTENCE_FESTIVAL
			}break;
		}
	}	
#endif SERV_CHAR_LOG
	//}}

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.20 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
	m_bTimeControlItemCheckDungeonPlay = true;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING
#ifdef SERV_SERVER_BUFF_SYSTEM// �۾���¥: 2013-06-08	// �ڼ���
#else // SERV_SERVER_BUFF_SYSTEM
	//{{ 2012. 04. 21	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	KRoomUserInfo kRoomUserInfo;
	KEGS_UPDATE_BUFF_INFO_IN_ROOM_NOT kPacket;
	const int iRoomType = CXSLRoom::GetRoomType( GetRoomUID() );

	kPacket.m_iUnitUID = GetCharUID();
	m_kUserBuffManager.GetBuffList( kPacket.m_vecActivateBuffList, iRoomType );
	GetRoomUserInfo( kRoomUserInfo, iRoomType );
	kPacket.m_kGameStat = kRoomUserInfo.m_kGameStat;
	SendToCnRoom( ERM_UPDATE_BUFF_INFO_IN_ROOM_NOT, kPacket );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}
#endif // SERV_SERVER_BUFF_SYSTEM

	SendPacket( EGS_PLAY_START_NOT, kPacket_ );

	//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
	m_kUserQuestManager.CheckEpicQuest_EnterDungeon( kPacket_.m_iDungeonID, GetThisPtr<KGSUser>() );
	m_kUserQuestManager.Handler_OnEnterTheDungeon( GetThisPtr<KGSUser>() );
#endif SERV_REFORM_QUEST
	//}}

}

IMPL_ON_FUNC( EGS_END_GAME_REQ )
{
	//{{ 2010. 9. 7	������	�ߺ� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	_VERIFY_STATE_REPEAT_FILTER( cerr, ( 1, KGSFSM::S_ROOM ), EGS_END_GAME_REQ, EGS_END_GAME_ACK, KPacketOK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	SendToCnRoom( ERM_END_GAME_REQ, kPacket_ );

	//{{ 2009. 2. 4  ������		���� ���÷���
	if( GetRoomUID() > 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_DUNGEON:
			{
				// ��Ƽ�� ������������ üũ
				if( GetPartyUID() > 0 )
				{
					//{{ 2010. 02. 23  ������	ä�� ��Ƽ ����
#ifdef SERV_CHANNEL_PARTY
					if( SiKPartyListManager()->E_EndGame( GetThisPtr<KGSUser>() ) == false )
#else
#pragma ELSE_MESSAGE
					//if( SiKPartyManager()->E_EndGame( GetPartyUID(), GetThisPtr<KGSUser>() ) == false )
#endif SERV_CHANNEL_PARTY
					//}}					
					{
						START_LOG( cerr, L"���� ���ῡ ���� ��Ƽ�������� ������Ʈ ����!" )
							<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
							<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
							<< BUILD_LOG( GetPartyUID() )
							<< END_LOG;
					}
				}
			}
			break;

		default:
			break;
		}
	}
	else
	{
		START_LOG( cerr, L"End Game�� �����Դµ� RoomUID���� �̻��ϴ�?" )
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
	//}}
}

_IMPL_ON_FUNC( ERM_END_GAME_ACK, KPacketOK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_END_GAME_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_END_GAME_NOT, KEGS_END_GAME_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.20 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
	m_bTimeControlItemCheckDungeonPlay = false;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

	SendPacket( EGS_END_GAME_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_STATE_CHANGE_RESULT_REQ )
{
	//{{ 2010. 9. 7	������	�ߺ� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_STATE_CHANGE_RESULT_REQ, EGS_STATE_CHANGE_RESULT_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}


	SendToCnRoom( ERM_STATE_CHANGE_RESULT_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_STATE_CHANGE_RESULT_ACK, KEGS_STATE_CHANGE_RESULT_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_STATE_CHANGE_RESULT_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_STATE_CHANGE_RESULT_NOT, KEGS_STATE_CHANGE_RESULT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
			//SendUpdateUnitState( CXSLUnit::CUS_PVP_WAIT );
			break;
		case CXSLRoom::RT_DUNGEON:			
			//SendUpdateUnitState( CXSLUnit::CUS_DUNGEON_WAIT );
			break;
		}
	}

	SendPacket( EGS_STATE_CHANGE_RESULT_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_USER_UNIT_RE_BIRTH_POS_REQ )
{
	//{{ 2010. 9. 7	������	�ߺ� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	_VERIFY_STATE_REPEAT_FILTER( cerr, ( 1, KGSFSM::S_ROOM ), EGS_USER_UNIT_RE_BIRTH_POS_REQ, EGS_USER_UNIT_RE_BIRTH_POS_ACK, KPacketOK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	SendToCnRoom( ERM_USER_UNIT_RE_BIRTH_POS_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_USER_UNIT_RE_BIRTH_POS_ACK, KPacketOK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_USER_UNIT_RE_BIRTH_POS_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_USER_UNIT_RE_BIRTH_NOT, KEGS_USER_UNIT_RE_BIRTH_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_USER_UNIT_RE_BIRTH_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_ROOM_LIST_REQ )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_ROOM_LIST_ACK );

	KEGS_ROOM_LIST_ACK kPacket;

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// �۾���¥: 2013-10-08	// �ڼ���
	if( GetRoomListID() == SiKRoomListManager()->GetPvpOffcialRoomListID() )
	{
		START_LOG( cerr, L"���� ���� �� ����Ʈ�� �� ��û����?? ���� ������?" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_ROOM_LIST_ACK, kPacket );
		return;
	}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM

	// RoomListID���� ��ȿ�Ҷ��� �渮��Ʈ�� �ش�
	if( GetRoomListID() > 0 )
	{
		if( SiKRoomListManager()->GetRoomInfoPage( GetRoomListID(), kPacket_, kPacket ) == false )
		{
			START_LOG_WITH_NAME( cerr )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_nViewPage )
				<< END_LOG;

			//06.12.08 Ư������üũ�� ����.
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			SendPacket( EGS_ROOM_LIST_ACK, kPacket );
			return;
		}
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_ROOM_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_USER_UNIT_DIE_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_USER_UNIT_DIE_REQ, kPacket_ );
}

//{{ 2011. 09. 19  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
IMPL_ON_FUNC( ERM_USER_UNIT_DIE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_USER_UNIT_DIE_ACK kAck;
	kAck.m_iOK = kPacket_.m_iOK;
	kAck.m_fReBirthTime = kPacket_.m_fReBirthTime;
	kAck.m_EXP = kPacket_.m_EXP;
	kAck.m_VP = kPacket_.m_VP;

	//{{ 2012. 04. 23	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	std::vector< int > vecDeactivatedBuff;
	m_kUserBuffManager.OnUserDied( GetThisPtr<KGSUser>(), vecDeactivatedBuff );
	if( vecDeactivatedBuff.empty() == false )
	{
		// �α��� ���� ���� �Ŵ����� ������Ʈ
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
		kReq.m_iUnitUID = GetCharUID();
		kReq.m_vecDeActivateBuff = vecDeactivatedBuff;
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	SendPacket( EGS_USER_UNIT_DIE_ACK, kAck );

	m_kUserTitleManager.OnUserUnitDie( static_cast< KEGS_USER_UNIT_DIE_REQ::USER_UNIT_DIE_REASON >(kPacket_.m_ucDieReason), GetThisPtr<KGSUser>() );
}
#endif SERV_ADD_TITLE_CONDITION
//}}

IMPL_ON_FUNC( ERM_USER_UNIT_DIE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_USER_UNIT_DIE_NOT kPacket;
	kPacket.m_KillerNPCUID					= kPacket_.m_KillerNPCUID;
	kPacket.m_KillerUserUnitUID				= kPacket_.m_KillerUserUnitUID;
	kPacket.m_MaxDamageKillerUserUnitUID	= kPacket_.m_MaxDamageKillerUserUnitUID;
	kPacket.m_KilledUserUnitUID				= kPacket_.m_KilledUserUnitUID;

	SendPacket( EGS_USER_UNIT_DIE_NOT, kPacket );

	if( GetCharUID() == kPacket_.m_KillerUserUnitUID || GetCharUID() == kPacket_.m_MaxDamageKillerUserUnitUID )
	{
		m_kUserQuestManager.Handler_OnPVPKill( kPacket_.m_iGameType
											 , GetThisPtr<KGSUser>()
											 //{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
											 , kPacket_.m_bOfficialMatch
#endif SERV_NEW_PVP_QUEST
											 //}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
											 , kPacket_.m_bHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
											, kPacket_.m_killedUserUnitType
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
											, kPacket_.m_killedUserUnitClass
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
											 );
	}
}

IMPL_ON_FUNC_NOPARAM( EGS_USER_UNIT_DIE_COMPLETE_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_USER_UNIT_DIE_COMPLETE_REQ );
}

_IMPL_ON_FUNC( ERM_USER_UNIT_DIE_COMPLETE_ACK, KEGS_USER_UNIT_DIE_COMPLETE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_USER_UNIT_DIE_COMPLETE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_USER_UNIT_DIE_COMPLETE_NOT, KEGS_USER_UNIT_DIE_COMPLETE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_USER_UNIT_DIE_COMPLETE_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_REMAINING_PLAY_TIME_NOT, float )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_REMAINING_PLAY_TIME_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( ERM_PLAY_TIME_OUT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendID( EGS_PLAY_TIME_OUT_NOT );
}

IMPL_ON_FUNC( EGS_MY_USER_UNIT_INFO_TO_SERVER_REQ )
{
	//{{ 2010. 9. 8	������	�ߺ� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	_VERIFY_STATE_REPEAT_FILTER( cerr, ( 1, KGSFSM::S_ROOM ), EGS_MY_USER_UNIT_INFO_TO_SERVER_REQ, EGS_MY_USER_UNIT_INFO_TO_SERVER_ACK, KPacketOK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	KERM_MY_USER_UNIT_INFO_TO_SERVER_REQ kPacket;
	//{{ 2011. 01. 21	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
	kPacket.m_kDungeonUnitInfo = kPacket_.m_kMyPlayResult;
#else
	kPacket.m_kDungeonUnitInfo.m_fHP			= kPacket_.m_fHP;
	kPacket.m_kDungeonUnitInfo.m_TotalScore		= kPacket_.m_TotalScore;
	kPacket.m_kDungeonUnitInfo.m_ComboScore		= kPacket_.m_ComboScore;
	kPacket.m_kDungeonUnitInfo.m_TechScore		= kPacket_.m_TechScore;
	kPacket.m_kDungeonUnitInfo.m_nDamageCount	= kPacket_.m_nDamageCount;
#endif SERV_DUNGEON_RANK_NEW
	//}}

	//{{ 2010. 05. 12  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	GetRoomUserInfo( kPacket.m_kRoomUserInfo );
#endif SERV_INTEGRATION
	//}}

	SendToCnRoom( ERM_MY_USER_UNIT_INFO_TO_SERVER_REQ, kPacket );
}

_IMPL_ON_FUNC( ERM_MY_USER_UNIT_INFO_TO_SERVER_ACK, KPacketOK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_MY_USER_UNIT_INFO_TO_SERVER_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_NPC_UNIT_CREATE_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_NPC_UNIT_CREATE_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_NPC_UNIT_CREATE_ACK, KPacketOK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_NPC_UNIT_CREATE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_NPC_UNIT_CREATE_NOT, KEGS_NPC_UNIT_CREATE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	// ���� ������ ��Ʋ�ʵ忡 �´� ���� ���� �������� üũ!
	if( GetRoomUID() != 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		if( kPacket_.m_iBattleFieldID != GetMapID() )
			return;
	}

	SendPacket( EGS_NPC_UNIT_CREATE_NOT, kPacket_ );

	//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
	std::vector< int > vecNPCData;
	BOOST_TEST_FOREACH( KNPCUnitNot, kNPCData, kPacket_.m_vecNPCUnitAck )
	{
		int iNPCID = 0;
		iNPCID = kNPCData.m_kNPCUnitReq.m_NPCID;
		vecNPCData.push_back( iNPCID );
	}

	if( GetRoomUID() != 0  &&  ( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD ) )
	{
		m_kUserQuestManager.Handler_OnFindNPC( GetThisPtr<KGSUser>(), vecNPCData, false );
	}
	else if( GetRoomUID() != 0  && ( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON ) )
	{
		m_kUserQuestManager.Handler_OnFindNPC( GetThisPtr<KGSUser>(), vecNPCData, true );
	}
#endif SERV_REFORM_QUEST
	//}}
}

IMPL_ON_FUNC( EGS_NPC_UNIT_DIE_REQ )
{
	// [����] npc������ �ڻ��Ҷ� ������� ó�������� ���ؼ� ���� ���� ����� ã�ƾ� �ҵ�..

	// ���� npc������ �ڻ��� ��� DieNPCUID�� �ش� npcuid�� ������ �ִ� ��Ȳ, ���� droptable�� ���ó�� �Ǿ������� �������� �۵���� ��. (������ �������)

	if( kPacket_.m_nDieNPCUID == -1 )
	{
		START_LOG( clog, L"�����Ǵ� ���� ���� Ŭ���̾�Ʈ ��ƾ����" );

		return;
	}
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//{{ 2012. 12. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_CODE_CLEANUP_2012_12_07
	// [2012-12-07][������] RoomUID���� ���µ� ���� ��Ŷ�� ���ͼ����� ���� �ʿ䰡 ����.
	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"S_ROOM�����ε� RoomUID���� ����Ǿ����� �ʴ�!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetRoomUID() )
			<< END_LOG;
		return;
	}
#endif SERV_CODE_CLEANUP_2012_12_07
	//}}

	SendToCnRoom( ERM_NPC_UNIT_DIE_REQ, kPacket_ );
}

IMPL_ON_FUNC( ERM_NPC_UNIT_DIE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KERM_NPC_UNIT_DIE_ACK kPacket;
	kPacket.m_iOK	= kPacket_.m_iOK;
	SendPacket( EGS_NPC_UNIT_DIE_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_NPC_UNIT_DIE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_NPC_UNIT_DIE_NOT kPacketNpcDie;
	kPacketNpcDie.m_cAttUnitType	= kPacket_.m_cAttUnitType;
	kPacketNpcDie.m_EXPList			= kPacket_.m_EXPList;
	kPacketNpcDie.m_iNPCID			= kPacket_.m_iNPCID;
	kPacketNpcDie.m_nDieNPCUID		= kPacket_.m_nDieNPCUID;
	kPacketNpcDie.m_uiAttUnit		= kPacket_.m_uiAttUnit;

	//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	if( kPacket_.m_bIsPvpNpc )
	{
		m_kUserTitleManager.OnNpcUnitDie( 0, 0, 0, kPacket_.m_iNPCID, GetThisPtr<KGSUser>() );

		//{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
		m_kUserQuestManager.Handler_OnPVPKill( 
#ifdef SERV_NEW_PVP_QUEST_ERROR_MODIFY
											   kPacket_.m_iGameType
											   , GetThisPtr<KGSUser>()
											   , kPacket_.m_bIsPvpNpc	//{{ 2012. 02. 22	��μ�	���� ����Ʈ, PVP NPC ���� ����Ʈ	���� ����
#else
											   , CXSLRoom::PVP_GAME_TYPE::PGT_NULL
											   , GetThisPtr<KGSUser>()
											   , true						//{{ 2012. 02. 22	��μ�	���� ����Ʈ, PVP NPC ���� ����Ʈ	���� ����
#endif SERV_NEW_PVP_QUEST_ERROR_MODIFY
#ifdef PVP_QUEST_HERO_KILL_COUNT
											   , kPacket_.m_bHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
											   , 0
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
											   , 0
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
											   );
#endif SERV_NEW_PVP_QUEST
		//}}
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}

	//////////////////////////////////////////////////////////////////////////
	// ����Ʈ
	{
		m_kUserQuestManager.Handler_ERM_NPC_UNIT_DIE_NOT( kPacket_.m_iDungeonID, kPacket_.m_cDifficulty, kPacket_.m_iNPCID, GetThisPtr<KGSUser>(), kPacket_.m_cDungeonMode );
	}

	//////////////////////////////////////////////////////////////////////////
	// Īȣ �̼�
	{
		m_kUserTitleManager.OnNpcUnitDie( kPacket_.m_iDungeonID, kPacket_.m_cDifficulty, kPacket_.m_cDungeonMode, kPacket_.m_iNPCID, GetThisPtr<KGSUser>() );
	}

	// 1. NoDrop�̸� �ƹ� ó���� ����
	if( kPacket_.m_bNoDrop )
	{
		SendPacket( EGS_NPC_UNIT_DIE_NOT, kPacketNpcDie );
		return;
	}

	// 2. ���� ���� ����ġ�� �ֳ�?
	std::map< UidType, KEXPData >::iterator mit = kPacketNpcDie.m_EXPList.find( GetCharUID() );
	if( mit == kPacketNpcDie.m_EXPList.end() )
	{
		// ������� ����ġ�� ���ٸ� �ٷ� Ŭ���̾�Ʈ�� ����
		SendPacket( EGS_NPC_UNIT_DIE_NOT, kPacketNpcDie );
	}
	else
	{
		float fAddPartyFeverExpRate = 0.0f;
		
		if( SiKPartyListManager()->GetPartyFeverExpRate( GetPartyUID(), fAddPartyFeverExpRate ) == true )
		{
			int iPartyFeverEXP = static_cast<int>( (mit->second.m_iEXP) * fAddPartyFeverExpRate );
			iPartyFeverEXP += static_cast<int>( (mit->second.m_iPartyEXP) * fAddPartyFeverExpRate );

			// party fever ���ʽ� ����ġ�� ���Ͽɼ� ���ʽ��� ���Ѵ�
			mit->second.m_iSocketOptEXP += iPartyFeverEXP;
		}

#ifdef SERV_ANTI_ADDICTION_SYSTEM
		// �����ʿ�
		ApplyAntiAddictionRatio(kPacketNpcDie);
#endif SERV_ANTI_ADDICTION_SYSTEM

		// party fever���ʽ����� ó���Ϸ�ȵ� Ŭ���̾�Ʈ�� ����
		SendPacket( EGS_NPC_UNIT_DIE_NOT, kPacketNpcDie );

		// ����ġ ����
		//{{ �ڱ��� : [2010/03/09] //	�߱� �ߵ����� ��ħ�� �ý���
#ifdef SERV_ANTI_ADDICTION_SYSTEM
		m_kEXP.AddExp( static_cast<int>(mit->second.SumEXP() * m_AntiAddictionManager.GetRewardRatio()) );

		START_LOG( clog2, L"ERM_NPC_UNIT_DIE_NOT ��ħ�� ����� ����." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( m_AntiAddictionManager.GetRewardRatio() )
			<< END_LOG;
#else
		m_kEXP.AddExp( mit->second.SumEXP() );
#endif SERV_ANTI_ADDICTION_SYSTEM

		// ������ üũ
		CheckCharLevelUp();
	}

	//////////////////////////////////////////////////////////////////////////
	// ������ �Ҹ�
	if( kPacket_.m_sSubStageExpNpcNum > 0  &&  kPacket_.m_bDecreaseEndurance == true )
	{
		KEGS_DECREASE_ENDURANCE_NOT kNot;
		std::vector< KInventoryItemInfo > vecZeroEndurance;

		// ItemTemplet�� ������ ���������� ����ġ �ִ� npc�� ��ŭ �������� ��´�
		m_kInventory.DecreaseEquippedItemEnduranceInDungeon( kPacket_.m_sSubStageExpNpcNum, kPacket_.m_cUserCount, kNot.m_vecEnduranceUpdate, vecZeroEndurance );

		// �������� �Ҹ�Ǿ��ٸ� Ŭ���̾�Ʈ�� ������Ʈ�Ѵ�
		if( !kNot.m_vecEnduranceUpdate.empty() )
		{
			SendPacket( EGS_DECREASE_ENDURANCE_NOT, kNot );

			// ������ ��� �Ҹ�� �������� �ִٸ�
			if( GetRoomUID() > 0  &&  vecZeroEndurance.empty() == false )
			{
				switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
				{
				case CXSLRoom::RT_DUNGEON:
				case CXSLRoom::RT_TRAININGCENTER:
					{
						KERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT kPacketNot;
						kPacketNot.m_UnitUID = GetCharUID();
						GetRoomUserInfo( kPacketNot.m_kRoomUserInfo );
						kPacketNot.m_vecInventorySlotInfo = vecZeroEndurance;
						SendToCnRoom( ERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT, kPacketNot );
					}
					break;
				}
			}
		}
	}

#ifdef SERV_GLOBAL_MISSION_MANAGER
#ifdef SERV_SECOND_CLASS_WORLD_WAR_EVENT
	bool bSecondChangeJob = false;

	if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true )
	{
		bSecondChangeJob = true;
	}
	else if( _CONST_SECOND_CLASS_WORLD_WAR_EVENT::bWorldWarEventForElesis1stClass == true )
	{
		if( CXSLUnit::IsFirstChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true &&
			static_cast<CXSLUnit::UNIT_CLASS>( GetUnitType() ) == CXSLUnit::UT_ELESIS &&
			GetLevel() >= 35 )
		{
			bSecondChangeJob = true;
		}
	}

	if(kPacket_.m_uiAttUnit == GetCharUID() && bSecondChangeJob == true )
#else
	if(kPacket_.m_uiAttUnit == GetCharUID())
#endif SERV_SECOND_CLASS_WORLD_WAR_EVENT	
	{
		CTime tCurrentTime = CTime::GetCurrentTime();
		std::wstring wstrGlobalMissionDay = tCurrentTime.Format(_T("%Y-%m-%d"));
		std::wstring wstrGlobalMissionBeginDate = wstrGlobalMissionDay;
		wstrGlobalMissionBeginDate += L" 00:00:00";

		KEGS_GLOBAL_MISSION_UPDATE_NOT kInfoNot;
		SiKGSGlobalMissionManager()->GetGlobalMissionInfo( kInfoNot );
		
		std::map< std::wstring, KGlobalMissionInfo >::iterator mit = kInfoNot.m_mapGlobalMissionInfo.find(wstrGlobalMissionBeginDate);
		if( mit != kInfoNot.m_mapGlobalMissionInfo.end() )
		{
			if( mit->second.m_iMissionType == 1 && mit->second.m_iClearCondition == kPacket_.m_iNPCID )
			{
				KEGB_GLOBAL_MISSION_UPDATE_NOT kNot;
				kNot.m_wstrGlobalMissionDay = mit->first;
				kNot.m_iNowCount = 1;
				SendToGlobalServer( EGB_GLOBAL_MISSION_UPDATE_NOT, kNot );

				KDBE_REGIST_GLOBAL_MISSION_CLEAR_NOT kNot2;
				kNot2.m_wstrGlobalMissionDay = wstrGlobalMissionDay;
				kNot2.m_iUnitUID = GetCharUID();
				SendToGameDB( DBE_REGIST_GLOBAL_MISSION_CLEAR_NOT, kNot2 );
			}
		}
	}
#endif SERV_GLOBAL_MISSION_MANAGER

	//////////////////////////////////////////////////////////////////////////
	// �� ������
#ifdef SERV_PET_SYSTEM
	// �� ������ ������ ������ ���� �����̶� �����ϰ� �Ѵ�.
	if( kPacket_.m_sSubStageExpNpcNum > 0  &&  kPacket_.m_bDecreaseEndurance == true )
	{
		KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT kNot;

		// �� ������ ���� üũ!

		//{{ 2012. 04. 26	�ڼ���	�� �峭��
#ifdef SERV_PET_TOY
		if( m_kUserPetManager.DecreaseSatietyOnDungeonRoom( kPacket_.m_cUserCount, kPacket_.m_sSubStageExpNpcNum, m_kInventory.CheckPetToyItem(), kNot ) == true )
#else
		if( m_kUserPetManager.DecreaseSatietyOnDungeonRoom( kPacket_.m_cUserCount, kPacket_.m_sSubStageExpNpcNum, kNot ) == true )
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
#endif SERV_PET_SYSTEM
	//}}

	//////////////////////////////////////////////////////////////////////////
	// �ټ��� �Ҹ�
	if( kPacket_.m_bDecreaseEndurance == true )
	{
		bool bPlayWithTuto = false;

		// ���ڶ� ���� �÷��� �ߴ��� ã�ƺ���
		if( GetPartyUID() > 0  &&  m_kUserTutorialInfo.GetTutorUnitType() == KUserTutorialInfo::TUT_TEACHER )
		{
			std::vector< UidType > vecTutoUserList;
			m_kUserTutorialInfo.GetTutorUnitUIDList( vecTutoUserList );

			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecTutoUserList )
			{
				if( SiKPartyListManager()->IsPartyMember( GetPartyUID(), iUnitUID ) )
				{
					bPlayWithTuto = true;
					break;
				}
			}
		}

		if( DecreaseSpirit( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_cDifficulty), kPacket_.m_cUserCount, kPacket_.m_sSubStageExpNpcNum, bPlayWithTuto, false ) == true )
		{
			KEGS_DECREASE_SPIRIT_NOT kNot;
			kNot.m_iSpirit = m_kUserSpiritManager.GetSpirit();
			kNot.m_iHonorPoint = m_kUserGuildManager.GetHonorPoint();
#ifdef SERV_CHINA_SPIRIT_EVENT
			kNot.m_arrChinaSpirit[0] = GetChinaSpirit( 0 );
			kNot.m_arrChinaSpirit[1] = GetChinaSpirit( 1 );
			kNot.m_arrChinaSpirit[2] = GetChinaSpirit( 2 );
			kNot.m_arrChinaSpirit[3] = GetChinaSpirit( 3 );
			kNot.m_arrChinaSpirit[4] = GetChinaSpirit( 4 );
			kNot.m_arrChinaSpirit[5] = GetChinaSpirit( 5 );
#endif SERV_CHINA_SPIRIT_EVENT
			SendPacket( EGS_DECREASE_SPIRIT_NOT, kNot );
		}
	}
}

//{{ 2012. 11. 12	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
_IMPL_ON_FUNC( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, KERM_NPC_UNIT_DIE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_NPC_UNIT_DIE_NOT kPacketNpcDie;
	kPacketNpcDie.m_cAttUnitType	= kPacket_.m_cAttUnitType;
	kPacketNpcDie.m_EXPList			= kPacket_.m_EXPList;
	kPacketNpcDie.m_iNPCID			= kPacket_.m_iNPCID;
	kPacketNpcDie.m_nDieNPCUID		= kPacket_.m_nDieNPCUID;
	kPacketNpcDie.m_uiAttUnit		= kPacket_.m_uiAttUnit;
	
	// 1. NoDrop�̸� �ƹ� ó���� ����
	if( kPacket_.m_bNoDrop )
	{
		SendPacket( EGS_NPC_UNIT_DIE_NOT, kPacketNpcDie );
		return;
	}

	// 2. ���� ���� ����ġ�� �ֳ�?
	std::map< UidType, KEXPData >::iterator mit = kPacketNpcDie.m_EXPList.find( GetCharUID() );
	if( mit == kPacketNpcDie.m_EXPList.end() )
	{
		// ������� ����ġ�� ���ٸ� �ٷ� Ŭ���̾�Ʈ�� ����
		SendPacket( EGS_NPC_UNIT_DIE_NOT, kPacketNpcDie );
	}
	else
	{
#ifdef SERV_PC_BANG_TYPE
		bool bSpiritFree = ExistActivateBuff( CXSLBuffManager::BTI_BUFF_PREMIUM_PC_ROOM ) & SiKGameSysVal()->IsSpiritFree( GetPcBangType() );
#endif SERV_PC_BANG_TYPE

		// ��Ʋ �ʵ��� �ټ����� ������ ���͸� ��Ƶ� EXP �� ���� ���ϰ� �Ѵ�.
		if( m_kUserSpiritManager.GetSpirit() <= 0
		 && SiKGameEventManager()->CheckSpiritEvent() == false
#ifdef SERV_PC_BANG_TYPE
		 && bSpiritFree == false
#else SERV_PC_BANG_TYPE
		 && ExistActivateBuff( CXSLBuffManager::BTI_BUFF_PREMIUM_PC_ROOM ) == false
#endif SERV_PC_BANG_TYPE
		 )
		{
			mit->second.m_iEXP = 0;
			mit->second.m_iPartyEXP = 0;
			mit->second.m_iSocketOptEXP = 0;
			//{{ 2012. 12. 18	������	�ʵ忡���� ����ġ �̺�Ʈ ����
#ifdef SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
			mit->second.m_iEventBonusEXP = 0;
#endif SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
			//}}
		}

		float fAddPartyFeverExpRate = 0.0f;

		if( SiKPartyListManager()->GetPartyFeverExpRate( GetPartyUID(), fAddPartyFeverExpRate ) == true )
		{
			int iPartyFeverEXP = static_cast<int>( (mit->second.m_iEXP) * fAddPartyFeverExpRate );
			iPartyFeverEXP += static_cast<int>( (mit->second.m_iPartyEXP) * fAddPartyFeverExpRate );

			// party fever ���ʽ� ����ġ�� ���Ͽɼ� ���ʽ��� ���Ѵ�
			mit->second.m_iSocketOptEXP += iPartyFeverEXP;
		}

		// party fever���ʽ����� ó���Ϸ�ȵ� Ŭ���̾�Ʈ�� ����
		SendPacket( EGS_NPC_UNIT_DIE_NOT, kPacketNpcDie );

		// ����ġ ����
		m_kEXP.AddExp( mit->second.SumEXP() );

		// ������ üũ
		CheckCharLevelUp();
	}

	//////////////////////////////////////////////////////////////////////////
	// ������ �Ҹ�	
	if( kPacket_.m_sSubStageExpNpcNum > 0  &&  kPacket_.m_bDecreaseEndurance == true  &&  kPacket_.m_bQuestComplete )
	{
		KEGS_DECREASE_ENDURANCE_NOT kNot;
		std::vector< KInventoryItemInfo > vecZeroEndurance;

		// ��Ʋ�ʵ� ������ �Ҹ�
		m_kInventory.DecreaseEquippedItemEnduranceInBattleField( kPacket_.m_sSubStageExpNpcNum, kPacket_.m_cUserCount, kNot.m_vecEnduranceUpdate, vecZeroEndurance );

		// �������� �Ҹ�Ǿ��ٸ� Ŭ���̾�Ʈ�� ������Ʈ�Ѵ�
		if( !kNot.m_vecEnduranceUpdate.empty() )
		{
			SendPacket( EGS_DECREASE_ENDURANCE_NOT, kNot );

			// ������ ��� �Ҹ�� �������� �ִٸ�
			if( GetRoomUID() > 0  &&  vecZeroEndurance.empty() == false )
			{
				switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
				{
				case CXSLRoom::RT_BATTLE_FIELD:
					{
						KERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT kPacketNot;
						kPacketNot.m_UnitUID = GetCharUID();
						GetRoomUserInfo( kPacketNot.m_kRoomUserInfo );
						kPacketNot.m_vecInventorySlotInfo = vecZeroEndurance;
						SendToCnRoom( ERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT, kPacketNot );
					}
					break;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ����Ʈ
	if( kPacket_.m_bQuestComplete == true )
	{
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
		bool bHasFriend = false;

		if( GetPartyUID() > 0 )
		{
			// ��õ�� �ִ��� �켱 ã��
			std::vector< KRecommendUserInfo > vecRecruitUnitList;
			std::vector< UidType > vecRecruitUnitUID;
			m_kUserRecommendManager.GetRecruitUnitList( vecRecruitUnitList );

			if( vecRecruitUnitList.empty() == true )
			{
				m_kUserRecommendManager.GetRecruiterUnitList( vecRecruitUnitList );
			}

			BOOST_TEST_FOREACH( KRecommendUserInfo&, kRecruitUnitList, vecRecruitUnitList )
			{
				vecRecruitUnitUID.push_back( kRecruitUnitList.m_iUnitUID );
			}

			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecRecruitUnitUID )
			{
				if( SiKPartyListManager()->IsPartyMember( GetPartyUID(), iUnitUID ) )
				{
					bHasFriend = true;
					break;
				}
			}
		}
#endif SERV_RECRUIT_EVENT_SUBQUEST

		m_kUserQuestManager.Handler_ERM_NPC_UNIT_DIE_NOT( kPacket_.m_iDungeonID,
														kPacket_.m_cDifficulty,
														kPacket_.m_iNPCID,
														GetThisPtr<KGSUser>(),
														kPacket_.m_cDungeonMode
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
														, bHasFriend
#endif SERV_RECRUIT_EVENT_SUBQUEST
														);
	}

	//////////////////////////////////////////////////////////////////////////
	// Īȣ �̼�
	if( kPacket_.m_bQuestComplete == true )
	{
		m_kUserTitleManager.OnNpcUnitDie( kPacket_.m_iDungeonID, kPacket_.m_cDifficulty, kPacket_.m_cDungeonMode, kPacket_.m_iNPCID, GetThisPtr<KGSUser>() );
	}
	
	//////////////////////////////////////////////////////////////////////////
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	IF_EVENT_ENABLED( CEI_EVENT_COBO_DUNGEON_AND_FIELD )
	{
		std::map< UidType, KEXPData >::iterator mit = kPacketNpcDie.m_EXPList.find( GetCharUID() );
		if( mit != kPacketNpcDie.m_EXPList.end() )
		{
			if( mit->second.m_iEXP > 0 || mit->second.m_iEventBonusEXP > 0 || mit->second.m_iPartyEXP > 0 || mit->second.m_iSocketOptEXP > 0 )
			{
				FieldMonsterKillCountNot(kPacket_.m_iNPCID,kPacket_.m_uiAttUnit);
			}
		}
	}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
	// �� ������
#ifdef SERV_PET_SYSTEM
	// �� ������ ������ ������ ���� �����̶� �����ϰ� �Ѵ�.
	if( kPacket_.m_sSubStageExpNpcNum > 0  &&  kPacket_.m_bDecreaseEndurance == true  &&  kPacket_.m_bQuestComplete )
	{
		KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT kNot;

		// �� ������ ���� üũ!

		//{{ 2012. 04. 26	�ڼ���	�� �峭��
#ifdef SERV_PET_TOY
		if( m_kUserPetManager.DecreaseSatietyOnDungeonRoom( kPacket_.m_cUserCount, kPacket_.m_sSubStageExpNpcNum, m_kInventory.CheckPetToyItem(), kNot ) == true )
#else
		if( m_kUserPetManager.DecreaseSatietyOnDungeonRoom( kPacket_.m_cUserCount, kPacket_.m_sSubStageExpNpcNum, kNot ) == true )
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
				case CXSLRoom::RT_BATTLE_FIELD:
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
#endif SERV_PET_SYSTEM
	//}}

	//////////////////////////////////////////////////////////////////////////
	// �ټ��� �Ҹ�
	if( kPacket_.m_bDecreaseEndurance == true  &&  kPacket_.m_bQuestComplete == true )
	{
		bool bPlayWithTuto = false;

		//////////////////////////////////////////////////////////////////////////
		// ���ڶ� ���� �÷��� �ߴ��� ã�ƺ���
		if( GetPartyUID() > 0  &&  m_kUserTutorialInfo.GetTutorUnitType() == KUserTutorialInfo::TUT_TEACHER )
		{
			std::vector< UidType > vecTutoUserList;
			m_kUserTutorialInfo.GetTutorUnitUIDList( vecTutoUserList );

			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecTutoUserList )
			{
				if( SiKPartyListManager()->IsPartyMember( GetPartyUID(), iUnitUID ) )
				{
					bPlayWithTuto = true;
					break;
				}
			}
		}

		if( DecreaseSpirit( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_cDifficulty), kPacket_.m_cUserCount, kPacket_.m_sSubStageExpNpcNum, bPlayWithTuto, true ) == true )
		{
			KEGS_DECREASE_SPIRIT_NOT kNot;
			kNot.m_iSpirit = m_kUserSpiritManager.GetSpirit();
			kNot.m_iHonorPoint = m_kUserGuildManager.GetHonorPoint();
#ifdef SERV_CHINA_SPIRIT_EVENT
			kNot.m_arrChinaSpirit[0] = GetChinaSpirit( 0 );
			kNot.m_arrChinaSpirit[1] = GetChinaSpirit( 1 );
			kNot.m_arrChinaSpirit[2] = GetChinaSpirit( 2 );
			kNot.m_arrChinaSpirit[3] = GetChinaSpirit( 3 );
			kNot.m_arrChinaSpirit[4] = GetChinaSpirit( 4 );
			kNot.m_arrChinaSpirit[5] = GetChinaSpirit( 5 );
#endif SERV_CHINA_SPIRIT_EVENT
			SendPacket( EGS_DECREASE_SPIRIT_NOT, kNot );
		}
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

_IMPL_ON_FUNC( ERM_DROP_ITEM_NOT, KEGS_DROP_ITEM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	// ��Ʋ �ʵ��� �ټ����� ������ ���͸� ��Ƶ� �������� ���� ���ϰ� �Ѵ�.
	if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD  )
	{
		if( m_kUserSpiritManager.GetSpirit() <= 0 )
		{
			kPacket_.m_DropItemDataList.clear();
		}
	}

	SendPacket( EGS_DROP_ITEM_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_GET_ITEM_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_GET_ITEM_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_GET_ITEM_ACK, KEGS_GET_ITEM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_GET_ITEM_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_GET_ITEM_NOT, KEGS_GET_ITEM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//{{ �ڱ��� : [2010/03/09] //	�߱� �ߵ����� ��ħ�� �ý���
#ifdef SERV_ANTI_ADDICTION_SYSTEM
	if(m_AntiAddictionManager.IsEnable())
	{
		std::map< UidType, KDungeonRewardED >::iterator mit;
		mit = kPacket_.m_mapGetED.find( GetCharUID() );
		if( mit != kPacket_.m_mapGetED.end() )
		{
			mit->second.m_iED		= static_cast<int>(m_AntiAddictionManager.GetRewardRatio() * mit->second.m_iED);
			mit->second.m_iBonusED	= static_cast<int>(m_AntiAddictionManager.GetRewardRatio() * mit->second.m_iBonusED);
		}
	}
#endif SERV_ANTI_ADDICTION_SYSTEM

	SendPacket( EGS_GET_ITEM_NOT, kPacket_ );

	//{{ 2009. 5. 11  ������	�ǽð�ED
	if( !kPacket_.m_mapGetED.empty() )
	{
		std::map< UidType, KDungeonRewardED >::const_iterator mit;
		mit = kPacket_.m_mapGetED.find( GetCharUID() );
		if( mit != kPacket_.m_mapGetED.end() )
		{
			//{{ 2012. 10. 02	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			IncreaseED( mit->second.SumED(), KUserEDManager::ER_IN_ED_DUNGEON_DROP );
#else
//#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// �۾���¥: 2013-06-17	// �ڼ���
			const int iBeforeED = m_iED;
//#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG

			m_iED += mit->second.SumED();

//#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// �۾���¥: 2013-06-17	// �ڼ���
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_BATTLEFIELD_REWARD, iBeforeED );
//#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			//}}			

			{
#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// �۾���¥: 2013-05-08	// �ڼ���
				if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
				{
					// DB��� ED
					KStatisticsKey kKey;
					kKey.m_vecIntKey.push_back( 0 );
					KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PBattleField, mit->second.SumED() );

					// ���� ���
#ifdef SERV_USER_STATISTICS_RENEWAL
					m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_PBattleFieldED, mit->second.SumED() );
#else //SERV_USER_STATISTICS_RENEWAL
					m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_PBattleFieldED, mit->second.SumED() );
#endif //SERV_USER_STATISTICS_RENEWAL					
				}
				else
#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG
				{
					// DB��� ED
					KStatisticsKey kKey;
					kKey.m_vecIntKey.push_back( 0 );
					KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PDungeon, mit->second.SumED() );

					// ���� ���
#ifdef SERV_USER_STATISTICS_RENEWAL
					m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_PDungeon, mit->second.SumED() );
#else //SERV_USER_STATISTICS_RENEWAL
					m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_PDungeonED, mit->second.SumED() );
#endif //SERV_USER_STATISTICS_RENEWAL
				}
			}

			// ED ����� - ��Ϻ󵵰� ������ DB�� �����Ҽ� ������..? �̰Ŵ� ��Ƽ� ������Ʈ �ؾ߰ڴ�..
			//if( m_bIsEDAbuserLog )
			//{
			//	KELOG_ED_ABUSER_LOG_NOT kPacketEDLog;
			//	kPacketEDLog.m_iUnitUID  = GetCharUID();
			//	kPacketEDLog.m_iState	 = KAbuserLogManager::ELS_DUNGEON_REALTIME;
			//	kPacketEDLog.m_iBeforeED = iBeforeED;
			//	kPacketEDLog.m_iAfterED	 = m_iED;
			//	kPacketEDLog.m_iED		 = iED;
			//	SendToLogDB( ELOG_ED_ABUSER_LOG_NOT, kPacketEDLog );
			//}
		}
	}
	//}}
}

//{{ 2009. 5. 11  ������		�ǽð�������
_IMPL_ON_FUNC( ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, KEGS_GET_ITEM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-30	// �ڼ���
	int iAfterWorkStorageKey;
	if( m_kAfterWorkStorage.InsertData( kPacket_, iAfterWorkStorageKey ) == KAfterWorkStorage::AWS_SUCCEED )
	{
		if( ( SiCXSLEnchantItemManager()->CheckRandomEnchant( kPacket_.m_iItemID, CXSLEnchantItemManager::RET_DROP ) == true ) &&
			( GetKGameServer()->PopFirstTimeEnchantEvent() == true )
			)
		{
			KEGB_TIME_ENCHANT_EVENT_NOT kPacket;
			kPacket.m_iGSUID				= GetKGameServer()->GetUID();
			kPacket.m_iUnitUID				= GetCharUID();
			kPacket.m_iAfterWorkStorageKey	= iAfterWorkStorageKey;
			kPacket.m_usEventID				= EGB_TIME_ENCHANT_EVENT_DROP_NOT;
			SendToGlobalServer( EGB_TIME_ENCHANT_EVENT_NOT, kPacket );
			return;
		}
		else
		{
			KEGB_TIME_ENCHANT_EVENT_DROP_NOT kPacket;
			kPacket.m_iAfterWorkStorageKey = iAfterWorkStorageKey;
			KEventPtr spEvent( new KEvent );
			spEvent->SetData( PI_GS_USER, NULL, EGB_TIME_ENCHANT_EVENT_DROP_NOT, kPacket );
			QueueingEvent( spEvent );
			return;
		}
	}
	else
	{
		START_LOG( cerr, L"AfterWorkStorage: Key���� ���� �� �����ϴ�." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return;
	}
#else // SERV_TIME_ENCHANT_EVENT
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kPacket_.m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"������ ���ø��� ���� ����." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemID )
			<< END_LOG;
		return;
	}

	// ȹ�� ������
	std::map< int, KItemInfo > mapGetItem;
	std::map< int, int > mapInsertedItem;

	KItemInfo kItemInfo;
	kItemInfo.m_iItemID		  = kPacket_.m_iItemID;
	kItemInfo.m_cUsageType	  = pItemTemplet->m_PeriodType;
	kItemInfo.m_iQuantity	  = 1;
	kItemInfo.m_sEndurance	  = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ) ? pItemTemplet->m_Endurance : 0;
	//{{ 2013. 1. 14	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
	kItemInfo.m_cEnchantLevel = kPacket_.m_cEnchantLevel;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
	//}}

	//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	// ���� ��ȭ ���� ����
	SiCXSLEnchantItemManager()->RandomEnchant( kPacket_.m_iItemID
											 , CXSLEnchantItemManager::RET_DROP
											 , kItemInfo.m_cEnchantLevel
											 );

	// 9�� �̻� ��ȭ ������ ��ü ����!
	SendEnchantNotice( kPacket_.m_iItemID, kItemInfo.m_cEnchantLevel );

	// Ư�� ����� �������� ��� �к� ó��
	SiCXSLItemManager()->ItemSealProcess( kPacket_.m_iItemID
		, kItemInfo.m_sPeriod
		, kItemInfo.m_ucSealData
		);
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

	//{{ 2009. 11. 18  ������	Ư���ð�����̺�Ʈ
	if( kPacket_.m_cGetItemType == KEGS_GET_ITEM_NOT::GIT_DROP_EVENT_ITEM )
	{
		// �к� ������ �������̶�� �к���Ų��!
		if( SiCXSLItemManager()->IsCanSealForCashItem( kPacket_.m_iItemID ) == true )
		{
			kItemInfo.m_ucSealData = 100;
		}
	}
	//}}

	mapGetItem.insert( std::make_pair( kPacket_.m_iItemID, kItemInfo ) );

	KDBE_GET_ITEM_INSERT_TO_INVENTORY_REQ kPacketReq;
	m_kInventory.PrepareInsert( mapGetItem, mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo );

	if( mapInsertedItem.empty() )
	{
		//////////////////////////////////////////////////////////////////////////
		//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		AddTempInventoryItem( kPacket_.m_iItemID
							, kItemInfo.m_ucSealData
							//{{ 2012. 1. 16	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
							, kItemInfo
#endif SERV_ENCHANT_ITEM_DROP_EVENT
							//}}
							);
#else
		// �ӽ� �κ��丮�� �����Ϸ� ����!
		KEGS_GET_ITEM_TEMP_INVENTORY_NOT kNot;
		if( m_kTempInventory.AddTempInventoryItem( kPacket_.m_iItemID, kItemInfo.m_ucSealData, kPacket_.m_iDropItemUID, kNot.m_kTempInvenItem ) )
		{
			SendPacket( EGS_GET_ITEM_TEMP_INVENTORY_NOT, kNot );
		}
		else
		{
			START_LOG( cerr, L"�ӽ� �κ��丮�� ������ ���� ����!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iItemID )
				<< END_LOG;
		}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}

		//{{ 2010. 02. 23  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
#else
#pragma ELSE_MESSAGE
		// ������ ȭ�鿡 ������ �����ϴ°� �����ֱ�
		//if( GetPartyUID() > 0  &&  kPacket_.IsDungeonDropItem() )
		//{
		//	SiKPartyManager()->BroadCastToPartyMembers( GetPartyUID(), EGS_GET_ITEM_NOT, kPacket_ );
		//}
#endif SERV_CHANNEL_PARTY
		//}}

		return;
	}

	// DB������Ʈ
	if( !kPacketReq.m_vecItemInfo.empty() )
	{
		kPacketReq.m_iUnitUID = GetCharUID();
		kPacketReq.m_mapInsertItem = mapInsertedItem;
		kPacketReq.m_kBroadInfo = kPacket_;
		m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
		SendToGameDB( DBE_GET_ITEM_INSERT_TO_INVENTORY_REQ, kPacketReq );
		return;
	}

	//{{ 2010. 02. 23  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY		
#else
#pragma ELSE_MESSAGE
	// �ٸ� �����鿡�� ������ ȹ�� �˸���
	//if( GetPartyUID() > 0  &&  kPacket_.IsDungeonDropItem() )
	//{
	//	SiKPartyManager()->BroadCastToPartyMembers( GetPartyUID(), EGS_GET_ITEM_NOT, kPacket_ );
	//}
#endif SERV_CHANNEL_PARTY
	//}}

	// ���â�� ȹ���� �����۵��� �����ֱ� ���� ��������������� �濡 ������Ʈ
	if( GetRoomUID() > 0  &&  kPacket_.m_cGetItemType != KEGS_GET_ITEM_NOT::GIT_RESULT_ITEM )
	{
		KERM_GET_ITEM_COMPLETE_NOT kPacketToRoom;
		kPacketToRoom.m_iUnitUID = GetCharUID();
		kPacketToRoom.m_mapGetItem = mapInsertedItem;
		SendToCnRoom( ERM_GET_ITEM_COMPLETE_NOT, kPacketToRoom );
	}

	// Ŭ���̾�Ʈ �κ��丮�� ���� ������ ������Ʈ
	KEGS_GET_ITEM_REALTIME_NOT kPacket;
	kPacket.m_vecKInventorySlotInfo = kPacketReq.m_vecUpdatedInventorySlot;
	SendPacket( EGS_GET_ITEM_REALTIME_NOT, kPacket );

	//////////////////////////////////////////////////////////////////////////
	// ��� : Character Info, Item
	// ������ ���, DB ��� ī��Ʈ ���� ����
	int iDropItemCnt = 0;
	std::map< int, int >::const_iterator mit;
	for ( mit = mapInsertedItem.begin(); mit != mapInsertedItem.end(); ++mit )
	{
		const int iItemID = mit->first;
		const int iQuantity = mit->second;

		// ED���������� �˻�
		switch( iItemID )
		{
		case CXSLItem::EDI_BRONZE_ED:
		case CXSLItem::EDI_SILVER_ED:
		case CXSLItem::EDI_GOLD_ED:
			continue;
		}

		// �Ϲݾ������̶�� ī��Ʈ
		iDropItemCnt += iQuantity;

		// ��� : Local Item
		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( iItemID );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_DROP, iQuantity );
			//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_DROP, iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}
	}

	// ���� ��� ���� ������
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Obtained_Item, iDropItemCnt );

	//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	m_kUserAbuserManager.IncreaseGetItemCount( GetThisPtr<KGSUser>(), iDropItemCnt );
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}

	//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
	// ������ �����
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_DUNGEON, mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}

#endif // SERV_TIME_ENCHANT_EVENT
}

IMPL_ON_FUNC( DBE_GET_ITEM_INSERT_TO_INVENTORY_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"�ǽð� ������ DB������Ʈ ����! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )			
			<< END_LOG;

		// ����ٰ� ������Ʈ ������ ������ ��� ����!
		return;
	}

	// Ŭ���̾�Ʈ �κ��丮�� ���� ������ ������Ʈ
	KEGS_GET_ITEM_REALTIME_NOT kPacket;
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecKInventorySlotInfo );
	kPacket.m_vecKInventorySlotInfo.insert( kPacket.m_vecKInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

	//{{ 2010. 02. 23  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY		
#else
#pragma ELSE_MESSAGE
	// �ٸ� �����鿡�� ������ ȹ�� �˸���
	//if( GetPartyUID() > 0  &&  kPacket_.m_kBroadInfo.m_cGetItemType == KEGS_GET_ITEM_NOT::GIT_DROP_ITEM )
	//{
	//	SiKPartyManager()->BroadCastToPartyMembers( GetPartyUID(), EGS_GET_ITEM_NOT, kPacket_.m_kBroadInfo );
	//}
#endif SERV_CHANNEL_PARTY
	//}}

	// ���â�� ȹ���� �����۵��� �����ֱ� ���� ��������������� �濡 ������Ʈ
	if( GetRoomUID() > 0  &&  kPacket_.m_kBroadInfo.m_cGetItemType != KEGS_GET_ITEM_NOT::GIT_RESULT_ITEM )
	{
		KERM_GET_ITEM_COMPLETE_NOT kPacketToRoom;
		kPacketToRoom.m_iUnitUID = GetCharUID();
		kPacketToRoom.m_mapGetItem = kPacket_.m_mapInsertItem;
		SendToCnRoom( ERM_GET_ITEM_COMPLETE_NOT, kPacketToRoom );
	}

	// Ŭ���̾�Ʈ�� ��������� ������Ʈ	
	SendPacket( EGS_GET_ITEM_REALTIME_NOT, kPacket );

	//////////////////////////////////////////////////////////////////////////
	// ��� : Character Info, Item
	// ������ ���, DB ��� ī��Ʈ ���� ����
	int iDropItemCnt = 0;
	std::map< int, int >::const_iterator mit;
	for ( mit = kPacket_.m_mapInsertItem.begin(); mit != kPacket_.m_mapInsertItem.end(); ++mit )
	{
		const int iItemID = mit->first;
		const int iQuantity = mit->second;

		// ED���������� �˻�
		switch( iItemID )
		{
		case CXSLItem::EDI_BRONZE_ED:
		case CXSLItem::EDI_SILVER_ED:
		case CXSLItem::EDI_GOLD_ED:
			continue;
		}

		// �Ϲݾ������̶�� ī��Ʈ		
		iDropItemCnt += iQuantity;

		// ��� : Local Item
		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( iItemID );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_DROP, iQuantity );
			//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_DROP, iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}
	}

	// ���� ��� ���� ������
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Obtained_Item, iDropItemCnt );

	//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	m_kUserAbuserManager.IncreaseGetItemCount( GetThisPtr<KGSUser>(), iDropItemCnt );
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}

	//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
	// ������ �����
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_DUNGEON, kPacket_.m_mapInsertItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}

IMPL_ON_FUNC( EGS_GET_TEMP_ITEM_REQ )
{
	// �ݵ�� S_ROOM���¿����� �����ؾ���! ( ������, ������������� ���۰��� )

	//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_TEMP_ITEM_REQ, EGS_GET_TEMP_ITEM_ACK );
#else
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_GET_TEMP_ITEM_REQ, EGS_GET_TEMP_ITEM_ACK );
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}	

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_GET_TEMP_ITEM_ACK kAck;

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
			SendPacket( EGS_GET_TEMP_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_GET_TEMP_ITEM_ACK kPacket;
	std::map< int, KItemInfo > mapGetItem;
	std::map< int, int > mapInsertedItem;

	if( m_kTempInventory.PrepareTempItemToInventory( kPacket_.m_iTempItemUID, kPacket.m_iItemID, mapGetItem ) == false )
	{
		START_LOG( cerr, L"�ӽ� �κ� ������ �������� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iTempItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_11;
		SendPacket( EGS_GET_TEMP_ITEM_ACK, kPacket );
		return;
	}

	KDBE_GET_TEMP_ITEM_REQ kPacketReq;
	m_kInventory.PrepareInsert( mapGetItem, mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo );

	if( mapInsertedItem.empty() )
	{
		START_LOG( clog, L"�ӽ� �κ� �������� ���� �κ��丮 ������ ������." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iTempItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_12;
		SendPacket( EGS_GET_TEMP_ITEM_ACK, kPacket );
		return;
	}

	// �ӽ� �κ��� �ش� ������ ����ó��
	if( m_kTempInventory.DeleteTempItem( kPacket_.m_iTempItemUID ) == false )
	{
		START_LOG( cerr, L"�ӽ� �κ��� ������ ����� ����! �Ͼ���� �ȵǴ¿���!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iTempItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_11;
		SendPacket( EGS_GET_TEMP_ITEM_ACK, kPacket );
		return;
	}

	// DB������Ʈ
	if( !kPacketReq.m_vecItemInfo.empty() )
	{
		kPacketReq.m_iUnitUID = GetCharUID();
		kPacketReq.m_iTempItemUID = kPacket_.m_iTempItemUID;
		kPacketReq.m_mapInsertItem = mapInsertedItem;
		m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
		SendToGameDB( DBE_GET_TEMP_ITEM_REQ, kPacketReq );
		return;
	}

	// ���â�� ȹ���� �����۵��� �����ֱ� ���� ��������������� �濡 ������Ʈ
	if( GetRoomUID() > 0 )
	{
		KERM_GET_ITEM_COMPLETE_NOT kPacketToRoom;
		kPacketToRoom.m_iUnitUID = GetCharUID();
		kPacketToRoom.m_mapGetItem = mapInsertedItem;
		SendToCnRoom( ERM_GET_ITEM_COMPLETE_NOT, kPacketToRoom );
	}

	// Ŭ���̾�Ʈ �κ��丮�� ���� ������ ������Ʈ
	kPacket.m_iTempItemUID = kPacket_.m_iTempItemUID;
	kPacket.m_vecInventorySlotInfo = kPacketReq.m_vecUpdatedInventorySlot;
	SendPacket( EGS_GET_TEMP_ITEM_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////
	// ��� : Character Info, Item
	// ������ ���, DB ��� ī��Ʈ ���� ����
	int iDropItemCnt = 0;
	std::map< int, int >::const_iterator mit;
	for ( mit = mapInsertedItem.begin(); mit != mapInsertedItem.end(); ++mit )
	{
		const int iItemID = mit->first;
		const int iQuantity = mit->second;

		// ED���������� �˻�
		switch( iItemID )
		{
		case CXSLItem::EDI_BRONZE_ED:
		case CXSLItem::EDI_SILVER_ED:
		case CXSLItem::EDI_GOLD_ED:
			continue;
		}

		// �Ϲݾ������̶�� ī��Ʈ		
		iDropItemCnt += iQuantity;

		// ��� : Local Item
		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( iItemID );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_DROP, iQuantity );
			//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_DROP, iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}
	}

	// ���� ��� ���� ������
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Obtained_Item, iDropItemCnt );

	//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	m_kUserAbuserManager.IncreaseGetItemCount( GetThisPtr<KGSUser>(), iDropItemCnt );
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}

	//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
	// ������ �����
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_DUNGEON, mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}

IMPL_ON_FUNC( DBE_GET_TEMP_ITEM_ACK )
{
	//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_TEMP_ITEM_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_GET_TEMP_ITEM_ACK );
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}	

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"�ǽð� ������ DB������Ʈ ����! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )			
			<< END_LOG;

		// ����ٰ� ������Ʈ ������ ������ ��� ����!
		KEGS_GET_TEMP_ITEM_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_GET_TEMP_ITEM_ACK, kPacket );
		return;
	}

	// Ŭ���̾�Ʈ �κ��丮�� ���� ������ ������Ʈ
	KEGS_GET_TEMP_ITEM_ACK kPacket;	
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecInventorySlotInfo );
	kPacket.m_vecInventorySlotInfo.insert( kPacket.m_vecInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

	// ���â�� ȹ���� �����۵��� �����ֱ� ���� ��������������� �濡 ������Ʈ
	if( GetRoomUID() > 0 )
	{
		KERM_GET_ITEM_COMPLETE_NOT kPacketToRoom;
		kPacketToRoom.m_iUnitUID = GetCharUID();
		kPacketToRoom.m_mapGetItem = kPacket_.m_mapInsertItem;
		SendToCnRoom( ERM_GET_ITEM_COMPLETE_NOT, kPacketToRoom );
	}

	// Ŭ���̾�Ʈ�� ��������� ������Ʈ	
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iTempItemUID = kPacket_.m_iTempItemUID;
	SendPacket( EGS_GET_TEMP_ITEM_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////
	// ��� : Character Info, Item
	// ������ ���, DB ��� ī��Ʈ ���� ����
	int iDropItemCnt = 0;
	std::map< int, int >::const_iterator mit;
	for( mit = kPacket_.m_mapInsertItem.begin(); mit != kPacket_.m_mapInsertItem.end(); ++mit )
	{
		const int iItemID = mit->first;
		const int iQuantity = mit->second;

		// ED���������� �˻�
		switch( iItemID )
		{
		case CXSLItem::EDI_BRONZE_ED:
		case CXSLItem::EDI_SILVER_ED:
		case CXSLItem::EDI_GOLD_ED:
			continue;
		}

		// �Ϲݾ������̸� ī��Ʈ
		iDropItemCnt += iQuantity;

		// ��� : Local Item
		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( iItemID );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_DROP, iQuantity );
			//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_DROP, iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}
	}

	// ���� ��� ���� ������
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Obtained_Item, iDropItemCnt );

	//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	m_kUserAbuserManager.IncreaseGetItemCount( GetThisPtr<KGSUser>(), iDropItemCnt );
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}

	//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
	// ������ �����
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_DUNGEON, kPacket_.m_mapInsertItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}

IMPL_ON_FUNC( EGS_DELETE_TEMP_ITEM_REQ )
{
	//{{ 2011. 12. 14	������	���� ���� �α� ���� ����
#ifdef SERV_SERVER_ERROR_LOG_BUG_FIX
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_DELETE_TEMP_ITEM_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_SERVER_ERROR_LOG_BUG_FIX
	//}}

	if( m_kTempInventory.DeleteTempItem( kPacket_.m_iTempItemUID ) == false )
	{
		START_LOG( cwarn, L"�������� �ʴ� �ӽ� �κ� �������� �������� ����." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iTempItemUID )
			<< END_LOG;
	}

	KEGS_DELETE_TEMP_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iTempItemUID = kPacket_.m_iTempItemUID;
	SendPacket( EGS_DELETE_TEMP_ITEM_ACK, kPacket );
}
//}}

IMPL_ON_FUNC_NOPARAM( EGS_REFRESH_ROOM_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_REFRESH_ROOM_REQ );
}

_IMPL_ON_FUNC( ERM_REFRESH_ROOM_ACK, KEGS_REFRESH_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_REFRESH_ROOM_ACK, kPacket_ );
}

IMPL_ON_FUNC( ERM_CHECK_INVALID_ZOMBIE_USER_NOT )
{
	// �濡�� �濡 ������ ������ ������ �ǴܵǸ� ������ �̺�Ʈ!

	START_LOG( cout2, L"���ӹ� �������� �����ε� ���ӵǾ�����.!" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetUserName() )
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetStateIDString() )
		<< BUILD_LOG( GetRoomUID() )
		<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
		<< BUILD_LOG( GetTradeUID() )
		<< BUILD_LOG( GetPersonalShopUID() );

	if( GetRoomUID() != 0  &&  GetRoomUID() != kPacket_.m_iRoomUID )
	{
		START_LOG( cerr, L"���ӹ� ���������ε� �̹� �ٸ��濡 ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( kPacket_.m_iRoomUID )
			<< END_LOG;
	}

	if( GetStateID() == KGSFSM::S_ROOM )
	{
		StateTransition( KGSFSM::I_TO_FIELD_MAP );

		//SendUpdateUnitState( CXSLUnit::CUS_ONLINE );

		if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE  ||
			CXSLRoom::GetRoomType( kPacket_.m_iRoomUID ) == CXSLRoom::RT_SQUARE )
		{
			//leave square ������
			//KEGS_LEAVE_SQUARE_ACK kPacket;
			//kPacket.m_iOK = NetError::NET_OK;

			//SendPacket( EGS_LEAVE_SQUARE_ACK, kPacket );
			SendID( EGS_BAN_USER_NOT );

			START_LOG( cout2, L"���������ε� ���� ���� ���忡 ����.?" );
		}
		else
		{
			//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
			std::vector< int > vecDeleteBuff;
			std::vector< KBuffInfo > vecActivateBuff;

			if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
			{
				//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
				m_kUserBuffManager.OnLeaveDungeonRoom( GetThisPtr<KGSUser>(), vecActivateBuff, vecDeleteBuff, false );
#else
				m_kUserBuffManager.OnLeaveDungeonRoom( GetThisPtr<KGSUser>(), vecDeleteBuff );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
				//}
			}
			else if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
			{
				m_kUserBuffManager.OnLeaveBattleField( GetThisPtr<KGSUser>(), vecActivateBuff, vecDeleteBuff );
			}

			if( vecDeleteBuff.empty() == false )
			{
				// �α��� ���� ���� �Ŵ����� ������Ʈ
				KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
				kReq.m_iUnitUID = GetCharUID();
				kReq.m_vecDeActivateBuff = vecDeleteBuff;
				kReq.m_vecActivateBuff = vecActivateBuff;
				SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
			}
#endif SERV_SERVER_BUFF_SYSTEM
			//}}

			//{{ 2012. 11. 20	������		���� ��Ʋ�ʵ� üũ �� ����
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
			KEGS_CHECK_INVALID_ROOM_NOT	kPacket;
			kPacket.m_iOK = NetError::NET_OK;
			//{{ 2012. 11. 20	������		��Ʋ�ʵ忡�� ������ �̵�ó��
#ifdef SERV_CN_SERVER_SHUTDOWN_MOVE_TO_VILLAGE
			kPacket.m_iMapID = SiCXSLMapData()->GetPossibleEnterVillageMapID( GetLevel(), m_mapDungeonClear );
#else
			kPacket.m_iMapID = GetMapID();
#endif SERV_CN_SERVER_SHUTDOWN_MOVE_TO_VILLAGE
			//}}
			// Ŭ���̾�Ʈ�� �Ʒ� ��Ŷ �ް� �ʵ�� �̵��ؾ���!
			SendPacket( EGS_CHECK_INVALID_ROOM_NOT, kPacket );
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
			//}}

			KEGS_UPDATE_UNIT_INFO_NOT kNot;
			GetUnitInfo( kNot.m_kUnitInfo );
			kNot.m_bGameEnd = true;
			SendPacket( EGS_UPDATE_UNIT_INFO_NOT, kNot );
		}

		if( GetRoomUID() != 0  &&  GetRoomUID() == kPacket_.m_iRoomUID )
		{
			START_LOG( cout2, L"���������ε� ���� �����濡 ����.?" );
		}

		SetRoomUID( 0 );
	}

	if( GetTradeUID() != 0 )
	{
		KEGS_BREAK_TRADE_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
		_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );

		// ���� �� TradeUID�� 0�� �ȳ־�?
	}

	if( GetPersonalShopUID() != 0 )
	{
		//{{ 2011. 05. 02	������	�븮����
#ifdef SERV_PSHOP_AGENCY
		if( CXSLRoom::GetRoomType( GetPersonalShopUID() ) == CXSLRoom::RT_PERSONAL_SHOP )
		{
			KEGS_BREAK_PERSONAL_SHOP_REQ kPacket;
			kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_01;
			_SendToCnRoom( GetPersonalShopUID(), ERM_BREAK_PERSONAL_SHOP_REQ, kPacket );
		}
		else
		{
			KERM_LEAVE_PERSONAL_SHOP_REQ kPacket;
			kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_01;
			_SendToLoginRoom( GetPersonalShopUID(), ERM_LEAVE_PSHOP_AGENCY_REQ, kPacket );
		}
#else
		KEGS_BREAK_PERSONAL_SHOP_REQ kPacket;
		kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_01;
		_SendToCnRoom( GetPersonalShopUID(), ERM_BREAK_PERSONAL_SHOP_REQ, kPacket );
#endif SERV_PSHOP_AGENCY
		//}}

		SetPersonalShopUID( 0 );
	}
}

//���ͼ��� ��������...ó��
_IMPL_ON_FUNC( ERM_CHECK_INVALID_ROOM_NOT, UidType )
{
	// 1. ���� ���Ϳ��� ������ �濡 ������ �־��ٸ�?
	if( GetRoomUID() != 0  &&  KncUid::ExtractServerID( GetRoomUID() ) == kPacket_ )
	{
		KEGS_CHECK_INVALID_ROOM_NOT	kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		//{{ 2012. 11. 20	������		��Ʋ�ʵ忡�� ������ �̵�ó��
#ifdef SERV_CN_SERVER_SHUTDOWN_MOVE_TO_VILLAGE
		kPacket.m_iMapID = SiCXSLMapData()->GetPossibleEnterVillageMapID( GetLevel(), m_mapDungeonClear );
#else
		kPacket.m_iMapID = GetMapID();
#endif SERV_CN_SERVER_SHUTDOWN_MOVE_TO_VILLAGE
		//}}

		SetRoomUID( 0 );

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			StateTransition( KGSFSM::I_TO_FIELD_MAP );
		}

		// Ŭ���̾�Ʈ�� �Ʒ� ��Ŷ �ް� �ʵ�� �̵��ؾ���!
		SendPacket( EGS_CHECK_INVALID_ROOM_NOT, kPacket );

		// �濡�� ��Ż�ߴٸ� ��Ƽ ��Ż�� ���� ó��
		SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_02 );

		//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
#endif SERV_PVP_NEW_SYSTEM
		//}}

		//{{ 2012. 02. 10	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		SendLeaveAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_02 );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}

	// 2. ���� ���Ϳ��� ������ �ŷ��濡 �־��ٸ�?
	if( GetTradeUID() != 0  &&  KncUid::ExtractServerID( GetTradeUID() ) == kPacket_ )
	{
		KEGS_BREAK_TRADE_NOT kPacketNot;
		kPacketNot.m_iReason = NetError::NOT_REQUEST_TRADE_REPLY_04;
		SendPacket( EGS_BREAK_TRADE_NOT, kPacketNot );

		// ���Ͱ� �̹� �׾����� TradeUID���� 0���� �׳� �־�����!
		m_iTradeUID = 0;
	}

	// 3. ���� ���Ϳ��� ������ ���λ����濡 �־��ٸ�?
	if( GetPersonalShopUID() != 0  &&  KncUid::ExtractServerID( GetPersonalShopUID() ) == kPacket_ )
	{
		KEGS_BREAK_PERSONAL_SHOP_NOT kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_01;
		SendPacket( EGS_BREAK_PERSONAL_SHOP_NOT, kPacket );

		// ���Ͱ� �̹� �׾����� PersonalShopUID���� 0���� �׳� �־�����!
		m_iPersonalShopUID = 0;
	}

	//{{ 2010. 03. 09  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY

	// 4. ���� ���Ϳ��� ������ ��Ƽ�� �־��ٸ�?
	if( GetPartyUID() != 0  &&  KncUid::ExtractServerID( GetPartyUID() ) == kPacket_ )
	{
		//�������� ��Ƽ���� ������Ʈ.
		std::set< UidType > setEmpty;
		SendUpdatePartyMemberList( GetPartyUID(), setEmpty );

		// �������� ���� ������ ��Ż��Ű��!
		if( GetRoomUID() > 0 )
		{
			SendToCnRoom( ERM_PARTY_BAN_USER_REQ );
		}
		else
		{
			SendID( EGS_PARTY_BAN_USER_NOT );
		}

		// ���Ͱ� �̹� �׾����� PartyUID���� 0���� �־�����!
		SetPartyUID( 0 );
	}

#endif SERV_CHANNEL_PARTY
	//}}
}

//{{ 2012. 10. 17	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
IMPL_ON_FUNC( EGS_RESULT_SUCCESS_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_RESULT_SUCCESS_REQ, kPacket_ );
}
#else
IMPL_ON_FUNC_NOPARAM( EGS_RESULT_SUCCESS_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_RESULT_SUCCESS_REQ );
}
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
//}}

_IMPL_ON_FUNC( ERM_RESULT_SUCCESS_ACK, KEGS_RESULT_SUCCESS_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_DUNGEON:		
			{
				if( CheckingSpirit( kPacket_.m_iDungeonID ) == false )
				{
					// �ټ��� ������ ������ �����Ų��.
					kPacket_.m_iOK = NetError::NOT_LEAVE_ROOM_REASON_14;
				}
			}
			break;

		default:
			break;
		}
	}
	else
	{
		START_LOG( cerr, L"�濡 �������ִµ� RoomUID�� ����?" )
			<< BUILD_LOG( GetRoomUID() )
			<< END_LOG;
	}

	SendPacket( EGS_RESULT_SUCCESS_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_RESULT_ALL_UNIT_SUCCESS_NOT, KEGS_RESULT_ALL_UNIT_SUCCESS_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//////////////////////////////////////////////////////////////////////////
	// ���� ���⼭ �ش� ĳ������ ���� ������ �ʵ尡 �ƴ϶�� �ٷ� �ٽ� ������ ��Ŷ�� ���� ���� ����� �ʵ�� ���� �ϵ��� ó���Ѵ�!
	if( SEnum::IsBattleFieldID( GetMapID() ) == true )
	{
		m_kUserUnitManager.ReservedReturnToFieldInfo( kPacket_.m_kReturnToFieldInfo );
	}
	else
	{
		KERM_COMPLETE_RETURN_TO_FIELD_NOT kPacketNot;
		kPacketNot.m_kReturnToFieldInfo = kPacket_.m_kReturnToFieldInfo;
		SendToCnRoom( ERM_NEXT_RETURN_TO_FIELD_NOT, kPacketNot );
	}

	SendPacket( EGS_RESULT_ALL_UNIT_SUCCESS_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_SQUARE_LIST_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_SQUARE_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	//{{ 2011. 02. 06	������	�ŷ� ���� ���
#ifdef SERV_BLOCK_TRADE
	if( SiKGameSysVal()->IsBlockPersonalShop() == true )
	{
#ifdef SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_06;
#else // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_00;
#endif // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kPacket.m_nTotalPage = 1;
		kPacket.m_nViewPage = 1;
		kPacket.m_vecSquareInfo.clear();
		SendPacket( EGS_SQUARE_LIST_ACK, kPacket );
		return;
	}
#endif SERV_BLOCK_TRADE
	//}}

	//{{ 2011. 03. 02	������	���� �ŷ� ����
#ifdef SERV_HACK_USER_TRADE_BLOCK
	if( m_kUserAbuserManager.IsTradeBlockUser() == true )
	{
		if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
		{
#ifdef SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_06;
#else // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			kPacket.m_iOK = NetError::ERR_SQUARE_01;
#endif // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			kPacket.m_nTotalPage = 1;
			kPacket.m_nViewPage = 1;
			kPacket.m_vecSquareInfo.clear();
			SendPacket( EGS_SQUARE_LIST_ACK, kPacket );
			return;
		}
	}
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	if( m_kUserAbuserManager.IsTradeBlockUnit() == true )
	{
		kPacket.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_02;
		kPacket.m_nTotalPage = 1;
		kPacket.m_nViewPage = 1;
		kPacket.m_vecSquareInfo.clear();
		SendPacket( EGS_SQUARE_LIST_ACK, kPacket );
		return;
	}
#endif SERV_NEW_UNIT_TRADE_LIMIT

	if( SiKRoomListManager()->GetSquareInfoPage( kPacket_, kPacket ) == false )
	{
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_nViewPage )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
	}

	SendPacket( EGS_SQUARE_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_JOIN_SQUARE_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_JOIN_SQUARE_REQ, EGS_JOIN_SQUARE_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_JOIN_SQUARE_ACK );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	//{{ 2011. 02. 06	������	�ŷ� ���� ���
#ifdef SERV_BLOCK_TRADE
	if( SiKGameSysVal()->IsBlockPersonalShop() == true )
	{
		KEGS_JOIN_SQUARE_ACK kPacket;
#ifdef SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_06;
#else //SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_00;
#endif //SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		SendPacket( EGS_JOIN_SQUARE_ACK, kPacket );
		return;
	}
#endif SERV_BLOCK_TRADE
	//}}

	//{{ 2011. 03. 02	������	���� �ŷ� ����
#ifdef SERV_HACK_USER_TRADE_BLOCK
	if( m_kUserAbuserManager.IsTradeBlockUser() == true )
	{
		if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
		{
			KEGS_JOIN_SQUARE_ACK kPacket;
#ifdef SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_06;
#else // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			kPacket.m_iOK = NetError::ERR_SQUARE_01;
#endif // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			SendPacket( EGS_JOIN_SQUARE_ACK, kPacket );
			return;
		}
	}
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	if( m_kUserAbuserManager.IsTradeBlockUnit() == true )
	{
		KEGS_JOIN_SQUARE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_02;
		SendPacket( EGS_JOIN_SQUARE_ACK, kPacket );
		return;
	}
#endif SERV_NEW_UNIT_TRADE_LIMIT

#ifndef DEPRECATED_SERVER_GROUP_MASK

#ifdef SERV_IN_HOUSE
#else
	//{{ 2010. 05. 20  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( IsSameServerGroupUID( kPacket_.m_iSquareUID ) == false )
	{
		START_LOG( cerr, L"�ٸ� �������� ���忡�� ������ �� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iSquareUID )
			<< END_LOG;

		KEGS_JOIN_SQUARE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SQUARE_01;
		SendPacket( EGS_JOIN_SQUARE_ACK, kPacket );
		return;
	}
#endif SERV_INTEGRATION
	//}}
#endif SERV_IN_HOUSE

#endif DEPRECATED_SERVER_GROUP_MASK

	KERM_JOIN_SQUARE_REQ kPacket;
	GetSquareUserInfo( kPacket );

	_SendToCnRoom( kPacket_.m_iSquareUID, ERM_JOIN_SQUARE_REQ, kPacket );
}

_IMPL_ON_FUNC( ERM_JOIN_SQUARE_ACK, KEGS_JOIN_SQUARE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetRoomUID( kPacket_.m_kSquareInfo.m_iSquareUID );

		StateTransition( KGSFSM::I_TO_ROOM );
		//SendUpdateUnitState( CXSLUnit::CUS_SQUARE );

		// ���� ��� ���� ���� ī��Ʈ
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Square_Count, 1 );

		// ���� ���� Ÿ�̸�
		m_kTimer[TM_SQUARE_PLAY].restart();

		// ��Ƽ�� �ξ��� ������� ��Ƽ Ż��ó��
		SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_03 );

		//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
#endif SERV_PVP_NEW_SYSTEM
		//}}

		//{{ 2012. 02. 10	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		SendLeaveAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_02 );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}
	else
	{
		START_LOG_WITH_NAME( clog )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			;
	}

	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
	if( GetAuthLevel() < SEnum::UAL_GM || GetEDMonitoringMode() == false )
	{
		// ED ����͸� ���� �ƴϰų� ������ ������ ED ������ 0 ���� �ʱ�ȭ �Ѵ�.
		for( int i = 0 ; i < (int)kPacket_.m_vecUserInfo.size() ; ++i )
		{
			kPacket_.m_vecUserInfo[i].m_iED = 0;
		}
	}
#endif SERV_ED_MONITORING_IN_GAME
	//}}

	SendPacket( EGS_JOIN_SQUARE_ACK, kPacket_ );

	//////////////////////////////////////////////////////////////////////////
	// �ʵ忡�� ����~
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );

		//{{ 2009. 10. 8  ������	���
#ifdef GUILD_TEST
		SendUpdateGuildMemberInfo();
#endif GUILD_TEST
		//}}
	}
	//////////////////////////////////////////////////////////////////////////
}

_IMPL_ON_FUNC( ERM_JOIN_SQUARE_NOT, KEGS_JOIN_SQUARE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_JOIN_SQUARE_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_LEAVE_SQUARE_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_LEAVE_SQUARE_REQ, EGS_LEAVE_SQUARE_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_LEAVE_SQUARE_ACK );
#endif SERV_REPEAT_FILTER_REFAC	
	//}}

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( GetRoomUID() );

	SendToCnRoom( ERM_LEAVE_SQUARE_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_LEAVE_SQUARE_ACK, KEGS_LEAVE_SQUARE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetRoomUID( 0 );

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			StateTransition( KGSFSM::I_TO_FIELD_MAP );
		}

		//SendUpdateUnitState( CXSLUnit::CUS_ONLINE );

		// ���� ��� ���� �÷��� Ÿ�̸�
		int iSqaurePlayTime = static_cast<int>( m_kTimer[TM_SQUARE_PLAY].elapsed() + 0.5 );
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Square_PlayTime, iSqaurePlayTime );

		if( GetTradeUID() != 0 )
		{
			KEGS_BREAK_TRADE_REQ kPacket;
			kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
			_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );

			// ��? ����� TradeUID�� 0�� �ȳ־�?
		}

		if( GetPersonalShopUID() != 0 )
		{
			//{{ 2011. 05. 02	������	�븮����
#ifdef SERV_PSHOP_AGENCY
			if( CXSLRoom::GetRoomType( GetPersonalShopUID() ) == CXSLRoom::RT_PERSONAL_SHOP )
			{
				KEGS_BREAK_PERSONAL_SHOP_REQ kPacket;
				kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_18;
				_SendToCnRoom( GetPersonalShopUID(), ERM_BREAK_PERSONAL_SHOP_REQ, kPacket );
			}
			else
			{
				KERM_LEAVE_PERSONAL_SHOP_REQ kPacket;
				kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_18;
				_SendToLoginRoom( GetPersonalShopUID(), ERM_LEAVE_PSHOP_AGENCY_REQ, kPacket );
			}
#else
			KEGS_BREAK_PERSONAL_SHOP_REQ kPacket;
			kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_18;
			_SendToCnRoom( GetPersonalShopUID(), ERM_BREAK_PERSONAL_SHOP_REQ, kPacket );
#endif SERV_PSHOP_AGENCY
			//}}

			SetPersonalShopUID( 0 );
		}
	}

	SendPacket( EGS_LEAVE_SQUARE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_LEAVE_SQUARE_NOT, KEGS_LEAVE_SQUARE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_LEAVE_SQUARE_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_SQUARE_UNIT_SYNC_DATA_REQ )
{
	//{{ 2010. 03. 17  ������	���� ���� ����
	VERIFY_STATE_WARN( ( 1, KGSFSM::S_ROOM ) );
	//}}

	SendToCnRoom( ERM_SQUARE_UNIT_SYNC_DATA_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_SQUARE_UNIT_SYNC_DATA_NOT, KEGS_SQUARE_UNIT_SYNC_DATA_NOT )
{
	//{{ 2010. 03. 17  ������	���� ���� ����
	//VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
	if( GetStateID() != KGSFSM::S_ROOM )
	{
		if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE )
		{
			START_LOG( cerr, L"���� ����. [�ŷ�����] Name : " << m_strName )
				<< L"    ���� ���� : " << GetStateIDString() << dbg::endl
				<< L"    ��� ���� : ( 1, KGSFSM::S_ROOM )"
				<< END_LOG;
		}
		else
		{
			START_LOG( cerr, L"���� ����. Name : " << m_strName )
				<< L"    ���� ���� : " << GetStateIDString() << dbg::endl
				<< L"    ��� ���� : ( 1, KGSFSM::S_ROOM )"
				<< END_LOG;
		}
		return;
	}
	//}}

	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
	if( GetAuthLevel() < SEnum::UAL_GM || GetEDMonitoringMode() == false )
	{
		kPacket_.m_iED = 0;
	}
#endif SERV_ED_MONITORING_IN_GAME
	//}}

	SendPacket( EGS_SQUARE_UNIT_SYNC_DATA_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_CREATE_TC_ROOM_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_CREATE_TC_ROOM_ACK kPacketAck;
	KERM_OPEN_TC_ROOM_REQ kPacket;

	if( SiKTrainingCenterTable()->CheckIfEnter( m_mapTCClear, kPacket_.m_iTCID ) == false )
	{
		START_LOG( cerr, L"���������� �����ʴ� �ƷüҸ� �����ҷ�����.!" )
			<< BUILD_LOG( kPacket_.m_iTCID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_TC_01;

		goto end_proc;
	}

	kPacket.m_kTCInfo.m_iTCID		= kPacket_.m_iTCID;	
	kPacket.m_kTCInfo.m_iDungeonID	= SiKTrainingCenterTable()->GetDungeonID( kPacket_.m_iTCID );
	kPacket.m_kTCInfo.m_fPlayTime	= SiKTrainingCenterTable()->GetPlayTime( kPacket_.m_iTCID );
	kPacket.m_kTCInfo.m_cRoomType	= CXSLRoom::RT_TRAININGCENTER;

	if( kPacket.m_kTCInfo.m_iDungeonID <= 0 )
	{
		START_LOG( cerr, L"�ش� �Ʒüҿ� ���� �������̵� ���� ���� +��+;" )
			<< BUILD_LOG( kPacket_.m_iTCID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacketAck.m_iOK	= NetError::ERR_TC_03;

		goto end_proc;
	}

	GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_TRAININGCENTER );

	START_LOG( clog2, L"���� ���, ���� ����" )
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( KBaseServer::GetKObj()->GetUID() )
		;

	SendToCnRoom( ERM_OPEN_TC_ROOM_REQ, kPacket );
	return;

	//kPacketAck.m_iOK = NetError::ERR_TC_02;

end_proc:
	SendPacket( EGS_CREATE_TC_ROOM_ACK, kPacketAck );
}

_IMPL_ON_FUNC( ERM_OPEN_TC_ROOM_ACK, KEGS_CREATE_TC_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetRoomUID( kPacket_.m_kTCInfo.m_RoomUID );
		StateTransition( KGSFSM::I_TO_ROOM );
		m_kTimer[TM_ROOM].restart();

		if( GetRoomUID() != 0 )
		{
			if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_TRAININGCENTER )
			{
				//SendUpdateUnitState( CXSLUnit::CUS_TC_PLAY );
			}
		}

		// ��Ƽ�� �ξ��� ������� ��Ƽ Ż��ó��
		SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_04 );

		//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
#endif SERV_PVP_NEW_SYSTEM
		//}}

		//{{ 2012. 02. 10	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		SendLeaveAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_02 );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		//{{ 2008. 12. 29	������	�Ʒü� ����� �ʵ� ��Ż
		SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );
		//}}
	}
	else
	{
		START_LOG( cerr, L"����� ����.!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}

	SendPacket( EGS_CREATE_TC_ROOM_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_END_TC_GAME_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_END_TC_GAME_REQ, kPacket_ );
}

IMPL_ON_FUNC( ERM_END_TC_GAME_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_END_TC_GAME_ACK kPacket;
	kPacket.m_iOK			= kPacket_.m_iOK;
	kPacket.m_bIsSuccess	= kPacket_.m_bIsSuccess;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( kPacket_.m_bIsSuccess == true )
		{
			int iED, iEXP;
			iED		= 0;
			iEXP	= 0;

			if( m_mapTCClear.find( kPacket_.m_iTCID ) == m_mapTCClear.end() )
			{
				// ���� ���̵��� Ŭ�����ߴ��� �˻�(���� ���鶧 �˻������� �׷��� �ѹ���)
				if( SiKTrainingCenterTable()->CheckIfEnter( m_mapTCClear, kPacket_.m_iTCID ) == true )
				{
					KTCClearInfo kTCInfo;
					kTCInfo.m_iTCID = kPacket_.m_iTCID;
					CTime tCurrentTime = CTime::GetCurrentTime();
					kTCInfo.m_wstrClearTime = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
					kTCInfo.m_bNew = true;
					m_mapTCClear.insert( std::make_pair( kTCInfo.m_iTCID, kTCInfo ) );
				}

				//����ó���� �ѹ��� �ϱ� ���ؼ�.
				if( SiKTrainingCenterTable()->GetReward( kPacket_.m_iTCID, iED, iEXP ) == true )
				{
					//{{ 2012. 10. 02	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
					IncreaseED( iED, KUserEDManager::ER_IN_ED_TRAINING_REWARD );
#else
					int iBeforeED = m_iED;

					m_iED		+= iED;

					//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
					//#ifdef SERV_USER_ABUSER_MANAGER
					// ED �����
					m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_TRAINING_REWARD, iBeforeED );
					//#endif SERV_USER_ABUSER_MANAGER
					//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
					//}}

					// DB��� ED
					KStatisticsKey kKey;
					kKey.m_vecIntKey.push_back( 0 );
					KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PTraining, iED );
#ifdef SERV_USER_STATISTICS_RENEWAL
					m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_PTraining, iED );
#endif //SERV_USER_STATISTICS_RENEWAL

					//{{ 2009. 1. 20  ������	��������ġ ����ó��
					m_kEXP.AddExp( iEXP );
					//}}

					CheckCharLevelUp();					
				}
				else
				{
					kPacket.m_iOK	= NetError::ERR_TC_04;
				}
			}

			GetUnitInfo( kPacket.m_kUnitInfo );
		}
	}

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.20 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
	m_bTimeControlItemCheckDungeonPlay = false;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING
	SetRoomUID( 0 );

	//�Ʒü� ����� �����ص� ���� ������� ������ ���¸� ä�η� �ٲ۴�.
	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	SendPacket( EGS_END_TC_GAME_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_SET_TC_REMAINING_TIME_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_SET_TC_REMAINING_TIME_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_SET_TC_REMAINING_TIME_ACK, KEGS_SET_TC_REMAINING_TIME_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_SET_TC_REMAINING_TIME_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_LEAVE_TC_ROOM_ACK, KEGS_LEAVE_TC_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetRoomUID( 0 );

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			StateTransition( KGSFSM::I_TO_FIELD_MAP );
		}

		//SendUpdateUnitState( CXSLUnit::CUS_ONLINE );	
	}

	KEGS_LEAVE_ROOM_ACK kAck;
	kAck.m_iOK = kPacket_.m_iOK;

	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	std::vector< int > vecDeleteBuff;
	std::vector< KBuffInfo > vecActivateBuff;

	if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
	{
		//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		m_kUserBuffManager.OnLeaveDungeonRoom( GetThisPtr<KGSUser>(), vecActivateBuff, vecDeleteBuff, false );
#else
		m_kUserBuffManager.OnLeaveDungeonRoom( GetThisPtr<KGSUser>(), vecDeleteBuff );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
		//}
	}
	else if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		m_kUserBuffManager.OnLeaveBattleField( GetThisPtr<KGSUser>(), vecActivateBuff, vecDeleteBuff );
	}

	if( vecDeleteBuff.empty() == false )
	{
		// �α��� ���� ���� �Ŵ����� ������Ʈ
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
		kReq.m_iUnitUID = GetCharUID();
		kReq.m_vecDeActivateBuff = vecDeleteBuff;
		kReq.m_vecActivateBuff = vecActivateBuff;
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}}
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.20 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
	m_bTimeControlItemCheckDungeonPlay = false;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

	SendPacket( EGS_LEAVE_ROOM_ACK, kAck );
}

IMPL_ON_FUNC( EGS_ADD_ON_STAT_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_ADD_ON_STAT_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_ADD_ON_STAT_ACK, KEGS_ADD_ON_STAT_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_ADD_ON_STAT_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_ADD_ON_STAT_NOT, KEGS_ADD_ON_STAT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_ADD_ON_STAT_NOT, kPacket_ );
}

//{{ 2009. 5. 21  ������		������ �Ҹ� ����
IMPL_ON_FUNC( ERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) )

		KEGS_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT kPacket;
	kPacket.m_UnitUID			   = kPacket_.m_UnitUID;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;
	kPacket.m_kGameStat			   = kPacket_.m_kRoomUserInfo.m_kGameStat;	
	SendPacket( EGS_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT, kPacket );
}
//}}

//{{ 2009. 9. 23  ������	1������ĳ����
_IMPL_ON_FUNC( ERM_CHANGE_ROOM_USER_INFO_NOT, KRoomUserInfo )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_CHANGE_USER_INFO_NOT kPacket;
	kPacket.m_iUnitUID	 = kPacket_.m_nUnitUID;
	kPacket.m_cUnitClass = kPacket_.m_cUnitClass;
	//{{ 2009. 10. 9  ������	���
#ifdef GUILD_TEST
	kPacket.m_wstrGuildName = kPacket_.m_wstrGuildName;
#endif GUILD_TEST
	//}}
	SendPacket( EGS_CHANGE_USER_INFO_NOT, kPacket );
}

_IMPL_ON_FUNC( ERM_CHANGE_SQUARE_USER_INFO_NOT, KSquareUserInfo )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_CHANGE_USER_INFO_NOT kPacket;
	kPacket.m_iUnitUID	 = kPacket_.m_iUnitUID;
	kPacket.m_cUnitClass = kPacket_.m_cUnitClass;
	//{{ 2009. 10. 9  ������	���
#ifdef GUILD_TEST
	kPacket.m_wstrGuildName = kPacket_.m_wstrGuildName;
#endif GUILD_TEST
	//}}
	SendPacket( EGS_CHANGE_USER_INFO_NOT, kPacket );
}
//}}

//{{ 2011. 03. 26  ��μ�	û 1�� �߰� ��ų �߰� - ����
#ifdef SERV_ADD_BUFF_SKILL_INFO
IMPL_ON_FUNC( EGS_BUFF_SKILL_INFO_CONVEY_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_BUFF_SKILL_INFO_CONVEY_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_BUFF_SKILL_INFO_CONVEY_ACK, KEGS_BUFF_SKILL_INFO_CONVEY_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_BUFF_SKILL_INFO_CONVEY_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_BUFF_SKILL_INFO_CONVEY_NOT, KEGS_BUFF_SKILL_INFO_CONVEY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_BUFF_SKILL_INFO_CONVEY_NOT, kPacket_ );
}
#endif SERV_ADD_BUFF_SKILL_INFO
//}}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
_IMPL_ON_FUNC( ERM_CREATE_ATTRIB_NPC_NOT, KEGS_CREATE_ATTRIB_NPC_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CREATE_ATTRIB_NPC_NOT, kPacket_ );

	//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
	std::vector< int > vecNPCData;
	BOOST_TEST_FOREACH( KAttribNpcUnitInfo, kNPCData, kPacket_.m_vecAttribNPCUnit )
	{
		int iNPCID = 0;
		iNPCID = kNPCData.m_sNPCUnitReq.m_NPCID;
		vecNPCData.push_back( iNPCID );
	}

	if( GetRoomUID() != 0  &&  ( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD ) )
	{
		m_kUserQuestManager.Handler_OnFindNPC( GetThisPtr<KGSUser>(), vecNPCData, false );
	}
	else if( GetRoomUID() != 0  && ( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON ) )
	{
		m_kUserQuestManager.Handler_OnFindNPC( GetThisPtr<KGSUser>(), vecNPCData, true );
	}
#endif SERV_REFORM_QUEST
	//}}
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
IMPL_ON_FUNC( ERM_ACTIVATE_BUFF_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	std::vector< KBuffInfo > vecActiveBuff;
	BOOST_TEST_FOREACH( const KBuffInfo&, kBuffInfo, kPacket_.m_vecActivateBuffList )
	{
		if( ActivateBuff( kBuffInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID, vecActiveBuff ) == false )
		{
			START_LOG( cerr, L"���� ���� Ȱ��ȭ ����!" )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( kBuffInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
		}
	}

	if( vecActiveBuff.empty() == false )
	{
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
		kReq.m_iUnitUID = GetCharUID();
		kReq.m_vecActivateBuff = vecActiveBuff;
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
}
#endif SERV_SERVER_BUFF_SYSTEM
//}}

//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
_IMPL_ON_FUNC( ERM_UPDATE_BUFF_INFO_IN_ROOM_NOT, KEGS_UPDATE_BUFF_INFO_IN_ROOM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"�� �����ε� RoomUID�� ����? �̻��ϴ�. �ϴ� ���� ����" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
	}

	SendPacket( EGS_UPDATE_BUFF_INFO_IN_ROOM_NOT, kPacket_ );
}

IMPL_ON_FUNC( ECN_PARTY_BUFF_UPDATE_NOT )
{
	std::vector<KBuffInfo> vecActivateBuff;
	std::vector<int> vecDeleteBuff;

	std::map<int,KBuffInfo>::iterator mit = kPacket_.m_mapActivateBuffList.begin();
	for(  ; mit != kPacket_.m_mapActivateBuffList.end() ; ++mit )
	{
		ActivateBuff( mit->first, vecActivateBuff, mit->second.m_iBuffStartTime );
	}

	std::set<int>::iterator sit = kPacket_.m_setDeactivateBuffList.begin();
	for(  ; sit != kPacket_.m_setDeactivateBuffList.end() ; ++sit )
	{
		DeactivateBuff( (*sit), vecDeleteBuff );
	}
	
	if( vecActivateBuff.empty() == false || vecDeleteBuff.empty() == false )
	{
		// �α��� ���� ���� �Ŵ����� ������Ʈ
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
		kReq.m_iUnitUID = GetCharUID();
		kReq.m_vecActivateBuff = vecActivateBuff;
		kReq.m_vecDeActivateBuff = vecDeleteBuff;
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
}
#endif SERV_SERVER_BUFF_SYSTEM
//}

//{{ 2013. 03. 12	������	���� ��ȿ �˻�
#ifdef SERV_CHECK_EXIST_MONSTER_UID
IMPL_ON_FUNC( EGS_CHECK_EXIST_MONSTER_UID_REQ )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_CHECK_EXIST_MONSTER_UID_ACK );

    if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"�濡 �ִ°͵� �ƴѵ� �� ��Ŷ�� ���³�? �ð��� �����ΰ�?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_CHECK_EXIST_MONSTER_UID_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::NET_OK;
		SendPacket( EGS_CHECK_EXIST_MONSTER_UID_ACK, kPacketAck );
		return;
	}

	SendToCnRoom( ERM_CHECK_EXIST_MONSTER_UID_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHECK_EXIST_MONSTER_UID_ACK, KEGS_CHECK_EXIST_MONSTER_UID_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_CHECK_EXIST_MONSTER_UID_ACK );

	SendPacket( EGS_CHECK_EXIST_MONSTER_UID_ACK, kPacket_ );
}
#endif SERV_CHECK_EXIST_MONSTER_UID
//}}

//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
IMPL_ON_FUNC( EGS_UDP_PING_SCORE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0 )
	{
		START_LOG( clog, L"�濡 ������ ���°� �ƴѵ� UDP PING SCORE��Ŷ�� ���´�" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return;
	}

	SendToCnRoom( ERM_UDP_PING_SCORE_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHECK_PING_SCORE_CHANGE_HOST_NOT, KEGS_CHECK_PING_SCORE_CHANGE_HOST_NOT )
{
	SendPacket( EGS_CHECK_PING_SCORE_CHANGE_HOST_NOT, kPacket_ );	
}
#endif SERV_CHOOSE_FASTEST_HOST
//}

#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-30	// �ڼ���
IMPL_ON_FUNC( EGB_TIME_ENCHANT_EVENT_DROP_NOT )
{
	if( kPacket_.m_iAfterWorkStorageKey == 0 )
	{
		START_LOG( cerr, L"AfterWorkStorage: Key���� �̻��մϴ�." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iAfterWorkStorageKey )
			<< END_LOG;
		return;
	}

	KEGS_GET_ITEM_NOT kStorage;
	if( m_kAfterWorkStorage.RetrieveData( kPacket_.m_iAfterWorkStorageKey, kStorage ) != KAfterWorkStorage::AWS_SUCCEED )
	{
		START_LOG( cerr, L"AfterWorkStorage: �������� ����" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return;
	}

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kStorage.m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"������ ���ø��� ���� ����." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kStorage.m_iItemID )
			<< END_LOG;
		return;
	}

	// ȹ�� ������
	std::map< int, KItemInfo > mapGetItem;
	std::map< int, int > mapInsertedItem;

	KItemInfo kItemInfo;
	kItemInfo.m_iItemID		  = kStorage.m_iItemID;
	kItemInfo.m_cUsageType	  = pItemTemplet->m_PeriodType;
	kItemInfo.m_iQuantity	  = 1;
	kItemInfo.m_sEndurance	  = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ) ? pItemTemplet->m_Endurance : 0;
	//{{ 2013. 1. 14	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
	kItemInfo.m_cEnchantLevel = kStorage.m_cEnchantLevel;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
	//}}

	//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	// ���� ��ȭ ���� ����
	SiCXSLEnchantItemManager()->RandomEnchant( kStorage.m_iItemID
		, CXSLEnchantItemManager::RET_DROP
		, kPacket_.m_cTimeEnchantEventLevel
		, kItemInfo.m_cEnchantLevel
		);

	// 9�� �̻� ��ȭ ������ ��ü ����!
	SendEnchantNotice( kStorage.m_iItemID, kItemInfo.m_cEnchantLevel );

	// Ư�� ����� �������� ��� �к� ó��
	SiCXSLItemManager()->ItemSealProcess( kStorage.m_iItemID
		, kItemInfo.m_sPeriod
		, kItemInfo.m_ucSealData
		);
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

	//{{ 2009. 11. 18  ������	Ư���ð�����̺�Ʈ
	if( kStorage.m_cGetItemType == KEGS_GET_ITEM_NOT::GIT_DROP_EVENT_ITEM )
	{
		// �к� ������ �������̶�� �к���Ų��!
		if( SiCXSLItemManager()->IsCanSealForCashItem( kStorage.m_iItemID ) == true )
		{
			kItemInfo.m_ucSealData = 100;
		}
	}
	//}}

	mapGetItem.insert( std::make_pair( kStorage.m_iItemID, kItemInfo ) );

	KDBE_GET_ITEM_INSERT_TO_INVENTORY_REQ kPacketReq;
	m_kInventory.PrepareInsert( mapGetItem, mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo );

	if( mapInsertedItem.empty() )
	{
		//////////////////////////////////////////////////////////////////////////
		//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		AddTempInventoryItem( kStorage.m_iItemID
			, kItemInfo.m_ucSealData
			//{{ 2012. 1. 16	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
			, kItemInfo
#endif SERV_ENCHANT_ITEM_DROP_EVENT
			//}}
			);
#else
		// �ӽ� �κ��丮�� �����Ϸ� ����!
		KEGS_GET_ITEM_TEMP_INVENTORY_NOT kNot;
		if( m_kTempInventory.AddTempInventoryItem( kStorage.m_iItemID, kItemInfo.m_ucSealData, kStorage.m_iDropItemUID, kNot.m_kTempInvenItem ) )
		{
			SendPacket( EGS_GET_ITEM_TEMP_INVENTORY_NOT, kNot );
		}
		else
		{
			START_LOG( cerr, L"�ӽ� �κ��丮�� ������ ���� ����!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kStorage.m_iItemID )
				<< END_LOG;
		}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}

		//{{ 2010. 02. 23  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
#else
#pragma ELSE_MESSAGE
		// ������ ȭ�鿡 ������ �����ϴ°� �����ֱ�
		//if( GetPartyUID() > 0  &&  kStorage.IsDungeonDropItem() )
		//{
		//	SiKPartyManager()->BroadCastToPartyMembers( GetPartyUID(), EGS_GET_ITEM_NOT, kStorage );
		//}
#endif SERV_CHANNEL_PARTY
		//}}

		return;
	}

	// DB������Ʈ
	if( !kPacketReq.m_vecItemInfo.empty() )
	{
		kPacketReq.m_iUnitUID = GetCharUID();
		kPacketReq.m_mapInsertItem = mapInsertedItem;
		kPacketReq.m_kBroadInfo = kStorage;
		m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
		SendToGameDB( DBE_GET_ITEM_INSERT_TO_INVENTORY_REQ, kPacketReq );
		return;
	}

	//{{ 2010. 02. 23  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY		
#else
#pragma ELSE_MESSAGE
	// �ٸ� �����鿡�� ������ ȹ�� �˸���
	//if( GetPartyUID() > 0  &&  kStorage.IsDungeonDropItem() )
	//{
	//	SiKPartyManager()->BroadCastToPartyMembers( GetPartyUID(), EGS_GET_ITEM_NOT, kStorage );
	//}
#endif SERV_CHANNEL_PARTY
	//}}

	// ���â�� ȹ���� �����۵��� �����ֱ� ���� ��������������� �濡 ������Ʈ
	if( GetRoomUID() > 0  &&  kStorage.m_cGetItemType != KEGS_GET_ITEM_NOT::GIT_RESULT_ITEM )
	{
		KERM_GET_ITEM_COMPLETE_NOT kPacketToRoom;
		kPacketToRoom.m_iUnitUID = GetCharUID();
		kPacketToRoom.m_mapGetItem = mapInsertedItem;
		SendToCnRoom( ERM_GET_ITEM_COMPLETE_NOT, kPacketToRoom );
	}

	// Ŭ���̾�Ʈ �κ��丮�� ���� ������ ������Ʈ
	KEGS_GET_ITEM_REALTIME_NOT kPacket;
	kPacket.m_vecKInventorySlotInfo = kPacketReq.m_vecUpdatedInventorySlot;
	SendPacket( EGS_GET_ITEM_REALTIME_NOT, kPacket );

	//////////////////////////////////////////////////////////////////////////
	// ��� : Character Info, Item
	// ������ ���, DB ��� ī��Ʈ ���� ����
	int iDropItemCnt = 0;
	std::map< int, int >::const_iterator mit;
	for ( mit = mapInsertedItem.begin(); mit != mapInsertedItem.end(); ++mit )
	{
		const int iItemID = mit->first;
		const int iQuantity = mit->second;

		// ED���������� �˻�
		switch( iItemID )
		{
		case CXSLItem::EDI_BRONZE_ED:
		case CXSLItem::EDI_SILVER_ED:
		case CXSLItem::EDI_GOLD_ED:
			continue;
		}

		// �Ϲݾ������̶�� ī��Ʈ
		iDropItemCnt += iQuantity;

		// ��� : Local Item
		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( iItemID );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_DROP, iQuantity );
			//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_DROP, iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}
	}

	// ���� ��� ���� ������
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Obtained_Item, iDropItemCnt );

	//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	m_kUserAbuserManager.IncreaseGetItemCount( GetThisPtr<KGSUser>(), iDropItemCnt );
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}

	//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
	// ������ �����
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_DUNGEON, mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS// �۾���¥: 2013-06-08	// �ڼ���
IMPL_ON_FUNC_NOPARAM( ERM_UPDATE_BUFF_INFO_NOT )
{
	VERIFY_STATE_LOG( clog, ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	SendUpdateBuffInfo();
}
#endif // SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS

//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////
