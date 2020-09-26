#include "NewCnParty.h"
//{{ 2011. 12. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewCnPartyManager.h"
	#include "X2Data/XSLBattleFieldManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#include "CnPartyFSM.h"
#include "NetError.h"
#include "X2Data/XSLRoom.h"
#include "X2Data/XSLDungeonManager.h"
#include "X2Data/XSLPartyManager.h"
#include "Lottery.h"
//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	#include "Enum/Enum.h"
#endif SERV_PVP_NEW_SYSTEM
//}}
//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
#include "Socket/NetCommon.h"		// ������ ����(����� �� ����) IP
#include "CnNetLayer.h"				// ������ ����(����� �� ����) Port
#endif SERV_CHOOSE_FASTEST_HOST
//}

//////////////////////////////////////////////////////////////////////////
//{{ 2011. 12. 05	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////

//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	#include "..\Common\AutoParty\AutoPartyScriptManager.h"
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
//}}

#define CLASS_TYPE KCnParty
ImplPfID( KCnParty, PI_CN_PARTY );

KCnParty::KCnParty( UidType iUID ) :
m_iUID( iUID )
{
	Reset();
}

KCnParty::~KCnParty()
{
}

void KCnParty::Reset()
{
	//{{ 2011. 12. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	for( int iIdx = 0; iIdx < TE_MAX; ++iIdx )
	{
		m_tTimer[iIdx].restart();
	}
	m_bRemakingAutoParty = false;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	SetPublic( false );
	SetItemType( 1 );
	SetDungeonDif( 0 );
	SetDungeonID( 0 );
	//{{ 2011. 06. 28	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	m_cPartyType = KPartyInfo::PT_NONE;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	m_wstrName.clear();
	SetPlaying( false );
	m_eDungeonMode = CXSLDungeon::DM_INVALID;

	//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	m_bCheckLowLevel = false;
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}
	//{{ 2011. 06. 20	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	m_cPvpGameType = CXSLRoom::PGT_NULL;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	m_kPartyUserManager.Reset();
	m_kGameManager.Reset();
	//{{ 2012. 08. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_kFeverManager.Reset();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
}

void KCnParty::SetInfo( IN const KPartyInfo& kInfo )
{
	SetName( kInfo.m_wstrPartyName );
	SetDungeonID( kInfo.m_iDungeonID );
	SetDungeonDif( kInfo.m_DifficultyLevel );
	//{{ 2011. 06. 28	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	m_cPartyType = kInfo.m_cPartyType;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	m_kPartyUserManager.SetMaxMember( kInfo.m_MaxSlot );
	SetPublic( kInfo.m_bPublic );
	SetItemType( kInfo.m_cGetItemType );
	SetDungeonMode( (CXSLDungeon::DUNGEON_MODE) kInfo.m_cDungeonMode );
	//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	SetCheckLowLevel( kInfo.m_bCheckLowLevel );
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}
	//{{ 2011. 06. 20	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	m_cPvpGameType = kInfo.m_cPVPGameType;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	m_kPartyUserManager.SetComeBackUserInParty( kInfo.m_bComeBackBuff );
	m_kPartyUserManager.SetComeBackBuffEnd( kInfo.m_wsrtComeBackBuffEnd );
#endif SERV_COME_BACK_USER_REWARD
	//}} 
}

//{{ 2012. 01. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnParty::SetInfo( IN const KCreatePartyInfo& kInfo )
{
	KPartyInfo kCreatePartyInfo;
	kCreatePartyInfo.m_cPartyType		 = kInfo.m_cPartyType;
	kCreatePartyInfo.m_wstrPartyName	 = L"�ڵ�������Ƽ";
	kCreatePartyInfo.m_MaxSlot			 = 4;
	kCreatePartyInfo.m_iDungeonID		 = kInfo.m_iDungeonID;
	kCreatePartyInfo.m_DifficultyLevel	 = kInfo.m_DifficultyLevel;
	kCreatePartyInfo.m_cGetItemType		 = kInfo.m_cGetItemType;
	kCreatePartyInfo.m_cDungeonMode		 = kInfo.m_cDungeonMode;
	kCreatePartyInfo.m_bCheckLowLevel	 = kInfo.m_bCheckLowLevel;
	kCreatePartyInfo.m_cPVPGameType		 = kInfo.m_cPVPGameType;
	SetInfo( kCreatePartyInfo );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

void KCnParty::GetInfo( OUT KPartyInfo& kInfo )
{
	//�⺻����
	kInfo.m_bPublic			= IsPublic();
	kInfo.m_cGetItemType	= GetItemType();
	kInfo.m_DifficultyLevel	= GetDungeonDif();
	kInfo.m_iDungeonID		= GetDungeonID();
	//{{ 2011. 06. 28	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	kInfo.m_cPartyType		= m_cPartyType;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	kInfo.m_iPartyUID		= GetUID();
	kInfo.m_JoinSlot		= m_kPartyUserManager.GetNumMember();
	kInfo.m_MaxSlot			= m_kPartyUserManager.GetMaxMember();
	kInfo.m_wstrPartyName	= GetName();	
	kInfo.m_cDungeonMode	= static_cast<char>( GetDungeonMode() );
	//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	kInfo.m_sPartyFever		= GetFeverPoint();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	kInfo.m_bCheckLowLevel	= GetCheckLowLevel();
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}
	//{{ 2011. 06. 20	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	kInfo.m_cPVPGameType = m_cPvpGameType;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	CTime tBuffEnd = m_kPartyUserManager.GetComeBackBuffEnd();
	kInfo.m_bComeBackBuff	= m_kPartyUserManager.GetComeBackUserInParty();
	kInfo.m_wsrtComeBackBuffEnd	= tBuffEnd.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
#endif SERV_COME_BACK_USER_REWARD
	//}} 
}

void KCnParty::GetInfo( OUT std::vector< KPartyInfo >& vecInfo )
{
	vecInfo.clear();
    KPartyInfo kInfo;
	GetInfo( kInfo );
	vecInfo.push_back( kInfo );
}

//{{ 2012. 05. 10	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnParty::GetDungeonGameInfo( OUT KDungeonGameInfo& kInfo ) const
{
	kInfo.m_cDungeonMode = GetDungeonMode();
	kInfo.m_iDungeonID = GetDungeonID();
	kInfo.m_cDifficulty = GetDungeonDif();
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

void KCnParty::SetDungeonMode( const CXSLDungeon::DUNGEON_MODE eDungeonMode )
{
	if( CXSLDungeon::IsValidDungeonMode( eDungeonMode ) == false )
	{
		START_LOG( cerr, L"���������� DugneonMode����." )
			<< BUILD_LOG( eDungeonMode )
			<< END_LOG;

		m_eDungeonMode = CXSLDungeon::DM_INVALID;
		return;
	}

	m_eDungeonMode = eDungeonMode;
}

void KCnParty::Tick()
{
	// �̺�Ʈ�� ���� ó���ѵ�..
	KPerformer::Tick();

	switch( GetStateID() )
	{
	case KCnPartyFSM::S_INIT:
	case KCnPartyFSM::S_CLOSE:
		break;

	case KCnPartyFSM::S_MAKING_PARTY:
		{
			// 7�ʵ��� ù��° ��Ƽ�� �ʴ밡 �Ϸ���� �ʴ´ٸ� ��Ƽ�� �ݴ´�!
			if( m_tTimer[TE_FIRST_INVITE].elapsed() > PARTY_INVITE_TIME_LIMIT )
			{
				CloseParty( NetError::NOT_CLOSE_PARTY_REASON_02 );
			}
		}
		break;

	case KCnPartyFSM::S_WAIT:
		break;

	case KCnPartyFSM::S_CHECK_PLAY_CONDITION:
		{
			if( m_tTimer[TE_CHECK_PLAY_CONDITION].elapsed() > 5.0 )
			{
				START_LOG( cerr, L"��Ƽ ���� ���� ���� ���� üũ�� �� ��Ƽ���鿡�� �����Ͽ��µ� 5���̻� ������ ������. �Ͼ���� �ȵǴ� ����!" )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( m_kPartyUserManager.GetNumMember() )
					<< END_LOG;

				// ���� ó��!
				FailProcessPlayCondition( NetError::ERR_PARTY_45 );
			}
		}
		break;

	case KCnPartyFSM::S_CHECK_AUTO_PARTY:
		{
			if( m_tTimer[TE_CHECK_AUTO_PARTY].elapsed() > 5.0 )
			{
				START_LOG( cerr, L"�ڵ� ��Ƽ ��û�� ���� üũ�� ���´µ� 5�ʵ��� ������ ������. �Ͼ���� �ȵǴ� ����!" )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( m_kPartyUserManager.GetNumMember() )
					<< END_LOG;

				// ���� ó��!
				FailProcessAutoParty( NetError::ERR_UNKNOWN );
			}
		}
		break;

	case KCnPartyFSM::S_ACCEPT_FOR_PLAY:
		{
			if( m_tTimer[TE_ACCEPT_FOR_PLAY].elapsed() > 10.0 )
			{
				// �÷��� ������ ���� ���� ó���� 10���̻� �����Ǿ����Ƿ� ��Ƽ ���� ������ ���� ó�� �մϴ�!
				FailProcessAcceptForPlay( NetError::NOT_PARTY_GAME_START_FAILED_01 );
			}
		}
		break;

	case KCnPartyFSM::S_PREPARE_FOR_PLAY:
		{
			if( m_tTimer[TE_PREPARE_FOR_PLAY].elapsed() > 5.0 )
			{
				START_LOG( cerr, L"��Ƽ ���� ������ ���� ���� ������ ���´µ� 5�ʵ��� ������ ������. �Ͼ���� �ȵǴ� ����!" )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( m_kPartyUserManager.GetNumMember() )
					<< END_LOG;

				// ���� ���� ���� ������ 5�� �̻� �����Ǿ� ���� ������ ��� ó�� �մϴ�!
				FailProcessPrepareForPlay( NetError::ERR_PARTY_23 );
			}
		}
		break;
	}

	// ��û ����� üũ
	//{{ 2012. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	if( GetStateID() != KCnPartyFSM::S_MAKING_PARTY )
	{
		std::vector< UidType > vecDeleteCadidateUser;
		m_kPartyUserManager.Tick( vecDeleteCadidateUser );
		if( vecDeleteCadidateUser.empty() == false )
		{
			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecDeleteCadidateUser )
			{
				LIF( LeaveCandidateMember( iUnitUID ) );
			}
		}
	}
#else
	m_kPartyUserManager.Tick();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//��Ƽ ������ 1�� �̻��̰�
	//������������ �ƴҶ� 3�ʸ��� ������ stateid �� Ȯ���Ͽ�
	//close �ϰ�� leave ó��.
}

// ��Ÿ�� �ʱ�ȭ
void KCnParty::AllPartyMembersQuickSlotCoolTimeReset()
{
	// ��Ƽ���鿡�� ������ ��Ÿ�� ���� ��Ŷ ��ε� ĳ����
	BroadCastID( EPM_QUICK_SLOT_COOL_TIME_RESET_NOT );
}

void KCnParty::UpdateAndBroadCastFeverPoint( IN const int iFeverPoint )
{
	// ��Ƽ ��� �� ���ϱ�
	const int iPartyMemberCount = m_kPartyUserManager.GetNumMember();

	// ��Ƽ �ǹ� ������Ʈ!
	int iResultFeverPoint = 0;
	const KCnPartyFeverManager::UPDATE_FEVER_POINT eRet = m_kFeverManager.UpdateFeverPoint( iFeverPoint, iPartyMemberCount, iResultFeverPoint );
	switch( eRet )
	{
	case KCnPartyFeverManager::UFP_ACTIVE_FEVER:
		{
			// fever�ߵ� �̺�Ʈ ������
			KEGS_CHANGE_PARTY_FEVER_STATE_NOT kNot;
			kNot.m_bIsStart = true;
			kNot.m_iPartyFever = iResultFeverPoint;
			BroadCast( EPM_CHANGE_PARTY_FEVER_STATE_NOT, kNot );
		}
		break;

	case KCnPartyFeverManager::UFP_CHANGE_FEVER:
		{
			// ���� fever��ġ ������
			KEGS_PARTY_FEVER_POINT_NOT kNot;
			kNot.m_iPartyFever = iResultFeverPoint;
			BroadCast( EPM_PARTY_FEVER_POINT_NOT, kNot );
		}
		break;

	default:
		return;
	}

	// ����� ��Ƽ fever������ ��� ���Ӽ����� ��Ƽ ����Ʈ�� ����!
	UpdatePartyFeverInPartyList( true );
}

void KCnParty::IncreaseFeverPoint( IN const char cRoomType, IN const short sSubStageNpcCount )
{
	// ��Ƽ���� ���ϱ�
	const int iPartyMemberCount = m_kPartyUserManager.GetNumMember();

	// ��Ƽ fever ������ ���ϱ�
	const int iIncreaseFeverPoint = m_kFeverManager.CalcIncreaseFeverPoint( iPartyMemberCount, cRoomType, sSubStageNpcCount, m_kPartyUserManager.CheckComeBackUserInParty() );
	if( iIncreaseFeverPoint == 0 )
		return;
	
	// ��Ƽ �ǹ� ������Ʈ!
	UpdateAndBroadCastFeverPoint( m_kFeverManager.GetRealFeverPoint() + iIncreaseFeverPoint );
}

void KCnParty::DecreaseFeverPoint( IN const int iDecreaseFeverPoint )
{
	// �ǹ� ����Ʈ ����!
	UpdateAndBroadCastFeverPoint( m_kFeverManager.GetRealFeverPoint() - iDecreaseFeverPoint );	
}
//}}

void KCnParty::AddPartyInfoInPartyList()
{
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	if( GetPartyType() != KPartyInfo::PT_DUNGEON_PARTY )
		return;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	KEPM_ADD_PARTY_LIST_NOT kPacket;
	kPacket.m_iPartyUID = GetUID();
	BroadCastGameServer( EPM_ADD_PARTY_LIST_NOT, kPacket );
}

//{{ 2011. 07. 11	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
void KCnParty::DelPartyInfoInPartyList( IN const bool bForce /*= false*/ )
//#else
//void KCnParty::DelPartyInfoInPartyList()
//#endif SERV_PVP_NEW_SYSTEM
//}}
{
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	if( GetPartyType() != KPartyInfo::PT_DUNGEON_PARTY  &&  bForce == false )
		return;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	KEPM_DEL_PARTY_LIST_NOT kPacket;
	kPacket.m_iPartyUID = GetUID();
	BroadCastGameServer( EPM_DEL_PARTY_LIST_NOT, kPacket );
}

//{{ 2011. 07. 23	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
void KCnParty::UpdatePartyInfoInPartyList( IN const bool bForce /*= false*/ )
//#else
//void KCnParty::UpdatePartyInfoInPartyList()
//#endif SERV_PVP_NEW_SYSTEM
//}}
{
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	if( GetPartyType() != KPartyInfo::PT_DUNGEON_PARTY  &&  bForce == false )
		return;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	KEPM_UPDATE_PARTY_IN_PARTY_LIST_NOT kPacket;
	GetInfo( kPacket.m_kPartyInfo );
	BroadCastGameServer( EPM_UPDATE_PARTY_IN_PARTY_LIST_NOT, kPacket );
}

