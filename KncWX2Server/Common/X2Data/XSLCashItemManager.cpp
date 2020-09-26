#include ".\xslcashitemmanager.h"
#include "BaseServer.h"
#include "../Common/KAutoPath.h"
#include "Enum/Enum.h"

ImplementSingleton( CXSLCashItemManager );


CXSLCashItemManager::CXSLCashItemManager(void) : 
m_bIsUpdate( false ),
m_ulProductNoResStone( 0 )
{
	//lua_tinker::class_<CXSLCashItemManager>("CXSLCashItemManager")
	//	.def("refresh",						CXSLCashItemManager::Refresh_LUA)
	//	.def("AddPackageItemData",			CXSLCashItemManager::AddPackageItemData_LUA)
	//	.def("dump",						CXSLCashItemManager::Dump)
	//	;

	//lua_tinker::decl( "g_pCashItemManager", this );
	lua_tinker::class_add<CXSLCashItemManager>( g_pLua, "CXSLCashItemManager" );
	lua_tinker::class_def<CXSLCashItemManager>( g_pLua, "AddPackageItemData",			&CXSLCashItemManager::AddPackageItemData_LUA );
	lua_tinker::class_def<CXSLCashItemManager>( g_pLua, "AddCashItemPrice",				&CXSLCashItemManager::AddCashItemPrice_LUA );

	//{{ 2011.2.27 ���� : ĳ�� �ƹ�Ÿ ���ؽ� �߰� ������ �������� �ϱ�
#ifdef SERV_CASHITEM_RESOLVE_ADD
	lua_tinker::class_def<CXSLCashItemManager>( g_pLua, "AddCashItemGroup",		&CXSLCashItemManager::AddCashItemGroup_LUA );
	lua_tinker::class_def<CXSLCashItemManager>( g_pLua, "AddCashItemResolveAdd",		&CXSLCashItemManager::AddCashItemResolveAdd_LUA );
#endif
	//}}

	//{{ 2009. 12. 19  ������	������ Ȯ��
	lua_tinker::class_def<CXSLCashItemManager>( g_pLua, "AddCashItemServerGroupData",	&CXSLCashItemManager::AddCashItemServerGroupData_LUA );	
	//}}

	//{{ 2011. 01. 04  ��μ�	product no �� ������ Ȯ��
#ifdef SERV_SERVER_DIVISION_CASHITEM_BY_PRODUCTNO
	lua_tinker::class_def<CXSLCashItemManager>( g_pLua, "AddCashItemProductNoData",		&CXSLCashItemManager::AddCashItemProductNoData_LUA );	
#endif SERV_SERVER_DIVISION_CASHITEM_BY_PRODUCTNO
	//}}

	lua_tinker::class_def<CXSLCashItemManager>( g_pLua, "dump",							&CXSLCashItemManager::Dump );

	lua_tinker::decl( g_pLua, "g_pCashItemManager", this );
}

CXSLCashItemManager::~CXSLCashItemManager(void)
{
	m_mapPackageData.clear();
}

ImplToStringW( CXSLCashItemManager )
{
	stm_	<< L"----------[ Cash Item Manager ]----------" << std::endl
			<< L"Package Data Num : "  << m_mapPackageData.size() << std::endl
		;

	return stm_;
}

bool CXSLCashItemManager::OpenScriptFile( const char* pFileName )
{
	_JIF( 0 == LUA_DOFILE( g_pLua, pFileName ), return false );
	return true;
}

