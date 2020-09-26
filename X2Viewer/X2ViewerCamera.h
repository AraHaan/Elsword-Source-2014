#pragma once
/**	@file	: X2ViewerCamera.h
	@desc	: ī�޶� ���� ��� ����
	@author	: AnomyMous

	@date	JHKang on 2013-09-13,
		- �ʵ� ���� ���� ����
	@date	JHKang on 2013-09-17,
		- ���� ���� �� ���ȭ
*/

#pragma region CX2ViewerCamera
/** @class : CX2ViewerCamera
	@brief : ��� ī�޶�
	@date  : 2013-09-17
*/
class CX2ViewerCamera : public CX2ViewerObject
{
public:
	enum CAMERA_MODE
	{
		CM_NORMAL = 0,
		CM_NAVIGATION,
	};

	enum LOCAR_DIR
	{
		LOCAL_X = 0,
		LOCAL_Y,
		LOCAL_Z
	};

public:
	CX2ViewerCamera();
	virtual ~CX2ViewerCamera(void);

	virtual	HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual	HRESULT	OnFrameRender();
	virtual	HRESULT	OnResetDevice();

	D3DXVECTOR3	GetEye()	{ return m_sCamera.m_Eye;}
	void SetEye( D3DXVECTOR3 v3Value_ ) { m_sCamera.m_Eye = v3Value_; }
	void SetEyeX( float fValue_ ) { m_sCamera.m_Eye.x = fValue_; }
	void SetEyeY( float fValue_ ) { m_sCamera.m_Eye.y = fValue_; }
	void SetEyeZ( float fValue_ ) { m_sCamera.m_Eye.z = fValue_; }

	D3DXVECTOR3	GetLookVec(){ return m_sCamera.m_LookAt;}
	void SetLookVec( D3DXVECTOR3 v3Value_ ) { m_sCamera.m_LookAt = v3Value_; }
	void SetLookVecX( float fValue_ ) { m_sCamera.m_LookAt.x = fValue_; }
	void SetLookVecY( float fValue_ ) { m_sCamera.m_LookAt.y = fValue_; }
	void SetLookVecZ( float fValue_ ) { m_sCamera.m_LookAt.z = fValue_; }

	D3DXVECTOR3	GetUpVec()	{ return m_sCamera.m_UpVec;}

	void		SetCameraMode( CAMERA_MODE cameraMode );
	CAMERA_MODE	GetCameraMode(){ return m_CameraMode; }

	void		CameraReset(){ Init(); }

	bool GetUsingWheel() { return m_bUseWheel; }
	void SetUsingWheel( bool bValue_ ) { m_bUseWheel = bValue_; }

private:
	void		Init();
	void		CameraMode_Normal();
	void		CameraMode_Function();

	void		SetTracking( float Time = 1.0f );
	void		MoveLocal( float dist, LOCAR_DIR localDir = LOCAL_X );
	void		RotateLocal( LOCAR_DIR localDir, float angle );
	void		SetView( float fElapsedTime = 0.0f );


private:

	//LPDIRECT3DDEVICE9			m_pd3dDevice;
	CKTDIDevice*				m_pMouse;			/// ���콺 ��ġ
	CKTDIDevice*				m_pKeyboard;		/// Ű���� ��ġ

	CKTDGCamera::CameraData		m_sCamera;			/// ī�޶� ����
	D3DXMATRIX					m_matWorld;			/// ���� ���
	D3DXMATRIX					m_matProjection;	/// ���� ���
	D3DXVECTOR3					m_vView;			/// ī�޶� ���ϴ� �������⺤��
	D3DXVECTOR3					m_vCross;			/// ī�޶��� ���麤�� cross( view, up )

	// MMO STyle
	D3DXVECTOR3					m_vAngle;			/// ����
	float						m_fDist;			/// �Ÿ�
	float						m_fElapsedTime;		/// ���� �ð�

	long						m_nMouseNowPosX, m_nMousePrePosX;	/// ���콺 ����, ���� X ��ǥ
	long						m_nMouseNowPosY, m_nMousePrePosY;	/// ���콺 ����, ���� Y ��ǥ
	long						m_nWheel;							/// ���콺 �� ����

	D3DXVECTOR3					m_TrackAt;			/// Track ����
	float						m_fTrackTime;		/// Ʈ�� �ð�
	bool						m_bIsTrack;			/// Ʈ�� ����

	CAMERA_MODE					m_CameraMode;

	bool	m_bUseWheel;	/// ī�޶� ���� ��� ����
};
#pragma endregion class