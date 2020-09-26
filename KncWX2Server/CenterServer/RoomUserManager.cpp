#include "RoomUserManager.h"

#include "X2Data/XSLRoom.h"
#include "X2data/XSLDungeonManager.h"
#include "X2Data/XSLItemManager.h"
#include "Room/RoomSlotFSM.h"
#include "Room/RoomUserFSM.h"
#include "Enum/Enum.h"
#include "ResultProcess.h"
#include "NetError.h"
//{{ 2011. 07. 21	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch/PvpMatchManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}

#if defined(SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE) || defined(SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK)// �۾���¥: 2013-05-08	// �ڼ���
    #include "GameSysVal/GameSysVal.h"
#endif 

FSMclassPtr KRoomUserManager::ms_spSlotFSM = KRoomSlotFSMPtr( new KRoomSlotFSM );
FSMclassPtr KRoomUserManager::ms_spUserFSM = KRoomUserFSMPtr( new KRoomUserFSM );

KRoomUserManager::KRoomUserManager()
:
//{{ 2011. 07. 25	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
m_iRedTeamRating( 0 )
, m_iBlueTeamRating( 0 )
#endif SERV_PVP_NEW_SYSTEM
//}}
, m_eRoomType( CXSLRoom::RT_TOTAL_NUM )
//{{ 2012. 06. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
, m_iSendReturnToFieldUnitUID( 0 )
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
, m_bIsCheckZU( false )
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-16	// �ڼ���
, m_byteStartPlayMemberNum( 0 )
#endif // SERV_HENIR_RENEWAL_2013
{
#ifdef  SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
    m_uidForceHostCID = 0;
#endif  SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
}

KRoomUserManager::~KRoomUserManager()
{
}


void KRoomUserManager::Init( IN const CXSLRoom::ROOM_TYPE eRoomType, IN const int nRoomSlot, IN const int nRoomObserverSlot /*= 3*/ )
{
	m_eRoomType = eRoomType;
	m_nRoomSlot[UT_GAME] = nRoomSlot;
	m_nRoomSlot[UT_OBSERVER] = nRoomObserverSlot;

	int iTypeCnt = 0;
	int iCnt = 0;
	//���� & ������ ���Ի���.
	for( iTypeCnt = UT_GAME; iTypeCnt < UT_TOTAL_NUM; ++iTypeCnt )
	{
		for( iCnt = 0; iCnt < m_nRoomSlot[iTypeCnt]; iCnt++ )
		{
			MakeSlot( iCnt, static_cast<USERLIST_TYPE>(iTypeCnt) );
		}
	}
}

void KRoomUserManager::Reset( USERLIST_TYPE eType /* = UT_GAME  */)
{
    for( int i = 0; i < m_nRoomSlot[eType]; i++ )
    {
        KRoomSlotPtr spRoomSlot = GetSlot( i, eType );
        if( !spRoomSlot )
        {
            START_LOG( cerr, L"���� ������ �̻�. ���� �Ͼ �� ���� ����." )
                << END_LOG;

            continue;
        }

        spRoomSlot->ResetSlot();
    }

    m_mapRoomUser[eType].clear();
#ifdef  SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
    m_uidForceHostCID = 0;
#endif  SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
    //{{ 2013. 01. 07	������	�濡�� �ʵ�� ���ƿ��� ���ϴ� ���� ����
#ifdef SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
	m_mapTeamNumKill.clear();
	//{{ 2011. 03. 08	��μ� ����ġ�� ȹ���� ������ ����Ʈ �Ϸ� ���� ����
//#ifdef SERV_EXP_ACQUISITION_CONDITION_CHANGE
	m_mapHaveExpInDungeon.clear();
//#endif SERV_EXP_ACQUISITION_CONDITION_CHANGE
	//}}
	//{{ 2011. 07. 25	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
	m_iRedTeamRating = 0;
	m_iBlueTeamRating = 0;
//#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2012. 06. 07	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_mapReturnToFieldRemainUnitUID.clear();
	m_iSendReturnToFieldUnitUID = 0;
//#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
//#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	m_mapSuitableLevelInfo.clear();
//#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	//}}
#endif SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
	//}}

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-16	// �ڼ���
	m_byteStartPlayMemberNum = 0;
#endif // SERV_HENIR_RENEWAL_2013
}

KRoomSlotPtr KRoomUserManager::GetSlot( int iSlotID, USERLIST_TYPE eType /* = UT_GAME  */)
{
    if( iSlotID < 0 || iSlotID >= m_nRoomSlot[eType] )
    {
        START_LOG( cerr, L"���� ID �̻�." )
            << BUILD_LOG( iSlotID )
            << BUILD_LOG( m_nRoomSlot[eType] )
            << END_LOG;

        return KRoomSlotPtr();
    }

    if( !m_vecRoomSlot[eType][iSlotID] )
    {
        START_LOG( cerr, L"���� ������ �̻�." )
            << END_LOG;

        HandleInvalidSlotPointer( iSlotID, eType );
    }

    return m_vecRoomSlot[eType][iSlotID];
}

KRoomUserPtr KRoomUserManager::GetUser( UidType nCID, USERLIST_TYPE eType /* = UT_GAME  */)
{
    std::map< UidType, KRoomUserPtr >::iterator mit;
    mit = m_mapRoomUser[eType].find( nCID );

    if( mit == m_mapRoomUser[eType].end() )
    {
        START_LOG( cwarn, L"�������� �ʴ� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return KRoomUserPtr();
    }

    return mit->second;
}

KRoomUserPtr KRoomUserManager::GetUser( int nIndex, USERLIST_TYPE eType /* = UT_GAME  */)
{
	if( (int)m_mapRoomUser[eType].size() <= nIndex )
	{
		START_LOG( cerr, L"�������� �ʴ� ����." )
			<< BUILD_LOG( nIndex )
			<< END_LOG;

		return KRoomUserPtr();
	}

	std::map< UidType, KRoomUserPtr >::iterator mit;
	int i = 0;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
	{
		if( i == nIndex )
		{
			if( !mit->second )
			{
				START_LOG( cerr, L"�� ���� ������ �̻�." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;

				return KRoomUserPtr();
			}

			return mit->second;
		}

		i++;
	}

	return KRoomUserPtr();
}

bool KRoomUserManager::AddUser( KRoomUserPtr spRoomUser, USERLIST_TYPE eType /* = UT_GAME  */)
{
    if( !spRoomUser )
    {
        START_LOG( cerr, L"���� ������ �̻�." )
            << END_LOG;

        return false;
    }

	std::map< UidType, KRoomUserPtr >::iterator mit;
	mit = m_mapRoomUser[eType].find( spRoomUser->GetCID() );

	if( mit != m_mapRoomUser[eType].end() )
	{
		START_LOG( cerr, L"�̹� �����ϴ� ��������." )
			<< BUILD_LOG( spRoomUser->GetCID() )
			<< END_LOG;

		return false;
	}

	m_mapRoomUser[eType].insert( std::make_pair( spRoomUser->GetCID(), spRoomUser ) );
	//}

    return true;
}

bool KRoomUserManager::DeleteUser( UidType nCID, USERLIST_TYPE eType /* = UT_GAME  */)
{
    std::map< UidType, KRoomUserPtr >::iterator mit;
    mit = m_mapRoomUser[eType].find( nCID );

    if( mit == m_mapRoomUser[eType].end() )
    {
        START_LOG( cerr, L"�������� �ʴ� ��������." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return true;
    }
#ifdef  SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
    if ( m_uidForceHostCID  == nCID )
        m_uidForceHostCID = 0;
#endif  SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
    m_mapRoomUser[eType].erase( mit );

    return true;
}

bool KRoomUserManager::GetGSUID( IN UidType nCID, OUT UidType& nGSUID, USERLIST_TYPE eType /* = UT_GAME  */)
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cwarn, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		SET_ERROR( ERR_ROOM_02 );

		return false;
	}

	nGSUID = spRoomUser->GetGSUID();

	return true;
}

int KRoomUserManager::DeleteUserByGsUID( IN UidType nGsUID, OUT std::vector<KLeaveUserInfo>& vecDelUserUID, USERLIST_TYPE eType /* = UT_GAME  */)
{
    int iCount = 0;

    std::map<UidType, KRoomUserPtr>::iterator mit;
    for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
    {	
        if( mit->second->GetGSUID() == nGsUID )
        {
			//������ user�� uid�� ��´�.
			KLeaveUserInfo kInfo;
			kInfo.m_iUnitUID = mit->second->GetCID();
			kInfo.m_iPartyUID = mit->second->GetPartyUID();
			vecDelUserUID.push_back( kInfo );
            iCount++;
        }
    }

    return iCount;
}


int KRoomUserManager::GetNumTotalSlot( USERLIST_TYPE eType /* = UT_GAME  */)
{
	if( CheckUserlistType( eType ) == false )
		return 0;

    return m_nRoomSlot[eType];
}

int KRoomUserManager::GetNumOpenedSlot( USERLIST_TYPE eType /* = UT_GAME  */)
{
    int iCount = 0;

	if( CheckUserlistType( eType ) == false )
		return iCount;

    for( int i = 0; i < m_nRoomSlot[eType]; i++ )
    {
        KRoomSlotPtr spRoomSlot = GetSlot( i, eType );
        if( spRoomSlot )
        {
            if( spRoomSlot->IsOpened() )
            {
                iCount++;
            }
        }
    }

    return iCount;
}

int KRoomUserManager::GetNumOccupiedSlot( USERLIST_TYPE eType /* = UT_GAME  */)
{
    int iCount = 0;

	if( CheckUserlistType( eType ) == false )
		return iCount;

    for( int i = 0; i < m_nRoomSlot[eType]; i++ )
    {
        KRoomSlotPtr spRoomSlot = GetSlot( i, eType );
        if( spRoomSlot )
        {
            if( spRoomSlot->IsOccupied() )
            {
                iCount++;
            }
        }
    }

    return iCount;
}

int KRoomUserManager::GetNumMember( USERLIST_TYPE eType /* = UT_GAME  */)
{
    int iCount = 0;

	if( CheckUserlistType( eType ) == false )
		return iCount;

    std::map< UidType, KRoomUserPtr >::iterator mit;
    for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"�� ���� ������ �̻�." )
                << BUILD_LOG( mit->first )
                << END_LOG;
        }
        
        iCount++;
    }

    if( iCount != GetNumOccupiedSlot(eType) )
    {
        START_LOG( cerr, L"���� ���� ���� ���� ������ �ִ� ������ ������ �ٸ�." )
            << BUILD_LOG( iCount )
            << BUILD_LOG( GetNumOccupiedSlot(eType) )
            << END_LOG;
    }

    return iCount;
}

int KRoomUserManager::GetNumPlaying( USERLIST_TYPE eType /* = UT_GAME  */)
{
    int iCount = 0;

	if( CheckUserlistType( eType ) == false )
		return iCount;

    std::map< UidType, KRoomUserPtr >::iterator mit;
    for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"�� ���� ������ �̻�." )
                << BUILD_LOG( mit->first )
                << END_LOG;
            continue;
        }

        if( mit->second->IsPlaying() )
        {
            iCount++;
        }
    }

    return iCount;
}

int KRoomUserManager::GetTeamNumPlaying(CXSLRoom::TEAM_NUM eTeamNum, USERLIST_TYPE eType /* = UT_GAME  */)
{
	int iCount = 0;

	if( CheckUserlistType( eType ) == false )
		return iCount;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if(  mit->second->GetTeam() == eTeamNum && mit->second->IsPlaying() )
		{
			iCount++;
		}
	}

	return iCount;
}

int KRoomUserManager::GetNumResultPlayer( USERLIST_TYPE eType /* = UT_GAME  */)
{
    int iCount = 0;

	if( CheckUserlistType( eType ) == false )
		return iCount;

    std::map< UidType, KRoomUserPtr >::iterator mit;
    for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"�� ���� ������ �̻�." )
                << BUILD_LOG( mit->first )
                << END_LOG;
            continue;
        }

        if( mit->second->GetStateID() == KRoomUserFSM::S_RESULT )
        {
            iCount++;
        }
    }

    return iCount;
}

int KRoomUserManager::GetNumReadyPlayer( USERLIST_TYPE eType /* = UT_GAME  */)
{
    int iCount = 0;

	if( CheckUserlistType( eType ) == false )
		return iCount;

    std::map< UidType, KRoomUserPtr >::iterator mit;
    for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"�� ���� ������ �̻�." )
                << BUILD_LOG( mit->first )
                << END_LOG;
            continue;
        }

        if( mit->second->IsReady() )
        {
            iCount++;
        }
    }

    return iCount;
}

int KRoomUserManager::GetLiveMember( USERLIST_TYPE eType /* = UT_GAME  */)
{
	int iCount = 0;

	if( CheckUserlistType( eType ) == false )
		return iCount;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
	{
		if( !mit->second )
		{
			START_LOG( cwarn, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->IsDie() == false )
		{
			iCount++;
		}
	}

	if( iCount <= 0 )
		iCount = 1;

	return iCount;
}

//{{ 2013. 04. 10	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
int KRoomUserManager::GetNumMemberOverLevel( IN const char cLevel, IN USERLIST_TYPE eType /*= UT_GAME*/ )
{
	int iCount = 0;

	if( CheckUserlistType( eType ) == false )
		return iCount;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( IS_NULL( mit->second ) )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->GetLevel() < cLevel )
			continue;

		++iCount;
	}

	if( iCount != GetNumOccupiedSlot(eType) )
	{
		START_LOG( cerr, L"���� ���� ���� ���� ������ �ִ� ������ ������ �ٸ�." )
			<< BUILD_LOG( iCount )
			<< BUILD_LOG( GetNumOccupiedSlot(eType) )
			<< END_LOG;
	}

	return iCount;
}

void KRoomUserManager::GetUnitLevelList( OUT std::map< UidType, int >& mapUnitLevelList )
{
	mapUnitLevelList.clear();

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

		mapUnitLevelList.insert( std::make_pair( mit->second->GetCID(), mit->second->GetLevel() ) );
	}
}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

