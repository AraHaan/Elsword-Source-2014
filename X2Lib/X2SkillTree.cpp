#include "StdAfx.h"
#include ".\x2skilltree.h"

//{{ �ֹ�ö [2013/1/4]  ���ӳ� ���� ��Ʈ���� �������Ϸ� ���
#ifdef PRINT_INGAMEINFO_TO_EXCEL
#include "BasicExcel.hpp"
using namespace YExcel;
#endif PRINT_INGAMEINFO_TO_EXCEL
//}} �ֹ�ö [2013/1/4]  ���ӳ� ���� ��Ʈ���� �������Ϸ� ���

/*static*/ const WCHAR* CX2SkillTree::GetAccelCoreParticleName( const SKILL_ID eAccelSkillID_ )
{
	switch ( eAccelSkillID_ )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	case CX2SkillTree::SI_SA_COMMON_AURA_POWER_ACCEL:
		return L"BuffAttackCore";
		break;

	case CX2SkillTree::SI_SA_COMMON_AURA_MAGIC_ACCEL:
		return L"BuffMagickCore";
		break;

	case CX2SkillTree::SI_SA_COMMON_AURA_SHIELD_ACCEL:
		return L"ShieldBuffCore";
		break;
#else //UPGRADE_SKILL_SYSTEM_2013
	case CX2SkillTree::SI_SA_COMMON_POWER_ACCEL:
	case CX2SkillTree::SI_SA_COMMON_AURA_POWER_ACCEL:
	case CX2SkillTree::SI_SA_COMMON_POWER_ADRENALIN:
		return L"BuffAttackCore";
		break;

	case CX2SkillTree::SI_SA_COMMON_MAGIC_ACCEL:
	case CX2SkillTree::SI_SA_COMMON_AURA_MAGIC_ACCEL:
	case CX2SkillTree::SI_SA_COMMON_MAGIC_ADRENALIN:
		return L"BuffMagickCore";
		break;

	case CX2SkillTree::SI_SA_COMMON_SHIELD_ACCEL:
	case CX2SkillTree::SI_SA_COMMON_AURA_SHIELD_ACCEL:
	case CX2SkillTree::SI_SA_COMMON_SHIELD_ADRENALIN:
		return L"ShieldBuffCore";
		break;
#endif //UPGRADE_SKILL_SYSTEM_2013

	case CX2SkillTree::SI_SA_COMMON_AURA_SPEED_ACCEL:
		return L"BuffSpeed_Core";
		break;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ���� ��ų ����, ������
	case CX2SkillTree::SI_SA_COMMON_AURA_CRITICAL_ACCEL:				
		return L"BuffCritical_Core";
		break;
	case CX2SkillTree::SI_SA_COMMON_AURA_ADDATK_ACCEL:
		return L"BuffAddAtk_Core";
		break;

	case CX2SkillTree::SI_SA_COMMON_AURA_EL_DEFENCE_ACCEL:			
		return L"BuffElDefence_Core";
		break;
#endif // UPGRADE_SKILL_SYSTEM_2013 // ���� ��ų ����, ������

	default:
		return NULL;
		break;
	}
}




CX2SkillTree::CX2SkillTree(void)
//{{ kimhc // 2010.7.29 // ���Խ� ���� ��ų ���� ����
#ifdef	MODIFY_INFINITE_SKILL_BUG_WHEN_INTRUDE
: m_bSkillSlotChanging( false )			// ��ų����ü���� ���ΰ�? (SkillSlotChangeReq�� ������ �� Ack�� ���� ���� ���� �����ΰ�?)
#endif	MODIFY_INFINITE_SKILL_BUG_WHEN_INTRUDE
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
, m_iUsedSPoint( 0 )
, m_iUsedCSPoint( 0 )
#endif // UPGRADE_SKILL_SYSTEM_2013
//}} kimhc // 2010.7.29 // ���Խ� ���� ��ų ���� ����
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	m_mapGetSkillInfo.clear();
#endif // UPGRADE_SKILL_SYSTEM_2013
#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
	m_vecNowLearnSkill.clear();
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL
}

CX2SkillTree::~CX2SkillTree(void)
{
	BOOST_TEST_FOREACH( SkillTempletMap::value_type&, value, m_mapSkillTemplet )
	{
		SAFE_DELETE( value.second );
	}
	m_mapSkillTemplet.clear();
}


bool CX2SkillTree::OpenScriptFile( const WCHAR* pFileName, const WCHAR* pSkillDataFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pSkillTree", this );

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	/// ������ ���� ������ ���� ��� ����, SkillData.lua�� ���� �Ľ��Ѵ�.
	if( g_pKTDXApp->LoadLuaTinker( pSkillDataFileName ) == false )
	{
		ErrorLogMsg( XEM_ERROR96, pSkillDataFileName );
		return false;
	}
#endif // UPGRADE_SKILL_SYSTEM_2013

	if( g_pKTDXApp->LoadLuaTinker( pFileName ) == false )
	{
		ErrorLogMsg( XEM_ERROR96, pFileName );
		return false;
	}
	
#ifdef LUA_TRANS_DEVIDE
#ifdef UPGRADE_SKILL_SYSTEM_2013
	if( g_pKTDXApp->LoadLuaTinker( L"NewSkillTempletVer2Trans.lua" ) == false )
	{
		ErrorLogMsg( XEM_ERROR96, L"NewSkillTempletVer2Trans.lua" );
		return false;
	}
#else //UPGRADE_SKILL_SYSTEM_2013
	if( g_pKTDXApp->LoadLuaTinker( L"NewSkillTempletTrans.lua" ) == false )
	{
		ErrorLogMsg( XEM_ERROR96, L"NewSkillTempletTrans.lua" );
		return false;
	}
#endif //UPGRADE_SKILL_SYSTEM_2013
#endif LUA_TRANS_DEVIDE

#ifndef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	/// ������ ���� ������ ���� ��� ����, SkillData.lua�� ���� �Ľ��ϵ��� ��ġ �̵�
	if( g_pKTDXApp->LoadLuaTinker( pSkillDataFileName ) == false )
	{
		ErrorLogMsg( XEM_ERROR96, pSkillDataFileName );
		return false;
	}
#endif // UPGRADE_SKILL_SYSTEM_2013

	OpenScriptFilePostProcess();

	return true;
}


void CX2SkillTree::OpenScriptFilePostProcess()
{

	BOOST_TEST_FOREACH( UnitSkillTreeTempletMap::value_type&, value, m_mapUnitSkillTreeTemplet )
	{
		SkillTreeTempletMap& mapSkillTreeTemplet = value.second;
		BOOST_TEST_FOREACH( SkillTreeTempletMap::value_type&, value2, mapSkillTreeTemplet )
		{
			SkillTreeTemplet& skillTreeTemplet = value2.second;
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
			skillTreeTemplet.m_iFollowingSkill = 0;
#else // UPGRADE_SKILL_SYSTEM_2013
			skillTreeTemplet.m_vecFollowingSkill.resize(0);
#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}



	BOOST_TEST_FOREACH( UnitSkillTreeTempletMap::value_type&, value, m_mapUnitSkillTreeTemplet )
	{
		SkillTreeTempletMap& mapSkillTreeTemplet = value.second;
		BOOST_TEST_FOREACH( SkillTreeTempletMap::value_type&, value2, mapSkillTreeTemplet )
		{
			SkillTreeTemplet& skillTreeTemplet = value2.second;
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
			const SkillTreeTemplet* pPrecedingSkillTreeTemplet = GetSkillTreeTemplet( value.first, static_cast<SKILL_ID>( skillTreeTemplet.m_iPrecedingSkill ) );
			if( NULL == pPrecedingSkillTreeTemplet )
				continue;

			SkillTreeTemplet* pPrecedingSkillTemplet_NotConst = const_cast<SkillTreeTemplet*>( pPrecedingSkillTreeTemplet );
			pPrecedingSkillTemplet_NotConst->m_iFollowingSkill= value2.first;
#else // UPGRADE_SKILL_SYSTEM_2013
			BOOST_TEST_FOREACH( int, iPrecedingSkillID, skillTreeTemplet.m_vecPrecedingSkill )
			{
				const SkillTreeTemplet* pPrecedingSkillTreeTemplet = GetSkillTreeTemplet( value.first, (SKILL_ID) iPrecedingSkillID );
				if( NULL == pPrecedingSkillTreeTemplet )
					continue;

				SkillTreeTemplet* pPrecedingSkillTemplet_NotConst = const_cast<SkillTreeTemplet*>( pPrecedingSkillTreeTemplet );
				pPrecedingSkillTemplet_NotConst->m_vecFollowingSkill.push_back( value2.first );
			}
#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}

	//{{ oasis907 : ����� [2009.12.4] //

	BOOST_TEST_FOREACH( GuildSkillTreeTempletMap::value_type&, value, m_mapGuildSkillTreeTemplet )
	{
		SkillTreeTempletMap& mapSkillTreeTemplet = value.second;
		BOOST_TEST_FOREACH( SkillTreeTempletMap::value_type&, value2, mapSkillTreeTemplet )
		{
			SkillTreeTemplet& skillTreeTemplet = value2.second;
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
			skillTreeTemplet.m_iFollowingSkill = 0;
#else // UPGRADE_SKILL_SYSTEM_2013
			skillTreeTemplet.m_vecFollowingSkill.resize(0);
#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}



	BOOST_TEST_FOREACH( GuildSkillTreeTempletMap::value_type&, value, m_mapGuildSkillTreeTemplet )
	{
		SkillTreeTempletMap& mapSkillTreeTemplet = value.second;
		BOOST_TEST_FOREACH( SkillTreeTempletMap::value_type&, value2, mapSkillTreeTemplet )
		{
			SkillTreeTemplet& skillTreeTemplet = value2.second;
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
				const SkillTreeTemplet* pPrecedingSkillTreeTemplet = GetGuildSkillTreeTemplet( value.first, static_cast<SKILL_ID>( skillTreeTemplet.m_iPrecedingSkill ) );
				if( NULL == pPrecedingSkillTreeTemplet )
					continue;

				SkillTreeTemplet* pPrecedingSkillTemplet_NotConst = const_cast<SkillTreeTemplet*>( pPrecedingSkillTreeTemplet );
				pPrecedingSkillTemplet_NotConst->m_iFollowingSkill = value2.first;
#else // UPGRADE_SKILL_SYSTEM_2013
			BOOST_TEST_FOREACH( int, iPrecedingSkillID, skillTreeTemplet.m_vecPrecedingSkill )
			{
				const SkillTreeTemplet* pPrecedingSkillTreeTemplet = GetGuildSkillTreeTemplet( value.first, (SKILL_ID) iPrecedingSkillID );
				if( NULL == pPrecedingSkillTreeTemplet )
					continue;

				SkillTreeTemplet* pPrecedingSkillTemplet_NotConst = const_cast<SkillTreeTemplet*>( pPrecedingSkillTreeTemplet );
				pPrecedingSkillTemplet_NotConst->m_vecFollowingSkill.push_back( value2.first );
			}
#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}
	//}} oasis907 : ����� [2009.12.4] //
}




bool CX2SkillTree::AddSkillTreeTemplet_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����

	CX2Unit::UNIT_CLASS eUnitClass = CX2Unit::UC_NONE;
	SKILL_ID eSkillID = SI_NONE;
	LUA_GET_VALUE_ENUM( luaManager, "m_eUnitClass",				eUnitClass,				CX2Unit::UNIT_CLASS,		CX2Unit::UC_NONE		);
	LUA_GET_VALUE_ENUM( luaManager, "m_iSkillID",				eSkillID,				SKILL_ID,					SI_NONE					);

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	const SkillTemplet* pSkillTemplet = GetSkillTemplet( eSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	const SkillTemplet* pSkillTemplet = GetSkillTemplet( eSkillID, 1 );
#endif // UPGRADE_SKILL_SYSTEM_2013
	if( NULL == pSkillTemplet )
	{
		StateLog( L"skilltemplet is null on loading skilltree templet" );
		return false;
	}

	SkillTreeTemplet skillTreeTemplet;
	LUA_GET_VALUE( luaManager,		"m_iMasterLevel",			skillTreeTemplet.m_iMaxSkillLevel,	0				);
	LUA_GET_VALUE( luaManager,		"m_iRowIndex",				skillTreeTemplet.m_iTier,			0				);
	LUA_GET_VALUE( luaManager,		"m_iColumnIndex",			skillTreeTemplet.m_iIndexInTier,	0				);

	int iPrecedingSkillID = 0;
	LUA_GET_VALUE( luaManager,		"m_iPrecedingSkill",		skillTreeTemplet.m_iPrecedingSkill,	0				);


	SkillTreeTempletMap& mapSkillTreeTemplet = m_mapUnitSkillTreeTemplet[ (int)eUnitClass ];
	mapSkillTreeTemplet[ eSkillID ] = skillTreeTemplet;

#else // UPGRADE_SKILL_SYSTEM_2013

	CX2Unit::UNIT_CLASS eUnitClass = CX2Unit::UC_NONE;
	SKILL_ID eSkillID = SI_NONE;
	LUA_GET_VALUE_ENUM( luaManager, "UNIT_CLASS",				eUnitClass,				CX2Unit::UNIT_CLASS,		CX2Unit::UC_NONE		);
	LUA_GET_VALUE_ENUM( luaManager, "SKILL_ID",					eSkillID,				SKILL_ID,					SI_NONE					);

	const SkillTemplet* pSkillTemplet = GetSkillTemplet( eSkillID, 1 );
	if( NULL == pSkillTemplet )
	{
		StateLog( L"skilltemplet is null on loading skilltree templet" );
		return false;
	}
	
			
	SkillTreeTemplet skillTreeTemplet;
	LUA_GET_VALUE( luaManager,		"MAX_SKILL_LEVEL",			skillTreeTemplet.m_iMaxSkillLevel,	0				);
	LUA_GET_VALUE( luaManager,		"TIER_INDEX",				skillTreeTemplet.m_iTier,			0				);
	LUA_GET_VALUE( luaManager,		"INDEX_IN_TIER",			skillTreeTemplet.m_iIndexInTier,	0				);



	std::wstring wstrPrecedingSkill = L"";
	LUA_GET_VALUE( luaManager,		"m_vecPrecedingSkill",		wstrPrecedingSkill, 		L""		);

	if( false == wstrPrecedingSkill.empty() )
	{
		WCHAR* pBegin = const_cast<WCHAR*>(wstrPrecedingSkill.c_str());
		WCHAR* pEnd = pBegin + wstrPrecedingSkill.length();


		while( true )
		{
			int iSkillID = TokenizeInt( &pBegin, pEnd );
			if( iSkillID > 0 )
				skillTreeTemplet.m_vecPrecedingSkill.push_back( iSkillID );
			else 
				break;
		}
	}
	

	SkillTreeTempletMap& mapSkillTreeTemplet = m_mapUnitSkillTreeTemplet[ (int)eUnitClass ];
	mapSkillTreeTemplet[ eSkillID ] = skillTreeTemplet;

#endif // UPGRADE_SKILL_SYSTEM_2013

	return true;

}


bool CX2SkillTree::AddSkillTemplet_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING


	bool bInHouseTestOnly = false;
	LUA_GET_VALUE( luaManager,		"IN_HOUSE_TEST_ONLY",				bInHouseTestOnly,	false );


	
#if defined( _SERVICE_ ) || defined( _OPEN_TEST_ ) || defined( _FOREIGN_ ) || defined( _IN_HOUSE_SERVICE_READY_QA_ )
	if( true == bInHouseTestOnly )
		return false;
#endif // defined( _SERVICE_ ) || defined( _OPEN_TEST_ ) || defined( _FOREIGN_ ) || defined( _IN_HOUSE_SERVICE_READY_QA_ )

#ifdef UPGRADE_SKILL_SYSTEM_2013

	SkillTemplet* pSkillTemplet = new SkillTemplet();

	LUA_GET_VALUE_ENUM( luaManager,		"m_eType",						pSkillTemplet->m_eType,							SKILL_TYPE,			ST_NONE					);

	/// ��� ��ų ����
	switch ( pSkillTemplet->m_eType )
	{
	case ST_GUILD_PASSIVE_PHYSIC_ATTACK_MAGIC_DEFENCE:
	case ST_GUILD_PASSIVE_MAGIC_ATTCK_PHYSIC_DEFENCE:
	case ST_GUILD_PASSIVE:
		{
			pSkillTemplet->m_bGuildSkill = true;
		} break;
	}

	LUA_GET_VALUE_ENUM( luaManager,		"m_eID",						pSkillTemplet->m_eID,							SKILL_ID,			SI_NONE					);

	if( true == luaManager.BeginTable( "m_SkillCoolTime" ) )			SetSkillVlaue( luaManager,						pSkillTemplet->m_vecSkillCoolTime		);
	if( true == luaManager.BeginTable( "m_MPConsumption" ) )			SetSkillVlaue( luaManager,						pSkillTemplet->m_vecMPConsumption		);
	if( true == luaManager.BeginTable( "m_RequireCharacterLevel" ) )	SetSkillVlaue( luaManager,						pSkillTemplet->m_vecRequireCharacterLevel	);

//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	LUA_GET_VALUE( luaManager,			"m_usCBConsumption",			pSkillTemplet->m_usCBConsumption,				0		);
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
	LUA_GET_VALUE( luaManager,			"m_usFPGainCount",				pSkillTemplet->m_usFPGainCount,					0		);
	LUA_GET_VALUE( luaManager,			"m_usFPConsumtion",				pSkillTemplet->m_usFPConsumtion,				0		);
#endif

	LUA_GET_VALUE( luaManager,			"m_wstrName",					pSkillTemplet->m_wstrName,						L""		);

	LUA_GET_VALUE( luaManager,			"m_wstrMainDesc",				pSkillTemplet->m_wstrMainDesc,					L""		);

#ifdef HIDE_DO_NOT_EXIST_SKILL_EFFECTIVE_DESC // ����ȯ
	/// ��ų ȿ�� ������ 30�������� ���� �Ǿ� ���� �ʴٸ�, �ڵ����� ä���� ����.
	if( true == luaManager.BeginTable( "m_EffectiveDesc" ) )			SetSkillVlaue( luaManager,						pSkillTemplet->m_vecEffectiveDesc, false);
#else //HIDE_DO_NOT_EXIST_SKILL_EFFECTIVE_DESC
	if( true == luaManager.BeginTable( "m_EffectiveDesc" ) )			SetSkillVlaue( luaManager,						pSkillTemplet->m_vecEffectiveDesc		);
#endif //HIDE_DO_NOT_EXIST_SKILL_EFFECTIVE_DESC

	LUA_GET_VALUE( luaManager,			"m_bBornSealed",				pSkillTemplet->m_bBornSealed,					false	);

	LUA_GET_VALUE( luaManager,			"m_wstrIconTextureName",		pSkillTemplet->m_wstrIconName,					L""		);
	LUA_GET_VALUE( luaManager,			"m_wstrIconTexturePieceName",	pSkillTemplet->m_wstrIconPieceName,				L""		);
	LUA_GET_VALUE( luaManager,			"m_wstrImageTextureName",		pSkillTemplet->m_wstrImageName,					L""		);
	LUA_GET_VALUE( luaManager,			"m_wstrImageTexturePieceName",	pSkillTemplet->m_wstrImagePieceName,			L""		);

	SKILL_POWER_RATE_TYPE ePowerRate = SPRT_BASIC_CLASS;
	LUA_GET_VALUE_ENUM( luaManager,		"m_ePowerRate",					ePowerRate,										SKILL_POWER_RATE_TYPE,			SPRT_BASIC_CLASS	);
	SetSkillPowerRate( ePowerRate, pSkillTemplet->m_vecPowerRate );
#ifdef SKILL_LEVEL_UP_BY_POWER_RATE_TYPE
	pSkillTemplet->m_eSkillPowerRateType = ePowerRate;
#endif //SKILL_LEVEL_UP_BY_POWER_RATE_TYPE

	LUA_GET_VALUE_ENUM( luaManager,		"m_eActiveSkillUseCondtion",	pSkillTemplet->m_eActiveSkillUseCondtion,		ACTIVE_SKILL_USE_CONDITION,		ASUT_NONE	);

	LUA_GET_VALUE( luaManager,			"m_bShowSkillLevel",			pSkillTemplet->m_bShowSkillLevel,				false	);

	LUA_GET_VALUE( luaManager,			"m_iRequireLearnSkillPoint",	pSkillTemplet->m_iRequireLearnSkillPoint,		0		);

	LUA_GET_VALUE( luaManager,			"m_iRequireUpgradeSkillPoint",	pSkillTemplet->m_iRequireUpgradeSkillPoint,		0		);

	LUA_GET_VALUE( luaManager,			"m_usWOSType",					static_cast<int>( pSkillTemplet->m_iWOSType ),	0		);

#ifdef NO_MORE_ERROR_TEST
	if( true == bInHouseTestOnly )
	{
		InHouseLog( L"�系���� ����Ǵ� skilltemplet�� �ֽ��ϴ�." );
		InHouseLog( pSkillTemplet->m_wstrName.c_str() );
	}
#endif NO_MORE_ERROR_TEST

	SetSkillStat( luaManager, pSkillTemplet->m_vecStat );

	std::map<SKILL_ID, SkillTemplet*>::iterator it = m_mapSkillTemplet.find( pSkillTemplet->m_eID );
	if( it == m_mapSkillTemplet.end() )
	{
		m_mapSkillTemplet[ pSkillTemplet->m_eID ] = pSkillTemplet;
	}
	else
	{
		ASSERT( !"SKILL is Duplication!" );	/// ��ų �ߺ�!
		return true;
	}

	float fValue = 0.f;

	if( true == luaManager.BeginTable( "SA_EFFECTIVE_TIME" ) )					SetSkillAbilityMap( &luaManager, SA_EFFECTIVE_TIME, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_EFFECTIVE_COUNT" ) )          		SetSkillAbilityMap( &luaManager, SA_EFFECTIVE_COUNT, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_SKILL_ID" ) )                 		SetSkillAbilityMap( &luaManager, SA_SKILL_ID, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_COMBO_ID" ) )                 		SetSkillAbilityMap( &luaManager, SA_COMBO_ID, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_EFFECTIVE_RATE" ) )           		SetSkillAbilityMap( &luaManager, SA_EFFECTIVE_RATE, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_ATK_REL" ) )                  		SetSkillAbilityMap( &luaManager, SA_ATK_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_ATK_PHYSIC_REL" ) )           		SetSkillAbilityMap( &luaManager, SA_ATK_PHYSIC_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_ATK_MAGIC_REL" ) )            		SetSkillAbilityMap( &luaManager, SA_ATK_MAGIC_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_DEF_PHYSIC_REL" ) )           		SetSkillAbilityMap( &luaManager, SA_DEF_PHYSIC_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_DEF_MAGIC_REL" ) )            		SetSkillAbilityMap( &luaManager, SA_DEF_MAGIC_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_TEAM_ATK_PHYSIC_REL" ) )      		SetSkillAbilityMap( &luaManager, SA_TEAM_ATK_PHYSIC_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_TEAM_ATK_MAGIC_REL" ) )       		SetSkillAbilityMap( &luaManager, SA_TEAM_DEF_PHYSIC_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_TEAM_DEF_PHYSIC_REL" ) )      		SetSkillAbilityMap( &luaManager, SA_TEAM_DEF_PHYSIC_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_TEAM_DEF_MAGIC_REL" ) )       		SetSkillAbilityMap( &luaManager, SA_TEAM_DEF_MAGIC_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_DAMAGE_REL" ) )               		SetSkillAbilityMap( &luaManager, SA_DAMAGE_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_DAMAGED_REL" ) )              		SetSkillAbilityMap( &luaManager, SA_DAMAGED_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_UNFIXED_DEFENSE_REL" ) )      		SetSkillAbilityMap( &luaManager, SA_UNFIXED_DEFENSE_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_SUMMON_ATK_REL" ) )           		SetSkillAbilityMap( &luaManager, SA_SUMMON_ATK_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_MAX_HP_ABS" ) )               		SetSkillAbilityMap( &luaManager, SA_MAX_HP_ABS, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_MAX_HP_REL" ) )               		SetSkillAbilityMap( &luaManager, SA_MAX_HP_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_MAX_MP_ABS" ) )               		SetSkillAbilityMap( &luaManager, SA_MAX_MP_ABS, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_MAX_MP_REL" ) )               		SetSkillAbilityMap( &luaManager, SA_MAX_MP_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_TELEPORT_RANGE_ABS" ) )       		SetSkillAbilityMap( &luaManager, SA_TELEPORT_RANGE_ABS, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_CHARGING_TIME" ) )            		SetSkillAbilityMap( &luaManager, SA_CHARGING_TIME, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_HP_REL_TO_MP_ABS_1" ) )       		SetSkillAbilityMap( &luaManager, SA_HP_REL_TO_MP_ABS_1, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_HP_REL_TO_MP_ABS_2" ) )       		SetSkillAbilityMap( &luaManager, SA_HP_REL_TO_MP_ABS_2, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_MP_CONSUME_REL" ) )           		SetSkillAbilityMap( &luaManager, SA_MP_CONSUME_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_MP_GAIN_ON_HIT_REL" ) )       		SetSkillAbilityMap( &luaManager, SA_MP_GAIN_ON_HIT_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_SCOPE_ABS" ) )                		SetSkillAbilityMap( &luaManager, SA_SCOPE_ABS, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_PROJECTILE_FIRE_COUNT" ) )    		SetSkillAbilityMap( &luaManager, SA_PROJECTILE_FIRE_COUNT, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_PROJECTILE_RANGE_REL" ) )     		SetSkillAbilityMap( &luaManager, SA_PROJECTILE_RANGE_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_PROJECTILE_SIZE" ) )          		SetSkillAbilityMap( &luaManager, SA_PROJECTILE_SIZE, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_PROJECTILE_PIERCING" ) )      		SetSkillAbilityMap( &luaManager, SA_PROJECTILE_PIERCING, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_MOVE_SPEED" ) )               		SetSkillAbilityMap( &luaManager, SA_MOVE_SPEED, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_JUMP_SPEED" ) )                   	SetSkillAbilityMap( &luaManager, SA_JUMP_SPEED, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_ATK_PHYSIC_TO_ATK_MAGIC" ) )      	SetSkillAbilityMap( &luaManager, SA_ATK_PHYSIC_TO_ATK_MAGIC, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_ATK_MAGIC_TO_ATK_PHYSIC" ) )      	SetSkillAbilityMap( &luaManager, SA_ATK_MAGIC_TO_ATK_PHYSIC, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_READY_TIME_REL" ) )               	SetSkillAbilityMap( &luaManager, SA_READY_TIME_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_FORCE_DOWN_REL" ) )               	SetSkillAbilityMap( &luaManager, SA_FORCE_DOWN_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_FORCE_DOWN_MELEE_REL" ) )         	SetSkillAbilityMap( &luaManager, SA_FORCE_DOWN_MELEE_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_FORCE_DOWN_RANGE_REL" ) )         	SetSkillAbilityMap( &luaManager, SA_FORCE_DOWN_RANGE_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_FORCE_DOWN_ABS" ) )               	SetSkillAbilityMap( &luaManager, SA_FORCE_DOWN_ABS, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_MP_REGENERATION_ABS" ) )          	SetSkillAbilityMap( &luaManager, SA_MP_REGENERATION_ABS, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_MP_INTAKE_ABS" ) )                	SetSkillAbilityMap( &luaManager, SA_MP_INTAKE_ABS, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_TARGET_ANI_SPEED" ) )             	SetSkillAbilityMap( &luaManager, SA_TARGET_ANI_SPEED, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_TARGET_MOVE_SPEED" ) )            	SetSkillAbilityMap( &luaManager, SA_TARGET_MOVE_SPEED, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_TARGET_JUMP_SPEED" ) )            	SetSkillAbilityMap( &luaManager, SA_TARGET_JUMP_SPEED, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_TARGET_ATK_REL" ) )               	SetSkillAbilityMap( &luaManager, SA_TARGET_ATK_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_HP_INTAKE_REL_DAMAGE" ) )         	SetSkillAbilityMap( &luaManager, SA_HP_INTAKE_REL_DAMAGE, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_ATK_CRITICAL_DAM_ABS" ) )         	SetSkillAbilityMap( &luaManager, SA_ATK_CRITICAL_DAM_ABS, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_ATK_CRITICAL_RATE_ABS" ) )        	SetSkillAbilityMap( &luaManager, SA_ATK_CRITICAL_RATE_ABS, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_DEF_CRITICAL_DAM_ABS" ) )         	SetSkillAbilityMap( &luaManager, SA_DEF_CRITICAL_DAM_ABS, pSkillTemplet );
	//{{ 2011-04 �� ��ġ�� û ĳ�� ��ų
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
	if( true == luaManager.BeginTable( "SA_DEF_CRITICAL_RATE_ABS" ) )        	SetSkillAbilityMap( &luaManager, SA_DEF_CRITICAL_RATE_ABS, pSkillTemplet );
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
	//}} 2011-04 �� ��ġ�� û ĳ�� ��ų
#ifdef GUILD_SKILL
	if( true == luaManager.BeginTable( "SA_BACK_SPEED_X" ) )                 	SetSkillAbilityMap( &luaManager, SA_BACK_SPEED_X, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_ITEM_SPECIAL_ABILITY_REL" ) )     	SetSkillAbilityMap( &luaManager, SA_ITEM_SPECIAL_ABILITY_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_MP_GAIN_GET_HIT_REL" ) )          	SetSkillAbilityMap( &luaManager, SA_MP_GAIN_GET_HIT_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_SOUL_GAIN_ON_HIT_REL" ) )         	SetSkillAbilityMap( &luaManager, SA_SOUL_GAIN_ON_HIT_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_SOUL_GAIN_GET_HIT_REL" ) )        	SetSkillAbilityMap( &luaManager, SA_SOUL_GAIN_GET_HIT_REL, pSkillTemplet );

	if( true == luaManager.BeginTable( "SA_HP_GAIN_REL_MAX_HP" ) )           	SetSkillAbilityMap( &luaManager, SA_HP_GAIN_REL_MAX_HP, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_ED_GAIN_REL" ) )                  	SetSkillAbilityMap( &luaManager, SA_ED_GAIN_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_ATK_PHYSIC_PLUS_ATK_MAGIC_REL" ) )	SetSkillAbilityMap( &luaManager, SA_ATK_PHYSIC_PLUS_ATK_MAGIC_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_EXP_GAIN_REL" ) )                 	SetSkillAbilityMap( &luaManager, SA_EXP_GAIN_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_DAMAGE_ABSORB_REL_MAX_HP" ) )     	SetSkillAbilityMap( &luaManager, SA_DAMAGE_ABSORB_REL_MAX_HP, pSkillTemplet );
