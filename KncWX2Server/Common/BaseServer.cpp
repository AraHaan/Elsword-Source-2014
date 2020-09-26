#include <WinSock2.h>
#include "BaseServer.h"
#include "dbg/dbg.hpp"
#include "NetError.h"
#include "ThreadManager.h"
#include "socket/NetCommon.h"
#include "NetLayer.h"
#include "SimLayer.h"
#include "DBLayer.h"
#include "KncSend.h"
#include "ActorManager.h"
//{{ 2010. 02. 12  ������	���� �α� �Ŵ���
#ifdef SERV_LOG_MANAGER
	#include "ServerLogManager.h"
#endif SERV_LOG_MANAGER
//}}

//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	#include "LogManager.h"
#endif SERV_LOG_SYSTEM_NEW
//}}

#include <ImportKncLua.h>
#include <ImportKncSerializer.h>
#include <ImportKncSecurity.h>
#include <ImportKncDebug.h>

#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "odbc32" )

#ifdef NDEBUG
#   include "CrashRpt/KncCrashRpt.h"
#   pragma comment(lib, "../Common/Crashrpt/CrashRpt.lib")

// ũ���� ����Ʈ ó�� �Լ�
void CrashReportProcess( LPVOID lpvState )
{
	std::wostringstream stm;

	stm << L"Local IP : " << NetCommon::GetLocalIPW() << std::endl
		<< L"GetTickCount : " << ::GetTickCount() << std::endl;

	AppendToMailContent( lpvState, stm.str().c_str() );

	// get post-crash execute file's full path.
	wchar_t szBuff[MAX_PATH] = {0};
	::GetCurrentDirectoryW( MAX_PATH, szBuff );
#ifdef _CONVERT_VS_2010
	wcscat_s( szBuff, MAX_PATH, L"\\DumpUploader.exe" );
#else
	::wcscat( szBuff, L"\\DumpUploader.exe" );
#endif _CONVERT_VS_2010

	stm.str( L"" );  // �ʱ�ȭ - ������ ������ ���ڷ� ���� ���ڿ��� ��ġ.

	std::wstring wstrLogFileName = KncUtil::toWideString( dbg::logfile::GetLogFileName() );

	// build command line parameter
	stm << L"/s " << NetCommon::GetLocalIPW()			// ���� �̸� (������)
		<< L" /f " << wstrLogFileName					// �α����� �̸�
		<< L" /f " << GetFullDumpFileName( lpvState );	// �������� �̸�

	ShellExecuteW( NULL, L"open", szBuff, stm.str().c_str(), NULL, SW_SHOWNORMAL );
}

unsigned int CALLBACK CrashCallback_StackOverFlow( LPVOID lpvState )
{
	// stack over flow�� ��� ������ �����忡�� ũ���� ����Ʈ ó���Ѵ�!
    CrashReportProcess( lpvState );
	return 0;
}

BOOL CALLBACK CrashCallback( LPVOID lpvState )
{
	//{{ 2010. 04. 13  ������	ũ���� ����Ʈ ����
	if( GetExcetionType( lpvState ) != EXCEPTION_STACK_OVERFLOW )
	{
		// ���� �����÷ο찡 �ƴ϶�� ������� ó���Ѵ�!
		CrashReportProcess( lpvState );
	}
	else
	{
		// stack over flow�� ��� ������ ������ �����Ͽ� ó��!
		HANDLE hThread = (HANDLE)_beginthreadex( 0, (1024 * 1024 * 2), CrashCallback_StackOverFlow, lpvState, 0, NULL );

		// ���� �����尡 ���� ���� ������ ��ٸ���
		WaitForSingleObject( hThread, INFINITE );

		CloseHandle( hThread );
	}
	//}}

    return true;
}
#endif


NiImplementRTTI( KBaseServer, KPerformer );
ImplInstanceKeeping( KBaseServer );

lua_State* g_pLua = NULL;

#define SERVER_INFO_UPDATE_GAP 60 * 1000
DWORD KBaseServer::ms_dwServerInfoUpdateGap( SERVER_INFO_UPDATE_GAP );

KBaseServer::KBaseServer(void) 
:
    m_iServerGroupID( 0 ),
    m_strTagVersion( L"Tag version undefined." ),
    m_dwShutDownWait( 1000 * 60 * 2 ),
    m_hWnd( HWND_DESKTOP ),
	//{{ 2012. 12. 24	������	���� ���� ���� ����ȭ
#ifdef SERV_GAME_SERVER_VERSION_DETAIL
	m_eUseVersion( VE_INTERNAL )
#else
	m_bUseSecondVer( false )
#endif SERV_GAME_SERVER_VERSION_DETAIL
	//}}
{
    srand( (unsigned) time( NULL ) );
    m_dwServerInfoUpdateTick = ::GetTickCount();
    m_spEventAck.reset( CreateEvent( NULL, false, false, NULL ), CloseHandle );
	m_bServerReady = false;
#ifdef SERV_MORNITORING
	m_bConnMornitoringEnable = false;
#endif SERV_MORNITORING
	//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
	m_bScriptParsingErrReport = false;
#endif SERV_SCRIPT_PARSING_ERR_REPORT
	//}}
	//{{ 2011. 02. 23	������	������ ���� Ÿ��
#ifdef SERV_SERVER_ROLL_TYPE
	m_cServerRollType = KServerInfo::SRT_NONE;
#endif SERV_SERVER_ROLL_TYPE
	//}}
}

