//#include "SimLayer.h"
#include "ServerDefine.h"
#include "PvPRoom.h"
#include "NetError.h"
#include "Room/RoomFSM.h"
#include "Room/RoomUserFSM.h"
#include "X2Data/XSLMain.h"
#include "ResultProcess.h"
//{{ 2011. 07. 21	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch/PvpMatchManager.h"
	#include "PvpMatchResultTable.h"
#else
	#include "KPVPResultTable.h"
#endif SERV_PVP_NEW_SYSTEM
//}}
#include "KDropTable.h"
#include "AbuserLog/KAbuserLogManager.h"

#include "GameEvent/GameEventManager.h"

#include <iomanip>

//{{ 2012. 07. 17	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2_2
	#include "..\Common\X2Data\XSLBuffManager.h"
#endif SERV_2012_PVP_SEASON2_2
//}}

//{{ 2012. 10. 10	�ڼ���	ƨ�� ���� ������ ������
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	#include "..\Common\GameSysVal\GameSysVal.h"
#endif UDP_CAN_NOT_SEND_USER_KICK
//}}

NiImplementRTTI( KPvPRoom, KRoom );

#define CLASS_TYPE KPvPRoom

KPvPRoom::KPvPRoom()
{
	//{{ 2011. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_spRoomUserManager = KRoomUserManagerPtr( new KRoomUserManager );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	m_spRoomUserManager->Init( CXSLRoom::RT_PVP, 8 );

    m_fRemainingPlayTime = 0.f;
    m_bEndPlay			 = false;

	m_iPVPChannelClass	 = (int)KPVPChannelInfo::PCC_NONE;

#ifdef DUNGEON_ITEM
	m_iDropItemUID		 = 0;
#endif DUNGEON_ITEM
	//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	m_eMatchType		 = SEnum::MT_NONE;
	m_ePvpNpcType		 = SEnum::PNT_NONE;
	m_bOfficialPvpCancel = 0;
#endif SERV_PVP_NEW_SYSTEM
	//}}

#ifdef SERV_PVP_REMATCH
	m_iRematchCount		= 0;
#endif SERV_PVP_REMATCH
}

KPvPRoom::~KPvPRoom()
{
}

ImplToStringW( KPvPRoom )
{
    return START_TOSTRING_PARENTW( KRoom );
}

void KPvPRoom::Tick()
{
    KRoom::Tick();

    if( GetStateID() == KRoomFSM::S_PLAY )
    {
        if( m_cGameType == CXSLRoom::PGT_SURVIVAL || m_cGameType == CXSLRoom::PGT_TEAM_DEATH )
        {
            CheckRebirth();
        }

        CheckEndPlay();

#ifdef DUNGEON_ITEM
		// ������ ���� �ý��� : ��ӵ� �������� Ư���ð��� ������ �ٸ� ���������� �����.
		if( m_bIsItemMode == true && m_vecDropItem.size() > 0 )
		{
			for( int i = 0; i < static_cast<int>(m_vecDropItem.size()); ++i )
			{
				// - Ư���ð��� ������
				// - ���� ���� ���� ��� �������� �ִٸ�..
				if( m_vecDropItem[i].m_TimerOfChangeItem.elapsed() >= SiKDropTable()->GetPVPItemChangeTime() &&
					m_vecDropItem[i].m_bIsGet == false )
				{
					KEGS_CHANGE_PVP_ITEM_NOT kNot;

					if( false == SiKDropTable()->DecisionbyGroupItem( m_vecDropItem[i].m_iGroup, m_vecDropItem[i].m_iItemID ) )
					{
						START_LOG( cerr, L"���� ��Ӿ����� ������ ����.!" )
							<< END_LOG;
						continue;
					}

					kNot.m_iDropItemUID = m_vecDropItem[i].m_iDropItemUID;
					kNot.m_iItemID		= m_vecDropItem[i].m_iItemID;

					BroadCast( ERM_CHANGE_PVP_ITEM_NOT, kNot );

					m_vecDropItem[i].m_TimerOfChangeItem.restart();
				}
			}
		}
#endif DUNGEON_ITEM
    }

	//{{ 2011. 07. 29	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	switch( GetStateID() )
	{
	case KRoomFSM::S_WAIT:
	case KRoomFSM::S_LOAD:
	case KRoomFSM::S_PLAY:
	case KRoomFSM::S_RESULT:
		{
			// ���� �����̸� ����NPC�� �����濡 �����ִ� ���¸� üũ�ؼ� ���ɹ��� �Ȼ���� ����!
			if( IsOfficialMatch() == false )
				break;

			std::vector< UidType > vecPvpNpcUnitUIDLIst;
			if( m_spRoomUserManager->IsOnlyPvpNpcInRoom( vecPvpNpcUnitUIDLIst ) == false )
				break;

			// �ϳ��� ��Ż ��Ű��!
			BOOST_TEST_FOREACH( const UidType, iPvpNpcUnitUID, vecPvpNpcUnitUIDLIst )
			{
				if( m_spRoomUserManager->LeaveRoom( iPvpNpcUnitUID ) == false )
				{
					START_LOG( cerr, L"���� NPC �� ��Ż ����! ���� �Ͼ���� �ȵǴ� ����!" )
						<< BUILD_LOG( iPvpNpcUnitUID )
						<< END_LOG;
				}
				else
				{
					DelRebirth( iPvpNpcUnitUID );
				}
			}

			// ���� ����!
			StateTransition( KRoomFSM::I_TO_CLOSE );

			// ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
			SendRoomListInfo( NetError::ERR_ROOM_16 );
		}
		break;
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

void KPvPRoom::SetRoomInfo( const KRoomInfo& kInfo )
{
    m_cRoomType			= kInfo.m_RoomType;
    m_uiRoomListID		= kInfo.m_uiRoomListID;
    SetName( kInfo.m_RoomName );
    m_bPublic			= kInfo.m_bPublic;
    m_bTeamBalance		= kInfo.m_bTeamBalance;
    m_wstrPassword		= kInfo.m_wstrPassword;
    m_cGameType			= kInfo.m_PVPGameType;
    m_cWinningNumKill	= kInfo.m_WinKillNum;
    SetPlayTimeLimit( kInfo.m_fPlayTime );
    m_sWorldID			= kInfo.m_WorldID;
	//{{ 2008. 5. 27  ������  ������
	m_sShowPvpMapWorldID = kInfo.m_ShowPvpMapWorldID;
	//}}
#ifdef SERV_PVP_REMATCH
	m_mapAllPlayersSelectedMap = kInfo.m_mapAllPlayersSelectedMap;
#endif SERV_PVP_REMATCH
    m_cDifficultyLevel	= static_cast<char>(CXSLDungeon::DL_NORMAL);
    m_iDungeonID		= static_cast<int>(SEnum::DI_TUTORIAL_ELSWORD);
	m_bCanIntrude		= kInfo.m_bCanIntrude;
	m_iPVPChannelClass	= kInfo.m_iPVPChannelClass;	

#ifdef DUNGEON_ITEM
	if( m_cGameType == CXSLRoom::PGT_SURVIVAL )
		m_bIsItemMode = true;
	else
		m_bIsItemMode = kInfo.m_bIsItemMode;
#endif DUNGEON_ITEM
}

void KPvPRoom::StartGame()
{
    KRoom::StartGame();

	//{{ 2011. 10. 25	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_kPvpMonsterManager.StartGame();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}


#ifdef SERVER_PVP_BASE_DEFENCE_TEST
	m_PVPGameOption.m_BaseDefenceData.Init();
#endif SERVER_PVP_BASE_DEFENCE_TEST


#ifdef DUNGEON_ITEM
	m_vecDropItem.clear();
	m_iDropItemUID = 0;
#endif DUNGEON_ITEM

	//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	m_bOfficialPvpCancel = false;
#endif SERV_PVP_NEW_SYSTEM
	//}}
    
    KStatisticsKey kKey;
    kKey.m_vecIntKey.push_back( static_cast<int>(m_uiRoomListID) );

	// ���� PVP �ΰ�	
	for ( int iU = 0; iU < m_spRoomUserManager->GetNumMember(); ++iU )
	{
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iU );
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"RoomUser�� �������� ����." )
				<< END_LOG;
			continue;
		}

		KRoomUserInfo kRoomUserInfo;
		spRoomUser->GetRoomUserInfo( kRoomUserInfo );

		int iKeyCol;
		if( !KStatistics::GetUnitClassPvpStatColumn( kRoomUserInfo.m_cUnitClass, iKeyCol ) )
			continue;
		
		KSIManager.IncreaseCount( KStatistics::SI_LOC_PVP, kKey, iKeyCol, 1 );	
	}

	//{{ 2008. 5. 6  ������  PVP��� ����	
	switch( GetPlayMode() )
	{
	case CXSLRoom::PGT_TEAM:
		KSIManager.IncreaseCount( KStatistics::SI_LOC_PVP, kKey, KStatistics::SLP_TEAM_MATCH_MC, 1 );
		break;

	case CXSLRoom::PGT_TEAM_DEATH:
		KSIManager.IncreaseCount( KStatistics::SI_LOC_PVP, kKey, KStatistics::SLP_TEAM_DEATH_MC, 1 );
		break;

	case CXSLRoom::PGT_SURVIVAL:
		KSIManager.IncreaseCount( KStatistics::SI_LOC_PVP, kKey, KStatistics::SLP_SURVIVAL_MC, 1 );
		break;

	default:
		START_LOG( cerr, L"�̻��� ��������Ÿ���Դϴ�." )
			<< BUILD_LOG( static_cast<int>( GetPlayMode() ) )
			<< END_LOG;
		break;
	}
	//}}
	
    int iNumPlaying = m_spRoomUserManager->GetNumPlaying();
    if( iNumPlaying < 1 || iNumPlaying > 8 )
    {
        START_LOG( cerr, L"�÷��� ���� ���� ���� �̻���." )
            << BUILD_LOG( iNumPlaying )
            << END_LOG;
    }

	//{{ 2012. 10. 7	�ڼ���	������ �α� �߰�
#ifdef UDP_CAN_NOT_SEND_USER_KICK_LOG
	m_vecLanBugRecord.clear();
#endif UDP_CAN_NOT_SEND_USER_KICK_LOG
	//}}
}

void KPvPRoom::StartPlay()
{
    KRoom::StartPlay();

    m_vecRebirthInfo.clear();	
}

void KPvPRoom::EndPlay()
{
    START_LOG( clog2, L"���� �� ó��." )
        << BUILD_LOG( GetStateID() )
        << BUILD_LOG( GetStateIDString() );

	//////////////////////////////////////////////////////////////////////////
	// 1. �÷��� ���� ó��
    m_bEndPlay = false;
    KRoom::EndPlay();


	//////////////////////////////////////////////////////////////////////////
	// 2. ��� ó�� ����
	//=== Result ===
	const int iPlayTime = ( int )( m_kTimer[TM_PLAY].elapsed() + 0.5 );

	KEGS_END_GAME_PVP_RESULT_DATA_NOT kPacketNot;	
	kPacketNot.m_iPlayTime = iPlayTime;
	kPacketNot.m_iGameType = (int)m_cGameType;
	
#ifdef SERV_PVP_REMATCH
	kPacketNot.m_iRematchCount = m_iRematchCount;
#endif SERV_PVP_REMATCH	

	//////////////////////////////////////////////////////////////////////////
	// 3. ���� & ��� ä���� ������ ä�ο��� ������ ������ �Ҹ�

	switch( GetPVPChannelClass() )
	{
	case KPVPChannelInfo::PCC_FREE:		
	case KPVPChannelInfo::PCC_PLAY:
		//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	case KPVPChannelInfo::PCC_TOURNAMENT:
#endif SERV_PVP_NEW_SYSTEM
		//}}
		break;

	default:
		{
			//���ӿ� �����ߴ� �������� ���������� �������� ���� ��Ŵ.		
			BroadCastID( ERM_DECREASE_ENDURANCE_PVP_NOT, KRoomUserManager::UL_RESULT );
		}
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// 4. ���� ��� ó��

	KStatistics::StatLocalPvp eStatPVPType;
	//{{ 2011. 08. 03	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	KPvpMatchUserResultLog::END_TYPE eEndType;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	switch( m_cGameType )
	{
	case CXSLRoom::PGT_TEAM:
		{
			//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
			if( SiKResultProcess()->Result_TEAM( GetPlayTimeLimit(), GetRemainingPlayTime(), GetPVPChannelClass(), m_bOfficialPvpCancel, GetMatchType(), GetPvpNpcType(), m_spRoomUserManager, kPacketNot, eEndType ) == false )
#else
			if( SiKResultProcess()->Result_TEAM( GetPlayTimeLimit(), GetRemainingPlayTime(), GetPVPChannelClass(), m_bOfficialPvpCancel, m_spRoomUserManager, kPacketNot ) == false )
#endif SERV_PVP_NEW_SYSTEM
			//}}
			{
				START_LOG( cerr, L"NORMAL TEAM RESULT DATA FAIL.!" )
					<< END_LOG;
				return;
			}

			eStatPVPType = KStatistics::SLP_TEAM_MATCH_PT;
		}
		break;

	case CXSLRoom::PGT_SURVIVAL:
		{
			//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
			if( SiKResultProcess()->Result_SURVIVAL( GetPlayTimeLimit(), GetRemainingPlayTime(), GetPVPChannelClass(), m_bOfficialPvpCancel, GetMatchType(), GetPvpNpcType(),m_spRoomUserManager, kPacketNot, eEndType ) == false )
#else
			if( SiKResultProcess()->Result_SURVIVAL( GetPlayTimeLimit(), GetRemainingPlayTime(), GetPVPChannelClass(), m_spRoomUserManager, kPacketNot ) == false )
#endif SERV_PVP_NEW_SYSTEM
			//}}
			{
				START_LOG( cerr, L"NORMAL TEAM RESULT DATA FAIL.!" )
					<< END_LOG;
				return;
			}

			eStatPVPType = KStatistics::SLP_SURVIVAL_PT;
		}
		break;

	case CXSLRoom::PGT_TEAM_DEATH:
		{
			//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
			if( SiKResultProcess()->Result_TEAM_DEATH( GetPlayTimeLimit(), GetRemainingPlayTime(), GetPVPChannelClass(), m_bOfficialPvpCancel, GetMatchType(), GetPvpNpcType(),m_spRoomUserManager, kPacketNot, eEndType ) == false )
#else
			if( SiKResultProcess()->Result_TEAM_DEATH( GetPlayTimeLimit(), GetRemainingPlayTime(), GetPVPChannelClass(), m_spRoomUserManager, kPacketNot ) == false )
#endif SERV_PVP_NEW_SYSTEM
			//}}
			{
				START_LOG( cerr, L"TEAM DEATH RESULT DATA FAIL.!" )
					<< END_LOG;

				return;
			}

			eStatPVPType = KStatistics::SLP_TEAM_DEATH_PT;
		}
		break;

	default:
		{
			START_LOG( cerr, L"PVP ���� �̷� ����Ÿ���� ����.!" )
				<< BUILD_LOGc( m_cGameType )
				<< END_LOG;
			return;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 5. ����ä�� / ����ä�ο����� ������ ����!
	switch( GetPVPChannelClass() )
	{
	case KPVPChannelInfo::PCC_FREE:
	case KPVPChannelInfo::PCC_PLAY:
		//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	case KPVPChannelInfo::PCC_TOURNAMENT:
#endif SERV_PVP_NEW_SYSTEM
		//}}
		{
			BOOST_TEST_FOREACH( KPVPUnitResultInfo&, kPvpUnitResultInfo, kPacketNot.m_vecPVPUnitInfo )
			{
				//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
				kPvpUnitResultInfo.m_iRating			= 0;
				kPvpUnitResultInfo.m_iRPoint			= 0;
				kPvpUnitResultInfo.m_iAPoint			= 0;
				kPvpUnitResultInfo.m_fKFactorIncrement	= 0;
#else
				kPvpUnitResultInfo.m_nVSPoint = 0;
#endif SERV_PVP_NEW_SYSTEM
				//}}
				kPvpUnitResultInfo.m_nEXP = 0;
			}
		}
		break;
	}

	//{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
	//{{ 2012. 02. 22	��μ�	���� ����Ʈ, PVP NPC ���� ����Ʈ	���� ����
#ifdef SERV_NEW_PVP_QUEST_ERROR_MODIFY
#ifdef PVP_QUEST_HERO_KILL_COUNT
	if( GetPvpNpcType() == SEnum::PNT_HERO_NPC )
	{
		kPacketNot.m_bIsHeroNPC = true;
	}
#endif //PVP_QUEST_HERO_KILL_COUNT
#else
	if( GetPvpNpcType() == SEnum::PNT_HERO_NPC )
	{
		kPacketNot.m_bMatchNPC = true;
	}
#endif SERV_NEW_PVP_QUEST_ERROR_MODIFY
	//}}
	kPacketNot.m_bOfficialMatch = IsOfficialMatch();
#endif SERV_NEW_PVP_QUEST
	//}} 

	//{{ 2012. 10. 7	�ڼ���	������ �α� �߰�
#ifdef UDP_CAN_NOT_SEND_USER_KICK_LOG
	RecordLanBugUserResultProcess( kPacketNot );
#endif UDP_CAN_NOT_SEND_USER_KICK_LOG
	//}}

	BroadCast( ERM_END_GAME_PVP_RESULT_DATA_NOT, kPacketNot, KRoomUserManager::UL_RESULT );



	//////////////////////////////////////////////////////////////////////////
	// 6. ���ó���� ���� ���� GSUser�� ������Ʈ �ϱ� ���� �غ��۾�

	//���Ӽ����� �� UNIT �ڽ��� ����� ������Ʈ
	std::vector< KRoomUserPtr >						vecKRoomUserPtr;
	std::vector< KERM_UPDATE_PVP_UNIT_INFO_NOT >	vecUpdateUnitInfo;
	const int iPlayPvpMemberCount = m_spRoomUserManager->GetNumMember();
	//{{ 2011. 07. 27	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	KDBE_PVP_MATCH_RESULT_LOG_NOT kPacketToPvpLog;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	BOOST_TEST_FOREACH( const KPVPUnitResultInfo&, kPvpUnitResultInfo, kPacketNot.m_vecPVPUnitInfo )
	{
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( kPvpUnitResultInfo.m_UnitUID );
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"�� ���� �����Ͱ��� NULL��" )
				<< BUILD_LOG( kPvpUnitResultInfo.m_UnitUID )
				<< END_LOG;
			continue;
		}

		//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		// ���� NPC�� ó���� �ʿ� �����ϴ�.
		if( spRoomUser->IsPvpNpc() == true )
			continue;
#endif SERV_PVP_NEW_SYSTEM
		//}}

		vecKRoomUserPtr.push_back( spRoomUser );
		KERM_UPDATE_PVP_UNIT_INFO_NOT kData;
		//{{ 2009. 11. 16  ������	�����������Ʈ
		kData.m_iMemberCount = iPlayPvpMemberCount;
		kData.m_iPlayTime = iPlayTime;
		//}}
		//{{ 2011. 07. 22	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		kData.m_bIsOfficialMatch  = IsOfficialMatch();
		kData.m_iCurrentRating	  = spRoomUser->GetRating();
		kData.m_iCurrentMaxRating = spRoomUser->GetMaxRating();
		kData.m_iCurrentRPoint	  = spRoomUser->GetRPoint();
		kData.m_iCurrentAPoint	  = spRoomUser->GetAPoint();
#endif SERV_PVP_NEW_SYSTEM
		//}}

		//080527.hoons.����ä�ο����� ������ ����.
		switch( GetPVPChannelClass() )
		{
		case KPVPChannelInfo::PCC_FREE:
		case KPVPChannelInfo::PCC_PLAY:
			//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		case KPVPChannelInfo::PCC_TOURNAMENT:
#endif SERV_PVP_NEW_SYSTEM
			//}}
			break;

		default:
			// VP / EXP ������Ʈ
			//{{ 2011. 07. 11	������	���� ����
			//#ifdef SERV_PVP_NEW_SYSTEM
			kData.m_iRating		= kPvpUnitResultInfo.m_iRating;
			kData.m_iRPoint		= kPvpUnitResultInfo.m_iRPoint;
			kData.m_iAPoint		= kPvpUnitResultInfo.m_iAPoint;

			//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
			kData.m_cRank				= spRoomUser->GetCurrentRank();
			kData.m_cRankForServer		= spRoomUser->GetCurrentRankForServer();
			kData.m_fKFactorIncrement	= kPvpUnitResultInfo.m_fKFactorIncrement;
#endif SERV_2012_PVP_SEASON2
			//}}

			//#else
			//				kData.m_iVSPoint	= kPvpUnitResultInfo.m_nVSPoint;
			//#endif SERV_PVP_NEW_SYSTEM
			//}}
			kData.m_iEXP		= kPvpUnitResultInfo.m_nEXP;

			if( kPacketNot.m_bIsDrawn == false )
			{
				if( kPvpUnitResultInfo.m_bWin == true )
					kData.m_iWin	= 1;
				else
					kData.m_iLose	= 1;

				//{{ 2009. 6. 17  ������	���� ��� ������
				//if( PvpDropItem( kData ) == false )
				//{
				//	START_LOG( cerr, L"���� �̺�Ʈ ������ ó�� ����.!" )
				//		<< BUILD_LOG( kPacketNot.m_vecPVPUnitInfo[j].m_UnitUID )
				//		<< END_LOG;
				//}
				//}}
			}
			break;
		}		

		//////////////////////////////////////////////////////////////////////////
		// [�̺�Ʈ] (2010-07-29 ~ 2010-08-11) ��ȸä�� �¸��� ���� ������ ����
#ifdef SERV_TOURNAMENT_CONDITION
#ifdef SERV_MAIN_TOURNAMENT
#else
		if( SiKGameEventManager()->CheckTournamentEvent() == true )
		{
			if( GetPVPChannelClass() == KPVPChannelInfo::PCC_TOURNAMENT  &&  kPvpUnitResultInfo.m_bWin == true )
			{
				int iEventItemID = CXSLItem::EI_CAMILLAS_COIN_2010; // ī�ж� ���� 2010�� ��������
				const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iEventItemID );
				if( pItemTemplet == NULL )
				{
					START_LOG( cerr, L"���� �̺�Ʈ ������ ���ø��� ���� ����." )
						<< BUILD_LOG( iEventItemID )
						<< END_LOG;

					break;
				}

				KItemInfo kItemInfo;
				kItemInfo.m_iItemID = iEventItemID;
				kItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
				kItemInfo.m_iQuantity = 1;
				kItemInfo.m_sEndurance = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ) ? pItemTemplet->m_Endurance : 0;
				kData.m_mapItem.insert( std::make_pair( iEventItemID, kItemInfo ) );
			}
		}
#endif SERV_MAIN_TOURNAMENT
#endif SERV_TOURNAMENT_CONDITION
		//////////////////////////////////////////////////////////////////////////

		vecUpdateUnitInfo.push_back( kData );


		//////////////////////////////////////////////////////////////////////////		
		// ����� ���� üũ
		switch( GetPVPChannelClass() )
		{
		case KPVPChannelInfo::PCC_FREE:
		case KPVPChannelInfo::PCC_PLAY:
			//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		case KPVPChannelInfo::PCC_TOURNAMENT:
#endif SERV_PVP_NEW_SYSTEM
			//}}
			{
				if( SiKAbuserLogManager()->IsPvpAbuser( spRoomUser->GetCID() ) )
				{
					//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
					AbuserLogToDB( spRoomUser, kData.m_iRating, kData.m_iEXP );
#else
					AbuserLogToDB( spRoomUser, kData.m_iVSPoint, kData.m_iEXP );
#endif SERV_PVP_NEW_SYSTEM
					//}}
				}
			}
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		// ��� ::
		// �����, ED, VP ���
		switch( GetPVPChannelClass() )
		{
		case KPVPChannelInfo::PCC_FREE:
		case KPVPChannelInfo::PCC_PLAY:
			//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		case KPVPChannelInfo::PCC_TOURNAMENT:
#endif SERV_PVP_NEW_SYSTEM
			//}}
			{
				// ��� �ε��� ����
				KStatisticsKey kKey;
				kKey.m_vecIntKey.push_back( static_cast<int>(m_uiRoomListID) );
				KSIManager.IncreaseCount( KStatistics::SI_LOC_PVP, kKey, KStatistics::SLP_GET_EXP, kData.m_iEXP );
				//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
				KSIManager.IncreaseCount( KStatistics::SI_LOC_PVP, kKey, KStatistics::SLP_GET_VP, kData.m_iRating );
#else
				KSIManager.IncreaseCount( KStatistics::SI_LOC_PVP, kKey, KStatistics::SLP_GET_VP, kData.m_iVSPoint );
#endif SERV_PVP_NEW_SYSTEM
				//}}
			}
			break;
		}

		//{{ 2011. 07. 27	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		if( GetPVPChannelClass() == KPVPChannelInfo::PCC_OFFICIAL )
		{
			KPvpMatchUserResultLog kPvpMatchUserLog;
			kPvpMatchUserLog.m_iUnitUID		= spRoomUser->GetCID();
			kPvpMatchUserLog.m_wstrNickName = spRoomUser->GetNickName();
			kPvpMatchUserLog.m_ucLevel		= spRoomUser->GetLevel();
			kPvpMatchUserLog.m_cUnitClass	= spRoomUser->GetUnitClass_LUA();
			kPvpMatchUserLog.m_iRating		= spRoomUser->GetRating();
			//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
			kPvpMatchUserLog.m_cPvpRank		= spRoomUser->GetCurrentRank();
#else
			CXSLUnit::PVP_EMBLEM ePvpEmblem = CXSLUnit::GetPVPEmblem( spRoomUser->GetRating() );
			kPvpMatchUserLog.m_cPvpRank		= static_cast<char>(CXSLUnit::PvpEmblemToPvpRank( ePvpEmblem ));
#endif SERV_2012_PVP_SEASON2
			//}}
			kPvpMatchUserLog.m_iRPoint		= spRoomUser->GetRPoint();
			kPvpMatchUserLog.m_iAPoint		= spRoomUser->GetAPoint();
			kPvpMatchUserLog.m_iOfficialMatchCnt = spRoomUser->GetOfficialMatchCount();
			kPvpMatchUserLog.m_cTeam		= spRoomUser->GetTeam();
			kPvpMatchUserLog.m_bPartyApp	= spRoomUser->IsPlayWithParty();
			kPvpMatchUserLog.m_iWaitTime	= spRoomUser->GetMatchWaitTime();
			if( kPacketNot.m_bIsDrawn )
			{
				kPvpMatchUserLog.m_iWin		= 2;
			}
			else
			{
				kPvpMatchUserLog.m_iWin		= ( kPvpUnitResultInfo.m_bWin ? 1 : 0 );
			}
			kPvpMatchUserLog.m_iKill		= spRoomUser->GetNumKill();
			kPvpMatchUserLog.m_iAssist		= spRoomUser->GetNumMDKill();
			kPvpMatchUserLog.m_iDeath		= spRoomUser->GetNumDie();
			kPvpMatchUserLog.m_iNetRatingInc = kData.m_iRating;
			kPvpMatchUserLog.m_iNetRPInc	= kData.m_iRPoint;
			kPvpMatchUserLog.m_iNetAPInc	= kData.m_iAPoint;
			kPvpMatchUserLog.m_iEXPearned	= kData.m_iEXP;
			kPvpMatchUserLog.m_cEndType		= static_cast<char>(eEndType);
			kPacketToPvpLog.m_vecPvpMatchUserLog.push_back( kPvpMatchUserLog );
		}
#endif SERV_PVP_NEW_SYSTEM
		//}}

		//////////////////////////////////////////////////////////////////////////		
		// ���� ���
		{
			int iDrawn = 0;
			char cGameResult;
			if( kPacketNot.m_bIsDrawn == true  || 
				GetPVPChannelClass() == KPVPChannelInfo::PCC_FREE  || 
				//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
				GetPVPChannelClass() == KPVPChannelInfo::PCC_TOURNAMENT  ||
#endif SERV_PVP_NEW_SYSTEM
				//}}
				GetPVPChannelClass() == KPVPChannelInfo::PCC_PLAY )
			{
				cGameResult = KERM_PVP_USER_STATISTICS_NOT::UGR_DRAW;
				iDrawn = 1;
			}
			else
			{
				cGameResult  = ( kData.m_iWin == 1 ? KERM_PVP_USER_STATISTICS_NOT::UGR_WIN : KERM_PVP_USER_STATISTICS_NOT::UGR_LOSE );
			}

			KERM_PVP_USER_STATISTICS_NOT kPacketNot;
			kPacketNot.m_cGameType		 = m_cGameType;
			kPacketNot.m_cGameResult	 = cGameResult;
			kPacketNot.m_iGamePlayTime	 = static_cast<int>(spRoomUser->GetPlayTime());
			if( m_cGameType == CXSLRoom::PGT_SURVIVAL || m_cGameType == CXSLRoom::PGT_TEAM_DEATH )
			{
				kPacketNot.m_bIntrude = spRoomUser->GetIsIntrude();
			}
			kPacketNot.m_sWorldID		 = m_sWorldID;
			kPacketNot.m_iKillNum		 = spRoomUser->GetNumKill();
			SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_PVP_USER_STATISTICS_NOT, kPacketNot );

			// PVP ���� �·� �α� [����� üũ]
			int iResult = 0;

			if( kData.m_iWin == 1 )
				iResult = 1;
			else if( iDrawn )
				iResult = 3;

			//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
			PvPLogToFile( spRoomUser, kData.m_iRating, kData.m_iEXP, iResult );
#else
			PvPLogToFile( spRoomUser, kData.m_iVSPoint, kData.m_iEXP, iResult );
#endif SERV_PVP_NEW_SYSTEM
			//}}		
			//////////////////////////////////////////////////////////////////////////
		}
	}

	BroadCastUpdateUnitInfo( vecKRoomUserPtr, ERM_UPDATE_PVP_UNIT_INFO_NOT, vecUpdateUnitInfo );

	//{{ 2011. 07. 27	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	if( GetPVPChannelClass() == KPVPChannelInfo::PCC_OFFICIAL )
	{
		CTime tCurTime = CTime::GetCurrentTime();
		kPacketToPvpLog.m_cMatchType  = static_cast<char>(GetMatchType());
		kPacketToPvpLog.m_cNpcPvpType = static_cast<char>(GetPvpNpcType());
		kPacketToPvpLog.m_wstrRegDate = (const wchar_t*)tCurTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		kPacketToPvpLog.m_iPlayTime	  = iPlayTime;
		SendToLogDB( DBE_PVP_MATCH_RESULT_LOG_NOT, kPacketToPvpLog );
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}

	//////////////////////////////////////////////////////////////////////////
	// 7. ���� ���
	{
		//{{ 2008. 5. 6  ������  PVP��� ����	
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( static_cast<int>(m_uiRoomListID) );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_PVP, kKey, eStatPVPType, iPlayTime );
		//}}

		CTime kEndGameTime = CTime::GetCurrentTime();

		//{{ 2010. 06. 21  ������	��� ������
