#pragma once

//{{ ����� : [2009/9/15] //	���Ŵ��� Ŭ���� ����

namespace		_CONST_GUILD_MANAGER_
{
	const BYTE		g_byGuildAdminMagicNumber	= 10;		// ���� 10��� ��� ������ 1��
	const USHORT	g_usMaxNumOfGuildMembers	= 100;		// Ȯ�� ������ �ִ� ��� �ο�
	const int		g_iMaxGuildHonorPoint		= 1600;		// ��� ������Ʈ �ְ�ġ
	// oasis907 : ����� [2009.12.9] //  enum GUILD_USER_GRADE 
	const UCHAR		g_ucLimitGuildGrade         = 6;    // ��� ��ų ���� ���� ���ϴ� ��� ĿƮ����

}

class CX2UIGuild;

#ifdef GUILD_MANAGEMENT
class CX2GuildManager{

public:
	enum	GUILD_UI_MESSAGE
	{
		GUM_GUILD_NAME_OK			= 35001,
		GUM_GUILD_NAME_CANCEL,
		GUM_GUILD_INVITE_MEMBER_OK,				// ��� �ʴ� Ȯ��
		GUM_GUILD_INVITE_MEMBER_CANCEL,			// ��� �ʴ� ���
		GUM_GUILD_JOIN_OK,						// �ʴ뿡 ���� ��� ���� Ȯ��
		GUM_GUILD_JOIN_CANCEL,					// �ʴ뿡 ���� ��� ���� ���

		GUM_GUILD_INFO_LEAVE_BUTTON,			// ��� �����ǿ��� ���Ż�� ��ư
		GUM_GUILD_INFO_DESTROY_BUTTON,			// ��� �����ǿ��� �����ü ��ư
		GUM_GUILD_INFO_CHANGE_MESSAGE_BUTTON,	// ��� �޽��� ���� ��ư

		GUM_GUILD_INFO_CHANGE_MESSAGE_BUTTON_OK,		//	��� �޽��� ���� Dialog Ȯ�� ��ư
		GUM_GUILD_INFO_CHANGE_MESSAGE_BUTTON_CANCEL,	//	��� �޽��� ���� Dialog ��� ��ư
		GUM_GUILD_INFO_CHANGE_MESSAGE_FINAL_OK,			//	��� �޽��� ���� ���� Ȯ��

		GUM_GUILD_INFO_CLOSE,					//	��� ���� Dialog �ݱ�

		GUM_GUILD_LEAVE_OK,					// ��� Ż�� Ȯ��
		GUM_GUILD_LEAVE_CANCEL,				// ��� Ż�� ���

		GUM_GUILD_DESTROY_OK,				// ��� ��ü Ȯ��
		GUM_GUILD_DESTROY_CANCEL,			// ��� ��ü ���

		GUM_GUILD_INFO_TAB,					//	��� ���� ��
		GUM_GUILD_MEMBER_INFO_TAB,			//	���� ���� ��
		GUM_GUILD_SKILL_TAB,				//	��� ��ų ��

		GUM_GUILD_MEMBER_INFO_CHANGE_MESSAGE,			//	�λ縻����
		GUM_GUILD_MEMBER_INFO_CHANGE_MESSAGE_OK,		//	�λ縻���� Dialog Ok
		GUM_GUILD_MEMBER_INFO_CHANGE_MESSAGE_CANCEL,	//	�λ縻���� Dialog Cancel

		GUM_GUILD_MEMBER_INFO_PAGE_LEFT,	//	���� ���� ������ �ٿ�
		GUM_GUILD_MEMBER_INFO_PAGE_RIGHT,	//	���� ���� ������ ��

		GUM_GUILD_NEW_GUILD_MASTER_OK,		//	���ο� ��� ������ �Ӹ� Dialog Ȯ�� ��ư
		GUM_GUILD_NEW_GUILD_MASTER_RE_OK,		//	���ο� ��� ������ �Ӹ� Dialog ��Ȯ�� ��ư
		GUM_GUILD_NEW_GUILD_MASTER_RE_CANCEL,	//	���ο� ��� ������ �Ӹ� Dialog ��Ȯ�� ��� ��ư

		GUM_GUILD_MEMBER_INFO_DELETE01,		//	���� �λ縻 ����
		GUM_GUILD_MEMBER_INFO_DELETE02,
		GUM_GUILD_MEMBER_INFO_DELETE03,
		GUM_GUILD_MEMBER_INFO_DELETE04,
		GUM_GUILD_MEMBER_INFO_DELETE05,
		GUM_GUILD_MEMBER_INFO_DELETE06,
		GUM_GUILD_MEMBER_INFO_DELETE07,
		GUM_GUILD_MEMBER_INFO_DELETE08,

		GUM_GUILD_MEMBER_INFO_DELETE_OK,	//	���� �޽��� ���� Ȯ�ι�ư

		GUM_GUILD_CREATE_OK,
		
		GUM_GUILD_NOTICE_OK,				//	��� �Ϸ� �˾� Ȯ�� ��ư

#ifdef SERV_GUILD_CHANGE_NAME
		GUM_GUILD_NAME_CHANGE_CHECK_OK,			//  ��� �̸� ���� üũ Ȯ�� ��ư
		GUM_GUILD_NAME_CHANGE_CHECK_CANCEL,		//  ��� �̸� ���� üũ ��� ��ư
		GUM_GUILD_NAME_CHANGE_OK,				//  ��� �̸� ���� Ȯ�� ��ư
		GUM_GUILD_NAME_CHANGE_CNACEL,			//  ��� �̸� ���� ��� ��ư
#endif //SERV_GUILD_CHANGE_NAME
	};


	//{{ ����� : [2009/9/19] //	��� ��� ����
	enum	GUILD_USER_GRADE
	{
		GUG_INVALID		= 0,
		GUG_MASTER,				//	������
		GUG_SYSOP,				//	������
		GUG_OLD_USER,			//	���� ����
		GUG_VIP_USER,			//	��� ����
		GUG_NORMAL_USER,		//	�Ϲ� ����
		GUG_NEW_USER,			//	���� ����
	};
	//}} ����� : [2009/9/19] //	��� ��� ����

	//	UI MSG
	enum	GUILD_CHANGE_GRADE_UI_MSG
	{
		GCGUM_MASTER	= 35101,
		GCGUM_SYSOP,
		GCGUM_OLD_USER,
		GCGUM_VIP_USER,
		GCGUM_NORMAL_USER,
		GCGUM_NEW_USER,
	};

	//{{ kimhc // 2009-09-18 // ��� ���� ����ü
	struct GuildInfo
	{
		UidType				m_iGuildUID;
		std::wstring		m_wstrGuildName;
		UINT				m_uiGuildEXP;
		std::wstring		m_wstrGuildMessage;
		std::wstring		m_wstrFoundingDay;			// ��� â����
		USHORT				m_usMaxNumOfGuildMember;
		USHORT				m_usNowNumOfGuildMember;
		std::wstring		m_wstrGuildMasterNickName;
		BYTE				m_byMaxNumOfGuildAdmin;		// ��� ������ �ִ� ��

		BYTE				m_byGuildLevel;				//{{ ����� : [2009/9/28] //	���� �߰�

		GuildInfo() :
		m_iGuildUID( 0 ),
		m_wstrGuildName(),
		m_uiGuildEXP( 0 ),
		m_wstrGuildMessage(),
		m_wstrFoundingDay(),
		m_usMaxNumOfGuildMember( 0 ),
		m_usNowNumOfGuildMember( 0 ),
		m_wstrGuildMasterNickName(),
		m_byMaxNumOfGuildAdmin( 0 ),
		m_byGuildLevel( 0 )
		{}

		GuildInfo( const KGuildInfo& kGuildInfo ) :
		m_iGuildUID( kGuildInfo.m_iGuildUID ),
		m_wstrGuildName( kGuildInfo.m_wstrGuildName.c_str() ),
		m_uiGuildEXP( kGuildInfo.m_iGuildEXP ),
		m_wstrGuildMessage( kGuildInfo.m_wstrGuildMessage.c_str() ),
		m_wstrFoundingDay( kGuildInfo.m_wstrFoundingDay.c_str() ),
		m_usMaxNumOfGuildMember( kGuildInfo.m_usMaxNumMember ),
		m_usNowNumOfGuildMember( 0 ),
		m_wstrGuildMasterNickName(),
		m_byMaxNumOfGuildAdmin( 0 ),//( kGuildInfo.m_usMaxNumMember / _CONST_GUILD_MANAGER_::g_byGuildAdminMagicNumber ) - 1 ),
		m_byGuildLevel( kGuildInfo.m_ucGuildLevel )
		{
		}