KBaseServer::~KBaseServer(void)
{
	//{{ 2010. 8. 30	������	���� ���� ���� ó��	
	if( g_pLua != NULL )
	{
		lua_close( g_pLua );
		g_pLua = NULL;
	}
	//}}

    //dbg::profiler::kill_profiler( "profile_info.htm" );
}

ImplToStringW( KBaseServer )
{
    size_t uiServerList = 0;
    {
        KLocker lock( m_csServerList );
        std::map< int, std::vector< KServerInfo > >::const_iterator mit;
        for( mit = m_mapServerList.begin(); mit != m_mapServerList.end(); ++mit )
        {
            uiServerList += mit->second.size();
        }
    }
    
    START_TOSTRING_PARENTW( KPerformer )
        << TOSTRINGW( m_strMailingList )
        << TOSTRINGW( m_strTagVersion )
        << TOSTRINGW( uiServerList )        
		//{{ 2012. 12. 24	������	���� ���� ���� ����ȭ
#ifdef SERV_GAME_SERVER_VERSION_DETAIL
		<< TOSTRINGW( m_eUseVersion )
		<< TOSTRINGW( m_strMainVer[VE_INTERNAL] )
		<< TOSTRINGW( m_strMainVer[VE_OPEN_TEST] )
		<< TOSTRINGW( m_strMainVer[VE_SERVICE] )
#ifdef SERV_KR_OPEN_TEST2_VERSION// �۾���¥: 2013-05-24	// �ڼ���
		<< TOSTRINGW( m_strMainVer[VE_OPEN_TEST2] )
#endif // SERV_KR_OPEN_TEST2_VERSION
#else
		<< TOSTRINGWb( m_bUseSecondVer )
		<< TOSTRINGW( m_strMainVer[0] )
		<< TOSTRINGW( m_strMainVer[1] )
#endif SERV_GAME_SERVER_VERSION_DETAIL
		//}}        
        << TOSTRINGW( m_dwShutDownWait );
 
//     m_stringTable.Dump( stm_ );

     return stm_;
}

#include <lua_tinker.h>

void KBaseServer::RegToLua()
{
	// global function set
	lua_tinker::def( g_pLua, "log",			&dbg::logfile::SetLogLevel );
	lua_tinker::def( g_pLua, "changelog",		&dbg::logfile::CloseFile );
	lua_tinker::def( g_pLua, "GetLocalIP",	&NetCommon::GetLocalIP );

 	lua_tinker::class_add<KThreadManager>( g_pLua, "KThreadManager" );
	lua_tinker::class_def<KThreadManager>( g_pLua, "Init",						&KThreadManager::Init );
	lua_tinker::class_def<KThreadManager>( g_pLua, "ResetMaxProcessingInfo",	&KThreadManager::ResetMaxProcessingInfo );
	lua_tinker::class_def<KThreadManager>( g_pLua, "SetThreadNum",				&KThreadManager::SetThreadNum );

 	lua_tinker::class_add<KBaseServer>( g_pLua, "KBaseServer" );
	//{{ 2012. 12. 24	������	���� ���� ���� ����ȭ
#ifdef SERV_GAME_SERVER_VERSION_DETAIL
	lua_tinker::class_def<KBaseServer>( g_pLua, "SetUseVersion",					&KBaseServer::SetUseVersion );
#else
	lua_tinker::class_def<KBaseServer>( g_pLua, "SetMainVersion",					&KBaseServer::SetMainVersion );
	lua_tinker::class_def<KBaseServer>( g_pLua, "SetSecondVersion",					&KBaseServer::SetSecondVersion );
#endif SERV_GAME_SERVER_VERSION_DETAIL
	//}}
	lua_tinker::class_def<KBaseServer>( g_pLua, "SetShutDownWait",					&KBaseServer::SetShutDownWait );
	lua_tinker::class_def<KBaseServer>( g_pLua, "SetMailingList",					&KBaseServer::SetMailingList );
	//{{ 2009. 12. 17  ������	������Ȯ��
	lua_tinker::class_def<KBaseServer>( g_pLua, "GetServerGroupID",					&KBaseServer::GetServerGroupID );
	//}}
	//{{ 2010. 11. 12	������	������ ���� ����͸�
#ifdef SERV_MORNITORING
	lua_tinker::class_def<KBaseServer>( g_pLua, "SetConnectionMornitoringEnable",	&KBaseServer::SetConnectionMornitoringEnable_LUA );
#endif SERV_MORNITORING
	//}}
	//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
	lua_tinker::class_def<KBaseServer>( g_pLua, "SetScriptParsingErrReport",		&KBaseServer::SetScriptParsingErrReport_LUA );
	lua_tinker::class_def<KBaseServer>( g_pLua, "AddScriptParsingErrReportMail",	&KBaseServer::AddScriptParsingErrReportMail_LUA );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
	//}}

	//{{ 2011.2.11  ��ȿ��  ����͸��� ���� ��Ʈ üũ �� �����α� ���°� ����
#ifdef SERV_PERMIT_PORT_CHECK
	lua_tinker::class_def<KBaseServer>( g_pLua, "AddPortCheckMoritoringServer", &KBaseServer::AddPortCheckMoritoringServer_LUA );
#endif SERV_PERMIT_PORT_CHECK
	//}}
	//{{ 2011. 04. 13  ���� �� �ŷ� ���� �ǽð� SMS ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE_SMS
	lua_tinker::class_def<KBaseServer>( g_pLua, "SetMonitoringLetterAndTradeSMS",		&KBaseServer::SetMonitoringLetterAndTradeSMS_LUA );
#endif SERV_MONITORING_LETTER_AND_TRADE_SMS
	//}} 2011. 04. 13 ���� �� �ŷ� ���� �ǽð� SMS ����

	//{{ 2012. 1. 9	������		DB���� ��ȣ ����ȭ
#ifdef SERV_DB_CONNECTION_SECURITY
	lua_tinker::decl( g_pLua, "ESC_CENTER",			KBaseServer::ESC_CENTER );
	lua_tinker::decl( g_pLua, "ESC_LOGIN",			KBaseServer::ESC_LOGIN );
	lua_tinker::decl( g_pLua, "ESC_GAME",			KBaseServer::ESC_GAME );
	lua_tinker::decl( g_pLua, "ESC_CHANNEL",		KBaseServer::ESC_CHANNEL );
	lua_tinker::decl( g_pLua, "ESC_SIMULATION",		KBaseServer::ESC_SIMULATION );
	lua_tinker::decl( g_pLua, "ESC_GLOBAL",			KBaseServer::ESC_GLOBAL );
#endif SERV_DB_CONNECTION_SECURITY
	//}}

	//{{ 2012. 12. 24	������	���� ���� ���� ����ȭ
#ifdef SERV_GAME_SERVER_VERSION_DETAIL
	lua_tinker::decl( g_pLua, "VE_INTERNAL",		KBaseServer::VE_INTERNAL );
	lua_tinker::decl( g_pLua, "VE_OPEN_TEST",		KBaseServer::VE_OPEN_TEST );
	lua_tinker::decl( g_pLua, "VE_SERVICE",			KBaseServer::VE_SERVICE );
#ifdef SERV_KR_OPEN_TEST2_VERSION// �۾���¥: 2013-05-24	// �ڼ���
	lua_tinker::decl( g_pLua, "VE_OPEN_TEST2",		KBaseServer::VE_OPEN_TEST2 );
#endif // SERV_KR_OPEN_TEST2_VERSION
#endif SERV_GAME_SERVER_VERSION_DETAIL
	//}}
}