#ifdef SERV_STATISTICS_THREAD
		//////////////////////////////////////////////////////////////////////////	
		KE_LOCAL_LOG_PVP_ROOM_NOT kNot;
		kNot.m_iPlayMode		= GetPlayMode();
		kNot.m_bIsItemMode		= m_bIsItemMode;
		kNot.m_sWorldID			= m_sWorldID;
		kNot.m_iNumMember		= GetNumMember();
		kNot.m_iPlayTime		= iPlayTime;
		kNot.m_wstrEndGameTime	= (const wchar_t*)kEndGameTime.Format(_T("%H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_PVP_ROOM_NOT, kNot );
		//////////////////////////////////////////////////////////////////////////	
#else
		//////////////////////////////////////////////////////////////////////////
		//{{ 2008. 5. 20  ������  ���� ���� �߰� ���
#define KLocAlign std::setw(8) << std::setiosflags( std::ios::left )

		std::wfstream& fout = KSIManager.GetLocalLog().LocalLogStm( KStatisticsLocalLog::LOG_PVP_ROOM );	

		fout << L"\n";
		fout << KLocAlign << GetPlayMode() << L"\t";
		//{{ 2010. 01. 06  ������	��������
		fout << KLocAlign << m_bIsItemMode << L"\t";
		//}}
		fout << KLocAlign << m_sWorldID << L"\t";
		fout << KLocAlign << GetNumMember() << L"\t";
		fout << KLocAlign << iPlayTime << L"\t";
		fout << KLocAlign << (const wchar_t*)kEndGameTime.Format(_T("%H:%M:%S")) << L"\t";

		fout.flush();
		//}}
		//////////////////////////////////////////////////////////////////////////	
#endif SERV_STATISTICS_THREAD
		//}}
	}
}

void KPvPRoom::EndGame()
{
    KRoom::EndGame();

	//{{ 2011. 10. 25	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_kPvpMonsterManager.EndGame();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

#ifdef DUNGEON_ITEM
	m_vecDropItem.clear();
	m_iDropItemUID = 0;
#endif DUNGEON_ITEM
}

bool KPvPRoom::CheckIfPlayEnd()
{
    if( KRoom::CheckIfPlayEnd() )
    {
        return true;
    }

    if( m_cGameType == CXSLRoom::PGT_TEAM )
    {
        // ����
        if( m_spRoomUserManager->IsAnyTeamEliminated() )
        {
            return true;
        }

		if( m_spRoomUserManager->IsOneTeamExist() )
		{
			return true;
		}


    }
    else if( m_cGameType == CXSLRoom::PGT_TEAM_DEATH )
    {


#ifdef SERVER_PVP_BASE_DEFENCE_TEST
		
		if( true == m_PVPGameOption.m_bBaseDefence )
		{
			if( true == m_PVPGameOption.m_BaseDefenceData.m_bStatueOfRedDead )
			{
				return true;
			}
			else if( true == m_PVPGameOption.m_BaseDefenceData.m_bStatueOfBlueDead )
			{
				return true;
			}
		}

#endif SERVER_PVP_BASE_DEFENCE_TEST




//#ifdef PVP_BOSS_COMBAT_TEST
//		bool bRedTeamBossDead = false;
//		bool bBlueTeamBossDead = false;
//		if( m_spRoomUserManager->IsAnyTeamBossDead( bRedTeamBossDead, bBlueTeamBossDead ) )
//		{
//			return true;
//		}
//#endif PVP_BOSS_COMBAT_TEST


		//����̸� ������ die�� �Ǿ ���� ����üũ�� ���� �ʴ´�.
		if( GetPVPChannelClass() != KPVPChannelInfo::PCC_PLAY )
		{
			if( m_spRoomUserManager->IsAnyTeamReachObjectiveNumKill( m_cWinningNumKill ) )
			{
				return true;
			}
		}

		//����̸� ������ die�� �Ǿ ���� ����üũ�� ���� �ʴ´�.
		if( GetPVPChannelClass() != KPVPChannelInfo::PCC_PLAY )
		{
			if( m_spRoomUserManager->IsOneTeamExist() )
			{
				return true;
			}
		}

        if( m_spRoomUserManager->GetNumMember() <= 1 || m_spRoomUserManager->GetNumPlaying() <= 1 )
        {
            return true;
        }

		// ������ 1���̻� ���� ���� ������ ���� ����..
		if( m_spRoomUserManager->GetTeamNumPlaying(CXSLRoom::TN_BLUE) < 1 || 
			m_spRoomUserManager->GetTeamNumPlaying(CXSLRoom::TN_RED) < 1 )
			return true;
    }
    else
    {
        START_LOG( clog, L"���� ���� �˻�" )
            << BUILD_LOG( m_spRoomUserManager->GetNumMember() )
            << BUILD_LOG( m_spRoomUserManager->GetNumPlaying() )
			<< BUILD_LOG( GetRemainingPlayTime() )
			<< BUILD_LOG( GetPlayTimeLimit() )
			<< BUILD_LOGc( m_cWinningNumKill )
			<< BUILD_LOGc( m_spRoomUserManager->GetMaxKillUnit() )
            << END_LOG;

		//����̸� ������ die�� �Ǿ ���� ����üũ�� ���� �ʴ´�.
		if( GetPVPChannelClass() != KPVPChannelInfo::PCC_PLAY )
		{
			// ���� ����
			if( m_spRoomUserManager->IsAnyoneReachObjectiveNumKill( m_cWinningNumKill ) )
			{
				return true;
			}
		}

        if( IsAbleToIntrude() )
        {
            if( m_spRoomUserManager->GetNumMember() <= 1 || m_spRoomUserManager->GetNumPlaying() <= 1 )
            {
                return true;
            }
        }
        else
        {
            if( m_spRoomUserManager->GetNumPlaying() <= 1 )
            {
                return true;
            }
        }
    }

    return false;
}

bool KPvPRoom::IsAbleToIntrude()
{
	SET_ERROR( NET_OK );

    switch( m_cGameType )
    {
    case CXSLRoom::PGT_TEAM:
        return false;
    case CXSLRoom::PGT_TEAM_DEATH:
    case CXSLRoom::PGT_SURVIVAL:
		{			
			if( m_bEndPlay == true )
			{
				SET_ERROR( ERR_ROOM_29 );
				return false;
			}

			//�÷��� ���϶� ����üũ..
			if( GetStateID() == KRoomFSM::S_PLAY )
			{
				//����̸� ������ die�� �Ǿ ���� ����üũ�� ���� �ʴ´�.
				if( GetPVPChannelClass() != KPVPChannelInfo::PCC_PLAY )
				{
					//���� ų���� 1�����̸� ���ԺҰ�.
					if( m_cGameType == CXSLRoom::PGT_TEAM_DEATH )
					{
						if( ( m_cWinningNumKill - ( char )m_spRoomUserManager->GetMaxKillTeam() ) <= 1 )
						{
							SET_ERROR( ERR_ROOM_29 );
							return false;
						}
					}
					else
					{
						if( ( m_cWinningNumKill - (char)m_spRoomUserManager->GetMaxKillUnit() ) <= 1 )
						{
							SET_ERROR( ERR_ROOM_29 );
							return false;
						}
					}
				}
			}

			//���� ������ 10�� �����̸� ���� �Ҽ� ����.(�÷���Ÿ�ӿ��� ���� ���ӽð��� ���� �˻�)
			//if( (GetPlayTimeLimit() - GetRemainingPlayTime()) <= 10.0f )
			//	return false;

			//������ ������ ������ �ε��� ��ĥ���ִ� �ð��̾�� �Ѵ�.
			if( GetRemainingPlayTime() <= LOADING_TIME_OUT )
			{
				SET_ERROR( ERR_ROOM_33 );
				return false;
			}

			//���â �������̸� ���ԺҰ�.
			if( GetStateID() == KRoomFSM::S_RESULT )
			{
				SET_ERROR( ERR_ROOM_29 );
				return false;
			}
		}
        return true;
    default:
        START_LOG( cerr, L"�� �� ���� ���� ���� ���." )
            << BUILD_LOGc( m_cGameType )
            << END_LOG;
        return false;
    }
}

int KPvPRoom::DecideWinTeam()
{
    //std::vector< int > vecWinTeam;
    //m_spRoomUserManager->DecideWinTeam( m_cGameType, vecWinTeam );

    //if( ( int )vecWinTeam.size() == 1 )
    //{
    //    return vecWinTeam[0];
    //}
    //else
    //{
    //    return -1;
    //}

	//std::map<int, KResultProcess::KTeamData> mapTeamInfo;
	//{{ 2011. 08. 03	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	KPvpMatchUserResultLog::END_TYPE eEndType;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	int nWinTeam = SiKResultProcess()->DecidePVPWinTeam( GetPlayTimeLimit(), GetRemainingPlayTime(), m_spRoomUserManager, static_cast<CXSLRoom::PVP_GAME_TYPE>(GetPlayMode()), eEndType );


#ifdef SERVER_PVP_BASE_DEFENCE_TEST
	if( true == m_PVPGameOption.m_bBaseDefence )
	{
		if( true == m_PVPGameOption.m_BaseDefenceData.m_bStatueOfRedDead )
		{
			nWinTeam = (int) CXSLRoom::TN_BLUE; 
		}
		else if( true == m_PVPGameOption.m_BaseDefenceData.m_bStatueOfBlueDead )
		{
			nWinTeam = (int) CXSLRoom::TN_RED; 
		}
	}
#endif SERVER_PVP_BASE_DEFENCE_TEST


	return nWinTeam;
}

int KPvPRoom::GetPlayMode()
{
    return ( int )m_cGameType;
}

void KPvPRoom::HandleTimeOutUser( std::map< UidType, UidType >& mapUserCIDNGS )
{
    if( mapUserCIDNGS.empty() )
    {
        return;
    }

    // ��Ż�� ���. TODO : �̷� ����� ���� ����ؾ� �� ��.
    KStatisticsKey kKey;
    kKey.m_vecIntKey.push_back( static_cast<int>(m_uiRoomListID) );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_PVP, kKey, KStatistics::SLP_GIVE_UP, 1 );

    std::map< UidType, UidType >::iterator mit;
    for( mit = mapUserCIDNGS.begin(); mit != mapUserCIDNGS.end(); mit++ )
    {
		//080115. hoons : �Ⱦ��� ��� �ּ�����

		if( !m_spRoomUserManager->LeaveRoom( mit->first ) )
		{
			START_LOG( cerr, L"�� ������ ����." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
		}

        SendToGSCharacter( mit->second, mit->first, ERM_GAME_LOADING_TIME_OUT_NOT );

		//{{ 2009. 4. 23  ������	������������Ʈ
		SendPVPUserInfo( mit->first, KCommunityUserInfo::US_PVP_LOBBY );
		//}}
    }

    // ���� ��� �����鿡�� ���� ������ ������.
    KEGS_LEAVE_ROOM_NOT kPacketNot;
    kPacketNot.m_cRoomState = GetStateID();
    kPacketNot.m_iUnitUID = mit->first;
    GetRoomSlotInfo( kPacketNot.m_vecSlot );
    BroadCast( ERM_LEAVE_ROOM_NOT, kPacketNot );

    if( GetStateID() == KRoomFSM::S_LOAD )
    {
		//{{ 2011. 08. 05	������	�����濡 ���� ȥ�� ���ԵǴ� ���� ����
#ifdef SERV_CODE_REFACTORING_20110804
		CheckAllPlayerFinishLoading();
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

    // ���������� ������ �Ǵ� ��� ó��.
    if( GetStateID() == KRoomFSM::S_PLAY )
    {
        if( CheckIfPlayEnd() )
        {
            EndPlay();
        }
    }

    // �濡�� ��� ������ �������� ���� �ݴ´�.
    if( IsEmpty() )
    {
        StateTransition( KRoomFSM::I_TO_CLOSE );
    }

    // ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
    if( GetStateID() == KRoomFSM::S_CLOSE )
    {
        SendRoomListInfo( NetError::ERR_ROOM_16 );

		//{{ 2009. 4. 23  ������	������������Ʈ
		SendAllPVPUserInfo(); // �����ִ� ������ �����Ƿ� ���� ������ �ʾƵ� �ɷ���..
		//}}
    }
    else
    {
        SendRoomListInfo( NetError::ERR_ROOM_15 );

		//{{ 2009. 4. 23  ������	������������Ʈ
		SendAllPVPUserInfo(); // ����� ���� ���� S_RESULT �����̴�.
		//}}
    }
}

void KPvPRoom::CloseRoom()
{
	// ���� ������ �����Ұ͵��� ��������.
	//{{ 2011. 10. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_kPvpMonsterManager.OnCloseRoom();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2013. 01. 07	������	�濡�� �ʵ�� ���ƿ��� ���ϴ� ���� ����
#ifdef SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
	// ���� �ݱ� ���� �ʱ�ȭ �ѹ� ������!
	m_spRoomUserManager->Reset();
#endif SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
	//}}

	KCnBaseRoom::CloseRoom();
}

void KPvPRoom::CheckRebirth()
{
    std::vector< KRebirthInfo >::iterator vit;

    vit = m_vecRebirthInfo.begin();
    while( vit != m_vecRebirthInfo.end() )
    {
 			//{{ 2012. 11. 27 PVP���� ������ ���� ���� ��Ȱ ���� ���� - ��μ�
#ifdef SERV_PVP_REBIRTH_ADD_CONDITION
		if( ( float )vit->kTimer.elapsed() >= vit->fDelay && vit->bReady == true )
#else
		if( ( float )vit->kTimer.elapsed() >= vit->fDelay )
#endif SERV_PVP_REBIRTH_ADD_CONDITION
			//}
        {
            START_LOG( clog, L"��Ȱ ���� ����" )
                << BUILD_LOG( vit->nCID )
                << BUILD_LOG( vit->iPosition );

            if( m_spRoomUserManager->IsPlaying( vit->nCID ) )
            {
                KEGS_USER_UNIT_RE_BIRTH_NOT kPacket;
                kPacket.m_StartPosIndex = vit->iPosition;
                kPacket.m_UserUnitUID = vit->nCID;
                BroadCast( ERM_USER_UNIT_RE_BIRTH_NOT, kPacket );

				//{{ 2009. 2. 26  ������	��Ȱ�� ���� IsDie���� -> false
                m_spRoomUserManager->SetDie( vit->nCID, false );
				//}}
            }

			//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
			if( m_spRoomUserManager->IsPvpNpc( vit->nCID ) == true )
			{
				//������ ���� ����Ʈ���� ã�´�.
				//{{ 2011. 10. 25	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
				if( m_kPvpMonsterManager.IsExistMonster( static_cast<int>(vit->nCID) ) == true )
				{
					LIF( m_kPvpMonsterManager.SetMonsterRebirth( static_cast<int>(vit->nCID) ) );
				}
#else
				std::map< int, NPC_DATA >::iterator mit;
				mit = m_mapNPCData.find( static_cast<int>(vit->nCID) );

				// �ִٸ� ��Ȱó��!
				if( mit != m_mapNPCData.end() )
				{
					mit->second.m_bDie = false;
				}
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
			}
#endif SERV_PVP_NEW_SYSTEM
			//}}

            vit = m_vecRebirthInfo.erase( vit );
            continue;
        }

        vit++;
    }
}

void KPvPRoom::DelRebirth( UidType nCID )
{
	std::vector<KRebirthInfo>::iterator vit;

	for( vit = m_vecRebirthInfo.begin(); vit != m_vecRebirthInfo.end(); vit++ )
	{
		if( vit->nCID == nCID )
		{
			m_vecRebirthInfo.erase( vit );

			return;
		}
	}
}

void KPvPRoom::ReserveEndPlay()
{
    m_bEndPlay = true;
    m_kTimer[TM_DELAY_PACKET].restart();
}

void KPvPRoom::CheckEndPlay()
{
    if( !m_bEndPlay )
    {
        return;
    }

    if( m_kTimer[TM_DELAY_PACKET].elapsed() >= 3.0 )
    {
        START_LOG( clog2, L"�����̵� ���� ��." )
            << BUILD_LOG( GetStateID() )
            << BUILD_LOG( GetStateIDString() );
        EndPlay();
    }
}

void KPvPRoom::IntrudeTeamBalance( UidType nCID )
{
	//���Խ� �������� �ɼ�üũ�� ���ȱ�
	if( m_cGameType == CXSLRoom::PGT_TEAM_DEATH && 
		m_bTeamBalance == true && 
		m_spRoomUserManager->GetNumMember() > 1 )
	{
		if( m_spRoomUserManager->CheckTeamChange( nCID ) == true )
		{
			KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( nCID );
			KRoomSlotPtr spRoomSlot = m_spRoomUserManager->GetSlot( spRoomUser->GetSlotID() );

			switch( spRoomSlot->GetTeam() )
			{
			case CXSLRoom::TN_RED:
				{
					if( m_spRoomUserManager->ChangeTeam( nCID, (int)CXSLRoom::TN_BLUE ) == false )
						return;
				}
				break;

			case CXSLRoom::TN_BLUE:
				{
					if( m_spRoomUserManager->ChangeTeam( nCID, (int)CXSLRoom::TN_RED ) == false )
						return;
				}
				break;
			}

			KEGS_CHANGE_TEAM_NOT kNot;
			kNot.m_UnitUID			= nCID;
			kNot.m_TeamNum			= ( char )spRoomUser->GetTeam();
			kNot.m_SwapSlotIndex	= ( char )spRoomUser->GetSlotID();

			BroadCast( ERM_CHANGE_TEAM_NOT, kNot );
		}
	}
}

void KPvPRoom::DeathTeamBalance()
{
	if( m_cGameType == CXSLRoom::PGT_TEAM_DEATH && 
		m_bTeamBalance == true && 
		m_spRoomUserManager->GetNumMember() > 1 )
	{
		std::vector<KRoomUserPtr>	vecRoomUser;
		if( m_spRoomUserManager->DeathTeamBalance( vecRoomUser ) == true )
		{
			for( int i = 0; i < (int)vecRoomUser.size(); ++i )
			{
				KEGS_CHANGE_TEAM_NOT kNot;
				kNot.m_UnitUID			= vecRoomUser[i]->GetCID();
				kNot.m_TeamNum			= ( char )vecRoomUser[i]->GetTeam();
				kNot.m_SwapSlotIndex	= ( char )vecRoomUser[i]->GetSlotID();

				BroadCast( ERM_CHANGE_TEAM_NOT, kNot );
			}
		}
	}
}

bool KPvPRoom::PvpDropItem( KERM_UPDATE_PVP_UNIT_INFO_NOT& kInfo )
{
	KDropTable::DROP_DATA sDropDataEvent;
	bool bRet = true;

	bRet = SiKDropTable()->PvpDropItem( 1, sDropDataEvent );
	if( bRet == false )
	{
		START_LOG( cerr, L"Pvp Drop Item Data ������ ���� ===" )
			<< BUILD_LOGc( m_cGameType )
			<< END_LOG;

		return false;
	}

	if( sDropDataEvent.m_vecItemID.empty() == false )//m_iItemID != -1 )
	{
		for( int i = 0; i < (int)sDropDataEvent.m_vecItemID.size(); ++i )
		{
            const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( sDropDataEvent.m_vecItemID[i] );
            if( pItemTemplet == NULL )
            {
                START_LOG( cerr, L"������ ���ø��� ���� ����." )
                    << BUILD_LOG( sDropDataEvent.m_vecItemID[i] )
                    << END_LOG;

                continue;
            }

            std::map< int, KItemInfo >::iterator mit = kInfo.m_mapItem.find( sDropDataEvent.m_vecItemID[i] );
            if( mit != kInfo.m_mapItem.end() )
            {
                mit->second.m_iQuantity++;
            }
            else
            {
                KItemInfo kItemInfo;
                kItemInfo.m_iItemID = sDropDataEvent.m_vecItemID[i];
                kItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
                kItemInfo.m_iQuantity = 1;
                kItemInfo.m_sEndurance = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ) ? pItemTemplet->m_Endurance : 0;
                kItemInfo.m_cEnchantLevel = 0;
			    kInfo.m_mapItem.insert( std::make_pair( sDropDataEvent.m_vecItemID[i], kItemInfo ) );
            }
		}
	}

	return true;
}

//{{ 2012. 10. 7	�ڼ���	������ �α� �߰�
#ifdef UDP_CAN_NOT_SEND_USER_KICK_LOG
void KPvPRoom::RecordLanBugUser( const KEGS_UDP_CHECK_KICK_USER_NOT& kPacket_ )
{
	if( m_spRoomUserManager->GetUser( kPacket_.m_iMyUnitUID ) == NULL )
	{
		START_LOG( cerr, L"RoomUser�� �������� ����." )
			<< BUILD_LOG( kPacket_.m_iMyUnitUID )
			<< END_LOG;
		return;
	}
	else if( m_spRoomUserManager->GetUser( kPacket_.m_iAccusationUnitUID ) == NULL )
	{
		return;
	}

	// ���� ������ ��� �Ű� �ߴ��� ���
	BOOST_TEST_FOREACH( KEGS_UDP_CHECK_KICK_USER_NOT, kInfo, m_vecLanBugRecord )
	{
		if( ( kInfo.m_iMyUnitUID == kPacket_.m_iMyUnitUID ) &&
			( kInfo.m_iAccusationUnitUID == kPacket_.m_iAccusationUnitUID ) )
		{
			if( 255 < ( kInfo.m_byteNum + kPacket_.m_byteNum ) )
			{
				kInfo.m_byteNum = 255;
			}
			else
			{
				kInfo.m_byteNum += kPacket_.m_byteNum;
			}

			if( kPacket_.m_bNowKickOut == true )
			{
				kInfo.m_bNowKickOut = true;
			}
			return;
		}
	}

	m_vecLanBugRecord.push_back( kPacket_ );
}

void KPvPRoom::RecordLanBugUserResultProcess( const KEGS_END_GAME_PVP_RESULT_DATA_NOT& kPacketNot )
{
	if( m_vecLanBugRecord.empty() == true )
	{
		return;
	}

	KDBE_UDP_CHECK_KICK_USER_NOT kPacket;
	byte i = 0;

	kPacket.m_iRoomUID			= GetUID();
	kPacket.m_iPVPChannelClass	= GetPVPChannelClass();
	kPacket.m_iMatchType		= GetMatchType();

	BOOST_TEST_FOREACH( const KEGS_UDP_CHECK_KICK_USER_NOT&, kInfo, m_vecLanBugRecord )
	{
		//if( kInfo.m_iMyUnitUID == kInfo.m_iAccusationUnitUID )
		//{
		//	// �ù� ���� ( �ڱ� �ڽ��� ƨ�ܳ� )
		//	KLanBugRecord kLanBugRecord( kInfo.m_iMyUnitUID, kInfo.m_iAccusationUnitUID, kInfo.m_byteNum, kInfo.m_bNowKickOut, 0 );
		//	kPacket.m_vecLanBugRecord.push_back( kLanBugRecord );
		//	continue;
		//}

		for( i = 0; i < kPacketNot.m_vecPVPUnitInfo.size(); ++i )
		{
			if( kPacketNot.m_vecPVPUnitInfo[i].m_UnitUID == kInfo.m_iMyUnitUID )
			{
				break;
			}
		}
		
		if( i < kPacketNot.m_vecPVPUnitInfo.size() )
		{
			if( kPacketNot.m_bIsDrawn == true )
			{
				KLanBugRecord kLanBugRecord( kInfo.m_iMyUnitUID, kInfo.m_iAccusationUnitUID, kInfo.m_byteNum, kInfo.m_bNowKickOut, 1 );
				kPacket.m_vecLanBugRecord.push_back( kLanBugRecord );
			}
			else
			{
				KLanBugRecord kLanBugRecord( kInfo.m_iMyUnitUID, kInfo.m_iAccusationUnitUID, kInfo.m_byteNum, kInfo.m_bNowKickOut, ( kPacketNot.m_vecPVPUnitInfo[i].m_bWin == true ) ? 2 : 0 );
				kPacket.m_vecLanBugRecord.push_back( kLanBugRecord );
			}
		}
		else
		{
			KLanBugRecord kLanBugRecord( kInfo.m_iMyUnitUID, kInfo.m_iAccusationUnitUID, kInfo.m_byteNum, kInfo.m_bNowKickOut, 3 );
			kPacket.m_vecLanBugRecord.push_back( kLanBugRecord );
		}
	}

	if( kPacket.m_vecLanBugRecord.empty() == false )
	{
		SendToLogDB( DBE_UDP_CHECK_KICK_USER_NOT, kPacket );
	}
}
#endif UDP_CAN_NOT_SEND_USER_KICK_LOG
//}}

void KPvPRoom::SendRoomListInfo( IN const int iCode )
{
	KERM_ROOM_LIST_INFO_NOT kPacket;
	kPacket.m_iCode = iCode;
	GetRoomInfo( kPacket.m_kRoomInfo );
	kPacket.m_kRoomInfo.m_iPVPChannelClass = GetPVPChannelClass();
	BroadCastAllGSServer( ERM_ROOM_LIST_INFO_NOT, kPacket );
}

