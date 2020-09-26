#pragma once

#include "StdAfx.h"

#ifdef SERV_PET_SYSTEM

//#include "./X2Pet.h"

class CX2PETAI
{
public:
	enum NPC_AI_TYPE
	{
		NAT_NORMAL,			// 
		NAT_ALLY,			// ��ȯ�� ģ�� NPC
		NAT_NO_BRAIN,
	};

	enum NPC_AI_STATE
	{
		NAS_WAIT,
		NAS_CHASE,
		NAS_PATROL,
		NAS_FLEE,
		NAS_RALLY,
		NAS_SIEGE, 
	};

	enum TARGET_PRIORITY
	{
		TP_RANDOM,
		TP_ONLY_ONE,				
		TP_HIGH_LEVEL_FIRST,
		TP_LOW_LEVEL_FIRST,
		TP_HIGH_HP_FIRST,
		TP_LOW_HP_FIRST,
		TP_NEAR_FIRST,
		TP_FAR_FIRST,
	};

	struct TargetData
	{
		bool				targetNPC;
		TARGET_PRIORITY		targetPriority;
		float 				targetInterval;
		int					targetNearRange;	// �� ���� �ȿ� ������ ������ Ÿ���� �ȴ�
		int					targetRange;		// �� ���� �ȿ� ������ Ÿ���� �ȴ�
		int					targetLostRange;	// �� ���� ������ ����� ���� Ÿ���� �Ұ� �ȴ�.
		float				targetSuccessRate;	
		float				attackTargetRate;	// ���� ������ ������ Ÿ������ Ȯ��
		float				preserveLastTargetRate;
		string				strLuaTargetingFunc;

		TargetData()
		{
			targetNPC				= false;
			targetPriority			= TP_LOW_HP_FIRST;
			targetInterval			= 0.5;
			targetNearRange			= 200;
			targetRange				= 10000;
			targetLostRange			= 2000;
			targetSuccessRate		= 100;
			attackTargetRate		= 100;
			preserveLastTargetRate	= 100;
			strLuaTargetingFunc	= "";
		}
	};

	struct ChaseMoveData
	{
		int			moveSplitRange;
		int			destGap;			// ���������� �̰Ÿ� �ȿ� ������ �����ߴٰ� �Ǵ��Ѵ�
		int			moveGap;
		int			runGap;

		float		dirChangeInterval;

		float		walkInterval;
		float		nearWalkRate;
		float		farWalkRate;

		float		jumpInterval;
		float		upJumpRate;
		float		upDownRate;
		float		downJumpRate;
		float		downDownRate;

		float		m_fLineEndDetectRange;
		bool		bOnlyThisLineGroup;
	};
		
	

#ifdef AI_FLY
	struct FlyMoveData
	{
		float fDestGap;
		float fDestHeight;
		float fDestArea;
		float fFlyMoveInterval;
	};
#endif

	struct AIData
	{
		TargetData		targetData;
		ChaseMoveData	chaseMoveData;		
		
#ifdef AI_FLY
		FlyMoveData		flymovedata;
#endif
	};


	
public:
	CX2PETAI( CX2PET* pPet );
	~CX2PETAI(void);

	virtual void OnFrameMove( double fTime, float fElapsedTime );
	virtual void LoadAIDataFromLUA( KLuaManager& luaManager );

	void TargetingNPC( float fElapsedTime );
	void Targeting( float fElapsedTime );
	void Moving( float fElapsedTime );

	D3DXVECTOR3 GetFinalDest() const { return m_vFinalDest; }
	bool	GetMasterIsRight() { return m_bMasterIsRight; }


	void SetAITargetNPC( bool bTargetNPC ) { m_pAIData->targetData.targetNPC = bTargetNPC; }
	bool GetEnableLuaTargetingFunc() const { return m_bEnableLuaTargetingFunc; }
	void SetEnableLuaTargetingFunc(bool val) { m_bEnableLuaTargetingFunc = val; }
	const std::string& GetLuaTargetingFunc() const { return m_strLuaTargetingFunc; }
	void SetLuaTargetingFunc( const std::string& val) { m_strLuaTargetingFunc = val; }

#ifdef AI_FLY
	float GetFlyDestHieght() { return m_pAIData->flymovedata.fDestHeight; }
#endif

	void SetMasterUnitData();
	void UpdateJumpSpeed();

	int GetLostRange() { return m_pAIData->targetData.targetLostRange; }

protected:
	void TargetUpdate();
	
		
private:	
	AIData*			m_pAIData;
	
	int				m_iFinalDestLineIndex;			// ���� �������� linedata index
	D3DXVECTOR3		m_vFinalDest;					// ���� ������


	bool			m_bLostTarget;
	D3DXVECTOR3		m_vPivotPoint;				// �� ���� �߽����� patrol�ϰų� guard, wander
	int				m_iPivotPointLineIndex;




