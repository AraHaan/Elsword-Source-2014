#include "LogManager.h"
#include <dbg/dbg.hpp>



//{{ 2013. 02. 15	������	�α� �ý��� ����
//#ifdef SERV_LOG_SYSTEM_NEW

ImplementRefreshSingleton( KLogManager )

KLogManager::KLogManager() :
m_fCheckTimeGap( 300.0 ),				// ����Ʈ ���� 5��
m_fErrorLogStatTimeGap( 1800.0 ),		// ����Ʈ ���� 30��
m_iLogFileMaxSize( 10 * 1024 * 1024 ),	// ����Ʈ ���� 10 mb
m_bNewLogByFileSize( false ),			// ����Ʈ ���� false
m_bNewLogDaily( false ),				// ����Ʈ ���� false
m_bErrorLogStat( false )				// ����Ʈ ���� false
{
	m_tLastCheckTime = CTime::GetCurrentTime();
}

KLogManager::~KLogManager()
{
}

ImplToStringW( KLogManager )
{
	return stm_;
}

ImplementLuaScriptParser( KLogManager )
{
	lua_tinker::class_add<KLogManager>( GetLuaState(), "KLogManager" );
	lua_tinker::class_def<KLogManager>( GetLuaState(), "SetNewLogByFileSize",		&KLogManager::SetNewLogByFileSize_LUA );
	lua_tinker::class_def<KLogManager>( GetLuaState(), "SetNewLogDaily",			&KLogManager::SetNewLogDaily_LUA );
	lua_tinker::class_def<KLogManager>( GetLuaState(), "SetErrorLogStat",			&KLogManager::SetErrorLogStat_LUA );
	lua_tinker::class_def<KLogManager>( GetLuaState(), "SetCheckTimeGap",			&KLogManager::SetCheckTimeGap_LUA );
	lua_tinker::class_def<KLogManager>( GetLuaState(), "SetErrorLogStatTimeGap",	&KLogManager::SetErrorLogStatTimeGap_LUA );
	lua_tinker::class_def<KLogManager>( GetLuaState(), "SetLogFileMaxSize",			&KLogManager::SetLogFileMaxSize_LUA );
	lua_tinker::class_def<KLogManager>( GetLuaState(), "dump",						&KLogManager::Dump );

	lua_tinker::decl( GetLuaState(), "LogManager", this );
}

void KLogManager::SetNewLogByFileSize_LUA( bool bValue )
{
	m_bNewLogByFileSize = bValue;

	START_LOG( cout, L"���� ũ�� �ʰ��� �α� ���� ���� ���� : " << m_bNewLogByFileSize );
}

void KLogManager::SetNewLogDaily_LUA( bool bValue )
{
	m_bNewLogDaily = bValue;

	START_LOG( cout, L"���� ���ο� �α� ���� ���� ���� : " << m_bNewLogDaily );
}

void KLogManager::SetErrorLogStat_LUA( bool bValue )
{
	m_bErrorLogStat = bValue;

	START_LOG( cout, L"���� �α� ��� ����� ���� : " << m_bErrorLogStat );
}

void KLogManager::SetCheckTimeGap_LUA( double fTimeGap )
{
	m_fCheckTimeGap = fTimeGap;

	START_LOG( cout, L"�α� ���� üũ Ÿ�� �ֱ� : " << m_fCheckTimeGap );
}

void KLogManager::SetErrorLogStatTimeGap_LUA( double fTimeGap )
{
	m_fErrorLogStatTimeGap = fTimeGap;

	START_LOG( cout, L"���� �α� ��� DB������Ʈ Ÿ�� �ֱ� : " << m_fErrorLogStatTimeGap );
}

void KLogManager::SetLogFileMaxSize_LUA( int iMByte, int iKByte )
{
	m_iLogFileMaxSize = 0;
	m_iLogFileMaxSize += 1024 * iKByte;
	m_iLogFileMaxSize += 1024 * 1024 * iMByte;

	START_LOG( cout, L"�α� ���� �ִ� ũ��" )
		<< BUILD_LOG( m_iLogFileMaxSize )
		<< BUILD_LOG( m_iLogFileMaxSize / 1024 )
		<< BUILD_LOG( m_iLogFileMaxSize / 1024 / 1024 );
}

