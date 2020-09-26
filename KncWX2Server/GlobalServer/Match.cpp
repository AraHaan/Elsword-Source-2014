#include "Match.h"

#include "MatchFSM.h"
#include "NetError.h"
#include "X2Data/XSLRoom.h"
#include "Enum/Enum.h"
#include "X2Data/XSLUnitManager.h"
//{{ 2011. 07. 18	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch/PvpMatchManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}
#ifdef SERV_GOLBAL_SERVER_EVENT_MANAGER
#include "GameEvent/GameEventManager.h"
#endif //SERV_GOLBAL_SERVER_EVENT_MANAGER


//{{ 2011. 06. 14	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM


#define CLASS_TYPE KMatch
ImplPfID( KMatch, PI_GLOBAL_MATCH_MAKING );


KMatch::KMatch( IN const UidType iMatchUID, IN const SEnum::MATCH_TYPE eMatchType )
{
	SetUID( iMatchUID );
	m_eMatchType = eMatchType;
	m_kMatchUserManager.Init( static_cast<char>(GetMatchType()) );
}

KMatch::~KMatch()
{
}

void KMatch::Tick()
{
	// �̺�Ʈ�� ���� ó���ѵ�..
	KPerformer::Tick();

	// �� State�� �°� ��������!
	switch( GetStateID() )
	{
	case KMatchFSM::S_INIT:
		break;

	case KMatchFSM::S_CLOSE:
		break;

	case KMatchFSM::S_MATCH_MAKING:
		{
			// ���� �ð����� ��ġ�� ���̷������ �ش� ��ġ�� ��ü ��Ų��. - ��ġ�� �ȵǴµ� ��� ���� ��ٸ��� �ϴ°͵� 

			//if( m_tTimer[MTE_MATCH_MAKING].elapsed() > 5.0 )
			//{
			//	KEGS_MATCH_MAKING_SUCCESS_NOT kNot;
			//	kNot.m_iMatchUID = GetUID();
			//	BroadCast( EGB_MATCH_MAKING_SUCCESS_NOT, kNot );

			//	// üũ ���·� ��ȯ!
			//	StateTransition( KMatchFSM::I_TO_CHECK_ACCEPT );
			//}
		}
		break;

	case KMatchFSM::S_CHECK_ACCEPT:
		{
			if( m_tTimer[MTE_CHECK_ACCEPT].elapsed() > 10.0 )
			{
				// 10�ʰ� �Ѿ��µ��� ��� ������ ������ ���� �ʾҴٸ� ������ ��ġ �Ҹ� ����!
				if( m_kMatchUserManager.IsAllPlayersAccept() == false )
				{
					//{{ 2012. 11. 28 ����,���� �ڵ� ��Ī ��� ���� �ʴ� ���� ����(���� ��Ȳ) - ��μ�
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
					std::vector<UidType> vecCancelUserList;
					m_kMatchUserManager.GetCancelUserList( vecCancelUserList );

					CloseMatchByCancel( vecCancelUserList );
#else
					CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
#endif SERV_AUTO_MATCH_CANCEL_ERROR
					//}
				}
				else
				{
					// RoomUserInfo ��û�ϱ� ���� �̸� �������!
					m_kMatchUserManager.ClearAllPlayerRoomUserInfo();

					// ���¸� �÷��� ��� ���·� �ٲ���!
					StateTransition( KMatchFSM::I_TO_PREPARE_FOR_PLAY );

					// �÷��� �غ� Ÿ�̸� �ʱ�ȭ
					m_tTimer[MTE_PREPARE_FOR_PLAY].restart();

					// ��ġ�� �����ϱ� ���� �� �������� RoomUserInfo�� ���´�!
					BroadCast( EGB_GET_ROOM_USER_INFO_FOR_MATCH_REQ, char() );
				}
			}
		}
		break;

	case KMatchFSM::S_PREPARE_FOR_PLAY:
		{
			//if( m_tTimer[MTE_PREPARE_FOR_PLAY].elapsed() > 60.0 )
			//{
			//	m_tTimer[MTE_PREPARE_FOR_PLAY].restart();

			//	START_LOG( cerr, L"1�е��� �÷��� �غ���°� ���ӵǾ���. ������ ��ġ�� ����!" )
			//		<< BUILD_LOG( GetMatchUID() )
			//		<< END_LOG;

			//	// 1���̻� ���º�ȭ�� ������ ������ �ִ°Ŵ�!
			//	CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
			//}
		}
		break;

	case KMatchFSM::S_PLAY:
		{
			//if( m_tTimer[MTE_PLAY].elapsed() > 60.0 )
			//{
			//	m_tTimer[MTE_PLAY].restart();

			//	START_LOG( cerr, L"1�е��� �÷��� ���°� ���ӵǾ���. ������ ��ġ�� ����!" )
			//		<< BUILD_LOG( GetMatchUID() )
			//		<< END_LOG;

			//	// 1���̻� ���º�ȭ�� ������ ������ �ִ°Ŵ�!
			//	CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
			//}
		}
		break;

	default:
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( GetStateID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		break;
	}
}

void KMatch::OnDestroy()
{
	if( GetStateID() != KMatchFSM::S_CLOSE )
	{
		START_LOG( cerr, L"�̻��� ���¿��� ����ó���� �ǰ� �ֽ��ϴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetStateID() )
			<< END_LOG;
	}
}

void KMatch::CloseMatch( IN const int iReason, IN const UidType iCancelUnitUID /*= 0*/ )
{	
	// ��ġ ���¸� �ٲٰ�
	StateTransition( KMatchFSM::I_TO_CLOSE );

	// ��ġ �����鿡�� �˸���!
	KEGS_MATCH_CLOSE_NOT kPacketNot;
	kPacketNot.m_iReason = iReason;
	kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
	BroadCast( EGB_MATCH_CLOSE_NOT, kPacketNot );
}

void KMatch::CloseMatchByCancel( IN const UidType iCancelUnitUID )
{
	// ��ġ ���¸� �ٲٰ�
	StateTransition( KMatchFSM::I_TO_CLOSE );

	// �����鿡�� ��ġ �ݱ� �˸� �� �ڵ� ���û ó��
	{
		KMatchUserPtr spCancelUser = m_kMatchUserManager.GetUser( iCancelUnitUID );
		if( spCancelUser == NULL )
		{
			START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
				<< BUILD_LOG( GetMatchUID() )
				<< BUILD_LOG( iCancelUnitUID )
				<< END_LOG;
			return;
		}

		// ��ҿ�û�� ���� ��Ƽ���� ���û ���ϵ����ϱ�
		std::set< UidType > setNoSendPartyUID;
		setNoSendPartyUID.insert( spCancelUser->GetPartyUID() );

		// ����ϰ� ��Ī�� ��ҵ� �������� ���� ���û ��Ŷ�� ������!
		std::vector< UidType > vecUserList;
		m_kMatchUserManager.GetMatchUserList( vecUserList );

		// ��ġ ���û�� ����Ʈ 
		std::vector< UidType > vecRematchUserList;


		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUserList )
		{
			KMatchUserPtr spMatchUser = m_kMatchUserManager.GetUser( iUnitUID );
			if( spMatchUser == NULL )
			{
				START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
					<< BUILD_LOG( GetMatchUID() )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
				continue;
			}

			// ��� ��û�� ��������� �˻�
			if( iUnitUID == iCancelUnitUID )
			{
				KEGS_MATCH_CLOSE_NOT kPacketNot;
				kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_02;
				kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
				spMatchUser->SendToGSCharacter( EGB_MATCH_CLOSE_NOT, kPacketNot );
				continue;
			}

			// ��� ��û�� ����ڿ� ���� ��Ƽ���� �˻�
			if( spCancelUser->GetPartyUID() != 0 )
			{
				if( spCancelUser->GetPartyUID() == spMatchUser->GetPartyUID() )
				{
					KEGS_MATCH_CLOSE_NOT kPacketNot;
					kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_03;
					kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
					spMatchUser->SendToGSCharacter( EGB_MATCH_CLOSE_NOT, kPacketNot );
					continue;
				}
			}

			// ������ �����鿡�Դ� ��ġ �ݱ⸦ ��������!
			KEGS_MATCH_CLOSE_NOT kPacketNot;
			kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_01;
			kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
			spMatchUser->SendToGSCharacter( EGB_MATCH_CLOSE_NOT, kPacketNot );

			// ���û�� ������ ���ƾ��� ��Ƽ���� üũ�Ѵ�.
			if( spMatchUser->GetPartyUID() != 0 )
			{
				if( setNoSendPartyUID.find( spMatchUser->GetPartyUID() ) != setNoSendPartyUID.end() )
					continue;

				// �� ��Ƽ�� ���ؼ� �� ��û �޽����� �߼��Ͽ����Ƿ� ���� ��Ƽ���� �߼����� �ʵ��� ����!
				setNoSendPartyUID.insert( spMatchUser->GetPartyUID() );
			}

			// ��ġ ���û�� ������
			vecRematchUserList.push_back( spMatchUser->GetCharUID() );
		}


		//////////////////////////////////////////////////////////////////////////
		// ��ġ ���û!
		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecRematchUserList )
		{
			KMatchUserPtr spMatchUser = m_kMatchUserManager.GetUser( iUnitUID );
			if( spMatchUser == NULL )
			{
				START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
					<< BUILD_LOG( GetMatchUID() )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
				continue;
			}

			KEGB_REMATCH_MAKING_USER_NOT kPacketNot;
			kPacketNot.m_cPvpGameType = KMatch::GetMatchPvpGameType( GetMatchType() );
			kPacketNot.m_cPvpPlayerCount = KMatch::GetMatchPlayerCount( GetMatchType() );
			spMatchUser->SendToGSCharacter( EGB_REMATCH_MAKING_USER_NOT, kPacketNot );
		}
	}
}

