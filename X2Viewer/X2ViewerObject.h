#pragma once
/**	@file	: X2ViewerMesh.h
	@desc	: �� ��ü ����
	@author	: Anomymouse

	@date	JHKang on 2013-09-13,
		- �ʵ� ���� ���� ����
	@date	JHKang on 2013-09-17,
		- ���� ���� �� ���ȭ
*/

#pragma region CX2ViewerObject
/** @class : CX2ViewerObject
	@brief : �� ������Ʈ
	@date  : 2013-09-17
*/
class CX2ViewerObject
{
	public:
		enum OBJECT_STYLE
		{
			OS_NONE		= 0,	/// ����
			OS_GRID,			/// ����
			OS_CAMERA,			/// ī�޶�
			OS_SKIN_MESH,		/// ��Ų �޽�
			OS_MESH,			/// �޽�
			OS_UI,				/// UI
			OS_WORLD_MESH,		/// ���� �޽�

#ifndef UPDATE_X2VIEWER_2013 //JHKang
			OS_PARTICLE,
			OS_PARTICLE_EDITOR,
#endif //UPDATE_X2VIEWER_2013
		};

	public:
		CX2ViewerObject(void);
		virtual ~CX2ViewerObject(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime ){return S_OK;}
		virtual HRESULT OnFrameRender(){return S_OK;}

		virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){return false;}

		virtual HRESULT OnResetDevice(){return S_OK;}
		virtual HRESULT OnLostDevice(){return S_OK;}

		OBJECT_STYLE	GetObjectStyle()							{ return m_ObjectStyle; }
		void			SetObjectStyle( OBJECT_STYLE objectStyle )	{ m_ObjectStyle = objectStyle; }              
        
	private:
		OBJECT_STYLE	m_ObjectStyle;	/// ������Ʈ ��Ÿ��
};
#pragma endregion class
