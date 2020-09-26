#include "BattleFieldUserManager.h"
#include "Enum/Enum.h"
#include "X2Data/XSLDungeon.h"
#include "X2Data/XSLTitleManager.h"
#include "ResultProcess.h"
#include "NetError.h"
#include "X2Data/XSLBattleFieldManager.h"
#include "Room/RoomUserFSM.h"


//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


KBattleFieldUserManager::KBattleFieldUserManager() :
m_MaxSlot( SEnum::BATTLE_FIELD_MAX_NUM )
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
, m_bIsCheckZombieAlert( false )
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
{
}

KBattleFieldUserManager::~KBattleFieldUserManager()
{
}

//{{ 2013. 01. 07	������	�濡�� �ʵ�� ���ƿ��� ���ϴ� ���� ����
#ifdef SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
void KBattleFieldUserManager::Reset( USERLIST_TYPE eType /*= UT_GAME*/ )
{
	KRoomUserManager::Reset( eType );

	m_mapPartyList.clear();
}
#endif SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
//}}

bool KBattleFieldUserManager::EnterRoom( IN const KRoomUserInfo& kInfo, IN const KBattleFieldJoinInfo& kJoinInfo, IN const bool bConsiderTeam )
{
	SET_ERROR( NET_OK );

	std::vector< UidType > vecUnitUIDList;

	// �̹� �������ִ� �������� üũ!
	if( IsExist( kInfo.m_nUnitUID ) == true )
	{
		START_LOG( cerr, L"�̹� �������ִ� ���� �Դϴ�!" )
			<< BUILD_LOG( kInfo.m_nUnitUID )
			<< BUILD_LOG( kInfo.m_iPartyUID )
			<< END_LOG;

		SET_ERROR( ERR_BATTLEFIELD_01 );
		return false;
	}

	// ������ ������ ���� ������ �ִ��� Ȯ��! ( �󽽷��� ������ ���൵ �ҿ����! )
	if( IsExistEmptySlot() == false )
	{
		SET_ERROR( ERR_BATTLEFIELD_02 );
		return false;
	}

	// ���� ���� ��Ƽ���� �� �ʵ忡 �־ ����� �������� Ȯ��! ( ���� ��Ƽ���� �ش� �ʵ忡 ������ ���� �����ϴ�! )
	if( IsReservedPartyMember( kInfo.m_nUnitUID ) == false )
	{
		// ���� ��Ʋ�ʵ忡 ������ �������� ��Ƽ���� ������ ������ ���� ���� ���θ� üũ!
		if( GetNumMemberTotalParty() >= GetMaxSlot() )
		{
			// [memo] �ð����� �߻��Ҽ� �ִ� ����. cerr�� cwarn���� �����..? ( ������ )

			START_LOG( cerr, L"���� �濡 ������ ������ ����!�濡 ���� ó�� ����!" )
				<< BUILD_LOG( kInfo.m_nUnitUID )
				<< BUILD_LOG( kInfo.m_iPartyUID )				
				<< BUILD_LOG( GetMaxSlot() )
				<< BUILD_LOG( GetNumMember() )
				<< BUILD_LOG( GetNumMemberTotalParty() )				
				<< END_LOG;

			SET_ERROR( ERR_BATTLEFIELD_03 );
			return false;
		}
	}

	// ���� ���� ó�� �Ѵ�!
	if( KRoomUserManager::EnterRoom( kInfo, bConsiderTeam ) == false )
	{
		START_LOG( cerr, L"�濡 ���� ó�� ����!" )
			<< BUILD_LOG( kInfo.m_nUnitUID )
			<< BUILD_LOG( kInfo.m_iPartyUID )
			<< END_LOG;

		SET_ERROR( ERR_BATTLEFIELD_04 );
		return false;
	}
		
	// �������� ���� �����ߴٸ� ��Ʋ�ʵ� ��Ƽ ����Ʈ���� �߰�����!
	if( AddRoomUserInBattleFieldPartyList( kInfo.m_nUnitUID, kInfo.m_iPartyUID, kJoinInfo.m_vecPartyMemberList ) == false )
	{
		START_LOG( cerr, L"��Ƽ ��� ���� �߰� ����!" )
			<< BUILD_LOG( kInfo.m_nUnitUID )
			<< BUILD_LOG( kInfo.m_iPartyUID )
			<< END_LOG;

		SET_ERROR( ERR_BATTLEFIELD_04 );
		goto end_proc;
	}
	
	GetUnitUIDList( vecUnitUIDList, kInfo.m_nUnitUID );

	if( vecUnitUIDList.empty() == false )
	{
		if( RegIntrudeUnitLoadUserList( kInfo.m_nUnitUID, vecUnitUIDList ) == false )
		{
			START_LOG( cerr, L"���� ���� �ε� ����� ����Ʈ ��� ����!" )
				<< BUILD_LOG( kInfo.m_nUnitUID )
				<< BUILD_LOG( vecUnitUIDList.size() )
				<< END_LOG;

			SET_ERROR( ERR_BATTLEFIELD_04 );
			goto end_proc;
		}
	}

	return true;

end_proc:
	// ��Ż ó�� �մϴ�!
	LIF( KRoomUserManager::LeaveRoom( kInfo.m_nUnitUID ) );
	return false;
}

