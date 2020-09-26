#include "GlobalSimLayer.h"
#include "BaseServer.h"
#include "BlockListManager.h"


//{{ 2013. 05. 14	������	������ ����
#ifdef SERV_BLOCK_LIST


ImplementRefreshSingleton( KBlockListManager );

KBlockListManager::KBlockListManager(void) :
m_iReleaseTick( 0 )
{
}

KBlockListManager::~KBlockListManager(void)
{
}

ImplToStringW( KBlockListManager )
{
	stm_	<< L"----------[ KBlockListManager ]----------" << std::endl
			//<< TOSTRINGW( m_bActive )
			;

	return stm_;
}

ImplementLuaScriptParser( KBlockListManager )
{

	lua_tinker::class_add<KBlockListManager>( GetLuaState(), "KBlockListManager" );
	lua_tinker::class_def<KBlockListManager>( GetLuaState(), "dump",							&KBlockListManager::Dump );

	lua_tinker::decl( GetLuaState(), "BlockListManager", this );
}

void KBlockListManager::Init()
{
	// ���� ���ڸ��� �� ����Ʈ ��������
	KDBE_CHECK_BLOCK_LIST_REQ kPacketToDB;
	kPacketToDB.m_iReleaseTick = GetReleaseTick();
	SendToAccountDB( DBE_CHECK_BLOCK_LIST_REQ, kPacketToDB );
}

void KBlockListManager::Tick()
{
	if( m_tTimer.elapsed() < 60.0 )
		return;

	m_tTimer.restart();

	// 1�п� �ѹ��� releasetick���� üũ�Ϸ� DB�� ���ϴ�.
	KDBE_CHECK_BLOCK_LIST_REQ kPacketToDB;
	kPacketToDB.m_iReleaseTick = GetReleaseTick();
	SendToAccountDB( DBE_CHECK_BLOCK_LIST_REQ, kPacketToDB );
}

void KBlockListManager::UpdateblockList( IN const int iReleaseTick, IN const std::map< int, std::vector< KBlockInfo > >& mapBlockInfo, OUT std::vector< KBlockInfo >& vecNewBlock, OUT std::vector< KBlockInfo >& vecDelBlock )
{
	vecNewBlock.clear();
	vecDelBlock.clear();

	if( GetReleaseTick() > iReleaseTick )
	{
		START_LOG( cerr, L"DB�κ��� �޾ƿ� ReleaseTick���� �̻��մϴ�." )
			<< BUILD_LOG( GetReleaseTick() )
			<< BUILD_LOG( iReleaseTick )
			<< BUILD_LOG( mapBlockInfo.size() )
			<< END_LOG;
		return;
	}

	// DB�κ��� ���� Tick���� ���� Tick���� ������ ��ȭ�� ������ ���ٴ� �ǹ��̴�.
	if( GetReleaseTick() == iReleaseTick )
		return;

	// ������ƽ ������Ʈ
	SetReleaseTick( iReleaseTick );

	// ���� �ִ� ����Ʈ�� ���Ѵ�.	
	for( int iType = 0; iType < KBlockInfo::BT_MAX; ++iType )
	{
		std::vector< KBlockInfo > vecBlockInfo;		

		std::map< int, std::vector< KBlockInfo > >::const_iterator mit;
		mit = mapBlockInfo.find( iType );
		if( mit != mapBlockInfo.end() )
		{
			vecBlockInfo = mit->second;
		}

		switch( iType )
		{
		case KBlockInfo::BT_TRADE_BLOCK:
			{
				UpdateTradeBlock( vecBlockInfo, vecNewBlock, vecDelBlock );
			}
			break;

		case KBlockInfo::BT_ACCOUNT_BLOCK:
			{
				UpdateAccountBlock( vecBlockInfo, vecNewBlock, vecDelBlock );
			}
			break;

		case KBlockInfo::BT_IP_BLOCK:
			{
				UpdateIpBlock( vecBlockInfo );
			}
			break;

		case KBlockInfo::BT_DLL_BLOCK:
			{
				UpdateModuleList( vecBlockInfo );
			}
			break;

		default:
			{
				START_LOG( cerr, L"��ϵ��� ���� ���Ÿ���Դϴ�." )
					<< BUILD_LOGc( iType )
					<< END_LOG;
			}
			break;
		}
	}
}

