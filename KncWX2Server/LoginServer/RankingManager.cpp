#include ".\rankingmanager.h"
#include "Enum\Enum.h"
#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "BaseServer.h"
//{{ 2011. 11. 3	������	��ϸ� �ð� ��ŷ ���� ������ ��ġ
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
	#include "LoginServer.h"
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
//}}

#include <iomanip>


ImplementSingleton( KRankingManager );

KRankingManager::KRankingManager(void) :
m_bInit( false ),
m_bReservedRankingReward( false )
{
	// lua
	lua_tinker::class_add<KRankingManager>( g_pLua, "KRankingManager" );
	lua_tinker::class_def<KRankingManager>( g_pLua, "dump",							&KRankingManager::Dump );
	lua_tinker::class_def<KRankingManager>( g_pLua, "clear",						&KRankingManager::ClearRanking_LUA );
	lua_tinker::class_def<KRankingManager>( g_pLua, "AddHenirRankingInfo",			&KRankingManager::AddHenirRankingInfo_LUA );
	lua_tinker::class_def<KRankingManager>( g_pLua, "AddRankingRewardInfo",			&KRankingManager::AddRankingRewardInfo_LUA );
	//{{ 2009. 8. 3  ������		��ŷ ���� ����
	lua_tinker::class_def<KRankingManager>( g_pLua, "SetRankingRewardBeginDate",	&KRankingManager::SetRankingRewardBeginDate_LUA );
	//}}
	lua_tinker::class_def<KRankingManager>( g_pLua, "NewRecordForTest",				&KRankingManager::NewRecordForTest_LUA ); // �׽�Ʈ��
	//{{ 2009. 7. 31  ������	��ŷ ����
	lua_tinker::class_def<KRankingManager>( g_pLua, "NewRecord",					&KRankingManager::NewRecord_LUA ); // ����������
	lua_tinker::class_def<KRankingManager>( g_pLua, "DeleteRecord",					&KRankingManager::DeleteRecord_LUA ); // ����������
	//}}

	lua_tinker::decl( g_pLua, "RankingManager", this );
}

KRankingManager::~KRankingManager(void)
{
}

ImplToStringW( KRankingManager )
{
	// �����̺�Ʈ ������
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_FIELD, NULL, DMP_DUMP_RANKING_MANAGER_NOT );
	spEvent->m_kDestPerformer.AddUID( 0 );

	KBaseServer::GetKObj()->QueueingEvent( spEvent );
	return stm_;
}

bool KRankingManager::OpenScriptFile( const char* pFileName )
{
	_JIF( 0 == LUA_DOFILE( g_pLua, pFileName ), return false );

	return true;
}

bool KRankingManager::AddHenirRankingInfo_LUA( int iRankingType, int iLastRank, int iHour, int iDayOfWeek /*= 0*/, int iWeekOfMonth /*= 0*/ )
{
	if( iRankingType < 0  ||  iHour < 0  ||  iDayOfWeek < 0  ||  iWeekOfMonth < 0 )
	{
		START_LOG( cerr, L"��ϸ��� �ð� ��ŷ ��ũ��Ʈ ������ �̻��մϴ�!" )
			<< BUILD_LOG( iRankingType )
			<< BUILD_LOG( iLastRank )
			<< BUILD_LOG( iHour )
			<< BUILD_LOG( iDayOfWeek )
			<< BUILD_LOG( iWeekOfMonth )
			<< END_LOG;

		return false;
	}
#ifndef SERV_HENIR_NO_REWARD
	// ��ŷ ��ü ����
    KHenirRanking kInfo;
	kInfo.SetLastRank( static_cast<u_int>(iLastRank) );
	kInfo.SetRefreshTime( iRankingType, iWeekOfMonth, iDayOfWeek, iHour );
	m_mapHenirRanking.insert( std::make_pair( iRankingType, kInfo ) );
#endif //SERV_HENIR_NO_REWARD
	return true;
}

bool KRankingManager::AddRankingRewardInfo_LUA( int iRankingType, int iRank, int iTitleID, short sPeriod )
{
	if( iRankingType < 0  ||  iRank <= 0  ||  iTitleID <= 0  ||  sPeriod < 0 )
	{
		START_LOG( cerr, L"��ŷ Ÿ��Ʋ ���� ��ũ��Ʈ ������ �̻��մϴ�!" )
			<< BUILD_LOG( iRankingType )
			<< BUILD_LOG( iRank )
			<< BUILD_LOG( iTitleID )
			<< BUILD_LOG( sPeriod )
			<< END_LOG;
		return false;
	}

	KTitleRewardKey kKey( iRankingType, iRank );
	KTitleReward kReward;
	kReward.m_iTitleID = iTitleID;
	kReward.m_sPeriod = sPeriod;

	std::map< KTitleRewardKey, std::vector< KTitleReward > >::iterator mit;
	mit = m_mapRankingTitleReward.find( kKey );
	if( mit == m_mapRankingTitleReward.end() )
	{
		std::vector< KTitleReward > vecReward;
		vecReward.push_back( kReward );
		m_mapRankingTitleReward.insert( std::make_pair( kKey, vecReward ) );
	}
	else
	{
		mit->second.push_back( kReward );
	}

	return true;
}

