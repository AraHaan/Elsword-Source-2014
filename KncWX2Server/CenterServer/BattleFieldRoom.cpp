//#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-06	// �ڼ���
#include <WinSock2.h>	// windows.h redefinition ���� ��ó��
#pragma pack( push, 8 )
//#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK
#include "BattleFieldRoom.h"
#include "NetError.h"
#include "Room/RoomFSM.h"
#include "X2Data/XSLRoom.h"
#include "KDropTable.h"
#include "ResultProcess.h"
#include "CnRoomManager.h"

#include "x2data/XSLItemManager.h"
#include "X2Data/XSLDungeonManager.h"
#include "X2Data/XSLUnitManager.h"
#include "KAttribNpcTable.h"

//{{ 2009. 5. 22  ������	��ӷ��̺�Ʈ
#include "GameEvent/GameEventManager.h"
//}}
#include "GameSysVal/GameSysVal.h"
//{{ 2010. 10. 05	������	���ͼ��� ��ŷüũ
#ifdef SERV_CN_SERVER_HACKING_CHECK
	#include "CnHackingCheckManager.h"
#endif SERV_CN_SERVER_HACKING_CHECK
//}}
//{{ 2010. 11. 20	������	Ư�� �ð� ���� ����
#ifdef SERV_TIME_DROP_MONSTER_EVENT
	#include "DungeonEventManager.h"
#endif SERV_TIME_DROP_MONSTER_EVENT
//}}

//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
#include "Socket/NetCommon.h"		// ������ ����(����� �� ����) IP
#include "CnNetLayer.h"				// ������ ����(����� �� ����) Port
#endif SERV_CHOOSE_FASTEST_HOST
//}

//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	#include "X2Data/XSLBuffManager.h"
#endif SERV_SERVER_BUFF_SYSTEM
//}

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-06	// �ڼ���
	#include "InteriorUdpSession.h"
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-31	// �ڼ���
	#include "X2Data/XSLFieldBossData.h"

#ifdef _CONVERT_VS_2010// �۾���¥: 2013-12-03	// �ڼ���
#include <intsafe.h>
#else // _CONVERT_VS_2010
#define	BYTE_MAX	0xff
#endif // _CONVERT_VS_2010

#endif // SERV_BATTLE_FIELD_BOSS


DWORD KBattleFieldRoom::m_dwMonsterRespawnCheckTick = 0;


NiImplementRTTI( KBattleFieldRoom, KRoom );

#define CLASS_TYPE KBattleFieldRoom

KBattleFieldRoom::KBattleFieldRoom()
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-05	// �ڼ���
	: m_byteBossFieldState( 0 )
	, m_tFieldHoldingTime( 0 )
	, m_tFieldOpenTime( 0 )
	, m_tFieldCloseTime( 0 )
#endif // SERV_BATTLE_FIELD_BOSS
{
	//{{ 2011. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_spUserManager = KBattleFieldUserManagerPtr( new KBattleFieldUserManager );
	SetRoomUserManager( m_spUserManager );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	m_spUserManager->Init( CXSLRoom::RT_BATTLE_FIELD, SEnum::BATTLE_FIELD_MAX_NUM );

	m_bEndPlay				= false;
	m_cGetItemType			= CXSLRoom::DGIT_RANDOM;

	//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
	m_fItemDropEventProbRate = REE_DROP_EVENT_PROB_DEFAULT;
#else // SERV_DROP_EVENT_RENEWAL
	m_iItemDropEventProbCount = REE_DROP_EVENT_PROB_DEFAULT;
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
	//}}
	//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
	m_fQuestItemDropEventProbRate = 1.f;
#endif SERV_QUEST_ITEM_DROP_EVENT
	//}

}

KBattleFieldRoom::~KBattleFieldRoom()
{
}

ImplToStringW( KBattleFieldRoom )
{
    return START_TOSTRING_PARENTW( KRoom );
}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-12	// �ڼ���
void KBattleFieldRoom::Tick()
{
    KRoom::Tick();

	switch( GetStateID() )
	{
	case KRoomFSM::S_WAIT:
		if( m_tTimer[TE_WAIT_TIMER].elapsed() > 10.0 )
		{
			// ��Ʋ�ʵ� ���� ��� �ð��� 10�ʸ� ������ ���� �Ҹ� ��Ų��!
			StateTransition( KRoomFSM::I_TO_CLOSE );
		}
		break;

	case KRoomFSM::S_PLAY:
		if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD ) == true )
		{
			CheckFieldBossSystem();
		}
		else
		{
			// ������ ���͸� üũ�մϴ�.
			CheckMonsterRespawn();

			//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			// �߰� ���� ���� üũ!
			CheckMiddleBossMonster();
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
			//}

			//{{ 2012. 12. 30	�ڼ���	�ʵ� �̺�Ʈ ����
#ifdef SERV_FIELD_EVENT_MONSTER
			CheckEventMonster();
#endif SERV_FIELD_EVENT_MONSTER
			//}}
		}

		// ������ ����ȭ üũ
		CheckIntrudeUserLoadComplete();

		// ���� P2P����ȭ ���� üũ
		CheckNpcP2PSyncUser();

		// ���� ���� üũ ���� ��Ŷ
		CheckZombieUserEcho();

		//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT
		CheckDropEventCount();
#endif SERV_ITEM_DROP_EVENT
		//}}

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
		if ( m_spUserManager->ZombieAlert_Tick() == true )
		{
#ifdef SERV_CHOOSE_FASTEST_HOST
			CheckPingScore();
			m_kTimer[TM_CHECK_CHANGE_HOST].restart();
#endif  SERV_CHOOSE_FASTEST_HOST
		}
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
		break;
	}
}
#else // SERV_BATTLE_FIELD_BOSS
void KBattleFieldRoom::Tick()
{
	KRoom::Tick();

	if( GetStateID() == KRoomFSM::S_PLAY )
	{
		//////////////////////////////////////////////////////////////////////////
		// ��Ƽ�� Ÿ�̸� üũ
		CheckContinueTimer();
		//////////////////////////////////////////////////////////////////////////

		// ���� �̺�Ʈ�� üũ�մϴ�.
		CheckDangerousEvent();

		// ������ ���͸� üũ�մϴ�.
		CheckMonsterRespawn();

		//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		// �߰� ���� ���� üũ!
		CheckMiddleBossMonster();
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}

		//{{ 2012. 12. 30	�ڼ���	�ʵ� �̺�Ʈ ����
#ifdef SERV_FIELD_EVENT_MONSTER
		CheckEventMonster();
#endif SERV_FIELD_EVENT_MONSTER
		//}}

		// ������ ����ȭ üũ
		CheckIntrudeUserLoadComplete();

		// ���� P2P����ȭ ���� üũ
		CheckNpcP2PSyncUser();

		// ���� ���� üũ ���� ��Ŷ
		CheckZombieUserEcho();

		//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT
		CheckDropEventCount();
#endif SERV_ITEM_DROP_EVENT
		//}}

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
		if ( m_spUserManager->ZombieAlert_Tick() == true )
		{
#ifdef SERV_CHOOSE_FASTEST_HOST
			CheckPingScore();
			m_kTimer[TM_CHECK_CHANGE_HOST].restart();
#endif  SERV_CHOOSE_FASTEST_HOST
		}
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

	}
	else if( GetStateID() == KRoomFSM::S_WAIT )
	{
		if( m_tTimer[TE_WAIT_TIMER].elapsed() > 10.0 )
		{
			// ��Ʋ�ʵ� ���� ��� �ð��� 10�ʸ� ������ ���� �Ҹ� ��Ų��!
			StateTransition( KRoomFSM::I_TO_CLOSE );
		}
	}
}
#endif // SERV_BATTLE_FIELD_BOSS

void KBattleFieldRoom::SetRoomInfo( const KRoomInfo& kInfo )
{
    m_cRoomType			= kInfo.m_RoomType;
    m_uiRoomListID		= kInfo.m_uiRoomListID;
    m_bPublic			= kInfo.m_bPublic;
    m_bTeamBalance		= false;
    m_wstrPassword		= kInfo.m_wstrPassword;
    m_cGameType			= CXSLRoom::PGT_TEAM;
    m_cWinningNumKill	= 0;
    m_sWorldID			= kInfo.m_WorldID;
    m_cDifficultyLevel	= kInfo.m_DifficultyLevel;
    m_iDungeonID		= kInfo.m_iDungeonID;
	m_cDungeonMode		= kInfo.m_cDungeonMode;

	if( kInfo.m_cGetItemType <= CXSLRoom::DGIT_NONE || kInfo.m_cGetItemType >= CXSLRoom::DGIT_END )
	{
		START_LOG( cwarn, L"�������� ������ ����Ÿ���� �߸��Ǿ���.!" )
			<< BUILD_LOGc( m_cGetItemType )
			<< BUILD_LOGc( kInfo.m_cGetItemType )
			<< BUILD_LOGc( kInfo.m_iDungeonID )
			<< BUILD_LOGc( kInfo.m_DifficultyLevel )
			<< END_LOG;

		m_cGetItemType		= CXSLRoom::DGIT_RANDOM;		
	}
	else
	{
		m_cGetItemType		= kInfo.m_cGetItemType;
	}

	SetName( kInfo.m_RoomName );
	SetPlayTimeLimit( kInfo.m_fPlayTime );
	SetBattleFieldID( static_cast<SEnum::BATTLE_FIELD_ID>(kInfo.m_iBattleFieldID) );
	m_spUserManager->SetMaxSlot( SiCXSLBattleFieldManager()->GetMaxJoinUserCountInBattleField( static_cast<SEnum::BATTLE_FIELD_ID>(kInfo.m_iBattleFieldID) ) );
}

void KBattleFieldRoom::GetRoomInfo( KRoomInfo& kInfo )
{
	KRoom::GetRoomInfo( kInfo );
    kInfo.m_iBattleFieldID = GetBattleFieldID();
	kInfo.m_MaxSlot = m_spUserManager->GetMaxSlot(); // �ش� ��Ʋ�ʵ常�� �ִ� ���� ������ ��������!
}

//{{ 2012. 10. 16	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
void KBattleFieldRoom::LogToDB_GamePlayNetWorkInfo( IN const UidType iUnitUID, IN const KGamePlayNetworkInfoPackage& kInfo, IN const bool bLeaveRoom )
{
	if( kInfo.m_vecGamePlayNetworkInfo.empty() == false )
	{
		CTime tCurTime = CTime::GetCurrentTime();
		KELOG_COLLECTION_OF_RELAY_AND_P2P_INFO_NOT kPacketToLog;

		BOOST_TEST_FOREACH( const KGamePlayNetworkInfo&, kGamePlayNetworkInfo, kInfo.m_vecGamePlayNetworkInfo )
		{
			if( kGamePlayNetworkInfo.m_iOpponentUnitUID == iUnitUID )
				continue;

			KDBUpdateGamePlayNetworkInfo kDBUpdateInfo;
			kDBUpdateInfo.m_iGameType			  = m_cRoomType;
			kDBUpdateInfo.m_iHolePunchingTryCount = kGamePlayNetworkInfo.m_iHolePunchingTryCount;
			kDBUpdateInfo.m_iP2PPlayTime		  = static_cast<int>(kGamePlayNetworkInfo.m_fP2PPlayTime);		// int�� Ÿ�� ĳ�����ϸ� �ʸ� ���´�.
			kDBUpdateInfo.m_iRelayPlayTime		  = static_cast<int>(kGamePlayNetworkInfo.m_fRelayPlayTime);	// int�� Ÿ�� ĳ�����ϸ� �ʸ� ���´�.
			kDBUpdateInfo.m_wstrRelayServerIP	  = NetCommon::GetLocalIPW();
			kDBUpdateInfo.m_wstrRegDate			  = (const wchar_t*)tCurTime.Format(_T("%Y-%m-%d %H:%M:%S"));
			kPacketToLog.m_vecDBUpdateInfo.push_back( kDBUpdateInfo );
		}

		if( kPacketToLog.m_vecDBUpdateInfo.empty() == false )
		{
			SendToLogDB( ELOG_COLLECTION_OF_RELAY_AND_P2P_INFO_NOT, kPacketToLog );
		}
	}
}
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
//}}

void KBattleFieldRoom::ProcessNativeEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {
	CASE( ERM_OPEN_BATTLE_FIELD_REQ );
	CASE( ERM_JOIN_BATTLE_FIELD_REQ );

   _CASE( ERM_BATTLE_FIELD_JOIN_USER_LOAD_COMPLETE_NOT, KEGS_BATTLE_FIELD_JOIN_USER_LOAD_COMPLETE_NOT );

   _CASE( ERM_BATTLE_FIELD_GAME_LOADING_REQ, KEGS_GAME_LOADING_REQ );

	CASE( ERM_LEAVE_ROOM_REQ );
   _CASE( ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_BATTLE_FIELD_REQ, KERM_LEAVE_ROOM_REQ );
   _CASE( ERM_LEAVE_BATTLE_FIELD_FOR_DUNGEON_GAME_START_REQ, KERM_LEAVE_ROOM_REQ );
   _CASE( ERM_LEAVE_BATTLE_FIELD_FOR_PVP_GAME_START_REQ, KERM_LEAVE_ROOM_REQ );

   _CASE( ERM_NPC_UNIT_CREATE_REQ, KEGS_NPC_UNIT_CREATE_REQ );
   _CASE( ERM_NPC_UNIT_DIE_REQ, KEGS_NPC_UNIT_DIE_REQ );
   _CASE( ERM_USER_UNIT_DIE_REQ, KEGS_USER_UNIT_DIE_REQ );
    CASE_NOPARAM( ERM_USER_UNIT_DIE_COMPLETE_REQ );
   
   _CASE( ERM_GET_ITEM_REQ, KEGS_GET_ITEM_REQ );
	//{{ 2009. 5. 11  ������	�ǽð�������
	CASE( ERM_GET_ITEM_COMPLETE_NOT );
	//}}
   _CASE( ERM_DUNGEON_PLAY_INFO_TO_SERVER_NOT, KEGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT );   
   _CASE( ERM_USE_QUICK_SLOT_NOT, KEGS_USE_QUICK_SLOT_NOT );

   _CASE( ERM_CHECK_INVALID_USER_NOT, UidType );

   _CASE( ERM_RESURRECT_TO_CONTINUE_DUNGEON_REQ, KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ );
	//CASE_NOPARAM( ERM_SKIP_CONTINUE_DUNGEON_NOT );
	
   _CASE( ERM_STOP_DUNGEON_CONTINUE_TIME_REQ, KEGS_STOP_DUNGEON_CONTINUE_TIME_REQ );

   //{{ 2011. 01. 25  ��μ�  ���� Ʈ���� ����(������ ������ - ���� PVP Ȯ��)
#ifdef SERV_WORLD_TRIGGER_RELOCATION
   _CASE( ECN_WORLD_TRIGGER_RELOCATION_REQ, KEGS_WORLD_TRIGGER_RELOCATION_REQ );
#endif SERV_WORLD_TRIGGER_RELOCATION
   //}}
	
	CASE_NOPARAM( ERM_BATTLE_FIELD_NPC_LOAD_COMPLETE_REQ );
//#ifndef SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//	CASE_NOPARAM( ERM_BATTLE_FIELD_NPC_P2P_SYNC_COMPLETE_NOT );
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

	CASE( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT );

	//{{ 2011. 12. 12	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CASE( ERM_DUMP_ROOM_MONSTER_NOT );
	CASE( ERM_DUMP_BATTLE_FIELD_NOT );
	CASE_NOPARAM( ERM_BATTLE_FIELD_ZU_ECHO_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    CASE_NOPARAM( ERM_BATTLE_FIELD_ZOMBIE_ALERT_NOT );
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
	//}}
	//{{ ��Ʋ�ʵ� ���赵 Ȯ�� ġƮ - ��μ�
#ifdef SERV_BATTLE_FIELD_DANGEROUS_CHEAT
	CASE_NOPARAM( ERM_BATTLE_FIELD_DANGEROUS_VALUE_REQ );
#endif SERV_BATTLE_FIELD_DANGEROUS_CHEAT
	//}}
	//{{ ��Ʋ�ʵ� Ŀ�´�Ƽ ����Ʈ Ȯ�� - ��μ�
#ifdef SERV_COMMUNITY_IN_BATTLE_FIELD
	_CASE( ERM_COMMUNITY_IN_BATTLE_FIELD_REQ, KEGS_COMMUNITY_USER_LIST_REQ );
#endif SERV_COMMUNITY_IN_BATTLE_FIELD
	//}}
	//{{ 2012. 11. 20	������		���� ��Ʋ�ʵ� üũ �� ����
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	CASE( ERM_KILL_ZOMBIE_USER_IN_BATTLE_FIELD_NOT );
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	//}}
	//{{ 2013. 03. 12	������	���� ��ȿ �˻�
#ifdef SERV_CHECK_EXIST_MONSTER_UID
	_CASE( ERM_CHECK_EXIST_MONSTER_UID_REQ, KEGS_CHECK_EXIST_MONSTER_UID_REQ );
#endif SERV_CHECK_EXIST_MONSTER_UID
	//}}
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	_CASE( ERM_ADMIN_BATTLE_FIELD_INCREASE_DANGER_REQ, KEGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_REQ );
	_CASE( ERM_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ, KEGS_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ );
	_CASE( ERM_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT, KEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	CASE( ERM_CALL_MY_LOVER_CHECK_ROOM_REQ );
	_CASE( ERM_LEAVE_BATTLE_FIELD_FOR_CALL_MY_LOVER_REQ, KERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ );
	CASE( ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ );
	CASE( ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_WEDDING_HALL_REQ );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-09	// �ڼ���
	_CASE( ERM_LEAVE_ROOM_FOR_WORKINGS_BLOCK_REQ, KERM_LEAVE_ROOM_REQ );
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-07	// �ڼ���
	CASE_NOPARAM( ERM_BOSS_FIELD_INTRUDE_RESTRICTION_REQ );
	CASE( ERM_BOSS_FIELD_LOG_NOT );
#endif // SERV_BATTLE_FIELD_BOSS

    default:
        START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ��� - " << spEvent_->GetIDStr() )
            << BUILD_LOG( spEvent_->m_usEventID );
    }
}

void KBattleFieldRoom::StartGame()
{
    KRoom::StartGame();

	// ��Ʋ�ʵ�� ���½ð�üũ�� ���� �ʽ��ϴ�.
	EnableRemainingPlaytime( false );
	
	const int iPlayerCount = m_spUserManager->GetNumMember();
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-14	// �ڼ���
	m_kMonsterManager.StartGame( GetBattleFieldID(), iPlayerCount, m_kGameManager.GetDangerousValue(), SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD ) );
#else // SERV_BATTLE_FIELD_BOSS
	m_kMonsterManager.StartGame( GetBattleFieldID(), iPlayerCount, m_kGameManager.GetDangerousValue() );
#endif // SERV_BATTLE_FIELD_BOSS
	m_kSecurityManager.StartGame();
	m_kGameManager.StartGame();

	m_nDropItemUID		= 1;
	m_fTotalPlayTime	= 0.0;	

	m_mapDropItem.clear();
    
    const int iPlayingUserCount = m_spUserManager->GetNumPlaying();
    if( iPlayingUserCount < 1 || iPlayingUserCount > 12 )
    {
        START_LOG( cerr, L"�÷��� ���� ���� ���� �̻���." )
            << BUILD_LOG( iPlayingUserCount )
            << END_LOG;
    }

	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	// �ش� ������ Ư�� ������ ���� ����Ʈ�� �������̸鼭 �������� �����ϰ� �ִ��� Ȯ���Ѵ�.
	m_spUserManager->CheckItemEquipQuestCondition( true );
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

}

void KBattleFieldRoom::StartPlay()
{
	//{{ 2010. 8. 25	������	���� �÷��� ���� üũ!
	if( GetStateID() == KRoomFSM::S_PLAY )
	{
		if( m_spUserManager->GetNumMember() == 1 )
		{
			START_LOG( cout, L"�̹� KRoomFSM::S_PLAY�����ε� �� StartPlay�� �Ϸ��� �ϳ�!? ��ŷ�ΰ�?" )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( GetTotalPlayTime() )
				<< BUILD_LOG( m_kTimer[TM_PLAY].elapsed() )
				<< BUILD_LOG( GetDungeonIDAndDif() );

			KRoomUserPtr spUser = m_spUserManager->GetHostUser();
			if( spUser != NULL )
			{
				START_LOG( cout, L"� ������ �̷��� �̻��� ������ �ϴ��� Ȯ���غ���!" )
					<< BUILD_LOG( spUser->GetCID() )
					<< BUILD_LOG( spUser->GetNickName() );

				// ��Ƽ�忡�� ��ŷüũ��Ŷ�� ������!
				//KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
				//kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_PLAY_START;
				//SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );
			}
		}
	}
	//}}

    KRoom::StartPlay();

}

void KBattleFieldRoom::EndPlay()
{
	m_bEndPlay = false;
    KRoom::EndPlay();

    m_fTotalPlayTime = m_kTimer[TM_PLAY].elapsed();

	// �÷��� Ÿ���� ���������� ���Ǿ� �������� Ȯ���غ���!
	if( GetTotalPlayTime() < 0.0 )
	{
		START_LOG( cout, L"�ε��ð��� �� ���� ���� �÷��� �ð��� ���̳ʽ����̴�! ���� �� ���� ����!" )
			<< BUILD_LOG( GetStateIDString() )
			<< BUILD_LOG( GetTotalPlayTime() )
			<< BUILD_LOG( m_kTimer[TM_PLAY].elapsed() )
			<< BUILD_LOG( GetDungeonIDAndDif() )
			<< BUILD_LOG( m_spUserManager->GetNumMember() )
			<< END_LOG;
	}
}

void KBattleFieldRoom::EndGame()
{
	// ��Ȱ�� ��� ���
	const int iPlayerCount = m_spUserManager->GetUsedRessurectionStonePlayerCount();

	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( 0 );
	KSIManager.IncreaseCount( KStatistics::SI_RES_STONE, kKey, KStatistics::eSIColResStone_PlayerCount, iPlayerCount );
	
    KRoom::EndGame();

	m_kMonsterManager.EndGame();
	m_kSecurityManager.EndGame();
	m_kGameManager.EndGame();

	// �����ϴ� ������ �̰����� �ٽ� �ʱ�ȭ�� �غ���.(07.12.28 demiahun)
	m_mapDropItem.clear();
}

int KBattleFieldRoom::DecideWinTeam()
{
	// ��Ʋ�ʵ�� ���а����� ����! ���� ȣ��ȴٰ� �ص� ���Ͱ� �¸��Ѱ����� ó������!
	return CXSLRoom::TN_MONSTER;
}

int KBattleFieldRoom::GetPlayMode()
{
    return m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
}

void KBattleFieldRoom::HandleTimeOutUser( std::map< UidType, UidType >& mapUserCIDNGS )
{
    if( mapUserCIDNGS.empty() )
    {
        return;
    }

    std::map< UidType, UidType >::iterator mit;
    for( mit = mapUserCIDNGS.begin(); mit != mapUserCIDNGS.end(); ++mit )
    {
		KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mit->first );
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"�ش� ������ �������� �ʽ��ϴ�!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		const UidType iUnitUID = mit->first;
		const UidType iPartyUID = spRoomUser->GetPartyUID();

		if( !m_spUserManager->LeaveRoom( mit->first ) )
		{
			START_LOG( cerr, L"�� ������ ����." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
		}

        // ���� �������� ��Ŷ�� ������.
        SendToGSCharacter( mit->second, mit->first, ERM_GAME_LOADING_TIME_OUT_NOT );

		// ���� ��� �����鿡�� ���� ������ ������.
		KEGS_LEAVE_BATTLE_FIELD_NOT kPacketNot;
		kPacketNot.m_cRoomState = GetStateID();
		kPacketNot.m_iUnitUID = iUnitUID;
		kPacketNot.m_iPartyUID = iPartyUID;
		GetRoomSlotInfo( kPacketNot.m_vecSlot );
		BroadCast( ERM_LEAVE_BATTLE_FIELD_NOT, kPacketNot );
    }    
	
	if( GetStateID() == KRoomFSM::S_LOAD  ||  GetStateID() == KRoomFSM::S_RESULT )
	{
		START_LOG( cerr, L"��Ʋ�ʵ忡���� ������� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( GetStateIDString() )
			<< BUILD_LOG( mapUserCIDNGS.size() )
			<< END_LOG;
	}

    // �濡�� ��� ������ �������� ���� �ϴ� WAIT���·� �̵���Ų��!
    if( IsEmpty() )
    {
		StateTransition( KRoomFSM::I_TO_RESULT );
        StateTransition( KRoomFSM::I_TO_WAIT );
		m_tTimer[TE_WAIT_TIMER].restart();
    }
}

void KBattleFieldRoom::DeleteZombie( std::vector<UidType>& vecZombieUnit )
{
	START_LOG( cout2, L"[�˸�] ��Ʋ�ʵ� ���� ���� ����!" )
		<< BUILD_LOG( vecZombieUnit.size() );

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecZombieUnit )
	{
		KRoomUserPtr spUser = m_spUserManager->GetUser( iUnitUID );
		if( IS_NOT_NULL( spUser ) )
		{
			START_LOG( cout2, L"[�˸�] ��Ʋ�ʵ� ���� ���� ����!" )
				<< BUILD_LOG( spUser->GetNickName() )
				<< BUILD_LOG( spUser->IsHost() );

			KERM_CHECK_INVALID_ZOMBIE_USER_NOT kPacket;
			kPacket.m_iRoomUID = GetUID();
			SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_CHECK_INVALID_ZOMBIE_USER_NOT, kPacket );
		}
		else
		{
			START_LOG( cerr, L"������ ������ �̻�.!" )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
		}

		if( m_spUserManager->LeaveRoom( iUnitUID ) == false )
		{
			START_LOG( cerr, L"���� ���� �� ������ ����." )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( vecZombieUnit.size() )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		// ��Ʋ�ʵ� ��Ż�� �����ߴٸ� �� ������ �ѹ� ��������
		SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );
	}

	// �濡�� ��� ������ �������� ���� �ݴ´�.
	if( IsEmpty() == true )
	{
		StateTransition( KRoomFSM::I_TO_CLOSE );
	}
	else
	{
		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecZombieUnit )
		{
			// ���� ��� �����鿡�� ���� ������ ������.
			KEGS_LEAVE_BATTLE_FIELD_NOT kPacketNot;
			kPacketNot.m_iReason = 0;
			kPacketNot.m_cRoomState = GetStateID();
			kPacketNot.m_iUnitUID = iUnitUID;
			kPacketNot.m_iPartyUID = 0;
			GetRoomSlotInfo( kPacketNot.m_vecSlot );
			BroadCast( ERM_LEAVE_BATTLE_FIELD_NOT, kPacketNot );
		}
	}
}

void KBattleFieldRoom::CloseRoom()
{
	// ���� ������ �����Ұ͵��� ��������.
	//{{ 2011. 10. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_kMonsterManager.OnCloseRoom();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	// ��Ʋ�ʵ� ���� ������ ����üũ�� ����
	m_spUserManager->ZU_CheckEnd();
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    m_spUserManager->ZombieAlert_CheckEnd();
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

	// ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
	SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_DELETE );

	//������� ������ �ȵǾ� ������ ���ϴ� �����
	if( GetStateID() == KRoomFSM::S_INIT )
	{
		StateTransition( KRoomFSM::I_TO_WAIT );
	}

	StateTransition( KRoomFSM::I_TO_CLOSE );

	//{{ 2013. 01. 07	������	�濡�� �ʵ�� ���ƿ��� ���ϴ� ���� ����
#ifdef SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
	// �ʱ�ȭ �ϱ� ���� �˻� �ѹ���~
	if( m_spUserManager->GetNumTotalParty() != 0  ||  m_spUserManager->GetNumMemberTotalParty() != 0 )
	{
		START_LOG( cout, L"[���] ��Ʋ�ʵ带 �������� �ϴµ� �����Ǿ��־���� ������ ���� ����ִ�! �̷��� ���װ�.." )
			<< BUILD_LOG( m_spUserManager->GetNumTotalParty() )
			<< BUILD_LOG( m_spUserManager->GetNumMemberTotalParty() );
	}

	// ���� �ݱ� ���� �ʱ�ȭ �ѹ� ������!
	m_spUserManager->Reset();
#endif SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-12-03	// �ڼ���
	if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD ) == true )
	{
		// �α� ���� DB ���� �� �ʱ�ȭ
		SendToLogDB( DBE_BOSS_FIELD_LOG_NOT, m_kBossFieldLog );
		m_kBossFieldLog.clear();
	}
#endif // SERV_BATTLE_FIELD_BOSS

	KCnBaseRoom::CloseRoom();
}

KDropItemData KBattleFieldRoom::CreateItemProcess( IN const int iItemID, 
												   IN const char cNpcLevel,
												   IN const int iNpcMaxHP,
												   IN const std::map< UidType, float >& mapDamageByUnit,
												   IN const bool bIsGP /*= false*/, 
												   IN const int iED /*= 0*/ )
{
	KDropItemData	kDropItem;	//not ��Ŷ�� �� ���� ����������
	ITEM_DATA		sItemData;	//�������� ���������� ����������

	int iDropItemUID = m_nDropItemUID++;

	kDropItem.m_iItemID					= iItemID;
	kDropItem.m_iDropItemUID			= iDropItemUID;
	{
		std::map< UidType, float >::const_iterator mit;
		for( mit = mapDamageByUnit.begin(); mit != mapDamageByUnit.end(); ++mit )
		{
			kDropItem.m_setGetPossibleUnitUID.insert( mit->first );
		}
	}	

	if( (rand()%2) == 1 )
		kDropItem.m_bLeft = true;
	else
		kDropItem.m_bLeft = false;

	kDropItem.m_fSpeed	= (float)(rand()%800) + 1.0f;	

	//�������� ����߸��� ���� ���� ���������� üũ,.
	sItemData.m_iItemID	 	 	= iItemID;
	sItemData.m_iDropItemUID 	= iDropItemUID;
	sItemData.m_bIsGet		 	= false;
	sItemData.m_bIsGP		 	= bIsGP;
	sItemData.m_cNpcLevel		= cNpcLevel;
	sItemData.m_iNpcMaxHP		= iNpcMaxHP;
	sItemData.m_mapDamageByUser	= mapDamageByUnit;

	if( bIsGP == true )
	{
		sItemData.m_iED = iED;
	}
	//{{ 2013. 1. 14	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
	else
	{
		SiKDropTable()->EnchantItemDropEventProcess( iItemID, sItemData.m_cEnchantLevel, sItemData.m_bNotify );
		kDropItem.m_cEnchantLevel = sItemData.m_cEnchantLevel;
	}
#endif SERV_ENCHANT_ITEM_DROP_EVENT
	//}}

	m_mapDropItem.insert( std::make_pair( iDropItemUID, sItemData ) );

	return kDropItem;
}

void KBattleFieldRoom::CheckLoadingSuccess()
{
	// �ƹ��͵� ���� �ʽ��ϴ�. �ε����¸� ���� �ʱ� ����!
}

void KBattleFieldRoom::CheckMonsterRespawn()
{
	// ���� �������� ���� �������� üũ�մϴ�!
	if( m_tTimer[TE_MONSTER_RESPAWN].elapsed() < 1.0 )
		return;

	m_tTimer[TE_MONSTER_RESPAWN].restart();

	// ������ �ʿ� ����
	//const int iPlayerCount = m_spUserManager->GetNumMember();
	const int iPlayerCount = SiCXSLBattleFieldManager()->GetMaxJoinUserCountInBattleField( m_eBattleFieldID ); // �ϴ� �ش� ��Ʋ�ʵ忡 �ִ� ���� ������ �ο��� ��ŭ �ִٰ� �����Ѵ�.

	//////////////////////////////////////////////////////////////////////////
	// ���� ������ �ִ� �����ð� üũ
	DWORD dwElapTime = ::GetTickCount();  

	// ���� ������ üũ
	KEGS_NPC_UNIT_CREATE_NOT kPacketNot;
	kPacketNot.m_iBattleFieldID = GetBattleFieldID();
	if( m_kMonsterManager.CheckRespawnMonster( GetBattleFieldID(), 
											   iPlayerCount, 
											   m_kGameManager.GetDangerousEventInfo(), 
											   kPacketNot ) == true )
	{
		if( kPacketNot.m_vecNPCUnitAck.empty() == false )
		{
			// ���� ������ ó���� �Ϸ�Ǿ��ٸ� 
			BroadCast( ERM_NPC_UNIT_CREATE_NOT, kPacketNot, KRoomUserManager::UL_NPC_LOAD_USER );
		}
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( m_dwMonsterRespawnCheckTick < dwElapTime )
	{
		START_LOG( cout, L"[�˸�]���� ������ üũ Ÿ�� �ִ� �ð� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( dwElapTime )
			<< BUILD_LOG( m_dwMonsterRespawnCheckTick );

		m_dwMonsterRespawnCheckTick = dwElapTime;
	}
	//////////////////////////////////////////////////////////////////////////	
}

void KBattleFieldRoom::CheckDangerousEvent()
{
	// ���� �������� ���� �̺�Ʈ�� üũ�մϴ�.
	if( m_tTimer[TE_DANGEROUS_EVENT].elapsed() < 5.0 )
		return;

	m_tTimer[TE_DANGEROUS_EVENT].restart();

	if( m_kGameManager.CheckAndDeleteReservedDangerousEvent( KDangerousEventInfo::DE_WARNING_MESSAGE ) == true )
	{
		KEGS_BATTLE_FIELD_EVENT_MESSAGE_NOT kNot;
		kNot.m_cEventType = KEGS_BATTLE_FIELD_EVENT_MESSAGE_NOT::BET_WARNING_MESSAGE;
		BroadCast( ERM_BATTLE_FIELD_EVENT_MESSAGE_NOT, kNot, KRoomUserManager::UL_NPC_LOAD_USER );
	}
}

void KBattleFieldRoom::CheckContinueTimer()
{
    // ��Ƽ�� Ÿ�̸Ӹ� üũ�Ͽ� 9�ʰ� ������ 
}

bool KBattleFieldRoom::IsDropDieState( char cState )
{
	switch( cState )
	{
	case KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_BY_USER:
	case KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_SELF:
	case KEGS_NPC_UNIT_DIE_REQ::NDS_BONUS_DROP:
		return true;

	case KEGS_NPC_UNIT_DIE_REQ::NDS_ALLY:
	case KEGS_NPC_UNIT_DIE_REQ::NDS_ESCAPE:
		return false;
	}

	START_LOG( cerr, L"npc������ ���� ���°� �̻��մϴ�." )
		<< BUILD_LOGc( cState )
		<< END_LOG;

	return false;
}

void KBattleFieldRoom::CheckIntrudeUserLoadComplete()
{
	// ���� �������� ���� ����ȭ ���� üũ
	if( m_tTimer[TE_INTRUDE_USER_LOAD].elapsed() < 5.0 )
		return;

	m_tTimer[TE_INTRUDE_USER_LOAD].restart();

	// �ٸ� ������κ����� �ε��� �Ϸ�� ������ ����Ʈ�� ����!
	std::vector< UidType > vecNpcLoadUserList;
	m_spUserManager->CheckAndGetIntrudeUnitLoadCompleteUserList( vecNpcLoadUserList );

	if( vecNpcLoadUserList.empty() == false )
	{
		// �ش� �����ڿ��� �� ���� ����Ʈ�� �̾Ƴ���!
		KEGS_BATTLE_FIELD_NPC_LOAD_NOT kPacket;
		kPacket.m_iBattleFieldID = GetBattleFieldID();
		m_kMonsterManager.GetAliveMonsterList( kPacket.m_vecNPCData, kPacket.m_mapAttribNpcInfo );

		//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
		KERM_UPDATE_QUEST_INFO_NOT kNot; 
		std::vector< int > vecNPC;
		BOOST_TEST_FOREACH( KNPCUnitReq&, kData, kPacket.m_vecNPCData )
		{
			kNot.m_vecNPC.push_back( kData.m_NPCID );
		}
#endif SERV_REFORM_QUEST
		//}}

		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecNpcLoadUserList )
		{
			KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iUnitUID );
			if( spRoomUser == NULL )
			{
				START_LOG( cerr, L"�ش� ĳ���͸� ã�� �� �����ϴ�!" )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
				continue;
			}

			SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_BATTLE_FIELD_NPC_LOAD_NOT, kPacket );

			//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			std::vector< std::vector<KNPCUnitReq> >	vecTempMiddleBossList;
			m_kMonsterManager.GetMiddleBossMonsterList( vecTempMiddleBossList );

			BOOST_TEST_FOREACH( std::vector<KNPCUnitReq>&, vecList, vecTempMiddleBossList )
			{
				KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT kPacketNot;

				BOOST_TEST_FOREACH( KNPCUnitReq&, kNpc, vecList )
				{
					KNPCUnitNot kMiddleBoss;
					kMiddleBoss.m_kNPCUnitReq = kNpc;

					kPacketNot.m_kCreatePacket.m_vecNPCUnitAck.push_back( kMiddleBoss );
				}

				kPacketNot.m_kCreatePacket.m_iBattleFieldID = GetBattleFieldID();
				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT, kPacketNot );
			}		
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
			//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
			std::vector< std::vector<KNPCUnitReq> >	vecTempEventBossList;
			m_kMonsterManager.GetEventBossMonsterList( vecTempEventBossList );

			BOOST_TEST_FOREACH( std::vector<KNPCUnitReq>&, vecList, vecTempEventBossList )
			{
				KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT kPacketNot;

				BOOST_TEST_FOREACH( KNPCUnitReq&, kNpc, vecList )
				{
					KNPCUnitNot kEventBoss;
					kEventBoss.m_kNPCUnitReq = kNpc;

					kPacketNot.m_kCreatePacket.m_vecNPCUnitAck.push_back( kEventBoss );
				}

				kPacketNot.m_kCreatePacket.m_iBattleFieldID = GetBattleFieldID();
				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT, kPacketNot );
			}		
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

			//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
			if( kNot.m_vecNPC.empty() == true )
				continue;

			kNot.m_isDungeon = false;

			if( spRoomUser->CheckFindNPCQuest( GetBattleFieldID(), kNot.m_vecNPC, false ) == true )
			{
				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_UPDATE_QUEST_INFO_NOT, kNot );
			}
