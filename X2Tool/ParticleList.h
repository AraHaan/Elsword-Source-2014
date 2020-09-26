#pragma once
#include "afxwin.h"


// ParticleList ��ȭ �����Դϴ�.

class ParticleList : public CDialogEx
{
	DECLARE_DYNAMIC(ParticleList)

public:
	ParticleList(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~ParticleList();

	virtual void OnFinalRelease();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_PARTICLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
// 	DECLARE_DISPATCH_MAP()
// 	DECLARE_INTERFACE_MAP()

public:
	CKTDGParticleSystem* GetParticleSystem();
	void UpdateParticleList(int iType);
	afx_msg BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

public:
	CComboBox m_comboParticleType;
	CListBox m_listParticle;

	int m_iSelCombo;
	wstring	m_wstrParticleName;
	CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqPreview;

	afx_msg void OnCbnSelchangeComboParticleType();
	afx_msg void OnLbnSelchangeListParticle();
	afx_msg void OnBnClickedButton1();
};