//{{ 2012. 09. 08	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KRoomUserManager::GetUnitUIDByPartyUIDList( OUT std::map< UidType, UidType  >& mapUnitUIDPartyUID )
{
	mapUnitUIDPartyUID.clear();

	// ��Ƽ�� �ƴ� ���� ����Ʈ ���
	std::map< UidType, KRoomUserPtr >::const_iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( IS_NULL( mit->second ) )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		mapUnitUIDPartyUID.insert( std::make_pair( mit->second->GetCID(), mit->second->GetPartyUID() ) );
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM

//{{ 2012. 11. 20	������		���� ��Ʋ�ʵ� üũ �� ����
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
int	KRoomUserManager::GetHostCount() const
{
	int iCount = 0;

	{
		std::map< UidType, KRoomUserPtr >::const_iterator mit;
		for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
		{
			if( IS_NULL( mit->second ) )
			{
				START_LOG( cwarn, L"�� ���� ������ �̻�." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
				continue;
			}

			if( mit->second->IsHost() == true )
			{
				++iCount;
			}
		}
	}	

	{
		std::map< UidType, KRoomUserPtr >::const_iterator mit;
		for( mit = m_mapRoomUser[UT_OBSERVER].begin(); mit != m_mapRoomUser[UT_OBSERVER].end(); ++mit )
		{
			if( IS_NULL( mit->second ) )
			{
				START_LOG( cwarn, L"�� ���� ������ �̻�." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
				continue;
			}

			if( mit->second->IsHost() == true )
			{
				++iCount;
			}
		}
	}
	
	return iCount;
}

bool KRoomUserManager::ReselectOnlyOneHost( IN const UidType iOldHostUID )
{
	if( m_mapRoomUser[UT_GAME].size() < 2 )
	{
		START_LOG( cerr, L"host�� ���� �����Ҹ�ŭ ���� �ο��� ������� �ʽ��ϴ�!" )
			<< BUILD_LOG( m_mapRoomUser[UT_GAME].size() )
			<< END_LOG;
		return false;
	}

	UidType iNewHostUID = 0;

	{
		std::map< UidType, KRoomUserPtr >::iterator mit;
		for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
		{
			if( IS_NULL( mit->second ) )
			{
				START_LOG( cwarn, L"�� ���� ������ �̻�." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
				continue;
			}

			if( mit->second->GetCID() == iOldHostUID )
			{
				mit->second->SetHost( false );
				continue;
			}

			if( iNewHostUID == 0 )
			{
				iNewHostUID = mit->second->GetCID();
				mit->second->SetHost( true );
			}
			else
			{
				mit->second->SetHost( false );
			}
		}
	}	

	{
		std::map< UidType, KRoomUserPtr >::iterator mit;
		for( mit = m_mapRoomUser[UT_OBSERVER].begin(); mit != m_mapRoomUser[UT_OBSERVER].end(); ++mit )
		{
			if( IS_NULL( mit->second ) )
			{
				START_LOG( cwarn, L"�� ���� ������ �̻�." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
				continue;
			}

			mit->second->SetHost( false );
		}
	}

	return true;
}
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
//}}

bool KRoomUserManager::EnterRoom( IN const KRoomUserInfo& kInfo, IN const bool bConsiderTeam )
{
	if( kInfo.m_kGameStat.m_iBaseHP <= 0 )
	{
		START_LOG( cerr, L"�濡 ���� �߰��� HP���� ����" )
			<< BUILD_LOG( kInfo.m_iGSUID )
			<< BUILD_LOGc( kInfo.m_cUnitClass )
			<< BUILD_LOG( kInfo.m_ucLevel )
			<< BUILD_LOG( kInfo.m_nUnitUID )
			<< BUILD_LOG( kInfo.m_kGameStat.m_iBaseHP )
			<< END_LOG;
	}

	USERLIST_TYPE eType = UT_GAME;
	if( kInfo.m_bIsObserver == true )
		eType = UT_OBSERVER;

    KRoomUserPtr spRoomUser = MakeUser( kInfo );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� ���� ����." )
            << BUILD_LOG( ( int )m_mapRoomUser[eType].size() )
            << END_LOG;

        return false;
    }

	if( eType == UT_GAME )
	{
		spRoomUser->SetHost( GetNumOccupiedSlot() == 0 );
	}

	KRoomSlotPtr spRoomSlot;
	spRoomSlot = FindEmptySlot( bConsiderTeam, eType );

    if( !spRoomSlot )
    {
		START_LOG( cerr, L"�� ������ ����." )
			<< BUILD_LOG( GetNumTotalSlot(eType) )
			<< BUILD_LOG( GetNumOpenedSlot(eType) )
			<< BUILD_LOG( GetNumOccupiedSlot(eType) )
			<< END_LOG;

        return false;
    }

    if( !AddUser( spRoomUser, eType ) )
    {
        START_LOG( cerr, L"�� ���� �߰� ����." )
            << BUILD_LOG( spRoomUser->GetCID() )
            << END_LOG;

        return false;
    }

    if( !spRoomSlot->Enter( spRoomUser ) )
    {
        START_LOG( cerr, L"���Կ� ���� ����." )
            << BUILD_LOG( spRoomSlot->GetSlotID() )
            << END_LOG;

        if( DeleteUser( spRoomUser->GetCID(), eType ) == false )
		{
			START_LOG( cerr, L"�������� ����.!" )
				<< BUILD_LOG( spRoomUser->GetCID() )
				<< BUILD_LOG( (int)eType )
				<< END_LOG;
		}

        return false;
    }

	//{{ 2012. 11. 20	������		���� ��Ʋ�ʵ� üũ �� ����
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	// ���� ī��Ʈ üũ!
	if( GetHostCount() != 1 )
	{
		START_LOG( cerr, L"�濡 �����ߴµ� ������ �Ѹ��� �ƴմϴ�!" )
			<< BUILD_LOG( spRoomUser->IsHost() )
			<< BUILD_LOG( spRoomUser->GetNickName() )
			<< BUILD_LOG( kInfo.m_bIsObserver )
			<< END_LOG;
	}
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	//}}

    return true;
}

//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
bool KRoomUserManager::EnterRoom( IN const std::vector< KRoomUserInfo >& vecRedTeam, IN const std::vector< KRoomUserInfo >& vecBlueTeam, IN const std::map< UidType, KPvpNpcInfo >& mapPvpNpcInfo, IN const std::map< UidType, int >& mapMatchWaitTime )
{
	if( vecRedTeam.size() != vecBlueTeam.size()  ||  vecRedTeam.empty()  ||  vecBlueTeam.empty() )
	{
		START_LOG( cerr, L"���� ������ ���� �����ų�� �ο����� �ٸ���? ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( vecRedTeam.size() )
			<< BUILD_LOG( vecBlueTeam.size() )
			<< END_LOG;
		return false;
	}

	// �� ���� �°� �����Ű��!
	const u_int uiTeamNumMember = vecRedTeam.size();

	for( u_int ui = 0; ui < uiTeamNumMember; ++ui )
	{
		//////////////////////////////////////////////////////////////////////////
		// Red�� ����!
		if( EnterRoom( vecRedTeam[ui], true ) == false )
		{
			START_LOG( cerr, L"������ ���忡 �����Ͽ����ϴ�!" )
				<< BUILD_LOG( vecRedTeam[ui].m_nUnitUID )
				<< END_LOG;
			return false;
		}

		if( SetReady( vecRedTeam[ui].m_nUnitUID, true ) == false )
		{
			START_LOG( cerr, L"���� ���� ����!" )
				<< BUILD_LOG( vecRedTeam[ui].m_nUnitUID )
				<< END_LOG;
			return false;
		}

		{
			std::map< UidType, int >::const_iterator mit;
			mit = mapMatchWaitTime.find( vecRedTeam[ui].m_nUnitUID );
			if( mit != mapMatchWaitTime.end() )
			{
				if( SetMatchWaitTime( vecRedTeam[ui].m_nUnitUID, mit->second ) == false )
				{
					START_LOG( cerr, L"��ġ ��� �ð� ���� ���� ����!" )
						<< BUILD_LOG( vecRedTeam[ui].m_nUnitUID )
						<< BUILD_LOG( mit->second )
						<< END_LOG;
					return false;
				}
			}
		}
		
		if( vecRedTeam[ui].m_bIsPvpNpc )
		{
			std::map< UidType, KPvpNpcInfo >::const_iterator mit;
			mit = mapPvpNpcInfo.find( vecRedTeam[ui].m_nUnitUID );
			if( mit == mapPvpNpcInfo.end() )
			{
				START_LOG( cerr, L"Pvp Npc������ ����!" )
					<< BUILD_LOG( vecRedTeam[ui].m_nUnitUID )
					<< END_LOG;
				return false;
			}

			// NPC���� ����
			if( SetPvpNpcInfo( vecRedTeam[ui].m_nUnitUID, mit->second.m_iNpcID ) == false )
			{
				START_LOG( cerr, L"Pvp Npc ���� ������Ʈ ����!" )
					<< BUILD_LOG( vecRedTeam[ui].m_nUnitUID )
					<< BUILD_LOG( mit->second.m_iNpcID )
					<< END_LOG;
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// Blue�� ����!
		if( EnterRoom( vecBlueTeam[ui], true ) == false )
		{
			START_LOG( cerr, L"������ ���忡 �����Ͽ����ϴ�!" )
				<< BUILD_LOG( vecBlueTeam[ui].m_nUnitUID )
				<< END_LOG;
			return false;
		}

		if( SetReady( vecBlueTeam[ui].m_nUnitUID, true ) == false )
		{
			START_LOG( cerr, L"���� ���� ����!" )
				<< BUILD_LOG( vecBlueTeam[ui].m_nUnitUID )
				<< END_LOG;
			return false;
		}

		{
			std::map< UidType, int >::const_iterator mit;
			mit = mapMatchWaitTime.find( vecBlueTeam[ui].m_nUnitUID );
			if( mit != mapMatchWaitTime.end() )
			{
				if( SetMatchWaitTime( vecBlueTeam[ui].m_nUnitUID, mit->second ) == false )
				{
					START_LOG( cerr, L"��ġ ��� �ð� ���� ���� ����!" )
						<< BUILD_LOG( vecBlueTeam[ui].m_nUnitUID )
						<< BUILD_LOG( mit->second )
						<< END_LOG;
					return false;
				}
			}
		}

		if( vecBlueTeam[ui].m_bIsPvpNpc )
		{
			std::map< UidType, KPvpNpcInfo >::const_iterator mit;
			mit = mapPvpNpcInfo.find( vecBlueTeam[ui].m_nUnitUID );
			if( mit == mapPvpNpcInfo.end() )
			{
				START_LOG( cerr, L"Pvp Npc������ ����!" )
					<< BUILD_LOG( vecBlueTeam[ui].m_nUnitUID )
					<< END_LOG;
				return false;
			}

			// NPC���� ����
			if( SetPvpNpcInfo( vecBlueTeam[ui].m_nUnitUID, mit->second.m_iNpcID ) == false )
			{
				START_LOG( cerr, L"Pvp Npc ���� ������Ʈ ����!" )
					<< BUILD_LOG( vecBlueTeam[ui].m_nUnitUID )
					<< BUILD_LOG( mit->second.m_iNpcID )
					<< END_LOG;
				return false;
			}
		}
	}

	// �� �������� ������!
	m_iRedTeamRating = GetRatingByTeam( CXSLRoom::TN_RED );
	if( m_iRedTeamRating < 0 )
	{
		START_LOG( cerr, L"�� �������� ������ ���߽��ϴ�! ���� ó��!" )
			<< BUILD_LOG( m_iRedTeamRating )
			<< END_LOG;
		return false;
	}

	m_iBlueTeamRating = GetRatingByTeam( CXSLRoom::TN_BLUE );
	if( m_iBlueTeamRating < 0 )
	{
		START_LOG( cerr, L"�� �������� ������ ���߽��ϴ�! ���� ó��!" )
			<< BUILD_LOG( m_iBlueTeamRating )
			<< END_LOG;
		return false;
	}

    return true;
}
#endif SERV_PVP_NEW_SYSTEM
//}}

bool KRoomUserManager::LeaveRoom( IN const UidType iUnitUID )
{
	USERLIST_TYPE eType = UT_GAME;
    KRoomUserPtr spRoomUser = GetUser( iUnitUID, eType );
    if( IS_NULL( spRoomUser ) )
    {
		eType = UT_OBSERVER;
		spRoomUser = GetUser( iUnitUID, UT_OBSERVER );

		if( IS_NULL( spRoomUser ) )
		{
			START_LOG( cwarn, L"������ ����." )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;

			//{{ 2008. 1. 29  ������  �뽽�Կ� ���� ���� �˻�
			std::vector< KRoomSlotPtr >::iterator vit;
			for( vit = m_vecRoomSlot[eType].begin(); vit != m_vecRoomSlot[eType].end(); ++vit )
			{
				if( (*vit) == NULL )
					continue;

				if( (*vit)->GetCID() == iUnitUID )
				{
					START_LOG( cerr, L"mapRoomUser���� ���µ� vecRoomSlot���� �����ϴ� ������ �ִ�!" )
						<< BUILD_LOG( iUnitUID )
						<< END_LOG;
				}
			}
			//}}

			return true;
		}
    }

    KRoomSlotPtr spRoomSlot = GetSlot( spRoomUser->GetSlotID(), eType );
    if( IS_NULL( spRoomSlot ) )
    {
        START_LOG( cerr, L"���� ������ �̻�." )
            << BUILD_LOG( spRoomUser->GetSlotID() )
            << END_LOG;

        return false;
    }

	if( spRoomUser->IsObserver() == false )
	{
		if( GetNumMember() > 1 && spRoomUser->IsHost() )
		{
			//{{ 2012. 11. 20	������		���� ��Ʋ�ʵ� üũ �� ����
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
			// Ȥ�� ���⼭�� ������ �θ��̻����� üũ����!
			if( GetHostCount() != 1 )
			{
				START_LOG( cerr, L"ȣ��Ʈ�� �Ѹ��� �ƴϳ�? ���� �Ͼ���� �ȵǴ� ����!" )
					<< BUILD_LOG( GetHostCount() )
					<< BUILD_LOG( GetNumMember() )
					<< END_LOG;
			}

			// ���ο� ���� �Ӹ�
			if( AppointNewHost() == 0 )
			{
				START_LOG( cerr, L"�� ���� �Ӹ� ����. ���� �Ͼ���� �ȵǴ� ����!" )
					<< BUILD_LOG( GetHostCount() )
					<< BUILD_LOG( GetNumMember() )
					<< END_LOG;

				// host�� ������ ���� ��������!
				if( ReselectOnlyOneHost( spRoomUser->GetCID() ) == false )
				{
					START_LOG( cerr, L"ȣ��Ʈ ���� ������ �����ߴ�! ���� �Ͼ���� �ȵǴ� ����!" )
						<< BUILD_LOG( spRoomUser->GetCID() )
						<< END_LOG;
				}

				// ȣ��Ʈ ī��Ʈ �ٽ� �ѹ� üũ!
				if( GetHostCount() != 1 )
				{
					START_LOG( cerr, L"ȣ��Ʈ�� �Ѹ��� �ƴϳ�? ���� �Ͼ���� �ȵǴ� ����!" )
						<< BUILD_LOG( GetHostCount() )
						<< BUILD_LOG( GetNumMember() )
						<< END_LOG;
				}
				// ���ο� ���� �Ӹ��� �����ϸ� �ȵȴ�! ������ ������Ű�� ������ ��Żó������!
				//return false;
			}

			// ���ο� ������ �Ӹ� �Ǿ��ٸ� ���� ������ ���忡�� ��������!
			spRoomUser->SetHost( false );
#else
			if( AppointNewHost() == 0 )
			{
				// �ϴ� �����α׸� ���� ���� cwarn���� ��´�.
				START_LOG( cwarn, L"�� ���� �Ӹ� ����." )
					<< END_LOG;
				return false;
			}
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
			//}}			
		}
	}

    if( !spRoomSlot->Leave() )
    {
        START_LOG( cerr, L"���Կ��� ������ ����." )
            << BUILD_LOG( spRoomSlot->GetSlotID() )
            << END_LOG;

        return false;
    }

    return DeleteUser( spRoomUser->GetCID(), eType );
}

bool KRoomUserManager::LeaveGame( UidType nCID )
{
	USERLIST_TYPE eType = UT_GAME;
    KRoomUserPtr spRoomUser = GetUser( nCID, eType );
    if( !spRoomUser )
    {
		eType = UT_OBSERVER;
		spRoomUser = GetUser( nCID, eType );

		if( !spRoomUser )
		{
			START_LOG( cerr, L"������ ����." )
				<< BUILD_LOG( nCID )
				<< END_LOG;

			return false;
		}
    }

    spRoomUser->EndGame();

	if( spRoomUser->IsObserver() == false )
	{
		if( GetNumMember() > 1 && spRoomUser->IsHost() )
		{
			if( AppointNewHost() == 0 )
			{
				START_LOG( cerr, L"�� ���� �Ӹ� ����." )
					<< END_LOG;

				return false;
			}

			spRoomUser->SetHost( false );
		}
	}

    return true;
}

void KRoomUserManager::LeaveAllUnit()
{
	std::map< UidType, KRoomUserPtr >::iterator mit;
	std::vector<UidType> vecUIDList;
	int i = 0;
	for( i = UT_GAME; i < UT_TOTAL_NUM; ++i )
	{
		for( mit = m_mapRoomUser[i].begin(); mit != m_mapRoomUser[i].end(); ++mit )
		{
			vecUIDList.push_back(mit->first);
		}
	}

	for( i = 0; i < static_cast<int>(vecUIDList.size()); ++i )
	{
		LeaveRoom( vecUIDList[i] );
	}
}

bool KRoomUserManager::SetRoomUserInfo( UidType nCID, const KRoomUserInfo& kInfo )
{
	USERLIST_TYPE eType = UT_GAME;
	KRoomUserPtr spRoomUser = GetUser( nCID, eType );
	if( !spRoomUser )
	{
		eType = UT_OBSERVER;
		spRoomUser = GetUser( nCID, eType );

		if( !spRoomUser )
		{
			START_LOG( cwarn, L"������ ����." )
				<< BUILD_LOG( nCID )
				<< END_LOG;

			return false;
		}
	}

	spRoomUser->SetRoomUserInfo( kInfo );

	return true;
}

bool KRoomUserManager::ChangeTeam( UidType nCID, int iDestTeam )
{
	USERLIST_TYPE eType = UT_GAME;
    KRoomUserPtr spRoomUser = GetUser( nCID, eType );
    if( !spRoomUser )
    {
        START_LOG( cwarn, L"������ ����.-ChangeTeam()" )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    KRoomSlotPtr spRoomSlotFrom = GetSlot( spRoomUser->GetSlotID() );
    if( !spRoomSlotFrom )
    {
        START_LOG( cerr, L"���� ������ �̻�.-ChangeTeam()" )
            << BUILD_LOG( spRoomUser->GetSlotID() )
            << END_LOG;

        return false;
    }

    if( spRoomSlotFrom->GetTeam() == iDestTeam )
    {
        START_LOG( cwarn, L"�̹� �ش� ���� ���� ����.-ChangeTeam()" )
            << BUILD_LOG( iDestTeam )
            << END_LOG;

        return true;
    }

    for( int i = 0; i < m_nRoomSlot[eType]; i++ )
    {
        KRoomSlotPtr spRoomSlotTo = GetSlot( i );
        if( spRoomSlotTo->IsOpened() && !spRoomSlotTo->IsOccupied() && spRoomSlotTo->GetTeam() == iDestTeam )
        {
            LIF( spRoomSlotFrom->Leave() );
            LIF( spRoomSlotTo->Enter( spRoomUser ) );
            return true;
        }
    }

    START_LOG( cwarn, L"�� ���� ����." )
        << END_LOG;

    return false;
}
//���� �� ��ġ���� ���Խ� ���ȱ�� üũ.
bool KRoomUserManager::CheckTeamChange( UidType nCID )
{
	USERLIST_TYPE eType = UT_GAME;
	KRoomUserPtr spRoomUser = GetUser( nCID, eType );
	if( !spRoomUser )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< BUILD_LOG( (int)eType )
			<< END_LOG;

		return false;
	}

	int nPlayRed	= 0;
	int nPlayBlue	= 0;
	int nTotalRed	= 0;
	int nTotalBlue	= 0;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	KRoomUserPtr spRoomUserMIT;
	KRoomSlotPtr spRoomSlot;

	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		spRoomUserMIT = mit->second;

		if( !spRoomUserMIT )
		{
			START_LOG( cerr, L"������ ����." )
				<< BUILD_LOG( nCID )
				<< END_LOG;

			return false;
		}

		spRoomSlot = GetSlot( spRoomUserMIT->GetSlotID() );

		if( !spRoomSlot )
		{
			START_LOG( cerr, L"���� ������ �̻�." )
				<< BUILD_LOG( spRoomUserMIT->GetSlotID() )
				<< END_LOG;

			return false;
		}

		switch( spRoomSlot->GetTeam() )
		{
		case CXSLRoom::TN_RED:
			{
				if( spRoomUserMIT->IsPlaying() == true )
					++nPlayRed;

				++nTotalRed;
			}
			break;

		case CXSLRoom::TN_BLUE:
			{
				if( spRoomUserMIT->IsPlaying() == true )
					++nPlayBlue;

				++nTotalBlue;
			}
			break;
		}
	}

	spRoomSlot = GetSlot( spRoomUser->GetSlotID(), eType );

	if( !spRoomSlot )
	{
		START_LOG( cerr, L"���� ������ �̻�." )
			<< BUILD_LOG( spRoomUserMIT->GetCID() )
			<< BUILD_LOG( spRoomUserMIT->GetSlotID() )
			<< END_LOG;

		return false;
	}

	switch( spRoomSlot->GetTeam() )
	{
	case CXSLRoom::TN_RED:
		{
			if( (nPlayRed - nPlayBlue) > 1 &&
				nTotalBlue < 4 )
				return true;
		}
		break;

	case CXSLRoom::TN_BLUE:
		{
			if( (nPlayBlue - nPlayRed) > 1 &&
				nTotalRed < 4 )
				return true;
		}
		break;
	}

	return false;
}

bool KRoomUserManager::DeathTeamBalance( std::vector<KRoomUserPtr>& vecChangeUnit )
{
	USERLIST_TYPE eType = UT_GAME;
	vecChangeUnit.clear();

	int nReadyRed	= 0;
	int nReadyBlue	= 0;
	int nTotalRed	= 0;
	int nTotalBlue	= 0;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	KRoomUserPtr spRoomUser;
	KRoomSlotPtr spRoomSlot;
	std::vector<KRoomUserPtr>	vecRedTeam;
	std::vector<KRoomUserPtr>	vecBlueTeam;

	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		spRoomUser = mit->second;

		if( !spRoomUser )
		{
			START_LOG( cwarn, L"������ �����Ͱ� �߸��Ǿ���.-DeathTeamBalance()" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			return false;
		}

		spRoomSlot = GetSlot( spRoomUser->GetSlotID(), eType );

		if( !spRoomSlot )
		{
			START_LOG( cerr, L"���� ������ �̻�." )
				<< BUILD_LOG( spRoomUser->GetSlotID() )
				<< END_LOG;

			return false;
		}

		switch( spRoomSlot->GetTeam() )
		{
		case CXSLRoom::TN_RED:
			{
				if( spRoomUser->IsReady() == true )
					++nReadyRed;

				++nTotalRed;

				vecRedTeam.push_back( spRoomUser );
			}
			break;

		case CXSLRoom::TN_BLUE:
			{
				if( spRoomUser->IsReady() == true )
					++nReadyBlue;

				++nTotalBlue;

				vecBlueTeam.push_back( spRoomUser );
			}
			break;
		}
	}

	CXSLRoom::TEAM_NUM eTeam = CXSLRoom::TN_MONSTER;
	if( nReadyRed > nReadyBlue )
		eTeam = CXSLRoom::TN_RED;
	else if( nReadyRed < nReadyBlue )
		eTeam = CXSLRoom::TN_BLUE;

	int nNum = abs( nReadyRed - nReadyBlue );

	std::vector<KRoomUserPtr>::reverse_iterator rvit;
	//���� ���� ������ΰ�.
	//���� ������ ���������� �ȱ�� 
	switch( eTeam )
	{
	case CXSLRoom::TN_RED:
		{
			switch( nNum )
			{
			case 2:
			case 3:
				{
					if( nTotalBlue < 4 )
					{
						rvit = vecRedTeam.rbegin();
						if( !(*rvit) )
						{
							START_LOG( cerr, L"Death Team Match ������ ���������� ����" )
								<< BUILD_LOG( vecRedTeam.size() )
								<< END_LOG;

							return false;
						}
						if( ChangeTeam( (*rvit)->GetCID(), CXSLRoom::TN_BLUE ) == true )
							vecChangeUnit.push_back( *rvit );
						else
							return false;

						return true;
					}
				}
				break;
			case 4:
				{
					if( nTotalBlue < 3 )
					{
						int i = 0;
						for( rvit = vecRedTeam.rbegin(); rvit != vecRedTeam.rend(); ++rvit )
						{
							++i;
							if( i == 3 )
								return true;

							if( !(*rvit) )
							{
								START_LOG( cerr, L"Death Team Match ������ ���������� ����" )
									<< BUILD_LOG( vecRedTeam.size() )
									<< END_LOG;

								return false;
							}

							if( ChangeTeam( (*rvit)->GetCID(), CXSLRoom::TN_BLUE ) == true )
								vecChangeUnit.push_back( *rvit );
						}

						if( vecChangeUnit.empty() == true )
							return false;
					}
				}
			}
		}
		break;

	case CXSLRoom::TN_BLUE:
		{
			switch( nNum )
			{
			case 2:
			case 3:
				{
					if( nTotalRed < 4 )
					{
						rvit = vecBlueTeam.rbegin();
						if( !(*rvit) )
						{
							START_LOG( cerr, L"Death Team Match ������ ���������� ����" )
								<< BUILD_LOG( vecRedTeam.size() )
								<< END_LOG;

							return false;
						}
						if( ChangeTeam( (*rvit)->GetCID(), CXSLRoom::TN_RED ) == true )
							vecChangeUnit.push_back( *rvit );
						else
							return false;

						return true;
					}
				}
				break;
			case 4:
				{
					if( nTotalRed < 3 )
					{
						int i = 0;
						for( rvit = vecBlueTeam.rbegin(); rvit != vecBlueTeam.rend(); ++rvit )
						{
							++i;
							if( i == 3 )
								return true;

							if( !(*rvit) )
							{
								START_LOG( cerr, L"Death Team Match ������ ���������� ����" )
									<< BUILD_LOG( vecRedTeam.size() )
									<< END_LOG;

								return false;
							}

							if( ChangeTeam( (*rvit)->GetCID(), CXSLRoom::TN_RED ) == true )
                                vecChangeUnit.push_back( *rvit );
						}

						if( vecChangeUnit.empty() == true )
							return false;
					}
				}
			}
		}
		break;
	}

	return false;
}

bool KRoomUserManager::GetTeamNum( int& nRedNum, int& nBlueNum )
{
	USERLIST_TYPE eType = UT_GAME;

	nRedNum		= 0;
	nBlueNum	= 0;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	KRoomUserPtr spRoomUser;
	KRoomSlotPtr spRoomSlot;

	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		spRoomUser = mit->second;

		if( !spRoomUser )
		{
			START_LOG( cerr, L"������ �����Ͱ� �߸��Ǿ���." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			return false;
		}

		spRoomSlot = GetSlot( spRoomUser->GetSlotID(), eType );

		if( !spRoomSlot )
		{
			START_LOG( cerr, L"���� ������ �̻�." )
				<< BUILD_LOG( spRoomUser->GetSlotID() )
				<< END_LOG;

			return false;
		}

		switch( spRoomSlot->GetTeam() )
		{
		case CXSLRoom::TN_RED:
				++nRedNum;
			break;

		case CXSLRoom::TN_BLUE:
				++nBlueNum;
			break;
		}
	}

	return true;
}

int KRoomUserManager::GetTeamReadyNum(CXSLRoom::TEAM_NUM eTeamNum)
{
	USERLIST_TYPE eType = UT_GAME;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	KRoomUserPtr spRoomUser;

	int iReadyNum = 0;

	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		spRoomUser = mit->second;

		if( !spRoomUser )
		{
			START_LOG( cerr, L"������ �����Ͱ� �߸��Ǿ���." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			return 0;
		}

		if( spRoomUser->GetTeam() == eTeamNum && spRoomUser->IsReady() == true )
			++iReadyNum;
	}

	return iReadyNum;
}

bool KRoomUserManager::IsReady( UidType nCID, bool& bReady )
{
	USERLIST_TYPE eType = UT_GAME;
    KRoomUserPtr spRoomUser = GetUser( nCID, eType );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"������ ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    KRoomSlotPtr spRoomSlot = GetSlot( spRoomUser->GetSlotID(), eType );
    if( !spRoomSlot )
    {
        START_LOG( cerr, L"���� ������ �̻�." )
            << BUILD_LOG( spRoomUser->GetSlotID() )
            << END_LOG;

        return false;
    }

    bReady = spRoomUser->IsReady();
    return true;
}

//{{ 2012. 12. 5	������		������ ���� ���� ���
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
bool KRoomUserManager::SetReady( IN const UidType nCID, IN const bool bReady, IN const bool bObserver /*= false*/ )
{
	const USERLIST_TYPE eType = ( bObserver ? UT_OBSERVER : UT_GAME );

	KRoomUserPtr spRoomUser = GetUser( nCID, eType );
	if( !spRoomUser )
	{
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	KRoomSlotPtr spRoomSlot = GetSlot( spRoomUser->GetSlotID() );
	if( !spRoomSlot )
	{
		START_LOG( cerr, L"���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< BUILD_LOG( spRoomUser->GetSlotID() )
			<< END_LOG;

		return false;
	}

	return spRoomUser->SetReady( bReady );
}
#else
bool KRoomUserManager::SetReady( IN const UidType nCID, IN const bool bReady )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	KRoomSlotPtr spRoomSlot = GetSlot( spRoomUser->GetSlotID() );
	if( !spRoomSlot )
	{
		START_LOG( cerr, L"���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< BUILD_LOG( spRoomUser->GetSlotID() )
			<< END_LOG;

		return false;
	}

	return spRoomUser->SetReady( bReady );
}
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
//}}

void KRoomUserManager::SetAllReady( bool bReady )
{
	USERLIST_TYPE eType = UT_GAME;
    std::map< UidType, KRoomUserPtr >::iterator mit;
    for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"�� ���� ������ �̻�." )
                << BUILD_LOG( mit->first )
                << END_LOG;
            continue;
        }
        mit->second->SetReady( bReady );
    }
}

bool KRoomUserManager::SetPitIn( UidType nCID, bool bPitIn )
{
	USERLIST_TYPE eType = UT_GAME;
	KRoomUserPtr spRoomUser = GetUser( nCID, eType );
	if( !spRoomUser )
	{
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	KRoomSlotPtr spRoomSlot = GetSlot( spRoomUser->GetSlotID(), eType );
	if( !spRoomSlot )
	{
		START_LOG( cerr, L"���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< BUILD_LOG( spRoomUser->GetSlotID() )
			<< END_LOG;

		return false;
	}

	spRoomUser->SetPitIn( bPitIn );

	return true;
}

bool KRoomUserManager::IsInTrade( UidType nCID, bool& bTrade )
{
	USERLIST_TYPE eType = UT_GAME;
    KRoomUserPtr spRoomUser = GetUser( nCID, eType );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    KRoomSlotPtr spRoomSlot = GetSlot( spRoomUser->GetSlotID(), eType );
    if( !spRoomSlot )
    {
        START_LOG( cerr, L"���� �˻� ����." )
            << BUILD_LOG( nCID )
            << BUILD_LOG( spRoomUser->GetSlotID() )
            << END_LOG;

        return false;
    }

    bTrade = spRoomUser->IsInTrade();

    return true;
}

bool KRoomUserManager::SetTrade( UidType nCID, bool bTrade )
{
	USERLIST_TYPE eType = UT_GAME;
    KRoomUserPtr spRoomUser = GetUser( nCID, eType );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    KRoomSlotPtr spRoomSlot = GetSlot( spRoomUser->GetSlotID(), eType );
    if( !spRoomSlot )
    {
        START_LOG( cerr, L"���� �˻� ����." )
            << BUILD_LOG( nCID )
            << BUILD_LOG( spRoomUser->GetSlotID() )
            << END_LOG;

        return false;
    }

    spRoomUser->SetTrade( bTrade );

    return true;
}

bool KRoomUserManager::SetLoadingProgress( UidType nCID, int iLoadingProgress )
{
	USERLIST_TYPE eType = UT_GAME;
    KRoomUserPtr spRoomUser = GetUser( nCID, eType );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    spRoomUser->SetLoadingProgress( iLoadingProgress );
    return true;
}

bool KRoomUserManager::SetStageLoaded( UidType nCID, bool bStageLoaded )
{
	USERLIST_TYPE eType = UT_GAME;
    KRoomUserPtr spRoomUser = GetUser( nCID, eType );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    spRoomUser->SetStageLoaded( bStageLoaded );
    return true;
}

void KRoomUserManager::ResetStageLoaded()
{
	USERLIST_TYPE eType = UT_GAME;
    std::map< UidType, KRoomUserPtr >::iterator mit;
    for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"�� ���� ������ �̻�." )
                << BUILD_LOG( mit->first )
                << END_LOG;
            continue;
        }

        mit->second->SetStageLoaded( false );
    }
}

bool KRoomUserManager::GetTeam( UidType nCID, int& iTeam )
{
	USERLIST_TYPE eType = UT_GAME;
    KRoomUserPtr spRoomUser = GetUser( nCID, eType );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"������ ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    iTeam = spRoomUser->GetTeam();
    return true;
}

bool KRoomUserManager::IncreaseTeamNumKill( UidType nCID )
{
	USERLIST_TYPE eType = UT_GAME;
    KRoomUserPtr spRoomUser = GetUser( nCID, eType );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"������ ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    std::map< int, int >::iterator mitTeamNumKill = m_mapTeamNumKill.find( spRoomUser->GetTeam() );
    if( mitTeamNumKill == m_mapTeamNumKill.end() )
    {
        m_mapTeamNumKill.insert( std::make_pair( spRoomUser->GetTeam(), 1 ) );
    }
    else
    {
        mitTeamNumKill->second += 1;
    }

    return true;
}

bool KRoomUserManager::IncreaseTeamNumKillByTeamNum( CXSLRoom::TEAM_NUM teamNum )
{
	std::map< int, int >::iterator mitTeamNumKill = m_mapTeamNumKill.find( (int)teamNum );
	if( mitTeamNumKill == m_mapTeamNumKill.end() )
	{
		m_mapTeamNumKill.insert( std::make_pair( (int)teamNum, 1 ) );
	}
	else
	{
		mitTeamNumKill->second += 1;
	}

	return true;
}

bool KRoomUserManager::SetStageID( UidType nCID, int iStageID )
{
	USERLIST_TYPE eType = UT_GAME;
    KRoomUserPtr spRoomUser = GetUser( nCID, eType );
    if( !spRoomUser )
    {
        return false;
    }

    spRoomUser->SetStageID( iStageID );
    return true;
}

void KRoomUserManager::ResetStageID()
{
	USERLIST_TYPE eType = UT_GAME;
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

        mit->second->SetStageID( -1 );
    }
}

//{{ 2012. 04. 17	������	���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
bool KRoomUserManager::SetSubStageID( IN const UidType iUnitUID, IN const int iSubStageID )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID, UT_GAME );
	if( IS_NULL( spRoomUser ) )
	{
		return false;
	}

	spRoomUser->SetSubStageID( iSubStageID );
	return true;
}

void KRoomUserManager::ResetSubStageID()
{
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

		mit->second->SetSubStageID( -1 );
	}
}
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
//}}

bool KRoomUserManager::GetNumKill( UidType nCID, int& nKill )
{
    KRoomUserPtr spRoomUser = GetUser( nCID );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    nKill = spRoomUser->GetNumKill();
    return true;
}

bool KRoomUserManager::IncreaseNumKill( UidType nCID )
{
    // 070730. jseop. ���� ����� �������� ������ �˻� ������ ���� �ִ�.
    // cerr -> cwarn
    KRoomUserPtr spRoomUser = GetUser( nCID );
    if( !spRoomUser )
    {
        START_LOG( cwarn, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    spRoomUser->IncreaseNumKill();
    return true;
}

bool KRoomUserManager::IncreaseNumMDKill( UidType nCID )
{
    // 070730. jseop. �ƽ� ������ ����� ����� �������� ������ �˻� ������ ���� �ִ�.
    // cerr -> cwarn
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cwarn, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spRoomUser->IncreaseNumMDKill();
	return true;
}

bool KRoomUserManager::GetNumDie( UidType nCID, int& nDie )
{
    KRoomUserPtr spRoomUser = GetUser( nCID );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    nDie = spRoomUser->GetNumDie();
    return true;
}

bool KRoomUserManager::IncreaseNumDie( UidType nCID )
{
    KRoomUserPtr spRoomUser = GetUser( nCID );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    spRoomUser->IncreaseNumDie();
    return true;
}

bool KRoomUserManager::IsDie( UidType nCID, bool& bDie )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	bDie = spRoomUser->IsDie();
	return true;
}

bool KRoomUserManager::SetDie( UidType nCID, bool bDie )
{
    KRoomUserPtr spRoomUser = GetUser( nCID );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    spRoomUser->SetDie( bDie );
    return true;
}

bool KRoomUserManager::SetHP( UidType nCID, float fHP )
{
    KRoomUserPtr spRoomUser = GetUser( nCID );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    spRoomUser->SetHP( fHP );
    return true;
}

bool KRoomUserManager::IsObserver( UidType nCID )
{
	KRoomUserPtr spRoomUser = GetUser( nCID, UT_OBSERVER );
	if( !spRoomUser )
	{
		START_LOG( clog, L"�� ���� �˻� ����.Observer�� �ƴ�.!" )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	return spRoomUser->IsObserver();
}

bool KRoomUserManager::GetRebirthPos( UidType nCID, int& iRebirthPos )
{
    KRoomUserPtr spRoomUser = GetUser( nCID );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    iRebirthPos = spRoomUser->GetRebirthPos();
    return true;
}

bool KRoomUserManager::SetRebirthPos( UidType nCID, int iRebirthPos )
{
    KRoomUserPtr spRoomUser = GetUser( nCID );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    spRoomUser->SetRebirthPos( iRebirthPos );
    return true;
}

void KRoomUserManager::GetCurrentKillScore( std::map< UidType, std::pair< int, int > >& mapKillScore, std::map< UidType, int >& mapMDKillScore )
{
	USERLIST_TYPE eType = UT_GAME;
    mapKillScore.clear();
    std::map< UidType, KRoomUserPtr >::iterator mit;
    for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"�� ���� ������ �̻�." )
                << BUILD_LOG( mit->first )
                << END_LOG;
            continue;
        }

        if( mit->second->IsPlaying() )
        {
            std::pair< int, int > pairKillDie = std::make_pair( mit->second->GetNumKill(), mit->second->GetNumDie() );
            mapKillScore.insert( std::make_pair( mit->second->GetCID(), pairKillDie ) );
			mapMDKillScore.insert( std::make_pair( mit->second->GetCID(), mit->second->GetNumMDKill() ) );
        }
    }
}

