#include "FieldUserManager.h"

#include "Profiler/Profiler.h"

KFieldUserManager::KFieldUserManager()
:
m_nMaxUser( 0 )
{
}

KFieldUserManager::~KFieldUserManager()
{
}

//{{ 2009. 3. 12  ������	����
void KFieldUserManager::Dump()
{
	dbg::cout << L"mapUserList Size : " << m_mapUserList.size() << dbg::endl;
	dbg::cout << L"vecUserList Size : " << m_vecUserList.size() << dbg::endl;
		
	std::map< SectorID, FieldUserListPtr >::const_iterator mit;
	for( mit = m_mapSectorUser.begin(); mit != m_mapSectorUser.end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		int iX = KSectorID::ExtractX( mit->first );
		int	iY = KSectorID::ExtractY( mit->first );
		int	iZ = KSectorID::ExtractZ( mit->first );

		dbg::cout << L"[" << iX << L":" << iY << L":" << iZ << L"] sector user list size : " << (*mit->second).size() << dbg::endl;
	}
}
//}}

void KFieldUserManager::Init( int nMaxUser )
{
	m_nMaxUser = nMaxUser;
}

KFieldUserPtr KFieldUserManager::GetUser( UidType nCID )
{
	std::map< UidType, KFieldUserPtr >::iterator mit;
	mit = m_mapUserList.find( nCID );

	if( mit == m_mapUserList.end() )
	{
		START_LOG( cwarn, L"�������� �ʴ� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return KFieldUserPtr();
	}

	return mit->second;
}

bool KFieldUserManager::GetSectorID( UidType nCID, SectorID& iSectorID )
{
    iSectorID = 0;

	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"���� ã�� ����" )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

	iSectorID = spFieldUser->GetMySectorID();
	return true;
}

bool KFieldUserManager::AddUser( KFieldUserPtr spFieldUser,
                                 const VECTOR3& vPos,
                                 std::vector< UidType >& vecNearbyUnitAddedThis,
                                 std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis )
{
	if( !spFieldUser )
	{
		START_LOG( cerr, L"���� ������ �̻�." )
			<< END_LOG;

		return false;
	}

    KFieldUserInfo kFieldUserInfoOfThis;
    vecNearbyUnitAddedThis.clear();
    std::vector< UidType > vecNearbyUnitRemovedThis;
    vecNearbyUnitAddedToThis.clear();
    std::vector< UidType > vecNearbyUnitRemovedFromThis;

    std::pair< std::map< UidType, KFieldUserPtr >::iterator, bool > pairResult;
    pairResult = m_mapUserList.insert( std::make_pair( spFieldUser->GetCID(), spFieldUser ) );

    if( !pairResult.second )
    {
        START_LOG( cerr, L"�̹� �����ϴ� �ʵ� ������." )
            << BUILD_LOG( spFieldUser->GetCID() )
            << END_LOG;

        return false;
	}

	//{{ 2009. 1. 15  ������	���� ����Ʈ
	if( !AddUserList( spFieldUser ) )
	{
		START_LOG( cerr, L"���� ����Ʈ�� �̹� �ش� ������ �����ϰ� �ִ�." )
			<< BUILD_LOG( spFieldUser->GetCID() )
			<< END_LOG;

		// ������ ������ ����Ʈ���� �����.
		RemoveUserList( spFieldUser->GetCID() );

		// �ٽ� ����Ʈ�� �߰�
		if( !AddUserList( spFieldUser ) )
		{
			START_LOG( cerr, L"����Ʈ�� �ߺ����� ����� �߰��Ǿ�� �ϴµ�?" )
				<< BUILD_LOG( spFieldUser->GetCID() )
				<< END_LOG;
			return false;
		}
	}
	//}}

    LIF( MoveUser( spFieldUser, vPos, kFieldUserInfoOfThis, vecNearbyUnitAddedThis, vecNearbyUnitRemovedThis, vecNearbyUnitAddedToThis, vecNearbyUnitRemovedFromThis ) );
    LIF( vecNearbyUnitRemovedThis.size() == 0 );
    LIF( vecNearbyUnitRemovedFromThis.size() == 0 );

    return true;
}