void KBlockListManager::UpdateTradeBlock( IN const std::vector< KBlockInfo >& vecBlockInfo, OUT std::vector< KBlockInfo >& vecNewBlock, OUT std::vector< KBlockInfo >& vecDelBlock )
{
	// ���ΰŷ� ���� �ǽð����� ���� Ǯ������ϴ� �������� ������ �׸� ���ؼ� ����Ʈ�� ���� ��������.

	std::set< UidType > setUpdateBlock;	// DB�κ��� �޾ƿ� �� ��� ����Ʈ
	std::set< UidType > setDelBlock;	// ���� ������ �� �� ��� ����Ʈ

	BOOST_TEST_FOREACH( const KBlockInfo&, kInfo, vecBlockInfo )
	{
		const UidType iTradeBlockUserUID = kInfo.m_iBlockTarget;

		// DB�� ���� �޾ƿ� �����͵��� ���� ����Ʈ�� ����
		setUpdateBlock.insert( iTradeBlockUserUID );

		std::map< UidType, KBlockData >::iterator mit;
		mit = m_mapTradeBlock.find( iTradeBlockUserUID );
		if( mit == m_mapTradeBlock.end() )
		{
			m_mapTradeBlock.insert( std::make_pair( iTradeBlockUserUID, KBlockData( kInfo ) ) );

			// �ش� ���� ���� �Ǿ����� Ȯ��
			//if( kNewBlock.IsActiveBlock() == true )
			//{
				vecNewBlock.push_back( kInfo );
			//}
		}
		else
		{
			// ���� ���¸� ����
			//const bool bBeforeActive = mit->second.IsActiveBlock();

			// ���ο� �������� ������Ʈ
			mit->second.SetBlockInfo( kInfo );

			// ���� Ȱ��ȭ �Ǵ°ǰ�?
			//if( bBeforeActive == false  &&  mit->second.IsActiveBlock() == true )
			//{
			//	vecNewBlock.push_back( kInfo );
			//}
			//else if( bBeforeActive == true  &&  mit->second.IsActiveBlock() == false )
			//{
			//	vecDelBlock.push_back( kInfo );
			//}
		}
	}

	std::map< UidType, KBlockData >::const_iterator mit;
	for( mit = m_mapTradeBlock.begin(); mit != m_mapTradeBlock.end(); ++mit )
	{
		if( setUpdateBlock.find( mit->first ) != setUpdateBlock.end() )
			continue;

		setDelBlock.insert( mit->first );
	}

	BOOST_TEST_FOREACH( const UidType, iBlockTarget, setDelBlock )
	{
		std::map< UidType, KBlockData >::iterator mit;
		mit = m_mapTradeBlock.find( iBlockTarget );
		if( mit == m_mapTradeBlock.end() )
			continue;

		// �� ���� ���
		KBlockInfo kInfo;
		mit->second.GetBlockInfo( kInfo );
		vecDelBlock.push_back( kInfo );

		// ���� ����Ʈ������ ����
		m_mapTradeBlock.erase( iBlockTarget );
	}
}

void KBlockListManager::UpdateAccountBlock( IN const std::vector< KBlockInfo >& vecBlockInfo, OUT std::vector< KBlockInfo >& vecNewBlock, OUT std::vector< KBlockInfo >& vecDelBlock )
{
	// ���ΰŷ� ���� �ǽð����� ���� Ǯ������ϴ� �������� ������ �׸� ���ؼ� ����Ʈ�� ���� ��������.

	std::set< UidType > setUpdateBlock;	// DB�κ��� �޾ƿ� �� ��� ����Ʈ
	std::set< UidType > setDelBlock;	// ���� ������ �� �� ��� ����Ʈ

	BOOST_TEST_FOREACH( const KBlockInfo&, kInfo, vecBlockInfo )
	{
		const UidType iBlockUserUID = kInfo.m_iBlockTarget;

		// DB�� ���� �޾ƿ� �����͵��� ���� ����Ʈ�� ����
		setUpdateBlock.insert( iBlockUserUID );

		std::map< UidType, KBlockData >::iterator mit;
		mit = m_mapAccountBlock.find( iBlockUserUID );
		if( mit == m_mapAccountBlock.end() )
		{
			m_mapAccountBlock.insert( std::make_pair( iBlockUserUID, KBlockData( kInfo ) ) );

			vecNewBlock.push_back( kInfo );
		}
		else
		{
			// ���ο� �������� ������Ʈ
			mit->second.SetBlockInfo( kInfo );
		}
	}

	std::map< UidType, KBlockData >::const_iterator mit;
	for( mit = m_mapAccountBlock.begin(); mit != m_mapAccountBlock.end(); ++mit )
	{
		if( setUpdateBlock.find( mit->first ) != setUpdateBlock.end() )
			continue;

		setDelBlock.insert( mit->first );
	}

	BOOST_TEST_FOREACH( const UidType, iBlockTarget, setDelBlock )
	{
		std::map< UidType, KBlockData >::iterator mit;
		mit = m_mapAccountBlock.find( iBlockTarget );
		if( mit == m_mapAccountBlock.end() )
			continue;

		// �� ���� ���
		KBlockInfo kInfo;
		mit->second.GetBlockInfo( kInfo );
		vecDelBlock.push_back( kInfo );

		// ���� ����Ʈ������ ����
		m_mapAccountBlock.erase( iBlockTarget );
	}



	m_mapAccountBlock.clear();

	BOOST_TEST_FOREACH( const KBlockInfo&, kInfo, vecBlockInfo )
	{
		m_mapAccountBlock.insert( std::make_pair( kInfo.m_iBlockTarget, KBlockData( kInfo ) ) );
	}
}

