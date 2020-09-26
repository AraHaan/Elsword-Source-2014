//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER

#pragma once

#include <KNCSingleton.h>

//{{ 2009. 6. 10  ������	Ÿ�̸� �Ŵ���
#include "TimerManager.h"
//}}

//{{ 2012. 03. 03	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
SmartPointer(KGSUser);
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

class KGSWorldMissionManager
{
	DeclareSingleton( KGSWorldMissionManager );

public:

	//{{ 2012. 03. 03	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	enum WORLD_BUFF_START_CONDITION
	{
		WBSC_STEP_NONE = 0,
		WBSC_STEP_ONE = 3,
		WBSCL_STEP_TWO = 9,
		WBSC_STEP_THREE = 15,
	};

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

	int GetWorldCrystalCount( void );

	bool GetDefenseDungeonEndTime( OUT CTime& tStartTime, OUT CTime& tEndTime ) const;
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	KGSWorldMissionManager(void);
	~KGSWorldMissionManager(void);

	void Init();

	void UpdateMissionInfo( IN const KEGB_GET_MISSION_INFO_ACK& kPacketAck );
	void GetMissionInfo( OUT KEGS_WORLD_MISSION_UPDATE_NOT &kOut );
	void UpdateMissionStartTime( IN std::wstring& wstrDate );
	
	int GetRemainTime();
	int GetMissionStartRemainTime();

	int GetTickCount()	{ return m_iTickCount; }

	bool GetIsActive() { return m_bActive; }
	bool UpdateWorldBuffInfo( IN const KEGB_UPDATE_WORLD_BUFF_NOT& kPacket );
	void GetWorldBuff( OUT KEGS_UPDATE_WORLD_BUFF_NOT& kPacket );
	int GetWorldBuffState();
	
	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	int GetWorldBuffEndTime();
#endif SERV_SERVER_BUFF_SYSTEM
	//}

	//{{ 2013. 04. 15	�ڼ���	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	bool UpdateIndividualBuffInfo( IN const KEGB_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_NOT& kPacket_ );
	int GetIndividualBuffID( void ) const{	return m_iIndividualBuffID;	}
#else // SERV_NEW_DEFENCE_DUNGEON	// �ؿ��� ����
	void GetIndividualBuffID( OUT int& iBuffLevel, std::vector<int>& vecDeleteBuff );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

protected:
	template < class T > void SendToGlobalServer( unsigned short usEventID, const T& data );

private:
	bool	m_bActive;

	int		m_iDungeonTargetCount;
	int		m_iWorldCrystalCount;
	//{{ 2013. 04. 15	�ڼ���	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	int		m_iWorldMaxCrystalCount;
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

	int		m_iMissionTime;

	int		m_iDungeonPlayTime;	//	���� 1ȸ �÷��� Ÿ��(���� : ��)

	int		m_iTickCount;
	
	KTimerManager	m_kTimer;

	u_short		m_usWorldBuff;
	u_short		m_usWorldBuffDurationTime;

	CTime		m_tWorldMissionEndTime;
	CTime		m_tWorldMissionNextStartTime;

	//{{ 2012. 03. 23	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	std::map< int, std::vector< KDefenseOpenTime > > m_mapDefenseOpenTime;	// ���Ϻ� ���� �ð�
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	//{{ 2013. 04. 15	�ڼ���	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	int			m_iIndividualBuffID;
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
};

DefSingletonInline( KGSWorldMissionManager );

template < class T >
void KGSWorldMissionManager::SendToGlobalServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GLOBAL_USER, 0, NULL, usEventID, data );
}

#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
