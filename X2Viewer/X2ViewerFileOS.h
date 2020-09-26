#pragma once
/**	@file	: X2ViewerFileOS.h
	@desc	: ���� �����
	@author	: Anomymous

	@date	JHKang on 2013-09-13,
		- �ʵ� ���� ���� ����
	@date	JHKang on 2013-09-17,
		- ���� ���� �� ���ȭ
*/

#pragma region CX2ViewerFileOS
/** @class : CX2ViewerFileOS
	@brief : ���� �ý���
	@date  : 2013-09-17
*/
class CX2ViewerFileOS
{
	public:
		enum FILE_STATE
		{
			FS_NONE,	/// �ƹ� ���µ� �ƴ�
			FS_XFILE,	/// XFile ����
			FS_LUA,		/// Scene Fiile ����(Lua)
		};

	public:
		CX2ViewerFileOS(void);
		~CX2ViewerFileOS(void);

		FILE_STATE		FileOpen( WCHAR* wsFilter );
		FILE_STATE		FileSave( WCHAR* wsFilter );

		std::wstring	GetPullFileName();
		std::wstring	GetTitleFileName();
		std::wstring	GetDirName();

	private:
		OPENFILENAME	m_OFN;		/// ���ϸ�
		HWND			m_hWnd;		/// ������ �ڵ�

		std::wstring	m_wsPULLFileName,	/// ���ϸ� ��ü
						m_wsTITLEFileName,	/// Ÿ��Ʋ ���ϸ�
						m_wsDirName;		/// ���丮��
};
#pragma endregion class