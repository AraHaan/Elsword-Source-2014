#include <WinSock2.h>
#include "GiantRoleRegManager.h"

#ifdef SERV_COUNTRY_CN

//#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;
#include "Socket/NetCommon.h"
#include "NetLayer.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "GiantRoleReg.h"
#include "GiantRoleRegPacket.h"

ImplToStringW( KGiantRoleRegManager )
{
	return stm_;
}

ImplPfID( KGiantRoleRegManager, PI_LOGIN_GIANT_ROLEREG );

ImplementSingleton( KGiantRoleRegManager );

KGiantRoleRegManager::KGiantRoleRegManager()
{
}

KGiantRoleRegManager::~KGiantRoleRegManager()
{
}



void KGiantRoleRegManager::RegToLua()
{
	lua_tinker::class_add<KGiantRoleRegManager>( g_pLua, "KGiantRoleRegManager" );

	lua_tinker::class_inh<KGiantRoleRegManager, KGiantBaseManager>( g_pLua );

	lua_tinker::decl( g_pLua, "GiantRoleRegManager", this );
}


KThread* KGiantRoleRegManager::CreateThread()
{
	return new KGiantRoleReg();
}

void KGiantRoleRegManager::RegisterUserIDUnit( unsigned int uiGiantUID, KEGIANT_ROLEREG_CREATE_UNIT_REQ kPacket )
{
	std::map< unsigned int, KEGIANT_ROLEREG_CREATE_UNIT_REQ >::iterator mit;

	KLocker lock( m_csUserIDUnit );
	mit = m_mapUserIDUnit.find( uiGiantUID );
	if( mit != m_mapUserIDUnit.end() )
	{
		START_LOG( cerr, L"user id exists." )
			<< BUILD_LOG( mit->first )
			<< BUILD_LOG( mit->second.m_iActorUID )
			<< END_LOG;
	}
	else
	{
		START_LOG( clog2, L"user id reg" )
			<< BUILD_LOG( uiGiantUID )
			<< BUILD_LOG( m_mapUserIDUnit.size() )
			<< END_LOG;
	}

	m_mapUserIDUnit[uiGiantUID] = kPacket;
}

void KGiantRoleRegManager::UnregisterUserIDUnit( unsigned int uiGiantUID, KEGIANT_ROLEREG_CREATE_UNIT_REQ& kPacket )
{
	std::map< unsigned int, KEGIANT_ROLEREG_CREATE_UNIT_REQ >::iterator mit;

	KLocker lock( m_csUserIDUnit );
	mit = m_mapUserIDUnit.find( uiGiantUID );
	if( mit == m_mapUserIDUnit.end() )
	{
		START_LOG( cerr, L"user id not exists." )
			<< BUILD_LOG( uiGiantUID )
			<< END_LOG;

//		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		kPacket.m_iActorUID = 0;
		return;
	}
	else
	{
		START_LOG( clog2, L"user id unreg" )
			<< BUILD_LOG( uiGiantUID )
			<< BUILD_LOG( mit->second.m_iActorUID )
			<< END_LOG;
	}

	kPacket = mit->second;
	m_mapUserIDUnit.erase( mit );
}

void KGiantRoleRegManager::RegisterUserIDGuild( unsigned int uiGiantUID, KEGIANT_ROLEREG_CREATE_GUILD_REQ kPacket )
{
	std::map< unsigned int, KEGIANT_ROLEREG_CREATE_GUILD_REQ >::iterator mit;

	KLocker lock( m_csUserIDGuild );
	mit = m_mapUserIDGuild.find( uiGiantUID );
	if( mit != m_mapUserIDGuild.end() )
	{
		/*
		START_LOG( cerr, L"user id exists." )
			<< BUILD_LOG( mit->first )
			<< BUILD_LOG( mit->second.m_iActorUID )
			<< END_LOG;
		*/
	}
	else
	{
		START_LOG( clog2, L"user id reg" )
			<< BUILD_LOG( uiGiantUID )
			<< BUILD_LOG( m_mapUserIDGuild.size() )
			<< END_LOG;
	}

	m_mapUserIDGuild[uiGiantUID] = kPacket;
}

void KGiantRoleRegManager::UnregisterUserIDGuild( unsigned int uiGiantUID, KEGIANT_ROLEREG_CREATE_GUILD_REQ& kPacket )
{
	std::map< unsigned int, KEGIANT_ROLEREG_CREATE_GUILD_REQ >::iterator mit;

	KLocker lock( m_csUserIDGuild );
	mit = m_mapUserIDGuild.find( uiGiantUID );
	if( mit == m_mapUserIDGuild.end() )
	{
		START_LOG( cerr, L"user id not exists." )
			<< BUILD_LOG( uiGiantUID )
			<< END_LOG;

		kPacket.m_iActorUID = 0;
		return;
	}
	else
	{
		START_LOG( clog2, L"user id unreg" )
			<< BUILD_LOG( uiGiantUID )
			<< BUILD_LOG( mit->second.m_iActorUID )
			<< END_LOG;

		kPacket = mit->second;
		m_mapUserIDGuild.erase( mit );
	}
}