void KLogManager::Tick()
{
	// Ư�� �ֱ� ���� üũ
	if( m_tCheckTimer.elapsed() < m_fCheckTimeGap )
		return;

	m_tCheckTimer.restart();

	// ���� ������ ���ϱ�!
	if( m_bNewLogByFileSize )
	{
		const int iFileSize = dbg::logfile::GetLogFileSize();

		// ���� ����� Ư�� ũ�⸦ ������ ���ο� �α� ������ �����!
		if( iFileSize > m_iLogFileMaxSize )
		{
			// �α� ���� �� ���� ��� �ϱ�!
			START_LOG( cout, L"--------------------------- �α� ���� �뷮�� 10mb�� �ʰ��Ͽ� ���ο� ���Ϸ� �α� ������ �����մϴ�! ---------------------------" )
				<< BUILD_LOG( iFileSize )
				<< BUILD_LOG( m_iLogFileMaxSize )
				<< BUILD_LOG( iFileSize / 1024 )
				<< BUILD_LOG( m_iLogFileMaxSize / 1024 )
				<< BUILD_LOG( iFileSize / 1024 / 1024 )
				<< BUILD_LOG( m_iLogFileMaxSize / 1024 / 1024 );

			// ���� �����ϱ�!
			dbg::logfile::CloseFile();

			// �α� ���� �� ���� ��� �ϱ�!
			START_LOG( cout, L"--------------------------- �α� ���� �뷮�� 10mb�� �ʰ��Ͽ� ���ο� ���Ϸ� �α� ������ �����մϴ�! ---------------------------" )
				<< BUILD_LOG( iFileSize )
				<< BUILD_LOG( m_iLogFileMaxSize )
				<< BUILD_LOG( iFileSize / 1024 )
				<< BUILD_LOG( m_iLogFileMaxSize / 1024 )
				<< BUILD_LOG( iFileSize / 1024 / 1024 )
				<< BUILD_LOG( m_iLogFileMaxSize / 1024 / 1024 );
			return;
		}
	}
	
	// 0�� �������� ���� �α� ������ �����մϴ�.
	if( m_bNewLogDaily )
	{
		CTime tBeforeCheckTime = m_tLastCheckTime;
		m_tLastCheckTime = CTime::GetCurrentTime();

		// ��¥�� �޶����ٸ� �������� �ȰŴ�!
		if( tBeforeCheckTime.GetDay() != m_tLastCheckTime.GetDay() )
		{
			// �α� ���� �� ���� ��� �ϱ�!
			START_LOG( cout, L"--------------------------- ���� 0�� �������� ���ο� �α� ������ �����մϴ�! ---------------------------" )
				<< BUILD_LOG( tBeforeCheckTime.GetDay() )
				<< BUILD_LOG( m_tLastCheckTime.GetDay() );

			// ���� �����ϱ�!
			dbg::logfile::CloseFile();

			// �α� ���� �� ���� ��� �ϱ�!
			START_LOG( cout, L"--------------------------- ���� 0�� �������� ���ο� �α� ������ �����մϴ�! ---------------------------" )
				<< BUILD_LOG( tBeforeCheckTime.GetDay() )
				<< BUILD_LOG( m_tLastCheckTime.GetDay() );
			return;
		}
	}

	// ���� �α� ��� �����
	if( m_bErrorLogStat )
	{
		if( m_tErrorLogStatTimer.elapsed() > m_fErrorLogStatTimeGap )
		{
			m_tErrorLogStatTimer.restart();

			std::map< unsigned int, dbg::logfile::KLogStat > mapLogStat;
			dbg::cerr.GetLogStat( mapLogStat );
			dbg::cerr.ClearLogStat();

			KDBE_LOG_STATISTICS_INFO_NOT kPacketToLog;

			std::map< unsigned int, dbg::logfile::KLogStat >::const_iterator mit;
			for( mit = mapLogStat.begin(); mit != mapLogStat.end(); ++mit )
			{
				//START_LOG( cout, L"--------------------------- �α� ��� ---------------------------" )
				//	<< BUILD_LOG( mit->second.m_wstrFunctionName )
				//	<< BUILD_LOG( mit->second.m_wstrFileName )
				//	<< BUILD_LOG( mit->second.m_wstrLineNum )
				//	<< BUILD_LOG( mit->second.m_iCount );

				KLogStatInfo kInfo;
				kInfo.m_wstrFunctionName = mit->second.m_wstrFunctionName;
				kInfo.m_wstrFileName	 = mit->second.m_wstrFileName;
				kInfo.m_wstrLineNum		 = mit->second.m_wstrLineNum;
				kInfo.m_iCount			 = mit->second.m_iCount;
				kPacketToLog.m_vecLogStat.push_back( kInfo );
			}

			if( kPacketToLog.m_vecLogStat.empty() == false )
			{
				SendToLogDB( DBE_LOG_STATISTICS_INFO_NOT, kPacketToLog );

				START_LOG( cout, L"--------------------------- ���� �α� ��踦 ����ϴ�! ---------------------------" )
					<< BUILD_LOG( kPacketToLog.m_vecLogStat.size() );
			}
		}
	}
}

//#endif SERV_LOG_SYSTEM_NEW
//}}
