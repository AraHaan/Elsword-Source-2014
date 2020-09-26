#include ".\xslresolveitemmanager.h"
#include "NetError.h"
#include "XSLItemManager.h"

ImplementRefreshSingleton( CXSLResolveItemManager );

CXSLResolveItemManager::CXSLResolveItemManager(void)
{
	m_iMinJackpotTime	= 0;
	m_iMaxJackpotTime	= 0;
}

CXSLResolveItemManager::~CXSLResolveItemManager(void)
{
}

ImplToStringW( CXSLResolveItemManager )
{
	stm_	<< L"----------[ Resolve Item Manager ]----------" << std::endl
		//{{ 2011. 01. 19	������	������ ���� ����
#ifdef SERV_ITEM_RESOLVE_NEW
			<< TOSTRINGW( m_vecBrokenPieceResolve[RRT_EL_FLOUR].size() )
			<< TOSTRINGW( m_vecBrokenPieceResolve[RRT_WEAPON_BROKEN_PIECE].size() )
			<< TOSTRINGW( m_vecBrokenPieceResolve[RRT_ARMOR_BROKEN_PIECE].size() )
#else
			<< TOSTRINGW( m_mapBrokenPieceResolve.size() )
#endif SERV_ITEM_RESOLVE_NEW
		//}}			
			<< TOSTRINGW( m_mapResolveData.size() )
			<< TOSTRINGW( m_iMinJackpotTime )
			<< TOSTRINGW( m_iMaxJackpotTime )
		;

	return stm_;
}

ImplementLuaScriptParser( CXSLResolveItemManager )
{
	lua_tinker::class_add<CXSLResolveItemManager>( GetLuaState(), "CXSLResolveItemManager" );
	lua_tinker::class_def<CXSLResolveItemManager>( GetLuaState(), "SetJackpotProb",						&CXSLResolveItemManager::SetJackpotProb_LUA );	
	lua_tinker::class_def<CXSLResolveItemManager>( GetLuaState(), "AddResolveDataBrokenPiece",			&CXSLResolveItemManager::AddResolveDataBrokenPiece_LUA );
	lua_tinker::class_def<CXSLResolveItemManager>( GetLuaState(), "AddResolveData",						&CXSLResolveItemManager::AddResolveData_LUA );
	lua_tinker::class_def<CXSLResolveItemManager>( GetLuaState(), "SetMinMaxOfJackpotTime",				&CXSLResolveItemManager::SetMinMaxOfJackpotTime_LUA );
	//{{ 2011. 01. 19	������	������ ���� ����
#ifdef SERV_ITEM_RESOLVE_NEW
	lua_tinker::class_def<CXSLResolveItemManager>( GetLuaState(), "AddWeaponResolveBrokenPieceRate",	&CXSLResolveItemManager::AddWeaponResolveBrokenPieceRate_LUA );
	lua_tinker::class_def<CXSLResolveItemManager>( GetLuaState(), "AddArmorResolveBrokenPieceRate",		&CXSLResolveItemManager::AddArmorResolveBrokenPieceRate_LUA );
#endif SERV_ITEM_RESOLVE_NEW
	//}}
	
	//{{ 2011.03.16   �ӱԼ� �ƹ�Ÿ ���� �ý���
#ifdef SERV_MULTI_RESOLVE
	lua_tinker::class_def<CXSLResolveItemManager>( GetLuaState(), "AddResolveDataAvatar",				&CXSLResolveItemManager::AddResolveDataAvatar_LUA );
#endif SERV_MULTI_RESOLVE
	//}}

	lua_tinker::class_def<CXSLResolveItemManager>( GetLuaState(), "dump",								&CXSLResolveItemManager::Dump );

	lua_tinker::decl( GetLuaState(), "g_pResolveItemManager", this );
}

bool CXSLResolveItemManager::SetJackpotProb_LUA( float fRate )
{
	if( !m_kJackpotLottery.AddCase( JP_JACKPOT, fRate ) )
	{
		START_LOG( cerr, L"��� Ȯ�� ���� ����!" )
			<< BUILD_LOG( fRate )
			<< END_LOG;
		return false;
	}

	return true;
}


//{{ 2011. 01. 19	������	������ ���� ����
#ifdef SERV_ITEM_RESOLVE_NEW
//////////////////////////////////////////////////////////////////////////

