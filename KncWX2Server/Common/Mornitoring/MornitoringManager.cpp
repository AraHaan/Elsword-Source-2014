#include "NetLayer.h"
#include "SimLayer.h"
#include "DBLayer.h"
#include "Room/RoomManager.h"
#include "BaseServer.h"
#include ".\mornitoringmanager.h"
#include "Socket/NetCommon.h"
#include "Enum/Enum.h"

//{{ 2012. 11. 16	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_MORNITORING
//////////////////////////////////////////////////////////////////////////


ImplementRefreshSingleton( KMornitoringManager );

KMornitoringManager::KMornitoringManager(void)
{
	for( u_int ui = 0; ui < ET_MAX; ++ui )
	{
		m_mapErrStatistics[EST_ONE_DAY].insert( std::make_pair( static_cast<ERR_TYPE>(ui), 0 ) );
	}

	CTime tCurTime = CTime::GetCurrentTime();
	m_NextSendTime = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), ME_SMS_SEND_TIME, 0, 0, 0 );
	if( m_NextSendTime < tCurTime )
	{
		m_NextSendTime += CTimeSpan( 1, 0, 0, 0	);
	}

	std::wstring wstrNextSendTime = (const wchar_t*)m_NextSendTime.Format(_T("%Y-%m-%d %H:%M:%S"));

	START_LOG( cout, L"������ ���� ����͸� SMS ���� �ð� ����!" )
		<< BUILD_LOG( wstrNextSendTime );

	m_bSendSMS = false;
	m_bSendMail = false;

	//{{ 2011. 03. 30	������	���� ���� ����
#ifdef SERV_ROOM_TICK_DUMP
	m_tDumpSendTime = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), 9, 0, 0 );
	if( m_tDumpSendTime < tCurTime )
	{
		m_tDumpSendTime += CTimeSpan( 1, 0, 0, 0 );
	}
	std::wstring wstrDumpSendTime = (const wchar_t*)m_tDumpSendTime.Format(_T("%Y-%m-%d %H:%M:%S"));

	START_LOG( cout, L"Dump ���� ���� �ð� ����!" )
		<< BUILD_LOG( wstrDumpSendTime );
#endif SERV_ROOM_TICK_DUMP
	//}}

	//{{ 2012. 03. 29	��μ�		x2.exe ��� ���� �˻�
#ifdef SERV_CHECK_X2_EXE_MODULE
	m_bCheckExistModule	= false;
#endif SERV_CHECK_X2_EXE_MODULE
	//}}
}

KMornitoringManager::~KMornitoringManager(void)
{
}

ImplToStringW( KMornitoringManager )
{
	stm_	<< L"----------[ Mornitoring Manager ]----------" << std::endl
			<< TOSTRINGW( m_vecPhoneNumList.size() )
			<< TOSTRINGW( m_vecServerNetworkMailList.size() )
			//{{ 2011. 03. 30	������	���� ���� ����
#ifdef SERV_ROOM_TICK_DUMP
			<< TOSTRINGW( m_vecServerDumpMailList.size() )
#endif SERV_ROOM_TICK_DUMP
			//}}
			<< TOSTRINGW( m_mapErrStatistics[EST_ONE_DAY].size() )
			<< TOSTRINGW( m_mapErrStatistics[EST_5_MINUTES].size() )
			<< TOSTRINGW( m_bSendSMS )
			<< TOSTRINGW( m_bSendMail )
			;

	return stm_;
}

