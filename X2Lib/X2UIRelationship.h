#pragma once

#ifdef ADDED_RELATIONSHIP_SYSTEM

namespace		_CONST_UI_RELATIONSHIP_
{
	const int		g_iMaxInvitedUser = 30;
	const int		g_iInvitingRubenWeddingHallMany = 15; 
	const int		g_iInvitingSanderWeddingHallMany = 30; 
	const int		g_iMaxReserveDay = 7;
	const int		g_iMaxInviteUserViewCount = 8;
	const int		g_iMaxInviteUser = 1000;
}



class CX2UIRelationship
{
public :
	
	enum	RELATIONSHIP_UI_MESSAGE
	{
		RUM_RELATIONSHIP_PROPOSE 							= 58000,						
		RUM_RELATIONSHIP_FAREWELL,

		RUM_COUPLE_INVITE_USE_ITEM_OK,
		RUM_COUPLE_INVITE_USE_ITEM_CANCEL,

		RUM_COUPLE_INVITE_OK,		
		RUM_COUPLE_INVITE_CANCEL,			
		
		RUM_COUPLE_INVITE_DENY_OK,				
		RUM_COUPLE_INVITE_BUSY_OK,			
		RUM_COUPLE_INVITE_TARGET_IS_COUPLE_OK,	
		RUM_COUPLE_INVITE_I_AM_COUPLE_OK,
		
		RUM_WEDDING_INVITE_OK,
		RUM_WEDDING_INVITE_CANCEL, 

		RUM_WEDDING_INVITE_ADD_INVITE_USER_ID,
		RUM_WEDDING_INVITE_POP_FRIEND_LIST,				
		RUM_WEDDING_INVITE_POP_GUILD_LIST,


		RUM_WEDDING_INVITE_POP_FRIEND_LIST_OK,
		RUM_WEDDING_INVITE_POP_FRIEND_LIST_CANCEL,
		RUM_WEDDING_INVITE_POP_ADD_LIST,
		RUM_WEDDING_INVITE_POP_ADD_LIST_OK,

		RUM_WEDDING_INVITE_USE_ITEM_OK,
		RUM_WEDDING_INVITE_USE_ITEM_CANCEL,

		RUM_WEDDING_INVITE_USER_LIST_LEFT_ARROW,		
		RUM_WEDDING_INVITE_USER_LIST_RIGHT_ARROW,

		RUM_WEDDING_INVITE_USER_LIST_CHECKED,
		RUM_WEDDING_INVITE_USER_LIST_UNCHECKED,

		RUM_COUPLE_CHANGE_LOVE_MESSAGE,
		RUM_COUPLE_CHANGE_LOVE_MESSAGE_OK,
		RUM_COUPLE_CHANGE_LOVE_MESSAGE_CANCEL,


		RUM_COUPLE_INVITED_OK,
		RUM_COUPLE_INVITED_CANCEL,
		RUM_RELATIONSHIP_REFRESH_DATA,

		RUM_WEDDING_INVITE_CHANGE_WEDDING_HALL,

		RUM_COUPLE_FAREWELL_OK,
		RUM_COUPLE_FAREWELL_CANCEL,

		RUM_COUPLE_BUY_RING,
		RUM_COUPLE_BUY_RING_OK,
		RUM_COUPLE_BUY_RING_CANCEL,

		RUM_WEDDING_INVITE_USER_LIST_ALL_CHECKED,
		RUM_WEDDING_INVITE_USER_LIST_ALL_UNCHECKED,
	};

	enum	RELATION_INVITE_USER_LIST_TYPE	// ���������� �ʴ��� ��� â�� ���� Ÿ��
	{
		RIT_FRIEND = 0,				// ģ�� ���
		RIT_GUILD,					// ��� ���
		RIT_ADDED,					// �߰��� ���
		RIT_NICKNAME,				// �г��� �Է�
	};
	CX2UIRelationship::CX2UIRelationship ();
	CX2UIRelationship::~CX2UIRelationship();
	


	struct InviteWeddingUserInfo
	{
		wstring		m_wstrUserName;
		UidType		m_uiUserUID;

		InviteWeddingUserInfo () : m_wstrUserName (0), m_uiUserUID (0) {}
		InviteWeddingUserInfo ( wstring wstrUserName_, UidType UserUID_ ) { m_wstrUserName = wstrUserName_; m_uiUserUID = UserUID_; }
	};


