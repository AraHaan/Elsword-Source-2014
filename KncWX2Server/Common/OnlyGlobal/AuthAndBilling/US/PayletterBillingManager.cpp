#include <WinSock2.h>
#include "PayletterBillingManager.h"

#ifdef SERV_COUNTRY_US

#include "Socket/NetCommon.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "PayletterBilling.h"
#include "Simlayer.h"

#include "_GameProtocol.h"

//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}

DWORD KPayletterBillingManager::ms_dwHeartBeatGap( 1 * 60 * 1000 );
DWORD KPayletterBillingManager::ms_dwConnectionCheckGap( 20 * 1000 );

ImplToStringW( KPayletterBillingManager )
{
	return stm_;
}

ImplPfID( KPayletterBillingManager, PI_GS_PUBLISHER_BILLING );
ImplementSingleton( KPayletterBillingManager );


KPayletterBillingManager::KPayletterBillingManager() :
m_bInitialized(false)
{

	m_kPayletterBillingInfo.m_strIP.clear();
	m_kPayletterBillingInfo.m_usPort = 0;
	m_kPayletterBillingInfo.m_iDomain = 0;
	//{{ 2009. 11. 21  ������	���������ӱ�������
	m_sock = INVALID_SOCKET;
	m_bFirstConnectSucc = false;
	//}}
	m_dwLastHeartBeatTick = ::GetTickCount();
	m_dwLastConnectionCheckTick = ::GetTickCount();

	m_ulRecvCP = 0;

	m_bSocketConnect = false;

	KCSLOCK_SET_VALUE( m_ulCurrentPacketNo, 0 );

	m_spEvent.reset( CreateEvent( NULL, false, false, NULL ), CloseHandle );
}

KPayletterBillingManager::~KPayletterBillingManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

void KPayletterBillingManager::RegToLua()
{
	//class_< KPurpleAuthManager >( "KNexomBillingAuthManager" )
	//	.def( "dump",						KPurpleAuthManager::Dump )
	//	.def( "InitPurpleAuthInfo",	KPurpleAuthManager::InitPurpleAuthInfo )
	//	.def( "SetNumThread",				KPurpleAuthManager::Init );

	//decl( "NXBillingAuthManager", this );
	lua_tinker::class_add<KPayletterBillingManager>( g_pLua, "KPayletterManager" );
	//lua_tinker::class_def<KPurpleAuthManager>( g_pLua, "dump",					 &KPurpleAuthManager::Dump );	
	lua_tinker::class_def<KPayletterBillingManager>( g_pLua, "InitPayletterBillingInfo",	&KPayletterBillingManager::InitPayletterBillingInfo );
	lua_tinker::class_def<KPayletterBillingManager>( g_pLua, "SetNumThread",			 &KPayletterBillingManager::Init );

	lua_tinker::decl( g_pLua, "PayletterBillingManager", this );
}

void KPayletterBillingManager::InitPayletterBillingInfo( const char* szNexonAuthIP, unsigned short usPort, int iDomain )
{
	m_kPayletterBillingInfo.m_strIP		= szNexonAuthIP;
	m_kPayletterBillingInfo.m_usPort		= usPort;
	m_kPayletterBillingInfo.m_iDomain	= iDomain;	
}


void KPayletterBillingManager::Tick()
{	
}

void KPayletterBillingManager::Init( int nThreadNum )
{
	m_ulRecvCP = 0;

	//////////////////////////////////////////////////////////////////////////
	// thread setting : recv�� recvfrom() �Լ����� �˾Ƽ� block�ȴ�.
	m_spThreadRecv = boost::shared_ptr< KTThread< KPayletterBillingManager > >
		( new KTThread< KPayletterBillingManager >( *this, &KPayletterBillingManager::Recv, 50 ) );

	m_spThreadSend = boost::shared_ptr< KTThread< KPayletterBillingManager > >
		( new KTThread< KPayletterBillingManager >( *this, &KPayletterBillingManager::Send, 100 ) );

	//{{ 2009. 11. 21  ������	���������ӱ�������
	m_spThreadKeepConnect = boost::shared_ptr< KTThread< KPayletterBillingManager > >
		( new KTThread< KPayletterBillingManager >( *this, &KPayletterBillingManager::KeepConnectionThread, 1000 ) );
	//}}



	KThreadManager::Init( nThreadNum );	

	// ���� �׽�Ʈ
	//Connect();
}

	

KThread* KPayletterBillingManager::CreateThread()
{
	return new KPayletterBilling;
}

void KPayletterBillingManager::BeginThread()
{	
	// ������Ŷ ���Ž�����
	if( m_spThreadRecv )
	{
		m_spThreadRecv->Begin();
	}

	// ������Ŷ ���۽�����
	if( m_spThreadSend )
	{
		m_spThreadSend->Begin();
	}

	KThreadManager::BeginThread();

	//{{ 2009. 11. 21  ������	���������ӱ�������
	// ������������ ��Ʈ��Ʈ üũ �� ���� ������ ���� ��������������
	if( m_spThreadKeepConnect )
	{
		// Thread�� �����ϸ鼭 ���������� ���ӽõ��Ѵ�.
		m_spThreadKeepConnect->Begin();
	}
	//}}

}