#endif SERV_REFORM_QUEST
			//}}
		}
		
	}
}

void KBattleFieldRoom::CheckNpcP2PSyncUser( IN const bool bForce /*= false*/ )
{
	// ���� �������� ���� ����ȭ ���� üũ
	if( m_tTimer[TE_NPC_P2P_SYNC_USER_CHECK].elapsed() < 3.0  &&  bForce == false )
		return;

	m_tTimer[TE_NPC_P2P_SYNC_USER_CHECK].restart();

	KEGS_BATTLE_FIELD_NPC_P2P_SYNC_NOT kNot;
	m_spUserManager->GetBattleFieldNpcSyncSubjectsList( kNot.m_vecNonNpcSyncUserList );
	if( kNot.m_vecNonNpcSyncUserList.empty() )
		return;

	KRoomUserPtr spRoomUser = m_spUserManager->GetHostUser();
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"������ ã�µ��� �����Ͽ����ϴ�! ���� �Ͼ�� �ȵǴ� ����!" )
			<< BUILD_LOG( kNot.m_vecNonNpcSyncUserList.size() )
			<< END_LOG;
		return;
	}

	SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_BATTLE_FIELD_NPC_P2P_SYNC_NOT, kNot );	
}

void KBattleFieldRoom::CheckZombieUserEcho()
{
	if( m_tTimer[TE_ZOMBIE_USER_ECHO].elapsed() < GetKCnRoomManager()->GetBattleFieldZUEchoTime() )
		return;

	m_tTimer[TE_ZOMBIE_USER_ECHO].restart();

	for( int idx = 0; idx < m_spRoomUserManager->GetNumMember(); ++idx )
	{
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( idx );
		if( IS_NULL( spRoomUser ) )
			continue;

#ifdef  SERV_PVP_NEW_SYSTEM
		if( spRoomUser->IsPvpNpc() == true )
			continue;
#endif  SERV_PVP_NEW_SYSTEM

		KERM_BATTLE_FIELD_ZU_ECHO_REQ kPacket;
		kPacket.m_iUnitUID = spRoomUser->GetCID();
		//{{ 2012. 11. 20	������		��Ʋ�ʵ� ���� ���� üũ ��ȭ
#ifdef SERV_ADVANCED_BATTLE_FIELD_ZOMBIE_USER_CHECK
		kPacket.m_iRoomUID = GetUID();
#endif SERV_ADVANCED_BATTLE_FIELD_ZOMBIE_USER_CHECK
		//}}

		SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_BATTLE_FIELD_ZU_ECHO_REQ, kPacket );
	}
}

void KBattleFieldRoom::GetBattleFieldRoomInfo( OUT KBattleFieldRoomInfo& kInfo )
{
	kInfo.m_iBattleFieldID = GetBattleFieldID();
	kInfo.m_RoomUID = GetUID();
	kInfo.m_MaxSlot = m_spUserManager->GetMaxSlot(); // �ش� ��Ʋ�ʵ常�� �ִ� ���� ������ ��������!
	m_spUserManager->GetAllPartyMemberListInfo( kInfo.m_mapUnitUIDPartyUID, kInfo.m_mapPartyList );
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-06	// �ڼ���
	kInfo.m_byteBossFieldState = m_byteBossFieldState;
#endif // SERV_BATTLE_FIELD_BOSS
}

void KBattleFieldRoom::SendRoomListInfo( IN const int iCode )
{
	KERM_BATTLE_FIELD_ROOM_INFO_NOT kPacket;
	kPacket.m_cType = static_cast<char>(iCode);
	GetBattleFieldRoomInfo( kPacket.m_kBFRoomInfo );
	BroadCastAllGSServer( ERM_BATTLE_FIELD_ROOM_INFO_NOT, kPacket );
}

//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KBattleFieldRoom::OnLeaveRoom( IN const UidType iLAST_SENDER_UID, IN const UidType iFIRST_SENDER_UID, IN const KERM_LEAVE_ROOM_REQ& kPacket_, IN const u_short usEventIDAck )
{
	KRoom::OnLeaveRoom( iLAST_SENDER_UID, iFIRST_SENDER_UID, kPacket_, usEventIDAck );

	//=== ������ ��Ż ���ó�� ===
	START_LOG( clog, L"=== BATTLE FIELD : ��Ʋ�ʵ�� ��Ż�Ѵٰ� ���Ƽ�� ���� �ʽ��ϴ�. ===" )
		<< BUILD_LOG( iFIRST_SENDER_UID )
		<< END_LOG;	

	KEGS_LEAVE_ROOM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
	
	// ��Ʋ�ʵ� ������ ���� ��Ż�̸� �ʵ� ���� ������ ���� �־�����!
	if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_27 )
	{
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_27;		
	}
	else
	{
		kPacket.m_iReason = NetError::NET_OK;
	}
	
	KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iFIRST_SENDER_UID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"������ ã�� �� �����ϴ�!" )
			<< BUILD_LOG( iFIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	// �濡�� ��Ż�ϱ� ���� RoomUserInfo������ ����
	KELOG_BATTLE_FIELD_LEAVE_LOG_NOT kPacketToLog;
	MakePacket_BattleFieldLeaveLog( iFIRST_SENDER_UID, kPacket_.m_iReason, kPacketToLog );

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-28	// �ڼ���
	_MakeBossFieldUserLogAtLeave( iFIRST_SENDER_UID );
#endif // SERV_BATTLE_FIELD_BOSS

	// ���� ���� ����.
	if( m_spUserManager->LeaveRoom( iFIRST_SENDER_UID ) == false )
	{
		// ���Կ��� ������ ����.
		kPacket.m_iOK = NetError::ERR_SLOT_04;
		if( kPacket_.m_bSendAck )
		{
			SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, usEventIDAck, kPacket );
		}
		return;
	}
	else
	{
		// ��Ʋ�ʵ忡�� ��Ż�ϴ°� �����ߴٸ� �ӽ� �κ��丮�� ����������!
		SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, ERM_CLEAR_TEMP_INVENTORY_NOT );

		if( kPacket_.m_bSendAck )
		{
			SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, usEventIDAck, kPacket );
		}

		// ��Ʋ�ʵ� ��Ż �α� �����
		SendToLogDB( ELOG_BATTLE_FIELD_LEAVE_LOG_NOT, kPacketToLog );

		//{{ 2012. 11. 15	�ڼ���	Field PT �α� �߰�
#ifdef SERV_FIELD_PLAY_TIME_LOG
		{
			KERM_FIELD_USER_STATISTICS_NOT kPacketNot;
			kPacketNot.m_iGamePlayTime	= static_cast<int>( spRoomUser->GetPlayTime() );
			SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, ERM_FIELD_USER_STATISTICS_NOT, kPacketNot );
		}
#endif SERV_FIELD_PLAY_TIME_LOG
		//}}

		//{{ 2012. 10. 16	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
		LogToDB_GamePlayNetWorkInfo( iFIRST_SENDER_UID, kPacket_.m_kGamePlayNetworkInfo, true );
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
		//}}

		// ��Ʋ�ʵ� ��Ż�� �����ߴٸ� �� ������ �ѹ� ��������
		SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );

		//{{ ��Ʋ �ʵ忡�� ���� ���� ���� ���� - ��μ� -- 2012�� 11�� 15�� ��ġ ����
#ifdef SERV_BATTLE_FIELD_NPC_CREATE_ERROR
		std::vector< int > vecDeleteNpcList;
		m_kMonsterManager.GetNpcOwnerListByUnitUID( iFIRST_SENDER_UID, vecDeleteNpcList );

		BOOST_TEST_FOREACH( int, iNPCUID, vecDeleteNpcList )
		{
			LIF( m_kMonsterManager.SetMonsterDie( iNPCUID, 0 ) );

			KERM_NPC_UNIT_DIE_NOT kNpcDietNot;
			kNpcDietNot.m_nDieNPCUID		 = iNPCUID;
			kNpcDietNot.m_bNoDrop			 = true;
			kNpcDietNot.m_cUserCount		 = 0;
			kNpcDietNot.m_EXPList.clear();
			
			BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
		}
#endif SERV_BATTLE_FIELD_NPC_CREATE_ERROR
		//}
	}

	if( IsEmpty() == false )
	{
		// ���� ��� �����鿡�� ���� ������ ������.
		KEGS_LEAVE_BATTLE_FIELD_NOT kPacketNot;
		kPacketNot.m_iReason = usEventIDAck; // ���� �Ǿ����� ACK��ŶŸ���� ����! ���߿� �����ؼ� ó���Ҽ� �ֵ���
		kPacketNot.m_cRoomState = GetStateID();
		kPacketNot.m_iUnitUID = iFIRST_SENDER_UID;
		kPacketNot.m_iPartyUID = spRoomUser->GetPartyUID();
		GetRoomSlotInfo( kPacketNot.m_vecSlot );
		BroadCast( ERM_LEAVE_BATTLE_FIELD_NOT, kPacketNot );
	}

	if( GetStateID() == KRoomFSM::S_LOAD )
	{
		START_LOG( cerr, L"��Ʋ�ʵ忡���� �ε� ���¸� ���� �ʽ��ϴ�!" )
			<< BUILD_LOG( iFIRST_SENDER_UID )
			<< END_LOG;
	}

	// �濡�� ��� ������ �������� �ϴ� WAIT���·� �̵���Ų��!
	if( IsEmpty() == true && IsEmpty( KBattleFieldUserManager::UT_OBSERVER ) == true )
	{
		StateTransition( KRoomFSM::I_TO_RESULT );
		StateTransition( KRoomFSM::I_TO_WAIT );
		m_tTimer[TE_WAIT_TIMER].restart();
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KBattleFieldRoom::IncreasePartyFever( IN const std::map< UidType, float >& mapDamageByUser )
{
	std::set< UidType > setIncreasedPartyUID;

	std::map< UidType, float >::const_iterator mit;
	for( mit = mapDamageByUser.begin(); mit != mapDamageByUser.end(); ++mit )
	{
		KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mit->first );
		if( IS_NULL( spRoomUser ) )
		{
			// �ð����� �濡�� �̹� ���� ��Ż�� ������ ��쿣 �������� ��Ȳ�Դϴ�. ���� �����α׷����� cwarn���� ����ϴ�.
			START_LOG( cwarn, L"�ش� ������ �����ϴ�!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return;
		}

		if( spRoomUser->GetPartyUID() == 0 )
			continue;

		if( setIncreasedPartyUID.find( spRoomUser->GetPartyUID() ) != setIncreasedPartyUID.end() )
			continue;

		// ���� ��Ƽ���� �ʵ忡 ������ �н�
		//if( m_spUserManager->GetSamePartyMember( iKillUnitUID ) <= 1 )
		//	return;

		// ��Ƽ �ǹ� ���!
		KERM_INCREASE_PARTY_FEVER_NOT kNot;
		kNot.m_iPartyUID = spRoomUser->GetPartyUID();
		kNot.m_cRoomType = CXSLRoom::RT_BATTLE_FIELD;
		kNot.m_sSubStageNpcCount = m_kMonsterManager.GetAtStartedMonsterCount();
		SendToGSServer( spRoomUser->GetGSUID(), ERM_INCREASE_PARTY_FEVER_NOT, kNot );

		// �ǹ� ��ġ ������ ��Ƽ�� ����!
		setIncreasedPartyUID.insert( spRoomUser->GetPartyUID() );
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

IMPL_ON_FUNC( ERM_OPEN_BATTLE_FIELD_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_INIT ), ERM_OPEN_BATTLE_FIELD_ACK, KERM_OPEN_BATTLE_FIELD_ACK );

	// ��Ʋ�ʵ� ID ����
	if( SiCXSLBattleFieldManager()->IsExistBattleFieldID( static_cast<SEnum::BATTLE_FIELD_ID>(kPacket_.m_kRoomInfo.m_iBattleFieldID) ) == false )
	{
		START_LOG( cerr, L"��ȿ���� ���� ��Ʋ�ʵ�ID�Դϴ�!" )
			<< BUILD_LOG( kPacket_.m_kRoomInfo.m_iBattleFieldID )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_iStartPosIndex )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_vecPartyMemberList.size() )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_bMoveForMyParty )
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_bNowBattleFieldPositionInfoStartPosition )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_usBattleFieldPositionValue )
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
			<< END_LOG;
	}

	//{{ 2010. 03. 14  ������	�� �й� ��� ����
//#ifdef SERV_ROOM_COUNT
	SendRoomCountInfo( CXSLRoom::RT_BATTLE_FIELD, true );
//#endif SERV_ROOM_COUNT
	//}}

    // �� �ɼ� ���� ����.
	SetRoomInfo( kPacket_.m_kRoomInfo );

    // �� �����̹Ƿ� ���� ������ �ʱ�ȭ �Ѵ�.
    m_spUserManager->Reset();
    m_spUserManager->AssignTeam( -1 );

    // ���Կ� ���� ��û�ڸ� �ִ´�.
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-05	// �ڼ���
	InitBossFieldCreateInfo( kPacket_.m_KBossFieldCreateInfo );

	if( EnterRoom( kPacket_.m_kRoomUserInfo
				 , kPacket_.m_kBattleFieldJoinInfo
				 , false
				 ) == false )
#else // SERV_BATTLE_FIELD_BOSS
    if( m_spUserManager->EnterRoom( kPacket_.m_kRoomUserInfo
								  , kPacket_.m_kBattleFieldJoinInfo
								  , false
								  ) == false )
#endif // SERV_BATTLE_FIELD_BOSS
	{
        START_LOG( cerr, L"�� ���� ����. ��Ʋ�ʵ带 �����ϴ°��ε� ���⼭ ���а� ����! ���� �Ͼ���� �ȵǴ� ����!" )
            << BUILD_LOG( FIRST_SENDER_UID )
            << BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetBattleFieldID() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
            << END_LOG;

        kPacket.m_iOK = NetError::GetLastError();
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_BATTLE_FIELD_ACK, kPacket );
        return;
    }

	//080409.hoons. ���� ����Ʈ�� �����Ѵ�.
	m_spUserManager->SetTutorialUIDList( FIRST_SENDER_UID, kPacket_.m_vecStudentUnitUID );

    // �� ���¸� wait �� �ٲ۴�.
    StateTransition( KRoomFSM::I_TO_WAIT );
	m_tTimer[TE_WAIT_TIMER].restart();
    m_kTimer[TM_WAIT].restart();

	//����������¸� ����� �ٲ۴�.
	m_spUserManager->SetAllReady( true );

	//gm �̻��̸� ���ͼ����� ip�� �־�����.
	if( m_spUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == true )
	{
		kPacket.m_wstrCNIP = NetCommon::GetLocalIPW();
	}

	// ���� �ʱ�ȭ.
	StartGame();

	// ������ �� �ε� ���� ���� ��Ʋ�ʵ忡���� �ٷ� ������ �����Ѵ�!
	StartPlay();

    // �� ���� ��û�ڿ��� ack�� ������.
    kPacket.m_iOK = NetError::NET_OK;
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
    kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	kPacket.m_StartPosIndex = kPacket_.m_kBattleFieldJoinInfo.m_iStartPosIndex;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	GetRoomInfo( kPacket.m_RoomInfo );
	GetRoomSlotInfo( kPacket.m_vecSlot );
	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	kPacket.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
	kPacket.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
#endif SERV_CHOOSE_FASTEST_HOST
	//}
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-05	// �ڼ���
	kPacket.m_bBossField = SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD );
	if( kPacket.m_bBossField == true )
	{
		kPacket.m_tRemainFieldHoldingTime	= m_tFieldHoldingTime;
		kPacket.m_kBossFieldJoinInfo		= kPacket_.m_kBossFieldJoinInfo;
	}
#endif // SERV_BATTLE_FIELD_BOSS
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_BATTLE_FIELD_ACK, kPacket );

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-06	// �ڼ���
	// ��Ʋ �ʵ� ���� �� UDP ���� ��û ������
	if( ( kPacket.m_iOK == NetError::NET_OK ) &&
		( SiKGameSysVal()->GetLanBugOutDisconnectCheckStartOnGameLoadingReq() == false ) &&
		( SiKGameSysVal()->GetLanBugOutVerify() == true ) &&
		( IsLanBugVerifyUser( FIRST_SENDER_UID ) == false ) &&
		( m_spUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == false )
		)
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_VERIFY_NOT, GetUID(), FIRST_SENDER_UID );
		AddLanBugVerifyUser( FIRST_SENDER_UID );
	}
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

	//������� �������� üũ����.
	m_spUserManager->ZU_CheckStart();
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    m_spUserManager->ZombieAlert_CheckStart();
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

	// �� ���� ����
	SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );

	//////////////////////////////////////////////////////////////////////////	
	// [����!!!] �ݵ�� �渮��Ʈ�� ���� ���� ���Ŀ� �� ��Ŷ�� ������ �Ѵ�! ������ ���͸� ���� ��Ŷ�� �ٽ� ���!
	if( kPacket_.m_kReturnToFieldInfo.m_iRoomUID != 0 )
	{
		KERM_COMPLETE_RETURN_TO_FIELD_NOT kPacketToGS;
		kPacketToGS.m_kReturnToFieldInfo = kPacket_.m_kReturnToFieldInfo;
		SendToGSServer( LAST_SENDER_UID, ERM_COMPLETE_RETURN_TO_FIELD_NOT, kPacketToGS );
	}
}

IMPL_ON_FUNC( ERM_JOIN_BATTLE_FIELD_REQ )
{
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-08	// �ڼ���
	KERM_JOIN_BATTLE_FIELD_ACK kPacket;
#else // SERV_BATTLE_FIELD_BOSS
	KEGS_JOIN_BATTLE_FIELD_ACK kPacket;
#endif // SERV_BATTLE_FIELD_BOSS

	kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;

	if( GetStateID() != KRoomFSM::S_PLAY  &&  GetStateID() != KRoomFSM::S_WAIT )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_09;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	// ����� ã�� �Դ��� ����!
	if( GetBattleFieldID() != kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID )
	{
		START_LOG( cerr, L"�ٸ� ��Ʋ�ʵ忡 �����ߴ�! �� ���״�!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetBattleFieldID() )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_05;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-07	// �ڼ���
	if( SEnum::CheckFlag( m_byteBossFieldState, ( SEnum::BFRS_BOSS_FIELD | SEnum::BFRS_INTRUDE_RESTRICTION ) ) == true )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_55;	// ���� �ʵ� ���忡 �����Ͽ����ϴ�. �ٽ� �õ� ���ּ���.
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}
#endif // SERV_BATTLE_FIELD_BOSS

	// ��Ʋ�ʵ忡 ���� ó�� �Ѵ�!
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-28	// �ڼ���
	if( EnterRoom( kPacket_.m_kRoomUserInfo
				 , kPacket_.m_kBattleFieldJoinInfo
				 , false
				 ) == false )
#else // SERV_BATTLE_FIELD_BOSS
	if( m_spUserManager->EnterRoom( kPacket_.m_kRoomUserInfo
								  , kPacket_.m_kBattleFieldJoinInfo
								  , false
								  ) == false )
#endif // SERV_BATTLE_FIELD_BOSS
	{
		START_LOG( cerr, L"�� ���� ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	if( m_spUserManager->SetReady( FIRST_SENDER_UID, true ) == false )
	{
		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_06;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	if( m_spUserManager->StartGame( FIRST_SENDER_UID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_06;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

#ifdef SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS// �۾���¥: 2013-06-10	// �ڼ���
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_BUFF_INFO_NOT );
#endif // SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS

	// �ش� ������ �ٷ� ���� ���� ó�� ����!
	if( m_spUserManager->StartPlay( kPacket_.m_kRoomUserInfo.m_nUnitUID ) == false )
	{
		START_LOG( cerr, L"���� ���� ó�� ����!" )
			<< BUILD_LOG( kPacket_.m_kRoomUserInfo.m_nUnitUID )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_06;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	// �� ���� ��ȭ.
	if( GetStateID() == KRoomFSM::S_WAIT )
	{
        StateTransition( KRoomFSM::I_TO_LOAD );
		StateTransition( KRoomFSM::I_TO_PLAY );
	}

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    m_spUserManager->ZombieAlert_CheckStart( kPacket_.m_kRoomUserInfo.m_nUnitUID );
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

	// ������ �������� ack�� ������.
	kPacket.m_iOK = NetError::NET_OK;
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
    kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	kPacket.m_iLastTouchIndex = kPacket_.m_kBattleFieldJoinInfo.m_iStartPosIndex;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	GetRoomInfo( kPacket.m_RoomInfo );
	GetRoomSlotInfo( kPacket.m_vecSlot );
	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	kPacket.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
	kPacket.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
#endif SERV_CHOOSE_FASTEST_HOST
	//}
	//{{ 2012. 11. 21	������		�����ڿ��� ĳ���� ���� ���� ������
#ifdef SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
	m_spUserManager->GetEnterCashShopUserList( kPacket.m_vecEnterCashShopUser );
#endif SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
	//}}
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-05	// �ڼ���
	kPacket.m_bBossField = SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD );

	if( kPacket.m_bBossField == true )
	{
		const __time64_t tCurrentTime = CTime::GetCurrentTime().GetTime();

		if( tCurrentTime < m_tFieldCloseTime )
		{
			kPacket.m_tRemainFieldHoldingTime = m_tFieldCloseTime - tCurrentTime;
		}

		kPacket.m_kBossFieldJoinInfo = kPacket_.m_kBossFieldJoinInfo;
	}
#endif // SERV_BATTLE_FIELD_BOSS

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_BATTLE_FIELD_ACK, kPacket );

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-06	// �ڼ���
	// ��Ʋ �ʵ� ���� �� UDP ���� ��û ������
	if( ( kPacket.m_iOK == NetError::NET_OK ) &&
		( SiKGameSysVal()->GetLanBugOutDisconnectCheckStartOnGameLoadingReq() == false ) &&
		( SiKGameSysVal()->GetLanBugOutVerify() == true ) &&
		( IsLanBugVerifyUser( FIRST_SENDER_UID ) == false ) &&
		( m_spUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == false )
		)
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_VERIFY_NOT, GetUID(), FIRST_SENDER_UID );
		AddLanBugVerifyUser( FIRST_SENDER_UID );
	}
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

	// ���� ��� �������� NOT�� ������.
	KEGS_JOIN_BATTLE_FIELD_NOT kPacketNot;
	//080425.hoons.������ ���º� ���������� �����ؼ� �����ش�.
	m_spUserManager->GetRoomSlotInfo( FIRST_SENDER_UID, kPacketNot.m_JoinSlot, KBattleFieldUserManager::UT_GAME );
	kPacketNot.m_StartPosIndex = kPacket_.m_kBattleFieldJoinInfo.m_iStartPosIndex;
	BroadCast( ERM_JOIN_BATTLE_FIELD_NOT, kPacketNot );

	// ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
	SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );

	//////////////////////////////////////////////////////////////////////////	
	// [����!!!] �ݵ�� �渮��Ʈ�� ���� ���� ���Ŀ� �� ��Ŷ�� ������ �Ѵ�! ������ ���͸� ���� ��Ŷ�� �ٽ� ���!
	if( kPacket_.m_kReturnToFieldInfo.m_iRoomUID != 0 )
	{
		KERM_COMPLETE_RETURN_TO_FIELD_NOT kPacketToGS;
		kPacketToGS.m_kReturnToFieldInfo = kPacket_.m_kReturnToFieldInfo;
		SendToGSServer( LAST_SENDER_UID, ERM_COMPLETE_RETURN_TO_FIELD_NOT, kPacketToGS );
	}
}

_IMPL_ON_FUNC( ERM_BATTLE_FIELD_JOIN_USER_LOAD_COMPLETE_NOT, KEGS_BATTLE_FIELD_JOIN_USER_LOAD_COMPLETE_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"ó�� ������ ���°� �ƴմϴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}
	
	if( m_spUserManager->UnregIntrudeUnitLoadUserList( kPacket_.m_iLoadedUnitUID, FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"������ �ε� �Ϸ� ���� ����� �����Ͽ����ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_iLoadedUnitUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}	
}

_IMPL_ON_FUNC( ERM_BATTLE_FIELD_GAME_LOADING_REQ, KEGS_GAME_LOADING_REQ )
{
	VERIFY_ROOM_STATE( ( 2, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY ), ERM_BATTLE_FIELD_GAME_LOADING_ACK, KPacketOK );

	// loading ���¸� ��ȭ��Ų��.
	if( m_spRoomUserManager->SetLoadingProgress( FIRST_SENDER_UID, kPacket_.m_iLoadingProgress ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_13;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BATTLE_FIELD_GAME_LOADING_ACK, kPacket );
		return;
	}

	// ���� ������� ack�� ������.
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BATTLE_FIELD_GAME_LOADING_ACK, kPacket );

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-06	// �ڼ���
	// ��Ʋ �ʵ� ���� �� UDP ���� ��û ������
	if( ( kPacket.m_iOK == NetError::NET_OK ) &&
		( SiKGameSysVal()->GetLanBugOutDisconnectCheckStartOnGameLoadingReq() == true ) &&
		( SiKGameSysVal()->GetLanBugOutVerify() == true ) &&
		( IsLanBugVerifyUser( FIRST_SENDER_UID ) == false ) &&
		( m_spUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == false )
		)
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_VERIFY_NOT, GetUID(), FIRST_SENDER_UID );
		AddLanBugVerifyUser( FIRST_SENDER_UID );
	}
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

	// ���� ��� �����鿡�� not�� ������.
	//KEGS_GAME_LOADING_NOT kPacketBR;
	//kPacketBR.m_iUnitUID = FIRST_SENDER_UID;
	//kPacketBR.m_iLoadingProgress = kPacket_.m_iLoadingProgress;
	//BroadCast( ERM_GAME_LOADING_NOT, kPacketBR );


    if ( kPacket_.m_iLoadingProgress >= 100 )
    {
#ifdef  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
        KEGS_GAME_LOADING_NOT kPacketBR;
        kPacketBR.m_iUnitUID = FIRST_SENDER_UID;
        kPacketBR.m_iLoadingProgress = 100;
        BroadCast( ERM_GAME_LOADING_NOT, kPacketBR );
#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
        if ( KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID ) )
        {
            spRoomUser->ZombieAlert_Set( false );
            spRoomUser->ZombieAlert_Refresh();
        }
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

    }



}

IMPL_ON_FUNC( ERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_ROOM_ACK );
}

_IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_BATTLE_FIELD_REQ, KERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_BATTLE_FIELD_ACK );
}

_IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_DUNGEON_GAME_START_REQ, KERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_BATTLE_FIELD_FOR_DUNGEON_GAME_START_ACK );
}

_IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_PVP_GAME_START_REQ, KERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_BATTLE_FIELD_FOR_PVP_GAME_START_ACK );
}

_IMPL_ON_FUNC( ERM_NPC_UNIT_CREATE_REQ, KEGS_NPC_UNIT_CREATE_REQ )
{
	VERIFY_ROOM_STATE_AND_WARN( ( 1, KRoomFSM::S_PLAY ), (1, KRoomFSM::S_RESULT), ERM_NPC_UNIT_CREATE_ACK, KPacketOK );

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_CREATE_ACK, kPacket );

	KEGS_NPC_UNIT_CREATE_NOT kPacketNot;
	kPacketNot.m_iBattleFieldID = GetBattleFieldID();

	BOOST_TEST_FOREACH( KNPCUnitReq&, kCreateNpcInfo, kPacket_.m_vecNPCUnitReq )
	{
#ifdef NOT_CREATE_NPC_EXCEPT_FOR_SPECIFIC_NPC
		/// ���� ���� �̰ų�
		/// �Ʊ� �� �Ʒ��� ����Ʈ NPC 
		/// �Ʊ� �� PVP_NPC_KEY_CODE �� ������ PC�� �θ� �� ����
		if ( !kCreateNpcInfo.m_bAllyNpc ||
			CXSLUnitManager::CanCreateThisNpc( kCreateNpcInfo.m_NPCID ) ||
			kCreateNpcInfo.m_KeyCode == PVP_NPC_KEY_CODE )
#endif // NOT_CREATE_NPC_EXCEPT_FOR_SPECIFIC_NPC
		{
			KNPCUnitNot kNPCInfo;

			switch( kCreateNpcInfo.m_NPCID )
			{
			case CXSLUnitManager::NUI_EVENT_TEACHER_ECHO:
				{
					kCreateNpcInfo.m_bNoDrop = false; // ED �뷮 ��� 
					kCreateNpcInfo.m_Level = SiCXSLBattleFieldManager()->GetStandardMonsterLevel( GetBattleFieldID() );

				}break;
			default:
				{
					kCreateNpcInfo.m_bNoDrop = true; // Ŭ���̾�Ʈ�� ���� ��û ������ ���ʹ� ���Ȼ� �ݵ�� NoDrop���� ó���Ѵ�!
				}break;	
			}

			kNPCInfo.m_kNPCUnitReq = kCreateNpcInfo;

			// ���� ����!
			LIF( m_kMonsterManager.CreateMonster( kCreateNpcInfo, kNPCInfo.m_kNPCUnitReq.m_UID ) );
			kPacketNot.m_vecNPCUnitAck.push_back( kNPCInfo );

			//{{ ��Ʋ �ʵ忡�� ���� ���� ���� ���� - ��μ� -- 2012�� 11�� 15�� ��ġ ����
#ifdef SERV_BATTLE_FIELD_NPC_CREATE_ERROR
			// ���� ���� ��� �� ����
			// ������ ������ �Ǹ� ��ȯ�� ���͵� ���� �ֱ� ���ؼ�
			m_kMonsterManager.SetNpcOwner( kNPCInfo.m_kNPCUnitReq.m_UID, kCreateNpcInfo.m_iAllyUID );
#endif SERV_BATTLE_FIELD_NPC_CREATE_ERROR
			//}
		}
	}

	BroadCast( ERM_NPC_UNIT_CREATE_NOT, kPacketNot, KRoomUserManager::UL_NPC_LOAD_USER );
}

//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
_IMPL_ON_FUNC( ERM_NPC_UNIT_DIE_REQ, KEGS_NPC_UNIT_DIE_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_NPC_UNIT_DIE_ACK, KERM_NPC_UNIT_DIE_ACK );

#ifdef	SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ// ���볯¥: 2013-05-03
	if( HostCheck() == true )
	{
		KRoomUserPtr spUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
		if( spUser == NULL )
		{
			START_LOG( cwarn, L"��Ŷ�� ���� ������ �������� �ʴ´�?" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
			return;
		}

		if( spUser->IsHost() == false )
		{
			START_LOG( cwarn, L"ȣ��Ʈ ������ �ƴѵ��� ERM_NPC_UNIT_DIE_REQ ��Ŷ�� �����Դ�." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;

			//// �߹� �α� ���
			//KDBE_UDP_CHECK_KICK_USER_NOT kPacket;
			//kPacket.m_iRoomUID = GetUID();
			//kPacket.m_iPVPChannelClass	= KPVPChannelInfo::PCC_END;
			//kPacket.m_iMatchType		= SEnum::MT_MAX;
			//kPacket.m_iUnitUID			= FIRST_SENDER_UID;
			//SendToLogDB( DBE_UDP_CHECK_KICK_USER_NOT, kPacket );
			return;
		}
	}
#endif	// SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-13	// �ڼ���
	// ���� �ʵ忡���� ���赵 ��ȭ�� ����.
	if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD ) == true )
	{
		kPacket.m_iOK = BossMonsterDieProcess( FIRST_SENDER_UID, kPacket_, kPacket );
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
	}
	else
	{
		char cMonsterGrade = 0;

		if( m_kMonsterManager.IsEventMonster( kPacket_.m_nDieNPCUID ) == true )
		{
			kPacket.m_iOK = EventMonsterDieProcess( FIRST_SENDER_UID, kPacket_, kPacket, cMonsterGrade );
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
		}
		else if( m_kMonsterManager.IsMiddleBossMonster( kPacket_.m_nDieNPCUID ) == true )
		{
			kPacket.m_iOK = MiddleBossMonsterDieProcess( FIRST_SENDER_UID, kPacket_, kPacket, cMonsterGrade );
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
		}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
		else if( m_kMonsterManager.IsEventBossMonster( kPacket_.m_nDieNPCUID ) == true )
		{
			kPacket.m_iOK = EventBossMonsterDieProcess( FIRST_SENDER_UID, kPacket_, kPacket, cMonsterGrade );
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
		}
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
		else
		{
			kPacket.m_iOK = MonsterDiePrcess( FIRST_SENDER_UID, kPacket_, kPacket, cMonsterGrade );
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
		}

		if( kPacket.m_iOK == NetError::NET_OK )
		{
			//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			// ���� �Ŵ������� ���Ͱ� �׾��ٴ°��� �˸���.
			OnNpcUnitDie( m_spUserManager->GetNumMember(), false, GetDifficultyLevel(), cMonsterGrade, !m_kMonsterManager.IsRemainMiddleBoss(), LAST_SENDER_UID );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
			//}
		}
	}
