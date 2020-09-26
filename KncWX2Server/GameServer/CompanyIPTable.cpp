#include "CompanyIPTable.h"

//{{ 2011.10.14     ��μ�    ��� ����� Ư�� IP ������ ��� ����
//#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP

ImplementRefreshSingleton( KCompanyIPTable );

KCompanyIPTable::KCompanyIPTable(void)
{
}

KCompanyIPTable::~KCompanyIPTable(void)
{
}

ImplToStringW( KCompanyIPTable )
{
	stm_	<< L"----------[ CompanyIPTable ]----------" << std::endl
			<< TOSTRINGW( m_vecCompanyIPData.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( KCompanyIPTable )
{
	lua_tinker::class_add<KCompanyIPTable>( GetLuaState(), "KCompanyIPTable" );

	lua_tinker::class_def<KCompanyIPTable>( GetLuaState(), "AddCompanyIP",		&KCompanyIPTable::AddCompanyIP_LUA );
	lua_tinker::class_def<KCompanyIPTable>( GetLuaState(), "dump",				&KCompanyIPTable::Dump );
	
	lua_tinker::decl( GetLuaState(), "g_pKCompanyIPTable", this );
}

bool KCompanyIPTable::AddCompanyIP_LUA( const char* szAddIP )
{
	
	if( szAddIP == NULL )
	{
		START_LOG( cerr, L"[��� ġƮ ��� ȯ��]IP ��Ʈ�� �̻�." )
			<< END_LOG;

		return false;
	}

	std::string strIP = szAddIP;
	if( strIP.size() < 1 || strIP.size() > 15 )
	{
		START_LOG( cerr, L"[��� ġƮ ��� ȯ��]�߰��Ϸ��� IP �̻�." )
			<< BUILD_LOG( strIP.size() )
			<< END_LOG;

		return false;
	}

	START_LOG( cout, L"=== [��� ġƮ ��� ȯ��] ADD IP ===" )
		<< BUILD_LOG( strIP.c_str() )
		<< BUILD_LOG( strIP.size() )
		<< L"================"
		<< dbg::endl;

	m_vecCompanyIPData.push_back( strIP );


	return true;
}

bool KCompanyIPTable::CheckIP( const char* szIP )
{
	//////////////////////////////////////////////////////////////////////////
	// ��� ġƮ ��� ������ �ִ� IP ���� Ȯ���Ѵ�.
	// ������ �ִ� IP ��� true
	//////////////////////////////////////////////////////////////////////////

	if( szIP == NULL )
	{
		START_LOG( cerr, L"IP�� ���ٰ�?? ��� �Ǵ� �ǰ�?" )
			<< END_LOG;

		return false;
	}

	std::string strIP = szIP;
	if( strIP.size() < 1 || strIP.size() > 15 )
	{
		START_LOG( cerr, L"�̻��� IP �Դϴ�." )
			<< BUILD_LOG( strIP.size() )
			<< END_LOG;

		return false;
	}

	std::vector< std::string >::iterator vit = m_vecCompanyIPData.begin();
	for( ; vit != m_vecCompanyIPData.end() ; ++vit )
	{
		if( strIP.compare( (*vit) ) == 0 )
		{
			return true;
		}
	}

	START_LOG( cerr, L"��� ġƮ ��� ������ ���� IP �Դϴ�." )
		<< BUILD_LOG( strIP )
		<< END_LOG;

	return false;
}

//#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
//}}