//{{ 2012. 11. 28 ����,���� �ڵ� ��Ī ��� ���� �ʴ� ���� ����(���� ��Ȳ) - ��μ�
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
void KMatch::CloseMatchByCancel( IN const std::vector<UidType>& vecCancelUnitUID )
{
	// ��ġ ���¸� �ٲٰ�
	StateTransition( KMatchFSM::I_TO_CLOSE );

	// ��ҿ�û�� ���� ��Ƽ���� ���û ���ϵ����ϱ�
	std::set< UidType > setNoSendPartyUID;

	// �����鿡�� �ڵ� ��Ƽ �ݱ� �˸� �� �ڵ� ���û ó��

	// ����ϰ� �ڵ� ��Ƽ�� ��ҵ� �������� ���� ���û ��Ŷ�� ������!
	std::vector< UidType > vecUserList;
	m_kMatchUserManager.GetMatchUserList( vecUserList );

	// ��ġ ���û�� ����Ʈ 
	std::vector< UidType > vecRematchUserList;

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUserList )
	{
		KMatchUserPtr spMatchUser = m_kMatchUserManager.GetUser( iUnitUID );
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
				<< BUILD_LOG( GetMatchUID() )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		bool bContinue = false;

		// ��� ��û�� ��������� �˻�
		BOOST_TEST_FOREACH( UidType, iCancelUnitUID, vecCancelUnitUID )
		{
			KMatchUserPtr spCancelUser = m_kMatchUserManager.GetUser( iCancelUnitUID );
			if( spCancelUser == NULL )
			{
				START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
					<< BUILD_LOG( GetMatchUID() )
					<< BUILD_LOG( iCancelUnitUID )
					<< END_LOG;
				continue;
			}

			// ��ҿ�û�� ���� ��Ƽ���� ���û ���ϵ����ϱ�
			setNoSendPartyUID.insert( spCancelUser->GetPartyUID() );

			// ��� ��û�� ��������� �˻�
			if( iUnitUID == iCancelUnitUID )
			{
				KEGS_MATCH_CLOSE_NOT kPacketNot;
				kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_02;
				kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
				spMatchUser->SendToGSCharacter( EGB_MATCH_CLOSE_NOT, kPacketNot );

				bContinue = true;
				continue;
			}

			// ��� ��û�� ����ڿ� ���� ��Ƽ���� �˻�
			if( spCancelUser->GetPartyUID() != 0 )
			{
				if( spCancelUser->GetPartyUID() == spMatchUser->GetPartyUID() )
				{
					KEGS_MATCH_CLOSE_NOT kPacketNot;
					kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_03;
					kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
					spMatchUser->SendToGSCharacter( EGB_MATCH_CLOSE_NOT, kPacketNot );

					bContinue = true;
					continue;
				}
			}
		}

		// ��Ƽ �ݱ⸦ ���� �����̹Ƿ� �Ѿ��
		if( bContinue == true )
		{
			continue;
		}

		// ������ �����鿡�Դ� ��ġ �ݱ⸦ ��������!
		KEGS_MATCH_CLOSE_NOT kPacketNot;
		kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_01;
		kPacketNot.m_iCancelUnitUID = 0;
		spMatchUser->SendToGSCharacter( EGB_MATCH_CLOSE_NOT, kPacketNot );

		// ���û�� ������ ���ƾ��� ��Ƽ���� üũ�Ѵ�.
		if( spMatchUser->GetPartyUID() != 0 )
		{
			if( setNoSendPartyUID.find( spMatchUser->GetPartyUID() ) != setNoSendPartyUID.end() )
				continue;

			// �� ��Ƽ�� ���ؼ� �� ��û �޽����� �߼��Ͽ����Ƿ� ���� ��Ƽ���� �߼����� �ʵ��� ����!
			setNoSendPartyUID.insert( spMatchUser->GetPartyUID() );
		}

		// ��ġ ���û�� ������
		vecRematchUserList.push_back( spMatchUser->GetCharUID() );
	}

	//////////////////////////////////////////////////////////////////////////
	// ��ġ ���û!
	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecRematchUserList )
	{
		KMatchUserPtr spMatchUser = m_kMatchUserManager.GetUser( iUnitUID );
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
				<< BUILD_LOG( GetMatchUID() )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		KEGB_REMATCH_MAKING_USER_NOT kPacketNot;
		kPacketNot.m_cPvpGameType = KMatch::GetMatchPvpGameType( GetMatchType() );
		kPacketNot.m_cPvpPlayerCount = KMatch::GetMatchPlayerCount( GetMatchType() );
		spMatchUser->SendToGSCharacter( EGB_REMATCH_MAKING_USER_NOT, kPacketNot );
	}
}
#endif SERV_AUTO_MATCH_CANCEL_ERROR
//}