void KPayletterBillingManager::EndThread()
{
	//{{ 2009. 11. 21  ������	���������ӱ�������
	if( m_spThreadKeepConnect )
	{
		m_spThreadKeepConnect->End( 3000 );

		START_LOG( cout, L"�ؽ� ���� ���� ���� üũ ������ ����!" );
	}
	//}}

	KThreadManager::EndThread();

	if( m_spThreadSend )
	{
		m_spThreadSend->End( 10000 );

		START_LOG( cout, L"�ؽ� ���� ���� Send ������ ����!" );
	}

	// recv ������ ���̱� ���� ������ ����!
	CLOSE_SOCKET( m_sock );

	if( m_spThreadRecv )
	{
		m_spThreadRecv->End( 3000 );

		START_LOG( cout, L"�ؽ� ���� ���� Recv ������ ����!" );
	}
}

bool KPayletterBillingManager::Connect()
{
	//{{ [����] Connect()�Լ��� ȣ��Ǵ� ��Ȳ������ RecvThread���� recv()�Լ��� �׻� ȣ������Ѵ�.
	//          ���� m_ulRecvCP������ 0���� �ʱ�ȭ �Ǵ��� �������� ����.
	m_ulRecvCP = 0;
	//}}

	m_sock = ::socket( AF_INET, SOCK_STREAM, 0 );    // ���� ����

	if( INVALID_SOCKET == m_sock )
	{
		START_LOG( cerr, GET_WSA_MSG )
			<< BUILD_LOG( WSAGetLastError() )
			<< END_LOG;

		return false;
	}

	SOCKADDR_IN addr;
	addr.sin_family			= AF_INET;
	addr.sin_addr.s_addr	= inet_addr( m_kPayletterBillingInfo.m_strIP.c_str() );
	addr.sin_port			= htons( m_kPayletterBillingInfo.m_usPort );

	int ret = ::connect( m_sock, ( struct sockaddr* )&addr, sizeof( addr ) );

	if( SOCKET_ERROR == ret )
	{
		//{{ 2009. 11. 21  ������	���������ӱ�������
		if( IsFirstConnectSucc() )
		{
			START_LOG( cerr, GET_WSA_MSG )
				<< BUILD_LOG( WSAGetLastError() )
				<< BUILD_LOG( m_kPayletterBillingInfo.m_strIP )
				<< BUILD_LOG( m_kPayletterBillingInfo.m_usPort )
				<< END_LOG;
		}
		//}}
		CLOSE_SOCKET( m_sock );
		return false;
	}
	
	// ���� : ���� ���� ���� recv, send �����尡 �۵��ϸ鼭 ��Ĺ�� �ݾƹ�����.
	//        ���� �ҽ� �׷��� �� �� ������ �� �̷����� �𸣰���.
	//		  �ϴ� ��Ĺ ���� �� �� ���� �ٸ� ������ �۵� ���ϵ��� ���Ƶ״ٰ� ���⼭ �δ� �������.
	m_bSocketConnect = true;

	
	// KENX_BT_INITIALIZE_REQ�� ť���ϱ� ���� false �� ������ �Ѵ�.
	// ���� : �׽�Ʈ - �̰� ������ ���� ���� ��� �� ��� �ϴ� ��. �׷��Ƿ� ������ true ��
	m_bInitialized = false;

	// ���� �Ǵϱ� ��Ŷ�� �ϳ� ���� ���ô�.
	KEPL_BT_HEALTH_CHECK_REQ kPacketInit;

	kPacketInit.m_usReqLen = sizeof(KEPL_BT_HEALTH_CHECK_REQ);
	kPacketInit.m_usReqType = KPayletterBillingPacket::HEALTH_CHECK;
	kPacketInit.m_ulReqKey = 0;
	kPacketInit.m_usRetCode = 0;

	boost::shared_ptr< KPayletterBillingPacket > spPacketInit( new KPayletterBillingPacket );
	spPacketInit->Write( kPacketInit );
	QueueingSendPacket( spPacketInit );

	switch( ::WaitForSingleObject( m_spEvent.get(), 5000 ) )
	{
	case WAIT_OBJECT_0: // ack�� ����� ���� ���.
		START_LOG( cout, L"[����] Payletter ���� ���� Initialize." )
			<< BUILD_LOG( m_bInitialized );
		break;
	case WAIT_TIMEOUT:  // �ð� �ʰ�
		START_LOG( cerr, L"[����] ���� ���� Initialize �ð� �ʰ�." );
		break;
	default:
		START_LOG( cerr, L"[����] ���� ���� Initialize ����." );
		break;
	}

	if( !m_bInitialized )
	{
		CLOSE_SOCKET( m_sock );
		return false;
	}
	
	return true;
}

