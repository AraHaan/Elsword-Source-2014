// X2ItemToolView.h : iCX2ItemToolView Ŭ������ �������̽�
//


#pragma once

#include "MainDlg.h"


class CX2ItemToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CX2ItemToolView();
	DECLARE_DYNCREATE(CX2ItemToolView)

// Ư��
public:
	CX2ItemToolDoc* GetDocument() const;

// �۾�
public:

// ������
	public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ����
public:
	virtual ~CX2ItemToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);




protected:
	CMainDlg		m_MainDlg;

public:
	afx_msg void OnFileOpen();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // X2ItemToolView.cpp�� ����� ����
inline CX2ItemToolDoc* CX2ItemToolView::GetDocument() const
   { return reinterpret_cast<CX2ItemToolDoc*>(m_pDocument); }
#endif

