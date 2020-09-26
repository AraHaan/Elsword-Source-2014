//{{ 2013. 2. 6	�ڼ���	������ ���� �ڵ�2
//#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	#include <WinSock2.h>	// windows.h redefinition ���� ��ó��
//#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

#include "Room.h"
#include "Room/RoomFSM.h"
#include "NetError.h"
#include "X2Data/XSLRoom.h"
#include "X2Data/XSLDungeonManager.h"

//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
#include "GameSysVal/GameSysVal.h"
#endif SERV_CHOOSE_FASTEST_HOST
//}}

//{{ 2013. 2. 6	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	#include "InteriorUdpSession.h"
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

float KRoom::ms_fPlayTimeNotifyGap = 5.f;

#define CLASS_TYPE KRoom

NiImplementRTTI( KRoom, KCnBaseRoom );

ImplToStringW( KRoom )
{
    return START_TOSTRING_PARENTW( KCnBaseRoom );
}

KRoom::KRoom()
{
	//{{ 2011. 11. 7	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//	if( m_spRoomUserManager == NULL )
//	{
//		START_LOG( cerr, L"RoomUserManager��ü�� �������� �ʾҽ��ϴ�! �����մϴ�!" )
//			<< BUILD_LOG( GetUID() )
//			<< END_LOG;
//	}
//#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	EnableRemainingPlaytime( true );

	m_cGetItemType		= CXSLRoom::DGIT_NONE;
	m_cDungeonMode		= static_cast<char>(CXSLDungeon::DM_INVALID);

#ifdef DUNGEON_ITEM
	m_bIsItemMode = false;
#endif DUNGEON_ITEM

	//{{ 2012. 05. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_usEventIDForTimeCount = 0;
	m_cCurrentTimeCount = 0;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

//#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//	m_bForceHost = false;
//#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef  SERV_CHOOSE_FASTEST_HOST
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    m_bCheckPingScoreExecuted = false;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#endif  SERV_CHOOSE_FASTEST_HOST

}

KRoom::~KRoom()
{
}

void KRoom::Tick()
{
    KCnBaseRoom::Tick();

	CXSLRoom::ROOM_TYPE eRoomType;

    switch( GetStateID() )
    {
    case KRoomFSM::S_INIT:
        break;

    case KRoomFSM::S_CLOSE:
		{
			//{{ 2012. 06. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
			CheckReserveRegroupOldParty(); // Ȥ�� ���� ������ ������ ���� �ȵǾ������ ���⼭ �ѹ� üũ��.
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			eRoomType = static_cast<CXSLRoom::ROOM_TYPE>(m_cRoomType); // �� ������ ���� ����
			m_spRoomUserManager->ZU_CheckEnd();
			CloseRoom();
			//{{ 2010. 03. 14  ������	�� �й� ��� ����
#ifdef SERV_ROOM_COUNT
			SendRoomCountInfo( eRoomType, false );
#endif SERV_ROOM_COUNT
			//}}
		}
        break;

    case KRoomFSM::S_WAIT:
        break;

		//{{ 2012. 05. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	case KRoomFSM::S_TIME_COUNT:
		{
			CheckTimeCount();
		}
		break;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

    case KRoomFSM::S_LOAD:
		{
			CheckGameLoadingTimeOut();
		}
        break;

    case KRoomFSM::S_PLAY:
		{
			CheckRemainingPlayTime();
			CheckGameLoadingTimeOut();
			//{{ 2011. 03. 26  ��μ�	û 1�� �߰� ��ų �߰� - ����
#ifdef SERV_ADD_BUFF_SKILL_INFO
			CheckCaughtBuffSkill();
#endif SERV_ADD_BUFF_SKILL_INFO
			//}}
			//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
			//if( m_bForceHost )
            if ( m_spRoomUserManager->GetForceHostCID() != 0 )
			{
				CheckPingScore();
				m_kTimer[TM_CHECK_CHANGE_HOST].restart();
			}
			else
#endif  SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
            if( m_kTimer[TM_CHECK_CHANGE_HOST].elapsed() >  ( ( m_cRoomType == CXSLRoom::RT_BATTLE_FIELD ) 
                    ? SiKGameSysVal()->GetBattleFieldCheckChangeHostTime()
                    : SiKGameSysVal()->GetCheckChangeHostTime() ) )
#else   SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
			if( m_kTimer[TM_CHECK_CHANGE_HOST].elapsed() > SiKGameSysVal()->GetCheckChangeHostTime() )
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
			{
				CheckPingScore();
				m_kTimer[TM_CHECK_CHANGE_HOST].restart();
			}
#endif SERV_CHOOSE_FASTEST_HOST
			//}
		}
        break;

	case KRoomFSM::S_RESULT:
		break;

		//{{ 2012. 06. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	case KRoomFSM::S_RETURN_TO_FIELD:
		{
			// ���� ���� �õ����� ����� 3�ʰ� ������ ���ƿ��� �ʴ´ٸ� �ش� ����� ���� ��Ż ��Ű�� ��������� ��Ż �õ� ����!
            if( m_kTimer[TM_RETURN_TO_FIELD].elapsed() > 3.0 )
			{
				m_kTimer[TM_RETURN_TO_FIELD].restart();
                
				// �ش� ����� ������Ż!
				LIF( m_spRoomUserManager->LeaveRoom( m_spRoomUserManager->GetSendReturnToFieldUnitUID() ) ); // ---> leave room�ϰ��� ���� ��� ������ ������ �ؾ����� ������?

				// ��Ŷ ����
				KEGS_RESULT_ALL_UNIT_SUCCESS_NOT kNot;
				GetRoomInfo( kNot.m_RoomInfo );
				GetRoomSlotInfo( kNot.m_vecSlot );

				// ù��° ������� ��Ż �õ�!
				UidType iGSUID = 0;
				UidType iNextUnitUID = 0;
				if( m_spRoomUserManager->GetNextReturnToFieldUnitInfo( GetUID(), iGSUID, iNextUnitUID, kNot.m_kReturnToFieldInfo ) == false )
				{
					// ���̻� ��Ż ó���� ������ ������ ���⼭ ��!!
					StateTransition( KRoomFSM::I_TO_WAIT );

					// ���� ��Ƽ ������!
					CheckReserveRegroupOldParty();
					break;
				}

				// ����!
				SendToGSCharacter( iGSUID, iNextUnitUID, ERM_RESULT_ALL_UNIT_SUCCESS_NOT, kNot );
			}
		}
		break;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	case KRoomFSM::S_WAIT_FOR_DEFENCE:
		break;
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}

    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( GetStateID() )
            << BUILD_LOG( GetStateIDString() )
            << END_LOG;
        break;
    }
	
	if( m_cRoomType != CXSLRoom::RT_DUNGEON )
	{
		std::vector<UidType> vecZombieUnit;
		if( m_spRoomUserManager->ZU_Tick( static_cast<CXSLRoom::ROOM_TYPE>(m_cRoomType), vecZombieUnit ) == true )
		{
			DeleteZombie( vecZombieUnit );

			START_LOG( cout2, L"�������� ����.!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( vecZombieUnit.size() )
				<< BUILD_LOGc( m_cRoomType )
				<< END_LOG;
		}
	}

    std::vector< UidType > vecUID;
    m_spRoomUserManager->ExpireTradeRequest( vecUID );
    if( !vecUID.empty() )
    {
		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUID )
        {
            // ���� ��� �������� �ٲ� ������ ������.
            KEGS_CHANGE_TRADE_NOT kNot;
            kNot.m_iUnitUID = iUnitUID;
            if( m_spRoomUserManager->IsInTrade( iUnitUID, kNot.m_bTrade ) )
            {
                BroadCast( ERM_CHANGE_TRADE_NOT, kNot );
            }
            else
            {
                START_LOG( cerr, L"�ŷ� ���� Ȯ�� ����." )
                    << BUILD_LOG( iUnitUID )
                    << END_LOG;
            }

            START_LOG( cout2, L"�ŷ� ��û ����." )
                << BUILD_LOG( iUnitUID )
                << END_LOG;
        }
    }
}

void KRoom::GetRoomInfo( OUT KRoomInfo& kInfo )
{
    kInfo.m_RoomType		= m_cRoomType;
    kInfo.m_RoomUID			= GetUID();
    kInfo.m_uiRoomListID	= m_uiRoomListID;
    kInfo.m_RoomName		= GetName();
    kInfo.m_RoomState		= GetStateID();
    kInfo.m_bPublic			= m_bPublic;
    kInfo.m_bTeamBalance	= m_bTeamBalance;
    kInfo.m_wstrPassword	= m_wstrPassword;
    kInfo.m_MaxSlot			= GetNumOpenSlot();
    kInfo.m_JoinSlot		= GetNumOccupiedSlot();
    kInfo.m_wstrUDPRelayIP	= m_wstrUDPRelayIP;
    kInfo.m_usUDPRelayPort	= m_usUDPRelayPort;
    kInfo.m_PVPGameType		= m_cGameType;
    kInfo.m_WinKillNum		= m_cWinningNumKill;
    kInfo.m_fPlayTime		= GetPlayTimeLimit();
    kInfo.m_WorldID			= m_sWorldID;	
	kInfo.m_ShowPvpMapWorldID = m_sShowPvpMapWorldID;
    kInfo.m_DifficultyLevel	= m_cDifficultyLevel;
    kInfo.m_iDungeonID		= m_iDungeonID;
	kInfo.m_bCanIntrude		= m_bCanIntrude;
	kInfo.m_cGetItemType	= m_cGetItemType;
	kInfo.m_cDungeonMode	= m_cDungeonMode;
#ifdef DUNGEON_ITEM
	kInfo.m_bIsItemMode		= m_bIsItemMode;
#endif DUNGEON_ITEM

#ifdef SERV_PVP_REMATCH
	kInfo.m_mapAllPlayersSelectedMap = m_mapAllPlayersSelectedMap;
#endif SERV_PVP_REMATCH
}

