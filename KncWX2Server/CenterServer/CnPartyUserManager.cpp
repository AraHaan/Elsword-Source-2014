#include "CnPartyUserManager.h"
#include "NetError.h"
#include "X2Data/XSLPartyManager.h"


const float KCnPartyUserManager::m_scfCandidateTimeLimit = 7.f;

KCnPartyUserManager::KCnPartyUserManager()
{
	Reset();
}

KCnPartyUserManager::~KCnPartyUserManager()
{
}

void KCnPartyUserManager::Reset()
{
    SetMaxMember( 0 );
	m_mapMember.clear();
	m_mapCandidate.clear();
}

//{{ 2012. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnPartyUserManager::Tick( OUT std::vector< UidType >& vecDeleteCadidateUser )
{
	vecDeleteCadidateUser.clear();

	// ��û ����� ����Ʈ üũ
	if( m_mapCandidate.empty() )
		return;

	std::vector< UidType > vecLeaveList;
	std::map< UidType, KWaitingUserInfo >::iterator mit;
	for( mit = m_mapCandidate.begin(); mit != m_mapCandidate.end(); ++mit )
	{
		const KWaitingUserInfo& kWaitingUserInfo = mit->second;

		// �ʴ� ���� ������ �ǻ� ������ ���� �ʰ� 7�ʰ� ����.
		if( kWaitingUserInfo.m_kTimer.elapsed() <= PARTY_INVITE_TIME_LIMIT )
			continue;

		if( IS_NULL( kWaitingUserInfo.m_spUser ) )
			continue;

		if( kWaitingUserInfo.m_bInvited )
		{	
			// �ʴ����� ���� �������� �ʴ밡 �����Ǿ��ٰ� �˷���.
			KCnPartyUserPtr spSendInviteUser = GetPartyUser( kWaitingUserInfo.m_iSendInviteUnitUID );
			if( IS_NOT_NULL( spSendInviteUser ) )
			{
				KEGS_INVITE_PARTY_DENY_NOT kDenyNot;
				kDenyNot.m_iDenyReasonNetError = NetError::ERR_PARTY_19;
				kDenyNot.m_iUnitUID = kWaitingUserInfo.m_spUser->GetCharUID();
				spSendInviteUser->SendToGSCharacter( EPM_INVITE_PARTY_DENY_NOT, kDenyNot );
			}
			//else -- ���� ��Ƽ�忡�Ա��� �����ʿ䰡 ������? �ϴ� �ּ�ó��
			//{
			//	// �ð��������� �ʴ����� ���� ������ ���ٸ� ��Ƽ�忡�� ������
			//	KCnPartyUserPtr spHost = GetHostPtr();
			//	if( IS_NOT_NULL( spHost ) )
			//	{
			//		KEGS_INVITE_PARTY_DENY_NOT kDenyNot;
			//		kDenyNot.m_iDenyReasonNetError = NetError::ERR_PARTY_19;
			//		kDenyNot.m_iUnitUID = kWaitingUserInfo.m_spUser->GetCharUID();
			//		spHost->SendToGSCharacter( EPM_INVITE_PARTY_DENY_NOT, kDenyNot );
			//	}
			//	else
			//	{
			//		START_LOG( cerr, L"��Ƽ�� �����Ͱ� �߸��Ǿ���.!" )
			//			<< END_LOG;
			//	}
			//}
		}
		else
		{
			// ��Ƽ ���� ��û ���� ��Ƽ���� �ǻ� ���� ���� �ʰ� 10�ʰ� ����.
			//## ����ڿ��� ��Ƽ�����Ǿ��ٰ� �˷���.
			if( mit->second.m_spUser != NULL )
			{
				KEGS_JOIN_PARTY_NOT kNot;
				kNot.m_iOK = NetError::ERR_PARTY_18;
				mit->second.m_spUser->SendToGSCharacter( EPM_JOIN_PARTY_NOT, kNot );
			}
			else
			{
				START_LOG( cerr, L"����� ���ð��� 10�ʰ� �Ѿ��µ� �����Ͱ� �߸��Ǿ���." )
					<< END_LOG;
			}
		}

		//ó���� ����ڿ��� �����ش�.
		vecDeleteCadidateUser.push_back( kWaitingUserInfo.m_spUser->GetCharUID() );
	}
}
#else
void KCnPartyUserManager::Tick()
{
	// ��û ����� ����Ʈ üũ
	if( m_mapCandidate.empty() )
		return;

	std::vector< UidType > vecLeaveList;
	std::map< UidType, KWaitingUserInfo >::iterator mit;
	for( mit = m_mapCandidate.begin(); mit != m_mapCandidate.end(); ++mit )
	{
		const KWaitingUserInfo& kWaitingUserInfo = mit->second;

		if( kWaitingUserInfo.m_kTimer.elapsed() <= m_scfCandidateTimeLimit )
			continue;

		if( kWaitingUserInfo.m_bInvited )
		{
			// �ʴ� ���� ������ �ǻ� ������ ���� �ʰ� 10�ʰ� ����.
			//## ��Ƽ�忡�� �ʴ밡 �����Ǿ��ٰ� �˷���.

			KCnPartyUserPtr spHost = GetHostPtr();
			if( spHost != NULL )
			{
				KEGS_INVITE_PARTY_DENY_NOT kDenyNot;
				kDenyNot.m_iDenyReasonNetError = NetError::ERR_PARTY_19;
				kDenyNot.m_iUnitUID = kWaitingUserInfo.m_spUser->GetCharUID();
				spHost->SendToGSCharacter( EPM_INVITE_PARTY_DENY_NOT, kDenyNot );
			}
			else
			{
				START_LOG( cerr, L"��Ƽ�� �����Ͱ� �߸��Ǿ���.!" )
					<< END_LOG;
			}
		}
		else
		{
			// ��Ƽ ���� ��û ���� ��Ƽ���� �ǻ� ���� ���� �ʰ� 10�ʰ� ����.
			//## ����ڿ��� ��Ƽ�����Ǿ��ٰ� �˷���.
			if( mit->second.m_spUser != NULL )
			{
				KEGS_JOIN_PARTY_NOT kNot;
				kNot.m_iOK = NetError::ERR_PARTY_18;
				mit->second.m_spUser->SendToGSCharacter( EPM_JOIN_PARTY_NOT, kNot );
			}
			else
			{
				START_LOG( cerr, L"����� ���ð��� 10�ʰ� �Ѿ��µ� �����Ͱ� �߸��Ǿ���." )
					<< END_LOG;
			}
		}

		//ó���� ����ڿ��� �����ش�.
		mit = m_mapCandidate.erase( mit );
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

bool KCnPartyUserManager::GetUserInfo( IN UidType nCID, OUT KPartyUserInfo& kInfo )
{
	KCnPartyUserPtr spPartyUser = GetPartyUser( nCID );
	if( spPartyUser == NULL )
	{
		START_LOG( cerr, L"������ ��û�� ������ ����." )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

	spPartyUser->GetPartyUserInfo( kInfo );
	kInfo.m_bHost	= spPartyUser->IsHost();
	kInfo.m_bReady	= spPartyUser->IsReady();
	return true;
}

void KCnPartyUserManager::GetUserInfoList( OUT std::vector< KPartyUserInfo >& kInfoList )
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"���������� �̻�.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		KPartyUserInfo kInfo;
		mit->second->GetPartyUserInfo( kInfo );
		kInfo.m_bHost	= mit->second->IsHost();
		kInfo.m_bReady	= mit->second->IsReady();
		kInfoList.push_back( kInfo );
	}
}

bool KCnPartyUserManager::GetDungeonRoomUserInfoList( OUT std::vector< KPartyRoomUserInfo >& vecInfoList )
{
	vecInfoList.clear();

	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"���������� �̻�.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		KPartyRoomUserInfo kInfo;
		if( mit->second->GetPartyRoomUserInfo( kInfo ) == false )
		{
			return false;
		}

		vecInfoList.push_back( kInfo );
	}

	return true;
}