bool KMatch::Enter( IN const std::vector< KMatchUserInfo >& vecMatchUserList )
{
	return m_kMatchUserManager.Enter( vecMatchUserList );
}

bool KMatch::CheckOptimalMatch( IN KMatchUserWaitListPtr spWaitList, OUT int& iOptimalMatchPoint )
{
	//{{ 2012. 09. 17	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	iOptimalMatchPoint = ::abs( m_kMatchUserManager.GetRatingWeightAverage() - spWaitList->GetRatingWeightAverage() );
#else
	iOptimalMatchPoint = ::abs( m_kMatchUserManager.GetRatingAverage() - spWaitList->GetRatingAverage() );
#endif SERV_2012_PVP_SEASON2
	//}}
	return true;
}

bool KMatch::CheckMatchMakingSuccess()
{
	switch( GetStateID() )
	{
	case KMatchFSM::S_INIT:
	case KMatchFSM::S_MATCH_MAKING:
		break;

	default:
		{
			START_LOG( cerr, L"���� ���°� �̻��մϴ�!" )
				<< BUILD_LOG( GetMatchUID() )
				<< BUILD_LOG( GetMatchType() )
				<< BUILD_LOG( GetStateIDString() )
				<< END_LOG;
		}
		return false;
	}

	// �ο��� ���� Ȯ��!
	if( KMatch::GetMatchTotalPlayerCount( GetMatchType() ) != GetNumMember() )
	{
		START_LOG( clog, L"���� ��Ī�� �ʿ��� �ο����� �������� �ʾҽ��ϴ�." )
			<< BUILD_LOG( GetMatchUID() )
			<< BUILD_LOG( GetMatchType() )
			<< BUILD_LOG( KMatch::GetMatchTotalPlayerCount( GetMatchType() ) )
			<< BUILD_LOG( GetNumMember() )
			<< END_LOG;

		// ��� ��ġ ����ŷ ����..
		if( GetStateID() != KMatchFSM::S_MATCH_MAKING )
		{
			StateTransition( KMatchFSM::I_TO_MATCH_MAKING );
		}
		return false;
	}

	// üũ ���·� ��ȯ!
	StateTransition( KMatchFSM::I_TO_CHECK_ACCEPT );

	// üũ Ÿ�̸� �ʱ�ȭ
	m_tTimer[MTE_CHECK_ACCEPT].restart();

	KEGS_MATCH_MAKING_SUCCESS_NOT kNot;
	kNot.m_iMatchUID = GetUID();
	BroadCast( EGB_MATCH_MAKING_SUCCESS_NOT, kNot );
	return true;
}

