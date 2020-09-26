#include "AutoParty.h"
#include "AutoPartyUserManager.h"

#include "Enum/Enum.h"
#include "NetError.h"
#include "AutoPartyUserFSM.h"
#include "CompareLimitList.h"


//{{ 2012. 02. 08	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


FSMclassPtr KAutoPartyUserManager::ms_spUserFSM = KAutoPartyUserFSMPtr( new KAutoPartyUserFSM );


KAutoPartyUserManager::KAutoPartyUserManager()
{
}

KAutoPartyUserManager::~KAutoPartyUserManager()
{
}

void KAutoPartyUserManager::Init( IN const KAutoPartyConditionInfo& kInfo )
{
	m_kConditionInfo	 = kInfo;
}

bool KAutoPartyUserManager::Enter( IN const KAutoPartyUserInfo& kInfo )
{
    if( IsExist( kInfo.m_iUnitUID ) == true )
	{
		START_LOG( cerr, L"�̹� ������ �ִ� �����Դϴ�!" )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;
		return false;
	}

	if( (int)( m_mapMember.size() + 1 )  > GetLimitPlayerCount() )
	{
		START_LOG( cerr, L"�ִ� ���� �ο� ���ѿ� �ɷȽ��ϴ�." )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< BUILD_LOG( GetLimitPlayerCount() )
			<< BUILD_LOG( m_mapMember.size() )
			<< END_LOG;
		return false;
	}

	KAutoPartyUserPtr spAutoPartyUser( new KAutoPartyUser( kInfo ) );
	spAutoPartyUser->SetFSM( ms_spUserFSM );
	m_mapMember.insert( std::make_pair( kInfo.m_iUnitUID, spAutoPartyUser ) );
	return true;
}

bool KAutoPartyUserManager::Enter( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserList )
{
	const int iAutoPartyReqMemberCount = static_cast<int>(vecAutoPartyUserList.size());

	if( (int)( m_mapMember.size() + iAutoPartyReqMemberCount )  > GetLimitPlayerCount() )
	{
		START_LOG( cerr, L"�ִ� ���� �ο� ���ѿ� �ɷȽ��ϴ�." )
			<< BUILD_LOG( GetLimitPlayerCount() )
			<< BUILD_LOG( m_mapMember.size() )
			<< BUILD_LOG( iAutoPartyReqMemberCount )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const KAutoPartyUserInfo, kInfo, vecAutoPartyUserList )
	{
		if( Enter( kInfo ) == false )
		{
			START_LOG( cerr, L"�ڵ� ��Ƽ ���� ó�� ����!" )
				<< END_LOG;
		}
	}

	return true;
}

void KAutoPartyUserManager::Reset()
{
}

KAutoPartyUserPtr KAutoPartyUserManager::GetUser( IN const UidType iUnitUID )
{
    std::map< UidType, KAutoPartyUserPtr >::iterator mit;
    mit = m_mapMember.find( iUnitUID );
    if( mit == m_mapMember.end() )
    {
        START_LOG( cerr, L"�������� �ʴ� ����." )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return KAutoPartyUserPtr();
    }

    return mit->second;
}

void KAutoPartyUserManager::GetAutoPartyUserList( OUT std::vector< UidType >& vecUserList )
{
	vecUserList.clear();

	std::map< UidType, KAutoPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		vecUserList.push_back( mit->first );
	}
}

bool KAutoPartyUserManager::DeleteUser( IN const UidType iUnitUID )
{
    std::map< UidType, KAutoPartyUserPtr >::iterator mit;
    mit = m_mapMember.find( iUnitUID );
    if( mit == m_mapMember.end() )
    {
        START_LOG( cerr, L"�������� �ʴ� ��������." )
            << BUILD_LOG( iUnitUID )
            << END_LOG;
        return true;
    }

    m_mapMember.erase( mit );
    return true;
}

int KAutoPartyUserManager::GetNumMember() const
{
    return static_cast<int>(m_mapMember.size());
}

bool KAutoPartyUserManager::Accept( IN const UidType iUnitUID )
{
	KAutoPartyUserPtr spAutoPartyUser = GetUser( iUnitUID );
	if( spAutoPartyUser == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}
	
	return spAutoPartyUser->Accept();
}

bool KAutoPartyUserManager::IsAllPlayersAccept()
{
	std::map< UidType, KAutoPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( mit->second->IsAccepted() == false )
		{
			return false;
		}
	}

	return true;
}