bool KFieldUserManager::RemoveUser( UidType nCID,
                                    std::vector< UidType >& vecNearbyUnitRemovedThis )
{
    KFieldUserInfo kFieldUserInfoOfThis;
    std::vector< UidType > vecNearbyUnitAddedThis;
    vecNearbyUnitRemovedThis.clear();
    std::vector< KFieldUserInfo > vecNearbyUnitAddedToThis;
    std::vector< UidType > vecNearbyUnitRemovedFromThis;

	//{{ 2009. 1. 15  ������	���� ����Ʈ
	if( !RemoveUserList( nCID ) )
	{
		START_LOG( cerr, L"���� ����Ʈ���� �����Ϸ��µ� ������ ����?" )
			<< BUILD_LOG( nCID )
			<< END_LOG;
	}
	//}}

	std::map< UidType, KFieldUserPtr >::iterator mit;
	mit = m_mapUserList.find( nCID );
	if( mit == m_mapUserList.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� �ʵ� ������." )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

    if( !mit->second )
    {
        START_LOG( cerr, L"�ʵ� ���� ������ �̻�" )
            << BUILD_LOG( nCID )
            << END_LOG;

        m_mapUserList.erase( mit );
        return false;
    }

    LIF( MoveUser( mit->second, INVALID_SECTOR, kFieldUserInfoOfThis, vecNearbyUnitAddedThis, vecNearbyUnitRemovedThis, vecNearbyUnitAddedToThis, vecNearbyUnitRemovedFromThis ) );
    LIF( vecNearbyUnitAddedThis.size() == 0 );
    LIF( vecNearbyUnitAddedToThis.size() == 0 );

    m_mapUserList.erase( mit );
	return true;
}

bool KFieldUserManager::MoveUser( KFieldUserPtr spFieldUser,
                                  SectorID iDestSectorID,
                                  KFieldUserInfo& kFieldUserInfoOfThis,
                                  std::vector< UidType >& vecNearbyUnitAddedThis,
                                  std::vector< UidType >& vecNearbyUnitRemovedThis,
                                  std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis,
                                  std::vector< UidType >& vecNearbyUnitRemovedFromThis )
{
    if( !spFieldUser )
    {
        START_LOG( cerr, L"���� ������ �̻�" )
            << BUILD_LOG( iDestSectorID )
            << END_LOG;

        return false;
    }

    vecNearbyUnitAddedThis.clear();
    vecNearbyUnitRemovedThis.clear();
    vecNearbyUnitAddedToThis.clear();
    vecNearbyUnitRemovedFromThis.clear();

    UidType iUnitUID = spFieldUser->GetCID();

    // ���Ͱ� �޶����� �ʾ����� �̵� �Ϸ�.
    SectorID iSrcSectorID = spFieldUser->GetMySectorID();

    if( iDestSectorID == iSrcSectorID )
    {
        spFieldUser->GetFieldUserInfo( kFieldUserInfoOfThis );
        return true;
    }

    SectorsPtr spSrcSectors = GetAdjacentSectors( iSrcSectorID ); _JIF( spSrcSectors, return false );
    SectorsPtr spDestSectors = GetAdjacentSectors( iDestSectorID ); _JIF( spDestSectors, return false );

    // ���� �־��� ���Ϳ��� ������
    if( !RemoveUserFromSector( iSrcSectorID, iUnitUID ) )
    {
        START_LOG( cerr, L"���Ϳ��� ������ ����." )
            << BUILD_LOG( iUnitUID )
            << BUILD_LOG( iDestSectorID )
            << END_LOG;

        return false;
    }

    // �ش� ���Ϳ� ����
    if( !AddUserToSector( iDestSectorID, spFieldUser ) )
    {
        START_LOG( cerr, L"���Ϳ� ���� �߰� ����" )
            << BUILD_LOG( iDestSectorID )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return false;
    }
    spFieldUser->SetMySectorID( iDestSectorID );

    // �ֺ� ���� ���� ����
    spFieldUser->SetAdjacentSectors( spDestSectors );

    // �� �ʵ����� ���� ���
    spFieldUser->GetFieldUserInfo( kFieldUserInfoOfThis );

	// ��ȭ�� ���� ���͸� ����Ѵ�
    std::vector< SectorID > vecSrcRemainder;
    std::vector< SectorID > vecDestRemainder;
    KSectorID::FindRemainder( *spSrcSectors, *spDestSectors, vecSrcRemainder, vecDestRemainder );

	// �ֺ������߿��� ������ ��Ż�� �˷����� �������� recv user list�� �˻��Ͽ� ��Żó�� �Ѵ�.
	std::map< UidType, KFieldUserPtr > mapSrcRemainderUnit;
    std::map< UidType, KFieldUserPtr > mapDestRemainderUnit;
    std::map< UidType, KFieldUserPtr > mapCandidateOther;
    std::map< UidType, KFieldUserPtr >::iterator mitOtherUnit;
    std::map< UidType, KFieldUserPtr >::iterator mitCandidateOther;

    GetSectorUnitList( vecSrcRemainder, mapSrcRemainderUnit );

	//////////////////////////////////////////////////////////////////////////
	// ��Ż ������ ������ ���� ó��

	for( mitOtherUnit = mapSrcRemainderUnit.begin(); mitOtherUnit != mapSrcRemainderUnit.end(); ++mitOtherUnit )
	{
        if( !mitOtherUnit->second )
        {
            START_LOG( cerr, L"������ �̻�" )
                << BUILD_LOG( mitOtherUnit->first )
                << END_LOG;

            continue;
        }

		// ���� �� ������ ����Ʈ�� ������..
		bool bDeletedFromOther = mitOtherUnit->second->DelEventRecvUser( iUnitUID );
		if( bDeletedFromOther )
		{
			// ���� send ����Ʈ���� �� ������ ����
			if( !spFieldUser->DelEventSendUser( mitOtherUnit->first ) )
			{
				START_LOG( cerr, L"���� ����Ʈ �̻�" )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
			}
		}

        // ���� �� ������ ����Ʈ�� �־��ų� ��Ƽ���� ��
        if( bDeletedFromOther || mitOtherUnit->second->IsParty( iUnitUID ) || mitOtherUnit->second->IsFriend( iUnitUID ) )
        {
			// �� �������� ���� delete �ϵ��� �˷�����Ѵ�.
			vecNearbyUnitRemovedThis.push_back( mitOtherUnit->first );
        }

        // �� ������ �� ����Ʈ�� ������..
		bool bDeletedFromThis = spFieldUser->DelEventRecvUser( mitOtherUnit->first );
		if( bDeletedFromThis )
		{
			// �� ������ send ����Ʈ���� �� ����.
			if( !mitOtherUnit->second->DelEventSendUser( iUnitUID ) )
			{
				START_LOG( cerr, L"���� ����Ʈ �̻�" )
					<< BUILD_LOG( mitOtherUnit->first )
					<< END_LOG;
			}
		}

		// �� ������ �� ����Ʈ�� �־��ų� ��Ƽ���� ��
        if( bDeletedFromThis || spFieldUser->IsParty( mitOtherUnit->first ) || spFieldUser->IsFriend( mitOtherUnit->first ) )
        {
            // ������ �� ������ delete �ϵ��� �˷�����Ѵ�.
			vecNearbyUnitRemovedFromThis.push_back( mitOtherUnit->first );
        }
	}

	// �ֺ������߿��� ������ ������ �˷����� �������� recv user list�� �˻��Ͽ� ����ó�� �Ѵ�
    GetSectorUnitList( vecDestRemainder, mapDestRemainderUnit );

	//////////////////////////////////////////////////////////////////////////
	// ���� ������ ������ ���� ó��

	for( mitOtherUnit = mapDestRemainderUnit.begin(); mitOtherUnit != mapDestRemainderUnit.end(); ++mitOtherUnit )
	{
        if( !mitOtherUnit->second )
        {
            START_LOG( cerr, L"���� ������ �̻�." )
                << BUILD_LOG( mitOtherUnit->first )
                << END_LOG;

            continue;
        }

		// �� ������ ����Ʈ�� ���� ���������..
		bool bAddFromOther = mitOtherUnit->second->AddEventRecvUser( iUnitUID );
		if( bAddFromOther )
		{
			// ���� send list�� �� ������ �߰��ؾ� �Ѵ�.
			if( !spFieldUser->AddEventSendUser( mitOtherUnit->first ) )
			{
				START_LOG( cerr, L"���� ����Ʈ �̻�" )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
			}
		}

        // �� ������ ����Ʈ�� ���� ���ų� ��Ƽ���� ���
        if( bAddFromOther || mitOtherUnit->second->IsParty( iUnitUID ) || mitOtherUnit->second->IsFriend( iUnitUID ) )
        {
			// ���� �߰��϶�� �˸���.
			vecNearbyUnitAddedThis.push_back( mitOtherUnit->first );            
        }

		// �� ����Ʈ�� �� ������ ����������..
		bool bAddFromThis = spFieldUser->AddEventRecvUser( mitOtherUnit->first );
		if( bAddFromThis )
		{
			// �� ������ send ����Ʈ�� �� �߰��Ѵ�.
			if( !mitOtherUnit->second->AddEventSendUser( iUnitUID ) )
			{
				START_LOG( cerr, L"���� ����Ʈ �̻�" )
					<< BUILD_LOG( mitOtherUnit->first )
					<< END_LOG;
			}
		}

        // �� ����Ʈ�� �� ������ ���ų� ��Ƽ���� ����� ó��.
        if( bAddFromThis || spFieldUser->IsParty( mitOtherUnit->first ) || spFieldUser->IsFriend( mitOtherUnit->first ) )
        {
            // ������ �� ������ �߰� �ϵ��� �˷�����Ѵ�.
            KFieldUserInfo kFieldUserInfo;
            mitOtherUnit->second->GetFieldUserInfo( kFieldUserInfo );
            vecNearbyUnitAddedToThis.push_back( kFieldUserInfo );            
        }
    }

	return true;
}

bool KFieldUserManager::MoveUser( UidType iUnitUID,
                                  SectorID iDestSectorID,
                                  KFieldUserInfo& kFieldUserInfoOfThis,
                                  std::vector< UidType >& vecNearbyUnitAddedThis,
                                  std::vector< UidType >& vecNearbyUnitRemovedThis,
                                  std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis,
                                  std::vector< UidType >& vecNearbyUnitRemovedFromThis )
{
    vecNearbyUnitAddedThis.clear();
    vecNearbyUnitRemovedThis.clear();
    vecNearbyUnitAddedToThis.clear();
    vecNearbyUnitRemovedFromThis.clear();

    KFieldUserPtr spFieldUser = GetUser( iUnitUID );
    if( !spFieldUser )
    {
        START_LOG( cerr, L"���� �˻� ����." )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return false;
    }

    return MoveUser( spFieldUser, iDestSectorID, kFieldUserInfoOfThis, vecNearbyUnitAddedThis, vecNearbyUnitRemovedThis, vecNearbyUnitAddedToThis, vecNearbyUnitRemovedFromThis );
}

bool KFieldUserManager::MoveUser( KFieldUserPtr spFieldUser,
                                  const VECTOR3& vPos,
                                  KFieldUserInfo& kFieldUserInfoOfThis,
                                  std::vector< UidType >& vecNearbyUnitAddedThis,
                                  std::vector< UidType >& vecNearbyUnitRemovedThis,
                                  std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis,
                                  std::vector< UidType >& vecNearbyUnitRemovedFromThis)
{
    if( !spFieldUser )
    {
        START_LOG( cerr, L"���� ������ �̻�" )
            << END_LOG;

        return false;
    }

    vecNearbyUnitAddedThis.clear();
    vecNearbyUnitRemovedThis.clear();
    vecNearbyUnitAddedToThis.clear();
    vecNearbyUnitRemovedFromThis.clear();

    if( !MoveUser( spFieldUser, GetDestSectorID( vPos ), kFieldUserInfoOfThis, vecNearbyUnitAddedThis, vecNearbyUnitRemovedThis, vecNearbyUnitAddedToThis, vecNearbyUnitRemovedFromThis ) )
    {
        START_LOG( cerr, L"���� �̵� ����" )
            << BUILD_LOG( spFieldUser->GetCID() )
            << END_LOG;

        return false;
    }

    spFieldUser->UpdateUserPos( vPos );
    return true;
}

bool KFieldUserManager::MoveUser( UidType iUnitUID,
                                  const VECTOR3& vPos,
                                  KFieldUserInfo& kFieldUserInfoOfThis,
                                  std::vector< UidType >& vecNearbyUnitAddedThis,
                                  std::vector< UidType >& vecNearbyUnitRemovedThis,
                                  std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis,
                                  std::vector< UidType >& vecNearbyUnitRemovedFromThis )
{
    vecNearbyUnitAddedThis.clear();
    vecNearbyUnitRemovedThis.clear();
    vecNearbyUnitAddedToThis.clear();
    vecNearbyUnitRemovedFromThis.clear();
    
    SectorID iSectorID = GetDestSectorID( vPos );

    if( !MoveUser( iUnitUID, iSectorID, kFieldUserInfoOfThis, vecNearbyUnitAddedThis, vecNearbyUnitRemovedThis, vecNearbyUnitAddedToThis, vecNearbyUnitRemovedFromThis ) )
    {
        START_LOG( cerr, L"���� �̵� ����" )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return false;
    }

    if( !UpdateUserPos( iUnitUID, vPos ) )
    {
        START_LOG( cerr, L"���� ��ġ ������Ʈ ����." )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return false;
    }

    return true;
}

bool KFieldUserManager::EnterField( const KERM_JOIN_FIELD_REQ& kJoinUser,
                                    std::vector< UidType >& vecNearbyUnitAddedThis,
                                    std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis )
{
    vecNearbyUnitAddedThis.clear();
    vecNearbyUnitAddedToThis.clear();

	KFieldUserPtr spFieldUser = MakeUser( kJoinUser.m_kFieldUserInfo, kJoinUser.m_setPartyUIDList, kJoinUser.m_setFriendUIDList, kJoinUser.m_kFieldOption );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"�ʵ� ���� ���� ����." )
			<< BUILD_LOG( ( int )GetNumMember() )
			<< END_LOG;

		return false;
	}

	if( !AddUser( spFieldUser, kJoinUser.m_kFieldUserInfo.m_vPos, vecNearbyUnitAddedThis, vecNearbyUnitAddedToThis ) )
	{
		START_LOG( cerr, L"�ʵ� ���� �߰� ����." )
			<< BUILD_LOG( spFieldUser->GetCID() )
			<< END_LOG;

		return false;
	}

	return true;
}