CXSLRoom::PVP_GAME_TYPE KMatch::GetMatchPvpGameType( IN const SEnum::MATCH_TYPE eMatchType )
{
	switch( eMatchType )
	{
	case SEnum::MT_TEAM_1_ON_1:
	case SEnum::MT_TEAM_2_ON_2:
	case SEnum::MT_TEAM_3_ON_3:
		return CXSLRoom::PGT_TEAM;

	case SEnum::MT_TEAM_DEATH_1_ON_1:
	case SEnum::MT_TEAM_DEATH_2_ON_2:
	case SEnum::MT_TEAM_DEATH_3_ON_3:
		return CXSLRoom::PGT_TEAM_DEATH;
	}

	return CXSLRoom::PGT_NULL;
}

int KMatch::GetMatchPlayerCount( IN const SEnum::MATCH_TYPE eMatchType )
{
	switch( eMatchType )
	{
	case SEnum::MT_TEAM_1_ON_1:
	case SEnum::MT_TEAM_DEATH_1_ON_1:
		return 1;

	case SEnum::MT_TEAM_2_ON_2:
	case SEnum::MT_TEAM_DEATH_2_ON_2:
		return 2;

	case SEnum::MT_TEAM_3_ON_3:
	case SEnum::MT_TEAM_DEATH_3_ON_3:
		return 3;
	}

	return 0;
}

int KMatch::GetMatchTotalPlayerCount( IN const SEnum::MATCH_TYPE eMatchType )
{
    return ( GetMatchPlayerCount( eMatchType ) * 2 );
}

bool KMatch::GetRoomInfoForPvpGame( IN const bool bIsNpcPvpMode, OUT KRoomInfo& kInfo ) const
{
	kInfo.m_RoomType		= CXSLRoom::RT_PVP;
	kInfo.m_RoomName		= L"Match Making Battle!";
	kInfo.m_RoomState; // �̰� ����?
	kInfo.m_bPublic			= false;
	kInfo.m_bTeamBalance; // �̰� ����?
	kInfo.m_MaxSlot			= KMatch::GetMatchTotalPlayerCount( GetMatchType() );
	kInfo.m_JoinSlot		= m_kMatchUserManager.GetNumMember();
	kInfo.m_PVPGameType		= KMatch::GetMatchPvpGameType( GetMatchType() );
#ifdef DUNGEON_ITEM
	//m_kMatchUserManager.DecisionGameMode( kInfo.m_bIsItemMode ); // ���Ŀ� ������ �ý���
	kInfo.m_bIsItemMode		= true; // ���Ĵ����� ��������!
#endif DUNGEON_ITEM
	kInfo.m_iPVPChannelClass = KPVPChannelInfo::PCC_OFFICIAL;
	kInfo.m_bCanIntrude		= false;	

	// ���� ���� ���
	if( SiKPvpMatchManager()->GetPvpMatchInfo( static_cast<int>(GetMatchType()), kInfo.m_WinKillNum, kInfo.m_fPlayTime ) == false )
	{
		START_LOG( cerr, L"���� ���� ��� ����!" )
			<< BUILD_LOG( GetMatchType() )
			<< BUILD_LOG( GetMatchUID() )
			<< END_LOG;
		return false;
	}

	// �� ������� ������ ���� ���� ���� mapID ����Ʈ�� ��������!
	std::map< short, int > mapAllPlayersSelectedMap;
	m_kMatchUserManager.GetAllPlayersSelectedPvpMap( mapAllPlayersSelectedMap );

	// ���� ���� ��������!
	if( SiKPvpMatchManager()->DecisionPvpMap( bIsNpcPvpMode, mapAllPlayersSelectedMap, kInfo.m_WorldID ) == false )
	{
		START_LOG( cerr, L"���� �� ���� ����!" )
			<< BUILD_LOG( GetMatchType() )
			<< BUILD_LOG( GetMatchUID() )
			<< BUILD_LOG( bIsNpcPvpMode )
			<< BUILD_LOG( mapAllPlayersSelectedMap.size() )
			<< END_LOG;
		return false;
	}
	else
	{
		kInfo.m_ShowPvpMapWorldID = kInfo.m_WorldID;
	}	

#ifdef SERV_PVP_REMATCH
	// ���߿��� �̿��ϱ� ����
	kInfo.m_mapAllPlayersSelectedMap = mapAllPlayersSelectedMap;
#endif SERV_PVP_REMATCH

	return true;
}

