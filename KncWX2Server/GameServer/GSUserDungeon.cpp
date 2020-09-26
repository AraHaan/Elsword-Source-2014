#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
#include "NexonBillingTCPManager.h"
#include "X2Data/XSLCashItemManager.h"
//{{ 2008. 11. 24  ������	��ŷ �Ŵ���
#include "GSRankingManager.h"
//}}

//{{ 2011. 12. 05	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewPartyListManager.h"
#else
	#include "PartyListManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#include "Enum/Enum.h"

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	#include "./GSWorldMissionManager.h"
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	#include "GSContentManager.h"
#endif SERV_CONTENT_MANAGER
//}}

//{{ 2012. 04. 23	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
#include "../Common/X2Data/XSLBuffManager.h"
#include "UserManager.h"
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX
#include "GameSysVal/GameSysVal.h"
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX

//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	#include "X2Data/XSLDefenceDungeonManager.h"
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
	#include "NewPartyListManager.h"
#endif SERV_RECRUIT_EVENT_SUBQUEST

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp ���� ������ �˴ϴ�!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC( EGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_DUNGEON_PLAY_INFO_TO_SERVER_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_DUNGEON_STAGE_LOAD_REQ )
{
	//{{ 2010. 9. 8	������	�ߺ� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_STAGE_LOAD_REQ, EGS_DUNGEON_STAGE_LOAD_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_STAGE_LOAD_ACK );
#endif SERV_REPEAT_FILTER_REFAC
	//}}	

	//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	SiKPartyListManager()->GetPartyFever( GetPartyUID(), kPacket_.m_sPartyFever );
#else
	SiKPartyListManager()->GetPartyFever( GetPartyUID(), kPacket_.m_cPartyFever );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	SendToCnRoom( ERM_DUNGEON_STAGE_LOAD_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_STAGE_LOAD_ACK, KEGS_DUNGEON_STAGE_LOAD_ACK )
{
	//{{ 2010. 04. 13  ������	���� ����	
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_STAGE_LOAD_ACK );
	//}}

	SendPacket( EGS_DUNGEON_STAGE_LOAD_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_STAGE_LOAD_NOT, KEGS_DUNGEON_STAGE_LOAD_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_STAGE_LOAD_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_DUNGEON_STAGE_LOAD_COMPLETE_REQ )
{
	//{{ 2010. 9. 8	������	�ߺ� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	_VERIFY_STATE_REPEAT_FILTER( cerr, ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_STAGE_LOAD_COMPLETE_REQ, EGS_DUNGEON_STAGE_LOAD_COMPLETE_ACK, KPacketOK );
#else
	_VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_STAGE_LOAD_COMPLETE_ACK, KPacketOK );
#endif SERV_REPEAT_FILTER_REFAC
	//}}	

	SendToCnRoom( ERM_DUNGEON_STAGE_LOAD_COMPLETE_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_STAGE_LOAD_COMPLETE_ACK, KPacketOK )
{
	//{{ 2010. 04. 13  ������	���� ����
	_VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_STAGE_LOAD_COMPLETE_ACK, KPacketOK );
	//}}

	SendPacket( EGS_DUNGEON_STAGE_LOAD_COMPLETE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_STAGE_LOAD_COMPLETE_NOT, KEGS_DUNGEON_STAGE_LOAD_COMPLETE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_STAGE_LOAD_COMPLETE_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_STAGE_START_NOT, KEGS_DUNGEON_STAGE_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_STAGE_START_NOT, kPacket_ );
}

#ifdef SERV_STAGE_CLEAR_IN_SERVER// �۾���¥: 2013-10-30	// �ڼ���
IMPL_ON_FUNC( EGS_DUNGEON_SUB_STAGE_CLEAR_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_DUNGEON_SUB_STAGE_CLEAR_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_CLEAR_ACK, KEGS_DUNGEON_SUB_STAGE_CLEAR_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_SUB_STAGE_CLEAR_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_SECRET_STAGE_LOAD_REQ )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_STAGE_LOAD_ACK );

	SendToCnRoom( ERM_SECRET_STAGE_LOAD_REQ, kPacket_ );
}
_IMPL_ON_FUNC( ERM_SECRET_STAGE_LOAD_ACK, KEGS_DUNGEON_STAGE_LOAD_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	SiKPartyListManager()->GetPartyFever( GetPartyUID(), kPacket_.m_sPartyFever );
#else
	SiKPartyListManager()->GetPartyFever( GetPartyUID(), kPacket_.m_cPartyFever );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

#ifdef SERV_PVP_EQUIPMENT
	UnequipPvpEquipment();
#endif SERV_PVP_EQUIPMENT

	SendToCnRoom( ERM_DUNGEON_STAGE_LOAD_REQ, kPacket_ );
}
#endif // SERV_STAGE_CLEAR_IN_SERVER

IMPL_ON_FUNC( EGS_DUNGEON_SUB_STAGE_OPEN_REQ )
{
	//{{ 2010. 9. 8	������	�ߺ� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_OPEN_REQ, EGS_DUNGEON_SUB_STAGE_OPEN_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_OPEN_ACK );
#endif SERV_REPEAT_FILTER_REFAC
	//}}	

	SendToCnRoom( ERM_DUNGEON_SUB_STAGE_OPEN_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_OPEN_ACK, KEGS_DUNGEON_SUB_STAGE_OPEN_ACK )
{
	//{{ 2010. 04. 13  ������	���� ����
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_OPEN_ACK );
	//}}

	SendPacket( EGS_DUNGEON_SUB_STAGE_OPEN_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_OPEN_NOT, KEGS_DUNGEON_SUB_STAGE_OPEN_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_SUB_STAGE_OPEN_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ )
{
	//{{ 2010. 9. 8	������	�ߺ� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ, EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK );
#endif SERV_REPEAT_FILTER_REFAC
	//}}	

	SendToCnRoom( ERM_DUNGEON_SUB_STAGE_GO_NEXT_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_GO_NEXT_ACK, KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK )
{
	//{{ 2010. 04. 13  ������	���� ����
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK );
	//}}

	SendPacket( EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT, KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ )
{
	//{{ 2010. 9. 6	������	���� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ, EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	SendToCnRoom( ERM_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK, KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK )
{
	//{{ 2010. 03. 17  ������	���� ���� ����
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK );
	//}}

	SendPacket( EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_START_NOT, KEGS_DUNGEON_SUB_STAGE_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_SUB_STAGE_START_NOT, kPacket_ );

	//{{ 2011. 02. 21  ��μ�	Ư�� ���� ���� ������ ���� ����
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
	if( IsReserveKickUser() == true )
	{
		KERM_LEAVE_ROOM_REQ kPacketReq;
		kPacketReq.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_22;
		kPacketReq.m_iItemID = GetDungeonRequiredItemID();
		SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacketReq );

		SetReserveKickUser( false );
		SetDungeonRequiredItemID( 0 );
	}
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
	//}}
}

//{{ 2009. 7. 20  ������	��н�������
IMPL_ON_FUNC_NOPARAM( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ )
{
	//{{ 2010. 9. 8	������	�ߺ� ��Ŷ ���� ����
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ, EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	if( GetPartyUID() <= 0 )
	{
		START_LOG( cerr, L"partyuid���� �̻��ϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( GetRoomUID() )
			<< END_LOG;

		KEGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK kPacket;
		kPacket.m_bCanEnterSecretStage = false;
		SendPacket( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kPacket );
		return;
	}

	//{{ 2010. 03. 02  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_DungeonSecretStageEnterCheck( GetThisPtr<KGSUser>() ) == false )
#else
#pragma ELSE_MESSAGE
	//if( SiKPartyManager()->E_DungeonSecretStageEnterCheck( GetPartyUID(), GetThisPtr<KGSUser>() ) == false )
#endif SERV_CHANNEL_PARTY
	//}}	
	{
		START_LOG( cerr, L"��� �������� ���� ���� üũ ����" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPartyUID() )			
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, KEGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kPacket_ );
}

#ifdef SERV_STAGE_CLEAR_IN_SERVER// �۾���¥: 2013-10-30	// �ڼ���
_IMPL_ON_FUNC( ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT, KEGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT, kPacket_ );
}
#else // SERV_STAGE_CLEAR_IN_SERVER
IMPL_ON_FUNC_NOPARAM( ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendID( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT );
}
#endif // SERV_STAGE_CLEAR_IN_SERVER

IMPL_ON_FUNC( EGS_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_SECRET_STAGE_ENTER_RESULT_NOT, KEGS_DUNGEON_SECRET_STAGE_ENTER_RESULT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_SECRET_STAGE_ENTER_RESULT_NOT, kPacket_ );
}
//}}

_IMPL_ON_FUNC( ERM_END_GAME_DUNGEON_RESULT_DATA_NOT, KEGS_END_GAME_DUNGEON_RESULT_DATA_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

#ifdef SERV_EVENT_CHECK_POWER
	IF_EVENT_ENABLED( CEI_CHECK_POWER )
	{
		bool bSuitableLevelDungeon = false;
		if( kPacket_.m_mapSuitableLevelInfo.find( GetCharUID() ) != kPacket_.m_mapSuitableLevelInfo.end() )
			bSuitableLevelDungeon = kPacket_.m_mapSuitableLevelInfo[ GetCharUID() ];

		BOOST_TEST_FOREACH( KDungeonUnitResultInfo&, kDungeonUnitResultInfo, kPacket_.m_vecDungeonUnitInfo )
		{
			if( kDungeonUnitResultInfo.m_UnitUID == GetCharUID() )
			{
				UpdateCheckPowerScore( kPacket_.m_iDungeonID, kDungeonUnitResultInfo.m_cTotalRank, kPacket_.m_vecDungeonUnitInfo.size(), bSuitableLevelDungeon, kPacket_.m_cDifficulty, kDungeonUnitResultInfo.m_bIsMVP, kPacket_.m_bIsWin );
				break;
			}
		}
	}
#endif SERV_EVENT_CHECK_POWER

	//{{ 2013. 04. 15	�ڼ���	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	if( ( CXSLDungeon::IsDefenceDungeon( kPacket_.m_iDungeonID ) == true ) &&
		( SiKGSWorldMissionManager()->GetIsActive() == true )
		)
	{
		if( kPacket_.m_bIsWin == true )
		{
			KGSPartyPtr partyPtr = SiKPartyListManager()->GetParty( GetPartyUID() );

			// ���� ���� ����
			if( partyPtr == NULL)
			{
				SendToGlobalServer( EGB_CLEAR_MISSION_DUNGEON_NOT, 1 );
			}
			else if( partyPtr != NULL && partyPtr->GetHostUID() == GetCharUID() )
			{
				//	������ ��쿡�� �۷ι������� ����Ѵ�.
				SendToGlobalServer( EGB_CLEAR_MISSION_DUNGEON_NOT, partyPtr->GetNumMember() );
			}
		}

#ifdef SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
#else //SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
		// ǥ�� �뵵�� ��¥ ����
		int iBuffID = CXSLBuffManager::BTI_BUFF_2013_DEFENSE_FAKE_BUFF;

		// ���� �� ������ ����? ������ �߰� �Ѵ�.
		if( m_kUserBuffManager.IsBuffActivated( iBuffID ) == false )
		{
			const int iWorldBuffPriod = SiKGSWorldMissionManager()->GetWorldBuffEndTime();
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
			kReq.m_iUnitUID = GetCharUID();
			ActivateBuff( iBuffID, kReq.m_vecActivateBuff, 0, iWorldBuffPriod );
			SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
		}
#endif //SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
	}
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

	//{{ 2010. 05. 17  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION	
	std::set< UidType > setPartyMembers;