void KCnPartyUserManager::GetListUserInfoList( OUT std::vector< KPartyListUserInfo >& vecUserInfo )
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;

	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"���������� �̻�.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		KPartyListUserInfo kInfo;
		kInfo.m_iUnitUID		= mit->second->GetCharUID();
		kInfo.m_wstrNickName	= mit->second->GetCharName();
		kInfo.m_cUnitClass		= mit->second->GetUnitClass();
		kInfo.m_ucLevel			= mit->second->GetLevel();
		kInfo.m_bHost			= mit->second->IsHost();
		vecUserInfo.push_back( kInfo );
	}
}

//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
void KCnPartyUserManager::GetMatchUserInfoList( IN const UidType iPartyUID, OUT std::vector< KMatchUserInfo >& vecUserList )
{
	vecUserList.clear();

	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"���������� �̻�.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;			
		}

		// ��ġ ���� ���� ������!
		KMatchUserInfo kUserInfo;
		spPartyUser->GetMatchUserInfo( iPartyUID, kUserInfo );
		vecUserList.push_back( kUserInfo );
	}
}
#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2012. 02. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnPartyUserManager::GetAutoPartyUserInfoList( IN const KOldPartyInfo& kOldPartyInfo, OUT std::vector< KAutoPartyUserInfo >& vecUserList )
{
	vecUserList.clear();

	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"���������� �̻�.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;			
		}

		// ��ġ ���� ���� ������!
		KAutoPartyUserInfo kUserInfo;
		spPartyUser->GetAutoPartyUserInfo( kUserInfo );
		kUserInfo.m_kOldPartyInfo = kOldPartyInfo;
		vecUserList.push_back( kUserInfo );
	}
}

bool KCnPartyUserManager::GetGSUID( IN const UidType nCID, OUT UidType& iGSUID )
{
	iGSUID = 0;

	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	mit = m_mapMember.find( nCID );
	if( mit == m_mapMember.end() )
	{
		START_LOG( cerr, L"��Ƽ ��� ������ ã�� �� �����ϴ�." )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

	if( IS_NULL( mit->second ) )
	{
		START_LOG( cerr, L"��Ƽ ��� �����Ͱ� �̻��մϴ�." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	iGSUID = mit->second->GetGSUID();
	return true;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

void KCnPartyUserManager::GetUnitUIDList( OUT std::set< UidType >& setUidList ) const
{
	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"���������� �̻�.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;			
		}

		setUidList.insert( mit->second->GetCharUID() );
	}
}

void KCnPartyUserManager::GetUnitUIDList( OUT std::vector< UidType >& vecUidList ) const
{
	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"���������� �̻�.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		vecUidList.push_back( mit->second->GetCharUID() );
	}
}

void KCnPartyUserManager::GetUnitUIDListByGSUID( IN UidType iGSUID, OUT std::vector< UidType >& vecUserList ) const
{
	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->GetGSUID() != iGSUID )
			continue;

		vecUserList.push_back( mit->second->GetCharUID() );
	}
}

void KCnPartyUserManager::ClearDungeonRoomUserInfoList()
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"���������� �̻�.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		mit->second->ClearPartyRoomUserInfo();
	}
}

bool KCnPartyUserManager::IsAllCheckedGameStart()
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"���������� �̻�.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->IsExistPartyRoomUserInfo() == false )
		{
			return false;
		}
	}

	return true;
}



KCnPartyUserPtr KCnPartyUserManager::GetHostPtr() const
{
	//{{ 2012. 12. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_CODE_CLEANUP_2012_12_07
	// [2012-12-07][������] ��Ƽ���� �ƿ� ������ ȣ��Ǵ� ��쵵 �ִ°� ����. �̰�쿡 ���� ����ó��.
	if( m_mapMember.empty() == true )
	{
		START_LOG( cerr, L"��Ƽ ����� �Ѹ� ���µ� �� �Լ��� ȣ��Ǿ���!" )
			<< BUILD_LOG( m_mapMember.size() )
			<< END_LOG;

		return KCnPartyUserPtr();
	}
#endif SERV_CODE_CLEANUP_2012_12_07
	//}}

	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->IsHost() == true )
		{
			return mit->second;
		}
	}

	START_LOG( cerr, L"��Ƽ���� ����.? �Ф��� ���� �� ����� ���� ������~~" )
		<< END_LOG;

	return KCnPartyUserPtr();
}

