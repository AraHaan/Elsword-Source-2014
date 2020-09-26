#pragma once

class CX2AllyNPCAI : public CX2NPCAI
{
public: 
	struct AllyData
	{
		float	allyLostRange;			// �� �Ÿ� ������ ����� ������ ally�� �i�ư���.
		float	allyFarLostRange;		// �� �Ÿ� ������ ����� ������ ally �ٷ� ������ ���� �̵��Ѵ�.

		AllyData()
		{
			allyLostRange		= 1000.f;
			allyFarLostRange	= 2000.f;
		}
	};
	
	struct TargetData
	{
		TARGET_PRIORITY		targetPriority;
		float 				targetInterval;		// �� ���� 
		float				targetRange;		// �� ���� �ȿ� ������ Ÿ���� �ȴ�
		float				targetNearRange;	// �� ���� �ȿ� ������ ������ Ÿ���� �ȴ�				���, Ÿ������
		float				targetLostRange;	// �� ���� ������ ����� ���� Ÿ���� �Ұ� �ȴ�.		���, Ÿ���� �Ҵ´�.
		int					targetSuccessRate;	// [0,100]
		int					targetAttakerRate;	// [0,100] ���� ������ ������ Ÿ������ Ȯ��				�´� ���, Ÿ������
		int					targetPreserveRate; // [0,100] ���� Ÿ���� ������ Ȯ��, ���� Ÿ���� ���� ��쿡��
#ifdef EVOKE_TARGETING_BUG_FIX
		std::string			strLuaTargetingFunc;	/// �޴��� Ÿ���� �Լ���
#endif EVOKE_TARGETING_BUG_FIX
#ifdef ADD_NPC_CONDITION_TABLE
		bool				bTargetOnlyOurTeam;
#endif // ADD_NPC_CONDITION_TABLE

		TargetData()
		{
			targetPriority		= CX2NPCAI::TP_RANDOM;
			targetInterval		= 0.f;
			targetRange			= 0.f;
			targetNearRange		= 0.f;
			targetLostRange		= 0.f;
			targetSuccessRate	= 0;
			targetAttakerRate	= 0;
			targetPreserveRate	= 0;
#ifdef EVOKE_TARGETING_BUG_FIX
			strLuaTargetingFunc = "";
#endif EVOKE_TARGETING_BUG_FIX
#ifdef ADD_NPC_CONDITION_TABLE
			bTargetOnlyOurTeam = false;
#endif // ADD_NPC_CONDITION_TABLE
		}
	};

	struct ChaseMoveData
	{
		float		destGap;				// ���������� �̰Ÿ� �ȿ� ������ �����ߴٰ� �Ǵ��Ѵ�
		float 		moveGap;				// �������� ���� �� �Ÿ����� �ָ� ������ ������ �ٽ� �i�ư��� �����Ѵ�.

		float		dirChangeInterval;		// ������ �ٲ� �ð� ����

		float		dashRange;				// �� ���� �ȿ� ������ �ٰ�, �ۿ� ������ �ȴ´�.
		bool		bReverseWalkDash;		// true�̸� ������ ���� �� �Ȱ�, �ָ� ���� �� �ڴ�.
		float		walkDashInterval;		// walk, dash ���¸� �ٲ� �ð� ����

		bool		bStayOnCurrentLineGroup;

#ifdef ADD_NPC_CONDITION_TABLE
		bool		bIfCannotFindMoveStateDoWait; // �̵� ������Ʈ ã�� ���ϸ� ��� �ϵ��� �ϴ� ����
#endif // ADD_NPC_CONDITION_TABLE

		ChaseMoveData()
		{
			destGap		= 0.f;
			moveGap		= 0.f;
			dashRange	= 0.f;
			
			bReverseWalkDash		= false;
			dirChangeInterval		= 0.f;
			walkDashInterval		= 0;
			bStayOnCurrentLineGroup	= false;
#ifdef ADD_NPC_CONDITION_TABLE
			bIfCannotFindMoveStateDoWait = false; // �̵� ������Ʈ ã�� ���ϸ� ��� �ϵ��� �ϴ� ����
#endif // ADD_NPC_CONDITION_TABLE
		}
	};