//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnParty::UpdatePartyFeverInPartyList( IN const bool bForce /*= false*/ )
{
	//{{ 2011. 07. 11	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
//	if( GetPartyType() != KPartyInfo::PT_DUNGEON_PARTY  &&  bForce == false )
//		return;
//#endif SERV_PVP_NEW_SYSTEM
	//}}

	KEPM_UPDATE_PARTY_FEVER_IN_PARTY_LIST_NOT kPacket;
	kPacket.m_iPartyUID = GetUID();
	kPacket.m_sPartyFever = GetFeverPoint();
	BroadCastGameServer( EPM_UPDATE_PARTY_FEVER_IN_PARTY_LIST_NOT, kPacket );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

void KCnParty::AddPartyMemberInPartyList( IN const KPartyUserInfo& kInfo )
{
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	if( GetPartyType() != KPartyInfo::PT_DUNGEON_PARTY )
		return;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	KEPM_ADD_PARTY_MEMBER_IN_PARTY_LIST_NOT kPacket;
	kPacket.m_iPartyUID = GetUID();
	kPacket.m_kPartyUserInfo = kInfo;
	BroadCastGameServer( EPM_ADD_PARTY_MEMBER_IN_PARTY_LIST_NOT, kPacket );
}

//{{ 2011. 07. 11	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
void KCnParty::DelPartyMemberInPartyList( IN const UidType iUnitUID, IN const bool bForce /*= false*/ )
//#else
//void KCnParty::DelPartyMemberInPartyList( IN UidType iUnitUID )
//#endif SERV_PVP_NEW_SYSTEM
//}}
{
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	if( GetPartyType() != KPartyInfo::PT_DUNGEON_PARTY  &&  bForce == false )
		return;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	KEPM_DEL_PARTY_MEMBER_IN_PARTY_LIST_NOT kPacket;
	kPacket.m_iPartyUID = GetUID();
	kPacket.m_iUnitUID = iUnitUID;
	BroadCastGameServer( EPM_DEL_PARTY_MEMBER_IN_PARTY_LIST_NOT, kPacket );
}

//{{ 2012. 03. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnParty::UpdateJoinRoomInfoAndGetOnOffResult( IN const UidType iUnitUID, IN const KPartyUserJoinRoomInfo& kPartyUserJoinRoomInfo )
{
	std::vector< UidType > vecOnMoveButton;
	std::vector< UidType > vecOffMoveButton;

	// �� ���� ������ ������Ʈ
	if( m_kPartyUserManager.UpdateJoinRoomInfoAndGetOnOffResult( iUnitUID, kPartyUserJoinRoomInfo, vecOnMoveButton, vecOffMoveButton ) == false )
	{
		START_LOG( cerr, L"��Ƽ���� �� ���� ������ ������Ʈ �����Ͽ����ϴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return;
	}

	// �ʵ忡�� �ʵ��, �ʵ忡�� �ٸ������� �ٸ������� �ʵ�� �̵��ѰŶ�� ���ο� ������ ó������!
	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecOnMoveButton )
	{
		KCnPartyUserPtr spPartyUser = m_kPartyUserManager.GetPartyUser( iUnitUID );
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"��Ƽ������ �������� �ʽ��ϴ�!" )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		KEGS_UPDATE_MOVE_TO_PARTY_MEMBER_INFO_NOT kNot;
		kNot.m_bIsOn = true;
		spPartyUser->SendToGSCharacter( EPM_UPDATE_MOVE_TO_PARTY_MEMBER_INFO_NOT, kNot );
	}

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecOffMoveButton )
	{
		KCnPartyUserPtr spPartyUser = m_kPartyUserManager.GetPartyUser( iUnitUID );
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"��Ƽ������ �������� �ʽ��ϴ�!" )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		KEGS_UPDATE_MOVE_TO_PARTY_MEMBER_INFO_NOT kNot;
		kNot.m_bIsOn = false;
		spPartyUser->SendToGSCharacter( EPM_UPDATE_MOVE_TO_PARTY_MEMBER_INFO_NOT, kNot );
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 05. 22	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
KOldPartyInfo KCnParty::MakeOldPartyInfo() const
{
	KOldPartyInfo kInfo;
	kInfo.m_iOldPartyUID = GetUID();
	//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	kInfo.m_iOldHostUnitUID = m_kPartyUserManager.GetHostUID();
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}
	kInfo.m_sOldPartyFever = GetFeverPoint();
	m_kPartyUserManager.GetUnitUIDList( kInfo.m_setOldPartyMember );
	return kInfo;
}

