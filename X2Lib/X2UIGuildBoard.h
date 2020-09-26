#ifdef	GUILD_BOARD

#pragma once

namespace		_CONST_UIGUILDBOARD_INFO_
{
	const	int		g_iNumAdvertisePerPage		= 6;		// �� ������ �� ��Ÿ�� �� �ִ� ���� ����
	const	int		g_iNumJoinListPerPage		= 6;		// �� ������ �� ��Ÿ�� �� �ִ� ���� ��û ����
	const	int		g_iGuildLevelLimit			= 3;		// ��� ������ 3�ڸ� ������
}

class CX2UIGuildBoard
{
public:


	enum UI_GUILD_BOARD_CUSTOM_MSG
	{
		UGBCM_EXIT							= 37000,
		UGBCM_JOIN_LIST_EXIT,
		UGBCM_ADVERTISE_EXIT,
		UGBCM_SELECT_GUILD_AD_TAP,
		UGBCM_SELECT_GUILD_NUM_MEMBER,
		UGBCM_SELECT_GUILD_LEVEL,
		UGBCM_SELECT_GUILD_NEW,
		UGBCM_SELECT_GUILD_HOT,
		UGBCM_SELECT_ADVERTISE_ED,
		UGBCM_PUSH_PREV_PAGE,
		UGBCM_PUSH_NEXT_PAGE,
		UGBCM_PUSH_JOIN,
		UGBCM_PUSH_JOIN_MESSAGE_OK,
		UGBCM_PUSH_JOIN_MESSAGE_CANCEL,
		UGBCM_VERIFY_JOIN_MESSAGE_OK,
		UGBCM_PUSH_SKILL,
		UGBCM_PUSH_ADVERTISE,
		UGBCM_PUSH_ADVERTISE_MESSAGE_OK,
		UGBCM_PUSH_ADVERTISE_MESSAGE_CANCEL,
		UGBCM_VERIFY_ADVERTISE_MESSAGE_OK,
		UGBCM_MODIFY_ADVERTISE_MESSAGE_OK,
		UGBCM_PUSH_JOIN_LIST,	
		UGBCM_PUSH_PREV_PAGE_JOIN_LIST,
		UGBCM_PUSH_NEXT_PAGE_JOIN_LIST,
		UGBCM_PUSH_ACCEPT,	
		UGBCM_PUSH_REMOVE,
		UGBCM_SELECT_JOIN_LIST,
		UGBCM_SELECT_REG_DATE,
	};
	//{{ oasis907 : ����� [2010.2.16] // 
	enum SORT_TYPE
	{
		ST_REG_DATE = 0,
		ST_NEW_GUILD,
		ST_HOT_GUILD,
		ST_MEMBER_COUNT,
		ST_GUILD_LEVEL,
		ST_MAX_NUM,
	};
	//}

	//{{ oasis907 : ����� [2009.12.28] //
	struct UserSkillData
	{
		int m_iSkillLevel;
		int m_iSkillCSPoint;

		UserSkillData() : 
		m_iSkillLevel( 0 ),
			m_iSkillCSPoint( 0 )
		{
		}

		UserSkillData( int iLevel, int iSkillCSPoint )
		{
			m_iSkillLevel = iLevel;
			m_iSkillCSPoint = iSkillCSPoint;
		}
	};
	//}} oasis907 : ����� [2009.12.28] //


	struct ColumnForAdvertise
	{
		CKTDGUIStatic* m_pStaticGuildName;					// ��� �̸�
		CKTDGUIStatic* m_pStaticGuildLevel;				// ��� ����
		CKTDGUIStatic* m_pStaticAdvertiseMessage;				// ��� ���� �޽���

		CKTDGUIButton* m_pButtonDescGuild;				// ��� ���� ����
		CKTDGUIButton* m_pButtonDescMessage;				// ��� ���� �޽��� ����

		CKTDGUIButton* m_pButtonJoin;	
		CKTDGUIButton* m_pButtonSkill;	

		CKTDGUIStatic* m_pStaticButtonJoin;
		CKTDGUIStatic* m_pStaticButtonSkill;

		int m_iGuildUID;				

		KGuildAdInfo m_KGuildAdInfo;

		ColumnForAdvertise() 
		{
            m_pStaticGuildName = NULL;
			m_pStaticGuildLevel = NULL;
			m_pStaticAdvertiseMessage = NULL;

			m_pButtonDescGuild = NULL;	
			m_pButtonDescMessage = NULL;			

			m_pButtonJoin = NULL;	
			m_pButtonSkill = NULL;	

			m_pStaticButtonJoin = NULL;	
			m_pStaticButtonSkill = NULL;	

			m_iGuildUID = 0;
		}
	};

	