//{{ 2009. 8. 3  ������		��ŷ�������
bool KRankingManager::SetRankingRewardBeginDate_LUA( int iRankingType, const char* pBeginDate )
{
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
	if( SEnum::IsHenirRankingType( iRankingType ) == false )
#else // SERV_HENIR_RENEWAL_2013
	if( iRankingType < 0  ||  iRankingType > SEnum::RT_MONTH_RANKING )
#endif // SERV_HENIR_RENEWAL_2013
	{
		START_LOG( cerr, L"Ranking������ �̻��մϴ�!" )
			<< BUILD_LOG( iRankingType )			
			<< END_LOG;

		return false;
	}

	std::wstring wstrBeginDate = KncUtil::toWideString( std::string( pBeginDate ) );

	std::map< int, KHenirRanking >::iterator mit = m_mapHenirRanking.find( iRankingType );
	if( mit == m_mapHenirRanking.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ��ŷ������ ������۽ð��� ����Ϸ��Ͽ���!" )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;

		return false;
	}

	// ��ŷ ���۽ð� ���
	mit->second.SetRewardBeginTime( wstrBeginDate );
	return true;
}
//}}

void KRankingManager::NewRecordForTest_LUA( int iCount )
{
	_JIF( iCount <= 150, return; );

	KELG_ADMIN_NEW_RECORD_TEST_NOT kNot;

	for( int i = 0; i < iCount; ++i )
	{
		CStringW strRefreshTime;
		strRefreshTime.Format( L"%d���༮", i );

		KHenirRankingInfo kInfo;
		kInfo.m_iUnitUID = 34729 + i;
		kInfo.m_wstrNickName = strRefreshTime.GetBuffer();
		kInfo.m_ucLevel = rand() % 50;
		kInfo.m_cUnitClass = ( rand() % 5 ) + 1;
		kInfo.m_iStageCount = ( rand() % 30 ) + 1;
		kInfo.m_iRank = 0;
		kInfo.m_ulPlayTime = ( rand() % 200 ) + 10;

		kNot.m_vecNewRecordTest.push_back( kInfo );
	}

	// �̺�Ʈ ������
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_FIELD, NULL, ELG_ADMIN_NEW_RECORD_TEST_NOT, kNot );
	spEvent->m_kDestPerformer.AddUID( 0 );

	KBaseServer::GetKObj()->QueueingEvent( spEvent );
}

//{{ 2009. 7. 31  ������	��ŷ ����
void KRankingManager::NewRecord_LUA( int iRankingType, int iStageCount, UINT ulPlayTime, __int64 tRegDate )
{
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
	if( ( SEnum::IsHenirRankingType( iRankingType ) == false ) ||  ( iStageCount < 0 ) )
#else // SERV_HENIR_RENEWAL_2013
	if( iRankingType < 0  ||  iRankingType > SEnum::RT_MONTH_RANKING  ||  iStageCount < 0 )
#endif // SERV_HENIR_RENEWAL_2013
	{
		START_LOG( cerr, L"Ranking������ �̻��մϴ�!" )
			<< BUILD_LOG( iRankingType )
			<< BUILD_LOG( iStageCount )
			<< END_LOG;
		return;
	}

	KELG_ADMIN_NEW_RECORD_NOT kNot;
	kNot.m_iRankingType = iRankingType;
	kNot.m_kNewRecordRanking.m_iStageCount = iStageCount;
	kNot.m_kNewRecordRanking.m_ulPlayTime = ulPlayTime;
	kNot.m_kNewRecordRanking.m_tRegDate = tRegDate;

	int iUnitUID = 0;
	int iUnitClass = 0;
	int iLevel = 0;

	KLuaManager luaMgr( g_pLua );
	LUA_GET_VALUE( luaMgr, L"UnitUID",		iUnitUID,		0 );
	LUA_GET_VALUE( luaMgr, L"NickName",		kNot.m_kNewRecordRanking.m_wstrNickName,	L"" );
	LUA_GET_VALUE( luaMgr, L"UnitClass",	iUnitClass,		0 );
	LUA_GET_VALUE( luaMgr, L"Level",		iLevel,			0 );

	if( iUnitUID < 0  ||  iUnitClass < 0  ||  iLevel <= 0  ||  kNot.m_kNewRecordRanking.m_wstrNickName.empty() )
	{
		START_LOG( cerr, L"Ranking Unit������ �̻��մϴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( kNot.m_kNewRecordRanking.m_wstrNickName )
			<< BUILD_LOG( iUnitClass )
			<< BUILD_LOG( iLevel )
			<< END_LOG;
		return;
	}

	kNot.m_kNewRecordRanking.m_iUnitUID = static_cast<UidType>(iUnitUID);
	kNot.m_kNewRecordRanking.m_cUnitClass = static_cast<char>(iUnitClass);
	kNot.m_kNewRecordRanking.m_ucLevel = static_cast<UCHAR>(iLevel);

	// �̺�Ʈ ������
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_FIELD, NULL, ELG_ADMIN_NEW_RECORD_NOT, kNot );
	spEvent->m_kDestPerformer.AddUID( 0 );

	KBaseServer::GetKObj()->QueueingEvent( spEvent );
}

void KRankingManager::DeleteRecord_LUA( int iRankingType, const char* pNickName )
{
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
	if( SEnum::IsHenirRankingType( iRankingType ) == false )
#else // SERV_HENIR_RENEWAL_2013
	if( iRankingType < 0  ||  iRankingType > SEnum::RT_MONTH_RANKING )
#endif // SERV_HENIR_RENEWAL_2013
	{
		START_LOG( cerr, L"RankingType�� �̻��մϴ�!" )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;
		return;
	}

	if( pNickName == NULL )
	{
		START_LOG( cerr, L"Ranking NickName�� �̻��մϴ�!" )
			<< END_LOG;
		return;
	}

	KELG_ADMIN_DELETE_RECORD_NOT kNot;

	kNot.m_iRankingType = iRankingType;
	kNot.m_wstrNickName = KncUtil::toWideString( std::string( pNickName ) );

	// �̺�Ʈ ������
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_FIELD, NULL, ELG_ADMIN_DELETE_RECORD_NOT, kNot );
	spEvent->m_kDestPerformer.AddUID( 0 );

	KBaseServer::GetKObj()->QueueingEvent( spEvent );
}
//}}

