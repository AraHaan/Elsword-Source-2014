#include "stdafx.h"

#include "KEsdRemoteController_ConsoleImp.h"
#include "KPacketID.h"

#include "KEsdRemoteController_Console.h"

#include <MMSystem.h>
#include <map>
#include <fstream>
#include "KFolderTraverse.h"
#include <stdio.h>
#include <algorithm>
#include "FileCrypt.h"
using namespace std;

#define KDATA_1		0x57e9f10a
#define KDATA_2		0xf16cd864
#define KDATA_3		0x0ef9f7b1

KEsdRemoteController_ConsoleImp::KEsdRemoteController_ConsoleImp()
{
	m_authLevel = 0;
	m_iEsdServerPort = 6005;
	count = 0;
	m_bIsRefreshListBox = false;
}

KEsdRemoteController_ConsoleImp::~KEsdRemoteController_ConsoleImp()
{
	KFileDsnDataIterator mit = m_mapFileDsn.begin();
	for( ; mit != m_mapFileDsn.end(); mit++ )
	{
		KFileDsnData* pKFileDsnData = mit->second;
        SAFE_DELETE( pKFileDsnData );
	}
}

int KEsdRemoteController_ConsoleImp::DoQueryLogin( const std::wstring& wstrID, const std::wstring& wstrPassword )
{
	KFileDsnDataIterator mit;
	mit = m_mapFileDsn.find( m_iLoginAccountDsnID );
	if( mit == m_mapFileDsn.end() )
		return -1; // �α��� ���� ó��

	KFileDsnData* pFileDsnData = mit->second;

	std::wostringstream os;
	os << L"exec mup_login_mornitor_user_old \'" << wstrID << L"\', \'" << wstrPassword << L"\'";
	pFileDsnData->m_kQuery.Exec( os.str().c_str() );
	
	if( pFileDsnData->m_kQuery.Fetch() != SQL_SUCCESS )
	{
		pFileDsnData->m_kQuery.Clear();
		return -1;
	}

	int ret;
	pFileDsnData->m_kQuery.GetInt( 1, ret );
	if( ret == FALSE )
	{
		pFileDsnData->m_kQuery.Clear();
		return -1;
	}
	
	pFileDsnData->m_kQuery.GetInt( 2, m_authLevel );
	pFileDsnData->m_kQuery.Clear();
	return m_authLevel;
}

bool KEsdRemoteController_ConsoleImp::Initialize()
{	
	m_vecSocket.reserve(m_vecEsdServerInfo.size());
	m_vecSocket.resize(m_vecEsdServerInfo.size());

	//for ( unsigned int iS = 0; iS < m_vecEsdServerInfo.size(); ++iS ) 
	//{
	//	KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
	//	KTCPSocket& kSocket = m_vecSocket[iS];

	//	if ( kSocket.Connect( kInfo.m_strIP.c_str(), m_iEsdServerPort ) )
	//	{
	//		kInfo.m_iLastRecvTime = timeGetTime();
	//		kInfo.m_bConnected = true;
	//	}
	//}

	return true;
}

void KEsdRemoteController_ConsoleImp::OnConnect()
{
#ifndef COMPILE_CANT_CONNECT_SERVER
	for ( unsigned int iS = 0; iS < m_vecEsdServerInfo.size(); ++iS ) 
	{
		KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
		KTCPSocket& kSocket = m_vecSocket[iS];

		// �̹� ���ӵǾ� �ִٸ� ���
		if( kInfo.m_bConnected == true )
			continue;

		// üũ�ڽ��� üũ�Ǿ����� �ʴٸ� ���
		if ( kInfo.m_bChecked == false )
			continue;

		if ( kSocket.Connect( KncUtil::toNarrowString( kInfo.m_wstrIP ).c_str(), m_iEsdServerPort ) )
		{
			kInfo.m_iLastRecvTime = timeGetTime();
			kInfo.m_bConnected = true;
			//kInfo.m_bSelected = true; ���ӵǸ� ��� ���õȰŴ�!
		}
		else
		{
			kInfo.m_bConnected = false;
		}
	}
#endif COMPILE_CANT_CONNECT_SERVER
}

void KEsdRemoteController_ConsoleImp::Destroy()
{
	for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
	{
		KTCPSocket& kSocket = m_vecSocket[iS];
		kSocket.CloseSocket();
	}
}

void KEsdRemoteController_ConsoleImp::Update()
{
	UpdateRecv();
}

void KEsdRemoteController_ConsoleImp::LoadConfig()
{
	lua_tinker::class_add<KEsdRemoteController_ConsoleImp>( g_pLua, "KEsdRemoteController_ConsoleImp" );
	lua_tinker::class_def<KEsdRemoteController_ConsoleImp>( g_pLua, "InitMornitoringToolOption",	&KEsdRemoteController_ConsoleImp::InitMornitoringToolOption_Lua );
	lua_tinker::class_def<KEsdRemoteController_ConsoleImp>( g_pLua, "AddFileDsn",					&KEsdRemoteController_ConsoleImp::AddFileDsn_Lua );
	lua_tinker::class_def<KEsdRemoteController_ConsoleImp>( g_pLua, "AddServerGroupInfo",			&KEsdRemoteController_ConsoleImp::AddServerGroupInfo_Lua );
	lua_tinker::class_def<KEsdRemoteController_ConsoleImp>( g_pLua, "AddServerInfo",				&KEsdRemoteController_ConsoleImp::AddServerInfo_Lua );
	lua_tinker::class_def<KEsdRemoteController_ConsoleImp>( g_pLua, "AddLineColor",					&KEsdRemoteController_ConsoleImp::AddLineColor_Lua );

	lua_tinker::decl( g_pLua, "ConsoleImp", this );

	std::string strPath = KGetCurFolder();
	strPath += "\\";
	strPath += "Config.lua";

	if( 0 != luaL_dofile( g_pLua, strPath.c_str() ) )
	{
		KDbgStm << "Config.lua open failed.!";
	}
}

