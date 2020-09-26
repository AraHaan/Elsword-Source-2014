#include ".\rewardtable.h"
#include "X2Data/XSLItemManager.h"


ImplementRefreshSingleton( KRewardTable );

KRewardTable::KRewardTable(void)
{
}

KRewardTable::~KRewardTable(void)
{
}

ImplToStringW( KRewardTable )
{
	stm_	<< L"----------[ Reward Table ]----------" << std::endl
			<< TOSTRINGW( m_mapRewardData.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( KRewardTable )
{
	lua_tinker::class_add<KRewardTable>( GetLuaState(), "KRewardTable" );
	//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL
	lua_tinker::class_def<KRewardTable>( GetLuaState(), "SetOutUserRetainingRewardItemInfo",	&KRewardTable::SetOutUserRetainingRewardItemInfo_LUA );
#endif SERV_NEXON_SESSION_PROTOCOL
	//}} 
	lua_tinker::class_def<KRewardTable>( GetLuaState(), "AddRewardData",	&KRewardTable::AddRewardData_LUA );
	lua_tinker::class_def<KRewardTable>( GetLuaState(), "dump",				&KRewardTable::Dump );

	//{{ 2011. 08. 29	������	ĳ���� ������ �̺�Ʈ
#ifdef SERV_CHAR_LEVEL_UP_EVENT
	lua_tinker::class_def<KRewardTable>( GetLuaState(), "AddLevelUpRewardData",	&KRewardTable::AddLevelUpRewardData_LUA );
#endif SERV_CHAR_LEVEL_UP_EVENT
	//}}
	
	lua_tinker::decl( GetLuaState(), "g_pKRewardTable", this );
}

bool KRewardTable::AddRewardData_LUA( int iRewardID, int iItemID, int iQuantity, short sPeriod )
{
	if( iRewardID <= 0  ||  iItemID <= 0  ||  iQuantity <= 0  ||  sPeriod < 0 )
	{
		START_LOG( cerr, L"�߸��� ��ũ��Ʈ �����Դϴ�." )
			<< BUILD_LOG( iRewardID )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iQuantity )
			<< BUILD_LOG( sPeriod )
			<< END_LOG;
		return false;
	}

	KRewardData kRewardData;
	kRewardData.m_iRewardItemID = iItemID;
	kRewardData.m_iQuantity		= iQuantity;
	kRewardData.m_sPeriod		= sPeriod;	

	std::map< int, std::vector< KRewardData > >::iterator mit;
	mit = m_mapRewardData.find( iRewardID );
	if( mit == m_mapRewardData.end() )
	{
		// ���ٸ�..
		std::vector< KRewardData > vecRewardData;
		vecRewardData.push_back( kRewardData );
		m_mapRewardData.insert( std::make_pair( iRewardID, vecRewardData ) );
	}
	else
	{
		mit->second.push_back( kRewardData );
	}

	return true;;
}

void KRewardTable::GetRewardItem( IN const KPostItemInfo& kPostItemInfo, OUT std::map< int, KItemInfo >& mapRewardItem ) const
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

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	mit = m_mapTotalRewardData.find( kPostItemInfo.m_iScriptIndex );
	if( mit == m_mapTotalRewardData.end() )
#else //SERV_EVENT_DB_CONTROL_SYSTEM
	mit = m_mapRewardData.find( kPostItemInfo.m_iScriptIndex );
	if( mit == m_mapRewardData.end() )
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
	
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

void KRewardTable::GetRewardItem( IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::map< int, KItemInfo >& mapRewardItem ) const
{
	mapRewardItem.clear();

	BOOST_TEST_FOREACH( const KPostItemInfo&, kPostItem, vecPostItemInfo )
	{
		GetRewardItem( kPostItem, mapRewardItem );
	}
}

bool KRewardTable::GetRewardInfo( IN int iRewardID, OUT std::map< int, int >& mapRewardInfo ) const
{
	std::map< int, std::vector< KRewardData > >::const_iterator mit;

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	mit = m_mapTotalRewardData.find( iRewardID );
	if( mit == m_mapTotalRewardData.end() )
#else //SERV_EVENT_DB_CONTROL_SYSTEM
	mit = m_mapRewardData.find( iRewardID );
	if( mit == m_mapRewardData.end() )
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
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

void KRewardTable::GetRewardInfo( IN int iRewardType, IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::map< int, int >& mapRewardInfo ) const
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

void KRewardTable::GetRewardInfo( IN int iRewardType, IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::vector< std::wstring >& vecMessage ) const
{
	vecMessage.clear();

	BOOST_TEST_FOREACH( const KPostItemInfo&, kPostItem, vecPostItemInfo )
	{
		if( kPostItem.m_cScriptType != static_cast<char>(iRewardType) )
			continue;

		vecMessage.push_back( kPostItem.m_wstrMessage );
	}
}

//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL
void KRewardTable::SetOutUserRetainingRewardItemInfo_LUA( int iSlot, int iItemID, int iQuantity )
{
	KRewardItem tempItem;
	tempItem.m_iItemID = iItemID;
	tempItem.m_iQuantity = iQuantity;

	m_mapRewardItem.insert( std::make_pair( iSlot, tempItem ) );

	START_LOG( cout, L"��Ż ���� ���� ������ ���� ���� �Ϸ�!")
		<< BUILD_LOG( iSlot )
		<< BUILD_LOG( iItemID )
		<< BUILD_LOG( iQuantity );
}

void KRewardTable::GetOutUserRetainingRewardItemInfo( std::map< int, KRewardItem >& mapRewardItem )
{
	std::map< int, KRewardItem >::iterator mit = m_mapRewardItem.begin();

	if( mit != m_mapRewardItem.end() )
	{
		mapRewardItem = m_mapRewardItem;
	}
}

bool KRewardTable::GetOutUserRetainingSelectItemInfo( IN int& iSlot, OUT KRewardItem& kInfo )
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

bool KRewardTable::GetOutUserRetainingSelectItemInfo( IN int& iItemID, OUT int& iQuantity )
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
#endif SERV_NEXON_SESSION_PROTOCOL
//}}

//{{ 2011. 08. 29	������	ĳ���� ������ �̺�Ʈ
#ifdef SERV_CHAR_LEVEL_UP_EVENT
bool KRewardTable::AddLevelUpRewardData_LUA( u_char ucLevel, int iRewardID )
{
	if( iRewardID <= 0 || ucLevel < 0 )
	{
		START_LOG( cerr, L"�߸��� ��ũ��Ʈ �����Դϴ�." )
			<< BUILD_LOG( iRewardID )
			<< BUILD_LOGc( ucLevel )
			<< END_LOG;
		return false;
	}

	std::map< u_char, std::vector< int > >::iterator mit;
	mit = m_mapLevelUpRewardItem.find( ucLevel );
	if( mit == m_mapLevelUpRewardItem.end() )
	{
		std::vector< int > vecRewardID;
		vecRewardID.push_back( iRewardID );
		m_mapLevelUpRewardItem.insert( std::make_pair( ucLevel, vecRewardID ) );
	}
	else
	{
		std::vector< int >::iterator vit = mit->second.begin();
		for( ; vit != mit->second.end() ; ++vit )
		{
			if( *vit == iRewardID )
			{
				START_LOG( cerr, L"�ߺ��Ǵ� ������ ���� �������� �����մϴ�." )
					<< BUILD_LOGc( ucLevel )
					<< BUILD_LOG( iRewardID )
					<< BUILD_LOG( *vit )
					<< END_LOG;

				return false;
			}
		}

		mit->second.push_back( iRewardID );
	}

	return true;
}

bool KRewardTable::GetLevelUpRewardItem( IN u_char& ucLevel, OUT std::vector< int >& vecRewardItem )
{
	std::map< u_char, std::vector< int > >::iterator mit;
	mit = m_mapLevelUpRewardItem.find( ucLevel );
	if( mit == m_mapLevelUpRewardItem.end() )
	{
		START_LOG( clog , L"������ ������ ������ ����." )
			<< BUILD_LOGc( ucLevel )
			<< END_LOG;

		return false;
	}
	
	std::vector< int >::iterator vit = mit->second.begin();
	if( vit == mit->second.end() )
	{
		return false;
	}

	vecRewardItem = mit->second;

	return true;
}
#endif SERV_CHAR_LEVEL_UP_EVENT
//}}

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
void KRewardTable::SetMapTotalRewardData(  IN const std::map< int, std::vector< KRewardData > > mapRewardScriptData, IN const std::map< int, std::vector< KRewardData > > mapRewardDBData  )
{
	m_mapTotalRewardData.clear();
	std::map< int, std::vector< KRewardData > >::const_iterator cmitRewardScript = mapRewardScriptData.begin();

	for (; cmitRewardScript != mapRewardScriptData.end(); ++cmitRewardScript )
	{
		std::map< int, std::vector< KRewardData > >::iterator mitTotalRewardData = m_mapTotalRewardData.find(cmitRewardScript->first);

		if ( mitTotalRewardData == m_mapTotalRewardData.end() )
		{
			// 2013.11.01 darkstarbt_������ // ���� RewardTable.lua �� �ִ� �����ʹ� ��ũ��Ʈ �Ľ� �Ҷ� �̹� �ʿ� ���� ���� ������ �� ��� �ִ� ���´�
			m_mapTotalRewardData.insert(std::make_pair(cmitRewardScript->first, cmitRewardScript->second));
		}
	}

	std::map< int, std::vector< KRewardData > >::const_iterator cmitRewardDB = mapRewardDBData.begin();

	for (; cmitRewardDB != mapRewardDBData.end(); ++cmitRewardDB )
	{
		std::map< int, std::vector< KRewardData > >::iterator mitTotalRewardData = m_mapTotalRewardData.find(cmitRewardDB->first);

		if ( mitTotalRewardData == m_mapTotalRewardData.end() )
		{
			m_mapTotalRewardData.insert(std::make_pair(cmitRewardDB->first, cmitRewardDB->second));
		}
		else
		{
			START_LOG( cerr, L"RewardTable.lua���� ���� RewardID �� DB �̺�Ʈ ���� ���� �ϴ� RewardID �� �ߺ��Ǿ����ϴ�. �ش� DB ������ ���� ���� �ʾҽ��ϴ�." )
				<< BUILD_LOG( mitTotalRewardData->first )
				;
		}
	}
}
#endif //SERV_EVENT_DB_CONTROL_SYSTEM