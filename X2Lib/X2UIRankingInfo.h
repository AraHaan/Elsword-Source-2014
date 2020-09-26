//{{ kimhc // 2009-07-09 // ��ϸ��� �ð��� �߰��Ǵ� Ranking UI �۾�
#ifdef	RANKING_INFO_UI

#pragma once

namespace		_CONST_UIRANKING_INFO_
{
	const	int		g_iNumRankingPerPage		= 5;		// �� ������ �� ��Ÿ�� �� �ִ� ��ŷ ����
	const	int		g_iLimitRankingToDisplay	= 2000;		// �� ���� ������ �˼����� ���� ǥ��
	const	int		g_iRankLimit				= 3;		// ������ 3�ڸ� ������
#ifdef SERV_HENIR_RANKING_GROUP_FIX
	const	int		g_iMaxPageLimit				= 21;
#else // SERV_HENIR_RANKING_GROUP_FIX
	const	int		g_iMaxPageLimit				= 20;
#endif // SERV_HENIR_RANKING_GROUP_FIX
}

#ifdef SERV_LOCAL_RANKING_SYSTEM
namespace		_CONST_UIRANKING_FRIEND_INFO_
{
	const	int		g_iNumRankingPerPage		= 8;		//�� �������� ��Ÿ���� ��ŷ ����
	const	int		g_iRankLimit				= 4;		//������ 4�ڸ�����
	const	int		g_iMaxPageLimit				= 143;		//�ִ� ������ ����
	const	int		g_iLimitRanking				= 1000;		//�� 1000�������� ������ �޴´�.
	const	int		g_iLimitStrongGage			= 6;		//�ټ��� ����Ʈ�� �ִ� 6�ڸ�����
}
#endif //SERV_LOCAL_RANKING_SYSTEM

class CX2UIRankingInfo
{
public:

	enum UI_RANKING_INFO_CUSTOM_MSG
	{
		URICM_EXIT							= 31000,
		URICM_SELECT_HENIR_TAP,
		URICM_SELECT_PVP_TAP,
		URICM_SELECT_LEVEL_TAP,
		URICM_SELECT_DAY_RADIO_BUTTON,
		URICM_SELECT_WEEK_RADIO_BUTTON,
		URICM_SELECT_MONTH_RADIO_BUTTON,
		URICM_PUSH_PREV_PAGE,
		URICM_PUSH_NEXT_PAGE,

//{{ ����� : [2009/8/21] //	���б� �̺�Ʈ ��
#ifdef NEW_TERM_EVENT
		URICM_SELECT_EVENT_TAP,

		URICM_SELECT_EVENT_HENIR_BUTTON,
		URICM_SELECT_EVENT_DUNGEON_BUTTON,
		URICM_SELECT_EVENT_KILL_BUTTON,
		URICM_SELECT_EVENT_DEATH_BUTTON,	
#endif	//	NEW_TERM_EVENT
//}} ����� : [2009/8/21] //	���б� �̺�Ʈ ��

//#ifdef SERV_LOCAL_RANKING_SYSTEM
		URICM_SELECT_WEEK_PVP_TAP			= 31014,
		URICM_SELECT_DUNGEON_FIELD_TAP,
		URICM_SELECT_AREA_RADIO_BUTTON,
		URICM_SELECT_GUILD_RADIO_BUTTON,
		URICM_SELECT_FRINED_CHECK,
		URICM_PAGE_SEARCH_FOCUS,
		URICM_PAGE_SEARCH_ENTER,
		URICM_OPEN_POPUP_MENU,
		URICM_POPUP_SELECT_CHARINFO,
		URICM_POPUP_SELECT_FRINED,
		URICM_POPUP_SELECT_WHISPER,
		URICM_PAGE_SERACH_CHANGE,
		URICM_SELECT_CHARACTER_CHECK,
		URICM_DUMMY_SELECT,		//�ƹ��͵� ���� ����. ����ư ��Ȱ��ȭ�� ���� �߰�
		URICM_DBCLK_CHAINFO,
		URICM_SELECT_MALE_CHECK,
		URICM_SELECT_FEMALE_CHECK,
// #ifdef ADDED_RELATIONSHIP_SYSTEM
		URICM_INVITE_COUPLE,
// #endif ADDED_RELATIONSHIP_SYSTEM
//#endif //SERV_LOCAL_RANKING_SYSTEM

//#ifdef NEW_HENIR_DUNGEON
		URICM_SELECT_HERO_BUTTON	= 31032,
//#endif // NEW_HENIR_DUNGEON

	};

