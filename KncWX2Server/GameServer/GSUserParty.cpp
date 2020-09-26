#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
#include "UserManager.h"
#include "GameSysVal/GameSysVal.h"

//{{ 2010. 01. 28  ������	������Ƽ
#ifdef SERV_CHANNEL_PARTY
	#include "PartyListManager.h"
#else
	#include "PartyManager.h"
#endif SERV_CHANNEL_PARTY
//}}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	#include "GSWorldMissionManager.h"
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

#ifdef SERV_TEMP_EVENT_MANAGER
#include "X2Data/XSLTempEventManager.h"
#endif 

#include "Enum/Enum.h"
//{{ 2012. 07. 03	������		���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
#include "Socket/NetCommon.h"
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
//}}

#ifdef SERV_NEW_EVENT_TYPES
#include "GameEvent/GameEventManager.h"
#endif SERV_NEW_EVENT_TYPES

//////////////////////////////////////////////////////////////////////////
//{{ 2011. 12. 05	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
else


#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC( EGS_PARTY_LIST_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_PARTY_LIST_ACK	kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	// 1. ���� ����Ʈ������ ��Ƽ ����Ʈ�� �ټ�����
	if( !CXSLMapData::IsDungeonGate( GetMapID() ) )
	{
		START_LOG( cerr, L"��������Ʈ�� �ƴѵ� ��Ƽ����Ʈ�� ��û�Ѵ�?" )
			<< BUILD_LOG( GetMapID() )
			<< BUILD_LOGc( kPacket_.m_cListType )
			<< BUILD_LOG( kPacket_.m_iID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ROOM_LIST_01;		
		goto end_proc;
	}

	// 2. �ٸ� ������ ��Ƽ ����Ʈ�� ���� ����
	switch( kPacket_.m_cListType )
	{
		//{{ 2010. 03. 02  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
		case KPartyListManager::PLT_DUNGEON:
#else
		case KPartyManager::PLT_DUNGEON:
#endif SERV_CHANNEL_PARTY
			//}}	
			{
				//{{ 2011. 10.10	�ҷ��� ���� �ϵ��ڵ�
#ifdef SERV_HALLOWEEN_DUNGEON
				if( kPacket_.m_iID == SEnum::DI_ELDER_HALLOWEEN_NORMAL || kPacket_.m_iID == SEnum::DI_ELDER_HALLOWEEN_HARD || kPacket_.m_iID == SEnum::DI_ELDER_HALLOWEEN_EXPERT )
				{
					// �Ҽ� ���� ����Ʈ �˻� ����
				}
				else
#endif SERV_HALLOWEEN_DUNGEON
				//}}
								//## 1. ��ûŸ���� ������ ���� �������ִ� ��������Ʈ �Ҽ� �����̾�� �ϰ�
				if( CXSLMapData::GetMapID( kPacket_.m_iID ) != GetMapID() )
				{
					START_LOG( cerr, L"�ٸ� ������ ��Ƽ����Ʈ�� ��û�Ѵ�?" )
#ifdef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharUID() )
#else
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetMapID() )
						<< BUILD_LOGc( kPacket_.m_cListType )
						<< BUILD_LOG( kPacket_.m_iID )
						<< END_LOG;

					kPacket.m_iOK = NetError::ERR_ROOM_LIST_02;
					goto end_proc;
				}
			}
			break;

			//{{ 2010. 03. 02  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
		case KPartyListManager::PLT_GATE_TOTAL:
#else
		case KPartyManager::PLT_GATE_TOTAL:
#endif SERV_CHANNEL_PARTY
			//}}	
			{
				//## 2. ��ûŸ���� ����Ʈ�� ���� ���� �ִ� �ʾ��̵�� ���ƾ� �Ѵ�.
				if( kPacket_.m_iID != GetMapID() )
				{
					START_LOG( cerr, L"�ٸ� ������ ��Ƽ����Ʈ�� ��û�Ѵ�?" )
#ifdef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharUID() )
#else
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetMapID() )
						<< BUILD_LOGc( kPacket_.m_cListType )
						<< BUILD_LOG( kPacket_.m_iID )
						<< END_LOG;

					kPacket.m_iOK = NetError::ERR_ROOM_LIST_02;
					goto end_proc;
				}
			}
			break;

			//{{ 2010. 04. 29  ������	��д��� ����
#ifdef SERV_SECRET_HELL
		case KPartyListManager::PLT_SECRET_DUNGEON:
			break;
#endif SERV_SECRET_HELL
			//}}

		default:
			START_LOG( cerr, L"�̻��� ��Ƽ����Ʈ Ÿ���Դϴ�." )
				<< BUILD_LOGc( kPacket_.m_cListType )
				<< BUILD_LOG( kPacket_.m_iID )
				<< BUILD_LOG( kPacket_.m_nViewPage )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ROOM_LIST_00;
			goto end_proc;
	}

	//{{ 2010. 02. 17  ������	ä�� ��Ƽ ����
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_PartyList( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_PartyList( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_nViewPage )
			<< BUILD_LOGc( kPacket_.m_cListType )
			<< BUILD_LOG( kPacket_.m_iID )
			<< END_LOG;
	}

	return;

end_proc:
	SendPacket( EGS_PARTY_LIST_ACK, kPacket );
}