	struct ColumnForJoinList
	{
		CKTDGUIDialogType m_pCharacterIcon;

		UidType m_iUnitUID;

		CKTDGUIRadioButton* m_pRadioJoinList;
		CKTDGUIStatic* m_pStaticJoinList;				
		CKTDGUIButton* m_pButtonDescMessage;				// ���� ��û �޽��� ����


		KApplyJoinGuildInfo m_KApplyJoinGuildInfo;


		ColumnForJoinList() 
		{
			m_pCharacterIcon = NULL;
			
			m_pRadioJoinList = NULL;
			m_pStaticJoinList = NULL;
			m_pButtonDescMessage = NULL;

			m_iUnitUID = 0;

		}
	};




public:
	CX2UIGuildBoard( const CKTDXStage* const pNowStage, const WCHAR* const pFileName );
	~CX2UIGuildBoard();

	HRESULT				OnFrameMove( double fTime, float fElapsedTime );
	bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				GetShow() const;
	void				SetShow( bool bShow );
	D3DXVECTOR2			GetDLGSize() const;
	void				SetPosition( D3DXVECTOR2 vec );
	void				SetLayer( X2_DIALOG_LAYER layer );


//{{ oasis907 : ����� [2009.12.18] //
	CX2UIGuildSkillTreeInfo* GetUIGuildSkillTreeInfo() { return m_pGuildSkillTreeInfo; }
	// oasis907 : ����� [2009.12.28] //
	void				SetRequiredGuildSkill( const std::vector<KGuildSkillData>& vecSkillList );




private:
	void				InitUIGuildBoard();
	void				InitColumnAdvertise();

	void				SetShowControlsByType( SORT_TYPE type, bool bShow );

	void				SetShowColumn( ColumnForAdvertise* const pColumnForAdvertise, SORT_TYPE type, bool bShow );
	void				SetShowColumn( ColumnForJoinList* const pColumnForJoinList, bool bShow, UINT uiListIndex ); // ���� ��û ���


	void				UpdatePageUI() const;	  // ��� �Խ����� ������ ��ȣ
	void				UpdatePageJoinUI() const; // ���� ��û ����� ������ ��ȣ

	void				UpdateButtonUI(bool bCanShowApplyList); // ��層����, ��û��Ϻ��� ��ư. 
	void				UpdateJoinListButtonUI(UINT uiJoinListIndex); // ���Խ�û����� ���� ����, �����ϱ� ��ư


	// Ȱ��ȭ, ��Ȱ��ȭ�� �̿� ���� ��ư�� �׷��� ó��.
	void				SetEnableButtonAccept(bool bEnable); // ���� ���� ��ư 
	void				SetEnableButtonRemove(bool bEnable); // �����ϱ� ��ư
	void				SetEnableButtonAdvertise(bool bEnable); // ���� ��� ��ư
	void				SetEnableButtonJoinList(bool bEnable); // ��û ��� ��ư

	std::wstring		ConvertTimeToRegisterDateStr( __int64 tRegisterDate );
	wstring				CutString( CKTDGUIStatic* pSeedStatic, int iWidth, const WCHAR* wszString, const WCHAR* wszReplaceString = L"");

	void				SetNumImage( CKTDGUIStatic* const pStaticNumImg, int iRankNum );
	void				SetNumToPicture( CKTDGUIControl::CPictureData* const pPictureData, int num );

	// oasis907 : ����� [2009.12.29] // ���� ��û ���
	void				InitColumnJoinList();

public:

	int					GetGuildSkillLevel( CX2SkillTree::SKILL_ID eSkillID );
	bool				IsMaxGuildSkillLevel( IN CX2SkillTree::SKILL_ID eSkillID );
	bool				GetGuildSkillLevelAndCSP( IN CX2SkillTree::SKILL_ID eSkillID, OUT int& iSkillLevel, OUT int& iSkillCSPoint );
	void				CalcUsedGuildSPointAndCSPoint( OUT int& iSPoint, OUT int& iCSPoint );
	bool				CanILearnThisSkill( CX2SkillTree::SKILL_ID eSkillID, bool bCheckSP = true );