#else // SERV_BATTLE_FIELD_BOSS
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	bool bIsEventBossMonster = false;
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	bool bIsMiddleBossMonster = false;
#else
	// �Ӽ� �������� üũ
	const bool bIsAttribNpc = m_kMonsterManager.IsAttribNpc( kPacket_.m_nDieNPCUID );
	const int iAttibAcount = m_kMonsterManager.GetAttribNpcAttribCount( kPacket_.m_nDieNPCUID );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

	//{{ 2013. 1. 1	�ڼ���	�ʵ� �̺�Ʈ ����
#ifdef SERV_FIELD_EVENT_MONSTER
	KRoomMonsterManager::NPC_DATA kDieNpcInfo;
	if( m_kMonsterManager.IsEventMonster( kPacket_.m_nDieNPCUID ) == true )
	{
		// ���Ͱ� ���� ����ִ��� üũ!
		if( m_kMonsterManager.IsEventMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_21;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			return;
		}

		// ���Ͱ� ���� ����ִٸ� ���� ���� ��û!
		if( m_kMonsterManager.GetEventNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_20;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			return;
		}

		// ���� ����ִٸ� ���λ���� ���������� ����..
		LIF( m_kMonsterManager.SetEventMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );

		kPacket.m_iOK		= NetError::NET_OK;
		kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );

		//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		// ���� �Ŵ������� ���Ͱ� �׾��ٴ°��� �˸���.
		m_kGameManager.OnNpcUnitDie( m_spUserManager->GetNumMember(), false, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade, !m_kMonsterManager.IsRemainMiddleBoss() );
		BattleFieldNormalDropProcess( kPacket_, kDieNpcInfo, FIRST_SENDER_UID );
		return;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}
	}
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	else if( m_kMonsterManager.IsMiddleBossMonster( kPacket_.m_nDieNPCUID ) == true )
	{
		bIsMiddleBossMonster = true;

		// ���Ͱ� ���� ����ִ��� üũ!
		if( m_kMonsterManager.IsMiddleBossMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_21;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			return;
		}

		// ���Ͱ� ���� ����ִٸ� ���� ���� ��û!
		if( m_kMonsterManager.GetMiddleBossNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_20;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			return;
		}

		// ���� ����ִٸ� ���λ���� ���������� ����..
		LIF( m_kMonsterManager.SetMiddleBossMonsterDie( kPacket_.m_nDieNPCUID ) );

		kPacket.m_iOK		= NetError::NET_OK;
		kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );

		// ���� �Ŵ������� ���Ͱ� �׾��ٴ°��� �˸���.
		m_kGameManager.OnNpcUnitDie( m_spUserManager->GetNumMember(), false, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade, !m_kMonsterManager.IsRemainMiddleBoss() );

        std::vector< KDropItemData > vecDropItem;
        vecDropItem.clear();
		BattleFieldMiddleBossDropProcess( kPacket_, kDieNpcInfo, FIRST_SENDER_UID, vecDropItem );

        KFieldBossGenKillStat kLog;
        //kLog.m_nDieNPCUID = kPacket_.m_nDieNPCUID;
        kLog.m_nNumUser = kPacket_.m_mapDamageByUser.size();
        //kLog.m_nNumDropItem = vecDropItem.size();
        kLog.m_nSpawnID = kDieNpcInfo.m_iBossGroupID;
        kLog.m_nFieldID = GetBattleFieldID();
        SendToLogDB( ELOG_KILL_FIELD_MIDDLE_BOSS_STAT_NOT, kLog );

        START_LOG( cwarn, L"�ʵ� �߰� ���� ų ��� �׽�Ʈ " )
            << BUILD_LOG( kLog.m_nFieldID )
            //<< BUILD_LOG( kLog.m_nDieNPCUID )
            << BUILD_LOG( kLog.m_nNumUser )
            << BUILD_LOG( kLog.m_nSpawnID )
            << END_LOG;

		return;
	}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	else if( m_kMonsterManager.IsEventBossMonster( kPacket_.m_nDieNPCUID ) == true )
	{
		bIsEventBossMonster = true;

		// ���Ͱ� ���� ����ִ��� üũ!
		if( m_kMonsterManager.IsEventBossMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_21;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			return;
		}

		// ���Ͱ� ���� ����ִٸ� ���� ���� ��û!
		if( m_kMonsterManager.GetEventBossNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_20;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			return;
		}

		// ���� ����ִٸ� ���λ���� ���������� ����..
		LIF( m_kMonsterManager.SetEventBossMonsterDie( kPacket_.m_nDieNPCUID ) );

		kPacket.m_iOK		= NetError::NET_OK;
		kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );

		// ���� �Ŵ������� ���Ͱ� �׾��ٴ°��� �˸���.
		m_kGameManager.OnNpcUnitDie( m_spUserManager->GetNumMember(), false, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade, !m_kMonsterManager.IsRemainMiddleBoss() );

		BattleFieldMiddleBossDropProcess( kPacket_, kDieNpcInfo, FIRST_SENDER_UID );
		return;
	}
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
	else
	{
		// ���Ͱ� ���� ����ִ��� üũ!
		if( m_kMonsterManager.IsMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_21;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			return;
		}

		// ���Ͱ� ���� ����ִٸ� ���� ���� ��û!
		if( m_kMonsterManager.GetNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_20;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			return;
		}

		// ���� ����ִٸ� ���λ���� ���������� ����..
		LIF( m_kMonsterManager.SetMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );

		kPacket.m_iOK		= NetError::NET_OK;
		kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );

		//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		// ���� �Ŵ������� ���Ͱ� �׾��ٴ°��� �˸���.
		m_kGameManager.OnNpcUnitDie( m_spUserManager->GetNumMember(), false, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade, !m_kMonsterManager.IsRemainMiddleBoss() );
		BattleFieldNormalDropProcess( kPacket_, kDieNpcInfo, FIRST_SENDER_UID );
		return;
#else
		// ���� �Ŵ������� ���Ͱ� �׾��ٴ°��� �˸���.
		m_kGameManager.OnNpcUnitDie( m_spUserManager->GetNumMember(), false, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}
	}
#else
	// ���Ͱ� ���� ����ִ��� üũ!
	if( m_kMonsterManager.IsMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_21;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
		return;
	}

	// ���Ͱ� ���� ����ִٸ� ���� ���� ��û!
	KRoomMonsterManager::NPC_DATA kDieNpcInfo;
	if( m_kMonsterManager.GetNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_20;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
		return;
	}

	// ���� ����ִٸ� ���λ���� ���������� ����..
	LIF( m_kMonsterManager.SetMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );

	kPacket.m_iOK		= NetError::NET_OK;
	kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );

	// ���� �Ŵ������� ���Ͱ� �׾��ٴ°��� �˸���.
	m_kGameManager.OnNpcUnitDie( m_spUserManager->GetNumMember(), false, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade );
#endif SERV_FIELD_EVENT_MONSTER
	//}}
#endif // SERV_BATTLE_FIELD_BOSS

	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	// �Լ��� ������ �Ʒ� ���� �ؾ���
#else
	//{{ 2009. 5. 11  ������	npc��������
	const bool bDropDieState = IsDropDieState( kPacket_.m_cNpcDieState );
	const bool bAddRoomUserKillCount = ( bDropDieState  &&  kPacket_.m_cNpcDieState != KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_SELF );
	//}}

	const CXSLUnitManager::NPCUnitTemplet* pNPCTemplet = SiCXSLUnitManager()->GetNPCUnitTemplet( (CXSLUnitManager::NPC_UNIT_ID)kDieNpcInfo.m_iNPCID );
	if( pNPCTemplet == NULL )
	{
		START_LOG( cwarn, L"NPC Unit Templet �б� ����.!" )
			<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
			<< END_LOG;
		return;
	}

	// ���� Ÿ�� ��� ���
	const float fMonsterTypeFactor = static_cast<float>( CXSLUnitManager::GetBattleFieldMonsterTypeFactor( iAttibAcount, kDieNpcInfo.m_cMonsterGrade ) );

	// �ʵ� ��� ���
	//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
	const float fFieldEDFactor = SiCXSLBattleFieldManager()->GetBattleFieldEDFactor() * fMonsterTypeFactor;
	const float fFieldEXPFactor = SiCXSLBattleFieldManager()->GetBattleFieldEXPFactor() * fMonsterTypeFactor;
#else
	const float fFieldFactor = SiCXSLBattleFieldManager()->GetBattleFieldFactor() * fMonsterTypeFactor;
#endif SERV_FIELD_ED_EXP_FACTOR
	//}

	//////////////////////////////////////////////////////////////////////////
	// uiAttUnit���� -1�̸� ������ ������ ���� �ʰ� npc�� �ڻ��Ѱ���. [��:��������]

	// �÷��̾ ���͸� ���ΰŶ�� �ش� �÷��̾��� ų���ͼ� üũ
	if( pNPCTemplet->m_bIsNPC  &&  bAddRoomUserKillCount == true  &&  kPacket_.m_uiAttUnit != -1 )
	{
		if( m_spUserManager->AddKillNPC( kPacket_.m_uiAttUnit ) == false )
		{
			if( m_kMonsterManager.IsExistMonster( static_cast<int>(kPacket_.m_uiAttUnit) ) == true )
			{
				START_LOG( clog, L"������ �׿���!!" )
					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
					<< BUILD_LOG( kPacket_.m_uiAttUnit );

				// ���Ͱ� ������ �ɷ� �״°���� : �������� �״�� ���ó����.
			}
		}
	}	

	//////////////////////////////////////////////////////////////////////////
	// 1. ���� ������ ���� ��� �������� �����Ǵ� ���� ���
	if( kDieNpcInfo.m_bNoDrop == false  &&  bDropDieState == true )
	{
		//////////////////////////////////////////////////////////////////////////
		// �� �������� �ٸ� ������ ��Ŷ�� ������ �ϹǷ� �̷��� ó���Ѵ�.
		KERM_NPC_UNIT_DIE_NOT kNpcDietNot;
		kNpcDietNot.m_iDungeonID		 = m_iDungeonID;
		kNpcDietNot.m_cDifficulty		 = m_cDifficultyLevel;
		kNpcDietNot.m_nDieNPCUID		 = kPacket_.m_nDieNPCUID;
		kNpcDietNot.m_iNPCID			 = kDieNpcInfo.m_iNPCID;
		kNpcDietNot.m_cAttUnitType		 = kPacket_.m_cAttUnitType;
		kNpcDietNot.m_uiAttUnit			 = kPacket_.m_uiAttUnit;
		kNpcDietNot.m_bNoDrop			 = kDieNpcInfo.m_bNoDrop;
		kNpcDietNot.m_cDungeonMode		 = m_cDungeonMode;
		kNpcDietNot.m_sSubStageExpNpcNum = m_kMonsterManager.GetAtStartedMonsterCount();
		kNpcDietNot.m_cUserCount		 = 0;
		kNpcDietNot.m_bDecreaseEndurance = false;
		kNpcDietNot.m_bQuestComplete	 = false;			// ��Ƽ�� �����Ͽ� �� �������� npc hp �� 50% �̻��̸� ����Ʈ �Ϸ� ����

		// ���� ������ ���� �� ��� �Ǿ�� �� ����
		// 1. �⿩�� (X�� ǥ��)
		std::map< UidType, float >	mapUserContribution;
		int iNpcMaxHP = kPacket_.m_iNpcMaxHP;
		std::map< UidType, float >::iterator mitDamageByUser = kPacket_.m_mapDamageByUser.begin();
		for( ; mitDamageByUser != kPacket_.m_mapDamageByUser.end() ; ++mitDamageByUser )
		{
			std::vector<UidType> vecPartyUserList;
			float fContribution = m_spUserManager->GetPartyTotalDamage( mitDamageByUser->first, kPacket_.m_mapDamageByUser ,vecPartyUserList ) / (static_cast<float>(iNpcMaxHP));

			if( fContribution > 1.5f )
				fContribution = 1.5f;

			// �ַ� �÷��̸� ��Ƽ�� ������ ����� ���� ���� �ִ�. �׷��� �ѹ� ����� �Ѵ�.
			mapUserContribution.insert( std::make_pair( mitDamageByUser->first, fContribution ) );

			// ��Ƽ ���� ������ �⿩���� �����Ѵ�.
			BOOST_TEST_FOREACH( UidType, iPartyUnitUID, vecPartyUserList )
			{
				mapUserContribution.insert( std::make_pair( iPartyUnitUID, fContribution ) );
			}
		}

		// 2. ������ ��� ���� ���
		KDropTable::KBattleFieldNpcDropInfo kDropInfo_Init;
		if( SiKDropTable()->GetBattleFieldNpcDropInfo( GetBattleFieldID(), kDieNpcInfo.m_iNPCID, kDropInfo_Init ) == false )
		{
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// ��Ƽ�� �����Ͽ� �� �������� npc hp �� 50% �̻��̸� ����Ʈ �Ϸ� ����
			kNpcDietNot.m_EXPList.clear();

			START_LOG( clog, L"��Ʋ �ʵ� npc drop info ������ ����" )
				<< BUILD_LOG( GetBattleFieldID() )
				<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
				<< END_LOG;

			BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
			return;
		}

		// 3. �⿩���� �ִ� �������� EXP, ED, DropItem ���
		std::map< UidType, KRoomUserPtr > mapRoomUserList;
		if( m_spUserManager->GetRoomUserList( mapRoomUserList ) == false )
		{
			//////////////////////////////////////////////////////////////////////////
			// ���� ���� ���� �ϱ� ������ ������ �ʱ�ȭ�� �ʿ��ϴ�.
			//����� �ο����� �˷��� ������ ���� ����..
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// ��Ƽ�� �����Ͽ� �� �������� npc hp �� 50% �̻��̸� ����Ʈ �Ϸ� ����
			kNpcDietNot.m_EXPList.clear();
			//////////////////////////////////////////////////////////////////////////

			START_LOG( cerr, L"��������  ����Ʈ�� ã�� ���� ����." )
				<< END_LOG;

			//////////////////////////////////////////////////////////////////////////
			// �ʵ� �������� �ش� npc�� �����Ϳ� ���� ������ ��ε� ĳ���� �Ѵ�! ( �Ϻ����� ���, ����ġ ���� )
			BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
			return;
		}

		std::map< UidType, KRoomUserPtr >::iterator mitUserList = mapRoomUserList.begin();
		for(  ; mitUserList != mapRoomUserList.end() ; ++mitUserList )
		{
			KDropTable::KBattleFieldNpcDropInfo kDropInfo;
			kDropInfo = kDropInfo_Init;

			if( mitUserList->second == NULL )
				continue;

			//////////////////////////////////////////////////////////////////////////
			// ���� ���� ���� �ϱ� ������ ������ �ʱ�ȭ�� �ʿ��ϴ�.
			//����� �ο����� �˷��� ������ ���� ����..
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// ��Ƽ�� �����Ͽ� �� �������� npc hp �� 50% �̻��̸� ����Ʈ �Ϸ� ����
			kNpcDietNot.m_EXPList.clear();
			//////////////////////////////////////////////////////////////////////////

			if( mitUserList->second->IsBattleFieldNpcLoad() == false )
				continue;

			// �ش� ������ ���͸� ���϶� ��ŭ ������ �ߴ��� �˻�����!
			std::map< UidType, float >::iterator mitUserContribution = mapUserContribution.find( mitUserList->first );
			if( mitUserContribution == mapUserContribution.end() )
			{
				// ���嵵�� ������ ���� ���� �ڰ��� ����!
				SendToGSCharacter( mitUserList->second->GetGSUID(), mitUserList->second->GetCID(), ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot );
				continue;
			}

			KDropTable::DROP_DATA sDropDataNpc;

			// ���� ���� �ʵ忡 �ִ� ��Ƽ�� ��
			kNpcDietNot.m_cUserCount	= static_cast<char>( m_spUserManager->GetSamePartyMember( mitUserList->first ) );

			// ���� ����� ���
			const float fLevelFactor = CXSLDungeon::GetMonsterLevelBalanceRate( m_spUserManager->GetUserLevel( mitUserList->first ), kDieNpcInfo.m_cLevel );

			// ��Ƽ �÷����� ��� ������ Ȯ�� ����
			const float fPartyDropBonus = m_spUserManager->GetBattleFieldPartyBonusRate( mitUserList->first ) + 1.0f;	

			// ��Ƽ�� �⿩���� 50% �̻��ΰ�?
			if( mitUserContribution->second >= 0.5f )
			{
				kNpcDietNot.m_bQuestComplete = true;
			}

			// ���������� ���/���� �Ǿ�� �ϴ� ǰ��
			// 1. ED
			// 2. EXP
			// 3. ������
			if( kDropInfo.m_iExp > 0 )
			{
				kDropInfo.m_iExp = static_cast<int>(SiKDropTable()->GetNpcExp( kDieNpcInfo.m_cLevel ) / kNpcDietNot.m_cUserCount);

				// ����ġ�� �ִ� ���Ͷ�� �������� ���� �ȴ�.
				kNpcDietNot.m_bDecreaseEndurance = true;
			}

			if( kDropInfo.m_iED > 0 )
			{
				kDropInfo.m_iED = (10 + ( kDropInfo.m_iED * kDieNpcInfo.m_cLevel )) / kNpcDietNot.m_cUserCount;
			}

			//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT
			//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
			if( SiKDropTable()->BattleFieldNpcDropItem( kDropInfo, fFieldEDFactor, fFieldEXPFactor, fLevelFactor, mitUserContribution->second, fPartyDropBonus, m_iItemDropEventProbCount, kDieNpcInfo.m_bIsBoss, sDropDataNpc ) == false )
#else
			if( SiKDropTable()->BattleFieldNpcDropItem( kDropInfo, fFieldFactor, fLevelFactor, mitUserContribution->second, fPartyDropBonus, m_iItemDropEventProbCount, kDieNpcInfo.m_bIsBoss, sDropDataNpc ) == false )
#endif SERV_FIELD_ED_EXP_FACTOR
			//}
#else
			if( SiKDropTable()->BattleFieldNpcDropItem( kDropInfo, fFieldFactor, fLevelFactor, mitUserContribution->second, fPartyDropBonus, kDieNpcInfo.m_bIsBoss, sDropDataNpc ) == false )
#endif SERV_ITEM_DROP_EVENT
				//}}
			{
				START_LOG( cerr, L"Drop Item Data ������ ���� ===" )
					<< BUILD_LOG( GetBattleFieldID() )
					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
					<< END_LOG;
			}

			// �������� ���/���� �Ǿ�� �ϴ� ǰ��
			// 1. Ư�� ������ ���(ex. ��������)
			// ����ġ�� �ִ� ��쿣 ������ �Ҹ� ��Ų��.
			SiKDropTable()->NpcSpecialDropItem( kDieNpcInfo.m_iNPCID, kDieNpcInfo.m_cMonsterGrade, kPacket_.m_cNpcDieState, sDropDataNpc.m_vecItemID );

			//////////////////////////////////////////////////////////////////////////		
			// �ʵ��� �������� ����ġ ����(���ʽ� ����)
			m_spUserManager->AddBattleFieldRewardEXP( mitUserList->first, fLevelFactor, sDropDataNpc.m_iExp, kNpcDietNot );

			// �ѹ��� �÷��־�� �Ѵ�.
			if( mitUserList->second->GetCID() == FIRST_SENDER_UID )
			{
				IncreasePartyFever( kPacket_.m_mapDamageByUser ); // �ش� ���͸� ���� �������� ���嵵�� ���� ��Ƽ �ǹ� ����!
			}
			//////////////////////////////////////////////////////////////////////////	
			SendToGSCharacter( mitUserList->second->GetGSUID(), mitUserList->second->GetCID(), ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot );

			// ����� ���� �ʴ� npc��� ����� ���� �ʴ´�.
			if( kDieNpcInfo.m_bNoDrop == true )
				continue;

			// ��ӻ��°� �ƴ�ä�� ����npc��� ������ ����� ���� �ʴ´�.
			if( bDropDieState == false )
				continue;

			// ���� �⿩�� * ��Ƽ ���ʽ�
			//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
			float fTotalDropRate = mitUserContribution->second * fFieldEXPFactor;// * fPartyDropBonus ;
#else
			float fTotalDropRate = mitUserContribution->second * fFieldFactor;// * fPartyDropBonus ;
#endif SERV_FIELD_ED_EXP_FACTOR
			//}

			//////////////////////////////////////////////////////////////////////////
			// 2. ������ �������� ��� �������� �����Ǵ� ���� ���
			KDropTable::DROP_DATA sDropDataStatic;	
			{
				if( SiKDropTable()->BattleFieldStaticDropItem( GetBattleFieldID(), sDropDataStatic, fTotalDropRate ) == false )
				{
					START_LOG( cerr, L"Drop Item Data ������ ���� ===" )
						<< BUILD_LOG( GetBattleFieldID() )
						<< END_LOG;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			// 4. �Ӽ� ���� ��� ������
			KDropTable::DROP_DATA sDropDataAttribNpc;

			if( bIsAttribNpc == true )
			{
				//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
				if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(CXSLDungeon::DL_EXPERT), static_cast<int>(kDieNpcInfo.m_cLevel), fTotalDropRate, m_iItemDropEventProbCount, sDropDataAttribNpc.m_vecItemID ) == false )
				{
					START_LOG( cerr, L"�Ӽ� ���� ������ ��� ����!" )
						<< BUILD_LOG( CXSLDungeon::DL_EXPERT )
						<< END_LOG;
				}

				sDropDataNpc.m_nGPNum = 3 * sDropDataNpc.m_nGPNum;


				if( SiKDropTable()->AttribNpcSpecialDrop( CXSLDungeon::DL_EXPERT, fTotalDropRate, sDropDataAttribNpc.m_vecItemID ) == false )
				{
					START_LOG( cerr, L"�Ӽ� ���� ����� ������ ��� ����!" )
						<< BUILD_LOG( CXSLDungeon::DL_EXPERT )
						<< END_LOG;
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// 5. ����Ʈ ��� ������ ó��
			{
				std::vector< KRoomUser::QUEST_ITEM_DROP_INFO > vecDropInfo;

				//���ֺ� ����Ʈ ��� �������� �ִ��� �˻�..
				//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
				if( m_spUserManager->GetQuestDropItem( mitUserList->second->GetCID(), GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo, m_fQuestItemDropEventProbRate ) == true )
#else
				if( m_spUserManager->GetQuestDropItem( mitUserList->second->GetCID(), GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo ) == true )
#endif SERV_QUEST_ITEM_DROP_EVENT
				//}
				{
					//�ִٸ� ���ֺ��� ��Ǫ�� ���鼭..
					BOOST_TEST_FOREACH( const KRoomUser::QUEST_ITEM_DROP_INFO&, kDropInfo, vecDropInfo )
					{
						//��ӵ� ������ ����ŭ ó���� ����..
						BOOST_TEST_FOREACH( const int, iDropQuestItemID, kDropInfo.m_vecDropQuestItem )
						{
							KEGS_GET_ITEM_NOT kDropNot;
							kDropNot.m_cGetItemType = KEGS_GET_ITEM_NOT::GIT_QUEST_ITEM;
							kDropNot.m_GetUnitUID	= kDropInfo.m_iUnitUID;
							kDropNot.m_iItemID		= iDropQuestItemID;
							//{{ 2009. 7. 9  ������		�ӽ� �κ��丮
							kDropNot.m_iDropItemUID = m_nDropItemUID++;
							//}}
							SendToGSCharacter( kDropInfo.m_iGSUID, kDropInfo.m_iUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kDropNot );
						}
					}
				}
				else
				{
					if( NetError::GetLastError() != NetError::NET_OK )
					{
						START_LOG( cerr, L"����Ʈ ��� ������ �޾ƿ��� ����.!" )
							<< BUILD_LOG( kPacket_.m_uiAttUnit )
							<< BUILD_LOG( m_iDungeonID )
							<< BUILD_LOG( NetError::GetLastErrMsg() )
							<< END_LOG;
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// ������ ������ ��� ������ ����Ʈ�� ���� �������� ������ ���� ó��
			KEGS_DROP_ITEM_NOT kDropItemNot;
			kDropItemNot.m_CreatePos = kPacket_.m_DiePos;

			if( mitUserList->second->ExistActiveBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO ) == true )
			{
				//{{ 2012. 11. 20	������		�޸� ���� ���� ���� ���� ( ��Ʋ�ʵ� ) 
#ifdef SERV_COME_BACK_USER_REWARD_IN_BATTLE_FIELD_BUG_FIX
				if( SiKDropTable()->GetComeBackBuffDropCount( m_spUserManager->GetSamePartyMember( mitUserList->second->GetCID() ) ) == true )
#else
				if( SiKDropTable()->GetComeBackBuffDropCount( m_spUserManager->GetNumMember() ) == true )
#endif SERV_COME_BACK_USER_REWARD_IN_BATTLE_FIELD_BUG_FIX
					//}}
				{
					// ��� ���̺��� Ȯ���ؼ� 'Special Drop'������� ��ü�ҿ���
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( 127260, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );		// ���ο� ������ ¡ǥ
				}	
			}

			//1.ED ���ó��
			if( sDropDataNpc.m_iED != -1 )
			{
				for( int i = 0; i < sDropDataNpc.m_nGPNum; ++i )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( sDropDataNpc.m_iGPItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution, true, sDropDataNpc.m_iED ) );
				}
			}

			//2.NPC ITEM ���ó��	
			if( sDropDataNpc.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataNpc.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}

			//{{ 2009. 4. 22  ������	��ü���
			if( sDropDataStatic.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataStatic.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}
			//}}

			//4.ATTRIB NPC ITEM ���ó��
			if( kDieNpcInfo.m_bActive == true && sDropDataAttribNpc.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataAttribNpc.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}

			//���ó���� �������� ������ ��Ŷ�� ������.
			if( kDropItemNot.m_DropItemDataList.empty() == false )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mitUserContribution->first );
				if( spRoomUser == NULL )
					continue;

				if( spRoomUser->IsBattleFieldNpcLoad() == false )
					continue;

				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_DROP_ITEM_NOT, kDropItemNot );
			}
		}

		//{{ 2012. 12. 26  Ư�� �ð� ���� ��� �� ������ ��� �̺�Ʈ(�ʵ�����) - ��μ�
#ifdef SERV_TIME_MONSTER_DROP_EVENT_IN_FIELD
		//////////////////////////////////////////////////////////////////////////
		// Ư�� �ð� ������ ��� �̺�Ʈ
		if( kDieNpcInfo.m_bActive == true )
		{
			//////////////////////////////////////////////////////////////////////////
			// Ư�� �ð��� NPC���������� �����ִ� ���
			if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_BY_USER )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( kPacket_.m_uiAttUnit );
				if( spRoomUser != NULL )
				{
					int iTimeDropItemID = 0;
					int iTimeDropID = 0;
					if( SiKDropTable()->CheckTimeDrop( iTimeDropID, iTimeDropItemID ) )
					{
						const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iTimeDropItemID );
						if( pItemTemplet != NULL )
						{
							// �ϴ� DB�� ���� �� ������ ���� �޾Ƶ� �Ǵ� �������� Ȯ������!
							KDBE_CHECK_TIME_DROP_RESULT_REQ kGetTimeDrop;
							kGetTimeDrop.m_iGSUID		 = spRoomUser->GetGSUID();
							kGetTimeDrop.m_iUserUID		 = spRoomUser->GetUserUID();
							kGetTimeDrop.m_iUnitUID		 = spRoomUser->GetCID();
							kGetTimeDrop.m_wstrNickName	 = spRoomUser->GetNickName();
							kGetTimeDrop.m_iTimeDropID	 = iTimeDropID;
							kGetTimeDrop.m_iRewardItemID = iTimeDropItemID;
							kGetTimeDrop.m_wstrRewardItemName = pItemTemplet->m_Name;
							kGetTimeDrop.m_iDropItemUID	 = m_nDropItemUID++;
							SendToLogDB( DBE_CHECK_TIME_DROP_RESULT_REQ, kGetTimeDrop );
						}
						else
						{
							START_LOG( cerr, L"�������� �ʴ� itemid�̴�. ���ÿ����ΰ�?" )
								<< BUILD_LOG( iTimeDropItemID )
								<< END_LOG;
						}
					}
				}
				else
				{
					START_LOG( cwarn, L"�̺�Ʈ ������ ȹ�� ó�� �Ϸ��µ� ���������� ����!" )
						<< BUILD_LOG( kPacket_.m_uiAttUnit )
						<< END_LOG;
				}
			}
			//////////////////////////////////////////////////////////////////////////
		}
#endif SERV_TIME_MONSTER_DROP_EVENT_IN_FIELD
		//}}
	}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
	
}

#else		// SERV_REFORM_ITEM_DROP

