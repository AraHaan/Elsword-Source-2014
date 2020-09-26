#include "RoomMonsterManager.h"
#include "X2Data/XSLUnitManager.h"


//{{ 2011. 01. 20	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW


KRoomMonsterManager::KRoomMonsterManager() : 
m_nNPCUID( 0 )
{
}

KRoomMonsterManager::~KRoomMonsterManager()
{
}

void KRoomMonsterManager::StartGame()
{
	m_nNPCUID = 1;
	m_mapAliveNpcList.clear();
	m_mapDieNpcList.clear();
}

void KRoomMonsterManager::EndGame()
{
    m_mapAliveNpcList.clear();
	m_mapDieNpcList.clear();
}

void KRoomMonsterManager::OnCloseRoom()
{
	m_nNPCUID = 1;
	m_mapAliveNpcList.clear();
	m_mapDieNpcList.clear();
}

bool KRoomMonsterManager::CreateMonster( IN const int iNpcUID, IN const KNPCUnitReq& kNpcInfo )
{
	int iDummy = 0;
    return CreateMonster( kNpcInfo, iDummy, iNpcUID );
}

bool KRoomMonsterManager::CreateMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID, IN const int iNpcUID /*= 0*/ )
{
	if( CXSLUnitManager::IsValidMonsterGrade( static_cast<CXSLUnitManager::MONSTER_GRADE>(kNpcInfo.m_cMonsterGrade) ) == false )
	{
        START_LOG( cerr, L"���� ��� ������ �̻��մϴ�!" )
			<< BUILD_LOG( kNpcInfo.m_NPCID )
			<< BUILD_LOGc( kNpcInfo.m_cMonsterGrade )
			<< END_LOG;
		//return false;
	}	

	// 1. NPC UID �߱�
	if( iNpcUID != 0 )
	{
		iCreatedNpcUID = iNpcUID;
	}
	else
	{
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		iCreatedNpcUID = GetNewNpcUID();
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		
//		iCreatedNpcUID = m_nNPCUID++;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		
	}

	// 3. NPC(Monster) ������ ���� data ����
	NPC_DATA npcData;
	npcData.m_cLevel	= kNpcInfo.m_Level;
	npcData.m_iNPCID	= kNpcInfo.m_NPCID;
	npcData.m_bActive	= kNpcInfo.m_bActive;
	npcData.m_bNoDrop	= kNpcInfo.m_bNoDrop;
	npcData.m_bIsBoss	= ( kNpcInfo.m_cMonsterGrade == CXSLUnitManager::MG_BOSS_NPC );	// ���� ������� ������������ üũ.
	//npcData.m_bIsBoss	= kNPCList.m_NPCList[i].m_bHasBossGage;	//������������ üũ.
	//{{ 2009. 12. 18  ������	��������ġ����
	npcData.m_cMonsterGrade = kNpcInfo.m_cMonsterGrade; //�Ϲݸ����� �߰��������� �������� üũ.
	//}}
	//{{ 2011. 11. 16	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	npcData.m_iGroupID	= kNpcInfo.m_iGroupID;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	m_mapAliveNpcList.insert( std::make_pair( iCreatedNpcUID, npcData ) );
	return true;
}

bool KRoomMonsterManager::GetNpcData( IN const int iNpcUID, OUT NPC_DATA& kNpcData ) const
{
	std::map< int, NPC_DATA >::const_iterator mit;
	mit = m_mapAliveNpcList.find( iNpcUID );
	if( mit == m_mapAliveNpcList.end() )
	{
		// ����ִ� �����߿� ������ ���� ���͸���Ʈ���� ã�´�.
		mit = m_mapDieNpcList.find( iNpcUID );
		if( mit == m_mapDieNpcList.end() )
		{
			return false;
		}

		kNpcData = mit->second;
		return true;
	}

	kNpcData = mit->second;
	return true;
}

bool KRoomMonsterManager::IsExistMonster( IN const int iNpcUID ) const
{
	bool bIsDie = false;
    return IsExistMonster( iNpcUID, bIsDie );
}

