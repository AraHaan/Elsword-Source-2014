#include "BaseServer.h"
#include ".\battlefieldlist.h"
#include "Enum\Enum.h"
#include "X2Data\XSLRoom.h"
#include "Lottery.h"
#include "NetError.h"


//{{ 2011. 11. 2	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


KBattleFieldList::KBattleFieldList()
{
}

KBattleFieldList::~KBattleFieldList(void)
{
}

KGSBattleFieldRoomPtr KBattleFieldList::GetRoomInfo( IN const UidType iRoomUID, IN const bool bErrorLog /*= false*/ )
{
	std::map< UidType, KGSBattleFieldRoomPtr >::iterator mit;
	mit = m_mapList.find( iRoomUID );
	if( mit == m_mapList.end() )
	{
		if( bErrorLog )
		{
			START_LOG( cerr, L"�� ������ �������� �ʽ��ϴ�." )
				<< BUILD_LOG( iRoomUID )
				<< END_LOG;
		}

		return KGSBattleFieldRoomPtr();
	}

	return mit->second;
}

bool KBattleFieldList::AddRoomInfo( IN const KBattleFieldRoomInfo& kInfo )
{
	if( IsExist( kInfo.m_RoomUID ) == true )
	{
		START_LOG( cerr, L"�̹� �����ϴ� ���Դϴ�" )
			<< BUILD_LOG( kInfo.m_RoomUID )
			<< END_LOG;
		return false;
	}

	KGSBattleFieldRoomPtr spRoomInfo( new KGSBattleFieldRoom( kInfo ) );
	if( spRoomInfo == NULL )
	{
		START_LOG( cerr, L"������ �����Ͱ� ���� ����!" )
			<< BUILD_LOG( kInfo.m_iBattleFieldID )
			<< END_LOG;
		return false;
	}

	m_mapList.insert( std::make_pair( kInfo.m_RoomUID, spRoomInfo ) );
	m_vecList.push_back( spRoomInfo );
	return true;
}

bool KBattleFieldList::DelRoomInfo( IN const UidType iRoomUID )
{
	if( IsExist( iRoomUID ) == false )
	{
		START_LOG( cerr, L"���� ���� ������� �Ͽ����ϴ�" )
			<< BUILD_LOG( iRoomUID )
			<< END_LOG;
		return false;
	}

	m_mapList.erase( iRoomUID );

	std::vector< KGSBattleFieldRoomPtr >::iterator vitDEL;
	for( vitDEL = m_vecList.begin(); vitDEL != m_vecList.end(); ++vitDEL )
	{
        if( (*vitDEL) == NULL )
		{
			START_LOG( cerr, L"�� �����Ͱ��� �̻��մϴ�!" )
				<< BUILD_LOG( iRoomUID )
				<< END_LOG;
			continue;
		}

		if( (*vitDEL)->GetRoomUID() == iRoomUID )
			break;
	}

	if( vitDEL != m_vecList.end() )
	{
		m_vecList.erase( vitDEL );
	}

	return true;
}

//{{ 2012. 11. 20	������		��Ʋ�ʵ� �� ���� ��� �߰�
#ifdef SERV_BATTLE_FIELD_ROOM_CLEAR_AT_CN_PROXY_DIE
void KBattleFieldList::DelRoomInfoByCnServerUID( IN const UidType iCnServerUID )
{
	std::vector< UidType > vecDeleteRoomUID;

	BOOST_TEST_FOREACH( const KGSBattleFieldRoomPtr, spGSBattleFieldRoom, m_vecList )
	{
		if( IS_NULL( spGSBattleFieldRoom ) )
		{
			START_LOG( cerr, L"�� �����Ͱ��� �̻��մϴ�!" )
				<< BUILD_LOG( iCnServerUID )
				<< END_LOG;
			continue;
		}

		// �ش� ���Ϳ��� ������� ��Ʋ�ʵ尡 �´��� Ȯ��!
		if( KncUid::ExtractServerID( spGSBattleFieldRoom->GetRoomUID() ) == iCnServerUID )
		{
            vecDeleteRoomUID.push_back( spGSBattleFieldRoom->GetRoomUID() );
		}
	}

	BOOST_TEST_FOREACH( const UidType, iRoomUID, vecDeleteRoomUID )
	{
		LIF( DelRoomInfo( iRoomUID ) );
	}
}
#endif SERV_BATTLE_FIELD_ROOM_CLEAR_AT_CN_PROXY_DIE
//}}