		void UpdateMaxNumOfGuildAdmin()
		{
			BYTE byMaxNum = 0;
			m_byMaxNumOfGuildAdmin = m_usNowNumOfGuildMember / _CONST_GUILD_MANAGER_::g_byGuildAdminMagicNumber;

			if ( (m_usNowNumOfGuildMember % _CONST_GUILD_MANAGER_::g_byGuildAdminMagicNumber) != 0 )
				m_byMaxNumOfGuildAdmin++;
		}
	};
	//}} kimhc // 2009-09-18 // ��� ���� ����ü

	//{{ kimhc // 2009-10-31 // ��� ����ġ ����
	struct GuildEXP
	{
		int				m_iNeedEXP;
		int				m_iTotalEXP;

		GuildEXP()
		{
			m_iNeedEXP		= 0;
			m_iTotalEXP		= 0;
		}

		/*GuildEXP( int iNeedExp, int iTotalExp )
		{
			m_iTotalEXP		= iNeedExp;
			m_iTotalEXP		= iTotalExp;
		}*/
	};
	//}} kimhc // 2009-10-31 // ��� ����ġ ����

	//{{ ����� : [2009/10/11] //	���� ���� ����ü(�λ縻 ���濡 ���)
	struct GuildMemberInfo{
		UidType	m_UID;		//	���� UID
		UCHAR	ucGrade;		//	���� ���
		char	m_cUnitClass;	//	���� Ŭ����
		UCHAR	m_ucLevel;		//	���� ����

		GuildMemberInfo() : m_UID(0), ucGrade(GUG_INVALID), m_cUnitClass(0), m_ucLevel(0) {}
	};
	
	//}} ����� : [2009/10/11] //	���� ���� ����ü(�λ縻 ���濡 ���)

public:
	CX2GuildManager();
	~CX2GuildManager();

	HRESULT			OnFrameMove( double fTime, float fElapsedTime );
	bool			UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool			UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool			DidJoinGuild() const { return m_pMyGuildInfo != NULL ? true : false; }		// ��忡 ���� �Ǿ��� �ִ°�?
	std::wstring	GetWStringGuildMemberGrade( BYTE byMemberGrade ) const;
	void			SetUseGuildItemUID( UidType iUID ) { m_iUseGuildItemUID = iUID; }
	bool			IsMyGuildUser( UidType iUid ) const { return m_setUidOfGuildMembers.find( iUid ) != m_setUidOfGuildMembers.end() ? true : false;	} //	�ڱ� �����ΰ�?
	bool			CanDestroyGuild() const;
	bool			CanInviteMember() const;
	void			UpdateGuildTabInCommunityUI() const;
	void			UpdateNowNumOfGuildMember( UidType iUnitUID, bool bIsAdd = true );	// ���� �߰�/����

	GuildInfo*	GetGuildInfo() const { return m_pMyGuildInfo; } 
	GuildInfo*	GetMyGuildInfo()	{	return m_pMyGuildInfo;	}
	wstring		GetMyGuildName()	{	return ( m_pMyGuildInfo == NULL ) ? L"" : m_pMyGuildInfo->m_wstrGuildName; }
	wstring		GetDisplayMyGuildName();
	wstring		ConvertDisplayGuildName( const wstring &wstrGuildName );	//	���� ���̿� []�� �־��ش�
	void		SafeDeleteGuildInfo();
	bool		IsMyUID( UidType iMyUID ) const;
	D3DXCOLOR	GetGradeColor( GUILD_USER_GRADE eGrade ) const;
	UidType		GetSelectedUserUID() {	return m_iSelectedUserUID;	}
	void		SetVecGuildMemberInfo( std::vector< GuildMemberInfo > vecGuildMemberInfo ) {	m_vecGuildMemberInfo = vecGuildMemberInfo;	}
	bool		GetShownGuildMessage() { return m_bIsShownGuildMessage;	}
	void		SetShownGuildMessage( bool bShown ) { m_bIsShownGuildMessage = bShown;	}
	std::vector< GuildMemberInfo >	GetVecGuildMemberInfo() {	return m_vecGuildMemberInfo;	}

