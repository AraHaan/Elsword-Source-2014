#include "ThreadStatisticsManager.h"

#include "KFolderTraverse.h"

#include <dbg/dbg.hpp>
#include <algorithm>
#include "Socket/NetCommon.h"
#include "BaseServer.h"


#ifdef SERV_STATISTICS_THREAD


ImplToStringW( KThreadStatisticsManager )
{
    stm_ << L"-> " << L"KThreadStatisticsManager" << std::endl
         << TOSTRINGW( ( int )m_mapStatistics.size() );

    std::map< int, KStatisticsPtr >::const_iterator mit;
    for( mit = m_mapStatistics.begin(); mit != m_mapStatistics.end(); ++mit )
    {
        stm_ << L"    " << L"( " << KStatistics::GetIDStr( mit->first )
             << L", " << mit->second->GetFlushGap()
             << L", " << ( mit->second->IsWriteToDB() ? L"true" : L"false" ) << L" )" << std::endl;
    }

    return stm_;
}

KThreadStatisticsManager::KThreadStatisticsManager()
{
	m_bIsInit					= 0;
	m_iLocalLogLastCheckTime	= 0;
	m_iLocalLogLastUpdateTime	= 0;
	m_bSendToFTPStatistics		= false;
	m_bUserLog					= false;
	//{{ 2010. 10. 11	������	�ؽ� ���� ��Ŷ �α�
#ifdef SERV_BILLING_PACKET_LOG
	m_bBillingPacketLog			= false;
#endif SERV_BILLING_PACKET_LOG
	//}}

	CreateDirectory( L"Statistics", NULL );

	m_wstrFTPServerIP			= L"14.45.79.17";
	m_iFTPPort					= 21;
	m_wstrFTPID					= L"elswordftp";
	m_wstrFTPPW					= L"spdlqj.com6424";

	//{{ 2011. 11. 21  ��μ�	��� �α� ��ġ
#ifdef SERV_STATISTICS_LOG_COUNT
	m_iStatisticsFileNo = 0;
	m_tStatisticsLogCount.restart();
#endif SERV_STATISTICS_LOG_COUNT
	//}}
}

KThreadStatisticsManager::~KThreadStatisticsManager()
{
}

void KThreadStatisticsManager::Run()
{
	START_LOG( clog, L"Enter Run()" );

	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 1 );

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Tick();

		else    START_LOG( cout, L"*** WaitForSingleObject() - return : " << ret );
	}
}

void KThreadStatisticsManager::Init()
{
	// locallogkey�ʱ�ȭ
	InitLocalLogKeys();

	START_LOG( cout, L"Thread Statistics Manager �ʱ�ȭ �Ϸ�! thread����!" );

	// thread����
	Begin();

	// �ʱ�ȭ ���� ����
	m_bIsInit = true;
}

void KThreadStatisticsManager::Shutdown()
{
	// �ʱ�ȭ ���� �ʾҴٸ� �ƹ��� ó���� ���� �ʴ´�!
	if( m_bIsInit == false )
		return;

	// thread����
	End();

	// ��� ���� ���
	FlushData();
	OnFlushLocalLog();
}

#include <KncLua.h>
#include <lua_tinker.h>

void KThreadStatisticsManager::RegToLua()
{
	lua_tinker::class_add<KThreadStatisticsManager>( g_pLua, "KStatisticsManager" );
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "dump",					&KThreadStatisticsManager::Dump );
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "dumpstat",				&KThreadStatisticsManager::DumpStatistics );
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "AddStatistics",			&KThreadStatisticsManager::AddStatistics_LUA );
	//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "SetStatisticsFlushTime",	&KThreadStatisticsManager::SetStatisticsFlushTime_LUA );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "OnFlushLocalLog",			&KThreadStatisticsManager::OnFlushLocalLog );
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "FlushData",				&KThreadStatisticsManager::FlushData );
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "ClearLocalLogData",		&KThreadStatisticsManager::ClearLocalLogData_ );
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "SendToFTPStatistics",		&KThreadStatisticsManager::SetSendToFTPStatistics_LUA );	
	//{{ 2009. 9. 13  ������	�����α�
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "SetUserLog",				&KThreadStatisticsManager::SetUserLog_LUA );
	//}}
	//{{ 2010. 10. 11	������	�ؽ� ���� ��Ŷ �α�
