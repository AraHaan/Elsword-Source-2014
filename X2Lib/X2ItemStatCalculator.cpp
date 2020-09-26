#include "StdAfx.h"
#include ".\X2ItemStatCalculator.h"

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
CX2ItemStatCalculator::CX2ItemStatCalculator():
m_eUnitType(CX2Unit::UT_NONE),
m_eUnitClass(CX2Unit::UC_NONE)
{

}

CX2ItemStatCalculator::~CX2ItemStatCalculator()
{

}

void CX2ItemStatCalculator::OpenScriptFile()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, false );
	if( true == g_pKTDXApp->LoadLuaTinker( L"CharacterStatTable.lua" ) )
	{
		// ��� ���� ���� �� �ɷ�ġ
		if( true == luaManager.BeginTable( "EQIP_POSITION_RELATIVE_STAT_TABLE" ) )
		{
			int iTableIndex = 1;
			while( true == luaManager.BeginTable( iTableIndex  ) )
			{
				CX2Unit::EQIP_POSITION eEqipPosition = CX2Unit::EP_NONE;

				LUA_GET_VALUE_ENUM( luaManager,		TDT_TYPE,		eEqipPosition,		CX2Unit::EQIP_POSITION,		CX2Unit::EP_NONE );
				if( CX2Unit::EP_NONE == eEqipPosition )
				{
					luaManager.EndTable();
					ASSERT(!"EQIP_POSITION_RELATIVE_STAT_TABLE Error");
					break;
				}

				CX2Item::KItemFormatStatData sStatData;

				LUA_GET_VALUE( luaManager,		TDT_ATK_PHYSIC,		sStatData.m_fAtkPhysic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_DEF_PHYSIC,		sStatData.m_fDefPhysic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_ATK_MAGIC,		sStatData.m_fAtkMagic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_DEF_MAGIC,		sStatData.m_fDefMagic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_HP,				sStatData.m_fBaseHP,		0.f );

				m_mapEqipStatDataTable.insert( std::make_pair( eEqipPosition, sStatData ) );

				++iTableIndex;
				luaManager.EndTable();
			}

			luaManager.EndTable();
		}

		// ĳ���ͺ� �ɷ�ġ
		if( true == luaManager.BeginTable( "CHARACTER_RELATIVE_STAT_TABLE" ) )
		{
			int iTableIndex = 1;
			while( true == luaManager.BeginTable( iTableIndex  ) )
			{
				CX2Unit::UNIT_TYPE eUnitType = CX2Unit::UT_NONE;

				LUA_GET_VALUE_ENUM( luaManager,		TDT_TYPE,		eUnitType,		CX2Unit::UNIT_TYPE,		CX2Unit::UT_NONE );
				if( CX2Unit::UC_NONE == eUnitType )
				{
					luaManager.EndTable();
					ASSERT(!"CHARACTER_RELATIVE_STAT_TABLE Error");
					break;
				}

				CX2Item::KItemFormatStatData sStatData;

				LUA_GET_VALUE( luaManager,		TDT_ATK_PHYSIC,		sStatData.m_fAtkPhysic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_DEF_PHYSIC,		sStatData.m_fDefPhysic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_ATK_MAGIC,		sStatData.m_fAtkMagic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_DEF_MAGIC,		sStatData.m_fDefMagic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_HP,				sStatData.m_fBaseHP,		0.f );

				m_mapCharStatDataTable.insert( std::make_pair( eUnitType, sStatData ) );

				++iTableIndex;
				luaManager.EndTable();
			}

			luaManager.EndTable();
		}

		// 2�� ������ �ɷ�ġ( 2�������� �ƴ϶�� ĳ���ͺ� �ɷ�ġ ��� )
		if( true == luaManager.BeginTable( "SECOND_CLASS_RELATIVE_STAT_TABLE" ) )
		{
			int iTableIndex = 1;
			while( true == luaManager.BeginTable( iTableIndex  ) )
			{
				CX2Unit::UNIT_CLASS eUnitClass = CX2Unit::UC_NONE;

				LUA_GET_VALUE_ENUM( luaManager,		TDT_TYPE,		eUnitClass,		CX2Unit::UNIT_CLASS,		CX2Unit::UC_NONE );
				if( CX2Unit::UT_NONE == eUnitClass )
				{
					luaManager.EndTable();
					ASSERT(!"SECOND_CLASS_RELATIVE_STAT_TABLE Error");
					break;
				}

				CX2Item::KItemFormatStatData sStatData;

				LUA_GET_VALUE( luaManager,		TDT_ATK_PHYSIC,		sStatData.m_fAtkPhysic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_DEF_PHYSIC,		sStatData.m_fDefPhysic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_ATK_MAGIC,		sStatData.m_fAtkMagic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_DEF_MAGIC,		sStatData.m_fDefMagic,		0.f );
				LUA_GET_VALUE( luaManager,		TDT_HP,				sStatData.m_fBaseHP,		0.f );

				m_mapClassStatDataTable.insert( std::make_pair( eUnitClass, sStatData ) );

				++iTableIndex;
				luaManager.EndTable();
			}

			luaManager.EndTable();
		}
	}

	return;
}