//_IMPL_ON_FUNC( ERM_NPC_UNIT_DIE_REQ, KEGS_NPC_UNIT_DIE_REQ )
//{
//	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_NPC_UNIT_DIE_ACK, KERM_NPC_UNIT_DIE_ACK );
//
//	// ���Ͱ� ���� ����ִ��� üũ!
//	if( m_kMonsterManager.IsMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
//	{
//		kPacket.m_iOK = NetError::ERR_ROOM_21;
//		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
//		return;
//	}
//
//	// ���Ͱ� ���� ����ִٸ� ���� ���� ��û!
//	KRoomMonsterManager::NPC_DATA kDieNpcInfo;
//	if( m_kMonsterManager.GetNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
//	{
//		kPacket.m_iOK = NetError::ERR_ROOM_20;
//		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
//		return;
//	}
//
//	// ���� ����ִٸ� ���λ���� ���������� ����..
//	LIF( m_kMonsterManager.SetMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );
//
//	kPacket.m_iOK		= NetError::NET_OK;
//	kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
//	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
//
//	// ���� �Ŵ������� ���Ͱ� �׾��ٴ°��� �˸���.
//	m_kGameManager.OnNpcUnitDie( m_spUserManager->GetNumMember(), false, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade );
//
//	//����� �ο����� �˷��� ������ ���� ����..
//	KERM_NPC_UNIT_DIE_NOT kNpcDietNot;
//	kNpcDietNot.m_iDungeonID	= m_iDungeonID;
//	kNpcDietNot.m_cDifficulty	= m_cDifficultyLevel;
//
//	kNpcDietNot.m_nDieNPCUID	= kPacket_.m_nDieNPCUID;
//	kNpcDietNot.m_iNPCID		= kDieNpcInfo.m_iNPCID;
//	kNpcDietNot.m_cAttUnitType	= kPacket_.m_cAttUnitType;
//	kNpcDietNot.m_uiAttUnit		= kPacket_.m_uiAttUnit;
//
//	kNpcDietNot.m_cUserCount	= static_cast<char>( m_spUserManager->GetSamePartyMember( kPacket_.m_uiAttUnit ) );
//	kNpcDietNot.m_bNoDrop		= kDieNpcInfo.m_bNoDrop;
//
//	//{{ 2009. 12. 31  ������	�������Ҹ���	
//	kNpcDietNot.m_bDecreaseEndurance = false;
//	//}}
//
//	//{{ 2011. 08. 31  ��μ�	��д��� ����
//#ifdef SERV_RANDOM_DAY_QUEST
//	kNpcDietNot.m_cDungeonMode	= m_cDungeonMode;
//#endif SERV_RANDOM_DAY_QUEST
//	//}}
//
//	// ���� ��Ʋ�ʵ��� �ʱ� ���� ���� ���ɴϴ�.
//	kNpcDietNot.m_sSubStageExpNpcNum = m_kMonsterManager.GetAtStartedMonsterCount();
//
//	//{{ 2009. 5. 11  ������	npc��������
//	const bool bDropDieState = IsDropDieState( kPacket_.m_cNpcDieState );
//	const bool bAddRoomUserKillCount = ( bDropDieState  &&  kPacket_.m_cNpcDieState != KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_SELF );
//	//}}
//
//	const CXSLUnitManager::NPCUnitTemplet* pNPCTemplet = SiCXSLUnitManager()->GetNPCUnitTemplet( (CXSLUnitManager::NPC_UNIT_ID)kDieNpcInfo.m_iNPCID );
//	if( pNPCTemplet == NULL )
//	{
//		START_LOG( cwarn, L"NPC Unit Templet �б� ����.!" )
//			<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
//			<< END_LOG;
//		return;
//	}
//
//	// ��Ƽ �÷����� ��� ������ ��� ī��Ʈ ����
//	const int iDropItemNum = 1; // ��Ʋ�ʵ�� ��� ī��Ʈ ������ 1��?
//
//	// ���� Ÿ�� ��� ���
//	const char cMonsterTypeFactor = static_cast<char>( CXSLUnitManager::GetMonsterTypeFactor( false, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade ) );	
//
//	// �Ӽ� �������� üũ
//	const bool bIsAttribNpc = false;
//
//	//////////////////////////////////////////////////////////////////////////
//	// uiAttUnit���� -1�̸� ������ ������ ���� �ʰ� npc�� �ڻ��Ѱ���. [��:��������]
//
//	// �÷��̾ ���͸� ���ΰŶ�� �ش� �÷��̾��� ų���ͼ� üũ
//	if( pNPCTemplet->m_bIsNPC  &&  bAddRoomUserKillCount == true  &&  kPacket_.m_uiAttUnit != -1 )
//	{
//		if( m_spUserManager->AddKillNPC( kPacket_.m_uiAttUnit ) == false )
//		{
//			if( m_kMonsterManager.IsExistMonster( static_cast<int>(kPacket_.m_uiAttUnit) ) == true )
//			{
//				START_LOG( clog, L"������ �׿���!!" )
//					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
//					<< BUILD_LOG( kPacket_.m_uiAttUnit );
//
//				// ���Ͱ� ������ �ɷ� �״°���� : �������� �״�� ���ó����.
//			}
//		}
//	}	
//
//	//////////////////////////////////////////////////////////////////////////
//	// 1. ���� ������ ���� ��� �������� �����Ǵ� ���� ���
//	KDropTable::DROP_DATA sDropDataNpc;
//
//	if( kDieNpcInfo.m_bNoDrop == false  &&  bDropDieState == true )
//	{
//		if( SiKDropTable()->BattleFieldNpcDropItem( GetBattleFieldID(), kDieNpcInfo.m_iNPCID, kDieNpcInfo.m_cLevel, cMonsterTypeFactor, sDropDataNpc, iDropItemNum, kDieNpcInfo.m_bIsBoss ) == false )
//		{
//			START_LOG( cerr, L"Drop Item Data ������ ���� ===" )
//				<< BUILD_LOG( GetBattleFieldID() )
//				<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
//				<< END_LOG;
//		}
//
//		// ����ġ�� �ִ� ��쿣 ������ �Ҹ� ��Ų��.
//		if( sDropDataNpc.m_iExp > 0 )
//		{
//			kNpcDietNot.m_bDecreaseEndurance = true;
//		}			
//
//		//{{ 2009. 5. 18  ������	��������
//		switch( kDieNpcInfo.m_iNPCID )
//		{
//		case CXSLUnitManager::NUI_MANA_EATER:
//		case CXSLUnitManager::NUI_HALLOWEEN_MANA_EATER:
//			//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
//#ifdef SERV_INSERT_GLOBAL_SERVER
//		case CXSLUnitManager::NUI_DEFENCE_MANA_EATER:
//#endif SERV_INSERT_GLOBAL_SERVER
//			//}}
//			//{{ 2012. 06. 25	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//		case CXSLUnitManager::NUI_MANA_EATER_FD:
//#endif SERV_BATTLE_FIELD_SYSTEM
//			//}}
//			{
//				if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_BONUS_DROP )
//				{
//					// ��� ���̺��� Ȯ���ؼ� 'Special Drop'������� ��ü�ҿ���
//					sDropDataNpc.m_vecItemID.push_back( 70055 ); // 70055
//				}
//			}
//			break;
//
//		case CXSLUnitManager::NUI_BLOOD_EATER:
//			//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
//#ifdef SERV_INSERT_GLOBAL_SERVER
//		case CXSLUnitManager::NUI_DEFENCE_BLOOD_EATER:
//#endif SERV_INSERT_GLOBAL_SERVER
//			//}}
//			//{{ 2012. 06. 25	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//		case CXSLUnitManager::NUI_BLOOD_EATER_FD:
//#endif SERV_BATTLE_FIELD_SYSTEM
//			//}}
//			{
//				if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_BONUS_DROP )
//				{
//					// ��� ���̺��� Ȯ���ؼ� 'Special Drop'������� ��ü�ҿ���
//					sDropDataNpc.m_vecItemID.push_back( 70056 ); // 70055
//				}
//			}
//			break;
//		}
//		//}}
//
//		//////////////////////////////////////////////////////////////////////////		
//		//{{ 2009. 12. 18  ������	��������ġ����
//#ifdef DUNGEON_EXP_NEW
//		if( sDropDataNpc.m_iExp > 0 )
//		{
//			// �ʵ��� �� �����鿡�� ����ġ ���
//			m_spUserManager->AddBattleFieldRewardEXP( kDieNpcInfo.m_cLevel, 
//				cMonsterTypeFactor,
//				kPacket_.m_iNpcMaxHP, 
//				kPacket_.m_mapDamageByUser, 
//				GetDungeonIDAndDif(), 
//				GetDifficultyLevel(), 
//				kNpcDietNot );
//
//			//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//			// �ش� ���͸� ���� �������� ���嵵�� ���� ��Ƽ �ǹ� ����!
//			IncreasePartyFever( kPacket_.m_mapDamageByUser );
//#endif SERV_BATTLE_FIELD_SYSTEM
//			//}}
//		}
//#endif DUNGEON_EXP_NEW
//		//}}
//		//////////////////////////////////////////////////////////////////////////		
//	}
//
//
//	//////////////////////////////////////////////////////////////////////////
//	// ���� ��Ƽ���鿡�� �ش� npc�� �����Ϳ� ���� ������ ��ε� ĳ���� �Ѵ�! ( ����ġ ���� ���� )
//	BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
//
//
//	// ����� ���� �ʴ� npc��� ����� ���� �ʴ´�.
//	if( kDieNpcInfo.m_bNoDrop == true )
//		return;
//
//	// ��ӻ��°� �ƴ�ä�� ����npc��� ������ ����� ���� �ʴ´�.
//	if( bDropDieState == false )
//		return;
//
//
//	//////////////////////////////////////////////////////////////////////////
//	// 2. ������ �������� ��� �������� �����Ǵ� ���� ���
//	KDropTable::DROP_DATA sDropDataStatic;	
//	{
//		if( SiKDropTable()->BattleFieldStaticDropItem( GetBattleFieldID(), sDropDataStatic, iDropItemNum ) == false )
//		{
//			START_LOG( cerr, L"Drop Item Data ������ ���� ===" )
//				<< BUILD_LOG( GetBattleFieldID() )
//				<< END_LOG;
//		}
//	}
//
//
//	//////////////////////////////////////////////////////////////////////////	
//	// 3. �̺�Ʈ ������ ��� üũ
//	//KDropTable::DROP_DATA sDropDataEvent;
//
//	//if( kDieNpcInfo.m_bActive == true )
//	//{
//	//	if( SiKDropTable()->EventDropItem( iDropItemNum, sDropDataEvent ) == false )
//	//	{
//	//		START_LOG( cerr, L"Event Drop Item Data ������ ���� ===" )
//	//			<< BUILD_LOG( kPacket_.m_nDieNPCUID )
//	//			<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
//	//			<< BUILD_LOGc( m_cDifficultyLevel )
//	//			<< END_LOG;
//	//	}
//
//	//	//////////////////////////////////////////////////////////////////////////
//	//	// Ư�� �ð��� NPC���������� �����ִ� ���
//	//	if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_BY_USER )
//	//	{
//	//		KRoomUserPtr spRoomUser = m_spUserManager->GetUser( kPacket_.m_uiAttUnit );
//	//		if( spRoomUser != NULL )
//	//		{
//	//			// ü��ID�� �ƴ� ������ ����ó��
//	//			if( spRoomUser->IsGuestUser() == false )
//	//			{
//	//				int iTimeDropItemID = 0;
//	//				int iTimeDropID = 0;
//	//				if( SiKDropTable()->CheckTimeDrop( iTimeDropID, iTimeDropItemID ) )
//	//				{
//	//					const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iTimeDropItemID );
//	//					if( pItemTemplet != NULL )
//	//					{
//	//						// �ϴ� DB�� ���� �� ������ ���� �޾Ƶ� �Ǵ� �������� Ȯ������!
//	//						KDBE_CHECK_TIME_DROP_RESULT_REQ kGetTimeDrop;
//	//						kGetTimeDrop.m_iGSUID		 = spRoomUser->GetGSUID();
//	//						kGetTimeDrop.m_iUserUID		 = spRoomUser->GetUserUID();
//	//						kGetTimeDrop.m_iUnitUID		 = spRoomUser->GetCID();
//	//						kGetTimeDrop.m_wstrNickName	 = spRoomUser->GetNickName();
//	//						kGetTimeDrop.m_iTimeDropID	 = iTimeDropID;
//	//						kGetTimeDrop.m_iRewardItemID = iTimeDropItemID;
//	//						kGetTimeDrop.m_wstrRewardItemName = pItemTemplet->m_Name;
//	//						kGetTimeDrop.m_iDropItemUID	 = m_nDropItemUID++;
//	//						SendToLogDB( DBE_CHECK_TIME_DROP_RESULT_REQ, kGetTimeDrop );
//	//					}
//	//					else
//	//					{
//	//						START_LOG( cerr, L"�������� �ʴ� itemid�̴�. ���ÿ����ΰ�?" )
//	//							<< BUILD_LOG( iTimeDropItemID )
//	//							<< END_LOG;
//	//					}
//	//				}
//	//			}
//	//		}
//	//		else
//	//		{
//	//			START_LOG( cwarn, L"�̺�Ʈ ������ ȹ�� ó�� �Ϸ��µ� ���������� ����!" )
//	//				<< BUILD_LOG( kPacket_.m_uiAttUnit )
//	//				<< END_LOG;
//	//		}
//	//	}
//	//	//////////////////////////////////////////////////////////////////////////		
//	//}
//
//	//////////////////////////////////////////////////////////////////////////
//	// 4. �Ӽ� ���� ��� ������
//	KDropTable::DROP_DATA sDropDataAttribNpc;
//
//	if( bIsAttribNpc == true )
//	{
//		if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(GetDifficultyLevel()), 0, iDropItemNum, sDropDataAttribNpc.m_vecItemID ) == false )
//		{
//			START_LOG( cerr, L"�Ӽ� ���� ������ ��� ����!" )
//				<< BUILD_LOGc( GetDifficultyLevel() )
//				<< END_LOG;
//		}
//
//		switch( GetDifficultyLevel() )
//		{
//		case CXSLDungeon::DL_NORMAL:
//		case CXSLDungeon::DL_HARD:
//			{
//				sDropDataNpc.m_nGPNum = 2 * sDropDataNpc.m_nGPNum;
//			}
//			break;
//
//		case CXSLDungeon::DL_EXPERT:
//			{
//				sDropDataNpc.m_nGPNum = 3 * sDropDataNpc.m_nGPNum;
//			}
//			break;
//
//		default:
//			START_LOG( cerr, L"���� ���̵��� �̻��մϴ�." )
//				<< BUILD_LOGc( GetDifficultyLevel() )
//				<< END_LOG;
//			break;
//		}
//
//		//{{ 2009. 12. 18  ������	������������
//#ifdef DUNGEON_ITEM
//
//		if( SiKDropTable()->AttribNpcSpecialDrop( GetDifficultyLevel(), sDropDataAttribNpc.m_vecItemID ) == false )
//		{
//			START_LOG( cerr, L"�Ӽ� ���� ����� ������ ��� ����!" )
//				<< BUILD_LOGc( GetDifficultyLevel() )
//				<< END_LOG;
//		}
//
//#endif DUNGEON_ITEM
//		//}}
//	}	
//
//
//	//////////////////////////////////////////////////////////////////////////
//	// 5. ����Ʈ ��� ������ ó��
//	{
//		std::vector< KRoomUser::QUEST_ITEM_DROP_INFO > vecDropInfo;
//
//		//���ֺ� ����Ʈ ��� �������� �ִ��� �˻�..
//		if( m_spUserManager->GetQuestDropItem( GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo ) == true )
//		{
//			//�ִٸ� ���ֺ��� ��Ǫ�� ���鼭..
//			BOOST_TEST_FOREACH( const KRoomUser::QUEST_ITEM_DROP_INFO&, kDropInfo, vecDropInfo )
//			{
//				//��ӵ� ������ ����ŭ ó���� ����..
//				BOOST_TEST_FOREACH( const int, iDropQuestItemID, kDropInfo.m_vecDropQuestItem )
//				{
//					KEGS_GET_ITEM_NOT kDropNot;
//					kDropNot.m_cGetItemType = KEGS_GET_ITEM_NOT::GIT_QUEST_ITEM;
//					kDropNot.m_GetUnitUID	= kDropInfo.m_iUnitUID;
//					kDropNot.m_iItemID		= iDropQuestItemID;
//					//{{ 2009. 7. 9  ������		�ӽ� �κ��丮
//					kDropNot.m_iDropItemUID = m_nDropItemUID++;
//					//}}
//					SendToGSCharacter( kDropInfo.m_iGSUID, kDropInfo.m_iUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kDropNot );
//				}
//			}
//		}
//		else
//		{
//			if( NetError::GetLastError() != NetError::NET_OK )
//			{
//				START_LOG( cerr, L"����Ʈ ��� ������ �޾ƿ��� ����.!" )
//					<< BUILD_LOG( kPacket_.m_uiAttUnit )
//					<< BUILD_LOG( m_iDungeonID )
//					<< BUILD_LOG( NetError::GetLastErrMsg() )
//					<< END_LOG;
//			}
//		}
//	}
//
//
//	//////////////////////////////////////////////////////////////////////////
//	// ������ ������ ��� ������ ����Ʈ�� ���� �������� ������ ���� ó��
//	KEGS_DROP_ITEM_NOT kDropItemNot;
//	kDropItemNot.m_CreatePos = kPacket_.m_DiePos;
//
//	//1.ED ���ó��
//	if( sDropDataNpc.m_iED != -1 )
//	{
//		for( int i = 0; i < sDropDataNpc.m_nGPNum; ++i )
//		{
//			kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( sDropDataNpc.m_iGPItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, kPacket_.m_mapDamageByUser, true, sDropDataNpc.m_iED ) );
//		}
//	}
//
//	//2.NPC ITEM ���ó��	
//	if( sDropDataNpc.m_vecItemID.empty() == false )
//	{
//		BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataNpc.m_vecItemID )
//		{
//			kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, kPacket_.m_mapDamageByUser ) );
//		}
//	}
//
//	//{{ 2009. 4. 22  ������	��ü���
//	if( sDropDataStatic.m_vecItemID.empty() == false )
//	{
//		BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataStatic.m_vecItemID )
//		{
//			kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, kPacket_.m_mapDamageByUser ) );
//		}
//	}
//	//}}
//
//	//3.EVENT ITEM ���ó��
//	//if( /*mit->second.m_bNoDrop == false &&*/ kDieNpcInfo.m_bActive == true && sDropDataEvent.m_vecItemID.empty() == false )
//	//{
//	//	BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataEvent.m_vecItemID )
//	//	{
//	//		// Ư�� �������� Ư���������� ��ӵ��� �ʵ��� �ϴ� �ڵ�
//	//		if( !CXSLDungeon::IsItemDropDungeon( iDropItemID, m_iDungeonID ) )
//	//			continue;
//
//	//		kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID ) );
//	//	}
//	//}
//
//	//���ó���� �������� ������ ��Ŷ�� ������.
//	if( kDropItemNot.m_DropItemDataList.empty() == false )
//	{
//		// �ش� ���Ϳ��� ���� ������� �� ĳ���Ϳ��Ը� ������ ��� ������ ������!
//		std::map< UidType, float >::const_iterator mit;
//		for( mit = kPacket_.m_mapDamageByUser.begin(); mit != kPacket_.m_mapDamageByUser.end(); ++mit )
//		{
//			KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mit->first );
//			if( spRoomUser == NULL )
//				continue;
//
//			if( spRoomUser->IsBattleFieldNpcLoad() == false )
//				continue;
//
//			SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_DROP_ITEM_NOT, kDropItemNot );
//		}
//	}
//}

#endif SERV_REFORM_ITEM_DROP
//}}

_IMPL_ON_FUNC( ERM_USER_UNIT_DIE_REQ, KEGS_USER_UNIT_DIE_REQ )
{
	//{{ 2011. 09. 19  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_USER_UNIT_DIE_ACK, KERM_USER_UNIT_DIE_ACK );
#else
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_USER_UNIT_DIE_ACK, KEGS_USER_UNIT_DIE_ACK );
#endif SERV_ADD_TITLE_CONDITION
	//}}

	// Die üũ
	if( !m_spUserManager->SetDie( FIRST_SENDER_UID, true ) )
	{
		kPacket.m_iOK = NetError::ERR_SLOT_07;
		//{{ 2011. 09. 19  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
		KERM_USER_UNIT_DIE_ACK kAck;
		kAck.m_iOK = kPacket.m_iOK;
		kAck.m_ucDieReason = kPacket_.m_ucDieReason;
		kAck.m_fReBirthTime	= kPacket.m_fReBirthTime;
		kAck.m_VP			= kPacket.m_VP;
		kAck.m_EXP			= kPacket.m_EXP;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_USER_UNIT_DIE_ACK, kAck );
#else
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_USER_UNIT_DIE_ACK, kPacket );
#endif SERV_ADD_TITLE_CONDITION
		//}}
		return;
	}

	// �����ϸ� ������Ŷ�� �ش��������� ������
	kPacket.m_iOK = NetError::NET_OK;

	//{{ 2011. 09. 19  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
	KERM_USER_UNIT_DIE_ACK kAck;
	kAck.m_iOK = kPacket.m_iOK;
	kAck.m_ucDieReason = kPacket_.m_ucDieReason;
	kAck.m_fReBirthTime	= kPacket.m_fReBirthTime;
	kAck.m_VP			= kPacket.m_VP;
	kAck.m_EXP			= kPacket.m_EXP;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_USER_UNIT_DIE_ACK, kAck );
#else
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_USER_UNIT_DIE_ACK, kPacket );
#endif SERV_ADD_TITLE_CONDITION
	//}}

	

	// ���� ��� �������� ������ �˸���.
	KERM_USER_UNIT_DIE_NOT kPacketNot;
	kPacketNot.m_iGameType			= m_cGameType;
	kPacketNot.m_KillerNPCUID = kPacket_.m_KillerNPCUID;
	kPacketNot.m_KilledUserUnitUID = FIRST_SENDER_UID;
	BroadCast( ERM_USER_UNIT_DIE_NOT, kPacketNot, KRoomUserManager::UL_NPC_LOAD_USER );

	//{{ 2008. 6. 17  ������  �ǽð� ��Ȱ��
	m_spUserManager->ReserveEndPlay( FIRST_SENDER_UID );
	//}}
}

IMPL_ON_FUNC_NOPARAM( ERM_USER_UNIT_DIE_COMPLETE_REQ )
{
	VERIFY_ROOM_STATE_AND_WARN( ( 1, KRoomFSM::S_PLAY ), (1, KRoomFSM::S_RESULT), ERM_USER_UNIT_DIE_COMPLETE_ACK, KEGS_USER_UNIT_DIE_COMPLETE_ACK );	

	KEGS_USER_UNIT_DIE_COMPLETE_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_USER_UNIT_DIE_COMPLETE_ACK, kAck );

	KEGS_USER_UNIT_DIE_COMPLETE_NOT kNot;
	kNot.m_UnitUID = FIRST_SENDER_UID;

	BroadCast( ERM_USER_UNIT_DIE_COMPLETE_NOT, kNot );
}

//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
_IMPL_ON_FUNC( ERM_GET_ITEM_REQ, KEGS_GET_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_GET_ITEM_ACK, KEGS_GET_ITEM_ACK );

	kPacket.m_iOK = NetError::NET_OK;

	KEGS_GET_ITEM_NOT kNot;

	std::map< int, ITEM_DATA >::iterator mit;

	KRoomUserPtr spGetUser = m_spUserManager->GetUser( FIRST_SENDER_UID );
	if( spGetUser == NULL )
	{
		START_LOG( cerr, L"�����Ϸ��� ������ ����." )
		<< BUILD_LOG( FIRST_SENDER_UID )
		<< BUILD_LOG( kPacket_.m_iDropItemUID )
		<< BUILD_LOG( kPacket_.m_kItemInfo.m_iItemID )
		<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_00;
		goto end_proc;
	}

	UidType iGetUnitPartyUID = spGetUser->GetPartyUID();

	// 1. �����ҷ��� item�� ���°��
	mit = m_mapDropItem.find( kPacket_.m_iDropItemUID );
	if( mit == m_mapDropItem.end() )
	{
		START_LOG( cerr, L"�����ҷ��� �������� ����" )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< BUILD_LOG( kPacket_.m_kItemInfo.m_iItemID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_00;
		goto end_proc;
	}

	KBattleFieldRoom::ITEM_DATA& sItemData = mit->second;

	// 2. ������ ���� �˻�
	if( kPacket_.m_kItemInfo.m_iItemID != sItemData.m_iItemID )
	{
		START_LOG( cerr, L"�����ҷ��� �������� ������ �߸��Ǿ���." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< BUILD_LOG( kPacket_.m_kItemInfo.m_iItemID )
			<< BUILD_LOG( sItemData.m_iItemID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_05;
		goto end_proc;
	}

	// 3. �ٸ� ������ �̹� �����Ѱǰ�
	if( sItemData.m_bIsGet == true )
	{
		kPacket.m_iOK = NetError::ERR_ITEM_01;
		goto end_proc;
	}

	// 4. ȹ�� ������ �����ΰ�?
	if( sItemData.IsGetItemPossibleUnit( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"�������� ȹ�� �� �� ���� ĳ�����ε� ȹ�� ��û�� ���±�.. ��ŷ�ΰ�?" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_00;
		goto end_proc;
	}

	kNot.m_iDropItemUID	= kPacket_.m_iDropItemUID;
	kNot.m_iItemID		= kPacket_.m_kItemInfo.m_iItemID;
	//{{ 2013. 1. 14	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
	kNot.m_cEnchantLevel= sItemData.m_cEnchantLevel;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
	//}}

	// �����Һ� ������
	bool bNotCreateItem = false;
	UidType iGSUID = LAST_SENDER_UID; // ServerUID����

	// 4. ���� �������� ED�ΰ�? Item�ΰ�?

	//////////////////////////////////////////////////////////////////////////	
	// ED������
	if( sItemData.m_bIsGP == true )
	{
		kNot.m_iTotalED = sItemData.m_iED;
		kNot.m_GetUnitUID = FIRST_SENDER_UID; // Ŭ���̾�Ʈ ȭ�鿡 ����� ED�Ծ��� ǥ�����ٶ� ���
		float fPartyBonusRate = m_spUserManager->GetBattleFieldPartyBonusRate( kNot.m_GetUnitUID );
		if( m_spUserManager->AddBattleFieldRewardED( sItemData.m_cNpcLevel, kNot.m_GetUnitUID, fPartyBonusRate, sItemData.m_iED, sItemData.m_mapDamageByUser, kNot ) == false )
		{
			START_LOG( cerr, L"ED����ó���� �̻��մϴ�!" )
				<< BUILD_LOG( kNot.m_GetUnitUID )
				<< BUILD_LOGc( sItemData.m_cNpcLevel )
				<< BUILD_LOG( sItemData.m_iNpcMaxHP )
				<< BUILD_LOG( sItemData.m_mapDamageByUser.size() )
				<< BUILD_LOG( sItemData.m_iED )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ITEM_04;
			goto end_proc;
		}

		bNotCreateItem = true;
	}
	//////////////////////////////////////////////////////////////////////////	
	// ������	�Դ� ������� ������ ����
	else
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( sItemData.m_iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"Item Templet ������ ����. �̻��� ������ �Ծ���." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( sItemData.m_iItemID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ITEM_04;

			sItemData.m_bIsGet = true; // �����ɷ� ó��
			goto end_proc;
		}
		else
		{
			kNot.m_GetUnitUID	= FIRST_SENDER_UID;

			//�����Һ� �������̸� �κ��� ���� �ʱ�����
			if( pItemTemplet->m_ItemType == CXSLItem::IT_OUTLAY )
			{
				bNotCreateItem = true;	
			}
		}
	}

	sItemData.m_bIsGet	= true;	//������ ����� �������� ���������� üũ

	// ack�� ���� ������
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GET_ITEM_ACK, kPacket );

	// ������ ȹ�� ��� ������ - �������� �ƴ� ������
	if( bNotCreateItem == false )
	{
		// �����鿡�� ������ ȹ���� �˸� - �ʵ���� ��Ƽ�� ���� ���
		std::map< UidType, KRoomUserPtr > mapRoomUserList;
		if( m_spUserManager->GetRoomUserList( mapRoomUserList ) == false )
			return;

		std::map< UidType, KRoomUserPtr >::const_iterator mit;
		for( mit = mapRoomUserList.begin(); mit != mapRoomUserList.end(); ++mit )
		{
			KRoomUserPtr spRoomUser = mit->second;
			if( spRoomUser == NULL )
				continue;
	
			if( spRoomUser->IsBattleFieldNpcLoad() == false )
				continue;
			
			if( spRoomUser->IsHost() == true )
			{
				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_GET_ITEM_NOT, kNot );
				continue;		// �Ʒ� ������ Ÿ�� �Ǹ� �ߺ����� not �� ���۵�
			}

			if( spRoomUser->GetPartyUID() != iGetUnitPartyUID )
				continue;

			SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_GET_ITEM_NOT, kNot );
		}

		// ������ ������ ȹ�� ó���� �� ��Ŷ�� ȹ�� �������� ����
		SendToGSCharacter( iGSUID, kNot.m_GetUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kNot );
		//}}

		//{{ 2009. 6. 23  ������	��� ȹ�� ���� ������ - ���������, �ŷ¾�����
		//{{ 2013. 1. 14	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
		if(		( sItemData.m_bNotify == true )
			||	( SiKDropTable()->IsNotifyDropItem( sItemData.m_iItemID ) )
			)
#else
		if( SiKDropTable()->IsNotifyDropItem( sItemData.m_iItemID ) )
#endif SERV_ENCHANT_ITEM_DROP_EVENT
			//}}
		{
			KRoomUserPtr spRoomUser = m_spUserManager->GetUser( kNot.m_GetUnitUID );
			if( spRoomUser != NULL )
			{
				const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( sItemData.m_iItemID );
				if( pItemTemplet != NULL )
				{
					KEGS_NOTIFY_MSG_NOT	kPacket;
					kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_DROP_GET_ITEM; // ������ ȹ�� ����
					kPacket.m_Count = 1;
					kPacket.m_wstrMSG = spRoomUser->GetNickName();
					kPacket.m_wstrMSG += L";";
					//{{ 2012. 1. 17	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
					if( sItemData.m_bNotify == true )
					{
#ifdef SERV_UNITED_SERVER_EU
						kPacket.m_wstrMSG += boost::str( boost::wformat( L"+%d; " ) % static_cast<int>( sItemData.m_cEnchantLevel ) );
#else //SERV_UNITED_SERVER_EU
						kPacket.m_wstrMSG += boost::str( boost::wformat( L"+%d " ) % static_cast<int>( sItemData.m_cEnchantLevel ) );
#endif //SERV_UNITED_SERVER_EU
					}
#endif SERV_ENCHANT_ITEM_DROP_EVENT
					//}}
#ifdef SERV_UNITED_SERVER_EU
					kPacket.m_wstrMSG += pItemTemplet->m_ItemID;
#else //SERV_UNITED_SERVER_EU
					kPacket.m_wstrMSG += pItemTemplet->m_Name;
#endif //SERV_UNITED_SERVER_EU					
					KncSend( PI_CN_ROOM, GetUID(), PI_CN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, ECN_NOTIFY_MSG_NOT, kPacket );
				}
				else
				{
					START_LOG( cerr, L"��� ȹ�� ������ ������ �������µ� �������� �ʴ� �������Դϴ�" )
						<< BUILD_LOG( kNot.m_GetUnitUID )
						<< BUILD_LOG( sItemData.m_iItemID )
						<< END_LOG;
				}
			}
			else
			{
				START_LOG( cerr, L"��� ȹ�� ������ ������ �������µ� �濡 ������ �����ϴ�" )
					<< BUILD_LOG( kNot.m_GetUnitUID )
					<< END_LOG;
			}
		}
	}
	else
	{
		if( true == SiKDropTable()->CheckPVPItem( kPacket_.m_kItemInfo.m_iItemID ) )
		{
			if( static_cast<float>(::rand()%100) > SiKDropTable()->GetPVPDropFailRate( KDropTable::PF_LOSETEAM ) )
				kNot.m_bIsItemSuccess = true;
		}
		// �����Һ� �����۰� ED�������� ��ε�ĳ����

		// �����鿡�� ������ ȹ���� �˸� - �ʵ���� ��Ƽ�� ���� ���
		std::map< UidType, KRoomUserPtr > mapRoomUserList;
		if( m_spUserManager->GetRoomUserList( mapRoomUserList ) == false )
			return;

		std::map< UidType, KRoomUserPtr >::const_iterator mitUser;
		for( mitUser = mapRoomUserList.begin(); mitUser != mapRoomUserList.end(); ++mitUser )
		{
			KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mitUser->first );
			if( spRoomUser == NULL )
				continue;

			if( spRoomUser->IsBattleFieldNpcLoad() == false )
				continue;

			if( spRoomUser->IsHost() == true )
			{
				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_GET_ITEM_NOT, kNot );
				continue;		// �Ʒ� ������ Ÿ�� �Ǹ� �ߺ����� not �� ���۵�
			}

			if( spRoomUser->GetPartyUID() != iGetUnitPartyUID )
				continue;

			SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_GET_ITEM_NOT, kNot );
		}
	}

	return;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GET_ITEM_ACK, kPacket );	
}
#else
_IMPL_ON_FUNC( ERM_GET_ITEM_REQ, KEGS_GET_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_GET_ITEM_ACK, KEGS_GET_ITEM_ACK );

	kPacket.m_iOK = NetError::NET_OK;

	KEGS_GET_ITEM_NOT kNot;

	std::map< int, ITEM_DATA >::iterator mit;
	mit = m_mapDropItem.find( kPacket_.m_iDropItemUID );

	// 1. �����ҷ��� item�� ���°��
	if( mit == m_mapDropItem.end() )
	{
		START_LOG( cerr, L"�����ҷ��� �������� ����" )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< BUILD_LOG( kPacket_.m_kItemInfo.m_iItemID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_00;
		goto end_proc;
	}

	KBattleFieldRoom::ITEM_DATA& sItemData = mit->second;

	// 2. ������ ���� �˻�
	if( kPacket_.m_kItemInfo.m_iItemID != sItemData.m_iItemID )
	{
		START_LOG( cerr, L"�����ҷ��� �������� ������ �߸��Ǿ���." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< BUILD_LOG( kPacket_.m_kItemInfo.m_iItemID )
			<< BUILD_LOG( sItemData.m_iItemID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_05;
		goto end_proc;
	}

	// 3. �ٸ� ������ �̹� �����Ѱǰ�
	if( sItemData.m_bIsGet == true )
	{
		kPacket.m_iOK = NetError::ERR_ITEM_01;
		goto end_proc;
	}

	//{{ 2012. 02. 27	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// 4. ȹ�� ������ �����ΰ�?
	if( sItemData.IsGetItemPossibleUnit( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"�������� ȹ�� �� �� ���� ĳ�����ε� ȹ�� ��û�� ���±�.. ��ŷ�ΰ�?" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_00;
		goto end_proc;
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	kNot.m_iDropItemUID	= kPacket_.m_iDropItemUID;
	kNot.m_iItemID		= kPacket_.m_kItemInfo.m_iItemID;

	// �����Һ� ������
	bool bNotCreateItem = false;
	UidType iGSUID = LAST_SENDER_UID; // ServerUID����

	// 4. ���� �������� ED�ΰ�? Item�ΰ�?

	//////////////////////////////////////////////////////////////////////////	
	// ED������
	if( sItemData.m_bIsGP == true )
	{
		kNot.m_iTotalED = sItemData.m_iED;
		kNot.m_GetUnitUID = FIRST_SENDER_UID; // Ŭ���̾�Ʈ ȭ�鿡 ����� ED�Ծ��� ǥ�����ٶ� ���

		if( m_spUserManager->AddBattleFieldRewardED( sItemData.m_cNpcLevel, sItemData.m_iNpcMaxHP, sItemData.m_mapDamageByUser, sItemData.m_iED, kNot ) == false )
		{
			START_LOG( cerr, L"ED����ó���� �̻��մϴ�!" )
				<< BUILD_LOGc( sItemData.m_cNpcLevel )
				<< BUILD_LOG( sItemData.m_iNpcMaxHP )
				<< BUILD_LOG( sItemData.m_mapDamageByUser.size() )
				<< BUILD_LOG( sItemData.m_iED )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ITEM_04;
			goto end_proc;
		}

		bNotCreateItem = true;
	}
	//////////////////////////////////////////////////////////////////////////	
	// ������
	else
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( sItemData.m_iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"Item Templet ������ ����. �̻��� ������ �Ծ���." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( sItemData.m_iItemID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ITEM_04;

			sItemData.m_bIsGet = true; // �����ɷ� ó��
			goto end_proc;
		}
		else
		{
			kNot.m_GetUnitUID	= FIRST_SENDER_UID;

			//�����Һ� �������̸� �κ��� ���� �ʱ�����
			if( pItemTemplet->m_ItemType != CXSLItem::IT_OUTLAY )
			{
				//������ �����Ŀ� ����..
				if( m_cGetItemType == CXSLRoom::DGIT_RANDOM )
				{
					// Item�� ���� ������ �� �� ���� ������ ������ ���� Unit�� UID�� ��ȯ���ش�.
					if( m_spUserManager->GetItemWithRandom( sItemData.m_mapDamageByUser, FIRST_SENDER_UID, sItemData.m_iItemID, GetDungeonID(), kNot.m_GetUnitUID, iGSUID ) == false )
					{
						kPacket.m_iOK = NetError::ERR_ITEM_03;
						goto end_proc;
					}
				}
			}
			else
			{
				bNotCreateItem = true;
			}
		}
	}

	sItemData.m_bIsGet	= true;	//������ ����� �������� ���������� üũ

	// ack�� ���� ������
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GET_ITEM_ACK, kPacket );

	{
		// ������ ȹ�� ��� ������
		if( bNotCreateItem == false )
		{
			// ������ ȹ���� �������� ����
			//{{ 2010. 03. 03  ������	ä�� ���� ��Ƽ
#ifdef SERV_CHANNEL_PARTY
			//{{ 2010. 03. 18  ������	������ ȹ�� �ڵ� ����

			// �����鿡�� ������ ȹ���� �˸�
			std::map< UidType, float >::const_iterator mit;
			for( mit = sItemData.m_mapDamageByUser.begin(); mit != sItemData.m_mapDamageByUser.end(); ++mit )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mit->first );
				if( spRoomUser == NULL )
					continue;

				if( spRoomUser->IsBattleFieldNpcLoad() == false )
					continue;

				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_GET_ITEM_NOT, kNot );
			}

			// ������ ������ ȹ�� ó���� �� ��Ŷ�� ȹ�� �������� ����
			SendToGSCharacter( iGSUID, kNot.m_GetUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kNot );
			//}}

#else
#pragma ELSE_MESSAGE
			//SendToGSCharacter( LAST_SENDER_UID, kNot.m_GetUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kNot );
#endif SERV_CHANNEL_PARTY
			//}}

			//{{ 2009. 6. 23  ������	��� ȹ�� ���� ������ - ���������, �ŷ¾�����
			if( SiKDropTable()->IsNotifyDropItem( sItemData.m_iItemID ) )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( kNot.m_GetUnitUID );
				if( spRoomUser != NULL )
				{
					const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( sItemData.m_iItemID );
					if( pItemTemplet != NULL )
					{
						KEGS_NOTIFY_MSG_NOT	kPacket;
						kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_DROP_GET_ITEM; // ������ ȹ�� ����
						kPacket.m_Count = 1;
						kPacket.m_wstrMSG = spRoomUser->GetNickName();
						kPacket.m_wstrMSG += L";";
#ifdef SERV_UNITED_SERVER_EU
						kPacket.m_wstrMSG += pItemTemplet->m_ItemID;
#else //SERV_UNITED_SERVER_EU
						kPacket.m_wstrMSG += pItemTemplet->m_Name;
#endif //SERV_UNITED_SERVER_EU
						KncSend( PI_CN_ROOM, GetUID(), PI_CN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, ECN_NOTIFY_MSG_NOT, kPacket );
					}
					else
					{
						START_LOG( cerr, L"��� ȹ�� ������ ������ �������µ� �������� �ʴ� �������Դϴ�" )
							<< BUILD_LOG( kNot.m_GetUnitUID )
							<< BUILD_LOG( sItemData.m_iItemID )
							<< END_LOG;
					}
				}
				else
				{
					START_LOG( cerr, L"��� ȹ�� ������ ������ �������µ� �濡 ������ �����ϴ�" )
						<< BUILD_LOG( kNot.m_GetUnitUID )
						<< END_LOG;
				}
			}
			//}}
		}
		else
		{
#ifdef DUNGEON_ITEM
			if( true == SiKDropTable()->CheckPVPItem( kPacket_.m_kItemInfo.m_iItemID ) )
			{
				if( static_cast<float>(::rand()%100) > SiKDropTable()->GetPVPDropFailRate( KDropTable::PF_LOSETEAM ) )
					kNot.m_bIsItemSuccess = true;
			}
#endif DUNGEON_ITEM
			// �����Һ� �����۰� ED�������� ��ε�ĳ����
			std::map< UidType, float >::const_iterator mit;
			for( mit = sItemData.m_mapDamageByUser.begin(); mit != sItemData.m_mapDamageByUser.end(); ++mit )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mit->first );
				if( spRoomUser == NULL )
					continue;

				if( spRoomUser->IsBattleFieldNpcLoad() == false )
					continue;

				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_GET_ITEM_NOT, kNot );
			}
		}
	}

	return;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GET_ITEM_ACK, kPacket );	
}
#endif SERV_REFORM_ITEM_DROP
//}}

//{{ 2009. 5. 11  ������	�ǽð�������
IMPL_ON_FUNC( ERM_GET_ITEM_COMPLETE_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
		return;

	std::map< int, int >::const_iterator mit;
	for( mit = kPacket_.m_mapGetItem.begin(); mit != kPacket_.m_mapGetItem.end(); ++mit )
	{
		if( !m_spUserManager->AddItem( kPacket_.m_iUnitUID, mit->first ) )
		{
			START_LOG( cerr, L"������ ȹ�� ���� ������Ʈ ����!" )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
		}
	}
}
//}}

