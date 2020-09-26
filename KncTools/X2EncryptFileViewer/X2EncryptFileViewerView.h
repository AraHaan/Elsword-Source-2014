// X2EncryptFileViewerView.h : iCX2EncryptFileViewerView Ŭ������ �������̽�
//


#pragma once


class CX2EncryptFileViewerView : public CEditView
{
protected: // serialization������ ��������ϴ�.
	CX2EncryptFileViewerView();
	DECLARE_DYNCREATE(CX2EncryptFileViewerView)

// Ư��
public:
	CX2EncryptFileViewerDoc* GetDocument() const;

// �۾�
public:

// ������
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate();
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDropFiles( HDROP hDropInfo );

	bool BrowseFolder( HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR* szFolder );

// ����
public:
	virtual ~CX2EncryptFileViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	char* m_cBufPointer;

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDecryptSave();
};

#ifndef _DEBUG  // X2EncryptFileViewerView.cpp�� ����� ����
inline CX2EncryptFileViewerDoc* CX2EncryptFileViewerView::GetDocument() const
   { return reinterpret_cast<CX2EncryptFileViewerDoc*>(m_pDocument); }
#endif

