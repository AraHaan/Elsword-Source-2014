#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CLineCamera ��ȭ �����Դϴ�.

class CLineCamera : public CDialog
{
	DECLARE_DYNAMIC(CLineCamera)

public:
	CLineCamera(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLineCamera();

	void ResetCameraList();
	void UpdateCameraInfo(int iItem);

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_LINE_CAMERA };

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_CameraList;
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	
	int m_iSelectedItem;	
	CComboBox m_ComboLineType;
	CEdit m_editEye;
	CEdit m_editLookat;
	afx_msg void OnBnClickedButtonGetcamera();
	CButton m_CheckFocusUnit;
	CEdit m_editReleativeEye;
	CEdit m_editRepositionSpeed;
	afx_msg void OnBnClickedButtonCameraInit();
	afx_msg void OnBnClickedButtonUpdateCamera();
	afx_msg void OnBnClickedButtonAddCamera();
};
