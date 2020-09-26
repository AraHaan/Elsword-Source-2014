#pragma once
/**	@file	: X2ViewerSkinMesh.h
	@desc	: �޽� ���� ����
	@author	: Anomymouse

	@date	JHKang on 2013-09-13,
		- �ʵ� ���� ���� ����
	@date	JHKang on 2013-09-17,
		- ���� ���� �� ���ȭ
*/

#pragma region CX2ViewerSkinMesh
/** @class : CX2ViewerSkinMesh
	@brief : �޽� ���� Ŭ����
*/
class CX2ViewerSkinMesh : public CX2ViewerObject
{
public:
	struct SBoneInfo;
	struct SUnitBoneModify;
	
	typedef std::map<std::wstring, CKTDXDeviceXSkinMesh*> XSkinMeshMap;
	typedef std::vector<SBoneInfo*> ModifyBoneInfoVector;
	typedef std::vector<SUnitBoneModify*> UnitBoneModifyVector;
	typedef std::map<std::wstring, SWeaponInfo*> WeaponMap;
	typedef std::map<std::wstring, SAccessoryInfo*> AccessoryMap;

	enum SKINMESH_OPEN_TYPE
	{
		SOT_NONE = 0,		/// ����
		SOT_SKINMESH,		/// ����� �ִ� �޽�
		SOT_MESH,			/// ����� ���� �޽�
		SOT_NOT_ADDMESH,	/// �߰����� �ʴ� �޽�
		SOT_END,			/// ����ü�� ��
	};

	/** @struct : SModifyBoneInfo
		@brief : ���� ��ȯ ����
		@date  : 2013/09/28
	*/
	struct SBoneInfo
	{
		wstring				m_strBoneName;		/// ���� �̸�
		MODIFY_BONE_STYLE	m_eModifyBoneStyle;	/// ���� ��ȯ ���
		D3DXVECTOR3			m_vValue;			/// ��ġ �Ǵ� ũ�� ��ȭ ��
	};
		