//{{ 2008. 9. 19  ������		���
_IMPL_ON_FUNC( ERM_DUNGEON_PLAY_INFO_TO_SERVER_NOT, KEGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cwarn, L"��Ŷ�� ���Ǵ� �� ���°� �ƴ�." )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;

		return;
	}

	if( !m_spUserManager->SetDungeonPlayInfo( FIRST_SENDER_UID, kPacket_ ) )
	{
		START_LOG( cwarn, L"������Ʈ �����߳�?" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}
}
//}}

_IMPL_ON_FUNC( ERM_USE_QUICK_SLOT_NOT, KEGS_USE_QUICK_SLOT_NOT )
{
	VERIFY_STATE( ( 1, KRoomFSM::S_PLAY ) );

    BroadCast( ERM_USE_QUICK_SLOT_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType )
{
	std::vector<KRoomUserManager::KLeaveUserInfo>	vecDelUserUID;
	const int iCount = m_spUserManager->DeleteUserByGsUID( kPacket_, vecDelUserUID );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_ )
		<< BUILD_LOG( iCount )
		<< BUILD_LOG( m_spUserManager->GetNumMember() );

	//����濡�� ������ ������ ������ ����.
	if( iCount == 0 )
		return;

	BOOST_TEST_FOREACH( const KRoomUserManager::KLeaveUserInfo&, kInfo, vecDelUserUID )
	{
		m_spUserManager->LeaveRoom( kInfo.m_iUnitUID );
	}

	BOOST_TEST_FOREACH( const KRoomUserManager::KLeaveUserInfo&, kInfo, vecDelUserUID )
	{
		if( IsEmpty() == false )
		{
			// ���� ��� �����鿡�� ���� ������ ������.
			KEGS_LEAVE_BATTLE_FIELD_NOT kPacketNot;
			kPacketNot.m_cRoomState		= GetStateID();
			kPacketNot.m_iUnitUID		= kInfo.m_iUnitUID;
			kPacketNot.m_iPartyUID		= kInfo.m_iPartyUID;
			GetRoomSlotInfo( kPacketNot.m_vecSlot );
			BroadCast( ERM_LEAVE_BATTLE_FIELD_NOT, kPacketNot );
		}
	}

	if( GetStateID() == KRoomFSM::S_LOAD  ||  GetStateID() == KRoomFSM::S_RESULT )
	{
		START_LOG( cerr, L"��Ʋ�ʵ忡���� ������� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( GetStateIDString() )
			<< BUILD_LOG( iCount )
			<< END_LOG;
	}

	// �濡�� ��� ������ �������� �ϴ� WAIT���·� �ٲ۴�.
	if( IsEmpty() == true )
	{
		StateTransition( KRoomFSM::I_TO_RESULT );
		StateTransition( KRoomFSM::I_TO_WAIT );
		m_tTimer[TE_WAIT_TIMER].restart();

		// ��Ʋ�ʵ� ��Ż�� �����ߴٸ� �� ������ �ѹ� ��������
		SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_DELETE );
	}
	else
	{
		// ��Ʋ�ʵ� ��Ż�� �����ߴٸ� �� ������ �ѹ� ��������
		SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );
	}
}

_IMPL_ON_FUNC( ERM_RESURRECT_TO_CONTINUE_DUNGEON_REQ, KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK );

	bool bDie;
	if( !m_spUserManager->IsDie( kPacket_.m_iUnitUID, bDie ) )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_17;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );

		return;
	}

	if( !bDie )
	{
		kPacket.m_iOK = NetError::ERR_RESURRECT_01;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );

		return;
	}

	if( !m_spUserManager->SetDie( kPacket_.m_iUnitUID, false ) )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_17;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );

		return;
	}

	kPacket.m_iDungeonID = (m_iDungeonID + static_cast<int>(m_cDifficultyLevel));
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );

	KEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT kPacketNot;
	kPacketNot.m_iUnitUID = kPacket_.m_iUnitUID;

	KRoomUserPtr roomSlotPtr = m_spUserManager->GetUser( kPacketNot.m_iUnitUID );
	if( roomSlotPtr == NULL )
	{
		START_LOG( cerr, L"�濡 ������ ��ü�� �������� ����." )
			<< BUILD_LOG( m_cRoomType )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}
	else
	{
		//{{ 2007. 11. 5  ������  ��Ȱ�� ��� ī��Ʈ
		roomSlotPtr->IncreaseUsedResurrectionStoneCount();
		//}}
		//{{ 2007. 12. 25  ������  ��Ȱ�� ��� ���
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_RES_STONE, kKey, KStatistics::eSIColResStone_UseCount, 1 );
		//}}
	}

	kPacketNot.m_iUsedByUnitUID = kPacket_.m_iUsedByUnitUID;
	kPacketNot.m_iStartPosIndex = 0;//SiCXSLDungeonManager()->GetStartPosByRandom( (m_iDungeonID + static_cast<int>(m_cDifficultyLevel)), m_iStageID, m_iSubStageID );
	BroadCast( EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT, kPacketNot );

	//{{ 2008. 6. 17  ������  �ǽð� ��Ȱ��
	m_spUserManager->SetEndPlay( kPacket_.m_iUnitUID, false );
	//}}
}

//IMPL_ON_FUNC_NOPARAM( ERM_SKIP_CONTINUE_DUNGEON_NOT )
//{
//	if( GetStateID() != KRoomFSM::S_PLAY )
//	{
//		START_LOG( cwarn, L"��Ŷ�� ���Ǵ� �� ���°� �ƴ�." )
//			<< BUILD_LOG( GetStateIDString() )
//			<< END_LOG;
//
//		return;
//	}
//
//	//{{ 2008. 6. 17  ������  �ǽð� ��Ȱ��
//	if( m_spUserManager->CheckEndPlayAnotherMember( FIRST_SENDER_UID ) )
//		EndPlay();
//	//}}
//}

//{{ 2008. 6. 16  ������  �ǽð� ��Ȱ��
_IMPL_ON_FUNC( ERM_STOP_DUNGEON_CONTINUE_TIME_REQ, KEGS_STOP_DUNGEON_CONTINUE_TIME_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_STOP_DUNGEON_CONTINUE_TIME_ACK, KEGS_STOP_DUNGEON_CONTINUE_TIME_ACK );

	// ��Ƽ�� �ð� ���߱� or Ǯ��
	m_spUserManager->StopDungeonContinueTime( FIRST_SENDER_UID, kPacket_.m_bIsStop );

	kPacket.m_iOK	  = NetError::NET_OK;
	kPacket.m_bIsStop = kPacket_.m_bIsStop;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STOP_DUNGEON_CONTINUE_TIME_ACK, kPacket );
}
//}}

//{{ 2011. 01. 25  ��μ�  ���� Ʈ���� ����(������ ������ - ���� PVP Ȯ��)
#ifdef SERV_WORLD_TRIGGER_RELOCATION
_IMPL_ON_FUNC( ECN_WORLD_TRIGGER_RELOCATION_REQ, KEGS_WORLD_TRIGGER_RELOCATION_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ECN_WORLD_TRIGGER_RELOCATION_ACK, KEGS_WORLD_TRIGGER_RELOCATION_ACK );

	// ��ȿ� ��� �������� ��ε� ĳ��Ʈ	
	KEGS_WORLD_TRIGGER_RELOCATION_NOT _Not;
	_Not.m_iWorldTrigger = kPacket_.m_iWorldTrigger;
	BroadCast( ECN_WORLD_TRIGGER_RELOCATION_NOT, _Not );
	
	// GSUser���� ACK����
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ECN_WORLD_TRIGGER_RELOCATION_ACK, kPacket );
}
#endif SERV_WORLD_TRIGGER_RELOCATION
//}}

IMPL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_NPC_LOAD_COMPLETE_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK, KEGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK );

    if( m_spRoomUserManager->IsExist( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"�ش� �濡 ������ ���µ� ��Ŷ�� ���ƿԴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_07;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK, kPacket );
		return;
	}

	if( m_spRoomUserManager->SetBattleFieldNpcSyncSubjects( FIRST_SENDER_UID, true ) == false )
	{
		START_LOG( cerr, L"NPC ����ȭ ����ڷ� ���� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_07;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK, kPacket );

	// ���忡�� NPC���� ����ȭ ��û�� ������!
	CheckNpcP2PSyncUser( true );
}


//#ifndef SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//
//IMPL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_NPC_P2P_SYNC_COMPLETE_NOT )
//{
//	if( GetStateID() != KRoomFSM::S_PLAY )
//	{
//		START_LOG( cerr, L"�߸��� ���¿��� ��Ŷ�� ������." )
//			<< BUILD_LOG( GetUID() )
//			<< BUILD_LOG( FIRST_SENDER_UID )
//			<< END_LOG;
//		return;
//	}
//
//	//{{ 2012. 12. 07	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_CODE_CLEANUP_2012_12_07
//	// [2012-12-07][������] �������� ���ܻ�Ȳ�̹Ƿ� ���� ���� ����� �� �ʿ䰡 ����.
//	if( m_spRoomUserManager->IsExist( FIRST_SENDER_UID ) == false )
//	{
//		START_LOG( cerr, L"NPC ����ȭ ����ڸ� ���� �����Ϸ��� �ϴµ� ������ �濡 ����!" )
//			<< BUILD_LOG( GetUID() )
//			<< BUILD_LOG( FIRST_SENDER_UID )
//			<< END_LOG;
//		return;
//	}
//#endif SERV_CODE_CLEANUP_2012_12_07
//	//}}
//
//	if( m_spRoomUserManager->SetBattleFieldNpcSyncSubjects( FIRST_SENDER_UID, false ) == false )
//	{
//		START_LOG( cerr, L"NPC ����ȭ ����ڷ� ���� ���� ����!" )
//			<< BUILD_LOG( GetUID() )
//			<< BUILD_LOG( FIRST_SENDER_UID )
//			<< END_LOG;
//	}
//}
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND


IMPL_ON_FUNC( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"�߸��� ���¿��� ��Ŷ�� ������." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	// ��Ƽ ���� ����
	m_spUserManager->UpdatePartyInfo( FIRST_SENDER_UID, kPacket_ );

	// ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
	SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );
}

IMPL_ON_FUNC( ERM_DUMP_ROOM_MONSTER_NOT )
{
	UidType iGSUID = 0;
	UidType iUnitUID = 0;
    if( m_spRoomUserManager->GetRoomUserGS( kPacket_.m_wstrNickName, iGSUID, iUnitUID ) == true )
	{
		if( m_kMonsterManager.IsMonsterAlive( kPacket_.m_iNpcUID ) == false )
		{
			START_LOG( cout, L"[����] �ش� ���ʹ� �׾����ϴ�!" )
				<< BUILD_LOG( kPacket_.m_wstrNickName )
				<< BUILD_LOG( kPacket_.m_iNpcUID );
		}
		else
		{
			START_LOG( cout, L"[����] �ش� ���ʹ� ����ֽ��ϴ�!" )
				<< BUILD_LOG( kPacket_.m_wstrNickName )
				<< BUILD_LOG( kPacket_.m_iNpcUID );

			// ���Ͱ� ���� ����ִٸ� ���� ���� ��û!
			//KRoomMonsterManager::NPC_DATA kDieNpcInfo;
			//if( m_kMonsterManager.GetNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
			//{
			//	kPacket.m_iOK = NetError::ERR_ROOM_20;
			//	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			//	return;
			//}
		}
	}
}

IMPL_ON_FUNC( ERM_DUMP_BATTLE_FIELD_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( clog, L"�÷��� ���� ���� �ƴϸ� üũ�� �ʿ� ����." )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< END_LOG;
		return;
	}

	START_LOG( cout, L"---------------------------------------------------------------------------------------" );

	//{{ 2012. 11. 20	������		���� ��Ʋ�ʵ� üũ �� ����
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	//////////////////////////////////////////////////////////////////////////
	// �濡 ������ ������ �ִٸ� 
	if( m_spUserManager->GetNumMember() > 0 )
	{
		// ȣ��Ʈ�� ����� �ִ��� �˻�
		if( m_spUserManager->GetHostCount() != 1 )
		{
			START_LOG( cout, L"host�� 1���� �ƴϴ�!! �̷� ���Ű��� ����!!!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( m_spUserManager->GetNumMember() )
				<< BUILD_LOG( m_spUserManager->GetHostCount() );
		}

		// ���� üũ ����
		if( m_spUserManager->ZU_IsCheckZU() == false )
		{
			START_LOG( cout, L"������ �����ϴµ� ���� ���� üũ�� �����ִ�!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( m_spUserManager->GetNumMember() )
				<< BUILD_LOG( m_spUserManager->ZU_IsCheckZU() );
		}
	}
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	//}}
	

	//////////////////////////////////////////////////////////////////////////
	// �ش� ������ �ʵ忡 �������ִ��� ã�ƺ���!
	if( kPacket_.m_wstrNickName.empty() == false )
	{
		std::vector< UidType > vecUserList;
		m_spUserManager->GetUnitUIDList( vecUserList );

		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUserList )
		{
			KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iUnitUID );
			if( IS_NULL( spRoomUser ) )
			{
				START_LOG( cerr, L"�����Ͱ��� �̻��մϴ�." )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
				continue;
			}

			if( spRoomUser->GetNickName() != kPacket_.m_wstrNickName )
				continue;

			START_LOG( cout, L"�ش� ������ ã�ҽ��ϴ�!" )
				<< BUILD_LOG( spRoomUser->GetNickName() )
				<< BUILD_LOG( spRoomUser->IsHost() );

			if( spRoomUser->GetPartyUID() == 0 )
			{
				START_LOG( cout, L"��Ƽ�� ���� ���� �ʽ��ϴ�!" )
					<< BUILD_LOG( spRoomUser->GetNickName() );
				continue;
			}

			std::vector< UidType > vecPartyMemberList;
			if( m_spUserManager->GetPartyMemberListInfo( spRoomUser->GetPartyUID(), vecPartyMemberList ) == false )
			{
				START_LOG( cerr, L"��Ƽ ����Ʈ ������ �����ϴ�! �Ͼ���� �ȵǴ� ����!" )
					<< BUILD_LOG( iUnitUID )
					<< BUILD_LOG( spRoomUser->GetPartyUID() )
					<< END_LOG;
				continue;
			}

			START_LOG( cout, L"��Ƽ�� ���� �ֽ��ϴ�!" )
				<< BUILD_LOG( spRoomUser->GetNickName() )
				<< BUILD_LOG( spRoomUser->GetPartyUID() )
				<< BUILD_LOG( vecPartyMemberList.size() );

			BOOST_TEST_FOREACH( const UidType, iPartyMemberUnitUID, vecPartyMemberList )
			{
				START_LOG( cout, L"��Ƽ�� UnitUID : " << iPartyMemberUnitUID );
			}
		}
	}
}



IMPL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_ZU_ECHO_ACK )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"�߸��� ���¿��� ��Ŷ�� ������." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}
    
    // �ƹ��͵� ���� �ʴ´�.
}

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
IMPL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_ZOMBIE_ALERT_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"�߸��� ���¿��� ��Ŷ�� ������." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

    if ( m_spUserManager->ZombieAlert_IsCheck() == false )
    {
        return;
    }

    if ( KRoomUserPtr spRoomUser = m_spUserManager->GetUser( FIRST_SENDER_UID ) )
    {
#ifdef  SERV_PVP_NEW_SYSTEM
        if ( spRoomUser->IsPvpNpc() == false )
#endif  
        {
            spRoomUser->ZombieAlert_Set( false );
            spRoomUser->ZombieAlert_Refresh();
        }
    }
}
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST



//{{ 2012. 11. 20	������		���� ��Ʋ�ʵ� üũ �� ����
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
IMPL_ON_FUNC( ERM_KILL_ZOMBIE_USER_IN_BATTLE_FIELD_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( clog, L"�÷��� ���� ���� �ƴϸ� üũ�� �ʿ� ����." )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< END_LOG;
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// �ش� ������ �ʵ忡 �������ִ��� ã�ƺ���!
	if( kPacket_.m_wstrNickName.empty() )
		return;

	std::vector< UidType > vecUserList;
	m_spUserManager->GetUnitUIDList( vecUserList );

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUserList )
	{
		KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iUnitUID );
		if( IS_NULL( spRoomUser ) )
		{
			START_LOG( cerr, L"�����Ͱ��� �̻��մϴ�." )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		if( spRoomUser->GetNickName() != kPacket_.m_wstrNickName )
			continue;

		START_LOG( cout, L"���� ������ ã�ҽ��ϴ�! �ش� ������ ������ ��Ż ��ŵ�ϴ�!" )
			<< BUILD_LOG( spRoomUser->GetNickName() )
			<< BUILD_LOG( spRoomUser->IsHost() );

		// ���� ��Ż ó��
		std::vector< UidType > vecDeleteZombie;
		vecDeleteZombie.push_back( spRoomUser->GetCID() );
		DeleteZombie( vecDeleteZombie );
	}
}
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
//}}

//{{ ��Ʋ�ʵ� ���赵 Ȯ�� ġƮ - ��μ�
#ifdef SERV_BATTLE_FIELD_DANGEROUS_CHEAT
IMPL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_DANGEROUS_VALUE_REQ )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"�߸��� ���¿��� ��Ŷ�� ������." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	KERM_BATTLE_FIELD_DANGEROUS_VALUE_ACK kPacket;
	kPacket = m_kGameManager.GetDangerousValue();

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BATTLE_FIELD_DANGEROUS_VALUE_ACK, kPacket );
}
#endif SERV_BATTLE_FIELD_DANGEROUS_CHEAT
//}}


//{{ 2013. 03. 12	������	���� ��ȿ �˻�
#ifdef SERV_CHECK_EXIST_MONSTER_UID
_IMPL_ON_FUNC( ERM_CHECK_EXIST_MONSTER_UID_REQ, KEGS_CHECK_EXIST_MONSTER_UID_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_CHECK_EXIST_MONSTER_UID_ACK, KEGS_CHECK_EXIST_MONSTER_UID_ACK );

	// ������ ���鼭 �ش� ���Ͱ� ������ �ִ��� Ȯ���Ѵ�
	BOOST_TEST_FOREACH( const int, iNpcUID, kPacket_.m_vecNpcUID )
	{
		// ������
        if( m_kMonsterManager.IsExistMonster( iNpcUID ) == false )
		{
			kPacket.m_vecNotExistNpcUID.push_back( iNpcUID );
		}
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHECK_EXIST_MONSTER_UID_ACK, kPacket );
}
#endif SERV_CHECK_EXIST_MONSTER_UID
//}}

//{{ ��Ʋ�ʵ� Ŀ�´�Ƽ ����Ʈ Ȯ�� - ��μ�
#ifdef SERV_COMMUNITY_IN_BATTLE_FIELD
_IMPL_ON_FUNC( ERM_COMMUNITY_IN_BATTLE_FIELD_REQ, KEGS_COMMUNITY_USER_LIST_REQ )
{
	KEGS_COMMUNITY_USER_LIST_ACK kPacket;
	kPacket.m_iOK		  = NetError::NET_OK;
	kPacket.m_uiTotalPage = 1;
	kPacket.m_uiViewPage  = 1;

	std::vector< UidType > vecUserList;
	m_spUserManager->GetUnitUIDList( vecUserList );
	
	BOOST_TEST_FOREACH( UidType, iUnitUid, vecUserList )
	{
		KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iUnitUid );

		if( spRoomUser )
		{
			KRoomUserInfo kInfo;
			spRoomUser->GetRoomUserInfo( kInfo );

			KCommunityUserInfo kUser;
			kUser.m_iUnitUID = kInfo.m_nUnitUID;
			kUser.m_wstrNickName = kInfo.m_wstrNickName;
			kUser.m_cUnitClass = kInfo.m_cUnitClass;
			kUser.m_ucLevel = kInfo.m_ucLevel;
			kUser.m_cState = KCommunityUserInfo::US_NONE;	
			kUser.m_iStateCode = 0;
			kUser.m_iRoomUID = GetUID();
			kUser.m_iED = 0;

			kPacket.m_vecUserList.push_back( kUser );
		}
	}

	// ���ϴ� �������� ������ ��������(����)
	GetPVPUserList( kPacket_.m_uiViewPage, kPacket.m_uiTotalPage, kPacket.m_vecUserList );

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_COMMUNITY_IN_BATTLE_FIELD_ACK, kPacket );
};

bool KBattleFieldRoom::GetPVPUserList( IN UINT& uiViewPage, IN UINT& uiTotalPage, IN OUT std::vector< KCommunityUserInfo >& vecUserList )
{
	std::vector< KCommunityUserInfo > vecUserListCopy;

	vecUserListCopy = vecUserList;
	vecUserList.clear();

	_JIF( uiViewPage > 0, return false );

	if( vecUserListCopy.empty() )
	{
		uiViewPage = 1;
		uiTotalPage = 1;
		return true;
	}

	uiTotalPage = vecUserListCopy.size() / PAGE_PER_BATTLE_FIELD_USER;
	if( ( vecUserListCopy.size() % PAGE_PER_BATTLE_FIELD_USER ) > 0 )
	{
		++uiTotalPage;
	}

	u_int uiBeginIndex = ( uiViewPage * PAGE_PER_BATTLE_FIELD_USER ) - PAGE_PER_BATTLE_FIELD_USER;
	if( vecUserListCopy.size() <= uiBeginIndex )
	{
		uiViewPage = uiTotalPage; // ������ ������
		uiBeginIndex = ( uiViewPage * PAGE_PER_BATTLE_FIELD_USER ) - PAGE_PER_BATTLE_FIELD_USER;

		if( vecUserListCopy.size() <= uiBeginIndex )
		{
			START_LOG( cerr, L"�Ͼ �� ���� ����!" )
				<< BUILD_LOG( uiViewPage )
				<< BUILD_LOG( uiTotalPage )
				<< BUILD_LOG( uiBeginIndex )
				<< BUILD_LOG( vecUserListCopy.size() )
				<< END_LOG;
			return false;
		}
	}

	std::vector< KCommunityUserInfo >::reverse_iterator vit = vecUserListCopy.rbegin();
	vit += uiBeginIndex;

	while( vit != vecUserListCopy.rend() )
	{	
		vecUserList.push_back( *vit );

		if( vecUserList.size() >= PAGE_PER_BATTLE_FIELD_USER )
			break;

		++vit;
	}

	return true;
}
#endif SERV_COMMUNITY_IN_BATTLE_FIELD
//}}

void KBattleFieldRoom::MakePacket_BattleFieldLeaveLog( IN const UidType iUnitUID, IN const int iReason, OUT KELOG_BATTLE_FIELD_LEAVE_LOG_NOT& kPacket )
{
	KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"������ ã�� �� �����ϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return;
	}

	switch( iReason )
	{
	case NetError::NOT_LEAVE_ROOM_REASON_01:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_DISCONNECT;				break;	// üũ
	case NetError::NOT_LEAVE_ROOM_REASON_29:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_CHANNEL_CHANGE;			break;	// üũ
	case NetError::NOT_LEAVE_ROOM_REASON_27:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_WALK_TO_DIF_FIELD;		break;	// üũ
	case NetError::NOT_LEAVE_ROOM_REASON_30:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_USE_WARP_ITEM;			break;	// üũ
	case NetError::NOT_LEAVE_ROOM_REASON_31:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_MOVE_PARTY_FIELD;		break;	// üũ
	case NetError::NOT_LEAVE_ROOM_REASON_28:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_ENTER_THE_DUNGEON;		break;	// üũ
	case NetError::NOT_LEAVE_ROOM_REASON_32:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_ENTER_THE_PVP;			break;	// üũ
	case NetError::NOT_LEAVE_ROOM_REASON_33:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_ABNORMAL_DISCONNECT;	break;	// üũ
	case NetError::NOT_LEAVE_ROOM_REASON_23:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_CLIENT_CRASH;			break;	// üũ
	default:									kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_NONE;					break;
	}
	
	CTime tCurTime = CTime::GetCurrentTime();
	kPacket.m_wstrServerIP	 = NetCommon::GetLocalIPW();
	kPacket.m_iBattleFieldID = GetBattleFieldID();
	kPacket.m_iRoomUID		 = GetUID();
	kPacket.m_iUnitUID		 = iUnitUID;
	kPacket.m_wstrNickName	 = spRoomUser->GetNickName();
	kPacket.m_cUnitClass	 = spRoomUser->GetUnitClass_LUA();
	kPacket.m_ucLevel		 = spRoomUser->GetLevel();
	kPacket.m_iPlayTime		 = static_cast<int>(spRoomUser->GetPlayTime());
	kPacket.m_iGetEXP		 = spRoomUser->GetRewardEXP() + spRoomUser->GetRewardPartyEXP();
	kPacket.m_iGetED		 = spRoomUser->GetRewardED();
	kPacket.m_wstrRegDate	 = (const wchar_t*)tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
	//{{ 2012. 11. 20	�ڼ���	Field Frame �α� �߰�
#ifdef SERV_FIELD_FRAME_LOG
	kPacket.m_iFieldFrame	 = spRoomUser->GetFieldFrame();
#endif SERV_FIELD_FRAME_LOG
	//}}

#ifdef SERV_ADD_FIELD_LOG_COLUMN
	kPacket.m_sNpcDieCount		= spRoomUser->GetKillNPC_LUA();
#endif SERV_ADD_FIELD_LOG_COLUMN
}

//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT
void KBattleFieldRoom::CheckDropEventCount()
{
	// ���� �������� ���� ����ȭ ���� üũ
	if( m_tTimer[TE_DROP_EVENT_CHECK].elapsed() < 10.0 )
		return;

#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
	if( SiKGameEventManager()->CheckItemDropProbEvent( m_fItemDropEventProbRate ) == true )
	{
		if( m_fItemDropEventProbRate < REE_DROP_EVENT_PROB_DEFAULT )
			m_fItemDropEventProbRate = REE_DROP_EVENT_PROB_DEFAULT;
	}
#else // SERV_DROP_EVENT_RENEWAL
	if( SiKGameEventManager()->CheckItemDropProbEvent( m_iItemDropEventProbCount ) == true )
	{
		if( m_iItemDropEventProbCount < REE_DROP_EVENT_PROB_DEFAULT )
			m_iItemDropEventProbCount = REE_DROP_EVENT_PROB_DEFAULT;
	}
#endif // SERV_DROP_EVENT_RENEWAL

	//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
	float fQuestItemDropEventProbRate = 0.f;
	if( SiKGameEventManager()->CheckQuestItemDropProbEvent( fQuestItemDropEventProbRate ) == true )
	{
		m_fQuestItemDropEventProbRate = fQuestItemDropEventProbRate + 1.f;

		if( m_fQuestItemDropEventProbRate < REE_DROP_EVENT_PROB_DEFAULT )
			m_fQuestItemDropEventProbRate = 1.f;
	}
#endif SERV_QUEST_ITEM_DROP_EVENT
	//}
}
#endif SERV_ITEM_DROP_EVENT
//}}

