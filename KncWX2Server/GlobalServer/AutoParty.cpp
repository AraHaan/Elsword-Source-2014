#include "AutoParty.h"

#include "AutoPartyFSM.h"
#include "NetError.h"
#include "X2Data/XSLRoom.h"
#include "Enum/Enum.h"
#include "X2Data/XSLUnitManager.h"
#include "AutoPartyMakingManager.h"

//{{ 2013. 09. 30	�ڼ���		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	#include "X2Data/XSLDungeon.h"
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}



//{{ 2012. 02. 08	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


#define CLASS_TYPE KAutoParty
ImplPfID( KAutoParty, PI_GLOBAL_AUTO_PARTY );


KAutoParty::KAutoParty( IN const UidType iAutoPartyUID, IN const KAutoPartyConditionInfo& kInfo )
{
	SetUID( iAutoPartyUID );
	m_kAutoPartyInfo = kInfo;
	m_kAutoPartyUserManager.Init( kInfo );
}

KAutoParty::~KAutoParty()
{
}

void KAutoParty::Tick()
{
	// �̺�Ʈ�� ���� ó���ѵ�..
	KPerformer::Tick();

	// �� State�� �°� ��������!
	switch( GetStateID() )
	{
	case KAutoPartyFSM::S_INIT:
		break;

	case KAutoPartyFSM::S_CLOSE:
		break;

	case KAutoPartyFSM::S_AUTO_PARTY_MAKING:
		{
			// ���� �ð����� ��ġ�� ���̷������ �ش� ��ġ�� ��ü ��Ų��. - ��ġ�� �ȵǴµ� ��� ���� ��ٸ��� �ϴ°͵� 

			//if( m_tTimer[MTE_MATCH_MAKING].elapsed() > 5.0 )
			//{
			//	KEGS_MATCH_MAKING_SUCCESS_NOT kNot;
			//	kNot.m_iAutoPartyUID = GetUID();
			//	BroadCast( EGB_MATCH_MAKING_SUCCESS_NOT, kNot );

			//	// üũ ���·� ��ȯ!
			//	StateTransition( KAutoPartyFSM::I_TO_CHECK_ACCEPT );
			//}
		}
		break;

	case KAutoPartyFSM::S_CHECK_ACCEPT:
		{
			if( m_tTimer[APTE_CHECK_ACCEPT].elapsed() > 10.0 )
			{
				//{{ 2012. 11. 28 ����,���� �ڵ� ��Ī ��� ���� �ʴ� ���� ����(���� ��Ȳ) - ��μ�
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
				// Ȯ���� ���� �������Դ� NOT_LEAVE_AUTO_PARTY_REASON_01 - ���� �ϰԲ�
				// ������ ���� �������Դ� NOT_LEAVE_AUTO_PARTY_REASON_02 - ���� �ȵ�

				std::vector< UidType > vecCancelUserList;
				m_kAutoPartyUserManager.GetAutoPartyCancelUserList( vecCancelUserList );

				CloseAutoPartyByCancel( vecCancelUserList );
#else
				// 10�ʰ� �Ѿ��µ��� ��� ������ ������ ���� �ʾҴٸ� ������ �ڵ� ��Ÿ�� �Ҹ� ����!
				//if( m_kAutoPartyUserManager.IsAllPlayersAccept() == false )
				//{
				CloseAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_REASON_02 );
				//}
				//else
				//{
				//	// RoomUserInfo ��û�ϱ� ���� �̸� �������!
				//	m_kAutoPartyUserManager.ClearAllPlayerRoomUserInfo();

				//	// ���¸� �÷��� ��� ���·� �ٲ���!
				//	StateTransition( KAutoPartyFSM::I_TO_PREPARE_FOR_PLAY );

				//	// �÷��� �غ� Ÿ�̸� �ʱ�ȭ
				//	m_tTimer[APTE_PREPARE_FOR_PLAY].restart();

				//	// ��ġ�� �����ϱ� ���� �� �������� RoomUserInfo�� ���´�!
				//	BroadCast( EGB_GET_ROOM_USER_INFO_FOR_MATCH_REQ, char() );
				//}
#endif SERV_AUTO_MATCH_CANCEL_ERROR
				//}
				
			}
		}
		break;

	case KAutoPartyFSM::S_REGROUP_PARTY:
		{
			if( m_tTimer[APTE_REGROUP_PARTY].elapsed() > 60.0 )
			{
				m_tTimer[APTE_REGROUP_PARTY].restart();

				START_LOG( cerr, L"1�е��� �÷��� �غ���°� ���ӵǾ���. ������ �ڵ� ��Ƽ�� ����!" )
					<< BUILD_LOG( GetAutoPartyUID() )
					<< END_LOG;

				// 1���̻� ���º�ȭ�� ������ ������ �ִ°Ŵ�!
				CloseAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_REASON_02 );
			}
		}
		break;

	case KAutoPartyFSM::S_PLAY:
		{
			//if( m_tTimer[MTE_PLAY].elapsed() > 60.0 )
			//{
			//	m_tTimer[MTE_PLAY].restart();

			//	START_LOG( cerr, L"1�е��� �÷��� ���°� ���ӵǾ���. ������ ��ġ�� ����!" )
			//		<< BUILD_LOG( GetAutoPartyUID() )
			//		<< END_LOG;

			//	// 1���̻� ���º�ȭ�� ������ ������ �ִ°Ŵ�!
			//	CloseAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_REASON_02 );
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

void KAutoParty::OnDestroy()
{
	if( GetStateID() != KAutoPartyFSM::S_CLOSE )
	{
		START_LOG( cerr, L"�̻��� ���¿��� ����ó���� �ǰ� �ֽ��ϴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetStateID() )
			<< END_LOG;
	}
}

void KAutoParty::CloseAutoParty( IN const int iReason, IN const UidType iCancelUnitUID /*= 0*/ )
{
	// ��ġ ���¸� �ٲٰ�
	StateTransition( KAutoPartyFSM::I_TO_CLOSE );

	// ��ġ �����鿡�� �˸���!
	KEGS_AUTO_PARTY_CLOSE_NOT kPacketNot;
	kPacketNot.m_iReason = iReason;
	kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
	BroadCast( EGB_AUTO_PARTY_CLOSE_NOT, kPacketNot );
}

void KAutoParty::CloseAutoPartyByCancel( IN const UidType iCancelUnitUID )
{
	// �ڵ� ��Ƽ ���¸� �ٲٰ�
	StateTransition( KAutoPartyFSM::I_TO_CLOSE );

	// �����鿡�� �ڵ� ��Ƽ �ݱ� �˸� �� �ڵ� ���û ó��
	{
		KAutoPartyUserPtr spCancelUser = m_kAutoPartyUserManager.GetUser( iCancelUnitUID );
		if( spCancelUser == NULL )
		{
			START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
				<< BUILD_LOG( GetAutoPartyUID() )
				<< BUILD_LOG( iCancelUnitUID )
				<< END_LOG;
			return;
		}

		// ��ҿ�û�� ���� ��Ƽ���� ���û ���ϵ����ϱ�
		std::set< UidType > setNoSendPartyUID;
		setNoSendPartyUID.insert( spCancelUser->GetOldPartyUID() );

		// ����ϰ� �ڵ� ��Ƽ�� ��ҵ� �������� ���� ���û ��Ŷ�� ������!
		std::vector< UidType > vecUserList;
		m_kAutoPartyUserManager.GetAutoPartyUserList( vecUserList );

		// �ڵ� ��Ƽ ���û�� ����Ʈ 
		std::vector< UidType > vecReAutoPartyUserList;


		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUserList )
		{
			KAutoPartyUserPtr spAutoPartyUser = m_kAutoPartyUserManager.GetUser( iUnitUID );
			if( spAutoPartyUser == NULL )
			{
				START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
					<< BUILD_LOG( GetAutoPartyUID() )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
				continue;
			}

			// ��� ��û�� ��������� �˻�
			if( iUnitUID == iCancelUnitUID )
			{
				KEGS_AUTO_PARTY_CLOSE_NOT kPacketNot;
				kPacketNot.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_REASON_02;
				kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
				spAutoPartyUser->SendToGSCharacter( EGB_AUTO_PARTY_CLOSE_NOT, kPacketNot );
				continue;
			}

			// ��� ��û�� ����ڿ� ���� ��Ƽ���� �˻�
			if( spCancelUser->GetOldPartyUID() != 0 )
			{
				if( spCancelUser->GetOldPartyUID() == spAutoPartyUser->GetOldPartyUID() )
				{
					KEGS_AUTO_PARTY_CLOSE_NOT kPacketNot;
					kPacketNot.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_REASON_03;
					kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
					spAutoPartyUser->SendToGSCharacter( EGB_AUTO_PARTY_CLOSE_NOT, kPacketNot );
					continue;
				}
			}

			// ������ �����鿡�Դ� ��ġ �ݱ⸦ ��������!
			KEGS_AUTO_PARTY_CLOSE_NOT kPacketNot;
			kPacketNot.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_REASON_01;
			kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
			spAutoPartyUser->SendToGSCharacter( EGB_AUTO_PARTY_CLOSE_NOT, kPacketNot );

			// ���û�� ������ ���ƾ��� ��Ƽ���� üũ�Ѵ�.
			if( spAutoPartyUser->GetOldPartyUID() != 0 )
			{
				if( setNoSendPartyUID.find( spAutoPartyUser->GetOldPartyUID() ) != setNoSendPartyUID.end() )
					continue;

//				// �� ��Ƽ�� ���ؼ� �� ��û �޽����� �߼��Ͽ����Ƿ� ���� ��Ƽ���� �߼����� �ʵ��� ����!
//				setNoSendPartyUID.insert( spAutoPartyUser->GetOldPartyUID() );
			}

			if( spAutoPartyUser->IsHost( spAutoPartyUser->GetCharUID() ) == true )
			{
				// ��ġ ���û�� ������
				vecReAutoPartyUserList.push_back( spAutoPartyUser->GetCharUID() );

				// �� ��Ƽ�� ���ؼ� �� ��û �޽����� �߼��Ͽ����Ƿ� ���� ��Ƽ���� �߼����� �ʵ��� ����!
				setNoSendPartyUID.insert( spAutoPartyUser->GetOldPartyUID() );
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// �ڵ� ��Ƽ ���û!
		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecReAutoPartyUserList )
		{
			KAutoPartyUserPtr spAutoPartyUser = m_kAutoPartyUserManager.GetUser( iUnitUID );
			if( spAutoPartyUser == NULL )
			{
				START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
					<< BUILD_LOG( GetAutoPartyUID() )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
				continue;
			}

			KEGB_REMAKING_AUTO_PARTY_NOT kPacketNot;
			kPacketNot.m_kAutoPartyInfo = m_kAutoPartyInfo;
			spAutoPartyUser->SendToGSCharacter( EGB_REMAKING_AUTO_PARTY_NOT, kPacketNot );
		}
	}
}

//{{ 2012. 11. 28 ����,���� �ڵ� ��Ī ��� ���� �ʴ� ���� ����(���� ��Ȳ) - ��μ�
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
void KAutoParty::CloseAutoPartyByCancel( IN const std::vector<UidType>& vecCancelUnitUID )
{
	// �ڵ� ��Ƽ ���¸� �ٲٰ�
	StateTransition( KAutoPartyFSM::I_TO_CLOSE );

	// ��ҿ�û�� ���� ��Ƽ���� ���û ���ϵ����ϱ�
	std::set< UidType > setNoSendPartyUID;

	// �����鿡�� �ڵ� ��Ƽ �ݱ� �˸� �� �ڵ� ���û ó��

	// ����ϰ� �ڵ� ��Ƽ�� ��ҵ� �������� ���� ���û ��Ŷ�� ������!
	std::vector< UidType > vecUserList;
	m_kAutoPartyUserManager.GetAutoPartyUserList( vecUserList );

	// �ڵ� ��Ƽ ���û�� ����Ʈ 
	std::vector< UidType > vecReAutoPartyUserList;

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUserList )
	{
		KAutoPartyUserPtr spAutoPartyUser = m_kAutoPartyUserManager.GetUser( iUnitUID );
		if( spAutoPartyUser == NULL )
		{
			START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
				<< BUILD_LOG( GetAutoPartyUID() )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		bool bContinue = false;

		// ��� ��û�� ��������� �˻�
		BOOST_TEST_FOREACH( UidType, iCancelUnitUID, vecCancelUnitUID )
		{
			KAutoPartyUserPtr spCancelUser = m_kAutoPartyUserManager.GetUser( iCancelUnitUID );
			if( spCancelUser == NULL )
			{
				START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
					<< BUILD_LOG( GetAutoPartyUID() )
					<< BUILD_LOG( iCancelUnitUID )
					<< END_LOG;
				continue;
			}

			// ��ҿ�û�� ���� ��Ƽ���� ���û ���ϵ����ϱ�
			setNoSendPartyUID.insert( spCancelUser->GetOldPartyUID() );

			if( iUnitUID == iCancelUnitUID )
			{
				KEGS_AUTO_PARTY_CLOSE_NOT kPacketNot;
				kPacketNot.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_REASON_02;
				kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
				spAutoPartyUser->SendToGSCharacter( EGB_AUTO_PARTY_CLOSE_NOT, kPacketNot );

				bContinue = true;
				break;
			}

			// ��� ��û�� ����ڿ� ���� ��Ƽ���� �˻�
			if( spCancelUser->GetOldPartyUID() != 0 )
			{
				if( spCancelUser->GetOldPartyUID() == spAutoPartyUser->GetOldPartyUID() )
				{
					KEGS_AUTO_PARTY_CLOSE_NOT kPacketNot;
					kPacketNot.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_REASON_03;
					kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
					spAutoPartyUser->SendToGSCharacter( EGB_AUTO_PARTY_CLOSE_NOT, kPacketNot );

					bContinue = true;
					break;
				}
			}
		}
		
		// ��Ƽ �ݱ⸦ ���� �����̹Ƿ� �Ѿ��
		if( bContinue == true )
		{
			continue;
		}

		// ������ �����鿡�Դ� ��ġ �ݱ⸦ ��������!
		KEGS_AUTO_PARTY_CLOSE_NOT kPacketNot;
		kPacketNot.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_REASON_01;
		kPacketNot.m_iCancelUnitUID = 0;
		spAutoPartyUser->SendToGSCharacter( EGB_AUTO_PARTY_CLOSE_NOT, kPacketNot );

		// ���û�� ������ ���ƾ��� ��Ƽ���� üũ�Ѵ�.
		if( spAutoPartyUser->GetOldPartyUID() != 0 )
		{
			if( setNoSendPartyUID.find( spAutoPartyUser->GetOldPartyUID() ) != setNoSendPartyUID.end() )
				continue;

			// �� ��Ƽ�� ���ؼ� �� ��û �޽����� �߼��Ͽ����Ƿ� ���� ��Ƽ���� �߼����� �ʵ��� ����!
			setNoSendPartyUID.insert( spAutoPartyUser->GetOldPartyUID() );
		}

		// ��ġ ���û�� ������
		vecReAutoPartyUserList.push_back( spAutoPartyUser->GetCharUID() );
	}


	//////////////////////////////////////////////////////////////////////////
	// �ڵ� ��Ƽ ���û!
	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecReAutoPartyUserList )
	{
		KAutoPartyUserPtr spAutoPartyUser = m_kAutoPartyUserManager.GetUser( iUnitUID );
		if( spAutoPartyUser == NULL )
		{
			START_LOG( cerr, L"���� �����Ͱ��� �̻��մϴ�." )
				<< BUILD_LOG( GetAutoPartyUID() )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		KEGB_REMAKING_AUTO_PARTY_NOT kPacketNot;
		kPacketNot.m_kAutoPartyInfo = m_kAutoPartyInfo;
		spAutoPartyUser->SendToGSCharacter( EGB_REMAKING_AUTO_PARTY_NOT, kPacketNot );
	}
}
#endif SERV_AUTO_MATCH_CANCEL_ERROR
//}

bool KAutoParty::Enter( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserList )
{
	return m_kAutoPartyUserManager.Enter( vecAutoPartyUserList );
}

bool KAutoParty::CheckAutoPartyMakingSuccess()
{
	switch( GetStateID() )
	{
	case KAutoPartyFSM::S_INIT:
	case KAutoPartyFSM::S_AUTO_PARTY_MAKING:
		break;

	default:
		{
			START_LOG( cerr, L"���� ���°� �̻��մϴ�!" )
				<< BUILD_LOG( GetAutoPartyUID() )
				<< BUILD_LOG( GetStateIDString() )
				<< END_LOG;
		}
		return false;
	}

	// üũ ���·� ��ȯ!
	StateTransition( KAutoPartyFSM::I_TO_CHECK_ACCEPT );

	// üũ Ÿ�̸� �ʱ�ȭ
	m_tTimer[APTE_CHECK_ACCEPT].restart();

	KEGS_AUTO_PARTY_MAKING_SUCCESS_NOT kNot;
	kNot.m_iAutoPartyUID = GetUID();
	kNot.m_iMemberCount = GetNumMember();
	kNot.m_iDungeonID = m_kAutoPartyInfo.m_iDungeonID;
	kNot.m_cDifficultyLevel = m_kAutoPartyInfo.m_cDifficultyLevel;
	//{{ 2012. 12. 17	�ڼ���	���� ��Ƽ �� ���� �˾� â ���� ���� ����
#ifdef SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
	kNot.m_cDungeonMode		= m_kAutoPartyInfo.m_cDungeonMode;
#endif SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
	//}}
	BroadCast( EGB_AUTO_PARTY_MAKING_SUCCESS_NOT, kNot );
	return true;
}

bool KAutoParty::GetCreatePartyInfo( OUT KCreatePartyInfo& kInfo ) const
{
	kInfo.m_cPartyType		= KPartyInfo::PT_DUNGEON_PARTY;		
	kInfo.m_iDungeonID		= m_kAutoPartyInfo.m_iDungeonID;
	kInfo.m_DifficultyLevel = m_kAutoPartyInfo.m_cDifficultyLevel;
	kInfo.m_cGetItemType	= m_kAutoPartyInfo.m_cGetItemType;
	kInfo.m_cDungeonMode	= m_kAutoPartyInfo.m_cDungeonMode;
	kInfo.m_bCheckLowLevel	= false;

	//kInfo.m_RoomType		= CXSLRoom::RT_DUNGEON;
	//kInfo.m_RoomName		= L"AutoParty Making Dungeon Play!";
	//kInfo.m_RoomState; // �̰� ����?
	//kInfo.m_bPublic			= false;
	//kInfo.m_bTeamBalance; // �̰� ����?
	//kInfo.m_MaxSlot			= SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT;
	//kInfo.m_JoinSlot		= m_kAutoPartyUserManager.GetNumMember();
	//kInfo.m_cDungeonMode	= m_kAutoPartyInfo.m_cDungeonMode;
	//kInfo.m_iDungeonID		= m_kAutoPartyInfo.m_iDungeonID;
	//kInfo.m_DifficultyLevel = m_kAutoPartyInfo.m_cDifficultyLevel;
	//kInfo.m_cGetItemType	= 0;
	//kInfo.m_iPartyUID		= 0;
	//kInfo.m_bCanIntrude		= false;
	return true;
}

void KAutoParty::ProcessEvent( const KEventPtr& spEvent_ )
{
	switch( spEvent_->m_usEventID )
	{
		CASE( EGB_CANCEL_AUTO_PARTY_MAKING_REQ );
	   _CASE( EGB_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT, KEGS_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT );
		CASE( EGB_GET_REGROUP_PARTY_INFO_ACK );
	   _CASE( EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK );

	default:
		START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
	}
}

IMPL_ON_FUNC( EGB_CANCEL_AUTO_PARTY_MAKING_REQ )
{
	switch( GetStateID() )
	{
	case KAutoPartyFSM::S_AUTO_PARTY_MAKING:
	case KAutoPartyFSM::S_CHECK_ACCEPT:
	case KAutoPartyFSM::S_REGROUP_PARTY:
	case KAutoPartyFSM::S_PLAY:
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
	if( m_kAutoPartyUserManager.IsExist( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"��ġ�� ���Ե� ������ �ƴѵ� ��ġ ��ҿ�û�� �Գ�? �հ� ������ �ֱ�.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iAutoPartyUID )
			<< BUILD_LOG( kPacket_.m_bSendBySystem )
			<< END_LOG;
		return;
	}

	// �ý����� �������� �ƴ϶�� ACK�� ��������!
	if( kPacket_.m_bSendBySystem == false )
	{
        KEGS_CANCEL_AUTO_PARTY_MAKING_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_CANCEL_AUTO_PARTY_MAKING_ACK, kPacket );
	}

	// ��ġ�� ����!
	CloseAutoPartyByCancel( FIRST_SENDER_UID );
}

_IMPL_ON_FUNC( EGB_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT, KEGS_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT )
{
	VERIFY_AUTO_PARTY_STATE( ( 1, KAutoPartyFSM::S_CHECK_ACCEPT ) );

	// ������ ������� ���� üũ�Ѵ�!
	if( kPacket_.m_bGameStartAgree == true )
	{
		if( m_kAutoPartyUserManager.Accept( FIRST_SENDER_UID ) == false )
		{
			START_LOG( cerr, L"������ �����Ͽ����ϴ�!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
			return;
		}
	}
	else
	{
		// �ڵ� ��Ƽ ���
		CloseAutoPartyByCancel( FIRST_SENDER_UID );
		return;
	}

	// ��� ������ ������ �Ϸ�Ǿ��ٸ�?
	if( m_kAutoPartyUserManager.IsAllPlayersAccept() == true )
	{
		// PartyUserInfo�� RoomUserInfo��û�ϱ� ���� �̸� �������!
		m_kAutoPartyUserManager.ClearAllPlayerRegroupPartyInfo();

		// ���¸� �÷��� ��� ���·� �ٲ���!
		StateTransition( KAutoPartyFSM::I_TO_REGROUP_PARTY );

		// �÷��� �غ� Ÿ�̸� �ʱ�ȭ
		m_tTimer[APTE_REGROUP_PARTY].restart();

        // �ڵ� ��Ƽ�� �����ϱ� ���� �� �������� RoomUserInfo�� ���´�!
		KEGB_GET_REGROUP_PARTY_INFO_REQ kPacket;
		kPacket.m_iAutoPartyUID = GetUID();
		kPacket.m_kDungeonGameInfo.m_cDungeonMode = m_kAutoPartyInfo.m_cDungeonMode;
		kPacket.m_kDungeonGameInfo.m_iDungeonID = m_kAutoPartyInfo.m_iDungeonID;
		kPacket.m_kDungeonGameInfo.m_cDifficulty = m_kAutoPartyInfo.m_cDifficultyLevel;
		BroadCast( EGB_GET_REGROUP_PARTY_INFO_REQ, kPacket );
	}
}

IMPL_ON_FUNC( EGB_GET_REGROUP_PARTY_INFO_ACK )
{
	VERIFY_AUTO_PARTY_STATE( ( 1, KAutoPartyFSM::S_REGROUP_PARTY ) );

	// ��Ƽ ���ġ�� ���� ���� ������Ʈ!
	if( m_kAutoPartyUserManager.UpdateRegroupPartyInfo( kPacket_.m_kPartyUserInfo, kPacket_.m_kPartyRoomUserInfo ) == false )
	{
		START_LOG( cerr, L"�ش� ��ġ ������ RegroupPartyInfo ������Ʈ�� �����Ͽ���." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_kPartyUserInfo.m_iUnitUID )
			<< END_LOG;

		// �� ������ �ͼ� �̰� �����ϸ� �׳� ��ġ�� ������ �ۿ� ����!
		CloseAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_REASON_02 );
		return;
	}

	// ��� ��ġ �������� PartyUserInfo�� RoomUserInfo�� �����Ǿ����� Ȯ��!
	if( m_kAutoPartyUserManager.IsAllPlayersUpdatedRegroupPartyInfo() == true )
	{
		// ��Ƽ ���� ���� �����
		KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ kPacketReq;
		kPacketReq.m_iAutoPartyUID = GetUID();
		GetCreatePartyInfo( kPacketReq.m_kCreatePartyInfo );

		// ��Ƽ regroup���� ���
		//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		if( m_kAutoPartyUserManager.GetRegroupPartyInfo( kPacketReq.m_vecRegroupPartyUserInfo, kPacketReq.m_iAutoPartyHostUnitUID ) == false )
#else
		if( m_kAutoPartyUserManager.GetRegroupPartyInfo( kPacketReq.m_vecRegroupPartyUserInfo ) == false )
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		{
			START_LOG( cerr, L"�ش� ��ġ ������ RegroupPartyInfo ������ �ҷ����Ⱑ �����Ͽ���." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;

			// �� ������ �ͼ� �̰� �����ϸ� �׳� �ڵ���Ƽ�� ������ �ۿ� ����!
			CloseAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_REASON_02 );
			return;
		}

		// ��Ƽ ��Ἲ�ϴ� �ο����� üũ!
		if( kPacketReq.m_vecRegroupPartyUserInfo.size() > 4 )
		{
			START_LOG( cerr, L"�ڵ� ��Ƽ ��Ī �ο����� 4���� �Ѿ���! �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( kPacketReq.m_vecRegroupPartyUserInfo.size() )
				<< END_LOG;
		}
		
		// ��Ƽ�� ��Ἲ�Ϸ� ����!
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ, kPacketReq );

		//////////////////////////////////////////////////////////////////////////		

		// ���� ������ ���������� ���¸� �ٲ���!
		StateTransition( KAutoPartyFSM::I_TO_PLAY );

		// �÷��� Ÿ�̸� �ʱ�ȭ
		m_tTimer[APTE_PLAY].restart();

		//////////////////////////////////////////////////////////////////////////
		// ���������� �ڵ� ��Ƽ�� �����ߴ� �ð� ������ �� �ڵ���Ƽ ����ŷ ���μ����� ó������!
		
		//{{ 2013. 09. 30	�ڼ���		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
		int iDungeonIDWithDif = m_kAutoPartyInfo.m_iDungeonID;
		switch( m_kAutoPartyInfo.m_cDungeonMode )
		{
		case CXSLDungeon::DM_HENIR_PRACTICE:
		case CXSLDungeon::DM_HENIR_CHALLENGE:
		case CXSLDungeon::DM_SECRET_NORMAL:
		case CXSLDungeon::DM_SECRET_HELL:
			iDungeonIDWithDif += m_kAutoPartyInfo.m_cDifficultyLevel;	// ���̵� �� �߰�
			break;

		default:
			break;
		}

		SiKAutoPartyMakingManager()->RefreshTimerAutoPartyGameStart( m_kAutoPartyInfo.m_cDungeonMode, iDungeonIDWithDif );
#else
		SiKAutoPartyMakingManager()->RefreshTimerAutoPartyGameStart( m_kAutoPartyInfo.m_cDungeonMode, m_kAutoPartyInfo.m_iDungeonID );
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
		//}}
	}
}

_IMPL_ON_FUNC( EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK )
{
	// �̹� �ݱ�� �ִ� ��Ȳ�� �� ��Ŷ�� �ü��� �ִ�. �̷��� �׳� ����!
	if( GetStateID() == KAutoPartyFSM::S_CLOSE )
		return;

	VERIFY_AUTO_PARTY_STATE( ( 1, KAutoPartyFSM::S_PLAY ) );

    // �ڵ� ��Ƽ ���� ����!
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
        START_LOG( cerr, L"�ڵ� ��Ƽ ���� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		// ���� ���� ���� ����! �ڵ� ��Ƽ�� ��������!
		CloseAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_REASON_02 );
	}
	else
	{
		// �ڵ� ��Ƽ�� ��������!
		CloseAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_REASON_00 );
	}
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}

