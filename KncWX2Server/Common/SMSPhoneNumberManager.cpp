#include ".\SMSPhoneNumberManager.h"

#include <dbg/dbg.hpp>
#include "BaseServer.h"
#include "NetError.h"


//{{ 2012. 09. 26	������		���� ���� ���� ����
#ifdef SERV_POST_COPY_BUG_FIX


ImplementRefreshSingleton( KSMSPhoneNumberManager );



KSMSPhoneNumberManager::KSMSPhoneNumberManager(void)
{
}

KSMSPhoneNumberManager::~KSMSPhoneNumberManager(void)
{
}

ImplToStringW( KSMSPhoneNumberManager )
{
	stm_	<< L"----------[ SMS Phone Number Manager ]----------" << std::endl;

	//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	for( int idx = 0; idx < FS_MAX; ++idx )
	{
		stm_<< TOSTRINGW( idx ) << std::endl;
		stm_<< TOSTRINGW( m_vecFuntionalPhoneNumList[idx].size() ) << std::endl;
		stm_<< TOSTRINGW( m_vecFuntionalTeamList[idx].size() ) << std::endl;
	}

	for( int idx = 0; idx < PNLT_MAX; ++idx )
	{
		stm_<< TOSTRINGW( idx ) << std::endl;
		stm_<< TOSTRINGW( m_vecTeamPhoneNumList[idx].size() ) << std::endl;
	}
#else
	for( int idx = 0; idx < PNLT_MAX; ++idx )
	{
		stm_<< TOSTRINGW( idx ) << std::endl;
		stm_<< TOSTRINGW( m_vecPhoneNumList[idx].size() ) << std::endl;
	}
#endif SERV_SMS_TOTAL_MANAGER
	//}}

	return stm_;
}