bool KFieldUserManager::LeaveField( UidType nCID, std::vector< UidType >& vecNearbyUnitRemovedThis )
{
    vecNearbyUnitRemovedThis.clear();

	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"�ʵ� �������� ���� ã�� ����!" )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	if( !RemoveUser( nCID, vecNearbyUnitRemovedThis ) )
    {
        START_LOG( cerr, L"�ʵ� ������ ����" )
            << BUILD_LOG( nCID )
            << END_LOG;
    
        return false;
    }

    return true;
}

//{{ 2009. 1. 15  ������	���� ����Ʈ
bool KFieldUserManager::AddUserList( KFieldUserPtr spFieldUser )
{	
	std::vector< KFieldUserPtr >::const_iterator vit;
	for( vit = m_vecUserList.begin(); vit != m_vecUserList.end(); ++vit )
	{
		KFieldUserPtr spCheckFieldUser = *vit;
		if( spCheckFieldUser == NULL )
		{
			START_LOG( cerr, L"�ʵ����� �����Ͱ� �̻��մϴ�." )
				<< BUILD_LOG( spFieldUser->GetCID() )
				<< END_LOG;
			continue;
		}

		if( spCheckFieldUser->GetCID() == spFieldUser->GetCID() )
		{
			START_LOG( cerr, L"�̹� �����ϴ� �ʵ� ������." )
				<< BUILD_LOG( spFieldUser->GetCID() )
				<< END_LOG;
			return false;
		}
	}

	m_vecUserList.push_back( spFieldUser );
	return true;
}

