#include ".\PVPLobbyManager.h"

#include "NetError.h"

ImplementSingleton( KPVPLobbyManager );

KPVPLobbyManager::KPVPLobbyManager(void)
{	
}

KPVPLobbyManager::~KPVPLobbyManager(void)
{
}

ImplToStringW( KPVPLobbyManager )
{
	return stm_;
}

bool KPVPLobbyManager::AddPVPUser( IN KCommunityUserInfo& kInfo_, IN UidType nGSUID )
{
	// ���������� �ִ��� �˻�
	if( IsExist( kInfo_.m_iUnitUID ) )
	{
		START_LOG( cerr, L"pvp������ �߰��Ϸ��µ� �̹� ������ �ִ�!? ���������ΰ�!" )
			<< BUILD_LOG( kInfo_.m_iUnitUID )
			<< END_LOG;

		// ����ó��!
		RemovePVPUser( kInfo_.m_iUnitUID );
	}

	// ���� ���¸� �κ�� ����!
	kInfo_.m_cState = static_cast<char>( KCommunityUserInfo::US_PVP_LOBBY );

	// pvp user ����
	KPVPUserPtr spAddUser = KPVPUserPtr( new KPVPUser( kInfo_, nGSUID ) );
	if( spAddUser == NULL )
	{
		START_LOG( cerr, L"pvp user ��������!?" )
			<< BUILD_LOG( kInfo_.m_iUnitUID )
			<< BUILD_LOG( nGSUID )
			<< END_LOG;

		return false;
	}

	// map�� �߰�
	std::pair< std::map< UidType, KPVPUserPtr >::iterator, bool > pairResult;
	pairResult = m_mapPVPUserList.insert( std::make_pair( kInfo_.m_iUnitUID, spAddUser ) );

	if( !pairResult.second )
	{
		START_LOG( cerr, L"pvp user ��� ����!? �Ͼ�����¿���!" )
			<< BUILD_LOG( kInfo_.m_iUnitUID )
			<< BUILD_LOG( nGSUID )
			<< END_LOG;

		return false;
	}
	
	// vector�� �߰�
	m_vecPVPUserList.push_back( spAddUser );

	return true;
}

