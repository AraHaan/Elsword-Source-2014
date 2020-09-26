#include <WinSock2.h>
#include "BRPayletterBillingManager.h"

#ifdef SERV_COUNTRY_BR

#include "Socket/NetCommon.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "BRPayletterBilling.h"
#include "Simlayer.h"

#include "_BRGameProtocol.h"

//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}

DWORD KBRPayletterBillingManager::ms_dwHeartBeatGap( 1 * 60 * 1000 );
DWORD KBRPayletterBillingManager::ms_dwConnectionCheckGap( 20 * 1000 );

ImplToStringW( KBRPayletterBillingManager )
{
	return stm_;
}

ImplPfID( KBRPayletterBillingManager, PI_GS_PUBLISHER_BILLING );
ImplementSingleton( KBRPayletterBillingManager );


KBRPayletterBillingManager::KBRPayletterBillingManager() :
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

	m_spEvent.reset( CreateEvent( NULL, false, false, NULL ), CloseHandle );
}

KBRPayletterBillingManager::~KBRPayletterBillingManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

void KBRPayletterBillingManager::RegToLua()
{
	lua_tinker::class_add<KBRPayletterBillingManager>( g_pLua, "KPayletterManager" );
	lua_tinker::class_def<KBRPayletterBillingManager>( g_pLua, "InitPayletterBillingInfo",	&KBRPayletterBillingManager::InitPayletterBillingInfo );
	lua_tinker::class_def<KBRPayletterBillingManager>( g_pLua, "SetNumThread",			 &KBRPayletterBillingManager::Init );

	lua_tinker::decl( g_pLua, "PayletterBillingManager", this );
}

void KBRPayletterBillingManager::InitPayletterBillingInfo( const char* szNexonAuthIP, unsigned short usPort, int iDomain )
{
	m_kPayletterBillingInfo.m_strIP		= szNexonAuthIP;
	m_kPayletterBillingInfo.m_usPort	= usPort;
	m_kPayletterBillingInfo.m_iDomain	= iDomain;	
}


void KBRPayletterBillingManager::Tick()
{	
}

void KBRPayletterBillingManager::Init( int nThreadNum )
{
	m_ulRecvCP = 0;

	//////////////////////////////////////////////////////////////////////////
	// thread setting : recv�� recvfrom() �Լ����� �˾Ƽ� block�ȴ�.
	m_spThreadRecv = boost::shared_ptr< KTThread< KBRPayletterBillingManager > >
		( new KTThread< KBRPayletterBillingManager >( *this, &KBRPayletterBillingManager::Recv, 50 ) );

	m_spThreadSend = boost::shared_ptr< KTThread< KBRPayletterBillingManager > >
		( new KTThread< KBRPayletterBillingManager >( *this, &KBRPayletterBillingManager::Send, 100 ) );

	//{{ 2009. 11. 21  ������	���������ӱ�������
	m_spThreadKeepConnect = boost::shared_ptr< KTThread< KBRPayletterBillingManager > >
		( new KTThread< KBRPayletterBillingManager >( *this, &KBRPayletterBillingManager::KeepConnectionThread, 1000 ) );
	//}}

	KThreadManager::Init( nThreadNum );	
}

	

KThread* KBRPayletterBillingManager::CreateThread()
{
	return new KBRPayletterBilling;
}

void KBRPayletterBillingManager::BeginThread()
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

void KBRPayletterBillingManager::EndThread()
{
	//{{ 2009. 11. 21  ������	���������ӱ�������
	if( m_spThreadKeepConnect )
	{
		m_spThreadKeepConnect->End( 3000 );

		START_LOG( cout, L"����� ���̷��� ���� ���� ���� üũ ������ ����!" );
	}
	//}}

	KThreadManager::EndThread();

	if( m_spThreadSend )
	{
		m_spThreadSend->End( 10000 );

		START_LOG( cout, L"����� ���̷��� ���� ���� Send ������ ����!" );
	}

	// recv ������ ���̱� ���� ������ ����!
	CLOSE_SOCKET( m_sock );

	if( m_spThreadRecv )
	{
		m_spThreadRecv->End( 3000 );

		START_LOG( cout, L"����� ���̷��� ���� ���� Recv ������ ����!" );
	}
}

bool KBRPayletterBillingManager::Connect()
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

	
	// KENX_BR_INITIALIZE_REQ�� ť���ϱ� ���� false �� ������ �Ѵ�.
	// ���� : �׽�Ʈ - �̰� ������ ���� ���� ��� �� ��� �ϴ� ��. �׷��Ƿ� ������ true ��
	m_bInitialized = true;	// true �� ����

	if( !m_bInitialized )
	{
		CLOSE_SOCKET( m_sock );
		return false;
	}
	
	return true;
}

void KBRPayletterBillingManager::Recv()
{

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

		return;
	}

	if( ret == 0 )
	{
		START_LOG( cerr, L"���� ������ ������." )
			<< END_LOG;
		return;
	}

	
	m_ulRecvCP += ret;

	while( m_ulRecvCP >= 6 )
	{
		// ���(9) + Ÿ��(1) = 6
		// ��� ��Ŷ�� 6����Ʈ �̻���
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

void KBRPayletterBillingManager::Send()
{	
	if( !IsConnected() )
	{
		return;
	}
	
	int ret;
	char buf[MAX_PACKET_SIZE_PLB];
	KBRPayletterBillingPacketPtr spPacket;
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
void KBRPayletterBillingManager::KeepConnectionThread()
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

}
//}}

void KBRPayletterBillingManager::KeepConnection()
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

}


void KBRPayletterBillingManager::CheckConnection()
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

void KBRPayletterBillingManager::HandleInitializeAck( bool bSucceed )
{
	m_bInitialized = bSucceed;
	SetEvent( m_spEvent.get() );
}


void KBRPayletterBillingManager::QueueingSendPacket( const KBRPayletterBillingPacketPtr& spPacket )
{	
	JIF( spPacket );
	
	KCSLOCK_BEGIN( m_kSendQueue )
		m_kSendQueue.push( spPacket );
	KCSLOCK_END()
}
bool KBRPayletterBillingManager::IsConnected()
{
	return ( m_sock != INVALID_SOCKET );
}


void KBRPayletterBillingManager::OnDisconnectBillingServer( IN const std::wstring& wstrReason )
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

bool KBRPayletterBillingManager::GetSendPacket( KBRPayletterBillingPacketPtr& spPacket )
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

void KBRPayletterBillingManager::MakeEventFromReceivedPacket()
{
	DumpBuffer( ( BYTE* )m_cRecvBuffer, false );
	KBRPayletterBillingPacket kPacket;
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
		K##EPL_BR_##name##_ACK kPacketAck; \
		kPacket.Read( kPacketAck ); \
		KEventPtr spEvent( new KEvent ); \
		spEvent->SetData( PI_GS_PUBLISHER_BILLING, NULL, EPL_BR_##name##_ACK, kPacketAck ); \
		QueueingEvent( spEvent ); \
		} \
		break;
#   include "BRPayletterBilling_def.h"
	default:
		START_LOG( cerr, L"��Ŷ Ÿ���� �̻���." )
			<< BUILD_LOG( kPacket.GetPacketType() )
			<< END_LOG;
		DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
		break;
	}
}

void KBRPayletterBillingManager::DumpBuffer( BYTE* buffer, bool bError )
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

void KBRPayletterBillingManager::InsertUniqueKeyUserUID( UidType iUniqueKey, UidType iUserUID )
{
	if( iUniqueKey <= 0 || iUserUID <= 0 )
	{
		START_LOG( cerr, L"UniqueKey �Ǵ� ���� UID �̻� " )
			<< BUILD_LOG( iUniqueKey )
			<< BUILD_LOG( iUserUID )
			<< END_LOG;

		return;
	}

	KCSLOCK_BEGIN( m_mapUniqueKeyUserUID )
	std::map< UidType, UidType >::iterator mit;
	mit = m_mapUniqueKeyUserUID.find( iUniqueKey );
	if( mit == m_mapUniqueKeyUserUID.end() )
	{
		// ������ �����ϰ� ������ ���� ����.
		m_mapUniqueKeyUserUID[iUniqueKey] = iUserUID;
	}
	else
	{
		START_LOG( cerr, L"�ۺ��� ����ũ Ű�� �ش��ϴ� UserUID �� �̹� ��ϵǾ� �ִ�." )
			<< BUILD_LOG( mit->first )
			<< BUILD_LOG( mit->second )
			<< BUILD_LOG( iUniqueKey )
			<< BUILD_LOG( iUserUID )
			<< END_LOG;
	}
	KCSLOCK_END()
}

void KBRPayletterBillingManager::DeleteUniqueKeyUserUID( UidType iUniqueKey )
{
	KCSLOCK_BEGIN( m_mapUniqueKeyUserUID )
	std::map< UidType, UidType >::iterator mit;
	mit = m_mapUniqueKeyUserUID.find( iUniqueKey );
	if( mit != m_mapUniqueKeyUserUID.end() )
	{
		m_mapUniqueKeyUserUID.erase( mit );
	}
	else
	{
		START_LOG( cerr, L"�ۺ��� ����ũ Ű�� �ش��ϴ� UserUID �� ��ϵǾ� ���� �ʴ�." )
			<< BUILD_LOG( iUniqueKey )
			<< END_LOG;
	}
	KCSLOCK_END()
}

UidType KBRPayletterBillingManager::GetUniqueKeyUserUID( UidType iUniqueKey )
{
	UidType iUserUID = 0;

	KCSLOCK_BEGIN( m_mapUniqueKeyUserUID )
	std::map< UidType, UidType >::iterator mit;
	mit = m_mapUniqueKeyUserUID.find( iUniqueKey );
	if( mit != m_mapUniqueKeyUserUID.end() )
	{
		iUserUID = mit->second;
	}
	else
	{
		START_LOG( cerr, L"�ۺ��� ����ũ Ű�� �ش��ϴ� UserUID �� ��ϵǾ� ���� �ʴ�." )
			<< BUILD_LOG( iUniqueKey )
			<< END_LOG;
	}
	KCSLOCK_END()

		return iUserUID;
}

//////////////////////////////////////////////////////////////////////////
// �����ϱ� 
void KBRPayletterBillingManager::InsertBuyPacket( IN KEBILL_BUY_PRODUCT_REQ kPacket_, OUT bool& bRet_ )
{
	KCSLOCK_BEGIN( m_mapBuyPacket )
		MAP_BUYPACKET_ITOR itor = m_mapBuyPacket.find( kPacket_.m_uiPublisherUID );
		if( m_mapBuyPacket.end() == itor )
		{
			KPayletterBuyPacket kBuyPacket(CTime::GetCurrentTime(), kPacket_);
			m_mapBuyPacket.insert(std::make_pair(kPacket_.m_uiPublisherUID, kBuyPacket));
			bRet_ = true;
		}
		else
		{
			CTime ctDeleteTime = itor->second.m_ctTime + CTimeSpan(0,0,PLBE_PACKET_DELETE_MIN,0);
			if( CTime::GetCurrentTime() < ctDeleteTime )
			{
				START_LOG( cerr, L"[����] �������� ���� ��Ŷ�� �̹� �ִ�. ó�� ���� ��..." )
					<< BUILD_LOG( kPacket_.m_uiPublisherUID )
					<< BUILD_LOG( kPacket_.m_iServerGroupID )
					<< END_LOG;
				bRet_ = false;
			}
			else
			{
				// ó�� ���� �Ѱ� �ð��� ������. ��Ŷ�� ����� ���� ó������. ���� ��Ŷ ������ �α׷� �����?
				START_LOG( cout, L"[����] �������� ���� ��Ŷ�� ����� ���� �ִ´�" )
					<< BUILD_LOG( kPacket_.m_uiPublisherUID )
					<< BUILD_LOG( kPacket_.m_iServerGroupID )
					<< END_LOG;

				KPayletterBuyPacket kBuyPacket(CTime::GetCurrentTime(), kPacket_);
				itor->second = kBuyPacket;

				bRet_ = true;
			}
		}
	KCSLOCK_END()
}

void KBRPayletterBillingManager::GetBuyPacket( IN const UidType iUniqueKey, OUT KEBILL_BUY_PRODUCT_REQ& kPacket_, OUT bool& bRet_ )
{
	bool bRet = false;
	KCSLOCK_BEGIN( m_mapBuyPacket )
		MAP_BUYPACKET_ITOR itor = m_mapBuyPacket.find(iUniqueKey);
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
				<< BUILD_LOG( iUniqueKey )
				<< END_LOG;

			bRet_ = false;
		}
	KCSLOCK_END()
}

void KBRPayletterBillingManager::EraseBuyPacket( IN const UidType iUniqueKey )
{
	KEBILL_BUY_PRODUCT_REQ tempPacket;

	KCSLOCK_BEGIN( m_mapBuyPacket )
		MAP_BUYPACKET_ITOR itor = m_mapBuyPacket.find(iUniqueKey);

		if(itor != m_mapBuyPacket.end())
		{
			m_mapBuyPacket.erase(itor);
		}
		else
		{
			START_LOG( cerr, L"[����] ���� ��Ŷ�� ���� �Ϸ��µ�, ��Ŷ�� ����" )
				<< BUILD_LOG( iUniqueKey )
				<< END_LOG;
		}
	KCSLOCK_END()
}

#endif //SERV_COUNTRY_BR