void KCnParty::CheckAllMembersPlayCondition()
{
	if( GetStateID() != KCnPartyFSM::S_CHECK_PLAY_CONDITION )
	{
		START_LOG( cerr, L"�߸��� ���¿��� �Լ��� ȣ��Ǿ����ϴ�!" )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}

	// ��� ������ üũ�� �Ϸ�Ǹ� ���� �ܰ�� ����!
	if( m_kPartyUserManager.IsAllPartyMembersCheckedPlayCondition() == true )
	{
		// ��Ƽ�� ã��!
		KCnPartyUserPtr spHost = m_kPartyUserManager.GetHostPtr();
		if( IS_NULL( spHost ) )
		{
			START_LOG( cerr, L"��Ƽ���� �������� �ʽ��ϴ�! �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( m_kPartyUserManager.GetNumMember() )
				<< END_LOG;

			// �Ͼ���� �ȵǴ� �������� ���࿡ �Ͼ�ٸ� ��� �ϸ� ������..?
			return;
		}

		// ��� ��Ƽ���� readyüũ �ʱ�ȭ
		m_kPartyUserManager.ChangeAllReady( false );

		// ��� ��Ƽ������ PartyRoomUserInfo����
		m_kPartyUserManager.ClearDungeonRoomUserInfoList();

#ifdef CORRECT_CHECKING_TICKET_TO_DUNGEON	// ���볯¥: 2013-03-05
		// ������ üũ �Ϸ� �÷��� �ʱ�ȭ
		m_kPartyUserManager.ClearCheckedPlayCondition();
#endif // CORRECT_CHECKING_TICKET_TO_DUNGEON
		
		// ���� Ÿ�̸� �ʱ�ȭ
		m_tTimer[TE_ACCEPT_FOR_PLAY].restart();

		// ���ӽ����� ���� ��� ��Ƽ������ ���� üũ ���·� �̵�!
		StateTransition( KCnPartyFSM::I_TO_ACCEPT_FOR_PLAY );

		// ��û�� ���� ����
		KEPM_PARTY_GAME_START_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		kPacket.m_usEventID = m_kGameManager.GetGameStartEventID();
		spHost->SendToGSCharacter( EPM_PARTY_GAME_START_ACK, kPacket );

		// ��� ��Ƽ���鿡�� ���� ���ۿ� ���� �ǰ� ����! : ��Ƽ�� ����
		//{{ 2012. 12. 17	�ڼ���	���� ��Ƽ �� ���� �˾� â ���� ���� ����
#ifdef SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
		{
			KEGS_PARTY_GAME_START_OPINION_CHECK_NOT kPacket;
			kPacket.m_iDungeonID		= GetDungeonID();
			kPacket.m_cDifficultyLevel	= GetDungeonDif();
			kPacket.m_cDungeonMode		= GetDungeonMode();
			BroadCast( EPM_PARTY_GAME_START_OPINION_CHECK_NOT, kPacket );
		}
#else
		BroadCastID( EPM_PARTY_GAME_START_OPINION_CHECK_NOT );
#endif SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
		//}}
	}
}

void KCnParty::FailProcessPlayCondition( IN const int iNetError, IN const UidType iFailUnitUID /*= 0*/, IN const std::wstring wstrFailUserNickName /*= L""*/ )
{
	if( GetStateID() != KCnPartyFSM::S_CHECK_PLAY_CONDITION )
	{
		START_LOG( cerr, L"�߸��� ���¿��� �Լ��� ȣ��Ǿ����ϴ�!" )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}

	// �ٽ� S_WAIT���·� �̵�!
	StateTransition( KCnPartyFSM::I_TO_WAIT );

	// ���� üũ �ʱ�ȭ
	m_kPartyUserManager.ClearCheckedPlayCondition();

	// ��Ƽ���� ã��!
	KCnPartyUserPtr spHost = m_kPartyUserManager.GetHostPtr();
	if( spHost == NULL )
	{
		START_LOG( cerr, L"��Ƽ���� ����! ����� ������ ���� ����!" )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;
		return;
	}

	KEPM_PARTY_GAME_START_ACK kPacket;
	kPacket.m_iOK = iNetError;
	kPacket.m_iFailUnitUID = iFailUnitUID;
	kPacket.m_wstrFailUserNickName = wstrFailUserNickName;
	kPacket.m_iDungeonMode = GetDungeonMode();
	kPacket.m_iDungeonID = GetDungeonIDAndDif();
	spHost->SendToGSCharacter( EPM_PARTY_GAME_START_ACK, kPacket );
}

void KCnParty::CheckAllMembersAutoParty()
{
	if( GetStateID() != KCnPartyFSM::S_CHECK_AUTO_PARTY )
	{
		START_LOG( cerr, L"�߸��� ���¿��� �Լ��� ȣ��Ǿ����ϴ�!" )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}
    
	// ��� ��Ƽ���� �ڵ� ��Ƽ üũ�� �Ϸ�Ǿ����� Ȯ��!
	if( m_kPartyUserManager.IsAllPartyMembersCheckedForAutoParty() == true )
	{
		// ���� �̵�!
		StateTransition( KCnPartyFSM::I_TO_WAIT );

		// Ȥ�� �𸣴ϱ� �غ� �ʱ�ȭ����!
		m_kPartyUserManager.ClearCheckedForAutoParty();

		// ��Ƽ�� ã�Ƽ�!
		KCnPartyUserPtr spPartyHost = m_kPartyUserManager.GetHostPtr();
		if( spPartyHost == NULL )
		{
			START_LOG( cerr, L"��Ƽ���� ����? ������ ���� ����!" )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;
			return;
		}

		// ���� ���
		KEPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		kPacket.m_bRemakingAutoParty = m_bRemakingAutoParty; // ���� ����!
		kPacket.m_kAutoPartyCondition.m_iDungeonID = GetDungeonID();
		kPacket.m_kAutoPartyCondition.m_cDifficultyLevel = GetDungeonDif();
		kPacket.m_kAutoPartyCondition.m_cGetItemType = GetItemType();
		kPacket.m_kAutoPartyCondition.m_cDungeonMode = static_cast<char>(GetDungeonMode());
		m_kPartyUserManager.GetAutoPartyUserInfoList( MakeOldPartyInfo(), kPacket.m_vecAutoPartyUser );
		spPartyHost->SendToGSCharacter( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK, kPacket );
	}
}

void KCnParty::FailProcessAutoParty( IN const int iNetError, IN const std::wstring wstrFailUserNickName /*= L""*/ )
{
	if( GetStateID() != KCnPartyFSM::S_CHECK_AUTO_PARTY )
	{
		START_LOG( cerr, L"�߸��� ���¿��� �Լ��� ȣ��Ǿ����ϴ�!" )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}

	// ���� �̵�!
	StateTransition( KCnPartyFSM::I_TO_WAIT );

	// Ȥ�� �𸣴ϱ� �غ� �ʱ�ȭ����!
	m_kPartyUserManager.ClearCheckedForAutoParty();

	// ��Ƽ�� ã�Ƽ�
	KCnPartyUserPtr spPartyHost = m_kPartyUserManager.GetHostPtr();
	if( spPartyHost == NULL )
	{
		START_LOG( cerr, L"��Ƽ���� ����? ������ ���� ����!" )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;
		return;
	}

	// �ڵ� ��Ƽ ���� üũ ���¸� ���� ����!
	KEPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK kAck;
	kAck.m_iOK = iNetError;
	kAck.m_wstrFailUserNickName = wstrFailUserNickName;
	kAck.m_kAutoPartyCondition.m_iDungeonID = GetDungeonID();
	kAck.m_kAutoPartyCondition.m_cDifficultyLevel = GetDungeonDif();
	kAck.m_kAutoPartyCondition.m_cDungeonMode = static_cast<char>(GetDungeonMode());
	spPartyHost->SendToGSCharacter( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK, kAck );
}

void KCnParty::CheckAllMembersAcceptForPlay()
{
	if( GetStateID() != KCnPartyFSM::S_ACCEPT_FOR_PLAY )
	{
		START_LOG( cerr, L"�߸��� ���¿��� �Լ��� ȣ��Ǿ����ϴ�!" )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}

	// ��� ��Ƽ���� �غ� �Ǿ����� Ȯ���մϴ�!
	if( m_kPartyUserManager.IsAllReady() == true )
	{
		// �÷��� ���� ������ ���� Ÿ�̸� �ʱ�ȭ
		m_tTimer[TE_PREPARE_FOR_PLAY].restart();

		// ��Ƽ ���� ���� ���ǰ� �Ϸ�Ǿ����ϴ�. ��Ƽ�� ���� ������ ���� �ٽ� �����̵�!
		StateTransition( KCnPartyFSM::I_TO_PREPARE_FOR_PLAY );

		// ��� ��Ƽ������ PartyRoomUserInfo���� - �Ʊ� ������� Ȥ�ó� �ؼ� �ѹ� �� ���ϴ�.
		m_kPartyUserManager.ClearDungeonRoomUserInfoList();

		//////////////////////////////////////////////////////////////////////////
		// ������ ���� �غ� ����.
		KEPM_CHECK_FOR_PARTY_GAME_START_REQ kPacketReq;
		kPacketReq.m_iPartyUID = GetUID();
		kPacketReq.m_usEventID = m_kGameManager.GetGameStartEventID();
		kPacketReq.m_sWorldID  = m_kGameManager.GetGameStartWorldID();
		GetDungeonGameInfo( kPacketReq.m_kDungeonGameInfo );
		BroadCast( EPM_CHECK_FOR_PARTY_GAME_START_REQ, kPacketReq );
	}	
}

void KCnParty::FailProcessAcceptForPlay( IN const int iNetError, IN const UidType iFailUnitUID /*= 0*/ )
{
	if( GetStateID() != KCnPartyFSM::S_ACCEPT_FOR_PLAY )
	{
		START_LOG( cerr, L"�߸��� ���¿��� �Լ��� ȣ��Ǿ����ϴ�!" )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}

	// ��Ƽ ���¸� Wait�� �̵��մϴ�.
	StateTransition( KCnPartyFSM::I_TO_WAIT );

	// ��� ��Ƽ������ ��Ƽ���ӽ��� ���� �޽����� �����ϴ�.
	KEGS_PARTY_GAME_START_FAILED_NOT kPacketNot;
	kPacketNot.m_iReason = iNetError;
	kPacketNot.m_iFailUnitUID = iFailUnitUID;
	BroadCast( EPM_PARTY_GAME_START_FAILED_NOT, kPacketNot );
}

void KCnParty::FailProcessPrepareForPlay( IN const int iNetError )
{
	if( GetStateID() != KCnPartyFSM::S_PREPARE_FOR_PLAY )
	{
		START_LOG( cerr, L"�߸��� ���¿��� �Լ��� ȣ��Ǿ����ϴ�!" )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}

	// ��Ƽ ���¸� Wait�� �̵��մϴ�.
	StateTransition( KCnPartyFSM::I_TO_WAIT );
    
	// ��� ��Ƽ������ ��Ƽ���ӽ��� ���� �޽����� �����ϴ�.
	KEGS_PARTY_GAME_START_FAILED_NOT kPacketNot;
	kPacketNot.m_iReason = iNetError;
	BroadCast( EPM_PARTY_GAME_START_FAILED_NOT, kPacketNot );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2011. 12. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
bool KCnParty::LeaveCandidateMember( IN const UidType iUnitUID )
{
	KCnPartyUserPtr spCandidateUser = m_kPartyUserManager.GetCandidateUser( iUnitUID );
	if( IS_NULL( spCandidateUser ) )
	{
		START_LOG( cerr, L"��Ƽ ���� ����� ������ �ƴմϴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	// ��Ƽ ��� �ӽ� ��� ������ �˸���.
	spCandidateUser->SendToGSCharacter( EPM_LEAVE_CANDIDATE_PARTY_NOT, char() );

	// �ӽ� ��� ��Ż ó��
	LIF( m_kPartyUserManager.LeaveCandidate( iUnitUID ) );
	return true;
}

bool KCnParty::LeaveMember( IN const UidType iUnitUID 
						#ifdef LOG_PARTY_BREAK
							, IN const int iReason	
						#endif // LOG_PARTY_BREAK
							)
{
	SET_ERROR( NET_OK );

	// ��Ż�ϴ� ������ �� ���� ������ �ʱ�ȭ ��������!
	UpdateJoinRoomInfoAndGetOnOffResult( iUnitUID, KPartyUserJoinRoomInfo() );

	// ��Ż���� GSUID���� ������.
	UidType iLeaveUnitGSUID = 0;
	if( m_kPartyUserManager.GetGSUID( iUnitUID, iLeaveUnitGSUID ) == false )
	{
		//��������.
		SET_ERROR( ERR_PARTY_01 );
		return false;
	}

	//��Ƽ���� ���������� ������.
	if( m_kPartyUserManager.LeaveMember( iUnitUID ) == false )
	{
		//��������.
		SET_ERROR( ERR_PARTY_01 );
		return false;
	}
	
	// ��Ƽ�� ��Ż�ϴ� ����ڿ��Ե� �������� ��Ż�� �˸���!
	{	
		KEGS_LEAVE_PARTY_NOT kNot;
		kNot.m_iLeaveUnitUID	= iUnitUID;
		kNot.m_iPartyUID		= GetUID();
		kNot.m_bExistComeBackUser = false;
#ifdef LOG_PARTY_BREAK
		kNot.m_iReason			= iReason;
#endif // LOG_PARTY_BREAK

		SendToGSCharacter( iLeaveUnitGSUID, iUnitUID, EPM_LEAVE_PARTY_NOT, kNot );
	}

	// ��Ƽ�� �ƹ��� ����.?
	if( m_kPartyUserManager.GetNumMember() > PE_CLOSE_PARTY_NUM_MEMBER )
	{	
		// ��Ƽ���� ��Ż�� ��� Party Fever�� ����
		DecreaseFeverPoint( KCnPartyFeverManager::FEVER_DECREASE_POINT_BY_LEAVE_PARTY );

		// ����� ��Ƽ ���� ��Ƽ����Ʈ�� ����
		UpdatePartyInfoInPartyList();

		// � ��Ƽ���� ��Ż�ߴ��� �ٸ� ��Ƽ���鿡�� �˸���!
		KEGS_LEAVE_PARTY_NOT kNot;
		kNot.m_iLeaveUnitUID	= iUnitUID;
		kNot.m_iNewHostUID		= m_kPartyUserManager.GetHostUID();
		m_kPartyUserManager.GetUnitUIDList( kNot.m_vecPartyUnitUIDList );
		kNot.m_iPartyUID		= GetUID();
		kNot.m_bExistComeBackUser = m_kPartyUserManager.CheckComeBackUserInParty();
#ifdef LOG_PARTY_BREAK
		kNot.m_iReason			= iReason;
#endif // LOG_PARTY_BREAK
		BroadCast( EPM_LEAVE_PARTY_NOT, kNot );

		//��Ƽ���� ������Ʈ.
		m_kPartyUserManager.UpdatePartyMemberListToFieldUser( GetUID() );

		// ���ο� ��Ƽ���� ���������״� �ֺ����� �����鿡�� ��Ƽ ��ǳ���� �˷�����!
		KCnPartyUserPtr spHost = m_kPartyUserManager.GetHostPtr();
		if( spHost != NULL )
		{
			const bool bLeaveHost = ( iUnitUID == m_kPartyUserManager.GetHostUID() );
			if( bLeaveHost )
			{
				// ��� ���Ӽ����� ��Ƽ ����Ʈ�� ����� ���� ����
				KEPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT kNot;
				kNot.m_iPartyUID = GetUID();
				kNot.m_vecUpdatePartyUser.push_back( spHost->GetPartyUserInfo() );	// ���Ӱ� �Ӹ�� ���� ����
				kNot.m_vecDeletePartyUser.push_back( iUnitUID );					// ��Ż�ϴ� ����
				BroadCastGameServer( EPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT, kNot );
			}
			else
			{
				// ��Ƽ ����Ʈ���� ��Ƽ �ο��� ����!
				DelPartyMemberInPartyList( iUnitUID );
			}
		}
	}
	//{{ 2012. 12. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_CODE_CLEANUP_2012_12_07
	// [2012-12-07][������] ��Ƽ���� �ƹ��� ���°�쿡�� �ش� �ڵ带 ��Ÿ���� ����.
	else if( m_kPartyUserManager.GetNumMember() == PE_CLOSE_PARTY_NUM_MEMBER )
#else
	else
#endif SERV_CODE_CLEANUP_2012_12_07
	//}}
	{
		// ������ �����ֱ� ���� ȥ�� ���� �ִ� ������Ե� ���� �ؾ� �Ѵ�.
		KEGS_LEAVE_PARTY_NOT kNot;
		kNot.m_iLeaveUnitUID	= iUnitUID;
		kNot.m_iNewHostUID		= m_kPartyUserManager.GetHostUID();
		m_kPartyUserManager.GetUnitUIDList( kNot.m_vecPartyUnitUIDList );
		kNot.m_iPartyUID		= GetUID();
		kNot.m_bExistComeBackUser = m_kPartyUserManager.CheckComeBackUserInParty();
#ifdef LOG_PARTY_BREAK
		kNot.m_iReason			= iReason;
#endif // LOG_PARTY_BREAK
		BroadCast( EPM_LEAVE_PARTY_NOT, kNot );
	}
		
	return true;
}

void KCnParty::CloseParty( IN const int iReason )
{
	// ���� ����ڰ� �ִ��� ���� �������ڿ��Ե� ��Ƽ ������ �˸���.
	std::vector< UidType > vecDeleteCadidateUser;
	m_kPartyUserManager.GetCadidateUserList( vecDeleteCadidateUser );

	if( vecDeleteCadidateUser.empty() == false )
	{
		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecDeleteCadidateUser )
		{
			// 7�� ���Ŀ� ������� ���°� ��ȭ�� �������� Ȯ��, �����ٸ� ��Ƽ �ʴ븦 �ź��Ѱ��̹Ƿ� �ʴ� �ź� �޽����� �ʴ��ڿ��� ������.
			KCnPartyUserPtr spHost = m_kPartyUserManager.GetHostPtr();
			if( IS_NULL( spHost ) )
			{
				START_LOG( cerr, L"��Ƽ�� �����Ͱ� �߸��Ǿ���.!" )
					<< END_LOG;
			}
			else
			{
				// �ʴ� ���� ������ �ǻ� ������ ���� �ʰ� 10�ʰ� ����.
				//## ��Ƽ�忡�� �ʴ밡 �����Ǿ��ٰ� �˷���.
				KEGS_INVITE_PARTY_DENY_NOT kDenyNot;
				kDenyNot.m_iDenyReasonNetError = NetError::ERR_PARTY_19;
				kDenyNot.m_iUnitUID = iUnitUID;
				spHost->SendToGSCharacter( EPM_INVITE_PARTY_DENY_NOT, kDenyNot );
			}

			LIF( LeaveCandidateMember( iUnitUID ) );
		}
	}

	// ��Ƽ�� �����ٴ°��� ��� ��Ƽ������ �˸���!
	KEGS_CLOSE_PARTY_NOT kPacketNot;
	kPacketNot.m_iPartyUID = GetUID();
	kPacketNot.m_iReason = iReason;
	BroadCast( EPM_CLOSE_PARTY_NOT, kPacketNot );

	// ��Ƽ �ݱ� ó��
    SiKCnPartyManager()->CloseParty( GetThisPtr<KCnParty>() );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

void KCnParty::BroadCastID( IN u_short usEventID, IN const UidType iUnitUID_ /*= 0*/ )
{
	BroadCast( usEventID, char(), iUnitUID_ );
}

void KCnParty::ProcessEvent( const KEventPtr& spEvent_ )
{
	switch( spEvent_->m_usEventID )
	{
		CASE( EPM_CREATE_PARTY_BY_INVITE_REQ );
		CASE( EPM_INVITE_PARTY_REQ );
		CASE( EPM_INVITE_PARTY_REPLY_NOT );
		CASE( EPM_UPDATE_PARTY_USER_INFO_NOT );
		CASE( EPM_LEAVE_PARTY_REQ );
		CASE( EPM_PARTY_CHANGE_READY_REQ );
		CASE( EPM_PARTY_CHANGE_DUNGEON_REQ );
		CASE( EPM_PARTY_GAME_START_REQ );
		//{{ 2012. 01. 12	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		CASE( EPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_ACK );
	   _CASE( EPM_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT, KEGS_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		CASE( EPM_CHECK_FOR_PARTY_GAME_START_ACK );
		CASE( EPM_END_GAME_NOT );
		CASE( EPM_PARTY_CHANGE_HOST_REQ );
		CASE( EPM_CHECK_PARTY_CHANGE_HOST_ACK );
		CASE( EPM_PARTY_CHANGE_NUM_OF_PER_REQ );
		CASE( EPM_PARTY_CHANGE_GET_ITEM_TYPE_REQ );
		CASE( EPM_PARTY_BAN_USER_REQ );
		CASE( EPM_CHAT_REQ );
		CASE( EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ );
	   _CASE( EPM_INCREASE_PARTY_FEVER_NOT, KERM_INCREASE_PARTY_FEVER_NOT );
	   //{{ 2012. 05. 10	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	   _CASE( EPM_MAX_PARTY_FEVER_NOT, KERM_MAX_PARTY_FEVER_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	   //}}
		CASE( EPM_REQUEST_UPDATE_PARTY_MEMBER_LIST_NOT );
	   _CASE( EPM_CHECK_INVALID_USER_NOT, UidType );
		//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
		CASE( EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ );
#endif SERV_DUNGEON_DIFFICULTY_REVISION
		//}}
		//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	   _CASE( EPM_CHANGE_PARTY_TYPE_REQ, KEGS_CHANGE_PARTY_TYPE_REQ );
		CASE( EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ );
	   _CASE( EPM_PVP_PARTY_CHANGE_MATCH_INFO_REQ, KEGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ );
#endif SERV_PVP_NEW_SYSTEM
		//}}
	   //{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	   CASE( EPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_REQ );
	   CASE( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ );
	   CASE( EPM_CHECK_FOR_AUTO_PARTY_MAKING_ACK );
	   CASE( EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ );
	   CASE( EPM_REQUEST_REGROUP_PARTY_NOT );
	   CASE_NOPARAM( ERM_END_DUNGEON_GAME_PARTY_FEVER_MODIFY_NOT );
	   CASE( EPM_UPDATE_PARTY_USER_JOIN_ROOM_INFO_NOT );
	   CASE_NOPARAM( EPM_END_GAME_REGROUP_PARTY_NOT );

	   _CASE( EPM_UPDATE_GAME_PLAY_STATUS_NOT, KPartyMemberStatus );
#endif SERV_BATTLE_FIELD_SYSTEM
	   //}}
	   //{{ 2012. 05. 30	������		���� ���� ���� ����
#ifdef SERV_OFFCIAL_PVP_BUG_FIX
	   CASE( EPM_UPDATE_PVP_INFO_NOT );
#endif SERV_OFFCIAL_PVP_BUG_FIX
	   //}}

	default:
		START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
	}
}

IMPL_ON_FUNC( EPM_CREATE_PARTY_BY_INVITE_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_INIT ), EPM_CREATE_PARTY_BY_INVITE_ACK, KEPM_CREATE_PARTY_BY_INVITE_ACK );

	KEPM_OPEN_PARTY_LIST_NOT kPacketNot;
	kPacket.m_iOK = NetError::NET_OK;	

	// ���ο� ��Ƽ ���� �� ��Ƽ�� ����
	if( m_kPartyUserManager.Create( kPacket_.m_kHostPartyUserInfo ) == false )
	{
		START_LOG( cerr, L"������ ��Ƽ�� ���� �������.!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_kHostPartyUserInfo.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_kHostPartyUserInfo.m_wstrNickName )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_00;
		goto end_proc;
	}

	// ���������� ��Ƽ�� �������ְ�
	SetInfo( kPacket_.m_kCreatePartyInfo );

	// ������ �α�
	if( kPacket_.m_cAuthLevel >= SEnum::UAL_DEVELOPER )
	{
		START_LOG( cout, L"������ ��Ƽ ���� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_kHostPartyUserInfo.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_kHostPartyUserInfo.m_wstrNickName );
	}

	// ��Ƽ����Ʈ�� ������ ��Ƽ �߰�
	GetInfo( kPacketNot.m_kPartyInfo );
	m_kPartyUserManager.GetUserInfoList( kPacketNot.m_vecUserInfo );
	BroadCastGameServer( EPM_OPEN_PARTY_LIST_NOT, kPacketNot );


	//@@ �渮��Ʈ �߰�
	//## 1. �����ϰ��
	//## 2. �ִ��ο��� 2�� �̻��� ���.
	//## 3. �������� ���°� �ƴѰ��.
	//{{ 2011. 07. 04	������	���� ����
	if( IsPublic() == true  &&
		m_kPartyUserManager.GetMaxMember() >= 2  &&
		IsPlaying() == false )
	{
		AddPartyInfoInPartyList();
	}

	//�ʵ����� ������Ʈ.
	m_kPartyUserManager.UpdatePartyMemberListToFieldUser( GetUID() );

	//����� ��ܿ� �ְ�..
	if( m_kPartyUserManager.JoinCandidate( kPacket_.m_kReceiverPartyUserInfo, true, kPacket_.m_kHostPartyUserInfo.m_iUnitUID ) == false )
	{
		START_LOG( cerr, L"��Ƽ ����� ����Ʈ ��� ����.!" )
			<< BUILD_LOG( kPacket_.m_kHostPartyUserInfo.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_kHostPartyUserInfo.m_wstrNickName )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_07;
		goto end_proc;
	}

	// ��Ƽ ������ ���� �⺻ �۾��� �������� ���� �̵��� ����!
	StateTransition( KCnPartyFSM::I_TO_MAKING_PARTY );

	//��Ƽ�忡�� �ʴ����̶�� �˸���.
	kPacket.m_iOK = NetError::NET_OK;
	GetInfo( kPacket.m_kPartyInfo );
	kPacket.m_kHostPartyUserInfo = kPacket_.m_kHostPartyUserInfo;
	kPacket.m_kReceiverPartyUserInfo = kPacket_.m_kReceiverPartyUserInfo;

	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	kPacket.m_kHostPartyUserInfo.m_wstrIP = NetCommon::GetLocalIPW();
	kPacket.m_kHostPartyUserInfo.m_usPort = KNetLayer::GetKObj()->GetNCUDPPort();
#endif SERV_CHOOSE_FASTEST_HOST
	//}

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CREATE_PARTY_BY_INVITE_ACK, kPacket );

	{
		//�ʴ��������� not�� �˷��ش�.
		KEGS_INVITE_PARTY_NOT kNot;
		kNot.m_iPartyUID = GetUID();
		kNot.m_iHostUnitUID = kPacket_.m_kHostPartyUserInfo.m_iUnitUID;
		kNot.m_wstrHostNickName = kPacket_.m_kHostPartyUserInfo.m_wstrNickName;
		//{{ 2011. 07. 08	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		kNot.m_cPartyType = GetPartyType();
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
		kNot.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
		kNot.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
#endif SERV_CHOOSE_FASTEST_HOST
		//}

		SendToGSCharacter( kPacket_.m_kReceiverPartyUserInfo.m_iGSUID, kPacket_.m_kReceiverPartyUserInfo.m_iUnitUID, EPM_INVITE_PARTY_NOT, kNot );
	}

	//////////////////////////////////////////////////////////////////////////	
	//////////////////////////////////////////////////////////////////////////

	return;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CREATE_PARTY_BY_INVITE_ACK, kPacket );

	// ��Ƽ ���� ����! ��Ƽ�� �ݽ��ϴ�.
	CloseParty( NetError::NOT_CLOSE_PARTY_REASON_00 );
}

IMPL_ON_FUNC( EPM_INVITE_PARTY_REQ )
{
	// ���� ��Ƽ ���� �ʴ밡 �Ϸ���� ���� �����̹Ƿ� �߰� �ʴ�� �Ұ����մϴ�.
	if( GetStateID() == KCnPartyFSM::S_MAKING_PARTY )
	{
        KEGS_INVITE_PARTY_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_PARTY_07;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );
		return;
	}

	// WAIT���¿����� �߰��ʴ밡 �����մϴ�.
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_INVITE_PARTY_ACK, KEGS_INVITE_PARTY_ACK );

	// �ʴ���� ������ UnitUID����
	kPacket.m_iReceiverUID = kPacket_.m_kPartyUserInfo.m_iUnitUID;

	// ��Ƽ�常 �ʴ� �����ϵ��� ����
	if( m_kPartyUserManager.GetHostUID() != FIRST_SENDER_UID )
	{
		START_LOG( clog, L"��Ƽ�常 �ʴ� �����մϴ�." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );
		return;
	}

	// ��Ƽ �Ҽ����� üũ
	KCnPartyUserPtr spHost = m_kPartyUserManager.GetPartyUser( FIRST_SENDER_UID );
	if( spHost == NULL )
	{
		START_LOG( cerr, L"��Ƽ�� ���� ������ ����." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_09;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );
		return;
	}

	//�̹� ��Ƽ�Ҽ��̰ų� ����� �ų�
	if( m_kPartyUserManager.IsMemberOrCandidate( kPacket_.m_kPartyUserInfo.m_iUnitUID ) == true )
	{
		START_LOG( cwarn, L"�̹� �ش���Ƽ�� �ҼӵǾ� ����." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_kPartyUserInfo.m_wstrNickName )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_12;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );
		return;
	}

	//����������..
	if( IsPlaying() == true )
	{
		START_LOG( cerr, L"��Ƽ�� �̹� ������." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( m_kPartyUserManager.GetNumTotal() )
			<< END_LOG;

		//��Ƽ���� ��û�ϴ� ���̹Ƿ� �������̸� �ʴ� �� �� ���� �����̴�
		kPacket.m_iOK = NetError::ERR_PARTY_07;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );
		return;
	}

	//�� �ڸ��� �մ���..
	if( m_kPartyUserManager.GetNumTotal() >= m_kPartyUserManager.GetMaxMember() )
	{
		START_LOG( clog, L"��Ƽ�� �� �ڸ��� ����.!" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( m_kPartyUserManager.GetNumTotal() )
			<< BUILD_LOG( m_kPartyUserManager.GetMaxMember() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_15;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );
		return;
	}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( GetDungeonMode() == CXSLDungeon::DM_HENIR_CHALLENGE )
	{
		// �����Ϸ��� ������ ��Ƽ������� ��� ������ ���������� �˻�
		if( m_kPartyUserManager.IsAllPartyMembersSameServerGroup( kPacket_.m_kPartyUserInfo.m_iUnitUID ) == false )
		{
			kPacket.m_iOK = NetError::ERR_DIF_SERVER_GROUP_05;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );
			return;
		}
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	//����� ��ܿ� �ְ�..
	if( m_kPartyUserManager.JoinCandidate( kPacket_.m_kPartyUserInfo, true, FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"��Ƽ ����� ����Ʈ ��� ����.!" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( spHost->GetCharUID() )
#else
			<< BUILD_LOG( spHost->GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_07;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );
		return;
	}

	//��Ƽ�忡�� �ʴ����̶�� �˸���.
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );

	//�ʴ��������� not�� �˷��ش�.
	KEGS_INVITE_PARTY_NOT kNot;
	kNot.m_iPartyUID = GetUID();
	kNot.m_iHostUnitUID = spHost->GetCharUID();
	kNot.m_wstrHostNickName = spHost->GetCharName();
	//{{ 2011. 07. 08	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	kNot.m_cPartyType = GetPartyType();
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	kNot.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
	kNot.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
#endif SERV_CHOOSE_FASTEST_HOST
	//}

	SendToGSCharacter( kPacket_.m_kPartyUserInfo.m_iGSUID, kPacket_.m_kPartyUserInfo.m_iUnitUID, EPM_INVITE_PARTY_NOT, kNot );
}

IMPL_ON_FUNC( EPM_INVITE_PARTY_REPLY_NOT )
{
	VERIFY_PARTY_STATE_WARN( ( 2, KCnPartyFSM::S_MAKING_PARTY, KCnPartyFSM::S_WAIT ) );

	//����ڿ� �ִ��� �˻��Ѵ�.
	if( m_kPartyUserManager.IsCandidate( kPacket_.m_kPartyUserInfo.m_iUnitUID ) == false )
	{
		if( kPacket_.m_cReplyReason == KEGS_INVITE_PARTY_REPLY_NOT::RR_ACCEPT )
		{
			KEGS_JOIN_PARTY_NOT kJoinNot;
			kJoinNot.m_iOK = NetError::ERR_PARTY_18;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_JOIN_PARTY_NOT, kJoinNot );
		}

		//�ʰ� ������ �Դ�! �ź��Ѱ����� ó��!
		goto end_proc;
	}

	//�ʴ���� ��Ƽ �ʴ� �źθ� �������.
	if( kPacket_.m_cReplyReason != KEGS_INVITE_PARTY_REPLY_NOT::RR_ACCEPT )
	{
		// �ӽ� ��� ��Ż ó��!
		LIF( LeaveCandidateMember( FIRST_SENDER_UID ) );

		// ��Ƽ�忡�Դ� �ʴ밡 �źεǾ����� �˸���.
		KCnPartyUserPtr spHost = m_kPartyUserManager.GetHostPtr();
		if( spHost != NULL )
		{
			KEGS_INVITE_PARTY_DENY_NOT kDenyNot;			
			kDenyNot.m_iDenyReasonNetError = ( kPacket_.m_cReplyReason == KEGS_INVITE_PARTY_REPLY_NOT::RR_BUSY ? NetError::ERR_PARTY_42 : NetError::ERR_PARTY_19 );
			kDenyNot.m_iUnitUID = kPacket_.m_kPartyUserInfo.m_iUnitUID;
			spHost->SendToGSCharacter( EPM_INVITE_PARTY_DENY_NOT, kDenyNot );
		}
		else
		{
			//��Ƽ�忡�� ���� ���θ� �˷��ִ°��ε� �����Ƿ� ��Ŷ�� ������ �ʴ´�.
			START_LOG( cwarn, L"�ʴ������ �˷��� ��Ƽ���� ����.!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( kPacket_.m_kPartyUserInfo.m_iUnitUID )
				<< END_LOG;
		}

		goto end_proc;
	}
	
	//��ȸ������ ����� ��Ų��.
	if( m_kPartyUserManager.JoinMember( kPacket_.m_kPartyUserInfo ) == false )
	{
		START_LOG( cerr, L"��Ƽ ������� ����.!" )
			<< BUILD_LOG( kPacket_.m_kPartyUserInfo.m_wstrNickName )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		//����ڿ��� ���� ��Ŷ�� ������.
		KEGS_JOIN_PARTY_NOT kJoinNot;
		kJoinNot.m_iOK = NetError::ERR_PARTY_18;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_JOIN_PARTY_NOT, kJoinNot );

		//���� ������ ���
		goto end_proc;
	}

	//////////////////////////////////////////////////////////////////////////
	// ó�� ���� �Դϴ�!
	if( GetStateID() == KCnPartyFSM::S_MAKING_PARTY )
	{
		KCnPartyUserPtr spHost = m_kPartyUserManager.GetHostPtr();
		if( spHost == NULL )
		{
			START_LOG( cerr, L"��Ƽ���� �������� �ʴ´�? ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
		}
		else
		{
			// ��Ƽ�忡�Ե� ������!
			KEGS_JOIN_PARTY_NOT kJoinNot;
			GetInfo( kJoinNot.m_vecPartyInfo );
			m_kPartyUserManager.GetUserInfoList( kJoinNot.m_vecPartyUserInfo );
			m_kPartyUserManager.GetUnitUIDList( kJoinNot.m_vecCurPartyUnitUIDList );
			kJoinNot.m_iOK = NetError::NET_OK;
			//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
			kJoinNot.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
			kJoinNot.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
#endif SERV_CHOOSE_FASTEST_HOST
			//}

			spHost->SendToGSCharacter( EPM_JOIN_PARTY_NOT, kJoinNot );

			// ��Ƽ�����Ե� �Ȱ����� �����ô�!
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_JOIN_PARTY_NOT, kJoinNot );
		}

		// ��Ƽ ���� ���������� Wait���·� �̵�����!
		StateTransition( KCnPartyFSM::I_TO_WAIT );
	}
	else
	{
		{
			//���� ��Ƽ�� �����ϴ� �������� ������ �����ְ�.
			KEGS_JOIN_PARTY_NOT kJoinNot;
			//## 1. ���� ��Ƽ�ο����� ����
			m_kPartyUserManager.GetUserInfoList( kJoinNot.m_vecPartyUserInfo );
			m_kPartyUserManager.GetUnitUIDList( kJoinNot.m_vecCurPartyUnitUIDList );
			//## 2. ��Ƽ����
			GetInfo( kJoinNot.m_vecPartyInfo );
			//## 3. �˷��ֱ�.
			kJoinNot.m_iOK = NetError::NET_OK;
			//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
			kJoinNot.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
			kJoinNot.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
#endif SERV_CHOOSE_FASTEST_HOST
			//}

			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_JOIN_PARTY_NOT, kJoinNot );
		}
		{
			//���� ��Ƽ���鿡�� ���ε��� ��Ƽ�� ������ �����ش�.
			//## 1. ���ε��� ������ ������ ���
			KEGS_JOIN_PARTY_NOT kJoinNot;
			GetInfo( kJoinNot.m_vecPartyInfo );
			KPartyUserInfo kPartyUserInfo;
			m_kPartyUserManager.GetUserInfo( kPacket_.m_kPartyUserInfo.m_iUnitUID, kPartyUserInfo );
			m_kPartyUserManager.GetUnitUIDList( kJoinNot.m_vecCurPartyUnitUIDList );
			kJoinNot.m_vecPartyUserInfo.push_back( kPartyUserInfo );
			//## 2. ���� �����鿡�� �˷���
			kJoinNot.m_iOK = NetError::NET_OK;
			//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
			kJoinNot.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
			kJoinNot.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
#endif SERV_CHOOSE_FASTEST_HOST
			//}

			BroadCast( EPM_JOIN_PARTY_NOT, kJoinNot, kPacket_.m_kPartyUserInfo.m_iUnitUID );
		}
	}

	{
		// ��Ƽ ����Ʈ�� ��Ƽ ���� ����
		UpdatePartyInfoInPartyList();

		// ��Ƽ ����Ʈ�� ������ ��Ƽ�� �߰�
		KPartyUserInfo kPartyUserInfo;
		m_kPartyUserManager.GetUserInfo( kPacket_.m_kPartyUserInfo.m_iUnitUID, kPartyUserInfo );
		AddPartyMemberInPartyList( kPartyUserInfo );

		//�ʵ����� ������Ʈ.
		m_kPartyUserManager.UpdatePartyMemberListToFieldUser( GetUID() );
	}
	
	return;