#ifdef SERV_BILLING_PACKET_LOG
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "SetBillingPacketLog",		&KThreadStatisticsManager::SetBillingPacketLog_LUA );
#endif SERV_BILLING_PACKET_LOG
	//}}
	//{{ 2009. 5. 7  ������		�׽�Ʈ�ڵ�
	//lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "SendServerLog",			&KThreadStatisticsManager::SendServerLog );
	//}}	

#   undef _ENUM
#   define _ENUM( id ) lua_tinker::decl( g_pLua, #id, KStatistics::##id );
#   include "StatisticsID_def.h"

	lua_tinker::decl( g_pLua, "StatisticsManager", this );
}

void KThreadStatisticsManager::AddStatistics_LUA( int iStatisticsID, float fFlushGap, bool bWriteToDB )
{
	KStatisticsPtr spStatistics = KStatisticsPtr( new KStatistics );
	spStatistics->Init( iStatisticsID, fFlushGap, bWriteToDB );
	m_mapStatistics.insert( std::make_pair( iStatisticsID, spStatistics ) );
}

//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
void KThreadStatisticsManager::SetStatisticsFlushTime_LUA( int iStatisticsID, int iHour )
{
	std::map< int, KStatisticsPtr >::iterator mit;
	mit = m_mapStatistics.find( iStatisticsID );
	if( mit == m_mapStatistics.end() )
	{
		START_LOG( cerr, L"��� ��ü�� �������� �ʽ��ϴ�!" )
			<< BUILD_LOG( iStatisticsID )
			<< BUILD_LOG( iHour )
			<< END_LOG;
		return;
	}

	KStatisticsPtr spStatistics = mit->second;
	if( spStatistics == NULL )
	{
		START_LOG( cerr, L"��� ��ü ������ ���� �̻��մϴ�!" )
			<< BUILD_LOG( iStatisticsID )
			<< BUILD_LOG( iHour )
			<< END_LOG;
		return;
	}
	
	// Ư�� �ð� Flush����
	spStatistics->SetFlushTime( iHour );
}
#endif SERV_ITEM_STATISTICS_TO_DB
//}}

void KThreadStatisticsManager::SetSendToFTPStatistics_LUA( bool bSendToFTPStatistics )
{
	m_bSendToFTPStatistics = bSendToFTPStatistics;

	START_LOG( cout2, L"SetSendToFTPStatistics : " << m_bSendToFTPStatistics );
}

void KThreadStatisticsManager::SetUserLog_LUA( bool bVal )
{
	m_bUserLog = bVal;

	START_LOG( cout2, L"SetUserLog : " << m_bUserLog );
}

//{{ 2010. 10. 11	������	�ؽ� ���� ��Ŷ �α�
#ifdef SERV_BILLING_PACKET_LOG
void KThreadStatisticsManager::SetBillingPacketLog_LUA( bool bVal )
{
	m_bBillingPacketLog = bVal;

	START_LOG( cout2, L"SetBillingPacketLog : " << m_bBillingPacketLog );
}
#endif SERV_BILLING_PACKET_LOG
//}}

void KThreadStatisticsManager::IncreaseCount( int iStatisticsID, const KStatisticsKey& kKey, int iIndex, int iCount )
{
	KE_STATISTICS_INFO_NOT kNot;
	kNot.AddStatisticsInfo( iStatisticsID, kKey, iIndex, iCount );

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_NULL, NULL, E_STATISTICS_INFO_NOT, kNot );
	m_kTSMEventProcess.QueueingEvent( spEvent );
}