ImplementLuaScriptParser( KMornitoringManager )
{
	lua_tinker::class_add<KMornitoringManager>( GetLuaState(), "KMornitoringManager" );
	lua_tinker::class_def<KMornitoringManager>( GetLuaState(), "SetSendSMS",				&KMornitoringManager::SetSendSMS_LUA );
	lua_tinker::class_def<KMornitoringManager>( GetLuaState(), "SetSendMail",				&KMornitoringManager::SetSendMail_LUA );
	lua_tinker::class_def<KMornitoringManager>( GetLuaState(), "AddPhoneNum",				&KMornitoringManager::AddPhoneNum_LUA );
	lua_tinker::class_def<KMornitoringManager>( GetLuaState(), "AddServerNetworkMailAddr",	&KMornitoringManager::AddServerNetworkMailAddr_LUA );
	//{{ 2011. 03. 30	������	���� ���� ����
#ifdef SERV_ROOM_TICK_DUMP
	lua_tinker::class_def<KMornitoringManager>( GetLuaState(), "AddServerDumpMailAddr",		&KMornitoringManager::AddServerDumpMailAddr_LUA );
	lua_tinker::class_def<KMornitoringManager>( GetLuaState(), "AddDumpServerUID",			&KMornitoringManager::AddDumpServerUID_LUA );
#endif SERV_ROOM_TICK_DUMP
	//}}
	//{{ 2012. 03. 29	��μ�		x2.exe ��� ���� �˻�
#ifdef SERV_CHECK_X2_EXE_MODULE
	lua_tinker::class_def<KMornitoringManager>( GetLuaState(), "CheckModuleList_X2Module",			&KMornitoringManager::CheckModuleList_X2Module_LUA );
	lua_tinker::class_def<KMornitoringManager>( GetLuaState(), "AddCheckModuleList",				&KMornitoringManager::AddCheckModuleList_LUA );
#endif SERV_CHECK_X2_EXE_MODULE
	//}}
	lua_tinker::class_def<KMornitoringManager>( GetLuaState(), "dump",						&KMornitoringManager::Dump );

	lua_tinker::decl( GetLuaState(), "MornitoringManager", this );
}

void KMornitoringManager::SetSendSMS_LUA( bool bVal )
{
	m_bSendSMS = bVal;

	START_LOG( cout, L"������ ���� ����͸� SMS ���� ���� : " << m_bSendSMS );
}

void KMornitoringManager::SetSendMail_LUA( bool bVal )
{
	m_bSendMail = bVal;

	START_LOG( cout, L"������ ���� ����͸� Mail ���� ���� : " << m_bSendMail );
}

void KMornitoringManager::AddPhoneNum_LUA( const char* pszPhoneNum )
{
	m_vecPhoneNumList.push_back( KncUtil::toWideString( pszPhoneNum ) );
}

void KMornitoringManager::AddServerNetworkMailAddr_LUA( const char* pszMailAddr )
{
	m_vecServerNetworkMailList.push_back( KncUtil::toWideString( pszMailAddr ) );
}

//{{ 2011. 03. 30	������	���� ���� ����
#ifdef SERV_ROOM_TICK_DUMP
void KMornitoringManager::AddServerDumpMailAddr_LUA( const char* pszMailAddr )
{
	m_vecServerDumpMailList.push_back( KncUtil::toWideString( pszMailAddr ) );
}

void KMornitoringManager::AddDumpServerUID_LUA( int iServerUID )
{
    m_setDumpServerUIDList.insert( iServerUID );
}
#endif SERV_ROOM_TICK_DUMP
//}}

void KMornitoringManager::Tick_GSSImLayer()
{
	// 5�п� �ѹ��� üũ!
	if( m_tTimer.elapsed() < 300.0 )
		return;

	m_tTimer.restart();

	// 5�� ������ ���� ����!
	if( m_mapErrStatistics[EST_5_MINUTES].empty() == false )
	{
		// ���� ����!		
		if( m_bSendMail )
		{
			KEMAIL_SEND_EMAIL_NOT kPacketNot;
			CheckDisconnectAndGetMail_GameServer( kPacketNot );
			KBaseServer::GetKObj()->QueueingEventToMailManager( EMAIL_SEND_EMAIL_NOT, kPacketNot );

			START_LOG( cout, L"5�� ���� ������ ���� ����͸� ����Ʈ ���� ����!" );
		}
		else
		{
			START_LOG( cout, L"5�� ���� ������ ���� ����͸� : ���� �������� �����Ǿ����� ���� ���� ��尡 �ƴ϶� �������� �ʽ��ϴ�!" );
		}

		// �ʱ�ȭ
		m_mapErrStatistics[EST_5_MINUTES].clear();
		m_mapErrStatistics[EST_ONE_DAY].clear();
	}

	//{{ 2011. 03. 30	������	���� ���� ����
#ifdef SERV_ROOM_TICK_DUMP
	CheckServerInfoToMail();
#endif SERV_ROOM_TICK_DUMP
	//}}
}

//{{ 2011. 03. 30	������	���� ���� ����
#ifdef SERV_ROOM_TICK_DUMP

void KMornitoringManager::Tick_CnSimLayer()
{
	// 5�п� �ѹ��� üũ!
	if( m_tTimer.elapsed() < 300.0 )
		return;
    
	m_tTimer.restart();

	//{{ 2011. 03. 30	������	���� ���� ����
#ifdef SERV_ROOM_TICK_DUMP
	CheckServerInfoToMail();
#endif SERV_ROOM_TICK_DUMP
	//}}
}

#endif SERV_ROOM_TICK_DUMP
//}}


void KMornitoringManager::Tick_LoginSImLayer()
{
	// 5�п� �ѹ��� üũ!
	if( m_tTimer.elapsed() < 300.0 )
		return;

	m_tTimer.restart();

	// ���� ����� SMS���ڰ� �ִ��� üũ!
	if( m_vecReservedSMSMessage.empty() == false )
	{
		BOOST_TEST_FOREACH( const KDBE_SEND_PHONE_MSG_NOT&, kPacket, m_vecReservedSMSMessage )
		{
			SendToSMSDB( DBE_SEND_PHONE_MSG_FOR_MORNITORING_REQ, kPacket );
		}

		m_vecReservedSMSMessage.clear();
	}

	// 5�� ������ ���� ����!
	{
		// ���� ����!
		KDBE_SEND_PHONE_MSG_NOT kPacketNot;
		if( CheckDisconnectAndGet5MinutesMessage_LoginServer( kPacketNot.m_wstrSMSMessage ) == true )
		{
			//{{ 2012. 11. 16	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
			SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_SERVER_STATE, kPacketNot.m_vecPhoneNum );
#else
			kPacketNot.m_vecPhoneNum = m_vecPhoneNumList;
#endif SERV_SMS_TOTAL_MANAGER
			//}}
			SendToSMSDB( DBE_SEND_PHONE_MSG_FOR_MORNITORING_REQ, kPacketNot );

			START_LOG( cout, L"5�� ���� ������ ���� ����͸� ����Ʈ SMS ����!" );
		}
	}

	// �Ϸ� ������ ���� ����!
	{
		// SMS���ڸ� ������ Ÿ�̹��� �Ǿ����� üũ!
		CTime tCurTime = CTime::GetCurrentTime();
		if( tCurTime < m_NextSendTime )
			return;

		// ���� �ð��� ����!
		m_NextSendTime += CTimeSpan( 1, 0, 0, 0	);
		std::wstring wstrNextSendTime = (const wchar_t*)m_NextSendTime.Format(_T("%Y-%m-%d %H:%M:%S"));

		// ���� ����!
		KDBE_SEND_PHONE_MSG_NOT kPacketNot;
		//{{ 2012. 11. 16	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
		SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_SERVER_STATE, kPacketNot.m_vecPhoneNum );
#else
		kPacketNot.m_vecPhoneNum = m_vecPhoneNumList;
#endif SERV_SMS_TOTAL_MANAGER
		//}}
		CheckDisconnectAndGetOneDayMessage_LoginServer( kPacketNot.m_wstrSMSMessage );
		SendToSMSDB( DBE_SEND_PHONE_MSG_FOR_MORNITORING_REQ, kPacketNot );

		START_LOG( cout, L"�Ϸ� ���� ������ ���� ����͸� ����Ʈ SMS ����!" )
			<< BUILD_LOG( wstrNextSendTime );
	}

	//{{ 2011. 03. 30	������	���� ���� ����
#ifdef SERV_ROOM_TICK_DUMP
	CheckServerInfoToMail();
#endif SERV_ROOM_TICK_DUMP
	//}}
}

void KMornitoringManager::CheckDisconnectAndGetMail_GameServer( OUT KEMAIL_SEND_EMAIL_NOT& kPacketNot )
{
	//////////////////////////////////////////////////////////////////////////
	// ���� ���� �ʱ�ȭ
	kPacketNot.m_wstrTitle = L"[���!] ������ ���� ���� �˸� ����!";
	kPacketNot.m_vecAttachFileNameList.push_back( std::wstring( L"Statistics\\SI_LOG_SERV_DISCONNECT.txt" ) );
	kPacketNot.m_vecRecvMailAddrList = m_vecServerNetworkMailList;

	CTime tCurTime = CTime::GetCurrentTime();
	CTime t5MinutesBeforeTime = tCurTime - CTimeSpan( 0, 0, 5, 0 );

	kPacketNot.m_wstrDesc += L"[";
	kPacketNot.m_wstrDesc += SEnum::GetServerGroupName( KBaseServer::GetKObj()->GetServerGroupID() );
	kPacketNot.m_wstrDesc += L"] ";
	kPacketNot.m_wstrDesc += (const wchar_t*)t5MinutesBeforeTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	kPacketNot.m_wstrDesc += L" ~ ";
	kPacketNot.m_wstrDesc += (const wchar_t*)tCurTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	kPacketNot.m_wstrDesc += L"\nIP : ";
	kPacketNot.m_wstrDesc += NetCommon::GetLocalIPW();
	kPacketNot.m_wstrDesc += L"\n[ ";

	std::map< ERR_TYPE, int >::iterator mit;
	for( mit = m_mapErrStatistics[EST_5_MINUTES].begin(); mit != m_mapErrStatistics[EST_5_MINUTES].end(); ++mit )
	{
		switch( mit->first )
		{
		case ET_DISCONNECT_GAME_CN_PROXY:
			{
				kPacketNot.m_wstrDesc += boost::str( boost::wformat( L"Center Server : %d  \n" ) % mit->second );
			}
			break;

		case ET_DISCONNECT_GAME_LOGIN_PROXY:
			{
				kPacketNot.m_wstrDesc += boost::str( boost::wformat( L"Global Server : %d  \n" ) % mit->second );
			}
			break;

		case ET_DISCONNECT_GAME_BILLING:
			{
				kPacketNot.m_wstrDesc += boost::str( boost::wformat( L"Nexon Billing Server : %d  \n" ) % mit->second );
			}
			break;

		case ET_DISCONNECT_LOGIN_AUTH:
			{
				// �α��� ���������� ���� �۾����� �ʾҽ��ϴ�.
				//wstrMessage += boost::str( boost::wformat( L"Auth : %d, " ) % mit->second );

				//{{ 2012. 01. 27  ��μ�	�α��� �������� ���� ���� �ڵ� sms ����
#ifdef SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS
				kPacketNot.m_wstrDesc += boost::str( boost::wformat( L"Nexon Auth Server : %d  \n" ) % mit->second );
#endif SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS
				//}}
			}
			break;

		case ET_DISCONNECT_DB_CONNECTION:
			{
				// DB�� ���� �۾����� �ʾҽ��ϴ�.
				//wstrMessage += boost::str( boost::wformat( L"DB : %d, " ) % mit->second );
			}
			break;
		}
	}

	kPacketNot.m_wstrDesc += L"]\n";
	kPacketNot.m_wstrDesc += L"������ ������ ���������ϴ�! �α� ���� �ٶ��ϴ�!";
}