//{{ 2012. 12. 30	�ڼ���	�ʵ� �̺�Ʈ ����
#ifdef SERV_FIELD_EVENT_MONSTER
void KBattleFieldRoom::CheckEventMonster()
{
	// ���� �������� �̺�Ʈ ���� ������ üũ�մϴ�!
	if( m_tTimer[TE_EVENT_MONSTER].elapsed() < 1.0 )
		return;

	m_tTimer[TE_EVENT_MONSTER].restart();

	//////////////////////////////////////////////////////////////////////////
	// �̺�Ʈ ���� �ִ� �����ð� üũ
	DWORD dwElapTime = GetTickCount();

	// ���� üũ
	KEGS_NPC_UNIT_CREATE_NOT			kNpcCreateNot;
	std::vector<KERM_NPC_UNIT_DIE_NOT>	veckPacketNpcDie;
	if( m_kMonsterManager.CheckEventMonster( GetBattleFieldID(), kNpcCreateNot, veckPacketNpcDie ) == true )
	{
		// ���� ó��
		if( kNpcCreateNot.m_vecNPCUnitAck.empty() == false )
		{
			kNpcCreateNot.m_iBattleFieldID = GetBattleFieldID();
			BroadCast( ERM_NPC_UNIT_CREATE_NOT, kNpcCreateNot, KRoomUserManager::UL_NPC_LOAD_USER );
		}
		
		// ���� ó��
		BOOST_TEST_FOREACH( KERM_NPC_UNIT_DIE_NOT&, kNpcDietNot, veckPacketNpcDie )
		{
			BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
		}
	}

	dwElapTime = GetTickCount() - dwElapTime;
	if( m_dwMonsterRespawnCheckTick < dwElapTime )
	{
		START_LOG( cout, L"[�˸�] �̺�Ʈ ���� üũ Ÿ�� �ִ� �ð� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( dwElapTime )
			<< BUILD_LOG( m_dwMonsterRespawnCheckTick );

		m_dwMonsterRespawnCheckTick = dwElapTime;
	}
	//////////////////////////////////////////////////////////////////////////	
}
#endif SERV_FIELD_EVENT_MONSTER
//}}

//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
void KBattleFieldRoom::CheckMiddleBossMonster()
{
	// ���� �������� ���� ���� ������ üũ�մϴ�!
	if( m_tTimer[TE_MIDDLE_BOSS_MONSTER].elapsed() < 5.0 )
		return;

	m_tTimer[TE_MIDDLE_BOSS_MONSTER].restart();

	//////////////////////////////////////////////////////////////////////////
	// ���� ���� �ִ� �����ð� üũ
	DWORD dwElapTime = ::GetTickCount();  

	// ���� üũ
	KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT kPacketNot;	
	if( m_kMonsterManager.CheckMiddleBossMonster( GetBattleFieldID(), m_kGameManager.GetDangerousValue(), m_kGameManager.GetDangerousEventInfo(), kPacketNot ) == true )
	{
		if( kPacketNot.m_kCreatePacket.m_vecNPCUnitAck.empty() == false )
		{
			// ���� ������ ó���� �Ϸ�Ǿ��ٸ� 
			kPacketNot.m_kCreatePacket.m_iBattleFieldID = GetBattleFieldID();
			BroadCast( ERM_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT, kPacketNot, KRoomUserManager::UL_NPC_LOAD_USER );
            
            KFieldBossGenKillStat kLog;
            kLog.m_nFieldID = kPacketNot.m_kCreatePacket.m_iBattleFieldID;
            //kLog.m_nNumGenBoss = kPacketNot.m_kCreatePacket.m_vecNPCUnitAck.size();

            //std::vector< KNPCUnitNot > m_vecNPCUnitAck
            BOOST_TEST_FOREACH( KNPCUnitNot, kNPCUnit, kPacketNot.m_kCreatePacket.m_vecNPCUnitAck )
            {
                 kLog.m_nSpawnID = kNPCUnit.m_kNPCUnitReq.m_iBossGroupID;
            }
            
            SendToLogDB( ELOG_GEN_FIELD_MIDDLE_BOSS_STAT_NOT, kLog );

            START_LOG( cwarn, L"�ʵ� �߰� ���� ���� ��� �׽�Ʈ " )
                << BUILD_LOG( kLog.m_nFieldID )
                << BUILD_LOG( kLog.m_nSpawnID )
                //<< BUILD_LOG( kLog.m_nNumGenBoss )
                << END_LOG;
		}

		// �߰� ���� ���Ͱ� �����ص� ���赵�� �ʱ�ȭ ���� �ʴ´�.
	}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT kEventPacketNot;	
	if( m_kMonsterManager.CheckEventBossMonster( GetBattleFieldID(), m_kGameManager.GetDangerousValue(), m_kGameManager.GetDangerousEventInfo(), kEventPacketNot ) == true )
	{
		if( kEventPacketNot.m_kCreatePacket.m_vecNPCUnitAck.empty() == false )
		{
			// ���� ������ ó���� �Ϸ�Ǿ��ٸ� 
			kEventPacketNot.m_kCreatePacket.m_iBattleFieldID = GetBattleFieldID();
			BroadCast( ERM_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT, kEventPacketNot, KRoomUserManager::UL_NPC_LOAD_USER );
		}

		// �߰� ���� ���Ͱ� �����ص� ���赵�� �ʱ�ȭ ���� �ʴ´�.
	}
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT
	dwElapTime = ::GetTickCount() - dwElapTime;
	if( m_dwMonsterRespawnCheckTick < dwElapTime )
	{
		START_LOG( cout, L"[�˸�]�߰� ���� ���� üũ Ÿ�� �ִ� �ð� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( dwElapTime )
			<< BUILD_LOG( m_dwMonsterRespawnCheckTick );

		m_dwMonsterRespawnCheckTick = dwElapTime;
	}
	//////////////////////////////////////////////////////////////////////////	
}

void KBattleFieldRoom::BattleFieldMiddleBossDropProcess( IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, IN KRoomMonsterManager::NPC_DATA& kDieNpcInfo, IN const UidType& iSendUnitUID, OUT std::vector< KDropItemData >& vecDropItem_ )
{
	const CXSLUnitManager::NPCUnitTemplet* pNPCTemplet = SiCXSLUnitManager()->GetNPCUnitTemplet( (CXSLUnitManager::NPC_UNIT_ID)kDieNpcInfo.m_iNPCID );
	if( pNPCTemplet == NULL )
	{
		START_LOG( cwarn, L"NPC Unit Templet �б� ����.!" )
			<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
			<< END_LOG;
		return;
	}

	// �Ӽ� �������� üũ
	const bool bIsAttribNpc = m_kMonsterManager.IsAttribNpc( kPacket_.m_nDieNPCUID );
	const int iAttibAcount = m_kMonsterManager.GetAttribNpcAttribCount( kPacket_.m_nDieNPCUID );

	// npc��������
	const bool bDropDieState = IsDropDieState( kPacket_.m_cNpcDieState );
	const bool bAddRoomUserKillCount = ( bDropDieState  &&  kPacket_.m_cNpcDieState != KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_SELF );
	
	// ���� Ÿ�� ��� ���
	const float fMonsterTypeFactor = static_cast<float>( CXSLUnitManager::GetBattleFieldMonsterTypeFactor( iAttibAcount, kDieNpcInfo.m_cMonsterGrade ) );

	// �ʵ� ��� ���
	//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
	const float fFieldEDFactor = SiCXSLBattleFieldManager()->GetBattleFieldEDFactor() * fMonsterTypeFactor;
	const float fFieldEXPFactor = SiCXSLBattleFieldManager()->GetBattleFieldEXPFactor() * fMonsterTypeFactor;
#else
	const float fFieldFactor = SiCXSLBattleFieldManager()->GetBattleFieldFactor() * fMonsterTypeFactor;
#endif SERV_FIELD_ED_EXP_FACTOR
	//}

	//////////////////////////////////////////////////////////////////////////
	// uiAttUnit���� -1�̸� ������ ������ ���� �ʰ� npc�� �ڻ��Ѱ���. [��:��������]
	// �÷��̾ ���͸� ���ΰŶ�� �ش� �÷��̾��� ų���ͼ� üũ
	if( pNPCTemplet->m_bIsNPC  &&  bAddRoomUserKillCount == true  &&  kPacket_.m_uiAttUnit != -1 )
	{
		if( m_spUserManager->AddKillNPC( kPacket_.m_uiAttUnit ) == false )
		{
			if( m_kMonsterManager.IsExistMonster( static_cast<int>(kPacket_.m_uiAttUnit) ) == true )
			{
				START_LOG( clog, L"������ �׿���!!" )
					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
					<< BUILD_LOG( kPacket_.m_uiAttUnit );

				// ���Ͱ� ������ �ɷ� �״°���� : �������� �״�� ���ó����.
			}
		}
	}	
	//////////////////////////////////////////////////////////////////////////
	// 1. ���� ������ ���� ��� �������� �����Ǵ� ���� ���
	if( kDieNpcInfo.m_bNoDrop == false  &&  bDropDieState == true )
	{
		//////////////////////////////////////////////////////////////////////////
		// �� �������� �ٸ� ������ ��Ŷ�� ������ �ϹǷ� �̷��� ó���Ѵ�.
		KERM_NPC_UNIT_DIE_NOT kNpcDietNot;
		kNpcDietNot.m_iDungeonID		 = m_iDungeonID;
		kNpcDietNot.m_cDifficulty		 = m_cDifficultyLevel;
		kNpcDietNot.m_nDieNPCUID		 = kPacket_.m_nDieNPCUID;
		kNpcDietNot.m_iNPCID			 = kDieNpcInfo.m_iNPCID;
		kNpcDietNot.m_cAttUnitType		 = kPacket_.m_cAttUnitType;
		kNpcDietNot.m_uiAttUnit			 = kPacket_.m_uiAttUnit;
		kNpcDietNot.m_bNoDrop			 = kDieNpcInfo.m_bNoDrop;
		kNpcDietNot.m_cDungeonMode		 = m_cDungeonMode;
		kNpcDietNot.m_sSubStageExpNpcNum = m_kMonsterManager.GetAtStartedMonsterCount();
		kNpcDietNot.m_cUserCount		 = 0;
		kNpcDietNot.m_bDecreaseEndurance = false;
		kNpcDietNot.m_bQuestComplete	 = false;			// ��Ƽ�� �����Ͽ� �� �������� npc hp �� 50% �̻��̸� ����Ʈ �Ϸ� ����

		// ���� ������ ���� �� ��� �Ǿ�� �� ����
		// 1. �⿩�� (X�� ǥ��)
		std::map< UidType, float >	mapUserContribution;
		int iNpcMaxHP = kPacket_.m_iNpcMaxHP;
		std::map< UidType, float >::const_iterator mitDamageByUser = kPacket_.m_mapDamageByUser.begin();
		for( ; mitDamageByUser != kPacket_.m_mapDamageByUser.end() ; ++mitDamageByUser )
		{
			std::vector<UidType> vecPartyUserList;

			// ���� ���� �ʵ忡 �ִ� ��Ƽ�� ��
			const int iPartyMemberCount = m_spUserManager->GetSamePartyMember( mitDamageByUser->first );

			float fContribution 
				= m_spUserManager->GetPartyTotalDamage( mitDamageByUser->first, kPacket_.m_mapDamageByUser ,vecPartyUserList ) 
				/ (static_cast<float>(iNpcMaxHP)) / iPartyMemberCount;

			if( fContribution > 1.5f )
				fContribution = 1.5f;

			if( fContribution < 0.08f )
				fContribution = 0.f;

			// �ַ� �÷��̸� ��Ƽ�� ������ ����� ���� ���� �ִ�. �׷��� �ѹ� ����� �Ѵ�.
			mapUserContribution.insert( std::make_pair( mitDamageByUser->first, fContribution ) );

			// ��Ƽ ���� ������ �⿩���� �����Ѵ�.
			BOOST_TEST_FOREACH( UidType, iPartyUnitUID, vecPartyUserList )
			{
				mapUserContribution.insert( std::make_pair( iPartyUnitUID, fContribution ) );
			}
		}

		// �⿩�� ���� ���ϱ�
		// ���� 30% ��Ƽ
		std::map< UidType, bool >	mapBonusItem;  // <unit UID, ���� ����>
		m_spUserManager->CalculatePartyRank( kPacket_.m_mapDamageByUser, mapBonusItem );
		
		// 2. ������ ��� ���� ���
		KDropTable::KBattleFieldNpcDropInfo kDropInfo_Init;
		if( SiKDropTable()->GetBattleFieldNpcDropInfo( GetBattleFieldID(), kDieNpcInfo.m_iNPCID, kDropInfo_Init ) == false )
		{
			//////////////////////////////////////////////////////////////////////////
			// ���� ���� ���� �ϱ� ������ ������ �ʱ�ȭ�� �ʿ��ϴ�.
			//����� �ο����� �˷��� ������ ���� ����..
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// ��Ƽ�� �����Ͽ� �� �������� npc hp �� 50% �̻��̸� ����Ʈ �Ϸ� ����
			kNpcDietNot.m_EXPList.clear();
			//////////////////////////////////////////////////////////////////////////

			START_LOG( cerr, L"��Ʋ �ʵ� npc drop info ������ ����" )
				<< BUILD_LOG( GetBattleFieldID() )
				<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
				<< END_LOG;

			BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
			return;
		}

		// 3. �⿩���� �ִ� �������� EXP, ED, DropItem ���
		std::map< UidType, KRoomUserPtr > mapRoomUserList;
		if( m_spUserManager->GetRoomUserList( mapRoomUserList ) == false )
		{
			//////////////////////////////////////////////////////////////////////////
			// ���� ���� ���� �ϱ� ������ ������ �ʱ�ȭ�� �ʿ��ϴ�.
			//����� �ο����� �˷��� ������ ���� ����..
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// ��Ƽ�� �����Ͽ� �� �������� npc hp �� 50% �̻��̸� ����Ʈ �Ϸ� ����
			kNpcDietNot.m_EXPList.clear();
			//////////////////////////////////////////////////////////////////////////

			START_LOG( cerr, L"��������  ����Ʈ�� ã�� ���� ����." )
				<< END_LOG;

			//////////////////////////////////////////////////////////////////////////
			// �ʵ� �������� �ش� npc�� �����Ϳ� ���� ������ ��ε� ĳ���� �Ѵ�! ( �Ϻ����� ���, ����ġ ���� )
			BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
			return;
		}

		std::map< UidType, KRoomUserPtr >::iterator mitUserList = mapRoomUserList.begin();
		for(  ; mitUserList != mapRoomUserList.end() ; ++mitUserList )
		{
			KDropTable::KBattleFieldNpcDropInfo kDropInfo;
			kDropInfo = kDropInfo_Init;

			if( mitUserList->second == NULL )
				continue;

			//////////////////////////////////////////////////////////////////////////
			// ���� ���� ���� �ϱ� ������ ������ �ʱ�ȭ�� �ʿ��ϴ�.
			//����� �ο����� �˷��� ������ ���� ����..
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// ��Ƽ�� �����Ͽ� �� �������� npc hp �� 50% �̻��̸� ����Ʈ �Ϸ� ����
			kNpcDietNot.m_EXPList.clear();
			//////////////////////////////////////////////////////////////////////////

			if( mitUserList->second->IsBattleFieldNpcLoad() == false )
				continue;

			// �ش� ������ ���͸� ���϶� ��ŭ ������ �ߴ��� �˻�����!
			std::map< UidType, float >::iterator mitUserContribution = mapUserContribution.find( mitUserList->first );
			if( mitUserContribution == mapUserContribution.end() )
			{
				// ���嵵�� ������ ���� ���� �ڰ��� ����!
				SendToGSCharacter( mitUserList->second->GetGSUID(), mitUserList->second->GetCID(), ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot );
				continue;
			}

			KDropTable::DROP_DATA sDropDataNpc;

			// ���� ���� �ʵ忡 �ִ� ��Ƽ�� ��
			kNpcDietNot.m_cUserCount	= static_cast<char>( m_spUserManager->GetSamePartyMember( mitUserList->first ) );

			// ���� ����� ���( ��Ƽ ���� ����)
			float fPartyLevelFactorMin = 0.f;
			float fPartyLevelFactorMax = 0.f;

			m_spUserManager->GetPartyLevelFactor( mitUserList->first, kDieNpcInfo.m_cLevel, fPartyLevelFactorMin, fPartyLevelFactorMax );

			const float fLevelFactor = (fPartyLevelFactorMin < fPartyLevelFactorMax) ? fPartyLevelFactorMin : fPartyLevelFactorMin;
            
			// ��Ƽ �÷����� ��� ������ Ȯ�� ����
			const float fPartyDropBonus = m_spUserManager->GetBattleFieldPartyBonusRate( mitUserList->first ) + 1.0f;	

			// ��Ƽ�� �⿩���� 50% �̻��ΰ�?
			if( mitUserContribution->second * kNpcDietNot.m_cUserCount >= 0.5f )
			{
				kNpcDietNot.m_bQuestComplete = true;
			}

			// ���������� ���/���� �Ǿ�� �ϴ� ǰ��
			// 1. ED
			// 2. EXP
			// 3. ������
			if( kDropInfo.m_iExp > 0 )
			{
				kDropInfo.m_iExp = static_cast<int>(SiKDropTable()->GetNpcExp( kDieNpcInfo.m_cLevel ));

				// ����ġ�� �ִ� ���Ͷ�� �������� ���� �ȴ�.
				kNpcDietNot.m_bDecreaseEndurance = true;
			}

			if( kDropInfo.m_iED > 0 )
			{
				kDropInfo.m_iED = (10 + ( kDropInfo.m_iED * kDieNpcInfo.m_cLevel ));
				// �ش� �ʵ�� ��� ��� ED * 100
				kDropInfo.m_iED = kDropInfo.m_iED * 20;
			}

			// �ߺ��� ����ϴ� �Ϲ� ����� ���� ������ �����ؾ߸� ����Ѵ�.
			float fUserContribution = mitUserContribution->second;
			if( fLevelFactor <= 0.f )
			{
				fUserContribution = 0.f;
			}

			if( SiKDropTable()->BattleFieldNpcDropItem( kDropInfo
				//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
													  , fFieldEDFactor
													  , fFieldEXPFactor
#else
													  , fFieldFactor
#endif SERV_FIELD_ED_EXP_FACTOR
													  //}
													  , fLevelFactor
													  , fUserContribution
													  , kNpcDietNot.m_cUserCount
													  , fPartyDropBonus
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
													  , m_fItemDropEventProbRate
#else // SERV_DROP_EVENT_RENEWAL
													  , m_iItemDropEventProbCount
#endif // SERV_DROP_EVENT_RENEWAL
													  , kDieNpcInfo.m_bIsBoss
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-14	// �ڼ���
													  , 1.0f
#endif // SERV_BATTLE_FIELD_BOSS
													  , sDropDataNpc
													  ) == false )
			{
				START_LOG( cerr, L"Drop Item Data ������ ���� ===" )
					<< BUILD_LOG( GetBattleFieldID() )
					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
					<< END_LOG;
			}
	
			//////////////////////////////////////////////////////////////////////////
			// �ʵ� �߰� ���� ���ʽ� ������
			// �⿩�� ���� 30% �̻��̸� �ʵ� �ߺ� ť�� 2�� ����
			// �⿩�� �׿� �̸�			�ʵ� �ߺ� ť�� 1�� ����		
			CXSLBattleFieldManager::KFieldMiddleBossDropInfo kMiddleBossDropInfo;
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
			CXSLBattleFieldManager::KFieldEventBossDropInfo kEventBossDropInfo;
			//////////////////////////////////////////////////////////////////////////
			// �ʵ� �̺�Ʈ ���� ���ʽ� ������
			// �⿩�� ���� 30% �̻��̸� �̺�Ʈ ���ʽ� ������ 2�� ����
			// �⿩�� �׿� �̸�			�̺�Ʈ ���ʽ� ������ 1�� ����		
			if( SiCXSLBattleFieldManager()->GetBattleFieldEventBossDropInfo( static_cast<int>(GetBattleFieldID()), kDieNpcInfo.m_iNPCID, kEventBossDropInfo ) == true )
			{
				// �⿩���� 8% �̻��̰� , ��Ƽ �������̰� 10�� �϶�� ������ ����
				bool bGiveCommonBonusItem = ( mitUserContribution->second > 0.f && fLevelFactor > 0.f ) ? true : false;

				if( bGiveCommonBonusItem == true )
				{
					sDropDataNpc.m_vecItemID.push_back( 70070 ); // HP 100% ���ȸ�� ����
					sDropDataNpc.m_vecItemID.push_back( 70050 ); // MP 150 ���ȸ�� ����

					// �߰� ������ ȥ �� ���� ������ �� �ٰ��ΰ�? 
					for( int iBonusCount = 0 ; iBonusCount < kEventBossDropInfo.m_iEventBossCommonBonusItemCount ; ++iBonusCount )
					{
						if( kEventBossDropInfo.m_iEventBossCommonBonusItemID <= 0 )
							break;

						sDropDataNpc.m_vecItemID.push_back( kEventBossDropInfo.m_iEventBossCommonBonusItemID );
					}

					// �ʵ� �߰� ���� ���ʽ� ������ - �ʵ� �߰� ���� ť��
					std::map< UidType, bool >::iterator mitBonus = mapBonusItem.find( mitUserList->second->GetCID() );
					if( mitBonus != mapBonusItem.end() )
					{
						if( mitBonus->second == true )				
						{
							// ���������� �⿩���� ���� 30%�� �Ǹ� �ΰ� ��
							for( int iCount = 0 ; iCount < kEventBossDropInfo.m_iEventBossBonusItemIDCount_High ; ++iCount )
							{
								if( kEventBossDropInfo.m_iEventBossBonusItemID <= 0 )
									break;

								sDropDataNpc.m_vecItemID.push_back( kEventBossDropInfo.m_iEventBossBonusItemID ); // �̺�Ʈ ���ʽ� ������ ť��
							}
						}
						else
						{
							// ���������� �⿩���� ���� ���� 30%�� �ȵǸ� �Ѱ� ��
							for( int iCount = 0 ; iCount < kEventBossDropInfo.m_iEventBossBonusItemIDCount_Low ; ++iCount )
							{
								if( kEventBossDropInfo.m_iEventBossBonusItemID <= 0 )
									break;

								sDropDataNpc.m_vecItemID.push_back( kEventBossDropInfo.m_iEventBossBonusItemID ); // �̺�Ʈ ���ʽ� ������ ť��
							}
						}
					}
				}
				// �������� ������ �翬 ����
				//////////////////////////////////////////////////////////////////////////
			}
			else
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT
			if( SiCXSLBattleFieldManager()->GetBattleFieldMiddleBossDropInfo( static_cast<int>(GetBattleFieldID()), kDieNpcInfo.m_iNPCID, kMiddleBossDropInfo ) == true )
			{
				// �⿩���� 8% �̻��̰� , ��Ƽ �������̰� 10�� �϶�� ������ ����
				bool bGiveCommonBonusItem = ( mitUserContribution->second > 0.f && fLevelFactor > 0.f ) ? true : false;

				if( bGiveCommonBonusItem == true )
				{
#ifdef SERV_CAMERA_EVENT
					CTime tCurrentTime = CTime::GetCurrentTime();
					if( tCurrentTime < CTime(2013, 8, 21, 9, 0, 0)  )
					{
						if( mitUserList->second->IsEquippedItem( 60006500 ) == true || mitUserList->second->IsEquippedItem( 60006501 ) == true )
						{
							sDropDataNpc.m_vecItemID.push_back( 60006543 );
						}
					}
#endif SERV_CAMERA_EVENT

#ifdef SERV_BURNING_CHAR_EVENT_SUB_QUEST
					////bool isbCharMale = false;
					//bool isbCharQuest = false;
					////CXSLUnit::UNIT_TYPE eUnitType = (CXSLUnit::UNIT_TYPE)mitUserList->second->GetUnitType();

					//KRoomUserInfo kInfoChar;
					//mitUserList->second->GetRoomUserInfo(kInfoChar);

					//std::map< int, KSubQuestInfo >::const_iterator mitQ = kInfoChar.m_mapOngoingQuest.find(_CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBestFieldPlayerQuest);
					//if( mitQ != kInfoChar.m_mapOngoingQuest.end() )
					//{
					//	isbCharQuest = true;
					//}

					//// ���� ����Ʈ ���� ������ ĳ���� ���� ���� �񱳿��� ���� ��쿡�� ������ ����
					////if(_CONST_BURNING_CHAR_EVENT_SUB_QUEST_::bCharMale == isbCharMale && true == isbCharQuest)
					//if(true == isbCharQuest)
					//{
					//	sDropDataNpc.m_vecItemID.push_back( _CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBestFieldPlayerItem );

					//	START_LOG( clog, L"�輮��_ĳ���� �����̺�Ʈ_�ʵ庸�� óġ ����������" )
					//		<< BUILD_LOG( _CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBestFieldPlayerItem )
					//		<< BUILD_LOG( _CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBestFieldPlayerQuest )
					//		<< END_LOG;
					//}
#endif //SERV_BURNING_CHAR_EVENT_SUB_QUEST

					sDropDataNpc.m_vecItemID.push_back( 70070 ); // HP 100% ���ȸ�� ����
					sDropDataNpc.m_vecItemID.push_back( 70050 ); // MP 150 ���ȸ�� ����

					// �߰� ������ ȥ
					for( int iBonusCount = 0 ; iBonusCount < kMiddleBossDropInfo.m_iMiddleBossCommonBonusItemCount ; ++iBonusCount )
					{
						if( kMiddleBossDropInfo.m_iMiddleBossCommonBonusItemID <= 0 )
							break;

						sDropDataNpc.m_vecItemID.push_back( kMiddleBossDropInfo.m_iMiddleBossCommonBonusItemID );
					}

					// �ʵ� �߰� ���� ���ʽ� ������ - �ʵ� �߰� ���� ť��
					std::map< UidType, bool >::iterator mitBonus = mapBonusItem.find( mitUserList->second->GetCID() );
					if( mitBonus != mapBonusItem.end() )
					{
						if( mitBonus->second == true )				
						{
							// ���������� �⿩���� ���� 30%�� �Ǹ� �ΰ� ��
							for( int iCount = 0 ; iCount < kMiddleBossDropInfo.m_iMiddleBossBonusItemIDCount_High ; ++iCount )
							{
								if( kMiddleBossDropInfo.m_iMiddleBossBonusItemID <= 0 )
									break;

								sDropDataNpc.m_vecItemID.push_back( kMiddleBossDropInfo.m_iMiddleBossBonusItemID ); // �ʵ� �߰� ���� ť��
							}
						}
						else
						{
							// ���������� �⿩���� ���� ���� 30%�� �ȵǸ� �Ѱ� ��
							for( int iCount = 0 ; iCount < kMiddleBossDropInfo.m_iMiddleBossBonusItemIDCount_Low ; ++iCount )
							{
								if( kMiddleBossDropInfo.m_iMiddleBossBonusItemID <= 0 )
									break;

								sDropDataNpc.m_vecItemID.push_back( kMiddleBossDropInfo.m_iMiddleBossBonusItemID ); // �ʵ� �߰� ���� ť��
							}
						}
					}
				}
				// �������� ������ �翬 ����
				//////////////////////////////////////////////////////////////////////////
			}
		
			//////////////////////////////////////////////////////////////////////////
			// ����ġ�� �ִ� ��쿣 ������ �Ҹ� ��Ų��.

			//////////////////////////////////////////////////////////////////////////		
			// �ʵ��� �������� ����ġ ����(���ʽ� ����)
			m_spUserManager->AddBattleFieldRewardEXP( mitUserList->first, fLevelFactor, sDropDataNpc.m_iExp, kNpcDietNot );

			// �ѹ��� �÷��־�� �Ѵ�.
			if( mitUserList->second->GetCID() == iSendUnitUID )
			{
				IncreasePartyFever( kPacket_.m_mapDamageByUser ); // �ش� ���͸� ���� �������� ���嵵�� ���� ��Ƽ �ǹ� ����!
			}
			//////////////////////////////////////////////////////////////////////////	
			SendToGSCharacter( mitUserList->second->GetGSUID(), mitUserList->second->GetCID(), ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot );

			// ����� ���� �ʴ� npc��� ����� ���� �ʴ´�.
			if( kDieNpcInfo.m_bNoDrop == true )
				continue;

			// ��ӻ��°� �ƴ�ä�� ����npc��� ������ ����� ���� �ʴ´�.
			if( bDropDieState == false )
				continue;

			// ���� �⿩�� * ��Ƽ ���ʽ�
			//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
			float fTotalDropRate = mitUserContribution->second * fFieldEXPFactor;// * fPartyDropBonus ;
#else
			float fTotalDropRate = mitUserContribution->second * fFieldFactor;// * fPartyDropBonus ;
#endif SERV_FIELD_ED_EXP_FACTOR
			//}

			//////////////////////////////////////////////////////////////////////////
			// 2. ������ �������� ��� �������� �����Ǵ� ���� ���
			KDropTable::DROP_DATA sDropDataStatic;	
			{
				if( SiKDropTable()->BattleFieldStaticDropItem( GetBattleFieldID(), sDropDataStatic, fTotalDropRate ) == false )
				{
					START_LOG( cerr, L"Drop Item Data ������ ���� ===" )
						<< BUILD_LOG( GetBattleFieldID() )
						<< END_LOG;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			// 4. �Ӽ� ���� ��� ������
			KDropTable::DROP_DATA sDropDataAttribNpc;

			if( bIsAttribNpc == true )
			{
				//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
				if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(CXSLDungeon::DL_EXPERT), static_cast<int>(kDieNpcInfo.m_cLevel), fTotalDropRate, m_fItemDropEventProbRate, sDropDataAttribNpc.m_vecItemID ) == false )
#else // SERV_DROP_EVENT_RENEWAL
				if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(CXSLDungeon::DL_EXPERT), static_cast<int>(kDieNpcInfo.m_cLevel), fTotalDropRate, m_iItemDropEventProbCount, sDropDataAttribNpc.m_vecItemID ) == false )
#endif // SERV_DROP_EVENT_RENEWAL
				{
					START_LOG( cerr, L"�Ӽ� ���� ������ ��� ����!" )
						<< BUILD_LOG( CXSLDungeon::DL_EXPERT )
						<< END_LOG;
				}

				sDropDataNpc.m_nGPNum = 3 * sDropDataNpc.m_nGPNum;
				

				if( SiKDropTable()->AttribNpcSpecialDrop( CXSLDungeon::DL_EXPERT, fTotalDropRate, sDropDataAttribNpc.m_vecItemID ) == false )
				{
					START_LOG( cerr, L"�Ӽ� ���� ����� ������ ��� ����!" )
						<< BUILD_LOG( CXSLDungeon::DL_EXPERT )
						<< END_LOG;
				}
			}	


			//////////////////////////////////////////////////////////////////////////
			// 5. ����Ʈ ��� ������ ó��
			{
				std::vector< KRoomUser::QUEST_ITEM_DROP_INFO > vecDropInfo;

				//���ֺ� ����Ʈ ��� �������� �ִ��� �˻�..
				//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
				if( m_spUserManager->GetQuestDropItem( mitUserList->second->GetCID(), GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo, m_fQuestItemDropEventProbRate ) == true )
#else
				if( m_spUserManager->GetQuestDropItem( mitUserList->second->GetCID(), GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo ) == true )
#endif SERV_QUEST_ITEM_DROP_EVENT
				//}
				{
					//�ִٸ� ���ֺ��� ��Ǫ�� ���鼭..
					BOOST_TEST_FOREACH( const KRoomUser::QUEST_ITEM_DROP_INFO&, kDropInfo, vecDropInfo )
					{
						//��ӵ� ������ ����ŭ ó���� ����..
						BOOST_TEST_FOREACH( const int, iDropQuestItemID, kDropInfo.m_vecDropQuestItem )
						{
							KEGS_GET_ITEM_NOT kDropNot;
							kDropNot.m_cGetItemType = KEGS_GET_ITEM_NOT::GIT_QUEST_ITEM;
							kDropNot.m_GetUnitUID	= kDropInfo.m_iUnitUID;
							kDropNot.m_iItemID		= iDropQuestItemID;
							//{{ 2009. 7. 9  ������		�ӽ� �κ��丮
							kDropNot.m_iDropItemUID = m_nDropItemUID++;
							//}}
							SendToGSCharacter( kDropInfo.m_iGSUID, kDropInfo.m_iUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kDropNot );
						}
					}
				}
				else
				{
					if( NetError::GetLastError() != NetError::NET_OK )
					{
						START_LOG( cerr, L"����Ʈ ��� ������ �޾ƿ��� ����.!" )
							<< BUILD_LOG( kPacket_.m_uiAttUnit )
							<< BUILD_LOG( m_iDungeonID )
							<< BUILD_LOG( NetError::GetLastErrMsg() )
							<< END_LOG;
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// ������ ������ ��� ������ ����Ʈ�� ���� �������� ������ ���� ó��
			KEGS_DROP_ITEM_NOT kDropItemNot;
			kDropItemNot.m_CreatePos = kPacket_.m_DiePos;

			if( mitUserList->second->ExistActiveBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO ) == true )
			{
				//{{ 2012. 11. 20	������		�޸� ���� ���� ���� ���� ( ��Ʋ�ʵ� ) 
				if( SiKDropTable()->GetComeBackBuffDropCount( m_spUserManager->GetSamePartyMember( mitUserList->second->GetCID() ) ) == true )
				{
					// ��� ���̺��� Ȯ���ؼ� 'Special Drop'������� ��ü�ҿ���
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( 127260, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );		// ���ο� ������ ¡ǥ
				}	
			}

			//1.ED ���ó��
			if( sDropDataNpc.m_iED != -1 )
			{
				for( int i = 0; i < sDropDataNpc.m_nGPNum; ++i )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( sDropDataNpc.m_iGPItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution, true, sDropDataNpc.m_iED ) );
				}
			}

			//2.NPC ITEM ���ó��	
			if( sDropDataNpc.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataNpc.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}

			//{{ 2009. 4. 22  ������	��ü���
			if( sDropDataStatic.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataStatic.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}
			//}}

			//4.ATTRIB NPC ITEM ���ó��
			if( kDieNpcInfo.m_bActive == true && sDropDataAttribNpc.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataAttribNpc.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}

			//���ó���� �������� ������ ��Ŷ�� ������.
			if( kDropItemNot.m_DropItemDataList.empty() == false )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mitUserContribution->first );
				if( spRoomUser == NULL )
					continue;

				if( spRoomUser->IsBattleFieldNpcLoad() == false )
					continue;

				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_DROP_ITEM_NOT, kDropItemNot );
                vecDropItem_ = kDropItemNot.m_DropItemDataList;
			}
		}

		//{{ 2012. 12. 26  Ư�� �ð� ���� ��� �� ������ ��� �̺�Ʈ(�ʵ�����) - ��μ�
		//////////////////////////////////////////////////////////////////////////
		// Ư�� �ð� ������ ��� �̺�Ʈ
		if( kDieNpcInfo.m_bActive == true )
		{
			//////////////////////////////////////////////////////////////////////////
			// Ư�� �ð��� NPC���������� �����ִ� ���
			if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_BY_USER )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( kPacket_.m_uiAttUnit );
				if( spRoomUser != NULL )
				{
					int iTimeDropItemID = 0;
					int iTimeDropID = 0;
					if( SiKDropTable()->CheckTimeDrop( iTimeDropID, iTimeDropItemID ) )
					{
						const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iTimeDropItemID );
						if( pItemTemplet != NULL )
						{
							// �ϴ� DB�� ���� �� ������ ���� �޾Ƶ� �Ǵ� �������� Ȯ������!
							KDBE_CHECK_TIME_DROP_RESULT_REQ kGetTimeDrop;
							kGetTimeDrop.m_iGSUID		 = spRoomUser->GetGSUID();
							kGetTimeDrop.m_iUserUID		 = spRoomUser->GetUserUID();
							kGetTimeDrop.m_iUnitUID		 = spRoomUser->GetCID();
							kGetTimeDrop.m_wstrNickName	 = spRoomUser->GetNickName();
							kGetTimeDrop.m_iTimeDropID	 = iTimeDropID;
							kGetTimeDrop.m_iRewardItemID = iTimeDropItemID;
							kGetTimeDrop.m_wstrRewardItemName = pItemTemplet->m_Name;
							kGetTimeDrop.m_iDropItemUID	 = m_nDropItemUID++;
							SendToLogDB( DBE_CHECK_TIME_DROP_RESULT_REQ, kGetTimeDrop );
						}
						else
						{
							START_LOG( cerr, L"�������� �ʴ� itemid�̴�. ���ÿ����ΰ�?" )
								<< BUILD_LOG( iTimeDropItemID )
								<< END_LOG;
						}
					}
				}
				else
				{
					START_LOG( cwarn, L"�̺�Ʈ ������ ȹ�� ó�� �Ϸ��µ� ���������� ����!" )
						<< BUILD_LOG( kPacket_.m_uiAttUnit )
						<< END_LOG;
				}
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}
}