void CX2ItemStatCalculator::ResetItemStatCalculator()
{
	m_mapEqipStatDataTable.clear();
	m_mapCharStatDataTable.clear();
	m_mapClassStatDataTable.clear();

	OpenScriptFile();

	if( false == InvalideDataCheck() )
	{
		ErrorLog( XEM_ERROR148 )
		ASSERT(!"CharacterStatTable.lua Error");
	}
}

/** @function : InvalideDataCheck
	@brief : ��ũ��Ʈ�κ��� �Ľ��� �������� ��ȿ�� üũ
			 -> ������ �߰� �Ǿ��� ��, ��ũ��Ʈ�� ������ �߰����� �ʴ� ��Ȳ ���
				��� �� �� �ֵ��� �ϴ� ���
*/
bool CX2ItemStatCalculator::InvalideDataCheck() const
{
	for( int iUnitType = (CX2Unit::UT_NONE + 1); iUnitType < CX2Unit::UT_END; ++iUnitType )
	{
		if( m_mapCharStatDataTable.find( static_cast<CX2Unit::UNIT_TYPE>(iUnitType) ) 
			== m_mapCharStatDataTable.end() )
			return false;
	}

	// 2�� ���� ID ù�� ° �� CX2Unit::UC_ELSWORD_LORD_KNIGHT
	for( int iUnitClass = CX2Unit::UC_ELSWORD_LORD_KNIGHT ; iUnitClass < CX2Unit::UC_VALIDE_END; ++iUnitClass )
	{
		if( m_mapClassStatDataTable.find( static_cast<CX2Unit::UNIT_CLASS>(iUnitClass) ) 
			== m_mapClassStatDataTable.end() )
			return false;
	}

	if( m_mapEqipStatDataTable.find( CX2Unit::EP_WEAPON_HAND ) == m_mapEqipStatDataTable.end() )
		return false;

	if( m_mapEqipStatDataTable.find( CX2Unit::EP_DEFENCE_BODY ) == m_mapEqipStatDataTable.end() )
		return false;

	if( m_mapEqipStatDataTable.find( CX2Unit::EP_DEFENCE_LEG ) == m_mapEqipStatDataTable.end() )
		return false;

	if( m_mapEqipStatDataTable.find( CX2Unit::EP_DEFENCE_HAND ) == m_mapEqipStatDataTable.end() )
		return false;

	if( m_mapEqipStatDataTable.find( CX2Unit::EP_DEFENCE_FOOT ) == m_mapEqipStatDataTable.end() )
		return false;

	return true;
}