bool KBattleFieldList::UpdateRoomInfo( IN const KBattleFieldRoomInfo& kInfo )
{
    KGSBattleFieldRoomPtr spRoomInfo = GetRoomInfo( kInfo.m_RoomUID );
	if( spRoomInfo == NULL )
	{
		START_LOG( cerr, L"�� ���� �����Ͱ��� �̻��մϴ�" )
			<< BUILD_LOG( kInfo.m_RoomUID )
			<< END_LOG;
		return false;
	}

	(*spRoomInfo) = kInfo;
	return true;
}

UidType KBattleFieldList::GetBattleFieldRoomUID_MaxPartyMemberCount( IN const UidType iUnitUID, IN const UidType iPartyUID )
{
	if( iPartyUID == 0 )
	{
		START_LOG( cerr, L"��ƼUID���� 0�̴�. �̷��� �̸� �˻� �ؾ���!" )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;
		return 0;
	}

	UidType iSelectedRoomUID = 0;
	int iMaxPartyMemberCount = 0;

	// ��� �ʵ忡 �ش� ��Ƽ���� ���� ���� �ʵ带 ��������
	BOOST_TEST_FOREACH( const KGSBattleFieldRoomPtr, spRoomInfo, m_vecList )
	{
		if( IS_NULL( spRoomInfo ) )
		{
			START_LOG( cerr, L"�� �����Ͱ��� �̻��մϴ�" )
				<< END_LOG;
			continue;
		}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-06	// �ڼ���
		if( SEnum::CheckFlag( spRoomInfo->GetBossFieldState(), ( SEnum::BFRS_BOSS_FIELD | SEnum::BFRS_INTRUDE_RESTRICTION ) ) == true )
			continue;
#endif // SERV_BATTLE_FIELD_BOSS

		// �ش� ��Ʋ�ʵ忡 �������ִ� ���� ��Ƽ���� �ִ��� Ȯ��!
		if( spRoomInfo->IsExistParty( iPartyUID ) == false )
			continue;

		int iPartyMemberCount = 0;

		// �ش� �ʵ忡 �ڱ� �ڽ��� �������ִ��� Ȯ��!
		if( spRoomInfo->IsJoinUser( iUnitUID ) == true )
		{
			// ������ ���� ������ �������� Ȯ��!
			if( ( spRoomInfo->GetJoinUserCount() - 1 ) >= spRoomInfo->GetMaxUserCount() )
				continue;

			// �� �ʵ忡 ������ ��Ƽ���� ����ΰ�?
			iPartyMemberCount = ( spRoomInfo->GetPartyMemberCountByPartyUID( iPartyUID ) - 1 );
			if( iMaxPartyMemberCount >= iPartyMemberCount )
				continue;
		}
		else
		{
			// ������ ���� ������ �������� Ȯ��! ( ������ ����־���� )
			if( spRoomInfo->GetJoinUserCount() >= spRoomInfo->GetMaxUserCount() )
				continue;

			// �� �ʵ忡 ������ ��Ƽ���� ����ΰ�?
			iPartyMemberCount = spRoomInfo->GetPartyMemberCountByPartyUID( iPartyUID );
			if( iMaxPartyMemberCount >= iPartyMemberCount )
				continue;
		}

		iSelectedRoomUID = spRoomInfo->GetRoomUID();
		iMaxPartyMemberCount = iPartyMemberCount;
	}

	return iSelectedRoomUID;
}