//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
void KPvPRoom::SendPVPUserInfo( UidType iUnitUID, char cState /*= KCommunityUserInfo::US_NONE*/ )
{
	if( m_cRoomType != CXSLRoom::RT_PVP )
		return;

	if( GetPVPChannelClass() == KPVPChannelInfo::PCC_OFFICIAL )
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

	if( GetPVPChannelClass() == KPVPChannelInfo::PCC_OFFICIAL )
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

//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KPvPRoom::OnLeaveRoom( IN const UidType iLAST_SENDER_UID, IN const UidType iFIRST_SENDER_UID, IN const KERM_LEAVE_ROOM_REQ& kPacket_, IN const u_short usEventIDAck )
{
	KRoom::OnLeaveRoom( iLAST_SENDER_UID, iFIRST_SENDER_UID, kPacket_, usEventIDAck );

	// ��Ż�� ���.
	if( GetStateID() == KRoomFSM::S_LOAD || GetStateID() == KRoomFSM::S_PLAY )
	{
		//=== ������ ��Ż ���ó�� ===
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iFIRST_SENDER_UID );
		if( !spRoomUser )
		{
			spRoomUser = m_spRoomUserManager->GetUser( iFIRST_SENDER_UID, KRoomUserManager::UT_OBSERVER );
		}

		if( !spRoomUser )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�" )
				<< BUILD_LOG( iFIRST_SENDER_UID )
				<< END_LOG;
		}
		else
		{
			if( spRoomUser->IsObserver() == false )
			{
				KStatisticsKey kKey;
				kKey.m_vecIntKey.push_back( static_cast<int>(m_uiRoomListID) );
				KSIManager.IncreaseCount( KStatistics::SI_LOC_PVP, kKey, KStatistics::SLP_GIVE_UP, 1 );

				KERM_UPDATE_PVP_UNIT_INFO_NOT kPacket;
				//{{ 2009. 11. 16  ������	�����������Ʈ
				kPacket.m_iMemberCount = m_spRoomUserManager->GetNumMember();
				//}}
				kPacket.m_bOutRoom = true;
				//{{ 2011. 07. 22	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
				kPacket.m_bIsOfficialMatch = IsOfficialMatch();
				kPacket.m_iCurrentRating = spRoomUser->GetRating();
				kPacket.m_iCurrentMaxRating = spRoomUser->GetMaxRating();
				kPacket.m_iCurrentRPoint = spRoomUser->GetRPoint();
				kPacket.m_iCurrentAPoint = spRoomUser->GetAPoint();
#endif SERV_PVP_NEW_SYSTEM
				//}}

				//{{ 2012. 09. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
				kPacket.m_cRank				= spRoomUser->GetCurrentRank();
				kPacket.m_cRankForServer	= spRoomUser->GetCurrentRankForServer();
#endif SERV_2012_PVP_SEASON2
				//}}

				if( m_cGameType == CXSLRoom::PGT_TEAM )
				{
					//{{ 2011. 07. 25	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
					SiKResultProcess()->Result_TEAM_BREAK( m_spRoomUserManager, spRoomUser, kPacket_.m_iReason, GetMatchType(), GetPvpNpcType(), kPacket );
#else
					SiKResultProcess()->Result_TEAM_BREAK( m_spRoomUserManager, spRoomUser, kPacket );
#endif SERV_PVP_NEW_SYSTEM
					//}}

					//������ ��Ż�� �� ��ϵ�����
					kPacket.m_iLose		= 1;

					if( GetPVPChannelClass() == KPVPChannelInfo::PCC_FREE  || 
						//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
						GetPVPChannelClass() == KPVPChannelInfo::PCC_TOURNAMENT  || 
#endif SERV_PVP_NEW_SYSTEM
						//}}
						GetPVPChannelClass() == KPVPChannelInfo::PCC_PLAY )
					{
						kPacket.m_iEXP = 0;
						//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
						kPacket.m_iRating = 0;
						kPacket.m_iRPoint = 0;
						kPacket.m_iAPoint = 0;
#else
						kPacket.m_iVSPoint = 0;
#endif SERV_PVP_NEW_SYSTEM
						//}}
						kPacket.m_iLose	= 0;
					}

					SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, ERM_UPDATE_PVP_UNIT_INFO_NOT, kPacket );

					if( GetPVPChannelClass() != KPVPChannelInfo::PCC_FREE  && 
						//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
						GetPVPChannelClass() != KPVPChannelInfo::PCC_TOURNAMENT  && 
#endif SERV_PVP_NEW_SYSTEM
						//}}
						GetPVPChannelClass() != KPVPChannelInfo::PCC_PLAY )
					{
						//{{ 2007. 12. 17  ������  ���� ��� [����]
						KERM_PVP_USER_STATISTICS_NOT kPacketNot;
						kPacketNot.m_cGameType		 = m_cGameType;
						kPacketNot.m_cGameResult	 = KERM_PVP_USER_STATISTICS_NOT::UGR_DROP;
						kPacketNot.m_iGamePlayTime	 = static_cast<int>(spRoomUser->GetPlayTime());
						kPacketNot.m_bIntrude		 = spRoomUser->GetIsIntrude();
						kPacketNot.m_sWorldID		 = m_sWorldID;
						kPacketNot.m_iKillNum		 = m_spRoomUserManager->GetKillNumber( iFIRST_SENDER_UID );
						SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, ERM_PVP_USER_STATISTICS_NOT, kPacketNot );
						//}}
					}

					// PVP ���� �·� �α� [����� üũ]
					PvPLogToFile( spRoomUser, 0, 0, 2 );
					//////////////////////////////////////////////////////////////////////////
				}
				//������ & �����̹����� leave game���� �г�Ƽ�� lose�� ����� ������
				//������ ������ ��츸 ó�����ش�.
				else if( kPacket_.m_iReason	== NetError::NOT_LEAVE_ROOM_REASON_01  ||
					kPacket_.m_iReason	== NetError::NOT_LEAVE_ROOM_REASON_23 )
				{
					switch( m_cGameType )
					{
					case CXSLRoom::PGT_TEAM_DEATH:
						//{{ 2011. 07. 25	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
						SiKResultProcess()->Result_TEAM_DEATH_BREAK( m_spRoomUserManager, spRoomUser, kPacket_.m_iReason, GetMatchType(), GetPvpNpcType(), kPacket );
#else
						SiKResultProcess()->Result_TEAM_DEATH_BREAK( m_spRoomUserManager, spRoomUser, kPacket );
#endif SERV_PVP_NEW_SYSTEM
						//}}
						break;

					case CXSLRoom::PGT_SURVIVAL:
						//{{ 2011. 07. 25	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
						SiKResultProcess()->Result_SURVIVAL_BREAK( m_spRoomUserManager, spRoomUser, kPacket_.m_iReason, GetMatchType(), GetPvpNpcType(), kPacket );
#else
						SiKResultProcess()->Result_SURVIVAL_BREAK( m_spRoomUserManager, spRoomUser, kPacket );
#endif SERV_PVP_NEW_SYSTEM
						//}}
						break;
					}

					//������ ��Ż�� �� ��ϵ�����
					kPacket.m_iLose		= 1;

					if( GetPVPChannelClass() == KPVPChannelInfo::PCC_FREE  || 
						//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
						GetPVPChannelClass() == KPVPChannelInfo::PCC_TOURNAMENT  || 
#endif SERV_PVP_NEW_SYSTEM
						//}}
						GetPVPChannelClass() == KPVPChannelInfo::PCC_PLAY )
					{
						kPacket.m_iEXP = 0;
						//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
						kPacket.m_iRating = 0;
						kPacket.m_iRPoint = 0;
						kPacket.m_iAPoint = 0;
#else
						kPacket.m_iVSPoint = 0;
#endif SERV_PVP_NEW_SYSTEM
						//}}
						kPacket.m_iLose	= 0;
					}

					SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, ERM_UPDATE_PVP_UNIT_INFO_NOT, kPacket );

					if( GetPVPChannelClass() != KPVPChannelInfo::PCC_FREE  && 
						//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
						GetPVPChannelClass() != KPVPChannelInfo::PCC_TOURNAMENT  && 
#endif SERV_PVP_NEW_SYSTEM
						//}}
						GetPVPChannelClass() != KPVPChannelInfo::PCC_PLAY )
					{
						//{{ 2007. 12. 17  ������  ���� ��� [����]
						KERM_PVP_USER_STATISTICS_NOT kPacketNot;
						kPacketNot.m_cGameType		 = m_cGameType;
						kPacketNot.m_cGameResult	 = KERM_PVP_USER_STATISTICS_NOT::UGR_DROP;
						kPacketNot.m_iGamePlayTime	 = static_cast<int>(spRoomUser->GetPlayTime());
						kPacketNot.m_bIntrude		 = spRoomUser->GetIsIntrude();
						kPacketNot.m_sWorldID		 = m_sWorldID;
						kPacketNot.m_iKillNum		 = m_spRoomUserManager->GetKillNumber( iFIRST_SENDER_UID );
						SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, ERM_PVP_USER_STATISTICS_NOT, kPacketNot );
						//}}
					}

					// PVP ���� �·� �α� [����� üũ]
					PvPLogToFile( spRoomUser, 0, 0, 2 );
					//////////////////////////////////////////////////////////////////////////
				}
			}
		}
	}

	// ��Ŷ ����
	KEGS_LEAVE_ROOM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;

	if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_27 )
	{
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_27;
	}
	else
	{
		kPacket.m_iReason = NetError::NET_OK;
	}
	
	if( !m_spRoomUserManager->LeaveRoom( iFIRST_SENDER_UID ) )
	{
		START_LOG( cerr, L"�� ������ ����." );
		kPacket.m_iOK = NetError::ERR_SLOT_04;
		SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, usEventIDAck, kPacket );
		return;
	}
	else
	{
		DelRebirth( iFIRST_SENDER_UID );

		SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, usEventIDAck, kPacket );

		//{{ 2009. 4. 23  ������	������������Ʈ
		SendPVPUserInfo( iFIRST_SENDER_UID, KCommunityUserInfo::US_PVP_LOBBY );
		//}}

		//{{ 2012. 10. 16	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
		LogToDB_GamePlayNetWorkInfo( iFIRST_SENDER_UID, kPacket_.m_kGamePlayNetworkInfo, true );
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
		//}}
	}

	if( IsEmpty() == false || IsEmpty( KRoomUserManager::UT_OBSERVER ) == false )
	{
		// ���� ��� �����鿡�� ���� ������ ������.
		KEGS_LEAVE_ROOM_NOT kPacketNot;
		kPacketNot.m_cRoomState = GetStateID();
		kPacketNot.m_iUnitUID = iFIRST_SENDER_UID;
		kPacketNot.m_iReason = kPacket_.m_iReason;
		GetRoomSlotInfo( kPacketNot.m_vecSlot );
		GetRoomSlotInfo( kPacketNot.m_vecObserverSlot, KRoomUserManager::UT_OBSERVER );
		BroadCast( ERM_LEAVE_ROOM_NOT, kPacketNot );
	}

	if( GetStateID() == KRoomFSM::S_LOAD && IsEmpty() == false )
	{
		//{{ 2011. 08. 05	������	�����濡 ���� ȥ�� ���ԵǴ� ���� ����
#ifdef SERV_CODE_REFACTORING_20110804
		CheckAllPlayerFinishLoading();
#else
		// ��� ������ �ε� �������� not�� ������.
		if( m_spRoomUserManager->IsAllPlayerFinishLoading() && IsEmpty() == false )
		{
			BroadCastID( ERM_GAME_LOADING_ALL_UNIT_OK_NOT );

			// ���������� �ʱ�ȭ.
			StartPlay();

			KEGS_PLAY_START_NOT kNot;
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

	// ���������� ������ �Ǵ� ��� ó��.
	if( GetStateID() == KRoomFSM::S_PLAY && IsEmpty() == false )
	{
		//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		// ���� ������ ���
		if( GetPVPChannelClass() == KPVPChannelInfo::PCC_OFFICIAL )
		{
			// �÷��� Ÿ���� 30�� �̸��̸�..?
			const int iPlayTime = ( int )( m_kTimer[TM_PLAY_WITHOUT_LOADING].elapsed() + 0.5 );
			if( iPlayTime < SEnum::PE_PVP_OFFICIAL_MIN_PLAY_TIME )
			{
				// ���� ���� ���!
				m_bOfficialPvpCancel = true;
			}
		}
#endif SERV_PVP_NEW_SYSTEM
		//}}

		if( ( CheckIfPlayEnd() == true ) || ( m_bOfficialPvpCancel == true ) )
		{
			EndPlay();
		}
	}

	//���â�� ���������� ���� �������ó��
	if( GetStateID() == KRoomFSM::S_RESULT && IsEmpty() == false )
	{
		CheckResultSuccess();
	}

	// �濡�� ��� ������ �������� ���� �ݴ´�.
	if( IsEmpty() == true && IsEmpty( KRoomUserManager::UT_OBSERVER ) == true )
	{
		StateTransition( KRoomFSM::I_TO_CLOSE );
	}

	// ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
	if( GetStateID() == KRoomFSM::S_CLOSE )
	{
		SendRoomListInfo( NetError::ERR_ROOM_16 );

		//{{ 2009. 4. 23  ������	������������Ʈ
		SendAllPVPUserInfo(); // �����ִ� ����� �����ٵ� ���� �ȳ�����..
		//}}
	}
	else
	{
		SendRoomListInfo( NetError::ERR_ROOM_15 );

		//{{ 2009. 4. 23  ������	������������Ʈ
		SendAllPVPUserInfo(); // ���� RoomFSM�� ���� ������ ���¸� ������Ʈ
		//}}
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 05. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KPvPRoom::TimeCountFinishProcess( IN const u_short usEventID )
{
	switch( usEventID )
	{
	case ERM_OPEN_PVP_ROOM_FOR_MATCH_REQ:
		{
			// ���� �ʱ�ȭ.
			StartGame();
		
#ifdef SERV_PVP_REMATCH
			// �Ϲ� ��Ī���� ���۵� �� �ʱ�ȭ
			m_iRematchCount = 0;
#endif SERV_PVP_REMATCH	

			// ���� ���� ��Ŷ!
			KEGS_GAME_START_PVP_MATCH_NOT kNot;
			m_kPvpMonsterManager.GetPvpNpcInfo( kNot.m_mapPvpNpcInfo );
			kNot.m_RoomInfo.m_iPVPChannelClass = GetPVPChannelClass();
			GetRoomInfo( kNot.m_RoomInfo );
			GetRoomSlotInfo( kNot.m_vecSlot );
			GetRoomSlotInfo( kNot.m_vecObserverSlot, KRoomUserManager::UT_OBSERVER );
			BroadCast( ERM_GAME_START_PVP_MATCH_NOT, kNot );
		}
		break;

	default:
		{
			START_LOG( cerr, L"����� ���� �ȵȴ�! �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
				<< END_LOG;
		}
		break;
	}   
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 10. 16	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
void KPvPRoom::LogToDB_GamePlayNetWorkInfo( IN const UidType iUnitUID, IN const KGamePlayNetworkInfoPackage& kInfo, IN const bool bLeaveRoom )
{
	if( bLeaveRoom == false )
	{
		if( GetPVPChannelClass() != KPVPChannelInfo::PCC_OFFICIAL )
		{
			return;
		}
	}

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

void KPvPRoom::ProcessNativeEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {
   _CASE( ERM_OPEN_PVP_ROOM_REQ, KERM_OPEN_ROOM_REQ );
	//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
   _CASE( ERM_OPEN_PVP_ROOM_FOR_MATCH_REQ, KEGB_OPEN_PVP_ROOM_FOR_MATCH_REQ );
#endif SERV_PVP_NEW_SYSTEM
	//}}
    CASE( ERM_GAME_START_REQ );
	CASE( ERM_LEAVE_ROOM_REQ );
	//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
   _CASE( ERM_LEAVE_ROOM_FOR_JOIN_BATTLE_FIELD_REQ, KERM_LEAVE_ROOM_REQ );
   _CASE( ERM_LEAVE_ROOM_FOR_ENTER_THE_VILLAGE_REQ, KERM_LEAVE_ROOM_REQ );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ dmlee 2008.11.13 PVP monster
	_CASE( ERM_NPC_UNIT_CREATE_REQ, KEGS_NPC_UNIT_CREATE_REQ );
	_CASE( ERM_NPC_UNIT_DIE_REQ, KEGS_NPC_UNIT_DIE_REQ );
	//}} dmlee 2008.11.13 PVP monster

	//{{ 2009. 7. 20  ������	��������
   _CASE( ERM_CHANGE_SLOT_OPEN_REQ, KEGS_CHANGE_SLOT_OPEN_REQ );
	//}}

   _CASE( ERM_USER_UNIT_DIE_REQ, KEGS_USER_UNIT_DIE_REQ );
    CASE_NOPARAM( ERM_USER_UNIT_DIE_COMPLETE_REQ );
   _CASE( ERM_USER_UNIT_RE_BIRTH_POS_REQ, KEGS_USER_UNIT_RE_BIRTH_POS_REQ );
	CASE( ERM_MY_USER_UNIT_INFO_TO_SERVER_REQ );
    CASE_NOPARAM( ERM_LEAVE_GAME_REQ );
    CASE_NOPARAM( ERM_STATE_CHANGE_GAME_INTRUDE_REQ );
   _CASE( ERM_INTRUDE_START_REQ, KEGS_INTRUDE_START_REQ );
   _CASE( ERM_CHANGE_TEAM_REQ, KEGS_CHANGE_TEAM_REQ );
   _CASE( ERM_CHANGE_MAP_REQ, KEGS_CHANGE_MAP_REQ );
   _CASE( ERM_CHANGE_WINNING_NUM_KILL_REQ, KEGS_CHANGE_WINNING_NUM_KILL_REQ );

#ifdef SERVER_PVP_BASE_DEFENCE_TEST
   _CASE( ERM_CHANGE_PVP_GAME_OPTION_REQ, KEGS_CHANGE_PVP_GAME_OPTION_REQ );
#endif SERVER_PVP_BASE_DEFENCE_TEST

   

   _CASE( ERM_CHECK_INVALID_USER_NOT, UidType );

#ifdef DUNGEON_ITEM
   _CASE( ERM_CREATE_PVP_ITEM_REQ, KEGS_CREATE_PVP_ITEM_REQ );
   _CASE( ERM_GET_ITEM_REQ, KEGS_GET_ITEM_REQ );
#endif DUNGEON_ITEM

   //{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	CASE( ERM_UPDATE_PVP_PLAY_UNIT_INFO_NOT );
   _CASE( ERM_PVP_NPC_UNIT_RE_BIRTH_POS_REQ, KEGS_PVP_NPC_UNIT_RE_BIRTH_POS_REQ );
   _CASE( ERM_PVP_NPC_UNIT_INFO_TO_SERVER_REQ, KEGS_PVP_NPC_UNIT_INFO_TO_SERVER_REQ );
#endif SERV_PVP_NEW_SYSTEM
   //}}

   //{{ 2012. 02. 14    ��μ�   PVP ���� Item ���� ��ġ ����
#ifdef SERV_PVP_ITEM_CHANGE_POS
	_CASE( ERM_CHANGE_PVP_ITEM_POS_REQ, KEGS_CHANGE_PVP_ITEM_POS_REQ );
#endif SERV_PVP_ITEM_CHANGE_POS
   //}}
	//{{ 2012. 09. 02	�ڼ���	Merge ( �������� Ŭ���̾�Ʈ ���۵����� UDP ��Ŷ�� ���ƿ��� �ʴ� ������ �߰ߵǸ� �������� ű�Ѵ�. // 2012.06.11 lygan_������ )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	_CASE( ERM_UDP_CHECK_KICK_USER_NOT, KEGS_UDP_CHECK_KICK_USER_NOT );
#endif UDP_CAN_NOT_SEND_USER_KICK
	//}}
#ifdef SERV_PVP_REMATCH
	_CASE( ERM_PVP_REMATCH_REQ, KEGS_PVP_REMATCH_REQ );
#endif SERV_PVP_REMATCH

#ifdef SERV_NEW_PVPROOM_PROCESS
	_CASE( ERM_CHANGE_PVP_ROOM_PUBLIC_REQ, KEGS_CHANGE_PVP_ROOM_PUBLIC_REQ );
	_CASE( ERM_CHANGE_PVP_ROOM_NAME_REQ, KEGS_CHANGE_PVP_ROOM_NAME_REQ );
#endif SERV_NEW_PVPROOM_PROCESS

	//{{ 2011. 01. 25  ��μ�  ���� Ʈ���� ����(������ ������ - ���� PVP Ȯ��)
#ifdef SERV_WORLD_TRIGGER_RELOCATION
	_CASE( ECN_WORLD_TRIGGER_RELOCATION_REQ, KEGS_WORLD_TRIGGER_RELOCATION_REQ );
#endif SERV_WORLD_TRIGGER_RELOCATION
	//}}

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    CASE_NOPARAM( ERM_BATTLE_FIELD_ZOMBIE_ALERT_NOT );
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

    default:
        START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ��� - " << spEvent_->GetIDStr() )
            << BUILD_LOG( spEvent_->m_usEventID );
    }
}

_IMPL_ON_FUNC( ERM_OPEN_PVP_ROOM_REQ, KERM_OPEN_ROOM_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_INIT ), ERM_OPEN_ROOM_ACK, KEGS_CREATE_ROOM_ACK );

	//{{ 2010. 03. 14  ������	�� �й� ��� ����
#ifdef SERV_ROOM_COUNT
	SendRoomCountInfo( CXSLRoom::RT_PVP, true );
#endif SERV_ROOM_COUNT
	//}}

    // �� �ɼ� ���� ����.
    SetRoomInfo( kPacket_.m_kRoomInfo );

	//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	// ��ġ Ÿ��
	SetMatchType( SEnum::MT_NONE );

	// ���� NPC����
	SetPvpNpcType( SEnum::PNT_NONE );
#endif SERV_PVP_NEW_SYSTEM
	//}}

    // �� �����̹Ƿ� ���� ������ �ʱ�ȭ �Ѵ�.
    m_spRoomUserManager->Reset();
	//������ڸ� �濡 �����鼭 ����Ÿ�Կ� ���� ���ɼǼ����� ���ְ� �־��ֱ�����
    m_spRoomUserManager->AssignTeam( kPacket_.m_kRoomInfo.m_PVPGameType );

	//{{ 2010. 02. 10  ������	��ȸ ä��	
	if( GetPVPChannelClass() == KPVPChannelInfo::PCC_TOURNAMENT  &&  kPacket_.m_kRoomInfo.m_PVPGameType != CXSLRoom::PGT_SURVIVAL )
	{
		// ��ȸ�� 2:2��
		m_spRoomUserManager->CloseSlot( 2 );
		m_spRoomUserManager->CloseSlot( 3 );
		m_spRoomUserManager->CloseSlot( 6 );
		m_spRoomUserManager->CloseSlot( 7 );
#ifdef SERV_RELATIONSHIP_EVENT_INT
		m_spRoomUserManager->CloseSlot( 1 );
		m_spRoomUserManager->CloseSlot( 5 );
#endif SERV_RELATIONSHIP_EVENT_INT
	}
	//}}

    // ���Կ� ���� ��û�ڸ� �ִ´�.
    if( !m_spRoomUserManager->EnterRoom( kPacket_.m_kRoomUserInfo, true ) )
    {
        START_LOG( cerr, L"�� ���� ����." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << BUILD_LOG( GetUID() )
            << END_LOG;

        kPacket.m_iOK = NetError::ERR_ROOM_30;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_ROOM_ACK, kPacket );
        return;
    }

    // �� ���¸� wait �� �ٲ۴�.
    StateTransition( KRoomFSM::I_TO_WAIT );
    m_kTimer[TM_WAIT].restart();

	//gm �̻��̸� ���ͼ����� ip�� �־�����.
	//if( kPacket_.m_kUnitInfo.m_cAuthLevel >= SEnum::UAL_GM )
	if( m_spRoomUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == true )
	{
		kPacket.m_wstrCNIP = NetCommon::GetLocalIPW();
	}

    // �� ���� ��û�ڿ��� ack�� ������.
    kPacket.m_iOK = NetError::NET_OK;
    GetRoomInfo( kPacket.m_RoomInfo );
	GetRoomSlotInfo( kPacket.m_vecSlot );
	GetRoomSlotInfo( kPacket.m_vecObserverSlot, KRoomUserManager::UT_OBSERVER );

    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_ROOM_ACK, kPacket );

    START_LOG( clog2, L"���� ĳ����, ���� ����" )
        << BUILD_LOG( FIRST_SENDER_UID )
        << BUILD_LOG( LAST_SENDER_UID );

    // ��� ���� ������ �� ����Ʈ�� �����϶�� ������ ������.
    SendRoomListInfo( ( int )NetError::ERR_ROOM_14 );

	//{{ 2009. 4. 23  ������	������������Ʈ
	SendPVPUserInfo( kPacket_.m_kRoomUserInfo.m_nUnitUID );
	//}}

	// �� ���� ���� ����
	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( static_cast<int>(m_uiRoomListID) );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_PVP, kKey, KStatistics::SLP_CREATE_COUNT, 1 );

	//������� �������� üũ����.
	m_spRoomUserManager->ZU_CheckStart();
}

//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
_IMPL_ON_FUNC( ERM_OPEN_PVP_ROOM_FOR_MATCH_REQ, KEGB_OPEN_PVP_ROOM_FOR_MATCH_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_INIT ), ERM_OPEN_PVP_ROOM_FOR_MATCH_ACK, KEGB_OPEN_PVP_ROOM_FOR_MATCH_ACK );

	//{{ 2010. 03. 14  ������	�� �й� ��� ����
#ifdef SERV_ROOM_COUNT
	SendRoomCountInfo( CXSLRoom::RT_PVP, true );
#endif SERV_ROOM_COUNT
	//}}

	// �� �ɼ� ���� ����.
	SetRoomInfo( kPacket_.m_kRoomInfo );

	// ���� Ÿ��
	SetMatchType( static_cast<SEnum::MATCH_TYPE>(kPacket_.m_cMatchType) );

	// NPC���� ��� ���� ����
	SetPvpNpcType( static_cast<SEnum::PVP_NPC_TYPE>(kPacket_.m_cPvpNpcType) );

	// �� �����̹Ƿ� ���� ������ �ʱ�ȭ �Ѵ�.
	m_spRoomUserManager->Reset();
	//������ڸ� �濡 �����鼭 ����Ÿ�Կ� ���� ���ɼǼ����� ���ְ� �־��ֱ�����
	m_spRoomUserManager->AssignTeam( kPacket_.m_kRoomInfo.m_PVPGameType );

	// ���Կ� ���� ��û�ڸ� �ִ´�.
	if( m_spRoomUserManager->EnterRoom( kPacket_.m_vecRedTeam, kPacket_.m_vecBlueTeam, kPacket_.m_mapPvpNpcInfo, kPacket_.m_mapMatchWaitTime ) == false )
	{
		START_LOG( cerr, L"�� ���� ����." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ROOM_30;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_PVP_ROOM_FOR_MATCH_ACK, kPacket );
		return;
	}

	// ���� NPC���� ������Ʈ
	//{{ 2011. 10. 25	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_kPvpMonsterManager.SetPvpNpcInfo( kPacket_.m_mapPvpNpcInfo );
#else
	m_mapPvpNpcInfo = kPacket_.m_mapPvpNpcInfo;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	// �� ���¸� wait �� �ٲ۴�.
	StateTransition( KRoomFSM::I_TO_WAIT );
	m_kTimer[TM_WAIT].restart();

	//////////////////////////////////////////////////////////////////////////
	//{{ 2012. 05. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// 3�� ī��Ʈ
	TimeCountForStartGame( ERM_OPEN_PVP_ROOM_FOR_MATCH_REQ );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//////////////////////////////////////////////////////////////////////////

	// �� ���� ��û�ڿ��� ack�� ������.	
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_PVP_ROOM_FOR_MATCH_ACK, kPacket );

	START_LOG( clog2, L"���� ĳ����, ���� ����" )
		<< BUILD_LOG( FIRST_SENDER_UID )
		<< BUILD_LOG( LAST_SENDER_UID );

	// ��� ���� ������ �� ����Ʈ�� �����϶�� ������ ������.
	SendRoomListInfo( ( int )NetError::ERR_ROOM_14 );

	// �� ���� ���� ����
	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( static_cast<int>(m_uiRoomListID) );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_PVP, kKey, KStatistics::SLP_CREATE_COUNT, 1 );

	//������� �������� üũ����.
	m_spRoomUserManager->ZU_CheckStart();
}
#endif SERV_PVP_NEW_SYSTEM
//}}

IMPL_ON_FUNC( ERM_GAME_START_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_GAME_START_ACK, KERM_GAME_START_ACK );

	if( m_spRoomUserManager->IsObserver( FIRST_SENDER_UID ) == false )
	{
		// �������� üũ�Ѵ�.
		VERIFY_HOST( ERM_GAME_START_ACK );
	}

	//{{ 2009. 2. 24  ������	����ų�� �˻�
	if( GetPVPChannelClass() != KPVPChannelInfo::PCC_PLAY )
	{
		if( CXSLRoom::IsValidWinKillNum( m_cGameType, m_cWinningNumKill ) == false )
		{
			START_LOG( cerr, L"���� ���� ���۽� ų�������� �̻���." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOGc( m_cWinningNumKill )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ROOM_47;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_START_ACK, kPacket );
			return;
		}

		// �÷��� Ÿ�� �˻�
		if( CXSLRoom::IsValidPvpPlayTime( m_cGameType, static_cast<int>(GetPlayTimeLimit()) ) == false )
		{
			START_LOG( cerr, L"������ �÷��� Ÿ�� ���ð��� �̻��ϴ�." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOGc( m_cGameType )
				<< BUILD_LOG( GetPlayTimeLimit() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ROOM_30;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_START_ACK, kPacket );
			return;
		}
#ifdef SERV_RELATIONSHIP_EVENT_INT
		if( GetPVPChannelClass() == KPVPChannelInfo::PCC_TOURNAMENT && m_spRoomUserManager->CheckCouplePVP() == false )
		{
			kPacket.m_iOK = NetError::ERR_RELATIONSHIP_EVENT_07;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_START_ACK, kPacket );
			return;
		}
#endif SERV_RELATIONSHIP_EVENT_INT
	}	
	//}}

	//���ӽ��� �� ������
	switch( m_cGameType )
	{
	case CXSLRoom::PGT_TEAM_DEATH:
		{			
			if( m_spRoomUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == true /*&& KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_INTERNAL*/ )
			{	
				START_LOG( clog, L"������ �������ӽ���" )
					<< BUILD_LOG( FIRST_SENDER_UID );

				//{{ 2012. 11. 26	�ڼ���	������ ��� ����
#ifdef SERV_FIX_OBSERVE_MODE
				// ���� ����
				m_spRoomUserManager->SetAllReady( true );
#endif SERV_FIX_OBSERVE_MODE
				//}}
			}
			else
			{
				if( m_spRoomUserManager->GetNumReadyPlayer() < 2 )
				{
					kPacket.m_iOK = NetError::ERR_ROOM_12;
					SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_START_ACK, kPacket );
					return;
				}
			}

			// Ǯ ���� ��� ������ ���� ������ ���� �Ұ�� ������ �����ϸ� �ȵǱ� ������..
			if( m_spRoomUserManager->GetTeamReadyNum(CXSLRoom::TN_BLUE) < 1 ||
				m_spRoomUserManager->GetTeamReadyNum(CXSLRoom::TN_RED) < 1 )
			{
				kPacket.m_iOK = NetError::ERR_ROOM_12;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_START_ACK, kPacket );
				return;
			}

			DeathTeamBalance();

//#ifdef PVP_BOSS_COMBAT_TEST
//			m_spRoomUserManager->PickRandomBoss();
//#endif PVP_BOSS_COMBAT_TEST


		}
		break;

	case CXSLRoom::PGT_TEAM:
		{
			if( m_spRoomUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == true /*&& KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_INTERNAL*/ )
			{	
				START_LOG( clog, L"������ �������ӽ���" )
					<< BUILD_LOG( FIRST_SENDER_UID );

				//���� �÷���
				m_spRoomUserManager->SetAllReady( true );
			}
			else
			{

				//���ο����� ���ƾ� �����Ҽ��ִ�.
				int nRed, nBlue;
				if( m_spRoomUserManager->GetTeamNum( nRed, nBlue ) == false )
				{
					START_LOG( cerr, L"Team �ο� ������ ����.!" )
						<< END_LOG;
					return;
				}

				if( nRed != nBlue )
				{
					kPacket.m_iOK = NetError::ERR_ROOM_11;
					SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_START_ACK, kPacket );
					return;
				}
			}

            if( m_spRoomUserManager->IsAllPlayerReady() == false )
            {
                kPacket.m_iOK = NetError::ERR_ROOM_10;
                SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_START_ACK, kPacket );
                return;
            }
		}
		break;

	case CXSLRoom::PGT_SURVIVAL:
		{
			if( m_spRoomUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == true /*&& KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_INTERNAL*/ )
			{	
				START_LOG( clog, L"������ �������ӽ���" )
					<< BUILD_LOG( FIRST_SENDER_UID );
			}
			else
			{
				if( m_spRoomUserManager->GetNumReadyPlayer() < 2 )
				{
					kPacket.m_iOK = NetError::ERR_ROOM_12;
					SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_START_ACK, kPacket );
					return;
				}
			}
		}
		break;
	}

	// [�̺�Ʈ] 080811.hoons.��ȸ�Ⱓ�߿���(0207~0228) Ǯ���̾�� ������ �����Ҽ� �ִ�.
	//{{ 2010. 02. 10  ������	��ȸ ä��
	if( GetPVPChannelClass() == KPVPChannelInfo::PCC_TOURNAMENT )
	{
#ifdef SERV_RELATIONSHIP_EVENT_INT
		if( m_spRoomUserManager->GetNumReadyPlayer() != 2 )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_38;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_START_ACK, kPacket );
			return;
		}
#else
		if( m_spRoomUserManager->GetNumReadyPlayer() != 4  &&  m_spRoomUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_38;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_START_ACK, kPacket );
			return;
		}
