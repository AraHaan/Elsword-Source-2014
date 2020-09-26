#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
#include "NexonBillingTCPManager.h"
#endif SERV_GUILD_CHANGE_NAME
//}}
#include "X2Data/XSLStringFilter.h"

#include "odbc/Odbc.h"
#include "Enum/Enum.h"

#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
#include "GSSimLayer.h"
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

#ifdef SERV_STRING_FILTER_USING_DB
#include "StringFilterManager.h"
#endif //SERV_STRING_FILTER_USING_DB

//////////////////////////////////////////////////////////////////////////
//#ifdef SERV_GSUSER_CPP
//#pragma NOTE( "GSUserHandler.cpp ���� ������ �˴ϴ�!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

//{{ 2009. 9. 18  ������	���
#ifdef GUILD_TEST

IMPL_ON_FUNC( EGS_CREATE_GUILD_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CREATE_GUILD_REQ, EGS_CREATE_GUILD_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_CREATE_GUILD_ACK kAck;

		// ü��ID�� ���â�� �Ұ���
		if( IsGuestUser() )
		{
			kAck.m_iOK = NetError::ERR_GUEST_USER_00;
			SendPacket( EGS_CREATE_GUILD_ACK, kAck );
			return;
		}

		// ��� �̸� ���� �˻�
		std::string strGuildName = KncUtil::toNarrowString( kPacket_.m_wstrGuildName );
		if( kPacket_.m_wstrGuildName.empty()  ||  strGuildName.size() > 16 )
		{
			kAck.m_iOK = NetError::ERR_GUILD_45;
			SendPacket( EGS_CREATE_GUILD_ACK, kAck );
			return;
		}

		// �г��Ӱ� ������ �������� ��� �̸��� ���� ���͸� ����
#ifdef SERV_STRING_FILTER_USING_DB
		if( SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrGuildName ) == false )
#else //SERV_STRING_FILTER_USING_DB
		if( SiCXSLStringFilter()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrGuildName ) == false )
#endif //SERV_STRING_FILTER_USING_DB
		{
			kAck.m_iOK = NetError::ERR_GUILD_04;
			SendPacket( EGS_CREATE_GUILD_ACK, kAck );
			return;
		}

		// sql injection �˻�
		if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrGuildName ) )
		{
			kAck.m_iOK = NetError::ERR_ODBC_00;
			SendPacket( EGS_CREATE_GUILD_ACK, kAck );
			return;
		}

		// sql injection �˻�
		if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrGuildMessage ) )
		{
			kAck.m_iOK = NetError::ERR_ODBC_00;
			SendPacket( EGS_CREATE_GUILD_ACK, kAck );
			return;
		}

		// ���� ���� ������ ��쿡�� ��� ������ �Ұ����ϴ�!
		if( GetGuildUID() > 0 )
		{
			START_LOG( cerr, L"�̹� ��忡 ������ ���¶� ��� ���� �Ұ�! �Ͼ���� �ȵǴ� ����" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetGuildUID() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUILD_00;
			SendPacket( EGS_CREATE_GUILD_ACK, kAck );
			return;
		}

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
			SendPacket( EGS_CREATE_GUILD_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		// ��� â�� �������� ��밡������ �˻�
		if( m_kInventory.UseGuildFoundationItemCheckOnly( kPacket_.m_iItemUID ) == false )
		{
			kAck.m_iOK = NetError::GetLastError();
			SendPacket( EGS_CREATE_GUILD_ACK, kAck );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
	if ( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_GLOBAL_SERVICE )
	{
		// 2010. 07. 26.	�ڱ���		���� ���ϼ� ����
		KEGIANT_ROLEREG_CREATE_GUILD_REQ kPacketReq;
		kPacketReq.m_iActorUID = GetCharUID();
		kPacketReq.m_uiGiantUID = m_kNexonAccountInfo.m_uiNexonSN;
		kPacketReq.m_CreateGuildReqPacket = kPacket_;

		SendToGiantRoleReg( EGIANT_ROLEREG_CREATE_GUILD_REQ, kPacketReq );
	}
	else
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
	{
		//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
	// Login Server�� ��� �����Ϸ� ����!
	KELG_CREATE_GUILD_REQ kPacketToLG;
	kPacketToLG.m_iUnitUID = GetCharUID();
	kPacketToLG.m_iItemUID = kPacket_.m_iItemUID;
	kPacketToLG.m_wstrGuildName = kPacket_.m_wstrGuildName;
	kPacketToLG.m_wstrGuildMessage = kPacket_.m_wstrGuildMessage;
	SendToLoginServer( ELG_CREATE_GUILD_REQ, kPacketToLG );
#else
	// DB�� ������ �������� �˻��Ϸ� ����!
	KDBE_CREATE_GUILD_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iItemUID = kPacket_.m_iItemUID;
	kPacketToDB.m_wstrGuildName = kPacket_.m_wstrGuildName;
	kPacketToDB.m_wstrGuildMessage = kPacket_.m_wstrGuildMessage;
	SendToGameDB( DBE_CREATE_GUILD_REQ, kPacketToDB );
#endif SERV_GUILD_CHANGE_NAME
	}

}
//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
IMPL_ON_FUNC( ELG_CREATE_GUILD_ACK )
#else
IMPL_ON_FUNC( DBE_CREATE_GUILD_ACK )
#endif SERV_GUILD_CHANGE_NAME
//}}
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CREATE_GUILD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_CREATE_GUILD_ACK kPacketAck;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		if( kPacket_.m_bReqGiant_DeleteName )
		{
			// �̸� ���ϼ� ���� ���� �Ŀ� ���� ���� �ٽ� ���̾�Ʈ�� ���� ��û�ؾ� �� 
			KEGIANT_ROLEREG_DELETE_GUILD_REQ kPacketReq;
			kPacketReq.m_uiGiantUID = m_kNexonAccountInfo.m_uiNexonSN;
			kPacketReq.m_wstrGuildName = kPacket_.m_wstrGuildName;

			SendToGiantRoleReg( EGIANT_ROLEREG_DELETE_GUILD_REQ, kPacketReq );
		}
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_CREATE_GUILD_ACK, kPacketAck );
		return;
	}

	// ��� ���� ���������� �����۵��� ������!
	if( m_kInventory.UseGuildFoundationItem( kPacket_.m_iItemUID, kPacketAck.m_vecInventorySlotInfo, false ) == false )
	{
		START_LOG( cerr, L"��� ���� ������ ���� ����! �˻��ߴµ�? �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< END_LOG;
	}

	// ������ ����� GuildUID�� �����Ѵ� - ��� ���������� ��� �����ͷ� �����
	m_kUserGuildManager.SetGuildInfo( kPacket_.m_kCreatedGuildInfo.m_iGuildUID, kPacket_.m_kCreatedGuildInfo.m_wstrGuildName, SEnum::GUG_MASTER );

	// Ŭ���̾�Ʈ�� ��� ������ ���� ������ ������.
	kPacketAck.m_kGuildInfo = kPacket_.m_kCreatedGuildInfo;	
	kPacketAck.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_CREATE_GUILD_ACK, kPacketAck );

	// DB�� ������ ��� ������ GuildManager���� ������Ʈ �Ϸ� ����!
	KELG_CREATE_GUILD_RESULT_REQ kPacketToLg;
	kPacketToLg.m_kGuildInfo = kPacket_.m_kCreatedGuildInfo;
	//{{ 2009. 12. 3  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	kPacketToLg.m_kGuildSkillInfo = kPacket_.m_kGuildSkillInfo;
#endif GUILD_SKILL_TEST
	//}}
	GetGuildMemberInfo( kPacketToLg.m_kLoginGuildMember );
	kPacketToLg.m_vecMemberList.push_back( kPacketToLg.m_kLoginGuildMember );
	SendToLoginServer( ELG_CREATE_GUILD_RESULT_REQ, kPacketToLg );

	//{{ 2009. 11. 2  ������	���â�� ����
	{
		KEGS_NOTIFY_MSG_NOT	kPacketNot;
		kPacketNot.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_GUILD_CREATION;
		kPacketNot.m_Count = 1;		

		CStringW cwstrMessage;
		cwstrMessage.Format( L"%s;%s", GetCharName().c_str(), kPacket_.m_kCreatedGuildInfo.m_wstrGuildName.c_str() );
		kPacketNot.m_wstrMSG = static_cast<LPCTSTR>(cwstrMessage);

		SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacketNot );
	}
	//}}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 12. 22  ������	�ż����̺�Ʈ "2009-12-23 ~ 2010-01-27"
	//if( KBaseServer::GetKObj()->GetServerGroupID() == SEnum::SGI_GAIA )
	//{
	//	// DB�� ���� ������ ������.
	//	KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
	//	kPacketToDB.m_iFromUnitUID = GetCharUID();
	//	kPacketToDB.m_iToUnitUID   = GetCharUID();
	//	kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT; // ���� Ÿ��
	//	kPacketToDB.m_iRewardID	   = 195;
	//	SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
	//}
	//}}
	//////////////////////////////////////////////////////////////////////////

#ifdef SERV_CREATE_GUILD_EVENT
	// �̺�Ʈ ������ ����!
