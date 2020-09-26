#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"
#include "Socket/NetCommon.h"

#include "BaseServer.h"
#include "UserManager.h"
#include "GameSysVal/GameSysVal.h"
#include "X2Data/XSLPartyManager.h"

//{{ 2011. 12. 05	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewPartyListManager.h"
#else
	#include "PartyListManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	#include "GSWorldMissionManager.h"
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

#include "Enum/Enum.h"
#include "odbc/Odbc.h"

//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	#include "X2Data/XSLBuffManager.h"
#endif SERV_SERVER_BUFF_SYSTEM
//}


#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC( EGS_INVITE_PARTY_REQ )
{
#ifdef SERV_REMOVE_REPEAT_FILTER_IN_EGS_INVITE_PARTY_REQ
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INVITE_PARTY_REQ, EGS_INVITE_PARTY_ACK );
#else SERV_REMOVE_REPEAT_FILTER_IN_EGS_INVITE_PARTY_REQ
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INVITE_PARTY_REQ, EGS_INVITE_PARTY_ACK );
#endif SERV_REMOVE_REPEAT_FILTER_IN_EGS_INVITE_PARTY_REQ

	KEGS_INVITE_PARTY_ACK kAck;
	kAck.m_iReceiverUID = kPacket_.m_iReceiverUID;
	kAck.m_iOK = NetError::NET_OK;

	// ������ ����
	if( kPacket_.m_iReceiverUID <= 0  &&  kPacket_.m_wstrNickName.empty() )
	{
		START_LOG( cerr, L"�ʴ��ҷ��� ĳ���� ������ �̻��մϴ�!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_INVITE_PARTY_ACK, kAck );
		return;
	}

	// valid�� ���ڿ����� �˻�
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrNickName ) )
	{
		kAck.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_INVITE_PARTY_ACK, kAck );
		return;
	}

	// ��Ƽ �ʴ��ڰ� �ʴ��� �� �ִ� ������ �Ǵ��� �˻�
	if( SiKPartyListManager()->CheckInvitePartyConditionForHost( GetThisPtr<KGSUser>() ) == false )
	{
		kAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_INVITE_PARTY_ACK, kAck );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� UnitUID�� ���� ���� ���� �ʵ忡�� �ش� ������ ���� ������� �����Ѱ��̴�.
	if( kPacket_.m_iReceiverUID > 0 )
	{
		// ���� ������ �ش� ������ �ִ��� Ȯ������
		KGSUserPtr spReceiver = GetKUserManager()->GetByCID( kPacket_.m_iReceiverUID );
		if( IS_NOT_NULL( spReceiver ) )
		{
			// ��Ƽ �ʴ��ڰ� ��Ƽ�Ҽ��� �ƴ϶��?
			if( GetPartyUID() == 0 )
			{
				// �ʴ븦 �ϸ鼭 ��Ƽ�� �����ϴ� ������ �Ѵ�!
				if( SiKPartyListManager()->E_CreatePartyInSameField( GetThisPtr<KGSUser>(), spReceiver ) == false )
				{
					START_LOG( cwarn, L"���� �ʵ忡 �ִ� ������ �Բ� ��Ƽ ���� ����!" )
						<< BUILD_LOG( GetPartyUID() )
						<< BUILD_LOG( GetCharName() )
						<< BUILD_LOG( spReceiver->GetCharName() )
						<< END_LOG;

					kAck.m_iOK = NetError::GetLastError();
					SendPacket( EGS_INVITE_PARTY_ACK, kAck );
					return;
				}
				else
				{
					// ���� �ش� �������� �ʴ����� ������!
					KEPM_CREATE_PARTY_BY_INVITE_REQ kPacketNot;
					kPacketNot.m_cAuthLevel = GetAuthLevel();
					kPacketNot.m_kCreatePartyInfo = kPacket_.m_kCreatePartyInfo;
					GetPartyUserInfo( kPacketNot.m_kHostPartyUserInfo );
					spReceiver->GetPartyUserInfo( kPacketNot.m_kReceiverPartyUserInfo );
					SendToCnParty( EPM_CREATE_PARTY_BY_INVITE_REQ, kPacketNot );
					return;
				}
			}
			// ��Ƽ �ʴ��ڰ� �̹� ��Ƽ�Ҽ��̶��?
			else
			{
				// �׳� ��Ƽ �ʴ� ������ �Ѵ�!
				if( SiKPartyListManager()->E_InvitePartyInSameField( GetThisPtr<KGSUser>(), spReceiver ) == false )
				{
					START_LOG( cwarn, L"���� �ʵ忡 �ִ� ������ ��Ƽ �ʴ� ����!" )
						<< BUILD_LOG( GetPartyUID() )
						<< BUILD_LOG( GetCharName() )
						<< BUILD_LOG( spReceiver->GetCharName() )
						<< END_LOG;

					kAck.m_iOK = NetError::GetLastError();
					kAck.m_iReceiverUID = spReceiver->GetCharUID();
					SendPacket( EGS_INVITE_PARTY_ACK, kAck );
					return;
				}
				else
				{
					// ����ڰ� �ʴ� ���ǿ� �����Ͽ� �ʴ�ó���մϴ�.
					KEPM_INVITE_PARTY_REQ kPacket;
					kPacket.m_iPartyUID = GetPartyUID();
					spReceiver->GetPartyUserInfo( kPacket.m_kPartyUserInfo );
					SendToCnParty( EPM_INVITE_PARTY_REQ, kPacket );	
					return;
				}			
			}

			return;
		}
	}
	

	//////////////////////////////////////////////////////////////////////////
	// ���� �ʵ� ���� ������ �������� �ʴ� ��� �α��� ������ �۷ι� ������ ������ ������ ��Ƽ �ʴ븦 �Ѵ�.
	{
		bool bInviteSameServerGroup = false;

		//////////////////////////////////////////////////////////////////////////
		// ���� �������� �ش�Ǵ� �г����� ������쿡�� �г��ӿ� �ش�Ǵ� UnitUID�� ã���� �α��μ����� ����!
#ifdef EXTEND_SERVER_GROUP_MASK
		if( kPacket_.m_iServerGroupID == -1 || kPacket_.m_iServerGroupID == KBaseServer::GetKObj()->GetServerGroupID() )
#else EXTEND_SERVER_GROUP_MASK
		if( kPacket_.m_iServerGroupID == SEnum::SGI_INVALID  ||  kPacket_.m_iServerGroupID == KBaseServer::GetKObj()->GetServerGroupID() )
#endif EXTEND_SERVER_GROUP_MASK
		{
			bInviteSameServerGroup = true;
		}

		// ��Ƽ �ʴ��ڰ� ��Ƽ�Ҽ��� �ƴ϶��?
		if( GetPartyUID() == 0 )
		{
			// �α��� ������ ������ üũ�ϰ� �ʴ�� �Բ� ��Ƽ ���� ������ �Ѵ�.
			if( SiKPartyListManager()->E_CreatePartyFarFieldCheckHost( GetThisPtr<KGSUser>(), kPacket_.m_wstrNickName ) == false )
			{
				START_LOG( cwarn, L"��Ƽ �ʴ��ڰ� ��Ƽ�ʴ븦 �� �� �ִ� ���ǿ� �������� �ʽ��ϴ�." )
					<< BUILD_LOG( GetPartyUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( kPacket_.m_wstrNickName )
					<< END_LOG;

				kAck.m_iOK = NetError::GetLastError();
				SendPacket( EGS_INVITE_PARTY_ACK, kAck );
				return;
			}
		}
		// �̹� ��Ƽ �Ҽ��̶��?
		else
		{
			// ��Ƽ �ʴ��ڿ� ���� �˻縦 ����.
			if( SiKPartyListManager()->E_InvitePartyFarFieldCheckHost( GetThisPtr<KGSUser>(), kPacket_.m_wstrNickName ) == false )
			{
				START_LOG( cwarn, L"��Ƽ �ʴ��ڰ� ��Ƽ�ʴ븦 �� �� �ִ� ���ǿ� �������� �ʽ��ϴ�." )
					<< BUILD_LOG( GetPartyUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( kPacket_.m_wstrNickName )
					<< END_LOG;

				kAck.m_iOK = NetError::GetLastError();
				SendPacket( EGS_INVITE_PARTY_ACK, kAck );
				return;
			}
		}

		KELG_INVITE_PARTY_FIND_RECEIVER_REQ kPacket;
		kPacket.m_iHostUnitUID			= GetCharUID();
		kPacket.m_iReceiverUnitUID		= kPacket_.m_iReceiverUID;		// kPacket.m_iReceiverUnitUID = kPacket.m_iReceiverUnitUID �Ǿ� �ִ� ���� ����	- �ڼ���	-- 2012�� 11�� 22�� ��ġ ����
#ifdef	SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE // ���볯¥: 2013-04-04
		kPacket.m_iServerGroupID		= kPacket_.m_iServerGroupID;
#endif	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
		kPacket.m_wstrReceiverNickName	= kPacket_.m_wstrNickName;
		kPacket.m_kCreatePartyInfo		= kPacket_.m_kCreatePartyInfo;
		if( bInviteSameServerGroup )
		{
			SendToLoginServer( ELG_INVITE_PARTY_FIND_RECEIVER_REQ, kPacket );
		}
		else
		{
			SendToGlobalServer( EGB_INVITE_PARTY_FIND_RECEIVER_REQ, kPacket );
		}
	}
}

_IMPL_ON_FUNC( EPM_INVITE_PARTY_ACK, KEGS_INVITE_PARTY_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INVITE_PARTY_ACK );

	SendPacket( EGS_INVITE_PARTY_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_INVITE_PARTY_NOT, KEGS_INVITE_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// �̰� �޴� �������� �ӽ� ��Ƽ ������ε� CandidatePartyUID���� �־�� ���� ������?
	SetCandidatePartyUID( kPacket_.m_iPartyUID );	

	SendPacket( EGS_INVITE_PARTY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ELG_INVITE_PARTY_GET_RECEIVER_USER_INFO_REQ, KELG_INVITE_PARTY_FIND_RECEIVER_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KELG_INVITE_PARTY_FIND_RECEIVER_ACK kAck;
	kAck.m_iHostUnitUID = kPacket_.m_iHostUnitUID;
	kAck.m_kCreatePartyInfo = kPacket_.m_kCreatePartyInfo;

	if( SiKPartyListManager()->E_InvitePartyFarFieldCheckReceiver( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( clog, L"��Ƽ �ʴ� ����ڰ� �ʴ�ɼ� ���� �����Դϴ�." )
			<< BUILD_LOG( kPacket_.m_wstrHostNickName )
			<< END_LOG;

		kAck.m_iOK = NetError::GetLastError();
		SendToLoginServer( ELG_INVITE_PARTY_GET_RECEIVER_USER_INFO_ACK, kAck );
		return;
	}
	else
	{
		kAck.m_iOK = NetError::NET_OK;
		KPartyUserInfo kPartyUserInfo;
		GetPartyUserInfo( kPartyUserInfo );
		kAck.m_vecReceiverPartyUserInfo.push_back( kPartyUserInfo );
		SendToLoginServer( ELG_INVITE_PARTY_GET_RECEIVER_USER_INFO_ACK, kAck );
		return;
	}
}

_IMPL_ON_FUNC( EGB_INVITE_PARTY_GET_RECEIVER_USER_INFO_REQ, KELG_INVITE_PARTY_FIND_RECEIVER_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KELG_INVITE_PARTY_FIND_RECEIVER_ACK kAck;
	kAck.m_iHostUnitUID = kPacket_.m_iHostUnitUID;
	kAck.m_kCreatePartyInfo = kPacket_.m_kCreatePartyInfo;

	if( SiKPartyListManager()->E_InvitePartyFarFieldCheckReceiver( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( clog, L"��Ƽ �ʴ� ����ڰ� �ʴ�ɼ� ���� �����Դϴ�." )
			<< BUILD_LOG( kPacket_.m_wstrHostNickName )
			<< END_LOG;

		kAck.m_iOK = NetError::GetLastError();
		SendToGlobalServer( EGB_INVITE_PARTY_GET_RECEIVER_USER_INFO_ACK, kAck );
		return;
	}
	else
	{
		kAck.m_iOK = NetError::NET_OK;
		KPartyUserInfo kPartyUserInfo;
		GetPartyUserInfo( kPartyUserInfo );
		kAck.m_vecReceiverPartyUserInfo.push_back( kPartyUserInfo );
		SendToGlobalServer( EGB_INVITE_PARTY_GET_RECEIVER_USER_INFO_ACK, kAck );
		return;
	}
}

IMPL_ON_FUNC( ELG_INVITE_PARTY_FIND_RECEIVER_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INVITE_PARTY_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// ��Ƽ ��û ��� ����
		m_kUserPartyManager.ClearWaitPartyUser();

		KEGS_INVITE_PARTY_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_INVITE_PARTY_ACK, kPacketAck );
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// �ʴ��ڰ� ��Ƽ�Ҽ��̶��
	if( GetPartyUID() != 0 )
	{
		KEPM_INVITE_PARTY_REQ kPacket;
		kPacket.m_iPartyUID = GetPartyUID();
		BOOST_TEST_FOREACH( const KPartyUserInfo&, kInfo, kPacket_.m_vecReceiverPartyUserInfo )
		{
			kPacket.m_kPartyUserInfo = kInfo;
		}
		SendToCnParty( EPM_INVITE_PARTY_REQ, kPacket );
	}
	//////////////////////////////////////////////////////////////////////////
	// �ʴ��ڰ� ��Ƽ�Ҽ��� �ƴ϶��
	else
	{
		// ��Ƽ ������ �ʴ븦 ���ÿ� ����!
		KEPM_CREATE_PARTY_BY_INVITE_REQ kPacketNot;
		kPacketNot.m_cAuthLevel = GetAuthLevel();
		kPacketNot.m_kCreatePartyInfo = kPacket_.m_kCreatePartyInfo;
		GetPartyUserInfo( kPacketNot.m_kHostPartyUserInfo );
		BOOST_TEST_FOREACH( const KPartyUserInfo&, kInfo, kPacket_.m_vecReceiverPartyUserInfo )
		{
			kPacketNot.m_kReceiverPartyUserInfo = kInfo;
		}
		SendToCnParty( EPM_CREATE_PARTY_BY_INVITE_REQ, kPacketNot );
	}
}

_IMPL_ON_FUNC( EGB_INVITE_PARTY_FIND_RECEIVER_ACK, KELG_INVITE_PARTY_FIND_RECEIVER_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INVITE_PARTY_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// ��Ƽ ��û ��� ����
		m_kUserPartyManager.ClearWaitPartyUser();

		KEGS_INVITE_PARTY_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_INVITE_PARTY_ACK, kPacketAck );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// �ʴ��ڰ� ��Ƽ�Ҽ��̶��
	if( GetPartyUID() != 0 )
	{
		KEPM_INVITE_PARTY_REQ kPacket;
		kPacket.m_iPartyUID = GetPartyUID();
		BOOST_TEST_FOREACH( const KPartyUserInfo&, kInfo, kPacket_.m_vecReceiverPartyUserInfo )
		{
			kPacket.m_kPartyUserInfo = kInfo;
		}
		SendToCnParty( EPM_INVITE_PARTY_REQ, kPacket );
	}
	//////////////////////////////////////////////////////////////////////////
	// �ʴ��ڰ� ��Ƽ�Ҽ��� �ƴ϶��
	else
	{
		// ��Ƽ ������ �ʴ븦 ���ÿ� ����!
		KEPM_CREATE_PARTY_BY_INVITE_REQ kPacketNot;
		kPacketNot.m_cAuthLevel = GetAuthLevel();
		kPacketNot.m_kCreatePartyInfo = kPacket_.m_kCreatePartyInfo;
		GetPartyUserInfo( kPacketNot.m_kHostPartyUserInfo );
		BOOST_TEST_FOREACH( const KPartyUserInfo&, kInfo, kPacket_.m_vecReceiverPartyUserInfo )
		{
			kPacketNot.m_kReceiverPartyUserInfo = kInfo;
		}
		SendToCnParty( EPM_CREATE_PARTY_BY_INVITE_REQ, kPacketNot );
	}
}

IMPL_ON_FUNC( EGS_INVITE_PARTY_REPLY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( SiKPartyListManager()->E_InvitePartyReply( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"��Ƽ �ʴ뿡 ���� ���� ����." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( EPM_INVITE_PARTY_DENY_NOT, KEGS_INVITE_PARTY_DENY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_INVITE_PARTY_DENY_NOT, kPacket_ );
}

//{{ 2012. 10. 25	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
IMPL_ON_FUNC_NOPARAM( EPM_LEAVE_CANDIDATE_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ��Ƽ �ʴ� ����ڰ� ��Ƽ�ʴ븦 �ź��߱� ������ ��Ƽ�� �ı��˴ϴ�. ���� �ӽ� ����� PartyUID���� 0���� �����ؾ���.
	SetCandidatePartyUID( 0 );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

IMPL_ON_FUNC( EGS_LEAVE_PARTY_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		// [����] ��Ƽ��Ż�ϱ����� ������ߴٸ� �ð����� �߻�����

		START_LOG( cwarn, L"��Ƽ�� Ż���ϱ����ε� UID�� ����.?" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_LEAVE_PARTY_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_PARTY_01;
		SendPacket( EGS_LEAVE_PARTY_ACK, kPacket );
		return;
	}
	
	if( SiKPartyListManager()->E_LeavePartyReq( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"��Ƽ ������ ����.!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( NetError::GetErrStr(kPacket_.m_iReason) )
			<< END_LOG;
	}

	//{{ 2011. 07. 08	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

IMPL_ON_FUNC( EPM_LEAVE_PARTY_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ���������� ��Ƽ ��Ż �Ǿ��ٸ�?
	if( kPacket_.m_iOK == NetError::NET_OK  ||
		kPacket_.m_iOK == NetError::ERR_PARTY_01 )
	{
		//{{ 2010. 03. 29  ������	��Ƽ ��Ż ����
		if( GetPartyUID() == 0 )
		{
			START_LOG( clog, L"��Ż ��û�� ���Ŀ� ���� PartyUID�� 0�̱�.." )
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

				//{{ 2012. 02. 21	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
				// ���⼭ ������.
				return;
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
			}
		}
		//}}

		// ��Ƽ �����UID�� ����
		SetCandidatePartyUID( 0 );

		//��Ƽ UID�� �����ϰ�.
		SetPartyUID( 0 );

		//�������� ��Ƽ���� ������Ʈ.
		std::set< UidType > setEmpty;
		SendUpdatePartyMemberList( 0, setEmpty );

		//{{ 2010. 03. 12  ������	ä�� ���� ��Ƽ ���� ����
		if( GetStateID() == KGSFSM::S_ROOM  &&  GetRoomUID() > 0 )
		{
			// ���� �濡 �ִ� ���¶�� ��Ż ó��!
			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_DUNGEON:
				{
					// �������̶�� ��Żó�� �մϴ�.
					KERM_LEAVE_ROOM_REQ kPacket;
					kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_00;
					SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacket );

					// RoomUID�ʱ�ȭ
					SetRoomUID( 0 );

					// field���·� �̵�
					StateTransition( KGSFSM::I_TO_FIELD_MAP );
				}
				break;
			case CXSLRoom::RT_SQUARE:
			case CXSLRoom::RT_TRAININGCENTER:
				//{{ 2011. 12. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
			case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
				// �Ʒü� �� ���忡 �����ϸ鼭 �ڵ����� ��Ƽ ��Ż ó���ϴ°Ŷ�� �ڵ� �� ��Ż ó������ �ʴ´�.
				break;

			default:
				{
					// �������� ��Ȳ�� �ƴϹǷ� �����α׸� �����.
					START_LOG( cerr, L"��Ƽ �ΰ� �־��µ� ������ �ƴ� �濡 �����ֳ�?" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
						<< END_LOG;

					KERM_LEAVE_ROOM_REQ kPacket;
					kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_00;
					SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacket );

					// RoomUID�ʱ�ȭ
					SetRoomUID( 0 );

					// field���·� �̵�
					StateTransition( KGSFSM::I_TO_FIELD_MAP );
				}
				break;
			}
		}
		//}}
	}


	//{{ 2011. 11. 28	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// ���� ��Ƽ ������ ���� ��Ż�̾��ٸ�!
	if( kPacket_.m_iReason == NetError::NOT_LEAVE_PARTY_REASON_06 )
	{
		// Ŭ���̾�Ʈ�� ACK��Ŷ�� ������ �ʴ´�!
		return;
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	// Ŭ���̾�Ʈ�� ACK��Ŷ ����
	KEGS_LEAVE_PARTY_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_LEAVE_PARTY_ACK, kPacket );
}

_IMPL_ON_FUNC( EPM_LEAVE_PARTY_NOT, KEGS_LEAVE_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2012. 02. 21	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// �ڱ� �ڽ��� ��Ż�� ���� ���̶�� �ٸ� ó���� �Ѵ�.
	if( GetCharUID() == kPacket_.m_iLeaveUnitUID )
	{
		// ���� ��Ʋ�ʵ忡�� ��Ƽ ������ �ߴٸ� ��Ʋ�ʵ忡�� ��Ƽ ���� ������ ������!
		if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
		{
			KERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT kPacketToRM;
			kPacketToRM.m_iPartyUID = 0; // ��Żó���ϱ� 0���� ����
			SendToCnRoom( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT, kPacketToRM );
		}

		//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
		if( m_kComeBackUserInfo.GetIsComeBackUser() == false )
		{
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
			kReq.m_iUnitUID = GetCharUID();

			DeactivateBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO, kReq.m_vecDeActivateBuff );

			if( kReq.m_vecDeActivateBuff.empty() == false )
			{
				SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
			}
		}
#endif SERV_SERVER_BUFF_SYSTEM
		//}

		return;
	}

	if( GetPartyUID() == 0  ||  GetPartyUID() != kPacket_.m_iPartyUID )
	{
		START_LOG( cwarn, L"�ٸ� ��Ƽ�� ���� ��Ż ó���� �Դ�! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( kPacket_.m_iPartyUID );
		return;
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2012. 02. 22	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// ���� ��Ʋ�ʵ忡�� ��Ƽ ������ �ߴٸ� ��Ʋ�ʵ忡�� ��Ƽ ���� ������ ������!
	if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		KERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT kPacketToRM;
		kPacketToRM.m_iPartyUID = GetPartyUID();
		kPacketToRM.m_vecPartyUnitUIDList = kPacket_.m_vecPartyUnitUIDList;
		SendToCnRoom( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT, kPacketToRM );
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	if( m_kComeBackUserInfo.GetIsComeBackUser() == false && kPacket_.m_bExistComeBackUser == false )
	{
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
		kReq.m_iUnitUID = GetCharUID();

		DeactivateBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO, kReq.m_vecDeActivateBuff );

		if( kReq.m_vecDeActivateBuff.empty() == false )
		{
			SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
		}
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}

	SendPacket( EGS_LEAVE_PARTY_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_READY_REQ )
{
	VERIFY_STATE( (2, KGSFSM::S_ROOM, KGSFSM::S_FIELD_MAP) );

	KEGS_PARTY_CHANGE_READY_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	////////////////////////////////////////////////////////////////////////// ���� ����
	// partyuid
	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��ƼUID �̻�.!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_03;
		goto end_proc;
	}

	int iDungeonID = 0;
	char cDifficultyLevel = 0;
	char cDungeonMode = 0;

	//��Ƽ�� ���������� ã�Ƽ�..	
	if( SiKPartyListManager()->GetDungeonInfo( GetPartyUID(), iDungeonID, cDifficultyLevel, cDungeonMode ) == false )
	{
		START_LOG( cerr, L"��Ƽ�� �������� ã�� ����.!" )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
		//��Ƽ UID �̻�� ���� ���� ����.
		kAck.m_iOK = NetError::ERR_PARTY_03;
		goto end_proc;
	}

	//{{ 2009. 8. 5  ������		ü��ID����
	if( IsGuestUser() )
	{
		if( GetLevel() > SiKGameSysVal()->GetGuestUserLimitLevel() )
		{
			START_LOG( cerr, L"Ŭ���̾�Ʈ���� ü��ID�� ����20�Ǿ����� ������ ĳ���������� �̵��������ٵ� ���� ������ �ϰ��ֳ�?" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUEST_USER_02;
			goto end_proc;
		}

		if( CXSLDungeon::IsHenirDungeon( iDungeonID ) )
		{
			kAck.m_iOK = NetError::ERR_GUEST_USER_00;
			goto end_proc;
		}
	}
	//}}

	//{{ 2010. 12. 28	������	�ϸ� ���� �߰�
//#ifdef SERV_SHIP_DUNGEON
//	if( CXSLDungeon::IsShipDungeon( iDungeonID ) == false )
//#endif SERV_SHIP_DUNGEON
//	//}}
//	{
//		//��������Ʈ�� �ƴϸ� �����Ҽ� ����.
//		if( CXSLMapData::IsDungeonGate( GetMapID() ) == false )
//		{
//			START_LOG( cwarn, L"��������Ʈ�� �ƴϸ� ��Ƽ���� �Ҽ�����." )
//				<< BUILD_LOG( GetCharName() )
//				<< BUILD_LOG( iDungeonID )
//				<< BUILD_LOG( GetMapID() )
//				<< END_LOG;
//
//			kAck.m_iOK = NetError::ERR_PARTY_20;
//			goto end_proc;
//		}
//
//		if( GetMapID() != CXSLMapData::GetMapID( iDungeonID ) )
//		{
//			START_LOG( cwarn, L"�����Ҽ��ִ� ��������Ʈ�� �ƴ�." )
//				<< BUILD_LOG( GetCharName() )
//				<< BUILD_LOG( iDungeonID )
//				<< BUILD_LOG( GetMapID() )
//				<< END_LOG;
//
//			kAck.m_iOK = NetError::ERR_PARTY_29;
//			goto end_proc;
//		}
//	}

	//�غ���·� ����� �ش� �������� �Ҽ��ִ��� �˻�
	if( kPacket_.m_bReady == true )
	{
		//::1. �ټ��� �˻�.
		if( CheckingSpirit( iDungeonID ) == false )
		{
			kAck.m_iOK = NetError::ERR_ROOM_27;
			goto end_proc;
		}

		//::2. �����ʿ� �����۰˻�.
		int iRequireItemID = 0;
		int iRequireQuantity = 0;

		if( CheckRequiredItemToEnterDungeonRoom( iDungeonID + static_cast<int>(cDifficultyLevel), (CXSLDungeon::DUNGEON_MODE) cDungeonMode, iRequireItemID, iRequireQuantity ) == false )
		{
			//{{ 2010. 05. 06  ������	���� �˾�
			KEGS_ERROR_POPUP_NOT kNot;
			kNot.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
			kNot.m_iErrorData1 = iRequireItemID;
			kNot.m_iErrorData2 = iRequireQuantity;
			SendPacket( EGS_ERROR_POPUP_NOT, kNot );
			//}}

			kAck.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
			goto end_proc;
		}

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
		if( CheckLimitedPlayTimes( iDungeonID + static_cast<int>(cDifficultyLevel) ) == false )
		{
			kAck.m_iOK = NetError::ERR_DUNGEON_ENTER_LIMIT_00;
			goto end_proc;
		}
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

		//{{ 2010. 12. 30	������	�ϸ� ���� �߰�
#ifdef SERV_SHIP_DUNGEON
		if( CXSLDungeon::IsShipDungeon( iDungeonID ) == true )
		{
			if( CXSLDungeon::IsVelderDungeon( iDungeonID ) == true )
			{
				// �ش� map�� �� �� �ִ��� ���� üũ
				//{{ 2012. 09. 21   ��μ�   ���� ���� ���� �޽��� ����
#ifdef SERV_ENTER_VILLAGE_ERROR
				int iTempLevel = 0;
				int iTempDungeonID = 0;
				if( CheckEnterTheVillage( CXSLMapData::MI_HAMEL, iTempLevel, iTempDungeonID ) == false )
#else
				if( CheckEnterTheVillage( CXSLMapData::MI_HAMEL ) == false )
#endif SERV_ENTER_VILLAGE_ERROR
				//}}
				{
					START_LOG( cwarn, L"������ ������ �� �ִ� ������ �ȵ�." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( iDungeonID )
						<< END_LOG;

					kAck.m_iOK = NetError::GetLastError();
					goto end_proc;
				}
			}
		}
#endif SERV_SHIP_DUNGEON
		//}}
	}

	////////////////////////////////////////////////////////////////////////// ó��
	// ���� ����
	if( SiKPartyListManager()->E_ChangeReadyReq( GetThisPtr<KGSUser>(), kPacket_.m_bReady ) == false )
	{
		START_LOG( cwarn, L"��Ƽ ���� ���� ����." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( static_cast<int>(kPacket_.m_bReady) )
			<< END_LOG;
	}

	return;

end_proc:
	SendPacket( EGS_PARTY_CHANGE_READY_ACK, kAck );
}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_DUNGEON_REQ )
{	
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_PARTY_CHANGE_DUNGEON_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	////////////////////////////////////////////////////////////////////////// ������ ����	
	// PartyUID����
	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��ƼUID �̻�.!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_03;
		goto end_proc;
	}

	// DungeonID����
	if( !SiCXSLDungeonManager()->VerifyDungeonID( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel) ) )
	{
		START_LOG( cerr, L"���������ʴ� DungeonID�Դϴ�. Ŭ�󿡼� �˻������ٵ�.." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_22;
		goto end_proc;
	}

	//{{ 2009. 7. 30  ������	HenirDungeonMode����
	if( CXSLDungeon::IsValidDungeonMode( static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_cDungeonMode) ) == false )
	{
		START_LOG( cerr, L"���������� HenirDungeonMode���Դϴ�." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOG( (int)kPacket_.m_cDungeonMode )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_22;
		goto end_proc;
	}
	//}}

	////////////////////////////////////////////////////////////////////////// ���� ����
	//{{ 2009. 8. 5  ������		ü��ID����
	if( IsGuestUser()  &&  CXSLDungeon::IsHenirDungeon( kPacket_.m_iDungeonID ) )
	{
		kAck.m_iOK = NetError::ERR_GUEST_USER_00;
		goto end_proc;
	}
	//}}

	//::1. �ټ��� �˻�.
	if( CheckingSpirit( kPacket_.m_iDungeonID ) == false )
	{
		kAck.m_iOK = NetError::ERR_ROOM_27;
		goto end_proc;
	}

	//::2. �����ʿ� �����۰˻�.		// ���� ���� �κп��� üũ �ϵ��� ����
// 	int iRequireItemID = 0;
// 	int iRequireQuantity = 0;
// 
// 	if( CheckRequiredItemToEnterDungeonRoom( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel), (CXSLDungeon::DUNGEON_MODE) kPacket_.m_cDungeonMode, iRequireItemID, iRequireQuantity ) == false )
// 	{
// 		//{{ 2010. 05. 06  ������	���� �˾�
// 		KEGS_ERROR_POPUP_NOT kNot;
// 		kNot.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
// 		kNot.m_iErrorData1 = iRequireItemID;
// 		kNot.m_iErrorData2 = iRequireQuantity;
// 		SendPacket( EGS_ERROR_POPUP_NOT, kNot );
// 		//}}
// 
// 		kAck.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
// 		goto end_proc;
// 	}

	//::3. ������� Ŭ��� ���� �ʾ���.
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( kPacket_.m_iDungeonID );
		if( iRequireDungeonID != 0 )
		{
			std::map<int,KDungeonClearInfo>::iterator mit;
			mit = m_mapDungeonClear.find( iRequireDungeonID );
			if( mit == m_mapDungeonClear.end() )
			{
				kAck.m_iOK	= NetError::ERR_ROOM_06;

				goto end_proc;
			}
		}
	}

	//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	//::4. ���� ������ �������� �ʴ� ������ ����.
	{
		KGSPartyPtr spParty = SiKPartyListManager()->GetParty( GetPartyUID() );
		if( spParty == NULL )
		{
			START_LOG( cerr, L"��Ƽ UID �̻�.!" )
				<< BUILD_LOG( GetCharName() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PARTY_03;

			goto end_proc;
		}

// 		if( spParty->GetCheckLowLevel() == true )
// 		{
// 			int iDungeonMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel) );
// 
// 			if( spParty->CheckPartyMinLevel( iDungeonMinLevel ) == false )
// 			{
// 				kAck.m_iOK = NetError::ERR_CHANGE_DUNGEON_MAP_FAILE;
// 
// 				goto end_proc;
// 			}
// 		}
	}
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

	//{{ 2012. 03. 03	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	if( CXSLDungeon::IsDefenceDungeon( kPacket_.m_iDungeonID ) == true )
	{
		if( SiKGSWorldMissionManager()->GetIsActive() == true )
		{
			int iWorldCrystalNum = SiKGSWorldMissionManager()->GetWorldCrystalCount();

			if( KGSWorldMissionManager::GATE_OF_THE_DARKNESS_EXPERT <= iWorldCrystalNum )
			{
				kPacket_.m_DifficultyLevel = CXSLDungeon::DL_EXPERT;
			}
			else if( KGSWorldMissionManager::GATE_OF_THE_DARKNESS_HARD <= iWorldCrystalNum )
			{
				kPacket_.m_DifficultyLevel = CXSLDungeon::DL_HARD;
			}
			else
			{
				kPacket_.m_DifficultyLevel = CXSLDungeon::DL_NORMAL;
			}
		}
		else
		{
			START_LOG( cerr, L"��Ŷ ���� �ǽ� �����Դϴ�.")
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PARTY_22;
			goto end_proc;
		}
	}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}	

	////////////////////////////////////////////////////////////////////////// ó��
	// ���� ����
	if( SiKPartyListManager()->E_ChangeDungeon( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"�������� ����." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
			<< END_LOG;
	}	

	return;

end_proc:
	SendPacket( EGS_PARTY_CHANGE_DUNGEON_ACK, kAck );
}

IMPL_ON_FUNC( EGS_QUICK_START_DUNGEON_GAME_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_QUICK_START_DUNGEON_GAME_REQ, EGS_QUICK_START_DUNGEON_GAME_ACK );

	KEGS_QUICK_START_DUNGEON_GAME_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// �۾���¥: 2013-09-26	// �ڼ���
	// ���� �ڵ� ��Ī ������ Ȯ��!!
	if( m_kUserDungeonManager.GetAutoPartyWaitNumber() != 0  ||  m_kUserDungeonManager.GetAutoPartyUID() != 0 )
	{
		START_LOG( cerr, L"�ڵ� ��Ī ���ε� �ٷ� ���� ��û�� ���Դ�!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_MATCH_MAKING_23;	// ���� ���� ��û �߿��� ����� �� ���� ����Դϴ�.
		goto end_proc;
	}

	// ���� ���� ��Ī ������ Ȯ��!!
	if( GetMatchUID() != 0 )
	{
		START_LOG( cerr, L"���� ���� ��Ī ���ε� ���� ���� ��û�� ���Դ�!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_AUTO_PARTY_07;	// ���� ���� ��û �߿��� ����� �� ���� ����Դϴ�.
		goto end_proc;
	}

	// �� ����Ʈ ��ȸ ������ Ȯ��!!
	if( GetRoomListID() != 0 )
	{
		START_LOG( cerr, L"�� ����Ʈ ��ȸ �߿� ���� ���� ��û�� ���Դ�!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_AUTO_PARTY_08;	// // �� ����Ʈ ��ȸ �߿��� ����� �� ���� ����Դϴ�.
		goto end_proc;
	}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM

	//{{ 2011. 02. 21  ��μ�	Ư�� ���� ���� ������ ���� ����
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
	// ���� �ʱ�ȭ
	SetReserveKickUser( false );
	SetDungeonRequiredItemID( 0 );
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
	//}}

	// ������ �� Ÿ�� ����
	m_kInventory.ResetQuickSlotCoolTime();

	// 1. ��Ƽ�� ���� �ִ��� �˻�
	if( GetPartyUID() == 0 )
	{
		// ���� ������ ������ ������ �����ϴ��� üũ
		if( CheckForDungeonGameStart( kPacket_.m_cDungeonMode, kPacket_.m_iDungeonID, kPacket_.m_DifficultyLevel, false, true ) == false )
		{
			kAck.m_iOK = NetError::GetLastError();
			kAck.m_iFailUnitUID = GetCharUID();
			kAck.m_wstrFailUserNickName = GetCharName();
			goto end_proc;
		}

		//////////////////////////////////////////////////////////////////////////
		// �������� ���� ���� ����!
		if( GetRoomUID() == 0 )
		{
			DungeonGameStart_SingleOpenDungeonRoom( kPacket_.m_cDungeonMode, kPacket_.m_iDungeonID, kPacket_.m_DifficultyLevel, false );
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

					//{{ 2012. 10. 18	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
					// RoomUID���� 0���� �ٲ����� ���µ� ��� �ʵ� ���·� ����!
					if( GetStateID() == KGSFSM::I_TO_ROOM )
					{
						StateTransition( KGSFSM::I_TO_FIELD_MAP );
					}
#endif SERV_BATTLE_FIELD_SYSTEM
					//}}					

					// ���� ���� ����鼭 ���� ������ ��������!
					DungeonGameStart_SingleOpenDungeonRoom( kPacket_.m_cDungeonMode, kPacket_.m_iDungeonID, kPacket_.m_DifficultyLevel, false );
				}
				break;

			default:
				{
					START_LOG( cerr, L"�߸��� ���°��Դϴ�!" )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;

					kAck.m_iOK = NetError::ERR_BATTLEFIELD_08;
					goto end_proc;
				}
				break;
			}
		}
		//////////////////////////////////////////////////////////////////////////
	}
	else
	{
		// 8. ��Ƽ ���� ����
		if( SiKPartyListManager()->E_GameStart( GetThisPtr<KGSUser>(), kPacket_ ) == false )
		{
			START_LOG( cerr, L"�������� �����ϱ� ����.!" )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( kPacket_.m_iDungeonID )
				<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
				<< BUILD_LOGc( GetPartyUID() )
				<< END_LOG;
		}
	}

	return;

end_proc:
	SendPacket( EGS_QUICK_START_DUNGEON_GAME_ACK, kAck );
}

IMPL_ON_FUNC( EPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ �Ҽӵ� �ƴѵ� ��Ƽ ���� ���� ���� üũ ��Ŷ�� �Գ�?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
		return;
	}

	KEPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::NET_OK;

	// �� ��Ƽ���鿡�� �ǰ��� �������� ���� ���� ���������� üũ �Ѵ�.
	if( CheckForDungeonGameStart( kPacket_.m_kDungeonGameInfo.m_cDungeonMode, kPacket_.m_kDungeonGameInfo.m_iDungeonID, kPacket_.m_kDungeonGameInfo.m_cDifficulty, false, false ) == false )
	{
		kPacketAck.m_iOK = NetError::GetLastError();
		kPacketAck.m_wstrFailUserNickName = GetCharName(); // ���� �Ҹ����̸� �˾����� �г��� ������ϴϱ� �г��ӵ� ��ư���!
	}

	SendToCnParty( EPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_ACK, kPacketAck );
}

//{{ 2012. 12. 17	�ڼ���	���� ��Ƽ �� ���� �˾� â ���� ���� ����
#ifdef SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
_IMPL_ON_FUNC( EPM_PARTY_GAME_START_OPINION_CHECK_NOT, KEGS_PARTY_GAME_START_OPINION_CHECK_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_GAME_START_OPINION_CHECK_NOT, kPacket_ );
}
#else
IMPL_ON_FUNC_NOPARAM( EPM_PARTY_GAME_START_OPINION_CHECK_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendID( EGS_PARTY_GAME_START_OPINION_CHECK_NOT );
}
#endif SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
//}}

IMPL_ON_FUNC( EGS_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ �Ҽ��� �ƴѵ� ���� ���ۿ� ���� ������ �ϳ�? �ð��� �����ε�" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
		return;
	}

	SendToCnParty( EPM_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_SINGLE_OPEN_DUNGEON_ROOM_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_QUICK_START_DUNGEON_GAME_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"���� ȥ�� �÷��� �ϱ� ���� ����!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		// �α׸� ����
	}

	KEGS_QUICK_START_DUNGEON_GAME_ACK kPacketAck;
	kPacketAck.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_QUICK_START_DUNGEON_GAME_ACK, kPacketAck );
}

_IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_DUNGEON_GAME_START_ACK, KEGS_LEAVE_ROOM_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_QUICK_START_DUNGEON_GAME_ACK );

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

