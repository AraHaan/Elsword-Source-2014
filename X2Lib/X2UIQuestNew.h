#pragma once

#ifdef SERV_EPIC_QUEST
namespace		_CONST_UIQUESTNEW_INFO_
{
	const	UINT		g_iNumQuestSlotPerPage		= 13;		// �� ������ �� ��Ÿ�� �� �ִ� �ִ� ����Ʈ ���� ����
	const	int			g_iRewardSlotNum			= 3;		// ���� ���� ����

	const	int			g_iMaxNumOngoingQuest		= 6;

	const	float		g_fQuickQuestMinimizeSizeY	= 22.f;

}





//class CX2StateMenu;
class CX2UIQuestNew : public CX2ItemSlotManager
{
	enum UI_QUEST_NEW_CUSTOM_MSG
	{
		UQNCM_EXIT				= 41000,
		UQNCM_QUEST_SELECT,		//����Ʈ ����
		UQNCM_QUEST_ACCEPT,		//����Ʈ ����
		UQNCM_QUEST_COMPLETE,	//����Ʈ �Ϸ�
		UQNCM_QUEST_GIVE_UP,	//����Ʈ ����
		UQNCM_QUEST_GIVE_UP_CONFIRM, //����Ʈ ����_Ȯ��

		UQNCM_QUESTCLEAR_OK,

		UQNCM_SCROOL_UP,		//��ũ��
		UQNCM_SCROOL_DOWN,

		UQNCM_REWARD_CHECK_ITEM,	//����
		UQNCM_REWARD_UNCHECK_ITEM,
		UQNCM_REWARD_MOUSE_OVER,
		UQNCM_REWARD_MOUSE_OUT,

		UQNCM_NAVI_LCLICKED,
		UQNCM_NAVI_DRAGGING,

		UQNCM_QUEST_ACCEPT_CONFIRM,
		UQNCM_QUEST_ACCEPT_CANCLE,
		UQNCM_QUEST_GIVE_UP_CANCLE,

		UQNCM_QUEST_ADMIN_COMPLETE,

		UQNCM_QUEST_DESC_UP,		//������� �ʴ� �׸�
		UQNCM_QUEST_DESC_DOWN,		//������� �ʴ� �׸�

		UQNCM_EXIT2,				//������� �ʴ� �׸�

		UQNCM_CATEGORY_SELECT,
		UQNCM_BACK_TO_QUEST_LIST,	//�������

		UQNCM_QUICK_TO_EXPAND,
		UQNCM_QUICK_TO_MINIMIZE,
		UQNCM_QUICK_EXPAND_MOUSE_OVER,
		UQNCM_QUICK_EXPAND_MOUSE_OUT,

		UQNCM_SELECT_QUEST_NORMAL_TAB,
		UQNCM_SELECT_QUEST_SCENARIO_TAB,

		UQNCM_QUEST_REMIND,
		UQNCM_CHECK_QUICK_QUEST,
		UQNCM_QUEST_SELECT_MOUSE_OVER,
		UQNCM_QUICK_TO_DETAIL,
		UQNCM_QUEST_COMPLETE_MOUSE_OVER,
		UQNCM_TOOLTIP_MOUSE_OUT,

//#ifdef REFORM_QUEST
		UQNCM_SELECT_QUEST_EVENT_TAB,
		UQNCM_QUEST_INSTANTLY_COMPLETE,
//#endif //REFORM_QUEST

	};

	struct QuestListSlot
	{
		int					m_QuestID;
		CKTDGUIRadioButton*	m_pButton;
		CKTDGUIStatic*		m_pStatic;
	};


public:

	CX2UIQuestNew( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2UIQuestNew(void);

	HRESULT OnFrameMove( double fTime, float fElapsedTime );
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GetShow() { return m_bShow; }
	bool GetShowClearDLG(){ return m_bShowClearDLG; }
	void SetShow(bool val);
	void SetLayer( X2_DIALOG_LAYER layer );
	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }
	void SetPosition( D3DXVECTOR2 vec );
	

	void ShowNextButton(bool val);



	//////////////////////////////////////////////////////////////////////////

