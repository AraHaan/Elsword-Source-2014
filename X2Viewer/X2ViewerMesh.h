#pragma once
/**	@file	: X2ViewerMesh.h
	@desc	: �޽� ó��
	@author	: Anomymouse

	@date	JHKang on 2013-09-13,
		- �ʵ� ���� ���� ����
	@date	JHKang on 2013-09-17,
		- ���� ���� �� ���ȭ
	@date	JHKang on 2013-10-03,
		- ���� �� �Ǽ����� ó��
*/

class CX2ViewerMesh : public CX2ViewerObject
{
	public:
		CX2ViewerMesh(void);
		virtual ~CX2ViewerMesh(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();
		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		void	Reset();
		bool	SetMesh( const WCHAR* fileName, const WCHAR* dir );

		void	SetScale( D3DXVECTOR3& vScale ){ m_vScale = vScale; }
		void	SetScale( float fX, float fY, float fZ );
		void	SetScaleX( float fX ){ m_vScale.x = fX; }
		void	SetScaleY( float fY ){ m_vScale.y = fY; }
		void	SetScaleZ( float fZ ){ m_vScale.z = fZ; }

		void	SetLightPos( D3DXVECTOR3& vLightPos ){ m_RenderParam.lightPos = vLightPos; }
		void	SetLightPos( float fX, float fY, float fZ );
		void	SetLightPosX( float fX ){ m_RenderParam.lightPos.x = fX; }
		void	SetLightPosY( float fY ){ m_RenderParam.lightPos.y = fY; }
		void	SetLightPosZ( float fZ ){ m_RenderParam.lightPos.z = fZ; }

		D3DXVECTOR3&	GetScale(){ return m_vScale; }
		D3DXVECTOR3&	GetLightPos(){ return m_RenderParam.lightPos; }
		void			SetLightOnOff( bool bIsLight ){ m_bIsLight = bIsLight; }

		CKTDGXRenderer::RenderParam*	GetRenderParam(){ return &m_RenderParam; }
		IMPACT_DATA*					GetImpactData(){ return &m_ImpactData; }
		std::vector<TEX_STAGE_DATA>*	GetTexStageData(){ return &m_vecTexStageData; }

	private:
		CKTDXDeviceXMesh*			m_pXMesh;
		CKTDGMatrix*				m_pMatrix;
		CKTDGXRenderer*				m_pRenderer;

		CKTDXDeviceXMesh*			m_pXMeshLight;
		CKTDGMatrix*				m_pMatrixLight;

		D3DXVECTOR3			m_vScale;

		bool				m_bWireframeMode;
		bool				m_bIsLight;
		bool				m_bIsBounding;

		CKTDGXRenderer::RenderParam	m_RenderParam;
		IMPACT_DATA					m_ImpactData;
		std::vector<TEX_STAGE_DATA>	m_vecTexStageData;
		float						m_ImpactNowAnimTime;
};
