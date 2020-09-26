#include "EventRewardDataRefreshManager.h"
//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#include <KncSend.h>
#include "DBLayer.h"
#include <boost/test/utils/foreach.hpp>
#include "X2Data/XSLItemManager.h"

ImplementSingleton( KEventRewardDataRefreshManager );

KEventRewardDataRefreshManager::KEventRewardDataRefreshManager(void)
{
}

KEventRewardDataRefreshManager::~KEventRewardDataRefreshManager(void)
{
}

ImplToStringW( KEventRewardDataRefreshManager )
{
	stm_	<< L"----------[ Event Reward Data ]----------" << std::endl
		<< TOSTRINGW( m_mapRewardData.size() )
		;

	return stm_;
}

void KEventRewardDataRefreshManager::GetRewardItem( IN const KPostItemInfo& kPostItemInfo, OUT std::map< int, KItemInfo >& mapRewardItem ) const
{
	// ������ ���� �޽����� ��� ���
	//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	if( KPostItemInfo::IsItemIDLetter( kPostItemInfo.m_cScriptType ) == true )
		return;
#else
	switch( kPostItemInfo.m_cScriptType )
	{
	case KPostItemInfo::LT_POST_OFFICE:
	case KPostItemInfo::LT_MESSAGE:
	case KPostItemInfo::LT_BROKEN_ITEM:
	case KPostItemInfo::LT_WEB_POINT_EVENT:
		return;

	default:
		break;
	}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}

	std::map< int, std::vector< KRewardData > >::const_iterator mit;
	mit = m_mapRewardData.find( kPostItemInfo.m_iScriptIndex );
	if( mit == m_mapRewardData.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� RewardID�Դϴ�." )
			<< BUILD_LOG( kPostItemInfo.m_iScriptIndex )
			<< END_LOG;
		return;
	}

	BOOST_TEST_FOREACH( const KRewardData&, kReward, mit->second )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kReward.m_iRewardItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"������ ���ø� ������ ã�� �� �����ϴ�." )
				<< BUILD_LOG( kReward.m_iRewardItemID )
				<< END_LOG;
			continue;
		}

		// ���� ������
		KItemInfo kRewardItemInfo;
		kRewardItemInfo.m_iItemID		= kReward.m_iRewardItemID;
		kRewardItemInfo.m_iQuantity		= kReward.m_iQuantity;
		kRewardItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
		kRewardItemInfo.m_sEndurance	= pItemTemplet->m_Endurance;

		// �Ⱓ�� �������� ��� �Ⱓ ����
		if( pItemTemplet->m_PeriodType == CXSLItem::PT_INFINITY )
		{
			kRewardItemInfo.m_sPeriod	= kReward.m_sPeriod;
		}

		// ������ ItemID�� ���� �ÿ��� ������ �÷��ش�.
		std::map< int, KItemInfo >::iterator mitFindSame;
		mitFindSame = mapRewardItem.find( kRewardItemInfo.m_iItemID );
		if( mitFindSame == mapRewardItem.end() )
		{
			mapRewardItem.insert( std::make_pair( kRewardItemInfo.m_iItemID, kRewardItemInfo ) );
		}
		else
		{
			mitFindSame->second.m_iQuantity += kRewardItemInfo.m_iQuantity;
		}
	}
}

void KEventRewardDataRefreshManager::GetRewardItem( IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::map< int, KItemInfo >& mapRewardItem ) const
{
	mapRewardItem.clear();

	BOOST_TEST_FOREACH( const KPostItemInfo&, kPostItem, vecPostItemInfo )
	{
		GetRewardItem( kPostItem, mapRewardItem );
	}
}

bool KEventRewardDataRefreshManager::GetRewardInfo( IN int iRewardID, OUT std::map< int, int >& mapRewardInfo ) const
{
	std::map< int, std::vector< KRewardData > >::const_iterator mit;
	mit = m_mapRewardData.find( iRewardID );
	if( mit == m_mapRewardData.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� RewardID�Դϴ�." )
			<< BUILD_LOG( iRewardID )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const KRewardData&, kReward, mit->second )
	{
		std::map< int, int >::iterator mitFindSame;
		mitFindSame = mapRewardInfo.find( kReward.m_iRewardItemID );
		if( mitFindSame == mapRewardInfo.end() )
		{
			mapRewardInfo.insert( std::make_pair( kReward.m_iRewardItemID, kReward.m_iQuantity ) );
		}
		else
		{
			mitFindSame->second += kReward.m_iQuantity;
		}
	}

	return true;
}

void KEventRewardDataRefreshManager::GetRewardInfo( IN int iRewardType, IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::map< int, int >& mapRewardInfo ) const
{
	mapRewardInfo.clear();

	BOOST_TEST_FOREACH( const KPostItemInfo&, kPostItem, vecPostItemInfo )
	{
		if( kPostItem.m_cScriptType != static_cast<char>(iRewardType) )
			continue;

		if( GetRewardInfo( kPostItem.m_iScriptIndex, mapRewardInfo ) == false )
			continue;
	}
}

void KEventRewardDataRefreshManager::GetRewardInfo( IN int iRewardType, IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::vector< std::wstring >& vecMessage ) const
{
	vecMessage.clear();

	BOOST_TEST_FOREACH( const KPostItemInfo&, kPostItem, vecPostItemInfo )
	{
		if( kPostItem.m_cScriptType != static_cast<char>(iRewardType) )
			continue;

		vecMessage.push_back( kPostItem.m_wstrMessage );
	}
}

void KEventRewardDataRefreshManager::UpdateRewardData( void )
{
	UidType						anTrace[2] = { 0, -1 };
	KEventPtr					spEvent( new KEvent );

	spEvent->SetData( PC_LOG_DB, anTrace, DBE_EVENT_REWARD_DATA_UPDATE_REQ );

	LIF( spEvent->m_kDestPerformer.AddUID( 0 ) );

	SiKDBLayer()->QueueingEvent( spEvent );
}

void KEventRewardDataRefreshManager::SetRewardData( IN const std::map< int, std::vector< KRewardData > >& mapRewardData )
{
	m_mapRewardData.clear();
	m_mapRewardData	= mapRewardData;
}

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}