/** @function : GetCalculateItemStat
	@brief : ->���������ø��� ������ �� ĳ���� ������ �����Ͽ� ���� ���� ���
			 ->��������� ������ �ִ� �� Ŭ���� ������ ���� ��� �� ��� 
			   m_mapItemTemplmetStat�� ��� ��� ���, �� �� üũ �� ���� ���� ����� ���� �� ����
			 ->���ȿ��� �߰� ������ 0�� ��� �Ϲ� ���������� �Ǵ��ϰ�, 
			   item.lua�� ������ �������� �Ѱ���
			   */
void CX2ItemStatCalculator::CalculateItemStat( OUT CX2Item::KItemFormatStatData& sRealStatData_, IN const CX2Item::ItemTemplet* pItemTemplet_ ,
												IN const CX2SocketItem::KItemStatRelLVData& sStatRelLVData_ /*= CX2SocketItem::ms_KDummyItemStatRelLVData*/,
												IN CX2Unit::UNIT_TYPE eUnitType_ /*= CX2Unit::UT_NONE*/, IN CX2Unit::UNIT_CLASS eUnitClass_ /*= CX2Unit::UC_NONE*/ )
{
	if( NULL == pItemTemplet_ )
		return;

	if( CX2Unit::UC_NONE == eUnitType_ || CX2Unit::UC_NONE == eUnitClass_ )
	{ // UNIT_TYPE or UNIT_CLASS�� ���� �� �ߴٸ� ����� �� ĳ���� ������ ����
		eUnitType_ = m_eUnitType;
		eUnitClass_ = m_eUnitClass;
	}

	CX2SocketItem::KItemStatRelLVData kStatRelationAddLV; 
	// ���� �ɼǰ�, �����ۿ� ������ �߰� ���� �� �ջ�
	{
		// ���� �ɼǿ� ���� �߰� ����
		kStatRelationAddLV.AddStat( sStatRelLVData_ );

		// ������ ������ ���� �߰� ���� ����
		const CX2Item::KItemFormatStatRelLVData& kItemTempletStatRelationLV = pItemTemplet_->GetStatRelLVData();
		kStatRelationAddLV.m_fBaseHPRelLV		+= static_cast<float>(kItemTempletStatRelationLV.m_byBaseHPRelLV);
		kStatRelationAddLV.m_fAtkPhysicRelLV	+= static_cast<float>(kItemTempletStatRelationLV.m_byAtkPhysicRelLV);
		kStatRelationAddLV.m_fAtkMagicRelLV		+= static_cast<float>(kItemTempletStatRelationLV.m_byAtkMagicRelLV);
		kStatRelationAddLV.m_fDefPhysicRelLV	+= static_cast<float>(kItemTempletStatRelationLV.m_byDefPhysicRelLV);
		kStatRelationAddLV.m_fDefMagicRelLV		+= static_cast<float>(kItemTempletStatRelationLV.m_byDefMagicRelLV);
	}

	// ���� ������ �����Ǿ����� �ʴٸ� �⺻ ���� ���ϱ�
	if( false == kStatRelationAddLV.IsValideCheck() || 
		( CX2Unit::UC_NONE == eUnitType_ || CX2Unit::UC_NONE == eUnitClass_ ) )
	{
		sRealStatData_ = pItemTemplet_->GetStatData();
		return;
	}
	else
	{	
		// Ű�� ����
		ItemStatKey sItemStatKey( pItemTemplet_->GetItemID(), eUnitClass_, kStatRelationAddLV );

		// �� �� ����� �������̶�� ���̻� ������� �ʰ� map�� ����� ���� �̿�
		map< ItemStatKey, CX2Item::KItemFormatStatData >::iterator it = m_mapItemTemplmetStat.find( sItemStatKey );
		if ( m_mapItemTemplmetStat.end() != it )
		{
			sRealStatData_ = it->second;
			return;
		}

		// ĳ���ͺ�, ������, ���������� ĳ���� ���� ���̺� ��� ���
		CX2Item::KItemFormatStatData sCharStatTable, sClassStatTable, sEqipStatTable ;
		GetStatDataTable( eUnitType_, eUnitClass_, pItemTemplet_->GetEqipPosition(), 
			sCharStatTable, sClassStatTable, sEqipStatTable );

		int iItemLevel = pItemTemplet_->GetUseLevel();  

		// ���� ������ ���� ����ϱ�
		CalculateRealStatData(sRealStatData_, sClassStatTable, sCharStatTable, 
			sEqipStatTable,	iItemLevel, kStatRelationAddLV);

		if( m_mapItemTemplmetStat.size() >= _CONST_::MAX_ITEM_TEMPLET_STAT_COUNT )
			m_mapItemTemplmetStat.clear();

		m_mapItemTemplmetStat.insert( std::make_pair( sItemStatKey, sRealStatData_ ));
	}
}

