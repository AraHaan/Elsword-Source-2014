#include ".\xslenchantitemmanager.h"
#include "NetError.h"
//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	#include "XSLItemManager.h"
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}

ImplementRefreshSingleton( CXSLEnchantItemManager );

CXSLEnchantItemManager::CXSLEnchantItemManager(void)
{
#ifdef SERV_MULTIPLE_BLESSED_ENCHANT_STONE
#else //SERV_MULTIPLE_BLESSED_ENCHANT_STONE
	m_iWeaponEnchantStone = 0;
	m_iRareWeaponEnchantStone = 0;
	m_iArmorEnchantStone = 0;
	m_iRareArmorEnchantStone = 0;
#endif //SERV_MULTIPLE_BLESSED_ENCHANT_STONE
	//{{ 2011. 01. 24	������	�÷���� ��ȭ ���� ����
#ifdef SERV_SUPPORT_MATERIAL_LIMIT
	m_iEnchantLimitLevel = 0;
#endif SERV_SUPPORT_MATERIAL_LIMIT
	//}}
	//{{ 2012. 01. 19	��μ�	�÷���� ���� ��ȭ �̺�Ʈ �ǽð� ����
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	m_iEventEnchantLimitLevel = 0;
#endif SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	//}}
#ifdef SERV_ENCHANT_LIMIT
	m_iRealEnchantLimitLevel = 0;
#endif SERV_ENCHANT_LIMIT
}

CXSLEnchantItemManager::~CXSLEnchantItemManager(void)
{
}

ImplToStringW( CXSLEnchantItemManager )
{
	stm_	<< L"----------[ Enchant Item Manager ]----------" << std::endl	
			<< TOSTRINGW( m_mapEnchantRate.size() )
			<< TOSTRINGW( m_mapEnchantProb.size() )			
			<< TOSTRINGW( m_mapEnchantRareProb.size() )
#ifdef SERV_MULTIPLE_BLESSED_ENCHANT_STONE
			<< TOSTRINGW( m_vecWeaponEnchantStone.size() )
			<< TOSTRINGW( m_vecRareWeaponEnchantStone.size() )
			<< TOSTRINGW( m_vecArmorEnchantStone.size() )
			<< TOSTRINGW( m_vecRareArmorEnchantStone.size() )
#else //SERV_MULTIPLE_BLESSED_ENCHANT_STONE
			<< TOSTRINGW( m_iWeaponEnchantStone )
			<< TOSTRINGW( m_iRareWeaponEnchantStone )
			<< TOSTRINGW( m_iArmorEnchantStone )
			<< TOSTRINGW( m_iRareArmorEnchantStone )
#endif //SERV_MULTIPLE_BLESSED_ENCHANT_STONE
			<< TOSTRINGW( m_mapEnchantStoneInfo.size() )
			<< TOSTRINGW( m_mapSupportMaterialInfo.size() )
			<< TOSTRINGW( m_mapRestoreItemInfo.size() )
			<< TOSTRINGW( m_mapEnchantAttach.size() )
			//{{  2011.12.21     ��μ�    �ں� �÷���� ���� �̺�Ʈ
#ifdef SERV_COBO_SUPPORT_MATERIAL_EVENT
#ifdef SERV_EVENT_SUPPORT_MATERIAL_MULTI
			<< TOSTRINGW( m_multimapEventSupportMaterialInfo.size() )
#else
			<< TOSTRINGW( m_mapEventSupportMaterialInfo.size() )
#endif SERV_EVENT_SUPPORT_MATERIAL_MULTI
#endif SERV_COBO_SUPPORT_MATERIAL_EVENT
			//}}
			//{{  2011.12.21     ��μ�    �ں� �ູ���� ������ �ֹ��� �̺�Ʈ
#ifdef SERV_COBO_RESTORE_SCROLL_EVENT
#ifdef SERV_EVENT_RESTORE_SCROLL_MULTI
			<< TOSTRINGW( m_multimapEventRestoreItemInfo.size() )
#else
			<< TOSTRINGW( m_mapEventRestoreItemInfo.size() )
#endif SERV_EVENT_RESTORE_SCROLL_MULTI
#endif SERV_COBO_RESTORE_SCROLL_EVENT
			//}}
#ifdef SERV_EVENT_ENCHANT_STONE_MULTI
			<< TOSTRINGW( m_multimapEventEnchantStoneInfo.size() )
#endif SERV_EVENT_ENCHANT_STONE_MULTI
			//{{ 2011.5.23 ���� : ��ȭ�� ���� Ȯ�� ���� ������ �߰�
#ifdef SERV_ENCHANT_PLUS_ITEM
			<< TOSTRINGW( m_mapEnchantPlusProb.size() )
			<< TOSTRINGW( m_mapEnchantPlusInfo.size() )
#endif
			//}}
			//{{ 2011.5.23 ���� : ��ȭ�� �ı� ���� ������ �߰�
#ifdef SERV_DESTROY_GUARD_ITEM
			<< TOSTRINGW( m_mapDestroyGuardInfo.size() )
#endif
			//}}
			//{{ 2012. 01. 19	��μ�	�÷���� ���� ��ȭ �̺�Ʈ �ǽð� ����
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
			<< TOSTRINGW( m_iEventEnchantLimitLevel )	
			<< TOSTRINGW( m_mapEventEnchantProb.size() )	
#endif SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
			//}}

		;

	return stm_;
}

