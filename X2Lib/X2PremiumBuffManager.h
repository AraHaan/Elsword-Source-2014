/**@file	X2PremiumBuff.h
   @breif	�޸� ���� ���� ���� �� ��Ÿ Ư�� ���� ���� 
*/

#pragma once

#ifdef COME_BACK_REWARD

#ifdef BUFF_TEMPLET_SYSTEM

class CX2PremiumBuffTemplet;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2PremiumBuffTemplet> CX2PremiumBuffTempletPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef shared_ptr<CX2PremiumBuffTemplet> CX2PremiumBuffTempletPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

class CX2PremiumBuffTemplet
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
{
public:

	enum BUFF_ID
	{
		BI_NONE							= 0,
		BI_GATHERING_OF_HERO,			// ������ ���� ( �ڵ���Ƽ���� )
		BI_REST_FOR_REGROUP,			// �������� ���� �޽�
		BI_WARM_EL_AURA,				// ������ ���� ��� (���� ����)
		BI_PROTECTION_OF_NUT,			// ��Ʈ�� ��ȣ     ( ���, �ӽ� BuffID )
		BI_PROTECTION_OF_PUTA,			// ��Ÿ�� ��ȣ     ( ���, �ӽ� BuffID )
		BI_PROTECTION_OF_LAHELL,		// ������ ��ȣ     ( ���, �ӽ� BuffID )
		BI_PROTECTION_OF_JAKIELL,		// ��Ű���� ��ȣ   ( ���, �ӽ� BuffID )
		BI_BELSSING_OF_SERAPHIM,		// �������� �ູ   ( ����, �ӽ� BuffID )
		BI_BELSSING_OF_GEV,				// �Ժ��� �ູ	  ( ����, �ӽ� BuffID )
		BI_BELSSING_OF_AMON,			// �Ƹ��� �ູ	  ( ����, �ӽ� BuffID )
		BI_BELSSING_OF_CRONOS,			// ũ�γ뽺�� �ູ ( ����, �ӽ� BuffID )
		BI_SAVE_CRYSTAL_BUFF_LEVEL_1,	// ����� �� ���� ���� ���� 4
		BI_SAVE_CRYSTAL_BUFF_LEVEL_2,	// ����� �� ���� ���� ���� 4
		BI_SAVE_CRYSTAL_BUFF_LEVEL_3,	// ����� �� ���� ���� ���� 4
		BI_SAVE_CRYSTAL_BUFF_LEVEL_4,	// ����� �� ���� ���� ���� 4
		BI_VITALITY_OF_EL,				// ����� �� ���� ���� ����
		BI_STRENGTHENING_BODY,			// ��ȭ�� ��ü ( ��� ��� ���� ����� )
	};

	//���� ���� ������ �� Ÿ��
	enum USING_ROOM_TYPE
	{
		URT_NONE						= -1,
		URT_PVP,						
		URT_DUNGEON,
		URT_SQUARE,
		URT_TRADE,
		URT_TRAININGCENTER,
		URT_PERSONAL_SHOP,
		URT_ARCADE,
		URT_PSHOP_AGENCY,
	};

	~CX2PremiumBuffTemplet()
	{   
		m_vUsingRoomType.clear();
		m_vecSocketOptions.clear();
	}

	static CX2PremiumBuffTempletPtr CreatePremiumBuffTemplet() 
	{
		return CX2PremiumBuffTempletPtr( new CX2PremiumBuffTemplet );
	}

	CX2PremiumBuffTemplet::BUFF_ID GetPremiumBuffID() const { return m_ePremiumBuffID; }
	void SetPremiumBuffID( const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_ ) { m_ePremiumBuffID = ePremiumBuffID_; }

	UINT GetStringID() const { return m_uiStringID; }
	void SetStringID( const UINT uiStringID_ ) { m_uiStringID = uiStringID_; }

	float GetBuffPeriodSecond() const { return m_fPremiumBuffPeriodSecond; };
	void  SetBuffPeriodSecond( const float m_fPremiumBuffPeriodSecond_ ) { m_fPremiumBuffPeriodSecond = m_fPremiumBuffPeriodSecond_; };

	void PushUsingRoomTypeList( const CX2PremiumBuffTemplet::USING_ROOM_TYPE m_vecSocketOptions_ ) { m_vecSocketOptions.push_back( m_vecSocketOptions_ ); }
	const vector<CX2PremiumBuffTemplet::USING_ROOM_TYPE>& GetUsingRoomTypeList() { return m_vUsingRoomType; }

	void PushBackSocketOption( const int iSocketOption_ ) { m_vecSocketOptions.push_back( iSocketOption_ ); }
	const vector<int>& GetSocketOptionsList() { return m_vecSocketOptions; }

	bool ParsingScript( IN KLuaManager& luaManager_ );

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

private:
	CX2PremiumBuffTemplet()
		: m_ePremiumBuffID( BI_NONE ), 
		  m_uiStringID( 0 ),
		  m_fPremiumBuffPeriodSecond( 0 )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
          , m_uRefCount( 0 )
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{}

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	CX2PremiumBuffTemplet::BUFF_ID					m_ePremiumBuffID;
	UINT											m_uiStringID;
	float											m_fPremiumBuffPeriodSecond;
	vector<CX2PremiumBuffTemplet::USING_ROOM_TYPE>	m_vUsingRoomType;
	vector<int>										m_vecSocketOptions;	
};