	/** @struct : SUnitBoneModify
		@brief : ĳ������ ����� ��ȯ ����
		@date  : 2013/09/28
	*/
	struct SUnitBoneModify
	{
		CHARACTER_ID			m_eUnitID;				/// UNIT ��ȣ
		ModifyBoneInfoVector	m_vecModifyBoneInfo;	/// ���� ����� ���� ��ȯ ����
	};


public:
	CX2ViewerSkinMesh(void);
	virtual ~CX2ViewerSkinMesh(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();
	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();
    virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool	SetAnimXSkinMesh( std::wstring fileName );
	bool	AddWeapon( const WCHAR* pFullFileName, const WCHAR* attachFrameName, D3DXVECTOR3 rot );
    bool    AddAccessory( const WCHAR* pFullFileName, const WCHAR* attachFrameName, D3DXVECTOR3 rot );
	bool	DelModelXSkinMesh( std::wstring fileName );
	bool	DelModelXSkinAnim( std::wstring fileName );
	bool	DelModelXMesh( std::wstring fileName );
	SKINMESH_OPEN_TYPE	InsertSkinMesh( std::wstring& fileName, std::wstring& dir );
	
	void	Reset();
	void	ChangeAnim( std::wstring animName );
	void	ChangeAnim( int index );

	void	SetScale( D3DXVECTOR3& vScale )	{ m_vScale = vScale; }
	void	SetScale( float fX, float fY, float fZ );
	void	SetScaleX( float fX )	{ m_vScale.x = fX; }
	void	SetScaleY( float fY )	{ m_vScale.y = fY; }
	void	SetScaleZ( float fZ )	{ m_vScale.z = fZ; }

	void	SetPosition( D3DXVECTOR3& vPosition )	{ m_vPosition = vPosition; }
	void	SetPosition( float fX, float fY, float fZ );
	void	SetPositionX( float fX )	{ m_vPosition.x = fX; }
	void	SetPositionY( float fY )	{ m_vPosition.y = fY; }
	void	SetPositionZ( float fZ )	{ m_vPosition.z = fZ; }

    void    SetTransX( float fX )    { m_TransAccessory.x = fX; }
    void    SetTransY( float fY )    { m_TransAccessory.y = fY; }
    void    SetTransZ( float fZ )    { m_TransAccessory.z = fZ; }
    void    SetAccRotX( float fX )    { m_RotAccessory.x = fX; }
    void    SetAccRotY( float fY )    { m_RotAccessory.y = fY; }
    void    SetAccRotZ( float fZ )    { m_RotAccessory.z = fZ; }
    void    SetAccScaleX( float fX )  { m_ScaleAccessory.x = fX; }
    void    SetAccScaleY( float fY )  { m_ScaleAccessory.y = fY; }
    void    SetAccScaleZ( float fZ )  { m_ScaleAccessory.z = fZ; }

    void    SetWeaponRotX( float fX )       { m_RotWeapon.x = fX; }
    void    SetWeaponRotY( float fY )       { m_RotWeapon.y = fY; }
    void    SetWeaponRotZ( float fZ )       { m_RotWeapon.z = fZ; }

	float    GetWeaponRotX()	{ return m_RotWeapon.x; }
    float    GetWeaponRotY()	{ return m_RotWeapon.y; }
    float    GetWeaponRotZ()	{ return m_RotWeapon.z; }

	void	SetLightPos( D3DXVECTOR3& vLightPos )	{ m_RenderParam.lightPos = vLightPos; }
	void	SetLightPos( float fX, float fY, float fZ );
	void	SetLightPosX( float fX )	{ m_RenderParam.lightPos.x = fX; }
	void	SetLightPosY( float fY )	{ m_RenderParam.lightPos.y = fY; }
	void	SetLightPosZ( float fZ )	{ m_RenderParam.lightPos.z = fZ; }

	void	SetWireFrameMode( bool bWireFrame )	{ m_bWireframeMode = bWireFrame; }

	D3DXVECTOR3&	GetScale()	{ return m_vScale; }
	D3DXVECTOR3&	GetPosition()	{ return m_vPosition; }
	D3DXVECTOR3&	GetLightPos()	{ return m_RenderParam.lightPos; }
	void	SetLightOnOff( bool bIsLight )	{ m_bIsLight = bIsLight; }

    bool	GetFrameNameList( VecMultiAnimFrame& vecFrameNameList );
	bool	GetAnimNameList( std::vector<std::wstring>& vecAnimNameList );

	bool	SetAnimPlayPause();
	void	SetAnimStop();
	void	SetMotionOnOff( bool bIsMotion );
	bool	GetMotionOnOff()	{ return m_bIsMotion; }
	void	SetPlaySpeed( float fPlaySpeed );
	void	GetAnimTime( float& fNowTime, float& fMaxTime );
	void	SetAnimTime( float fTime );
	void	SetPlayType( WCHAR* wszPlayType );
	void	SetBounding( bool bIsBounding )	{ m_bIsBounding = bIsBounding; }
	bool	GetBounding()	{ return m_bIsBounding; }
	void	SetShowAttackBox( bool bShowAttackBox ) { m_bShowAttackBox = bShowAttackBox; }
	bool	GetShowAttackBox() { return m_bShowAttackBox; }

	CKTDGXRenderer::RenderParam*	GetRenderParam()	{ return &m_RenderParam; }
	IMPACT_DATA*					GetImpactData()		{ return &m_ImpactData; }
	std::vector<TEX_STAGE_DATA>*	GetTexStageData()	{ return &m_vecTexStageData; }
	CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE	GetPlayType()	{ return m_AnimPlaytype; }

	CKTDGXSkinAnim*	GetXSkinAnim()			{ return m_pXSkinAnim; }
	//CKTDGXSkinAnim* GetWeaponXSkinAnim( Value_ )	{ return m_mapXSkinWeapon[iValue_]; }
	WeaponMap& GetWeaponXSkinAnim() { return m_mapXSkinWeapon; }
	CKTDGXSkinAnim* GetWeaponXSkinAnim( wstring strName_ );
	SWeaponInfo* GetWeaponInfo( wstring strName_ );

	AccessoryMap& GetAccessoryXMesh() { return m_mapXMeshAccessory; }
	CKTDXDeviceXMesh* GetAccessoryXMesh( wstring strName_ );
	SAccessoryInfo* GetAccessoryInfo( wstring strName_ );
		
    void SetAttachPoint(WCHAR *szName);
    void SelectionChange( DWORD dwX,  DWORD dwY);
    BOOL BIntersectMeshContainer ( CKTDXDeviceXSkinMesh::MultiAnimMC *pmcMesh, DWORD dwX, DWORD dwY, D3DVIEWPORT9 *pViewport,
		D3DXMATRIX *pmatProjection, D3DXMATRIX *pmatView, float *pfDistMin, /*SMeshContainer **ppmcHit,*/ DWORD *pdwFaceHit,
        DWORD *pdwVertHit );

	bool OpenScriptFile( IN const WCHAR* pFileName_, KLuabinder* pKLuabinder );
	void RefreshBoneInfoScript( KLuabinder* pKLuabinder );
	bool AddBoneInfo();
	void SetUnitReForm( CHARACTER_ID eValue_ = CI_NONE );
	void GetModifyBoneList( CHARACTER_ID eValue_, VecMultiAnimFrame &vecBoneList_ );

private:
	CKTDGXSkinAnim*		m_pXSkinAnim;			/// ���� ���
	CKTDXDeviceXET*		m_pXET;					/// ��� XET
	
	CKTDGXRenderer*		m_pRendererAccessory;	/// �Ǽ������� ������ �Ķ���� ����
    
	CKTDXDeviceXMesh*   m_pXMeshAccessory;		/// �Ǽ����� �޽�
	AccessoryMap		m_mapXMeshAccessory;	/// �Ǽ����� �޽� ��
	
	WeaponMap			m_mapXSkinWeapon;		/// ���� ��Ų �޽� ��
	CKTDXDeviceXET*		m_pXETWeapon;			/// ���� XET ����

	XSkinMeshMap		m_mapSkinMesh;			/// SkinMesh Map Data

	CKTDXDeviceXMesh*	m_pXMeshLight;			/// �¾� ��� �޽�
	CKTDGMatrix*		m_pMatrixLight;			/// �� ���

	CKTDXDeviceXMesh*	m_pXMeshSphere;			/// �浹�� �޽�
	CKTDXDeviceXMesh*	m_pXAttackMeshSphere;	/// ���ÿ� �޽�
	CKTDGMatrix*		m_pMatrixSphere;		/// �浹, ���ùڽ��� ���

	D3DXVECTOR3			m_vScale;				/// ���� ����
	D3DXVECTOR3			m_vPosition;			/// ��ġ ����

	bool	m_bWireframeMode;	/// ���̾� ������ ���
	bool	m_bIsLight;			/// �� ���
	bool	m_bIsAnimPlay;		/// ���
	bool	m_bIsMotion;		/// ����� �� �޽��� ǥ���� ���ΰ�?
	bool	m_bIsBounding;		/// �ٿ�� �ڽ� ǥ��
	bool	m_bShowAttackBox;	/// ���� �ڽ� ǥ��

	CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE	m_AnimPlaytype;			/// ��� ���
	CKTDGXRenderer::RenderParam			m_RenderParam;			/// ������ �ĸ�����
	IMPACT_DATA							m_ImpactData;			/// �÷ο� ����Ʈ ����
	std::vector<TEX_STAGE_DATA>			m_vecTexStageData;		/// �ؽ�ó �������� ������
	float								m_ImpactNowAnimTime;	/// �÷ο� ����Ʈ �� ����� ���� �ð� ��

	D3DXMATRIX*			m_pWeaponMatrix;	/// ���
	D3DXVECTOR3			m_RotWeapon;		/// ���� ȸ��

    D3DXMATRIX*			m_pMatrixAccessory;	/// �Ǽ����� ���
    D3DXVECTOR3			m_TransAccessory;	/// �̵� ����
    D3DXVECTOR3			m_RotAccessory;		/// ȸ�� ����
    D3DXVECTOR3			m_ScaleAccessory;	/// ���� ����

    bool                m_bAttachPoint;		/// ���̴� ��ġ
    CKTDGXRenderer*		m_pRendererPoint;	/// ������ ������
    CKTDXDeviceXMesh*	m_pXMeshPoint;		/// �޽��� ������
    D3DXMATRIX*			m_pMatrixPoint;		/// ��� ������
	D3DXVECTOR3			m_vBoneScale;		/// ���� ũ�� ����

	UnitBoneModifyVector	m_vUnitBoneModify;		/// �⺻ ���� ����� ��ȯ ������ ������ ����

	//LPD3DXMESH	m_pSMesh;
	//CKTDGMatrix* m_pSMatrix;
};
#pragma endregion class