ImplementLuaScriptParser( KSMSPhoneNumberManager )
{
	lua_tinker::class_add<KSMSPhoneNumberManager>( GetLuaState(), "KSMSPhoneNumberManager" );
	//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	lua_tinker::class_def<KSMSPhoneNumberManager>( GetLuaState(), "AddFuntionalPhoneNumber",	&KSMSPhoneNumberManager::AddFuntionalPhoneNumber_LUA );
	lua_tinker::class_def<KSMSPhoneNumberManager>( GetLuaState(), "AddTeamPhoneNumber",			&KSMSPhoneNumberManager::AddTeamPhoneNumber_LUA );
	lua_tinker::class_def<KSMSPhoneNumberManager>( GetLuaState(), "Link",						&KSMSPhoneNumberManager::Link_LUA );
#else
	lua_tinker::class_def<KSMSPhoneNumberManager>( GetLuaState(), "AddPhoneNumber",			&KSMSPhoneNumberManager::AddPhoneNumber_LUA );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	lua_tinker::class_def<KSMSPhoneNumberManager>( GetLuaState(), "dump",					&KSMSPhoneNumberManager::Dump );

	lua_tinker::decl( GetLuaState(), "g_SMSPhoneNumberManager", this );

	lua_tinker::decl( GetLuaState(), "PNLT_ELSWORD_DEV_TEAM",			KSMSPhoneNumberManager::PNLT_ELSWORD_DEV_TEAM );
	//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	lua_tinker::decl( GetLuaState(), "PNLT_ELSWORD_QA_TEAM",			KSMSPhoneNumberManager::PNLT_ELSWORD_QA_TEAM );
	lua_tinker::decl( GetLuaState(), "PNLT_ELSWORD_MOR_TEAM",			KSMSPhoneNumberManager::PNLT_ELSWORD_MOR_TEAM );
	lua_tinker::decl( GetLuaState(), "PNLT_NEXON",						KSMSPhoneNumberManager::PNLT_NEXON );

#ifdef SERV_MORNITORING_ABUSER_NOTIFY_SMS// �۾���¥: 2013-05-22	// �ڼ���
	lua_tinker::decl( GetLuaState(), "PNLT_ELSWORD_DEV_TEAM_SERVER",	KSMSPhoneNumberManager::PNLT_ELSWORD_DEV_TEAM_SERVER );
#endif // SERV_MORNITORING_ABUSER_NOTIFY_SMS

	// ��ɺ� ���� ����
	lua_tinker::decl( GetLuaState(), "FS_TEST",									KSMSPhoneNumberManager::FS_TEST );
	lua_tinker::decl( GetLuaState(), "FS_MINUS_ED",								KSMSPhoneNumberManager::FS_MINUS_ED );
	lua_tinker::decl( GetLuaState(), "FS_SEND_A_LETTER_TO_SAME_NICKNAME",		KSMSPhoneNumberManager::FS_SEND_A_LETTER_TO_SAME_NICKNAME );
	lua_tinker::decl( GetLuaState(), "FS_INCORRECT_USER_UNIT",					KSMSPhoneNumberManager::FS_INCORRECT_USER_UNIT );
	lua_tinker::decl( GetLuaState(), "FS_CHECK_AUTH_LEVEL",						KSMSPhoneNumberManager::FS_CHECK_AUTH_LEVEL );
	lua_tinker::decl( GetLuaState(), "FS_SURVEILLANCE_THE_LETTER_AND_TRADE",	KSMSPhoneNumberManager::FS_SURVEILLANCE_THE_LETTER_AND_TRADE );
	lua_tinker::decl( GetLuaState(), "FS_PERSONAL_SHOP_ITEM",					KSMSPhoneNumberManager::FS_PERSONAL_SHOP_ITEM );
	lua_tinker::decl( GetLuaState(), "FS_AUTO_HACK_CHECK_GET_ITEM",				KSMSPhoneNumberManager::FS_AUTO_HACK_CHECK_GET_ITEM );
	lua_tinker::decl( GetLuaState(), "FS_DUNGEON_CLEAR_TIME_HACK_USER_CHECK",	KSMSPhoneNumberManager::FS_DUNGEON_CLEAR_TIME_HACK_USER_CHECK );
	lua_tinker::decl( GetLuaState(), "FS_DB_HACKING_ED_UPDATE_TRAP",			KSMSPhoneNumberManager::FS_DB_HACKING_ED_UPDATE_TRAP );
	lua_tinker::decl( GetLuaState(), "FS_CHECK_DROP_CCU",						KSMSPhoneNumberManager::FS_CHECK_DROP_CCU );
	lua_tinker::decl( GetLuaState(), "FS_SERVER_STATE",							KSMSPhoneNumberManager::FS_SERVER_STATE );
	lua_tinker::decl( GetLuaState(), "FS_EVENT_JACK_POT",						KSMSPhoneNumberManager::FS_EVENT_JACK_POT );
	lua_tinker::decl( GetLuaState(), "FS_LOCAL_RANKING_SYSTEM",					KSMSPhoneNumberManager::FS_LOCAL_RANKING_SYSTEM );

#ifdef SERV_MORNITORING_ABUSER_NOTIFY_SMS// �۾���¥: 2013-05-22	// �ڼ���
	lua_tinker::decl( GetLuaState(), "FS_MORNITORING_ABUSER_NOTIFY",			KSMSPhoneNumberManager::FS_MORNITORING_ABUSER_NOTIFY );
#endif // SERV_MORNITORING_ABUSER_NOTIFY_SMS

#endif SERV_SMS_TOTAL_MANAGER
	//}}
}

//{{ 2012. 10. 8	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
void KSMSPhoneNumberManager::AddFuntionalPhoneNumber_LUA( IN int iType, const char* pPhoneNum )
{
	if( iType < 0  ||  iType >= FS_MAX )
	{
		START_LOG( cerr, L"���ǵ��� ���� Ÿ�԰��� �Ѿ�Խ��ϴ�!" )
			<< BUILD_LOG( iType )
			<< BUILD_LOG( pPhoneNum )
			<< END_LOG;
		return;
	}

	std::wstring wstrPhoneNumber = KncUtil::toWideString( pPhoneNum );

	// ����ȣ �ֱ�!
	m_vecFuntionalPhoneNumList[iType].push_back( wstrPhoneNumber );
}

