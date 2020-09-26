//{{ ����� : [2011/3/22/] //	���� �̼�
#pragma once

#ifdef SERV_INSERT_GLOBAL_SERVER

const int REWARD_SUCCESS_ITEM_ID	= 60001797;	//	������ ���޵Ǵ� ������
const int REWARD_FAIL_ITEM_ID		= 60001798;	//	���н� �ð������� �������� ���޵Ǵ� ������

class CX2UIWorldMission
{
public:
	enum	WORLD_MISSION_UI_MSG
	{
		WMUM_REWARD_CLOSE	= 46200,
	};

	CX2UIWorldMission();
	~CX2UIWorldMission();

	//bool SetShowRewardDlg( bool bEnable );
	void SetShowTimeDlg( bool bEnable );
	//bool SetShowEXPBonusDlg( bool bEnable );

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

//  	void SetReservedRewardItem( bool bEnable, int iItemID = 0, int iQuantity = 0 );
//  	bool GetReservedRewardItem()	{ return m_bReservedReward; }

	void SetForceUpdateCrystalUI(){m_bForceUpdate = true;}

	void UpdateTimeDlg( int iTime );

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-15
	void UpdateNumOfCrystalsProtected( int iNumOfCrystalsProtected_ = 0, int iNumOfMaxCrystalsProtected_ = 1 );
#else // SERV_NEW_DEFENCE_DUNGEON
	void UpdateNumOfCrystalsProtected( int iNumOfCrystalsProtected_ );
#endif // SERV_NEW_DEFENCE_DUNGEON

	//int GetNumOfCrystalsProtected() const { return m_iNumOfCrystalsProtected; }
	//void SetNumOfCrystalsProtected( int iNumOfCrystalsProtected_ ) { m_iNumOfCrystalsProtected = iNumOfCrystalsProtected_; }

	void SetShowCrystalPictureData( CKTDGUIStatic* pStaticTime_, int iIndexOfColor_, bool bShow_ );

	CKTDGUIDialogType			GetTimeDlg()	{ return m_pDlgTime; }
	//CKTDGUIDialogType			GetEXPBonusDlg()	{ return m_pDlgEXPBonus; }

	bool GetEnableTimeDlg() const { return m_bEnableTimeDlg; }
	void SetEnableTimeDlg( bool bEnableTimeDlg_ ) { m_bEnableTimeDlg = bEnableTimeDlg_; }

#ifdef SERV_NEW_DEFENCE_DUNGEON	// ���볯¥: 2013-03-27
	bool GetIsFlickerDanger() const { return m_bIsFlickerDanger; }		/// ����� �� ���� �̼� UI�� Danger ��ũ�� �����̴� ���� ��ȯ
	void SetIsFlickerDanger(bool val) { m_bIsFlickerDanger = val; }		/// ����� �� ���� �̼� UI�� Danger ��ũ�� �����̴� ���� ����
#endif // SERV_NEW_DEFENCE_DUNGEON

private:
	//CKTDGUIDialogType			m_pDlgReward;
	CKTDGUIDialogType			m_pDlgTime;
	//CKTDGUIDialogType			m_pDlgEXPBonus;

// 	bool						m_bReservedReward;
// 	int							m_iRewardItemID;
// 	int							m_iRewardQuantity;
	int							m_iNumOfCrystalsProtected;	// ������� ������ ũ����Ż�� ����
	bool						m_bForceUpdate;
	bool						m_bEnableTimeDlg;

#ifdef SERV_NEW_DEFENCE_DUNGEON	// ���볯¥: 2013-03-27
	bool						m_bIsFlickerDanger;			/// ����� �� ���� �̼� UI�� Danger ��ũ�� �����̴� ����
#endif // SERV_NEW_DEFENCE_DUNGEON
	
};

#endif	SERV_INSERT_GLOBAL_SERVER
//}} ����� : [2011/3/22/] //	���� �̼�
