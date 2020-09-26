#pragma once

#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0

#include <Windows.h>
#include <MMSystem.h>
#include <vector>

#include "KOGGamePerformanceCheck.h"

//
class KLagCheck
{
public:
	KLagCheck( int iLagType );
	~KLagCheck();

	void AddChildLagTime( DWORD dwLagTime ) { m_dwChildLagTimeSum += dwLagTime; }
	DWORD GetChildLagTimeSum() { return m_dwChildLagTimeSum; }

private:
	KLagCheck() 
	{}

	bool m_bActive;
	int m_iCurGameSpace;
	int m_iCurGameType;

	int m_iLagType;
	DWORD m_dwStartTime;

	//
	DWORD m_dwChildLagTimeSum;
};

class KLagCheckManager
{
public:
	class KVillageInfo
	{
	public:
		KVillageInfo() { Init(); }

		void Init()
		{
			m_iLagType = -1;
			m_ePartyType = KOGGamePerformanceCheck::ePartyType_No;
			m_iPartyUserNum = -1;
			m_iMapID = -1;
			m_iVillageUserNum = -1;
			m_fLagTime = -1.0f;
			m_iLagTotalNum = -1;
			m_fLagTotalTime = -1.0f;
		}

		void operator=( const KVillageInfo& kSrc )
		{
			m_iLagType = kSrc.m_iLagType;
			m_ePartyType = kSrc.m_ePartyType;
			m_iPartyUserNum = kSrc.m_iPartyUserNum;
			m_iMapID = kSrc.m_iMapID;
			m_iVillageUserNum = kSrc.m_iVillageUserNum;
			m_fLagTime = kSrc.m_fLagTime;
			m_iLagTotalNum = kSrc.m_iLagTotalNum;
			m_fLagTotalTime = kSrc.m_fLagTotalTime;
		}

		void SetLagType( int iLagType ) { m_iLagType = iLagType; }
		int GetLagType() const { return m_iLagType; }

		void SetPartyType( KOGGamePerformanceCheck::EPartyType ePartyType ) { m_ePartyType = ePartyType; }
		KOGGamePerformanceCheck::EPartyType GetPartyType() const { return m_ePartyType; }

		void SetPartyUserNum( int iPartyUserNum ) { m_iPartyUserNum = iPartyUserNum; }
		int GetPartyUserNum() const { return m_iPartyUserNum; }

		void SetMapID( int iMapID ) { m_iMapID = iMapID; } 
		int GetMapID() const { return m_iMapID; }

		void SetVillageUserNum( int iVillageUserNum ) { m_iVillageUserNum = iVillageUserNum; }
		int GetVillageUserNum() const { return m_iVillageUserNum; }

		void SetLagTime( float fLagTime ) { m_fLagTime = fLagTime; }
		float GetLagTime() const { return m_fLagTime; } 

		void SetLagTotalNum( int iLagTotalNum ) { m_iLagTotalNum = iLagTotalNum; }
		int GetLagTotalNum() const { return m_iLagTotalNum; } 

		void SetLagTotalTime( float fLagTotalTime ) { m_fLagTotalTime = fLagTotalTime; }
		float GetLagTotalTime() const { return m_fLagTotalTime; } 

	private:
		int m_iLagType;		//lag type 
		KOGGamePerformanceCheck::EPartyType m_ePartyType;		//��ƼŸ��
		int m_iPartyUserNum;//��Ƽ������
		int m_iMapID;		//����ID 
		int m_iVillageUserNum;	//����������
		float m_fLagTime;		//�� �ɸ� �ð� 
		int m_iLagTotalNum;	//�� �� Ƚ��
		float m_fLagTotalTime;	//�� �� �ð�
	};

	class KFieldInfo
	{
	public:
		KFieldInfo() { Init(); }

		void Init()
		{
			m_iLagType = -1;
			m_ePartyType = KOGGamePerformanceCheck::ePartyType_No;
			m_iPartyUserNum = -1;
			m_iMapID = -1;
			m_iFieldUserNum = -1;
			m_fLagTime = -1.0f;
			m_iLagTotalNum = -1;
			m_fLagTotalTime = -1.0f;
		}

		void operator=( const KFieldInfo& kSrc )
		{
			m_iLagType = kSrc.m_iLagType;
			m_ePartyType = kSrc.m_ePartyType;
			m_iPartyUserNum = kSrc.m_iPartyUserNum;
			m_iMapID = kSrc.m_iMapID;
			m_iFieldUserNum = kSrc.m_iFieldUserNum;
			m_fLagTime = kSrc.m_fLagTime;
			m_iLagTotalNum = kSrc.m_iLagTotalNum;
			m_fLagTotalTime = kSrc.m_fLagTotalTime;
		}

		void SetLagType( int iLagType ) { m_iLagType = iLagType; }
		int GetLagType() const { return m_iLagType; }

		void SetPartyType( KOGGamePerformanceCheck::EPartyType ePartyType ) { m_ePartyType = ePartyType; }
		KOGGamePerformanceCheck::EPartyType GetPartyType() const { return m_ePartyType; }

		void SetPartyUserNum( int iPartyUserNum ) { m_iPartyUserNum = iPartyUserNum; }
		int GetPartyUserNum() const { return m_iPartyUserNum; }

		void SetMapID( int iMapID ) { m_iMapID = iMapID; } 
		int GetMapID() const { return m_iMapID; }

		void SetFieldUserNum( int iFieldUserNum ) { m_iFieldUserNum = iFieldUserNum; }
		int GetFieldUserNum() const { return m_iFieldUserNum; }

		void SetLagTime( float fLagTime ) { m_fLagTime = fLagTime; }
		float GetLagTime() const { return m_fLagTime; } 

		void SetLagTotalNum( int iLagTotalNum ) { m_iLagTotalNum = iLagTotalNum; }
		int GetLagTotalNum() const { return m_iLagTotalNum; } 

		void SetLagTotalTime( float fLagTotalTime ) { m_fLagTotalTime = fLagTotalTime; }
		float GetLagTotalTime() const { return m_fLagTotalTime; } 

	private:
		int m_iLagType;		//lag type 
		KOGGamePerformanceCheck::EPartyType m_ePartyType;		//��ƼŸ��
		int m_iPartyUserNum;//��Ƽ������
		int m_iMapID;		//����ID 
		int m_iFieldUserNum;	//�ʵ�������
		float m_fLagTime;		//�� �ɸ� �ð� 
		int m_iLagTotalNum;	//�� �� Ƚ��
		float m_fLagTotalTime;	//�� �� �ð�
	};

	class KDungeonInfo
	{
	public:
		KDungeonInfo() { Init(); }

		void Init()
		{
			m_iLagType = -1;
			m_ePartyType = KOGGamePerformanceCheck::ePartyType_No;
			m_iPartyUserNum = -1;
			m_iMapID = -1;
			m_iDifficulty = -1;
			m_iStage = -1;
			m_iBlock = -1;
			m_fLagTime = -1.0f;
			m_iLagTotalNum = -1;
			m_fLagTotalTime = -1.0f;
		}

		void operator=( const KDungeonInfo& kSrc )
		{
			m_iLagType = kSrc.m_iLagType;
			m_ePartyType = kSrc.m_ePartyType;
			m_iPartyUserNum = kSrc.m_iPartyUserNum;
			m_iMapID = kSrc.m_iMapID;
			m_iDifficulty = kSrc.m_iDifficulty;
			m_iStage = kSrc.m_iStage;
			m_iBlock = kSrc.m_iBlock;
			m_fLagTime = kSrc.m_fLagTime;
			m_iLagTotalNum = kSrc.m_iLagTotalNum;
			m_fLagTotalTime = kSrc.m_fLagTotalTime;
		}

		void SetLagType( int iLagType ) { m_iLagType = iLagType; }
		int GetLagType() const { return m_iLagType; }

		void SetPartyType( KOGGamePerformanceCheck::EPartyType ePartyType ) { m_ePartyType = ePartyType; }
		KOGGamePerformanceCheck::EPartyType GetPartyType() const { return m_ePartyType; }

		void SetPartyUserNum( int iPartyUserNum ) { m_iPartyUserNum = iPartyUserNum; }
		int GetPartyUserNum() const { return m_iPartyUserNum; }

		void SetMapID( int iMapID ) { m_iMapID = iMapID; } 
		int GetMapID() const { return m_iMapID; }

		void SetDifficulty( int iDifficulty ) { m_iDifficulty = iDifficulty; }
		int GetDifficulty() const { return m_iDifficulty; }

		void SetStage( int iStage ) { m_iStage = iStage; }
		int GetStage() const { return m_iStage; }

		void SetBlock( int iBlock ) { m_iBlock = iBlock; }
		int GetBlock() const { return m_iBlock; }

		void SetLagTime( float fLagTime ) { m_fLagTime = fLagTime; }
		float GetLagTime() const { return m_fLagTime; } 

		void SetLagTotalNum( int iLagTotalNum ) { m_iLagTotalNum = iLagTotalNum; }
		int GetLagTotalNum() const { return m_iLagTotalNum; } 

		void SetLagTotalTime( float fLagTotalTime ) { m_fLagTotalTime = fLagTotalTime; }
		float GetLagTotalTime() const { return m_fLagTotalTime; } 

	private:
		int m_iLagType;		//lag type 
		KOGGamePerformanceCheck::EPartyType m_ePartyType;		//��ƼŸ��
		int m_iPartyUserNum;		//�÷��������� 
		int m_iMapID;		//����ID 
		int m_iDifficulty;//�������̵�
		int m_iStage;		//Stage��ȣ 
		int m_iBlock;		//Block��ȣ 
		float m_fLagTime;		//�� �ɸ� �ð� 
		int m_iLagTotalNum;	//�� �� Ƚ��
		float m_fLagTotalTime;	//�� �� �ð�
	};

	class KPvPInfo
	{
	public:
		KPvPInfo() { Init(); }

		void Init()
		{
			m_iLagType = -1;
			m_iMapID = -1;
			m_iPvPUserNum = -1;
			m_iRuleType = -1;
			m_iRound = -1;
			m_iObserverNum = -1;
			m_fLagTime = -1.0f;
			m_iLagTotalNum = -1;
			m_fLagTotalTime = -1.0f;
		}

		void operator=( const KPvPInfo& kSrc )
		{
			m_iLagType = kSrc.m_iLagType;
			m_iMapID = kSrc.m_iMapID;
			m_iPvPUserNum = kSrc.m_iPvPUserNum;
			m_iRuleType = kSrc.m_iRuleType;
			m_iRound = kSrc.m_iRound;
			m_iObserverNum = kSrc.m_iObserverNum;
			m_fLagTime = kSrc.m_fLagTime;
			m_iLagTotalNum = kSrc.m_iLagTotalNum;
			m_fLagTotalTime = kSrc.m_fLagTotalTime;
		}

		void SetLagType( int iLagType ) { m_iLagType = iLagType; }
		int GetLagType() const { return m_iLagType; }

		void SetMapID( int iMapID ) { m_iMapID = iMapID; } 
		int GetMapID() const { return m_iMapID; }

		void SetPvPUserNum( int iPvPUserNum ) { m_iPvPUserNum = iPvPUserNum; } 
		int GetPvPUserNum() const { return m_iPvPUserNum; }
		
		void SetRuleType( int iRuleType ) { m_iRuleType = iRuleType; }
		int GetRuleType() const { return m_iRuleType; }

		void SetRound( int iRound ) { m_iRound = iRound; }
		int GetRound() const { return m_iRound; }

		void SetObserverNum( int iObserverNum ) { m_iObserverNum = iObserverNum; }
		int GetObserverNum() const { return m_iObserverNum; }

		void SetLagTime( float fLagTime ) { m_fLagTime = fLagTime; }
		float GetLagTime() const { return m_fLagTime; } 

		void SetLagTotalNum( int iLagTotalNum ) { m_iLagTotalNum = iLagTotalNum; }
		int GetLagTotalNum() const { return m_iLagTotalNum; } 

		void SetLagTotalTime( float fLagTotalTime ) { m_fLagTotalTime = fLagTotalTime; }
		float GetLagTotalTime() const { return m_fLagTotalTime; } 

	private:
		int m_iLagType;		//lag type 
		int m_iMapID;		//������ID 
		int m_iPvPUserNum;
		int m_iRuleType;	//��Ÿ��
		int m_iRound;		//Round��ȣ �Ǵ� ��ü Kill��
		int m_iObserverNum;		//���߼� 
		float m_fLagTime;		//�� �ɸ� �ð� 
		int m_iLagTotalNum;	//�� �� Ƚ��
		float m_fLagTotalTime;	//�� �� �ð�
	};