/** @function : GetStatDataTable
	@brief : ��ũ��Ʈ�κ��� �Ľ��� ������ ���� �������̺� ����
*/
void CX2ItemStatCalculator::GetStatDataTable( IN CX2Unit::UNIT_TYPE eUnitType_,
											IN CX2Unit::UNIT_CLASS eUnitClass_,
											IN CX2Unit::EQIP_POSITION eEqipPosition_, 
											OUT CX2Item::KItemFormatStatData& sCharStatTable_, 
											OUT CX2Item::KItemFormatStatData& sClassStatTable_, 
											OUT CX2Item::KItemFormatStatData& sEqipStatTable_ ) const
{
	// ĳ���ͺ� ���� ���̺�
	map< CX2Unit::UNIT_TYPE, CX2Item::KItemFormatStatData >::const_iterator itChar = m_mapCharStatDataTable.find(eUnitType_);  
	if( m_mapCharStatDataTable.end() != itChar )
	{
		sCharStatTable_ = itChar->second;
	}
	else
	{
		ASSERT(!"GetStatDataTable Error");
	}

	// ������ ���� ���̺�
	if( 2 == GetX2UnitClassGrade( eUnitClass_ ) )
	{
		map< CX2Unit::UNIT_CLASS, CX2Item::KItemFormatStatData >::const_iterator itClass = m_mapClassStatDataTable.find(eUnitClass_);
		if( m_mapClassStatDataTable.end() != itClass )
		{
			sClassStatTable_ = itClass->second;
		}
	}
	else
	{
		// 2�� ������ �ƴ϶�� �⺻ ĳ���ͺ� ���� ���̺� ���
		sClassStatTable_ = sCharStatTable_;
	}

	// ��� ���� ������ ���� ���̺�	
	map< CX2Unit::EQIP_POSITION, CX2Item::KItemFormatStatData >::const_iterator itEqip = m_mapEqipStatDataTable.find( eEqipPosition_ );
	if( m_mapEqipStatDataTable.end() != itEqip )
	{
		sEqipStatTable_ = itEqip->second;
	}
	else
	{
		ASSERT(!"GetStatDataTable Error");
	}
}