ImplementLuaScriptParser( CXSLEnchantItemManager )
{
	lua_tinker::class_add<CXSLEnchantItemManager>( GetLuaState(), "CXSLEnchantItemManager" );	
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "AddEnchantStoneInfo",		&CXSLEnchantItemManager::AddEnchantStoneInfo_LUA );
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "AddSupportMaterialInfo",		&CXSLEnchantItemManager::AddSupportMaterialInfo_LUA );
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "AddRestoreItemInfo",			&CXSLEnchantItemManager::AddRestoreItemInfo_LUA );
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "AddAttachItemInfo",			&CXSLEnchantItemManager::AddAttachItemInfo_LUA );
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "SetEnchantRate",				&CXSLEnchantItemManager::SetEnchantRate_LUA );
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "SetEnchantProbability",		&CXSLEnchantItemManager::SetEnchantProbability_LUA );
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "SetEnchantRareProbability",	&CXSLEnchantItemManager::SetEnchantRareProbability_LUA );
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "dump",						&CXSLEnchantItemManager::Dump );
#ifdef SERV_MULTIPLE_BLESSED_ENCHANT_STONE
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "AddWeaponEnchantStone",		&CXSLEnchantItemManager::AddWeaponEnchantStone_LUA );
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "AddRareWeaponEnchantStone",	&CXSLEnchantItemManager::AddRareWeaponEnchantStone_LUA );
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "AddArmorEnchantStone",		&CXSLEnchantItemManager::AddArmorEnchantStone_LUA );
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "AddRareArmorEnchantStone",	&CXSLEnchantItemManager::AddRareArmorEnchantStone_LUA );
#else //SERV_MULTIPLE_BLESSED_ENCHANT_STONE
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "SetWeaponEnchantStone",		&CXSLEnchantItemManager::SetWeaponEnchantStone_LUA );
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "SetRareWeaponEnchantStone",	&CXSLEnchantItemManager::SetRareWeaponEnchantStone_LUA );
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "SetArmorEnchantStone",		&CXSLEnchantItemManager::SetArmorEnchantStone_LUA );
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "SetRareArmorEnchantStone",	&CXSLEnchantItemManager::SetRareArmorEnchantStone_LUA );
#endif //SERV_MULTIPLE_BLESSED_ENCHANT_STONE

	//{{ 2011. 01. 24	������	�÷���� ��ȭ ���� ����
#ifdef SERV_SUPPORT_MATERIAL_LIMIT
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "SetSupportMaterialLimit",	&CXSLEnchantItemManager::SetSupportMaterialEnchantLimitLevel_LUA );
#endif SERV_SUPPORT_MATERIAL_LIMIT
	//}}
#ifdef SERV_EVENT_ENCHANT_STONE_MULTI
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "AddEventEnchantStoneInfo",	&CXSLEnchantItemManager::AddEventEnchantStoneInfo_LUA );
#endif SERV_EVENT_ENCHANT_STONE_MULTI
	//{{  2011.12.21     ��μ�    �ں� �÷���� ���� �̺�Ʈ
#ifdef SERV_COBO_SUPPORT_MATERIAL_EVENT
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "AddEventSupportMaterialInfo",		&CXSLEnchantItemManager::AddEventSupportMaterialInfo_LUA );
#endif SERV_COBO_SUPPORT_MATERIAL_EVENT
	//}}
	//{{  2011.12.21     ��μ�    �ں� �ູ���� ������ �ֹ��� �̺�Ʈ
#ifdef SERV_COBO_RESTORE_SCROLL_EVENT
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "AddEventRestoreItemInfo",		&CXSLEnchantItemManager::AddEventRestoreItemInfo_LUA );
#endif SERV_COBO_RESTORE_SCROLL_EVENT
	//}}
#ifdef SERV_ENCHANT_LIMIT
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "SetEnchantLimit",	&CXSLEnchantItemManager::SetEnchantLimitLevel_LUA );
#endif SERV_ENCHANT_LIMIT
	//{{ 2011.5.23 ���� : ��ȭ�� ���� Ȯ�� ���� ������ �߰�
#ifdef SERV_ENCHANT_PLUS_ITEM
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "AddEnchantPlusInfo",			&CXSLEnchantItemManager::AddEnchantPlusInfo_LUA );
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "SetEnchantPlusProbability",		&CXSLEnchantItemManager::SetEnchantPlusProbability_LUA );
#endif
	//}}
	//{{ 2011.5.23 ���� : ��ȭ�� �ı� ���� ������ �߰�
#ifdef SERV_DESTROY_GUARD_ITEM
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "AddDestroyGuardInfo",		&CXSLEnchantItemManager::AddDestroyGuardInfo_LUA );
#endif
	//}}

	//{{ 2012. 01. 19	��μ�	�÷���� ���� ��ȭ �̺�Ʈ �ǽð� ����
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "SetEventSupportMaterialLimit",		&CXSLEnchantItemManager::SetEventSupportMaterialEnchantLimitLevel_LUA );
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "SetEnchantProbability_Event",		&CXSLEnchantItemManager::SetEnchantProbability_Event_LUA );
#endif SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	//}}
	//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "AddDropRandomEnchantInfo",			&CXSLEnchantItemManager::AddDropRandomEnchantInfo_LUA );
	lua_tinker::class_def<CXSLEnchantItemManager>( GetLuaState(), "AddCubeRandomEnchantInfo",			&CXSLEnchantItemManager::AddCubeRandomEnchantInfo_LUA );
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

	lua_tinker::decl( GetLuaState(), "EnchantItemManager", this );
}

bool CXSLEnchantItemManager::GetEnchantRate( IN int iEnchantLevel, OUT float& fPercent )
{
	std::map< int, float >::const_iterator mit;
	mit = m_mapEnchantRate.find( iEnchantLevel );
	if( mit == m_mapEnchantRate.end() )	
	{
		START_LOG( cerr, L"������ �ɷ�ġ ��ȭ ���� ����ã�� ����.!" )
			<< BUILD_LOG( iEnchantLevel )
			<< END_LOG;
	
		return false;
	}

	fPercent = mit->second;
	return true;
}

//{{ 2011.5.23 ���� : ��ȭ�� ���� Ȯ�� ���� ������ �߰�
#ifdef SERV_ENCHANT_PLUS_ITEM
bool CXSLEnchantItemManager::GetEnchantResult( IN bool bIsRare, IN bool bIsPlus, IN int iEnchantLevel, OUT int& iResult )
#else
bool CXSLEnchantItemManager::GetEnchantResult( IN bool bIsRare, IN int iEnchantLevel, OUT int& iResult )
#endif
//}}
{
	std::map< int, KLottery >::const_iterator mit;
	if( bIsRare )
	{
		mit = m_mapEnchantRareProb.find( iEnchantLevel );
		if( mit == m_mapEnchantRareProb.end() )
		{
			START_LOG( cerr, L"���� ��ȭ �ܰ� ���� ã�� ����.!" )
				<< BUILD_LOG( iEnchantLevel )
				<< END_LOG;

			return false;
		}
	}
	//{{ 2011.5.23 ���� : ��ȭ�� ���� Ȯ�� ���� ������ �߰�
#ifdef SERV_ENCHANT_PLUS_ITEM
	else if( bIsPlus )
	{
		START_LOG( clog, L"��ȭ ���� Ȯ�� ���� ������ ����!!" )
			<< BUILD_LOG( iEnchantLevel )
			<< END_LOG;
		mit = m_mapEnchantPlusProb.find( iEnchantLevel );
		if( mit == m_mapEnchantPlusProb.end() )
		{
			START_LOG( cerr, L"��ȭ �ܰ� ���� ã�� ����.!" )
				<< BUILD_LOG( iEnchantLevel )
				<< END_LOG;

			return false;
		}
	}
#endif SERV_ENCHANT_PLUS_ITEM
	//}}
	else
	{
		mit = m_mapEnchantProb.find( iEnchantLevel );
		if( mit == m_mapEnchantProb.end() )
		{
			START_LOG( cerr, L"��ȭ �ܰ� ���� ã�� ����.!" )
				<< BUILD_LOG( iEnchantLevel )
				<< END_LOG;

			return false;
		}
	}
	
	iResult = mit->second.Decision();
    if( iResult < NetError::ERR_ENCHANT_RESULT_00 || iResult > NetError::ERR_ENCHANT_RESULT_04 )
    {
        START_LOG( cerr, L"��ȭ ��� �̻�." )
            << BUILD_LOG( iResult )
            << BUILD_LOG( NetError::GetErrStr( iResult ) )
            << END_LOG;

        // ��ȭ �������� ó��.
        iResult = NetError::ERR_ENCHANT_RESULT_01;
    }

	return true;
}

