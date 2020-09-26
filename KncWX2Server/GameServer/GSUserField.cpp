#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
//{{ 2009. 4. 22  ������	������������Ʈ
#include "PVPLobbyManager.h"
//}}

//{{ 2011. 12. 05	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewPartyListManager.h"
#else
	#include "PartyListManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
#include "UserManager.h"
#endif SERV_ED_MONITORING_IN_GAME
//}}
//{{ 2012. 03. 05	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLBuffManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#ifdef SERV_CODE_EVENT
#include "GameEvent/GameEventManager.h"
#endif SERV_CODE_EVENT

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp ���� ������ �˴ϴ�!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

//{{ 2008. 11. 4  ������	���� ����
IMPL_ON_FUNC( EGS_STATE_CHANGE_FIELD_REQ )
{
#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-06	// �ڼ���
	SetLanBugOutDisconnectCheck( false );
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
//#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 3, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_STATE_CHANGE_FIELD_REQ, EGS_STATE_CHANGE_FIELD_ACK );
//#else
//	VERIFY_STATE( ( 3, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
//#endif SERV_REPEAT_FILTER_REFAC
	//}}	

	KEGS_STATE_CHANGE_FIELD_ACK kPacketAck;

#ifdef SERV_ALTERA_AUTO_OPEN_HARD_CODE
	IF_EVENT_ENABLED( CEI_ALTERA_AUTO_OPEN_HARD_CODE )
	{
		// 2013.03.14 lygan_������ // �ڵ� ��� �ִ°� �½��ϴ�.
	}
	ELSE
	{
		if( kPacket_.m_iMapID == SEnum::VMI_ALTERA || kPacket_.m_iMapID == SEnum::VMI_BATTLE_FIELD_ALTERA_REST_00 )
		{
			kPacket_.m_iMapID = SEnum::VMI_RUBEN;
		}
	}
#endif //SERV_ALTERA_AUTO_OPEN_HARD_CODE
#ifdef SERV_PEITA_AUTO_OPEN_HARD_CODE
	IF_EVENT_ENABLED( CEI_PEITA_AUTO_OPEN_HARD_CODE )
	{
		// 2013.03.14 lygan_������ // �ڵ� ��� �ִ°� �½��ϴ�.
	}
	ELSE
	{
		if( kPacket_.m_iMapID == SEnum::VMI_PEITA || kPacket_.m_iMapID == SEnum::VMI_BATTLE_FIELD_PEITA_REST_00 )
		{
			kPacket_.m_iMapID = SEnum::VMI_RUBEN;
		}
	}
#endif //SERV_PEITA_AUTO_OPEN_HARD_CODE
#ifdef SERV_VELDER_AUTO_OPEN_HARD_CODE
	IF_EVENT_ENABLED( CEI_VELDER_AUTO_OPEN_HARD_CODE )
	{
		// 2013.03.14 lygan_������ // �ڵ� ��� �ִ°� �½��ϴ�.
	}
	ELSE
	{
		if( kPacket_.m_iMapID == SEnum::VMI_VELDER || kPacket_.m_iMapID == SEnum::VMI_BATTLE_FIELD_VELDER_REST_00 )
		{
			kPacket_.m_iMapID = SEnum::VMI_RUBEN;
		}
	}
#endif //SERV_VELDER_AUTO_OPEN_HARD_CODE
#ifdef SERV_HAMEL_AUTO_OPEN_HARD_CODE
	IF_EVENT_ENABLED( CEI_HAMEL_AUTO_OPEN_HARD_CODE )
	{
		// 2013.03.14 lygan_������ // �ڵ� ��� �ִ°� �½��ϴ�.
	}
	ELSE
	{
		if( kPacket_.m_iMapID == SEnum::VMI_HAMEL || kPacket_.m_iMapID == SEnum::VMI_BATTLE_FIELD_HAMEL_REST_00 )
		{
			kPacket_.m_iMapID = SEnum::VMI_RUBEN;
		}
	}
#endif //SERV_HAMEL_AUTO_OPEN_HARD_CODE
#ifdef SERV_SANDER_AUTO_OPEN_HARD_CODE
	IF_EVENT_ENABLED( CEI_SANDER_AUTO_OPEN_HARD_CODE )
	{
		// 2013.03.14 lygan_������ // �ڵ� ��� �ִ°� �½��ϴ�.
	}
	ELSE
	{
		if( kPacket_.m_iMapID == SEnum::VMI_SANDER || kPacket_.m_iMapID == SEnum::VMI_BATTLE_FIELD_SANDER_REST_00 )
		{
			kPacket_.m_iMapID = SEnum::VMI_RUBEN;
		}
	}
#endif //SERV_SANDER_AUTO_OPEN_HARD_CODE

#ifdef NO_SANDER_VILLIAGE
	if( kPacket_.m_iMapID == SEnum::VMI_SANDER || kPacket_.m_iMapID == SEnum::VMI_BATTLE_FIELD_SANDER_REST_00 )
	{
		kPacket_.m_iMapID = SEnum::VMI_RUBEN;
	}
#endif // NO_SANDER_VILLIAGE

#ifdef SERV_NO_VELDER_VILLIAGE
	if( kPacket_.m_iMapID == SEnum::VMI_VELDER || kPacket_.m_iMapID == SEnum::VMI_BATTLE_FIELD_VELDER_REST_00 )
	{
		kPacket_.m_iMapID = SEnum::VMI_RUBEN;
	}
#endif //SERV_NO_VELDER_VILLIAGE

#ifdef SERV_NO_HAMEL_VILLIAGE
	if( kPacket_.m_iMapID == SEnum::VMI_HAMEL || kPacket_.m_iMapID == SEnum::VMI_BATTLE_FIELD_HAMEL_REST_00 )
	{
		kPacket_.m_iMapID = SEnum::VMI_RUBEN;
	}
#endif //SERV_NO_HAMEL_VILLIAGE



	//{{ 2012. 09. 21   ��μ�   ���� ���� ���� �޽��� ����
#ifdef SERV_ENTER_VILLAGE_ERROR
	if( CheckEnterTheVillage( kPacket_.m_iMapID, kPacketAck.m_iLimitLevel, kPacketAck.m_iRequireDungeonID ) == false )
	{
		kPacketAck.m_iOK = NetError::GetLastError();

		START_LOG( cwarn, L"������ ������ �� �ִ� ������ �ȵ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iMapID )
			<< BUILD_LOG( kPacketAck.m_iOK )
			<< BUILD_LOG( kPacketAck.m_iLimitLevel )
			<< BUILD_LOG( kPacketAck.m_iRequireDungeonID )
			<< END_LOG;
		SendPacket( EGS_STATE_CHANGE_FIELD_ACK, kPacketAck );
		return;
	}
#else
	// �ش� map�� �� �� �ִ��� ���� üũ
	if( CheckEnterTheVillage( kPacket_.m_iMapID ) == false )
	{
		START_LOG( cwarn, L"������ ������ �� �ִ� ������ �ȵ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iMapID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_STATE_CHANGE_FIELD_ACK, kPacketAck );
		return;
	}	
#endif SERV_ENTER_VILLAGE_ERROR
	//}}

	if( GetFieldUID() > 0 )
	{
		//  �ش� �ʵ忡�� ������~!
		KERM_LEAVE_FIELD_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_FIELD_REASON_00;
		kPacket.m_iMapID  = kPacket_.m_iMapID;

		SendToGSField( ERM_LEAVE_FIELD_REQ, kPacket );
	}
	else
	{
		if( !SiCXSLMapData()->VerifyMapID( kPacket_.m_iMapID ) )
		{
			START_LOG( cerr, L"�ʵ忡 �����Ϸ��µ� MapID�� �̻��մϴ�." )
				<< BUILD_LOG( kPacket_.m_iMapID )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_FIELD_00; // map������ �̻��մϴ�.
			SendPacket( EGS_STATE_CHANGE_FIELD_ACK, kPacketAck );
			return;
		}

		// MapID ����
		//{{ 2012. 02. 02	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		SetMapID( kPacket_.m_iMapID );
#else
		m_nMapID = kPacket_.m_iMapID;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		kPacketAck.m_iOK = NetError::NET_OK;
		SendPacket( EGS_STATE_CHANGE_FIELD_ACK, kPacketAck );

		//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
		KEGS_UPDATE_BUFF_INFO_NOT kPacket;
		m_kUserBuffManager.GetBuffList( kPacket.m_vecActivateBuffList );

		if( ( kPacket.m_vecActivateBuffList.empty() == false ) )
		{
			SendPacket( EGS_UPDATE_BUFF_INFO_NOT, kPacket );
		}
#endif SERV_SERVER_BUFF_SYSTEM
		//}
	}

	//////////////////////////////////////////////////////////////////////////
	// �ʵ�� �̵��Ҷ� ó���ؾ��ϴ°�..

	// PVP�κ� ��Ż ó��
	SendLeavePVPLobby();

	//{{ 2011. 07. 11	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
//	SendLeavePVPMatch(); ���⿡���� ���� ȣ���� �ʿ�� ������..?
//#endif SERV_PVP_NEW_SYSTEM
	//}}

	// ���ΰŷ� ��Ż ó��
	if( GetTradeUID() != 0 )
	{
		KEGS_BREAK_TRADE_REQ kPacket;
		kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_01;
		_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );

		m_iTradeUID = 0;
	}

	// ���λ��� ��Ż ó��
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

	// ����, ����, �Ʒü�, �ŷ����� ��Ż ó��
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_SQUARE:
			{
				KEGS_LEAVE_SQUARE_REQ kPacket;
				kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_01;
				SendToCnRoom( ERM_LEAVE_SQUARE_REQ, kPacket );
			}
			break;

		case CXSLRoom::RT_TRAININGCENTER:
			{
				KERM_LEAVE_ROOM_REQ kPacket;
				kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_01;
				SendToCnRoom( ERM_LEAVE_TC_ROOM_REQ, kPacket );
			}
			break;

			//{{ 2012. 10. 18	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
			//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
		case CXSLRoom::RT_WEDDING_HALL:
#endif SERV_RELATIONSHIP_SYSTEM
			//}
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				KERM_LEAVE_ROOM_REQ kPacket;
				if( kPacket_.m_bUseWarpItem )
				{
					kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_30;
				}
				else
				{
					kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_27;
				}
				SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacket );
			}
			break;
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}

		default:
			{
				KERM_LEAVE_ROOM_REQ kPacket;
				kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_01;
				//{{ 2012. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
				SendToCnRoom( ERM_LEAVE_ROOM_FOR_ENTER_THE_VILLAGE_REQ, kPacket );
#else
				SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacket );
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
			}
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
#ifdef SERV_EPIC_QUEST
	m_kUserQuestManager.CheckEpicQuest_SuccessStateChange( kPacket_.m_iMapID, GetThisPtr<KGSUser>() );