end_proc:
	// ù��° ��Ƽ �ʴ��� ���� ��Ƽ�� ��������.
	if( GetStateID() == KCnPartyFSM::S_MAKING_PARTY )
	{
		CloseParty( NetError::NOT_CLOSE_PARTY_REASON_00 );
	}
}

IMPL_ON_FUNC( EPM_UPDATE_PARTY_USER_INFO_NOT )
{
	VERIFY_PARTY_STATE( ( 1, KCnPartyFSM::S_WAIT ) );

	// ��Ƽ ���� ���� ����
	KCnPartyUserPtr spUser = m_kPartyUserManager.GetPartyUser( FIRST_SENDER_UID );
	if( spUser == NULL )
	{
		START_LOG( cerr, L"��Ƽ ���� ������ ������ ������ �����ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	// ������Ʈ
	spUser->SetUnitClass( kPacket_.m_cUnitClass );
	spUser->SetLevel( kPacket_.m_ucLevel );
	spUser->SetState( kPacket_.m_cState );
	spUser->SetStateCode( kPacket_.m_iStateCode );


	// ��Ƽ ����鿡�� ����� ���� ����
	{
		KEGS_UPDATE_PARTY_USER_INFO_NOT kNot;
		kNot.m_iUnitUID		= kPacket_.m_iUnitUID;
		kNot.m_cUnitClass	= kPacket_.m_cUnitClass;
		kNot.m_ucLevel		= kPacket_.m_ucLevel;
		kNot.m_cState		= kPacket_.m_cState;
		kNot.m_iStateCode	= kPacket_.m_iStateCode;

		BroadCast( EPM_UPDATE_PARTY_USER_INFO_BROAD_NOT, kNot );
	}

	// ��� ���Ӽ����� ��Ƽ ����Ʈ�� ����� ���� ����
	{
		KEPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT kNot;
		kNot.m_iPartyUID = GetUID();
		kNot.m_vecUpdatePartyUser.push_back( spUser->GetPartyUserInfo() );
		BroadCastGameServer( EPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT, kNot );
	}

	if( kPacket_.m_bChangeField == true  &&  kPacket_.m_iUnitUID != m_kPartyUserManager.GetHostUID() )
	{
		if( m_kPartyUserManager.ChangeReady( kPacket_.m_iUnitUID, false ) == false )
		{
			START_LOG( cerr, L"�ʵ� �̵��� ���� ���� ����." )
				<< BUILD_LOG( kPacket_.m_iPartyUID )
				<< BUILD_LOG( m_kPartyUserManager.GetNumTotal() )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< END_LOG;
		}

		// ���� ���и� �Ұ��� ������ ���� Ǯ����.
		KEGS_PARTY_CHANGE_READY_NOT kNot;
		kNot.m_UnitUID	= kPacket_.m_iUnitUID;
		kNot.m_bReady	= false;
		BroadCast( EPM_PARTY_CHANGE_READY_NOT, kNot );
	}
}

IMPL_ON_FUNC( EPM_LEAVE_PARTY_REQ )
{
	//{{ 2012. 02. 21	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	switch( GetStateID() )
	{
	case KCnPartyFSM::S_INIT:
		{
			// �ð����� �߻��Ҽ� �ִ� �����̹Ƿ� �����α� ������ cwarn���� �����.
			START_LOG( cwarn, L"�̹� �����ִ� ��Ƽ�ε� ��Ż ��û�� �Դ�! ������ �ð����� �߻��� �� �ִ� ������." )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
			return;
		}
		break;

	case KCnPartyFSM::S_MAKING_PARTY:
	case KCnPartyFSM::S_WAIT:
	case KCnPartyFSM::S_CHECK_PLAY_CONDITION:
	case KCnPartyFSM::S_CHECK_AUTO_PARTY:
	case KCnPartyFSM::S_ACCEPT_FOR_PLAY:
	case KCnPartyFSM::S_PREPARE_FOR_PLAY:
		break;

	default:
		{
			START_LOG( cerr, L"���� ����. Name : " << m_strName )
				<< L"    ���� ���� : " << GetStateIDString() << dbg::endl
				<< END_LOG;

			KEPM_LEAVE_PARTY_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_WRONG_STATE_00;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_LEAVE_PARTY_ACK, kPacket );
			return;
		}
		break;
	}
#else
	VERIFY_PARTY_STATE_ACK( ( 4, KCnPartyFSM::S_MAKING_PARTY, KCnPartyFSM::S_WAIT, KCnPartyFSM::S_ACCEPT_FOR_PLAY, KCnPartyFSM::S_PREPARE_FOR_PLAY ), EPM_LEAVE_PARTY_ACK, KEPM_LEAVE_PARTY_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	KEPM_LEAVE_PARTY_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	//{{ 2011. 02. 21  ��μ�	Ư�� ���� ���� ������ ���� ����
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
	kPacket.m_iReason = kPacket_.m_iReason;
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
	//}}
	kPacket.m_iPartyUID = kPacket_.m_iPartyUID;
	//{{ 2011. 11. 28	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
	kPacket.m_iNewJoinPartyUID = kPacket_.m_iNewJoinPartyUID;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	const UidType iLeavePartyUnitUID = FIRST_SENDER_UID;

	// ��Ż ���� ����	
	KPartyUserInfo kLeaveUserInfo;

	// ��Ƽ �Ҽ��̶��..
	if( m_kPartyUserManager.IsMember( iLeavePartyUnitUID ) == true )
	{
		// ��Ż�� ���� ������ ã�´�.
		if( m_kPartyUserManager.GetUserInfo( iLeavePartyUnitUID, kLeaveUserInfo ) == false )
		{
			START_LOG( cerr, L"��Ƽ��Ż�� ���� ������ �����ϴ�." )
				<< BUILD_LOG( kPacket_.m_iPartyUID )
				<< BUILD_LOG( iLeavePartyUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PARTY_01;
			goto end_proc;
		}
	}
	// ����� ���..
	else if( m_kPartyUserManager.IsCandidate( iLeavePartyUnitUID ) == true )
	{
		// ����ڿ��� ���� ������!
		LIF( LeaveCandidateMember( iLeavePartyUnitUID ) );

		//����ڿ��� �����ߴٰ� �˷��ְ� ����..
		goto end_proc;
	}
	else
	{
		// ����� �����µ� ���ð������� ����ڿ��� �ڵ������� ��� �߻��� �� �ִ�.
		START_LOG( cwarn, L"��Ƽ��Ż�� ���� ������ �����ϴ�." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( iLeavePartyUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_01;
		goto end_proc;
	}

	// ��Ƽ ��Ż ó��
	if( LeaveMember( iLeavePartyUnitUID
				#ifdef LOG_PARTY_BREAK
					, kPacket.m_iReason
				#endif // LOG_PARTY_BREAK
					) == false )
	{
		//��������.
		kPacket.m_iOK = NetError::GetLastError();
		goto end_proc;
	}

end_proc:

	//{{ 2011. 02. 21  ��μ�	Ư�� ���� ���� ������ ���� ����
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
	if( kPacket_.m_iReason != NetError::NOT_LEAVE_PARTY_REASON_05 )//&& kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_22)
#else
	if( kPacket_.m_iReason != NetError::NOT_LEAVE_PARTY_REASON_05 )
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
		//}}
	{
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_LEAVE_PARTY_ACK, kPacket );
	}

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	if( m_kPartyUserManager.GetComeBackUserInParty() == true )			// ��Ƽ ������ �־��µ�
	{
		if( m_kPartyUserManager.CheckComeBackUserInParty() == false )	// ���� ���� ó���ϰ� ���ϱ� ����.
		{
			CTime tCurr = CTime::GetCurrentTime();

			// ���������� ������ ������ ����.
			KEPM_CHANGE_COME_BACK_BUFF_NOT kNot;
			kNot.m_bIsBuffFinish = true;
			kNot.m_wstrComeBackBuffEnd = tCurr.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
			BroadCast( EPM_CHANGE_COME_BACK_BUFF_NOT, kNot );
		}
	}
#endif SERV_COME_BACK_USER_REWARD
	//}}

	// ��Ƽ�� ������ �� ���� ��ŭ ��Ƽ���� ����� �ִ°�?
	if( m_kPartyUserManager.GetNumMember() <= PE_CLOSE_PARTY_NUM_MEMBER )
	{
		CloseParty( NetError::NOT_CLOSE_PARTY_REASON_01 );
	}
	else
	{
		switch( GetStateID() )
		{
		case KCnPartyFSM::S_MAKING_PARTY:
		case KCnPartyFSM::S_WAIT:
			break;

		case KCnPartyFSM::S_CHECK_PLAY_CONDITION:
			{
				// ���� �����鸸�̶� �÷��� ���� üũ ����!
                CheckAllMembersPlayCondition();
			}
			break;

		case KCnPartyFSM::S_CHECK_AUTO_PARTY:
			{
				// ���� �����鸸�̶� �ڵ� ��Ƽ ���� üũ ����!
				CheckAllMembersAutoParty();
			}
			break;

		case KCnPartyFSM::S_ACCEPT_FOR_PLAY:
			{
				// ���� �����鸸�̶� ���� ���� ���� üũ ����!
				CheckAllMembersAcceptForPlay();
			}
			break;

		case KCnPartyFSM::S_PREPARE_FOR_PLAY:
			{
				// ��Ż�� ��Ƽ���� ������ �ٸ� ��Ƽ���� ��� ���� ������ �غ� �ٵǾ��ִٸ�?
				if( m_kPartyUserManager.IsAllCheckedGameStart() )
				{
					// ��Ƽ�� ã��
					KCnPartyUserPtr spHost = m_kPartyUserManager.GetHostPtr();
					if( spHost == NULL )
					{
						START_LOG( cerr, L"��Ƽ�� ã�� ����!" )
							<< BUILD_LOG( GetUID() )
							<< END_LOG;
						return;
					}

					// ��Ƽ�忡�� ���� ��Ŷ ����
					KEGS_PARTY_GAME_START_ACK kAck;
					kAck.m_iOK = NetError::ERR_PARTY_23;
					spHost->SendToGSCharacter( EPM_PARTY_GAME_START_ACK, kAck );
				}
			}
			break;
		}		
	}
}


IMPL_ON_FUNC( EPM_PARTY_CHANGE_READY_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PARTY_CHANGE_READY_ACK, KEGS_PARTY_CHANGE_READY_ACK );

	if( FIRST_SENDER_UID == m_kPartyUserManager.GetHostUID() )
	{
		// [����] ��Ƽ������ �ǰ����� ready������ �õ����� ��� �߻� ������ ����

		START_LOG( cwarn, L"��Ƽ���� ���𺯰��� ����ȵȴ�." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )			
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_32;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_READY_ACK, kPacket );
		return;
	}

	if( m_kPartyUserManager.ChangeReady( FIRST_SENDER_UID, kPacket_.m_bReady ) == false )
	{
		START_LOG( cerr, L"��Ƽ���� ���� ���� ����." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_bReady )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_21;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_READY_ACK, kPacket );
		return;
	}

	//���º��� �ϷḦ ����ڿ��� �˸���
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_READY_ACK, kPacket );

	// ��Ƽ���鿡�� ���𺯰��� �˷��ش�.
	KEGS_PARTY_CHANGE_READY_NOT kNot;
	kNot.m_UnitUID	= FIRST_SENDER_UID;
	kNot.m_bReady	= kPacket_.m_bReady;
	BroadCast( EPM_PARTY_CHANGE_READY_NOT, kNot );
}