/** @function : SetUnitData
	@brief : �������̺� �ڷḦ ������� CharacterStatTable.lua ��ũ��Ʈ�� ��� �Լ��� ���� ���� ���
*/
void CX2ItemStatCalculator::CalculateRealStatData( OUT CX2Item::KItemFormatStatData& sRealStatData_,
													IN const CX2Item::KItemFormatStatData& sClassStatTable_, 
													IN const CX2Item::KItemFormatStatData& sCharStatTable_, 
													IN const CX2Item::KItemFormatStatData& sEqipStat_, 
													IN int iItemLevel_,
													IN const CX2SocketItem::KItemStatRelLVData& sStatRelLVData_ ) const
{
	//���� 
	sRealStatData_.m_fAtkPhysic = 
		lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),	"CalculatePhysicalAndMagicalStat", 
		sClassStatTable_.m_fAtkPhysic, sCharStatTable_.m_fAtkPhysic, 
		sEqipStat_.m_fAtkPhysic, iItemLevel_, sStatRelLVData_.m_fAtkPhysicRelLV );

	//����
	sRealStatData_.m_fDefPhysic = 
		lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),	"CalculatePhysicalAndMagicalStat", 
		sClassStatTable_.m_fDefPhysic, sCharStatTable_.m_fDefPhysic, 
		sEqipStat_.m_fDefPhysic, iItemLevel_, sStatRelLVData_.m_fDefPhysicRelLV );

	//����
	sRealStatData_.m_fAtkMagic = 
		lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),	"CalculatePhysicalAndMagicalStat", 
		sClassStatTable_.m_fAtkMagic, sCharStatTable_.m_fAtkMagic, 
		sEqipStat_.m_fAtkMagic, iItemLevel_, sStatRelLVData_.m_fAtkMagicRelLV );

	//����
	sRealStatData_.m_fDefMagic =
		lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),	"CalculatePhysicalAndMagicalStat", 
		sClassStatTable_.m_fDefMagic, sCharStatTable_.m_fDefMagic, 
		sEqipStat_.m_fDefMagic, iItemLevel_, sStatRelLVData_.m_fDefMagicRelLV );

	//ü��
	sRealStatData_.m_fBaseHP = 
		lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),	"CalculateHPStat", 
		sClassStatTable_.m_fBaseHP, sCharStatTable_.m_fBaseHP, 
		sEqipStat_.m_fBaseHP, iItemLevel_, sStatRelLVData_.m_fBaseHPRelLV );
}

/** @function : SetUnitData
	@brief : ���� ������ ������ ������ ����.
	    	 g_pData->GetMyUser()->SelectUnit( pUnit ) �� ȣ��Ǵ� ��� �ݵ�� �ҷ����� �Ѵ�.
*/
void CX2ItemStatCalculator::SetUnitData( CX2Unit::UNIT_TYPE eUnitType_, CX2Unit::UNIT_CLASS eUnitClass_ )
{
	m_eUnitType = eUnitType_;
	m_eUnitClass = eUnitClass_;

	// Ÿ��, Ŭ������ ����Ǹ� ���� ������ ����Ǳ� ������ �� �ʱ�ȭ
	m_mapItemTemplmetStat.clear();
}
/** @function : GetItemDescDev
	@brief : �系 �׽�Ʈ�� ���� ���� ��ũ����
*/
void CX2ItemStatCalculator::GetItemDescDev( OUT wstring& wstrItemDesc_, 
	IN const CX2Item::ItemTemplet* pItemTemplet ) const
{
#ifndef _IN_HOUSE_
	return;
#endif //_IN_HOUSE_

	if( NULL == pItemTemplet )
		return;

	if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		const CX2Item::KItemFormatStatRelLVData kStatRelLV = pItemTemplet->GetStatRelLVData();
		if( 0 == (	kStatRelLV.m_byAtkPhysicRelLV + kStatRelLV.m_byAtkMagicRelLV + kStatRelLV.m_byDefPhysicRelLV +
					kStatRelLV.m_byDefMagicRelLV + kStatRelLV.m_byBaseHPRelLV ) )
		{
			// ��� ���� 0�̶�� ��������� �ʱ�.
			return;
		}

		wstrItemDesc_ += L"#CFFFF0";
		wstrItemDesc_ += L"----------DEV-----------\n";
		wstrItemDesc_ += L"������ ��ũ��Ʈ�� �ɷ�ġ\n";

		WCHAR buff[256] = {0};
		vector<int> vecRandomSocketID;
		pItemTemplet->GetRandomSocketOptions(vecRandomSocketID);
		if( false == vecRandomSocketID.empty() )
		{
			wstrItemDesc_ += L"Random Socket GroupID : ";
			BOOST_FOREACH( int iSocketGroupID, vecRandomSocketID )
			{
				StringCchPrintf( buff, 256, L"%d,", iSocketGroupID );
				wstrItemDesc_ += buff;
			}
			wstrItemDesc_ += L"\n";
		}
		
		if( 0 != kStatRelLV.m_byAtkPhysicRelLV )
		{
			StringCchPrintf( buff, 256, L"AtkPhysic LV : %d\n", kStatRelLV.m_byAtkPhysicRelLV);
			wstrItemDesc_ += buff;
		}
		if( 0 != kStatRelLV.m_byAtkMagicRelLV )
		{
			StringCchPrintf( buff, 256, L"AtkMagic LV : %d\n", kStatRelLV.m_byAtkMagicRelLV);
			wstrItemDesc_ += buff;
		}
		if( 0 != kStatRelLV.m_byDefPhysicRelLV )
		{
			StringCchPrintf( buff, 256, L"DefPhysic LV : %d\n", kStatRelLV.m_byDefPhysicRelLV);
			wstrItemDesc_ += buff;
		}
		if( 0 != kStatRelLV.m_byDefMagicRelLV )
		{
			StringCchPrintf( buff, 256, L"DefMagic LV : %d\n", kStatRelLV.m_byDefMagicRelLV);
			wstrItemDesc_ += buff;
		}
		if( 0 != kStatRelLV.m_byBaseHPRelLV )
		{
			StringCchPrintf( buff, 256, L"HP LV : %d\n", kStatRelLV.m_byBaseHPRelLV );
			wstrItemDesc_ += buff;
		}

		wstrItemDesc_ += L"----------DEV-----------\n";
		wstrItemDesc_ += L"#CX";
	}
}