#endif SERV_INTEGRATION	
	//}}

	// ���� Ŭ���� �ߴ°�?
	if( kPacket_.m_bIsWin == false )
	{
		// ���� Ŭ���� �����̹Ƿ� �ƹ��� ó���� ���� �ʴ´�!
		//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
		kPacket_.m_iProtectedSmallCrystal= 0;
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

#ifdef SERV_POINT_COUNT_SYSTEM
		m_kUserQuestManager.SetUpdateQuestInstance(GetThisPtr<KGSUser>());
#endif //SERV_POINT_COUNT_SYSTEM

		goto end_proc;
	}

	// RoomType �˻�
	if( CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_DUNGEON )
	{
		START_LOG( cerr, L"RoomType�� ������ �ƴѵ� �������â ������ �����Դ�?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
			<< END_LOG;

		goto end_proc;
	}

	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	{
		bool bRefreshBuff = false;		// ���� ���� �ؾ��ϴ°�?
		int iBuffID = CXSLBuffManager::BTI_NONE;
		std::vector<KBuffInfo> vecActiveBuff;
		std::vector<int> vecDeleteBuff;
		std::vector<int> vecDeactiveBuff;

		// ��ȯ�ڰ� �ƴѵ� ������ �ִٸ� �����.
		if( m_kComeBackUserInfo.GetIsComeBackUser() == false && kPacket_.m_bComeBackUserInParty == false )
		{
			DeactivateBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO, vecDeactiveBuff );

			bRefreshBuff = true;
		}

		//{{ 2013. 04. 13	�ڼ���	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
#else
		if( kPacket_.m_bIsClearDefenceDungeon == true )
		{
			KGSPartyPtr partyPtr = SiKPartyListManager()->GetParty( GetPartyUID() );

			// ���� ���� ����
			if( partyPtr == NULL)
			{
				KEGB_CLEAR_MISSION_DUNGEON_NOT KNot;
				KNot = kPacket_.m_iProtectedSmallCrystal + 1;
				SendToGlobalServer( EGB_CLEAR_MISSION_DUNGEON_NOT, KNot );
			}
			else if( partyPtr != NULL && partyPtr->GetHostUID() == GetCharUID() )
			{
				//	���潺���� Ŭ���� ������� �۷ι������� ����Ѵ�(������ ��쿡��) ���� ���� + ū ���� 
				KEGB_CLEAR_MISSION_DUNGEON_NOT KNot;
				KNot = kPacket_.m_iProtectedSmallCrystal + 1;
				SendToGlobalServer( EGB_CLEAR_MISSION_DUNGEON_NOT, KNot );
			}

			SiKGSWorldMissionManager()->GetIndividualBuffID( iBuffID, vecDeleteBuff );

			// ���� �� ������ ����? ���� ������ ������ ����� ���� ������ �߰� �Ѵ�.
			if( m_kUserBuffManager.IsBuffActivated( iBuffID ) == false )
			{
				ActivateBuff( iBuffID, vecActiveBuff );

				BOOST_TEST_FOREACH(int, iDeleteBuff, vecDeleteBuff)
				{
					DeactivateBuff( iDeleteBuff, vecDeactiveBuff );
				}

				bRefreshBuff = true;
			}		
		}
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}

		if( bRefreshBuff == true )
		{
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
			kReq.m_iUnitUID = GetCharUID();
			kReq.m_vecActivateBuff = vecActiveBuff;
			kReq.m_vecDeActivateBuff = vecDeactiveBuff;
			SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
		}
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}

	//{{ 2010. 05. 17  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	BOOST_TEST_FOREACH( const KDungeonUnitResultInfo&, kDungeonUnitResultInfo, kPacket_.m_vecDungeonUnitInfo )
	{
		setPartyMembers.insert( kDungeonUnitResultInfo.m_UnitUID );
	}
#endif SERV_INTEGRATION
	//}}

	// �ش� ĳ���� ������ ã�Ƽ� ������Ʈ
	BOOST_TEST_FOREACH( KDungeonUnitResultInfo&, kDungeonUnitResultInfo, kPacket_.m_vecDungeonUnitInfo )
	{
		if( kDungeonUnitResultInfo.m_UnitUID != GetCharUID() )
			continue;

		std::map< int, KSubQuestInfo > mapOngoingQuest;

		//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND		
		kPacket_.GetOngoingQuestByUnitUID( GetCharUID(), mapOngoingQuest );
#endif SERV_QUEST_CLEAR_EXPAND
		//}}

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
		bool bHasFriend = false;

		if( GetPartyUID() > 0 )
		{
			START_LOG( cout, L"��Ƽ�� ����. ��õ�� Ȯ���մϴ�." )
				<< BUILD_LOG( GetPartyUID() )
				<< END_LOG;

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
				START_LOG( cout, L"(��)��õ�� �߰�" )
					<< BUILD_LOG( kRecruitUnitList.m_iUnitUID )
					<< END_LOG;
			}

			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecRecruitUnitUID )
			{
				START_LOG( cout, L"��Ƽ��UID ��" )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;

				if( SiKPartyListManager()->IsPartyMember( GetPartyUID(), iUnitUID ) )
				{
					START_LOG( cout, L"��Ƽ�� �� (��)��õ���� ã��" )
						<< BUILD_LOG( iUnitUID )
						<< END_LOG;
					bHasFriend = true;
					break;
				}
			}
		}
#endif SERV_RECRUIT_EVENT_SUBQUEST

		//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
		m_kUserQuestManager.Handler_OnDungeonClear(
			GetThisPtr<KGSUser>(),
			kPacket_.m_iDungeonID,
			kPacket_.m_cDifficulty,
			kPacket_.m_cDungeonMode,
			kDungeonUnitResultInfo.m_cTotalRank,
			kPacket_.m_nSec,
			kDungeonUnitResultInfo.m_nDamaged,
			setPartyMembers,
			mapOngoingQuest,
			kPacket_.m_mapHaveExpInDungeon
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
			, kPacket_.m_mapSuitableLevelInfo
#endif //SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
#ifdef SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
			, kPacket_.m_bSpecChar1, kPacket_.m_bSpecChar2
#endif //SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
			, kPacket_.m_vecRelation
#endif //SERV_RELATIONSHIP_EVENT_SUBQUEST
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
			, bHasFriend
#endif //SERV_RECRUIT_EVENT_SUBQUEST
#ifdef SERV_THREE_COLOR_EVENT
			, kPacket_.m_setEquippedTitle
#endif SERV_THREE_COLOR_EVENT
			);
		//}}
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		IF_EVENT_ENABLED( CEI_EVENT_COBO_DUNGEON_AND_FIELD )
		{
			///���⼭ ���� Ŭ���� üũ �ؼ� Ŭ�� ����
			DungeonClearCountNot(kPacket_.m_iDungeonID, kPacket_.m_mapSuitableLevelInfo );
		}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		EventDungeonClearCountAdd( kPacket_.m_iDungeonID ); //���� ���̵� �־� ����
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM

		//{{ 2013. 03. 21	 ���� ����Ʈ - ���� ���� ���� Ŭ���� ���� �߰� - ��μ�
#ifdef SERV_ACCOUNT_QUEST_ADD_CONDITION
		m_kUserAccountQuestManager.Handler_OnDungeonClear( GetThisPtr<KGSUser>(), kPacket_.m_iDungeonID, kPacket_.m_cDifficulty, kPacket_.m_cDungeonMode, setPartyMembers, kPacket_.m_mapHaveExpInDungeon, kPacket_.m_mapSuitableLevelInfo );
#endif SERV_ACCOUNT_QUEST_ADD_CONDITION
		//}

		//{{ 2010. 8. 23	������	���� Īȣ
#ifdef SERV_HELL_MODE_TITLE
		m_kUserTitleManager.OnDungeonClear( GetThisPtr<KGSUser>(), kPacket_.m_iDungeonID, kPacket_.m_cDifficulty, kPacket_.m_cDungeonMode, kDungeonUnitResultInfo.m_cTotalRank, kPacket_.m_nSec, kDungeonUnitResultInfo.m_nDamaged, setPartyMembers );
#else
		//{{ 2010. 05. 17  ������	���� ���� ������ ����
		//#ifdef SERV_INTEGRATION
		m_kUserTitleManager.OnDungeonClear( GetThisPtr<KGSUser>(), kPacket_.m_iDungeonID, kPacket_.m_cDifficulty, kDungeonUnitResultInfo.m_cTotalRank, kPacket_.m_nSec, kDungeonUnitResultInfo.m_nDamaged, setPartyMembers );
		//#endif SERV_INTEGRATION
		//}}
#endif SERV_HELL_MODE_TITLE
		//}}

		//Ʃ�丮���̸� ������ �ѹ��� �ֱ�����..
		if( CXSLDungeon::IsTutorialDungeon( kPacket_.m_iDungeonID ) == true )
		{
			std::map< int, KDungeonClearInfo >::iterator mit;
			mit = m_mapDungeonClear.find( kPacket_.m_iDungeonID );

			if( mit != m_mapDungeonClear.end() )
			{
				kDungeonUnitResultInfo.m_nED = 0;
				kDungeonUnitResultInfo.ResetEXP();
			}
		}

		//{{ �ڱ��� : [2010/03/09] //	�߱� �ߵ����� ��ħ�� �ý���
