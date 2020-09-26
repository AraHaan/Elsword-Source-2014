#include ".\UserUnitSelectRewardManager.h"
#include "GameServer.h"


//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY


KUserUnitSelectRewardManager::KUserUnitSelectRewardManager(void)
{
	Clear();
}

KUserUnitSelectRewardManager::~KUserUnitSelectRewardManager(void)
{
}

void KUserUnitSelectRewardManager::Clear()
{
	m_vecAccessItem.clear();
}

void KUserUnitSelectRewardManager::AddRewardItemInfo( IN const int iAddRewardID )
{
	// �ߺ� �Ǵ� �������� ���� ���� �ʴ´�
	BOOST_TEST_FOREACH( const int, iRewardID, m_vecAccessItem )
	{
		if( iRewardID == iAddRewardID )
		{
			START_LOG( clog, L"�ߺ� �Ǵ� �������� �����Ϸ� �մϴ�." )
				<< BUILD_LOG( iAddRewardID )
				<< BUILD_LOG( iRewardID )
				<< END_LOG;
			return;
		}
	}

	m_vecAccessItem.push_back( iAddRewardID );
}

void KUserUnitSelectRewardManager::GetRewardAndClear( OUT std::vector< int >& vecRewardInfo )
{
	// ���� ������ ��´�!
	vecRewardInfo = m_vecAccessItem;

	// ���� ������ �Ѱ��־����� �ʱ�ȭ ��Ų��!
	Clear();
}

//{{ 2013. 01. 21  ��Ը� �̺�Ʈ ���� �� ���� ���� ĳ���Ϳ��� �����ϴ� �ý��� - ��μ�
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
void KUserUnitSelectRewardManager::SetFirstSelectUnitRewardInfo( IN std::vector<KFirstSelectUnitReward>& vecInfo )
{
	if( vecInfo.empty() == true )
		return;

	m_mapFirstSelectUnitRewardInfo.clear();

	BOOST_TEST_FOREACH( KFirstSelectUnitReward&, kInfo, vecInfo )
	{
		m_mapFirstSelectUnitRewardInfo.insert( std::make_pair(kInfo.m_iDBIndexID, kInfo) );
	}
}

bool KUserUnitSelectRewardManager::GetFirstSelectUnitRewardInfo( IN int iServerGroupID, OUT std::vector<KFirstSelectUnitReward>& vecInfo )
{
	if( m_mapFirstSelectUnitRewardInfo.empty() == true )
	{
		return false;
	}

	vecInfo.clear();

	std::map<int, KFirstSelectUnitReward>::iterator mit = m_mapFirstSelectUnitRewardInfo.begin();
	for( ; mit != m_mapFirstSelectUnitRewardInfo.end() ; ++mit )
	{
		if( mit->second.m_cGiveType == iServerGroupID 
		 || mit->second.m_cGiveType == KFirstSelectUnitReward::GGT_ONLY_ONE )
		{
			vecInfo.push_back( mit->second );
		}
	}

	if( vecInfo.empty() == true )
	{
		return false;
	}

	return true;
}

void KUserUnitSelectRewardManager::UpdateFirstSelectUnitRewardInfo( IN int iDBIndexID, UidType iUnitUID )
{
	std::map<int, KFirstSelectUnitReward>::iterator mit = m_mapFirstSelectUnitRewardInfo.begin();
	for( ; mit != m_mapFirstSelectUnitRewardInfo.end() ; ++mit )
	{
		if( mit->first == iDBIndexID )
		{
			mit->second.m_iUnitUID = iUnitUID;
			return;
		}
	}
}

void KUserUnitSelectRewardManager::DeleteAndGetFirstSelectUnitRewardInfo( IN int iDBIndexID, KFirstSelectUnitReward& kReward )
{
	std::map<int, KFirstSelectUnitReward>::iterator mit = m_mapFirstSelectUnitRewardInfo.begin();
	for( ; mit != m_mapFirstSelectUnitRewardInfo.end() ; ++mit )
	{
		if( mit->first == iDBIndexID )
		{
			kReward = mit->second;

			m_mapFirstSelectUnitRewardInfo.erase( mit );

			return;
		}
	}
}

#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
//}

#endif SERV_SECOND_SECURITY
//}}