bool KBattleFieldUserManager::LeaveRoom( IN const UidType iUnitUID )
{
	if( DelRoomUserInBattleFieldPartyList( iUnitUID ) == false )
	{
		START_LOG( cerr, L"��Ƽ ��� ���� ���� ����!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
	}

	return KRoomUserManager::LeaveRoom( iUnitUID );
}

void KBattleFieldUserManager::GetAllPartyMemberListInfo( OUT std::map< UidType, UidType >& mapUnitUIDPartyUID, OUT std::map< UidType, std::set< UidType > >& mapAllPartyMemberList )
{
	mapUnitUIDPartyUID.clear();
	mapAllPartyMemberList.clear();

	// ���� ������ ������ UnitUID������ PartyUID������ ��´�.
	GetUnitUIDByPartyUIDList( mapUnitUIDPartyUID );
    
	// ��Ƽ ����Ʈ ���
	std::map< UidType, KBattleFieldParty >::const_iterator mit;
	for( mit = m_mapPartyList.begin(); mit != m_mapPartyList.end(); ++mit )
	{
		mapAllPartyMemberList.insert( std::make_pair( mit->first, mit->second.GetPartyMemberList() ) );
	}

	// �˻縦 �ѹ� ����!
	{
		std::map< UidType, UidType >::const_iterator mit;
		for( mit = mapUnitUIDPartyUID.begin(); mit != mapUnitUIDPartyUID.end(); ++mit )
		{
			const UidType iUnitUID = mit->first;
			const UidType iPartyUID = mit->second;

			if( iPartyUID == 0 )
				continue;

			std::map< UidType, std::set< UidType > >::const_iterator mitPL;
			mitPL = mapAllPartyMemberList.find( iPartyUID );
			if( mitPL == mapAllPartyMemberList.end() )
			{
				// �����αװ� �ʹ� ���������� �α� ������ �����.
				START_LOG( cwarn, L"��ƼUID���� �ִµ� ��Ƽ ����Ʈ���� �ش� ��Ƽ ������ ����? �Ͼ���� �ȵǴ� ����!" )
					<< BUILD_LOG( iUnitUID )
					<< BUILD_LOG( iPartyUID )
					<< END_LOG;
				continue;
			}

			const std::set< UidType >& setPartyMemberList = mitPL->second;
			if( setPartyMemberList.find( iUnitUID ) == setPartyMemberList.end() )
			{
				// �����αװ� �ʹ� ���������� �α� ������ �����.
				START_LOG( cwarn, L"��Ƽ ����Ʈ�� ��Ƽ ������ �ִµ� �ش� ������ ��Ƽ �Ҽ��� �ƴϴ�? �Ͼ���� �ȵǴ� ����!" )
					<< BUILD_LOG( iUnitUID )
					<< BUILD_LOG( iPartyUID )
					<< BUILD_LOG( setPartyMemberList.size() )
					<< END_LOG;
				continue;
			}
		}
	}	
}

bool KBattleFieldUserManager::GetPartyMemberListInfo( IN const UidType iPartyUID, OUT std::vector< UidType >& vecPartyMemberList )
{
	vecPartyMemberList.clear();

	std::map< UidType, KBattleFieldParty >::const_iterator mit;
	mit = m_mapPartyList.find( iPartyUID );
	if( mit == m_mapPartyList.end() )
	{
		START_LOG( cerr, L"��Ƽ�� ã�� ���Ͽ����ϴ�!" )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;
		return false;
	}

	mit->second.GetPartyMemberList( vecPartyMemberList );
	return true;
}

bool KBattleFieldUserManager::AddRoomUserInBattleFieldPartyList( IN const UidType iUnitUID, IN const UidType iPartyUID, IN const std::vector< UidType >& vecPartyMemberList )
{
	if( iPartyUID == 0 )
		return true;

	// �̹� �� �ʵ忡 ������ ��Ƽ���� �ִ��� ã�ƺ���!
	std::map< UidType, KBattleFieldParty >::iterator mit;
	mit = m_mapPartyList.find( iPartyUID );
	if( mit == m_mapPartyList.end() )
	{
		// ���ٸ� ��Ƽ ���� ��������!
		KBattleFieldParty kParty;
		kParty.UpdatePartyMemberList( vecPartyMemberList );
		m_mapPartyList.insert( std::make_pair( iPartyUID, kParty ) );
		return true;
	}

	if( mit->second.IsPartyMember( iUnitUID ) == false )
	{
		START_LOG( cerr, L"����� ������ �ƴѵ� ��Ƽ�� ������ �߰��Ϸ��� �Ѵ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	return true;
}

bool KBattleFieldUserManager::DelRoomUserInBattleFieldPartyList( IN const UidType iUnitUID )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"������ �������� �ʴµ� ������� ����!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	if( spRoomUser->GetPartyUID() == 0 )
		return true;

	std::map< UidType, KBattleFieldParty >::iterator mitP;
	mitP = m_mapPartyList.find( spRoomUser->GetPartyUID() );
	if( mitP == m_mapPartyList.end() )
	{
		START_LOG( cwarn, L"�ش� ��Ƽ ������ �������� �ʽ��ϴ�!" )
			<< BUILD_LOG( spRoomUser->GetPartyUID() )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		// ������ ���� ��Ƽ���������� true�� ��ȯ�ؼ� ���������� ��Ż ó���ϰ� ����!
		return true;
	}
	
	BOOST_TEST_FOREACH( const UidType, iMemberUnitUID, mitP->second.GetPartyMemberList() )
	{
		if( iMemberUnitUID == iUnitUID )
			continue;

		// ��Ƽ���߿� �ٸ� ��Ƽ���� ���� �� ��Ʋ�ʵ� �Ҽ����� Ȯ������!		
		if( IsExist( iMemberUnitUID ) == true )
		{
			// �Ҽ��� �´ٸ� ��Ʋ�ʵ� ��Ƽ������ ������ �ʾƵ� �ȴ�!
			return true;
		}
	}

	// �� ������ �Ҽӵ� ��� ��Ƽ���� ��Ż�ϸ� ��Ƽ ������ ���� ������ �Ѵ�!
    m_mapPartyList.erase( mitP );
	return true;
}

int KBattleFieldUserManager::GetNumMemberTotalParty() const
{
	int iNumMemberTotalParty = 0;

	std::map< UidType, KBattleFieldParty >::const_iterator mit;
	for( mit = m_mapPartyList.begin(); mit != m_mapPartyList.end(); ++mit )
	{
		iNumMemberTotalParty += mit->second.GetNumMember();
	}

	return iNumMemberTotalParty;
}

bool KBattleFieldUserManager::IsReservedPartyMember( IN const UidType iUnitUID )
{
	std::map< UidType, KBattleFieldParty >::const_iterator mit;
	for( mit = m_mapPartyList.begin(); mit != m_mapPartyList.end(); ++mit )
	{
		if( mit->second.IsPartyMember( iUnitUID ) == true )
			return true;
	}

	return false;
}

bool KBattleFieldUserManager::IsPartyMembersSameGuild( IN const UidType iUnitUID )
{
	int iGuildUID = 0;

	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}
	
	if( spRoomUser->GetPartyUID() == 0 )
		return false;

	//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
	if( iGuildUID == 0 )
		return false;

	iGuildUID = spRoomUser->GetGuildUID();

	USERLIST_TYPE eType = UT_GAME;
	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		// ���� �����ʹ� ���(�ڱ��ڽŰ� �˻�)
		if( mit->second->GetCID() == iUnitUID )
			continue;

		if( mit->second->GetPartyUID() != spRoomUser->GetPartyUID() )
			continue;

		if( mit->second->GetGuildUID() != iGuildUID )
			return false;
	}

	return true;
#else

	USERLIST_TYPE eType = UT_GAME;
	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( mit->second->GetPartyUID() != spRoomUser->GetPartyUID() )
			continue;

		if( mit->second->GetGuildUID() == 0 )
			return false;

		if( iGuildUID == 0 )
		{
			iGuildUID = mit->second->GetGuildUID();
			continue;
		}

		if( iGuildUID != mit->second->GetGuildUID() )
			return false;
	}

	return true;
#endif SERV_REFORM_ITEM_DROP
	//}}
}

bool KBattleFieldUserManager::RegIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const std::vector< UidType >& vecUnitUIDList )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"�ش� ������ ã������ �����ϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const UidType, iRegUnitUID, vecUnitUIDList )
	{
		spRoomUser->RegIntrudeUnitLoadUser( iRegUnitUID );
	}

	return true;
}

bool KBattleFieldUserManager::RegIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const UidType iRegUnitUID )
{
	std::vector< UidType > vecReg;
	vecReg.push_back( iRegUnitUID );
    return RegIntrudeUnitLoadUserList( iUnitUID, vecReg );
}

bool KBattleFieldUserManager::UnregIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const std::vector< UidType >& vecUnitUIDList )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"�ش� ������ ã������ �����ϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const UidType, iRegUnitUID, vecUnitUIDList )
	{
		spRoomUser->UnregIntrudeUnitLoadUser( iRegUnitUID );
	}

	return true;
}

bool KBattleFieldUserManager::UnregIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const UidType iUnregUnitUID )
{
	std::vector< UidType > vecUnreg;
	vecUnreg.push_back( iUnregUnitUID );
	return UnregIntrudeUnitLoadUserList( iUnitUID, vecUnreg );
}

void KBattleFieldUserManager::CheckAndGetIntrudeUnitLoadCompleteUserList( OUT std::vector< UidType >& vecNpcLoadUserList )
{
	vecNpcLoadUserList.clear();

	std::map< UidType, KRoomUserPtr >::const_iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin() ; mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		KRoomUserPtr spRoomUser = mit->second;
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// ���ӷε��� ��� �Ϸ�Ǿ����� üũ!
		if( spRoomUser->GetLoadingProgress() < 100 )
			continue;

		// ������ �ε��� ��� �Ϸ�Ǿ����� üũ!
		if( spRoomUser->IsEmptyIntrudeUnitLoadUserList() == false )
			continue;

		// �����ڰ� Npc�ε��� ������ �ִ��� üũ!
		if( spRoomUser->IsBattleFieldNpcLoad() == true )
			continue;

		// ���� ���� ���� ����� ����Ʈ�� ����!
		vecNpcLoadUserList.push_back( spRoomUser->GetCID() );

		// �÷��װ� ����!
		spRoomUser->SetBattleFieldNpcLoad( true );
	}
}

int	 KBattleFieldUserManager::GetSamePartyMember( IN const UidType iUnitUID, USERLIST_TYPE eType /*= UT_GAME*/ )
{
	int iCount = 0;

	if( CheckUserlistType( eType ) == false )
		return iCount;

	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return 1;
	}

	if( spRoomUser->GetPartyUID() == 0 )
		return 1;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cwarn, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->GetPartyUID() != spRoomUser->GetPartyUID() )
			continue;

		++iCount;
	}

	if( iCount <= 0 )
		iCount = 1;

	return iCount;
}

int KBattleFieldUserManager::GetSamePartyLiveMember( IN const UidType iUnitUID, USERLIST_TYPE eType /*= UT_GAME*/ )
{
	int iCount = 0;

	if( CheckUserlistType( eType ) == false )
		return iCount;

	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return 1;
	}

	if( spRoomUser->GetPartyUID() == 0 )
		return 1;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cwarn, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->IsDie() == true )
			continue;

		if( mit->second->GetPartyUID() != spRoomUser->GetPartyUID() )
			continue;

		++iCount;
	}

	if( iCount <= 0 )
		iCount = 1;

	return iCount;
}