#ifdef SERV_ANTI_ADDICTION_SYSTEM
		START_LOG( clog2, L"��ħ�� �ý���. �����1" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_AntiAddictionManager.GetRewardRatio() )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nED )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nClearBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nRankBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nPartyBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nGameBangBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nPremiumBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nEventEXP )
			<< END_LOG;

		// ���ʽ� ED
		kDungeonUnitResultInfo.m_nED = static_cast<int>(static_cast<float>(kDungeonUnitResultInfo.m_nED) * m_AntiAddictionManager.GetRewardRatio());

		// ���ʽ� ����ġ
		kDungeonUnitResultInfo.m_nEXP = static_cast<int>(static_cast<float>(kDungeonUnitResultInfo.m_nEXP) * m_AntiAddictionManager.GetRewardRatio());
		kDungeonUnitResultInfo.m_nClearBonusEXP = static_cast<int>(static_cast<float>(kDungeonUnitResultInfo.m_nClearBonusEXP) * m_AntiAddictionManager.GetRewardRatio());
		kDungeonUnitResultInfo.m_nRankBonusEXP = static_cast<int>(static_cast<float>(kDungeonUnitResultInfo.m_nRankBonusEXP) * m_AntiAddictionManager.GetRewardRatio());
		kDungeonUnitResultInfo.m_nPartyBonusEXP = static_cast<int>(static_cast<float>(kDungeonUnitResultInfo.m_nPartyBonusEXP) * m_AntiAddictionManager.GetRewardRatio());
		kDungeonUnitResultInfo.m_nGameBangBonusEXP = static_cast<int>(static_cast<float>(kDungeonUnitResultInfo.m_nGameBangBonusEXP) * m_AntiAddictionManager.GetRewardRatio());
		kDungeonUnitResultInfo.m_nPremiumBonusEXP = static_cast<int>(static_cast<float>(kDungeonUnitResultInfo.m_nPremiumBonusEXP) * m_AntiAddictionManager.GetRewardRatio());
		kDungeonUnitResultInfo.m_nEventEXP = static_cast<int>(static_cast<float>(kDungeonUnitResultInfo.m_nEventEXP) * m_AntiAddictionManager.GetRewardRatio());

		START_LOG( clog2, L"��ħ�� �ý���. �����2" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_AntiAddictionManager.GetRewardRatio() )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nED )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nClearBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nRankBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nPartyBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nGameBangBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nPremiumBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nEventEXP )
			<< END_LOG;
#endif SERV_ANTI_ADDICTION_SYSTEM
		//}}

		break;		
	}

end_proc:

	SendPacket( EGS_END_GAME_DUNGEON_RESULT_DATA_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_UPDATE_DUNGEON_UNIT_INFO_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_bDungeonClear == true )
	{
		//////////////////////////////////////////////////////////////////////////
		//Ʃ�丮���� ������ �ѹ��� �ֱ�����..
		if( CXSLDungeon::IsTutorialDungeon( kPacket_.m_iDungeonID ) == true )
		{
			std::map< int, KDungeonClearInfo >::iterator mit;
			mit = m_mapDungeonClear.find( kPacket_.m_iDungeonID );

			if( mit != m_mapDungeonClear.end() )
			{
				kPacket_.m_iED	= 0;
				kPacket_.m_iEXP	= 0;
			}
		}
		//////////////////////////////////////////////////////////////////////////

		//���ӽ����� ù��° ����(�ʽ����� �� �븻) Ŭ����� Ŭ���� ���ʽ� exp +800 �ֱ�����
		//#ù��° ����Ŭ���������� �뼭������ �˼�����(���� �������� ����ȭ�� ����)
		// ù��° ������ �ʽ����� ���� ȥ���÷��̸� �ɼ��ְ� ����Ǳ⶧���� �̰����� �������ش�.
		//if( m_mapDungeonClear.empty() == true &&
		//	kPacket_.m_iDungeonID == SEnum::DI_EL_FOREST_GATE_NORMAL )
		//{
		//	kPacket_.m_iEXP += 800;
		//}		
	}

	//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG	
	m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_DUNGEON_PLAY_TIME, kPacket_.m_iPlayTime / 60 );
#endif SERV_CHAR_LOG
	//}}
	
#ifdef SERV_USER_STATISTICS_RENEWAL
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Dungeon_Play_Time, kPacket_.m_iPlayTime );
#endif //SERV_USER_STATISTICS_RENEWAL

	//{{ 2012. 10. 02	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	// ���ʽ� ED
	IncreaseED( kPacket_.m_iED, KUserEDManager::ER_IN_ED_DUNGEON_CLEAR_BONUS );
#else
	int iBeforeED = m_iED;

	//{{ �ڱ��� : [2010/03/09] //	�߱� �ߵ����� ��ħ�� �ý���
#ifdef SERV_ANTI_ADDICTION_SYSTEM
	if(m_AntiAddictionManager.IsEnable())
	{
		START_LOG( clog, L"��ħ�� �ý���. ����� ������" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_AntiAddictionManager.GetRewardRatio() )
			<< BUILD_LOG( kPacket_.m_iED )
			<< BUILD_LOG( kPacket_.m_iEXP )
			<< BUILD_LOG( m_iED )
			<< BUILD_LOG( m_kEXP.GetExp() )
			<< END_LOG;

		// ���ʽ� ED
		kPacket_.m_iED = static_cast<int>(static_cast<float>(kPacket_.m_iED) * m_AntiAddictionManager.GetRewardRatio());

		// ���ʽ� ����ġ
		kPacket_.m_iEXP = static_cast<int>(static_cast<float>(kPacket_.m_iEXP) * m_AntiAddictionManager.GetRewardRatio());

		START_LOG( clog, L"��ħ�� �ý���. ����� ������" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_AntiAddictionManager.GetRewardRatio() )
			<< BUILD_LOG( kPacket_.m_iED )
			<< BUILD_LOG( kPacket_.m_iEXP )
			<< BUILD_LOG( m_iED )
			<< BUILD_LOG( m_kEXP.GetExp() )
			<< END_LOG;
	}
#endif // SERV_ANTI_ADDICTION_SYSTEM
	//}}

	// ���ʽ� ED
	m_iED += kPacket_.m_iED;

	//{{ 2010. 07. 05  ������	���� ����� �Ŵ���
#ifdef SERV_USER_ABUSER_MANAGER
	// ED �����
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_DUNGEON_REWARD, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
	//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}	

	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( 0 );

	// ED ���Ƽ
	if( kPacket_.m_iED < 0 )
	{
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MDunPenalty, -kPacket_.m_iED );
#ifdef SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_MDunPenalty, kPacket_.m_iED );
#endif //SERV_USER_STATISTICS_RENEWAL
	}
	else
	{
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PDungeon, kPacket_.m_iED );

		// ���� ���
#ifdef SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_PDungeon, kPacket_.m_iED );
#else //SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_PDungeonED, kPacket_.m_iED );
#endif //SERV_USER_STATISTICS_RENEWAL
		
	}

	// ���ʽ� ����ġ
	m_kEXP.AddExp( kPacket_.m_iEXP );

	//Update Level/Stat 
	CheckCharLevelUp();

	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	// ���� ó��
	{
		std::vector< int > vecDeactivateBuff;
		m_kUserBuffManager.OnEndDungeonGame( GetThisPtr<KGSUser>(), kPacket_.m_bDungeonClear, kPacket_.m_bStartedByAutoParty, vecDeactivateBuff );
		if( vecDeactivateBuff.empty() == false )
		{
			// �α��� ���� ���� �Ŵ����� ������Ʈ
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
			kReq.m_iUnitUID = GetCharUID();
			kReq.m_vecDeActivateBuff = vecDeactivateBuff;
			SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
		}
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}}

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
	std::map< int, KDungeonPlayInfo >::iterator mitDungeonPlay = m_mapDungeonPlay.find( kPacket_.m_iDungeonID );
	if( mitDungeonPlay == m_mapDungeonPlay.end() )
	{
		// �ϴ� �� ����� ��ŷ����. ū ������ �ƴϴ� ����Ƚ���� ��������.
		KDungeonPlayInfo kDungeonPlayInfo;
		kDungeonPlayInfo.m_iDungeonID = kPacket_.m_iDungeonID;
		kDungeonPlayInfo.m_iPlayTimes = 1;
		kDungeonPlayInfo.m_iClearTimes = static_cast<int>( kPacket_.m_bDungeonClear );
		kDungeonPlayInfo.m_bNew = true;

		m_mapDungeonPlay.insert( std::make_pair( kDungeonPlayInfo.m_iDungeonID, kDungeonPlayInfo ) );
	}
	else
	{
		mitDungeonPlay->second.m_iClearTimes += static_cast<int>( kPacket_.m_bDungeonClear );
		mitDungeonPlay->second.m_bNew = true;
	}
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

	//{{ 2012. 05. 14	������	���� ���� ���� ����
	//////////////////////////////////////////////////////////////////////////
