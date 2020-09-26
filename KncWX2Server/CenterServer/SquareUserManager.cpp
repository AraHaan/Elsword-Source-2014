#include ".\squareusermanager.h"

KSquareUserManager::KSquareUserManager(void)
:
m_dwMaxUser( 0 ),
m_bIsCheckZU( false )
{
}

KSquareUserManager::~KSquareUserManager(void)
{
	m_mapSquareUser.clear();
}

void KSquareUserManager::Init( int nMaxUser )
{
	m_dwMaxUser	= (DWORD)nMaxUser;
}

void KSquareUserManager::Reset()
{
	m_mapSquareUser.clear();
}

KSquareUserPtr KSquareUserManager::GetUser( UidType nCID )
{
	std::map< UidType, KSquareUserPtr >::iterator mit;
	mit = m_mapSquareUser.find( nCID );

	if( mit == m_mapSquareUser.end() )
	{
		START_LOG( cwarn, L"�������� �ʴ� ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return KSquareUserPtr();
	}

	return mit->second;
}

bool KSquareUserManager::AddUser( KSquareUserPtr spSquareUser )
{
	if( !spSquareUser )
	{
		START_LOG( cerr, L"���� ������ �̻�." )
			<< END_LOG;

		return false;
	}

	std::map< UidType, KSquareUserPtr >::iterator mit;
	mit = m_mapSquareUser.find( spSquareUser->GetCID() );

	if( mit != m_mapSquareUser.end() )
	{
		START_LOG( cerr, L"�̹� �����ϴ� ���� ������." )
			<< BUILD_LOG( spSquareUser->GetCID() )
			<< END_LOG;

		//m_mapSquareUser.erase( mit );
		return false;
	}

	m_mapSquareUser.insert( std::make_pair( spSquareUser->GetCID(), spSquareUser ) );
	return true;
}

bool KSquareUserManager::DeleteUser( UidType nCID )
{
	std::map< UidType, KSquareUserPtr >::iterator mit;
	mit = m_mapSquareUser.find( nCID );

	if( mit == m_mapSquareUser.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ���� ������." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return true;
	}

	m_mapSquareUser.erase( mit );
	return true;
}

int KSquareUserManager::DeleteUserByGsUID( IN UidType nGsUID, OUT std::vector<UidType>& vecDelUserUID )
{
	std::map<UidType, KSquareUserPtr>::iterator mit;
	for( mit = m_mapSquareUser.begin(); mit != m_mapSquareUser.end(); mit++ )
	{
		KSquareUserInfo kInfo;
		mit->second->GetSquareUserInfo( kInfo );

		if( kInfo.m_iGSUID == nGsUID )
		{
			//������ user�� uid�� ��´�.
			vecDelUserUID.push_back( kInfo.m_iUnitUID );
		}
	}

	return static_cast<int>(vecDelUserUID.size());
}

bool KSquareUserManager::EnterSquare( const KSquareUserInfo& kInfo )
{
	KSquareUserPtr spSquareUser = MakeUser( kInfo );
	if( !spSquareUser )
	{
		//{{ 2009. 1. 28  ������	�����α� �ٿ�����!
		if( m_mapSquareUser.size() == m_dwMaxUser )
		{
			START_LOG( cwarn, L"������ ������ ������ ���� ���� ����." )
				<< BUILD_LOG( ( int )m_mapSquareUser.size() )
				<< END_LOG;
		}
		else
		{
			START_LOG( cerr, L"���� ���� ���� ����." )
				<< BUILD_LOG( ( int )m_mapSquareUser.size() )
				<< END_LOG;
		}
		//}}

		return false;
	}

	if( AddUser( spSquareUser ) == false )
	{
		START_LOG( cerr, L"���� ���� �߰� ����." )
			<< BUILD_LOG( spSquareUser->GetCID() )
			<< END_LOG;

		return false;
	}

	return true;
}

bool KSquareUserManager::LeaveSquare( UidType nCID )
{
	//���࿡ �����Ͱ� ���������� ��츦 ������..��..
	if( GetPersonalShopState( nCID ) > CXSLSquareUnit::PSS_NONE )
	{
		KSquarePersonalShopInfo kInfo;
		
		kInfo.m_cUpdateType = KSquarePersonalShopInfo::UT_DATA_CLEAR;
		//kInfo.m_cPersonalShopState = CXSLSquareUnit::PSS_NONE;
		SetPersonalShopData( nCID, kInfo );
	}

	return DeleteUser( nCID );
}

bool KSquareUserManager::SetSyncPos( UidType nCID, VECTOR3& vSyncPos )
{
	KSquareUserPtr spSquareUser = GetUser( nCID );
	if( !spSquareUser )
	{
		START_LOG( cwarn, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spSquareUser->SetSyncPos( vSyncPos );

    return true;
}

void KSquareUserManager::GetUserList( std::map< UidType, std::set< UidType > >& mapUidList_ )
{
    mapUidList_.clear();

    std::map< UidType, KSquareUserPtr >::iterator mit;
    for( mit = m_mapSquareUser.begin(); mit != m_mapSquareUser.end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"�� ���� ������ �̻�." )
                << BUILD_LOG( mit->first )
                << END_LOG;

            continue;
        }

        // operator[]()�� �ǵ����� ���. �����Ͱ� ���� ��� insert�Ǵ� ���� �����Ѵ�.
        mapUidList_[mit->second->GetGSUID()].insert( mit->second->GetCID() );
    }
}

void KSquareUserManager::GetUserInfoList( std::vector<KSquareUserInfo>& vecUserInfo )
{
	KSquareUserInfo kInfo;

	std::map< UidType, KSquareUserPtr >::iterator mit;
	for( mit = m_mapSquareUser.begin(); mit != m_mapSquareUser.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"SQUARE USER ������ �̻�.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			continue;
		}

		mit->second->GetSquareUserInfo( kInfo );
		vecUserInfo.push_back( kInfo );
	}
}