	//
	KLagCheckManager();
	~KLagCheckManager();

	void Init( DWORD dwLagTimeDefinition, int iLagTypeMax );

	//
	DWORD GetLagTimeDefinition() { return m_dwLagTimeDefinition; }

	//
	void SetActiveLagCheck( bool bActiveLagCheck ) { m_bActiveLagCheck = bActiveLagCheck; }
	bool GetActiveLagCheck() { return m_bActiveLagCheck; }

	//
	void IncDepthIndex() { m_iDepthIndex++; }
	void DecDepthIndex() { m_iDepthIndex--; }
	int GetDepthIndex() { return m_iDepthIndex; }
	//
	void SetLagCheck( int iDepthIndex, KLagCheck* pkLagCheck ) { m_vecLagCheck[iDepthIndex] =  pkLagCheck; }
	KLagCheck* GetLagCheck( int iDepthIndex ) { return m_vecLagCheck[iDepthIndex]; }

	//
	void ResetLagCheckVec();

	//üũ�� ����Ÿ������ �˻�.
	void Update( int iGameSpace, int iLagType, DWORD dwUpdateLagTime );
	
	void Start();
	void Pause() { m_bPause = true; m_bSkipOneFrame = true; }
	void Resume() 	{ m_bPause = false; }
	void End();

	//
	const KVillageInfo& GetUnKnownLagInfo_Village() { return m_kUnKnown_CurVillageLagInfo; }
	const KFieldInfo& GetUnKnownLagInfo_Field() { return m_kUnKnown_CurFieldLagInfo; }
	const KDungeonInfo& GetUnKnownLagInfo_Dungeon() { return m_kUnKnown_CurDungeonLagInfo; }
	const KPvPInfo& GetUnKnownLagInfo_PvP() { return m_kUnKnown_CurPvPLagInfo; }

	//
	const KVillageInfo& GetKnownLagInfo_Village() { return m_kKnown_CurVillageLagInfo; }
	const KFieldInfo& GetKnownLagInfo_Field() { return m_kKnown_CurFieldLagInfo; }
	const KDungeonInfo& GetKnownLagInfo_Dungeon() { return m_kKnown_CurDungeonLagInfo; }
	const KPvPInfo& GetKnownLagInfo_PvP() { return m_kKnown_CurPvPLagInfo; }

	//�÷��� �ð��� ����
	float GetPlayTime() { return m_fPlayTime; }

private:
	//
	DWORD m_dwLagTimeDefinition;

	//
	bool m_bActiveLagCheck;

	//
	std::vector< KLagCheck* > m_vecLagCheck;

	//���� depth index
	int m_iDepthIndex;

	//���� �˷����� ���� �� ����
	KVillageInfo m_kUnKnown_CurVillageLagInfo;
	KFieldInfo m_kUnKnown_CurFieldLagInfo;
	KDungeonInfo m_kUnKnown_CurDungeonLagInfo;
	KPvPInfo m_kUnKnown_CurPvPLagInfo;

	//���� �� ����
	KVillageInfo m_kKnown_CurVillageLagInfo;
	KFieldInfo m_kKnown_CurFieldLagInfo;
	KDungeonInfo m_kKnown_CurDungeonLagInfo;
	KPvPInfo m_kKnown_CurPvPLagInfo;

	//checking ����
	bool m_bChecking;
	//pause ����
	bool m_bPause;
	//1������ skip����
	bool m_bSkipOneFrame;

	//play ���� �ð�
	float m_fPlayStartTime;
	//���� play �ð�
	float m_fPlayTime;

public:
	void SetLagTestActive( bool bLagTestActive ) { m_bLagTestActive = bLagTestActive; }
	bool GetLagTestActive() { return m_bLagTestActive; }

	void SetLagTestTime( float fLagTestTime ) { m_fLagTestTime = fLagTestTime; }
	float GetLagTestTime() { return m_fLagTestTime; }

	void SetLagTestType( int iLagTestType ) { m_iLagTestType = iLagTestType; }
	int GetLagTestType() { return m_iLagTestType; }
private:
	//"������" ġƮ�� �׽�Ʈ �����ϵ���...
	bool m_bLagTestActive;
	float m_fLagTestTime;
	int m_iLagTestType;
};