extern const wchar_t* g_szConfigFile;
#if defined( SERV_CHANGE_SERVER_CONFIG_FORDER ) && !defined( SERV_INT_INTERNAL )
	#if defined( SERV_COUNTRY_TWHK )
		const wchar_t* g_szConfigForderName = L"Config\\TW\\";
	#elif defined( SERV_COUNTRY_JP )
		const wchar_t* g_szConfigForderName = L"Config\\JP\\";
	#elif defined( SERV_COUNTRY_EU )
		const wchar_t* g_szConfigForderName = L"Config\\EU\\";
	#elif defined( SERV_COUNTRY_US )
		const wchar_t* g_szConfigForderName = L"Config\\US\\";
	#elif defined( SERV_COUNTRY_CN )
		const wchar_t* g_szConfigForderName = L"Config\\CN\\";
	#elif defined( SERV_COUNTRY_TH )
		const wchar_t* g_szConfigForderName = L"Config\\TH\\";
	#elif defined( SERV_COUNTRY_ID )
		const wchar_t* g_szConfigForderName = L"Config\\ID\\";
	#elif defined( SERV_COUNTRY_BR )
		const wchar_t* g_szConfigForderName = L"Config\\BR\\";
	#elif defined( SERV_COUNTRY_PH )
		const wchar_t* g_szConfigForderName = L"Config\\PH\\";
	#elif defined( SERV_COUNTRY_IN )
		const wchar_t* g_szConfigForderName = L"Config\\IN\\";
	#else
		const wchar_t* g_szConfigForderName = L"Config\\INT\\";
	#endif
#else	
		const wchar_t* g_szConfigForderName = L"Config\\INT\\";
#endif //SERV_CHANGE_SERVER_CONFIG_FORDER

#ifndef _DEBUG	// ����� �ȵǴ� ���� �ؿ��� ����
#include "Lottery.h"
#endif // _DEBUG

