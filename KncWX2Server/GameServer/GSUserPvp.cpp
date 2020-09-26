#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "ExpTable.h"
#include "BaseServer.h"
//{{ 2008. 11. 24  ������	�� ����Ʈ
#include "RoomListManager.h"
//}}

#include "Enum/Enum.h"

//{{ 2012 ���� ����2 ���� ��Ī �̺�Ʈ	- ��μ�
//#ifdef SERV_2012_PVP_SEASON2_EVENT
//	#include "GameSysVal/GameSysVal.h"
//#endif SERV_2012_PVP_SEASON2_EVENT
//}}

//{{ 2012. 09. 18   ��μ�   2012 ���� ����2 �̺�Ʈ
#ifdef SERV_2012_PVP_SEASON2_NEW_EVENT
	#include "GameSysVal/GameSysVal.h"
#endif SERV_2012_PVP_SEASON2_NEW_EVENT
//}}

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
	#include "NewPartyListManager.h"
#endif SERV_RECRUIT_EVENT_SUBQUEST

#ifdef SERV_STRING_FILTER_USING_DB
#include "StringFilterManager.h"
#endif //SERV_STRING_FILTER_USING_DB

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp ���� ������ �˴ϴ�!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC_NOPARAM( EGS_OPEN_PVP_ROOM_LIST_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_OPEN_PVP_ROOM_LIST_ACK kPacket;
	KStatisticsKey kKey;

	if( SiKRoomListManager()->OpenPVPRoomList( GetThisPtr<KGSUser>(), kPacket ) == false )
	{
		START_LOG_WITH_NAME( cerr )
			<< L"pvp ä�� ���� ����" << dbg::endl
			<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
			<< END_LOG;

		goto end_proc;
	}

end_proc:
	SendPacket( EGS_OPEN_PVP_ROOM_LIST_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////
	if( kPacket.m_iOK == NetError::NET_OK )
	{
		// �ʵ� ��Ż ó�� - �ʵ忡���� �������� S_FIELD_MAP�� �����˴ϴ�!
		SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );		

		// ��Ƽ�� �ξ��� ������� ��Ƽ Ż��ó��
		SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_01 );

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
	//////////////////////////////////////////////////////////////////////////
}