//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
bool KBattleFieldUserManager::GetRoomUserList( std::map< UidType, KRoomUserPtr >& mapRoomUserList, USERLIST_TYPE eType /*= UT_GAME*/ )
{
	if( CheckUserlistType( eType ) == false )
		return false;

	mapRoomUserList.clear();

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cwarn, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		mapRoomUserList.insert( std::make_pair( mit->first, mit->second ) );
	}

	if( mapRoomUserList.size() <= 0 )
		return false;

	return true;
}

bool KBattleFieldUserManager::AddBattleFieldRewardEXP( IN const UidType iUnitUID, IN const float fLevelFactor, IN const int iExp, OUT KERM_NPC_UNIT_DIE_NOT& kInfo )
{
	USERLIST_TYPE eType = UT_GAME;

	// ���� ���ǿ� ���ؼ� ����ġ�� ���� ���� ���
	if( iExp <= 0 && fLevelFactor <= 0.f )
	{
		// �ڽ��� ������ ����ġ�� 0�̶�� �˷���� �Ѵ�.
		KEXPData kEXPData;
		kEXPData.m_iEXP = 0;
		kEXPData.m_iPartyEXP = 0;
		kEXPData.m_iSocketOptEXP = 0;

		kInfo.m_EXPList.insert( std::make_pair( iUnitUID, kEXPData ) );

		return true;
	}
	else if( iExp <= 0 )
	{
		return false;
	}

	// EXP ���� ���� ã��
	KRoomUserPtr spExpRewardRoomUser = GetUser( iUnitUID );
	if( IS_NULL( spExpRewardRoomUser ) )
	{
		START_LOG( cerr, L"�������� �ʴ� �������� exp �� �����Ϸ� �Ѵ�." )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iExp )
			<< END_LOG;
		return false;
	}
			
	const int iLivePartyMember = GetSamePartyLiveMember( iUnitUID );

	//////////////////////////////////////////////////////////////////////////
	// ����ġ ���� ó��
	//////////////////////////////////////////////////////////////////////////
	
	// ���� ���� ���� ���
	AddHaveExpInDungeon( spExpRewardRoomUser->GetCID(), fLevelFactor > 0.0f );
	
	const int iBattleFieldPartyBonusEXP = static_cast<int>( iExp * GetBattleFieldPartyBonusRate( iUnitUID ) );


	// GSUser�� ������Ʈ�ϰ� Ŭ���̾�Ʈ ȭ�鿡 �Ѹ� EXP ������
	KEXPData kEXPData;
	kEXPData.m_iEXP			= iExp;	// �̹� �⺻ ����ġ�� ���� ����� ���߾���.
	kEXPData.m_iPartyEXP	= iBattleFieldPartyBonusEXP;

	//////////////////////////////////////////////////////////////////////////		
	// ���Ͽɼ� ���ʽ� ����
	bool bSocketOptBonus = false;
	int iSocketOptEXP		= 0;
	int iSocketOptPartyEXP	= 0;

	const float fAddSocketOptExpRate = spExpRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_SOCKET_OPT_EXP_RATE );
	if( fAddSocketOptExpRate > 0.0f )
	{
		iSocketOptEXP		+= static_cast<int>( kEXPData.m_iEXP * fAddSocketOptExpRate );
		iSocketOptPartyEXP	+= static_cast<int>( kEXPData.m_iPartyEXP * fAddSocketOptExpRate );

		bSocketOptBonus = true;
	}

	// ���� ��Ƽ���� 2���̻�!
	if( iLivePartyMember >= 2 )
	{
		// ���� ��Ƽ���� ���� �����̸�?
		if( IsPartyMembersSameGuild( iUnitUID ) )
		{
			const float fAddGuildBonusExpRate = spExpRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_GUILD_BONUS_EXP_RATE );
			if( fAddGuildBonusExpRate > 0.0f )
			{
				iSocketOptEXP		+= static_cast<int>( kEXPData.m_iEXP * fAddGuildBonusExpRate );
				iSocketOptPartyEXP	+= static_cast<int>( kEXPData.m_iPartyEXP * fAddGuildBonusExpRate );

				bSocketOptBonus = true;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////		
	// ä�� ���ʽ�
	const float fAddChannelBonusExpRate = spExpRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_CHANNEL_BONUS_EXP_RATE );
	if( fAddChannelBonusExpRate > 0.0f )
	{
		iSocketOptEXP		+= static_cast<int>( kEXPData.m_iEXP * fAddChannelBonusExpRate );
		iSocketOptPartyEXP	+= static_cast<int>( kEXPData.m_iPartyEXP * fAddChannelBonusExpRate );

		bSocketOptBonus = true;
	}

	// ���Ͽɼ� ���ʽ�
	kEXPData.m_iSocketOptEXP	 = iSocketOptEXP + iSocketOptPartyEXP;

	// ���Ͽɼ� ���ʽ�
	if( bSocketOptBonus == true  &&  kEXPData.m_iSocketOptEXP == 0  &&  fLevelFactor > 0.0f )
	{
		kEXPData.m_iSocketOptEXP = iSocketOptEXP = 1; // ���ʽ� ����ġ �ּҰ� 1�ֱ�
	}

	//{{ 2012. 12. 18	������	�ʵ忡���� ����ġ �̺�Ʈ ����
#ifdef SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
	// �̺�Ʈ ����ġ ���ʽ�
	const float fAddEventBonusExpRate = spExpRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_EXP );
	if( fAddEventBonusExpRate > 0.f )
	{
		const int iBaseEXP = ( kEXPData.m_iEXP + kEXPData.m_iPartyEXP + kEXPData.m_iSocketOptEXP );
        kEXPData.m_iEventBonusEXP = static_cast<int>( iBaseEXP * fAddEventBonusExpRate );
	}
#endif SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
	//}}

	// �α� ����� ���� ȹ�� ����ġ ���� ����
	spExpRewardRoomUser->AddRewardEXP( kEXPData.SumEXP() );

	//���κ� EXP ó��..
	std::map< UidType, KEXPData >::iterator mitTemp;
	mitTemp = kInfo.m_EXPList.find( spExpRewardRoomUser->GetCID() );
	if( mitTemp == kInfo.m_EXPList.end() )
	{
		kInfo.m_EXPList.insert( std::make_pair( spExpRewardRoomUser->GetCID(), kEXPData ) );
	}
	else
	{
		mitTemp->second.m_iEXP += kEXPData.m_iEXP;
		mitTemp->second.m_iPartyEXP += kEXPData.m_iPartyEXP;
		mitTemp->second.m_iSocketOptEXP += kEXPData.m_iSocketOptEXP;
		//{{ 2012. 12. 18	������	�ʵ忡���� ����ġ �̺�Ʈ ����
#ifdef SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
		mitTemp->second.m_iEventBonusEXP += kEXPData.m_iEventBonusEXP;
#endif SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
		//}}
	}

	START_LOG( clog, L"::: ���� ���� EXP :::" )
		<< BUILD_LOG( spExpRewardRoomUser->GetCID() )
		<< BUILD_LOG( kEXPData.m_iEXP )
		<< BUILD_LOG( kEXPData.m_iPartyEXP )
		<< BUILD_LOG( kEXPData.m_iSocketOptEXP )
		//{{ 2012. 12. 18	������	�ʵ忡���� ����ġ �̺�Ʈ ����
#ifdef SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
		<< BUILD_LOG( kEXPData.m_iEventBonusEXP )
#endif SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
		//}}
		;

	return true;
}

#else		// SERV_REFORM_ITEM_DROP