bool CXSLResolveItemManager::AddResolveDataBrokenPiece_LUA( int iResolveResultType, int iItemID )
{
	if( iItemID <= 0  ||  iResolveResultType < RRT_EL_FLOUR  ||  iResolveResultType >= RRT_MAX )
	{
		START_LOG( cerr, L"���� �׷� ������ �Ľ� ����.!" )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iResolveResultType )
			<< END_LOG;

		return false;
	}

	std::map< int, BrokenPieceResolve >::iterator mit;

	BrokenPieceResolve kResolveData;	
	kResolveData.m_iItemID = iItemID;

	KLuaManager luaMgr( GetLuaState() );
	LUA_GET_VALUE_RETURN( luaMgr, "iUnitCost",	kResolveData.m_iUnitCost,	 0,		goto error_proc; );
	LUA_GET_VALUE_RETURN( luaMgr, "iBeginLv",	kResolveData.m_iBeginLv,	 0,		goto error_proc; );
	LUA_GET_VALUE_RETURN( luaMgr, "iEndLv",		kResolveData.m_iEndLv,		 0,		goto error_proc; );

	m_vecBrokenPieceResolve[iResolveResultType].push_back( kResolveData );

	return true;

error_proc:
	START_LOG( cerr, L"���� ��ũ��Ʈ �Ľ� ����!" )
		<< BUILD_LOG( iResolveResultType )
		<< BUILD_LOG( iItemID )
		<< BUILD_LOG( kResolveData.m_iUnitCost )
		<< BUILD_LOG( kResolveData.m_iBeginLv )
		<< BUILD_LOG( kResolveData.m_iEndLv )
		<< END_LOG;

	return false;
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

bool CXSLResolveItemManager::AddResolveDataBrokenPiece_LUA( int iEqipPosition, int iStdValue, bool bIsDown, int  iItemID )
{
	if( iEqipPosition <= 0  ||  iStdValue <= 0  ||  iItemID <= 0 )
	{
		START_LOG( cerr, L"���� �׷� ������ �Ľ� ����.!" )
			<< BUILD_LOG( iEqipPosition )
			<< BUILD_LOG( iStdValue )
			<< BUILD_LOG( bIsDown )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return false;
	}

	std::map< int, BrokenPieceResolve >::iterator mit;

	BPResolveData kResolveData;	
	kResolveData.m_iResultItemID = iItemID;

	int iMin = 0;
	int iMax = 0;

	KLuaManager luaMgr( GetLuaState() );
	LUA_GET_VALUE_RETURN( luaMgr, "fRate",		kResolveData.m_fRate,	 0.0f,	goto error_proc; );
	LUA_GET_VALUE_RETURN( luaMgr, "iFactor",	kResolveData.m_iFactor,	 0,		goto error_proc; );
	LUA_GET_VALUE_RETURN( luaMgr, "iMin",		iMin,					 0,		goto error_proc; );
	LUA_GET_VALUE_RETURN( luaMgr, "iMax",		iMax,					 0,		goto error_proc; );

	for( int iCnt = iMin; iCnt <= iMax; ++iCnt )
	{
		kResolveData.m_kLottery.AddCaseSameProb( iCnt );
	}	

	mit = m_mapBrokenPieceResolve.find( iEqipPosition );
	if( mit == m_mapBrokenPieceResolve.end() )
	{
		BrokenPieceResolve kInfo;
		kInfo.m_iStdValue = iStdValue;
		if( bIsDown )
		{
			kInfo.m_vecDownResolve.push_back( kResolveData );
		}
		else
		{
			kInfo.m_vecUpResolve.push_back( kResolveData );
		}
		m_mapBrokenPieceResolve.insert( std::make_pair( iEqipPosition, kInfo ) );
	}
	else
	{
		if( bIsDown )
		{
			mit->second.m_vecDownResolve.push_back( kResolveData );
		}
		else
		{
			mit->second.m_vecUpResolve.push_back( kResolveData );
		}
	}

	return true;

error_proc:
	START_LOG( cerr, L"���� ��ũ��Ʈ �Ľ� ����!" )
		<< BUILD_LOG( iEqipPosition )
		<< BUILD_LOG( iStdValue )		
		<< BUILD_LOG( bIsDown )
		<< BUILD_LOG( iItemID )
		<< BUILD_LOG( kResolveData.m_fRate )
		<< BUILD_LOG( kResolveData.m_iFactor )
		<< BUILD_LOG( iMin )
		<< BUILD_LOG( iMax )
		<< END_LOG;

	return false;
}

