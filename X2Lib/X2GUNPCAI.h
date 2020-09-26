#pragma once


class CX2GUNPCAI : public CX2NPCAI
{
public:
	struct TargetData
	{
		bool				targetNPC;			/// NPC�� Ÿ������ ����
		TARGET_PRIORITY		targetPriority;
		float 				targetInterval;
		int					targetHeightLimit;	/// (��Ʋ�ʵ忡���� ��� ��)���̰� �� ���� ���� �־�� Ÿ�� ���� (�⺻100)
		int					targetNearRange;	/// �� ���� �ȿ� ������ ������ Ÿ���� �ȴ�
		int					targetRange;		/// �� ���� �ȿ� ������ Ÿ���� ����
		int					targetLostRange;	/// �� ���� ������ ����� ���� Ÿ���� �Ұ� �ȴ�.
		float				targetSuccessRate;	
		float				attackTargetRate;	/// ���� ������ ������ Ÿ������ Ȯ��
		float				preserveLastTargetRate;	/// ������ Ÿ���� ����� ������ Ȯ�� (�ٸ� ������ ������ �߰ų�, ���������ִٰų�, ����������� ã����� ����)
		string				strLuaTargetingFunc;	/// Targeting �� ������ ��� �Լ��� ������ �� ���� // X2GUNPCAI�� m_strLuaTargetingFunc�� ���õ�

		TargetData() : targetNPC( false ), targetPriority( CX2NPCAI::TP_RANDOM ), 
			targetInterval( 0 ), targetHeightLimit( 1000 ), targetNearRange( 0 ), targetRange( 0 ),
			targetLostRange( 0 ), targetSuccessRate( 0.0f ), attackTargetRate( 0.0f ), 
			preserveLastTargetRate( 0.0f ), strLuaTargetingFunc()
		{
		}
	};

	struct ChaseMoveData
	{
		int			moveSplitRange;		/// ������ġ�� ������ ��ġ ������ �Ÿ��� �� ���ؿ� ���� near ���� far ������ ������
		int			destGap;			/// ���������� �̰Ÿ� �ȿ� ������ �����ߴٰ� �Ǵ��Ѵ�
		int			moveGap;

		float		dirChangeInterval;	/// CHASE_MOVE�� DIR_CHANGE_INTERVAL�� AI Message�� ���� ������ �ٲٴ� �ð� ����

		float		walkInterval;
		float		nearWalkRate;		/// ������ġ�� ������ ��ġ ������ �Ÿ���moveSplitRange ���� ���� ��� WALK�� Ȯ��
		float		farWalkRate;		/// ������ġ�� ������ ��ġ ������ �Ÿ���moveSplitRange ���� ū ��� WALK�� Ȯ��

		float		jumpInterval;
		float		upJumpRate;
		float		upDownRate;
		float		downJumpRate;
		float		downDownRate;

		float		m_fLineEndDetectRange;	/// ���� ���� �����ߴ��� üũ�� �� ���Ǵ� ����
		bool		bOnlyThisLineGroup;
	};

	struct PatrolMoveData
	{
		float		patrolBeginRate;		/// Ÿ���� �Ҿ��� �� patrol�� �� Ȯ��
		int			patrolRange;
		float		patrolCoolingTime;
		bool		bOnlyThisLineGroup;		/// ������ ���α׷츸 ��Ʈ�� �ϴ��� ����
	};

	struct EscapeMoveData
	{
		int			moveSplitRange;		/// ������
		int			escapeGap;			/// �� �Ÿ� ���� �ָ� ���� ����, ������ �׸��д�

		float		walkInterval;		/// ������
		float		nearWalkRate;		/// ������
		float		farWalkRate;		/// ������

		float		jumpInterval;
		float		upJumpRate;		/// �������� ���� �ִµ� ���� �� Ȯ��
		float		upDownRate;		/// �������� ���� �ִµ� �Ʒ� ���θ����� �� Ȯ��..
		float		downJumpRate;	/// �������� �Ʒ��� �ִµ� ���� �� Ȯ��..
		float		downDownRate;	/// �������� �Ʒ��� �ִµ� �Ʒ� ���θ����� �� Ȯ��..

		float		m_fLineEndDetectRange;	/// ������
	};

	struct EscapeCondition
	{ 
		// escapeRate, myHP, escapeRange �� �Ѱ����� �����Ǿ� ������ �������� �������� 100, 10000 ������ ������
		// LoadAIDataFromLUA() ����
		float		escapeRate;		/// �ش� Ȯ���� ����
		int			myHP;			/// Percentage�� NPC �ڽ��� HP�� �̰� ������ ����
		float		escapeRange;
		// 09.04.16 �¿� : ���� ���ǿ� Flag Check �߰�
		int			m_iFlagTrue;	/// ���� �Ǿ� ���� ������ ����, ���õǾ� ������ GUNPC�� Flag�� Index�� ����Ͽ� Flag ������ �Ǵ�
		
		EscapeCondition()
		{
			escapeRate		= 0;
			myHP			= 0;
			escapeRange 	= 0.f;			
			m_iFlagTrue		= -1;
		}
	};

#ifdef AI_FLY
	struct FlyMoveData
	{
		float	fDestGap;
		float	fDestHeight;	/// DEGT_HEIGHT_GAP ���� Ÿ�ٰ� ������ ����
		float	fDestArea;
		float	fFlyMoveInterval;	/// FLY_MOVE_INTERVAL �� �̵� AI�� ���� Inverval Ÿ�� (interval�� �������� ���ϸ� ������ �̵��� ���)
		bool	bUnderWaterMode;
	};
#endif

	struct AIData
	{
		TargetData		targetData;
		ChaseMoveData	chaseMoveData;
		PatrolMoveData	patrolMoveData;
		EscapeMoveData	escapeMoveData;
		EscapeCondition	escapeCondition;
#ifdef AI_FLY
		FlyMoveData		flymovedata;
#endif
	};


	
public:
	CX2GUNPCAI( CX2GUNPC* pMasterNPC );
	virtual ~CX2GUNPCAI(void);

	virtual void OnFrameMove( double fTime, float fElapsedTime );
	virtual void LoadAIDataFromLUA( KLuaManager& luaManager );

	void TargetingNPC( float fElapsedTime );
	virtual void Targeting( float fElapsedTime );
	virtual void Moving( float fElapsedTime );

	void SetRallyToUnit( CX2GameUnit* pRallyGameUnit_ ); 	

	const D3DXVECTOR3& GetFinalDest() const { return m_vFinalDest; }
	void SetFinalDestPos( const D3DXVECTOR3& vDestPos_ ) { m_vFinalDest = vDestPos_; }

	void SetAITargetNPC( bool bTargetNPC ) { m_pAIData->targetData.targetNPC = bTargetNPC; }
	bool GetEnableLuaTargetingFunc() const { return m_bEnableLuaTargetingFunc; }
	void SetEnableLuaTargetingFunc(bool val) { m_bEnableLuaTargetingFunc = val; }
	const string& GetLuaTargetingFunc() const { return m_strLuaTargetingFunc; }
	void SetLuaTargetingFunc( const std::string& val) { m_strLuaTargetingFunc = val; }
	
	bool EscapeFlagCheck( int iFlagIndex );		//09.04.16 �¿�

	//{{ megagame / �ڱ��� / 2010.04.30 / NPC ���� �߰�
	float GetScanCloaking() const { return m_fScanCloaking; }
	float GetScanNearRange() const { return m_fScanCloakingNearRange; }
	//}} megagame / �ڱ��� / 2010.04.30 / NPC ���� �߰�
	void SetScanCloaking(float val) { m_fScanCloaking = val; }
	void SetScanNearRange(float val) { m_fScanCloakingNearRange = val; }

#ifdef AI_FLY
	float GetFlyDestHieght() { return m_pAIData->flymovedata.fDestHeight; }
#endif


	//{{ oasis : ����� // 2009-10-7 ////////////////
	// AI Data �ٲٴ� �Լ�
	void SetAItoMainAIData();
	void SetAItoSubAIData();
	//}} oasis : ����� // 2009-10-7 ////////////////

#ifdef RIDING_MONSTER
	void SetRideToUnit( CX2GameUnit* pRallyGameUnit_ );
	void StopRideToUnit();
#endif RIDING_MONSTER