void KRankingManager::Tick()
{
	// ��ϸ� �ð� ��ŷ
	TickHenirRanking();
	
	// ����&���� ��ŷ
	TickDungeonAndPvpRanking();
}

void KRankingManager::Init()
{
	// ����/���� ��ŷ ���� �ð� �ʱ�ȭ
	if( m_tResetRankingTime.GetTime() == 0 )
	{
		// ����Ʈ
		CTime tCurTime	 = CTime::GetCurrentTime();
#ifdef SERV_COUNTRY_US
		CTime tResetTime = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), 3, 30, 0 );
#else
		CTime tResetTime = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), 6, 30, 0 );
#endif //SERV_COUNTRY_US
		tResetTime += CTimeSpan( 1, 0, 0, 0 );

		m_tResetRankingTime = tResetTime;

		CStringW strRefreshTime = (CStringW)m_tResetRankingTime.Format( _T("%Y-%m-%d %H:%M:%S") );

		START_LOG( cout, L"����/�������� ��ŷ ���� �ð� ����!" << strRefreshTime.GetBuffer() );
	}

	//////////////////////////////////////////////////////////////////////////	
	// ����, ���� ��ŷ ���
	SendToGameDB( DBE_GET_WEB_RANKING_INFO_REQ );

	//////////////////////////////////////////////////////////////////////////	
	// DB�κ��� ����� ��ϸ��� �ð� ��ŷ �޾ƿ���
	SendToLogDB( DBE_GET_HENIR_RANKING_INFO_REQ );
}

void KRankingManager::ShutDown()
{
	KDBE_BACKUP_RANKING_INFO_NOT kPacketToDB;
	GetHenirRankingInfo( kPacketToDB.m_mapHenirRanking, false );

	// DB Update
	SendToLogDB( DBE_BACKUP_RANKING_INFO_NOT, kPacketToDB );

	// Refresh Time File Backup
	RefreshTimeFileBackup();
}

void KRankingManager::DumpRankingInfo()
{
	START_LOG( cout, L"-------------------- Dump RankingManager --------------------" )
		<< BUILD_LOG( m_bInit )
		<< BUILD_LOG( m_bReservedRankingReward );

	std::map< int, KHenirRanking >::const_iterator mit;
	for( mit = m_mapHenirRanking.begin(); mit != m_mapHenirRanking.end(); ++mit )
	{
		CTime tRefreshTime = mit->second.GetRefreshTime();

		CStringW strRefreshTime = (CStringW)tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );

		START_LOG( cout, L"Ranking Type : " << mit->first )
			<< BUILD_LOG( mit->second.IsRankingChanged() )
			<< BUILD_LOG( mit->second.GetRankingInfo().size() )
			<< BUILD_LOG( strRefreshTime.GetBuffer() );
	}
}

void KRankingManager::TickHenirRanking()
{
	// 1. 5�ʸ��� üũ
	if( m_tBroadCastTimer.elapsed() < 5.0 )
		return;

	// 2. Ÿ�̸� �ʱ�ȭ	
	m_tBroadCastTimer.restart();

	// 3. Init()�Լ� ȣ�� �� DB�κ��� ����� ��ŷ �޾ƿ��Ⱑ �����ߴٸ�..	
	if( !m_bInit )
	{
		START_LOG( cerr, L"5�ʾȿ� ��ϸ� �ð� ��ŷ������ DB�κ��� ���� ���Ͽ����ϴ�. �ٽ� ��ŷ ���� ��û�մϴ�." );

		// 5�ʾȿ� DB�κ��� ��ŷ ������ �������� �ٽ� �޾ƿ����� ����!
		SendToLogDB( DBE_GET_HENIR_RANKING_INFO_REQ );
		return;
	}

	// 4. ��ŷ ���� ����
	if( m_bReservedRankingReward )
	{
		if( CheckRefreshTimeFileBackup() == false )
		{
			m_bReservedRankingReward = true;
		}
	}

	// 4. ��ŷ �ʱ�ȭ �� ���� üũ
	if( m_tClearTimer.elapsed() > 60.0 )
	{
		CheckRankingClearAndReward();

		m_tClearTimer.restart();
	}

	// 5. ���ŵ� ��ŷ ���
	KELG_HENIR_RANKING_REFRESH_NOT kPacket;
	GetHenirRankingInfo( kPacket.m_mapHenirRanking, true );

	// 6. ��� ���Ӽ����� ��ε� ĳ����
	if( kPacket.m_mapHenirRanking.empty() == false )
	{
		UidType anTrace[2] = { 0, -1 };
		KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, ELG_HENIR_RANKING_REFRESH_NOT, kPacket );
	}    
}

void KRankingManager::ClearRanking_LUA( bool bSendToFTP /*= true*/ )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( 0, NULL, ELG_CLEAR_RANKING_NOT, bSendToFTP );

	// �̺�Ʈ ť��
	KBaseServer::GetKObj()->QueueingEvent( spEvent );
}