bool KFieldUserManager::RemoveUserList( UidType nCID )
{
	std::vector< KFieldUserPtr >::iterator vit;
	for( vit = m_vecUserList.begin(); vit != m_vecUserList.end(); ++vit )
	{
		KFieldUserPtr spCheckFieldUser = *vit;
		if( spCheckFieldUser == NULL )
		{
			START_LOG( cerr, L"�ʵ����� �����Ͱ� �̻��մϴ�." )
				<< BUILD_LOG( nCID )
				<< END_LOG;
			continue;
		}

		if( spCheckFieldUser->GetCID() == nCID )
			break;
	}

	if( m_vecUserList.end() == vit )
	{
		START_LOG( cerr, L"��������Ʈ���� ������ �����Ϸ��µ� ������ �����ϴ�." )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

	m_vecUserList.erase( vit );
	return true;
}
//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
bool KFieldUserManager::GetFieldUserList( UINT& uiViewPage, UINT& uiTotalPage, std::vector< KCommunityUserInfo >& vecUserList, bool bEdView /*false*/ )
#else
bool KFieldUserManager::GetFieldUserList( UINT& uiViewPage, UINT& uiTotalPage, std::vector< KCommunityUserInfo >& vecUserList )
#endif SERV_ED_MONITORING_IN_GAME
//}}
{
	vecUserList.clear();

	_JIF( uiViewPage > 0, return false; );

	if( m_vecUserList.empty() )
	{
		uiViewPage = 1;
		uiTotalPage = 1;
		return true;
	}
	
	uiTotalPage = m_vecUserList.size() / PAGE_PER_USER_COUNT;
	if( ( m_vecUserList.size() % PAGE_PER_USER_COUNT ) > 0 )
	{
		++uiTotalPage;
	}

	u_int uiBeginIndex = ( uiViewPage * PAGE_PER_USER_COUNT ) - PAGE_PER_USER_COUNT;
	if( m_vecUserList.size() <= uiBeginIndex )
	{
        uiViewPage = uiTotalPage; // ������ ������
		uiBeginIndex = ( uiViewPage * PAGE_PER_USER_COUNT ) - PAGE_PER_USER_COUNT;

		if( m_vecUserList.size() <= uiBeginIndex )
		{
			START_LOG( cerr, L"�Ͼ �� ���� ����!" )
				<< BUILD_LOG( uiViewPage )
				<< BUILD_LOG( uiTotalPage )
				<< BUILD_LOG( uiBeginIndex )
				<< BUILD_LOG( m_vecUserList.size() )
				<< END_LOG;
			return false;
		}
	}
	
	std::vector< KFieldUserPtr >::reverse_iterator vit = m_vecUserList.rbegin();
	vit += uiBeginIndex;

	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
	KCommunityUserInfo kTemp;
#endif SERV_ED_MONITORING_IN_GAME
	//}}

	while( vit != m_vecUserList.rend() )
	{
		KFieldUserPtr spFieldUser = *vit;
		if( spFieldUser == NULL )
		{
			START_LOG( cerr, L"�ʵ� ���� �����Ͱ� �̻��մϴ�." )
				<< END_LOG;			
		}
		else
		{
			//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
			kTemp = spFieldUser->GetCommunityUserInfo();
			if( bEdView == false )
			{
				kTemp.m_iED = 0;
			}
            vecUserList.push_back( kTemp );
#else
			vecUserList.push_back( spFieldUser->GetCommunityUserInfo() );
#endif SERV_ED_MONITORING_IN_GAME
			//}}
			
			if( vecUserList.size() >= PAGE_PER_USER_COUNT )
				break;
		}

		++vit;
	}

	return true;
}
//}}

//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE

bool KFieldUserManager::SetUserEquippedTitle( IN UidType nCID, IN int iEquippedTitleID )
{
	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spFieldUser->SetUserEquippedTitle( iEquippedTitleID );
	return true;
}

#else

bool KFieldUserManager::SetUserEquippedTitle( IN UidType nCID, IN short sEquippedTitleID )
{
	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spFieldUser->SetUserEquippedTitle( sEquippedTitleID );
	return true;
}

#endif SERV_TITLE_DATA_SIZE
//}}

bool KFieldUserManager::SetUserEquipItem( IN UidType nCID, IN std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spFieldUser->SetUserEquipItem( vecInventorySlotInfo );
    return true;
}

bool KFieldUserManager::SetFieldOption( IN UidType nCID, IN const KFieldOption& kFieldOption )
{
	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"�ʵ� �ɼ��� ������Ʈ �Ϸ��� �ϴµ� �ʵ������� ����?" )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

	spFieldUser->SetFieldOption( kFieldOption );
	return true;
}