_IMPL_ON_FUNC( EPM_PARTY_GAME_START_FAILED_NOT, KEGS_PARTY_GAME_START_FAILED_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ �Ҽ��� �ƴѵ� ���� ���� ���� ��Ŷ�� ����?" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
		return;
	}

	SendPacket( EGS_PARTY_GAME_START_FAILED_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_PARTY_OPEN_DUNGEON_ROOM_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_QUICK_START_DUNGEON_GAME_ACK );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( GetPartyUID() == 0 )
		{
			START_LOG( cerr, L"��Ƽ�� ���� ������ �����޴µ� ��Ƽ UID�� ����.!" )
				<< BUILD_LOG( GetCharName() )
				<< END_LOG;
		}
	}

	KEGS_QUICK_START_DUNGEON_GAME_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_QUICK_START_DUNGEON_GAME_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_PARTY_OPEN_DUNGEON_ROOM_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//////////////////////////////////////////////////////////////////////////
	if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		KERM_LEAVE_ROOM_REQ kPacket;		
		kPacket.m_bSendAck = false;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_28;
		SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacket );

		// ��Żó�� ������ ���⼭ ��!
		SetRoomUID( 0 );

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			StateTransition( KGSFSM::I_TO_FIELD_MAP );
		}
	}
	//////////////////////////////////////////////////////////////////////////

	if( GetAuthLevel() < SEnum::UAL_GM )
		kPacket_.m_wstrCNIP.clear();

	SetRoomUID( kPacket_.m_RoomInfo.m_RoomUID );

	//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
	m_kUserDungeonManager.SetDungeonGameInfo( kPacket_.m_RoomInfo.m_iDungeonID, kPacket_.m_RoomInfo.m_DifficultyLevel, kPacket_.m_RoomInfo.m_cDungeonMode );