#endif SERV_ITEM_RESOLVE_NEW
//////////////////////////////////////////////////////////////////////////
//}}

bool CXSLResolveItemManager::AddResolveData_LUA( int iItemGrade, int iItemID )
{	
	if( iItemGrade <= 0 || iItemID <= 0 )
	{
		START_LOG( cerr, L"���� ������ ������ �Ľ� ����.!" )
			<< BUILD_LOG( iItemGrade )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	std::map< int, std::vector< SResolveData > >::iterator mit;

	SResolveData resolveData;
	resolveData.m_iResultItemID = iItemID;

	float fRate = 0.0f;

	KLuaManager luaMgr( GetLuaState() );
	LUA_GET_VALUE_RETURN( luaMgr, "fSuccRate",		fRate,							 0.0f,	goto error_proc; );
	LUA_GET_VALUE_RETURN( luaMgr, "iRandomCnt",		resolveData.m_iRandomCount,		 0,		goto error_proc; );
	LUA_GET_VALUE_RETURN( luaMgr, "iResultCnt",		resolveData.m_iResultCount,		 0,		goto error_proc; );
	LUA_GET_VALUE_RETURN( luaMgr, "iBeginLv",		resolveData.m_iBeginLv,			 0,		goto error_proc; );
	LUA_GET_VALUE_RETURN( luaMgr, "iEndLv",			resolveData.m_iEndLv,			 0,		goto error_proc; );

	resolveData.m_kLottery.AddCase( iItemID, fRate );

	mit = m_mapResolveData.find( iItemGrade );
	if( mit == m_mapResolveData.end() )
	{
		std::vector< SResolveData > vecResolveData;
		vecResolveData.push_back( resolveData );
		m_mapResolveData.insert( std::make_pair( iItemGrade, vecResolveData ) );
	}
	else
	{
		mit->second.push_back( resolveData );
	}

	return true;

error_proc:
	START_LOG( cerr, L"��ũ��Ʈ �Ľ� ����!" )
		<< BUILD_LOG( iItemGrade )
		<< BUILD_LOG( iItemID )
		<< BUILD_LOG( fRate )
		<< BUILD_LOG( resolveData.m_iRandomCount )
		<< BUILD_LOG( resolveData.m_iResultCount )
		<< BUILD_LOG( resolveData.m_iBeginLv )
		<< BUILD_LOG( resolveData.m_iEndLv )
		<< END_LOG;

	return false;
}

//{{ 2011. 01. 19	������	������ ���� ����
#ifdef SERV_ITEM_RESOLVE_NEW
void CXSLResolveItemManager::AddWeaponResolveBrokenPieceRate_LUA( int iItemType, float fRate )
{
	if( m_kWeaponResolveLottery.AddCase( iItemType, fRate ) == false )
	{
		START_LOG( cerr, L"���� ���� ���� Ȯ�� ���� ����!" )
			<< BUILD_LOG( iItemType )
			<< BUILD_LOG( fRate )
			<< END_LOG;
	}
}

void CXSLResolveItemManager::AddArmorResolveBrokenPieceRate_LUA( int iItemType, float fRate )
{
	if( m_kArmorResolveLottery.AddCase( iItemType, fRate ) == false )
	{
		START_LOG( cerr, L"�� ���� ���� Ȯ�� ���� ����!" )
			<< BUILD_LOG( iItemType )
			<< BUILD_LOG( fRate )
			<< END_LOG;
	}
}
#endif SERV_ITEM_RESOLVE_NEW
//}}

double CXSLResolveItemManager::GetJackpotTime() const
{
	const int iRandomValue = m_iMaxJackpotTime - m_iMinJackpotTime;
	if( iRandomValue <= 0 )
	{
		START_LOG( cerr, L"���� ��� Ȯ������ 0�̰ų� 0���� �۴�. 0 ������ ���� ���!" )
			<< BUILD_LOG( m_iMinJackpotTime )
			<< BUILD_LOG( m_iMaxJackpotTime )
			<< BUILD_LOG( iRandomValue )
			<< END_LOG;

		// �ϴ� �⺻�� 3�ð��� �ش�
		return 10800.f;
	}

	return static_cast<double>( ( rand() % iRandomValue ) + m_iMinJackpotTime );
}

//{{ 2011. 01. 19	������	������ ���� ����
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_ITEM_RESOLVE_NEW

bool CXSLResolveItemManager::GetResultItem( IN const int iItemGrade, 
										   IN const int iEqipLevel, 
										   IN const int iItemType, 
										   IN const int iSellPrice, 
										   IN OUT bool& bJackpot, 
										   OUT std::map< int, KItemInfo >& mapResultItem ) const
{
	SET_ERROR( NET_OK );

	mapResultItem.clear();

	// ��� Ȯ�� ������
	if( bJackpot )
	{
		const int iResult = m_kJackpotLottery.Decision();
		if( iResult != JP_JACKPOT )
		{
			bJackpot = false;
		}
	}

	////////////////////////////////////////////////////////////////////////// ���� �۾�

	// 1. ������ ��޿� �´� ��ȭ�� ȹ��
	{
		std::map< int, std::vector< SResolveData > >::const_iterator mitRD;
		mitRD = m_mapResolveData.find( iItemGrade );
		if( mitRD != m_mapResolveData.end() )
		{
			BOOST_TEST_FOREACH( const SResolveData&, kResolveData, mitRD->second )
			{
				if( iEqipLevel < kResolveData.m_iBeginLv  ||  iEqipLevel > kResolveData.m_iEndLv )
					continue;

				ResolveResult_EnchantStone( kResolveData, iEqipLevel, bJackpot, mapResultItem );
			}
		}
	}	

	// 2. ���� ���� ȹ��
	{
		ResolveResult_BrokenPiece( RRT_EL_FLOUR, iEqipLevel, iSellPrice, mapResultItem );
	}

	// 3. ���� ���� �Ǵ� �� ���� ȹ��
	switch( iItemType )
	{
	case CXSLItem::IT_WEAPON:
		{
			const int iResolveItemType = m_kWeaponResolveLottery.Decision();
			if( iResolveItemType == CXSLItem::IT_WEAPON )
			{
				ResolveResult_BrokenPiece( RRT_WEAPON_BROKEN_PIECE, iEqipLevel, iSellPrice, mapResultItem );
			}
			else if( iResolveItemType == CXSLItem::IT_DEFENCE )
			{
				ResolveResult_BrokenPiece( RRT_ARMOR_BROKEN_PIECE, iEqipLevel, iSellPrice, mapResultItem );
			}
			else
			{
                START_LOG( cerr, L"��ȹ�� ���� �����ٵ� ���� ���Գ�?" )
					<< BUILD_LOG( iItemType )
					<< BUILD_LOG( iResolveItemType )
					<< END_LOG;
			}
		}
		break;

	case CXSLItem::IT_DEFENCE:
		{			
			int iResolveItemType = CXSLItem::IT_NONE;
			int iWeaponMinUnitCost = 0;

			BOOST_TEST_FOREACH( const BrokenPieceResolve&, kResolveData, m_vecBrokenPieceResolve[RRT_WEAPON_BROKEN_PIECE] )
			{
				if( iEqipLevel < kResolveData.m_iBeginLv  ||  iEqipLevel > kResolveData.m_iEndLv )
					continue;

				iWeaponMinUnitCost = kResolveData.m_iUnitCost;
				break;
			}
			
			if( iWeaponMinUnitCost <= iSellPrice )
			{
				// ���� ���ȱ� ������ ���� ���� 1Unit�� ��ġ���� ���ٸ� Ȯ�� ������ ���� ���� ������ �� ���� ������ ����!
				iResolveItemType = m_kArmorResolveLottery.Decision();
			}
			else
			{
				// ���� ���ȱ� ������ ���� ���� 1Unit�� ��ġ���� ���ٸ� 100% �� �������� ȯ��
				iResolveItemType = CXSLItem::IT_DEFENCE;
			}

			if( iResolveItemType == CXSLItem::IT_WEAPON )
			{
				ResolveResult_BrokenPiece( RRT_WEAPON_BROKEN_PIECE, iEqipLevel, iSellPrice, mapResultItem );
			}
			else if( iResolveItemType == CXSLItem::IT_DEFENCE )
			{
				ResolveResult_BrokenPiece( RRT_ARMOR_BROKEN_PIECE, iEqipLevel, iSellPrice, mapResultItem );
			}
			else
			{
				START_LOG( cerr, L"��ȹ�� ���� �����ٵ� ���� ���Գ�?" )
					<< BUILD_LOG( iItemType )
					<< BUILD_LOG( iResolveItemType )
					<< END_LOG;
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"���� �� �� ���� ������ Ÿ�� �Դϴ�!" )
				<< BUILD_LOG( iItemType )
				<< END_LOG;
			goto end_proc;
		}
		break;
	}

	return true;

end_proc:
	SET_ERROR( ERR_RESOLVE_ITEM_02 );

	START_LOG( cerr, L"���� ������ �׷�ã�� ����.!" )
		<< BUILD_LOG( iItemGrade )
		<< BUILD_LOG( iEqipLevel )
		<< BUILD_LOG( iItemType )
		<< BUILD_LOG( iSellPrice )
		<< END_LOG;
	return false;
}
//}}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

//{{ 2008. 11. 15  ������	������ ����
bool CXSLResolveItemManager::GetResultItem( IN const int iItemGrade, 
										   IN const int iEqipLevel, 
										   IN const int iEqipPosition, 
										   IN const int iPrice, 
										   IN OUT bool& bJackpot, 
										   OUT std::map< int, KItemInfo >& mapResultItem ) const
{
	SET_ERROR( NET_OK );

	mapResultItem.clear();

	std::map< int, BrokenPieceResolve >::const_iterator mit;
	std::map< int, std::vector< SResolveData > >::const_iterator mitRD;

	switch( iEqipPosition )
	{
	case CXSLUnit::EP_WEAPON_HAND:
	case CXSLUnit::EP_DEFENCE_BODY:
	case CXSLUnit::EP_DEFENCE_LEG:
	case CXSLUnit::EP_DEFENCE_HAND:
	case CXSLUnit::EP_DEFENCE_FOOT:
		break;

	default:
		START_LOG( cerr, L"�߸��� ������ ���� ��ġ�Դϴ�." )
			<< BUILD_LOG( iEqipPosition )
			<< BUILD_LOG( iPrice )
			<< END_LOG;

		goto end_proc;
	}

	// ��� Ȯ�� ������
	if( bJackpot )
	{		
		const int iResult = m_kJackpotLottery.Decision();
		if( iResult != JP_JACKPOT )
		{
			bJackpot = false;
		}
	}

	////////////////////////////////////////////////////////////////////////// ���� �۾�

	// 1. ������ ��޿� �´� ��ȭ�� ȹ��
	mitRD = m_mapResolveData.find( iItemGrade );
	if( mitRD != m_mapResolveData.end() )
	{
		BOOST_TEST_FOREACH( const SResolveData&, kResolveData, mitRD->second )
		{
			if( iEqipLevel < kResolveData.m_iBeginLv  ||  iEqipLevel > kResolveData.m_iEndLv )
				continue;

			ResolveItem( kResolveData, iEqipLevel, bJackpot, mapResultItem );
		}
	}

	// 2. ��� ���� ��ġ�� �´� ���� ����
	mit = m_mapBrokenPieceResolve.find( iEqipPosition );
	if( mit == m_mapBrokenPieceResolve.end() )
		goto end_proc;

	// ���ȱ� ���� ���
	const int iSellPrice = (int)((float)(iPrice) * 0.2);

	// ���ȱ� ���� ���ؿ� ���� ���ذ���� �ٸ���
	const std::vector< BPResolveData >& vecResolve = ( mit->second.m_iStdValue < iSellPrice ? mit->second.m_vecDownResolve : mit->second.m_vecUpResolve );

	BOOST_TEST_FOREACH( const BPResolveData&, kBPResolve, vecResolve )
	{
		if( kBPResolve.m_iFactor <= 0 )
		{
			START_LOG( cerr, L"0������ ����! factor���� 0�̴�?" )
				<< BUILD_LOG( iEqipPosition )
				<< BUILD_LOG( iSellPrice )
				<< END_LOG;
			continue;
		}

		const int iRandomValue = kBPResolve.m_kLottery.DecisionSameProb();
		if( iRandomValue < -2  ||  iRandomValue > 2 )
		{
			START_LOG( cerr, L"�ǵ����� �ʴ� ���� ���Դ�!" )
				<< BUILD_LOG( iRandomValue )
				<< END_LOG;
			continue;
		}

		// [����] ��Ȯ�ϰ� ������ ���Ǵ��� �ݵ�� �׽�Ʈ �Ұ�!
		const int iResultCount = ( (int)( (float)iSellPrice * kBPResolve.m_fRate ) / kBPResolve.m_iFactor ) + iRandomValue;
		if( iResultCount <= 0 )
			continue;

		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kBPResolve.m_iResultItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"������ ���ø� ������ ã�� �� �����ϴ�." )
				<< BUILD_LOG( kBPResolve.m_iResultItemID )
				<< END_LOG;
			continue;
		}

		KItemInfo kInsertItemInfo;
		kInsertItemInfo.m_iItemID	 = kBPResolve.m_iResultItemID;
		kInsertItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
		kInsertItemInfo.m_iQuantity	 = ( bJackpot ? iResultCount * 2 : iResultCount );
		kInsertItemInfo.m_sEndurance = pItemTemplet->m_Endurance;
		mapResultItem.insert( std::make_pair( kBPResolve.m_iResultItemID, kInsertItemInfo ) );
	}

	// [�̺�Ʈ] ���� �̺�Ʈ		2008�� 11�� 20�� ~ 2008�� 12�� 3��
	//{
	//	if( iItemGrade > CXSLItem::IG_RARE )
	//	{
	//		// 10% �� �̺�Ʈ ���� ����
	//		if( ( rand() % 10 ) != 0 )
	//			return true;
	//	}

	//	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( 130096 );
	//	if( pItemTemplet == NULL )
	//	{
	//		START_LOG( cerr, L"������ ���ø� ������ ã�� �� �����ϴ�." )
	//			<< BUILD_LOG( 130096 )
	//			<< END_LOG;
	//	}
	//	else
	//	{
	//		KItemInfo kEventItemInfo;
	//		kEventItemInfo.m_iItemID	= 130096;
	//		kEventItemInfo.m_iQuantity	= 1;
	//		kEventItemInfo.m_iEndurance = pItemTemplet->m_Endurance;
	//		kEventItemInfo.m_iUsageType = pItemTemplet->m_PeriodType;

	//		mapResultItem.insert( std::make_pair( 130096, kEventItemInfo ) );
	//	}
	//}
	//}}

	return true;