//bool KBattleFieldUserManager::AddBattleFieldRewardEXP( IN const char cNpcLevel,
//													  IN const char cMonsterTypeFactor,
//													  IN const int iNpcMaxHP,
//													  IN const std::map< UidType, float > mapDamageByUser, 
//													  IN const int iDungeonID, 
//													  IN const char cDifficultyLevel, 
//													  OUT KERM_NPC_UNIT_DIE_NOT& kInfo )
//{
//	USERLIST_TYPE eType = UT_GAME;
//
//	// ������ ����
//	if( iNpcMaxHP <= 0 )
//	{
//		START_LOG( cerr, L"���� HP������ �̻��մϴ�! ������ ������ �� �����ϴ�!" )
//			<< BUILD_LOG( iNpcMaxHP )
//			<< END_LOG;
//		return false;
//	}
//
//	// ���� ���� ���̺�κ��� ����ġ �����͸� �����;���! ( �� ����ġ = ���� ���� x ���� Ÿ�� ��� )
//	const int nTotalExp = SiKDropTable()->GetNpcExp( cNpcLevel ) * static_cast<int>(cMonsterTypeFactor);
//
//	// ��Ʋ�ʵ� EXP����� ���ϱ�! ( ��Ʋ�ʵ� �� ����ġ = �� ����ġ x ��Ʋ�ʵ� ��� )
//	const int iBattleFieldTotalExp = static_cast<int>( static_cast<double>(nTotalExp) * SiCXSLBattleFieldManager()->GetBattleFieldFactor() );
//	if( iBattleFieldTotalExp <= 0 )
//	{
//		return false;
//	}
//
//	std::map< UidType, int > mapBattleFieldExpReward; // �� ĳ���ͺ� ����ġ ȹ�淮
//
//	// 1. �� ĳ���͵��� �޾ƾ��� �⺻ EXP ���
//	{
//		std::map< UidType, float >::const_iterator mit;
//		for( mit = mapDamageByUser.begin(); mit != mapDamageByUser.end(); ++mit )
//		{
//			// ���� ��� = ����� / ����HP
//			float fRewardFactor = mit->second / static_cast<float>(iNpcMaxHP);
//			if( fRewardFactor > 1.5f )
//				fRewardFactor = 1.5f;
//
//			// ���� EXP = �������ġ * ������
//			const int iRewardEXP = static_cast<int>( static_cast<float>(iBattleFieldTotalExp) * fRewardFactor );
//
//			mapBattleFieldExpReward.insert( std::make_pair( mit->first, iRewardEXP ) );
//		}
//	}
//
//	//EXP ���.
//	std::map< UidType, KRoomUserPtr >::iterator mit;
//	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
//	{
//		KRoomUserPtr spExpRewardRoomUser = mit->second;
//		if( spExpRewardRoomUser == NULL )
//			continue;
//
//		// ���� ���� ������..
//		if( spExpRewardRoomUser->IsDie() == true )
//			continue;
//
//		// ����ġ ���� ��������� Ȯ��!
//		std::map< UidType, int >::const_iterator mitBF;
//		mitBF = mapBattleFieldExpReward.find( spExpRewardRoomUser->GetCID() );
//		if( mitBF == mapBattleFieldExpReward.end() )
//			continue;
//
//		// 1. �⺻ ���� ����ġ
//		const int iRewardEXP = mitBF->second;
//		if( iRewardEXP == 0 )
//			continue;
//
//		// ���� �ʵ�ȿ��� ����ִ� ��Ƽ�� ��
//		const int iLivePartyMember = GetSamePartyLiveMember( spExpRewardRoomUser->GetCID() );
//
//		// 2. ��Ƽ ���ʽ� ( �ʵ� ��Ƽ ���ʽ� = NPC����ġ / ���� ������ �ִ� ��Ƽ�� �� * ��Ƽ�� ��� * ���̵� ��� )
//		int iRewardPartyEXP = 0;
//		if( iLivePartyMember > 2 )
//		{
//			const float fDungeonPartyBonusRate = SiKResultProcess()->GetDungeonPartyBonusRate( iLivePartyMember );
//			const float fDungeonPartyDifficultyBonusRate = SiKResultProcess()->GetDungeonPartyDifficultyBonusRate( CXSLDungeon::DL_EXPERT );
//			iRewardPartyEXP = static_cast<int>( static_cast<float>(iBattleFieldTotalExp) / static_cast<float>(iLivePartyMember) * fDungeonPartyBonusRate * fDungeonPartyDifficultyBonusRate );
//		}
//
//		//////////////////////////////////////////////////////////////////////////
//		// ����ġ ���� ó��
//
//		// ���� ���� ���� ���
//		const float fMonsterLvBalRate = CXSLDungeon::GetMonsterLevelBalanceRate( spExpRewardRoomUser->GetLevel(), cNpcLevel );
//
//		//{{ 2011. 03. 08	��μ� ����ġ�� ȹ���� ������ ����Ʈ �Ϸ� ���� ����
//#ifdef SERV_EXP_ACQUISITION_CONDITION_CHANGE
//		AddHaveExpInDungeon( mit->second->GetCID(), fMonsterLvBalRate > 0.0f );
//#endif SERV_EXP_ACQUISITION_CONDITION_CHANGE
//		//}}
//
//		// GSUser�� ������Ʈ�ϰ� Ŭ���̾�Ʈ ȭ�鿡 �Ѹ� EXP ������
//		KEXPData kEXPData;
//		kEXPData.m_iEXP			= static_cast<int>( iRewardEXP * fMonsterLvBalRate );
//		kEXPData.m_iPartyEXP	= static_cast<int>( iRewardPartyEXP * fMonsterLvBalRate );
//
//		//////////////////////////////////////////////////////////////////////////		
//		// ���Ͽɼ� ���ʽ� ����
//		bool bSocketOptBonus = false;
//		int iSocketOptEXP		= 0;
//		int iSocketOptPartyEXP	= 0;
//
//		//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
//		//#ifdef SERV_ROOMUSERINFO_REFAC
//		const float fAddSocketOptExpRate = spExpRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_SOCKET_OPT_EXP_RATE );
//		//#endif SERV_ROOMUSERINFO_REFAC
//		//}}
//		if( fAddSocketOptExpRate > 0.0f )
//		{
//			iSocketOptEXP		+= static_cast<int>( kEXPData.m_iEXP * fAddSocketOptExpRate );
//			iSocketOptPartyEXP	+= static_cast<int>( kEXPData.m_iPartyEXP * fAddSocketOptExpRate );
//
//			bSocketOptBonus = true;
//		}
//
//		//{{ 2009. 12. 4  ������	��彺ų
//#ifdef GUILD_SKILL_TEST
//		// ���� ��Ƽ���� 2���̻�!
//		if( iLivePartyMember >= 2 )
//		{
//			// ���� ��Ƽ���� ���� �����̸�?
//			if( IsPartyMembersSameGuild( spExpRewardRoomUser->GetCID() ) )
//			{
//				//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
//				//#ifdef SERV_ROOMUSERINFO_REFAC
//				const float fAddGuildBonusExpRate = spExpRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_GUILD_BONUS_EXP_RATE );
//				//#endif SERV_ROOMUSERINFO_REFAC
//				//}}
//				if( fAddGuildBonusExpRate > 0.0f )
//				{
//					iSocketOptEXP		+= static_cast<int>( kEXPData.m_iEXP * fAddGuildBonusExpRate );
//					iSocketOptPartyEXP	+= static_cast<int>( kEXPData.m_iPartyEXP * fAddGuildBonusExpRate );
//
//					bSocketOptBonus = true;
//				}
//			}
//		}
//#endif GUILD_SKILL_TEST
//		//}}
//
//		// ä�� ���ʽ�
//		const float fAddChannelBonusExpRate = spExpRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_CHANNEL_BONUS_EXP_RATE );
//		if( fAddChannelBonusExpRate > 0.0f )
//		{
//			iSocketOptEXP		+= static_cast<int>( kEXPData.m_iEXP * fAddChannelBonusExpRate );
//			iSocketOptPartyEXP	+= static_cast<int>( kEXPData.m_iPartyEXP * fAddChannelBonusExpRate );
//
//			bSocketOptBonus = true;
//		}
//
//		// ���Ͽɼ� ���ʽ�
//		kEXPData.m_iSocketOptEXP	 = iSocketOptEXP + iSocketOptPartyEXP;
//
//		// ���Ͽɼ� ���ʽ�
//		if( bSocketOptBonus == true  &&  kEXPData.m_iSocketOptEXP == 0  &&  fMonsterLvBalRate > 0.0f )
//		{
//			kEXPData.m_iSocketOptEXP = iSocketOptEXP = 1; // ���ʽ� ����ġ �ּҰ� 1�ֱ�
//		}
//
//		// ���Ͽɼ� ���ʽ� �ջ�
//		const int iEXPWithBonus		 = kEXPData.m_iEXP + iSocketOptEXP;
//		const int iPartyEXPWithBonus = kEXPData.m_iPartyEXP + iSocketOptPartyEXP;
//
//		//////////////////////////////////////////////////////////////////////////		
//		//080408.hoons. �ش��������� ���ڰ� ������ ���ڿ��� ����ġ�� �ش�.
//		std::vector< UidType > vecTakeEXPList; //������ EXP ���� ���ڴ� ��������.
//		{
//			std::vector< UidType > vecTutorialUIDList;
//			spExpRewardRoomUser->GetTutorialUIDList( vecTutorialUIDList );
//
//			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecTutorialUIDList )
//			{
//				KRoomUserPtr spRoomUser = GetUser( iUnitUID );
//				if( IS_NULL( spRoomUser ) )
//					continue;
//
//				//���ڰ� �������.
//				spRoomUser->AddRewardEXP( iEXPWithBonus );
//				spRoomUser->AddRewardPartyEXP( iPartyEXPWithBonus );
//				vecTakeEXPList.push_back( iUnitUID );
//			}
//		}
//
//		if( vecTakeEXPList.empty() == true )
//		{
//			//���κ� EXP ó��..
//			spExpRewardRoomUser->AddRewardEXP( iEXPWithBonus );
//			spExpRewardRoomUser->AddRewardPartyEXP( iPartyEXPWithBonus );
//
//			std::map< UidType, KEXPData >::iterator mitTemp;
//			mitTemp = kInfo.m_EXPList.find( mit->first );
//			if( mitTemp == kInfo.m_EXPList.end() )
//			{
//				kInfo.m_EXPList.insert( std::make_pair( mit->first, kEXPData ) );
//			}
//			else
//			{
//				mitTemp->second.m_iEXP += kEXPData.m_iEXP;
//				mitTemp->second.m_iPartyEXP += kEXPData.m_iPartyEXP;
//				mitTemp->second.m_iSocketOptEXP += kEXPData.m_iSocketOptEXP;
//			}
//
//			START_LOG( clog, L"::: ���� ���� EXP :::" )
//				<< BUILD_LOG( spExpRewardRoomUser->GetCID() )
//				<< BUILD_LOG( kEXPData.m_iEXP )
//				<< BUILD_LOG( kEXPData.m_iPartyEXP )
//				<< BUILD_LOG( kEXPData.m_iSocketOptEXP )
//				;
//		}
//		else
//		{
//			// ���ڿ��� ����ġ�� �� ��� ó��
//			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecTakeEXPList )
//			{
//				std::map< UidType, KEXPData >::iterator mitTemp;
//				mitTemp = kInfo.m_EXPList.find( iUnitUID );
//				if( mitTemp == kInfo.m_EXPList.end() )
//				{
//					kInfo.m_EXPList.insert( std::make_pair( iUnitUID, kEXPData ) );
//				}
//				else
//				{
//					mitTemp->second.m_iEXP += kEXPData.m_iEXP;
//					mitTemp->second.m_iPartyEXP += kEXPData.m_iPartyEXP;
//					mitTemp->second.m_iSocketOptEXP += kEXPData.m_iSocketOptEXP;
//				}
//			}
//		}		
//	}
//
//	return true;
//}
#endif SERV_REFORM_ITEM_DROP
//}}

