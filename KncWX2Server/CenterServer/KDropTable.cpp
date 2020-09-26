#include "KDropTable.h"
#include "X2Data/XSLItemManager.h"
//{{ 2012. 02. 16	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLBattleFieldManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
#include "X2Data/XSLDungeonManager.h"

ImplementRefreshSingleton( KDropTable );

KDropTable::KDropTable()
{
	//{{ 2009. 6. 17  ������	������� - �ϴ� �ϵ��ڵ�
	m_kPvpDrop.AddCase( CXSLItem::EI_CAMILLAS_COIN, 17.f ); // ī�ж��� ����
	//}}

	//{{ 2009. 6. 29  ������	��� ȹ�� ����
	m_bNotifyDropItem = false;
	//}}

#ifdef DUNGEON_ITEM
	::ZeroMemory( m_fPVPFailRate, sizeof(float) * PF_END );
	m_dfPVPItemChangeTime = 0;
#endif DUNGEON_ITEM
	//{{ 2011. 03. 28	������	���� ���� üũ
#ifdef SERV_MANY_ED_DROP_EVENT_NPC
	m_bIsDropManyEDEvent = false;
#endif SERV_MANY_ED_DROP_EVENT_NPC
	//}}
}

KDropTable::~KDropTable()
{
}

ImplToStringW( KDropTable )
{
	stm_	<< L"----------[ Drop Table ]----------" << std::endl
			<< TOSTRINGW( m_mapMonsterDrop.size() )
			<< TOSTRINGW( m_mapDropGroup.size() )
			<< TOSTRINGW( m_mapStaticDrop.size() )
			<< TOSTRINGW( m_mapHenirNpcDrop.size() )
			//{{ 2010. 05. 02  ������	��д��� ����
#ifdef SERV_SECRET_HELL
			<< TOSTRINGW( m_mapExtraStageNpcDrop.size() )
#endif SERV_SECRET_HELL
			//}}
			//{{ 2011. 11. 23	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
			<< TOSTRINGW( m_mapBattleFieldNpcDrop.size() )
			<< TOSTRINGW( m_mapBattleFieldStaticDrop.size() )
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			<< TOSTRINGW( m_vecTimeDrop.size() )
			<< TOSTRINGW( m_mapTimeDrop.size() )
			<< TOSTRINGW( m_kPvpDrop.GetTotalProb() )
			<< TOSTRINGW( m_setNotifyDropItem.size() )
			<< TOSTRINGW( m_bNotifyDropItem )
#ifdef DUNGEON_ITEM
			<< TOSTRINGW( m_vecPVPDropItem.size() )
			<< TOSTRINGW( m_mapPVPDropItem.size() )
			<< TOSTRINGW( m_fPVPFailRate[PF_WINTEAM] )
			<< TOSTRINGW( m_fPVPFailRate[PF_LOSETEAM] )
			<< TOSTRINGW( m_fPVPFailRate[PF_INC_NUM] )
			<< TOSTRINGW( m_fPVPFailRate[PF_DEC_NUM] )
			<< TOSTRINGW( m_dfPVPItemChangeTime )
			<< TOSTRINGW( m_mapAttribNpcSPDrop.size() )
#endif DUNGEON_ITEM
	//{{ 2009. 12. 18  ������	��������ġ����
#ifdef DUNGEON_EXP_NEW
			<< TOSTRINGW( m_mapNpcExpTable.size() )
#endif DUNGEON_EXP_NEW
	//}}
	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
			<< TOSTRINGW( m_mapComeBackBuffDropRate.size() )
#endif SERV_COME_BACK_USER_REWARD
	//}} 
	//{{ 2011. 08. 23	������	��ϸ� �ð� ����
#ifdef SERV_NEW_HENIR_TEST
			<< TOSTRINGW( m_mapHenirSpecialNpcDrop.size() )
#endif SERV_NEW_HENIR_TEST
	//}}
		;

	return stm_;
}

ImplementLuaScriptParser( KDropTable )
{
	lua_tinker::class_add<KDropTable>( GetLuaState(), "KDropTable" );
	lua_tinker::class_def<KDropTable>( GetLuaState(), "AddToGroup",						&KDropTable::AddToGroup_LUA );
	lua_tinker::class_def<KDropTable>( GetLuaState(), "AddMonsterDropInfo",				&KDropTable::AddMonsterDropInfo_LUA );
	//{{ 2009. 7. 25  ������	��ϸ� �ð�
	lua_tinker::class_def<KDropTable>( GetLuaState(), "AddHenirMonsterDropInfo",		&KDropTable::AddHenirMonsterDropInfo_LUA );
	//}}
	//{{ 2010. 05. 02  ������	��д��� ����
#ifdef SERV_SECRET_HELL
	lua_tinker::class_def<KDropTable>( GetLuaState(), "AddExtraStageMonsterDropInfo",	&KDropTable::AddExtraStageMonsterDropInfo_LUA );
#endif SERV_SECRET_HELL
	//}}	
	//{{ 2009. 4. 21  ������	��ü������̺�
	lua_tinker::class_def<KDropTable>( GetLuaState(), "AddStaticDropInfo",				&KDropTable::AddStaticDropInfo_LUA );
	//}}
	//{{ 2011. 11. 23	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
	lua_tinker::class_def<KDropTable>( GetLuaState(), "AddBattleFieldNpcDropInfo",		&KDropTable::AddBattleFieldNpcDropInfo_LUA );
	lua_tinker::class_def<KDropTable>( GetLuaState(), "AddBattleFieldStaticDropInfo",	&KDropTable::AddBattleFieldStaticDropInfo_LUA );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2009. 6. 29  ������	ȹ�� ����
	lua_tinker::class_def<KDropTable>( GetLuaState(), "AddNotifyDropItemInfo",			&KDropTable::AddNotifyDropItemInfo_LUA );
	lua_tinker::class_def<KDropTable>( GetLuaState(), "SetNotifyDropItem",				&KDropTable::SetNotifyDropItem_LUA );
	//}}
	lua_tinker::class_def<KDropTable>( GetLuaState(), "dump",							&KDropTable::Dump );
	lua_tinker::class_def<KDropTable>( GetLuaState(), "DumpGroup",						&KDropTable::DumpGroup );
	lua_tinker::class_def<KDropTable>( GetLuaState(), "DumpMonsterDrop",				&KDropTable::DumpMonsterDrop );

#ifdef DUNGEON_ITEM
	lua_tinker::class_def<KDropTable>( GetLuaState(), "SetPVPFailRate",					&KDropTable::SetPVPFailRate_LUA );
	lua_tinker::class_def<KDropTable>( GetLuaState(), "SetPVPItemChangeTime",			&KDropTable::SetPVPItemChangeTime_LUA );
	lua_tinker::class_def<KDropTable>( GetLuaState(), "AddPVPDropInfo",					&KDropTable::AddPVPDropInfo_LUA );
	//{{ 2009. 12. 18  ������	������������
	lua_tinker::class_def<KDropTable>( GetLuaState(), "AddAttribNpcSpecialDrop",		&KDropTable::AddAttribNpcSpecialDrop_LUA );
	//}}
#endif DUNGEON_ITEM
	//{{ 2009. 12. 18  ������	��������ġ����
#ifdef DUNGEON_EXP_NEW
	lua_tinker::class_def<KDropTable>( GetLuaState(), "AddNpcExpTable",					&KDropTable::AddNpcExpTable_LUA );
#endif DUNGEON_EXP_NEW
	//}}
	//{{ 2011. 03. 28	������	���� ���� üũ
#ifdef SERV_MANY_ED_DROP_EVENT_NPC
	lua_tinker::class_def<KDropTable>( GetLuaState(), "SetManyEDDropNpcEvent",			&KDropTable::SetManyEDDropNpcEvent_LUA );
	lua_tinker::class_def<KDropTable>( GetLuaState(), "AddManyEDDropNpc",				&KDropTable::AddManyEDDropNpc_LUA );
#endif SERV_MANY_ED_DROP_EVENT_NPC
	//}}

	//{{ 2011. 05. 04  ��μ�	���� Ŭ����� ������ ���� ���� �߰�
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	lua_tinker::class_def<KDropTable>( GetLuaState(), "AddPaymentDungeon",				&KDropTable::AddPaymentDungeon_LUA );

	//{{ 2012. 10. 17	�ڼ���	PaymentItem ���� ���� �߰�
#ifdef SERV_UNPAYMENT_CONDITION
	lua_tinker::class_def<KDropTable>( GetLuaState(), "AddUnPaymentCondition",			&KDropTable::AddUnPaymentCondition_LUA );
#endif SERV_UNPAYMENT_CONDITION
	//}}

#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}
	//{{ 2011. 08. 23	������	��ϸ� �ð� ����
#ifdef SERV_NEW_HENIR_TEST
	lua_tinker::class_def<KDropTable>( GetLuaState(), "AddHenirSpecialNpcDropInfo",		&KDropTable::AddHenirSpecialNpcDropInfo_LUA );
#endif SERV_NEW_HENIR_TEST
	//}}
	//{{ 2013. 1. 14	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
	lua_tinker::class_def<KDropTable>( GetLuaState(), "SetEnchantItemDropEvent",		&KDropTable::SetEnchantItemDropEvent_LUA );
#endif SERV_ENCHANT_ITEM_DROP_EVENT
	//}}

	lua_tinker::decl( GetLuaState(), "DropTable", this );
}

void KDropTable::DumpGroup()
{
    std::wostringstream stm;

    stm << L"  -- Item Group -- " << std::endl;
    std::map<int,KLottery>::const_iterator mit;
    for( mit = m_mapDropGroup.begin(); mit != m_mapDropGroup.end(); ++mit )
    {
        stm << L"  Group ID : " << mit->first << std::endl;
        mit->second.ToString( stm );
    }

    dbg::cout << stm.str();
}

void KDropTable::DumpMonsterDrop()
{
    std::wostringstream stm;

    stm << std::endl << L"  -- Monster Drop Info -- " << std::endl;
    std::map<std::pair<int,int>,KMonsterDropInfo>::const_iterator mit;
    for( mit = m_mapMonsterDrop.begin(); mit != m_mapMonsterDrop.end(); ++mit )
    {
        stm << L"  Dungeon,Monster key : " << mit->first.first << L", " << mit->first.second << std::endl
            << TOSTRINGW( mit->second.m_iExp )
            << TOSTRINGW( mit->second.m_iED )
            << TOSTRINGW( mit->second.m_fEDProperty );

        mit->second.m_kLottery.ToString( stm );
    }

    dbg::cout << stm.str();
}

#ifdef DUNGEON_ITEM
void KDropTable::SetPVPFailRate_LUA( IN int iEnumKey, IN float fRate )
{
	if( iEnumKey >= 0 && iEnumKey < PF_END )
	{
		m_fPVPFailRate[iEnumKey] = fRate;
	}
	else
	{
		START_LOG( cerr, L"���� ��Ӿ����� ���� ���� Ȯ�� ���� ����" )
			<< BUILD_LOG( iEnumKey )
			<< BUILD_LOG( fRate )
			<< END_LOG;
	}
}

void KDropTable::SetPVPItemChangeTime_LUA( float fTime )
{
	m_dfPVPItemChangeTime = static_cast<double>(fTime);
}

void KDropTable::AddPVPDropInfo_LUA( IN int iGroup, IN int iItemID, IN float fDropRate )
{
	std::map< int, KLottery >::iterator mit;
	mit = m_mapPVPDropItem.find( iGroup );
	if( mit == m_mapPVPDropItem.end() )
	{
		KLottery kLottery;
		kLottery.AddCaseIntegerCast( iItemID, fDropRate );
		m_mapPVPDropItem.insert( std::make_pair(iGroup,kLottery) );
	}
	else
	{
		mit->second.AddCaseIntegerCast( iItemID, fDropRate );
	}

	m_vecPVPDropItem.push_back( iItemID );
}

bool KDropTable::DecisionbyAllItem( OUT int& iGroup, OUT int& iItemID )
{
	iGroup = 0;
	iItemID = 0;

	if( m_mapPVPDropItem.size() <= 0 )
	{
		START_LOG( cerr, L"���� ��Ӿ����� �����Ͱ� ����.!" )
			<< BUILD_LOG( m_mapPVPDropItem.size() )
			<< END_LOG;

		return false;
	}

	int iSelGroup = rand() % static_cast<int>(m_mapPVPDropItem.size());

	std::map< int, KLottery >::iterator mit;
	mit = m_mapPVPDropItem.begin();
	for( int i = 0; i < static_cast<int>(m_mapPVPDropItem.size()); ++i )
	{
		if( i == iSelGroup )
		{
			int iDecision = mit->second.Decision();

			if( iDecision == KLottery::CASE_BLANK )
			{
				START_LOG( cerr, L"���� ��Ӿ����� ���Ȯ�� ���ÿ���.!" )
					<< END_LOG;
				return 0;
			}

			START_LOG( clog, L"���� ��� ������" )
				<< BUILD_LOG( iDecision )
				<< END_LOG;

			// ���� ����..
			iGroup = mit->first;//iSelGroup;
			iItemID = iDecision;

			return true;
		}

		++mit;
	}

	START_LOG( cerr, L"���� ��Ӿ����� ����.!" )
		<< END_LOG;

	return false;
}

bool KDropTable::DecisionbyGroupItem( IN int iGroup, OUT int& iItemID )
{
	iItemID = 0;

	std::map< int, KLottery >::iterator mit;
	mit = m_mapPVPDropItem.find( iGroup );

	if( mit == m_mapPVPDropItem.end() )
	{
		START_LOG( cerr, L"��ӵ� ������ �׷���̵� �̻���.!" )
			<< BUILD_LOG( iGroup )
			<< END_LOG;

		return false;
	}

	int iDecision = mit->second.Decision();

	if( iDecision == KLottery::CASE_BLANK )
	{
		START_LOG( cerr, L"���� ��Ӿ����� ���Ȯ�� ���ÿ���.!" )
			<< END_LOG;
		return false;
	}

	START_LOG( clog, L"���� ��� ������" )
		<< BUILD_LOG( iDecision )
		<< END_LOG;

	iItemID = iDecision;

	return true;
}

float KDropTable::GetPVPDropFailRate( PVP_FAILRATE eKey )
{
	if( eKey < 0 || 
		eKey >= PF_END )
	{
		START_LOG( cerr, L"���� ��Ӿ����� : ����Ȯ�� ã�� Ű�� �̻���." )
			<< BUILD_LOG( eKey )
			<< END_LOG;
		return 0.f;
	}

	return m_fPVPFailRate[eKey];

}

bool KDropTable::CheckPVPItem( int _iItemID )
{
	BOOST_TEST_FOREACH( const int&, iItemID, m_vecPVPDropItem )
	{
		if( _iItemID == iItemID )
			return true;
	}

	return false;
}

//{{ 2009. 12. 18  ������	������������
bool KDropTable::AddAttribNpcSpecialDrop_LUA( IN char cDifficulty, IN int iItemID, IN float fDropRate )
{
	if( cDifficulty < CXSLDungeon::DL_NORMAL  ||  
		cDifficulty > CXSLDungeon::DL_EXPERT  ||
		iItemID <= 0  ||
		fDropRate <= 0.0f )
	{
		START_LOG( cerr, L"�Ӽ� ���� ����� ��� ������ �̻��մϴ�." )
			<< BUILD_LOGc( cDifficulty )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( fDropRate )
			<< END_LOG;

		return false;
	}

	if( SiCXSLItemManager()->GetItemTemplet( iItemID ) == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� �������� NPC ����� ��� ���̺� ����Ͽ� ����." )
			<< BUILD_LOGc( cDifficulty )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( fDropRate )
			<< END_LOG;

		return false;
	}

	std::map< char, KLottery >::iterator mit = m_mapAttribNpcSPDrop.find( cDifficulty );
	if( mit == m_mapAttribNpcSPDrop.end() )
	{
        KLottery kLot;
		kLot.AddCase( iItemID, fDropRate );
		m_mapAttribNpcSPDrop.insert( std::make_pair( cDifficulty, kLot ) );
	}
	else
	{
        LIF( mit->second.AddCase( iItemID, fDropRate ) );
	}

	return true;
}

//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
bool KDropTable::AttribNpcSpecialDrop( IN char cDifficulty, IN float fUserContribution ,OUT std::vector< int >& vecDropItem )
{
	std::map< char, KLottery >::const_iterator mit = m_mapAttribNpcSPDrop.find( cDifficulty );
	if( mit == m_mapAttribNpcSPDrop.end() )
		return false;

	if( fUserContribution <= 0.0f )
		return true;

	KLottery kLott = mit->second;
	kLott.AddMultiProbRate( fUserContribution );

	const int iResultItemID = kLott.Decision();
	if( iResultItemID == KLottery::CASE_BLANK )	
		return true;

	vecDropItem.push_back( iResultItemID );
	return true;
}
#else
bool KDropTable::AttribNpcSpecialDrop( IN char cDifficulty, OUT std::vector< int >& vecDropItem )
{
	std::map< char, KLottery >::const_iterator mit = m_mapAttribNpcSPDrop.find( cDifficulty );
	if( mit == m_mapAttribNpcSPDrop.end() )
		return false;

	const int iResultItemID = mit->second.Decision();
	if( iResultItemID == KLottery::CASE_BLANK )	
		return true;

	vecDropItem.push_back( iResultItemID );
	return true;
}
#endif SERV_REFORM_ITEM_DROP
//}}

//}}

#endif DUNGEON_ITEM

//{{ 2009. 12. 18  ������	��������ġ����
#ifdef DUNGEON_EXP_NEW

int KDropTable::GetNpcExp( IN char cNpcLv )
{
	std::map< int, int >::const_iterator mit = m_mapNpcExpTable.find( static_cast<int>(cNpcLv) );
	if( mit == m_mapNpcExpTable.end() )
	{
        START_LOG( cwarn, L"��ϵǾ��������� Npc�����Դϴ�." )
			<< BUILD_LOGc( cNpcLv )
			<< END_LOG;

		return 0;
	}

	return mit->second;
}

#endif DUNGEON_EXP_NEW
//}}

void KDropTable::AddToGroup_LUA( int nGroupID, int nItemID, float fProbability )
{
    std::map<int,KLottery>::iterator mit;
    mit = m_mapDropGroup.find( nGroupID );

    if( mit == m_mapDropGroup.end() )  // ������ ��ϵ� ���� �׷� �����Ͱ� ���� - ���� �߰�.
    {
        m_mapDropGroup.insert( std::make_pair( nGroupID, KLottery() ) );
        mit = m_mapDropGroup.find( nGroupID );
    }

    if( !mit->second.AddCaseIntegerCast( nItemID, fProbability ) )
	{
		START_LOG( cerr, L"��� �׷� Ȯ���� �̻���!" )
			<< BUILD_LOG( nGroupID )
			<< BUILD_LOG( nItemID )
			<< BUILD_LOG( fProbability )
			<< BUILD_LOG( mit->second.GetTotalProb() )
			<< END_LOG;
	}
}