	bool				Handler_EGS_GET_GUILD_AD_LIST_REQ();
	bool				Handler_EGS_GET_GUILD_AD_LIST_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_GET_GUILD_SKILL_IN_BOARD_REQ();
	bool				Handler_EGS_GET_GUILD_SKILL_IN_BOARD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_REGISTRATION_GUILD_AD_REQ( wstring &wstrMessage ) const;
	bool				Handler_EGS_REGISTRATION_GUILD_AD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_MODIFY_REG_GUILD_AD_REQ( wstring &wstrMessage ) const;
	bool				Handler_EGS_MODIFY_REG_GUILD_AD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );



	bool				Handler_EGS_APPLY_JOIN_GUILD_REQ( wstring &wstrMessage, bool bOldApplyDelete );
	bool				Handler_EGS_APPLY_JOIN_GUILD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_GET_APPLY_JOIN_GUILD_LIST_REQ();
	bool				Handler_EGS_GET_APPLY_JOIN_GUILD_LIST_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_ACCEPT_JOIN_GUILD_REQ();
	bool				Handler_EGS_ACCEPT_JOIN_GUILD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_DELETE_APPLY_JOIN_GUILD_REQ();
	bool				Handler_EGS_DELETE_APPLY_JOIN_GUILD_ACK(  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool				SetShowGuildAdvertiseMessageDlg(bool bEnable);
	bool				SetShowGuildJoinMessageDlg(bool bEnable);




private:
	bool								m_bShow;

	bool								m_bOldApplyDelete;
	bool								m_bRegMyGuildAd; 

	std::wstring						m_wstrLastMessage;
	std::wstring						m_wstrLastADMessage;

	const CKTDXStage* const				m_pNowStage;
	CKTDGUIDialogType					m_pDlgGuildBoard;
									


	SORT_TYPE							m_nowSortType;				// ���� ������� �� ��ŷ ���� Ÿ��(��ϸ� ����, ���� ��)
	UINT								m_uiNowPage;						// ���� �������� ������ ��
	UINT								m_uiMaxPage;						// ���� ������ �� �ִ� �ִ� ������ ��

	UINT								m_uiNowPageJoinList;					// ���� �������� ������ �� (���� ��û ���)
	UINT								m_uiMaxPageJoinList;					// ���� ������ �� �ִ� �ִ� ������ ��(���� ��û ���)

	UINT								m_uiListIndexForJoin;				 // ��� ���� ��û ��ư ����Ʈ �ε���
	UINT								m_uiListIndexForSkill;				// ��� ��ų ���� ��ư ����Ʈ �ε���


	UINT								m_uiJoinListIndex;			 // ��� ���� ��û ��ư ����Ʈ �ε���


	short								m_sAdvertisePeriod;					

	D3DXVECTOR2							m_vSizeDlg;
	D3DXVECTOR2							m_vPosDlg;

	

	CKTDGUIRadioButton*					m_pRadioGuildNumMember;
	CKTDGUIRadioButton*					m_pRadioGuildLevel;
	CKTDGUIRadioButton*					m_pRadioGuildNew;
	CKTDGUIRadioButton*					m_pRadioGuildHot;
	//{{ oasis907 : ����� [2010.2.16] // 
	CKTDGUIRadioButton*					m_pRadioRegDate;


	CKTDGUIButton*						m_pButtonAdvertise;	
	CKTDGUIButton*						m_pButtonJoinList;

	CKTDGUIButton*						m_pButtonAccept;	
	CKTDGUIButton*						m_pButtonRemove;	


	ColumnForAdvertise					m_columnForAdvertise[_CONST_UIGUILDBOARD_INFO_::g_iNumAdvertisePerPage];
	ColumnForJoinList					m_columnForJoinList[_CONST_UIGUILDBOARD_INFO_::g_iNumJoinListPerPage]; // ���� ��û ���

	// oasis907 : ����� [2009.12.18] //
	CX2UIGuildSkillTreeInfo*			m_pGuildSkillTreeInfo;

	// oasis907 : ����� [2009.12.29] //
	CKTDGUIDialogType					m_pDlgJoinList;
	CKTDGUIDialogType					m_pDlgGuildAdvertiseMessage;
	CKTDGUIDialogType					m_pDlgGuildJoinMessage;



	typedef std::map< CX2SkillTree::SKILL_ID, UserSkillData > SkillDataMap;				// <skill id, UserSkillData>
	SkillDataMap						m_mapGuildSkillRequired;


public:
	//{{ oasis907 : ����� [2009.12.28] //
	KProtectedType<int>					m_iGuildSPoint;				// ������ �ִ� ��ų ����Ʈ
	KProtectedType<int>					m_iGuildCSPoint;				// ������ �ִ� ĳ�� ��ų ����Ʈ
	KProtectedType<int>					m_iMaxGuildCSPoint; 
	std::wstring						m_wstrGuildCSPointEndDate;
	//}} oasis907 : ����� [2009.12.28] //



//}} oasis907 : ����� [2009.12.28] //


};

#endif	GUILD_BOARD