bool KMornitoringManager::CheckDisconnectAndGet5MinutesMessage_LoginServer( OUT std::wstring& wstrMessage )
{
	wstrMessage.clear();

	// ��������� ���� ���� ����.
	if( m_mapErrStatistics[EST_5_MINUTES].empty() )
		return false;

	CTime tCurTime = CTime::GetCurrentTime();
	CTime t5MinutesBeforeTime = tCurTime - CTimeSpan( 0, 0, 5, 0 );

	wstrMessage += L"[";
	wstrMessage += SEnum::GetServerGroupName( KBaseServer::GetKObj()->GetServerGroupID() );
	wstrMessage += L"] ";
	wstrMessage += (const wchar_t*)t5MinutesBeforeTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	wstrMessage += L" ~ ";
	wstrMessage += (const wchar_t*)tCurTime.Format(_T("%Y-%m-%d %H:%M:%S"));

	int iTotalCount = 0;

	std::map< ERR_TYPE, int >::iterator mit;
	for( mit = m_mapErrStatistics[EST_5_MINUTES].begin(); mit != m_mapErrStatistics[EST_5_MINUTES].end(); ++mit )
	{
		iTotalCount += mit->second;

		switch( mit->first )
		{
		case ET_DISCONNECT_GAME_CN_PROXY:
			{
				wstrMessage += boost::str( boost::wformat( L"Cn:%d, " ) % mit->second );
			}
			break;

		case ET_DISCONNECT_GAME_LOGIN_PROXY:
			{
				wstrMessage += boost::str( boost::wformat( L"Lg:%d, " ) % mit->second );
			}
			break;

		case ET_DISCONNECT_GAME_BILLING:
			{
				wstrMessage += boost::str( boost::wformat( L"NB:%d, " ) % mit->second );
			}
			break;

		case ET_DISCONNECT_LOGIN_AUTH:
			{
				// �α��� ���������� ���� �۾����� �ʾҽ��ϴ�.
				//wstrMessage += boost::str( boost::wformat( L"Auth : %d, " ) % mit->second );

				//{{ 2012. 01. 27  ��μ�	�α��� �������� ���� ���� �ڵ� sms ����
#ifdef SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS
				wstrMessage += boost::str( boost::wformat( L"Auth : %d, " ) % mit->second );
#endif SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS
				//}}

			}
			break;

		case ET_DISCONNECT_DB_CONNECTION:
			{
				// DB�� ���� �۾����� �ʾҽ��ϴ�.
				//wstrMessage += boost::str( boost::wformat( L"DB : %d, " ) % mit->second );
			}
			break;
		}
	}

	wstrMessage += L"]";

	if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_NEXON_KOREA )
	{
		if( iTotalCount == 0 )
		{
			wstrMessage += L" �̻�!";
		}
		else
		{
			wstrMessage += L" ���!";
		}
	}
	else
	{
		wstrMessage += L" �׽�Ʈ!";
	}

	// ���ڿ� ���� ���Ŀ� ������ ���ϴ�!
	m_mapErrStatistics[EST_5_MINUTES].clear();
	return true;
}