void KRoomUserManager::GetLoadingTimeOutUser( OUT std::map< UidType, UidType >& mapUserCIDNGS )
{
	USERLIST_TYPE eType = UT_GAME;
    mapUserCIDNGS.clear();
    std::map< UidType, KRoomUserPtr >::iterator mit;
    for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cwarn, L"�� ���� ������ �̻�.-GetLoadingTimeOutUser()" )
                << BUILD_LOG( mit->first )
                << END_LOG;
            continue;
        }

        if( mit->second->IsLoadingTimeOut() )
        {
            // mit->first �� key �̹Ƿ� overwrite ���� �ʴ´�.
            mapUserCIDNGS[mit->first] = mit->second->GetGSUID();
        }
    }
}

bool KRoomUserManager::GetPlayTime( UidType nCID, float& fPlayTime )
{
	USERLIST_TYPE eType = UT_GAME;
    KRoomUserPtr spRoomUser = GetUser( nCID );
    if( !spRoomUser )
    {
        START_LOG( cwarn, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << BUILD_LOG( m_mapRoomUser[eType].size() )
            << END_LOG;

		fPlayTime = 0.0f;
        return false;
    }

    fPlayTime = spRoomUser->GetPlayTime();
    return true;
}

//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
#else
bool KRoomUserManager::GetPVPEmblem( IN UidType nCID, OUT CXSLUnit::PVP_EMBLEM& eEmblem )
{
	USERLIST_TYPE eType = UT_GAME;
	KRoomUserPtr spRoomUser = GetUser( nCID );

	if( !spRoomUser )
	{
		START_LOG( cwarn, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< BUILD_LOG( m_mapRoomUser[eType].size() )
			<< END_LOG;

		return false;
	}

	eEmblem = spRoomUser->GetPVPEmblem();

	return true;
}
#endif SERV_2012_PVP_SEASON2
//}}

void KRoomUserManager::GetPlayTime( OUT std::map< UidType , float >& mapPlayTime )
{
	USERLIST_TYPE eType = UT_GAME;
    mapPlayTime.clear();
    std::map< UidType, KRoomUserPtr >::iterator mit;
    for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"�� ���� ������ �̻�." )
                << BUILD_LOG( mit->first )
                << END_LOG;
            mapPlayTime[mit->first] = 0;
        }
        else
        {
            mapPlayTime[mit->first] = mit->second->GetPlayTime();
        }
    }
}

//bool KRoomUserManager::AddItemWithRandom( IN int iItemID, OUT UidType& iGetUnitUID )
//{
//	USERLIST_TYPE eType = UT_GAME;
//	KRoomUserPtr spUser;
//
//	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
//	if( pItemTemplet == NULL )
//	{
//		START_LOG( cerr, L"Item Templet ������ ����. �Ͼ �� ���� ����" )
//			<< BUILD_LOG( iItemID )
//			<< END_LOG;
//		return false;
//	}
//
//	{ // �߸��� iterator������ �����ϱ� ���� scope
//
//		std::map< UidType, KRoomUserPtr >::iterator mit;
//		for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
//		{
//			if( !mit->second )
//			{
//				START_LOG( cerr, L"�� ���� ������ �̻�. CUID : " << mit->first );
//				continue;
//			}
//
//			// ���� ���� �����ϰ� ȹ���� �������� ���� ���� ���� ã��.
//			if( mit->second->IsDie() != true )
//			{
//				if( !spUser )   // base case.
//				{
//					spUser = mit->second;
//				}
//				else if( (mit->second->GetItemCount() * (int)(rand()%100 + 1)) < (spUser->GetItemCount() * (int)(rand()%100 + 1)) )
//				{
//					spUser = mit->second;   // ������ ȹ����� �� ���� �༮�� ����Ѵ�.
//				}
//			}
//		}
//
//	}
//
//	//{{ 2009. 2. 26  ������	�ŷ��Ұ��̸鼭 ����Ÿ�� ���� �������� ��� �ش�Ǵ� ĳ���� �켱����
//	if( ( pItemTemplet->m_UseCondition == CXSLItem::UC_ONE_UNIT )  &&  pItemTemplet->m_bVested )
//	{
//		std::map< UidType, KRoomUserPtr >::iterator mitOC;
//		for( mitOC = m_mapRoomUser[eType].begin(); mitOC != m_mapRoomUser[eType].end(); ++mitOC )
//		{
//			if( !mitOC->second )
//			{
//				START_LOG( cerr, L"�� ���� ������ �̻�. CUID : " << mitOC->first );
//				continue;
//			}
//
//			// ���� ���� �����ϰ� �ش� ����Ÿ���� �����߿� ȹ���� �������� ���� ���� ���� ã��.
//			if( mitOC->second->IsDie() != true  &&  mitOC->second->GetUnitType() == static_cast<char>(pItemTemplet->m_UnitType) )
//			{
//				if( !spUser )   // base case.
//				{
//					spUser = mitOC->second;
//				}
//				else if( spUser->GetUnitType() != static_cast<char>(pItemTemplet->m_UnitType) ) // ���� ������ ����Ÿ���� �ٸ��ٸ� ����
//				{
//					spUser = mitOC->second;
//				}
//				else if( (mitOC->second->GetItemCount() * (int)(rand()%100 + 1)) < (spUser->GetItemCount() * (int)(rand()%100 + 1)) )
//				{
//					spUser = mitOC->second;   // ������ ȹ����� �� ���� �༮�� ����Ѵ�.
//				}
//			}
//		}
//	}
//	//}}
//
//	_JIF( spUser, return false );
//
//	iGetUnitUID = spUser->GetCID();
//
//	spUser->AddItem( iItemID, 1 );
//
//	return true;
//}

//////////////////////////////////////////////////////////////////////////
//{{ 2011. 04. 25	������	���� ȹ�� ���� ����
#ifdef SERV_GET_RANDOM_ITEM_LOGIC_FIX
//////////////////////////////////////////////////////////////////////////
//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
bool KRoomUserManager::GetItemWithRandom( IN const int iItemID, IN const int iDungeonID,  OUT UidType& iGetUnitUID, OUT UidType& iGSUID )
#else
bool KRoomUserManager::GetItemWithRandom( IN const int iItemID, OUT UidType& iGetUnitUID, OUT UidType& iGSUID )
#endif SERV_NEW_HENIR_TEST
//}}
{
	USERLIST_TYPE eType = UT_GAME;
	KRoomUserPtr spSelectedUser;

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"Item Templet ������ ����. �Ͼ �� ���� ����" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	std::vector< UidType > vecUserList;
	GetUnitUIDList( vecUserList );

	const std::vector< UidType > vecRoomUserShuffle = KncUtil::NumberShuffle( vecUserList );
	if( vecRoomUserShuffle.size() != m_mapRoomUser[eType].size() )
	{
		START_LOG( cerr, L"�濡 ������ �ο� ���� ���õ� ���� ����Ʈ ũ�Ⱑ �ٸ���? ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( m_mapRoomUser[eType].size() )
			<< BUILD_LOG( vecUserList.size() )
			<< BUILD_LOG( vecRoomUserShuffle.size() )
			<< END_LOG;
		// �ϴ� �α׸� ����!
	}

	{ // �߸��� iterator������ �����ϱ� ���� scope
		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecRoomUserShuffle )
		{
			KRoomUserPtr spRoomUser = GetUser( iUnitUID );
			if( spRoomUser == NULL )
			{
				START_LOG( cerr, L"�� ���� ������ �̻�. CUID : " << iUnitUID );
				continue;
			}

			//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
			if( CXSLDungeon::IsHenirDungeon( iDungeonID ) == true )
			{
				if( spRoomUser->IsHenirRewardUser() == false )
				{
					START_LOG( clog, L"��ϸ����� ������� ������ �����ϴ�. �����Ѵ�." )
						<< BUILD_LOG( spRoomUser->GetCID() )
						<< BUILD_LOG( spRoomUser->IsHenirRewardUser() )
						<< END_LOG;
					continue;
				}
			}
#endif SERV_NEW_HENIR_TEST
			//}}

			// ���� ���� �����ϰ� ȹ���� �������� ���� ���� ���� ã��.
			if( spRoomUser->IsDie() != true )
			{
				if( !spSelectedUser )   // base case.
				{
					spSelectedUser = spRoomUser;
				}
				else if( (spRoomUser->GetItemCount() * (int)(rand()%100 + 1)) < (spSelectedUser->GetItemCount() * (int)(rand()%100 + 1)) )
				{
					spSelectedUser = spRoomUser;   // ������ ȹ����� �� ���� �༮�� ����Ѵ�.
				}
			}
		}
	}

	//{{ 2010. 9. 10	������	��� �������� ����/���� ��� ���� Ŭ������ �������� �Ͽ� �켱 ����	
	if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON  ||  
		pItemTemplet->m_ItemType == CXSLItem::IT_DEFENCE  ||
		pItemTemplet->m_ItemType == CXSLItem::IT_ACCESSORY )
	{
		if( pItemTemplet->m_UseCondition == CXSLItem::UC_ONE_CLASS )
		{
			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecRoomUserShuffle )
			{
				KRoomUserPtr spRoomUser = GetUser( iUnitUID );
				if( spRoomUser == NULL )
				{
					START_LOG( cerr, L"�� ���� ������ �̻�. CUID : " << iUnitUID );
					continue;
				}


				//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
				if( CXSLDungeon::IsHenirDungeon( iDungeonID ) == true )
				{
					if( spRoomUser->IsHenirRewardUser() == false )
					{
						START_LOG( clog, L"��ϸ����� ������� ������ �����ϴ�. �����Ѵ�." )
							<< BUILD_LOG( spRoomUser->GetCID() )
							<< BUILD_LOG( spRoomUser->IsHenirRewardUser() )
							<< END_LOG;
						continue;
					}
				}
#endif SERV_NEW_HENIR_TEST
				//}}

				// ���� ���� �����ϰ� �ش� ����Ŭ������ �����߿� ȹ���� �������� ���� ���� ���� ã��.
				if( spRoomUser->IsDie() != true  &&  spRoomUser->GetUnitClass_LUA() == static_cast<char>(pItemTemplet->m_UnitClass) )
				{
					if( !spSelectedUser )   // base case.
					{
						spSelectedUser = spRoomUser;
					}
					else if( spSelectedUser->GetUnitClass_LUA() != static_cast<char>(pItemTemplet->m_UnitClass) ) // ���� ������ ����Ÿ���� �ٸ��ٸ� ����
					{
						spSelectedUser = spRoomUser;
					}
					else if( (spRoomUser->GetItemCount() * (int)(rand()%100 + 1)) < (spSelectedUser->GetItemCount() * (int)(rand()%100 + 1)) )
					{
						spSelectedUser = spRoomUser;   // ������ ȹ����� �� ���� �༮�� ����Ѵ�.
					}
				}
			}
		}
		else
		{
			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecRoomUserShuffle )
			{
				KRoomUserPtr spRoomUser = GetUser( iUnitUID );
				if( spRoomUser == NULL )
				{
					START_LOG( cerr, L"�� ���� ������ �̻�. CUID : " << iUnitUID );
					continue;
				}


				//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
				if( CXSLDungeon::IsHenirDungeon( iDungeonID ) == true )
				{
					if( spRoomUser->IsHenirRewardUser() == false )
					{
						START_LOG( clog, L"��ϸ����� ������� ������ �����ϴ�. �����Ѵ�." )
							<< BUILD_LOG( spRoomUser->GetCID() )
							<< BUILD_LOG( spRoomUser->IsHenirRewardUser() )
							<< END_LOG;
						continue;
					}
				}
#endif SERV_NEW_HENIR_TEST
				//}}

				// ���� ���� �����ϰ� �ش� ����Ŭ������ �����߿� ȹ���� �������� ���� ���� ���� ã��.
				if( spRoomUser->IsDie() != true  &&  spRoomUser->GetUnitType() == static_cast<char>(pItemTemplet->m_UnitType) )
				{
					if( !spSelectedUser )   // base case.
					{
						spSelectedUser = spRoomUser;
					}
					else if( spSelectedUser->GetUnitType() != static_cast<char>(pItemTemplet->m_UnitType) ) // ���� ������ ����Ÿ���� �ٸ��ٸ� ����
					{
						spSelectedUser = spRoomUser;
					}
					else if( (spRoomUser->GetItemCount() * (int)(rand()%100 + 1)) < (spSelectedUser->GetItemCount() * (int)(rand()%100 + 1)) )
					{
						spSelectedUser = spRoomUser;   // ������ ȹ����� �� ���� �༮�� ����Ѵ�.
					}
				}
			}            
		}
	}
	//}}

	_JIF( spSelectedUser, return false );

	iGetUnitUID = spSelectedUser->GetCID();
	iGSUID = spSelectedUser->GetGSUID();
	return true;
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

bool KRoomUserManager::GetItemWithRandom( IN const int iItemID, OUT UidType& iGetUnitUID, OUT UidType& iGSUID )
{
	USERLIST_TYPE eType = UT_GAME;
	KRoomUserPtr spUser;

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"Item Templet ������ ����. �Ͼ �� ���� ����" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	{ // �߸��� iterator������ �����ϱ� ���� scope
		std::map< UidType, KRoomUserPtr >::const_iterator mit;
		for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
		{
			if( !mit->second )
			{
				START_LOG( cerr, L"�� ���� ������ �̻�. CUID : " << mit->first );
				continue;
			}

			// ���� ���� �����ϰ� ȹ���� �������� ���� ���� ���� ã��.
			if( mit->second->IsDie() != true )
			{
				if( !spUser )   // base case.
				{
					spUser = mit->second;
				}
				else if( (mit->second->GetItemCount() * (int)(rand()%100 + 1)) < (spUser->GetItemCount() * (int)(rand()%100 + 1)) )
				{
					spUser = mit->second;   // ������ ȹ����� �� ���� �༮�� ����Ѵ�.
				}
			}
		}
	}

	//{{ 2010. 9. 10	������	��� �������� ����/���� ��� ���� Ŭ������ �������� �Ͽ� �켱 ����	
	if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON  ||  
		pItemTemplet->m_ItemType == CXSLItem::IT_DEFENCE  ||
		pItemTemplet->m_ItemType == CXSLItem::IT_ACCESSORY )
	{
		if( pItemTemplet->m_UseCondition == CXSLItem::UC_ONE_CLASS )
		{
			std::map< UidType, KRoomUserPtr >::const_iterator mitOC;
			for( mitOC = m_mapRoomUser[eType].begin(); mitOC != m_mapRoomUser[eType].end(); ++mitOC )
			{
				if( !mitOC->second )
				{
					START_LOG( cerr, L"�� ���� ������ �̻�. CUID : " << mitOC->first );
					continue;
				}

				// ���� ���� �����ϰ� �ش� ����Ŭ������ �����߿� ȹ���� �������� ���� ���� ���� ã��.
				if( mitOC->second->IsDie() != true  &&  mitOC->second->GetUnitClass_LUA() == static_cast<char>(pItemTemplet->m_UnitClass) )
				{
					if( !spUser )   // base case.
					{
						spUser = mitOC->second;
					}
					else if( spUser->GetUnitClass_LUA() != static_cast<char>(pItemTemplet->m_UnitClass) ) // ���� ������ ����Ÿ���� �ٸ��ٸ� ����
					{
						spUser = mitOC->second;
					}
					else if( (mitOC->second->GetItemCount() * (int)(rand()%100 + 1)) < (spUser->GetItemCount() * (int)(rand()%100 + 1)) )
					{
						spUser = mitOC->second;   // ������ ȹ����� �� ���� �༮�� ����Ѵ�.
					}
				}
			}
		}
		else
		{
			std::map< UidType, KRoomUserPtr >::const_iterator mitOC;
			for( mitOC = m_mapRoomUser[eType].begin(); mitOC != m_mapRoomUser[eType].end(); ++mitOC )
			{
				if( !mitOC->second )
				{
					START_LOG( cerr, L"�� ���� ������ �̻�. CUID : " << mitOC->first );
					continue;
				}

				// ���� ���� �����ϰ� �ش� ����Ŭ������ �����߿� ȹ���� �������� ���� ���� ���� ã��.
				if( mitOC->second->IsDie() != true  &&  mitOC->second->GetUnitType() == static_cast<char>(pItemTemplet->m_UnitType) )
				{
					if( !spUser )   // base case.
					{
						spUser = mitOC->second;
					}
					else if( spUser->GetUnitType() != static_cast<char>(pItemTemplet->m_UnitType) ) // ���� ������ ����Ÿ���� �ٸ��ٸ� ����
					{
						spUser = mitOC->second;
					}
					else if( (mitOC->second->GetItemCount() * (int)(rand()%100 + 1)) < (spUser->GetItemCount() * (int)(rand()%100 + 1)) )
					{
						spUser = mitOC->second;   // ������ ȹ����� �� ���� �༮�� ����Ѵ�.
					}
				}
			}            
		}
	}
	//}}

	_JIF( spUser, return false );

	iGetUnitUID = spUser->GetCID();
	iGSUID = spUser->GetGSUID();
	return true;
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_GET_RANDOM_ITEM_LOGIC_FIX
//////////////////////////////////////////////////////////////////////////
//}}

bool KRoomUserManager::AddItem( UidType nCID, IN int iItemID )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );

	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spRoomUser->AddItem( iItemID, 1 );
	return true;
}

bool KRoomUserManager::GetItemList( UidType nCID, std::map< int, int >& mapItemList )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );

	if( !spRoomUser )
	{
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spRoomUser->GetItemList( mapItemList );
	return true;
}

bool KRoomUserManager::AddKillNPC( UidType nCID )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cwarn, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spRoomUser->AddKillNPC();

	return true;
}

//{{ 2010. 05. 12  ������	���� �ڵ� ����
bool KRoomUserManager::SetDungeonUnitInfo( IN UidType nCID, IN const KDungeonPlayResultInfo& kDungeonUnitInfo )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spRoomUser->SetDungeonUnitInfo( kDungeonUnitInfo );
	return true;
}
//}}

void KRoomUserManager::ResetDungeonUnitInfo()
{
	USERLIST_TYPE eType = UT_GAME;
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

		mit->second->ResetDungeonUnitInfo();
	}
}

//{{ 2011. 08. 03	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
bool KRoomUserManager::SetPvpPlayResultInfo( IN const UidType iUnitUID, IN const KPvpPlayResultInfo& kPvpUnitInfo )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( !spRoomUser )
	{
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	spRoomUser->SetPvpPlayResultInfo( kPvpUnitInfo );
	return true;
}
#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2008. 9. 19  ������	���
bool KRoomUserManager::SetDungeonPlayInfo( UidType nCID, KEGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT& kDungeonPlayInfo )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spRoomUser->SetDungeonPlayInfo( kDungeonPlayInfo );
	return true;
}
//}}

bool KRoomUserManager::SetSuccessResult( UidType nCID, bool bResult )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		//{{ 2012. 11. 21	�ڼ���	��ȸ ä�� ���� ����
#if defined( SERV_FIX_ZOMBIE_OBSERVER ) || defined( SERV_TOURNAMENT_CONDITION )
		spRoomUser = GetUser( nCID, UT_OBSERVER );
		if( !spRoomUser )
		{
			START_LOG( cerr, L"�� ���� �˻� ����." )
				<< BUILD_LOG( nCID )
				<< END_LOG;

			return false;
		}
#else
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
#endif SERV_TOURNAMENT_CONDITION
		//}}
	}

	spRoomUser->SetSuccessResult( bResult );

	return true;
}

void KRoomUserManager::StartGame()
{
	USERLIST_TYPE eType = UT_GAME;
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

        if( !mit->second->IsReady() )
        {
            continue;
        }

        mit->second->StartGame();
    }

	//{{ 2011. 03. 08	��μ� ����ġ�� ȹ���� ������ ����Ʈ �Ϸ� ���� ����
#ifdef SERV_EXP_ACQUISITION_CONDITION_CHANGE
	m_mapHaveExpInDungeon.clear();
#endif SERV_EXP_ACQUISITION_CONDITION_CHANGE
	//}}
	//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	m_mapSuitableLevelInfo.clear();
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	//}}

}

void KRoomUserManager::StartPlay()
{
	USERLIST_TYPE eType = UT_GAME;
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

        if( mit->second->GetStateID() != KRoomUserFSM::S_LOAD )
        {
            continue;
        }

        mit->second->StartPlay();
    }

	m_mapTeamNumKill.clear();

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-16	// �ڼ���
	m_byteStartPlayMemberNum = GetNumMember();
#endif // SERV_HENIR_RENEWAL_2013
}

void KRoomUserManager::EndPlay()
{
	USERLIST_TYPE eType = UT_GAME;
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

        if( mit->second->GetStateID() != KRoomUserFSM::S_PLAY )
        {
            continue;
        }

        mit->second->EndPlay();
    }
}

void KRoomUserManager::EndGame()
{
	USERLIST_TYPE eType = UT_GAME;
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

        if( mit->second->GetStateID() != KRoomUserFSM::S_RESULT )
        {
            continue;
        }

        mit->second->EndGame();
    }
}

//{{ 2012. 12. 5	������		������ ���� ���� ���
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
bool KRoomUserManager::StartGame( IN const UidType nCID, IN const bool bObserver /*= false*/ )
{
	const USERLIST_TYPE eType = ( bObserver ? UT_OBSERVER : UT_GAME );

	KRoomUserPtr spRoomUser = GetUser( nCID, eType );
	if( !spRoomUser )
	{
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spRoomUser->StartGame();
	return true;
}

bool KRoomUserManager::StartPlay( IN const UidType nCID, IN const bool bObserver /*= false*/ )
{
	const USERLIST_TYPE eType = ( bObserver ? UT_OBSERVER : UT_GAME );

	KRoomUserPtr spRoomUser = GetUser( nCID, eType );
	if( !spRoomUser )
	{
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spRoomUser->StartPlay();
	return true;
}
#else
bool KRoomUserManager::StartGame( UidType nCID )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );

	if( !spRoomUser )
	{
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spRoomUser->StartGame();
	return true;
}

bool KRoomUserManager::StartPlay( UidType nCID )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );

	if( !spRoomUser )
	{
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spRoomUser->StartPlay();
	return true;
}
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
//}}

bool KRoomUserManager::EndPlay( UidType nCID )
{
    KRoomUserPtr spRoomUser = GetUser( nCID );

    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    spRoomUser->EndPlay();
    return true;
}

bool KRoomUserManager::EndGame( UidType nCID )
{
    KRoomUserPtr spRoomUser = GetUser( nCID );

    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    spRoomUser->EndGame();
    return true;
}

bool KRoomUserManager::IsHost( UidType nCID )
{
    KRoomUserPtr spRoomUser = GetUser( nCID );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    return spRoomUser->IsHost();
}

bool KRoomUserManager::IsPlaying( UidType nCID )
{
    KRoomUserPtr spRoomUser = GetUser( nCID );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    return spRoomUser->IsPlaying();
}

bool KRoomUserManager::IsAllPlayerReady()
{
	USERLIST_TYPE eType = UT_GAME;
    std::map< UidType, KRoomUserPtr >::iterator mit;
    for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"�� ���� ������ �̻�." )
                << BUILD_LOG( mit->first )
                << END_LOG;
            return false;
        }

        if( !mit->second->IsReady() )
        {
            return false;
        }
    }

    return true;
}

bool KRoomUserManager::IsAllPlayerFinishLoading()
{
	USERLIST_TYPE eType = UT_GAME;
    std::map< UidType, KRoomUserPtr >::iterator mit;
    for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"�� ���� ������ �̻�." )
                << BUILD_LOG( mit->first )
                << END_LOG;
            return false;
        }

        if( mit->second->IsPlaying() && ( mit->second->GetLoadingProgress() >= 0 && mit->second->GetLoadingProgress() < 100 ) )
        {
            return false;
        }
    }

    return true;
}

bool KRoomUserManager::IsAllPlayerStageLoaded()
{
	USERLIST_TYPE eType = UT_GAME;
    std::map< UidType, KRoomUserPtr >::iterator mit;
    for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"�� ���� ������ �̻�." )
                << BUILD_LOG( mit->first )
                << END_LOG;
            return false;
        }

        if( mit->second->IsPlaying() && !mit->second->IsStageLoaded() )
        {
            return false;
        }
    }

    return true;
}

//{{ 2009. 7. 20  ������	��� ��������
bool KRoomUserManager::IsAllPlayerAlive()
{
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

		if( mit->second->IsPlaying() && mit->second->IsDie() )
		{
			return false;
		}
	}

	return true;
}
//}}

bool KRoomUserManager::IsAllPlayerDie()
{
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

        if( mit->second->IsPlaying() && !mit->second->IsDie() )
        {
            return false;
        }
    }

    return true;
}

