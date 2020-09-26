#pragma once

/**
	@file	MainFrm.h
	@author	Wonpok
	@brief	CMainFrame Ŭ������ �������̽�
		
	@date	JHKang on 2013/02/19,
			- ��� �ľ�
			- ���� �۾�( �ּ�ó��, �� ������, ���� ���� ���� )
*/

#pragma region Includes
#include "ClassView.h"
#include "PropertiesWnd.h"
#include "TriggerPropertiesWnd.h"
#include "Resource.h"
#include "SelectToolUnit.h"
#include "DungeonInfo.h"
#include "CameraInfo.h"
#include "LineStartPositionInfo.h"
#include "LineBaseEditor.h"
#include "ToolStartNotice.h"
#include "EditWorld.h"
#include "EditDungeon.h"
#include "TriggerEdit.h"
#pragma endregion MainFrm�� ���Ǵ� ��ɵ�

#pragma region CMainFrame
class CMainFrame : public CFrameWndEx
{
	#pragma region protected
protected:
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	#pragma endregion function
	
	#pragma region public
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
		
	void CreateDockingWindowAfter();
	void OnResetMain( bool bEditLine, bool bEditWorld, bool bViewLine, bool bViewWorld, bool bEditTrigger );

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	#pragma endregion function

	#pragma region public
public:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CMFCMenuBar				m_wndMenuBar;				/// �޴���
	CMFCToolBar				m_wndToolBar;				/// ����
	CMFCStatusBar			m_wndStatusBar;				/// ���¹�
	CMFCToolBarImages		m_UserImages;				/// ���� �̹���
	CClassView				m_wndClassView;				/// �� Ŭ����( ���� ����Ʈ )
	CPropertiesWnd			m_wndProperties;			/// �Ӽ�â
	CTriggerPropertiesWnd	m_wndTriggerProperties;		/// Ʈ���� �Ӽ�â
	CMFCShellTreeCtrl		m_wndTree;					/// Ʈ�� ��Ʈ��
	CDungeonInfo			m_wndDungeonInfo;			/// ���� ����
	CCameraInfo				m_wndCameraInfo;			/// ī�޶� ����
	CLineStartPositionInfo	m_wndStartPosition;			/// ��������
	CLineBaseEditor			m_DlgLineBaseEditor;		/// ���� ������
	CEditWorld				m_DlgWorldEditor;			/// ���� ������
	CEditDungeon			m_DlgDungeonEditor;			/// ���� ������
	CSelectToolUnit			m_DlgSelectUnit;			/// ���� ����

	CTriggerEdit			m_DlgTriggerEdit;			/// ������� ���� Ʈ���� ����â
	#pragma endregion variable �پ��� ��Ʈ�ѵ�

	#pragma region protected
protected:
	#pragma endregion variable
	
	#pragma region afx_msg
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

public:	
	afx_msg void OnEditLinemap();
	afx_msg void OnUpdateEditLinemap(CCmdUI *pCmdUI);
	afx_msg void OnEditWorld();
	afx_msg void OnUpdateEditWorld(CCmdUI *pCmdUI);
	afx_msg void OnViewLine();
	afx_msg void OnUpdateViewLine(CCmdUI *pCmdUI);
	afx_msg void OnViewLineInfo();
	afx_msg void OnUpdateViewLineInfo(CCmdUI *pCmdUI);
	afx_msg void OnViewWorld();
	afx_msg void OnUpdateViewWorld(CCmdUI *pCmdUI);
	afx_msg void OnDropFiles( HDROP hDropInfo );
	afx_msg void OnWorldObjectMove();
	afx_msg void OnWorldobjectRotation();
	afx_msg void OnWorldobjectScale();
	afx_msg void OnViewWorldObject();
	afx_msg void OnUpdateViewWorldobject(CCmdUI *pCmdUI);
	void OnViewWorldObjectSelected();
	afx_msg void OnEditDungeon();
	afx_msg void OnViewLineNumber();
	afx_msg void OnViewLineSetNumber();
	afx_msg void OnViewLineCameraNumber();
	afx_msg void OnViewLineTriggerNumber();
	afx_msg void OnViewWaterInfo();
	afx_msg void OnTriggerBck();
	#pragma endregion ������ �޽��� �� �Լ�
};
#pragma endregion ���� ������