#endif SERV_RELATIONSHIP_EVENT_INT
	}
	//}}

	//{{ 2008. 5. 27  ������  ������
	if( kPacket_.m_sWorldID == 0 )
	{
		START_LOG( cerr, L"�̻��� ������ ������ �Դ�." )
			<< BUILD_LOG( kPacket_.m_sWorldID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ROOM_36;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_START_ACK, kPacket );
		return;
	}

	// ���� ������ ������ �� ������ ����.
	m_sWorldID = kPacket_.m_sWorldID;
	//}}



	

    // ���� �ʱ�ȭ.
    StartGame();


    // ���忡�� ack�� ������.
    kPacket.m_iOK = NetError::NET_OK;
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_START_ACK, kPacket );

    // ���� ��� �����鿡�� not�� ������.    
	KERM_GAME_START_NOT kPacketNot;
    kPacketNot.m_cRoomState = GetStateID();
	kPacketNot.m_iGameType	= m_cGameType;
	kPacketNot.m_iDungeonID	= m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
	kPacketNot.m_sWorldID	= m_sWorldID;
    GetRoomSlotInfo( kPacketNot.m_vecSlot );
    BroadCast( ERM_GAME_START_NOT, kPacketNot );

    // ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
    SendRoomListInfo( NetError::ERR_ROOM_15 );

	//{{ 2009. 4. 23  ������	������������Ʈ	
	SendAllPVPUserInfo();
	//}}


}

//{{ 2012. 04. 06	������	��Ʋ�ʵ� �ý���
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( ERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_ROOM_ACK );
}

_IMPL_ON_FUNC( ERM_LEAVE_ROOM_FOR_JOIN_BATTLE_FIELD_REQ, KERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_ROOM_FOR_JOIN_BATTLE_FIELD_ACK );
}

_IMPL_ON_FUNC( ERM_LEAVE_ROOM_FOR_ENTER_THE_VILLAGE_REQ, KERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_ROOM_FOR_ENTER_THE_VILLAGE_ACK );
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( ERM_LEAVE_ROOM_REQ )
{
	// �ǵ����� Leave Room ���� ������ ������ �� �� �� �� Leave Room �ϰ� �Ǵ� ��찡 �ִ�.
	KEGS_LEAVE_ROOM_ACK kPacket;
	if( GetStateID() == KRoomFSM::S_INIT || GetStateID() == KRoomFSM::S_CLOSE )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_09;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_ROOM_ACK, kPacket );
		return;
	}

	// ��Ż�� ���.
	if( GetStateID() == KRoomFSM::S_LOAD || GetStateID() == KRoomFSM::S_PLAY )
	{
		//=== ������ ��Ż ���ó�� ===
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );

		if( !spRoomUser )
		{
			spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID, KRoomUserManager::UT_OBSERVER );
		}

		if( !spRoomUser )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
		}
		else
		{
			if( spRoomUser->IsObserver() == false )
			{
				KStatisticsKey kKey;
				kKey.m_vecIntKey.push_back( static_cast<int>(m_uiRoomListID) );
				KSIManager.IncreaseCount( KStatistics::SI_LOC_PVP, kKey, KStatistics::SLP_GIVE_UP, 1 );

				KERM_UPDATE_PVP_UNIT_INFO_NOT kPacket;
				//{{ 2009. 11. 16  ������	�����������Ʈ
				kPacket.m_iMemberCount = m_spRoomUserManager->GetNumMember();
				//}}
				kPacket.m_bOutRoom = true;
				//{{ 2011. 07. 22	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
				kPacket.m_bIsOfficialMatch = IsOfficialMatch();
				kPacket.m_iCurrentRating = spRoomUser->GetRating();
				kPacket.m_iCurrentMaxRating = spRoomUser->GetMaxRating();
				kPacket.m_iCurrentRPoint = spRoomUser->GetRPoint();
				kPacket.m_iCurrentAPoint = spRoomUser->GetAPoint();
#endif SERV_PVP_NEW_SYSTEM
				//}}

				//{{ 2012. 09. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
				kPacket.m_cRank				= spRoomUser->GetCurrentRank();
				kPacket.m_cRankForServer	= spRoomUser->GetCurrentRankForServer();
#endif SERV_2012_PVP_SEASON2
				//}}

				if( m_cGameType == CXSLRoom::PGT_TEAM )
				{
					//{{ 2011. 07. 25	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
					SiKResultProcess()->Result_TEAM_BREAK( m_spRoomUserManager, spRoomUser, kPacket_.m_iReason, GetMatchType(), GetPvpNpcType(), kPacket );
#else
					SiKResultProcess()->Result_TEAM_BREAK( m_spRoomUserManager, spRoomUser, kPacket );
#endif SERV_PVP_NEW_SYSTEM
					//}}

					//������ ��Ż�� �� ��ϵ�����
					kPacket.m_iLose		= 1;

					if( GetPVPChannelClass() == KPVPChannelInfo::PCC_FREE  || 
						//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
						GetPVPChannelClass() == KPVPChannelInfo::PCC_TOURNAMENT  || 
#endif SERV_PVP_NEW_SYSTEM
						//}}
						GetPVPChannelClass() == KPVPChannelInfo::PCC_PLAY )
					{
						kPacket.m_iEXP = 0;
						//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
						kPacket.m_iRating = 0;
						kPacket.m_iRPoint = 0;
						kPacket.m_iAPoint = 0;
#else
						kPacket.m_iVSPoint = 0;
#endif SERV_PVP_NEW_SYSTEM
						//}}
						kPacket.m_iLose	= 0;
					}

					SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_PVP_UNIT_INFO_NOT, kPacket );

					if( GetPVPChannelClass() != KPVPChannelInfo::PCC_FREE  && 
						//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
						GetPVPChannelClass() != KPVPChannelInfo::PCC_TOURNAMENT  && 
#endif SERV_PVP_NEW_SYSTEM
						//}}
						GetPVPChannelClass() != KPVPChannelInfo::PCC_PLAY )
					{
						//{{ 2007. 12. 17  ������  ���� ��� [����]
						KERM_PVP_USER_STATISTICS_NOT kPacketNot;
						kPacketNot.m_cGameType		 = m_cGameType;
						kPacketNot.m_cGameResult	 = KERM_PVP_USER_STATISTICS_NOT::UGR_DROP;
						kPacketNot.m_iGamePlayTime	 = static_cast<int>(spRoomUser->GetPlayTime());
						kPacketNot.m_bIntrude		 = spRoomUser->GetIsIntrude();
						kPacketNot.m_sWorldID		 = m_sWorldID;
						kPacketNot.m_iKillNum		 = m_spRoomUserManager->GetKillNumber( FIRST_SENDER_UID );
						SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PVP_USER_STATISTICS_NOT, kPacketNot );
						//}}
					}

					// PVP ���� �·� �α� [����� üũ]
					PvPLogToFile( spRoomUser, 0, 0, 2 );
					//////////////////////////////////////////////////////////////////////////
				}
				//������ & �����̹����� leave game���� �г�Ƽ�� lose�� ����� ������
				//������ ������ ��츸 ó�����ش�.
				else if( kPacket_.m_iReason	== NetError::NOT_LEAVE_ROOM_REASON_01  ||
						 kPacket_.m_iReason	== NetError::NOT_LEAVE_ROOM_REASON_23 )
				{
					switch( m_cGameType )
					{
					case CXSLRoom::PGT_TEAM_DEATH:
						//{{ 2011. 07. 25	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
						SiKResultProcess()->Result_TEAM_DEATH_BREAK( m_spRoomUserManager, spRoomUser, kPacket_.m_iReason, GetMatchType(), GetPvpNpcType(), kPacket );
#else
						SiKResultProcess()->Result_TEAM_DEATH_BREAK( m_spRoomUserManager, spRoomUser, kPacket );
#endif SERV_PVP_NEW_SYSTEM
						//}}
						break;

					case CXSLRoom::PGT_SURVIVAL:
						//{{ 2011. 07. 25	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
						SiKResultProcess()->Result_SURVIVAL_BREAK( m_spRoomUserManager, spRoomUser, kPacket_.m_iReason, GetMatchType(), GetPvpNpcType(), kPacket );
#else
						SiKResultProcess()->Result_SURVIVAL_BREAK( m_spRoomUserManager, spRoomUser, kPacket );
#endif SERV_PVP_NEW_SYSTEM
						//}}
						break;
					}

					//������ ��Ż�� �� ��ϵ�����
					kPacket.m_iLose		= 1;

					if( GetPVPChannelClass() == KPVPChannelInfo::PCC_FREE  || 
						//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
						GetPVPChannelClass() == KPVPChannelInfo::PCC_TOURNAMENT  || 
#endif SERV_PVP_NEW_SYSTEM
						//}}
						GetPVPChannelClass() == KPVPChannelInfo::PCC_PLAY )
					{
						kPacket.m_iEXP = 0;
						//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
						kPacket.m_iRating = 0;
						kPacket.m_iRPoint = 0;
						kPacket.m_iAPoint = 0;
#else
						kPacket.m_iVSPoint = 0;
#endif SERV_PVP_NEW_SYSTEM
						//}}
						kPacket.m_iLose	= 0;
					}

					SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_PVP_UNIT_INFO_NOT, kPacket );

					if( GetPVPChannelClass() != KPVPChannelInfo::PCC_FREE  && 
						//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
						GetPVPChannelClass() != KPVPChannelInfo::PCC_TOURNAMENT  && 
#endif SERV_PVP_NEW_SYSTEM
						//}}
						GetPVPChannelClass() != KPVPChannelInfo::PCC_PLAY )
					{
						//{{ 2007. 12. 17  ������  ���� ��� [����]
						KERM_PVP_USER_STATISTICS_NOT kPacketNot;
						kPacketNot.m_cGameType		 = m_cGameType;
						kPacketNot.m_cGameResult	 = KERM_PVP_USER_STATISTICS_NOT::UGR_DROP;
						kPacketNot.m_iGamePlayTime	 = static_cast<int>(spRoomUser->GetPlayTime());
						kPacketNot.m_bIntrude		 = spRoomUser->GetIsIntrude();
						kPacketNot.m_sWorldID		 = m_sWorldID;
						kPacketNot.m_iKillNum		 = m_spRoomUserManager->GetKillNumber( FIRST_SENDER_UID );
						SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PVP_USER_STATISTICS_NOT, kPacketNot );
						//}}
					}

					// PVP ���� �·� �α� [����� üũ]
					PvPLogToFile( spRoomUser, 0, 0, 2 );
					//////////////////////////////////////////////////////////////////////////
				}

				//{{ 2011. 07. 27	������	���� ����
				//#ifdef SERV_PVP_NEW_SYSTEM
				//				if( GetPVPChannelClass() == KPVPChannelInfo::PCC_OFFICIAL )
				//				{
				//					KDBE_PVP_MATCH_RESULT_LOG_NOT kPacketToPvpLog;
				//					KPvpMatchUserResultLog kPvpMatchUserLog;
				//					kPvpMatchUserLog.m_iUnitUID		= spRoomUser->GetCID();
				//					kPvpMatchUserLog.m_wstrNickName = spRoomUser->GetNickName();
				//					kPvpMatchUserLog.m_ucLevel		= spRoomUser->GetLevel();
				//					kPvpMatchUserLog.m_cUnitClass	= spRoomUser->GetUnitClass_LUA();
				//					kPvpMatchUserLog.m_iRating		= spRoomUser->GetRating();
				//					CXSLUnit::PVP_EMBLEM ePvpEmblem = CXSLUnit::GetPVPEmblem( spRoomUser->GetRating() );
				//					kPvpMatchUserLog.m_cPvpRank		= static_cast<char>(CXSLUnit::PvpEmblemToPvpRank( ePvpEmblem ));
				//					kPvpMatchUserLog.m_iRPoint		= spRoomUser->GetRPoint();
				//					kPvpMatchUserLog.m_iAPoint		= spRoomUser->GetAPoint();
				//					kPvpMatchUserLog.m_iOfficialMatchCnt = spRoomUser->GetOfficialMatchCount();
				//					kPvpMatchUserLog.m_cTeam		= spRoomUser->GetTeam();
				//					kPvpMatchUserLog.m_bPartyApp	= spRoomUser->IsPlayWithParty();
				//					kPvpMatchUserLog.m_iWaitTime	= spRoomUser->GetMatchWaitTime();
				//					kPvpMatchUserLog.m_iWin			= 2;
				//					kPvpMatchUserLog.m_iKill		= spRoomUser->GetNumKill();
				//					kPvpMatchUserLog.m_iAssist		= spRoomUser->GetNumMDKill();
				//					kPvpMatchUserLog.m_iDeath		= spRoomUser->GetNumDie();
				//					kPvpMatchUserLog.m_iNetRatingInc = 0;
				//					kPvpMatchUserLog.m_iNetRPInc	= kPacket.m_iRPoint;
				//					kPvpMatchUserLog.m_iNetAPInc	= kPacket.m_iAPoint;
				//					kPvpMatchUserLog.m_iEXPearned	= kPacket.m_iEXP;
				//					kPvpMatchUserLog.m_cEndType		= static_cast<char>(KPvpMatchUserResultLog::ET_LEAVE_SELF);
				//					kPacketToPvpLog.m_vecPvpMatchUserLog.push_back( kPvpMatchUserLog );
				//
				//					CTime tCurTime = CTime::GetCurrentTime();
				//					kPacketToPvpLog.m_cMatchType  = static_cast<char>(GetMatchType());
				//					kPacketToPvpLog.m_cNpcPvpType = static_cast<char>(GetPvpNpcType());
				//					kPacketToPvpLog.m_wstrRegDate = (const wchar_t*)tCurTime.Format(_T("%Y-%m-%d %H:%M:%S"));
				//					kPacketToPvpLog.m_iPlayTime	  = static_cast<int>(spRoomUser->GetPlayTime());
				//					SendToLogDB( DBE_PVP_MATCH_RESULT_LOG_NOT, kPacketToPvpLog );
				//				}
				//#endif SERV_PVP_NEW_SYSTEM
				//}}
			}
		}
	}

	kPacket.m_iOK = NetError::NET_OK;
	if( !m_spRoomUserManager->LeaveRoom( FIRST_SENDER_UID ) )
	{
		START_LOG( cerr, L"�� ������ ����." );
		kPacket.m_iOK = NetError::ERR_SLOT_04;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_ROOM_ACK, kPacket );
		return;
	}
	else
	{
		DelRebirth( FIRST_SENDER_UID );

		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_ROOM_ACK, kPacket );

		//{{ 2009. 4. 23  ������	������������Ʈ
		SendPVPUserInfo( FIRST_SENDER_UID, KCommunityUserInfo::US_PVP_LOBBY );
		//}}

		//{{ 2012. 10. 16	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
		LogToDB_GamePlayNetWorkInfo( FIRST_SENDER_UID, kPacket_.m_kGamePlayNetworkInfo );
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
		//}}
	}

	if( IsEmpty() == false || IsEmpty( KRoomUserManager::UT_OBSERVER ) == false )
	{
		// ���� ��� �����鿡�� ���� ������ ������.
		KEGS_LEAVE_ROOM_NOT kPacketNot;
		kPacketNot.m_cRoomState = GetStateID();
		kPacketNot.m_iUnitUID = FIRST_SENDER_UID;
		kPacketNot.m_iReason = kPacket_.m_iReason;
		GetRoomSlotInfo( kPacketNot.m_vecSlot );
		GetRoomSlotInfo( kPacketNot.m_vecObserverSlot, KRoomUserManager::UT_OBSERVER );
		BroadCast( ERM_LEAVE_ROOM_NOT, kPacketNot );
	}

	if( GetStateID() == KRoomFSM::S_LOAD && IsEmpty() == false )
	{
		//{{ 2011. 08. 05	������	�����濡 ���� ȥ�� ���ԵǴ� ���� ����
#ifdef SERV_CODE_REFACTORING_20110804
		CheckAllPlayerFinishLoading();
#else
		// ��� ������ �ε� �������� not�� ������.
		if( m_spRoomUserManager->IsAllPlayerFinishLoading() && IsEmpty() == false )
		{
			BroadCastID( ERM_GAME_LOADING_ALL_UNIT_OK_NOT );

			// ���������� �ʱ�ȭ.
			StartPlay();

			KEGS_PLAY_START_NOT kNot;
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

	// ���������� ������ �Ǵ� ��� ó��.
	if( GetStateID() == KRoomFSM::S_PLAY && IsEmpty() == false )
	{
		if( CheckIfPlayEnd() )
		{
			EndPlay();
		}

		//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		// ���� ������ ���
		if( GetPVPChannelClass() == KPVPChannelInfo::PCC_OFFICIAL )
		{
			// �÷��� Ÿ���� 30�� �̸��̸�..?
			const int iPlayTime = ( int )( m_kTimer[TM_PLAY_WITHOUT_LOADING].elapsed() + 0.5 );
			if( iPlayTime < SEnum::PE_PVP_OFFICIAL_MIN_PLAY_TIME )
			{
				// ���� ���� ���!
				m_bOfficialPvpCancel = true;

				// �÷��̸� �����Ѵ�!
				EndPlay();
			}
		}
#endif SERV_PVP_NEW_SYSTEM
		//}}
	}

	//���â�� ���������� ���� �������ó��
	if( GetStateID() == KRoomFSM::S_RESULT && IsEmpty() == false )
	{
		CheckResultSuccess();
	}

	// �濡�� ��� ������ �������� ���� �ݴ´�.
	if( IsEmpty() == true && IsEmpty( KRoomUserManager::UT_OBSERVER ) == true )
	{
		StateTransition( KRoomFSM::I_TO_CLOSE );
	}

	// ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
	if( GetStateID() == KRoomFSM::S_CLOSE )
	{
		SendRoomListInfo( NetError::ERR_ROOM_16 );

		//{{ 2009. 4. 23  ������	������������Ʈ
		SendAllPVPUserInfo(); // �����ִ� ����� �����ٵ� ���� �ȳ�����..
		//}}
	}
	else
	{
		SendRoomListInfo( NetError::ERR_ROOM_15 );

		//{{ 2009. 4. 23  ������	������������Ʈ
		SendAllPVPUserInfo(); // ���� RoomFSM�� ���� ������ ���¸� ������Ʈ
		//}}
	}
}


//{{ 2012. 10. 17	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO   
_IMPL_ON_FUNC( ERM_RESULT_SUCCESS_REQ, KEGS_RESULT_SUCCESS_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_RESULT ), ERM_RESULT_SUCCESS_ACK, KEGS_RESULT_SUCCESS_ACK );

	if( m_spRoomUserManager->SetSuccessResult( FIRST_SENDER_UID, true ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_17;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_RESULT_SUCCESS_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_RESULT_SUCCESS_ACK, kPacket );

	//{{ 2012. 10. 16	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	if( GetPVPChannelClass() == KPVPChannelInfo::PCC_OFFICIAL )
	{
		LogToDB_GamePlayNetWorkInfo( FIRST_SENDER_UID, kPacket_.m_kGamePlayNetworkInfo );
	}
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	//}}

	//��� ������ ������ ���â Ȯ���� ��������
	if( m_spRoomUserManager->IsAllPlayerSuccessResult() == true )
	{
		EndGame();

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

		KEGS_RESULT_ALL_UNIT_SUCCESS_NOT kNot;
		GetRoomInfo( kNot.m_RoomInfo );
		GetRoomSlotInfo( kNot.m_vecSlot );

		//���ó������ ������ ���� �濡 ������� �������Ա��� ��Ŷ�� ������.
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
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
//}}


//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}

//{{ 2011. 07. 25	������	���� ����
//////////////////////////////////////////////////////////////////////////
//#ifdef SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////

//{{ 2011. 10. 25	������	��Ʋ�ʵ� �ý���
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////

_IMPL_ON_FUNC( ERM_NPC_UNIT_CREATE_REQ, KEGS_NPC_UNIT_CREATE_REQ )
{
	VERIFY_ROOM_STATE_AND_WARN( ( 1, KRoomFSM::S_PLAY ), (1, KRoomFSM::S_RESULT), ERM_NPC_UNIT_CREATE_ACK, KPacketOK );

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_CREATE_ACK, kPacket );

	KEGS_NPC_UNIT_CREATE_NOT kPacketNot;

	BOOST_TEST_FOREACH( const KNPCUnitReq&, kNpcUnitInfo, kPacket_.m_vecNPCUnitReq )
	{
#ifdef NOT_CREATE_NPC_EXCEPT_FOR_SPECIFIC_NPC
		/// ���� ���� �̰ų�
		/// �Ʊ� �� �Ʒ��� ����Ʈ NPC 
		/// �Ʊ� �� PVP_NPC_KEY_CODE �� ������ PC�� �θ� �� ����
		if ( !kNpcUnitInfo.m_bAllyNpc ||
			  CXSLUnitManager::CanCreateThisNpc( kNpcUnitInfo.m_NPCID ) ||
			  kNpcUnitInfo.m_KeyCode == PVP_NPC_KEY_CODE )
#endif // NOT_CREATE_NPC_EXCEPT_FOR_SPECIFIC_NPC
		{

			KNPCUnitNot kNPCInfo;
			kNPCInfo.m_kNPCUnitReq = kNpcUnitInfo;

			if( SiKPvpMatchManager()->IsPvpNpc( kNpcUnitInfo.m_NPCID ) == true  &&  IsOfficialMatch() == true )
			{
				if( m_kPvpMonsterManager.CreatePvpMonster( kNpcUnitInfo, kNPCInfo.m_kNPCUnitReq.m_UID ) == false )
				{
					START_LOG( cerr, L"���� NPC���� ����!" )
						<< BUILD_LOG( kNPCInfo.m_kNPCUnitReq.m_UID )
						<< END_LOG;
					continue;
				}	
			}
			else
			{
				LIF( m_kPvpMonsterManager.CreateMonster( kNpcUnitInfo, kNPCInfo.m_kNPCUnitReq.m_UID ) );
			}

			kPacketNot.m_vecNPCUnitAck.push_back( kNPCInfo );		

		}

	}

	BroadCast( ERM_NPC_UNIT_CREATE_NOT, kPacketNot );
}

_IMPL_ON_FUNC( ERM_NPC_UNIT_DIE_REQ, KEGS_NPC_UNIT_DIE_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_NPC_UNIT_DIE_ACK, KERM_NPC_UNIT_DIE_ACK );

	//������ ���� ����Ʈ���� ã�´�.
	KRoomMonsterManager::NPC_DATA kDieNpcInfo;
	if( m_kPvpMonsterManager.GetNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_20;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
		return;
	}

	//�ִٸ� �̹� �׾����� üũ..
	if( m_kPvpMonsterManager.IsMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_21;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
		return;
	}

	//���� ����ִٸ� ���λ���� ���������� ����..
	LIF( m_kPvpMonsterManager.SetMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );

	kPacket.m_iOK		= NetError::NET_OK;
	kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );

	//����� �ο����� �˷��� ������ ���� ����..
	KERM_NPC_UNIT_DIE_NOT kNPCDieNot;
	kNPCDieNot.m_iDungeonID	= 0; //m_iDungeonID;
	kNPCDieNot.m_cDifficulty	= 0; //m_cDifficultyLevel;

	kNPCDieNot.m_nDieNPCUID	= kPacket_.m_nDieNPCUID;
	kNPCDieNot.m_iNPCID		= kDieNpcInfo.m_iNPCID;
	kNPCDieNot.m_cAttUnitType	= kPacket_.m_cAttUnitType;
	kNPCDieNot.m_uiAttUnit		= kPacket_.m_uiAttUnit;
	kNPCDieNot.m_bNoDrop		= true;
	//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	//{{ 2012. 02. 22	��μ�	���� ����Ʈ, PVP NPC ���� ����Ʈ	���� ����
#ifdef SERV_NEW_PVP_QUEST_ERROR_MODIFY
	kNPCDieNot.m_iGameType		= GetPlayMode();
#endif SERV_NEW_PVP_QUEST_ERROR_MODIFY
	//}}
	kNPCDieNot.m_bIsPvpNpc		= m_spRoomUserManager->IsPvpNpc( kPacket_.m_nDieNPCUID );
#endif SERV_PVP_NEW_SYSTEM
	//}}
	kNPCDieNot.m_bQuestComplete = true;

#ifdef PVP_QUEST_HERO_KILL_COUNT
	kNPCDieNot.m_bHeroNPC		= GetPvpNpcType() == SEnum::PNT_HERO_NPC ? true : false ;