void KDropTable::AddMonsterDropInfo_LUA()
{
    KLuaManager luaMgr( GetLuaState() );

    KMonsterDropInfo kInfo;
    LUA_GET_VALUE( luaMgr, "DungeonID",     kInfo.m_nDungeonID, -1 );
    LUA_GET_VALUE( luaMgr, "MonsterID",     kInfo.m_nMonsterID, -1 );
    LUA_GET_VALUE( luaMgr, "Exp",           kInfo.m_iExp, -1 );
    LUA_GET_VALUE( luaMgr, "ED",            kInfo.m_iED, -1 );
    LUA_GET_VALUE( luaMgr, "EDProperty",    kInfo.m_fEDProperty, 0.0f );

	if( kInfo.m_nDungeonID == -1 && kInfo.m_nMonsterID == -1 && kInfo.m_nMonsterID == 0 )
    {
        START_LOG( cerr, L"Invalid key" )
            << BUILD_LOG( kInfo.m_nDungeonID )
            << BUILD_LOG( kInfo.m_nMonsterID );
        return;
    }

	//�̺�Ʈ ���̺� �ߺ� �˻�.
	if( kInfo.m_nDungeonID == 0 && kInfo.m_nMonsterID == 0 )
	{
		std::map<std::pair<int,int>,KMonsterDropInfo>::iterator mit;
		mit = m_mapMonsterDrop.find( std::make_pair( 0, 0 ) );

		if( mit != m_mapMonsterDrop.end() )
		{
			START_LOG( cerr, L"�̺�Ʈ ���̵� �ߺ����� ����." )
				<< END_LOG;

			return;
		}
	}

    // ��ǰ ������ ����Ʈ�� ������ �׷� ����Ʈ �ε� ó���� �ſ� �����ϰ� �Ǿ�������,
    // ��ũ��Ʈ������ magic number ���� ������ ���� �̰��� ó���Ͽ���.

    if( luaMgr.BeginTable( "DropItemList" ) == S_OK )   // ��ǰ ������ ����Ʈ Ȯ�� ����
    {
        int nTableIndex = 1;
        while( luaMgr.BeginTable( nTableIndex ) == S_OK )
        {
            int nItemID         = -1;
            float fProperty     = 0.0f;

            LUA_GET_VALUE( luaMgr, 1, nItemID, -1 );
            LUA_GET_VALUE( luaMgr, 2, fProperty, 0.0f );

			//{{ 2008. 10. 24  ������	��� �׷� üũ
			if( SiCXSLItemManager()->GetItemTemplet( nItemID ) == NULL )
			{
				START_LOG( cerr, L"�������� �ʴ� ItemID�� DropTable�� ���õǾ� �ֽ��ϴ�." )
					<< BUILD_LOG( kInfo.m_nDungeonID )
					<< BUILD_LOG( kInfo.m_nMonsterID )
					<< BUILD_LOG( nTableIndex )
					<< BUILD_LOG( nItemID )
					<< END_LOG;				
			}
			//}}

            if( kInfo.m_kLottery.AddCase( nItemID, fProperty, DCP_UNARY_ITEM ) == false )
			{
				START_LOG( cerr, L"Drop Item List" )
					<< BUILD_LOG( kInfo.m_nDungeonID )
					<< BUILD_LOG( kInfo.m_nMonsterID );
			}

            luaMgr.EndTable();

            nTableIndex++;
        } // end while

        luaMgr.EndTable();
    }

    if( luaMgr.BeginTable( "DropGroupList" ) == S_OK ) // ������ �׷� ����Ʈ Ȯ�� ����
    {
        int nTableIndex = 1;
        while( luaMgr.BeginTable( nTableIndex ) == S_OK )
        {
            int nGroupID    = -1;
            float fProperty = 0.0f;

            LUA_GET_VALUE( luaMgr, 1, nGroupID, -1 );
            LUA_GET_VALUE( luaMgr, 2, fProperty, 0.0f );

			//{{ 2008. 10. 24  ������	��� �׷� üũ
			if( m_mapDropGroup.find( nGroupID ) == m_mapDropGroup.end() )
			{
				START_LOG( cerr, L"�������� �ʴ� DropGroupID�� DropTable�� ���õǾ� �ֽ��ϴ�." )
					<< BUILD_LOG( kInfo.m_nDungeonID )
					<< BUILD_LOG( kInfo.m_nMonsterID )
					<< BUILD_LOG( nTableIndex )
					<< BUILD_LOG( nGroupID )
					<< END_LOG;
			}
			//}}

            if( kInfo.m_kLottery.AddCase( nGroupID, fProperty, DCP_ITEM_GROUP ) == false )
			{
				START_LOG( cerr, L"Drop Group List" )
					<< BUILD_LOG( kInfo.m_nDungeonID )
					<< BUILD_LOG( kInfo.m_nMonsterID );					
			}

            luaMgr.EndTable();

            nTableIndex++;
        } // end while

        luaMgr.EndTable();
    }

    // operator[]�� �ǵ����� ��� - ������ �ִ� ���� �����͸� ����� ������.
    m_mapMonsterDrop[ std::make_pair( kInfo.m_nDungeonID, kInfo.m_nMonsterID ) ] = kInfo;
}

//{{ 2009. 7. 25  ������	��ϸ��ð�
void KDropTable::AddHenirMonsterDropInfo_LUA()
{
	KLuaManager luaMgr( GetLuaState() );

	KHenirNpcDropInfo kInfo;
	LUA_GET_VALUE( luaMgr, "HenirDungeonMode",	kInfo.m_iHenirDungeonMode, -1 );
	LUA_GET_VALUE( luaMgr, "MonsterID",			kInfo.m_nMonsterID, -1 );
	LUA_GET_VALUE( luaMgr, "Exp",				kInfo.m_iExp, -1 );
	LUA_GET_VALUE( luaMgr, "ED",				kInfo.m_iED, -1 );
	LUA_GET_VALUE( luaMgr, "EDProperty",		kInfo.m_fEDProperty, 0.0f );

	if( kInfo.m_iHenirDungeonMode == -1 && kInfo.m_nMonsterID == -1 && kInfo.m_nMonsterID == 0 )
	{
		START_LOG( cerr, L"Invalid key" )
			<< BUILD_LOG( kInfo.m_iHenirDungeonMode )
			<< BUILD_LOG( kInfo.m_nMonsterID );
		return;
	}

	//�̺�Ʈ ���̺� �ߺ� �˻�.
	if( kInfo.m_iHenirDungeonMode == 0 && kInfo.m_nMonsterID == 0 )
	{
		std::map< std::pair< int, int >, KHenirNpcDropInfo >::const_iterator mit;
		mit = m_mapHenirNpcDrop.find( std::make_pair( 0, 0 ) );

		if( mit != m_mapHenirNpcDrop.end() )
		{
			START_LOG( cerr, L"�̺�Ʈ ���̵� �ߺ����� ����." )
				<< END_LOG;

			return;
		}
	}

	// ��ǰ ������ ����Ʈ�� ������ �׷� ����Ʈ �ε� ó���� �ſ� �����ϰ� �Ǿ�������,
	// ��ũ��Ʈ������ magic number ���� ������ ���� �̰��� ó���Ͽ���.

	if( luaMgr.BeginTable( "DropItemList" ) == S_OK )   // ��ǰ ������ ����Ʈ Ȯ�� ����
	{
		int nTableIndex = 1;
		while( luaMgr.BeginTable( nTableIndex ) == S_OK )
		{
			int nItemID         = -1;
			float fProperty     = 0.0f;

			LUA_GET_VALUE( luaMgr, 1, nItemID, -1 );
			LUA_GET_VALUE( luaMgr, 2, fProperty, 0.0f );

			//{{ 2008. 10. 24  ������	��� �׷� üũ
			if( SiCXSLItemManager()->GetItemTemplet( nItemID ) == NULL )
			{
				START_LOG( cerr, L"�������� �ʴ� ItemID�� DropTable�� ���õǾ� �ֽ��ϴ�." )
					<< BUILD_LOG( kInfo.m_iHenirDungeonMode )
					<< BUILD_LOG( kInfo.m_nMonsterID )
					<< BUILD_LOG( nTableIndex )
					<< BUILD_LOG( nItemID )
					<< END_LOG;				
			}
			//}}

			if( kInfo.m_kLottery.AddCase( nItemID, fProperty, DCP_UNARY_ITEM ) == false )
			{
				START_LOG( cerr, L"Drop Item List" )
					<< BUILD_LOG( kInfo.m_iHenirDungeonMode )
					<< BUILD_LOG( kInfo.m_nMonsterID );
			}

			luaMgr.EndTable();

			nTableIndex++;
		} // end while

		luaMgr.EndTable();
	}

	if( luaMgr.BeginTable( "DropGroupList" ) == S_OK ) // ������ �׷� ����Ʈ Ȯ�� ����
	{
		int nTableIndex = 1;
		while( luaMgr.BeginTable( nTableIndex ) == S_OK )
		{
			int nGroupID    = -1;
			float fProperty = 0.0f;

			LUA_GET_VALUE( luaMgr, 1, nGroupID, -1 );
			LUA_GET_VALUE( luaMgr, 2, fProperty, 0.0f );

			//{{ 2008. 10. 24  ������	��� �׷� üũ
			if( m_mapDropGroup.find( nGroupID ) == m_mapDropGroup.end() )
			{
				START_LOG( cerr, L"�������� �ʴ� DropGroupID�� DropTable�� ���õǾ� �ֽ��ϴ�." )
					<< BUILD_LOG( kInfo.m_iHenirDungeonMode )
					<< BUILD_LOG( kInfo.m_nMonsterID )
					<< BUILD_LOG( nTableIndex )
					<< BUILD_LOG( nGroupID )
					<< END_LOG;
			}
			//}}

			if( kInfo.m_kLottery.AddCase( nGroupID, fProperty, DCP_ITEM_GROUP ) == false )
			{
				START_LOG( cerr, L"Drop Group List" )
					<< BUILD_LOG( kInfo.m_iHenirDungeonMode )
					<< BUILD_LOG( kInfo.m_nMonsterID );					
			}

			luaMgr.EndTable();

			nTableIndex++;
		} // end while

		luaMgr.EndTable();
	}

	// operator[]�� �ǵ����� ��� - ������ �ִ� ���� �����͸� ����� ������.
	m_mapHenirNpcDrop[ std::make_pair( kInfo.m_iHenirDungeonMode, kInfo.m_nMonsterID ) ] = kInfo;
}
//}}

//{{ 2010. 05. 02  ������	��д��� ����
#ifdef SERV_SECRET_HELL

void KDropTable::AddExtraStageMonsterDropInfo_LUA()
{
	KLuaManager luaMgr( GetLuaState() );

	KExtraStageNpcDropInfo kInfo;
	LUA_GET_VALUE( luaMgr, "MonsterID",			kInfo.m_nMonsterID, -1 );
	LUA_GET_VALUE( luaMgr, "Exp",				kInfo.m_iExp, -1 );
	LUA_GET_VALUE( luaMgr, "ED",				kInfo.m_iED, -1 );
	LUA_GET_VALUE( luaMgr, "EDProperty",		kInfo.m_fEDProperty, 0.0f );

	if( kInfo.m_nMonsterID == -1 && kInfo.m_nMonsterID == 0 )
	{
		START_LOG( cerr, L"Invalid key" )
			<< BUILD_LOG( kInfo.m_nMonsterID );
		return;
	}

	//�̺�Ʈ ���̺� �ߺ� �˻�.	
	std::map< int, KExtraStageNpcDropInfo >::const_iterator mit;
	mit = m_mapExtraStageNpcDrop.find( kInfo.m_nMonsterID );
	if( mit != m_mapExtraStageNpcDrop.end() )
	{
		START_LOG( cerr, L"���� ID�� �ߺ����� ����." )
			<< BUILD_LOG( kInfo.m_nMonsterID )
			<< END_LOG;
		return;
	}	

	// ��ǰ ������ ����Ʈ�� ������ �׷� ����Ʈ �ε� ó���� �ſ� �����ϰ� �Ǿ�������,
	// ��ũ��Ʈ������ magic number ���� ������ ���� �̰��� ó���Ͽ���.

	if( luaMgr.BeginTable( "DropItemList" ) == S_OK )   // ��ǰ ������ ����Ʈ Ȯ�� ����
	{
		int nTableIndex = 1;
		while( luaMgr.BeginTable( nTableIndex ) == S_OK )
		{
			int nItemID         = -1;
			float fProperty     = 0.0f;

			LUA_GET_VALUE( luaMgr, 1, nItemID, -1 );
			LUA_GET_VALUE( luaMgr, 2, fProperty, 0.0f );

			//{{ 2008. 10. 24  ������	��� �׷� üũ
			if( SiCXSLItemManager()->GetItemTemplet( nItemID ) == NULL )
			{
				START_LOG( cerr, L"�������� �ʴ� ItemID�� DropTable�� ���õǾ� �ֽ��ϴ�." )					
					<< BUILD_LOG( kInfo.m_nMonsterID )
					<< BUILD_LOG( nTableIndex )
					<< BUILD_LOG( nItemID )
					<< END_LOG;				
			}
			//}}

			if( kInfo.m_kLottery.AddCase( nItemID, fProperty, DCP_UNARY_ITEM ) == false )
			{
				START_LOG( cerr, L"Drop Item List" )					
					<< BUILD_LOG( kInfo.m_nMonsterID );
			}

			luaMgr.EndTable();

			nTableIndex++;
		} // end while

		luaMgr.EndTable();
	}

	if( luaMgr.BeginTable( "DropGroupList" ) == S_OK ) // ������ �׷� ����Ʈ Ȯ�� ����
	{
		int nTableIndex = 1;
		while( luaMgr.BeginTable( nTableIndex ) == S_OK )
		{
			int nGroupID    = -1;
			float fProperty = 0.0f;

			LUA_GET_VALUE( luaMgr, 1, nGroupID, -1 );
			LUA_GET_VALUE( luaMgr, 2, fProperty, 0.0f );

			//{{ 2008. 10. 24  ������	��� �׷� üũ
			if( m_mapDropGroup.find( nGroupID ) == m_mapDropGroup.end() )
			{
				START_LOG( cerr, L"�������� �ʴ� DropGroupID�� DropTable�� ���õǾ� �ֽ��ϴ�." )					
					<< BUILD_LOG( kInfo.m_nMonsterID )
					<< BUILD_LOG( nTableIndex )
					<< BUILD_LOG( nGroupID )
					<< END_LOG;
			}
			//}}

			if( kInfo.m_kLottery.AddCase( nGroupID, fProperty, DCP_ITEM_GROUP ) == false )
			{
				START_LOG( cerr, L"Drop Group List" )					
					<< BUILD_LOG( kInfo.m_nMonsterID );					
			}

			luaMgr.EndTable();

			nTableIndex++;
		} // end while

		luaMgr.EndTable();
	}

	// operator[]�� �ǵ����� ��� - ������ �ִ� ���� �����͸� ����� ������.
	m_mapExtraStageNpcDrop[ kInfo.m_nMonsterID ] = kInfo;
}

#endif SERV_SECRET_HELL
//}}

//{{ 2009. 4. 21  ������	��ü������̺�
void KDropTable::AddStaticDropInfo_LUA()
{
	KLuaManager luaMgr( GetLuaState() );

	bool bEnable = true;
	KStaticDropInfo kInfo;
	LUA_GET_VALUE( luaMgr, "DungeonID",     kInfo.m_nDungeonID, -1 );
	LUA_GET_VALUE( luaMgr, "Enable",	    bEnable,			true );

	if( !bEnable )
		return;

	if( kInfo.m_nDungeonID == -1 )
	{
		START_LOG( cerr, L"Invalid key" )
			<< BUILD_LOG( kInfo.m_nDungeonID )
			<< END_LOG;
		return;
	}

	// DungeonID �ߺ� �˻�.
	std::map< int, KStaticDropInfo >::const_iterator mit;
	mit = m_mapStaticDrop.find( kInfo.m_nDungeonID );
	if( mit != m_mapStaticDrop.end() )
	{
		START_LOG( cerr, L"���� ���̵� �ߺ����� ����." )
			<< BUILD_LOG( kInfo.m_nDungeonID )
			<< END_LOG;
		return;
	}
	
	// ������̺�
	if( luaMgr.BeginTable( "DropItemList" ) == S_OK )   // ��ǰ ������ ����Ʈ Ȯ�� ����
	{
		int nTableIndex = 1;
		while( luaMgr.BeginTable( nTableIndex ) == S_OK )
		{
			int nItemID         = -1;
			float fProperty     = 0.0f;

			LUA_GET_VALUE( luaMgr, 1, nItemID, -1 );
			LUA_GET_VALUE( luaMgr, 2, fProperty, 0.0f );

			//{{ 2008. 10. 24  ������	��� �׷� üũ
			if( SiCXSLItemManager()->GetItemTemplet( nItemID ) == NULL )
			{
				START_LOG( cerr, L"�������� �ʴ� ItemID�� DropTable�� ���õǾ� �ֽ��ϴ�." )
					<< BUILD_LOG( kInfo.m_nDungeonID )					
					<< BUILD_LOG( nTableIndex )
					<< BUILD_LOG( nItemID )
					<< END_LOG;				
			}
			//}}

			if( kInfo.m_kLottery.AddCase( nItemID, fProperty, DCP_UNARY_ITEM ) == false )
			{
				START_LOG( cerr, L"Drop Item List" )
					<< BUILD_LOG( kInfo.m_nDungeonID )
					<< END_LOG;
			}

			luaMgr.EndTable();

			nTableIndex++;
		} // end while

		luaMgr.EndTable();
	}

	// operator[]�� �ǵ����� ��� - ������ �ִ� ���� �����͸� ����� ������.
	m_mapStaticDrop.insert( std::make_pair( kInfo.m_nDungeonID, kInfo ) );
}
//}}

//{{ 2011. 11. 23	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KDropTable::AddBattleFieldNpcDropInfo_LUA()
{
	KLuaManager luaMgr( GetLuaState() );

	KBattleFieldNpcDropInfo kInfo;
	LUA_GET_VALUE( luaMgr, "BattleFieldID", kInfo.m_nBattleFieldID, -1 );
	LUA_GET_VALUE( luaMgr, "MonsterID",     kInfo.m_nMonsterID, -1 );
	LUA_GET_VALUE( luaMgr, "Exp",           kInfo.m_iExp, -1 );
	LUA_GET_VALUE( luaMgr, "ED",            kInfo.m_iED, -1 );
	LUA_GET_VALUE( luaMgr, "EDProperty",    kInfo.m_fEDProperty, 0.0f );

	if( kInfo.m_nBattleFieldID == -1 && kInfo.m_nMonsterID == -1 && kInfo.m_nMonsterID == 0 )
	{
		START_LOG( cerr, L"Invalid key" )
			<< BUILD_LOG( kInfo.m_nBattleFieldID )
			<< BUILD_LOG( kInfo.m_nMonsterID );
		return;
	}

	//�̺�Ʈ ���̺� �ߺ� �˻�.
	if( kInfo.m_nBattleFieldID == 0 && kInfo.m_nMonsterID == 0 )
	{
		std::map<std::pair<int,int>,KBattleFieldNpcDropInfo>::iterator mit;
		mit = m_mapBattleFieldNpcDrop.find( std::make_pair( 0, 0 ) );

		if( mit != m_mapBattleFieldNpcDrop.end() )
		{
			START_LOG( cerr, L"�̺�Ʈ ���̵� �ߺ����� ����." )
				<< END_LOG;

			return;
		}
	}

	// ��ǰ ������ ����Ʈ�� ������ �׷� ����Ʈ �ε� ó���� �ſ� �����ϰ� �Ǿ�������,
	// ��ũ��Ʈ������ magic number ���� ������ ���� �̰��� ó���Ͽ���.

	if( luaMgr.BeginTable( "DropItemList" ) == S_OK )   // ��ǰ ������ ����Ʈ Ȯ�� ����
	{
		int nTableIndex = 1;
		while( luaMgr.BeginTable( nTableIndex ) == S_OK )
		{
			int nItemID         = -1;
			float fProperty     = 0.0f;

			LUA_GET_VALUE( luaMgr, 1, nItemID, -1 );
			LUA_GET_VALUE( luaMgr, 2, fProperty, 0.0f );

			//{{ 2008. 10. 24  ������	��� �׷� üũ
			if( SiCXSLItemManager()->GetItemTemplet( nItemID ) == NULL )
			{
				START_LOG( cerr, L"�������� �ʴ� ItemID�� DropTable�� ���õǾ� �ֽ��ϴ�." )
					<< BUILD_LOG( kInfo.m_nBattleFieldID )
					<< BUILD_LOG( kInfo.m_nMonsterID )
					<< BUILD_LOG( nTableIndex )
					<< BUILD_LOG( nItemID )
					<< END_LOG;				
			}
			//}}

			if( kInfo.m_kLottery.AddCase( nItemID, fProperty, DCP_UNARY_ITEM ) == false )
			{
				START_LOG( cerr, L"Drop Item List" )
					<< BUILD_LOG( kInfo.m_nBattleFieldID )
					<< BUILD_LOG( kInfo.m_nMonsterID );
			}

			luaMgr.EndTable();

			nTableIndex++;
		} // end while

		luaMgr.EndTable();
	}

	if( luaMgr.BeginTable( "DropGroupList" ) == S_OK ) // ������ �׷� ����Ʈ Ȯ�� ����
	{
		int nTableIndex = 1;
		while( luaMgr.BeginTable( nTableIndex ) == S_OK )
		{
			int nGroupID    = -1;
			float fProperty = 0.0f;

			LUA_GET_VALUE( luaMgr, 1, nGroupID, -1 );
			LUA_GET_VALUE( luaMgr, 2, fProperty, 0.0f );

			//{{ 2008. 10. 24  ������	��� �׷� üũ
			if( m_mapDropGroup.find( nGroupID ) == m_mapDropGroup.end() )
			{
				START_LOG( cerr, L"�������� �ʴ� DropGroupID�� DropTable�� ���õǾ� �ֽ��ϴ�." )
					<< BUILD_LOG( kInfo.m_nBattleFieldID )
					<< BUILD_LOG( kInfo.m_nMonsterID )
					<< BUILD_LOG( nTableIndex )
					<< BUILD_LOG( nGroupID )
					<< END_LOG;
			}
			//}}

			if( kInfo.m_kLottery.AddCase( nGroupID, fProperty, DCP_ITEM_GROUP ) == false )
			{
				START_LOG( cerr, L"Drop Group List" )
					<< BUILD_LOG( kInfo.m_nBattleFieldID )
					<< BUILD_LOG( kInfo.m_nMonsterID );					
			}

			luaMgr.EndTable();

			nTableIndex++;
		} // end while

		luaMgr.EndTable();
	}

	// operator[]�� �ǵ����� ��� - ������ �ִ� ���� �����͸� ����� ������.
	m_mapBattleFieldNpcDrop[ std::make_pair( kInfo.m_nBattleFieldID, kInfo.m_nMonsterID ) ] = kInfo;
}