//	KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
//	kPacketToDB.m_iFromUnitUID = GetCharUID();
//	kPacketToDB.m_iToUnitUID   = GetCharUID();
//	kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
//	kPacketToDB.m_iRewardID	   = 832;// �׳�ý��� ���ູ(15��,5GSP) ť��
//	SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );	
#endif //SERV_CREATE_GUILD_EVENT


	// ��� ���� ���������� ����̸� ����������!
	if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		// �ʵ忡 ������� ���� ���� �����ϱ�
		if( GetFieldUID() > 0 )
		{
			KERM_CHANGE_FIELD_USER_INFO_NOT kNot;
			GetFieldUserInfo( kNot.m_kInfo );
			SendToGSField( ERM_CHANGE_FIELD_USER_INFO_NOT, kNot );
		}
	}		
	else if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			{
				KRoomUserInfo kRoomUserInfo;
				GetRoomUserInfo( kRoomUserInfo );
				SendToCnRoom( ERM_CHANGE_ROOM_USER_INFO_NOT, kRoomUserInfo );
			}
			break;

		case CXSLRoom::RT_SQUARE:
			{
				KSquareUserInfo kSquareUserInfo;
				GetSquareUserInfo( kSquareUserInfo );
				SendToCnRoom( ERM_CHANGE_SQUARE_USER_INFO_NOT, kSquareUserInfo );
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
}

_IMPL_ON_FUNC( ELG_CREATE_GUILD_RESULT_ACK, KEGS_GUILD_INFO_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// �α��μ����� ������Ʈ �س���� ����� �ٽ� Ŭ���̾�Ʈ���� ����!
	SendPacket( EGS_GUILD_INFO_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ELG_GET_GUILD_INFO_ACK, KEGS_GUILD_INFO_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2009. 11. 25  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	m_kUserGuildManager.SetGuildSkill( kPacket_.m_kGuildSkillInfo );
#endif GUILD_SKILL_TEST
	//}}

	SendPacket( EGS_GUILD_INFO_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_GET_GUILD_USER_LIST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ���� ��� �Ҽ����� ����
	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"���Ҽӵ� �ƴѵ� �����������Ʈ �������� �ϳ�?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< END_LOG;

		KEGS_GET_GUILD_USER_LIST_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUILD_16;
		SendPacket( EGS_GET_GUILD_USER_LIST_ACK, kPacket );
		return;
	}

	// �α��μ����� ������
	KELG_GET_GUILD_USER_LIST_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = GetGuildUID();
	kPacketToLg.m_uiViewPage = kPacket_.m_uiViewPage;
	kPacketToLg.m_cGuildUserListType = kPacket_.m_cGuildUserListType;
	SendToLoginServer( ELG_GET_GUILD_USER_LIST_REQ, kPacketToLg );
}

_IMPL_ON_FUNC( ELG_GET_GUILD_USER_LIST_ACK, KEGS_GET_GUILD_USER_LIST_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_GET_GUILD_USER_LIST_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ELG_GUILD_INFO_NOT, KEGS_GUILD_INFO_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_GUILD_INFO_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ELG_GUILD_MESSAGE_NOT, KEGS_GUILD_MESSAGE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_cMessageType == KEGS_GUILD_MESSAGE_NOT::MT_KICK_MEMBER  ||
		kPacket_.m_cMessageType == KEGS_GUILD_MESSAGE_NOT::MT_KICK_BY_MASTER )
	{
		// Ż���ϴ� ����� �����? �ʱ�ȭ!
		if( GetCharUID() == kPacket_.m_iUnitUID )
		{
			// ��� ���� �ʱ�ȭ
			m_kUserGuildManager.Clear();

			// ��� �̸� ����
			if( GetStateID() == KGSFSM::S_FIELD_MAP )
			{
				// �ʵ忡 ������� ���� ���� �����ϱ�
				if( GetFieldUID() > 0 )
				{
					KERM_CHANGE_FIELD_USER_INFO_NOT kNot;
					GetFieldUserInfo( kNot.m_kInfo );
					SendToGSField( ERM_CHANGE_FIELD_USER_INFO_NOT, kNot );
				}
			}		
			else if( GetStateID() == KGSFSM::S_ROOM )
			{
				switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
				{
				case CXSLRoom::RT_PVP:
				case CXSLRoom::RT_DUNGEON:
				case CXSLRoom::RT_TRAININGCENTER:
					{
						KRoomUserInfo kRoomUserInfo;
						GetRoomUserInfo( kRoomUserInfo );
						SendToCnRoom( ERM_CHANGE_ROOM_USER_INFO_NOT, kRoomUserInfo );

						//{{ 2009. 12. 8  ������	��� ��ų
#ifdef GUILD_SKILL_TEST
						KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
						kPacketNot = kRoomUserInfo;
						SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
#endif GUILD_SKILL_TEST
						//}}
					}
					break;

				case CXSLRoom::RT_SQUARE:
					{
						KSquareUserInfo kSquareUserInfo;
						GetSquareUserInfo( kSquareUserInfo );
						SendToCnRoom( ERM_CHANGE_SQUARE_USER_INFO_NOT, kSquareUserInfo );
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
		}
	}
	else if( kPacket_.m_cMessageType == KEGS_GUILD_MESSAGE_NOT::MT_CHANGE_MEMBER_GRADE )
	{
		if( GetCharUID() == kPacket_.m_iUnitUID )
		{
			// ���� �� ��� ���
			const u_char ucBeforeMemberShipGrade = m_kUserGuildManager.GetMemberShipGrade();

			// ����� ��� ���濡 ���� ó��
			m_kUserGuildManager.SetMemberShipGrade( static_cast<u_char>(kPacket_.m_iMessageData) );

			// ���࿡ ����̶�� ����� �ٸ� �����鿡�� �ش� ������ ��ε�ĳ���� ����!
			if( ucBeforeMemberShipGrade == SEnum::GUG_NEW_USER  || 
				m_kUserGuildManager.GetMemberShipGrade() == SEnum::GUG_NEW_USER )
			{
				if( GetStateID() == KGSFSM::S_ROOM )
				{
					switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
					{
					case CXSLRoom::RT_PVP:
					case CXSLRoom::RT_DUNGEON:
					case CXSLRoom::RT_TRAININGCENTER:
						{
							KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
							GetRoomUserInfo( kPacketNot );
							SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
						}
						break;
					}
				}
			}			
		}
	}
	else if( kPacket_.m_cMessageType == KEGS_GUILD_MESSAGE_NOT::MT_INSERT_GUILD_CSPOINT )
	{
		// ������� �Ⱓ�� ��ų ����Ʈ�� ����!
		m_kUserGuildManager.SetMaxGuildCSPoint( kPacket_.m_iMessageData2 );
	}

	SendPacket( EGS_GUILD_MESSAGE_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_INVITE_GUILD_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_CHAT, kPacket_.m_wstrNickName ) == false )
	{
		KEGS_INVITE_GUILD_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RECOMMEND_USER_00;
		SendPacket( EGS_INVITE_GUILD_ACK, kPacket );
		return;
	}
#endif //SERV_STRING_FILTER_USING_DB

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"��忡 ���ԵǾ����� ������ ����ʴ븦 �Ϸ���." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< END_LOG;

		KEGS_INVITE_GUILD_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUILD_07;
		SendPacket( EGS_INVITE_GUILD_ACK, kPacket );
		return;
	}

	// ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrNickName ) == true )
	{
		KEGS_INVITE_GUILD_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_INVITE_GUILD_ACK, kPacket );
		return;
	}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 12  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( kPacket_.m_iUnitUID > 0 )
	{
		if( IsSameServerGroupUID( kPacket_.m_iUnitUID ) == false )
		{
			KEGS_INVITE_GUILD_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_DIF_SERVER_GROUP_02;
			SendPacket( EGS_INVITE_GUILD_ACK, kPacket );
			return;
		}
	}	
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	// Guild Manager�� ���� ���� �׸��� üũ�մϴ�.
	KELG_INVITE_GUILD_REQ kPacket;
	kPacket.m_iSenderUnitUID = GetCharUID();
	kPacket.m_iGuildUID = GetGuildUID();
	kPacket.m_wstrReceiverNickName = kPacket_.m_wstrNickName;
	kPacket.m_iReceiverUnitUID = kPacket_.m_iUnitUID;
	SendToLoginServer( ELG_INVITE_GUILD_REQ, kPacket );
}