int KEsdRemoteController_ConsoleImp::AuthenticateServer( const std::wstring& wstrIPAddr, int iFileDsnID )
{
	// FileDsnID�� 0���̸� �������� �ȹް� �׷��� ���� �ȹްڴٴ� �ǹ̷� ServerUID�� -1���� �Ѱ���
	if( iFileDsnID == 0 )
		return -1;

	KFileDsnDataIterator mit;
	mit = m_mapFileDsn.find( iFileDsnID );
	if( mit == m_mapFileDsn.end() )
		return -1; // file dsn ����

	KFileDsnData* pFileDsnData = mit->second;

	std::wostringstream os;
	os << L"exec lup_server_auth_for_ccu \'"<< wstrIPAddr << L"\'";
	pFileDsnData->m_kQuery.Exec( os.str().c_str() );

	if( pFileDsnData->m_kQuery.Fetch() != SQL_SUCCESS )
	{
		pFileDsnData->m_kQuery.Clear();
		return -1;
	}

	int serverUID;
	pFileDsnData->m_kQuery.GetInt( 1, serverUID );
	if( serverUID == -1 )
	{
		pFileDsnData->m_kQuery.Clear();
		return -1;
	}

	m_vecAuthServer.push_back( serverUID );
	pFileDsnData->m_kQuery.Clear();
	return serverUID;
}

void KEsdRemoteController_ConsoleImp::UpdateRecv()
{
	for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
	{
		KTCPSocket& kSocket = m_vecSocket[iS];
		if ( Select( kSocket.GetSocket() ) == true )
		{
			char acTempBuffer[1024];
			int iReadSize = kSocket.Recv( acTempBuffer, 1024 );

			if( iReadSize == SOCKET_ERROR )
			{
				int iNetError = ::WSAGetLastError();
				m_vecEsdServerInfo[iS].m_bConnected		 = false;
				m_vecEsdServerInfo[iS].m_bOnlineLgServer = false;
				m_vecEsdServerInfo[iS].m_bOnlineCnServer = false;
				m_vecEsdServerInfo[iS].m_bOnlineGmServer = false;
				m_vecEsdServerInfo[iS].m_bOnlineChServer = false;
				kSocket.CloseSocket();
			}
			// ���� ����
			else if( iReadSize == 0 )
			{
				m_vecEsdServerInfo[iS].m_bConnected		 = false;
				m_vecEsdServerInfo[iS].m_bOnlineLgServer = false;
				m_vecEsdServerInfo[iS].m_bOnlineCnServer = false;
				m_vecEsdServerInfo[iS].m_bOnlineGmServer = false;
				m_vecEsdServerInfo[iS].m_bOnlineChServer = false;
				kSocket.CloseSocket();
			}
			// ��Ŷ ����
			else if ( iReadSize > 0 )
			{
				KPacket kPacket;
				kPacket.SetBuffer(acTempBuffer, iReadSize);
                OnRecv( kPacket );

				// -�� - .. �� �Ƚ��ڵ�..
				if( iReadSize > kPacket.GetSize() )
				{
					KPacket kPacketTemp;
					kPacketTemp.SetBuffer( acTempBuffer+kPacket.GetSize(), iReadSize - kPacket.GetSize() );
					OnRecv( kPacketTemp );
				}
			}

			// ����Ʈ �ڽ� ���� ����
			m_bIsRefreshListBox = true;
		}
	}
}

void KEsdRemoteController_ConsoleImp::OnRecv( KPacket& kPacket )
{	
	std::wstring wstrIP;
	std::string strIP;

	int iPacketID = kPacket.GetID();
	switch ( iPacketID )
	{
	//case eESD_PID_SC_VERIFY_ACCOUNT:
	//	// ���� ����
	//	break;

	case eESD_PID_SC_STATUS:
		OnEsdStatus(kPacket);
		break;

	case eESD_PID_SC_LG_CRASH:
		kPacket >> strIP;
		wstrIP = KncUtil::toWideString( strIP );
		for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
		{
			KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
			if ( kInfo.m_wstrIP == wstrIP )
			{
				ServerCrash( kInfo.m_wstrName, SERVER_STATE::Login_Server_Crash );
				kInfo.m_bCrashLgServer = true;
				break;
			}				
		}
		break;

	case eESD_PID_SC_CN_CRASH:
		kPacket >> strIP;
		wstrIP = KncUtil::toWideString( strIP );
		for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
		{
			KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
			if ( kInfo.m_wstrIP == wstrIP )
			{
				ServerCrash( kInfo.m_wstrName, SERVER_STATE::Center_Server_Crash );
				kInfo.m_bCrashCnServer = true;
				break;
			}
		}
		break;

	case eESD_PID_SC_GS_CRASH:
		kPacket >> strIP;
		wstrIP = KncUtil::toWideString( strIP );
		for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
		{
			KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
			if ( kInfo.m_wstrIP == wstrIP )
			{
				ServerCrash( kInfo.m_wstrName, SERVER_STATE::Game_Server_Crash );
				kInfo.m_bCrashGmServer = true;
				break;
			}
		}
		break;

	case eESD_PID_SC_CH_CRASH:
		kPacket >> strIP;
		wstrIP = KncUtil::toWideString( strIP );
		for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
		{
			KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
			if ( kInfo.m_wstrIP == wstrIP )
			{
				ServerCrash( kInfo.m_wstrName, SERVER_STATE::Channel_Server_Crash );
				kInfo.m_bCrashChServer = true;
				break;
			}
		}
		break;

	default:
		break;
	}
}