//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
bool KBattleFieldUserManager::AddBattleFieldRewardED( IN const char cNpcLevel,
													 IN UidType& iUserUID,
													 IN float fPartyBonusRate,
													 IN const int iED,
													 IN const std::map< UidType, float >& mapDamageByUser,
													 OUT KEGS_GET_ITEM_NOT& kInfo )
{
	kInfo.m_mapGetED.clear();

	if( iED <= 0 )
	{
		START_LOG( cwarn, L"ED�� 0�ΰ�쵵 �ֳ�?" )
			<< BUILD_LOG( iED )
			<< END_LOG;
		return false;
	}

	// 1. UserFactor ���ϱ�
	{
		std::map< UidType, float >::const_iterator mit = mapDamageByUser.find( iUserUID );
		if( mit == mapDamageByUser.end() )
		{
			START_LOG( cerr, L"���� ���� ������ �ƴѵ�....���?!" )
				<< BUILD_LOG( iUserUID )
				<< BUILD_LOGc( cNpcLevel )
				<< END_LOG;
			return false;
		}
	}
	
	//ED �й����� ����.
	std::map< UidType, KRoomUserPtr >::iterator mit = m_mapRoomUser[UT_GAME].find( iUserUID );
	if( mit == m_mapRoomUser[UT_GAME].end() )
	{
		START_LOG( cerr, L"�������� �ʴ� �������ϴ�." )
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOGc( cNpcLevel )
			<< END_LOG;
		return false;
	}

	KRoomUserPtr spEDRewardRoomUser = mit->second;
	if( spEDRewardRoomUser == NULL )
		return false;

	// ���� ���� �������Ը� ����
	if( mit->second->IsDie() == true )
		return false;

	//////////////////////////////////////////////////////////////////////////
	// ED���� ó��
	// ���ʽ�
	const float fAddSocketObtEDRate = spEDRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_SOCKET_OPT_ED_RATE );

	KDungeonRewardED kRewardED;
	kRewardED.m_iED = iED;
	kRewardED.m_iBonusED = static_cast<int>( iED * fAddSocketObtEDRate );

	if( fAddSocketObtEDRate > 0.0f  &&  kRewardED.m_iBonusED == 0 )
	{
		kRewardED.m_iBonusED = 1; // ED���ʽ� �ּҰ� 1ED ����
	}

	if( fPartyBonusRate > 0.0f )
	{
		kRewardED.m_iBonusED += static_cast<int>( iED * fPartyBonusRate );
	}

	if( IsComeBackUserInParty( iUserUID ) == true )
	{
		kRewardED.m_iBonusED += static_cast<int>( iED * 0.1f );
	}

	// �������� ED��������
	spEDRewardRoomUser->AddRewardED( kRewardED.SumED() );

	// GSUser�� ������Ʈ�� ED���� ����
	kInfo.m_mapGetED.insert( std::make_pair( spEDRewardRoomUser->GetCID(), kRewardED ) );

	return true;
}
#else
bool KBattleFieldUserManager::AddBattleFieldRewardED( IN const char cNpcLevel,
													 IN const int iNpcMaxHP,
													 IN const std::map< UidType, float > mapDamageByUser, 
													 IN const int iED,
													 OUT KEGS_GET_ITEM_NOT& kInfo )
{
	kInfo.m_mapGetED.clear();

	if( iED <= 0 )
	{
		START_LOG( cwarn, L"ED�� 0�ΰ�쵵 �ֳ�?" )
			<< BUILD_LOG( iED )
			<< END_LOG;
		return false;
	}

	if( iNpcMaxHP <= 0 )
	{
		START_LOG( cerr, L"���� HP������ �̻��մϴ�! ������ ������ �� �����ϴ�!" )
			<< BUILD_LOG( iNpcMaxHP )
			<< END_LOG;
		return false;
	}

	std::map< UidType, int > mapBattleFieldEDReward; // �� ĳ���ͺ� ED ȹ�淮

	// 1. �� ĳ���͵��� �޾ƾ��� �⺻ ED ���
	{
		std::map< UidType, float >::const_iterator mit;
		for( mit = mapDamageByUser.begin(); mit != mapDamageByUser.end(); ++mit )
		{
			// ���� ��� = Ÿ�� ����� / ����HP
			float fRewardFactor = mit->second / static_cast<float>(iNpcMaxHP);
			if( fRewardFactor > 1.5f )
				fRewardFactor = 1.5f;

			// �ش� ĳ���Ϳ��� ���޵� ED���� = ED���� * ���� ���
			float fRewardED = static_cast<float>(iED) * fRewardFactor;

			// ED���� ����
			if( fRewardED > 0  &&  fRewardED < 1.f )
			{
				fRewardED = 1.f;
			}

			mapBattleFieldEDReward.insert( std::make_pair( mit->first, static_cast<int>(fRewardED) ) );
		}
	}

	//ED ���.
	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		KRoomUserPtr spEDRewardRoomUser = mit->second;
		if( spEDRewardRoomUser == NULL )
			continue;

		// ���� ���� �������Ը� ����
		if( mit->second->IsDie() == true )
			continue;

		std::map< UidType, int >::const_iterator mitBF;
		mitBF = mapBattleFieldEDReward.find( spEDRewardRoomUser->GetCID() );
		if( mitBF == mapBattleFieldEDReward.end() )
			continue;

		// �ش� ĳ���Ͱ� �޾ƾ��� ���� ED
		const int iRewardED = mitBF->second;
		if( iRewardED == 0 )
			continue;

		//////////////////////////////////////////////////////////////////////////
		// ED���� ó��

		// ���� ���� ���
		const float fMonsterLvBalRate = CXSLDungeon::GetMonsterLevelBalanceRate( spEDRewardRoomUser->GetLevel(), cNpcLevel );

		//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
		//#ifdef SERV_ROOMUSERINFO_REFAC
		const float fAddSocketObtEDRate = spEDRewardRoomUser->GetBonusRate( KRoomUserInfo::BT_SOCKET_OPT_ED_RATE );
		//#endif SERV_ROOMUSERINFO_REFAC
		//}}

		KDungeonRewardED kRewardED;
		kRewardED.m_iED = static_cast<int>( iRewardED * fMonsterLvBalRate );
		kRewardED.m_iBonusED = static_cast<int>( iRewardED * fAddSocketObtEDRate );

		if( fAddSocketObtEDRate > 0.0f  &&  kRewardED.m_iBonusED == 0 )
		{
			kRewardED.m_iBonusED = 1; // ED���ʽ� �ּҰ� 1ED ����
		}

		//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
		if( IsComeBackUserInRoom( iUserUID ) == true )
		{
			kRewardED.m_iBonusED += static_cast<int>( iRewardED * 0.1f );
		}
#endif SERV_COME_BACK_USER_REWARD
		//}} 

		// �������� ED��������
		spEDRewardRoomUser->AddRewardED( kRewardED.SumED() );

		// GSUser�� ������Ʈ�� ED���� ����
		kInfo.m_mapGetED.insert( std::make_pair( spEDRewardRoomUser->GetCID(), kRewardED ) );
	}

	return true;
}
#endif SERV_REFORM_ITEM_DROP
//}}

