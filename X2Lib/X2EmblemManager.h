#pragma once

/* ������ // 2013-05-20
-������ ����, ����� �� ���� ���� �� �� 
 ȭ�� ��� ��Ÿ���� ������ ���� �ϴ� �Ŵ��� Ŭ�����Դϴ�.

-���� �ڵ�� ������ ���� �� �� ���, 
 ���� �ð��뿡 ��� ���� ��, ������ ��ġ�� ������ �־�����
 ��� ������ �ذ� �ϱ� ���� �ۼ��� Ŭ�����Դϴ�.
*/
#ifdef NEW_EMBLEM_MANAGER
class CX2EmblemManager
{
public:
	enum EMBLEM_ID
	{
		EI_NONE	= -1,
		EI_FESTIVAL = 0,	// ������ ����
		EI_DEFENCE_START,	// � ħ�� ����
		EI_DEFENCE_SUCCESS,	// ����
		EI_DEFENEC_FAIL,	// ����
		EI_EVENT_CRAYONPOP,	// ũ������ �̺�Ʈ
		EI_EVENT_ELESIS,	// �����ý� �̺�Ʈ
		EI_WEEKEND_BURNING_EVENT, // �ָ� ���� �̺�Ʈ
		EI_OPEN_FIELD_BOSS_RAID, // ���� ���̵� ����
		EI_CLOSE_FIELD_BOSS_RAID,// ���� ���̵� Ŭ����
// #ifdef WORLD_BUFF_2013_CHRISTMAS 	// ������, 2013�� ũ���������� ���� ����
		EI_EVENT_2013_CHRISTMAS,
// #endif // WORLD_BUFF_2013_CHRISTMAS	// ������, 2013�� ũ���������� ���� ����

// #ifdef ADD_2013_CHARACTER_ADD_EVENT		// 2013 �ֵ� �߰� ��� ���� �̺�Ʈ
		EI_EVENT_BURNING_NEW_CHARACTER_TRIPLE,
		EI_EVENT_BURNING_NEW_CHARACTER_DOUBLE,
// #endif // ADD_2013_CHARACTER_ADD_EVENT	// 2013 �ֵ� �߰� ��� ���� �̺�Ʈ

// #ifdef FIX_REFORM_ENTRY_POINT_7TH		// ������, ��ȯ�� ���� ��� �κ� Emblem Manager �� ����
		EI_RETURN_OF_HERO,					
// #endif FIX_REFORM_ENTRY_POINT_7TH		// ������, ��ȯ�� ���� ��� �κ� Emblem Manager �� ����

		EI_NAVER_EVENT,						// ���̹� ä�θ� �̺�Ʈ
	};
#ifdef ADD_PLAY_MUSIC_WHEN_EMBLEM_POP 		// ������, ���� ���� ��, ���� ��� ��� �߰�

	enum EMBLEM_POPUP_SOUND_TYPE 
	{
		EPST_NONE					= 0,		// �Ϲ� ���
		EPST_RELATIVE_UNIT_TYPE_RANDOM	= 1,	// Ŭ������ �°� ���
	};
	
	struct EmblemPopupSound 
	{
		
		wstring					m_wstrSoundFileName;
		float					m_fStartTime;
		EMBLEM_POPUP_SOUND_TYPE m_eEmblemPopupSoundType;
		CX2Unit::UNIT_TYPE		m_eUnitType;
		bool					m_bIsPlaying;
		EmblemPopupSound () :
			m_wstrSoundFileName ( L"" ),
			m_fStartTime ( -1.f ),
			m_eEmblemPopupSoundType ( EPST_NONE ),
			m_eUnitType ( CX2Unit::UT_NONE ),
			m_bIsPlaying ( false )
		{ }

		EmblemPopupSound ( wstring wstrSoundFileName, float fStartTime, EMBLEM_POPUP_SOUND_TYPE eEmblemPopupSoundType, CX2Unit::UNIT_TYPE eUnitType ) :
			m_wstrSoundFileName ( wstrSoundFileName ),
			m_fStartTime ( fStartTime ),
			m_eEmblemPopupSoundType ( eEmblemPopupSoundType ),
			m_eUnitType ( eUnitType ),
			m_bIsPlaying ( false )
		{ }

	};


#endif // ADD_PLAY_MUSIC_WHEN_EMBLEM_POP 	// ������, ���� ���� ��, ���� ��� ��� �߰�


#ifdef ADD_2013_CHARACTER_ADD_EVENT 	// ������, 2013 �ֵ� �߰� ��� ���� �̺�Ʈ