IMPL_ON_FUNC( ELG_INVITE_GUILD_FIND_USER_REQ )
{
	// ���¸� ������?

	KELG_INVITE_GUILD_FIND_USER_ACK kPacket;
	kPacket.m_usEventID = kPacket_.m_usEventID;
	kPacket.m_iGuildUID = kPacket_.m_kGuildInfo.m_iGuildUID;
	kPacket.m_iSenderUnitUID = kPacket_.m_iSenderUnitUID;
	kPacket.m_iReceiverUnitUID = kPacket_.m_iReceiverUnitUID;
	kPacket.m_wstrReceiverNickName = GetCharName();

	// ����� �Դ°�?
	if( kPacket_.m_iReceiverUnitUID != GetCharUID() )
	{
		START_LOG( cerr, L"�ʴ����� �߸��Դ�!? �Ͼ���� �ȵǴ¿���!" )
			<< BUILD_LOG( kPacket_.m_iReceiverUnitUID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_07;
		SendToLoginServer( ELG_INVITE_GUILD_FIND_USER_ACK, kPacket );
		return;
	}

	// �̹� ��忡 ���ԵǾ��ִ��� üũ
	if( GetGuildUID() > 0 )
	{
		// �̹� ��忡 ���ԵǾ��ֳ�?
		kPacket.m_iOK = NetError::ERR_GUILD_15;
		SendToLoginServer( ELG_INVITE_GUILD_FIND_USER_ACK, kPacket );
		return;
	}

	// �ʴ��Ϸ��� ���� ���� �������� üũ
	if( GetGuildUID() == kPacket_.m_kGuildInfo.m_iGuildUID )
	{
		START_LOG( cerr, L"�̹� �츮 �����̳�? GuildManager���� �˻������ٵ�.." )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_14;
		SendToLoginServer( ELG_INVITE_GUILD_FIND_USER_ACK, kPacket );
		return;
	}

	// ��� �ʴ� �ź�
	if( m_kUserGameOption.IsDenyInviteGuild() )
	{
		kPacket.m_iOK = NetError::ERR_GUILD_17; // ��� �ʴ� ����
		SendToLoginServer( ELG_INVITE_GUILD_FIND_USER_ACK, kPacket );
		return;
	}

	// ������ ������!
	kPacket.m_iOK = NetError::NET_OK;
	SendToLoginServer( ELG_INVITE_GUILD_FIND_USER_ACK, kPacket );

	// �ʴ����� Ŭ���̾�Ʈ�� ������!
	KEGS_INVITE_GUILD_NOT kPacketNot;
	kPacketNot.m_kGuildInfo = kPacket_.m_kGuildInfo;
	kPacketNot.m_wstrSenderNickName = kPacket_.m_wstrSenderNickName;
	kPacketNot.m_bAutoJoinGuild = kPacket_.m_bAutoJoinGuild;
	SendPacket( EGS_INVITE_GUILD_NOT, kPacketNot );
}

IMPL_ON_FUNC( ELG_INVITE_GUILD_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// �������� �����߿��� ã�� ���ߴٸ�..
	if( kPacket_.m_iOK == NetError::ERR_GUILD_08 )
	{
		// DB�� ���� �����ϴ� �г������� ã�ƺ���!
		SendToGameDB( DBE_INVITE_GUILD_NICKNAME_CHECK_REQ, kPacket_ );
		return;
	}

	KEGS_INVITE_GUILD_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;
	SendPacket( EGS_INVITE_GUILD_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_INVITE_GUILD_NICKNAME_CHECK_ACK, KELG_INVITE_GUILD_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_INVITE_GUILD_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_INVITE_GUILD_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_INVITE_GUILD_REPLY_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ��� ���� �ʴ뿡 ���� ������ �˸�������!
	KELG_INVITE_GUILD_REPLY_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacketToLg.m_wstrSenderNickName = kPacket_.m_wstrSenderNickName;
	kPacketToLg.m_iReason = kPacket_.m_iReason;
	kPacketToLg.m_iUnitUID = GetCharUID();
	kPacketToLg.m_wstrNickName = GetCharName();
	SendToLoginServer( ELG_INVITE_GUILD_REPLY_REQ, kPacketToLg );
}

IMPL_ON_FUNC( ELG_INVITE_GUILD_REPLY_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ��� ���� ��û ������ ���
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KDBE_JOIN_GUILD_REQ kPacketToDB;
		kPacketToDB.m_iGuildUID = kPacket_.m_iGuildUID;		
		kPacketToDB.m_iGuildMasterUnitUID = kPacket_.m_iGuildMasterUnitUID;
		GetGuildMemberInfo( kPacketToDB.m_kJoinGuildMember );
		SendToGameDB( DBE_JOIN_GUILD_REQ, kPacketToDB );
	}

	KEGS_INVITE_GUILD_REPLY_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_INVITE_GUILD_REPLY_ACK, kPacket );
}

_IMPL_ON_FUNC( ELG_INVITE_GUILD_REPLY_DENY_NOT, KEGS_INVITE_GUILD_REPLY_DENY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_INVITE_GUILD_REPLY_DENY_NOT, kPacket_ );
}

IMPL_ON_FUNC( ELG_JOIN_GUILD_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ��� ���� ����
	m_kUserGuildManager.SetGuildInfo( kPacket_.m_kGuildInfoNot.m_kGuildInfo.m_iGuildUID, kPacket_.m_kGuildInfoNot.m_kGuildInfo.m_wstrGuildName, SEnum::GUG_NEW_USER );
	//{{ 2009. 11. 25  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	m_kUserGuildManager.SetGuildSkill( kPacket_.m_kGuildInfoNot.m_kGuildSkillInfo );
#endif GUILD_SKILL_TEST
	//}}

	// �α��μ����κ��� ��������� �޾ƿԴٸ�! ��� ���� ����� �˷�����!
	KEGS_JOIN_GUILD_SUCCESS_NOT kPacketNot;
	kPacketNot.m_kGuildInfo = kPacket_.m_kGuildInfoNot.m_kGuildInfo;
	kPacketNot.m_wstrGuildMasterName = kPacket_.m_kGuildInfoNot.m_wstrGuildMasterName;
	kPacketNot.m_vecMemberUnitUID = kPacket_.m_kGuildInfoNot.m_vecMemberUnitUID;
	//{{ 2009. 11. 25  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	kPacketNot.m_kGuildSkillInfo = kPacket_.m_kGuildInfoNot.m_kGuildSkillInfo;
#endif GUILD_SKILL_TEST
	//}}
	SendPacket( EGS_JOIN_GUILD_SUCCESS_NOT, kPacketNot );

	// ��� �̸� ����
	if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		// �ʵ忡 ������� ���� ���� �����ϱ�
		if( GetFieldUID() > 0 )
		{
			KERM_CHANGE_FIELD_USER_INFO_NOT kNot;
			GetFieldUserInfo( kNot.m_kInfo );
			SendToGSField( ERM_CHANGE_FIELD_USER_INFO_NOT, kNot );
		}
	}		
	else if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			{
				KRoomUserInfo kRoomUserInfo;
				GetRoomUserInfo( kRoomUserInfo );
				SendToCnRoom( ERM_CHANGE_ROOM_USER_INFO_NOT, kRoomUserInfo );
			}
			break;

		case CXSLRoom::RT_SQUARE:
			{
				KSquareUserInfo kSquareUserInfo;
				GetSquareUserInfo( kSquareUserInfo );
				SendToCnRoom( ERM_CHANGE_SQUARE_USER_INFO_NOT, kSquareUserInfo );
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
}

IMPL_ON_FUNC( EGS_CHANGE_GUILD_MEMBER_GRADE_REQ )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_GUILD_MEMBER_GRADE_REQ, EGS_CHANGE_GUILD_MEMBER_GRADE_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"������ �ƴѵ� ����޺����� �Ϸ��� �մϴ�!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_CHANGE_GUILD_MEMBER_GRADE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_CHANGE_GUILD_MEMBER_GRADE_ACK, kPacket );
		return;
	}

	// �ڱ� �ڽ��� ��� ����� �����Ҽ� ����
	if( kPacket_.m_iUnitUID == GetCharUID() )
	{
		START_LOG( cerr, L"�ڱ� �ڽ��� ����� ����Ұ�! Ŭ�󿡼� üũ ���ϴ°�?" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_CHANGE_GUILD_MEMBER_GRADE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUILD_36;
		SendPacket( EGS_CHANGE_GUILD_MEMBER_GRADE_ACK, kPacket );
		return;
	}	

	// �α��μ����� ���� �������� ���� üũ
	KELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iGuildUID = GetGuildUID();
	kPacketToDB.m_iTargetUnitUID = kPacket_.m_iUnitUID;
	kPacketToDB.m_ucMemberShipGrade = kPacket_.m_ucMemberShipGrade;
	SendToLoginServer( ELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_REQ, kPacketToDB );
}