#ifdef SERV_DUNGEON_NPC_LEVEL
	//////////////////////////////////////////////////////////////////////////
	//Update Dungeon Clear data 
	if( kPacket_.m_bDungeonClear == true )
	{
		std::map< int, KDungeonClearInfo >::iterator mit;
		mit = m_mapDungeonClear.find( kPacket_.m_iDungeonID );
		if( mit == m_mapDungeonClear.end() )
		{
			bool bIsClear = false;

			// Ʃ�丮�� ������ ������ Ŭ����
			if( CXSLDungeon::IsTutorialDungeon( kPacket_.m_iDungeonID ) == true )
			{	
				bIsClear = true;

				KDungeonClearInfo kDCInfo;

				CTime tCurrentTime = CTime::GetCurrentTime();
				kDCInfo.m_iDungeonID	= kPacket_.m_iDungeonID;
				kDCInfo.m_wstrClearTime = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
				kDCInfo.m_iMaxScore		= kPacket_.m_iMaxScore;
				kDCInfo.m_cMaxTotalRank	= kPacket_.m_cMaxTotalRank;
				kDCInfo.m_bNew = true;
				m_mapDungeonClear.insert( std::make_pair( kDCInfo.m_iDungeonID, kDCInfo ) );

				//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
				m_kUserStatistics.SetLastClearDungeonID( kPacket_.m_iDungeonID );
				WriteCharacterLogToDBWithDungeonID( KELOG_USER_CHARACTER_LOG_NOT::LT_DUNGEON_PLAY_COMPLETE, kPacket_.m_iDungeonID );
#endif SERV_CHAR_LOG
				//}}
			}
			else
			{
				// ���� ������ Ŭ���� �ߴ��� �˻�
				const int iDifficulty = (kPacket_.m_iDungeonID % 10);
				const int iDungeonIDWithOutDif = kPacket_.m_iDungeonID - iDifficulty;
				int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( iDungeonIDWithOutDif );			
				if( iRequireDungeonID > 0 )
				{
					std::map<int,KDungeonClearInfo>::iterator mit;
					mit = m_mapDungeonClear.find( iRequireDungeonID );
					if( mit != m_mapDungeonClear.end() )
					{
						bIsClear = true;
					}
				}
				else
				{
					bIsClear = true;
				}

				if( bIsClear == true )
				{
					switch( iDifficulty )
					{
					case CXSLDungeon::DL_EXPERT:
						{
							const int iExpertDungeonID = iDungeonIDWithOutDif + CXSLDungeon::DL_EXPERT;

							if( m_mapDungeonClear.find( iExpertDungeonID ) == m_mapDungeonClear.end() )
							{
								KDungeonClearInfo kDCInfo;
								CTime tCurrentTime = CTime::GetCurrentTime();
								kDCInfo.m_iDungeonID	= iExpertDungeonID;
								kDCInfo.m_wstrClearTime = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
								kDCInfo.m_iMaxScore		= kPacket_.m_iMaxScore;
								kDCInfo.m_cMaxTotalRank	= kPacket_.m_cMaxTotalRank;
								kDCInfo.m_bNew = true;
								m_mapDungeonClear.insert( std::make_pair( kDCInfo.m_iDungeonID, kDCInfo ) );

								//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
								m_kUserStatistics.SetLastClearDungeonID( iExpertDungeonID );
								WriteCharacterLogToDBWithDungeonID( KELOG_USER_CHARACTER_LOG_NOT::LT_DUNGEON_PLAY_COMPLETE, iExpertDungeonID );
#endif SERV_CHAR_LOG
								//}}
							}
						}
						// �ǵ������� break�� ���� �ʾҽ��ϴ�.

					case CXSLDungeon::DL_HARD:
						{
							const int iHardDungeonID = iDungeonIDWithOutDif + CXSLDungeon::DL_HARD;

							if( m_mapDungeonClear.find( iHardDungeonID ) == m_mapDungeonClear.end() )
							{
								KDungeonClearInfo kDCInfo;
								CTime tCurrentTime = CTime::GetCurrentTime();
								kDCInfo.m_iDungeonID	= iHardDungeonID;
								kDCInfo.m_wstrClearTime = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
								kDCInfo.m_iMaxScore		= kPacket_.m_iMaxScore;
								kDCInfo.m_cMaxTotalRank	= kPacket_.m_cMaxTotalRank;
								kDCInfo.m_bNew = true;
								m_mapDungeonClear.insert( std::make_pair( kDCInfo.m_iDungeonID, kDCInfo ) );

								//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
								m_kUserStatistics.SetLastClearDungeonID( iHardDungeonID );
								WriteCharacterLogToDBWithDungeonID( KELOG_USER_CHARACTER_LOG_NOT::LT_DUNGEON_PLAY_COMPLETE, iHardDungeonID );
#endif SERV_CHAR_LOG
								//}}
							}
						}
						// �ǵ������� break�� ���� �ʾҽ��ϴ�.

					case CXSLDungeon::DL_NORMAL:
						{
							const int iNormalDungeonID = iDungeonIDWithOutDif;

							if( m_mapDungeonClear.find( iNormalDungeonID ) == m_mapDungeonClear.end() )
							{
								KDungeonClearInfo kDCInfo;
								CTime tCurrentTime = CTime::GetCurrentTime();
								kDCInfo.m_iDungeonID	= iNormalDungeonID;
								kDCInfo.m_wstrClearTime = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
								kDCInfo.m_iMaxScore		= kPacket_.m_iMaxScore;
								kDCInfo.m_cMaxTotalRank	= kPacket_.m_cMaxTotalRank;
								kDCInfo.m_bNew = true;
								m_mapDungeonClear.insert( std::make_pair( kDCInfo.m_iDungeonID, kDCInfo ) );

								//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
								m_kUserStatistics.SetLastClearDungeonID( iNormalDungeonID );
								WriteCharacterLogToDBWithDungeonID( KELOG_USER_CHARACTER_LOG_NOT::LT_DUNGEON_PLAY_COMPLETE, iNormalDungeonID );
#endif SERV_CHAR_LOG
								//}}
							}
						}
						break;

					default:
						START_LOG( cerr, "���̵� ������ �̻��մϴ�!" )
							<< BUILD_LOG( kPacket_.m_iDungeonID )
							<< BUILD_LOG( iDifficulty )
							<< END_LOG;
						break;
					}					
				}
			}
		}
		//Ŭ�����ߴ� ������ �ִٸ� �ְ����� ����üũ
		else
		{
			if( CXSLDungeon::IsTutorialDungeon( kPacket_.m_iDungeonID ) == false )
			{
				bool bIsChange = false;

				if( mit->second.m_iMaxScore < kPacket_.m_iMaxScore )
				{
					bIsChange = true;
					mit->second.m_iMaxScore = kPacket_.m_iMaxScore;
				}

				if( mit->second.m_cMaxTotalRank < kPacket_.m_cMaxTotalRank )
				{
					bIsChange = true;
					mit->second.m_cMaxTotalRank = kPacket_.m_cMaxTotalRank;
				}

				if( bIsChange == true )
					mit->second.m_bNew = true;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
#else
	//////////////////////////////////////////////////////////////////////////
	//Update Dungeon Clear data 
	if( kPacket_.m_bDungeonClear == true )
	{
		std::map< int, KDungeonClearInfo >::iterator mit;
		mit = m_mapDungeonClear.find( kPacket_.m_iDungeonID );
		if( mit == m_mapDungeonClear.end() )
		{
			bool bIsClear = false;

			// ���� ���̵��� Ŭ�����ߴ��� �˻�
			int iDifficulty = (kPacket_.m_iDungeonID % 10);
			if( iDifficulty == 0 )
			{
				int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( kPacket_.m_iDungeonID );
				//��������� �־�� �Ѵ�.
				if( iRequireDungeonID > 0 )
				{
					std::map<int,KDungeonClearInfo>::iterator mit;
					mit = m_mapDungeonClear.find( iRequireDungeonID );

					if( mit != m_mapDungeonClear.end() )
					{
						bIsClear = true;
					}
				}
				else
				{
					bIsClear = true;
				}
			}
			else if ( 
				(iDifficulty > 0 && m_mapDungeonClear.find( kPacket_.m_iDungeonID - 1 ) != m_mapDungeonClear.end()) ||
				CXSLDungeon::IsTutorialDungeon( kPacket_.m_iDungeonID ) == true 
				)
			{	
				bIsClear = true;
			}

			if( bIsClear == true )
			{
				KDungeonClearInfo kDCInfo;

				CTime tCurrentTime = CTime::GetCurrentTime();
				kDCInfo.m_iDungeonID	= kPacket_.m_iDungeonID;
				kDCInfo.m_wstrClearTime = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
				kDCInfo.m_iMaxScore		= kPacket_.m_iMaxScore;
				kDCInfo.m_cMaxTotalRank	= kPacket_.m_cMaxTotalRank;
				kDCInfo.m_bNew = true;
				m_mapDungeonClear.insert( std::make_pair( kDCInfo.m_iDungeonID, kDCInfo ) );

				//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
				m_kUserStatistics.SetLastClearDungeonID( kPacket_.m_iDungeonID );
				WriteCharacterLogToDBWithDungeonID( KELOG_USER_CHARACTER_LOG_NOT::LT_DUNGEON_PLAY_COMPLETE, kPacket_.m_iDungeonID );
#endif SERV_CHAR_LOG
				//}}
			}
		}
		//Ŭ�����ߴ� ������ �ִٸ� �ְ����� ����üũ
		else
		{
			if( CXSLDungeon::IsTutorialDungeon( kPacket_.m_iDungeonID ) == false )
			{
				bool bIsChange = false;

				if( mit->second.m_iMaxScore < kPacket_.m_iMaxScore )
				{
					bIsChange = true;
					mit->second.m_iMaxScore = kPacket_.m_iMaxScore;
				}

				if( mit->second.m_cMaxTotalRank < kPacket_.m_cMaxTotalRank )
				{
					bIsChange = true;
					mit->second.m_cMaxTotalRank = kPacket_.m_cMaxTotalRank;
				}

				if( bIsChange == true )
					mit->second.m_bNew = true;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
#endif SERV_DUNGEON_NPC_LEVEL
	//////////////////////////////////////////////////////////////////////////
	//}}

#ifndef SERV_HENIR_NO_REWARD
	//{{ 2009. 7. 6  ������		��ŷ���� ������Ʈ
	if( !kPacket_.m_kHenirRankingInfo.empty()  &&  !IsGuestUser() )
	{
		std::vector< KHenirRankingInfo >::const_iterator vitHR = kPacket_.m_kHenirRankingInfo.begin();
		if( vitHR != kPacket_.m_kHenirRankingInfo.end() )
		{
			// ���� ��� ����
			m_kUserRankingManager.SetRankingInfo( *vitHR );

			// 100���ȿ� ��� ��ŷ���� �˻�!
			if( SiKGSRankingManager()->CheckNewRecord( *vitHR ) == true )
			{
				// 100���ȿ� �����!
				KELG_NEW_RECORD_HENIR_RANKING_NOT kNot;
				kNot.m_kNewRecordRanking = *vitHR;
				SendToLoginServer( ELG_NEW_RECORD_HENIR_RANKING_NOT, kNot );
			}
		}
		else
		{
			START_LOG( cerr, L"��ŷ������ �̻��ϴ�? �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_kHenirRankingInfo.size() )
				<< END_LOG;
		}
	}
	//}}
#endif //SERV_HENIR_NO_REWARD

	//{{ 2012. 02. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	if( GetPartyUID() > 0 )
	{
		SendToCnParty( ERM_END_DUNGEON_GAME_PARTY_FEVER_MODIFY_NOT );
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2010. 8. 6	������	�� �ý���
#ifdef SERV_PET_SYSTEM
	m_kUserPetManager.Handler_ERM_UPDATE_DUNGEON_UNIT_INFO_NOT();
#endif SERV_PET_SYSTEM
	//}}

	KEGS_UPDATE_UNIT_INFO_NOT kUpdatePacketNot;

	//�������� ������ �űԾ����ۿ� ���� dbó���� �Ұ����� üũ
	if( !kPacket_.m_mapResultItem.empty() )
	{
		KDBE_INSERT_ITEM_REQ kPacketReq;
		kPacketReq.m_iUnitUID = GetCharUID();
		//{{ 2010. 9. 8	������	������ ȹ�� ����
#ifdef SERV_GET_ITEM_REASON
		kPacketReq.m_cGetItemReason = SEnum::GIR_DUNGEON_RESULT;
#endif SERV_GET_ITEM_REASON
		//}}

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX
		int iEventItemID = SiKGameSysVal()->GetDungeonClearPaymentItemID();

		std::map< int, KItemInfo >::iterator mitEventItemCheck = kPacket_.m_mapResultItem.find( iEventItemID );
		if( mitEventItemCheck != kPacket_.m_mapResultItem.end() )
		{
			int iQuantity = 0;

			if( m_kInventory.GetQuantity( iEventItemID, iQuantity ) == true )   // �κ��丮�� ������ �ֹ����� �ִٸ� �������� �ʴ´�.
			{
				std::map< int, int >::iterator mitGetItem = kPacket_.m_mapGetItem.find( iEventItemID );
				if( mitGetItem != kPacket_.m_mapGetItem.end() )
				{
					kPacket_.m_mapGetItem.erase( mitGetItem );	// ���â���� �������� ������
				}

				kPacket_.m_mapResultItem.erase( mitEventItemCheck );	// ���� ���� ������
			}
			else if( m_kUserPetManager.ExistItemInPetInventory( iEventItemID ) == true ) // �� �κ��丮�� ������ �������� �ʴ´�.
			{
				std::map< int, int >::iterator mitGetItem = kPacket_.m_mapGetItem.find( iEventItemID );
				if( mitGetItem != kPacket_.m_mapGetItem.end() )
				{
					kPacket_.m_mapGetItem.erase( mitGetItem );	// ���â���� �������� ������
				}

				kPacket_.m_mapResultItem.erase( mitEventItemCheck );	// ���� ���� ������
			}
		}
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX

		//{{ 2011. 12.13    ��μ�	���� Ŭ���� �� ������ ���� �̺�Ʈ - ������ �ֹ���(�ߺ� ���� ����)
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT
		std::map< int, KItemInfo >::iterator mitEventItemCheck = kPacket_.m_mapResultItem.find( CXSLItem::EI_CHAR_LEVEL_UP_ITEM );
		if( mitEventItemCheck != kPacket_.m_mapResultItem.end() )
		{
			int iQuantity = 0;
			if( ( m_kInventory.GetQuantity( CXSLItem::EI_CHAR_LEVEL_UP_ITEM, iQuantity ) == true )	||	// �κ��丮��
				( m_kUserPetManager.ExistItemInPetInventory( CXSLItem::EI_CHAR_LEVEL_UP_ITEM ) == true ) // �� �κ��丮�� ������ �ֹ����� �ִٸ� �������� �ʴ´�.
				)
			{
				std::map< int, int >::iterator mitGetItem = kPacket_.m_mapGetItem.find( CXSLItem::EI_CHAR_LEVEL_UP_ITEM );
				if( mitGetItem != kPacket_.m_mapGetItem.end() )
				{
					kPacket_.m_mapGetItem.erase( mitGetItem );	// ���â���� �������� ������
				}

				kPacket_.m_mapResultItem.erase( mitEventItemCheck );	// ���� ���� ������
			}
		}
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT
		//}}
#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
		// ������ ������ ����� ���� ����
		std::map< int, int > mapConsumingItem;

		std::map< int, KItemInfo >::iterator mitResultItem = kPacket_.m_mapResultItem.begin();
		while( mitResultItem != kPacket_.m_mapResultItem.end() )
		{
			int iItemID = mitResultItem->first;
			KItemInfo kItemInfo = mitResultItem->second;

			START_LOG( clog, L"�Ҹ�Ǵ� ������" )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( kItemInfo.m_iQuantity );

			if( kItemInfo.m_iQuantity <= 0 )
			{
				kPacket_.m_mapResultItem.erase( mitResultItem++ );

				if( kItemInfo.m_iQuantity < 0 )
				{
					mapConsumingItem.insert( std::make_pair( iItemID, -kItemInfo.m_iQuantity ) );
				}
			}
			else
			{
				mitResultItem++;
			}
		}

		KEGS_UPDATE_INVENTORY_SLOT_INFO_NOT kPacket;
		m_kInventory.DeleteItemOnlyInventory( mapConsumingItem, kPacket.m_vecInventorySlotInfo, KDeletedItemInfo::DR_DUNGEON_REWARD );
		SendPacket( EGS_UPDATE_INVENTORY_SLOT_INFO_NOT, kPacket );
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM

		m_kInventory.PrepareInsert( kPacket_.m_mapResultItem, kPacketReq.m_mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo );

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
		m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_DUNGEON, kPacketReq.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
		//}}
		//////////////////////////////////////////////////////////////////////////

		// �κ��丮�� ���� ���� �������� �ִ��� �˻�!
		std::map< int, KItemInfo >::const_iterator mitResult;
		for( mitResult = kPacket_.m_mapResultItem.begin(); mitResult != kPacket_.m_mapResultItem.end(); ++mitResult )
		{
			std::map< int, int >::const_iterator mitFind = kPacketReq.m_mapInsertedItem.find( mitResult->first );
			if( mitFind == kPacketReq.m_mapInsertedItem.end() )
			{
				// �κ��丮�� �ϳ��� ���־��� ���̳�!?
				for( int i = 0; i < mitResult->second.m_iQuantity; ++i )
				{
					//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
					AddTempInventoryItem( mitResult->first
										, 0
										//{{ 2012. 1. 16	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
										, mitResult->second
#endif SERV_ENCHANT_ITEM_DROP_EVENT
										//}}
										);
#else
					// �ӽ� �κ��丮�� �����Ϸ� ����!
					KEGS_GET_ITEM_TEMP_INVENTORY_NOT kNot;
					if( m_kTempInventory.AddTempInventoryItem( mitResult->first, 0, ++kPacket_.m_iDropItemUID, kNot.m_kTempInvenItem ) )
					{
						SendPacket( EGS_GET_ITEM_TEMP_INVENTORY_NOT, kNot );
					}
					else
					{
						START_LOG( cerr, L"�ӽ� �κ��丮�� ������ ���� ����!" )
							<< BUILD_LOG( GetCharUID() )
							<< BUILD_LOG( mitResult->first )
							<< END_LOG;
					}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
					//}}
				}
			}
			else
			{
				// ����� �־���?
				int iGetCount = mitResult->second.m_iQuantity - mitFind->second;
				LIF( iGetCount >= 0 );
				for( int i = 0; i < iGetCount; ++i )
				{
					//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
					AddTempInventoryItem( mitResult->first
										, 0
										//{{ 2012. 1. 16	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
										, mitResult->second
#endif SERV_ENCHANT_ITEM_DROP_EVENT
										//}}
										);
#else
					// �ӽ� �κ��丮�� �����Ϸ� ����!
					KEGS_GET_ITEM_TEMP_INVENTORY_NOT kNot;
					if( m_kTempInventory.AddTempInventoryItem( mitResult->first, 0, ++kPacket_.m_iDropItemUID, kNot.m_kTempInvenItem ) )
					{
						SendPacket( EGS_GET_ITEM_TEMP_INVENTORY_NOT, kNot );
					}
					else
					{
						START_LOG( cerr, L"�ӽ� �κ��丮�� ������ ���� ����!" )
							<< BUILD_LOG( GetCharUID() )
							<< BUILD_LOG( mitResult->first )
							<< END_LOG;
					}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
					//}}					
				}
			}
		}

		// DB�� ���������� �ִ°�?
		if( !kPacketReq.m_vecItemInfo.empty() )
		{
			m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
			m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
			kPacketReq.m_bOutRoom = kPacket_.m_bOutRoom;

			//{{ 2011. 05. 04  ��μ�	���� Ŭ����� ������ ���� ���� �߰�
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
			kPacketReq.m_mapGetItem = kPacket_.m_mapGetItem;
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
			//}}

			SendToGameDB( DBE_INSERT_ITEM_REQ, kPacketReq );
			return;
		}
		else
		{
			kUpdatePacketNot.m_vecKInventorySlotInfo = kPacketReq.m_vecUpdatedInventorySlot;
		}
	}

	// Update Server Unit data
	KRoomUserInfo kInfo;
	GetRoomUserInfo( kInfo );

	// �濡�� ������ ��찡 �ƴҰ�� ���ͼ����� ������Ʈ�� �����ش�.
	// ������ ó���� ������ ������Ʈ�� �ؾ��ϱ⶧���� �̰����� ��.
	if( kPacket_.m_bOutRoom == false )
	{
		SendToCnRoom( ERM_SET_UNIT_INFO_NOT, kInfo );
	}

	// �������� ������ �����͸� �Ѱ��ش�.
	GetUnitInfo( kUpdatePacketNot.m_kUnitInfo );
	kUpdatePacketNot.m_bGameEnd = true;
	kUpdatePacketNot.m_mapItemObtained = kPacket_.m_mapGetItem;	

	SendPacket( EGS_UPDATE_UNIT_INFO_NOT, kUpdatePacketNot );
}

IMPL_ON_FUNC( EGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
	
	if( IsHenirDungeonChallengeMode() )
	{
		// ��ϸ� �ð����� �������� ��Ȱ�� ��� �Ұ�!
		START_LOG( cerr, L"Ŭ���̾�Ʈ���� ����������� üũ�����ٵ�.." )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_RESURRECT_03;
		SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacketAck );
		return;
	}

	if( !kPacket_.m_bUseCash )
	{
		//{{ 2009. 10. 14  ������	�ڵ����� ��Ȱ��
#ifdef AP_RESTONE
		if( m_iNumResurrectionStone <= 0  &&  m_iNumAutoPaymentResStone <= 0 )
		{
			KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_RESURRECT_00;
			SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );
			return;
		}
#else
		if( m_iNumResurrectionStone <= 0 )
		{
			KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_RESURRECT_00;
			SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );
			return;
		}
#endif AP_RESTONE		
		//}}		

		kPacket_.m_iUsedByUnitUID = GetCharUID(); // ��Ȱ�� �����
		SendToCnRoom( ERM_RESURRECT_TO_CONTINUE_DUNGEON_REQ, kPacket_ );
	}
	else
	{	
		//{{ 2011. 10. 12	������	������ ������
#ifdef SERV_CONTENT_MANAGER
		if( SiKGSContentManager()->IsEnableCashShop() == false )
		{
			START_LOG( clog, L"ĳ���� ������ ĳ�� ��Ȱ�� ��� ��û" )
				<< BUILD_LOG( GetName() );

			KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_CONTENT_00;
			SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacketAck );
			return;
		}
#endif SERV_CONTENT_MANAGER
		//}}

		// ü��ID ��� ����
		if( IsGuestUser() )
		{
			KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
			SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );
			return;
		}

		// �ǽð� ��Ȱ�� ��� �ð� ����
		if( m_kTimer[TM_CASH_RES_STONE].elapsed() < 2.0 )
			return;

		m_kTimer[TM_CASH_RES_STONE].restart();

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GLOBAL_BILLING

#if defined SERV_COUNTRY_TWHK
		const int ciResurrectionStoneID = 51;
#elif defined SERV_COUNTRY_JP
		const int ciResurrectionStoneID = 1000051;
#elif defined SERV_COUNTRY_CN
		const int ciResurrectionStoneID = 10001;
#elif defined SERV_COUNTRY_BR
		const int ciResurrectionStoneID = 50051;
#else // SERV_COUNTRY_XX
		const int ciResurrectionStoneID = 51;
#endif //SERV_COUNTRY_XX

		KBillProductInfo kBillProductInfo;
		if( !SiKBillManager()->GetProductInfo( ciResurrectionStoneID, kBillProductInfo ) )
		{
			START_LOG( cerr, L"�ǽð� ��Ȱ�� ��ǰ ���� ���� ����." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< END_LOG;

			KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
			SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );
			return;
		}

		KEBILL_BUY_PRODUCT_REQ kPacketReq;
		kPacketReq.m_wstrTotalOrderID = MakeOrderID();	// TotalOrderID�� ���� �߱�
		kPacketReq.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
		kPacketReq.m_wstrUserID = GetName();
		kPacketReq.m_uiPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;
		kPacketReq.m_iUserUID = GetUID();
		kPacketReq.m_iUnitUID = GetCharUID();
		kPacketReq.m_wstrNickName = GetCharName();
		kPacketReq.m_strUserIP = GetIPStr();

		// ����� : [2010/7/29/] //	ĳ�� �α�
		kPacketReq.m_BuyCharInfo.m_cLevel = GetLevel();
		kPacketReq.m_BuyCharInfo.m_iUnitClass = GetUnitClass();

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		kPacketReq.m_iUserIP = GetIP();

#if defined (SERV_COUNTRY_CN) || defined (SERV_COUNTRY_PH)
		kPacketReq.m_iUseCashType = (int)KGlobalCashInfo::GCT_KOG_ELSWORD_CASH;
#elif defined (SERV_COUNTRY_TWHK)
		// �ϴ� UI �۾� �������� GASH�� ��û
		kPacketReq.m_iUseCashType = (int)KGlobalCashInfo::GCT_PUBLISHER_CASH;

// 		// Gash �� �������� ���� Gash�� ��û
// 		if(m_GlobalCashInfo.m_ulCash[KGlobalCashInfo::GCT_PUBLISHER_CASH] > kBillProductInfo.m_iSalePrice)
// 		{
// 			kPacketReq.m_iUseCashType = (int)KGlobalCashInfo::GCT_PUBLISHER_CASH;
// 		}
// 		// Gash ���� ��� ���ҵ� ����Ʈ�� ��û
// 		else
// 		{
// 			kPacketReq.m_iUseCashType = (int)KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH;
// 		}
		
#else // SERV_COUNTRY_XX
		kPacketReq.m_iUseCashType = (int)KGlobalCashInfo::GCT_PUBLISHER_CASH;
#endif // SERV_COUNTRY_CN
		
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

#ifdef SERV_BALANCE_CHEAK_BEFORE_REALTIME_RESSTONE		
		//�ܾ� �˻�
		if( m_GlobalCashInfo.m_ulCash[kPacketReq.m_iUseCashType] < kBillProductInfo.m_iSalePrice )
		{
			START_LOG( cerr, L"�ܾ� ���µ� �ǽð� ��Ȱ�� ��� �ߴ� " )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( m_GlobalCashInfo.m_ulCash[kPacketReq.m_iUseCashType] )
				<< BUILD_LOG( kBillProductInfo.m_iSalePrice )
				<< END_LOG;

			KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_16;
			SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );
			return;
		}
#endif //SERV_BALANCE_CHEAK_BEFORE_REALTIME_RESSTONE

		KBillBuyInfo kInfo;
		kInfo.m_wstrOrderID = MakeOrderID();
		kInfo.m_iProductNo = kBillProductInfo.m_iProductNo;
		kInfo.m_iPoint = kBillProductInfo.m_iSalePrice;
		kInfo.m_iProductID = kBillProductInfo.m_iProductID;
		kInfo.m_usOrderQuantity = 1;
#if defined(SERV_COUNTRY_BR) || defined(SERV_COUNTRY_ID)
		kInfo.m_wstrMemo = kBillProductInfo.m_wstrProductName;		
#endif //SERV_COUNTRY_BR
		kInfo.m_iCategory = ( int )kBillProductInfo.m_cCategoryNo;
		kPacketReq.m_vecBillBuyInfo.push_back( kInfo );

		DWORD dwBillingFlag = KSimLayer::GetKObj()->GetBillingFlag();
		DWORD dwNationFlag = KSimLayer::GetKObj()->GetDwNationFlag();
		DWORD dwPublisherBillingConnectType = KSimLayer::GetKObj()->GetPublisherBillingConnectType();

#if defined (SERV_COUNTRY_CN) || defined (SERV_COUNTRY_PH)
		SendToKOGBillingDB( EBILL_BUY_PRODUCT_REQ, kPacketReq );
#else // SERV_COUNTRY_CN
		if(dwBillingFlag == KSimLayer::BF_INTERNAL)
		{
			SendToKOGBillingDB( EBILL_BUY_PRODUCT_REQ, kPacketReq );
		}
		else if (dwPublisherBillingConnectType == KSimLayer::PBCT_UNUSUAL && dwNationFlag == KSimLayer::NF_TW)
		{
			// �����ؼ� DB�� ������ TCP�� ������ �����ؾ� ��
			if (kPacketReq.m_iUseCashType == KGlobalCashInfo::GCT_PUBLISHER_CASH)
				SendToPublisherBilling( EBILL_BUY_PRODUCT_REQ, kPacketReq );
			else //
				SendToPublisherBillingDB( EBILL_BUY_PRODUCT_REQ, kPacketReq );
		}
		else if(dwPublisherBillingConnectType == KSimLayer::PBCT_DB)
		{
			SendToPublisherBillingDB( EBILL_BUY_PRODUCT_REQ, kPacketReq );
		}
		else
		{
			SendToPublisherBilling( EBILL_BUY_PRODUCT_REQ, kPacketReq );
		}
#endif // SERV_COUNTRY_CN
		// �򿡼� ĳ������
		m_kUserWishList.SetBuyCashItemInWishList( false );

		// �ӽ� ������ ����
		m_kUserCashInventory.SetCashResurrectionStone( true );
		m_kUserCashInventory.SetCashRStoneUnitUID( kPacket_.m_iUnitUID );

#else // SERV_GLOBAL_BILLING
		// ��Ȱ�� �ѵ��� ���� ���ϰ� �Ѵ�.
		KNXBTProductInfo kProductInfo;
		if( !SiKNexonBillingTCPManager()->GetProductInfo( SiCXSLCashItemManager()->GetProductNoResStone(), kProductInfo ) )
		{
			START_LOG( cerr, L"�ǽð� ��Ȱ�� ��ǰ ���� ���� ����." )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;

			KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
			SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );
			return;
		}

		if( m_iNumResurrectionStone + kProductInfo.m_usProductPieces > 2000 )
		{
			KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_18;
			SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );
			return;
		}

		KENX_BT_PURCHASE_ITEM_REQ kPacketReq;
		kPacketReq.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
		kPacketReq.m_bytePacketType = KNexonBillingTCPPacket::PURCHASE_ITEM;
		kPacketReq.m_ulRemoteIP = GetIP();
		kPacketReq.m_byteReason = 1;            // Game Client
		//{{ 2011. 08. 10    ��μ�    ä�θ� SOAP
#ifdef SERV_CHANNELING_SOAP
		if( GetChannelCode() == KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
		{
			GetToonilandID( kPacketReq.m_wstrGameID );
			GetToonilandID( kPacketReq.m_wstrUserID );
			if( kPacketReq.m_wstrGameID.empty() )
			{
				START_LOG( cerr, L"���Ϸ���ID�� ����? ���� ���� ����?" )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( GetName() )
					<< END_LOG;

				KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacketAck;
				kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
				SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacketAck );
				return;
			}
		}
		else
		{
			kPacketReq.m_wstrGameID = GetName();    // �κ��丮 ������ ���� Nexon ID�� ����.
			kPacketReq.m_wstrUserID = GetName();
		}
#else
		kPacketReq.m_wstrGameID = GetName();    // �κ��丮 ������ ���� Nexon ID�� ����.
		kPacketReq.m_wstrUserID = GetName();
#endif SERV_CHANNELING_SOAP
		//}}
		kPacketReq.m_ulUserOID = m_kNexonAccountInfo.m_uiNexonSN;
		kPacketReq.m_wstrUserName = GetName();
		kPacketReq.m_byteUserAge = BYTE( m_kNexonAccountInfo.m_uiAge );
		CTime tCurrent = CTime::GetCurrentTime();
		kPacketReq.m_wstrOrderID = ( CStringW )tCurrent.Format( _T( "%Y%m%d%H%M%S" ) );
		wchar_t wszNumber[32];
#ifdef _CONVERT_VS_2010
		_i64tow_s( KBaseServer::GetKObj()->GetUID(), wszNumber, 32, 10 );
#else
		_i64tow( KBaseServer::GetKObj()->GetUID(), wszNumber, 10 );
#endif _CONVERT_VS_2010
		kPacketReq.m_wstrOrderID += L"_";
		kPacketReq.m_wstrOrderID += wszNumber;
#ifdef _CONVERT_VS_2010
		_i64tow_s( GetUID(), wszNumber, 32, 10 );
#else
		_i64tow( GetUID(), wszNumber, 10 );
#endif _CONVERT_VS_2010
		kPacketReq.m_wstrOrderID += L"_";
		kPacketReq.m_wstrOrderID += wszNumber;
#ifdef _CONVERT_VS_2010
		_i64tow_s( SiKNexonBillingTCPManager()->GetNextPurchaseNo(), wszNumber, 32, 10 );
#else
		_i64tow( SiKNexonBillingTCPManager()->GetNextPurchaseNo(), wszNumber, 10 );
#endif _CONVERT_VS_2010
		kPacketReq.m_wstrOrderID += L"_";
		kPacketReq.m_wstrOrderID += wszNumber;

		START_LOG( clog, L"�ֹ���ȣ" )
			<< BUILD_LOG( kPacketReq.m_wstrOrderID );

		//{{ 2011. 07. 27    ��μ�    ���Ϸ��� ä�θ�
#ifdef SERV_TOONILAND_CHANNELING
		if( GetChannelCode() == KNexonAccountInfo::CE_TOONILAND_ACCOUNT )  // ���Ϸ��� ä�θ� ����
		{
			kPacketReq.m_ulPaymentType = 14001;
		}
		else
		{
			kPacketReq.m_ulPaymentType = 13001;
		}
#else
		kPacketReq.m_ulPaymentType = 13001;
#endif SERV_TOONILAND_CHANNELING
		//}}

		kPacketReq.m_ulTotalAmount = kProductInfo.m_ulSalePrice;

		KNXBTPurchaseReqInfo kPurchaseReqInfo;
		kPurchaseReqInfo.m_ulProductNo	   = SiCXSLCashItemManager()->GetProductNoResStone();
		kPurchaseReqInfo.m_usOrderQuantity = 1;
		kPacketReq.m_vecNXBTPurchaseInfo.push_back( kPurchaseReqInfo );

		// �򿡼� ĳ������
		m_kUserWishList.SetBuyCashItemInWishList( false );

		// �ӽ� ������ ����
		m_kUserCashInventory.SetCashResurrectionStone( true );
		m_kUserCashInventory.SetCashRStoneUnitUID( kPacket_.m_iUnitUID );

		KEventPtr spEvent( new KEvent );
		UidType anTrace[2] = { GetUID(), -1 };
		spEvent->SetData( PI_GS_NX_BILLING_TCP, anTrace, ENX_BT_PURCHASE_ITEM_REQ, kPacketReq );
		SiKNexonBillingTCPManager()->QueueingEvent( spEvent );
#endif // SERV_GLOBAL_BILLING
	}
}

