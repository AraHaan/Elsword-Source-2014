#pragma once
/**	@file	: X2ViewerGrid.h
	@desc	: ��� ���ο� ���� ���
	@author	: Anomymous

	@date	JHKang on 2013-09-13,
		- �ʵ� ���� ���� ����
	@date	JHKang on 2013-09-17,
		- ���� ���� �� ���ȭ
*/

#define D3DFVF_GRID_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define LINE_NUM	46

#pragma region CX2ViewerGrid
/** @class : CX2ViewerGrid
	@brief : ����
	@date  : 2013-09-17
*/
class CX2ViewerGrid : public CX2ViewerObject
{
	public:
		struct GRID_VERTEX
		{
			D3DXVECTOR3	pos;	/// ��ġ
			DWORD		color;	/// ����
		};

	public:
		CX2ViewerGrid(void);
		virtual ~CX2ViewerGrid(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();
		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		void	SetOnOff( bool bOnOff ){ m_bOnOff = bOnOff; }

	private:
		bool	Init();

	private:
		
		//LPDIRECT3DDEVICE9			m_pd3dDevice;
		LPDIRECT3DVERTEXBUFFER9		m_pGridVB;		/// ���� ����
		CKTDGMatrix*				m_pMatrix;		/// ���

		//CKTDGFontManager::CKTDGFont*	m_pFont;
		CKTDGFontManager::CUKFont*	m_pFont;		/// ��Ʈ

		CKTDIDevice*	m_pKTDIMouse;		/// ���콺 ��ġ
		bool			m_bOnOff;			/// On/Off
};
#pragma endregion class