bool KBaseServer::Init()
{
#ifndef _DEBUG	// ����� �ȵǴ� ���� �ؿ��� ����
	KLottery::Seed( static_cast<int>(std::time(0)) );
	KLottery kLotTest;
	kLotTest.Decision();
#endif // _DEBUG

    dbg::logfile::SetFileNameAtTime( true );            // init log file
    
    _JIF( NetCommon::InitWinSock(), return false );     // socket init

    g_pLua = lua_open();
        
    _JIF( g_pLua != NULL, return false );
    
    //luaopen_base( g_pLua );  // ����Լ� tonumber�� ����ؾ� �Ѵ�.
    //luaopen_table( g_pLua );
    //luaopen_io( g_pLua );
    //luaopen_string( g_pLua );
    //luaopen_math( g_pLua );

    luaL_openlibs( g_pLua );

    lua_settop( g_pLua, 0 );

    RegToLua();
	//{{ 2010. 06. 21  ������	��� ������
#ifdef SERV_STATISTICS_THREAD
	m_kTStatisticsManager.RegToLua();
#else
	m_kStatisticsManager.RegToLua();
#endif SERV_STATISTICS_THREAD
	//}}

    SiKDBLayer()->RegToLua();
    KSimLayer::GetKObj()->RegToLua();
    KNetLayer::GetKObj()->RegToLua();

    LoadINIFile( g_szConfigFile );

    // 050317. config ������ lua, ini �ΰ����� �þ. Ȯ���ڴ� ������ ���δ�.
	std::string strLuaFileName = KncUtil::toNarrowString( g_szConfigFile );
		
	if( m_vecCmdArgs.size() >= 3 ) // �������� ������ 2�̻��̸� �����±װ� �������ڷ� ���°��� Ȯ����!
	{
		std::string strVersionTag = KncUtil::toNarrowString( m_vecCmdArgs[1].GetString() );
		std::string strServerGroupID = KncUtil::toNarrowString( m_vecCmdArgs[2].GetString() );
		
		// ���� config���� �̸� ����
		strLuaFileName += "_";
		strLuaFileName += strVersionTag;

		// ������ID ����
        SetServerGroupID( static_cast<int>( atoi( strServerGroupID.c_str() ) ) );

		// cmdArgs���� DB Conn Str�� �����ϰ� �����Ѵ�
		while( m_vecCmdArgs.size() > 2 )
		{
			m_vecCmdArgs.erase( m_vecCmdArgs.begin() );
		}
	}
	else
	{
		START_LOG( cerr, L"���� �Ӷ� �ʿ��� �������ڰ� �����մϴ�!" )
			<< BUILD_LOG( m_vecCmdArgs.size() )
			<< END_LOG;
        return false;
	}

	strLuaFileName += ".lua";
#ifdef SERV_CHANGE_SERVER_CONFIG_FORDER
	// �系������ ������ ������
	if( (int)(strLuaFileName.find( "INTERNAL" )) != -1 ) // ã�Ҵ�
	{
		strLuaFileName = KncUtil::toNarrowString(g_szConfigForderName) + strLuaFileName;
	}
#endif //SERV_CHANGE_SERVER_CONFIG_FORDER

	START_LOG( cout, L"Config File Name : " << strLuaFileName );

    _JIF( 0 == LUA_DOFILE( g_pLua, strLuaFileName.c_str() ), return false );
    
    if( NetError::GetLastError() != NetError::NET_OK )
        return false;

	//{{ 2010. 02. 12  ������	���� �α� �Ŵ���
#ifdef SERV_LOG_MANAGER
	KServerLogManager::GetInstance().Begin();
#endif SERV_LOG_MANAGER
	//}}

    SiKDBLayer()->Init();
    KSimLayer::GetKObj()->Init();
	//{{ 2013. 02. 13	������	Tick�����ս� DB�α� ���
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	KSimLayer::GetKObj()->TickDumpReset();
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	//}}
    KSimLayer::GetKObj()->Begin();

    _JIF( Authenticate( 10000 ), return false );

    // ���� ��ü ������ ��ģ �ڿ� Actor���� ������ �޴´�.
    KNetLayer::GetKObj()->Init();

    //{{ 050810. jseop. tag ���� ����.
    SetTagVersion();    // ���� ���� ���� �ð� ���.
    //}} 050810. jseop. tag ���� ����.
	
	//{{ 2010. 06. 21  ������	��� ������
#ifdef SERV_STATISTICS_THREAD
	m_kTStatisticsManager.Init();
#else
	m_kStatisticsManager.InitLocalLogKeys();
#endif SERV_STATISTICS_THREAD	
	//}}
	//{{ 2010. 12. 7	������	���� ���� �ý���
#ifdef SERV_MAIL_SYSTEM
	m_kMailManager.Init();
#endif SERV_MAIL_SYSTEM
	//}}
	//{{ 2007. 12. 24  ������  �系������� �α׸� �������� �ʴ´�.
	//if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_NEXON_KOREA )
	//	m_kStatisticsManager.SetSendToFTP( true );
	//}}

	// cmd args�� ���� �ʿ�����Ƿ� ����
	m_vecCmdArgs.clear();

    return true;
}

void KBaseServer::Run( bool bRunInterpreter_ )
{
    if( bRunInterpreter_ )  lua_interpreter( g_pLua );
}