//{{ 2008. 11. 16  ������	��ȭ ����
bool CXSLEnchantItemManager::AddEnchantStoneInfo_LUA( int iItemID, int iItemType, int iEquipLvMin, int iEquipLvMax )
{
	_JIF( iItemID > 0 && iItemType > 0 , goto err_proc );
	_JIF( 0 <= iEquipLvMin && iEquipLvMax <= 100, goto err_proc );
	
	for( int iIdx = iEquipLvMin; iIdx <= iEquipLvMax; ++iIdx )
	{
        KEStoneInfoKey kKey;
		kKey.first  = iIdx;
		kKey.second = iItemType;
		m_mapEnchantStoneInfo.insert( std::make_pair( kKey, iItemID ) );
	}
    return true;

err_proc:
	START_LOG( cerr, L"��ȭ�� ���� �ε� ����!" )
		<< BUILD_LOG( iItemID )
		<< BUILD_LOG( iEquipLvMin )	
		<< BUILD_LOG( iEquipLvMax )	
		<< END_LOG;
	return false;
}
//}}

//{{ 2008. 12. 14  ������	��ȭ ������
bool CXSLEnchantItemManager::AddSupportMaterialInfo_LUA( int iItemID, int iEquipLvMin, int iEquipLvMax )
{
	_JIF( iItemID > 0, goto err_proc );
	_JIF( 0 <= iEquipLvMin && iEquipLvMax <= 100, goto err_proc );

	for( int iIdx = iEquipLvMin; iIdx <= iEquipLvMax; ++iIdx )
	{
		m_mapSupportMaterialInfo.insert( std::make_pair( iIdx, iItemID ) );
	}
	return true;

err_proc:
	START_LOG( cerr, L"��ȭ ������ ���� �ε� ����!" )
		<< BUILD_LOG( iItemID )
		<< BUILD_LOG( iEquipLvMin )	
		<< BUILD_LOG( iEquipLvMax )
		<< END_LOG;
	return false;
}
//}}

//{{ 2008. 12. 21  ������	������
bool CXSLEnchantItemManager::AddRestoreItemInfo_LUA( int iItemID, int iItemType, int iEquipLvMin, int iEquipLvMax )
{
	_JIF( iItemID > 0, goto err_proc );
	_JIF( 0 <= iEquipLvMin && iEquipLvMax <= 100, goto err_proc );
	_JIF( iItemType >= CXSLItem::IT_NONE  &&  iItemType <= CXSLItem::IT_ETC, goto err_proc );

	for( int iIdx = iEquipLvMin; iIdx <= iEquipLvMax; ++iIdx )
	{
		KRestoreInfoKey kKey;
		kKey.first = iItemType;
		kKey.second = iIdx;
		m_mapRestoreItemInfo.insert( std::make_pair( kKey, iItemID ) );
	}
	return true;

err_proc:
	START_LOG( cerr, L"���Ұ� ������ ���� �ε� ����!" )
		<< BUILD_LOG( iItemID )
		<< BUILD_LOG( iItemType )
		<< BUILD_LOG( iEquipLvMin )	
		<< BUILD_LOG( iEquipLvMax )
		<< END_LOG;
	return false;
}
//}}

//{{ 2008. 12. 25  ������	��ȭ�ο�
#ifdef SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM// �۾���¥: 2013-07-02	// �ڼ���
bool CXSLEnchantItemManager::AddAttachItemInfo_LUA( int iItemID )
{
	KLuaManager luaMgr( GetLuaState() );

	KAttachInfo kInfo;

	_JIF( iItemID > 0, goto err_proc );
	
	LUA_GET_VALUE( luaMgr,	"iItemType",		kInfo.m_iItemType,		0	);
	_JIF( kInfo.m_iItemType >= CXSLItem::IT_NONE  &&  kInfo.m_iItemType <= CXSLItem::IT_ETC, goto err_proc );

	LUA_GET_VALUE( luaMgr,	"iEnchantLevel",	kInfo.m_iEnchantLv,		0	);
	_JIF( 0 < kInfo.m_iEnchantLv && kInfo.m_iEnchantLv <= 20, goto err_proc );

	LUA_GET_VALUE( luaMgr,	"iMinItemLevel",	kInfo.m_iMinItemLevel,	0	);
	_JIF( 0 <= kInfo.m_iMinItemLevel, goto err_proc );

	LUA_GET_VALUE( luaMgr,	"iMaxItemLevel",	kInfo.m_iMaxItemLevel,	0	);
	_JIF( 0 <= kInfo.m_iMaxItemLevel, goto err_proc );

	m_mapEnchantAttach.insert( std::make_pair( iItemID, kInfo ) );
	return true;

err_proc:
	START_LOG( cerr, L"��ȭ �ο� ������ ���� �ε� ����!" )
		<< BUILD_LOG( iItemID )
		<< BUILD_LOG( kInfo.m_iItemType )
		<< BUILD_LOG( kInfo.m_iEnchantLv )
		<< BUILD_LOG( kInfo.m_iMinItemLevel )
		<< BUILD_LOG( kInfo.m_iMaxItemLevel )
		<< END_LOG;
	return false;
}
#else // SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM
bool CXSLEnchantItemManager::AddAttachItemInfo_LUA( int iItemID, int iItemType, int iEnchantLevel )
{
#ifdef SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM// �۾���¥: 2013-07-02	// �ڼ���
	KLuaManager luaMgr( GetLuaState() );
#endif // SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM

	_JIF( iItemID > 0, goto err_proc );
	_JIF( 0 < iEnchantLevel && iEnchantLevel <= 20, goto err_proc );
	_JIF( iItemType >= CXSLItem::IT_NONE  &&  iItemType <= CXSLItem::IT_ETC, goto err_proc );

	{
		KAttachInfo kInfo;
		kInfo.m_iEnchantLv = iEnchantLevel;
		kInfo.m_iItemType = iItemType;

#ifdef SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM// �۾���¥: 2013-07-02	// �ڼ���
		LUA_GET_VALUE( luaMgr,	"iMinItemLevel",	kInfo.m_iMinItemLevel,	0	);
		LUA_GET_VALUE( luaMgr,	"iMaxItemLevel",	kInfo.m_iMaxItemLevel,	0	);
#endif // SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM

		m_mapEnchantAttach.insert( std::make_pair( iItemID, kInfo ) );
	}
	return true;

err_proc:
	START_LOG( cerr, L"��ȭ �ο� ������ ���� �ε� ����!" )
		<< BUILD_LOG( iItemID )
		<< BUILD_LOG( iEnchantLevel )
		<< BUILD_LOG( iItemType )
		<< END_LOG;
	return false;
}
#endif // SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM
//}}