bool KRoomUserManager::IsAllPlayerHPReported()
{
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

        START_LOG( clog2, L"HP ����Ʈ �˻�." )
            << BUILD_LOG( mit->second->GetStateIDString() )
            << BUILD_LOG( mit->second->GetHP() );

        if( mit->second->GetStateID() == KRoomUserFSM::S_PLAY && mit->second->GetHP() < 0.f )
        {
            return false;
        }
    }

    return true;
}

bool KRoomUserManager::IsAllPlayerStageID()
{
	USERLIST_TYPE eType = UT_GAME;
	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
	{
		KRoomUserPtr pKRoomUserPtr = mit->second;
		if( pKRoomUserPtr == NULL )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( pKRoomUserPtr )
				<< END_LOG;

			return false;
		}

		if( pKRoomUserPtr->IsPlaying()	== true &&	// ���� �������̰�
			pKRoomUserPtr->IsDie()		== false )	// ���� �ʾҴٸ� �������� üũ�� �Ѵ�.
		{
			if( pKRoomUserPtr->GetStageID() == -1 )
			{
				return false;
			}
		}
	}

    return true;
}

bool KRoomUserManager::IsAllPlayerDungeonUnitInfoSet()
{
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

		if( mit->second->IsDungeonUnitInfo() == false )
		{
			return false;
		}

        if ( mit->second->GetStateID() != KRoomUserFSM::S_RESULT )
		{
			return false;
		}
	}

	return true;
}

//{{ 2007. 8. 27  ������  
bool KRoomUserManager::CheckAuthLevel( UidType nCID )
{
	USERLIST_TYPE eType = UT_GAME;
	std::map< UidType, KRoomUserPtr >::iterator mit;
	mit = m_mapRoomUser[eType].find( nCID );

	if( mit == m_mapRoomUser[eType].end() )
	{
		eType = UT_OBSERVER;
		mit = m_mapRoomUser[eType].find( nCID );

		if( mit == m_mapRoomUser[eType].end() )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}
	}

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-07	// �ڼ���
	switch( mit->second->GetAuthLevel() )
	{
	case SEnum::UAL_GM:
	case SEnum::UAL_DEVELOPER:
	case SEnum::UAL_SUPERDEVELOPER:
		return true;

	default:
		return false;
	}
#else
	if( mit->second->GetAuthLevel() <= SEnum::UAL_DEVELOPER  &&
		mit->second->GetAuthLevel() >= SEnum::UAL_GM )
		return true;
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

	return false;
}
//}}	

bool KRoomUserManager::IsAllPlayerSuccessResult()
{
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

		//{{ 2011. 07. 25	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		if( mit->second->IsPvpNpc() == true )
			continue;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		if( mit->second->GetStateID() != KRoomUserFSM::S_RESULT )
			continue;

        if( mit->second->IsSuccessResult() == false )
        {
            return false;
        }
    }

    return true;
}

//{{ 2009. 12. 4  ������	��彺ų
#ifdef GUILD_SKILL_TEST

bool KRoomUserManager::IsAllPlayerSameGuildMember()
{
	int iGuildUID = 0;

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
}

#endif GUILD_SKILL_TEST
//}}

void KRoomUserManager::AssignTeam( int iGameMode )
{
	USERLIST_TYPE eType = UT_GAME;
    for( int i = 0; i < m_nRoomSlot[eType]; i++ )
    {
        KRoomSlotPtr spRoomSlot = GetSlot( i );
        if( !spRoomSlot )
        {
            START_LOG( cerr, L"���� ������ �̻�." )
                << BUILD_LOG( i )
                << END_LOG;

            continue;
        }
        spRoomSlot->AssignTeam( iGameMode );
    }

    return;
}

bool KRoomUserManager::OpenSlot( int iSlotID )
{
    KRoomSlotPtr spRoomSlot = GetSlot( iSlotID );
    if( !spRoomSlot )
    {
        START_LOG( cerr, L"���� ������ �̻�." )
            << BUILD_LOG( iSlotID )
            << END_LOG;

        return false;
    }

    return spRoomSlot->Open();
}

bool KRoomUserManager::CloseSlot( int iSlotID )
{
    KRoomSlotPtr spRoomSlot = GetSlot( iSlotID );
    if( !spRoomSlot )
    {
        START_LOG( cerr, L"���� ������ �̻�." )
            << BUILD_LOG( iSlotID )
            << END_LOG;

        return false;
    }

    return spRoomSlot->Close();
}

bool KRoomUserManager::OpenSlotTeam( int iSlotID, OUT int& iTeamBalanceSlotID )
{
	KRoomSlotPtr spRoomSlot_01, spRoomSlot_02;
	iTeamBalanceSlotID = 0;

	spRoomSlot_01 = GetSlot( iSlotID );

	if( spRoomSlot_01 == NULL )
	{
		START_LOG( cerr, L"���� ������ �̻�." )
			<< BUILD_LOG( iSlotID )
			<< END_LOG;

		return false;
	}

	if( spRoomSlot_01->IsOpened() == true || spRoomSlot_01->IsOccupied() == true )
	{
		return false;
	}

	int nHalfSlot = GetNumTotalSlot() / 2;
	int	nStart, nEnd;

	if( nHalfSlot > iSlotID )
	{
		nStart	= nHalfSlot;
		nEnd	= GetNumTotalSlot();
	}
	else
	{
		nStart	= 0;
		nEnd	= nHalfSlot;
	}

	for( ; nStart < nEnd; ++nStart )
	{
		spRoomSlot_02 = GetSlot( nStart );

		if( spRoomSlot_02 == NULL )
		{
			START_LOG( cerr, L"���� ������ �̻�." )
				<< BUILD_LOG( iSlotID )
				<< BUILD_LOG( nHalfSlot )
				<< BUILD_LOG( nStart )
				<< END_LOG;

			continue;
		}

		//������ �����ְ� ����� �������.
		if( spRoomSlot_02->IsOpened() == true || spRoomSlot_02->IsOccupied() == true )
		{
			continue;
		}
		else
		{
			break;
		}
	}

	if( nStart < nEnd )
	{
		iTeamBalanceSlotID = nStart;

		if( spRoomSlot_01->Open() == true && spRoomSlot_02->Open() == true )
		{
			return true;
		}
	}

	return false;
}

bool KRoomUserManager::CloseSlotTeam( int iSlotID, OUT int& iTeamBalanceSlotID )
{
	KRoomSlotPtr spRoomSlot_01, spRoomSlot_02;
	iTeamBalanceSlotID = 0;

	spRoomSlot_01 = GetSlot( iSlotID );

	if( spRoomSlot_01 == NULL )
	{
		START_LOG( cerr, L"���� ������ �̻�." )
			<< BUILD_LOG( iSlotID )
			<< END_LOG;

		return false;
	}

	if( spRoomSlot_01->IsOpened() == false || spRoomSlot_01->IsOccupied() == true )
	{
		return false;
	}

	int nHalfSlot = GetNumTotalSlot() / 2;
	int	nStart, nEnd;

	if( nHalfSlot > iSlotID )
	{
		nStart	= nHalfSlot;
		nEnd	= GetNumTotalSlot();
	}
	else
	{
		nStart	= 0;
		nEnd	= nHalfSlot;
	}

	for( ; nStart < nEnd; ++nStart )
	{
		spRoomSlot_02 = GetSlot( nStart );

		if( spRoomSlot_02 == NULL )
		{
			START_LOG( cerr, L"���� ������ �̻�." )
				<< BUILD_LOG( iSlotID )
				<< BUILD_LOG( nHalfSlot )
				<< BUILD_LOG( nStart )
				<< END_LOG;

			continue;
		}

		if( spRoomSlot_02->IsOpened() == false || spRoomSlot_02->IsOccupied() == true )
		{
			continue;
		}
		else
		{
			break;
		}
	}

	if( nStart < nEnd )
	{
		iTeamBalanceSlotID = nStart;

		if( spRoomSlot_01->Close() == true && spRoomSlot_02->Close() == true )
		{
			return true;
		}
	}

	return false;
}

bool KRoomUserManager::ToggleOpenClose( int iSlotID )
{
    KRoomSlotPtr spRoomSlot = GetSlot( iSlotID );
    if( !spRoomSlot )
    {
        START_LOG( cerr, L"���� ������ �̻�." )
            << BUILD_LOG( iSlotID )
            << END_LOG;

        return false;
    }

    return spRoomSlot->ToggleOpenClose();
}

bool KRoomUserManager::GetRoomSlotInfo( int iSlotID, KRoomSlotInfo& kInfo, USERLIST_TYPE eType )
{
    KRoomSlotPtr spRoomSlot = GetSlot( iSlotID, eType );
    if( !spRoomSlot )
    {
        START_LOG( cerr, L"���� ������ �̻�." )
            << BUILD_LOG( iSlotID )
            << END_LOG;

        return false;
    }

    spRoomSlot->GetRoomSlotInfo( kInfo );
    return true;
}

bool KRoomUserManager::GetRoomSlotInfo( UidType nCID, KRoomSlotInfo& kInfo, USERLIST_TYPE eType )
{
	if( CheckUserlistType( eType ) == false )
	{
		START_LOG( cerr, L"��������Ʈ Ÿ�� �̻�.!" )
			<< BUILD_LOG( nCID )
			<< BUILD_LOG( (int)eType )
			<< END_LOG;
		return false;
	}

    KRoomUserPtr spRoomUser = GetUser( nCID, eType );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� �˻� ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    return GetRoomSlotInfo( spRoomUser->GetSlotID(), kInfo, eType );
}

void KRoomUserManager::GetRoomSlotInfo( std::vector< KRoomSlotInfo >& vecRoomSlotInfo, USERLIST_TYPE eType )
{
	if( CheckUserlistType( eType ) == false )
	{
		START_LOG( cerr, L"��������Ʈ Ÿ�� �̻�.!" )
			<< BUILD_LOG( (int)eType )
			<< END_LOG;
		return;
	}

    vecRoomSlotInfo.clear();

    for( int i = 0; i < m_nRoomSlot[eType]; i++ )
    {
        KRoomSlotInfo kInfo;
        GetRoomSlotInfo( i, kInfo, eType );

        vecRoomSlotInfo.push_back( kInfo );
    }
}

bool KRoomUserManager::GetRoomUserGS( UidType nCID, UidType& nGS )
{
    KRoomUserPtr spRoomUser = GetUser( nCID );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"������ ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    nGS = spRoomUser->GetGSUID();

    return true;
}

//{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION

bool KRoomUserManager::GetRoomUserGS( IN const std::wstring& wstrNickName, OUT UidType& nGS, OUT UidType& iUnitUID )
{
	std::map< UidType, KRoomUserPtr >::const_iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cwarn, L"������ ������ �̻�." )
				<< END_LOG;
			continue;
		}

		// ������ ã�Ҵ�!
		if( mit->second->GetNickName() == wstrNickName )
		{
			nGS = mit->second->GetGSUID();
			iUnitUID = mit->second->GetCID();
			return true;
		}
	}

	return false;
}

#endif SERV_INTEGRATION
//}}

void KRoomUserManager::GetUserList( IN const int iFlag_, OUT std::map< UidType, std::set< UidType > >& mapUidList_, IN const USERLIST_TYPE eType )
{
	if( CheckUserlistType( eType ) == false )
	{
		START_LOG( cerr, L"��������Ʈ Ÿ�� �̻�.!" )
			<< BUILD_LOG( (int)eType )
			<< END_LOG;
		return;
	}

    //mapUidList_.clear();

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

        switch( iFlag_ )
        {
        case UL_ALL:
            // operator[]()�� �ǵ����� ���. �����Ͱ� ���� ��� insert�Ǵ� ���� �����Ѵ�.
            mapUidList_[mit->second->GetGSUID()].insert( mit->second->GetCID() );
            break;
        case UL_BLUE_TEAM:
            if( mit->second->GetTeam() == CXSLRoom::TN_BLUE )
            {
                // operator[]()�� �ǵ����� ���. �����Ͱ� ���� ��� insert�Ǵ� ���� �����Ѵ�.
                mapUidList_[mit->second->GetGSUID()].insert( mit->second->GetCID() );
            }
            break;
        case UL_RED_TEAM:
            if( mit->second->GetTeam() == CXSLRoom::TN_RED )
            {
                // operator[]()�� �ǵ����� ���. �����Ͱ� ���� ��� insert�Ǵ� ���� �����Ѵ�.
                mapUidList_[mit->second->GetGSUID()].insert( mit->second->GetCID() );
            }
            break;
        case UL_PLAY:
			if( mit->second->IsPlaying() == true )
            {
                // operator[]()�� �ǵ����� ���. �����Ͱ� ���� ��� insert�Ǵ� ���� �����Ѵ�.
                mapUidList_[mit->second->GetGSUID()].insert( mit->second->GetCID() );
            }
            break;
        case UL_RESULT:
            if( mit->second->GetStateID() == KRoomUserFSM::S_RESULT )
            {
                // operator[]()�� �ǵ����� ���. �����Ͱ� ���� ��� insert�Ǵ� ���� �����Ѵ�.
                mapUidList_[mit->second->GetGSUID()].insert( mit->second->GetCID() );
            }
            break;
			//{{ 2011. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case UL_NPC_LOAD_USER:
			if( mit->second->IsBattleFieldNpcLoad() == true )
			{
				// operator[]()�� �ǵ����� ���. �����Ͱ� ���� ��� insert�Ǵ� ���� �����Ѵ�.
				mapUidList_[mit->second->GetGSUID()].insert( mit->second->GetCID() );
			}
			break;
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
        default:
            START_LOG( cerr, L"Never get here!" );
            break;
        } // end switch

    } // end for
}

//{{ 2011. 05. 18	������	���� ȹ�� ���� ����
#ifdef SERV_GET_RANDOM_ITEM_LOGIC_FIX
void KRoomUserManager::GetUnitUIDList( OUT std::vector< UidType >& vecUidList )
{
	vecUidList.clear();

	std::map< UidType, KRoomUserPtr >::const_iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

        vecUidList.push_back( mit->first );
	}
}
#endif SERV_GET_RANDOM_ITEM_LOGIC_FIX
//}}

void KRoomUserManager::DecideWinTeam( unsigned char ucGameType, std::vector< int >& vecWinTeam )
{
	USERLIST_TYPE eType = UT_GAME;

    std::map< UidType, KRoomUserPtr >::iterator mit;
    std::map< int, std::pair< int, float > > mapResultInfo;
    std::map< int, std::pair< int, float > >::iterator mitTeam;
    int iMaxAlive = -1;
    float fMaxHP = -1;
    int iMaxKill = -1;
    std::map< int, int > mapTeamNumKill;
    std::map< int, int >::iterator mitTeamNumKill;

    vecWinTeam.clear();
    switch( ucGameType )
    {
    case CXSLRoom::PGT_TEAM:
        for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
        {
            if( !mit->second )
            {
                START_LOG( cerr, L"�� ���� ������ �̻�." )
                    << BUILD_LOG( mit->first )
                    << END_LOG;
                continue;
            }

            mitTeam = mapResultInfo.find( mit->second->GetTeam() );
            if( mitTeam == mapResultInfo.end() )
            {
                mapResultInfo.insert( std::make_pair( mit->second->GetTeam(), std::make_pair( ( mit->second->IsDie() ? 0 : 1 ), mit->second->GetHP() ) ) );
            }
            else
            {
                mitTeam->second.first += ( mit->second->IsDie() ? 0 : 1 );
                mitTeam->second.second += mit->second->GetHP();
            }
        }
        for( mitTeam = mapResultInfo.begin(); mitTeam != mapResultInfo.end(); mitTeam++ )
        {
            if( mitTeam->second.first > iMaxAlive )
            {
                iMaxAlive = mitTeam->second.first;
                fMaxHP = mitTeam->second.second;
                vecWinTeam.clear();
                vecWinTeam.push_back( mitTeam->first );
                continue;
            }
            else if( mitTeam->second.first < iMaxAlive )
            {
                continue;
            }

            if( mitTeam->second.second > fMaxHP )
            {
                fMaxHP = mitTeam->second.second;
                vecWinTeam.clear();
                vecWinTeam.push_back( mitTeam->first );
                continue;
            }
            else if( mitTeam->second.second < fMaxHP )
            {
                continue;
            }

            vecWinTeam.push_back( mitTeam->first );
        }
        break;
    case CXSLRoom::PGT_TEAM_DEATH:
        for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
        {
            if( !mit->second )
            {
                START_LOG( cerr, L"�� ���� ������ �̻�." )
                    << BUILD_LOG( mit->first )
                    << END_LOG;
                continue;
            }

            if( !mit->second->IsPlaying() )
            {
                continue;
            }

            mitTeamNumKill = mapTeamNumKill.find( mit->second->GetTeam() );
            if( mitTeamNumKill == mapTeamNumKill.end() )
            {
                mapTeamNumKill.insert( std::make_pair( mit->second->GetTeam() , mit->second->GetNumKill() ) );
            }
            else
            {
                mitTeamNumKill->second += mit->second->GetNumKill();
            }
        }

        for( mitTeamNumKill = mapTeamNumKill.begin(); mitTeamNumKill != mapTeamNumKill.end(); mitTeamNumKill++ )
        {
            if( mitTeamNumKill->second > iMaxKill )
            {
                iMaxKill = mitTeamNumKill->second;
                vecWinTeam.clear();
                vecWinTeam.push_back( mitTeamNumKill->first );
            }
            else if( mitTeamNumKill->second == iMaxKill )
            {
                vecWinTeam.push_back( mitTeamNumKill->first );
            }
        }
        break;
    case CXSLRoom::PGT_SURVIVAL:
        for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
        {
            if( !mit->second )
            {
                START_LOG( cerr, L"�� ���� ������ �̻�." )
                    << BUILD_LOG( mit->first )
                    << END_LOG;
                continue;
            }

            if( mit->second->GetStateID() != KRoomUserFSM::S_RESULT )
            {
                continue;
            }

            if( mit->second->GetNumKill() > iMaxKill )
            {
                iMaxKill = mit->second->GetNumKill();
                vecWinTeam.clear();
                vecWinTeam.push_back( mit->second->GetTeam() );
            }
        }
        break;
    default:
        START_LOG( cerr, L"�̻��� ���Ӹ��." )
            << BUILD_LOGc( ucGameType )
            << END_LOG;
    }
}

bool KRoomUserManager::IsAnyTeamEliminated()
{
	USERLIST_TYPE eType = UT_GAME;

    std::map< int, bool > mapIsDie;
    std::map< int, bool >::iterator mitIsDie;
    std::map< UidType, KRoomUserPtr >::iterator mit;
    for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"�� ���� ������ �̻�." )
                << BUILD_LOG( mit->first )
                << END_LOG;
            continue;
        }

        mitIsDie = mapIsDie.find( mit->second->GetTeam() );
        if( mitIsDie == mapIsDie.end() )
        {
            mapIsDie.insert( std::make_pair( mit->second->GetTeam(), mit->second->IsDie() ) );
        }
        else
        {
            if( !mit->second->IsDie() )
            {
                mitIsDie->second = false;
            }
        }
    }

    for( mitIsDie = mapIsDie.begin(); mitIsDie != mapIsDie.end(); mitIsDie++ )
    {
        if( mitIsDie->second )
        {
            START_LOG( clog, L"������ ��� ���� ���� ����." );
            return true;
        }
    }

    START_LOG( clog, L"������ ��� ���� ���� ����." );
    return false;
}

bool KRoomUserManager::IsOneTeamExist()
{
	USERLIST_TYPE eType = UT_GAME;
    if( m_mapRoomUser[eType].empty() )
    {
        return true;
    }

    std::map< UidType, KRoomUserPtr >::iterator mit;
    for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"�� ���� ������ �̻�." )
                << BUILD_LOG( mit->first )
                << END_LOG;

            continue;
        }
        else
        {
            break;
        }
    }

    if( mit == m_mapRoomUser[eType].end() )
    {
        START_LOG( cwarn, L"�� ������ ���ų� ������ �̻�." )
            << BUILD_LOG( GetNumMember() )
            << BUILD_LOG( ( int )m_mapRoomUser[eType].size() )
            << END_LOG;

        return true;
    }

    int iTeam = mit->second->GetTeam();
    for( mit++; mit != m_mapRoomUser[eType].end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"�� ���� ������ �̻�." )
                << BUILD_LOG( mit->first )
                << END_LOG;
            continue;
        }

        if( mit->second->GetTeam() != iTeam )
        {
            START_LOG( clog, L"�� �� �̻� ������." );
            return false;
        }
    }

    START_LOG( clog, L"�� ���� ������." );
    return true;
}

bool KRoomUserManager::IsAnyoneReachObjectiveNumKill( int nKill )
{
	USERLIST_TYPE eType = UT_GAME;
    std::map< UidType, KRoomUserPtr >::iterator mit;
    for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"�� ���� ������ �̻�." )
                << BUILD_LOG( mit->first )
                << END_LOG;
            continue;
        }

		if( mit->second->IsPlaying() == false )
			continue;

        if( mit->second->GetNumKill() >= nKill )
        {
            return true;
        }
    }

    return false;
}



bool KRoomUserManager::IsAnyTeamReachObjectiveNumKill( int nKill )
{
    std::map< int, int >::iterator mitTeamNumKill;

    for( mitTeamNumKill = m_mapTeamNumKill.begin(); mitTeamNumKill != m_mapTeamNumKill.end(); mitTeamNumKill++ )
    {
        if( mitTeamNumKill->second >= nKill )
        {
            return true;
        }
    }

    return false;
}



//#ifdef PVP_BOSS_COMBAT_TEST
//	bool KRoomUserManager::GetIsBoss( UidType nCID, bool& bIsBoss )
//	{
//		USERLIST_TYPE eType = UT_GAME;
//		KRoomUserPtr spRoomUser = GetUser( nCID, eType );
//		if( !spRoomUser )
//		{
//			START_LOG( cerr, L"������ ����." )
//				<< BUILD_LOG( nCID )
//				<< END_LOG;
//
//			return false;
//		}
//
//		KRoomSlotPtr spRoomSlot = GetSlot( spRoomUser->GetSlotID(), eType );
//		if( !spRoomSlot )
//		{
//			START_LOG( cerr, L"���� ������ �̻�." )
//				<< BUILD_LOG( spRoomUser->GetSlotID() )
//				<< END_LOG;
//
//			return false;
//		}
//
//		bIsBoss = spRoomUser->GetIsBoss();
//		return true;
//	}
//
//
//	bool KRoomUserManager::SetIsBoss( UidType nCID, bool bIsBoss )
//	{
//		USERLIST_TYPE eType = UT_GAME;
//		KRoomUserPtr spRoomUser = GetUser( nCID, eType );
//		if( !spRoomUser )
//		{
//			START_LOG( cerr, L"������ ����." )
//				<< BUILD_LOG( nCID )
//				<< END_LOG;
//
//			return false;
//		}
//
//		KRoomSlotPtr spRoomSlot = GetSlot( spRoomUser->GetSlotID(), eType );
//		if( !spRoomSlot )
//		{
//			START_LOG( cerr, L"���� ������ �̻�." )
//				<< BUILD_LOG( spRoomUser->GetSlotID() )
//				<< END_LOG;
//
//			return false;
//		}
//
//		spRoomUser->SetIsBoss( bIsBoss );
//		return true;
//	}
//
//
//
//	bool KRoomUserManager::PickRandomBoss()
//	{
//		USERLIST_TYPE eType = UT_GAME;
//
//		std::vector< KRoomUserPtr > vecRedTeamUser;
//		std::vector< KRoomUserPtr > vecBlueTeamUser;
//
//		std::map< UidType, KRoomUserPtr >::iterator mit;
//		for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
//		{
//			KRoomUserPtr spRoomUser = mit->second;
//			if( !spRoomUser )
//			{
//				START_LOG( cerr, L"������ �����Ͱ� �߸��Ǿ���." )
//					<< BUILD_LOG( mit->first )
//					<< END_LOG;
//
//				continue;
//			}
//
//
//			spRoomUser->SetIsBoss( false );
//
//			switch( (CXSLRoom::TEAM_NUM) spRoomUser->GetTeam() )
//			{
//			case CXSLRoom::TN_RED:
//				{
//					vecRedTeamUser.push_back( spRoomUser );
//				} break;
//
//			case CXSLRoom::TN_BLUE:
//				{
//					vecBlueTeamUser.push_back( spRoomUser );
//				} break;
//			}
//		}
//
//
//		if( true == vecRedTeamUser.empty() )
//			return false;
//
//		if( true == vecBlueTeamUser.empty() )
//			return false;
//
//		int iRedTeamBossIndex = rand() % (int) vecRedTeamUser.size();
//		int iBlueTeamBossIndex = rand() % (int) vecBlueTeamUser.size();
//
//
//		vecRedTeamUser[iRedTeamBossIndex]->SetIsBoss( true );
//		vecBlueTeamUser[iBlueTeamBossIndex]->SetIsBoss( true );
//
//
//		return true;
//	}
//	
//	bool KRoomUserManager::IsAnyTeamBossDead( OUT bool& bRedTeamBossDead, OUT bool& bBlueTeamBossDead )
//	{
//		USERLIST_TYPE eType = UT_GAME;
//
//		std::map< UidType, KRoomUserPtr >::iterator mit;
//		KRoomUserPtr spRoomUser;
//
//		bool bRedTeamBossAlive = false;
//		bool bBlueTeamBossAlive = false;
//
//
//		for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
//		{
//			spRoomUser = mit->second;
//
//			if( !spRoomUser )
//			{
//				START_LOG( cerr, L"������ �����Ͱ� �߸��Ǿ���." )
//					<< BUILD_LOG( mit->first )
//					<< END_LOG;
//
//				continue;
//			}
//
//			if( true == spRoomUser->GetIsBoss() )
//			{
//				switch( (CXSLRoom::TEAM_NUM) spRoomUser->GetTeam() )
//				{
//				case CXSLRoom::TN_RED:
//					{
//						if( spRoomUser->GetNumDie() <= 0 && 
//							!spRoomUser->IsDie() )
//						{
//							bRedTeamBossAlive = true;
//						}	
//					} break;
//
//				case CXSLRoom::TN_BLUE:
//					{
//						if( spRoomUser->GetNumDie() <= 0 && 
//							!spRoomUser->IsDie() )
//						{
//							bBlueTeamBossAlive = true;
//						}	
//
//					} break;
//				}
//			}
//		}
//
//
//		bRedTeamBossDead = !bRedTeamBossAlive;
//		bBlueTeamBossDead = !bBlueTeamBossAlive;
//
//		return bRedTeamBossDead || bBlueTeamBossDead;
//
//	}
//#endif PVP_BOSS_COMBAT_TEST