	////////////////////////////////////////////////////
	// ������ // 2013-09-23 // �� �߰� �Ϸ��� Enum.h�� RANKING_TYPE�� ����� ��
	enum RANKING_TYPE 
	{
		RT_DAY_RANKING		= 0,
		RT_WEEK_RANKING,
		RT_MONTH_RANKING,
		RT_LEVEL_RANKING,			// enum.h�� RANKING_TYPE�� RT_DUNGEON_RANKING�� ����
		RT_PVP_RANKING,

		//{{ ����� : [2009/8/20] //	���б� �̺�Ʈ ���� enum
#ifdef NEW_TERM_EVENT
		RT_EVENT_DUNGEON,
		RT_EVENT_HENIR,
		RT_EVENT_PVP_KILL,
		RT_EVENT_PVP_DIE,
#endif	//	NEW_TERM_EVENT
		//}} ����� : [2009/8/20] //	���б� �̺�Ʈ ���� enum

#ifdef SERV_LOCAL_RANKING_SYSTEM
		RT_WEEK_PVP_TAP,
		RT_DUNGEON_FIELD_TAP,
		RT_AREA,
		RT_GUILD,
#endif //SERV_LOCAL_RANKING_SYSTEM

#ifdef NEW_HENIR_DUNGEON
		RT_HERO_RANKING = 9,
#endif // NEW_HENIR_DUNGEON
		RT_MAX_NUM,
	};

#ifdef SERV_LOCAL_RANKING_SYSTEM
	struct ColumnForRankFriend
	{
		u_char			ucLevel;				// ���� ����
		byte			byteGender;				// ���� ����
		int				UnitClass;				// ���� Ŭ����
		int				iArea;					// ����
		__time64_t		iBirth;					// ����
		UidType			m_iUserUID;				// ���� UID
		int				m_iRanking;				// ����
		CKTDGUIStatic*	m_pStaticRanking;		// ���� string
		CKTDGUIStatic*	m_pStaticClass;			// Ŭ���� ������
		CKTDGUIStatic*	m_pStaticNickName;		// �г��� string
		CKTDGUIStatic*	m_pStaticGender;		// ���� ������
		CKTDGUIStatic*	m_pStaticPoint;			// �ټ��� ��ġ or �ְ� ���� AP ��ġ �ؽ�ó
		CKTDGUIButton*	m_pButtonPopUp;			// ������ Ŭ���� �˾� ���
		wstring			m_wstsrIntroduce;		// �ڱ� �Ұ�
		ColumnForRankFriend() : m_wstsrIntroduce(L"")
		{
			byteGender			= 0;
			UnitClass			= 0;
			m_iUserUID			= 0;
			m_iRanking			= 0;
			m_pStaticRanking	= NULL;
			m_pStaticClass		= NULL;
			m_pStaticNickName	= NULL;
			m_pStaticGender		= NULL;
			m_pStaticPoint		= NULL;
			m_pButtonPopUp		= NULL;
		}
	};

	struct CommonMenuCategory
	{
		CKTDGUIStatic*			m_pStaticCommonInfo;
		CKTDGUIRadioButton*		m_pRadioArea;
		CKTDGUIRadioButton*		m_pRadioGuild;
		CKTDGUICheckBox*		m_pCheckFrield;
		CKTDGUICheckBox*		m_pCheckCharacter;
		CKTDGUICheckBox*		m_pCheckMale;
		CKTDGUICheckBox*		m_pCheckFemale;

		CommonMenuCategory()
		{
			m_pStaticCommonInfo	= NULL;
			m_pRadioArea		= NULL;
			m_pRadioGuild		= NULL;
			m_pCheckFrield		= NULL;	
			m_pCheckCharacter	= NULL;
			m_pCheckMale		= NULL;
			m_pCheckFemale		= NULL;
		}
	};

	enum BAR_SELECT //��׶��忡�� Bar(������, ������) ����
	{
		BS_OLDBAR		= 0,
		BS_NEWBAR,
//#ifdef LOCAL_RANKING_UPDATE
		BS_NONE,
//#endif //LOCAL_RANKING_UPDATE
	};

	enum GENDER_SELECT //���� ����
	{
		GS_NONE			= 0,
		GS_MALE,
		GS_FEMALE,
	};
#endif //SERV_LOCAL_RANKING_SYSTEM

