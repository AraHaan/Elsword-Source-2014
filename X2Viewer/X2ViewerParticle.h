#pragma once
//////////////////////////////////////////////////////////////////////////
// 2009.12 �¿�
// X2Viewer���� ��ƼŬ �������� ���� ����ϴ� class.
// ��ƼŬ ���� ��� ���õ� �����͵� �Ϻ� ������ �ִ�.
//
// 
class CX2ViewerParticle : public CX2ViewerObject
{
public:

	enum PARTICLEOBJECT_MODE
	{
		PM_NORMAL,
		PM_EDITOR,
	};

	// ��ƼŬ�� �޽� / ��Ų�޽��� ȭ�鿡 ǥ���� �� ����ϴ�, ���� ������ ���� ����ü.
	// NPC ��ũ��Ʈ�� PARTICLE_SEQ�� ���� data�� ��� �����Ѵ�. ( ��ũ��Ʈ�� �̾Ƴ� �� �ֵ��� )
	struct ParticleEffectData
	{
		bool		m_bIsTemplet;
		bool		m_bUse;
		//bool		m_bMajor;		// Tool���� �� ����.
		float		m_fTime;
		wstring		m_Name;
		wstring		m_SequenceName;
		bool		m_bWeapon;
		wstring		m_Pos;
		//bool		m_StateEndDelete;		// ����� ������ ������ ������ ��.
		bool		m_bTrace;
		D3DXVECTOR3 m_vOffset;
		bool		m_bLandPosition;
		bool		m_bApplyUnitRotation;
		D3DXVECTOR3	m_vRotation;
		D3DXVECTOR3 m_vScale;
		bool		m_bUnitScale;


	#ifdef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE //JHKang
		CKTDGParticleSystem::CParticleHandle m_hSeq;
	#else //X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		CKTDGParticleSystem::CParticleEventSequenceHandle  m_hSeq;
	#endif //X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		ParticleEffectData()
		{
			m_bIsTemplet		= true;
			m_bUse				= false;
			//m_bMajor			= false;
			m_fTime				= 0.0f;
			m_bWeapon			= false;
			//m_StateEndDelete	= false;
			m_hSeq				= INVALID_PARTICLE_HANDLE;
			m_bTrace			= true;
			m_vOffset			= D3DXVECTOR3(0, 0, 0);
			m_bLandPosition		= false;
			m_bApplyUnitRotation = false;
			m_vRotation			= D3DXVECTOR3(0, 0, 0);
			m_vScale            = D3DXVECTOR3(1, 1, 1);
			m_bUnitScale		= false;

		}
		~ParticleEffectData(){}

	};

	CX2ViewerParticle( CX2ViewerSkinMesh* pSkinMeshObj );
	virtual ~CX2ViewerParticle(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();
	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	CKTDGParticleSystem*		GetParticleSystem()			{ return m_pParticleSystem; }
	CKTDGParticleSystem*		GetCustomParticleSystem()			{ return m_pCustomParticleSystem; }

	int					GetParticleEffectNum(){ return m_ParticleEffData.size(); }
	vector<ParticleEffectData*>& GetParticleEffectData(){ return m_ParticleEffData; }
	ParticleEffectData* GetParticleEffectDataByName( const wstring name );
	bool				AddParticleEffectData( ParticleEffectData* pData );
	bool				DeleteParticleEffectData( const wstring name );
	void				SaveParticleEffectData( const WCHAR* pwszFileFullPath );
	
	void				ResetAllParticle();
	void				ReloadParticleFile();

	void				SetMode( PARTICLEOBJECT_MODE pm ){ m_mode = pm; }

	void				SetPreviewParticle( wstring name, bool bIsTemplet );
	bool				CopyTempletParticleToCustom( wstring name );
	
	CX2ViewerParticleEditor&	GetParticleEditor(){ return m_X2ParticleEditor; }
	void				ReplayPreviewParticle();
	void				SetPreviewPlayMode( bool bIsReplay ){ m_bPreviewRepeat = bIsReplay; }
	bool				GetPreviewPlayMode(){ return m_bPreviewRepeat; }
	void				SetPreviewPlaySpeed( float fPreviewPlaySpeed ){ m_fPreviewPlaySpeed = fPreviewPlaySpeed; }
	float				GetPreviewPlaySpeed(){ return m_fPreviewPlaySpeed; }
	void				SetPreviewPlayTime( float time ){ m_fElapsedPlayTime = time; }
	float				GetPreviewPlayTime(){ return m_fElapsedPlayTime; }


private:
	CKTDGParticleSystem*		m_pParticleSystem;				// ������ Particle�� ���⼭ �����ȴ�.
	CKTDGParticleSystem*		m_pCustomParticleSystem;		// ������ ������ Paticle Templet�� ���´�. ��ƼŬ ���� ������� ������
																// ��ƼŬ���� �� �Ʒ����� �����ȴ�.

	vector<ParticleEffectData*>						m_ParticleEffData;		// Model Viwe ���¿��� ǥ���� ��ƼŬ ����Ʈ
	CX2ViewerSkinMesh*			m_pSkinMeshObj;								// X2Viewer���� ǥ���ϰ� �ִ� ��Ų�޽�
	int m_iLastPlayCount;
	wstring m_LastAnimName;
	
	PARTICLEOBJECT_MODE m_mode;									// Model view����, ��ƼŬ ���� ��������

	ParticleEffectData			m_PreviewParticleEffData;		// Particle Edit��忡�� ǥ���� ��ƼŬ
	bool						m_bPreviewRepeat;
	CX2ViewerParticleEditor		m_X2ParticleEditor;
	float						m_fPreviewPlaySpeed;
	float						m_fElapsedPlayTime;

};