bool CXSLEnchantItemManager::SetEnchantRate_LUA( int iEnchantLevel, float fPercent )
{
    std::map< int, float >::iterator mit;

	_JIF( 0 <= iEnchantLevel && iEnchantLevel <= MAX_ENCHANT_LEVEL, goto err_proc );
    _JIF( fPercent >= 0.0f, goto err_proc );

    mit = m_mapEnchantRate.find( iEnchantLevel );
    if( mit != m_mapEnchantRate.end() )
    {
        START_LOG( cerr, L"������ �ɷ�ġ ��ȭ ���� ���� �ߺ�." )
            << BUILD_LOG( iEnchantLevel )
            << BUILD_LOG( fPercent )
            << END_LOG;
    }

    // ��ġ�� �����.
	m_mapEnchantRate[iEnchantLevel] = fPercent;
	return true;

err_proc:
	START_LOG( cerr, L"������ �ɷ�ġ ��ȭ ���� ���� �ε� ����!" )
		<< BUILD_LOG( iEnchantLevel )
		<< BUILD_LOG( fPercent )	
		<< END_LOG;

	return false;
}

bool CXSLEnchantItemManager::SetEnchantProbability_LUA( int iEnchantLevel )
{
    std::map< int, KLottery >::iterator mit;
    KLottery kLottery;
    //float fProb; ��μ�
	float fProb = 0.f;
    KLuaManager luaMgr( GetLuaState() );

	_JIF( 0 < iEnchantLevel && iEnchantLevel <= MAX_ENCHANT_LEVEL, goto err_proc );

    LUA_GET_VALUE( luaMgr, "Up1", fProb, 0.f );
    _JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
    kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_00, fProb );

    LUA_GET_VALUE( luaMgr, "NoChange", fProb, 0.f );
    _JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
    kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_01, fProb );

    LUA_GET_VALUE( luaMgr, "Down1", fProb, 0.f );
    _JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
    kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_02, fProb );

    LUA_GET_VALUE( luaMgr, "DownTo0", fProb, 0.f );
    _JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
    kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_03, fProb );

    LUA_GET_VALUE( luaMgr, "Break", fProb, 0.f );
    _JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
    kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_04, fProb );

    mit = m_mapEnchantProb.find( iEnchantLevel );
    if( mit != m_mapEnchantProb.end() )
    {
        START_LOG( cerr, L"�ش� ��ȭ �ܰ迡 ���� ��ȭ Ȯ���� �̹� �����Ǿ� ����." )
            << BUILD_LOG( iEnchantLevel )
            << END_LOG;
    }

	//{{ 2010. 05. 31  ������	���� �Ѱ��� �ʱ⿡ ���� ó��
#ifdef SERV_INIT_RANDOM
	const int iRandomCount = ( rand() % 5 ) + 5;

	for( int i = 0; i < iRandomCount; ++i )
	{
		// �ǵ������� ������ n������ �����ϴ�.
		kLottery.Decision();
	}
#endif SERV_INIT_RANDOM
	//}}

    // ��ġ�� �����.
    m_mapEnchantProb[iEnchantLevel] = kLottery;

	return true;

err_proc:
	START_LOG( cerr, L"��ȭ Ȯ�� ���� ��� ����!" )
		<< BUILD_LOG( iEnchantLevel )
		<< BUILD_LOG( fProb )
		<< END_LOG;

	return false;
}

bool CXSLEnchantItemManager::SetEnchantRareProbability_LUA( int iEnchantLevel )
{
	std::map< int, KLottery >::iterator mit;
	KLottery kLottery;
	//float fProb; ��μ�
	float fProb = 0.f;
	KLuaManager luaMgr( GetLuaState() );

	_JIF( 0 < iEnchantLevel && iEnchantLevel <= MAX_ENCHANT_LEVEL, goto err_proc );

	LUA_GET_VALUE( luaMgr, "Up1", fProb, 0.f );
	_JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
	kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_00, fProb );

	LUA_GET_VALUE( luaMgr, "NoChange", fProb, 0.f );
	_JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
	kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_01, fProb );

	LUA_GET_VALUE( luaMgr, "Down1", fProb, 0.f );
	_JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
	kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_02, fProb );

	LUA_GET_VALUE( luaMgr, "DownTo0", fProb, 0.f );
	_JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
	kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_03, fProb );

	LUA_GET_VALUE( luaMgr, "Break", fProb, 0.f );
	_JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
	kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_04, fProb );

	mit = m_mapEnchantRareProb.find( iEnchantLevel );
	if( mit != m_mapEnchantRareProb.end() )
	{
		START_LOG( cerr, L"�ش� ���� ��ȭ �ܰ迡 ���� ��ȭ Ȯ���� �̹� �����Ǿ� ����." )
			<< BUILD_LOG( iEnchantLevel )
			<< END_LOG;
	}

	//{{ 2010. 05. 31  ������	���� �Ѱ��� �ʱ⿡ ���� ó��
#ifdef SERV_INIT_RANDOM
	const int iRandomCount = ( rand() % 5 ) + 5;

	for( int i = 0; i < iRandomCount; ++i )
	{
		// �ǵ������� ������ n������ �����ϴ�.
		kLottery.Decision();
	}
