//{{2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER

#pragma once

#include "RefreshSingleton.h"
#include "DBLayer.h"
#include <KncSend.h>
#include <atltime.h>
#include <boost/timer.hpp>
#include "CommonPacket.h"
#include "ServerPacket.h"

//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
#include "Lottery.h"
#endif SERV_EARTH_QUAKE_EFFECT
//}}

//{{ 2013. 04. 13	�ڼ���	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
class KWorldMissionManager
{
	DeclareRefreshSingleton( KWorldMissionManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	enum DEFENSE_REWARD_BUFF_LEVEL_NEEDS
	{
		DEFENSE_REWARD_BUFF_LEVEL1_NEEDS = 0,
		DEFENSE_REWARD_BUFF_LEVEL2_NEEDS = 3,
		DEFENSE_REWARD_BUFF_LEVEL3_NEEDS = 9,
		DEFENSE_REWARD_BUFF_LEVEL4_NEEDS = 15,
		DEFENSE_REWARD_BUFF_MAX_NEEDS = 16,
	};

public:
	KWorldMissionManager(void);
	~KWorldMissionManager(void);

	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );

	void	Tick( void );
	void	Init( void );

	void	SetMissionSuccess_LUA(bool bSuccess);	//	ġƮ��
	void	SetDefenseOpenTime_LUA( int iDayOfWeek, int iStartHour, int iStartMin, int iDurationTime );
	void	SetWorldBuffDurationTime_LUA( int iBuffDurationTime )		{ m_usWorldBuffDurationTime = static_cast<u_short>(iBuffDurationTime); }

	void	WorldMissionDump_LUA();
	
	//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
	void SetRandomEarthQuakeTime_LUA( int iSec, float fRate );
#endif SERV_EARTH_QUAKE_EFFECT
	//}}

	void	SetReduceTerm_LUA( float fReduceTerm ){	m_fReduceTerm = fReduceTerm;	}
	float	GetReduceTerm( void ) const{	return m_fReduceTerm;	}

	int		GetDayOfTheWeek( void ) const{	return m_tToDayMissionTime.GetDayOfWeek();	}
	bool	SetTodayMissionTime( void );
	void	CheckWorldMissionProcess( void );

	void	SetActive( IN const bool bActive ){ m_bActive = bActive; }
	bool	IsActive( void ) const{ return m_bActive; }

	void	InitMissionData( void );
	void	SetMissionData( void );
	void	StartMission( IN const bool IsReStart = false );
	void	EndMission( void );

	void	BroadCastMission( IN const KEGB_UPDATE_MISSION_INFO_NOT::FLAG_TYPE eType );
	void	BroadCastWorldBuff( IN const int _skill );
	void	BroadCastMissionStartTime( void );

	void	IncreaseProtectedCrystalCount( IN const int iProtectedCrystalCount );
	int		GetProtectedCrystalCount( void ) const{	return m_iProtectedCrystalCount;	}
	void	ResetProtectedCrystalCount( void );
	
	void	GetWorldMissionInfo( IN const int& iType, OUT KEGB_GET_MISSION_INFO_ACK &kPacketAck ) const;

	void	SetIsFirstTick( IN const bool IsFirst ){	m_bIsFirstTick = IsFirst;	}
	bool	GetIsFirstTick( void ) const{	return m_bIsFirstTick;	}
	int		GetCorrectMissionTime( void ) const;
	CTime	GetToDayMissionTime( void ) const{	return m_tToDayMissionTime;	}

	void	WriteWorldMissionCrystalCount( IN const bool bforce = false );
	void	SetInitMissionInfo( IN const KDBE_GET_DEFENCE_CRYSTAL_ACK& KPacket );

	void	SetMissionTime( IN const int iTime ){	m_iMissionTime = iTime;	}
	bool	GetWorldMissionState( void ) const{	return m_bWorldMissionOn;	}

	void	BroadCastIndividualBuffLevel( IN const int& iIndividualBuffLevel ) const;

	int		CheckWorldBuff( void ) const;
	int		CheckIndividualBuff( void ) const;

	void	BuffProcess( void );

	//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
	int  GetRandomEarthQuakeTime();
#endif SERV_EARTH_QUAKE_EFFECT
	//}}

private:
	bool			m_bIsFirstTick;

	boost::timer	m_kTimerDOT;			//	�������� �ð� üũ�� ���� Ÿ�̸�
	boost::timer	m_kTimerEnableCheck;	//	���� �ֱ� üũ�� ���� Ÿ�̸�
	boost::timer	m_kTimerUpdate;			//	1�п� �ѹ��� ���Ӽ����� ������� ����
	boost::timer	m_kTimerWorldBuff;		//	���� ���� ���� �ð� ��
	boost::timer	m_kTimerForReduceTheProtectedCrystalCount;		//	�д� �Ҹ�Ǵ� ī��Ʈ�� ���� Ÿ�̸�

	bool			m_bActive;				//	���� ���� ����Ǿ� ���� ����
	bool			m_bWorldBuffTime;		//  ���� ���� �ð� ��
	int				m_iMissionTime;		//	����̼� ����ð�(���� : ��)
	int				m_iNextDOT;			//	������ ����� ���� ���ð�(���� : ��)
	int				m_iTotalCCU;

	//	���� ���� �̼�
	int				m_iStartedCrystalCount;
	int				m_iProtectedCrystalCount;	//  ��ȣ�� ��ü ���� ��
	int				m_iReduceCount;				//	�д� �Ҹ�Ǵ� ī��Ʈ	: m_iTotalCCU / 60

	CTime													m_tToDayMissionTime;	// ��ũ������� ���� ���� �ð�
	CTime													m_tToDayStartTime;		// ���� ���� ���µ� �ð�
	std::map< int, std::vector< KDefenseOpenTime > >		m_mapDefenseOpenTime;	// ���Ϻ� ���� �ð�
	bool													m_bWorldMissionOn;		// ���� �̼��� �ϴ°�?

	u_short			m_usWorldBuffDurationTime;	//  ���� ���� ���� �ð�

	int				m_iWorldBuffID;
	int				m_iIndividualBuffID;

	//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
	KLottery					m_kEarthQuakeLottery;
#endif SERV_EARTH_QUAKE_EFFECT
	//}}

	float			m_fReduceTerm;
};
#else
class KWorldMissionManager
{
	DeclareRefreshSingleton( KWorldMissionManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:

	enum WORLD_BUFF_START_CONDITION
	{
		WBSC_STEP_NONE = 0,
		WBSC_STEP_ONE = 3,
		WBSCL_STEP_TWO = 9,
		WBSC_STEP_THREE = 15,
	};

	//{{ 2012. 04. 2	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	enum DEFENSE_REWARD_BUFF_LEVEL_NEEDS
	{
		DEFENSE_REWARD_BUFF_LEVEL1_NEEDS = 0,
		DEFENSE_REWARD_BUFF_LEVEL2_NEEDS = 3,
		DEFENSE_REWARD_BUFF_LEVEL3_NEEDS = 9,
		DEFENSE_REWARD_BUFF_LEVEL4_NEEDS = 15,
		DEFENSE_REWARD_BUFF_MAX_NEEDS = 16,
	};

	enum GATE_OF_THE_DARKNESS_DIFF
	{
		GATE_OF_THE_DARKNESS_HARD	= 7,
		GATE_OF_THE_DARKNESS_EXPERT = 13,
	};
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	KWorldMissionManager(void);
	~KWorldMissionManager(void);

	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );

	void	Tick();

	void	Init();

	void	SetDungeonPlayTime_LUA(int iTime)		{ m_iDungeonPlayTime = iTime; }
	void	SetMissionSuccess_LUA(bool bSuccess);	//	ġƮ��

	void	SetDefenseOpenTime_LUA( int iDayOfWeek, int iStartHour, int iStartMin, int iDurationTime );
	void	SetWorldMissionOn_LUA( bool bOn )			{ m_bWorldMissionOn = bOn; 	}
	
	void	SetNormalPlayRate_LUA(float fRate)			{ m_fNormalPlayRate = fRate; }
	void	SetHardPlayRate_LUA(float fRate)			{ m_fHardPlayRate = fRate; }
	void	SetExpertPlayRate_LUA(float fRate)			{ m_fExpertPlayRate = fRate; }

	void	SetNormalSuccessRate_LUA(float fRate)		{ m_fNormalSuccessRate = fRate; }
	void	SetHardSuccessRate_LUA(float fRate)			{ m_fHardSuccessRate = fRate; }
	void	SetExpertSuccessRate_LUA(float fRate)		{ m_fExpertSuccessRate = fRate; }

	void	SetWorldBuffDurationTime_LUA( int iBuffDurationTime )		{ m_usWorldBuffDurationTime = static_cast<u_short>(iBuffDurationTime); }

	void	SetNormalBasicCrystalCount_LUA( char cCrystal )				{ m_cNormalBasicCrystalCount = cCrystal; }
	void	SetHardBasicCrystalCount_LUA( char cCrystal )				{ m_cHardBasicCrystalCount = cCrystal; }
	void	SetExpertBasicCrystalCount_LUA( char cCrystal )				{ m_cExpertBasicCrystalCount = cCrystal; }

	void	BuffCheat_LUA( int iBuffType, bool bForce );
	void	WorldMissionDump_LUA();

	int		GetDayOfTheWeek()		{ return m_tToDayMissionTime.GetDayOfWeek(); }
	bool	SetTodayMissionTime();
	void	CheckWorldMissionProcess();

	void	Reset_LUA();	//	ġƮ��

	void	SetActive( bool bActive )				{ m_bActive = bActive; }
	bool	IsActive()								{ return m_bActive; }

	void	InitMissionData();
	void	SetMissionData();
	void	StartMission(bool IsReStart = false );
	void	EndMission();

	void	BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FLAG_TYPE eType );
	void	BroadCastWorldBuff( int _skill, bool bIsEnd = false );
	void	BroadCastMissionStartTime();

	int		IncreaseProtectedCrystalCount( int ProtectedCrystalCount );
	int		IncreaseWorldCrystalCount()													{ return ++m_iWorldCrystalCount; }

	int		GetProtectedCrystalCount()													{ return m_iProtectedCrystalCount; }
	int		GetWorldCrystalCount();

	//{{ 2012. 04. 24	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	void	ResetProtectedCrystalCount( void );
	void	GetWorldMissionInfo( IN const int& iType, OUT KEGB_GET_MISSION_INFO_ACK &kPacketAck );
#else
	void	GetWorldMissionInfo( OUT KEGB_GET_MISSION_INFO_ACK &kPacketAck );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}
	
	void	SetIsFirstTick( bool IsFirst )			{ m_bIsFirstTick = IsFirst; }
	bool	GetIsFirstTick()						{ return m_bIsFirstTick; }
	void	CheckWorldCrystalCount( bool flag = false );
	int		GetCorrectMissionTime();
	CTime	GetToDayMissionTime()					{ return m_tToDayMissionTime; }

	void	WriteWorldMissionCrystalCount( bool bforce = false );
	void	SetInitMissionInfo( KDBE_GET_DEFENCE_CRYSTAL_ACK KPacket );

	void	SetMissionTime( int iTime );
	bool	GetWorldMissionState()					{ return m_bWorldMissionOn; }

	//{{ 2012. 03. 23	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	void BroadCastIndividualBuffLevel( void ) const;
	void BroadCastIndividualBuffLevel( IN const int& iIndividualBuffLevel ) const;
	void IndividualBuffEndProcess( void );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
	void SetRandomEarthQuakeTime_LUA( int iSec, float fRate );
	int  GetRandomEarthQuakeTime();
#endif SERV_EARTH_QUAKE_EFFECT
	//}}

private:
	bool			m_bIsFirstTick;

	boost::timer	m_kTimerDOT;			//	�������� �ð� üũ�� ���� Ÿ�̸�
	boost::timer	m_kTimerEnableCheck;	//	���� �ֱ� üũ�� ���� Ÿ�̸�
	boost::timer	m_kTimerUpdate;			//	1�п� �ѹ��� ���Ӽ����� ������� ����
	boost::timer	m_kTimerWorldBuff;		//	���� ���� ���� �ð� ��

	bool			m_bActive;				//	���� ���� ����Ǿ� ���� ����
	bool			m_bWorldBuffTime;		//  ���� ���� �ð� ��

	int				m_iMissionTime;		//	����̼� ����ð�(���� : ��)

	int				m_iNextDOT;			//	������ ����� ���� ���ð�(���� : ��)
	
	int				m_iDungeonPlayTime;	//	���� 1ȸ �÷��� Ÿ��(���� : ��)

	int				m_iTotalCCU;

	//	���� ���� �̼�
	int				m_iCrystalTargetCount;		//	��ȣ ���� ��ǥ ��
	int				m_iProtectedCrystalCount;	//  ��ȣ�� ��ü ���� ��
	float			m_fCountPerWorldCrystal;	//  ���� ũ����Ż �ϳ��� ������ ��   m_iCrystalTargetCount = m_fCountPerWorldCrystal * m_iWorldCrystalCount
	int				m_iWorldCrystalCount;		//	��ȣ�� ���� ũ����Ż ��   x / 15

	CTime													m_tToDayMissionTime;	// ��ũ������� ���� ���� �ð�
	CTime													m_tToDayStartTime;		// ���� ���� ���µ� �ð�
	std::map< int, std::vector< KDefenseOpenTime > >		m_mapDefenseOpenTime;	// ���Ϻ� ���� �ð�
	bool													m_bWorldMissionOn;		// ���� �̼��� �ϴ°�?

	float			m_fNormalPlayRate;			//	���� ������
	float			m_fHardPlayRate;			//	����� ������
	float			m_fExpertPlayRate;			//	�ſ����� ������

	float			m_fNormalSuccessRate;		//	���� ������
	float			m_fHardSuccessRate;			//	����� ������
	float			m_fExpertSuccessRate;		//	�ſ����� ������

	char			m_cNormalBasicCrystalCount;	//  ���� �⺻ ����(���� ��)
	char			m_cHardBasicCrystalCount;	//  ����� �⺻ ����(���� ��)
	char			m_cExpertBasicCrystalCount;	//  �ſ� ����� �⺻ ����(���� ��)

	u_short			m_usSuccessWorldBuff;		//  ����� ���� ����
	u_short			m_usWorldBuffDurationTime;	//  ���� ���� ���� �ð�

	//{{ 2012. 05. 03	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	bool			m_bRewardBuff;				// ����� �� ������ ���� ����
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
	KLottery					m_kEarthQuakeLottery;
#endif SERV_EARTH_QUAKE_EFFECT
	//}}
};
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

DefRefreshSingletonInline( KWorldMissionManager );

template < class T >
void KWorldMissionManager::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOG_DB, 0, NULL, usEventID, data );
}

#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
