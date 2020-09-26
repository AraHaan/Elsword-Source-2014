#pragma once

class CX2CharPopupMenu : public CX2PopupUIBase
{   	
public:
	enum USER_MENU_UI_MSG
	{		
		UMUI_INFO = 17000,	/// ��������
		UMUI_WATCH,			/// ������
		UMUI_WHISPER,		/// �ӼӸ�
		UMUI_FRIEND,		/// ģ����û
		UMUI_DISCIPLE,		/// ������û
		UMUI_LEADER,		/// ��Ƽ���Ӹ�
		UMUI_PARTY,			/// ��Ƽ��û
		UMUI_INVITE,		/// ��Ƽ�ʴ�
		UMUI_OUT,			/// ����
		UMUI_TRADE,			/// ���ΰŷ�
		//{{ kimhc // 2009-09-29 // ��� ���� �޽��� �߰�
		UMUI_INVITE_GUILD_MEMBER,	/// ����ʴ�
		//}} kimhc // 2009-09-29 // ��� ���� �޽��� �߰�
		UMUI_INFO_SIMPLE,
		UMUI_MOVE_TO_PARTY,	/// ��Ƽ �Ҽ��ʵ�� �̵�
		UMUI_INPUT_PARTY,	/// �ʴ��� ĳ���͸� �Է�
		UMUI_DROP_OUT,		/// ��Ƽ Ż��
		UMUI_INPUT_EXIT,	/// ĳ�� �ʴ� ���̾�α� ����
		UMUI_INVITE_PARTY_NAME,	/// ĳ���� �̸����� ��Ƽ �ʴ�
		UMUI_SELECT_INVITE_MEMBER_SEVER,//�ʴ��� ��Ƽ�� ����
		UMUI_EXIT,			/// ����
//#ifdef ADDED_RELATIONSHIP_SYSTEM
		UMUI_COUPLE,		// Ŀ�� �ʴ�
		UMUI_SUMMON,
//#endif // ADDED_RELATIONSHIP_SYSTEM

	};

	enum USER_MENU
	{
		UM_NONE = 0,
		UM_NORMAL_NORMAL,		// �Ϲ�->�Ϲ�
		UM_NORAML_PARTY,		// �Ϲ�->��Ƽ��
		UM_NORMAL_PARTYL,		// �Ϲ�->��Ƽ��
		UM_PARTY_NORMAL,		// ��Ƽ��->�Ϲ�
		UM_PARTY_PARTY,			// ��Ƽ��->��Ƽ��
		UM_PARTY_PARTYL,		// ��Ƽ��->��Ƽ��
		UM_PARTYL_NORMAL,		// ��Ƽ��->�Ϲ�
		UM_PARTYL_PARTY,		// ��Ƽ��->��Ƽ��
	};

    CX2CharPopupMenu();
    virtual ~CX2CharPopupMenu();

	//void SetStage( CKTDXStage* pNowState  );
	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GetShowMenu() const { return m_bShow; }	
	virtual void OpenUserPopupMenu( UidType iUnitUID, bool bPartyMember = false );
	virtual void ClosePopupMenu();

	//CKTDGUIDialogType GetDialog() { return m_pDlgMenu; }

private:
	void SetUnit( UidType uidUnitUID, bool bPartyMember = false );
	void SetMode( USER_MENU eMode, bool bPartyMember = false );
	bool SetPopupMenu( UidType iUnitUID, bool bPartyMember = false );
	void OnPartyInput( bool bOn_ );
	void SetOnPopup();

protected:    
    //CX2State*				m_pNowState;
    CKTDGUIDialogType			m_pDlgMenu;

private:	
	//int						m_nMenuCount;
	//CX2Unit*				m_pUnit;
	UidType					m_iUid;
	wstring					m_wstrName;
	int						m_iUnitLevel;
	CX2Unit::UNIT_CLASS		m_eUnitClass;

	//bool							m_bShow;

	CKTDGUIControl::CPictureData*	m_pPicMiddle1;
	CKTDGUIControl::CPictureData*	m_pPicBottom1;
	CKTDGUIControl::CPictureData*	m_pPicMiddle2;
	CKTDGUIControl::CPictureData*	m_pPicBottom2;
    
	CKTDGUIControl::CPictureData*	m_pPicCharacter;		/// ĳ���� �̹���

	CKTDGUIControl::CPictureData*	m_pPicLevelTen[10];		/// ���� �ʴ���
	CKTDGUIControl::CPictureData*	m_pPicLevelOne[10];		/// ���� �ϴ���

	CKTDGUIStatic*					m_pStaticName;			/// ĳ���� �̸�

	CKTDGUIButton*					m_pButtonSimpleInfo;	/// ��������(����)
	CKTDGUIButton*					m_pButtonUserInfo;		/// ��������(���캸��)
	CKTDGUIButton*					m_pButtonWatch;			/// ������
	CKTDGUIButton*					m_pButtonwhisper;		/// �ӼӸ�
	CKTDGUIButton*					m_pButtonfriend;		/// ģ����û
	CKTDGUIButton*					m_pButtondisciple;		/// ������û
	CKTDGUIButton*					m_pButtonleader;		/// ��Ƽ���Ӹ�
	CKTDGUIButton*					m_pButtonparty;			/// ��Ƽ��û
	CKTDGUIButton*					m_pButtoninvite;		/// ��Ƽ�ʴ�
	CKTDGUIButton*					m_pButtonout;			/// ����
	CKTDGUIButton*					m_pButtontrade;			/// ���ΰŷ�

	//{{ ����� : [2009/9/18] //	��� �ʴ� ��ư
	CKTDGUIButton*					m_pButtonInviteGuild;	///	����ʴ�
#ifdef ADDED_RELATIONSHIP_SYSTEM
	CKTDGUIButton*					m_pButtonInviteCouple;	///	Ŀ���ʴ�
	CKTDGUIButton*					m_pButtonSummonCouple;	///	Ŀ�ü�ȯ
#endif // ADDED_RELATIONSHIP_SYSTEM
};