bool KRankingManager::BackupRankingLogFile( IN int iRankingType, IN bool bSendToFTP /*= true*/ )
{	
	// �ش� ��ŷ Ÿ�Կ� ���� ��ŷ ������ ���Ϸ� ����� FTP�� ����
	std::map< int, KHenirRanking >::const_iterator mit;
	mit = m_mapHenirRanking.find( iRankingType );
	if( mit == m_mapHenirRanking.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ��ŷŸ���� ��ŷ������ LogFile�� ����� �ߴ�" )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;

		return false;
	}

	const std::vector< KHenirRankingInfo >& vecRankingList = mit->second.GetRankingInfo();

	//{{ 2010. 06. 21  ������	��� ������
#ifdef SERV_STATISTICS_THREAD
	//////////////////////////////////////////////////////////////////////////
	KE_LOCAL_LOG_HENIR_RANKING_NOT kNot;
	kNot.m_iRankingType = iRankingType;
	kNot.m_vecRankingInfo = vecRankingList;
	KSIManager.QueueingEvent( E_LOCAL_LOG_HENIR_RANKING_NOT, kNot );
	//////////////////////////////////////////////////////////////////////////
#else
	//////////////////////////////////////////////////////////////////////////
	
	for( u_int ui = 0; ui < vecRankingList.size(); ++ui )
	{
		const KHenirRankingInfo& kRankingInfo = vecRankingList[ui];

		// ��� �ð� ���
		CTime kEndGameTime = CTime( kRankingInfo.m_tRegDate );

#define KLocAlign std::setw(8) << std::setiosflags( std::ios::left )

		std::wfstream& fout = KSIManager.GetLocalLog().LocalLogStm_Henir_Ranking( iRankingType );

		fout << L"\n";
		fout << KLocAlign << mit->first << L"\t";
		fout << KLocAlign << ui << L"\t";
		fout << KLocAlign << kRankingInfo.m_iRank << L"\t";
		fout << KLocAlign << kRankingInfo.m_iStageCount << L"\t";
		fout << KLocAlign << kRankingInfo.m_ulPlayTime << L"\t";
		fout << KLocAlign << kRankingInfo.m_wstrNickName << L"\t";
		fout << KLocAlign << static_cast<int>(kRankingInfo.m_cUnitClass) << L"\t";
		fout << KLocAlign << static_cast<int>(kRankingInfo.m_ucLevel) << L"\t";
		fout << KLocAlign << (const wchar_t*)kEndGameTime.Format(_T("%Y-%m-%d %H:%M:%S")) << L"\t";

		fout.flush();
	}
	//////////////////////////////////////////////////////////////////////////
#endif SERV_STATISTICS_THREAD
	//}}

	// FTP ����
	if( bSendToFTP )
	{
		//{{ 2010. 06. 21  ������	��� ������
#ifdef SERV_STATISTICS_THREAD
		KE_HENIR_RANKING_LOG_NOT kNot;
		kNot.m_iRankingType = iRankingType;
		KSIManager.QueueingEvent( E_HENIR_RANKING_LOG_NOT, kNot );
#else
		KSIManager.SendHenirRankingLog( iRankingType );
#endif SERV_STATISTICS_THREAD
		//}}
	}

	// Ȯ�� �α�
	START_LOG( cout, L"��ŷ �ʱ�ȭ�� ���� ��ŷ ���� TEXT���� ��� ó�� �Ϸ�!" )
		<< BUILD_LOG( iRankingType )
		<< BUILD_LOG( vecRankingList.size() )
		<< BUILD_LOG( bSendToFTP );

	return true;
}

void KRankingManager::RefreshTimeFileBackup()
{
	std::wstring wstrFilePath;

	wchar_t buff[MAX_PATH] = {0};
	::GetCurrentDirectoryW( MAX_PATH, buff );
	wstrFilePath = buff;
	wstrFilePath += L"\\RefreshTimeBackup.ini";

	std::map< int, KHenirRanking >::const_iterator mit;
	for( mit = m_mapHenirRanking.begin(); mit != m_mapHenirRanking.end(); ++mit )
	{
		CStringW cstrRankingType;
		cstrRankingType.Format( L"RANKING_TYPE_%d", mit->first );
		
		CStringW cstrRefreshTime;
		cstrRefreshTime.Format( L"%d", mit->second.GetRefreshTime().GetTime() );

		::WritePrivateProfileStringW( L"Ranking Refresh Time", cstrRankingType.GetBuffer(), cstrRefreshTime.GetBuffer(), wstrFilePath.c_str() );
	}

	START_LOG( cout, L"���� ���� �� ��ŷ ���� �ð� ��� �Ϸ�!" )
		<< BUILD_LOG( wstrFilePath );
}