	void		SetGuildInfo( const KGuildInfo &kInfo )
	{
		if( m_pMyGuildInfo == NULL )
		{
			m_pMyGuildInfo = new GuildInfo( kInfo );
		}
		else
		{
			m_pMyGuildInfo->m_iGuildUID = kInfo.m_iGuildUID;
			m_pMyGuildInfo->m_wstrGuildName = kInfo.m_wstrGuildName.c_str();
			m_pMyGuildInfo->m_uiGuildEXP = kInfo.m_iGuildEXP;
			m_pMyGuildInfo->m_wstrGuildMessage = kInfo.m_wstrGuildMessage.c_str();
			m_pMyGuildInfo->m_wstrFoundingDay = kInfo.m_wstrFoundingDay.c_str();
			m_pMyGuildInfo->m_usMaxNumOfGuildMember = kInfo.m_usMaxNumMember;
			m_pMyGuildInfo->m_byMaxNumOfGuildAdmin = ( m_pMyGuildInfo->m_usMaxNumOfGuildMember / _CONST_GUILD_MANAGER_::g_byGuildAdminMagicNumber ) - 1;
			m_pMyGuildInfo->m_byGuildLevel = kInfo.m_ucGuildLevel;




		}

		//	���� : Ŭ���� ��� ���� ������ ����ü �����ڿ��� �� �� ���� �ϴ� ������ ������.
		m_pMyGuildInfo->m_usNowNumOfGuildMember	= m_setUidOfGuildMembers.size();
		m_pMyGuildInfo->UpdateMaxNumOfGuildAdmin();
	}
	
	void		SetGuildInfo( const KEGS_GUILD_INFO_NOT &kPacket )
	{
		SetGuildInfo( kPacket.m_kGuildInfo );

		m_pMyGuildInfo->m_wstrGuildMasterNickName = kPacket.m_wstrGuildMasterName;
		m_pMyGuildInfo->UpdateMaxNumOfGuildAdmin();
	}
	
	//	�ڵ鷯
	bool	Handler_EGS_CREATE_GUILD_REQ( const wstring& wszGuildName ) const;
	bool	Handler_EGS_CREATE_GUILD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool	Handler_EGS_GUILD_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_CHANGE_GUILD_MEMBER_GRADE_REQ( UidType iUID, int iGrade );
	bool	Handler_EGS_CHANGE_GUILD_MEMBER_GRADE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_GUILD_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_INVITE_GUILD_REQ( UidType unitUID ) const;	// ��� �ʴ�
	bool	Handler_EGS_INVITE_GUILD_REQ( const wstring& wstrNickName ) const;
	bool	Handler_EGS_INVITE_GUILD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) const;

	bool	Handler_EGS_INVITE_GUILD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool	Handler_EGS_INVITE_GUILD_REPLY_REQ( int iGuildUID, int iReasonForDeny );
	bool	Handler_EGS_INVITE_GUILD_REPLY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) const;
	bool	Handler_EGS_INVITE_GUILD_REPLY_DENY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_KICK_GUILD_MEMBER_REQ( UidType iUnitUID ) const;
	bool	Handler_EGS_KICK_GUILD_MEMBER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_CHANGE_GUILD_MESSAGE_REQ( wstring &wstrMessage ) const;
	bool	Handler_EGS_CHANGE_GUILD_MESSAGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_DISBAND_GUILD_REQ() const;
	bool	Handler_EGS_DISBAND_GUILD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_GET_GUILD_USER_LIST_REQ();
	bool	Handler_EGS_GET_GUILD_USER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_CHANGE_GUILD_MEMBER_MESSAGE_REQ( wstring &wstrMessage, UidType iUid = 0 ) const;
	bool	Handler_EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool	Handler_EGS_JOIN_GUILD_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

//{{ oasis907 : ����� [2009.12.17] //
	bool	Handler_RESERVED_EGS_INVITE_GUILD_NOT( KEGS_INVITE_GUILD_NOT kEvent);
//}} oasis907 : ����� [2009.12.17] //