void KBaseServer::ShutDown()
{
	//{{ 2009. 12. 15  ������	��������
	START_LOG( cout, L"---------------------------------------- ���� ���� ���� ----------------------------------------" );
	//{{ 2010. 8. 29	������	�̺�Ʈ ť ������ ����
#ifdef SERV_MAX_QUEUE_SIZE_DUMP
	DumpToLogFile();
#endif SERV_MAX_QUEUE_SIZE_DUMP
	//}}
	KSimLayer::GetKObj()->DumpToLogFile();
	KNetLayer::GetKObj()->DumpToLogFile();
	SiKDBLayer()->DumpToLogFile();
	START_LOG( cout, L"---------------------------------------- ���� ���� ���� ----------------------------------------" );
	//}}

    KNetLayer::GetKObj()->CloseAccepter();  // ������� ���ο� ������ ���´�.

    LIF( DestroyAllSession() );             // �������� ������ ����� child���� �Ѵ�.
	
	//{{ 2010. 06. 21  ������	��� ������
#ifdef SERV_STATISTICS_THREAD
	m_kTStatisticsManager.Shutdown();
#else
	m_kStatisticsManager.FlushData();
	m_kStatisticsManager.OnFlushLocalLog();
#endif SERV_STATISTICS_THREAD
	//}}
	//{{ 2010. 12. 7	������	���� ���� �ý���
#ifdef SERV_MAIL_SYSTEM
	m_kMailManager.Shutdown();
#endif SERV_MAIL_SYSTEM
	//}}

	SendToLogDB( DBE_SERVER_OFF_NOT );

    SiKDBLayer()->ShutDown( 60000 );        // 60�� ���� DB �̺�Ʈ ó���� ��ٸ���.
    KSimLayer::GetKObj()->ShutDown();
    KNetLayer::GetKObj()->ShutDown();

	::WSACleanup();

    SiKDBLayer()->ReleaseInstance();
    KSimLayer::ReleaseKObj();
    KNetLayer::ReleaseKObj();
}

//void KBaseServer::WriteServerInfoToDB()
//{
//    KServerInfo kPacket;
//    GetServerInfo( kPacket );
//    SendToLogDB( DBE_UPDATE_SERVER_INFO_REQ, kPacket );
//
//    m_dwServerInfoUpdateTick = ::GetTickCount();
//}

void KBaseServer::Tick()
{
	//{{ 2011. 12. 14	������	SimLayer Tick�����ս� üũ
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	BEGIN_CHECK_TICK_LATENCY;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

    KPerformer::Tick();

	//{{ 2010. 06. 21  ������	��� ������
#ifdef SERV_STATISTICS_THREAD
#else
	m_kStatisticsManager.Tick();
#endif SERV_STATISTICS_THREAD
	//}}

	//{{ 2013. 02. 19	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
	SiKLogManager()->Tick();
#endif SERV_LOG_SYSTEM_NEW
	//}}

	//{{ 2011. 12. 14	������	SimLayer Tick�����ս� üũ
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	END_CHECK_TICK_LATENCY;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}
}

void KBaseServer::OnServerReadyComplete()
{
	// ��ũ��Ʈ �Ľ� ���� ����Ʈ
	if( m_bScriptParsingErrReport == false )
		return;

    if( IsScriptParsingSuccess() == true )
		return;

	std::wstring wstrLogFileName = KncUtil::toWideString( dbg::logfile::GetLogFileName() );

	// ���Ϸ� ���� �α׸� ��������!
	KEMAIL_SEND_EMAIL_NOT kPacket;
	kPacket.m_vecRecvMailAddrList = m_vecScriptParsingErrMailList;

	// ����
	kPacket.m_wstrTitle = L"��ũ��Ʈ �Ľ� ���� ����Ʈ! : ";
	kPacket.m_wstrTitle += GetName();

	// ����
	kPacket.m_wstrDesc = GetName();
	kPacket.m_wstrDesc += L" ������ �Ѵ� ���߿� ��ũ��Ʈ �Ľ��� �����Ͽ����ϴ�! �����α� Ȯ�� �ٶ��ϴ�!\n\n";

	BOOST_TEST_FOREACH( const std::wstring&, wstrScriptFileName, m_vecFailScriptFileList )
	{
		kPacket.m_wstrDesc += wstrScriptFileName;
		kPacket.m_wstrDesc += L"\n";
	}

	// ���� �α� ���� ÷��
	kPacket.m_vecAttachFileFullPathList.push_back( wstrLogFileName );
	QueueingEventToMailManager( EMAIL_SEND_EMAIL_NOT, kPacket );

	START_LOG( cout, L"--- ��ũ��Ʈ �Ľ� ���� ����Ʈ ---" )
		<< BUILD_LOG( m_bScriptParsingErrReport )
		<< BUILD_LOG( IsScriptParsingSuccess() )
		<< BUILD_LOG( wstrLogFileName );
}

void KBaseServer::SetTagVersion()
{
    KncUtil::GetTimeStampByNameW( m_strTagVersion );

	//{{ 2010. 04. 13  ������	ũ���� ����Ʈ	
    // 050716. crash report ���� �κ��� main.cpp���� simlayer�� �ű� - �±� ������ ��ũ��Ʈ�� �����ϱ� ����.
#ifdef NDEBUG
	// ����� ���尡 �ƴҶ��� install
    LPVOID lpvState = Install( CrashCallback, m_strMailingList.c_str(), m_strTagVersion.c_str() );
#endif
	//}}

    // ������ ó�� �ʱ�ȭ�� ����ϹǷ�, ���� �ƹ� �α׵� ������� ���� ���¿��� �Ѵ�. 
    dbg::logfile::SetLogFileTitle( m_strTagVersion.c_str() ); 
}

//{{ 2012. 12. 24	������	���� ���� ���� ����ȭ
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GAME_SERVER_VERSION_DETAIL
//////////////////////////////////////////////////////////////////////////