bool KEsdRemoteController_ConsoleImp::Select( SOCKET kSocket )
{
	fd_set fdsetSelect;
	FD_ZERO(&fdsetSelect);

#pragma warning( push )
	// The implementation of FD_SET use a do { ... }while (0) and thus fire this warning
#pragma warning( disable : 4127 ) // conditional expression is constant
	if ( INVALID_SOCKET != kSocket )
	{
		FD_SET( kSocket, &fdsetSelect );
	}
#pragma warning( pop )

	// Specify a timeval of 0 sec 0 microsecond. This will make it a non-blocking call
	timeval timevalTimeOut;
	timevalTimeOut.tv_sec = 0l;
	timevalTimeOut.tv_usec = 0l;

	int iSelectRetVal = select( 0, &fdsetSelect, NULL, NULL, &timevalTimeOut );
	if ( SOCKET_ERROR == iSelectRetVal )
	{
		//KDbgStm << "Select ����!" << trace;
		return false;
	}

	// No connection waiting to be Selected so continue
	if ( 0 == iSelectRetVal )
	{
		return false;
	}

	// if the listening socket is invalid it means that we are not Selecting connection and should only check to see 
	// if the outgoing socket was reset/closed
	if ( INVALID_SOCKET == kSocket )
	{
		return false;
	}

	return true;
}

void KEsdRemoteController_ConsoleImp::OnEsdLaunch()
{
#ifndef COMPILE_CANT_CONTROL_SERVER
	for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
	{
		KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
		KTCPSocket& kSocket = m_vecSocket[iS];

		if ( kInfo.m_bChecked == true  &&  kInfo.m_bConnected == true )
		{
			KPacket kPacket( eESD_PID_CS_LAUNCH );
			kSocket.Send( kPacket.GetBuffer(), kPacket.GetSize() );
		}
	}
#endif
}

void KEsdRemoteController_ConsoleImp::OnEsdLaunch_CO()
{
#ifndef COMPILE_CANT_CONTROL_SERVER
	for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
	{
		KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
		KTCPSocket& kSocket = m_vecSocket[iS];

		if ( kInfo.m_bChecked == true  &&  kInfo.m_bConnected == true )
		{
			KPacket kPacket( eESD_PID_CS_LAUNCH_CO );
			kSocket.Send( kPacket.GetBuffer(), kPacket.GetSize() );
		}
	}
#endif
}

void KEsdRemoteController_ConsoleImp::OnEsdLaunch_LG()
{
#ifndef COMPILE_CANT_CONTROL_SERVER
	for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
	{
		KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
		KTCPSocket& kSocket = m_vecSocket[iS];

		if ( kInfo.m_bChecked == true  &&  kInfo.m_bConnected == true )
		{
			KPacket kPacket( eESD_PID_CS_LAUNCH_LG );
			kSocket.Send( kPacket.GetBuffer(), kPacket.GetSize() );
		}
	}
#endif
}

void KEsdRemoteController_ConsoleImp::OnEsdLaunch_CH()
{
#ifndef COMPILE_CANT_CONTROL_SERVER
	for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
	{
		KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
		KTCPSocket& kSocket = m_vecSocket[iS];

		if ( kInfo.m_bChecked == true  &&  kInfo.m_bConnected == true )
		{
			KPacket kPacket( eESD_PID_CS_LAUNCH_CH );
			kSocket.Send( kPacket.GetBuffer(), kPacket.GetSize() );
		}
	}
#endif
}

void KEsdRemoteController_ConsoleImp::OnEsdWindowsUpdate()
{
#ifndef COMPILE_CANT_CONTROL_SERVER
	for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
	{
		KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
		KTCPSocket& kSocket = m_vecSocket[iS];

		if ( kInfo.m_bChecked == true  &&  kInfo.m_bConnected == true )
		{
			KPacket kPacket( eESD_PID_CS_WINDOWS_UPDATE );
			kSocket.Send( kPacket.GetBuffer(), kPacket.GetSize() );
		}
	}
#endif
}

void KEsdRemoteController_ConsoleImp::OnEsdSVNUpdate()
{
#ifndef COMPILE_CANT_CONTROL_SERVER
	for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
	{
		KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
		KTCPSocket& kSocket = m_vecSocket[iS];

		if ( kInfo.m_bChecked == true  &&  kInfo.m_bConnected == true )
		{
			KPacket kPacket( eESD_PID_CS_SVN_UPDATE );
			kSocket.Send( kPacket.GetBuffer(), kPacket.GetSize() );
		}
	}
#endif
}

void KEsdRemoteController_ConsoleImp::OnEsdHSBUpdate()
{
#ifndef COMPILE_CANT_HSB_UPDATE
	for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
	{
		KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
		KTCPSocket& kSocket = m_vecSocket[iS];

		if ( kInfo.m_bChecked == true  &&  kInfo.m_bConnected == true )
		{
			KPacket kPacket( eESD_PID_CS_HSB_UPDATE );
			kSocket.Send( kPacket.GetBuffer(), kPacket.GetSize() );
		}
	}
#endif
}

void KEsdRemoteController_ConsoleImp::OnEsdVirusCheck()
{
#ifndef COMPILE_CANT_CONTROL_SERVER
	for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
	{
		KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
		KTCPSocket& kSocket = m_vecSocket[iS];

		if ( kInfo.m_bChecked == true  &&  kInfo.m_bConnected == true )
		{
			KPacket kPacket( eESD_PID_CS_VIRUS_CHECK );
			kSocket.Send( kPacket.GetBuffer(), kPacket.GetSize() );
		}
	}
#endif
}

void KEsdRemoteController_ConsoleImp::OnEsdDBConnectionIDPW( const std::string strID, const std::string strPW )
{
#ifndef COMPILE_CANT_CONTROL_SERVER
	for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
	{
		KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
		KTCPSocket& kSocket = m_vecSocket[iS];

		if ( kInfo.m_bChecked == true  &&  kInfo.m_bConnected == true )
		{
			KPacket kPacket( eESD_PID_CS_DBCONNECTION_IDPW );			
			kPacket << strID;
			kPacket << strPW;

			kSocket.Send( kPacket.GetBuffer(), kPacket.GetSize() );
		}
	}
#endif
}