IMPL_ON_FUNC( ELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_GUILD_MEMBER_GRADE_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_CHANGE_GUILD_MEMBER_GRADE_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_CHANGE_GUILD_MEMBER_GRADE_ACK, kPacket );
		return;
	}

	// DB�� ���� ��� ��� ��� ������ ��û�Ѵ�.
	KDBE_CHANGE_GUILD_MEMBER_GRADE_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iGuildUID = GetGuildUID();
	kPacketToDB.m_iTargetUnitUID = kPacket_.m_iTargetUnitUID;
	kPacketToDB.m_wstrTargetNickName = kPacket_.m_wstrTargetNickName;
	kPacketToDB.m_ucMemberShipGrade = kPacket_.m_ucMemberShipGrade;
	kPacketToDB.m_bChangeGuildMaster = kPacket_.m_bChangeGuildMaster;
	SendToGameDB( DBE_CHANGE_GUILD_MEMBER_GRADE_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_CHANGE_GUILD_MEMBER_GRADE_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_GUILD_MEMBER_GRADE_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	// ��� ����
	KEGS_CHANGE_GUILD_MEMBER_GRADE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_wstrTargetNickName = kPacket_.m_wstrTargetNickName;
	kPacket.m_ucMemberShipGrade = kPacket_.m_ucMemberShipGrade;
	SendPacket( EGS_CHANGE_GUILD_MEMBER_GRADE_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_CHANGE_GUILD_MESSAGE_REQ )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_GUILD_MESSAGE_REQ, EGS_CHANGE_GUILD_MESSAGE_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

#ifdef SERV_STRING_FILTER_USING_DB
	kPacket_.m_wstrMessage = SiKStringFilterManager()->FilteringChatString( kPacket_.m_wstrMessage.c_str(), L'��' );
#endif //SERV_STRING_FILTER_USING_DB

	KEGS_CHANGE_GUILD_MESSAGE_ACK kPacket;

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"������ �ƴѵ� ���޽����� �����Ϸ��� �մϴ�!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_CHANGE_GUILD_MESSAGE_ACK, kPacket );
		return;
	}

	// ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrMessage ) )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_CHANGE_GUILD_MESSAGE_ACK, kPacket );
		return;
	}

	// DB�� ���� ��� �޽��� ������ ��û�Ѵ�.
	KDBE_CHANGE_GUILD_MESSAGE_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iGuildUID = GetGuildUID();
	kPacketToDB.m_wstrMessage = kPacket_.m_wstrMessage;
	SendToGameDB( DBE_CHANGE_GUILD_MESSAGE_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_CHANGE_GUILD_MESSAGE_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_GUILD_MESSAGE_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}	

	// ��� ����
	KEGS_CHANGE_GUILD_MESSAGE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_CHANGE_GUILD_MESSAGE_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_KICK_GUILD_MEMBER_REQ )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_KICK_GUILD_MEMBER_REQ, EGS_KICK_GUILD_MEMBER_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"������ �ƴѵ� ��� Ż�� �Ǵ� ���� ���� �Ϸ��� �մϴ�!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_KICK_GUILD_MEMBER_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_KICK_GUILD_MEMBER_ACK, kPacket );
		return;
	}

	// DB�� ���� ��� ��� Ż�� ��û�Ѵ�.
	KDBE_KICK_GUILD_MEMBER_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iGuildUID = GetGuildUID();
	kPacketToDB.m_iTargetUnitUID = kPacket_.m_iUnitUID;
	SendToGameDB( DBE_KICK_GUILD_MEMBER_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_KICK_GUILD_MEMBER_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_KICK_GUILD_MEMBER_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	// ��� ����
	KEGS_KICK_GUILD_MEMBER_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_bKickBySysop = ( GetCharUID() != kPacket_.m_iTargetUnitUID );
	SendPacket( EGS_KICK_GUILD_MEMBER_ACK, kPacket );


	// ���� Ż�� �����ϸ� ��� �Ŵ����� ������ ������ ������Ʈ �Ѵ�.
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// Ż���ϴ� ����� �����? �ʱ�ȭ!
		if( GetCharUID() == kPacket_.m_iTargetUnitUID )
		{
			m_kUserGuildManager.Clear();
		}
	}
}

IMPL_ON_FUNC( EGS_CHANGE_GUILD_MEMBER_MESSAGE_REQ )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_GUILD_MEMBER_MESSAGE_REQ, EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

#ifdef SERV_STRING_FILTER_USING_DB
	kPacket_.m_wstrMessage = SiKStringFilterManager()->FilteringChatString( kPacket_.m_wstrMessage.c_str(), L'��' );
#endif //SERV_STRING_FILTER_USING_DB

	KEGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK kPacket;

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"������ �ƴѵ� ���� �λ縻 ������ �Ϸ��� �մϴ�!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK, kPacket );
		return;
	}

	// ���� �λ縻 ���� ���� ����
	std::string strMessage = KncUtil::toNarrowString( kPacket_.m_wstrMessage );
	if( strMessage.size() > 60 )
	{
		kPacket.m_iOK = NetError::ERR_GUILD_27;
		SendPacket( EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK, kPacket );
		return;
	}

	// ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrMessage ) )
	{		
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK, kPacket );
		return;
	}

	// DB�� ���� ��� ��� ��� ������ ��û�Ѵ�.
	KDBE_CHANGE_GUILD_MEMBER_MESSAGE_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iTargetUnitUID = kPacket_.m_iUnitUID;
	kPacketToDB.m_iGuildUID = GetGuildUID();
	kPacketToDB.m_wstrMessage = kPacket_.m_wstrMessage;
	SendToGameDB( DBE_CHANGE_GUILD_MEMBER_MESSAGE_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_CHANGE_GUILD_MEMBER_MESSAGE_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	// ��� ����
	KEGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_bDelete = kPacket_.m_wstrMessage.empty(); // �޽����� ��������� delete��!
	SendPacket( EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK, kPacket );
}

IMPL_ON_FUNC_NOPARAM( EGS_DISBAND_GUILD_REQ )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_DISBAND_GUILD_REQ, EGS_DISBAND_GUILD_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}	

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"������ �ƴѵ� ����ػ��� �Ϸ��� �մϴ�!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_DISBAND_GUILD_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_DISBAND_GUILD_ACK, kPacket );
		return;
	}

	// DB�� ���� ����ػ� ��û�� �Ѵ�.
	KDBE_DISBAND_GUILD_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iGuildUID = GetGuildUID();
	SendToGameDB( DBE_DISBAND_GUILD_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_DISBAND_GUILD_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_DISBAND_GUILD_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	// ��� ����
	KEGS_DISBAND_GUILD_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_DISBAND_GUILD_ACK, kPacket );


	// ����ػ��� �����ϸ� ��� ���� �ʱ�ȭ
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		if ( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_GLOBAL_SERVICE )
		{
			KEGIANT_ROLEREG_DELETE_GUILD_REQ kPacketReq;
			kPacketReq.m_uiGiantUID = m_kNexonAccountInfo.m_uiNexonSN;
			kPacketReq.m_wstrGuildName = m_kUserGuildManager.GetGuildName();

			SendToGiantRoleReg( EGIANT_ROLEREG_DELETE_GUILD_REQ, kPacketReq );
		}
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

		// ������� �ʱ�ȭ!
		m_kUserGuildManager.Clear();

		// ��� �̸� ����
		if( GetStateID() == KGSFSM::S_FIELD_MAP )
		{
			// �ʵ忡 ������� ���� ���� �����ϱ�
			if( GetFieldUID() > 0 )
			{
				KERM_CHANGE_FIELD_USER_INFO_NOT kNot;
				GetFieldUserInfo( kNot.m_kInfo );
				SendToGSField( ERM_CHANGE_FIELD_USER_INFO_NOT, kNot );
			}
		}
		else if( GetStateID() == KGSFSM::S_ROOM )
		{
			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_PVP:
			case CXSLRoom::RT_DUNGEON:
			case CXSLRoom::RT_TRAININGCENTER:
				{
					KRoomUserInfo kRoomUserInfo;
					GetRoomUserInfo( kRoomUserInfo );
					SendToCnRoom( ERM_CHANGE_ROOM_USER_INFO_NOT, kRoomUserInfo );
				}
				break;

			case CXSLRoom::RT_SQUARE:
				{
					KSquareUserInfo kSquareUserInfo;
					GetSquareUserInfo( kSquareUserInfo );
					SendToCnRoom( ERM_CHANGE_SQUARE_USER_INFO_NOT, kSquareUserInfo );
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
	}
}

IMPL_ON_FUNC( ELG_CHECK_EXPAND_GUILD_MAX_MEMBER_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
		return;
	}

	if( m_kInventory.UseItemInInventory( kPacket_.m_iItemUID, kPacket.m_vecKInventorySlotInfo ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	//{{ 2012. 11. 15	�ڼ���	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	kPacket.m_iED = GetED();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );

	// ��� �ο� Ȯ���Ϸ� ����!
	KDBE_EXPAND_GUILD_MAX_MEMBER_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iGuildUID = GetGuildUID();
	SendToGameDB( DBE_EXPAND_GUILD_MAX_MEMBER_REQ, kPacketToDB );
}

//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
IMPL_ON_FUNC( EGS_CHANGE_GUILD_NAME_CHECK_REQ )
{
	/*
	
	����: EGS_CHANGE_GUILD_NAME_REQ�� �ݵ�� EGS_CHANGE_GUILD_NAME_CHECK�� ���������� ����� �ڿ��� �۵� �ϰԲ� �Ѵ�.

	1. EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_CHECK_REQ �� �ߺ� ���͸� �Ǵ�.
	2. �ߺ� �˻簡 ���� ��,
		m_iOK != NetError::NET_OK �̸�
		REMOVE_VERIFY_STATE_REPEAT_FILTER( EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );�� ����	EGS_CHANGE_GUILD_NAME_REQ�� �����ϰ�,
		SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket_ );�� ����			EGS_CHANGE_GUILD_NAME_CHECK_REQ�� �����Ѵ�.

		m_iOK == NetError::NET_OK �̸�
		SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket_ );�� ����			EGS_CHANGE_GUILD_NAME_CHECK_REQ�� �����Ѵ�.

	3. Ŭ���̾�Ʈ���� EGS_CHANGE_GUILD_NAME_REQ�� �������� ��,
	EGS_CHANGE_GUILD_NAME_CHECK_REQ�� �ɷ� �־ �ȵǸ�, ���ÿ� EGS_CHANGE_GUILD_NAME_REQ�� �ɷ� �־�߸� �Ѵ�.

	- EGS_CHANGE_GUILD_NAME_CHECK ���� EGS_CHANGE_GUILD_NAME_REQ�� �����Ͽ� ������ ��츦 ���Ҵ�.
	- EGS_CHANGE_GUILD_NAME_CHECK_ACK�� �ޱ� ���� ���۵� EGS_CHANGE_GUILD_NAME_REQ�� ������ ��츦 ���Ҵ�.

	*/

	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CHANGE_GUILD_NAME_CHECK_REQ, EGS_CHANGE_GUILD_NAME_CHECK_ACK );

	KEGS_CHANGE_GUILD_NAME_CHECK_ACK kPacket;

	// 1. ��忡 �ҼӵǾ����� �ʴٸ�, ��� �̸��� �ٲ� �� ����.
	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"��忡 �ҼӵǾ����� ������ ��� �̸��� �����Ϸ��� �մϴ�!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket );
		REMOVE_VERIFY_STATE_REPEAT_FILTER( EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );
		return;
	}

	const u_char ucMemberShipGrade = m_kUserGuildManager.GetMemberShipGrade();
	// 2. ���� ���� ��帶���Ͱ� �ƴ� ��쿡�� ��� �̸� ������ �Ұ����ϴ�!
	if( ucMemberShipGrade != SEnum::GUG_MASTER )
	{
		START_LOG( cerr, L"��� �����Ͱ� �ƴѵ� ��� �̸��� ���� �Ϸ���!" )
			<< BUILD_LOG( m_nUnitUID )
			<< BUILD_LOG( ucMemberShipGrade )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_46; // ��帶���Ͱ� �ƴմϴ�.
		SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket );
		REMOVE_VERIFY_STATE_REPEAT_FILTER( EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );
		return;
	}

	const std::string strGuildName = KncUtil::toNarrowString( kPacket_.m_wstrGuildName );
	// 3. ��� �̸� ���� �˻�
	if( kPacket_.m_wstrGuildName.empty()  ||  strGuildName.size() > 16 )
	{
		kPacket.m_iOK = NetError::ERR_GUILD_45;
		SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket );
		REMOVE_VERIFY_STATE_REPEAT_FILTER( EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );
		return;
	}

	// 4. �г��Ӱ� ������ �������� ��� �̸��� ���� ���͸� ����
#ifdef SERV_STRING_FILTER_USING_DB
	if( SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrGuildName ) == false )
#else //SERV_STRING_FILTER_USING_DB
	if( SiCXSLStringFilter()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrGuildName ) == false )
#endif //SERV_STRING_FILTER_USING_DB
	{
		kPacket.m_iOK = NetError::ERR_GUILD_04;
		SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket );
		REMOVE_VERIFY_STATE_REPEAT_FILTER( EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );
		return;
	}

	// 5. sql injection �˻�
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrGuildName ) )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket );
		REMOVE_VERIFY_STATE_REPEAT_FILTER( EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );
		return;
	}

	// 6. LoginUser�� ���� ��Ŷ ����
	KELG_CHANGE_GUILD_NAME_CHECK_REQ kPacketToLG;
	kPacketToLG.m_iUnitUID = GetCharUID();
	kPacketToLG.m_wstrNewGuildName = kPacket_.m_wstrGuildName;

	// LoginUser�� ����
	m_kUserGuildManager.SetGuildNameChanging();
	SendToLoginServer( ELG_CHANGE_GUILD_NAME_CHECK_REQ, kPacketToLG );
}

IMPL_ON_FUNC( ELG_CHANGE_GUILD_NAME_CHECK_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		REMOVE_VERIFY_STATE_REPEAT_FILTER( EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );
		m_kUserGuildManager.ResetGuildNameChanging();
	}
	SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_CHANGE_GUILD_NAME_REQ )
{
	JUST_VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CHANGE_GUILD_NAME_CHECK_REQ, EGS_CHANGE_GUILD_NAME_ACK );
	NEGATIVE_VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KELG_CHANGE_GUILD_NAME_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUILD_52;
		kPacket.m_iUnitUID = GetCharUID();
		SendToLoginServer( DBE_CHANGE_GUILD_NAME_ACK, kPacket );
		return;
	}

	KELG_CHANGE_GUILD_NAME_TIME_CHECK_REQ kPacket;
	kPacket.m_iUnitUID = GetCharUID();
	kPacket.m_ulOrderNo = kPacket_.m_ulOrderNo;
	kPacket.m_ulProductNo = kPacket_.m_ulProductNo;
	kPacket.m_iQuantity = kPacket_.m_iQuantity;
	SendToLoginServer( ELG_CHANGE_GUILD_NAME_TIME_CHECK_REQ, kPacket );
}

IMPL_ON_FUNC( ELG_CHANGE_GUILD_NAME_TIME_CHECK_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_CHANGE_GUILD_NAME_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_CHANGE_GUILD_NAME_ACK, kPacket );
		m_kUserGuildManager.ResetGuildNameChanging();
		return;
	}

#ifdef SERV_GLOBAL_BILLING
	// �ؿܿ� ������ ����
	KEBILL_PICK_UP_REQ kPacket;
	kPacket.m_iUnitUID = GetCharUID();
	kPacket.m_iTransNo		  = kPacket_.m_ulOrderNo;
	kPacket.m_iProductNo	  = kPacket_.m_ulProductNo;
	kPacket.m_byteProductKind = 0;
	SendToKOGBillingDB( EBILL_PICK_UP_REQ, kPacket );
#else //SERV_GLOBAL_BILLING
	// �ؽ� ���� ������ ����
	KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ kPacket;
	kPacket.m_ulPacketNo	  = SiKNexonBillingTCPManager()->GetNextPacketNo();
	kPacket.m_bytePacketType  = KNexonBillingTCPPacket::NISMS_INVENTORY_PICK_UP_ONCE;
	kPacket.m_ulOrderNo		  = kPacket_.m_ulOrderNo;
	kPacket.m_ulProductNo	  = kPacket_.m_ulProductNo;
	kPacket.m_usOrderQuantity = kPacket_.m_iQuantity;

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData( PI_GS_NX_BILLING_TCP, anTrace, ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ, kPacket );
	SiKNexonBillingTCPManager()->QueueingEvent( spEvent );
#endif //SERV_GLOBAL_BILLING
}

IMPL_ON_FUNC( ELG_CHANGE_GUILD_NAME_ACK )
{
	m_kUserGuildManager.ResetGuildNameChanging();

	KEGS_CHANGE_GUILD_NAME_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		if( kPacket_.m_iOK == NetError::ERR_GUILD_52 )
		{
			REMOVE_VERIFY_STATE_REPEAT_FILTER( EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );
		}
		else
		{
			SendPacket( EGS_CHANGE_GUILD_NAME_ACK, kPacket );
		}
		return;
	}

	// GameServer�� Guild Name ����
	m_kUserGuildManager.SetGuildName( kPacket_.m_kGuildInfo.m_wstrGuildName );

	// Ȥ�ó� �ؼ� �˻��غ�
	if( m_kUserGuildManager.GetGuildUID() != kPacket_.m_kGuildInfo.m_iGuildUID )
	{
		START_LOG( cerr, L"GuildUID ������ ��ġ���� �ʽ��ϴ�. ( ���� ���ɼ� ���� )" )
			<< BUILD_LOG( kPacket_.m_kGuildInfo.m_iGuildUID )
			<< BUILD_LOG( m_kUserGuildManager.GetGuildUID() )
			<< BUILD_LOG( kPacket_.m_kGuildInfo.m_wstrGuildName )
			<< END_LOG;

		// �׳� �Ѿ� ����!
	}

	// Ŭ���̾�Ʈ�� ��� �̸� ���濡 ���� ������ ������.
	kPacket.m_kGuildInfo = kPacket_.m_kGuildInfo;
	SendPacket( EGS_CHANGE_GUILD_NAME_ACK, kPacket );

	// DB�� ����� ��� ������ GuildManager���� ������Ʈ �Ϸ� ����!
	KELG_CHANGE_GUILD_NAME_RESULT_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = kPacket_.m_kGuildInfo.m_iGuildUID;
	kPacketToLg.m_wstrGuildName = kPacket_.m_kGuildInfo.m_wstrGuildName;

	SendToLoginServer( ELG_CHANGE_GUILD_NAME_RESULT_REQ, kPacketToLg );

	// ���� ���� ����
	KEGS_NOTIFY_MSG_NOT	kPacketNot;
	kPacketNot.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_GUILD_NAME_CHANGED;
	kPacketNot.m_Count = 1;		

	CStringW cwstrMessage;
	cwstrMessage.Format( L"%s;%s", kPacket_.m_wstrOldGuildName.c_str(), kPacket_.m_kGuildInfo.m_wstrGuildName.c_str() );
	kPacketNot.m_wstrMSG = static_cast<LPCTSTR>(cwstrMessage);

	SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacketNot );

	// ��� �̸� ���� ���������� ��� �̸� ����������!
	if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		// �ʵ忡 ������� ���� ���� �����ϱ�
		if( GetFieldUID() > 0 )
		{
			KERM_CHANGE_FIELD_USER_INFO_NOT kNot;
			GetFieldUserInfo( kNot.m_kInfo );
			SendToGSField( ERM_CHANGE_FIELD_USER_INFO_NOT, kNot );
		}
	}		
	else if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			{
				KRoomUserInfo kRoomUserInfo;
				GetRoomUserInfo( kRoomUserInfo );
				SendToCnRoom( ERM_CHANGE_ROOM_USER_INFO_NOT, kRoomUserInfo );
			}
			break;

		case CXSLRoom::RT_SQUARE:
			{
				KSquareUserInfo kSquareUserInfo;
				GetSquareUserInfo( kSquareUserInfo );
				SendToCnRoom( ERM_CHANGE_SQUARE_USER_INFO_NOT, kSquareUserInfo );
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
}
#endif SERV_GUILD_CHANGE_NAME
//}}

#endif GUILD_TEST
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 11. 23  ������	��彺ų
#ifdef GUILD_SKILL_TEST

IMPL_ON_FUNC( EGS_GET_GUILD_SKILL_REQ )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_GUILD_SKILL_REQ, EGS_GET_GUILD_SKILL_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}	

	KEGS_GET_GUILD_SKILL_ACK kPacketAck;

	// ��� �Ҽ����� �˻�
	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"��� �Ҽ��� �ƴѵ� ��彺ų�� �������Ѵ�." )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iGuildSkillID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_GET_GUILD_SKILL_ACK, kPacketAck );
		return;
	}

	// ��� ������ ������� �˻�
	if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER )
	{
		START_LOG( cerr, L"��帶���� ����� �ƴѵ� ��彺ų�� �������Ѵ�." )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOGc( m_kUserGuildManager.GetMemberShipGrade() )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_GUILD_SKILL_00;
		SendPacket( EGS_GET_GUILD_SKILL_ACK, kPacketAck );
		return;
	}

	KELG_GET_GUILD_SKILL_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = GetGuildUID();
	kPacketToLg.m_iUnitUID = GetCharUID();
	kPacketToLg.m_iGuildSkillID = kPacket_.m_iGuildSkillID;
	SendToLoginServer( ELG_GET_GUILD_SKILL_REQ, kPacketToLg );
}

_IMPL_ON_FUNC( ELG_GET_GUILD_SKILL_ACK, KEGS_GET_GUILD_SKILL_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_GUILD_SKILL_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	SendPacket( EGS_GET_GUILD_SKILL_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ELG_GET_GUILD_SKILL_NOT, KEGS_GET_GUILD_SKILL_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ��� ��� ��ų�� UserGuildManager�� �������!
	m_kUserGuildManager.UpdateGuildSkill( kPacket_ );

	// Ŭ���̾�Ʈ�� ȹ�� ��ų ���� ����
	SendPacket( EGS_GET_GUILD_SKILL_NOT, kPacket_ );

	// ���࿡ ����̶�� ����� �ٸ� �����鿡�� �ش� ������ ��ε�ĳ���� ����!
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			{
				KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
				GetRoomUserInfo( kPacketNot );
				SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
			}
			break;
		}
	}
}

IMPL_ON_FUNC( EGS_RESET_GUILD_SKILL_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_RESET_GUILD_SKILL_REQ, EGS_RESET_GUILD_SKILL_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_RESET_GUILD_SKILL_ACK kAck;

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
			SendPacket( EGS_RESET_GUILD_SKILL_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		// ��� �Ҽ����� �˻�
		if( GetGuildUID() <= 0 )
		{
			START_LOG( cerr, L"��� �Ҽ��� �ƴѵ� ��彺ų �ǵ����⸦ �Ϸ��� �Ѵ�." )
				<< BUILD_LOG( GetGuildUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iGuildSkillID )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUILD_19;
			SendPacket( EGS_RESET_GUILD_SKILL_ACK, kAck );
			return;
		}

		// ��� ������ ������� �˻�
		if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER )
		{
			START_LOG( cerr, L"��帶���� ����� �ƴѵ� ��彺ų �ǵ����⸦ �Ϸ��� �Ѵ�." )
				<< BUILD_LOG( GetGuildUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOGc( m_kUserGuildManager.GetMemberShipGrade() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUILD_SKILL_07;
			SendPacket( EGS_RESET_GUILD_SKILL_ACK, kAck );
			return;
		}

		// �κ��丮�� ��� ��ų �ǵ����� �޴��� �ִ��� �˻�		
		if( m_kInventory.ResetGuildSkillItemCheckOnly() == false )
		{
			START_LOG( cerr, L"��� ��ų �ʱ�ȭ �������� �κ��丮�� ����." )
				<< BUILD_LOG( GetGuildUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iGuildSkillID )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUILD_SKILL_08;
			SendPacket( EGS_RESET_GUILD_SKILL_ACK, kAck );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	KELG_RESET_GUILD_SKILL_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = GetGuildUID();
	kPacketToLg.m_iUnitUID = GetCharUID();
	kPacketToLg.m_iGuildSkillID = kPacket_.m_iGuildSkillID;
	SendToLoginServer( ELG_RESET_GUILD_SKILL_REQ, kPacketToLg );
}

_IMPL_ON_FUNC( ELG_RESET_GUILD_SKILL_ACK, KEGS_RESET_GUILD_SKILL_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_RESET_GUILD_SKILL_ACK );

	SendPacket( EGS_RESET_GUILD_SKILL_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ELG_RESET_GUILD_SKILL_DEL_ITEM_REQ, KDBE_RESET_GUILD_SKILL_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ��� ��ų �ǵ����� �޴��� ��������!
	if( m_kInventory.ResetGuildSkillItem( kPacket_.m_vecUpdatedInventorySlot ) == false )
	{
		START_LOG( cerr, L"��� ��ų �ʱ�ȭ �������� �κ��丮�� ����. �̸� �˻��ߴµ�.." )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( GetCharUID() )			
			<< BUILD_LOG( kPacket_.m_iGuildSkillID )
			<< END_LOG;

		KEGS_RESET_GUILD_SKILL_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_GUILD_SKILL_08;
		SendPacket( EGS_RESET_GUILD_SKILL_ACK, kPacketAck );
		return;
	}

	// ���� �����ϸ� DB�ΰ��� ������ ������������ ������Ʈ�ϰ� ��� ��ų �ʱ�ȭ ������ ������Ʈ����!
	m_kInventory.FlushQuantityChange( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );

	SendToGameDB( DBE_RESET_GUILD_SKILL_REQ, kPacket_ );
}

IMPL_ON_FUNC( DBE_RESET_GUILD_SKILL_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	}

	KEGS_RESET_GUILD_SKILL_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_RESET_GUILD_SKILL_ACK, kPacket );
}

_IMPL_ON_FUNC( ELG_RESET_GUILD_SKILL_NOT, KEGS_RESET_GUILD_SKILL_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ���� ��� �Ŵ����� ����
	if( m_kUserGuildManager.ResetGuildSkill( kPacket_ ) == false )
	{
		START_LOG( cerr, L"��� ��ų ���� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return;
	}

	SendPacket( EGS_RESET_GUILD_SKILL_NOT, kPacket_ );

	// ���࿡ ����̶�� ����� �ٸ� �����鿡�� �ش� ������ ��ε�ĳ���� ����!
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			{
				KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
				GetRoomUserInfo( kPacketNot );
				SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
			}
			break;
		}
	}
}

IMPL_ON_FUNC( EGS_INIT_GUILD_SKILL_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INIT_GUILD_SKILL_REQ, EGS_INIT_GUILD_SKILL_ACK );

	//////////////////////////////////////////////////////////////////////////
	// ����ó��
	{
		KEGS_INIT_GUILD_SKILL_ACK kAck;

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
			SendPacket( EGS_INIT_GUILD_SKILL_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		// ��� �Ҽ����� �˻�
		if( GetGuildUID() <= 0 )
		{
			START_LOG( cerr, L"��� �Ҽ��� �ƴѵ� ��彺ų �ʱ�ȭ�� �Ϸ��� �Ѵ�." )
				<< BUILD_LOG( GetGuildUID() )
				<< BUILD_LOG( GetCharUID() )			
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUILD_19;
			SendPacket( EGS_INIT_GUILD_SKILL_ACK, kAck );
			return;
		}

		// ��� ������ ������� �˻�
		if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER )
		{
			START_LOG( cerr, L"��帶���� ����� �ƴѵ� ��彺ų �ʱ�ȭ�� �Ϸ��� �Ѵ�." )
				<< BUILD_LOG( GetGuildUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOGc( m_kUserGuildManager.GetMemberShipGrade() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUILD_SKILL_11;
			SendPacket( EGS_INIT_GUILD_SKILL_ACK, kAck );
			return;
		}

		// �κ��丮�� ��� ��ų �ʱ�ȭ �������� �ִ��� �˻�
		if( m_kInventory.InitGuildSkillItemCheckOnly( kPacket_.m_iItemUID ) == false )
		{
			START_LOG( cerr, L"��ų�ʱ�ȭ �������� ���µ� ��Ŷ ���ƿ�.!" )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUILD_SKILL_12;
			SendPacket( EGS_INIT_GUILD_SKILL_ACK, kAck );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	KELG_INIT_GUILD_SKILL_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = GetGuildUID();
	kPacketToLg.m_iUnitUID = GetCharUID();
	kPacketToLg.m_iItemUID = kPacket_.m_iItemUID;
	SendToLoginServer( ELG_INIT_GUILD_SKILL_REQ, kPacketToLg );
}

_IMPL_ON_FUNC( ELG_INIT_GUILD_SKILL_ACK, KEGS_INIT_GUILD_SKILL_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INIT_GUILD_SKILL_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	SendPacket( EGS_INIT_GUILD_SKILL_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ELG_INIT_GUILD_SKILL_DEL_ITEM_REQ, KDBE_INIT_GUILD_SKILL_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ��� ��ų �ʱ�ȭ �������� ��������!
	if( m_kInventory.InitGuildSkillItem( kPacket_.m_iItemUID, kPacket_.m_vecUpdatedInventorySlot ) == false )
	{
		KEGS_INIT_GUILD_SKILL_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_GUILD_SKILL_12;
		SendPacket( EGS_INIT_GUILD_SKILL_ACK, kPacketAck );
		return;
	}

	// ���� �����ϸ� DB�ΰ��� ������ ������������ ������Ʈ�ϰ� ��� ��ų �ʱ�ȭ ������ ������Ʈ����!
	m_kInventory.FlushQuantityChange( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );

	SendToGameDB( DBE_INIT_GUILD_SKILL_REQ, kPacket_ );
}

IMPL_ON_FUNC( DBE_INIT_GUILD_SKILL_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	}

	KEGS_INIT_GUILD_SKILL_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_INIT_GUILD_SKILL_ACK, kPacket );
}

_IMPL_ON_FUNC( ELG_INIT_GUILD_SKILL_NOT, KEGS_INIT_GUILD_SKILL_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ���� ��� �Ŵ����� ����
	m_kUserGuildManager.InitGuildSkill();

	// Ŭ���̾�Ʈ�� ��� ����
	SendPacket( EGS_INIT_GUILD_SKILL_NOT, kPacket_ );

	// ���࿡ ����̶�� ����� �ٸ� �����鿡�� �ش� ������ ��ε�ĳ���� ����!
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			{
				KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
				GetRoomUserInfo( kPacketNot );
				SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
			}
			break;
		}
	}
}

_IMPL_ON_FUNC( ELG_EXPIRE_GUILD_CASH_SKILL_POINT_NOT, KEGS_EXPIRE_GUILD_CASH_SKILL_POINT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ���� ��� �Ŵ����� ����
	m_kUserGuildManager.ExpireGuildSkill( kPacket_ );

	// Ŭ���̾�Ʈ�� ��� ����
	SendPacket( EGS_EXPIRE_GUILD_CASH_SKILL_POINT_NOT, kPacket_ );

	// ���࿡ ����̶�� ����� �ٸ� �����鿡�� �ش� ������ ��ε�ĳ���� ����!
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			{
				KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
				GetRoomUserInfo( kPacketNot );
				SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
			}
			break;
		}
	}
}

IMPL_ON_FUNC( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacket;
	kPacket.m_iUnitUID = kPacket_.m_nUnitUID;
	kPacket.m_ucMemberShipGrade = kPacket_.m_ucMemberShipGrade;
	kPacket.m_kGameStat = kPacket_.m_kGameStat;
	kPacket.m_UnitSkillData = kPacket_.m_UnitSkillData;
	SendPacket( EGS_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacket );
}

#endif GUILD_SKILL_TEST

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 12. 31  ������	��� ����
#ifdef SERV_GUILD_AD

IMPL_ON_FUNC( EGS_GET_GUILD_AD_LIST_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KELG_GET_GUILD_AD_LIST_REQ kPacket;
	kPacket.m_iGuildUID = GetGuildUID();
	kPacket.m_iUnitUID = GetCharUID();
	kPacket.m_cSortType = kPacket_.m_cSortType;
	kPacket.m_uiViewPage = kPacket_.m_uiViewPage;    
	SendToLoginServer( ELG_GET_GUILD_AD_LIST_REQ, kPacket );
}

_IMPL_ON_FUNC( ELG_GET_GUILD_AD_LIST_ACK, KEGS_GET_GUILD_AD_LIST_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	SendPacket( EGS_GET_GUILD_AD_LIST_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_GET_GUILD_SKILL_IN_BOARD_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	// ü��ID �˻�
	if( IsGuestUser() )
	{
		KEGS_GET_GUILD_SKILL_IN_BOARD_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_GET_GUILD_SKILL_IN_BOARD_ACK, kPacketAck );
		return;
	}

	KELG_GET_GUILD_SKILL_IN_BOARD_REQ kPacket;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacket.m_iUnitUID = GetCharUID();
	SendToLoginServer( ELG_GET_GUILD_SKILL_IN_BOARD_REQ, kPacket );
}

_IMPL_ON_FUNC( ELG_GET_GUILD_SKILL_IN_BOARD_ACK, KEGS_GET_GUILD_SKILL_IN_BOARD_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	SendPacket( EGS_GET_GUILD_SKILL_IN_BOARD_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_REGISTRATION_GUILD_AD_REQ )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_REGISTRATION_GUILD_AD_REQ, EGS_REGISTRATION_GUILD_AD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_REGISTRATION_GUILD_AD_ACK kPacket;

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_CHAT, kPacket_.m_wstrAdMessage ) == false )
	{
		kPacket.m_iOK = NetError::ERR_STRING_FILTER_04;
		SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}
#endif //SERV_STRING_FILTER_USING_DB

	// ü��ID �˻�
	if( IsGuestUser() )
	{
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}

	// ���ڿ� �˻�
	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrAdMessage ) )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"������ �ƴѵ� ��� ���� ��û�Ϸ��� �մϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}

	if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER  &&
		m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_SYSOP )
	{
		START_LOG( cerr, L"��� ���� ����� �� ���� ��������Դϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOGc( m_kUserGuildManager.GetMemberShipGrade() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_00;
		SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}

	// ���� ���ڿ� ���� �˻�
	std::string strAdMessage = KncUtil::toNarrowString( kPacket_.m_wstrAdMessage );
	if( strAdMessage.size() > 80 )
	{
		START_LOG( cerr, L"���� ���� ���̰� �ʹ� ��ϴ�. Ŭ�󿡼� üũ�����ٵ�.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( kPacket_.m_wstrAdMessage )
			<< BUILD_LOG( strAdMessage.size() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_02;
		SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}

	// ��û ������ �Ⱓ���� �˻� ( ���� ��� �Ⱓ 1�� ~ 5�� ���� )
	if( kPacket_.m_sPeriod < 0  ||  kPacket_.m_sPeriod > 5 )
	{
		START_LOG( cerr, L"��û ������ ���� ��� �Ⱓ�� �ƴմϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( kPacket_.m_sPeriod )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_03;
		SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}

	// �����ᰡ ����� �ִ��� �˻�
	const int iCost = kPacket_.m_sPeriod * 3000; // �Ϸ翡 3000ED
	if( iCost > GetED() )
	{
		START_LOG( clog, L"�����ᰡ �����ؼ� ���� ����� �� �� �����ϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( kPacket_.m_sPeriod )
			<< BUILD_LOG( iCost )
			<< BUILD_LOG( GetED() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_04;
		SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}

	//{{ 2012. 10. 02	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	DecreaseED( iCost, KUserEDManager::ER_OUT_ED_REG_GUILD_AD_COMMISSION );
#else
	const int iBeforeED = m_iED;

	m_iED -= iCost;

	//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
	// ED �����
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_GUILD_AD_COST, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
	//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}	
	// ���� ���
	//m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_MManufactureED, iED );

	// �α��μ����� ���� ��� ���� ��� ��û
	KELG_REGISTRATION_GUILD_AD_REQ kPacetToLg;
	kPacetToLg.m_iUnitUID = GetCharUID();
	kPacetToLg.m_iGuildUID = GetGuildUID();
	kPacetToLg.m_sPeriod = kPacket_.m_sPeriod;
	kPacetToLg.m_wstrAdMessage = kPacket_.m_wstrAdMessage;
	kPacetToLg.m_iCost = iCost;
	SendToLoginServer( ELG_REGISTRATION_GUILD_AD_REQ, kPacetToLg );
}

IMPL_ON_FUNC( ELG_REGISTRATION_GUILD_AD_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_REGISTRATION_GUILD_AD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}	

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		//{{ 2012. 10. 02	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		// ������ �ѹ��̹Ƿ� �������Ѿ��Ѵ�.
		IncreaseED( kPacket_.m_iCost, KUserEDManager::ER_IN_ED_REG_GUILD_AD_COMMISSION_ROLLBACK );
#else
		const int iBeforeED = m_iED;

		m_iED += kPacket_.m_iCost;

		//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
		// ED �����
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_GUILD_AD_COST_ROLLBACK, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
		//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}

		// ���� ���
		//m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_MManufactureED, iED );
		//}}

	}

	KEGS_REGISTRATION_GUILD_AD_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iED = GetED();
	kPacket.m_sPeriod = kPacket_.m_sPeriod;
	kPacket.m_wstrAdMessage = kPacket_.m_wstrAdMessage;
	SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_MODIFY_REG_GUILD_AD_REQ )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_MODIFY_REG_GUILD_AD_REQ, EGS_MODIFY_REG_GUILD_AD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_MODIFY_REG_GUILD_AD_ACK kPacket;

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_CHAT, kPacket_.m_wstrAdMessage ) == false )
	{
		kPacket.m_iOK = NetError::ERR_STRING_FILTER_04;
		SendPacket( EGS_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}
#endif //SERV_STRING_FILTER_USING_DB

	// ���ڿ� �˻�
	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrAdMessage ) )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"������ �ƴѵ� ��� ���� ��û�Ϸ��� �մϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}

	if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER  &&
		m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_SYSOP )
	{
		START_LOG( cerr, L"��� ���� ����� �� ���� ��������Դϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOGc( m_kUserGuildManager.GetMemberShipGrade() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_00;
		SendPacket( EGS_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}

	// ���� ���ڿ� ���� �˻�
	std::string strAdMessage = KncUtil::toNarrowString( kPacket_.m_wstrAdMessage );
	if( strAdMessage.size() > 80 )
	{
		START_LOG( cerr, L"���� ���� ���̰� �ʹ� ��ϴ�. Ŭ�󿡼� üũ�����ٵ�.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( kPacket_.m_wstrAdMessage )
			<< BUILD_LOG( strAdMessage.size() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_02;
		SendPacket( EGS_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}

	// ��û ������ �Ⱓ���� �˻� ( ���� ��� �Ⱓ 1�� ~ 5�� ���� )
	if( kPacket_.m_sPeriod < 0  ||  kPacket_.m_sPeriod > 5 )
	{
		START_LOG( cerr, L"��û ������ ���� ��� �Ⱓ�� �ƴմϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( kPacket_.m_sPeriod )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_03;
		SendPacket( EGS_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}

	// �����ᰡ ����� �ִ��� �˻�
	const int iCost = kPacket_.m_sPeriod * 3000; // �Ϸ翡 3000ED
	if( iCost > GetED() )
	{
		START_LOG( clog, L"�����ᰡ �����ؼ� ���� ����� �� �� �����ϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( kPacket_.m_sPeriod )
			<< BUILD_LOG( iCost )
			<< BUILD_LOG( GetED() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_04;
		SendPacket( EGS_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}

	//{{ 2012. 10. 02	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	DecreaseED( iCost, KUserEDManager::ER_OUT_ED_MODIFY_GUILD_AD_COMMISSION );
#else
	const int iBeforeED = m_iED;

	m_iED -= iCost;

	//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
	// ED �����
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_GUILD_AD_COST, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
	//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}	
	// ���� ���
	//m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_MManufactureED, iED );

	// �α��μ����� ���� ��� ���� ��� ��û
	KELG_MODIFY_REG_GUILD_AD_REQ kPacetToLg;
	kPacetToLg.m_iUnitUID = GetCharUID();
	kPacetToLg.m_iGuildUID = GetGuildUID();
	kPacetToLg.m_sPeriod = kPacket_.m_sPeriod;
	kPacetToLg.m_wstrAdMessage = kPacket_.m_wstrAdMessage;
	kPacetToLg.m_iCost = iCost;
	SendToLoginServer( ELG_MODIFY_REG_GUILD_AD_REQ, kPacetToLg );
}

IMPL_ON_FUNC( ELG_MODIFY_REG_GUILD_AD_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_MODIFY_REG_GUILD_AD_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		//{{ 2012. 10. 02	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		// ������ ��� �ѹ��̹Ƿ� ���� ���Ѿ��Ѵ�.
		IncreaseED( kPacket_.m_iCost, KUserEDManager::ER_IN_ED_MODIFY_GUILD_AD_COMMISSION_ROLLBACK );
#else
		const int iBeforeED = m_iED;

		m_iED += kPacket_.m_iCost;

		//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
		// ED �����
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_GUILD_AD_COST_ROLLBACK, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
		//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}
		// ���� ���
		//m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_MManufactureED, iED );
		// ED��� ������
	}

	KEGS_MODIFY_REG_GUILD_AD_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iED = GetED();
	kPacket.m_sPeriod = kPacket_.m_sPeriod;
	kPacket.m_wstrAdMessage = kPacket_.m_wstrAdMessage;
	SendPacket( EGS_MODIFY_REG_GUILD_AD_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_APPLY_JOIN_GUILD_REQ )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_APPLY_JOIN_GUILD_REQ, EGS_APPLY_JOIN_GUILD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_APPLY_JOIN_GUILD_ACK kPacket;

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_CHAT, kPacket_.m_wstrMessage ) == false )
	{
		kPacket.m_iOK = NetError::ERR_STRING_FILTER_04;
		SendPacket( EGS_APPLY_JOIN_GUILD_ACK, kPacket );
		return;
	}
#endif //SERV_STRING_FILTER_USING_DB

	// ü��ID �˻�
	if( IsGuestUser() )
	{
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_APPLY_JOIN_GUILD_ACK, kPacket );
		return;
	}

	// ���ڿ� �˻�
	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrMessage ) )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_APPLY_JOIN_GUILD_ACK, kPacket );
		return;
	}

	// ���� �޽��� ���� �˻�
	std::string strMessage = KncUtil::toNarrowString( kPacket_.m_wstrMessage );
	if( strMessage.size() > 80 )
	{
		START_LOG( cerr, L"���� �޽��� ���� ���̰� �ʹ� ��ϴ�. Ŭ�󿡼� üũ�����ٵ�.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( kPacket_.m_wstrMessage )
			<< BUILD_LOG( strMessage.size() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_02;
		SendPacket( EGS_APPLY_JOIN_GUILD_ACK, kPacket );
		return;
	}

	// �̹� ��忡 ���Ե� �������� üũ	
	if( GetGuildUID() > 0 )
	{
		START_LOG( cerr, L"�̹� ��忡 ���Ե� �����ε� ��� ���� ��û�� �Ϸ��� ����. Ŭ�󿡼� �˻� �����ٵ�.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_15;
		SendPacket( EGS_APPLY_JOIN_GUILD_ACK, kPacket );
		return;
	}

	// ��� ���� ��û �ź� üũ �ߴ��� �˻�
	if( m_kUserGameOption.IsDenyInviteGuild() == true )
	{
		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_15;
		SendPacket( EGS_APPLY_JOIN_GUILD_ACK, kPacket );
		return;
	}

	KELG_APPLY_JOIN_GUILD_REQ kPacketToLg;
	GetApplyJoinGuildInfo( kPacketToLg.m_kApplyJoinGuildInfo );
	kPacketToLg.m_kApplyJoinGuildInfo.m_iApplyJoinGuildUID = kPacket_.m_iGuildUID;
	kPacketToLg.m_kApplyJoinGuildInfo.m_wstrMessage = kPacket_.m_wstrMessage;
	kPacketToLg.m_bOldApplyDelete = kPacket_.m_bOldApplyDelete;
	SendToLoginServer( ELG_APPLY_JOIN_GUILD_REQ, kPacketToLg );
}

_IMPL_ON_FUNC( ELG_APPLY_JOIN_GUILD_ACK, KEGS_APPLY_JOIN_GUILD_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_APPLY_JOIN_GUILD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}	

	SendPacket( EGS_APPLY_JOIN_GUILD_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_GET_APPLY_JOIN_GUILD_LIST_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KELG_GET_APPLY_JOIN_GUILD_LIST_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = GetGuildUID();
	kPacketToLg.m_iUnitUID = GetCharUID();
	kPacketToLg.m_uiViewPage = kPacket_.m_uiViewPage;
	SendToLoginServer( ELG_GET_APPLY_JOIN_GUILD_LIST_REQ, kPacketToLg );
}

_IMPL_ON_FUNC( ELG_GET_APPLY_JOIN_GUILD_LIST_ACK, KEGS_GET_APPLY_JOIN_GUILD_LIST_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	SendPacket( EGS_GET_APPLY_JOIN_GUILD_LIST_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ACCEPT_JOIN_GUILD_REQ )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_ACCEPT_JOIN_GUILD_REQ, EGS_ACCEPT_JOIN_GUILD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_ACCEPT_JOIN_GUILD_ACK kPacket;

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"������ �ƴѵ� ��� ���� ������ �Ϸ��� �Ѵ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_ACCEPT_JOIN_GUILD_ACK, kPacket );
		return;
	}

	if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER  &&
		m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_SYSOP )
	{
		START_LOG( cerr, L"��� ���� ������ �� �� ���� ��������Դϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOGc( m_kUserGuildManager.GetMemberShipGrade() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_00;
		SendPacket( EGS_ACCEPT_JOIN_GUILD_ACK, kPacket );
		return;
	}

	KELG_ACCEPT_JOIN_GUILD_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = GetGuildUID();
	kPacketToLg.m_iUnitUID = GetCharUID();
	kPacketToLg.m_iApplyJoinUnitUID = kPacket_.m_iUnitUID;
	SendToLoginServer( ELG_ACCEPT_JOIN_GUILD_REQ, kPacketToLg );
}

_IMPL_ON_FUNC( ELG_ACCEPT_JOIN_GUILD_ACK, KEGS_ACCEPT_JOIN_GUILD_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_ACCEPT_JOIN_GUILD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	SendPacket( EGS_ACCEPT_JOIN_GUILD_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_DELETE_APPLY_JOIN_GUILD_REQ )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_DELETE_APPLY_JOIN_GUILD_REQ, EGS_DELETE_APPLY_JOIN_GUILD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_DELETE_APPLY_JOIN_GUILD_ACK kPacket;

	if( GetGuildUID() > 0 )
	{
		if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER  &&
			m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_SYSOP )
		{
			START_LOG( cwarn, L"��� ���� ��û ������ �� �� ���� ��������Դϴ�." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetGuildUID() )
				<< BUILD_LOGc( m_kUserGuildManager.GetMemberShipGrade() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_GUILD_BOARD_00;
			SendPacket( EGS_DELETE_APPLY_JOIN_GUILD_ACK, kPacket );
			return;
		}
	}

	KELG_DELETE_APPLY_JOIN_GUILD_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = GetGuildUID();
	kPacketToLg.m_iUnitUID = GetCharUID();
	kPacketToLg.m_iDeletedUnitUID = kPacket_.m_iUnitUID;
	kPacketToLg.m_bDeleteChar = false;
	SendToLoginServer( ELG_DELETE_APPLY_JOIN_GUILD_REQ, kPacketToLg );
}

_IMPL_ON_FUNC( ELG_DELETE_APPLY_JOIN_GUILD_ACK, KEGS_DELETE_APPLY_JOIN_GUILD_ACK )
{
	//{{ 2010. 10. 07	������	�ߺ� ��Ŷ ���� �߰�
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_DELETE_APPLY_JOIN_GUILD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	SendPacket( EGS_DELETE_APPLY_JOIN_GUILD_ACK, kPacket_ );
}

#endif SERV_GUILD_AD
//}}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