namespace ConfigINI 
{
    /*//// ...in ini file... ////////////////////////////////////////////////////

    [Version]
    Protocol Version 1 = XXX
    Protocol Version 2 = YYY
    
    [String]
    aaa = AAA
    bbb = BBB
    ...

    /////////////////////////////////////////////////////////////////////////*/

    const wchar_t* szSectionVersion = L"Version";
    const wchar_t* szSectionString  = L"String";
	const wchar_t* szKeyVersion1    = L"Internal Version";
	const wchar_t* szKeyVersion2    = L"OpenTest Version";
	const wchar_t* szKeyVersion3    = L"Service Version";
#ifdef SERV_KR_OPEN_TEST2_VERSION// �۾���¥: 2013-05-24	// �ڼ���
	const wchar_t* szKeyVersion4    = L"OpenTest Version2";
#endif // SERV_KR_OPEN_TEST2_VERSION
}

void KBaseServer::LoadINIFile( const wchar_t* szFileName_ )
{
	// 050318. florist. ini ������ ��ü ���(Ȥ�� �����)�� ��� ����ؾ� �Ѵ�. 
	// �׷��� ������ ������ ������ ���� �ȿ����� ã�´�. �̿� �ؾ� �ҰŸ� Ȯ���ϰ� �����θ� ����.
	wchar_t buff[MAX_PATH] = {0};
	::GetCurrentDirectoryW( MAX_PATH, buff );
	m_strIniFilePath = buff;
	m_strIniFilePath += L"\\";
#ifdef SERV_CHANGE_SERVER_CONFIG_FORDER
	// �系������ ���� ���� �Ѵ�
	if( m_vecCmdArgs.size() >= 3 )
	{
		std::string strVersionTag = KncUtil::toNarrowString( m_vecCmdArgs[1].GetString() );
		if( (int)(strVersionTag.find( "INTERNAL" )) != -1 ) // ã�Ҵ�
		{
			m_strIniFilePath += g_szConfigForderName;
		}
	}
#endif //SERV_CHANGE_SERVER_CONFIG_FORDER
	m_strIniFilePath += szFileName_;
	m_strIniFilePath += L".ini";
	
	// 050317. version ���� ������.
	::GetPrivateProfileStringW( ConfigINI::szSectionVersion, ConfigINI::szKeyVersion1, 0, buff, MAX_PATH, m_strIniFilePath.c_str() );
	m_strMainVer[VE_INTERNAL] = buff;
	::GetPrivateProfileStringW( ConfigINI::szSectionVersion, ConfigINI::szKeyVersion2, 0, buff, MAX_PATH, m_strIniFilePath.c_str() );
	m_strMainVer[VE_OPEN_TEST] = buff;
	::GetPrivateProfileStringW( ConfigINI::szSectionVersion, ConfigINI::szKeyVersion3, 0, buff, MAX_PATH, m_strIniFilePath.c_str() );
	m_strMainVer[VE_SERVICE] = buff;
#ifdef SERV_KR_OPEN_TEST2_VERSION// �۾���¥: 2013-05-24	// �ڼ���
	::GetPrivateProfileStringW( ConfigINI::szSectionVersion, ConfigINI::szKeyVersion4, 0, buff, MAX_PATH, m_strIniFilePath.c_str() );
	m_strMainVer[VE_OPEN_TEST2] = buff;
#endif // SERV_KR_OPEN_TEST2_VERSION

	LIF( !m_strMainVer[VE_INTERNAL].empty() );
	LIF( !m_strMainVer[VE_OPEN_TEST].empty() );
	LIF( !m_strMainVer[VE_SERVICE].empty() );
#ifdef SERV_KR_OPEN_TEST2_VERSION// �۾���¥: 2013-05-24	// �ڼ���
	LIF( !m_strMainVer[VE_OPEN_TEST2].empty() );
#endif // SERV_KR_OPEN_TEST2_VERSION
	
	if( !m_stringTable.LoadIni( m_strIniFilePath.c_str() ) )
	{
		START_LOG( cerr, L"INI ������ �о� ���� ���߽��ϴ�." )
			<< BUILD_LOG( m_strIniFilePath )
			<< END_LOG;
	}

	m_stringTable.SetCurrentSection( ConfigINI::szSectionString );
}