void KMornitoringManager::CheckDisconnectAndGetOneDayMessage_LoginServer( OUT std::wstring& wstrMessage )
{
	wstrMessage.clear();

	CTime tCurTime = CTime::GetCurrentTime();
	CTime tYesterdayTime = tCurTime - CTimeSpan( 1, 0, 0, 0 );
	
	wstrMessage += L"[����][";
	wstrMessage += SEnum::GetServerGroupName( KBaseServer::GetKObj()->GetServerGroupID() );
	wstrMessage += L"] ";
	wstrMessage += (const wchar_t*)tYesterdayTime.Format(_T("%Y-%m-%d"));
	wstrMessage += L" 10��~";
	wstrMessage += (const wchar_t*)tCurTime.Format(_T("%Y-%m-%d"));
	wstrMessage += L" 10�� [";

	int iTotalCount = 0;

	std::map< ERR_TYPE, int >::iterator mit;
	for( mit = m_mapErrStatistics[EST_ONE_DAY].begin(); mit != m_mapErrStatistics[EST_ONE_DAY].end(); ++mit )
	{
		iTotalCount += mit->second;

		switch( mit->first )
		{
		case ET_DISCONNECT_GAME_CN_PROXY:
			{
				wstrMessage += boost::str( boost::wformat( L"Cn:%d, " ) % mit->second );
			}
			break;

		case ET_DISCONNECT_GAME_LOGIN_PROXY:
			{
				wstrMessage += boost::str( boost::wformat( L"Lg:%d, " ) % mit->second );
			}
			break;

		case ET_DISCONNECT_GAME_BILLING:
			{
				wstrMessage += boost::str( boost::wformat( L"NB:%d, " ) % mit->second );
			}
			break;

		case ET_DISCONNECT_LOGIN_AUTH:
			{
				// �α��� ���������� ���� �۾����� �ʾҽ��ϴ�.
				//wstrMessage += boost::str( boost::wformat( L"Auth : %d, " ) % mit->second );

				//{{ 2012. 01. 27  ��μ�	�α��� �������� ���� ���� �ڵ� sms ����
#ifdef SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS
				wstrMessage += boost::str( boost::wformat( L"Auth : %d, " ) % mit->second );
#endif SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS
				//}}
			}
			break;

		case ET_DISCONNECT_DB_CONNECTION:
			{
				// DB�� ���� �۾����� �ʾҽ��ϴ�.
				//wstrMessage += boost::str( boost::wformat( L"DB : %d, " ) % mit->second );
			}
			break;
		}

		// 0���� �ʱ�ȭ!
		mit->second = 0;
	}

	wstrMessage += L"]";

	if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_NEXON_KOREA )
	{
		if( iTotalCount == 0 )
		{
			wstrMessage += L" �̻�!";
		}
		else
		{
			wstrMessage += L" ���!";
		}
	}
	else
	{
		wstrMessage += L" �׽�Ʈ!";
	}
}

bool KMornitoringManager::IncreaseDisconnectCount( IN ERR_TYPE eType )
{
	{
		std::map< ERR_TYPE, int >::iterator mit;
		mit = m_mapErrStatistics[EST_5_MINUTES].find( eType );
		if( mit == m_mapErrStatistics[EST_5_MINUTES].end() )
		{
			m_mapErrStatistics[EST_5_MINUTES].insert( std::make_pair( eType, 1 ) );
		}
		else
		{
			// 5�� ���� ī��Ʈ ����
			++mit->second;
		}
	}	

	{
		std::map< ERR_TYPE, int >::iterator mit;
		mit = m_mapErrStatistics[EST_ONE_DAY].find( eType );
		if( mit == m_mapErrStatistics[EST_ONE_DAY].end() )
		{
			START_LOG( cerr, L"��� ���� ���� ���� ���� Ÿ�� �Դϴ�!" )
				<< BUILD_LOG( eType )
				<< END_LOG;
			return false;
		}

		// �Ϸ� ���� ī��Ʈ ����
		++mit->second;
		return true;
	}	
}

void KMornitoringManager::ReserveSMSMessage( IN const KDBE_SEND_PHONE_MSG_NOT& kNot )
{
    m_vecReservedSMSMessage.push_back( kNot );

	START_LOG( cout, L"SMS ���� ���� ����!" )
		<< BUILD_LOG( m_vecReservedSMSMessage.size() )
		<< BUILD_LOG( kNot.m_vecPhoneNum.size() )
		<< BUILD_LOG( kNot.m_wstrSMSMessage );
}