IMPL_ON_FUNC( EPM_PARTY_CHANGE_DUNGEON_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PARTY_CHANGE_DUNGEON_ACK, KEGS_PARTY_CHANGE_DUNGEON_ACK );

	if( m_kPartyUserManager.IsMember( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"��Ƽ ����� �ƴմϴ�." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_03;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_DUNGEON_ACK, kPacket );
		return;
	}

	const UidType iHostUID = m_kPartyUserManager.GetHostUID();
	if( FIRST_SENDER_UID != iHostUID )
	{
		START_LOG( cerr, L"��Ƽ���� �ƴѵ� �������� ������ ��û��." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( iHostUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_DUNGEON_ACK, kPacket );
		return;
	}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( kPacket_.m_cDungeonMode == CXSLDungeon::DM_HENIR_CHALLENGE )
	{
		// ��ϸ� �ð� ���� ���� ���� �������� ���������� �÷��� �� �� �ִ�!
		if( m_kPartyUserManager.IsAllPartyMembersSameServerGroup( iHostUID ) == false )
		{
			kPacket.m_iOK = NetError::ERR_DIF_SERVER_GROUP_05;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_DUNGEON_ACK, kPacket );
			return;
		}
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	// Ŭ���̾�Ʈ�� ��� ����
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_DUNGEON_ACK, kPacket );

	//## 1. ���������� ���
	//## 2. �ִ��ο��� 2�� �̻��� ���
	//## 3. ������ ���°� �ƴ� ���
	if( IsPublic() == true  &&
		m_kPartyUserManager.GetMaxMember() >= 2  &&
		IsPlaying() == false )
	{
		// ��Ƽ ����Ʈ���� ���� �����.
		DelPartyInfoInPartyList();
	}	

	//////////////////////////////////////////////////////////////////////////	
	// �ٲ� ���� ���� ����	
	SetDungeonID( kPacket_.m_iDungeonID );
	SetDungeonDif( kPacket_.m_DifficultyLevel );
	SetDungeonMode( static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_cDungeonMode) );

	//�ٲ���� ������ �˷��ش�.
	KEGS_PARTY_CHANGE_DUNGEON_NOT kNot;
	kNot.m_iDungeonID		= kPacket_.m_iDungeonID;
	kNot.m_DifficultyLevel	= kPacket_.m_DifficultyLevel;
	kNot.m_cDungeonMode		= kPacket_.m_cDungeonMode;
	BroadCast( EPM_PARTY_CHANGE_DUNGEON_NOT, kNot );

	//���������� �ٲ�����Ƿ� ��� ��Ƽ���� ���ڻ��¸� Ǯ���ش�.
	//ChangeAllReady() �Լ��ȿ��� ���� ���¸� �ٲ��� ��Ŷ�� �����ش�.
	m_kPartyUserManager.ChangeAllReady( false );

	//@@ ��Ƽ����Ʈ ������Ʈ.
	UpdatePartyInfoInPartyList();

	//## 1. ���������� ���
	//## 2. �ִ��ο��� 2�� �̻��� ���
	//## 3. ������ ���°� �ƴ� ���
	if( IsPublic() == true  &&
		m_kPartyUserManager.GetMaxMember() >= 2  &&
		IsPlaying() == false )
	{
		// �ٽ� �߰��Ѵ�.
		AddPartyInfoInPartyList();
	}
}

IMPL_ON_FUNC( EPM_PARTY_GAME_START_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PARTY_GAME_START_ACK, KEPM_PARTY_GAME_START_ACK );
	
	// ��Ƽ������ �˻�
	const UidType iHostUID = m_kPartyUserManager.GetHostUID();
	if( FIRST_SENDER_UID != iHostUID )
	{
		START_LOG( cerr, L"��Ƽ���� �ƴѵ� �������� ������ ����." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( iHostUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_GAME_START_ACK, kPacket );
		return;
	}

	// ��Ƽ���� �����ؼ� ��Ƽ������ 1����ΰ�� ���ӽ����� �Ҽ� �����ϴ�!
	if( m_kPartyUserManager.GetNumMember() <= PE_CLOSE_PARTY_NUM_MEMBER )
	{
		START_LOG( cwarn, L"���� ��Ƽ������ �����Ͽ� ��Ƽ ���� ������ �� �� �����ϴ�!" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( iHostUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_09;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_GAME_START_ACK, kPacket );
		return;
	}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	if( GetDungeonMode() == CXSLDungeon::DM_HENIR_CHALLENGE )
	{
		// ��ϸ� �ð� ���� ���� ���� �������� ���������� �÷��� �� �� �ִ�!
		if( m_kPartyUserManager.IsAllPartyMembersSameServerGroup( iHostUID ) == false )
		{
			START_LOG( cerr, L"��ϸ� �ð� ���� ���� ���� ���������������� �÷��� ����! �̹� üũ ���� �ٵ�.." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( iHostUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_DIF_SERVER_GROUP_05;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_GAME_START_ACK, kPacket );
			return;
		}
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	// ��Ƽ ���� ���ۿ� �ʿ��� ������ �ӽ÷� �����Ѵ�.
	m_kGameManager.SetGameStartInfo( kPacket_.m_usEventID, kPacket_.m_sWorldID );

	// üũ Ÿ�̸� �ʱ�ȭ
	m_tTimer[TE_CHECK_PLAY_CONDITION].restart();

	// �� ��Ƽ������ ���� ���� ���� ���� ������ üũ�Ѵ�!
	StateTransition( KCnPartyFSM::I_TO_CHECK_PLAY_CONDITION );

	// �� ��Ƽ���鿡�� üũ ��Ŷ ����
	KEPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_REQ kPacketReq;
	GetDungeonGameInfo( kPacketReq.m_kDungeonGameInfo );
	BroadCast( EPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_REQ, kPacketReq );
}

//{{ 2012. 01. 12	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
IMPL_ON_FUNC( EPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_ACK )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_CHECK_PLAY_CONDITION ) );

	KEPM_PARTY_GAME_START_ACK kPacket;

	// �ش� ��Ƽ���� �÷��� ���� ������ ���ϴ� ���!
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// ���� ó�� ����!
		FailProcessPlayCondition( kPacket_.m_iOK, FIRST_SENDER_UID, kPacket_.m_wstrFailUserNickName );
		return;
	}

	// ���� üũ�� ������ ������ üũ ��� ����!
	if( m_kPartyUserManager.SetCheckedPlayCondition( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"���� üũ�� ����Ϸ��� ������ ��Ƽ���� �ƴմϴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetName() )
			<< END_LOG;
		return;
	}

	// ��� ����� üũ�� �Ϸ�Ǿ����� Ȯ��!
	CheckAllMembersPlayCondition();
}

_IMPL_ON_FUNC( EPM_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT, KEGS_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT )
{
	VERIFY_PARTY_STATE( ( 1, KCnPartyFSM::S_ACCEPT_FOR_PLAY ) );

	// �Ѹ��� ��Ƽ���̶� �������� �ʴ´ٸ� ������ �������� �ʽ��ϴ�.
	if( kPacket_.m_bAccept == false )
	{
		FailProcessAcceptForPlay( NetError::NOT_PARTY_GAME_START_FAILED_00, FIRST_SENDER_UID ); // �������� ���� ����� �ֽ��ϴ�!
        return;
	}

	// �ش� ��Ƽ���� Accept������ �����մϴ�.
	if( m_kPartyUserManager.ChangeReady( FIRST_SENDER_UID, true ) == false )
	{
        START_LOG( cerr, L"�ش� ��Ƽ���� �������� �ʽ��ϴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	// ��� ��Ƽ������ Acceptó���� �Ǿ����� üũ����!
	CheckAllMembersAcceptForPlay();
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

IMPL_ON_FUNC( EPM_CHECK_FOR_PARTY_GAME_START_ACK )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_PREPARE_FOR_PLAY ) );

	KEGS_PARTY_GAME_START_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	// �Ҽ� ��Ƽ������ �˻�
	KCnPartyUserPtr spUser = m_kPartyUserManager.GetPartyUser( FIRST_SENDER_UID );
	if( spUser == NULL )
	{
		START_LOG( cerr, L"���ӽ��� üũ��Ŷ ������ �Դµ� �Ҽ� ��Ƽ���� �ƴϴ�?" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		// ���⼭ ������.
		return;
	}

	// ��Ƽ�忡�� ���� ������ ������.
	KCnPartyUserPtr spHost = m_kPartyUserManager.GetHostPtr();
	if( spHost == NULL )
	{
		START_LOG( cerr, L"��Ƽ�� ������ �����ϴ�. ������ ���� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_kPartyUserManager.GetNumMember() )
			<< END_LOG;
		return;
	}

	// NetError�� OK���� �ƴ϶��..
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"������� �Դµ� ������ �������� �ʴ� ����� �����Ѵ�! �����ΰ� ���ܻ�Ȳ�ΰ�?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_kPartyUserManager.GetNumMember() )
			<< END_LOG;

		// ������ ������ ������ �������� �ʴ� ����� �����Ѵ�. ���� ������ �������!
		FailProcessPrepareForPlay( kPacket_.m_iOK );
		return;
	}

	// ������ PartyRoomUserInfo�� �˻�
	if( kPacket_.m_vecUserInfo.empty() )
	{
		START_LOG( cerr, L"���ӽ��� üũ��Ŷ ������ �Դµ� PartyRoomUserInfo�� ����ִ�?" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		// ������ ������ ������ �������� �ʴ� ����� �����Ѵ�. ���� ������ �������!
		FailProcessPrepareForPlay( kPacket_.m_iOK );
		return;
	}

	KPartyRoomUserInfo kPartyRoomUserInfo;
	BOOST_TEST_FOREACH( const KPartyRoomUserInfo&, kInfo, kPacket_.m_vecUserInfo )
	{
		kPartyRoomUserInfo = kInfo;
		break;
	}

	// ������ PartyRoomUserInfo�� CnPartyUser�� ������Ʈ �Ѵ�.
	spUser->SetPartyRoomUserInfo( kPartyRoomUserInfo );

	// ��� ��Ƽ ������ PartyRoomUserInfo�� �������ִ��� �˻�.
	if( m_kPartyUserManager.IsAllCheckedGameStart() == false )
	{
		// ���⼭ ������ ���� ������ ������ ��Ŷ���� �ٽ� üũ.
		return;
	}

	// ��Ƽ������ PartyRoomUserInfo�� ����
	KERM_PARTY_OPEN_DUNGEON_ROOM_REQ kPacket;
	if( m_kPartyUserManager.GetDungeonRoomUserInfoList( kPacket.m_vecUserInfo ) == false )
	{
		START_LOG( cerr, L"��Ƽ�� ���� �������� �����غ��� �������� ��������.!" )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		// ������ ������ ������ �������� �ʴ� ����� �����Ѵ�. ���� ������ �������!
		FailProcessPrepareForPlay( kPacket_.m_iOK );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��� ������ PartyRoomUserInfo�� �غ�Ǿ��ٸ� ���ӽ���!

	// ���� ������ �غ� �Ǿ����� �����̵�!
	StateTransition( KCnPartyFSM::I_TO_WAIT );

	// �������� ����� �ֵ��� ������ �����Ѵ�.	
	kPacket.m_sWorldID						= kPacket_.m_sWorldID;
	kPacket.m_kRoomInfo.m_RoomType			= CXSLRoom::RT_DUNGEON;
	kPacket.m_kRoomInfo.m_bPublic			= false;
	kPacket.m_kRoomInfo.m_MaxSlot			= static_cast<char>(m_kPartyUserManager.GetMaxMember());
	kPacket.m_kRoomInfo.m_iDungeonID		= GetDungeonID();
	kPacket.m_kRoomInfo.m_DifficultyLevel	= GetDungeonDif();	
	kPacket.m_kRoomInfo.m_cDungeonMode		= static_cast<char>(GetDungeonMode());	
	kPacket.m_kRoomInfo.m_iPartyUID			= GetUID();

	switch( GetItemType() )
	{
	case CXSLRoom::DGIT_PERSON:
	case CXSLRoom::DGIT_RANDOM:
		kPacket.m_kRoomInfo.m_cGetItemType = GetItemType();
		break;
	default:
		//������ �߸��Ǿ������� �׻� ��������.
		kPacket.m_kRoomInfo.m_cGetItemType = CXSLRoom::DGIT_RANDOM;
		break;
	}
	kPacket.m_kRoomInfo.m_fPlayTime = DUNGEON_DEFAULT_LIMIT_PLAY_TIME;

	float fPlayTimeLimit = SiCXSLDungeonManager()->GetPlayTimeLimit( ( kPacket.m_kRoomInfo.m_iDungeonID + kPacket.m_kRoomInfo.m_DifficultyLevel ) );
	if( fPlayTimeLimit > 0.0f )
	{
		kPacket.m_kRoomInfo.m_fPlayTime = fPlayTimeLimit;
	}

	u_short usEventID = 0;
	switch( kPacket_.m_usEventID )
	{
	case ERM_PARTY_OPEN_DUNGEON_ROOM_REQ: usEventID = EPM_PARTY_OPEN_DUNGEON_ROOM_NOT; break;
	//case ERM_PARTY_GAME_REPLAY_START_REQ: usEventID = EPM_PARTY_GAME_REPLAY_START_NOT; break;
	default:
		{
			START_LOG( cerr, L"��Ƽ�� ���� �������� �����غ��� �������� ��������.!" )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			//kAck.m_iOK = NetError::ERR_PARTY_23;
			//spHost->SendToGSCharacter( EPM_PARTY_GAME_START_ACK, kAck );
		}
		break;
	}

	// ��Ƽ�忡�� ������ ��������� ��Ŷ ����
	spHost->SendToGSCharacter( usEventID, kPacket );

	//{{ 2009. 6. 10  ������	������ ��Ÿ�� ����
	AllPartyMembersQuickSlotCoolTimeReset();
	//}}

	//���������� �����ϰ�
	SetPlaying( true );

	//��Ƽ����Ʈ���� ���ش�.
	DelPartyInfoInPartyList();
	//////////////////////////////////////////////////////////////////////////
}

IMPL_ON_FUNC( EPM_END_GAME_NOT )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_WAIT ) );

	KEGS_PARTY_GAME_START_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cwarn, L"��Ƽ���� �ƴѵ� E_EndGame�� ȣ���ߴ�?" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;
	}

	// ������ ����Ǿ����Ƿ� ��Ƽ�������� ready�� Ǯ���ش�.
	m_kPartyUserManager.ChangeAllReady( false );
	SetPlaying( false );
}

IMPL_ON_FUNC( EPM_PARTY_CHANGE_HOST_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PARTY_CHANGE_HOST_ACK, KEGS_PARTY_CHANGE_HOST_ACK );

	//��Ƽ�� Ȯ��.
	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cerr, L"��Ƽ���� �ƴѵ� ��Ƽ�� ������ ��û��." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	//�ٲ� ��Ƽ���� �ִ��� Ȯ��.
	KCnPartyUserPtr spNewHost = m_kPartyUserManager.GetPartyUser( kPacket_.m_iNewHostUnitUID );
	if( spNewHost == NULL )
	{
		// �ð����� �߻��� �� �ִ� ������.
		START_LOG( cwarn, L"���� �Ӹ��� ��Ƽ���� �����ϴ�." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_27;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	if( ( GetDungeonID() + static_cast<int>(GetDungeonDif()) ) <= 0 )
	{
		START_LOG( cerr, L"dungeonid���� �̻��ϴ�!" )
			<< BUILD_LOG( GetDungeonID() )
			<< BUILD_LOGc( GetDungeonDif() )
			<< END_LOG;
	}
	//////////////////////////////////////////////////////////////////////////

	KEPM_CHECK_PARTY_CHANGE_HOST_REQ kPacketReq;
	kPacketReq.m_iPartyUID = kPacket_.m_iPartyUID;
	//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	kPacketReq.m_cPartyType = GetPartyType();
#endif SERV_PVP_NEW_SYSTEM
	//}}
	kPacketReq.m_iOldHostUnitUID = FIRST_SENDER_UID;
	kPacketReq.m_iNewHostUnitUID = kPacket_.m_iNewHostUnitUID;
	kPacketReq.m_iDungeonID		 = GetDungeonID();
	kPacketReq.m_cDifficulty	 = GetDungeonDif();
	kPacketReq.m_cDungeonMode	 = GetDungeonMode();
	//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	kPacketReq.m_bForRegroupParty = false;
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}
	spNewHost->SendToGSCharacter( EPM_CHECK_PARTY_CHANGE_HOST_REQ, kPacketReq );
}