IMPLEMENT_INTRUSIVE_PTR( CX2PremiumBuffTemplet );


class CX2PremiumBuffIcon;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2PremiumBuffIcon> CX2PremiumBuffIconPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2PremiumBuffIcon> CX2PremiumBuffIconPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

class CX2PremiumBuffIcon
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
{
public:

	static CX2PremiumBuffIconPtr CreatePremiumBuff( const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_, UINT uiIndex_,
		const WCHAR* pFileName_, const WCHAR* pKeyName_, wstring wstrEndTime, bool m_bUsingIcon );

	UINT	GetIndex() const { return m_uiIndex; }
	void	SetIndex( const UINT uiIndex_ ) { m_uiIndex = uiIndex_; }
	void	ChangeBuffIconPosition( const UINT uiNewIndex_ );
	wstring GetBuffEndTime() const { return m_wstrEndTime; }
	bool	GetUsingIcon() const { return m_bUsingIcon; }

	CX2PremiumBuffTemplet::BUFF_ID GetPremiumBuffID() const { return m_ePremiumBuffID; }
	CKTDGUIDialogType& GetBuffIconDialogType() { return m_pDlgBuffIcon; }

	~CX2PremiumBuffIcon();

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

protected:
	CX2PremiumBuffIcon( const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_, UINT uiIndex_, wstring wstrEndTime, bool bUsingIcon ) 
		: m_ePremiumBuffID( ePremiumBuffID_ ), m_pDlgBuffIcon( NULL ), m_uiIndex( uiIndex_ ), m_wstrEndTime( wstrEndTime ), m_bUsingIcon( bUsingIcon )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{}
	void CreateBuffIcon( const WCHAR* pFileName_, const WCHAR* pKeyName_ );
	void UpdateBuffIconPosition( const UINT uiIndexGap_ );
	void SetBuffDesc();

private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	CX2PremiumBuffTemplet::BUFF_ID	m_ePremiumBuffID;
	CKTDGUIDialogType				m_pDlgBuffIcon;
	UINT							m_uiIndex;
	wstring							m_wstrEndTime;	

	bool							m_bUsingIcon;

	const static float				POSITION_X;
	const static float				POSITION_Y;
	const static UINT				NUMBER_OF_ROW	= 5;
	const static UINT				SIZE_OF_ICON	= 31;
};

IMPLEMENT_INTRUSIVE_PTR( CX2PremiumBuffIcon );

#endif BUFF_TEMPLET_SYSTEM