	bool Handler_EGS_TALK_WITH_NPC_REQ( CX2UnitManager::NPC_UNIT_ID questNPCID );
	bool Handler_EGS_TALK_WITH_NPC_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_NEW_QUEST_REQ( int iQuestID );
	bool Handler_EGS_NEW_QUEST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_NEW_QUEST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_GIVE_UP_QUEST_REQ( int questID );
#ifdef SERV_GATHER_GIVE_UP_QUEST 
	bool Handler_EGS_GATHER_GIVE_UP_QUEST_REQ( std::vector< int >& vecGatherQuestID );
#endif // SERV_GATHER_GIVE_UP_QUEST
	bool Handler_EGS_GIVE_UP_QUEST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool Handler_EGS_QUEST_COMPLETE_REQ( CX2UnitManager::NPC_UNIT_ID questNPCID, int questID, std::map<int,int> mapSelectItemID );
	bool Handler_EGS_QUEST_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_ADMIN_QUEST_COMPLETE_REQ( CX2UnitManager::NPC_UNIT_ID questNPCID, int questID, std::map<int,int> mapSelectItemID );

#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	bool Handler_EGS_ALL_COMPLETED_QUEST_COMPLETE_REQ( const KEGS_ALL_COMPLETED_QUEST_COMPLETE_REQ& kPacketNot );
	bool Handler_EGS_ALL_COMPLETED_QUEST_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_ENTER_FIELD_QUEST_CLEAR

#ifdef SERV_POINT_COUNT_SYSTEM
	bool Handler_EGS_QUEST_POINT_COUNT_SYSTEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_POINT_COUNT_SYSTEM

	//////////////////////////////////////////////////////////////////////////

	void SetNPCID(CX2UnitManager::NPC_UNIT_ID val){	m_eNPCID = val; }
	//void UpdateQuestList( CX2UnitManager::NPC_UNIT_ID eNPCID = CX2UnitManager::NUI_NONE );
	//void UpdateQuestUI( bool bUpdateNavi = true );
	void UpdateNavi();
	//void ResetQuestUI();
	//void UncheckAllQuestList(); // X


	//void UpdateQuestDescPage(); // X


	//void UpdateNPCQuestDesc(); // X

	void SelectQuest( int SelectedQuestID );
	//void SelectNextQuest( bool bNext ); // X

	bool GetThereIsSelectionReward() const ;
	void OpenQuestRewardWindow( int iQuestID, bool bEnableSelection );
	void CloseQuestRewardWindow();
#ifdef QUEST_REWARD_PERIOD
	void OpenRewardItemInfoPopup( bool bOpen, D3DXVECTOR2 vPos, int iItemID, int socketID = 0, int Period = 0 );
#else
	void OpenRewardItemInfoPopup( bool bOpen, D3DXVECTOR2 vPos, int iItemID, int socketID = 0 );
#endif QUEST_REWARD_PERIOD
	bool CanIAcceptQuest( int iQuestID );

	// �̴ϸ� ����Ʈ �˸��� ����
	void UpdateNewQuestNotice();
	bool IsNearVillage( int CurrentVillageID, int TargetVillageID );	// ����Ʈ �˸��̿��� "PC�� ���� ����"������ Ȯ���ϴ� �Լ�.

	// ���콺 �� ����
	bool MouseOnQuestListArea( D3DXVECTOR2 mousePos );
	//bool MouseOnQuestCommentArea( D3DXVECTOR2 mousePos ); // X
#ifdef REFORM_QUEST
	void SetOpenEpicCategory();
	void InitNewQuestList(){m_setNewEventQuestIDList.clear();}
	void NotificationByQuickQuestInfoChange(const int iQuestID_, bool bIsSubQuestComplete_ = false);
	void PlayQuestCompleteSound( bool bIsSubQuestComplete_ = true);
#endif //REFORM_QUEST

protected:
	virtual wstring GetSlotItemDesc();
	virtual wstring GetSlotItemDescExtra_RBtn( int itemTID, CX2Item* pItem );
	bool TargetQuestisOnScreenList( int TargetQuestID );

private:
	int						m_iQuestPerPage;


	bool					m_bShow;
	bool					m_bShowClearDLG;
	D3DXVECTOR2				m_MovedPosition;
	D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;

	CKTDGUIDialogType			m_pDLGUIQuestReceive;   
	CKTDGUIDialogType			m_pDLGUIQuestClear;
	CKTDGUIDialogType			m_pDLGQuestRewardItemInfo;