UidType KBattleFieldList::GetBattleFieldRoomUID_TargetRoom( IN const UidType iTargetRoomUID )
{
	if( iTargetRoomUID == 0 )
	{
		return 0;
	}

	if( CXSLRoom::GetRoomType( iTargetRoomUID ) != CXSLRoom::RT_BATTLE_FIELD )
	{
		// ��ǥ�Ǵ� ���� ��Ʋ�ʵ尡 �ƴϴ�.
		return 0;
	}

	KGSBattleFieldRoomPtr spBeforeRoomInfo = GetRoomInfo( iTargetRoomUID );
	if( IS_NULL( spBeforeRoomInfo ) )
	{
		// ���� �������� �ʴ´�. �ð����� ��������� �ִ�.
        return 0;
	}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-06	// �ڼ���
	if( SEnum::CheckFlag( spBeforeRoomInfo->GetBossFieldState(), ( SEnum::BFRS_BOSS_FIELD | SEnum::BFRS_INTRUDE_RESTRICTION ) ) == true )
		return 0;
#endif // SERV_BATTLE_FIELD_BOSS

	if( spBeforeRoomInfo->GetJoinUserCountWithReservedUserCount() >= spBeforeRoomInfo->GetMaxUserCount() )
	{
		// ������ �� ������ŭ �� �ڸ��� ����.
		return 0;
	}

	return spBeforeRoomInfo->GetRoomUID();
}

UidType KBattleFieldList::GetBattleFieldRoomUID_AllRandom()
{
	KLotterySameProb< UidType > kLotRandomBF;

	// �׳� �ٸ�������� �ִ� ���� �ϳ� ��� ����!
	BOOST_TEST_FOREACH( const KGSBattleFieldRoomPtr, spRoomInfo, m_vecList )
	{
		if( IS_NULL( spRoomInfo ) )
		{
			START_LOG( cerr, L"�� �����Ͱ��� �̻��մϴ�" )
				<< END_LOG;
			continue;
		}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-06	// �ڼ���
		if( SEnum::CheckFlag( spRoomInfo->GetBossFieldState(), ( SEnum::BFRS_BOSS_FIELD | SEnum::BFRS_INTRUDE_RESTRICTION ) ) == true )
			continue;
#endif // SERV_BATTLE_FIELD_BOSS

		// ���� ��ȿ� �ִ� ���� + �� ������ ��Ƽ�� ��� �ش� �Ҽ� ��Ƽ�� ���� �ջ�
		if( spRoomInfo->GetJoinUserCountWithReservedUserCount() >= spRoomInfo->GetMaxUserCount() )
			continue;

		// ������ Ȯ���� ������ ������ ���� case�� �����Ѵ�.
		kLotRandomBF.AddCaseSameProb( spRoomInfo->GetRoomUID() );
	}

	if( kLotRandomBF.Size() == 0 )
	{
		// ������ �������� ���� ���� ����.
		return 0;
	}

	// �����ϰ� ������ ��Ʋ�ʵ带 �����Ѵ�!
	const UidType iJoinBFRoomUID = kLotRandomBF.DecisionSameProb();
	if( iJoinBFRoomUID == kLotRandomBF.CASE_BLANK )
	{
		// ���� ������� BLANK�� ���Դ�.
		return 0;
	}

	return iJoinBFRoomUID;
}

bool KBattleFieldList::GetRoomUIDForJoinBattleField( IN const UidType iUnitUID, IN const UidType iTargetRoomUID, IN const UidType iPartyUID, OUT UidType& iJoinBFRoomUID )
{
	SET_ERROR( NET_OK );

	iJoinBFRoomUID = 0;

	// ��Ƽ �Ҽ��� ���
	if( iPartyUID != 0 )
	{
		// ���� ��Ƽ���� ���� ���� ã�´�!
		iJoinBFRoomUID = GetBattleFieldRoomUID_MaxPartyMemberCount( iUnitUID, iPartyUID );
	}

	// ������ ������ �ʵ尡 �������� �ʾҴٸ�
	if( iJoinBFRoomUID == 0 )
	{
		// ������ �����߾��� �濡 �ٽ� �����ϱ� �������� ����.
		iJoinBFRoomUID = GetBattleFieldRoomUID_TargetRoom( iTargetRoomUID );
	}
		
	// ���������� ������ ��Ʋ�ʵ尡 �������� �ʾҴٸ�
	if( iJoinBFRoomUID == 0 )
	{
		// �ٸ� �ʵ���� ��� �˻��ؼ� ������ ���� ã��!
        iJoinBFRoomUID = GetBattleFieldRoomUID_AllRandom();
	}

	return true;
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}