#endif SERV_REFORM_QUEST
	//}}

#ifdef SERV_ADD_TITLE_CONDITION
	UseResurrectionStone( false );
	SetStartedNumMember( kPacket_.m_RoomInfo.m_JoinSlot );
#endif SERV_ADD_TITLE_CONDITION

	//{{ 2013. 02. 05  Īȣ ȹ�� ���� �߰�(��Ȱ�� ��� Ƚ��, ���� ����) - ��μ�
#ifdef SERV_ADD_TITLE_CONDITION_SANDER
	ClearUseResurrectionStoneCount();
#endif SERV_ADD_TITLE_CONDITION_SANDER
	//}

	StateTransition( KGSFSM::I_TO_ROOM );
	m_kTimer[TM_ROOM].restart();

	// �ʵ忡�� ��Ż
	SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );

	//{{ 2009. 10. 8  ������	���
#ifdef GUILD_TEST
	SendUpdateGuildMemberInfo();
#endif GUILD_TEST
	//}}

	switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
	{
	case CXSLRoom::RT_DUNGEON:
		{
			int iRequireDungeonID = kPacket_.m_RoomInfo.m_iDungeonID + static_cast<int>(kPacket_.m_RoomInfo.m_DifficultyLevel);

			CXSLDungeon::DUNGEON_MODE eDungeonMode = (CXSLDungeon::DUNGEON_MODE) kPacket_.m_RoomInfo.m_cDungeonMode;

			//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
			// ��ϸ� �����̶�� ���� ���� Ƚ���� ������Ų��.
			if( CXSLDungeon::IsHenirDungeon( kPacket_.m_RoomInfo.m_iDungeonID ) == true )
			{
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
				if( m_kUserDungeonManager.IncreaseHenirRewardCount( kPacket_.m_RoomInfo.m_cDungeonMode ) == false )
#else // SERV_HENIR_RENEWAL_2013
				if( m_kUserDungeonManager.IncreaseHenirRewardCount() == false )
#endif // SERV_HENIR_RENEWAL_2013
				{
					START_LOG( clog, L"��ϸ� ���� ȹ�� Ƚ�� ���� ����!! ���̻� ���� �� �� ������ ���� ���� �� ����." )
						<< BUILD_LOG( GetCharName() )
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardNormalCount() )
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardPremiumCount() )
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardEventCount() )
						<< END_LOG;
				}
				//{{ 2012. 11. 9	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
				else
				{
					// ��ϸ� �ð� ���� ��������� ���ͼ������� �˸��� ����!
					SendToCnRoom( ERM_UPDATE_HENIR_REWARD_USER_NOT );
				}
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
			}