#endif //PVP_QUEST_HERO_KILL_COUNT

	BroadCast( ERM_NPC_UNIT_DIE_NOT, kNPCDieNot );

	//////////////////////////////////////////////////////////////////////////
	// ���� �����̸�? ���� NPC�� üũ����!
	if( IsOfficialMatch() == true )
	{
		const UidType iNpcUnitUID = kPacket_.m_nDieNPCUID;
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iNpcUnitUID );
		if( spRoomUser != NULL )
		{
			// Kill �� ����.
			const bool bIncreaseNumKill = m_spRoomUserManager->IncreaseNumKill( kPacket_.m_uiAttUnit );
			if( bIncreaseNumKill == false )
			{
				kPacket.m_iOK = NetError::ERR_SLOT_07;

				START_LOG( cwarn, L"IncreaseNumKill() fail" )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( GetName() )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( NetError::GetErrStr( NetError::ERR_SLOT_07 ) )
					<< END_LOG;
			}

			// MDKill �� ����.
			const bool bIncreaseNumMDKill = m_spRoomUserManager->IncreaseNumMDKill( kPacket_.m_uiMDAttUnit );
			if( bIncreaseNumMDKill == false )
			{
				kPacket.m_iOK = NetError::ERR_SLOT_07;

				START_LOG( cwarn, L"IncreaseNumMDKill() fail" )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( GetName() )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( NetError::GetErrStr( NetError::ERR_SLOT_07 ) )
					<< END_LOG;
			}

			// Die �� ����.
			const bool bIncreaseNumDie = m_spRoomUserManager->IncreaseNumDie( iNpcUnitUID );
			if( bIncreaseNumDie == false )
			{
				START_LOG( cwarn, L"IncreaseNumDie() fail" )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( GetName() )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( NetError::GetErrStr( NetError::ERR_SLOT_07 ) )
					<< END_LOG;

				return;
			}

			// �׾��ٰ� ǥ��.
			m_spRoomUserManager->SetDie( iNpcUnitUID, true );

			// ��Ȱ ����Ʈ�� �ִ´�.
			KRebirthInfo kRebirthInfo;
			kRebirthInfo.nCID = iNpcUnitUID;
			m_spRoomUserManager->GetRebirthPos( iNpcUnitUID, kRebirthInfo.iPosition );
			kRebirthInfo.fDelay = 12.f;
			kRebirthInfo.kTimer.restart();
			//{{ 2012. 11. 27 PVP���� ������ ���� ���� ��Ȱ ���� ���� - ��μ�
#ifdef SERV_PVP_REBIRTH_ADD_CONDITION
			kRebirthInfo.bReady = true;		// NPC �� �ٷ� ��Ȱ ����
#endif SERV_PVP_REBIRTH_ADD_CONDITION
			//}
			m_vecRebirthInfo.push_back( kRebirthInfo );

			//���γ��� ���� ������ ��쿡 ������ ���� ��ų���� �ø���. �ȱ׷��� ������ �� �����Ƿ�
			if( bIncreaseNumKill == false && m_cGameType == CXSLRoom::PGT_TEAM )
			{
				KRoomUserPtr pkKilledUser = m_spRoomUserManager->GetUser( iNpcUnitUID );
				if( pkKilledUser != NULL && pkKilledUser->IsPlaying() == true )
				{
					CXSLRoom::TEAM_NUM teamNum = (CXSLRoom::TEAM_NUM)pkKilledUser->GetTeam();
					if( teamNum == CXSLRoom::TN_RED )
					{
						teamNum = CXSLRoom::TN_BLUE;
					}
					else
					{
						teamNum = CXSLRoom::TN_RED;
					}
					m_spRoomUserManager->IncreaseTeamNumKillByTeamNum( teamNum );
				}
			}
			else
			{
				m_spRoomUserManager->IncreaseTeamNumKill( kPacket_.m_uiAttUnit );
			}

			// ���� ��� �������� ������ �˸���.
			KERM_USER_UNIT_DIE_NOT kPacketNot;
			kPacketNot.m_iGameType			= m_cGameType;
			if( bIncreaseNumKill == true && bIncreaseNumMDKill == true )
			{
				kPacketNot.m_KillerUserUnitUID			= kPacket_.m_uiAttUnit;
				kPacketNot.m_MaxDamageKillerUserUnitUID = kPacket_.m_uiMDAttUnit;
				kPacketNot.m_KilledUserUnitUID			= iNpcUnitUID;
#ifdef PVP_QUEST_HERO_KILL_COUNT
				kPacketNot.m_bHeroNPC					= GetPvpNpcType() == SEnum::PNT_HERO_NPC ? true : false ;
#endif //PVP_QUEST_HERO_KILL_COUNT
			}
			else
			{
				kPacketNot.m_KillerUserUnitUID			= -1;
				kPacketNot.m_MaxDamageKillerUserUnitUID = -1;
				kPacketNot.m_KilledUserUnitUID			= iNpcUnitUID;
			}
			BroadCast( ERM_USER_UNIT_DIE_NOT, kPacketNot );

			if( GetStateID() == KRoomFSM::S_PLAY )
			{
				if( m_cGameType == CXSLRoom::PGT_SURVIVAL )
				{
					KEGS_CURRENT_KILL_SCORE_NOT kPacket2;
					m_spRoomUserManager->GetCurrentKillScore( kPacket2.m_mapKillScore, kPacket2.m_mapMDKillScore );
					BroadCast( ERM_CURRENT_KILL_SCORE_NOT, kPacket2 );
				}

				if( m_cGameType == CXSLRoom::PGT_TEAM_DEATH )
				{
					KEGS_CURRENT_KILL_SCORE_NOT kPacket2;
					m_spRoomUserManager->GetCurrentKillScore( kPacket2.m_mapKillScore, kPacket2.m_mapMDKillScore );
					BroadCast( ERM_CURRENT_KILL_SCORE_NOT, kPacket2 );

					KEGS_CURRENT_TEAM_KILL_SCORE_NOT kPacket3;
					kPacket3.m_RedTeamKill = m_spRoomUserManager->GetTeamScore( CXSLRoom::TN_RED );
					kPacket3.m_BlueTeamKill = m_spRoomUserManager->GetTeamScore( CXSLRoom::TN_BLUE );
					BroadCast( ERM_CURRENT_TEAM_KILL_SCORE_NOT, kPacket3 );
				}
			}

			// ���а� ���� ������ ���带 ������.
			if( CheckIfPlayEnd() )
			{
				ReserveEndPlay();
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}

//////////////////////////////////////////////////////////////////////////
//#else
//////////////////////////////////////////////////////////////////////////

//_IMPL_ON_FUNC( ERM_NPC_UNIT_CREATE_REQ, KEGS_NPC_UNIT_CREATE_REQ )
//{
//	VERIFY_ROOM_STATE_AND_WARN( ( 1, KRoomFSM::S_PLAY ), (1, KRoomFSM::S_RESULT), ERM_NPC_UNIT_CREATE_ACK, KPacketOK );
//
//	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_CREATE_ACK, kPacket );
//
//	KNPCUnitNot kNPCInfo;
//	KEGS_NPC_UNIT_CREATE_NOT kPacketNot;
//	NPC_DATA npcData;
//	for( int i = 0; i < (int)kPacket_.m_vecNPCUnitReq.size(); ++i )
//	{
//		kNPCInfo.m_kNPCUnitReq			= kPacket_.m_vecNPCUnitReq[i];
//		kNPCInfo.m_kNPCUnitReq.m_UID	= m_nNPCUID++;
//
//		kPacketNot.m_vecNPCUnitAck.push_back( kNPCInfo );
//
//		//NPC(Monster) ������ ���� data ����
//		npcData.m_bDie		= false;
//		npcData.m_cLevel	= kPacket_.m_vecNPCUnitReq[i].m_Level;
//		npcData.m_iNPCID	= kPacket_.m_vecNPCUnitReq[i].m_NPCID;
//		npcData.m_bNoDrop	= kPacket_.m_vecNPCUnitReq[i].m_bNoDrop;
//		npcData.m_bActive	= kPacket_.m_vecNPCUnitReq[i].m_bActive;
//		npcData.m_bIsBoss	= kPacket_.m_vecNPCUnitReq[i].m_bHasBossGage;
//
//		m_mapNPCData.insert( std::make_pair( kNPCInfo.m_kNPCUnitReq.m_UID, npcData ) );
//
//
//
//
//#ifdef SERVER_PVP_BASE_DEFENCE_TEST
//
//		if( true == m_PVPGameOption.m_bBaseDefence )
//		{
//			switch( kNPCInfo.m_kNPCUnitReq.m_cTeamNum )
//			{
//			case CXSLRoom::TN_RED:
//				{
//					if( m_PVPGameOption.m_BaseDefenceData.m_StatueOfRedUID <= 0 )
//					{
//						m_PVPGameOption.m_BaseDefenceData.m_StatueOfRedUID = kNPCInfo.m_kNPCUnitReq.m_UID;
//					}
//				} break;
//
//			case CXSLRoom::TN_BLUE:
//				{
//					if( m_PVPGameOption.m_BaseDefenceData.m_StatueOfBlueUID <= 0 )
//					{
//						m_PVPGameOption.m_BaseDefenceData.m_StatueOfBlueUID = kNPCInfo.m_kNPCUnitReq.m_UID;
//					}
//				} break;
//			}
//		}
//
//#endif SERVER_PVP_BASE_DEFENCE_TEST
//
//	}
//
//
//
//
//	BroadCast( ERM_NPC_UNIT_CREATE_NOT, kPacketNot );
//}
//
//_IMPL_ON_FUNC( ERM_NPC_UNIT_DIE_REQ, KEGS_NPC_UNIT_DIE_REQ )
//{
//	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_NPC_UNIT_DIE_ACK, KERM_NPC_UNIT_DIE_ACK );
//
//	//������ ���� ����Ʈ���� ã�´�.
//	std::map<int, NPC_DATA>::iterator mit;
//	mit = m_mapNPCData.find( kPacket_.m_nDieNPCUID );
//
//	//���� ���..
//	if( mit == m_mapNPCData.end() )
//	{
//		kPacket.m_iOK = NetError::ERR_ROOM_20;
//		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
//
//		return;
//	}
//	//�ִٸ� �̹� �׾����� üũ..
//	if( mit->second.m_bDie == true )
//	{
//		kPacket.m_iOK = NetError::ERR_ROOM_21;
//		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
//
//		return;
//	}
//
//	//���� ����ִٸ� ���λ���� ���������� ����..
//	mit->second.m_bDie		= true;
//	mit->second.m_uiAttUnit	= kPacket_.m_uiAttUnit;
//
//	kPacket.m_iOK		= NetError::NET_OK;
//	kPacket.m_iNPCID	= mit->second.m_iNPCID;
//	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
//
//	//����� �ο����� �˷��� ������ ���� ����..
//	KERM_NPC_UNIT_DIE_NOT kNPCDieNot;
//	kNPCDieNot.m_iDungeonID	= 0; //m_iDungeonID;
//	kNPCDieNot.m_cDifficulty	= 0; //m_cDifficultyLevel;
//
//	kNPCDieNot.m_nDieNPCUID	= kPacket_.m_nDieNPCUID;
//	kNPCDieNot.m_iNPCID		= mit->second.m_iNPCID;
//	kNPCDieNot.m_cAttUnitType	= kPacket_.m_cAttUnitType;
//	kNPCDieNot.m_uiAttUnit		= kPacket_.m_uiAttUnit;
//	kNPCDieNot.m_bNoDrop		= true;
//
//
//	BroadCast( ERM_NPC_UNIT_DIE_NOT, kNPCDieNot );
//
//
//
//
//
//#ifdef SERVER_PVP_BASE_DEFENCE_TEST
//
//	if( true == m_PVPGameOption.m_bBaseDefence )
//	{
//		if( m_PVPGameOption.m_BaseDefenceData.m_StatueOfRedUID == kPacket_.m_nDieNPCUID )
//		{
//			m_PVPGameOption.m_BaseDefenceData.m_bStatueOfRedDead = true;
//		}
//		else if( m_PVPGameOption.m_BaseDefenceData.m_StatueOfBlueUID == kPacket_.m_nDieNPCUID )
//		{
//			m_PVPGameOption.m_BaseDefenceData.m_bStatueOfBlueDead = true;
//		}
//	}
//
//
//	// ���а� ���� ������ ���带 ������.
//	if( CheckIfPlayEnd() )
//	{
//		ReserveEndPlay();
//	}
//
//#endif SERVER_PVP_BASE_DEFENCE_TEST
//
//
//}

//////////////////////////////////////////////////////////////////////////
//#endif SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}


//}} dmlee 2008.11.13 PVP monster

//{{ 2009. 7. 20  ������	��������	
_IMPL_ON_FUNC( ERM_CHANGE_SLOT_OPEN_REQ, KEGS_CHANGE_SLOT_OPEN_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_CHANGE_SLOT_OPEN_ACK, KEGS_CHANGE_SLOT_OPEN_ACK );

	if( m_spRoomUserManager->IsObserver( FIRST_SENDER_UID ) == false )
	{
		// �������� üũ�Ѵ�.
		VERIFY_HOST( ERM_CHANGE_SLOT_OPEN_ACK );
	}
	
	//{{ 2010. 02. 10  ������	��ȸ ä�ο����� ���� ���� ���� �Ұ�		
	if( GetPVPChannelClass() == KPVPChannelInfo::PCC_TOURNAMENT )
	{
		kPacket.m_iOK = NetError::ERR_SLOT_02;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_SLOT_OPEN_ACK, kPacket );
		return;
	}
	//}}

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
//}}

//{{ 2011. 07. 13	������	���� ����
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////

_IMPL_ON_FUNC( ERM_USER_UNIT_DIE_REQ, KEGS_USER_UNIT_DIE_REQ )
{
	//{{ 2011. 09. 19  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_USER_UNIT_DIE_ACK, KERM_USER_UNIT_DIE_ACK );
#else
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_USER_UNIT_DIE_ACK, KEGS_USER_UNIT_DIE_ACK );
#endif SERV_ADD_TITLE_CONDITION
	//}}

	//{{ 2012. 07. 12	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2_2
#else
	float fRebirthTime = 12.f;
	if( m_spRoomUserManager->IsRingofpvprebirth( FIRST_SENDER_UID ) == true )
	{
		fRebirthTime = 6.f; 
	}
#endif SERV_2012_PVP_SEASON2_2
	//}}

	kPacket.m_iOK = NetError::NET_OK;
	// Kill �� ����.
	const bool bIncreaseNumKill = m_spRoomUserManager->IncreaseNumKill( kPacket_.m_KillerUserUnitUID );
	if( bIncreaseNumKill == false )
	{
		kPacket.m_iOK = NetError::ERR_SLOT_07;

		START_LOG( cwarn, L"IncreaseNumKill() fail" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( NetError::GetErrStr( NetError::ERR_SLOT_07 ) )
			<< END_LOG;
	}

	// MDKill �� ����.
	const bool bIncreaseNumMDKill = m_spRoomUserManager->IncreaseNumMDKill( kPacket_.m_MaxDamageKillerUserUnitUID );
	if( bIncreaseNumMDKill == false )
	{
		kPacket.m_iOK = NetError::ERR_SLOT_07;

		START_LOG( cwarn, L"IncreaseNumMDKill() fail" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( NetError::GetErrStr( NetError::ERR_SLOT_07 ) )
			<< END_LOG;
	}

	// Die �� ����.
	const bool bIncreaseNumDie = m_spRoomUserManager->IncreaseNumDie( FIRST_SENDER_UID );
	if( bIncreaseNumDie == false )
	{
		kPacket.m_iOK = NetError::ERR_SLOT_07;
		//{{ 2011. 09. 19  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
		KERM_USER_UNIT_DIE_ACK kAck;
		kAck.m_iOK = kPacket.m_iOK;
		kAck.m_ucDieReason = kPacket_.m_ucDieReason;
		kAck.m_fReBirthTime	= kPacket.m_fReBirthTime;
		kAck.m_VP			= kPacket.m_VP;
		kAck.m_EXP			= kPacket.m_EXP;;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_USER_UNIT_DIE_ACK, kAck );
#else
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_USER_UNIT_DIE_ACK, kPacket );
#endif SERV_ADD_TITLE_CONDITION
		//}}

		START_LOG( cwarn, L"IncreaseNumDie() fail" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( NetError::GetErrStr( NetError::ERR_SLOT_07 ) )
			<< END_LOG;

		return;
	}

	// �׾��ٰ� ǥ��.
	m_spRoomUserManager->SetDie( FIRST_SENDER_UID, true );

	//{{ 2012. 07. 12	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2_2
	
	// ����� ��ü ų ���� �����ϱ� ���� �ӽ÷� �����մϴ�.
	int iPrevMyTeamKillNumGab = m_spRoomUserManager->GetMyTeamKillNum( FIRST_SENDER_UID ) - m_spRoomUserManager->GetEnemyTeamKillNum( FIRST_SENDER_UID );

	//���γ��� ���� ������ ��쿡 ������ ���� ��ų���� �ø���. �ȱ׷��� ������ �� �����Ƿ�
	if( bIncreaseNumKill == false && m_cGameType == CXSLRoom::PGT_TEAM )
	{
		KRoomUserPtr pkKilledUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
		if( pkKilledUser != NULL && pkKilledUser->IsPlaying() == true )
		{
			CXSLRoom::TEAM_NUM teamNum = (CXSLRoom::TEAM_NUM)pkKilledUser->GetTeam();
			if( teamNum == CXSLRoom::TN_RED )
			{
				teamNum = CXSLRoom::TN_BLUE;
			}
			else
			{
				teamNum = CXSLRoom::TN_RED;
			}
			m_spRoomUserManager->IncreaseTeamNumKillByTeamNum( teamNum );
		}
	}
	else
	{
		m_spRoomUserManager->IncreaseTeamNumKill( kPacket_.m_KillerUserUnitUID );
	}

	// �츮�� ��ü ų ���� ������� ��ü ų ���� ���̸� ���մϴ�.
	int iNowMyTeamKillNumGab = m_spRoomUserManager->GetMyTeamKillNum( FIRST_SENDER_UID ) - m_spRoomUserManager->GetEnemyTeamKillNum( FIRST_SENDER_UID );

#ifdef SERV_PVP_REMATCH
	if ( IsOfficialMatch() == true && GetMatchType() == SEnum::MT_TEAM_1_ON_1 )
	{
		// 2013.05.14 lygan_������ // ���� ���� 1:1 ��Ī���� ������ ���� ������ ���� ���ܼ� �̷��� ����ó���� ��.
	}
	else
	{
		// ������ ���� ó��
		RevengeBuffProcess( FIRST_SENDER_UID, iPrevMyTeamKillNumGab, iNowMyTeamKillNumGab );
	}
#else //SERV_PVP_REMATCH
	// ������ ���� ó��
	RevengeBuffProcess( FIRST_SENDER_UID, iPrevMyTeamKillNumGab, iNowMyTeamKillNumGab );
#endif//SERV_PVP_REMATCH

	// ������ Ÿ�� ���
	float fRebirthTime = SiKDropTable()->CalculateRebirthTime( m_spRoomUserManager->GetMyTeamKillNum( FIRST_SENDER_UID )
															 , m_spRoomUserManager->GetEnemyTeamKillNum( FIRST_SENDER_UID )
															 , m_spRoomUserManager->IsRingofpvprebirth( FIRST_SENDER_UID ) );
	// ��Ȱ ����Ʈ�� �ִ´�.
	KRebirthInfo kRebirthInfo;
	kRebirthInfo.nCID = FIRST_SENDER_UID;
	m_spRoomUserManager->GetRebirthPos( FIRST_SENDER_UID, kRebirthInfo.iPosition );
	kRebirthInfo.fDelay = fRebirthTime;
	kRebirthInfo.kTimer.restart();
	//{{ 2012. 11. 27 PVP���� ������ ���� ���� ��Ȱ ���� ���� - ��μ�
#ifdef SERV_PVP_REBIRTH_ADD_CONDITION
	kRebirthInfo.bReady = false;			// USER_UNIT_DIE_COMPLETE �� ������ true �� �Ѵ�.
#endif SERV_PVP_REBIRTH_ADD_CONDITION
	//}
	m_vecRebirthInfo.push_back( kRebirthInfo );
#else
	// ��Ȱ ����Ʈ�� �ִ´�.
	KRebirthInfo kRebirthInfo;
	kRebirthInfo.nCID = FIRST_SENDER_UID;
	m_spRoomUserManager->GetRebirthPos( FIRST_SENDER_UID, kRebirthInfo.iPosition );
	kRebirthInfo.fDelay = fRebirthTime;
	kRebirthInfo.kTimer.restart();
	//{{ 2012. 11. 27 PVP���� ������ ���� ���� ��Ȱ ���� ���� - ��μ�
#ifdef SERV_PVP_REBIRTH_ADD_CONDITION
	kRebirthInfo.bReady = false;			// USER_UNIT_DIE_COMPLETE �� ������ true �� �Ѵ�.
#endif SERV_PVP_REBIRTH_ADD_CONDITION
	//}
	m_vecRebirthInfo.push_back( kRebirthInfo );

	//���γ��� ���� ������ ��쿡 ������ ���� ��ų���� �ø���. �ȱ׷��� ������ �� �����Ƿ�
	if( bIncreaseNumKill == false && m_cGameType == CXSLRoom::PGT_TEAM )
	{
		KRoomUserPtr pkKilledUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
		if( pkKilledUser != NULL && pkKilledUser->IsPlaying() == true )
		{
			CXSLRoom::TEAM_NUM teamNum = (CXSLRoom::TEAM_NUM)pkKilledUser->GetTeam();
			if( teamNum == CXSLRoom::TN_RED )
			{
				teamNum = CXSLRoom::TN_BLUE;
			}
			else
			{
				teamNum = CXSLRoom::TN_RED;
			}
			m_spRoomUserManager->IncreaseTeamNumKillByTeamNum( teamNum );
		}
	}
	else
	{
		m_spRoomUserManager->IncreaseTeamNumKill( kPacket_.m_KillerUserUnitUID );
	}
#endif SERV_2012_PVP_SEASON2_2
	//}}

	KRoomUserPtr pkKillerUser;
	KRoomUserPtr pkKillerUserMD;
	KRoomUserPtr pkKilledUser;
	//const KPvpMatchResultTable::SResultData* pResultDataKill		= NULL;
	//const KPvpMatchResultTable::SResultData* pResultDataDeath		= NULL;

	//���̴� ���� ���� ���� ED, VP�� ������
	if( GetStateID() == KRoomFSM::S_PLAY && bIncreaseNumKill == true && bIncreaseNumMDKill == true )
	{
		//������ġ �迭������ �Ѵ�
		if( m_cGameType == CXSLRoom::PGT_SURVIVAL || m_cGameType == CXSLRoom::PGT_TEAM_DEATH )
		{
			pkKillerUser	= m_spRoomUserManager->GetUser( kPacket_.m_KillerUserUnitUID );
			pkKillerUserMD	= m_spRoomUserManager->GetUser( kPacket_.m_MaxDamageKillerUserUnitUID );
			pkKilledUser	= m_spRoomUserManager->GetUser( FIRST_SENDER_UID );

			if( pkKillerUser != NULL && pkKillerUser->IsPlaying() == true
				&& pkKilledUser != NULL && pkKilledUser->IsPlaying() == true )
			{
				// �������̿� ���� ���� ����
				//pResultDataKill = SiKPvpMatchResultTable()->GetDeathMatchKill( pkKilledUser->GetPVPEmblem() );

				//kill user & kill Max damage user
				//if( pResultDataKill != NULL )
				//{
				//	//pkKillerUser->AddRewardVP( pResultDataKill->m_VP );
				//	pkKillerUser->AddRewardEXP( pResultDataKill->m_EXP );

				//	if( pkKillerUserMD != NULL && pkKillerUserMD->IsPlaying() == true && kPacket_.m_KillerUserUnitUID != kPacket_.m_MaxDamageKillerUserUnitUID )
				//	{
				//		//pkKillerUserMD->AddRewardVP( pResultDataKill->m_VP );
				//		pkKillerUserMD->AddRewardEXP( pResultDataKill->m_EXP );
				//	}
				//}
				//else
				//{
				//	START_LOG( cerr, L"������� ������ �������̻�.!" )
				//		<< BUILD_LOG( pkKilledUser->GetPVPEmblem() )
				//		<< BUILD_LOG( FIRST_SENDER_UID )
				//		<< END_LOG;
				//}

				////���� ���� ����.
				////pResultDataDeath = SiKPvpMatchResultTable()->GetDeathMatchDeath( pkKilledUser->GetPVPEmblem() );

				//if( pResultDataDeath != NULL )
				//{
				//	//pkKilledUser->AddRewardVP( pResultDataDeath->m_VP );
				//	pkKilledUser->AddRewardEXP( pResultDataDeath->m_EXP );
				//}							
			}
		}
	}

	// ���� ��� �������� ������ �˸���.
	KERM_USER_UNIT_DIE_NOT kPacketNot;
	kPacketNot.m_iGameType			= m_cGameType;
	if( bIncreaseNumKill == true && bIncreaseNumMDKill == true )
	{
		kPacketNot.m_KillerUserUnitUID			= kPacket_.m_KillerUserUnitUID;
		kPacketNot.m_MaxDamageKillerUserUnitUID = kPacket_.m_MaxDamageKillerUserUnitUID;
		kPacketNot.m_KilledUserUnitUID			= FIRST_SENDER_UID;
	}
	else
	{
		kPacketNot.m_KillerUserUnitUID			= -1;
		kPacketNot.m_MaxDamageKillerUserUnitUID = -1;
		kPacketNot.m_KilledUserUnitUID			= FIRST_SENDER_UID;
	}
	kPacketNot.m_KillerNPCUID = kPacket_.m_KillerNPCUID;

	//{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
	kPacketNot.m_bOfficialMatch = IsOfficialMatch();
#endif SERV_NEW_PVP_QUEST

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
	{	
		KRoomUserPtr pkKilledUser	= m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
		if( pkKilledUser != NULL )
		{
			kPacketNot.m_killedUserUnitType			= pkKilledUser->GetUnitType();			
		}
		else
		{
			START_LOG( clog, L"[�׽�Ʈ�α�] ���� ���� ������ ��µ� ����!" )
				<< BUILD_LOG( kPacketNot.m_killedUserUnitType );		
		}
	}
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL
	//}} 
#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
	{	
		KRoomUserPtr pkKilledUser	= m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
		if( pkKilledUser != NULL )
		{
			kPacketNot.m_killedUserUnitClass			= pkKilledUser->GetUnitClass_LUA();		
		}
		else
		{
			START_LOG( clog, L"[�׽�Ʈ�α�] ���� ������ Ŭ������ ��µ� ����" )
				<< BUILD_LOG( kPacketNot.m_killedUserUnitClass );		
		}
	}
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL

	BroadCast( ERM_USER_UNIT_DIE_NOT, kPacketNot );

	if( GetStateID() == KRoomFSM::S_PLAY )
	{
		if( m_cGameType == CXSLRoom::PGT_SURVIVAL )
		{
			KEGS_CURRENT_KILL_SCORE_NOT kPacket2;
			m_spRoomUserManager->GetCurrentKillScore( kPacket2.m_mapKillScore, kPacket2.m_mapMDKillScore );
			BroadCast( ERM_CURRENT_KILL_SCORE_NOT, kPacket2 );
		}

		if( m_cGameType == CXSLRoom::PGT_TEAM_DEATH )
		{
			KEGS_CURRENT_KILL_SCORE_NOT kPacket2;
			m_spRoomUserManager->GetCurrentKillScore( kPacket2.m_mapKillScore, kPacket2.m_mapMDKillScore );
			BroadCast( ERM_CURRENT_KILL_SCORE_NOT, kPacket2 );

			KEGS_CURRENT_TEAM_KILL_SCORE_NOT kPacket3;
			kPacket3.m_RedTeamKill = m_spRoomUserManager->GetTeamScore( CXSLRoom::TN_RED );
			kPacket3.m_BlueTeamKill = m_spRoomUserManager->GetTeamScore( CXSLRoom::TN_BLUE );
			BroadCast( ERM_CURRENT_TEAM_KILL_SCORE_NOT, kPacket3 );
		}
	}

	//������Ŷ�� �ش��������� ������ ��������𿡰� ���� ���� ������
	//������
	kPacket.m_fReBirthTime	= fRebirthTime;
	kPacket.m_VP			= 0;
	kPacket.m_EXP			= 0;
	//if( pResultDataDeath != NULL && bIncreaseNumKill == true && bIncreaseNumMDKill == true )
	//{
	//	//kPacket.m_VP			= pResultDataDeath->m_VP;
	//	kPacket.m_EXP			= pResultDataDeath->m_EXP;
	//}	
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

	// ������Ŷ�� �ش��������� ������ ��������𿡰� ���� ���� ������
	// ��Ÿģ��
	if( pkKillerUser != NULL && pkKillerUser->IsPlaying() == true && bIncreaseNumKill == true && bIncreaseNumMDKill == true )
	{
		kPacket.m_iOK = NetError::NET_OK;
		kPacket.m_fReBirthTime	= 0.0f;
		//if( pResultDataKill != NULL )
		//{
		//	//kPacket.m_VP			= pResultDataKill->m_VP;
		//	kPacket.m_EXP			= pResultDataKill->m_EXP;
		//}	

		//{{ 2011. 09. 19  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
		KERM_USER_UNIT_DIE_ACK kAck;
		kAck.m_iOK = kPacket.m_iOK;
		kAck.m_ucDieReason = kPacket_.m_ucDieReason;
		kAck.m_fReBirthTime	= kPacket.m_fReBirthTime;
		kAck.m_VP			= kPacket.m_VP;
		kAck.m_EXP			= kPacket.m_EXP;
		SendToGSCharacter( pkKillerUser->GetGSUID(), kPacket_.m_KillerUserUnitUID, ERM_USER_UNIT_DIE_ACK, kAck );
#else
		SendToGSCharacter( pkKillerUser->GetGSUID(), kPacket_.m_KillerUserUnitUID , ERM_USER_UNIT_DIE_ACK, kPacket );
#endif SERV_ADD_TITLE_CONDITION
		//}}
		

		//{{ 2010. 8. 4	������	�� �ý��� - �����濡���� �� ������ ���� ��Ű�� �ʴ´�.
		//#ifdef SERV_PET_SYSTEM
		//		// �� ó��!
		//		if( m_cGameType == CXSLRoom::PGT_SURVIVAL )
		//		{
		//			if( pkKillerUser->HavePet() == true )
		//			{
		//				KERM_DECREASE_PET_SATIETY_NOT kPacketNot;
		//				kPacketNot.m_cTeamNumMember = 1;
		//				SendToGSCharacter( pkKillerUser->GetGSUID(), kPacket_.m_KillerUserUnitUID, ERM_DECREASE_PET_SATIETY_NOT, kPacketNot );
		//			}
		//		}
		//		else
		//		{
		//			std::vector< UidType > vecTeamMemberList;
		//			m_spRoomUserManager->GetTeamMemberList( static_cast<CXSLRoom::TEAM_NUM>(pkKillerUser->GetTeam()), true, vecTeamMemberList );
		//
		//			// ��Ŷ�� �̸� �غ�����!
		//			KERM_DECREASE_PET_SATIETY_NOT kPacketNot;
		//			kPacketNot.m_cTeamNumMember = static_cast<char>(vecTeamMemberList.size());			
		//
		//			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecTeamMemberList )
		//			{
		//				KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iUnitUID );
		//				if( spRoomUser == NULL )
		//				{
		//					START_LOG( cerr, L"RoomUser�� �������� ����." )
		//						<< END_LOG;
		//					continue;
		//				}
		//
		//				// �� ������ �н�!
		//				if( spRoomUser->HavePet() == false )
		//					continue;
		//
		//				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_DECREASE_PET_SATIETY_NOT, kPacketNot );
		//			}
		//		}
		//#endif SERV_PET_SYSTEM
		//}}	
	}

	// ������Ŷ�� �ش��������� ������ ��������𿡰� ���� ���� ������
	// �Ƶ�
	if( pkKillerUserMD != NULL && pkKillerUserMD->IsPlaying() == true && bIncreaseNumKill == true && bIncreaseNumMDKill == true )
	{
		if( kPacket_.m_KillerUserUnitUID != kPacket_.m_MaxDamageKillerUserUnitUID )
		{
			kPacket.m_iOK = NetError::NET_OK;
			kPacket.m_fReBirthTime	= 0.0f;
			//if( pResultDataKill != NULL )
			//{
			//	//kPacket.m_VP			= pResultDataKill->m_VP;
			//	kPacket.m_EXP			= pResultDataKill->m_EXP;
			//}	
			//{{ 2011. 09. 19  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
			KERM_USER_UNIT_DIE_ACK kAck;
			kAck.m_iOK = kPacket.m_iOK;
			kAck.m_ucDieReason = kPacket_.m_ucDieReason;
			kAck.m_fReBirthTime	= kPacket.m_fReBirthTime;
			kAck.m_VP			= kPacket.m_VP;
			kAck.m_EXP			= kPacket.m_EXP;
			SendToGSCharacter( pkKillerUserMD->GetGSUID(), kPacket_.m_MaxDamageKillerUserUnitUID, ERM_USER_UNIT_DIE_ACK, kAck );
#else
			SendToGSCharacter( pkKillerUserMD->GetGSUID(), kPacket_.m_MaxDamageKillerUserUnitUID, ERM_USER_UNIT_DIE_ACK, kPacket );
#endif SERV_ADD_TITLE_CONDITION
			//}}
		}
	}



	// ���а� ���� ������ ���带 ������.
	if( CheckIfPlayEnd() )
	{
		ReserveEndPlay();
	}
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

_IMPL_ON_FUNC( ERM_USER_UNIT_DIE_REQ, KEGS_USER_UNIT_DIE_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_USER_UNIT_DIE_ACK, KEGS_USER_UNIT_DIE_ACK );

	float fRebirthTime = 12.f;
	if( m_spRoomUserManager->IsRingofpvprebirth( FIRST_SENDER_UID ) == true )
	{
		fRebirthTime = 6.f; 
	}

	kPacket.m_iOK = NetError::NET_OK;
	// Kill �� ����.
	bool bIncreaseNumKill = m_spRoomUserManager->IncreaseNumKill( kPacket_.m_KillerUserUnitUID );
	if( bIncreaseNumKill == false )
	{
		kPacket.m_iOK = NetError::ERR_SLOT_07;

		START_LOG( cwarn, L"IncreaseNumKill() fail" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( NetError::GetErrStr( NetError::ERR_SLOT_07 ) )
			<< END_LOG;
	}

	// MDKill �� ����.
	bool bIncreaseNumMDKill = m_spRoomUserManager->IncreaseNumMDKill( kPacket_.m_MaxDamageKillerUserUnitUID );
	if( bIncreaseNumMDKill == false )
	{
		kPacket.m_iOK = NetError::ERR_SLOT_07;

		START_LOG( cwarn, L"IncreaseNumMDKill() fail" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( NetError::GetErrStr( NetError::ERR_SLOT_07 ) )
			<< END_LOG;
	}

	// Die �� ����.
	bool bIncreaseNumDie = m_spRoomUserManager->IncreaseNumDie( FIRST_SENDER_UID );
	if( bIncreaseNumDie == false )
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
		

		START_LOG( cwarn, L"IncreaseNumDie() fail" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( NetError::GetErrStr( NetError::ERR_SLOT_07 ) )
			<< END_LOG;

		return;
	}

	// �׾��ٰ� ǥ��.
	m_spRoomUserManager->SetDie( FIRST_SENDER_UID, true );

	// ��Ȱ ����Ʈ�� �ִ´�.
	KRebirthInfo kRebirthInfo;
	kRebirthInfo.nCID = FIRST_SENDER_UID;
	m_spRoomUserManager->GetRebirthPos( FIRST_SENDER_UID, kRebirthInfo.iPosition );
	kRebirthInfo.fDelay = fRebirthTime;
	kRebirthInfo.kTimer.restart();
	//{{ 2012. 11. 27 PVP���� ������ ���� ���� ��Ȱ ���� ���� - ��μ�
#ifdef SERV_PVP_REBIRTH_ADD_CONDITION
	kRebirthInfo.bReady = false;			// USER_UNIT_DIE_COMPLETE �� ������ true �� �Ѵ�.
#endif SERV_PVP_REBIRTH_ADD_CONDITION
	//}
	m_vecRebirthInfo.push_back( kRebirthInfo );

	//���γ��� ���� ������ ��쿡 ������ ���� ��ų���� �ø���. �ȱ׷��� ������ �� �����Ƿ�
	if( bIncreaseNumKill == false && m_cGameType == CXSLRoom::PGT_TEAM )
	{
		KRoomUserPtr pkKilledUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
		if( pkKilledUser != NULL && pkKilledUser->IsPlaying() == true )
		{
			CXSLRoom::TEAM_NUM teamNum = (CXSLRoom::TEAM_NUM)pkKilledUser->GetTeam();
			if( teamNum == CXSLRoom::TN_RED )
			{
				teamNum = CXSLRoom::TN_BLUE;
			}
			else
			{
				teamNum = CXSLRoom::TN_RED;
			}
			m_spRoomUserManager->IncreaseTeamNumKillByTeamNum( teamNum );
		}
	}
	else
	{
		m_spRoomUserManager->IncreaseTeamNumKill( kPacket_.m_KillerUserUnitUID );
	}

	KRoomUserPtr pkKillerUser;
	KRoomUserPtr pkKillerUserMD;
	KRoomUserPtr pkKilledUser;
	const KPVPResultTable::ResultData* pResultDataKill		= NULL;
	const KPVPResultTable::ResultData* pResultDataDeath		= NULL;

	//���̴� ���� ���� ���� ED, VP�� ������
	if( GetStateID() == KRoomFSM::S_PLAY && bIncreaseNumKill == true && bIncreaseNumMDKill == true )
	{
		//������ġ �迭������ �Ѵ�
		if( m_cGameType == CXSLRoom::PGT_SURVIVAL || m_cGameType == CXSLRoom::PGT_TEAM_DEATH )
		{
			pkKillerUser	= m_spRoomUserManager->GetUser( kPacket_.m_KillerUserUnitUID );
			pkKillerUserMD	= m_spRoomUserManager->GetUser( kPacket_.m_MaxDamageKillerUserUnitUID );
			pkKilledUser	= m_spRoomUserManager->GetUser( FIRST_SENDER_UID );

			if( pkKillerUser != NULL && pkKillerUser->IsPlaying() == true
				&& pkKilledUser != NULL && pkKilledUser->IsPlaying() == true )
			{
				// �������̿� ���� ���� ����
				pResultDataKill = SiKPVPResultTable()->GetDeathMatchKill( pkKilledUser->GetPVPEmblem() );

				//kill user & kill Max damage user
				if( pResultDataKill != NULL )
				{
					pkKillerUser->AddRewardVP( pResultDataKill->m_VP );
					pkKillerUser->AddRewardEXP( pResultDataKill->m_EXP );

					if( pkKillerUserMD != NULL && pkKillerUserMD->IsPlaying() == true && kPacket_.m_KillerUserUnitUID != kPacket_.m_MaxDamageKillerUserUnitUID )
					{
						pkKillerUserMD->AddRewardVP( pResultDataKill->m_VP );
						pkKillerUserMD->AddRewardEXP( pResultDataKill->m_EXP );
					}
				}
				else
				{
					START_LOG( cerr, L"������� ������ �������̻�.!" )
						<< BUILD_LOG( pkKilledUser->GetPVPEmblem() )
						<< BUILD_LOG( FIRST_SENDER_UID )
						<< END_LOG;
				}

				//���� ���� ����.
				pResultDataDeath = SiKPVPResultTable()->GetDeathMatchDeath( pkKilledUser->GetPVPEmblem() );

				if( pResultDataDeath != NULL )
				{
					pkKilledUser->AddRewardVP( pResultDataDeath->m_VP );
					pkKilledUser->AddRewardEXP( pResultDataDeath->m_EXP );
				}							
			}
		}
	}

	// ���� ��� �������� ������ �˸���.
	KERM_USER_UNIT_DIE_NOT kPacketNot;
	kPacketNot.m_iGameType			= m_cGameType;
	if( bIncreaseNumKill == true && bIncreaseNumMDKill == true )
	{
		kPacketNot.m_KillerUserUnitUID			= kPacket_.m_KillerUserUnitUID;
		kPacketNot.m_MaxDamageKillerUserUnitUID = kPacket_.m_MaxDamageKillerUserUnitUID;
		kPacketNot.m_KilledUserUnitUID			= FIRST_SENDER_UID;
	}
	else
	{
		kPacketNot.m_KillerUserUnitUID			= -1;
		kPacketNot.m_MaxDamageKillerUserUnitUID = -1;
		kPacketNot.m_KilledUserUnitUID			= FIRST_SENDER_UID;
	}	

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
	if( pkKilledUser != NULL )
		kPacketNot.m_killedUserUnitType			= pkKilledUser->GetUnitType();
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
	if( pkKilledUser != NULL )
		kPacketNot.m_killedUserUnitClass			= pkKilledUser->GetUnitClass_LUA();
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL

	BroadCast( ERM_USER_UNIT_DIE_NOT, kPacketNot );

	if( GetStateID() == KRoomFSM::S_PLAY )
	{
		if( m_cGameType == CXSLRoom::PGT_SURVIVAL )
		{
			KEGS_CURRENT_KILL_SCORE_NOT kPacket2;
			m_spRoomUserManager->GetCurrentKillScore( kPacket2.m_mapKillScore, kPacket2.m_mapMDKillScore );
			BroadCast( ERM_CURRENT_KILL_SCORE_NOT, kPacket2 );
		}

		if( m_cGameType == CXSLRoom::PGT_TEAM_DEATH )
		{
			KEGS_CURRENT_KILL_SCORE_NOT kPacket2;
			m_spRoomUserManager->GetCurrentKillScore( kPacket2.m_mapKillScore, kPacket2.m_mapMDKillScore );
			BroadCast( ERM_CURRENT_KILL_SCORE_NOT, kPacket2 );

			KEGS_CURRENT_TEAM_KILL_SCORE_NOT kPacket3;
			kPacket3.m_RedTeamKill = m_spRoomUserManager->GetTeamScore( CXSLRoom::TN_RED );
			kPacket3.m_BlueTeamKill = m_spRoomUserManager->GetTeamScore( CXSLRoom::TN_BLUE );
			BroadCast( ERM_CURRENT_TEAM_KILL_SCORE_NOT, kPacket3 );
		}
	}

	//������Ŷ�� �ش��������� ������ ��������𿡰� ���� ���� ������
	//������
	kPacket.m_fReBirthTime	= fRebirthTime;
	kPacket.m_VP			= 0;
	kPacket.m_EXP			= 0;

	if( pResultDataDeath != NULL && bIncreaseNumKill == true && bIncreaseNumMDKill == true )
	{
		kPacket.m_VP			= pResultDataDeath->m_VP;
		kPacket.m_EXP			= pResultDataDeath->m_EXP;
	}	
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

	// ������Ŷ�� �ش��������� ������ ��������𿡰� ���� ���� ������
	// ��Ÿģ��
	if( pkKillerUser != NULL && pkKillerUser->IsPlaying() == true && bIncreaseNumKill == true && bIncreaseNumMDKill == true )
	{
		kPacket.m_iOK = NetError::NET_OK;
		kPacket.m_fReBirthTime	= 0.0f;
		if( pResultDataKill != NULL )
		{
			kPacket.m_VP			= pResultDataKill->m_VP;
			kPacket.m_EXP			= pResultDataKill->m_EXP;
		}	

		//{{ 2011. 09. 19  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
		KERM_USER_UNIT_DIE_ACK kAck;
		kAck.m_iOK = kPacket.m_iOK;
		kAck.m_ucDieReason = kPacket_.m_ucDieReason;
		kAck.m_fReBirthTime	= kPacket.m_fReBirthTime;
		kAck.m_VP			= kPacket.m_VP;
		kAck.m_EXP			= kPacket.m_EXP;
		SendToGSCharacter( pkKillerUser->GetGSUID(), kPacket_.m_KillerUserUnitUID, ERM_USER_UNIT_DIE_ACK, kAck );
#else
		SendToGSCharacter( pkKillerUser->GetGSUID(), kPacket_.m_KillerUserUnitUID , ERM_USER_UNIT_DIE_ACK, kPacket );
#endif SERV_ADD_TITLE_CONDITION
		//}}
		

		//{{ 2010. 8. 4	������	�� �ý��� - �����濡���� �� ������ ���� ��Ű�� �ʴ´�.
		//#ifdef SERV_PET_SYSTEM
		//		// �� ó��!
		//		if( m_cGameType == CXSLRoom::PGT_SURVIVAL )
		//		{
		//			if( pkKillerUser->HavePet() == true )
		//			{
		//				KERM_DECREASE_PET_SATIETY_NOT kPacketNot;
		//				kPacketNot.m_cTeamNumMember = 1;
		//				SendToGSCharacter( pkKillerUser->GetGSUID(), kPacket_.m_KillerUserUnitUID, ERM_DECREASE_PET_SATIETY_NOT, kPacketNot );
		//			}
		//		}
		//		else
		//		{
		//			std::vector< UidType > vecTeamMemberList;
		//			m_spRoomUserManager->GetTeamMemberList( static_cast<CXSLRoom::TEAM_NUM>(pkKillerUser->GetTeam()), true, vecTeamMemberList );
		//
		//			// ��Ŷ�� �̸� �غ�����!
		//			KERM_DECREASE_PET_SATIETY_NOT kPacketNot;
		//			kPacketNot.m_cTeamNumMember = static_cast<char>(vecTeamMemberList.size());			
		//
		//			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecTeamMemberList )
		//			{
		//				KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iUnitUID );
		//				if( spRoomUser == NULL )
		//				{
		//					START_LOG( cerr, L"RoomUser�� �������� ����." )
		//						<< END_LOG;
		//					continue;
		//				}
		//
		//				// �� ������ �н�!
		//				if( spRoomUser->HavePet() == false )
		//					continue;
		//
		//				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_DECREASE_PET_SATIETY_NOT, kPacketNot );
		//			}
		//		}
		//#endif SERV_PET_SYSTEM
		//}}	
	}

	// ������Ŷ�� �ش��������� ������ ��������𿡰� ���� ���� ������
	// �Ƶ�
	if( pkKillerUserMD != NULL && pkKillerUserMD->IsPlaying() == true && bIncreaseNumKill == true && bIncreaseNumMDKill == true )
	{
		if( kPacket_.m_KillerUserUnitUID != kPacket_.m_MaxDamageKillerUserUnitUID )
		{
			kPacket.m_iOK = NetError::NET_OK;
			kPacket.m_fReBirthTime	= 0.0f;
			if( pResultDataKill != NULL )
			{
				kPacket.m_VP			= pResultDataKill->m_VP;
				kPacket.m_EXP			= pResultDataKill->m_EXP;
			}
			//{{ 2011. 09. 19  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
			KERM_USER_UNIT_DIE_ACK kAck;
			kAck.m_iOK = kPacket.m_iOK;
			kAck.m_ucDieReason = kPacket_.m_ucDieReason;
			kAck.m_fReBirthTime	= kPacket.m_fReBirthTime;
			kAck.m_VP			= kPacket.m_VP;
			kAck.m_EXP			= kPacket.m_EXP;
			SendToGSCharacter( pkKillerUserMD->GetGSUID(), kPacket_.m_MaxDamageKillerUserUnitUID, ERM_USER_UNIT_DIE_ACK, kAck );
#else
			SendToGSCharacter( pkKillerUserMD->GetGSUID(), kPacket_.m_MaxDamageKillerUserUnitUID, ERM_USER_UNIT_DIE_ACK, kPacket );
#endif SERV_ADD_TITLE_CONDITION
			//}}
		}
	}



	// ���а� ���� ������ ���带 ������.
	if( CheckIfPlayEnd() )
	{
		ReserveEndPlay();
	}
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}


IMPL_ON_FUNC_NOPARAM( ERM_USER_UNIT_DIE_COMPLETE_REQ )
{
	VERIFY_ROOM_STATE_AND_WARN( ( 1, KRoomFSM::S_PLAY ), (1, KRoomFSM::S_RESULT), ERM_USER_UNIT_DIE_COMPLETE_ACK, KEGS_USER_UNIT_DIE_COMPLETE_ACK );	

	//// ��Ȱ ����Ʈ�� �ִ´�.
	//KRebirthInfo kRebirthInfo;
	//kRebirthInfo.nCID = FIRST_SENDER_UID;
	//m_spRoomUserManager->GetRebirthPos( FIRST_SENDER_UID, kRebirthInfo.iPosition );
	//kRebirthInfo.fDelay = 10.f;
	//kRebirthInfo.kTimer.restart();
	//m_vecRebirthInfo.push_back( kRebirthInfo );

	//KEGS_USER_UNIT_DIE_COMPLETE_ACK kAck;


	//{{ 2012. 11. 27 PVP���� ������ ���� ���� ��Ȱ ���� ���� - ��μ�
#ifdef SERV_PVP_REBIRTH_ADD_CONDITION
	BOOST_TEST_FOREACH( KRebirthInfo&,  kInfo, m_vecRebirthInfo )
	{
		if( kInfo.nCID == FIRST_SENDER_UID )
		{
			kInfo.bReady = true;
			break;
		}
	}
#endif SERV_PVP_REBIRTH_ADD_CONDITION
	//}

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_USER_UNIT_DIE_COMPLETE_ACK, kPacket );

	KEGS_USER_UNIT_DIE_COMPLETE_NOT kNot;
	kNot.m_UnitUID = FIRST_SENDER_UID;

	BroadCast( ERM_USER_UNIT_DIE_COMPLETE_NOT, kNot );
}

_IMPL_ON_FUNC( ERM_USER_UNIT_RE_BIRTH_POS_REQ, KEGS_USER_UNIT_RE_BIRTH_POS_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_USER_UNIT_RE_BIRTH_POS_ACK, KPacketOK );	

    if( !m_spRoomUserManager->SetRebirthPos( FIRST_SENDER_UID, kPacket_.m_StartPosIndex ) )
    {
        kPacket.m_iOK = NetError::ERR_ROOM_17;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_USER_UNIT_RE_BIRTH_POS_ACK, kPacket );
        return;
    }

    kPacket.m_iOK = NetError::NET_OK;
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_USER_UNIT_RE_BIRTH_POS_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_MY_USER_UNIT_INFO_TO_SERVER_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_MY_USER_UNIT_INFO_TO_SERVER_ACK, KPacketOK );
	
	//{{ 2010. 05. 12  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_.m_kRoomUserInfo ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_17;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_MY_USER_UNIT_INFO_TO_SERVER_ACK, kPacket );
		return;
	}

	// RoomUserInfo�� ���������� ������
	BroadCast( ERM_CHANGE_ROOM_USER_INFO_NOT, kPacket_.m_kRoomUserInfo );
