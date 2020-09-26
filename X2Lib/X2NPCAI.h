#pragma once


// �ٽɸ�ǥ�� ���Ŀ� ��� ����, �߰��� �����ϰ� flexible�ϰ� stable�� AI �ڵ常���

// ���� �ӵ� ���͸� ���� sync�� ������
// ������ ����!! ���!! ������!! �ڹ�!! 
// ���Ͱ� �ϴ� �ൿ, ����, ���, ����, ???

// REINFORCEMENT RULE



#ifdef REDUCED_PRECOMPILED_HEADER_TEST
#include "./X2GameUnit.h"
#include "./X2GUNPC.h"
#endif REDUCED_PRECOMPILED_HEADER_TEST



class CX2NPCAI
{
public:
	enum NPC_AI_TYPE
	{
		NAT_NORMAL,			// 
		NAT_ALLY,			// ��ȯ�� ģ�� NPC
		NAT_NO_BRAIN,
		NAT_FIELD,			/// �ʵ� Ÿ��
	};

	// ���ο� Ÿ���� ���� �� TARGET_RANGE ���� �ִ� ���� �߿��� �켱 ����
	enum TARGET_PRIORITY
	{
		TP_RANDOM,				/// 50% Ȯ���� �ƹ��� ó�� �ɸ��� ����
		TP_ONLY_ONE,			/// ó�� �ɸ��� ���� 	
		TP_HIGH_LEVEL_FIRST,	/// ������ ���� ���� ����
		TP_LOW_LEVEL_FIRST,		/// ������ ���� ���� ����
		TP_HIGH_HP_FIRST,		/// HP�� ���� ���� ����
		TP_LOW_HP_FIRST,		/// HP�� ���� ���� ����
		TP_NEAR_FIRST,			/// ���� ����� ����
		TP_FAR_FIRST,			/// ���� �ָ� ������ 
	};


private:
	CX2NPCAI() {};

public: 
	CX2NPCAI( CX2GUNPC* pOwnerNPC, NPC_AI_TYPE eAIType );	
	virtual ~CX2NPCAI();

	virtual void OnFrameMove( double fTime, float fElapsedTime ) = NULL;
	virtual void LoadAIDataFromLUA( KLuaManager& luaManager );


public:
	bool GetEnabled() const { return m_bEnabled; }
	void SetEnabled(bool val) 
	{ 
		m_bEnabled = val; 

		if ( false == m_bEnabled )
			ResetTarget();
	}

	void ResetTarget() { m_optrTargetGameUnit.Reset(); }
	CX2GameUnit* GetTargetGameUnit() { return ( null != m_optrTargetGameUnit ? m_optrTargetGameUnit.GetObservable() : NULL ); }
	void SetTargetGameUnit( CX2GameUnit* pTargetGameUnit_ ) 
	{ 
		if ( NULL != pTargetGameUnit_ )
			m_optrTargetGameUnit = pTargetGameUnit_; 
	}
	//UidType GetTargetUnitUID() const { return m_TargetUnitUID; }
	//void SetTargetUnitUID(UidType val) { m_TargetUnitUID = val; }

	//CX2GameUnit::GAME_UNIT_TYPE GetTargetUnitType() const { return m_TargetUnitType; }
	//void SetTargetUnitType(CX2GameUnit::GAME_UNIT_TYPE val) { m_TargetUnitType = val; }

	CX2NPCAI::NPC_AI_TYPE GetAIType() const { return m_eAIType; }
	void SetAIType(CX2NPCAI::NPC_AI_TYPE val) { m_eAIType = val; }

public:
	CX2GUNPC::STATE_CHANGE_TYPE GetPrevAIMessage() const { return m_PrevAIMessage; }
	void SetPrevAIMessage(CX2GUNPC::STATE_CHANGE_TYPE val) { m_PrevAIMessage = val; }

	bool GetFleeing() const { return m_bFleeing; }
	void SetFleeing(bool val) { m_bFleeing = val; }


	void ResetAttackerGameUnit() { m_optrAttackerGameUnit.Reset(); }
	CX2GameUnit* GetAttackerGameUnit() { return ( null != m_optrAttackerGameUnit ? m_optrAttackerGameUnit.GetObservable() : NULL ); }
	void SetAttackerGameUnit( CX2GameUnit* pAttackerGameUnit_ ) 
	{ 
		if ( NULL != pAttackerGameUnit_ )
			m_optrAttackerGameUnit = pAttackerGameUnit_; 
	}
	
