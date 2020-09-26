#include ".\xslattribenchantitem.h"
#include "NetError.h"
#include "X2Data/XSLItem.h"

ImplementRefreshSingleton( CXSLAttribEnchantItem );

CXSLAttribEnchantItem::CXSLAttribEnchantItem(void)
{
}

CXSLAttribEnchantItem::~CXSLAttribEnchantItem(void)
{
}

ImplToStringW( CXSLAttribEnchantItem )
{
	stm_	<< L"----------[ Attrib Enchant Item Manager ]----------" << std::endl		
			<< TOSTRINGW( m_kLotRandomAttribSingle.GetCaseNum() )
			<< TOSTRINGW( m_mapRandomAttribDual.size() )
			<< TOSTRINGW( m_mapRandomAttribTriple.size() )
			<< TOSTRINGW( m_mapEnchantRequireInfo.size() )
			<< TOSTRINGW( m_mapIdentifyInfo.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( CXSLAttribEnchantItem )
{
	lua_tinker::class_add<CXSLAttribEnchantItem>( GetLuaState(), "CXSLAttribEnchantItem" );	
	lua_tinker::class_def<CXSLAttribEnchantItem>( GetLuaState(), "AddEnchantRequire_LUA",		&CXSLAttribEnchantItem::AddEnchantRequire_LUA );
	lua_tinker::class_def<CXSLAttribEnchantItem>( GetLuaState(), "AddRandomAttribSingle",		&CXSLAttribEnchantItem::AddRandomAttribSingle_LUA );
	lua_tinker::class_def<CXSLAttribEnchantItem>( GetLuaState(), "AddRandomAttribDual",			&CXSLAttribEnchantItem::AddRandomAttribDual_LUA );
	//{{ 2009. 11. 9  ������	Ʈ���üӼ�
	lua_tinker::class_def<CXSLAttribEnchantItem>( GetLuaState(), "AddRandomAttribTriple",		&CXSLAttribEnchantItem::AddRandomAttribTriple_LUA );
	//}}
	lua_tinker::class_def<CXSLAttribEnchantItem>( GetLuaState(), "AddIdentifyInfo",				&CXSLAttribEnchantItem::AddIdentifyInfo_LUA );

	//{{ 2011. 01. 04  ��μ�	�Ӽ� ����
#ifdef SERV_ATTRIBUTE_CHARM
	lua_tinker::class_def<CXSLAttribEnchantItem>( GetLuaState(), "AddAttribAttachInfo",			&CXSLAttribEnchantItem::AddAttribAttachInfo_LUA );
#endif SERV_ATTRIBUTE_CHARM
		//}}

	lua_tinker::class_def<CXSLAttribEnchantItem>( GetLuaState(), "dump",						&CXSLAttribEnchantItem::Dump );

	lua_tinker::decl( GetLuaState(), "g_pCX2EnchantItem", this );
}

bool CXSLAttribEnchantItem::AddEnchantRequire_LUA( int iAttribEnchantType, int iEquipLv )
{
	std::map< int, int > mapItemGradeTable;
	std::pair< int, int > pairKey;
	int iLow = 0;
	int iNormal = 0;
	int iRare = 0;
	int iElite = 0;
	int iUnique = 0;	

	// ���̺� Ű��
	pairKey.first = iAttribEnchantType;
	pairKey.second = iEquipLv;

	// �����ۺ� �䱸 ����
	KLuaManager luaMgr( GetLuaState() );
	LUA_GET_VALUE_RETURN( luaMgr, "LOW",		iLow,	 0, goto fail_proc; );
	LUA_GET_VALUE_RETURN( luaMgr, "NORMAL",		iNormal, 0, goto fail_proc; );
	LUA_GET_VALUE_RETURN( luaMgr, "RARE",		iRare,	 0, goto fail_proc; );
	LUA_GET_VALUE_RETURN( luaMgr, "ELITE",		iElite,	 0, goto fail_proc; );
	LUA_GET_VALUE_RETURN( luaMgr, "UNIQUE",		iUnique, 0, goto fail_proc; );
	
	mapItemGradeTable.insert( std::make_pair( CXSLItem::IG_LOW,	   iLow ) );
	mapItemGradeTable.insert( std::make_pair( CXSLItem::IG_NORMAL, iNormal ) );
	mapItemGradeTable.insert( std::make_pair( CXSLItem::IG_RARE,   iRare ) );
	mapItemGradeTable.insert( std::make_pair( CXSLItem::IG_ELITE,  iElite ) );
	mapItemGradeTable.insert( std::make_pair( CXSLItem::IG_UNIQUE, iUnique ) );

	if( !VerifyAttribEnchantType( iAttribEnchantType )  ||  iEquipLv <= 0 )
		goto fail_proc;

	m_mapEnchantRequireInfo.insert( std::make_pair( pairKey, mapItemGradeTable ) );
	return true;

fail_proc:
	START_LOG( cerr, L"��ũ��Ʈ �Ľ� ����!" )
		<< BUILD_LOG( iAttribEnchantType )
		<< BUILD_LOG( iEquipLv )
		<< BUILD_LOG( iLow )
		<< BUILD_LOG( iNormal )
		<< BUILD_LOG( iRare )
		<< BUILD_LOG( iElite )
		<< BUILD_LOG( iUnique )
		<< END_LOG;
	return false;
}

bool CXSLAttribEnchantItem::AddRandomAttribSingle_LUA( char cEncahntType, float fRate )
{
	if( fRate < 0.0f )
	{
		START_LOG( cerr, L"�̱� ���� �Ӽ� Ȯ������ �̻��մϴ�." )
			<< BUILD_LOGc( cEncahntType )
			<< BUILD_LOG( fRate )
			<< END_LOG;
		return false;
	}

	return m_kLotRandomAttribSingle.AddCase( static_cast<int>(cEncahntType), fRate );
}

bool CXSLAttribEnchantItem::AddRandomAttribDual_LUA( char cSingleEncahntType, char cDualEncahntType, float fRate )
{
	if( fRate < 0.0f )
	{
		START_LOG( cerr, L"��� ���� �Ӽ� Ȯ������ �̻��մϴ�." )
			<< BUILD_LOGc( cSingleEncahntType )
			<< BUILD_LOGc( cDualEncahntType )
			<< BUILD_LOG( fRate )
			<< END_LOG;
		return false;
	}

	std::map< char, KLottery >::iterator mit;
	mit = m_mapRandomAttribDual.find( cSingleEncahntType );
	if( mit == m_mapRandomAttribDual.end() )
	{
		KLottery kLot;
		if( !kLot.AddCase( static_cast<int>(cDualEncahntType), fRate ) ) goto err_proc;
		m_mapRandomAttribDual.insert( std::make_pair( cSingleEncahntType, kLot ) );
	}
	else
	{
		if( !mit->second.AddCase( static_cast<int>(cDualEncahntType), fRate ) ) goto err_proc;
	}

	return true;

err_proc:
	START_LOG( cerr, L"��ũ��Ʈ �Ľ� ����!" )
		<< BUILD_LOGc( cSingleEncahntType )
		<< BUILD_LOGc( cDualEncahntType )
		<< BUILD_LOG( fRate )
		<< END_LOG;
	return false;
}

//{{ 2009. 11. 9  ������	Ʈ���üӼ�
bool CXSLAttribEnchantItem::AddRandomAttribTriple_LUA( char cSingleEncahntType, char cDualEncahntType, char cTripleEnchantType, float fRate )
{
	if( fRate < 0.0f )
	{
		START_LOG( cerr, L"��� ���� �Ӽ� Ȯ������ �̻��մϴ�." )
			<< BUILD_LOGc( cSingleEncahntType )
			<< BUILD_LOGc( cDualEncahntType )
			<< BUILD_LOGc( cTripleEnchantType )
			<< BUILD_LOG( fRate )
			<< END_LOG;
		return false;
	}

	{ // scope
		KTripleEnchantKey kKey( cSingleEncahntType, cDualEncahntType );

		std::map< KTripleEnchantKey, KLottery >::iterator mit;
		mit = m_mapRandomAttribTriple.find( kKey );
		if( mit == m_mapRandomAttribTriple.end() )
		{
			KLottery kLot;
			if( !kLot.AddCase( static_cast<int>(cTripleEnchantType), fRate ) ) goto err_proc;
			m_mapRandomAttribTriple.insert( std::make_pair( kKey, kLot ) );
		}
		else
		{
			if( !mit->second.AddCase( static_cast<int>(cTripleEnchantType), fRate ) ) goto err_proc;
		}
	} // scope

	// �Ӽ����� �ٸ� ����� key���� �ڹٲ㼭 ����
	if( cSingleEncahntType != cDualEncahntType )
	{
		KTripleEnchantKey kReverseKey( cDualEncahntType, cSingleEncahntType );

		std::map< KTripleEnchantKey, KLottery >::iterator mit;
		mit = m_mapRandomAttribTriple.find( kReverseKey );
		if( mit == m_mapRandomAttribTriple.end() )
		{
			KLottery kLot;
			if( !kLot.AddCase( static_cast<int>(cTripleEnchantType), fRate ) ) goto err_proc;
			m_mapRandomAttribTriple.insert( std::make_pair( kReverseKey, kLot ) );
		}
		else
		{
			if( !mit->second.AddCase( static_cast<int>(cTripleEnchantType), fRate ) ) goto err_proc;
		}		
	}

	return true;

err_proc:
	START_LOG( cerr, L"��ũ��Ʈ �Ľ� ����!" )
		<< BUILD_LOGc( cSingleEncahntType )
		<< BUILD_LOGc( cDualEncahntType )
		<< BUILD_LOGc( cTripleEnchantType )
		<< BUILD_LOG( fRate )
		<< END_LOG;
	return false;
}
//}}

bool CXSLAttribEnchantItem::AddIdentifyInfo_LUA( int iSourceItemID, int iRequireItemID, int iResultItemID, float fRate )
{
	if( iSourceItemID <= 0  ||  iRequireItemID <= 0  ||  iResultItemID <= 0  ||  fRate < 0.0f )
	{
		START_LOG( cerr, L"���� �����Ͱ� �̻��մϴ�." )
			<< BUILD_LOG( iSourceItemID )
			<< BUILD_LOG( iRequireItemID )
			<< BUILD_LOG( iResultItemID )
			<< BUILD_LOG( fRate )
			<< END_LOG;
		return false;
	}

	std::map< int, KIdentifyInfo >::iterator mit;
	mit = m_mapIdentifyInfo.find( iSourceItemID );
	if( mit == m_mapIdentifyInfo.end() )
	{
		KIdentifyInfo kIdentifyInfo;
		kIdentifyInfo.m_iRequireItemID = iRequireItemID;
		if( !kIdentifyInfo.m_kResult.AddCase( iResultItemID, fRate ) ) goto err_proc;
		m_mapIdentifyInfo.insert( std::make_pair( iSourceItemID, kIdentifyInfo ) );
	}
	else
	{
		if( !mit->second.m_kResult.AddCase( iResultItemID, fRate ) ) goto err_proc;
	}

	return true;

err_proc:
	START_LOG( cerr, L"��ũ��Ʈ �Ľ� ����!" )
		<< BUILD_LOG( iSourceItemID )
		<< BUILD_LOG( iRequireItemID )
		<< BUILD_LOG( iResultItemID )
		<< BUILD_LOG( fRate )
		<< END_LOG;
	return false;
}

//{{ 2011. 01. 04  ��μ�	�Ӽ� ����
#ifdef SERV_ATTRIBUTE_CHARM

bool CXSLAttribEnchantItem::AddAttribAttachInfo_LUA( int iItemID, char cSlot0, char cSlot1, char cSlot2 )
{
	if( iItemID <= 0  ||  cSlot0 < 0  ||  cSlot1 < 0  ||  cSlot2 < 0 )
	{
		START_LOG( cerr, L"���� �����Ͱ� �̻��մϴ�." )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( cSlot0 )
			<< BUILD_LOG( cSlot1 )
			<< BUILD_LOG( cSlot2 )
			<< END_LOG;
		return false;
	}

	//std::map< int, KItemAttributeEnchantInfo >::iterator mit = m_mapAttribAttachInfo.begin();
	
	KItemAttributeEnchantInfo AttributeEnchantInfo;

	AttributeEnchantInfo.m_cAttribEnchant0 = cSlot0;
	AttributeEnchantInfo.m_cAttribEnchant1 = cSlot1;
	AttributeEnchantInfo.m_cAttribEnchant2 = cSlot2;

	m_mapAttribAttachInfo.insert( std::make_pair( iItemID, AttributeEnchantInfo ) );
	
	return true;
}

bool CXSLAttribEnchantItem::GetAttribAttachInfo( IN int iAttachItemID, OUT KItemAttributeEnchantInfo& kInfo )
{
	std::map< int, KItemAttributeEnchantInfo >::const_iterator mit = m_mapAttribAttachInfo.find( iAttachItemID );
	if( mit == m_mapAttribAttachInfo.end() )
	{
		START_LOG( cerr, L"�Ӽ� �ο� �������� �ƴѵ� ������ ��û�Ͽ����ϴ�! Ŭ���̾�Ʈ���� �˻������ٵ�.." )
			<< BUILD_LOG( iAttachItemID )
			<< END_LOG;
		return false;
	}

	kInfo = mit->second;
	return true;
}
#endif SERV_ATTRIBUTE_CHARM
//}}

const CXSLAttribEnchantItem::KIdentifyInfo* CXSLAttribEnchantItem::GetIdentifyInfo( int iItemID )
{
	std::map< int, KIdentifyInfo >::const_iterator mit = m_mapIdentifyInfo.find( iItemID );
	if( mit == m_mapIdentifyInfo.end() )
		return NULL;

	return &mit->second;
}

CXSLAttribEnchantItem::ATTRIB_COUNT_TYPE CXSLAttribEnchantItem::GetAttribCountType( IN const KItemAttributeEnchantInfo& kInfo )
{
	if( kInfo.m_cAttribEnchant0 != 0 )
	{
		if( kInfo.m_cAttribEnchant1 != 0 )
		{
			if( kInfo.m_cAttribEnchant2 != 0 )
			{
				return ACT_TRIPLE;
			}
			else
			{
				return ACT_DUAL;
			}
		}
		else
		{
			if( kInfo.m_cAttribEnchant2 != 0 )
			{
				return ACT_DUAL;
			}
			else
			{
				return ACT_SINGLE;
			}
		}
	}
	else
	{
		if( kInfo.m_cAttribEnchant1 != 0 )
		{
			if( kInfo.m_cAttribEnchant2 != 0 )
			{
				return ACT_DUAL;
			}
			else
			{
				return ACT_SINGLE;
			}
		}
		else
		{
			if( kInfo.m_cAttribEnchant2 != 0 )
			{
                return ACT_SINGLE;
			}
			else
			{
                return ACT_NONE;
			}
		}
	}

	START_LOG( cerr, L"������ �������� �������ü� ����." )
		<< END_LOG;

	return ACT_NONE;
}

char CXSLAttribEnchantItem::GetAttribEnchantType( IN ATTRIB_COUNT_TYPE eCountType, IN bool bWeapon )
{
	switch( eCountType )
	{
	case ACT_NONE:
		{
			if( bWeapon )
			{
				return AET_SINGLE_WEAPON;
			}
			else
			{
				return AET_ARMOR;
			}
		}
		break;

	case ACT_SINGLE:
		{
			if( bWeapon )
			{
				return AET_DUAL_WEAPON;
			}
			else
			{
				return AET_DUAL_ARMOR;
			}
		}
		break;

	case ACT_DUAL:
		{
			if( bWeapon )
			{
				return AET_TRIPLE_WEAPON;
			}
		}
		break;
	}

	START_LOG( cerr, L"������ �ʴ� Ÿ���ε� �������� �Գ�?" )
		<< BUILD_LOG( eCountType )
		<< BUILD_LOG( bWeapon )
		<< END_LOG;

	return AET_NONE;	
}

bool CXSLAttribEnchantItem::GetRequireItemCount_Insert( IN ATTRIB_COUNT_TYPE eCountType, IN bool bWeapon, IN int iEqipLevel, IN int iItemGrade, OUT int& iRequireCount ) const
{
	std::pair< int, int > pairKey;
	pairKey.first  = GetAttribEnchantType( eCountType, bWeapon );
	pairKey.second = iEqipLevel;

	MapEnchantRequire::const_iterator mit;
	mit = m_mapEnchantRequireInfo.find( pairKey );
	if( mit == m_mapEnchantRequireInfo.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� �Ӽ���ȭ �����Դϴ�." )
			<< BUILD_LOG( pairKey.first )
			<< BUILD_LOG( pairKey.second )
			<< END_LOG;

		return false;
	}

	std::map< int, int >::const_iterator mitGrade;
	mitGrade = mit->second.find( iItemGrade );
	if( mitGrade == mit->second.end() )
	{
		START_LOG( cerr, L"������ ��� ������ �̻��մϴ�." )
			<< BUILD_LOG( pairKey.first )
			<< BUILD_LOG( pairKey.second )
			<< BUILD_LOG( iItemGrade )
			<< END_LOG;

		return false;
	}

	iRequireCount = mitGrade->second;
	return true;
}

bool CXSLAttribEnchantItem::GetRequireED_Insert( IN ATTRIB_COUNT_TYPE eCountType, IN bool bWeapon, IN int iEqipLevel, IN int iItemGrade, OUT int& iRequireED ) const
{
	// ���� �����ִ� �Ӽ��� ���� Ÿ���� ������ ATTRIB_ENCHANT_TYPE�� ����ϴ�.
	const char cAttribEnchantType = GetAttribEnchantType( eCountType, bWeapon );
    
	switch( cAttribEnchantType )
	{
	case AET_SINGLE_WEAPON:
		{
			iRequireED = ( iEqipLevel + CXSLAttribEnchantItem::GetRequireEDFactor( iItemGrade ) ) * 100;
		}
		break;

	case AET_DUAL_WEAPON:
		{
			iRequireED = ( ( iEqipLevel + CXSLAttribEnchantItem::GetRequireEDFactor( iItemGrade ) ) * 100 ) * 3;
		}
		break;

		//{{ 2009. 11. 9  ������	Ʈ���üӼ�
	case AET_TRIPLE_WEAPON:
		{
			iRequireED = ( ( iEqipLevel + CXSLAttribEnchantItem::GetRequireEDFactor( iItemGrade ) ) * 100 ) * 6;
		}
		break;
		//}}

	case AET_ARMOR:
		{
			iRequireED = static_cast<int>( ( ( iEqipLevel + CXSLAttribEnchantItem::GetRequireEDFactor( iItemGrade ) ) * 100 ) * 0.25f );
		}
		break;

		//{{ 2009. 11. 9  ������	Ʈ���üӼ�
	case AET_DUAL_ARMOR:
		{
			iRequireED = static_cast<int>( ( ( iEqipLevel + CXSLAttribEnchantItem::GetRequireEDFactor( iItemGrade ) ) * 100 ) * 0.75f );
		}
		break;
		//}}
	
	default:
		START_LOG( cerr, L"�̻��� �Ӽ���ȭ Ÿ���Դϴ�." )
			<< BUILD_LOG( eCountType )
			<< BUILD_LOG( bWeapon )
			<< END_LOG;

		return false;
	}

	return true;
}

bool CXSLAttribEnchantItem::GetRequireED_Remove( IN ATTRIB_COUNT_TYPE eCountType, IN bool bWeapon, IN int iEqipLevel, IN int iItemGrade, OUT int& iRequireED ) const
{
	if( bWeapon )
	{
		switch( eCountType )
		{
		case ACT_SINGLE:
			{
				iRequireED = ( ( iEqipLevel + CXSLAttribEnchantItem::GetRequireEDFactor( iItemGrade ) ) * 100 ) * 3;
			}
			break;

		case ACT_DUAL:
			{
				iRequireED = ( ( iEqipLevel + CXSLAttribEnchantItem::GetRequireEDFactor( iItemGrade ) ) * 100 ) * 6;
			}
			break;

			//{{ 2009. 11. 9  ������	Ʈ���üӼ�
		case ACT_TRIPLE:
			{
				iRequireED = ( ( iEqipLevel + CXSLAttribEnchantItem::GetRequireEDFactor( iItemGrade ) ) * 100 ) * 12;
			}
			break;
			//}}

		default:
			START_LOG( cerr, L"�̻��� �Ӽ���ȭī��Ʈ Ÿ���Դϴ�." )
				<< BUILD_LOG( eCountType )
				<< BUILD_LOG( bWeapon )
				<< END_LOG;

			return false;
		}
	}
	else
	{
		switch( eCountType )
		{
		case ACT_SINGLE:
			{
				iRequireED = static_cast<int>( ( ( iEqipLevel + CXSLAttribEnchantItem::GetRequireEDFactor( iItemGrade ) ) * 100 ) * 0.75f );
			}
			break;

			//{{ 2009. 11. 9  ������	Ʈ���üӼ�
		case ACT_DUAL:
			{
				iRequireED = static_cast<int>( ( ( iEqipLevel + CXSLAttribEnchantItem::GetRequireEDFactor( iItemGrade ) ) * 100 ) * 1.5f );
			}
			break;
			//}}

		default:
			START_LOG( cerr, L"�̻��� �Ӽ���ȭī��Ʈ Ÿ���Դϴ�." )
				<< BUILD_LOG( eCountType )
				<< BUILD_LOG( bWeapon )
				<< END_LOG;

			return false;
		}
	}

	return true;
}

int	CXSLAttribEnchantItem::GetRequireEDFactor( int iItemGrade )
{
	switch( iItemGrade )
	{
	case CXSLItem::IG_LOW:
	case CXSLItem::IG_NORMAL:
		return 0;

	case CXSLItem::IG_RARE:
		return 2;

	case CXSLItem::IG_ELITE:
		return 4;

	case CXSLItem::IG_UNIQUE:
		return 6;

	default:
		START_LOG( cerr, L"������ ����� �̻��ϴ�?" )
			<< BUILD_LOG( iItemGrade )
			<< END_LOG;

		return 0;
	}
}

bool CXSLAttribEnchantItem::IsAttribItemID( int iItemID )
{
	switch( iItemID )
	{
	case ATI_UNKNOWN:
	case ATI_RED:
	case ATI_BLUE:
	case ATI_GREEN:
	case ATI_WIND:
	case ATI_LIGHT:
	case ATI_DARK:
		return true;

	default:
		break;
	}

	return false;
}

bool CXSLAttribEnchantItem::VerifyAttribEnchantType( IN char cAttribEnchantType )
{
	switch( cAttribEnchantType )
	{
	case ET_NONE:
	case ET_BLAZE:
	case ET_WATER:
	case ET_NATURE:
	case ET_WIND:
	case ET_LIGHT:
	case ET_DARK:
	case ET_RANDOM:
		return true;

	default:
		return false;
	}
}

char CXSLAttribEnchantItem::GetRandomAttribResult( IN ATTRIB_COUNT_TYPE eCurrentCountType, IN char cFirstAttribEnchant /*= ET_NONE*/, IN char cSecondAttribEnchant /*= ET_NONE*/ ) const
{
	char cResultCase = 0;

	// ���� �Ӽ����� �Ѱ��� ��������
	switch( eCurrentCountType )
	{
	case ACT_NONE:
		{
			cResultCase = static_cast<char>( m_kLotRandomAttribSingle.Decision() );
		}
		break;

	case ACT_SINGLE:
		{
			std::map< char, KLottery >::const_iterator mit;
			mit = m_mapRandomAttribDual.find( cFirstAttribEnchant );
			if( mit == m_mapRandomAttribDual.end() )
			{
				START_LOG( cerr, L"�ش� �Ӽ���ȭ�� �������̺� ��� ����." )
					<< BUILD_LOGc( cFirstAttribEnchant )
					<< END_LOG;

				cResultCase = ET_NONE;
				break;
			}

			cResultCase = static_cast<char>( mit->second.Decision() );
		}
		break;

		//{{ 2009. 11. 9  ������	Ʈ���üӼ�
	case ACT_DUAL:
		{
			KTripleEnchantKey kKey( cFirstAttribEnchant, cSecondAttribEnchant );

			std::map< KTripleEnchantKey, KLottery >::const_iterator mit;
			mit = m_mapRandomAttribTriple.find( kKey );
			if( mit == m_mapRandomAttribTriple.end() )
			{
				START_LOG( cerr, L"�ش� �Ӽ���ȭ�� �������̺� ��� ����." )
					<< BUILD_LOGc( cFirstAttribEnchant )
					<< BUILD_LOGc( cSecondAttribEnchant )
					<< END_LOG;

				cResultCase = ET_NONE;
				break;
			}

			cResultCase = static_cast<char>( mit->second.Decision() );
		}
		break;
		//}}

	default:
		{
			START_LOG( cerr, L"�߸��� COUNT_TYPE�Դϴ�." )
				<< BUILD_LOG( eCurrentCountType )
				<< END_LOG;

			cResultCase = ET_NONE;
		}
		break;
	}

	if( cResultCase == KLottery::CASE_BLANK )
	{
		START_LOG( cerr, L"�Ӽ� Ȯ���� �̻��մϴ�." )
			<< BUILD_LOG( eCurrentCountType )
			<< BUILD_LOGc( cFirstAttribEnchant )
			<< BUILD_LOGc( cSecondAttribEnchant )
			<< END_LOG;

		cResultCase = ET_NONE;
	}

	return cResultCase;
}

bool CXSLAttribEnchantItem::IsPossibleAttribEnchant( IN OUT char& cAttribEnchantType, IN char cAttribEnchantSlotNo, IN const KItemAttributeEnchantInfo& kInfo ) const
{	
	// �ڴ°��� ���°��� üũ
	if( cAttribEnchantType != ET_NONE )
	{
		//////////////////////////////////////////////////////////////////////////
		// ���ο� �Ӽ� �ڱ� - ���� ��ġ �˻�
		char cFirstAttribType = 0;
		char cSecondAttribType = 0;

		switch( cAttribEnchantSlotNo )
		{
		case ESI_SLOT_1:
			{
				if( kInfo.m_cAttribEnchant0 != ET_NONE )
				{
					SET_ERROR( ERR_ATTRIB_ENCHANT_02 );
					return false;
				}

				cFirstAttribType = kInfo.m_cAttribEnchant1;
				cSecondAttribType = kInfo.m_cAttribEnchant2;
			}
			break;

		case ESI_SLOT_2:
			{
				if( kInfo.m_cAttribEnchant1 != ET_NONE )
				{
					SET_ERROR( ERR_ATTRIB_ENCHANT_02 );
					return false;
				}

				cFirstAttribType = kInfo.m_cAttribEnchant0;
				cSecondAttribType = kInfo.m_cAttribEnchant2;
			}
			break;

			//{{ 2009. 11. 9  ������	Ʈ���üӼ�
		case ESI_SLOT_3:
			{
				if( kInfo.m_cAttribEnchant2 != ET_NONE )
				{
					SET_ERROR( ERR_ATTRIB_ENCHANT_02 );
					return false;
				}

				cFirstAttribType = kInfo.m_cAttribEnchant0;
				cSecondAttribType = kInfo.m_cAttribEnchant1;
			}
			break;
			//}}

		default:
			START_LOG( cerr, L"�Ӽ���ȭ ���԰��� �̻��մϴ�." )
				<< BUILD_LOGc( cAttribEnchantSlotNo )
				<< END_LOG;

			SET_ERROR( ERR_ATTRIB_ENCHANT_01 );
			return false;
		}

		//////////////////////////////////////////////////////////////////////////
		// �Ӽ� ���� �ľ�
		// �Ӽ� ��ȭ ���� Ÿ�� ���
		const ATTRIB_COUNT_TYPE eCurrentCountType = CXSLAttribEnchantItem::GetAttribCountType( kInfo );

		switch( eCurrentCountType )
		{
		case ACT_NONE: // ���� �ƹ��͵� �ȹ����־�!
			{
				// ������ ���
				if( cAttribEnchantType == ET_RANDOM )
				{
					cAttribEnchantType = GetRandomAttribResult( eCurrentCountType );
				}
			}
			break;

		case ACT_SINGLE: // �Ѱ� �����ֳ�
			{
				if( cFirstAttribType != 0 )
				{
					// ������ ���
					if( cAttribEnchantType == ET_RANDOM )
					{
						cAttribEnchantType = GetRandomAttribResult( eCurrentCountType, cFirstAttribType );
					}

					// �ο� ������ �Ӽ����� �˻�
					if( IsPossibleToPush( cFirstAttribType, cAttribEnchantType ) == false )
					{
						SET_ERROR( ERR_ATTRIB_ENCHANT_01 );
						return false;
					}
				}
				else if( cSecondAttribType != 0 )
				{
					// ������ ���
					if( cAttribEnchantType == ET_RANDOM )
					{
						cAttribEnchantType = GetRandomAttribResult( eCurrentCountType, cSecondAttribType );
					}

					// �ο� ������ �Ӽ����� �˻�
					if( IsPossibleToPush( cSecondAttribType, cAttribEnchantType ) == false )
					{
						SET_ERROR( ERR_ATTRIB_ENCHANT_01 );
						return false;
					}
				}
				else
				{
					START_LOG( cerr, L"�Ӽ� ���� �̻��մϴ�!" )
						<< BUILD_LOGc( kInfo.m_cAttribEnchant0 )
						<< BUILD_LOGc( kInfo.m_cAttribEnchant1 )
						<< BUILD_LOGc( kInfo.m_cAttribEnchant2 )
						<< END_LOG;

					SET_ERROR( ERR_ATTRIB_ENCHANT_01 );
					return false;
				}
			}
			break;

		case ACT_DUAL: // �ΰ� �����ֳ�
			{
				// ������ ���
				if( cAttribEnchantType == ET_RANDOM )
				{
					cAttribEnchantType = GetRandomAttribResult( eCurrentCountType, cFirstAttribType, cSecondAttribType );
				}
                				
				// �ο� ������ �Ӽ����� �˻�
				KTripleEnchantKey kKey( cFirstAttribType, cSecondAttribType );

				std::map< KTripleEnchantKey, KLottery >::const_iterator mit;
				mit = m_mapRandomAttribTriple.find( kKey );
				if( mit == m_mapRandomAttribTriple.end() )
				{
					START_LOG( cerr, L"�ΰ� �����ִµ� ���̺��� ������ ����?" )
						<< BUILD_LOGc( cFirstAttribType )
						<< BUILD_LOGc( cSecondAttribType )
						<< END_LOG;

					return false;
				}
                
				// Ʈ���� �Ӽ� ���� ���̺� �ش� �Ӽ����� �ֳ�? ������ ������ �ִ°Ŵ�!
				if( mit->second.IsExistCase( cAttribEnchantType ) == false )
				{
					START_LOG( cerr, L"Ŭ���̾�Ʈ���� �������ִ��� �˻� �����ٵ�?" )
						<< BUILD_LOGc( cFirstAttribType )
						<< BUILD_LOGc( cSecondAttribType )
						<< BUILD_LOGc( cAttribEnchantType )
						<< END_LOG;

                    return false;
				}
			}
			break;

		default:
			{
				START_LOG( cerr, L"�̹� Ʈ���� �Ӽ��̴�! �� �������� ����!" )
					<< BUILD_LOGc( kInfo.m_cAttribEnchant0 )
					<< BUILD_LOGc( kInfo.m_cAttribEnchant1 )
					<< BUILD_LOGc( kInfo.m_cAttribEnchant2 )
					<< END_LOG;

				SET_ERROR( ERR_ATTRIB_ENCHANT_01 );
				return false;
			}
			break;
		}
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////		
		// ���� �Ӽ� ����
		switch( cAttribEnchantSlotNo )
		{
		case ESI_SLOT_1:
			{
				if( kInfo.m_cAttribEnchant0 == ET_NONE )
				{
					SET_ERROR( ERR_ATTRIB_ENCHANT_03 );
					return false;
				}
			}
			break;

		case ESI_SLOT_2:
			{
				if( kInfo.m_cAttribEnchant1 == ET_NONE )
				{
					SET_ERROR( ERR_ATTRIB_ENCHANT_03 );
					return false;
				}
			}
			break;

			//{{ 2009. 11. 9  ������	Ʈ���üӼ�
		case ESI_SLOT_3:
			{
				if( kInfo.m_cAttribEnchant2 == ET_NONE )
				{
					SET_ERROR( ERR_ATTRIB_ENCHANT_03 );
					return false;
				}
			}
			break;
			//}}

		default:
			START_LOG( cerr, L"�Ӽ���ȭ ���԰��� �̻��մϴ�." )
				<< BUILD_LOGc( cAttribEnchantSlotNo )
				<< END_LOG;

			SET_ERROR( ERR_ATTRIB_ENCHANT_01 );
			return false;
		}
	}

	return true;
}

int CXSLAttribEnchantItem::GetRequireItemID( IN char cAttribEnchantType )
{
	switch( cAttribEnchantType )
	{
	case ET_NONE:	return ATI_NONE;
	case ET_BLAZE:  return ATI_RED;	
	case ET_WATER:  return ATI_BLUE;
	case ET_NATURE: return ATI_GREEN;
	case ET_WIND:	return ATI_WIND;
	case ET_LIGHT:  return ATI_LIGHT;
	case ET_DARK:   return ATI_DARK;
	case ET_RANDOM: return ATI_UNKNOWN;

	default:
		break;
	}

	return -1;
}

bool CXSLAttribEnchantItem::IsPossibleToPush( IN char cExistingType, IN char cTypeToAdd ) const
{
	if( cExistingType == ET_NONE  ||  cTypeToAdd == ET_NONE )
	{
		START_LOG( cerr, L"�˻� �Ϸ��� Ÿ���� �̻��մϴ�." )
			<< BUILD_LOGc( cExistingType )
			<< BUILD_LOGc( cTypeToAdd )
			<< END_LOG;

		return false;
	}

	if ( cExistingType == ET_BLAZE )
	{
		if ( cTypeToAdd == ET_WATER || cTypeToAdd == ET_NATURE )
		{
			return false;
		}
	}
	else if ( cExistingType == ET_WATER )
	{
		if ( cTypeToAdd == ET_BLAZE || cTypeToAdd == ET_NATURE )
		{
			return false;
		}
	}
	else if ( cExistingType == ET_NATURE )
	{
		if ( cTypeToAdd == ET_BLAZE || cTypeToAdd == ET_WATER )
		{
			return false;
		}
	}
	else if ( cExistingType == ET_WIND )
	{
		if ( cTypeToAdd == ET_LIGHT || cTypeToAdd == ET_DARK )
		{
			return false;
		}
	}
	else if ( cExistingType == ET_LIGHT )
	{
		if ( cTypeToAdd == ET_WIND || cTypeToAdd == ET_DARK )
		{
			return false;
		}
	}
	else if ( cExistingType == ET_DARK )
	{
		if ( cTypeToAdd == ET_WIND || cTypeToAdd == ET_LIGHT )
		{
			return false;
		}
	}

	return true;
}



#ifdef SERV_FINALITY_SKILL_SYSTEM	// ���볯¥: 2013-08-01
bool CXSLAttribEnchantItem::IsExtractItem( const int iItemID )
{
	switch( iItemID )
	{
	case ATI_RED:
	case ATI_BLUE:
	case ATI_GREEN:
	case ATI_WIND:
	case ATI_LIGHT:
	case ATI_DARK:
		return true;
	default:
		break;
	}

	return false;
}
#endif // SERV_FINALITY_SKILL_SYSTEM