	struct ColumnForRank
	{
		int				m_iRankingForOthers;				// �ٸ� ������� ����
		CKTDGUIStatic*	m_pStaticRanking;					// ������ ��ŷstring(������ string, �ٸ� ������ picture 3��)
		CKTDGUIStatic*	m_pStaticClass;						// ������ Ŭ���� ������
		CKTDGUIStatic*	m_pStaticNickName;					// �г���string
		
		CKTDGUIStatic*	m_pStaticClearStage;				// ������ Ŭ���������string
		CKTDGUIStatic*	m_pStaticPlayTime;					// �ɸ��ð�string ��
		CKTDGUIButton*	m_pButtonRegistredDate;				// ��ϸ� �ð��� ����� ��ϵ� ��¥(���콺 �� �� ���)

		CKTDGUIStatic*	m_pStaticPvpEmblem;					// ������ ���� ���
		CKTDGUIStatic*	m_pStaticWin;						// ������ ���� ��Ÿ���� string
		CKTDGUIStatic*	m_pStaticLose;						// ������ �и� ��Ÿ���� string
		CKTDGUIStatic*	m_pStaticExp;						// ������ ����ġstring

		ColumnForRank() 
		{
			m_iRankingForOthers				= 0;
			m_pStaticRanking				= NULL;
            m_pStaticNickName				= NULL;
			m_pStaticClass					= NULL;
	
			m_pStaticClearStage				= NULL;
			m_pStaticPlayTime				= NULL;
			m_pButtonRegistredDate			= NULL;

			m_pStaticPvpEmblem				= NULL;
			m_pStaticWin					= NULL;
			m_pStaticLose					= NULL;
			m_pStaticExp					= NULL;
		}
	};

public:
	CX2UIRankingInfo( const CKTDXStage* const pNowStage, const WCHAR* const pFileName );
	~CX2UIRankingInfo();

	HRESULT				OnFrameMove( double fTime, float fElapsedTime );
	bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				GetShow() const;
	void				SetShow( bool bShow );
	D3DXVECTOR2			GetDLGSize() const;
	void				SetPosition( D3DXVECTOR2 vec );
	void				SetLayer( X2_DIALOG_LAYER layer );

	//{{ ����� : [2009/8/20] //	�̺�Ʈ ��Ʈ�� Ȱ��ȭ �Լ�
#ifdef NEW_TERM_EVENT
	void				InitUIRankingEventControl( bool bShow );
#endif	//	NEW_TERM_EVENT
	//}} ����� : [2009/8/20] //	�̺�Ʈ ��Ʈ�� Ȱ��ȭ �Լ�


private:
	void				InitUIRankingInfo();
	void				InitMyColumn();
	void				InitOthersColumn();

	void				SetShowControlsByType( RANKING_TYPE type, bool bShow );
	void				SetShowMyInfoByType( RANKING_TYPE type, bool bShow );
	void				SetShowOthersInfoByType( ColumnForRank* const pColumnForOthersRank, RANKING_TYPE type, bool bShow );

	void				UpdatePageUI() const;
	std::wstring		ConvertSecToPlayTimeStr( UINT uiSec ) const;
	std::wstring		ConvertTimeToRegisterDateStr( __int64 tRegisterDate );
	void				DestoryUIRankingInfo();
	void				SetCharIcon( CKTDGUIStatic* const pStaticClass, char cUnitClass, int iLevel );
	void				SetLevelIcon( CKTDGUIControl::CPictureData* const pPictureData, int num );
	void				SetRankNumImage( CKTDGUIStatic* const pStaticRankNumImg, int iRankNum );
	void				SetNumToPictureForRank( CKTDGUIControl::CPictureData* const pPictureData, int num );


	bool				Handler_EGS_GET_RANKING_INFO_REQ();
	bool				Handler_EGS_GET_RANKING_INFO_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
#ifdef SERV_LOCAL_RANKING_SYSTEM
	#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
		 //���ڴ� ���÷�ŷ�� ���� �α׸� ����� ���� �߰�. �Խ��ǿ��� ��ư�� Ŭ���Ͽ� ȣ�������츸 true
		bool				Handler_EGS_LOCAL_RANKING_INQUIRY_REQ( bool IsBoardButton = false );
	#else
		bool				Handler_EGS_LOCAL_RANKING_INQUIRY_REQ();
	#endif //SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
	
