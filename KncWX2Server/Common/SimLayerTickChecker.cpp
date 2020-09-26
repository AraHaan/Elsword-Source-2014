#include "SimLayer.h"
#include "BaseServer.h"
#include "SimLayerTickChecker.h"
#include "ServerPacket.h"


//{{ 2013. 02. 	������	Tick�����ս� DB�α� ���
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG


KSimLayerTickChecker::KSimLayerTickChecker()
{
	Reset();
}

KSimLayerTickChecker::~KSimLayerTickChecker()
{
}

void KSimLayerTickChecker::Reset()
{
	m_dwSimLayerTick = ::GetTickCount();
	m_dwMaxTick = 0;
	m_tRegTime = CTime();
}

const std::wstring KSimLayerTickChecker::GetSimLayerMaxTickRegDate() const
{
	std::wstring wstrRegDate = ( std::wstring )( m_tRegTime.Format( _T( "%Y-%m-%d_%H%M%S" ) ) );
	return wstrRegDate;
}

void KSimLayerTickChecker::CheckSimLayerTick()
{
	const DWORD dwCurTickCount = ::GetTickCount();
	const DWORD dwDelayTick = dwCurTickCount - m_dwSimLayerTick;

	if( dwDelayTick > m_dwMaxTick )
	{
		m_dwMaxTick = dwDelayTick;
		m_tRegTime = CTime::GetCurrentTime();

		if( dwDelayTick > 30000 )
		{
			START_LOG( cout, L"[���!] SimLayer Tick �����ð��� 30�ʸ� �Ѿ���!" )
				<< BUILD_LOG( dwDelayTick );
		}
	}
	m_dwSimLayerTick = dwCurTickCount;
	
	// 10�и��� �α� ���
	if( m_tTimer.elapsed() > 600.0 )
	{
		m_tTimer.restart();

		CTime tCurTime = CTime::GetCurrentTime();

		KDBE_TICK_PERFORMANCE_LOG_NOT kPacketNot;
		kPacketNot.m_iServerUID = KBaseServer::GetKObj()->GetUID();
		kPacketNot.m_iServerType = KBaseServer::GetKObj()->GetServerClass();
		kPacketNot.m_dwMaxTickCount = m_dwMaxTick;
		kPacketNot.m_wstrMaxTickRegDate = ( std::wstring )( m_tRegTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		kPacketNot.m_wstrRegDate = ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		KncSend( PI_GS_SERVER, 0, PI_LOG_DB, 0, NULL, DBE_TICK_PERFORMANCE_LOG_NOT, kPacketNot );
	}
}


#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
//}}


