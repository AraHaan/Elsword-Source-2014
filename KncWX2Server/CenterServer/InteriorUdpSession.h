#pragma once
#include "ServerDefine.h"

//{{ 2013. 2. 5	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
#include "..\Common\Socket\KTDNUDP.h"
#include <thread/thread.h>
#include <KNCSingleton.h>

// �ϴ��� ������ ���� �뵵�θ� ����Ѵ�.
// �ٸ� ������ �ְ� ���� �ʿ䰡 ����ٸ�, �����ϵ��� ����.

// KTDNUDP���� Relay ��Ŷ�� ���� �������� UnitUID�� ������ �˷��ִ� �͵� �� ���� ���
// �׷��� �Ǹ� ��� ó���� �̰����� �ϰ� �ȴ�.
class KInteriorUdpSession
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK    
//    : public KThread
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK    
{
	DeclInstanceKeeping( KInteriorUdpSession );

public:
	KInteriorUdpSession();
	virtual ~KInteriorUdpSession(void);

	void	Init( IN u_short usTargetPort );

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	// derived from KThread
//	void	Run( void );
//	void	ShutDown( void );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK    

	// send info
	void	DataPushBack( IN const CKTDNUDP::LB_EVENT_ID eEventID, IN const __int64 iRoomUID, IN const __int64 iUnitUID ){	m_vecStorage.push_back( CKTDNUDP::LB_INFO_STRUCT( eEventID, iRoomUID, iUnitUID ) );	}
	void	SendAndFlush( void );


//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	__forceinline	bool	IsLocal( IN const u_long ulCheckIP )
//	{
//		if( ( ulCheckIP != m_ulLocalIP1 ) &&
//			( ulCheckIP != m_ulLocalIP2 )
//			)
//		{
//			return false;
//		}
//		return true;
//	}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

private:
	void	OnFrameMove( void );
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	bool	Recv( void );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	std::vector<CKTDNUDP::LB_INFO_STRUCT>	m_vecStorage;

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//	SOCKET					m_Socket;
//	u_long					m_ulLocalIP1;
//	u_long					m_ulLocalIP2;
//	WSAEVENT				m_RecvEvent;
//	char					m_pRecvBuffer[RECV_BUFFER_SIZE];
//	char					m_pUncompressBuffer[RECV_BUFFER_SIZE];
//	std::vector<CKTDNUDP::LB_INFO_STRUCT>	m_RecvDataList;
//
//	// send info
//	SOCKADDR_IN				m_DesAddr;
//	char					m_pSendBuffer[RECV_BUFFER_SIZE];
//	char					m_pCompressBuffer[RECV_BUFFER_SIZE];
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
};
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}