/** @class : CX2PremuimBuff
	@brief : ������ ���� �������� ���� ���� �� ȭ�鿡 UI ǥ��
	@date  : 2011/05/31
*/
class CX2PremiumBuffManager
{
public:
#pragma region enumulation
	enum STATE_COMBE_BACK_UI_CUSTOM_MSG
	{
		SCBUCM_WELCOME_OK = 50000,
		SCBUCM_COME_BACK_BUFF_DESC_OK,
	};
#pragma endregion enumulation

#pragma region public function
	CX2PremiumBuffManager();
	~CX2PremiumBuffManager();

	//{{ �޸� ���� Ȯ��
	bool IsComeBackUser() const;
	void SetComeBackUser( IN bool bComeBackUser_ );
	INT64 GetComeBackBuffEnd() const;
	//}}

	//{{ �޸� ���� ó�� ����
	bool IsDescComplete() const;
	bool IsFirstComeBack() const;
	bool IsFirstComeBackBuffDesc() const;
	void SetFirstComeBack( IN bool bFirstComeBack_ );
	void SetShowFirstComBackUI( IN bool bShow_ );
	//}}

	//{{ PC�� ���� ��
	bool IsPremiumPC() const;
	void SetPremiumPC( IN bool bFirstComeBack_ );
	//}}

#ifdef SERV_PC_BANG_TYPE
	bool IsPcBangAvatar();
	void SetPcBangReward( KPcBangReward kPcBangReward );
	wstring GetPcBangBuffString();
#endif SERV_PC_BANG_TYPE

	//{{ kimhc // 2011-08-08 // ���� ���� ä�θ� �̺�Ʈ
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
	bool IsTooniLandChannelingEvent() const { return m_bTooniLandChannelingEvent; }
	void SetTooniLandChannelingEvent( bool bTooniLandChannelingEvent_ ) { m_bTooniLandChannelingEvent = bTooniLandChannelingEvent_; }
#endif	SERV_TOONILAND_CHANNELING_EVENT
	//}} kimhc // 2011-08-08 // ���� ���� ä�θ� �̺�Ʈ

	//{{ �޸� ���� ���� UI ����
	bool GetShowPremiumBuffUI() const;
	bool SetShowPremiumBuffUI( IN bool bShow_ );
	void UpdateGuideDescPremiumBuffUI();
	//}}

	//{{ �޸� ���� ���� ��� ����
	bool GetViewBuffChat() const;
	void SetViewBuffChat( IN bool bShow_ );
	//}}

	//{{ ��Ƽ ���� �޸� ���� ����
	bool IsComeBackUserInParty() const;
	void SetComeBackUserInParty( IN bool bShow_ );
	//}}

	//{{ �̺�Ʈ ó��
	bool UICustomEventProc( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );
	//}}

	//{{ ������ ���
#ifdef BUFF_TEMPLET_SYSTEM
	bool Handler_EGS_UPDATE_BUFF_INFO_IN_ROOM_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );
	bool Handler_EGS_UPDATE_BUFF_INFO_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );
#endif BUFF_TEMPLET_SYSTEM
	bool Handler_EGS_COME_BACK_BUFF_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );
	bool Handler_EGS_COME_BACK_USER_NOT( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );
	bool UIServerEventProc( IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM lParam );
	//}}

	//{{ ������ �� ó��
	HRESULT OnFrameMove( IN double fTime, IN float fElapsedTime );
	//}

#ifdef GRACE_OF_EL
	bool IsGraceOfEl() const;
	void SetGraceOfEl( IN bool bGraceOfEl_ );