	CKTDGUIDialogType			m_pDLGMsgBox;


	CKTDGUICheckBox*					m_pCheckListQuickQuest;
	CKTDGUICheckBox*					m_pCheckDetailQuickQuest;


	CX2UnitManager::NPC_UNIT_ID m_eNPCID;


	//int					m_iTopQuestSlotIndex;
	vector<int>			m_vecPrevNotAssignedQuestID;		// ���� ���ŵ� ����Ʈ ����Ʈ�� �ִ��� Ȯ���ϱ� ���ؼ�
	vector<int>			m_vecNotAssignedQuestID;
	vector<int>			m_vecAvailableQuestID;
	vector<QuestListSlot>			m_vQuestList;

	int					m_iPickedQuestID;

	map<int, bool >		m_mapNewQuest;					// ���� ���� ����Ʈ ���

	bool				m_bAdminQuestComplete;
	bool				m_bLastRewardWasSelection;

	// ���콺 �� ����
	SHORT				m_SumDelta;
	std::multimap<int, int>	m_mulmapNearVillageData;

	vector<wstring>		m_vecQuestDesc;
	int					m_iQuestDescIndex; // �ʿ�X
	int					m_iQuestDescLinePerPage; // �ʿ�X
	D3DXVECTOR2			m_vQuestDescSize; // �ʿ�X

	// ����Ʈ ���� ����..
	map< int, int >		m_mapItemIDAndSocketID;			// ���� ���� �������� ������ �ִ� �׷� ���� ������
	
	// kimhc / /2009-12-18 // ���� ���� ������ �����ϴ� �� ����
	BYTE				m_byNumOfItemThatICanGet;		// ���� �� �ִ� ���� ���� ������ ����



public:

	enum QUEST_UI_MODE
	{
		QUM_QUEST,
		QUM_QUEST_RECEIVE,
		QUM_EPIC_QUEST,
		QUM_QUEST_DETAIL,
		QUM_EVENT_QUEST,
#ifdef ACCEPT_QUEST_TAB_FIX
		QUM_EVENT_QUEST_RECEIVE,
#endif // ACCEPT_QUEST_TAB_FIX
	};


	enum QUEST_STATE_SORT
	{
		QSS_COMPLETE_EPIC,
		QSS_COMPLETE,
		QSS_WAIT_EPIC,
		QSS_FAILED,
		QSS_ONGOING,
		QSS_AVAILABLE,
		QSS_END,
	};

	enum QUEST_TYPE_SORT
	{
		QTS_EPIC,
		QTS_CONTEST,
		QTS_EVENT,
		QTS_PCBANG,
		QTS_CHANGE_JOB,
		QTS_SKILL,
		QTS_NORMAL,
	};

	enum REPEAT_TYPE_SORT
	{
		RTS_NORMAL,
		RTS_REPEAT,
		RTS_DAY,
	};

	struct UIQuickQuestInfo
	{
		CKTDGUIDialogType	m_pDLGUIQuickQuest;
		bool				m_bIsExpand;

		float				m_fExpandSizeY;

		int					m_QuestID;
		int					m_DungeonID;
		QUEST_STATE_SORT	m_eQuestState;
		QUEST_TYPE_SORT		m_eQuestType;
		REPEAT_TYPE_SORT	m_eRepeatType;

		CKTDGUIButton*		m_pButtonMinimize;
		CKTDGUIButton*		m_pButtonExpand;
#ifdef REFORM_QUEST
		CKTDGUIButton*		m_pButtonComplete;
#endif //REFORM_QUEST
		CKTDGUIStatic*		m_pStaticExpandBGNormal;
		CKTDGUIStatic*		m_pStaticExpandBGOver;

		CKTDGUIStatic*		m_pStaticQuestTitle;
		CKTDGUIStatic*		m_pStaticQuestInfo;

		UIQuickQuestInfo()
		{

			m_QuestID		= -1;
			m_DungeonID		= 0;

			m_eQuestState	= CX2UIQuestNew::QSS_AVAILABLE;
			m_eQuestType	= CX2UIQuestNew::QTS_NORMAL;
			m_eRepeatType	= CX2UIQuestNew::RTS_NORMAL;
			m_bIsExpand = false;
			m_fExpandSizeY	= 0.f;

			m_pButtonMinimize		= NULL;
			m_pButtonExpand			= NULL;
#ifdef REFORM_QUEST
			m_pButtonComplete		= NULL;
#endif //REFORM_QUEST
			m_pStaticExpandBGNormal = NULL;
			m_pStaticExpandBGOver	= NULL;

			m_pStaticQuestTitle		= NULL;
			m_pStaticQuestInfo		= NULL;
		}
	};