void KEsdRemoteController_ConsoleImp::OnEsdShutdown()
{
#ifndef COMPILE_CANT_CONTROL_SERVER
	for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
	{
		KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
		KTCPSocket& kSocket = m_vecSocket[iS];

		if ( kInfo.m_bChecked == true  &&  kInfo.m_bConnected == true )
		{
			KPacket kPacket( eESD_PID_CS_SHUTDOWN );
			kPacket << static_cast<int>(eESD_PID_SHUTDOWN_CODE); // �˴ٿ� �ڵ带 ��´�.

			kSocket.Send( kPacket.GetBuffer(), kPacket.GetSize() );
		}
	}
#endif
}

void KEsdRemoteController_ConsoleImp::OnEsdStatus( KPacket& kPacket )
{
	char buf[128] = {0};
	std::string strIP;
	std::wstring wstrIP;
	HWND hLoginServer;
	HWND hCenterServer;
	HWND hGameServer;
	HWND hChannelServer;

	kPacket >> strIP >> hLoginServer >> hCenterServer >> hGameServer >> hChannelServer;

	wstrIP = KncUtil::toWideString( strIP );

	for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
	{
		KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
		if ( kInfo.m_wstrIP == wstrIP )
		{
			kInfo.m_iLastRecvTime = timeGetTime();

			kInfo.m_bOnlineLgServer = ( hLoginServer != NULL );
			kInfo.m_bOnlineCnServer = ( hCenterServer != NULL );
			kInfo.m_bOnlineGmServer = ( hGameServer != NULL );
			kInfo.m_bOnlineChServer = ( hChannelServer != NULL );

			if( kInfo.m_bConnected )
			{
				// �α��� ���� ũ���� ����
				if( kInfo.m_bCrashLgServer == true && kInfo.m_bOnlineLgServer == true )
				{
					kInfo.m_bCrashLgServer = false;
					ServerRestore( kInfo.m_wstrName, SERVER_STATE::Login_Server_Restore );
				}

				// ���� ���� ũ���� ����
				if( kInfo.m_bCrashCnServer == true && kInfo.m_bOnlineCnServer == true )
				{
					kInfo.m_bCrashCnServer = false;
					ServerRestore( kInfo.m_wstrName, SERVER_STATE::Center_Server_Restore );
				}

				// ���� ���� ũ���� ����
				if( kInfo.m_bCrashGmServer == true && kInfo.m_bOnlineGmServer == true )
				{
					kInfo.m_bCrashGmServer = false;
					ServerRestore( kInfo.m_wstrName, SERVER_STATE::Game_Server_Restore );
				}

				// ä�� ���� ũ���� ����
				if( kInfo.m_bCrashChServer == true && kInfo.m_bOnlineChServer == true )
				{
					kInfo.m_bCrashChServer = false;
					ServerRestore( kInfo.m_wstrName, SERVER_STATE::Channel_Server_Restore );
				}
			}

			kInfo.m_bBeforeLgState = kInfo.m_bOnlineLgServer;
			kInfo.m_bBeforeCnState = kInfo.m_bOnlineCnServer;
			kInfo.m_bBeforeGmState = kInfo.m_bOnlineGmServer;
			kInfo.m_bBeforeChState = kInfo.m_bOnlineChServer;
		}
	}	
}

int KEsdRemoteController_ConsoleImp::GetEsdServerInfoCount()
{
	return (int) m_vecEsdServerInfo.size();
}

KESDServerInfo* KEsdRemoteController_ConsoleImp::GetEsdServerInfoAt( int iServerGroupID, int iIdx )
{
	if( iServerGroupID == 0 )
	{
		return &m_vecEsdServerInfo[iIdx];
	}
	else
	{
		std::vector< KESDServerInfo >::iterator vit = m_vecEsdServerInfo.begin();
		for( int iIndexCnt = 0; vit != m_vecEsdServerInfo.end(); vit++ )
		{
			KESDServerInfo& info = *vit;
			if( info.m_iServerGroupID == iServerGroupID )
			{
				if( iIndexCnt == iIdx )
					return &info;

				++iIndexCnt;
			}
		}
	}
	return NULL;
}

void KEsdRemoteController_ConsoleImp::SetEsdServerSelectClear()
{
	for ( unsigned int iS = 0; iS < m_vecSocket.size(); ++iS )
	{
		KESDServerInfo& kInfo = m_vecEsdServerInfo[iS];
		kInfo.m_bChecked = false;
	}	
}

bool KEsdRemoteController_ConsoleImp::GetTodayConcurrentUserFromDB( int iServerUID, int iFileDsnID, std::vector< CCUInfo >& vecTodayCCU, int& iCCU )
{			
	return GetConcurrentUserFromDB( CCU_SP::TODAY_CCU, iServerUID, iFileDsnID, vecTodayCCU, iCCU );
}

bool KEsdRemoteController_ConsoleImp::GetYesterdayConcurrentUserFromDB( int iServerUID, int iFileDsnID, std::vector< CCUInfo >& vecYesterdayCCU, int& iCCU )
{
	return GetConcurrentUserFromDB( CCU_SP::YESTERDAY_CCU, iServerUID, iFileDsnID, vecYesterdayCCU, iCCU );
}

bool KEsdRemoteController_ConsoleImp::GetGuestIDConcurrentUserFromDB( int iServerUID, int iFileDsnID, std::vector< CCUInfo >& vecGuestUserCCU, int& iCCU )
{
	return GetConcurrentUserFromDB( CCU_SP::GUESTID_CCU, iServerUID, iFileDsnID, vecGuestUserCCU, iCCU );
}

