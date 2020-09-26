#pragma once

#ifdef SERV_GLOBAL_MISSION_MANAGER

#include "RefreshSingleton.h"
#include "DBLayer.h"
#include <KncSend.h>
#include <atltime.h>
#include <boost/timer.hpp>
#include "CommonPacket.h"
#include "ServerPacket.h"

class KGlobalMissionManager
{
	DeclareRefreshSingleton( KGlobalMissionManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:

	KGlobalMissionManager(void);
	~KGlobalMissionManager(void);

	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );

	void	Tick();

	void	Init();


	void	SetGlobalMissionSuccess_LUA( bool bSuccess, std::wstring wstrGlobalMissionDay );	//	ġƮ��
	void	SetGlobalMissionOn_LUA( bool bOn )		{ m_bGlobalMissionOn = bOn; }
	
	void	GlobalMissionDump_LUA();

	bool	SetTodayMissionTime();
	void	CheckGlobalMissionProcess();
	void	Reset_LUA();	//	ġƮ��
	
	bool	GetGlobalMissionState()					{ return m_bGlobalMissionOn; }
	void	InitMissionData();
	void	SetMissionData();
	void	StartMission( std::wstring wstrGlobalMissionDay, bool IsReStart = false );
	void	EndMission( std::wstring wstrGlobalMissionDay );
	void	SetInitMissionInfo( KDBE_GET_GLOBAL_MISSION_INFO_ACK kPacket );
	int		GetCorrectMissionTime( std::wstring wstrGlobalMissionDay );

	void	BroadCastGlobalMission( KEGB_UPDATE_GLOBAL_MISSION_INFO_NOT::FLAG_TYPE eType, std::wstring wstrGlobalMissionDay );
	void	BroadCastGlobalMissionStartTime();

	int		IncreaseGlobalMissionClearCount( std::wstring& wstrGlobalMissionDay, int iClearCount );
	void	GetGlobalMissionInfo( IN const int& iType, OUT KEGB_GET_GLOBAL_MISSION_INFO_ACK &kPacketAck );

	void	SetIsFirstTick( bool IsFirst )			{ m_bIsFirstTick = IsFirst; }
	bool	GetIsFirstTick()						{ return m_bIsFirstTick; }

	void	WriteGlobalMissionInfo();
	//void	WriteGlobalMissionRewardInfo( KGlobalMissionInfo* pkGlobalMissionInfo, bool bSuccess );

private:
	bool			m_bIsFirstTick;
	bool			m_bReceiveGlobalMissionInfo;

	boost::timer	m_kTimerMissionStart;				//	���� �۷ι� �̼� ���� �ð� üũ�� ���� Ÿ�̸�
	boost::timer	m_kTimerEnableCheck;				//	���� �ֱ� üũ�� ���� Ÿ�̸�
	boost::timer	m_kTimerUpdate;						//	���� ���� ���� ������Ʈ�� Ÿ�̸�
	
	std::map< std::wstring, KGlobalMissionInfo >	m_mapGlobalMissionInfo;					// �۷ι� �̼Ǻ� ���� ����
	bool											m_bGlobalMissionOn;						// �۷ι� �̼��� �ϴ°�?
	int												m_iNextGlobalMissionStartRemainTime;	// ���� �۷ι� �̼��� ������ �����Ŀ� ���۵� ���ΰ�?
};

DefRefreshSingletonInline( KGlobalMissionManager );

template < class T >
void KGlobalMissionManager::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOG_DB, 0, NULL, usEventID, data );
}

#endif SERV_GLOBAL_MISSION_MANAGER