void KPayletterBillingManager::Recv()
{
	// ���� : ó�� ���������� �������� �������� �۾� ���� �ʴ´�.
	//if( !m_bSocketConnect )
	//	return;

	if( !IsConnected() )
	{
		return;
	}

	int ret;
	ret = ::recv( m_sock,
		m_cRecvBuffer + m_ulRecvCP,
		( int )( MAX_PACKET_SIZE_PLB - m_ulRecvCP),
		0 );

	if(ret == 330)
	{
		int a =0;
		a = 0;
	}

	START_LOG( clog, L"��Ŷ ����." )
		<< BUILD_LOG( ret );

	if( ret == SOCKET_ERROR )
	{
		//{{ 2009. 11. 21  ������	���������ӱ�������
		if( IsFirstConnectSucc() )
		{
			START_LOG( cerr, GET_WSA_MSG );
		}
		//}}

		CLOSE_SOCKET( m_sock );

		//{{ 2010. 10. 13	������	�ؽ� ���� ���� ����
		//OnDisconnectBillingServer( std::wstring( L"��ȿ ���� ���� ����" ) );
		//}}
		return;
	}

	if( ret == 0 )
	{
		START_LOG( cerr, L"���� ������ ������." )
			<< END_LOG;

		//CLOSE_SOCKET( m_sock );

		//{{ 2010. 10. 13	������	�ؽ� ���� ���� ����
		//OnDisconnectBillingServer( std::wstring( L"���������� ���� ����" ) );
		//}}
		return;
	}

	
	m_ulRecvCP += ret;

	while( m_ulRecvCP >= 10 )
	{
		// ���(9) + Ÿ��(1) = 10
		// ��� ��Ŷ�� 10����Ʈ �̻���
		unsigned short usLength;
		::memcpy( &usLength, m_cRecvBuffer, sizeof( usLength ) );


		unsigned long ulTotalPacketSize = usLength;
		if( ulTotalPacketSize > MAX_PACKET_SIZE_PLB )
		{
			START_LOG( cerr, L"������ ��Ŷ ������ �̻�." )
				<< BUILD_LOG( ulTotalPacketSize )
				<< BUILD_LOG( MAX_PACKET_SIZE_PLB )
				<< END_LOG;

			CLOSE_SOCKET( m_sock );

			//{{ 2010. 10. 13	������	�ؽ� ���� ���� ����
			std::wstring wstrReason = boost::str( boost::wformat( L"�ִ� ��Ŷ ������ �ʰ�! MaxPacketSize : %d bytes" ) % ulTotalPacketSize );
			OnDisconnectBillingServer( wstrReason );
			//}}
			return;
		}

		if( ulTotalPacketSize > m_ulRecvCP )
		{
			return;
		}

		MakeEventFromReceivedPacket();

		if( m_ulRecvCP > ulTotalPacketSize )
		{
			::memmove( m_cRecvBuffer, m_cRecvBuffer + ulTotalPacketSize, m_ulRecvCP - ulTotalPacketSize );
		}
		m_ulRecvCP -= ulTotalPacketSize;

		
	}
	
}

void KPayletterBillingManager::Send()
{
	// ���� : ó�� ���������� �������� �������� �۾� ���� �ʴ´�.
	//if( !m_bSocketConnect )
	//	return;
	
	if( !IsConnected() )
	{
		return;
	}
	
	int ret;
	char buf[MAX_PACKET_SIZE_PLB];
	KPayletterBillingPacketPtr spPacket;
	while( GetSendPacket( spPacket ) )
	{		
		if( !spPacket )
		{
			unsigned int uiSendQueueSize;
			KCSLOCK_BEGIN( m_kSendQueue )
				uiSendQueueSize = m_kSendQueue.size();
			KCSLOCK_END()

				START_LOG( cerr, L"������ �̻�." )
				<< BUILD_LOG( uiSendQueueSize )
				<< END_LOG;

			continue;
		}

		spPacket->WriteToBuffer( ( BYTE* )buf );
		DumpBuffer( ( BYTE* )buf, false );

		START_LOG( clog, L"PL_BILLING ������ ��Ŷ Ÿ�� : " << spPacket->GetPacketType())
			<< END_LOG;

		char* cCurBufPos = buf;
		int iRemainedSendSize = spPacket->GetTotalLength();
		while( iRemainedSendSize > 0 )
		{
			ret = ::send( m_sock,
				cCurBufPos,
				iRemainedSendSize,
				0 );

			if( ret == SOCKET_ERROR )
			{
				START_LOG( cwarn, GET_WSA_MSG );
				CLOSE_SOCKET( m_sock );
				return;
			}

			iRemainedSendSize -= ret;
			cCurBufPos += ret;
		}
	}
}