bool KBattleFieldUserManager::GetItemWithRandom( IN const std::map< UidType, float > mapDamageByUser, 
												 IN const UidType iGetItemUnitUID, 
												 IN const int iItemID, 
												 IN const int iDungeonID,  
												 OUT UidType& iGetUnitUID, 
												 OUT UidType& iGSUID )
{
	USERLIST_TYPE eType = UT_GAME;	

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"Item Templet ������ ����. �Ͼ �� ���� ����" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	// �������� ȹ���� ���� ����Ʈ�� ����!
	std::map< UidType, float > mapGetItemProb;	
	float fTotalDamage = 0.f;

	{
		// ȹ�� ������ ������ ������ �����!
		std::map< UidType, float >::const_iterator mit;
		for( mit = mapDamageByUser.begin(); mit != mapDamageByUser.end(); ++mit )
		{
			KRoomUserPtr spRoomUser = GetUser( mit->first );
			if( spRoomUser == NULL )
			{
				START_LOG( cerr, L"�� ���� ������ �̻�. CUID : " << mit->first );
				continue;
			}

			if( spRoomUser->IsDie() == true )
				continue;

			fTotalDamage += mit->second;
			mapGetItemProb.insert( std::make_pair( mit->first, mit->second ) );
		}
	}

	if( fTotalDamage == 0.f )
	{
		// �������� ���� ����ڰ� ����.
		return false;
	}

	std::map< int, UidType > mapUnitUIDByIndex;
	int iGetItemUnitIndex = 0;
	KLottery kGetItemRandom;

	{
		std::map< UidType, float >::const_iterator mit;
		for( mit = mapGetItemProb.begin(); mit != mapGetItemProb.end(); ++mit )
		{
			const float fDamagePercent = mit->second / fTotalDamage;
			const int iUnitIndex = ++iGetItemUnitIndex;

			// �ش� �ε����� Ű������ UnitUID�� ��������!
			mapUnitUIDByIndex.insert( std::make_pair( iUnitIndex, mit->first ) );

			// �ش� �ε����� case�� �������� ��������!
			LIF( kGetItemRandom.AddCase( iUnitIndex, fDamagePercent ) );
		}
	}

	// ��� Ȯ�� ���� 100%
	if( kGetItemRandom.GetTotalProb() < 100.f )
	{
		kGetItemRandom.MakeHundredProbabillty();
	}

	const int iResultCase = kGetItemRandom.Decision();
	if( iResultCase == KLottery::CASE_BLANK )
	{
		START_LOG( cerr, L"������ ȹ���� ������ �����ϴ�!" )
			<< BUILD_LOG( kGetItemRandom.GetCaseNum() )
			<< END_LOG;
		return false;
	}

	std::map< int, UidType >::const_iterator mitIdx;
	mitIdx = mapUnitUIDByIndex.find( iResultCase );
	if( mitIdx == mapUnitUIDByIndex.end() )
	{
		START_LOG( cerr, L"������ ȹ���� ������ ã�� ���߽��ϴ�!" )
			<< BUILD_LOG( iResultCase )
			<< END_LOG;
		return false;
	}

	KRoomUserPtr spSelectedUser = GetUser( mitIdx->second );
	if( spSelectedUser == NULL )
	{
		START_LOG( cerr, L"������ ȹ���� ������ ã�� ���߽��ϴ�!" )
			<< BUILD_LOG( iResultCase )
			<< END_LOG;
		return false;
	}

	iGetUnitUID = spSelectedUser->GetCID();
	iGSUID = spSelectedUser->GetGSUID();
	return true;
}

void KBattleFieldUserManager::UpdatePartyInfo( IN const UidType iUnitUID, IN const KERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT& kNot )
{
	// �ش� ���� ���� ���
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"���� ���� �ʴ� ���� �Դϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return;
	}

	// ���� PartyUID���� ��ȿ�ϴٸ� ��Ƽ ���� ���� �Ǵ� ���� ó����!
	if( kNot.m_iPartyUID != 0 )
	{
		// PartyUID�� ����!
		spRoomUser->SetPartyUID( kNot.m_iPartyUID );

		// ��Ƽ�� ����Ʈ ����!
		std::map< UidType, KBattleFieldParty >::iterator mit;
		mit = m_mapPartyList.find( kNot.m_iPartyUID );
		if( mit == m_mapPartyList.end() )
		{
			// �ش� ��Ƽ������ ������ �߰��ϸ� �ȴ�.
			KBattleFieldParty kPartyInfo;
			kPartyInfo.UpdatePartyMemberList( kNot.m_vecPartyUnitUIDList );
			m_mapPartyList.insert( std::make_pair( kNot.m_iPartyUID, kPartyInfo ) );
		}
		else
		{
			mit->second.Clear();
			mit->second.UpdatePartyMemberList( kNot.m_vecPartyUnitUIDList );
		}
	}
	// �ƴ϶�� ��Ƽ ��Ż ó�� �մϴ�.
	else
	{
		const UidType iPartyUID = spRoomUser->GetPartyUID();
		if( iPartyUID != 0 )
		{
			if( DelRoomUserInBattleFieldPartyList( iUnitUID ) == false )
			{
				START_LOG( cerr, L"��Ƽ�� �ҼӵǾ� �־��µ� ��Ƽ ������ ����? �Ͼ���� �ȵǴ� ����!" )
					<< BUILD_LOG( iUnitUID )
					<< BUILD_LOG( iPartyUID )
					<< END_LOG;
			}

			spRoomUser->SetPartyUID( 0 );
		}
	}
	
}

/*
//{{ 2012. 07. 16	������		��Ʋ�ʵ� ���� ����Ʈ
#ifdef SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
bool KBattleFieldUserManager::GetQuestDropItem( IN const int iBattleFieldID, IN const int iNPCID, OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo, IN float fQuestItemDropRate )
#else
bool KBattleFieldUserManager::GetQuestDropItem( IN const int iBattleFieldID, IN const int iNPCID, OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo )
#endif SERV_QUEST_ITEM_DROP_EVENT
//}
{
	SET_ERROR( NET_OK );

	vecDropInfo.clear();

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( IS_NULL( mit->second ) )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->IsDie() == false )
		{
			//����Ʈ ������ ����� ������..
			KRoomUser::QUEST_ITEM_DROP_INFO sDropInfo;
			//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
			if( mit->second->GetQuestDropItemInBattleField( iBattleFieldID, iNPCID, sDropInfo, fQuestItemDropRate ) == true )
#else
			if( mit->second->GetQuestDropItemInBattleField( iBattleFieldID, iNPCID, sDropInfo ) == true )
#endif SERV_QUEST_ITEM_DROP_EVENT
			//}
			{
				vecDropInfo.push_back( sDropInfo );
			}
		}
	}

	if( vecDropInfo.size() > 0 )
		return true;

	return false;
}
#endif SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
//}}
*/