	bool IsTargetFront() const { return m_bIsTargetFront; }
	bool IsTargetAccquired() 
	{ 
		if( null != m_optrTargetGameUnit ) 
			return true; 
		else 
			return false; 
	}

	virtual void RenderStateParam( WCHAR** wDest_ );
	virtual void RenderPatrolLineIndex( WCHAR** wDest_ );
	virtual void RenderPlayLineIndex( WCHAR** wDest_ );

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

	int GetMasterNpcUid() const { return m_pMasterNPC->GetUID(); }

// 	float GetIncreaseMoveSpeedPercentAtComeBackState() const { return m_fIncreaseMoveSpeedPercentAtComeBackState; }
// 	void SetIncreaseMoveSpeedPercentAtComeBackState( const float fVal_ ) { m_fIncreaseMoveSpeedPercentAtComeBackState = fVal_; }
// 
// 	float GetIncreaseAnimSpeedPercentAtComeBackState() const { return m_fIncreaseAnimSpeedPercentAtComeBackState; }
// 	void SetIncreaseAnimSpeedPercentAtComeBackState( const float fVal_ ) { m_fIncreaseAnimSpeedPercentAtComeBackState = fVal_; }

protected:
	CX2SimplePathFinder		m_PathFinder; // (M)

	bool					m_bNoBrain; /// (F) AI �� �۵����� �ʵ��� ��ũ��Ʈ���� �����Ѵ� (����, ����, �����, ��ƿ����� ��)
	bool					m_bEnabled;	/// (F) ����, ���ʱ�, ����̾��, ���̵�, callNearOneNpc ġƮ �� NPC�� �������� ���ϰ� �� �� ���
	CX2GUNPC*				m_pMasterNPC;	/// (T, M, F)
	

protected:
	NPC_AI_TYPE				m_eAIType;
		
	CX2GameUnitoPtr					m_optrTargetGameUnit;	/// ���� Ÿ�� ���� GameUnit;
	//UidType							m_TargetUnitUID;	/// (T,M)���� Ÿ���� �� ������ UID
	//CX2GameUnit::GAME_UNIT_TYPE 	m_TargetUnitType;	/// (T)���� Ÿ���� �� ������ Ÿ��

protected:

	CX2GameUnitoPtr					m_optrAttackerGameUnit;		/// �ڽ��� ������ GameUnit;

	CX2GUNPC::STATE_CHANGE_TYPE		m_PrevAIMessage;	/// (M) ������ AI Message (SCT_AI_DASH ��)

	bool			m_bIsTargetFront;		/// (M)Ÿ���� �ٶ󺸰� �� �ִ���
	bool			m_bFleeing;				/// (T,M)���� ���� ������ ��Ÿ���� ���� true��, ������ TargetUID�� �־����, Target�� ������ m_bFleeing�� false �� ��


	float 			m_fElapsedTimeAfterLastTargeting;		/// (T,F) targetInterval �ð��� ������ ������ ����ð�

	float			m_fElapsedTimeAfterLastRandomJump;		/// (M) �����ϰ� �Ʒ�, ���� �����ϴ� ���� ��Ʈ�� �ϴ� ���� (���� �Ӹ� �ƴ϶� �ٿ..)
	float			m_fElapsedTimeAfterLastWalkRunUpdate;	/// (M) walk, dash �߿� walk, dash���� ���º�ȭ�� CHASE_MOVE�� WALK_INTERVAL �������� ����
	float			m_fElapsedTimeAfterLastDirChange;	/// (M)
	float			m_fElapsedPatrolCoolingTime;	/// (M)��Ʈ�� ��Ÿ��������... ��Ʈ�� �Ÿ��� ����ų�, �� ���α׷쿡���� ��Ʈ���ؾ��ϴ� ��쿡 �׷��� �����ڸ��� �ְԵɶ��� ���� ����

// 	float			m_fIncreaseMoveSpeedPercentAtComeBackState;
// 	float			m_fIncreaseAnimSpeedPercentAtComeBackState;

#ifdef AI_FLY
	float			m_fElapsedTimeAfterLastFlyMoveUpdate;	/// (M)
	bool			m_bFlyUnit;
	bool			m_bNowStateFly;
#endif

};