bool KFieldUserManager::SetParty( IN UidType nCID, IN UidType iPartyUID, IN const std::set< UidType >& setParty, OUT std::vector< KFieldUserInfo >& vecAdded, OUT std::vector< UidType >& vecRemoved )
{
    vecAdded.clear();
    vecRemoved.clear();

    KFieldUserPtr spFieldUser = GetUser( nCID );
    if( !spFieldUser )
    {
        START_LOG( cerr, L"���� ã�� ����" )
            << BUILD_LOG( nCID )
			<< BUILD_LOG( setParty.size() )
            << END_LOG;
        return false;
    }

	//////////////////////////////////////////////////////////////////////////	
    std::set< UidType > setPartyRev = setParty;

	// ��Ƽ ���� üũ
	if( !setPartyRev.empty() )
	{
        if( setPartyRev.find( nCID ) == setPartyRev.end() )
		{
			START_LOG( cerr, L"��Ƽ ����Ʈ�� ������Ʈ �Ϸ��� �ϴµ� ���� ��Ƽ����Ʈ�� ����?" )
				<< BUILD_LOG( nCID )
				<< BUILD_LOG( setPartyRev.size() )
				<< END_LOG;
		}
	}

	// �ڱ� �ڽ��� ������.
	setPartyRev.erase( nCID );

    std::set< UidType > setCurrentParty;
    spFieldUser->GetParty( setCurrentParty );

    std::set< UidType >::iterator sitParty = setPartyRev.begin();
    std::set< UidType >::iterator sitCurrentParty = setCurrentParty.begin();

    while( sitParty != setPartyRev.end() || sitCurrentParty != setCurrentParty.end() )
    {
        if( sitParty == setPartyRev.end() )
        {
            if( !spFieldUser->IsEventRecvUser( *sitCurrentParty )  &&  !spFieldUser->IsFriend( *sitCurrentParty ) )
            {
                vecRemoved.push_back( *sitCurrentParty );
            }

            sitCurrentParty++;
            continue;
        }
        
        if( sitCurrentParty == setCurrentParty.end() )
        {
            if( !spFieldUser->IsEventRecvUser( *sitParty )  &&  !spFieldUser->IsFriend( *sitParty ) )
            {
                KFieldUserPtr spPartyUser = GetUser( *sitParty );
                if( !spPartyUser )
                {
					// [����] ������ ��ã�� ��찡 ���� : ��Ƽ�� �δ� ���� ���� �ٸ� �ʵ忡 ��Ƽ��û�ڿ� ��Ƽ���� ��ġ�ϰ������� ������ ��ã�´�!

					// �ٸ� �ʵ忡 �����Ƿ� �ʵ����������� �����ʿ䰡 ����, ���� �α״� cwarnó����.

                    START_LOG( cwarn, L"���� ã�� ����" )
                        << BUILD_LOG( *sitParty )
                        << END_LOG;

                    sitParty++;
                    continue;
                }
                KFieldUserInfo kFieldUserInfo;
                spPartyUser->GetFieldUserInfo( kFieldUserInfo );
                vecAdded.push_back( kFieldUserInfo );
            }

            sitParty++;
            continue;
        }

        if( *sitParty < *sitCurrentParty )
        {
            if( !spFieldUser->IsEventRecvUser( *sitParty )  &&  !spFieldUser->IsFriend( *sitParty ) )
            {
                KFieldUserPtr spPartyUser = GetUser( *sitParty );
                if( !spPartyUser )
                {
					// [����] ������ ��ã�� ��찡 ���� : ��Ƽ�� �δ� ���� ���� �ٸ� �ʵ忡 ��Ƽ��û�ڿ� ��Ƽ���� ��ġ�ϰ������� ������ ��ã�´�!

					// �ٸ� �ʵ忡 �����Ƿ� �ʵ����������� �����ʿ䰡 ����, ���� �α״� cwarnó����.

                    START_LOG( cwarn, L"���� ã�� ����" )
                        << BUILD_LOG( *sitParty )
                        << END_LOG;

                    sitParty++;
                    continue;
                }
                KFieldUserInfo kFieldUserInfo;
                spPartyUser->GetFieldUserInfo( kFieldUserInfo );
                vecAdded.push_back( kFieldUserInfo );
            }

            sitParty++;
            continue;
        }
        else if( *sitParty > *sitCurrentParty )
        {
            if( !spFieldUser->IsEventRecvUser( *sitCurrentParty )  &&  !spFieldUser->IsFriend( *sitCurrentParty ) )
            {
                vecRemoved.push_back( *sitCurrentParty );
            }

            sitCurrentParty++;
            continue;
        }
        else
        {
            sitParty++;
            sitCurrentParty++;
            continue;
        }
    }

    spFieldUser->SetParty( setPartyRev );

	//{{ 2009. 4. 1  ������		��ƼUID ������Ʈ
	spFieldUser->SetPartyUID( iPartyUID );
	//}}

    return true;
}

