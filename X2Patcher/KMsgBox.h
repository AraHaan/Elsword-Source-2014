#ifndef __KMSGBOX_H__
#define __KMSGBOX_H__

#include <windows.h>
#include <tchar.h>
#include "KAssert.h"
#include "resource.h"

/// �޼����� ����ڿ��� �����ֱ� ���� �޼����ڽ� Ŭ����.
/// �׷���ü�̽��� �̹����� ����Ͽ� �ٸ���Ҵ�.
/// Open �޽��� ������ Ÿ���� ����� �����ϸ�,
/// �� �� �⺻ Ÿ���� �𵨸��� ���̾�α� �ڽ��μ�
/// �ݵ�� Close�޽��� �ݾ��־�� �Ѵ�.
/// ������ Ÿ�Ե��� ��޷� ��µǹǷ� Close�޽�带 ������� �ʾƵ� �ȴ�.
/// ��ġ ���α׷� �Ӹ� �ƴ϶�, ���� Ŭ���̾�Ʈ������ ����ϴ� Ŭ�����̴�.
class KMsgBox
{
private: // Disable construction of this class!
    /// Default constructor - disabled.
    KMsgBox(void)
    { ASSERT(false && _T("Do not use default constructor!")); }
    /// Copy constructor - disabled.
    KMsgBox(const KMsgBox&)
    { ASSERT(false && _T("Do not use copy constructor!")); }
    /// Default destructor.
    ~KMsgBox(void) {}
    /// Assignment operator - disabled.
    const KMsgBox& operator=(const KMsgBox&)
    { ASSERT(false && _T("Do not use assignment operator!")); return *this; }
public:
    enum // Message box type
    {
        MBOX_NOTHING,   ///< Dummy value.
        MBOX_DEFAULT,   ///< ��ư�� ���� �⺻ Ÿ��.
        MBOX_OK,        ///< "Ȯ��" ��ư�� �ִ� Ÿ��.
        MBOX_OKCANCEL,  ///< "Ȯ��", "���" ��ư�� �ִ� Ÿ��.
    };
public:
    /// Open message box.
    static INT_PTR Open(const char* szTxtLn1,
                        const char* szTxtLn2,
                        UINT uType = MBOX_DEFAULT,
                        HWND hParentWnd = NULL);
    /// Close message box.
    static BOOL Close(int iResult = IDOK);
private:
    enum
    {
        // Message text line
        LINE_LENGTH = 144,		// 09.08.07 �¿� : UTF-8 ����� ���ؼ� ���� 32���� ���� ũ�⸦ 4��� �ø�
        LINE_INDENT = 16,
        FIRST_LINE  = 66,
        SECOND_LINE = 86,

        // Message box size
        MBOX_WIDTH  = 303,
        MBOX_HEIGHT = 145,
    };
private:
    /// Application-defined dialog box callback function.

	static void ConvertUtf8ToWCHAR( WCHAR* dest, const char* pSrc, const int iLength = -1 );
    static BOOL CALLBACK MsgBoxProc(HWND hDlg, UINT uMsg,
                                    WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK MsgBoxProcUTF8(HWND hDlg, UINT uMsg,
									WPARAM wParam, LPARAM lParam);
private:
    static HWND ms_hDlg;                      ///< Handle to message box.
    static UINT ms_uType;                     ///< Type of message box.
    static char ms_szTxtLn1[LINE_LENGTH + 1]; ///< First line message.
    static char ms_szTxtLn2[LINE_LENGTH + 1]; ///< Second line message.
};

#endif // __KMSGBOX_H__