	struct OpenTimeSetting  
	{
		int					m_iYear;
		int					m_iMonth;
		int					m_iDay;
		int					m_iHour;
		int					m_iMinute;
		int					m_iRemainingMinute;

		OpenTimeSetting () :
			m_iYear ( -1 ),
			m_iMonth  ( -1 ),
			m_iDay  ( -1 ),
			m_iHour  ( -1 ),
			m_iMinute ( -1 ),
			m_iRemainingMinute  ( -1 )
		{ }

		OpenTimeSetting ( int iYear, int iMonth, int iDay, int iHour, int iMinute, int iRemainingMinute ) :
			m_iYear ( iYear ),
			m_iMonth  ( iMonth ),
			m_iDay  ( iDay ),
			m_iHour  ( iHour ),
			m_iMinute ( iMinute ),
			m_iRemainingMinute  ( iRemainingMinute )
		{ }

	};

#endif // ADD_2013_CHARACTER_ADD_EVENT 	// ������, 2013 �ֵ� �߰� ��� ���� �̺�Ʈ

	struct EmblemTemplet
	{
		EMBLEM_ID	m_eEmblemID;			// ID
		
		bool		m_bIsFirstOnly;			// ��ȸ ù ĳ���� ���� �ÿ��� ���� �� ���ΰ�?
		bool		m_bCheckCondition;		// ���� üũ �ؼ� ���� �� ���ΰ�?(FALSE = ������ ���)	
		bool		m_bIsPassCondition;		// ���� üũ ��� ����

		UINT		m_uiShowOrder;			// ��� ����( ������� ����, 0���� ���� )

		wstring		m_wstrTextureFileName;	// ��� �ؽ��� ���� ��
		D3DXVECTOR3 m_vecTime;	// x : ��� ���� �ð�(���̵� �ð� ����)
								// y : ���� �� ���İ� 1�� �� �� ���� �ҿ�Ǵ� �ð�
								// z : ���� �� ���İ� 0�� �� �� ���� �ҿ�Ǵ� �ð�


		// ������ 2���� ����Ʈ�� ���� �Ǵ� ���
		wstring		m_wstrTextureFileName2nd;	// ��� �ؽ��� ���� ��

		D3DXVECTOR2	m_vTexture1stCostomPos;
		D3DXVECTOR2	m_vTexture2ndCostomPos;

		bool		m_bShowVilleage;
		bool		m_bShowBattleFIeld;
		bool		m_bShowDungeon;

		bool		m_bIsAlreadyShow;

#ifdef ADD_PLAY_MUSIC_WHEN_EMBLEM_POP 	// ������, ���� ���� ��, ���� ��� ��� �߰�
		vector<EmblemPopupSound>		m_vecPlayEmblemSoundTemplet;		// ù��° ���� ���� ��, ���� ���� �̸�
#endif // ADD_PLAY_MUSIC_WHEN_EMBLEM_POP 	// ������, ���� ���� ��, ���� ��� ��� �߰�
		

#ifdef ADD_2013_CHARACTER_ADD_EVENT 	// ������, 2013 �ֵ� �߰� ��� ���� �̺�Ʈ
		vector<OpenTimeSetting>			m_vecOpenTimeSettingTemplet;
#endif // ADD_2013_CHARACTER_ADD_EVENT 	// ������, 2013 �ֵ� �߰� ��� ���� �̺�Ʈ

		EmblemTemplet():
		m_bIsFirstOnly(false),
		m_bCheckCondition(false),
		m_uiShowOrder(0),
		m_bShowVilleage(false),
		m_bShowBattleFIeld(false),
		m_bShowDungeon(false),
		m_bIsAlreadyShow(false),
		m_bIsPassCondition(false),
		m_vTexture1stCostomPos(-1.f, -1.f),
		m_vTexture2ndCostomPos(-1.f, -1.f)
		{}