//{{ 2009. 4. 9  ������		ģ������Ʈ
bool KFieldUserManager::SetFriend( IN UidType nCID, IN const std::set< UidType >& setFriend, OUT std::vector< KFieldUserInfo >& vecAdded, OUT std::vector< UidType >& vecRemoved )
{
	vecAdded.clear();
	vecRemoved.clear();

	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"���� ã�� ����" )
			<< BUILD_LOG( nCID )
			<< BUILD_LOG( setFriend.size() )
			<< END_LOG;
		return false;
	}

	//////////////////////////////////////////////////////////////////////////	
	std::set< UidType > setFriendRev = setFriend;

	// ģ�� ���� üũ
	if( !setFriendRev.empty() )
	{
		if( setFriendRev.find( nCID ) != setFriendRev.end() )
		{
			START_LOG( cerr, L"ģ�� ����Ʈ�� ������Ʈ �Ϸ��� �ϴµ� ���� ģ������Ʈ�� �ִ�?" )
				<< BUILD_LOG( nCID )
				<< BUILD_LOG( setFriendRev.size() )
				<< END_LOG;
		}
	}

	// �ڱ� �ڽ��� ������.
	setFriendRev.erase( nCID );

	std::set< UidType > setCurrentFriend;
	spFieldUser->GetFriend( setCurrentFriend );

	std::set< UidType >::iterator sitFriend = setFriendRev.begin();
	std::set< UidType >::iterator sitCurrentFriend = setCurrentFriend.begin();

	while( sitFriend != setFriendRev.end() || sitCurrentFriend != setCurrentFriend.end() )
	{
		if( sitFriend == setFriendRev.end() )
		{
			if( !spFieldUser->IsEventRecvUser( *sitCurrentFriend )  &&  !spFieldUser->IsParty( *sitCurrentFriend ) )
			{
				vecRemoved.push_back( *sitCurrentFriend );
			}

			sitCurrentFriend++;
			continue;
		}

		if( sitCurrentFriend == setCurrentFriend.end() )
		{
			if( !spFieldUser->IsEventRecvUser( *sitFriend )  &&  !spFieldUser->IsParty( *sitFriend ) )
			{
				KFieldUserPtr spPartyUser = GetUser( *sitFriend );
				if( !spPartyUser )
				{
					// [����] ������ ��ã�� ��찡 ���� : ��Ƽ�� �δ� ���� ���� �ٸ� �ʵ忡 ��Ƽ��û�ڿ� ��Ƽ���� ��ġ�ϰ������� ������ ��ã�´�!

					// �ٸ� �ʵ忡 �����Ƿ� �ʵ����������� �����ʿ䰡 ����, ���� �α״� cwarnó����.

					START_LOG( cwarn, L"���� ã�� ����" )
						<< BUILD_LOG( *sitFriend )
						<< END_LOG;

					sitFriend++;
					continue;
				}
				KFieldUserInfo kFieldUserInfo;
				spPartyUser->GetFieldUserInfo( kFieldUserInfo );
				vecAdded.push_back( kFieldUserInfo );
			}

			sitFriend++;
			continue;
		}

		if( *sitFriend < *sitCurrentFriend )
		{
			if( !spFieldUser->IsEventRecvUser( *sitFriend )  &&  !spFieldUser->IsParty( *sitFriend ) )
			{
				KFieldUserPtr spPartyUser = GetUser( *sitFriend );
				if( !spPartyUser )
				{
					// [����] ������ ��ã�� ��찡 ���� : ��Ƽ�� �δ� ���� ���� �ٸ� �ʵ忡 ��Ƽ��û�ڿ� ��Ƽ���� ��ġ�ϰ������� ������ ��ã�´�!

					// �ٸ� �ʵ忡 �����Ƿ� �ʵ����������� �����ʿ䰡 ����, ���� �α״� cwarnó����.

					START_LOG( cwarn, L"���� ã�� ����" )
						<< BUILD_LOG( *sitFriend )
						<< END_LOG;

					sitFriend++;
					continue;
				}
				KFieldUserInfo kFieldUserInfo;
				spPartyUser->GetFieldUserInfo( kFieldUserInfo );
				vecAdded.push_back( kFieldUserInfo );
			}

			sitFriend++;
			continue;
		}
		else if( *sitFriend > *sitCurrentFriend )
		{
			if( !spFieldUser->IsEventRecvUser( *sitCurrentFriend )  &&  !spFieldUser->IsParty( *sitCurrentFriend ) )
			{
				vecRemoved.push_back( *sitCurrentFriend );
			}

			sitCurrentFriend++;
			continue;
		}
		else
		{
			sitFriend++;
			sitCurrentFriend++;
			continue;
		}
	}

	spFieldUser->SetFriend( setFriendRev );
	return true;
}
//}}

//{{ 2009. 9. 23  ������	1������ĳ����
bool KFieldUserManager::SetFieldUserInfo( IN const KFieldUserInfo& kInfo )
{
	KFieldUserPtr spFieldUser = GetUser( kInfo.m_iUnitUID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"����� ���� ������ ������Ʈ �Ϸ��� �ϴµ� �ʵ������� ����?" )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	spFieldUser->SetFieldUserInfo( kInfo );
	return true;
}
//}}

//{{ 2010. 8. 3	������	�� �ý���
#ifdef SERV_PET_SYSTEM

bool KFieldUserManager::SetPetInfo( IN UidType nCID, IN const std::vector< KPetInfo >& vecPetInfo )
{
	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"���� ��ġ ������ ������Ʈ �Ϸ��� �ϴµ� �ʵ������� ����?" )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

	spFieldUser->SetPetInfo( vecPetInfo );
	return true;
}