bool KRankingManager::CheckRefreshTimeFileBackup()
{
	bool bCheckSuccess = true;
	std::wstring wstrFilePath;

	CTime tCurTime = CTime::GetCurrentTime();

	wchar_t buff[MAX_PATH] = {0};
	::GetCurrentDirectoryW( MAX_PATH, buff );
	wstrFilePath = buff;
	wstrFilePath += L"\\RefreshTimeBackup.ini";

	char strFilePath[255] = {0,};
	::WideCharToMultiByte( CP_ACP, 0, wstrFilePath.c_str(), -1, strFilePath, 255, NULL, NULL );

	std::ifstream ifs;
	ifs.open( strFilePath );
	if( !ifs.is_open() )
	{
		START_LOG( cout, L"RefreshTimeBackup������ �����ϴ�! [����] ������ ó�� ���۽����� ��� ������ ���� �� �ֽ��ϴ�." )
			<< BUILD_LOG( wstrFilePath );

		return true;
	}

	std::map< int, KHenirRanking >::iterator mit;
	for( mit = m_mapHenirRanking.begin(); mit != m_mapHenirRanking.end(); ++mit )
	{
		// ��ŷ ���� ���۽ð�
		if( tCurTime < mit->second.GetRewardBeginTime() )
			continue;

		CStringW cstrRankingType;
		cstrRankingType.Format( L"RANKING_TYPE_%d", mit->first );

		memset( buff, 0, MAX_PATH );

		::GetPrivateProfileStringW( L"Ranking Refresh Time", cstrRankingType.GetBuffer(), 0, buff, MAX_PATH, wstrFilePath.c_str() );

		CTime tBackupRefreshTime( static_cast<__time64_t>(_wtoi( buff )) );

		// ����Ǿ��� refreshtime�� ������ refreshtime�� ���Ѵ�!
		if( tBackupRefreshTime == mit->second.GetRefreshTime() )
			continue;

		// ��ŷ ���� ó��
		if( RankingRewardProcess( mit->first ) == false )
		{
			START_LOG( clog, L"����ó�� ����! �����س��� ���� Tick�� �ٽ��ѹ� �õ�����!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			bCheckSuccess = false;
			continue;
		}

		// FTP�� ����
		if( BackupRankingLogFile( mit->first, true ) == false )
		{
			START_LOG( cerr, L"���Ϲ�� �� FTP���� ����!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			// �α׸� ����~
		}

		// ��ŷ ���� �ʱ�ȭ
		mit->second.ClearRanking();

#ifndef SERV_NOT_DELETE_HENIR_RANKING
		// ��ŷ ��������� �ʱ�ȭ
		KDBE_DELETE_RANKING_INFO_NOT kNot;
		kNot.m_iRankingType = mit->first;
		SendToLogDB( DBE_DELETE_RANKING_INFO_NOT, kNot );
#endif // SERV_NOT_DELETE_HENIR_RANKING

		// ��� ó���� �����Ͽ����� RefreshTimeBackup ������ ������
		CStringW cstrRefreshTime;
		cstrRefreshTime.Format( L"%d", mit->second.GetRefreshTime().GetTime() );

		::WritePrivateProfileStringW( L"Ranking Refresh Time", cstrRankingType.GetBuffer(), cstrRefreshTime.GetBuffer(), wstrFilePath.c_str() );

		START_LOG( cout, L"���� ���� �ð����� RefreshTime�� ���ԵǾ��־ �������ڸ��� ����ó����!" )
			<< BUILD_LOG( mit->first )
			<< BUILD_LOG( tBackupRefreshTime.GetTime() );
	}

	return bCheckSuccess;
}

void KRankingManager::CheckRankingClearAndReward( bool bForce /*= false*/ )
{
	CTime tCurTime = CTime::GetCurrentTime();

	std::map< int, KHenirRanking >::iterator mit;
	for( mit = m_mapHenirRanking.begin(); mit != m_mapHenirRanking.end(); ++mit )
	{
		//{{ 2009. 8. 3  ������		��ŷ �ʱ�ȭ ���۽ð�
		if( tCurTime < mit->second.GetRewardBeginTime()  &&  !bForce )
			continue;
		//}}

		// �ʱ�ȭ �Ҷ��� �Ǿ���?
		if( tCurTime < mit->second.GetRefreshTime()  &&  !bForce )
			continue;
		
		// ��ŷ ���� ó��
		if( RankingRewardProcess( mit->first ) == false )
		{
			START_LOG( cerr, L"��ŷ ���� ó�� ����!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// FTP�� ����
		if( BackupRankingLogFile( mit->first, true ) == false )
		{
			START_LOG( cerr, L"���Ϲ�� �� FTP���� ����!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			// �α׸� ����~
		}

		// ��ŷ ���� �ʱ�ȭ
		mit->second.ClearRanking();

#ifndef SERV_NOT_DELETE_HENIR_RANKING
		// ��ŷ ��������� �ʱ�ȭ
		KDBE_DELETE_RANKING_INFO_NOT kNot;
		kNot.m_iRankingType = mit->first;
		SendToLogDB( DBE_DELETE_RANKING_INFO_NOT, kNot );
#endif // SERV_NOT_DELETE_HENIR_RANKING

		// ���� RefreshTime ����
		mit->second.NextRefreshTimeSet();
	}
}

void KRankingManager::GetHenirRankingInfo( std::map< int, std::vector< KHenirRankingInfo > >& mapHenirRanking, bool bChangedOnly )
{
	mapHenirRanking.clear();

#ifndef SERV_HENIR_NO_REWARD
	if( bChangedOnly )
	{
		// ��ŷ 100���ȿ� ����� �˻��Ͽ� ��ٸ� ��ŷ����
		std::map< int, KHenirRanking >::iterator mitHR;
		for( mitHR = m_mapHenirRanking.begin(); mitHR != m_mapHenirRanking.end(); ++mitHR )
		{
			// ��ŷ�� ���ŵǾ����� üũ!
			if( mitHR->second.IsRankingChanged() )
			{
				mapHenirRanking.insert( std::make_pair( mitHR->first, mitHR->second.GetRankingInfo() ) );

				// �������� üũ �ȵǵ��� false�� ����
				mitHR->second.SetRankingChanged( false );
			}
		}
	}
	else
	{
		// ��� ��ŷ ���
		std::map< int, KHenirRanking >::const_iterator mitHR;
		for( mitHR = m_mapHenirRanking.begin(); mitHR != m_mapHenirRanking.end(); ++mitHR )
		{
			mapHenirRanking.insert( std::make_pair( mitHR->first, mitHR->second.GetRankingInfo() ) );
		}
	}
#endif //SERV_HENIR_NO_REWARD
}

void KRankingManager::GetHenirRankingInfo( IN int iRankingType, OUT std::map< int, std::vector< KHenirRankingInfo > >& mapHenirRanking )
{
	mapHenirRanking.clear();

	std::map< int, KHenirRanking >::iterator mit;
	mit = m_mapHenirRanking.find( iRankingType );
	if( mit == m_mapHenirRanking.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ��ŷŸ���Դϴ�." )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;

		return;
	}

	mapHenirRanking.insert( std::make_pair( mit->first, mit->second.GetRankingInfo() ) );	
}

void KRankingManager::InitHenirRanking( const std::map< int, std::vector< KHenirRankingInfo > >& mapHenirRanking )
{
	std::map< int, std::vector< KHenirRankingInfo > >::const_iterator mit;
	for( mit = mapHenirRanking.begin(); mit != mapHenirRanking.end(); ++mit )
	{
		std::map< int, KHenirRanking >::iterator mitHR;
		mitHR = m_mapHenirRanking.find( mit->first );
		if( mitHR == m_mapHenirRanking.end() )
		{
			START_LOG( cerr, L"DB�� ���� ���� ��ŷ����Ʈ�� RankingType�� ������ ��ϵ��� �ʾҴ�!" )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOG( mit->second.size() )
				<< END_LOG;
			continue;
		}

		// ��ŷ ����Ʈ ���� ������Ʈ
		mitHR->second.SetRankingInfo( mit->second );

		START_LOG( cout, L"DB�κ��� ��ϸ��� �ð� ��ŷ ���� �ޱ� ����!" )
			<< BUILD_LOG( mit->first )
			<< BUILD_LOG( mit->second.size() );
	}

	// ��ϸ��� �ð� ��ŷ ��� ������ �ε� ����!
	m_bInit = true;

	// ������ shutdown�Ǿ��� �ð����� refresh������ ���ԵǾ����� �˻��Ͽ� ����ó��
	if( CheckRefreshTimeFileBackup() == false )
	{
		m_bReservedRankingReward = true; // ó�� ���н� �ٽ�ó������!
	}
}

void KRankingManager::GetHenirLastRank( std::map< int, u_int >& mapLastRank )
{
	mapLastRank.clear();

	std::map< int, KHenirRanking >::iterator mitHR;
	for( mitHR = m_mapHenirRanking.begin(); mitHR != m_mapHenirRanking.end(); ++mitHR )
	{
		mapLastRank.insert( std::make_pair( mitHR->first, mitHR->second.GetLastRank() ) );
	}
}

bool KRankingManager::NewRecordHenirRanking( const KHenirRankingInfo& kNewRecord )
{
	bool bNewRecord = false;

	// ��ŷ�� ��� �õ�! ���ο� ��ũ�� ��ϼ����ϸ� true��ȯ!
	std::map< int, KHenirRanking >::iterator mit;
	for( mit = m_mapHenirRanking.begin(); mit != m_mapHenirRanking.end(); ++mit )
	{
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-17	// �ڼ���
		if( ( mit->first == SEnum::RT_HERO_RANKING ) && ( kNewRecord.QualificationForHeroRank() == false ) )
			continue;
#endif // SERV_HENIR_RENEWAL_2013

		if( mit->second.CheckNewRecord( kNewRecord ) == true )
		{
			bNewRecord = true;
		}
	}

	return bNewRecord;
}

bool KRankingManager::NewRecordHenirRanking( IN const KHenirRankingInfo& kNewRecord, IN int iRankingType )
{
	// ��ŷ�� ��� �õ�! ���ο� ��ũ�� ��ϼ����ϸ� true��ȯ!
	std::map< int, KHenirRanking >::iterator mit;
	mit = m_mapHenirRanking.find( iRankingType );
	if( mit == m_mapHenirRanking.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� ��ŷŸ���Դϴ�." )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;

		return false;
	}

	START_LOG( cout2, L"----- ��ŷ �߰� ���� -----" )
		<< BUILD_LOG( kNewRecord.m_iStageCount )
		<< BUILD_LOG( kNewRecord.m_ulPlayTime )
		<< BUILD_LOG( kNewRecord.m_tRegDate )
		<< BUILD_LOG( kNewRecord.m_iUnitUID )
		<< BUILD_LOG( kNewRecord.m_wstrNickName )
		<< BUILD_LOGc( kNewRecord.m_cUnitClass )
		<< BUILD_LOGc( kNewRecord.m_ucLevel );

	return mit->second.CheckNewRecord( kNewRecord );
}

//{{ 2009. 7. 31  ������	��ŷ ����
void KRankingManager::NewRecordForTest( const std::vector< KHenirRankingInfo >& vecNewRecordList )
{
	std::vector< KHenirRankingInfo >::const_iterator vit;
	for( vit = vecNewRecordList.begin(); vit != vecNewRecordList.end(); ++vit )
	{
		SiKRankingManager()->NewRecordHenirRanking( *vit );
	}
}

bool KRankingManager::DeleteRecord( int iRankingType, const std::wstring& wstrNickName )
{
	std::map< int, KHenirRanking >::iterator mit;
	mit = m_mapHenirRanking.find( iRankingType );
	if( mit == m_mapHenirRanking.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� RankingType�Դϴ�." )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;

		return false;
	}
    
	return mit->second.DeleteRecord( wstrNickName );
}

bool KRankingManager::DeleteRecord( int iRankingType, UidType iUnitUID )
{
	std::map< int, KHenirRanking >::iterator mit;
	mit = m_mapHenirRanking.find( iRankingType );
	if( mit == m_mapHenirRanking.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� RankingType�Դϴ�." )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;

		return false;
	}

	return mit->second.DeleteRecord( iUnitUID );
}
//}}

//{{ 2011.03.04  �ӱԼ� ��ϸ� ��ŷ ���� ġƮ ( ���,������ ���� )
#ifdef SERV_DELETE_HENIR_RANKING
bool KRankingManager::DeleteSearchRank( int iRankingType, int iRank)
{
	std::map< int, KHenirRanking >::iterator mit;
	mit = m_mapHenirRanking.find( iRankingType );
	if( mit == m_mapHenirRanking.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� RankingType�Դϴ�." )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;

		return false;
	}

	std::vector< KHenirRankingInfo > vecRankingInfo = mit->second.GetRankingInfo();
	std::vector< KHenirRankingInfo >::iterator vitFind;
	for( vitFind = vecRankingInfo.begin(); vitFind != vecRankingInfo.end(); ++vitFind )
	{
		if( vitFind->m_iRank == iRank )
		{
			return mit->second.DeleteRecord( vitFind->m_wstrNickName );
		}
	}

	return false;
}
#endif SERV_DELETE_HENIR_RANKING
//}}

//{{ 2009. 7. 7  ������		��ŷ ����
bool KRankingManager::RankingRewardProcess( IN int iRankingType )
{
	KELG_RANKING_TITLE_REWARD_NOT kPacketNot;

	std::map< int, KHenirRanking >::const_iterator mit;
	mit = m_mapHenirRanking.find( iRankingType );
	if( mit == m_mapHenirRanking.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� RankingType�Դϴ�." )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;

		return false;
	}
	
	// 1. Īȣ ���� ó��
	const std::vector< KHenirRankingInfo >& vecRankingList = mit->second.GetRankingInfo();
	std::vector< KHenirRankingInfo >::const_iterator vit;
	for( vit = vecRankingList.begin(); vit != vecRankingList.end(); ++vit )
	{
		KRankingTitleReward kRewardInfo;
        if( GetRankingTitleRewardInfo( iRankingType, vit->m_iRank, kRewardInfo.m_vecReward ) == false )
			continue;
		
		kRewardInfo.m_iUnitUID = vit->m_iUnitUID;
		kPacketNot.m_mapTitleReward.insert( std::make_pair( vit->m_iUnitUID, kRewardInfo ) );
	}

	if( kPacketNot.m_mapTitleReward.empty() )
		return true;

	//{{ 2011. 11. 3	������	��ϸ� �ð� ��ŷ ���� ������ ��ġ
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
	// 3. ���Ӽ����� ��ŷ ���� ������
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_LOGIN_NX_AUTH, NULL, ELG_RANKING_TITLE_REWARD_NOT, kPacketNot );
	GetKLoginServer()->QueueingEvent( spEvent );
#else
	//// 2. ����� ���Ӽ����� �ѱ����� ����
	//UidType iGameServerUID = KBaseServer::GetKObj()->GetFirstActorKey();
	//if( iGameServerUID <= 0 )
	//{
	//	START_LOG( clog, L"��ŷ Ÿ��Ʋ ������ ���Ӽ����� �����Ϸ��µ� ActorManager�� ����� ���Ӽ����� �ϳ��� ����.." )
	//		<< BUILD_LOG( iGameServerUID )
	//		<< END_LOG;

	//	return false;
	//}

	//// 3. ���Ӽ����� ��ŷ ���� ������
	//UidType anTrace[2] = { iGameServerUID, -1 };
	//KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, iGameServerUID, anTrace, ELG_RANKING_TITLE_REWARD_NOT, kPacketNot );
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
	//}}	

	// 4. Ȯ�� �α�
	START_LOG( cout, L"��ŷ Īȣ ���� ó�� �Ϸ�!" )
		<< BUILD_LOG( iRankingType )
		<< BUILD_LOG( kPacketNot.m_mapTitleReward.size() );

	return true;
}

bool KRankingManager::GetRankingTitleRewardInfo( IN int iRankingType, IN int iRank, OUT std::vector< KTitleReward >& vecReward )
{
	vecReward.clear();

	KTitleRewardKey kKey;
	kKey.first  = iRankingType;
	kKey.second = iRank;

	std::map< KTitleRewardKey, std::vector< KTitleReward > >::const_iterator mit;
	mit = m_mapRankingTitleReward.find( kKey );
	if( mit == m_mapRankingTitleReward.end() )
		return false;

	vecReward = mit->second;
	return true;
}
//}}

//{{ 2009. 11. 9  ������	�α��μ���GameDB
void KRankingManager::TickDungeonAndPvpRanking()
{
	// 1. 1�и��� üũ
	if( m_kRefreshTimer.elapsed() < 60.0 )
		return;

	// 2. Ÿ�̸� �ʱ�ȭ	
	m_kRefreshTimer.restart();

	// 3. ������ �ð��� �Ǹ� ����, ������ŷ �ٽ� ���
	CTime tCurTime = CTime::GetCurrentTime();
	if( tCurTime > m_tResetRankingTime )
	{
		// 4. ����, ���� ��ŷ �ٽ� ���
		SendToGameDB( DBE_GET_WEB_RANKING_INFO_REQ );

		// �������� ����
		m_tResetRankingTime += CTimeSpan( 1, 0, 0, 0 );
	}
}

void KRankingManager::UpdateDungeonRanking( const std::vector< KDungeonRankingInfo >& vecDungeonRanking )
{
	m_vecDungeonRanking.clear();
	m_mapDungeonRanking.clear();

	std::vector< KDungeonRankingInfo >::const_iterator vit;
	for( vit = vecDungeonRanking.begin(); vit != vecDungeonRanking.end(); ++vit )
	{
		m_mapDungeonRanking.insert( std::make_pair( vit->m_iUnitUID, *vit ) );

		if( m_vecDungeonRanking.size() < 100 )
		{
			m_vecDungeonRanking.push_back( *vit );
		}
	}

	START_LOG( cout, L"���� ��ŷ ����!" )
		<< BUILD_LOG( m_mapDungeonRanking.size() )
		<< BUILD_LOG( m_vecDungeonRanking.size() );
}

void KRankingManager::UpdatePvpRanking( const std::vector< KPvpRankingInfo >& vecPvpRanking )
{
	m_vecPvpRanking.clear();
	m_mapPvpRanking.clear();

	std::vector< KPvpRankingInfo >::const_iterator vit;
	for( vit = vecPvpRanking.begin(); vit != vecPvpRanking.end(); ++vit )
	{
		m_mapPvpRanking.insert( std::make_pair( vit->m_iUnitUID, *vit ) );

		if( m_vecPvpRanking.size() < 100 )
		{
			m_vecPvpRanking.push_back( *vit );
		}
	}

	START_LOG( cout, L"���� ��ŷ ����!" )
		<< BUILD_LOG( m_mapPvpRanking.size() )
		<< BUILD_LOG( m_vecPvpRanking.size() );
}

void KRankingManager::GetDungeonRankingVector( u_int uiLastIndex, KELG_WEB_RANKING_REFRESH_NOT& kNot )
{
	// ���Ӽ����� ���������� ���� index�� ��ü ������� ũ�ٸ� ���̻� ���� �ʴ´�.
	if( uiLastIndex >= m_vecDungeonRanking.size() )
		return;

	for( u_int ui = uiLastIndex; ui < m_vecDungeonRanking.size(); ++ui )
	{
		kNot.m_vecDungeonRanking.push_back( m_vecDungeonRanking[ui] );

		if( kNot.m_vecDungeonRanking.size() >= 150 )		
			break;
	}

	kNot.m_uiTotalSize = m_vecDungeonRanking.size();
}

void KRankingManager::GetDungeonRankingMap( u_int uiLastIndex, KELG_WEB_RANKING_REFRESH_NOT& kNot )
{
	// ���Ӽ����� ���������� ���� index�� ��ü ������� ũ�ٸ� ���̻� ���� �ʴ´�.
	if( uiLastIndex >= m_mapDungeonRanking.size() )
		return;

	u_int uiCount = 0;
	std::map< UidType, KDungeonRankingInfo >::const_iterator mit;
	for( mit = m_mapDungeonRanking.begin(); mit != m_mapDungeonRanking.end(); ++mit, ++uiCount )
	{
		if( uiCount < uiLastIndex )
			continue;

		kNot.m_mapDungeonRanking.insert( std::make_pair( mit->first, mit->second ) );

		if( kNot.m_mapDungeonRanking.size() >= 150 )
			break;
	}

	kNot.m_uiTotalSize = m_mapDungeonRanking.size();
}

void KRankingManager::GetPvpRankingVector( u_int uiLastIndex, KELG_WEB_RANKING_REFRESH_NOT& kNot )
{
	// ���Ӽ����� ���������� ���� index�� ��ü ������� ũ�ٸ� ���̻� ���� �ʴ´�.
	if( uiLastIndex >= m_vecPvpRanking.size() )
		return;

	for( u_int ui = uiLastIndex; ui < m_vecPvpRanking.size(); ++ui )
	{
		kNot.m_vecPvpRanking.push_back( m_vecPvpRanking[ui] );

		if( kNot.m_vecPvpRanking.size() >= 150 )
			break;
	}

	kNot.m_uiTotalSize = m_vecPvpRanking.size();
}

void KRankingManager::GetPvpRankingMap( u_int uiLastIndex, KELG_WEB_RANKING_REFRESH_NOT& kNot )
{
	// ���Ӽ����� ���������� ���� index�� ��ü ������� ũ�ٸ� ���̻� ���� �ʴ´�.
	if( uiLastIndex >= m_mapPvpRanking.size() )
		return;

	u_int uiCount = 0;
	std::map< UidType, KPvpRankingInfo >::const_iterator mit;
	for( mit = m_mapPvpRanking.begin(); mit != m_mapPvpRanking.end(); ++mit, ++uiCount )
	{
		if( uiCount < uiLastIndex )
			continue;

		kNot.m_mapPvpRanking.insert( std::make_pair( mit->first, mit->second ) );

		if( kNot.m_mapPvpRanking.size() >= 150 )
			break;
	}

	kNot.m_uiTotalSize = m_mapPvpRanking.size();
}
//}}

void KRankingManager::SendToGameDB( unsigned short usEventID )
{
	SendToGameDB( usEventID, char() );
}

void KRankingManager::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}