bool KEsdRemoteController_ConsoleImp::GetConcurrentUserFromDB( CCU_SP eState, int iServerUID, int iFileDsnID, std::vector< CCUInfo >& vecCCU, int& iCCU )
{
	KFileDsnDataIterator mit;
	mit = m_mapFileDsn.find( iFileDsnID );
	if( mit == m_mapFileDsn.end() )
		return false; // ���� ó��

	KFileDsnData* pFileDsnData = mit->second;
	
	// �����ڼ� �α� ���
	std::wostringstream os;
	switch( eState )
	{
	case CCU_SP::TODAY_CCU:
		os << L"exec lup_get_today_ccu " << iServerUID;
		break;

	case CCU_SP::YESTERDAY_CCU:
		os << L"exec lup_get_yesterday_ccu " << iServerUID;
		break;

	case CCU_SP::GUESTID_CCU:
		os << L"exec lup_get_today_ccu_guestuser"; // ����UID �ȳ־ �ǳ�?
		break;
	}
	pFileDsnData->m_kQuery.Exec( os.str().c_str() );
	
	// �׷����� �׸� �ڷ� ����
	vecCCU.clear();
	WORD oldHour = 0, oldMinute = 0, gapMinute = 0;
	OldTime oldTime;

	while( pFileDsnData->m_kQuery.Fetch() == SQL_SUCCESS )
	{
		CCUInfo ccu;
		pFileDsnData->m_kQuery.GetDate( "RegDate", &ccu.m_date );
		pFileDsnData->m_kQuery.GetInt( "ServerUID", ccu.m_serverUID );
		pFileDsnData->m_kQuery.GetInt( "CCU", ccu.m_CCUcount );

		// Log DB�� ��ϵ��� ���� �κ� �˻�
		gapMinute = ( ( ccu.m_date.wHour - oldTime.m_iOldHour ) * 60 ) + ( ccu.m_date.wMinute - oldTime.m_iOldMinute );

		// ������ �κ��� ä������ ���� ������
		CCUInfo dummyCcu;
		dummyCcu.m_serverUID = ccu.m_serverUID;
		dummyCcu.m_date		 = ccu.m_date;
		dummyCcu.m_CCUcount  = 0;

		// Log DB�� ������ ���������� ä���ֱ�
		for( ; gapMinute > 1; gapMinute-- )
		{
			vecCCU.push_back( dummyCcu );
		}

		oldTime.m_iOldHour   = ccu.m_date.wHour;
		oldTime.m_iOldMinute = ccu.m_date.wMinute;

		vecCCU.push_back( ccu );
	}

	pFileDsnData->m_kQuery.Clear();
	return true;
}

bool KEsdRemoteController_ConsoleImp::GetDateConcurrentUserFromDB( IN int iServerUID, IN std::wstring wstrDate, IN int iFileDsnID, OUT std::vector< CCUInfo >& vecCCU, OUT int& iCCU )
{
	KFileDsnDataIterator mit;
	mit = m_mapFileDsn.find( iFileDsnID );
	if( mit == m_mapFileDsn.end() )
		return false; // ���� ó��

	KFileDsnData* pFileDsnData = mit->second;

	// �����ڼ� �α� ���
	std::wostringstream os;

#ifndef COMPILE_CANT_DATE_INFO
	os << L"exec lup_get_date_ccu " << iServerUID << L", \'" << wstrDate << L"\'";
#endif
	
	pFileDsnData->m_kQuery.Exec( os.str().c_str() );

	// �׷����� �׸� �ڷ� ����
	vecCCU.clear();
	WORD oldHour = 0, oldMinute = 0, gapMinute = 0;
	OldTime oldTime;

	while( pFileDsnData->m_kQuery.Fetch() == SQL_SUCCESS )
	{
		CCUInfo ccu;
		pFileDsnData->m_kQuery.GetDate( "RegDate", &ccu.m_date );
		pFileDsnData->m_kQuery.GetInt( "ServerUID", ccu.m_serverUID );
		pFileDsnData->m_kQuery.GetInt( "CCU", ccu.m_CCUcount );

		// Log DB�� ��ϵ��� ���� �κ� �˻�
		gapMinute = ( ( ccu.m_date.wHour - oldTime.m_iOldHour ) * 60 ) + ( ccu.m_date.wMinute - oldTime.m_iOldMinute );

		// ������ �κ��� ä������ ���� ������
		CCUInfo dummyCcu;
		dummyCcu.m_serverUID = ccu.m_serverUID;
		dummyCcu.m_date		 = ccu.m_date;
		dummyCcu.m_CCUcount  = 0;

		// Log DB�� ������ ���������� ä���ֱ�
		for( ; gapMinute > 1; gapMinute-- )
		{
			vecCCU.push_back( dummyCcu );
		}

		oldTime.m_iOldHour   = ccu.m_date.wHour;
		oldTime.m_iOldMinute = ccu.m_date.wMinute;

		vecCCU.push_back( ccu );
	}

	pFileDsnData->m_kQuery.Clear();
	return true;
}