bool KSquareUserManager::GetSortSyncPos( UidType nCID, std::vector<KSquareUserPtr>& vecUserList )
{
	std::map< UidType, KSquareUserPtr >::iterator mit;
	std::vector<KSquareUserPtr> vecSquareUser;

	//map�� �ִ� ����Ʈ�� vector�� ��´�.
	for( mit = m_mapSquareUser.begin(); mit != m_mapSquareUser.end(); ++mit )
	{
		vecSquareUser.push_back( mit->second );
	}

	KSquareUserPtr spMyUser = GetUser( nCID );
	if( spMyUser == NULL )
	{
		START_LOG( cerr, L"SQUARE USER ������ �̻�.!" )
			<< BUILD_LOG( mit->first )
			<< END_LOG;

		return false;
	}

	//�ڽ��� ��ġ�� �������� ����� ������ �����Ѵ�.
	KSquareUserPtr spSquareUser;
	for( int nCnt1 = 0; nCnt1 < (int)vecSquareUser.size(); ++nCnt1 )
	{
		bool bSwapped = false;

		for( int nCnt2 = 0; nCnt2 < (int)vecSquareUser.size() - nCnt1 - 1; ++nCnt2 )
		{
			if( GetDistance( spMyUser->GetSyncPos(), vecSquareUser[nCnt2]->GetSyncPos() ) > GetDistance( spMyUser->GetSyncPos(), vecSquareUser[nCnt2+1]->GetSyncPos() ) )
			{
				spSquareUser			= vecSquareUser[nCnt2];
				vecSquareUser[nCnt2]	= vecSquareUser[nCnt2+1];
				vecSquareUser[nCnt2+1]	= spSquareUser;
				bSwapped = true;
			}
		}

		if( bSwapped == false )
		{
			break;
		}
	}

	//�Ÿ� 1500 ����
	//�ִ��ο� 20��
	//�������� ����Ʈ�� ��� �Ѱ��ش�.
	vecUserList.clear();
	for( int i = 0; i < (int)vecSquareUser.size(); ++i )
	{
		if( GetDistance( spMyUser->GetSyncPos(), vecSquareUser[i]->GetSyncPos() ) <= 1500.0f &&
			(int)vecUserList.size() <= 20 )
		{
			vecUserList.push_back( vecSquareUser[i] );
		}
		else
		{
			break;
		}
	}

	return true;
}