IMPL_ON_FUNC( EPM_CHECK_PARTY_CHANGE_HOST_ACK )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_WAIT ) );

	KEGS_PARTY_CHANGE_HOST_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;	

	//���� ��Ƽ�� ����
	KCnPartyUserPtr spOldHost = m_kPartyUserManager.GetHostPtr();
	if( spOldHost == NULL )
	{
		START_LOG( cerr, L"���� ��Ƽ���� �����ϴ�." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( kPacket_.m_iOldHostUnitUID )
			<< END_LOG;

		// ���⼭ ����.
		return;
	}

	//��Ƽ�� Ȯ��.
	if( kPacket_.m_iOldHostUnitUID != spOldHost->GetCharUID() )
	{
		START_LOG( cerr, L"��Ƽ���� �ƴѵ� ��Ƽ�� ������ ��û��." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iOldHostUnitUID )
			<< BUILD_LOG( spOldHost->GetCharUID() )
			<< END_LOG;

		// ���⼭ ����
		return;
	}

	// OK�� üũ!
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		kAck.m_iOK = kPacket_.m_iOK;
		//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		if( kPacket_.m_bForRegroupParty == false )
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		{
			spOldHost->SendToGSCharacter( EPM_PARTY_CHANGE_HOST_ACK, kAck );
		}
		return;
	}

	//�ٲ� ��Ƽ���� �ִ��� Ȯ��.
	KCnPartyUserPtr spNewHost = m_kPartyUserManager.GetPartyUser( kPacket_.m_iNewHostUnitUID );
	if( spNewHost == NULL )
	{
		START_LOG( cerr, L"���� �Ӹ��� ��Ƽ���� �����ϴ�." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( kPacket_.m_iOldHostUnitUID )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_27;
		goto end_proc;
	}

	//////////////////////////////////////////////////////////////////////////	
	//��Ƽ�� �ٲٱ�

	if( m_kPartyUserManager.ChangeHost( kPacket_.m_iNewHostUnitUID ) == false )
	{
		START_LOG( cerr, L"��Ƽ�� �������." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iOldHostUnitUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_27;
		goto end_proc;
	}

	//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	if( kPacket_.m_bForRegroupParty == false )
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
	{
		spOldHost->SendToGSCharacter( EPM_PARTY_CHANGE_HOST_ACK, kAck );
	}

	// ��� ��Ƽ������ �˸���
	{
		KEGS_PARTY_CHANGE_HOST_NOT kNot;
		kNot.m_iBeforeUID	 = kPacket_.m_iOldHostUnitUID;
		kNot.m_iAfterUID	 = m_kPartyUserManager.GetHostUID();
		//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		kNot.m_bRegroupParty = kPacket_.m_bForRegroupParty;
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		BroadCast( EPM_PARTY_CHANGE_HOST_NOT, kNot );
	}

	//��Ƽ���� �ٲ�� ���̹Ƿ� ��� ������ ������¸� false�� �����Ѵ�.
	m_kPartyUserManager.ChangeAllReady( false );

	// ��� ���Ӽ����� ��Ƽ ����Ʈ�� ����� ���� ����
	{
		KEPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT kPacketNot;
		kPacketNot.m_iPartyUID = GetUID();
		kPacketNot.m_vecUpdatePartyUser.push_back( spOldHost->GetPartyUserInfo() ); // ���� ��Ƽ�� ���� ���
		kPacketNot.m_vecUpdatePartyUser.push_back( spNewHost->GetPartyUserInfo() ); // ���ο� ��Ƽ�� ���� ���
		BroadCastGameServer( EPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT, kPacketNot );
	}

	return;

end_proc:
	//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	if( kPacket_.m_bForRegroupParty == false )
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}
	{
		spOldHost->SendToGSCharacter( EPM_PARTY_CHANGE_HOST_ACK, kAck );
	}
}

IMPL_ON_FUNC( EPM_PARTY_CHANGE_NUM_OF_PER_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PARTY_CHANGE_NUM_OF_PER_ACK, KEGS_PARTY_CHANGE_NUM_OF_PER_ACK );

	//��Ƽ�� Ȯ��.
	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cerr, L"��Ƽ���� �ƴѵ� ��Ƽ�ִ��ο� ������ ��û��." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_NUM_OF_PER_ACK, kPacket );
		return;
	}

	//���� ������ �ִ��ο��� Ʋ������.
	if( m_kPartyUserManager.GetMaxMember() == kPacket_.m_cNumOfPer )
	{
		START_LOG( cwarn, L"���� �ִ뼳���� ���� �������� ��.")
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_28;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_NUM_OF_PER_ACK, kPacket );
		return;
	}

	//���� �ο����ٴ� ���ų� Ŀ���Ѵ�.
	if( m_kPartyUserManager.GetNumTotal() > kPacket_.m_cNumOfPer )
	{
		// [����] �ð����� �߻��Ҽ� �ִ� �����̹Ƿ� cwarn
		START_LOG( cwarn, L"���� �����ϴ� �������� ���� �������� ���ƿ�." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_28;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_NUM_OF_PER_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_NUM_OF_PER_ACK, kPacket );

	//@@ �渮��Ʈ ����
	if( IsPlaying() == false )
	{
		//## 1. ������Ƽ�� �����ο����� 1���϶� ����Ʈ ����
		if( IsPublic() == true && kPacket_.m_cNumOfPer <= 1 )
		{
			DelPartyInfoInPartyList();
		}
		//## 2. ������Ƽ�� �����ο����� 1���� 2���̻����� �ٲܶ� ����Ʈ �߰�.
		else if( IsPublic() == true && m_kPartyUserManager.GetMaxMember() <= 1 && kPacket_.m_cNumOfPer >= 2 )
		{
			AddPartyInfoInPartyList();
		}
	}

	// �� �ִ� �ο� ����
	m_kPartyUserManager.SetMaxMember( kPacket_.m_cNumOfPer );

	// �ٸ� ��Ƽ���鿡�Ե� �˸�
	KEGS_PARTY_CHANGE_NUM_OF_PER_NOT kPacketNot;
	kPacketNot = kPacket_.m_cNumOfPer;
	BroadCast( EPM_PARTY_CHANGE_NUM_OF_PER_NOT, kPacketNot );

	// ��Ƽ ����Ʈ�� �ִ� ��Ƽ ���� ������Ʈ
	UpdatePartyInfoInPartyList();
}

IMPL_ON_FUNC( EPM_PARTY_CHANGE_GET_ITEM_TYPE_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PARTY_CHANGE_GET_ITEM_TYPE_ACK, KEGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK );

	//��Ƽ�� Ȯ��.
	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cerr, L"��Ƽ���� �ƴѵ� �������� �����۸Դ� ��� ������ ��û��." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_GET_ITEM_TYPE_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_GET_ITEM_TYPE_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////	
	// ������ ȹ�� Ÿ�� ����
	SetItemType( kPacket_.m_cGetItemType );

	// �ٸ� ��Ƽ���鿡�� �˸�
	KEGS_PARTY_CHANGE_GET_ITEM_TYPE_NOT kPacketNot;
	kPacketNot = kPacket_.m_cGetItemType;
	BroadCast( EPM_PARTY_CHANGE_GET_ITEM_TYPE_NOT, kPacketNot );
}

IMPL_ON_FUNC( EPM_PARTY_BAN_USER_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PARTY_BAN_USER_ACK, KEGS_PARTY_BAN_USER_ACK );

	kPacket.m_iOK = NetError::NET_OK;

	//��Ƽ�� Ȯ��.
	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cerr, L"��Ƽ���� �ƴѵ� ��Ƽ���� �� ��û��." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_BAN_USER_ACK, kPacket );
		return;
	}

	//������� ������ �ִ��� Ȯ��..
	KCnPartyUserPtr spBanUser = m_kPartyUserManager.GetPartyUser( kPacket_.m_iUnitUID );
	if( spBanUser == NULL )
	{
		// [����] �ð����� ������ ������ �ִ�
		START_LOG( cwarn, L"��Ƽ���� ������ ������ �����ϴ�." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ROOM_17;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_BAN_USER_ACK, kPacket );
		return;
	}

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_BAN_USER_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////	
	// ���� ��Ŷ ����
	KEPM_PARTY_BAN_USER_NOT kPacketNot;
	kPacketNot.m_iPartyUID = GetUID();
	spBanUser->SendToGSCharacter( EPM_PARTY_BAN_USER_NOT, kPacketNot );

	//�����Ŵ.
	if( LeaveMember( kPacket_.m_iUnitUID
			#ifdef LOG_PARTY_BREAK
					, NetError::NOT_LEAVE_PARTY_REASON_07
			#endif // LOG_PARTY_BREAK
					) == false )
	{
		START_LOG( cerr, L"���� ���� ��Ƽ�� ���� ���� ó���� �����Ͽ����ϴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;
	}
	
	// ��Ƽ�� �ƹ��� ����.?
	if( m_kPartyUserManager.GetNumMember() <= PE_CLOSE_PARTY_NUM_MEMBER )
	{
		CloseParty( NetError::NOT_CLOSE_PARTY_REASON_01 );
	}
}

IMPL_ON_FUNC( EPM_CHAT_REQ )
{
	VERIFY_PARTY_STATE_ACK_WARN( ( 3, KCnPartyFSM::S_WAIT, KCnPartyFSM::S_ACCEPT_FOR_PLAY, KCnPartyFSM::S_PREPARE_FOR_PLAY ), EPM_CHAT_ACK, KEGS_CHAT_ACK );

	KEGS_CHAT_NOT kNot;
	kNot.m_SenderUnitUID	= FIRST_SENDER_UID;
	kNot.m_RecieverUnitUID	= kPacket_.m_ToUnitUID;
	kNot.m_cChatPacketType  = kPacket_.m_cChatPacketType;
	kNot.m_cRoomChatType	= kPacket_.m_cRoomChatType;
	kNot.m_wstrMsg			= kPacket_.m_wstrMsg;

	switch( kPacket_.m_cRoomChatType )
	{
	case CXSLRoom::RCT_ALL:
		{
			kPacket.m_iOK = NetError::NET_OK;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHAT_ACK, kPacket );
			BroadCast( EPM_CHAT_NOT, kNot );
			return;
		}
		break;

	case CXSLRoom::RCT_WHISPER:
		{
			KCnPartyUserPtr spWhisper = m_kPartyUserManager.GetPartyUser( kPacket_.m_ToUnitUID );
			if( spWhisper == NULL )
			{
				// �ð����� ����ڰ� ��Ƽ��Ż �� �� �����Ƿ� �߻��� �� �ִ� ������.
				START_LOG( cwarn, L"��Ƽ ä�� ����ڰ� ����." )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( kPacket_.m_ToUnitUID )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_PARTY_33;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHAT_ACK, kPacket );
				return;
			}

			kPacket.m_iOK = NetError::NET_OK;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHAT_ACK, kPacket );

			// �ڱ� �ڽŰ� �ӼӸ� ����ڿ��� ä�� �޽��� ����
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHAT_NOT, kNot );
			spWhisper->SendToGSCharacter( EPM_CHAT_NOT, kNot );
			return;
		}
		break;

	default:
		{
			START_LOG( cerr, L"�̻��� ä�� Ÿ��." )
				<< BUILD_LOGc( kPacket_.m_cRoomChatType )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ROOM_24;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHAT_ACK, kPacket );
		}
		break;
	}
}

IMPL_ON_FUNC( EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ )
{
	VERIFY_PARTY_STATE( ( 1, KCnPartyFSM::S_WAIT ) );

	KEGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK kAck;

	//{{ 2012. 11. 12	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	const float fEnterSecretRate = SiCXSLPartyManager()->GetDungeonSecretStageEnterRate( m_kFeverManager.GetRealFeverPoint(), m_kPartyUserManager.GetNumMember() );
#else
	//// Ȯ���� ���	
	//const int iPartyFeverPoint = m_kFeverManager.GetRealFeverPoint();

	////{{ 2011. 10. 04  ��μ�	   ��ϸ� �뷱�� ��ġ
	////#ifdef SERV_HENIR_BALANCE
	//float fEnterSecretRate = 0.0f;
	//if( m_kPartyUserManager.GetNumMember() > 1 )
	//{
	//	fEnterSecretRate = static_cast<float>( (15 + (iPartyFeverPoint * 0.1)) * ( m_kPartyUserManager.GetNumMember() -1) );
	//}
	//else
	//{
	//	fEnterSecretRate = 0.0f;
	//}
	////#else
	////switch( m_kPartyUserManager.GetNumMember() )
	////{
	////case 1: // ��Ƽ���� 1�����ϸ� ��� �������� ���� �Ұ�!
	////	{
	////		START_LOG( cerr, L"��Ƽ���� 1���̱���. Ŭ�󿡼� �˻������ٵ�?!" )
	////			<< BUILD_LOG( kPacket_.m_iPartyUID )
	////			<< BUILD_LOG( FIRST_SENDER_UID )
	////			<< END_LOG;

	////		kAck.m_bCanEnterSecretStage = false;
	////		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kAck );
	////		return;
	////	}
	////	break;
	////case 2: fEnterSecretRate = static_cast<float>(iPartyFeverPoint) * 0.3f;	break;
	////case 3: fEnterSecretRate = static_cast<float>(iPartyFeverPoint) * 0.6f;	break;
	////case 4: fEnterSecretRate = static_cast<float>(iPartyFeverPoint) * 1.f;	break;
	////default:
	////	{
	////		START_LOG( cerr, L"��Ƽ�ο����� �̻��ϴ�!" )
	////			<< BUILD_LOG( GetNumMember() )
	////			<< END_LOG;

	////		kAck.m_bCanEnterSecretStage = false;
	////		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kAck );
	////		return;
	////	}
	////	break;
	////}
	////#endif SERV_HENIR_BALANCE
	////}}

	//// �����ϰ� ���� ���� ����!
	//if( fEnterSecretRate > 100.f )
	//	fEnterSecretRate = 100.f;	
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	KLottery kLot;
	kLot.AddCase( 1, fEnterSecretRate );
	int iResult = kLot.Decision();
	if( iResult == KLottery::CASE_BLANK )
	{
		// ��� �������� ���� ����!		
		kAck.m_bCanEnterSecretStage = false;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kAck );

		//{{ 2012. 11. 16	������		��ϸ� �ð� ��� �������� ����
#ifdef SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
		SiCXSLPartyManager()->SetHenirSecretStageEnterRateDump( m_kFeverManager.GetRealFeverPoint(), m_kPartyUserManager.GetNumMember(), fEnterSecretRate, false );
#endif SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
		//}}
		return;
	}

	// ���� �����Ǹ� ������ ����üũ�Ϸ� ���ͷ� �̺�Ʈ ����!
	kAck.m_bCanEnterSecretStage = true;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kAck );

	//{{ 2012. 11. 16	������		��ϸ� �ð� ��� �������� ����
#ifdef SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
	SiCXSLPartyManager()->SetHenirSecretStageEnterRateDump( m_kFeverManager.GetRealFeverPoint(), m_kPartyUserManager.GetNumMember(), fEnterSecretRate, true );
#endif SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
	//}}
}

_IMPL_ON_FUNC( EPM_INCREASE_PARTY_FEVER_NOT, KERM_INCREASE_PARTY_FEVER_NOT )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_WAIT ) );

	// Party Fever�� ����
	IncreaseFeverPoint( kPacket_.m_cRoomType, kPacket_.m_sSubStageNpcCount );
}

//{{ 2012. 05. 10	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
_IMPL_ON_FUNC( EPM_MAX_PARTY_FEVER_NOT, KERM_MAX_PARTY_FEVER_NOT )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_WAIT ) );

	// Party Fever�� Max
	UpdateAndBroadCastFeverPoint( KCnPartyFeverManager::FEVER_100_PERCENT_POINT );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

IMPL_ON_FUNC( EPM_REQUEST_UPDATE_PARTY_MEMBER_LIST_NOT )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_WAIT ) );

	// ��Ƽ ��� ����Ʈ�� �ʵ忡 ������Ʈ�Ϸ�����!
	KEPM_SEND_UPDATE_PARTY_MEMBER_LIST_NOT kPacket;
	kPacket.m_iPartyUID = GetUID();
	m_kPartyUserManager.GetUnitUIDList( kPacket.m_setPartyUIDList );
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_SEND_UPDATE_PARTY_MEMBER_LIST_NOT, kPacket );
}

