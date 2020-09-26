#include ".\AutoPartyScriptManager.h"
#include "NetError.h"
#include "BaseServer.h"
#include "Lottery.h"

//{{ 2012. 11. 29	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	#include "Enum/Enum.h"
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
//}}


ImplementRefreshSingleton( KAutoPartyScriptManager );

KAutoPartyScriptManager::KAutoPartyScriptManager(void) :
m_fSameUnitTypeWaitTime( SAME_UNIT_TYPE_WAIT_TIME ),
m_fDirectGameStartWaitTime( DIRECT_GAME_START_WAIT_TIME ),
//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
m_fSameDifficultyLevelWaitTime( SAME_DIFFICULTY_LEVEL_WAIT_TIME ),
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}
m_fAutoPartyBonusTime( AUTO_PARTY_BONUS_TIME_DEFALUT ),
m_iAutoPartyMakingBeginCount( 1 ),
//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
m_bIsAllMemberCheckSameUnitTypeTime( true ),
m_bIsAllMemberCheckSameDifficultyLevelTime( true ),
m_bIsAllMemberCheckDirectGameStartTime( true ),
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}
m_iCheckWaitUserCount( 10 )
{
}

KAutoPartyScriptManager::~KAutoPartyScriptManager(void)
{
}

ImplToStringW( KAutoPartyScriptManager )
{
	stm_	<< L"----------[ Auto Party Script Manager ]----------" << std::endl
			<< TOSTRINGW( m_vecAutoPartyDungeonInfo.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( KAutoPartyScriptManager )
{
	lua_tinker::class_add<KAutoPartyScriptManager>( GetLuaState(), "KAutoPartyScriptManager" );	
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "AddAutoPartyDungeonInfo",		&KAutoPartyScriptManager::AddAutoPartyDungeonInfo_LUA );
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetSameUnitTypeWaitTime",		&KAutoPartyScriptManager::SetSameUnitTypeWaitTime_LUA );
	//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetSameDifficultyLevelWaitTime",&KAutoPartyScriptManager::SetSameDifficultyLevelWaitTime_LUA );
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetDirectGameStartWaitTime",	&KAutoPartyScriptManager::SetDirectGameStartWaitTime_LUA );	
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetAutoPartyBonusTime",			&KAutoPartyScriptManager::SetAutoPartyBonusTime_LUA );
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetAutoPartyMakingBeginCount",	&KAutoPartyScriptManager::SetAutoPartyMakingBeginCount_LUA );
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetCheckWaitUserCount",			&KAutoPartyScriptManager::SetCheckWaitUserCount_LUA );
	//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetIsAllMemberCheckSameUnitTypeTime",			&KAutoPartyScriptManager::SetIsAllMemberCheckSameUnitTypeTime_LUA );
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetIsAllMemberCheckSameDifficultyLevelTime",	&KAutoPartyScriptManager::SetIsAllMemberCheckSameDifficultyLevelTime_LUA );
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetIsAllMemberCheckDirectGameStartTime",		&KAutoPartyScriptManager::SetIsAllMemberCheckDirectGameStartTime_LUA );
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}

	//{{ 2012. 11. 29	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetAutoPartyMatchingSuccessNum",				&KAutoPartyScriptManager::SetAutoPartyMatchingSuccessNum_LUA );
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	//}}

	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "dump",							&KAutoPartyScriptManager::Dump );

	lua_tinker::decl( GetLuaState(), "AutoPartyScriptManager", this );
}

bool KAutoPartyScriptManager::AddAutoPartyDungeonInfo_LUA( int iDungeonMode, int iDungeonIDWithDif, int iBeginLevel, int iEndLevel )
{
	if( iDungeonMode < 0  ||  iDungeonIDWithDif < 0  ||  iBeginLevel < 0  ||  iEndLevel < 0 )
	{
		START_LOG( cerr, L"��ġ ������ �̻��մϴ�." )
			<< BUILD_LOG( iDungeonMode )
			<< BUILD_LOG( iDungeonIDWithDif )
			<< BUILD_LOG( iBeginLevel )
			<< BUILD_LOG( iEndLevel )
			<< END_LOG;
		return false;
	}

	KAutoPartyMakingProcessCondition kAutoPartyDungeonInfo;
	kAutoPartyDungeonInfo.m_iDungeonMode = iDungeonMode;
	kAutoPartyDungeonInfo.m_iDungeonIDWithDif = iDungeonIDWithDif;
	kAutoPartyDungeonInfo.m_iBeginLevel = iBeginLevel;
	kAutoPartyDungeonInfo.m_iEndLevel = iEndLevel;
	m_vecAutoPartyDungeonInfo.push_back( kAutoPartyDungeonInfo );
	return true;
}

void KAutoPartyScriptManager::SetSameUnitTypeWaitTime_LUA( double fWaitTime )
{
	m_fSameUnitTypeWaitTime = fWaitTime;

	START_LOG( cout, L"[�ڵ� ��Ƽ ����] ���� ���� ĳ���� ��Ī ��� �ð� : " << m_fSameUnitTypeWaitTime );
}