//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
bool KBattleFieldUserManager::GetQuestDropItem( IN const UidType iUnitUID, IN const int iBattleFieldID, IN const int iNPCID, OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo, IN float fQuestItemDropRate )
#else
bool KBattleFieldUserManager::GetQuestDropItem( IN const UidType iUnitUID, IN const int iBattleFieldID, IN const int iNPCID, OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo )
#endif SERV_QUEST_ITEM_DROP_EVENT
//}
{
	SET_ERROR( NET_OK );

	vecDropInfo.clear();

	std::map< UidType, KRoomUserPtr >::iterator mit = m_mapRoomUser[UT_GAME].find( iUnitUID );
	if( mit != m_mapRoomUser[UT_GAME].end() )
	{
		if( IS_NULL( mit->second ) )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( mit->second->IsDie() == false )
		{
			//����Ʈ ������ ����� ������..
			KRoomUser::QUEST_ITEM_DROP_INFO sDropInfo;
			//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
			if( mit->second->GetQuestDropItemInBattleField( iBattleFieldID, iNPCID, fQuestItemDropRate, sDropInfo ) == true )
#else
			if( mit->second->GetQuestDropItemInBattleField( iBattleFieldID, iNPCID, sDropInfo ) == true )
#endif SERV_QUEST_ITEM_DROP_EVENT
			//}
			{
				vecDropInfo.push_back( sDropInfo );
			}
		}
	}

	if( vecDropInfo.size() > 0 )
		return true;

	return false;
}

float KBattleFieldUserManager::GetBattleFieldPartyBonusRate( IN const UidType& iUnitUID )
{
	int iPartyMemeber = GetSamePartyLiveMember( iUnitUID );

	return KRoomUserManager::GetPartyBonusRate( iPartyMemeber );
}

bool KBattleFieldUserManager::IsComeBackUserInParty( IN UidType iUnitUID )
{
	int iCount = 0;

	if( CheckUserlistType( UT_GAME ) == false )
		return false;

	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	if( spRoomUser->IsComeBackUser() == true )
		return true;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cwarn, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->IsDie() == true )
			continue;

		if( mit->second->GetPartyUID() != spRoomUser->GetPartyUID() )
			continue;

		if( spRoomUser->IsComeBackUser() == true)
		{
			return true;
		}
	}

	return false;
}
#endif SERV_REFORM_ITEM_DROP
//}}

float KBattleFieldUserManager::GetPartyTotalDamage( IN const UidType iUnitUID, 
												  IN const std::map< UidType, float>& mapDamageByUser, 
												  OUT std::vector<UidType>& vecPartyUserList, 
												  IN const USERLIST_TYPE eType /*= UT_GAME*/ )
{
	vecPartyUserList.clear();

	if( CheckUserlistType( eType ) == false )
	{
		std::map< UidType, float>::const_iterator mit = mapDamageByUser.find( iUnitUID );
		if( mit != mapDamageByUser.end() )
		{
			return mit->second;
		}

		return 0.f;
	}

	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		// �ð����� ���� ��Ż�ߴٸ� ������ �ƴϴ�.
		START_LOG( cwarn, L"�������� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return 0.f;
	}

	// party �� ����.
	if( spRoomUser->GetPartyUID() == 0 )
	{
		std::map< UidType, float>::const_iterator mit = mapDamageByUser.find( iUnitUID );
		if( mit != mapDamageByUser.end() )
		{
			return mit->second;
		}

		return 0.f;
	}

	float fTotalDamage = 0;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cwarn, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->GetPartyUID() == spRoomUser->GetPartyUID() )
		{
			std::map< UidType, float>::const_iterator mitDamage = mapDamageByUser.find( mit->first );
			if( mitDamage != mapDamageByUser.end() )
			{
				 fTotalDamage += mitDamage->second;
			}

			vecPartyUserList.push_back( mit->first );
		}
	}

	return fTotalDamage;
}