bool CXSLCashItemManager::AddPackageItemData_LUA( int iPackageItemID, int iItemID, unsigned short usProductPieces, bool bShowItem )
{
	if( iPackageItemID <= 0 || iItemID <= 0 )
	{
		START_LOG( cerr, L"ĳ�� ��Ű�� ������ ���� �б� ����!" )
			<< BUILD_LOG( iPackageItemID )
			<< BUILD_LOG( usProductPieces )
			<< END_LOG;
		return false;
	}

	// ��Ű�� ����
	PackageItemData kPackageItemData;
	kPackageItemData.m_iItemID		   = iItemID;
	kPackageItemData.m_usProductPieces = usProductPieces;
	//kPackageItemData.m_bShowItem	   = bShowItem; ���������� ������ �ʴ´�.

	std::map< int, std::vector< PackageItemData > >::iterator mit;
	mit = m_mapPackageData.find( iPackageItemID );
	if( mit == m_mapPackageData.end() )
	{
		std::vector< PackageItemData > vecPackageData;		
		vecPackageData.push_back( kPackageItemData );
		m_mapPackageData.insert( std::make_pair( iPackageItemID, vecPackageData ) );
	}
	else
	{
		// �ߺ� ProductNo �˻�
		std::vector< PackageItemData >::const_iterator vit;
		for( vit = mit->second.begin(); vit != mit->second.end(); ++vit )
		{
			if( vit->m_iItemID == iItemID )
			{
				START_LOG( cerr, L"�ϳ��� ��Ű���� �ߺ��Ǵ� ������ǰ ItemID�� �����մϴ�." )
					<< BUILD_LOG( iPackageItemID )
					<< BUILD_LOG( iItemID )
					<< END_LOG;
				return false;
			}
		}

		mit->second.push_back( kPackageItemData );
	}

	return true;
}

bool CXSLCashItemManager::AddCashItemPrice_LUA( int iItemID, int iPrice )
{
	if( iItemID <= 0  ||  iPrice <= 0 )
	{
		START_LOG( cerr, L"�ؽ� ���� ��ǰ ���� ������ �̻��մϴ�." )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iPrice )
			<< END_LOG;
		return false;
	}

	m_mapCashItemPriceFromScript.insert( std::make_pair( iItemID, iPrice ) );
	return true;
}