#ifdef SERV_GUILD_CHANGE_NAME
	bool Handler_EGS_CHANGE_GUILD_NAME_CHECK_REQ( wstring wstrGuildName );
	bool Handler_EGS_CHANGE_GUILD_NAME_CHECK_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_CHANGE_GUILD_NAME_REQ( int iOk ) ;
	bool Handler_EGS_CHANGE_GUILD_NAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_GUILD_CHANGE_NAME

public:
	CX2UIGuild *GetUIGuild() const { return m_pUIGuild; }
	USHORT		GetNowNumOfGuildMember() const { return m_pMyGuildInfo != NULL ? m_pMyGuildInfo->m_usNowNumOfGuildMember : 0; }
	USHORT		GetMaxNumOfGuildMember() const { return m_pMyGuildInfo != NULL ? m_pMyGuildInfo->m_usMaxNumOfGuildMember : 0; }
	bool		GetShowUIGuildInfo() const;

	const GuildEXP&	GetGuildExpData( BYTE byNowGuildLevel ) const;	
	BYTE			GetLimitGuildLevel() const { return m_byLimitGuildLevel; }
#ifdef GUILD_SKILL
	UCHAR			GetLimitGuildGrade() const { return m_ucLimitGuildGrade; } // oasis907 : ����� [2009.11.18] //
#endif GUILD_SKILL
	bool			OpenScriptFile( const wstring wstrFileName );
	void			SetLimitGuildLevel_LUA( BYTE byLimitGuildLevel );
	void			AddGuildEXP_LUA( BYTE byGuildLevel, int iNeedEXP, int iTotalEXP );
	UINT			GetNowGuildExp( BYTE byNowGuildLevel ) const;
	UINT			GetNextLevelNeedExp( BYTE byNowGuildLevel ) const;
	UINT			GetTotalGuildExp( BYTE byNowGuildLevel ) const;
	int				GetNowMyGuildHonorPoint( BYTE byNowGuildLevel ) const;

	void			EmptyFuncUsedParsing_LUA() {};


	


private:
	GuildInfo*								m_pMyGuildInfo;
	std::set< UidType >						m_setUidOfGuildMembers;		// �������� UID
	
	std::map< CKTDGUIDialogType,int >		m_mapGuildInvitationDlg;	// vec< pair< Dialog, ���UID > > // ������ �ʴ� ����...
	std::set< int >							m_setGuildUIDForInvitation;	// ������ �ʴ� ���� ����� UID


	UidType									m_iUseGuildItemUID;			//	����� �������� UID(â���ֹ���, ���� Ȯ�� ������ � �̿�)
	CKTDGUIDialogType						m_pDlgChangeGuildMessage;	//	���� ���� ���� Ȯ�ο� Dialog��ü

	CKTDGUIDialogType						m_pDlgNewMaster;			//	��� ������ �����ٶ� Ȯ�� Dlg
	CKTDGUIDialogType						m_pDlgNewMasterRe;			//	��� ������ �����ٶ� ��Ȯ�� Dlg

	wstring									m_wstrSelectedUserName;	//	���õ� �ɸ����̸�
	UidType									m_iSelectedUserUID;		//	���õ� �ɸ���UID

	std::vector< GuildMemberInfo >			m_vecGuildMemberInfo;	//	���� ���� ����ü

	wstring									m_wstrCreateGuildName;	//	���� ��� �̸�
	wstring									m_wstrSenderNickName;	// ������ ��� �ʴ븦 �� ������ �г���

	bool									m_bIsShownGuildMessage;	//	��� �޽����� ��������� ����

	std::map< BYTE, GuildEXP >				m_mapGuildExpTable;		// ��� ����ġ ���̺�
	BYTE									m_byLimitGuildLevel;	// ��� ���� ����
#ifdef GUILD_SKILL
	UCHAR									m_ucLimitGuildGrade; // oasis907 : ����� [2009.11.18] // ��� ��ų�� ������ ���� ���ϴ� ���� ���
#endif GUILD_SKILL

	CKTDGUIDialogType						m_DlgKickByMaster;		// ���� ���ߴٴ� �˾�
public:
	CX2UIGuild	*m_pUIGuild;
	
};

#endif	//	GUILD_MANAGEMENT
//}} ����� : [2009/9/15] //	���Ŵ��� Ŭ���� ����
