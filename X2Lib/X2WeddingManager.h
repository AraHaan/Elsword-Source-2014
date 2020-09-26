#pragma once
#ifdef ADDED_RELATIONSHIP_SYSTEM

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CX2OfficiantManger
// �ַ� ���õ� ���� ����.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CX2OfficiantManger
{
public:

	/// �ַ� �ൿ ���
	struct OfficiantBehavior
	{
		float									m_fTalkDelayTime;	// ���� ��� ���� �� ���� �ð�
		int										m_iTalkStrID;
		wstring									m_wstrMotionName;
		CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE		m_eAnimPlayType;

		OfficiantBehavior():
		m_fTalkDelayTime(0.f),
		m_iTalkStrID(-1),
		m_wstrMotionName(L""),
		m_eAnimPlayType(CKTDGXSkinAnim::XAP_LOOP)
		{}
	};

	/// �ַ� ���ø�
	struct OfficiantTemplet
	{
		wstring							m_wstrMeshTempletName;	// �޽����ø� �̸�
		vector<OfficiantBehavior*>		m_vecOfficiantBehavior;	// �ַ� �ൿ ���
		D3DXVECTOR3						m_vTalkBoxOffsetPos;	// ��ǳ�� ��ġ offset
		SEnum::WEDDING_OFFICIANT_TYPE	m_eOfficantType;		// �ַʻ� Ÿ��
		float							m_fCameraDurationTime;	// ī�޶� ���� �ð�

		OfficiantTemplet():
		m_vTalkBoxOffsetPos(0,0,0),
		m_eOfficantType( SEnum::WOT_NPC_END ),
		m_fCameraDurationTime(0.f)
		{}

		OfficiantTemplet& operator=( const OfficiantTemplet& rhs )
		{
			m_wstrMeshTempletName	= rhs.m_wstrMeshTempletName;
			m_vTalkBoxOffsetPos		= rhs.m_vTalkBoxOffsetPos;
			m_eOfficantType			= rhs.m_eOfficantType;
			m_fCameraDurationTime	= rhs.m_fCameraDurationTime;

			BOOST_FOREACH( OfficiantBehavior* pOfficiantBehavior, rhs.m_vecOfficiantBehavior )
			{
				OfficiantBehavior* pTempOfficiantBehavior = new OfficiantBehavior;
				*pTempOfficiantBehavior = *pOfficiantBehavior;

				m_vecOfficiantBehavior.push_back( pTempOfficiantBehavior );
			}
			
			return *this;
		}
	};

public:
	CX2OfficiantManger();
	virtual ~CX2OfficiantManger(void);
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	bool AddOfficiantTemplet_LUA();

	bool CreateOfficiant();
	void StartOfficiantsMessage();

	void SetOfficiant( SEnum::WEDDING_OFFICIANT_TYPE eOfficiantType_ ) { m_eCurrentOfficiantType = eOfficiantType_;}
	void SetOfficiantPos( const D3DXVECTOR3& vPos_, const D3DXVECTOR3& vCameraEye_, const D3DXVECTOR3& vCameraLookAt_);
	
private:

	void ChangeOfficiantMotion( const WCHAR* pMotionName_, CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE eAnimPlayType_ );
	void PlayOfficiantsMessage( int iStrID_ );
	void PlayOfficiantCamera( const D3DXVECTOR3& vEye, const D3DXVECTOR3& vLookAt, float fTrackingTime );

private:
	std::map<SEnum::WEDDING_OFFICIANT_TYPE, OfficiantTemplet*> m_mapOfficiantTemplet;		// ��ü �ַ� ����.

	SEnum::WEDDING_OFFICIANT_TYPE			m_eCurrentOfficiantType;	// ���� �ַ� Ÿ��.	
	OfficiantTemplet*						m_pCurrentOfficiant;		// ���� �ַ� ��ü
	CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hOfficiantMesh;			// �ַ� �޽� �ڵ�

	bool									m_bIsPlayOfficiantsMessage; // �ַʻ� ���� ����
	float									m_fOfficiantTime;			// �ַʻ� �ð�

	
	D3DXVECTOR3								m_vPos;			// �ַ� ��ġ
	D3DXVECTOR3								m_vCameraEye;	// �ַ� �����ϴ� ���� �̵� �� ī�޶� ��ġ	
	D3DXVECTOR3								m_vCameraLootAt;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CX2WeddingHallManager
// �����忡 ���õ� ���� ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CX2WeddingHallManager
{	
public:
	struct WeddingHallTemplet
	{
		// ��ȥ�� ������ ���� ��ġ
		int							m_iBrideStartPos;
		int							m_iGroomStartPos;
		vector<int>					m_vecWeddingGuestStartPos;

		// �Ŷ�,�ź��� �ַʻ� ���� ���� �̵� �� ��ġ
		int							m_iBrideOfficiantStartPos;
		int							m_iGroomOfficiantStartPos;

		// �ַ� ��ġ
		D3DXVECTOR3					m_vOffciantPos;
		D3DXVECTOR3					m_vOffciantCameraEye;		// �ַ� ���� ī�޶�
		D3DXVECTOR3					m_vOffciantCmaeraLookAt;	

		D3DXCOLOR					m_vColor;					//���� ����
		D3DXCOLOR					m_vOutlineColor; 

		CX2World::WORLD_ID			m_eWorldID;
		SEnum::WEDDING_HALL_TYPE	m_eWeddingHallType;

		WeddingHallTemplet():
		m_iBrideStartPos(0),
		m_iGroomStartPos(0),
		m_iBrideOfficiantStartPos(0),
		m_iGroomOfficiantStartPos(0),
		m_vOffciantPos(0,0,0),
		m_vOffciantCameraEye(0,0,0),
		m_vOffciantCmaeraLookAt(0,0,0),
		m_eWorldID(CX2World::WI_NONE),
		m_eWeddingHallType(SEnum::WHT_CASH_A),
		m_vColor(1,1,1,1),
		m_vOutlineColor(0,0,0,1)
		{}
	};

public:
	CX2WeddingHallManager();
	virtual ~CX2WeddingHallManager();
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

	bool AddWeddingHallTemplet_LUA();
	void SetWeddingHall( SEnum::WEDDING_HALL_TYPE eHallType_ );

	// ���� ����Ȧ�� ���� ��� �Լ�
	CX2World::WORLD_ID GetCurrentWorldID() const;
	void GetOfficiantPos( OUT D3DXVECTOR3& vPos_ ) const;
	void GetOfficiantsMessageEye( OUT D3DXVECTOR3& vEye_ ) const;
	void GetOfficiantsMessageLookAt( OUT D3DXVECTOR3& vLookAt_ ) const;
	UINT GetStartPosIndexByWeddingStatus( SEnum::WEDDING_STATUS eWeddingStatus_ ) const;
	UINT GetOfficiantStartPosIndexByWeddingStatus( SEnum::WEDDING_STATUS eWeddingStatus_ ) const;

	void GetNoticeColor( OUT D3DXCOLOR& vColor_){ if( NULL != m_pCurrentWeddingHallTemplet) vColor_ = m_pCurrentWeddingHallTemplet->m_vColor; }
	void GetNoticeOutlineColor( OUT D3DXCOLOR& vOutlineColor_){ if( NULL != m_pCurrentWeddingHallTemplet) vOutlineColor_ = m_pCurrentWeddingHallTemplet->m_vOutlineColor; }

	void GetQuestStartPosList( OUT vector<int>& vecStartPos_ );
private:
	std::map<SEnum::WEDDING_HALL_TYPE, WeddingHallTemplet*> m_mapWeddingHallTemplet;
	SEnum::WEDDING_HALL_TYPE								m_eCurrentWeddingHallType;

	WeddingHallTemplet*										m_pCurrentWeddingHallTemplet;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CX2WeddingBehavior
// ��ȥ�� ���࿡ �ʿ��� �ൿ ���� Ŭ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CX2WeddingBehavior
{
public:
	// ����Ʈ ��� ��ġ Ÿ��
	enum WEDDING_EFFECTSET_POS_TYPE
	{
		WEPT_CUSTOM,	// ����� ���� ��ġ
		WEPT_GROOM,		// �Ŷ� ��ġ
		WEPT_BRIDE,		// �ź� ��ġ
	};
	// ���� ��ġ �̵� Ÿ��
	enum WEDDING_POS_MOVE_TYPE
	{
		WPMT_WEDDING_START,
		WPMT_OFFICIANT_START,
	};

	// ����
	struct Behavior_Notice
	{
		float m_fStartTime;
		float m_fDurationTime;
		int   m_iStrID;
		bool  m_bEndNotice;
	};

	// ��ġ �̵�
	struct Behavior_Move
	{
		float m_fStartTime;
		WEDDING_POS_MOVE_TYPE eWeddingPosMove;
	};

	// ���� ���
	struct Behavior_Sound
	{
		float m_fStartTime;
		wstring m_wstrSoundFileName;
		float m_fDurationTime;
	};

	// ���ӽð����� ��� �� ���ŵǾ�� �ϴ� ���� ����
	struct Behavior_DurationTimeSound
	{
		// �������� �Ҵ����� ������ KTDXDeviceManagerŬ�������� ó���ϰ� ����.
		CKTDXDeviceSound* m_pSound;
		float			  m_fTime;
		float			  m_fDurationTime;
		
		Behavior_DurationTimeSound():
		m_pSound(NULL),
		m_fTime(0.f),
		m_fDurationTime(0.f)
		{}
	};

	struct Behavior_Effectset
	{
		float m_fStartTime;
		wstring m_wstrEffectSetName;
		D3DXVECTOR3 m_vPos;
		WEDDING_EFFECTSET_POS_TYPE m_ePosType;
	};

public:
	CX2WeddingBehavior();
	~CX2WeddingBehavior();
	void OpenScript( KLuaManager& luaManager_ );

	vector<Behavior_Notice>		m_vecBehavior_Notice;
	vector<Behavior_Move>		m_vecBehavior_Move;
	vector<Behavior_Sound>		m_vecBehavior_Sound;
	vector<Behavior_Effectset>	m_vecBehavior_Effectset;
	
	vector<Behavior_DurationTimeSound> m_vecBehavior_DurationTimeSound; // ���ӽð����� ��� �� ���ŵǾ�� �ϴ� ���� ����
private:
	void CheckTimeError( float fBehaviorTime_, float fPreBehabiorTime_ );
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CX2WeddingManager
// �ַ�, �������� �����ϴ� Ŭ������ �Բ� ��ȥ���� �������� ������ ����.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CX2WeddingManager
{	
public:
	struct NoticeInfo
	{
		wstring wstrNotice;
		float	m_fElapseTime; // ������ �ð�
		float   m_fMaxShowTime;   // ������� �� �ð�

		NoticeInfo():
		wstrNotice(L""),
		m_fElapseTime(0.f),
		m_fMaxShowTime(0.f)
		{}
	};
public:
	CX2WeddingManager();
	virtual ~CX2WeddingManager(void);
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	HRESULT	OnFrameRender();

	/// ���� �Ŵ���Ŭ������ �����ϱ� ���� �����Լ�
	bool OpenScriptFile( const WCHAR* pOfficiantFileName_,  
						const WCHAR* pWeddingHallFileName_,
						const WCHAR* pWeddingManagerFileName_);

	bool AddOfficiantTemplet_LUA();
	bool AddWeddingHallTemplet_LUA();

	void SetOfficiant( SEnum::WEDDING_OFFICIANT_TYPE eOfficiantType_ );
	void SetWeddingHallAndOfficiantInfo( SEnum::WEDDING_HALL_TYPE eHallType_ );

	// ���� ��ü ���� �� �� �ʿ��� ���� ��ġ ���� �Լ�
	CX2World::WORLD_ID	GetCurrentWorldID() const { return m_eCurrentWorldID; }
	UINT				GetStartPosIndexByWeddingStatus( IN SEnum::WEDDING_STATUS eWeddingStatus_ ) const;
	
	/// ���� ����
	void				StartWedding();
private:

	void OnFrameMove_Notice();
	void OnFrameMove_Move();
	void OnFrameMove_Sound();
	void OnFrameMove_Effectset();
	void OnFrameMove_SoundStop( float fElapsedTime_ );

private:	
	bool					m_bIsPlayWedding;
	float					m_fWeddingTime;

	CX2OfficiantManger*		m_pOfficiantManager;
	CX2WeddingHallManager*	m_pWeddingHallManager;
	CX2WeddingBehavior		m_WeddingBehavior;

	CX2World::WORLD_ID		m_eCurrentWorldID;


	NoticeInfo				m_NoticeInfo;
	

};//class CX2WeddingManager


#endif //ADDED_RELATIONSHIP_SYSTEM