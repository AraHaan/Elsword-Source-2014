#include "NetError.h"
#include "GSSimLayer.h"
#include ".\battlefieldlistmanager.h"


//{{ 2011. 10. 27	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


ImplementSingleton( KBattleFieldListManager );

KBattleFieldListManager::KBattleFieldListManager(void)
{
	lua_tinker::class_add<KBattleFieldListManager>( g_pLua, "CXSLItemManager" );
	lua_tinker::class_def<KBattleFieldListManager>( g_pLua, "dump",						&KBattleFieldListManager::Dump );
	lua_tinker::decl( g_pLua, "BattleFieldListManager", this );
}

KBattleFieldListManager::~KBattleFieldListManager(void)
{
}

ImplToStringW( KBattleFieldListManager )
{
	stm_	<< L"----------[ Battle Field List Manager ]----------" << std::endl
			<< TOSTRINGW( m_mapBattleFieldList.size() )
			;

	return stm_;
}

KBattleFieldListPtr KBattleFieldListManager::GetBattleFieldRoomList( IN const int iBattleFieldID )
{
	std::map< int, KBattleFieldListPtr >::iterator mit;
	mit = m_mapBattleFieldList.find( iBattleFieldID );
	if( mit == m_mapBattleFieldList.end() )
	{
		START_LOG( cerr, L"�ش� ��Ʋ�ʵ� ����Ʈ ������ �������� �ʽ��ϴ�!" )
			<< BUILD_LOG( iBattleFieldID )
			<< END_LOG;

		return KBattleFieldListPtr();
	}

	return mit->second;
}

void KBattleFieldListManager::UpdateBattleFieldRoomInfo( IN const KBattleFieldRoomInfo& kRoomInfo )
{
	if( IsExist( kRoomInfo.m_iBattleFieldID ) == false )
	{
		KBattleFieldListPtr spRoomList( new KBattleFieldList );
		spRoomList->AddRoomInfo( kRoomInfo );
		m_mapBattleFieldList.insert( std::make_pair( kRoomInfo.m_iBattleFieldID, spRoomList ) );
		return;
	}

	KBattleFieldListPtr spRoomList = GetBattleFieldRoomList( kRoomInfo.m_iBattleFieldID );
	if( spRoomList == NULL )
	{
		START_LOG( cerr, L"�� ����Ʈ ���� �����Ͱ��� �̻��մϴ�!" )
			<< BUILD_LOG( kRoomInfo.m_iBattleFieldID )
			<< END_LOG;
		return;
	}

	if( spRoomList->IsExist( kRoomInfo.m_RoomUID ) == false )
	{
		spRoomList->AddRoomInfo( kRoomInfo );
	}
	else
	{
		spRoomList->UpdateRoomInfo( kRoomInfo );
	}
}

void KBattleFieldListManager::DeleteBattleFieldRoomInfo( IN const KBattleFieldRoomInfo& kRoomInfo )
{
	if( IsExist( kRoomInfo.m_iBattleFieldID ) == false )
	{
		START_LOG( cerr, L"�ش� ��Ʋ�ʵ��� �渮��Ʈ�� �������� �ʽ��ϴ�!" )
			<< BUILD_LOG( kRoomInfo.m_iBattleFieldID )
			<< END_LOG;
		return;
	}	

	KBattleFieldListPtr spRoomList = GetBattleFieldRoomList( kRoomInfo.m_iBattleFieldID );
	if( spRoomList == NULL )
	{
		START_LOG( cerr, L"�� ����Ʈ ���� �����Ͱ��� �̻��մϴ�!" )
			<< BUILD_LOG( kRoomInfo.m_iBattleFieldID )
			<< END_LOG;
		return;
	}

	LIF( spRoomList->DelRoomInfo( kRoomInfo.m_RoomUID ) );
}

//{{ 2012. 11. 20	������		��Ʋ�ʵ� �� ���� ��� �߰�
#ifdef SERV_BATTLE_FIELD_ROOM_CLEAR_AT_CN_PROXY_DIE
void KBattleFieldListManager::DeleteBattleFieldRoomInfo( IN const UidType iCnServerUID )
{
	int nDelCount = 0;

	std::map< int, KBattleFieldListPtr >::iterator mit;
	for( mit = m_mapBattleFieldList.begin(); mit != m_mapBattleFieldList.end(); ++mit )
	{
		KBattleFieldListPtr spBattleFieldList = mit->second;
		if( IS_NULL( spBattleFieldList ) )
		{
			START_LOG( cerr, L"�����Ͱ��� ��ȿ���� �ʽ��ϴ�!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		spBattleFieldList->DelRoomInfoByCnServerUID( iCnServerUID );
	}
}
#endif SERV_BATTLE_FIELD_ROOM_CLEAR_AT_CN_PROXY_DIE
//}}

bool KBattleFieldListManager::GetRoomUIDForJoinBattleField( IN const int iBattleFieldID, IN const UidType iUnitUID, IN const UidType iTargetRoomUID, IN const UidType iPartyUID, OUT UidType& iJoinBFRoomUID )
{
	SET_ERROR( NET_OK );

	iJoinBFRoomUID = 0;

	if( IsExist( iBattleFieldID ) == false )
	{
		// �̷� ��� ���ο� ��Ʋ�ʵ���� ���� �����ؾ��ϹǷ� true�� �����Ѵ�. ( ������ �ƴϴ� )
		return true;
	}

	KBattleFieldListPtr spRoomList = GetBattleFieldRoomList( iBattleFieldID );
	if( IS_NULL( spRoomList ) )
	{
		START_LOG( cerr, L"�������� �ʴ� ��Ʋ�ʵ� �� ����Ʈ ���� �����Ͱ��� �̻��մϴ�!" )
			<< BUILD_LOG( iBattleFieldID )
			<< END_LOG;

		SET_ERROR( ERR_BATTLEFIELD_04 );
		return false;
	}
	
	return spRoomList->GetRoomUIDForJoinBattleField( iUnitUID, iTargetRoomUID, iPartyUID, iJoinBFRoomUID );
}


#endif SERV_BATTLE_FIELD_SYSTEM
//}}