IMPL_ON_FUNC( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK )
{
	// ���������� ��Ȱó�� �Ǿ��µ� ���������� �����? �׷��� ��ȯ���� �Ҹ���� �ʴ´�.
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		//{{ 2009. 10. 14  ������	�ڵ����� ��Ȱ��
#ifdef AP_RESTONE
		if( m_iNumResurrectionStone <= 0  &&  m_iNumAutoPaymentResStone <= 0 )
		{
			START_LOG( cerr, L"��ȯ�� ����. �̹� ������ Ȯ���� �����̱� ������ �̷� ��Ȳ�� �������� �ȵȴ�." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( m_iNumResurrectionStone )
				<< BUILD_LOG( m_iNumAutoPaymentResStone )
				<< END_LOG;
		}
		else
		{
			// ���� �ڵ����� ��Ȱ���� �����ִٸ� �ڵ����� ��Ȱ�� ���� �Ҹ��ŵ�ϴ�.
			if( m_iNumAutoPaymentResStone > 0 )
			{
				m_iNumAutoPaymentResStone -= 1;
			}
			else
			{
				m_iNumResurrectionStone -= 1;
			}

#ifdef GIANT_RESURRECTION_CASHSTONE
			if(m_iNumResurrectionStone == 0)
			{
				KDBE_REALTIME_RESURRECTION_CASHSTONE_NOT kPakcetToGame;
				kPakcetToGame.m_iUnitUID = GetCharUID();
				kPakcetToGame.m_iNumResurrection_CashStone = m_iNumResurrectionStone.GetChangeValue();
				m_iNumResurrectionStone.SetValue( m_iNumResurrectionStone );

				SendToGameDB(DBE_REALTIME_RESURRECTION_CASHSTONE_NOT , kPakcetToGame);
			}
#endif //GIANT_RESURRECTION_CASHSTONE

			// ��Ȱ�� �α�
			KELOG_RESURRECTION_STONE_LOG_NOT kPacketToLog;
			CTime kUsedTime = CTime::GetCurrentTime();
			kPacketToLog.m_UnitUID	   = GetCharUID();
			kPacketToLog.m_iDungeonID  = kPacket_.m_iDungeonID;
			kPacketToLog.m_wstrRegDate = ( CStringW )( kUsedTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
			SendToLogDB( ELOG_RESURRECTION_STONE_LOG_NOT, kPacketToLog );
		}

		//{{ 2011. 05. 16  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
		UseResurrectionStone( true );		// ��Ȱ�� ����� ����
#endif SERV_ADD_TITLE_CONDITION
		//}}
		//{{ 2013. 02. 05  Īȣ ȹ�� ���� �߰�(��Ȱ�� ��� Ƚ��, ���� ����) - ��μ�
#ifdef SERV_ADD_TITLE_CONDITION_SANDER
		AddUseResurrectionStoneCount();
#endif SERV_ADD_TITLE_CONDITION_SANDER
		//}
		kPacket_.m_iNumResurrectionStone = m_iNumResurrectionStone;				// ��Ȱ��
		kPacket_.m_iNumAutoPaymentResStone = m_iNumAutoPaymentResStone;			// �ڵ����� ��Ȱ��
		kPacket_.m_bUseCash = m_kUserCashInventory.IsCashResurrectionStone();	// �ǽð� ��Ȱ�� ����
#else
		if( m_iNumResurrectionStone <= 0 )
		{
			START_LOG( cerr, L"��ȯ�� ����. �̹� ������ Ȯ���� �����̱� ������ �̷� ��Ȳ�� �������� �ȵȴ�." )
				<< BUILD_LOG( m_iNumResurrectionStone )
				<< END_LOG;
		}
		else
		{
			m_iNumResurrectionStone -= 1;

			// ��Ȱ�� �α�
			KELOG_RESURRECTION_STONE_LOG_NOT kPacketToLog;
			CTime kUsedTime = CTime::GetCurrentTime();
			kPacketToLog.m_UnitUID	   = GetCharUID();
			kPacketToLog.m_iDungeonID  = kPacket_.m_iDungeonID;
			kPacketToLog.m_wstrRegDate = ( CStringW )( kUsedTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
			SendToLogDB( ELOG_RESURRECTION_STONE_LOG_NOT, kPacketToLog );			
		}

		kPacket_.m_iNumResurrectionStone = m_iNumResurrectionStone;		
		kPacket_.m_bUseCash = m_kUserCashInventory.IsCashResurrectionStone(); // �ǽð� ��Ȱ��
#endif AP_RESTONE
		//}}

		//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
		// ����� ������ ��Ȱ�ѰŶ�� ���� ������ �ٽ� ����!
		if( CXSLDungeon::IsDefenceDungeon( m_kUserDungeonManager.GetDungeonID() ) == true )
		{
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;

			if( ActivateBuff( CXSLBuffManager::BTI_BUFF_GATE_DEFENCE, kReq.m_vecActivateBuff ) == true )
			{
				kReq.m_iUnitUID = GetCharUID();
				SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
			}
		}
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
	}

	m_kUserCashInventory.SetCashResurrectionStone( false ); // �ʱ�ȭ

	SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_STOP_DUNGEON_CONTINUE_TIME_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_STOP_DUNGEON_CONTINUE_TIME_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_STOP_DUNGEON_CONTINUE_TIME_ACK, KEGS_STOP_DUNGEON_CONTINUE_TIME_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_STOP_DUNGEON_CONTINUE_TIME_ACK, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_SKIP_CONTINUE_DUNGEON_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_SKIP_CONTINUE_DUNGEON_NOT );
}

//{{ 2010. 05. 06  ������	��д��� ����
#ifdef SERV_SECRET_HELL

_IMPL_ON_FUNC( ERM_DUNGEON_EFFECT_REMAINING_TIME_NOT, KEGS_DUNGEON_EFFECT_REMAINING_TIME_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_EFFECT_REMAINING_TIME_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_EFFECT_TIME_OUT_NOT, KEGS_DUNGEON_EFFECT_TIME_OUT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_EFFECT_TIME_OUT_NOT, kPacket_ );
}