//{{ 2011. 03. 30	������	���� ���� ����
#ifdef SERV_ROOM_TICK_DUMP
void KMornitoringManager::CheckServerInfoToMail()
{
	if( m_setDumpServerUIDList.empty() )
		return;

	if( m_setDumpServerUIDList.find( KBaseServer::GetKObj()->GetUID() ) == m_setDumpServerUIDList.end() )
		return;
	
	CTime tCurTime = CTime::GetCurrentTime();
	if( tCurTime < m_tDumpSendTime )
		return;

	m_tDumpSendTime += CTimeSpan( 1, 0, 0, 0 );

	KEMAIL_SEND_EMAIL_NOT kPacketNot;
	
	// ���� ����!
	switch( KBaseServer::GetKObj()->GetServerClass() )
	{
	case KBaseServer::ESC_GAME:		kPacketNot.m_wstrTitle = L"[����] ���Ӽ��� ����!";		break;
	case KBaseServer::ESC_CENTER:	kPacketNot.m_wstrTitle = L"[����] ���ͼ��� ����!";		break;
	case KBaseServer::ESC_LOGIN:	kPacketNot.m_wstrTitle = L"[����] �α��μ��� ����!";	break;
		break;

	default:
		return;
	}

	// ServerInfo
	{
		kPacketNot.m_wstrDesc += L"\n\n-------------------------------- ServerInfo --------------------------------\n";
		std::wstringstream wstrMailMessage;
		KBaseServer::GetKObj()->ToString( wstrMailMessage );
		kPacketNot.m_wstrDesc += wstrMailMessage.str();
	}

	// SimLayer
	{
		kPacketNot.m_wstrDesc += L"\n\n-------------------------------- SimLayer --------------------------------\n";
		std::wstringstream wstrMailMessage;
		KSimLayer::GetKObj()->ToString( wstrMailMessage );
		kPacketNot.m_wstrDesc += wstrMailMessage.str();
	}
	
	// NetLayer
	{
		kPacketNot.m_wstrDesc += L"\n\n-------------------------------- NetLayer --------------------------------\n";
		std::wstringstream wstrMailMessage;
		KNetLayer::GetKObj()->ToString( wstrMailMessage );
		kPacketNot.m_wstrDesc += wstrMailMessage.str();
	}

	// DBLayer
	{
		kPacketNot.m_wstrDesc += L"\n\n-------------------------------- DBLayer --------------------------------\n";
		std::wstringstream wstrMailMessage;
		SiKDBLayer()->ToString( wstrMailMessage );
		kPacketNot.m_wstrDesc += wstrMailMessage.str();
	}

	// RoomManager
	if( KBaseServer::GetKObj()->GetServerClass() == KBaseServer::ESC_CENTER )
	{
		kPacketNot.m_wstrDesc += L"\n\n-------------------------------- RoomManager --------------------------------\n";
		std::wstringstream wstrMailMessage;
		KRoomManager::GetKObj()->ToString( wstrMailMessage );
		kPacketNot.m_wstrDesc += wstrMailMessage.str();
	}
	
	kPacketNot.m_vecRecvMailAddrList = m_vecServerDumpMailList;
	KBaseServer::GetKObj()->QueueingEventToMailManager( EMAIL_SEND_EMAIL_NOT, kPacketNot );
}
#endif SERV_ROOM_TICK_DUMP
//}}

void KMornitoringManager::SendToSMSDB( unsigned short usEventID )
{
	SendToSMSDB( usEventID, char() );
}

//{{ 2012. 03. 29	��μ�		x2.exe ��� ���� �˻�
#ifdef SERV_CHECK_X2_EXE_MODULE
void KMornitoringManager::GetCheckExistModule( std::map< std::wstring, bool >& mapModule )
{
	// �ʱ�ȭ �Ѵ�.
	std::map<std::wstring, bool>::iterator mit = m_mapExistModule.begin();
	for( ; mit != m_mapExistModule.end() ; ++mit )
	{
		mit->second = false;
	}

	mapModule = m_mapExistModule;
}

void KMornitoringManager::AddCheckModuleList_LUA( const char* pszModule )
{
	std::wstring wstrModule = KncUtil::toWideString( pszModule );
	MakeUpperCase( wstrModule );
	m_mapExistModule.insert( std::make_pair( wstrModule, false ) );
}

void KMornitoringManager::CheckModuleList_X2Module_LUA( bool bRet )
{
	m_bCheckExistModule = bRet;

	START_LOG( cout, L"Module Ȯ�� �÷��� ���� ����" )
		<< BUILD_LOG( m_bCheckExistModule );
}
#endif SERV_CHECK_X2_EXE_MODULE
//}}


//////////////////////////////////////////////////////////////////////////
#endif SERV_MORNITORING
//////////////////////////////////////////////////////////////////////////