#endif SERV_INIT_RANDOM
	//}}

	// ��ġ�� �����.
	m_mapEnchantRareProb[iEnchantLevel] = kLottery;

	return true;

err_proc:
	START_LOG( cerr, L"���� ��ȭ Ȯ�� ���� ��� ����!" )
		<< BUILD_LOG( iEnchantLevel )
		<< BUILD_LOG( fProb )
		<< END_LOG;

	return false;
}

//{{ 2011. 01. 24	������	�÷���� ��ȭ ���� ����
#ifdef SERV_SUPPORT_MATERIAL_LIMIT
void CXSLEnchantItemManager::SetSupportMaterialEnchantLimitLevel_LUA( IN int iEnchantLimitLevel )
{
	m_iEnchantLimitLevel = iEnchantLimitLevel;

	START_LOG( cout, L"�÷���� ��ȭ ���� ���� ���� : " << m_iEnchantLimitLevel );
}
#endif SERV_SUPPORT_MATERIAL_LIMIT
//}}

//{{ 2008. 11. 16  ������	��ȭ ����
int CXSLEnchantItemManager::GetEnchantStoneItemID( int iEquipLv, int iItemType )
{
	KEStoneInfoKey kKey;
	kKey.first  = iEquipLv;
	kKey.second = iItemType;

	std::map< KEStoneInfoKey, int >::const_iterator	mit;
	mit = m_mapEnchantStoneInfo.find( kKey );
	if( mit == m_mapEnchantStoneInfo.end() )
	{
		START_LOG( cerr, L"��ȭ ������ID�� ã�� �� ����? �Ͼ �� ���� ����!" )
			<< BUILD_LOG( iEquipLv )
			<< BUILD_LOG( iItemType )
			<< END_LOG;
		return 0;
	}

	return mit->second;
}
//}}

//{{ 2008. 12. 14  ������	��ȭ ������
int CXSLEnchantItemManager::GetEnchantSupportMaterialItemID( int iEquipLv )
{
	std::map< int, int >::const_iterator mit;
	mit = m_mapSupportMaterialInfo.find( iEquipLv );
	if( mit == m_mapSupportMaterialInfo.end() )
	{
		START_LOG( cerr, L"��ȭ ������ ������ID�� ã�� �� ����? �Ͼ �� ���� ����!" )
			<< BUILD_LOG( iEquipLv )
			<< END_LOG;
		return 0;
	}

	return mit->second;
}
//}}

//{{ 2008. 12. 21  ������	��ȭ���� ����
int	CXSLEnchantItemManager::GetRestoreItemID( int iEquipLv, int iItemType )
{
	KRestoreInfoKey kKey;
	kKey.first = iItemType;
	kKey.second = iEquipLv;

	std::map< KRestoreInfoKey, int >::const_iterator mit;
	mit = m_mapRestoreItemInfo.find( kKey );
	if( mit == m_mapRestoreItemInfo.end() )
	{
		START_LOG( cerr, L"������ ������ID�� ã�� �� ����? �Ͼ �� ���� ����!" )
			<< BUILD_LOG( iEquipLv )
			<< BUILD_LOG( iItemType )
			<< END_LOG;
		return 0;
	}

	return mit->second;
}
//}}

//{{ 2008. 12. 26  ������	��ȭ �ο�
bool CXSLEnchantItemManager::GetAttachItemEnchantLevel( int iAttachItemID, int& iEnchantLevel, int& iItemType )
{
	std::map< int, KAttachInfo >::const_iterator mit;
	mit = m_mapEnchantAttach.find( iAttachItemID );
	if( mit == m_mapEnchantAttach.end() )
		return false;

	iEnchantLevel = mit->second.m_iEnchantLv;
	iItemType = mit->second.m_iItemType;
	return true;
}
//}}

//{{  2011.12.21     ��μ�    �ں� �÷���� ���� �̺�Ʈ
#ifdef SERV_COBO_SUPPORT_MATERIAL_EVENT
bool CXSLEnchantItemManager::AddEventSupportMaterialInfo_LUA( int iItemID, int iEquipLvMin, int iEquipLvMax )
{
	_JIF( iItemID > 0, goto err_proc );
	_JIF( 0 <= iEquipLvMin && iEquipLvMax <= 100, goto err_proc );

	for( int iIdx = iEquipLvMin; iIdx <= iEquipLvMax; ++iIdx )
	{
#ifdef SERV_EVENT_SUPPORT_MATERIAL_MULTI
		m_multimapEventSupportMaterialInfo.insert( std::make_pair( iIdx, iItemID ) );
#else
		m_mapEventSupportMaterialInfo.insert( std::make_pair( iIdx, iItemID ) );
#endif SERV_EVENT_SUPPORT_MATERIAL_MULTI
	}
	return true;

err_proc:
	START_LOG( cerr, L"�̺�Ʈ ��ȭ ������ ���� �ε� ����!" )
		<< BUILD_LOG( iItemID )
		<< BUILD_LOG( iEquipLvMin )	
		<< BUILD_LOG( iEquipLvMax )
		<< END_LOG;
	return false;
}

#ifdef SERV_EVENT_SUPPORT_MATERIAL_MULTI
void CXSLEnchantItemManager::GetEventEnchantSupportMaterialItemID( int iEquipLv , std::vector< int >& vecItemID )
{
	vecItemID.clear();

	std::multimap< int, int >::iterator mit;
	std::pair< std::multimap< int, int >::iterator , std::multimap< int, int >::iterator > range = m_multimapEventSupportMaterialInfo.equal_range( iEquipLv );
	for( mit = range.first; mit != range.second; ++mit )
	{
		vecItemID.push_back( mit->second );
	}
}
#else
int CXSLEnchantItemManager::GetEventEnchantSupportMaterialItemID( int iEquipLv )
{
	std::map< int, int >::const_iterator mit;
	mit = m_mapEventSupportMaterialInfo.find( iEquipLv );
	if( mit == m_mapEventSupportMaterialInfo.end() )
	{
		START_LOG( clog, L"�̺�Ʈ ��ȭ ������ ������ID�� ã�� �� ����." )
			<< BUILD_LOG( iEquipLv )
			<< END_LOG;
		return 0;
	}

	return mit->second;
}
#endif SERV_EVENT_SUPPORT_MATERIAL_MULTI

#endif SERV_COBO_SUPPORT_MATERIAL_EVENT
//}}