void KDropTable::AddBattleFieldStaticDropInfo_LUA()
{
	KLuaManager luaMgr( GetLuaState() );

	bool bEnable = true;
	KBattleFieldStaticDropInfo kInfo;
	LUA_GET_VALUE( luaMgr, "BattleFieldID",     kInfo.m_nBattleFieldID, -1 );
	LUA_GET_VALUE( luaMgr, "Enable",	    bEnable,			true );

	if( !bEnable )
		return;

	if( kInfo.m_nBattleFieldID == -1 )
	{
		START_LOG( cerr, L"Invalid key" )
			<< BUILD_LOG( kInfo.m_nBattleFieldID )
			<< END_LOG;
		return;
	}

	// DungeonID �ߺ� �˻�.
	std::map< int, KBattleFieldStaticDropInfo >::const_iterator mit;
	mit = m_mapBattleFieldStaticDrop.find( kInfo.m_nBattleFieldID );
	if( mit != m_mapBattleFieldStaticDrop.end() )
	{
		START_LOG( cerr, L"���� ���̵� �ߺ����� ����." )
			<< BUILD_LOG( kInfo.m_nBattleFieldID )
			<< END_LOG;
		return;
	}

	// ������̺�
	if( luaMgr.BeginTable( "DropItemList" ) == S_OK )   // ��ǰ ������ ����Ʈ Ȯ�� ����
	{
		int nTableIndex = 1;
		while( luaMgr.BeginTable( nTableIndex ) == S_OK )
		{
			int nItemID         = -1;
			float fProperty     = 0.0f;

			LUA_GET_VALUE( luaMgr, 1, nItemID, -1 );
			LUA_GET_VALUE( luaMgr, 2, fProperty, 0.0f );

			//{{ 2008. 10. 24  ������	��� �׷� üũ
			if( SiCXSLItemManager()->GetItemTemplet( nItemID ) == NULL )
			{
				START_LOG( cerr, L"�������� �ʴ� ItemID�� DropTable�� ���õǾ� �ֽ��ϴ�." )
					<< BUILD_LOG( kInfo.m_nBattleFieldID )					
					<< BUILD_LOG( nTableIndex )
					<< BUILD_LOG( nItemID )
					<< END_LOG;				
			}
			//}}

			if( kInfo.m_kLottery.AddCase( nItemID, fProperty, DCP_UNARY_ITEM ) == false )
			{
				START_LOG( cerr, L"Drop Item List" )
					<< BUILD_LOG( kInfo.m_nBattleFieldID )
					<< END_LOG;
			}

			luaMgr.EndTable();

			nTableIndex++;
		} // end while

		luaMgr.EndTable();
	}

	// operator[]�� �ǵ����� ��� - ������ �ִ� ���� �����͸� ����� ������.
	m_mapBattleFieldStaticDrop.insert( std::make_pair( kInfo.m_nBattleFieldID, kInfo ) );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2009. 6. 29  ������	��Ӱ��� ������
bool KDropTable::AddNotifyDropItemInfo_LUA( int iItemID )
{
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� �������� ���ȹ����� ���������� ����Ϸ� �߽��ϴ�!" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		return false;
	}

	m_setNotifyDropItem.insert( iItemID );
	return true;
}
//}}

//{{ 2009. 12. 18  ������	��������ġ����
#ifdef DUNGEON_EXP_NEW
bool KDropTable::AddNpcExpTable_LUA( int iNpcLv, int iEXP )
{
	if( iNpcLv <= 0  ||  iEXP <= 0 )
	{
		START_LOG( cerr, L"npc ����ġ ���ð��� �̻��մϴ�." )
			<< BUILD_LOG( iNpcLv )
			<< BUILD_LOG( iEXP )
			<< END_LOG;

		return false;
	}

	m_mapNpcExpTable.insert( std::make_pair( iNpcLv, iEXP ) );
	return true;
}
#endif DUNGEON_EXP_NEW
//}}

//{{ 2011. 03. 28	������	���� ���� üũ
#ifdef SERV_MANY_ED_DROP_EVENT_NPC
void KDropTable::SetManyEDDropNpcEvent_LUA( bool bVal )
{
    m_bIsDropManyEDEvent = bVal;

	START_LOG( cout, L"ED ���� ����ϴ� NPC�̺�Ʈ ����! : " )
		<< BUILD_LOG( m_bIsDropManyEDEvent );
}

bool KDropTable::AddManyEDDropNpc_LUA( int iNpcID, float fMultiplyDropED, float fMultiplyDropEDCount )
{
	if( iNpcID <= 0  ||  fMultiplyDropED <= 0  ||  fMultiplyDropEDCount <= 0 )
	{
		START_LOG( cerr, L"many ed drop ���ð��� �̻��մϴ�." )
			<< BUILD_LOG( iNpcID )
			<< BUILD_LOG( fMultiplyDropED )
			<< BUILD_LOG( fMultiplyDropEDCount )
			<< END_LOG;

		return false;
	}

    KManyEDDropNpcInfo kInfo;
	kInfo.m_fMultiplyDropED = fMultiplyDropED;
	kInfo.m_fMultiplyDropEDCount = fMultiplyDropEDCount;
	m_mapManyEDDropTable.insert( std::make_pair( iNpcID, kInfo ) );
	return true;
}
#endif SERV_MANY_ED_DROP_EVENT_NPC
//}}

int KDropTable::GetEDItemID( int nED ) const
{
    // ���� �� �Լ��� ���� ������ �����ϵ��� script ȭ �Ѵ�.

	int iEDItemID = 0;

	if( 0 < nED )	{	iEDItemID = CXSLItem::EDI_BRONZE_ED;	}
	if( 50 < nED )	{	iEDItemID = CXSLItem::EDI_SILVER_ED;	}
	if( 150 < nED )	{	iEDItemID = CXSLItem::EDI_GOLD_ED;	}
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-15	// �ڼ���
	if( 1999 < nED )	{	iEDItemID = CXSLItem::EDI_GOLD_BAR;	}
#endif // SERV_BATTLE_FIELD_BOSS

	return iEDItemID;
}

//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
bool KDropTable::NpcDropItem( IN const int iNpcID,
							 IN const int iDropEXP,
							 IN const float fEDProperty,
							 IN const int iDropED,
							 IN const KLottery& kLot,
							 IN const float fLevelFactor,
							 IN const float fDungeonFactor,
							 IN const float fContribution,
							 IN const int iPartyUserCount_,
							 IN const float fPartyDropBonus,
							 //{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
							 IN const float fDropRate,
#else // SERV_DROP_EVENT_RENEWAL
							 IN const int iDropCount,
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
							 //}}
							 OUT DROP_DATA& sDropData,
							 IN const bool bIsBoss )
{
	//////////////////////////////////////////////////////////////////////////
	// X = ������ �ص����� / ����HP (X ���� �ְ� 1.5�� ���� �ʴ´�)
	// Y = NPC�� ���� �ִ� EXP
	// Z = NPC�� ���� �ִ� ED
	// B = ��Ƽ ���ʽ� = NPC���� ����ġ / ������ ������ �ִ¡� ��Ƽ�� �� * ������ ������ �ִ¡� ��Ƽ�� ���� ���� ��Ƽ�� ��� * ���ſ����� ������ ���̵� ���
	// A = �ʵ� ��� = SiCXSLBattleFieldManager()->GetBattleFieldFactor()
	// F = ���� ���� = ���Ϳ��� �������� +-10���� �̻��̸�, ������ ȹ���� �� ����
	// T = ��� ���̺� Ȯ��
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// EXP, ED

	// EXP = Y * X * A * F
	// ED =  Z * X * A * F
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ��� ������

	// ������ ���(ȹ��) Ȯ�� = T * X * A * ( B * 0.5 ) * F
	//////////////////////////////////////////////////////////////////////////

	sDropData.Clear();

	//////////////////////////////////////////////////////////////////////////	
	// NPC�κ��� ��ԵǴ� EXP
	sDropData.m_iExp = (int)(iDropEXP * fContribution * fDungeonFactor * fLevelFactor);	// fDungeonFactor -> ���ͻ��(�Ϲ�,����Ʈ....����)

	//////////////////////////////////////////////////////////////////////////	
	// NPC�κ��� ��ԵǴ� ED
	//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
	int iGetTotalED = static_cast<int>( iDropED * fContribution * fDungeonFactor * fLevelFactor * fDropRate );
#else // SERV_DROP_EVENT_RENEWAL
	int iGetTotalED = (int)(iDropED * fContribution * fDungeonFactor * fLevelFactor) * iDropCount;
#endif // SERV_DROP_EVENT_RENEWAL
#else
	int iGetTotalED = (int)(iDropED * fContribution * fDungeonFactor * fLevelFactor);
#endif SERV_ITEM_DROP_EVENT
	//}}

	//////////////////////////////////////////////////////////////////////////	
	// NPC�κ��� ��ԵǴ� ED
	if( ( (float)rand() / RAND_MAX ) < fEDProperty )
	{
		if( true == bIsBoss )
		{
			sDropData.m_nGPNum		= ( rand() % 11 + 10 ); // [10,20]
			sDropData.m_iED			= ( sDropData.m_nGPNum > 0 ? iGetTotalED / sDropData.m_nGPNum : 0 );
			sDropData.m_iGPItemID	= GetEDItemID( sDropData.m_iED );
		}
		else
		{
			sDropData.m_nGPNum      = rand() % 2 + 2;    // [2,3]
			sDropData.m_iED			= ( sDropData.m_nGPNum > 0 ? iGetTotalED / sDropData.m_nGPNum : 0 );
			sDropData.m_iGPItemID   = GetEDItemID( sDropData.m_iED );
		}

		KManyEDDropNpcInfo kManyDropInfo;
		if( GetManyEDDropNpcInfo( iNpcID, kManyDropInfo ) == true )
		{
			sDropData.m_nGPNum		= static_cast<int>(static_cast<float>(sDropData.m_nGPNum) * kManyDropInfo.m_fMultiplyDropEDCount);
			sDropData.m_iED			= static_cast<int>(static_cast<float>(sDropData.m_iED) * kManyDropInfo.m_fMultiplyDropED);
			sDropData.m_iGPItemID	= GetEDItemID( sDropData.m_iED );
		}
	}

	//////////////////////////////////////////////////////////////////////////	
	// ������ ������.
	// 	T = ������̺� Ȯ��
	// 	X = �ش������� �ص����� / ����HP (X ���� �ְ� 1.5�� ���� �ʴ´�)
	// 	A = �ʵ� ���
	// 	C = ������ ������ �ִ¡� ��Ƽ�� ���� ���� ��Ƽ�� ��� (0, 0.5, 1, 1.5)
	// 	F = �����������
	// 	������ ���(ȹ��) Ȯ�� = T * X * A * ( 1 + C ) * F

	if( fContribution <= 0.0f )
		return true;

	// ������ ����� ���� ���� ��� ���� ����ϱ�� ����
	//if( fLevelFactor <= 0.0f )
	//	return true;

	KLottery kDropInfoCopy = kLot;
	kDropInfoCopy.AddMultiProbRate( fContribution * iPartyUserCount_ );
//	kDropInfoCopy.AddMultiProbRate( fPartyDropBonus );

	//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT

#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
	{
		float fDropRateCopy = fDropRate;

		while( 0.0f < fDropRateCopy )
		{
			if( fDropRateCopy < 1.0f )
			{
				kDropInfoCopy.AddMultiProbRate( fDropRateCopy );
				fDropRateCopy = 0.0f;
			}
			else
			{
				fDropRateCopy -= 1.0f;
			}

			const int nCaseID = kDropInfoCopy.Decision();
			if( nCaseID == KLottery::CASE_BLANK )   // ȹ���� ������ ����.
				continue;

			const int nCaseParam = kDropInfoCopy.GetParam1( nCaseID );

			switch( nCaseParam )    // ��ǰ ������, ������ �׷�, Ȥ�� ���� ���� ����.
			{
			case DCP_UNARY_ITEM:    // ��ǰ ������
				{
					sDropData.m_vecItemID.push_back( nCaseID );
				}			
				break;
			case DCP_ITEM_GROUP:    // ������ �׷�.
				{
					std::map< int, KLottery >::const_iterator mit;
					mit = m_mapDropGroup.find( nCaseID );
					if( mit == m_mapDropGroup.end() )
					{
						START_LOG( cerr, L"������ �׷� ���� ����" )
							<< BUILD_LOG( nCaseID )
							<< BUILD_LOG( nCaseParam )
							<< BUILD_LOG( m_mapDropGroup.size() );
						continue;
					}

					// ������ �׷� �߿��� ���� ������ �ϳ� ����.
					const int iResultItemID = mit->second.Decision();
					if( iResultItemID == KLottery::CASE_BLANK )   // ȹ���� ������ ����.
						continue;

					sDropData.m_vecItemID.push_back( iResultItemID );
				}
				break;
			default:
				{
					START_LOG( cerr, L"Never Get Here ~" )
						<< BUILD_LOG( nCaseParam )
						<< BUILD_LOG( nCaseID )
						<< BUILD_LOG( kDropInfoCopy.GetCaseNum() )
						<< BUILD_LOG( kDropInfoCopy.GetTotalProb() )
						<< END_LOG;
					continue;
				}
			}
		}
	}
#else // SERV_DROP_EVENT_RENEWAL
	for( int iCount = 0 ; iCount < iDropCount ; ++iCount )
	{
		const int nCaseID = kDropInfoCopy.Decision();
		if( nCaseID == KLottery::CASE_BLANK )   // ȹ���� ������ ����.
			continue;

		const int nCaseParam = kDropInfoCopy.GetParam1( nCaseID );

		switch( nCaseParam )    // ��ǰ ������, ������ �׷�, Ȥ�� ���� ���� ����.
		{
		case DCP_UNARY_ITEM:    // ��ǰ ������
			{
				sDropData.m_vecItemID.push_back( nCaseID );
			}			
			break;
		case DCP_ITEM_GROUP:    // ������ �׷�.
			{
				std::map< int, KLottery >::const_iterator mit;
				mit = m_mapDropGroup.find( nCaseID );
				if( mit == m_mapDropGroup.end() )
				{
					START_LOG( cerr, L"������ �׷� ���� ����" )
						<< BUILD_LOG( nCaseID )
						<< BUILD_LOG( nCaseParam )
						<< BUILD_LOG( m_mapDropGroup.size() );
					continue;
				}

				// ������ �׷� �߿��� ���� ������ �ϳ� ����.
				const int iResultItemID = mit->second.Decision();
				if( iResultItemID == KLottery::CASE_BLANK )   // ȹ���� ������ ����.
					continue;

				sDropData.m_vecItemID.push_back( iResultItemID );
			}
			break;
		default:
			{
				START_LOG( cerr, L"Never Get Here ~" )
					<< BUILD_LOG( nCaseParam )
					<< BUILD_LOG( nCaseID )
					<< BUILD_LOG( kDropInfoCopy.GetCaseNum() )
					<< BUILD_LOG( kDropInfoCopy.GetTotalProb() )
					<< END_LOG;
				continue;
			}
		}
	}
#endif // SERV_DROP_EVENT_RENEWAL

#else
	const int nCaseID = kDropInfoCopy.Decision();
	if( nCaseID == KLottery::CASE_BLANK )   // ȹ���� ������ ����.
		return true;

	const int nCaseParam = kDropInfoCopy.GetParam1( nCaseID );

	switch( nCaseParam )    // ��ǰ ������, ������ �׷�, Ȥ�� ���� ���� ����.
	{
	case DCP_UNARY_ITEM:    // ��ǰ ������
		{
			sDropData.m_vecItemID.push_back( nCaseID );
		}			
		break;
	case DCP_ITEM_GROUP:    // ������ �׷�.
		{
			std::map< int, KLottery >::const_iterator mit;
			mit = m_mapDropGroup.find( nCaseID );
			if( mit == m_mapDropGroup.end() )
			{
				START_LOG( cerr, L"������ �׷� ���� ����" )
					<< BUILD_LOG( nCaseID )
					<< BUILD_LOG( nCaseParam )
					<< BUILD_LOG( m_mapDropGroup.size() );
				return false;
			}

			// ������ �׷� �߿��� ���� ������ �ϳ� ����.
			const int iResultItemID = mit->second.Decision();
			if( iResultItemID == KLottery::CASE_BLANK )   // ȹ���� ������ ����.
				break;

			sDropData.m_vecItemID.push_back( iResultItemID );
		}
		break;
	default:
		START_LOG( cerr, L"Never Get Here ~" )
			<< BUILD_LOG( nCaseParam )
			<< BUILD_LOG( nCaseID )
			<< BUILD_LOG( kDropInfoCopy.GetCaseNum() )
			<< BUILD_LOG( kDropInfoCopy.GetTotalProb() )
			<< END_LOG;
		return false;
	}
#endif SERV_ITEM_DROP_EVENT
	//}}
	
	return true;
}

#else		// SERV_REFORM_ITEM_DROP

bool KDropTable::NpcDropItem( IN const int iNpcID, IN const int iDropEXP, IN const float fEDProperty, IN const int iDropED, IN const KLottery& kLot, IN const char cNpcLevel, IN const char cMonsterTypeFactor, OUT DROP_DATA& sDropData, IN const int iItemDropNum, IN const bool bIsBoss )
{
	// NPC�κ��� ��ԵǴ� EXP
	sDropData.m_iExp = iDropEXP;

	// NPC�κ��� ��ԵǴ� ED
	if( (float)rand() / RAND_MAX < fEDProperty )
	{
		//{{ 2008. 11. 17  ������	EDȹ�� ����		
		const float fPercent = (float)( ( rand() % 20 ) + 90 ) / 100.f;

		//{{ 2012. 03. 28	�ڼ���	��ӷ� ���� �̺�Ʈ ���� Merge �۾� ( 2012.03.20 lygan_������ )
#ifdef DROPEVENT_RENEWAL
		const int iGetTotalED = (int)( (float)( 10 + ( iDropED * (int)cNpcLevel ) ) * fPercent * (float)cMonsterTypeFactor * iItemDropNum );
#else
		const int iGetTotalED = (int)( (float)( 10 + ( iDropED * (int)cNpcLevel ) ) * fPercent * (float)cMonsterTypeFactor );
#endif DROPEVENT_RENEWAL
		//}}

		if( true == bIsBoss )
		{
			sDropData.m_nGPNum		= ( rand() % 11 + 10 ); // [10,20]
			sDropData.m_iED			= ( sDropData.m_nGPNum > 0 ? iGetTotalED / sDropData.m_nGPNum : 0 );
			sDropData.m_iGPItemID	= GetEDItemID( sDropData.m_iED );
		}
		else
		{
			sDropData.m_nGPNum      = rand() % 2 + 2;    // [2,3]
			sDropData.m_iED			= ( sDropData.m_nGPNum > 0 ? iGetTotalED / sDropData.m_nGPNum : 0 );
			sDropData.m_iGPItemID   = GetEDItemID( sDropData.m_iED );
		}			

		//{{ 2011. 03. 28	������	���� ���� üũ
#ifdef SERV_MANY_ED_DROP_EVENT_NPC
		KManyEDDropNpcInfo kManyDropInfo;
		if( GetManyEDDropNpcInfo( iNpcID, kManyDropInfo ) == true )
		{
			sDropData.m_nGPNum		= static_cast<int>(static_cast<float>(sDropData.m_nGPNum) * kManyDropInfo.m_fMultiplyDropEDCount);
			sDropData.m_iED			= static_cast<int>(static_cast<float>(sDropData.m_iED) * kManyDropInfo.m_fMultiplyDropED);
			sDropData.m_iGPItemID	= GetEDItemID( sDropData.m_iED );
		}
#endif SERV_MANY_ED_DROP_EVENT_NPC
		//}}
	}

	// ������ ������.
	for( int i = 0; i < iItemDropNum; ++i )
	{
		const int nCaseID = kLot.Decision();
		if( nCaseID == KLottery::CASE_BLANK )   // ȹ���� ������ ����.
			return true;

		const int nCaseParam = kLot.GetParam1( nCaseID );

		switch( nCaseParam )    // ��ǰ ������, ������ �׷�, Ȥ�� ���� ���� ����.
		{
		case DCP_UNARY_ITEM:    // ��ǰ ������
			{
				sDropData.m_vecItemID.push_back( nCaseID );
			}			
			break;
		case DCP_ITEM_GROUP:    // ������ �׷�.
			{
				std::map< int, KLottery >::const_iterator mit;
				mit = m_mapDropGroup.find( nCaseID );
				if( mit == m_mapDropGroup.end() )
				{
					START_LOG( cerr, L"������ �׷� ���� ����" )
						<< BUILD_LOG( nCaseID )
						<< BUILD_LOG( nCaseParam )
						<< BUILD_LOG( m_mapDropGroup.size() );
					return false;
				}

				// ������ �׷� �߿��� ���� ������ �ϳ� ����.
				const int iResultItemID = mit->second.Decision();
				if( iResultItemID == KLottery::CASE_BLANK )   // ȹ���� ������ ����.
					break;

				sDropData.m_vecItemID.push_back( iResultItemID );
			}
			break;
		default:
			START_LOG( cerr, L"Never Get Here ~" )
				<< BUILD_LOG( nCaseParam )
				<< BUILD_LOG( nCaseID )
				<< BUILD_LOG( kLot.GetCaseNum() )
				<< BUILD_LOG( kLot.GetTotalProb() )
				<< END_LOG;
			return false;
		}
	}

	return true;
}
#endif SERV_REFORM_ITEM_DROP
//}}

