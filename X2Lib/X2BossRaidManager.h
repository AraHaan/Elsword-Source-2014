#pragma once

#ifdef FIELD_BOSS_RAID
class CX2BossRaidManager
{
public:
	void OnFrameMove( double fTime, float fElapsedTime );
	void OnFrameRender();
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
public:
	static CX2BossRaidManager* GetInstance();
	static void ResetBossRaidManager();
	static void DestroyInstance();

	// ���̵� �ʵ� Ȱ��ȭ ����
	bool GetIsOngoingRaid() const { return m_bIsOngoingRaid; }
	void SetIsOngoingRaid(bool val) { m_bIsOngoingRaid = val; }

	// ���̵� �ʵ�� ���� ��Ż Ȱ��ȭ ����
	bool IsActiveRaidPortal() const;
	
	// ���̵� �ʵ� MapID ����
	void InitRaidFieldMapID();
	SEnum::VILLAGE_MAP_ID GetBossRaidCreatorMapID() const { return m_eBossRaidCreatorMapID; }
	SEnum::VILLAGE_MAP_ID GetBossRaidMapID() const { return m_eCurrentRaidMapID; }

	// ���� ���� ���� ����
	void SetSendIntrudeRestrictionNot(bool val) { m_bSendIntrudeRestrictionNot = val; }

	// ���� ����ó��.
	void JoinFieldProcess( float fFieldClearLeftTime_ = 0.f);

	// ����Ʈ ��ġ�� ���ߴ� ī�޶� �ε���
	int GetCameraIndex( IN SEnum::VILLAGE_MAP_ID eFieldMapID_ ) const ;
	void GetLoadingUIFileName( OUT wstring& wstrImgLFileName_, OUT wstring& wstrImgRFileName_) const;

	// ���尡 ������� ���� ä��, ������ �����Ƿ��� �� ���� ����ó��.
	void CreateNpcExceptProcess( const CX2UnitManager::NPC_UNIT_ID eNpcID_ ) const;

	CX2UnitManager::NPC_UNIT_ID GetBossNPCIDByMapID( SEnum::VILLAGE_MAP_ID eFieldMapID_ ) const;

	// Load NPC REQ ���� ����.. �� �ʵ忡�� 1ȸ�� ���� �� ����
	//bool IsValideNpcLoadReq() const;
	//bool GetSendNpcLoadReq() const { return m_bSendNpcLoadReq; }
	//void SetSendNpcLoadReq(bool val) { m_bSendNpcLoadReq = val; }

	// ���� �ð� ����
	void SetRaidFieldClearLimitTime(float val) { m_fRaidFieldClearLimitTime = val; }
	float GetRaidFieldClearLimitTime() const { return m_fRaidFieldClearLimitTime; }

	float GetStandardLeftTime() const { return m_fStandardLeftTime; }
	void SetStandardLeftTime(float val) { m_fStandardLeftTime = val; }

	const CTime& GetReceivePacketTime() const { return m_tReceivePacketTime; }
	void SetReceivePacketTime(CTime& val) { m_tReceivePacketTime = val; }

	/// ġƮ
	void OpenCheat();
	void CloseCheat();
	// ġƮ ��Ŷ
	void Handler_EGS_ADMIN_BOSS_FIELD_GATE_OPEN_REQ(); // ���� �ʵ� ��Ż�� ����
	bool Handler_EGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void Send_EGS_ADMIN_BOSS_FIELD_GATE_CLOSE_NOT(); // ���� �ʵ� ��Ż�� �ݱ�

	void Handler_EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ(); // ��Ʋ �ʵ��� TotalDangerousValue�� �˾Ƴ���
	bool Handler_EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void Handler_EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ( int byDangerousValue_ ); // ��Ʋ �ʵ��� TotalDangerousValue�� �����ϱ� ���� ġƮ ��Ŷ
	bool Handler_EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void Handler_EGS_BOSS_FIELD_INTRUDE_RESTRICTION_REQ_Cheat() { m_bSendIntrudeRestrictionNot = false; Handler_EGS_BOSS_FIELD_INTRUDE_RESTRICTION_REQ(); };

	// ~End Public Function
private:

	bool Handler_EGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	bool Handler_EGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_BOSS_FIELD_CLOSE_PROCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_BOSS_FIELD_RETURN_PROCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	// ���� �ʵ� ���� �Ұ� ó�� ��û
	void Handler_EGS_BOSS_FIELD_INTRUDE_RESTRICTION_REQ();
	bool Handler_EGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	// ���̵� �ʵ� �Ա� ����Ʈ ����
	void UpdateLinemapPortalEffect();

	void OnFrameMove_BattleField( double fTime, float fElapsedTime );
	void OnFrameRender_BattleField();

	// �ʵ� ���� ���� �ؽ��� �̸� ���
	void GetFieldOpenTextureName( IN SEnum::VILLAGE_MAP_ID eBossRaidCreatorMapID_, OUT wstring& wstrOpenTextureName_ ) const;

	// ���� �Ұ� ��û�� ���� ü�� �˻�
	bool IsBossHPLessThen10() const;

#ifdef SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
	void RenderRanking() const;

	void UpdateContributionRanking();
#endif // SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD

	// ~End Private Function
private:
	CX2BossRaidManager();
	~CX2BossRaidManager();
	
	static CX2BossRaidManager* m_pManager;
	//////////////////////////////////////////////////////


	bool						m_bIsOngoingRaid;		// ���� ���̵� ���� ����
	SEnum::VILLAGE_MAP_ID		m_eCurrentRaidMapID;	// ���� �����ִ� ���̵� �ʵ� ID

	SEnum::VILLAGE_MAP_ID		m_eBossRaidCreatorMapID;// ���赵 �޼��� ���� ���̵� �ʵ带 ������ �ʵ��� ID
	bool						m_bShowFieldClearTime;
	CKTDGFontManager::CUKFont*	m_pFontForTime;		// Ÿ�� �ƿ��� �ð� ǥ�ÿ�


	//float						m_fBanLeftTimeFromRaidField;	// ���̵� �ʵ忡�� �Ѿ� �� ���� �ð�


	// ���� ���̵� ���� ü�� 10% ������ �� ���� ����
	bool						m_bSendIntrudeRestrictionNot;	// ���� �Ұ� ��Ŷ ��û ����.

#ifdef SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD
	vector<wstring>				m_vecContributionRanking;		// �⿩�� ��ŷ

	float						m_fElaspedTimeCalRank;			// �⿩�� ��� �ð�
#endif // SHOW_CONTRIBUTION_RANKING_AT_RAID_FIELD

	float						m_fRaidFieldClearLimitTime;		// ���� ���� �ð�
	CTime						m_tReceivePacketTime;			// ù ���� �� �����κ��� ���� �ð��� ���� �ð�
	float						m_fStandardLeftTime;			// ù ���� �� �����κ��� ���� ���� �ð�

	//bool						m_bSendNpcLoadReq;				// NPC Load REQ ���� ����(1ȸ�� ������ ���� ���)
#ifdef ADD_RAID_FIELD_LOG
	float						m_fTestLimitTime;
#endif // ADD_RAID_FIELD_LOG
};

#endif // FIELD_BOSS_RAID