#endif GUILD_SKILL



	if( true == luaManager.BeginTable( "SA_STRONG_MIND_ACTIVE_REL" ) )     	SetSkillAbilityMap( &luaManager, SA_STRONG_MIND_ACTIVE_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_STRONG_MIND_SPECIAL_ACTIVE_REL" ) ) SetSkillAbilityMap( &luaManager, SA_STRONG_MIND_SPECIAL_ACTIVE_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_MP_RETAKE_ABS" ) )     				SetSkillAbilityMap( &luaManager, SA_MP_RETAKE_ABS, pSkillTemplet );


	if( true == luaManager.BeginTable( "SA_RESIST_FIRE_ABS" ) )     			SetSkillAbilityMap( &luaManager, SA_RESIST_FIRE_ABS, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_RESIST_WATER_ABS" ) )     			SetSkillAbilityMap( &luaManager, SA_RESIST_WATER_ABS, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_RESIST_GREEN_ABS" ) )     			SetSkillAbilityMap( &luaManager, SA_RESIST_GREEN_ABS, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_RESIST_WIND_ABS" ) )     			SetSkillAbilityMap( &luaManager, SA_RESIST_WIND_ABS, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_RESIST_LIGHT_ABS" ) )     			SetSkillAbilityMap( &luaManager, SA_RESIST_LIGHT_ABS, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_RESIST_DARK_ABS" ) )     			SetSkillAbilityMap( &luaManager, SA_RESIST_DARK_ABS, pSkillTemplet );

	if( true == luaManager.BeginTable( "SA_ATK_MAGIC_FIRST_REL" ) )     		SetSkillAbilityMap( &luaManager, SA_ATK_MAGIC_FIRST_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_ATK_MAGIC_SECOND_REL" ) )     		SetSkillAbilityMap( &luaManager, SA_ATK_MAGIC_SECOND_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_ATK_MAGIC_THIRD_REL" ) )     		SetSkillAbilityMap( &luaManager, SA_ATK_MAGIC_THIRD_REL, pSkillTemplet );

	if( true == luaManager.BeginTable( "SA_REMOTE_SPEED_ABS" ) )     			SetSkillAbilityMap( &luaManager, SA_REMOTE_SPEED_ABS, pSkillTemplet );

	if( true == luaManager.BeginTable( "SA_DAMAGE_TYPE_CHANGE" ) )     		SetSkillAbilityMap( &luaManager, SA_DAMAGE_TYPE_CHANGE, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_DAMAGE_TIME" ) )     				SetSkillAbilityMap( &luaManager, SA_DAMAGE_TIME, pSkillTemplet );	

	//{{ ����� : 2010.11.12
#ifdef NEW_SKILL_2010_11
	if ( true == luaManager.BeginTable( "SA_RECOVER_MP_ABS" ) )     			SetSkillAbilityMap( &luaManager, SA_RECOVER_MP_ABS, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_TARGET_MANA" ) )     				SetSkillAbilityMap( &luaManager, SA_TARGET_MANA, pSkillTemplet );
#endif NEW_SKILL_2010_11
	//}} ����� : 2010.11.12

	//{{ JHKang / ������ / 2010/11/22
#ifdef NEW_SKILL_2010_11
	if ( true == luaManager.BeginTable( "SA_TARGET_MANA_REL" ) )				SetSkillAbilityMap( &luaManager, SA_TARGET_MANA_REL, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_SIZE_ABS" ) )						SetSkillAbilityMap( &luaManager, SA_SIZE_ABS, pSkillTemplet );
#endif NEW_SKILL_2010_11
	//}} JHKang / ������ / 2010/11/22

#ifdef NEW_SKILL_2010_11 // oasis907 : ����� [2010.11.24] 
	if ( true == luaManager.BeginTable( "SA_REFLECT_REL" ) )					SetSkillAbilityMap( &luaManager, SA_REFLECT_REL, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_EL_DEFENCE_ABS" ) )				SetSkillAbilityMap( &luaManager, SA_EL_DEFENCE_ABS, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_TARGET_HP_REL" ) )					SetSkillAbilityMap( &luaManager, SA_TARGET_HP_REL, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_TARGET_HITRATE_REL" ) )			SetSkillAbilityMap( &luaManager, SA_TARGET_HITRATE_REL, pSkillTemplet );
#endif NEW_SKILL_2010_11

	//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	if ( true == luaManager.BeginTable( "SA_CHARGE_CANNONBALL" ) )				SetSkillAbilityMap( &luaManager, SA_CHARGE_CANNON_BALL, pSkillTemplet );
#endif	NEW_CHARACTER_CHUNG
	//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

	//{{ kimhc // 2011.1.14 // û 1�� ����
#ifdef	CHUNG_FIRST_CLASS_CHANGE
	if ( true == luaManager.BeginTable( "SA_FIRE_DEFENCE_ABS" ) )				SetSkillAbilityMap( &luaManager, SA_FIRE_DEFENCE_ABS, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_WATER_DEFENCE_ABS" ) )				SetSkillAbilityMap( &luaManager, SA_WATER_DEFENCE_ABS, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_NATURE_DEFENCE_ABS" ) )			SetSkillAbilityMap( &luaManager, SA_NATURE_DEFENCE_ABS, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_WIND_DEFENCE_ABS" ) )				SetSkillAbilityMap( &luaManager, SA_WIND_DEFENCE_ABS, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_LIGHT_DEFENCE_ABS" ) )				SetSkillAbilityMap( &luaManager, SA_LIGHT_DEFENCE_ABS, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_DARK_DEFENCE_ABS" ) )				SetSkillAbilityMap( &luaManager, SA_DARK_DEFENCE_ABS, pSkillTemplet );

	if ( true == luaManager.BeginTable( "SA_EDT_FIRE_RESIST_REL" ) )			SetSkillAbilityMap( &luaManager, SA_EDT_FIRE_RESIST_REL, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_EDT_FROZEN_RESIST_REL" ) )			SetSkillAbilityMap( &luaManager, SA_EDT_FROZEN_RESIST_REL, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_EDT_ICE_RESIST_REL" ) )			SetSkillAbilityMap( &luaManager, SA_EDT_ICE_RESIST_REL, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_EDT_POISON_RESIST_REL" ) )			SetSkillAbilityMap( &luaManager, SA_EDT_POISON_RESIST_REL, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_EDT_Y_PRESSED_RESIST_REL" ) )		SetSkillAbilityMap( &luaManager, SA_EDT_Y_PRESSED_RESIST_REL, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_EDT_LEG_WOUND_RESIST_REL" ) )		SetSkillAbilityMap( &luaManager, SA_EDT_LEG_WOUND_RESIST_REL, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_EDT_STUN_RESIST_REL" ) )			SetSkillAbilityMap( &luaManager, SA_EDT_STUN_RESIST_REL, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_EDT_CURSE_RESIST_REL" ) )			SetSkillAbilityMap( &luaManager, SA_EDT_CURSE_RESIST_REL, pSkillTemplet );

	if ( true == luaManager.BeginTable( "SA_EXPAND_CB" ) )						SetSkillAbilityMap( &luaManager, SA_EXPAND_CB, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_ANI_SPEED" ) )						SetSkillAbilityMap( &luaManager, SA_ANI_SPEED, pSkillTemplet );
#endif	CHUNG_FIRST_CLASS_CHANGE
	//}} kimhc // 2011.1.14 // û 1�� ����

	//{{ kimhc // 2011-04-01 // 2011-04 �� ��ġ�� û ĳ�� ��ų
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
	if ( true == luaManager.BeginTable( "SA_ACCURACY_PERCENT_ABS" ) )			SetSkillAbilityMap( &luaManager, SA_ACCURACY_PERCENT_ABS, pSkillTemplet );
#endif	CASH_SKILL_FOR_CHUNG_2011_04
	//}} kimhc // 2011-04-01 // 2011-04 �� ��ġ�� û ĳ�� ��ų

#ifdef CHUNG_SECOND_CLASS_CHANGE
	if ( true == luaManager.BeginTable( "SA_CRITICAL_EVASION_REL" ) )			SetSkillAbilityMap( &luaManager, SA_CRITICAL_EVASION_REL, pSkillTemplet );
#endif CHUNG_SECOND_CLASS_CHANGE

#ifdef CHUNG_SECOND_CLASS_CHANGE
	if ( true == luaManager.BeginTable( "SA_DECREASE_REL" ) )					SetSkillAbilityMap( &luaManager, SA_DECREASE_REL, pSkillTemplet );
#endif CHUNG_SECOND_CLASS_CHANGE

#ifdef ELSWORD_SHEATH_KNIGHT
	if ( true == luaManager.BeginTable( "SA_ATK_ADD_DAMAGE_FIRST_ABS" ) )		SetSkillAbilityMap( &luaManager, SA_ATK_ADD_DAMAGE_FIRST_ABS, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_ATK_ADD_DAMAGE_SECOND_ABS" ) )		SetSkillAbilityMap( &luaManager, SA_ATK_ADD_DAMAGE_SECOND_ABS, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_ATK_ADD_DAMAGE_THIRD_ABS" ) )		SetSkillAbilityMap( &luaManager, SA_ATK_ADD_DAMAGE_THIRD_ABS, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_ATK_ADD_DAMAGE_FOURTH_ABS" ) )		SetSkillAbilityMap( &luaManager, SA_ATK_ADD_DAMAGE_FOURTH_ABS, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_ATK_ADD_DAMAGE_FIFTH_ABS" ) )		SetSkillAbilityMap( &luaManager, SA_ATK_ADD_DAMAGE_FIFTH_ABS, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_TELEPORT_SPEED_REL" ) )			SetSkillAbilityMap( &luaManager, SA_TELEPORT_SPEED_REL, pSkillTemplet );
	if ( true == luaManager.BeginTable( "HP_CONSUME_REL" ) )					SetSkillAbilityMap( &luaManager, HP_CONSUME_REL, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_MP_BURN_ABS" ) )					SetSkillAbilityMap( &luaManager, SA_MP_BURN_ABS, pSkillTemplet );
#endif ELSWORD_SHEATH_KNIGHT

#ifdef	SERV_TRAPPING_RANGER_TEST
	if ( true == luaManager.BeginTable( "SA_SKILLDAMAGE_MULTIPLE_01" ) )		SetSkillAbilityMap( &luaManager, SA_SKILLDAMAGE_MULTIPLE_01, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_SKILLDAMAGE_MULTIPLE_02" ) )		SetSkillAbilityMap( &luaManager, SA_SKILLDAMAGE_MULTIPLE_02, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_SKILLDAMAGE_MULTIPLE_03" ) )		SetSkillAbilityMap( &luaManager, SA_SKILLDAMAGE_MULTIPLE_03, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_SKILLDAMAGE_MULTIPLE_04" ) )		SetSkillAbilityMap( &luaManager, SA_SKILLDAMAGE_MULTIPLE_04, pSkillTemplet );

	if ( true == luaManager.BeginTable( "SA_SKILLDAMAGE_MULTIPLE_01_RATE" ) )	SetSkillAbilityMap( &luaManager, SA_SKILLDAMAGE_MULTIPLE_01_RATE, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_SKILLDAMAGE_MULTIPLE_02_RATE" ) )	SetSkillAbilityMap( &luaManager, SA_SKILLDAMAGE_MULTIPLE_02_RATE, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_SKILLDAMAGE_MULTIPLE_03_RATE" ) )	SetSkillAbilityMap( &luaManager, SA_SKILLDAMAGE_MULTIPLE_03_RATE, pSkillTemplet );
	if ( true == luaManager.BeginTable( "SA_SKILLDAMAGE_MULTIPLE_04_RATE" ) )	SetSkillAbilityMap( &luaManager, SA_SKILLDAMAGE_MULTIPLE_04_RATE, pSkillTemplet );
#endif	SERV_TRAPPING_RANGER_TEST

#ifdef RAVEN_WEAPON_TAKER
	if( true == luaManager.BeginTable( "SA_EVASION_REL" ) )     				SetSkillAbilityMap( &luaManager, SA_EVASION_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_REDUCE_EDT_TIME_REL" ) )     		SetSkillAbilityMap( &luaManager, SA_REDUCE_EDT_TIME_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_OVERHEAT_HP" ) )     				SetSkillAbilityMap( &luaManager, SA_OVERHEAT_HP, pSkillTemplet );
#endif RAVEN_WEAPON_TAKER

#ifdef EVE_ELECTRA
	if( true == luaManager.BeginTable( "SA_SCOPE_ABS_HYPER" ) )     			SetSkillAbilityMap( &luaManager, SA_SCOPE_ABS_HYPER, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_EFFECTIVE_TIME_HYPER" ) )     		SetSkillAbilityMap( &luaManager, SA_EFFECTIVE_TIME_HYPER, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_AIR_MOVE_COUNT" ) )     			SetSkillAbilityMap( &luaManager, SA_AIR_MOVE_COUNT, pSkillTemplet );
#endif EVE_ELECTRA

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	if( true == luaManager.BeginTable( "SA_NOT_CANNONBALL_REL" ) )     		SetSkillAbilityMap( &luaManager, SA_NOT_CANNONBALL_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_INSERT_CANNONBALL_REL" ) )     		SetSkillAbilityMap( &luaManager, SA_INSERT_CANNONBALL_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_BE_SHOT_CANNONBALL_REL" ) )     	SetSkillAbilityMap( &luaManager, SA_BE_SHOT_CANNONBALL_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_CANNONBALL_DAMAGE_UP_REL" ) )     	SetSkillAbilityMap( &luaManager, SA_CANNONBALL_DAMAGE_UP_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_FORCE_DOWN_REL_DAMAGE_RATE_BASE" ) )SetSkillAbilityMap( &luaManager, SA_FORCE_DOWN_REL_DAMAGE_RATE_BASE, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_SIEGE_SHELLING_REL" ) )     		SetSkillAbilityMap( &luaManager, SA_SIEGE_SHELLING_REL, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_EFFECTIVE_DAMAGED_ATTACK_BASE" ) )  SetSkillAbilityMap( &luaManager, SA_EFFECTIVE_DAMAGED_ATTACK_BASE, pSkillTemplet );
#endif