#endif SERV_EPIC_QUEST
}

IMPL_ON_FUNC( ERM_LEAVE_FIELD_ACK )
{
	// ���´� ��� �ǳ�..?
	VERIFY_STATE( ( 3, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iReason == NetError::NOT_LEAVE_FIELD_REASON_00 )
	{
		KEGS_STATE_CHANGE_FIELD_ACK kPacketAck;

		if( kPacket_.m_iOK == NetError::NET_OK )
		{
			//{{ 2012. 02. 02	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
			SetMapID( kPacket_.m_iMapID );
#else
			m_nMapID = kPacket_.m_iMapID;
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}

			SetFieldUID( 0 ); // �ʵ忡�� ������ ���������Ƿ� 0����..

			// �̵��� �ʵ��� FieldUId�� ��´�.			
			if( !SiCXSLMapData()->VerifyMapID( kPacket_.m_iMapID ) )
			{
				START_LOG( cerr, L"�ʵ忡 �����Ϸ��µ� MapID�� �̻��մϴ�." )
					<< BUILD_LOG( kPacket_.m_iMapID )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_FIELD_00; // map������ �̻��մϴ�.
				SendPacket( EGS_STATE_CHANGE_FIELD_ACK, kPacketAck );
				return;
			}
		}

		// Ŭ���̾�Ʈ��..
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_STATE_CHANGE_FIELD_ACK, kPacketAck );
	}
	else
	{
		if( kPacket_.m_iOK == NetError::NET_OK )
		{
			SetFieldUID( 0 ); // �ʵ忡�� ������ ���������Ƿ� 0����..
		}
		else
		{
			START_LOG( cerr, L"�ʵ� ��Ż ����!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;
		}
	}
}