//{{ 2012. 05. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KRoom::TimeCountForStartGame( IN const u_short usEventID )
{
	if( GetStateID() != KRoomFSM::S_WAIT )
	{
		START_LOG( cerr, L"S_WAIT���°� �ƴѵ� �� �Լ��� ȣ��Ǿ���! �̰� �߸��Ȱž�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;

		// �׳� �α׸� ����!
	}

	// � ��Ŷ���� ���۵Ǿ����� ���߿� Ȯ���ϱ� ���� �����Ѵ�!
	m_usEventIDForTimeCount = usEventID;

	// �ִ� ī��Ʈ!
	SetCurrentTimeCount( RE_TIME_COUNT_LIMIT );

	// ���� �̵� �� Ÿ�̸� �ʱ�ȭ!
	StateTransition( KRoomFSM::I_TO_TIME_COUNT );
	m_kTimer[TM_TIME_COUNT].restart();

	// 3�� Ÿ�̸Ӱ� ���۵Ǿ��ٴ� ��Ŷ�� ��������!
	KEGS_GAME_START_TIME_COUNT_NOT kPacketNot;
	kPacketNot.m_cSecondCount = GetCurrentTimeCount();
	kPacketNot.m_bIsDungeonRoom = ( m_cRoomType == CXSLRoom::RT_DUNGEON );
	BroadCast( ERM_GAME_START_TIME_COUNT_NOT, kPacketNot );
}

void KRoom::CheckTimeCount()
{
	if( m_kTimer[TM_TIME_COUNT].elapsed() > 1.0 )
	{
		m_kTimer[TM_TIME_COUNT].restart();

		// 1�� ���� ��Ű��!
		DecreaseCurrentTimeCount();

		// ���̻� ������ ���� ���ٸ� ���⼭ ������!
		if( GetCurrentTimeCount() <= 0 )
		{
			// ���� �̵��ϰ�
			StateTransition( KRoomFSM::I_TO_WAIT );
			
			// �� Ÿ�Կ� ���� ó��!
			TimeCountFinishProcess( m_usEventIDForTimeCount );
            return;
		}

		// 3�� Ÿ�̸Ӱ� ���۵Ǿ��ٴ� ��Ŷ�� ��������!
		KEGS_GAME_START_TIME_COUNT_NOT kPacketNot;
		kPacketNot.m_iRoomUID = GetUID();
		kPacketNot.m_cSecondCount = GetCurrentTimeCount();
#ifdef FIX_DUNGEON_START_STRING
		kPacketNot.m_bIsDungeonRoom = ( m_cRoomType == CXSLRoom::RT_DUNGEON );
#endif //FIX_DUNGEON_START_STRING
		BroadCast( ERM_GAME_START_TIME_COUNT_NOT, kPacketNot );
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

void KRoom::ProcessEvent( const KEventPtr& spEvent_ )
{
	m_spRoomUserManager->ZU_Refresh( spEvent_->m_anTrace[0], spEvent_->m_usEventID );	// 080114. �������� ƼŸ���� �����Ѵ�.

    DWORD dwElapTime = ::GetTickCount();  // 040428. ��Ŷ ó�� �ҿ�ð��� ���Ѵ�.

    switch( spEvent_->m_usEventID )
    {
    CASE( ERM_JOIN_ROOM_REQ );
   _CASE( ERM_BAN_USER_REQ, KEGS_BAN_USER_REQ );
   _CASE( ERM_CHANGE_READY_REQ,	KEGS_CHANGE_READY_REQ );
   _CASE( ERM_CHANGE_PITIN_REQ,	KEGS_CHANGE_PITIN_REQ );
   _CASE( ERM_CHANGE_SLOT_OPEN_REQ, KEGS_CHANGE_SLOT_OPEN_REQ );
   _CASE( ERM_GAME_LOADING_REQ, KEGS_GAME_LOADING_REQ );
   _CASE( ERM_STATE_CHANGE_RESULT_REQ, KEGS_STATE_CHANGE_RESULT_REQ );
   _CASE( ERM_CHAT_REQ, KEGS_CHAT_REQ );
   _CASE( ERM_CHANGE_MOTION_REQ, KEGS_CHANGE_MOTION_REQ );
   _CASE( ERM_CHANGE_PLAY_TIME_LIMIT_REQ, KEGS_CHANGE_PLAY_TIME_LIMIT_REQ );

   _CASE( ERM_SET_UNIT_INFO_NOT, KRoomUserInfo );
    CASE_NOPARAM( ERM_REFRESH_ROOM_REQ );
    CASE( ERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT );
	//{{ 2009. 5. 21  ������	������ �Ҹ� ����
	CASE( ERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT );
	//}}
	//{{ 2012. 10. 17	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
   _CASE( ERM_RESULT_SUCCESS_REQ, KEGS_RESULT_SUCCESS_REQ );
#else
	CASE_NOPARAM( ERM_RESULT_SUCCESS_REQ );
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	//}}

   _CASE( ERM_ADD_ON_STAT_REQ, KEGS_ADD_ON_STAT_REQ );

   _CASE( ERM_REPAIR_ITEM_NOT, KEGS_REPAIR_ITEM_NOT );

    CASE( ERM_REQUEST_TRADE_NOT );
    CASE( ERM_REQUEST_TRADE_REPLY_NOT );

    CASE_NOPARAM( ERM_TRADE_START_NOT );
    CASE_NOPARAM( ERM_TRADE_FINISH_NOT );

	CASE( ERM_SET_QUEST_ITEM_INFO_NOT );

	//{{ 2008. 4. 2  ������  �ټ��� ȸ�� ������
   _CASE( ERM_RESTORE_SPIRIT_NOT, KEGS_RESTORE_SPIRIT_NOT );
	//}}

	//{{ 2009. 5. 8  ������		�ǽð� ������
	CASE( ERM_CHAR_LEVEL_UP_NOT );
	//}}
	//{{ 2009. 9. 23  ������	1������ĳ����
   _CASE( ERM_CHANGE_ROOM_USER_INFO_NOT, KRoomUserInfo );
	//}}

	//{{ 2009. 11. 30  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	CASE( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT );
#endif GUILD_SKILL_TEST
	//}}

#ifdef DUNGEON_ITEM
	CASE_NOPARAM( ERM_CHANGE_PVP_ITEMMODE_REQ );
#endif DUNGEON_ITEM

	//{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
   _CASE( ERM_SEARCH_UNIT_REQ, KELG_SEARCH_UNIT_REQ );
   _CASE( ERM_GET_UNIT_INFO_ACK, KELG_GET_UNIT_INFO_ACK );
#endif SERV_INTEGRATION
	//}}

	//{{ 2010. 8. 3	������	�� �ý���
#ifdef SERV_PET_SYSTEM
	CASE( ERM_SUMMON_PET_NOT );
	CASE( ERM_PET_ACTION_NOT );
   _CASE( ERM_CHANGED_PET_SATIETY_NOT, KEGS_CHANGED_PET_SATIETY_NOT );
   _CASE( ERM_CHANGED_PET_INTIMACY_NOT, KEGS_CHANGED_PET_INTIMACY_NOT );
   _CASE( ERM_CHANGED_PET_SATIETY_INTIMACY_NOT, KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT );
	CASE( ERM_PET_EVOLUTION_NOT );
#endif SERV_PET_SYSTEM
	//}}

	//{{ 2011. 03. 26  ��μ�	û 1�� �߰� ��ų �߰� - ����
#ifdef SERV_ADD_BUFF_SKILL_INFO
	_CASE( ERM_BUFF_SKILL_INFO_CONVEY_REQ, KEGS_BUFF_SKILL_INFO_CONVEY_REQ );
#endif SERV_ADD_BUFF_SKILL_INFO
	//}}
	//{{ 2012. 04. 21	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	_CASE( ERM_UPDATE_BUFF_INFO_IN_ROOM_NOT, KEGS_UPDATE_BUFF_INFO_IN_ROOM_NOT );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}
	//{{ 2012. 05. 31	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	_CASE( ERM_USE_SKILL_BUFF_IN_PLAY_NOT, KEGS_USE_SKILL_BUFF_IN_PLAY_NOT );
	_CASE( ERM_NEXT_RETURN_TO_FIELD_NOT, KERM_COMPLETE_RETURN_TO_FIELD_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	_CASE( ERM_UPDATE_USER_UNIT_BUFF_INFO_NOT, KEGS_UPDATE_USER_UNIT_BUFF_INFO_NOT );
	_CASE( ERM_UPDATE_NPC_UNIT_BUFF_INFO_NOT, KEGS_UPDATE_NPC_UNIT_BUFF_INFO_NOT );
#endif SERV_SERVER_BUFF_SYSTEM
	//}}
	//{{ 2012. 07. 27	������		�����ý��� EXP ���ʽ� ���� ����
#ifdef SERV_TUTO_EXP_BONUS_BUG_FIX
	CASE( ERM_UPDATE_TUTORIAL_INFO_NOT );
#endif SERV_TUTO_EXP_BONUS_BUG_FIX
	//}}
	//{{ 2012. 09. 19   ��μ�   ĳ�ü� �湮 ����
#ifdef SERV_VISIT_CASH_SHOP
	_CASE( ERM_VISIT_CASH_SHOP_NOT, KEGS_VISIT_CASH_SHOP_NOT );
#endif SERV_VISIT_CASH_SHOP
	//}}
	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	_CASE( ERM_UDP_PING_SCORE_NOT, KEGS_UDP_PING_SCORE_NOT );
#endif SERV_CHOOSE_FASTEST_HOST
	//}
	//{{ 2012. 11. 20	�ڼ���	Field Frame �α� �߰�
#ifdef SERV_FIELD_FRAME_LOG
	_CASE( ERM_FIELD_USER_FRAME_INFO_NOT, int );
#endif SERV_FIELD_FRAME_LOG
	//}}

	//{{ 2013. 04. 09	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	_CASE( ERM_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT, KRoomUserInfo );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-04-08
	CASE( ERM_LOCAL_RANKING_WATCH_UNIT_REQ );
	CASE( ERM_LOCAL_RANKING_WATCH_UNIT_ACK );
#endif	// SERV_LOCAL_RANKING_SYSTEM
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	_CASE( ERM_COUPLE_MAKING_SUCCESS_NOT, KERM_COUPLE_MAKING_SUCCESS_NOT );
	CASE_NOPARAM( ERM_BREAK_UP_SUCCESS_NOT );
	_CASE( ERM_RELATIONSHIP_EFFECT_NOT, KEGS_RELATIONSHIP_EFFECT_NOT );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
	_CASE( ERM_SUMMON_RIDING_PET_NOT, KEGS_SUMMON_RIDING_PET_NOT );
	_CASE( ERM_UNSUMMON_RIDING_PET_NOT, KEGS_UNSUMMON_RIDING_PET_NOT );
#endif	// SERV_RIDING_PET_SYSTM

	//{{ 2013. 2. 1	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	_CASE( ERM_FORCE_RELAY_NOT, KEGS_FORCE_RELAY_NOT );
	_CASE( ERM_ADD_UDP_CHECK_KICK_USER_NOT, UidType );
	_CASE( ERM_REMOVE_LANBUG_CHECK_USER_NOT, UidType );
	_CASE( ERM_REMOVE_LANBUG_VERIFY_USER_NOT, UidType );
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

    default:
        ProcessNativeEvent( spEvent_ );
    }

    dwElapTime = ::GetTickCount() - dwElapTime;
    if( dwElapTime > 3000 )
    {
        START_LOG_WITH_NAME( cwarn )
            << BUILD_LOG( spEvent_->GetIDStr() )
            << BUILD_LOG( spEvent_->m_usEventID )
            << BUILD_LOG( dwElapTime );
    }
}

void KRoom::CheckRemainingPlayTime()
{
    if( GetRemainingPlayTime() <= 0.f || m_bEnableRemainingPlayTime == false )
    {
        return;
    }

    if( m_kTimer[TM_PLAY_TIME_NOT].elapsed() >= ms_fPlayTimeNotifyGap )
    {
		//���������� GetRemainingPlayTime()�Լ��� �����ε��Ͽ� �ε��ð��� ���ֱ⶧����
		//SetRemainingPlayTime() �Լ� ȣ��� m_fRemainingPlayTime ������ ����ȣ���Ͽ� ���ش�.
        SetRemainingPlayTime( m_fRemainingPlayTime - ( float )m_kTimer[TM_PLAY_TIME_NOT].elapsed() ); 
        if( GetRemainingPlayTime() <= 0.f )
        {
            START_LOG( clog2, L"�÷��� Ÿ�� �ƿ�." )
                << BUILD_LOG( GetStateID() )
                << BUILD_LOG( GetStateIDString() );
            BroadCastID( ERM_PLAY_TIME_OUT_NOT );
        }
        else
        {
            m_kTimer[TM_PLAY_TIME_NOT].restart();
            BroadCast( ERM_REMAINING_PLAY_TIME_NOT, GetRemainingPlayTime() );
        }
    }
}

void KRoom::CheckGameLoadingTimeOut()
{
    std::map< UidType, UidType > mapUserCIDNGS;
    m_spRoomUserManager->GetLoadingTimeOutUser( mapUserCIDNGS );

    if( !mapUserCIDNGS.empty() )
    {
        HandleTimeOutUser( mapUserCIDNGS );
    }
}

void KRoom::CheckResultSuccess()
{
	if( GetStateID() != KRoomFSM::S_RESULT )
		return;

	//��� ������ ������ ���â Ȯ���� ��������
	if( m_spRoomUserManager->IsAllPlayerSuccessResult() == true )
	{
		EndGame();

		KEGS_RESULT_ALL_UNIT_SUCCESS_NOT kNot;
		GetRoomInfo( kNot.m_RoomInfo );
		GetRoomSlotInfo( kNot.m_vecSlot );

		//���ó������ ������ ���� �濡 ������� �������Ա��� ��Ŷ�� ������.
		BroadCast( ERM_RESULT_ALL_UNIT_SUCCESS_NOT, kNot );

		//������� ó�� �Ǿ����Ƿ� ������ ������Ʈ
		SendRoomListInfo( NetError::ERR_ROOM_15 );

		//{{ 2009. 4. 23  ������	������������Ʈ		
		SendAllPVPUserInfo();
		//}}
	}
}

void KRoom::EnableRemainingPlaytime( bool bEnable )
{
	if( bEnable == true )
	{
		m_bEnableRemainingPlayTime = bEnable;
		m_kTimer[TM_PLAY_TIME_NOT].restart();
	}
	else
	{
		m_bEnableRemainingPlayTime = bEnable;
	}
}

//{{ 2011. 08. 05	������	�����濡 ���� ȥ�� ���ԵǴ� ���� ����
#ifdef SERV_CODE_REFACTORING_20110804
bool KRoom::CheckAllPlayerFinishLoading()
{
	// ��� ������ �ε� �������� not�� ������.
	if( m_spRoomUserManager->IsAllPlayerFinishLoading() )
	{
		BroadCastID( ERM_GAME_LOADING_ALL_UNIT_OK_NOT );

		// ���������� �ʱ�ȭ.
		StartPlay();

		KEGS_PLAY_START_NOT kNot;
		//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
		kNot.m_iDungeonID = GetDungeonIDAndDif();
#endif SERV_CHAR_LOG
		//}}
		kNot.m_RoomState = GetStateID();
		GetRoomSlotInfo( kNot.m_vecSlot );

		// ��� �������� �÷��� ������ �˸�.
		BroadCast( ERM_PLAY_START_NOT, kNot );

		// ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
		SendRoomListInfo( NetError::ERR_ROOM_15 );

		//{{ 2009. 4. 23  ������	������������Ʈ
		SendAllPVPUserInfo();
		//}}
		return true;
	}

	return false;
}
#endif SERV_CODE_REFACTORING_20110804
//}}

void KRoom::StartGame()
{
	//�������� üũ����
	m_spRoomUserManager->ZU_CheckEnd();

    SetRemainingPlayTime( -1.f );
    m_spRoomUserManager->StartGame();
#ifdef SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS// �۾���¥: 2013-06-10	// �ڼ���
	BroadCast( ERM_UPDATE_BUFF_INFO_NOT, char(), KRoomUserManager::UL_ALL );
#endif // SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS

    StateTransition( KRoomFSM::I_TO_LOAD );
    m_kTimer[TM_PLAY].restart();


}

void KRoom::StartPlay()
{
	m_bEndPlay = false;
    SetRemainingPlayTime( GetPlayTimeLimit() );
    m_spRoomUserManager->StartPlay();
    m_kTimer[TM_PLAY_TIME_NOT].restart();
	//{{ 2011. 07. 21	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	m_kTimer[TM_PLAY_WITHOUT_LOADING].restart();
#endif SERV_PVP_NEW_SYSTEM
	//}}
    StateTransition( KRoomFSM::I_TO_PLAY );



}

void KRoom::EndPlay()
{
	//{{ 2013. 2. 7	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	BOOST_TEST_FOREACH( const UidType, iUnitUID, m_setRanBugCheckList )
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_CHECK_CANCLE_NOT, GetUID(), iUnitUID );
	}
	ClearLanBugCheckUser();

	BOOST_TEST_FOREACH( const UidType, iUnitUID, m_setRanBugVerifyList )
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_VERIFY_CANCLE_NOT, GetUID(), iUnitUID );
	}
	ClearLanBugVerifyUser();
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

	KEGS_END_GAME_NOT kPacket;
	kPacket.m_cRoomState = GetStateID();
	kPacket.m_iWinTeam = DecideWinTeam();
	m_spRoomUserManager->GetRoomSlotInfo( kPacket.m_vecSlot );

	//������ ���� �������Դ� ������ �ʴ´�.
	// #1 �����Ŷó���� �Ѿ�� �ʰ� �ϱ�����
	// #2 �濡�ִ���������(PVP ���Խ�) ���ó������ ������ ���� �������� �ֱ�����
	BroadCast( ERM_END_GAME_NOT, kPacket, KRoomUserManager::UL_PLAY );

    m_spRoomUserManager->EndPlay();
    StateTransition( KRoomFSM::I_TO_RESULT );

	SendRoomListInfo( NetError::ERR_ROOM_15 );

	//{{ 2009. 4. 23  ������	������������Ʈ	
	SendAllPVPUserInfo();
	//}}
}

void KRoom::EndGame()
{
	m_spRoomUserManager->EndGame();

	StateTransition( KRoomFSM::I_TO_WAIT );
    m_kTimer[TM_WAIT].restart();

	//�������� üũ����.
	m_spRoomUserManager->ZU_CheckStart();
}

bool KRoom::CheckIfPlayEnd()
{
    return false;
}

bool KRoom::IsAbleToIntrude()
{
    return false;
}

