#include "BaseServer.h"
#include ".\xslpartymanager.h"


ImplementRefreshSingleton( CXSLPartyManager );

CXSLPartyManager::CXSLPartyManager(void) :
//{{ 2012. 11. 12	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
m_fBasePercent( 0.f ),
m_fPartyFeverFactor( 0.f ),
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2012. 11. 16	������		��ϸ� �ð� ��� �������� ����
#ifdef SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
m_iEnterSuccessCount( 0 ),
m_iEnterFailCount( 0 ),
#endif SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
//}}
m_iActivateFeverPoint( 0 )
{
	//{{ 2012. 11. 16	������		��ϸ� �ð� ��� �������� ����
#ifdef SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
	m_kHenirSecretEnterMinRate.m_fEnterSecretRate = 100.f;
	m_kHenirSecretEnterMaxRate.m_fEnterSecretRate = 0.f;
#endif SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
	//}}
}

CXSLPartyManager::~CXSLPartyManager(void)
{
}

ImplToStringW( CXSLPartyManager )
{
	stm_	<< L"----------[ Party Manager ]----------" << std::endl
		<< TOSTRINGW( m_mapMaxFeverPoint.size() )
		<< TOSTRINGW( m_mapIncreasePartyFever.size() )
		//{{ 2012. 11. 16	������		��ϸ� �ð� ��� �������� ����
#ifdef SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
		<< TOSTRINGW( m_iEnterSuccessCount )
		<< TOSTRINGW( m_iEnterFailCount )
#endif SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
		//}}
		;

	//{{ 2012. 11. 16	������		��ϸ� �ð� ��� �������� ����
#ifdef SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
	stm_	<< L"----------[ Henir Scret Enter Min Rate ]----------" << std::endl
		<< TOSTRINGW( m_kHenirSecretEnterMinRate.m_iFeverPoint )
		<< TOSTRINGW( m_kHenirSecretEnterMinRate.m_iNumMember )
		<< TOSTRINGW( m_kHenirSecretEnterMinRate.m_fEnterSecretRate )
		<< TOSTRINGW( m_kHenirSecretEnterMinRate.m_bEnterSuccess )
		;

	stm_	<< L"----------[ Henir Scret Enter Max Rate ]----------" << std::endl
		<< TOSTRINGW( m_kHenirSecretEnterMaxRate.m_iFeverPoint )
		<< TOSTRINGW( m_kHenirSecretEnterMaxRate.m_iNumMember )
		<< TOSTRINGW( m_kHenirSecretEnterMaxRate.m_fEnterSecretRate )
		<< TOSTRINGW( m_kHenirSecretEnterMaxRate.m_bEnterSuccess )
		;
#endif SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
	//}}

	return stm_;
}

ImplementLuaScriptParser( CXSLPartyManager )
{
	lua_tinker::class_add<CXSLPartyManager>( GetLuaState(), "CXSLRandomItemManager" );
	lua_tinker::class_def<CXSLPartyManager>( GetLuaState(), "SetActivateFeverPoint",			&CXSLPartyManager::SetActivateFeverPoint_LUA );
	lua_tinker::class_def<CXSLPartyManager>( GetLuaState(), "SetMaxFeverPointByNumMember",		&CXSLPartyManager::SetMaxFeverPointByNumMember_LUA );
	lua_tinker::class_def<CXSLPartyManager>( GetLuaState(), "SetIncreasePartyFeverByNumMember",	&CXSLPartyManager::SetIncreasePartyFeverByNumMember_LUA );
	//{{ 2012. 11. 12	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	lua_tinker::class_def<CXSLPartyManager>( GetLuaState(), "SetDungeonSecretStageEnterFactor",	&CXSLPartyManager::SetDungeonSecretStageEnterFactor_LUA );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	lua_tinker::class_def<CXSLPartyManager>( GetLuaState(), "dump",								&CXSLPartyManager::Dump );

	lua_tinker::decl( GetLuaState(), "g_pPartyManager", this );
}