//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
bool KDropTable::NormalNpcDropItem( IN const int nDungeonID, 
								    IN const int iNpcID, 
									IN const char cNpcLevel,
								    IN const float fLevelFactor,
								    IN const float fDungeonFactor, 
								    IN const float fContribution,
								    IN const float fPartyDropBonus,
									IN const int iPartyUserCount,
									//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
									IN const float fDropRate,
#else // SERV_DROP_EVENT_RENEWAL
									IN const int iDropCount,
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
									//}}
									OUT bool& bDecreaseEndurance,
								    OUT DROP_DATA& sDropData, 
								    IN const bool bIsBoss )
{
	sDropData.Clear();

	// �켱 �ش� DungeonID + MonsterID�� �ش��ϴ� ��� ������ �˻��Ѵ�.
	std::map< std::pair< int, int >, KMonsterDropInfo >::const_iterator mit;
	mit = m_mapMonsterDrop.find( std::make_pair( nDungeonID, iNpcID ) );

	// �ش� ��쿡 ���� ������ �������� ����.
	if( mit == m_mapMonsterDrop.end() )
	{
		// ���� ���̵� ������ ���� ���� ������ �����ϴ��� �ٽ� �ѹ� Ȯ��.
		mit = m_mapMonsterDrop.find( std::make_pair( 0, iNpcID ) );

		// �װ͸��� ���ٸ�?
		if( mit == m_mapMonsterDrop.end() )
		{
			// �������� ����. �����϶��� false.
			return true;
		}
	}

	KMonsterDropInfo kDropInfo = mit->second;

	// ��� ���̺� ����ġ ������ ���õ� ���͸� ����ġ�� �ش�.
	if( kDropInfo.m_iExp > 0 )
	{
		kDropInfo.m_iExp = GetNpcExp( cNpcLevel );

		// ��� ���̺� ����ġ ������ ���õ� ���ʹ� ������ �������� ���� ��Ų��!
		bDecreaseEndurance = true;
	}

	if( kDropInfo.m_iED > 0 )
	{
		kDropInfo.m_iED = (10 + ( kDropInfo.m_iED * cNpcLevel ));
	}

	// ���� ���� �̶�� fLevelFactor �� 1.0f �� �����ȴ�.
	float fCheckLevelFactor = fLevelFactor;
	if( CXSLDungeon::IsEventDungeon( nDungeonID ) == true )
	{
		fCheckLevelFactor = 1.0f;
	}

	// NPC ��� ó��
	//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT

#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
	return NpcDropItem( iNpcID, kDropInfo.m_iExp, kDropInfo.m_fEDProperty, kDropInfo.m_iED, kDropInfo.m_kLottery, fCheckLevelFactor, fDungeonFactor, fContribution, iPartyUserCount, fPartyDropBonus, fDropRate, sDropData, bIsBoss );
#else // SERV_DROP_EVENT_RENEWAL
	return NpcDropItem( iNpcID, kDropInfo.m_iExp, kDropInfo.m_fEDProperty, kDropInfo.m_iED, kDropInfo.m_kLottery, fCheckLevelFactor, fDungeonFactor, fContribution, fPartyDropBonus, iDropCount, sDropData, bIsBoss );
#endif // SERV_DROP_EVENT_RENEWAL

#else
	return NpcDropItem( iNpcID, kDropInfo.m_iExp, kDropInfo.m_fEDProperty, kDropInfo.m_iED, kDropInfo.m_kLottery, fCheckLevelFactor, fDungeonFactor, fContribution, fPartyDropBonus, sDropData, bIsBoss );
#endif SERV_ITEM_DROP_EVENT
	//}}
}

bool KDropTable::HenirNpcDropItem( IN const CXSLDungeon::DUNGEON_MODE eDungeonMode, 
							 	   IN const int iNpcID, 
								   IN const char cNpcLevel,
								   IN const float fDungeonFactor, 
								   IN const float fContribution,
								   IN const float fPartyDropBonus,
								   IN const int iPartyUserCount,
								   //{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
								   IN const float fDropRate,
#else // SERV_DROP_EVENT_RENEWAL
								   IN const int iDropCount,
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
								   //}}
								   OUT bool& bDecreaseEndurance,
								   OUT DROP_DATA& sDropData,  
								   IN const bool bIsBoss )
{
	sDropData.Clear();

	// �켱 �ش� DungeonMode�� NpcID�� �ش��ϴ� ��� ������ �˻��Ѵ�.
	std::map< std::pair< int, int >, KHenirNpcDropInfo >::const_iterator mit;
	mit = m_mapHenirNpcDrop.find( std::make_pair( static_cast<int>( eDungeonMode ), iNpcID ) );

	// �ش� ��쿡 ���� ������ �������� ����.
	if( mit == m_mapHenirNpcDrop.end() )
	{
		// ���� ���̵� ������ ���� ���� ������ �����ϴ��� �ٽ� �ѹ� Ȯ��.
		mit = m_mapHenirNpcDrop.find( std::make_pair( 0, iNpcID ) );

		// �װ͸��� ���ٸ�?
		if( mit == m_mapHenirNpcDrop.end() )
		{
			// �������� ����. �����϶��� false.
			return true;
		}
	}

	KHenirNpcDropInfo kDropInfo = mit->second;

	// ����ġ ���õ� ���͸� ����ġ�� ����!
	if( kDropInfo.m_iExp > 0 )
	{
		kDropInfo.m_iExp = GetNpcExp( cNpcLevel );

		// ����ġ�� ���õ� ���ʹ� �������� ����!
		bDecreaseEndurance = true;
	}

	if( kDropInfo.m_iED > 0 )
	{
		kDropInfo.m_iED = (10 + ( kDropInfo.m_iED * cNpcLevel ));
	}

	// NPC ��� ó��
	// ��ϸ��� ������ LevelFactor �� 1.0f �� �Ѵ�.
	//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
	return NpcDropItem( iNpcID, kDropInfo.m_iExp, kDropInfo.m_fEDProperty, kDropInfo.m_iED, kDropInfo.m_kLottery, 1.0f, fDungeonFactor, fContribution, iPartyUserCount, fPartyDropBonus, fDropRate, sDropData, bIsBoss );
#else // SERV_DROP_EVENT_RENEWAL
	return NpcDropItem( iNpcID, kDropInfo.m_iExp, kDropInfo.m_fEDProperty, kDropInfo.m_iED, kDropInfo.m_kLottery, 1.0f, fDungeonFactor, fContribution, fPartyDropBonus, iDropCount, sDropData, bIsBoss );
#endif // SERV_DROP_EVENT_RENEWAL
#else
	return NpcDropItem( iNpcID, kDropInfo.m_iExp, kDropInfo.m_fEDProperty, kDropInfo.m_iED, kDropInfo.m_kLottery, 1.0f, fDungeonFactor, fContribution, fPartyDropBonus, sDropData, bIsBoss );
#endif SERV_ITEM_DROP_EVENT
	//}}
}

bool KDropTable::ExtraStageNpcDropItem( IN const int iNpcID, 
									    IN const char cNpcLevel,
									    IN const float fLevelFactor,
									    IN const float fDungeonFactor, 
									    IN const float fContribution,
									    IN const float fPartyDropBonus,
										IN const int iPartyUserCount,
										//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
										IN const float fDropRate,
#else // SERV_DROP_EVENT_RENEWAL
										IN const int iDropCount,
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
										//}}
										OUT bool& bDecreaseEndurance,
									    OUT DROP_DATA& sDropData, 
									    IN const bool bIsBoss /*= false*/ )
{
	sDropData.Clear();

	// �켱 �ش� MonsterID�� NpcLevel�� �ش��ϴ� ��� ������ �˻��Ѵ�.
	std::map< int, KExtraStageNpcDropInfo >::const_iterator mit;
	mit = m_mapExtraStageNpcDrop.find( iNpcID );
	if( mit == m_mapExtraStageNpcDrop.end() )
	{
		// �������� ����. �����϶��� false.
		return true;
	}

	KExtraStageNpcDropInfo kDropInfo = mit->second;

	// ����ġ ���õ� ���͸� ����ġ�� ����!
	if( kDropInfo.m_iExp > 0 )
	{
		kDropInfo.m_iExp = GetNpcExp( cNpcLevel );

		// ����ġ�� ���õ� ���ʹ� �������� ����!
		bDecreaseEndurance = true;
	}

	if( kDropInfo.m_iED > 0 )
	{
		kDropInfo.m_iED = (10 + ( kDropInfo.m_iED * cNpcLevel ));
	}

	//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT

#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
	return NpcDropItem( iNpcID, kDropInfo.m_iExp, kDropInfo.m_fEDProperty, kDropInfo.m_iED, kDropInfo.m_kLottery, fLevelFactor, fDungeonFactor, fContribution, iPartyUserCount, fPartyDropBonus, fDropRate, sDropData, bIsBoss );
#else // SERV_DROP_EVENT_RENEWAL
	return NpcDropItem( iNpcID, kDropInfo.m_iExp, kDropInfo.m_fEDProperty, kDropInfo.m_iED, kDropInfo.m_kLottery, fLevelFactor, fDungeonFactor, fContribution, fPartyDropBonus, iDropCount, sDropData, bIsBoss );
#endif // SERV_DROP_EVENT_RENEWAL

#else
	return NpcDropItem( iNpcID, kDropInfo.m_iExp, kDropInfo.m_fEDProperty, kDropInfo.m_iED, kDropInfo.m_kLottery, fLevelFactor, fDungeonFactor, fContribution, fPartyDropBonus, sDropData, bIsBoss );
#endif SERV_ITEM_DROP_EVENT
	//}}
}

#else		// SERV_REFORM_ITEM_DROP

bool KDropTable::NormalNpcDropItem( IN const int nDungeonID, IN const int iNpcID, IN const char cNpcLevel, IN const char cMonsterTypeFactor, OUT DROP_DATA& sDropData, IN const int iItemDropNum, IN const bool bIsBoss )
{
	sDropData.Clear();

	// �켱 �ش� DungeonID + MonsterID�� �ش��ϴ� ��� ������ �˻��Ѵ�.
	std::map< std::pair< int, int >, KMonsterDropInfo >::const_iterator mit;
	mit = m_mapMonsterDrop.find( std::make_pair( nDungeonID, iNpcID ) );

	// �ش� ��쿡 ���� ������ �������� ����.
	if( mit == m_mapMonsterDrop.end() )
	{
		// ���� ���̵� ������ ���� ���� ������ �����ϴ��� �ٽ� �ѹ� Ȯ��.
		mit = m_mapMonsterDrop.find( std::make_pair( 0, iNpcID ) );

		// �װ͸��� ���ٸ�?
		if( mit == m_mapMonsterDrop.end() )
		{
			// �������� ����. �����϶��� false.
			return true;
		}
	}

	const KMonsterDropInfo& kDropInfo = mit->second;

	// NPC ��� ó��
	return NpcDropItem( iNpcID, kDropInfo.m_iExp, kDropInfo.m_fEDProperty, kDropInfo.m_iED, kDropInfo.m_kLottery, cNpcLevel, cMonsterTypeFactor, sDropData, iItemDropNum, bIsBoss );    
}

//{{ 2009. 7. 26  ������	��ϸ� �ð�
bool KDropTable::HenirNpcDropItem( IN const CXSLDungeon::DUNGEON_MODE eDungeonMode, IN const int iNpcID, IN const char cNpcLevel, IN const char cMonsterTypeFactor, OUT DROP_DATA& sDropData, IN const int iItemDropNum, IN const bool bIsBoss )
{
	sDropData.Clear();

	// �켱 �ش� DungeonMode�� NpcID�� �ش��ϴ� ��� ������ �˻��Ѵ�.
	std::map< std::pair< int, int >, KHenirNpcDropInfo >::const_iterator mit;
	mit = m_mapHenirNpcDrop.find( std::make_pair( static_cast<int>( eDungeonMode ), iNpcID ) );

	// �ش� ��쿡 ���� ������ �������� ����.
	if( mit == m_mapHenirNpcDrop.end() )
	{
		// ���� ���̵� ������ ���� ���� ������ �����ϴ��� �ٽ� �ѹ� Ȯ��.
		mit = m_mapHenirNpcDrop.find( std::make_pair( 0, iNpcID ) );

		// �װ͸��� ���ٸ�?
		if( mit == m_mapHenirNpcDrop.end() )
		{
			// �������� ����. �����϶��� false.
			return true;
		}
	}

	const KHenirNpcDropInfo& kDropInfo = mit->second;

	// NPC ��� ó��
	return NpcDropItem( iNpcID, kDropInfo.m_iExp, kDropInfo.m_fEDProperty, kDropInfo.m_iED, kDropInfo.m_kLottery, cNpcLevel, cMonsterTypeFactor, sDropData, iItemDropNum, bIsBoss );
}
//}}

//{{ 2010. 05. 02  ������	��д��� ����
#ifdef SERV_SECRET_HELL

bool KDropTable::ExtraStageNpcDropItem( IN const int iNpcID, IN const char cNpcLevel, IN const char cMonsterTypeFactor, OUT DROP_DATA& sDropData, IN const int iItemDropNum /*= 1*/, IN const bool bIsBoss /*= false*/ )
{
	sDropData.Clear();

	// �켱 �ش� MonsterID�� NpcLevel�� �ش��ϴ� ��� ������ �˻��Ѵ�.
	std::map< int, KExtraStageNpcDropInfo >::const_iterator mit;
	mit = m_mapExtraStageNpcDrop.find( iNpcID );
	if( mit == m_mapExtraStageNpcDrop.end() )
	{
		// �������� ����. �����϶��� false.
		return true;
	}

	const KExtraStageNpcDropInfo& kDropInfo = mit->second;

	return NpcDropItem( iNpcID, kDropInfo.m_iExp, kDropInfo.m_fEDProperty, kDropInfo.m_iED, kDropInfo.m_kLottery, cNpcLevel, cMonsterTypeFactor, sDropData, iItemDropNum, bIsBoss );
}

#endif SERV_SECRET_HELL
//}}
#endif SERV_REFORM_ITEM_DROP
//}}

//{{ 2011. 11. 22	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//bool KDropTable::BattleFieldNpcDropItem( IN const CXSLBattleField::BATTLE_FIELD_ID eBattleFieldID, IN const int iNpcID ) const
//{
//    return true;
//}
//#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 11. 12	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KDropTable::NpcSpecialDropItem( IN const int iNpcID, IN const char cMonsterGrade, IN const char cNpcDieState, IN OUT std::vector< int >& vecItemID )
{
	switch( iNpcID )
	{
	case CXSLUnitManager::NUI_MANA_EATER:
	case CXSLUnitManager::NUI_HALLOWEEN_MANA_EATER:
	case CXSLUnitManager::NUI_DEFENCE_MANA_EATER:
	case CXSLUnitManager::NUI_MANA_EATER_FD:
		{
			if( cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_BONUS_DROP )
			{
				// ��� ���̺��� Ȯ���ؼ� 'Special Drop'������� ��ü�ҿ���
				vecItemID.push_back( 70055 ); // 70055
			}
		}
		break;

	case CXSLUnitManager::NUI_BLOOD_EATER:
	case CXSLUnitManager::NUI_DEFENCE_BLOOD_EATER:
	case CXSLUnitManager::NUI_BLOOD_EATER_FD:
		{
			if( cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_BONUS_DROP )
			{
				// ��� ���̺��� Ȯ���ؼ� 'Special Drop'������� ��ü�ҿ���
				vecItemID.push_back( 70056 ); // 70055
			}
		}
		break;
	}		
	//}}

	//{{ ����, �ߺ� ���� �� ������ ��� �Ǵ� ���(MonsterGrade ����) - ��μ�
#ifdef SERV_KILL_BOSS_MIDDLE_BOSS_DROP_ITEM
	switch( cMonsterGrade )
	{
	case CXSLUnitManager::MG_MIDDLE_BOSS_NPC:
		{
			// ��� ���̺��� Ȯ���ؼ� 'Special Drop'������� ��ü�ҿ���
			if( rand() % 100 < 50 )	// �ߺ����� 50% Ȯ���θ� ���
			{
				vecItemID.push_back( 70057 ); // 70057
			}
		}
		break;

	case CXSLUnitManager::MG_BOSS_NPC:
		{
			// ��� ���̺��� Ȯ���ؼ� 'Special Drop'������� ��ü�ҿ���
			vecItemID.push_back( 70058 ); // 70057
		}
		break;

#ifdef SERV_ADD_MONSTER_GRADE_FOR_DEFINITELY_DROP_HP_BALL_ITEM// �۾���¥: 2013-08-28	// �ڼ���
	case CXSLUnitManager::MG_DEFINITELY_DROP_HP_BALL:
		vecItemID.push_back( 70057 ); // 70057
		break;
#endif // SERV_ADD_MONSTER_GRADE_FOR_DEFINITELY_DROP_HP_BALL_ITEM
	}
#endif SERV_KILL_BOSS_MIDDLE_BOSS_DROP_ITEM
	//}	
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2009. 5. 13  ������	���꽺�������� ����ġ �ִ� npc��
bool KDropTable::IsExpDropNpc( IN int nDungeonID, IN int nMonsterID )
{
	std::map< std::pair< int, int >, KMonsterDropInfo >::const_iterator mit;
	mit = m_mapMonsterDrop.find( std::make_pair( nDungeonID, nMonsterID ) );
	if( mit == m_mapMonsterDrop.end() )
	{		
		mit = m_mapMonsterDrop.find( std::make_pair( 0, nMonsterID ) );

		if( mit == m_mapMonsterDrop.end() )
			return false;
	}

	return ( mit->second.m_iExp > 0 );
}
//}}

//{{ 2010. 01. 05  ������	����������
bool KDropTable::IsHenirExpDropNpc( IN CXSLDungeon::DUNGEON_MODE eDungeonMode, IN int nMonsterID )
{
	std::map< std::pair< int, int >, KHenirNpcDropInfo >::const_iterator mit;
	mit = m_mapHenirNpcDrop.find( std::make_pair( static_cast<int>( eDungeonMode ), nMonsterID ) );
	if( mit == m_mapHenirNpcDrop.end() )
	{		
		mit = m_mapHenirNpcDrop.find( std::make_pair( 0, nMonsterID ) );

		if( mit == m_mapHenirNpcDrop.end() )
			return false;
	}

	return ( mit->second.m_iExp > 0 );
}
//}}

//{{ 2009. 4. 22  ������	��ü���
//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
bool KDropTable::StaticDropItem( IN int nDungeonID, OUT DROP_DATA& sDropData, IN float fUserContribution )
{
	sDropData.m_vecItemID.clear();
	sDropData.m_iExp        = -1;
	sDropData.m_iGPItemID   = -1;
	sDropData.m_iED         = -1;
	sDropData.m_nGPNum      = -1;

	std::map< int, KStaticDropInfo >::const_iterator mit;
	mit = m_mapStaticDrop.find( nDungeonID );
	if( mit == m_mapStaticDrop.end() )
	{
		START_LOG( clog, L"�ش� ������ ���� ��ü��� ������ ����" )
			<< BUILD_LOG( nDungeonID );
		return true;
	}

	if( fUserContribution <= 0.0f )
		return true;

	KLottery kLott = mit->second.m_kLottery;
	kLott.AddMultiProbRate( fUserContribution );

	int iResultItemID = kLott.Decision();
	if( iResultItemID != KLottery::CASE_BLANK )
	{
		// ������ ���!
		sDropData.m_vecItemID.push_back( iResultItemID );
	}
	
	return true;
}

bool KDropTable::EventDropItem( IN float fUserContribution, OUT DROP_DATA& sDropData )
{	
	return StaticDropItem( 0, sDropData, fUserContribution );
}

#else
bool KDropTable::StaticDropItem( IN int nDungeonID, OUT DROP_DATA& sDropData, IN int iItemDropNum /*= 1*/ )
{
	sDropData.m_vecItemID.clear();
	sDropData.m_iExp        = -1;
	sDropData.m_iGPItemID   = -1;
	sDropData.m_iED         = -1;
	sDropData.m_nGPNum      = -1;

	std::map< int, KStaticDropInfo >::const_iterator mit;
	mit = m_mapStaticDrop.find( nDungeonID );
	if( mit == m_mapStaticDrop.end() )
	{
		START_LOG( clog, L"�ش� ������ ���� ��ü��� ������ ����" )
			<< BUILD_LOG( nDungeonID );
		return true;
	}

	for( int i = 0; i < iItemDropNum; ++i )
	{
		int iResultItemID = mit->second.m_kLottery.Decision();
		if( iResultItemID != KLottery::CASE_BLANK )
		{
			// ������ ���!
			sDropData.m_vecItemID.push_back( iResultItemID );
		}
	}
	return true;
}

bool KDropTable::EventDropItem( IN int iItemDropNum, OUT DROP_DATA& sDropData )
{	
	return StaticDropItem( 0, sDropData, iItemDropNum );
}
#endif SERV_REFORM_ITEM_DROP
//}}
//}}

bool KDropTable::DropGroupItem( IN int iDropGroupID, OUT int& iDropItemID )
{
	std::map< int, KLottery >::const_iterator mit;
	mit = m_mapDropGroup.find( iDropGroupID );
	if( mit == m_mapDropGroup.end() )
		return false;

	iDropItemID = mit->second.Decision();
	return true;
}

//{{ 2009. 6. 17  ������	�������
bool KDropTable::PvpDropItem( IN int iItemDropNum, OUT DROP_DATA& sDropData )
{
	sDropData.m_vecItemID.clear();
	sDropData.m_iExp        = -1;
	sDropData.m_iGPItemID   = -1;
	sDropData.m_iED         = -1;
	sDropData.m_nGPNum      = -1;

	int iResultItemID = m_kPvpDrop.Decision();
	if( iResultItemID != KLottery::CASE_BLANK )
	{
		sDropData.m_vecItemID.push_back( iResultItemID );
	}
	return true;
}
//}}

//{{ 2009. 5. 23  ������	�ð� ���
void KDropTable::SetTimeDropInfo( IN const std::vector< KTimeDropInfo >& vecTimeDropInfo, IN const std::wstring& wstrServerIP )
{	
	CTime tCurTime = CTime::GetCurrentTime();

	BOOST_TEST_FOREACH( const KTimeDropInfo&, kTimeDropInfo, vecTimeDropInfo )
	{
		CTime tTimeDrop;		
		std::wstring wstrDropTime = kTimeDropInfo.m_wstrDropTime;
		if( KncUtil::ConvertStringToCTime( wstrDropTime, tTimeDrop ) == false )
		{
			START_LOG( cerr, L"Drop Time �Ľ� ����!" )
				<< BUILD_LOG( kTimeDropInfo.m_iTimeDropID )
				<< BUILD_LOG( kTimeDropInfo.m_wstrDropTime )
				<< END_LOG;

			continue;
		}

		// �������� ���������� �˻�
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kTimeDropInfo.m_iDropItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"�������� �ʴ� itemid�� ��ϵǾ��ֽ��ϴ�." )
				<< BUILD_LOG( kTimeDropInfo.m_iTimeDropID )
				<< BUILD_LOG( kTimeDropInfo.m_iDropItemID )
				<< END_LOG;

			continue;
		}
		
		// �������� ������Ʈ ����
        if( tTimeDrop <= tCurTime )
		{
			START_LOG( clog, L"��¥ ���� �̺�Ʈ" )
				<< BUILD_LOG( kTimeDropInfo.m_iTimeDropID )
				<< BUILD_LOG( kTimeDropInfo.m_wstrDropTime );

			continue;
		}

		// �ش� �������� �˻�
		if( kTimeDropInfo.m_wstrServerIP != wstrServerIP )
			continue;

        KTimeDropItemInfo kInfo;
		kInfo.m_iTimeDropID	 = kTimeDropInfo.m_iTimeDropID;
		kInfo.m_iItemID		 = kTimeDropInfo.m_iDropItemID;
		kInfo.m_wstrItemName = pItemTemplet->m_Name;
		kInfo.m_kDropTime	 = tTimeDrop;
		kInfo.m_bIsDrop		 = false;
		kInfo.m_bNoticeMsg	 = kTimeDropInfo.m_bNoticeMsg;

		m_vecTimeDrop.push_back( kInfo );
		m_mapTimeDrop.insert( std::make_pair( kInfo.m_iTimeDropID, kInfo ) );

		START_LOG( cout, L"�ð� ��� �̺�Ʈ ���!" )
			<< BUILD_LOG( kInfo.m_iTimeDropID )			
			<< BUILD_LOG( kInfo.m_wstrItemName )
			<< BUILD_LOG( kTimeDropInfo.m_wstrDropTime )
			<< BUILD_LOG( kInfo.m_bNoticeMsg );
	}
}