end_proc:
	SET_ERROR( ERR_RESOLVE_ITEM_02 );

	START_LOG( cerr, L"���� ������ �׷�ã�� ����.!" )
		<< BUILD_LOG( iItemGrade )
		<< BUILD_LOG( iEqipLevel )
		<< BUILD_LOG( iEqipPosition )
		<< BUILD_LOG( iPrice )
		<< END_LOG;

	return false;
}
//}}

//////////////////////////////////////////////////////////////////////////
#endif SERV_ITEM_RESOLVE_NEW
//}}

void CXSLResolveItemManager::ResolveResult_EnchantStone( IN const SResolveData& kResolveData,										  
														 IN const int iEqipLevel,
														 IN const bool bJackpot,
														 IN OUT std::map< int, KItemInfo >& mapResultItem ) const
{
	// ���� ���� Ƚ�� ��ŭ �����ϴ�
	for( int iCnt = 0; iCnt < kResolveData.m_iRandomCount; ++iCnt )
	{
		int iResultItemID = 0;
		if( bJackpot )
		{
			// ���!!
			iResultItemID = kResolveData.m_iResultItemID;
		}
		else
		{
			// �귿!
			iResultItemID = kResolveData.m_kLottery.Decision();
		}

		// ���� ���ȴµ� ���̱���
		if( iResultItemID == KLottery::CASE_BLANK )
			continue;

		std::map< int, KItemInfo >::iterator mitResult;
		mitResult = mapResultItem.find( iResultItemID );
		if( mitResult == mapResultItem.end() )
		{
			//{{ 2008. 3. 17  ������  DeleteAndInsert()
			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iResultItemID );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"������ ���ø� ������ ã�� �� �����ϴ�." )
					<< BUILD_LOG( iResultItemID )
					<< END_LOG;
			}
			else
			{
				KItemInfo kInsertItemInfo;
				kInsertItemInfo.m_iItemID	 = iResultItemID;
				kInsertItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
				kInsertItemInfo.m_iQuantity	 = ( bJackpot ? kResolveData.m_iResultCount * 2 : kResolveData.m_iResultCount );
				kInsertItemInfo.m_sEndurance = pItemTemplet->m_Endurance;
				mapResultItem.insert( std::make_pair( iResultItemID, kInsertItemInfo ) );
			}
			//}}
		}
		else
		{
			mitResult->second.m_iQuantity += ( bJackpot ? kResolveData.m_iResultCount : kResolveData.m_iResultCount );
		}
	}
}