#endif SERV_NEW_HENIR_TEST
			//}}

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
			std::map< int, KDungeonPlayInfo >::iterator mitDungeonPlay = m_mapDungeonPlay.find( kPacket_.m_RoomInfo.m_iDungeonID );
			if( mitDungeonPlay == m_mapDungeonPlay.end() )
			{
				KDungeonPlayInfo kDungeonPlayInfo;
				kDungeonPlayInfo.m_iDungeonID = kPacket_.m_RoomInfo.m_iDungeonID;
				kDungeonPlayInfo.m_iPlayTimes = 1;
				kDungeonPlayInfo.m_iClearTimes = 0;
				kDungeonPlayInfo.m_bNew = true;

				m_mapDungeonPlay.insert( std::make_pair( kPacket_.m_RoomInfo.m_iDungeonID, kDungeonPlayInfo ) );
			}
			else
			{
				mitDungeonPlay->second.m_iPlayTimes += 1;
				mitDungeonPlay->second.m_bNew = true;
			}
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

			//{{ 2011. 02. 21  ��μ�	Ư�� ���� ���� ������ ���� ����
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
			int iItemID = 0;
			if( !DeleteRequiredItemToEnterDungeonRoom( iRequireDungeonID, eDungeonMode, kPacket_.m_vecInventorySlotInfo, iItemID ) )
#else
			if( !DeleteRequiredItemToEnterDungeonRoom( iRequireDungeonID, eDungeonMode, kPacket_.m_vecInventorySlotInfo ) )
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
			//}}
			{
				// �켱 ���ӽ��۽� �������� ������ ������ �α׸� ���ܺ���..
				START_LOG( cerr, L"�������� ���۽� �ʿ�������� ����.!" )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( iRequireDungeonID )
					<< BUILD_LOG( (int)eDungeonMode )
					<< END_LOG;

//{{ 2011. 02. 21  ��μ�	Ư�� ���� ���� ������ ���� ����
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
				// �������� ���� ����
				SetReserveKickUser( true );
				SetDungeonRequiredItemID( iItemID );

// 				KERM_RESERVE_LEAVE_USER_NOT kPacketReq;
// 				kPacketReq.m_iUnitUID = GetCharUID();
// 				kPacketReq.m_iItemID = iItemID;
// 				SendToCnRoom( ERM_RESERVE_LEAVE_USER_NOT, kPacketReq );
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
//}}
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"�Ͼ�� ���� ����!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< END_LOG;
		}
	}	

	KEGS_PARTY_GAME_START_NOT kNot;
	kNot.m_RoomInfo = kPacket_.m_RoomInfo;
	kNot.m_sWorldID = kPacket_.m_sWorldID;
	kNot.m_vecSlot = kPacket_.m_vecSlot;
	kNot.m_vecObserverSlot = kPacket_.m_vecObserverSlot;
	kNot.m_wstrCNIP = kPacket_.m_wstrCNIP;
	kNot.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;

	SendPacket( EGS_PARTY_GAME_START_NOT, kNot );

	//{{ 2009. 5. 30  ������	�ӽ� �κ�
	ClearTempInventory();
	//}}

	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	std::map< UidType, std::vector< KBuffInfo > >::iterator mit = kPacket_.m_mapActivateBuff.find( GetCharUID() );
	if( mit != kPacket_.m_mapActivateBuff.end() )
	{
		// �߰� �� ������ ������ ������.
		std::vector< KBuffInfo > vecActivateBuff;
		BOOST_TEST_FOREACH( KBuffInfo&, kInfo, mit->second )
		{
			ActivateBuff( kInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID, vecActivateBuff );
		}

		if( vecActivateBuff.empty() == false )
		{
			// �α��� ���� ���� �Ŵ����� ������Ʈ
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
			kReq.m_iUnitUID = GetCharUID();
			kReq.m_vecActivateBuff = vecActivateBuff;
			SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
		}
	}	
#endif SERV_SERVER_BUFF_SYSTEM
	//}
}