void KRoom::DeleteZombie( std::vector<UidType>& vecZombieUnit )
{
	START_LOG( cout2, L"������ ���� ��������.!" )
		<< BUILD_LOG( vecZombieUnit.size() )
		<< END_LOG;

	std::vector<UidType>::iterator vit;
	for( vit = vecZombieUnit.begin(); vit != vecZombieUnit.end(); ++vit )
	{
		KRoomUserPtr spUser = m_spRoomUserManager->GetUser( *vit );

		if( spUser != NULL )
		{
			KERM_CHECK_INVALID_ZOMBIE_USER_NOT kPacket;
			kPacket.m_iRoomUID = GetUID();

			SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_CHECK_INVALID_ZOMBIE_USER_NOT, kPacket );

			//{{ 2009. 4. 23  ������	������������Ʈ
			SendPVPUserInfo( spUser->GetCID(), KCommunityUserInfo::US_PVP_LOBBY );
			//}}
		}
		else
		{
			START_LOG( cerr, L"������ ������ �̻�.!" )
				<< BUILD_LOG( *vit )
				<< END_LOG;
		}

		if( !m_spRoomUserManager->LeaveRoom( *vit ) )
		{
			START_LOG( cerr, L"�� ������ ����." );

			continue;
		}
	}

	// �濡�� ��� ������ �������� ���� �ݴ´�.
	if( IsEmpty() == true )
	{
		StateTransition( KRoomFSM::I_TO_CLOSE );
	}
	else
	{
		std::vector<UidType>::iterator vit;
		for( vit = vecZombieUnit.begin(); vit != vecZombieUnit.end(); ++vit )
		{
			// ���� ��� �����鿡�� ���� ������ ������.
			KEGS_LEAVE_ROOM_NOT kPacketNot;
			kPacketNot.m_cRoomState	= GetStateID();
			kPacketNot.m_iUnitUID	= *vit;
			kPacketNot.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_02;	//����������� ����.
			GetRoomSlotInfo( kPacketNot.m_vecSlot );
			BroadCast( ERM_LEAVE_ROOM_NOT, kPacketNot );
		}
	}

	// ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
	if( GetStateID() == KRoomFSM::S_CLOSE )
	{
		SendRoomListInfo( NetError::ERR_ROOM_16 );
	}
	else
	{
		SendRoomListInfo( NetError::ERR_ROOM_15 );
	}
}

bool KRoom::IsEmpty( KRoomUserManager::USERLIST_TYPE eType )
{
    return m_spRoomUserManager->GetNumOccupiedSlot( eType ) == 0;
}

bool KRoom::IsFull()
{
    return m_spRoomUserManager->GetNumOpenedSlot() == m_spRoomUserManager->GetNumOccupiedSlot();
}

bool KRoom::IsHost( UidType nCID )
{
    return m_spRoomUserManager->IsHost( nCID );
}

void KRoom::BroadCastID( u_short usEventID, int iOption /*= BC_ALL */)
{
    BroadCast( usEventID, char(), iOption );
}

void KRoom::BroadCastRoomSlotInfo()
{
    BroadCastRoomSlotInfo( ERM_ROOM_SLOT_INFO_NOT );
}

void KRoom::BroadCastRoomSlotInfo( u_short usEventID )
{
    std::vector< KRoomSlotInfo > kPacket;
    GetRoomSlotInfo( kPacket );
    BroadCast( usEventID, kPacket );
}

void KRoom::SendRoomListInfo( IN const int iCode )
{
	// ������ �� ����Ʈ�� �������� �ʽ��ϴ�.
	if( m_cRoomType == CXSLRoom::RT_DUNGEON )
		return;

    KERM_ROOM_LIST_INFO_NOT kPacket;
    kPacket.m_iCode = iCode;
    GetRoomInfo( kPacket.m_kRoomInfo );
    BroadCastAllGSServer( ERM_ROOM_LIST_INFO_NOT, kPacket );
}

//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
#else
void KPvPRoom::SendPVPUserInfo( UidType iUnitUID, char cState /*= KCommunityUserInfo::US_NONE*/ )
{
	if( m_cRoomType != CXSLRoom::RT_PVP )
		return;

	if( cState == KCommunityUserInfo::US_NONE )
	{
		switch( GetStateID() )
		{
		case KRoomFSM::S_INIT:
		case KRoomFSM::S_CLOSE:
			cState = KCommunityUserInfo::US_PVP_LOBBY;
			break;

		case KRoomFSM::S_WAIT:
		case KRoomFSM::S_RESULT: // ���â��.. ���Ƿ�����! ( ���Ƿ� ���ðŴϱ� )
			cState = KCommunityUserInfo::US_PVP_WAIT;
			break;

		case KRoomFSM::S_LOAD:
		case KRoomFSM::S_PLAY:
			cState = KCommunityUserInfo::US_PVP_PLAY;
			break;

		default:
			return;
		}
	}	

	KPVPUserSimpleInfo kInfo;
	kInfo.m_iUnitUID	 = iUnitUID;
	kInfo.m_cState		 = cState;
	kInfo.m_iStateCode	 = 0;
	kInfo.m_iRoomUID	 = GetUID();

	KECN_UPDATE_PVP_USER_INFO_NOT kPacket;
	kPacket.m_vecUserInfo.push_back( kInfo );
	BroadCastAllGSServer( ECN_UPDATE_PVP_USER_INFO_NOT, kPacket );
}

void KPvPRoom::SendAllPVPUserInfo()
{
	if( m_cRoomType != CXSLRoom::RT_PVP )
		return;

	// ��� ������ ���� ����
	KECN_UPDATE_PVP_USER_INFO_NOT kPacket;

	char cState = KCommunityUserInfo::US_NONE;

	switch( GetStateID() )
	{
	case KRoomFSM::S_INIT:
	case KRoomFSM::S_CLOSE:
		{
			cState = KCommunityUserInfo::US_PVP_LOBBY;

			for( int iU = 0; iU < m_spRoomUserManager->GetNumMember(); ++iU )
			{
				KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iU );
				if( spRoomUser == NULL )
				{
					START_LOG( cerr, L"RoomUser�� �������� ����." )
						<< END_LOG;
					continue;
				}

				KPVPUserSimpleInfo kInfo;
				kInfo.m_iUnitUID	 = spRoomUser->GetCID();
				kInfo.m_cState		 = cState;
				kInfo.m_iStateCode	 = 0;
				kInfo.m_iRoomUID	 = GetUID();
				kPacket.m_vecUserInfo.push_back( kInfo );
			}
		}
		break;

	case KRoomFSM::S_WAIT:
	case KRoomFSM::S_RESULT: // ���â��.. ���Ƿ�����! ( ���Ƿ� ���ðŴϱ� )
		{
			cState = KCommunityUserInfo::US_PVP_WAIT;

			for( int iU = 0; iU < m_spRoomUserManager->GetNumMember(); ++iU )
			{
				KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iU );
				if( spRoomUser == NULL )
				{
					START_LOG( cerr, L"RoomUser�� �������� ����." )
						<< END_LOG;
					continue;
				}

				KPVPUserSimpleInfo kInfo;
				kInfo.m_iUnitUID	 = spRoomUser->GetCID();
				kInfo.m_cState		 = cState;
				kInfo.m_iStateCode	 = 0;
				kInfo.m_iRoomUID	 = GetUID();
				kPacket.m_vecUserInfo.push_back( kInfo );
			}
		}
		break;

	case KRoomFSM::S_LOAD:
	case KRoomFSM::S_PLAY:
		{
			cState = KCommunityUserInfo::US_PVP_PLAY;

			for( int iU = 0; iU < m_spRoomUserManager->GetNumMember(); ++iU )
			{
				KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iU );
				if( spRoomUser == NULL )
				{
					START_LOG( cerr, L"RoomUser�� �������� ����." )
						<< END_LOG;
					continue;
				}

				// �������� ������ ���������Ѵ�.
				if( !spRoomUser->IsPlaying() )
					continue;

				KPVPUserSimpleInfo kInfo;
				kInfo.m_iUnitUID	 = spRoomUser->GetCID();
				kInfo.m_cState		 = cState;
				kInfo.m_iStateCode	 = 0;
				kInfo.m_iRoomUID	 = GetUID();
				kPacket.m_vecUserInfo.push_back( kInfo );
			}
		}		
		break;

	default:
		return;
	}

	if( !kPacket.m_vecUserInfo.empty() )
	{
		BroadCastAllGSServer( ECN_UPDATE_PVP_USER_INFO_NOT, kPacket );
	}	
}
#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2011. 03. 26  ��μ�	û 1�� �߰� ��ų �߰� - ����
#ifdef SERV_ADD_BUFF_SKILL_INFO
void KRoom::CheckCaughtBuffSkill()
{
	KEGS_BUFF_SKILL_INFO_CONVEY_NOT KNot;

	if( m_spRoomUserManager->CheckCaughtBuffSkill( KNot ) == true)
	{
		BroadCast( ERM_BUFF_SKILL_INFO_CONVEY_NOT, KNot );
	}
}
#endif SERV_ADD_BUFF_SKILL_INFO
//}}

//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
void KRoom::CheckPingScore()
{
	KRoomUserPtr spHost = m_spRoomUserManager->GetHostUser();
	if( IS_NULL( spHost ) )
		return;

#ifdef SERV_DO_NOT_CHANGE_HOST_AT_DUNGEON// �۾���¥: 2013-05-22	// �ڼ���
	if( m_cRoomType == CXSLRoom::RT_DUNGEON )
	{
		// ���� ������ ��쿡�� ȣ��Ʈ ������ ���� �ʴ´�.
		return;
	}
#endif // SERV_DO_NOT_CHANGE_HOST_AT_DUNGEON

#ifdef  SERV_CHOOSE_FASTEST_HOST
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    m_bCheckPingScoreExecuted = true;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#endif  SERV_CHOOSE_FASTEST_HOST


	//{{ 2013. 03. 06   ���� ���� ���� ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST_NEW
//#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//	KRoomUserPtr spNewHost = m_spRoomUserManager->ChooseBetterHost( m_bForceHost, spHost->GetCID() );
//#else//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	KRoomUserPtr spNewHost = m_spRoomUserManager->ChooseBetterHost( spHost->GetCID() );
//#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
#else
	// ���� ������ �� ���ھ ���ٸ� �ٲ��� ����
	if( 
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
        spHost->IsPlaying() == true && spHost->ZombieAlert_Get() == false &&
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
        spHost->GetPingScore() < KRoomUserManager::PV_HIGH )
	{
		m_spRoomUserManager->ClearPingScore();
		return;
	}
	
	KRoomUserPtr spNewHost = m_spRoomUserManager->ChooseBetterHost( spHost->GetPingScore()
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
        , spHost->IsPlaying(), spHost->ZombieAlert_Get()
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
        );
#endif SERV_CHOOSE_FASTEST_HOST_NEW
	//}
	if( IS_NOT_NULL( spNewHost ) )
	{
//#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
		//if( m_bForceHost && spHost == spNewHost )
		//	return;
        if ( spHost == spNewHost )
        {
            return;
        }
//#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK

		// ������ ���� �Ǿ���!
		spHost->SetHost( false );
		spNewHost->SetHost( true );

		// ���� ���� ��Ŷ ����
		KEGS_CHECK_PING_SCORE_CHANGE_HOST_NOT kNot;
		kNot.m_iOldHostUID = spHost->GetCID();
		kNot.m_iNewHostUID = spNewHost->GetCID();
		BroadCast( ERM_CHECK_PING_SCORE_CHANGE_HOST_NOT, kNot );

#ifdef	SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ// ���볯¥: 2013-05-03
		m_kHostCheckTimer.restart();
#endif	// SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ
	
		START_LOG( clog, L"Host ���� �˸� ��Ŷ ��ε� �߼�!" )
			<< BUILD_LOG( kNot.m_iOldHostUID )
			<< BUILD_LOG( kNot.m_iNewHostUID )
			<< END_LOG;

		//{{ 2012. 11. 20	������		���� ��Ʋ�ʵ� üũ �� ����
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
		// host�� 1������ Ȯ��!
		LIF( m_spRoomUserManager->GetHostCount() == 1 );
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
		//}}
	}

	m_spRoomUserManager->ClearPingScore();
}

#endif SERV_CHOOSE_FASTEST_HOST
//}

void KRoom::_BroadCast( int iFlag_, const KEvent& kEvent_ )
{
    std::map< UidType, std::set<UidType> > mapUserList;
    m_spRoomUserManager->GetUserList( iFlag_, mapUserList );
	m_spRoomUserManager->GetUserList( KRoomUserManager::UL_ALL, mapUserList, KRoomUserManager::UT_OBSERVER );

    // GameServer�� ���� loop
    std::map< UidType, std::set<UidType> >::iterator mit;
    for( mit = mapUserList.begin(); mit != mapUserList.end(); mit++ )
    {
        KEventPtr spEvent( kEvent_.Clone() );
        spEvent->m_anTrace[0] = mit->first;
        spEvent->m_anTrace[1] = -1;

        // User ������ ���� loop
        std::set<UidType>::iterator sit;
        for( sit = mit->second.begin(); sit != mit->second.end(); sit++ )
        {
            LIF( spEvent->m_kDestPerformer.AddUID( *sit ) );
        }

        KncSend( PI_CN_ROOM, GetUID(), spEvent );
    }
}

//{{ 2013. 2. 7	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
void KRoom::OnLeaveRoom( IN const UidType iLAST_SENDER_UID, IN const UidType iFIRST_SENDER_UID, IN const KERM_LEAVE_ROOM_REQ& kPacket_, IN const u_short usEventIDAck )
{
	// �ǵ����� Leave Room ���� ������ ������ �� �� �� �� Leave Room �ϰ� �Ǵ� ��찡 �ִ�.
	KEGS_LEAVE_ROOM_ACK kPacket;
	if( GetStateID() == KRoomFSM::S_INIT || GetStateID() == KRoomFSM::S_CLOSE )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_09;
		SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, usEventIDAck, kPacket );
		return;
	}

	if( IsLanBugCheckUser( iFIRST_SENDER_UID ) == true )
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_CHECK_CANCLE_NOT, GetUID(), iFIRST_SENDER_UID );
		RemoveLanBugCheckUser( iFIRST_SENDER_UID );
	}

	if( IsLanBugVerifyUser( iFIRST_SENDER_UID ) == true )
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_VERIFY_CANCLE_NOT, GetUID(), iFIRST_SENDER_UID );
		RemoveLanBugVerifyUser( iFIRST_SENDER_UID );
	}
}
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

#ifdef	SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ// ���볯¥: 2013-05-03
bool KRoom::HostCheck( void ) const
{
	const float fHostCheckTerm = SiKGameSysVal()->GetHostCheckTerm();

	if( fHostCheckTerm < 0.0f )
	{
		return false;
	}

	return ( fHostCheckTerm <= m_kHostCheckTimer.elapsed() );
}
#endif	// SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ



IMPL_ON_FUNC( ERM_JOIN_ROOM_REQ )
{
    unsigned short usEventID;
    if( kPacket_.m_bQuickJoin )
    {
        usEventID = ERM_QUICK_JOIN_ACK;
    }
    else
    {
        usEventID = ERM_JOIN_ROOM_ACK;
    }	

    KEGS_JOIN_ROOM_ACK kPacket;
	if( !( GetStateID() == KRoomFSM::S_WAIT || GetStateID() == KRoomFSM::S_LOAD || GetStateID() == KRoomFSM::S_PLAY || GetStateID() == KRoomFSM::S_RESULT ) )
    {
        kPacket.m_iOK = NetError::ERR_ROOM_09;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );
        return;
    }

	//gm �̻��̸� ���ͼ����� ip�� �־�����.
	if( kPacket_.m_kRoomUserInfo.m_cAuthLevel >= SEnum::UAL_GM )
	{
		kPacket.m_wstrCNIP = NetCommon::GetLocalIPW();
	}

	//{{ 2012. 12. 5	������		������ ���� ���� ���
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
#else
	//���԰����ϸ� ����´� üũ���� �ʴ´�.
	if( IsAbleToIntrude() == false )
	{
		if( GetStateID() != KRoomFSM::S_WAIT )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_09;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );
			return;
		}
	}
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
	//}}	

	//080425.hoons.������ �ϰ�� üũ�� ���� �ʴ´�.
	if( kPacket_.m_kRoomUserInfo.m_bIsObserver == false )
	{
		//{{ 2012. 12. 5	������		������ ���� ���� ���
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
		//���԰����ϸ� ����´� üũ���� �ʴ´�.
		if( IsAbleToIntrude() == false )
		{
			if( GetStateID() != KRoomFSM::S_WAIT )
			{
				kPacket.m_iOK = NetError::ERR_ROOM_09;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );
				return;
			}
		}
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
		//}}

		// �濡 ������ ������ �ִ��� üũ�Ѵ�.
		if( IsFull() )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_03;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );
			return;
		}

		// ��й��̸� ��й�ȣ�� üũ�Ѵ�.
		if( !m_bPublic )
		{
#ifdef SERV_NEW_PVPROOM_PROCESS
			if( kPacket_.m_bInvitation != true && m_wstrPassword != kPacket_.m_wstrPassword )
#else
			if( m_wstrPassword != kPacket_.m_wstrPassword )
#endif SERV_NEW_PVPROOM_PROCESS
			{
				kPacket.m_iOK = NetError::ERR_ROOM_04;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );
				return;
			}
		}

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// �۾���¥: 2013-10-08	// �ڼ���
		if( IsOfficialMatch() == true )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_53;	// ���� �������� ������ �� �����ϴ�.
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );
			return;
		}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM
	}

    // ������ ������ ���Կ� �ִ´�.

	switch( m_cRoomType )
	{
	case CXSLRoom::RT_PVP:
		if( !m_spRoomUserManager->EnterRoom( kPacket_.m_kRoomUserInfo, true ) )
        {
            START_LOG( cerr, L"�� ���� ����." )
                << BUILD_LOG( FIRST_SENDER_UID )
                << BUILD_LOG( GetUID() )
                << END_LOG;

            kPacket.m_iOK = NetError::ERR_ROOM_30;
            SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );
            return;
        }

		break;
	case CXSLRoom::RT_DUNGEON:
		if( !m_spRoomUserManager->EnterRoom( kPacket_.m_kRoomUserInfo, false ) )
        {
            START_LOG( cerr, L"�� ���� ����." )
                << BUILD_LOG( FIRST_SENDER_UID )
                << BUILD_LOG( GetUID() )
                << END_LOG;

            kPacket.m_iOK = NetError::ERR_ROOM_30;
            SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );

            return;
        }
		else
		{
			//080409.hoons. ���� ����Ʈ�� �����Ѵ�.
			m_spRoomUserManager->SetTutorialUIDList( FIRST_SENDER_UID, kPacket_.m_vecStudentUnitUID );
		}

		break;
	}

    // �� ���� ��ȭ.
    // ����.

    // ������ �������� ack�� ������.
    kPacket.m_iOK = NetError::NET_OK;
    GetRoomInfo( kPacket.m_RoomInfo );
    GetRoomSlotInfo( kPacket.m_vecSlot );
	//������ ������ �׻��ش�. �����ӽ� ��������� ������ �����͵� ������ �ؾ��ϴ�..
	GetRoomSlotInfo( kPacket.m_vecObserverSlot, KRoomUserManager::UT_OBSERVER );
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );

    // ���� ��� �������� NOT�� ������.
    KEGS_JOIN_ROOM_NOT kPacketNot;
	//080425.hoons.������ ���º� ���������� �����ؼ� �����ش�.
	if( kPacket_.m_kRoomUserInfo.m_bIsObserver == false )
		m_spRoomUserManager->GetRoomSlotInfo( FIRST_SENDER_UID, kPacketNot.m_JoinSlot, KRoomUserManager::UT_GAME );
	else
		m_spRoomUserManager->GetRoomSlotInfo( FIRST_SENDER_UID, kPacketNot.m_JoinSlot, KRoomUserManager::UT_OBSERVER );
    BroadCast( ERM_JOIN_ROOM_NOT, kPacketNot );

    // ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
    SendRoomListInfo( NetError::ERR_ROOM_15 );

	//{{ 2009. 4. 23  ������	������������Ʈ
	SendPVPUserInfo( kPacket_.m_kRoomUserInfo.m_nUnitUID );
	//}}
}

_IMPL_ON_FUNC( ERM_BAN_USER_REQ, KEGS_BAN_USER_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_BAN_USER_ACK, KPacketOK );

	kPacket.m_iOK = NetError::NET_OK;

	//{{ 2008. 1. 29  ������  ���� ���� ���
	KRoomUserPtr pkSenderRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	
	//{{ 2012. 11. 28	�ڼ���	������ ��� ����
#ifdef SERV_FIX_OBSERVE_MODE
	if( pkSenderRoomUser == NULL )
	{
		pkSenderRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID, KRoomUserManager::UT_OBSERVER );
	}
#endif SERV_FIX_OBSERVE_MODE
	//}}

	if( pkSenderRoomUser == NULL )
	{
		START_LOG( cerr, L"������ �������� �ʽ��ϴ�." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		
		kPacket.m_iOK = NetError::ERR_ROOM_17;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BAN_USER_ACK, kPacket );
		return;
	}

	// ��� üũ
	char cSenderAuthLevel = pkSenderRoomUser->GetAuthLevel();
	bool bSenderIsHost = pkSenderRoomUser->IsHost();
	if( cSenderAuthLevel < SEnum::UAL_GM  &&  m_spRoomUserManager->IsObserver( FIRST_SENDER_UID ) == false )
	{
		// ��ڰ� �ƴҰ�� �������� üũ�Ѵ�.
		VERIFY_HOST( ERM_BAN_USER_ACK );
	}
	//}}

	UidType nGSUID;
	bool bIsHost = false;

	KRoomUserPtr pkRoomUser = m_spRoomUserManager->GetUser( kPacket_.m_iUnitUID );	
	if( pkRoomUser == NULL )
	{
		START_LOG( cerr, L"�����ų ������ ã�����" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}
	else
	{
		nGSUID = pkRoomUser->GetGSUID();
		bIsHost = pkRoomUser->IsHost(); // ������� ������ �������� ���
	}

    // �� ���� ���� ����.
    if( !m_spRoomUserManager->LeaveRoom( kPacket_.m_iUnitUID ) || pkRoomUser == NULL )
    {
        // ���� ����.
        kPacket.m_iOK = NetError::ERR_SLOT_05;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BAN_USER_ACK, kPacket );
        return;
    }
	else
	{
		if( cSenderAuthLevel >= SEnum::UAL_GM && bSenderIsHost == false )
		{
			START_LOG( cout, L"��ڰ� ������ ������׽��ϴ�." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOGc( cSenderAuthLevel )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( bIsHost );
		}
	}

    // ���� ����ڿ��� not�� ������.
    SendToGSCharacter( nGSUID, kPacket_.m_iUnitUID, ERM_BAN_USER_NOT );

	//{{ 2009. 4. 23  ������	������������Ʈ
	SendPVPUserInfo( kPacket_.m_iUnitUID, KCommunityUserInfo::US_PVP_LOBBY );
	//}}

	//������ ��� ����� ���� ��� ������ ������
	//{{ 2012. 11. 28	�ڼ���	������ ��� ����
#ifdef SERV_FIX_OBSERVE_MODE
	if( ( IsEmpty( KRoomUserManager::UT_GAME ) == false ) || ( IsEmpty( KRoomUserManager::UT_OBSERVER ) == false ) )
#else
	if( IsEmpty() == false )
#endif SERV_FIX_OBSERVE_MODE
		//}}
	{
		// ���忡�� ack�� ������.
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BAN_USER_ACK, kPacket );

		// ���� ��� �����鿡�� ���� ������ ������.
		KEGS_LEAVE_ROOM_NOT kPacketNot;
		kPacketNot.m_cRoomState = GetStateID();
		kPacketNot.m_iUnitUID = kPacket_.m_iUnitUID;
		GetRoomSlotInfo( kPacketNot.m_vecSlot );
		BroadCast( ERM_LEAVE_ROOM_NOT, kPacketNot );

		//�渮��Ʈ ����
		SendRoomListInfo( NetError::ERR_ROOM_15 );
	}
	else
	{
		StateTransition( KRoomFSM::I_TO_CLOSE );	

		//�渮��Ʈ ����.
		SendRoomListInfo( NetError::ERR_ROOM_16 );
	}
}

_IMPL_ON_FUNC( ERM_CHANGE_READY_REQ, KEGS_CHANGE_READY_REQ )
{
	//{{ 2010. 02. 10  ������	�α� ���� ����
	VERIFY_ROOM_STATE_WARN( ( 1, KRoomFSM::S_WAIT ), ERM_CHANGE_READY_ACK, KERM_CHANGE_READY_ACK );
	//}}
	
	if( m_spRoomUserManager->SetReady( FIRST_SENDER_UID, kPacket_.m_bReady ) == false )
	{
		kPacket.m_iOK = NetError::ERR_SLOT_03;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_READY_ACK, kPacket );
		return;
	}

	// ��û�� �������� ack�� ������.
	kPacket.m_iDungeonID	= m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
	kPacket.m_cDungeonMode	= m_cDungeonMode;
	kPacket.m_iOK			= NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_READY_ACK, kPacket );

	// ���� ��� �������� ���� ������ ������.
	KEGS_CHANGE_READY_NOT kNot;
	kNot.m_UnitUID	= FIRST_SENDER_UID;
	kNot.m_bReady	= kPacket_.m_bReady;

	BroadCast( ERM_CHANGE_READY_NOT, kNot );
}

_IMPL_ON_FUNC( ERM_CHANGE_PITIN_REQ,	KEGS_CHANGE_PITIN_REQ )
{
	VERIFY_ROOM_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ), ERM_CHANGE_PITIN_ACK, KEGS_CHANGE_PITIN_ACK );

	if( m_spRoomUserManager->SetPitIn( FIRST_SENDER_UID, kPacket_.m_bPitIn ) == false )
	{
		kPacket.m_iOK = NetError::ERR_SLOT_09;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_PITIN_ACK, kPacket );
		return;
	}

	kPacket.m_iOK			  = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_PITIN_ACK, kPacket );

	// ���� ��� �������� ���� ������ ������.
	KEGS_CHANGE_PITIN_NOT kNot;
	kNot.m_UnitUID	= FIRST_SENDER_UID;
	kNot.m_bPitIn	= kPacket_.m_bPitIn;

	BroadCast( ERM_CHANGE_PITIN_NOT, kNot );
}

_IMPL_ON_FUNC( ERM_CHANGE_SLOT_OPEN_REQ, KEGS_CHANGE_SLOT_OPEN_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_CHANGE_SLOT_OPEN_ACK, KEGS_CHANGE_SLOT_OPEN_ACK );

	if( m_spRoomUserManager->IsObserver( FIRST_SENDER_UID ) == false )
	{
		// �������� üũ�Ѵ�.
		VERIFY_HOST( ERM_CHANGE_SLOT_OPEN_ACK );
	}

	bool	bValue = true;
	int		nTeamBalanceSlotIndex = -1;
    switch( kPacket_.m_SlotState )
    {
    case CXSLRoom::SS_EMPTY:

		if( m_cRoomType == CXSLRoom::RT_PVP )
		{
			switch( m_cGameType )
			{
			case CXSLRoom::PGT_TEAM_DEATH:
			case CXSLRoom::PGT_TEAM:
				bValue = m_spRoomUserManager->OpenSlotTeam( kPacket_.m_SlotIndex, nTeamBalanceSlotIndex );
				break;

			default:
				bValue = m_spRoomUserManager->OpenSlot( kPacket_.m_SlotIndex );
			}
		}
		else
		{
			bValue = m_spRoomUserManager->OpenSlot( kPacket_.m_SlotIndex );
		}

        if( bValue == false )
        {
            kPacket.m_iOK = NetError::ERR_SLOT_02;
            SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_SLOT_OPEN_ACK, kPacket );
            return;
        }
        break;

    case CXSLRoom::SS_CLOSE:
		if( m_cRoomType == CXSLRoom::RT_PVP )
		{
			switch( m_cGameType )
			{
			case CXSLRoom::PGT_TEAM_DEATH:
			case CXSLRoom::PGT_TEAM:
				bValue = m_spRoomUserManager->CloseSlotTeam( kPacket_.m_SlotIndex, nTeamBalanceSlotIndex );
				break;

			default:
				bValue = m_spRoomUserManager->CloseSlot( kPacket_.m_SlotIndex );
			}
		}
		else
		{
			bValue = m_spRoomUserManager->CloseSlot( kPacket_.m_SlotIndex );
		}

		if( bValue == false )
        {
            kPacket.m_iOK = NetError::ERR_SLOT_02;
            SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_SLOT_OPEN_ACK, kPacket );
            return;
        }
        break;

    default:
        kPacket.m_iOK = NetError::ERR_SLOT_02;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_SLOT_OPEN_ACK, kPacket );
        return;
    }

    kPacket.m_iOK = NetError::NET_OK;
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_SLOT_OPEN_ACK, kPacket );

	KEGS_CHANGE_SLOT_OPEN_NOT kPacketNot;
	kPacketNot.m_SlotIndex				= kPacket_.m_SlotIndex;
	kPacketNot.m_SlotState				= kPacket_.m_SlotState;
	kPacketNot.m_TeamBalanceSlotIndex	= nTeamBalanceSlotIndex;
    BroadCast( ERM_CHANGE_SLOT_OPEN_NOT, kPacketNot );

	// ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
	SendRoomListInfo( NetError::ERR_ROOM_15 );

	START_LOG( clog, L"::: Change Slot ::: " )
		<< BUILD_LOGc( kPacketNot.m_SlotState )
		<< BUILD_LOGc( kPacketNot.m_SlotIndex )
		<< BUILD_LOGc( kPacketNot.m_TeamBalanceSlotIndex )
		;
}