bool KMatch::GetMatchTypeByMatchInfo( IN const char cPvpGameType, IN const char cPvpPlayerCount, OUT SEnum::MATCH_TYPE& eMatchType )
{
    switch( cPvpGameType )
	{
	case CXSLRoom::PGT_TEAM:
		{
			switch( cPvpPlayerCount )
			{
			case 1: eMatchType = SEnum::MT_TEAM_1_ON_1; return true;
			case 2: eMatchType = SEnum::MT_TEAM_2_ON_2; return true;
			case 3: eMatchType = SEnum::MT_TEAM_3_ON_3; return true;
			}
		}
		break;

	case CXSLRoom::PGT_TEAM_DEATH:
		{
			switch( cPvpPlayerCount )
			{
			case 1: eMatchType = SEnum::MT_TEAM_DEATH_1_ON_1; return true;
			case 2: eMatchType = SEnum::MT_TEAM_DEATH_2_ON_2; return true;
			case 3: eMatchType = SEnum::MT_TEAM_DEATH_3_ON_3; return true;
			}
		}
		break;
	}

	START_LOG( cerr, L"�߸��� ���� ���� Ÿ�� �����Դϴ�!" )
		<< BUILD_LOGc( cPvpGameType )
		<< BUILD_LOGc( cPvpPlayerCount )
		<< END_LOG;
	return false;	
}

void KMatch::ProcessEvent( const KEventPtr& spEvent_ )
{
	switch( spEvent_->m_usEventID )
	{
		CASE( EGB_CANCEL_MATCH_MAKING_REQ );
	   _CASE( EGB_MATCH_MAKING_SUCCESS_REPLY_NOT, KEGS_MATCH_MAKING_SUCCESS_REPLY_NOT );
		CASE( EGB_GET_ROOM_USER_INFO_FOR_MATCH_ACK );
		CASE( EGB_OPEN_PVP_ROOM_FOR_MATCH_ACK );

	default:
		START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
	}
}