void KThreadStatisticsManager::IncreaseCount( const KE_STATISTICS_INFO_NOT& kInfo )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_NULL, NULL, E_STATISTICS_INFO_NOT, kInfo );
	m_kTSMEventProcess.QueueingEvent( spEvent );
}

void KThreadStatisticsManager::QueueingEventID( unsigned short usEventID )
{
	QueueingEvent( usEventID, char() );
}

int KThreadStatisticsManager::GetCurLocalTime()
{
	time_t t0 = time(0);
#ifdef _CONVERT_VS_2010
	tm t;
	localtime_s(&t, &t0);
	int iCurTime = t.tm_year * 365 * 24 * 60 + t.tm_yday * 24 * 60 + t.tm_hour * 60 + t.tm_min;
#else
	tm* t = localtime(&t0);
	int iCurTime = t->tm_year * 365 * 24 * 60 + t->tm_yday * 24 * 60 + t->tm_hour * 60 + t->tm_min;
#endif _CONVERT_VS_2010

	return iCurTime;
}

void KThreadStatisticsManager::ClearLocalLogData_()
{
	m_kStatisticsLocalLog.ClearLocalLog();

	// ���� �ð� ����
	int iCurTime = GetCurLocalTime();
	std::wfstream fout( "Statistics\\LastUpdateLog.txt", std::ios::out );
	fout << iCurTime;
	fout.close();
}

void KThreadStatisticsManager::_QueueingEvent( const KEventPtr& spEvent_ )
{
	m_kTSMEventProcess.QueueingEvent( spEvent_ );
}

void KThreadStatisticsManager::Tick()
{
	std::map< int, KStatisticsPtr >::iterator mit;
	for( mit = m_mapStatistics.begin(); mit != m_mapStatistics.end(); ++mit )
	{
		mit->second->Tick();
	}

	UpdateFlushLocalLog();

	//{{ 2008. 4. 7  ������  ���Ϸ� �����ϴ� �׳��� ���� �ٽ� ���� �ʴ´�. [���� ��� ��ȹ�� ����]
	//m_kStatisticsLocalLog.LoadLocalLogFromFile();
	//}}

	// �̺�Ʈ ó��
	m_kTSMEventProcess.Tick();


	//{{ 2011. 11. 21  ��μ�	��� �α� ��ġ
#ifdef SERV_STATISTICS_LOG_COUNT
	WriteStatisticsLogCount();
#endif SERV_STATISTICS_LOG_COUNT
	//}}
}

void KThreadStatisticsManager::FlushData()
{
	std::map< int, KStatisticsPtr >::iterator mit;
	for( mit = m_mapStatistics.begin(); mit != m_mapStatistics.end(); ++mit )
	{
		mit->second->FlushData();
	}
}

void KThreadStatisticsManager::DumpStatistics( int iStatisticsID ) const
{
    std::map< int, KStatisticsPtr >::const_iterator mit = m_mapStatistics.find( iStatisticsID );
    if( mit == m_mapStatistics.end() )
    {
        START_LOG( cerr, L"�ش� ��谡 ���ų� ��ϵǾ� ���� ����." )
            << BUILD_LOG( iStatisticsID )
            << BUILD_LOG( KStatistics::GetIDStr( iStatisticsID ) )
            << END_LOG;
    }
    else
	{
        mit->second->Dump();
    }
}

bool KThreadStatisticsManager::CheckStatistics( int iStatisticsID )
{
	std::map< int, KStatisticsPtr >::const_iterator mit = m_mapStatistics.find( iStatisticsID );
	if( mit == m_mapStatistics.end() )
	{
		return false;
	}

	return true;
}

bool KThreadStatisticsManager::UpdateStatistics( int iStatisticsID, const KStatisticsKey& kKey, int iIndex, int iCount )
{
	std::map< int, KStatisticsPtr >::iterator mit;
	mit = m_mapStatistics.find( iStatisticsID );
	if( mit == m_mapStatistics.end() )
	{
		START_LOG( cerr, L"�䷱ ��� ����." )
			<< BUILD_LOG( iStatisticsID )
			<< BUILD_LOG( iIndex )
			<< BUILD_LOG( iCount )
			<< END_LOG;

		return false;
	}

	mit->second->IncreaseCount( kKey, iIndex, iCount );
	return true;
}