bool KEsdRemoteController_ConsoleImp::GetTimeConcurrentUserFromDB( IN int iServerUID, IN std::wstring wstrDate, IN int iFileDsnID, OUT std::vector< CCUInfo >& vecCCU, OUT int& iCCU )
{
	KFileDsnDataIterator mit;
	mit = m_mapFileDsn.find( iFileDsnID );
	if( mit == m_mapFileDsn.end() )
		return false; // ���� ó��

	KFileDsnData* pFileDsnData = mit->second;

	// �����ڼ� �α� ���
	std::wostringstream os;
	os << L"exec lup_get_nowtime_ccu " << iServerUID << L", \'" << wstrDate << L"\'";

	pFileDsnData->m_kQuery.Exec( os.str().c_str() );

	// �׷����� �׸� �ڷ� ����
	vecCCU.clear();
	WORD oldHour = 0, oldMinute = 0, gapMinute = 0;
	bool bIsFirst = true;
	OldTime oldTime;

	while( pFileDsnData->m_kQuery.Fetch() == SQL_SUCCESS )
	{
		CCUInfo ccu;
		pFileDsnData->m_kQuery.GetDate( "RegDate", &ccu.m_date );
		pFileDsnData->m_kQuery.GetInt( "ServerUID", ccu.m_serverUID );
		pFileDsnData->m_kQuery.GetInt( "CCU", ccu.m_CCUcount );

		// Log DB�� ��ϵ��� ���� �κ� �˻�
		gapMinute = ( ( ccu.m_date.wHour - oldTime.m_iOldHour ) * 60 ) + ( ccu.m_date.wMinute - oldTime.m_iOldMinute );

		if( bIsFirst )
		{
			gapMinute = 0;
			bIsFirst = false;
		}

		// ������ �κ��� ä������ ���� ������
		CCUInfo dummyCcu;
		dummyCcu.m_serverUID = ccu.m_serverUID;
		dummyCcu.m_date		 = ccu.m_date;
		dummyCcu.m_CCUcount  = 0;

		// Log DB�� ������ ���������� ä���ֱ�
		for( ; gapMinute > 1; gapMinute-- )
		{
			vecCCU.push_back( dummyCcu );
		}

		oldTime.m_iOldHour   = ccu.m_date.wHour;
		oldTime.m_iOldMinute = ccu.m_date.wMinute;

		vecCCU.push_back( ccu );
	}

	pFileDsnData->m_kQuery.Clear();
	return true;
}

bool KEsdRemoteController_ConsoleImp::GetDateStatisticsFromDB( IN std::wstring wstrDate, IN int iFileDsnID, OUT KESDDateStatistics& dateStatistics )
{
	KFileDsnDataIterator mit;
	mit = m_mapFileDsn.find( iFileDsnID );
	if( mit == m_mapFileDsn.end() )
		return false; // ���� ó��

	KFileDsnData* pFileDsnData = mit->second;

	// ���� ��� ���
	std::wostringstream os;

#ifndef COMPILE_CANT_DATE_INFO
	os << L"exec statsDayUserInfo \'" << wstrDate << L"\'";
#endif

	pFileDsnData->m_kQuery.Exec( os.str().c_str() );	

	if( pFileDsnData->m_kQuery.Fetch() == SQL_SUCCESS )
	{
		dateStatistics.clear(); // �ʱ�ȭ
		dateStatistics.m_iDate.second = wstrDate;
		pFileDsnData->m_kQuery.GetInt(  1, dateStatistics.m_iTopCCU.first );
		pFileDsnData->m_kQuery.GetDate( 2, &dateStatistics.m_iTopCCUTime.first );
		pFileDsnData->m_kQuery.GetInt(  3, dateStatistics.m_iDayAvgCCU.first );
		pFileDsnData->m_kQuery.GetInt(  4, dateStatistics.m_iWeekAvgCCU.first );
		pFileDsnData->m_kQuery.GetInt(  5, dateStatistics.m_iMonthAvgCCU.first );
		pFileDsnData->m_kQuery.GetInt(  6, dateStatistics.m_iJoinUser.first );
		pFileDsnData->m_kQuery.GetInt(  7, dateStatistics.m_iUnicUser.first );
		pFileDsnData->m_kQuery.GetInt(  8, dateStatistics.m_iTotalUnicUser.first );

		dateStatistics.m_iTopCCU.second			= BOOST_FORMAT( ( L"%d" ) % dateStatistics.m_iTopCCU.first );
		dateStatistics.m_iTopCCUTime.second		= BOOST_FORMAT( ( L"%d�� %d��")
			% dateStatistics.m_iTopCCUTime.first.wHour
			% dateStatistics.m_iTopCCUTime.first.wMinute
			);
		dateStatistics.m_iDayAvgCCU.second		= BOOST_FORMAT( ( L"%d" ) % dateStatistics.m_iDayAvgCCU.first );
		dateStatistics.m_iWeekAvgCCU.second		= BOOST_FORMAT( ( L"%d" ) % dateStatistics.m_iWeekAvgCCU.first );
		dateStatistics.m_iMonthAvgCCU.second	= BOOST_FORMAT( ( L"%d" ) % dateStatistics.m_iMonthAvgCCU.first );
		dateStatistics.m_iJoinUser.second		= BOOST_FORMAT( ( L"%d" ) % dateStatistics.m_iJoinUser.first );
		dateStatistics.m_iUnicUser.second		= BOOST_FORMAT( ( L"%d" ) % dateStatistics.m_iUnicUser.first );
		dateStatistics.m_iTotalUnicUser.second	= BOOST_FORMAT( ( L"%d" ) % dateStatistics.m_iTotalUnicUser.first );
	}

	pFileDsnData->m_kQuery.Clear();
	return true;
}

COLORREF KEsdRemoteController_ConsoleImp::GetLineColor( int iLineColorID )
{
	std::map< int, COLORREF >::iterator mit = m_mapLineColor.find( iLineColorID );
	if( mit == m_mapLineColor.end() )
		return RGB( 255, 255, 255 );

	return mit->second;
}

static std::wstring g_serverName;

DWORD WINAPI SirenCallbackFunction( LPVOID lpArg )
{
	HANDLE hEvent = reinterpret_cast<HANDLE>(lpArg);

	while(1)
	{
		if( WaitForSingleObject( hEvent, 3000 ) == 0 )
			break;

		::PlaySound( L"Siren.wav", NULL, SND_FILENAME | SND_ASYNC );
	}

	return 0;
}