bool KDropTable::CheckTimeDrop( OUT int& iTimeDropID, OUT int& iDropItemID )
{
	iDropItemID = 0;
	
	if( m_vecTimeDrop.empty() )
		return false;

	CTime tCurTime = CTime::GetCurrentTime();
	
	std::vector< KTimeDropItemInfo >::iterator vit;
	for( vit = m_vecTimeDrop.begin(); vit != m_vecTimeDrop.end(); ++vit )
	{
		// ���� ����� �ð��� �ƴϴ�
		if( vit->m_kDropTime > tCurTime )
			return false;

		// �̺�Ʈ ����!
		iTimeDropID = vit->m_iTimeDropID;
		iDropItemID = vit->m_iItemID;

		START_LOG( clog2, L"Ư�� �ð��� ����ϱ� ����!" )
			<< BUILD_LOG( iTimeDropID )
			<< BUILD_LOG( iDropItemID );

		break;
	}

	if( vit != m_vecTimeDrop.end() )
	{
		// �̺�Ʈ ����Ʈ���� ����
        m_vecTimeDrop.erase( vit );
		return true;
	}

	return false;
}

bool KDropTable::RollbackTimeDrop( IN int iTimeDropID )
{
	std::map< int, KTimeDropItemInfo >::const_iterator mit;
	mit = m_mapTimeDrop.find( iTimeDropID );
	if( mit == m_mapTimeDrop.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� �ð� ��� �����Դϴ�." )
			<< BUILD_LOG( iTimeDropID )
			<< END_LOG;

		return false;
	}
    
	// �ѹ�!
	m_vecTimeDrop.insert( m_vecTimeDrop.begin(), mit->second );

	START_LOG( clog2, L"Ư�� �ð��� ��� �������� �ѹ��Ͽ����ϴ�." )
		<< BUILD_LOG( iTimeDropID );

	return true;
}
//}}

//{{ 2009. 6. 29  ������	��� ȹ�� ����
bool KDropTable::IsNotifyDropItem( IN int iItemID )
{
	if( !m_bNotifyDropItem )
		return false;
	
    return ( m_setNotifyDropItem.find( iItemID ) != m_setNotifyDropItem.end() );
}
//}}

//{{ 2011. 03. 28	������	���� ���� üũ
#ifdef SERV_MANY_ED_DROP_EVENT_NPC
bool KDropTable::GetManyEDDropNpcInfo( IN const int iNpcID, OUT KManyEDDropNpcInfo& kInfo )
{
	if( IsManyEDDropNpcEvent() == false )
		return false;

	std::map< int, KManyEDDropNpcInfo >::const_iterator mit;
	mit = m_mapManyEDDropTable.find( iNpcID );
	if( mit == m_mapManyEDDropTable.end() )
		return false;

    kInfo = mit->second;
	return true;
}
#endif SERV_MANY_ED_DROP_EVENT_NPC
//}}

//{{ 2011. 05. 04  ��μ�	���� Ŭ����� ������ ���� ���� �߰�
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
bool KDropTable::AddPaymentDungeon_LUA( IN int iDungeonID )
{

	KLuaManager luaMgr( GetLuaState() );
	DUNGEON_CLEAR_REWARD sData;

	// ����ġ,VP,ED ���ʽ� �̺�Ʈ
	
	//{{ 2012. 10. 17	�ڼ���	PaymentItem ���� ���� �߰�
#ifdef SERV_UNPAYMENT_CONDITION
	sData.m_iDungeonID	= iDungeonID;
#endif SERV_UNPAYMENT_CONDITION
	//}}

	// AddUnPaymentCondition_LUA �κ��� �ʱ�ȭ ������ ��ġ��ų �� �ֵ��� �����ϴ� ���� �����ϴ�.
	LUA_GET_VALUE( luaMgr,		"iItemID",			sData.m_iItemID,		0 );
	LUA_GET_VALUE( luaMgr,		"fRate",			sData.m_fRate,			0.f );
	LUA_GET_VALUE( luaMgr,		"iQuestID",			sData.m_iQuestID,		0 );
#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
	LUA_GET_VALUE( luaMgr,		"iGoingQuestID",	sData.m_iGoingQuestID,		0 );
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST
	LUA_GET_VALUE( luaMgr,		"iMinLv",			sData.m_iMinLv,			0 );
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
	LUA_GET_VALUE( luaMgr,		"iMaxLv",			sData.m_iMaxLv,			99 );
#else
	LUA_GET_VALUE( luaMgr,		"iMaxLv",			sData.m_iMaxLv,			0 );
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
	LUA_GET_VALUE( luaMgr,		"iQuantity",		sData.m_iQuantity,		1 );
	LUA_GET_VALUE( luaMgr,		"cUnitClass",				sData.m_cUnitClass,			CXSLUnit::UC_NONE );
	LUA_GET_VALUE( luaMgr,		"fPcBangUserRate",			sData.m_fPcBangUserRate,	0.f );

#ifdef	SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
	LUA_GET_VALUE( luaMgr,		"iTitleID1",			sData.m_iTitleID,			0 );
	LUA_GET_VALUE( luaMgr,		"iTitleID2",			sData.m_iTitleID2,			0 );
	LUA_GET_VALUE_ENUM( luaMgr,		"eUnitType",		sData.m_eUnitType,		CXSLUnit::UNIT_TYPE,	CXSLUnit::UT_NONE);
	LUA_GET_VALUE( luaMgr,		"bPartyPay",			sData.m_bPartyPay,		false);
	LUA_GET_VALUE( luaMgr,		"bComebak",				sData.m_bComeback,		false);
	LUA_GET_VALUE( luaMgr,		"iEquippedItemID1",		sData.m_iEquippedItemID1,		0 );
	LUA_GET_VALUE( luaMgr,		"iEquippedItemID2",		sData.m_iEquippedItemID2,		0);
	LUA_GET_VALUE( luaMgr,		"iUseItemID",			sData.m_iUseItemID,		0 );
	LUA_GET_VALUE( luaMgr,		"bMustParty",			sData.m_bMustParty,		false );
#endif  SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX

#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
	LUA_GET_VALUE( luaMgr,		"iConsumingItemID",		sData.m_iConsumingItemID,		0 );

	if( sData.m_bPartyPay == true && sData.m_iConsumingItemID > 0 )
	{
		START_LOG( cerr, L"��Ƽ ���ް� ������ �Ҹ�� ���� ���� �� �ȴ�!" )
			<< BUILD_LOG( sData.m_bPartyPay )
			<< BUILD_LOG( sData.m_iConsumingItemID )
			<< END_LOG;
			
		return false;
	}
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM

#ifdef SERV_PAYMENT_ITEM_WITH_ALLY_NPC
	LUA_GET_VALUE( luaMgr,		"iAllyNPCID",			sData.m_iAllyNPCID,		0 );
#endif SERV_PAYMENT_ITEM_WITH_ALLY_NPC

	//{{ 2012. 11. 05	�ڼ���	PaymentItem �Ⱓ ���� �߰�
#ifdef SERV_PAYMENT_TIME_CONDITION
	std::wstring wstrDate;
	LUA_GET_VALUE( luaMgr,		L"wstrBeginDate",	wstrDate,				L"" );
	if( wstrDate.empty() == false )
	{
		if( KncUtil::ConvertStringToCTime( wstrDate, sData.m_tBeginDate ) == false )
		{
			START_LOG( cerr, L"�ð� ��ȯ ����" )
				<< BUILD_LOG( wstrDate )
				<< BUILD_LOG( iDungeonID )
				<< BUILD_LOG( sData.m_iItemID )
				<< BUILD_LOG( sData.m_fRate )
				<< END_LOG;
			return false;
		}
	}
	else
	{
		sData.m_tBeginDate = 0;
	}

	LUA_GET_VALUE( luaMgr,		L"wstrEndDate",	wstrDate,				L"" );
	if( wstrDate.empty() == false )
	{
		if( KncUtil::ConvertStringToCTime( wstrDate, sData.m_tEndDate ) == false )
		{
			START_LOG( cerr, L"�ð� ��ȯ ����" )
				<< BUILD_LOG( wstrDate )
				<< BUILD_LOG( iDungeonID )
				<< BUILD_LOG( sData.m_iItemID )
				<< BUILD_LOG( sData.m_fRate )
				<< END_LOG;
			return false;
		}
	}
	else
	{
		sData.m_tEndDate = 0;
	}
#endif SERV_PAYMENT_TIME_CONDITION
	//}}

	// �Է� �� �˻�(�ʼ����鸸 �˻�)
	if( ( iDungeonID < 0 ) || ( sData.m_iItemID <=0 ) || ( sData.m_fRate > 100.f ) || ( sData.m_iQuantity < 1 ) || ( sData.m_fPcBangUserRate > 100.f ))
	{
		START_LOG( cerr, L"���� Ŭ����� ������ ���� ���� ���ð��� �̻��մϴ�." )
			<< BUILD_LOG( iDungeonID )
			<< BUILD_LOG( sData.m_iItemID )
			<< BUILD_LOG( sData.m_fRate )
			<< BUILD_LOG( sData.m_iQuestID )
#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
			<< BUILD_LOG( sData.m_iGoingQuestID )
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST
			<< BUILD_LOG( sData.m_iMinLv )
			<< BUILD_LOG( sData.m_iMaxLv )
			<< BUILD_LOG( sData.m_iQuantity )
			<< BUILD_LOGc( sData.m_cUnitClass )
			<< BUILD_LOG( sData.m_fPcBangUserRate )
#ifdef	SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
			<< BUILD_LOG( sData.m_iTitleID )
			<< BUILD_LOG( sData.m_iTitleID2 )
			<< BUILD_LOG( sData.m_eUnitType )
			<< BUILD_LOG( sData.m_bPartyPay )
			<< BUILD_LOG( sData.m_bComeback )
			<< BUILD_LOG( sData.m_iEquippedItemID1 )
			<< BUILD_LOG( sData.m_iEquippedItemID2 )
			<< BUILD_LOG( sData.m_iUseItemID )
			<< BUILD_LOG( sData.m_bMustParty )
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
			<< BUILD_LOG( sData.m_iConsumingItemID )
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
#ifdef SERV_PAYMENT_ITEM_WITH_ALLY_NPC
			<< BUILD_LOG( sData.m_iAllyNPCID )
#endif SERV_PAYMENT_ITEM_WITH_ALLY_NPC
			<< END_LOG;

		return false;
	}

	std::map< int, std::vector< DUNGEON_CLEAR_REWARD > >::iterator mit = m_mapPaymentItemTable.find( iDungeonID );
	if( mit == m_mapPaymentItemTable.end() )  // �ش� DungeonID �� �����Ͱ� ����.
	{
		std::vector< DUNGEON_CLEAR_REWARD > vecNewData;
		vecNewData.push_back( sData );

		m_mapPaymentItemTable.insert( std::make_pair( iDungeonID, vecNewData ) );		
	}
	else  // �ش� DungeonID �� �����Ͱ� �ִ�.
	{
		mit->second.push_back( sData );

		//std::vector< DUNGEON_CLEAR_REWARD >::iterator vit = mit->second.begin();
		//if( vit == mit->second.end() )  // �ش� ������ ����? ���� �� �ִ°�?
		//{
		//	mit->second.push_back( sData );
		//}
		//else  // �ߺ��Ǵ� ������ �ִٸ� �Է����� ����
		//{
		//	for( ; vit != mit->second.end() ; ++vit )
		//	{
		//		// ������ ã�´�   ����Ʈ, ����ġ�޴´���, ����
		//		if( (sData.m_iItemID == vit->m_iItemID) 
		//		 && (sData.m_iQuestID == vit->m_iQuestID) 
		//		 && sData.m_cUnitClass == vit->m_cUnitClass
		//		 && ((sData.m_iMaxLv >= vit->m_iMinLv) || (vit->m_iMaxLv >= sData.m_iMinLv))  )  
		//		{
		//			START_LOG( cerr, L"���� Ŭ����� ������ ���� ���� �ߺ� �����Ͱ� �����Ѵ�!!" )
		//				<< BUILD_LOG( iDungeonID )
		//				<< BUILD_LOG( sData.m_iItemID )
		//				<< BUILD_LOG( sData.m_fRate )
		//				<< BUILD_LOG( sData.m_iQuestID )
		//				<< BUILD_LOG( sData.m_iMinLv )
		//				<< BUILD_LOG( sData.m_iMaxLv )
		//				<< BUILD_LOG( sData.m_iQuantity )
		//				<< BUILD_LOGc( sData.m_cUnitClass )
		//				<< BUILD_LOG( sData.m_fPcBangUserRate )
		//				<< END_LOG;

		//			return false;
		//		}
		//	}
		//	
		//	mit->second.push_back( sData );

		//	START_LOG( clog, L"���� Ŭ����� ������ ���� ���� �߰� ����!!" )
		//		<< BUILD_LOG( iDungeonID )
		//		<< BUILD_LOG( sData.m_iItemID )
		//		<< BUILD_LOG( sData.m_fRate )
		//		<< BUILD_LOG( sData.m_iQuestID )
		//		<< BUILD_LOG( sData.m_iMinLv )
		//		<< BUILD_LOG( sData.m_iMaxLv )
		//		<< BUILD_LOG( sData.m_iQuantity )
		//		<< BUILD_LOGc( sData.m_cUnitClass )
		//		<< BUILD_LOG( sData.m_fPcBangUserRate )
		//		<< END_LOG;
		//}
	}	

	return true;
}

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT

bool KDropTable::CheckDungeonClearPaymentItem( IN int iDungeonID, IN char cDifficulty, IN bool bExp, KRoomUserPtr spRoomUser,
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
											  KRoomUserManagerPtr spRoomUserManager,
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
											  OUT std::map< int, KItemInfo >& DungeonClearPaymentItem, IN bool bAraEvent )

#else //SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT

bool KDropTable::CheckDungeonClearPaymentItem( IN int iDungeonID, IN char cDifficulty, IN bool bExp, KRoomUserPtr spRoomUser,
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
											  KRoomUserManagerPtr spRoomUserManager,
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
											  OUT std::map< int, KItemInfo >& DungeonClearPaymentItem )

#endif //SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT
{
	std::map< int, std::vector< DUNGEON_CLEAR_REWARD > >::iterator mit;

	// 1. ��� ���� ����
	mit = m_mapPaymentItemTable.find( 0 );
	if( mit != m_mapPaymentItemTable.end() )
	{
		ProcessPaymentItem( mit,
			//{{ 2012. 10. 17	�ڼ���	PaymentItem ���� ���� �߰�
#ifdef SERV_UNPAYMENT_CONDITION
			iDungeonID,
			cDifficulty,
#endif SERV_UNPAYMENT_CONDITION
			//}}
			spRoomUser,
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
			spRoomUserManager,
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
			DungeonClearPaymentItem
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT
			, bAraEvent
#endif SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT
			);
	}

	// 2. ����ġ�� �޴� ���� ����
	mit = m_mapPaymentItemTable.find( 1 );
	if( mit != m_mapPaymentItemTable.end() && bExp == true )
	{
		ProcessPaymentItem( mit,
			//{{ 2012. 10. 17	�ڼ���	PaymentItem ���� ���� �߰�
#ifdef SERV_UNPAYMENT_CONDITION
			iDungeonID,
			cDifficulty,
#endif SERV_UNPAYMENT_CONDITION
			//}}
			spRoomUser,
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
			spRoomUserManager,
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
			DungeonClearPaymentItem
			);
	}

	// 3. �������� ������ �´� ���� ����
	int iDungeonMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( iDungeonID + static_cast<int>(cDifficulty) );
	int iDungeonMaxLevel = SiCXSLDungeonManager()->GetDungeonMaxLevel( iDungeonID + static_cast<int>(cDifficulty) );

	if( (iDungeonMinLevel - DUNGEON_CLEAR_REWARD::TL_MIN_LEVEL) <= spRoomUser->GetLevel()  && spRoomUser->GetLevel() <= (iDungeonMaxLevel + DUNGEON_CLEAR_REWARD::TL_MAX_LEVEL) )
	{
		mit = m_mapPaymentItemTable.find( 2 );
		if( mit != m_mapPaymentItemTable.end() && bExp == true )
		{
			ProcessPaymentItem( mit,
				//{{ 2012. 10. 17	�ڼ���	PaymentItem ���� ���� �߰�
#ifdef SERV_UNPAYMENT_CONDITION
				iDungeonID,
				cDifficulty,
#endif SERV_UNPAYMENT_CONDITION
				//}}
				spRoomUser,
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
				spRoomUserManager,
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
				DungeonClearPaymentItem
				);
		}
	}

	// 4. Ư�� ����, Ư�� ���̵� �� ��
	int iDungeon = iDungeonID + static_cast<int>( cDifficulty );
	mit = m_mapPaymentItemTable.find( iDungeon );
	if( mit != m_mapPaymentItemTable.end() )
	{
		ProcessPaymentItem( mit,
			//{{ 2012. 10. 17	�ڼ���	PaymentItem ���� ���� �߰�
#ifdef SERV_UNPAYMENT_CONDITION
			iDungeonID,
			cDifficulty,
#endif SERV_UNPAYMENT_CONDITION
			//}}
			spRoomUser,
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
			spRoomUserManager,
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
			DungeonClearPaymentItem
			);
	}

	return true;
}