//-----------------------------------------------------------------------------
// Local Log

void KThreadStatisticsManager::InitLocalLogKeys()
{
	std::map< int, KStatisticsPtr >::iterator mit;
	for( mit = m_mapStatistics.begin(); mit != m_mapStatistics.end(); ++mit )
	{
		mit->second->InitLocalLogKeys();
	}
}

void KThreadStatisticsManager::WriteLocalLogData( std::wfstream& fout, int iStatisticsID )
{
	std::map< int, KStatisticsPtr >::iterator mit;
	mit = m_mapStatistics.find( iStatisticsID );
	if( mit == m_mapStatistics.end() )
	{
		START_LOG( cerr, L"�䷱ ��� ����." )
			<< BUILD_LOG( iStatisticsID )
			<< END_LOG;

		return;
	}

	mit->second->WriteLocalLogData( fout );
}

//{{ 2007. 12. 18  ������  
void KThreadStatisticsManager::ClearLocalLogData( int iStatisticsID )
{
	std::map< int, KStatisticsPtr >::iterator mit;
	mit = m_mapStatistics.find( iStatisticsID );
	if( mit == m_mapStatistics.end() )
	{
		// �����α� �����ʿ� ����.
		// [����] ���ͼ����� ���Ӽ������� ���Ǵ� Local Log �׸��� �ٸ��� ����.
		return;
	}

	mit->second->ClearLocalLogData();
}
//}}

//{{ 2008. 5. 20  ������  ��� �ڵ� ����
struct KSendLocalLog
{
	KLogReporter& m_kLogReporter;
	const std::wstring m_wstrFindTagName;
	KSendLocalLog( KLogReporter& kLogReporter, const wchar_t* wstrFindTagName )	: m_kLogReporter( kLogReporter ), m_wstrFindTagName( wstrFindTagName ) {}