void KAutoPartyScriptManager::SetDirectGameStartWaitTime_LUA( double fWaitTime )
{
	m_fDirectGameStartWaitTime = fWaitTime;

	START_LOG( cout, L"[�ڵ� ��Ƽ ����] �ٷ� ���� �����ϴ� ��Ī ��� �ð� : " << m_fDirectGameStartWaitTime );
}

//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
void KAutoPartyScriptManager::SetSameDifficultyLevelWaitTime_LUA( double fWaitTime )
{
	m_fSameDifficultyLevelWaitTime = fWaitTime;

	START_LOG( cout, L"[�ڵ� ��Ƽ ����] ���� ���̵� ��Ī ��� �ð� : " << m_fSameDifficultyLevelWaitTime );
}
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}

void KAutoPartyScriptManager::SetAutoPartyBonusTime_LUA( double fBonusTime )
{
	m_fAutoPartyBonusTime = fBonusTime;

	START_LOG( cout, L"[�ڵ� ��Ƽ ����] �ڵ� ��Ƽ ���ʽ� ���� �ð�!" )
		<< BUILD_LOG( GetAutoPartyBonusTime() );
}

void KAutoPartyScriptManager::SetAutoPartyMakingBeginCount_LUA( int iAutoPartyMakingBeginCount )
{
	m_iAutoPartyMakingBeginCount = iAutoPartyMakingBeginCount;

	START_LOG( cout, L"[�ڵ� ��Ƽ ����] �ڵ� ��Ƽ ��Ī ���� ����� �ο� ��" )
		<< BUILD_LOG( GetAutoPartyMakingBeginCount() );
}

void KAutoPartyScriptManager::SetCheckWaitUserCount_LUA( int iCheckWaitUserCount )
{
	m_iCheckWaitUserCount = iCheckWaitUserCount;

	START_LOG( cout, L"[�ڵ� ��Ƽ ����] �ڵ� ��Ƽ ����� ����Ʈ ���� ī��Ʈ" )
		<< BUILD_LOG( GetCheckWaitUserCount() );
}

//{{ 2012. 11. 29	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
void KAutoPartyScriptManager::SetAutoPartyMatchingSuccessNum_LUA( IN int iDungeon, IN int iNum )
{
	if( ( iDungeon < 0 ) || ( iNum < 1 ) )
	{
		START_LOG( cerr, L"�߸��� ���ڰ��Դϴ�.")
			<< BUILD_LOG( iDungeon )
			<< BUILD_LOG( iNum )
			<< END_LOG;
	}

	std::pair< std::map<int, int>::iterator, bool > pairResult = m_mapAutoPartyMatchingSuccessNum.insert( std::map<int, int>::value_type( iDungeon, iNum ) );
	if( pairResult.second == false )
	{
		START_LOG( cerr, L"�̹� ���ǵǾ� �ִ� iDungeon �Դϴ�.")
			<< BUILD_LOG( iDungeon )
			<< END_LOG;
	}
}
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
//}}

//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
void KAutoPartyScriptManager::SetIsAllMemberCheckSameUnitTypeTime_LUA( bool bVal )
{
	m_bIsAllMemberCheckSameUnitTypeTime = bVal;

	START_LOG( cout, L"[�ڵ� ��Ƽ ����] �ڵ� ��Ƽ ��Ī�� ���� �������� üũ ����" )
		<< BUILD_LOG( IsCheckAllMemberSameUnitTypeTime() );
}

void KAutoPartyScriptManager::SetIsAllMemberCheckSameDifficultyLevelTime_LUA( bool bVal )
{
	m_bIsAllMemberCheckSameDifficultyLevelTime = bVal;

	START_LOG( cout, L"[�ڵ� ��Ƽ ����] �ڵ� ��Ƽ ��Ī�� �ٸ� ���̵� ���� üũ ����" )
		<< BUILD_LOG( IsCheckAllMemberSameDifficultyLevelTime() );
}

void KAutoPartyScriptManager::SetIsAllMemberCheckDirectGameStartTime_LUA( bool bVal )
{
	m_bIsAllMemberCheckDirectGameStartTime = bVal;

	START_LOG( cout, L"[�ڵ� ��Ƽ ����] �ڵ� ��Ƽ ��Ī�� �ο� �� �����ص� �ٷ� ���� ����" )
		<< BUILD_LOG( IsCheckAllMemberDirectGameStartTime() );
}
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}

//{{ 2012. 11. 29	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
int KAutoPartyScriptManager::GetAutoPartyMatchingSuccessNum( IN const int iDungeon ) const
{
	std::map<int, int>::const_iterator it = m_mapAutoPartyMatchingSuccessNum.find( iDungeon );
	if( it == m_mapAutoPartyMatchingSuccessNum.end() )
	{
		return SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT;
	}

	return it->second;
}
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
//}}