bool KBattleFieldUserManager::RequestTradeTo( UidType nCID, UidType nCIDTo )
{
    KRoomUserPtr spRoomUser = GetUser( nCID );
    if( !spRoomUser )
    {
        START_LOG( cwarn, L"������ ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    KRoomUserPtr spRoomUserTo = GetUser( nCIDTo );
    if( !spRoomUserTo )
    {
        START_LOG( cwarn, L"������ ����." )
            << BUILD_LOG( nCIDTo )
            << END_LOG;

        return false;
    }

    if( spRoomUser->GetStateID() != KRoomUserFSM::S_PLAY )
    {
        START_LOG( cwarn, L"��û�ϴ� ������ ���°� �̻���." )
            << BUILD_LOG( nCID )
            << BUILD_LOG( spRoomUser->GetStateID() )
            << BUILD_LOG( spRoomUser->GetStateIDString() )
            << END_LOG;

        return false;
    }

    if( spRoomUserTo->GetStateID() != KRoomUserFSM::S_PLAY )
    {
        START_LOG( cwarn, L"��û�޴� ������ ���°� �̻���." )
            << BUILD_LOG( nCIDTo )
            << BUILD_LOG( spRoomUserTo->GetStateID() )
            << BUILD_LOG( spRoomUserTo->GetStateIDString() )
            << END_LOG;

        return false;
    }

    return spRoomUser->RequestTradeTo( nCIDTo );
}

bool KBattleFieldUserManager::TradeAcceptedBy( UidType nCID, UidType nCIDAcceptor )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cwarn, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	KRoomUserPtr spRoomUserAcceptor = GetUser( nCIDAcceptor );
	if( !spRoomUserAcceptor )
	{
		START_LOG( cwarn, L"������ ����." )
			<< BUILD_LOG( nCIDAcceptor )
			<< END_LOG;

		return false;
	}

	if( spRoomUserAcceptor->GetStateID() != KRoomUserFSM::S_PLAY )
	{
		START_LOG( cwarn, L"��û�޴� ������ ���°� �̻���." )
			<< BUILD_LOG( nCIDAcceptor )
			<< BUILD_LOG( spRoomUserAcceptor->GetStateID() )
			<< BUILD_LOG( spRoomUserAcceptor->GetStateIDString() )
			<< END_LOG;

		return false;
	}

	return spRoomUser->TradeAcceptedBy( nCIDAcceptor );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
void KBattleFieldUserManager::CalculatePartyRank( IN const std::map< UidType, float >& mapDamageByUser, OUT std::map< UidType, bool >& mapBonusItem )
{
	// ��Ƽ ��(���� �̶�� ��Ƽ �ϳ��� �Ǵ��Ѵ�.)  < ��Ƽ ���� index, ��������Ʈ >
	std::map< int, std::set<UidType> > mapPartyUserList;

	// ��Ƽ ���п� �ӽ� ��
	int iPartyIndex = 1;	
	std::map< UidType, float >::const_iterator mitDamage = mapDamageByUser.begin();
	for( ; mitDamage != mapDamageByUser.end() ; ++mitDamage )
	{
		KRoomUserPtr spUser = GetUser( mitDamage->first );
		if( !spUser )
		{
			START_LOG( cwarn, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mitDamage->first )
				<< END_LOG;
			continue;
		}
	
		std::set<UidType> setUserList;

		// ���� ����
		if(spUser->GetPartyUID() == 0 )
		{
			setUserList.insert( spUser->GetCID() );
			mapPartyUserList.insert( std::make_pair( iPartyIndex++, setUserList ) );
			continue;
		}
		

		std::map< UidType, KRoomUserPtr >::iterator mit;
		for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
		{
			if( !mit->second )
			{
				START_LOG( cwarn, L"�� ���� ������ �̻�." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
				continue;
			}

			if( mit->second->GetPartyUID() == spUser->GetPartyUID() )
			{
				setUserList.insert( spUser->GetCID() );
			}
		}

		mapPartyUserList.insert( std::make_pair( iPartyIndex++, setUserList ) );
	}

	// ��Ƽ ���� index �� ���� ��������(���� �̶�� ��Ƽ �ϳ��� �Ǵ��Ѵ�.)  < ������, ��Ƽ ���� index >
	// ������ �� key �� �� ������ ���� 30% �� �����ϱ� ���� �ڵ� ���� ��Ŵ
	std::map< float, int, std::greater<float> > mapPartyDamage;

	std::map< int, std::set<UidType> >::iterator mitParty = mapPartyUserList.begin();
	for( ; mitParty != mapPartyUserList.end() ; ++mitParty )
	{
		float fTatalDamage = 0.f;
		std::set<UidType>::iterator sit = mitParty->second.begin();
		for( ; sit != mitParty->second.end() ; ++sit )
		{
			std::map< UidType, float >::const_iterator mitUser = mapDamageByUser.find( *sit );
			if( mitUser != mapDamageByUser.end() )
			{
				fTatalDamage += mitUser->second;
			}
		}
	
		mapPartyDamage.insert( std::make_pair( fTatalDamage, mitParty->first ) );
    }

	// �׷� �� ���� ���ؾ���
	int iTotalPartyCount = static_cast<int>(mapPartyUserList.size());
	int iTopRankPartyCount = static_cast<int>(iTotalPartyCount * 0.3);						// ���� 30%�� ��Ƽ ��

	std::map< float, int, std::greater<float> >::iterator mitBonus = mapPartyDamage.begin();
	for( int iCount = 0 ; mitBonus != mapPartyDamage.end(); ++iCount, ++mitBonus )
	{
		if( iCount < iTopRankPartyCount )
		{
			std::map< int, std::set<UidType> >::iterator mitParty = mapPartyUserList.find( mitBonus->second );
			if( mitParty != mapPartyUserList.end() )
			{
				std::set<UidType>::iterator sitUnit = mitParty->second.begin();
				for( ; sitUnit != mitParty->second.end() ; ++sitUnit )
				{
					mapBonusItem.insert( std::make_pair( *sitUnit, true ) );
				}
			}			
		}
		else
		{
			std::map< int, std::set<UidType> >::iterator mitParty = mapPartyUserList.find( mitBonus->second );
			if( mitParty != mapPartyUserList.end() )
			{
				std::set<UidType>::iterator sitUnit = mitParty->second.begin();
				for( ; sitUnit != mitParty->second.end() ; ++sitUnit )
				{
					mapBonusItem.insert( std::make_pair( *sitUnit, false ) );
				}
			}
		}		
	}
}

void KBattleFieldUserManager::GetPartyLevelFactor( IN const UidType iUnitUID, IN char cLevel, OUT float& fPartyLevelFactorMin, OUT float& fPartyLevelFactorMax )
{
	fPartyLevelFactorMin = 0.f;
	fPartyLevelFactorMax = 0.f;

	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		// �ð����� ���� ��Ż�ߴٸ� ������ �ƴϴ�.
		START_LOG( cwarn, L"�������� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return;
	}

	// party �� ����.
	if( spRoomUser->GetPartyUID() == 0 )
	{
		fPartyLevelFactorMin = CXSLDungeon::GetMonsterLevelBalanceRate( spRoomUser->GetLevel(), cLevel );
		fPartyLevelFactorMax = fPartyLevelFactorMin;
		return;
	}

	fPartyLevelFactorMin = CXSLDungeon::GetMonsterLevelBalanceRate( spRoomUser->GetLevel(), cLevel );
	fPartyLevelFactorMax = fPartyLevelFactorMin;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cwarn, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->GetPartyUID() == spRoomUser->GetPartyUID() )
		{
			float fUserLevelFactor = CXSLDungeon::GetMonsterLevelBalanceRate( mit->second->GetLevel(), cLevel );

			fPartyLevelFactorMin = ( fUserLevelFactor < fPartyLevelFactorMin ) ? fUserLevelFactor : fPartyLevelFactorMin;
			fPartyLevelFactorMax = ( fUserLevelFactor > fPartyLevelFactorMax ) ? fUserLevelFactor : fPartyLevelFactorMax;
		}
	}

	return;
}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
//}


#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
void KBattleFieldUserManager::ZombieAlert_CheckStart()
{
	USERLIST_TYPE eType = UT_GAME;

	START_LOG( clog, L"�������� ALERT üũ����.!" );

	//�������� üũ �����ϱ��� �ð� �ʱ�ȭ
	for( int i = UT_GAME; i < UT_TOTAL_NUM; ++i )
	{
		USERLIST_TYPE eType = (USERLIST_TYPE)i;

		std::map< UidType, KRoomUserPtr >::iterator mit;
		for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
		{
			if( !mit->second )
			{
				START_LOG( cerr, L"�� ���� ������ �̻�." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
				continue;
			}

			//{{ 2011. 07. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
			if( mit->second->IsPvpNpc() == true )
				continue;
#endif SERV_PVP_NEW_SYSTEM
			//}}

            mit->second->ZombieAlert_Set( true );
            mit->second->ZombieAlert_Refresh();
		}
	}

	//�������� üũ�� ����
	m_bIsCheckZombieAlert = true;
}


bool KBattleFieldUserManager::ZombieAlert_CheckStart(  IN const UidType nCID )
{
	if( ZombieAlert_IsCheck() == false )
		return false;

	KRoomUserPtr spRoomUser = GetUser( nCID, UT_GAME );
	if( !spRoomUser )
	{
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

			//{{ 2011. 07. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	if( spRoomUser->IsPvpNpc() == true )
        return false;
#endif SERV_PVP_NEW_SYSTEM
			//}}

    spRoomUser->ZombieAlert_Set( true );
    spRoomUser->ZombieAlert_Refresh();
	return true;
}


void KBattleFieldUserManager::ZombieAlert_CheckEnd()
{
	START_LOG( clog, L"�������� ALERT üũ����.!" );

	//�������� üũ ����.
	m_bIsCheckZombieAlert = false;
}

bool KBattleFieldUserManager::ZombieAlert_Tick()
{
	if( ZombieAlert_IsCheck() == false )
		return false;

	USERLIST_TYPE eType = UT_GAME;

    UidType cidHost = 0;
    bool    bHostFound = false;
    bool    bHostAlive = false;
    bool    bNonHostAlive = false;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			continue;
		}

		//{{ 2011. 07. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		if( mit->second->IsPvpNpc() == true )
			continue;
#endif SERV_PVP_NEW_SYSTEM
		//}}

        mit->second->ZombieAlert_Tick();
        bool    bNewZombieAlert = mit->second->ZombieAlert_Get();
        if ( mit->second->IsHost() == true )
        {
            bHostFound = true;
            cidHost = mit->second->GetCID();
            bHostAlive = mit->second->IsPlaying() == true && bNewZombieAlert == false;
        }
        else if ( bNonHostAlive == false )
        {
            bNonHostAlive = mit->second->IsPlaying() == true && bNewZombieAlert == false;
        }
	}

    if ( bHostFound == true
#ifdef  SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
        && GetForceHostCID() != cidHost
#endif  SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
        && bHostAlive == false && bNonHostAlive == true )
    {
        return true;
    }

	return false;
}

#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-15	// �ڼ���
void KBattleFieldUserManager::GetUnitListGroupdByParty( OUT std::vector< std::set< UidType > >& vecUnitListGroupdByParty ) const
{
	const std::map<UidType, KRoomUserPtr>& mapRoomUser = m_mapRoomUser[UT_GAME];
	std::set<UidType> setChecker;

	for( std::map<UidType, KRoomUserPtr>::const_iterator it = mapRoomUser.begin(); it != mapRoomUser.end(); ++it )
	{
		if( it->second == NULL )
		{
			START_LOG( cwarn, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( it->first )
				<< END_LOG;
			continue;
		}

		if( setChecker.find( it->second->GetCID() ) != setChecker.end() )
			continue;

		const UidType		iPartyUID = it->second->GetPartyUID();
		std::set<UidType>	setUnitList;

		if( 0 < iPartyUID )
		{
			for( std::map<UidType, KRoomUserPtr>::const_iterator itTemp = mapRoomUser.begin(); itTemp != mapRoomUser.end(); ++itTemp )
			{
				if( iPartyUID == itTemp->second->GetPartyUID() )
				{
					const UidType iUnitUID = itTemp->second->GetCID();
					setUnitList.insert( iUnitUID );
					setChecker.insert( iUnitUID );
				}
			}
		}
		else
		{
			const UidType iUnitUID = it->second->GetCID();
			setUnitList.insert( iUnitUID );
			setChecker.insert( iUnitUID );
		}

		vecUnitListGroupdByParty.push_back( setUnitList );
	}
}
#endif // SERV_BATTLE_FIELD_BOSS