	void operator()( const WIN32_FIND_DATA& wfd )
	{
		if ( ( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == false )
		{
			std::wstring strFilename = wfd.cFileName;
			//std::transform(strFilename.begin(), strFilename.end(), strFilename.begin(), std::toupper);

			std::wstring strFindTag = m_wstrFindTagName;
			int iPosTag = strFilename.find( strFindTag );
			if ( iPosTag == std::string::npos )
				return;

			std::wstring strFindExt = L".txt";
			int iPosExt = strFilename.rfind( strFindExt );
			if ( iPosExt == std::string::npos )
				return;

			std::wstring strRootName = L"Statistics\\";

			std::wstring strFileFirst = strFilename.substr( 0, iPosExt );
			std::wostringstream ostmDgTag;
			ostmDgTag << strFileFirst;
			KThreadStatisticsManager::AttachTimeTag( ostmDgTag );
			KThreadStatisticsManager::AttachSvnameTag( ostmDgTag );
			ostmDgTag << L".txt";

			m_kLogReporter.InsertFile( (strRootName + strFilename).c_str(), ostmDgTag.str().c_str() );
		}
	}	
};
//}}

void KThreadStatisticsManager::BackUpAndSendFTPLocalLog( bool bSendFileFtp )
{
	START_LOG( cout, L"�ؽ�Ʈ ��� ���� ��� ����!" );

	KLogReporter kLogReporter;
	kLogReporter.SetConnectInfo( m_wstrFTPServerIP, m_iFTPPort );
	kLogReporter.SetAccount( m_wstrFTPID, m_wstrFTPPW );

	// - - - - - - - - - - - - - - - - - - - - - - - - -
	// File
	std::wostringstream ostmPvp;
	ostmPvp << L"SI_LOG_PVP";
	AttachTimeTag( ostmPvp );
	AttachSvnameTag( ostmPvp );
	ostmPvp << L".txt";

	std::wostringstream ostmItem;
	ostmItem << L"SI_LOG_ITEM";
	AttachTimeTag( ostmItem );
	AttachSvnameTag( ostmItem );
	ostmItem << L".txt";
	
	std::wostringstream ostmSpirit;
	ostmSpirit << L"SI_LOG_SPIRIT";
	AttachTimeTag( ostmSpirit );
	AttachSvnameTag( ostmSpirit );
	ostmSpirit << L".txt";
	
	std::wostringstream ostmEnchant;
	ostmEnchant << L"SI_LOG_ENCHANT";
	AttachTimeTag( ostmEnchant );
	AttachSvnameTag( ostmEnchant );
	ostmEnchant << L".txt";

	// - - - - - - - - - - - - - - - - - - - - - - - - -

	m_kStatisticsLocalLog.CloseLocalLogData();

	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_DUNGEON" ) );
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_PVP_VICTORY_PER" ) );
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_PVP_ROOM" ) );
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_DUNGEON_ROOM" ) );
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_TITLE" ) );
	//{{ 2010. 9. 14	������	�� �ý���
#ifdef SERV_PET_SYSTEM
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_PET_SUMMON" ) );
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2010. 10. 11	������	������ ���� ���� �α�
#ifdef SERV_SERVER_DISCONNECT_LOG
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_SERV_DISCONNECT" ) );
#endif SERV_SERVER_DISCONNECT_LOG
	//}}
	//{{ 2010. 10. 11	������	�ؽ� ���� ��Ŷ �α�
#ifdef SERV_BILLING_PACKET_LOG
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_BILLING_PACKET" ) );
#endif SERV_BILLING_PACKET_LOG
	//}}
	//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_ABUSER_MORNITORING" ) );
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}
#ifdef SERV_LOG_UNDEFINED_QUEST_TEMPLET
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_UNDEFINED_QUEST_TEMPLET" ) );
#endif // SERV_LOG_UNDEFINED_QUEST_TEMPLET

	if ( CheckStatistics( KStatistics::SI_LOC_PVP ) == true )
		kLogReporter.InsertFile( L"Statistics\\SI_LOG_PVP.txt", ostmPvp.str().c_str() );

	if ( CheckStatistics( KStatistics::SI_LOC_ITEM ) == true )
		kLogReporter.InsertFile( L"Statistics\\SI_LOG_ITEM.txt", ostmItem.str().c_str() );
	
	if ( CheckStatistics( KStatistics::SI_LOC_SPIRIT ) == true )
		kLogReporter.InsertFile( L"Statistics\\SI_LOG_SPIRIT.txt", ostmSpirit.str().c_str() );
	
	if ( CheckStatistics( KStatistics::SI_LOC_ENCHANT ) == true )
		kLogReporter.InsertFile( L"Statistics\\SI_LOG_ENCHANT.txt", ostmEnchant.str().c_str() );

	// FTP���� ���� ���
	BackUpSendFTPFiles( kLogReporter.m_vecFileList );

	START_LOG( cout, L"�ؽ�Ʈ ��� ���� ��� �Ϸ�!" );

	// FTP����
	if( bSendFileFtp )
	{
		START_LOG( cout, L"�ؽ�Ʈ ��� ���� FTP ���� ����!" );

		kLogReporter.Send();

		START_LOG( cout, L"�ؽ�Ʈ ��� ���� FTP ���� �Ϸ�!" );
	}
}

void KThreadStatisticsManager::SendHenirRankingLog( int iRankingType )
{
	START_LOG( cout, L"��ϸ� ��ŷ �α� ���� ��� ����!" )
		<< BUILD_LOG( iRankingType );

	KLogReporter kLogReporter;
	kLogReporter.SetConnectInfo( m_wstrFTPServerIP, m_iFTPPort );
	kLogReporter.SetAccount( m_wstrFTPID, m_wstrFTPPW );

	m_kStatisticsLocalLog.CloseHenirRankingLogData( iRankingType );

	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_HENIR_RANKING" ) );

	// FTP���� ���� ���
	BackUpSendFTPFiles( kLogReporter.m_vecFileList );

	START_LOG( cout, L"��ϸ� ��ŷ �α� ���� ��� �Ϸ�!" )
		<< BUILD_LOG( iRankingType );

	//kLogReporter.Send(); - ������ �� �ִ�, ���� �׽�Ʈ �ʿ�, �ϴ� ���Ϲ�����̶� �ص���!

	// ���� ����
	m_kStatisticsLocalLog.DeleteHenirRankingLog();
}