//{{ 2011.2.27 ���� : ĳ�� �ƹ�Ÿ ���ؽ� �߰� ������ �������� �ϱ�
#ifdef SERV_CASHITEM_RESOLVE_ADD
bool CXSLCashItemManager::AddCashItemGroup_LUA( int iGroupID, int iItemID)
{
	std::map<int, int>::iterator mit; 
	mit = m_mapCashItemGroup.find( iItemID );

	if( mit != m_mapCashItemGroup.end() )  // ������ ��ϵ� ���� �������� ������? ����
	{
		START_LOG( cerr, L"�̹� ��� �� ������ �Դϴ�." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	m_mapCashItemGroup.insert( std::make_pair( iItemID, iGroupID ) );

	return true;
}

bool CXSLCashItemManager::AddCashItemResolveAdd_LUA( int iGroupID, int iItemID, int iQuantity, float fProbability )
{
	// �׷� ���� üũ ����
	// �׷��� second�� �־����� �� ������.. �׳� ã�� �Լ� ����?
	std::map<int, int>::iterator mit; 
	for(mit = m_mapCashItemGroup.begin(); mit != m_mapCashItemGroup.end(); mit++)
	{	
		if( mit == m_mapCashItemGroup.end() )
		{
			START_LOG( cerr, L"ĳ�� ������ �׷� ������ �̻��մϴ�." )
				<< BUILD_LOG( iGroupID )
				<< END_LOG;
			return false;
		}
	}

	if( iItemID <= 0 )
	{
		START_LOG( cerr, L"�ؽ� ���� ��ǰ ���� ������ �̻��մϴ�." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}


	std::map<int,VecResolveInfo>::iterator mit2;
	mit2 = m_mapCashItemResolveGroup.find( iGroupID );

	if( mit2 == m_mapCashItemResolveGroup.end() )  // ������ ��ϵ� ���� �׷� �����Ͱ� ���� - ���� �߰�.
	{
		VecResolveInfo vecInfo;
		vecInfo.clear();

		m_mapCashItemResolveGroup.insert( std::make_pair( iGroupID, vecInfo ) );
		mit2 = m_mapCashItemResolveGroup.find( iGroupID );
	}

	ResolveAddInfo info;
	info.iItemID = iItemID;
	info.iQuantity = iQuantity;
	info.kLottery.AddCase(RESOLVE_SUCCESS, fProbability);

	mit2->second.push_back(info);

	return true;
}

bool CXSLCashItemManager::GetCashItemResolveAddFromScript( int iItemID, VecResolveInfo& vecItem )
{
	vecItem.clear();

	// �켱 �׷��� ã��
	std::map<int, int>::iterator mit;
	mit = m_mapCashItemGroup.find(iItemID);

	if(mit == m_mapCashItemGroup.end())
	{
		// ���� ��� �ȵǴ� �����۵鵵 �����Ƿ� ���� �α״� ������ �ʴ´�.
		/*
		START_LOG( cerr, L"�׷� ����� �ȵ� ������ �Դϴ�." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		*/
		return false;
	}

	int group = mit->second;

	std::map< int, VecResolveInfo >::const_iterator mit2;
	mit2 = m_mapCashItemResolveGroup.find( group );
	if( mit2 == m_mapCashItemResolveGroup.end() )
	{
		START_LOG( cerr, L"�׷쿡 ��ϵ� ��� �������� �����ϴ�." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return false;
	}

	// ��� ������ ���������, �ش� �׷��� ��� �����۵鿡 ���� ��� Ȯ������� �ؼ�
	// ���� ������ �͵��� vecItem �� ��Ƽ� �Ѱ�����.
	VecResolveInfo vecInfo = mit2->second;
	VecResolveInfo::iterator vit;
	for(vit = vecInfo.begin(); vit != vecInfo.end(); vit++)
	{
		ResolveAddInfo info = (*vit);
		if(info.kLottery.Decision() == RESOLVE_SUCCESS)
		{
			// ���� ������ �ִٸ� ������ �����ش�.... �׳� �ϸ� ��� �Ǵ��� �ϴ� ����.
			VecResolveInfo::iterator vit2;
			for(vit2 = vecItem.begin(); vit2 != vecItem.end(); vit2++)
			{
				if( info.iItemID == vit2->iItemID )
				{
					vit2->iQuantity += info.iQuantity;
					break;
				}
			}
			if(vit2 == vecItem.end() )
				vecItem.push_back(info);
		}
	}

	return true;
}
#endif SERV_CASHITEM_RESOLVE_ADD
//}}


//{{ 2009. 12. 19  ������	������ Ȯ��
bool CXSLCashItemManager::AddCashItemServerGroupData_LUA( int iServerGroupID, int iItemID )
{
	if( !SEnum::IsValidServerGroupID( iServerGroupID )  ||  iItemID <= 0 )
	{
		START_LOG( cerr, L"�������� ĳ���� �Ⱥ��̴� ��ǰ �������� �̻��մϴ�." )
			<< BUILD_LOG( iServerGroupID )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return false;
	}

	if( KBaseServer::GetKObj()->GetServerGroupID() != iServerGroupID )
		return true;

	// �ǸŵǾ�� �ȵǴ� ������ ����Ʈ
	m_setNotSellCashItem.insert( iItemID );
	return true;
}
//}}

//{{ 2011. 01. 04  ��μ�	product no �� ������ Ȯ��
#ifdef SERV_SERVER_DIVISION_CASHITEM_BY_PRODUCTNO
bool CXSLCashItemManager::AddCashItemProductNoData_LUA( int iServerGroupID, unsigned long ulProductNo )
{
	if( !SEnum::IsValidServerGroupID( iServerGroupID )  ||  ulProductNo <= 0 )
	{
		START_LOG( cerr, L"ProductNo�� ĳ���� �Ⱥ��̴� ��ǰ �������� �̻��մϴ�." )
			<< BUILD_LOG( iServerGroupID )
			<< BUILD_LOG( ulProductNo )
			<< END_LOG;

		return false;
	}
	// CashItemData.lua ���� ���� - �ش� ������ ĳ���� ǥ����� �ʵ���
	// m_setCashItemProductNoData �� �ִ� item�� ĳ������ ������ �ȵȴ�.
	if( !KBaseServer::GetKObj()->GetServerGroupID() != iServerGroupID )
		return true;

	m_setNotSellCashItemProductNo.insert( ulProductNo );
	return true;
}
#endif SERV_SERVER_DIVISION_CASHITEM_BY_PRODUCTNO
//}}

bool CXSLCashItemManager::GetPackageData( int iPackageItemID, std::vector< PackageItemData >& vecPackageItem ) const
{
	vecPackageItem.clear();

	std::map< int, std::vector< PackageItemData > >::const_iterator mit;
	mit = m_mapPackageData.find( iPackageItemID );
	if( mit == m_mapPackageData.end() )
		return false;

	vecPackageItem = mit->second;
	return true;
}

//{{ 2008. 5. 15  ������  �Ǹ����� ��ǰ���� �˻� ItemID
void CXSLCashItemManager::UpdateProductList( std::vector< int >& vecProductItemID, std::vector< KCashItemResolveInfo >& vecCashResolveInfo, unsigned long ulProductNoResStone )
{
	m_setProductItemID.clear();
	m_mapCashItemPrice.clear();

	std::vector< int >::const_iterator vit;
	for( vit = vecProductItemID.begin(); vit != vecProductItemID.end(); ++vit )
	{
		m_setProductItemID.insert( *vit );
	}

	if( !m_setProductItemID.empty() )
		m_bIsUpdate = true;

	//{{ 2008. 6. 10  ������  �ǽð� ��Ȱ��
	m_ulProductNoResStone = ulProductNoResStone;
	//}}

	//std::map< int, int >::const_iterator mit;
	//std::vector< KCashItemResolveInfo >::const_iterator vitCR;
	//for( vitCR = vecCashResolveInfo.begin(); vitCR != vecCashResolveInfo.end(); ++vitCR )
	//{
	//	mit = m_mapCashItemPrice.find( vitCR->m_iItemID );
	//	if( mit != m_mapCashItemPrice.end() )
	//	{
	//		START_LOG( cerr, L"���? ĳ�� ���� ������ ������Ʈ �ϴ��߿� �ߺ��Ǵ� ItemID�� �ֳ�?" )
	//			<< BUILD_LOG( vitCR->m_iItemID )
	//			<< BUILD_LOG( vitCR->m_iSalePrice )
	//			<< BUILD_LOG( mit->first )
	//			<< BUILD_LOG( mit->second )
	//			<< END_LOG;
	//	}

	//	m_mapCashItemPrice.insert( std::make_pair( vitCR->m_iItemID, vitCR->m_iSalePrice ) );
	//}
}

bool CXSLCashItemManager::IsBuyAvailable( int iItemID )
{
	if( !m_bIsUpdate )
		return true;

	std::set< int >::const_iterator sit;
	sit = m_setProductItemID.find( iItemID );
	if( sit == m_setProductItemID.end() )
		return false;

	return true;
}
//}}

//{{ 2008. 9. 23  ������	ĳ������
//bool CXSLCashItemManager::GetCashItemPrice( int iItemID, int& iPrice )
//{
//	std::map< int, int >::const_iterator mit;
//	mit = m_mapCashItemPrice.find( iItemID );
//	if( mit == m_mapCashItemPrice.end() )
//		return false;
//
//	iPrice = mit->second;
//	return true;
//}

bool CXSLCashItemManager::GetCashItemPriceFromScript( int iItemID, int& iPrice )
{
	std::map< int, int >::const_iterator mit;
	mit = m_mapCashItemPriceFromScript.find( iItemID );
	if( mit == m_mapCashItemPriceFromScript.end() )
		return false;

	iPrice = mit->second;
	return true;
}
//}}