void CXSLPartyManager::SetActivateFeverPoint_LUA( int iFever )
{
	if( iFever < 0 )
	{
		START_LOG( cerr, L"��ũ��Ʈ ���� �̻��մϴ�." )
			<< BUILD_LOG( iFever )
			<< END_LOG;
		return;
	}

	m_iActivateFeverPoint = iFever;

	START_LOG( cout, L"[��Ƽ] �ǹ� Ȱ��ȭ ����Ʈ" )
		<< BUILD_LOG( GetActivateFeverPoint() );
}

bool CXSLPartyManager::SetMaxFeverPointByNumMember_LUA( int iNumMember, int iMaxFever )
{
	if( iNumMember < 0  ||  iMaxFever < 0 )
	{
		START_LOG( cerr, L"��ũ��Ʈ ���� �̻��մϴ�." )
			<< BUILD_LOG( iNumMember )
			<< BUILD_LOG( iMaxFever )
			<< END_LOG;
		return false;
	}

	START_LOG( cout, L"[��Ƽ] ��Ƽ ��� ���� �ִ� fever��ġ" )
		<< BUILD_LOG( iNumMember )
		<< BUILD_LOG( iMaxFever );

	m_mapMaxFeverPoint.insert( std::make_pair( iNumMember, iMaxFever ) );
	return true;
}

bool CXSLPartyManager::SetIncreasePartyFeverByNumMember_LUA( int iNumMember, int iMin, int iMax )
{
	if( iNumMember < 0  ||  iMin < 0 || iMax < 0 )
	{
		START_LOG( cerr, L"��ũ��Ʈ ���� �̻��մϴ�." )
			<< BUILD_LOG( iNumMember )
			<< BUILD_LOG( iMin )
			<< BUILD_LOG( iMax )
			<< END_LOG;
		return false;
	}

	START_LOG( cout, L"[��Ƽ] ��Ƽ ��� ���� fever���� ��ġ" )
		<< BUILD_LOG( iNumMember )
		<< BUILD_LOG( iMin )
		<< BUILD_LOG( iMax );
    
	// �ִ밪���� �ּҰ��� ������ �����Ѵ�.
	SIncreasePartyFeverRange sData;
	sData.m_iMin = iMin;
	sData.m_iMax = iMax;
	sData.m_iMinMaxGap = iMax - iMin;
	m_mapIncreasePartyFever.insert( std::make_pair( iNumMember, sData ) );
	return true;
}

//{{ 2012. 11. 12	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
void CXSLPartyManager::SetDungeonSecretStageEnterFactor_LUA( float fBasePercent, float fPartyFeverFactor )
{
	if( fBasePercent < 0.f  ||  fPartyFeverFactor < 0.f )
	{
		START_LOG( cerr, L"��ũ��Ʈ ���� �̻��մϴ�." )
			<< BUILD_LOG( fBasePercent )
			<< BUILD_LOG( fPartyFeverFactor )
			<< END_LOG;
		return;
	}

	m_fBasePercent = fBasePercent;
	m_fPartyFeverFactor = fPartyFeverFactor;

	START_LOG( cout, L"[��Ƽ] ��ϸ� �ð� ��� ���� �����" )
		<< BUILD_LOG( fBasePercent )
		<< BUILD_LOG( fPartyFeverFactor );
}