bool KSquareUserManager::GetSquareUserGS( UidType nCID, UidType& nGSUID )
{
	std::map< UidType, KSquareUserPtr >::iterator mit;

	mit = m_mapSquareUser.find( nCID );

	if( mit != m_mapSquareUser.end() )
	{
		if( mit->second != NULL )
		{
			nGSUID = mit->second->GetGSUID();

			return true;
		}
		else
		{
			START_LOG( cerr, L"���� ���� ������ �̻�.!" )
				<< BUILD_LOG( nCID )
				<< END_LOG
				;
		}
	}
	else
	{
		START_LOG( cwarn, L"ã�� ���������� ����.!" )
			<< BUILD_LOG( nCID )
			<< END_LOG
			;
	}

	return false;
}


KSquareUserPtr KSquareUserManager::MakeUser( const KSquareUserInfo& kInfo )
{
	if( m_mapSquareUser.size() >= m_dwMaxUser )
	{
		START_LOG( cwarn, L"���� �Ѱ��ο��� �Ѿ��µ� �� �����ҷ��� �ϳ�.?��?" )
			<< BUILD_LOG( kInfo.m_iGSUID )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< BUILD_LOG( kInfo.m_wstrNickName )
			<< END_LOG;

		return KSquareUserPtr();
	}

	KSquareUserPtr spSquareUser( new KSquareUser );
	if( !spSquareUser )
	{
		return KSquareUserPtr();
	}

	spSquareUser->SetSquareUserInfo( kInfo );
	return spSquareUser;
}

//{{ 2009. 9. 23  ������	1������ĳ����
bool KSquareUserManager::SetSquareUserInfo( IN const KSquareUserInfo& kInfo )
{
	KSquareUserPtr spSquareUser = GetUser( kInfo.m_iUnitUID );
	if( !spSquareUser )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	spSquareUser->SetSquareUserInfo( kInfo );
	return true;
}
//}}

void KSquareUserManager::SetUserEquippedTitle( IN UidType nCID, IN short sEquippedTitleID )
{
	KSquareUserPtr spSquareUser = GetUser( nCID );

	if( !spSquareUser )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return;
	}

	spSquareUser->SetUserEquippedTitle( sEquippedTitleID );
}

void KSquareUserManager::SetUserEquipItem( IN UidType nCID, IN std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
	KSquareUserPtr spSquareUser = GetUser( nCID );

	if( !spSquareUser )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return;
	}

	spSquareUser->SetUserEquipItem( vecInventorySlotInfo );
}

bool KSquareUserManager::SetUnitPartyInfo( UidType nCID, bool bIsParty )
{
	KSquareUserPtr spSquareUser = GetUser( nCID );

	if( !spSquareUser )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spSquareUser->SetUnitPartyInfo( bIsParty );

	return true;
}

bool KSquareUserManager::IsCreatePersonalShop()
{
	int iCountPS = 0;

	std::map< UidType, KSquareUserPtr >::iterator mit;
	for( mit = m_mapSquareUser.begin(); mit != m_mapSquareUser.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"SQUARE USER ������ �̻�.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			continue;
		}

		if( mit->second->GetPersonalShopState() > CXSLSquareUnit::PSS_NONE )
			++iCountPS;
	}

    if( iCountPS >= static_cast<int>(GetNumMaxUser()/2) )
	//if( iCountPS >= 33 ) //08.02.12 hoons.���� ������ ��� �ο��� ������� �ʾ� �ӽ÷� 50%�� ��´�.
		return false;

	return true;
}

char KSquareUserManager::GetPersonalShopState( UidType nCID )
{
	KSquareUserPtr spSquareUser = GetUser( nCID );

	if( !spSquareUser )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return CXSLSquareUnit::PSS_NONE;
	}

	return spSquareUser->GetPersonalShopState();
}

UidType KSquareUserManager::GetPersonalShopUID( UidType nCID )
{
	KSquareUserPtr spSquareUser = GetUser( nCID );

	if( !spSquareUser )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return 0;
	}

	return spSquareUser->GetPersonalShopUID();
}

bool KSquareUserManager::SetPersonalShopData( IN UidType nCID, IN const KSquarePersonalShopInfo& kInfo, OUT KEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT& kNot )
{
	KSquareUserPtr spSquareUser = GetUser( nCID );

	if( !spSquareUser )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spSquareUser->SetPersonalShopData( kInfo, kNot );

	return true;
}