//{{ 2011. 06. 20	������	���� ����
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( EGS_CREATE_PARTY_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_CREATE_PARTY_ACK kAck;

	switch( kPacket_.m_cPartyType )
	{
	case KPartyInfo::PT_DUNGEON_PARTY:
		{
			if( CheckDungeonPartyCondition( kPacket_.m_iDungeonID, kPacket_.m_DifficultyLevel, kPacket_.m_cDungeonMode, kPacket_.m_bCheckLowLevel, false ) == false )
			{
				kAck.m_iOK = NetError::GetLastError();
				goto end_proc;
			}
		}
		break;

	case KPartyInfo::PT_PVP_PARTY:
		{
			if( CheckPvpPartyCondition( kPacket_.m_cPVPGameType, false ) == false )
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

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	{
		CTime tComeBackBuffEnd	= m_kComeBackUserInfo.GetComeBackBuffEnd();
		kPacket_.m_wsrtComeBackBuffEnd = tComeBackBuffEnd.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
		kPacket_.m_bComeBackBuff = m_kComeBackUserInfo.GetIsComeBackUser();
	}
#endif SERV_COME_BACK_USER_REWARD
	//}} 

#ifdef SERV_DUNGEON_OPTION_IN_LUA
	if( CXSLDungeon::IsFixedMembers( kPacket_.m_iDungeonID ) == true )
	{
		kPacket_.m_maxSlot = CXSLDungeon::GetFixedMembers( kPacket_.m_iDungeonID );
	}
#endif SERV_DUNGEON_OPTION_IN_LUA

	////////////////////////////////////////////////////////////////////////// ó��
	// ��Ƽ ����
	//{{ 2010. 02. 17  ������	ä����Ƽ����
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_CreatePartyReq( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_CreateParty( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"��Ƽ���� ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

	return;

end_proc:
	START_LOG( cwarn, L"��Ƽ ��������.!" )
#ifdef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharUID() )
#else
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( NetError::GetErrStr(kAck.m_iOK) )
		<< END_LOG;

	SendPacket( EGS_CREATE_PARTY_ACK, kAck );
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////
//IMPL_ON_FUNC( EGS_CREATE_PARTY_REQ )
//{
//	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
//
//	KEGS_CREATE_PARTY_ACK kAck;
//
//	////////////////////////////////////////////////////////////////////////// ������ ����
//	// DungeonID�˻�
//	if( ( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel) ) <= 0 )
//	{
//		START_LOG( cerr, L"dungeonid���� �̻��ϴ�!" )
//			<< BUILD_LOG( kPacket_.m_iDungeonID )
//			<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
//			<< END_LOG;
//	}
//
//	//���� ���̵� �������� üũ�Ѵ�.
//	//{{ 2009. 10. 16  ������	����
//	if( false == CXSLDungeon::IsRubenDungeon( kPacket_.m_iDungeonID )  &&
//		false == CXSLDungeon::IsElderDungeon( kPacket_.m_iDungeonID )  &&
//		false == CXSLDungeon::IsBesmaDungeon( kPacket_.m_iDungeonID )  &&
//		false == CXSLDungeon::IsAlteraDungeon( kPacket_.m_iDungeonID )  &&
//		false == CXSLDungeon::IsPeitaDungeon( kPacket_.m_iDungeonID )  &&		
//		//{{ 2010. 12. 29	������	�ϸ� ���� �߰�
//#ifdef SERV_SHIP_DUNGEON
//		false == CXSLDungeon::IsVelderDungeon( kPacket_.m_iDungeonID )  &&
//		false == CXSLDungeon::IsHamelDungeon( kPacket_.m_iDungeonID )
//#else
//		false == CXSLDungeon::IsVelderDungeon( kPacket_.m_iDungeonID )
//#endif SERV_SHIP_DUNGEON
//		//}}
//		)
//	{
//		kAck.m_iOK = NetError::ERR_PARTY_22;
//		goto end_proc;
//	}
//	//}}	
//
//	//��ϸ�������尡 �������� üũ�Ѵ�.
//	if( CXSLDungeon::IsValidDungeonMode( static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_cDungeonMode) ) == false )
//	{
//		START_LOG( cerr, L"���� ��尪�� �̻��մϴ�!" )
//			<< BUILD_LOGc( kPacket_.m_cDungeonMode )
//			<< END_LOG;
//
//		kAck.m_iOK = NetError::ERR_PARTY_22;
//		goto end_proc;
//	}
//
//	////////////////////////////////////////////////////////////////////////// ���� ����
//	//{{ 2009. 8. 5  ������		ü��ID����
//	if( IsGuestUser()  &&  CXSLDungeon::IsHenirDungeon( kPacket_.m_iDungeonID ) )
//	{
//		kAck.m_iOK = NetError::ERR_GUEST_USER_00;
//		goto end_proc;
//	}
//	//}}
//
//	//�̹� ��Ƽ�� �ִ°��..
//	if( GetPartyUID() != 0 )
//	{
//		kAck.m_iOK = NetError::ERR_PARTY_12;
//		goto end_proc;
//	}
//
//	//���� �κ� �ִ°��.
//	if( GetUnitState() == CXSLUnit::CUS_PVP_LOBBY )
//	{
//		kAck.m_iOK = NetError::ERR_PARTY_13;
//		goto end_proc;
//	}
//
//	//::1. �ټ��� �˻�.
//	if( CheckingSpirit( kPacket_.m_iDungeonID ) == false )
//	{
//		kAck.m_iOK = NetError::ERR_ROOM_27;
//		goto end_proc;
//	}
//
//	//::2. �����ʿ� �����۰˻�.
//	int iRequireItemID = 0;
//	int iRequireQuantity = 0;
//
//	if( CheckRequiredItemToEnterDungeonRoom( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel), (CXSLDungeon::DUNGEON_MODE) kPacket_.m_cDungeonMode, iRequireItemID, iRequireQuantity ) == false )
//	{
//		//{{ 2010. 05. 06  ������	���� �˾�
//		KEGS_ERROR_POPUP_NOT kNot;
//		kNot.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
//		kNot.m_iErrorData1 = iRequireItemID;
//		kNot.m_iErrorData2 = iRequireQuantity;
//		SendPacket( EGS_ERROR_POPUP_NOT, kNot );
//		//}}
//
//		kAck.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
//		goto end_proc;
//	}
//
//	//��� �̻��� �ƴϸ� üũ�Ѵ�.
//	if( GetAuthLevel() < SEnum::UAL_GM )
//	{
//		//::3. ������� Ŭ��� ���� �ʾ���.
//		const int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( kPacket_.m_iDungeonID );
//		if( iRequireDungeonID != 0 )
//		{
//			std::map<int,KDungeonClearInfo>::iterator mit;
//			mit = m_mapDungeonClear.find( iRequireDungeonID );
//			if( mit == m_mapDungeonClear.end() )
//			{
//				kAck.m_iOK	= NetError::ERR_ROOM_06;
//				goto end_proc;
//			}
//		}
//
//		//{{ 2010. 12. 30	������	�ϸ� ���� �߰�
//#ifdef SERV_SHIP_DUNGEON
//		if( CXSLDungeon::IsShipDungeon( kPacket_.m_iDungeonID ) == true )
//		{
//			if( CXSLDungeon::IsVelderDungeon( kPacket_.m_iDungeonID ) == true )
//			{
//				// �ش� map�� �� �� �ִ��� ���� üũ
//				if( CheckEnterTheVillage( CXSLMapData::MI_HAMEL ) == false )
//				{
//					START_LOG( cwarn, L"������ ������ �� �ִ� ������ �ȵ�." )
//						<< BUILD_LOG( GetCharUID() )
//						<< BUILD_LOG( kPacket_.m_iDungeonID )
//						<< END_LOG;
//
//					kAck.m_iOK = NetError::GetLastError();
//					goto end_proc;
//				}
//			}
//		}
//#endif SERV_SHIP_DUNGEON
//		//}}
//	}
//	else
//	{
//		START_LOG( cout, L"��� ��Ƽ ���� �õ�!" )
//			<< BUILD_LOG( GetCharName() )
//			<< BUILD_LOG( kPacket_.m_iDungeonID );
//	}
//
//	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
//#ifdef SERV_COME_BACK_USER_REWARD
//	{
//		// ��Ƽ �����ڰ� �޸� �����̶�� �޸� ��Ƽ
//		CTime tComeBackBuffEnd = m_kComeBackUserInfo.GetComeBackBuffEnd();
//		kPacket_.m_bComeBackBuff = m_kComeBackUserInfo.GetIsComeBackUser();
//		kPacket_.m_wsrtComeBackBuffEnd = tComeBackBuffEnd.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
//	}
//#endif SERV_COME_BACK_USER_REWARD
//	//}} 
//
//	//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
//#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
//	if( kPacket_.m_bCheckLowLevel == true )	// ���� ������ ���� �Ǿ� �ִٸ�
//	{
//		int iDungeonMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel) );
//
//		if( static_cast<int>(GetLevel()) < iDungeonMinLevel - 2 )
//		{
//			kAck.m_iOK = NetError::ERR_CREATE_PARTY_FAILE;
//			goto end_proc;
//		}
//	}
//#endif SERV_DUNGEON_DIFFICULTY_REVISION
//	//}}
//
//	////////////////////////////////////////////////////////////////////////// ó��
//	// ��Ƽ ����
//	//{{ 2010. 02. 17  ������	ä����Ƽ����
//#ifdef SERV_CHANNEL_PARTY
//	if( SiKPartyListManager()->E_CreatePartyReq( GetThisPtr<KGSUser>(), kPacket_ ) == false )
//#else
//	if( SiKPartyManager()->E_CreateParty( GetThisPtr<KGSUser>(), kPacket_ ) == false )
//#endif SERV_CHANNEL_PARTY
//		//}}	
//	{
//		START_LOG( cerr, L"��Ƽ���� ����.!" )
//			<< BUILD_LOG( GetCharName() )
//			<< END_LOG;
//	}
//
//	return;
//
//end_proc:
//	START_LOG( cwarn, L"��Ƽ ��������.!" )
//		<< BUILD_LOG( GetCharName() )
//		<< BUILD_LOG( NetError::GetErrStr(kAck.m_iOK) )
//		<< END_LOG;
//
//	SendPacket( EGS_CREATE_PARTY_ACK, kAck );
//}
//////////////////////////////////////////////////////////////////////////
#endif SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}

IMPL_ON_FUNC( EGS_JOIN_PARTY_REQ )
{
	//��Ƽ ������ ����/��������Ʈ �϶��� ������.
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_JOIN_PARTY_ACK kAck;	

	////////////////////////////////////////////////////////////////////////// ������ ����
	// partyuid
	if( kPacket_.m_iPartyUID <= 0 )
	{
		START_LOG( cwarn, L"��Ƽuid ������ �̻��մϴ�." )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_17;
		goto end_proc;
	}

	////////////////////////////////////////////////////////////////////////// ���� ����	
	// �̹� ���Ե� ��Ƽ�� �ִ���..?
	if( GetPartyUID() != 0 )
	{
		START_LOG( cwarn, L"�̹� ���Ե� ��Ƽ�� ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_12;
		goto end_proc;
	}

	// �����κ� �մ���..
	if( GetUnitState() == CXSLUnit::CUS_PVP_LOBBY )
	{
		START_LOG( cerr, L"�����κ񿡼��� ��Ƽ�� �����Ҽ� �����ϴ�." )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_14;
		goto end_proc;
	}

	//{{ 2011. 07. 27	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	// ��Ƽ ��û�ڰ� ���� ��Ī ��û �ߴٸ� ��û �Ұ��� 
	if( IsRequestMatchMaking() == true )
	{
		kAck.m_iOK = NetError::ERR_MATCH_MAKING_18;
		goto end_proc;
	}

	// ��Ƽ ��û�ڰ� ���� ��Ī ���̶�� ��û �Ұ��� 
	if( GetMatchUID() != 0 )
	{
		kAck.m_iOK = NetError::ERR_MATCH_MAKING_19;
		goto end_proc;
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}

	int iDungeonID = 0;
	char cDifficultyLevel = 0;
	char cDungeonMode = 0;
	//��Ƽ�� ���������� ã�Ƽ�..
	//{{ 2010. 02. 20  ������	ä�� ��Ƽ ����
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->GetDungeonInfo( kPacket_.m_iPartyUID, iDungeonID, cDifficultyLevel, cDungeonMode ) == false )
#else
	if( SiKPartyManager()->GetDungeonInfo( kPacket_.m_iPartyUID, iDungeonID, cDifficultyLevel, cDungeonMode ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cwarn, L"��Ƽ�� �������� ã�� ����.!" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( GetPartyUID() )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		// �ð����� ��Ƽ������ ��ã���� �ִ�
		kAck.m_iOK = NetError::ERR_PARTY_17;
		goto end_proc;
	}

	//{{ 2009. 8. 5  ������		ü��ID����
	if( IsGuestUser()  &&  CXSLDungeon::IsHenirDungeon( iDungeonID ) )
	{
		kAck.m_iOK = NetError::ERR_GUEST_USER_00;
		goto end_proc;
	}
	//}}

	//::1. �ټ��� �˻�.
	if( CheckingSpirit( iDungeonID ) == false )
	{
		kAck.m_iOK = NetError::ERR_ROOM_27;
		goto end_proc;
	}

	//::2. �����ʿ� �����۰˻�.
	int iRequireItemID = 0;
	int iRequireQuantity = 0;

	if( CheckRequiredItemToEnterDungeonRoom( iDungeonID + static_cast<int>(cDifficultyLevel), (CXSLDungeon::DUNGEON_MODE)cDungeonMode, iRequireItemID, iRequireQuantity ) == false )
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

	//��� �̻��� �ƴϸ� üũ�Ѵ�.
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		//::3. ������� �˻�.
		//int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( iDungeonID + static_cast<int>(cDifficultyLevel) );

		////��������� �־�� �Ѵ�.
		//if( iRequireDungeonID != 0 )
		//{
		//	std::map< int, KDungeonClearInfo >::const_iterator mit;
		//	mit = m_mapDungeonClear.find( iRequireDungeonID );
		//	if( mit == m_mapDungeonClear.end() )
		//	{
		//		kAck.m_iOK = NetError::ERR_ROOM_06;
		//		goto end_proc;
		//	}
		//}
	}
	else
	{
		START_LOG( cout, L"��� ��Ƽ ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( iDungeonID )
			;
	}	
	//}}

	//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	bool isPartyNULL = false;
	if( SiKPartyListManager()->IsPossibleLowLevel( GetThisPtr<KGSUser>(), kPacket_.m_iPartyUID, isPartyNULL ) == false )
	{
		if( isPartyNULL == true )
		{
			kAck.m_iOK = NetError::ERR_PARTY_17;
			goto end_proc;
		}
		kAck.m_iOK = NetError::ERR_MIN_LEVEL_JOIN_PARTY_FAILE;
		goto end_proc;
	}
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

	////////////////////////////////////////////////////////////////////////// ó��	
	// ��Ƽ�� �����Ҽ� �ִ� ��Ȳ�̸�..
	//{{ 2010. 02. 17  ������	ä�� ��Ƽ ����
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_JoinPartyReq( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_JoinParty( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}
	{
		START_LOG( cwarn, L"��Ƽ ���Խ�û ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< END_LOG;
	}

	return;

end_proc:
	START_LOG( cwarn, L"��Ƽ �������.!" )
#ifdef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharUID() )
#else
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( NetError::GetErrStr( kAck.m_iOK ) )
		<< END_LOG;

	SendPacket( EGS_JOIN_PARTY_ACK, kAck );
}

IMPL_ON_FUNC( EGS_JOIN_PARTY_INVITE_REPLY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( GetPartyUID() == 0 )
	{
		// [����] �ð����� �߻�����

		START_LOG( cwarn, L"��Ƽ���ε� Party UID �� ����.?" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

	//{{ 2010. 02. 17  ������	ä�� ��Ƽ ����
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_JoinPartyInviteReply( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_JoinPartyInviteReply( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"��Ƽ���� ����ó�� ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_LEAVE_PARTY_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		// [����] ��Ƽ��Ż�ϱ����� ������ߴٸ� �ð����� �߻�����

		START_LOG( cwarn, L"��Ƽ�� Ż���ϱ����ε� UID�� ����.?" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_LEAVE_PARTY_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_PARTY_01;
		SendPacket( EGS_LEAVE_PARTY_ACK, kPacket );
		return;
	}

	//{{ 2010. 02. 22  ������	ä�� ��Ƽ ����
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_LeavePartyReq( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_LeaveParty( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"��Ƽ ������ ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( NetError::GetErrStr(kPacket_.m_iReason) )
			<< END_LOG;
	}

	//{{ 2011. 07. 08	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

IMPL_ON_FUNC( EGS_INVITE_PARTY_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_INVITE_PARTY_ACK kAck;
	kAck.m_iReceiverUID = kPacket_.m_iReceiverUID;
	kAck.m_iOK = NetError::NET_OK;	

	// ���� ä���� ��츸 �ʴ밡 �����մϴ�.
	KGSUserPtr spReceiver = GetKUserManager()->GetByCID( kPacket_.m_iReceiverUID );
	if( spReceiver == NULL )
	{
		// �ð����� ������ �������� ���� �� �ִ�.
		START_LOG( cwarn, L"�ʴ��ҷ��� ������ ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iReceiverUID )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_07;
		SendPacket( EGS_INVITE_PARTY_ACK, kAck );
		return;
	}

	if( GetPartyUID() == 0 )
	{
		kAck.m_iOK = NetError::ERR_PARTY_09;
		SendPacket( EGS_INVITE_PARTY_ACK, kAck );
		return;
	}

	//{{ 2010. 02. 22  ������	ä�� ��Ƽ ����
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_InvitePartyReq( GetThisPtr<KGSUser>(), spReceiver ) == false )
#else
	if( SiKPartyManager()->E_InviteParty( GetPartyUID(), GetThisPtr<KGSUser>(), spReceiver ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cwarn, L"��Ƽ���� �ʴ��� ���� �ʴ����.!" )
			<< BUILD_LOG( GetPartyUID() )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( spReceiver->GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( spReceiver->GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_INVITE_PARTY_REPLY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	////��Ƽ�� �ʴ뿡 ���ϴ� ���..
	//KERM_INVITE_PARTY_REPLY_NOT kNot;
	//kNot.m_bAccept = kPacket_.m_bAccept;
	//kNot.m_iReason = 0;	// ���ٸ� ������ �������.
	//GetPartyUserInfo( kNot.m_kPartyUserInfo );

	//_SendToCnRoom( kPacket_.m_iPartyUID, ERM_INVITE_PARTY_REPLY_NOT, kNot );

	//{{ 2010. 02. 22  ������	ä�� ��Ƽ ����
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_InvitePartyReply( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_InvitePartyReply( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"��Ƽ �ʴ뿡 ���� ���� ����." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}
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
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_03;
		goto end_proc;
	}

	int iDungeonID = 0;
	char cDifficultyLevel = 0;
	char cDungeonMode = 0;
	//��Ƽ�� ���������� ã�Ƽ�..

	//{{ 2010. 02. 22  ������	ä�� ��Ƽ ����
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->GetDungeonInfo( GetPartyUID(), iDungeonID, cDifficultyLevel, cDungeonMode ) == false )
#else
	if( SiKPartyManager()->GetDungeonInfo( GetPartyUID(), iDungeonID, cDifficultyLevel, cDungeonMode ) == false )
#endif SERV_CHANNEL_PARTY
		//}}
	{
		START_LOG( cerr, L"��Ƽ�� �������� ã�� ����.!" )
			<< BUILD_LOG( GetPartyUID() )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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
#ifdef SERV_SHIP_DUNGEON
	if( CXSLDungeon::IsShipDungeon( iDungeonID ) == false )
#endif SERV_SHIP_DUNGEON
	//}}
	{
		//��������Ʈ�� �ƴϸ� �����Ҽ� ����.
		if( CXSLMapData::IsDungeonGate( GetMapID() ) == false )
		{
			START_LOG( cwarn, L"��������Ʈ�� �ƴϸ� ��Ƽ���� �Ҽ�����." )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
#else
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( iDungeonID )
				<< BUILD_LOG( GetMapID() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PARTY_20;
			goto end_proc;
		}

			//{{ 2011. 10.10	�ҷ��� ���� �ϵ��ڵ�
#ifdef SERV_HALLOWEEN_DUNGEON
		if( iDungeonID == SEnum::DI_ELDER_HALLOWEEN_NORMAL || iDungeonID == SEnum::DI_ELDER_HALLOWEEN_HARD || iDungeonID == SEnum::DI_ELDER_HALLOWEEN_EXPERT )
		{
			// �Ҽ� ���� ����Ʈ �˻� ����
		}
		else
#endif SERV_HALLOWEEN_DUNGEON
			//}}
		if( GetMapID() != CXSLMapData::GetMapID( iDungeonID ) )
		{
			START_LOG( cwarn, L"�����Ҽ��ִ� ��������Ʈ�� �ƴ�." )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
#else
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( iDungeonID )
				<< BUILD_LOG( GetMapID() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PARTY_29;
			goto end_proc;
		}
	}

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

		//{{ 2010. 12. 30	������	�ϸ� ���� �߰�
#ifdef SERV_SHIP_DUNGEON
		if( CXSLDungeon::IsShipDungeon( iDungeonID ) == true )
		{
			if( CXSLDungeon::IsVelderDungeon( iDungeonID ) == true )
			{
				// �ش� map�� �� �� �ִ��� ���� üũ
				if( CheckEnterTheVillage( CXSLMapData::MI_HAMEL ) == false )
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
	//{{ 2010. 02. 22  ������	ä�� ��Ƽ ����
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ChangeReadyReq( GetThisPtr<KGSUser>(), kPacket_.m_bReady ) == false )
#else
	if( SiKPartyManager()->E_ChangeReady( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_.m_bReady ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cwarn, L"��Ƽ ���� ���� ����." )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_22;
		goto end_proc;
	}

#ifdef SERV_NEW_EVENT_TYPES
	bool bIsEventDungeon = SiCXSLDungeonManager()->IsSwitchingWithEventInfo( kPacket_.m_iDungeonID );
	if( !SiKGameEventManager()->IsEnableDungeon( kPacket_.m_iDungeonID, bIsEventDungeon ) )
	{
		START_LOG( cerr, L"�����ִ� �̺�Ʈ DungeonID�Դϴ�. Ŭ�󿡼� �˻������ٵ�.." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_22;
		goto end_proc;
	}
#endif SERV_NEW_EVENT_TYPES

	//{{ 2009. 7. 30  ������	HenirDungeonMode����
	if( CXSLDungeon::IsValidDungeonMode( static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_cDungeonMode) ) == false )
	{
		START_LOG( cerr, L"���������� HenirDungeonMode���Դϴ�." )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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

	//::2. �����ʿ� �����۰˻�.
	int iRequireItemID = 0;
	int iRequireQuantity = 0;

	if( CheckRequiredItemToEnterDungeonRoom( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel), (CXSLDungeon::DUNGEON_MODE) kPacket_.m_cDungeonMode, iRequireItemID, iRequireQuantity ) == false )
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
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
#else
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PARTY_03;

			goto end_proc;
		}

		if( spParty->GetCheckLowLevel() == true )
		{
			int iDungeonMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel) );

			if( spParty->CheckPartyMinLevel( iDungeonMinLevel ) == false )
			{
				kAck.m_iOK = NetError::ERR_CHANGE_DUNGEON_MAP_FAILE;

				goto end_proc;
			}
		}
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
	//{{ 2010. 02. 22  ������	ä�� ��Ƽ ����
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ChangeDungeon( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_ChangeDungeon( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"�������� ����." )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
			<< END_LOG;
	}	

	return;

end_proc:
	SendPacket( EGS_PARTY_CHANGE_DUNGEON_ACK, kAck );
}

IMPL_ON_FUNC( EGS_PARTY_GAME_START_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_PARTY_GAME_START_REQ, EGS_PARTY_GAME_START_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	KEGS_PARTY_GAME_START_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	//{{ 2011. 02. 21  ��μ�	Ư�� ���� ���� ������ ���� ����
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
	// ���� �ʱ�ȭ
	SetReserveKickUser( false );
	SetDungeonRequiredItemID( 0 );
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
	//}}

	////////////////////////////////////////////////////////////////////////// ���� �˻�
	// 1. ��Ƽ�� ���� �ִ��� �˻�
	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ UID �̻�.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_03;
		goto end_proc;
	}
	
	int iDungeonID = 0;
	char cDifficultyLevel = 0;
	char cDungeonMode = 0;
	// 2. ��Ƽ�� ���������� ã�Ƽ�..
	//{{ 2010. 02. 22  ������	ä�� ��Ƽ ����
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->GetDungeonInfo( GetPartyUID(), iDungeonID, cDifficultyLevel, cDungeonMode ) == false )
#else
	if( SiKPartyManager()->GetDungeonInfo( GetPartyUID(), iDungeonID, cDifficultyLevel, cDungeonMode ) == false )
#endif SERV_CHANNEL_PARTY
		//}}
	{
		START_LOG( cerr, L"��Ƽ�� �������� ã�� ����.!" )
			<< BUILD_LOG( GetPartyUID() )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		//��Ƽ UID �̻�� ���� ���� ����.
		kAck.m_iOK = NetError::ERR_PARTY_03;
		goto end_proc;
	}

#ifdef SERV_DUNGEON_OPTION_IN_LUA
	if( CXSLDungeon::IsFixedMembers( iDungeonID ) == true )
	{
		int iNumMember = 0;
		if( SiKPartyListManager()->GetPartyNumMember( GetPartyUID(), iNumMember ) == false )
		{
			START_LOG( cerr, L"��Ƽ �ɹ� �̻�.!" )
				<< BUILD_LOG( GetPartyUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( iNumMember )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PARTY_46;
			goto end_proc;
		}

		if( iNumMember != CXSLDungeon::GetFixedMembers( iDungeonID ) )
		{
			kAck.m_iOK = NetError::ERR_PARTY_46;
			goto end_proc;
		}
	}
#endif SERV_DUNGEON_OPTION_IN_LUA

#ifdef SERV_NEW_EVENT_TYPES
	bool bIsEventDungeon = SiCXSLDungeonManager()->IsSwitchingWithEventInfo( iDungeonID );
	if( !SiKGameEventManager()->IsEnableDungeon( iDungeonID, bIsEventDungeon ) )
	{
		START_LOG( cerr, L"�����ִ� �̺�Ʈ DungeonID�Դϴ�. Ŭ�󿡼� �˻������ٵ�.." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( iDungeonID )
			<< BUILD_LOGc( (int)cDifficultyLevel )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_22;
		goto end_proc;
	}
#endif SERV_NEW_EVENT_TYPES

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
#ifdef SERV_SHIP_DUNGEON
	if( CXSLDungeon::IsShipDungeon( iDungeonID ) == false )
#endif SERV_SHIP_DUNGEON
	//}}
	{
		// 3. ��������Ʈ���� �˻�.
		if( CXSLMapData::IsDungeonGate( GetMapID() ) == false )
		{
			kAck.m_iOK = NetError::ERR_PARTY_20;
			goto end_proc;
		}

		//{{ 2011. 10.10	�ҷ��� ���� �ϵ��ڵ�
#ifdef SERV_HALLOWEEN_DUNGEON
		if( iDungeonID == SEnum::DI_ELDER_HALLOWEEN_NORMAL || iDungeonID == SEnum::DI_ELDER_HALLOWEEN_HARD || iDungeonID == SEnum::DI_ELDER_HALLOWEEN_EXPERT )
		{
			// �Ҽ� ���� ����Ʈ �˻� ����
		}
		else
#endif SERV_HALLOWEEN_DUNGEON
			//}}
		// 4. �ش� ������ �´� ��������Ʈ���� �˻�	
		if( GetMapID() != CXSLMapData::GetMapID( iDungeonID ) )
		{
			kAck.m_iOK = NetError::ERR_PARTY_29;
			goto end_proc;
		}
	}

	// 5. ��Ƽ���� �ټ��� �˻�
	if( CheckingSpirit( iDungeonID ) == false )
	{
		kAck.m_iOK = NetError::ERR_ROOM_27;
		goto end_proc;
	}

	// 6. �����ʿ� �����۰˻�.
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

	// 7. ������� Ŭ��� �˻�
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( iDungeonID + static_cast<int>(cDifficultyLevel) );
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

		//{{ 2010. 12. 30	������	�ϸ� ���� �߰�
#ifdef SERV_SHIP_DUNGEON
		if( CXSLDungeon::IsShipDungeon( iDungeonID ) == true )
		{
			if( CXSLDungeon::IsVelderDungeon( iDungeonID ) == true )
			{
				// �ش� map�� �� �� �ִ��� ���� üũ
				if( CheckEnterTheVillage( CXSLMapData::MI_HAMEL ) == false )
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
	
	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	//	���潺���� �˻�
	if( CXSLDungeon::IsDefenceDungeon( iDungeonID ) == true )
	{
		//	���� ���� �˻�
		if( false == SiKGSWorldMissionManager()->GetIsActive() )
		{
			kAck.m_iOK = NetError::ERR_WORLD_MISSION_02;
			goto end_proc;
		}
	}
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	// 8. ��Ƽ ���� ����
	//{{ 2010. 02. 22  ������	ä�� ��Ƽ ����
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_GameStart( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_GameStart( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"�������� �����ϱ� ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( iDungeonID )
			<< BUILD_LOGc( cDifficultyLevel )
			<< BUILD_LOGc( GetPartyUID() )
			<< END_LOG;
	}

	return;

end_proc:
	SendPacket( EGS_PARTY_GAME_START_ACK, kAck );
}

IMPL_ON_FUNC( ERM_PARTY_OPEN_DUNGEON_ROOM_ACK )
{
	// ��Ƽ�忡�� �˷��ֱ� ����.
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( GetPartyUID() == 0 )
		{
			START_LOG( cerr, L"��Ƽ�� ���� ������ �����޴µ� ��Ƽ UID�� ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
#else
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;
		}
	}

	KEGS_PARTY_GAME_START_ACK kAck;
	kAck.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_PARTY_GAME_START_ACK, kAck );
}

_IMPL_ON_FUNC( ERM_PARTY_OPEN_DUNGEON_ROOM_NOT, KEGS_PARTY_GAME_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
		kPacket_.m_wstrCNIP.clear();

	m_iRoomUID = kPacket_.m_RoomInfo.m_RoomUID;

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
				if( m_kUserDungeonManager.IncreaseHenirRewardCount() == false )
				{
					START_LOG( clog, L"��ϸ� ���� ȹ�� Ƚ�� ���� ����!!" )
#ifdef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharUID() )
#else
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardNormalCount() )
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardPremiumCount() )
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardEventCount() )
						<< END_LOG;
				}
			}
#endif SERV_NEW_HENIR_TEST
			//}}

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
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCharUID() )
#else
					<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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

	SendPacket( EGS_PARTY_GAME_START_NOT, kPacket_ );

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
	// ���� �����ϸ� �ʱ�ȭ
	m_kInventory.SetUseItemNeedPayment( false );
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX

	//{{ 2009. 5. 30  ������	�ӽ� �κ�
	ClearTempInventory();
	//}}
}

//{{ 2009. 2. 4  ������	���� ���÷���
_IMPL_ON_FUNC( ERM_PARTY_GAME_REPLAY_START_ACK, KERM_PARTY_OPEN_DUNGEON_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( GetPartyUID() == 0 )
		{
			START_LOG( cerr, L"��Ƽ�� ���� ���� ���÷��̸� �ߴµ� ��Ƽ UID�� ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
#else
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;
		}
	}

	KEGS_PARTY_GAME_START_ACK kAck;
	kAck.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_PARTY_GAME_START_ACK, kAck );
}

_IMPL_ON_FUNC( ERM_PARTY_GAME_REPLAY_START_NOT, KEGS_PARTY_GAME_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
		kPacket_.m_wstrCNIP.clear();

	if( GetRoomUID() != kPacket_.m_RoomInfo.m_RoomUID )
	{
		START_LOG( cerr, L"���� ���÷����ε� RoomUID�� �ٸ��� �ȵǴµ�?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( kPacket_.m_RoomInfo.m_RoomUID )
			<< END_LOG;

		// roomuid�� �ϴ� �־��ش�. ���࿡ roomuid�� 0�̶��.. �̰� ������ �ȴ�
		m_iRoomUID = kPacket_.m_RoomInfo.m_RoomUID;
	}

	m_kTimer[TM_ROOM].restart();

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
				if( m_kUserDungeonManager.IncreaseHenirRewardCount() == false )
				{
					START_LOG( cerr, L"��ϸ� ���� ȹ�� Ƚ�� ���� ����!!" )
#ifdef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharUID() )
#else
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardNormalCount() )
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardPremiumCount() )
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardEventCount() )
						<< END_LOG;
				}
			}
#endif SERV_NEW_HENIR_TEST
			//}}

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
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCharUID() )
#else
					<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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
				// �� ���� ���� ó�� - ���ͼ����� ERM_LEAVE_ROOM_REQ�� ������ ����Ż ó�� ��
				// ���ƿ��� ACK�� KGSFSM�� �ɷ��� Ŭ���̾�Ʈ�� ���۵��� �ʴ´�.
				//KERM_LEAVE_ROOM_REQ kPacketReq;
				//kPacketReq.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_22;
				//SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacketReq );
				//return;
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

	SendPacket( EGS_PARTY_GAME_START_NOT, kPacket_ );

	//{{ 2009. 5. 30  ������	�ӽ� �κ�
	ClearTempInventory();
	//}}
}
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2010. 12. 28	������	�ϸ� ���� �߰�
#ifdef SERV_SHIP_DUNGEON

IMPL_ON_FUNC( EGS_PARTY_TO_FIELD_MAP_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_PARTY_TO_FIELD_MAP_ACK kAck;

	CXSLRoom::ROOM_TYPE eRoomType = CXSLRoom::GetRoomType( GetRoomUID() );

	if( eRoomType != CXSLRoom::RT_DUNGEON )
	{
		START_LOG( cerr, L"��Ƽ ������ �����ϴµ� UID�� ���� �Ǵ� �����̵尡 �ƴ�.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( static_cast<int>(eRoomType) )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_25;
		SendPacket( EGS_PARTY_TO_FIELD_MAP_ACK, kAck );
		return;
	}

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ UID �̻�.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_TO_FIELD_MAP_ACK, kAck );
		return;
	}
	
	if( SiKPartyListManager()->E_ToFieldMap( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"�������� ������ ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC_NOPARAM( EGS_PARTY_TO_FIELD_MAP_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_PARTY_TO_FIELD_MAP_ACK kAck;

	CXSLRoom::ROOM_TYPE eRoomType = CXSLRoom::GetRoomType( GetRoomUID() );

	if( eRoomType != CXSLRoom::RT_DUNGEON )
	{
		START_LOG( cerr, L"��Ƽ ������ �����ϴµ� UID�� ������ �ƴ�.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( static_cast<int>(eRoomType) )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_25;
		SendPacket( EGS_PARTY_TO_FIELD_MAP_ACK, kAck );
		return;
	}

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ UID �̻�.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_TO_FIELD_MAP_ACK, kAck );
		return;
	}

	//{{ 2010. 02. 23  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ToFieldMap( GetThisPtr<KGSUser>() ) == false )
#else
	if( SiKPartyManager()->E_ToFieldMap( GetPartyUID(), GetThisPtr<KGSUser>() ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"�������� ������ ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_SHIP_DUNGEON
//}}

_IMPL_ON_FUNC( ERM_PARTY_TO_FIELD_MAP_ACK, KEGS_PARTY_TO_FIELD_MAP_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK  ||  GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ ������ ���� ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}

	SendPacket( EGS_PARTY_TO_FIELD_MAP_ACK, kPacket_ );
}

//{{ 2010. 12. 29	������	�ϸ� ���� �߰�
#ifdef SERV_SHIP_DUNGEON

_IMPL_ON_FUNC( ERM_PARTY_TO_FIELD_MAP_NOT, KEGS_PARTY_TO_FIELD_MAP_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SetRoomUID( 0 );

	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	if( GetTradeUID() != 0 )
	{
		KEGS_BREAK_TRADE_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
		_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
	}
	
	SendPacket( EGS_PARTY_TO_FIELD_MAP_NOT, kPacket_ );
}

#else

IMPL_ON_FUNC_NOPARAM( ERM_PARTY_TO_FIELD_MAP_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SetRoomUID( 0 );

	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	if( GetTradeUID() != 0 )
	{
		KEGS_BREAK_TRADE_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
		_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
	}

	SendID( EGS_PARTY_TO_FIELD_MAP_NOT );
}

#endif SERV_SHIP_DUNGEON
//}}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_NAME_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );	

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ UID �̻�.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_PARTY_CHANGE_NAME_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_NAME_ACK, kAck );
		return;
	}

	//{{ 2010. 02. 23  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ChangeName( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_ChangeName( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"��Ƽ �������� ���� ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}	
}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_PUBLIC_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ UID �̻�.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_PARTY_CHANGE_PUBLIC_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_PUBLIC_ACK, kAck );
		return;
	}

	//{{ 2010. 02. 23  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ChangePublic( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_ChangePublic( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"��Ƽ �������� ���� ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_HOST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ UID �̻�.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}
//}}

//{{ 2009. 5. 1  ������		��Ƽ ������
IMPL_ON_FUNC( EGS_PARTY_QUICK_JOIN_REQ )
{
	// ��Ƽ �������� ����/��������Ʈ �϶��� ������.
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_PARTY_QUICK_JOIN_ACK kAck;

	// �̹� ���Ե� ��Ƽ�� �ִ���..?
	if( GetPartyUID() != 0 )
	{
		START_LOG( cwarn, L"�̹� ���Ե� ��Ƽ�� ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_12;
		goto end_proc;
	}

	// �����κ� �մ���..
	if( GetUnitState() == CXSLUnit::CUS_PVP_LOBBY )
	{
		START_LOG( cerr, L"�����κ񿡼��� ��Ƽ�� �����Ҽ� �����ϴ�." )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_14;
		goto end_proc;
	}

	//::1. �ټ��� �˻�.				
	if( CheckingSpirit( kPacket_.m_iDungeonID ) == false )
	{
		kAck.m_iOK = NetError::ERR_ROOM_27;
		goto end_proc;
	}

	//////////////////////////////////////////////////////////////////////////
	if( ( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_cDifficulty) ) <= 0 )
	{
		START_LOG( cerr, L"dungeonid���� �̻��ϴ�!" )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_cDifficulty )
			<< END_LOG;
	}
	//////////////////////////////////////////////////////////////////////////

	//::2. �����ʿ� �����۰˻�.
	int iRequireItemID = 0;
	int iRequireQuantity = 0;

	if( CheckRequiredItemToEnterDungeonRoom( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_cDifficulty), (CXSLDungeon::DUNGEON_MODE) kPacket_.m_cDungeonMode, iRequireItemID, iRequireQuantity ) == false )
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

	//��� �̻��� �ƴϸ� üũ�Ѵ�.
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		//::3. ������� �˻�.
		//int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( iDungeonID + static_cast<int>(cDifficultyLevel) );

		////��������� �־�� �Ѵ�.
		//if( iRequireDungeonID != 0 )
		//{
		//	std::map< int, KDungeonClearInfo >::const_iterator mit;
		//	mit = m_mapDungeonClear.find( iRequireDungeonID );
		//	if( mit == m_mapDungeonClear.end() )
		//	{
		//		kAck.m_iOK = NetError::ERR_ROOM_06;
		//		goto end_proc;
		//	}
		//}
	}
	else
	{
		START_LOG( cout, L"��� ����Ƽ!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_cDungeonMode )
			<< BUILD_LOGc( kPacket_.m_cDifficulty );
	}
	//}}

	// ��Ƽ�� �����Ҽ� �ִ� ��Ȳ�̸�..
	//{{ 2010. 03. 02  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY	
	if( SiKPartyListManager()->E_QuickJoin( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_QuickJoin( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cwarn, L"��Ƽ ���Խ�û ����.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_cDifficulty )
			<< END_LOG;
	}

	return;

end_proc:
	START_LOG( cwarn, L"��Ƽ �������.!" )
#ifdef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharUID() )
#else
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( NetError::GetErrStr(kAck.m_iOK) )
		<< END_LOG;

	SendPacket( EGS_PARTY_QUICK_JOIN_ACK, kAck );	
}
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2010. 02. 17  ������	ä�� ��Ƽ ����
#ifdef SERV_CHANNEL_PARTY

IMPL_ON_FUNC( EPM_CREATE_PARTY_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );	

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		//������ �̻����� üũ�Ѵ�.
		if( GetAuthLevel() >= SEnum::UAL_DEVELOPER )
		{
			START_LOG( cout, L"������ ��Ƽ ���� ����!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
#else
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) );
		}

		KEGS_CREATE_PARTY_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_CREATE_PARTY_ACK, kPacketAck );
		return;
	}

	//{{ 2010. 03. 12  ������	ä�� ���� ��Ƽ ���� ����	
	// �̹� ��Ƽ ���Ե� �������� Ȯ���Ѵ�.
	if( GetPartyUID() > 0 )
	{
		if( GetPartyUID() != kPacket_.m_kPartyInfo.m_iPartyUID )
		{
			START_LOG( cerr, L"�̹� PartyUID�� ����Ǿ��ִµ� �� PartyUID�� �����Ϸ��� �ϳ�? ���λ����� ��Ƽ �ڵ���Żó��!" )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetPartyUID() )
				<< BUILD_LOG( kPacket_.m_kPartyInfo.m_iPartyUID )
				<< END_LOG;

			// ������ ���ԵǾ� �ִ� ��Ƽ�� ��Ż ó���Ѵ�.
			//SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_05 );

			// ���ο� ��Ƽ ������ ���ؼ��� �ڵ� ��Ż ó���Ѵ�.
			KEPM_LEAVE_PARTY_REQ kPacketReq;
			kPacketReq.m_iPartyUID = kPacket_.m_kPartyInfo.m_iPartyUID;
			kPacketReq.m_iReason = NetError::NOT_LEAVE_PARTY_REASON_05;
			_SendToCnParty( kPacket_.m_kPartyInfo.m_iPartyUID, EPM_LEAVE_PARTY_REQ, kPacketReq );

			// ��Ƽ ���� ���� ó��
			KEGS_CREATE_PARTY_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_PARTY_00;
			SendPacket( EGS_CREATE_PARTY_ACK, kPacketAck );
			return;
		}
		else
		{
			START_LOG( cerr, L"�̹� PartyUID�� ����Ǿ��ִµ� �Ȱ��� PartyUID�� �� �����Ϸ��� �ϳ�? ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetPartyUID() )
				<< END_LOG;

			// �̰� ��Żó���� �� �ʿ� ���� ��Ȳ.
		}
	}
	//}}

	//��Ƽ�� ������ �������� UID�� �������ش�.
	SetPartyUID( kPacket_.m_kPartyInfo.m_iPartyUID );

	KEGS_CREATE_PARTY_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_kPartyInfo = kPacket_.m_kPartyInfo;
	kPacket.m_kPartyUserInfo = kPacket_.m_kPartyUserInfo;
	SendPacket( EGS_CREATE_PARTY_ACK, kPacket );

	//������ �̻����� üũ�Ѵ�.
	if( GetAuthLevel() >= SEnum::UAL_DEVELOPER )
	{
		START_LOG( cout, L"������ ��Ƽ ���� ����!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_kPartyInfo.m_iDungeonID );
	}
}

IMPL_ON_FUNC( EPM_JOIN_PARTY_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_JOIN_PARTY_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_JOIN_PARTY_ACK, kPacket );
}

_IMPL_ON_FUNC( EPM_JOIN_PARTY_NOT, KEGS_JOIN_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		for( u_int ui = 0; ui < kPacket_.m_vecPartyInfo.size(); ++ui )
		{
			const KPartyInfo& kInfo = kPacket_.m_vecPartyInfo[ui];

			//{{ 2010. 03. 12  ������	ä�� ���� ��Ƽ ���� ����
			if( GetPartyUID() > 0 )
			{
				if( GetPartyUID() != kInfo.m_iPartyUID )
				{
					START_LOG( cerr, L"�̹� ��Ƽ �Ҽ��ε� �� ��Ƽ ����ó���� �Ǿ���.. ������Ƽ�� ��Ż ��Ű��!" )
						<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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
				else
				{
					START_LOG( cerr, L"�̹� PartyUID�� ����Ǿ��ִµ� �Ȱ��� PartyUID�� �� �����Ϸ��� �ϳ�? ���� �Ͼ���� �ȵǴ� ����!" )
						<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetPartyUID() )
						<< END_LOG;

					// �̰� ��Żó���� �� �ʿ� ���� ��Ȳ.
				}
			}

			// ����� partyuid����
			SetCandidatePartyUID( 0 );
			//}}

			// partyuid�� ����
			SetPartyUID( kInfo.m_iPartyUID );
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

_IMPL_ON_FUNC( EPM_JOIN_PARTY_INVITE_NOT, KEGS_JOIN_PARTY_INVITE_NOT )
{
	//{{ 2010. 03. 12  ������	ä�� ���� ��Ƽ ���� ����	
	// ���� �� ��������Ʈ������ ��Ƽ ���� ������ �޾ƾ���..
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
	//}}

	SendPacket( EGS_JOIN_PARTY_INVITE_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_JOIN_PARTY_INVITE_DENY_NOT, KEGS_JOIN_PARTY_INVITE_DENY_NOT )
{
	//{{ 2010. 03. 12  ������	ä�� ���� ��Ƽ ���� ����	
	// ���� �� ��������Ʈ������ ��Ƽ ���� ������ �޾ƾ���..
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
	//}}

	SendPacket( EGS_JOIN_PARTY_INVITE_DENY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_INVITE_PARTY_DENY_NOT, KEGS_INVITE_PARTY_DENY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_INVITE_PARTY_DENY_NOT, kPacket_ );
}

//{{ 2010. 03. 12  ������	ä�� ���� ��Ƽ ���� ����
IMPL_ON_FUNC( EPM_CHECK_JOIN_PARTY_USER_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEPM_CHECK_JOIN_PARTY_USER_ACK kPacket;

	// �̹� ��Ƽ�� �Ҽӵ� �������� üũ
	if( GetPartyUID() > 0 )
	{
		kPacket.m_iOK = NetError::ERR_PARTY_12;
		_SendToCnParty( kPacket_.m_iPartyUID, EPM_CHECK_JOIN_PARTY_USER_ACK, kPacket );
		return;
	}

	// ��Ƽ ���� ����
	SetCandidatePartyUID( kPacket_.m_iPartyUID );

	// ��Ƽ�� ������ ������ ����
	kPacket.m_iOK = NetError::NET_OK;
	GetPartyUserInfo( kPacket.m_kPartyUserInfo );
	_SendToCnParty( kPacket_.m_iPartyUID, EPM_CHECK_JOIN_PARTY_USER_ACK, kPacket );
}
//}}

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

	// Ŭ���̾�Ʈ�� ACK��Ŷ ����
	KEGS_LEAVE_PARTY_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_LEAVE_PARTY_ACK, kPacket );
}

_IMPL_ON_FUNC( EPM_LEAVE_PARTY_NOT, KEGS_LEAVE_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_LEAVE_PARTY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_INVITE_PARTY_ACK, KEGS_INVITE_PARTY_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	SendPacket( EGS_INVITE_PARTY_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_INVITE_PARTY_NOT, KEGS_INVITE_PARTY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	SendPacket( EGS_INVITE_PARTY_NOT, kPacket_ );
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

	SendPacket( EGS_PARTY_CHANGE_DUNGEON_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_GAME_START_ACK, KEGS_PARTY_GAME_START_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_GAME_START_ACK, kPacket_ );
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
				START_LOG( cwarn, L"��Ƽ��� ���� ���°� �ƴ϶� ������ �����Ҽ� ����." )
					<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
					<< BUILD_LOG( GetStateIDString() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_UNKNOWN;
				SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
				return;
			}
		}
		break;

	case ERM_PARTY_GAME_REPLAY_START_REQ:
		{
			if( GetStateID() != KGSFSM::S_ROOM )
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
		break;

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

	//{{ 2011. 05. 16  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
	UseResurrectionStone( false );

	int ipartynum = 0;
	SiKPartyListManager()->GetPartyNumMember( GetPartyUID(), ipartynum );
	SetStartedNumMember( ipartynum );
#endif SERV_ADD_TITLE_CONDITION
	//}}

	//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
	m_kUserDungeonManager.SetPossibleHenirReward();
#endif SERV_NEW_HENIR_TEST
	//}}

	// PartyRoomUserInfo���
	KPartyRoomUserInfo kPartyRoomUserInfo;
	GetTutorUnitUIDList( kPartyRoomUserInfo.m_vecStudentUnitUID );
	GetRoomUserInfo( kPartyRoomUserInfo.m_kRoomUserInfo, CXSLRoom::RT_DUNGEON );
	//{{ 2009. 6. 10  ������	���� ������ ��� EXP,ED���
	kPartyRoomUserInfo.m_iCurEXP = GetEXP();
	kPartyRoomUserInfo.m_iCurED = GetED();
	//}}
	//{{ 2012. 07. 03	������		���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	kPartyRoomUserInfo.m_wstrChannelIP = NetCommon::GetLocalIPW();
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}

	kPacket.m_iPartyUID = kPacket_.m_iPartyUID;
	kPacket.m_sWorldID = kPacket_.m_sWorldID;
	kPacket.m_usEventID = kPacket_.m_usEventID;
	kPacket.m_vecUserInfo.push_back( kPartyRoomUserInfo );
	SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
}

_IMPL_ON_FUNC( EPM_PARTY_OPEN_DUNGEON_ROOM_NOT, KERM_PARTY_OPEN_DUNGEON_ROOM_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_PARTY_OPEN_DUNGEON_ROOM_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_GAME_REPLAY_START_NOT, KERM_PARTY_OPEN_DUNGEON_ROOM_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_PARTY_GAME_REPLAY_START_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_TO_FIELD_MAP_ACK, KEGS_PARTY_TO_FIELD_MAP_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		SendPacket( EGS_PARTY_TO_FIELD_MAP_ACK, kPacket_ );
		return;
	}

	// ���������� ���� �ʵ�� ��Ż ó�� ����!
	//{{ 2010. 12. 28	������	�ϸ� ���� �߰�
#ifdef SERV_SHIP_DUNGEON
	KEGS_PARTY_TO_FIELD_MAP_REQ kPacket;
	kPacket.m_cPartyToFieldType = kPacket_.m_cPartyToFieldType;
	kPacket.m_iFieldStartPos = kPacket_.m_iFieldStartPos;
	SendToCnRoom( ERM_PARTY_TO_FIELD_MAP_REQ, kPacket );
#else
	SendToCnRoom( ERM_PARTY_TO_FIELD_MAP_REQ );
#endif SERV_SHIP_DUNGEON
	//}}	
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_NAME_ACK, KEGS_PARTY_CHANGE_NAME_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_NAME_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_NAME_NOT, KEGS_PARTY_CHANGE_NAME_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_NAME_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_PUBLIC_ACK, KEGS_PARTY_CHANGE_PUBLIC_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_PUBLIC_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_PUBLIC_NOT, KEGS_PARTY_CHANGE_PUBLIC_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_PUBLIC_NOT, kPacket_ );
}

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

_IMPL_ON_FUNC( EPM_PARTY_QUICK_JOIN_ACK, KEGS_PARTY_QUICK_JOIN_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// ��Ƽ �� ���� ����!
		SetQuickJoinPartyUID( 0 );
		kPacket_.m_iPartyUID = 0;
	}

	SendPacket( EGS_PARTY_QUICK_JOIN_ACK, kPacket_ );
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

IMPL_ON_FUNC( EPM_PARTY_TALKBOX_INFO_UPDATE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ���� �ʵ� �������� �˻�
	if( GetFieldUID() > 0 )
	{
		// �ʵ�� ������Ʈ
		KERM_PARTY_TALKBOX_INFO_UPDATE_NOT kPacketNot;
		kPacketNot.m_bIsDelete = kPacket_.m_bIsDelete;
		kPacketNot.m_vecPartyInfo = kPacket_.m_vecPartyInfo;
		SendToGSField( ERM_PARTY_TALKBOX_INFO_UPDATE_NOT, kPacketNot );
	}
	else
	{
		// ������ �ٷ� ����
		KEGS_PARTY_TALKBOX_INFO_NOT kPacketNot;
		kPacketNot.m_bIsDelete = kPacket_.m_bIsDelete;
		kPacketNot.m_iUnitUID = kPacket_.m_iUnitUID;
		kPacketNot.m_vecPartyInfo = kPacket_.m_vecPartyInfo;
		SendPacket( EGS_PARTY_TALKBOX_INFO_NOT, kPacketNot );
	}
}

IMPL_ON_FUNC_NOPARAM( EPM_QUICK_SLOT_COOL_TIME_RESET_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// ������ �� Ÿ�� ����
	m_kInventory.ResetQuickSlotCoolTime();
}

#endif SERV_CHANNEL_PARTY

//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
IMPL_ON_FUNC( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ UID �̻�.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
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
}
#endif SERV_COME_BACK_USER_REWARD
//}}

//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}