#endif SERV_SECRET_HELL
//}}

IMPL_ON_FUNC( ERM_DUNGEON_USER_STATISTICS_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iGamePlayTime < 0 )
		kPacket_.m_iGamePlayTime = 0;

	// ���� �÷��� Ÿ��
	m_kUserStatistics.AddPlayTime( KUserStatistics::PT_TOTAL_PLAY_TIME, kPacket_.m_iGamePlayTime );
	m_kUserStatistics.AddPlayTime( KUserStatistics::PT_DUNGEON_PLAY_TIME, kPacket_.m_iGamePlayTime );
	//{{ 2012. 11. 14	�ڼ���	Field PT �α� �߰�
#ifdef SERV_FIELD_PLAY_TIME_LOG
	m_kUserStatistics.AddPlayTime( KUserStatistics::PT_SPIRIT_PLAY_TIME, kPacket_.m_iGamePlayTime );
#endif SERV_FIELD_PLAY_TIME_LOG
	//}}

	// ���� ��� ����
	switch( kPacket_.m_cGameResult )
	{
	case KERM_DUNGEON_USER_STATISTICS_NOT::UGR_WIN:
		{
			// ���� ��� ���� ��
			if( kPacket_.m_bIsPartyPlay == false )
			{
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_MC_Win, 1 );
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_PT_Win, kPacket_.m_iGamePlayTime );
			}
			else
			{
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_MC_Party_Win, 1 );
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_PT_Party_Win, kPacket_.m_iGamePlayTime );
			}			
		}
		break;

	case KERM_DUNGEON_USER_STATISTICS_NOT::UGR_LOSE:
		{
			// ���� ��� ���� ��
			if( kPacket_.m_bIsPartyPlay == false )
			{			
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_MC_Lose, 1 );
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_PT_Lose, kPacket_.m_iGamePlayTime );
			}
			else
			{
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_MC_Party_Lose, 1 );
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_PT_Party_Lose, kPacket_.m_iGamePlayTime );
			}			
		}
		break;

	case KERM_DUNGEON_USER_STATISTICS_NOT::UGR_DROP:
		{
			// ���� ��� ���� ��Ż
			if( kPacket_.m_bIsPartyPlay == false )
			{
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_MC_Drop, 1 );
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_PT_Drop, kPacket_.m_iGamePlayTime );
			}
			else
			{
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_MC_Party_Drop, 1 );
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_PT_Party_Drop, kPacket_.m_iGamePlayTime );
			}
		}
		break;

	default:
		START_LOG( cerr, L"�̷� ��� Ÿ���� ����." )
			<< BUILD_LOGc( kPacket_.m_cGameResult )
			<< END_LOG;
		break;
	}

	// ���� ��� ���� ��Ȱ��
	if( kPacket_.m_bIsPartyPlay == false )
	{
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_MC_ReStone, kPacket_.m_iUsedResurrectionStone );
	}
	else
	{
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_MC_Party_ReStone, kPacket_.m_iUsedResurrectionStone );
	}
}