DWORD WINAPI MessageBoxCallbackFunction( LPVOID lpArg )
{
	int servStat = reinterpret_cast<int>(lpArg);
	std::wstring errMessage;
	HANDLE hThread = NULL;
	HANDLE hEvent = NULL;
	bool bCrash = false;

	errMessage = g_serverName;

	switch( servStat )
	{
	case KEsdRemoteController_ConsoleImp::SERVER_STATE::Login_Server_Crash:
		errMessage += L" [�α��� ����]�� �ٿ�Ǿ����ϴ�!!!";
		bCrash = true;		
		break;

	case KEsdRemoteController_ConsoleImp::SERVER_STATE::Center_Server_Crash:
		errMessage += L" [���� ����]�� �ٿ�Ǿ����ϴ�!!!";
		bCrash = true;
		break;

	case KEsdRemoteController_ConsoleImp::SERVER_STATE::Game_Server_Crash:
		errMessage += L" [���� ����]�� �ٿ�Ǿ����ϴ�!!!";
		bCrash = true;
		break;

	case KEsdRemoteController_ConsoleImp::SERVER_STATE::Channel_Server_Crash:
		errMessage += L" [ä�� ����]�� �ٿ�Ǿ����ϴ�!!!";
		bCrash = true;
		break;

	case KEsdRemoteController_ConsoleImp::SERVER_STATE::Login_Server_Restore:
		errMessage += L" [�α��� ����]�� �����Ǿ����ϴ�!!!";
		break;

	case KEsdRemoteController_ConsoleImp::SERVER_STATE::Center_Server_Restore:
		errMessage += L" [���� ����]�� �����Ǿ����ϴ�!!!";
		break;

	case KEsdRemoteController_ConsoleImp::SERVER_STATE::Game_Server_Restore:
		errMessage += L" [���� ����]�� �����Ǿ����ϴ�!!!";
		break;

	case KEsdRemoteController_ConsoleImp::SERVER_STATE::Channel_Server_Restore:
		errMessage += L" [ä�� ����]�� �����Ǿ����ϴ�!!!";
		break;

	default:
		errMessage = L" �߸��� �������°��Դϴ�!";
	}

	if( bCrash == true )
	{
		hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
		hThread = CreateThread( NULL, NULL, SirenCallbackFunction, reinterpret_cast<LPVOID>(hEvent), NULL, NULL );
	}	
	
	::MessageBox( NULL, errMessage.c_str(), L"!!! �˸� �޽��� !!!", MB_ICONWARNING );

	if( bCrash == true )
	{
		SetEvent( hEvent );
	}

	return 0;
}

void KEsdRemoteController_ConsoleImp::ServerCrash( std::wstring& serverName, SERVER_STATE serverState )
{
	g_serverName.clear();
	g_serverName = serverName;

	CreateThread( NULL, NULL, MessageBoxCallbackFunction, reinterpret_cast<LPVOID>(serverState), NULL, NULL );
}

void KEsdRemoteController_ConsoleImp::ServerRestore( std::wstring& serverName, SERVER_STATE serverState )
{
	g_serverName.clear();
	g_serverName = serverName;

	CreateThread( NULL, NULL, MessageBoxCallbackFunction, reinterpret_cast<LPVOID>(serverState), NULL, NULL );
}

bool KEsdRemoteController_ConsoleImp::InitMornitoringToolOption_Lua() 
{
	KLuaManager luaManager( g_pLua );

	int iMinute;

	LUA_GET_VALUE_RETURN(	luaManager,	"m_iLoginAccountDsnID",		m_iLoginAccountDsnID,		0,	goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager,	"m_iCCUGraphMaxValue",		m_iMaxValue,				0,	goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager,	"m_iGraphRefreshMinute",	iMinute,					0,	goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager,	"m_iTodayLineColor",		m_iTotalCCUTodayCID,		0,	goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager,	"m_iYesterdayLineColor",	m_iTotalCCUYesCID,			0,	goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager,	"m_iAverageLineColor",		m_iAverageCCUColorID,		0,	goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager,	"m_iGuestUserLineColor",	m_iGuestUserLineColorID,	0,	goto LoadFail; );

	m_iGraphRefreshTime = iMinute * 60000;

	return true;

LoadFail:
	KDbgStm << "Concurrent User Graph Max Value �Ľ� ����" << trace;

	return false;
}

bool KEsdRemoteController_ConsoleImp::AddServerGroupInfo_Lua()
{
	KLuaManager luaManager( g_pLua );

	int iServerGroupID;
	std::wstring wstrGroupName;
				
	LUA_GET_VALUE_RETURN(	luaManager,	L"m_iGroupID",			iServerGroupID,			0,				goto LoadFail;	);
	LUA_GET_VALUE_RETURN(	luaManager,	L"m_strGroupName",		wstrGroupName,			L"����",		goto LoadFail;	);
	
	{
		// ������ ���� ����
		KESDServerGroupInfo kServerGroupInfo;
		kServerGroupInfo.m_iServerGroupID	= iServerGroupID;
		kServerGroupInfo.m_wstrGroupName	= wstrGroupName;

		m_vecServerGroupInfo.push_back( kServerGroupInfo );
	}
	return true;

LoadFail:
	KDbgStm << "Add Server Group Info �Ľ� ����" << trace;

	return false;
}