#endif SERV_INTEGRATION
	//}}

    if( m_spRoomUserManager->SetHP( FIRST_SENDER_UID, kPacket_.m_kDungeonUnitInfo.m_fHP ) == false )
    {
        kPacket.m_iOK = NetError::ERR_ROOM_17;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_MY_USER_UNIT_INFO_TO_SERVER_ACK, kPacket );
        return;
    }	

    kPacket.m_iOK = NetError::NET_OK;
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_MY_USER_UNIT_INFO_TO_SERVER_ACK, kPacket );

    if( m_spRoomUserManager->IsAllPlayerHPReported() )
    {
        START_LOG( clog2, L"��� ���� HP ����Ʈ." )
            << BUILD_LOG( GetStateID() )
            << BUILD_LOG( GetStateIDString() );

        EndPlay();
    }
}

IMPL_ON_FUNC_NOPARAM( ERM_LEAVE_GAME_REQ )
{
	//{{ 2012. 11. 26	�ڼ���	������ ��� ����
#ifdef SERV_FIX_OBSERVE_MODE
	KEGS_LEAVE_GAME_ACK kPacket;

	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID, KRoomUserManager::UT_OBSERVER );
	if( ( spRoomUser == NULL ) || ( spRoomUser->IsObserver() == false ) )
	{
		VERIFY_ROOM_STATE( ( 3, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ), ERM_LEAVE_GAME_ACK, KEGS_LEAVE_GAME_ACK );
	}
#else
	VERIFY_ROOM_STATE( ( 3, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ), ERM_LEAVE_GAME_ACK, KEGS_LEAVE_GAME_ACK );
#endif SERV_FIX_OBSERVE_MODE
	//}}

	// �̸� PlayTime�� ���Ѵ�. 
	// [����1] LeaveGame()�� ȣ���� �� EndPlay()�� ȣ��Ǿ�� �ð��� �������� ����.
	// [����2] ���� KRoomUser::EndPlay()�Լ����� PlayTime�� ���ϰ��ִ�.
	float fPlayTime = 0.0f;
	m_spRoomUserManager->GetPlayTime( FIRST_SENDER_UID, fPlayTime );

	START_LOG( clog2, L"��Ż �÷��� Ÿ��[���ͼ���]" )
		<< BUILD_LOG( static_cast<int>(fPlayTime) );

    if( !m_spRoomUserManager->LeaveGame( FIRST_SENDER_UID ) )
    {
        kPacket.m_iOK = NetError::ERR_ROOM_19;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_GAME_ACK, kPacket );
        return;
    }

    // ��Ż�� ���.
    if( GetStateID() == KRoomFSM::S_PLAY )
    {
		//������ ��Ż ���ó��
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );

		if( !spRoomUser )
		{
			spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID, KRoomUserManager::UT_OBSERVER );
		}

		if( !spRoomUser )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
		}
		else
		{
			if( spRoomUser->IsObserver() == false )
			{
				KERM_UPDATE_PVP_UNIT_INFO_NOT kPacket;
				//{{ 2009. 11. 16  ������	�����������Ʈ
				kPacket.m_iMemberCount = m_spRoomUserManager->GetNumMember();
				//}}
				kPacket.m_bOutRoom = false;
				//{{ 2011. 07. 25	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
                kPacket.m_iCurrentRating = spRoomUser->GetRating();
				kPacket.m_iCurrentMaxRating = spRoomUser->GetMaxRating();
				kPacket.m_iCurrentRPoint = spRoomUser->GetRPoint();
				kPacket.m_iCurrentAPoint = spRoomUser->GetAPoint();
#endif SERV_PVP_NEW_SYSTEM
				//}}

				//{{ 2012. 09. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
				kPacket.m_cRank				= spRoomUser->GetCurrentRank();
				kPacket.m_cRankForServer	= spRoomUser->GetCurrentRankForServer();
#endif SERV_2012_PVP_SEASON2
				//}}

				switch( m_cGameType )
				{
				case CXSLRoom::PGT_TEAM_DEATH:
					//{{ 2011. 07. 25	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
					SiKResultProcess()->Result_TEAM_DEATH_BREAK( m_spRoomUserManager, spRoomUser, NetError::NOT_LEAVE_ROOM_REASON_00, GetMatchType(), GetPvpNpcType(), kPacket );
#else
					SiKResultProcess()->Result_TEAM_DEATH_BREAK( m_spRoomUserManager, spRoomUser, kPacket );
#endif SERV_PVP_NEW_SYSTEM
					//}}
					break;

				case CXSLRoom::PGT_SURVIVAL:
					//{{ 2011. 07. 25	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
					SiKResultProcess()->Result_SURVIVAL_BREAK( m_spRoomUserManager, spRoomUser, NetError::NOT_LEAVE_ROOM_REASON_00, GetMatchType(), GetPvpNpcType(), kPacket );
#else
					SiKResultProcess()->Result_SURVIVAL_BREAK( m_spRoomUserManager, spRoomUser, kPacket );
#endif SERV_PVP_NEW_SYSTEM
					//}}
					break;
				}

				//������ ��Ż�� �� ��ϵ�����
				kPacket.m_iLose		= 1;

				//������ ��Ż�� �� ��ϵ�����
				if( GetPVPChannelClass() == KPVPChannelInfo::PCC_FREE  || 
					//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
					GetPVPChannelClass() == KPVPChannelInfo::PCC_TOURNAMENT  || 
#endif SERV_PVP_NEW_SYSTEM
					//}}
					GetPVPChannelClass() == KPVPChannelInfo::PCC_PLAY )
				{
					kPacket.m_iEXP = 0;
					//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
					kPacket.m_iRating = 0;
					kPacket.m_iRPoint = 0;
					kPacket.m_iAPoint = 0;
#else
					kPacket.m_iVSPoint = 0;
#endif SERV_PVP_NEW_SYSTEM
					//}}
					kPacket.m_iLose	= 0;
				}

				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_PVP_UNIT_INFO_NOT, kPacket );

				//////////////////////////////////////////////////////////////////// ���			
				KStatisticsKey kKey;
				kKey.m_vecIntKey.push_back( static_cast<int>(m_uiRoomListID) );
				KSIManager.IncreaseCount( KStatistics::SI_LOC_PVP, kKey, KStatistics::SLP_GIVE_UP, 1 );

				if( GetPVPChannelClass() != KPVPChannelInfo::PCC_FREE  &&  
					//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
					GetPVPChannelClass() != KPVPChannelInfo::PCC_TOURNAMENT  &&  
#endif SERV_PVP_NEW_SYSTEM
					//}}
					GetPVPChannelClass() != KPVPChannelInfo::PCC_PLAY )
				{
					// ���� DB���
					KERM_PVP_USER_STATISTICS_NOT kPacketStats;
					kPacketStats.m_cGameType		 = m_cGameType;
					kPacketStats.m_cGameResult		 = KERM_PVP_USER_STATISTICS_NOT::UGR_DROP;
					kPacketStats.m_iGamePlayTime	 = static_cast<int>(fPlayTime);
					kPacketStats.m_bIntrude			 = m_spRoomUserManager->GetIsIntrude(	FIRST_SENDER_UID );
					kPacketStats.m_sWorldID			 = m_sWorldID;
					kPacketStats.m_iKillNum			 = m_spRoomUserManager->GetKillNumber( FIRST_SENDER_UID );
					SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PVP_USER_STATISTICS_NOT, kPacketStats );
				}

				// PVP ���� �·� �α� [����� üũ]
				PvPLogToFile( spRoomUser, 0, 0, 2 );
			}
		}
	}

    kPacket.m_iOK = NetError::NET_OK;	
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_GAME_ACK, kPacket );

	//{{ 2009. 4. 23  ������	������������Ʈ
	SendPVPUserInfo( FIRST_SENDER_UID, KCommunityUserInfo::US_PVP_WAIT );
	//}}

    if( GetStateID() == KRoomFSM::S_LOAD )
    {
		//{{ 2011. 08. 05	������	�����濡 ���� ȥ�� ���ԵǴ� ���� ����
#ifdef SERV_CODE_REFACTORING_20110804
		CheckAllPlayerFinishLoading();
#else
		// ��� ������ �ε� �������� not�� ������.
		if( m_spRoomUserManager->IsAllPlayerFinishLoading() )
		{
			BroadCastID( ERM_GAME_LOADING_ALL_UNIT_OK_NOT );

			// ���������� �ʱ�ȭ.
			StartPlay();

			KEGS_PLAY_START_NOT kNot;
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

	if( GetStateID() == KRoomFSM::S_PLAY )
	{
		if( CheckIfPlayEnd() )
		{
			if( m_spRoomUserManager->GetNumPlaying() >= 1 )
			{
				ReserveEndPlay();
				//EndPlay();
			}
			else
			{
				//�߸��� ��Ȳ���� �������� ���� ������ ������ �����ΰ��
				//��������ó���� ����.
				EndPlay();
				EndGame();
			}
		}
	}

	//���â�� ���������� ���� �������ó��
	if( GetStateID() == KRoomFSM::S_RESULT )
	{
		CheckResultSuccess();
	}

	// ���� ��� �����鿡�� ���� ������ ������.
	KEGS_LEAVE_GAME_NOT kPacketNot;
	kPacketNot.m_cRoomState = GetStateID();
	kPacketNot.m_iUnitUID = FIRST_SENDER_UID;
	GetRoomSlotInfo( kPacketNot.m_vecSlot );
	GetRoomSlotInfo( kPacketNot.m_vecObserverSlot, KRoomUserManager::UT_OBSERVER );
	BroadCast( ERM_LEAVE_GAME_NOT, kPacketNot );
}

//{{ 2012. 12. 5	������		������ ���� ���� ���
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
//////////////////////////////////////////////////////////////////////////
IMPL_ON_FUNC_NOPARAM( ERM_STATE_CHANGE_GAME_INTRUDE_REQ )
{
	VERIFY_ROOM_STATE_WARN( ( 2, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY ), ERM_STATE_CHANGE_GAME_INTRUDE_ACK, KERM_STATE_CHANGE_GAME_INTRUDE_ACK );

	// ������ üũ
	const bool bObserver = m_spRoomUserManager->IsObserver( FIRST_SENDER_UID );

	//�����÷��� �ð��� 10�� �̸��̸� �����Ҽ� ����.
	if( bObserver == false )
	{
		if( IsAbleToIntrude() == false )
		{
			kPacket.m_iOK = NetError::GetLastError();//NetError::ERR_ROOM_18;

			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STATE_CHANGE_GAME_INTRUDE_ACK, kPacket );
			return;
		}

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// �۾���¥: 2013-10-08	// �ڼ���
		switch( GetPVPChannelClass() )
		{
		case KPVPChannelInfo::PCC_TOURNAMENT:
			{
				kPacket.m_iOK = NetError::ERR_ROOM_37;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STATE_CHANGE_GAME_INTRUDE_ACK, kPacket );
				return;
			}

		case KPVPChannelInfo::PCC_OFFICIAL:
			{
				kPacket.m_iOK = NetError::ERR_ROOM_53;	// ���� �������� ������ �� �����ϴ�.
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STATE_CHANGE_GAME_INTRUDE_ACK, kPacket );
				return;
			}
		}
#else // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM
		//{{ 2010. 02. 10  ������	��ȸ ä�ο����� ���� �Ұ�
		if( GetPVPChannelClass() == KPVPChannelInfo::PCC_TOURNAMENT )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_37;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STATE_CHANGE_GAME_INTRUDE_ACK, kPacket );
			return;
		}
		//}}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM
	}

	if( !m_spRoomUserManager->SetReady( FIRST_SENDER_UID, true, bObserver ) )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_18;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STATE_CHANGE_GAME_INTRUDE_ACK, kPacket );
		return;
	}

	if( !m_spRoomUserManager->StartGame( FIRST_SENDER_UID, bObserver ) )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_18;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STATE_CHANGE_GAME_INTRUDE_ACK, kPacket );
		return;
	}

#ifdef SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS// �۾���¥: 2013-06-10	// �ڼ���
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_BUFF_INFO_NOT );
#endif // SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS

	if( bObserver == false )
	{
		IntrudeTeamBalance( FIRST_SENDER_UID );
	}

	if( GetStateID() == KRoomFSM::S_LOAD )
	{
		m_spRoomUserManager->SetLoadingProgress( FIRST_SENDER_UID, 100 );
	}

	kPacket.m_iOK		= NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STATE_CHANGE_GAME_INTRUDE_ACK, kPacket );

	KEGS_STATE_CHANGE_GAME_INTRUDE_NOT kPacketNot;
	kPacketNot.m_RoomState = GetStateID();
	if( bObserver == false )
	{
		m_spRoomUserManager->GetRoomSlotInfo( FIRST_SENDER_UID, kPacketNot.m_IntruderSlot );
	}
	else
	{
		m_spRoomUserManager->GetRoomSlotInfo( FIRST_SENDER_UID, kPacketNot.m_IntruderSlot, KRoomUserManager::UT_OBSERVER );
	}
	BroadCast( ERM_STATE_CHANGE_GAME_INTRUDE_NOT, kPacketNot );

	//{{ 2007. 12. 18  ������  	���� ���·� ���� [for statistics]
	m_spRoomUserManager->SetIsIntrude( FIRST_SENDER_UID, true, bObserver );
	//}}
}

_IMPL_ON_FUNC( ERM_INTRUDE_START_REQ, KEGS_INTRUDE_START_REQ )
{
	VERIFY_ROOM_STATE( ( 2, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY ), ERM_INTRUDE_START_ACK, KEGS_INTRUDE_START_ACK );

	// ������ üũ
	const bool bObserver = m_spRoomUserManager->IsObserver( FIRST_SENDER_UID );

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// �۾���¥: 2013-10-08	// �ڼ���
	if( ( bObserver == false ) && ( IsOfficialMatch() == true ) )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_53;	// ���� �������� ������ �� �����ϴ�.
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_INTRUDE_START_ACK, kPacket );
		return;
	}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM

	if( !m_spRoomUserManager->StartPlay( FIRST_SENDER_UID, bObserver ) )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_18;

		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_INTRUDE_START_ACK, kPacket );
		return;
	}

	// ������ ī��Ʈ�� ����Ѵ�.
	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( static_cast<int>(m_uiRoomListID) );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_PVP, kKey, KStatistics::SLP_INTRUDER, 1 );

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_INTRUDE_START_ACK, kPacket );	

	KEGS_INTRUDE_START_NOT kPacketNot;
	kPacketNot.m_RoomState = GetStateID();
	kPacketNot.m_StartPosIndex = kPacket_.m_StartPosIndex;
	kPacketNot.m_bObserver = bObserver;
	if( bObserver == false )
	{
		m_spRoomUserManager->GetRoomSlotInfo( FIRST_SENDER_UID, kPacketNot.m_IntruderSlot );
	}
	else
	{
		m_spRoomUserManager->GetRoomSlotInfo( FIRST_SENDER_UID, kPacketNot.m_IntruderSlot, KRoomUserManager::UT_OBSERVER );
	}
	BroadCast( ERM_INTRUDE_START_NOT, kPacketNot );

	KEGS_CURRENT_KILL_SCORE_NOT kPacket2;
	m_spRoomUserManager->GetCurrentKillScore( kPacket2.m_mapKillScore, kPacket2.m_mapMDKillScore );
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CURRENT_KILL_SCORE_NOT, kPacket2 );

	if( GetStateID() == KRoomFSM::S_PLAY && m_cGameType == CXSLRoom::PGT_TEAM_DEATH )
	{
		KEGS_CURRENT_TEAM_KILL_SCORE_NOT kPacket3;
		kPacket3.m_RedTeamKill = m_spRoomUserManager->GetTeamScore( CXSLRoom::TN_RED );
		kPacket3.m_BlueTeamKill = m_spRoomUserManager->GetTeamScore( CXSLRoom::TN_BLUE );
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CURRENT_TEAM_KILL_SCORE_NOT, kPacket3 );
	}