IMPL_ON_FUNC( EGS_FIELD_LOADING_COMPLETE_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP ), EGS_FIELD_LOADING_COMPLETE_REQ, EGS_FIELD_LOADING_COMPLETE_ACK );
#else
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP ), EGS_FIELD_LOADING_COMPLETE_ACK );
#endif SERV_REPEAT_FILTER_REFAC
	//}}	

	// �ش� �ʵ��� ���� ���� ���δ� EGS_STATE_CHANGE_FIELD_REQ���� üũ����!

	KEGS_FIELD_LOADING_COMPLETE_ACK kPacketAck;

	if( GetCharUID() == 0 )
	{
		START_LOG( cerr, L"ĳ���� ������ ��ȿ���� �ʴµ� ���� ���� �Ϸ� �Ѵ�?!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_ucLastTouchLineIndex )
			<< BUILD_LOG( kPacket_.m_usLastPosValue )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_FIELD_00;
		SendPacket( EGS_FIELD_LOADING_COMPLETE_ACK, kPacketAck );
		return;
	}

	if( GetFieldUID() > 0 )
	{
		START_LOG( cerr, L"�ʵ忡 ����˻絵 ���ߴµ� �� ��Ŷ�� ����?" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_FIELD_00;
		SendPacket( EGS_FIELD_LOADING_COMPLETE_ACK, kPacketAck );
		return;
	}
	else
	{
		KERM_JOIN_FIELD_REQ kPacket;

		// FieldUId�� ��´�.
		if( !SiCXSLMapData()->GetFieldUID( GetMapID(), kPacket.m_iFieldUID ) )
		{
			START_LOG( cerr, L"�߸��� �� ����" )
				<< BUILD_LOG( GetMapID() )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_FIELD_00; // map������ �̻��մϴ�.
			SendPacket( EGS_FIELD_LOADING_COMPLETE_ACK, kPacketAck );
			return;
		}

		// ���� �ʵ忡 ���������� �ʴ�!		
		GetFieldUserInfo( kPacket.m_kFieldUserInfo );
		kPacket.m_kFieldUserInfo.m_vPos = kPacket_.m_vPos;
		kPacket.m_kFieldOption		    = m_kFieldOption;
		if( GetPartyUID() > 0 )
		{
			//{{ 2010. 03. 02  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
			KEPM_REQUEST_UPDATE_PARTY_MEMBER_LIST_NOT kPacketToPM;
			kPacketToPM.m_iPartyUID = GetPartyUID();
			SendToCnParty( EPM_REQUEST_UPDATE_PARTY_MEMBER_LIST_NOT, kPacketToPM );
#else
			if( SiKPartyManager()->GetUserUIDList( GetPartyUID(), kPacket.m_setPartyUIDList ) == false )
			{
				START_LOG( cerr, L"PartyUID�� �ִµ� ��Ƽ����Ʈ ������ ���� �� ����?" )
					<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetPartyUID() )
					<< END_LOG;
			}
#endif SERV_CHANNEL_PARTY
			//}}			
		}

		// warning!! join_field �ϱ� ���� ���� Ŭ���̾�Ʈ�� ���� ���� ��ġ ������ �����Ѵ�... 
		// join field �����ϰ� unit sync ��Ŷ�� �����ϱ� ���� Ŭ���̾�Ʈ �����ϸ� ��ġ������ ���� ���� �� �ִ�.
		//{{ 2012. 02. 02	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_kLastPos.m_ucLastTouchLineIndex	= kPacket_.m_ucLastTouchLineIndex;
		m_kLastPos.m_usLastPosValue			= kPacket_.m_usLastPosValue;
#else
		m_ucLastTouchLineIndex	= kPacket_.m_ucLastTouchLineIndex;
		m_usLastPosValue		= kPacket_.m_usLastPosValue;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		//{{ 2009. 4. 9  ������		ģ������Ʈ
		std::map< std::wstring, UidType >::const_iterator mitF;
		for( mitF = m_mapFriendList.begin(); mitF != m_mapFriendList.end(); ++mitF )
		{
			kPacket.m_setFriendUIDList.insert( mitF->second );
		}
		//}}

		// ������Ŷ ���������� �̸� FieldUID�� �����Ѵ�.
		SetFieldUID( kPacket.m_iFieldUID );
		LIF( GetCharUID() != 0 );
		_SendToGSField( GetFieldUID(), ERM_JOIN_FIELD_REQ, kPacket );
	}
}