#ifdef SERV_CHUNG_TACTICAL_TROOPER
	if( true == luaManager.BeginTable( "SA_FIRST_HYPER_EFFECTIVE" ) )			SetSkillAbilityMap( &luaManager, SA_FIRST_HYPER_EFFECTIVE, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_SECOND_HYPER_EFFECTIVE" ) )     	SetSkillAbilityMap( &luaManager, SA_SECOND_HYPER_EFFECTIVE, pSkillTemplet );
	if( true == luaManager.BeginTable( "SA_THIRD_HYPER_EFFECTIVE" ) )			SetSkillAbilityMap( &luaManager, SA_THIRD_HYPER_EFFECTIVE, pSkillTemplet );
#endif SERV_CHUNG_TACTICAL_TROOPER

#ifdef BALANCE_CODE_NEMESIS_20121213
	if( true == luaManager.BeginTable( "SA_ATK_CRITICAL_RATE_ATK_BASE" ) )		SetSkillAbilityMap( &luaManager, SA_ATK_CRITICAL_RATE_ATK_BASE, pSkillTemplet );
#endif //BALANCE_CODE_NEMESIS_20121213
	if( true == luaManager.BeginTable( "SA_PROJECTILE_CURVE_SPEED_REL" ) )		SetSkillAbilityMap( &luaManager, SA_PROJECTILE_CURVE_SPEED_REL, pSkillTemplet );	
	if( true == luaManager.BeginTable( "SA_DP_GAIN_REL" ) )					SetSkillAbilityMap( &luaManager, SA_DP_GAIN_REL, pSkillTemplet );	
	if( true == luaManager.BeginTable( "SA_DP_USE_REL" ) )						SetSkillAbilityMap( &luaManager, SA_DP_USE_REL, pSkillTemplet );	
	
	CX2BuffTempletManager::GetInstance()->GetBuffFactorPtrFromBuffFactorList( luaManager, pSkillTemplet->m_vecBuffFactorPtr, pSkillTemplet->m_vecBuffFactorID );

#else // UPGRADE_SKILL_SYSTEM_2013

	SkillTemplet* pSkillTemplet = new SkillTemplet();

	LUA_GET_VALUE( luaManager,		"m_bGuildSkill",			pSkillTemplet->m_bGuildSkill,		false		);

	LUA_GET_VALUE_ENUM( luaManager, "m_eType",					pSkillTemplet->m_eType,				SKILL_TYPE,					ST_NONE				);
	LUA_GET_VALUE_ENUM( luaManager, "m_eID",					pSkillTemplet->m_eID,				SKILL_ID,					SI_NONE				);
	LUA_GET_VALUE( luaManager,		"m_iSkillLevel",			pSkillTemplet->m_iSkillLevel,		1												);
	LUA_GET_VALUE( luaManager,		"m_fSkillCoolTime",			pSkillTemplet->m_fSkillCoolTime,	0.f												);
	LUA_GET_VALUE( luaManager,		"m_fMPConsumption",			pSkillTemplet->m_fMPConsumption,	0.f												);

//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	LUA_GET_VALUE( luaManager,		"CB_CONSUMPTION",			pSkillTemplet->m_usCBConsumption,	0 );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
	LUA_GET_VALUE( luaManager,		"FP_GAIN_COUNT",			pSkillTemplet->m_usFPGainCount,	0 );
	LUA_GET_VALUE( luaManager,		"FP_CONSUME_COUNT",			pSkillTemplet->m_usFPConsumtion,	0 );
#endif

	LUA_GET_VALUE( luaManager,		"m_Name",						pSkillTemplet->m_wstrName,					L""	);
	LUA_GET_VALUE( luaManager,		"m_Desc",						pSkillTemplet->m_wstrMainDesc,				L""	);

	LUA_GET_VALUE( luaManager,		"m_bBornSealed",			pSkillTemplet->m_bBornSealed,		false											);

	LUA_GET_VALUE( luaManager,		"m_wstrTextureName",			pSkillTemplet->m_wstrIconName,				L""	);
	LUA_GET_VALUE( luaManager,		"m_wstrTexturePieceName",		pSkillTemplet->m_wstrIconPieceName,			L""	);

	LUA_GET_VALUE( luaManager,		"m_fPowerRate",				pSkillTemplet->m_fPowerRate,		1.f												);

	LUA_GET_VALUE_ENUM( luaManager, "m_eActiveSkillUseCondtion",	pSkillTemplet->m_eActiveSkillUseCondtion,	ACTIVE_SKILL_USE_CONDITION,		ASUT_NONE	);

	LUA_GET_VALUE( luaManager,		"m_bShowSkillLevel",			pSkillTemplet->m_bShowSkillLevel,		false		);




#ifdef NO_MORE_ERROR_TEST
	if( true == bInHouseTestOnly )
	{
		InHouseLog( L"�系���� ����Ǵ� skilltemplet�� �ֽ��ϴ�." );
		InHouseLog( pSkillTemplet->m_wstrName.c_str() );
	}
#endif NO_MORE_ERROR_TEST






	if( luaManager.BeginTable( "m_Stat" ) == true )
	{
		float fBaseHP;		
		float fAtkPhysic;		
		float fAtkMagic;		
		float fDefPhysic;		
		float fDefMagic;		

		LUA_GET_VALUE( luaManager,		"m_fBaseHP",			fBaseHP,			0.f		);
		LUA_GET_VALUE( luaManager,		"m_fAtkPhysic",			fAtkPhysic,			0.f		);
		LUA_GET_VALUE( luaManager,		"m_fAtkMagic",			fAtkMagic,			0.f		);
		LUA_GET_VALUE( luaManager,		"m_fDefPhysic",			fDefPhysic,			0.f		);
		LUA_GET_VALUE( luaManager,		"m_fDefMagic",			fDefMagic,			0.f		);

		pSkillTemplet->m_Stat.m_fBaseHP 			= fBaseHP;		
		pSkillTemplet->m_Stat.m_fAtkPhysic 			= fAtkPhysic;			
		pSkillTemplet->m_Stat.m_fAtkMagic 			= fAtkMagic;		
		pSkillTemplet->m_Stat.m_fDefPhysic 			= fDefPhysic;			
		pSkillTemplet->m_Stat.m_fDefMagic 			= fDefMagic;		

		luaManager.EndTable(); // m_Stat
	}

	SkillKey skillKey;
	skillKey.m_eID			= pSkillTemplet->m_eID;
	skillKey.m_iSkillLevel	= pSkillTemplet->m_iSkillLevel;
	SkillTempletMap::iterator it = m_mapSkillTemplet.find( skillKey );
	ASSERT( it == m_mapSkillTemplet.end() );
	if( it == m_mapSkillTemplet.end() )
	{
		m_mapSkillTemplet[ skillKey ] = pSkillTemplet;
	}
	else
	{
		// ��ų �ߺ�
		return true;
	}






	float fValue = 0.f;

	if( true == luaManager.GetValue( L"SA_EFFECTIVE_TIME", fValue ) )					pSkillTemplet->m_mapSkillAbility[ SA_EFFECTIVE_TIME ] = fValue;
	if( true == luaManager.GetValue( L"SA_EFFECTIVE_COUNT", fValue ) )          		pSkillTemplet->m_mapSkillAbility[ SA_EFFECTIVE_COUNT ] = fValue;
	if( true == luaManager.GetValue( L"SA_SKILL_ID", fValue ) )                 		pSkillTemplet->m_mapSkillAbility[ SA_SKILL_ID ] = fValue;
	if( true == luaManager.GetValue( L"SA_COMBO_ID", fValue ) )                 		pSkillTemplet->m_mapSkillAbility[ SA_COMBO_ID ] = fValue;
	if( true == luaManager.GetValue( L"SA_EFFECTIVE_RATE", fValue ) )           		pSkillTemplet->m_mapSkillAbility[ SA_EFFECTIVE_RATE ] = fValue;
	if( true == luaManager.GetValue( L"SA_ATK_REL", fValue ) )                  		pSkillTemplet->m_mapSkillAbility[ SA_ATK_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_ATK_PHYSIC_REL", fValue ) )           		pSkillTemplet->m_mapSkillAbility[ SA_ATK_PHYSIC_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_ATK_MAGIC_REL", fValue ) )            		pSkillTemplet->m_mapSkillAbility[ SA_ATK_MAGIC_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_DEF_PHYSIC_REL", fValue ) )           		pSkillTemplet->m_mapSkillAbility[ SA_DEF_PHYSIC_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_DEF_MAGIC_REL", fValue ) )            		pSkillTemplet->m_mapSkillAbility[ SA_DEF_MAGIC_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_TEAM_ATK_PHYSIC_REL", fValue ) )      		pSkillTemplet->m_mapSkillAbility[ SA_TEAM_ATK_PHYSIC_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_TEAM_ATK_MAGIC_REL", fValue ) )       		pSkillTemplet->m_mapSkillAbility[ SA_TEAM_ATK_MAGIC_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_TEAM_DEF_PHYSIC_REL", fValue ) )      		pSkillTemplet->m_mapSkillAbility[ SA_TEAM_DEF_PHYSIC_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_TEAM_DEF_MAGIC_REL", fValue ) )       		pSkillTemplet->m_mapSkillAbility[ SA_TEAM_DEF_MAGIC_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_DAMAGE_REL", fValue ) )               		pSkillTemplet->m_mapSkillAbility[ SA_DAMAGE_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_DAMAGED_REL", fValue ) )              		pSkillTemplet->m_mapSkillAbility[ SA_DAMAGED_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_UNFIXED_DEFENSE_REL", fValue ) )      		pSkillTemplet->m_mapSkillAbility[ SA_UNFIXED_DEFENSE_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_SUMMON_ATK_REL", fValue ) )           		pSkillTemplet->m_mapSkillAbility[ SA_SUMMON_ATK_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_MAX_HP_ABS", fValue ) )               		pSkillTemplet->m_mapSkillAbility[ SA_MAX_HP_ABS ] = fValue;
	if( true == luaManager.GetValue( L"SA_MAX_HP_REL", fValue ) )               		pSkillTemplet->m_mapSkillAbility[ SA_MAX_HP_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_MAX_MP_ABS", fValue ) )               		pSkillTemplet->m_mapSkillAbility[ SA_MAX_MP_ABS ] = fValue;
	if( true == luaManager.GetValue( L"SA_MAX_MP_REL", fValue ) )               		pSkillTemplet->m_mapSkillAbility[ SA_MAX_MP_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_TELEPORT_RANGE_ABS", fValue ) )       		pSkillTemplet->m_mapSkillAbility[ SA_TELEPORT_RANGE_ABS ] = fValue;
	if( true == luaManager.GetValue( L"SA_CHARGING_TIME", fValue ) )            		pSkillTemplet->m_mapSkillAbility[ SA_CHARGING_TIME ] = fValue;
	if( true == luaManager.GetValue( L"SA_HP_REL_TO_MP_ABS_1", fValue ) )       		pSkillTemplet->m_mapSkillAbility[ SA_HP_REL_TO_MP_ABS_1 ] = fValue;
	if( true == luaManager.GetValue( L"SA_HP_REL_TO_MP_ABS_2", fValue ) )       		pSkillTemplet->m_mapSkillAbility[ SA_HP_REL_TO_MP_ABS_2 ] = fValue;
	if( true == luaManager.GetValue( L"SA_MP_CONSUME_REL", fValue ) )           		pSkillTemplet->m_mapSkillAbility[ SA_MP_CONSUME_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_MP_GAIN_ON_HIT_REL", fValue ) )       		pSkillTemplet->m_mapSkillAbility[ SA_MP_GAIN_ON_HIT_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_SCOPE_ABS", fValue ) )                		pSkillTemplet->m_mapSkillAbility[ SA_SCOPE_ABS ] = fValue;
	if( true == luaManager.GetValue( L"SA_PROJECTILE_FIRE_COUNT", fValue ) )    		pSkillTemplet->m_mapSkillAbility[ SA_PROJECTILE_FIRE_COUNT ] = fValue;
	if( true == luaManager.GetValue( L"SA_PROJECTILE_RANGE_REL", fValue ) )     		pSkillTemplet->m_mapSkillAbility[ SA_PROJECTILE_RANGE_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_PROJECTILE_SIZE", fValue ) )          		pSkillTemplet->m_mapSkillAbility[ SA_PROJECTILE_SIZE ] = fValue;
	if( true == luaManager.GetValue( L"SA_PROJECTILE_PIERCING", fValue ) )      		pSkillTemplet->m_mapSkillAbility[ SA_PROJECTILE_PIERCING ] = fValue;
	if( true == luaManager.GetValue( L"SA_MOVE_SPEED", fValue ) )               		pSkillTemplet->m_mapSkillAbility[ SA_MOVE_SPEED ] = fValue;
	if( true == luaManager.GetValue( L"SA_JUMP_SPEED", fValue ) )                   	pSkillTemplet->m_mapSkillAbility[ SA_JUMP_SPEED ] = fValue;
	if( true == luaManager.GetValue( L"SA_ATK_PHYSIC_TO_ATK_MAGIC", fValue ) )      	pSkillTemplet->m_mapSkillAbility[ SA_ATK_PHYSIC_TO_ATK_MAGIC ] = fValue;
	if( true == luaManager.GetValue( L"SA_ATK_MAGIC_TO_ATK_PHYSIC", fValue ) )      	pSkillTemplet->m_mapSkillAbility[ SA_ATK_MAGIC_TO_ATK_PHYSIC ] = fValue;
	if( true == luaManager.GetValue( L"SA_READY_TIME_REL", fValue ) )               	pSkillTemplet->m_mapSkillAbility[ SA_READY_TIME_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_FORCE_DOWN_REL", fValue ) )               	pSkillTemplet->m_mapSkillAbility[ SA_FORCE_DOWN_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_FORCE_DOWN_MELEE_REL", fValue ) )         	pSkillTemplet->m_mapSkillAbility[ SA_FORCE_DOWN_MELEE_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_FORCE_DOWN_RANGE_REL", fValue ) )         	pSkillTemplet->m_mapSkillAbility[ SA_FORCE_DOWN_RANGE_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_FORCE_DOWN_ABS", fValue ) )               	pSkillTemplet->m_mapSkillAbility[ SA_FORCE_DOWN_ABS ] = fValue;
	if( true == luaManager.GetValue( L"SA_MP_REGENERATION_ABS", fValue ) )          	pSkillTemplet->m_mapSkillAbility[ SA_MP_REGENERATION_ABS ] = fValue;
	if( true == luaManager.GetValue( L"SA_MP_INTAKE_ABS", fValue ) )                	pSkillTemplet->m_mapSkillAbility[ SA_MP_INTAKE_ABS ] = fValue;
	if( true == luaManager.GetValue( L"SA_TARGET_ANI_SPEED", fValue ) )             	pSkillTemplet->m_mapSkillAbility[ SA_TARGET_ANI_SPEED ] = fValue;
	if( true == luaManager.GetValue( L"SA_TARGET_MOVE_SPEED", fValue ) )            	pSkillTemplet->m_mapSkillAbility[ SA_TARGET_MOVE_SPEED ] = fValue;
	if( true == luaManager.GetValue( L"SA_TARGET_JUMP_SPEED", fValue ) )            	pSkillTemplet->m_mapSkillAbility[ SA_TARGET_JUMP_SPEED ] = fValue;
	if( true == luaManager.GetValue( L"SA_TARGET_ATK_REL", fValue ) )               	pSkillTemplet->m_mapSkillAbility[ SA_TARGET_ATK_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_HP_INTAKE_REL_DAMAGE", fValue ) )         	pSkillTemplet->m_mapSkillAbility[ SA_HP_INTAKE_REL_DAMAGE ] = fValue;
	if( true == luaManager.GetValue( L"SA_ATK_CRITICAL_DAM_ABS", fValue ) )         	pSkillTemplet->m_mapSkillAbility[ SA_ATK_CRITICAL_DAM_ABS ] = fValue;
	if( true == luaManager.GetValue( L"SA_ATK_CRITICAL_RATE_ABS", fValue ) )        	pSkillTemplet->m_mapSkillAbility[ SA_ATK_CRITICAL_RATE_ABS ] = fValue;
	if( true == luaManager.GetValue( L"SA_DEF_CRITICAL_DAM_ABS", fValue ) )         	pSkillTemplet->m_mapSkillAbility[ SA_DEF_CRITICAL_DAM_ABS ] = fValue;
	//{{ 2011-04 �� ��ġ�� û ĳ�� ��ų
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
	if( true == luaManager.GetValue( L"SA_DEF_CRITICAL_RATE_ABS", fValue ) )        	pSkillTemplet->m_mapSkillAbility[ SA_DEF_CRITICAL_RATE_ABS ] = fValue;
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
	//}} 2011-04 �� ��ġ�� û ĳ�� ��ų
#ifdef GUILD_SKILL
	if( true == luaManager.GetValue( L"SA_BACK_SPEED_X", fValue ) )                 	pSkillTemplet->m_mapSkillAbility[ SA_BACK_SPEED_X ] = fValue;
	if( true == luaManager.GetValue( L"SA_ITEM_SPECIAL_ABILITY_REL", fValue ) )     	pSkillTemplet->m_mapSkillAbility[ SA_ITEM_SPECIAL_ABILITY_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_MP_GAIN_GET_HIT_REL", fValue ) )          	pSkillTemplet->m_mapSkillAbility[ SA_MP_GAIN_GET_HIT_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_SOUL_GAIN_ON_HIT_REL", fValue ) )         	pSkillTemplet->m_mapSkillAbility[ SA_SOUL_GAIN_ON_HIT_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_SOUL_GAIN_GET_HIT_REL", fValue ) )        	pSkillTemplet->m_mapSkillAbility[ SA_SOUL_GAIN_GET_HIT_REL ] = fValue;

	if( true == luaManager.GetValue( L"SA_HP_GAIN_REL_MAX_HP", fValue ) )           	pSkillTemplet->m_mapSkillAbility[ SA_HP_GAIN_REL_MAX_HP ] = fValue;
	if( true == luaManager.GetValue( L"SA_ED_GAIN_REL", fValue ) )                  	pSkillTemplet->m_mapSkillAbility[ SA_ED_GAIN_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_ATK_PHYSIC_PLUS_ATK_MAGIC_REL", fValue ) )	pSkillTemplet->m_mapSkillAbility[ SA_ATK_PHYSIC_PLUS_ATK_MAGIC_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_EXP_GAIN_REL", fValue ) )                 	pSkillTemplet->m_mapSkillAbility[ SA_EXP_GAIN_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_DAMAGE_ABSORB_REL_MAX_HP", fValue ) )     	pSkillTemplet->m_mapSkillAbility[ SA_DAMAGE_ABSORB_REL_MAX_HP ] = fValue;
#endif GUILD_SKILL



	if( true == luaManager.GetValue( L"SA_STRONG_MIND_ACTIVE_REL", fValue ) )     		pSkillTemplet->m_mapSkillAbility[ SA_STRONG_MIND_ACTIVE_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_STRONG_MIND_SPECIAL_ACTIVE_REL", fValue ) )   pSkillTemplet->m_mapSkillAbility[ SA_STRONG_MIND_SPECIAL_ACTIVE_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_MP_RETAKE_ABS", fValue ) )     				pSkillTemplet->m_mapSkillAbility[ SA_MP_RETAKE_ABS ] = fValue;


	if( true == luaManager.GetValue( L"SA_RESIST_FIRE_ABS", fValue ) )     				pSkillTemplet->m_mapSkillAbility[ SA_RESIST_FIRE_ABS ] = fValue;
	if( true == luaManager.GetValue( L"SA_RESIST_WATER_ABS", fValue ) )     			pSkillTemplet->m_mapSkillAbility[ SA_RESIST_WATER_ABS ] = fValue;
	if( true == luaManager.GetValue( L"SA_RESIST_GREEN_ABS", fValue ) )     			pSkillTemplet->m_mapSkillAbility[ SA_RESIST_GREEN_ABS ] = fValue;
	if( true == luaManager.GetValue( L"SA_RESIST_WIND_ABS", fValue ) )     				pSkillTemplet->m_mapSkillAbility[ SA_RESIST_WIND_ABS ] = fValue;
	if( true == luaManager.GetValue( L"SA_RESIST_LIGHT_ABS", fValue ) )     			pSkillTemplet->m_mapSkillAbility[ SA_RESIST_LIGHT_ABS ] = fValue;
	if( true == luaManager.GetValue( L"SA_RESIST_DARK_ABS", fValue ) )     				pSkillTemplet->m_mapSkillAbility[ SA_RESIST_DARK_ABS ] = fValue;

	if( true == luaManager.GetValue( L"SA_ATK_MAGIC_FIRST_REL", fValue ) )     			pSkillTemplet->m_mapSkillAbility[ SA_ATK_MAGIC_FIRST_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_ATK_MAGIC_SECOND_REL", fValue ) )     		pSkillTemplet->m_mapSkillAbility[ SA_ATK_MAGIC_SECOND_REL ] = fValue;
	if( true == luaManager.GetValue( L"SA_ATK_MAGIC_THIRD_REL", fValue ) )     			pSkillTemplet->m_mapSkillAbility[ SA_ATK_MAGIC_THIRD_REL ] = fValue;
	
	if( true == luaManager.GetValue( L"SA_REMOTE_SPEED_ABS", fValue ) )     			pSkillTemplet->m_mapSkillAbility[ SA_REMOTE_SPEED_ABS ] = fValue;

	if( true == luaManager.GetValue( L"SA_DAMAGE_TYPE_CHANGE", fValue ) )     			pSkillTemplet->m_mapSkillAbility[ SA_DAMAGE_TYPE_CHANGE ] = fValue;	
	if( true == luaManager.GetValue( L"SA_DAMAGE_TIME", fValue ) )     					pSkillTemplet->m_mapSkillAbility[ SA_DAMAGE_TIME ] = fValue;	

//{{ ����� : 2010.11.12
#ifdef NEW_SKILL_2010_11
	if ( true == luaManager.GetValue( L"SA_RECOVER_MP_ABS", fValue ) )     			pSkillTemplet->m_mapSkillAbility[ SA_RECOVER_MP_ABS ] = fValue;	
	if ( true == luaManager.GetValue( L"SA_TARGET_MANA", fValue ) )     			pSkillTemplet->m_mapSkillAbility[ SA_TARGET_MANA ] = fValue;	
#endif NEW_SKILL_2010_11
//}} ����� : 2010.11.12

	//{{ JHKang / ������ / 2010/11/22
#ifdef NEW_SKILL_2010_11
	if ( true == luaManager.GetValue( L"SA_TARGET_MANA_REL", fValue ) )	pSkillTemplet->m_mapSkillAbility[ SA_TARGET_MANA_REL ] = fValue;
	if ( true == luaManager.GetValue( L"SA_SIZE_ABS", fValue ) )		pSkillTemplet->m_mapSkillAbility[ SA_SIZE_ABS ] = fValue;
#endif NEW_SKILL_2010_11
	//}} JHKang / ������ / 2010/11/22

#ifdef NEW_SKILL_2010_11 // oasis907 : ����� [2010.11.24] 
	if ( true == luaManager.GetValue( L"SA_REFLECT_REL", fValue ) )						pSkillTemplet->m_mapSkillAbility[ SA_REFLECT_REL ] = fValue;
	if ( true == luaManager.GetValue( L"SA_EL_DEFENCE_ABS", fValue ) )					pSkillTemplet->m_mapSkillAbility[ SA_EL_DEFENCE_ABS ] = fValue;
	if ( true == luaManager.GetValue( L"SA_TARGET_HP_REL", fValue ) )					pSkillTemplet->m_mapSkillAbility[ SA_TARGET_HP_REL ] = fValue;
	if ( true == luaManager.GetValue( L"SA_TARGET_HITRATE_REL", fValue ) )				pSkillTemplet->m_mapSkillAbility[ SA_TARGET_HITRATE_REL ] = fValue;
#endif NEW_SKILL_2010_11

//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	if ( true == luaManager.GetValue( L"SA_CHARGE_CANNONBALL", fValue ) )				pSkillTemplet->m_mapSkillAbility[ SA_CHARGE_CANNON_BALL ] = fValue;
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

//{{ kimhc // 2011.1.14 // û 1�� ����
#ifdef	CHUNG_FIRST_CLASS_CHANGE
	if ( true == luaManager.GetValue( L"SA_FIRE_DEFENCE_ABS", fValue ) )				pSkillTemplet->m_mapSkillAbility[ SA_FIRE_DEFENCE_ABS ]			= fValue;
	if ( true == luaManager.GetValue( L"SA_WATER_DEFENCE_ABS", fValue ) )				pSkillTemplet->m_mapSkillAbility[ SA_WATER_DEFENCE_ABS ]		= fValue;
	if ( true == luaManager.GetValue( L"SA_NATURE_DEFENCE_ABS", fValue ) )				pSkillTemplet->m_mapSkillAbility[ SA_NATURE_DEFENCE_ABS ]		= fValue;
	if ( true == luaManager.GetValue( L"SA_WIND_DEFENCE_ABS", fValue ) )				pSkillTemplet->m_mapSkillAbility[ SA_WIND_DEFENCE_ABS ]			= fValue;
	if ( true == luaManager.GetValue( L"SA_LIGHT_DEFENCE_ABS", fValue ) )				pSkillTemplet->m_mapSkillAbility[ SA_LIGHT_DEFENCE_ABS ]		= fValue;
	if ( true == luaManager.GetValue( L"SA_DARK_DEFENCE_ABS", fValue ) )				pSkillTemplet->m_mapSkillAbility[ SA_DARK_DEFENCE_ABS ]			= fValue;

	if ( true == luaManager.GetValue( L"SA_EDT_FIRE_RESIST_REL", fValue ) )				pSkillTemplet->m_mapSkillAbility[ SA_EDT_FIRE_RESIST_REL ]		= fValue;
	if ( true == luaManager.GetValue( L"SA_EDT_FROZEN_RESIST_REL", fValue ) )			pSkillTemplet->m_mapSkillAbility[ SA_EDT_FROZEN_RESIST_REL ]	= fValue;
	if ( true == luaManager.GetValue( L"SA_EDT_ICE_RESIST_REL", fValue ) )				pSkillTemplet->m_mapSkillAbility[ SA_EDT_ICE_RESIST_REL ]		= fValue;
	if ( true == luaManager.GetValue( L"SA_EDT_POISON_RESIST_REL", fValue ) )			pSkillTemplet->m_mapSkillAbility[ SA_EDT_POISON_RESIST_REL ]	= fValue;
	if ( true == luaManager.GetValue( L"SA_EDT_Y_PRESSED_RESIST_REL", fValue ) )		pSkillTemplet->m_mapSkillAbility[ SA_EDT_Y_PRESSED_RESIST_REL ]	= fValue;
	if ( true == luaManager.GetValue( L"SA_EDT_LEG_WOUND_RESIST_REL", fValue ) )		pSkillTemplet->m_mapSkillAbility[ SA_EDT_LEG_WOUND_RESIST_REL ]	= fValue;
	if ( true == luaManager.GetValue( L"SA_EDT_STUN_RESIST_REL", fValue ) )				pSkillTemplet->m_mapSkillAbility[ SA_EDT_STUN_RESIST_REL ]		= fValue;
	if ( true == luaManager.GetValue( L"SA_EDT_CURSE_RESIST_REL", fValue ) )			pSkillTemplet->m_mapSkillAbility[ SA_EDT_CURSE_RESIST_REL ]		= fValue;

	if ( true == luaManager.GetValue( L"SA_EXPAND_CB", fValue ) )						pSkillTemplet->m_mapSkillAbility[ SA_EXPAND_CB ]				= fValue;
	if ( true == luaManager.GetValue( L"SA_ANI_SPEED", fValue ) )						pSkillTemplet->m_mapSkillAbility[ SA_ANI_SPEED ]				= fValue;
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.14 // û 1�� ����

	//{{ kimhc // 2011-04-01 // 2011-04 �� ��ġ�� û ĳ�� ��ų
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
	if ( true == luaManager.GetValue( L"SA_ACCURACY_PERCENT_ABS", fValue ) )			pSkillTemplet->m_mapSkillAbility[ SA_ACCURACY_PERCENT_ABS ]		= fValue;
#endif	CASH_SKILL_FOR_CHUNG_2011_04
	//}} kimhc // 2011-04-01 // 2011-04 �� ��ġ�� û ĳ�� ��ų

#ifdef CHUNG_SECOND_CLASS_CHANGE
	if ( true == luaManager.GetValue( L"SA_CRITICAL_EVASION_REL", fValue ) )			pSkillTemplet->m_mapSkillAbility[ SA_CRITICAL_EVASION_REL ]		= fValue;
#endif CHUNG_SECOND_CLASS_CHANGE

#ifdef CHUNG_SECOND_CLASS_CHANGE
	if ( true == luaManager.GetValue( L"SA_DECREASE_REL", fValue ) )					pSkillTemplet->m_mapSkillAbility[ SA_DECREASE_REL ]		= fValue;
#endif CHUNG_SECOND_CLASS_CHANGE

#ifdef ELSWORD_SHEATH_KNIGHT
	if ( true == luaManager.GetValue( L"SA_ATK_ADD_DAMAGE_FIRST_ABS", fValue ) )		pSkillTemplet->m_mapSkillAbility[ SA_ATK_ADD_DAMAGE_FIRST_ABS ]		= fValue;
	if ( true == luaManager.GetValue( L"SA_ATK_ADD_DAMAGE_SECOND_ABS", fValue ) )		pSkillTemplet->m_mapSkillAbility[ SA_ATK_ADD_DAMAGE_SECOND_ABS ]	= fValue;
	if ( true == luaManager.GetValue( L"SA_ATK_ADD_DAMAGE_THIRD_ABS", fValue ) )		pSkillTemplet->m_mapSkillAbility[ SA_ATK_ADD_DAMAGE_THIRD_ABS ]		= fValue;
	if ( true == luaManager.GetValue( L"SA_ATK_ADD_DAMAGE_FOURTH_ABS", fValue ) )		pSkillTemplet->m_mapSkillAbility[ SA_ATK_ADD_DAMAGE_FOURTH_ABS ]	= fValue;
	if ( true == luaManager.GetValue( L"SA_ATK_ADD_DAMAGE_FIFTH_ABS", fValue ) )		pSkillTemplet->m_mapSkillAbility[ SA_ATK_ADD_DAMAGE_FIFTH_ABS ]		= fValue;
	if ( true == luaManager.GetValue( L"SA_TELEPORT_SPEED_REL", fValue ) )				pSkillTemplet->m_mapSkillAbility[ SA_TELEPORT_SPEED_REL ]	= fValue;
	if ( true == luaManager.GetValue( L"HP_CONSUME_REL", fValue ) )						pSkillTemplet->m_mapSkillAbility[ HP_CONSUME_REL ]		= fValue;
	if ( true == luaManager.GetValue( L"SA_MP_BURN_ABS", fValue ) )						pSkillTemplet->m_mapSkillAbility[ SA_MP_BURN_ABS ]		= fValue;
#endif ELSWORD_SHEATH_KNIGHT

#ifdef	SERV_TRAPPING_RANGER_TEST
	if ( true == luaManager.GetValue( L"SA_SKILLDAMAGE_MULTIPLE_01", fValue ) )						pSkillTemplet->m_mapSkillAbility[ SA_SKILLDAMAGE_MULTIPLE_01 ]		= fValue;
	if ( true == luaManager.GetValue( L"SA_SKILLDAMAGE_MULTIPLE_02", fValue ) )						pSkillTemplet->m_mapSkillAbility[ SA_SKILLDAMAGE_MULTIPLE_02 ]		= fValue;
	if ( true == luaManager.GetValue( L"SA_SKILLDAMAGE_MULTIPLE_03", fValue ) )						pSkillTemplet->m_mapSkillAbility[ SA_SKILLDAMAGE_MULTIPLE_03 ]		= fValue;
	if ( true == luaManager.GetValue( L"SA_SKILLDAMAGE_MULTIPLE_04", fValue ) )						pSkillTemplet->m_mapSkillAbility[ SA_SKILLDAMAGE_MULTIPLE_04 ]		= fValue;

	if ( true == luaManager.GetValue( L"SA_SKILLDAMAGE_MULTIPLE_01_RATE", fValue ) )				pSkillTemplet->m_mapSkillAbility[ SA_SKILLDAMAGE_MULTIPLE_01_RATE ]		= fValue;
	if ( true == luaManager.GetValue( L"SA_SKILLDAMAGE_MULTIPLE_02_RATE", fValue ) )				pSkillTemplet->m_mapSkillAbility[ SA_SKILLDAMAGE_MULTIPLE_02_RATE ]		= fValue;
	if ( true == luaManager.GetValue( L"SA_SKILLDAMAGE_MULTIPLE_03_RATE", fValue ) )				pSkillTemplet->m_mapSkillAbility[ SA_SKILLDAMAGE_MULTIPLE_03_RATE ]		= fValue;
	if ( true == luaManager.GetValue( L"SA_SKILLDAMAGE_MULTIPLE_04_RATE", fValue ) )				pSkillTemplet->m_mapSkillAbility[ SA_SKILLDAMAGE_MULTIPLE_04_RATE ]		= fValue;
#endif	SERV_TRAPPING_RANGER_TEST

#ifdef RAVEN_WEAPON_TAKER
	if( true == luaManager.GetValue( L"SA_EVASION_REL", fValue ) )     					pSkillTemplet->m_mapSkillAbility[ SA_EVASION_REL ]				= fValue;
	if( true == luaManager.GetValue( L"SA_REDUCE_EDT_TIME_REL", fValue ) )     			pSkillTemplet->m_mapSkillAbility[ SA_REDUCE_EDT_TIME_REL ]		= fValue;
	if( true == luaManager.GetValue( L"SA_OVERHEAT_HP", fValue ) )     					pSkillTemplet->m_mapSkillAbility[ SA_OVERHEAT_HP ]				= fValue;
#endif RAVEN_WEAPON_TAKER

#ifdef EVE_ELECTRA
	if( true == luaManager.GetValue( L"SA_SCOPE_ABS_HYPER", fValue ) )     				pSkillTemplet->m_mapSkillAbility[ SA_SCOPE_ABS_HYPER ]			= fValue;
	if( true == luaManager.GetValue( L"SA_EFFECTIVE_TIME_HYPER", fValue ) )     		pSkillTemplet->m_mapSkillAbility[ SA_EFFECTIVE_TIME_HYPER ]		= fValue;
	if( true == luaManager.GetValue( L"SA_AIR_MOVE_COUNT", fValue ) )     				pSkillTemplet->m_mapSkillAbility[ SA_AIR_MOVE_COUNT ]			= fValue;
#endif EVE_ELECTRA

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	if( true == luaManager.GetValue( L"SA_NOT_CANNONBALL_REL", fValue ) )     			pSkillTemplet->m_mapSkillAbility[ SA_NOT_CANNONBALL_REL ]			= fValue;
	if( true == luaManager.GetValue( L"SA_INSERT_CANNONBALL_REL", fValue ) )     		pSkillTemplet->m_mapSkillAbility[ SA_INSERT_CANNONBALL_REL ]			= fValue;
	if( true == luaManager.GetValue( L"SA_BE_SHOT_CANNONBALL_REL", fValue ) )     		pSkillTemplet->m_mapSkillAbility[ SA_BE_SHOT_CANNONBALL_REL ]			= fValue;
	if( true == luaManager.GetValue( L"SA_CANNONBALL_DAMAGE_UP_REL", fValue ) )     	pSkillTemplet->m_mapSkillAbility[ SA_CANNONBALL_DAMAGE_UP_REL ]			= fValue;
	if( true == luaManager.GetValue( L"SA_FORCE_DOWN_REL_DAMAGE_RATE_BASE", fValue ) )  pSkillTemplet->m_mapSkillAbility[ SA_FORCE_DOWN_REL_DAMAGE_RATE_BASE ]			= fValue;
	if( true == luaManager.GetValue( L"SA_SIEGE_SHELLING_REL", fValue ) )     			pSkillTemplet->m_mapSkillAbility[ SA_SIEGE_SHELLING_REL ]			= fValue;
	if( true == luaManager.GetValue( L"SA_EFFECTIVE_DAMAGED_ATTACK_BASE", fValue ) )    pSkillTemplet->m_mapSkillAbility[ SA_EFFECTIVE_DAMAGED_ATTACK_BASE ]			= fValue;
#endif

#ifdef SERV_CHUNG_TACTICAL_TROOPER
	if( true == luaManager.GetValue( L"SA_FIRST_HYPER_EFFECTIVE", fValue ) )			pSkillTemplet->m_mapSkillAbility[ SA_FIRST_HYPER_EFFECTIVE ]			= fValue;
	if( true == luaManager.GetValue( L"SA_SECOND_HYPER_EFFECTIVE", fValue ) )     		pSkillTemplet->m_mapSkillAbility[ SA_SECOND_HYPER_EFFECTIVE ]			= fValue;
	if( true == luaManager.GetValue( L"SA_THIRD_HYPER_EFFECTIVE", fValue ) )			pSkillTemplet->m_mapSkillAbility[ SA_THIRD_HYPER_EFFECTIVE ]			= fValue;
#endif SERV_CHUNG_TACTICAL_TROOPER

#ifdef BALANCE_CODE_NEMESIS_20121213
	if( true == luaManager.GetValue( L"SA_ATK_CRITICAL_RATE_ATK_BASE", fValue ) )			pSkillTemplet->m_mapSkillAbility[ SA_ATK_CRITICAL_RATE_ATK_BASE ]	= fValue;
#endif //BALANCE_CODE_NEMESIS_20121213
	if( true == luaManager.GetValue( L"SA_PROJECTILE_CURVE_SPEED_REL", fValue ) )			pSkillTemplet->m_mapSkillAbility[ SA_PROJECTILE_CURVE_SPEED_REL ]	= fValue;	
	if( true == luaManager.GetValue( L"SA_DP_GAIN_REL", fValue ) )							pSkillTemplet->m_mapSkillAbility[ SA_DP_GAIN_REL ]	= fValue;	
	if( true == luaManager.GetValue( L"SA_DP_USE_REL", fValue ) )							pSkillTemplet->m_mapSkillAbility[ SA_DP_USE_REL ]	= fValue;	

	CX2BuffTempletManager::GetInstance()->GetBuffFactorPtrFromBuffFactorList( luaManager, pSkillTemplet->m_vecBuffFactorPtr, pSkillTemplet->m_vecBuffFactorID );

#endif // UPGRADE_SKILL_SYSTEM_2013

	return true;
}

#ifdef LUA_TRANS_DEVIDE
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
bool CX2SkillTree::AddSkillTempletTrans_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
	
	SkillTemplet* pSkillTemplet = new SkillTemplet();
	
	LUA_GET_VALUE_ENUM( luaManager,		"m_eID",						pSkillTemplet->m_eID,							SKILL_ID,			SI_NONE					);
	LUA_GET_VALUE_ENUM( luaManager,		"m_eType",						pSkillTemplet->m_eType,							SKILL_TYPE,			ST_NONE					);
	LUA_GET_VALUE( luaManager,			"m_wstrName",					pSkillTemplet->m_wstrName,						L""		);
	LUA_GET_VALUE( luaManager,			"m_wstrMainDesc",				pSkillTemplet->m_wstrMainDesc,					L""		);
	if( true == luaManager.BeginTable( L"m_EffectiveDesc" ) )			SetSkillVlaue( luaManager,						pSkillTemplet->m_vecEffectiveDesc		);
	
	// ���⼭ ġȯ ���־�� ��.
	SkillTempletMap::iterator mit;
	mit = m_mapSkillTemplet.find( pSkillTemplet->m_eID );
	if( mit == m_mapSkillTemplet.end() )
	{
		wstringstream wstrStream;
		wstrStream << L"NewSkillTempletVer2Trans.lua And NewSkillTempletVer2.lua Not Equal m_eID..." << (pSkillTemplet->m_eID) << L"_";
		ErrorLogMsg( XEM_ERROR105, wstrStream.str().c_str() );	
		// �����α� ���
		SAFE_DELETE(pSkillTemplet); 
		return false;	
	}
	else
	{
		mit->second->m_eType = pSkillTemplet->m_eType;
		mit->second->m_wstrName = pSkillTemplet->m_wstrName;		
		mit->second->m_wstrMainDesc = pSkillTemplet->m_wstrMainDesc;		
		mit->second->m_vecEffectiveDesc = pSkillTemplet->m_vecEffectiveDesc;		
	}

	return true;
}
#else //UPGRADE_SKILL_SYSTEM_2013
bool CX2SkillTree::AddSkillTempletTrans_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );

	SkillTemplet* pSkillTemplet = new SkillTemplet();

	LUA_GET_VALUE( luaManager,		"m_bGuildSkill",			pSkillTemplet->m_bGuildSkill,		false		);
	LUA_GET_VALUE_ENUM( luaManager, "m_eID",					pSkillTemplet->m_eID,				SKILL_ID,					SI_NONE				);
	LUA_GET_VALUE_ENUM( luaManager, "m_eType",					pSkillTemplet->m_eType,				SKILL_TYPE,					ST_NONE				);
	LUA_GET_VALUE( luaManager,		"m_Name",					pSkillTemplet->m_Name,				L""												);
	LUA_GET_VALUE( luaManager,		"m_Desc",					pSkillTemplet->m_Desc,				L""												);
	LUA_GET_VALUE( luaManager,		"m_iSkillLevel",			pSkillTemplet->m_iSkillLevel,		1												);	

	// ���⼭ ġȯ ���־�� ��.
	SkillKey skillKey;
	skillKey.m_eID			= pSkillTemplet->m_eID;
	skillKey.m_iSkillLevel	= pSkillTemplet->m_iSkillLevel;
	SkillTempletMap::iterator mit;
	mit = m_mapSkillTemplet.find( skillKey );
	if( mit == m_mapSkillTemplet.end() )
	{
		wstringstream wstrStream;
		wstrStream << L"NewSkillTempletTrans.lua And NewSkillTemplet.lua Not Equal m_eID&&m_iSkillLevel..." << (pSkillTemplet->m_Desc) << L"_";
		ErrorLogMsg( XEM_ERROR105, wstrStream.str().c_str() );	
		// �����α� ���
		SAFE_DELETE(pSkillTemplet); 
		return false;
	}
	else
	{
		mit->second->m_bGuildSkill = pSkillTemplet->m_bGuildSkill;
		mit->second->m_eType = pSkillTemplet->m_eType;
		mit->second->m_Name = pSkillTemplet->m_Name;
		mit->second->m_Desc = pSkillTemplet->m_Desc;
	}

	return true;
}
#endif //UPGRADE_SKILL_SYSTEM_2013
#endif LUA_TRANS_DEVIDE

