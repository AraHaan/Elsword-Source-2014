#pragma once
#include "afxcmn.h"


// CTriggerFunc ��ȭ �����Դϴ�.

class CTriggerFunc : public CDialogEx
{
	DECLARE_DYNAMIC(CTriggerFunc)

public:
	CTriggerFunc(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTriggerFunc();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_TRIGGER_FUNC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	void SetFunctionData(wstring wstrFunctionData);
};