IMPL_ON_FUNC( ERM_JOIN_FIELD_ACK )
{
	VERIFY_STATE( ( 3, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	// ���´� ��� �ǳ�?

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		LIF( GetFieldUID() == kPacket_.m_iFieldUID );

		// �ʵ� ���� ����
		SetFieldUID( kPacket_.m_iFieldUID );

		if( GetStateID() != KGSFSM::S_FIELD_MAP )
		{
			StateTransition( KGSFSM::I_TO_FIELD_MAP );
		}

		//{{ 2012. 04. 17	������	DB���� ���� üũ
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		m_kUserQuestManager.Handler_OnEnterTheVillage( GetThisPtr<KGSUser>() );
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}

		//��Ƽ�� ������� �ʵ��̵� ������ ��Ƽ�� ������Ʈ ��.
		SendUpdatePartyUnitInfo( true );

		//{{ 2009. 10. 8  ������	���
#ifdef GUILD_TEST
		SendUpdateGuildMemberInfo();
#endif GUILD_TEST
		//}}

		//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
		// ������ ���� �ູ
		std::vector< KBuffInfo > vecActiveBuff;
		ActivateBuff( CXSLBuffManager::BTI_BUFF_WARM_ENERGE_OF_EL, vecActiveBuff );

		if( vecActiveBuff.empty() == false )
		{
			// �α��� ���� ���� �Ŵ����� ������Ʈ
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
			kReq.m_iUnitUID = GetCharUID();
			kReq.m_vecActivateBuff = vecActiveBuff;
			SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );			
		}
#endif SERV_SERVER_BUFF_SYSTEM
		//}}

#ifdef SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
		if(m_bIdentityConfirmCheck == false)
		{
			if(m_GiantAccountType.IsType(KGiantAccountType::ADULT_PLAYER) == false)
			{
				KEGS_IDENTITY_CONFIRM_POPUP_MESSAGE_NOT kPacket;
				kPacket.m_bCheckAdult = false;
				SendPacket( EGS_IDENTITY_CONFIRM_POPUP_MESSAGE_NOT, kPacket );
			}
			m_bIdentityConfirmCheck = true;
#ifdef SERV_EPAY_SYSTEM
			KEGS_EPAY_INFO_NOT kPacket2;

			kPacket2.m_wstrLoginID = m_wstrUserID;
			kPacket2.m_usZone = m_usGiantZone;
			kPacket2.m_usGame = m_usGiantGame;
			kPacket2.m_iUnitUID = GetCharUID();
			SendPacket( EGS_EPAY_INFO_NOT, kPacket2 );
#endif //SERV_EPAY_SYSTEM
		}		
#endif //SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
	}

	KEGS_FIELD_LOADING_COMPLETE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	
	//{{ 2012. 05. 16	�ڼ���	ù ���� �� ���̵� ���� ����ֱ�
#ifdef SERV_EVENT_GUIDELINE_POPUP
	kPacket.m_bPopupTheGuideLine = m_bPopupTheGuideLine;
	m_bPopupTheGuideLine = false;
#endif SERV_EVENT_GUIDELINE_POPUP
	//}}

	SendPacket( EGS_FIELD_LOADING_COMPLETE_ACK, kPacket );
}

IMPL_ON_FUNC_NOPARAM( EGS_LEAVE_FIELD_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	// ��Ż ��Ŷ
	SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );
}