bool KDropTable::GetPaymentItemInfo( IN const DUNGEON_CLEAR_REWARD& sData, IN bool bIsPcBang, OUT KItemInfo& kResultItemInfo ) const
{
	// Ȯ�� ���
	KLottery Cal;
	float fRate = sData.m_fRate;

	if( bIsPcBang == true )
	{
		if( sData.m_fPcBangUserRate > fRate )
		{
			fRate = sData.m_fPcBangUserRate;
		}
	}

	Cal.AddCase( 1, fRate );

	if( Cal.Decision() == KLottery::CASE_BLANK )
	{
		START_LOG( clog, L"ȹ�� Ȯ�� ��� - ���� Ȯ�� �̴޷� ���� ���� ����" )
			<< BUILD_LOG( sData.m_iItemID )
			<< BUILD_LOG( sData.m_fRate )
			<< END_LOG;
		return false;
	}

	// ������ ����
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( sData.m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"������ ������ �������� �ʽ��ϴ�! ���� Ŭ����� ������ ���� ����!!" )
			<< BUILD_LOG( sData.m_iItemID )
			<< END_LOG;
		return false;
	}
	else
	{
		kResultItemInfo.m_iItemID	 = pItemTemplet->m_ItemID;
		kResultItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
		kResultItemInfo.m_iQuantity	 = sData.m_iQuantity;
		kResultItemInfo.m_sEndurance = pItemTemplet->m_Endurance;
	}

	return true;
}

void KDropTable::ProcessPaymentItem( IN const std::map< int, std::vector< DUNGEON_CLEAR_REWARD > >::iterator& mit,
									//{{ 2012. 10. 17	�ڼ���	PaymentItem ���� ���� �߰�
#ifdef SERV_UNPAYMENT_CONDITION
									IN const int iDungeonID,
									IN const char cDifficulty,
#endif SERV_UNPAYMENT_CONDITION
									//}}
									IN const KRoomUserPtr spRoomUser,
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
									IN const KRoomUserManagerPtr spRoomUserManager,
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
									OUT std::map< int, KItemInfo >& DungeonClearPaymentItem
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT
									, IN const bool bAraEvent
#endif SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT
									) const
{
	KRoomUserInfo sRoomUserInfo;
	spRoomUser->GetRoomUserInfo( sRoomUserInfo );

	std::vector< DUNGEON_CLEAR_REWARD >::iterator vit;
	vit = mit->second.begin();
	for( ; vit != mit->second.end() ; ++vit )
	{
		//{{ 2012. 11. 05	�ڼ���	PaymentItem �Ⱓ ���� �߰�
#ifdef SERV_PAYMENT_TIME_CONDITION
		if( ( 0 < vit->m_tBeginDate.GetTime() ) || ( 0 < vit->m_tEndDate.GetTime() ) )
		{
			CTime tCurrentTime = CTime::GetCurrentTime();

			if( ( 0 < vit->m_tBeginDate.GetTime() ) &&
				( tCurrentTime < vit->m_tBeginDate )
				)
			{
				continue;
			}

			if( ( 0 < vit->m_tEndDate.GetTime() ) &&
				( vit->m_tEndDate <= tCurrentTime )
				)
			{
				continue;
			}
		}
#endif SERV_PAYMENT_TIME_CONDITION
		//}}

		//{{ 2012. 10. 17	�ڼ���	PaymentItem ���� ���� �߰�
#ifdef SERV_UNPAYMENT_CONDITION
		DUNGEON_CLEAR_REWARD kCheckValue(	iDungeonID + static_cast<int>( cDifficulty ),
			vit->m_iItemID,
			vit->m_fRate,
			vit->m_iQuestID,
#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
			vit->m_iGoingQuestID,
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST
			vit->m_iMinLv,
			vit->m_iMaxLv
			, vit->m_iQuantity
			, vit->m_cUnitClass
			, vit->m_fPcBangUserRate
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
			, vit->m_iTitleID
			, vit->m_iTitleID2
			, vit->m_eUnitType
			, vit->m_bPartyPay
			, vit->m_bComeback
			, vit->m_iEquippedItemID1
			, vit->m_iEquippedItemID2
			, vit->m_iUseItemID
			, vit->m_bMustParty
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
			, vit->m_iConsumingItemID
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
#ifdef SERV_PAYMENT_ITEM_WITH_ALLY_NPC
			, vit->m_iAllyNPCID
#endif SERV_PAYMENT_ITEM_WITH_ALLY_NPC
			);

		if( CheckUnPaymentCondition( kCheckValue ) == true )
		{
			continue;
		}
#endif SERV_UNPAYMENT_CONDITION
		//}}

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT
		if( vit->m_iQuestID == 63750 && bAraEvent == false )
			continue;
#endif SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
		if(_CheckPaymentCondition( *vit, spRoomUser, spRoomUserManager))
		{
			// ������ ����
			KItemInfo kResultItemInfo;
			if( GetPaymentItemInfo( *vit, spRoomUser->IsGameBang(), kResultItemInfo ) == true )
			{
				std::map< int, KItemInfo >::iterator mit = DungeonClearPaymentItem.find( kResultItemInfo.m_iItemID );
				if( mit != DungeonClearPaymentItem.end() )
				{
					// �������� �� ������� ���� ������ �����ش�.
					// �ִ� �������� ��������?? �۽�...
					mit->second.m_iQuantity += kResultItemInfo.m_iQuantity;
				}
				else
					DungeonClearPaymentItem.insert( std::make_pair(kResultItemInfo.m_iItemID, kResultItemInfo ) );
				spRoomUser->AddItem( kResultItemInfo.m_iItemID, kResultItemInfo.m_iQuantity );

#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
				// Ŭ���� ������ ���޵ǰ�, �ش� Ŭ���� ������ ���� �Ҹ��ؾ� �� �������� ���� �� �Ҹ��մϴ�.
				if( spRoomUser->HasSpecialItem( vit->m_iConsumingItemID ) == true )
				{
					const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( vit->m_iConsumingItemID );
					if( pItemTemplet == NULL )
					{
						START_LOG( cerr, L"������ ������ �������� �ʽ��ϴ�! ���� Ŭ����� ������ �Ҹ� ����!!" )
							<< BUILD_LOG( vit->m_iConsumingItemID )
							<< END_LOG;
					}
					else
					{
						KItemInfo kConsumingItemInfo;
						kConsumingItemInfo.m_iItemID	 = pItemTemplet->m_ItemID;
						kConsumingItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
						kConsumingItemInfo.m_iQuantity	 = -1;
						kConsumingItemInfo.m_sEndurance = pItemTemplet->m_Endurance;

						START_LOG( clog, L"�Ҹ�Ǵ� ������" )
							<< BUILD_LOG( kConsumingItemInfo.m_iItemID )
							<< BUILD_LOG( kConsumingItemInfo.m_iQuantity );

						//spRoomUser->AddItem( kConsumingItemInfo.m_iItemID, kConsumingItemInfo.m_iQuantity );
						DungeonClearPaymentItem.insert( std::make_pair( kConsumingItemInfo.m_iItemID, kConsumingItemInfo ) );
					}
				}
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
			}
		}
#else //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
		if( vit->m_iQuestID != 0 ) // ����Ʈ ���� ����
		{
			std::set< int >::iterator qsit = sRoomUserInfo.m_setQuestInfo.find( vit->m_iQuestID );
			if( qsit != sRoomUserInfo.m_setQuestInfo.end() ) // ����Ʈ�� �����ϰ� �ִ�
			{
				if( vit->m_iMinLv != 0 && vit->m_iMaxLv != 0 ) // ����Ʈ �����ϰ� ���� ����
				{
					if( vit->m_iMinLv <= sRoomUserInfo.m_ucLevel && vit->m_iMaxLv >= sRoomUserInfo.m_ucLevel ) // ���� ���� ������
					{
						KItemInfo kResultItemInfo;
						// ������ ����
						if( vit->m_cUnitClass != CXSLUnit::UC_NONE && sRoomUserInfo.m_cUnitClass != vit->m_cUnitClass )
							continue;

						if( GetPaymentItemInfo( *vit, spRoomUser->IsGameBang(), kResultItemInfo ) == true )
						{
							DungeonClearPaymentItem.insert( std::make_pair(vit->m_iItemID, kResultItemInfo ) );
							spRoomUser->AddItem( kResultItemInfo.m_iItemID, kResultItemInfo.m_iQuantity );
						}
					}
				}
				else if( vit->m_iMinLv !=0 && vit->m_iMaxLv == 0 )
				{
					if( vit->m_iMinLv <= sRoomUserInfo.m_ucLevel ) // ���� ���� ������
					{
						KItemInfo kResultItemInfo;
						// ������ ����
						if( vit->m_cUnitClass != CXSLUnit::UC_NONE && sRoomUserInfo.m_cUnitClass != vit->m_cUnitClass )
							continue;

						if( GetPaymentItemInfo( *vit, spRoomUser->IsGameBang(), kResultItemInfo ) == true )
						{
							DungeonClearPaymentItem.insert( std::make_pair(vit->m_iItemID, kResultItemInfo ) );
							spRoomUser->AddItem( kResultItemInfo.m_iItemID, kResultItemInfo.m_iQuantity );
						}
					}
				}
				else if( vit->m_iMaxLv !=0 && vit->m_iMinLv == 0 )
				{
					if( vit->m_iMaxLv >= sRoomUserInfo.m_ucLevel ) // ���� ���� ������
					{
						KItemInfo kResultItemInfo;
						// ������ ����
						if( vit->m_cUnitClass != CXSLUnit::UC_NONE && sRoomUserInfo.m_cUnitClass != vit->m_cUnitClass )
							continue;

						if( GetPaymentItemInfo( *vit, spRoomUser->IsGameBang(), kResultItemInfo ) == true )
						{
							DungeonClearPaymentItem.insert( std::make_pair(vit->m_iItemID, kResultItemInfo ) );
							spRoomUser->AddItem( kResultItemInfo.m_iItemID, kResultItemInfo.m_iQuantity );
						}
					}
				}
				else	// ����Ʈ ���� ���Ǹ� ���� ���
				{
					KItemInfo kResultItemInfo;
					// ������ ����
					if( vit->m_cUnitClass != CXSLUnit::UC_NONE && sRoomUserInfo.m_cUnitClass != vit->m_cUnitClass )
						continue;

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT
					if( vit->m_iQuestID == 63750 && bAraEvent == false )
						continue;
#endif SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT

					if( GetPaymentItemInfo( *vit, spRoomUser->IsGameBang(), kResultItemInfo ) == true )
					{
						DungeonClearPaymentItem.insert( std::make_pair(vit->m_iItemID, kResultItemInfo ) );
						spRoomUser->AddItem( kResultItemInfo.m_iItemID, kResultItemInfo.m_iQuantity );
					}

				}
			}
		}
		else if( vit->m_iQuestID == 0 )	// ���� ���� ����
		{
			if( vit->m_iMinLv !=0 && vit->m_iMaxLv != 0 )  // Ư�� ���� ������ ����
			{
				if( vit->m_iMinLv <= sRoomUserInfo.m_ucLevel && vit->m_iMaxLv >= sRoomUserInfo.m_ucLevel )
				{
					KItemInfo kResultItemInfo;
					// ������ ����
					if( vit->m_cUnitClass != CXSLUnit::UC_NONE && sRoomUserInfo.m_cUnitClass != vit->m_cUnitClass )
						continue;

					if( GetPaymentItemInfo( *vit, spRoomUser->IsGameBang(), kResultItemInfo ) == true )
					{
						DungeonClearPaymentItem.insert( std::make_pair(vit->m_iItemID, kResultItemInfo ) );
						spRoomUser->AddItem( kResultItemInfo.m_iItemID, kResultItemInfo.m_iQuantity );
					}
				}
			}
			else if( vit->m_iMinLv !=0 && vit->m_iMaxLv == 0 ) // Ư�� ���� �̻��̸� ����
			{
				if( vit->m_iMinLv <= sRoomUserInfo.m_ucLevel ) // ���� ���� ������
				{
					KItemInfo kResultItemInfo;
					// ������ ����
					if( vit->m_cUnitClass != CXSLUnit::UC_NONE && sRoomUserInfo.m_cUnitClass != vit->m_cUnitClass )
						continue;

					if( GetPaymentItemInfo( *vit, spRoomUser->IsGameBang(), kResultItemInfo ) == true )
					{
						DungeonClearPaymentItem.insert( std::make_pair(vit->m_iItemID, kResultItemInfo ) );
						spRoomUser->AddItem( kResultItemInfo.m_iItemID, kResultItemInfo.m_iQuantity );
					}
				}
			}
			else if( vit->m_iMaxLv !=0 && vit->m_iMinLv == 0 ) // Ư�� ���� ���ϸ� ����
			{
				if( vit->m_iMaxLv >= sRoomUserInfo.m_ucLevel ) // ���� ���� ������
				{
					KItemInfo kResultItemInfo;
					// ������ ����
					if( vit->m_cUnitClass != CXSLUnit::UC_NONE && sRoomUserInfo.m_cUnitClass != vit->m_cUnitClass )
						continue;

					if( GetPaymentItemInfo( *vit, spRoomUser->IsGameBang(), kResultItemInfo ) == true )
					{
						DungeonClearPaymentItem.insert( std::make_pair(vit->m_iItemID, kResultItemInfo ) );
						spRoomUser->AddItem( kResultItemInfo.m_iItemID, kResultItemInfo.m_iQuantity );
					}
				}
			}
			else
			{
				KItemInfo kResultItemInfo;
				// ������ ����
				if( vit->m_cUnitClass != CXSLUnit::UC_NONE && sRoomUserInfo.m_cUnitClass != vit->m_cUnitClass )
					continue;

				if( GetPaymentItemInfo( *vit, spRoomUser->IsGameBang(), kResultItemInfo ) == true )
				{
					DungeonClearPaymentItem.insert( std::make_pair(vit->m_iItemID, kResultItemInfo ) );
					spRoomUser->AddItem( kResultItemInfo.m_iItemID, kResultItemInfo.m_iQuantity );
				}
			}
		}
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
	}
}

//{{ 2012. 10. 17	�ڼ���	PaymentItem ���� ���� �߰�
#ifdef SERV_UNPAYMENT_CONDITION
bool KDropTable::AddUnPaymentCondition_LUA( IN int iDungeonID )
{

	KLuaManager luaMgr( GetLuaState() );
	DUNGEON_CLEAR_REWARD sData;

	// ����ġ,VP,ED ���ʽ� �̺�Ʈ

	sData.m_iDungeonID	= iDungeonID;

	// AddPaymentDungeon_LUA �κ��� �ʱ�ȭ ������ ��ġ��ų �� �ֵ��� �����ϴ� ���� �����ϴ�.
	LUA_GET_VALUE( luaMgr,		"iItemID",			sData.m_iItemID,		0 );
	LUA_GET_VALUE( luaMgr,		"fRate",			sData.m_fRate,			0.f );
	LUA_GET_VALUE( luaMgr,		"iQuestID",			sData.m_iQuestID,		0 );
	LUA_GET_VALUE( luaMgr,		"iMinLv",			sData.m_iMinLv,			0 );
	LUA_GET_VALUE( luaMgr,		"iMaxLv",			sData.m_iMaxLv,			0 );
	LUA_GET_VALUE( luaMgr,		"iQuantity",		sData.m_iQuantity,		1 );
	LUA_GET_VALUE( luaMgr,		"cUnitClass",				sData.m_cUnitClass,			CXSLUnit::UC_NONE );
	LUA_GET_VALUE( luaMgr,		"fPcBangUserRate",			sData.m_fPcBangUserRate,	0.f );

	// �Է� �� �˻�(�ʼ����鸸 �˻�)
	//{{ 2012. 05. 9	�ڼ���	����� �� ����
	if( ( iDungeonID < 0 ) || ( sData.m_iItemID <=0 ) || ( sData.m_fRate > 100.f ) || ( sData.m_iQuantity < 1 ) || ( sData.m_fPcBangUserRate > 100.f ))
	{
		START_LOG( cerr, L"���� Ŭ����� ������ ���� [����] ���� ���ð��� �̻��մϴ�." )
			<< BUILD_LOG( iDungeonID )
			<< BUILD_LOG( sData.m_iItemID )
			<< BUILD_LOG( sData.m_fRate )
			<< BUILD_LOG( sData.m_iQuestID )
			<< BUILD_LOG( sData.m_iMinLv )
			<< BUILD_LOG( sData.m_iMaxLv )
			<< BUILD_LOG( sData.m_iQuantity )
			<< BUILD_LOGc( sData.m_cUnitClass )
			<< BUILD_LOG( sData.m_fPcBangUserRate )
			<< END_LOG;

		return false;
	}

	m_setUnPaymentCondition.insert( sData );

	return true;
}
#endif SERV_UNPAYMENT_CONDITION
//}}

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
bool KDropTable::_CheckPaymentCondition(IN const DUNGEON_CLEAR_REWARD& dcr, KRoomUserPtr spRoomUser, IN KRoomUserManagerPtr spRoomUserManager ) const
{
	if( dcr.m_bMustParty == true && spRoomUserManager->GetNumMember() == 1 )
	{
		START_LOG(clog, L"���� Ŭ���� ������ ���� : ������ ���� ���Ѵ�!")
			<< END_LOG;
		return false;
	}

	KRoomUserInfo sRoomUserInfo;
	// ���� ����
	if(dcr.m_bPartyPay == false)
	{
		START_LOG(clog, L"���� Ŭ���� ������ ���� : ���� ���� üũ ����")
			<< END_LOG;

		spRoomUser->GetRoomUserInfo( sRoomUserInfo );

#ifdef SERV_PAYMENT_ITEM_WITH_ALLY_NPC
		if( dcr.m_iAllyNPCID > 0 )
		{
			if( spRoomUser->IsAllyNPC( dcr.m_iAllyNPCID ) == false )
			{
				START_LOG( cwarn, L"NPC ���ᰡ �����ϴ�." )
					<< BUILD_LOGc( dcr.m_iAllyNPCID )
					<< END_LOG;

				return false;
			}
		}
#endif SERV_PAYMENT_ITEM_WITH_ALLY_NPC

#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
		if( dcr.m_iConsumingItemID != 0 )
		{
			if( spRoomUser->HasSpecialItem( dcr.m_iConsumingItemID ) == false )
			{
				START_LOG( cwarn, L"�Ҹ��� �������� �����ϴ�." )
					<< BUILD_LOGc( dcr.m_iConsumingItemID )
					<< END_LOG;

				return false;
			}
		}
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
		return _CheckPaymentConditionDetail(dcr, sRoomUserInfo, spRoomUserManager);
	}
	else// ��Ƽ ����
	{
		START_LOG(clog, L"���� Ŭ���� ������ ���� : ��Ƽ ���� üũ ����")
			<< END_LOG;
		for( int i = 0; i < spRoomUserManager->GetNumMember(); ++i )
		{
			KRoomUserPtr spRoomUserForParty = spRoomUserManager->GetUser( i );// NULL üũ�� �տ��� �̸� �ϴϱ� �ʿ� �����~
			spRoomUserForParty->GetRoomUserInfo( sRoomUserInfo );
			if( _CheckPaymentConditionDetail(dcr, sRoomUserInfo, spRoomUserManager) == true)
				return true;
		}
	}

	return false;
}