//{{ 2011. 01. 19	������	������ ���� ����
#ifdef SERV_ITEM_RESOLVE_NEW
void CXSLResolveItemManager::ResolveResult_BrokenPiece( IN RESOLVE_RESULT_TYPE eType, IN const int iEqipLevel, IN const int iSellPrice, IN OUT std::map< int, KItemInfo >& mapResultItem ) const
{
	if( eType < RRT_EL_FLOUR  ||  eType >= RRT_MAX )
	{
		START_LOG( cerr, L"���� ��� Ÿ���� �̻��մϴ�!" )
			<< BUILD_LOG( eType )
			<< END_LOG;
		return;
	}

	BOOST_TEST_FOREACH( const BrokenPieceResolve&, kResolveData, m_vecBrokenPieceResolve[eType] )
	{
		if( iEqipLevel < kResolveData.m_iBeginLv  ||  iEqipLevel > kResolveData.m_iEndLv )
			continue;

		_JIF( kResolveData.m_iUnitCost > 0, continue; );
		const int iQuantityInteger = (int)( (float)iSellPrice / (float)kResolveData.m_iUnitCost );
		const float fQuantityFloat = ( ( (float)iSellPrice / (float)kResolveData.m_iUnitCost ) - (float)iQuantityInteger ) * 100.f;
		int iResultQuantity = iQuantityInteger;

		KLottery kLot;
		kLot.AddCase( 0, fQuantityFloat );
		if( kLot.Decision() != KLottery::CASE_BLANK )
		{
			// 1�� �� �߰�!
			++iResultQuantity;
		}

		// ������ 0�̸� �Ѿ��!
		if( iResultQuantity <= 0 )
			continue;

		std::map< int, KItemInfo >::iterator mitResult;
		mitResult = mapResultItem.find( kResolveData.m_iItemID );
		if( mitResult == mapResultItem.end() )
		{
			//{{ 2008. 3. 17  ������  DeleteAndInsert()
			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kResolveData.m_iItemID );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"������ ���ø� ������ ã�� �� �����ϴ�." )
					<< BUILD_LOG( kResolveData.m_iItemID )
					<< END_LOG;
			}
			else
			{
				KItemInfo kInsertItemInfo;
				kInsertItemInfo.m_iItemID	 = kResolveData.m_iItemID;
				kInsertItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
				kInsertItemInfo.m_iQuantity	 = iResultQuantity;
				kInsertItemInfo.m_sEndurance = pItemTemplet->m_Endurance;
				mapResultItem.insert( std::make_pair( kResolveData.m_iItemID, kInsertItemInfo ) );
			}
			//}}
		}
		else
		{
			mitResult->second.m_iQuantity += iResultQuantity;
		}
	}
}
#endif SERV_ITEM_RESOLVE_NEW
//}}