float CXSLPartyManager::GetDungeonSecretStageEnterRate( IN const int iPartyFeverPoint, IN const int iPartyNumMember )
{	
	float fEnterSecretRate = 0.0f;

	if( iPartyNumMember > 1 )
	{
		fEnterSecretRate = ( m_fBasePercent + ( static_cast<float>(iPartyFeverPoint) * m_fPartyFeverFactor ) ) * static_cast<float>( iPartyNumMember -1 );
	}
	else
	{
		fEnterSecretRate = 0.0f;
	}

	// 100%�� ���� �ʵ���!
	if( fEnterSecretRate > 100.f )
		fEnterSecretRate = 100.f;

	return fEnterSecretRate;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

short CXSLPartyManager::GetMaxFeverPointByNumMember( IN const short sPartyFever, IN const int iPartyNumMember )
{
	short sPartyFeverResult = sPartyFever;

	// ����� ���� �ִ밪 ����
	std::map< int, int >::const_iterator mit;
	mit = m_mapMaxFeverPoint.find( iPartyNumMember );
	if( mit == m_mapMaxFeverPoint.end() )
	{
		START_LOG( clog, L"���� �Ǿ����� ���� ���� 0���� ó��!" )
			<< BUILD_LOG( iPartyNumMember )
			<< BUILD_LOG( sPartyFever );
		return 0;
	}

	if( sPartyFeverResult > mit->second )
		sPartyFeverResult = mit->second;

	return sPartyFeverResult;
}

//{{ 2012. 11. 15	������		�ǹ� ���� ���� ����
#ifdef SERV_FEVER_INCREASE_BUG_FIX
int	CXSLPartyManager::GetIncreasePartyFever( IN const int iNumMember )
{
	std::map< int, SIncreasePartyFeverRange >::const_iterator mit;
	mit = m_mapIncreasePartyFever.find( iNumMember );
	if( mit == m_mapIncreasePartyFever.end() )
	{
		START_LOG( clog, L"�ǹ� ��� ���� �����Ƿ� 0�� ��ȯ!" )
			<< BUILD_LOG( iNumMember );
		return 0;
	}

	int iResult = 0;
	const int iMinMaxGap = mit->second.m_iMinMaxGap;
	if( iMinMaxGap > 0 )
	{
		iResult = ( rand() % ( iMinMaxGap + 1 ) ) + mit->second.m_iMin;
	}
	else
	{
		iResult = mit->second.m_iMin;
	}

	return iResult;
}
#else
int	CXSLPartyManager::GetIncreasePartyFever( IN const int iNumMember )
{
	std::map< int, SIncreasePartyFeverRange >::const_iterator mit;
	mit = m_mapIncreasePartyFever.find( iNumMember );
	if( mit == m_mapIncreasePartyFever.end() )
	{
		START_LOG( clog, L"�ǹ� ��� ���� �����Ƿ� 0�� ��ȯ!" )
			<< BUILD_LOG( iNumMember );
		return 0;
	}

	int iResult = 0;
	const int iMinMaxGap = mit->second.m_iMinMaxGap;
	if( iMinMaxGap == 0 )
	{
		iResult = ( rand() % ( iMinMaxGap + 1 ) ) + iMinMaxGap;
	}
	else
	{
		iResult = mit->second.m_iMax;
	}

	return iResult;
}
#endif SERV_FEVER_INCREASE_BUG_FIX
//}}

//{{ 2012. 11. 16	������		��ϸ� �ð� ��� �������� ����
#ifdef SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
void CXSLPartyManager::SetHenirSecretStageEnterRateDump( IN const int iFeverPoint, IN const int iNumMember, IN const float fRate, IN const bool bEnterSuccess )
{
	// �ּҰ� ����
	if( fRate < m_kHenirSecretEnterMinRate.m_fEnterSecretRate )
	{
		m_kHenirSecretEnterMinRate.m_iFeverPoint	  = iFeverPoint;
		m_kHenirSecretEnterMinRate.m_iNumMember		  = iNumMember;
		m_kHenirSecretEnterMinRate.m_fEnterSecretRate = fRate;
		m_kHenirSecretEnterMinRate.m_bEnterSuccess	  = bEnterSuccess;
	}

	// �ִ밪 ����
	if( fRate > m_kHenirSecretEnterMaxRate.m_fEnterSecretRate )
	{
		m_kHenirSecretEnterMaxRate.m_iFeverPoint	  = iFeverPoint;
		m_kHenirSecretEnterMaxRate.m_iNumMember		  = iNumMember;
		m_kHenirSecretEnterMaxRate.m_fEnterSecretRate = fRate;
		m_kHenirSecretEnterMaxRate.m_bEnterSuccess	  = bEnterSuccess;
	}

	// ���� ���� ī��Ʈ
	if( bEnterSuccess )
	{
		++m_iEnterSuccessCount;
	}
	else
	{
		++m_iEnterFailCount;
	}
}
#endif SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
//}}