//{{ 2010. 8. 2	������	���� ��ũ ����ȭ
#ifdef SERV_SQUARE_SYNC

bool KSquareUserManager::CompareSquareSyncPacket( IN UidType nCID, IN const KEGS_SQUARE_UNIT_SYNC_DATA_REQ& kInfo )
{
	KSquareUserPtr spSquareUser = GetUser( nCID );
	if( spSquareUser == NULL )
	{
		START_LOG( cerr, L"������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	return spSquareUser->CompareSquareSyncPacket( kInfo );
}

#endif SERV_SQUARE_SYNC
//}}

void KSquareUserManager::ZU_CheckStart()
{
	START_LOG( clog, L"���� �������� üũ����.!" );

	//�������� üũ �����ϱ��� �ð� �ʱ�ȭ
	//std::map< UidType, KSquareUserPtr >::iterator mit;
	//for( mit = m_mapSquareUser.begin(); mit != m_mapSquareUser.end(); mit++ )
	//{
	//	if( !mit->second )
	//	{
	//		START_LOG( cerr, L"���� ���� ������ �̻�." )
	//			<< BUILD_LOG( mit->first )
	//			<< END_LOG;
	//		continue;
	//	}

	//	mit->second->ZU_Refresh();
	//}

	//�������� üũ�� ����
	m_bIsCheckZU = true;
}

void KSquareUserManager::ZU_CheckEnd()
{
	START_LOG( clog, L"���� �������� üũ����.!" );

	//�������� üũ ����.
	m_bIsCheckZU = false;
}

bool KSquareUserManager::ZU_Tick( std::vector<UidType>& vecZombieUnit )
{
	if( m_bIsCheckZU == false )
	{
		return false;
	}
	//else
	//{
	//	//������ ������ üũ�� ���ش�.
	//	if( m_mapSquareUser.empty() == true )
	//	{
	//		m_bIsCheckZU = false;
	//		return false;
	//	}
	//}

	vecZombieUnit.clear();

	std::map< UidType, KSquareUserPtr >::iterator mit;
	for( mit = m_mapSquareUser.begin(); mit != m_mapSquareUser.end(); mit++ )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"���� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			continue;
		}

		if( mit->second->ZU_Tick() == true )
		{
			vecZombieUnit.push_back( mit->second->GetCID() );
		}
	}

	//���������� ���ٸ�..
	if( vecZombieUnit.empty() == true )
		return false;

	return true;
}

void KSquareUserManager::ZU_Refresh( UidType nCID, u_short usEventID )
{
	if( m_bIsCheckZU == false )
		return;

	//������ �濡 ���µ� ���� ��Ŷ�� ��� �̰��� �߰��Ѵ�.
	switch( usEventID )
	{
	case ERM_OPEN_SQUARE_NOT:
	case ERM_JOIN_SQUARE_REQ:
	case ERM_CHECK_INVALID_USER_NOT:
	case ERM_SQUARE_LIST_INFO_NOT:
		return;
	}

	START_LOG( clog, L"�������� Refresh.!" )
		<< BUILD_LOG( nCID );

	//�̺�Ʈ�� ���ƿ� ������ ã�´�.
	std::map< UidType, KSquareUserPtr >::iterator mit;
	mit = m_mapSquareUser.find( nCID );

	if( mit != m_mapSquareUser.end() )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�������� ������ �̻�.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			return;
		}

		//�̻������ üũ �ð��� �����Ѵ�.
		mit->second->ZU_Refresh();
	}
	else
	{
		START_LOG( cerr, L"�ð��� ������ ������ ����.!" )
			<< BUILD_LOG( nCID )
			<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
			<< END_LOG;
	}
}

//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
int KSquareUserManager::GetUserED( IN UidType nCID )
{
	std::map< UidType, KSquareUserPtr >::iterator mit = m_mapSquareUser.find( nCID );
	if( mit == m_mapSquareUser.end() )
	{
		return 0;
	}

	if( !mit->second )
	{
		START_LOG( cerr, L"�������� ������ �̻�.!" )
			<< BUILD_LOG( mit->first )
			<< END_LOG;

		return 0;
	}

    return mit->second->GetED();
}
#endif SERV_ED_MONITORING_IN_GAME
//}}