	bool				Handler_EGS_LOCAL_RANKING_INQUIRY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void				ChangeBar( BAR_SELECT _barSelect );
	void				InitMyColumnFriend();
	void				InitOthersColumnFriend();
	void				SetShowInfoByType_F( ColumnForRankFriend* const pColumnForOthersRank, bool bShow );
	void				SetRank( CKTDGUIStatic* _static, int _rank );
	void				SetPoint( CKTDGUIStatic* _static, int _point );
	void				SetGender( CKTDGUIStatic* _static, GENDER_SELECT _select );
#endif //SERV_LOCAL_RANKING_SYSTEM

private:
	bool								m_bShow;
	const CKTDXStage* const				m_pNowStage;
	CKTDGUIDialogType					m_pDlgRankingInfo;

	RANKING_TYPE						m_nowRankingInfoType;				// ���� ������� �� ��ŷ ���� Ÿ��(��ϸ� ����, ���� ��)
	UINT								m_uiNowPage;						// ���� �������� ������ ��
	UINT								m_uiMaxPage;						// ���� ������ �� �ִ� �ִ� ������ ��
	D3DXVECTOR2							m_vSizeDlg;
	D3DXVECTOR2							m_vPosDlg;

	CKTDGUIStatic*						m_pStaticCommonInfo;		//������ �������� ���̴� �����׸��� �����ص�(��ϸ�,�������)
	CKTDGUIStatic*						m_pStaticHenirInfo;
	CKTDGUIStatic*						m_pStaticPvpInfo;
	CKTDGUIStatic*						m_pStaticLevelInfo;

	CKTDGUIRadioButton*					m_pRadioButtonDaily;
	CKTDGUIRadioButton*					m_pRadioButtonWeekly;
	CKTDGUIRadioButton*					m_pRadioButtonMonthly;
#ifdef NEW_HENIR_DUNGEON
	CKTDGUIRadioButton*					m_pRadioButtonHero;
#endif // NEW_HENIR_DUNGEON
	//{{ ����� : [2009/8/20] //	���б� �̺�Ʈ ���� ��Ʈ��
#ifdef NEW_TERM_EVENT
	CKTDGUIStatic*						m_pStaticEventInfo;
	CKTDGUIRadioButton*					m_pRadioButtonEventHenir;
	CKTDGUIRadioButton*					m_pRadioButtonEventDungeon;
	CKTDGUIRadioButton*					m_pRadioButtonEventKill;
	CKTDGUIRadioButton*					m_pRadioButtonEventDeath;
#endif	//NEW_TERM_EVENT
	//}} ����� : [2009/8/20] //	���б� �̺�Ʈ ���� ��Ʈ��

	ColumnForRank						m_columnForOthersRank[_CONST_UIRANKING_INFO_::g_iNumRankingPerPage];
	ColumnForRank						m_columnForMyRank;

#ifdef SERV_LOCAL_RANKING_SYSTEM
	RANKING_TYPE						m_nowRankingInfoTypeSub;

	CKTDGUIDialogType					m_pDlgRankingPopUp;

	ColumnForRankFriend					m_columnForOthersFriendRank[_CONST_UIRANKING_FRIEND_INFO_::g_iNumRankingPerPage]; //��ġ ����
	ColumnForRankFriend					m_columnForMyFriendRank;

	CommonMenuCategory					m_sCommonMenu;			//���� ���� �������� ���̴� �����׸�(����&�ʵ�,�ְ�����)
	CKTDGUIStatic*						m_pStaticDnFd;			//����&�ʵ忡���� ���̴� �����׸�
	CKTDGUIStatic*						m_pStaticWeekPvp;		//�ְ����������� ���̴� �����׸�
	CKTDGUIStatic*						m_pStaticNotice;		//��� �۾�, ����ǥ

	CKTDGUIIMEEditBox*					m_IMEEditPageSearch;	//������ �˻�

	byte								Is_Filter_Check;		//���� ���Ͱ� üũ�� �����ΰ�
	int									m_iPickedUser;			//���õǼ� �˾� �޴��� ����� ����

	CX2ProfileManager::Profile			m_sProfile;

#endif //SERV_LOCAL_RANKING_SYSTEM
};

#endif	RANKING_INFO_UI
//{{ kimhc // 2009-07-09 // ��ϸ��� �ð��� �߰��Ǵ� Ranking UI �۾