#ifdef SERV_FIX_REVENGE_BUFF// �۾���¥: 2013-08-09	// �ڼ���
#ifdef SERV_2012_PVP_SEASON2_2
	if( GetStateID() == KRoomFSM::S_PLAY )
	{
		const int iRevengeBuffID = GetTeamRevengeBuffID( FIRST_SENDER_UID );
		if( iRevengeBuffID != CXSLBuffManager::BTI_NONE )
		{			
			CTime tCurrentTime = CTime::GetCurrentTime();
			KBuffInfo kBuffInfo;
			kBuffInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID	= iRevengeBuffID;
			kBuffInfo.m_iBuffStartTime = tCurrentTime.GetTime();

			KECN_PARTY_BUFF_UPDATE_NOT kPacket;
			kPacket.m_mapActivateBuffList.insert( std::map<int, KBuffInfo>::value_type( kBuffInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID, kBuffInfo ) );
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ECN_PARTY_BUFF_UPDATE_NOT, kPacket );
		}
	}
#endif // SERV_2012_PVP_SEASON2_2
#endif // SERV_FIX_REVENGE_BUFF
}
//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////
IMPL_ON_FUNC_NOPARAM( ERM_STATE_CHANGE_GAME_INTRUDE_REQ )
{
	//{{ 2010. 02. 10  ������	�α� ���� ����	
	VERIFY_ROOM_STATE_WARN( ( 2, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY ), ERM_STATE_CHANGE_GAME_INTRUDE_ACK, KERM_STATE_CHANGE_GAME_INTRUDE_ACK );
	//}}

	//�����÷��� �ð��� 10�� �̸��̸� �����Ҽ� ����.
	if( IsAbleToIntrude() == false )
	{
		kPacket.m_iOK = NetError::GetLastError();//NetError::ERR_ROOM_18;

		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STATE_CHANGE_GAME_INTRUDE_ACK, kPacket );
		return;
	}

	//{{ 2010. 02. 10  ������	��ȸ ä�ο����� ���� �Ұ�
	if( GetPVPChannelClass() == KPVPChannelInfo::PCC_TOURNAMENT )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_37;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STATE_CHANGE_GAME_INTRUDE_ACK, kPacket );
		return;
	}
	//}}

	if( !m_spRoomUserManager->SetReady( FIRST_SENDER_UID, true ) )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_18;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STATE_CHANGE_GAME_INTRUDE_ACK, kPacket );
		return;
	}

	if( !m_spRoomUserManager->StartGame( FIRST_SENDER_UID ) )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_18;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STATE_CHANGE_GAME_INTRUDE_ACK, kPacket );
		return;
	}

#ifdef SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS// �۾���¥: 2013-06-10	// �ڼ���
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_BUFF_INFO_NOT );
#endif // SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS

	IntrudeTeamBalance( FIRST_SENDER_UID );

	if( GetStateID() == KRoomFSM::S_LOAD )
	{
		m_spRoomUserManager->SetLoadingProgress( FIRST_SENDER_UID, 100 );
	}

	kPacket.m_iOK		= NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STATE_CHANGE_GAME_INTRUDE_ACK, kPacket );

	KEGS_STATE_CHANGE_GAME_INTRUDE_NOT kPacketNot;
	kPacketNot.m_RoomState = GetStateID();
	m_spRoomUserManager->GetRoomSlotInfo( FIRST_SENDER_UID, kPacketNot.m_IntruderSlot );
	BroadCast( ERM_STATE_CHANGE_GAME_INTRUDE_NOT, kPacketNot );

	//{{ 2007. 12. 18  ������  	���� ���·� ���� [for statistics]
	m_spRoomUserManager->SetIsIntrude( FIRST_SENDER_UID, true );
	//}}
}

_IMPL_ON_FUNC( ERM_INTRUDE_START_REQ, KEGS_INTRUDE_START_REQ )
{
	VERIFY_ROOM_STATE( ( 2, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY ), ERM_INTRUDE_START_ACK, KEGS_INTRUDE_START_ACK );

	if( !m_spRoomUserManager->StartPlay( FIRST_SENDER_UID ) )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_18;

		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_INTRUDE_START_ACK, kPacket );
		return;
	}

	// ������ ī��Ʈ�� ����Ѵ�.
	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( static_cast<int>(m_uiRoomListID) );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_PVP, kKey, KStatistics::SLP_INTRUDER, 1 );

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_INTRUDE_START_ACK, kPacket );	

	KEGS_INTRUDE_START_NOT kPacketNot;
	kPacketNot.m_RoomState = GetStateID();
	kPacketNot.m_StartPosIndex = kPacket_.m_StartPosIndex;
	m_spRoomUserManager->GetRoomSlotInfo( FIRST_SENDER_UID, kPacketNot.m_IntruderSlot );
	BroadCast( ERM_INTRUDE_START_NOT, kPacketNot );

	KEGS_CURRENT_KILL_SCORE_NOT kPacket2;
	m_spRoomUserManager->GetCurrentKillScore( kPacket2.m_mapKillScore, kPacket2.m_mapMDKillScore );
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CURRENT_KILL_SCORE_NOT, kPacket2 );

	if( GetStateID() == KRoomFSM::S_PLAY && m_cGameType == CXSLRoom::PGT_TEAM_DEATH )
	{
		KEGS_CURRENT_TEAM_KILL_SCORE_NOT kPacket3;
		kPacket3.m_RedTeamKill = m_spRoomUserManager->GetTeamScore( CXSLRoom::TN_RED );
		kPacket3.m_BlueTeamKill = m_spRoomUserManager->GetTeamScore( CXSLRoom::TN_BLUE );
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CURRENT_TEAM_KILL_SCORE_NOT, kPacket3 );
	}
}
//////////////////////////////////////////////////////////////////////////
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
//////////////////////////////////////////////////////////////////////////

_IMPL_ON_FUNC( ERM_CHANGE_TEAM_REQ, KEGS_CHANGE_TEAM_REQ )
{
	//{{ 2010. 02. 10  ������	�α� ���� ����
    VERIFY_ROOM_STATE_WARN( ( 3, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY ), ERM_CHANGE_TEAM_ACK, KEGS_CHANGE_TEAM_ACK );
	//}}

    if( !IsAbleToIntrude() )
    {
        _VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_CHANGE_TEAM_ACK, KEGS_CHANGE_TEAM_ACK, NetError::ERR_ROOM_09 );
    }

	if( m_spRoomUserManager->ChangeTeam( FIRST_SENDER_UID, kPacket_.m_TeamNum ) == false )
	{
		kPacket.m_iOK = NetError::ERR_SLOT_01;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_TEAM_ACK, kPacket );
        return;
	}

    KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� �˻� ����. �Ͼ�� �ʴ� ��Ȳ." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;
        kPacket.m_iOK = NetError::ERR_ROOM_17;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_TEAM_ACK, kPacket );
        return;
    }

	// ��û�� �������� ack�� ������.
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_TEAM_ACK, kPacket );

	// ���� ��� �������� ���� ������ ������.
	KEGS_CHANGE_TEAM_NOT kNot;
	kNot.m_UnitUID			= FIRST_SENDER_UID;
	kNot.m_TeamNum			= ( char )spRoomUser->GetTeam();
	kNot.m_SwapSlotIndex	= ( char )spRoomUser->GetSlotID();

	BroadCast( ERM_CHANGE_TEAM_NOT, kNot );
}

_IMPL_ON_FUNC( ERM_CHANGE_MAP_REQ, KEGS_CHANGE_MAP_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_CHANGE_MAP_ACK, KEGS_CHANGE_MAP_ACK );

	if( m_spRoomUserManager->IsObserver( FIRST_SENDER_UID ) == false )
	{
		// �������� üũ�Ѵ�.
		VERIFY_HOST( ERM_CHANGE_MAP_ACK );
	}

	//note : ���� ���氡���� map id���� üũ�ϴ� ��ƾ�� �־�� �Ѵ�.???
	//{{ 2008. 5. 27  ������  ������
	m_sShowPvpMapWorldID = kPacket_.m_WorldID; // UI�� �����ֱ� ���� �� ������.
	//}}	

	// ��û�� �������� ack�� ������.
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_MAP_ACK, kPacket );

	// ���� ��� �������� ���� ������ ������.
	KEGS_CHANGE_MAP_NOT kNot;
	kNot.m_WorldID = m_sShowPvpMapWorldID;

	BroadCast( ERM_CHANGE_MAP_NOT, kNot );

	SendRoomListInfo( NetError::ERR_ROOM_15 );
}

_IMPL_ON_FUNC( ERM_CHANGE_WINNING_NUM_KILL_REQ, KEGS_CHANGE_WINNING_NUM_KILL_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_CHANGE_WINNING_NUM_KILL_ACK, KEGS_CHANGE_WINNING_NUM_KILL_ACK );

	if( m_spRoomUserManager->IsObserver( FIRST_SENDER_UID ) == false )
	{
		VERIFY_HOST( ERM_CHANGE_WINNING_NUM_KILL_ACK );
	}

	//{{ 2009. 10. 22  ������	������ ���׼���
	if( CXSLRoom::IsValidWinKillNum( m_cGameType, static_cast<char>(kPacket_.m_iWinKillNum) ) == false )
	{
		START_LOG( cerr, L"������ ų�� ���ð��� �̻��ϴ�." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOGc( m_cGameType )
			<< BUILD_LOG( kPacket_.m_iWinKillNum )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ROOM_47;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_WINNING_NUM_KILL_ACK, kPacket );
		return;
	}
	//}}

    m_cWinningNumKill = ( char )kPacket_.m_iWinKillNum;
    
    kPacket.m_iOK = NetError::NET_OK;
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_WINNING_NUM_KILL_ACK, kPacket );

    KEGS_CHANGE_WINNING_NUM_KILL_NOT kPacketNot;
    kPacketNot.m_iWinKillNum = kPacket_.m_iWinKillNum;
    BroadCast( ERM_CHANGE_WINNING_NUM_KILL_NOT, kPacketNot );
    SendRoomListInfo( NetError::ERR_ROOM_15 );
}









#ifdef SERVER_PVP_BASE_DEFENCE_TEST

	_IMPL_ON_FUNC( ERM_CHANGE_PVP_GAME_OPTION_REQ, KEGS_CHANGE_PVP_GAME_OPTION_REQ )
	{
		VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_CHANGE_PVP_GAME_OPTION_ACK, KEGS_CHANGE_PVP_GAME_OPTION_ACK );

		if( m_spRoomUserManager->IsObserver( FIRST_SENDER_UID ) == false )
		{
			VERIFY_HOST( ERM_CHANGE_PVP_GAME_OPTION_ACK );
		}

		m_PVPGameOption.m_bBaseDefence	= kPacket_.m_bBaseDefence;
		m_PVPGameOption.m_bBossCombat	= kPacket_.m_bBossCombat;

		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_PVP_GAME_OPTION_ACK, kPacket );

		KEGS_CHANGE_PVP_GAME_OPTION_NOT kPacketNot;
		kPacketNot.m_bBaseDefence	= kPacket_.m_bBaseDefence;
		kPacketNot.m_bBossCombat	= kPacket_.m_bBossCombat;
		BroadCast( ERM_CHANGE_PVP_GAME_OPTION_NOT, kPacketNot );
		
		//SendRoomListInfo( NetError::ERR_ROOM_15 );
	}

#endif SERVER_PVP_BASE_DEFENCE_TEST















_IMPL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType )
{
	std::vector<KRoomUserManager::KLeaveUserInfo>	vecDelUserUID;
	int iCount = m_spRoomUserManager->DeleteUserByGsUID( kPacket_, vecDelUserUID );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_ )
		<< BUILD_LOG( iCount )
		<< BUILD_LOG( m_spRoomUserManager->GetNumMember() );

	//����濡�� ������ ������ ������ ����.
	if( iCount == 0 )
		return;

	//���� �� ���ְ�
	for( int i = 0; i < (int)vecDelUserUID.size(); ++i )
	{
		if( m_spRoomUserManager->LeaveRoom( vecDelUserUID[i].m_iUnitUID ) == true )
		{
			DelRebirth( vecDelUserUID[i].m_iUnitUID );

			//{{ 2009. 4. 23  ������	������������Ʈ
			SendPVPUserInfo( vecDelUserUID[i].m_iUnitUID, KCommunityUserInfo::US_PVP_LOBBY );
			//}}
		}
	}

	//�׸��� ��Ƴ��� ������ ������ leave room
	for( int i = 0; i < (int)vecDelUserUID.size(); ++i )
	{
		if( IsEmpty() == false )
		{
			// ���� ��� �����鿡�� ���� ������ ������.
			KEGS_LEAVE_ROOM_NOT kPacketNot;
			kPacketNot.m_cRoomState		= GetStateID();
			kPacketNot.m_iUnitUID		= vecDelUserUID[i].m_iUnitUID;

			GetRoomSlotInfo( kPacketNot.m_vecSlot );
			BroadCast( ERM_LEAVE_ROOM_NOT, kPacketNot );
		}
	}

	if( GetStateID() == KRoomFSM::S_LOAD )
	{
		//{{ 2011. 08. 05	������	�����濡 ���� ȥ�� ���ԵǴ� ���� ����
#ifdef SERV_CODE_REFACTORING_20110804
		if( IsEmpty() == false )
		{
			CheckAllPlayerFinishLoading();
		}
#else
		// ��� ������ �ε� �������� not�� ������.
		if( m_spRoomUserManager->IsAllPlayerFinishLoading() && IsEmpty() == false )
		{
			BroadCastID( ERM_GAME_LOADING_ALL_UNIT_OK_NOT );

			// ���������� �ʱ�ȭ.
			StartPlay();

			KEGS_PLAY_START_NOT kNot;
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

	// ���������� ������ �Ǵ� ��� ó��.
	if( GetStateID() == KRoomFSM::S_PLAY && IsEmpty() == false )
	{
		if( CheckIfPlayEnd() )
		{
			EndPlay();
		}
	}

	// �濡�� ��� ������ �������� ���� �ݴ´�.
	if( IsEmpty() )
	{
		StateTransition( KRoomFSM::I_TO_CLOSE );
	}

	//��� ���� ������ �� ����Ʈ�� �����϶�� ������.
	if( GetStateID() == KRoomFSM::S_CLOSE )
	{
		SendRoomListInfo( NetError::ERR_ROOM_16 );

		//{{ 2009. 4. 23  ������	������������Ʈ
		SendAllPVPUserInfo(); // ������ ���� �ʹ�.
		//}}
	}
	else
	{
		SendRoomListInfo( NetError::ERR_ROOM_15 );

		//{{ 2009. 4. 23  ������	������������Ʈ
		SendAllPVPUserInfo(); // ���� RoomFSM�� ���� �������� ������Ʈ
		//}}
	}
}

#ifdef DUNGEON_ITEM
_IMPL_ON_FUNC( ERM_CREATE_PVP_ITEM_REQ, KEGS_CREATE_PVP_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_CREATE_PVP_ITEM_ACK, KEGS_CREATE_PVP_ITEM_ACK );

	// ������ ��尡 �ƴϸ� ó���� ���� ����.
	if( m_bIsItemMode == false )
	{
		kPacket.m_iOK = NetError::ERR_DUNGEON_ITEM_00;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CREATE_PVP_ITEM_ACK, kPacket );
		return;
	}

	// ��ӵ� �������� ���� ���� ���� ���� ������ ���� ����Ҽ� ����.
	for( int i = 0; i < static_cast<int>(m_vecDropItem.size()); ++i )
	{
		if( m_vecDropItem[i].m_bIsGet == false )
		{
			kPacket.m_iOK = NetError::ERR_DUNGEON_ITEM_01;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CREATE_PVP_ITEM_ACK, kPacket );
			return;
		}
	}	

	int iGroup = 0;
	int iItemID = 0;
	if( false == SiKDropTable()->DecisionbyAllItem(iGroup, iItemID) )
	{
		START_LOG( cerr, L"���� ��Ӿ����� ������ ����.!" )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_DUNGEON_ITEM_02;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CREATE_PVP_ITEM_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CREATE_PVP_ITEM_ACK, kPacket );

	KEGS_CREATE_PVP_ITEM_NOT kNot;
	kNot.m_iPositionIndex = kPacket_.m_iPositionIndex;
	kNot.m_kDropItemData.m_bLeft = false;
	kNot.m_kDropItemData.m_fSpeed = 1.f;
	kNot.m_kDropItemData.m_iDropItemUID = ++m_iDropItemUID;
	kNot.m_kDropItemData.m_iItemID = iItemID;

	BroadCast( ERM_CREATE_PVP_ITEM_NOT, kNot );

	// ��ӵ� ������ ������ ����..
	DROP_ITEMDATA kDropdata;
	kDropdata.m_iDropItemUID = kNot.m_kDropItemData.m_iDropItemUID;
	kDropdata.m_iGroup = iGroup;
	kDropdata.m_iItemID = kNot.m_kDropItemData.m_iItemID;
	m_vecDropItem.push_back( kDropdata );
}

_IMPL_ON_FUNC( ERM_GET_ITEM_REQ, KEGS_GET_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_GET_ITEM_ACK, KEGS_GET_ITEM_ACK );

	// ������ ��尡 �ƴϸ� ó���� ���� ����.
	if( m_bIsItemMode == false )
	{
		kPacket.m_iOK = NetError::ERR_DUNGEON_ITEM_00;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GET_ITEM_ACK, kPacket );
		return;
	}

	for( int i = 0; i < static_cast<int>(m_vecDropItem.size()); ++i )
	{
		if( m_vecDropItem[i].m_iDropItemUID == kPacket_.m_iDropItemUID &&
			m_vecDropItem[i].m_bIsGet == false )
		{
			kPacket.m_iOK = NetError::NET_OK;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GET_ITEM_ACK, kPacket );

			// ��� ������ ����ó��..
			m_vecDropItem[i].m_bIsGet = true;

			KEGS_GET_ITEM_NOT kNot;
			kNot.m_GetUnitUID = FIRST_SENDER_UID;
			kNot.m_iItemID = m_vecDropItem[i].m_iItemID;
			kNot.m_iDropItemUID = m_vecDropItem[i].m_iDropItemUID;

			//{{ 2012. 07. 17	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2_2
			kNot.m_bIsItemSuccess = SiKDropTable()->SuccessfullyGetTheItem( static_cast<CXSLRoom::PVP_GAME_TYPE>( m_cGameType ),
																			m_spRoomUserManager->GetMyTeamKillNum( FIRST_SENDER_UID ),
																			m_spRoomUserManager->GetEnemyTeamKillNum( FIRST_SENDER_UID ) );
#else

			switch( m_cGameType )
			{
			case CXSLRoom::PGT_SURVIVAL:	// ���� Ȯ���� ������ 5%
				{
					// ����Ȯ������ ������ ������ ����.
					if( static_cast<float>(rand()%100) > SiKDropTable()->GetPVPDropFailRate(KDropTable::PF_LOSETEAM) )
						kNot.m_bIsItemSuccess = true;
				}
				break;
			case CXSLRoom::PGT_TEAM:
			case CXSLRoom::PGT_TEAM_DEATH:
				{
					int iMyTeam = 0;

					// ���� �Ҽӵ� ���� ����.
					if( true == m_spRoomUserManager->GetTeam( FIRST_SENDER_UID, iMyTeam ) )
					{
						float fResultRate = 0.f;
						bool  bStepSuccess = true;

						int iOtherTeam = 0;

						int iUnitNumbyMyTeam = 0;
						int iUnitNumbyOtherTeam = 0;

						// ���� �� Ű���� ����
						switch( iMyTeam )
						{
						case CXSLRoom::TN_RED:
							{
								iOtherTeam = CXSLRoom::TN_BLUE;

								// ���� �ο��� üũ
								if( false == m_spRoomUserManager->GetTeamNum( iUnitNumbyMyTeam, iUnitNumbyOtherTeam ) )
								{
									bStepSuccess = false;

									START_LOG( cerr, L"���� ������ ������ ����." )
										<< BUILD_LOG( FIRST_SENDER_UID )
										<< END_LOG;
								}
							}
							break;
						case CXSLRoom::TN_BLUE:
							{
								iOtherTeam = CXSLRoom::TN_RED;

								// ���� �ο��� üũ
								if( false == m_spRoomUserManager->GetTeamNum( iUnitNumbyOtherTeam, iUnitNumbyMyTeam ) )
								{
									bStepSuccess = false;

									START_LOG( cerr, L"���� ������ ������ ����." )
										<< BUILD_LOG( FIRST_SENDER_UID )
										<< END_LOG;
								}
							}							
							break;
						default:
							{
								bStepSuccess = false;

								START_LOG( cerr, L"������ ������ ����." )
									<< BUILD_LOG( FIRST_SENDER_UID )
									<< BUILD_LOG( iMyTeam )
									<< END_LOG;								
							}
							break;
						}

						if( bStepSuccess == true )
						{
							int iKillNumbyMyTeam = m_spRoomUserManager->GetTeamScore( iMyTeam );
							int iKillNumbyOtherTeam = m_spRoomUserManager->GetTeamScore( iOtherTeam );

							// ��/�� �� Ȯ�� ����
							if( iKillNumbyMyTeam <= iKillNumbyOtherTeam )
							{
								fResultRate = SiKDropTable()->GetPVPDropFailRate( KDropTable::PF_LOSETEAM );
							}
							else
							{
								fResultRate = SiKDropTable()->GetPVPDropFailRate( KDropTable::PF_WINTEAM );
							}

							// �ο����� Ȯ�� ����
							int iResSum = iUnitNumbyOtherTeam - iUnitNumbyMyTeam;
							if( iResSum < 0 )	// ����� ���� �ο��� �������
							{
								fResultRate += SiKDropTable()->GetPVPDropFailRate( KDropTable::PF_INC_NUM ) * static_cast<float>(iResSum);

								if( fResultRate < 0.f )
									fResultRate = 0.f;
							}
							else if( iResSum > 0 ) // ����� ���� �ο��� �������
							{
								fResultRate += SiKDropTable()->GetPVPDropFailRate( KDropTable::PF_INC_NUM ) * static_cast<float>(iResSum);
							}

							if( static_cast<float>(rand()%100) > fResultRate )
								kNot.m_bIsItemSuccess = true;
						}
						else
						{
							kNot.m_bIsItemSuccess = false;
						}
					}
					else
					{
						START_LOG( cerr, L"���� �Ҽ��� ���� ������ ����." )
							<< BUILD_LOG( FIRST_SENDER_UID )
							<< END_LOG;

						kNot.m_bIsItemSuccess = false;
					}
				}
				break;
			}
#endif SERV_2012_PVP_SEASON2_2
			//}}

			BroadCast( ERM_GET_ITEM_NOT, kNot );

			break;
		}
	}
}
#endif DUNGEON_ITEM


//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
IMPL_ON_FUNC( ERM_UPDATE_PVP_PLAY_UNIT_INFO_NOT )
{
	if( GetStateID() != KRoomFSM::S_LOAD )
	{
		START_LOG( cerr, L"���� ����. Name : " << m_strName )
			<< L"    ���� ���� : " << GetStateIDString() << dbg::endl
			<< L"    ��� ���� : KRoomFSM::S_LOAD"
			<< END_LOG;
		return;
	}

	//{{ 2011. 08. 03	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	if( m_spRoomUserManager->SetPvpPlayResultInfo( FIRST_SENDER_UID, kPacket_.m_kPvpUnitInfo ) == false )
	{
		START_LOG( cerr, L"���� ���� �ʴ� ���� ������ ������Ʈ �Ϸ��� �Ͽ����ϴ�!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

_IMPL_ON_FUNC( ERM_PVP_NPC_UNIT_RE_BIRTH_POS_REQ, KEGS_PVP_NPC_UNIT_RE_BIRTH_POS_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_PVP_NPC_UNIT_RE_BIRTH_POS_ACK, KEGS_PVP_NPC_UNIT_RE_BIRTH_POS_ACK );

	// ���� NPC���� �˻�!
	if( m_spRoomUserManager->IsPvpNpc( kPacket_.m_iPvpNpcUnitUID ) == false )
	{
        START_LOG( cerr, L"���� NPC�� �ƴѵ� ��Ȱ�� �Ϸ��� �ϳ�!? ��������!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iPvpNpcUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ROOM_17;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PVP_NPC_UNIT_RE_BIRTH_POS_ACK, kPacket );
		return;
	}

	if( m_spRoomUserManager->SetRebirthPos( kPacket_.m_iPvpNpcUnitUID, kPacket_.m_iStartPosIndex ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_17;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PVP_NPC_UNIT_RE_BIRTH_POS_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PVP_NPC_UNIT_RE_BIRTH_POS_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_PVP_NPC_UNIT_INFO_TO_SERVER_REQ, KEGS_PVP_NPC_UNIT_INFO_TO_SERVER_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_PVP_NPC_UNIT_INFO_TO_SERVER_ACK, KEGS_PVP_NPC_UNIT_INFO_TO_SERVER_ACK );

	std::map< UidType, float >::const_iterator mit;
	for( mit = kPacket_.m_mapPvpNpcInfo.begin(); mit != kPacket_.m_mapPvpNpcInfo.end(); ++mit )
	{
		if( m_spRoomUserManager->SetHP( mit->first, mit->second ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_17;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PVP_NPC_UNIT_INFO_TO_SERVER_ACK, kPacket );
			return;
		}
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PVP_NPC_UNIT_INFO_TO_SERVER_ACK, kPacket );

	if( m_spRoomUserManager->IsAllPlayerHPReported() )
	{
		START_LOG( clog2, L"��� ���� HP ����Ʈ." )
			<< BUILD_LOG( GetStateID() )
			<< BUILD_LOG( GetStateIDString() );

		EndPlay();
	}
}

#endif SERV_PVP_NEW_SYSTEM
//}}


//{{ 2012. 02. 14    ��μ�   PVP ���� Item ���� ��ġ ����
#ifdef SERV_PVP_ITEM_CHANGE_POS
_IMPL_ON_FUNC( ERM_CHANGE_PVP_ITEM_POS_REQ, KEGS_CHANGE_PVP_ITEM_POS_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_CHANGE_PVP_ITEM_POS_ACK, KEGS_CHANGE_PVP_ITEM_POS_ACK );

	bool bExist = false;

	// ������ item ���� Ȯ�� ����
	BOOST_TEST_FOREACH( const DROP_ITEMDATA, kDropItem, m_vecDropItem )
	{
		if( kDropItem.m_iDropItemUID == kPacket_.m_iItemUID )
		{
			bExist = true;
			break;
		}
	}

	// ��ӵ� item �� �����Ѵٸ�
	if( bExist == true )
	{
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_PVP_ITEM_POS_ACK, kPacket );

		KEGS_CHANGE_PVP_ITEM_POS_NOT kNot;
		kNot.m_iItemUID = kPacket_.m_iItemUID;
		kNot.m_iPos = kPacket_.m_iPos;

		BroadCast( ERM_CHANGE_PVP_ITEM_POS_NOT, kNot );
		return;
	}

	kPacket.m_iOK = NetError::ERR_ITEM_04;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_PVP_ITEM_POS_ACK, kPacket );
}
#endif SERV_PVP_ITEM_CHANGE_POS
//}}

//{{ 2008. 3. 4  ������  ����� �˻�
void KPvPRoom::AbuserLogToDB( KRoomUserPtr spRoomUser, int iVSPoint, int iEXP )
{
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"�� ���� ������ �̻�" )
			<< END_LOG;
		return;
	}

	//{{ 2011. 07. 26	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	if( spRoomUser->IsPvpNpc() == true )
		return;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	// �г������� ����� �˻�
	KRoomUserInfo kRoomUserInfo;
	spRoomUser->GetRoomUserInfo( kRoomUserInfo );
	CTime kEndGameTime = CTime::GetCurrentTime();

	KELOG_INSERT_ABUSER_LOG_NOT kPacketLog;
	kPacketLog.m_iUserUID		 = kRoomUserInfo.m_iOwnerUserUID;
	kPacketLog.m_iUnitUID		 = kRoomUserInfo.m_nUnitUID;
	kPacketLog.m_wstrIP			 = kRoomUserInfo.m_wstrIP;
	kPacketLog.m_iGameType		 = GetPlayMode();
	kPacketLog.m_fPlayTimeLimit  = GetPlayTimeLimit();
	kPacketLog.m_bPublic		 = m_bPublic;
	kPacketLog.m_iPlayWithMember = m_spRoomUserManager->GetNumMember();
	kPacketLog.m_wstrEndGameTime = (const wchar_t*)kEndGameTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	kPacketLog.m_iPlayTime		 = static_cast<int>(spRoomUser->GetPlayTime());
	kPacketLog.m_iRewardVP		 = iVSPoint;
	kPacketLog.m_iRewardEXP		 = iEXP;
	kPacketLog.m_iKillCount		 = spRoomUser->GetNumMDKill();
	kPacketLog.m_iDeathCount	 = spRoomUser->GetNumDie();
	SendToLogDB( ELOG_INSERT_ABUSER_LOG_NOT, kPacketLog );

	START_LOG( clog2, L"����� ���ɷȾ�!" )
		<< BUILD_LOG( kPacketLog.m_iUserUID )
		<< BUILD_LOG( kPacketLog.m_iUnitUID )
		<< BUILD_LOG( kPacketLog.m_wstrIP )
		<< BUILD_LOG( kPacketLog.m_iGameType )
		<< BUILD_LOG( kPacketLog.m_fPlayTimeLimit )
		<< BUILD_LOG( kPacketLog.m_bPublic )
		<< BUILD_LOG( kPacketLog.m_iPlayWithMember )
		<< BUILD_LOG( kPacketLog.m_wstrEndGameTime )
		<< BUILD_LOG( kPacketLog.m_iPlayTime )
		<< BUILD_LOG( kPacketLog.m_iRewardVP )
		<< BUILD_LOG( kPacketLog.m_iRewardEXP )
		<< BUILD_LOG( kPacketLog.m_iKillCount )
		<< BUILD_LOG( kPacketLog.m_iDeathCount )
		;
}
//}}

//{{ 2008. 3. 25  ������  PVP �α�
void KPvPRoom::PvPLogToFile( KRoomUserPtr spRoomUser, int iVSPoint, int iEXP, int iResult )
{
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"�� ���� ������ �̻�, ���� �α׸� ����� �� �����ϴ�." )
			<< END_LOG;
		return;
	}

	// ���� �·� �α�
	KRoomUserInfo kRoomUserInfo;
	spRoomUser->GetRoomUserInfo( kRoomUserInfo );

	CTime kEndGameTime = CTime::GetCurrentTime();

	//{{ 2010. 06. 21  ������	��� ������
	//////////////////////////////////////////////////////////////////////////
