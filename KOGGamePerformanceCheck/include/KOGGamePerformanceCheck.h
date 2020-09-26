#pragma once

#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0

#include <Windows.h>

class KAvgFpsCheckManager;
class KPingCheckManager;
class KUDPTransCheckManager;
class KNonHostNpcReactionCheckManager;
class KLagCheckManager;
class KMemUsageCheckManager;

class KOGGamePerformanceCheck
{
public:
	static KOGGamePerformanceCheck* GetInstance();
	static void DeleteInstance();

private:
	static KOGGamePerformanceCheck* m_pkInstance;

public:
	enum EPartyType
	{
		ePartyType_No = 0,
		ePartyType_Host = 1,
		ePartyType_Member = 2,
		ePartyType_Observer = 3,
	};

	class KVillageInfo
	{
	public:
		KVillageInfo()
		{
			m_ePartyType = KOGGamePerformanceCheck::ePartyType_No;
			m_iPartyUserNum = -1;
			m_iMapID = -1;
			m_iVillageUserNum = -1;
		}

		KOGGamePerformanceCheck::EPartyType m_ePartyType;		//��ƼŸ��
		int m_iPartyUserNum;//��Ƽ������
		int m_iMapID;		//����ID 
		int m_iVillageUserNum;	//����������
	};

	class KFieldInfo
	{
	public:
		KFieldInfo()
		{
			m_ePartyType = KOGGamePerformanceCheck::ePartyType_No;
			m_iPartyUserNum = -1;
			m_iMapID = -1;
			m_iFieldUserNum = -1;
		}

		KOGGamePerformanceCheck::EPartyType m_ePartyType;		//��ƼŸ��
		int m_iPartyUserNum;//��Ƽ������
		int m_iMapID;		//����ID 
		int m_iFieldUserNum;	//�ʵ�������
	};

	class KDungeonInfo
	{
	public:
		KDungeonInfo()
		{
			m_ePartyType = KOGGamePerformanceCheck::ePartyType_No;
			m_iPartyUserNum = -1;
			m_iMapID = -1;
			m_iDifficulty = -1;
			m_bEscape = true;
			m_iStage = -1;
			m_iBlock = -1;
		}

		KOGGamePerformanceCheck::EPartyType m_ePartyType;		//��ƼŸ��
		int m_iPartyUserNum;//��Ƽ������
		int m_iMapID;		//����ID 
		int m_iDifficulty;//�������̵�
		bool m_bEscape;	//�����÷��� �� ������������ ����
		int m_iStage;		//Stage��ȣ 
		int m_iBlock;		//Block��ȣ
	};

	class KPvPInfo
	{
	public:
		KPvPInfo()
		{
			m_iMapID = -1;
			m_iPvPUserNum = -1;
			m_iRuleType = -1;
			m_bEscape = true;
			m_iRound = -1;
			m_iObserverNum = -1;
		}

		int m_iMapID;		//������ID 
		int m_iPvPUserNum;	//����������
		int m_iRuleType;	//��Ÿ��
		bool m_bEscape;	//�����÷��� �� ������������ ����
		int m_iRound;		//Round��ȣ �Ǵ� ��ü Kill��
		int m_iObserverNum;		//���߼� 
	};

	KOGGamePerformanceCheck();
	~KOGGamePerformanceCheck();

	void Init( float fAvgFpsUpdateTime,//��� 
				int iUdpPingLossWindowSize,//UDP
				int iUdpPingTimeOut,//UDP
				int iMinSampleNum,//�ּ� ���� ��
				float fMemUsageUpdateTime, //�޸�
				DWORD dwLagTimeDefinition, int iLagTypeMax//�� 
				);

	void Start( bool bUdpPing_MainThread, bool bTcpPing_MainThread );
	void Pause();
	void Resume();
	void End();

	//
	KAvgFpsCheckManager* GetAvgFpsCheckMgr() { return m_pkAvgFpsCheckManager; }
	KPingCheckManager* GetP2PPingCheckMgr() { return m_pkP2PPingCheckManager; }
	KPingCheckManager* GetRelayPingCheckMgr() { return m_pkRelayPingCheckManager; }
	KPingCheckManager* GetUdpPingCheckMgr() { return m_pkUdpPingCheckManager; }
	KPingCheckManager* GetTcpPingCheckMgr() { return m_pkTcpPingCheckManager; }
	KUDPTransCheckManager* GetUDPTransCheckMgr() { return m_pkUdpTransCheckManager; }
	KNonHostNpcReactionCheckManager* GetNonHostNpcReactionCheckMgr() { return m_pkNonHostNpcReactionCheckManager; }
	KMemUsageCheckManager* GetMemUsageCheckMgr() { return m_pkMemUsageCheckManager; }
	KLagCheckManager* GetLagCheckMgr() { return m_pkLagCheckManager; }

private:
	//
	KAvgFpsCheckManager* m_pkAvgFpsCheckManager;
	KPingCheckManager* m_pkP2PPingCheckManager;
	KPingCheckManager* m_pkRelayPingCheckManager;
	KPingCheckManager* m_pkUdpPingCheckManager;
	KPingCheckManager* m_pkTcpPingCheckManager;
	KUDPTransCheckManager* m_pkUdpTransCheckManager;
	KNonHostNpcReactionCheckManager* m_pkNonHostNpcReactionCheckManager;
	KLagCheckManager* m_pkLagCheckManager;
	KMemUsageCheckManager* m_pkMemUsageCheckManager;
};