	int GetFinalDestLineIndex() const { return m_iFinalDestLineIndex; }
	AIData* GetAIData() const { return m_pAIData; }
	int GetLastTouchLineIndex() const { return m_iLastTouchLineIndex; }
	bool GetFootOnLine() const { return m_bFootOnLine; }
	const D3DXVECTOR3& GetNowPosition() const { return m_vPosition; }
	float GetMaxJumpUp() const { return m_fMaxJumpUp; }
	float GetMaxJumpRight() const { return m_fMaxJumpRight; }

protected:

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    void TargetUpdate( bool bAccumulate_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void TargetUpdate();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void SetMasterUnitData();

	void SetFinalDestLineIndex( const int iFinalDestLineIndex_ ) { m_iFinalDestLineIndex = iFinalDestLineIndex_; }

	bool GetNowPatroling() const { return m_bNowPatroling; }
	void SetNowPatroling( bool bNowPatroling_ ) { m_bNowPatroling = bNowPatroling_; }
	
	bool IsRight() const { return m_bIsRight; }	

	bool GetLostTarget() const { return m_bLostTarget; }
	void SetLostTarget( const bool bLostTarget_ ) { m_bLostTarget = bLostTarget_; }


private:
	//{{ oasis : ����� // 2009-10-7 ////////////////
	// ����: AIData    m_AIData;
	AIData*			m_pAIData;	
	
	AIData*			m_pMainAIData;
	AIData*			m_pSubAIData;
	//}} oasis : ����� // 2009-10-7 ////////////////
	int				m_iFinalDestLineIndex;			/// (T,M) ���� �������� linedata index
	D3DXVECTOR3		m_vFinalDest;					// (T,M) ���� ������


	bool			m_bLostTarget;	/// (T,M)
	D3DXVECTOR3		m_vPivotPoint;	/// (T,M)			// Ÿ���� ���� ��쿡��, �� ���� �߽����� patrol�ϰų� guard, wander (���� �� ���� Ÿ���� ���� ���� ���� ��ġ)
	int				m_iPivotPointLineIndex;	/// ������




	// MasterNPC�� ���� ����
	D3DXVECTOR3		m_vPosition;	///(T,) AI�� ������ NPC�� ��ġ
	int				m_iLastTouchLineIndex;	///(M) AI�� ������ NPC�� ���������� �־��� LineMapIndex
	bool			m_bIsRight;		///(M)
	bool			m_bFootOnLine;	///(M)
	float			m_fMaxJumpUp;	///(M)
	float			m_fMaxJumpRight;	///(M)
	float			m_fNowHP;	///(T)
	float			m_fMaxHP;	///(T)	

	bool			m_bNowPatroling;			// (T,M) ���� patrol ������ ��Ÿ���� ����

	//bool			m_bRallyToUnit;			/// (T,M) Ư�� ������ �ִ� �� ���� �����ϴ����� ����

	CX2GameUnitoPtr	m_optrRallyGameUnit;	/// Ư�� ���� ( 2011.10.27 ����� Ride�� Call monster ��� ����)
	//UidType			m_RallyUnitUID;			/// (T,M) Ư�� ������ UID ( 2011.10.27 ����� Ride�� Call monster ��� ����)
	//UidType			m_RallyUnitType;		/// (T)Ư�� ������ Ÿ��

#ifdef RIDING_MONSTER
	bool			m_bRideToUnit;			/// (T,M)�ٸ� ������ ride
#endif RIDING_MONSTER

	bool			m_bEnableLuaTargetingFunc;
	string			m_strLuaTargetingFunc;			/// Ÿ���� Function���� �������� ��� �Լ� ��

	float			m_fScanCloakingNearRange;		/// (T) ���������� �� �Ÿ����� ������ Ž�� ����
	float			m_fScanCloaking;				/// (T) ���������� Ž���� Ȯ��
	bool			m_bScanCloaking;				/// (T) ���������� Ž���� �� �ִ��� ����

#ifdef SEASON3_MONSTER_2010_12
	bool			m_bTargetedOnlyOne;		/// �ʿ������...(T) �ϳ��� Ÿ���� �̹� ���õǾ����� Ȯ��
#endif

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    std::vector<CX2GameUnit*>   m_vecpTempGameUnit;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
};