IMPL_ON_FUNC( ERM_TC_USER_STATISTICS_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iGamePlayTime < 0 )
		kPacket_.m_iGamePlayTime = 0;

	// ���� �÷��� Ÿ��
	m_kUserStatistics.AddPlayTime( KUserStatistics::PT_TOTAL_PLAY_TIME, kPacket_.m_iGamePlayTime );	

	// ���� ��� �Ʒü�
	switch( kPacket_.m_cGameResult )
	{
	case KERM_TC_USER_STATISTICS_NOT::UGR_SUCCESS:
		{
			// ���� ��� �Ʒü� ����
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_TrainingCenter, kPacket_.m_iTCID, KUserStatistics::US_TC_MC_Success, 1 );
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_TrainingCenter, kPacket_.m_iTCID, KUserStatistics::US_TC_PT_Success, kPacket_.m_iGamePlayTime );
		}
		break;

	case KERM_TC_USER_STATISTICS_NOT::UGR_FAIL:
		{
			// ���� ��� �Ʒü� ����
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_TrainingCenter, kPacket_.m_iTCID, KUserStatistics::US_TC_MC_Fail, 1 );
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_TrainingCenter, kPacket_.m_iTCID, KUserStatistics::US_TC_PT_Fail, kPacket_.m_iGamePlayTime );
		}
		break;

	case KERM_TC_USER_STATISTICS_NOT::UGR_DROP:
		{
			// ���� ��� �Ʒü� ��Ż
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_TrainingCenter, kPacket_.m_iTCID, KUserStatistics::US_TC_MC_Drop, 1 );
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_TrainingCenter, kPacket_.m_iTCID, KUserStatistics::US_TC_PT_Drop, kPacket_.m_iGamePlayTime );
		}
		break;

	default:
		START_LOG( cerr, L"�̷� ��� Ÿ���� ����." )
			<< BUILD_LOGc( kPacket_.m_cGameResult )
			<< END_LOG;
		break;
	}
}