//{{ 2009. 8. 5  ������		���� ��ų ������ ����
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����

bool CX2SkillTree::AddSealSkillInfo_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	int							iUnsealItemID = 0;					/// ������ ���̵�
	CX2Unit::UNIT_CLASS_TYPE	eUnitClassType = CX2Unit::UCT_NONE;	/// ��� ������ ���� Ŭ����
	vector<SKILL_ID>			vecSkillID;							/// ��� ������ ���� ��ų ���̵� ���� �����̳�

	/// ������ ���̵� �Ľ�
	LUA_GET_VALUE( luaManager,		"m_iItemID",		iUnsealItemID,		0 );

	/// ��� ������ ���� Ŭ���� �Ľ�
	LUA_GET_VALUE_ENUM( luaManager, "m_eUnitClassType",	eUnitClassType,		CX2Unit::UNIT_CLASS_TYPE,		CX2Unit::UCT_NONE		);
	
	/// ���� ��ų ���̵� �Ľ��ؼ� �����̳ʿ� ����
	if( true == luaManager.BeginTable( "m_SkillID" ) )
	{
		int iSkillID	= 0;	/// ��ų ���̵�
		int iTableIndex = 1;	/// ���̺� �ε���

		while( luaManager.GetValue( iTableIndex, iSkillID ) == true )
		{
			vecSkillID.push_back( static_cast<SKILL_ID>( iSkillID ) );
			++iTableIndex;
		}

		luaManager.EndTable();
	}

	/// �ʿ� ����
	SealSkillItemInfo kInfo;
	kInfo.m_iUnitClassType	= static_cast<int>( eUnitClassType );
	kInfo.m_vecSkillID		= vecSkillID;
	m_mapSealSkillItemTemplet.insert( std::make_pair( iUnsealItemID, kInfo ) );
	return true;
}

#else // UPGRADE_SKILL_SYSTEM_2013

bool CX2SkillTree::AddSealSkillInfo_LUA( int iUnsealItemID, int iSkillID, char cUnitClass )
{
	if( iUnsealItemID <= 0  ||  iSkillID <= 0  ||  cUnitClass <= 0 )
		return false;

	SealSkillItemInfo kInfo;
	kInfo.m_iSkillID = iSkillID;
	kInfo.m_cUnitClass = cUnitClass;
	m_mapSealSkillItemTemplet.insert( std::make_pair( iUnsealItemID, kInfo ) );
	return true;
}

#endif // UPGRADE_SKILL_SYSTEM_2013
//}}



const CX2SkillTree::SkillTreeTemplet* CX2SkillTree::GetSkillTreeTemplet( int iUnitClass, SKILL_ID eSkillID )
{
	KTDXPROFILE();

	UnitSkillTreeTempletMap::iterator it = m_mapUnitSkillTreeTemplet.find( iUnitClass );
	if( it == m_mapUnitSkillTreeTemplet.end() )
		return 0;

	SkillTreeTempletMap& mapSkillTreeTemplet = it->second;
	SkillTreeTempletMap::iterator itSkillTreeTemplet = mapSkillTreeTemplet.find( eSkillID );
	if( itSkillTreeTemplet == mapSkillTreeTemplet.end() )
		return 0;

	return &itSkillTreeTemplet->second;
}
// oasis907 : ����� [2009.11.25] // ���� �Ľ� ���� �ּ� ����
#ifdef GUILD_SKILL
//{{ oasis907 : ����� //// 2009-11-16 //// 
const CX2SkillTree::SkillTreeTemplet* CX2SkillTree::GetGuildSkillTreeTemplet( int iGuildClass, SKILL_ID eSkillID )
{
	KTDXPROFILE();

	if(iGuildClass != 0)
	{
		ASSERT( !"Guild Class Not Implemented Yet!!" );
	}

	GuildSkillTreeTempletMap::iterator it = m_mapGuildSkillTreeTemplet.find( iGuildClass );
	if( it == m_mapGuildSkillTreeTemplet.end() )
		return 0;

	SkillTreeTempletMap& mapSkillTreeTemplet = it->second;
	SkillTreeTempletMap::iterator itSkillTreeTemplet = mapSkillTreeTemplet.find( eSkillID );
	if( itSkillTreeTemplet == mapSkillTreeTemplet.end() )
		return 0;

	return &itSkillTreeTemplet->second;
}
//}} oasis907 : ����� //// 2009-11-16 //// 
#endif GUILD_SKILL

int CX2SkillTree::GetMaxSkillLevel( int iUnitClass, SKILL_ID eSkillID )
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
#ifndef _SERVICE_
	/// ������ ��� - 30�������� ���� �����ϵ��� ����
	if( true == g_pMain->IsMyAuthLevelHigherThan( CX2User::XUAL_DEV ) )
	{
		return MAX_LIMITED_SKILL_LEVEL;
	}
#endif _SERVICE_
#endif // UPGRADE_SKILL_SYSTEM_2013

	const SkillTreeTemplet* pSkillTreeTemplet = GetSkillTreeTemplet( iUnitClass, eSkillID );
	if( NULL == pSkillTreeTemplet )
		return 0;

	return pSkillTreeTemplet->m_iMaxSkillLevel;
}
#ifdef GUILD_SKILL
//{{ oasis907 : ����� //// 2009-11-16 //// 
int CX2SkillTree::GetMaxGuildSkillLevel( int iGuildClass, SKILL_ID eSkillID )
{
	const SkillTreeTemplet* pSkillTreeTemplet = GetGuildSkillTreeTemplet( iGuildClass, eSkillID );
	if( NULL == pSkillTreeTemplet )
		return 0;

	return pSkillTreeTemplet->m_iMaxSkillLevel;
}
//}} oasis907 : ����� //// 2009-11-16 //// 
#endif GUILD_SKILL

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����

const CX2SkillTree::SkillTemplet* CX2SkillTree::GetSkillTemplet( SKILL_ID eSkillID ) const
{
	KTDXPROFILE();

#ifdef EFFECT_TOOL // ����Ʈ ������ ��ų ���ø� ���� �ʵ��� ����
	return NULL;
#endif // EFFECT_TOOL

	SkillTempletMap::const_iterator it = m_mapSkillTemplet.find( eSkillID );
	if( m_mapSkillTemplet.end() != it )
	{
		SkillTemplet* pSkillTemplet = it->second;
		return pSkillTemplet;
	}

	if ( CX2SkillTree::SI_NONE != eSkillID )		/// ��ų ���ø��� ������, ��ų ���̵� 0�� �ƴ� ��
		/// �̻��� ��ų ���̵�� ��ų ���ø��� ã���� �ؿ�!!
		ASSERT( !L"Not Find Skill Templet From Unknown SKill ID!!!" );
	

	return NULL;
}

#else // UPGRADE_SKILL_SYSTEM_2013

const CX2SkillTree::SkillTemplet* CX2SkillTree::GetSkillTemplet( SKILL_ID eSkillID, int iSkillLevel ) const
{
	SkillKey skillKey;
	skillKey.m_eID			= eSkillID;
	skillKey.m_iSkillLevel	= iSkillLevel;

	SkillTempletMap::const_iterator it = m_mapSkillTemplet.find( skillKey );
	if( m_mapSkillTemplet.end() != it )
	{
		SkillTemplet* pSkillTemplet = it->second;
		return pSkillTemplet;
	}

	return NULL;
}

#endif // UPGRADE_SKILL_SYSTEM_2013

// 09.05.22 �¿�
const CX2SkillTree::SkillTreeTempletMap& CX2SkillTree::GetSkillTreeTempletMap( int eUnitClass )
{
	UnitSkillTreeTempletMap::iterator it = m_mapUnitSkillTreeTemplet.find(eUnitClass);
	if( it != m_mapUnitSkillTreeTemplet.end() )
	{
		return it->second;
	}

	ASSERT( !"UnitClass Not Found!!" );
	it = m_mapUnitSkillTreeTemplet.begin();
	return it->second;

}

#ifdef X2TOOL
CX2SkillTree::SkillTreeTempletMap& CX2SkillTree::GetSkillTreeTempletMap4Tool( int eUnitClass )
{
	UnitSkillTreeTempletMap::iterator it = m_mapUnitSkillTreeTemplet.find(eUnitClass);
	if( it != m_mapUnitSkillTreeTemplet.end() )
	{
		return it->second;
	}

	ASSERT( !"UnitClass Not Found!!" );
	it = m_mapUnitSkillTreeTemplet.begin();
	return it->second;
}
#endif

bool CX2SkillTree::IsUnitTypeDefaultSkill( SKILL_ID eSkillID )
{
	switch( eSkillID )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	case SI_A_ES_MEGASLASH:
	case SI_A_AV_CHAIN_FIRE_BALL:
	case SI_A_LE_RAIL_STINGER:
	case SI_A_RF_MAXIMUM_CANNON:
	case SI_A_EN_ILLUSION_STRIKE:
#else //UPGRADE_SKILL_SYSTEM_2013
	case SI_SA_ES_MEGASLASH:
	case SI_SA_AV_CHAIN_FIRE_BALL:
	case SI_SA_LE_RAIL_STINGER:
	case SI_SA_RF_MAXIMUM_CANNON:
	case SI_SA_EN_ILLUSION_STRIKE:
#endif //UPGRADE_SKILL_SYSTEM_2013
#ifdef ELSWORD_SHEATH_KNIGHT
	case SI_A_ES_ROLLING:
#else
	case SI_A_ES_COUNTER:
#endif ELSWORD_SHEATH_KNIGHT
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case SI_A_AV_FIRE_ROAD:
#else
	case SI_A_AV_TELEPORT:
#endif
#ifdef SERV_TRAPPING_RANGER_TEST
	case SI_A_LE_RISING_FALCON:
#else
	case SI_A_LE_SIEGEMODE:
#endif SERV_TRAPPING_RANGER_TEST
#ifdef RAVEN_WEAPON_TAKER
	case SI_A_RF_BREAKING_FIST:
#else
	case SI_A_RF_BURNING_RUSH:
#endif RAVEN_WEAPON_TAKER
#ifdef EVE_ELECTRA
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		case SI_A_EN_PHOTON_BLINK:
#else //UPGRADE_SKILL_SYSTEM_2013
		case SI_A_EVE_PHOTON_BLINK:
#endif //UPGRADE_SKILL_SYSTEM_2013
#else
	case SI_A_EN_CLOAKING:
#endif EVE_ELECTRA
#ifdef ARA_CHARACTER_BASE
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	case SI_A_AM_WIND_WEDGE:
	case SI_A_AM_GAIN_FORCE:
#else //UPGRADE_SKILL_SYSTEM_2013
	case SI_SA_ARA_WIND_WEDGE:
	case SI_A_ARA_GAIN_FORCE:
#endif //UPGRADE_SKILL_SYSTEM_2013
#endif
		{
			return true;
		} break;
	}

	return false;
}


bool CX2SkillTree::AddGuildSkillTreeTemplet_LUA()

{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����

	SKILL_ID eSkillID = SI_NONE;
	LUA_GET_VALUE_ENUM( luaManager, "m_iSkillID",				eSkillID,				SKILL_ID,					SI_NONE					);

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	const SkillTemplet* pSkillTemplet = GetSkillTemplet( eSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	const SkillTemplet* pSkillTemplet = GetSkillTemplet( eSkillID, 1 );
#endif // UPGRADE_SKILL_SYSTEM_2013
	if( NULL == pSkillTemplet )
	{
		StateLog( L"skilltemplet is null on loading skilltree templet" );
		return false;
	}

	SkillTreeTemplet skillTreeTemplet;
	LUA_GET_VALUE( luaManager,		"m_iMasterLevel",			skillTreeTemplet.m_iMaxSkillLevel,		0			);
	LUA_GET_VALUE( luaManager,		"m_iRowIndex",				skillTreeTemplet.m_iTier,				0			);
	LUA_GET_VALUE( luaManager,		"m_iColumnIndex",			skillTreeTemplet.m_iIndexInTier,		0			);

	int iPrecedingSkillID = 0;
	LUA_GET_VALUE( luaManager,		"m_iPrecedingSkill",		skillTreeTemplet.m_iPrecedingSkill,		0			);

	SkillTreeTempletMap& mapSkillTreeTemplet = m_mapGuildSkillTreeTemplet[ 0 ];

#else // UPGRADE_SKILL_SYSTEM_2013

	int eGuildClass = 0;
	SKILL_ID eSkillID = SI_NONE;
	LUA_GET_VALUE_ENUM( luaManager, "GUILD_CLASS",				eGuildClass,				int,						0			);
	LUA_GET_VALUE_ENUM( luaManager, "SKILL_ID",					eSkillID,				SKILL_ID,					SI_NONE					);



	const SkillTemplet* pSkillTemplet = GetSkillTemplet( eSkillID, 1 );
	if( NULL == pSkillTemplet )
	{
		StateLog( L"skilltemplet is null on loading skilltree templet" );
		return false;
	}


	SkillTreeTemplet skillTreeTemplet;
	LUA_GET_VALUE( luaManager,		"MAX_SKILL_LEVEL",			skillTreeTemplet.m_iMaxSkillLevel,	0				);
	LUA_GET_VALUE( luaManager,		"TIER_INDEX",				skillTreeTemplet.m_iTier,			0				);
	LUA_GET_VALUE( luaManager,		"INDEX_IN_TIER",			skillTreeTemplet.m_iIndexInTier,	0				);



	std::wstring wstrPrecedingSkill = L"";
	LUA_GET_VALUE( luaManager,		"m_vecPrecedingSkill",		wstrPrecedingSkill, 		L""		);

	if( false == wstrPrecedingSkill.empty() )
	{
		WCHAR* pBegin = const_cast<WCHAR*>(wstrPrecedingSkill.c_str());
		WCHAR* pEnd = pBegin + wstrPrecedingSkill.length();


		while( true )
		{
			int iSkillID = TokenizeInt( &pBegin, pEnd );
			if( iSkillID > 0 )
				skillTreeTemplet.m_vecPrecedingSkill.push_back( iSkillID );
			else 
				break;
		}
	}


	SkillTreeTempletMap& mapSkillTreeTemplet = m_mapGuildSkillTreeTemplet[ (int)eGuildClass ];

#endif // UPGRADE_SKILL_SYSTEM_2013

	mapSkillTreeTemplet[ eSkillID ] = skillTreeTemplet;


	return true;

}



#ifdef GUILD_SKILL
//{{ oasis907 : ����� //// 2009-11-12 //// 

const CX2SkillTree::SkillTreeTempletMap& CX2SkillTree::GetGuildSkillTreeTempletMap( int eGuildClass )
{
	GuildSkillTreeTempletMap::iterator it = m_mapGuildSkillTreeTemplet.find(eGuildClass);
	if( it != m_mapGuildSkillTreeTemplet.end() )
	{
		return it->second;
	}
	/*
	//{{ oasis907 : ����� //// 2009-11-17 //// �ӽ�
	ASSERT( !"GuildClass Not Found!!" );
	*/
	return nulltemplet;


	it = m_mapGuildSkillTreeTemplet.begin();
	return it->second;

}
//}} oasis907 : ����� //// 2009-11-12 //// 
#endif GUILD_SKILL


//{{ �ֹ�ö [2013/1/4]  ���ӳ� ���� ��Ʈ���� �������Ϸ� ���
#ifdef PRINT_INGAMEINFO_TO_EXCEL
/*
void CX2SkillTree::PrintSkillInfo_ToExcel()
{
	SkillTemplet* pSkillTemplet = NULL;
	map<SkillKey, SkillTemplet*>::iterator iter;
	wofstream of(L"��ų ���.txt");
	of.imbue(std::locale("kor",locale::ctype));

	for( iter = m_mapSkillTemplet.begin(); iter != m_mapSkillTemplet.end(); iter++ )
	{
		pSkillTemplet = iter->second;
		of << endl << pSkillTemplet->m_eID << endl << pSkillTemplet->m_Name << endl << endl << pSkillTemplet->m_Desc <<endl;
		of << L"----------------------------------------------------------------------------------------"<< endl;
	}

	of.close();
}
*/

void CX2SkillTree::PrintSkillInfo_ToExcel()
{
	BasicExcel e;
	e.New(1);
	
	BasicExcelWorksheet* sheet = e.GetWorksheet((size_t)0);
	int cal = 1;
	SkillTemplet* pSkillTemplet = NULL;
	map<SkillKey, SkillTemplet*>::iterator iter;
	
	// Title ���
	sheet->Cell(0,0)->SetWString(L"��ų ID");	
	sheet->Cell(0,1)->SetWString(L"��ų �̸�");	
	sheet->Cell(0,2)->SetWString(L"��ų ����");	

	// ���� ���
	for( iter = m_mapSkillTemplet.begin(); iter != m_mapSkillTemplet.end(); iter++ )
	{
		pSkillTemplet = iter->second;
		if(sheet)
		{
			sheet->Cell(cal,0)->SetInteger(pSkillTemplet->m_eID);
			sheet->Cell(cal,1)->SetWString(pSkillTemplet->m_wstrName.c_str());
			sheet->Cell(cal,2)->SetWString(pSkillTemplet->m_wstrMainDesc.c_str());	
		}
		cal++;
	}
	e.SaveAs("SKILL_LIST.xls");
}
#endif PRINT_INGAMEINFO_TO_EXCEL
//}} �ֹ�ö [2013/1/4]  ���ӳ� ���� ��Ʈ���� �������Ϸ� ���

//////////////////////////////////////////////////////////////////////////
bool CX2SkillTree::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case EGS_RESET_SKILL_ACK:
		{
			return Handler_EGS_RESET_SKILL_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_INIT_SKILL_TREE_ACK:
		{
			return Handler_EGS_INIT_SKILL_TREE_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_GET_SKILL_ACK:
		{
			return Handler_EGS_GET_SKILL_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_CHANGE_SKILL_SLOT_ACK:
		{
			return Handler_EGS_CHANGE_SKILL_SLOT_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_CHANGE_SKILL_SLOT_NOT:
		{
			return Handler_EGS_CHANGE_SKILL_SLOT_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_EXPIRE_CASH_SKILL_POINT_NOT:
		{
			return Handler_EGS_EXPIRE_CASH_SKILL_POINT_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_UPDATE_CASH_SKILL_POINT_NOT:
		{
			return Handler_EGS_UPDATE_CASH_SKILL_POINT_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_EXPAND_SKILL_SLOT_NOT:
		{
			return Handler_EGS_EXPAND_SKILL_SLOT_NOT( hWnd, uMsg, wParam, lParam );
		} break;


	case EGS_EXPIRE_SKILL_SLOT_NOT:
		{
			return Handler_EGS_EXPIRE_SKILL_SLOT_NOT( hWnd, uMsg, wParam, lParam );
		} break;

#ifdef SERV_SKILL_SLOT_CHANGE_INVENTORY
	case EGS_SKILL_SLOT_CHANGE_ITEM_NOT:
		{
			return Handler_EGS_SKILL_SLOT_CHANGE_ITEM_NOT( hWnd, uMsg, wParam, lParam );
		} break;
#endif SERV_SKILL_SLOT_CHANGE_INVENTORY

	case EGS_UNSEAL_SKILL_NOT:
		{
			return Handler_EGS_UNSEAL_SKILL_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_ACK:
		{
			Handler_EGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_ACK( hWnd, uMsg, wParam, lParam );
			return true;
		} break;
#ifdef GUILD_SKILL

	case EGS_RESET_GUILD_SKILL_ACK:
		{
			return Handler_EGS_RESET_GUILD_SKILL_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_INIT_GUILD_SKILL_ACK:
		{
			return Handler_EGS_INIT_GUILD_SKILL_TREE_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_GET_GUILD_SKILL_ACK:
		{
			return Handler_EGS_GET_GUILD_SKILL_ACK( hWnd, uMsg, wParam, lParam );
		} break;


	case EGS_RESET_GUILD_SKILL_NOT:
		{
			return Handler_EGS_RESET_GUILD_SKILL_NOT( hWnd, uMsg, wParam, lParam );
		} break;


	case EGS_INIT_GUILD_SKILL_NOT:
		{
			return Handler_EGS_INIT_GUILD_SKILL_TREE_NOT( hWnd, uMsg, wParam, lParam );
		} break;



	case EGS_GET_GUILD_SKILL_NOT:
		{
			return Handler_EGS_GET_GUILD_SKILL_NOT( hWnd, uMsg, wParam, lParam );
		} break;


/*
	case EGS_CHANGE_GUILD_SKILL_SLOT_ACK:
		{
			return Handler_EGS_CHANGE_GUILD_SKILL_SLOT_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_CHANGE_GUILD_SKILL_SLOT_NOT:
		{
			return Handler_EGS_CHANGE_GUILD_SKILL_SLOT_NOT( hWnd, uMsg, wParam, lParam );
		} break;

*/

	case EGS_EXPIRE_GUILD_CASH_SKILL_POINT_NOT:
		{
			return Handler_EGS_EXPIRE_GUILD_CASH_SKILL_POINT_NOT( hWnd, uMsg, wParam, lParam );
		} break;
/*
	case EGS_UPDATE_CASH_GUILD_SKILL_POINT_NOT:
		{
			return Handler_EGS_UPDATE_CASH_GUILD_SKILL_POINT_NOT( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_EXPAND_GUILD_SKILL_SLOT_NOT:
		{
			return Handler_EGS_EXPAND_GUILD_SKILL_SLOT_NOT( hWnd, uMsg, wParam, lParam );
		} break;


	case EGS_EXPIRE_GUILD_SKILL_SLOT_NOT:
		{
			return Handler_EGS_EXPIRE_GUILD_SKILL_SLOT_NOT( hWnd, uMsg, wParam, lParam );
		} break;


	case EGS_UNSEAL_GUILD_SKILL_NOT:
		{
			return Handler_EGS_UNSEAL_GUILD_SKILL_NOT( hWnd, uMsg, wParam, lParam );
		} break;
*/

//}} oasis907 : ����� //// 2009-11-12 //// 
#endif GUILD_SKILL

#ifdef SKILL_PAGE_SYSTEM //JHKang
	case EGS_GET_NEXT_SKILL_PAGE_ED_ACK:
		{
			return Handler_EGS_GET_NEXT_SKILL_PAGE_ED_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_EXPAND_SKILL_PAGE_ACK:
		{
			return Handler_EGS_EXPAND_SKILL_PAGE_ACK( hWnd, uMsg, wParam, lParam );
		} break;

	case EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK:
		{
			return Handler_EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK( hWnd, uMsg, wParam, lParam );
		} break;
#endif //SKILL_PAGE_SYSTEM
	}

	return false;
}




bool CX2SkillTree::Handler_EGS_CHANGE_SKILL_SLOT_REQ( int iSlotID, CX2SkillTree::SKILL_ID eSkillID )
{

	// ���� ���� ���Ŀ� skill slot �ٲٱ⸦ �õ��ϸ� �ȵǰ� ���´�. ���� �÷��� ������. 
	// ������ų ������ �濡 ������ �� �ް�, ���� ���� ���Ŀ� �ٽ� �޴µ�, 
	// ��ų ���ÿ� MP �Ҹ� ������ ���ӽ��������� ������ �Ǵµ� ���ӽ��� �Ŀ��� ������ �ȵǱ� ������ ����� ����
	if( false == g_pInstanceData->IsElapsedPacketTiming() )
	{
		return false;
	}
	//g_pInstanceData->GetTimerPacketTiming().restart();


//{{ kimhc // 2010.3.26 // ���� ��ų ���� ����
#ifdef MODIFY_INFINITE_SKILL_BUG
	// Change_state_Game_start_req�� �����ϸ� ���Ŀ��� ���� ������ ���� �� �� ���� ��ų������ �ٲܼ� ����
	if ( g_pInstanceData->GetSendGameStartReqPacket() == true )
		return true;
#endif MODIFY_INFINITE_SKILL_BUG
//}} kimhc // 2010.3.26 // ���� ��ų ���� ����

	if ( g_pData->GetMyUser() == NULL )
		return false;

	if ( g_pData->GetMyUser()->GetSelectUnit() == NULL )
		return false;
	
#ifdef FIX_SKILL_SLOT_CHANGE_BUG
	if( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() &&
		false == g_pX2Game->GetMyUnit()->IsValideSlotChange() ) // ���� ���� ��ȿ ���� �˻�
	{
		return false;
	}
#endif // FIX_SKILL_SLOT_CHANGE_BUG

	KEGS_CHANGE_SKILL_SLOT_REQ kPacket;
	kPacket.m_iSlotID = iSlotID;
	kPacket.m_iSkillID = static_cast<int>(eSkillID);

	g_pData->GetServerProtocol()->SendPacket( EGS_CHANGE_SKILL_SLOT_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_CHANGE_SKILL_SLOT_ACK );

	//{{ kimhc // 2010.7.29 // ���Խ� ���� ��ų ���� ����
#ifdef	MODIFY_INFINITE_SKILL_BUG_WHEN_INTRUDE
	SetSkillSlotChanging( true );			// ��ų����ü���� ��
#endif	MODIFY_INFINITE_SKILL_BUG_WHEN_INTRUDE
	//}} kimhc // 2010.7.29 // ���Խ� ���� ��ų ���� ����

	return true;
}


bool CX2SkillTree::Handler_EGS_CHANGE_SKILL_SLOT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_SKILL_SLOT_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	

	g_pMain->DeleteServerPacket( EGS_CHANGE_SKILL_SLOT_ACK );

	if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
	{

// #ifdef FIX_SKILL_SLOT_CHANGE_BUG
// 		if( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() && 
// 			false == g_pX2Game->GetMyUnit()->IsValideSlotChange() ) // ���� ���� ��ȿ ���� �˻�
// 		{
// 			return true;
// 		}
// #endif // FIX_SKILL_SLOT_CHANGE_BUG


		CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();
//{{ kimhc // 2010.3.26 // ���� ��ų ���� ����
#ifdef MODIFY_INFINITE_SKILL_BUG
		// 2010-03-23 // Change_state_Game_Start_Not�� ���� ���Ŀ��� �ٲ� �� ����
		if ( g_pInstanceData->GetReceiveGameStartNotPacket() == false )
#endif MODIFY_INFINITE_SKILL_BUG
//}} kimhc // 2010.3.26 // ���� ��ų ���� ����
		{
			pUnitData->m_UserSkillTree.SetSkillSlotInfo( kEvent.m_iSlotID, 
				static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iSkillID ) );

#ifdef SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
			if ( -1 != kEvent.m_iSlotID2 )
			{
				pUnitData->m_UserSkillTree.SetSkillSlotInfo( kEvent.m_iSlotID2, 
					static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iSkillID2 ) );
			}
#endif // SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE


			if( NULL != g_pData->GetUIManager()  )
			{
				g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, false, true);
			}

			//{{ kimhc // 2010.7.29 // ���Խ� ���� ��ų ���� ����
#ifdef	MODIFY_INFINITE_SKILL_BUG_WHEN_INTRUDE
			SetSkillSlotChanging( false );			// ��ų����ü���� �Ϸ�
#endif	MODIFY_INFINITE_SKILL_BUG_WHEN_INTRUDE
			//}} kimhc // 2010.7.29 // ���Խ� ���� ��ų ���� ����
		}

		if ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
		{
#ifdef FIX_SKILL_SLOT_CHANGE_BUG
			g_pX2Game->GetMyUnit()->SetCanNotInputTime_LUA(0.5f); // ���� �ð� ��ų ��� �Ұ� ���·� ����
#endif // FIX_SKILL_SLOT_CHANGE_BUG
			g_pX2Game->GetMyUnit()->ChangeEquippedSkillState( kEvent.m_iSlotID, kEvent.m_iSkillID );

#ifdef SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
			if ( -1 != kEvent.m_iSlotID2 )
			{
				g_pX2Game->GetMyUnit()->ChangeEquippedSkillState( kEvent.m_iSlotID2, kEvent.m_iSkillID2 );
			}
#endif // SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
 		}

#ifdef FIX_SKILL_SLOT_COOLTIME_IN_VILLAGE //2013.08.09
		if ( NULL != g_pTFieldGame && NULL != g_pData->GetMyUser() )
		{
			g_pData->GetMyUser()->ChangeEquippedSkillState( kEvent.m_iSlotID, kEvent.m_iSkillID );

			#ifdef SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
			if ( -1 != kEvent.m_iSlotID2 )
			{
				g_pData->GetMyUser()->ChangeEquippedSkillState( kEvent.m_iSlotID2, kEvent.m_iSkillID2 );
			}
			#endif // SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
		}
#endif //FIX_SKILL_SLOT_COOLTIME_IN_VILLAGE
	}
	return true;
}


bool CX2SkillTree::Handler_EGS_CHANGE_SKILL_SLOT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_CHANGE_SKILL_SLOT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );	


	// ���� �� ������ ��� ó��
	CX2Unit::UnitData* pUnitData = NULL;
	if ( g_pMain->GetNowStateID() == CX2Main::XS_PVP_ROOM )
	{
		CX2StatePVPRoom* pkStatePVPRoom = (CX2StatePVPRoom*) g_pMain->GetNowState();
		CX2Room::SlotData* pkSlotData = g_pX2Room->GetSlotDataByUnitUID( kEvent.m_iUnitUID );
		if ( pkSlotData != NULL )
		{
			pUnitData = &pkSlotData->m_pUnit->AccessUnitData();
		}
	}

	if ( pUnitData == NULL )
		return false;


	pUnitData->m_UserSkillTree.SetSkillSlotInfo( kEvent.m_iSlotID, (CX2SkillTree::SKILL_ID) kEvent.m_iSkillID );

	return true;
}


#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����

#ifdef SKILL_PAGE_SYSTEM //JHKang
bool CX2SkillTree::Handler_EGS_GET_SKILL_REQ( USHORT usPageNumber_/* = 0*/ )
#else //SKILL_PAGE_SYSTEM
bool CX2SkillTree::Handler_EGS_GET_SKILL_REQ()
#endif //SKILL_PAGE_SYSTEM
{
#ifdef DISABLE_REDUDANT_PACKET_TEST
	if( true == g_pMain->IsWaitingServerPacket( EGS_GET_SKILL_ACK ) )
	{
		return false;
	}
#endif DISABLE_REDUDANT_PACKET_TEST

	if (	NULL == g_pData ||
			NULL == g_pData->GetSkillTree() )
	{
		return false;
	}

	const std::map<int, KGetSkillInfo>& mapSkillInfo = g_pData->GetSkillTree()->GetMapSkillInfo();

	if ( 0 == mapSkillInfo.size() )
		return false;

	KEGS_GET_SKILL_REQ kPacket;
	kPacket.m_mapSkillList = mapSkillInfo;
#ifdef SKILL_PAGE_SYSTEM //JHKang
	USHORT usSkillPage = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.GetUsingPage() + 1;

	if ( usPageNumber_ > 0 )
		usSkillPage = usPageNumber_;

	kPacket.m_iActiveSkillPageNumber = usSkillPage;
#endif //SKILL_PAGE_SYSTEM

#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
	kPacket.m_vecNowLearnSkill = m_vecNowLearnSkill;
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_SKILL_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_GET_SKILL_ACK );

	return true;
}

#else // UPGRADE_SKILL_SYSTEM_2013

bool CX2SkillTree::Handler_EGS_GET_SKILL_REQ( CX2SkillTree::SKILL_ID eSkillID )
{

#ifdef DISABLE_REDUDANT_PACKET_TEST
	if( true == g_pMain->IsWaitingServerPacket( EGS_GET_SKILL_ACK ) )
	{
		return false;
	}
#endif DISABLE_REDUDANT_PACKET_TEST



	if ( g_pData->GetMyUser() == NULL )
		return false;

	if ( g_pData->GetMyUser()->GetSelectUnit() == NULL )
		return false;


	KEGS_GET_SKILL_REQ kPacket;
	kPacket.m_iSkillID = (int) eSkillID;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_SKILL_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_GET_SKILL_ACK );

	return true;
}

#endif // UPGRADE_SKILL_SYSTEM_2013




bool CX2SkillTree::Handler_EGS_GET_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_SKILL_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	


	g_pMain->DeleteServerPacket( EGS_GET_SKILL_ACK );
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����

		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_25108 ), g_pMain->GetNowState() );

			if (	NULL == g_pData ||
					NULL == g_pData->GetMyUser() ||
					NULL == g_pData->GetMyUser()->GetSelectUnit() ||
					NULL == g_pData->GetUIManager() ||
					NULL == g_pData->GetUIManager()->GetUISkillTree() )
			{
				return false;
			}

			CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

			/// ���� ����Ʈ ����
			pUnitData->m_iSPoint	= kEvent.m_iRemainSP;
			pUnitData->m_iCSPoint	= kEvent.m_iRemainCSP;

