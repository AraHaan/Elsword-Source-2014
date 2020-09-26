#include "BattleFieldParty.h"


//{{ 2011. 11. 2	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


KBattleFieldParty::KBattleFieldParty()
{
	Clear();
}

KBattleFieldParty::~KBattleFieldParty()
{
}

void KBattleFieldParty::Clear()
{
	m_setPartyMemberList.clear();
}

void KBattleFieldParty::UpdatePartyMemberList( IN const std::vector< UidType >& vecInfo )
{
	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecInfo )
	{
		m_setPartyMemberList.insert( iUnitUID );
	}
}

void KBattleFieldParty::GetPartyMemberList( OUT std::vector< UidType >& vecMemberList ) const
{
	vecMemberList.clear();

	BOOST_TEST_FOREACH( const UidType, iUnitUID, m_setPartyMemberList )
	{
		vecMemberList.push_back( iUnitUID );
	}
}

bool KBattleFieldParty::JoinPartyMember( IN const UidType iUnitUID )
{
	if( IsPartyMember( iUnitUID ) == true )
	{
		START_LOG( cerr, L"�̹� ��Ƽ ����ε� �� ���÷��� �ϳ�?" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	m_setPartyMemberList.insert( iUnitUID );
	return true;
}
bool KBattleFieldParty::LeavePartyMember( IN const UidType iUnitUID )
{
	if( IsPartyMember( iUnitUID ) == false )
	{
		START_LOG( cerr, L"��Ƽ ����� �ƴѵ� �����Ϸ��� �ϳ�?" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	m_setPartyMemberList.erase( iUnitUID );
	return true;
}


#endif SERV_BATTLE_FIELD_SYSTEM
//}}