bool KPVPLobbyManager::RemovePVPUser( IN UidType iUnitUID )
{
	KPVPUserPtr spPVPUser = Get( iUnitUID );
	if( spPVPUser == NULL )
	{
		START_LOG( cerr, L"pvp������ �����Ϸ��µ� ������ ����!?" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		// ������������..
		return true;
	}

	// vector���� ������!
	std::vector< KPVPUserPtr >::iterator vitRemove;
	for( vitRemove = m_vecPVPUserList.begin(); vitRemove != m_vecPVPUserList.end(); ++vitRemove )
	{
		if( *vitRemove == NULL )
		{
            START_LOG( cerr, L"������ �����Ϸ��µ� null���̴�?!" )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		// ã�Ҵ�!
        if( (*vitRemove)->m_kUserInfo.m_iUnitUID == iUnitUID )
			break;
	}

	if( vitRemove != m_vecPVPUserList.end() )
	{
        m_vecPVPUserList.erase( vitRemove );
	}
	else
	{
		START_LOG( cerr, L"vector���� ������ ��ã�Ҵ�? map���� �ִµ�?" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
	}

	// map���� ������!
	m_mapPVPUserList.erase( iUnitUID );
	return true;
}

bool KPVPLobbyManager::UpdatePVPUserInfo( IN UidType iUnitUID, IN char cState, IN int iRoomListID, IN UidType iRoomUID )
{
	KPVPUserPtr spPVPUser = Get( iUnitUID );
	if( spPVPUser == NULL )
	{
		// �ð����� �߻�����
		START_LOG( cwarn, L"pvp���������� �����Ϸ��µ� ������ ����!?" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOGc( cState )
			<< END_LOG;
		
		return false;
	}

	// ���� ���� ����
	spPVPUser->m_kUserInfo.m_cState		= cState;
	if( iRoomListID > 0 )
	{
		// ��ȿ�� RoomListID�� ������Ʈ�Ѵ�.
		spPVPUser->m_kUserInfo.m_iStateCode = iRoomListID;
	}
	spPVPUser->m_kUserInfo.m_iRoomUID	= iRoomUID;

	return true;
}

bool KPVPLobbyManager::GetPVPUserList( IN UINT& uiViewPage, IN UINT& uiTotalPage, OUT std::vector< KCommunityUserInfo >& vecUserList )
{
	vecUserList.clear();

	_JIF( uiViewPage > 0, return false; );

	if( m_vecPVPUserList.empty() )
	{
		uiViewPage = 1;
		uiTotalPage = 1;
		return true;
	}

	uiTotalPage = m_vecPVPUserList.size() / PAGE_PER_PVP_USER;
	if( ( m_vecPVPUserList.size() % PAGE_PER_PVP_USER ) > 0 )
	{
		++uiTotalPage;
	}

	u_int uiBeginIndex = ( uiViewPage * PAGE_PER_PVP_USER ) - PAGE_PER_PVP_USER;
	if( m_vecPVPUserList.size() <= uiBeginIndex )
	{
		uiViewPage = uiTotalPage; // ������ ������
		uiBeginIndex = ( uiViewPage * PAGE_PER_PVP_USER ) - PAGE_PER_PVP_USER;

		if( m_vecPVPUserList.size() <= uiBeginIndex )
		{
			START_LOG( cerr, L"�Ͼ �� ���� ����!" )
				<< BUILD_LOG( uiViewPage )
				<< BUILD_LOG( uiTotalPage )
				<< BUILD_LOG( uiBeginIndex )
				<< BUILD_LOG( m_vecPVPUserList.size() )
				<< END_LOG;
			return false;
		}
	}

	std::vector< KPVPUserPtr >::reverse_iterator vit = m_vecPVPUserList.rbegin();
	vit += uiBeginIndex;

	while( vit != m_vecPVPUserList.rend() )
	{
		KPVPUserPtr spPVPUser = *vit;
		if( spPVPUser == NULL )
		{
			START_LOG( cerr, L"pvp user �����Ͱ� �̻��մϴ�." )
				<< END_LOG;
		}
		else
		{
			vecUserList.push_back( spPVPUser->m_kUserInfo );

			if( vecUserList.size() >= PAGE_PER_PVP_USER )
				break;
		}

		++vit;
	}

	return true;
}

void KPVPLobbyManager::RemovePVPUserDisconnectGS( IN UidType nGSUID )
{
	std::vector< UidType >::const_iterator vitRemove;
	std::vector< UidType > vecRemoveUserList;

	// ������ ������ ���Ӽ����� �������� ã�´�
	std::vector< KPVPUserPtr >::const_iterator vit;
	for( vit = m_vecPVPUserList.begin(); vit != m_vecPVPUserList.end(); ++vit )
	{
		if( *vit == NULL )
		{
			START_LOG( cerr, L"pvp user �����Ͱ� �̻��ϴ�!?" )
				<< BUILD_LOG( nGSUID )
				<< END_LOG;
			continue;
		}

		if( (*vit)->m_nGSUID == nGSUID )
		{
			vecRemoveUserList.push_back( (*vit)->m_kUserInfo.m_iUnitUID );
		}
	}

	// ã�� �������� ����Ʈ���� �����Ѵ�!
	for( vitRemove = vecRemoveUserList.begin(); vitRemove != vecRemoveUserList.end(); ++vitRemove )
	{
		if( !RemovePVPUser( *vitRemove ) )
		{
			START_LOG( cerr, L"���� ���� ����!" )
				<< BUILD_LOG( *vitRemove )
				<< END_LOG;
		}
	}
}

KPVPUserPtr KPVPLobbyManager::Get( IN UidType iUnitUID )
{
	std::map< UidType, KPVPUserPtr >::iterator mit;
	mit = m_mapPVPUserList.find( iUnitUID );
	if( mit == m_mapPVPUserList.end() )
	{
		START_LOG( cwarn, L"pvp user�� ã���� �����ϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KPVPUserPtr();
	}

	return mit->second;
}