//{{ 2012. 11. 21	������		�����ڿ��� ĳ���� ���� ���� ������
#ifdef SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
void KRoomUserManager::GetEnterCashShopUserList( OUT std::vector< UidType >& vecEnterCashShopUser ) const
{
	vecEnterCashShopUser.clear();

	std::map< UidType, KRoomUserPtr >::const_iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( IS_NULL( mit->second ) )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->IsEnterCashShopUser() == true )
		{
            vecEnterCashShopUser.push_back( mit->second->GetCID() );
		}
	}
}
#endif SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
//}}


int KRoomUserManager::GetMaxKillUnit()
{
	USERLIST_TYPE eType = UT_GAME;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	int iNumMaxKill = 0;

	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->IsPlaying() == false )
			continue;

		if( mit->second->GetNumKill() >= iNumMaxKill )
		{
			iNumMaxKill = mit->second->GetNumKill();
		}
	}

	return iNumMaxKill;
}

int KRoomUserManager::GetMaxKillTeam()
{
	std::map< int, int >::iterator mitTeamNumKill;
	int iNumMaxKill = 0;

	for( mitTeamNumKill = m_mapTeamNumKill.begin(); mitTeamNumKill != m_mapTeamNumKill.end(); mitTeamNumKill++ )
	{
		if( mitTeamNumKill->second > iNumMaxKill )
		{
			iNumMaxKill = mitTeamNumKill->second;
		}
	}

	return iNumMaxKill;
}

int KRoomUserManager::GetTeamScore( int iTeam )
{
    int iRet;
    std::map< int, int >::iterator mitTeamNumKill = m_mapTeamNumKill.find( iTeam );
    if( mitTeamNumKill == m_mapTeamNumKill.end() )
    {
        iRet = 0;
    }
    else
    {
        iRet = mitTeamNumKill->second;
    }

    return iRet;
}

bool KRoomUserManager::AddRewardEXP( IN UidType nCID, OUT int iEXP )
{
	USERLIST_TYPE eType = UT_GAME;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	mit = m_mapRoomUser[eType].find( nCID );

	if( mit == m_mapRoomUser[eType].end() )
	{
		START_LOG( cerr, L"������ POINTER �̻�." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	mit->second->AddRewardEXP( iEXP );

	return true;
}

//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
bool KRoomUserManager::AddDungeonRewardEXP( IN const UidType iUnitUID,
										    IN const int iEXP,
										    IN const int iDungeonID,
										    IN const char cDifficultyLevel,
										    IN float fLevelFactor,
										    OUT KERM_NPC_UNIT_DIE_NOT& kInfo )
{
	// ���� ���ǿ� ���ؼ� ����ġ�� ���� ���� ���
	if( iEXP <= 0 && fLevelFactor <= 0.f )
	{
		// �ڽ��� ������ ����ġ�� 0�̶�� �˷���� �Ѵ�.
		KEXPData kEXPData;
		kEXPData.m_iEXP = 0;
		kEXPData.m_iPartyEXP = 0;
		kEXPData.m_iSocketOptEXP = 0;

		kInfo.m_EXPList.insert( std::make_pair( iUnitUID, kEXPData ) );

		return true;
	}
	else if( iEXP <= 0 )
	{
		return false;
	}

	USERLIST_TYPE eType = UT_GAME;

	// EXP ���� ���� ã��
	std::map< UidType, KRoomUserPtr >::iterator mit = m_mapRoomUser[eType].find( iUnitUID );
	if( mit == m_mapRoomUser[eType].end() )
	{
		START_LOG( cerr, L"�ش� ������ ã�� �� �����ϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iEXP )
			<< BUILD_LOG( iDungeonID )
			<< END_LOG;

		return false;
	}
	
	if( mit->second->IsDie() == true )
	{
		START_LOG( clog, L"�ش� ������ �׾����ϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iEXP )
			<< BUILD_LOG( iDungeonID )
			<< END_LOG;

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// ����ġ ���� ó��
	//////////////////////////////////////////////////////////////////////////
	int iBonusEXP = 0;
	int iLivePartyMember = GetLiveMember();
	
	// ���� ���� ���� ���
	AddHaveExpInDungeon( mit->second->GetCID(), fLevelFactor > 0.0f );

	if( CXSLDungeon::IsSecretDungeon( iDungeonID ) == true ||
		CXSLDungeon::IsHenirDungeon( iDungeonID ) == true )
	{
		iBonusEXP = static_cast<int>( iEXP * GetPartyBonusRate( iLivePartyMember ) * SiKResultProcess()->GetDungeonPartyDifficultyBonusRate( CXSLDungeon::DL_EXPERT ) );
	}
	else
	{
		iBonusEXP = static_cast<int>( iEXP * GetPartyBonusRate( iLivePartyMember ) * SiKResultProcess()->GetDungeonPartyDifficultyBonusRate( cDifficultyLevel ) );
	}

	// GSUser�� ������Ʈ�ϰ� Ŭ���̾�Ʈ ȭ�鿡 �Ѹ� EXP ������
	KEXPData kEXPData;
	kEXPData.m_iEXP			= iEXP;
	kEXPData.m_iPartyEXP	= iBonusEXP;

	//////////////////////////////////////////////////////////////////////////		
	// ���Ͽɼ� ���ʽ� ����
	bool bSocketOptBonus = false;
	int iSocketOptEXP		= 0;
	int iSocketOptPartyEXP	= 0;
	
	const float fAddSocketOptExpRate = mit->second->GetBonusRate( KRoomUserInfo::BT_SOCKET_OPT_EXP_RATE );
	if( fAddSocketOptExpRate > 0.0f )
	{
		iSocketOptEXP		+= static_cast<int>( kEXPData.m_iEXP * fAddSocketOptExpRate );
		iSocketOptPartyEXP	+= static_cast<int>( kEXPData.m_iPartyEXP * fAddSocketOptExpRate );

		bSocketOptBonus = true;
	}

	if( iLivePartyMember >= 2 )
	{
		if( IsAllPlayerSameGuildMember() == true )
		{
			const float fAddGuildBonusExpRate = mit->second->GetBonusRate( KRoomUserInfo::BT_GUILD_BONUS_EXP_RATE );

			if( fAddGuildBonusExpRate > 0.0f )
			{
				iSocketOptEXP		+= static_cast<int>( kEXPData.m_iEXP * fAddGuildBonusExpRate );
				iSocketOptPartyEXP	+= static_cast<int>( kEXPData.m_iPartyEXP * fAddGuildBonusExpRate );

				bSocketOptBonus = true;
			}
		}
	}

	// ���Ͽɼ� ���ʽ�
	kEXPData.m_iSocketOptEXP	 = iSocketOptEXP + iSocketOptPartyEXP;

	// ���Ͽɼ� ���ʽ�
	if( bSocketOptBonus == true  &&  kEXPData.m_iSocketOptEXP == 0  &&  fLevelFactor > 0.0f )
	{
		kEXPData.m_iSocketOptEXP = iSocketOptEXP = 1; // ���ʽ� ����ġ �ּҰ� 1�ֱ�
	}
	// ���Ͽɼ� ���ʽ� �ջ�
	const int iEXPWithBonus		 = kEXPData.m_iEXP + iSocketOptEXP;
	const int iPartyEXPWithBonus = kEXPData.m_iPartyEXP + iSocketOptPartyEXP;

	//{{ 2012. 12. 14	������	���� �ý��� ���� ����ġ ó�� ����
#ifdef SERV_REMOVE_SYSTEM_TUTORIAL_REWARD_IN_DUNGEON
#else
	// �ش��������� ���ڰ� ������ ���ڿ��� ����ġ�� �ش�.
	//std::vector< UidType > vecTakeEXPList; //������ EXP ���� ���ڴ� ��������.
	//{
	//	std::vector< UidType > vecTemp;
	//	mit->second->GetTutorialUIDList( vecTemp );

	//	for( int i = 0; i < (int)vecTemp.size(); ++i )
	//	{
	//		KRoomUserPtr spRoomUser = GetUser( vecTemp[i] );

	//		//���ڰ� �������.
	//		if( spRoomUser != NULL )
	//		{
	//			spRoomUser->AddRewardEXP( iEXPWithBonus );
	//			spRoomUser->AddRewardPartyEXP( iPartyEXPWithBonus );

	//			vecTakeEXPList.push_back( vecTemp[i] );
	//		}
	//	}
	//}

	//if( vecTakeEXPList.empty() == true )
#endif SERV_REMOVE_SYSTEM_TUTORIAL_REWARD_IN_DUNGEON
	//}}
	{
		//���κ� EXP ó��..
		mit->second->AddRewardEXP( iEXPWithBonus );
		mit->second->AddRewardPartyEXP( iPartyEXPWithBonus );

		std::map< UidType, KEXPData >::iterator mitTemp;
		mitTemp = kInfo.m_EXPList.find( mit->first );
		if( mitTemp == kInfo.m_EXPList.end() )
		{
			kInfo.m_EXPList.insert( std::make_pair( mit->first, kEXPData ) );
		}
		else
		{
			mitTemp->second.m_iEXP += kEXPData.m_iEXP;
			mitTemp->second.m_iPartyEXP += kEXPData.m_iPartyEXP;
			mitTemp->second.m_iSocketOptEXP += kEXPData.m_iSocketOptEXP;
		}

		START_LOG( clog, L"::: ���� ���� EXP :::" )
			<< BUILD_LOG( mit->second->GetCID() )
			<< BUILD_LOG( kEXPData.m_iEXP )
			<< BUILD_LOG( kEXPData.m_iPartyEXP )
			<< BUILD_LOG( kEXPData.m_iSocketOptEXP )
			;
	}
	//{{ 2012. 12. 14	������	���� �ý��� ���� ����ġ ó�� ����
#ifdef SERV_REMOVE_SYSTEM_TUTORIAL_REWARD_IN_DUNGEON
#else
	//else
	//{
	//	// ���ڿ��� ����ġ�� �� ��� ó��
	//	for( int i = 0; i < (int)vecTakeEXPList.size(); ++i )
	//	{
	//		std::map< UidType, KEXPData >::iterator mitTemp;
	//		mitTemp = kInfo.m_EXPList.find( vecTakeEXPList[i] );
	//		if( mitTemp == kInfo.m_EXPList.end() )
	//		{
	//			kInfo.m_EXPList.insert( std::make_pair( vecTakeEXPList[i], kEXPData ) );
	//		}
	//		else
	//		{
	//			mitTemp->second.m_iEXP += kEXPData.m_iEXP;
	//			mitTemp->second.m_iPartyEXP += kEXPData.m_iPartyEXP;
	//			mitTemp->second.m_iSocketOptEXP += kEXPData.m_iSocketOptEXP;
	//		}
	//	}
	//}
#endif SERV_REMOVE_SYSTEM_TUTORIAL_REWARD_IN_DUNGEON
	//}}

	return true;
}

#else

//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
//#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
bool KRoomUserManager::AddDungeonRewardEXP( IN const int iEXP, IN const int iDungeonID, IN const int iNpcLv, OUT KERM_NPC_UNIT_DIE_NOT& kInfo, IN const char cDifficultyLevel )
//#else
//{{ 2010. 04. 30  ������	��д��� ����
//#ifdef SERV_SECRET_HELL
//bool KRoomUserManager::AddDungeonRewardEXP( IN int iEXP, IN int iDungeonID, IN int iNpcLv, OUT KERM_NPC_UNIT_DIE_NOT& kInfo )
//#else
//bool KRoomUserManager::AddDungeonRewardEXP( IN int iEXP, IN int iDungeonID, OUT KERM_NPC_UNIT_DIE_NOT& kInfo )
//#endif SERV_SECRET_HELL
//}}
//#endif SERV_DUNGEON_DIFFICULTY_REVISION
//}}
{
	USERLIST_TYPE eType = UT_GAME;

	if( iEXP <= 0 )
	{
		return false;
	}

	//�⺻ : exp/��Ƽ���� | �߰����ʽ� : personExp * ��Ƽ������ Ȯ��

	// 0. �̸� ���س��°�
	const int iLiveMember = GetLiveMember();

	// 1. �⺻ EXP
	int iRewardEXP		= static_cast<int>( iEXP / iLiveMember );
	int iRewardPartyEXP	= 0;

	// 2. PARTY EXP
	if( iLiveMember >= 2 )
	{
		//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
		if( CXSLDungeon::IsSecretDungeon( iDungeonID ) == true ||
			CXSLDungeon::IsHenirDungeon( iDungeonID ) == true )
		{
			iRewardPartyEXP = static_cast<int>( iRewardEXP * SiKResultProcess()->GetDungeonPartyBonusRate( iLiveMember ) * SiKResultProcess()->GetDungeonModePartyBonusRate( CXSLDungeon::DT_SECRET ) );
		}
		else
		{
			iRewardPartyEXP = static_cast<int>( iRewardEXP * SiKResultProcess()->GetDungeonPartyBonusRate( iLiveMember ) * SiKResultProcess()->GetDungeonPartyDifficultyBonusRate( cDifficultyLevel ) );
		}
#else
		iRewardPartyEXP = static_cast<int>( iRewardEXP * SiKResultProcess()->GetDungeonPartyBonusRate( iLiveMember ) );
#endif SERV_DUNGEON_DIFFICULTY_REVISION
		//}}	
	}	

	//{{ 2009. 12. 4  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	bool bAllPlayerSameGuildMemeber = false;
	if( iLiveMember >= 2 )
	{
		bAllPlayerSameGuildMemeber = IsAllPlayerSameGuildMember();
	}
#endif GUILD_SKILL_TEST
	//}}

	//EXP ���.
	std::map< UidType, KRoomUserPtr >::iterator mit;

	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		// ���� ���� ������..
		if( mit->second->IsDie() == true )
			continue;

		//////////////////////////////////////////////////////////////////////////
		// ����ġ ���� ó��
		//{{ 2010. 04. 30  ������	��д��� ����
#ifdef SERV_SECRET_HELL
		const float fDungeonBalRate = CXSLDungeon::CheckDungeonBalRateByNpcLv( mit->second->GetLevel(), iNpcLv );
#else
		const float fDungeonBalRate = CheckDungeonBalRate( mit->second->GetLevel(), iDungeonID );
#endif SERV_SECRET_HELL
		//}}

		//{{ 2011. 03. 08	��μ� ����ġ�� ȹ���� ������ ����Ʈ �Ϸ� ���� ����
#ifdef SERV_EXP_ACQUISITION_CONDITION_CHANGE
		AddHaveExpInDungeon( mit->second->GetCID(), fDungeonBalRate > 0.0f );
#endif SERV_EXP_ACQUISITION_CONDITION_CHANGE
		//}}

		// GSUser�� ������Ʈ�ϰ� Ŭ���̾�Ʈ ȭ�鿡 �Ѹ� EXP ������
		KEXPData kEXPData;
		kEXPData.m_iEXP			= static_cast<int>( iRewardEXP * fDungeonBalRate );
		kEXPData.m_iPartyEXP	= static_cast<int>( iRewardPartyEXP * fDungeonBalRate );

		//{{ 2009. 5. 12  ������	���Ͽɼ� ���ʽ� ����
		bool bSocketOptBonus = false;
		int iSocketOptEXP		= 0;
		int iSocketOptPartyEXP	= 0;

		//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
		//#ifdef SERV_ROOMUSERINFO_REFAC
		const float fAddSocketOptExpRate = mit->second->GetBonusRate( KRoomUserInfo::BT_SOCKET_OPT_EXP_RATE );
		//#endif SERV_ROOMUSERINFO_REFAC
		//}}
		if( fAddSocketOptExpRate > 0.0f )
		{
			iSocketOptEXP		+= static_cast<int>( kEXPData.m_iEXP * fAddSocketOptExpRate );
			iSocketOptPartyEXP	+= static_cast<int>( kEXPData.m_iPartyEXP * fAddSocketOptExpRate );

			bSocketOptBonus = true;
		}

		//{{ 2009. 12. 4  ������	��彺ų
#ifdef GUILD_SKILL_TEST		
		if( bAllPlayerSameGuildMemeber )
		{
			//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
			//#ifdef SERV_ROOMUSERINFO_REFAC
			const float fAddGuildBonusExpRate = mit->second->GetBonusRate( KRoomUserInfo::BT_GUILD_BONUS_EXP_RATE );
			//#endif SERV_ROOMUSERINFO_REFAC
			//}}
			if( fAddGuildBonusExpRate > 0.0f )
			{
				iSocketOptEXP		+= static_cast<int>( kEXPData.m_iEXP * fAddGuildBonusExpRate );
				iSocketOptPartyEXP	+= static_cast<int>( kEXPData.m_iPartyEXP * fAddGuildBonusExpRate );

				bSocketOptBonus = true;
			}
		}
#endif GUILD_SKILL_TEST
		//}}		


		// ���Ͽɼ� ���ʽ�
		kEXPData.m_iSocketOptEXP	 = iSocketOptEXP + iSocketOptPartyEXP;

		// ���Ͽɼ� ���ʽ�
		if( bSocketOptBonus == true  &&  kEXPData.m_iSocketOptEXP == 0  &&  fDungeonBalRate > 0.0f )
		{
			kEXPData.m_iSocketOptEXP = iSocketOptEXP = 1; // ���ʽ� ����ġ �ּҰ� 1�ֱ�
		}

		// ���Ͽɼ� ���ʽ� �ջ�
		const int iEXPWithBonus		 = kEXPData.m_iEXP + iSocketOptEXP;
		const int iPartyEXPWithBonus = kEXPData.m_iPartyEXP + iSocketOptPartyEXP;

		//080408.hoons. �ش��������� ���ڰ� ������ ���ڿ��� ����ġ�� �ش�.
		std::vector< UidType > vecTakeEXPList; //������ EXP ���� ���ڴ� ��������.
		{
			std::vector< UidType > vecTemp;
			mit->second->GetTutorialUIDList( vecTemp );

			for( int i = 0; i < (int)vecTemp.size(); ++i )
			{
				KRoomUserPtr spRoomUser = GetUser( vecTemp[i] );

				//���ڰ� �������.
				if( spRoomUser != NULL )
				{
					spRoomUser->AddRewardEXP( iEXPWithBonus );
					spRoomUser->AddRewardPartyEXP( iPartyEXPWithBonus );

					vecTakeEXPList.push_back( vecTemp[i] );
				}
			}
		}

		if( vecTakeEXPList.empty() == true )
		{
			//���κ� EXP ó��..
			mit->second->AddRewardEXP( iEXPWithBonus );
			mit->second->AddRewardPartyEXP( iPartyEXPWithBonus );

			std::map< UidType, KEXPData >::iterator mitTemp;
			mitTemp = kInfo.m_EXPList.find( mit->first );
			if( mitTemp == kInfo.m_EXPList.end() )
			{
				kInfo.m_EXPList.insert( std::make_pair( mit->first, kEXPData ) );
			}
			else
			{
				mitTemp->second.m_iEXP += kEXPData.m_iEXP;
				mitTemp->second.m_iPartyEXP += kEXPData.m_iPartyEXP;
				mitTemp->second.m_iSocketOptEXP += kEXPData.m_iSocketOptEXP;
			}

			START_LOG( clog, L"::: ���� ���� EXP :::" )
				<< BUILD_LOG( mit->second->GetCID() )
				<< BUILD_LOG( kEXPData.m_iEXP )
				<< BUILD_LOG( kEXPData.m_iPartyEXP )
				<< BUILD_LOG( kEXPData.m_iSocketOptEXP )
				;
		}
		else
		{
			// ���ڿ��� ����ġ�� �� ��� ó��
			for( int i = 0; i < (int)vecTakeEXPList.size(); ++i )
			{
				std::map< UidType, KEXPData >::iterator mitTemp;
				mitTemp = kInfo.m_EXPList.find( vecTakeEXPList[i] );
				if( mitTemp == kInfo.m_EXPList.end() )
				{
					kInfo.m_EXPList.insert( std::make_pair( vecTakeEXPList[i], kEXPData ) );
				}
				else
				{
					mitTemp->second.m_iEXP += kEXPData.m_iEXP;
					mitTemp->second.m_iPartyEXP += kEXPData.m_iPartyEXP;
					mitTemp->second.m_iSocketOptEXP += kEXPData.m_iSocketOptEXP;
				}
			}
		}		
	}

	return true;
}
#endif SERV_REFORM_ITEM_DROP
//}}

bool KRoomUserManager::GetRewardEXP( IN UidType nCID, OUT int& iEXP )
{
	USERLIST_TYPE eType = UT_GAME;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	mit = m_mapRoomUser[eType].find( nCID );

	if( mit == m_mapRoomUser[eType].end() )
	{
		START_LOG( cerr, L"������ POINTER �̻�." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	iEXP = mit->second->GetRewardEXP();

	return true;
}

bool KRoomUserManager::GetRewardPartyEXP( IN UidType nCID, OUT int& iEXP )
{
	USERLIST_TYPE eType = UT_GAME;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	mit = m_mapRoomUser[eType].find( nCID );

	if( mit == m_mapRoomUser[eType].end() )
	{
		START_LOG( cerr, L"������ POINTER �̻�." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	iEXP = mit->second->GetRewardPartyEXP();

	return true;
}

//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
bool KRoomUserManager::AddDungeonRewardED( IN int iED, IN UidType iUserUID, IN float fPartyBonusRate, IN int iDungeonID, IN char cDifficultyLevel, OUT KEGS_GET_ITEM_NOT& kInfo )
{
	kInfo.m_mapGetED.clear();

	if( iED <= 0 )
	{
		START_LOG( cwarn, L"ED�� 0�ΰ�쵵 �ֳ�?" )
			<< BUILD_LOG( iED )
			<< END_LOG;

		return false;
	}

	//ED ���.
	std::map< UidType, KRoomUserPtr >::iterator mit = m_mapRoomUser[UT_GAME].find( iUserUID );
	if( mit == m_mapRoomUser[UT_GAME].end() )
	{

		START_LOG( cerr, L"�������� �ʴ� �������ϴ�." )
			<< BUILD_LOG( iUserUID )
			<< END_LOG;
		return false;
	}

	KRoomUserPtr spEDRewardRoomUser = mit->second;
	if( spEDRewardRoomUser == NULL )
	{
		START_LOG( cerr, L"KRoomUserPtr�� NULL�� ���·� �� �ִ�?" )
			<< BUILD_LOG( mit->first )
			<< END_LOG;

		return false;
	}

	// ���� ���� �������Ը� ����
	if( spEDRewardRoomUser->IsDie() == true )
	{
		START_LOG( cwarn, L"���� ������ �������� ȹ���� �õ���." )
			<< BUILD_LOG( mit->first )
			<< END_LOG;

		return false;
	}

	if( CXSLDungeon::IsHenirDungeon( iDungeonID ) == true )
	{
		if( spEDRewardRoomUser->IsHenirRewardUser() == false )
		{
			START_LOG( cwarn, L"��ϸ� �ð� ���� ��ȸ�� ���� ������ �������� ȹ���� �õ���." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ED���� ó��

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
		kRewardED.m_iBonusED += static_cast<int>( iED * fPartyBonusRate * SiKResultProcess()->GetDungeonPartyDifficultyBonusRate( cDifficultyLevel ) );
	}

	if( IsComeBackUserInRoom() == true )
	{
		kRewardED.m_iBonusED += static_cast<int>( iED * 0.1f );
	}

	// �������� ED��������
	spEDRewardRoomUser->AddRewardED( kRewardED.SumED() );

	// GSUser�� ������Ʈ�� ED���� ����
	kInfo.m_mapGetED.insert( std::make_pair( spEDRewardRoomUser->GetCID(), kRewardED ) );

	return true;
}
//}}

#else		// SERV_REFORM_ITEM_DROP

//{{ 2009. 5. 12  ������	�ǽð� ED����
//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
//#ifdef SERV_NEW_HENIR_TEST
bool KRoomUserManager::AddDungeonRewardED(IN int iED, IN int iDungeonID, OUT KEGS_GET_ITEM_NOT& kInfo )
//#else
//bool KRoomUserManager::AddDungeonRewardED( IN int iED, OUT KEGS_GET_ITEM_NOT& kInfo )
//#endif SERV_NEW_HENIR_TEST
//}}
{
	kInfo.m_mapGetED.clear();

	if( iED <= 0 )
	{
		START_LOG( cwarn, L"ED�� 0�ΰ�쵵 �ֳ�?" )
			<< BUILD_LOG( iED )
			<< END_LOG;

		return false;
	}

	// ���� ����ִ� ���� ��
	int iUserNum = GetLiveMember();
	if( iUserNum < 0 )
	{
		START_LOG( clog, L"����ִ� ������ ����! �ð����� �߻�?" )
			<< BUILD_LOG( iED )
			<< END_LOG;

		return false;
	}

	// �ѻ������ �� ED ���ϱ�
	int iRewardED = ( iED / iUserNum );	

	//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
	// ��ϸ� ���� ȹ�� ������ �ִ� ������Ը� �ش�
	if( CXSLDungeon::IsHenirDungeon( iDungeonID ) == true )
	{
		int iRewardUserNum = GetHenirRewardUserNum();

		if( iRewardUserNum > 0 ) // ���� ȹ�� ������ ���� ���� ����
		{
			iRewardED = ( iED / iRewardUserNum );
		}
		else			// ���� ȹ�� ������ �ƹ��� ������ ���� ȹ�� ���� �ʴ´�.
		{
			iRewardED = 0;
			// ��� �� �ʿ䰡 ����.
			return true;
		}
	}
#endif SERV_NEW_HENIR_TEST
	//}}

	//ED ���.
	std::map< UidType, KRoomUserPtr >::iterator mit;

	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->IsDie() == true )
			continue;

		//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
		if( CXSLDungeon::IsHenirDungeon( iDungeonID ) == true )
		{
			if( mit->second->IsHenirRewardUser() == false )
				continue;
		}
#endif SERV_NEW_HENIR_TEST
		//}}

		//////////////////////////////////////////////////////////////////////////
		// ED���� ó��
		//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
		//#ifdef SERV_ROOMUSERINFO_REFAC
		const float fAddSocketObtEDRate = mit->second->GetBonusRate( KRoomUserInfo::BT_SOCKET_OPT_ED_RATE );
		//#endif SERV_ROOMUSERINFO_REFAC
		//}}

		KDungeonRewardED kRewardED;
		kRewardED.m_iED = iRewardED;
		kRewardED.m_iBonusED = static_cast<int>( iRewardED * fAddSocketObtEDRate );

		if( fAddSocketObtEDRate > 0.0f  &&  kRewardED.m_iBonusED == 0 )
		{
			kRewardED.m_iBonusED = 1; // ED���ʽ� �ּҰ� 1ED ����
		}

		//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
		if( IsComeBackUserInRoom() == true )
		{
			kRewardED.m_iBonusED += static_cast<int>( iRewardED * 0.1f );
		}
#endif SERV_COME_BACK_USER_REWARD
		//}} 

		// �������� ED��������
		mit->second->AddRewardED( kRewardED.SumED() );

		// GSUser�� ������Ʈ�� ED���� ����
		kInfo.m_mapGetED.insert( std::make_pair( mit->second->GetCID(), kRewardED ) );

		// ED���� ���� �˻�
		iED -= iRewardED;

		// ������ ���ٸ�?
		if( iED < iRewardED )
			break;		
	}

	return true;
}
//}}
#endif SERV_REFORM_ITEM_DROP
//}}

void KRoomUserManager::SetUserEquipItem( IN UidType nCID, IN std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
    KRoomUserPtr spRoomUser = GetUser( nCID );
    if( !spRoomUser )
    {
        START_LOG( cerr, L"������ ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return;
    }

    spRoomUser->SetUserEquipItem( vecInventorySlotInfo );

	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	// ������ ��� �ٲ� ���� ����Ʈ ���� üũ �մϴ�.
	spRoomUser->CheckItemEquipQuestCondition( false );
#endif SERV_QUEST_CLEAR_EXPAND
	//}}
}

void KRoomUserManager::MakeSlot( int iSlotID, USERLIST_TYPE eType /* = UT_GAME  */)
{
    if( iSlotID < 0 )
    {
        START_LOG( cerr, L"���� ID �̻�." )
            << BUILD_LOG( iSlotID )
            << END_LOG;

        return;
    }

	if( CheckUserlistType( eType ) == false )
	{
		START_LOG( cerr, L"USER LIST TYPE �̻�.!" )
			<< BUILD_LOG( (int)eType )
			<< END_LOG;
	}

    KRoomSlotPtr spRoomSlot( new KRoomSlot( iSlotID ) );
    spRoomSlot->SetFSM( ms_spSlotFSM );

    if( ( int )m_vecRoomSlot[eType].size() == iSlotID )
    {
        m_vecRoomSlot[eType].push_back( spRoomSlot );
    }
    else if( ( int )m_vecRoomSlot[eType].size() > iSlotID )
    {
        START_LOG( cwarn, L"�̹� �ִ� ������ �ٽ� ����." )
            << BUILD_LOG( iSlotID )
            << END_LOG;

        m_vecRoomSlot[eType][iSlotID].reset();
        m_vecRoomSlot[eType][iSlotID] = spRoomSlot;
    }
    else
    {
        START_LOG( cerr, L"���� ID �̻�." )
            << BUILD_LOG( iSlotID )
            << END_LOG;
    }
}

KRoomUserPtr KRoomUserManager::MakeUser( const KRoomUserInfo& kInfo )
{
    KRoomUserPtr spRoomUser( new KRoomUser );
    if( !spRoomUser )
    {
        return KRoomUserPtr();
    }

    spRoomUser->SetFSM( ms_spUserFSM );
    spRoomUser->SetRoomUserInfo( kInfo );
    return spRoomUser;
}

void KRoomUserManager::HandleInvalidSlotPointer( int iSlotID, USERLIST_TYPE eType )
{
    START_LOG( cerr, L"�����Ͱ� ��ȿ���� ����." )
        << BUILD_LOG( iSlotID )
        << END_LOG;

    MakeSlot( iSlotID, eType );
}

KRoomSlotPtr KRoomUserManager::FindEmptySlot( bool bConsiderTeam, USERLIST_TYPE eType )
{
	if( CheckUserlistType( eType ) == false )
	{
		START_LOG( cerr, L"��������Ʈ Ÿ�� �̻�.!" )
			<< BUILD_LOG( (int)eType )
			<< END_LOG;
		return KRoomSlotPtr();
	}

    if( bConsiderTeam == true )
    {
        std::map< int, int > mapTeamAndNumEmpty;
        std::map< int, KRoomSlotPtr > mapTeamAndSlotPtr;
        std::map< int, int >::iterator mit1;
        std::map< int, KRoomSlotPtr >::iterator mit2;
        for( int i = 0; i < m_nRoomSlot[eType]; i++ )
        {
            KRoomSlotPtr spRoomSlot = GetSlot( i, eType );
            if( !spRoomSlot )
            {
                START_LOG( cerr, L"�̻��� ������." )
                    << BUILD_LOG( i )
                    << END_LOG;

                continue;
            }
            if( spRoomSlot->IsOpened() && !spRoomSlot->IsOccupied() )
            {
                mit1 = mapTeamAndNumEmpty.find( spRoomSlot->GetTeam() );
                if( mit1 == mapTeamAndNumEmpty.end() )
                {
                    mapTeamAndNumEmpty.insert( std::make_pair( spRoomSlot->GetTeam(), 1 ) );
                }
                else
                {
                    ( mit1->second )++;
                }

                if( mapTeamAndSlotPtr.find( spRoomSlot->GetTeam() ) == mapTeamAndSlotPtr.end() )
                {
                    mapTeamAndSlotPtr.insert( std::make_pair( spRoomSlot->GetTeam(), spRoomSlot ) );
                }
            }
        }

        int iCandidateTeam;
        int iMaxEmptySlot = 0;
        for( mit1 = mapTeamAndNumEmpty.begin(); mit1 != mapTeamAndNumEmpty.end(); mit1++ )
        {
            if( mit1->second > iMaxEmptySlot )
            {
                iCandidateTeam = mit1->first;
                iMaxEmptySlot = mit1->second;
            }
        }

        if( iMaxEmptySlot > 0 )
        {
            mit2 = mapTeamAndSlotPtr.find( iCandidateTeam );
            if( mit2 == mapTeamAndSlotPtr.end() )
            {
                START_LOG( cerr, L"�߻��ϸ� �ȵǴ� ��Ȳ��." )
                    << BUILD_LOG( iCandidateTeam )
                    << BUILD_LOG( iMaxEmptySlot )
                    << END_LOG;

                return FindEmptySlot( false );
            }

            return mit2->second;
        }

        return KRoomSlotPtr();
    }
    else
    {
        for( int i = 0; i < m_nRoomSlot[eType]; i++ )
        {
            KRoomSlotPtr spRoomSlot = GetSlot( i, eType );
            if( !spRoomSlot )
            {
                START_LOG( cerr, L"�̻��� ������." )
                    << BUILD_LOG( i )
                    << END_LOG;

                continue;
            }
            if( spRoomSlot->IsOpened() && !spRoomSlot->IsOccupied() )
            {
                return spRoomSlot;
            }
        }

        return KRoomSlotPtr();
    }
}

//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
UidType KRoomUserManager::AppointNewHost()
{
	USERLIST_TYPE eType = UT_GAME;

	KRoomUserPtr spRoomUserCandidate = KRoomUserPtr();

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( IS_NULL( mit->second ) )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// �ϴ� ���� ȣ��Ʈ�� �ƴѻ�� �߿�..
		if( mit->second->IsHost() == true )
			continue;

#ifdef  SERV_PVP_NEW_SYSTEM
		if( mit->second->IsPvpNpc() == true )
			continue;
#endif  SERV_PVP_NEW_SYSTEM

		// ���� �÷��� ���� ���
		if( mit->second->IsPlaying() == true )
		{
			// �� ������ ���� ������ �ȵǾ��ִٸ� ����!
			if( IS_NULL( spRoomUserCandidate ) )
			{
				spRoomUserCandidate = mit->second;
				continue;
			}

            {

			    // �� ������ ������ ���ؼ� �� ������ ���� ����!
			    if( 

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
                    spRoomUserCandidate->IsPlaying() == false ||
                    ( spRoomUserCandidate->ZombieAlert_Get() == true && mit->second->ZombieAlert_Get() == false ) ||
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST                   
                    spRoomUserCandidate->GetPingScore() > mit->second->GetPingScore() )
			    {
				    spRoomUserCandidate = mit->second;
			    }
            }
		}
		// �÷��� ���ϰ� �ִ� ���
		else
		{
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
			if( IS_NULL( spRoomUserCandidate ) )
			{
				spRoomUserCandidate = mit->second;
			}
#else   SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
			spRoomUserCandidate = mit->second;
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
		}
	}

	if( IS_NOT_NULL( spRoomUserCandidate ) )
	{
		spRoomUserCandidate->SetHost( true );
		return spRoomUserCandidate->GetCID();
	}
	else
	{
		// �ϴ� �����α� ���� ���� cwarn���� ��´�.
		START_LOG( cwarn, L"ȣ��Ʈ ���� ����." )
			<< BUILD_LOG( GetNumMember() )
			<< END_LOG;

		return 0;
	}
}

#else

UidType KRoomUserManager::AppointNewHost()
{
	USERLIST_TYPE eType = UT_GAME;

	KRoomUserPtr spRoomUserCandidate = KRoomUserPtr();

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

#ifdef  SERV_PVP_NEW_SYSTEM
		if( mit->second->IsPvpNpc() == true )
			continue;
#endif  SERV_PVP_NEW_SYSTEM

		if( !mit->second->IsHost() )
		{
			if( mit->second->IsPlaying() )
			{
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
			    if( IS_NULL( spRoomUserCandidate ) )
			    {
				    spRoomUserCandidate = mit->second;
                    continue;
			    }
                if ( spRoomUserCandidate->IsPlaying() == false ||
                    ( spRoomUserCandidate->ZombieAlert_Get() == true && mit->second->ZombieAlert_Get() == false ) )
                {
				    spRoomUserCandidate = mit->second;
                    continue;
                }
#else   SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
				mit->second->SetHost( true );
				return mit->first;
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
			}
			else
			{
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
			    if( IS_NULL( spRoomUserCandidate ) )
			    {
				    spRoomUserCandidate = mit->second;
                    continue;
			    }
#else   SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
				spRoomUserCandidate = mit->second;
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
			}
		}
	}

	if( spRoomUserCandidate )
	{
		spRoomUserCandidate->SetHost( true );
		return spRoomUserCandidate->GetCID();
	}
	else
	{
		START_LOG( cerr, L"ȣ��Ʈ ���� ����." )
			<< BUILD_LOG( GetNumMember() )
			<< END_LOG;

		return 0;
	}
}
#endif SERV_CHOOSE_FASTEST_HOST
//}

float KRoomUserManager::CheckDungeonBalRate( int iUnitLevel, int nDungeonID )
{
	int nConst =  abs( SiCXSLDungeonManager()->GetDungeonMinLevel( nDungeonID ) - iUnitLevel );
	if( nConst <= 3 )
	{
		return 1.0f;
	}
	else if( nConst == 4 )
	{
		return 0.8f;
	}
	else if( nConst >= 5 && nConst <= 6 )
	{
		return 0.6f;
	}
	else if( nConst >= 7 && nConst <= 9 )
	{
		return 0.5f;
	}
	else if( nConst >= 10 && nConst <= 12 )
	{
		return 0.4f;
	}
	else if( nConst >= 13 && nConst <= 15 )
	{
		return 0.2f;
	}
	else if( nConst >= 16 && nConst <= 19 )
	{
		return 0.1f;
	}

	return 0.0f;
}

//{{ 2007. 10. 29  ������  Kill + MDKill ��� �Լ�
int KRoomUserManager::GetKillNumber( UidType nCID )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"ų���� �������µ� ������ ����" )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return 0;
	}

	return spRoomUser->GetNumKill();
}
//}}

//{{ 2012. 12. 5	������		������ ���� ���� ���
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
void KRoomUserManager::SetIsIntrude( IN const UidType nCID, IN const bool bIsIntrude, IN const bool bObserver /*= false*/ )
{
	const USERLIST_TYPE eType = ( bObserver ? UT_OBSERVER : UT_GAME );

	KRoomUserPtr spRoomUser = GetUser( nCID, eType );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"���� ���¸� �����Ϸ��µ� ������ ����" )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return;
	}

	return spRoomUser->SetIsIntrude( bIsIntrude );
}
#else
void KRoomUserManager::SetIsIntrude( UidType nCID, bool bIsIntrude )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"���� ���¸� �����Ϸ��µ� ������ ����" )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return;
	}

	return spRoomUser->SetIsIntrude( bIsIntrude );
}
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
//}}

