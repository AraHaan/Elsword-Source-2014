#include "EventRewardRetainUserItemRefreshManager.h"
//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB

//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL

#include <KncSend.h>
#include "DBLayer.h"

ImplementSingleton( KEventRewardRetainUserItemRefreshManager );

KEventRewardRetainUserItemRefreshManager::KEventRewardRetainUserItemRefreshManager(void)
{
}

KEventRewardRetainUserItemRefreshManager::~KEventRewardRetainUserItemRefreshManager(void)
{
}

ImplToStringW( KEventRewardRetainUserItemRefreshManager )
{
	stm_	<< L"----------[ Event Reward Item ]----------" << std::endl
		<< TOSTRINGW( m_mapRewardItem.size() )
		;

	return stm_;
}

void KEventRewardRetainUserItemRefreshManager::GetOutUserRetainingRewardItemInfo( std::map< int, KRewardItem >& mapRewardItem )
{
	std::map< int, KRewardItem >::iterator mit = m_mapRewardItem.begin();

	if( mit != m_mapRewardItem.end() )
	{
		mapRewardItem = m_mapRewardItem;
	}
}

bool KEventRewardRetainUserItemRefreshManager::GetOutUserRetainingSelectItemInfo( IN int& iSlot, OUT KRewardItem& kInfo )
{
	std::map< int, KRewardItem >::iterator mit = m_mapRewardItem.find( iSlot );
	if( mit != m_mapRewardItem.end() )
	{
		kInfo = mit->second;
		return true;
	}

	START_LOG( cerr, L"��Ż ���� ���� ���� ������ ������ �����ϴ�!")
		<< BUILD_LOG( iSlot )
		<< BUILD_LOG( m_mapRewardItem.size() )
		<< END_LOG;

	return false;	
}

bool KEventRewardRetainUserItemRefreshManager::GetOutUserRetainingSelectItemInfo( IN int& iItemID, OUT int& iQuantity )
{
	KRewardItem kInfo;
	std::map< int, KRewardItem >::iterator mit = m_mapRewardItem.begin();
	for( ; mit != m_mapRewardItem.end() ; ++mit )
	{
		kInfo = mit->second;
		if( kInfo.m_iItemID == iItemID )
		{
			iQuantity = kInfo.m_iQuantity;
			return true;
		}
	}

	START_LOG( cerr, L"��Ż ���� ���� ���� ������ ������ �����ϴ�!")
		<< BUILD_LOG( iItemID )
		<< BUILD_LOG( m_mapRewardItem.size() )
		<< END_LOG;

	return false;	
}
void KEventRewardRetainUserItemRefreshManager::UpdateRewardRetainUserItem( void )
{
	UidType						anTrace[2] = { 0, -1 };
	KEventPtr					spEvent( new KEvent );

	spEvent->SetData( PC_LOG_DB, anTrace, DBE_EVENT_REWARD_RETAIN_USER_ITEM_UPDATE_REQ );

	LIF( spEvent->m_kDestPerformer.AddUID( 0 ) );

	SiKDBLayer()->QueueingEvent( spEvent );
}

void KEventRewardRetainUserItemRefreshManager::SetRewardRetainUserItem( IN const std::map< int, KRewardItem >& mapRewardItem )
{
	m_mapRewardItem.clear();
	m_mapRewardItem = mapRewardItem;
}
#endif SERV_NEXON_SESSION_PROTOCOL
//}}

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}