IMPL_ON_FUNC( EGB_CANCEL_MATCH_MAKING_REQ )
{
	switch( GetStateID() )
	{
	case KMatchFSM::S_MATCH_MAKING:
	case KMatchFSM::S_CHECK_ACCEPT:
	case KMatchFSM::S_PREPARE_FOR_PLAY:
	case KMatchFSM::S_PLAY:
		break;

	default:
		{
            START_LOG( cerr, L"�̻��� ���¿��� ��Ŷ�� �����Ͽ����ϴ�." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( GetStateIDString() )
				<< END_LOG;
		}
		break;
	}
    
	// �ش� ������ �����ϴ��� �˻�!
	if( m_kMatchUserManager.IsExist( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"��ġ�� ���Ե� ������ �ƴѵ� ��ġ ��ҿ�û�� �Գ�? �հ� ������ �ֱ�.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iMatchUID )
			<< BUILD_LOG( kPacket_.m_bSendBySystem )
			<< END_LOG;
		return;
	}

	// �ý����� �������� �ƴ϶�� ACK�� ��������!
	if( kPacket_.m_bSendBySystem == false )
	{
        KEGS_CANCEL_MATCH_MAKING_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_CANCEL_MATCH_MAKING_ACK, kPacket );
	}

	// ��ġ�� ����!
	CloseMatchByCancel( FIRST_SENDER_UID );
}

_IMPL_ON_FUNC( EGB_MATCH_MAKING_SUCCESS_REPLY_NOT, KEGS_MATCH_MAKING_SUCCESS_REPLY_NOT )
{
	VERIFY_MATCH_STATE( ( 1, KMatchFSM::S_CHECK_ACCEPT ) );

	// ������ ������� ���� üũ�Ѵ�!
	if( kPacket_.m_bGameStartAgree == true )
	{
		if( m_kMatchUserManager.Accept( FIRST_SENDER_UID ) == false )
		{
			START_LOG( cerr, L"������ �����Ͽ����ϴ�!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
			return;
		}

		if( m_kMatchUserManager.UpdateSelectedPvpMap( FIRST_SENDER_UID, kPacket_.m_setSelectedPvpMap ) == false )
		{
			START_LOG( cerr, L"��ġ ���� ������ ���� �� ���� ������Ʈ ����!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_setSelectedPvpMap.size() )
				<< END_LOG;
			return;
		}

		if( m_kMatchUserManager.UpdatePvpGameMode( FIRST_SENDER_UID, kPacket_.m_bPvpPowerMode, kPacket_.m_bPvpItemMode ) == false )
		{
			START_LOG( cerr, L"��ġ ���� ������ ���� ��� ���� ������Ʈ ����!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_bPvpPowerMode )
				<< BUILD_LOG( kPacket_.m_bPvpItemMode )
				<< END_LOG;
			return;
		}
	}
	else
	{
		// ��ġ ���
		CloseMatchByCancel( FIRST_SENDER_UID );
		return;
	}

	// ��� ������ ������ �Ϸ�Ǿ��ٸ�?
	if( m_kMatchUserManager.IsAllPlayersAccept() == true )
	{
		// RoomUserInfo ��û�ϱ� ���� �̸� �������!
		m_kMatchUserManager.ClearAllPlayerRoomUserInfo();

		// ���¸� �÷��� ��� ���·� �ٲ���!
		StateTransition( KMatchFSM::I_TO_PREPARE_FOR_PLAY );

		// �÷��� �غ� Ÿ�̸� �ʱ�ȭ
		m_tTimer[MTE_PREPARE_FOR_PLAY].restart();

        // ��ġ�� �����ϱ� ���� �� �������� RoomUserInfo�� ���´�!
		BroadCast( EGB_GET_ROOM_USER_INFO_FOR_MATCH_REQ, char() );
	}
}

IMPL_ON_FUNC( EGB_GET_ROOM_USER_INFO_FOR_MATCH_ACK )
{
	VERIFY_MATCH_STATE( ( 1, KMatchFSM::S_PREPARE_FOR_PLAY ) );

	// roomuserinfo�� ������Ʈ ����!
	if( m_kMatchUserManager.UpdateRoomUserInfo( kPacket_.m_kRoomUserInfo ) == false )
	{
		START_LOG( cerr, L"�ش� ��ġ ������ RoomUserInfo ������Ʈ�� �����Ͽ���." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_kRoomUserInfo.m_nUnitUID )
			<< END_LOG;

		// �� ������ �ͼ� �̰� �����ϸ� �׳� ��ġ�� ������ �ۿ� ����!
		CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
		return;
	}

	// ��� ��ġ �������� RoomUserInfo�� �����Ǿ����� Ȯ��!
	if( m_kMatchUserManager.IsAllPlayersUpdatedRoomUserInfo() == true )
	{
		//////////////////////////////////////////////////////////////////////////
		// 1. ��� ������ RoomUserInfo�� �����Ǿ����� ���� ���� ������!
		std::vector< KRoomUserInfo > vecRedTeam;
		std::vector< KRoomUserInfo > vecBlueTeam;
		if( m_kMatchUserManager.TeamMemberSelection( vecRedTeam, vecBlueTeam ) == false )
		{
            START_LOG( cerr, L"�� ������ �����Ͽ���. ����� �Ͼ���� �ȵǴ� ����! ��ġ�� ���� ��Ű��!" )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			// �� ������ �ͼ� �̰� �����ϸ� �׳� ��ġ�� ������ �ۿ� ����!
			CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
			return;
		}

		//////////////////////////////////////////////////////////////////////////		
		// 2. NPC������ ���� ���� ����!
		SEnum::PVP_NPC_TYPE eRedTeamPvpNpcMode = SEnum::PNT_NONE;
		SEnum::PVP_NPC_TYPE eBlueTeamPvpNpcMode = SEnum::PNT_NONE;

		//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		const bool bIsAllPlayersBeginner = m_kMatchUserManager.IsAllPlayersBeginner();
		const int iRedTeamRatingWeightAverage = KMatchUserManager::GetRatingWeightAverage( vecRedTeam );
		const int iBlueTeamRatingWeightAverage = KMatchUserManager::GetRatingWeightAverage( vecBlueTeam );

		if( bIsAllPlayersBeginner == true )
		{
			eRedTeamPvpNpcMode = SEnum::PNT_BEGINNER_NPC;
			eBlueTeamPvpNpcMode = SEnum::PNT_BEGINNER_NPC;
		}
		else if( m_kMatchUserManager.ExistArrangeRankPlayer() == false )
		{
			if( SiKPvpMatchManager()->DecisionNpcPvpMode( vecRedTeam ) == true )
			{
				eRedTeamPvpNpcMode = SEnum::PNT_HERO_NPC;
			}

			if( SiKPvpMatchManager()->DecisionNpcPvpMode( vecBlueTeam ) == true )
			{
				eBlueTeamPvpNpcMode = SEnum::PNT_HERO_NPC;
			}
		}
#else
		// ��� ������ �ʺ� �������� üũ�Ѵ�!
		const bool bIsAllPlayersBeginner = m_kMatchUserManager.IsAllPlayersSameRank( CXSLUnit::PVPRANK_RANK_F );

		if( bIsAllPlayersBeginner )
		{
			eRedTeamPvpNpcMode = SEnum::PNT_BEGINNER_NPC;
			eBlueTeamPvpNpcMode = SEnum::PNT_BEGINNER_NPC;
		}
		else
		{
			if( SiKPvpMatchManager()->DecisionNpcPvpMode( vecRedTeam ) == true )
			{
				eRedTeamPvpNpcMode = SEnum::PNT_HERO_NPC;
			}

			if( SiKPvpMatchManager()->DecisionNpcPvpMode( vecBlueTeam ) == true )
			{
				eBlueTeamPvpNpcMode = SEnum::PNT_HERO_NPC;
			}
		}
#endif SERV_2012_PVP_SEASON2
		//}}

		//////////////////////////////////////////////////////////////////////////		
		// 3. �Ϲ� �����̴�!
		if( eRedTeamPvpNpcMode == SEnum::PNT_NONE  &&  eBlueTeamPvpNpcMode == SEnum::PNT_NONE )
		{
			KEGB_OPEN_PVP_ROOM_FOR_MATCH_REQ kPacketReq;

			// ���� �� ���� ���
			if( GetRoomInfoForPvpGame( false, kPacketReq.m_kRoomInfo ) == false )
			{
				START_LOG( cerr, L"������ ������ ��µ� �����Ͽ���. ����� �Ͼ���� �ȵǴ� ����! ��ġ�� ���� ��Ű��!" )
					<< BUILD_LOG( GetUID() )
					<< END_LOG;

				CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
				return;
			}

			kPacketReq.m_cMatchType = static_cast<char>(GetMatchType());
			kPacketReq.m_cPvpNpcType = static_cast<char>(SEnum::PNT_NONE);
			kPacketReq.m_iMatchUID = GetUID();
			kPacketReq.m_vecRedTeam = vecRedTeam;
			kPacketReq.m_vecBlueTeam = vecBlueTeam;
			m_kMatchUserManager.GetMatchWaitTimeList( kPacketReq.m_mapMatchWaitTime );
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_OPEN_PVP_ROOM_FOR_MATCH_REQ, kPacketReq );
		}

		if( eRedTeamPvpNpcMode != SEnum::PNT_NONE )
		{
			BOOST_TEST_FOREACH( const KRoomUserInfo&, kRedTeamUser, vecRedTeam )
			{
				KEGB_OPEN_PVP_ROOM_FOR_MATCH_REQ kPacketReq;

				// ������ ���� ���
				if( GetRoomInfoForPvpGame( true, kPacketReq.m_kRoomInfo ) == false )
				{
					START_LOG( cerr, L"������ ������ ��µ� �����Ͽ���. ����� �Ͼ���� �ȵǴ� ����! ��ġ�� ���� ��Ű��!" )
						<< BUILD_LOG( GetUID() )
						<< END_LOG;

					CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
					return;
				}

				// ���� NPC���� �ҷ�����!
				//{{ 2012. 09. 17	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
				if( SiKPvpMatchManager()->GetPvpNpcInfo( bIsAllPlayersBeginner, iRedTeamRatingWeightAverage, vecRedTeam.size(), kPacketReq.m_vecBlueTeam, kPacketReq.m_mapPvpNpcInfo ) == false )
#else
				if( SiKPvpMatchManager()->GetPvpNpcInfo( bIsAllPlayersBeginner, vecRedTeam.size(), kPacketReq.m_vecBlueTeam, kPacketReq.m_mapPvpNpcInfo ) == false )
#endif SERV_2012_PVP_SEASON2
					//}}
				{
					START_LOG( cerr, L"���� NPC ������ ��µ� �����Ͽ���. ����� �Ͼ���� �ȵǴ� ����! ��ġ�� ���� ��Ű��!" )
						<< BUILD_LOG( GetUID() )
						<< BUILD_LOG( bIsAllPlayersBeginner )
						<< BUILD_LOG( vecRedTeam.size() )
						<< END_LOG;

					CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
					return;
				}

				kPacketReq.m_cMatchType = static_cast<char>(GetMatchType());
				kPacketReq.m_cPvpNpcType = static_cast<char>(eRedTeamPvpNpcMode);
				kPacketReq.m_iMatchUID = GetUID();
				kPacketReq.m_vecRedTeam = vecRedTeam; // ������ ������ �״�� ������!
				m_kMatchUserManager.GetMatchWaitTimeList( kPacketReq.m_mapMatchWaitTime );
				SendToGSCharacter( kRedTeamUser.m_iGSUID, kRedTeamUser.m_nUnitUID, EGB_OPEN_PVP_ROOM_FOR_MATCH_REQ, kPacketReq );
				break;
			}

			if( eBlueTeamPvpNpcMode == SEnum::PNT_NONE )
			{
				BOOST_TEST_FOREACH( const KRoomUserInfo&, kRoomUserInfo, vecBlueTeam )
				{
					// ��ġ �����鿡�� �˸���!
					KEGS_MATCH_CLOSE_NOT kPacketNot;
					kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_01;
					SendToGSCharacter( kRoomUserInfo.m_iGSUID, kRoomUserInfo.m_nUnitUID, EGB_MATCH_CLOSE_NOT, kPacketNot );

					// ��ġ���� ��Ż ��Ű��!
					LIF( m_kMatchUserManager.DeleteUser( kRoomUserInfo.m_nUnitUID ) );

#ifdef SERV_FIX_PVP_NPC_AUTO_REMATCH// �۾���¥: 2013-05-24	// �ڼ���
					// ��ġ ���û!
					KEGB_REMATCH_MAKING_USER_NOT kPacketNotForRematch;
					kPacketNotForRematch.m_cPvpGameType = KMatch::GetMatchPvpGameType( GetMatchType() );
					kPacketNotForRematch.m_cPvpPlayerCount = KMatch::GetMatchPlayerCount( GetMatchType() );
					SendToGSCharacter( kRoomUserInfo.m_iGSUID, kRoomUserInfo.m_nUnitUID, EGB_REMATCH_MAKING_USER_NOT, kPacketNotForRematch );
#endif // SERV_FIX_PVP_NPC_AUTO_REMATCH
				}
			}
		}

		if( eBlueTeamPvpNpcMode != SEnum::PNT_NONE )
		{
			BOOST_TEST_FOREACH( const KRoomUserInfo&, kBlueTeamUser, vecBlueTeam )
			{
				KEGB_OPEN_PVP_ROOM_FOR_MATCH_REQ kPacketReq;

				// ���� �� ���� ���
				if( GetRoomInfoForPvpGame( true, kPacketReq.m_kRoomInfo ) == false )
				{
					START_LOG( cerr, L"������ ������ ��µ� �����Ͽ���. ����� �Ͼ���� �ȵǴ� ����! ��ġ�� ���� ��Ű��!" )
						<< BUILD_LOG( GetUID() )
						<< END_LOG;

					CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
					return;
				}

				// ���� NPC���� �ҷ�����!
				//{{ 2012. 09. 17	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
				if( SiKPvpMatchManager()->GetPvpNpcInfo( bIsAllPlayersBeginner, iBlueTeamRatingWeightAverage, vecBlueTeam.size(), kPacketReq.m_vecBlueTeam, kPacketReq.m_mapPvpNpcInfo ) == false )
#else
				if( SiKPvpMatchManager()->GetPvpNpcInfo( bIsAllPlayersBeginner, vecBlueTeam.size(), kPacketReq.m_vecBlueTeam, kPacketReq.m_mapPvpNpcInfo ) == false )
#endif SERV_2012_PVP_SEASON2
					//}}
				{
					START_LOG( cerr, L"���� NPC ������ ��µ� �����Ͽ���. ����� �Ͼ���� �ȵǴ� ����! ��ġ�� ���� ��Ű��!" )
						<< BUILD_LOG( GetUID() )
						<< BUILD_LOG( bIsAllPlayersBeginner )
						<< BUILD_LOG( vecRedTeam.size() )
						<< END_LOG;

					CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
					return;
				}

				kPacketReq.m_cMatchType = static_cast<char>(GetMatchType());
				kPacketReq.m_cPvpNpcType = static_cast<char>(eBlueTeamPvpNpcMode);
				kPacketReq.m_iMatchUID = GetUID();
				kPacketReq.m_vecRedTeam = vecBlueTeam; // ����� �������� ���������� ��������!
				m_kMatchUserManager.GetMatchWaitTimeList( kPacketReq.m_mapMatchWaitTime );
				SendToGSCharacter( kBlueTeamUser.m_iGSUID, kBlueTeamUser.m_nUnitUID, EGB_OPEN_PVP_ROOM_FOR_MATCH_REQ, kPacketReq );
				break;
			}

			if( eRedTeamPvpNpcMode == SEnum::PNT_NONE )
			{
				BOOST_TEST_FOREACH( const KRoomUserInfo&, kRoomUserInfo, vecRedTeam )
				{
					// ��ġ �����鿡�� �˸���!
					KEGS_MATCH_CLOSE_NOT kPacketNot;
					kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_01;
					SendToGSCharacter( kRoomUserInfo.m_iGSUID, kRoomUserInfo.m_nUnitUID, EGB_MATCH_CLOSE_NOT, kPacketNot );

					// ��ġ���� ��Ż ��Ű��!
					LIF( m_kMatchUserManager.DeleteUser( kRoomUserInfo.m_nUnitUID ) );

#ifdef SERV_FIX_PVP_NPC_AUTO_REMATCH// �۾���¥: 2013-05-24	// �ڼ���
					// ��ġ ���û!
					KEGB_REMATCH_MAKING_USER_NOT kPacketNotForRematch;
					kPacketNotForRematch.m_cPvpGameType = KMatch::GetMatchPvpGameType( GetMatchType() );
					kPacketNotForRematch.m_cPvpPlayerCount = KMatch::GetMatchPlayerCount( GetMatchType() );
					SendToGSCharacter( kRoomUserInfo.m_iGSUID, kRoomUserInfo.m_nUnitUID, EGB_REMATCH_MAKING_USER_NOT, kPacketNotForRematch );
#endif // SERV_FIX_PVP_NPC_AUTO_REMATCH
				}
			}
		}		

		//////////////////////////////////////////////////////////////////////////		

		// ���� ������ ���������� ���¸� �ٲ���!
		StateTransition( KMatchFSM::I_TO_PLAY );

		// �÷��� Ÿ�̸� �ʱ�ȭ
		m_tTimer[MTE_PLAY].restart();
	}
}

IMPL_ON_FUNC( EGB_OPEN_PVP_ROOM_FOR_MATCH_ACK )
{
	// �̹� �ݱ�� �ִ� ��Ȳ�� �� ��Ŷ�� �ü��� �ִ�. �̷��� �׳� ����!
	if( GetStateID() == KMatchFSM::S_CLOSE )
		return;

	VERIFY_MATCH_STATE( ( 1, KMatchFSM::S_PLAY ) );

    // ��ġ ������ ���� ����!
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
        START_LOG( cerr, L"��ġ ������ ���� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		// ���� ���� ���� ����! ��ġ ��������!
		CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
	}
	else
	{
		// ��Ī�� ��������!
		CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_00 );
	}
}

#endif SERV_PVP_NEW_SYSTEM
//}}