void KBlockListManager::UpdateIpBlock( IN const std::vector< KBlockInfo >& vecBlockInfo )
{
	m_mapIpBlock.clear();

	BOOST_TEST_FOREACH( const KBlockInfo&, kInfo, vecBlockInfo )
	{
		const std::string strIP = KncUtil::toNarrowString( kInfo.m_wstrBlockTarget );
		const u_int uiIP = ::inet_addr( strIP.c_str() );

		m_mapIpBlock.insert( std::make_pair( uiIP, KBlockData( kInfo ) ) );
	}
}

bool KBlockListManager::IsTradeBlockUser( IN const UidType iUserUID ) const
{
	std::map< UidType, KBlockData >::const_iterator mit;
	mit = m_mapTradeBlock.find( iUserUID );
	if( mit == m_mapTradeBlock.end() )
	{
		return false;
	}

	return mit->second.IsActiveBlock();
}

bool KBlockListManager::IsAccountBlockUser( IN const UidType iUserUID ) const
{
	std::map< UidType, KBlockData >::const_iterator mit;
	mit = m_mapAccountBlock.find( iUserUID );
	if( mit == m_mapAccountBlock.end() )
	{
		return false;
	}

	return mit->second.IsActiveBlock();
}

bool KBlockListManager::IsIpBlockUser( IN const std::wstring& wstrIP ) const
{
	const std::string strIP = KncUtil::toNarrowString( wstrIP );
	const u_int uiIP = ::inet_addr( strIP.c_str() );

	std::map< u_int, KBlockData >::const_iterator mit;
	mit = m_mapIpBlock.find( uiIP );
	if( mit == m_mapIpBlock.end() )
	{
		return false;
	}

	return mit->second.IsActiveBlock();
}

bool KBlockListManager::AddModuleInfo( IN const KBlockInfo& kInfo )
{
	if( m_mapDllBlock.find( kInfo.m_wstrBlockTarget ) != m_mapDllBlock.end() )
	{
		START_LOG( cwarn, L"�̹� ��ϵǾ��ִ� ���UID�Դϴ�." )
			<< BUILD_LOG( kInfo.m_wstrBlockTarget )
			<< END_LOG;
		return false;
	}

	m_mapDllBlock.insert( std::make_pair( kInfo.m_wstrBlockTarget, KBlockData( kInfo ) ) );
	return true;
}

void KBlockListManager::UpdateModuleInfo( IN const std::wstring& wstrModuleName )
{
	std::map< std::wstring, KBlockData >::iterator mit;
	mit = m_mapDllBlock.find( wstrModuleName );
	if( mit == m_mapDllBlock.end() )
	{
		KBlockInfo kInfo;
		kInfo.m_cBlockType = KBlockInfo::BT_DLL_BLOCK;
		kInfo.m_wstrBlockTarget = wstrModuleName;
		kInfo.m_wstrStartDate = L"2000-01-01 00:00:00";
		kInfo.m_wstrEndDate = L"2050-01-01 00:00:00";
		AddModuleInfo( kInfo );
	}
	//else
	//{
	//	++mit->second.m_iCount;
	//	mit->second.m_bReserveDBUpdate = true;
	//}
}

void KBlockListManager::UpdateModuleList( IN const std::vector< KBlockInfo >& vecModuleList )
{
	BOOST_TEST_FOREACH( const KBlockInfo&, kInfo, vecModuleList )
	{
		UpdateModuleInfo( kInfo.m_wstrBlockTarget );
	}
}

void KBlockListManager::UpdateModuleList( IN const std::vector< std::wstring >& vecModuleList )
{
	BOOST_TEST_FOREACH( const std::wstring&, wstrModuleName, vecModuleList )
	{
		UpdateModuleInfo( wstrModuleName );
	}
}

bool KBlockListManager::CheckHackingModuleList( IN const std::vector< std::wstring >& vecModuleList, OUT std::map< std::wstring, KModuleInfo >& mapModuleName )
{
	bool bCheckResult = true;

	std::map< std::wstring, KBlockData >::iterator mit;

	BOOST_TEST_FOREACH( const std::wstring&, wstrName, vecModuleList )
	{
		mit = m_mapDllBlock.find( wstrName );	
		if( mit != m_mapDllBlock.end() )
		{
			START_LOG( clog, L"Allow ���� Black List �Դϴ�." )
				<< BUILD_LOG( wstrName )
				<< END_LOG;

			bCheckResult = false;

			KModuleInfo kInfo;
			kInfo.m_iModuleUID = static_cast<int>(mit->second.GetBlockUID());
			kInfo.m_wstrModuleName = mit->second.GetBlockTargetString();
			kInfo.m_iCount;
			kInfo.m_iFlag;
			//{{  2012. 1. 30	��μ�		�ѹ��� ���(��ŷ ���μ��� �˻�)
#ifdef SERV_HACKING_DLL_CHECK
			kInfo.m_iAllow;
#else
			kInfo.m_bAllow;
#endif SERV_HACKING_DLL_CHECK
			//}}
			kInfo.m_bReserveDBUpdate;
			mapModuleName.insert( std::make_pair( kInfo.m_wstrModuleName, kInfo ) );
		}
	}

	return bCheckResult;
}

#endif SERV_BLOCK_LIST
//}}


