#pragma once


// CTriggerEdit ��ȭ �����Դϴ�.

class CTriggerEdit : public CDialog
{
	DECLARE_DYNAMIC(CTriggerEdit)

public:
	CTriggerEdit(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTriggerEdit();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_TRIGGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