	struct UIQuestSlotInfo
	{	
		bool				m_bIsCategory;
		bool				m_bIsCategoryOpen;
		int					m_QuestID;
		int					m_DungeonID;
		int					m_iPlayLevel;
		QUEST_STATE_SORT	m_eQuestState;
		QUEST_TYPE_SORT		m_eQuestType;
		REPEAT_TYPE_SORT	m_eRepeatType;

		CKTDGUIButton*		m_pButtonCategory;
		CKTDGUIStatic*		m_pStaticCategory;
		CKTDGUIRadioButton*	m_pButtonQuest;
		CKTDGUIStatic*		m_pStaticQuest;
		int					m_iQuestIndex;

		UIQuestSlotInfo()
		{
			m_bIsCategory	= false;
			m_bIsCategoryOpen = false;
			m_QuestID		= -1;
			m_DungeonID		= 0;
			m_iPlayLevel	= 0;

			m_eQuestState = CX2UIQuestNew::QSS_AVAILABLE;
			m_eQuestType = CX2UIQuestNew::QTS_NORMAL;
			m_eRepeatType = CX2UIQuestNew::RTS_NORMAL;

			m_pButtonCategory = NULL;
			m_pStaticCategory = NULL;
			m_pButtonQuest = NULL;
			m_pStaticQuest = NULL;
			m_iQuestIndex = 0;
		}
	};

	struct QuestInfo
	{
		bool m_bIsCategory;
	
		int m_iID;
		int m_iEpicGroupID;
		int m_iAfterQuestID;
		QUEST_STATE_SORT m_eQuestState;
		QUEST_TYPE_SORT m_eQuestType;
		REPEAT_TYPE_SORT m_eRepeatType;
		int m_iConditionLevel;
		int m_iPlayLevel;
		int m_iFairLevel;

		QuestInfo()
		{
			m_bIsCategory = false;
			m_iID = -1;
			m_iEpicGroupID = -1;
			m_iAfterQuestID = -1;
			m_eQuestState = CX2UIQuestNew::QSS_AVAILABLE;
			m_eQuestType = CX2UIQuestNew::QTS_NORMAL;
			m_eRepeatType = CX2UIQuestNew::RTS_NORMAL;
			m_iConditionLevel = 0;
			m_iPlayLevel = 0;
			m_iFairLevel = 0;

		}
	};
	

	struct EpicQuestInfoSort : public std::binary_function< QuestInfo, QuestInfo, bool >
	{
		bool operator()( const QuestInfo& _Left, const QuestInfo& _Right ) const
		{
			if(_Left.m_iEpicGroupID == _Right.m_iEpicGroupID)
			{
				if(_Left.m_iID == _Right.m_iID)
				{
					return true;
				}
				else if(_Left.m_iID > _Right.m_iID)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if(_Left.m_iEpicGroupID > _Right.m_iEpicGroupID)
			{
				return true;
			}
			else
			{
				return false;
			}
		};
	};



	struct QuestInfoSort : public std::binary_function< QuestInfo, QuestInfo, bool >
	{
		bool operator()( const QuestInfo& _Left, const QuestInfo& _Right ) const
		{
			if(_Left.m_eQuestState == _Right.m_eQuestState)
			{
				if(_Left.m_eQuestType == _Right.m_eQuestType)
				{
					if(_Left.m_eRepeatType == _Right.m_eRepeatType)
					{
						if(_Left.m_iConditionLevel == _Right.m_iConditionLevel)
						{
							if(_Left.m_iID == _Right.m_iID)
							{
								return true;
							}
							else if(_Left.m_iID < _Right.m_iID)
							{
								return true;
							}
							else
							{
								return false;
							}
						}
						else if(_Left.m_iConditionLevel < _Right.m_iConditionLevel)
						{
							return true;
						}
						else
						{
							return false;
						}
					}
					else if(_Left.m_eRepeatType < _Right.m_eRepeatType)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else if(_Left.m_eQuestType < _Right.m_eQuestType)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if(_Left.m_eQuestState < _Right.m_eQuestState)
			{
				return true;
			}
			else
			{
				return false;
			}
		};
	};

	//������Ʈ ���Ŀ�
	struct QuickQuestInfoSort : public std::binary_function< QuestInfo, QuestInfo, bool >
	{
		bool operator()( const QuestInfo& _Left, const QuestInfo& _Right ) const
		{
			if(_Left.m_eQuestState == _Right.m_eQuestState)
			{
				if(_Left.m_iFairLevel == _Right.m_iFairLevel)
				{
					if(_Left.m_iID == _Right.m_iID)
					{
						return true;
					}
					else if(_Left.m_iID < _Right.m_iID)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else if(_Left.m_iFairLevel < _Right.m_iFairLevel)
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			else if(_Left.m_eQuestState < _Right.m_eQuestState)
			{
				return true;
			}
			else
			{
				return false;
			}
		};
	};


	// oasis907 : ����� [2010.6.21] // 
	// UIQuestListInfo�� Quest UI �� �ѷ����� �� ĭ�� ����Ʈ ����̴�.
	// �ϳ��� Quest ������ �ɼ���, �ƹ��� ������ ���� ���� ī�װ��� �� ���� �ִ�.
	UIQuestSlotInfo		m_vecUIQuestSlotInfo[_CONST_UIQUESTNEW_INFO_::g_iNumQuestSlotPerPage];

	vector<QuestInfo>	m_vecOnGoingQuestInfo;
	vector<QuestInfo>	m_vecAvailQuestInfo;
#ifdef REFORM_QUEST
	vector<QuestInfo>	m_vecOnGoingEventQuestInfo;
	std::map<int, bool> m_mapEpicQuestProgressInfo;	//��������Ʈ ī�װ� �� ���� ���� <EpicGroupID, bIsProgress>
	std::set<int>		m_setNewEventQuestIDList;	//���Ӱ� �ڵ� ������ ����Ʈ ���
#endif //REFORM_QUEST


	vector<QuestInfo>	m_vecEpicQuestInfo;
	vector<QuestInfo>	m_vecQuestInfoList;
	vector<QuestInfo>	m_vecQuickQuest;		//������Ʈ ���Ŀ� ����


	vector<int>			m_vecOnGoingEpicQuestID;

	CKTDGUIDialogType	m_pDLGUIQuestList;

	CKTDGUIDialogType	m_pDLGUIQuestDetail;

	UIQuickQuestInfo	m_vecUIQuickQuest[_CONST_UIQUESTNEW_INFO_::g_iMaxNumOngoingQuest];

	int					m_iMaxLengthQuestList;
	
	QUEST_UI_MODE		m_eNowQuestUIMode;

	bool				m_bIsOpenAvailCategory;
	bool				m_bIsOpenOngoingCategory;
	bool				m_bIsOpenEventCategory;
	int					m_iOpenEpicCategory;

	std::set<int>		m_setOpenQuickQuestID;



	// ����Ʈ â ������ �� �� ���� �켱 ������ ���̴� ����
	bool				m_bIsTalkingQuestNPC;
	bool				m_bAvailableQuestNPCHas;
	bool				m_bCompleteQuestInEpicTab;
	bool				m_bCompleteQuestInNormalTab;
	bool				m_bOngoingQuestInEpicTab;
	bool				m_bOngoingQuestInNormalTab;
	
	int					m_iCompleteEpicCategory;
	int					m_iOngoingEpicCategory;

	int					m_iSizeQuestInfoList;

	int					m_iTopQuestSlotIndex;

	QUEST_UI_MODE		m_eLastQuestUIMode;

	int					m_iSelectedQuestID;
	int					m_iSelectedQuestSlotIndex;

	// ��ũ��Ʈ���� �Ľ��ϴ� ���
	int			m_iLineWidthQuestClearCondition;
	int			m_iLineWidthNPCQuestDescription;

#ifdef GUIDE_QUICK_QUEST_COMPLETE
	int			m_iShowQuickQuest;	// ������ // 2012-05-14 // �������� ������Ʈ ����
#endif //GUIDE_QUICK_QUEST_COMPLETE
	// ToolTip : TalkBox �̿��ؼ�.
	CX2TalkBoxManager*		m_pTalkBoxManager;

	CKTDGFontManager::CUKFont* m_pFontQuestClearCondition;
	CKTDGFontManager::CUKFont* m_pFontNPCQuestDescription;

	CKTDGFontManager::CUKFont* m_pFontQuickQuestClearCondition;

	wstring GetLocalMapQuestDesc( std::vector<QuestInfo>& vecQuestInfo , int iDungeonID);
	bool GetLocalMapQuestDesc(int iDungeonID, OUT wstring& QuestDesc);
	bool	GetPartyUIQuestIcon( std::vector<QuestInfo>& vecQuestInfo, int iDungeonID, int iDungeonDifficulty, int iDungeonMode);
	bool	GetPartyUIQuestIcon( int iDungeonID, int iDungeonDifficulty, int iDungeonMode);



	wstring CreateStringQuestClearCondition();
	wstring CreateStringNPCQuestDescription();

	void ProcessQuestDesc( const wstring& wstrDesc, const int iLineWidth, CKTDGFontManager::CUKFont* pFont );

	void UpdateQuestClearCondition();
	void UpdateNPCQuestDescription();

	void ShowUIDesc( bool bShow, wstring wstr = L"", D3DXVECTOR2 pos = D3DXVECTOR2(0,0), 
		CX2TalkBoxManagerImp::TALK_BOX_TYPE TBT = CX2TalkBoxManagerImp::TBT_FROM_UP_LEFT,
		D3DXCOLOR coTextColor = D3DXCOLOR(0,0,0,1), D3DXCOLOR coBackColor = D3DXCOLOR(1, 0.96f, 0.6f, 1 ) );


	void UpdateDetailQuestTitle();
	void UpdateRewardInfo(); // EXP, SP, ED ��Ʈ��

	void InitRewardItemSlot();
	void UpdateRewardItemSlot();

	void UpdateQuestButton();

	bool IsTalkingQuestNPC();
	void SetTalkingQuestNPC(bool bVal, CX2UnitManager::NPC_UNIT_ID NpcID = CX2UnitManager::NUI_NONE);

	void ReturnToQuestListUI();

	void SetNowQuestUIMode(QUEST_UI_MODE _eNowQuestUIMode);

	bool IsStateQuickQueskFilter();
	bool IsStateSubQuestCanClear( int iSubQuestID, int iNowDungeonID );


	QUEST_TYPE_SORT ConvertToQuestTypeSort( int _eQuestType );
	REPEAT_TYPE_SORT ConvertToRepeatTypeSort( int _eQuestRepeatType );
#ifdef MODIFY_QUEST_UI
	QUEST_STATE_SORT ConvertToStateTypeSort( int eQuestStateType_ );
#endif //MODIFY_QUEST_UI

	int GetQuestPictureNumber(QUEST_TYPE_SORT _eQuestTypeSort, REPEAT_TYPE_SORT _eRepeatTypeSort);
	int GetQuestPictureNumber(QUEST_STATE_SORT _eQuestStateSort);

	bool GetUnitQuest( std::vector<int>& vecQuestID );
	bool GetCompleteEpicQuest( std::vector<int>& vecQuestID );

	void UpdateAvailQuestInfo( CX2UnitManager::NPC_UNIT_ID eNPCID = CX2UnitManager::NUI_NONE );
	void UpdateOngoingQuestInfo();
	void UpdateUIQuestSlotInfo( UINT iQuestSlotInfoIndex,  QuestInfo& QuestInfo );

	void SetUIQuestSlotInfoToCategory( UINT iQuestSlotInfoIndex,  QuestInfo& QuestInfo );
	void SetUIQuestSlotInfoToEpicCategory( UINT iQuestSlotInfoIndex,  QuestInfo& QuestInfo );

	void EpicQuestSort( std::vector<QuestInfo>& vecEpicQuestInfo);


	void InitUIQuestSlotInfo();
	void HideUIQuestSlotInfo();

	QUEST_UI_MODE DetermineUIModeWithTabPriority();
	void ResetTabPriorityFactor();


	void ResetQuestList(QUEST_UI_MODE eQuestUIMode);
	void ResetQuestUI( bool bUpdateNavi = true, bool bResetExpand = false, bool bIgnoreTabPriority = true);


	void NewEpicStartEffect();
	//�ܺ� �������̽� UIQuest
	// UpdateQuickQuestDLG
	// GetShowQuickQuestDLG
	// UpdateQuestUI

	void FocusQuest( bool bDown );

	wstring GetNPCFaceTextureKey(CX2UnitManager::NPC_UNIT_ID NpcID);


	void MinimizeAllUIQuickQuest();
	void MinimizeAllUIQuickQuestWithExpandInfo();

	void ExpandAllUIQuickQuest();
	void ExpandAllUIQuickQuestWithExpandInfo();


	void ExpandUIQuickQuest(int iSelectedUIQuickQuestIndex, bool bMouseOver = true);
	void MinimizeUIQuickQuest(int iSelectedUIQuickQuestIndex, bool bPreserveExpandInfo = false);
#ifdef GUIDE_QUICK_QUEST_COMPLETE
	void MouseOverUIQuickQuest(int iSelectedUIQuickQuestIndex, D3DXVECTOR2 vPos);
#else
	void MouseOverUIQuickQuest(int iSelectedUIQuickQuestIndex);
#endif //GUIDE_QUICK_QUEST_COMPLETE
	void MouseOutUIQuickQuest(int iSelectedUIQuickQuestIndex);


	bool GetOverMouseQuest();
	bool GetShowQuickQuestDLG();
	void SetShowQuickQuestDLG( bool forOpen );
	void UserShowQuickQuestDLG( bool forOpen );
	void InitUserShowQuickQuestDLG( bool forOpen );
	void UpdateQuickQuestDLG(bool bResetExpand = false, bool bUpdateOngoingQuest = true); //������ // 2012-10-02 // ResetQuestUI���� ȣ��Ǿ��� ��쿡�� UpdateOngoingQuestInfo�ߺ� ���� ���� �ʵ��� ���� �߰�(bUpdateOngoingQuest)
	void ToggleUserShowQuickQuestDLG()
	{ 
#ifdef FIX_UI_OFF_TAB_OFF
		if( g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog() == true )
			return;
#endif //FIX_UI_OFF_TAB_OFF
		m_bUserShowQuickQuestDLG = !m_bUserShowQuickQuestDLG; 
		UpdateQuickQuestDLG();UserShowQuickQuestDLG(m_bUserShowQuickQuestDLG); ShowUIDesc(false); 
	}
	bool	m_bShowQuickQuestDLG;	// �ý��� �� ������ �������� ������Ʈâ�� �¿���
	bool	m_bUserShowQuickQuestDLG; // ������ ���ϴ� ������Ʈâ�� �¿���

#ifdef REFORM_QUEST
	bool GetSubQuestDungeonIDList( IN int iSubQuestID, OUT vector<int>& vecDungeonID_ );
	void ClearQuestCompleteParticle();
#else
	int GetSubQuestDungeonID( int iSubQuestID );
#endif //REFORM_QUEST

	bool SetUIQuickQuestSlotInfo(int iQuestSlotInfoIndex, QuestInfo& QuestInfo );

#ifdef REFORM_NOVICE_GUIDE
	void SetNoviceGuideStep();
#endif //REFORM_NOVICE_GUIDE

#ifdef GUIDE_QUEST_MONSTER
	bool GetIsQuestMonster( CX2UnitManager::NPC_UNIT_ID eNPCUnitID_ ){ return (m_setQuestNPCList.find(eNPCUnitID_) != m_setQuestNPCList.end()); }
#endif //GUIDE_QUEST_MONSTER

	void UpdateQuestMonster();
private:
#ifdef REFORM_QUEST
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hQuestCompleteParticle;
#endif //REFORM_QUEST

#ifdef GUIDE_QUEST_MONSTER
	void SetQuestMonsterByQuestInfo( const vector<QuestInfo>& vecQuestInfo_ );
	std::set<CX2UnitManager::NPC_UNIT_ID>	m_setQuestNPCList;
#endif //GUIDE_QUEST_MONSTER

#ifdef DAY_OF_THE_WEEK_QUEST
	// ���� ����Ʈ ��ȿ�� ���� üũ.
	// ���� ����Ʈ�� �ƴ϶�� ������ treu ��ȯ.
	bool GetIsValidDayTheWeekQuest( int iQuestID_ ) ;
#endif //DAY_OF_THE_WEEK_QUEST
};
#endif SERV_EPIC_QUEST