void KAutoPartyUserManager::ClearAllPlayerRegroupPartyInfo()
{
	std::map< UidType, KAutoPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KAutoPartyUserPtr spAutoPartyUser = mit->second;
		if( spAutoPartyUser == NULL )
		{
			START_LOG( cerr, L"��ġ ���� �����Ͱ��� �̻��մϴ�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}
        
		spAutoPartyUser->ClearPartyUserInfo();
		spAutoPartyUser->ClearPartyRoomUserInfo();
	}
}

bool KAutoPartyUserManager::UpdateRegroupPartyInfo( IN const KPartyUserInfo& kPartyUserInfo, IN const KPartyRoomUserInfo& kPartyRoomUserInfo )
{
	KAutoPartyUserPtr spAutoPartyUser = GetUser( kPartyUserInfo.m_iUnitUID );
	if( spAutoPartyUser == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( kPartyUserInfo.m_iUnitUID )
			<< END_LOG;
		return false;
	}

	spAutoPartyUser->UpdatePartyUserInfo( kPartyUserInfo );
	spAutoPartyUser->UpdatePartyRoomUserInfo( kPartyRoomUserInfo );
	return true;
}

bool KAutoPartyUserManager::IsAllPlayersUpdatedRegroupPartyInfo()
{
	std::map< UidType, KAutoPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( mit->second->IsEmptyPartyRoomUserInfo() == true  ||  mit->second->IsEmptyPartyUserInfo() == true )
		{
			return false;
		}
	}

	return true;
}

//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
bool KAutoPartyUserManager::GetRegroupPartyInfo( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUserInfo, OUT UidType& iRegroupPartyHostUnitUID )
#else
bool KAutoPartyUserManager::GetRegroupPartyInfo( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUserInfo )
#endif SERV_REGROUP_OLD_PARTY_HOST
//}}
{
	//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	iRegroupPartyHostUnitUID = 0;
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}
	vecRegroupPartyUserInfo.clear();

	std::map< UidType, KAutoPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{	
		KAutoPartyUserPtr spAutoPartyUser = mit->second;
		if( spAutoPartyUser == NULL )
		{
			START_LOG( cerr, L"�ڵ� ��Ƽ ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		KRegroupPartyUserInfo kRegroupPartyUserInfo;

		// ��Ƽ ��Ἲ�� ���� KPartyUserInfo
		if( spAutoPartyUser->GetPartyUserInfo( kRegroupPartyUserInfo.m_kPartyUserInfo ) == false )
		{
			START_LOG( cerr, L"��Ƽ ���� ������ ����!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}
        
		// �濡 �����ϱ� ���� KRoomUserInfo
		if( spAutoPartyUser->GetPartyRoomUserInfo( kRegroupPartyUserInfo.m_kPartyRoomUserInfo ) == false )
		{
			START_LOG( cerr, L"�� ���� ������ ����!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		// �ڵ� ��Ƽ �α� ������ ��Ƽ ���� ����!
		kRegroupPartyUserInfo.m_kOldPartyInfo = spAutoPartyUser->GetOldPartyInfo();

		// ����Ʈ�� ����
		vecRegroupPartyUserInfo.push_back( kRegroupPartyUserInfo );

		//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		// �ش� ������ ��Ƽ���̾����� ���׷� ��Ƽ�� �ĺ��� ����!
		if( iRegroupPartyHostUnitUID == 0 )
		{
			if( kRegroupPartyUserInfo.m_kOldPartyInfo.m_iOldHostUnitUID == spAutoPartyUser->GetCharUID() )
			{
				iRegroupPartyHostUnitUID = spAutoPartyUser->GetCharUID();
			}
		}
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
	}

	return true;
}

//{{ 2012. 11. 28 ����,���� �ڵ� ��Ī ��� ���� �ʴ� ���� ����(���� ��Ȳ) - ��μ�
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
void KAutoPartyUserManager::GetAutoPartyCancelUserList( OUT std::vector< UidType >& vecCancelUserList )
{
	vecCancelUserList.clear();

	std::map< UidType, KAutoPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->IsAccepted() == false )
		{
			vecCancelUserList.push_back( mit->first );
		}
	}
}
#endif SERV_AUTO_MATCH_CANCEL_ERROR
//}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}