bool KDropTable::_CheckPaymentConditionDetail(IN const DUNGEON_CLEAR_REWARD& dcr, IN KRoomUserInfo& sRoomUserInfo, IN KRoomUserManagerPtr spRoomUserManager ) const
{
	// 1. Ÿ��Ʋ üũ
	if( _CheckTitlePass( sRoomUserInfo, spRoomUserManager, dcr ) == false )
		return false;

	START_LOG(clog2, L"���� Ŭ���� ������ ���� : ���� �׸� üũ 1 - Ÿ��Ʋ ���")
		<< END_LOG;

	// 2. ����Ʈ üũ
	if(dcr.m_iQuestID != 0)
	{
		std::set< int >::iterator qsit = sRoomUserInfo.m_setQuestInfo.find( dcr.m_iQuestID );
		if( qsit == sRoomUserInfo.m_setQuestInfo.end() ) // ����Ʈ�� �����ϰ� ���� �ʴ�
			return false;
	}

#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
	if( dcr.m_iGoingQuestID != 0 )
	{
		std::set< int >::iterator qsit = sRoomUserInfo.m_setGoingQuestInfo.find( dcr.m_iGoingQuestID );
		if( qsit == sRoomUserInfo.m_setGoingQuestInfo.end() ) // ����Ʈ�� �����ϰ� ���� �ʴ�
			return false;
	}
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST

	START_LOG(clog2, L"���� Ŭ���� ������ ���� : ���� �׸� üũ 2 -����Ʈ ���")
		<< END_LOG;

	// 3. ���� üũ
	if((dcr.m_iMinLv <= sRoomUserInfo.m_ucLevel && dcr.m_iMaxLv >= sRoomUserInfo.m_ucLevel) == false)
		return false;

	START_LOG(clog2, L"���� Ŭ���� ������ ���� : ���� �׸� üũ 3 - ���� ���")
		<< END_LOG;
	
	if(dcr.m_eUnitType != CXSLUnit::UT_NONE)
	{
		// 4. ĳ���� üũ
		if(dcr.m_eUnitType != CXSLUnit::GetUnitClassToUnitType(static_cast<CXSLUnit::UNIT_CLASS>(sRoomUserInfo.m_cUnitClass)))
			return false;
	}

	START_LOG(clog2, L"���� Ŭ���� ������ ���� : ���� �׸� üũ 4 - ĳ���� ���")
		<< END_LOG;

	if(dcr.m_cUnitClass != CXSLUnit::UC_NONE && dcr.m_cUnitClass != sRoomUserInfo.m_cUnitClass)
	{
		return false;
	}

	START_LOG(clog2, L"���� Ŭ���� ������ ���� : ���� �׸� üũ 4 - Ŭ���� ���")
		<< END_LOG;
	
	// 5. �������� üũ
	if(dcr.m_bComeback != false)
	{
		if(sRoomUserInfo.m_bComeBackUser != true)
			return false;
	}
	START_LOG(clog2, L"���� Ŭ���� ������ ���� : ���� �׸� üũ 5 - �������� ���")
		<< END_LOG;

	// ���� �Ǽ��縮 üũ
	if( _CheckEquippedItemPass( sRoomUserInfo, spRoomUserManager, dcr ) == false)
		return false;

	// 6. ��� ������ üũ
	if(dcr.m_iUseItemID != 0)
	{
		if(sRoomUserInfo.m_bUseItem != true)
			return false;
	}
	START_LOG(clog2, L"���� Ŭ���� ������ ���� : ���� �׸� üũ 6 - ������ ��� ���")
		<< END_LOG;

	START_LOG(clog2, L"���� Ŭ���� ������ ���� : ���� �׸� üũ ��� ���!!")
		<< END_LOG;

	return true;
}

bool KDropTable::_CheckEquippedItem(IN KRoomUserInfo& sRoomUserInfo, IN const int iItemID) const
{
	std::map< int, KInventoryItemInfo >::const_iterator mit = sRoomUserInfo.m_mapEquippedItem.begin();
	while(mit != sRoomUserInfo.m_mapEquippedItem.end())
	{
		if(mit->second.m_kItemInfo.m_iItemID == iItemID)
			return true;
		mit++;
	}
	return false;
}
// ��� ������ üũ Ȯ�ο�
bool KDropTable::CheckPaymentUseItem(IN const int iItemID )
{
	std::map< int, std::vector< DUNGEON_CLEAR_REWARD > >::iterator mit;

	mit = m_mapPaymentItemTable.begin();
	for(; mit != m_mapPaymentItemTable.end(); ++mit)
	{
		if( _CheckPaymentUseItemDetail(mit, iItemID ) == true)
			return true;
	}

	return false;
}

bool KDropTable::_CheckPaymentUseItemDetail( std::map< int, std::vector< DUNGEON_CLEAR_REWARD > >::iterator& mit, IN const int iItemID) const
{
	std::vector< DUNGEON_CLEAR_REWARD >::const_iterator vit;
	vit = mit->second.begin();
	for( ; vit != mit->second.end() ; ++vit )
	{
		if( vit->m_iUseItemID == iItemID )
			return true;
	}

	return false;
}

bool KDropTable::_CheckEquippedItemPass( IN KRoomUserInfo& sRoomUserInfo, IN KRoomUserManagerPtr spRoomUserManager, IN const DUNGEON_CLEAR_REWARD& dcr ) const
{
	bool bPassEquippedItem =  false;
	if(dcr.m_iEquippedItemID1 != 0)
	{
		if(_CheckEquippedItem(sRoomUserInfo,  dcr.m_iEquippedItemID1) == true)
		{
			if( dcr.m_iEquippedItemID2 == 0 )
			{
				// ������ ������ 1���� ��� ������ �����ϰ� ������ OK
				START_LOG(clog2, L"���� Ŭ���� ������ ���� : ���� �׸� üũ 5 - ������ ���� ���(ȥ��)")
					<< END_LOG;
				bPassEquippedItem = true;
			}
			else
			{
				// �ٸ� ������ �����ߴ��� ã��.
				for( int i = 0; i < spRoomUserManager->GetNumMember(); ++i )
				{
					KRoomUserInfo tempRoomUserInfo;
					KRoomUserPtr spRoomUserForParty = spRoomUserManager->GetUser( i );
					spRoomUserForParty->GetRoomUserInfo( tempRoomUserInfo );
					if(_CheckEquippedItem(tempRoomUserInfo, dcr.m_iEquippedItemID2 ) == true)
					{
						START_LOG(clog2, L"���� Ŭ���� ������ ���� : ���� �׸� üũ 5 - ������ ���� ���(��)")
							<< END_LOG;
						bPassEquippedItem = true;
						break;
					}
				}
			}
		}
		else if( _CheckEquippedItem(sRoomUserInfo,  dcr.m_iEquippedItemID2) == true )
		{
			if( dcr.m_iEquippedItemID1 == 0 )
			{
				START_LOG(cerr, L"���� Ŭ���� ������ ���� : 2��° ������ ���� �Ǿ� �ִµ� 1��° ������ID�� ����.")
					<< END_LOG;
			}
			else
			{
				// �ٸ� ������ �����ߴ��� ã��.
				for( int i = 0; i < spRoomUserManager->GetNumMember(); ++i )
				{
					KRoomUserInfo tempRoomUserInfo;
					KRoomUserPtr spRoomUserForParty = spRoomUserManager->GetUser( i );
					spRoomUserForParty->GetRoomUserInfo( tempRoomUserInfo );
					if(_CheckEquippedItem(tempRoomUserInfo, dcr.m_iEquippedItemID1 ) == true)
					{
						START_LOG(clog2, L"���� Ŭ���� ������ ���� : ���� �׸� üũ 5 - ������ ���� ���(��)")
							<< END_LOG;
						bPassEquippedItem = true;
						break;
					}
				}
			}
		}
	}
	else
		bPassEquippedItem = true;

	return bPassEquippedItem;
}

bool KDropTable::_CheckTitlePass( IN KRoomUserInfo& sRoomUserInfo, IN KRoomUserManagerPtr spRoomUserManager, IN const DUNGEON_CLEAR_REWARD& dcr ) const
{
	bool bPassTitle =  false;
	if(dcr.m_iTitleID != 0)
	{
		if( dcr.m_iTitleID == sRoomUserInfo.m_iTitleID )
		{
			if( dcr.m_iTitleID2 == 0 )
			{
				// Ÿ��Ʋ ������ 1���� ��� ������ �����ϰ� ������ OK
				START_LOG(clog2, L"���� Ŭ���� ������ ���� : ���� �׸� üũ 1 - Ÿ��Ʋ ���(ȥ��)")
					<< END_LOG;
				bPassTitle = true;
			}
			else
			{
				// �ٸ� ������ �����ߴ��� ã��.
				for( int i = 0; i < spRoomUserManager->GetNumMember(); ++i )
				{
					KRoomUserInfo tempRoomUserInfo;
					KRoomUserPtr spRoomUserForParty = spRoomUserManager->GetUser( i );
					spRoomUserForParty->GetRoomUserInfo( tempRoomUserInfo );
					if( dcr.m_iTitleID2 == tempRoomUserInfo.m_iTitleID )
					{
						START_LOG(clog2, L"���� Ŭ���� ������ ���� : ���� �׸� üũ 1 - Ÿ��Ʋ ���(��)")
							<< END_LOG;
						bPassTitle = true;
						break;
					}
				}
			}
		}
		else if( dcr.m_iTitleID2 != 0 && dcr.m_iTitleID2 == sRoomUserInfo.m_iTitleID )
		{
			if( dcr.m_iTitleID == 0 )
			{
				START_LOG(cerr, L"���� Ŭ���� ������ ���� : 2��° Ÿ��Ʋ ���� �Ǿ� �ִµ� 1��° Ÿ��ƲID�� ����.")
					<< END_LOG;
			}
			else
			{
				// �ٸ� ������ �����ߴ��� ã��.
				for( int i = 0; i < spRoomUserManager->GetNumMember(); ++i )
				{
					KRoomUserInfo tempRoomUserInfo;
					KRoomUserPtr spRoomUserForParty = spRoomUserManager->GetUser( i );
					spRoomUserForParty->GetRoomUserInfo( tempRoomUserInfo );
					if( dcr.m_iTitleID == tempRoomUserInfo.m_iTitleID )
					{
						START_LOG(clog2, L"���� Ŭ���� ������ ���� : ���� �׸� üũ 1 - Ÿ��Ʋ ���� ���(��)")
							<< END_LOG;
						bPassTitle = true;
						break;
					}
				}
			}
		}
	}
	else
		bPassTitle = true;

	return bPassTitle;
}
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX

#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
//}}

//{{ 2011. 08. 23	������	��ϸ� �ð� ����
#ifdef SERV_NEW_HENIR_TEST
bool KDropTable::AddHenirSpecialNpcDropInfo_LUA( int iNpcID, int iItemID, int iQuantity )
{
    if( iNpcID < 0  ||  iItemID <= 0  ||  iQuantity < 0 )
	{
        START_LOG( cerr, L"��ϸ� �ð� NPC Ư�� ��� ������ �̻��մϴ�." )
			<< BUILD_LOG( iNpcID )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iQuantity )
			<< END_LOG;
		return false;
	}

	std::map< int, SHenirSpecialNpcDropInfo >::iterator mit;
	mit = m_mapHenirSpecialNpcDrop.find( iNpcID );
	if( mit == m_mapHenirSpecialNpcDrop.end() )
	{
		SHenirSpecialNpcDropInfo kInfo;
		kInfo.m_mapDropInfo.insert( std::make_pair( iItemID, iQuantity ) );
		m_mapHenirSpecialNpcDrop.insert( std::make_pair( iNpcID, kInfo ) );
	}
	else
	{
		if( mit->second.m_mapDropInfo.find( iItemID ) != mit->second.m_mapDropInfo.end() )
		{
			START_LOG( cerr, L"�̹� ��ϵ� ��ϸ� �ð� NPC Ư�� ��� �����Դϴ�!" )
				<< BUILD_LOG( iNpcID )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iQuantity )
				<< END_LOG;
			return false;
		}

		mit->second.m_mapDropInfo.insert( std::make_pair( iItemID, iQuantity ) );
	}

	return true;
}

//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
void KDropTable::GetHenirSpecialNpcDropInfo( IN const int iNpcID,IN float fTotalDropRate, OUT DROP_DATA& sDropDataHenir )
#else
void KDropTable::GetHenirSpecialNpcDropInfo( IN const int iNpcID, OUT DROP_DATA& sDropDataHenir )
#endif SERV_REFORM_ITEM_DROP
//}}
{
	if( fTotalDropRate <= 0.f)
		return;

	std::map< int, SHenirSpecialNpcDropInfo >::const_iterator mit;
	mit = m_mapHenirSpecialNpcDrop.find( iNpcID );
	if( mit == m_mapHenirSpecialNpcDrop.end() )
	{
		START_LOG( clog, L"�ش� ���Ϳ� ���� Ư�� ��� ������ �����ϴ�!" )
			<< BUILD_LOG( iNpcID )
			<< END_LOG;
		return;
	}

	std::map< int, int >::const_iterator mitSD;
	for( mitSD = mit->second.m_mapDropInfo.begin(); mitSD != mit->second.m_mapDropInfo.end(); ++mitSD )
	{
		const int iDropItemID = mitSD->first;
		const int iDropCnt = mitSD->second;

		for( int iCnt = 0; iCnt < iDropCnt; ++iCnt )
		{
			sDropDataHenir.m_vecItemID.push_back( iDropItemID );
		}
	}
}

#endif SERV_NEW_HENIR_TEST
//}}

//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
bool KDropTable::GetComeBackBuffDropCount( IN const int& iRoomMember )
{
	// Ȯ�� ���
	float fRate = 0.0f;

	switch( iRoomMember )
	{
	case 1: fRate = 1.3f;  break;
	case 2: fRate = 2.2f;  break;
	case 3: fRate = 3.1f;  break;
	case 4: fRate = 4.0f;  break;
	default:
		{
			START_LOG( cerr, L"��Ƽ �ο����� �߸� �Ǿ���" )
				<< BUILD_LOG( iRoomMember )
				<< BUILD_LOG( fRate )
				<< END_LOG;
			return false;
		}
	}

	KLottery Cal;
	Cal.AddCase( 1, fRate );

	if( Cal.Decision() == KLottery::CASE_BLANK )
	{
		START_LOG( clog, L"ȹ�� Ȯ�� ��� - ���� Ȯ�� �̴޷� ���� ���� ����" )
			<< BUILD_LOG( iRoomMember )
			<< BUILD_LOG( fRate )
			<< END_LOG;

		return false;
	}

	return true;	// �ϳ� ����
}
#endif SERV_COME_BACK_USER_REWARD
//}}

//{{ 2011. 11. 22	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
//{{ �ʵ� ��� ���� - ��μ�
//#ifdef SERV_REFORM_ITEM_DROP
bool KDropTable::GetBattleFieldNpcDropInfo( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN const int iNpcID, OUT KBattleFieldNpcDropInfo& kDropInfo )
{
	// �켱 �ش� BattleFieldID + MonsterID�� �ش��ϴ� ��� ������ �˻��Ѵ�.
	std::map< std::pair< int, int >, KBattleFieldNpcDropInfo >::const_iterator mit;
	mit = m_mapBattleFieldNpcDrop.find( std::make_pair( eBattleFieldID, iNpcID ) );

	// �ش� ��쿡 ���� ������ �������� ����.
	if( mit == m_mapBattleFieldNpcDrop.end() )
	{
		// ���� ���̵� ������ ���� ���� ������ �����ϴ��� �ٽ� �ѹ� Ȯ��.
		mit = m_mapBattleFieldNpcDrop.find( std::make_pair( 0, iNpcID ) );

		// �װ͸��� ���ٸ�?
		if( mit == m_mapBattleFieldNpcDrop.end() )
		{
			return false;
		}
	}
    
	kDropInfo = mit->second;
    return true;
}

bool KDropTable::BattleFieldNpcDropItem( IN const KBattleFieldNpcDropInfo& kDropInfo
										//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
									   , IN const float fFieldEDFactor
									   , IN const float fFieldEXPFactor
#else
									   , IN const float fFieldFactor
#endif SERV_FIELD_ED_EXP_FACTOR
										//}
									   , IN const float fLevelFactor
									   , IN const float fUserContribution
									   , IN const int iPartyCount_	// kimhc // ����ö // 2013-12-16 // �⿩�� ���� �۾�
									   , IN const float fPartyDropBonus
										//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
									   , IN const float fDropRate
#else // SERV_DROP_EVENT_RENEWAL
									   , IN const int iDropCount
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
										//}}
									   , IN const bool bIsBoss
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-14	// �ڼ���
									   , IN const float fBattleFieldBossFactor
#endif // SERV_BATTLE_FIELD_BOSS
									   , OUT DROP_DATA& sDropData
									   )
{
	//////////////////////////////////////////////////////////////////////////
	// X = ������ �ص����� / ����HP (X ���� �ְ� 1.5�� ���� �ʴ´�)
	// Y = NPC�� ���� �ִ� EXP
	// Z = NPC�� ���� �ִ� ED
	// B = ��Ƽ ���ʽ� = NPC���� ����ġ / ������ ������ �ִ¡� ��Ƽ�� �� * ������ ������ �ִ¡� ��Ƽ�� ���� ���� ��Ƽ�� ��� * ���ſ����� ������ ���̵� ���
	// A = �ʵ� ��� = SiCXSLBattleFieldManager()->GetBattleFieldFactor()
	// F = ���� ���� = ���Ϳ��� �������� +-10���� �̻��̸�, ������ ȹ���� �� ����
	// T = ��� ���̺� Ȯ��
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// EXP, ED

	// EXP = Y * X * A * F
	// ED =  Z * X * A * F
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ��� ������

	// ������ ���(ȹ��) Ȯ�� = T * X * A * ( B * 0.5 ) * F
	//////////////////////////////////////////////////////////////////////////

	sDropData.Clear();

	//////////////////////////////////////////////////////////////////////////	
	// NPC�κ��� ��ԵǴ� EXP
	float fTemp;

	fTemp = kDropInfo.m_iExp * fUserContribution * fLevelFactor;

	//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
	fTemp *= fFieldEXPFactor;
#else
	fTemp *= fFieldFactor;
#endif SERV_FIELD_ED_EXP_FACTOR
	//}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-14	// �ڼ���
	fTemp *= fBattleFieldBossFactor;
#endif // SERV_BATTLE_FIELD_BOSS

	sDropData.m_iExp = static_cast<int>( fTemp );

	//////////////////////////////////////////////////////////////////////////	
	// NPC�κ��� ��ԵǴ� ED
	fTemp = kDropInfo.m_iED * fUserContribution * fLevelFactor;
	
	//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
	fTemp *= fFieldEDFactor;
#else
	fTemp *= fFieldFactor;
#endif SERV_FIELD_ED_EXP_FACTOR
	//}

	//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
	fTemp *= fDropRate;
#else
	fTemp *= iDropCount;
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
	//}}

	int iGetTotalED = static_cast<int>( fTemp );

	//////////////////////////////////////////////////////////////////////////	
	// NPC�κ��� ��ԵǴ� ED
	if( ( (float)rand() / RAND_MAX ) < kDropInfo.m_fEDProperty )
	{
		if( true == bIsBoss )
		{
			sDropData.m_nGPNum		= ( rand() % 11 + 10 ); // [10,20]
			sDropData.m_iED			= ( sDropData.m_nGPNum > 0 ? iGetTotalED / sDropData.m_nGPNum : 0 );
			sDropData.m_iGPItemID	= GetEDItemID( sDropData.m_iED );
		}
		else
		{
			sDropData.m_nGPNum      = rand() % 2 + 2;    // [2,3]
			sDropData.m_iED			= ( sDropData.m_nGPNum > 0 ? iGetTotalED / sDropData.m_nGPNum : 0 );
			sDropData.m_iGPItemID   = GetEDItemID( sDropData.m_iED );
		}

		KManyEDDropNpcInfo kManyDropInfo;
		if( GetManyEDDropNpcInfo( kDropInfo.m_nMonsterID, kManyDropInfo ) == true )
		{
			sDropData.m_nGPNum		= static_cast<int>(static_cast<float>(sDropData.m_nGPNum) * kManyDropInfo.m_fMultiplyDropEDCount);
			sDropData.m_iED			= static_cast<int>(static_cast<float>(sDropData.m_iED) * kManyDropInfo.m_fMultiplyDropED);
			sDropData.m_iGPItemID	= GetEDItemID( sDropData.m_iED );
		}
	}

	//////////////////////////////////////////////////////////////////////////	
	// ������ ������.
	if( fUserContribution <= 0.0f )
		return true;

	// �������� ���� ���̿� ������� ��ӵ��� ����
	//if( fLevelFactor <= 0.0f )
	//	return true;

	KLottery kDropInfoCopy = kDropInfo.m_kLottery;
	kDropInfoCopy.AddMultiProbRate( fUserContribution * iPartyCount_ );
	//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
	kDropInfoCopy.AddMultiProbRate( fFieldEXPFactor );
#else
	kDropInfoCopy.AddMultiProbRate( fFieldFactor );
#endif SERV_FIELD_ED_EXP_FACTOR
	//}
//	kDropInfoCopy.AddMultiProbRate( fPartyDropBonus );

	//{{ 2012. 12. 16  ��� �̺�Ʈ - ��μ�
#ifdef SERV_ITEM_DROP_EVENT

#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
	{
		float fDropRateCopy = fDropRate;

		while( 0.0f < fDropRateCopy )
		{
			if( fDropRateCopy < 1.0f )
			{
				kDropInfoCopy.AddMultiProbRate( fDropRateCopy );
				fDropRateCopy = 0.0f;
			}
			else
			{
				fDropRateCopy -= 1.0f;
			}

			const int nCaseID = kDropInfoCopy.Decision();
			if( nCaseID == KLottery::CASE_BLANK )   // ȹ���� ������ ����.
				continue;

			const int nCaseParam = kDropInfoCopy.GetParam1( nCaseID );

			switch( nCaseParam )    // ��ǰ ������, ������ �׷�, Ȥ�� ���� ���� ����.
			{
			case DCP_UNARY_ITEM:    // ��ǰ ������
				{
					sDropData.m_vecItemID.push_back( nCaseID );
				}			
				break;
			case DCP_ITEM_GROUP:    // ������ �׷�.
				{
					std::map< int, KLottery >::const_iterator mit;
					mit = m_mapDropGroup.find( nCaseID );
					if( mit == m_mapDropGroup.end() )
					{
						START_LOG( cerr, L"������ �׷� ���� ����" )
							<< BUILD_LOG( nCaseID )
							<< BUILD_LOG( nCaseParam )
							<< BUILD_LOG( m_mapDropGroup.size() );
						continue;
					}

					// ������ �׷� �߿��� ���� ������ �ϳ� ����.
					const int iResultItemID = mit->second.Decision();
					if( iResultItemID == KLottery::CASE_BLANK )   // ȹ���� ������ ����.
						continue;

					sDropData.m_vecItemID.push_back( iResultItemID );
				}
				break;
			default:
				{
					START_LOG( cerr, L"Never Get Here ~" )
						<< BUILD_LOG( nCaseParam )
						<< BUILD_LOG( nCaseID )
						<< BUILD_LOG( kDropInfo.m_kLottery.GetCaseNum() )
						<< BUILD_LOG( kDropInfo.m_kLottery.GetTotalProb() )
						<< END_LOG;
					continue;
				}
			}
		}
	}
#else // SERV_DROP_EVENT_RENEWAL
	for( int iCount = 0 ; iCount < iDropCount ; ++iCount )
	{
		const int nCaseID = kDropInfoCopy.Decision();
		if( nCaseID == KLottery::CASE_BLANK )   // ȹ���� ������ ����.
			continue;

		const int nCaseParam = kDropInfoCopy.GetParam1( nCaseID );

		switch( nCaseParam )    // ��ǰ ������, ������ �׷�, Ȥ�� ���� ���� ����.
		{
		case DCP_UNARY_ITEM:    // ��ǰ ������
			{
				sDropData.m_vecItemID.push_back( nCaseID );
			}			
			break;
		case DCP_ITEM_GROUP:    // ������ �׷�.
			{
				std::map< int, KLottery >::const_iterator mit;
				mit = m_mapDropGroup.find( nCaseID );
				if( mit == m_mapDropGroup.end() )
				{
					START_LOG( cerr, L"������ �׷� ���� ����" )
						<< BUILD_LOG( nCaseID )
						<< BUILD_LOG( nCaseParam )
						<< BUILD_LOG( m_mapDropGroup.size() );
					continue;
				}

				// ������ �׷� �߿��� ���� ������ �ϳ� ����.
				const int iResultItemID = mit->second.Decision();
				if( iResultItemID == KLottery::CASE_BLANK )   // ȹ���� ������ ����.
					continue;

				sDropData.m_vecItemID.push_back( iResultItemID );
			}
			break;
		default:
			{
				START_LOG( cerr, L"Never Get Here ~" )
					<< BUILD_LOG( nCaseParam )
					<< BUILD_LOG( nCaseID )
					<< BUILD_LOG( kDropInfo.m_kLottery.GetCaseNum() )
					<< BUILD_LOG( kDropInfo.m_kLottery.GetTotalProb() )
					<< END_LOG;
				continue;
			}
		}
	}
#endif // SERV_DROP_EVENT_RENEWAL

#else
	const int nCaseID = kDropInfoCopy.Decision();
	if( nCaseID == KLottery::CASE_BLANK )   // ȹ���� ������ ����.
		return true;

	const int nCaseParam = kDropInfoCopy.GetParam1( nCaseID );

	switch( nCaseParam )    // ��ǰ ������, ������ �׷�, Ȥ�� ���� ���� ����.
	{
	case DCP_UNARY_ITEM:    // ��ǰ ������
		{
			sDropData.m_vecItemID.push_back( nCaseID );
		}			
		break;
	case DCP_ITEM_GROUP:    // ������ �׷�.
		{
			std::map< int, KLottery >::const_iterator mit;
			mit = m_mapDropGroup.find( nCaseID );
			if( mit == m_mapDropGroup.end() )
			{
				START_LOG( cerr, L"������ �׷� ���� ����" )
					<< BUILD_LOG( nCaseID )
					<< BUILD_LOG( nCaseParam )
					<< BUILD_LOG( m_mapDropGroup.size() );
				return false;
			}

			// ������ �׷� �߿��� ���� ������ �ϳ� ����.
			const int iResultItemID = mit->second.Decision();
			if( iResultItemID == KLottery::CASE_BLANK )   // ȹ���� ������ ����.
				break;

			sDropData.m_vecItemID.push_back( iResultItemID );
		}
		break;
	default:
		START_LOG( cerr, L"Never Get Here ~" )
			<< BUILD_LOG( nCaseParam )
			<< BUILD_LOG( nCaseID )
			<< BUILD_LOG( kDropInfo.m_kLottery.GetCaseNum() )
			<< BUILD_LOG( kDropInfo.m_kLottery.GetTotalProb() )
			<< END_LOG;
		return false;
	}
#endif SERV_ITEM_DROP_EVENT
	//}}

	return true;
}