_IMPL_ON_FUNC( EPM_CHECK_INVALID_USER_NOT, UidType )
{
	const UidType iDisconnectedGSUID = kPacket_;

	// ������ ���� ����Ʈ ���
	std::vector< UidType > vecDeleteUserList;
	m_kPartyUserManager.GetUnitUIDListByGSUID( iDisconnectedGSUID, vecDeleteUserList );

	BOOST_TEST_FOREACH( const UidType, iDeleteUnitUID, vecDeleteUserList )
	{
		//��Ƽ���� ���������� ������.
		if( LeaveMember( iDeleteUnitUID 
			#ifdef LOG_PARTY_BREAK
						, NetError::NOT_LEAVE_PARTY_REASON_09
			#endif // LOG_PARTY_BREAK
						) == false )
			
		{
            START_LOG( cerr, L"��ȿ���� ���� ��Ƽ�� ������ �����ϴٰ� �����Ͽ����ϴ�!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( iDeleteUnitUID )
				<< BUILD_LOG( NetError::GetLastErrMsg() )
				<< END_LOG;
		}

		// ��Ƽ�� �ƹ��� ����.?
		if( m_kPartyUserManager.GetNumMember() <= PE_CLOSE_PARTY_NUM_MEMBER )
		{
			CloseParty( NetError::NOT_CLOSE_PARTY_REASON_01 );
		}
	}
}

//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
IMPL_ON_FUNC( EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, KEPM_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK );

	//��Ƽ������ �˻�
	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cerr, L"��Ƽ���� �ƴѵ� �ּ� ���� ���� ���� ������ ��û��." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kPacket );
		return;
	}

	//���� ������ ������ �˻�.
	if( GetCheckLowLevel() == kPacket_.m_bCheck )
	{
		START_LOG( cwarn, L"���缳���� ���� �ּ� ���� ���� ���� ������." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( GetCheckLowLevel() )
			<< BUILD_LOG( kPacket_.m_bCheck )
			<< BUILD_LOG( GetName() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_43;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////
	// ���� ���� ���� ó��

	//���������� �ٲٰ�.
	SetCheckLowLevel( kPacket_.m_bCheck );

	// �ٸ� ��Ƽ���鿡�� �ٲ� ���� �˸�
	BroadCast( EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_NOT, kPacket_.m_bCheck );

	//@@ �渮��Ʈ �߰�&����
	if( IsPlaying() == false )
	{
		if( IsPublic() == true &&  m_kPartyUserManager.GetMaxMember() >= 2 )
		{
			AddPartyInfoInPartyList();
		}
	}

	// ��Ƽ ���� ����
	UpdatePartyInfoInPartyList();
}
#endif SERV_DUNGEON_DIFFICULTY_REVISION
//}}

//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
_IMPL_ON_FUNC( EPM_CHANGE_PARTY_TYPE_REQ, KEGS_CHANGE_PARTY_TYPE_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_CHANGE_PARTY_TYPE_ACK, KEGS_CHANGE_PARTY_TYPE_ACK );

	//��Ƽ������ �˻�
	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cerr, L"��Ƽ���� �ƴѵ� ��Ƽ Ÿ���� ���� �Ϸ���." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHANGE_PARTY_TYPE_ACK, kPacket );
		return;
	}

	// ��Ƽ Ÿ�� üũ
	if( GetPartyType() == kPacket_.m_cPartyType )
	{
		START_LOG( cerr, L"�̹� �ش� Ÿ���ε� �� �ٲܷ��� �ϳ�?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOGc( GetPartyType() )
			<< END_LOG;
		
		kPacket.m_iOK = NetError::ERR_MATCH_MAKING_05;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHANGE_PARTY_TYPE_ACK, kPacket );
		return;
	}

	// ���� ��Ƽ�� �����Ҷ� üũ�ؾ��ϴ� ���ǵ�..
	if( kPacket_.m_cPartyType == KPartyInfo::PT_PVP_PARTY )
	{
		// ������Ƽ���� ������Ƽ�� �����Ҷ� �����ο� ���� 4���̸� �������� ���Ѵ�.
		if( m_kPartyUserManager.GetNumMember() >= PE_LIMIT_PARTY_MEBMER )
		{
			START_LOG( cerr, L"��Ƽ�ο��� 4���϶��� ������Ƽ�� ������ �Ұ����ϴ�!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOGc( kPacket_.m_cPartyType )
				<< BUILD_LOG( m_kPartyUserManager.GetNumMember() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_MATCH_MAKING_06;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHANGE_PARTY_TYPE_ACK, kPacket );
			return;
		}

		// ��Ƽ�ο��� ������ 10���� �ȵǴ� ������ �ִ°�� ������Ƽ�� ������ �Ұ���.
		if( m_kPartyUserManager.IsAllPartyMemberEqualOrOverThisLevel( SEnum::PE_PVP_OFFICIAL_LIMIT_LEVEL ) == false )
		{
			START_LOG( cerr, L"��Ƽ���߿� 10������ �ȵǴ� ����� �ֽ��ϴ�!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOGc( kPacket_.m_cPartyType )
				<< BUILD_LOG( m_kPartyUserManager.GetNumMember() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_MATCH_MAKING_08;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHANGE_PARTY_TYPE_ACK, kPacket );
			return;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// �׿ܿ� ��Ƽ Ÿ�� ����� üũ�ؾ��� �͵��� ���⿡�� üũ����!!
	//////////////////////////////////////////////////////////////////////////

	// ��Ƽ Ÿ�� ����
	SetInfo( kPacket_ );

	// ���ó��
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHANGE_PARTY_TYPE_ACK, kPacket );

    // ��� ��Ƽ���鿡�� �˸���!
	KEGS_CHANGE_PARTY_TYPE_NOT kNot;
	GetInfo( kNot.m_kPartyInfo );	
	BroadCast( EPM_CHANGE_PARTY_TYPE_NOT, kNot );

	// ��Ƽ ���� ����
	UpdatePartyInfoInPartyList( true );

	// ��Ƽ ��ǳ�� �߰� ����
	if( GetPartyType() == KPartyInfo::PT_DUNGEON_PARTY )
	{
		// ��Ƽ ����Ʈ�� �ִ� ��Ƽ ���� ������Ʈ
		AddPartyInfoInPartyList();
	}
	else
	{
		// ��� ��Ƽ������ ready���¸� �ʱ�ȭ �Ѵ�!
		m_kPartyUserManager.ChangeAllReady( false );

		// ���Ӽ����� ��Ƽ ����Ʈ�� ��Ƽ ���� ����
		DelPartyInfoInPartyList( true );
	}
}

IMPL_ON_FUNC( EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_ACK, KEPM_GET_PARTY_INFO_FOR_MATCH_MAKING_ACK );

	// ���� ��Ī ���û���� �˻�
	if( kPacket_.m_bRegFrontOfList == false )
	{
		// ���û�� �ƴ϶�� ��Ƽ������ �˻�����!
		if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
		{
			START_LOG( cerr, L"��Ƽ���� �ƴѵ� ��ġ ����ŷ�� ��û�Ϸ���." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PARTY_26;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_ACK, kPacket );
			return;
		}
	}

	// ���Ĵ����� 10���� �̻� �����մϴ�.
	if( m_kPartyUserManager.IsAllPartyMemberEqualOrOverThisLevel( SEnum::PE_PVP_OFFICIAL_LIMIT_LEVEL ) == false )
	{
		START_LOG( cerr, L"��Ƽ���߿� 10������ �ȵǴ� ����� �ֽ��ϴ�!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_MATCH_MAKING_09;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHANGE_PARTY_TYPE_ACK, kPacket );
		return;
	}

	// ��ġ ���� ���� ������!
	m_kPartyUserManager.GetMatchUserInfoList( GetUID(), kPacket.m_vecMatchUserInfo );

	// ��ġ ��û�Ϸ� ����!
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_bRegFrontOfList = kPacket_.m_bRegFrontOfList;
	kPacket.m_cPvpGameType = kPacket_.m_cPvpGameType;
	kPacket.m_cPvpPlayerCount = kPacket_.m_cPvpPlayerCount;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_ACK, kPacket );
}

_IMPL_ON_FUNC( EPM_PVP_PARTY_CHANGE_MATCH_INFO_REQ, KEGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PVP_PARTY_CHANGE_MATCH_INFO_ACK, KEGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK );

	//��Ƽ������ �˻�
	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cerr, L"��Ƽ���� �ƴѵ� ��ġ ������ �����Ϸ���." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PVP_PARTY_CHANGE_MATCH_INFO_ACK, kPacket );
		return;
	}

	// �������� ���� Ÿ������ üũ
	if( kPacket_.m_cPvpGameType != CXSLRoom::PGT_TEAM  &&
		kPacket_.m_cPvpGameType != CXSLRoom::PGT_TEAM_DEATH )
	{
		START_LOG( cerr, L"���� ��Ƽ���� ���� ������ ���� Ÿ���� �ƴմϴ�." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< BUILD_LOGc( kPacket_.m_cPvpGameType )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PVP_PARTY_CHANGE_MATCH_INFO_ACK, kPacket );
		return;
	}

	// ���� Ÿ�� ����
	m_cPvpGameType = kPacket_.m_cPvpGameType;

	// ��ġ ���� ����!
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PVP_PARTY_CHANGE_MATCH_INFO_ACK, kPacket );

	// ��� ��Ƽ���鿡�� ����
	KEGS_PVP_PARTY_CHANGE_MATCH_INFO_NOT kPacketNot;
	kPacketNot.m_cPvpGameType = kPacket_.m_cPvpGameType;
	BroadCast( EPM_PVP_PARTY_CHANGE_MATCH_INFO_NOT, kPacketNot );
}

#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
IMPL_ON_FUNC( EPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_REQ )
{
	// �ð����� ��Ƽ�� �����ִ� ���¶�� ��Ƽ ���� ���� ���� �״�� ����������!
	switch( GetStateID() )
	{
	case KCnPartyFSM::S_INIT:
	case KCnPartyFSM::S_CLOSE:
		{
			KEPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_ACK kPacket;
			kPacket.m_iOK = NetError::NET_OK;
			kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_ACK, kPacket );
			return;
		}
		break;
	}

	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_ACK, KEPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_ACK );

	// ���� ��Ƽ������ �����ؼ� �ٽ� ��������!
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;

	// ��Ƽ�� ����Ʈ�� ����
	m_kPartyUserManager.GetUnitUIDList( kPacket.m_kBattleFieldJoinInfo.m_vecPartyMemberList );
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_ACK, kPacket );
}

IMPL_ON_FUNC( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK, KEPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK );

	//��Ƽ������ �˻�
	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cerr, L"��Ƽ���� �ƴѵ� ��ġ ������ �����Ϸ���." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK, kPacket );
		return;
	}

#ifdef SERV_DUNGEON_OPTION_IN_LUA
	if( CXSLDungeon::IsFixedMembers( GetDungeonIDAndDif() ) == true )
	{
		int iFixedMembers = CXSLDungeon::GetFixedMembers( GetDungeonIDAndDif() );
		int iCurrentMembers = m_kPartyUserManager.GetNumMember();

		if( iCurrentMembers == iFixedMembers )
		{
			kPacket.m_iOK = NetError::ERR_PARTY_61;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK, kPacket );
			return;
		}
		else if( iCurrentMembers > iFixedMembers )
		{
			// �ο� �ʰ��� ����
			kPacket.m_iOK = NetError::ERR_PARTY_46;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK, kPacket );
			return;
		}
	}
	else // �ο� ���� ������ �ƴҰ�� ������ ���� ����
#endif SERV_DUNGEON_OPTION_IN_LUA

	//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	if( SiKAutoPartyScriptManager()->GetAutoPartyMatchingSuccessNum( GetDungeonIDAndDif() ) <= m_kPartyUserManager.GetNumMember() )
	{
		kPacket.m_iOK = NetError::ERR_PARTY_61;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK, kPacket );
		return;
	}
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	//}}

	// Ÿ�̸� �ʱ�ȭ
	m_tTimer[TE_CHECK_AUTO_PARTY].restart();

	// �ڵ� ��Ƽ ������ ���� üũ ���·� �̵�!
	StateTransition( KCnPartyFSM::I_TO_CHECK_AUTO_PARTY );

	// ���� �ڵ� ��Ƽ ��û�� ���û �°����� ����!
	m_bRemakingAutoParty = kPacket_.m_bRemakingAutoParty;

	// �غ� ���� üũ���� �ʱ�ȭ!
	m_kPartyUserManager.ClearCheckedForAutoParty();

	// �� ��Ƽ���鿡�� ���� �غ� ���¸� üũ �Ϸ� ����!
	KEPM_CHECK_FOR_AUTO_PARTY_MAKING_REQ kPacketReq;
	GetDungeonGameInfo( kPacketReq );
	BroadCast( EPM_CHECK_FOR_AUTO_PARTY_MAKING_REQ, kPacketReq );
}

IMPL_ON_FUNC( EPM_CHECK_FOR_AUTO_PARTY_MAKING_ACK )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_CHECK_AUTO_PARTY ) );

	// üũ ����� ���ж�� �ڵ� ��Ƽ ��û ���� ���� ó��!
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// ���� ó��!
		FailProcessAutoParty( kPacket_.m_iOK, kPacket_.m_wstrFailUserNickName );
		return;
	}
	
	// �ش� ��Ƽ�� üũ ���� ������Ʈ!
	if( m_kPartyUserManager.SetCheckedForAutoParty( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"��Ƽ�� üũ ��� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}

	// �ش� ��Ƽ���� üũ ���� ������Ʈ!
	CheckAllMembersAutoParty();
}

IMPL_ON_FUNC( EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_INIT ), EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK );
		
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iAutoPartyUID = kPacket_.m_iAutoPartyUID;

	// �������ϴ� ��Ƽ���� �˻�
	if( static_cast<int>(kPacket_.m_vecRegroupPartyUserInfo.size()) > CXSLPartyManager::PE_MAX_PARTY_NUM )
	{
        START_LOG( cerr, L"�� �����ϴ� ��Ƽ������ 4���� �Ѿ���! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( kPacket_.m_vecRegroupPartyUserInfo.size() )
			<< END_LOG;
	}

	// ���ο� ��Ƽ ���� �� ��� ���ο� ��Ƽ���� ���� ó��
	if( m_kPartyUserManager.Create( kPacket_.m_vecRegroupPartyUserInfo ) == false )
	{
		START_LOG( cerr, L"������ ��Ƽ�� ���� ����Ʈ �������.!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_vecRegroupPartyUserInfo.size() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_00;
		goto end_proc;
	}

	//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	// �ڵ� ��Ƽ ��Ƽ���� ����������!
	if( kPacket_.m_iAutoPartyHostUnitUID != 0  &&  m_kPartyUserManager.GetHostUID() != kPacket_.m_iAutoPartyHostUnitUID )
	{
		if( m_kPartyUserManager.ChangeHost( kPacket_.m_iAutoPartyHostUnitUID ) == false )
		{
			START_LOG( cerr, L"��Ƽ�� �������." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
				<< BUILD_LOG( kPacket_.m_iAutoPartyHostUnitUID )
				<< END_LOG;
			// �����α׸� ����
		}
	}
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}

	// ���������� ��Ƽ�� �������ְ�
	SetInfo( kPacket_.m_kCreatePartyInfo );

	// ���� ������ �� �� �ֵ��� PartyRoomUserInfo�� ����
	BOOST_TEST_FOREACH( const KRegroupPartyUserInfo&, kRegroupPartyUserInfo, kPacket_.m_vecRegroupPartyUserInfo )
	{
		KCnPartyUserPtr spUser = m_kPartyUserManager.GetPartyUser( kRegroupPartyUserInfo.m_kPartyRoomUserInfo.m_kRoomUserInfo.m_nUnitUID );
		if( spUser == NULL )
		{
			START_LOG( cerr, L"���ӽ��� üũ��Ŷ ������ �Դµ� �Ҽ� ��Ƽ���� �ƴϴ�?" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( kRegroupPartyUserInfo.m_kPartyRoomUserInfo.m_kRoomUserInfo.m_nUnitUID )
				<< END_LOG;
			continue;
		}

		// PartyRoomUserInfo����
        spUser->SetPartyRoomUserInfo( kRegroupPartyUserInfo.m_kPartyRoomUserInfo );

		// OldPartyInfo����!
		spUser->SetOldPartyInfo( kRegroupPartyUserInfo.m_kOldPartyInfo );
	}

	// ��Ƽ����Ʈ�� ������ ��Ƽ �߰�
	{
		KEPM_OPEN_PARTY_LIST_NOT kPacketNot;
		GetInfo( kPacketNot.m_kPartyInfo );
		m_kPartyUserManager.GetUserInfoList( kPacketNot.m_vecUserInfo );
		BroadCastGameServer( EPM_OPEN_PARTY_LIST_NOT, kPacketNot );
	}

	//@@ �渮��Ʈ �߰�
	//## 1. �����ϰ��
	//## 2. �ִ��ο��� 2�� �̻��� ���.
	//## 3. �������� ���°� �ƴѰ��.
	//{{ 2011. 07. 04	������	���� ����
	if( IsPublic() == true  &&
		m_kPartyUserManager.GetMaxMember() >= 2  &&
		IsPlaying() == false )
	{
		AddPartyInfoInPartyList();
	}

	//�ʵ����� ������Ʈ.
	m_kPartyUserManager.UpdatePartyMemberListToFieldUser( GetUID() );

	// ��Ƽ ������ ���� �⺻ �۾��� �������� ���� �̵��� ����!
	StateTransition( KCnPartyFSM::I_TO_MAKING_PARTY );
	StateTransition( KCnPartyFSM::I_TO_WAIT );

	//////////////////////////////////////////////////////////////////////////
	// ��Ƽ ������ �Ϸ�Ǿ����� ���ԿϷ�� ������ �� ��Ƽ���鿡�� �˷��ְ�
	{
		KEGS_REGROUP_PARTY_NOT kRegroupNot;
		GetInfo( kRegroupNot.m_kPartyInfo );
		m_kPartyUserManager.GetUserInfoList( kRegroupNot.m_vecPartyUserInfo );
		BroadCast( EPM_REGROUP_PARTY_NOT, kRegroupNot );
	}

	//////////////////////////////////////////////////////////////////////////
	// �ٷ� ���� ���� ó�� ����!

	// ��Ƽ������ PartyRoomUserInfo�� ����
	{
		KERM_PARTY_OPEN_DUNGEON_ROOM_REQ kPacketStartGame;
		KCnPartyUserPtr spHost;

		if( m_kPartyUserManager.GetDungeonRoomUserInfoList( kPacketStartGame.m_vecUserInfo ) == false )
		{
			START_LOG( cerr, L"��Ƽ�� ���� �������� �����غ��� �������� ��������.!" )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_AUTO_PARTY_00;
			goto end_proc;
		}

		// ��Ƽ�忡�� ���� ������ ������.
		spHost = m_kPartyUserManager.GetHostPtr();
		if( spHost == NULL )
		{
			START_LOG( cerr, L"��Ƽ�� ������ �����ϴ�. ������ ���� ����!" )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_AUTO_PARTY_00;
			goto end_proc;
		}

		// �������� ����� �ֵ��� ������ �����Ѵ�.		
		kPacketStartGame.m_sWorldID								= 0;
		kPacketStartGame.m_kRoomInfo.m_RoomType					= CXSLRoom::RT_DUNGEON;
		kPacketStartGame.m_kRoomInfo.m_bPublic					= false;
		kPacketStartGame.m_kRoomInfo.m_MaxSlot					= static_cast<char>(m_kPartyUserManager.GetMaxMember());
		kPacketStartGame.m_kRoomInfo.m_iDungeonID				= GetDungeonID();
		kPacketStartGame.m_kRoomInfo.m_DifficultyLevel			= GetDungeonDif();	
		kPacketStartGame.m_kRoomInfo.m_cDungeonMode				= static_cast<char>(GetDungeonMode());	
		kPacketStartGame.m_kRoomInfo.m_iPartyUID				= GetUID();
		kPacketStartGame.m_kRoomInfo.m_bStartedByAutoParty		= true;		// �ڵ� ��Ƽ�� ���� ���� ����!

		switch( GetItemType() )
		{
		case CXSLRoom::DGIT_PERSON:
		case CXSLRoom::DGIT_RANDOM:
			kPacketStartGame.m_kRoomInfo.m_cGetItemType = GetItemType();
			break;
		default:
			//������ �߸��Ǿ������� �׻� ��������.
			kPacketStartGame.m_kRoomInfo.m_cGetItemType = CXSLRoom::DGIT_RANDOM;
			break;
		}
		kPacketStartGame.m_kRoomInfo.m_fPlayTime = DUNGEON_DEFAULT_LIMIT_PLAY_TIME;

		float fPlayTimeLimit = SiCXSLDungeonManager()->GetPlayTimeLimit( ( kPacketStartGame.m_kRoomInfo.m_iDungeonID + kPacketStartGame.m_kRoomInfo.m_DifficultyLevel ) );
		if( fPlayTimeLimit > 0.0f )
		{
			kPacketStartGame.m_kRoomInfo.m_fPlayTime = fPlayTimeLimit;
		}

		// ��Ƽ�忡�� ������ ��������� ��Ŷ ����
		spHost->SendToGSCharacter( EPM_PARTY_OPEN_DUNGEON_ROOM_NOT, kPacketStartGame );
	}

	// ��� ó���� �Ϸ�Ǿ����� ACK�� �ٽ� ������!
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacket );

	//{{ 2009. 6. 10  ������	������ ��Ÿ�� ����
	AllPartyMembersQuickSlotCoolTimeReset();
	//}}

	//���������� �����ϰ�
	SetPlaying( true );

	//��Ƽ����Ʈ���� ���ش�.
	DelPartyInfoInPartyList();
	//////////////////////////////////////////////////////////////////////////

	return;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacket );

	// ��Ƽ ���� ����! ��Ƽ�� �ݽ��ϴ�.
	CloseParty( NetError::NOT_CLOSE_PARTY_REASON_00 );
}