bool KRoomMonsterManager::IsExistMonster( IN const int iNpcUID, OUT bool& bIsDie ) const
{
	std::map< int, NPC_DATA >::const_iterator mit;
	mit = m_mapAliveNpcList.find( iNpcUID );	
	if( mit == m_mapAliveNpcList.end() )
	{
		// ����ִ� �����߿� ������ ���� ���͸���Ʈ���� ã�´�.
		mit = m_mapDieNpcList.find( iNpcUID );
		if( mit == m_mapDieNpcList.end() )
		{
			return false;
		}

		bIsDie = true;
		return true;
	}

	bIsDie = false;
	return true;
}

//{{ 2013. 04. 16	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
bool KRoomMonsterManager::IsAllBossMonstersDead() const
{
	std::map< int, NPC_DATA >::const_iterator mit;
	for( mit = m_mapAliveNpcList.begin(); mit != m_mapAliveNpcList.end(); ++mit )
	{
		if( mit->second.m_bIsBoss )
			return false;
	}

	return true;
}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

bool KRoomMonsterManager::SetMonsterDie( IN const int iNpcUID, IN const UidType iAttUnitUID, IN const bool bInsertDieList /*= true*/ )
{
	std::map< int, NPC_DATA >::iterator mit;
	mit = m_mapAliveNpcList.find( iNpcUID );
	if( mit == m_mapAliveNpcList.end() )
	{
		// ����ִ� ���� ����Ʈ�� ������ ����!
		return false;
	}

	// ���� ���� ����Ʈ�� �ִ´�!
	if( bInsertDieList )
	{
		m_mapDieNpcList.insert( std::make_pair( iNpcUID, mit->second ) );
	}

	// ����ִ� ���� ����Ʈ������ ����!
	m_mapAliveNpcList.erase( mit );
	return true;
}

bool KRoomMonsterManager::SetMonsterRebirth( IN const int iNpcUID )
{
	std::map< int, NPC_DATA >::iterator mit;
	mit = m_mapDieNpcList.find( iNpcUID );
	if( mit == m_mapDieNpcList.end() )
	{
		// ����ִ� ���͸� ��Ȱ��ų���� �����ϴ�!
		return false;
	}

	// ���� ���� ����Ʈ�� �ִ´�!
	mit->second.m_uiAttUnit = 0;
	m_mapAliveNpcList.insert( std::make_pair( iNpcUID, mit->second ) );

	// ����ִ� ���� ����Ʈ������ ����!
	m_mapDieNpcList.erase( mit );
	return true;
}

void KRoomMonsterManager::PrintLuaLog_LUA( int iType, double fValue )
{
    START_LOG( cout, L"��� �α� Ȯ��" )
		<< BUILD_LOG( iType )
		<< BUILD_LOG( fValue );
}

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
int KRoomMonsterManager::GetNewNpcUID()
{
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	while( m_setReservedNpcUID.find( m_nNPCUID ) != m_setReservedNpcUID.end() )
		m_nNPCUID++;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		

	return m_nNPCUID++;
}
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#endif SERV_DUNGEON_RANK_NEW
//}}


//{{ 2011. 11. 16	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM

void KRoomMonsterManager::GetAliveMonsterCountInfo( OUT CXSLBattleField::KBattleFieldMonsterCountInfo& kAliveMonsterInfo ) const
{
	std::map< int, NPC_DATA >::const_iterator mit;
	for( mit = m_mapAliveNpcList.begin(); mit != m_mapAliveNpcList.end(); ++mit )
	{
		// ����ִ� ���� ������ ��´�.
		kAliveMonsterInfo.AddGroupMonsterCount( mit->second.m_iGroupID, mit->second.m_iNPCID );
	}
}

bool KRoomMonsterManager::GetMonsterGroupID( IN const int iNpcUID, OUT int& iGroupID ) const
{
	iGroupID = 0;

	std::map< int, NPC_DATA >::const_iterator mit;
	mit	= m_mapAliveNpcList.find( iNpcUID );
	if( mit	== m_mapAliveNpcList.end() )
	{
		START_LOG( cerr, L"�����ϴ� ���Ͱ� �ƴմϴ�!" )
			<< BUILD_LOG( iNpcUID )
			<< END_LOG;
		return false;
	}

	iGroupID = mit->second.m_iGroupID;
	return true;
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}