#ifdef SKILL_PAGE_SYSTEM //JHKang
			pUnitData->m_UserSkillTree.SetUsingPage( kEvent.m_iActiveSkillPageNumber );
			pUnitData->m_UserSkillTree.SetSkillPoint( pUnitData->m_iSPoint );
			pUnitData->m_UserSkillTree.SetCashSkillPoint( pUnitData->m_iCSPoint );
#endif //SKILL_PAGE_SYSTEM

			std::map<int, KGetSkillInfo>::iterator it = kEvent.m_mapSkillList.begin();

			for ( ; it != kEvent.m_mapSkillList.end(); ++it )
			{
				KGetSkillInfo getSkillInfo = it->second;

#ifdef SKILL_PAGE_SYSTEM //JHKang
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( getSkillInfo.m_iSkillID ), 
																getSkillInfo.m_iSkillLevel, getSkillInfo.m_iSpendSkillCSPoint,
																pUnitData->m_UserSkillTree.GetUsingPage() );
#else //SKILL_PAGE_SYSTEM
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( getSkillInfo.m_iSkillID ), 
					getSkillInfo.m_iSkillLevel, getSkillInfo.m_iSpendSkillCSPoint );
#endif //SKILL_PAGE_SYSTEM

				bool bEnableSkillEquipped = true;
				const CX2SkillTree::SKILL_ID eSkillId = static_cast<CX2SkillTree::SKILL_ID>( getSkillInfo.m_iSkillID );

				const CX2SkillTree::SkillTemplet* pSkillTemplet = GetSkillTemplet( eSkillId );
				switch( pSkillTemplet->m_eType )
				{
				case CX2SkillTree::ST_PASSIVE_PHYSIC_ATTACK:
				case CX2SkillTree::ST_PASSIVE_MAGIC_ATTACK:
				case CX2SkillTree::ST_PASSIVE_MAGIC_DEFENCE:
				case CX2SkillTree::ST_PASSIVE_PHYSIC_DEFENCE:
				case CX2SkillTree::ST_PASSIVE:
					{
						if( g_pData != NULL && 
							g_pData->GetUIManager() != NULL && 
							g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
						{
							g_pData->GetUIManager()->GetUICharInfo()->ResetStat();
						}
					} break;
				}
			}

			// ����
			g_pKTDXApp->GetDeviceManager()->PlaySound( L"SkillTree_SkillUp.ogg", false, false );

			if  ( true == g_pData->GetUIManager()->GetUISkillTree()->GetShow() )
				g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, true, true);

			/// ��ų �߰� ���� ȿ�� ����
			g_pData->GetMyUser()->GetSelectUnit()->ResetIncreaseSkillLevelByBuff();
			g_pData->GetMyUser()->GetSelectUnit()->ResetIncreaseSkillLevelBySocket();
		}

#else // UPGRADE_SKILL_SYSTEM_2013

		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			bool bEnableSkillEquipped = true;
			const CX2SkillTree::SKILL_ID eSkillId = static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iSkillID );

			CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();
			{
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( eSkillId, kEvent.m_iSkillLevel, kEvent.m_iSkillCSPoint );
				pUnitData->m_iSPoint	= kEvent.m_iSPoint;
				pUnitData->m_iCSPoint	= kEvent.m_iCSPoint;
			}

			const CX2SkillTree::SkillTemplet* pSkillTemplet = GetSkillTemplet( eSkillId, kEvent.m_iSkillLevel );
			switch( pSkillTemplet->m_eType )
			{
			case CX2SkillTree::ST_ACTIVE:
			case CX2SkillTree::ST_BUFF:
			case CX2SkillTree::ST_SPECIAL_ACTIVE:
#ifdef ADDED_RELATIONSHIP_SYSTEM
			case CX2SkillTree::ST_RELATIONSHIP_SKILL:
#endif // ADDED_RELATIONSHIP_SYSTEM
#ifdef FINALITY_SKILL_SYSTEM //JHKang
			case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
#endif //FINALITY_SKILL_SYSTEM
				{
					// ���� ������ ��ų�� ���� ����ٸ�
					if( kEvent.m_iSkillLevel == 1 )
					{
						// �� ������ �˻��ؼ�
						int EmptySlot = pUnitData->m_UserSkillTree.GetSkillEquippedSlotIndex( CX2SkillTree::SI_NONE, false );
						if( EmptySlot != -1 )
						{
							// �������ش�
							Handler_EGS_CHANGE_SKILL_SLOT_REQ( EmptySlot, eSkillId );
						}
						else if( pUnitData->m_UserSkillTree.GetEnabledSkillSlotB() )
						{
							EmptySlot = pUnitData->m_UserSkillTree.GetSkillEquippedSlotIndex( CX2SkillTree::SI_NONE, true );
							if( EmptySlot != -1 )
							{
								Handler_EGS_CHANGE_SKILL_SLOT_REQ( EmptySlot+EQUIPPED_SKILL_SLOT_COUNT, eSkillId );
							}
						}
					}
					// 					else
					// 					{
					// 						// ���� ���� ������ ��ų�� �ƴϰ�
					// 						if ( NULL != g_pX2Game )	// ���� ���̸�
					// 						{
					// 							// ���� ���� ���� ��ų���� �˾ƺ�
					// 							const int iEquippedSlotAIndex
					// 								= pUnitData->m_UserSkillTree.GetSkillEquippedSlotIndex( eSkillId, false );
					// 
					// 							if ( -1 != iEquippedSlotAIndex )	// ���� A���� ���� ���� ��ų�̶��
					// 							{
					// 								g_pX2Game->GetMyUnit()->ChangeEquippedSkillState( pUnitData, eSkillId, iEquippedSlotAIndex, false );
					// 								g_pX2Game->UpdateSkillSlotUI();
					// 							}
					// 							else	// ���� A���� ���� ���� ��ų�� �ƴϰ�
					// 							{
					// 								const int iEquippedSlotBIndex
					// 									= pUnitData->m_UserSkillTree.GetSkillEquippedSlotIndex( eSkillId, true );
					// 
					// 								if ( -1 != iEquippedSlotBIndex )	// ���� B���� ���� ���� ��ų�̶��
					// 								{
					// 									g_pX2Game->GetMyUnit()->ChangeEquippedSkillState( pUnitData, eSkillId, iEquippedSlotBIndex, true );
					// 									g_pX2Game->UpdateSkillSlotUI();
					// 								}
					// 							}
					// 						}
					// 					}

				} break;
			case CX2SkillTree::ST_PASSIVE_PHYSIC_ATTACK:
			case CX2SkillTree::ST_PASSIVE_MAGIC_ATTACK:
			case CX2SkillTree::ST_PASSIVE_MAGIC_DEFENCE:
			case CX2SkillTree::ST_PASSIVE_PHYSIC_DEFENCE:
			case CX2SkillTree::ST_PASSIVE:
				{
					if( g_pData != NULL && 
						g_pData->GetUIManager() != NULL && 
						g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
					{
						g_pData->GetUIManager()->GetUICharInfo()->ResetStat();
					}
				} break;
			}

			if( NULL != g_pData->GetUIManager() &&
				true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL) )
			{
				//CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();

				g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, true, true);

			}

			// ����
			g_pKTDXApp->GetDeviceManager()->PlaySound( L"SkillTree_SkillUp.ogg", false, false );

			if(pUnitData->m_UserSkillTree.IsMaxSkillLevel( (SKILL_ID)kEvent.m_iSkillID ))
			{
				g_pKTDXApp->GetDeviceManager()->PlaySound( L"SkillTree_SkillMaster.ogg", false, false );
				const SkillTreeTempletMap& refSkillTreeTempletMap = GetSkillTreeTempletMap( (int)pUnitData->m_UnitClass );
				SkillTreeTempletMap::const_iterator it = refSkillTreeTempletMap.find((SKILL_ID)kEvent.m_iSkillID);
				if( it != refSkillTreeTempletMap.end() )
				{
					const CX2SkillTree::SkillTreeTemplet& refSkillTreeTemplet = it->second;
					if(false == refSkillTreeTemplet.m_vecFollowingSkill.empty())
					{
						// ���� ��ų�� ������ (ȭ��ǥ�� ���� ���̴�)
						g_pKTDXApp->GetDeviceManager()->PlaySound( L"SkillTree_SkillRootOpen.ogg", false, false );
					}
				}
			}			
		}

#endif // UPGRADE_SKILL_SYSTEM_2013

		return true;
	}


	return false;

}

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
bool CX2SkillTree::Handler_EGS_RESET_SKILL_REQ( CX2SkillTree::SKILL_ID eDelSkillID )
#else // UPGRADE_SKILL_SYSTEM_2013
bool CX2SkillTree::Handler_EGS_RESET_SKILL_REQ( CX2SkillTree::SKILL_ID eSkillID )
#endif // UPGRADE_SKILL_SYSTEM_2013
{

#ifdef DISABLE_REDUDANT_PACKET_TEST
	if( true == g_pMain->IsWaitingServerPacket( EGS_RESET_SKILL_ACK ) )
	{
		return false;
	}
#endif DISABLE_REDUDANT_PACKET_TEST


	KEGS_RESET_SKILL_REQ kPacket;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	kPacket.m_iSkillID = static_cast<int>( eDelSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	kPacket.m_iSkillID = (int) eSkillID;
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef SKILL_PAGE_SYSTEM //JHKang
	kPacket.m_iActiveSkillPageNumber = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.GetUsingPage() + 1;
#endif //SKILL_PAGE_SYSTEM

	g_pData->GetServerProtocol()->SendPacket( EGS_RESET_SKILL_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_RESET_SKILL_ACK );

	return true;
}

void CX2SkillTree::Hander_EGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ( const int iYear_, const int iMonth_, const int iDay_, const int iHour_, const int iMinute_ )
{
	KEGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ kPacket;

	kPacket.m_iYear		= iYear_;
	kPacket.m_iMonth	= iMonth_;
	kPacket.m_iDay		= iDay_;
	kPacket.m_iHour		= iHour_;
	kPacket.m_iMinute	= iMinute_;

	g_pData->GetServerProtocol()->SendPacket( EGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_ACK );
}

void CX2SkillTree::Handler_EGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			g_pChatBox->AddChatLog( kEvent.m_wstrCspEndDate.c_str(), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
		}
	}
}




bool CX2SkillTree::Handler_EGS_RESET_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( NULL == g_pData ||
		 NULL == g_pData->GetMyUser() ||
		 NULL == g_pData->GetMyUser()->GetSelectUnit() ||
		 NULL == g_pData->GetSkillTree() )
	{
		return false;
	}

	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RESET_SKILL_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_RESET_SKILL_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����

			/// ���� �ȳ� �˾�
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_25108 ), g_pMain->GetNowState() );

			CX2Unit::UNIT_CLASS		eUnitClass		= static_cast<CX2Unit::UNIT_CLASS>( g_pData->GetMyUser()->GetSelectUnit()->GetClass() );
			CX2SkillTree::SKILL_ID	eDeleteSkillID	= static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iDelSkillID );
			CX2UserSkillTree& userSkillTree	= pUnitData->m_UserSkillTree;

			
			/// ��ų Ʈ�� ���ø� ��ȯ
			const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet	= g_pData->GetSkillTree()->GetSkillTreeTemplet( eUnitClass, eDeleteSkillID );

			if ( NULL == pSkillTreeTemplet )
				return false; 

			/// ���� ��ų�� ���� ��ų�� �� ��ų�� ���� �ִ��� �˻�
			const bool bHaveFollowingSkill = 0 < userSkillTree.GetSkillLevel( static_cast<CX2SkillTree::SKILL_ID>( pSkillTreeTemplet->m_iFollowingSkill ) ) ? true : false ;

		#ifdef SKILL_PAGE_SYSTEM //JHKang
			/// �ʱ�ȭ �� ��ų�� �⺻ ��ų �̰ų�, ���� ��ų�� ���� �ִٸ� 1������ �ʱ�ȭ ��Ű��
			if ( true == isDefaultSkill( eDeleteSkillID ) || true == bHaveFollowingSkill )
				userSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iDelSkillID ), 1, 0,
													pUnitData->m_UserSkillTree.GetUsingPage() );
			/// �� �ܿ� 0������ �ʱ�ȭ
			else
				userSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iDelSkillID ), 0, 0,
													pUnitData->m_UserSkillTree.GetUsingPage() );
		#else //SKILL_PAGE_SYSTEM
			/// �ʱ�ȭ �� ��ų�� �⺻ ��ų �̰ų�, ���� ��ų�� ���� �ִٸ� 1������ �ʱ�ȭ ��Ű��
			if ( true == isDefaultSkill( eDeleteSkillID ) || true == bHaveFollowingSkill )
				userSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iDelSkillID ), 1, 0 );
			/// �� �ܿ� 0������ �ʱ�ȭ
			else
				userSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( kEvent.m_iDelSkillID ), 0, 0 );
		#endif //SKILL_PAGE_SYSTEM

			pUnitData->m_iSPoint	= kEvent.m_iSPoint;
			pUnitData->m_iCSPoint	= kEvent.m_iCSPoint;

#ifdef SKILL_PAGE_SYSTEM //JHKang
			pUnitData->m_UserSkillTree.SetUsingPage( kEvent.m_iActiveSkillPageNumber );
			pUnitData->m_UserSkillTree.SetSkillPoint( pUnitData->m_iSPoint );
			pUnitData->m_UserSkillTree.SetCashSkillPoint( pUnitData->m_iCSPoint );
#endif //SKILL_PAGE_SYSTEM

	#else // UPGRADE_SKILL_SYSTEM_2013

			// ��ų Ʈ�� ����
			int iSkillLevel = 0;
			int iSkillCSPoint = 0;
			pUnitData->m_UserSkillTree.GetSkillLevelAndCSP( (CX2SkillTree::SKILL_ID) kEvent.m_iDelSkillID, iSkillLevel, iSkillCSPoint );
			iSkillLevel -= 1;
			if( iSkillCSPoint > 0 )
			{
				iSkillCSPoint -= kEvent.m_iRestoreCSPoint;
			}
			pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( (CX2SkillTree::SKILL_ID) kEvent.m_iDelSkillID, iSkillLevel, iSkillCSPoint ); 

			pUnitData->m_iSPoint	+= kEvent.m_iRestoreSPoint;
			pUnitData->m_iCSPoint	+= kEvent.m_iRestoreCSPoint;

	#endif // UPGRADE_SKILL_SYSTEM_2013

			// ��ų �ʱ�ȭ �������� ����߱� ������ �κ� ������ �����Ѵ�
			if( kEvent.m_vecInventorySlotInfo.size() > 0 )
			{
				if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
				{
					g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
				}
				// UI��..
				if( NULL != g_pData->GetUIManager() &&
					true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_INVEN))
				{
					g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
				}
			}


			// UI ���� �� ���� ��ų ����
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( (CX2SkillTree::SKILL_ID) kEvent.m_iDelSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
			const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( (CX2SkillTree::SKILL_ID) kEvent.m_iDelSkillID, 1 );
#endif // UPGRADE_SKILL_SYSTEM_2013

			if ( NULL == pSkillTemplet )
				return false;

			switch( pSkillTemplet->m_eType )
			{
			case CX2SkillTree::ST_PASSIVE_PHYSIC_ATTACK:
			case CX2SkillTree::ST_PASSIVE_MAGIC_ATTACK:
			case CX2SkillTree::ST_PASSIVE_MAGIC_DEFENCE:
			case CX2SkillTree::ST_PASSIVE_PHYSIC_DEFENCE:
			case CX2SkillTree::ST_PASSIVE:
				{
					if( g_pData != NULL && 
						g_pData->GetUIManager() != NULL && 
						g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
					{
						g_pData->GetUIManager()->GetUICharInfo()->ResetStat();
					}
				} break;

			case CX2SkillTree::ST_BUFF:
			case CX2SkillTree::ST_ACTIVE:
			case CX2SkillTree::ST_SPECIAL_ACTIVE:
#ifdef ADDED_RELATIONSHIP_SYSTEM
			case CX2SkillTree::ST_RELATIONSHIP_SKILL:
#endif // ADDED_RELATIONSHIP_SYSTEM
#ifdef FINALITY_SKILL_SYSTEM //JHKang
			case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
#endif //FINALITY_SKILL_SYSTEM
				{
					for( int i=0; i<EQUIPPED_SKILL_SLOT_COUNT*2; i++ )
					{
						bool bSlotB = (i > 3) ? true : false;
						int iSlotIndex = (i > 3) ? i-4 : i;
						const CX2UserSkillTree::SkillSlotData* pSkillSlotData = pUnitData->m_UserSkillTree.GetSkillSlot( iSlotIndex, bSlotB );
						if( NULL != pSkillSlotData )
						{
							if( pSkillSlotData->m_eID == (CX2SkillTree::SKILL_ID) kEvent.m_iDelSkillID )
							{
								//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
#ifdef ARA_CHARACTER_BASE
								pUnitData->m_UserSkillTree.SetSkillSlot( iSlotIndex, (CX2SkillTree::SKILL_ID)kEvent.m_iDelSkillID, 0, 0, 0, bSlotB );
#else
								pUnitData->m_UserSkillTree.SetSkillSlot( iSlotIndex, (CX2SkillTree::SKILL_ID)kEvent.m_iDelSkillID, 0, 0, bSlotB );
#endif
#else	NEW_CHARACTER_CHUNG
								pUnitData->m_UserSkillTree.SetSkillSlot( iSlotIndex, (CX2SkillTree::SKILL_ID)kEvent.m_iDelSkillID, 0, bSlotB );
#endif	NEW_CHARACTER_CHUNG
								//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
							}
						}
					}

				} break;
			}

			if( NULL != g_pData->GetUIManager() &&
				true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL) )
			{
				g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, true, true);				
			}

			// ����
			g_pKTDXApp->GetDeviceManager()->PlaySound( L"SkillTree_SkillDown.ogg", false, false );


		

		}
		return true;
	}
	return false;
}



bool CX2SkillTree::Handler_EGS_INIT_SKILL_TREE_REQ( UidType itemUID )
{
	KEGS_INIT_SKILL_TREE_REQ kPacket;
	kPacket.m_iItemUID = itemUID;
#ifdef SKILL_PAGE_SYSTEM //JHKang
	kPacket.m_iActiveSkillPageNumber = g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.GetUsingPage() + 1;
#endif //SKILL_PAGE_SYSTEM

	g_pData->GetServerProtocol()->SendPacket( EGS_INIT_SKILL_TREE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_INIT_SKILL_TREE_ACK );

	return true;
}


bool CX2SkillTree::Handler_EGS_INIT_SKILL_TREE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_INIT_SKILL_TREE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_INIT_SKILL_TREE_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();


			if( kEvent.m_vecInventorySlotInfo.size() > 0 )
			{
				if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
				{
					g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
				}
				// UI��..
				if( NULL != g_pData->GetUIManager() && 
					NULL != g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_INVEN) )
				{
					g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList(kEvent.m_vecInventorySlotInfo);
                    
				}
			}

			pUnitData->m_iSPoint = kEvent.m_iSPoint;
			pUnitData->m_iCSPoint = kEvent.m_iCSPoint;

#ifdef SKILL_PAGE_SYSTEM //JHKang
			pUnitData->m_UserSkillTree.SetUsingPage( kEvent.m_iActiveSkillPageNumber );
			pUnitData->m_UserSkillTree.SetSkillPoint( pUnitData->m_iSPoint );
			pUnitData->m_UserSkillTree.SetCashSkillPoint( pUnitData->m_iCSPoint );
#endif //SKILL_PAGE_SYSTEM

			// ��ųƮ���� ������ų �ʱ�ȭ
			pUnitData->m_UserSkillTree.Reset( true, true, false, false );

	#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		#ifdef SKILL_PAGE_SYSTEM //JHKang
			pUnitData->m_UserSkillTree.SetDefaultSkill( pUnitData->m_UserSkillTree.GetUsingPage() );	// �� ������ �⺻ ��ų ����
		#else //SKILL_PAGE_SYSTEM
			pUnitData->m_UserSkillTree.SetDefaultSkill();		/// �� ������ �⺻ ��ų ����
		#endif //SKILL_PAGE_SYSTEM

			/// ������ ����
			if ( NULL != g_pData->GetUIManager() &&  NULL != g_pData->GetUIManager()->GetUISkillTree() )
				g_pData->GetUIManager()->GetUISkillTree()->UpdateUI( true, false, false );
	#else // UPGRADE_SKILL_SYSTEM_2013
			// note!! ĳ���� �߰��Ǹ� �۾��ؾ���
			// ĳ���ͺ� �⺻ ��ų ����
			switch( g_pData->GetMyUser()->GetSelectUnit()->GetType() )
			{
			case CX2Unit::UT_ELSWORD:
				{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_ES_MEGASLASH, 1, 0 );
#ifdef ELSWORD_SHEATH_KNIGHT
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_ES_ROLLING, 1, 0 );
#else
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_ES_COUNTER, 1, 0 );
#endif ELSWORD_SHEATH_KNIGHT
#else //UPGRADE_SKILL_SYSTEM_2013
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_ES_MEGASLASH, 1, 0 );
#ifdef ELSWORD_SHEATH_KNIGHT
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_ES_ROLLING, 1, 0 );
#else
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_ES_COUNTER, 1, 0 );
#endif ELSWORD_SHEATH_KNIGHT
#endif //UPGRADE_SKILL_SYSTEM_2013
				} break;

			case CX2Unit::UT_ARME:
				{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_AV_CHAIN_FIRE_BALL, 1, 0 );
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_AV_FIRE_ROAD, 1, 0 );					
#else
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_AV_TELEPORT, 1, 0 );
#endif
#else //UPGRADE_SKILL_SYSTEM_2013
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_AV_CHAIN_FIRE_BALL, 1, 0 );
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_AV_FIRE_ROAD, 1, 0 );					
#else
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_AV_TELEPORT, 1, 0 );
#endif
#endif //UPGRADE_SKILL_SYSTEM_2013
					
				} break;

			case CX2Unit::UT_LIRE:
				{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_LE_RAIL_STINGER, 1, 0 );