bool CX2ItemStatCalculator::IsRandomSocketOptionItem( const CX2Item::ItemTemplet* pItemTemplet ) const
{
	if( NULL == pItemTemplet )
		return false;

	if( 0 == pItemTemplet->GetNumRandomSocketOption() )
		return false;

	if( true == pItemTemplet->GetFashion() )
		return false;

	return true;
};

/** @function : GetSocketOptionStatRelLV
	@brief : ���� �߰� ���� ���� �ɼ� �� ������
*/
void CX2ItemStatCalculator::GetSocketOptionStatRelLV( OUT CX2SocketItem::KItemStatRelLVData& kItemStatRelLVData_,
	IN const CX2Item::ItemData& kItemData_, IN const CX2Item::ItemTemplet* pItemTemplet_) const
{
	kItemStatRelLVData_.Init();
	if( NULL != g_pData->GetSocketItem() )
	{
		// �����ۿ� �����Ǿ� �ִ� ���� �ɼ�
		if( NULL != pItemTemplet_ )
		{
			vector<int> vecSocketOption;
			pItemTemplet_->GetSocketOptions( vecSocketOption );
			BOOST_FOREACH( int iSocketID, vecSocketOption )
			{
				kItemStatRelLVData_.AddStat( g_pData->GetSocketItem()->GetStatRelLVDataBySocktID( iSocketID ) );
			}
		}

		{
			// ���������� �߰��ϴ� ���� �ɼ�
			BOOST_FOREACH( int iSocketID, kItemData_.m_SocketOption )
			{
				kItemStatRelLVData_.AddStat( g_pData->GetSocketItem()->GetStatRelLVDataBySocktID( iSocketID ) );
			}

			// ������ ���� ��� ���� ���� �ɼ�
			BOOST_FOREACH( int iSocketID, kItemData_.m_vecRandomSocket )
			{
				kItemStatRelLVData_.AddStat( g_pData->GetSocketItem()->GetStatRelLVDataBySocktID( iSocketID ) );
			}
		}
	}
}

#endif //SERV_NEW_ITEM_SYSTEM_2013_05