IMPL_ON_FUNC( EGS_CHANGE_PVP_ROOM_LIST_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_CHANGE_PVP_ROOM_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	if( SiKRoomListManager()->ChangePVPRoomList( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
	}

	SendPacket( EGS_CHANGE_PVP_ROOM_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_INVITE_PVP_ROOM_REQ )
{
	//{{ 2010. 9. 7	������	�ߺ� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_INVITE_PVP_ROOM_REQ, EGS_INVITE_PVP_ROOM_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}	

	if( GetRoomUID() < 0  ||  CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_PVP )
	{
		KEGS_INVITE_PVP_ROOM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_ROOM_43;
		SendPacket( EGS_INVITE_PVP_ROOM_ACK, kPacketAck );
		return;
	}

	KRoomInfoPtr spRoomInfo = SiKRoomListManager()->GetRoomInfo( GetRoomUID() );
	if( !spRoomInfo )
	{
		START_LOG( cerr, L"������ �ʴ븦 �Ϸ��µ� �������� ����?" )
			<< BUILD_LOG( GetRoomUID() )
			<< END_LOG;

		KEGS_INVITE_PVP_ROOM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_ROOM_16;
		SendPacket( EGS_INVITE_PVP_ROOM_ACK, kPacketAck );
		return;
	}

	// �α��� ������ ������!
	KELG_INVITE_PVP_ROOM_REQ kPacketReq;
	kPacketReq.m_iToUnitUID		  = kPacket_.m_iUnitUID;	
	kPacketReq.m_wstrFromNickName = GetCharName();
	kPacketReq.m_kRoomInfo		  = *spRoomInfo;
	SendToLoginServer( ELG_INVITE_PVP_ROOM_REQ, kPacketReq );
}

IMPL_ON_FUNC( ELG_INVITE_PVP_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_INVITE_PVP_ROOM_ACK kPacket;
	kPacket.m_iOK		   = kPacket_.m_iOK;
	kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
	SendPacket( EGS_INVITE_PVP_ROOM_ACK, kPacket );
}

_IMPL_ON_FUNC( ELG_INVITE_PVP_ROOM_NOT, KEGS_INVITE_PVP_ROOM_NOT )
{
	// �������� �ɾ���ϳ�?

	// �ʴ��� �� ���� ����
	if( GetStateID() != KGSFSM::S_FIELD_MAP  &&  GetStateID() != KGSFSM::S_ROOM )
		return;

	char cUnitState = GetUnitState();
	if( cUnitState != CXSLUnit::CUS_PVP_LOBBY  &&  cUnitState != CXSLUnit::CUS_PVP_WAIT )
	{
		// �ʴ� �޽����� ������!
		return;
	}	

	// �ʴ� �޽��� ����
	KEGS_INVITE_PVP_ROOM_NOT kPacketNot;
	kPacketNot.m_wstrNickName = kPacket_.m_wstrNickName;
	kPacketNot.m_kRoomInfo	  = kPacket_.m_kRoomInfo;
	SendPacket( EGS_INVITE_PVP_ROOM_NOT, kPacketNot );
}

IMPL_ON_FUNC_NOPARAM( EGS_STATE_CHANGE_GAME_INTRUDE_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_STATE_CHANGE_GAME_INTRUDE_REQ, EGS_STATE_CHANGE_GAME_INTRUDE_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}    

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// �۾���¥: 2013-10-08	// �ڼ���
	if( ( m_kUserCheatManager.IsObserverMode() == false ) && ( GetRoomListID() == SiKRoomListManager()->GetPvpOffcialRoomListID() ) )
	{
		KEGS_STATE_CHANGE_GAME_INTRUDE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ROOM_53;	// ���� �������� ������ �� �����ϴ�.
		SendPacket( EGS_STATE_CHANGE_GAME_INTRUDE_ACK, kPacket );
		return;
	}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM

	SendToCnRoom( ERM_STATE_CHANGE_GAME_INTRUDE_REQ );
}

IMPL_ON_FUNC( ERM_STATE_CHANGE_GAME_INTRUDE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );	

	KEGS_STATE_CHANGE_GAME_INTRUDE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	SendPacket( EGS_STATE_CHANGE_GAME_INTRUDE_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_STATE_CHANGE_GAME_INTRUDE_NOT, KEGS_STATE_CHANGE_GAME_INTRUDE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_STATE_CHANGE_GAME_INTRUDE_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_INTRUDE_START_REQ )
{
	//{{ 2010. 9. 7	������	�ߺ� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_INTRUDE_START_REQ, EGS_INTRUDE_START_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}    

	START_LOG_WITH_NAME( clog );

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// �۾���¥: 2013-10-08	// �ڼ���
	if( ( m_kUserCheatManager.IsObserverMode() == false ) && ( GetRoomListID() == SiKRoomListManager()->GetPvpOffcialRoomListID() ) )
	{
		KEGS_INTRUDE_START_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ROOM_53;	// ���� �������� ������ �� �����ϴ�.
		SendPacket( EGS_INTRUDE_START_ACK, kPacket );
		return;
	}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM

	SendToCnRoom( ERM_INTRUDE_START_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_INTRUDE_START_ACK, KEGS_INTRUDE_START_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_INTRUDE_START_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_INTRUDE_START_NOT, KEGS_INTRUDE_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

#ifdef SERV_SERVER_BUFF_SYSTEM// �۾���¥: 2013-06-08	// �ڼ���
#else // SERV_SERVER_BUFF_SYSTEM
	//{{ 2012. 09. 17	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	KRoomUserInfo kRoomUserInfo;
	KEGS_UPDATE_BUFF_INFO_IN_ROOM_NOT kPacket;
	const int iRoomType = CXSLRoom::GetRoomType( GetRoomUID() );

	kPacket.m_iUnitUID = GetCharUID();
	m_kUserBuffManager.GetBuffList( kPacket.m_vecActivateBuffList, CXSLRoom::RT_PVP );
	GetRoomUserInfo( kRoomUserInfo, iRoomType );
	kPacket.m_kGameStat = kRoomUserInfo.m_kGameStat;
	SendToCnRoom( ERM_UPDATE_BUFF_INFO_IN_ROOM_NOT, kPacket );
#endif SERV_2012_PVP_SEASON2
	//}}
#endif // SERV_SERVER_BUFF_SYSTEM

	SendPacket( EGS_INTRUDE_START_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CURRENT_KILL_SCORE_NOT, KEGS_CURRENT_KILL_SCORE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CURRENT_KILL_SCORE_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CURRENT_TEAM_KILL_SCORE_NOT, KEGS_CURRENT_TEAM_KILL_SCORE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CURRENT_TEAM_KILL_SCORE_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_CHANGE_MOTION_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_CHANGE_MOTION_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_MOTION_ACK, KEGS_CHANGE_MOTION_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_MOTION_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_MOTION_NOT, KEGS_CHANGE_MOTION_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_MOTION_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_CHANGE_TEAM_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( GetRoomUID() );

	SendToCnRoom( ERM_CHANGE_TEAM_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_TEAM_ACK, KEGS_CHANGE_TEAM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	SendPacket( EGS_CHANGE_TEAM_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_TEAM_NOT, KEGS_CHANGE_TEAM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_TEAM_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_CHANGE_READY_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
//#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_CHANGE_READY_REQ, EGS_CHANGE_READY_ACK );
//#else
//	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
//#endif SERV_REPEAT_FILTER_REFAC
	//}}	

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( GetRoomUID() );

	if( GetTradeUID() != 0 )
	{
		START_LOG( cerr, L"�ŷ� �� ���� ����." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetTradeUID() )
			<< BUILD_LOG( GetRoomUID() )
			<< END_LOG;

		KEGS_CHANGE_READY_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_ROOM_32;
		SendPacket( EGS_CHANGE_READY_ACK, kPacketAck );
		return;
	}

	if( GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_DUNGEON:
			{
				// �ټ��� �˻� - ����id�� 0���� ���� <-- ����� �ǹ̾��� �ڵ��̹Ƿ� ���Ŀ� �����ؾ���.
				//if( CheckingSpirit( 0 ) == false )
				//{
				//	KEGS_CHANGE_READY_ACK kPacketAck;
				//	kPacketAck.m_iOK = NetError::ERR_ROOM_27;
				//	SendPacket( EGS_CHANGE_READY_ACK, kPacketAck );
				//	return;
				//}
			}
			break;

			//{{ 2010. 7. 28  ������	��ȸ ä�� ���� ó��
#ifdef SERV_TOURNAMENT_CONDITION
		case CXSLRoom::RT_PVP:
			{
				int iPvpChannelClass = 0;
				LIF( GetPVPChannelClass( iPvpChannelClass ) );
				if( iPvpChannelClass == KPVPChannelInfo::PCC_NONE )
				{
					START_LOG( cerr, L"�渮��Ʈ���� ���� ã�� �� �����ϴ�." )
						<< BUILD_LOG( GetRoomUID() )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;

					KEGS_CHANGE_READY_ACK kPacketAck;
					kPacketAck.m_iOK = NetError::ERR_ROOM_31;
					SendPacket( EGS_CHANGE_READY_ACK, kPacketAck );
					return;
				}
				else if( iPvpChannelClass == KPVPChannelInfo::PCC_TOURNAMENT )
				{
#ifdef SERV_MAIN_TOURNAMENT
					// �������� Ƽ�� ������ ��ȸä�� ����.
					if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == false )
					{
						KEGS_CHANGE_READY_ACK kPacketAck;
						kPacketAck.m_iOK = NetError::ERR_ROOM_50;
						SendPacket( EGS_CHANGE_READY_ACK, kPacketAck );
						return;
					}
#else
					// �������� Ƽ�� ������ ��ȸä�� ����.
					if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == true )
					{
						KEGS_CHANGE_READY_ACK kPacketAck;
						kPacketAck.m_iOK = NetError::ERR_ROOM_39;
						SendPacket( EGS_CHANGE_READY_ACK, kPacketAck );
						return;
					}
#endif SERV_MAIN_TOURNAMENT
				}
			}
			break;			
#endif SERV_TOURNAMENT_CONDITION
			//}}

		default:
			break;
		}
	}
	else
	{
		START_LOG( cerr, L"�濡 �������ִµ� RoomUID�� ����?" )
			<< BUILD_LOG( GetRoomUID() )
			<< END_LOG;

		KEGS_CHANGE_READY_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_ROOM_31;
		SendPacket( EGS_CHANGE_READY_ACK, kPacketAck );
		return;
	}

	SendToCnRoom( ERM_CHANGE_READY_REQ, kPacket_ );
}

IMPL_ON_FUNC( ERM_CHANGE_READY_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	KEGS_CHANGE_READY_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	SendPacket( EGS_CHANGE_READY_ACK, kPacket );

	//{{ 2008. 10. 29  ������	���� �޽��� ����	
	switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
	{
	case CXSLRoom::RT_DUNGEON:
		{
			switch( kPacket_.m_iDungeonID )
			{
			case SEnum::DI_ELDER_WALLY_CASTLE_LAB_NORMAL:
			case SEnum::DI_ELDER_WALLY_CASTLE_LAB_HARD:
			case SEnum::DI_ELDER_WALLY_CASTLE_LAB_EXPERT:
			case SEnum::DI_BESMA_SECRET_NORMAL:
			case SEnum::DI_BESMA_SECRET_HARD:
			case SEnum::DI_BESMA_SECRET_EXPERT:
			case SEnum::DI_ELDER_HALLOWEEN_NORMAL:
			case SEnum::DI_ELDER_HALLOWEEN_HARD:
			case SEnum::DI_ELDER_HALLOWEEN_EXPERT:
			case SEnum::DI_ALTERA_SECRET_NORMAL:
			case SEnum::DI_ALTERA_SECRET_HARD:
			case SEnum::DI_ALTERA_SECRET_EXPERT:
			case SEnum::DI_ELDER_NEWYEAR_NORMAL:
				{
					// �����ʿ� �����۰˻�.
					if( !CheckRequiredItemToEnterDungeonRoom( kPacket_.m_iDungeonID, (CXSLDungeon::DUNGEON_MODE) kPacket_.m_cDungeonMode ) )
					{
						KEGS_DUNGEON_REQUIRE_ITEM_MESSAGE_NOT kPacketNot;
						kPacketNot.m_iOK = NetError::ERR_ROOM_41;
						SendPacket( EGS_DUNGEON_REQUIRE_ITEM_MESSAGE_NOT, kPacketNot );
						return;
					}
				}
				break;
			}
		}
		break;
	}
	//}}	
}

_IMPL_ON_FUNC( ERM_CHANGE_READY_NOT, KEGS_CHANGE_READY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_READY_NOT, kPacket_ );
}

//////////////////////////////////////////////////////////////////////////
IMPL_ON_FUNC( EGS_CHANGE_PITIN_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( GetRoomUID() );

	SendToCnRoom( ERM_CHANGE_PITIN_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PITIN_ACK, KEGS_CHANGE_PITIN_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	SendPacket( EGS_CHANGE_PITIN_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PITIN_NOT, KEGS_CHANGE_PITIN_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_PITIN_NOT, kPacket_ );
}
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( EGS_CHANGE_MAP_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_CHANGE_MAP_REQ, EGS_CHANGE_MAP_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( GetRoomUID() );

	SendToCnRoom( ERM_CHANGE_MAP_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_MAP_ACK, KEGS_CHANGE_MAP_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	SendPacket( EGS_CHANGE_MAP_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_MAP_NOT, KEGS_CHANGE_MAP_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_WorldID );

	SendPacket( EGS_CHANGE_MAP_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_CHANGE_SLOT_OPEN_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_CHANGE_SLOT_OPEN_REQ, EGS_CHANGE_SLOT_OPEN_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}    

	START_LOG_WITH_NAME( clog );

	SendToCnRoom( ERM_CHANGE_SLOT_OPEN_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_SLOT_OPEN_ACK, KEGS_CHANGE_SLOT_OPEN_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_SLOT_OPEN_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_SLOT_OPEN_NOT, KEGS_CHANGE_SLOT_OPEN_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_SLOT_OPEN_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_CHANGE_PLAY_TIME_LIMIT_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_CHANGE_PLAY_TIME_LIMIT_REQ, EGS_CHANGE_PLAY_TIME_LIMIT_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}    

	START_LOG_WITH_NAME( clog );

	SendToCnRoom( ERM_CHANGE_PLAY_TIME_LIMIT_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PLAY_TIME_LIMIT_ACK, KEGS_CHANGE_PLAY_TIME_LIMIT_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PLAY_TIME_LIMIT_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PLAY_TIME_LIMIT_NOT, KEGS_CHANGE_PLAY_TIME_LIMIT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PLAY_TIME_LIMIT_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_CHANGE_WINNING_NUM_KILL_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC 
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_CHANGE_WINNING_NUM_KILL_REQ, EGS_CHANGE_WINNING_NUM_KILL_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}    

	START_LOG_WITH_NAME( clog );

	SendToCnRoom( ERM_CHANGE_WINNING_NUM_KILL_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_WINNING_NUM_KILL_ACK, KEGS_CHANGE_WINNING_NUM_KILL_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_WINNING_NUM_KILL_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_WINNING_NUM_KILL_NOT, KEGS_CHANGE_WINNING_NUM_KILL_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_WINNING_NUM_KILL_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_LEAVE_GAME_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_LEAVE_GAME_REQ, EGS_LEAVE_GAME_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}    

	SendToCnRoom( ERM_LEAVE_GAME_REQ );
}

_IMPL_ON_FUNC( ERM_LEAVE_GAME_ACK, KEGS_LEAVE_GAME_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_LEAVE_GAME_ACK, kPacket_ );
	//StateTransition( KGSFSM::I_TO_ROOM );

	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	std::vector< int > vecPvpBuff;
	m_kUserBuffManager.GetBuffIDList( vecPvpBuff, CXSLRoom::RT_PVP );

	KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
	kReq.m_iUnitUID = GetCharUID();
	BOOST_TEST_FOREACH( int, iBuff, vecPvpBuff )
	{
		DeactivateBuff( iBuff, kReq.m_vecDeActivateBuff );
	}

	if( kReq.m_vecDeActivateBuff.empty() == false )
	{
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}
}

_IMPL_ON_FUNC( ERM_LEAVE_GAME_NOT, KEGS_LEAVE_GAME_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_LEAVE_GAME_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_END_GAME_PVP_RESULT_DATA_NOT, KEGS_END_GAME_PVP_RESULT_DATA_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
	bool bCouplePvp = false;

	if( m_bCouple == true )
	{
		for( int i = 0; i < (int)kPacket_.m_vecPVPUnitInfo.size(); ++i )
		{
			if( kPacket_.m_vecPVPUnitInfo[i].m_UnitUID == m_iRelationTargetUserUid )
			{
				bCouplePvp = true;
			}
		}
	}
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
	bool bHasFriend = false;

	// ��õ�� �ִ��� �켱 ã��
	std::vector< KRecommendUserInfo > vecRecruitUnitList;
	m_kUserRecommendManager.GetRecruitUnitList( vecRecruitUnitList );
	if( vecRecruitUnitList.empty() == true )
	{
		m_kUserRecommendManager.GetRecruiterUnitList( vecRecruitUnitList );
	}

	// �α�-�׽�Ʈ �� ���� ��
	START_LOG( clog, L"Recruit : Recruit Size!" )
		<< BUILD_LOG( vecRecruitUnitList.size() );
	BOOST_TEST_FOREACH( KRecommendUserInfo&, kRecruitUnitInfo, vecRecruitUnitList )
	{
		START_LOG( clog, L"Recruit : Recruit UID!" )
			<< BUILD_LOG( kRecruitUnitInfo.m_iUnitUID );
	}

	START_LOG( clog, L"Recruit : Party Size!" )
		<< BUILD_LOG( kPacket_.m_vecPVPUnitInfo.size() );
	BOOST_TEST_FOREACH( KPVPUnitResultInfo&, kPVPUnitResultInfo, kPacket_.m_vecPVPUnitInfo )
	{
		START_LOG( clog, L"Recruit : Party UID!" )
			<< BUILD_LOG( kPVPUnitResultInfo.m_UnitUID );
	}
	// �α�-�׽�Ʈ �� ���� ��

	BOOST_TEST_FOREACH( KRecommendUserInfo&, kRecruitUnitInfo, vecRecruitUnitList )
	{		
		UidType iRecruitUID = kRecruitUnitInfo.m_iUnitUID;
		BOOST_TEST_FOREACH( KPVPUnitResultInfo&, kPVPUnitResultInfo, kPacket_.m_vecPVPUnitInfo )
		{
			UidType iPVPUnitUID = kPVPUnitResultInfo.m_UnitUID;
			if( iRecruitUID == iPVPUnitUID )
			{
				bHasFriend = true;
			}
		}
	}
#endif SERV_RECRUIT_EVENT_SUBQUEST

	for( int i = 0; i < (int)kPacket_.m_vecPVPUnitInfo.size(); ++i )
	{
		//{{ 2012. 09. 18   ��μ�   2012 ���� ����2 �̺�Ʈ
#ifdef SERV_2012_PVP_SEASON2_NEW_EVENT
		CTime tCurr = CTime::GetCurrentTime();
		if( kPacket_.m_bOfficialMatch == true && SiKGameSysVal()->IsPvpEventTime( tCurr ) == true )
		{
			kPacket_.m_vecPVPUnitInfo[i].m_nEXP = kPacket_.m_vecPVPUnitInfo[i].m_nEXP * 2;
			kPacket_.m_vecPVPUnitInfo[i].m_iAPoint = kPacket_.m_vecPVPUnitInfo[i].m_iAPoint * 2;
		}
#endif SERV_2012_PVP_SEASON2_NEW_EVENT
		//}}

		//{{ 2012 ���� ����2 ���� ��Ī �̺�Ʈ	- ��μ�
#ifdef SERV_2012_PVP_SEASON2_EVENT
		// �������� 1ų�̶� �Ͽ��ٸ�(���� ��� ����)
		if( GetCharUID() == kPacket_.m_vecPVPUnitInfo[i].m_UnitUID && kPacket_.m_vecPVPUnitInfo[i].m_nKillNum > 0 && kPacket_.m_bOfficialMatch == true )
		{
			CTime tCurr = CTime::GetCurrentTime();
			int iEventIndex = 0;

			std::wstring wstrTemp;
			bool bTemp = false;

			// ���� �̺�Ʈ �ð��ΰ�?
			if( m_bIsPvpEventUser == true && SiKGameSysVal()->IsPvpEventTime( tCurr, iEventIndex, wstrTemp, bTemp ) == true )
			{
				// ���� ������ ����( ���� )
				KDBE_PVP_WIN_EVENT_CHECK_REQ kPacketToDB;
				kPacketToDB.m_iUnitUID = GetCharUID();
				kPacketToDB.m_iIndex   = iEventIndex;
				kPacketToDB.m_wstrRegDate = tCurr.Format(_T("%Y-%m-%d %H:%M:%S"));
				SendToGameDB( DBE_PVP_WIN_EVENT_CHECK_REQ, kPacketToDB );
			}
		}
#endif SERV_2012_PVP_SEASON2_EVENT
		//}}

		if( GetCharUID() == kPacket_.m_vecPVPUnitInfo[i].m_UnitUID )
		{
			//{{ 2013. 3. 17	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
			IncreasedLocalRankingPoint( 0, kPacket_.m_vecPVPUnitInfo[i].m_iAPoint );
#endif SERV_LOCAL_RANKING_SYSTEM
			//}}

			//{{ 2012. 09. 18	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
			// ���� ���� �÷��� ī��Ʈ
			if( kPacket_.m_bOfficialMatch == true )
			{
				m_kUserQuestManager.Handler_OnPVPPlay_AccumulatedOfficialMatchCount( GetThisPtr<KGSUser>(), true );
			}
#endif SERV_2012_PVP_SEASON2
			//}}

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

			m_kUserQuestManager.Handler_OnPVPPlay( kPacket_.m_iGameType
												 , GetThisPtr<KGSUser>()
												 // ���� �÷��� ����Ʈ ���� ����	- ��μ�
#ifdef SERV_PVP_PLAY_QUEST
												 , kPacket_.m_bOfficialMatch
#endif SERV_PVP_PLAY_QUEST
												 // ���� �÷��� ����Ʈ ���� ���� - ��μ�
#ifdef PVP_QUEST_HERO_KILL_COUNT
												 , kPacket_.m_bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
												 , bCouplePvp
												 , kPacket_.m_bIsDrawn
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
												 , bHasFriend
#endif SERV_RECRUIT_EVENT_SUBQUEST
												 );

			if( kPacket_.m_bIsDrawn == false )
			{
				//quest
				if( kPacket_.m_vecPVPUnitInfo[i].m_bWin == true )
				{
					m_kUserQuestManager.Handler_OnPVPWin( kPacket_.m_iGameType
														, GetThisPtr<KGSUser>()
														//{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
														//{{ 2012. 02. 22	��μ�	���� ����Ʈ, PVP NPC ���� ����Ʈ	���� ����
#ifdef SERV_NEW_PVP_QUEST_ERROR_MODIFY
#else
														, kPacket_.m_bMatchNPC
#endif SERV_NEW_PVP_QUEST_ERROR_MODIFY
														//}}
														, kPacket_.m_bOfficialMatch
#endif SERV_NEW_PVP_QUEST
														//}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
														, kPacket_.m_bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT
														);
				}
			}
			break;
		}
	}


	SendPacket( EGS_END_GAME_PVP_RESULT_DATA_NOT, kPacket_ );

	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	std::vector< int > vecPvpBuff;
	m_kUserBuffManager.GetBuffIDList( vecPvpBuff, CXSLRoom::RT_PVP );

	KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
	kReq.m_iUnitUID = GetCharUID();
	BOOST_TEST_FOREACH( int, iBuff, vecPvpBuff )
	{
		DeactivateBuff( iBuff, kReq.m_vecDeActivateBuff );
	}

	if( kReq.m_vecDeActivateBuff.empty() == false )
	{
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}
}

IMPL_ON_FUNC( ERM_UPDATE_PVP_UNIT_INFO_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
	m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_PVP_PLAY_TIME, kPacket_.m_iPlayTime / 60 );
#endif SERV_CHAR_LOG
	//}}
#ifdef SERV_USER_STATISTICS_RENEWAL
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_PVP_Play_Time, kPacket_.m_iPlayTime );
#endif //SERV_USER_STATISTICS_RENEWAL

	//{{ 2012. 09. 18   ��μ�   2012 ���� ����2 �̺�Ʈ
#ifdef SERV_2012_PVP_SEASON2_NEW_EVENT
	CTime tCurr = CTime::GetCurrentTime();
	if( kPacket_.m_bIsOfficialMatch == true && SiKGameSysVal()->IsPvpEventTime( tCurr ) == true )
	{
		kPacket_.m_iEXP = kPacket_.m_iEXP * 2;
		kPacket_.m_iAPoint = kPacket_.m_iAPoint * 2;
	}
#endif SERV_2012_PVP_SEASON2_NEW_EVENT
	//}}

	//{{ 2009. 1. 20  ������	��������ġ ����ó��	
	m_kEXP.AddExp( kPacket_.m_iEXP );
	//}}
	//{{ 2011. 06. 20	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	// ���� ��� ������Ʈ
	//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	m_kUserPvpManager.UpdatePvpResult( kPacket_ );
#else
	m_kUserPvpManager.UpdatePvpResult( kPacket_.m_iRating, kPacket_.m_iRPoint, kPacket_.m_iAPoint, kPacket_.m_iWin, kPacket_.m_iLose, kPacket_.m_bIsOfficialMatch, kPacket_.m_bOutRoom );
#endif SERV_2012_PVP_SEASON2
	//}}

	//{{ 2012. 05. 30	������		���� ���� ���� ����
#ifdef SERV_OFFCIAL_PVP_BUG_FIX
	if( GetPartyUID() != 0 )
	{
		KEPM_UPDATE_PVP_INFO_NOT kPacketToPM;
		//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		kPacketToPM.m_cRank = GetPvpRankForClient();
		kPacketToPM.m_iOfficialMatchCnt	= GetOfficialMatchCount();
#endif SERV_2012_PVP_SEASON2
		//}}
		kPacketToPM.m_iRating = m_kUserPvpManager.GetRating();
		kPacketToPM.m_iRPoint = m_kUserPvpManager.GetRPoint();
		SendToCnParty( EPM_UPDATE_PVP_INFO_NOT, kPacketToPM );
	}
#endif SERV_OFFCIAL_PVP_BUG_FIX
	//}}
#else
	m_iVSPoint	+= kPacket_.m_iVSPoint;

	//���Ƽ�� �޴� ���� �߰����� �ʴ´�.
	if( kPacket_.m_iVSPoint > 0  )
	{
		m_iVSPointMax	+= kPacket_.m_iVSPoint;	 //�ƽ����� ������ ������ �޴� ������Ʈ �����͸� �޴´�.
		m_ePVPEmblem	= CXSLUnit::GetPVPEmblem( m_iVSPointMax );
	}

	if( kPacket_.m_iWin > 1 && kPacket_.m_iLose > 1 )
	{
		START_LOG( cerr, L"�¸���� ���� ���ÿ� 2���� ���õǾ� �Դ�.!" )
			<< BUILD_LOG( kPacket_.m_iWin )
			<< BUILD_LOG( kPacket_.m_iLose )
			<< END_LOG;
	}
	if( kPacket_.m_iWin > 1 )
	{
		START_LOG( cerr, L"WIN ����Ʈ �̻�.!" )
			<< BUILD_LOG( kPacket_.m_iWin )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket_.m_iWin = 1;
	}
	if( kPacket_.m_iLose > 1 )
	{
		START_LOG( cerr, L"LOSE ����Ʈ �̻�.!" )
			<< BUILD_LOG( kPacket_.m_iLose )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket_.m_iLose = 1;
	}

	m_iWin		+= kPacket_.m_iWin;
	m_iLose		+= kPacket_.m_iLose;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	CheckCharLevelUp();

	//{{ 2009. 11. 16  ������	�����������Ʈ
	if( GetGuildUID() > 0  &&  kPacket_.m_bOutRoom == false )
	{
		// �� �Ǵ� �а� �ִٸ� ��� ����ġ �ش� + ���� �÷��� Ÿ���� 20�� �̻��̾����.
		if( ( kPacket_.m_iWin > 0  ||  kPacket_.m_iLose > 0 )  &&  kPacket_.m_iPlayTime > SEnum::ET_PVP_DRAWN_TIME )
		{
			m_kUserGuildManager.OnUpdatePvpUnitInfo( GetThisPtr<KGSUser>(), kPacket_.m_iMemberCount );

			KEGS_DECREASE_SPIRIT_NOT kNot;
			//{{ 2012. 03. 20	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
			kNot.m_iSpirit = m_kUserSpiritManager.GetSpirit(); // ���� �ټ�����
#else
			kNot.m_iSpirit = m_iSpirit; // ���� �ټ�����
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}			
			kNot.m_iHonorPoint = m_kUserGuildManager.GetHonorPoint(); // ���� ������Ʈ

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
	//}}

	//{{ 2010. 8. 6	������	�� �ý���
#ifdef SERV_PET_SYSTEM
	m_kUserPetManager.Handler_ERM_UPDATE_PVP_UNIT_INFO_NOT();
#endif SERV_PET_SYSTEM
	//}}

	//�������� ������ �����͸� �Ѱ��ش�.
	KEGS_UPDATE_UNIT_INFO_NOT kUpdatePacketNot;

	//�������� ������ �űԾ����ۿ� ���� dbó���� �Ұ����� üũ
	if( !kPacket_.m_mapItem.empty() )
	{
		KDBE_INSERT_ITEM_REQ kPacketReq;
		kPacketReq.m_iUnitUID = GetCharUID();
		//{{ 2010. 9. 8	������	������ ȹ�� ����
#ifdef SERV_GET_ITEM_REASON
		kPacketReq.m_cGetItemReason = SEnum::GIR_PVP_RESULT;
#endif SERV_GET_ITEM_REASON
		//}}
#ifdef SERV_INT_ONLY
		m_kInventory.PrepareInsert( kPacket_.m_mapItem, kPacketReq.m_mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo );
#else //SERV_INT_ONLY
		if ( false == m_kInventory.PrepareInsert( kPacket_.m_mapItem, kPacketReq.m_mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo, true ) ) 
        {
            typedef std::map< int, KItemInfo > KPVPItem;
            BOOST_TEST_FOREACH( const KPVPItem::value_type& , kItem, kPacket_.m_mapItem )
            {
                KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
                kPacketToDB.m_iFromUnitUID = GetCharUID();
                kPacketToDB.m_iToUnitUID   = GetCharUID();
                kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
                kPacketToDB.m_iRewardID	   = 10613;
                kPacketToDB.m_sQuantity    = kItem.second.m_iQuantity;
                SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );

                START_LOG( cwarn, L"ī�ж� ������ �������� ����")
                    << BUILD_LOG( kPacket_.m_mapItem.size() )
                    << END_LOG;
            }
        }
#endif //SERV_INT_ONLY

		//////////////////////////////////////////////////////////////////////////
		// ��� : ������ ��������� ����
		int iDropItemCnt = 0;
		std::map< int, int >::const_iterator mit;
		for( mit = kPacketReq.m_mapInsertedItem.begin(); mit != kPacketReq.m_mapInsertedItem.end(); ++mit )
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

			// �Ϲ� �������̶�� ī��Ʈ
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

		// ������ �����
		//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
		m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_PVP, kPacketReq.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
		//}}
		//////////////////////////////////////////////////////////////////////////


		if( !kPacketReq.m_vecItemInfo.empty() )
		{
			m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );			
			m_kInventory.FlushEnduranceChange( kPacketReq.m_kItemEnduranceUpdate );
			m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
			kPacketReq.m_bOutRoom = kPacket_.m_bOutRoom;
			SendToGameDB( DBE_INSERT_ITEM_REQ, kPacketReq );
			return;
		}
		else
		{
			kUpdatePacketNot.m_mapItemObtained = kPacketReq.m_mapInsertedItem;
			std::map< int, int >::iterator mit;
			for( mit = kUpdatePacketNot.m_mapItemObtained.begin(); mit != kUpdatePacketNot.m_mapItemObtained.end(); ++mit )
			{
				START_LOG( clog2, L"ȹ�� ������." )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( mit->second );
			}
			kUpdatePacketNot.m_vecKInventorySlotInfo = kPacketReq.m_vecUpdatedInventorySlot;
		}
	}

	//{{ 2011. 07. 29	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	// ���Ĵ����� ��� RoomListID�� ������ ����������!
	if( kPacket_.m_bIsOfficialMatch )
	{
        SetRoomListID( SiKRoomListManager()->GetPvpOffcialRoomListID() );
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}

	KRoomUserInfo kInfo;
	GetRoomUserInfo( kInfo );

	//�濡�� ������ ��찡 �ƴҰ�� ���ͼ����� ������Ʈ�� �����ش�.
	if( kPacket_.m_bOutRoom == false )
	{
		SendToCnRoom( ERM_SET_UNIT_INFO_NOT, kInfo );
	}

	GetUnitInfo( kUpdatePacketNot.m_kUnitInfo );
	kUpdatePacketNot.m_kUnitInfo.m_nNowBaseLevelEXP		= SiKExpTable()->GetRequireTotalExpbyLevel( static_cast<int>(GetLevel()) );
	kUpdatePacketNot.m_kUnitInfo.m_nNextBaseLevelEXP	= SiKExpTable()->GetRequireTotalExpbyLevel( static_cast<int>(GetLevel() + 1) );

	SendPacket( EGS_UPDATE_UNIT_INFO_NOT, kUpdatePacketNot );

	//{{ 2011. 07. 29	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	// ���Ĵ����� ������! RoomListID�� 0���� �ٲ���!
	if( kPacket_.m_bIsOfficialMatch )
	{
		if( GetRoomListID() != SiKRoomListManager()->GetPvpOffcialRoomListID() )
		{
			START_LOG( cerr, L"���� ��ġ�ε� RoomListID���� �̻��մϴ�! ���״�!" )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetRoomListID() )
				<< BUILD_LOG( SiKRoomListManager()->GetPvpOffcialRoomListID() )
				<< END_LOG;

			// �״�� ����!
		}

        SetRoomListID( 0 );
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

