#include ".\userpartymanager.h"
#include "BaseServer.h"


//{{ 2011. 12. 20	������	����ť�� DB�ǽð� ����
#ifdef SERV_BATTLE_FIELD_SYSTEM


KUserPartyManager::KUserPartyManager(void)
{
	Clear();
}

KUserPartyManager::~KUserPartyManager(void)
{
}

void KUserPartyManager::Clear()
{
	m_iPartyUID = 0;
	m_iCandidatePartyUID = 0;
	m_iQuickJoinPartyUID = 0;
	//{{ 2010. 02. 09  ������	������Ƽ ��ɰ���
	//#ifdef SERV_QUICK_PARTY_UPGRADE
	m_mapKickedPartyUIDList.clear();
	//#endif SERV_QUICK_PARTY_UPGRADE
	//}}
	m_vecWaitPartyUserList.clear();
}

//{{ 2010. 02. 09  ������	������Ƽ ��ɰ���
//#ifdef SERV_QUICK_PARTY_UPGRADE	
void KUserPartyManager::AddKickedPartyUID( IN UidType iPartyUID )
{
	if( CheckKickedPartyUID( iPartyUID ) )
		return;

	boost::timer tKickedPartyUIDTimer;
	tKickedPartyUIDTimer.restart();
	m_mapKickedPartyUIDList.insert( std::make_pair( iPartyUID, tKickedPartyUIDTimer ) );
}

void KUserPartyManager::CheckExpiredKickedPartyUID()
{
	if( m_mapKickedPartyUIDList.empty() )
		return;

	std::vector< UidType > vecDeletePartyUIDList;

	std::map< UidType, boost::timer >::const_iterator mit;
	for( mit = m_mapKickedPartyUIDList.begin(); mit != m_mapKickedPartyUIDList.end(); ++mit )
	{
		// 20�� �̻� �����ٸ� �����Ѵ�!
		if( mit->second.elapsed() > 20.0 )
		{
			vecDeletePartyUIDList.push_back( mit->first );
		}
	}

	BOOST_TEST_FOREACH( const UidType, iDeletePartyUID, vecDeletePartyUIDList )
	{
		m_mapKickedPartyUIDList.erase( iDeletePartyUID );
	}
}
//#endif SERV_QUICK_PARTY_UPGRADE
//}}

void KUserPartyManager::CheckWaitPartyUser()
{
	if( m_vecWaitPartyUserList.empty() )
		return;

	std::vector< SWaitPartyUserInfo >::iterator vit;
	for( vit = m_vecWaitPartyUserList.begin(); vit != m_vecWaitPartyUserList.end(); ++vit )
	{
        if( vit->m_tPartyWaitTime.elapsed() > 10.0 )
			break;
	}

	if( vit != m_vecWaitPartyUserList.end() )
	{
		m_vecWaitPartyUserList.clear();
	}
}

bool KUserPartyManager::RegWaitPartyUser( IN const UidType iUnitUID )
{
	if( IsWaitPartyUser( iUnitUID ) == true )
	{
		START_LOG( cerr, L"�̹� �ش� ������ ��Ƽ�ʴ��ο����� ��ϵǾ��ֽ��ϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	SWaitPartyUserInfo kInsertInfo;
	kInsertInfo.m_iWaitUnitUID = iUnitUID;
	m_vecWaitPartyUserList.push_back( kInsertInfo );
	return true;
}

bool KUserPartyManager::UnregWaitPartyUser( IN const UidType iUnitUID )
{
	if( IsWaitPartyUser( iUnitUID ) == true )
	{
		START_LOG( cerr, L"�̹� �ش� ������ ��Ƽ�ʴ��ο����� ��ϵ����� �����ϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	m_vecWaitPartyUserList.clear();
	return true;
}

bool KUserPartyManager::IsWaitPartyUser( IN const UidType iUnitUID ) const
{
	if( m_vecWaitPartyUserList.empty() )
		return false;

	BOOST_TEST_FOREACH( const SWaitPartyUserInfo&, kInfo, m_vecWaitPartyUserList )
	{
		if( kInfo.m_iWaitUnitUID == iUnitUID )
			return true;

		break;
	}

	return false;
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}