//{{ 2008. 6. 27  ������  FTP���� ���� ���
void KThreadStatisticsManager::BackUpSendFTPFiles( const std::vector< KLogReporter::KFileInfo >& vecFileList )
{
	CTime tCurTime = CTime::GetCurrentTime();

	// 1. ��� ���� ����
#ifdef SERV_FTP_PATH_CHANGE
	std::wstring wstrBackupFolder = L"D:\\StatisticsBackup\\";
#else // SERV_FTP_PATH_CHANGE
	std::wstring wstrBackupFolder = L"C:\\StatisticsBackup\\";
#endif // SERV_FTP_PATH_CHANGE
	CreateDirectoryW( wstrBackupFolder.c_str(), NULL );
		
	// 2. ���ں� ���� ����
	wstrBackupFolder += ( CStringW )( tCurTime.Format( _T( "%Y-%m-%d\\" ) ) );
	CreateDirectoryW( wstrBackupFolder.c_str(), NULL );

	// 3. ��� �� �α� ���� ���
	std::vector< KLogReporter::KFileInfo >::const_iterator vit;
	for( vit = vecFileList.begin(); vit != vecFileList.end(); ++vit )
	{	
		const KLogReporter::KFileInfo& kFileInfo = *vit;		

		std::wstring wstrDesFileName = wstrBackupFolder;
		wstrDesFileName += kFileInfo.m_strRemoteFileName;

		CopyFileW( kFileInfo.m_strLocalFileName.c_str(), wstrDesFileName.c_str(), false );
	}
}
//}}

void KThreadStatisticsManager::AttachTimeTag( std::wostringstream& os )
{
	time_t t0 = time(0);
#ifdef _CONVERT_VS_2010
	tm t;
	localtime_s(&t, &t0);
	os
		<< L"_["
		<< 1900 + t.tm_year	<< L"_"
		<< t.tm_mon + 1		<< L"_"
		<< t.tm_mday		<< L"_"
		<< t.tm_hour		<< L"_"
		<< t.tm_min			<< L"]";
#else
	tm* t = localtime(&t0);
	os
		<< L"_["
		<< 1900 + t->tm_year	<< L"_"
		<< t->tm_mon + 1		<< L"_"
		<< t->tm_mday			<< L"_"
		<< t->tm_hour			<< L"_"
		<< t->tm_min			<< L"]";
#endif _CONVERT_VS_2010
}

void KThreadStatisticsManager::AttachSvnameTag( std::wostringstream& os )
{
	char strTemp[256] = {0};
	::gethostname( strTemp, 256 );

	HOSTENT* hostent;
	char* ip;

	hostent = ::gethostbyname( strTemp );
	ip = ::inet_ntoa( *( struct in_addr* )*hostent->h_addr_list );

	os
		<< L"_[" << ip << L"]";
}

void KThreadStatisticsManager::CheckFlushLocalLog()
{
	//int iCurCheckTime = timeGetTime();
	//if ( iCurCheckTime - m_iLocalLogLastCheckTime < 1 )
	//{
	//	return;
	//}

	// �ʱ�ȭ
	if ( m_iLocalLogLastUpdateTime == 0 )
	{
		std::wfstream fin( "Statistics\\LastUpdateLog.txt", std::ios::in );
		if ( fin.good() )
		{
			fin >> m_iLocalLogLastUpdateTime;
		}
		else
		{
			m_iLocalLogLastUpdateTime = GetCurLocalTime();
		}
		fin.close();
	}
}