	// MasterNPC�� ���� ����
	D3DXVECTOR3		m_vPosition;
	int				m_iLastTouchLineIndex;
	bool			m_bIsRight;
	bool			m_bFootOnLine;
	float			m_fMaxJumpUp;
	float			m_fMaxJumpRight;	
	float			m_fNowHP;
	float			m_fMaxHP;

	bool			m_bMasterIsRight;

	bool			m_bEnableLuaTargetingFunc;
	string			m_strLuaTargetingFunc;

public:
	bool GetEnabled() const { return m_bEnabled; }
	void SetEnabled(bool val) { m_bEnabled = val; }

	UidType GetTargetUnitUID() const { return m_TargetUnitUID; }
	void SetTargetUnitUID(UidType val) { m_TargetUnitUID = val; }	

	CX2PETAI::NPC_AI_STATE GetAIState() const { return m_eAIState; }
	void SetAIState(CX2PETAI::NPC_AI_STATE val) { m_eAIState = val; }

public:
	CX2PET::STATE_CHANGE_TYPE GetPrevAIMessage() const { return m_PrevAIMessage; }
	void SetPrevAIMessage(CX2PET::STATE_CHANGE_TYPE val) { m_PrevAIMessage = val; }

	bool GetFleeing() const { return m_bFleeing; }
	void SetFleeing(bool val) { m_bFleeing = val; }

	bool IsTargetFront() const { return m_bIsTargetFront; }
	bool IsTargetAccquired() 
	{ 
		if( -1 != m_TargetUnitUID ) 
			return true; 
		else 
			return false; 
	}

#ifdef AI_FLY
	void SetAIFly(bool val) 
	{ 
		m_bFlyUnit = val;
		m_bNowStateFly = val;
	}
	bool GetAIFly() { return m_bFlyUnit; }
	void SetAiFlyNowState(bool val) { m_bNowStateFly = val; }
	bool GetAIFlyNowState() { return m_bNowStateFly; }	
#endif

	bool GetNoBrain() const { return m_bNoBrain; }
	void SetNoBrain( bool bNoBrain ) { m_bNoBrain = bNoBrain; }

	//void SetMonsterUid( UidType iMonsterUid	) { m_MonsterUid = iMonsterUid; }
	//UidType GetMonsterUid() { return m_MonsterUid; }

	void			ResetGameUnitAttackMeOrBeAttackedByMe() { m_optrGameUnitAttackMeOrBeAttackedByMe.Reset(); }
	void			SetGameUnitAttackMeOrBeAttackedByMe( CX2GameUnit* pGameUnit_ ) 
	{ 
		if ( NULL != pGameUnit_ )
			m_optrGameUnitAttackMeOrBeAttackedByMe = pGameUnit_; 
	}
	CX2GameUnit*	GetGameUnitAttackMeOrBeAttackedByMe() const { return ( null != m_optrGameUnitAttackMeOrBeAttackedByMe ? m_optrGameUnitAttackMeOrBeAttackedByMe.GetObservable() : NULL ); }

protected:
	CX2SimplePathFinder		m_PathFinder;

	bool					m_bNoBrain; // AI �� �۵����� �ʵ��� ��ũ��Ʈ���� �����Ѵ�	
	bool					m_bEnabled;
	CX2PET*					m_pPet;


protected:	
	NPC_AI_STATE			m_eAIState;

	UidType							m_TargetUnitUID;

protected:
	CX2PET::STATE_CHANGE_TYPE		m_PrevAIMessage;

	bool			m_bIsTargetFront;
	bool			m_bFleeing;				// ���� ���� ������ ��Ÿ���� ����


	float 			m_fElapsedTimeAfterLastMoving;			// �Ȱų� �ٴ� ������ ������Ʈ �ð����� ����
	float 			m_fElapsedTimeAfterLastTargeting;

	float			m_fElapsedTimeAfterLastRandomJump;		// �����ϰ� �Ʒ�, ���� �����ϴ� ���� ��Ʈ�� �ϴ� ����
	float			m_fElapsedTimeAfterLastWalkRunUpdate;
	float			m_fElapsedTimeAfterLastDirChange;
	float			m_fElapsedPatrolCoolingTime;


#ifdef AI_FLY
	float			m_fElapsedTimeAfterLastFlyMoveUpdate;	
	bool			m_bFlyUnit;
	bool			m_bNowStateFly;
#endif

	UidType			m_MasterUid;
	//UidType			m_MonsterUid;	// ���� ������ ���� or ���� ������ ���� ( ���� �Ǵ� ������ �� �� �ִ�. )
	CX2GameUnitoPtr		m_optrGameUnitAttackMeOrBeAttackedByMe;		// ���� ������ ���� or ���� ������ ���� ( ���� �Ǵ� ������ �� �� �ִ�. )
};
#endif

