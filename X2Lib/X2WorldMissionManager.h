#pragma once

#ifdef SERV_INSERT_GLOBAL_SERVER

#define MAX_CRYSTAL_COUNT	5

class CX2UIWorldMission;

class CX2WorldMissionManager
{
public:
	~CX2WorldMissionManager();
	CX2WorldMissionManager();

	void Init();

	bool IsActiveDefenceDungeon() const { return m_bActiveDefenceDungeon; }
	void SetIsActiveDefenceDungeon( bool bActiveDefenceDungeon_ ) { m_bActiveDefenceDungeon = bActiveDefenceDungeon_; }
	//bool IsEXPBonus()		{ return m_bIsEXPBonus; }

//	int GetDungeonTargetCount() { return m_iDungeonTargetCount; }
	int GetNumOfWorldCrystalsProtected() const { return m_iNumOfWorldCrystalsProtected; }
	void SetNumOfWorldCrystalsProtected( int iNumOfWorldCrystalsProtected_ ) { m_iNumOfWorldCrystalsProtected = iNumOfWorldCrystalsProtected_; }

	USHORT GetRemainTimeWorldBuff() const { return m_usRemainTimeWorldBuff; }
	void SetRemainTimeWorldBuff( USHORT usRemainTimeWorldBuff_ ) { m_usRemainTimeWorldBuff = usRemainTimeWorldBuff_; }

	USHORT GetWorldBuffStepNum() const { return m_usWorldBuffStepNum; }
	void SetWorldBuffStepNum( USHORT usWorldBuffStepNum_ ) { m_usWorldBuffStepNum = usWorldBuffStepNum_; }

//	char GetNeedPartyNum() { return m_cNeedPartyNum; }

	void RenderMiniMapTexture( CKTDXDeviceRenderTargetTexture* 	pRenderTargetTexture );
	void RenderCrystal( int iPosX_, int iPosY_, const CX2GUNPC* pCrystal_, CKTDXTimer& tTimer_ );
	
	CX2UIWorldMission *GetUIWorldMission() const { return m_pUIWorldMission; }

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	HRESULT OnFrameMove( double fTime, float fElapsedTime );

	bool Handler_EGS_WORLD_MISSION_UPDATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_UPDATE_WORLD_BUFF_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	CKTDXDeviceTexture* GetTextureDefenceDungeonMiniMap() const { return m_pTextureDefenceDungeonMiniMap; }
	void SetTextureDefenceDungeonMiniMap( IN CKTDXDeviceTexture* pTextureDefenceDungeonMiniMap_ ) { m_pTextureDefenceDungeonMiniMap = pTextureDefenceDungeonMiniMap_; }

	const CKTDXDeviceTexture::TEXTURE_UV* GetUVDefenceDungeonMiniMap() const { return m_pUVDefenceDungeonMiniMap; }
	void SetUVDefenceDungeonMiniMap( const CKTDXDeviceTexture::TEXTURE_UV* pUVDefenceDungeonMiniMap_ ) { m_pUVDefenceDungeonMiniMap = pUVDefenceDungeonMiniMap_; }

	void SetDefenceDungeonMiniMap( const wstring& wstrTextureName, const wstring& wstrKeyName );

	int GetNumOfCrystals() const { return m_iNumOfCrystals; }
	void SetNumOfCrystals( int iNumOfCrystals_ ) { m_iNumOfCrystals = iNumOfCrystals_; }

	// �巡���� ����
	bool GetDragonBreathEnchantStatIncreased( OUT const CX2Stat::EnchantStat*& pEnchantStatInreased_ ) const;
	const int GetDragonBreathAnimSpeedRateIncreased() const;
	const float GetDragonBreathAttackStatRateIncreased() const;

	const float GetEnchantStatFromDrgonBreath( int iEnchantType ) const;
	const bool IsNullWorldBuffDragonBreath() const { return (NULL == m_pWorldBuffDragonBreath ? true : false); }

	// ���� �ູ
	const int GetElBlessCriticalRateIncreased() const;
	const float GetElBlessDefenceStatRateIncreased() const;

	const bool IsNULLWorldBuffElBless() const { return (NULL == m_pWorldBuffElBless ? true : false); }

	int GetRemainTimeToFinish() const { return m_iRemainTimeToFinish; }
	void SetRemainTimeToFinish( const int iRemainTime_ ) { m_iRemainTimeToFinish = iRemainTime_; }

	int GetRemainTimeMissionStart() const { return m_iRemainTimeMissionStart; }
	void SetRemainTimeMissionStart( const int iRemainTimeMissionStart_ ) { m_iRemainTimeMissionStart = iRemainTimeMissionStart_; }

	void GetDefenceDungeonIcnoDesc( OUT wstringstream& wstrStreamDungeonIconDesc_ );
	//const wstring& GetDefenceDungeonIconDesc() const;
	//void LoadScript();

#ifdef NEW_DEFENCE_DUNGEON
	//���� ����� �� ���̵�
	int GetNowDefenceDungeonLevel() const { return m_iNowDefenceDungeonLevel; }
	void SetNowDefenceDungeonLevel( const int iNowDefenceDungeonLevel_ ) { m_iNowDefenceDungeonLevel = iNowDefenceDungeonLevel_; }
	void ChangeDefenceDungeonLevel();	//���ѳ� ������ ������ ���� ����� �� ���̵� ���� �����Ѵ�.
#endif NEW_DEFENCE_DUNGEON

#ifdef SERV_2012_PVP_SEASON2_EVENT
	/// ���� ���� ���� 2 ���� �̺�Ʈ - ������ �ð� Ȱ�� ����
	bool GetActiveTimeOfGlory() const { return m_bActiveTimeOfGlory; }
	void SetActiveTimeOfGlory( bool bActiveTimeOfGlory_ ) { m_bActiveTimeOfGlory = bActiveTimeOfGlory_ ; }

	void SetTimeOfGloryDLG();			/// ������ �ð� Ȱ�� ���̾�α� ǥ�� ����
	void ProcessTimeOfGloryDLG();		/// ������ �ð� ���̾�α� ���İ� ���� �Լ�
	bool Handler_EGS_PVP_EVENT_INFO_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );		/// ������ �ð� ��Ŷ �ڵ鷯
#endif SERV_2012_PVP_SEASON2_EVENT 

#ifdef TOGETHER_FESTIVAL_2012_AUTUMN
	bool Handler_EGS_EARTH_QUAKE_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );			/// ������ ���� ���� �̺�Ʈ ��Ŷ
#endif TOGETHER_FESTIVAL_2012_AUTUMN


protected:
	void CreateWorldBuffDragonBreath();
	void DeleteWorldBuffDragonBreath();

	void CreateWorldBuffElBless();
	void DeleteWorldBuffElBless();

private:
	// ������� 2�ܰ�, �巡���� ����
	struct WORLD_BUFF_DRAGON_BREATH
	{
		CX2Stat::EnchantStat	m_EnchantStatIncreased;
		int						m_iAnimSpeedRateIncreased;
		float					m_fAttackStatRateIncreased;

		WORLD_BUFF_DRAGON_BREATH( const float fEnchantStatIncreased_, const int iAnimSpeedRateIncreased_, const float fAttackStatRateIncreased_ ) 
			: m_EnchantStatIncreased( fEnchantStatIncreased_ ), m_iAnimSpeedRateIncreased( iAnimSpeedRateIncreased_ ), m_fAttackStatRateIncreased( fAttackStatRateIncreased_ )
		{}
	};

	// ������� 3�ܰ�, ���� �ູ
	struct WORLD_BUFF_EL_BLESS
	{
		int						m_iCriticalRateIncreased;
		float					m_fDefenceStatRateIncreased;

		WORLD_BUFF_EL_BLESS( const int iCriticalRateIncreased_, const float fDefenceStatRateIncreased_ )
			: m_iCriticalRateIncreased( iCriticalRateIncreased_ ), m_fDefenceStatRateIncreased( fDefenceStatRateIncreased_ )
		{}
	};

	bool			m_bActiveDefenceDungeon;			///	���� ���� ����Ǿ� �ִ��� ����

	int				m_iNumOfWorldCrystalsProtected;		///	���� ������ ������ ���� ����

	int				m_iRemainTimeToFinish;				///	���� �ð�
	USHORT			m_usRemainTimeWorldBuff;			/// ���� ���� ���� �ð�
	USHORT			m_usWorldBuffStepNum;				/// ���� ���� ���� ���� ���� �ܰ�
	int				m_iRemainTimeMissionStart;			/// �̼� ���۽� ���� ���� �ð� (�� ����, ���� ���� ������ ���̻� ���� ��� -1)
		
	CKTDXTimer		m_timerMissionToFinish;				///	�̼� ����ð�
	CKTDXTimer		m_timerWorldBuff;					///	���� ���� ���� �ð�
	CKTDXTimer		m_timerMissionToStart;				/// �̼��� ���۵� �� ������ ���� �ð�

	//bool			m_bIsEXPBonus;						///	���ʽ� ����ġ ����

	CKTDXTimer		m_timerDamaged[MAX_CRYSTAL_COUNT];	

	CX2UIWorldMission	*m_pUIWorldMission;

	// �Ʒ� �ΰ��� CX2UIWorldMission���� �ִ� ����� ����� ����
	CKTDXDeviceTexture*	m_pTextureDefenceDungeonMiniMap;			/// ���潺 ���� �̴ϸ� �ؽ��� ������
	const CKTDXDeviceTexture::TEXTURE_UV* m_pUVDefenceDungeonMiniMap;		/// ���潺 ���� �̴ϸ� �ؽ��� UV ������

	const int m_iStartKeyNumberForCrystal;				/// �� ũ����Ż�� ������ �ִ� Ű�ڵ� �� ���� ù��° KeyNumber
	int m_iNumOfCrystals;								/// ���̵��� ���� �޶����� ũ����Ż�� ����

	WORLD_BUFF_DRAGON_BREATH*	m_pWorldBuffDragonBreath;
	WORLD_BUFF_EL_BLESS*		m_pWorldBuffElBless;

#ifdef NEW_DEFENCE_DUNGEON
	int							m_iNowDefenceDungeonLevel;	/// ���� ����� �� ���̵�
#endif NEW_DEFENCE_DUNGEON

#ifdef SERV_2012_PVP_SEASON2_EVENT
	bool						m_bActiveTimeOfGlory;		///	������ �ð� Ȱ�� ����
	bool						m_bShowTimeOfGloryDLG;		///	������ �ð� ���̾�α� Ȱ�� ����
#endif SERV_2012_PVP_SEASON2_EVENT

	// TEST
// 	int MAGIC_X_LEFT;
// 	int MAGIC_X_RIGHT;
// 	int MAGIC_Z_UP;
// 	int MAGIC_Z_DOWN;
// 
// 	int MAGIC_POS_X;
// 	int MAGIC_POS_Y;
// 
// 	int MAGIC_WIDTH;
// 	int MAGIC_HEIGHT;
// 
// 	int MAGIC_EL_WIDTH;
// 	int MAGIC_EL_HEIGHT;
};

#endif SERV_INSERT_GLOBAL_SERVER