void KThreadStatisticsManager::UpdateFlushLocalLog()
{
	CheckFlushLocalLog();

	if ( m_iLocalLogLastUpdateTime == 0 )
		return;

	time_t t0 = time(0);
#ifdef _CONVERT_VS_2010
	tm t;
	localtime_s(&t, &t0);
#else
	tm* t = localtime(&t0);
#endif _CONVERT_VS_2010

	int iCurTime = GetCurLocalTime();

	// App �����, �ѹ��� ����
	static bool bAtOnce = false;
	if ( bAtOnce == false )
	{
		// Ÿ�� ���� 1�� �̻��� ���
		if ( iCurTime - m_iLocalLogLastUpdateTime >= 24 * 60 )
		{
			OnFlushLocalLog();

			ClearLocalLogData_();

			m_iLocalLogLastUpdateTime = iCurTime;

			bAtOnce = true;
		}
	}

	// �����췯, 4�ÿ� ����, Ÿ�� ���� 2�ð� �̻��� ���
#ifdef _CONVERT_VS_2010
	if ( t.tm_hour == 4 && 
		iCurTime - m_iLocalLogLastUpdateTime >= 2 * 60 )
#else
	if ( t->tm_hour == 4 && 
		iCurTime - m_iLocalLogLastUpdateTime >= 2 * 60 )
#endif _CONVERT_VS_2010
	{
		OnFlushLocalLog();

		ClearLocalLogData_();

		m_iLocalLogLastUpdateTime = iCurTime;
	}
}

void KThreadStatisticsManager::OnFlushLocalLog( bool bClearLocalLog /*= false*/ )
{
	m_kStatisticsLocalLog.FlushLocalLogStm();

	// ����� �⺻������ ������ �Ѵ�. FTP������ ���Ǻη� ó��.
	BackUpAndSendFTPLocalLog( m_bSendToFTPStatistics );

	//{{ 2008. 7. 1  ������	 ��� ������ �ʱ�ȭ ����
	if( bClearLocalLog )
	{
		m_kStatisticsLocalLog.ClearLocalLog();
	}
	//}}
}

#endif SERV_STATISTICS_THREAD


//{{ 2011. 11. 21  ��μ�	��� �α� ��ġ
#ifdef SERV_STATISTICS_LOG_COUNT
void KThreadStatisticsManager::UpdateStatisticsLogCount( IN UidType iCount )
{
	CTime tCurr = CTime::GetCurrentTime();
	std::wstring strTime = tCurr.Format( _T("%Y-%m-%d %H:%M:%S") );

	m_mapStatisticsLogCount.insert( std::make_pair( strTime, iCount) );
}

void KThreadStatisticsManager::WriteStatisticsLogCount()
{
	if( m_tStatisticsLogCount.elapsed() > 3600 )
	{
		m_tStatisticsLogCount.restart();
		++m_iStatisticsFileNo;
		
		if( m_iStatisticsFileNo > 24 ) // 1�� ġ�� �������
		{
			return;
		}

		std::string strPath = "Statistics\\StatisticsCount";
		std::string strPath_txt = ".txt"; 
		std::string strPath_Mid = boost::str( boost::format( "%d" ) % m_iStatisticsFileNo );
		
		strPath += strPath_Mid;
		strPath += strPath_txt;

		std::wfstream fout;
		fout.imbue(std::locale("korean", LC_CTYPE));
		fout.open( strPath.c_str(), std::ios::out );

		std::map< std::wstring, UidType >::iterator mit = m_mapStatisticsLogCount.begin();
		if( mit == m_mapStatisticsLogCount.end() )
		{
			return;
		}
		for( ; mit != m_mapStatisticsLogCount.end() ; ++mit )
		{
			fout << mit->first << L"\t" << mit->second << L"\n";
		}

		fout.close();

		m_mapStatisticsLogCount.clear();
	}
}
#endif SERV_STATISTICS_LOG_COUNT
//}}