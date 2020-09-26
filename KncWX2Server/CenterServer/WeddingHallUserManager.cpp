#include "WeddingHallUserManager.h"
#include "Enum/Enum.h"
#include "X2Data/XSLDungeon.h"
#include "NetError.h"
#include "Room/RoomUserFSM.h"


//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM


KWeddingHallUserManager::KWeddingHallUserManager() :
m_MaxSlot( SEnum::WEDDING_HALL_MAX_NUM )
{
}

KWeddingHallUserManager::~KWeddingHallUserManager()
{
}

//{{ 2013. 01. 07	������	�濡�� �ʵ�� ���ƿ��� ���ϴ� ���� ����
void KWeddingHallUserManager::Reset( USERLIST_TYPE eType /*= UT_GAME*/ )
{
	KRoomUserManager::Reset( eType );
}
//}}


bool KWeddingHallUserManager::EnterRoom( IN const KRoomUserInfo& kInfo, IN const bool bConsiderTeam )
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
	LIF( LeaveRoom( kInfo.m_nUnitUID ) );
	return false;
}

bool KWeddingHallUserManager::LeaveRoom( IN const UidType iUnitUID )
{
	return KRoomUserManager::LeaveRoom( iUnitUID );
}

bool KWeddingHallUserManager::RegIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const std::vector< UidType >& vecUnitUIDList )
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

bool KWeddingHallUserManager::RegIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const UidType iRegUnitUID )
{
	std::vector< UidType > vecReg;
	vecReg.push_back( iRegUnitUID );
    return RegIntrudeUnitLoadUserList( iUnitUID, vecReg );
}

bool KWeddingHallUserManager::UnregIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const std::vector< UidType >& vecUnitUIDList )
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

bool KWeddingHallUserManager::UnregIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const UidType iUnregUnitUID )
{
	std::vector< UidType > vecUnreg;
	vecUnreg.push_back( iUnregUnitUID );
	return UnregIntrudeUnitLoadUserList( iUnitUID, vecUnreg );
}

void KWeddingHallUserManager::CheckAndGetIntrudeUnitLoadCompleteUserList( OUT std::vector< UidType >& vecNpcLoadUserList )
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

	}
}

bool KWeddingHallUserManager::GetRoomUserList( std::map< UidType, KRoomUserPtr >& mapRoomUserList, USERLIST_TYPE eType /*= UT_GAME*/ )
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

void KWeddingHallUserManager::GetRoomUserList( OUT std::vector< UidType >& vecUidList, USERLIST_TYPE eType /*= UT_GAME*/ )
{
	if( CheckUserlistType( eType ) == false )
		return;

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

		vecUidList.push_back( mit->first );
	}
}

void KWeddingHallUserManager::GetRoomSlotInfo( std::vector< KRoomSlotInfo >& vecRoomSlotInfo, USERLIST_TYPE eType /*= UT_GAME*/ )
{
	if( CheckUserlistType( eType ) == false )
	{
		START_LOG( cerr, L"��������Ʈ Ÿ�� �̻�.!" )
			<< BUILD_LOG( (int)eType )
			<< END_LOG;
		return;
	}

	vecRoomSlotInfo.clear();

	for( int i = 0; i < GetNumMember() ; i++ )
	{
		KRoomSlotInfo kInfo;
		KRoomUserManager::GetRoomSlotInfo( i, kInfo, eType );

		vecRoomSlotInfo.push_back( kInfo );
	}
}

bool KWeddingHallUserManager::IsExistGroomAndBride( USERLIST_TYPE eType /*= UT_GAME*/ )
{
	bool IsExistGroom = false;
	bool IsExistBride = false;

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

		if( mit->second->GetWeddingStatus() == SEnum::WS_GROOM )
		{
			if( IsExistGroom == true )
			{
				// �Ŷ��� �Ѹ��� ����?!
				START_LOG( cerr, L"�Ŷ��� �Ѹ��� �ƴϴ�?!" )
					<< END_LOG;
			}

			IsExistGroom = true;
		}
		else if( mit->second->GetWeddingStatus() == SEnum::WS_BRIDE )
		{
			if( IsExistBride == true )
			{
				// �ź��� �Ѹ��� ����?!
				START_LOG( cerr, L"�ź��� �Ѹ��� �ƴϴ�?!" )
					<< END_LOG;
			}

			IsExistBride = true;
		}			
	}

	return ( IsExistGroom == true && IsExistBride == true );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}