_IMPL_ON_FUNC( ERM_GAME_LOADING_REQ, KEGS_GAME_LOADING_REQ )
{
	VERIFY_ROOM_STATE( ( 2, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY ), ERM_GAME_LOADING_ACK, KPacketOK );

	// ���Խ� �ε� ��û ����.
	if( GetStateID() == KRoomFSM::S_PLAY )
	{
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_LOADING_ACK, kPacket );
		return;
	}

    // loading ���¸� ��ȭ��Ų��.
    if( m_spRoomUserManager->SetLoadingProgress( FIRST_SENDER_UID, kPacket_.m_iLoadingProgress ) == false )
    {
        kPacket.m_iOK = NetError::ERR_ROOM_13;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_LOADING_ACK, kPacket );
        return;
    }

	//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	if( kPacket_.m_iLoadingProgress >= 100 )
	{
		const int iSuitableMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( GetDungeonIDAndDif() ) - 2;
		const int iSuitableMaxLevel = SiCXSLDungeonManager()->GetDungeonMaxLevel( GetDungeonIDAndDif() ) + 2;

		bool bSuitableLevel = false;

		if( iSuitableMinLevel <= kPacket_.m_iUnitLevel &&  kPacket_.m_iUnitLevel <= iSuitableMaxLevel )
		{
			bSuitableLevel = true;
		}		
		m_spRoomUserManager->SetUnitLevelBeforGameStart( FIRST_SENDER_UID, bSuitableLevel );
	}
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	//}}

    // ���� ������� ack�� ������.
    kPacket.m_iOK = NetError::NET_OK;
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_LOADING_ACK, kPacket );

    // ���� ��� �����鿡�� not�� ������.
    KEGS_GAME_LOADING_NOT kPacketBR;
    kPacketBR.m_iUnitUID = FIRST_SENDER_UID;
    kPacketBR.m_iLoadingProgress = kPacket_.m_iLoadingProgress;
    BroadCast( ERM_GAME_LOADING_NOT, kPacketBR );

	//{{ 2011. 07. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	if( m_spRoomUserManager->IsHost( FIRST_SENDER_UID ) == true )
	{
		for( int i = 0; i < m_spRoomUserManager->GetNumMember(); ++i )
		{
			KRoomUserPtr pkRoomUser = m_spRoomUserManager->GetUser( i );
			if( pkRoomUser == NULL )
				continue;

			if( pkRoomUser->IsPvpNpc() == false )
				continue;

			pkRoomUser->SetLoadingProgress( kPacket_.m_iLoadingProgress );

			KEGS_GAME_LOADING_NOT kPacketBR;
			kPacketBR.m_iUnitUID = pkRoomUser->GetCID();
			kPacketBR.m_iLoadingProgress = kPacket_.m_iLoadingProgress;
			BroadCast( ERM_GAME_LOADING_NOT, kPacketBR );
		}
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}

	//{{ 2011. 08. 05	������	�����濡 ���� ȥ�� ���ԵǴ� ���� ����
#ifdef SERV_CODE_REFACTORING_20110804
	if( CheckAllPlayerFinishLoading() == true )
	{
		//{{ 2011. 08. 05	������	�����濡 ���� ȥ�� ���ԵǴ� ���� ����
#ifdef SERV_PVP_USER_ALONE_IN_ROOM_BUG_FIX
		if( IsOfficialMatch() )
		{
			if( CheckIfPlayEnd() )
			{
				EndPlay();
			}
		}
#endif SERV_PVP_USER_ALONE_IN_ROOM_BUG_FIX
		//}}
	}
#else
	// ��� ������ �ε� �������� not�� ������.
	if( m_spRoomUserManager->IsAllPlayerFinishLoading() )
	{
		BroadCastID( ERM_GAME_LOADING_ALL_UNIT_OK_NOT );

		// ���������� �ʱ�ȭ.
		StartPlay();

		KEGS_PLAY_START_NOT kNot;
		//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
		kNot.m_iDungeonID = GetDungeonIDAndDif();
#endif SERV_CHAR_LOG
		//}}
		kNot.m_RoomState = GetStateID();
		GetRoomSlotInfo( kNot.m_vecSlot );

		// ��� �������� �÷��� ������ �˸�.
		BroadCast( ERM_PLAY_START_NOT, kNot );

		// ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
		SendRoomListInfo( NetError::ERR_ROOM_15 );

		//{{ 2009. 4. 23  ������	������������Ʈ
		SendAllPVPUserInfo();
		//}}
	}
#endif SERV_CODE_REFACTORING_20110804
	//}}
}

_IMPL_ON_FUNC( ERM_STATE_CHANGE_RESULT_REQ, KEGS_STATE_CHANGE_RESULT_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_RESULT ), ERM_STATE_CHANGE_RESULT_ACK, KEGS_STATE_CHANGE_RESULT_ACK );

    // �������� üũ�Ѵ�.
    VERIFY_HOST( ERM_STATE_CHANGE_RESULT_ACK );

    // ���� ������� ack�� ������.
    kPacket.m_iOK = NetError::NET_OK;
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STATE_CHANGE_RESULT_ACK, kPacket );

    // ��� �������� ���� ��ȭ �϶�� �˸���.
    KEGS_STATE_CHANGE_RESULT_NOT kPacketNot;
    kPacketNot.m_cRoomState = GetStateID();
    m_spRoomUserManager->GetRoomSlotInfo( kPacketNot.m_vecSlot );
	BroadCast( ERM_STATE_CHANGE_RESULT_NOT, kPacketNot, KRoomUserManager::UL_RESULT );
}

//{{ 2007. 8. 20  ������  ä�� ����
_IMPL_ON_FUNC( ERM_CHAT_REQ, KEGS_CHAT_REQ )
{
    VERIFY_ROOM_STATE_CHAT_ONLY( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ),
                       ERM_CHAT_ACK, KERM_CHAT_ACK );

	if( kPacket_.m_cChatPacketType != KEGS_CHAT_REQ::CPT_TOTAL )
	{
		START_LOG( cerr, L"�Ϲ�ä�� ��ŶŸ���� �ƴѵ� ����Գ�?" )
			<< BUILD_LOGc( kPacket_.m_cChatPacketType )
			<< END_LOG;
		return;
	}

    // ���� ������� ack�� ������.
    kPacket.m_iOK			  = NetError::NET_OK;
	kPacket.m_cChatPacketType = kPacket_.m_cChatPacketType; 

    // ��� �������� ä�� ������ ������.
    KEGS_CHAT_NOT kPacketBR;
    kPacketBR.m_SenderUnitUID	= FIRST_SENDER_UID;
	kPacketBR.m_RecieverUnitUID = kPacket_.m_ToUnitUID;
	kPacketBR.m_cChatPacketType = kPacket_.m_cChatPacketType; 
	kPacketBR.m_cRoomChatType	= kPacket_.m_cRoomChatType;
    kPacketBR.m_wstrMsg			= kPacket_.m_wstrMsg;

    /*  070709. jseop. ä�� �α� ����.
    KELOG_CHAT_LOG_NOT kNot;
    kNot.m_nRoomUID = GetUID();
    kNot.m_nCharUID = FIRST_SENDER_UID;
    //kNot.m_nUserUID = ; // �Ʒ����� ó���Ѵ�.
    kNot.m_cChatType = kPacket_.m_cRoomChatType;
    kNot.m_strMsg = kPacket_.m_wstrMsg.substr( 0, 62 );

    KRoomUserPtr spUser = m_spRoomUserManager->GetUser( kNot.m_nCharUID );
    if( spUser )
    {
        KRoomUserInfo kInfo;
        spUser->GetRoomUserInfo( kInfo );
        kNot.m_nUserUID = kInfo.m_kUnitInfo.m_iOwnerUserUID;
    }

    KncSend( PI_CN_ROOM, GetUID(), PI_LOG_DB, 0, NULL, ELOG_CHAT_LOG_NOT, kNot );
    */
	
    if ( (CXSLRoom::ROOM_CHAT_TYPE)kPacketBR.m_cRoomChatType == CXSLRoom::RCT_ALL )
	{
		BroadCast( ERM_CHAT_NOT, kPacketBR );
	}
	else if ( (CXSLRoom::ROOM_CHAT_TYPE)kPacketBR.m_cRoomChatType == CXSLRoom::RCT_TEAM )
	{
		int iTeam;
        if( m_spRoomUserManager->GetTeam( FIRST_SENDER_UID, iTeam ) )
        {
            BroadCast( ERM_CHAT_NOT, kPacketBR, 
                ( ( iTeam == CXSLRoom::TN_RED ) ? KRoomUserManager::UL_RED_TEAM : KRoomUserManager::UL_BLUE_TEAM ) );
        }
        else
        {
            kPacket.m_iOK = NetError::ERR_ROOM_23;
        }
	}
	else if ( (CXSLRoom::ROOM_CHAT_TYPE)kPacketBR.m_cRoomChatType == CXSLRoom::RCT_WHISPER )
	{
#ifndef DEPRECATED_SERVER_GROUP_MASK
		//{{ 2010. 05. 19  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
		if( KncUid::ExtractServerGroupID( kPacket_.m_ToUnitUID ) != KncUid::ExtractServerGroupID( FIRST_SENDER_UID ) )
		{
			kPacket.m_iOK = NetError::ERR_CHAT_04;
		}
		else
#endif SERV_INTEGRATION
		//}}
#endif DEPRECATED_SERVER_GROUP_MASK
		{
			UidType nGS;
			if( m_spRoomUserManager->GetRoomUserGS( kPacket_.m_ToUnitUID, nGS ) )
			{
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHAT_NOT, kPacketBR );
				SendToGSCharacter( nGS, kPacket_.m_ToUnitUID, ERM_CHAT_NOT, kPacketBR );
			}
			else
			{
				kPacket.m_iOK = NetError::ERR_ROOM_23;
			}
		}
	}

    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHAT_ACK, kPacket );
}
//}}

_IMPL_ON_FUNC( ERM_CHANGE_MOTION_REQ, KEGS_CHANGE_MOTION_REQ )
{
	VERIFY_ROOM_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ), 
		ERM_CHANGE_MOTION_ACK, KEGS_CHANGE_MOTION_ACK );

	// ���� ������� ack�� ������.
	kPacket.m_iOK = NetError::NET_OK;

	// ��� �������� Motion Not ������.
	KEGS_CHANGE_MOTION_NOT kEGS_CHANGE_MOTION_NOT;
	kEGS_CHANGE_MOTION_NOT.m_UnitUID = FIRST_SENDER_UID;
	kEGS_CHANGE_MOTION_NOT.m_cMotionID = kPacket_.m_cMotionID;
	
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_MOTION_ACK, kPacket );

	BroadCast( ERM_CHANGE_MOTION_NOT, kEGS_CHANGE_MOTION_NOT );
}

_IMPL_ON_FUNC( ERM_CHANGE_PLAY_TIME_LIMIT_REQ, KEGS_CHANGE_PLAY_TIME_LIMIT_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_CHANGE_PLAY_TIME_LIMIT_ACK, KEGS_CHANGE_PLAY_TIME_LIMIT_ACK );

	if( m_spRoomUserManager->IsObserver( FIRST_SENDER_UID ) == false )
	{
		VERIFY_HOST( ERM_CHANGE_PLAY_TIME_LIMIT_ACK );
	}

	//{{ 2009. 10. 22  ������	������ ���׼���	
	if( m_cRoomType == CXSLRoom::RT_PVP )
	{
		if( CXSLRoom::IsValidPvpPlayTime( m_cGameType, static_cast<int>(kPacket_.m_fPlayTime) ) == false )
		{
			START_LOG( cerr, L"������ �÷��� Ÿ�� ���ð��� �̻��ϴ�." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOGc( m_cGameType )
				<< BUILD_LOG( kPacket_.m_fPlayTime )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ROOM_30;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_PLAY_TIME_LIMIT_ACK, kPacket );
			return;
		}
	}
	//}}

    SetPlayTimeLimit( kPacket_.m_fPlayTime );

    kPacket.m_iOK = NetError::NET_OK;
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_PLAY_TIME_LIMIT_ACK, kPacket );

    KEGS_CHANGE_PLAY_TIME_LIMIT_NOT kPacketNot;
    kPacketNot.m_fPlayTime = kPacket_.m_fPlayTime;
    BroadCast( ERM_CHANGE_PLAY_TIME_LIMIT_NOT, kPacketNot );
    SendRoomListInfo( NetError::ERR_ROOM_15 );
}

_IMPL_ON_FUNC( ERM_SET_UNIT_INFO_NOT, KRoomUserInfo )
{
	//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	if( !( GetStateID() == KRoomFSM::S_WAIT || GetStateID() == KRoomFSM::S_LOAD || GetStateID() == KRoomFSM::S_PLAY || GetStateID() == KRoomFSM::S_RESULT || GetStateID() == KRoomFSM::S_WAIT_FOR_DEFENCE ) )
#else
	if( !( GetStateID() == KRoomFSM::S_WAIT || GetStateID() == KRoomFSM::S_LOAD || GetStateID() == KRoomFSM::S_PLAY || GetStateID() == KRoomFSM::S_RESULT ) )
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}    
    {
        START_LOG( cerr, L"���� ����." )
            << BUILD_LOG( GetStateID() )
            << BUILD_LOG( GetStateIDString() )
            << END_LOG;

        return;
    }

    m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( ERM_REFRESH_ROOM_REQ )
{
    VERIFY_ROOM_STATE( ( 3, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY ), ERM_REFRESH_ROOM_ACK, KEGS_REFRESH_ROOM_ACK );

    kPacket.m_iOK = NetError::NET_OK;
    GetRoomInfo( kPacket.m_RoomInfo );
    GetRoomSlotInfo( kPacket.m_vecSlot );

    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_REFRESH_ROOM_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

    m_spRoomUserManager->SetUserEquipItem( FIRST_SENDER_UID, kPacket_.m_vecInventorySlotInfo );
    m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_.m_kRoomUserInfo );
	
	// ���� ��������?
	if( GetStateID() == KRoomFSM::S_WAIT )
	{
		if( m_cRoomType == CXSLRoom::RT_DUNGEON )
		{
			KEGS_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT kPacketNot;
			kPacketNot.m_iUnitUID = FIRST_SENDER_UID;
			kPacketNot.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;
			//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
			kPacketNot.m_iEquippedTitleID = kPacket_.m_kRoomUserInfo.m_iTitleID;
#else
			kPacketNot.m_sEquippedTitleID = kPacket_.m_kRoomUserInfo.m_sTitleID;
#endif SERV_TITLE_DATA_SIZE
			//}}			
			BroadCast( ERM_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT, kPacketNot );
			return;
		}
	}
	
	// �׿ܿ� ����ġ���..
	BroadCast( ERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT, kPacket_ );
}