IMPL_ON_FUNC( EPM_REQUEST_REGROUP_PARTY_NOT )
{
	VERIFY_PARTY_STATE( ( 1, KCnPartyFSM::S_INIT ) );

	// ���ο� ��Ƽ ���� �� ��� ���ο� ��Ƽ���� ���� ó��
	if( m_kPartyUserManager.Create( kPacket_.m_vecRegroupPartyUserInfo ) == false )
	{
		START_LOG( cerr, L"������ ��Ƽ�� ���� ����Ʈ �������.!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_vecRegroupPartyUserInfo.size() )
			<< END_LOG;
		goto end_proc;
	}

	// ���������� ��Ƽ�� �������ְ�
	SetInfo( kPacket_.m_kCreatePartyInfo );

	// ��Ƽ �ǹ� ����!
	UpdateAndBroadCastFeverPoint( kPacket_.m_sPartyFever );

	// ��Ƽ����Ʈ�� ������ ��Ƽ �߰�
	{
		KEPM_OPEN_PARTY_LIST_NOT kPacketNot;
		GetInfo( kPacketNot.m_kPartyInfo );
		m_kPartyUserManager.GetUserInfoList( kPacketNot.m_vecUserInfo );
		BroadCastGameServer( EPM_OPEN_PARTY_LIST_NOT, kPacketNot );
	}

	//@@ �渮��Ʈ �߰�
	//## 1. �����ϰ��
	//## 2. �ִ��ο��� 2�� �̻��� ���.
	//## 3. �������� ���°� �ƴѰ��.
	//{{ 2011. 07. 04	������	���� ����
	if( IsPublic() == true  &&
		m_kPartyUserManager.GetMaxMember() >= 2  &&
		IsPlaying() == false )
	{
		AddPartyInfoInPartyList();
	}

	//�ʵ����� ������Ʈ.
	m_kPartyUserManager.UpdatePartyMemberListToFieldUser( GetUID() );

	// ��Ƽ ������ ���� �⺻ �۾��� �������� ���� �̵��� ����!
	StateTransition( KCnPartyFSM::I_TO_MAKING_PARTY );
	StateTransition( KCnPartyFSM::I_TO_WAIT );

	//////////////////////////////////////////////////////////////////////////
	// ��Ƽ ������ �Ϸ�Ǿ����� ���ԿϷ�� ������ �� ��Ƽ���鿡�� �˷��ְ�
	{
		KEGS_REGROUP_PARTY_NOT kRegroupNot;
		GetInfo( kRegroupNot.m_kPartyInfo );
		m_kPartyUserManager.GetUserInfoList( kRegroupNot.m_vecPartyUserInfo );
		BroadCast( EPM_REGROUP_PARTY_NOT, kRegroupNot );
	}
	//////////////////////////////////////////////////////////////////////////

	{
		// ��Ƽ �ǹ��� �ѹ� �Ѹ���!
		KEGS_PARTY_FEVER_POINT_NOT kNot;
		kNot.m_iPartyFever = GetFeverPoint();
		BroadCast( EPM_PARTY_FEVER_POINT_NOT, kNot );
	}

	{
		//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		// ��Ƽ���� ��ü�� �ʿ䰡 �ִٸ� ��ü ����!
		if( kPacket_.m_iOldHostUnitUID != m_kPartyUserManager.GetHostUID() )
		{
			KCnPartyUserPtr spNewHost = m_kPartyUserManager.GetPartyUser( kPacket_.m_iOldHostUnitUID );
			if( IS_NOT_NULL( spNewHost ) )
			{
				KEPM_CHECK_PARTY_CHANGE_HOST_REQ kPacketReq;
				kPacketReq.m_iPartyUID		 = GetUID();
				//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
				kPacketReq.m_cPartyType		 = GetPartyType();
#endif SERV_PVP_NEW_SYSTEM
				//}}
				kPacketReq.m_iOldHostUnitUID = m_kPartyUserManager.GetHostUID();
				kPacketReq.m_iNewHostUnitUID = kPacket_.m_iOldHostUnitUID;
				kPacketReq.m_iDungeonID		 = GetDungeonID();
				kPacketReq.m_cDifficulty	 = GetDungeonDif();
				kPacketReq.m_cDungeonMode	 = GetDungeonMode();
				//{{ 2012. 11. 13	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
				kPacketReq.m_bForRegroupParty = true;
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
				spNewHost->SendToGSCharacter( EPM_CHECK_PARTY_CHANGE_HOST_REQ, kPacketReq );
			}
			else
			{
				START_LOG( cerr, L"��Ƽ���� �����Ͱ��� �������� �ʴ´�!" )
					<< BUILD_LOG( kPacket_.m_iOldHostUnitUID )
					<< END_LOG;
			}
		}
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
	}

	return;

end_proc:
	// ��Ƽ ���� ����! ��Ƽ�� �ݽ��ϴ�.
	CloseParty( NetError::NOT_CLOSE_PARTY_REASON_00 );
}

IMPL_ON_FUNC_NOPARAM( ERM_END_DUNGEON_GAME_PARTY_FEVER_MODIFY_NOT )
{
	VERIFY_PARTY_STATE( ( 1, KCnPartyFSM::S_WAIT ) );

	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		// ��Ƽ���� �ƴϸ� �ƹ��͵� ���� �ʴ´�.
		return;
	}

	// ��Ƽ �ǹ� ����
	if( m_kFeverManager.GetRealFeverPoint() > KCnPartyFeverManager::FEVER_100_PERCENT_POINT )
	{
		UpdateAndBroadCastFeverPoint( KCnPartyFeverManager::FEVER_100_PERCENT_POINT );
	}
}

IMPL_ON_FUNC( EPM_UPDATE_PARTY_USER_JOIN_ROOM_INFO_NOT )
{
	VERIFY_PARTY_STATE( ( 1, KCnPartyFSM::S_WAIT ) );

	// ��Ƽ�� �� ���� ���� ������Ʈ!
	UpdateJoinRoomInfoAndGetOnOffResult( FIRST_SENDER_UID, kPacket_.m_kJoinRoomInfo );
}

IMPL_ON_FUNC_NOPARAM( EPM_END_GAME_REGROUP_PARTY_NOT )
{
	VERIFY_PARTY_STATE( ( 1, KCnPartyFSM::S_WAIT ) );

	KEPM_REQUEST_REGROUP_PARTY_NOT kRegroupNot;
	std::set< UidType > setLeaveUnitUIDList;

	// ��Ƽ �����
	m_kPartyUserManager.GetUnitUIDListAboutEndGameRegroupParty( kRegroupNot.m_vecRegroupPartyUserInfo, setLeaveUnitUIDList );
	
	// ���� ��Ż���Ѿ��� �������� ��Ż ��Ű��!
	BOOST_TEST_FOREACH( const UidType, iUnitUID, setLeaveUnitUIDList )
	{
		KCnPartyUserPtr spLeaveUser = m_kPartyUserManager.GetPartyUser( iUnitUID );
		if( IS_NULL( spLeaveUser ) )
		{	
			START_LOG( cerr, L"��Ƽ���� ��Ż��ų ������ �����ϴ�." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			return;
		}

		// �ش� �������� ��Ƽ ��Ż ó���� ������!
		KEPM_LEAVE_PARTY_BY_AUTO_PARTY_NOT kLeavePartyNot;
		kLeavePartyNot.m_iPartyUID = GetUID();
		spLeaveUser->SendToGSCharacter( EPM_LEAVE_PARTY_BY_AUTO_PARTY_NOT, kLeavePartyNot );

		if( LeaveMember( iUnitUID
			#ifdef LOG_PARTY_BREAK
						, NetError::NOT_LEAVE_PARTY_REASON_08
			#endif // LOG_PARTY_BREAK
						) == false )
		{
			START_LOG( cerr, L"��Ƽ ������� ���� ��Ƽ�� ��Ż ó�� ����!" )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
		}
	}

	// ���ο� ��Ƽ ����ڰ� �����Ѵٸ� ����� �Ϸ� ����!
	BOOST_TEST_FOREACH( const KRegroupPartyUserInfo&, kInfo, kRegroupNot.m_vecRegroupPartyUserInfo )
	{
		kRegroupNot.m_kCreatePartyInfo.m_cPartyType		 = GetPartyType();
		kRegroupNot.m_kCreatePartyInfo.m_iDungeonID		 = GetDungeonID();
		kRegroupNot.m_kCreatePartyInfo.m_DifficultyLevel = GetDungeonDif();
		kRegroupNot.m_kCreatePartyInfo.m_cGetItemType	 = GetItemType();
		kRegroupNot.m_kCreatePartyInfo.m_cDungeonMode	 = GetDungeonMode();
		kRegroupNot.m_kCreatePartyInfo.m_bCheckLowLevel	 = GetCheckLowLevel();
		kRegroupNot.m_kCreatePartyInfo.m_cPVPGameType	 = m_cPvpGameType;
		//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		kRegroupNot.m_iOldHostUnitUID					 = kInfo.m_kOldPartyInfo.m_iOldHostUnitUID;
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		kRegroupNot.m_sPartyFever						 = kInfo.m_kOldPartyInfo.m_sOldPartyFever;
		SendToGSCharacter( kInfo.m_kPartyUserInfo.m_iGSUID, kInfo.m_kPartyUserInfo.m_iUnitUID, EPM_PREPARE_FOR_REGROUP_PARTY_AND_CHECK_NEW_HOST_NOT, kRegroupNot );
		break;
	}

	// ��Ƽ�� �ƹ��� ����.?
	if( m_kPartyUserManager.GetNumMember() <= PE_CLOSE_PARTY_NUM_MEMBER )
	{
		CloseParty( NetError::NOT_CLOSE_PARTY_REASON_01 );
	}
	else
	{
		//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		// ��Ƽ�� ���� ��� ���� ��Ƽ���� �������� Ȯ���� ��Ƽ���� �ٲ� �ʿ䰡 �ִٸ� �ٲ���!
		UidType iNewHostUID = 0;
		if( m_kPartyUserManager.ChangeOldPartyHost( iNewHostUID ) == true )
		{
			KCnPartyUserPtr spNewHost = m_kPartyUserManager.GetPartyUser( iNewHostUID );
			if( IS_NOT_NULL( spNewHost ) )
			{
				KEPM_CHECK_PARTY_CHANGE_HOST_REQ kPacketReq;
				kPacketReq.m_iPartyUID		 = GetUID();
				//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
				kPacketReq.m_cPartyType		 = GetPartyType();
#endif SERV_PVP_NEW_SYSTEM
				//}}
				kPacketReq.m_iOldHostUnitUID = m_kPartyUserManager.GetHostUID();
				kPacketReq.m_iNewHostUnitUID = iNewHostUID;
				kPacketReq.m_iDungeonID		 = GetDungeonID();
				kPacketReq.m_cDifficulty	 = GetDungeonDif();
				kPacketReq.m_cDungeonMode	 = GetDungeonMode();
				//{{ 2012. 11. 13	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
				kPacketReq.m_bForRegroupParty = true;
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
				spNewHost->SendToGSCharacter( EPM_CHECK_PARTY_CHANGE_HOST_REQ, kPacketReq );
			}
			else
			{
				START_LOG( cerr, L"��Ƽ���� �����Ͱ��� �������� �ʴ´�!" )
					<< BUILD_LOG( iNewHostUID )
					<< END_LOG;
			}
		}
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}

		// ��Ƽ�� ���� ��� ���� ��Ƽ �ǹ��� �ǵ�����!
		const short sModifyPartyFever = m_kPartyUserManager.GetOldPartyFeverPoint();
		UpdateAndBroadCastFeverPoint( sModifyPartyFever );		
	}
}

_IMPL_ON_FUNC( EPM_UPDATE_GAME_PLAY_STATUS_NOT, KPartyMemberStatus )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_WAIT ) );

	if( m_kPartyUserManager.SetPartyMemberStatus( FIRST_SENDER_UID, kPacket_ ) == false )
	{
		START_LOG( cerr, L"��Ƽ ���� �÷��� ���� ������Ʈ ����!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	KEGS_UPDATE_PARTY_MEMBER_STATUS_NOT kPacketNot;
	kPacketNot.m_iUnitUID = FIRST_SENDER_UID;
	kPacketNot.m_kStatus = kPacket_;
    BroadCast( EPM_BROADCAST_PARTY_MEMBER_STATUS_NOT, kPacketNot );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 05. 30	������		���� ���� ���� ����
#ifdef SERV_OFFCIAL_PVP_BUG_FIX
IMPL_ON_FUNC( EPM_UPDATE_PVP_INFO_NOT )
{
	if( GetStateID() != KCnPartyFSM::S_WAIT )
	{
		// wait���°� �ƴ϶�� ����!
		return;
	}

	KCnPartyUserPtr spPartyUser = m_kPartyUserManager.GetPartyUser( FIRST_SENDER_UID );
	if( spPartyUser == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ��Ƽ ����Դϴ�!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	// ����� ���� ���� ������Ʈ!
	//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	spPartyUser->SetRank( kPacket_.m_cRank );
	spPartyUser->SetOfficialMatchCnt( kPacket_.m_iOfficialMatchCnt );
#endif SERV_2012_PVP_SEASON2
	//}}
	spPartyUser->SetRating( kPacket_.m_iRating );
	spPartyUser->SetRPoint( kPacket_.m_iRPoint );
}
#endif SERV_OFFCIAL_PVP_BUG_FIX
//}}


//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}