//{{ 2011.03.16   �ӱԼ� �ƹ�Ÿ ���� �ý���
#ifdef SERV_MULTI_RESOLVE
bool CXSLResolveItemManager::AddResolveDataAvatar_LUA( int iItemGrade, int iItemID )
{
	if( iItemGrade <= 0 || iItemID <= 0 )
	{
		START_LOG( cerr, L"���� ������ ������ �Ľ� ����.!" )
			<< BUILD_LOG( iItemGrade )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	float fRate = 0.0f;
	SResolveDataAvatar ResolveData;
	std::map< int, SResolveDataAvatarGrade  >::iterator mit;

	KLuaManager luaMgr( GetLuaState() );
	LUA_GET_VALUE_RETURN( luaMgr, "fSuccRate",		fRate,		0.0f,	goto error_proc; );
	
	ResolveData.m_iResultItemID = iItemID;
	ResolveData.m_fRate = fRate;

	mit = m_mapResolveDataAvatar.find( iItemGrade );

	if( mit != m_mapResolveDataAvatar.end() )
	{
		mit->second.m_vecResolveDataAvatar.push_back( ResolveData );
		if( mit->second.m_kLottery.AddCase( iItemID, fRate ) == false )
		{
			START_LOG( cerr, L"���� ������ Ȯ�� ���� ����!" )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( fRate )
				<< END_LOG;
		}
	}
	else
	{
		SResolveDataAvatarGrade resolvegrade;
		resolvegrade.m_iGrade	= iItemGrade;
		resolvegrade.m_vecResolveDataAvatar.push_back( ResolveData );
		if( resolvegrade.m_kLottery.AddCase( iItemID, fRate ) == false )
		{
			START_LOG( cerr, L"���� ������ Ȯ�� ���� ����!" )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( fRate )
				<< END_LOG;
		}

		m_mapResolveDataAvatar.insert( std::make_pair( iItemGrade, resolvegrade ) );
	}



	return true;

error_proc:
	START_LOG( cerr, L"��ũ��Ʈ �Ľ� ����!" )
		<< BUILD_LOG( fRate )
		<< END_LOG;

	return false;
}


bool CXSLResolveItemManager::GetResultItemAvatar( IN const int iItemGrade, 
											IN const int iItemType, 
											OUT std::map< int, KItemInfo >& mapResultItem,
											OUT int& iAfterItem, 
											OUT int& iAfterItemQuantity) const
{
	SET_ERROR( NET_OK );

	//mapResultItem.clear();

	// 1. ������ ��޿� �´� ������ ȹ��

	std::map< int, SResolveDataAvatarGrade >::const_iterator mitRD;
	mitRD = m_mapResolveDataAvatar.find( 1 );

	if( mitRD == m_mapResolveDataAvatar.end() )
	{
		goto end_proc;
	}

	int iResultItemID = 0;

	// �귿!
	iResultItemID = mitRD->second.m_kLottery.Decision();

	if( iResultItemID == KLottery::CASE_BLANK )
	{
		goto end_proc;
	}

	START_LOG( cwarn, L"�ƹ�Ÿ ���� ���� ����" )
		<< BUILD_LOG( iResultItemID )
		<< END_LOG;
		;

	//{{ 2008. 3. 17  ������  DeleteAndInsert()
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iResultItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"������ ���ø� ������ ã�� �� �����ϴ�." )
			<< BUILD_LOG( iResultItemID )
			<< END_LOG;
	}
	else
	{
		iAfterItem	= iResultItemID;
		iAfterItemQuantity	= 1;

		KItemInfo kInsertItemInfo;
		kInsertItemInfo.m_iItemID	 = iResultItemID;
		kInsertItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
		kInsertItemInfo.m_iQuantity	 = 1;
		kInsertItemInfo.m_sEndurance = pItemTemplet->m_Endurance;

		std::map< int, KItemInfo >::iterator it = mapResultItem.find(iResultItemID); 
		if( it != mapResultItem.end())
		{
			it->second.m_iQuantity += 1;
		}
		else
		{
			mapResultItem.insert( std::make_pair( iResultItemID, kInsertItemInfo ) );
		}
		
	}
	//}}

	return true;