void KBattleFieldRoom::BattleFieldNormalDropProcess( const IN KEGS_NPC_UNIT_DIE_REQ& kPacket_, IN KRoomMonsterManager::NPC_DATA& kDieNpcInfo, IN const UidType& iSendUnitUID )
{
	const CXSLUnitManager::NPCUnitTemplet* pNPCTemplet = SiCXSLUnitManager()->GetNPCUnitTemplet( (CXSLUnitManager::NPC_UNIT_ID)kDieNpcInfo.m_iNPCID );
	if( pNPCTemplet == NULL )
	{
		START_LOG( cwarn, L"NPC Unit Templet �б� ����.!" )
			<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
			<< END_LOG;
		return;
	}

	// �Ӽ� �������� üũ
	const bool bIsAttribNpc = m_kMonsterManager.IsAttribNpc( kPacket_.m_nDieNPCUID );
	const int iAttibAcount = m_kMonsterManager.GetAttribNpcAttribCount( kPacket_.m_nDieNPCUID );

	//{{ 2009. 5. 11  ������	npc��������
	const bool bDropDieState = IsDropDieState( kPacket_.m_cNpcDieState );
	const bool bAddRoomUserKillCount = ( bDropDieState  &&  kPacket_.m_cNpcDieState != KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_SELF );
	//}}

	// ���� Ÿ�� ��� ���
	const float fMonsterTypeFactor = static_cast<float>( CXSLUnitManager::GetBattleFieldMonsterTypeFactor( iAttibAcount, kDieNpcInfo.m_cMonsterGrade ) );

	// �ʵ� ��� ���
	//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
	const float fFieldEDFactor = SiCXSLBattleFieldManager()->GetBattleFieldEDFactor() * fMonsterTypeFactor;
	const float fFieldEXPFactor = SiCXSLBattleFieldManager()->GetBattleFieldEXPFactor() * fMonsterTypeFactor;
#else
	const float fFieldFactor = SiCXSLBattleFieldManager()->GetBattleFieldFactor() * fMonsterTypeFactor;
#endif SERV_FIELD_ED_EXP_FACTOR
	//}

	//////////////////////////////////////////////////////////////////////////
	// uiAttUnit���� -1�̸� ������ ������ ���� �ʰ� npc�� �ڻ��Ѱ���. [��:��������]

	// �÷��̾ ���͸� ���ΰŶ�� �ش� �÷��̾��� ų���ͼ� üũ
	if( pNPCTemplet->m_bIsNPC  &&  bAddRoomUserKillCount == true  &&  kPacket_.m_uiAttUnit != -1 )
	{
		if( m_spUserManager->AddKillNPC( kPacket_.m_uiAttUnit ) == false )
		{
			if( m_kMonsterManager.IsExistMonster( static_cast<int>(kPacket_.m_uiAttUnit) ) == true )
			{
				START_LOG( clog, L"������ �׿���!!" )
					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
					<< BUILD_LOG( kPacket_.m_uiAttUnit );

				// ���Ͱ� ������ �ɷ� �״°���� : �������� �״�� ���ó����.
			}
		}
	}	

	//////////////////////////////////////////////////////////////////////////
	// 1. ���� ������ ���� ��� �������� �����Ǵ� ���� ���
	if( kDieNpcInfo.m_bNoDrop == false  &&  bDropDieState == true )
	{
		//////////////////////////////////////////////////////////////////////////
		// �� �������� �ٸ� ������ ��Ŷ�� ������ �ϹǷ� �̷��� ó���Ѵ�.
		KERM_NPC_UNIT_DIE_NOT kNpcDietNot;
		kNpcDietNot.m_iDungeonID		 = m_iDungeonID;
		kNpcDietNot.m_cDifficulty		 = m_cDifficultyLevel;
		kNpcDietNot.m_nDieNPCUID		 = kPacket_.m_nDieNPCUID;
		kNpcDietNot.m_iNPCID			 = kDieNpcInfo.m_iNPCID;
		kNpcDietNot.m_cAttUnitType		 = kPacket_.m_cAttUnitType;
		kNpcDietNot.m_uiAttUnit			 = kPacket_.m_uiAttUnit;
		kNpcDietNot.m_bNoDrop			 = kDieNpcInfo.m_bNoDrop;
		kNpcDietNot.m_cDungeonMode		 = m_cDungeonMode;
		kNpcDietNot.m_sSubStageExpNpcNum = m_kMonsterManager.GetAtStartedMonsterCount();
		kNpcDietNot.m_cUserCount		 = 0;
		kNpcDietNot.m_bDecreaseEndurance = false;
		kNpcDietNot.m_bQuestComplete	 = false;			// ��Ƽ�� �����Ͽ� �� �������� npc hp �� 50% �̻��̸� ����Ʈ �Ϸ� ����

		// ���� ������ ���� �� ��� �Ǿ�� �� ����
		// 1. �⿩�� (X�� ǥ��)
		std::map< UidType, float >	mapUserContribution;
		int iNpcMaxHP = kPacket_.m_iNpcMaxHP;
		std::map< UidType, float >::const_iterator mitDamageByUser = kPacket_.m_mapDamageByUser.begin();
		for( ; mitDamageByUser != kPacket_.m_mapDamageByUser.end() ; ++mitDamageByUser )
		{
			std::vector<UidType> vecPartyUserList;
			const int iPartyMemberCount = m_spUserManager->GetSamePartyMember( mitDamageByUser->first );

			float fContribution 
				= m_spUserManager->GetPartyTotalDamage( mitDamageByUser->first, kPacket_.m_mapDamageByUser ,vecPartyUserList ) 
				/ (static_cast<float>(iNpcMaxHP));

			if( fContribution > 1.5f )
				fContribution = 1.5f;

			 fContribution /= iPartyMemberCount;

			// �ַ� �÷��̸� ��Ƽ�� ������ ����� ���� ���� �ִ�. �׷��� �ѹ� ����� �Ѵ�.
			mapUserContribution.insert( std::make_pair( mitDamageByUser->first, fContribution ) );

			// ��Ƽ ���� ������ �⿩���� �����Ѵ�.
			BOOST_TEST_FOREACH( UidType, iPartyUnitUID, vecPartyUserList )
			{
				mapUserContribution.insert( std::make_pair( iPartyUnitUID, fContribution ) );
			}
		}

		// 2. ������ ��� ���� ���
		KDropTable::KBattleFieldNpcDropInfo kDropInfo_Init;
		if( SiKDropTable()->GetBattleFieldNpcDropInfo( GetBattleFieldID(), kDieNpcInfo.m_iNPCID, kDropInfo_Init ) == false )
		{
			//////////////////////////////////////////////////////////////////////////
			// ���� ���� ���� �ϱ� ������ ������ �ʱ�ȭ�� �ʿ��ϴ�.
			//����� �ο����� �˷��� ������ ���� ����..
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// ��Ƽ�� �����Ͽ� �� �������� npc hp �� 50% �̻��̸� ����Ʈ �Ϸ� ����
			kNpcDietNot.m_EXPList.clear();
			//////////////////////////////////////////////////////////////////////////

			START_LOG( cerr, L"��Ʋ �ʵ� npc drop info ������ ����" )
				<< BUILD_LOG( GetBattleFieldID() )
				<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
				<< END_LOG;

			BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
			return;
		}

		// 3. �⿩���� �ִ� �������� EXP, ED, DropItem ���
		std::map< UidType, KRoomUserPtr > mapRoomUserList;
		if( m_spUserManager->GetRoomUserList( mapRoomUserList ) == false )
		{
			//////////////////////////////////////////////////////////////////////////
			// ���� ���� ���� �ϱ� ������ ������ �ʱ�ȭ�� �ʿ��ϴ�.
			//����� �ο����� �˷��� ������ ���� ����..
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// ��Ƽ�� �����Ͽ� �� �������� npc hp �� 50% �̻��̸� ����Ʈ �Ϸ� ����
			kNpcDietNot.m_EXPList.clear();
			//////////////////////////////////////////////////////////////////////////

			START_LOG( cerr, L"��������  ����Ʈ�� ã�� ���� ����." )
				<< END_LOG;

			//////////////////////////////////////////////////////////////////////////
			// �ʵ� �������� �ش� npc�� �����Ϳ� ���� ������ ��ε� ĳ���� �Ѵ�! ( �Ϻ����� ���, ����ġ ���� )
			BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
			return;
		}

		std::map< UidType, KRoomUserPtr >::iterator mitUserList = mapRoomUserList.begin();
		for(  ; mitUserList != mapRoomUserList.end() ; ++mitUserList )
		{
			KDropTable::KBattleFieldNpcDropInfo kDropInfo;
			kDropInfo = kDropInfo_Init;

			if( mitUserList->second == NULL )
				continue;

			//////////////////////////////////////////////////////////////////////////
			// ���� ���� ���� �ϱ� ������ ������ �ʱ�ȭ�� �ʿ��ϴ�.
			//����� �ο����� �˷��� ������ ���� ����..
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// ��Ƽ�� �����Ͽ� �� �������� npc hp �� 50% �̻��̸� ����Ʈ �Ϸ� ����
			kNpcDietNot.m_EXPList.clear();
			//////////////////////////////////////////////////////////////////////////

			if( mitUserList->second->IsBattleFieldNpcLoad() == false )
				continue;

			// �ش� ������ ���͸� ���϶� ��ŭ ������ �ߴ��� �˻�����!
			std::map< UidType, float >::iterator mitUserContribution = mapUserContribution.find( mitUserList->first );
			if( mitUserContribution == mapUserContribution.end() )
			{
				// ���嵵�� ������ ���� ���� �ڰ��� ����!
				SendToGSCharacter( mitUserList->second->GetGSUID(), mitUserList->second->GetCID(), ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot );
				continue;
			}

			KDropTable::DROP_DATA sDropDataNpc;

			// ���� ���� �ʵ忡 �ִ� ��Ƽ�� ��
			kNpcDietNot.m_cUserCount	= static_cast<char>( m_spUserManager->GetSamePartyMember( mitUserList->first ) );

			// ���� ����� ���
			const float fLevelFactor = CXSLDungeon::GetMonsterLevelBalanceRate( m_spUserManager->GetUserLevel( mitUserList->first ), kDieNpcInfo.m_cLevel );

			// ��Ƽ �÷����� ��� ������ Ȯ�� ����
			const float fPartyDropBonus = m_spUserManager->GetBattleFieldPartyBonusRate( mitUserList->first ) + 1.0f;	

			// ��Ƽ�� �⿩���� 50% �̻��ΰ�?
			if( mitUserContribution->second * kNpcDietNot.m_cUserCount >= 0.5f )
			{
				kNpcDietNot.m_bQuestComplete = true;
			}

			// ���������� ���/���� �Ǿ�� �ϴ� ǰ��
			// 1. ED
			// 2. EXP
			// 3. ������
			if( kDropInfo.m_iExp > 0 )
			{
				kDropInfo.m_iExp = static_cast<int>(SiKDropTable()->GetNpcExp( kDieNpcInfo.m_cLevel ));

				// ����ġ�� �ִ� ���Ͷ�� �������� ���� �ȴ�.
				kNpcDietNot.m_bDecreaseEndurance = true;
			}

			if( kDropInfo.m_iED > 0 )
			{
				kDropInfo.m_iED = (10 + ( kDropInfo.m_iED * kDieNpcInfo.m_cLevel ));
			}

			if( SiKDropTable()->BattleFieldNpcDropItem( kDropInfo
				//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
													  , fFieldEDFactor
													  , fFieldEXPFactor
#else
													  , fFieldFactor
#endif SERV_FIELD_ED_EXP_FACTOR
													  //}
													  , fLevelFactor
													  , mitUserContribution->second
													  , kNpcDietNot.m_cUserCount
													  , fPartyDropBonus
													  //{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
													  , m_fItemDropEventProbRate
#else // SERV_DROP_EVENT_RENEWAL
													  , m_iItemDropEventProbCount
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
													  //}}
													  , kDieNpcInfo.m_bIsBoss
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-14	// �ڼ���
													  , 1.0f
#endif // SERV_BATTLE_FIELD_BOSS
													  , sDropDataNpc ) == false )
			{
				START_LOG( cerr, L"Drop Item Data ������ ���� ===" )
					<< BUILD_LOG( GetBattleFieldID() )
					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
					<< END_LOG;
			}

			// �������� ���/���� �Ǿ�� �ϴ� ǰ��
			// 1. Ư�� ������ ���(ex. ��������)
			// ����ġ�� �ִ� ��쿣 ������ �Ҹ� ��Ų��.
			SiKDropTable()->NpcSpecialDropItem( kDieNpcInfo.m_iNPCID, kDieNpcInfo.m_cMonsterGrade, kPacket_.m_cNpcDieState, sDropDataNpc.m_vecItemID );

			//////////////////////////////////////////////////////////////////////////		
			// �ʵ��� �������� ����ġ ����(���ʽ� ����)
			m_spUserManager->AddBattleFieldRewardEXP( mitUserList->first, fLevelFactor, sDropDataNpc.m_iExp, kNpcDietNot );

			// �ѹ��� �÷��־�� �Ѵ�.
			if( mitUserList->second->GetCID() == iSendUnitUID )
			{
				IncreasePartyFever( kPacket_.m_mapDamageByUser ); // �ش� ���͸� ���� �������� ���嵵�� ���� ��Ƽ �ǹ� ����!
			}
			//////////////////////////////////////////////////////////////////////////	
			SendToGSCharacter( mitUserList->second->GetGSUID(), mitUserList->second->GetCID(), ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot );

			// ����� ���� �ʴ� npc��� ����� ���� �ʴ´�.
			if( kDieNpcInfo.m_bNoDrop == true )
				continue;

			// ��ӻ��°� �ƴ�ä�� ����npc��� ������ ����� ���� �ʴ´�.
			if( bDropDieState == false )
				continue;

			// ���� �⿩�� * ��Ƽ ���ʽ�
			//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
			float fTotalDropRate = mitUserContribution->second * fFieldEXPFactor;// * fPartyDropBonus ;
#else
			float fTotalDropRate = mitUserContribution->second * fFieldFactor;// * fPartyDropBonus ;
#endif SERV_FIELD_ED_EXP_FACTOR
			//}

			//////////////////////////////////////////////////////////////////////////
			// 2. ������ �������� ��� �������� �����Ǵ� ���� ���
			KDropTable::DROP_DATA sDropDataStatic;	
			{
				if( SiKDropTable()->BattleFieldStaticDropItem( GetBattleFieldID(), sDropDataStatic, fTotalDropRate ) == false )
				{
					START_LOG( cerr, L"Drop Item Data ������ ���� ===" )
						<< BUILD_LOG( GetBattleFieldID() )
						<< END_LOG;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			// 4. �Ӽ� ���� ��� ������
			KDropTable::DROP_DATA sDropDataAttribNpc;

			if( bIsAttribNpc == true )
			{
				//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
				if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(CXSLDungeon::DL_EXPERT), static_cast<int>(kDieNpcInfo.m_cLevel), fTotalDropRate, m_fItemDropEventProbRate, sDropDataAttribNpc.m_vecItemID ) == false )
#else // SERV_DROP_EVENT_RENEWAL
				if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(CXSLDungeon::DL_EXPERT), static_cast<int>(kDieNpcInfo.m_cLevel), fTotalDropRate, m_iItemDropEventProbCount, sDropDataAttribNpc.m_vecItemID ) == false )
#endif // SERV_DROP_EVENT_RENEWAL
				{
					START_LOG( cerr, L"�Ӽ� ���� ������ ��� ����!" )
						<< BUILD_LOG( CXSLDungeon::DL_EXPERT )
						<< END_LOG;
				}

				sDropDataNpc.m_nGPNum = 3 * sDropDataNpc.m_nGPNum;

				if( SiKDropTable()->AttribNpcSpecialDrop( CXSLDungeon::DL_EXPERT, fTotalDropRate, sDropDataAttribNpc.m_vecItemID ) == false )
				{
					START_LOG( cerr, L"�Ӽ� ���� ����� ������ ��� ����!" )
						<< BUILD_LOG( CXSLDungeon::DL_EXPERT )
						<< END_LOG;
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// 5. ����Ʈ ��� ������ ó��
			{
				std::vector< KRoomUser::QUEST_ITEM_DROP_INFO > vecDropInfo;

				//���ֺ� ����Ʈ ��� �������� �ִ��� �˻�..
				//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
				if( m_spUserManager->GetQuestDropItem( mitUserList->second->GetCID(), GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo, m_fQuestItemDropEventProbRate ) == true )
#else
				if( m_spUserManager->GetQuestDropItem( mitUserList->second->GetCID(), GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo ) == true )
#endif SERV_QUEST_ITEM_DROP_EVENT
				//}
				{
					//�ִٸ� ���ֺ��� ��Ǫ�� ���鼭..
					BOOST_TEST_FOREACH( const KRoomUser::QUEST_ITEM_DROP_INFO&, kDropInfo, vecDropInfo )
					{
						//��ӵ� ������ ����ŭ ó���� ����..
						BOOST_TEST_FOREACH( const int, iDropQuestItemID, kDropInfo.m_vecDropQuestItem )
						{
							KEGS_GET_ITEM_NOT kDropNot;
							kDropNot.m_cGetItemType = KEGS_GET_ITEM_NOT::GIT_QUEST_ITEM;
							kDropNot.m_GetUnitUID	= kDropInfo.m_iUnitUID;
							kDropNot.m_iItemID		= iDropQuestItemID;
							//{{ 2009. 7. 9  ������		�ӽ� �κ��丮
							kDropNot.m_iDropItemUID = m_nDropItemUID++;
							//}}
							SendToGSCharacter( kDropInfo.m_iGSUID, kDropInfo.m_iUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kDropNot );
						}
					}
				}
				else
				{
					if( NetError::GetLastError() != NetError::NET_OK )
					{
						START_LOG( cerr, L"����Ʈ ��� ������ �޾ƿ��� ����.!" )
							<< BUILD_LOG( kPacket_.m_uiAttUnit )
							<< BUILD_LOG( m_iDungeonID )
							<< BUILD_LOG( NetError::GetLastErrMsg() )
							<< END_LOG;
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// ������ ������ ��� ������ ����Ʈ�� ���� �������� ������ ���� ó��
			KEGS_DROP_ITEM_NOT kDropItemNot;
			kDropItemNot.m_CreatePos = kPacket_.m_DiePos;

			if( mitUserList->second->ExistActiveBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO ) == true )
			{
				//{{ 2012. 11. 20	������		�޸� ���� ���� ���� ���� ( ��Ʋ�ʵ� ) 
#ifdef SERV_COME_BACK_USER_REWARD_IN_BATTLE_FIELD_BUG_FIX
				if( SiKDropTable()->GetComeBackBuffDropCount( m_spUserManager->GetSamePartyMember( mitUserList->second->GetCID() ) ) == true )
#else
				if( SiKDropTable()->GetComeBackBuffDropCount( m_spUserManager->GetNumMember() ) == true )
#endif SERV_COME_BACK_USER_REWARD_IN_BATTLE_FIELD_BUG_FIX
					//}}
				{
					// ��� ���̺��� Ȯ���ؼ� 'Special Drop'������� ��ü�ҿ���
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( 127260, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );		// ���ο� ������ ¡ǥ
				}	
			}

			//1.ED ���ó��
			if( sDropDataNpc.m_iED != -1 )
			{
				for( int i = 0; i < sDropDataNpc.m_nGPNum; ++i )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( sDropDataNpc.m_iGPItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution, true, sDropDataNpc.m_iED ) );
				}
			}

			//2.NPC ITEM ���ó��	
			if( sDropDataNpc.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataNpc.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}

			//{{ 2009. 4. 22  ������	��ü���
			if( sDropDataStatic.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataStatic.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}
			//}}

			//4.ATTRIB NPC ITEM ���ó��
			if( kDieNpcInfo.m_bActive == true && sDropDataAttribNpc.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataAttribNpc.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}

			//���ó���� �������� ������ ��Ŷ�� ������.
			if( kDropItemNot.m_DropItemDataList.empty() == false )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mitUserContribution->first );
				if( spRoomUser == NULL )
					continue;

				if( spRoomUser->IsBattleFieldNpcLoad() == false )
					continue;

				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_DROP_ITEM_NOT, kDropItemNot );
			}
		}

		//{{ 2012. 12. 26  Ư�� �ð� ���� ��� �� ������ ��� �̺�Ʈ(�ʵ�����) - ��μ�
#ifdef SERV_TIME_MONSTER_DROP_EVENT_IN_FIELD
		//////////////////////////////////////////////////////////////////////////
		// Ư�� �ð� ������ ��� �̺�Ʈ
		if( kDieNpcInfo.m_bActive == true )
		{
			//////////////////////////////////////////////////////////////////////////
			// Ư�� �ð��� NPC���������� �����ִ� ���
			if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_BY_USER )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( kPacket_.m_uiAttUnit );
				if( spRoomUser != NULL )
				{
					int iTimeDropItemID = 0;
					int iTimeDropID = 0;
					if( SiKDropTable()->CheckTimeDrop( iTimeDropID, iTimeDropItemID ) )
					{
						const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iTimeDropItemID );
						if( pItemTemplet != NULL )
						{
							// �ϴ� DB�� ���� �� ������ ���� �޾Ƶ� �Ǵ� �������� Ȯ������!
							KDBE_CHECK_TIME_DROP_RESULT_REQ kGetTimeDrop;
							kGetTimeDrop.m_iGSUID		 = spRoomUser->GetGSUID();
							kGetTimeDrop.m_iUserUID		 = spRoomUser->GetUserUID();
							kGetTimeDrop.m_iUnitUID		 = spRoomUser->GetCID();
							kGetTimeDrop.m_wstrNickName	 = spRoomUser->GetNickName();
							kGetTimeDrop.m_iTimeDropID	 = iTimeDropID;
							kGetTimeDrop.m_iRewardItemID = iTimeDropItemID;
							kGetTimeDrop.m_wstrRewardItemName = pItemTemplet->m_Name;
							kGetTimeDrop.m_iDropItemUID	 = m_nDropItemUID++;
							SendToLogDB( DBE_CHECK_TIME_DROP_RESULT_REQ, kGetTimeDrop );
						}
						else
						{
							START_LOG( cerr, L"�������� �ʴ� itemid�̴�. ���ÿ����ΰ�?" )
								<< BUILD_LOG( iTimeDropItemID )
								<< END_LOG;
						}
					}
				}
				else
				{
					START_LOG( cwarn, L"�̺�Ʈ ������ ȹ�� ó�� �Ϸ��µ� ���������� ����!" )
						<< BUILD_LOG( kPacket_.m_uiAttUnit )
						<< END_LOG;
				}
			}
			//////////////////////////////////////////////////////////////////////////
		}
#endif SERV_TIME_MONSTER_DROP_EVENT_IN_FIELD
		//}}
	}
}

_IMPL_ON_FUNC( ERM_ADMIN_BATTLE_FIELD_INCREASE_DANGER_REQ, KEGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_REQ )
{
	KEGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK kPacket;

	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"�߸��� ���¿��� ��Ŷ�� ������." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_07;
		kPacket.m_iDanger = m_kGameManager.GetDangerousValue();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK, kPacket );
		return;
	}

	if( kPacket_.m_iDanger <= 0 )
	{
		kPacket_.m_iDanger = 1;
	}

	if( m_kMonsterManager.IsRemainMiddleBoss() == false )
	{
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-28	// �ڼ���
		IncreaseDangerousValue( kPacket_.m_iDanger, LAST_SENDER_UID );
#else // SERV_BATTLE_FIELD_BOSS
		m_kGameManager.IncreaseDangerousValue( kPacket_.m_iDanger );
#endif // SERV_BATTLE_FIELD_BOSS
	}

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iDanger = m_kGameManager.GetDangerousValue();

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ, KEGS_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"�߸��� ���¿��� ��Ŷ�� ������." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	KRoomUserPtr spUser = m_spUserManager->GetHostUser();
	if( spUser != NULL )
	{
		SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_BATTLE_FIELD_NPC_DAMAGED_MAP_ACK, kPacket_ );
	}
}

_IMPL_ON_FUNC( ERM_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT, KEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"�߸��� ���¿��� ��Ŷ�� ������." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	KRoomUserPtr spUser = m_spUserManager->GetUser( kPacket_.m_iUnitUID );
	if( spUser != NULL )
	{
		SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT, kPacket_ );
	}
}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
//}

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
IMPL_ON_FUNC( ERM_CALL_MY_LOVER_CHECK_ROOM_REQ )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"�߸��� ���¿��� ��Ŷ�� ������." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		KERM_CALL_MY_LOVER_CHECK_ROOM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_29;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_CHECK_ROOM_ACK, kPacket );
		return;
	}

	// ��� �� �� �ִ��� �ο� ��üũ
	if( m_spUserManager->GetNumMember() + 1 > m_spUserManager->GetMaxSlot() )
	{
		KERM_CALL_MY_LOVER_CHECK_ROOM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_38;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_CHECK_ROOM_ACK, kPacket );
		return;
	}
	
	// ���� ���� �ϴٰ� �Ѵٸ�
	KERM_CALL_MY_LOVER_CHECK_ROOM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_vPos = kPacket_.m_vPos;
	kPacket.m_LastTouchLineIndex = kPacket_.m_LastTouchLineIndex;
	kPacket.m_iLoverUnitUID = kPacket_.m_iLoverUnitUID;
	kPacket.m_cCallType = KEGS_CALL_MY_LOVER_ROOM_NOT::CT_OTHER_CHANNEL;
	GetBattleFieldRoomInfo( kPacket.m_kBTInfo );

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_CHECK_ROOM_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_CALL_MY_LOVER_REQ, KERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ )
{
	KERM_LEAVE_ROOM_REQ kPacket;
	kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_35;
	kPacket.m_bSendAck = false;
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket, ERM_LEAVE_BATTLE_FIELD_FOR_CALL_MY_LOVER_ACK );

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_BATTLE_FIELD_FOR_CALL_MY_LOVER_ACK, kPacket_ );
}

IMPL_ON_FUNC( ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ )
{
	KEGS_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT kPacket;
	kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
	kPacket.m_iCalledUserUID = kPacket_.m_iCalledUserUID;
	kPacket.m_iCalledGSUID = kPacket_.m_iCalledGSUID;

	if( GetStateID() != KRoomFSM::S_PLAY  &&  GetStateID() != KRoomFSM::S_WAIT )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_09;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	// ����� ã�� �Դ��� ����!
	if( GetBattleFieldID() != kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID )
	{
		START_LOG( cerr, L"�ٸ� ��Ʋ�ʵ忡 �����ߴ�! �� ���״�!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetBattleFieldID() )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_05;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	// ��Ʋ�ʵ忡 ���� ó�� �Ѵ�!
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-28	// �ڼ���
	if( EnterRoom( kPacket_.m_kRoomUserInfo, kPacket_.m_kBattleFieldJoinInfo, false ) == false )
#else // SERV_BATTLE_FIELD_BOSS
	if( m_spUserManager->EnterRoom( kPacket_.m_kRoomUserInfo, kPacket_.m_kBattleFieldJoinInfo, false ) == false )
#endif // SERV_BATTLE_FIELD_BOSS
	{
		START_LOG( cerr, L"�� ���� ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	if( m_spUserManager->SetReady( FIRST_SENDER_UID, true ) == false )
	{
		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_06;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	if( m_spUserManager->StartGame( FIRST_SENDER_UID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_06;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

#ifdef SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS// �۾���¥: 2013-06-10	// �ڼ���
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_BUFF_INFO_NOT );
#endif // SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS

	// �ش� ������ �ٷ� ���� ���� ó�� ����!
	if( m_spUserManager->StartPlay( kPacket_.m_kRoomUserInfo.m_nUnitUID ) == false )
	{
		START_LOG( cerr, L"���� ���� ó�� ����!" )
			<< BUILD_LOG( kPacket_.m_kRoomUserInfo.m_nUnitUID )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_06;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	// �� ���� ��ȭ.
	if( GetStateID() == KRoomFSM::S_WAIT )
	{
		StateTransition( KRoomFSM::I_TO_LOAD );
		StateTransition( KRoomFSM::I_TO_PLAY );
	}

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    m_spUserManager->ZombieAlert_CheckStart( kPacket_.m_kRoomUserInfo.m_nUnitUID );
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST


	// ������ �������� ack�� ������.
	kPacket.m_iOK = NetError::NET_OK;
#ifndef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	kPacket.m_iLastTouchIndex = kPacket_.m_kBattleFieldJoinInfo.m_iStartPosIndex;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	kPacket.m_vPos = kPacket_.m_vPos;
	GetRoomInfo( kPacket.m_RoomInfo );
	GetRoomSlotInfo( kPacket.m_vecSlot );
	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
	kPacket.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
	kPacket.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
	//{{ 2012. 11. 21	������		�����ڿ��� ĳ���� ���� ���� ������
	m_spUserManager->GetEnterCashShopUserList( kPacket.m_vecEnterCashShopUser );
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_ACK, kPacket );


	// ���� ��� �������� NOT�� ������.
	KEGS_JOIN_BATTLE_FIELD_NOT kPacketNot;
	//080425.hoons.������ ���º� ���������� �����ؼ� �����ش�.
	m_spUserManager->GetRoomSlotInfo( FIRST_SENDER_UID, kPacketNot.m_JoinSlot, KBattleFieldUserManager::UT_GAME );
	kPacketNot.m_StartPosIndex = kPacket_.m_kBattleFieldJoinInfo.m_iStartPosIndex;

	BroadCast( ERM_JOIN_BATTLE_FIELD_NOT, kPacketNot );

	// ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
	SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );

	//////////////////////////////////////////////////////////////////////////	
	// [����!!!] �ݵ�� �渮��Ʈ�� ���� ���� ���Ŀ� �� ��Ŷ�� ������ �Ѵ�! ������ ���͸� ���� ��Ŷ�� �ٽ� ���!
	if( kPacket_.m_kReturnToFieldInfo.m_iRoomUID != 0 )
	{
		KERM_COMPLETE_RETURN_TO_FIELD_NOT kPacketToGS;
		kPacketToGS.m_kReturnToFieldInfo = kPacket_.m_kReturnToFieldInfo;
		SendToGSServer( LAST_SENDER_UID, ERM_COMPLETE_RETURN_TO_FIELD_NOT, kPacketToGS );
	}
}

IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_WEDDING_HALL_REQ )
{
	KERM_LEAVE_ROOM_REQ kPacket;
	kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_36;
	kPacket.m_bSendAck = false;
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket, ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_WEDDING_HALL_ACK );

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_WEDDING_HALL_ACK, kPacket_ );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-09	// �ڼ���
_IMPL_ON_FUNC( ERM_LEAVE_ROOM_FOR_WORKINGS_BLOCK_REQ, KERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_ROOM_FOR_WORKINGS_BLOCK_ACK );
}
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-28	// �ڼ���
void KBattleFieldRoom::IncreaseDangerousValue( IN const int iIncreaseValue, IN const UidType iGSUID )
{
	int iDangerousValue = m_kGameManager.GetDangerousValue();
	
	iDangerousValue += iIncreaseValue * SiCXSLBattleFieldManager()->GetDangerousValueEventRate();

	if( ( 0 <= iDangerousValue ) && ( iDangerousValue < SiCXSLBattleFieldManager()->GetDangerousValueMax() ) )
	{
		m_kGameManager.UpdateDangerousValue( iDangerousValue );
	}
	else
	{
		m_kGameManager.ResetDangerousValue();

		// TotalDangerousValue�� ������ �ִ� ��Ʋ �ʵ��ΰ�?
		if( SiCXSLFieldBossData()->DoesFieldAffectTotalDangerousValue( GetBattleFieldID() ) == true )
		{
			// GlobalServer�� TotalDangerousValue �ø��� ����
			KERM_UPDATE_TOTAL_DANGEROUS_VALUE_NOT kPacket;
			kPacket.m_iBattleFieldID		= static_cast<int>( GetBattleFieldID() );
			kPacket.m_byteIncreasedValue	= 1;
			SendToGSServer( iGSUID, ERM_UPDATE_TOTAL_DANGEROUS_VALUE_NOT, kPacket );
		}
	}
}

void KBattleFieldRoom::OnNpcUnitDie( IN const int iPlayerCount
								   , IN const bool bIsAttribNpc
								   , IN const char cDifficultyLevel
								   , IN const char cMonsterGrade
								   , IN const bool bIncreaseDanger
								   , IN const UidType iGSUID
								   )
{
	// ���� ������ ���� Ÿ�� ������� ���Ѵ�.
	const int iMonsterTypeFactor = CXSLUnitManager::GetMonsterTypeFactor( bIsAttribNpc, cDifficultyLevel, cMonsterGrade );

	// ���赵�� ���� ��Ų��.
	if( bIncreaseDanger == true )
	{
		// �������� ���赵 ����
		const int iBeforeDangeroursValue = m_kGameManager.GetDangerousValue();

		IncreaseDangerousValue( iMonsterTypeFactor, iGSUID );

		// ��� �޽��� �̺�Ʈ üũ
		m_kGameManager.CheckReserveWarningEvent( iBeforeDangeroursValue );

		// ����Ʈ ���� ���� �̺�Ʈ üũ
		m_kGameManager.CheckReserveEliteMonsterDropEvent( iBeforeDangeroursValue );

		// �߰� ���� ���� �̺�Ʈ üũ
		m_kGameManager.CheckReserveMiddleBossDropEvent( iPlayerCount );

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
		m_kGameManager.CheckReserveEventBossDropEvent( iPlayerCount );
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

		// ���� ���� �̺�Ʈ üũ
		m_kGameManager.CheckReserveBossDropEvent( iPlayerCount );
	}
}

void KBattleFieldRoom::CheckFieldBossSystem( void )
{
	const __time64_t tCurrentTime = CTime::GetCurrentTime().GetTime();

	if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_CLOSE_PROCESS ) == true )
	{
		// ���� �ʵ� ���� ��Ż üũ
		if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_RETURN_TO_FIELD ) == false )
		{
			if( ( m_tFieldCloseTime + SiCXSLFieldBossData()->GetFieldBossEndGameFieldHoldingTime_S() ) <= tCurrentTime )
			{
				// �������� ���� �ʵ�� ��Ż�ϵ��� ��������
				SEnum::AddFlag( m_byteBossFieldState, SEnum::BFRS_RETURN_TO_FIELD );
				BroadCastID( ERM_BOSS_FIELD_RETURN_PROCESS_NOT );
			}
		}
		// ���� �ʵ� ���� ���� üũ
		else if( ( m_tFieldCloseTime + SiCXSLFieldBossData()->GetFieldBossEndGameFieldHoldingTimeForce_S() ) <= tCurrentTime )
		{
			std::map< UidType, UidType > mapUserList;
			m_spRoomUserManager->GetUnitUIDListNotPrepareForDefenceDungeon( mapUserList );
			HandleTimeOutUser( mapUserList );
			StateTransition( KRoomFSM::I_TO_CLOSE );
		}
	}
	else
	{
		// ���� �ð��� 20�� �̸��̶��, ���� �Ұ��� �����Ѵ�.
		if( ( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_INTRUDE_RESTRICTION ) == false )
			&& ( ( m_tFieldCloseTime - SiCXSLFieldBossData()->GetIntrudeRestrictionTime_S() ) < tCurrentTime )
			)
		{
			SEnum::AddFlag( m_byteBossFieldState, SEnum::BFRS_INTRUDE_RESTRICTION );
			SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );
		}

		if( m_tFieldCloseTime <= tCurrentTime )
		{
			// ���� ���μ��� �����ϵ��� �˸�
			_BossFieldCloseProcess( true );
		}
	}
}

void KBattleFieldRoom::InitBossFieldCreateInfo( IN const KBossFieldCreateInfo& kBossFieldCreateInfo )
{
	// ���� �ʵ� �α� ���� �ʱ�ȭ
	m_kBossFieldLog.clear();

	// ���� �ʵ� �� ���� ���� �ʱ�ȭ
	m_byteBossFieldState = 0;

	if( kBossFieldCreateInfo.m_bBossField == false )
		return;

	// ���� �ʵ� �� ���� ����
	SEnum::AddFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD );
	m_tFieldHoldingTime	= kBossFieldCreateInfo.m_tFieldHoldingTime;
	m_tFieldOpenTime	= CTime::GetCurrentTime().GetTime();
	m_tFieldCloseTime	= m_tFieldOpenTime + m_tFieldHoldingTime;

	// �α� ������ ����
	m_kBossFieldLog.m_kFieldLog.m_iBossFieldID	= kBossFieldCreateInfo.m_iBossFieldID;
	m_kBossFieldLog.m_kFieldLog.m_iPortalMapID	= kBossFieldCreateInfo.m_iPortalMapID;

	CTime tPortalOpenTime( kBossFieldCreateInfo.m_tPortalOpenTime );
	m_kBossFieldLog.m_kFieldLog.m_wstrPortalOpenTime = tPortalOpenTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
}