#ifdef SERV_TRAPPING_RANGER_TEST
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_LE_RISING_FALCON, 1, 0 );
#else
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_LE_SIEGEMODE, 1, 0 );
#endif SERV_TRAPPING_RANGER_TEST
#else //UPGRADE_SKILL_SYSTEM_2013
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_LE_RAIL_STINGER, 1, 0 );
#ifdef SERV_TRAPPING_RANGER_TEST
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_LE_RISING_FALCON, 1, 0 );
#else
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_LE_SIEGEMODE, 1, 0 );
#endif SERV_TRAPPING_RANGER_TEST
#endif //UPGRADE_SKILL_SYSTEM_2013
				} break;

			case CX2Unit::UT_RAVEN:
				{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_RF_MAXIMUM_CANNON, 1, 0 );
#ifdef RAVEN_WEAPON_TAKER
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_RF_BREAKING_FIST, 1, 0 );
#else
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_RF_BURNING_RUSH, 1, 0 );
#endif RAVEN_WEAPON_TAKER
#else //UPGRADE_SKILL_SYSTEM_2013
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_RF_MAXIMUM_CANNON, 1, 0 );
#ifdef RAVEN_WEAPON_TAKER
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_RF_BREAKING_FIST, 1, 0 );
#else
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_RF_BURNING_RUSH, 1, 0 );
#endif RAVEN_WEAPON_TAKER
#endif //UPGRADE_SKILL_SYSTEM_2013
				} break;

			case CX2Unit::UT_EVE:
				{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_EN_ILLUSION_STRIKE, 1, 0 );
#ifdef EVE_ELECTRA
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_EN_PHOTON_BLINK, 1, 0 );
#else
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_EN_CLOAKING, 1, 0 );
#endif EVE_ELECTRA
#else //UPGRADE_SKILL_SYSTEM_2013
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_EN_ILLUSION_STRIKE, 1, 0 );
#ifdef EVE_ELECTRA
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_EVE_PHOTON_BLINK, 1, 0 );
#else
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_EN_CLOAKING, 1, 0 );
#endif EVE_ELECTRA
#endif //UPGRADE_SKILL_SYSTEM_2013
				} break;

				//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
		#ifdef	NEW_CHARACTER_CHUNG
			case CX2Unit::UT_CHUNG:
				{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_CC_BRUTAL_SWING, 1, 0 );
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_CC_RELOAD_CANNON, 1, 0 );
#else //UPGRADE_SKILL_SYSTEM_2013
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_CHUNG_BRUTAL_SWING, 1, 0 );
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_CHUNG_RELOAD_CANNON, 1, 0 );
#endif //UPGRADE_SKILL_SYSTEM_2013
				} break;

		#endif	NEW_CHARACTER_CHUNG
				//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

		#ifdef ARA_CHARACTER_BASE
			case CX2Unit::UT_ARA:
				{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_AM_WIND_WEDGE, 1, 0 );
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_AM_GAIN_FORCE, 1, 0 );
#else //UPGRADE_SKILL_SYSTEM_2013
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_SA_ARA_WIND_WEDGE, 1, 0 );
					pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( CX2SkillTree::SI_A_ARA_GAIN_FORCE, 1, 0 );
#endif //UPGRADE_SKILL_SYSTEM_2013
				} break;
		#endif
			}
	#endif // UPGRADE_SKILL_SYSTEM_2013


			if ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
				g_pX2Game->GetMyUnit()->InitEquippedActiveSkillState();
						
			if( NULL != g_pData->GetUIManager() )
			{
				if( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL) )
				{
					g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, true, true);
				}

				if( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO) )
				{
					g_pData->GetUIManager()->GetUICharInfo()->ResetStat();
				}
			}

			//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), L"��� ��ų�� �ʱ�ȭ �Ͽ����ϴ�.", m_pNowStage );
		}
		else
		{
			//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), L"��ų �ʱ�ȭ�� �����Ͽ����ϴ�.", m_pNowStage );
		}
		return true;
	}
	return false;
}

bool CX2SkillTree::Handler_EGS_EXPIRE_CASH_SKILL_POINT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EXPIRE_CASH_SKILL_POINT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );	


	CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();

	switch( g_pMain->GetNowStateID() )
	{
	default:
		{
			On_EGS_EXPIRE_CASH_SKILL_POINT_NOT( kEvent );
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_2687 ), (CKTDXStage*) g_pMain->GetNowState() );
			g_pData->GetUIManager()->GetUISkillTree()->UpdateSkillEquipedSlotUI();
			
			pMyUnit->AccessUnitData().m_wstrCSPointEndDate.clear();

// 			if ( NULL != g_pX2Game )
// 			{
// 				g_pX2Game->GetMyUnit()->UpdateSocketDataAndEnchantData();
// 				g_pX2Game->GetMyUnit()->UpdatePassiveAndActiveSkillState();
// 			}

			

		} break;

	case CX2Main::XS_DUNGEON_GAME:
	case CX2Main::XS_DUNGEON_RESULT:
	case CX2Main::XS_PVP_GAME:
	case CX2Main::XS_PVP_RESULT:
		{
			// packetó���� �����صд�.
			KEGS_EXPIRE_CASH_SKILL_POINT_NOT* pEvent = new KEGS_EXPIRE_CASH_SKILL_POINT_NOT;
			*pEvent = kEvent;
			g_pMain->AddReservedServerPacket( EGS_EXPIRE_CASH_SKILL_POINT_NOT, pEvent );

			wstring wstrSystemMessageColor = L"#CFF3F4D";								// 255, 63, 77
			D3DXCOLOR coSystemMessageTextColor(1.f, 0.24705f, 0.30196f, 1.f);			// 255, 63, 77

			g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2687 ), coSystemMessageTextColor, D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
			g_pChatBox->AddChatLog( GET_STRING( STR_ID_2687 ), KEGS_CHAT_REQ::CPT_SYSTEM, coSystemMessageTextColor, wstrSystemMessageColor );

		} break;
	}


	return true;
}

bool CX2SkillTree::Handler_EGS_UPDATE_CASH_SKILL_POINT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UPDATE_CASH_SKILL_POINT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	

	if( false == g_pMain->IsValidPacket( kEvent.m_iOK ) )
		return false;
	

	if( NULL == g_pData->GetMyUser() ||
		NULL == g_pData->GetMyUser()->GetSelectUnit() )
		return false;

	CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();
	if( pUnitData->m_UnitUID != kEvent.m_iUnitUID )
		return false;


	if( false == kEvent.m_bUpdateEndDateOnly )
	{
#ifdef SKILL_PAGE_SYSTEM //JHKang
		pUnitData->m_iCSPoint = kEvent.m_iCSPoint;

		int iUsingPage = pUnitData->m_UserSkillTree.GetUsingPage();

		for ( int i = 0; i < pUnitData->m_UserSkillTree.GetOpenedPage(); ++i )
		{
			pUnitData->m_UserSkillTree.SetUsingPage( i + 1 );
			pUnitData->m_UserSkillTree.SetCashSkillPoint( pUnitData->m_iCSPoint );
		}

		pUnitData->m_UserSkillTree.SetUsingPage( iUsingPage + 1 );
#else //SKILL_PAGE_SYSTEM
		pUnitData->m_iCSPoint = kEvent.m_iCSPoint;
#endif //SKILL_PAGE_SYSTEM
	}
	pUnitData->m_wstrCSPointEndDate = kEvent.m_wstrCSPointEndDate;


	if( NULL != g_pData->GetUIManager() &&
		true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL) )
	{
		g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(false, true, false);
	}

#ifdef SKILL_PLUS_ITEM_USE_POPUP
	if(kEvent.m_iSkillPointItemID  == SKILL_PLUS_ITEM_ID
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
		|| kEvent.m_iSkillPointItemID  == SKILL_PLUS_ITEM_ID_JP 
		|| kEvent.m_iSkillPointItemID  == SKILL_POINT_10_30DAY_USE_INVEN_JP 
		|| kEvent.m_iSkillPointItemID  == SKILL_POINT_10_15DAY_USE_INVEN_JP 
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP	
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
		|| kEvent.m_iSkillPointItemID == SKILL_POINT_30_7DAY_USE_INVEN
		|| kEvent.m_iSkillPointItemID == SKILL_POINT_60_7DAY_USE_INVEN
		|| kEvent.m_iSkillPointItemID == SKILL_POINT_60_15DAY_USE_INVEN
		|| kEvent.m_iSkillPointItemID == SKILL_POINT_60_30DAY_USE_INVEN
		|| kEvent.m_iSkillPointItemID == SKILL_POINT_60_7DAY_USE_INVEN_2
		|| kEvent.m_iSkillPointItemID == SKILL_POINT_30_7DAY_USE_INVEN_2
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK

#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
		|| kEvent.m_iSkillPointItemID == SKILL_POINT_30_7DAY_USE_INVEN_INT
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_INT

#ifdef SERV_SKILL_5_POINT_7_DAY_EU
		|| kEvent.m_iSkillPointItemID == EVENT_SKILL_5_POINT_7_DAY
#endif SERV_SKILL_5_POINT_7_DAY_EU
#ifdef SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
		|| kEvent.m_iSkillPointItemID == SKILL_POINT_130_1DAY_USE_INVEN
#endif SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
#ifdef SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
		|| kEvent.m_iSkillPointItemID == SKILL_POINT_30_1DAY_USE_INVEN
		|| kEvent.m_iSkillPointItemID == SKILL_POINT_60_1DAY_USE_INVEN
#endif SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
#ifdef SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
		|| kEvent.m_iSkillPointItemID == SKILL_POINT_30_14DAY_USE_INVEN	
#endif SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
#ifdef SERV_LURIEL_GNOSIS
		|| kEvent.m_iSkillPointItemID == LURIEL_GNOSIS_30_15DAY
		|| kEvent.m_iSkillPointItemID == LURIEL_GNOSIS_30_30DAY
		|| kEvent.m_iSkillPointItemID == LURIEL_GNOSIS_30_60DAY
		|| kEvent.m_iSkillPointItemID == LURIEL_GNOSIS_60_15DAY
		|| kEvent.m_iSkillPointItemID == LURIEL_GNOSIS_60_30DAY
		|| kEvent.m_iSkillPointItemID == LURIEL_GNOSIS_60_60DAY
		|| kEvent.m_iSkillPointItemID == LURIEL_GNOSIS_30_7DAY
		|| kEvent.m_iSkillPointItemID == LURIEL_GNOSIS_60_7DAY
#endif //SERV_LURIEL_GNOSIS
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
		|| kEvent.m_iSkillPointItemID == SKILL_POINT_30_7DAY_USE_INVEN_INT
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
		)
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_9836 ), (CKTDXStage*) g_pMain->GetNowState() );
#endif SKILL_PLUS_ITEM_USE_POPUP
	//{{ 2011. 11. 30	������	��Ű�� ��ǰ �߰�
#ifdef SERV_ADD_PACKAGE_PRODUCT
	if(kEvent.m_iSkillPointItemID  == SKILL_POINT_5_USE_INVEN_ITEM_ID || kEvent.m_iSkillPointItemID  == SKILL_POINT_5_USE_INVEN_ITEM_ID_7_DAY)
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_9836 ), (CKTDXStage*) g_pMain->GetNowState() );
	if(kEvent.m_iSkillPointItemID  == SKILL_POINT_10_USE_INVEN_ITEM_ID)
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_9836 ), (CKTDXStage*) g_pMain->GetNowState() );
#endif SERV_ADD_PACKAGE_PRODUCT
	//}}
#ifdef SERV_CN_GNOSIS
	if(kEvent.m_iSkillPointItemID  == EVENT_SKILL_POINT_10_USE_INVEN_7_DAY)
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_9836 ), (CKTDXStage*) g_pMain->GetNowState() );
	if(kEvent.m_iSkillPointItemID  == EVENT_SKILL_POINT_5_USE_INVEN_1_DAY)
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_9836 ), (CKTDXStage*) g_pMain->GetNowState() );
	if(kEvent.m_iSkillPointItemID  == EVENT_SKILL_POINT_5_USE_INVEN_7_DAY)
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_9836 ), (CKTDXStage*) g_pMain->GetNowState() );
	if(kEvent.m_iSkillPointItemID  == EVENT_SKILL_POINT_5_USE_INVEN_7_DAY_TYPE2)
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_9836 ), (CKTDXStage*) g_pMain->GetNowState() );
	if(kEvent.m_iSkillPointItemID  == EVENT_SKILL_POINT_10_USE_INVEN_15_DAY)
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_9836 ), (CKTDXStage*) g_pMain->GetNowState() );
#endif SERV_CN_GNOSIS

#ifdef SERV_GNOSIS_BR
	if( kEvent.m_iSkillPointItemID  == EVENT_SKILL_POINT_5_USE_INVEN_7_DAY || kEvent.m_iSkillPointItemID  == EVENT_SKILL_POINT_5_USE_INVEN_15_DAY )
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_9836 ), (CKTDXStage*) g_pMain->GetNowState() );
#endif SERV_GNOSIS_BR

#ifdef SERV_US_GNOSIS
	if(kEvent.m_iSkillPointItemID  == EVENT_SKILL_POINT_5_USE_INVEN_15_DAY)
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_9836 ), (CKTDXStage*) g_pMain->GetNowState() );
#endif SERV_US_GNOSIS

	return true;
}



bool CX2SkillTree::Handler_EGS_EXPAND_SKILL_SLOT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EXPAND_SKILL_SLOT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	if( NULL == g_pData->GetMyUser() ||
		NULL == g_pData->GetMyUser()->GetSelectUnit() )
		return false;

	if( g_pData->GetMyUser()->GetSelectUnit()->GetUID() != kEvent.m_iUnitUID )
		return false;

	g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlotBExpirationState( (CX2UserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE) kEvent.m_cSkillSlotBExpirationState );
	g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlotBEndDateString( kEvent.m_wstrSkillSlotBEndDate );


	// ���� 
	if( NULL != g_pData->GetUIManager() &&
		true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL) )
	{
		g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, false, false);
	}

#ifdef REFORM_UI_SKILLSLOT
	if( NULL != g_pX2Game )
	{
		g_pX2Game->UpdateSkillSlotUI( true );
		g_pX2Game->InitExpandSkillCoolTime();
	}
#endif //REFORM_UI_SKILLSLOT

	return true;
}


bool CX2SkillTree::Handler_EGS_EXPIRE_SKILL_SLOT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( NULL != g_pData->GetMyUser() ) 
		return false;

	if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
		return false;


	if( CX2UserSkillTree::SSBES_PERMANENT != g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetSkillSlotBExpirationState() )
	{
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlotBExpirationState( CX2UserSkillTree::SSBES_EXPIRED );

		//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
	#ifdef	NEW_CHARACTER_CHUNG
	#ifdef ARA_CHARACTER_BASE
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 0, CX2SkillTree::SI_NONE, 0.f, 0, 0, true );
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 1, CX2SkillTree::SI_NONE, 0.f, 0, 0, true );
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 2, CX2SkillTree::SI_NONE, 0.f, 0, 0, true );
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 3, CX2SkillTree::SI_NONE, 0.f, 0, 0, true );
	#else
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 0, CX2SkillTree::SI_NONE, 0.f, 0, true );
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 1, CX2SkillTree::SI_NONE, 0.f, 0, true );
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 2, CX2SkillTree::SI_NONE, 0.f, 0, true );
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 3, CX2SkillTree::SI_NONE, 0.f, 0, true );
	#endif
	#else	NEW_CHARACTER_CHUNG
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 0, CX2SkillTree::SI_NONE, 0.f, true );
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 1, CX2SkillTree::SI_NONE, 0.f, true );
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 2, CX2SkillTree::SI_NONE, 0.f, true );
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 3, CX2SkillTree::SI_NONE, 0.f, true );
	#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
		
		if( g_pData->GetUIManager() != NULL &&
			g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL))
		{
			g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, false, true);
		}

		switch( g_pMain->GetNowStateID() )
		{
		default:
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_2686 ), (CKTDXStage*) g_pMain->GetNowState() );
			} break;

		case CX2Main::XS_BATTLE_FIELD:
		case CX2Main::XS_DUNGEON_GAME:
		case CX2Main::XS_DUNGEON_RESULT:
		case CX2Main::XS_PVP_GAME:
		case CX2Main::XS_PVP_RESULT:
			{
				wstring wstrSystemMessageColor = L"#CFF3F4D";								// 255, 63, 77
				D3DXCOLOR coSystemMessageTextColor(1.f, 0.24705f, 0.30196f, 1.f);			// 255, 63, 77

				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2686 ), coSystemMessageTextColor, D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				g_pChatBox->AddChatLog( GET_STRING( STR_ID_2686 ), KEGS_CHAT_REQ::CPT_SYSTEM, coSystemMessageTextColor, wstrSystemMessageColor );
			} break;
		}
	}

	return true;
}

#ifdef SERV_SKILL_SLOT_CHANGE_INVENTORY
bool CX2SkillTree::Handler_EGS_SKILL_SLOT_CHANGE_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( NULL == g_pData->GetMyUser() ) 
		return false;

	if( NULL == g_pData->GetMyUser()->GetSelectUnit() )
		return false;


	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_SKILL_SLOT_CHANGE_ITEM_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	if( CX2UserSkillTree::SSBES_PERMANENT != g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetSkillSlotBExpirationState() )
	{
		if ( kEvent.m_cSkillSlotBExpirationState == CX2UserSkillTree::SSBES_EXPIRED )
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlotBExpirationState( CX2UserSkillTree::SSBES_EXPIRED );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlotBEndDateString( kEvent.m_wstrSkillSlotBEndDate );

			//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
#ifdef ARA_CHARACTER_BASE
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 0, CX2SkillTree::SI_NONE, 0.f, 0, 0, true );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 1, CX2SkillTree::SI_NONE, 0.f, 0, 0, true );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 2, CX2SkillTree::SI_NONE, 0.f, 0, 0, true );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 3, CX2SkillTree::SI_NONE, 0.f, 0, 0, true );
#else
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 0, CX2SkillTree::SI_NONE, 0.f, 0, true );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 1, CX2SkillTree::SI_NONE, 0.f, 0, true );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 2, CX2SkillTree::SI_NONE, 0.f, 0, true );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 3, CX2SkillTree::SI_NONE, 0.f, 0, true );
#endif
#else	NEW_CHARACTER_CHUNG
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 0, CX2SkillTree::SI_NONE, 0.f, true );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 1, CX2SkillTree::SI_NONE, 0.f, true );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 2, CX2SkillTree::SI_NONE, 0.f, true );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 3, CX2SkillTree::SI_NONE, 0.f, true );
#endif	NEW_CHARACTER_CHUNG
			//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

			if( g_pData->GetUIManager() != NULL &&
				g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL))
			{
				g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, false, true);
			}

			switch( g_pMain->GetNowStateID() )
			{
			default:
				{
					g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_2686 ), (CKTDXStage*) g_pMain->GetNowState() );
				} break;

			case CX2Main::XS_BATTLE_FIELD:
			case CX2Main::XS_DUNGEON_GAME:
			case CX2Main::XS_DUNGEON_RESULT:
			case CX2Main::XS_PVP_GAME:
			case CX2Main::XS_PVP_RESULT:
				{
					wstring wstrSystemMessageColor = L"#CFF3F4D";								// 255, 63, 77
					D3DXCOLOR coSystemMessageTextColor(1.f, 0.24705f, 0.30196f, 1.f);			// 255, 63, 77

					g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2686 ), coSystemMessageTextColor, D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
					g_pChatBox->AddChatLog( GET_STRING( STR_ID_2686 ), KEGS_CHAT_REQ::CPT_SYSTEM, coSystemMessageTextColor, wstrSystemMessageColor );
				} break;
			}
		}
		else if ( kEvent.m_cSkillSlotBExpirationState == CX2UserSkillTree::SSBES_NOT_EXPIRED )
		{
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlotBExpirationState( CX2UserSkillTree::SSBES_NOT_EXPIRED );
			g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlotBEndDateString( kEvent.m_wstrSkillSlotBEndDate );

			if( NULL != g_pData->GetUIManager() &&
				true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL) )
			{
				g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, false, false);
			}
		}
	}
#ifdef REFORM_UI_SKILLSLOT
	if( NULL != g_pData && NULL != g_pData->GetGuildManager() && NULL != g_pData->GetUIManager()->GetUISkillTree())
	{
		g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, false, false);
	}
	
	if( NULL != g_pX2Game )
	{
		if ( NULL != g_pX2Game->GetMyUnit() && g_pX2Game->GetMyUnit()->IsMyUnit() )
		{
			g_pX2Game->UpdateSkillSlotUI( true);
			g_pX2Game->InitExpandSkillCoolTime();
		}
	}
#endif //REFORM_UI_SKILLSLOT

#ifdef SKILL_SLOT_UI_TYPE_B
	if( NULL != g_pMain)
	{
		g_pMain->GetGameOption().SetIsSkillUITypeA( g_pMain->GetGameOption().GetIsSkillUITypeA() );				
	}
#endif //SKILL_SLOT_UI_TYPE_B

	// ������ // 2013-08-23 // ��Ÿ�� ���� ���� �ʱ�ȭ ���� �߰�
	CX2GageManager::GetInstance()->InitMySkillCoolTimeList( g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree );

	return true;
}
#endif SERV_SKILL_SLOT_CHANGE_INVENTORY





bool CX2SkillTree::Handler_EGS_UNSEAL_SKILL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UNSEAL_SKILL_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.AddSkillUnsealed( (CX2SkillTree::SKILL_ID) kEvent.m_iSkillID );


	if( NULL != g_pData->GetUIManager() &&
		NULL != g_pData->GetUIManager()->GetUISkillTree() )
	{
		g_pData->GetUIManager()->GetUISkillTree()->UpdateSkillTree( (CX2SkillTree::SKILL_ID) kEvent.m_iSkillID );
		
		g_pData->GetUIManager()->GetUISkillTree()->OpenDLGSkillUnsealed( true );
		g_pData->GetUIManager()->GetUISkillTree()->UpdateDLGSkillUnsealed( (CX2SkillTree::SKILL_ID) kEvent.m_iSkillID );
	}


	if( NULL != g_pData->GetQuestManager() )
	{
		g_pData->GetQuestManager()->GiveUpForbiddenQuest();
	}

	return true;
}


void CX2SkillTree::On_EGS_EXPIRE_CASH_SKILL_POINT_NOT( KEGS_EXPIRE_CASH_SKILL_POINT_NOT kEvent )
{
	if( kEvent.m_iUnitUID <= 0 )
		return;

	if( NULL == g_pData->GetMyUser() ||
		NULL == g_pData->GetMyUser()->GetSelectUnit() )
		return;

	CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();


	if( pMyUnit->GetUID() == kEvent.m_iUnitUID )
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		/// ��ų ���� ���� �ʱ�ȭ �ϰ�, ��Ŷ���� �� ���ڷ� ���� �� ��ų ���� �� ���� 0�� ��ų ����
		if ( NULL != pMyUnit )
  	#ifdef SKILL_PAGE_SYSTEM //JHKang
		{
			int iUsingPage = pMyUnit->AccessUnitData().m_UserSkillTree.GetUsingPage();

			for ( int i = 0; i < pMyUnit->AccessUnitData().m_UserSkillTree.GetOpenedPage(); ++i )
			{
				pMyUnit->AccessUnitData().m_UserSkillTree.SetUsingPage( i + 1 );
				pMyUnit->AccessUnitData().m_UserSkillTree.ExpireCashSkillPoint( kEvent.m_vecMapHaveSKill[i] );
				int iSP = kEvent.m_vecRetrievedSPoint[i];
				pMyUnit->AccessUnitData().m_UserSkillTree.AddSkillPoint( iSP );
				pMyUnit->AccessUnitData().m_UserSkillTree.SetCashSkillPoint( 0 );

				if( NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUISkillTree() )
					g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, true, true);
			}
			pMyUnit->AccessUnitData().m_UserSkillTree.SetUsingPage( iUsingPage + 1 );
			pMyUnit->AccessUnitData().m_iSPoint = pMyUnit->AccessUnitData().m_UserSkillTree.GetSkillPoint( iUsingPage + 1 );
			pMyUnit->AccessUnitData().m_iCSPoint = pMyUnit->AccessUnitData().m_UserSkillTree.GetCashSkillPoint( iUsingPage + 1 );

			if( NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUISkillTree() )
				g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, true, true);
		}

	#else //SKILL_PAGE_SYSTEM
			pMyUnit->AccessUnitData().m_UserSkillTree.ExpireCashSkillPoint( kEvent.m_mapHaveSKill );
			  
		pMyUnit->AccessUnitData().m_iSPoint += kEvent.m_iRetrievedSPoint;
		pMyUnit->AccessUnitData().m_iCSPoint = 0;

		if( NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUISkillTree() )
			g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, true, true);
	#endif //SKILL_PAGE_SYSTEM

#else // UPGRADE_SKILL_SYSTEM_2013
		pMyUnit->AccessUnitData().m_UserSkillTree.ExpireCashSkillPoint();

		pMyUnit->AccessUnitData().m_iSPoint += kEvent.m_iRetrievedSPoint;
		pMyUnit->AccessUnitData().m_iCSPoint = 0;

		if( NULL != g_pData->GetUIManager() &&
			true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL) )
		{
			g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, true, true);
		}
#endif // UPGRADE_SKILL_SYSTEM_2013
	}

}








#ifdef GUILD_SKILL

bool CX2SkillTree::Handler_EGS_GET_GUILD_SKILL_REQ( CX2SkillTree::SKILL_ID eSkillID )
{
	// ���� ��� ��ų �۾� �Ϸ�ñ��� �ּ�
	
	//{{ oasis907 : ����� [2009.11.19] // ��帶���� �ƴ� ����� ��� ��ų ���� Ŭ���̾�Ʈ ����
	if(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade != CX2GuildManager::GUG_MASTER)
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(300, 250), GET_STRING( STR_ID_4801 ), g_pMain->GetNowState() );
		return true;
	}
	//}} oasis907 : ����� [2009.11.19] //


	#ifdef DISABLE_REDUDANT_PACKET_TEST
	if( true == g_pMain->IsWaitingServerPacket( EGS_GET_GUILD_SKILL_ACK ) )
	{
	return false;
	}
	#endif DISABLE_REDUDANT_PACKET_TEST


	if ( g_pData->GetMyUser() == NULL )
	return false;

	if ( g_pData->GetMyUser()->GetSelectUnit() == NULL )
	return false;


	KEGS_GET_GUILD_SKILL_REQ kPacket;
	kPacket.m_iGuildSkillID = (int) eSkillID;

	g_pData->GetServerProtocol()->SendPacket( EGS_GET_GUILD_SKILL_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_GET_GUILD_SKILL_ACK );

	return true;
}




bool CX2SkillTree::Handler_EGS_GET_GUILD_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// oasis907 : ����� [2009.11.19] // ACK ���� �Ϸ�

	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_GUILD_SKILL_ACK kEvent;
	DeSerialize( pBuff, &kEvent );	


	g_pMain->DeleteServerPacket( EGS_GET_GUILD_SKILL_ACK );
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			return true;
		}
		return true;
	}

	return false;
	
}