bool KRoomUserManager::GetIsIntrude( UidType nCID )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"���� ���¸� �������µ� ������ ����" )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	return spRoomUser->GetIsIntrude();
}

//{{ 2007. 12. 18  ������  ��Ȱ�� ��� ����[for statistics]
int KRoomUserManager::GetUsedRessurectionStoneCount( UidType nCID )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"��Ȱ�� ��뷮�� �������µ� ������ ����" )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	return spRoomUser->GetUsedRessurectionStoneCount();
}

int KRoomUserManager::GetUsedRessurectionStonePlayerCount()
{
	USERLIST_TYPE eType = UT_GAME;

	int iPlayerCount = 0;

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

		if( mit->second->GetUsedRessurectionStoneCount() > 0 )
			++iPlayerCount;
	}

	return iPlayerCount;
}
//}}


bool KRoomUserManager::IsGameBang( UidType nCID )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"GAME BANG ������ ã�µ� ������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	return spRoomUser->IsGameBang();
}

bool KRoomUserManager::IsRingofpvprebirth( UidType nCID )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"������������ ������ ������ ã�µ� ������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	return spRoomUser->IsRingofpvprebirth();
}

void KRoomUserManager::ZU_CheckStart()
{
	USERLIST_TYPE eType = UT_GAME;

	START_LOG( clog, L"�������� üũ����.!" );

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

			mit->second->ZU_Refresh();
		}
	}

	//�������� üũ�� ����
	m_bIsCheckZU = true;
}

void KRoomUserManager::ZU_CheckEnd()
{
	START_LOG( clog, L"�������� üũ����.!" );

	//�������� üũ ����.
	m_bIsCheckZU = false;
}







//{{ 2012. 09. 08	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
bool KRoomUserManager::ZU_Tick( IN const CXSLRoom::ROOM_TYPE eRoomType, OUT std::vector< UidType >& vecZombieUnit )
{
	USERLIST_TYPE eType = UT_GAME;

	if( ZU_IsCheckZU() == false )
		return false;

	
	// ��Ʋ�ʵ�� ������ ��� ���� üũ�� ���� �ȵȴ�.
	if( eRoomType != CXSLRoom::RT_BATTLE_FIELD )
	{
		//������ ������ üũ�� ���ش�.
		if( m_mapRoomUser[eType].empty() == true )
		{
			m_bIsCheckZU = false;
			return false;
		}
	}

	vecZombieUnit.clear();

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

#ifdef SERV_FIX_ZOMBIE_OBSERVER
			if( mit->second->IsObserver() == true )
				continue;
#endif SERV_FIX_ZOMBIE_OBSERVER

			if( mit->second->ZU_Tick( eRoomType ) == true )
			{
				vecZombieUnit.push_back( mit->second->GetCID() );
			}
		}
	}

	//���������� ���ٸ�..
	if( vecZombieUnit.empty() == true )
		return false;

	return true;
}
#else
bool KRoomUserManager::ZU_Tick( OUT std::vector< UidType >& vecZombieUnit )
{
	USERLIST_TYPE eType = UT_GAME;

	if( ZU_IsCheckZU() == false )
	{
		return false;
	}
	else
	{
		//������ ������ üũ�� ���ش�.
		if( m_mapRoomUser[eType].empty() == true )
		{
			m_bIsCheckZU = false;
			return false;
		}
	}

	vecZombieUnit.clear();

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

#ifdef SERV_FIX_ZOMBIE_OBSERVER
			if( mit->second->IsObserver() == true )
				continue;
#endif SERV_FIX_ZOMBIE_OBSERVER

			if( mit->second->ZU_Tick() == true )
			{
				vecZombieUnit.push_back( mit->second->GetCID() );
			}
		}
	}

	//���������� ���ٸ�..
	if( vecZombieUnit.empty() == true )
		return false;

	return true;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

void KRoomUserManager::ZU_Refresh( UidType nCID, u_short usEventID )
{
	if( ZU_IsCheckZU() == false )
		return;

	//������ �濡 ���µ� ���� ��Ŷ�� ��� �̰��� �߰��Ѵ�.
	switch( usEventID )
	{
	case ERM_OPEN_PVP_ROOM_REQ:
	case ERM_OPEN_DUNGEON_ROOM_REQ:
		//{{ 2012. 09. 08	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	case ERM_OPEN_BATTLE_FIELD_REQ:
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	case ERM_OPEN_WEDDING_HALL_REQ:
#endif SERV_RELATIONSHIP_SYSTEM
		//}
	case ERM_JOIN_ROOM_REQ:
	case ERM_CHECK_INVALID_USER_NOT:
		return;
	}

	//�̺�Ʈ�� ���ƿ� ������ ã�´�.
	for( int i = UT_GAME; i < UT_TOTAL_NUM; ++i )
	{
		USERLIST_TYPE eType = (USERLIST_TYPE)i;

		std::map< UidType, KRoomUserPtr >::iterator mit;
		mit = m_mapRoomUser[eType].find( nCID );

		if( mit != m_mapRoomUser[eType].end() )
		{
			if( !mit->second )
			{
				START_LOG( cerr, L"�� ���� ������ �̻�." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;

				return;
			}

			START_LOG( clog, L"�������� check refresh.!" )
				<< BUILD_LOG( nCID );

			//�̻������ üũ �ð��� �����Ѵ�.
			mit->second->ZU_Refresh();
			return;
		}
		else
		{
			//������ ������ ��� �������� ����Ʈ���� ���⶧��..
			if( eType != UT_GAME )
			{
				START_LOG( cwarn, L"�ð��� ������ ������ ����.!" )
					<< BUILD_LOG( nCID )
					<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
					<< END_LOG;
			}
		}
	}
}


bool KRoomUserManager::RequestTradeTo( UidType nCID, UidType nCIDTo )
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

    if( spRoomUser->GetStateID() != KRoomUserFSM::S_INIT )
    {
        START_LOG( cwarn, L"��û�ϴ� ������ ���°� �̻���." )
            << BUILD_LOG( nCID )
            << BUILD_LOG( spRoomUser->GetStateID() )
            << BUILD_LOG( spRoomUser->GetStateIDString() )
            << END_LOG;

        return false;
    }

    if( spRoomUserTo->GetStateID() != KRoomUserFSM::S_INIT )
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

bool KRoomUserManager::TradeAcceptedBy( UidType nCID, UidType nCIDAcceptor )
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

    if( spRoomUserAcceptor->GetStateID() != KRoomUserFSM::S_INIT )
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

bool KRoomUserManager::TradeRejectedBy( UidType nCID, UidType nCIDRejector )
{
    KRoomUserPtr spRoomUser = GetUser( nCID );
    if( !spRoomUser )
    {
        START_LOG( cwarn, L"������ ����." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    return spRoomUser->TradeRejectedBy( nCIDRejector );
}

void KRoomUserManager::ExpireTradeRequest( std::vector< UidType >& vecTradeExpireUnit )
{
	USERLIST_TYPE eType = UT_GAME;

    std::map< UidType, KRoomUserPtr >::iterator mit;
    for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"�� ���� ������ �̻�." )
                << BUILD_LOG( mit->first )
                << END_LOG;

            continue;
        }

        if( mit->second->ExpireTradeRequest() )
        {
            vecTradeExpireUnit.push_back( mit->second->GetCID() );
        }
    }
}

//{{ 2012. 11. 9	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
bool KRoomUserManager::GetQuestDropItem( IN const UidType iUnitUID, IN const int iDungeonID, IN const char cDifficulty, IN const char cDungeonMode, IN const int iNPCID, OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo, IN float fQuestItemDropRate )
#else
bool KRoomUserManager::GetQuestDropItem( IN const UidType iUnitUID, IN const int iDungeonID, IN const char cDifficulty, IN const char cDungeonMode, IN const int iNPCID, OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo )
#endif SERV_QUEST_ITEM_DROP_EVENT
//}
{
	USERLIST_TYPE eType = UT_GAME;

	SET_ERROR( NET_OK );

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	const bool bIsComeBackUserInRoom = IsComeBackUserInRoom();
#endif SERV_COME_BACK_USER_REWARD
	//}}

	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( IS_NULL( spRoomUser ) )
	{
		START_LOG( cerr, L"������ �������� �ʴ´�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	if( spRoomUser->IsDie() == true )
		return false;

	//����Ʈ ������ ����� ������..
	KRoomUser::QUEST_ITEM_DROP_INFO sDropInfo;

#ifdef SERV_THANKS_GIVING_DAY_DROP_FOR_EQUIPPED_USER
	const std::map<UidType, bool> mapInfo = GetSuitableLevelInfo();
	bool bSuitableLevel = false;
	std::map<UidType, bool>::const_iterator mit = mapInfo.find(iUnitUID);
    if(mit != mapInfo.end())
		bSuitableLevel = mit->second;

#endif //SERV_THANKS_GIVING_DAY_DROP_FOR_EQUIPPED_USER

	//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
	#ifdef SERV_THANKS_GIVING_DAY_DROP_FOR_EQUIPPED_USER
		if( spRoomUser->GetQuestDropItem( iDungeonID, cDifficulty, cDungeonMode, iNPCID, sDropInfo, bIsComeBackUserInRoom, fQuestItemDropRate, bSuitableLevel ) == true )
	#else //SERV_THANKS_GIVING_DAY_DROP_FOR_EQUIPPED_USER
		if( spRoomUser->GetQuestDropItem( iDungeonID, cDifficulty, cDungeonMode, iNPCID, sDropInfo, bIsComeBackUserInRoom, fQuestItemDropRate ) == true )
	#endif //SERV_THANKS_GIVING_DAY_DROP_FOR_EQUIPPED_USER
#else
	//{{ 2011. 08. 31  ��μ�	��д��� ����
#ifdef SERV_RANDOM_DAY_QUEST
	if( spRoomUser->GetQuestDropItem( iDungeonID, cDifficulty, cDungeonMode, iNPCID, sDropInfo, bIsComeBackUserInRoom ) == true )
#else
	if( spRoomUser->GetQuestDropItem( iDungeonID, cDifficulty, iNPCID, sDropInfo, bIsComeBackUserInRoom ) == true )
#endif SERV_RANDOM_DAY_QUEST
	//}}
#endif SERV_QUEST_ITEM_DROP_EVENT
	//}
	{
		vecDropInfo.push_back( sDropInfo );
	}

	if( vecDropInfo.empty() )
		return false;

	return true;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2011. 08. 31  ��μ�	��д��� ����
//#ifdef SERV_RANDOM_DAY_QUEST
bool KRoomUserManager::GetQuestDropItem( IN const int iDungeonID, 
										IN const char cDifficulty, 
										IN const char cDungeonMode, 
										IN const int iNPCID, 
										OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo 
										//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
										, IN float fQuestItemDropRate
#endif SERV_QUEST_ITEM_DROP_EVENT
										//}
										)
//#endif SERV_RANDOM_DAY_QUEST
//}}
{
	USERLIST_TYPE eType = UT_GAME;

	SET_ERROR( NET_OK );

	vecDropInfo.clear();

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	const bool bIsComeBackUserInRoom = IsComeBackUserInRoom();
#endif SERV_COME_BACK_USER_REWARD
	//}} 

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

		if( mit->second->IsDie() == false )
		{
			//����Ʈ ������ ����� ������..
			KRoomUser::QUEST_ITEM_DROP_INFO sDropInfo;

			//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
			if( mit->second->GetQuestDropItem( iDungeonID, cDifficulty, cDungeonMode, iNPCID, sDropInfo, bIsComeBackUserInRoom, fQuestItemDropRate ) == true )
#else
			//{{ 2011. 08. 31  ��μ�	��д��� ����
#ifdef SERV_RANDOM_DAY_QUEST
			if( mit->second->GetQuestDropItem( iDungeonID, cDifficulty, cDungeonMode, iNPCID, sDropInfo, bIsComeBackUserInRoom ) == true )
#else
			if( mit->second->GetQuestDropItem( iDungeonID, cDifficulty, iNPCID, sDropInfo, bIsComeBackUserInRoom ) == true )
#endif SERV_RANDOM_DAY_QUEST
				//}}
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

bool KRoomUserManager::SetUnitQuestInvenInfo( UidType nCID, std::map<int, KSubQuestInfo>& mapInvenInfo )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cwarn, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	return spRoomUser->SetUnitQuestInvenInfo( mapInvenInfo );
}

bool KRoomUserManager::SetTutorialUIDList(UidType nCID, const std::vector<UidType>& vecTutorUIDList )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cwarn, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spRoomUser->SetTutorialUIDList( vecTutorUIDList );

	return true;
}

//{{ 2009. 6. 10  ������	���� ������ ��� EXP,ED
bool KRoomUserManager::UpdateetOldExpED( UidType nCID, int iOldEXP, int iOldED )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cwarn, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spRoomUser->UpdateetOldExpED( iOldEXP, iOldED );
	return true;
}
//}}

//{{ 2012. 07. 03	������		���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
bool KRoomUserManager::SetGameChannelIP( IN const UidType iUnitUID, IN const std::wstring& wstrGameChannelIP )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( IS_NULL( spRoomUser ) )
	{
		START_LOG( cwarn, L"������ ����." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spRoomUser->SetGameChannelIP( wstrGameChannelIP );
	return true;
}
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
//}}
//{{ 2012. 06. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
bool KRoomUserManager::SetActivatedAutoPartyBonus( IN const UidType iUnitUID, IN const bool bActivatedAutoPartyBonus )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( IS_NULL( spRoomUser ) )
	{
		START_LOG( cwarn, L"������ ����." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spRoomUser->SetActivatedAutoPartyBonus( bActivatedAutoPartyBonus );
	return true;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

bool KRoomUserManager::CheckUserlistType( USERLIST_TYPE eType ) const
{
	if( UT_NONE < eType && UT_TOTAL_NUM > eType )
	{
		return true;
	}

	return false;
}

//{{ 2008. 6. 17  ������  �ǽð� ��Ȱ��
void KRoomUserManager::SetEndPlay( UidType nCID, bool bEndPlay )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cwarn, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		// �ð����� ������ ���� �� �ִ�.
		return;
	}

	spRoomUser->SetEndPlay( bEndPlay );
}

void KRoomUserManager::SetCashContinueReady( UidType nCID, bool bCashContinueReady )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cwarn, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		// �ð����� ������ ���� �� �ִ�.
		return;
	}

	spRoomUser->SetCashContinueReady( bCashContinueReady );
}

void KRoomUserManager::ReserveEndPlay( UidType nCID )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cwarn, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		// �ð����� ������ ���� �� �ִ�.
		return;
	}

	spRoomUser->ReserveEndPlay();
}

