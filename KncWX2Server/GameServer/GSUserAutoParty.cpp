#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"
#include "Socket/NetCommon.h"

#include "BaseServer.h"

#include "Enum/Enum.h"
//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLBuffManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}


#define CLASS_TYPE      KGSUser


IMPL_ON_FUNC( EGS_AUTO_PARTY_DUNGEON_GAME_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_AUTO_PARTY_DUNGEON_GAME_REQ, EGS_AUTO_PARTY_DUNGEON_GAME_ACK );

	if( GetRoomUID() != 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_BATTLE_FIELD )
	{
		START_LOG( cerr, L"�ڵ� ��Ƽ ��û�� �Ҽ� ���� ������ ��û ��û�� �Ͽ����ϴ�!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_01;
		SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}

	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	// �������� ���� �޽� ������ �ɷ������� �ڵ� ��Ƽ ��û�� ���� ���Ѵ�.
	if( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY ) == true )
	{
		START_LOG( clog, L"�������� ���� �޽Ĺ����� ���� �ڵ� ��Ƽ ��û�� �Ҽ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_02;
		SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}}

	// �ڵ� ��Ī ����� �Ҽ� �ִ��� ���� üũ
	if( m_kUserDungeonManager.GetAutoPartyWaitNumber() != 0  ||  m_kUserDungeonManager.GetAutoPartyUID() != 0 )
	{
		START_LOG( cerr, L"�̹� �ڵ� ��Ƽ ��� �����Դϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyWaitNumber() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyUID() )
			<< END_LOG;

		KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_03;
		SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// �۾���¥: 2013-09-26	// �ڼ���
	// ���� ���� ��Ī ������ Ȯ��!!
	if( GetMatchUID() != 0 )
	{
		START_LOG( cerr, L"���� ���� ��Ī ���ε� ���� �ڵ� ��Ī ��û�� ���Դ�!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_07;	// ���� ���� ��û �߿��� ����� �� ���� ����Դϴ�.
		SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}

	// �� ����Ʈ ��ȸ ������ Ȯ��!!
	if( GetRoomListID() != 0 )
	{
		START_LOG( cerr, L"�� ����Ʈ ��ȸ �߿� ���� �ڵ� ��Ī ��û�� ���Դ�!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_08;	// �� ����Ʈ ��ȸ �߿��� ����� �� ���� ����Դϴ�.
		SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM

	// DungeonID����
	if( !SiCXSLDungeonManager()->VerifyDungeonID( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel) ) )
	{
		START_LOG( cerr, L"���������ʴ� DungeonID�Դϴ�. Ŭ�󿡼� �˻������ٵ�.." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
			<< END_LOG;

		KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_04;
		SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}

	// HenirDungeonMode����
	if( CXSLDungeon::IsValidDungeonMode( static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_cDungeonMode) ) == false )
	{
		START_LOG( cerr, L"���������� HenirDungeonMode���Դϴ�." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOG( (int)kPacket_.m_cDungeonMode )
			<< END_LOG;

		KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_04;
		SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}

	// ���� ��Ƽ �Ҽ��̳� �ƴϳĿ� ���� �ٸ��� ó���Ѵ�.
	if( GetPartyUID() == 0 )
	{
		////////////////////////////////////////////////////////////////////////// ���� ����
		if( CheckForDungeonGameStart( kPacket_.m_cDungeonMode, kPacket_.m_iDungeonID, kPacket_.m_DifficultyLevel, true, true ) == false )
		{
			KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::GetLastError();
			kPacketAck.m_wstrFailUserNickName = GetCharName();
			SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
			return;
		}

		// ��Ƽ�� �ҼӵǾ� ���� �ʴٸ� KAutoPartyUserInfo �� ���� GSUser�κ��� �� ��Ī ����� �õ� ����!
		KEGB_REQUEST_AUTO_PARTY_MAKING_REQ kPacketReq;
		kPacketReq.m_kAutoPartyCondition.m_iDungeonID = kPacket_.m_iDungeonID;
		kPacketReq.m_kAutoPartyCondition.m_cDifficultyLevel = kPacket_.m_DifficultyLevel;
		kPacketReq.m_kAutoPartyCondition.m_cGetItemType = kPacket_.m_cGetItemType;
		kPacketReq.m_kAutoPartyCondition.m_cDungeonMode = kPacket_.m_cDungeonMode;
		KAutoPartyUserInfo kInfo;
		GetAutoPartyUserInfo( kInfo );
		kPacketReq.m_vecAutoPartyUser.push_back( kInfo );
		SendToAutoParty( EGB_REQUEST_AUTO_PARTY_MAKING_REQ, kPacketReq );
	}
	else
	{
		// ��Ƽ�� �Ҽ� �Ǿ� �ִٸ� ���� ��Ƽ������ ������ ������ ����!
		KEPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ kPacket;
		kPacket.m_iPartyUID = GetPartyUID();
		SendToCnParty( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ, kPacket );
	}
}

IMPL_ON_FUNC( EPM_CHECK_FOR_AUTO_PARTY_MAKING_REQ )
{
	VERIFY_STATE_WARN( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ��Ƽ �Ҽ����� Ȯ��!
	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ �Ҽ��� �ƴѵ� �̷� ��Ŷ�� ���ƿԳ�?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
		return;
	}

	// �ڵ� ��Ƽ�� ������ ������ �����ϴ��� üũ����!
	KEPM_CHECK_FOR_AUTO_PARTY_MAKING_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	// �������� ���� �޽� ������ �ɷ������� �ڵ� ��Ƽ ��û�� ���� ���Ѵ�.
	if( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY ) == true )
	{
		START_LOG( clog, L"�������� ���� �޽Ĺ����� ���� �ڵ� ��Ƽ ��û�� �Ҽ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_AUTO_PARTY_02;
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}}

	// �ش� ��Ƽ���� ������ ������ ������ �����ϴ��� üũ!
	if( CheckForDungeonGameStart( kPacket_.m_cDungeonMode, kPacket_.m_iDungeonID, kPacket_.m_cDifficulty, true, false ) == false )
	{	
		kPacket.m_iOK = NetError::GetLastError();
		kPacket.m_wstrFailUserNickName = GetCharName();
	}

	SendToCnParty( EPM_CHECK_FOR_AUTO_PARTY_MAKING_ACK, kPacket );
}

IMPL_ON_FUNC( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_AUTO_PARTY_DUNGEON_GAME_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		if( kPacket_.m_iOK == NetError::ERR_DUNGEON_REQUIRE_ITEM_00 )
		{
			KEGS_ERROR_POPUP_NOT kNot;
			kNot.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
			//{{ ���� ����� ���� �˾� ����
#ifdef SERV_CHANGE_ERROR_POPUP
			kNot.m_bPartyMember = true;
#endif SERV_CHANGE_ERROR_POPUP
			//}}
			const int iDungeonID = kPacket_.m_kAutoPartyCondition.m_iDungeonID + static_cast<int>(kPacket_.m_kAutoPartyCondition.m_cDifficultyLevel);
			const CXSLDungeon::DUNGEON_MODE eDungeonMode = static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_kAutoPartyCondition.m_cDungeonMode);
			if( SiCXSLDungeonManager()->GetRequireItemIDAndCount( iDungeonID, eDungeonMode, kNot.m_iErrorData1, kNot.m_iErrorData2 ) == true )
			{
				SendPacket( EGS_ERROR_POPUP_NOT, kNot );
			}
			else
			{
				START_LOG( cerr, L"���� ����� ������ ã�� ���ߴ�!" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( iDungeonID )
					<< BUILD_LOG( eDungeonMode )
					<< END_LOG;
			}
		}
		//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
		else if( kPacket_.m_iOK != NetError::ERR_PARTY_61 )
#else
		else
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
			//}}
		{
			START_LOG( cerr, L"�ڵ� ��Ƽ ��Ī ���� ������ ����!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
				<< END_LOG;
		}

		KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		kPacketAck.m_wstrFailUserNickName = kPacket_.m_wstrFailUserNickName;
		SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}

	// ��Ƽ ������ ���������� �ٽ� ��û�Ϸ� ����!
	KEGB_REQUEST_AUTO_PARTY_MAKING_REQ kPacketReq;
	kPacketReq.m_bRemakingAutoParty = kPacket_.m_bRemakingAutoParty; // ���Ͽ���!
	kPacketReq.m_kAutoPartyCondition = kPacket_.m_kAutoPartyCondition;
	kPacketReq.m_vecAutoPartyUser = kPacket_.m_vecAutoPartyUser;
	SendToAutoParty( EGB_REQUEST_AUTO_PARTY_MAKING_REQ, kPacketReq );
}

_IMPL_ON_FUNC( EGB_REQUEST_AUTO_PARTY_MAKING_ACK, KEGS_AUTO_PARTY_DUNGEON_GAME_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_AUTO_PARTY_DUNGEON_GAME_ACK );

	SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EGB_REG_AUTO_PARTY_WAIT_LIST_SUCCESS_NOT, KEGS_REG_AUTO_PARTY_WAIT_LIST_SUCCESS_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserDungeonManager.GetAutoPartyWaitNumber() != 0 )
	{
		START_LOG( cerr, L"�̹� �ڵ� ��Ƽ ������ε� �� ����ϴ°� �Գ�? �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyWaitNumber() )
			<< BUILD_LOG( kPacket_.m_iWaitNumber )
			<< BUILD_LOG( kPacket_.m_iEstimatedTime )
			<< END_LOG;

		// �ϴ��� �ߺ� ������ ���ϱ� ���� ������ ����Ǿ��ִ����� ��� ��Ű��!
		KEGB_CANCEL_AUTO_PARTY_MAKING_REQ kPacketReq;
		kPacketReq.m_iAutoPartyWaitNumber = m_kUserDungeonManager.GetAutoPartyWaitNumber();
		kPacketReq.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_01;
		kPacketReq.m_bSendBySystem = true;
		SendToAutoParty( EGB_CANCEL_AUTO_PARTY_MAKING_REQ, kPacketReq );
	}

	// ���ǥ ��ȣ�� ��������!
	m_kUserDungeonManager.SetAutoPartyWaitNumber( kPacket_.m_iWaitNumber );

	SendPacket( EGS_REG_AUTO_PARTY_WAIT_LIST_SUCCESS_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EGB_AUTO_PARTY_MAKING_SUCCESS_NOT, KEGS_AUTO_PARTY_MAKING_SUCCESS_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// �̹� ��ϵǾ� �ִٱ�?
	if( m_kUserDungeonManager.GetAutoPartyUID() != 0 )
	{
		START_LOG( cerr, L"�̹� �ڵ� ��Ƽ �ε� �� �ڵ���Ƽ�� �Ƴ�? �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyUID() )
			<< BUILD_LOG( kPacket_.m_iAutoPartyUID )
			<< END_LOG;

		// �ϴ��� �ߺ� ������ ���ϱ� ���� ������ ����Ǿ��ִ����� ��� ��Ű��!
		KEGB_CANCEL_AUTO_PARTY_MAKING_REQ kPacketReq;
		kPacketReq.m_iAutoPartyWaitNumber = m_kUserDungeonManager.GetAutoPartyUID();
		kPacketReq.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_01;
		kPacketReq.m_bSendBySystem = true;
		SendToAutoParty( EGB_CANCEL_AUTO_PARTY_MAKING_REQ, kPacketReq );
	}

	// �ڵ� ��Ƽ UID�� ����!
	m_kUserDungeonManager.SetAutoPartyUID( kPacket_.m_iAutoPartyUID );

	// ��д��� �̶�� ����
	if( CXSLDungeon::IsSecretDungeon( kPacket_.m_iDungeonID ) == true )
	{
		kPacket_.m_iDungeonID = ( (kPacket_.m_iDungeonID / 10) * 10 );
	}

	SendPacket( EGS_AUTO_PARTY_MAKING_SUCCESS_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserDungeonManager.GetAutoPartyUID() == 0 )
	{
		START_LOG( clog, L"���� �ڵ� ��Ƽ�� ��ϵ� ���°� �ƴѵ� ������ �Ϸ��� �ϳ�? �ð��� �����ΰ�?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_bGameStartAgree )
			<< END_LOG;
		return;
	}

	SendToAutoParty( EGB_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_CANCEL_AUTO_PARTY_MAKING_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CANCEL_AUTO_PARTY_MAKING_REQ, EGS_CANCEL_AUTO_PARTY_MAKING_ACK );

	KEGS_CANCEL_AUTO_PARTY_MAKING_ACK kPacket;

	if( m_kUserDungeonManager.GetAutoPartyWaitNumber() == 0  &&  m_kUserDungeonManager.GetAutoPartyUID() == 0 )
	{
		START_LOG( cerr, L"�ڵ� ��Ƽ�� ��û������ ���µ� ��ҿ�û�� �Գ�?" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetRoomListID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_AUTO_PARTY_06;
		SendToMatch( EGS_CANCEL_AUTO_PARTY_MAKING_ACK, kPacket );
		return;
	}

	KEGB_CANCEL_AUTO_PARTY_MAKING_REQ kPacketReq;
	kPacketReq.m_iAutoPartyUID = m_kUserDungeonManager.GetAutoPartyUID();
	kPacketReq.m_iAutoPartyWaitNumber = m_kUserDungeonManager.GetAutoPartyWaitNumber();
	kPacketReq.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_01;
	kPacketReq.m_bSendBySystem = false;	
	SendToAutoParty( EGB_CANCEL_AUTO_PARTY_MAKING_REQ, kPacketReq );
}

_IMPL_ON_FUNC( EGB_CANCEL_AUTO_PARTY_MAKING_ACK, KEGS_CANCEL_AUTO_PARTY_MAKING_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CANCEL_AUTO_PARTY_MAKING_ACK );

	SendPacket( EGS_CANCEL_AUTO_PARTY_MAKING_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EGB_UNREG_AUTO_PARTY_WAIT_LIST_NOT, KEGS_UNREG_AUTO_PARTY_WAIT_LIST_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserDungeonManager.GetAutoPartyWaitNumber() == 0 )
	{
		START_LOG( cerr, L"�������� �ڵ� ��Ƽ ��û�� ������ �ƴѵ� �Դ�?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iReason ) )
			<< END_LOG;
	}

	// ��ġ ��û ���� ����
	m_kUserDungeonManager.SetAutoPartyWaitNumber( 0 );

	if( kPacket_.m_iReason != NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_00 )
	{
		SendPacket( EGS_UNREG_AUTO_PARTY_WAIT_LIST_NOT, kPacket_ );
	}
}

IMPL_ON_FUNC( EGB_REMAKING_AUTO_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// �ڵ� ��Ī ����� �Ҽ� �ִ��� ���� üũ
	if( m_kUserDungeonManager.GetAutoPartyWaitNumber() != 0  ||  m_kUserDungeonManager.GetAutoPartyUID() != 0 )
	{
		START_LOG( cerr, L"�̹� �ڵ� ��Ƽ ��� �����Դϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyWaitNumber() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyUID() )
			<< END_LOG;
		return;
	}

	// DungeonID����
	if( !SiCXSLDungeonManager()->VerifyDungeonID( kPacket_.m_kAutoPartyInfo.m_iDungeonID + static_cast<int>(kPacket_.m_kAutoPartyInfo.m_cDifficultyLevel) ) )
	{
		START_LOG( cerr, L"���������ʴ� DungeonID�Դϴ�. Ŭ�󿡼� �˻������ٵ�.." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_kAutoPartyInfo.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_kAutoPartyInfo.m_cDifficultyLevel )
			<< END_LOG;
		return;
	}

	// HenirDungeonMode����
	if( CXSLDungeon::IsValidDungeonMode( static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_kAutoPartyInfo.m_cDungeonMode) ) == false )
	{
		START_LOG( cerr, L"���������� HenirDungeonMode���Դϴ�." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_kAutoPartyInfo.m_iDungeonID )
			<< BUILD_LOG( (int)kPacket_.m_kAutoPartyInfo.m_cDungeonMode )
			<< END_LOG;
		return;
	}

	//::1. �ټ��� �˻�.
	if( CheckingSpirit( kPacket_.m_kAutoPartyInfo.m_iDungeonID ) == false )
	{
		return;
	}

	// �ڵ� ��Ī ���� ���� üũ!	


	// �ڵ� ��Ī ������ ���� ���� ���� üũ!


	// ���� ��Ƽ �Ҽ��̳� �ƴϳĿ� ���� �ٸ��� ó���Ѵ�.
	if( GetPartyUID() == 0 )
	{
		// ��Ƽ�� �ҼӵǾ� ���� �ʴٸ� KAutoPartyUserInfo �� ���� GSUser�κ��� �� ��Ī ����� �õ� ����!
		KEGB_REQUEST_AUTO_PARTY_MAKING_REQ kPacketReq;
		kPacketReq.m_bRemakingAutoParty = true; // ����!!
		kPacketReq.m_kAutoPartyCondition = kPacket_.m_kAutoPartyInfo;
		KAutoPartyUserInfo kInfo;
		GetAutoPartyUserInfo( kInfo );
		kPacketReq.m_vecAutoPartyUser.push_back( kInfo );
		SendToAutoParty( EGB_REQUEST_AUTO_PARTY_MAKING_REQ, kPacketReq );
	}
	else
	{
		// ��Ƽ�� �Ҽ� �Ǿ� �ִٸ� ���� ��Ƽ������ ������ ������ ����!
		KEPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ kPacket;
		kPacket.m_bRemakingAutoParty = true; // ����!!
		kPacket.m_iPartyUID = GetPartyUID();
		SendToCnParty( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ, kPacket );
	}
}

_IMPL_ON_FUNC( EGB_AUTO_PARTY_CLOSE_NOT, KEGS_AUTO_PARTY_CLOSE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserDungeonManager.GetAutoPartyUID() == 0 )
	{
		START_LOG( cerr, L"�������� �ڵ� ��Ƽ�� ������ �ƴѵ� �Դ�?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iReason ) )
			<< END_LOG;
	}

	// �ڵ� ��Ƽ UID������Ʈ
	m_kUserDungeonManager.SetAutoPartyUID( 0 );


	// Ŭ���̾�Ʈ�� �˸���!
	if( kPacket_.m_iReason != NetError::NOT_LEAVE_AUTO_PARTY_REASON_00 )
	{
		SendPacket( EGS_AUTO_PARTY_CLOSE_NOT, kPacket_ );
	}
}

IMPL_ON_FUNC( EGB_GET_REGROUP_PARTY_INFO_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserDungeonManager.GetAutoPartyUID() == 0  ||  m_kUserDungeonManager.GetAutoPartyUID() != kPacket_.m_iAutoPartyUID )
	{
		START_LOG( cerr, L"���� �ڵ� ��Ƽ�� ��ϵ� ���°� �ƴѵ� ��Ƽ ���ġ�� ���� ������ �Ϸ��� �Ѵ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyUID() )
			<< BUILD_LOG( kPacket_.m_iAutoPartyUID )
			<< END_LOG;
		return;
	}

	// ���� ��Ƽ�� �ҼӵǾ� �ְų� ��Ƽ �ʴ� ����ڶ�� �ڵ����� ��Ż ��Ų��!
	if( GetPartyUID() > 0  ||  GetCandidatePartyUID() > 0 )
	{
		// ���� ������ ��Ƽ�� �ҼӵǾ��־��ٸ� ���� ��Ż ��Ų��!
		SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_06 );

		// ��Ƽ �����UID�� ����
		SetCandidatePartyUID( 0 );

		//��Ƽ UID�� �����ϰ�.
		SetPartyUID( 0 );
	}

	// ������ ������ �ٽ� ��������.
	KEGB_GET_REGROUP_PARTY_INFO_ACK kPacket;
	GetPartyUserInfo( kPacket.m_kPartyUserInfo );

	// ���� ���� ���� ����
	m_kUserDungeonManager.SetDungeonGameInfo( kPacket_.m_kDungeonGameInfo );

	GetRoomUserInfo( kPacket.m_kPartyRoomUserInfo.m_kRoomUserInfo, CXSLRoom::RT_DUNGEON );
	GetTutorUnitUIDList( kPacket.m_kPartyRoomUserInfo.m_vecStudentUnitUID );
	kPacket.m_kPartyRoomUserInfo.m_iCurEXP = GetEXP();
	kPacket.m_kPartyRoomUserInfo.m_iCurED = GetED();
	kPacket.m_kPartyRoomUserInfo.m_wstrChannelIP = NetCommon::GetLocalIPW();
	SendToAutoParty( EGB_GET_REGROUP_PARTY_INFO_ACK, kPacket );	
}

_IMPL_ON_FUNC( EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ, KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserDungeonManager.GetAutoPartyUID() == 0  ||  m_kUserDungeonManager.GetAutoPartyUID() != kPacket_.m_iAutoPartyUID )
	{
		START_LOG( cerr, L"���� �ڵ� ��Ƽ�� ��ϵ� ���°� �ƴѵ� ��Ƽ ���ġ�� ���� ������ �Ϸ��� �Ѵ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyUID() )
			<< BUILD_LOG( kPacket_.m_iAutoPartyUID )
			<< END_LOG;

		KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_06;
		kPacketAck.m_iAutoPartyUID = kPacket_.m_iAutoPartyUID;
		SendToAutoParty( EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}

	if( GetPartyUID() > 0  ||  GetCandidatePartyUID() > 0 )
	{
		START_LOG( cerr, L"���� �������� ��Ƽ �Ҽ��� �Ǹ� �ȵȴ�! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyUID() )
			<< BUILD_LOG( kPacket_.m_iAutoPartyUID )
			<< END_LOG;

		KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_06;
		kPacketAck.m_iAutoPartyUID = kPacket_.m_iAutoPartyUID;
		SendToAutoParty( EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}

	// ���� ��Ƽ���� �Ѹ��̶�� ���� ��Ƽ�� ���� ������ ������ �ʿ䰡 ����!
	if( kPacket_.m_vecRegroupPartyUserInfo.size() == 1 )
	{
		// �������� ���� ���� ����!
		if( GetRoomUID() == 0 )
		{
			// ���� ���� ����鼭 ���� ������ ��������!
			DungeonGameStart_SingleOpenDungeonRoom( kPacket_.m_kCreatePartyInfo.m_cDungeonMode, kPacket_.m_kCreatePartyInfo.m_iDungeonID, kPacket_.m_kCreatePartyInfo.m_DifficultyLevel, true );

			// �̱� ���� ���� ó���ǰ� ���� �ٷ� AutoParty�� ������ ����!
			KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::NET_OK;
			kPacketAck.m_iAutoPartyUID = kPacket_.m_iAutoPartyUID;
			SendToAutoParty( EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		}
		else
		{
			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_BATTLE_FIELD:
				{
					KERM_LEAVE_ROOM_REQ kPacket;
					kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_28;
					SendToCnRoom( ERM_LEAVE_BATTLE_FIELD_FOR_DUNGEON_GAME_START_REQ, kPacket );

					// ��Żó�� ������ ���⼭ ��!
					SetRoomUID( 0 );

					if( GetStateID() == KGSFSM::S_ROOM )
					{
						StateTransition( KGSFSM::I_TO_FIELD_MAP );
					}

					// ���� ���� ����鼭 ���� ������ ��������!
					DungeonGameStart_SingleOpenDungeonRoom( kPacket_.m_kCreatePartyInfo.m_cDungeonMode, kPacket_.m_kCreatePartyInfo.m_iDungeonID, kPacket_.m_kCreatePartyInfo.m_DifficultyLevel, true );

					// �̱� ���� ���� ó���ǰ� ���� �ٷ� AutoParty�� ������ ����!
					KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
					kPacketAck.m_iOK = NetError::NET_OK;
					kPacketAck.m_iAutoPartyUID = kPacket_.m_iAutoPartyUID;
					SendToAutoParty( EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
				}
				break;

			default:
				{
					START_LOG( cerr, L"�߸��� ���°��Դϴ�!" )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;

					KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
					kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_00;
					kPacketAck.m_iAutoPartyUID = kPacket_.m_iAutoPartyUID;
					SendToAutoParty( EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
					return;
				}
				break;
			}
		}
	}
	else
	{
		// �� ��Ŷ�� ���� ������ ���ο� ��Ƽ�� ��Ƽ���� �ɰ��Դϴ�.
		SendToCnParty( EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ, kPacket_ );
	}
}

_IMPL_ON_FUNC( EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserDungeonManager.GetAutoPartyUID() == 0  ||  m_kUserDungeonManager.GetAutoPartyUID() != kPacket_.m_iAutoPartyUID )
	{
		START_LOG( cerr, L"���� �ڵ� ��Ƽ�� ��ϵ� ���°� �ƴѵ� ��Ƽ ���ġ�� ���� ������ �Ϸ��� �Ѵ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyUID() )
			<< BUILD_LOG( kPacket_.m_iAutoPartyUID )
			<< END_LOG;
	}

	_SendToAutoParty( kPacket_.m_iAutoPartyUID, EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_REGROUP_PARTY_NOT, KEGS_REGROUP_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() != 0  &&  GetPartyUID() != kPacket_.m_kPartyInfo.m_iPartyUID )
	{
		START_LOG( cerr, L"�ٸ� ��Ƽ �Ҽ��ε� �� ��Ŷ�� �� ������ �޴ٴ�.. ���� ������ �ִ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( kPacket_.m_kPartyInfo.m_iPartyUID )
			<< END_LOG;
	}

	if( GetCandidatePartyUID() != 0  &&  GetCandidatePartyUID() != kPacket_.m_kPartyInfo.m_iPartyUID )
	{
		START_LOG( cerr, L"�ٸ� ��Ƽ�� ���� ��û �����ε� �� ��Ŷ�� �� ������ �޴ٴ�.. ���� ������ �ִ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( GetCandidatePartyUID() )
			<< BUILD_LOG( kPacket_.m_kPartyInfo.m_iPartyUID )
			<< END_LOG;
	}

	// ��Ƽ ���� ������Ʈ
	SetPartyUID( kPacket_.m_kPartyInfo.m_iPartyUID );

	//{{ 2012. 04. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// ��Ƽ�� �������� ��������!
	m_kUserDungeonManager.SetDungeonGameInfo( kPacket_.m_kPartyInfo.m_iDungeonID, kPacket_.m_kPartyInfo.m_DifficultyLevel, kPacket_.m_kPartyInfo.m_cDungeonMode );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2012. 02. 22	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// ���� ��Ʋ�ʵ忡�� ��Ƽ ������ �ߴٸ� ��Ʋ�ʵ忡�� ��Ƽ ���� ������ ������!
	if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		// ���� ������ �ʱ�ȭ!
		KERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT kPacketToRM;
		kPacketToRM.m_iPartyUID = 0;
		SendToCnRoom( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT, kPacketToRM );

		// ���ο� ��Ƽ�� ����
		kPacketToRM.m_iPartyUID = GetPartyUID();
		BOOST_TEST_FOREACH( const KPartyUserInfo&, kInfo, kPacket_.m_vecPartyUserInfo )
		{
			kPacketToRM.m_vecPartyUnitUIDList.push_back( kInfo.m_iUnitUID );
		}		
		SendToCnRoom( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT, kPacketToRM );
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	SendPacket( EGS_REGROUP_PARTY_NOT, kPacket_ );
}

IMPL_ON_FUNC( EPM_LEAVE_PARTY_BY_AUTO_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2010. 03. 29  ������	��Ƽ ��Ż ����
	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ż ��û�� ���Ŀ� ���� PartyUID�� 0�̴�! ����� ������ ���� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< END_LOG;
	}
	else
	{
		if( GetPartyUID() != kPacket_.m_iPartyUID )
		{
			START_LOG( cerr, L"��Ż��û�� PartyUID�� ���� PartyUID�� �ٸ���! ����� ������ ���� ����!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetPartyUID() )
				<< BUILD_LOG( kPacket_.m_iPartyUID )
				<< END_LOG;
		}			
	}
	//}}

	// PartyUID�� 0���� ����
	SetPartyUID( 0 );

	//�������� ��Ƽ���� ������Ʈ.
	std::set< UidType > setEmpty;
	SendUpdatePartyMemberList( GetPartyUID(), setEmpty );

	//����ڿ��� ������Ŷ�� �����ش�.
	SendID( EGS_PARTY_BAN_USER_NOT );

	//{{ 2011. 07. 08	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

_IMPL_ON_FUNC( EPM_PREPARE_FOR_REGROUP_PARTY_AND_CHECK_NEW_HOST_NOT, KEPM_REQUEST_REGROUP_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserDungeonManager.GetAutoPartyUID() != 0 )
	{
		START_LOG( cerr, L"���� �ڵ� ��Ƽ�� ��ϵ� �����ε� ��Ƽ ������� �õ��Ϸ��� �Ѵ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyUID() )
			<< END_LOG;
		return;
	}

	if( GetPartyUID() > 0  ||  GetCandidatePartyUID() > 0 )
	{
		START_LOG( cerr, L"���� �������� ��Ƽ �Ҽ��� �Ǹ� �ȵȴ�! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( GetCandidatePartyUID() )
			<< END_LOG;
		return;
	}

	SendToCnParty( EPM_REQUEST_REGROUP_PARTY_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_REQUEST_GET_AUTO_PARTY_BONUS_INFO_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// �ڵ� ��Ƽ ���ʽ� ������ ��û�� ���� ����Ʈ ����!
	KEGS_UPDATE_AUTO_PARTY_BONUS_INFO_NOT kPacket;

	if( kPacket_.m_iVillageMapID != 0 )
	{
		m_kUserDungeonManager.GetAutoPartyBonusDungeonList( kPacket_.m_iVillageMapID, kPacket.m_mapDungeonBonusList );
	}

	if( kPacket_.m_vecDungeonList.empty() == false )
	{
		m_kUserDungeonManager.GetAutoPartyBonusDungeonList( kPacket_.m_vecDungeonList, kPacket.m_mapDungeonBonusList );
	}
	
	if( kPacket.m_mapDungeonBonusList.empty() == false )
	{
		SendPacket( EGS_UPDATE_AUTO_PARTY_BONUS_INFO_NOT, kPacket );
	}
}

//{{ 2012. 11. 13	������		�ڵ���Ƽ ��⿭ ī��Ʈ ǥ��
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
_IMPL_ON_FUNC( EGB_AUTO_PARTY_WAIT_USER_COUNT_NOT, KEGS_AUTO_PARTY_WAIT_USER_COUNT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_AUTO_PARTY_WAIT_USER_COUNT_NOT, kPacket_ );
}
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
//}}

