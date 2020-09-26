
#pragma once
#include "afxwin.h"


// CCrashImageDlg ��ȭ �����Դϴ�.

class CCrashImageDlg : public CDialog
{
	DECLARE_DYNAMIC(CCrashImageDlg)

public:
	CCrashImageDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCrashImageDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
    CStatic m_pic;

    void OnPaint();
    char LoadImage(const char *parm_path);
    void Draw();

private:
    // �̹��� ��ü�� �����͸� ������ ����
    LPPICTURE mp_image_object;
    // �о���� �̹����� ���� ���̿� ���� ũ�� ����
    long m_width_size, m_height_size;

};