const std::wstring& KBaseServer::GetMainVersion() const
{
	if( m_eUseVersion < 0  ||  m_eUseVersion >= VE_MAX )
	{
		START_LOG( cerr, L"�߸��� ���� �ε��� �Դϴ�!" )
			<< BUILD_LOG( m_eUseVersion )
			<< END_LOG;

		return m_strMainVer[VE_INTERNAL];
	}

	return m_strMainVer[m_eUseVersion];
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

namespace ConfigINI 
{
    /*//// ...in ini file... ////////////////////////////////////////////////////

    [Version]
    Protocol Version 1 = XXX
    Protocol Version 2 = YYY
    
    [String]
    aaa = AAA
    bbb = BBB
    ...

    /////////////////////////////////////////////////////////////////////////*/

    const wchar_t* szSectionVersion = L"Version";
    const wchar_t* szSectionString  = L"String";
	const wchar_t* szKeyVersion1    = L"Main Version";
	const wchar_t* szKeyVersion2    = L"Service Version";
}

void KBaseServer::LoadINIFile( const wchar_t* szFileName_ )
{
	// 050318. florist. ini ������ ��ü ���(Ȥ�� �����)�� ��� ����ؾ� �Ѵ�. 
	// �׷��� ������ ������ ������ ���� �ȿ����� ã�´�. �̿� �ؾ� �ҰŸ� Ȯ���ϰ� �����θ� ����.
	wchar_t buff[MAX_PATH] = {0};
	::GetCurrentDirectoryW( MAX_PATH, buff );
	m_strIniFilePath = buff;
	m_strIniFilePath += L"\\";
	m_strIniFilePath += szFileName_;
	m_strIniFilePath += L".ini";
	
	// 050317. version ���� ������.
	//m_nProtocolVer  = ::GetPrivateProfileIntW( ConfigINI::szSectionVersion, ConfigINI::szKeyVersion1, 0, m_strIniFilePath.c_str() );
	::GetPrivateProfileStringW( ConfigINI::szSectionVersion, ConfigINI::szKeyVersion1, 0, buff, MAX_PATH, m_strIniFilePath.c_str() );
	m_strMainVer[0] = buff;
	::GetPrivateProfileStringW( ConfigINI::szSectionVersion, ConfigINI::szKeyVersion2, 0, buff, MAX_PATH, m_strIniFilePath.c_str() );
	m_strMainVer[1] = buff;

	LIF( !m_strMainVer[0].empty() );
	LIF( !m_strMainVer[1].empty() );

	//{{ 060201. kkurrung. ���ڿ� ���̺�
	if( !m_stringTable.LoadIni( m_strIniFilePath.c_str() ) )
	{
		START_LOG( cerr, L"INI ������ �о� ���� ���߽��ϴ�." )
			<< BUILD_LOG( m_strIniFilePath )
			<< END_LOG;
	}

	m_stringTable.SetCurrentSection( ConfigINI::szSectionString );

	//}} kkurrung.
}

void KBaseServer::SetMainVersion( int nIndex_, const char* szVersion_ )
{
	std::wstring* pstrVersion = &m_strMainVer[ nIndex_ ];
	const wchar_t* szVersionName = nIndex_ == 0 ? ConfigINI::szKeyVersion1 : ConfigINI::szKeyVersion2;

	START_LOG( cout, L"Change Main Version." )
		<< BUILD_LOG( nIndex_ )
		<< BUILD_LOG( szVersionName )
		<< BUILD_LOG( *pstrVersion )
		<< BUILD_LOG( szVersion_ );

	*pstrVersion = KncUtil::toWideString( szVersion_ );

	LIF( ::WritePrivateProfileStringW( ConfigINI::szSectionVersion, szVersionName,
		pstrVersion->c_str(), m_strIniFilePath.c_str() ) );
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_GAME_SERVER_VERSION_DETAIL
//////////////////////////////////////////////////////////////////////////
//}}


void KBaseServer::SetServerList( const std::map< int, std::vector< KServerInfo > >& mapServerList )
{
    KLocker lock( m_csServerList );
    m_mapServerList = mapServerList;
}

void KBaseServer::GetServerList( std::map< int, std::vector< KServerInfo > >& mapServerList )
{
    KLocker lock( m_csServerList );
    mapServerList = m_mapServerList;
}

//void KBaseServer::GetServerInfo( KServerInfo& kServerInfo )
//{
//    kServerInfo.m_iUID = ( int )GetUID();
//    kServerInfo.m_wstrName = GetName();
//    kServerInfo.m_iServerGroupID = GetServerGroupID();
//    kServerInfo.m_iServerClass = GetServerClass();
//	//{{ 2013. 01. 17	������	������ ��� �缳IP ����
//#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
//	kServerInfo.m_wstrIP = NetCommon::GetLocalIPW();
//#else
//	kServerInfo.m_wstrIP = NetCommon::GetLocalIPW();
//#endif SERV_PRIVATE_IP_SERVER_NETWORKING
//	//}}
//    kServerInfo.m_usMasterPort = KNetLayer::GetKObj()->m_usPortMaster;
//    kServerInfo.m_usNCUDPPort = KNetLayer::GetKObj()->GetNCUDPPort();
//	//{{ 2011. 12. 16	������	���� �ִ� �ο� �ǽð� ����
//#ifdef SERV_REALTIME_MAX_USER
//	kServerInfo.m_nMaxUser = GetDefaultMaxUser();
//#else
//	kServerInfo.m_nMaxUser = GetMaxUser();
//#endif SERV_REALTIME_MAX_USER
//	//}}    
//    kServerInfo.m_nCurrentUser = KActorManager::GetKObj()->GetCount();
//    kServerInfo.m_bOn = true;
//}

void KBaseServer::SetServerInfo( const KServerInfo& kServerInfo )
{
	if( GetServerGroupID() != kServerInfo.m_iServerGroupID )
	{
		START_LOG( cerr, L"������ڷ� ���� ServerGroupID�� DB�κ��� ������������ ServerGroupID�� �ٸ��ϴ�!" )
			<< BUILD_LOG( GetServerGroupID() )
			<< BUILD_LOG( kServerInfo.m_iServerGroupID )
			<< END_LOG;
	}

    SetUID( kServerInfo.m_iUID );
    SetName( kServerInfo.m_wstrName );
    SetServerGroupID( kServerInfo.m_iServerGroupID );
    SetServerClass( kServerInfo.m_iServerClass );
	//{{ 2013. 01. 17	������	������ ��� �缳IP ����
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
	SetPublicIP( kServerInfo.m_wstrPublicIP );
	SetPrivateIP( kServerInfo.m_wstrPrivateIP );
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}
	KNetLayer::GetKObj()->SetPort( kServerInfo.m_usMasterPort );
    KNetLayer::GetKObj()->InitNCUDP( kServerInfo.m_usNCUDPPort );
	//{{ 2011. 12. 16	������	���� �ִ� �ο� �ǽð� ����
#ifdef SERV_REALTIME_MAX_USER
	SetDefaultMaxUser( kServerInfo.m_nMaxUser );
#else
	SetMaxUser( kServerInfo.m_nMaxUser );
#endif SERV_REALTIME_MAX_USER
	//}}    
	//{{ 2011. 02. 23	������	������ ���� Ÿ��
#ifdef SERV_SERVER_ROLL_TYPE
	m_cServerRollType = kServerInfo.m_cServerRollType;
	if( m_cServerRollType != KServerInfo::SRT_NONE )
	{
		START_LOG( cout, L"--- ������ ���� �ο� ---" )
			<< BUILD_LOGc( GetServerRollType() );
	}
#endif SERV_SERVER_ROLL_TYPE
	//}}
}

bool KBaseServer::Authenticate( DWORD dwTimeOut_ )
{
    KNetAddress kPacket;
    kPacket.m_wstrIP    = NetCommon::GetLocalIPW();
    kPacket.m_usPort    = KNetLayer::GetKObj()->m_usPortMaster;

    SendToLogDB( DBE_SERVER_ON_REQ, kPacket );

    switch( ::WaitForSingleObject( m_spEventAck.get(), dwTimeOut_ ) )
    {
    case WAIT_OBJECT_0: // ack�� ����� ���� ���.
        return true;
    case WAIT_TIMEOUT: // �ð� �ʰ�
        START_LOG( cerr, L"verify account timeout." );
        return false;
    default:
        START_LOG( cerr, L"invalide event object." );
        return false;
    }

    return false; // never get here.
}

//{{ 2008. 10. 10  ������	
UidType KBaseServer::GetFirstActorKey()
{
	return KActorManager::GetKObj()->GetFirstActorKey();
}
//}}

//{{ 2010. 8. 29	������	�̺�Ʈ ť ������ ����
#ifdef SERV_MAX_QUEUE_SIZE_DUMP
void KBaseServer::DumpToLogFile()
{
	std::wstringstream wstrDump;
	ToString( wstrDump );
	dbg::cout << wstrDump.str() << dbg::endl;
}
#endif SERV_MAX_QUEUE_SIZE_DUMP
//}}

//{{ 2010. 11. 12	������	������ ���� ����͸�
#ifdef SERV_MORNITORING
void KBaseServer::SetConnectionMornitoringEnable_LUA( bool bVal )
{
	m_bConnMornitoringEnable = bVal;

	START_LOG( cout, L"������ ���� ����͸� ���� ���� : " << m_bConnMornitoringEnable );
}
#endif SERV_MORNITORING
//}}

//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
void KBaseServer::SetScriptParsingErrReport_LUA( bool bVal )
{
	m_bScriptParsingErrReport = bVal;

	START_LOG( cout, L"��ũ��Ʈ �Ľ� ���� ����Ʈ ��� ���� ���� : " << m_bScriptParsingErrReport );
}

void KBaseServer::AddScriptParsingErrReportMail_LUA( const char* pszMail )
{
	std::wstring wstrMail = KncUtil::toWideString( pszMail );
    m_vecScriptParsingErrMailList.push_back( wstrMail );

	START_LOG( cout, L"-- ��ũ��Ʈ �Ľ� ���� ����Ʈ ��� ���� �ּ� ��� --" )
		<< BUILD_LOG( wstrMail );
}

#endif SERV_SCRIPT_PARSING_ERR_REPORT
//}}

void KBaseServer::SendToAccountDB( unsigned short usEventID )
{
    SendToAccountDB( usEventID, char() );
}

void KBaseServer::SendToLogDB( unsigned short usEventID )
{
    SendToLogDB( usEventID, char() );
}

#ifdef SERV_ADD_EVENT_DB
void KBaseServer::SendToEventDB( unsigned short usEventID )
{
	SendToEventDB( usEventID, char() );
}
#endif //SERV_ADD_EVENT_DB

//{{ 2011.2.11  ��ȿ��  ����͸��� ���� ��Ʈ üũ �� �����α� ���°� ����
#ifdef SERV_PERMIT_PORT_CHECK
void KBaseServer::AddPortCheckMoritoringServer_LUA( const char* pStrIP )
{
	m_PortCheckMoritoringServerIPList.insert( pStrIP );
}

bool KBaseServer::IsMonitoringServerForPortCheck( std::string strIP )
{
	if( m_PortCheckMoritoringServerIPList.find(strIP) == m_PortCheckMoritoringServerIPList.end() )
		return false;
	else
		return true;
}
#endif SERV_PERMIT_PORT_CHECK
//}}