IMPL_ON_FUNC_NOPARAM( ERM_DECREASE_ENDURANCE_PVP_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	// ItemTemplet�� ������ �������� �����Ͽ� ��´�
	m_kInventory.DecreaseEquippedItemEnduranceInPvp();
}

IMPL_ON_FUNC( ERM_PVP_USER_STATISTICS_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iGamePlayTime < 0 )
		kPacket_.m_iGamePlayTime = 0;

	// ���� �÷��� Ÿ��
	m_kUserStatistics.AddPlayTime( KUserStatistics::PT_TOTAL_PLAY_TIME, kPacket_.m_iGamePlayTime );
	m_kUserStatistics.AddPlayTime( KUserStatistics::PT_PVP_PLAY_TIME, kPacket_.m_iGamePlayTime );	

	// ���� ��� ����
	switch( kPacket_.m_cGameResult )
	{
	case KERM_PVP_USER_STATISTICS_NOT::UGR_WIN:
		{
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpModeCount, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_MC_Win, 1 );
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpPlayTime, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_PT_Win, kPacket_.m_iGamePlayTime );
		}
		break;

	case KERM_PVP_USER_STATISTICS_NOT::UGR_LOSE:
		{
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpModeCount,	static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_MC_Lose, 1 );
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpPlayTime, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_PT_Lose, kPacket_.m_iGamePlayTime );
		}
		break;

	case KERM_PVP_USER_STATISTICS_NOT::UGR_DRAW:
		{
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpModeCount, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_MC_Draw, 1 );
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpPlayTime, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_PT_Draw,	kPacket_.m_iGamePlayTime );
		}
		break;

	case KERM_PVP_USER_STATISTICS_NOT::UGR_DROP:
		{
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpModeCount, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_MC_Drop, 1 );
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpPlayTime, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_PT_Drop, kPacket_.m_iGamePlayTime );
		}
		break;

	default:
		START_LOG( cerr, L"�̷� ��� Ÿ���� ����." )
			<< BUILD_LOGc( kPacket_.m_cGameResult )
			<< END_LOG;
		break;
	}

	// ���� ��� ����
	if( kPacket_.m_bIntrude == true )
	{				
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpModeCount, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_MC_Intrude, 1 );
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpPlayTime, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_PT_Intrude, kPacket_.m_iGamePlayTime );
	}

	// ���� ��� ���� ų��
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Pvp_Kill_Count, kPacket_.m_iKillNum );

	// ���� �� ���
	KStatisticsKey kKeyPvpMap;
	kKeyPvpMap.m_vecIntKey.push_back( kPacket_.m_sWorldID );
	KSIManager.IncreaseCount( KStatistics::SI_PVP_MAP, kKeyPvpMap, KStatistics::eSIColPvpMap_PlayCount, 1 );
}