end_proc:
	SET_ERROR( ERR_RESOLVE_ITEM_02 );

	START_LOG( cerr, L"���� ������ �׷�ã�� ����.!" )
		<< BUILD_LOG( iItemGrade )
		<< BUILD_LOG( iItemType )
		<< END_LOG;
	return false;
}
//}}

bool CXSLResolveItemManager::ResolveResult_Avatar( IN const SResolveDataAvatar& kResolveDataAvatar,										  
														IN OUT std::map< int, KItemInfo >& mapResultItem ) const
{
/*		int iResultItemID = 0;

		// �귿!
		iResultItemID = kResolveDataAvatar.m_kLottery.Decision();

		if( iResultItemID == KLottery::CASE_BLANK )
		{
			goto end_proc;
		}

		START_LOG( cwarn, L"�ƹ�Ÿ ���� ���� ����" )
			<< BUILD_LOG( iResultItemID )
			<< END_LOG;
;

		//{{ 2008. 3. 17  ������  DeleteAndInsert()
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iResultItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"������ ���ø� ������ ã�� �� �����ϴ�." )
				<< BUILD_LOG( iResultItemID )
				<< END_LOG;
		}
		else
		{
			KItemInfo kInsertItemInfo;
			kInsertItemInfo.m_iItemID	 = iResultItemID;
			kInsertItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
			kInsertItemInfo.m_iQuantity	 = 1;
			kInsertItemInfo.m_sEndurance = pItemTemplet->m_Endurance;
			mapResultItem.insert( std::make_pair( iResultItemID, kInsertItemInfo ) );
		}
		//}}

		return true;

end_proc:
		SET_ERROR( ERR_RANDOM_ITEM_02 );

		START_LOG( cwarn, L"�ƹ�Ÿ ���� ���� ����" )
			<< BUILD_LOG( iResultItemID )
			<< END_LOG;
*/
	return false;
}

#endif SERV_MULTI_RESOLVE
//}}