//{{  2011.12.21     ��μ�    �ں� �ູ���� ������ �ֹ��� �̺�Ʈ
#ifdef SERV_COBO_RESTORE_SCROLL_EVENT
bool CXSLEnchantItemManager::AddEventRestoreItemInfo_LUA( int iItemID, int iItemType, int iEquipLvMin, int iEquipLvMax )
{
	_JIF( iItemID > 0, goto err_proc );
	_JIF( 0 <= iEquipLvMin && iEquipLvMax <= 100, goto err_proc );
	_JIF( iItemType >= CXSLItem::IT_NONE  &&  iItemType <= CXSLItem::IT_ETC, goto err_proc );

	for( int iIdx = iEquipLvMin; iIdx <= iEquipLvMax; ++iIdx )
	{
		KRestoreInfoKey kKey;
		kKey.first = iItemType;
		kKey.second = iIdx;

#ifdef SERV_EVENT_RESTORE_SCROLL_MULTI
		m_multimapEventRestoreItemInfo.insert( std::make_pair( kKey, iItemID ) );
#else
		m_mapEventRestoreItemInfo.insert( std::make_pair( kKey, iItemID ) );
#endif SERV_EVENT_RESTORE_SCROLL_MULTI

	}
	return true;

err_proc:
	START_LOG( cerr, L"���Ұ� ������ ���� �ε� ����!" )
		<< BUILD_LOG( iItemID )
		<< BUILD_LOG( iItemType )
		<< BUILD_LOG( iEquipLvMin )	
		<< BUILD_LOG( iEquipLvMax )
		<< END_LOG;
	return false;
}

#ifdef SERV_EVENT_RESTORE_SCROLL_MULTI
void CXSLEnchantItemManager::GetEventRestoreItemID( int iEquipLv, int iItemType , std::vector< int >& vecItemID )
{
	vecItemID.clear();

	KRestoreInfoKey kKey;
	kKey.first = iItemType;
	kKey.second = iEquipLv;

	std::multimap< KRestoreInfoKey, int >::iterator mit;
	std::pair< std::multimap< KRestoreInfoKey, int >::iterator , std::multimap< KRestoreInfoKey, int >::iterator > range = m_multimapEventRestoreItemInfo.equal_range( kKey );
	for( mit = range.first; mit != range.second; ++mit )
	{
		vecItemID.push_back( mit->second );
	}
}
#else
int CXSLEnchantItemManager::GetEventRestoreItemID( int iEquipLv, int iItemType )
{
	KRestoreInfoKey kKey;
	kKey.first = iItemType;
	kKey.second = iEquipLv;

	std::map< KRestoreInfoKey, int >::const_iterator mit;
	mit = m_mapEventRestoreItemInfo.find( kKey );
	if( mit == m_mapEventRestoreItemInfo.end() )
	{
		START_LOG( clog, L"�̺�Ʈ ������ ������ID�� ã�� �� ����." )
			<< BUILD_LOG( iEquipLv )
			<< BUILD_LOG( iItemType )
			<< END_LOG;
		return 0;
	}

	return mit->second;
}
#endif SERV_EVENT_RESTORE_SCROLL_MULTI

#endif SERV_COBO_RESTORE_SCROLL_EVENT
//}}

#ifdef SERV_EVENT_ENCHANT_STONE_MULTI
bool CXSLEnchantItemManager::AddEventEnchantStoneInfo_LUA( int iItemID, int iItemType, int iEquipLvMin, int iEquipLvMax )
{
	_JIF( iItemID > 0 && iItemType > 0 , goto err_proc );
	_JIF( 0 <= iEquipLvMin && iEquipLvMax <= 100, goto err_proc );

	for( int iIdx = iEquipLvMin; iIdx <= iEquipLvMax; ++iIdx )
	{
		KEStoneInfoKey kKey;
		kKey.first  = iIdx;
		kKey.second = iItemType;
		m_multimapEventEnchantStoneInfo.insert( std::make_pair( kKey, iItemID ) );
	}
	return true;

err_proc:
	START_LOG( cerr, L"�̺�Ʈ ��ȭ�� ���� �ε� ����!" )
		<< BUILD_LOG( iItemID )
		<< BUILD_LOG( iEquipLvMin )	
		<< BUILD_LOG( iEquipLvMax )	
		<< END_LOG;
	return false;
}

void CXSLEnchantItemManager::GetEventEnchantStoneItemID( int iEquipLv, int iItemType, std::vector< int >& vecItemID )
{
	vecItemID.clear();

	KEStoneInfoKey kKey;
	kKey.first  = iEquipLv;
	kKey.second = iItemType;

	std::multimap< KEStoneInfoKey, int >::iterator	mit;
	std::pair< std::multimap< KEStoneInfoKey, int >::iterator , std::multimap< KEStoneInfoKey, int >::iterator > range = m_multimapEventEnchantStoneInfo.equal_range( kKey );
	for( mit = range.first; mit != range.second; ++mit )
	{
		vecItemID.push_back( mit->second );
	}
}
#endif SERV_EVENT_ENCHANT_STONE_MULTI

#ifdef SERV_ENCHANT_LIMIT
void CXSLEnchantItemManager::SetEnchantLimitLevel_LUA( IN int iEnchantLimitLevel )
{
	m_iRealEnchantLimitLevel = iEnchantLimitLevel;

	START_LOG( cout, L"��ȭ ���� ���� ���� : " << m_iRealEnchantLimitLevel );
}
#endif SERV_ENCHANT_LIMIT

//{{ 2011.5.23 ���� : ��ȭ�� �ı� ���� ������ �߰�
#ifdef SERV_DESTROY_GUARD_ITEM
bool CXSLEnchantItemManager::AddDestroyGuardInfo_LUA( int iItemID, int iEquipLvMin, int iEquipLvMax )
{
	_JIF( iItemID > 0, goto err_proc );
	_JIF( 0 <= iEquipLvMin && iEquipLvMax <= 100, goto err_proc );

	for( int iIdx = iEquipLvMin; iIdx <= iEquipLvMax; ++iIdx )
	{
		m_mapDestroyGuardInfo.insert( std::make_pair( iIdx, iItemID ) );
	}
	return true;

err_proc:
	START_LOG( cerr, L"��ȭ �ı� ���� ���� �ε� ����!" )
		<< BUILD_LOG( iItemID )
		<< BUILD_LOG( iEquipLvMin )	
		<< BUILD_LOG( iEquipLvMax )
		<< END_LOG;
	return false;
}
int CXSLEnchantItemManager::GetDestroyGuardItemID( int iEquipLv )
{
	std::map< int, int >::const_iterator mit;
	mit = m_mapDestroyGuardInfo.find( iEquipLv );
	if( mit == m_mapDestroyGuardInfo.end() )
	{
		START_LOG( cerr, L"�ı� ���� ������ID�� ã�� �� ����? �Ͼ �� ���� ����!" )
			<< BUILD_LOG( iEquipLv )
			<< END_LOG;
		return 0;
	}

	return mit->second;
}
#endif
//}}