#endif
#ifdef BUFF_TEMPLET_SYSTEM
	void InsertPremiumBuff( const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_, wstring wstrEndTime );
	void ErasePremiumBuff( const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_ );
	bool GetFileAndKeyName( IN const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_, OUT const WCHAR** ppFileName_, OUT const WCHAR** ppKeyName_, OUT bool* bUsingIcon_ );
	void UpdatePremiumBuffIconPosition();

	void OpenScriptFile( const wchar_t* wszScriptFileName_ );
	void AddBuffInfo_LUA();

	CX2PremiumBuffTempletPtr GetPremiumBuffTempletPtr( const CX2PremiumBuffTemplet::BUFF_ID ePremiumBuffID_ );	

	void GetSocketOptions( OUT vector<int>& vecBuffSocetOptions_ );
	void GetSocketOptions( IN const vector<int>& vecBuffIdList_, OUT vector<int>& vecBuffSocetOptions_ );
	
	vector<CX2PremiumBuffIconPtr> GetPremiumBuffList() { return m_vecPremiumBuffList; }

	bool GetShowBuffTempletUI() { return m_bBuffTempletUI; }
	void SetShowBuffTempletUI( bool bShow ) { m_bBuffTempletUI = bShow; }

#endif BUFF_TEMPLET_SYSTEM
#pragma endregion public function

private:
#pragma region private function
	void Clear();
#ifdef SERV_PC_BANG_TYPE
	wstring _GetLastString( wstring wstr )
	{
		unsigned int uiFound = wstr.find_last_of( L'\n' );
		unsigned int uiLastLineSize = wstr.size() - uiFound;

		if( uiLastLineSize > 15 )
			return L"\n";
		else
			return L", ";
	}
#endif SERV_PC_BANG_TYPE

#pragma endregion private function

private:
#pragma region private member
	bool m_bComeBackUser;			/// �޸� ���� ���� ����(���� ������ �� �ʵ� Ÿ��Ʋ)
	bool m_bChatViewFirst;			/// ä�� â�� ǥ�õǴ� ���� ����
	bool m_bDescComplete;			/// ù ���� �� ���� ���� UI Ȯ��(â �ݱ�)
	bool m_bFirstComeBack;			/// ù ���� ����(ù �˸� UI ����)
	bool m_bFirstComeBackBuffDesc;	/// ù ���� �� ���� ���� UI
	bool m_bPremiumPC;				/// PC�� ���� ����
	bool m_bPremiumBuffUI;			/// �޸�, PC�� �� ���� ������ ǥ�� ����
	bool m_bComeBackuserInParty;	/// ��Ƽ�� �޸� ���� ���� ���ԵǾ����� ����
		
	//{{ kimhc // 2011-08-08 // ���� ���� ä�θ� �̺�Ʈ
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
	bool m_bTooniLandChannelingEvent;	/// ���Ϸ��� ä�θ� �̺�Ʈ ���� �ű� ������ ������ ����
#endif	SERV_TOONILAND_CHANNELING_EVENT
	//}} kimhc // 2011-08-08 // ���� ���� ä�θ� �̺�Ʈ

#ifdef GRACE_OF_EL
	bool m_bGraceOfEl;			/// ���������� Ư���뿡�� ����Ǵ� "���� ��ȣ" ���� ����
#endif

	CKTDGUIDialogType	m_pDLGWelcomeComeBackUser;		/// �޸� ���� ���� ȯ�� �޽��� ���̾�α�
	CKTDGUIDialogType	m_pDLGComeBackUserBuffDesc;		/// �޸� ���� ���� ���� ���� ���̾�α�

	INT64	m_iComeBackBuffEnd;		/// �޸� ���� ���� �ð�

#ifdef BUFF_TEMPLET_SYSTEM
	vector<CX2PremiumBuffIconPtr>		m_vecPremiumBuffList;

	typedef	map<CX2PremiumBuffTemplet::BUFF_ID, CX2PremiumBuffTempletPtr> PremiumBuffTempletMap;
	PremiumBuffTempletMap				m_mapPremiumBuffTempletMap;
	bool								m_bBuffTempletUI;/// ���� ���ø� ǥ�� ����
#endif BUFF_TEMPLET_SYSTEM

#ifdef SERV_PC_BANG_TYPE
	KPcBangReward						m_kPcBangReward;
#endif SERV_PC_BANG_TYPE

#pragma endregion private member
};

#endif