void KGiantRoleRegManager::MakeEventFromReceivedPacket()
{
	DumpBuffer( ( BYTE* )m_cRecvBuffer, false );
	KGiantRoleRegPacket kReceivedPacket;
	if( !kReceivedPacket.ReadFromBuffer( ( BYTE* )m_cRecvBuffer ) )
	{
		START_LOG( cerr, L"���ۿ��� �б� ����." )
			<< END_LOG;

		DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
		return;
	}

	switch( kReceivedPacket.GetCommand() )
	{
	case KGiantCommonPacket::GCP_CCT_NULL:
	case KGiantCommonPacket::GCP_CCT_INITIALIZE:
		Handle_OnCommonPacket(kReceivedPacket);
		return;
	case KGiantRoleRegPacket::GRP_RCT_NAME:
		Handle_OnRoleRegPacket(kReceivedPacket);
		return;
	}

//end_proc:
	START_LOG( cerr, L"��Ŷ Ÿ�� �̻�." )
		<< BUILD_LOG( kReceivedPacket.GetTotalLength() )
		<< BUILD_LOG( kReceivedPacket.GetCommand() )
		<< BUILD_LOG( kReceivedPacket.GetParaCommand() )
		<< END_LOG;

	DumpBuffer( ( BYTE* )m_cRecvBuffer, true );


}


void KGiantRoleRegManager::Handle_OnCommonPacket(KGiantCommonPacket& kReceivedPacket)
{
	switch( kReceivedPacket.GetCommand() )
	{
	case KGiantRoleRegPacket::GCP_CCT_INITIALIZE:
		{
			switch(kReceivedPacket.GetParaCommand())
			{
			case KGiantRoleRegPacket::GCP_PCT_INITIALIZE_ACK:
				{
					KEGIANT_COMMON_INITIALIZE_ACK kPacket;
					kReceivedPacket.Read( kPacket );

					m_kGiantServerInfo.m_usZone = kPacket.m_usZone;
					m_kGiantServerInfo.m_usGame = kPacket.m_usGame;
					m_kGiantServerInfo.m_strZoneName = kPacket.m_wstrZoneName;
					m_kGiantServerInfo.m_byteNetType = kPacket.m_byteNetType;

					START_LOG( cout, L"RoleRegServer ���� ���� ����!" )
						<< BUILD_LOG( kPacket.m_usZone )
						<< BUILD_LOG( kPacket.m_usGame )				
						<< BUILD_LOG( kPacket.m_wstrZoneName )
						<< BUILD_LOG( kPacket.m_byteNetType )
						<< END_LOG;
				}
				return;
			default:
				// ���� ���������Ƿ� ���� ������ �����Ѵ�.
				CLOSE_SOCKET( m_sock );		
				goto end_proc;
			}
		}
		break;
	case KGiantRoleRegPacket::GCP_CCT_NULL:
		{
			switch( kReceivedPacket.GetParaCommand() )
			{
			case KGiantCommonPacket::GCP_PCT_NULL_SERVER:
				{
					// Ŀ�ؼ� ������ ���� �������� ���� �� ��Ŷ�� ���� �״�� �����ش�.
					KEGIANT_COMMON_NULL_SERVER kPacket;
					kReceivedPacket.Read( kPacket );

					KEventPtr spEvent( new KEvent );
					spEvent->SetData(PI_NULL, NULL, EGIANT_COMMON_NULL_SERVER, kPacket );
					QueueingEvent( spEvent );

					return;
				}
				break;
			case KGiantCommonPacket::GCP_PCT_NULL_CLIENT:
				{
					// ���� ���� ��Ŷ�̹Ƿ� ���ٸ� ó�� ���� �׳� ����.
					START_LOG( clog, L"RoleRegServer Null Client" )
						<< END_LOG;

					return;
				}
				break;
			}
		}
		break;
	}

end_proc:
	START_LOG( cerr, L"��Ŷ Ÿ�� �̻�." )
		<< BUILD_LOG( kReceivedPacket.GetTotalLength() )
		<< BUILD_LOG( kReceivedPacket.GetCommand() )
		<< BUILD_LOG( kReceivedPacket.GetParaCommand() )
		<< END_LOG;

	DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
}