		void InitShowState()
		{
			m_bIsAlreadyShow = false;
		}		
		void InitIsPassCondition()
		{// ���� ���ٸ� ������ true
			if( false == m_bCheckCondition )
				m_bIsPassCondition = true;
			else
			{
				if( true == m_bIsAlreadyShow )
					m_bIsPassCondition = false;
			}
		}
		void GetTexture1stPos( OUT D3DXVECTOR3& vPos) const
		{
			if( m_vTexture1stCostomPos.x > -1.f && m_vTexture1stCostomPos.y > -1.f )
				vPos = D3DXVECTOR3(m_vTexture1stCostomPos.x,m_vTexture1stCostomPos.y,0);
			else
				vPos = D3DXVECTOR3(512,300,0);
		}
		void GetTexture2ndPos( OUT D3DXVECTOR3& vPos) const
		{
			if( m_vTexture2ndCostomPos.x > -1.f && m_vTexture2ndCostomPos.y > -1.f )
				vPos = D3DXVECTOR3(m_vTexture2ndCostomPos.x,m_vTexture2ndCostomPos.y,0);
			else
				vPos = D3DXVECTOR3(512,300,0);
		}
	};

public:
	static CX2EmblemManager* GetInstance();
	static void ResetEmblemManager();
	static void DestroyInstance();
	
	void OnFrameMove( float fElpaseTime );
	void InitEmbelmPlayInfo( bool bCharChange = false );	// ���� ���� ���� ���� �ʱ�ȭ

	void PlayEmblem( EMBLEM_ID eEmblemID_, bool bReShow_ = false ); 
	void EndEmblem( EMBLEM_ID eEmblemID_ );
	bool HasEmblem( EMBLEM_ID eEmblemID_ );

	void SetTexture2ndName( EMBLEM_ID eEmblemID_, const wstring& wstrTextureName );

#ifdef EVENT_NEW_HENIR
	bool IsBurningEventTime() const;
#endif // EVENT_NEW_HENIR

#ifdef ADD_2013_CHARACTER_ADD_EVENT		// 2013 �ֵ� �߰� ��� ���� �̺�Ʈ
	bool SetOpenTimeEventEmblem ( const EmblemTemplet& sEmblemTemplet_ );
#endif // ADD_2013_CHARACTER_ADD_EVENT	// 2013 �ֵ� �߰� ��� ���� �̺�Ʈ
private:
	// �Ľ�
	bool OpenScriptFile( IN const WCHAR* pFileName_ );
	void ParsingEmblemTemplet( KLuaManager& luaManager );

	// ����Ʈ ����
	CKTDGParticleSystem::CParticleEventSequence* CreateEventSequence( const WCHAR* wstrTextureFile_, 
		const D3DXVECTOR3& vTime);
	void PlayEmblemEffect( const EmblemTemplet& sEmblemTemplet_ );
	void PlayEmblemOtherProcess( const EmblemTemplet& sEmblemTemplet_ );
	void CheckNextPlayingEmblem();

private:
	CX2EmblemManager();
	~CX2EmblemManager();
	
	static CX2EmblemManager* m_pEmblemManager;
	std::vector<EmblemTemplet*>	m_vecEmblemTemplet;

	bool m_bHasNextPlayEmblem;	// ������ ��� �� ������ ���� ����
	float m_fWaitTime; // ��� �� ��� �ð�
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hCurrentEmblem;
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hCurrentEmblem2nd;
#ifdef ADD_PLAY_MUSIC_WHEN_EMBLEM_POP		// ������, ���� ���� ��, ���� ��� ��� �߰�
	float m_fSoundPlayingCheckTime;
	vector<EmblemPopupSound>	m_vecNowPlayEmblemSound;
	vector<CKTDXDeviceSound *>	m_vecPlayingEmblemSound;
#endif // ADD_PLAY_MUSIC_WHEN_EMBLEM_POP	// ������, ���� ���� ��, ���� ��� ��� �߰�

};

#endif // NEW_EMBLEM_MANAGER