//{{ 2011.5.23 ���� : ��ȭ�� ���� Ȯ�� ���� ������ �߰�
#ifdef SERV_ENCHANT_PLUS_ITEM
bool CXSLEnchantItemManager::AddEnchantPlusInfo_LUA( int iItemID, int iEquipLvMin, int iEquipLvMax )
{
	_JIF( iItemID > 0, goto err_proc );
	_JIF( 0 <= iEquipLvMin && iEquipLvMax <= 100, goto err_proc );

	for( int iIdx = iEquipLvMin; iIdx <= iEquipLvMax; ++iIdx )
	{
		m_mapEnchantPlusInfo.insert( std::make_pair( iIdx, iItemID ) );
	}
	return true;

err_proc:
	START_LOG( cerr, L"��ȭ Ȯ�� ���� ������ ���� �ε� ����!" )
		<< BUILD_LOG( iItemID )
		<< BUILD_LOG( iEquipLvMin )	
		<< BUILD_LOG( iEquipLvMax )
		<< END_LOG;
	return false;
}

bool CXSLEnchantItemManager::SetEnchantPlusProbability_LUA( int iEnchantLevel )
{
	std::map< int, KLottery >::iterator mit;
	KLottery kLottery;
	float fProb;
	KLuaManager luaMgr( GetLuaState() );

	_JIF( 0 < iEnchantLevel && iEnchantLevel <= ENCHANT_INFO::MAX_ENCHANT_LEVEL, goto err_proc );

	LUA_GET_VALUE( luaMgr, "Up1", fProb, 0.f );
	_JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
	kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_00, fProb );

	LUA_GET_VALUE( luaMgr, "NoChange", fProb, 0.f );
	_JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
	kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_01, fProb );

	LUA_GET_VALUE( luaMgr, "Down1", fProb, 0.f );
	_JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
	kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_02, fProb );

	LUA_GET_VALUE( luaMgr, "DownTo0", fProb, 0.f );
	_JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
	kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_03, fProb );

	LUA_GET_VALUE( luaMgr, "Break", fProb, 0.f );
	_JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
	kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_04, fProb );

	mit = m_mapEnchantPlusProb.find( iEnchantLevel );
	if( mit != m_mapEnchantPlusProb.end() )
	{
		START_LOG( cerr, L"�ش� ��ȭ �ܰ迡 ���� ��ȭ Ȯ���� �̹� �����Ǿ� ����." )
			<< BUILD_LOG( iEnchantLevel )
			<< END_LOG;
	}

	//{{ 2010. 05. 31  ������	���� �Ѱ��� �ʱ⿡ ���� ó��
#ifdef SERV_INIT_RANDOM
	const int iRandomCount = ( rand() % 5 ) + 5;

	for( int i = 0; i < iRandomCount; ++i )
	{
		// �ǵ������� ������ n������ �����ϴ�.
		kLottery.Decision();
	}
#endif SERV_INIT_RANDOM
	//}}

	// ��ġ�� �����.
	m_mapEnchantPlusProb[iEnchantLevel] = kLottery;

	return true;

err_proc:
	START_LOG( cerr, L"��ȭ Ȯ�� ���� ��� ����!" )
		<< BUILD_LOG( iEnchantLevel )
		<< BUILD_LOG( fProb )
		<< END_LOG;

	return false;
}

int CXSLEnchantItemManager::GetEnchantPlusItemID( int iEquipLv )
{
	std::map< int, int >::const_iterator mit;
	mit = m_mapEnchantPlusInfo.find( iEquipLv );
	if( mit == m_mapEnchantPlusInfo.end() )
	{
		START_LOG( cerr, L"��ȭ Ȯ�� ���� ������ID�� ã�� �� ����? �Ͼ �� ���� ����!" )
			<< BUILD_LOG( iEquipLv )
			<< END_LOG;
		return 0;
	}

	return mit->second;
}
#endif SERV_ENCHANT_PLUS_ITEM
//}}

//{{ 2012. 01. 19	��μ�	�÷���� ���� ��ȭ �̺�Ʈ �ǽð� ����
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
void CXSLEnchantItemManager::SetEventSupportMaterialEnchantLimitLevel_LUA( IN int iEnchantLimitLevel )
{
	m_iEventEnchantLimitLevel = iEnchantLimitLevel;

	START_LOG( cout, L"�̺�Ʈ �÷���� ��ȭ ���� ���� ���� : " << m_iEventEnchantLimitLevel );
}

bool CXSLEnchantItemManager::SetEnchantProbability_Event_LUA( int iEnchantLevel )
{
	std::map< int, KLottery >::iterator mit;
	KLottery kLottery;
	float fProb = 0.f;
	KLuaManager luaMgr( GetLuaState() );

	_JIF( 0 < iEnchantLevel && iEnchantLevel <= MAX_ENCHANT_LEVEL, goto err_proc );

	LUA_GET_VALUE( luaMgr, "Up1", fProb, 0.f );
	_JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
	kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_00, fProb );

	LUA_GET_VALUE( luaMgr, "NoChange", fProb, 0.f );
	_JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
	kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_01, fProb );

	LUA_GET_VALUE( luaMgr, "Down1", fProb, 0.f );
	_JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
	kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_02, fProb );

	LUA_GET_VALUE( luaMgr, "DownTo0", fProb, 0.f );
	_JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
	kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_03, fProb );

	LUA_GET_VALUE( luaMgr, "Break", fProb, 0.f );
	_JIF( 0.f <= fProb && fProb <= 100.f, goto err_proc );
	kLottery.AddCaseIntegerCast( NetError::ERR_ENCHANT_RESULT_04, fProb );

	mit = m_mapEventEnchantProb.find( iEnchantLevel );
	if( mit != m_mapEventEnchantProb.end() )
	{
		START_LOG( cerr, L"�ش� �̺�Ʈ ��ȭ �ܰ迡 ���� ��ȭ Ȯ���� �̹� �����Ǿ� ����." )
			<< BUILD_LOG( iEnchantLevel )
			<< END_LOG;
	}

	const int iRandomCount = ( rand() % 5 ) + 5;

	for( int i = 0; i < iRandomCount; ++i )
	{
		// �ǵ������� ������ n������ �����ϴ�.
		kLottery.Decision();
	}

	// ��ġ�� �����.
	m_mapEventEnchantProb[iEnchantLevel] = kLottery;

	return true;