bool KEsdRemoteController_ConsoleImp::AddServerInfo_Lua()
{
	KLuaManager luaManager( g_pLua );

	std::wstring wstrName;
	std::wstring wstrIP;	

	int iServerGroupID;
	int iFileDsnID;
	int iTodayLineColorID;
	int iYesterdayLineColorID;
	bool bIsLoginServer;

	LUA_GET_VALUE(			luaManager, L"m_bIsLoginServer",		bIsLoginServer,			false						);
	LUA_GET_VALUE(			luaManager,	L"m_iServerGroupID",		iServerGroupID,			0							);
	LUA_GET_VALUE_RETURN(	luaManager,	L"m_iLogDsnID",				iFileDsnID,				0,			goto LoadFail;	);
	LUA_GET_VALUE_RETURN(	luaManager,	L"m_iTodayLineColorID",		iTodayLineColorID,		0,			goto LoadFail;	);
	LUA_GET_VALUE_RETURN(	luaManager,	L"m_iYesterdayLineColorID",	iYesterdayLineColorID,	0,			goto LoadFail;	);
	LUA_GET_VALUE_RETURN(	luaManager,	L"m_strServerName",			wstrName,				L"",		goto LoadFail;	);
	LUA_GET_VALUE_RETURN(	luaManager,	L"m_strServerIPAddr",		wstrIP,					L"",		goto LoadFail;	);

	{
		// ���� ���� ����
		KESDServerInfo kInfo;
		kInfo.m_wstrName		  	  = wstrName;
		kInfo.m_wstrIP		  		  = wstrIP;
		kInfo.m_iNowTimeCCU			  = 0;
		kInfo.m_iFileDsnID	  		  = iFileDsnID;
		kInfo.m_iTodayLineColorID	  = iTodayLineColorID;
		kInfo.m_iYesterdayLineColorID = iYesterdayLineColorID;
		kInfo.m_iServerGroupID		  = iServerGroupID;
		kInfo.m_bIsRemoteShutdown	  = false;
		kInfo.m_bConnected	   		  = false;
		kInfo.m_bChecked	   		  = false;
		kInfo.m_bGraphSelected		  = false;
		kInfo.m_bIsLoginServer		  = bIsLoginServer;
		kInfo.m_bBeforeLgState 		  = false;
		kInfo.m_bBeforeCnState 		  = false;
		kInfo.m_bBeforeGmState 		  = false;
		kInfo.m_bCrashLgServer 		  = false;
		kInfo.m_bCrashCnServer 		  = false;
		kInfo.m_bCrashGmServer 		  = false;
		kInfo.m_iLastRecvTime  		  = 0;

		// ���� ����
		kInfo.m_serverUID = AuthenticateServer( wstrIP.c_str(), kInfo.m_iFileDsnID );

		m_vecEsdServerInfo.push_back( kInfo );
	}

	return true;

LoadFail:
	KDbgStm << "Add Server Info �Ľ� ����" << trace;

	return false;
}

bool KEsdRemoteController_ConsoleImp::AddLineColor_Lua()
{
	KLuaManager luaManager( g_pLua );

	int iLineColorID;
	int r, g, b;

	LUA_GET_VALUE_RETURN(	luaManager,	"m_iLineColorID",		iLineColorID,	0,	goto LoadFail; );

	LUA_GET_VALUE_RETURN(	luaManager,	"m_iLineColorR",		r,				0,	goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager,	"m_iLineColorG",		g,				0,	goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager,	"m_iLineColorB",		b,				0,	goto LoadFail; );

	m_mapLineColor.insert( std::make_pair( iLineColorID, RGB( r, g, b ) ) );

	return true;

LoadFail:
	KDbgStm << "Line Color �Ľ� ����" << trace;

	return true;
}

bool KEsdRemoteController_ConsoleImp::AddFileDsn_Lua()
{
	KLuaManager luaManager( g_pLua );

	std::string strDSNName;
	KFileDsnData* pFileDsnData = new KFileDsnData;
	
	LUA_GET_VALUE_RETURN(	luaManager,	"m_iFileDsnID",			pFileDsnData->m_iFileDsnID,		0,		goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager,	"m_strDsnFileName",		pFileDsnData->m_strFileDsnName,	"",		goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager,	"m_bIsLog",				pFileDsnData->m_bIsLog,			false,	goto LoadFail; );

#ifndef COMPILE_FOR_MAIN_SERVER
	// �系�� ���ӿ� (File DSN)
	strDSNName = KGetCurFolder();
	strDSNName += "\\";
	strDSNName += pFileDsnData->m_strFileDsnName;
	pFileDsnData->m_kQuery.Connect( KQuery::eConnectType_SQL, (char*) strDSNName.c_str(), NULL, NULL );
#else
	{
		char* pBuffer = NULL; // DB Connection String Buffer

		std::vector< int > vecKey;
		vecKey.push_back( KDATA_1 );
		vecKey.push_back( KDATA_2 );
		vecKey.push_back( KDATA_3 );

		// ���� ���ӿ� (Direct)
		pBuffer = CFileCrypt::FileDecrypt( vecKey, pFileDsnData->m_strFileDsnName );

		if( pBuffer == NULL )
		{
			MessageBox( NULL, L"����͸��� ���������� ��� ������ �� �����ϴ�!", L"Warning!!!", MB_OK );
			delete pFileDsnData;
			delete []pBuffer;
			return false;
		}
		
		if( pFileDsnData->m_kQuery.Connect( KQuery::eConnectType_SQLDirect, pBuffer, NULL, NULL ) == FALSE )
		{
			KDbgStm << "DB Connection Failed" << trace;
			delete pFileDsnData;
			delete []pBuffer;
			return false;
		}
		delete []pBuffer;
	}
#endif

	m_mapFileDsn.insert( std::make_pair( pFileDsnData->m_iFileDsnID, pFileDsnData ) );

	return true;

LoadFail:
	KDbgStm << "AddFileDsn �Ľ� ����" << trace;
	delete pFileDsnData;

	return false;
}

bool KEsdRemoteController_ConsoleImp::CreateDBConnectionFile( std::wstring wstrTargetFilePath )
{
	std::vector< int > vecKey;
	vecKey.push_back( KDATA_1 );
	vecKey.push_back( KDATA_2 );
	vecKey.push_back( KDATA_3 );

	return CFileCrypt::FileEncrypt( vecKey, KncUtil::toNarrowString( wstrTargetFilePath ) );
}