//{{ 2012. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
_IMPL_ON_FUNC( ERM_LEAVE_ROOM_FOR_ENTER_THE_VILLAGE_ACK, KEGS_LEAVE_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	// �����濡�� ������� ��Ƽ Ż��.
	const CXSLRoom::ROOM_TYPE eRoomType = CXSLRoom::GetRoomType( GetRoomUID() );

	//{{ 2012. 04. 16	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	std::vector< int > vecDeleteBuff;
	std::vector< KBuffInfo > vecActivateBuff;

	if( eRoomType == CXSLRoom::RT_DUNGEON )
	{
		//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		m_kUserBuffManager.OnLeaveDungeonRoom( GetThisPtr<KGSUser>(), vecActivateBuff, vecDeleteBuff, false );
#else
		m_kUserBuffManager.OnLeaveDungeonRoom( GetThisPtr<KGSUser>(), vecDeleteBuff );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
		//}
	}
	else if( eRoomType == CXSLRoom::RT_BATTLE_FIELD )
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

#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

//	if( eRoomType == CXSLRoom::RT_DUNGEON ) -- �̹� Leave Room Req���� ��Ƽ ��Ż�� �߱� ������ ���⼭ �ٽ� ��Ƽ ��Ż ó���� ������ �ʴ´�.
//	{
//		//{{ 2011. 02. 21  ��μ�	Ư�� ���� ���� ������ ���� ����
//#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
//		if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_22 )
//		{
//			SendLeaveParty( NetError::NOT_LEAVE_ROOM_REASON_22 );
//		}
//		else
//		{
//			SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_02 );
//		}
//#else
//		SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_02 );
//#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
//		//}}
//	}

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

	//SendPacket( EGS_LEAVE_ROOM_ACK, kPacket_ ); -- ���� ���� �߿� ���� ��Ż�Ѱ��̹Ƿ� Ŭ���̾�Ʈ�� ��Ŷ�� ������ �ʾƵ� �ȴ�.
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

IMPL_ON_FUNC( EGS_FIELD_UNIT_SYNC_DATA_NOT )
{
	//////////////////////////////////////////////////////////////////////////	
	// �ʵ� ��ũ ��Ŷ ���� üũ - ���¿����� ���ϱ����� ���°˻纸�� ������ üũ�մϴ�.
	if( m_kTimer[TM_FIELD_SYNC_GAP].elapsed() < 1.5 )
	{
		START_LOG( cwarn, L"�ʵ� ��ũ ������ 1.5�� �̳��Դϴ�!" )
			<< BUILD_LOG( GetCharUID() );
		return;
	}

	// ��Ŷ ��� ��Ű�� �� Ÿ�̸Ӵ� �ʱ�ȭ
	m_kTimer[TM_FIELD_SYNC_GAP].restart();
	//////////////////////////////////////////////////////////////////////////	

	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );	

	// �ʵ� ��ũ
	if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		if( GetFieldUID() == 0 )
		{
			START_LOG( cwarn, L"fieldUID�� 0�ε� sync��Ŷ�� ����." )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			// [����] ����ä�� �κ� ������ �ִٸ� S_FIELD_MAP�����̸鼭 FieldUID�� 0�� �ɼ� �ִ�.
			return;
		}

		SendToGSField( ERM_FIELD_UNIT_SYNC_DATA_NOT, kPacket_ );

		//{{ 2012. 02. 02	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_kLastPos.m_ucLastTouchLineIndex	= kPacket_.m_ucLastTouchLineIndex;
		m_kLastPos.m_usLastPosValue			= kPacket_.m_usLastPosValue;
#else
		m_ucLastTouchLineIndex	= kPacket_.m_ucLastTouchLineIndex;
		m_usLastPosValue		= kPacket_.m_usLastPosValue;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		//////////////////////////////////////////////////////////////////////////
		//GetKGameServer()->AddFieldSyncCount();
		//////////////////////////////////////////////////////////////////////////
	}
	// ���÷��� �� ��ũ
	else if( GetStateID() == KGSFSM::S_ROOM )
	{
		if( GetRoomUID() == 0 )
		{
			START_LOG( cerr, L"RoomUID�� 0�ε� sync��Ŷ�� ����." )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;
			return;
		}

		//{{ 2010. 03. 17  ������	���¿�������
		CXSLRoom::ROOM_TYPE eRoomType = CXSLRoom::GetRoomType( GetRoomUID() );
		switch( eRoomType )
		{
		case CXSLRoom::RT_DUNGEON:
			{
				SendToCnRoom( ERM_FIELD_UNIT_SYNC_DATA_NOT, kPacket_ );
			}
			break;

		case CXSLRoom::RT_SQUARE:
			{
				// �ʵ忡�� ���� ����� �ð����� �߻��� �� �ִ� ������.
				START_LOG( cwarn, L"���忡 ������ �����ε� Field Sync�� ������." )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
			}
			break;

		default:
			{
				START_LOG( cerr, L"�����浵 �����̵�浵 �ƴѵ� field sync�� ������?" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( (int)eRoomType )
					<< END_LOG;
				return;
			}
			break;
		}
		//}}
	}
	else
	{
		START_LOG( cerr, L"�ʵ� ��ũ�� �������� ���°� �̻��մϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetStateID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// �κ� ��ũ��Ʈ ���� �����
#ifdef SERV_ROBOT_TEST
	//SiKRobotTestManager()->PrintRobotScript( kPacket_, GetMapID() );
#endif SERV_ROBOT_TEST
	//////////////////////////////////////////////////////////////////////////
}

_IMPL_ON_FUNC( ERM_FIELD_UNIT_SYNC_DATA_BROAD_NOT, KEGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT )
{
	VERIFY_STATE_WARN( ( 1, KGSFSM::S_ROOM ) );

	// ���� ������κ��� ���� ��ũ �̺�Ʈ
	SendPacket( EGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT, kPacket_ );
}

//{{ 2009. 1. 13  ������	�ʵ� ���� ����Ʈ
IMPL_ON_FUNC( EGS_COMMUNITY_USER_LIST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_COMMUNITY_USER_LIST_ACK kPacket;
	kPacket.m_iOK		  = NetError::NET_OK;
	kPacket.m_uiTotalPage = 1;
	kPacket.m_uiViewPage  = 1;

	if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		// �ʵ忡 �ִٸ�..
		if( GetFieldUID() > 0 )
		{
			//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
			if( GetAuthLevel() >= SEnum::UAL_GM && GetEDMonitoringMode() == true )
			{
				kPacket_.m_bMode = true;
			}
#endif SERV_ED_MONITORING_IN_GAME
			//}}
			SendToGSField( ERM_FIELD_USER_LIST_REQ, kPacket_ );
			return;
		}
		// �����κ� �ִٸ�..
		else if( GetRoomListID() > 0 )
		{
			if( !SiKPVPLobbyManager()->GetPVPUserList( kPacket_.m_uiViewPage, kPacket.m_uiTotalPage, kPacket.m_vecUserList ) )
			{
				START_LOG( cerr, L"pvp user list��� ����!" )
					<< BUILD_LOG( kPacket_.m_uiViewPage )
					<< END_LOG;
			}
			else
			{
				kPacket.m_uiViewPage = kPacket_.m_uiViewPage;
			}
			goto end_proc;
		}
		// �������?
		else
		{
			// �ð����� �߻�������.
			START_LOG( cwarn, L"��𿡼� ��û�Ѱ�?" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetFieldUID() )
				<< BUILD_LOG( GetRoomListID() )
				<< BUILD_LOG( GetStateIDString() )
				<< END_LOG;

			goto end_proc;
		}
	}
	else // KGSFSM::S_ROOM - �����
	{
		if( GetRoomUID() == 0 )
		{
			START_LOG( cerr, L"���� ������� ������ ���°� �ƴѵ� �ʵ� ���� ����Ʈ�� �䱸�Ѵ�?" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_FIELD_04;
			goto end_proc;
		}

		//{{ 2009. 3. 20  ������	������, ������ ���?
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
			{
				// �������̶��..
				if( !SiKPVPLobbyManager()->GetPVPUserList( kPacket_.m_uiViewPage, kPacket.m_uiTotalPage, kPacket.m_vecUserList ) )
				{
					START_LOG( cerr, L"pvp user list��� ����!" )
						<< BUILD_LOG( kPacket_.m_uiViewPage )
						<< END_LOG;
				}
				else
				{
					kPacket.m_uiViewPage = kPacket_.m_uiViewPage;
				}
				goto end_proc;
			}
			break;
			//{{ ��Ʋ�ʵ� Ŀ�´�Ƽ ����Ʈ Ȯ�� - ��μ�
#ifdef SERV_COMMUNITY_IN_BATTLE_FIELD
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				// ��Ʋ �ʵ��̶��..�뿡�� ����� �޾ƿ;� ��
				SendToCnRoom( ERM_COMMUNITY_IN_BATTLE_FIELD_REQ, kPacket_ );

				START_LOG( clog, L"��Ʋ �ʵ忡�� Ŀ�´�Ƽ ���� Ȯ��" )
					<< BUILD_LOG( GetName() )
					<< BUILD_LOG( GetRoomUID() )
					<< END_LOG;

				return;
			}
			break;
#endif SERV_COMMUNITY_IN_BATTLE_FIELD
			//}}
		case CXSLRoom::RT_SQUARE:
			{
				START_LOG( cwarn, L"Ŭ���̾�Ʈ���� �̰� �ȳ����� �ϴµ�?" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< END_LOG;
				goto end_proc;
			}
			break;
		}
		//}}

		//{{ 2010. 03. 02  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
		if( !SiKPartyListManager()->GetPartyUserList( GetPartyUID(), kPacket.m_vecUserList ) )
#else
		if( !SiKPartyManager()->GetPartyUserList( GetPartyUID(), kPacket.m_vecUserList ) )
#endif SERV_CHANNEL_PARTY
			//}}
		{
			START_LOG( cerr, L"��Ƽ������ �ƴѵ� ���� ������� �����ؼ� �ʵ���������Ʈ�� �䱸�Ѵ�?" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetPartyUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_FIELD_04;
			goto end_proc;
		}
	}

end_proc:
	SendPacket( EGS_COMMUNITY_USER_LIST_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_FIELD_USER_LIST_ACK, KEGS_COMMUNITY_USER_LIST_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	SendPacket( EGS_COMMUNITY_USER_LIST_ACK, kPacket_ );
}
//}}

IMPL_ON_FUNC( EGS_OPTION_UPDATE_REQ )
{
	// ���´� ��� �ǳ�?

	KEGS_OPTION_UPDATE_ACK kPacketAck;

	// �ɼ� ������Ʈ
	//if( !KFieldOption::VerifyLimitOption( kPacket_.m_kFieldOption.m_sLimitOption ) )
	//{
	//	kPacketAck.m_iOK = NetError::ERR_FIELD_02;
	//	SendPacket( EGS_OPTION_UPDATE_ACK, kPacketAck );
	//	return;
	//}

	m_kFieldOption = kPacket_.m_kFieldOption;

	kPacketAck.m_iOK = NetError::NET_OK;
	SendPacket( EGS_OPTION_UPDATE_ACK, kPacketAck );

	// �ʵ忡 ������ �ִٸ� ������Ʈ
	//if( m_iFieldUID > 0 )
	//{
	//	KERM_OPTION_UPDATE_NOT kPacket;
	//	kPacket.m_kFieldOption = kPacket_.m_kFieldOption;
	//	SendToGSField( ERM_OPTION_UPDATE_NOT, kPacket );
	//}
}
//}}

//{{ 2009. 4. 7  ������		ģ������
IMPL_ON_FUNC( EGS_UPDATE_FRIEND_INFO_REQ )
{
	// �������� �ѱ�?

	KEGS_UPDATE_FRIEND_INFO_ACK kPacket;

	if( GetCharUID() <= 0 )
	{
		START_LOG( cwarn, L"ĳ���� ���õ� �ȵƴµ� ģ��������û�� ���ƿ���?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_wstrFriendNickName )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UPDATE_FRIEND_00;
		SendPacket( EGS_UPDATE_FRIEND_INFO_ACK, kPacket );
		return;
	}

	if( kPacket_.m_bDelete )
	{
		// ģ������Ʈ���� ����
		m_mapFriendList.erase( kPacket_.m_wstrFriendNickName );

		kPacket.m_iOK = NetError::ERR_UPDATE_FRIEND_00;
		SendPacket( EGS_UPDATE_FRIEND_INFO_ACK, kPacket );

		// ���� ��ġ�� Field��� FieldUser�� ģ������Ʈ���� �ش� ģ���� ����!
		SendUpdateFriendList();
		return;
	}
	else
	{
		// ģ������Ʈ�� �̹� �ִ� ģ������ ã�ƺ���!
		std::map< std::wstring, UidType >::const_iterator mit = m_mapFriendList.find( kPacket_.m_wstrFriendNickName );
		if( mit != m_mapFriendList.end() )
		{
			kPacket.m_iOK = NetError::NET_OK;
			kPacket.m_iFriendUnitUID = mit->second;
			SendPacket( EGS_UPDATE_FRIEND_INFO_ACK, kPacket );
			return;
		}

		// ģ�� ����Ʈ�� ������ �α��� ������ ģ��ã���� ����!
		SendToLoginServer( ELG_FIND_FRIEND_UNIT_INFO_REQ, kPacket_ );
		return;
	}
}

_IMPL_ON_FUNC( ELG_FIND_FRIEND_UNIT_INFO_ACK, KEGS_UPDATE_FRIEND_INFO_ACK )
{
	// �������� �ѱ�?

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// ģ�� ����Ʈ�� �߰�
		m_mapFriendList.insert( std::make_pair( kPacket_.m_wstrFriendNickName, kPacket_.m_iFriendUnitUID ) );

		// ���� ��ġ�� Field��� FieldUser�� ģ������Ʈ�� �߰�����!
		SendUpdateFriendList();
	}

	SendPacket( EGS_UPDATE_FRIEND_INFO_ACK, kPacket_ );
}
//}}

//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
IMPL_ON_FUNC( EGS_ED_MONITORING_BLOCK_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		START_LOG( cerr, L"�����ڰ� �ƴѵ� ����.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< END_LOG;
		return;
	}

	KEGS_ED_MONITORING_BLOCK_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	KGSUserPtr spUser = GetKUserManager()->GetByCID( kPacket_.m_iUnitUID );
	if( spUser == NULL )
	{
		// �ð����� ������ �������� ���� �� �ִ�.
		START_LOG( cwarn, L"������ ����.!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_ROOM_23;
		SendPacket( EGS_ED_MONITORING_BLOCK_ACK, kAck );
		return;
	}	

	//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;

	switch( kPacket_.m_ucType )
	{
	case KEGS_ED_MONITORING_BLOCK_REQ::EMBT_SEND_MAIL:
		{
			kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_ED_MONITORING_SEND_MAIL;
			spUser->SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );

			spUser->SetHackingUserSendType( KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_ED_MONITORING_SEND_MAIL );
		}break;
	case KEGS_ED_MONITORING_BLOCK_REQ::EMBT_ACCOUNT_BLOCK:
		{
			kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_ED_MONITORING_ACCOUNT_BLOCK;
			spUser->HackUserRegRejectedUser( kPacket.m_cHackingCheckType );
			spUser->HackUserRegMonitorsUser( kPacket.m_cHackingCheckType );
			spUser->HackUserKick( kPacket );

		}break;
	case KEGS_ED_MONITORING_BLOCK_REQ::EMBT_MACHINE_ID_BLOCK:
		{
			kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_ED_MONITORING_MACHINE_ID_BLOCK;
			spUser->SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );          

			spUser->SetHackingUserSendType( KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_ED_MONITORING_MACHINE_ID_BLOCK );
		}break;
	default:
		{
			START_LOG( cerr, L"ED ���� ����͸� Ÿ���� �������� Ÿ���� �ƴմϴ�." )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOGc( kPacket_.m_ucType )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_CLIENT_STATE;
			SendPacket( EGS_ED_MONITORING_BLOCK_ACK, kAck );
			return;
		}
	}
#else
	KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;

	switch( kPacket_.m_ucType )
	{
	case KEGS_ED_MONITORING_BLOCK_REQ::EMBT_SEND_MAIL:
		{
			kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_ED_MONITORING_SEND_MAIL;
			spUser->SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
		}break;
	case KEGS_ED_MONITORING_BLOCK_REQ::EMBT_ACCOUNT_BLOCK:
		{
			kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_ED_MONITORING_ACCOUNT_BLOCK;
			spUser->HackUserRegRejectedUser( kPacket.m_cHackingCheckType );
			spUser->HackUserRegMonitorsUser( kPacket.m_cHackingCheckType );
			spUser->HackUserKick( kPacket );

		}break;
	case KEGS_ED_MONITORING_BLOCK_REQ::EMBT_MACHINE_ID_BLOCK:
		{
			kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_ED_MONITORING_MACHINE_ID_BLOCK;
			spUser->SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );            
		}break;
	default:
		{
			START_LOG( cerr, L"ED ���� ����͸� Ÿ���� �������� Ÿ���� �ƴմϴ�." )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOGc( kPacket_.m_ucType )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_CLIENT_STATE;
			SendPacket( EGS_ED_MONITORING_BLOCK_ACK, kAck );
			return;
		}
	}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	//}}

	SendPacket( EGS_ED_MONITORING_BLOCK_ACK, kAck );
}

IMPL_ON_FUNC( EGS_COMMUNITY_USER_LIST_MONITORING_MODE_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_COMMUNITY_USER_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_FIELD_04;

	// �ʵ忡 �ִٸ�..
	if( GetFieldUID() > 0 )
	{
		if( GetAuthLevel() >= SEnum::UAL_GM && GetEDMonitoringMode() == true )
		{
			// �ֽ� ������ �ʵ�� ���� ���� ��������.

			KERM_FIELD_USER_LIST_MONITORING_MODE_REQ kReq;
			kReq.m_uiViewPage = kPacket_.m_uiViewPage;

			BOOST_TEST_FOREACH( const UidType, iUnitUID, kPacket_.m_vecUnitUIDList )
			{
				KGSUserPtr spReceiver = GetKUserManager()->GetByCID( iUnitUID );
				if( spReceiver == NULL )
				{
					// �ð����� ������ �������� ���� �� �ִ�.
					START_LOG( cerr, L"�ʴ��ҷ��� ������ ����.!" )
						<< BUILD_LOG( GetCharName() )
						<< BUILD_LOG( iUnitUID )
						<< END_LOG;
					continue;
				}
				
				kReq.m_mapUnit_ED.insert( std::make_pair( iUnitUID, spReceiver->GetED() ) );
			}
		
			SendToGSField( ERM_FIELD_USER_LIST_MONITORING_MODE_REQ, kReq );
			return;
		}
	}
	else
	{
		if( GetRoomUID() == 0 )
		{
			START_LOG( cerr, L"���� ������� ������ ���°� �ƴѵ� �ʵ� ���� ����Ʈ�� �䱸�Ѵ�?" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;
		}

		//{{ 2009. 3. 20  ������	������, ������ ���?
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
			{
				// �������̶��..
// 				if( !SiKPVPLobbyManager()->GetPVPUserList( kPacket_.m_uiViewPage, kPacket.m_uiTotalPage, kPacket.m_vecUserList ) )
// 				{
// 					START_LOG( cerr, L"pvp user list��� ����!" )
// 						<< BUILD_LOG( kPacket_.m_uiViewPage )
// 						<< END_LOG;
// 				}
// 				else
// 				{
// 					kPacket.m_uiViewPage = kPacket_.m_uiViewPage;
// 				}

			}break;
		case CXSLRoom::RT_SQUARE:
			{
				START_LOG( clog, L"Ŭ���̾�Ʈ���� �̰� ó������ ������ �׳� �н�" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< END_LOG;

				kPacket.m_iOK = NetError::NET_OK;
			}break;
		}
	}

	SendPacket( EGS_COMMUNITY_USER_LIST_ACK, kPacket );
}
#endif SERV_ED_MONITORING_IN_GAME
//}}

//{{ 2012. 11. 15	�ڼ���	Field PT �α� �߰�
#ifdef SERV_FIELD_PLAY_TIME_LOG
IMPL_ON_FUNC( ERM_FIELD_USER_STATISTICS_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iGamePlayTime < 0 )
		kPacket_.m_iGamePlayTime = 0;

	// ���� �÷��� Ÿ��
	m_kUserStatistics.AddPlayTime( KUserStatistics::PT_TOTAL_PLAY_TIME, kPacket_.m_iGamePlayTime );
	m_kUserStatistics.AddPlayTime( KUserStatistics::PT_FIELD_PLAY_TIME, kPacket_.m_iGamePlayTime );	
}
#endif SERV_FIELD_PLAY_TIME_LOG
//}}

#ifdef SERV_INVISIBLE_GM
IMPL_ON_FUNC_NOPARAM( EGS_TOGGLE_INVISIBLE_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	// GM ���ϸ� ����
	if( GetAuthLevel() < SEnum::UAL_GM )
		return;

	// �̸� ���¸� ����
	ToggleInvisible();

	// ���� ����
	KERM_TOGGLE_INVISIBLE_NOT kPacket;
	kPacket.m_iUnitUID = GetCharUID();
	GetFieldUserInfo( kPacket.m_kFieldUserInfo );

	SendToGSField( ERM_TOGGLE_INVISIBLE_NOT, kPacket );
}
#endif SERV_INVISIBLE_GM

//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