bool KRoomUserManager::CheckEndPlay()
{
	bool bEndPlay = true;

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

		if( !mit->second->CheckEndPlay() )
		{
			bEndPlay = false;
		}
	}

	return bEndPlay;
}

bool KRoomUserManager::CheckEndPlayAnotherMember( UidType nCID )
{
	bool bEndPlay = true;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->GetCID() == nCID )
			continue;

		if( !mit->second->CheckEndPlay() )
		{
			bEndPlay = false;
		}
	}

	return bEndPlay;
}

void KRoomUserManager::StopDungeonContinueTime( UidType nCID, bool bIsStop )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cwarn, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		// �ð����� ������ ���� �� �ִ�.
		return;
	}

	spRoomUser->StopDungeonContinueTime( bIsStop );
}
//}}

//{{ 2008. 7. 24  ������	�����̵�
bool KRoomUserManager::IsRankFail( IN int iDungeonID )
{
	int iRankMaxLevel = SiCXSLDungeonManager()->GetDungeonMaxLevel( iDungeonID );

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// ��ũ ��� ���� ���� üũ
		if( iRankMaxLevel < mit->second->GetLevel() )
			return true;
	}

	return false;
}
//}}

int KRoomUserManager::GetHighestUserLevel() const
{
	USERLIST_TYPE eType = UT_GAME;
	if( CheckUserlistType( eType ) == false )
		return 0;

	int iHighestLevel = 0;

	std::map< UidType, KRoomUserPtr >::const_iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		KRoomUserPtr kRoomUserPtr = mit->second;
		if( iHighestLevel < kRoomUserPtr->GetLevel() )
		{
			iHighestLevel = kRoomUserPtr->GetLevel();
		}
	}

	return iHighestLevel;
}

//{{ 2012. 05. 10	������	���� ���� ���� ����
#ifdef SERV_DUNGEON_NPC_LEVEL
int	KRoomUserManager::GetAverageUserLevel( IN const int iIncludeLevelGap /*= 0*/ ) const
{
	USERLIST_TYPE eType = UT_GAME;
	if( CheckUserlistType( eType ) == false )
		return 0;

	int iTotalLevel = 0;
	int iUserCount = 0;

	int iHighestUserLevel = 0;

	// ���� ���� üũ���� ������ �ִ� ������ ���� ������
	if( iIncludeLevelGap != 0 )
	{
		iHighestUserLevel = GetHighestUserLevel();
	}

	std::map< UidType, KRoomUserPtr >::const_iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
	{
		KRoomUserPtr kRoomUserPtr = mit->second;
		if( IS_NULL( kRoomUserPtr ) )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( iIncludeLevelGap != 0 )
		{
			if( ( iHighestUserLevel - kRoomUserPtr->GetLevel() ) > iIncludeLevelGap )
				continue;
		}
		
		iTotalLevel += kRoomUserPtr->GetLevel();
		++iUserCount;
	}

	if( iUserCount == 0 )
		return 0;

	return ( iTotalLevel / iUserCount );
}
#endif SERV_DUNGEON_NPC_LEVEL
//}}

//{{ 2009. 7. 21  ������	��� ��������
void KRoomUserManager::ResetAgreeEnterSecretStage()
{
	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// ���� ���� �ʱ�ȭ
		mit->second->SetAgreeEnterSecretStage( CXSLRoom::ESS_NONE );
	}
}

bool KRoomUserManager::AgreeEnterSecretStage( UidType nCID, CXSLRoom::ENTER_SECRET_STAGE eAgreeValue )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		
		return false;
	}

	spRoomUser->SetAgreeEnterSecretStage( eAgreeValue );
	return true;
}

bool KRoomUserManager::IsAllPlayerAgreed()
{
	const int iNumMember = GetNumMember();
	if( iNumMember <= 0 )
		return false;

	int iAgreeUserCount = 0;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// �ǻ� ǥ���� �� ���� ��
		if( mit->second->GetAgreeEnterSecretStage() != CXSLRoom::ESS_NONE )
		{
			++iAgreeUserCount;
		}
	}

	return ( iNumMember <= iAgreeUserCount );
}

bool KRoomUserManager::IsEnterSecretStage()
{
	int iAgreeUserCount = 0;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// ������ ���� ��
		if( mit->second->GetAgreeEnterSecretStage() == CXSLRoom::ESS_AGREE )
		{
            ++iAgreeUserCount;
		}
	}

	// ������ ���� ���� ��Ƽ����� �����̻��̸� ��� �������� ����!
	return ( GetNumMember() <= ( iAgreeUserCount * 2 ) );
}
//}}

//{{ 2010. 07. 09  ������	��ӷ� �̺�Ʈ Ȯ��
#ifdef SERV_PC_BANG_DROP_EVENT

bool KRoomUserManager::IsExistPcBangPlayer()
{
	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// PC�� �������� üũ!
		if( mit->second->IsGameBang() == true )
		{
			return true;
		}
	}

	// PC�� ������ ���ٸ�?!
	return false;
}

#endif SERV_PC_BANG_DROP_EVENT
//}}

//{{ 2010. 8. 3	������	�� �ý���
#ifdef SERV_PET_SYSTEM

bool KRoomUserManager::HavePet( IN UidType nCID )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}
	
	return spRoomUser->HavePet();
}

void KRoomUserManager::GetTeamMemberList( IN CXSLRoom::TEAM_NUM eTeamNum, IN bool bIsPlayingMember, OUT std::vector< UidType >& vecUnitUIDList )
{
	vecUnitUIDList.clear();

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( eTeamNum != mit->second->GetTeam() )
			continue;

		if( bIsPlayingMember )
		{
			if( mit->second->IsPlaying() == false )
				continue;
		}

		vecUnitUIDList.push_back( mit->second->GetCID() );
	}
}

bool KRoomUserManager::UpdatePetInfo( IN UidType nCID, IN const std::vector< KPetInfo >& vecPetInfo )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spRoomUser->UpdatePetInfo( vecPetInfo );
	return true;
}

bool KRoomUserManager::UpdatePetInfo( IN UidType nCID, IN const KPetInfo& kPetInfo )
{
	std::vector< KPetInfo > vecPetInfo;
	vecPetInfo.push_back( kPetInfo );
    return UpdatePetInfo( nCID, vecPetInfo );
}

bool KRoomUserManager::UpdatePetSatiety( IN UidType nCID, IN UidType iPetUID, IN short sSatiety )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	return spRoomUser->UpdatePetSatiety( iPetUID, sSatiety );
}

bool KRoomUserManager::UpdatePetIntimacy( IN UidType nCID, IN UidType iPetUID, IN int iIntimacy )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	return spRoomUser->UpdatePetIntimacy( iPetUID, iIntimacy );
}

#endif SERV_PET_SYSTEM
//}}

//{{ 2012. 03. 06	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
bool KRoomUserManager::UpdateGameStat( IN const UidType iUnitUID, IN const KStat& kGameStat )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spRoomUser->UpdateGameStat( kGameStat );
	return true;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND

void KRoomUserManager::CheckItemEquipQuestCondition( IN bool bIsGameStart )
{
	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		KRoomUserPtr spRoomUser = mit->second;
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		spRoomUser->CheckItemEquipQuestCondition( bIsGameStart );
	}
}

#endif SERV_QUEST_CLEAR_EXPAND
//}}

//{{ 2010. 8. 25	������	���� ������ ���
KRoomUserPtr KRoomUserManager::GetHostUser()
{
	UidType iHostUID = 0;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->IsHost() == false )
			continue;

		// ������ UnitUID���
		iHostUID = mit->second->GetCID();
		break;
	}

	return GetUser( iHostUID );
}
//}}

//{{ 2010. 12. 7	������	���� ���� ��� ���
#ifdef SERV_APPROPRIATE_LEVEL_STAT
void KRoomUserManager::GetAppropriateLevelStatistics( OUT KELOG_APPROPRIATE_LEVEL_STATISTICS_NOT& kNot )
{
	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		KRoomUserPtr spRoomUser = mit->second;
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// ��ڴ� ������� ������ �ʴ´�.
		if( spRoomUser->GetAuthLevel() >= SEnum::UAL_GM )
			continue;

		KUserAppropriateLevelInfo kInfo;
		spRoomUser->GetAppropriateLevelInfo( kInfo );
		kNot.m_vecUserAppropriateLevel.push_back( kInfo );
	}    
}
#endif SERV_APPROPRIATE_LEVEL_STAT
//}}

//{{ 2011. 03. 08	��μ� ����ġ�� ȹ���� ������ ����Ʈ �Ϸ� ���� ����
#ifdef SERV_EXP_ACQUISITION_CONDITION_CHANGE
void KRoomUserManager::AddHaveExpInDungeon( UidType UnitUid, bool bState )
{
	if( UnitUid <= 0 )
	{
		START_LOG( cerr, L"���������� UnitUID���Դϴ�!" )
			<< BUILD_LOG( UnitUid )
			<< END_LOG;
		return;
	}

	m_mapHaveExpInDungeon.insert( std::make_pair( UnitUid, bState ) );
}
#endif SERV_EXP_ACQUISITION_CONDITION_CHANGE
//}}

//{{ 2011. 03. 17	��μ�	���� �÷��� �α� ��û���� 
#ifdef SERV_DUNGEON_TEMP_LOG_DB
int KRoomUserManager::GetTotalPatyDamage_LUA()
{
	int iTotal = 0;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		KRoomUserPtr spRoomUser = mit->second;
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}
		else
		{
			iTotal += spRoomUser->GetTotalGivenDamamge_LUA();
		}
	}
	
	if( iTotal < 0)
	{
		START_LOG( cerr, L"��Ƽ���� �� ������ ���� 0���� ����" )
			<< BUILD_LOG( iTotal )
			<< END_LOG;

		return 0;
	}

	return iTotal;
}
#endif SERV_DUNGEON_TEMP_LOG_DB
//}}

//{{ 2011. 03. 26  ��μ�	û 1�� �߰� ��ų �߰� - ����
#ifdef SERV_ADD_BUFF_SKILL_INFO
bool KRoomUserManager::ProcessBuffSkill( IN KEGS_BUFF_SKILL_INFO_CONVEY_REQ& kPacket, OUT KEGS_BUFF_SKILL_INFO_CONVEY_NOT& KNot )
{
	bool ret = false;

	if( kPacket.m_bIsBuffCasting == true )   // ���� ����
	{
		BOOST_TEST_FOREACH( const UidType, iCaughtUnitUID, kPacket.m_vecCaughtUIDList )
		{
			std::map< UidType, KRoomUserPtr >::iterator mit;
			mit = m_mapRoomUser[UT_GAME].find( iCaughtUnitUID );
			if( mit == m_mapRoomUser[UT_GAME].end() )
			{
				START_LOG( clog, L"ĳ���� ������ ����!" )
					<< BUILD_LOG( iCaughtUnitUID )
					<< END_LOG;
				continue;
			}

			KRoomUserPtr spRoomUser = mit->second;
			if( spRoomUser == NULL )
			{
				START_LOG( cerr, L"�� ���� ������ �̻�." )
					<< BUILD_LOG( iCaughtUnitUID )
					<< END_LOG;
				continue;
			}
			
			if( spRoomUser->AddCaughtBuffSkill( kPacket.m_iBuffEnum, kPacket.m_fBuffTime, kPacket.m_bIsBuffCasting, kPacket.m_iCastingUnitUID, kPacket.m_cBuffLevel ) == false )
			{
				continue;
			}

			KBuffSkillInfo tempBuff;
			tempBuff.m_iCaughtUnitUID	= spRoomUser->GetCID();
			tempBuff.m_bIsBuffCasting	= kPacket.m_bIsBuffCasting;
			tempBuff.m_iBuffEnum		= kPacket.m_iBuffEnum;
			tempBuff.m_iCastingUnitUID	= kPacket.m_iCastingUnitUID;
			tempBuff.m_cBuffLevel		= kPacket.m_cBuffLevel;
		
			std::map< int, std::vector< KBuffSkillInfo > >::iterator mitBuffInfo;
			mitBuffInfo = KNot.m_mapCaughtUIDList.find( kPacket.m_iBuffEnum );
			if( mitBuffInfo == KNot.m_mapCaughtUIDList.end() ) // ���� �߰�
			{
				std::vector< KBuffSkillInfo > vecNewBuffInfo;
				vecNewBuffInfo.push_back( tempBuff );
				KNot.m_mapCaughtUIDList.insert( std::make_pair( kPacket.m_iBuffEnum, vecNewBuffInfo ) );
			}
			else // �̾ �߰�
			{
				mitBuffInfo->second.push_back( tempBuff );
			}

			ret = true;
		}
	}
	else									// ĵ�� ����
	{
		BOOST_TEST_FOREACH( const UidType, iCaughtUnitUID, kPacket.m_vecCaughtUIDList )
		{
			std::map< UidType, KRoomUserPtr >::iterator mit;
			mit = m_mapRoomUser[UT_GAME].find( iCaughtUnitUID );
			if( mit == m_mapRoomUser[UT_GAME].end() )
			{
				START_LOG( clog, L"ĳ���� ������ ����!" )
					<< BUILD_LOG( iCaughtUnitUID )
					<< END_LOG;
				continue;
			}

			KRoomUserPtr spRoomUser = mit->second;
			if( spRoomUser == NULL )
			{
				START_LOG( cerr, L"�� ���� ������ �̻�." )
					<< BUILD_LOG( iCaughtUnitUID )
					<< END_LOG;
				continue;
			}
			
			if( spRoomUser->DeleteCaughtBuffSkill( kPacket.m_iBuffEnum ) == false )
			{
				START_LOG( clog, L"�� ������ �ɷ� ���� �ʴ� ������ �����Ϸ� �Ѵ�." )
					<< BUILD_LOG( spRoomUser->GetCID() )
					<< BUILD_LOG( kPacket.m_iBuffEnum )
					<< END_LOG;
				continue;
			}

			KBuffSkillInfo tempBuff;
			tempBuff.m_iCaughtUnitUID	= spRoomUser->GetCID();
			tempBuff.m_bIsBuffCasting	= false;
			tempBuff.m_iBuffEnum		= kPacket.m_iBuffEnum;
			tempBuff.m_iCastingUnitUID	= kPacket.m_iCastingUnitUID;
			tempBuff.m_cBuffLevel		= kPacket.m_cBuffLevel;

			std::map< int, std::vector< KBuffSkillInfo > >::iterator mitBuffInfo;
			mitBuffInfo = KNot.m_mapCaughtUIDList.find( kPacket.m_iBuffEnum );
			if( mitBuffInfo == KNot.m_mapCaughtUIDList.end() ) // ���� �߰�
			{
				std::vector< KBuffSkillInfo > vecNewBuffInfo;
				vecNewBuffInfo.push_back( tempBuff );
				KNot.m_mapCaughtUIDList.insert( std::make_pair( kPacket.m_iBuffEnum, vecNewBuffInfo ) );
			}
			else // �̾ �߰�
			{
				mitBuffInfo->second.push_back( tempBuff );
			}

			ret = true;
		}
	}

	return ret;
}

bool KRoomUserManager::CheckCaughtBuffSkill( OUT KEGS_BUFF_SKILL_INFO_CONVEY_NOT& KNot )
{
	bool ret = false;
	// ��Ŷ�� ���� ����
	std::vector< KBuffSkillInfo > vecBuffInfo;
	std::map< int, std::vector< KBuffSkillInfo > >::iterator mitBuffInfo;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin() ; mit != m_mapRoomUser[UT_GAME].end() ; ++mit )
	{
		KRoomUserPtr spRoomUser = mit->second;
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( spRoomUser->GetFinishBuffSkill( vecBuffInfo ) == false )
		{
			continue;
		}
		
		BOOST_TEST_FOREACH( const KBuffSkillInfo&, kBuffSkillInfo, vecBuffInfo )
		{
			mitBuffInfo = KNot.m_mapCaughtUIDList.find( kBuffSkillInfo.m_iBuffEnum );
			if( mitBuffInfo == KNot.m_mapCaughtUIDList.end() ) // ���� �߰�
			{
				std::vector< KBuffSkillInfo > vecNewBuffInfo;
				vecNewBuffInfo.push_back( kBuffSkillInfo );
				KNot.m_mapCaughtUIDList.insert( std::make_pair( kBuffSkillInfo.m_iBuffEnum, vecNewBuffInfo ) );
				ret = true;
			}
			else // �̾ �߰�
			{
				mitBuffInfo->second.push_back( kBuffSkillInfo );
				ret = true;
			}
		}		
	}
	return ret;
}
#endif SERV_ADD_BUFF_SKILL_INFO
//}}

//{{ 2011. 05. 04  ��μ�	���� Ŭ����� ������ ���� ���� �߰�
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
bool KRoomUserManager::IsRoomUserGetExp( IN UidType iUnitUID )
{
	std::map< UidType, bool >::iterator mit = m_mapHaveExpInDungeon.find( iUnitUID );
	if( mit == m_mapHaveExpInDungeon.end() )
	{
		START_LOG( clog, L"����ġ�� ȹ���� ���� ����!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	return mit->second;
}
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
//}}

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT
bool KRoomUserManager::IsAraEvent()
{
	if( m_mapRoomUser[UT_GAME].size() < 2 )
	{
		return false;
	}

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		KRoomUserPtr spRoomUser = mit->second;
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( spRoomUser->GetUnitClass_LUA() == CXSLUnit::UC_ARA_LITTLE_HSIEN )
		{
			std::map< UidType, bool >::iterator mit = m_mapHaveExpInDungeon.find( spRoomUser->GetCID() );
			if( mit == m_mapHaveExpInDungeon.end() )
			{
				START_LOG( clog, L"����ġ�� ȹ���� ���� ����!" )
					<< BUILD_LOG( spRoomUser->GetCID() )
					<< END_LOG;
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}
#endif SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT

//{{ 2011. 05. 04  ��μ�	���� Ŭ����� ������ ���� ���� �߰�
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
bool KRoomUserManager::SetUnitAllQuestInfo( UidType nCID, std::set< int >& setQuestInfo )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cwarn, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}
	
	spRoomUser->SetUnitAllQuestInfo( setQuestInfo  );

	return true;
}
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
//}}

//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
bool KRoomUserManager::IsComeBackUserInRoom()
{
	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin() ; mit != m_mapRoomUser[UT_GAME].end() ; ++mit )
	{
		KRoomUserPtr spRoomUser = mit->second;
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( spRoomUser->IsComeBackUser() == true)
		{
			return true;
		}
	}
	return false;
}
#endif SERV_COME_BACK_USER_REWARD
//}}


//{{ 2011. 07. 21	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
int	KRoomUserManager::GetRatingByTeam( IN const CXSLRoom::TEAM_NUM eTeamNum )
{
	if( GetNumMember() > SEnum::PE_MAX_TOTAL_PLAYER_COUNT )
	{
        START_LOG( cerr, L"�� �������� ����Ϸ��µ� �ο����� �̻��ϴ�!" )
			<< BUILD_LOG( GetNumMember() )
			<< END_LOG;
		return -1;
	}

	std::map< UidType, int > mapTeamMemberRatingInfo;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin() ; mit != m_mapRoomUser[UT_GAME].end() ; ++mit )
	{
		KRoomUserPtr spRoomUser = mit->second;
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return -1;
		}

		if( spRoomUser->GetTeam() != eTeamNum )
			continue;

		mapTeamMemberRatingInfo.insert( std::make_pair( spRoomUser->GetCID(), spRoomUser->GetRating() ) );
	}
    	
	return SiKPvpMatchManager()->GetRatingByTeam( mapTeamMemberRatingInfo );
}

bool KRoomUserManager::SetPvpNpcInfo( IN const UidType iUnitUID, IN const int iNpcID )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	const CXSLUnitManager::NPCUnitStat* pNpcStat = SiCXSLUnitManager()->GetNPCUnitStat( static_cast<CXSLUnitManager::NPC_UNIT_ID>(iNpcID) );
	if( pNpcStat == NULL )
	{
		START_LOG( cerr, L"Npc Stat�� �������� �ʴ� NpcID�Դϴ�!" )
			<< BUILD_LOG( iNpcID )
			<< END_LOG;
        return false;
	}

	// NpcID���� ����!
	spRoomUser->SetPvpNpcID( iNpcID );

	// NpcID���� �ش��ϴ� Npc Stat�� �ҷ��ͼ� �����Ѵ�!
	KStat kNpcStat;
	kNpcStat.m_iBaseHP	  = static_cast<int>(pNpcStat->m_fMaxHP);
	kNpcStat.m_iAtkPhysic = static_cast<int>(pNpcStat->m_fAtkPhysic);
	kNpcStat.m_iAtkMagic  = static_cast<int>(pNpcStat->m_fAtkMagic);
	kNpcStat.m_iDefPhysic = static_cast<int>(pNpcStat->m_fDefPhysic);
	kNpcStat.m_iDefMagic  = static_cast<int>(pNpcStat->m_fDefMagic);
	spRoomUser->SetStat( kNpcStat );
	return true;
}

bool KRoomUserManager::IsPvpNpc( IN const UidType iUnitUID )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( clog, L"���� NPC�� �ƴմϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}
	
	return spRoomUser->IsPvpNpc();
}

bool KRoomUserManager::IsOnlyPvpNpcInRoom( OUT std::vector< UidType >& vecPvpNpcUnitUIDList ) const
{
	vecPvpNpcUnitUIDList.clear();

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

		if( spRoomUser->IsPvpNpc() == false )
			return false;

		vecPvpNpcUnitUIDList.push_back( spRoomUser->GetCID() );
	}

	return true;
}

bool KRoomUserManager::SetMatchWaitTime( IN const UidType iUnitUID, IN const int iMatchWaitTime )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( clog, L"���� ������ �����ϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spRoomUser->SetMatchWaitTime( iMatchWaitTime );
	return true;
}

#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
int KRoomUserManager::GetHenirRewardUserNum()
{
	int iHenirRewardUser = 0;

	std::map< UidType, KRoomUserPtr >::const_iterator mit = m_mapRoomUser[UT_GAME].begin();
	if( mit == m_mapRoomUser[UT_GAME].end() )
	{
		START_LOG( cerr, L"�� ������ �ϳ��� ���ٴ� ���� ���� �����ʴ´�." )
			<< BUILD_LOG( m_eRoomType )
			<< END_LOG;

		return 0;
	}

	KRoomUserPtr spRoomUser;

	for( ; mit != m_mapRoomUser[UT_GAME].end() ; ++mit )
	{
		spRoomUser = mit->second;
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�. ��ϸ� ���� ���� ���� ����!" )
				<< BUILD_LOG( m_eRoomType )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( spRoomUser->IsHenirRewardUser() == true )
		{
			// �Ѹ��̶� ������ ������ ���� �� �ִ�.

			iHenirRewardUser++;
		}
	}

	return iHenirRewardUser;
}

void KRoomUserManager::GetHenirRewardUserList( OUT std::map< UidType, UidType >& mapList )
{
	mapList.clear();

	std::map< UidType, KRoomUserPtr >::const_iterator mit = m_mapRoomUser[UT_GAME].begin();
	if( mit == m_mapRoomUser[UT_GAME].end() )
	{
		START_LOG( cerr, L"�� ������ �ϳ��� ���ٴ� ���� ���� �����ʴ´�." )
			<< BUILD_LOG( m_eRoomType )
			<< END_LOG;
		return;
	}

	KRoomUserPtr spRoomUser;

	for( ; mit != m_mapRoomUser[UT_GAME].end() ; ++mit )
	{
		spRoomUser = mit->second;
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�. ��ϸ� ���� ���� ���� ����!" )
				<< BUILD_LOG( m_eRoomType )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( spRoomUser->IsHenirRewardUser() == true )
		{
			mapList.insert( std::make_pair( spRoomUser->GetCID(), spRoomUser->GetGSUID() ) );
		}
	}
}
#endif SERV_NEW_HENIR_TEST
//}}

//{{ 2011. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
bool KRoomUserManager::SetBattleFieldNpcLoad( IN const UidType iUnitUID, IN const bool bVal )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"�ش� ĳ���� ������ ã�� ���Ͽ����ϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spRoomUser->SetBattleFieldNpcLoad( bVal );
	return true;
}

bool KRoomUserManager::IsBattleFieldNpcLoad( IN const UidType iUnitUID )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"�ش� ĳ���� ������ ã�� ���Ͽ����ϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	return spRoomUser->IsBattleFieldNpcLoad();
}