//{{ 2009. 5. 21  ������	������ �Ҹ� ����
IMPL_ON_FUNC( ERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT )
{
	VERIFY_STATE( ( 2, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	m_spRoomUserManager->SetUserEquipItem( FIRST_SENDER_UID, kPacket_.m_vecInventorySlotInfo );
	m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_.m_kRoomUserInfo );

	BroadCast( ERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT, kPacket_ );
}
//}}

//{{ 2012. 10. 17	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
_IMPL_ON_FUNC( ERM_RESULT_SUCCESS_REQ, KEGS_RESULT_SUCCESS_REQ )
#else
IMPL_ON_FUNC_NOPARAM( ERM_RESULT_SUCCESS_REQ )
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
//}}
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_RESULT ), ERM_RESULT_SUCCESS_ACK, KEGS_RESULT_SUCCESS_ACK );

	if( m_spRoomUserManager->SetSuccessResult( FIRST_SENDER_UID, true ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_17;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_RESULT_SUCCESS_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	//{{ 2009. 4. 28  ������	�̺�Ʈ����
	kPacket.m_iDungeonID = m_iDungeonID;
	//}}
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_RESULT_SUCCESS_ACK, kPacket );

	//{{ 2012. 10. 16	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	LogToDB_GamePlayNetWorkInfo( FIRST_SENDER_UID, kPacket_.m_kGamePlayNetworkInfo, false );
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	//}}

	//��� ������ ������ ���â Ȯ���� ��������
	if( m_spRoomUserManager->IsAllPlayerSuccessResult() == true )
	{
		// ���� ���� ó��
		EndGame();

#ifdef SERV_PVP_REMATCH
		if( CanRematch() == true )
		{
			Rematch();
			return;
		}
#endif SERV_PVP_REMATCH

		//{{ 2012. 06. 07	������	��Ʋ�ʵ� �ý���
		//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BATTLE_FIELD_SYSTEM
		//////////////////////////////////////////////////////////////////////////

		// S_RETURN_TO_FIELD ���·� �����Ѵ��� ���� �۾��� ��������!
		StateTransition( KRoomFSM::I_TO_RETURN_TO_FIELD );

		// Ÿ�̸� �ʱ�ȭ!
		m_kTimer[TM_RETURN_TO_FIELD].restart();

		// ��Ŷ ����
		KEGS_RESULT_ALL_UNIT_SUCCESS_NOT kNot;
		GetRoomInfo( kNot.m_RoomInfo );
		GetRoomSlotInfo( kNot.m_vecSlot );		

		// ù��° ������� ��Ż �õ�!
		UidType iGSUID = 0;
		UidType iFirstUnitUID = 0;
		m_spRoomUserManager->PrepareForReturnToFieldProcess( GetUID(), iGSUID, iFirstUnitUID, kNot.m_kReturnToFieldInfo );

		// ����!
		SendToGSCharacter( iGSUID, iFirstUnitUID, ERM_RESULT_ALL_UNIT_SUCCESS_NOT, kNot );

		//////////////////////////////////////////////////////////////////////////
#else
		//////////////////////////////////////////////////////////////////////////

		//���ó������ ������ ���� �濡 ������� �������Ա��� ��Ŷ�� ������.
		KEGS_RESULT_ALL_UNIT_SUCCESS_NOT kNot;
		GetRoomInfo( kNot.m_RoomInfo );
		GetRoomSlotInfo( kNot.m_vecSlot );
		BroadCast( ERM_RESULT_ALL_UNIT_SUCCESS_NOT, kNot );

		//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
		//////////////////////////////////////////////////////////////////////////
		//}}

		//������� ó�� �Ǿ����Ƿ� ������ ������Ʈ
		SendRoomListInfo( NetError::ERR_ROOM_15 );

		//{{ 2009. 4. 23  ������	������������Ʈ		
		SendAllPVPUserInfo();
		//}}
	}
}

_IMPL_ON_FUNC( ERM_ADD_ON_STAT_REQ, KEGS_ADD_ON_STAT_REQ )
{
	//{{ 2009. 3. 30  ������	cerr -> cwarn
	VERIFY_STATE_WARN( ( 1, KRoomFSM::S_PLAY ) );
	//}}

	//demiahun. 07.10.26. ����μ��� Ư���� �����̳� ���� ����� ����.

	if( (int)KncUid::ExtractReservedID( GetUID() ) == CXSLRoom::RT_PVP ||
		(int)KncUid::ExtractReservedID( GetUID() ) == CXSLRoom::RT_DUNGEON ||
		(int)KncUid::ExtractReservedID( GetUID() ) == CXSLRoom::RT_BATTLE_FIELD ||
		(int)KncUid::ExtractReservedID( GetUID() ) == CXSLRoom::RT_TRAININGCENTER )
	{
		KEGS_ADD_ON_STAT_ACK kAck;
		kAck.m_iOK	= NetError::NET_OK;

		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_ADD_ON_STAT_ACK, kAck );

		KEGS_ADD_ON_STAT_NOT kNot;
		kNot.m_UID			= kPacket_.m_UID;
		kNot.m_bIsUnit		= kPacket_.m_bIsUnit;
		kNot.m_cStatType	= kPacket_.m_cStatType;
		kNot.m_fStatVal		= kPacket_.m_fStatVal;
		kNot.m_fTime		= kPacket_.m_fTime;

		BroadCast( ERM_ADD_ON_STAT_NOT, kNot, KRoomUserManager::UL_PLAY );
	}
	else
	{
		KEGS_ADD_ON_STAT_ACK kAck;
		kAck.m_iOK	= NetError::ERR_ROOM_28;

		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_ADD_ON_STAT_ACK, kAck );

		START_LOG( cerr, L"�߸��� ����Ÿ��[���ӽ���]" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( NetError::GetErrStr( kAck.m_iOK ) )
			<< BUILD_LOG( (int)KncUid::ExtractReservedID( GetUID() ) )
			<< END_LOG
			;
	}
}