err_proc:
	START_LOG( cerr, L"�̺�Ʈ ��ȭ Ȯ�� ���� ��� ����!" )
		<< BUILD_LOG( iEnchantLevel )
		<< BUILD_LOG( fProb )
		<< END_LOG;

	return false;
}

bool CXSLEnchantItemManager::GetEventEnchantResult( IN bool bIsRare, IN int iEnchantLevel, OUT int& iResult )
{
	std::map< int, KLottery >::const_iterator mit;
	if( bIsRare )
	{
		mit = m_mapEnchantRareProb.find( iEnchantLevel );
		if( mit == m_mapEnchantRareProb.end() )
		{
			START_LOG( cerr, L"���� ��ȭ �ܰ� ���� ã�� ����.!" )
				<< BUILD_LOG( iEnchantLevel )
				<< END_LOG;

			return false;
		}
	}
	else
	{
		mit = m_mapEventEnchantProb.find( iEnchantLevel );
		if( mit == m_mapEventEnchantProb.end() )
		{
			START_LOG( cerr, L"��ȭ �ܰ� ���� ã�� ����.!" )
				<< BUILD_LOG( iEnchantLevel )
				<< END_LOG;

			return false;
		}
	}

	iResult = mit->second.Decision();
	if( iResult < NetError::ERR_ENCHANT_RESULT_00 || iResult > NetError::ERR_ENCHANT_RESULT_04 )
	{
		START_LOG( cerr, L"��ȭ ��� �̻�." )
			<< BUILD_LOG( iResult )
			<< BUILD_LOG( NetError::GetErrStr( iResult ) )
			<< END_LOG;

		// ��ȭ �������� ó��.
		iResult = NetError::ERR_ENCHANT_RESULT_01;
	}

	return true;
}
#endif SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
//}}

//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
bool CXSLEnchantItemManager::AddDropRandomEnchantInfo_LUA( int iEnchantLevel, float fRate )
{
	_JIF( iEnchantLevel >= 0 , goto err_proc );
	_JIF( 100.f >= fRate  &&  fRate >= 0.f, goto err_proc );

	// ��ӿ� ���� ���� ��ȭ ���� �߰�
	LIF( m_kRandomEnchant[RET_DROP].AddCase( iEnchantLevel, fRate ) );
	return true;
	
err_proc:
	START_LOG( cerr, L"�߸��� ���� ��ȭ �����Դϴ�" )
		<< BUILD_LOG( iEnchantLevel )
		<< BUILD_LOG( fRate )
		<< END_LOG;
	return false;
}

bool CXSLEnchantItemManager::AddCubeRandomEnchantInfo_LUA( int iEnchantLevel, float fRate )
{
	_JIF( iEnchantLevel >= 0 , goto err_proc );
	_JIF( 100.f >= fRate  &&  fRate >= 0.f, goto err_proc );

	// ť�꿡 ���� ���� ��ȭ ���� �߰�
	LIF( m_kRandomEnchant[RET_CUBE].AddCase( iEnchantLevel, fRate ) );
	return true;

err_proc:
	START_LOG( cerr, L"�߸��� ���� ��ȭ �����Դϴ�" )
		<< BUILD_LOG( iEnchantLevel )
		<< BUILD_LOG( fRate )
		<< END_LOG;
	return false;
}

bool CXSLEnchantItemManager::RandomEnchant( IN const int iItemID
										  , IN const RANDOM_ENCHANT_TYPE eType
										  , IN OUT char& cTimeEnchantEventLevel
										  , OUT char& cEnchantLevel
										  )
{
	if( CheckRandomEnchant( iItemID, eType ) == false )
	{
		return false;
	}

	if( 0 < cTimeEnchantEventLevel )
	{
		cEnchantLevel			= cTimeEnchantEventLevel;
		cTimeEnchantEventLevel	= 0;
		return true;
	}

	int iEnchantLevelResult = m_kRandomEnchant[eType].Decision();
	if( iEnchantLevelResult == KLottery::CASE_BLANK )
	{
		iEnchantLevelResult = 0;
	}

	cEnchantLevel = static_cast<char>(iEnchantLevelResult);
	return true;
}

bool CXSLEnchantItemManager::CheckRandomEnchant( IN const int iItemID, IN const RANDOM_ENCHANT_TYPE eType ) const
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( IS_NULL( pItemTemplet ) )
	{
		START_LOG( cerr, L"���� ���� �ʴ� ������ id�Դϴ�." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return false;
	}

	if( eType >= RET_MAX  ||  eType < 0 )
	{
		START_LOG( cerr, L"�߸��� ���� ��ȭ Ÿ���Դϴ�." )
			<< BUILD_LOG( eType )
			<< END_LOG;
		return false;
	}

	if( pItemTemplet->m_bFashion == true )
	{
		return false;
	}

	if( ( pItemTemplet->m_ItemType != CXSLItem::IT_WEAPON )		&&
		( pItemTemplet->m_ItemType != CXSLItem::IT_DEFENCE )
		)
	{
		return false;
	}

	switch( pItemTemplet->m_EqipPosition )
	{
	case CXSLUnit::EP_WEAPON_HAND:
	case CXSLUnit::EP_DEFENCE_BODY:
	case CXSLUnit::EP_DEFENCE_LEG:
	case CXSLUnit::EP_DEFENCE_HAND:
	case CXSLUnit::EP_DEFENCE_FOOT:
		break;

	default:
		return false;
	}

	return true;
}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}

#ifdef SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM// �۾���¥: 2013-07-02	// �ڼ���
bool CXSLEnchantItemManager::CheckAttachItemEnableLevel( IN const int iAttachItemID, IN const int iUseLevel ) const
{
	std::map< int, KAttachInfo >::const_iterator mit = m_mapEnchantAttach.find( iAttachItemID );
	if( mit == m_mapEnchantAttach.end() )
		return false;

	if( ( mit->second.m_iMinItemLevel == 0 ) && ( mit->second.m_iMaxItemLevel == 0 ) )
	{
		return true;
	}
	else if( iUseLevel < mit->second.m_iMinItemLevel )
	{
		return false;
	}
	else if( mit->second.m_iMaxItemLevel < iUseLevel )
	{
		return false;
	}
	else
	{
		return true;
	}
}
#endif // SERV_ADD_ITEM_LEVEL_CONDITION_FOR_ENCHANT_ATTACH_ITEM