UidType KCnPartyUserManager::GetHostUID() const
{
	KCnPartyUserPtr spHost = GetHostPtr();
	if( spHost == NULL )
	{
		START_LOG( cerr, L"��Ƽ���� ����." )
			<< END_LOG;

		return 0;
	}

	return spHost->GetCharUID();
}

KCnPartyUserPtr KCnPartyUserManager::GetPartyUser( IN const UidType nCID )
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	mit = m_mapMember.find( nCID );
	if( mit == m_mapMember.end() )
	{
		START_LOG( cwarn, L"��Ƽ ��� ������ ã�� �� �����ϴ�." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return KCnPartyUserPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"��Ƽ ��� �����Ͱ� �̻��մϴ�." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return KCnPartyUserPtr();
	}

	return mit->second;
}

KCnPartyUserPtr KCnPartyUserManager::GetCandidateUser( IN const UidType nCID )
{
	std::map< UidType, KWaitingUserInfo >::iterator mit;
	mit = m_mapCandidate.find( nCID );
	if( mit == m_mapCandidate.end() )
	{
		START_LOG( cwarn, L"��Ƽ ����� ������ ã�� �� �����ϴ�." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return KCnPartyUserPtr();
	}

	if( mit->second.m_spUser == NULL )
	{
		START_LOG( cerr, L"��Ƽ ����� �����Ͱ� �̻��մϴ�." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return KCnPartyUserPtr();
	}

	return mit->second.m_spUser;
}

bool KCnPartyUserManager::AddCandidate( IN const KPartyUserInfo& kUserInfo, IN const bool bInvited, IN const UidType iSendInviteUnitUID )
{ 
	if( kUserInfo.m_iUnitUID <= 0 )
	{
		START_LOG( cerr, L"ĳ���� UID �̻�" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	if( IsCandidate( kUserInfo.m_iUnitUID ) == true )
	{
		START_LOG( cerr, L"�̹� �ĺ� ȸ����" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	KWaitingUserInfo kWaitUser;
	kWaitUser.m_spUser		= KCnPartyUserPtr( new KCnPartyUser( kUserInfo ) );
	kWaitUser.m_bInvited	= bInvited;
	//{{ 2012. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	kWaitUser.m_iSendInviteUnitUID = iSendInviteUnitUID;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	kWaitUser.m_kTimer.restart();

	m_mapCandidate.insert( std::make_pair( kUserInfo.m_iUnitUID, kWaitUser ) );
	return true;
}

bool KCnPartyUserManager::AddMember( IN const KPartyUserInfo& kUserInfo )
{
	if( kUserInfo.m_iUnitUID <= 0 )
	{
		START_LOG( cerr, L"ĳ���� UID �̻�" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	std::map< UidType, KCnPartyUserPtr >::iterator mit = m_mapMember.find( kUserInfo.m_iUnitUID );
	if( mit != m_mapMember.end() )
	{
		START_LOG( cerr, L"�̹� ��ȸ����" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	// ��Ƽ�� ����
	KCnPartyUserPtr spPartyUser( new KCnPartyUser( kUserInfo ) );

	//��Ƽ�� �Ӹ�
	if( m_mapMember.size() == 0 )
	{
		spPartyUser->SetHost( true );
	}

	m_mapMember.insert( std::make_pair( kUserInfo.m_iUnitUID, spPartyUser ) );

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	if( kUserInfo.m_bComeBack == true )
	{
		if( CheckComeBackUserInParty() == true ) // ������ ��Ƽ�� �� ���� ������ ���� ��
		{
			CTime tUserBuffEnd;
			if( KncUtil::ConvertStringToCTime( kUserInfo.m_wsrtComeBackEnd, tUserBuffEnd ) == false )
			{
				START_LOG( cerr, L"���ڿ� �ð����� ����.!" )
					<< BUILD_LOG( kUserInfo.m_iGSUID )
					<< BUILD_LOG( kUserInfo.m_wsrtComeBackEnd )
					<< END_LOG;

				tUserBuffEnd = CTime( 0 );
			}

			if( m_tComeBackBuffEnd < tUserBuffEnd )
			{
				m_tComeBackBuffEnd = tUserBuffEnd;
			}
			SetComeBackUserInParty( true );
		}
		else // ������ ��Ƽ�� �� ���� ������ ���� ��
		{
			SetComeBackUserInParty( true );
			SetComeBackBuffEnd( kUserInfo.m_wsrtComeBackEnd );
		}
	}
#endif SERV_COME_BACK_USER_REWARD
	//}} 

	// ��Ƽ�� üũ
	if( GetHostUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ�� ������ �߸��Ǿ���!" )
			<< BUILD_LOG( kUserInfo.m_wstrNickName )
			<< END_LOG;
	}

	return true;
}

bool KCnPartyUserManager::DeleteCandidate( IN UidType nCID )
{
	std::map< UidType, KWaitingUserInfo >::iterator mit = m_mapCandidate.find( nCID );
	if( mit == m_mapCandidate.end() )
	{
		START_LOG( cerr, L"�ĺ� ȸ���� �������� ����" )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	m_mapCandidate.erase( mit );
	return true;
}

bool KCnPartyUserManager::DeleteMember( IN UidType nCID )
{
	// ��Ƽ���̾��� ��� ���ο� ��Ƽ�� �Ӹ�.

	std::map< UidType, KCnPartyUserPtr >::iterator mit = m_mapMember.find( nCID );
	if( mit == m_mapMember.end() )
	{
		START_LOG( cerr, L"��ȸ���� �������� ����" )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	// ��Ƽ���̾����� �����Ѵ��� ����
	const bool bIsHost = mit->second->IsHost();

	// ����
	m_mapMember.erase( mit );

	//��Ƽ���̾��µ� ��Ƽ���� �����ִ°��.
	if( bIsHost == true  &&  m_mapMember.size() > 0 )
	{
		mit = m_mapMember.begin();
		mit->second->SetHost( true );

		// ��Ƽ�� üũ
		if( GetHostUID() == 0 )
		{
			START_LOG( cerr, L"��Ƽ�� ������ �߸��Ǿ���!" )
				<< BUILD_LOG( nCID )
				<< END_LOG;
		}
	}	

	return true;
}

bool KCnPartyUserManager::Create( IN const std::vector< KPartyUserInfo >& vecUserInfo )
{
	bool bRet = false;

	BOOST_TEST_FOREACH( const KPartyUserInfo&, kInfo, vecUserInfo )
	{
		if( GetNumMember() == 0 )
		{
			if( Create( kInfo ) == true )
			{
				bRet = true;
			}
		}
		else
		{
			if( AddMember( kInfo ) == true )
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool KCnPartyUserManager::Create( IN const std::vector< KRegroupPartyUserInfo >& vecUserInfo )
{
	bool bRet = false;

	BOOST_TEST_FOREACH( const KRegroupPartyUserInfo&, kInfo, vecUserInfo )
	{
		if( GetNumMember() == 0 )
		{
			if( Create( kInfo.m_kPartyUserInfo ) == true )
			{
				bRet = true;
			}
		}
		else
		{
			if( AddMember( kInfo.m_kPartyUserInfo ) == true )
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool KCnPartyUserManager::Create( IN const KPartyUserInfo& kUserInfo )
{
	if( kUserInfo.m_iUnitUID <= 0 )
	{
		START_LOG( cerr, L"ĳ���� UID �̻�" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	if( GetNumTotal() > 0 )
	{
		START_LOG( cerr, L"�� ��Ƽ�� �ƴ�" )
			<< BUILD_LOG( GetNumTotal() )
			<< END_LOG;

		return false;
	}

	LIF( AddMember( kUserInfo ) );
	return true;
}

bool KCnPartyUserManager::JoinCandidate( IN const KPartyUserInfo& kUserInfo, IN const bool bInvited, IN const UidType iSendInviteUnitUID )
{
	if( kUserInfo.m_iUnitUID <= 0 )
	{
		START_LOG( cerr, L"ĳ���� UID �̻�" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	if( GetNumTotal() < 1 )
	{
		START_LOG( cerr, L"��Ƽ���� ���� ��Ƽ��" )
			<< BUILD_LOG( GetNumMember() )
			<< BUILD_LOG( GetNumCandidate() )
			<< BUILD_LOG( GetNumTotal() )
			<< BUILD_LOG( GetMaxMember() )
			<< END_LOG;

		return false;
	}

	if( GetNumTotal() >= GetMaxMember() )
	{
		// �ð����� �߻� ������ ������.
		// ����1 : ��Ƽ����Ʈ�� ���Ű����� 3����, ���� ��Ƽ�ο� ������ ���ŵǱ����� ������ ��Ƽ ���� �õ��ϸ� �߻�������.

		START_LOG( cwarn, L"��Ƽ ���� �ο��� ���� ��." )
			<< BUILD_LOG( GetNumMember() )
			<< BUILD_LOG( GetNumCandidate() )
			<< BUILD_LOG( GetNumTotal() )
			<< BUILD_LOG( GetMaxMember() )
			<< END_LOG;

		return false;
	}

	if( IsMemberOrCandidate( kUserInfo.m_iUnitUID ) )
	{
		START_LOG( cerr, L"�̹� ��Ƽ�� �����ϴ� ������" )
			<< BUILD_LOG( GetNumTotal() )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	LIF( AddCandidate( kUserInfo, bInvited, iSendInviteUnitUID ) );
	return true;
}

bool KCnPartyUserManager::JoinMember( IN const KPartyUserInfo& kUserInfo )
{
	if( kUserInfo.m_iUnitUID <= 0 )
	{
		START_LOG( cerr, L"ĳ���� UID �̻�" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	if( !IsCandidate( kUserInfo.m_iUnitUID ) )
	{
		START_LOG( cerr, L"�ĺ� ȸ���� �ƴ�" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	if( IsMember( kUserInfo.m_iUnitUID ) )
	{
		START_LOG( cerr, L"�̹� ��ȸ����" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	LIF( DeleteCandidate( kUserInfo.m_iUnitUID ) );
	LIF( AddMember( kUserInfo ) );

	return true;
}

bool KCnPartyUserManager::LeaveCandidate( IN UidType iUnitUID )
{
	if( iUnitUID <= 0 )
	{
		START_LOG( cerr, L"ĳ���� UID �̻�" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	if( !IsCandidate( iUnitUID ) )
	{
		START_LOG( cerr, L"�ĺ� ȸ���� �ƴ�" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	LIF( DeleteCandidate( iUnitUID ) );
	return true;
}

bool KCnPartyUserManager::LeaveMember( IN UidType iUnitUID )
{	
	if( iUnitUID <= 0 )
	{
		START_LOG( cerr, L"ĳ���� UID �̻�" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	if( !IsMember( iUnitUID ) )
	{
		START_LOG( cerr, L"��ȸ���� �ƴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	LIF( DeleteMember( iUnitUID ) );
	return true;
}

bool KCnPartyUserManager::IsHost( IN UidType nCID )
{
	KCnPartyUserPtr spPartyUser = GetPartyUser( nCID );
	if( spPartyUser == NULL )
	{
		START_LOG( cerr, L"��Ƽ�� üũ�� ������ ����.!" )
			<< BUILD_LOG( nCID )
			<< BUILD_LOG( m_mapMember.size() )
			<< END_LOG;
		return false;
	}

	return spPartyUser->IsHost();
}

bool KCnPartyUserManager::IsAllReady()
{
	//����ڰ� ������ üũ�� ������ �ʴ´�.
	if( m_mapCandidate.empty() == false )
		return false;

	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->IsHost() == false  &&  mit->second->IsReady() == false )
			return false;
	}

	return true;
}

//{{ 2010. 05. 18  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION

bool KCnPartyUserManager::IsAllPartyMembersSameServerGroup( IN UidType iUnitUID )
{
	const UidType iServerGroupID = KncUid::ExtractServerGroupID( iUnitUID );

	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		// üũ�Ϸ��� ������ UnitUID�� �ٸ� ������ ������ �ִ��� ����!
		if( KncUid::ExtractServerGroupID( mit->second->GetCharUID() ) != iServerGroupID )
		{
			return false;			
		}
	}

	return true;
}

#endif SERV_INTEGRATION
//}}

//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
bool KCnPartyUserManager::IsAllPartyMemberEqualOrOverThisLevel( IN const u_char ucLevel )
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"��Ƽ�� �����Ͱ��� �̻��մϴ�!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( spPartyUser->GetLevel() < ucLevel )
			return false;
	}

	return true;
}
#endif SERV_PVP_NEW_SYSTEM
//}}

bool KCnPartyUserManager::ChangeReady( IN UidType nCID, IN bool bIsReady )
{
	KCnPartyUserPtr spPartyUser = GetPartyUser( nCID );
	if( spPartyUser == NULL )
	{
		START_LOG( cerr, L"��ȸ���� �������� ����" )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spPartyUser->SetReady( bIsReady );
	return true;
}

void KCnPartyUserManager::ChangeAllReady( IN bool bIsReady )
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;

	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->IsHost() == false )
		{
			mit->second->SetReady( bIsReady );

			KEGS_PARTY_CHANGE_READY_NOT kNot;
			kNot.m_UnitUID	= mit->second->GetCharUID();
			kNot.m_bReady	= bIsReady;
			BroadCastToGSCharacter( EPM_PARTY_CHANGE_READY_NOT, kNot );
		}
	}
}

bool KCnPartyUserManager::ChangeHost( IN UidType iNewHostUID )
{	
	KCnPartyUserPtr spOldHostUser = GetHostPtr();
	if( spOldHostUser == NULL )
	{
		START_LOG( cerr, L"��Ƽ���� ã���� ����.!" )
			<< END_LOG;
		return false;
	}

	KCnPartyUserPtr spNewHostUser = GetPartyUser( iNewHostUID );
	if( spNewHostUser == NULL )
	{
		START_LOG( cerr, L"����� ��Ƽ���� ã���� ����.!" )
			<< BUILD_LOG( spOldHostUser->GetCharUID() )
			<< BUILD_LOG( iNewHostUID )
			<< END_LOG;
		return false;
	}

	spOldHostUser->SetHost( false );
	spNewHostUser->SetHost( true );

	// ��Ƽ�� üũ
	if( GetHostUID() == 0 )
	{
		START_LOG( cerr, L"��Ƽ�� ������ �߸��Ǿ���!" )
			<< BUILD_LOG( iNewHostUID )
			<< END_LOG;
	}
	return true;
}

void KCnPartyUserManager::UpdatePartyMemberListToFieldUser( IN const UidType iPartyUID )
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		// Ŭ���̾�Ʈ�� ��Ƽ ����Ʈ ������Ʈ�� ������.
		KEPM_SEND_UPDATE_PARTY_MEMBER_LIST_NOT kPacket;
		kPacket.m_iPartyUID = iPartyUID;
		GetUnitUIDList( kPacket.m_setPartyUIDList );
		mit->second->SendToGSCharacter( EPM_SEND_UPDATE_PARTY_MEMBER_LIST_NOT, kPacket );
	}
}


//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
bool KCnPartyUserManager::CheckComeBackUserInParty()
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit = m_mapMember.begin();
	for( ; mit != m_mapMember.end() ; ++mit )
	{
		KCnPartyUserPtr pCnPartyUser = mit->second;
		if( pCnPartyUser == NULL)
		{
			START_LOG( cerr, L"CnPartyUser �����Ͱ� �̻��ϴ�!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// ���� �������� �Ǻ�����!
		// �Ѹ��̶� ���� �����̸� true
		if( pCnPartyUser->GetComeBackUser() == true )
		{
			SetComeBackUserInParty( true );
			return true;
		}
	}
	SetComeBackUserInParty( false );
	return false;
}

void KCnPartyUserManager::SetComeBackBuffEnd( IN const std::wstring& wstrBuffEnd )
{
	if( GetComeBackUserInParty() == true )
	{
		if( KncUtil::ConvertStringToCTime( wstrBuffEnd, m_tComeBackBuffEnd ) == false )
		{
			START_LOG( cerr, L"���ڿ� �ð����� ����.!" )
				<< BUILD_LOG( wstrBuffEnd )
				<< END_LOG;

			m_tComeBackBuffEnd = CTime( 0 );
		}
	}
	else
	{
		m_tComeBackBuffEnd = CTime( 0 );
	}
}
#endif SERV_COME_BACK_USER_REWARD
//}} 

//{{ 2012. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnPartyUserManager::GetCadidateUserList( OUT std::vector< UidType >& vecCadidateUserList )
{
	vecCadidateUserList.clear();

	// ��û ����� ����Ʈ üũ
	if( m_mapCandidate.empty() )
		return;

	std::vector< UidType > vecLeaveList;
	std::map< UidType, KWaitingUserInfo >::iterator mit;
	for( mit = m_mapCandidate.begin(); mit != m_mapCandidate.end(); ++mit )
	{
		const KWaitingUserInfo& kWaitingUserInfo = mit->second;

		if( IS_NULL( kWaitingUserInfo.m_spUser ) )
			continue;

		// ������ ����� ����Ʈ�� �߰��Ѵ�.
		vecCadidateUserList.push_back( kWaitingUserInfo.m_spUser->GetCharUID() );
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 03. 22	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
bool KCnPartyUserManager::SetPartyMemberStatus( IN const UidType iUnitUID, IN const KPartyMemberStatus& kInfo )
{
	KCnPartyUserPtr spCnPartyUser = GetPartyUser( iUnitUID );
	if( IS_NULL( spCnPartyUser ) )
	{
		START_LOG( cerr, L"���� ���� �ʴ� �����Դϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spCnPartyUser->SetPartyMemberStatus( kInfo );
	return true;
}

short KCnPartyUserManager::GetOldPartyFeverPoint() const
{
	std::map< UidType, KCnPartyUserPtr >::const_iterator mit = m_mapMember.begin();
	for( ; mit != m_mapMember.end() ; ++mit )
	{
		KCnPartyUserPtr spCnPartyUser = mit->second;
		if( IS_NULL( spCnPartyUser ) )
		{
			START_LOG( cerr, L"CnPartyUser �����Ͱ� �̻��ϴ�!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// ���� ��Ƽ ������ �־����� Ȯ��!
		KOldPartyInfo kInfo;
		if( spCnPartyUser->GetOldPartyInfo( kInfo ) == false )
			break;

		return kInfo.m_sOldPartyFever;
	}

	return 0;
}

bool KCnPartyUserManager::UpdateJoinRoomInfoAndGetOnOffResult( IN const UidType iUnitUID, 
															   IN const KPartyUserJoinRoomInfo& kJoinRoomInfo, 
															   OUT std::vector< UidType >& vecOnMoveButton,
															   OUT std::vector< UidType >& vecOffMoveButton )
{
	vecOnMoveButton.clear();
	vecOffMoveButton.clear();

	KCnPartyUserPtr spPartyUser = GetPartyUser( iUnitUID );
	if( spPartyUser == NULL )
	{
		START_LOG( cerr, L"�� ���� ������ ������ ������ ã�� ���Ͽ����ϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	const KPartyUserJoinRoomInfo& kBeforeJoinRoomInfo = spPartyUser->GetJoinRoomInfo();
	spPartyUser->UpdateJoinRoomInfo( kJoinRoomInfo );

	// �̵��� ���� ��Ʋ�ʵ�� ������ �ִ� ��츸 ó��
	if( CXSLRoom::GetRoomType( kBeforeJoinRoomInfo.m_iRoomUID ) == CXSLRoom::RT_BATTLE_FIELD  ||  
		CXSLRoom::GetRoomType( kJoinRoomInfo.m_iRoomUID ) == CXSLRoom::RT_BATTLE_FIELD )
	{		
		std::map< int, UidType > mapCheckSameBattleField; // ���� �ʵ忡 �ִ� ������ ã�� ���� �����̳�		

        // ���� �ʵ忡 �ִ� ������ �ִ��� Ȯ���ϱ�!
		std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
		for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
		{
			KCnPartyUserPtr spCheckPartyUser = mit->second;
			if( spCheckPartyUser == NULL )
			{
				START_LOG( cerr, L"�� ���� ������ ������ ������ ã�� ���Ͽ����ϴ�!" )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
				continue;
			}

			// �濡 ������ ���°� �ƴѵ�..
			if( spCheckPartyUser->GetJoinRoomUID() == 0 )
			{
				// ��ư�� �����ִٸ�.. ����!
				if( spCheckPartyUser->IsOnGoToPartyButton() == true )
				{
					spCheckPartyUser->SetOnGoToPartyButton( false );
					vecOffMoveButton.push_back( spCheckPartyUser->GetCharUID() );					
				}

				// ����������..
				continue;
			}

			// ���� ��ġ�� ��Ʋ�ʵ尡 �ƴѵ�..
			if( spCheckPartyUser->GetJoinRoomType() != CXSLRoom::RT_BATTLE_FIELD )
			{
				// ��ư�� �����ִٸ�.. ����!
				if( spCheckPartyUser->IsOnGoToPartyButton() == true )
				{
					spCheckPartyUser->SetOnGoToPartyButton( false );
                    vecOffMoveButton.push_back( spCheckPartyUser->GetCharUID() );
				}

				// ���� ������..
				continue;
			}
			// ���� ��ġ�� ��Ʋ�ʵ��ε�..
			else
			{
				// ���� �ʵ������� üũ������ ���ٸ�!
				std::map< int, UidType >::const_iterator mitFB = mapCheckSameBattleField.find( spCheckPartyUser->GetJoinBattleFieldID() );
                if( mitFB == mapCheckSameBattleField.end() )
				{
					// �������! �� �������� ���̴°ž�!!
					mapCheckSameBattleField.insert( std::make_pair( spCheckPartyUser->GetJoinBattleFieldID(), spCheckPartyUser->GetJoinRoomUID() ) );

					// ��ϵ� ����� ��ư�� ���� ����!
					if( spCheckPartyUser->IsOnGoToPartyButton() == true )
					{
						spCheckPartyUser->SetOnGoToPartyButton( false );
						vecOffMoveButton.push_back( spCheckPartyUser->GetCharUID() );
					}
				}
				// �̹� ��ϵ� ������ �ִٸ�!
				else
				{
					// �̹� �������� �ִ°Ŷ��..
					if( mitFB->second == spCheckPartyUser->GetJoinRoomUID() )
					{
						// ��ư�� ���� ����!
						if( spCheckPartyUser->IsOnGoToPartyButton() == true )
						{
							spCheckPartyUser->SetOnGoToPartyButton( false );
							vecOffMoveButton.push_back( spCheckPartyUser->GetCharUID() );							
						}

						// ���� ������..
						continue;
					}
					// �ٸ� �ʵ忡 �ִ� �����..
					else
					{
						// ��ư�� ������!
						if( spCheckPartyUser->IsOnGoToPartyButton() == false )
						{
							spCheckPartyUser->SetOnGoToPartyButton( true );
							vecOnMoveButton.push_back( spCheckPartyUser->GetCharUID() );
						}

						// ���� ������..
						continue;
					}
				}
			}
		}
	}

	return true;
}

void KCnPartyUserManager::GetUnitUIDListAboutEndGameRegroupParty( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUser, OUT std::set< UidType >& setLeaveUnitUIDList )
{
	vecRegroupPartyUser.clear();
	setLeaveUnitUIDList.clear();

	// 1. ���� ���� ��Ƽ�� 3���̾��µ� ���� ���ο��� 4���� ���! ( 1�� ���� ��Ű�� �ȴ�! )
	// 2. ���� ���� ��Ƽ�� 2���̾��µ� ���� ���ο��� 3���ΰ��! ( 1�� ���� ��Ű�� �ȴ�! )
	// 3. ���� ���� ��Ƽ�� �Ѹ� ������ ���! ( ��δ� ��Ƽ Ż�� ó�� �ϸ� �ȴ�! )

	// ����� �ؾ� �ϴ� ���
	// 1. ���� ���� ��Ƽ�� 2��:2�� �̾��� ���� ���ο��� 4���� ���! ( 2���� �ڵ� ���� ó��, ���Ŀ� ����� ��ġ! )

	switch( GetNumMember() ) 
	{
	case 1:
		CheckRegroup1Member( vecRegroupPartyUser, setLeaveUnitUIDList );
		break;

	case 2:
		CheckRegroup2Member( vecRegroupPartyUser, setLeaveUnitUIDList );
		break;

	case 3:
		CheckRegroup3Member( vecRegroupPartyUser, setLeaveUnitUIDList );
		break;

	case 4:
		CheckRegroup4Member( vecRegroupPartyUser, setLeaveUnitUIDList );
		break;

	default:
		{
            START_LOG( cerr, L"���� ��Ƽ�� ���� �̻��մϴ�!" )
				<< BUILD_LOG( GetNumMember() )
				<< END_LOG;
		}
		break;
	}
}

void KCnPartyUserManager::GetCheckedSameOldParty( OUT std::map< UidType, std::set< UidType > >& mapCheckedSameOldParty, OUT std::set< UidType >& setLeaveUnitUIDList )
{
	mapCheckedSameOldParty.clear();

	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"�� ���� ������ ������ ������ ã�� ���Ͽ����ϴ�!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		KOldPartyInfo kInfo;
		if( spPartyUser->GetOldPartyInfo( kInfo ) == false )
		{
			// ���� ��Ƽ ������ ���ٸ� �׳� ��Ƽ ��Ż!
			setLeaveUnitUIDList.insert( spPartyUser->GetCharUID() );
			continue;
		}

		// �����Ҷ� ��Ƽ ���°� �ƴϾ��ٸ� ��Ƽ ��Ż!
		if( kInfo.m_iOldPartyUID == 0 )
		{
			setLeaveUnitUIDList.insert( spPartyUser->GetCharUID() );
			continue;
		}

		// �����Ҷ� ���� ��Ƽ���� �����ٸ� ���� ��Ƽ ��Ż!
		if( kInfo.m_setOldPartyMember.empty() == true )
		{
			setLeaveUnitUIDList.insert( spPartyUser->GetCharUID() );
			continue;
		}

		// ���� ����߿� ���� ��Ƽ���� �ִ��� üũ����!		
		BOOST_TEST_FOREACH( const UidType, iUnitUID, kInfo.m_setOldPartyMember )
		{
			bool bInsertOldPartyMember = false;

			if( iUnitUID == spPartyUser->GetCharUID() )
			{
				bInsertOldPartyMember = true;
			}
			else
			{
				if( IsMember( iUnitUID ) == true )
				{
					bInsertOldPartyMember = true;
				}
			}

			// ���� ����߿� ���� ���� ��Ƽ���� �ִٸ� üũ!!
			if( bInsertOldPartyMember )
			{
				std::map< UidType, std::set< UidType > >::iterator mitOP;
				mitOP = mapCheckedSameOldParty.find( kInfo.m_iOldPartyUID );
				if( mitOP == mapCheckedSameOldParty.end() )
				{
					std::set< UidType > setOldPartyMember;
					setOldPartyMember.insert( iUnitUID );
					mapCheckedSameOldParty.insert( std::make_pair( kInfo.m_iOldPartyUID, setOldPartyMember ) );
				}
				else
				{
					mitOP->second.insert( iUnitUID );
				}
			}
		}
	}
}

void KCnPartyUserManager::CheckRegroup1Member( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUser, OUT std::set< UidType >& setLeaveUnitUIDList )
{
	vecRegroupPartyUser.clear();
	setLeaveUnitUIDList.clear();

	_JIF( GetNumMember() == 1, return; );

	//////////////////////////////////////////////////////////////////////////
	// ���� ��Ƽ������ 1���� ��쿣 �翬�� �ڵ����� ��Ƽ ��ü�� ���Ѿ� �Ѵ�!
	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"�� ���� ������ ������ ������ ã�� ���Ͽ����ϴ�!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		setLeaveUnitUIDList.insert( spPartyUser->GetCharUID() );
	}
}

void KCnPartyUserManager::CheckRegroup2Member( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUser, OUT std::set< UidType >& setLeaveUnitUIDList )
{
	vecRegroupPartyUser.clear();
	setLeaveUnitUIDList.clear();

	//////////////////////////////////////////////////////////////////////////
	// ���� ��Ƽ������ 2���� ��쿣
	// 1. �Ѵ� ���� ���� �Ҽ� ������Ƽ���̶�� ��Ƽ ����!
	// 2. �׷��� �ʴٸ� ������ ��Ƽ ��Ż ó��!

	const int iNumMember = GetNumMember();
	_JIF( iNumMember == 2, return; );

	std::map< UidType, std::set< UidType > > mapCheckedSameOldParty;
	GetCheckedSameOldParty( mapCheckedSameOldParty, setLeaveUnitUIDList );

	// ��Ƽ �Ҽ��� 1�� ������ ��� ��Ż ó�� �մϴ�!
	std::map< UidType, std::set< UidType > >::const_iterator mit;
	for( mit = mapCheckedSameOldParty.begin(); mit != mapCheckedSameOldParty.end(); ++mit )
	{
		const std::set< UidType >& setOldPartyMember = mit->second;
		if( setOldPartyMember.size() == 1 )
		{
			BOOST_TEST_FOREACH( const UidType, iUnitUID, setOldPartyMember )
			{
				setLeaveUnitUIDList.insert( iUnitUID );
			}
		}
	}
}

void KCnPartyUserManager::CheckRegroup3Member( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUser, OUT std::set< UidType >& setLeaveUnitUIDList )
{
	vecRegroupPartyUser.clear();
	setLeaveUnitUIDList.clear();

	//////////////////////////////////////////////////////////////////////////
	// ���� ��Ƽ������ 3���� ��쿣
	// 1. ���� �� ���� ���� �Ҽ� ������Ƽ���̶�� ��Ƽ ����!
	// 2. ������ �θ� ���� �Ҽ� ������Ƽ���̶�� �� �θ� �����ϰ� ���� �Ѹ��� ��Ż ó��!
	// 3. ���� ��� ���� �Ҽ� ������Ƽ���� �ƴ϶�� ��� ��Ż ó��!

	const int iNumMember = GetNumMember();
	_JIF( iNumMember == 3, return; );

	std::map< UidType, std::set< UidType > > mapCheckedSameOldParty;
	GetCheckedSameOldParty( mapCheckedSameOldParty, setLeaveUnitUIDList );

	// ��Ƽ �Ҽ��� 1�� ������ ��� ��Ż ó�� �մϴ�!
	std::map< UidType, std::set< UidType > >::const_iterator mit;
	for( mit = mapCheckedSameOldParty.begin(); mit != mapCheckedSameOldParty.end(); ++mit )
	{
		const std::set< UidType >& setOldPartyMember = mit->second;
		if( setOldPartyMember.size() == 1 )
		{
			BOOST_TEST_FOREACH( const UidType, iUnitUID, setOldPartyMember )
			{
				setLeaveUnitUIDList.insert( iUnitUID );
			}
		}
	}
}

void KCnPartyUserManager::CheckRegroup4Member( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUser, OUT std::set< UidType >& setLeaveUnitUIDList )
{
	vecRegroupPartyUser.clear();
	setLeaveUnitUIDList.clear();

	const int iNumMember = GetNumMember();
	_JIF( iNumMember == 4, return; );

	std::map< UidType, std::set< UidType > > mapCheckedSameOldParty;
	GetCheckedSameOldParty( mapCheckedSameOldParty, setLeaveUnitUIDList );

	// ��Ƽ �Ҽ��� 1�� ������ ��� ��Ż ó�� �մϴ�!
	bool bCheckedDoubleParty = false;

	std::map< UidType, std::set< UidType > >::const_iterator mit;
	for( mit = mapCheckedSameOldParty.begin(); mit != mapCheckedSameOldParty.end(); ++mit )
	{
		const std::set< UidType >& setOldPartyMember = mit->second;

		// ���� ��Ƽ ������� 1�� ���. �ٷ� ��Ż ó��.
		if( setOldPartyMember.size() == 1 )
		{
			BOOST_TEST_FOREACH( const UidType, iUnitUID, setOldPartyMember )
			{
				setLeaveUnitUIDList.insert( iUnitUID );
			}
			continue;
		}

		if( setOldPartyMember.size() != 2 )
			continue;

		// ���� ��Ƽ ������� 2�� ��� ó�� ������� ���� ��Ƽ�� ��� �Ѵ�. ( ���� ��Ƽ�� ��Ż��ų �ʿ䰡 ���� )
		if( bCheckedDoubleParty == false )
		{
			bCheckedDoubleParty = true;
			continue;
		}
		
		// �ι�°�� üũ�Ǵ� ��Ƽ�� ����� �ؾ��ϴ� ��Ƽ�̴�!
		BOOST_TEST_FOREACH( const UidType, iUnitUID, setOldPartyMember )
		{
			KCnPartyUserPtr spPartyUser = GetPartyUser( iUnitUID );
			if( spPartyUser == NULL )
			{
				START_LOG( cerr, L"�� ���� ������ ������ ������ ã�� ���Ͽ����ϴ�!" )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
				continue;
			}

			KRegroupPartyUserInfo kInfo;
			spPartyUser->GetPartyUserInfo( kInfo.m_kPartyUserInfo );
			spPartyUser->GetOldPartyInfo( kInfo.m_kOldPartyInfo );
			vecRegroupPartyUser.push_back( kInfo );

			// ����յǴ� ��Ƽ������ ���� ��Ƽ���� ��Żó���Ѵ�.
			setLeaveUnitUIDList.insert( iUnitUID );
		}
	}
}

//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
bool KCnPartyUserManager::ChangeOldPartyHost( OUT UidType& iNewHostUID )
{
	iNewHostUID = 0;

	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"�ڵ� ��Ƽ üũ�� �ʱ�ȭ�� ������ ã�� ���Ͽ����ϴ�!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		KOldPartyInfo kInfo;
		if( spPartyUser->GetOldPartyInfo( kInfo ) == false )
			continue;

		if( iNewHostUID == 0 )
		{
			iNewHostUID = kInfo.m_iOldHostUnitUID;
			continue;
		}
		
		if( iNewHostUID != kInfo.m_iOldHostUnitUID )
		{
			START_LOG( cerr, L"������ ���� ��Ƽ���̾��µ� ��Ƽ���� �޶���? �̻��� ���̳�.." )
				<< BUILD_LOG( iNewHostUID )
				<< BUILD_LOG( kInfo.m_iOldHostUnitUID )
				<< END_LOG;
		}
	}

	// ���õ� ���� ��Ƽ���� ���ٸ� ��ü���� ����!
	if( iNewHostUID == 0 )
		return false;

	// ���� ��Ƽ��� �ٸ��ٸ� ��Ƽ���� ��ü����!
	return ( iNewHostUID != GetHostUID() );
}
#endif SERV_REGROUP_OLD_PARTY_HOST
//}}

void KCnPartyUserManager::ClearCheckedForAutoParty()
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"�ڵ� ��Ƽ üũ�� �ʱ�ȭ�� ������ ã�� ���Ͽ����ϴ�!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		spPartyUser->SetCheckedForAutoParty( false );
	}
}

bool KCnPartyUserManager::SetCheckedForAutoParty( IN const UidType iUnitUID )
{
    KCnPartyUserPtr spPartyUser = GetPartyUser( iUnitUID );
	if( spPartyUser == NULL )
	{
		START_LOG( cerr, L"��Ƽ ������ �������� �ʽ��ϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;	
		return false;
	}

    spPartyUser->SetCheckedForAutoParty( true );
	return true;
}

bool KCnPartyUserManager::IsAllPartyMembersCheckedForAutoParty() const
{
	if( m_mapMember.empty() )
		return false;

	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"�ڵ� ��Ƽ üũ�� �ʱ�ȭ�� ������ ã�� ���Ͽ����ϴ�!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( spPartyUser->IsCheckedForAutoParty() == false )
			return false;
	}

	return true;
}

void KCnPartyUserManager::ClearCheckedPlayCondition()
{

	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"�ڵ� ��Ƽ üũ�� �ʱ�ȭ�� ������ ã�� ���Ͽ����ϴ�!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		spPartyUser->SetCheckedPlayCondition( false );
	}
}

bool KCnPartyUserManager::SetCheckedPlayCondition( IN const UidType iUnitUID )
{
	KCnPartyUserPtr spPartyUser = GetPartyUser( iUnitUID );
	if( spPartyUser == NULL )
	{
		START_LOG( cerr, L"��Ƽ ������ �������� �ʽ��ϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;	
		return false;
	}

	spPartyUser->SetCheckedPlayCondition( true );
	return true;
}

bool KCnPartyUserManager::IsAllPartyMembersCheckedPlayCondition() const
{
	if( m_mapMember.empty() )
		return false;

	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"�ڵ� ��Ƽ üũ�� �ʱ�ȭ�� ������ ã�� ���Ͽ����ϴ�!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( spPartyUser->IsCheckedPlayCondition() == false )
			return false;
	}

	return true;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}