#ifdef SERV_STATISTICS_THREAD
	KE_LOCAL_LOG_PVP_NOT kNot;
	kNot.m_iPVPChannelClass = GetPVPChannelClass();
	kNot.m_iOwnerUserUID	= kRoomUserInfo.m_iOwnerUserUID;
	kNot.m_wstrIP			= kRoomUserInfo.m_wstrIP;
	kNot.m_nUnitUID			= kRoomUserInfo.m_nUnitUID;
	kNot.m_wstrNickName		= kRoomUserInfo.m_wstrNickName;
	kNot.m_iUnitClass		= static_cast<int>(kRoomUserInfo.m_cUnitClass);
	kNot.m_iRoomUID			= GetUID();
	kNot.m_iPlayMode		= GetPlayMode();
	kNot.m_bIsItemMode		= m_bIsItemMode;
	kNot.m_bPublic			= !m_bPublic;
	kNot.m_iNumMember		= m_spRoomUserManager->GetNumMember();
	kNot.m_fPlayTimeLimit	= GetPlayTimeLimit();
	kNot.m_wstrEndGameTime	= (const wchar_t*)kEndGameTime.Format(_T("%H:%M:%S"));
	kNot.m_iPlayTime		= static_cast<int>(spRoomUser->GetPlayTime());
	kNot.m_iVSPoint			= iVSPoint;
	kNot.m_iEXP				= iEXP;
	kNot.m_iNumMDKill		= spRoomUser->GetNumMDKill();
	kNot.m_iNumDie			= spRoomUser->GetNumDie();
	kNot.m_iResult			= iResult;
	KSIManager.QueueingEvent( E_LOCAL_LOG_PVP_NOT, kNot );
	//////////////////////////////////////////////////////////////////////////	
#else
	//////////////////////////////////////////////////////////////////////////	
#define KLocAlign std::setw(8) << std::setiosflags( std::ios::left )

	std::wfstream& fout = KSIManager.GetLocalLog().LocalLogStm( KStatisticsLocalLog::LOG_PVP );	

	fout << L"\n";
	fout << KLocAlign << GetPVPChannelClass() << L"\t";
	fout << KLocAlign << kRoomUserInfo.m_iOwnerUserUID << L"\t";
	fout << KLocAlign << kRoomUserInfo.m_wstrIP << L"\t";
	fout << KLocAlign << kRoomUserInfo.m_nUnitUID << L"\t";
	fout << KLocAlign << kRoomUserInfo.m_wstrNickName << L"\t";
	fout << KLocAlign << static_cast<int>(kRoomUserInfo.m_cUnitClass) << L"\t";
	fout << KLocAlign << GetUID() << L"\t";
	fout << KLocAlign << GetPlayMode() << L"\t";
	//{{ 2010. 01. 06  ������	��������
	fout << KLocAlign << m_bIsItemMode << L"\t";
	//}}
	fout << KLocAlign << !m_bPublic << L"\t";
	fout << KLocAlign << m_spRoomUserManager->GetNumMember() << L"\t";
	fout << KLocAlign << GetPlayTimeLimit() << L"\t";
	fout << KLocAlign << (const wchar_t*)kEndGameTime.Format(_T("%H:%M:%S")) << L"\t";
	fout << KLocAlign << static_cast<int>(spRoomUser->GetPlayTime()) << L"\t";
	fout << KLocAlign << iVSPoint << L"\t";
	fout << KLocAlign << iEXP << L"\t";
	fout << KLocAlign << spRoomUser->GetNumMDKill() << L"\t";
	fout << KLocAlign << spRoomUser->GetNumDie() << L"\t";
	fout << KLocAlign << iResult << L"\t";

	fout.flush();
	//////////////////////////////////////////////////////////////////////////	
#endif SERV_STATISTICS_THREAD
	//}}
}
//}}

//{{ 2012. 07. 16	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2_2
void KPvPRoom::RevengeBuffProcess( IN const UidType& iMyUnitUID, IN int iPrevMyTeamKillNumGab, IN int iNowMyTeamKillNumGab )
{
	iPrevMyTeamKillNumGab	= min( 3, iPrevMyTeamKillNumGab );
	iNowMyTeamKillNumGab	= min( 3, iNowMyTeamKillNumGab );

	iPrevMyTeamKillNumGab	= max( -3, iPrevMyTeamKillNumGab );
	iNowMyTeamKillNumGab	= max( -3, iNowMyTeamKillNumGab );
	
	if( iPrevMyTeamKillNumGab == iNowMyTeamKillNumGab )
		return;

	KECN_PARTY_BUFF_UPDATE_NOT kPacketToMyTeam, kPacketToEnemyTeam;

	// �̹� �ɷ� �ִ� ������ ������ ó���Ѵ�.
	// �츮�� ų ���� �� ���Ҵٸ�, ������� ������ �ɷ����� ���̴�.
	if( 0 < iPrevMyTeamKillNumGab )		
	{
		kPacketToEnemyTeam.m_setDeactivateBuffList.insert( CXSLBuffManager::BTI_BUFF_PVP_REVENGE_MODE_LEVEL_1 );
		kPacketToEnemyTeam.m_setDeactivateBuffList.insert( CXSLBuffManager::BTI_BUFF_PVP_REVENGE_MODE_LEVEL_2 );
		kPacketToEnemyTeam.m_setDeactivateBuffList.insert( CXSLBuffManager::BTI_BUFF_PVP_REVENGE_MODE_LEVEL_3 );
	}
	else if( iPrevMyTeamKillNumGab < 0)	
	{
		kPacketToMyTeam.m_setDeactivateBuffList.insert( CXSLBuffManager::BTI_BUFF_PVP_REVENGE_MODE_LEVEL_1 );
		kPacketToMyTeam.m_setDeactivateBuffList.insert( CXSLBuffManager::BTI_BUFF_PVP_REVENGE_MODE_LEVEL_2 );
		kPacketToMyTeam.m_setDeactivateBuffList.insert( CXSLBuffManager::BTI_BUFF_PVP_REVENGE_MODE_LEVEL_3 );
	}

	// ���ο� ������ ������ ó���Ѵ�.
	if( 0 < iNowMyTeamKillNumGab )
	{
		KBuffInfo kBuffInfo;
		kBuffInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID		= CXSLBuffManager::BTI_BUFF_PVP_REVENGE_MODE_LEVEL_1 - 1 + abs( iNowMyTeamKillNumGab );
		CTime tCurrentTime		= CTime::GetCurrentTime();
		kBuffInfo.m_iBuffStartTime	= tCurrentTime.GetTime();
		kPacketToEnemyTeam.m_mapActivateBuffList.insert( std::map<int, KBuffInfo>::value_type( kBuffInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID, kBuffInfo ) );
	}
	else if( iNowMyTeamKillNumGab < 0 )
	{		
		KBuffInfo kBuffInfo;
		kBuffInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID		= CXSLBuffManager::BTI_BUFF_PVP_REVENGE_MODE_LEVEL_1 - 1 + abs( iNowMyTeamKillNumGab );		
		CTime tCurrentTime		= CTime::GetCurrentTime();
		kBuffInfo.m_iBuffStartTime	= tCurrentTime.GetTime();
		kPacketToMyTeam.m_mapActivateBuffList.insert( std::map<int, KBuffInfo>::value_type( kBuffInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID, kBuffInfo ) );
	}

	int						iMyTeamIndex;
	std::vector<UidType>	vecUnitUIDList;

	if( ( kPacketToMyTeam.m_setDeactivateBuffList.empty() == false ) || ( kPacketToMyTeam.m_mapActivateBuffList.empty() == false ) )
	{
		m_spRoomUserManager->GetTeam( iMyUnitUID, iMyTeamIndex );
		m_spRoomUserManager->GetTeamMemberList( static_cast<CXSLRoom::TEAM_NUM>( iMyTeamIndex ), false, vecUnitUIDList );
		BOOST_TEST_FOREACH( const UidType&, iUnitUID, vecUnitUIDList )
		{
			UidType iGSUID;
			if( m_spRoomUserManager->GetGSUID( iUnitUID, iGSUID ) == false )
				continue;
			SendToGSCharacter( iGSUID, iUnitUID, ECN_PARTY_BUFF_UPDATE_NOT, kPacketToMyTeam );
		}
	}

	if( ( kPacketToEnemyTeam.m_setDeactivateBuffList.empty() == false ) || ( kPacketToEnemyTeam.m_mapActivateBuffList.empty() == false ) )
	{
		m_spRoomUserManager->GetTeam( iMyUnitUID, iMyTeamIndex );
		if( iMyTeamIndex == CXSLRoom::TN_RED )
		{
			iMyTeamIndex = CXSLRoom::TN_BLUE;
		}
		else
		{
			iMyTeamIndex = CXSLRoom::TN_RED;
		}
		m_spRoomUserManager->GetTeamMemberList( static_cast<CXSLRoom::TEAM_NUM>( iMyTeamIndex ), false, vecUnitUIDList );
		BOOST_TEST_FOREACH( const UidType&, iUnitUID, vecUnitUIDList )
		{
			UidType iGSUID;
			if( m_spRoomUserManager->GetGSUID( iUnitUID, iGSUID ) == false )
				continue;
			SendToGSCharacter( iGSUID, iUnitUID, ECN_PARTY_BUFF_UPDATE_NOT, kPacketToEnemyTeam );
		}
	}
}

#ifdef SERV_FIX_REVENGE_BUFF// �۾���¥: 2013-08-09	// �ڼ���
int	KPvPRoom::GetTeamRevengeBuffID( IN const UidType iMyUnitUID ) const
{
	int iNowMyTeamKillNumGab	= m_spRoomUserManager->GetMyTeamKillNum( iMyUnitUID ) - m_spRoomUserManager->GetEnemyTeamKillNum( iMyUnitUID );
	iNowMyTeamKillNumGab		= max( -3, iNowMyTeamKillNumGab );

	if( iNowMyTeamKillNumGab < 0 )
	{
		return CXSLBuffManager::BTI_BUFF_PVP_REVENGE_MODE_LEVEL_1 - 1 + abs( iNowMyTeamKillNumGab );
	}

	return CXSLBuffManager::BTI_NONE;
}
#endif // SERV_FIX_REVENGE_BUFF

#endif SERV_2012_PVP_SEASON2_2
//}}

//{{ 2012. 09. 02	�ڼ���	Merge ( �������� Ŭ���̾�Ʈ ���۵����� UDP ��Ŷ�� ���ƿ��� �ʴ� ������ �߰ߵǸ� �������� ű�Ѵ�. // 2012.06.11 lygan_������ )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
_IMPL_ON_FUNC( ERM_UDP_CHECK_KICK_USER_NOT, KEGS_UDP_CHECK_KICK_USER_NOT )
{
	VERIFY_ROOM_STATE( ( 3, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ), ERM_LEAVE_GAME_ACK, KEGS_LEAVE_GAME_ACK );

	//{{ 2012. 10. 07	�ڼ���	������ �α� �߰�
#ifdef UDP_CAN_NOT_SEND_USER_KICK_LOG
	RecordLanBugUser( kPacket_ );
#endif UDP_CAN_NOT_SEND_USER_KICK_LOG
	//}}

	if (kPacket_.m_bNowKickOut == true)
	{	
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( kPacket_.m_iAccusationUnitUID );
		if( spRoomUser == NULL )
		{
			START_LOG( cout, L"����, Ȥ�� ���� �ǿ� ���� ������ ƨ��� Ȯ��")
				<< BUILD_LOG( kPacket_.m_iMyUnitUID )
				<< BUILD_LOG( kPacket_.m_iAccusationUnitUID )
				<< END_LOG;
		}
		else
		{			
			if( kPacket_.m_iMyUnitUID != kPacket_.m_iAccusationUnitUID )
			{
				// ������ ��� ������ ƨ�ܳ� ���ΰ�?
				if( SiKGameSysVal()->GetLanBugOut1() == true )
				{
					START_LOG( cout, L"����, Ȥ�� ���� �ǿ� ���� ������ ƨ��� Ȯ��")
						<< BUILD_LOG( kPacket_.m_iMyUnitUID )
						<< BUILD_LOG( kPacket_.m_iAccusationUnitUID )
						<< BUILD_LOG( spRoomUser->GetNickName() )
						<< END_LOG;

					BroadCast( ERM_UDP_KICK_STATE_CHANGE_FIELD_NOT, kPacket_.m_iAccusationUnitUID );
					//SendToGSCharacter( spRoomUser->GetGSUID(), kPacket_.m_iAccusationUnitUID, ERM_UDP_KICK_STATE_CHANGE_FIELD_NOT );
				}
			}
			else
			{
				// ������ �ǿ� ������ ƨ�ܳ� ���ΰ�?
				if( SiKGameSysVal()->GetLanBugOut2() == true )
				{
					START_LOG( cout, L"����, Ȥ�� ���� �ǿ� ���� ������ ƨ��� Ȯ��")
						<< BUILD_LOG( kPacket_.m_iMyUnitUID )
						<< BUILD_LOG( kPacket_.m_iAccusationUnitUID )
						<< BUILD_LOG( spRoomUser->GetNickName() )
						<< END_LOG;

					BroadCast( ERM_UDP_KICK_STATE_CHANGE_FIELD_NOT, kPacket_.m_iAccusationUnitUID );
					//SendToGSCharacter( spRoomUser->GetGSUID(), kPacket_.m_iAccusationUnitUID, ERM_UDP_KICK_STATE_CHANGE_FIELD_NOT );
				}
			}
		}

//		int iReason = NetError::NOT_LEAVE_ROOM_REASON_01; // ���� ���� ���� ���� ���ؾ� �Ѵ�.
//
//
//		// �̸� PlayTime�� ���Ѵ�. 
//		// [����1] LeaveGame()�� ȣ���� �� EndPlay()�� ȣ��Ǿ�� �ð��� �������� ����.
//		// [����2] ���� KRoomUser::EndPlay()�Լ����� PlayTime�� ���ϰ��ִ�.
//		float fPlayTime = 0.0f;
//		m_kRoomUserManager.GetPlayTime( kPacket_.m_iAccusationUnitUID, fPlayTime );
//
//		START_LOG( clog2, L"��Ż �÷��� Ÿ��[���ͼ���]" )
//			<< BUILD_LOG( static_cast<int>(fPlayTime) );
//
//		if( !m_kRoomUserManager.LeaveGame( kPacket_.m_iAccusationUnitUID ) )
//		{
//			kPacket.m_iOK = NetError::ERR_ROOM_19;
//			SendToGSCharacter( LAST_SENDER_UID, kPacket_.m_iAccusationUnitUID, ERM_LEAVE_GAME_ACK, kPacket );
//			return;
//		}
//
//		// ��Ż�� ���.
//		if( GetStateID() == KRoomFSM::S_PLAY )
//		{
//			//������ ��Ż ���ó��
//			KRoomUserPtr spRoomUser = m_kRoomUserManager.GetUser( kPacket_.m_iAccusationUnitUID );
//
//			if( !spRoomUser )
//			{
//				spRoomUser = m_kRoomUserManager.GetUser( kPacket_.m_iAccusationUnitUID, KRoomUserManager::UT_OBSERVER );
//			}
//
//			if( !spRoomUser )
//			{
//				START_LOG( cerr, L"�� ���� ������ �̻�" )
//					<< BUILD_LOG( kPacket_.m_iAccusationUnitUID )
//					<< END_LOG;
//			}
//			else
//			{
//				if( spRoomUser->IsObserver() == false )
//				{
//					KERM_UPDATE_PVP_UNIT_INFO_NOT kPacket;
//					//{{ 2009. 11. 16  ������	�����������Ʈ
//					kPacket.m_iMemberCount = m_kRoomUserManager.GetNumMember();
//					//}}
//					kPacket.m_bOutRoom = false;
//					//{{ 2011. 07. 25	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
//					kPacket.m_iCurrentRating = spRoomUser->GetRating();
//					kPacket.m_iCurrentMaxRating = spRoomUser->GetMaxRating();
//					kPacket.m_iCurrentRPoint = spRoomUser->GetRPoint();
//					kPacket.m_iCurrentAPoint = spRoomUser->GetAPoint();
//#endif SERV_PVP_NEW_SYSTEM
//					//}}
//
//					//{{ 2012. 09. 22	�ڼ���	2012 ���� ����2
//#ifdef SERV_2012_PVP_SEASON2
//					kPacket.m_cRank				= spRoomUser->GetCurrentRank();
//					kPacket.m_cRankForServer	= spRoomUser->GetCurrentRankForServer();
//#endif SERV_2012_PVP_SEASON2
//					//}}
//
//					switch( m_cGameType )
//					{
//					case CXSLRoom::PGT_TEAM_DEATH:
//						//{{ 2011. 07. 25	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
//						SiKResultProcess()->Result_TEAM_DEATH_BREAK( m_kRoomUserManager, spRoomUser, NetError::NOT_LEAVE_ROOM_REASON_00, GetMatchType(), GetPvpNpcType(), kPacket );
//#else
//						SiKResultProcess()->Result_TEAM_DEATH_BREAK( m_kRoomUserManager, spRoomUser, kPacket );
//#endif SERV_PVP_NEW_SYSTEM
//						//}}
//						break;
//
//					case CXSLRoom::PGT_SURVIVAL:
//						//{{ 2011. 07. 25	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
//						SiKResultProcess()->Result_SURVIVAL_BREAK( m_kRoomUserManager, spRoomUser, NetError::NOT_LEAVE_ROOM_REASON_00, GetMatchType(), GetPvpNpcType(), kPacket );
//#else
//						SiKResultProcess()->Result_SURVIVAL_BREAK( m_kRoomUserManager, spRoomUser, kPacket );
//#endif SERV_PVP_NEW_SYSTEM
//						//}}
//						break;
//					}
//
//					//������ ��Ż�� �� ��ϵ�����
//					kPacket.m_iLose		= 1;
//
//					//������ ��Ż�� �� ��ϵ�����
//					if( GetPVPChannelClass() == KPVPChannelInfo::PCC_FREE  || 
//						//{{ 2011. 07. 26	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
//						GetPVPChannelClass() == KPVPChannelInfo::PCC_TOURNAMENT  || 
//#endif SERV_PVP_NEW_SYSTEM
//						//}}
//						GetPVPChannelClass() == KPVPChannelInfo::PCC_PLAY )
//					{
//						kPacket.m_iEXP = 0;
//						//{{ 2011. 07. 11	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
//						kPacket.m_iRating = 0;
//						kPacket.m_iRPoint = 0;
//						kPacket.m_iAPoint = 0;
//#else
//						kPacket.m_iVSPoint = 0;
//#endif SERV_PVP_NEW_SYSTEM
//						//}}
//						kPacket.m_iLose	= 0;
//					}
//
//					SendToGSCharacter( LAST_SENDER_UID, kPacket_.m_iAccusationUnitUID, ERM_UPDATE_PVP_UNIT_INFO_NOT, kPacket );
//
//					//////////////////////////////////////////////////////////////////// ���			
//					KStatisticsKey kKey;
//					kKey.m_vecIntKey.push_back( static_cast<int>(m_uiRoomListID) );
//					KSIManager.IncreaseCount( KStatistics::SI_LOC_PVP, kKey, KStatistics::SLP_GIVE_UP, 1 );
//
//					if( GetPVPChannelClass() != KPVPChannelInfo::PCC_FREE  &&  
//						//{{ 2011. 07. 26	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
//						GetPVPChannelClass() != KPVPChannelInfo::PCC_TOURNAMENT  &&  
//#endif SERV_PVP_NEW_SYSTEM
//						//}}
//						GetPVPChannelClass() != KPVPChannelInfo::PCC_PLAY )
//					{
//						// ���� DB���
//						KERM_PVP_USER_STATISTICS_NOT kPacketStats;
//						kPacketStats.m_cGameType		 = m_cGameType;
//						kPacketStats.m_cGameResult		 = KERM_PVP_USER_STATISTICS_NOT::UGR_DROP;
//						kPacketStats.m_iGamePlayTime	 = static_cast<int>(fPlayTime);
//						kPacketStats.m_bIntrude			 = m_kRoomUserManager.GetIsIntrude(	kPacket_.m_iAccusationUnitUID );
//						kPacketStats.m_sWorldID			 = m_sWorldID;
//						kPacketStats.m_iKillNum			 = m_kRoomUserManager.GetKillNumber( kPacket_.m_iAccusationUnitUID );
//						SendToGSCharacter( LAST_SENDER_UID, kPacket_.m_iAccusationUnitUID, ERM_PVP_USER_STATISTICS_NOT, kPacketStats );
//					}
//
//					// PVP ���� �·� �α� [����� üũ]
//					PvPLogToFile( spRoomUser, 0, 0, 2 );
//				}
//			}
//		}
//
//		kPacket.m_iOK = NetError::NET_OK;	
//		SendToGSCharacter( LAST_SENDER_UID, kPacket_.m_iAccusationUnitUID, ERM_LEAVE_GAME_ACK, kPacket );
//
//		//{{ 2009. 4. 23  ������	������������Ʈ
//		SendPVPUserInfo( kPacket_.m_iAccusationUnitUID, KCommunityUserInfo::US_PVP_WAIT );
//		//}}
//
//		if( GetStateID() == KRoomFSM::S_LOAD )
//		{
//			//{{ 2011. 08. 05	������	�����濡 ���� ȥ�� ���ԵǴ� ���� ����
//#ifdef SERV_CODE_REFACTORING_20110804
//			CheckAllPlayerFinishLoading();
//#else
//			// ��� ������ �ε� �������� not�� ������.
//			if( m_kRoomUserManager.IsAllPlayerFinishLoading() )
//			{
//				BroadCastID( ERM_GAME_LOADING_ALL_UNIT_OK_NOT );
//
//				// ���������� �ʱ�ȭ.
//				StartPlay();
//
//				KEGS_PLAY_START_NOT kNot;
//				kNot.m_RoomState = GetStateID();
//				GetRoomSlotInfo( kNot.m_vecSlot );
//
//				// ��� �������� �÷��� ������ �˸�.
//				BroadCast( ERM_PLAY_START_NOT, kNot );
//
//				// ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
//				SendRoomListInfo( NetError::ERR_ROOM_15 );
//
//				//{{ 2009. 4. 23  ������	������������Ʈ
//				SendAllPVPUserInfo();
//				//}}
//			}
//#endif SERV_CODE_REFACTORING_20110804
//			//}}        
//		}
//
//		if( GetStateID() == KRoomFSM::S_PLAY )
//		{
//			if( CheckIfPlayEnd() )
//			{
//				if( m_kRoomUserManager.GetNumPlaying() >= 1 )
//				{
//					ReserveEndPlay();
//					//EndPlay();
//				}
//				else
//				{
//					//�߸��� ��Ȳ���� �������� ���� ������ ������ �����ΰ��
//					//��������ó���� ����.
//					EndPlay();
//					EndGame();
//				}
//			}
//		}
//
//		//���â�� ���������� ���� �������ó��
//		if( GetStateID() == KRoomFSM::S_RESULT )
//		{
//			CheckResultSuccess();
//		}
//
//		// ���� ��� �����鿡�� ���� ������ ������.
//		KEGS_LEAVE_GAME_NOT kPacketNot;
//		kPacketNot.m_cRoomState = GetStateID();
//		kPacketNot.m_iUnitUID = kPacket_.m_iAccusationUnitUID;
//		GetRoomSlotInfo( kPacketNot.m_vecSlot );
//		GetRoomSlotInfo( kPacketNot.m_vecObserverSlot, KRoomUserManager::UT_OBSERVER );
//		BroadCast( ERM_LEAVE_GAME_NOT, kPacketNot );

//		//{{ 2012. 09. 05	�ڼ���	������ �α� �߰�
//#ifdef UDP_CAN_NOT_SEND_USER_KICK_LOG
//		SendToLogDB( DBE_UDP_CHECK_KICK_USER_NOT, kPacket_ );
//#endif UDP_CAN_NOT_SEND_USER_KICK_LOG
//		//}}
	}
//	else // UDP ��Ŷ �ν����� 50�� �̻��� �����鿡 ���ؼ��� ���ǰ� �ʿ��ϴ�.
//	{
//		//{{ 2012. 09. 05	�ڼ���	������ �α� �߰�
//#ifdef UDP_CAN_NOT_SEND_USER_KICK_LOG
//		SendToLogDB( DBE_UDP_CHECK_KICK_USER_NOT, kPacket_ );
//#endif UDP_CAN_NOT_SEND_USER_KICK_LOG
//		//}}
//	}
	/*kPacket.m_iOK = NetError::ERR_ITEM_04;
	SendToGSCharacter( LAST_SENDER_UID, kPacket_.m_iAccusationUnitUID, ERM_CHANGE_PVP_ITEM_POS_ACK, kPacket );*/
}
#endif UDP_CAN_NOT_SEND_USER_KICK
//}}

#ifdef SERV_NEW_PVPROOM_PROCESS
_IMPL_ON_FUNC( ERM_CHANGE_PVP_ROOM_PUBLIC_REQ, KEGS_CHANGE_PVP_ROOM_PUBLIC_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_CHANGE_PVP_ROOM_PUBLIC_ACK, KEGS_CHANGE_PVP_ROOM_PUBLIC_ACK );

	if( m_spRoomUserManager->IsObserver( FIRST_SENDER_UID ) == false )
	{
		// �������� üũ�Ѵ�.
		VERIFY_HOST( ERM_CHANGE_SLOT_OPEN_ACK );
	}

	if( kPacket_.m_bPublic == false )
	{
		m_wstrPassword = kPacket_.m_wstrPassword;
	}

	m_bPublic = kPacket_.m_bPublic;

	// ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
	SendRoomListInfo( NetError::ERR_ROOM_15 );

	START_LOG( clog, L"::: Change Room Info ::: " )
		<< BUILD_LOGc( kPacket_.m_bPublic )
		;

	KEGS_CHANGE_PVP_ROOM_PUBLIC_NOT kNot;
	kNot.m_bPublic = kPacket_.m_bPublic;
	BroadCast( ERM_CHANGE_PVP_ROOM_PUBLIC_NOT, kNot );

	kPacket.m_bPublic = kPacket_.m_bPublic;
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_PVP_ROOM_PUBLIC_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ROOM_NAME_REQ, KEGS_CHANGE_PVP_ROOM_NAME_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_CHANGE_PVP_ROOM_NAME_ACK, KEGS_CHANGE_PVP_ROOM_NAME_ACK );

	if( m_spRoomUserManager->IsObserver( FIRST_SENDER_UID ) == false )
	{
		// �������� üũ�Ѵ�.
		VERIFY_HOST( ERM_CHANGE_SLOT_OPEN_ACK );
	}

	SetName( kPacket_.m_wstrRoomName );

	// ��� ���� ������ �� ����Ʈ�� �����϶�� ������.
	SendRoomListInfo( NetError::ERR_ROOM_15 );

	START_LOG( clog, L"::: Change Room Info ::: " )
		<< BUILD_LOG( kPacket_.m_wstrRoomName )
		;

	KEGS_CHANGE_PVP_ROOM_NAME_NOT kNot;
	kNot.m_wstrRoomName = kPacket_.m_wstrRoomName;
	BroadCast( ERM_CHANGE_PVP_ROOM_NAME_NOT, kNot );

	kPacket.m_wstrRoomName = kPacket_.m_wstrRoomName;
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_PVP_ROOM_NAME_ACK, kPacket );
}
#endif SERV_NEW_PVPROOM_PROCESS

#ifdef SERV_PVP_REMATCH
_IMPL_ON_FUNC( ERM_PVP_REMATCH_REQ, KEGS_PVP_REMATCH_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_RESULT ), ERM_PVP_REMATCH_ACK, KEGS_PVP_REMATCH_ACK );

	if( m_spRoomUserManager->SetRematch( FIRST_SENDER_UID, kPacket_.m_bAcceptRematch ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_17;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PVP_REMATCH_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PVP_REMATCH_ACK, kPacket );

	KEGS_PVP_REMATCH_NOT kNot;
	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	kNot.m_iUnitUID = spRoomUser->GetCID();		// SetRematch �Լ� ���ο��� �˻� �����Ƿ� �ΰ˻� �� �Ѵ�.
	kNot.m_bAcceptRematch = kPacket_.m_bAcceptRematch;
	BroadCast( ERM_PVP_REMATCH_NOT, kNot );
}

void KPvPRoom::Rematch()
{
	// �� ���¸� wait �� �ٲ۴�.
	StateTransition( KRoomFSM::I_TO_WAIT );
	m_kTimer[TM_WAIT].restart();

	m_spRoomUserManager->SetAllReady( true );
	m_spRoomUserManager->SetAllRematch( false );

	//////////////////////////////////////////////////////////////////////////
	// ���� �ʱ�ȭ.
	StartGame();

	// ���� ���� ��Ŷ!
	KEGS_GAME_START_PVP_MATCH_NOT kNot;
	kNot.m_RoomInfo.m_iPVPChannelClass = GetPVPChannelClass();

	if( SiKPvpMatchManager()->DecisionPvpMap( false, m_mapAllPlayersSelectedMap, m_sWorldID ) == false )
	{
		START_LOG( cerr, L"���� �� ���� ����!" )
			<< BUILD_LOG( m_mapAllPlayersSelectedMap.size() )
			<< END_LOG;
	}
	else
	{
		m_sShowPvpMapWorldID = m_sWorldID;
	}	

	GetRoomInfo( kNot.m_RoomInfo );
	GetRoomSlotInfo( kNot.m_vecSlot );
	GetRoomSlotInfo( kNot.m_vecObserverSlot, KRoomUserManager::UT_OBSERVER );
	BroadCast( ERM_GAME_START_PVP_MATCH_NOT, kNot );

	++m_iRematchCount;
}

bool KPvPRoom::CanRematch()
{
	if( IsOfficialMatch() == true && m_spRoomUserManager->IsAllPlayerWantRematch() == true && m_iRematchCount < 2 )
		return true;

	return false;
}
#endif SERV_PVP_REMATCH

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


#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
IMPL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_ZOMBIE_ALERT_NOT )
{
}
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST