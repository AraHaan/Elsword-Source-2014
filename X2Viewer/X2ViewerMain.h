#pragma once
/**	@file	: X2ViewerMain.h
	@desc	: ��� ���� �ڵ�
	@author	: Anomymouse

	@date	JHKang on 2013-09-13,
		- �ʵ� ���� ���� ����
	@date	JHKang on 2013-09-17,
		- ���� ���� �� ���ȭ
*/

// ����� #define X2VIEWER �� �ּ���. ( KTDXLIB )

class CX2ViewerMain : public CKTDXStage
{
	public:
		CX2ViewerMain(void);
		virtual ~CX2ViewerMain(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		CX2ViewerObject*	GetObject( CX2ViewerObject::OBJECT_STYLE objectStyle );

//		D3DXMATRIX*			GetWeaponMatrix() { return m_pWeaponMatrix; }
//		void				SetWeaponMatrix( D3DXMATRIX* pMatrix ) { m_pWeaponMatrix = pMatrix; }

		int					GetSelectedAnimIndex() { return m_SelectedAnimIndex; }
		void				SetSelectedAnimIndex( int index ) { m_SelectedAnimIndex = index; }

		wstring				GetAnimFileName() { return m_AnimFileName; }
		void				SetAnimFileName(const WCHAR* pName) { m_AnimFileName = pName; }

		wstring				GetAnimDirName() { return m_AnimFileDir; }
		void				SetAnimDirName(const WCHAR* pName) { m_AnimFileDir = pName; }

		// 09.12.23 �¿� :(��û�� ���� ��� �߰�) Resource �о���� ������ �߰��Ѵ�. �ش簪�� LUA���� �����ϵ��� �Ѵ�. 
		void				ReadAdditionalResourceFolder();
		void				AddAdditionalResourceFolder( string folder, bool bIncludeSub );

		void	RegisterLuabind( KLuabinder* pKLuabinder );

	private:
		std::vector<CX2ViewerObject*>	m_vecObject;
		CX2ViewerSkinMesh*				m_pWeaponXSkinMesh;

//		D3DXMATRIX*						m_pWeaponMatrix;
		int								m_SelectedAnimIndex;
		wstring							m_AnimFileName;
		wstring							m_AnimFileDir;
};
