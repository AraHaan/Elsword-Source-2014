#pragma once
/**	@file	: X2ViewerParam.h
	@desc	: ������ �Ķ����
	@author	: Anomymouse

	@date	JHKang on 2013-09-13,
		- �ʵ� ���� ���� ����
	@date	JHKang on 2013-09-17,
		- ���� ���� �� ���ȭ
*/

#pragma region CX2ViewerParam
/** @class : CX2ViewerParam
	@brief : ������ �Ķ���� �����͸� ������ Ŭ����
*/
class CX2ViewerParam
{
public:
	typedef std::map<std::wstring, CKTDGXRenderer::RENDER_TYPE> RenderTypeMap;
	typedef	std::map<std::wstring, CKTDGXRenderer::CARTOON_TEX_TYPE> CartoonTexTypeMap;
	typedef std::map<std::wstring, D3DCULL> D3DCullMap;
	typedef std::map<std::wstring, D3DBLEND> D3DBlendMap;
	//typedef std::map<std::wstring, bool> BooleanMap;

public:
	CX2ViewerParam(void);
	~CX2ViewerParam(void);

	void	GetRenderParam( CKTDGXRenderer::RenderParam* renderParam, IMPACT_DATA* impactData,
							std::vector<TEX_STAGE_DATA>& vecTexStageData );

	void	SetParamDlg( CDXUTDialog* pDlg );
	void	GetParamDlg( CDXUTDialog* pDlg );

	void	Reset();
	void	SetEffect();

private:
	RenderTypeMap		m_mapRenderType;		/// Rendering Type Map ������
	CartoonTexTypeMap	m_mapCartoonTexType;	/// Cartoon Texture Type Map ������
	D3DCullMap			m_mapD3DCull;			/// D3DCULL Map ������
	D3DBlendMap			m_mapD3DBlend;			/// D3DBlend Map ������
	//BooleanMap			m_mapTrueFalse;			/// AlphaBlend, ZEnable
	bool	m_bAlpha;		/// AlphaBlend
	bool	m_bZEnable;		/// ZEnable
	bool	m_bZWritable;	/// ZWritable

	CKTDGXRenderer::RenderParam*		m_pRenderParam;		/// Rendering Parameter
	IMPACT_DATA							m_ImpactData;		/// ����Ʈ �÷ο� ȿ���� ����ϴ� ������
	std::vector<TEX_STAGE_DATA>			m_vecTexStageData;	/// �ؽ�ó �������� ������
};
#pragma endregion class