	struct RandomJumpData
	{
		bool		bEnabled;
		float		jumpInterval;		// ��
		int			jumpUpRate;			// [0,100]
		int			jumpDownRate;		// [0,100]

		RandomJumpData()
		{
			bEnabled		= false;
			jumpInterval	= 0.f;
			jumpUpRate		= 0;
			jumpDownRate	= 0;
		}
	};



	//struct PatrolData
	//{
	//	bool	m_bEnabled;
	//	float	m_fPatrolRange;
	//	float	m_fPatrolTime;			// patrol�� �����ϴ� �ð�
	//	float	m_fPatrolCoolTime;		// patrol�� ���߰� ���� �ð�
	//	bool	m_bStayOnLineGroup;
	//};

public:
	CX2AllyNPCAI( CX2GUNPC* pMasterNPC );
	virtual ~CX2AllyNPCAI(void);

	virtual void OnFrameMove( double fTime, float fElapsedTime );
	virtual void LoadAIDataFromLUA( KLuaManager& luaManager );


public:
	bool RallyToAlly();
	void Targeting( float fElapsedTime );
	void Moving( float fElapsedTime );

	void TargetUpdate();
	void SetMasterUnitData();

#ifdef EVOKE_TARGETING_BUG_FIX			/// �޴��� Ÿ���� �߰�
	bool GetEnableLuaTargetingFunc() const { return m_bEnableLuaTargetingFunc; }
	void SetEnableLuaTargetingFunc(bool val) { m_bEnableLuaTargetingFunc = val; }
	const std::string& GetLuaTargetingFunc() const { return m_strLuaTargetingFunc; }
	void SetLuaTargetingFunc( const std::string& val) { m_strLuaTargetingFunc = val; }
#endif EVOKE_TARGETING_BUG_FIX

public:
	UidType GetAllyUnitUID() const { return m_AllyUnitUID; }
	void SetAllyUnitUID( const UidType uidAllyGameUnit_ ) { m_AllyUnitUID = uidAllyGameUnit_; }

	CX2GameUnit* GetAllyGameUnit() const { return ( null != m_optrAllyGameUnit ? m_optrAllyGameUnit.GetObservable() : NULL ); }
	void SetAllyGameUnit( CX2GameUnit* pAllyGameUnit_ )
	{ 
		if ( NULL != pAllyGameUnit_ )
		{
			m_optrAllyGameUnit = pAllyGameUnit_; 
			SetAllyUnitUID( m_optrAllyGameUnit->GetUnitUID() );
			SetAllyUnitType( m_optrAllyGameUnit->GetGameUnitType() );
		}
	}

	CX2GameUnit::GAME_UNIT_TYPE GetAllyUnitType() const { return m_eAllyUnitType; }
	void SetAllyUnitType(const CX2GameUnit::GAME_UNIT_TYPE val) { m_eAllyUnitType = val; }

private:
	CX2GameUnitoPtr				m_optrAllyGameUnit;
	UidType						m_AllyUnitUID;	
	CX2GameUnit::GAME_UNIT_TYPE	m_eAllyUnitType;


	AllyData		m_AllyData;
	TargetData		m_TargetData;
	ChaseMoveData	m_ChaseMoveData;
	RandomJumpData	m_RandomJumpData;
	//PatrolData	m_PatrolData;


private:
	int				m_iFinalDestLineIndex;				// targetupdate()���� ���� ���� �������� linedata index
	D3DXVECTOR3		m_vFinalDest;						// ���� ������


	float			m_fMaxJumpUp;
	float			m_fMaxJumpRight;	

	//bool			m_bIsTargetFront;

#ifdef EVOKE_TARGETING_BUG_FIX							/// �޴��� Ÿ���� �߰�
	bool			m_bEnableLuaTargetingFunc;
	string			m_strLuaTargetingFunc;
#endif EVOKE_TARGETING_BUG_FIX

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    std::vector<CX2GameUnit*>   m_vecpTempGameUnit;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
};
