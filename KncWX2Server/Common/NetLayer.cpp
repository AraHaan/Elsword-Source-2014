#include "NetLayer.h"
#include "ActorManager.h"
#include "Socket/IOCP.h"
#include "Socket/IOThread.h"
#include "Socket/Session.h"
#include "Socket/NetCommon.h"
#include "BaseServer.h"

#include <dbg/dbg.hpp>
#include <KncSecurity/KncSecurity.h>
#include <boost/bind.hpp>

NiImplementRootRTTI( KNetLayer );
ImplInstanceKeeping( KNetLayer );

KNetLayer::KNetLayer(void) : m_usUdpPort( 0 )
{
    m_kMaxTimeInfo.m_dwTerm	= 0;
    m_kMaxTimeInfo.m_cTime	= CTime::GetCurrentTime();
    // m_kMaxTimeInfo�� ������ �������� �����ڰ� ���� �����Ƿ� �ʱ�ȭ���� �ʿ� ����.

    m_usPortMaster			= 0;

	//{{ 2009. 10. 18  ������	SequenceNum
	m_bCheckSequenceNum		= false;
	//}}
	//{{ 2011. 06. 08	������	KEvent ��ó ��� ���
#ifdef SERV_KEVENT_FROM
	m_uiSessionType = KSession::SST_NONE;
#endif SERV_KEVENT_FROM
	//}}
}

KNetLayer::~KNetLayer(void)
{
}

ImplToStringW( KNetLayer )
{
    SiKIocp()->ToString( stm_ );

    KSocketObject::DumpSendQueueLog( stm_ );

    stm_ << std::endl;

    KSession::DumpMaxSendData( stm_ );

    stm_ << std::endl;

	//{{ 2009. 10. 18  ������	SequenceNum	
	KSession::DumpPacketAuthInfo( stm_ );
	//}}

	stm_ << std::endl;

    START_TOSTRINGW 
        << TOSTRINGW( m_usPortMaster )
        << TOSTRINGW( m_usUdpPort );

    KIOThread::DumpExceptionInfo( stm_ );

    DumpMaxTimeInfo( stm_ );

    return stm_;
}

//{{ 2009. 12. 15  ������	��������
void KNetLayer::DumpToLogFile()
{
	std::wstringstream wstrDump;
	ToString( wstrDump );
	dbg::cout << wstrDump.str() << dbg::endl;
}
//}}

bool KNetLayer::Init()
{
	m_kAccepter.Init( m_usPortMaster, KNetLayer::OnAccept );

    // 050502. florist. ���� �ڵ� �ʱ�ȭ ����
    KncSecurity::InitSecurity();

    // init iocp
    SiKIocp()->Init( 4 );

    SiKIocp()->BeginThread();

	m_kAccepter.Begin();
	
	if( m_usUdpPort > 0 )
	{
		_JIF( m_kUdpEcho.Init( m_usUdpPort ), return false );

		m_kUdpEcho.Begin();
	}

    return true;
}

bool KNetLayer::InitNCUDP( int port )
{
	if( port > 0 )
	{
		_JIF( m_kLBSUdpEcho.Init( port ), return false );
		m_kLBSUdpEcho.Begin();
	}
	else
	{
		return false;
	}

	return true;
}

void KNetLayer::CloseAccepter()
{
    m_kUdpEcho.ShutDown();

//{{ robobeg : 2013-08-08
    // ���ͼ��� ����� ũ������ �����ϱ� ���� ����
    // �̰͸����δ� ������� �ʰ�, NetLayer, SimLayer �� CnRoomManager ������ ��������� �������ؾ� �Ѵ�.
    // ���ҵ� �ؿ��� ������Կ��� �����Ͻÿ�
    //m_kLBSUdpEcho.End();
    m_kLBSUdpEcho.End( INFINITE );
//}} robobeg : 2013-08-08

    m_kAccepter.End();
}

void KNetLayer::ShutDown()
{
    SiKIocp()->EndThread();
	Sleep( 100 );
    KIocp::ReleaseInstance();

    KncSecurity::ReleaseSecurity();
}

#include <KncLua.h>
#include <lua_tinker.h>

void KNetLayer::RegToLua()
{
	lua_tinker::class_add<KNetLayer>( g_pLua, "KNetLayer" );
	lua_tinker::class_def<KNetLayer>( g_pLua, "SetPort",				&KNetLayer::SetPort );
	lua_tinker::class_def<KNetLayer>( g_pLua, "SetUdpEcho",				&KNetLayer::SetUdpEcho );
	lua_tinker::class_def<KNetLayer>( g_pLua, "ResetMaxData",			&KNetLayer::ResetMaxData );
	//{{ 2009. 10. 18  ������	SequenceNum
	lua_tinker::class_def<KNetLayer>( g_pLua, "SetCheckSequenceNum",	&KNetLayer::SetCheckSequenceNum_LUA );
	lua_tinker::class_def<KNetLayer>( g_pLua, "SetPacketAuthFailLimit",	&KNetLayer::SetPacketAuthFailLimit_LUA );	
	//}}
}

void KNetLayer::ResetMaxData()
{
    std::wostringstream strstm;

    DumpMaxTimeInfo( strstm );
    KSession::DumpMaxSendData( strstm );

    dbg::cout << strstm.str();
    
    ResetMaxTimeInfo();
    KSession::ResetMaxSendData();
}

void KNetLayer::SetCheckSequenceNum_LUA( bool bVal )
{
	KLocker lock( m_csCheckSequenceNum );
	
	m_bCheckSequenceNum = bVal;

	START_LOG( cout, L"Check Sequence Number : " << m_bCheckSequenceNum );
}

void KNetLayer::SetPacketAuthFailLimit_LUA( u_int uiCount )
{
	KLocker lock( m_csPacketAuthFailLimit );

	m_uiPacketAuthFailLimit = uiCount;

	START_LOG( cout, L"Packet Auth Fail Limit : " << m_uiPacketAuthFailLimit );
}

void KNetLayer::OnAccept( SOCKET sock, const SOCKADDR_IN& sinRemote )
{
	// 070423. florist. �ܼ��� ����(delegate)�� �߻�.
	// KAccepter�� Ư�� ��ü�� ������� ���������� ���۵� �� �ֵ��� �ϴµ� ���ǰ� ����.
	KNetLayer::GetKObj()->OnAccept_( sock, sinRemote );
}

void KNetLayer::OnAccept_( SOCKET sock, const SOCKADDR_IN& sinRemote )
{
    KActorPtr spActor( m_spActorFactory->CreateActor() );
    if( !spActor )
    {
        START_LOG( cerr, L"Create Actor Failed" );
        CLOSE_SOCKET( sock );
        return;
    }

	//{{ 2011. 06. 08	������	KEvent ��ó ��� ���
#ifdef SERV_KEVENT_FROM
	spActor->SetSessionType( static_cast<KSession::SESSION_TYPE>(m_uiSessionType) );
#endif SERV_KEVENT_FROM
	//}}

	//{{ 2009. 10. 18  ������	SequenceNum
	spActor->InitCheckSequenceNumInfo( IsCheckSequenceNum(), GetPacketAuthFailLimit() ); // IOCP�� ����ϱ� ���� �����մϴ�.
	//}}

    KSocketObjectPtr spSockObj = spActor->GetSockObj();
    JIF( spSockObj );

    spSockObj->SetSocketInfo( sock, sinRemote );

    LIF( SiKIocp()->AssociateSocket( spSockObj ) );

    spSockObj->InitRecv();
    spSockObj->OnAcceptConnection();

    DWORD dwElapTime = ::GetTickCount();  // 040428. ó�� �ҿ�ð��� ���Ѵ�.

    KActorManager::GetKObj()->ReserveAdd( spActor );

    dwElapTime = ::GetTickCount() - dwElapTime;

    {   // locking scope
        KLocker lock( m_kMaxTimeInfo.m_cs );

        if( dwElapTime > m_kMaxTimeInfo.m_dwTerm )
        {
            m_kMaxTimeInfo.m_strIP    = KncUtil::toWideString(inet_ntoa(sinRemote.sin_addr));
            m_kMaxTimeInfo.m_cTime    = CTime::GetCurrentTime();
            m_kMaxTimeInfo.m_dwTerm   = dwElapTime;

            START_LOG( clog2, L"accepter ��ü ���� ���ð� ����. Remote IP : " << m_kMaxTimeInfo.m_strIP )
                << BUILD_LOG( m_kMaxTimeInfo.m_dwTerm );
        }
    }   // locking scope
}

void KNetLayer::DumpMaxTimeInfo( OUT std::wostream& stm_ ) const
{
    KLocker lock( m_kMaxTimeInfo.m_cs );

    stm_ << " -- Max accepter waiting time. NagleOn : " 
         << std::boolalpha << m_kAccepter.GetNagleAlgOn() << std::noboolalpha << std::endl
         << "  user IP : " << m_kMaxTimeInfo.m_strIP << std::endl
         << "  duration : " << m_kMaxTimeInfo.m_dwTerm << std::endl
         << "  executed time : " << (const wchar_t*)m_kMaxTimeInfo.m_cTime.Format(_T("%Y-%m-%d %H:%M:%S")) << std::endl;
}

void KNetLayer::ResetMaxTimeInfo()
{
    KLocker lock( m_kMaxTimeInfo.m_cs );

    m_kMaxTimeInfo.m_dwTerm = 0;
    m_kMaxTimeInfo.m_cTime = CTime::GetCurrentTime();
    m_kMaxTimeInfo.m_strIP = (const wchar_t*)m_kMaxTimeInfo.m_cTime.Format(_T("%Y-%m-%d %H:%M:%S"));
    m_kMaxTimeInfo.m_strIP.append( L" �� ������ �ʱ�ȭ �Ǿ����ϴ�." );
}

