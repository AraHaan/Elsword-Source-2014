#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
#include "UserManager.h"
#include "GameSysVal/GameSysVal.h"

//{{ 2011. 12. 05	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewPartyListManager.h"
#else
	#include "PartyListManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#include "Enum/Enum.h"
#include "RoomListManager.h"

#ifdef SERV_STRING_FILTER_USING_DB
#include "StringFilterManager.h"
#endif //SERV_STRING_FILTER_USING_DB


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp ���� ������ �˴ϴ�!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser


//{{ 2011. 06. 21	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
IMPL_ON_FUNC( EGS_CHANGE_PARTY_TYPE_REQ )
{
	//{{ 2012. 04. 30	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_PARTY_TYPE_REQ, EGS_CHANGE_PARTY_TYPE_ACK );
#else
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CHANGE_PARTY_TYPE_REQ, EGS_CHANGE_PARTY_TYPE_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

#ifdef SERV_STRING_FILTER_USING_DB
	kPacket_.m_wstrPartyName = SiKStringFilterManager()->FilteringChatString( kPacket_.m_wstrPartyName.c_str(), L'��' );
#endif //SERV_STRING_FILTER_USING_DB

	KEGS_CHANGE_PARTY_TYPE_ACK kAck;

	switch( kPacket_.m_cPartyType )
	{
	case KPartyInfo::PT_DUNGEON_PARTY:
		{
			if( CheckDungeonPartyCondition( kPacket_.m_iDungeonID, kPacket_.m_DifficultyLevel, kPacket_.m_cDungeonMode, kPacket_.m_bCheckLowLevel, true ) == false )
			{
				kAck.m_iOK = NetError::GetLastError();
				goto end_proc;
			}
		}
		break;

	case KPartyInfo::PT_PVP_PARTY:
		{
			if( CheckPvpPartyCondition( kPacket_.m_cPVPGameType, true ) == false )
			{
				kAck.m_iOK = NetError::GetLastError();
				goto end_proc;
			}
		}
		break;

	default:
		{
			kAck.m_iOK = NetError::ERR_PARTY_00;
			goto end_proc;
		}
		break;
	}

	if( SiKPartyListManager()->E_ChangePartyType( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"��Ƽ Ÿ�� ���� ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

	return;

end_proc:
	START_LOG( cwarn, L"��Ƽ Ÿ�� ���� ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharUID() )
#else
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( NetError::GetErrStr( kAck.m_iOK ) )
		<< END_LOG;

	SendPacket( EGS_CHANGE_PARTY_TYPE_ACK, kAck );
}

_IMPL_ON_FUNC( EPM_CHANGE_PARTY_TYPE_ACK, KEGS_CHANGE_PARTY_TYPE_ACK )
{
	//{{ 2012. 04. 30	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_PARTY_TYPE_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CHANGE_PARTY_TYPE_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	SendPacket( EGS_CHANGE_PARTY_TYPE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_CHANGE_PARTY_TYPE_NOT, KEGS_CHANGE_PARTY_TYPE_NOT )
{
	//{{ 2012. 04. 30	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

    SendPacket( EGS_CHANGE_PARTY_TYPE_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ )
{
	//{{ 2012. 04. 30	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ, EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK );
#else
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ, EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	if( GetPartyUID() == 0 )
	{
		START_LOG( clog, L"�̰� �������� ��Ȳ��." )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK kAck;
		kAck.m_iOK = NetError::NET_OK;
		SendPacket( EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK, kAck );
		return;
	}

	if( SiKPartyListManager()->E_ChangePvpMatchInfo( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"��Ƽ ���� ��ġ ���� ���� ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( EPM_PVP_PARTY_CHANGE_MATCH_INFO_ACK, KEGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK )
{
	//{{ 2012. 04. 30	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK );	
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK );	
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

    SendPacket( EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PVP_PARTY_CHANGE_MATCH_INFO_NOT, KEGS_PVP_PARTY_CHANGE_MATCH_INFO_NOT )
{
	//{{ 2012. 04. 30	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );	
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );	
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	SendPacket( EGS_PVP_PARTY_CHANGE_MATCH_INFO_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_REQUEST_MATCH_MAKING_REQ )
{
	//{{ 2012. 04. 04	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_REQUEST_MATCH_MAKING_REQ, EGS_REQUEST_MATCH_MAKING_ACK );
#else
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_REQUEST_MATCH_MAKING_REQ, EGS_REQUEST_MATCH_MAKING_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	KEGS_REQUEST_MATCH_MAKING_ACK kPacketAck;

	// �̹� ��ġ�� ��ϵ� �������� Ȯ���Ѵ�!
	if( GetMatchUID() != 0 )
	{
		START_LOG( cerr, L"�̹� ��ġ�� ��ϵ� �����Դϴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetMatchUID() )
			<< END_LOG;
		
		kPacketAck.m_iOK = NetError::ERR_MATCH_MAKING_00;
		SendPacket( EGS_REQUEST_MATCH_MAKING_ACK, kPacketAck );
		return;
	}

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// �۾���¥: 2013-09-26	// �ڼ���
	// ���� �ڵ� ��Ī ������ Ȯ��!!
	if( m_kUserDungeonManager.GetAutoPartyWaitNumber() != 0  ||  m_kUserDungeonManager.GetAutoPartyUID() != 0 )
	{
		START_LOG( cerr, L"���� �ڵ� ��Ī ���ε� ���� ���� ��Ī ��û�� ���Դ�!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_MATCH_MAKING_23;	// ���� ���� ��û �߿��� ����� �� ���� ����Դϴ�.
		SendPacket( EGS_REQUEST_MATCH_MAKING_ACK, kPacketAck );
		return;
	}

	// �� ����Ʈ ��ȸ ������ Ȯ��!!
	if( GetRoomListID() != 0 )
	{
		START_LOG( cerr, L"�� ����Ʈ ��ȸ �߿� ���� ���� ��Ī ��û�� ���Դ�!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_08;	// �� ����Ʈ ��ȸ �߿��� ����� �� ���� ����Դϴ�.
		SendPacket( EGS_REQUEST_MATCH_MAKING_ACK, kPacketAck );
		return;
	}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM

	// Ŭ���̾�Ʈ�� ���� ������ �����Ѵ�.
	switch( kPacket_.m_cPvpPlayerCount )
	{
	case 1:
	case 2:
	case 3:
		break;

	default:
		{
			START_LOG( cerr, L"�̻��� ���� �ο� ��û�Դϴ�." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetMatchUID() )
				<< BUILD_LOGc( kPacket_.m_cPvpPlayerCount )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_MATCH_MAKING_01;
			SendPacket( EGS_REQUEST_MATCH_MAKING_ACK, kPacketAck );
		}
		return;
	}

	if( kPacket_.m_cPvpGameType != CXSLRoom::PGT_TEAM  &&
		kPacket_.m_cPvpGameType != CXSLRoom::PGT_TEAM_DEATH )
	{
		START_LOG( cerr, L"�̻��� ���� Ÿ�� �Դϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetMatchUID() )
			<< BUILD_LOGc( kPacket_.m_cPvpGameType )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_MATCH_MAKING_02;
		SendPacket( EGS_REQUEST_MATCH_MAKING_ACK, kPacketAck );
		return;
	}

	// ���� ������ �ּҷ����� 10������ �Ǿ�� �Ѵ�.
	if( GetLevel() < SEnum::PE_PVP_OFFICIAL_LIMIT_LEVEL )
	{
		START_LOG( cerr, L"���� ������ �ϱ⿡ ������ ĳ���� �����Դϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetMatchUID() )
			<< BUILD_LOGc( GetLevel() )
			<< END_LOG;
		
		kPacketAck.m_iOK = NetError::ERR_MATCH_MAKING_03;
		SendPacket( EGS_REQUEST_MATCH_MAKING_ACK, kPacketAck );
		return;
	}
    
	// ��Ƽ�� ������ ��Ƽ�� ���� �� ���� ������ ����!
	if( GetPartyUID() == 0 )
	{
		KEGB_REQUEST_MATCH_MAKING_REQ kPacket;
		KMatchUserInfo kInfo;
		GetMatchUserInfo( kInfo );
		kPacket.m_bRegFrontOfList = false;
		kPacket.m_vecMatchUserInfo.push_back( kInfo );
		kPacket.m_cPvpPlayerCount = kPacket_.m_cPvpPlayerCount;
		kPacket.m_cPvpGameType = kPacket_.m_cPvpGameType;
		SendToMatch( EGB_REQUEST_MATCH_MAKING_REQ, kPacket );
	}
	else
	{
		KEPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ kPacketReq;
		kPacketReq.m_cPvpGameType = kPacket_.m_cPvpGameType;
		kPacketReq.m_cPvpPlayerCount = kPacket_.m_cPvpPlayerCount;
		SendToCnParty( EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ, kPacketReq );
	}
}

IMPL_ON_FUNC( EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_ACK )
{
	//{{ 2012. 04. 04	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_REQUEST_MATCH_MAKING_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_REQUEST_MATCH_MAKING_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_REQUEST_MATCH_MAKING_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_REQUEST_MATCH_MAKING_ACK, kPacketAck );
		return;
	}

	KEGB_REQUEST_MATCH_MAKING_REQ kPacket;
	kPacket.m_bRegFrontOfList = kPacket_.m_bRegFrontOfList;
	kPacket.m_cPvpGameType = kPacket_.m_cPvpGameType;
	kPacket.m_cPvpPlayerCount = kPacket_.m_cPvpPlayerCount;
	kPacket.m_vecMatchUserInfo = kPacket_.m_vecMatchUserInfo;
	SendToMatch( EGB_REQUEST_MATCH_MAKING_REQ, kPacket );
}

_IMPL_ON_FUNC( EGB_REQUEST_MATCH_MAKING_ACK, KEGS_REQUEST_MATCH_MAKING_ACK )
{
	//{{ 2012. 04. 04	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_REQUEST_MATCH_MAKING_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_REQUEST_MATCH_MAKING_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	SendPacket( EGS_REQUEST_MATCH_MAKING_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EGB_REG_MATCH_WAIT_LIST_SUCCESS_NOT, KEGS_REG_MATCH_WAIT_LIST_SUCCESS_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserPvpManager.IsRequestMatchMaking() == true )
	{
		START_LOG( cerr, L"�̹� ���� ��ġ ����Ʈ�� ��ϵǾ��ִµ� �� ����ϴ°� �Գ�? �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iEstimatedTime )
			<< END_LOG;
	}

	// ��ġ ����ŷ ��û���� ������Ʈ
	m_kUserPvpManager.SetFlagRequestMatchMaking( true );

	SendPacket( EGS_REG_MATCH_WAIT_LIST_SUCCESS_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_CANCEL_MATCH_MAKING_REQ )
{
	//{{ 2012. 04. 04	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CANCEL_MATCH_MAKING_REQ, EGS_CANCEL_MATCH_MAKING_ACK );
#else
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CANCEL_MATCH_MAKING_REQ, EGS_CANCEL_MATCH_MAKING_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	KEGS_CANCEL_MATCH_MAKING_ACK kPacket;

	if( m_kUserPvpManager.IsRequestMatchMaking() == false  &&  GetMatchUID() == 0 )
	{
		START_LOG( cerr, L"���� ��Ī�� ��û������ ���µ� ��ҿ�û�� �Գ�?" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetRoomListID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_MATCH_MAKING_12;
        SendToMatch( EGS_CANCEL_MATCH_MAKING_ACK, kPacket );
		return;
	}

	KEGB_CANCEL_MATCH_MAKING_REQ kPacketReq;
	kPacketReq.m_iMatchUID = GetMatchUID();
	kPacketReq.m_iPartyUID = GetPartyUID();
	kPacketReq.m_iReason = NetError::NOT_LEAVE_MATCH_WAIT_REASON_01;
	kPacketReq.m_bSendBySystem = false;	
    SendToMatch( EGB_CANCEL_MATCH_MAKING_REQ, kPacketReq );
}

_IMPL_ON_FUNC( EGB_CANCEL_MATCH_MAKING_ACK, KEGS_CANCEL_MATCH_MAKING_ACK )
{
	//{{ 2012. 04. 04	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CANCEL_MATCH_MAKING_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CANCEL_MATCH_MAKING_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
	}

	SendPacket( EGS_CANCEL_MATCH_MAKING_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EGB_MATCH_MAKING_SUCCESS_NOT, KEGS_MATCH_MAKING_SUCCESS_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ��ġ ����
	m_kUserPvpManager.SetMatchUID( kPacket_.m_iMatchUID );

	SendPacket( EGS_MATCH_MAKING_SUCCESS_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_MATCH_MAKING_SUCCESS_REPLY_NOT )
{
	//{{ 2012. 04. 04	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	if( GetMatchUID() == 0 )
	{
		START_LOG( cerr, L"���� ��ġ�� ��ϵ� ���°� �ƴѵ� ��ġ ������ ���� ������ �Ϸ��� �Ѵ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_bGameStartAgree )
			<< END_LOG;
		return;
	}

	// ������ ��ȣ �������� 2�� �̻����� üũ�Ѵ�.
	if( kPacket_.m_setSelectedPvpMap.size() > 2 )
	{
        START_LOG( cerr, L"������ ������ ��ȣ �������� 2�� �̻��̴�. ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_setSelectedPvpMap.size() )
			<< END_LOG;

		// ���⼭ ����!
		return;
	}

	SendToMatch( EGB_MATCH_MAKING_SUCCESS_REPLY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EGB_UNREG_MATCH_WAIT_LIST_NOT, KEGS_UNREG_MATCH_WAIT_LIST_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserPvpManager.IsRequestMatchMaking() == false )
	{
		START_LOG( cerr, L"��ġ ����Ʈ ��������� ���µ� �����Ұ� �Գ�?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iReason ) )
			<< END_LOG;
	}

	// ��ġ ��û ���� ����
	m_kUserPvpManager.SetFlagRequestMatchMaking( false );

	if( kPacket_.m_iReason != NetError::NOT_LEAVE_MATCH_WAIT_REASON_00 )
	{
		SendPacket( EGS_UNREG_MATCH_WAIT_LIST_NOT, kPacket_ );
	}
}

_IMPL_ON_FUNC( EGB_MATCH_CLOSE_NOT, KEGS_MATCH_CLOSE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetMatchUID() == 0 )
	{
		START_LOG( cerr, L"�������� ��ġ�� ������ �ƴѵ� ��ġ ��Ұ� �Դ�?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iReason ) )
			<< END_LOG;
	}

	// ��ġ UID������Ʈ
	m_kUserPvpManager.SetMatchUID( 0 );


	// Ŭ���̾�Ʈ�� �˸���!
	if( kPacket_.m_iReason != NetError::NOT_LEAVE_MATCH_REASON_00 )
	{
		SendPacket( EGS_MATCH_CLOSE_NOT, kPacket_ );
	}
}

IMPL_ON_FUNC( EGB_REMATCH_MAKING_USER_NOT )
{
	//{{ 2012. 04. 04	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	// �̹� ��ġ�� ��ϵ� �������� Ȯ���Ѵ�!
	if( GetMatchUID() != 0 )
	{
		START_LOG( cerr, L"�̹� ��ġ�� ��ϵ� �����Դϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetMatchUID() )
			<< END_LOG;
		return;
	}

	// Ŭ���̾�Ʈ�� ���� ������ �����Ѵ�.
	switch( kPacket_.m_cPvpPlayerCount )
	{
	case 1:
	case 2:
	case 3:
		break;

	default:
		{
			START_LOG( cerr, L"�̻��� ���� �ο� ��û�Դϴ�." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetMatchUID() )
				<< BUILD_LOGc( kPacket_.m_cPvpPlayerCount )
				<< END_LOG;
		}
		return;
	}

	if( kPacket_.m_cPvpGameType != CXSLRoom::PGT_TEAM  &&
		kPacket_.m_cPvpGameType != CXSLRoom::PGT_TEAM_DEATH )
	{
		START_LOG( cerr, L"�̻��� ���� Ÿ�� �Դϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetMatchUID() )
			<< BUILD_LOGc( kPacket_.m_cPvpGameType )
			<< END_LOG;
		return;
	}

	// ���� ������ �ּҷ����� 10������ �Ǿ�� �Ѵ�.
	if( GetLevel() < SEnum::PE_PVP_OFFICIAL_LIMIT_LEVEL )
	{
		START_LOG( cerr, L"���� ������ �ϱ⿡ ������ ĳ���� �����Դϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetMatchUID() )
			<< BUILD_LOGc( GetLevel() )
			<< END_LOG;
		return;
	}

	// ��Ƽ�� ������ ��Ƽ�� ���� �� ���� ������ ����!
	if( GetPartyUID() == 0 )
	{
		KEGB_REQUEST_MATCH_MAKING_REQ kPacket;
		KMatchUserInfo kInfo;
		GetMatchUserInfo( kInfo );
		kPacket.m_bRegFrontOfList = true; // ���� ��Ī ���û�� ���� ����Ʈ �ֻ����� ���!
		kPacket.m_vecMatchUserInfo.push_back( kInfo );
		kPacket.m_cPvpPlayerCount = kPacket_.m_cPvpPlayerCount;
		kPacket.m_cPvpGameType = kPacket_.m_cPvpGameType;
		SendToMatch( EGB_REQUEST_MATCH_MAKING_REQ, kPacket );
	}
	else
	{
		KEPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ kPacketReq;
		kPacketReq.m_bRegFrontOfList = true; // ���� ��Ī ���û�� ���� ����Ʈ �ֻ����� ���!
		kPacketReq.m_cPvpGameType = kPacket_.m_cPvpGameType;
		kPacketReq.m_cPvpPlayerCount = kPacket_.m_cPvpPlayerCount;
		SendToCnParty( EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ, kPacketReq );
	}    
}

IMPL_ON_FUNC_NOPARAM( EGB_GET_ROOM_USER_INFO_FOR_MATCH_REQ )
{
	//{{ 2012. 04. 04	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	if( GetMatchUID() == 0 )
	{
		START_LOG( cerr, L"���� ��ġ�� ��ϵ� ���°� �ƴѵ� ��ġ ������ ���� ������ �Ϸ��� �Ѵ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

	// �� ��Ŷ�� ���� ���Ǽ� ��ġ RoomListID�� �������!
	SetRoomListID( SiKRoomListManager()->GetPvpOffcialRoomListID() );


    KEGB_GET_ROOM_USER_INFO_FOR_MATCH_ACK kPacket;
	GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_PVP ); // ä�� Ŭ���� �����ϴ°� �����α�..
	SendToMatch( EGB_GET_ROOM_USER_INFO_FOR_MATCH_ACK, kPacket );
}

IMPL_ON_FUNC( EGB_OPEN_PVP_ROOM_FOR_MATCH_REQ )
{
	//{{ 2012. 04. 04	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetRoomUID() != 0 )
	{
		if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
		{
			KERM_LEAVE_ROOM_REQ kPacket;
			kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_32;
			SendToCnRoom( ERM_LEAVE_BATTLE_FIELD_FOR_PVP_GAME_START_REQ, kPacket );

			// ��Żó�� ������ ���⼭ ��!
			SetRoomUID( 0 );

			if( GetStateID() == KGSFSM::S_ROOM )
			{
				StateTransition( KGSFSM::I_TO_FIELD_MAP );
			}
		}		
		else
		{
			START_LOG( cerr, L"�̹� �濡 �������ִ�! ���� ��Ȳ�� �����ϴ°��ΰ�?" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< END_LOG;

			KEGB_OPEN_PVP_ROOM_FOR_MATCH_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_MATCH_MAKING_13;
			SendToMatch( EGB_OPEN_PVP_ROOM_FOR_MATCH_ACK, kPacket );
			return;
		}
	}
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( GetRoomUID() != 0 )
	{
		START_LOG( cerr, L"�̹� �濡 �������ִ�! ���� ��Ȳ�� �����ϴ°��ΰ�?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
			<< END_LOG;

		KEGB_OPEN_PVP_ROOM_FOR_MATCH_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MATCH_MAKING_13;
		SendToMatch( EGB_OPEN_PVP_ROOM_FOR_MATCH_ACK, kPacket );
		return;
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	// �������� �����Ϸ� ����!
	kPacket_.m_kRoomInfo.m_uiRoomListID = SiKRoomListManager()->GetPvpOffcialRoomListID();
	SendToCnRoom( ERM_OPEN_PVP_ROOM_FOR_MATCH_REQ, kPacket_ );
}

//{{ 2012. 06. 05	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
_IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_PVP_GAME_START_ACK, KEGS_LEAVE_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

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
		if( GetTradeUID() != 0 )
		{
			KEGS_BREAK_TRADE_REQ kPacket;
			kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
			_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
		}
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

_IMPL_ON_FUNC( ERM_OPEN_PVP_ROOM_FOR_MATCH_ACK, KEGB_OPEN_PVP_ROOM_FOR_MATCH_ACK )
{
	//{{ 2012. 04. 04	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"������ ���� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}

	// ���� ������!
	SendToMatch( EGB_OPEN_PVP_ROOM_FOR_MATCH_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_GAME_START_PVP_MATCH_NOT, KEGS_GAME_START_PVP_MATCH_NOT )
{
	//{{ 2012. 04. 04	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetRoomUID() != 0 )
	{
		if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
		{
			KERM_LEAVE_ROOM_REQ kPacket;
			kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_32;
			SendToCnRoom( ERM_LEAVE_BATTLE_FIELD_FOR_PVP_GAME_START_REQ, kPacket );

			// ��Żó�� ������ ���⼭ ��!
			SetRoomUID( 0 );

			if( GetStateID() == KGSFSM::S_ROOM )
			{
				StateTransition( KGSFSM::I_TO_FIELD_MAP );
			}
		}
	}
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	// �濡 ���� ó��
	SetRoomUID( kPacket_.m_RoomInfo.m_RoomUID );

	// �� ���·� �̵�
	StateTransition( KGSFSM::I_TO_ROOM );
	m_kTimer[TM_ROOM].restart();

	// �ʵ忡�� ��Ż
	SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );

	//{{ 2009. 10. 8  ������	���
#ifdef GUILD_TEST
	SendUpdateGuildMemberInfo();
#endif GUILD_TEST
	//}}

#ifdef SERV_PVP_REMATCH
	SetRoomListID( SiKRoomListManager()->GetPvpOffcialRoomListID() ); // 2013.05.13 lygan_������ // ���� ���� ���� Rematch ���� ����
#endif SERV_PVP_REMATCH

	SendPacket( EGS_GAME_START_PVP_MATCH_NOT, kPacket_ );

	// ������ ���� ���� ������Ʈ
	KERM_UPDATE_PVP_PLAY_UNIT_INFO_NOT kPacketNot;
	kPacketNot.m_kPvpUnitInfo.m_iRPoint = GetRPoint();
	kPacketNot.m_kPvpUnitInfo.m_iAPoint = GetAPoint();
	kPacketNot.m_kPvpUnitInfo.m_iWinCount = GetWin();
	kPacketNot.m_kPvpUnitInfo.m_bIsPlayWithParty = ( GetPartyUID() != 0 );
	SendToCnRoom( ERM_UPDATE_PVP_PLAY_UNIT_INFO_NOT, kPacketNot );
}

IMPL_ON_FUNC( EGS_PVP_NPC_UNIT_RE_BIRTH_POS_REQ )
{
	// ������ �ߺ����� �ü� �ֱ� ������ �ߺ� ���� ���� �ʴ´�.
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
    
	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"�濡 ���µ� ���� NPC��Ȱ��Ŷ�� ���?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_PVP_NPC_UNIT_RE_BIRTH_POS_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MATCH_MAKING_14;
		SendPacket( EGS_PVP_NPC_UNIT_RE_BIRTH_POS_ACK, kPacket );
		return;
	}

	SendToCnRoom( ERM_PVP_NPC_UNIT_RE_BIRTH_POS_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_PVP_NPC_UNIT_RE_BIRTH_POS_ACK, KEGS_PVP_NPC_UNIT_RE_BIRTH_POS_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PVP_NPC_UNIT_RE_BIRTH_POS_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_PVP_NPC_UNIT_INFO_TO_SERVER_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_PVP_NPC_UNIT_INFO_TO_SERVER_REQ, EGS_PVP_NPC_UNIT_INFO_TO_SERVER_ACK );

	SendToCnRoom( ERM_PVP_NPC_UNIT_INFO_TO_SERVER_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_PVP_NPC_UNIT_INFO_TO_SERVER_ACK, KEGS_PVP_NPC_UNIT_INFO_TO_SERVER_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_PVP_NPC_UNIT_INFO_TO_SERVER_ACK );

	SendPacket( EGS_PVP_NPC_UNIT_INFO_TO_SERVER_ACK, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGB_CHECK_INVALID_MATCH_NOT )
{
	if( IsRequestMatchMaking() == true )
	{
		// ��ġ ��û ���� ����
		m_kUserPvpManager.SetFlagRequestMatchMaking( false );

		KEGS_UNREG_MATCH_WAIT_LIST_NOT kPacketNot;
		kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_WAIT_REASON_01;
		SendPacket( EGS_UNREG_MATCH_WAIT_LIST_NOT, kPacketNot );
	}
	
	if( GetMatchUID() != 0 )
	{
		// ��ġ UID������Ʈ
		m_kUserPvpManager.SetMatchUID( 0 );

		// Ŭ���̾�Ʈ�� �˸���!
		KEGS_MATCH_CLOSE_NOT kPacketNot;
		kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_02;
		SendPacket( EGS_MATCH_CLOSE_NOT, kPacketNot );
	}
}

#endif SERV_PVP_NEW_SYSTEM
//}}


//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