void KSMSPhoneNumberManager::AddTeamPhoneNumber_LUA( IN int iType, const char* pPhoneNum )
{
	if( iType < 0  ||  iType >= PNLT_MAX )
	{
		START_LOG( cerr, L"���ǵ��� ���� Ÿ�԰��� �Ѿ�Խ��ϴ�!" )
			<< BUILD_LOG( iType )
			<< BUILD_LOG( pPhoneNum )
			<< END_LOG;
		return;
	}

	std::wstring wstrPhoneNumber = KncUtil::toWideString( pPhoneNum );

	// ����ȣ �ֱ�!
	m_vecTeamPhoneNumList[iType].push_back( wstrPhoneNumber );
}

void KSMSPhoneNumberManager::Link_LUA( IN int iType, IN PHONE_NUMBER_LIST_TYPE ePNLTType )
{
	if( iType < 0  ||  iType >= FS_MAX )
	{
		START_LOG( cerr, L"���ǵ��� ���� Ÿ�԰��� �Ѿ�Խ��ϴ�!" )
			<< BUILD_LOG( iType )
			<< BUILD_LOG( ePNLTType )
			<< END_LOG;
		return;
	}

	// ����ȣ �ֱ�!
	m_vecFuntionalTeamList[iType].push_back( ePNLTType );
}

void KSMSPhoneNumberManager::GetPhoneNumberList( IN const FUNTIONAL_SET eType, OUT std::vector< std::wstring >& vecPhoneNumList ) const
{
	vecPhoneNumList.clear();

	if( eType < 0  ||  eType >= FS_MAX )
	{
		START_LOG( cerr, L"�߸��� SMS ��� Ÿ���Դϴ�!" )
			<< BUILD_LOG( eType )
			<< END_LOG;
		return;
	}

	vecPhoneNumList = m_vecFuntionalPhoneNumList[eType];

	// ������
	BOOST_TEST_FOREACH( PHONE_NUMBER_LIST_TYPE, eTeam, m_vecFuntionalTeamList[eType] )
	{
		// �� ���� ����� ��ȭ��ȣ�� ���鼭
		BOOST_TEST_FOREACH( const std::wstring&, wstrPhoneNum, m_vecTeamPhoneNumList[eTeam] )
		{
			// ����Ǿ� �ִ����� üũ�� �� ���ٸ� �߰��Ѵ�.
#ifdef _CONVERT_VS_2010
			unsigned int i = 0;
			for( ; i < vecPhoneNumList.size(); ++i )
#else
			for( unsigned int i=0; i < vecPhoneNumList.size(); ++i )
#endif _CONVERT_VS_2010
			{
				if( wcscmp( wstrPhoneNum.c_str(), vecPhoneNumList[i].c_str() ) == 0 )
				{
					break;
				}
			}

			if( i == vecPhoneNumList.size() )
			{
				vecPhoneNumList.push_back( wstrPhoneNum );
			}
		}
	}
}
#else
void KSMSPhoneNumberManager::AddPhoneNumber_LUA( int iType, const char* pPhoneNum )
{
	if( iType < 0  ||  iType >= PNLT_MAX )
	{
		START_LOG( cerr, L"���ǵ��� ���� Ÿ�԰��� �Ѿ�Խ��ϴ�!" )
			<< BUILD_LOG( iType )
			<< BUILD_LOG( pPhoneNum )
			<< END_LOG;
		return;
	}

	std::wstring wstrPhoneNumber = KncUtil::toWideString( pPhoneNum );

	// ����ȣ �ֱ�!
	m_vecPhoneNumList[iType].push_back( wstrPhoneNumber );
}

void KSMSPhoneNumberManager::GetPhoneNumberList( IN const PHONE_NUMBER_LIST_TYPE eType, OUT std::vector< std::wstring >& vecPhoneNumList ) const
{
	vecPhoneNumList.clear();

	if( eType < 0  ||  eType >= PNLT_MAX )
	{
		START_LOG( cerr, L"�߸��� �� �ѹ� Ÿ���Դϴ�!" )
			<< BUILD_LOG( eType )
			<< END_LOG;
		return;
	}

	vecPhoneNumList = m_vecPhoneNumList[eType];
}
#endif SERV_SMS_TOTAL_MANAGER
//}}

#endif SERV_POST_COPY_BUG_FIX
//}}