bool KRoomUserManager::SetBattleFieldNpcSyncSubjects( IN const UidType iUnitUID, IN const bool bVal )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"�ش� ĳ���� ������ ã�� ���Ͽ����ϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spRoomUser->SetBattleFieldNpcSyncSubjects( bVal );
	return true;
}

bool KRoomUserManager::IsBattleFieldNpcSyncSubjects( IN const UidType iUnitUID )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"�ش� ĳ���� ������ ã�� ���Ͽ����ϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}
	
	return spRoomUser->IsBattleFieldNpcSyncSubjects();
}

void KRoomUserManager::GetBattleFieldNpcSyncSubjectsList( OUT std::vector< UidType >& vecUnitUIDList )
{
	vecUnitUIDList.clear();

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

		// ������ ����!
		if( spRoomUser->IsHost() == true )
			continue;

		if( spRoomUser->IsBattleFieldNpcSyncSubjects() == true )
		{
			vecUnitUIDList.push_back( spRoomUser->GetCID() );
		}
	}
}

void KRoomUserManager::GetUnitUIDList( OUT std::vector< UidType >& vecUnitUIDList, IN const UidType iNotIncludeUnitUID /*= 0*/ ) const
{
	vecUnitUIDList.clear();

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

		if( spRoomUser->GetCID() == iNotIncludeUnitUID )
			continue;

		vecUnitUIDList.push_back( spRoomUser->GetCID() );
	}
}

bool KRoomUserManager::SetOldPartyInfo( IN const UidType iUnitUID, IN const KOldPartyInfo& kInfo )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"�ش� ĳ���� ������ ã�� ���Ͽ����ϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spRoomUser->SetOldPartyInfo( kInfo );
	return true;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}


//{{ 2012. 06. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KRoomUserManager::PrepareForReturnToFieldProcess( IN const UidType iRoomUID, OUT UidType& iGSUID, OUT UidType& iUnitUID, OUT KReturnToFieldInfo& kInfo )
{
	// ó�� �����ϴ°��̹Ƿ� �����͸� �ʱ�ȭ ��Ų��!
	m_mapReturnToFieldRemainUnitUID.clear();
	m_iSendReturnToFieldUnitUID = 0;

	// ���� ĳ���� ����Ʈ �����!
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

		m_mapReturnToFieldRemainUnitUID.insert( std::make_pair( spRoomUser->GetCID(), spRoomUser->GetGSUID() ) );
	}

	//{{ 2012. 11. 21	�ڼ���	��ȸ ä�� ���� ����
#if defined( SERV_FIX_ZOMBIE_OBSERVER ) || defined( SERV_TOURNAMENT_CONDITION )
	// ���� ĳ���� ����Ʈ �����!
	for( mit = m_mapRoomUser[UT_OBSERVER].begin() ; mit != m_mapRoomUser[UT_OBSERVER].end(); ++mit )
	{
		KRoomUserPtr spRoomUser = mit->second;
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		m_mapReturnToFieldRemainUnitUID.insert( std::make_pair( spRoomUser->GetCID(), spRoomUser->GetGSUID() ) );
	}
#endif SERV_TOURNAMENT_CONDITION
	//}}
	
	// ù��° ����� ����!
	GetNextReturnToFieldUnitInfo( iRoomUID, iGSUID, iUnitUID, kInfo );
}

bool KRoomUserManager::GetNextReturnToFieldUnitInfo( IN const UidType iRoomUID, OUT UidType& iGSUID, OUT UidType& iUnitUID, OUT KReturnToFieldInfo& kInfo )
{
	iGSUID = 0;
	iUnitUID = 0;
	bool bSelectedFirst = false;

	// �������� ���´� ĳ���� ������ �����Ѵ�!
	if( m_iSendReturnToFieldUnitUID != 0 )
	{
		m_mapReturnToFieldRemainUnitUID.erase( m_iSendReturnToFieldUnitUID );
        m_iSendReturnToFieldUnitUID = 0;
	}

	// �������� ĳ���Ͱ� ����!
	if( m_mapReturnToFieldRemainUnitUID.empty() )
		return false;

	// ����Ʈ �����	
	std::map< UidType, UidType >::const_iterator mit;
	for( mit = m_mapReturnToFieldRemainUnitUID.begin(); mit != m_mapReturnToFieldRemainUnitUID.end(); ++mit )
	{
		// ù��°�� ���õ� ����!
		if( bSelectedFirst == false )
		{
			bSelectedFirst = true;

			m_iSendReturnToFieldUnitUID = mit->first;
			iUnitUID = mit->first;
			iGSUID = mit->second;
		}
		// �ٸ� �������� ���� ��������Ʈ��!
		else
		{
			kInfo.m_setRemainUnitUID.insert( mit->first );
		}
	}

	kInfo.m_iReturnToFieldUnitUID = m_iSendReturnToFieldUnitUID;
	kInfo.m_iRoomUID = iRoomUID;
    return true;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
bool KRoomUserManager::EnemyIsPlacementTeam( IN const int& iMyTeam )
{
	// ������� ��ġ ��Ŀ�� ������ ���ΰ�?

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( iMyTeam == mit->second->GetTeam() )
			continue;

		if( mit->second->IsPlacementPvpUser() == true )
		{
			return true;
		}
	}

	return false;
}
#endif SERV_2012_PVP_SEASON2
//}}

//{{ 2012. 07. 13	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2_2
int KRoomUserManager::GetMyTeamKillNum( IN const UidType& iMyUnitUID )
{
	KRoomUserPtr pMyUnit = GetUser( iMyUnitUID );

	if( pMyUnit == NULL )
	{
		START_LOG( cerr, L"�� ���� ������ �̻�." )
			<< BUILD_LOG( pMyUnit )
			<< END_LOG;
		return -1;
	}

	std::map<int, int>::iterator mitTeamNumKill = m_mapTeamNumKill.find( pMyUnit->GetTeam() );
	if( mitTeamNumKill == m_mapTeamNumKill.end() )
	{
		return 0;
	}

	return mitTeamNumKill->second;
}
int KRoomUserManager::GetEnemyTeamKillNum( IN const UidType& iMyUnitUID )
{
	KRoomUserPtr pMyUnit = GetUser( iMyUnitUID );

	if( pMyUnit == NULL )
	{
		START_LOG( cerr, L"�� ���� ������ �̻�." )
			<< BUILD_LOG( pMyUnit )
			<< END_LOG;
		return -1;
	}

	int iMyTeam		= pMyUnit->GetTeam();
	int iEnemyTeam	= -1;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( iMyTeam != mit->second->GetTeam() )
		{
			iEnemyTeam = mit->second->GetTeam();
			break;
		}
	}

	if( iEnemyTeam < 0 )
	{
		START_LOG( cerr, L"�ùٸ��� ���� TeamIndex�Դϴ�." )
		<< BUILD_LOG( iEnemyTeam )
		<< END_LOG;
		return -1;
	}

	std::map<int, int>::iterator mitTeamNumKill = m_mapTeamNumKill.find( iEnemyTeam );
	if( mitTeamNumKill == m_mapTeamNumKill.end() )
	{
		return 0;
	}

	return mitTeamNumKill->second;
}
#endif SERV_2012_PVP_SEASON2_2
//}}

//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
int KRoomUserManager::GetUserLevel( IN const UidType& iMyUnitUID )
{
	KRoomUserPtr pMyUnit = GetUser( iMyUnitUID );

	if( pMyUnit == NULL )
	{
		START_LOG( cerr, L"�� ���� ������ �̻�." )
			<< BUILD_LOG( pMyUnit )
			<< END_LOG;
		return 1;
	}

	return pMyUnit->GetLevel();
}

float KRoomUserManager::GetPartyBonusRate( IN const int& iPartyMember )
{
	switch( iPartyMember )
	{
	case 1:
		return 0.0f;
		break;
	case 2:
		return 0.5f;
		break;
	case 3:
		return 1.0f;
		break;
	case 4:
		return 1.5f;
		break;
	}

	return 0.0f;
}

void KRoomUserManager::GetLiveMemberList( OUT std::vector< UidType >& vecLiveMember, IN USERLIST_TYPE eType/* = UT_GAME*/ )
{
	if( CheckUserlistType( eType ) == false )
		return;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		vecLiveMember.push_back( mit->second->GetCID() );
	}
}
#endif SERV_REFORM_ITEM_DROP
//}}

//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
bool KRoomUserManager::SetPingScore( UidType nCID, bool bForceHost, DWORD dwPingScore )
#else//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
bool KRoomUserManager::SetPingScore( UidType nCID, DWORD dwPingScore )
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
{
	USERLIST_TYPE eType = UT_GAME;

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	//���� host �����̶��...
	if( bForceHost )
	{
		std::map< UidType, KRoomUserPtr >::iterator mit;
		for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); ++mit )
		{
			KRoomUserPtr spRoomUser = mit->second;
			if( spRoomUser )
			{
				//���� host ������ ��û�� ������ ���� ���� ���� ������ ����
				if( mit->first == nCID )
				{
                    m_uidForceHostCID = nCID;
					spRoomUser->SetPingScoreForForceHost( 1 );
				}
				else
				{
					spRoomUser->SetPingScoreForForceHost( 99 );
				}
			}
		}

		return true;
	}
    else
    {
        if ( m_uidForceHostCID == nCID )
            m_uidForceHostCID = 0;
    }
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	KRoomUserPtr spRoomUser = GetUser( nCID, eType );
	if( !spRoomUser )
	{
		//{{ 2012. 11. 21	�ڼ���	��ȸ ä�� ���� ����
#if defined( SERV_FIX_ZOMBIE_OBSERVER ) || defined( SERV_TOURNAMENT_CONDITION )
		spRoomUser = GetUser( nCID, UT_OBSERVER );
		if( !spRoomUser )
		{
			START_LOG( cerr, L"�� ���� �˻� ����." )
				<< BUILD_LOG( nCID )
				<< END_LOG;

			return false;
		}
#else
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
#endif SERV_TOURNAMENT_CONDITION
		//}}
	}

	START_LOG( clog, L"�� ������ ping �ӵ��� ������." )
		<< BUILD_LOG( nCID )
		<< BUILD_LOG( dwPingScore )
		<< END_LOG;

	spRoomUser->SetPingScore( dwPingScore );
	return true;
}

//{{ 2013. 03. 06   ���� ���� ���� ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST_NEW
KRoomUserPtr KRoomUserManager::ChooseBetterHost( UidType iHostUID )
{
	USERLIST_TYPE eType = UT_GAME;

#if defined(SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE) || defined(SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK) // �۾���¥: 2013-05-08	// �ڼ���
	KRoomUserPtr spOldHost = KRoomUserPtr();
#endif


	KRoomUserPtr spNewHost = KRoomUserPtr();

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

#ifdef  SERV_PVP_NEW_SYSTEM
		if( mit->second->IsPvpNpc() == true )
			continue;
#endif  SERV_PVP_NEW_SYSTEM

#if defined(SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE) || defined(SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK) // �۾���¥: 2013-05-08	// �ڼ���
		if( mit->second->GetCID() == iHostUID )
		{
			spOldHost = mit->second;
		}
#endif

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
        if ( m_uidForceHostCID != 0 || mit->second->GetCID() != iHostUID )
        {
			if( mit->second->IsPlaying() )
			{
				if( IS_NOT_NULL( spNewHost ) )
				{
                    if ( spNewHost->GetCID() != m_uidForceHostCID )
                    {
                        if ( mit->second->GetCID() == m_uidForceHostCID
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
                            || spNewHost->ZombieAlert_Get() == true && mit->second->ZombieAlert_Get() == false
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
                            || spNewHost->GetPingScore() > mit->second->GetPingScore() )
					    {
						spNewHost = mit->second;
					    }
                    }
					
				}
				else
				{
					spNewHost = mit->second;
				}
			}
        }
#else//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK		
		if( mit->second->GetCID() != iHostUID )
		{
			if( mit->second->IsPlaying() )
			{
				if( IS_NOT_NULL( spNewHost ) )
				{
					if( 
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
                        spNewHost->ZombieAlert_Get() == true && mit->second->ZombieAlert_Get() == false ||
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST                        
                        spNewHost->GetPingScore() > mit->second->GetPingScore() )
					{
						spNewHost = mit->second;
					}

					
				}
				else
				{
					spNewHost = mit->second;
				}
			}
		}
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK	
	}

#if defined(SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE) || defined(SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK)// �۾���¥: 2013-05-08	// �ڼ���

#ifdef  SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
    if ( m_uidForceHostCID != 0 && ( spNewHost != NULL ) && spNewHost->GetCID() == m_uidForceHostCID )
        return spNewHost;
#endif  SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	if( ( spOldHost != NULL ) && ( spNewHost != NULL )
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST   
        && spOldHost->IsPlaying() == true
        && false == ( spOldHost->ZombieAlert_Get() == true && spNewHost->ZombieAlert_Get() == false ) 
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST   
        )
	{
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST 
        if ( spOldHost->ZombieAlert_Get() == false && spNewHost->ZombieAlert_Get() == true )
        {
            spNewHost = KRoomUserPtr();
        }
        else
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
        {
		    const DWORD dwMaxPingScore = SiKGameSysVal()->GetMaxPingScore();
    		
		    if( ( dwMaxPingScore <= spNewHost->GetPingScore() ) &&
			    ( spOldHost->GetPingScore() < dwMaxPingScore )
			    )
		    {
			    spNewHost = KRoomUserPtr();
		    }
        }
	}
#endif

	return spNewHost;
}
#else
KRoomUserPtr KRoomUserManager::ChooseBetterHost( DWORD dwHostPingScore
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
        , bool bHostIsPlaying, bool bHostZombieAlert
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
        )
{
	USERLIST_TYPE eType = UT_GAME;

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
	KRoomUserPtr spHostCandidate = KRoomUserPtr();
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

#ifdef  SERV_PVP_NEW_SYSTEM
		if( mit->second->IsPvpNpc() == true )
			continue;
#endif  SERV_PVP_NEW_SYSTEM

		if( !mit->second->IsHost() )
		{
			if( mit->second->IsPlaying())
			{
				DWORD dwPingScore = mit->second->GetPingScore();

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
                if ( mit->second->ZombieAlert_Get() == false )
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
                {
				    if( dwPingScore < PV_HIGH )
				    {
					    START_LOG( clog, L"�� ���°� �ſ� ���� ������ ã��" )
						    << BUILD_LOG( mit->first )
						    << BUILD_LOG( dwPingScore )
						    << BUILD_LOG( dwHostPingScore )
						    << END_LOG;

					    return mit->second;
				    }
				    else if( dwPingScore < dwHostPingScore )
				    {
					    START_LOG( clog, L"�� ���°� �ſ�� �ƴ����� ���� ���� ���� ���� ������ ã��" )
						    << BUILD_LOG( mit->first )
						    << BUILD_LOG( dwPingScore )
						    << BUILD_LOG( dwHostPingScore )
						    << END_LOG;

					    return mit->second;
				    }
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
                    else if ( bHostIsPlaying == false || bHostZombieAlert == true )
                    {
                        if ( spHostCandidate == NULL )
                        {
                            spHostCandidate = mit->second;
                        }
                        else if ( spHostCandidate->ZombieAlert_Get() == true 
                            || spHostCandidate->GetPingScore() < dwPingScore )
                        {
                            spHostCandidate = mit->second;
                        }
                    }
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
                }
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
                else if ( bHostIsPlaying == false )
                {
                    if ( spHostCandidate == NULL )
                    {
                        spHostCandidate = mit->second;
                    }
                }
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
			}
		}
	}

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    return  spHostCandidate;
#else   SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
	return KRoomUserPtr();
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
}
#endif SERV_CHOOSE_FASTEST_HOST_NEW
//}


		
		

				
			

void KRoomUserManager::ClearPingScore()
{
	USERLIST_TYPE eType = UT_GAME;

	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		mit->second->ClearPingScore();
	}
}
#endif SERV_CHOOSE_FASTEST_HOST
//}

//{{ �ڵ� ��Ƽ�� ���� ���� �� ��Ī ��� �ð� �α� �߰� - ��μ�
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
bool KRoomUserManager::SetAutoPartyWaitTime( IN const UidType iUnitUID, IN const int iAutoPartyWaitTime )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"�ش� ĳ���� ������ ã�� ���Ͽ����ϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spRoomUser->SetAutoPartyWaitTime( iAutoPartyWaitTime );
	return true;
}
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
//}

//{{ 2012. 12. 17	������	�ƶ� ��Ƽ �÷��� ���ʽ� ����ġ
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
bool KRoomUserManager::IsExistCharType( IN const CXSLUnit::UNIT_TYPE eUnitType ) const
{
	std::map< UidType, KRoomUserPtr >::const_iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( IS_NULL( mit->second ) )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->GetUnitType() == eUnitType )
			return true;
	}

	return false;
}
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
//}}

//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
void KRoomUserManager::SetUnitLevelBeforGameStart( IN const UidType iUnitUID, IN const bool bSuitableLevel )
{
	m_mapSuitableLevelInfo.insert( std::make_pair(iUnitUID, bSuitableLevel) );
}
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
//}}

#ifdef SERV_PC_BANG_TYPE
int KRoomUserManager::GetPcBangType( UidType nCID )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"GAME BANG ������ ã�µ� ������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return -1;
	}

	return spRoomUser->GetPcBangType();
}
#endif SERV_PC_BANG_TYPE

#ifdef SERV_PVP_REMATCH
bool KRoomUserManager::SetRematch( UidType nCID, bool bAcceptRematch )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spRoomUser->SetRematch( bAcceptRematch );

	return true;
}

bool KRoomUserManager::SetAllRematch( bool bAcceptRematch )
{
	USERLIST_TYPE eType = UT_GAME;
	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}
		mit->second->SetRematch( bAcceptRematch );
	}

	return true;
}

bool KRoomUserManager::IsAllPlayerWantRematch()
{
	char cNumRedTeam = 0;
	char cNumBlueTeam = 0;

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

		if( mit->second->IsPvpNpc() == true )
			return false;

		if( mit->second->IsAcceptRematch() == false )
			return false;

		if( mit->second->GetTeam() == CXSLRoom::TN_RED )
			++cNumRedTeam;
		else if( mit->second->GetTeam() == CXSLRoom::TN_BLUE )
			++cNumBlueTeam;
	}

	// �� �� �ο��� ����, �ο��� ���� ���� ����ġ
	if( cNumRedTeam == cNumBlueTeam && cNumRedTeam > 0 )
		return true;

	return false;
}
#endif SERV_PVP_REMATCH

#ifdef SERV_RELATIONSHIP_EVENT_INT
bool KRoomUserManager::CheckCouplePVP()
{
	USERLIST_TYPE eType = UT_GAME;
	
	if( m_mapRoomUser[eType].size() != 2 )
	{
		return false;
	}

	std::vector<UidType> vecRelation;
	std::map< UidType, KRoomUserPtr >::iterator mit;
	for( mit = m_mapRoomUser[eType].begin(); mit != m_mapRoomUser[eType].end(); mit++ )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		KRoomUserInfo kInfo;
		mit->second->GetRoomUserInfo( kInfo );

		if( kInfo.m_bCouple == true && kInfo.m_iRelationTargetUserUid > 0 )
		{
			vecRelation.push_back( kInfo.m_iRelationTargetUserUid );
		}
	}

	std::map< UidType, KRoomUserPtr >::iterator mit2;
	for( mit2 = m_mapRoomUser[eType].begin(); mit2 != m_mapRoomUser[eType].end(); mit2++ )
	{
		if( !mit2->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit2->first )
				<< END_LOG;
			continue;
		}

		KRoomUserInfo kInfo;
		mit2->second->GetRoomUserInfo( kInfo );

		for( int i = 0; i < vecRelation.size(); i++ )
		{
			if( kInfo.m_nUnitUID == vecRelation[i] )
			{
				return true;
			}
		}
	}

	return false;
}
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
bool KRoomUserManager::SetUnitGoingQuestInfo( UidType nCID, std::set< int >& setQuestInfo )
{
	KRoomUserPtr spRoomUser = GetUser( nCID );
	if( !spRoomUser )
	{
		START_LOG( cwarn, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spRoomUser->SetUnitGoingQuestInfo( setQuestInfo  );

	return true;
}
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST

#ifdef SERV_PAYMENT_ITEM_WITH_ALLY_NPC
bool KRoomUserManager::AddAllyNPC( IN const UidType iUnitUID, IN const KNPCUnitReq& kNPCUnit )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cwarn, L"�� ������ ��ȯ�� ��û�Ѱǵ� ����?" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spRoomUser->AddAllyNPC( kNPCUnit );
	return true;
}
void KRoomUserManager::DeleteAllyNPC( IN const UidType iNPCUID )
{
	std::map< UidType, KRoomUserPtr >::const_iterator mit;
	for( mit = m_mapRoomUser[UT_GAME].begin(); mit != m_mapRoomUser[UT_GAME].end(); ++mit )
	{
		if( IS_NULL( mit->second ) )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		mit->second->DeleteAllyNPC( iNPCUID );
	}
}
#endif SERV_PAYMENT_ITEM_WITH_ALLY_NPC

//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
void KRoomUserManager::SetAllPrepareForDefenceDungeon( IN const bool bVal )
{
	USERLIST_TYPE eType = UT_GAME;
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

		mit->second->SetPrepareForDefence( bVal );
	}
}

bool KRoomUserManager::SetPrepareForDefenceDungeon( IN const UidType iUnitUID, IN const bool bVal )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( IS_NULL( spRoomUser ) )
	{
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	KRoomSlotPtr spRoomSlot = GetSlot( spRoomUser->GetSlotID() );
	if( IS_NULL( spRoomSlot ) )
	{
		START_LOG( cerr, L"���� �˻� ����." )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( spRoomUser->GetSlotID() )
			<< END_LOG;
		return false;
	}

	spRoomUser->SetPrepareForDefence( bVal );
	return true;
}

bool KRoomUserManager::IsAllPlayerPrepareForDefenceDungeon()
{
	if( GetNumOccupiedSlot() == 0 )
		return false;

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

		if( !mit->second->IsPrepareForDefence() )
		{
			return false;
		}
	}

	return true;
}

void KRoomUserManager::GetUnitUIDListNotPrepareForDefenceDungeon( OUT std::map< UidType, UidType >& mapUserList )
{
	mapUserList.clear();

	USERLIST_TYPE eType = UT_GAME;
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

		if( mit->second->IsPrepareForDefence() == false )
		{
			mapUserList.insert( std::make_pair( mit->second->GetGSUID(), mit->second->GetCID() ) );
		}
	}
}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

//{{ 2013. 04. 22	������	����� �� ���� ���� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
void KRoomUserManager::SetAllEnterDefenceDungeon()
{
	USERLIST_TYPE eType = UT_GAME;
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

		// ����� �� ���� ���� �˾��� ���� ������ �Դ°�?
		if( mit->second->IsRecvEnterPopupReply() == true )
			continue;
		
		// ������ ���� ó�� ����!
		mit->second->SetRecvEnterPopupReply( true );
		mit->second->SetEnterDefenceDungeon( true );
	}
}

bool KRoomUserManager::SetEnterDefenceDungeon( IN const UidType iUnitUID, IN const bool bAgree )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( IS_NULL( spRoomUser ) )
	{
		START_LOG( cerr, L"�� ���� �˻� ����." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	KRoomSlotPtr spRoomSlot = GetSlot( spRoomUser->GetSlotID() );
	if( IS_NULL( spRoomSlot ) )
	{
		START_LOG( cerr, L"���� �˻� ����." )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( spRoomUser->GetSlotID() )
			<< END_LOG;
		return false;
	}

	spRoomUser->SetRecvEnterPopupReply( true );		// �˾��� ���� ���� ������ üũ
	spRoomUser->SetEnterDefenceDungeon( bAgree );	// ���� ��� ����
	return true;
}

bool KRoomUserManager::IsAllPlayerPrepareForDefenceDungeon( OUT std::vector< UidType >& vecLeaveUnitUID )
{
	vecLeaveUnitUID.clear();

	if( GetNumOccupiedSlot() == 0 )
		return false;

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

		// ����� �� ������ �����ϱ� ���� RoomUserInfo�� �޾Ҵ°�?
		if( mit->second->IsPrepareForDefence() == false )
		{
			return false;
		}

		// ����� �� ���� ���� �˾��� ���� ������ �Դ°�?
		if( mit->second->IsRecvEnterPopupReply() == false )
		{
			return false;
		}

		// ������ ������ ������ �濡�� ��Żó�� �ϵ��� ����!
		if( mit->second->IsEnterDefenceDungeon() == false )
		{
			vecLeaveUnitUID.push_back( mit->second->GetCID() );
		}
	}

	return true;    
}

void KRoomUserManager::GetUnitUIDListDisagreeEnterDefenceDungeon( OUT std::vector< UidType >& vecLeaveUnitUID )
{
	vecLeaveUnitUID.clear();

	USERLIST_TYPE eType = UT_GAME;
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

		// ����� �� ������ �����ϱ� ���� RoomUserInfo�� �޾Ҵ°�?
		if( mit->second->IsPrepareForDefence() == false )
			continue;

		// ����� �� ���� ���� �˾��� ���� ������ �Դ°�?
		if( mit->second->IsRecvEnterPopupReply() == false )
			continue;

		// ������ ������ ������ �濡�� ��Żó�� �ϵ��� ����!
		if( mit->second->IsEnterDefenceDungeon() == false )
		{
			vecLeaveUnitUID.push_back( mit->second->GetCID() );
		}
	}
}
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
//}}

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
bool KRoomUserManager::SetRidingPetInfo( IN const UidType iUnitUID, IN const UidType iRidingPetUID, IN const USHORT usRidingPetID )
{
	KRoomUserPtr spRoomUser = GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"���� ���̵� �� ������ ������Ʈ �Ϸ��� �ϴµ� �� ������ ����?" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spRoomUser->SetRidingPetInfo( iRidingPetUID, usRidingPetID );
	return true;
}
#endif	// SERV_RIDING_PET_SYSTM



#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	//bool IsForceHost() { return m_bForceHost; }
UidType KRoomUserManager::GetForceHostCID() 
{ 
    return m_uidForceHostCID; 
}

#endif  SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK