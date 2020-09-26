#pragma once

#include "RefreshSingleton.h"
#include "DBLayer.h"
#include <KncSend.h>
#include <boost/timer.hpp>
#include "CommonPacket.h"


//{{ 2011. 12. 14	������	��Ŷ ó�� ���� �ð� üũ
#ifdef SERV_PACKET_PROCESSING_LATENCY_CHECK	// ���� ������ �ؿ��� �߰�


class KPacketMornitoring
{
	DeclareRefreshSingleton( KPacketMornitoring );
	DeclareLuaScriptParser;		
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

	struct SPacketLatencyInfo
	{
		UidType		m_iUnitUID;
        DWORD		m_dwMaxTickCount;
		CTime		m_tMaxCurTime;

		SPacketLatencyInfo()
		{
			Clear();
		}

		void Clear()
		{
			m_iUnitUID = 0;
			m_dwMaxTickCount = 0;
			m_tMaxCurTime = CTime::GetCurrentTime();
		}
	};

	struct SPacketCountInfo
	{
		DWORD		m_dwProcessingCount;

		SPacketCountInfo()
		{
			Clear();
		}

		void Clear()
		{
			m_dwProcessingCount = 0;
		}
	};

public:
	KPacketMornitoring(void);
	~KPacketMornitoring(void);	

	virtual void Tick();

	// �ʱ�ȭ
	void Clear();

	// ��Ŷ ���� �ð� üũ
	void CheckPacketProcessingLatency( IN const u_short usEventID, IN const UidType iUnitUID, IN const DWORD dwTickCount );
	void CheckLoginProxyPacketProcessingCount( IN const u_short usEventID );
	void CheckCnProxyPacketProcessingCount( IN const u_short usEventID );
	void CheckGlobalProxyPacketProcessingCount( IN const u_short usEventID );

	// DB�� �α� ������Ʈ
	void BackUpLogToDB();

protected:
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void SendToLogDB( unsigned short usEventID );

private:
	SPacketLatencyInfo	m_arrPacketLatency[EVENT_EVENTID_SENTINEL];
	SPacketCountInfo	m_arrLoginProxyPacketCount[EVENT_EVENTID_SENTINEL];
	SPacketCountInfo	m_arrCnProxyPacketCount[EVENT_EVENTID_SENTINEL];
	SPacketCountInfo	m_arrGlobalProxyPacketCount[EVENT_EVENTID_SENTINEL];
	boost::timer		m_tDBUpdateTimer;
};

DefRefreshSingletonInline( KPacketMornitoring );

template < class T >
void KPacketMornitoring::SendToLogDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { 0, -1 };

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PC_LOG_DB, anTrace, usEventID, data );
	LIF( spEvent->m_kDestPerformer.AddUID( 0 ) );

	SiKDBLayer()->QueueingEvent( spEvent );
}


#endif SERV_PACKET_PROCESSING_LATENCY_CHECK	// ���� ������ �ؿ��� �߰�
//}}