bool CX2SkillTree::Handler_EGS_GET_GUILD_SKILL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// oasis907 : ����� [2009.11.19] // NOT ���� �Ϸ�


	// ��Ŷ ó��
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_GET_GUILD_SKILL_NOT kEvent;
	DeSerialize( pBuff, &kEvent );	



	// oasis907 : ����� [2009.11.19] // ���� �ȿ����� �ǽð� ��ų ���� �ݿ�, ������ �����ε� ����
	// ���� �� ������ ��� ó��
	CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();
	{
	#ifdef SKILL_PAGE_SYSTEM //JHKang
		pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( (CX2SkillTree::SKILL_ID) kEvent.m_iGuildSkillID, kEvent.m_iGuildSkillLv,
			kEvent.m_iGuildSkillCSPoint, pUnitData->m_UserSkillTree.GetUsingPage() );
	#else //SKILL_PAGE_SYSTEM
		pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( (CX2SkillTree::SKILL_ID) kEvent.m_iGuildSkillID, kEvent.m_iGuildSkillLv, kEvent.m_iGuildSkillCSPoint );
	#endif //SKILL_PAGE_SYSTEM
		pUnitData->m_iGuildSPoint	= kEvent.m_iGuildSPoint;
		pUnitData->m_iGuildCSPoint	= kEvent.m_iGuildCSPoint;
	}

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetSkillTemplet( (SKILL_ID)kEvent.m_iGuildSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	const CX2SkillTree::SkillTemplet* pSkillTemplet = GetSkillTemplet( (SKILL_ID)kEvent.m_iGuildSkillID, kEvent.m_iGuildSkillLv );
#endif // UPGRADE_SKILL_SYSTEM_2013
	switch( pSkillTemplet->m_eType )
	{
	case CX2SkillTree::ST_ACTIVE:
	case CX2SkillTree::ST_BUFF:
	case CX2SkillTree::ST_SPECIAL_ACTIVE:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2SkillTree::ST_RELATIONSHIP_SKILL:
#endif // ADDED_RELATIONSHIP_SYSTEM
#ifdef FINALITY_SKILL_SYSTEM //JHKang
	case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
#endif //FINALITY_SKILL_SYSTEM
		{
			// oasis907 : ����� [2009.11.27] // ��� ��ų���� ��Ƽ�� ��ų �����Ƿ� �ּ�
			/*
			// ���� ������ ��ų�� ���� ����ٸ�
			if(kEvent.m_iSkillLevel == 1)
			{
				// �� ������ �˻��ؼ�
				int EmptySlot = pUnitData->m_UserSkillTree.GetSkillEquippedSlotIndex( CX2SkillTree::SI_NONE, false );
				if( EmptySlot != -1 )
				{
					// �������ش�
					Handler_EGS_CHANGE_SKILL_SLOT_REQ( EmptySlot, (CX2SkillTree::SKILL_ID)kEvent.m_iSkillID );
				}
				else if( pUnitData->m_UserSkillTree.GetEnabledSkillSlotB() )
				{
					EmptySlot = pUnitData->m_UserSkillTree.GetSkillEquippedSlotIndex( CX2SkillTree::SI_NONE, true );
					if( EmptySlot != -1 )
					{
						Handler_EGS_CHANGE_SKILL_SLOT_REQ( EmptySlot+EQUIPPED_SKILL_SLOT_COUNT, (CX2SkillTree::SKILL_ID)kEvent.m_iSkillID );
					}
				}
			}
			*/

		} break;
	case CX2SkillTree::ST_PASSIVE_PHYSIC_ATTACK:
	case CX2SkillTree::ST_PASSIVE_MAGIC_ATTACK:
	case CX2SkillTree::ST_PASSIVE_MAGIC_DEFENCE:
	case CX2SkillTree::ST_PASSIVE_PHYSIC_DEFENCE:
	case CX2SkillTree::ST_PASSIVE:
	case CX2SkillTree::ST_GUILD_PASSIVE_PHYSIC_ATTACK_MAGIC_DEFENCE:
	case CX2SkillTree::ST_GUILD_PASSIVE_MAGIC_ATTCK_PHYSIC_DEFENCE:
		{
			if( g_pData != NULL && 
				g_pData->GetUIManager() != NULL && 
				g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
			{
				g_pData->GetUIManager()->GetUICharInfo()->ResetStat();
			}
		} break;
	}

	if( NULL != g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree() )
	{
		g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->UpdateUI(true, true, true); // oasis
	}


	// ����
	g_pKTDXApp->GetDeviceManager()->PlaySound( L"SkillTree_SkillUp.ogg", false, false );

	if(pUnitData->m_UserSkillTree.IsMaxSkillLevel( (SKILL_ID) kEvent.m_iGuildSkillID ))
	{
		g_pKTDXApp->GetDeviceManager()->PlaySound( L"SkillTree_SkillMaster.ogg", false, false );
		//const SkillTreeTempletMap& refSkillTreeTempletMap = GetSkillTreeTempletMap( (int)pUnitData->m_UnitClass );

		const SkillTreeTempletMap& refSkillTreeTempletMap = GetGuildSkillTreeTempletMap( (int) 0 ); // oasis
		SkillTreeTempletMap::const_iterator it = refSkillTreeTempletMap.find((SKILL_ID) kEvent.m_iGuildSkillID);
		if( it != refSkillTreeTempletMap.end() )
		{
			const CX2SkillTree::SkillTreeTemplet& refSkillTreeTemplet = it->second;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
			if(0 != refSkillTreeTemplet.m_iFollowingSkill )
			{
				// ���� ��ų�� ������ (ȭ��ǥ�� ���� ���̴�)
				g_pKTDXApp->GetDeviceManager()->PlaySound( L"SkillTree_SkillRootOpen.ogg", false, false );
			}
#else // UPGRADE_SKILL_SYSTEM_2013
			if(false == refSkillTreeTemplet.m_vecFollowingSkill.empty())
			{
				// ���� ��ų�� ������ (ȭ��ǥ�� ���� ���̴�)
				g_pKTDXApp->GetDeviceManager()->PlaySound( L"SkillTree_SkillRootOpen.ogg", false, false );
			}
#endif // UPGRADE_SKILL_SYSTEM_2013
		}
	}

	//	switch( pSkillTemplet->m_Type )
	//	{
	//	case CX2SkillTree::ST_PASSIVE_PHYSIC_ATTACK:
	//	case CX2SkillTree::ST_PASSIVE_MAGIC_ATTACK:
	//	case CX2SkillTree::ST_PASSIVE_MAGIC_DEFENCE:
	//	case CX2SkillTree::ST_PASSIVE_PHYSIC_DEFENCE:
	//		{
	//			if( g_pData != NULL && 
	//				g_pData->GetUIManager() != NULL && 
	//				g_pData->GetUIManager()->GetUICharInfo() != NULL)
	//			{
	//				g_pData->GetUIManager()->GetUICharInfo()->ResetStat();
	//			}
	//		} break;
	//	}
	//}


	//// ��ų ������ ����ų� ������ ��ų���� ���� ������ ���� ��ų�̸� �ٷ� ����
	//EquipSkillWhenGetSkill( GetGetSkillID() );

	// oasis907 : ����� [2009.12.3] // ������ ���� ��� ��ų ����Ʈ�� ����߽��ϴ�.
	g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4795, "L", g_pData->GetGuildManager()->GetMyGuildInfo()->m_wstrGuildMasterNickName)), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
	
	return true;

}







// oasis907 : ����� [2009.11.27] //
bool CX2SkillTree::Handler_EGS_RESET_GUILD_SKILL_REQ( CX2SkillTree::SKILL_ID eSkillID )
{

#ifdef DISABLE_REDUDANT_PACKET_TEST
	if( true == g_pMain->IsWaitingServerPacket( EGS_RESET_GUILD_SKILL_ACK ) )
	{
		return false;
	}
#endif DISABLE_REDUDANT_PACKET_TEST


	if(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade != CX2GuildManager::GUG_MASTER)
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(300, 250), GET_STRING( STR_ID_4801 ), g_pMain->GetNowState() );
		return true;
	}


	KEGS_RESET_GUILD_SKILL_REQ kPacket;
	kPacket.m_iGuildSkillID = (int) eSkillID;	

	g_pData->GetServerProtocol()->SendPacket( EGS_RESET_GUILD_SKILL_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_RESET_GUILD_SKILL_ACK );

	return true;
}

// oasis907 : ����� [2009.11.27] //
bool CX2SkillTree::Handler_EGS_RESET_GUILD_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RESET_GUILD_SKILL_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_RESET_GUILD_SKILL_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			// ��ų �ʱ�ȭ �������� ����߱� ������ �κ� ������ �����Ѵ�
			if( kEvent.m_vecInventorySlotInfo.size() > 0 )
			{
				if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
				{
					g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
				}
				// UI��..
				if( NULL != g_pData->GetUIManager() &&
					true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_INVEN))
				{
					g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
				}
			}
			// ����
			g_pKTDXApp->GetDeviceManager()->PlaySound( L"SkillTree_SkillDown.ogg", false, false );

		}
		return true;
	}
	return false;
}


// oasis907 : ����� [2009.11.27] //
bool CX2SkillTree::Handler_EGS_RESET_GUILD_SKILL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_RESET_GUILD_SKILL_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

	// ��ų Ʈ�� ����
	int iSkillLevel = 0;
	int iSkillCSPoint = 0;
	pUnitData->m_UserSkillTree.GetSkillLevelAndCSP( (CX2SkillTree::SKILL_ID) kEvent.m_iDelGuildSkillID, iSkillLevel, iSkillCSPoint );
	iSkillLevel -= 1;
	if( iSkillCSPoint > 0 )
	{
		iSkillCSPoint -= kEvent.m_iRestoreGuildCSPoint;
	}
#ifdef SKILL_PAGE_SYSTEM //JHKang
	pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( (CX2SkillTree::SKILL_ID) kEvent.m_iDelGuildSkillID, iSkillLevel, iSkillCSPoint,
		pUnitData->m_UserSkillTree.GetUsingPage() ); 
#else //SKILL_PAGE_SYSTEM
	pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( (CX2SkillTree::SKILL_ID) kEvent.m_iDelGuildSkillID, iSkillLevel, iSkillCSPoint ); 
#endif //SKILL_PAGE_SYSTEM

	pUnitData->m_iGuildSPoint	+= kEvent.m_iRestoreGuildSPoint;
	pUnitData->m_iGuildCSPoint	+= kEvent.m_iRestoreGuildCSPoint;


	// UI ���� �� ���� ��ų ����
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( (CX2SkillTree::SKILL_ID) kEvent.m_iDelGuildSkillID );
#else // UPGRADE_SKILL_SYSTEM_2013
	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( (CX2SkillTree::SKILL_ID) kEvent.m_iDelGuildSkillID, 1 );
#endif // UPGRADE_SKILL_SYSTEM_2013
	switch( pSkillTemplet->m_eType )
	{
	case CX2SkillTree::ST_PASSIVE_PHYSIC_ATTACK:
	case CX2SkillTree::ST_PASSIVE_MAGIC_ATTACK:
	case CX2SkillTree::ST_PASSIVE_MAGIC_DEFENCE:
	case CX2SkillTree::ST_PASSIVE_PHYSIC_DEFENCE:
	case CX2SkillTree::ST_PASSIVE:
	case CX2SkillTree::ST_GUILD_PASSIVE_PHYSIC_ATTACK_MAGIC_DEFENCE:
	case CX2SkillTree::ST_GUILD_PASSIVE_MAGIC_ATTCK_PHYSIC_DEFENCE:
		{
			if( g_pData != NULL && 
				g_pData->GetUIManager() != NULL && 
				g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
			{
				g_pData->GetUIManager()->GetUICharInfo()->ResetStat();
			}
		} break;

	case CX2SkillTree::ST_BUFF:
	case CX2SkillTree::ST_ACTIVE:
	case CX2SkillTree::ST_SPECIAL_ACTIVE:
#ifdef ADDED_RELATIONSHIP_SYSTEM
	case CX2SkillTree::ST_RELATIONSHIP_SKILL:
#endif // ADDED_RELATIONSHIP_SYSTEM
#ifdef FINALITY_SKILL_SYSTEM //JHKang
	case CX2SkillTree::ST_HYPER_ACTIVE_SKILL:
#endif //FINALITY_SKILL_SYSTEM
		{
			// oasis907 : ����� [2009.11.27] // ��� ��ų���� ��Ƽ�� ��ų �����Ƿ� �ּ�
			/*
			for( int i=0; i<EQUIPPED_SKILL_SLOT_COUNT; i++ )
			{
				CX2UserSkillTree::SkillSlotData* pSkillSlotData = pUnitData->m_UserSkillTree.GetSkillSlot( i, false );
				if( NULL != pSkillSlotData )
				{
					if( pSkillSlotData->m_eID == (CX2SkillTree::SKILL_ID) kEvent.m_iDelSkillID )
					{
						pUnitData->m_UserSkillTree.SetSkillSlot( i, (CX2SkillTree::SKILL_ID)kEvent.m_iDelSkillID, 0, false );
					}
				}


				pSkillSlotData = pUnitData->m_UserSkillTree.GetSkillSlot( i, true );
				if( NULL != pSkillSlotData )
				{
					if( pSkillSlotData->m_eID == (CX2SkillTree::SKILL_ID) kEvent.m_iDelSkillID )
					{
						pUnitData->m_UserSkillTree.SetSkillSlot( i, (CX2SkillTree::SKILL_ID)kEvent.m_iDelSkillID, 0, true );
					}
				}
			}
			*/
		} break;
	}

	if( NULL != g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree() )
	{
		g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->UpdateUI(true, true, true); // oasis
	}

	// ����
	g_pKTDXApp->GetDeviceManager()->PlaySound( L"SkillTree_SkillDown.ogg", false, false );

	int iRestoreGuildSPoint = kEvent.m_iRestoreGuildSPoint + kEvent.m_iRestoreGuildCSPoint;


	g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4810, "L", g_pData->GetGuildManager()->GetMyGuildInfo()->m_wstrGuildMasterNickName)), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
	// oasis907 : ����� [2009.12.3] // ��� ��ų����Ʈ�� 1 �����Ǿ����ϴ�.
	g_pChatBox->AddChatLog( GET_REPLACED_STRING( ( STR_ID_4797, "i", iRestoreGuildSPoint)), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );

	return true;
}








// oasis907 : ����� [2009.11.27] //
bool CX2SkillTree::Handler_EGS_INIT_GUILD_SKILL_TREE_REQ( UidType itemUID )
{
	if(g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_byMemberShipGrade != CX2GuildManager::GUG_MASTER)
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(300, 250), GET_STRING( STR_ID_4801 ), g_pMain->GetNowState() );
		return true;
	}

	KEGS_INIT_GUILD_SKILL_REQ kPacket;
	kPacket.m_iItemUID = itemUID;

	g_pData->GetServerProtocol()->SendPacket( EGS_INIT_GUILD_SKILL_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_INIT_GUILD_SKILL_ACK );

	return true;
}


bool CX2SkillTree::Handler_EGS_INIT_GUILD_SKILL_TREE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_INIT_GUILD_SKILL_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	if( g_pMain->DeleteServerPacket( EGS_INIT_GUILD_SKILL_ACK ) == true )
	{
		if( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			//CX2Unit::UnitData* pUnitData = g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();


			if( kEvent.m_vecInventorySlotInfo.size() > 0 )
			{
				if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
				{
					g_pData->GetMyUser()->GetSelectUnit()->AccessInventory().UpdateInventorySlotList( kEvent.m_vecInventorySlotInfo );
				}
				// UI��..
				if( NULL != g_pData->GetUIManager() && 
					NULL != g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_INVEN) )
				{
					g_pData->GetUIManager()->GetUIInventory()->UpdateInventorySlotList(kEvent.m_vecInventorySlotInfo);

				}
			}
	
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(300, 250), GET_STRING( STR_ID_4798 ), g_pMain->GetNowState() );
			return true;

		}
		else
		{
			//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), L"��ų �ʱ�ȭ�� �����Ͽ����ϴ�.", m_pNowStage );
		}
		return true;
	}
	return false;
}



bool CX2SkillTree::Handler_EGS_INIT_GUILD_SKILL_TREE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_INIT_GUILD_SKILL_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

	pUnitData->m_iGuildSPoint = kEvent.m_iGuildSPoint;
	pUnitData->m_iGuildCSPoint = kEvent.m_iGuildCSPoint;

	// ��ųƮ���� ������ų �ʱ�ȭ
	pUnitData->m_UserSkillTree.Reset( false, true, true, false, false ); // Ŀ���ؾ���


	if( NULL != g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree() )
	{
		g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->UpdateUI(true, true, true); // oasis
	}

	if( NULL != g_pData->GetUIManager() )
	{
		if( g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO) )
		{
			g_pData->GetUIManager()->GetUICharInfo()->ResetStat();
		}
	}

	//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), L"��� ��ų�� �ʱ�ȭ �Ͽ����ϴ�.", m_pNowStage );

	// oasis907 : ����� [2009.12.3] // ��� ��ų�� �ʱ�ȭ �Ǿ����ϴ�.
	g_pChatBox->AddChatLog( GET_STRING(STR_ID_4798), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );

	return false;
}





bool CX2SkillTree::Handler_EGS_EXPIRE_GUILD_CASH_SKILL_POINT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EXPIRE_GUILD_CASH_SKILL_POINT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );	


	CX2Unit* pMyUnit = g_pData->GetMyUser()->GetSelectUnit();

	// oasis907 : ����� [2009.12.4] // ��� ���Խ�
	CX2Unit* pUnit = g_pData->GetMyUser()->GetSelectUnit();
	pUnit->AccessUnitData().m_UserSkillTree.SetGuildClass(0);
	pUnit->AccessUnitData().m_UserSkillTree.SetAcquiredGuildSkill( kEvent.m_vecGuildSkillData ); 

	// oasis907 : ����� [2009.11.27] // ��� ��ų ����Ʈ, ĳ�� ��ų ����Ʈ SET
	pUnit->AccessUnitData().m_iGuildSPoint = kEvent.m_iGuildSPoint;
	pUnit->AccessUnitData().m_iGuildCSPoint = kEvent.m_iGuildCSPoint;
	pUnit->AccessUnitData().m_iMaxGuildCSPoint = 0;
	pUnit->AccessUnitData().m_wstrGuildCSPointEndDate = L"";


	//{{ oasis907 : ����� [2009.12.8] // ��� ��ų�� ���� ���� ���� 
	if( g_pData != NULL && 
		g_pData->GetUIManager() != NULL && 
		g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
	{
		g_pData->GetUIManager()->GetUICharInfo()->ResetStat();
	}

	// ��� ��ų Ʈ��â ������Ʈ 
	if( NULL != g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree() )
	{
		g_pData->GetGuildManager()->GetUIGuild()->GetUIGuildSkillTree()->UpdateUI(true, true, true); // oasis
	}
	return true;
}


/*

bool CX2SkillTree::Handler_EGS_EXPAND_GUILD_SKILL_SLOT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_EXPAND_SKILL_SLOT_NOT kEvent;
	DeSerialize( pBuff, &kEvent );


	if( NULL == g_pData->GetMyUser() ||
		NULL == g_pData->GetMyUser()->GetSelectUnit() )
		return false;

	if( g_pData->GetMyUser()->GetSelectUnit()->GetUID() != kEvent.m_iUnitUID )
		return false;

	g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlotBExpirationState( (CX2UserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE) kEvent.m_cSkillSlotBExpirationState );
	g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlotBEndDateString( kEvent.m_wstrSkillSlotBEndDate );


	// ���� 
	if( NULL != g_pData->GetUIManager() &&
		true == g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL) )
	{
		g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, false, false);
	}

	return true;
}


bool CX2SkillTree::Handler_EGS_EXPIRE_GUILD_SKILL_SLOT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( NULL != g_pData->GetMyUser() ) 
		return false;

	if( NULL != g_pData->GetMyUser()->GetSelectUnit() )
		return false;


	if( CX2UserSkillTree::SSBES_PERMANENT != g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree.GetSkillSlotBExpirationState() )
	{
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlotBExpirationState( CX2UserSkillTree::SSBES_EXPIRED );

		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 0, CX2SkillTree::SI_NONE, 0.f, true );
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 1, CX2SkillTree::SI_NONE, 0.f, true );
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 2, CX2SkillTree::SI_NONE, 0.f, true );
		g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.SetSkillSlot( 3, CX2SkillTree::SI_NONE, 0.f, true );

		if( g_pData->GetUIManager() != NULL &&
			g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_SKILL))
		{
			g_pData->GetUIManager()->GetUISkillTree()->UpdateUI(true, false, true);
		}

		switch( g_pMain->GetNowStateID() )
		{
		default:
			{
				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( 250, 300 ), GET_STRING( STR_ID_2686 ), (CKTDXStage*) g_pMain->GetNowState() );
			} break;

		case CX2Main::XS_DUNGEON_GAME:
		case CX2Main::XS_DUNGEON_RESULT:
		case CX2Main::XS_PVP_GAME:
		case CX2Main::XS_PVP_RESULT:
			{
				wstring wstrSystemMessageColor = L"#CFF3F4D";								// 255, 63, 77
				D3DXCOLOR coSystemMessageTextColor(1.f, 0.24705f, 0.30196f, 1.f);			// 255, 63, 77

				g_pX2Game->GetInfoTextManager().PushText( XUF_DODUM_20_BOLD, GET_STRING( STR_ID_2686 ), coSystemMessageTextColor, D3DXCOLOR(0,0,0,1), DT_CENTER, 1.f, 1.f );
				g_pChatBox->AddChatLog( GET_STRING( STR_ID_2686 ), KEGS_CHAT_REQ::CPT_SYSTEM, coSystemMessageTextColor, wstrSystemMessageColor );
			} break;
		}
	}

	return true;
}






bool CX2SkillTree::Handler_EGS_UNSEAL_GUILD_SKILL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = (KSerBuffer*)lParam;
	KEGS_UNSEAL_SKILL_NOT kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_UserSkillTree.AddSkillUnsealed( (CX2SkillTree::SKILL_ID) kEvent.m_iSkillID );


	if( NULL != g_pData->GetUIManager() &&
		NULL != g_pData->GetUIManager()->GetUISkillTree() )
	{
		g_pData->GetUIManager()->GetUISkillTree()->UpdateSkillTree( (CX2SkillTree::SKILL_ID) kEvent.m_iSkillID );

		g_pData->GetUIManager()->GetUISkillTree()->OpenDLGSkillUnsealed( true );
		g_pData->GetUIManager()->GetUISkillTree()->UpdateDLGSkillUnsealed( (CX2SkillTree::SKILL_ID) kEvent.m_iSkillID );
	}


	if( NULL != g_pData->GetQuestManager() )
	{
		g_pData->GetQuestManager()->GiveUpForbiddenQuest();
	}

	return true;
}
*/

//}} oasis907 : ����� //// 2009-11-12 //// 
#endif GUILD_SKILL

#pragma region Skill Page System
#ifdef SKILL_PAGE_SYSTEM //JHKang
bool CX2SkillTree::Handler_EGS_GET_NEXT_SKILL_PAGE_ED_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>( lParam );
	KEGS_GET_NEXT_SKILL_PAGE_ED_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->DeleteServerPacket( EGS_GET_NEXT_SKILL_PAGE_ED_ACK );
	{
		if (g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( g_pData && g_pData->GetUIManager() && g_pData->GetUIManager()->GetUISkillTree() )
			{
				g_pData->GetUIManager()->GetUISkillTree()->BuySkillPageDialog( kEvent.m_iED );

				return true;
			}
		}
	}

	return false;
}

bool CX2SkillTree::Handler_EGS_EXPAND_SKILL_PAGE_REQ()
{
	if( true == g_pMain->IsWaitingServerPacket( EGS_EXPAND_SKILL_PAGE_ACK ) )
		return false;

	if ( g_pData && g_pData->GetMyUser() && g_pData->GetMyUser()->GetSelectUnit() 
		 && g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData().m_ED >= 1000 )
	{
		g_pData->GetServerProtocol()->SendID( EGS_EXPAND_SKILL_PAGE_REQ );
		g_pMain->AddServerPacket( EGS_EXPAND_SKILL_PAGE_ACK );

		return true;
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_28961 ), g_pMain->GetNowState() );

		return false;
	}
}

bool CX2SkillTree::Handler_EGS_EXPAND_SKILL_PAGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>( lParam );
	KEGS_EXPAND_SKILL_PAGE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->DeleteServerPacket( EGS_EXPAND_SKILL_PAGE_ACK );
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( NULL == g_pData ||
				 NULL == g_pData->GetMyUser() ||
				 NULL == g_pData->GetMyUser()->GetSelectUnit() ||
				 NULL == g_pData->GetUIManager() ||
				 NULL == g_pData->GetUIManager()->GetUISkillTree() )
			{
				return false;
			}

			CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

			if ( pUnitData == NULL )
				return false;

			int usDiffValue = kEvent.m_iTheNumberOfSkillPagesAvailable - pUnitData->m_UserSkillTree.GetOpenedPage();
			
			if ( usDiffValue > 1 )
			{
				pUnitData->m_UserSkillTree.AddSkillPage();
				pUnitData->m_UserSkillTree.AddSkillPage();

				pUnitData->m_UserSkillTree.SetOpenedPage( static_cast<USHORT>( kEvent.m_iTheNumberOfSkillPagesAvailable ) );
				pUnitData->m_UserSkillTree.SetDefaultSkill( pUnitData->m_UserSkillTree.GetOpenedPage() - 1 );	// �� ������ �⺻ ��ų ����
				pUnitData->m_UserSkillTree.SetDefaultSkill( pUnitData->m_UserSkillTree.GetOpenedPage() - 2 );	// �� ������ �⺻ ��ų ����

				pUnitData->m_UserSkillTree.SetSkillPoint( kEvent.m_iSPointAvailable, pUnitData->m_UserSkillTree.GetOpenedPage() - 1 );
				pUnitData->m_UserSkillTree.SetCashSkillPoint( kEvent.m_iCSPointAvailable, pUnitData->m_UserSkillTree.GetOpenedPage() - 1 );

				pUnitData->m_UserSkillTree.SetSkillPoint( kEvent.m_iSPointAvailable, pUnitData->m_UserSkillTree.GetOpenedPage() - 2 );
				pUnitData->m_UserSkillTree.SetCashSkillPoint( kEvent.m_iCSPointAvailable, pUnitData->m_UserSkillTree.GetOpenedPage() - 2 );
			}
			else
			{
				pUnitData->m_UserSkillTree.AddSkillPage();
				pUnitData->m_UserSkillTree.SetOpenedPage( static_cast<USHORT>( kEvent.m_iTheNumberOfSkillPagesAvailable ) );
				pUnitData->m_UserSkillTree.SetDefaultSkill( pUnitData->m_UserSkillTree.GetOpenedPage() - 1 );	// �� ������ �⺻ ��ų ����

				pUnitData->m_UserSkillTree.SetSkillPoint( kEvent.m_iSPointAvailable, pUnitData->m_UserSkillTree.GetOpenedPage() - 1 );
				pUnitData->m_UserSkillTree.SetCashSkillPoint( kEvent.m_iCSPointAvailable, pUnitData->m_UserSkillTree.GetOpenedPage() - 1 );
			}

			if( NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUISkillTree() )
			{
				g_pData->GetUIManager()->GetUISkillTree()->UpdateUI( true, true, true, true );
			}

			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2( -999, -999 ), GET_STRING( STR_ID_29386 ), g_pMain->GetNowState() );

			pUnitData->m_ED = kEvent.m_iED;

			return true;
		}
	}

	return false;
}

bool CX2SkillTree::Handler_EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ( USHORT usPageNumber_ )
{
	if( true == g_pMain->IsWaitingServerPacket( EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK ) )
		return false;
	
	KEGS_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ kPacket;
	kPacket.m_iSkillPagesNumberDecidedToUse = usPageNumber_;

	g_pData->GetServerProtocol()->SendPacket( EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ, kPacket );
	g_pMain->AddServerPacket( EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK );

	return true;
}