bool KFieldUserManager::SetPetInfo( IN UidType nCID, IN const KPetInfo& kPetInfo )
{
	std::vector< KPetInfo > vecPetInfo;
	vecPetInfo.push_back( kPetInfo );
    return SetPetInfo( nCID, vecPetInfo );
}

#endif SERV_PET_SYSTEM
//}}

bool KFieldUserManager::UpdateUserPos( UidType nCID, const VECTOR3& UserPos )
{
	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"���� ��ġ ������ ������Ʈ �Ϸ��� �ϴµ� �ʵ������� ����?" )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

	spFieldUser->UpdateUserPos( UserPos );
	return true;
}

bool KFieldUserManager::AddUserToSector( SectorID iSectorID, KFieldUserPtr spUser )
{
    if( iSectorID == INVALID_SECTOR )
    {
        return true;
    }

    if( !spUser )
    {
        START_LOG( cerr, L"�ʵ� ���� ������ �̻�." )
            << BUILD_LOG( iSectorID )
            << END_LOG;

        return false;
    }

    UidType nCID = spUser->GetCID();
    if( nCID <= 0 )
    {
        START_LOG( cerr, L"ĳ���� UID�̻�." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    std::map< SectorID, FieldUserListPtr >::iterator mitSector = m_mapSectorUser.find( iSectorID );
    if( mitSector == m_mapSectorUser.end() )
    {
        // ���Ͱ� ������ �������
        FieldUserListPtr spFieldUserList( new std::map< UidType, KFieldUserPtr > ); _JIF( spFieldUserList, return false );
        spFieldUserList->insert( std::make_pair( nCID, spUser ) );
        m_mapSectorUser.insert( std::make_pair( iSectorID, spFieldUserList ) );
    }
    else
    {
        if( !mitSector->second )
        {
            START_LOG( cerr, L"���� ����Ʈ ������ �̻�." )
                << BUILD_LOG( iSectorID )
                << END_LOG;

            return false;
        }

        std::pair< std::map< UidType, KFieldUserPtr >::iterator, bool > pairResult;
        pairResult = mitSector->second->insert( std::make_pair( nCID, spUser ) );

        if( !pairResult.second )
        {
            START_LOG( cerr, L"�̹� �����ϴ� ������." )
                << BUILD_LOG( nCID )
                << END_LOG;

            return false;
        }
    }

    return true;
}

bool KFieldUserManager::RemoveUserFromSector( SectorID iSectorID, UidType nCID )
{
    if( iSectorID == INVALID_SECTOR )
    {
        // �Ҽ� ���Ͱ� ������
        return true;
    }

    std::map< SectorID, FieldUserListPtr >::iterator mitSector = m_mapSectorUser.find( iSectorID );
    if( mitSector == m_mapSectorUser.end() )
    {
        START_LOG( cerr, L"���� ���Ͱ� �ֳ�..?" )
            << BUILD_LOG( iSectorID )
            << END_LOG;
        return false;
    }

    if( !mitSector->second )
    {
        START_LOG( cerr, L"���� ����Ʈ �� ������ �̻�." )
            << BUILD_LOG( iSectorID )
            << END_LOG;

        return false;
    }

    // �ش� ���Ϳ��� ���� ����
    std::map< UidType, KFieldUserPtr >::iterator mitUser = mitSector->second->find( nCID );
    if( mitUser == mitSector->second->end() )
    {
        START_LOG( cerr, L"���� �˻� ����." )
            << BUILD_LOG( iSectorID )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    mitSector->second->erase( mitUser );
    return true;
}

void KFieldUserManager::GetSectorUnitList( const std::vector< SectorID >& vecSectorList, std::map< UidType, KFieldUserPtr >& mapUnitList )
{
    mapUnitList.clear();

    std::vector< SectorID >::const_iterator vit;
    for( vit = vecSectorList.begin(); vit != vecSectorList.end(); ++vit )
    {
        GetAddedSectorUnitList( *vit, mapUnitList );
	}
}

SectorsPtr KFieldUserManager::GetAdjacentSectors( SectorID iSector )
{
    std::map< SectorID, SectorsPtr >::iterator mit = m_mapAdjacentSector.find( iSector );
    if( mit == m_mapAdjacentSector.end() )
    {
        SectorsPtr spVector( new std::vector< SectorID > ); _JIF( spVector, return SectorsPtr() );
        KSectorID::ExtractAdjacentSector( iSector, m_kTileInfo.m_NumX, m_kTileInfo.m_NumY, m_kTileInfo.m_NumZ, *spVector );
        m_mapAdjacentSector.insert( std::make_pair( iSector, spVector ) );
        return spVector;
    }
    else
    {
        return mit->second;
    }
}

bool KFieldUserManager::GetBroadcastDestination( UidType nCID, std::vector< UidType >& vecDestination )
{
	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"���� ã�� ����" )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

	spFieldUser->GetBroadcastDestination( vecDestination );
	return true;
}

KFieldUserPtr KFieldUserManager::MakeUser( const KFieldUserInfo& kFieldUserInfo, const std::set< UidType >& setPartyList, const std::set< UidType >& setFriendList, const KFieldOption& kFieldOption )
{
	if( GetNumMember() >= GetNumMaxUser() )
	{
		START_LOG( cwarn, L"�ʵ� �Ѱ��ο��� �Ѿ��µ� �� �����ҷ��� �ϳ�.?��?" )
			<< BUILD_LOG( kFieldUserInfo.m_iUnitUID )
			<< BUILD_LOG( kFieldUserInfo.m_wstrNickName )
			<< END_LOG;

		return KFieldUserPtr();
	}

	KFieldUserPtr spFieldUser( new KFieldUser );
	if( !spFieldUser )
	{
		return KFieldUserPtr();
	}

	// 1. �ʵ��������� ���
	spFieldUser->SetFieldUserInfo( kFieldUserInfo );
	
	// 2. ��Ƽ������ ģ���������� �˻�
    std::set< UidType > setPartyRev = setPartyList;
	std::set< UidType > setFriendRev = setFriendList;

	// 3. ��Ƽ������ ģ������ üũ
	if( !setPartyRev.empty() )
	{
		if( setPartyRev.find( kFieldUserInfo.m_iUnitUID ) == setPartyRev.end() )
		{
			START_LOG( cerr, L"��Ƽ ����Ʈ�� ������Ʈ �Ϸ��� �ϴµ� ���� ��Ƽ����Ʈ�� ����?" )
				<< BUILD_LOG( kFieldUserInfo.m_iUnitUID )
				<< BUILD_LOG( setPartyRev.size() )
				<< END_LOG;
		}
	}

	if( !setFriendRev.empty() )
	{
		if( setFriendRev.find( kFieldUserInfo.m_iUnitUID ) != setFriendRev.end() )
		{
			START_LOG( cerr, L"ģ������Ʈ�� �� �ڽ��� ���ԵǾ��ֳ�?" )
				<< BUILD_LOG( kFieldUserInfo.m_iUnitUID )
				<< BUILD_LOG( setFriendRev.size() )
				<< END_LOG;
		}
	}

	// 4. �ڱ� �ڽ��� ������.
	setPartyRev.erase( kFieldUserInfo.m_iUnitUID );
	setFriendRev.erase( kFieldUserInfo.m_iUnitUID );
		
	// 5. ��Ƽ����Ʈ�� ģ������Ʈ ���
    spFieldUser->SetParty( setPartyRev );
	spFieldUser->SetFriend( setFriendRev );

	// 6. �ʵ� �ɼ� ���
    spFieldUser->SetFieldOption( kFieldOption );

	return spFieldUser;
}

void KFieldUserManager::GetAddedSectorUnitList( SectorID iSectorID, std::map< UidType, KFieldUserPtr >& mapUnitList  )
{
    std::map< SectorID, FieldUserListPtr >::iterator mitSector = m_mapSectorUser.find( iSectorID );
    if( mitSector == m_mapSectorUser.end() )
    {
		return;
	}

    if( !mitSector->second )
    {
        START_LOG( cerr, L"���� ����Ʈ ������ �̻�" )
            << BUILD_LOG( iSectorID )
            << END_LOG;

        return;
    }

    // Ȥ�� ���� �ȵ����� �� ���� insert �ϴ� ����� ������??
    std::map< UidType, KFieldUserPtr >::iterator mitUser;
    for( mitUser = mitSector->second->begin(); mitUser != mitSector->second->end(); mitUser++ )
    {
        std::pair< std::map< UidType, KFieldUserPtr >::iterator, bool > pairResult;
        pairResult = mapUnitList.insert( std::make_pair( mitUser->first, mitUser->second ) );
        LIF( pairResult.second );
    }
}

SectorID KFieldUserManager::GetDestSectorID( const VECTOR3& vPos )
{	
	int iIndexX = 0;
	if( m_kTileInfo.m_SizeX > 0 )
	{
		iIndexX = ( static_cast< short >( vPos.x ) - m_kMapInfo.m_MinX ) / m_kTileInfo.m_SizeX;
		if( iIndexX < 0 ) iIndexX = 0;
		else if( iIndexX >= m_kTileInfo.m_NumX ) iIndexX = m_kTileInfo.m_NumX - 1;
	}

	int iIndexY = 0;
	if( m_kTileInfo.m_SizeY > 0 ) 
	{
		iIndexY = ( static_cast< short >( vPos.y ) - m_kMapInfo.m_MinY ) / m_kTileInfo.m_SizeY;
		if( iIndexY < 0 ) iIndexY = 0;
		else if( iIndexY >= m_kTileInfo.m_NumY ) iIndexY = m_kTileInfo.m_NumY - 1;
	}

	int iIndexZ = 0;
	if( m_kTileInfo.m_SizeZ > 0 )
	{
		iIndexZ = ( static_cast< short >( vPos.z ) - m_kMapInfo.m_MinZ ) / m_kTileInfo.m_SizeZ;
		if( iIndexZ < 0 ) iIndexZ = 0;
		else if( iIndexZ >= m_kTileInfo.m_NumZ ) iIndexZ = m_kTileInfo.m_NumZ - 1;
	}
	
	SectorID iDestSectorID;
	KSectorID::SetIndex( iDestSectorID, iIndexX, iIndexY, iIndexZ );

	return iDestSectorID;
}

//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
void KFieldUserManager::RefreshUser_ED( std::map< UidType, int >& mapUnit_ED )
{
	std::map< UidType, int >::iterator mit;

	std::vector< KFieldUserPtr >::const_iterator vit;
	for( vit = m_vecUserList.begin(); vit != m_vecUserList.end(); ++vit )
	{
		KFieldUserPtr spCheckFieldUser = *vit;
		if( spCheckFieldUser == NULL )
		{
			START_LOG( cerr, L"�ʵ����� �����Ͱ� �̻��մϴ�." )
				<< BUILD_LOG( spCheckFieldUser->GetCID() )
				<< END_LOG;
			continue;
		}
		
		mit = mapUnit_ED.find( spCheckFieldUser->GetCID() );
		if( mit != mapUnit_ED.end() )
		{
			if( mit->second >= 0 )
				spCheckFieldUser->UpdateUserED( mit->second );
		}
	}
}
#endif SERV_ED_MONITORING_IN_GAME
//}}

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
bool KFieldUserManager::SetRidingPetInfo( IN const UidType iUnitUID, IN const UidType iRidingPetUID, IN const USHORT usRidingPetID )
{
	KFieldUserPtr spFieldUser = GetUser( iUnitUID );
	if( spFieldUser == NULL )
	{
		START_LOG( cerr, L"���� ���̵� �� ������ ������Ʈ �Ϸ��� �ϴµ� �ʵ� ������ ����?" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spFieldUser->SetRidingPetInfo( iRidingPetUID, usRidingPetID );
	return true;
}
#endif	// SERV_RIDING_PET_SYSTM