#ifdef DUNGEON_ITEM
IMPL_ON_FUNC( EGS_CREATE_PVP_ITEM_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_CREATE_PVP_ITEM_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CREATE_PVP_ITEM_ACK, KEGS_CREATE_PVP_ITEM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CREATE_PVP_ITEM_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CREATE_PVP_ITEM_NOT, KEGS_CREATE_PVP_ITEM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CREATE_PVP_ITEM_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ITEM_NOT, KEGS_CHANGE_PVP_ITEM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_PVP_ITEM_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_CHANGE_PVP_ITEMMODE_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_CHANGE_PVP_ITEMMODE_REQ );

}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ITEMMODE_ACK, KEGS_CHANGE_PVP_ITEMMODE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_PVP_ITEMMODE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ITEMMODE_NOT, KEGS_CHANGE_PVP_ITEMMODE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_PVP_ITEMMODE_NOT, kPacket_ );
}
#endif DUNGEON_ITEM

//}}
//////////////////////////////////////////////////////////////////////////

#ifdef SERVER_PVP_BASE_DEFENCE_TEST
IMPL_ON_FUNC( EGS_CHANGE_PVP_GAME_OPTION_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendToCnRoom( ERM_CHANGE_PVP_GAME_OPTION_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_GAME_OPTION_ACK, KEGS_CHANGE_PVP_GAME_OPTION_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PVP_GAME_OPTION_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_GAME_OPTION_NOT, KEGS_CHANGE_PVP_GAME_OPTION_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PVP_GAME_OPTION_NOT, kPacket_ );
}
#endif SERVER_PVP_BASE_DEFENCE_TEST


//{{ 2012. 02. 14    ��μ�   PVP ���� Item ���� ��ġ ����
#ifdef SERV_PVP_ITEM_CHANGE_POS
IMPL_ON_FUNC( EGS_CHANGE_PVP_ITEM_POS_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_CHANGE_PVP_ITEM_POS_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ITEM_POS_ACK, KEGS_CHANGE_PVP_ITEM_POS_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PVP_ITEM_POS_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ITEM_POS_NOT, KEGS_CHANGE_PVP_ITEM_POS_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PVP_ITEM_POS_NOT, kPacket_ );
}
#endif SERV_PVP_ITEM_CHANGE_POS
//}}

//{{ 2012 ���� ����2 ���� ��Ī �̺�Ʈ	- ��μ�
#ifdef SERV_2012_PVP_SEASON2_EVENT
IMPL_ON_FUNC( DBE_PVP_WIN_EVENT_CHECK_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
    
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// ������ �޾����Ƿ� �̺�Ʈ ��󿡼� ����
		m_bIsPvpEventUser = false;
	}
}
#endif SERV_2012_PVP_SEASON2_EVENT
//}}

#ifdef SERV_NEW_PVPROOM_PROCESS
_IMPL_ON_FUNC( EGS_CHANGE_PVP_ROOM_PUBLIC_REQ, KEGS_CHANGE_PVP_ROOM_PUBLIC_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendToCnRoom( ERM_CHANGE_PVP_ROOM_PUBLIC_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ROOM_PUBLIC_ACK, KEGS_CHANGE_PVP_ROOM_PUBLIC_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PVP_ROOM_PUBLIC_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ROOM_PUBLIC_NOT, KEGS_CHANGE_PVP_ROOM_PUBLIC_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PVP_ROOM_PUBLIC_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EGS_CHANGE_PVP_ROOM_NAME_REQ, KEGS_CHANGE_PVP_ROOM_NAME_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

#ifdef SERV_STRING_FILTER_USING_DB
	kPacket_.m_wstrRoomName = SiKStringFilterManager()->FilteringChatString( kPacket_.m_wstrRoomName.c_str(), L'��' );
#endif //SERV_STRING_FILTER_USING_DB

	START_LOG_WITH_NAME( clog );

	SendToCnRoom( ERM_CHANGE_PVP_ROOM_NAME_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ROOM_NAME_ACK, KEGS_CHANGE_PVP_ROOM_NAME_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PVP_ROOM_NAME_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ROOM_NAME_NOT, KEGS_CHANGE_PVP_ROOM_NAME_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PVP_ROOM_NAME_NOT, kPacket_ );
}
#endif SERV_NEW_PVPROOM_PROCESS

#ifdef SERV_PVP_REMATCH
IMPL_ON_FUNC( EGS_PVP_REMATCH_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_PVP_REMATCH_REQ, kPacket_ );
}
_IMPL_ON_FUNC( ERM_PVP_REMATCH_ACK, KEGS_PVP_REMATCH_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PVP_REMATCH_ACK, kPacket_ );
}
_IMPL_ON_FUNC( ERM_PVP_REMATCH_NOT, KEGS_PVP_REMATCH_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PVP_REMATCH_NOT, kPacket_ );
}
#endif SERV_PVP_REMATCH

//{{ 2012. 10. 10	�ڼ���	ƨ�� ���� ������ ������
#ifdef UDP_CAN_NOT_SEND_USER_KICK
_IMPL_ON_FUNC( ERM_UDP_KICK_STATE_CHANGE_FIELD_NOT, UidType )
//IMPL_ON_FUNC_NOPARAM( ERM_UDP_KICK_STATE_CHANGE_FIELD_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_UDP_KICK_STATE_CHANGE_FIELD_NOT kPacket;
	kPacket.m_iAccusationUnitUID = kPacket_;
	SendPacket( EGS_UDP_KICK_STATE_CHANGE_FIELD_NOT, kPacket );
	//SendID( EGS_UDP_KICK_STATE_CHANGE_FIELD_NOT );
}

#endif UDP_CAN_NOT_SEND_USER_KICK
//}}

//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