//{{ 2011. 01. 25  ��μ�  ���� Ʈ���� ����(������ ������ - ���� PVP Ȯ��)
#ifdef SERV_WORLD_TRIGGER_RELOCATION
IMPL_ON_FUNC( EGS_WORLD_TRIGGER_RELOCATION_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"�� �����ε� RoomUID�� ����? �̻��ϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_WORLD_TRIGGER_RELOCATION_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_WORLD_TRIGGER_RELOCATION_ACK, kPacket );
		return;
	}

	SendToCnRoom( ECN_WORLD_TRIGGER_RELOCATION_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ECN_WORLD_TRIGGER_RELOCATION_ACK, KEGS_WORLD_TRIGGER_RELOCATION_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_WORLD_TRIGGER_RELOCATION_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ECN_WORLD_TRIGGER_RELOCATION_NOT, KEGS_WORLD_TRIGGER_RELOCATION_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_WORLD_TRIGGER_RELOCATION_NOT, kPacket_ );
}

#endif SERV_WORLD_TRIGGER_RELOCATION
//}}

//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
IMPL_ON_FUNC( EGS_BAD_ATTITUDE_USER_CHECK_INFO_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"�� �����ε� RoomUID�� ����? �̻��ϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return;
	}

	SendToCnRoom( ERM_BAD_ATTITUDE_USER_CHECK_INFO_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_BAD_ATTITUDE_USER_MSG_NOT, KEGS_BAD_ATTITUDE_USER_MSG_NOT )
{
	SendPacket( EGS_BAD_ATTITUDE_USER_MSG_NOT , kPacket_ );
}

_IMPL_ON_FUNC( ERM_BAD_ATTITUDE_USER_NOT, KEGS_BAD_ATTITUDE_USER_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_BAD_ATTITUDE_USER_NOT , kPacket_ );
}

IMPL_ON_FUNC( EGS_FORCED_EXIT_VOTE_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"�� �����ε� RoomUID�� ����? �̻��ϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return;
	}

	SendToCnRoom( ERM_FORCED_EXIT_VOTE_REQ, kPacket_ );
}

IMPL_ON_FUNC( EGS_DEFENCE_DUNGEON_WAVE_ID_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"�� �����ε� RoomUID�� ����? �̻��ϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return;
	}

	SendToCnRoom( ERM_DEFENCE_DUNGEON_WAVE_ID_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_FORCED_EXIT_VOTE_ACK, KEGS_FORCED_EXIT_VOTE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_FORCED_EXIT_VOTE_ACK , kPacket_ );
}

IMPL_ON_FUNC( ERM_BAD_ATTITUDE_USER_LOG_NOT )
{
	SendToLogDB( DBE_BAD_ATTITUDE_USER_LOG_NOT, kPacket_ );	
}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}

//{{ 2013. 04. 09	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
_IMPL_ON_FUNC( ERM_DECISION_ENTER_DEFENCE_DUNGEON_NOT, KEGS_DECISION_ENTER_DEFENCE_DUNGEON_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DECISION_ENTER_DEFENCE_DUNGEON_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_PREPARE_FOR_DEFENCE_DUNGEON_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//////////////////////////////////////////////////////////////////////////
	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"���� �濡 ������ ���°� �ƴմϴ�!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
		return;
	}
	
	if( CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_DUNGEON )
	{
		START_LOG( cerr, L"���� ���� �濡 ������ ���°� �ƴմϴ�!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
			<< END_LOG;
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
	m_kUserDungeonManager.SetDungeonGameInfo( kPacket_.m_kDefenceDungeonInfo );
#endif SERV_REFORM_QUEST
	//}}

	KERM_PREPARE_FOR_DEFENCE_DUNGEON_ACK kPacketAck;
	GetRoomUserInfo( kPacketAck.m_kRoomUserInfo );
	SendToCnRoom( ERM_PREPARE_FOR_DEFENCE_DUNGEON_ACK, kPacketAck );

	//{{ 2013. 04. 22	������	����� �� ���� ���� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	// ����� �� ���� ���θ� Ȯ���ϱ� ���� �˾� ���� ��Ŷ ����
	SendID( EGS_ENTER_DEFENCE_DUNGEON_POPUP_NOT );
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	//}}
}

_IMPL_ON_FUNC( ERM_DEFENCE_DUNGEON_GAME_START_NOT, KEGS_DEFENCE_DUNGEON_GAME_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

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

	//SetRoomUID( kPacket_.m_RoomInfo.m_RoomUID );

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

	//{{ 2009. 10. 8  ������	���
#ifdef GUILD_TEST
	SendUpdateGuildMemberInfo();
#endif GUILD_TEST
	//}}

	//////////////////////////////////////////////////////////////////////////
	// ����� �� ���� ������ ������ ����
	m_kInventory.SetupDefenceDungeonQuickSlotItems( CXSLDefenceDungeonManager::DDP_PHASE_1, kPacket_.m_vecInventorySlotInfo );
    //////////////////////////////////////////////////////////////////////////
	
	SendPacket( EGS_DEFENCE_DUNGEON_GAME_START_NOT, kPacket_ );

	//{{ 2009. 5. 30  ������	�ӽ� �κ�
	ClearTempInventory();
	//}}

	// ������ ���� �ٲ������ GameStat�� ����!
	//KRoomUserInfo kRoomUserInfo;
	//GetRoomUserInfo( kRoomUserInfo );
	//SendToCnRoom( ERM_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT, kRoomUserInfo );

	// ����� �� ���� ������ ����!
	{
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;

		if( ActivateBuff( CXSLBuffManager::BTI_BUFF_GATE_DEFENCE, kReq.m_vecActivateBuff ) == true )
		{
			kReq.m_iUnitUID = GetCharUID();
			SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
		}
	}
}

_IMPL_ON_FUNC( ERM_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT, KEGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

    SendPacket( EGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_CHANGE_DEFENCE_DUNGEON_PHASE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	// ����� �� ������ ����� ����Ǿ��� �ش� ����� �´� ������ ���������� ��������!
	KEGS_UPDATE_INVENTORY_SLOT_INFO_NOT kPacket;
	m_kInventory.SetupDefenceDungeonQuickSlotItems( kPacket_.m_iDefenceDungeonPhase, kPacket.m_vecInventorySlotInfo );
	SendPacket( EGS_UPDATE_INVENTORY_SLOT_INFO_NOT, kPacket );
}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

//{{ 2013. 04. 22	������	����� �� ���� ���� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
IMPL_ON_FUNC( EGS_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//////////////////////////////////////////////////////////////////////////
	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"���� �濡 ������ ���°� �ƴմϴ�!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
		return;
	}

	if( CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_DUNGEON )
	{
		START_LOG( cerr, L"���� ���� �濡 ������ ���°� �ƴմϴ�!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
			<< END_LOG;
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	SendToCnRoom( ERM_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( ERM_LEAVE_ROOM_BEFORE_DEFENCE_DUNGEON_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	// ��Ƽ �Ҽ��̶�� �ڵ����� ��Ż ó�� ����!

#ifdef LOG_PARTY_BREAK // ������1 ����Ű�����,..
	SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_10 );
#else
	SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_02 );
#endif // LOG_PARTY_BREAK 

	// �ӽ� �κ��丮 �ʱ�ȭ
	ClearTempInventory();

	// �� �ʱ�ȭ
	SetRoomUID( 0 );

	// ������ ���� ����
	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	// ������ �̵�!
	SendID( EGS_LEAVE_ROOM_BEFORE_DEFENCE_DUNGEON_START_NOT );
}
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
//}}

//{{ 2013. 02. 01  �̺�Ʈ �߷�Ÿ�� ���� - ��μ�
#ifdef SERV_EVENT_VALENTINE_DUNGEON
IMPL_ON_FUNC_NOPARAM( EGS_SYNC_DUNGEON_TIMER_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"�� �����ε� RoomUID�� ����? �̻��ϴ�." )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		return;
	}

	SendToCnRoom( ERM_SYNC_DUNGEON_TIMER_NOT, char() );
}

_IMPL_ON_FUNC( ERM_SYNC_DUNGEON_TIMER_NOT, KEGS_SYNC_DUNGEON_TIMER_NOT )
{
	SendPacket( EGS_SYNC_DUNGEON_TIMER_NOT, kPacket_ );	
}
#endif SERV_EVENT_VALENTINE_DUNGEON
//}


//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