//{{ 2009. 2. 4  ������	���� ���÷���
//_IMPL_ON_FUNC( ERM_PARTY_GAME_REPLAY_START_ACK, KERM_PARTY_OPEN_DUNGEON_ROOM_ACK )
//{
//	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
//
//	if( kPacket_.m_iOK == NetError::NET_OK )
//	{
//		if( GetPartyUID() == 0 )
//		{
//			START_LOG( cerr, L"��Ƽ�� ���� ���� ���÷��̸� �ߴµ� ��Ƽ UID�� ����.!" )
//				<< BUILD_LOG( GetCharName() )
//				<< END_LOG;
//		}
//	}
//
//	KEGS_QUICK_START_DUNGEON_GAME_ACK kPacket;
//	kPacket.m_iOK = kPacket_.m_iOK;
//	SendPacket( EGS_QUICK_START_DUNGEON_GAME_ACK, kPacket );
//}
//
//_IMPL_ON_FUNC( ERM_PARTY_GAME_REPLAY_START_NOT, KEGS_PARTY_GAME_START_NOT )
//{
//	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
//
//	if( GetAuthLevel() < SEnum::UAL_GM )
//		kPacket_.m_wstrCNIP.clear();
//
//	if( GetRoomUID() != kPacket_.m_RoomInfo.m_RoomUID )
//	{
//		START_LOG( cerr, L"���� ���÷����ε� RoomUID�� �ٸ��� �ȵǴµ�?" )
//			<< BUILD_LOG( GetCharUID() )
//			<< BUILD_LOG( GetRoomUID() )
//			<< BUILD_LOG( kPacket_.m_RoomInfo.m_RoomUID )
//			<< END_LOG;
//
//		// roomuid�� �ϴ� �־��ش�. ���࿡ roomuid�� 0�̶��.. �̰� ������ �ȴ�
//		SetRoomUID( kPacket_.m_RoomInfo.m_RoomUID );
//	}
//
//	m_kTimer[TM_ROOM].restart();
//
//	switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
//	{
//	case CXSLRoom::RT_DUNGEON:
//		{
//			int iRequireDungeonID = kPacket_.m_RoomInfo.m_iDungeonID + static_cast<int>(kPacket_.m_RoomInfo.m_DifficultyLevel);
//
//			CXSLDungeon::DUNGEON_MODE eDungeonMode = (CXSLDungeon::DUNGEON_MODE) kPacket_.m_RoomInfo.m_cDungeonMode;
//			
//			//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
//#ifdef SERV_NEW_HENIR_TEST
//			// ��ϸ� �����̶�� ���� ���� Ƚ���� ������Ų��.
//			if( CXSLDungeon::IsHenirDungeon( kPacket_.m_RoomInfo.m_iDungeonID ) == true )
//			{
//				if( m_kUserDungeonManager.IncreaseHenirRewardCount() == false )
//				{
//					START_LOG( cerr, L"��ϸ� ���� ȹ�� Ƚ�� ���� ����!!" )
//						<< BUILD_LOG( GetCharName() )
//						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardNormalCount() )
//						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardPremiumCount() )
//						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardEventCount() )
//						<< END_LOG;
//				}
//			}
//#endif SERV_NEW_HENIR_TEST
//			//}}
//
//			//{{ 2011. 02. 21  ��μ�	Ư�� ���� ���� ������ ���� ����
//#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
//			int iItemID = 0;
//			if( !DeleteRequiredItemToEnterDungeonRoom( iRequireDungeonID, eDungeonMode, kPacket_.m_vecInventorySlotInfo, iItemID ) )
//#else
//			if( !DeleteRequiredItemToEnterDungeonRoom( iRequireDungeonID, eDungeonMode, kPacket_.m_vecInventorySlotInfo ) )
//#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
//				//}}
//			{
//				// �켱 ���ӽ��۽� �������� ������ ������ �α׸� ���ܺ���..
//				START_LOG( cerr, L"�������� ���۽� �ʿ�������� ����.!" )
//					<< BUILD_LOG( GetCharName() )
//					<< BUILD_LOG( iRequireDungeonID )
//					<< BUILD_LOG( (int)eDungeonMode )
//					<< END_LOG;
//
//				//{{ 2011. 02. 21  ��μ�	Ư�� ���� ���� ������ ���� ����
//#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
//				// �������� ���� ����
//				SetReserveKickUser( true );
//				SetDungeonRequiredItemID( iItemID );
//
//// 				KERM_RESERVE_LEAVE_USER_NOT kPacketReq;
//// 				kPacketReq.m_iUnitUID = GetCharUID();
//// 				kPacketReq.m_iItemID = iItemID;
//// 				SendToCnRoom( ERM_RESERVE_LEAVE_USER_NOT, kPacketReq );
//#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
//				//}}
//				// �� ���� ���� ó�� - ���ͼ����� ERM_LEAVE_ROOM_REQ�� ������ ����Ż ó�� ��
//				// ���ƿ��� ACK�� KGSFSM�� �ɷ��� Ŭ���̾�Ʈ�� ���۵��� �ʴ´�.
//				//KERM_LEAVE_ROOM_REQ kPacketReq;
//				//kPacketReq.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_22;
//				//SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacketReq );
//				//return;
//			}
//		}
//		break;
//
//	default:
//		{
//			START_LOG( cerr, L"�Ͼ�� ���� ����!" )
//				<< BUILD_LOG( GetCharUID() )
//				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
//				<< END_LOG;
//		}
//	}	
//
//	SendPacket( EGS_PARTY_GAME_START_NOT, kPacket_ );
//
//	//{{ 2009. 5. 30  ������	�ӽ� �κ�
//	ClearTempInventory();
//	//}}
//}
//}}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_HOST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ UID �̻�.!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_PARTY_CHANGE_HOST_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_HOST_ACK, kAck );
		return;
	}

	//{{ 2010. 02. 26  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ChangeHost( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_ChangeHost( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}
	{
		START_LOG( cwarn, L"��Ƽ�� �ٲٱ� ����.!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_PARTY_BAN_USER_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ UID �̻�.!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_PARTY_BAN_USER_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_BAN_USER_ACK, kAck );
		return;
	}

	//�ڱ��ڽ��� ���� ��ų�� ����.
	if( kPacket_ == GetCharUID() )
	{
		KEGS_PARTY_BAN_USER_ACK kAck;
		kAck.m_iOK = NetError::ERR_ROOM_26;
		SendPacket( EGS_PARTY_BAN_USER_ACK, kAck );
		return;
	}

	//{{ 2010. 02. 26  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_BanUser( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_BanUser( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cwarn, L"��Ƽ���� ����.!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

//{{ 2009. 3. 18  ������	��Ƽ������
IMPL_ON_FUNC( ERM_PARTY_BAN_USER_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// �����߾ not�� ��������!
	}

	//{{ 2010. 8. 18	������	���� �ڵ� ����
	if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
	{
		ClearTempInventory();
	}
	//}}

	// �� ��Żó��
	SetRoomUID( 0 );

	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	SendID( EGS_PARTY_BAN_USER_NOT );

	if( GetTradeUID() != 0 )
	{
		KEGS_BREAK_TRADE_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
		_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
	}
}
//}}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_GET_ITEM_TYPE_REQ )
{	
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ UID �̻�.!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK, kAck );
		return;
	}

	//{{ 2010. 03. 02  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ChangeGetItemtype( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_ChangeGetItemtype( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"�������� ������ ������ �������." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_NUM_OF_PER_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ UID �̻�.!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_PARTY_CHANGE_NUM_OF_PER_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_NUM_OF_PER_ACK, kAck );
		return;
	}

	//{{ 2010. 03. 02  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ChangeNumOfPer( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_ChangeNumOfPer( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cwarn, L"��Ƽ �ο����� ����.!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

//{{ 2009. 4. 1  ������		��Ƽ����
IMPL_ON_FUNC( EGS_PARTY_LIMIT_LEVEL_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ UID �̻�.!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_PARTY_LIMIT_LEVEL_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_LIMIT_LEVEL_ACK, kAck );
		return;
	}

	//{{ 2010. 03. 02  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