void KGiantRoleRegManager::Handle_OnRoleRegPacket(KGiantRoleRegPacket& kReceivedPacket)
{
	if( kReceivedPacket.GetCommand() != KGiantRoleRegPacket::GRP_RCT_NAME )
		goto end_proc;

	switch( kReceivedPacket.GetParaCommand() )
	{
	case KGiantRoleRegPacket::GR_PCT_NAME:
		{
			KEGIANT_ROLEREG kPacket;
			kReceivedPacket.Read( kPacket );

			switch( kPacket.m_usRegType )
			{
			case KGiantRoleRegPacket::GR_RT_UNIT:
				{
					if( kPacket.m_usState & KGiantRoleRegPacket::GR_ST_MASK_INSERT_REQ )
					{
						KEventPtr spEvent( new KEvent );
						spEvent->SetData(PI_NULL, NULL, EGIANT_ROLEREG_CREATE_UNIT_ACK, kPacket );
						QueueingEvent( spEvent );

						return;
					}

					if( kPacket.m_usState & KGiantRoleRegPacket::GR_ST_MASK_DELETE_REQ )
					{
						KEventPtr spEvent( new KEvent );
						spEvent->SetData(PI_NULL, NULL, EGIANT_ROLEREG_DELETE_UNIT_ACK, kPacket );
						QueueingEvent( spEvent );

						return;
					}
				}
				break;
			case KGiantRoleRegPacket::GR_RT_GUILD:
				{
					if( kPacket.m_usState & KGiantRoleRegPacket::GR_ST_MASK_INSERT_REQ )
					{
						KEventPtr spEvent( new KEvent );
						spEvent->SetData(PI_NULL, NULL, EGIANT_ROLEREG_CREATE_GUILD_ACK, kPacket );
						QueueingEvent( spEvent );

						return;
					}

					if( kPacket.m_usState & KGiantRoleRegPacket::GR_ST_MASK_DELETE_REQ )
					{
						KEventPtr spEvent( new KEvent );
						spEvent->SetData(PI_NULL, NULL, EGIANT_ROLEREG_DELETE_GUILD_ACK, kPacket );
						QueueingEvent( spEvent );

						return;
					}
				}
				break;
			}
		}
		break;
	}

end_proc:
	START_LOG( cerr, L"��Ŷ Ÿ�� �̻�." )
		<< BUILD_LOG( kReceivedPacket.GetTotalLength() )
		<< BUILD_LOG( kReceivedPacket.GetCommand() )
		<< BUILD_LOG( kReceivedPacket.GetParaCommand() )
		<< END_LOG;

	DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
}

//2011.09.23 lygan_������ //��å �������� ���� �ش� ó���� �ǽð����� �̷�� ����.
//{{ 

//void KGiantRoleRegManager::TickDeleteRole()
//{
//	const static double check_time = 1/*hours*/ * 60/*mins*/ * 60/*secs*/;
//
//	// 1. 1�ð����� üũ
//	if( m_kRefreshTimer.elapsed() < check_time )
//		return;
//
//	// 2. Ÿ�̸� �ʱ�ȭ	
//	m_kRefreshTimer.restart();
//
//	// 3. ������ �ð��� �Ǹ� ĳ����, ���� ���� ��Ŷ ����
//	CTime tCurTime = CTime::GetCurrentTime();
//	//	if( tCurTime > m_tDeleteRoleTime )
//	{
//		SendToGameDB( DBE_GET_GIANT_DELETED_UNIT_LIST_REQ );
//
//		//		SetNextDeleteRoleTime();
//	}
//}
//void KGiantRoleRegManager::SetDeleteRoleTime_LUA( const int iHour )
//{
//	// ���� ��Ŷ ���� �ð� ����
//	CTime tCurTime	 = CTime::GetCurrentTime();
//	m_tDeleteRoleTime = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), iHour, 0, 0 );
//	if( m_tDeleteRoleTime < tCurTime )
//	{
//		// �̹� ������ �ð��̶�� �������� ����
//		SetNextDeleteRoleTime();
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	CStringW strDeleteTime = (CStringW)m_tDeleteRoleTime.Format( _T("%Y-%m-%d %H:%M:%S") );
//	START_LOG( cout, L"���̾�Ʈ Role ���� ��Ŷ ���� �ð� ����!" )
//		<< BUILD_LOG( strDeleteTime.GetBuffer() );
//	//////////////////////////////////////////////////////////////////////////
//}
//
//void KGiantRoleRegManager::SetNextDeleteRoleTime()
//{
//	// ���� ���� ��Ŷ ���� �ð� ����
//	m_tDeleteRoleTime += CTimeSpan( 1, 0, 0, 0 );
//
//	//////////////////////////////////////////////////////////////////////////
//	CStringW strDeleteTime = (CStringW)m_tDeleteRoleTime.Format( _T("%Y-%m-%d %H:%M:%S") );
//	START_LOG( cout, L"���̾�Ʈ Role ���� ��Ŷ ���� ���ڸ� ���� ��¥�� ����!" )
//		<< BUILD_LOG( strDeleteTime.GetBuffer() );
//	//////////////////////////////////////////////////////////////////////////
//}

//}}
void KGiantRoleRegManager::SendToGameDB( unsigned short usEventID )
{
	SendToGameDB( usEventID, char() );
}

#endif // SERV_COUNTRY_CN