//{{ 2009. 11. 21  ������	���������ӱ�������
void KPayletterBillingManager::KeepConnectionThread()
{
	if( !IsFirstConnectSucc() )
	{
		// ���� �������� ���� �õ�. connect()�Լ� ȣ���߿��� Thread Block
		if( Connect() )
		{
			FirstConnectSucc();
		}

		return;
	}

	CheckConnection();

	// ���� : �̺κ��� �ʿ��Ѱ�?
	//KeepConnection();
}
//}}

void KPayletterBillingManager::KeepConnection()
{
	if( ::GetTickCount() - m_dwLastHeartBeatTick < ms_dwHeartBeatGap )
	{
		return;
	}

	m_dwLastHeartBeatTick = ::GetTickCount();

	//{{ 2008. 5. 30  ������  �ؽ����� �ʱ�ȭ ����
	if( !m_bInitialized  ||  !IsConnected() )
		return;
	//}}

	// ��Ʈ�� ������
	KEPL_BT_HEALTH_CHECK_REQ kPacketNot;
	kPacketNot.m_ulReqKey = 0;
	kPacketNot.m_usReqType = KPayletterBillingPacket::HEALTH_CHECK;
	kPacketNot.m_usReqLen = sizeof(KEPL_BT_HEALTH_CHECK_REQ);
	boost::shared_ptr< KPayletterBillingPacket > spPacket( new KPayletterBillingPacket );
	spPacket->Write( kPacketNot );
	QueueingSendPacket( spPacket );
}


void KPayletterBillingManager::CheckConnection()
{
	if( ::GetTickCount() - m_dwLastConnectionCheckTick < ms_dwConnectionCheckGap )
	{
		return;
	}

	m_dwLastConnectionCheckTick = ::GetTickCount();

	if( !IsConnected() )
	{
		// �ʱ�ȭ ���� ���� ���·� ����!
		m_bInitialized = false;

		m_bSocketConnect = false;

		Connect();
	}
}

void KPayletterBillingManager::HandleInitializeAck( bool bSucceed )
{
	m_bInitialized = bSucceed;
	SetEvent( m_spEvent.get() );
}


void KPayletterBillingManager::QueueingSendPacket( const KPayletterBillingPacketPtr& spPacket )
{	
	JIF( spPacket );
	
	KCSLOCK_BEGIN( m_kSendQueue )
		m_kSendQueue.push( spPacket );
	KCSLOCK_END()
}
bool KPayletterBillingManager::IsConnected()
{
	return ( m_sock != INVALID_SOCKET );
}


void KPayletterBillingManager::OnDisconnectBillingServer( IN const std::wstring& wstrReason )
{
	// ���� ������ ���� �������� �ʾұ� ������ ���� ���� �������� ���� �ʴ´�.
	if( IsFirstConnectSucc() == false )
		return;

	//{{ 2010. 10. 11	������	������ ���� ���� �α�
#ifdef SERV_SERVER_DISCONNECT_LOG
	{
		CTime kRegDate = CTime::GetCurrentTime();
		KE_LOCAL_LOG_SERVER_DISCONNECT_NOT kNot;
		kNot.m_cLogType				= KE_LOCAL_LOG_SERVER_DISCONNECT_NOT::SDLT_DISCONNECT;
		kNot.m_wstrSourceName		= boost::str( boost::wformat( L"%d" ) % KBaseServer::GetKObj()->GetUID() );
		kNot.m_wstrDestinationName	= L"Nexon Billing";
		kNot.m_wstrDestinationIP	= KncUtil::toWideString( m_kPayletterBillingInfo.m_strIP );
		kNot.m_wstrReason			= wstrReason;
		kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_SERVER_DISCONNECT_NOT, kNot );
	}
#endif SERV_SERVER_DISCONNECT_LOG
	//}}
	//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	{
		KE_DISCONNECT_SERVER_REPORT_NOT kNot;
		kNot.m_cType = KMornitoringManager::ET_DISCONNECT_GAME_BILLING;
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( 0, NULL, E_DISCONNECT_SERVER_REPORT_NOT, kNot );
		KBaseServer::GetKObj()->QueueingEvent( spEvent );
	}
#endif SERV_MORNITORING
	//}}

	START_LOG( cout, L"[����] Payletter ���� ���� ���� ����!" )
		<< BUILD_LOG( wstrReason );
}

bool KPayletterBillingManager::GetSendPacket( KPayletterBillingPacketPtr& spPacket )
{
	bool bNotEmpty = false;
	KCSLOCK_BEGIN( m_kSendQueue )
		bNotEmpty = !(m_kSendQueue.empty());
		if(  bNotEmpty == true )
		{
			spPacket = m_kSendQueue.front();
			m_kSendQueue.pop();
		}
	KCSLOCK_END()

	return bNotEmpty;
}

void KPayletterBillingManager::MakeEventFromReceivedPacket()
{
	DumpBuffer( ( BYTE* )m_cRecvBuffer, false );
	KPayletterBillingPacket kPacket;
	if( !kPacket.ReadFromBuffer( ( BYTE* )m_cRecvBuffer ) )
	{
		START_LOG( cerr, L"���ۿ��� �б� ����." )
			<< END_LOG;

		DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
		return;
	}

	// ���� ��Ŷ ����
	START_LOG( clog, L"PL_BILLING ���� ��Ŷ Ÿ�� : " << kPacket.GetPacketType())
		<< END_LOG;
	
	switch( kPacket.GetPacketType() )
	{
#   undef  _ENUM
#   define _ENUM( name, id ) \
	case id: \
		{ \
		K##EPL_BT_##name##_ACK kPacketAck; \
		kPacket.Read( kPacketAck ); \
		KEventPtr spEvent( new KEvent ); \
		spEvent->SetData( PI_GS_PUBLISHER_BILLING, NULL, EPL_BT_##name##_ACK, kPacketAck ); \
		QueueingEvent( spEvent ); \
		} \
		break;
#   include "PayletterBilling_def.h"
	default:
		START_LOG( cerr, L"��Ŷ Ÿ���� �̻���." )
			<< BUILD_LOG( kPacket.GetPacketType() )
			<< END_LOG;
		DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
		break;
	}
}

void KPayletterBillingManager::DumpBuffer( BYTE* buffer, bool bError )
{
	JIF( buffer );
	JIF( sizeof( buffer ) >= 4 );

	unsigned short ulLength;
	::memcpy( &ulLength, buffer, sizeof( ulLength ) );

	//////////////////////////////////////////////////////////////////////////
	//START_LOG( clog2, L"[�׽�Ʈ�α�] ��Ŷ ������ ���!" )
	//	<< BUILD_LOG( ulLength );	
	//////////////////////////////////////////////////////////////////////////	

	ulLength = ::ntohl( ulLength );
	unsigned short ulTotalPacketSize = ulLength + 5;

	LIF( ulTotalPacketSize <= MAX_PACKET_SIZE_PLB );
	ulTotalPacketSize = std::min< int >( ulTotalPacketSize, MAX_PACKET_SIZE_PLB );

	char szBuffer[MAX_PACKET_SIZE_PLB * 3 + 1];
	char szByte[4];

	for( unsigned short i = 0; i < ulTotalPacketSize; i++ )
	{
		if( ( int )buffer[i] >= 16 )
		{
			::itoa( ( int )buffer[i], szByte, 16 );
			szBuffer[i * 3] = szByte[0];
			szBuffer[i * 3 + 1] = szByte[1];
			szBuffer[i * 3 + 2] = ' ';
		}
		else
		{
			::itoa( ( int )buffer[i], szByte, 16 );
			szBuffer[i * 3] = '0';
			szBuffer[i * 3 + 1] = szByte[0];
			szBuffer[i * 3 + 2] = ' ';
		}
	}
	szBuffer[ulTotalPacketSize * 3] = '\0';

	std::string strBuffer = szBuffer;

	if( bError )
	{
		START_LOG( cerr2, L"BUFFER : ")
			<< BUILD_LOG( strBuffer );
	}
	else
	{
		START_LOG( clog2, L"BUFFER : ")
			<< BUILD_LOG( strBuffer );
	}
}

void KPayletterBillingManager::InsertPacketNoUserUID( unsigned long ulPacketNo, UidType iUserUID )
{
	//KLocker lock( m_csPacketNoUserUID );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"���� UID �̻�." )
			<< BUILD_LOG( ulPacketNo )
			<< BUILD_LOG( iUserUID )
			<< END_LOG;

		return;
	}

	KCSLOCK_BEGIN( m_mapPacketNoUserUID )
		std::map< unsigned long, UidType >::iterator mit;
		mit = m_mapPacketNoUserUID.find( ulPacketNo );
		if( mit == m_mapPacketNoUserUID.end() )
		{
			// ������ �����ϰ� ������ ���� ����.
			m_mapPacketNoUserUID[ulPacketNo] = iUserUID;
		}
		else
		{
			START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID �� �̹� ��ϵǾ� �ִ�." )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOG( mit->second )
				<< BUILD_LOG( ulPacketNo )
				<< BUILD_LOG( iUserUID )
				<< END_LOG;
		}
	KCSLOCK_END()
}

void KPayletterBillingManager::DeletePacketNoUserUID( unsigned long ulPacketNo )
{
	//KLocker lock( m_csPacketNoUserUID );

	KCSLOCK_BEGIN( m_mapPacketNoUserUID )
		std::map< unsigned long, UidType >::iterator mit;
		mit = m_mapPacketNoUserUID.find( ulPacketNo );
		if( mit != m_mapPacketNoUserUID.end() )
		{
			m_mapPacketNoUserUID.erase( mit );
		}
		else
		{
			START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID �� ��ϵǾ� ���� �ʴ�." )
				<< BUILD_LOG( ulPacketNo )
				<< END_LOG;
		}
	KCSLOCK_END()
}

UidType KPayletterBillingManager::GetCorrespondingUserUID( unsigned long ulPacketNo )
{
	//KLocker lock( m_csPacketNoUserUID );

	UidType iUserUID = 0;

	KCSLOCK_BEGIN( m_mapPacketNoUserUID )
		std::map< unsigned long, UidType >::iterator mit;
		mit = m_mapPacketNoUserUID.find( ulPacketNo );
		if( mit != m_mapPacketNoUserUID.end() )
		{
			iUserUID = mit->second;
		}
		else
		{
			START_LOG( cerr, L"��Ŷ ��ȣ�� �ش��ϴ� UserUID �� ��ϵǾ� ���� �ʴ�." )
				<< BUILD_LOG( ulPacketNo )
				<< END_LOG;
		}
	KCSLOCK_END()

	return iUserUID;
}

unsigned long KPayletterBillingManager::GetNextPacketNo()
{
	//KLocker lock( m_csCurrentPacketNo );

	unsigned long ulCurrentPacketNo;

	KCSLOCK_BEGIN( m_ulCurrentPacketNo )
		m_ulCurrentPacketNo++;
		if( m_ulCurrentPacketNo > 0xfffffff0 )
		{
			m_ulCurrentPacketNo = 1;
		}
		ulCurrentPacketNo = m_ulCurrentPacketNo;
	KCSLOCK_END()

	return ulCurrentPacketNo;
}

//////////////////////////////////////////////////////////////////////////
// �����ϱ� 
#ifdef SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
void KPayletterBillingManager::InsertBuyPacket( IN const KEBILL_BUY_PRODUCT_REQ kPacket_, OUT KEBILL_BUY_PRODUCT_REQ& kPacketDeleted_, OUT bool& bRet_ )
#else 
void KPayletterBillingManager::InsertBuyPacket( IN const KEBILL_BUY_PRODUCT_REQ kPacket_, OUT bool& bRet_ )
#endif //SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
{
	KCSLOCK_BEGIN( m_mapBuyPacket )
		MAP_BUYPACKET_ITOR itor = m_mapBuyPacket.find( kPacket_.m_iUserUID );
		if( m_mapBuyPacket.end() == itor )
		{
			KPayletterBuyPacket kBuyPacket(CTime::GetCurrentTime(), kPacket_);
			m_mapBuyPacket.insert(std::make_pair(kPacket_.m_iUserUID, kBuyPacket));
			bRet_ = true;
		}
		else
		{
			CTime ctDeleteTime = itor->second.m_ctTime + CTimeSpan(0,0,PLBE_PACKET_DELETE_MIN,0);
			if( CTime::GetCurrentTime() < ctDeleteTime )
			{
				START_LOG( cerr, L"[����] �������� ���� ��Ŷ�� �̹� �ִ�. ó�� ���� ��..." )
					<< BUILD_LOG( kPacket_.m_iUserUID )
					<< BUILD_LOG( kPacket_.m_iServerGroupID )
					<< END_LOG;
				bRet_ = false;
			}
			else
			{
				// ó�� ���� �Ѱ� �ð��� ������. ��Ŷ�� ����� ���� ó������. ���� ��Ŷ ������ �α׷� �����?
				START_LOG( cout, L"[����] �������� ���� ��Ŷ�� ����� ���� �ִ´�" )
					<< BUILD_LOG( kPacket_.m_iUserUID )
					<< BUILD_LOG( kPacket_.m_iServerGroupID )
					<< END_LOG;
				
#ifdef SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
				// �α׸� ���ؼ� ���� ��Ŷ ���� �ѱ�ϴ�.
				kPacketDeleted_ = itor->second.m_kPacket;
#endif //SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG

				KPayletterBuyPacket kBuyPacket(CTime::GetCurrentTime(), kPacket_);
				itor->second = kBuyPacket;

				bRet_ = true;
			}
		}
	KCSLOCK_END()
}

void KPayletterBillingManager::GetBuyPacket( IN const UidType iUserUID_, OUT KEBILL_BUY_PRODUCT_REQ& kPacket_, OUT bool& bRet_ )
{
	KCSLOCK_BEGIN( m_mapBuyPacket )
		MAP_BUYPACKET_ITOR itor = m_mapBuyPacket.find(iUserUID_);
		if(itor != m_mapBuyPacket.end())
		{
			kPacket_ = (KEBILL_BUY_PRODUCT_REQ)itor->second.m_kPacket;		
			m_mapBuyPacket.erase(itor);
			
			bRet_ = true;
		}
		else
		{
			// ���� ��Ŷ ������ ���� �־�״� ��Ŷ�� �������? �߻��ϸ� �ȵ�
			START_LOG( cerr, L"[����] ���� ��Ŷ ������ �Դµ�, �����ص״� ��Ŷ�� �������? �߻��ϸ� �ȵ�" )
				<< BUILD_LOG( iUserUID_ )
				<< END_LOG;

			bRet_ = false;
		}
	KCSLOCK_END()
}

void KPayletterBillingManager::EraseBuyPacket( IN const UidType iUserUID_ )
{
	KEBILL_BUY_PRODUCT_REQ tempPacket;

	KCSLOCK_BEGIN( m_mapBuyPacket )
		MAP_BUYPACKET_ITOR itor = m_mapBuyPacket.find(iUserUID_);

		if(itor != m_mapBuyPacket.end())
		{
			//tempPacket = (KEBILL_BUY_PRODUCT_REQ)itor->second;		
			m_mapBuyPacket.erase(itor);
		}
		else
		{
			START_LOG( cerr, L"[����] ���� ��Ŷ�� ���� �Ϸ��µ�, ��Ŷ�� ����" )
				<< BUILD_LOG( iUserUID_ )
				<< END_LOG;
		}
	KCSLOCK_END()
}



//////////////////////////////////////////////////////////////////////////
// �����ϱ� 
#ifdef SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
void KPayletterBillingManager::InsertGiftPacket( IN const KEBILL_GIFT_ITEM_REQ kPacket_, OUT KEBILL_GIFT_ITEM_REQ& kPacketDeleted_, OUT bool& bRet_ )
#else //SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
void KPayletterBillingManager::InsertGiftPacket( IN const KEBILL_GIFT_ITEM_REQ kPacket_, OUT bool& bRet_ )
#endif //SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
{
	KCSLOCK_BEGIN( m_mapGiftPacket )
		MAP_GIFTPACKET_ITOR itor = m_mapGiftPacket.find( kPacket_.m_iSenderUserUID );
		if( m_mapGiftPacket.end() == itor)
		{
			KPayletterGiftPacket kGiftPacket(CTime::GetCurrentTime(), kPacket_);
			m_mapGiftPacket.insert(std::make_pair(kPacket_.m_iSenderUserUID, kGiftPacket));
			bRet_ = true;
		}
		else
		{
			CTime ctDeleteTime = itor->second.m_ctTime + CTimeSpan(0,0,PLBE_PACKET_DELETE_MIN,0);
			if( CTime::GetCurrentTime() < ctDeleteTime )
			{
				START_LOG( cerr, L"[����] �������� ���� ��Ŷ�� �̹� �ִ�. ó�� ���� ��..." )
					<< BUILD_LOG( kPacket_.m_iSenderUserUID )
					<< BUILD_LOG( kPacket_.m_iServerGroupID )
					<< END_LOG;
				bRet_ = false;
			}
			else
			{
				// ó�� ���� �Ѱ� �ð��� ������. ��Ŷ�� ����� ���� ó������. ���� ��Ŷ ������ �α׷� �����?
				START_LOG( cout, L"[����] �������� ���� ��Ŷ�� ����� ���� �ִ´�" )
					<< BUILD_LOG( kPacket_.m_iSenderUserUID )
					<< BUILD_LOG( kPacket_.m_iServerGroupID )
					<< END_LOG;
				
#ifdef SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
				kPacketDeleted_ = itor->second.m_kPacket;
#endif //SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG

				KPayletterGiftPacket kGiftPacket(CTime::GetCurrentTime(), kPacket_);
				itor->second = kGiftPacket;

				bRet_ = true;
			}
		}
	KCSLOCK_END()
}

void KPayletterBillingManager::GetGiftPacket( IN const UidType iUserUID_, OUT KEBILL_GIFT_ITEM_REQ& kPacket_, OUT bool& bRet_ )
{
	KCSLOCK_BEGIN( m_mapGiftPacket )
		MAP_GIFTPACKET_ITOR itor = m_mapGiftPacket.find(iUserUID_);

		if(itor != m_mapGiftPacket.end())
		{
			kPacket_ = (KEBILL_GIFT_ITEM_REQ)itor->second.m_kPacket;		
			m_mapGiftPacket.erase(itor);

			bRet_ = true;
		}
		else
		{
			// ���� ��Ŷ ������ ���� �־�״� ��Ŷ�� �������? �߻��ϸ� �ȵ�
			START_LOG( cerr, L"[����] ���� ��Ŷ ������ �Դµ�, �����ص״� ��Ŷ�� �������? �߻��ϸ� �ȵ�" )
				<< BUILD_LOG( iUserUID_ )
				<< END_LOG;

			bRet_ = false;
		}
	KCSLOCK_END()
}

void KPayletterBillingManager::EraseGiftPacket( IN const UidType iUserUID_ )
{
	KEBILL_GIFT_ITEM_REQ tempPacket;

	KCSLOCK_BEGIN( m_mapGiftPacket )
		MAP_GIFTPACKET_ITOR itor = m_mapGiftPacket.find(iUserUID_);

	if(itor != m_mapGiftPacket.end())
	{
		//tempPacket = (KEBILL_GIFT_ITEM_REQ)itor->second;		
		m_mapGiftPacket.erase(itor);
	}
	else
	{
		START_LOG( cerr, L"[����] ���� ��Ŷ�� ���� �Ϸ��µ�, ��Ŷ�� ����" )
			<< BUILD_LOG( iUserUID_ )
			<< END_LOG;
	}
	KCSLOCK_END()
}

//////////////////////////////////////////////////////////////////////////
// ���� ����ϱ� 
#ifdef SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
void KPayletterBillingManager::InsertCouponPacket( IN const KEBILL_USE_COUPON_REQ kPacket_, OUT KEBILL_USE_COUPON_REQ& kPacketDeleted_, OUT bool& bRet_ )
#else //SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
void KPayletterBillingManager::InsertCouponPacket( IN const KEBILL_USE_COUPON_REQ kPacket_, OUT bool& bRet_ )
#endif //SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
{
	KCSLOCK_BEGIN( m_mapCouponPacket )
	MAP_COUPONPACKET_ITOR itor = m_mapCouponPacket.find( kPacket_.m_PurchaserInfo.m_iUserUID );
	if( m_mapCouponPacket.end() == itor )
	{
		KPayletterCouponPacket kCouponPacket(CTime::GetCurrentTime(), kPacket_);
		m_mapCouponPacket.insert(std::make_pair(kPacket_.m_PurchaserInfo.m_iUserUID, kCouponPacket));
		bRet_ = true;
	}
	else
	{
		CTime ctDeleteTime = itor->second.m_ctTime + CTimeSpan(0,0,PLBE_PACKET_DELETE_MIN,0);
		if( CTime::GetCurrentTime() < ctDeleteTime )
		{
			START_LOG( cerr, L"[����] �������� ������� ��Ŷ�� �̹� �ִ�. ó�� ���� ��..." )
				<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUserUID )
				<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iServerGroupID )
				<< END_LOG;
			bRet_ = false;
		}
		else
		{
			// ó�� ���� �Ѱ� �ð��� ������. ��Ŷ�� ����� ���� ó������. ���� ��Ŷ ������ �α׷� �����?
			START_LOG( cout, L"[����] �������� ������� ��Ŷ�� ����� ���� �ִ´�" )
				<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUserUID )
				<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iServerGroupID )
				<< END_LOG;
#ifdef SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
			kPacketDeleted_ = itor->second.m_kPacket;
#endif //SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG

			KPayletterCouponPacket kCouponPacket(CTime::GetCurrentTime(), kPacket_);
			itor->second = kCouponPacket;

			bRet_ = true;
		}
	}
	KCSLOCK_END()
}

void KPayletterBillingManager::GetCouponPacket( IN const UidType iUserUID_, OUT KEBILL_USE_COUPON_REQ& kPacket_, OUT bool& bRet_ )
{
	KCSLOCK_BEGIN( m_mapCouponPacket )
		MAP_COUPONPACKET_ITOR itor = m_mapCouponPacket.find(iUserUID_);
		if(itor != m_mapCouponPacket.end())
		{
			kPacket_ = (KEBILL_USE_COUPON_REQ)itor->second.m_kPacket;		
			m_mapCouponPacket.erase(itor);

			bRet_ = true;
		}
		else
		{
			// ���� ��Ŷ ������ ���� �־�״� ��Ŷ�� �������? �߻��ϸ� �ȵ�
			START_LOG( cerr, L"[����] ������� ��Ŷ ������ �Դµ�, �����ص״� ��Ŷ�� �������? �߻��ϸ� �ȵ�" )
				<< BUILD_LOG( iUserUID_ )
				<< END_LOG;

			bRet_ = false;
		}
	KCSLOCK_END()
}

void KPayletterBillingManager::EraseCouponPacket( IN const UidType iUserUID_ )
{
	KEBILL_USE_COUPON_REQ tempPacket;

	KCSLOCK_BEGIN( m_mapCouponPacket )
	MAP_COUPONPACKET_ITOR itor = m_mapCouponPacket.find(iUserUID_);

	if(itor != m_mapCouponPacket.end())
	{
		//tempPacket = (KEBILL_USE_COUPON_REQ)itor->second;		
		m_mapCouponPacket.erase(itor);
	}
	else
	{
		START_LOG( cerr, L"[����] ���� ��Ŷ�� ���� �Ϸ��µ�, ��Ŷ�� ����" )
			<< BUILD_LOG( iUserUID_ )
			<< END_LOG;
	}
	KCSLOCK_END()
}


#endif //SERV_COUNTRY_US