void KBattleFieldRoom::BattleFieldBossDropProcess( const IN KEGS_NPC_UNIT_DIE_REQ& kPacket_, IN KRoomMonsterManager::NPC_DATA& kDieNpcInfo, IN const UidType& iSendUnitUID )
{
	const CXSLUnitManager::NPCUnitTemplet* pNPCTemplet = SiCXSLUnitManager()->GetNPCUnitTemplet( (CXSLUnitManager::NPC_UNIT_ID)kDieNpcInfo.m_iNPCID );
	if( pNPCTemplet == NULL )
	{
		START_LOG( cwarn, L"NPC Unit Templet �б� ����.!" )
			<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
			<< END_LOG;
		return;
	}

	// �Ӽ� �������� üũ
	const bool bIsAttribNpc = m_kMonsterManager.IsAttribNpc( kPacket_.m_nDieNPCUID );
	const int iAttibAcount = m_kMonsterManager.GetAttribNpcAttribCount( kPacket_.m_nDieNPCUID );

	// npc��������
	const bool bDropDieState = IsDropDieState( kPacket_.m_cNpcDieState );
	const bool bAddRoomUserKillCount = ( bDropDieState  &&  kPacket_.m_cNpcDieState != KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_SELF );

	// ���� Ÿ�� ��� ���
	const float fMonsterTypeFactor = static_cast<float>( CXSLUnitManager::GetBattleFieldMonsterTypeFactor( iAttibAcount, kDieNpcInfo.m_cMonsterGrade ) );

	// �ʵ� ��� ���
	//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
	const float fFieldEDFactor = SiCXSLBattleFieldManager()->GetBattleFieldEDFactor() * fMonsterTypeFactor;
	const float fFieldEXPFactor = SiCXSLBattleFieldManager()->GetBattleFieldEXPFactor() * fMonsterTypeFactor;
#else
	const float fFieldFactor = SiCXSLBattleFieldManager()->GetBattleFieldFactor() * fMonsterTypeFactor;
#endif SERV_FIELD_ED_EXP_FACTOR
	//}

	//////////////////////////////////////////////////////////////////////////
	// uiAttUnit���� -1�̸� ������ ������ ���� �ʰ� npc�� �ڻ��Ѱ���. [��:��������]
	// �÷��̾ ���͸� ���ΰŶ�� �ش� �÷��̾��� ų���ͼ� üũ
	if( pNPCTemplet->m_bIsNPC  &&  bAddRoomUserKillCount == true  &&  kPacket_.m_uiAttUnit != -1 )
	{
		if( m_spUserManager->AddKillNPC( kPacket_.m_uiAttUnit ) == false )
		{
			if( m_kMonsterManager.IsExistMonster( static_cast<int>(kPacket_.m_uiAttUnit) ) == true )
			{
				START_LOG( clog, L"������ �׿���!!" )
					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
					<< BUILD_LOG( kPacket_.m_uiAttUnit );

				// ���Ͱ� ������ �ɷ� �״°���� : �������� �״�� ���ó����.
			}
		}
	}	
	//////////////////////////////////////////////////////////////////////////
	// 1. ���� ������ ���� ��� �������� �����Ǵ� ���� ���
	if( ( kDieNpcInfo.m_bNoDrop == true ) || ( bDropDieState == false ) )
		return;

	//////////////////////////////////////////////////////////////////////////
	// �� �������� �ٸ� ������ ��Ŷ�� ������ �ϹǷ� �̷��� ó���Ѵ�.
	KERM_NPC_UNIT_DIE_NOT kNpcDietNot;
	kNpcDietNot.m_iDungeonID		 = m_iDungeonID;
	kNpcDietNot.m_cDifficulty		 = m_cDifficultyLevel;
	kNpcDietNot.m_nDieNPCUID		 = kPacket_.m_nDieNPCUID;
	kNpcDietNot.m_iNPCID			 = kDieNpcInfo.m_iNPCID;
	kNpcDietNot.m_cAttUnitType		 = kPacket_.m_cAttUnitType;
	kNpcDietNot.m_uiAttUnit			 = kPacket_.m_uiAttUnit;
	kNpcDietNot.m_bNoDrop			 = kDieNpcInfo.m_bNoDrop;
	kNpcDietNot.m_cDungeonMode		 = m_cDungeonMode;
	kNpcDietNot.m_sSubStageExpNpcNum = m_kMonsterManager.GetAtStartedMonsterCount();
	kNpcDietNot.m_cUserCount		 = 0;
	kNpcDietNot.m_bDecreaseEndurance = false;
	kNpcDietNot.m_bQuestComplete	 = false;			// ��Ƽ�� �����Ͽ� �� �������� npc hp �� 50% �̻��̸� ����Ʈ �Ϸ� ����

	// ���� ������ ���� �� ��� �Ǿ�� �� ����
	// 1. �⿩�� (X�� ǥ��)
	std::map< UidType, float > mapUserContribution;
	{
		const float fLimiteContribution = SiCXSLFieldBossData()->GetLimiteContribution();
		const float fMaximumContribution = SiCXSLFieldBossData()->GetMaximumContribution();

		for( std::map< UidType, float >::const_iterator mitDamageByUser = kPacket_.m_mapDamageByUser.begin(); mitDamageByUser != kPacket_.m_mapDamageByUser.end(); ++mitDamageByUser )
		{
			std::vector<UidType> vecPartyUserList;
			const int iPartyMemberCount = m_spUserManager->GetSamePartyMember( mitDamageByUser->first );

			float fContribution 
				= m_spUserManager->GetPartyTotalDamage( mitDamageByUser->first, kPacket_.m_mapDamageByUser ,vecPartyUserList ) 
				/ (static_cast<float>(kPacket_.m_iNpcMaxHP)) / iPartyMemberCount;

			if( fMaximumContribution < fContribution )
				fContribution = fMaximumContribution;

			if( fContribution < fLimiteContribution )
				continue;

			// �ַ� �÷��̸� ��Ƽ�� ������ ����� ���� ���� �ִ�. �׷��� �ѹ� ����� �Ѵ�.
			mapUserContribution.insert( std::make_pair( mitDamageByUser->first, fContribution ) );

			// ��Ƽ ���� ������ �⿩���� �����Ѵ�.
			BOOST_TEST_FOREACH( UidType, iPartyUnitUID, vecPartyUserList )
			{
				mapUserContribution.insert( std::make_pair( iPartyUnitUID, fContribution ) );
			}
		}
	}

	// ��Ƽ�� �⿩�� ���� �� ���� ����Ʈ
	std::multimap< float, std::set< UidType > > mmapPartyContribution;
	_MakePartyContributionData( mapUserContribution, mmapPartyContribution );

	// 2. ������ ��� ���� ���
	KDropTable::KBattleFieldNpcDropInfo kDropInfo_Init;
	if( SiKDropTable()->GetBattleFieldNpcDropInfo( GetBattleFieldID(), kDieNpcInfo.m_iNPCID, kDropInfo_Init ) == false )
	{
		//////////////////////////////////////////////////////////////////////////
		// ���� ���� ���� �ϱ� ������ ������ �ʱ�ȭ�� �ʿ��ϴ�.
		//����� �ο����� �˷��� ������ ���� ����..
		kNpcDietNot.m_cUserCount = 0;
		kNpcDietNot.m_bDecreaseEndurance = false;
		kNpcDietNot.m_bQuestComplete = false;			// ��Ƽ�� �����Ͽ� �� �������� npc hp �� 50% �̻��̸� ����Ʈ �Ϸ� ����
		kNpcDietNot.m_EXPList.clear();
		//////////////////////////////////////////////////////////////////////////

		START_LOG( cerr, L"��Ʋ �ʵ� npc drop info ������ ����" )
			<< BUILD_LOG( GetBattleFieldID() )
			<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
			<< END_LOG;

		BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
		return;
	}

	// 3. �⿩���� �ִ� �������� EXP, ED, DropItem ���
	std::map< UidType, KRoomUserPtr > mapRoomUserList;
	if( m_spUserManager->GetRoomUserList( mapRoomUserList ) == false )
	{
		//////////////////////////////////////////////////////////////////////////
		// ���� ���� ���� �ϱ� ������ ������ �ʱ�ȭ�� �ʿ��ϴ�.
		//����� �ο����� �˷��� ������ ���� ����..
		kNpcDietNot.m_cUserCount = 0;
		kNpcDietNot.m_bDecreaseEndurance = false;
		kNpcDietNot.m_bQuestComplete = false;			// ��Ƽ�� �����Ͽ� �� �������� npc hp �� 50% �̻��̸� ����Ʈ �Ϸ� ����
		kNpcDietNot.m_EXPList.clear();
		//////////////////////////////////////////////////////////////////////////

		START_LOG( cerr, L"��������  ����Ʈ�� ã�� ���� ����." )
			<< END_LOG;

		//////////////////////////////////////////////////////////////////////////
		// �ʵ� �������� �ش� npc�� �����Ϳ� ���� ������ ��ε� ĳ���� �Ѵ�! ( �Ϻ����� ���, ����ġ ���� )
		BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
		return;
	}

	std::map< UidType, KRoomUserPtr >::iterator mitUserList = mapRoomUserList.begin();
	for(  ; mitUserList != mapRoomUserList.end() ; ++mitUserList )
	{
		KDropTable::KBattleFieldNpcDropInfo kDropInfo;
		kDropInfo = kDropInfo_Init;

		if( mitUserList->second == NULL )
			continue;

		//////////////////////////////////////////////////////////////////////////
		// ���� ���� ���� �ϱ� ������ ������ �ʱ�ȭ�� �ʿ��ϴ�.
		//����� �ο����� �˷��� ������ ���� ����..
		kNpcDietNot.m_cUserCount = 0;
		kNpcDietNot.m_bDecreaseEndurance = false;
		kNpcDietNot.m_bQuestComplete = false;			// ��Ƽ�� �����Ͽ� �� �������� npc hp �� 50% �̻��̸� ����Ʈ �Ϸ� ����
		kNpcDietNot.m_EXPList.clear();
		//////////////////////////////////////////////////////////////////////////

		if( mitUserList->second->IsBattleFieldNpcLoad() == false )
			continue;

		// �ش� ������ ���͸� ���϶� ��ŭ ������ �ߴ��� �˻�����!
		std::map< UidType, float >::iterator mitUserContribution = mapUserContribution.find( mitUserList->first );
		if( mitUserContribution == mapUserContribution.end() )
		{
			// ���嵵�� ������ ���� ���� �ڰ��� ����!
			SendToGSCharacter( mitUserList->second->GetGSUID(), mitUserList->second->GetCID(), ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot );
			continue;
		}

		KDropTable::DROP_DATA sDropDataNpc;

		// ���� ���� �ʵ忡 �ִ� ��Ƽ�� ��
		kNpcDietNot.m_cUserCount	= static_cast<char>( m_spUserManager->GetSamePartyMember( mitUserList->first ) );

		const float fLevelFactor = 1.0f;

		// ��Ƽ �÷����� ��� ������ Ȯ�� ����
		const float fPartyDropBonus = m_spUserManager->GetBattleFieldPartyBonusRate( mitUserList->first ) + 1.0f;	

		// ��Ƽ�� �⿩���� 50% �̻��ΰ�?
		if( mitUserContribution->second * kNpcDietNot.m_cUserCount >= 0.002f )
		{
			kNpcDietNot.m_bQuestComplete = true;
		}

		// ���������� ���/���� �Ǿ�� �ϴ� ǰ��
		// 1. ED
		// 2. EXP
		// 3. ������
		if( kDropInfo.m_iExp > 0 )
		{
			kDropInfo.m_iExp = static_cast<int>(SiKDropTable()->GetNpcExp( kDieNpcInfo.m_cLevel ));

			// ����ġ�� �ִ� ���Ͷ�� �������� ���� �ȴ�.
			kNpcDietNot.m_bDecreaseEndurance = true;
		}

		if( kDropInfo.m_iED > 0 )
		{
			kDropInfo.m_iED = (10 + ( kDropInfo.m_iED * kDieNpcInfo.m_cLevel ));
			// �ش� �ʵ�� ��� ��� ED * 100
			kDropInfo.m_iED = kDropInfo.m_iED * 20;
		}

		// ��Ʋ �ʵ� ���� ��� �۾� ��û��: �̽¸�
		const float fBattleFieldBossFactor = 16.0f * powf( 1.1f, static_cast<float>( mitUserList->second->GetLevel() - 49 ) );

		if( SiKDropTable()->BattleFieldNpcDropItem( kDropInfo
			//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
												  , fFieldEDFactor
												  , fFieldEXPFactor
#else
												  , fFieldFactor
#endif SERV_FIELD_ED_EXP_FACTOR
			//}
												  , fLevelFactor
												  , mitUserContribution->second
												  , kNpcDietNot.m_cUserCount
												  , fPartyDropBonus
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
												  , m_fItemDropEventProbRate
#else // SERV_DROP_EVENT_RENEWAL
												  , m_iItemDropEventProbCount
#endif // SERV_DROP_EVENT_RENEWAL
												  , kDieNpcInfo.m_bIsBoss
												  , fBattleFieldBossFactor
												  , sDropDataNpc
												  ) == false )
		{
			START_LOG( cerr, L"Drop Item Data ������ ���� ===" )
				<< BUILD_LOG( GetBattleFieldID() )
				<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
				<< END_LOG;
		}

		//////////////////////////////////////////////////////////////////////////
		// �ʵ� �߰� ���� ���ʽ� ������ ���� ó��
		_BossRewardProcess( mitUserList->first, mmapPartyContribution, sDropDataNpc.m_vecItemID );

		//////////////////////////////////////////////////////////////////////////
		// ����ġ�� �ִ� ��쿣 ������ �Ҹ� ��Ų��.

		//////////////////////////////////////////////////////////////////////////		
		// �ʵ��� �������� ����ġ ����(���ʽ� ����)
		m_spUserManager->AddBattleFieldRewardEXP( mitUserList->first, fLevelFactor, sDropDataNpc.m_iExp, kNpcDietNot );

		// �ѹ��� �÷��־�� �Ѵ�.
		if( mitUserList->second->GetCID() == iSendUnitUID )
		{
			IncreasePartyFever( kPacket_.m_mapDamageByUser ); // �ش� ���͸� ���� �������� ���嵵�� ���� ��Ƽ �ǹ� ����!
		}
		//////////////////////////////////////////////////////////////////////////	

#ifdef SERV_BATTLE_FIELD_BOSS_QUEST_LOG// �۾���¥: 2013-11-28	// �ڼ���
		if( kNpcDietNot.m_bQuestComplete == false )
		{
			START_LOG( cerr, L"�ʵ� ���� �ý��� ����Ʈ �α�: �� ��ƾ���� ����Ʈ �Ϸ�� �������̾�� �Ѵ�." )
				<< BUILD_LOG( mitUserList->second->GetCID() )
				<< BUILD_LOG( mitUserContribution->second )
				<< END_LOG;
		}
#endif // SERV_BATTLE_FIELD_BOSS_QUEST_LOG

		SendToGSCharacter( mitUserList->second->GetGSUID(), mitUserList->second->GetCID(), ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot );

		// ����� ���� �ʴ� npc��� ����� ���� �ʴ´�.
		if( kDieNpcInfo.m_bNoDrop == true )
			continue;

		// ��ӻ��°� �ƴ�ä�� ����npc��� ������ ����� ���� �ʴ´�.
		if( bDropDieState == false )
			continue;

		// ���� �⿩�� * ��Ƽ ���ʽ�
		//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
		float fTotalDropRate = mitUserContribution->second * fFieldEXPFactor;// * fPartyDropBonus ;
#else
		float fTotalDropRate = mitUserContribution->second * fFieldFactor;// * fPartyDropBonus ;
#endif SERV_FIELD_ED_EXP_FACTOR
		//}

		//////////////////////////////////////////////////////////////////////////
		// 2. ������ �������� ��� �������� �����Ǵ� ���� ���
		KDropTable::DROP_DATA sDropDataStatic;	
		{
			if( SiKDropTable()->BattleFieldStaticDropItem( GetBattleFieldID(), sDropDataStatic, fTotalDropRate ) == false )
			{
				START_LOG( cerr, L"Drop Item Data ������ ���� ===" )
					<< BUILD_LOG( GetBattleFieldID() )
					<< END_LOG;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		// 4. �Ӽ� ���� ��� ������
		KDropTable::DROP_DATA sDropDataAttribNpc;

		if( bIsAttribNpc == true )
		{
			//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
			if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(CXSLDungeon::DL_EXPERT), static_cast<int>(kDieNpcInfo.m_cLevel), fTotalDropRate, m_fItemDropEventProbRate, sDropDataAttribNpc.m_vecItemID ) == false )
#else // SERV_DROP_EVENT_RENEWAL
			if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(CXSLDungeon::DL_EXPERT), static_cast<int>(kDieNpcInfo.m_cLevel), fTotalDropRate, m_iItemDropEventProbCount, sDropDataAttribNpc.m_vecItemID ) == false )
#endif // SERV_DROP_EVENT_RENEWAL
			{
				START_LOG( cerr, L"�Ӽ� ���� ������ ��� ����!" )
					<< BUILD_LOG( CXSLDungeon::DL_EXPERT )
					<< END_LOG;
			}

			sDropDataNpc.m_nGPNum = 3 * sDropDataNpc.m_nGPNum;


			if( SiKDropTable()->AttribNpcSpecialDrop( CXSLDungeon::DL_EXPERT, fTotalDropRate, sDropDataAttribNpc.m_vecItemID ) == false )
			{
				START_LOG( cerr, L"�Ӽ� ���� ����� ������ ��� ����!" )
					<< BUILD_LOG( CXSLDungeon::DL_EXPERT )
					<< END_LOG;
			}
		}	


		//////////////////////////////////////////////////////////////////////////
		// 5. ����Ʈ ��� ������ ó��
		{
			std::vector< KRoomUser::QUEST_ITEM_DROP_INFO > vecDropInfo;

			//���ֺ� ����Ʈ ��� �������� �ִ��� �˻�..
			//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
			if( m_spUserManager->GetQuestDropItem( mitUserList->second->GetCID(), GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo, m_fQuestItemDropEventProbRate ) == true )
#else
			if( m_spUserManager->GetQuestDropItem( mitUserList->second->GetCID(), GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo ) == true )
#endif SERV_QUEST_ITEM_DROP_EVENT
				//}
			{
				//�ִٸ� ���ֺ��� ��Ǫ�� ���鼭..
				BOOST_TEST_FOREACH( const KRoomUser::QUEST_ITEM_DROP_INFO&, kDropInfo, vecDropInfo )
				{
					//��ӵ� ������ ����ŭ ó���� ����..
					BOOST_TEST_FOREACH( const int, iDropQuestItemID, kDropInfo.m_vecDropQuestItem )
					{
						KEGS_GET_ITEM_NOT kDropNot;
						kDropNot.m_cGetItemType = KEGS_GET_ITEM_NOT::GIT_QUEST_ITEM;
						kDropNot.m_GetUnitUID	= kDropInfo.m_iUnitUID;
						kDropNot.m_iItemID		= iDropQuestItemID;
						//{{ 2009. 7. 9  ������		�ӽ� �κ��丮
						kDropNot.m_iDropItemUID = m_nDropItemUID++;
						//}}
						SendToGSCharacter( kDropInfo.m_iGSUID, kDropInfo.m_iUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kDropNot );
					}
				}
			}
			else
			{
				if( NetError::GetLastError() != NetError::NET_OK )
				{
					START_LOG( cerr, L"����Ʈ ��� ������ �޾ƿ��� ����.!" )
						<< BUILD_LOG( kPacket_.m_uiAttUnit )
						<< BUILD_LOG( m_iDungeonID )
						<< BUILD_LOG( NetError::GetLastErrMsg() )
						<< END_LOG;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// ������ ������ ��� ������ ����Ʈ�� ���� �������� ������ ���� ó��
		KEGS_DROP_ITEM_NOT kDropItemNot;
		kDropItemNot.m_CreatePos = kPacket_.m_DiePos;

		if( mitUserList->second->ExistActiveBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO ) == true )
		{
			//{{ 2012. 11. 20	������		�޸� ���� ���� ���� ���� ( ��Ʋ�ʵ� ) 
			if( SiKDropTable()->GetComeBackBuffDropCount( m_spUserManager->GetSamePartyMember( mitUserList->second->GetCID() ) ) == true )
			{
				// ��� ���̺��� Ȯ���ؼ� 'Special Drop'������� ��ü�ҿ���
				kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( 127260, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );		// ���ο� ������ ¡ǥ
			}	
		}

		//1.ED ���ó��
		if( sDropDataNpc.m_iED != -1 )
		{
			for( int i = 0; i < sDropDataNpc.m_nGPNum; ++i )
			{
				kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( sDropDataNpc.m_iGPItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution, true, sDropDataNpc.m_iED ) );
			}
		}

		//2.NPC ITEM ���ó��	
		if( sDropDataNpc.m_vecItemID.empty() == false )
		{
			BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataNpc.m_vecItemID )
			{
				kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
			}
		}

		//{{ 2009. 4. 22  ������	��ü���
		if( sDropDataStatic.m_vecItemID.empty() == false )
		{
			BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataStatic.m_vecItemID )
			{
				kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
			}
		}
		//}}

		//4.ATTRIB NPC ITEM ���ó��
		if( kDieNpcInfo.m_bActive == true && sDropDataAttribNpc.m_vecItemID.empty() == false )
		{
			BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataAttribNpc.m_vecItemID )
			{
				kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
			}
		}

		//���ó���� �������� ������ ��Ŷ�� ������.
		if( kDropItemNot.m_DropItemDataList.empty() == false )
		{
			KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mitUserContribution->first );
			if( spRoomUser == NULL )
				continue;

			if( spRoomUser->IsBattleFieldNpcLoad() == false )
				continue;

			SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_DROP_ITEM_NOT, kDropItemNot );
		}
	}

	////{{ 2012. 12. 26  Ư�� �ð� ���� ��� �� ������ ��� �̺�Ʈ(�ʵ�����) - ��μ�
	////////////////////////////////////////////////////////////////////////////
	//// Ư�� �ð� ������ ��� �̺�Ʈ
	//if( kDieNpcInfo.m_bActive == true )
	//{
	//	//////////////////////////////////////////////////////////////////////////
	//	// Ư�� �ð��� NPC���������� �����ִ� ���
	//	if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_BY_USER )
	//	{
	//		KRoomUserPtr spRoomUser = m_spUserManager->GetUser( kPacket_.m_uiAttUnit );
	//		if( spRoomUser != NULL )
	//		{
	//			int iTimeDropItemID = 0;
	//			int iTimeDropID = 0;
	//			if( SiKDropTable()->CheckTimeDrop( iTimeDropID, iTimeDropItemID ) )
	//			{
	//				const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iTimeDropItemID );
	//				if( pItemTemplet != NULL )
	//				{
	//					// �ϴ� DB�� ���� �� ������ ���� �޾Ƶ� �Ǵ� �������� Ȯ������!
	//					KDBE_CHECK_TIME_DROP_RESULT_REQ kGetTimeDrop;
	//					kGetTimeDrop.m_iGSUID		 = spRoomUser->GetGSUID();
	//					kGetTimeDrop.m_iUserUID		 = spRoomUser->GetUserUID();
	//					kGetTimeDrop.m_iUnitUID		 = spRoomUser->GetCID();
	//					kGetTimeDrop.m_wstrNickName	 = spRoomUser->GetNickName();
	//					kGetTimeDrop.m_iTimeDropID	 = iTimeDropID;
	//					kGetTimeDrop.m_iRewardItemID = iTimeDropItemID;
	//					kGetTimeDrop.m_wstrRewardItemName = pItemTemplet->m_Name;
	//					kGetTimeDrop.m_iDropItemUID	 = m_nDropItemUID++;
	//					SendToLogDB( DBE_CHECK_TIME_DROP_RESULT_REQ, kGetTimeDrop );
	//				}
	//				else
	//				{
	//					START_LOG( cerr, L"�������� �ʴ� itemid�̴�. ���ÿ����ΰ�?" )
	//						<< BUILD_LOG( iTimeDropItemID )
	//						<< END_LOG;
	//				}
	//			}
	//		}
	//		else
	//		{
	//			START_LOG( cwarn, L"�̺�Ʈ ������ ȹ�� ó�� �Ϸ��µ� ���������� ����!" )
	//				<< BUILD_LOG( kPacket_.m_uiAttUnit )
	//				<< END_LOG;
	//		}
	//	}
	//	//////////////////////////////////////////////////////////////////////////
	//}
}

void KBattleFieldRoom::_BossRewardProcess( IN const UidType iUnitUID, IN const std::multimap< float, std::set< UidType > > mmapPartyContribution, OUT std::vector<int>& vecItemID ) const
{
	if( mmapPartyContribution.empty() == true )
		return;

	size_t iRank = 1;
	std::multimap< float, std::set< UidType > >::const_reverse_iterator it;
	for( it = mmapPartyContribution.rbegin(); it != mmapPartyContribution.rend(); ++it )
	{
		const std::set<UidType>& setUnitList = it->second;
		
		if( setUnitList.find( iUnitUID ) != setUnitList.end() )
		{
			break;
		}

		++iRank;
	}

	if( it == mmapPartyContribution.rend() )
	{
		return;
	}

	std::map<int, byte> mapRewardData;
	SiCXSLFieldBossData()->GetPrecedenceRewardData( iRank, mmapPartyContribution.size(), mapRewardData );

	for( std::map<int, byte>::const_iterator it = mapRewardData.begin(); it != mapRewardData.end(); ++it )
	{
		int iItemID = it->first;
		byte byteNum = it->second;
		
		while( 0 < byteNum )
		{
			vecItemID.push_back( iItemID );
			--byteNum;
		}
	}

	if( 0 <= iRank )
	{
		// �α� ���� ������ const_cast�� �����. �� �Լ��� ������� �����ϰų� �ܺο��� ó���ص� ���� ����.
		const_cast<KBattleFieldRoom*>( this )->_MakeBossFieldUserLog( iUnitUID, static_cast<byte>( min( iRank, BYTE_MAX ) ) );
	}
}

int KBattleFieldRoom::EventMonsterDieProcess( IN const UidType iUnitUID, IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, OUT KERM_NPC_UNIT_DIE_ACK& kPacket, OUT char& cMonsterGrade )
{
	// ���Ͱ� ���� ����ִ��� üũ!
	if( m_kMonsterManager.IsEventMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
	{
		return NetError::ERR_ROOM_21;
	}

	KRoomMonsterManager::NPC_DATA kDieNpcInfo;

	// ���Ͱ� ���� ����ִٸ� ���� ���� ��û!
	if( m_kMonsterManager.GetEventNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
	{
		return NetError::ERR_ROOM_20;
	}

	// ���� ����ִٸ� ���λ���� ���������� ����..
	LIF( m_kMonsterManager.SetEventMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );

	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	BattleFieldNormalDropProcess( kPacket_, kDieNpcInfo, iUnitUID );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

	kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
	cMonsterGrade		= kDieNpcInfo.m_cMonsterGrade;

	return NetError::NET_OK;
}

int KBattleFieldRoom::MiddleBossMonsterDieProcess( IN const UidType iUnitUID, IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, OUT KERM_NPC_UNIT_DIE_ACK& kPacket, OUT char& cMonsterGrade )
{
	// ���Ͱ� ���� ����ִ��� üũ!
	if( m_kMonsterManager.IsMiddleBossMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
	{
		return NetError::ERR_ROOM_21;
	}

	KRoomMonsterManager::NPC_DATA kDieNpcInfo;

	// ���Ͱ� ���� ����ִٸ� ���� ���� ��û!
	if( m_kMonsterManager.GetMiddleBossNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
	{
		return NetError::ERR_ROOM_20;
	}

	// ���� ����ִٸ� ���λ���� ���������� ����..
	LIF( m_kMonsterManager.SetMiddleBossMonsterDie( kPacket_.m_nDieNPCUID ) );

	std::vector< KDropItemData > vecDropItem;
	vecDropItem.clear();
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	BattleFieldMiddleBossDropProcess( kPacket_, kDieNpcInfo, iUnitUID, vecDropItem );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

	kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
	cMonsterGrade		= kDieNpcInfo.m_cMonsterGrade;

	KFieldBossGenKillStat kLog;
	//kLog.m_nDieNPCUID = kPacket_.m_nDieNPCUID;
	kLog.m_nNumUser = kPacket_.m_mapDamageByUser.size();
	//kLog.m_nNumDropItem = vecDropItem.size();
	kLog.m_nSpawnID = kDieNpcInfo.m_iBossGroupID;
	kLog.m_nFieldID = GetBattleFieldID();
	SendToLogDB( ELOG_KILL_FIELD_MIDDLE_BOSS_STAT_NOT, kLog );

	START_LOG( cwarn, L"�ʵ� �߰� ���� ų ��� �׽�Ʈ " )
		<< BUILD_LOG( kLog.m_nFieldID )
		//<< BUILD_LOG( kLog.m_nDieNPCUID )
		<< BUILD_LOG( kLog.m_nNumUser )
		<< BUILD_LOG( kLog.m_nSpawnID )
		<< END_LOG;

	return NetError::NET_OK;
}

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
int KBattleFieldRoom::EventBossMonsterDieProcess( IN const UidType iUnitUID, IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, OUT KERM_NPC_UNIT_DIE_ACK& kPacket, OUT char& cMonsterGrade )
{
	// ���Ͱ� ���� ����ִ��� üũ!
	if( m_kMonsterManager.IsEventBossMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
	{
		return NetError::ERR_ROOM_21;
	}

	KRoomMonsterManager::NPC_DATA kDieNpcInfo;

	// ���Ͱ� ���� ����ִٸ� ���� ���� ��û!
	if( m_kMonsterManager.GetEventBossNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
	{
		return NetError::ERR_ROOM_20;
	}

	// ���� ����ִٸ� ���λ���� ���������� ����..
	LIF( m_kMonsterManager.SetEventBossMonsterDie( kPacket_.m_nDieNPCUID ) );

	std::vector< KDropItemData > vecDropItem;
	vecDropItem.clear();
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	BattleFieldMiddleBossDropProcess( kPacket_, kDieNpcInfo, iUnitUID, vecDropItem );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

	kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
	cMonsterGrade		= kDieNpcInfo.m_cMonsterGrade;

	return NetError::NET_OK;
}
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

int KBattleFieldRoom::MonsterDiePrcess( IN const UidType iUnitUID, IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, OUT KERM_NPC_UNIT_DIE_ACK& kPacket, OUT char& cMonsterGrade )
{
	// ���Ͱ� ���� ����ִ��� üũ!
	if( m_kMonsterManager.IsMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
	{
		return NetError::ERR_ROOM_21;
	}

	KRoomMonsterManager::NPC_DATA kDieNpcInfo;

	// ���Ͱ� ���� ����ִٸ� ���� ���� ��û!
	if( m_kMonsterManager.GetNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
	{
		return NetError::ERR_ROOM_20;
	}

	// ���� ����ִٸ� ���λ���� ���������� ����..
	LIF( m_kMonsterManager.SetMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );

	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	BattleFieldNormalDropProcess( kPacket_, kDieNpcInfo, iUnitUID );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

	kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
	cMonsterGrade		= kDieNpcInfo.m_cMonsterGrade;

	return NetError::NET_OK;
}

int KBattleFieldRoom::BossMonsterDieProcess( IN const UidType iUnitUID, IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, OUT KERM_NPC_UNIT_DIE_ACK& kPacket )
{
	// ���Ͱ� ���� ����ִ��� üũ!
	if( m_kMonsterManager.IsMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
	{
		return NetError::ERR_ROOM_21;
	}

	KRoomMonsterManager::NPC_DATA kDieNpcInfo;

	// ���Ͱ� ���� ����ִٸ� ���� ���� ��û!
	if( m_kMonsterManager.GetNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
	{
		return NetError::ERR_ROOM_20;
	}

	if( m_kMonsterManager.IsAliveBossMonster( kPacket_.m_nDieNPCUID ) == true )
	{
		if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_CLOSE_PROCESS ) == true )
		{
			return NetError::ERR_FIELD_BOSS_07;
		}

		// ���� ����ִٸ� ���λ���� ���������� ����..
		LIF( m_kMonsterManager.SetMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );

		BattleFieldBossDropProcess( kPacket_, kDieNpcInfo, iUnitUID );

		// ���� �ʵ� ���� �� üũ
		_BossFieldCloseProcess( false );
	}
	else
	{
		// ���� ����ִٸ� ���λ���� ���������� ����..
		LIF( m_kMonsterManager.SetMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );
	}

	kPacket.m_iNPCID = kDieNpcInfo.m_iNPCID;

	return NetError::NET_OK;
}

void KBattleFieldRoom::_MakePartyContributionData( IN const std::map<UidType, float>& mapUserContribution, OUT std::multimap< float, std::set< UidType > >& mmapPartyContribution ) const
{
	std::vector< std::set< UidType > > vecUnitListGroupdByParty;
	m_spUserManager->GetUnitListGroupdByParty( vecUnitListGroupdByParty );

	BOOST_TEST_FOREACH( const std::set<UidType>&, setUnitList, vecUnitListGroupdByParty )
	{
		if( setUnitList.empty() == true )
			continue;
			
		const UidType iUnitUID = *setUnitList.begin();

		std::map<UidType, float>::const_iterator it = mapUserContribution.find( iUnitUID );
		if( it == mapUserContribution.end() )
			continue;

		const float fContribution = it->second;
		mmapPartyContribution.insert( std::multimap< float, std::set< UidType > >::value_type( fContribution, setUnitList ) );
	}
}

void KBattleFieldRoom::_BossFieldCloseProcess( const bool bTimeOut )
{
	if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_INTRUDE_RESTRICTION ) == false )
	{
		// ���� �Ұ��� �����Ѵ�.
		SEnum::AddFlag( m_byteBossFieldState, SEnum::BFRS_INTRUDE_RESTRICTION );
		SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );
	}

	__time64_t tCurrentTime = CTime::GetCurrentTime().GetTime();

	// m_tFieldCloseTime �� �����Ͽ� ���� ���μ����� �������� �ð����� ó���ǵ��� ����.
	if( bTimeOut == false )
	{
		m_tFieldCloseTime = tCurrentTime;
	}

	// �α� ���� ����
#ifdef _CONVERT_VS_2010// �۾���¥: 2013-11-29	// �ڼ���
	m_kBossFieldLog.m_kFieldLog.m_iPlayTime = static_cast<int>( min( ( tCurrentTime - m_tFieldOpenTime ), INT_MAX ) );
#else // _CONVERT_VS_2010
	m_kBossFieldLog.m_kFieldLog.m_iPlayTime = static_cast<int>( min( ( tCurrentTime - m_tFieldOpenTime ), 2147483647 ) );
#endif // _CONVERT_VS_2010
	m_kBossFieldLog.m_kFieldLog.m_byteRemainUserCount = m_spUserManager->GetNumMember();
	m_kBossFieldLog.m_kFieldLog.m_bSucceed = !bTimeOut;

	// ���� ���μ��� �����ϵ��� �˸�
	SEnum::AddFlag( m_byteBossFieldState, SEnum::BFRS_CLOSE_PROCESS );
	BroadCastID( ERM_BOSS_FIELD_CLOSE_PROCESS_NOT );
}

bool KBattleFieldRoom::EnterRoom( IN const KRoomUserInfo& kInfo, IN const KBattleFieldJoinInfo& kJoinInfo, IN const bool bConsiderTeam )
{
	if( m_spUserManager->EnterRoom( kInfo, kJoinInfo, bConsiderTeam ) == false )
	{
		return false;
	}

	if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD ) == true )
	{
		// ���� �ʵ� ���� �α� ���� ����
		KDBE_BOSS_FIELD_LOG_NOT::TYPE_BOSS_FIELD_USER_LOG::iterator it = m_kBossFieldLog.m_mapUserLog.find( kInfo.m_nUnitUID );
		if( it == m_kBossFieldLog.m_mapUserLog.end() )
		{
			std::pair<KDBE_BOSS_FIELD_LOG_NOT::TYPE_BOSS_FIELD_USER_LOG::iterator, bool> pairResult = m_kBossFieldLog.m_mapUserLog.insert( KDBE_BOSS_FIELD_LOG_NOT::TYPE_BOSS_FIELD_USER_LOG::value_type( kInfo.m_nUnitUID, std::vector<KBossFieldUserLog>() ) );
			if( pairResult.second == false )
			{
				return true;
			}

			it = pairResult.first;
		}

		KBossFieldUserLog kBossFieldUserLog;
		kBossFieldUserLog.m_iUnitUID		= kInfo.m_nUnitUID;
		kBossFieldUserLog.m_byteLevel		= kInfo.m_ucLevel;
		kBossFieldUserLog.m_byteClass		= max( kInfo.m_cUnitClass, 0 );
		kBossFieldUserLog.m_wstrNickName	= kInfo.m_wstrNickName;
		it->second.push_back( kBossFieldUserLog );
	}

	return true;
}

void KBattleFieldRoom::_MakeBossFieldUserLogAtLeave( IN const UidType iUnitUID )
{
	KDBE_BOSS_FIELD_LOG_NOT::TYPE_BOSS_FIELD_USER_LOG::iterator it = m_kBossFieldLog.m_mapUserLog.find( iUnitUID );
	if( it == m_kBossFieldLog.m_mapUserLog.end() )
	{
		return;
	}

	if( it->second.empty() == true )
	{
		return;
	}

	KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		return;
	}

	KBossFieldUserLog& kBossFieldUserLog = it->second.back();
	kBossFieldUserLog.m_iEXP					= spRoomUser->GetRewardEXP() + spRoomUser->GetRewardPartyEXP();
	kBossFieldUserLog.m_iED						= spRoomUser->GetRewardED();
	kBossFieldUserLog.m_sNumResurrectionStone	= spRoomUser->GetUsedRessurectionStoneCount();

	if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_CLOSE_PROCESS ) == true )
	{
		// ������ �׾��ų� ��������.
		kBossFieldUserLog.m_byteCompletionType	= ( m_kBossFieldLog.m_kFieldLog.m_bSucceed == true ) ? 0 : 2;
	}
	else
	{
		// ������ �����Ѵ�.
		kBossFieldUserLog.m_byteCompletionType	= 1;
	}
}

void KBattleFieldRoom::_MakeBossFieldUserLog( IN const UidType iUnitUID, IN const byte byteContributionRank )
{
	KDBE_BOSS_FIELD_LOG_NOT::TYPE_BOSS_FIELD_USER_LOG::iterator it = m_kBossFieldLog.m_mapUserLog.find( iUnitUID );
	if( it == m_kBossFieldLog.m_mapUserLog.end() )
	{
		return;
	}

	if( it->second.empty() == true )
	{
		return;
	}

	KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		return;
	}

	KBossFieldUserLog& kBossFieldUserLog = it->second.back();
	kBossFieldUserLog.m_byteContributionRank = byteContributionRank;
}

IMPL_ON_FUNC_NOPARAM( ERM_BOSS_FIELD_INTRUDE_RESTRICTION_REQ )
{
	KERM_BOSS_FIELD_INTRUDE_RESTRICTION_ACK kPacket;

	if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD ) == false )
	{
		kPacket.m_iOK = NetError::ERR_FIELD_BOSS_03;	// ���� �ʵ尡 �ƴմϴ�.
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BOSS_FIELD_INTRUDE_RESTRICTION_ACK, kPacket );
		return;
	}

	if( m_spUserManager->IsHost( FIRST_SENDER_UID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_FIELD_BOSS_04;	// ��� ������ �����ϴ�.
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BOSS_FIELD_INTRUDE_RESTRICTION_ACK, kPacket );
		return;
	}
	
	if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_INTRUDE_RESTRICTION ) == false )
	{
		// ���� �Ұ� ������ ��û�ؿԴ�. ( ��ȹ�� �ǵ���ζ��, ������ �ǰ� 10% �Ʒ� ���� �� �� ��Ŷ�� �� ���̴�. )
		SEnum::AddFlag( m_byteBossFieldState, SEnum::BFRS_INTRUDE_RESTRICTION );
		SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BOSS_FIELD_INTRUDE_RESTRICTION_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_BOSS_FIELD_LOG_NOT )
{
	// ���� �ʵ� ���� �α� ���� ����
	for( KDBE_BOSS_FIELD_LOG_NOT::TYPE_BOSS_FIELD_USER_LOG::iterator it = m_kBossFieldLog.m_mapUserLog.begin(); it != m_kBossFieldLog.m_mapUserLog.end(); ++it )
	{
		const UidType		iUnitUID = it->first;
		KBossFieldUserLog&	kBossFieldUserLog = it->second.back();

		std::map<UidType, __int64>::iterator itFind = kPacket_.m_mapAttackDamage.find( iUnitUID );
		if( itFind != kPacket_.m_mapAttackDamage.end() )
		{
			kBossFieldUserLog.m_iAttackDamage = itFind->second;
		}

		itFind = kPacket_.m_mapGivenDamage.find( iUnitUID );
		if( itFind != kPacket_.m_mapGivenDamage.end() )
		{
			kBossFieldUserLog.m_iGivenDamage = itFind->second;
		}
	}
}
#endif // SERV_BATTLE_FIELD_BOSS

#pragma pack( pop )