#else
	if( SiKPartyManager()->E_LimitLevel( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cwarn, L"��Ƽ ���� �ɼ� ���� ����.!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}
//}}

IMPL_ON_FUNC( EPM_CREATE_PARTY_BY_INVITE_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INVITE_PARTY_ACK );
    
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"��Ƽ ���� ����!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}
	else
	{
		// ������ ��Ƽ�� ������ �����̹Ƿ� ��Ƽ �Ҽ����� ����!
		SetPartyUID( kPacket_.m_kPartyInfo.m_iPartyUID );
	}

	KEGS_INVITE_PARTY_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iReceiverUID = kPacket_.m_kReceiverPartyUserInfo.m_iUnitUID;
	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	kPacket.m_wstrUDPRelayIP = kPacket_.m_kHostPartyUserInfo.m_wstrIP;
	kPacket.m_usUDPRelayPort = kPacket_.m_kHostPartyUserInfo.m_usPort;
#endif SERV_CHOOSE_FASTEST_HOST
	//}
	SendPacket( EGS_INVITE_PARTY_ACK, kPacket );
}

_IMPL_ON_FUNC( EPM_JOIN_PARTY_NOT, KEGS_JOIN_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		BOOST_TEST_FOREACH( const KPartyInfo&, kInfo, kPacket_.m_vecPartyInfo )
		{
			//{{ 2010. 03. 12  ������	ä�� ���� ��Ƽ ���� ����
			if( GetPartyUID() > 0 )
			{
				if( GetPartyUID() != kInfo.m_iPartyUID )
				{
					START_LOG( cerr, L"�̹� ��Ƽ �Ҽ��ε� �� ��Ƽ ����ó���� �Ǿ���.. ������Ƽ�� ��Ż ��Ű��!" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( GetCharName() )
						<< BUILD_LOG( GetPartyUID() )
						<< BUILD_LOG( kInfo.m_iPartyUID )
						<< END_LOG;

					// ���� ��Ƽ ��Ż ó��
					//SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_05 );

					// ���ο� ��Ƽ ������ ���ؼ��� �ڵ� ��Ż ó���Ѵ�.
					KEPM_LEAVE_PARTY_REQ kPacketReq;
					kPacketReq.m_iPartyUID = kInfo.m_iPartyUID;
					kPacketReq.m_iReason = NetError::NOT_LEAVE_PARTY_REASON_05;
					_SendToCnParty( kInfo.m_iPartyUID, EPM_LEAVE_PARTY_REQ, kPacketReq );
					return;
				}
				//else
				//{
				//	START_LOG( cerr, L"�̹� PartyUID�� ����Ǿ��ִµ� �Ȱ��� PartyUID�� �� �����Ϸ��� �ϳ�? ���� �Ͼ���� �ȵǴ� ����!" )
				//		<< BUILD_LOG( GetCharUID() )
				//		<< BUILD_LOG( GetCharName() )
				//		<< BUILD_LOG( GetPartyUID() )
				//		<< END_LOG;

				//	// �̰� ��Żó���� �� �ʿ� ���� ��Ȳ.
				//}
			}

			// ����� partyuid����
			SetCandidatePartyUID( 0 );
			//}}

			// partyuid�� ����
			SetPartyUID( kInfo.m_iPartyUID );

			//{{ 2012. 04. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
			// ��Ƽ�� �������� ��������!
			m_kUserDungeonManager.SetDungeonGameInfo( kInfo.m_iDungeonID, kInfo.m_DifficultyLevel, kInfo.m_cDungeonMode );
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			break;
		}

		if( GetPartyUID() <= 0 )
		{
			START_LOG( cerr, L"��ƼUID���� �̻��մϴ�. ���� �Ͼ���� �ȵǴ¿���!" )
				<< BUILD_LOG( GetPartyUID() )
				<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
				<< BUILD_LOG( kPacket_.m_vecPartyInfo.size() )
				<< BUILD_LOG( kPacket_.m_vecPartyUserInfo.size() )
				<< END_LOG;
		}

		//{{ 2012. 02. 22	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		// ���� ��Ʋ�ʵ忡�� ��Ƽ ������ �ߴٸ� ��Ʋ�ʵ忡�� ��Ƽ ���� ������ ������!
		if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
		{
			KERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT kPacketToRM;
			kPacketToRM.m_iPartyUID = GetPartyUID();
			kPacketToRM.m_vecPartyUnitUIDList = kPacket_.m_vecCurPartyUnitUIDList;
			SendToCnRoom( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT, kPacketToRM );
		}
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
		bool bExistComeBackUser = false;
		BOOST_TEST_FOREACH( const KPartyUserInfo&, kInfo, kPacket_.m_vecPartyUserInfo )
		{
			if( kInfo.m_bComeBack == true )
			{
				bExistComeBackUser = true;
				break;
			}
		}

		if( bExistComeBackUser == true )
		{
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
			kReq.m_iUnitUID = GetCharUID();

			ActivateBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO, kReq.m_vecActivateBuff );
			
			if( kReq.m_vecActivateBuff.empty() == false )
			{
				SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
			}
		}
#endif SERV_SERVER_BUFF_SYSTEM
		//}
	}
	//{{ 2010. 03. 12  ������	ä�� ���� ��Ƽ ���� ����
	else if( kPacket_.m_iOK == NetError::ERR_PARTY_18 )
	{
		// ����� partyuid����
		SetCandidatePartyUID( 0 );
	}
	//}}

	SendPacket( EGS_JOIN_PARTY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_CLOSE_PARTY_NOT, KEGS_CLOSE_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2012. 02. 22	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// �ӽ� ��Ƽ ����� ��� ���� ��Ż ó��!
	if( GetCandidatePartyUID() != 0  &&  GetCandidatePartyUID() == kPacket_.m_iPartyUID )
	{
		SetCandidatePartyUID( 0 );
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	if( GetPartyUID() == 0  ||  GetPartyUID() != kPacket_.m_iPartyUID )
	{
		START_LOG( cerr, L"���� ���� ��Ƽ�ε� ��Ƽ �ݱ� ó���� �Դ�!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
		return;
	}

	// ��Ƽ ��Ż ó��!
	SetPartyUID( 0 );

	//{{ 2012. 02. 22	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// ���� ��Ʋ�ʵ忡�� ��Ƽ ������ �ߴٸ� ��Ʋ�ʵ忡�� ��Ƽ ���� ������ ������!
	if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		KERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT kPacketToRM;
		kPacketToRM.m_iPartyUID = 0; // ��Żó���ϱ� 0���� ����
		SendToCnRoom( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT, kPacketToRM );
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	if( m_kComeBackUserInfo.GetIsComeBackUser() == false )
	{
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
		kReq.m_iUnitUID = GetCharUID();

		DeactivateBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO, kReq.m_vecDeActivateBuff );

		if( kReq.m_vecDeActivateBuff.empty() == false )
		{
			SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
		}
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}

	SendPacket( EGS_CLOSE_PARTY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_UPDATE_PARTY_USER_INFO_BROAD_NOT, KEGS_UPDATE_PARTY_USER_INFO_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_UPDATE_PARTY_USER_INFO_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_READY_ACK, KEGS_PARTY_CHANGE_READY_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_READY_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_READY_NOT, KEGS_PARTY_CHANGE_READY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_READY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_DUNGEON_ACK, KEGS_PARTY_CHANGE_DUNGEON_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_DUNGEON_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_DUNGEON_NOT, KEGS_PARTY_CHANGE_DUNGEON_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2012. 04. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_kUserDungeonManager.SetDungeonGameInfo( kPacket_.m_iDungeonID, kPacket_.m_DifficultyLevel, kPacket_.m_cDungeonMode );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	SendPacket( EGS_PARTY_CHANGE_DUNGEON_NOT, kPacket_ );
}

IMPL_ON_FUNC( EPM_PARTY_GAME_START_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ���� �������� ������ ���� ���� ���� �˾��� ����.
	//{{ 2012. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	if( kPacket_.m_iOK == NetError::ERR_DUNGEON_REQUIRE_ITEM_00 )
	{
		KEGS_ERROR_POPUP_NOT kNot;
		kNot.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
		//{{ ���� ����� ���� �˾� ����
#ifdef SERV_CHANGE_ERROR_POPUP
		kNot.m_bPartyMember = true;
#endif SERV_CHANGE_ERROR_POPUP
		//}}
		if( SiCXSLDungeonManager()->GetRequireItemIDAndCount( kPacket_.m_iDungeonID, static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_iDungeonMode), kNot.m_iErrorData1, kNot.m_iErrorData2 ) == true )
		{
			SendPacket( EGS_ERROR_POPUP_NOT, kNot );
		}
		else
		{
			START_LOG( cerr, L"���� ����� ������ ã�� ���ߴ�!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iDungeonID )
				<< BUILD_LOG( kPacket_.m_iDungeonMode )
				<< END_LOG;
		}
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	KEGS_QUICK_START_DUNGEON_GAME_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iFailUnitUID = kPacket_.m_iFailUnitUID;
	kPacket.m_wstrFailUserNickName = kPacket_.m_wstrFailUserNickName;
	SendPacket( EGS_QUICK_START_DUNGEON_GAME_ACK, kPacket );
}

IMPL_ON_FUNC( EPM_CHECK_FOR_PARTY_GAME_START_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEPM_CHECK_FOR_PARTY_GAME_START_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	// ��Ƽ �Ҽ����� �˻�
	if( GetPartyUID() <= 0 )
	{
		START_LOG( cerr, L"��Ƽ���� �ƴѵ� ��Ƽ���� ���� �������ڴ� �̺�Ʈ�� �Դ�?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
		return;
	}

	// �ٸ� ��Ƽ������ �˻�
	if( GetPartyUID() != kPacket_.m_iPartyUID )
	{
		START_LOG( cerr, L"�ش� �Ҽ� ��Ƽ���� �ƴѵ� �ٸ� ��Ƽ���� ���� �������ڴ� �̺�Ʈ�� �Դ�?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
		return;
	}

	switch( kPacket_.m_usEventID )
	{
	case ERM_PARTY_OPEN_DUNGEON_ROOM_REQ:
		{
			if( GetStateID() != KGSFSM::S_FIELD_MAP )
			{
				if( GetStateID() == KGSFSM::S_ROOM  &&  CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_BATTLE_FIELD )
				{
					START_LOG( cwarn, L"��Ƽ��� ���� ���°� �ƴ϶� ������ �����Ҽ� ����." )
						<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
						<< BUILD_LOG( GetStateIDString() )
						<< END_LOG;

					kPacket.m_iOK = NetError::ERR_UNKNOWN;
					SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
					return;
				}
			}
		}
		break;

	//case ERM_PARTY_GAME_REPLAY_START_REQ:
	//	{
	//		if( GetStateID() != KGSFSM::S_ROOM )
	//		{
	//			START_LOG( cwarn, L"��Ƽ��� ���� ���°� �ƴ϶� ������ �����Ҽ� ����." )
	//				<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
	//				<< BUILD_LOG( GetStateIDString() )
	//				<< END_LOG;

	//			kPacket.m_iOK = NetError::ERR_UNKNOWN;
	//			SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
	//			return;
	//		}
	//	}
	//	break;

	default:
		{
			START_LOG( cerr, L"�߸��� �̺�Ʈ ���� �Դϴ�. ���� �Ͼ�����¿���!" )
				<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
			return;
		}
		break;
	}

	m_kUserDungeonManager.SetDungeonGameInfo( kPacket_.m_kDungeonGameInfo );

	// PartyRoomUserInfo���
	KPartyRoomUserInfo kPartyRoomUserInfo;
	GetTutorUnitUIDList( kPartyRoomUserInfo.m_vecStudentUnitUID );
	GetRoomUserInfo( kPartyRoomUserInfo.m_kRoomUserInfo, CXSLRoom::RT_DUNGEON );
	//{{ 2009. 6. 10  ������	���� ������ ��� EXP,ED���
	kPartyRoomUserInfo.m_iCurEXP = GetEXP();
	kPartyRoomUserInfo.m_iCurED = GetED();
	//}}
	kPartyRoomUserInfo.m_wstrChannelIP = NetCommon::GetLocalIPW();

	kPacket.m_iPartyUID = kPacket_.m_iPartyUID;
	kPacket.m_sWorldID = kPacket_.m_sWorldID;
	kPacket.m_usEventID = kPacket_.m_usEventID;
	kPacket.m_vecUserInfo.push_back( kPartyRoomUserInfo );
	SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
}

_IMPL_ON_FUNC( EPM_PARTY_OPEN_DUNGEON_ROOM_NOT, KERM_PARTY_OPEN_DUNGEON_ROOM_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2013. 04. 10	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	kPacket_.m_kRoomInfo.m_bDefenceDungeonOpen = SiKGSWorldMissionManager()->GetIsActive();
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	SendToCnRoom( ERM_PARTY_OPEN_DUNGEON_ROOM_REQ, kPacket_ );
}

//_IMPL_ON_FUNC( EPM_PARTY_GAME_REPLAY_START_NOT, KERM_PARTY_OPEN_DUNGEON_ROOM_REQ )
//{
//	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
//
//	SendToCnRoom( ERM_PARTY_GAME_REPLAY_START_REQ, kPacket_ );
//}

//{{ 2011. 07. 19	������	���� ����
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( EPM_CHECK_PARTY_CHANGE_HOST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEPM_CHECK_PARTY_CHANGE_HOST_ACK kPacket;	
	kPacket.m_iPartyUID = kPacket_.m_iPartyUID;
	kPacket.m_iOldHostUnitUID = kPacket_.m_iOldHostUnitUID;
	kPacket.m_iNewHostUnitUID = kPacket_.m_iNewHostUnitUID;
	//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	kPacket.m_bForRegroupParty = kPacket_.m_bForRegroupParty;
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}

	// �ش� ��Ƽ�� �´��� �˻�
	if( kPacket_.m_iPartyUID != GetPartyUID() )
	{
		START_LOG( cerr, L"��Ƽ �Ҽ��� �ƴѵ� ��Ƽ�� ���� ��û�� ���Ա�." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	// �ش� ������ �´��� �˻�
	if( kPacket_.m_iNewHostUnitUID != GetCharUID() )
	{
		START_LOG( cerr, L"��Ƽ �Ҽ��� �ƴѵ� ��Ƽ�� ���� ��û�� ���Ա�." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	switch( kPacket_.m_cPartyType )
	{
	case KPartyInfo::PT_DUNGEON_PARTY:
		{
			//�ٲ���Ƽ���� ���� ������ ������ �Ҽ��ִ��� Ȯ��.
			if( CheckingSpirit( kPacket_.m_iDungeonID ) == false )
			{
				START_LOG( cwarn, L"���� �Ӹ�� ������ ���� ������ ������ �����Ҽ�����." )
					<< BUILD_LOG( kPacket_.m_iPartyUID )
					<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_PARTY_31;
				SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
				return;
			}

			const int iDungeonID = kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_cDifficulty);
			const CXSLDungeon::DUNGEON_MODE eDungeonMode = static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_cDungeonMode);

			if( CheckRequiredItemToEnterDungeonRoom( iDungeonID, eDungeonMode ) == false )
			{
				START_LOG( cwarn, L"���� �Ӹ�� ������ ���� ������ ������ �����Ҽ�����." )
					<< BUILD_LOG( kPacket_.m_iPartyUID )
					<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_PARTY_31;
				SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
				return;
			}

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
			if( CheckLimitedPlayTimes( iDungeonID ) == false )
			{
				START_LOG( cwarn, L"���� �Ӹ�� ������ ���� ������ ������ �����Ҽ�����." )
					<< BUILD_LOG( kPacket_.m_iPartyUID )
					<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_DUNGEON_ENTER_LIMIT_00;
				SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
				return;
			}
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

			const int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( kPacket_.m_iDungeonID );
			if( iRequireDungeonID != 0 )
			{
				std::map< int, KDungeonClearInfo >::iterator mit;
				mit = m_mapDungeonClear.find( iRequireDungeonID );
				if( mit == m_mapDungeonClear.end() )
				{
					START_LOG( cwarn, L"���� �Ӹ�� ������ ���� ������ ������ �����Ҽ�����." )
						<< BUILD_LOG( kPacket_.m_iPartyUID )
						<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;

					kPacket.m_iOK = NetError::ERR_PARTY_31;
					SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
					return;
				}
			}
		}
		break;

	case KPartyInfo::PT_PVP_PARTY:
		{
			// ���� ��Ƽ ���� ����
			if( GetLevel() < SEnum::PE_PVP_OFFICIAL_LIMIT_LEVEL )
			{
				START_LOG( cwarn, L"���� ������ �ϱ⿡ ������ ĳ���� �����Դϴ�." )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetMatchUID() )
					<< BUILD_LOGc( GetLevel() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_MATCH_MAKING_11;
				SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
				return;
			}			
		}
		break;

	default:
		{
			START_LOG( cerr, L"��Ƽ Ÿ���� �̻��մϴ�!" )
				<< BUILD_LOG( kPacket_.m_iPartyUID )
				<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOGc( kPacket_.m_cPartyType )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PARTY_27;
			SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
			return;
		}
		break;
	}

	// ��� ������ �����ߴٸ� OK���� ������!
	kPacket.m_iOK = NetError::NET_OK;
	SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( EPM_CHECK_PARTY_CHANGE_HOST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEPM_CHECK_PARTY_CHANGE_HOST_ACK kPacket;	
	kPacket.m_iPartyUID = kPacket_.m_iPartyUID;
	kPacket.m_iOldHostUnitUID = kPacket_.m_iOldHostUnitUID;
	kPacket.m_iNewHostUnitUID = kPacket_.m_iNewHostUnitUID;

	// �ش� ��Ƽ�� �´��� �˻�
	if( kPacket_.m_iPartyUID != GetPartyUID() )
	{
		START_LOG( cerr, L"��Ƽ �Ҽ��� �ƴѵ� ��Ƽ�� ���� ��û�� ���Ա�." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	// �ش� ������ �´��� �˻�
	if( kPacket_.m_iNewHostUnitUID != GetCharUID() )
	{
		START_LOG( cerr, L"��Ƽ �Ҽ��� �ƴѵ� ��Ƽ�� ���� ��û�� ���Ա�." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	//�ٲ���Ƽ���� ���� ������ ������ �Ҽ��ִ��� Ȯ��.
	if( CheckingSpirit( kPacket_.m_iDungeonID ) == false )
	{
		START_LOG( cwarn, L"���� �Ӹ�� ������ ���� ������ ������ �����Ҽ�����." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_31;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	const int iDungeonID = kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_cDifficulty);
	const CXSLDungeon::DUNGEON_MODE eDungeonMode = static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_cDungeonMode);

	if( CheckRequiredItemToEnterDungeonRoom( iDungeonID, eDungeonMode ) == false )
	{
		START_LOG( cwarn, L"���� �Ӹ�� ������ ���� ������ ������ �����Ҽ�����." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_31;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	const int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( kPacket_.m_iDungeonID );
	if( iRequireDungeonID != 0 )
	{
		std::map< int, KDungeonClearInfo >::iterator mit;
		mit = m_mapDungeonClear.find( iRequireDungeonID );
		if( mit == m_mapDungeonClear.end() )
		{
			START_LOG( cwarn, L"���� �Ӹ�� ������ ���� ������ ������ �����Ҽ�����." )
				<< BUILD_LOG( kPacket_.m_iPartyUID )
				<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PARTY_31;
			SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
			return;
		}
	}

	// ��� ������ �����ߴٸ� OK���� ������!
	kPacket.m_iOK = NetError::NET_OK;
	SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_HOST_ACK, KEGS_PARTY_CHANGE_HOST_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_HOST_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_HOST_NOT, KEGS_PARTY_CHANGE_HOST_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_HOST_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_BAN_USER_ACK, KEGS_PARTY_BAN_USER_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_BAN_USER_ACK, kPacket_ );

	//{{ 2011. 07. 08	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_NUM_OF_PER_ACK, KEGS_PARTY_CHANGE_NUM_OF_PER_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_NUM_OF_PER_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_NUM_OF_PER_NOT, KEGS_PARTY_CHANGE_NUM_OF_PER_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_NUM_OF_PER_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_GET_ITEM_TYPE_ACK, KEGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_GET_ITEM_TYPE_NOT, KEGS_PARTY_CHANGE_GET_ITEM_TYPE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_GET_ITEM_TYPE_NOT, kPacket_ );
}

IMPL_ON_FUNC( EPM_PARTY_BAN_USER_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2010. 02. 09  ������	������Ƽ ��ɰ���
#ifdef SERV_QUICK_PARTY_UPGRADE
	AddKickedPartyUID( GetPartyUID() );
#endif SERV_QUICK_PARTY_UPGRADE
	//}}

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
	//{{ 2009. 3. 18  ������	�������� ���� ������ ��Ż��Ű��!
	if( GetRoomUID() > 0 )
	{
		//{{ 2010. 03. 17  ������	���� ���� ����
		if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
		{
			SendToCnRoom( ERM_PARTY_BAN_USER_REQ );
		}
		else if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
		{
			SendID( EGS_PARTY_BAN_USER_NOT );
		}
		else
		{
			START_LOG( cerr, L"�������� �ƴѵ� ��Ƽ�� �ΰ� �־���?" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< END_LOG;
		}
		//}}
	}
	else
	{
		SendID( EGS_PARTY_BAN_USER_NOT );
	}

	//{{ 2011. 07. 08	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

_IMPL_ON_FUNC( EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, KEGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_bCanEnterSecretStage == false )
	{
		SendPacket( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kPacket_ );
		return;
	}

	// ������ ��н������� üũ�Ϸ� ����!
	SendToCnRoom( ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ );
}

_IMPL_ON_FUNC( EPM_CHAT_ACK, KEGS_CHAT_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHAT_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_CHAT_NOT, KEGS_CHAT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHAT_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_CHANGE_PARTY_FEVER_STATE_NOT, KEGS_CHANGE_PARTY_FEVER_STATE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_PARTY_FEVER_STATE_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_FEVER_POINT_NOT, KEGS_PARTY_FEVER_POINT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_FEVER_POINT_NOT, kPacket_ );
}

IMPL_ON_FUNC( EPM_SEND_UPDATE_PARTY_MEMBER_LIST_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// �ʵ忡 ��Ƽ�� ����Ʈ�� ������Ʈ �Ϸ� ����!
	SendUpdatePartyMemberList( kPacket_.m_iPartyUID, kPacket_.m_setPartyUIDList );
}

IMPL_ON_FUNC_NOPARAM( EPM_QUICK_SLOT_COOL_TIME_RESET_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ������ �� Ÿ�� ����
	m_kInventory.ResetQuickSlotCoolTime();
}

//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
IMPL_ON_FUNC( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ UID �̻�.!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kAck );
		return;
	}

	// ���� ������ ������ ���� ���� �ʴ� ������ �ִ��� �˻�
	KGSPartyPtr spParty = SiKPartyListManager()->GetParty( GetPartyUID() );
	if( spParty == NULL )
	{
		START_LOG( cerr, L"��Ƽ UID �̻�.!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kAck );
		return;
	}

	int iDungeonMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( spParty->GetDungeonID() + static_cast<int>(spParty->GetDungeonDif()) );

	if( spParty->CheckPartyMinLevel( iDungeonMinLevel ) == false )
	{
		KEGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK kAck;
		kAck.m_iOK = NetError::ERR_CHANGE_PARTY_MIN_LEVEL_CHECK_FAILE;
		SendPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kAck );
		return;
	}

	//{{ 2010. 02. 23  ������	ä�� ���� ��Ƽ

	if( SiKPartyListManager()->E_ChangeCheckMinLevel( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"��Ƽ �ּ� ���� ���� ���� ���� ����!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kPacket_ );
}

IMPL_ON_FUNC( EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_NOT, kPacket_ );
}
#endif SERV_DUNGEON_DIFFICULTY_REVISION
//}}

//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
IMPL_ON_FUNC( EPM_CHANGE_COME_BACK_BUFF_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	
	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
	kReq.m_iUnitUID = GetCharUID();

	if( kPacket_.m_bIsBuffFinish == false )
	{
		ActivateBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO, kReq.m_vecActivateBuff );
	}
	else
	{
		DeactivateBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO, kReq.m_vecDeActivateBuff );
	}

	if( kReq.m_vecActivateBuff.empty() == false || kReq.m_vecDeActivateBuff.empty() == false )
	{
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
#else
	CTime tComeBackBuffEnd;

	if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrComeBackBuffEnd, tComeBackBuffEnd ) == false )
	{
		tComeBackBuffEnd = CTime( 0 );
		START_LOG( cerr, L"�ð� ��ȯ ����!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( kPacket_.m_wstrComeBackBuffEnd )
			<< END_LOG;
	}

	KEGS_COME_BACK_BUFF_NOT kNot;
	kNot.m_bIsFinish = kPacket_.m_bIsBuffFinish;
	kNot.m_iComeBackBuffEnd = tComeBackBuffEnd.GetTime();
	SendPacket( EGS_COME_BACK_BUFF_NOT, kNot );
#endif SERV_SERVER_BUFF_SYSTEM
	//}
}
#endif SERV_COME_BACK_USER_REWARD
//}}

//{{ 2012. 05. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
_IMPL_ON_FUNC( ERM_GAME_START_TIME_COUNT_NOT, KEGS_GAME_START_TIME_COUNT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_GAME_START_TIME_COUNT_NOT, kPacket_ );

	//{{ 2013. 01. 01	������	�������� �ʵ�� ���ƿ��� ���ϴ� ���� ����
#ifdef SERV_SKILL_CHANGE_COOL_TIME_AT_START_COUNT
	// ��ų ü���� �� Ÿ�� üũ�� ���� Ÿ�̸Ӹ� �ʱ�ȭ �Ѵ�.
	m_kTimer[TM_SKILL_CHANGE_COOL_TIME_AT_START_COUNT].restart();
#endif SERV_SKILL_CHANGE_COOL_TIME_AT_START_COUNT
	//}}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}