_IMPL_ON_FUNC( ERM_REPAIR_ITEM_NOT, KEGS_REPAIR_ITEM_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	BroadCast( ERM_REPAIR_ITEM_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_REQUEST_TRADE_NOT )
{
    VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

    UidType iGSUID;
    if( !m_spRoomUserManager->GetRoomUserGS( kPacket_.m_iReceiverUID, iGSUID ) )
    {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( kPacket_.m_iSenderUID )
            << BUILD_LOG( kPacket_.m_iReceiverUID )
            << BUILD_LOG( kPacket_.m_wstrSenderNickName )
            << END_LOG;
        return;
    }

    if( !m_spRoomUserManager->RequestTradeTo( FIRST_SENDER_UID, kPacket_.m_iReceiverUID ) )
    {
        START_LOG( clog, L"�ŷ� ��û ����." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;

        KERM_REQUEST_TRADE_REPLY_NOT kPacket;
        kPacket.m_iReceiverUID = FIRST_SENDER_UID;
        kPacket.m_iOK = NetError::NOT_REQUEST_TRADE_REPLY_03;
        kPacket.m_iMemberUID = kPacket_.m_iReceiverUID;
        kPacket.m_iMemberGSUID = iGSUID;

        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_REQUEST_TRADE_REPLY_NOT, kPacket );
        return;
    }

    // ���� ��� �������� �ٲ� ������ ������.
    KEGS_CHANGE_TRADE_NOT kNot;
    kNot.m_iUnitUID = FIRST_SENDER_UID;
    if( !m_spRoomUserManager->IsInTrade( FIRST_SENDER_UID, kNot.m_bTrade ) )
    {
        START_LOG( cerr, L"�ŷ� ���� Ȯ�� ����." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;
        return;
    }
    BroadCast( ERM_CHANGE_TRADE_NOT, kNot );

    KEGS_CHANGE_READY_NOT kReadyNot;
    kReadyNot.m_UnitUID = FIRST_SENDER_UID;
    if( !m_spRoomUserManager->IsReady( FIRST_SENDER_UID, kReadyNot.m_bReady ) )
    {
        START_LOG( cerr, L"�غ� ���� Ȯ�� ����." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;
        return;
    }
    BroadCast( ERM_CHANGE_READY_NOT, kReadyNot );

    SendToGSCharacter( iGSUID, kPacket_.m_iReceiverUID, ERM_REQUEST_TRADE_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_REQUEST_TRADE_REPLY_NOT )
{
   VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

    UidType iGSUID;
    if( !m_spRoomUserManager->GetRoomUserGS( kPacket_.m_iReceiverUID, iGSUID ) )
    {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( kPacket_.m_iReceiverUID )
            << END_LOG;
        return;
    }

    if( kPacket_.m_iOK == NetError::NOT_REQUEST_TRADE_REPLY_00 )
    {
        if( m_spRoomUserManager->TradeAcceptedBy( kPacket_.m_iReceiverUID, FIRST_SENDER_UID ) )
        {
            SendToGSCharacter( iGSUID, kPacket_.m_iReceiverUID, ERM_REQUEST_TRADE_REPLY_NOT, kPacket_ );
            return;
        }
        else
        {
            kPacket_.m_iOK = NetError::NOT_REQUEST_TRADE_REPLY_03;
        }
    }

    if( m_spRoomUserManager->TradeRejectedBy( kPacket_.m_iReceiverUID, FIRST_SENDER_UID ) )
    {
        // ���� ��� �������� �ٲ� ������ ������.
        KEGS_CHANGE_TRADE_NOT kNot;
        kNot.m_iUnitUID = kPacket_.m_iReceiverUID;
        if( m_spRoomUserManager->IsInTrade( kPacket_.m_iReceiverUID, kNot.m_bTrade ) )
        {
            BroadCast( ERM_CHANGE_TRADE_NOT, kNot );
        }
        else
        {
            START_LOG( cerr, L"�ŷ� ���� Ȯ�� ����." )
                << BUILD_LOG( kPacket_.m_iReceiverUID )
                << END_LOG;
        }

        KEGS_CHANGE_READY_NOT kReadyNot;
        kReadyNot.m_UnitUID = kPacket_.m_iReceiverUID;
        if( m_spRoomUserManager->IsReady( kPacket_.m_iReceiverUID, kReadyNot.m_bReady ) )
        {
            BroadCast( ERM_CHANGE_READY_NOT, kReadyNot );
        }
        else
        {
            START_LOG( cerr, L"�غ� ���� Ȯ�� ����." )
                << BUILD_LOG( kPacket_.m_iReceiverUID )
                << END_LOG;
        }
    }

    SendToGSCharacter( iGSUID, kPacket_.m_iReceiverUID, ERM_REQUEST_TRADE_REPLY_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( ERM_TRADE_START_NOT )
{
    VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

    if( !m_spRoomUserManager->SetTrade( FIRST_SENDER_UID, true ) )
    {
        START_LOG( clog, L"�ŷ� ��Ȳ ������Ʈ ����." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;

        KEGS_BREAK_TRADE_REQ kPacket;
        kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_09;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BREAK_TRADE_REQ, kPacket );
        return;
    }

    // ���� ��� �������� �ٲ� ������ ������.
    KEGS_CHANGE_TRADE_NOT kNot;
    kNot.m_iUnitUID = FIRST_SENDER_UID;
    if( !m_spRoomUserManager->IsInTrade( FIRST_SENDER_UID, kNot.m_bTrade ) )
    {
        START_LOG( cerr, L"�ŷ� ���� Ȯ�� ����." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;
        return;
    }
    BroadCast( ERM_CHANGE_TRADE_NOT, kNot );

    KEGS_CHANGE_READY_NOT kReadyNot;
    kReadyNot.m_UnitUID = FIRST_SENDER_UID;
    if( !m_spRoomUserManager->IsReady( FIRST_SENDER_UID, kReadyNot.m_bReady ) )
    {
        START_LOG( cerr, L"�غ� ���� Ȯ�� ����." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;
        return;
    }
    BroadCast( ERM_CHANGE_READY_NOT, kReadyNot );
}

IMPL_ON_FUNC_NOPARAM( ERM_TRADE_FINISH_NOT )
{
    VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

    if( !m_spRoomUserManager->SetTrade( FIRST_SENDER_UID, false ) )
    {
        START_LOG( clog, L"�ŷ� ��Ȳ ������Ʈ." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;
        return;
    }

    bool bTrade;
    if( m_spRoomUserManager->IsInTrade( FIRST_SENDER_UID, bTrade ) )
    {
        // ���� ��� �������� �ٲ� ������ ������.
        KEGS_CHANGE_TRADE_NOT kNot;
        kNot.m_iUnitUID = FIRST_SENDER_UID;
        kNot.m_bTrade = bTrade;

        BroadCast( ERM_CHANGE_TRADE_NOT, kNot );
    }
    else
    {
        START_LOG( cerr, L"�ŷ� ���� Ȯ�� ����." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;
    }
}

IMPL_ON_FUNC( ERM_SET_QUEST_ITEM_INFO_NOT )
{
	//����� ���������� �޴´ٴ� �����Ͽ� ����Ʈ �϶��� ó����.
	//{{ 2010. 02. 10  ������	���� �̺�Ʈ ����Ʈ
	// �ǽð����� ����Ʈ�� ����ó���Ǵ� ����� �߰��ʿ� ���� ���� ������ �ɸ��� �ȵȴ�. ���� �ּ�ó����.
	//VERIFY_STATE( ( 1, KRoomFSM::S_WAIT ) );
	//}}	

	if( m_spRoomUserManager->SetUnitQuestInvenInfo( FIRST_SENDER_UID, kPacket_.m_mapDropQuestItembyIngQuest ) == false )
	{
		START_LOG( cerr, L"����Ʈ ������ �κ��丮 ���� ���� ����.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}
	
	//{{ 2011. 05. 04  ��μ�	���� Ŭ����� ������ ���� ���� �߰�
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	if( m_spRoomUserManager->SetUnitAllQuestInfo( FIRST_SENDER_UID, kPacket_.m_setQuestInfo ) == false )
	{
		START_LOG( cerr, L"��� ����Ʈ ���� ����.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}

#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
	if( m_spRoomUserManager->SetUnitGoingQuestInfo( FIRST_SENDER_UID, kPacket_.m_setGoingQuestInfo ) == false )
	{
		START_LOG( cerr, L"������ ����Ʈ ���� ����.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST
}

//{{ 2008. 4. 2  ������  �ټ��� ȸ�� ������
_IMPL_ON_FUNC( ERM_RESTORE_SPIRIT_NOT, KEGS_RESTORE_SPIRIT_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	BroadCast( ERM_RESTORE_SPIRIT_NOT, kPacket_ );
}
//}}

//{{ 2009. 5. 8  ������		�ǽð� ������
IMPL_ON_FUNC( ERM_CHAR_LEVEL_UP_NOT )
{
	// ��� ���� �� ����ֵ� �ǳ�? Ŭ��� ������ ����!
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_.m_kRoomUserInfo );

	KEGS_CHAR_LEVEL_UP_NOT kPacketNot;
	kPacketNot.m_iUnitUID = FIRST_SENDER_UID;
	kPacketNot.m_ucLevel = kPacket_.m_kRoomUserInfo.m_ucLevel;
	kPacketNot.m_kBaseStat = kPacket_.m_kBaseStat;
	kPacketNot.m_kGameStat = kPacket_.m_kRoomUserInfo.m_kGameStat;
#ifdef SERV_ELESIS_UPDATE_EVENT
	kPacketNot.m_iNoteViewCount = kPacket_.m_iNoteViewCount;
#endif SERV_ELESIS_UPDATE_EVENT
	BroadCast( ERM_CHAR_LEVEL_UP_NOT, kPacketNot );
}
//}}

//{{ 2009. 9. 23  ������	1������ĳ����
_IMPL_ON_FUNC( ERM_CHANGE_ROOM_USER_INFO_NOT, KRoomUserInfo )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );
	
	m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_ );

	// ���������� ������
	BroadCast( ERM_CHANGE_ROOM_USER_INFO_NOT, kPacket_ );
}
//}}

//{{ 2009. 11. 30  ������	��彺ų
#ifdef GUILD_SKILL_TEST

IMPL_ON_FUNC( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_ );

	// ���������� ������
	BroadCast( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacket_ );
}

#ifdef DUNGEON_ITEM
IMPL_ON_FUNC_NOPARAM( ERM_CHANGE_PVP_ITEMMODE_REQ )
{
	VERIFY_STATE( ( 1, KRoomFSM::S_WAIT ) );

	KEGS_CHANGE_PVP_ITEMMODE_ACK kAck;
	KEGS_CHANGE_PVP_ITEMMODE_NOT kNot;

	kAck.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_PVP_ITEMMODE_ACK, kAck );

	m_bIsItemMode = !m_bIsItemMode;

	kNot = m_bIsItemMode;
	BroadCast( ERM_CHANGE_PVP_ITEMMODE_NOT, kNot );

	//������ ������Ʈ
	SendRoomListInfo( NetError::ERR_ROOM_15 );
}
#endif DUNGEON_ITEM


#endif GUILD_SKILL_TEST
//}}


//{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION

_IMPL_ON_FUNC( ERM_SEARCH_UNIT_REQ, KELG_SEARCH_UNIT_REQ )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	UidType iTargetGS = 0;

	switch( kPacket_.m_usEventID )
	{
	case EGS_GET_CONNECTION_UNIT_INFO_REQ:
		{
			if( m_spRoomUserManager->GetRoomUserGS( kPacket_.m_wstrNickName, iTargetGS, kPacket_.m_iUnitUID ) == false )
			{
				KELG_SEARCH_UNIT_ACK kPacket;
				kPacket.m_iOK		= NetError::ERR_SEARCH_UNIT_03; // ���� ���� ���� �ƴմϴ�.
				kPacket.m_usEventID = kPacket_.m_usEventID;
				kPacket.m_kCUnitInfo.m_wstrCharName = kPacket_.m_wstrNickName;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SEARCH_UNIT_ACK, kPacket );
				return;
			}
		}
		break;

	case EGS_WATCH_UNIT_REQ:
		{
			if( m_spRoomUserManager->GetRoomUserGS( kPacket_.m_iUnitUID, iTargetGS ) == false )
			{
				KELG_SEARCH_UNIT_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_SEARCH_UNIT_03; // ���� ���� ���� �ƴմϴ�.
				kPacket.m_usEventID = kPacket_.m_usEventID;
				kPacket.m_kCUnitInfo.m_iUnitUID = kPacket_.m_iUnitUID;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SEARCH_UNIT_ACK, kPacket );
				return;
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"�̻��� eventid�� ����� �Խ��ϴ�." )
				<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
				<< END_LOG;
		}
		return;
	}
	
	//ã�� ĳ���� ������ �ش� GS�� ������û
	KELG_GET_UNIT_INFO_REQ kReq;
	kReq.m_usEventID		= kPacket_.m_usEventID;
	kReq.m_iDemandGSUID		= LAST_SENDER_UID;
	kReq.m_iDemandUserUID	= FIRST_SENDER_UID;
	kReq.m_wstrNickName		= kPacket_.m_wstrNickName;
	SendToGSCharacter( iTargetGS, kPacket_.m_iUnitUID, ERM_GET_UNIT_INFO_REQ, kReq );
}

_IMPL_ON_FUNC( ERM_GET_UNIT_INFO_ACK, KELG_GET_UNIT_INFO_ACK )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	KELG_SEARCH_UNIT_ACK kPacket;
	kPacket.m_iOK			= kPacket_.m_iOK;
	kPacket.m_usEventID		= kPacket_.m_usEventID;
	kPacket.m_kCUnitInfo	= kPacket_.m_kCUnitInfo;

	SendToGSCharacter( kPacket_.m_iDemandGSUID, kPacket_.m_iDemandUserUID, ERM_SEARCH_UNIT_ACK, kPacket );
}

#endif SERV_INTEGRATION
//}}

//{{ 2010. 8. 3	������	�� �ý���
#ifdef SERV_PET_SYSTEM

IMPL_ON_FUNC( ERM_SUMMON_PET_NOT )
{
	VERIFY_STATE( ( 2, KRoomFSM::S_WAIT, KRoomFSM::S_PLAY ) );

    // RoomUserInfo�� ������Ʈ �Ѵ�!
	if( m_spRoomUserManager->UpdatePetInfo( kPacket_.m_iUnitUID, kPacket_.m_vecPetInfo ) == false )
	{
		START_LOG( cerr, L"�� ���� ������Ʈ ����!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
		return;
	}

	// ��ε� ĳ�����ؼ� �� ��ȯ�� �˸���!
    BroadCast( ERM_SUMMON_PET_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_PET_ACTION_NOT )
{
	// [2012-12-07][������] ���� ����α׸� ���� �ʿ�� ����.
	VERIFY_STATE_WARN( ( 2, KRoomFSM::S_WAIT, KRoomFSM::S_PLAY ) );

	BroadCast( ERM_PET_ACTION_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGED_PET_SATIETY_NOT, KEGS_CHANGED_PET_SATIETY_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	// �� ������ ���� ������Ʈ	
	if( m_spRoomUserManager->UpdatePetSatiety( kPacket_.m_iUnitUID, kPacket_.m_iPetUID, kPacket_.m_sCurrentSatiety ) == false )
	{
		START_LOG( cerr, L"�� ���� ������Ʈ ����!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iPetUID )
			<< BUILD_LOG( kPacket_.m_sCurrentSatiety )
			<< END_LOG;
		return;
	}

	BroadCast( ERM_CHANGED_PET_SATIETY_NOT, kPacket_ );    
}

_IMPL_ON_FUNC( ERM_CHANGED_PET_INTIMACY_NOT, KEGS_CHANGED_PET_INTIMACY_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	// �� ģ�е� ���� ������Ʈ	
	if( m_spRoomUserManager->UpdatePetIntimacy( kPacket_.m_iUnitUID, kPacket_.m_iPetUID, kPacket_.m_iCurrentIntimacy ) == false )
	{
		START_LOG( cerr, L"�� ���� ������Ʈ ����!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iPetUID )
			<< BUILD_LOG( kPacket_.m_iCurrentIntimacy )
			<< END_LOG;
		return;
	}

	BroadCast( ERM_CHANGED_PET_INTIMACY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGED_PET_SATIETY_INTIMACY_NOT, KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT )
{
	// �� ������ ���� ������Ʈ
	if( m_spRoomUserManager->UpdatePetSatiety( kPacket_.m_iUnitUID, kPacket_.m_iPetUID, kPacket_.m_sCurrentSatiety ) == false )
	{
		START_LOG( cerr, L"�� ���� ������Ʈ ����!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iPetUID )
			<< BUILD_LOG( kPacket_.m_sCurrentSatiety )
			<< END_LOG;
		return;
	}

	// �� ģ�е� ���� ������Ʈ	
	if( m_spRoomUserManager->UpdatePetIntimacy( kPacket_.m_iUnitUID, kPacket_.m_iPetUID, kPacket_.m_iCurrentIntimacy ) == false )
	{
		START_LOG( cerr, L"�� ���� ������Ʈ ����!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iPetUID )
			<< BUILD_LOG( kPacket_.m_iCurrentIntimacy )
			<< END_LOG;
		return;
	}

	BroadCast( ERM_CHANGED_PET_SATIETY_INTIMACY_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_PET_EVOLUTION_NOT )
{
	VERIFY_STATE( ( 2, KRoomFSM::S_WAIT, KRoomFSM::S_PLAY ) );

	// RoomUserInfo�� ������Ʈ �Ѵ�!
	if( m_spRoomUserManager->UpdatePetInfo( kPacket_.m_iUnitUID, kPacket_.m_kEvolutionPetInfo ) == false )
	{
		START_LOG( cerr, L"�� ���� ������Ʈ ����!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
		return;
	}

	// ��ε� ĳ�����ؼ� �� ��ȯ�� �˸���!
	BroadCast( ERM_PET_EVOLUTION_NOT, kPacket_ );
}
#endif SERV_PET_SYSTEM
//}}

//{{ 2011. 03. 26  ��μ�	û 1�� �߰� ��ų �߰� - ����
#ifdef SERV_ADD_BUFF_SKILL_INFO
_IMPL_ON_FUNC( ERM_BUFF_SKILL_INFO_CONVEY_REQ, KEGS_BUFF_SKILL_INFO_CONVEY_REQ )
{
	VERIFY_STATE( ( 1, KRoomFSM::S_PLAY ) );
	KEGS_BUFF_SKILL_INFO_CONVEY_ACK KAck;
	KAck.m_iOK = NetError::NET_OK;

	KEGS_BUFF_SKILL_INFO_CONVEY_NOT KNot;

	if( m_spRoomUserManager->ProcessBuffSkill( kPacket_, KNot) == false )		// ���� ���� ó��
	{	
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BUFF_SKILL_INFO_CONVEY_ACK, KAck );

		START_LOG( cerr, L"���� ��ų ó�� ����!" )
			<< BUILD_LOG( kPacket_.m_iCastingUnitUID )
			<< BUILD_LOG( kPacket_.m_iBuffEnum )
			<< BUILD_LOG( kPacket_.m_fBuffTime )
			<< BUILD_LOG( kPacket_.m_bIsBuffCasting )
			<< END_LOG;

		return;
	}
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BUFF_SKILL_INFO_CONVEY_ACK, KAck );

	BroadCast( ERM_BUFF_SKILL_INFO_CONVEY_NOT, KNot );
}
#endif SERV_ADD_BUFF_SKILL_INFO
//}}

//{{ 2012. 04. 21	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
_IMPL_ON_FUNC( ERM_UPDATE_BUFF_INFO_IN_ROOM_NOT, KEGS_UPDATE_BUFF_INFO_IN_ROOM_NOT )
{
	//{{ 2013. 04. 23	������	����� �� ���� ���� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	VERIFY_STATE( ( 5, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT, KRoomFSM::S_WAIT_FOR_DEFENCE ) );	
#else
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );	
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	//}}	
	
	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	if( spRoomUser == NULL )
	{
		//{{ 2012. 11. 21	�ڼ���	��ȸ ä�� ���� ����
#if defined( SERV_FIX_ZOMBIE_OBSERVER ) || defined( SERV_TOURNAMENT_CONDITION )
		spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID, KRoomUserManager::UT_OBSERVER );
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"������ ã�� �� �����ϴ�!" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
			return;
		}
#else
		START_LOG( cerr, L"������ ã�� �� �����ϴ�!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
#endif SERV_TOURNAMENT_CONDITION
		//}}
	}

	spRoomUser->UpdateBuffInfo( kPacket_.m_vecActivateBuffList );
	spRoomUser->UpdateGameStat( kPacket_.m_kGameStat );
	//{{ 2012. 12. 18	������	�ƶ� ��Ƽ �÷��� ���ʽ� ����ġ
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	spRoomUser->SetBonusRate( kPacket_.m_mapBonusRate );
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	//}}

	// ����� ��� �����鿡�� ������!
	BroadCast( ERM_UPDATE_BUFF_INFO_IN_ROOM_NOT, kPacket_ );
}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

//{{ 2012. 05. 31	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
_IMPL_ON_FUNC( ERM_USE_SKILL_BUFF_IN_PLAY_NOT, KEGS_USE_SKILL_BUFF_IN_PLAY_NOT )
{
	VERIFY_STATE( ( 1, KRoomFSM::S_WAIT ) );

	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"������ ã�� �� �����ϴ�!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	if( kPacket_.m_bOn )
	{
		spRoomUser->AddUseSkillBuffInPlay( kPacket_.m_iSkillID );
	}
	else
	{
		spRoomUser->DelUseSkillBuffInPlay( kPacket_.m_iSkillID );
	}

	// ����� ��� �����鿡�� ������!
	KEGS_USE_SKILL_BUFF_IN_PLAY_BROAD_NOT kPacketNot;
	kPacketNot.m_iUnitUID = FIRST_SENDER_UID;
	kPacketNot.m_iSkillID = kPacket_.m_iSkillID;
	kPacketNot.m_bOn = kPacket_.m_bOn;
	BroadCast( ERM_USE_SKILL_BUFF_IN_PLAY_BROAD_NOT, kPacketNot );
}

_IMPL_ON_FUNC( ERM_NEXT_RETURN_TO_FIELD_NOT, KERM_COMPLETE_RETURN_TO_FIELD_NOT )
{
	if( GetStateID() == KRoomFSM::S_INIT )
	{
		// �ð����� ���� �ʱ�ȭ �Ǿ����� ��Ŷ�� �ü��� �ִ�.
		return;
	}

	VERIFY_STATE( ( 1, KRoomFSM::S_RETURN_TO_FIELD ) );

	// Ÿ�̸� �ʱ�ȭ!
	m_kTimer[TM_RETURN_TO_FIELD].restart();

	// ��Ŷ ����
	KEGS_RESULT_ALL_UNIT_SUCCESS_NOT kNot;
	GetRoomInfo( kNot.m_RoomInfo );
	GetRoomSlotInfo( kNot.m_vecSlot );

	// ù��° ������� ��Ż �õ�!
	UidType iGSUID = 0;
	UidType iNextUnitUID = 0;
	if( m_spRoomUserManager->GetNextReturnToFieldUnitInfo( GetUID(), iGSUID, iNextUnitUID, kNot.m_kReturnToFieldInfo ) == false )
	{
		// ���̻� ��Ż ó���� ������ ������ ���⼭ ��!!
		StateTransition( KRoomFSM::I_TO_WAIT );

		// ���� ��Ƽ ������!
		CheckReserveRegroupOldParty();
		return;
	}

	// ����!
	SendToGSCharacter( iGSUID, iNextUnitUID, ERM_RESULT_ALL_UNIT_SUCCESS_NOT, kNot );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
_IMPL_ON_FUNC( ERM_UPDATE_USER_UNIT_BUFF_INFO_NOT, KEGS_UPDATE_USER_UNIT_BUFF_INFO_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	if( m_spRoomUserManager->IsExist( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"���� ������ �������� ĳ���ʹ� ���� �̹�ȿ� �����ϴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_vecBuffFactor.size() )
			<< END_LOG;
		return;
	}

	KEGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT kPacketNot;
	kPacketNot.m_iUnitUID = FIRST_SENDER_UID;
	kPacketNot.m_vecBuffFactor = kPacket_.m_vecBuffFactor;

	// ToUnitUID���� 0�̸� ��ε� ĳ����
	if( kPacket_.m_iToUnitUID == 0 )
	{
		BroadCast( ERM_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT, kPacketNot );
	}
	else
	{
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( kPacket_.m_iToUnitUID );
		if( IS_NULL( spRoomUser ) )
		{
			START_LOG( cerr, L"�������� �ʴ� �������� ���������� �������� �ߴ�!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iToUnitUID )
				<< END_LOG;
			return;
		}

		SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT, kPacketNot );
	}
}

_IMPL_ON_FUNC( ERM_UPDATE_NPC_UNIT_BUFF_INFO_NOT, KEGS_UPDATE_NPC_UNIT_BUFF_INFO_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	if( m_spRoomUserManager->IsExist( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"���� ������ �������� ĳ���ʹ� ���� �̹�ȿ� �����ϴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_vecNpcUnitBuff.size() )
			<< END_LOG;
		return;
	}

	KEGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT kPacketNot;
	kPacketNot.m_vecNpcUnitBuff = kPacket_.m_vecNpcUnitBuff;

//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

	// ToUnitUID���� 0�̸� ��ε� ĳ����
	if( kPacket_.m_vecToUnitUID.empty() == true )
	{
		BroadCast( ERM_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT, kPacketNot );
	}
	else
	{
        BOOST_TEST_FOREACH( UidType, uidUnitUID, kPacket_.m_vecToUnitUID )
        {
		    KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( uidUnitUID );
		    if( IS_NULL( spRoomUser ) )
		    {
			    START_LOG( cerr, L"�������� �ʴ� �������� ���������� �������� �ߴ�!" )
				    << BUILD_LOG( GetUID() )
				    << BUILD_LOG( FIRST_SENDER_UID )
				    << BUILD_LOG( uidUnitUID )
				    << END_LOG;
		    }
            else
            {
#ifdef  SERV_BATTLE_FIELD_SYSTEM
                spRoomUser->SetBattleFieldNpcSyncSubjects( false );
#endif  SERV_BATTLE_FIELD_SYSTEM
    		    SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT, kPacketNot );
            }
        }
	}


//#else   SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//
//	// ToUnitUID���� 0�̸� ��ε� ĳ����
//	if( kPacket_.m_iToUnitUID == 0 )
//	{
//		BroadCast( ERM_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT, kPacketNot );
//	}
//	else
//	{
//		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( kPacket_.m_iToUnitUID );
//		if( IS_NULL( spRoomUser ) )
//		{
//			START_LOG( cerr, L"�������� �ʴ� �������� ���������� �������� �ߴ�!" )
//				<< BUILD_LOG( GetUID() )
//				<< BUILD_LOG( FIRST_SENDER_UID )
//				<< BUILD_LOG( kPacket_.m_iToUnitUID )
//				<< END_LOG;
//			return;
//		}
//
//		SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT, kPacketNot );
//	}
//
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

}
#endif SERV_SERVER_BUFF_SYSTEM
//}}


//{{ 2012. 07. 27	������		�����ý��� EXP ���ʽ� ���� ����
#ifdef SERV_TUTO_EXP_BONUS_BUG_FIX
IMPL_ON_FUNC( ERM_UPDATE_TUTORIAL_INFO_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	m_spRoomUserManager->SetTutorialUIDList( FIRST_SENDER_UID, kPacket_.m_vecStudentUnitUID );
}
#endif SERV_TUTO_EXP_BONUS_BUG_FIX
//}}

//{{ 2012. 09. 19   ��μ�   ĳ�ü� �湮 ����
#ifdef SERV_VISIT_CASH_SHOP
_IMPL_ON_FUNC( ERM_VISIT_CASH_SHOP_NOT, KEGS_VISIT_CASH_SHOP_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	KERM_UPDATE_VISIT_CASH_SHOP_NOT kPacket;
	kPacket.m_bEnterCashShop = kPacket_.m_bEnterCashShop;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;

	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"������ ã�� �� �����ϴ�!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	spRoomUser->UpdateUserCashShopInfo( kPacket.m_bEnterCashShop );

	// ����� ��� �����鿡�� ������!
	BroadCast( ERM_UPDATE_VISIT_CASH_SHOP_NOT, kPacket );
}
#endif SERV_VISIT_CASH_SHOP
//}}

//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
_IMPL_ON_FUNC( ERM_UDP_PING_SCORE_NOT, KEGS_UDP_PING_SCORE_NOT )
{
	//{{ 2013. 04. 24	������	����� �� ���� ���� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	VERIFY_STATE( ( 5, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT, KRoomFSM::S_WAIT_FOR_DEFENCE ) );
#else
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	//}}	

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	//m_bForceHost = kPacket_.m_bForceHost;
	//m_spRoomUserManager->SetPingScore( FIRST_SENDER_UID, m_bForceHost, kPacket_.m_dwPingScore );
    m_spRoomUserManager->SetPingScore( FIRST_SENDER_UID, kPacket_.m_bForceHost, kPacket_.m_dwPingScore );
#else//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	m_spRoomUserManager->SetPingScore( FIRST_SENDER_UID, kPacket_.m_dwPingScore );
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
}
#endif SERV_CHOOSE_FASTEST_HOST
//}

//{{ 2012. 11. 20	�ڼ���	Field Frame �α� �߰�
#ifdef SERV_FIELD_FRAME_LOG
_IMPL_ON_FUNC( ERM_FIELD_USER_FRAME_INFO_NOT, int )
{
	VERIFY_STATE_WARN( ( 1, KRoomFSM::S_PLAY ) );

	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"������ ã�� �� �����ϴ�!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	spRoomUser->SetFieldFrame( kPacket_ );
}
#endif SERV_FIELD_FRAME_LOG
//}}

//{{ 2013. 04. 09	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
_IMPL_ON_FUNC( ERM_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT, KRoomUserInfo )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_ );

	// ���������� ������
	KEGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT kPacket;
	kPacket.m_iUnitUID = FIRST_SENDER_UID;
	kPacket.m_kGameStat = kPacket_.m_kGameStat;
	BroadCast( ERM_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT, kPacket );
}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-04-08
IMPL_ON_FUNC( ERM_LOCAL_RANKING_WATCH_UNIT_REQ )
{
	KRoomUserPtr spUser = m_spRoomUserManager->GetUser( kPacket_.m_iTargetUnitUID );

	if( spUser != NULL )
	{
		SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_LOCAL_RANKING_WATCH_UNIT_REQ, kPacket_ );
		return;
	}

	KERM_LOCAL_RANKING_WATCH_UNIT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_LOCALRANKING_11;
	SendToGSCharacter( kPacket_.m_iRequestGSUID, kPacket_.m_iRequestUnitUID, ERM_LOCAL_RANKING_WATCH_UNIT_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_LOCAL_RANKING_WATCH_UNIT_ACK )
{
	SendToGSCharacter( kPacket_.m_iRequestGSUID, kPacket_.m_iRequestUnitUID, ERM_LOCAL_RANKING_WATCH_UNIT_ACK, kPacket_ );
}
#endif	// SERV_LOCAL_RANKING_SYSTEM

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
_IMPL_ON_FUNC( ERM_SUMMON_RIDING_PET_NOT, KEGS_SUMMON_RIDING_PET_NOT )
{
	VERIFY_STATE( ( 2, KRoomFSM::S_WAIT, KRoomFSM::S_PLAY ) );

	// RoomUserInfo�� ������Ʈ �Ѵ�!
	if( m_spRoomUserManager->SetRidingPetInfo( kPacket_.m_iUnitUID, kPacket_.m_iRidingPetUID, kPacket_.m_usRidingPetID ) == false )
	{
		START_LOG( cerr, L"���̵� �� ���� ������Ʈ ����!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
		return;
	}

	// ��ε� ĳ�����ؼ� �� ��ȯ�� �˸���!
	BroadCast( ERM_SUMMON_RIDING_PET_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_UNSUMMON_RIDING_PET_NOT, KEGS_UNSUMMON_RIDING_PET_NOT )
{
	VERIFY_STATE( ( 2, KRoomFSM::S_WAIT, KRoomFSM::S_PLAY ) );

	// RoomUserInfo�� ������Ʈ �Ѵ�!
	if( m_spRoomUserManager->SetRidingPetInfo( kPacket_.m_iUnitUID, 0, 0 ) == false )
	{
		START_LOG( cerr, L"���̵� �� ���� ������Ʈ ����!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
		return;
	}

	// ��ε� ĳ�����ؼ� �� ��ȯ�� �˸���!
	BroadCast( ERM_UNSUMMON_RIDING_PET_NOT, kPacket_ );
}
#endif	// SERV_RIDING_PET_SYSTM

//{{ 2013. 2. 1	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
_IMPL_ON_FUNC( ERM_FORCE_RELAY_NOT, KEGS_FORCE_RELAY_NOT )
{
	if( m_pkCurrentState->CheckState( 1, KRoomFSM::S_PLAY ) == false )
	{
		return;
	}

	if( m_spRoomUserManager->CheckAuthLevel( kPacket_.m_iTargetUnitUID ) == true )
	{
		return;
	}

	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( kPacket_.m_iTargetUnitUID );
	if( spRoomUser == NULL )
	{
		return;
	}

	// �Ű� ���� ���� -> �Ű��ڷ��� ��Ŷ �帧�� �����̷� ������ ���� �����Ѵ�.
	kPacket_.m_iTargetUnitUID = FIRST_SENDER_UID;
	UidType iTargetUnitUID = spRoomUser->GetCID();
	SendToGSCharacter( spRoomUser->GetGSUID(), iTargetUnitUID, ERM_FORCE_RELAY_NOT, kPacket_ );

	// �Ű� ���� ������ ���� ��û�� ������.
	if( ( SiKGameSysVal()->GetLanBugOutCheck() == true ) &&
		( IsLanBugCheckUser( iTargetUnitUID ) == false )
		)
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_CHECK_NOT, GetUID(), iTargetUnitUID );
		AddLanBugCheckUser( iTargetUnitUID );
	}

	// �Ű����� ���� ��û�� ������!!
	if( ( SiKGameSysVal()->GetLanBugOutVerify() == true ) &&
		( IsLanBugVerifyUser( FIRST_SENDER_UID ) == false ) &&
		( m_spRoomUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == false )
		)
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_VERIFY_NOT, GetUID(), FIRST_SENDER_UID );
		AddLanBugVerifyUser( FIRST_SENDER_UID );
	}
}

_IMPL_ON_FUNC( ERM_ADD_UDP_CHECK_KICK_USER_NOT, UidType )
{
	RemoveLanBugCheckUser( kPacket_ );
	RemoveLanBugVerifyUser( kPacket_ );

	if( m_pkCurrentState->CheckState( 1, KRoomFSM::S_PLAY ) == false )
	{
		return;
	}

	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( kPacket_ );
	if( spRoomUser == NULL )
	{
		return;
	}

	// �߹� �α� ���
	KDBE_UDP_CHECK_KICK_USER_NOT kPacket;
	kPacket.m_iRoomUID			= GetUID();
	if( m_cRoomType == CXSLRoom::RT_PVP )
	{
		kPacket.m_iPVPChannelClass	= GetPVPChannelClass();
		kPacket.m_iMatchType		= GetMatchType();
	}

	kPacket.m_iUnitUID = kPacket_;
	SendToLogDB( DBE_UDP_CHECK_KICK_USER_NOT, kPacket );

	if( SiKGameSysVal()->GetLanBugOutJustLog() == true )
	{
		return;
	}

	SendToGSCharacter( spRoomUser->GetGSUID(), kPacket_, ERM_UDP_KICK_STATE_CHANGE_FIELD_NOT );
}

_IMPL_ON_FUNC( ERM_REMOVE_LANBUG_CHECK_USER_NOT, UidType )
{
	RemoveLanBugCheckUser( kPacket_ );
}

_IMPL_ON_FUNC( ERM_REMOVE_LANBUG_VERIFY_USER_NOT, UidType )
{
	RemoveLanBugVerifyUser( kPacket_ );
}
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
_IMPL_ON_FUNC( ERM_COUPLE_MAKING_SUCCESS_NOT, KERM_COUPLE_MAKING_SUCCESS_NOT )
{
	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	if( spRoomUser != NULL )
	{
		spRoomUser->SetLoverUnitUID( ( FIRST_SENDER_UID == kPacket_.m_iRequestUnitUID ) ? kPacket_.m_iAcceptUnitUID : kPacket_.m_iRequestUnitUID );
	}

	KEGS_COUPLE_MAKING_SUCCESS_NOT kPacket;
	kPacket.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
	kPacket.m_wstrAcceptUnitName = kPacket_.m_wstrAcceptUnitName;
	BroadCast( ERM_COUPLE_MAKING_SUCCESS_NOT, kPacket );
}

IMPL_ON_FUNC_NOPARAM( ERM_BREAK_UP_SUCCESS_NOT )
{
	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	if( spRoomUser != NULL )
	{
		spRoomUser->SetLoverUnitUID( 0 );
	}
}

_IMPL_ON_FUNC( ERM_RELATIONSHIP_EFFECT_NOT, KEGS_RELATIONSHIP_EFFECT_NOT )
{
	BroadCast( ERM_RELATIONSHIP_EFFECT_NOT, kPacket_ );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}