bool KDropTable::BattleFieldStaticDropItem( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, OUT DROP_DATA& sDropData, const float fUserContribution )
{
	sDropData.m_vecItemID.clear();
	sDropData.m_iExp        = -1;
	sDropData.m_iGPItemID   = -1;
	sDropData.m_iED         = -1;
	sDropData.m_nGPNum      = -1;

	std::map< int, KBattleFieldStaticDropInfo >::const_iterator mit;
	mit = m_mapBattleFieldStaticDrop.find( eBattleFieldID );
	if( mit == m_mapBattleFieldStaticDrop.end() )
	{
		START_LOG( clog, L"�ش� ������ ���� ��ü��� ������ ����" )
			<< BUILD_LOG( eBattleFieldID );
		return true;
	}

	if( fUserContribution <= 0.0f )
		return true;

	KLottery kDropInfoCopy = mit->second.m_kLottery;
	kDropInfoCopy.AddMultiProbRate( fUserContribution );

	int iResultItemID = kDropInfoCopy.Decision();
	if( iResultItemID != KLottery::CASE_BLANK )
	{
		// ������ ���!
		sDropData.m_vecItemID.push_back( iResultItemID );
	}

	return true;
}

#else			// SERV_REFORM_ITEM_DROP

bool KDropTable::BattleFieldNpcDropItem( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
										IN const int iNpcID, 
										IN const char cNpcLevel, 
										IN const char cMonsterTypeFactor, 
										OUT DROP_DATA& sDropData, 
										IN const int iItemDropNum /*= 1*/, 
										IN const bool bIsBoss /*= false*/ )
{
	sDropData.Clear();

	// �켱 �ش� BattleFieldID + MonsterID�� �ش��ϴ� ��� ������ �˻��Ѵ�.
	std::map< std::pair< int, int >, KBattleFieldNpcDropInfo >::const_iterator mit;
	mit = m_mapBattleFieldNpcDrop.find( std::make_pair( eBattleFieldID, iNpcID ) );

	// �ش� ��쿡 ���� ������ �������� ����.
	if( mit == m_mapBattleFieldNpcDrop.end() )
	{
		// ���� ���̵� ������ ���� ���� ������ �����ϴ��� �ٽ� �ѹ� Ȯ��.
		mit = m_mapBattleFieldNpcDrop.find( std::make_pair( 0, iNpcID ) );

		// �װ͸��� ���ٸ�?
		if( mit == m_mapBattleFieldNpcDrop.end() )
		{
			// �������� ����. �����϶��� false.
			return true;
		}
	}

	const KBattleFieldNpcDropInfo& kDropInfo = mit->second;

	//////////////////////////////////////////////////////////////////////////	
	// NPC�κ��� ��ԵǴ� EXP
	sDropData.m_iExp = kDropInfo.m_iExp;

	//////////////////////////////////////////////////////////////////////////	
	// NPC�κ��� ��ԵǴ� ED
	if( (float)rand() / RAND_MAX < kDropInfo.m_fEDProperty )
	{
		//{{ 2008. 11. 17  ������	EDȹ�� ����		
		const float fPercent = (float)( ( rand() % 20 ) + 90 ) / 100.f;

		//{{ 2013. 03. 26	 �ʵ� ED, EXP ���� �߰� - ��μ�
#ifdef SERV_FIELD_ED_EXP_FACTOR
		const int iGetTotalED = (int)( (float)( 10 + ( kDropInfo.m_iED * (int)cNpcLevel ) ) * fPercent * (float)cMonsterTypeFactor * SiCXSLBattleFieldManager()->GetBattleFieldEXPFactor() );
#else
		const int iGetTotalED = (int)( (float)( 10 + ( kDropInfo.m_iED * (int)cNpcLevel ) ) * fPercent * (float)cMonsterTypeFactor * SiCXSLBattleFieldManager()->GetBattleFieldFactor() );
#endif SERV_FIELD_ED_EXP_FACTOR
		//}

		if( true == bIsBoss )
		{
			sDropData.m_nGPNum		= ( rand() % 11 + 10 ); // [10,20]
			sDropData.m_iED			= ( sDropData.m_nGPNum > 0 ? iGetTotalED / sDropData.m_nGPNum : 0 );
			sDropData.m_iGPItemID	= GetEDItemID( sDropData.m_iED );
		}
		else
		{
			sDropData.m_nGPNum      = rand() % 2 + 2;    // [2,3]
			sDropData.m_iED			= ( sDropData.m_nGPNum > 0 ? iGetTotalED / sDropData.m_nGPNum : 0 );
			sDropData.m_iGPItemID   = GetEDItemID( sDropData.m_iED );
		}

		//{{ 2011. 03. 28	������	���� ���� üũ
#ifdef SERV_MANY_ED_DROP_EVENT_NPC
		KManyEDDropNpcInfo kManyDropInfo;
		if( GetManyEDDropNpcInfo( iNpcID, kManyDropInfo ) == true )
		{
			sDropData.m_nGPNum		= static_cast<int>(static_cast<float>(sDropData.m_nGPNum) * kManyDropInfo.m_fMultiplyDropEDCount);
			sDropData.m_iED			= static_cast<int>(static_cast<float>(sDropData.m_iED) * kManyDropInfo.m_fMultiplyDropED);
			sDropData.m_iGPItemID	= GetEDItemID( sDropData.m_iED );
		}
#endif SERV_MANY_ED_DROP_EVENT_NPC
		//}}
	}

	//////////////////////////////////////////////////////////////////////////	
	// ������ ������.
	for( int i = 0; i < iItemDropNum; ++i )
	{
		const int nCaseID = kDropInfo.m_kLottery.Decision();
		if( nCaseID == KLottery::CASE_BLANK )   // ȹ���� ������ ����.
			return true;

		const int nCaseParam = kDropInfo.m_kLottery.GetParam1( nCaseID );

		switch( nCaseParam )    // ��ǰ ������, ������ �׷�, Ȥ�� ���� ���� ����.
		{
		case DCP_UNARY_ITEM:    // ��ǰ ������
			{
				sDropData.m_vecItemID.push_back( nCaseID );
			}			
			break;
		case DCP_ITEM_GROUP:    // ������ �׷�.
			{
				std::map< int, KLottery >::const_iterator mit;
				mit = m_mapDropGroup.find( nCaseID );
				if( mit == m_mapDropGroup.end() )
				{
					START_LOG( cerr, L"������ �׷� ���� ����" )
						<< BUILD_LOG( nCaseID )
						<< BUILD_LOG( nCaseParam )
						<< BUILD_LOG( m_mapDropGroup.size() );
					return false;
				}

				// ������ �׷� �߿��� ���� ������ �ϳ� ����.
				const int iResultItemID = mit->second.Decision();
				if( iResultItemID == KLottery::CASE_BLANK )   // ȹ���� ������ ����.
					break;

				sDropData.m_vecItemID.push_back( iResultItemID );
			}
			break;
		default:
			START_LOG( cerr, L"Never Get Here ~" )
				<< BUILD_LOG( nCaseParam )
				<< BUILD_LOG( nCaseID )
				<< BUILD_LOG( kDropInfo.m_kLottery.GetCaseNum() )
				<< BUILD_LOG( kDropInfo.m_kLottery.GetTotalProb() )
				<< END_LOG;
			return false;
		}
	}

	return true;
}

bool KDropTable::BattleFieldStaticDropItem( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, OUT DROP_DATA& sDropData, IN int iItemDropNum /*= 1*/ )
{
	sDropData.m_vecItemID.clear();
	sDropData.m_iExp        = -1;
	sDropData.m_iGPItemID   = -1;
	sDropData.m_iED         = -1;
	sDropData.m_nGPNum      = -1;

	std::map< int, KBattleFieldStaticDropInfo >::const_iterator mit;
	mit = m_mapBattleFieldStaticDrop.find( eBattleFieldID );
	if( mit == m_mapBattleFieldStaticDrop.end() )
	{
		START_LOG( clog, L"�ش� ������ ���� ��ü��� ������ ����" )
			<< BUILD_LOG( eBattleFieldID );
		return true;
	}

	for( int i = 0; i < iItemDropNum; ++i )
	{
		int iResultItemID = mit->second.m_kLottery.Decision();
		if( iResultItemID != KLottery::CASE_BLANK )
		{
			// ������ ���!
			sDropData.m_vecItemID.push_back( iResultItemID );
		}
	}
	return true;
}
//#endif SERV_REFORM_ITEM_DROP
//}}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 07. 17	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2_2
float KDropTable::CalculateRebirthTime( IN const int& iMyTeamKillNum
									, IN const int& iEnemyTeamKillNum
									, IN const bool& bDidIHaveARebirthRing )
{
	return lua_tinker::call<float>( GetLuaState(), "CALCULATE_REBIRTH_TIME", iMyTeamKillNum, iEnemyTeamKillNum, bDidIHaveARebirthRing );
}

bool KDropTable::SuccessfullyGetTheItem( IN const CXSLRoom::PVP_GAME_TYPE& ePvpGameType, IN const int& iMyTeamKillNum, IN const int& iEnemyTeamKillNum )
{
	return lua_tinker::call<bool>( GetLuaState(), "SUCCESSFULLY_GET_THE_ITEM", ePvpGameType, iMyTeamKillNum, iEnemyTeamKillNum );
}
#endif SERV_2012_PVP_SEASON2_2
//}}

//{{ 2013. 1. 14	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
void KDropTable::SetEnchantItemDropEvent_LUA( void )
{
	if( m_kEnchantItemDropEventInfo.empty() == false )
	{
		START_LOG( cerr, L"��ȭ ������ ��� �̺�Ʈ�� �̹� �����Ǿ� �ֽ��ϴ�.")
			<< BUILD_LOG( m_kEnchantItemDropEventInfo.m_vecEIDESettingInfo.size() )
			<< END_LOG;
		return;
	}

	KLuaManager luaManager( GetLuaState() );

	std::wstring wstrBeginDate;
	LUA_GET_VALUE_RETURN( luaManager,	L"m_wstrBeginDate",	wstrBeginDate,	L"",	return );
	if( KncUtil::ConvertStringToCTime( wstrBeginDate, m_kEnchantItemDropEventInfo.m_tBeginDate ) == false )
	{
		START_LOG( cerr, L"��ȭ ������ ��� �̺�Ʈ ���� �ð� ��ȯ ����")
			<< BUILD_LOG( wstrBeginDate )
			<< END_LOG;
		return;
	}

	std::wstring wstrEndDate;
	LUA_GET_VALUE_RETURN( luaManager,	L"m_wstrEndDate",	wstrEndDate,	L"",	return );
	if( KncUtil::ConvertStringToCTime( wstrEndDate, m_kEnchantItemDropEventInfo.m_tEndDate ) == false )
	{
		START_LOG( cerr, L"��ȭ ������ ��� �̺�Ʈ ���� �ð� ��ȯ ����")
			<< BUILD_LOG( wstrEndDate )
			<< END_LOG;
		return;
	}

	if( m_kEnchantItemDropEventInfo.m_tEndDate <= m_kEnchantItemDropEventInfo.m_tBeginDate )
	{
		START_LOG( cerr, L"��ȭ ������ ��� �̺�Ʈ �ð� ������ �ùٸ��� �ʽ��ϴ�.")
			<< BUILD_LOG( wstrBeginDate )
			<< BUILD_LOG( wstrEndDate )
			<< END_LOG;
		return;
	}

	for( int i=1; luaManager.BeginTable( i ) == S_OK; ++i )
	{
		EIDESettingInfo m_kEIDESettingInfo;
		LUA_GET_VALUE( luaManager,	"iMinLevel",			m_kEIDESettingInfo.m_iMinLevel,			0 );
		LUA_GET_VALUE( luaManager,	"iMaxLevel",			m_kEIDESettingInfo.m_iMaxLevel,			0 );

		for( int i=1; luaManager.BeginTable( i ) == S_OK; ++i )
		{
			char	cEnchantLevel;
			float	fRate;
			bool	bNotify;

			LUA_GET_VALUE_RETURN( luaManager,	"m_cEnchantLevel",		cEnchantLevel,		0,		goto end_proc );
			LUA_GET_VALUE_RETURN( luaManager,	"m_fRate",				fRate,				0.0f,	goto end_proc );
			LUA_GET_VALUE( luaManager,	"m_bNotify",			bNotify,			false );

			LIF( m_kEIDESettingInfo.m_kLottery.AddCase( cEnchantLevel, fRate ) );

			if( bNotify == true )
			{
				m_kEIDESettingInfo.m_setNotify.insert( cEnchantLevel );
			}
			luaManager.EndTable();
		}
		luaManager.EndTable();

		m_kEnchantItemDropEventInfo.m_vecEIDESettingInfo.push_back( m_kEIDESettingInfo );

		START_LOG( cout, L"������ ��� �̺�Ʈ ����" )
			<< BUILD_LOG( m_kEIDESettingInfo.m_iMinLevel )
			<< BUILD_LOG( m_kEIDESettingInfo.m_iMaxLevel )
			<< BUILD_LOG( m_kEIDESettingInfo.m_kLottery.GetTotalProb() )
			<< BUILD_LOG( m_kEIDESettingInfo.m_setNotify.size() )
			<< END_LOG;
	}

	START_LOG( cout, L"������ ��� �̺�Ʈ ���� �Ϸ�" )
		<< BUILD_LOG( wstrBeginDate )
		<< BUILD_LOG( wstrEndDate )
		<< BUILD_LOG( m_kEnchantItemDropEventInfo.m_vecEIDESettingInfo.size() )
		<< END_LOG;

	return;

end_proc:
	// ���� ó��
	m_kEnchantItemDropEventInfo.m_vecEIDESettingInfo.clear();

	START_LOG( cerr, L"��æƮ ���� �� Ȯ�� ������ �о���µ� �����Ͽ����ϴ�." )
		<< END_LOG;
}

void KDropTable::EnchantItemDropEventProcess( IN const int iItemID, OUT char& cEnchantLevel, OUT bool& bNotify )
{
	// �̺�Ʈ ���� �ð����� üũ
	CTime tCurrentTime = CTime::GetCurrentTime();
	if(		( tCurrentTime < m_kEnchantItemDropEventInfo.m_tBeginDate )
		||	( m_kEnchantItemDropEventInfo.m_tEndDate <= tCurrentTime )
		)
	{
		return;
	}

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� �������Դϴ�." )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
		return;
	}

	// ��æƮ �Ұ� �������� �Ѿ��
	if( pItemTemplet->m_bCanEnchant == false )
	{
		return;
	}

	BOOST_TEST_FOREACH( const EIDESettingInfo&, kEIDESettingInfo, m_kEnchantItemDropEventInfo.m_vecEIDESettingInfo )
	{
		if( ( kEIDESettingInfo.m_iMinLevel != 0 ) && ( pItemTemplet->m_UseLevel < kEIDESettingInfo.m_iMinLevel ) )
		{
			continue;
		}

		if( ( kEIDESettingInfo.m_iMaxLevel != 0 ) && ( kEIDESettingInfo.m_iMaxLevel < pItemTemplet->m_UseLevel ) )
		{
			continue;
		}

		cEnchantLevel = static_cast<char>( kEIDESettingInfo.m_kLottery.Decision() );
		if( cEnchantLevel < 0 )
		{
			cEnchantLevel = 0;
		}

		bNotify = kEIDESettingInfo.m_setNotify.find( cEnchantLevel ) != kEIDESettingInfo.m_setNotify.end();
		break;
	}
}
#endif SERV_ENCHANT_ITEM_DROP_EVENT
//}}