	HRESULT			OnFrameMove( double fTime, float fElapsedTime );
	bool			UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool			UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool			ShowInviteCoupleDlg( bool bEnable );		
	bool			ShowInviteWeddingDlg ( bool bEnable );
	bool			ShowInviteWeddingUserListDlg ( bool bEnable, bool bIsSave, RELATION_INVITE_USER_LIST_TYPE eType_ );
	bool			ShowInvitedWeddingUserListControlListDlg ( bool bEnable );
	void			ShowChangeLoveMessageDlg ( bool bEnable, UidType ItemUid_ = 0 );
	bool			ShowJoinWeddingDlg ( bool bEnable, bool bIsWeddingHeroine = false, UidType itemUID_ = 0 );

	void			AddInviteWeddingFriendList ( vector<InviteWeddingUserInfo> vecInviteWeddingUser );

	void			AddInvitingUserInfo ( InviteWeddingUserInfo UserInfo_, RELATION_INVITE_USER_LIST_TYPE eType_ = RIT_GUILD );
	void			ResetInvitingUserInfo ();
	void			ResetInviteWeddingUserList () { m_vecInviteWeddingUserList.clear(); }
//	void			AddGuildMemberInfoListUsingRelationship (InviteWeddingUserInfo UserInfo_ ) { m_vecGuildMemberInfoList.push_back(UserInfo_); }
	UINT			GetNowInviteUserPage () const	{ return m_uiNowInviteUserPage; }
	void			SetNowInviteUserPage (UINT val)  { m_uiNowInviteUserPage = val; }
	UINT			GetMaxInviteUserPage() const { return m_uiMaxInviteUserPage; }
	void			SetMaxInviteUserPage( UINT val ) { m_uiMaxInviteUserPage = val; }
	void			AddInviteWeddingUserList ( InviteWeddingUserInfo UserInfo_ );
	int				GetNowUserButtonIndexNumber ( int iIndex ) { return ( GetNowInviteUserPage() - 1 ) * _CONST_UI_RELATIONSHIP_::g_iMaxInviteUserViewCount + iIndex; }
	void			CheckWeddingInviteUserList ( CKTDGUICheckBox* pCheckBox_ );
	void			UnCheckWeddingInviteUserList ( CKTDGUICheckBox* pCheckBox_ );
	void			GetInvitingWeddingUserListFromFriendList ( int idx );
	void			GetFriendListForInvitingWeddingUser ();
	int				GetMaxWeddingInviteNumber() const { return m_iMaxWeddingInviteNumber; }
	void			SetMaxWeddingInviteNumber(int val) { m_iMaxWeddingInviteNumber = val; }

	bool			SetSelecteWeddingHallByPropose( int iWeddingLetterMany );
private:
	CKTDGUIDialogType			m_pDLGInviteCoupleMessage;	//	Ŀ�ø� �޽��� �Է� Dialog
	CKTDGUIDialogType			m_pDLGChangeLoveMessage;	//	��Ī �޽��� �Է� Dialog
	CKTDGUIDialogType			m_pDLGInviteWedding; // ûø�� �ʴ� Dialog
	CKTDGUIDialogType			m_pDLGInviteWeddingUserList;
	CKTDGUIDialogType			m_pDLGJoinWedding;
	
	int									m_iMaxWeddingInviteNumber;
	vector<InviteWeddingUserInfo>		m_vecInviteWeddingUserList;  // �ʴ� �� ����
	vector<InviteWeddingUserInfo>		m_vecInvitedWeddingUserList; // �ʴ� �� ����
//	vector<InviteWeddingUserInfo>		m_vecGuildMemberInfoList;	// ��� ���� ����
	UINT								m_uiNowInviteUserPage;		// �ʴ� DLG �� ���� ������
	UINT								m_uiMaxInviteUserPage;		// �ʴ� DLG �� �ִ� ������
	RELATION_INVITE_USER_LIST_TYPE		m_eRelationInviteUserListType;	// �ʴ� DLG �� Ÿ��
	UidType								m_UidSelectedIItemUID;	// ������ �ʴ��� UID 
	vector<InviteWeddingUserInfo>       m_vecFriendMemberInfoList;
	//	float								m_f
	//	vector<wstring >					m_vecFriendNameList;
};

#endif // ADDED_RELATIONSHIP_SYSTEM