bool CX2SkillTree::Handler_EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KSerBuffer* pBuff = reinterpret_cast<KSerBuffer*>( lParam );
	KEGS_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK kEvent;
	DeSerialize( pBuff, &kEvent );

	g_pMain->DeleteServerPacket( EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK );
	{
		if ( g_pMain->IsValidPacket( kEvent.m_iOK ) == true )
		{
			if ( NULL == g_pData ||
				NULL == g_pData->GetMyUser() ||
				NULL == g_pData->GetMyUser()->GetSelectUnit() ||
				NULL == g_pData->GetUIManager() ||
				NULL == g_pData->GetUIManager()->GetUISkillTree() )
			{
				return false;
			}

			CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->AccessUnitData();

			if ( pUnitData == NULL )
				return false;

			pUnitData->m_UserSkillTree.SetUsingPage( kEvent.m_iSkillPagesNumberDecidedToUse );
			pUnitData->m_UserSkillTree.SetSelectPage( kEvent.m_iSkillPagesNumberDecidedToUse );
			pUnitData->m_iSPoint = kEvent.m_iSPointAvailable;
			pUnitData->m_iCSPoint = kEvent.m_iCSPointAvailable;
			pUnitData->m_UserSkillTree.SetSkillPoint( pUnitData->m_iSPoint );
			pUnitData->m_UserSkillTree.SetCashSkillPoint( pUnitData->m_iCSPoint );

			BOOST_FOREACH( const KUserSkillData& userSkillData, kEvent.m_kUserSkillPageData.m_vecUserSkillData )
			{
				pUnitData->m_UserSkillTree.SetSkillLevelAndCSP( static_cast<CX2SkillTree::SKILL_ID>( userSkillData.m_iSkillID ), 
					userSkillData.m_cSkillLevel, userSkillData.m_cSkillCSPoint, pUnitData->m_UserSkillTree.GetUsingPage() );

				bool bEnableSkillEquipped = true;
				const CX2SkillTree::SKILL_ID eSkillId = static_cast<CX2SkillTree::SKILL_ID>( userSkillData.m_iSkillID );

				if ( eSkillId == CX2SkillTree::SI_NONE )
					continue;

				const CX2SkillTree::SkillTemplet* pSkillTemplet = GetSkillTemplet( eSkillId );

				switch( pSkillTemplet->m_eType )
				{
				case CX2SkillTree::ST_PASSIVE_PHYSIC_ATTACK:
				case CX2SkillTree::ST_PASSIVE_MAGIC_ATTACK:
				case CX2SkillTree::ST_PASSIVE_MAGIC_DEFENCE:
				case CX2SkillTree::ST_PASSIVE_PHYSIC_DEFENCE:
				case CX2SkillTree::ST_PASSIVE:
					{
						if( g_pData != NULL && 
							g_pData->GetUIManager() != NULL && 
							g_pData->GetUIManager()->GetShow(CX2UIManager::UI_MENU_CHARINFO))
						{
							g_pData->GetUIManager()->GetUICharInfo()->ResetStat();
						}
					} break;
				}
			}

			pUnitData->m_UserSkillTree.SetEquippedSkill( kEvent.m_kUserSkillPageData.m_aEquippedSkill, 
				kEvent.m_kUserSkillPageData.m_aEquippedSkillSlotB );

			if ( NULL != g_pTFieldGame && NULL != g_pData->GetMyUser() )
			{
				for ( int i = 0; i < EQUIPPED_SKILL_SLOT_COUNT; ++i )
				{
					g_pData->GetMyUser()->ChangeEquippedSkillState( i, kEvent.m_kUserSkillPageData.m_aEquippedSkill[i].m_iSkillID );
					g_pData->GetMyUser()->ChangeEquippedSkillState( i, kEvent.m_kUserSkillPageData.m_aEquippedSkillSlotB[i].m_iSkillID );
				}
			}
			
			if ( true == g_pData->GetUIManager()->GetUISkillTree()->GetShow() )
				g_pData->GetUIManager()->GetUISkillTree()->UpdateUI( true, true, true );

			// ��ų �߰� ���� ȿ�� ����
			g_pData->GetMyUser()->GetSelectUnit()->ResetIncreaseSkillLevelByBuff();
			g_pData->GetMyUser()->GetSelectUnit()->ResetIncreaseSkillLevelBySocket();

			return true;
		}
	}

	return false;
}
#endif //SKILL_PAGE_SYSTEM
#pragma endregion ������ Ȯ��, ���� ��Ŷ ó��





// �¿� : ���ҽ� ������ �۾�
wstring CX2SkillTree::GetSkillLevelTextureName( int iLevel )
{
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	return L"DLG_UI_Common_Texture_NewSkill_01.tga";
#else // UPGRADE_SKILL_SYSTEM_2013
	return L"DLG_SKILL_Level_01.tga";
#endif // UPGRADE_SKILL_SYSTEM_2013

}
wstring CX2SkillTree::GetSkillLevelTextureKey( int iLevel )
{

	WCHAR wszText[16] = {0,};
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	StringCchPrintf( wszText, 16, L"LV.%d", iLevel );
#else // UPGRADE_SKILL_SYSTEM_2013
	StringCchPrintf( wszText, 16, L"LV%d", iLevel );
#endif // UPGRADE_SKILL_SYSTEM_2013
	//wsprintf( wszText, L"LV%d", iLevel );
	wstring retval = L"";
	retval += wszText;
	
	return retval;
	
}


//{{ 2009. 8. 5  ������		���� ��ų ������ ����
#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����

const CX2SkillTree::SKILL_ID CX2SkillTree::GetUnsealSkillItemInfo( const int iItemID ) const
{
	if ( NULL != g_pData &&
		 NULL != g_pData->GetMyUser() &&
		 NULL != g_pData->GetMyUser()->GetSelectUnit() &&
		 NULL != g_pData->GetSkillTree() )
	{
		/// ���� ��ų �����̳ʿ��� ��ų ������ �˻�
		map<int, SealSkillItemInfo>::const_iterator mit = m_mapSealSkillItemTemplet.find( iItemID );

		if( mit == m_mapSealSkillItemTemplet.end() )	
			return CX2SkillTree::SI_NONE;

		const int iUnitClass = static_cast<CX2SkillTree::SKILL_ID>( g_pData->GetMyUser()->GetSelectUnit()->GetClass() );	/// ���� Ŭ���� Ÿ��

		const std::vector<CX2SkillTree::SKILL_ID> vVal = mit->second.m_vecSkillID;		/// �ش� ���� �������� ���� ��ų �����̳�

		/// �ش� �ɸ����� ��ų�̶��, ���̵� ��ȯ
		BOOST_FOREACH( CX2SkillTree::SKILL_ID eSkillID, vVal )
		{
			/// ��ų ���̵� �ش� ĳ������ ��ų Ʈ�� ���ø��� �����ϴ��� �˻�
			const CX2SkillTree::SkillTreeTemplet* pSkillTreeTemplet = g_pData->GetSkillTree()->GetSkillTreeTemplet( iUnitClass, eSkillID );

			if ( NULL != pSkillTreeTemplet )
				return eSkillID;
		}
	}

	return CX2SkillTree::SI_NONE;
}

#else // UPGRADE_SKILL_SYSTEM_2013

bool CX2SkillTree::GetUnsealSkillItemInfo( int iItemID, int& iSkillID, char& cUnitClass )
{
	map< int, SealSkillItemInfo >::const_iterator mit = m_mapSealSkillItemTemplet.find( iItemID );
	if( mit == m_mapSealSkillItemTemplet.end() )	
		return false;

	iSkillID = mit->second.m_iSkillID;
	cUnitClass = mit->second.m_cUnitClass;

	return true;
}

#endif // UPGRADE_SKILL_SYSTEM_2013
//}}

#ifdef ADDED_RELATIONSHIP_SYSTEM
/** @function : UnequipRelationSkill
	@breif : �ο� ��ų ���� ����
*/
void CX2SkillTree::UnequipRelationSkill()
{
	// �ο� ��ų ���� ����
	if( NULL != g_pData && 
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		const CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();
		{
			// A���Կ��� �˻�
			int iSlotIndex = pUnitData->m_UserSkillTree.GetSkillEquippedSlotIndex( CX2SkillTree::SI_ETC_WS_COMMON_LOVE, false );

			// ���ٸ� B���Կ��� �˻�
			if( -1 == iSlotIndex )
				pUnitData->m_UserSkillTree.GetSkillEquippedSlotIndex( CX2SkillTree::SI_ETC_WS_COMMON_LOVE, true );

			// B���Կ��� ���ٸ� ������ ����
			if( -1 == iSlotIndex )
				return;

			Handler_EGS_CHANGE_SKILL_SLOT_REQ( iSlotIndex, CX2SkillTree::SI_NONE );
		}
	}
}
#endif ADDED_RELATIONSHIP_SYSTEM

#ifdef UPGRADE_SKILL_SYSTEM_2013
/** @function	: SetSkillAbilityMap
	@brief		: SKILL_ABILITY_TYPE�� �ش��ϴ� ���� �����ϴ� �Լ�
	@param		: �߰��� SKILL_ABILITY_TYPE, �߰��� SkillTemplet

*/
void CX2SkillTree::SetSkillAbilityMap( KLuaManager* pLuaManager, SKILL_ABILITY_TYPE eSkillAbility, CX2SkillTree::SkillTemplet* pSkillTemplet )
{
	if ( NULL == pLuaManager )
	{
		pLuaManager->EndTable();

		return;
	}

	vector<float>	vecSkillValue;			/// 30 ���� ������ ��ų ȿ�� ���� �����ϴ� ��ü
	int				iTableIndex		= 1;	/// ���̺� �ε���
	float			fValue			= 0.f;	/// �Ľ̵� ��

	while( pLuaManager->GetValue( iTableIndex, fValue ) == true )		/// 30���� ���ڰ� �Ľ�
	{
		vecSkillValue.push_back( fValue );
		++iTableIndex;
	}

	pLuaManager->EndTable();				/// ���̺� �ݱ�

	if ( true == vecSkillValue.empty() )	/// ���� ���� �ϳ��� ���� ���Ͽ��ٸ�, 0���� ���� ä����.
	{
		ASSERT( !"SKILL_ABILITY_TYPE Value is Empty!" );	/// ���� �����!
		vecSkillValue.push_back( 0.f );		/// 0���� ��� ä��� ����, ù��° ���� �߰�
	}

	if ( MAX_LIMITED_SKILL_LEVEL >= iTableIndex )	/// ���� ���ڼ��� �ִ� ���� ��ŭ ���ٸ�, ���� �ε����� ���ڷ� ä�� ������.
	{
		for( ; iTableIndex > MAX_LIMITED_SKILL_LEVEL; ++iTableIndex )
		{
			vecSkillValue.push_back( vecSkillValue[iTableIndex - 1] );
		}
	}

	/// ���ڰ� �߰�
	pSkillTemplet->m_mapSkillAbility.insert( std::make_pair( eSkillAbility, vecSkillValue ) );
}

/** @function	: SetSkillVlaue
	@brief		: ��ų ������ �ش��ϴ� ���� �����ϴ� �Լ�
	@param		: ������ �����̳�
*/
template< typename T >
#ifdef HIDE_DO_NOT_EXIST_SKILL_EFFECTIVE_DESC // ����ȯ
	void CX2SkillTree::SetSkillVlaue( KLuaManager& pLuaManager, vector<T>& vecValue, bool bAutoFill /*= true*/ )
#else //HIDE_DO_NOT_EXIST_SKILL_EFFECTIVE_DESC
	void CX2SkillTree::SetSkillVlaue( KLuaManager& pLuaManager, vector<T>& vecValue )
#endif //HIDE_DO_NOT_EXIST_SKILL_EFFECTIVE_DESC
{
	/*if ( NULL != pLuaManager )
	{
		pLuaManager.EndTable();

		return;
	}*/

	vecValue.clear();
	int				iTableIndex		= 1;	/// ���̺� �ε���
	T				fValue;					/// �Ľ̵� ��

	while( pLuaManager.GetValue( iTableIndex, fValue ) == true )		/// 30���� ���ڰ� �Ľ�
	{
		vecValue.push_back( fValue );
		++iTableIndex;
	}

	pLuaManager.EndTable();				/// ���̺� �ݱ�

	/// ���� ���ڼ��� �ִ� ���� ��ŭ ����, �ڵ� ä��⸦ ���� �ߴٸ� ���� �ε����� ���ڷ� ä�� ������.
#ifdef HIDE_DO_NOT_EXIST_SKILL_EFFECTIVE_DESC // ����ȯ
	if ( true != vecValue.empty() && MAX_LIMITED_SKILL_LEVEL >= iTableIndex && true == bAutoFill )
#else //HIDE_DO_NOT_EXIST_SKILL_EFFECTIVE_DESC
	if ( true != vecValue.empty() && MAX_LIMITED_SKILL_LEVEL >= iTableIndex )
#endif //HIDE_DO_NOT_EXIST_SKILL_EFFECTIVE_DESC
	{
		for( iTableIndex = 1; iTableIndex < MAX_LIMITED_SKILL_LEVEL; ++iTableIndex )
		{
			vecValue.push_back( vecValue[iTableIndex - 1] );
		}
	}
}

/** @function	: SetSkillStat
	@brief		: ��ų ���� ��ġ�� �����ϴ� �Լ�
	@param		: ������ �����̳�
*/
void CX2SkillTree::SetSkillStat( KLuaManager& pLuaManager, vector<CX2Stat::Stat>& vecStat )
{
	/// �� ���� ���ڵ�
	vector<float> vecBaseHP;
	vector<float> vecAtkPhysic;
	vector<float> vecAtkMagic;
	vector<float> vecDefPhysic;
	vector<float> vecDefMagic;

	vecStat.clear();

	/// ���� ��ġ �Ľ�
	if( pLuaManager.BeginTable( "m_Stat" ) == true )
	{
		if( true == pLuaManager.BeginTable( "m_BaseHP" ) )			SetSkillVlaue( pLuaManager, vecBaseHP );
		if( true == pLuaManager.BeginTable( "m_AtkPhysic" ) )		SetSkillVlaue( pLuaManager, vecAtkPhysic );
		if( true == pLuaManager.BeginTable( "m_AtkMagic" ) )		SetSkillVlaue( pLuaManager, vecAtkMagic );
		if( true == pLuaManager.BeginTable( "m_DefPhysic" ) )		SetSkillVlaue( pLuaManager, vecDefPhysic );
		if( true == pLuaManager.BeginTable( "m_DefMagic" ) )		SetSkillVlaue( pLuaManager, vecDefMagic );	

		pLuaManager.EndTable(); // m_Stat
	}

	/// ���� ���Ϳ� ���� ��ġ ����
	for ( int i = 0; i < MAX_LIMITED_SKILL_LEVEL; ++i )
	{
		/// ���� ���� ���Ϳ� ���� ������, 0���� ����
		CX2Stat::Stat statvalue;
		statvalue.m_fBaseHP		= 0 <= i && i < static_cast<int>( vecBaseHP.size() ) ? vecBaseHP[i] : 0.f;
		statvalue.m_fAtkPhysic	= 0 <= i && i < static_cast<int>( vecAtkPhysic.size() ) ? vecAtkPhysic[i] : 0.f;
		statvalue.m_fAtkMagic	= 0 <= i && i < static_cast<int>( vecAtkMagic.size() ) ? vecAtkMagic[i] : 0.f;
		statvalue.m_fDefPhysic	= 0 <= i && i < static_cast<int>( vecDefPhysic.size() ) ? vecDefPhysic[i] : 0.f;
		statvalue.m_fDefMagic	= 0 <= i && i < static_cast<int>( vecDefMagic.size() ) ? vecDefMagic[i] : 0.f;

		vecStat.push_back( statvalue );
	}
}

/** @function	: FixOrInsertMapSkillInfoValue
	@brief		: ��ų ���� Ȥ�� ��ȭ�� ������ �ѱ�� ���� ������ ���� �����̳��� �� ���� Ȥ�� �߰� �Լ�
	@param		: ������ ����ü
*/
void CX2SkillTree::FixOrInsertMapSkillInfoValue( CX2SkillTree::SKILL_ID eSkillID )
{
	if (	NULL == g_pData ||
			NULL == g_pData->GetSkillTree() ||
			NULL == g_pData->GetUIManager() ||
			NULL == g_pData->GetUIManager()->GetUISkillTree() ||
			NULL == g_pData->GetMyUser() ||
			NULL == g_pData->GetMyUser()->GetSelectUnit() )
	{
		return;
	}

	const CX2UserSkillTree&	refUserSkillTree	= g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_UserSkillTree;				/// ������ ��ų ���
	const int				iSkillLevel			= g_pData->GetUIManager()->GetUISkillTree()->GetSkillLevelInSkillWimdow( eSkillID );	/// ���� ��ų�� ����

	int						iUnitClass			= static_cast<int>( g_pData->GetMyUser()->GetSelectUnit()->GetClass() );				/// ���� Ŭ����
	const CX2SkillTree::SkillTreeTemplet*		pSkillTreeTemplet	= g_pData->GetSkillTree()->GetSkillTreeTemplet( iUnitClass, eSkillID );	/// ��ų Ʈ�� ���ø�

	/// ���� �ø� ������ ��ų�� �Ѱ� ������ �Ѿ�ٸ�, �н�
	if ( NULL != pSkillTreeTemplet && pSkillTreeTemplet->m_iMaxSkillLevel < iSkillLevel + 1 )
		return;

	const CX2SkillTree::SkillTemplet* pSkillTemplet = g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );				/// ��ų ���ø�

	if ( NULL != pSkillTemplet )
	{
		const int iUnitLevel	= g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_Level;						/// ���� ���� ����

		/// ���� �ø� ������ ��ų�� �䱸 ���� ���ѿ� �ɸ���, �н�
		if ( pSkillTemplet->GetSkillRequireLevelValue( iSkillLevel + 1 ) > iUnitLevel )
			return;

		const int iHaveSPoint	= g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iSPoint - m_iUsedSPoint;		/// ���� ��ų ����Ʈ
		const int iHaveCSPoint	= g_pData->GetMyUser()->GetSelectUnit()->GetUnitData().m_iCSPoint - m_iUsedCSPoint;	/// ���� ĳ�� ��ų ����Ʈ

		/// �Һ� ��ų ����Ʈ ���� ( �������� ��ȭ���� )
		const int	iRequireSP			= 0 == iSkillLevel ? pSkillTemplet->m_iRequireLearnSkillPoint : pSkillTemplet->m_iRequireUpgradeSkillPoint;
		int			iSpendSkillCSPoint	= 0;		/// ����ü�� ������ ĳ�� ��ų ����Ʈ ��� ��

		/// ��ų ����Ʈ �Һ� ó��
		if ( 0 < iHaveCSPoint )  /// ĳ�� ��ų ����Ʈ�� ���� ��
		{
			if ( iRequireSP <= iHaveCSPoint )	/// ĳ�� ��ų ����Ʈ�� ����� ���� ��
			{
				iSpendSkillCSPoint	=  iRequireSP;	/// ���� ĳ�� ��ų ����Ʈ ����
				m_iUsedCSPoint		+= iRequireSP;	/// ĳ�� ��ų ����Ʈ �Һ�
			}
			else		/// ĳ�� ��ų ����Ʈ�� ������ ��
			{
				iSpendSkillCSPoint	=  iHaveCSPoint;					/// ���� ĳ�� ��ų ����Ʈ ���θ� ���
				m_iUsedCSPoint		+= iSpendSkillCSPoint;				/// ĳ�� ��ų ����Ʈ �Һ�
				m_iUsedSPoint		+= iRequireSP - iSpendSkillCSPoint;	/// ������ ����Ʈ�� ��ų ����Ʈ�� �Һ� 
			}
		}
		else		/// ĳ�� ����Ʈ�� ���ٸ�, �Ϲ� ��ų ����Ʈ �Һ�
		{
			m_iUsedSPoint += iRequireSP;
		}

		std::map<int, KGetSkillInfo>::iterator it = m_mapGetSkillInfo.find( static_cast<int>( eSkillID ) );

		if ( it != m_mapGetSkillInfo.end() )	/// �ش� ��ų�� �����ϸ�, �ʿ��� ���� ����
		{
			KGetSkillInfo&						getSkillInfo	= it->second;

			const CX2SkillTree::SkillTemplet*	pSkillTemplet	= g_pData->GetSkillTree()->GetSkillTemplet( eSkillID );

			getSkillInfo.m_iSkillLevel			+= 1;
			getSkillInfo.m_iSpendSkillCSPoint	+= iSpendSkillCSPoint;

		}
		else									/// �ش� ��ų�� �������� ������, �ʿ� �߰�
		{
			KGetSkillInfo getSkillInfo;

			getSkillInfo.m_iSkillID				=  static_cast<int>( eSkillID );
			getSkillInfo.m_iBeforeSkillLevel	=  iSkillLevel;
			getSkillInfo.m_iSkillLevel			=  iSkillLevel + 1;
			getSkillInfo.m_iSpendSkillCSPoint	+= iSpendSkillCSPoint;

			m_mapGetSkillInfo.insert( std::make_pair( static_cast<int>( getSkillInfo.m_iSkillID ), getSkillInfo ) );

#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
			if( iSkillLevel == 0 )	// ���� ��� ��ų�̸�
			{
				m_vecNowLearnSkill.push_back(getSkillInfo.m_iSkillID);
			}
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL
		}
	}
	else
	{
		ASSERT( !"FixOrInsertMapSkillInfoValue() SkillTemplet Is Error!!!" );
	}
}

/** @function	: AddSkillPowerRate_LUA
	@brief		: ������ ������ ���� ���̺� �Ľ� �Լ�
*/
void CX2SkillTree::AddSkillPowerRate_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	SKILL_POWER_RATE_TYPE	eSkillPowerRateType = SPRT_NONE;	/// ������ ���� ���̵�
	vector<float>			vecPoewrRate;						/// ������ ���� �����̳�

	LUA_GET_VALUE_ENUM( luaManager,	"m_eID", eSkillPowerRateType, SKILL_POWER_RATE_TYPE, SPRT_NONE );

	if( true == luaManager.BeginTable( "m_Value" ) )
		SetSkillVlaue( luaManager, vecPoewrRate );

	if ( SPRT_NONE != eSkillPowerRateType && true != vecPoewrRate.empty() )		/// ������ �Ľ� �Ǿ��� ����, �߰� �Ѵ�.
		m_mapSkillPowerRateType.insert( make_pair( eSkillPowerRateType, vecPoewrRate ) );
}

void CX2SkillTree::SetSkillPowerRate( IN SKILL_POWER_RATE_TYPE eSkillPowerRateType, OUT vector<float>& vecPowerRate )
{
	std::map<SKILL_POWER_RATE_TYPE, vector<float>>::iterator it = m_mapSkillPowerRateType.find( eSkillPowerRateType );

	if ( it != m_mapSkillPowerRateType.end() )
	{
		vector<float> vecValue = it->second;
		
		if ( vecValue.size() == MAX_LIMITED_SKILL_LEVEL )		/// 30���� ���� �� ������, ���� ���� ������
			vecPowerRate = vecValue;
		else
			ASSERT( !"PowerRate Value Size is Error!!!" );		/// ������ ���� ���� �Ľ� Ȥ�� ������ �߸��Ȱ� ��������!!!
	}
	else
		ASSERT( !"PowerRate Value Size is Error!!!" );			/// ������ ���� ���� �Ľ� Ȥ�� ������ �߸��Ȱ� ��������!!!
}

/** @function	: AddDefaultSkill_LUA
	@brief		: ������ �⺻ ��ų ���̺� �Ľ� �Լ�
*/
void CX2SkillTree::AddDefaultSkill_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING	
    TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	int iTableIndex = 1;	/// ���̺� �ε��� ( Lua�� ���̺� ù��° �ε����� 1 )
	int iValue		= -1;	/// ���ް�
	int iUnitClass	= -1;	/// ���� Ŭ����
	std::vector<int> vecDefaultSkill;

	while( luaManager.GetValue( iTableIndex, iValue ) == true )
	{
		if ( 1 == iTableIndex )		/// ù��° �ε����� Ŭ���� ��
			iUnitClass = static_cast<CX2Unit::UNIT_CLASS>( iValue );
		else						/// ���ķδ� ��ų ���̵�
		{
			if ( 0 != iValue )
				vecDefaultSkill.push_back( iValue );
		}

		++iTableIndex;
	}

	m_mapDefaultSkill.insert( std::make_pair( iUnitClass, vecDefaultSkill ) );
}

/** @function	: AddDefaultSkill_LUA
	@brief		: �ش� ��ų ���̵� �⺻ ��ų���� �˻�
	@param		: �˻��� ��ų ���̵�
	@return		: �⺻ ��ų ����
*/
bool CX2SkillTree::isDefaultSkill( SKILL_ID eSkillID )
{
	if ( NULL == g_pData ||
		 NULL == g_pData->GetMyUser() ||
		 NULL == g_pData->GetMyUser()->GetSelectUnit() )
		 return false;

	const CX2Unit::UnitData* pUnitData = &g_pData->GetMyUser()->GetSelectUnit()->GetUnitData();


	/// �ش� Ŭ������ �⺻ ��ų ��� ��ȯ
	const std::map<int, std::vector<int>>::const_iterator mit = m_mapDefaultSkill.find( pUnitData->m_UnitClass );

	if ( mit != m_mapDefaultSkill.end() )
	{
		std::vector<int> vecDefaultSkill = mit->second;		/// �⺻ ��ų ���

		/// ���ڷ� ���� ���̵� �⺻ ��ų ��Ͽ� �ִ��� �˻�
		BOOST_FOREACH( int iVal, vecDefaultSkill )
		{
			if ( eSkillID == static_cast<SKILL_ID>( iVal ) )
				return true;		/// ��ġ�ϴ� ���̵� �����Ƿ�, �⺻ ��ų�̴�.
		}
	}

	return false;
}

/** @function	: 
	@brief		: ���� �ɸ����� ��ų���� �˻�
	@param		: �˻��� ��ų ���̵�
	@return		: �ڽ��� ��ų ����
*/
bool CX2SkillTree::IsMyClassSkillID( SKILL_ID eSkillID )
{
	if ( NULL == g_pData ||
		 NULL == g_pData->GetMyUser() ||
		 NULL == g_pData->GetMyUser()->GetSelectUnit() )
	{
		return false;
	}

	/// �ش� ��ų ���̵� ���� �ɸ����� ��ų���� �˻�
	const CX2Unit::UNIT_CLASS eUnitClass = g_pData->GetMyUser()->GetSelectUnit()->GetClass();

	const SkillTreeTempletMap& mapSkillTreeTemplet = GetSkillTreeTempletMap( static_cast<int>( eUnitClass ) );

	SkillTreeTempletMap::const_iterator mit = mapSkillTreeTemplet.find( eSkillID );

	if ( mapSkillTreeTemplet.end() != mit )
		return true;

	return false;
}

/** @function	: GetUnsealSkillItemRequireClass
	@brief		: �ش� ���� ��ų �������� �䱸�ϴ� ���� Ŭ���� ���� ��ȯ
	@param		: ������ ���̵�
	@return		: ���� Ŭ���� ����
*/
const int CX2SkillTree::GetUnsealSkillItemRequireClass( const int iItemID ) const
{
	std::map< int, SealSkillItemInfo >::const_iterator mit = m_mapSealSkillItemTemplet.find( iItemID );

	if ( mit != m_mapSealSkillItemTemplet.end() )
		return mit->second.m_iUnitClassType;

	return 0;
}

/** @function	: GetCalcInitSkillPoint
	@brief		: ������ ���� �� ��ų ����Ʈ ��ȯ
	@param		: ���� ����
	@return		: �� ��ų ����Ʈ
*/
const int CX2SkillTree::GetCalcInitSkillPoint( const int iUnitLevel ) const
{
	int iSkillPoint = 0;

	for( int iLevel = 2 ; iLevel <= iUnitLevel ; ++iLevel )
	{
		int iLevelSkillPoint = lua_tinker::call<int>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalcLevelUpIncreaseSkillPoint", iLevel );
		iSkillPoint += iLevelSkillPoint;
	}

	return iSkillPoint;
}

#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef SKILL_LEVEL_UP_BY_POWER_RATE_TYPE
UINT CX2SkillTree::GetSkillLevelUpNumByPowerRateType( SKILL_POWER_RATE_TYPE eSkillPowerRateType_ )
{
	switch( eSkillPowerRateType_ )
	{
	case SPRT_BASIC_CLASS:			/// �⺻ ����
		{
			return 4;
		} break;
	case SPRT_FIRST_CLASS:			/// 1������
		{
			return 3;
		} break;
	case SPRT_SECOND_CLASS:			/// 2������
		{
			return 2;
		} break;
	case SPRT_SECOND_CLASS2:			/// 2������2 (�Ĺ� ��ų) 
		{
			return 1;
		} break;
	// ������� ������ ���Ͽ� ������� �ʾұ� ������ �ּ� ó��
// 	case SPRT_HYPER_SKILL:			/// ������
// 		{
// 			return 1;
// 		} break;
	default:
		return 0;
		break;
	}
	return 0;
}
#endif // SKILL_LEVEL_UP_BY_POWER_RATE_TYPE


#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
bool CX2SkillTree::SkillTemplet::GetSkillEffectiveDescExpand ( OUT wstring & wstrSkillEffectiveDesc, int iLevel ) const
{
	// ��ų ������ Ȯ���ؾ� �� ��ų �ΰ�?
	// Ex. ���� ��ġ, ������ ����Ʈ
	switch ( m_eID )
	{
	case CX2SkillTree::SI_SA_ABM_ENERGY_SPURT:	
		// Ȥ�� �� ũ���ø� ���� ���� ó��..
		if ( NULL != g_pData && NULL != g_pData->GetMyUser() && NULL != g_pData->GetMyUser()->GetSelectUnit() )
		{
			float fEnergySpurtAddDamageValue = powf ( static_cast<float> ( g_pData->GetSelectUnitLevel() ), 2.f ) * 35.f * GetSkillPowerRateValue( iLevel );

			wstrSkillEffectiveDesc = g_pMain->GetReplacedWstring ( wstrSkillEffectiveDesc, "i", static_cast<int> ( fEnergySpurtAddDamageValue ) );			
			return true;
		}			
		break;
	default :
		return false;
	}
	return false;
}
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
