// X2EncryptFileViewerDoc.cpp : CX2EncryptFileViewerDoc Ŭ������ ����
//

#include "stdafx.h"
#include "X2EncryptFileViewer.h"

#include "X2EncryptFileViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CX2EncryptFileViewerDoc

IMPLEMENT_DYNCREATE(CX2EncryptFileViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CX2EncryptFileViewerDoc, CDocument)
END_MESSAGE_MAP()


// CX2EncryptFileViewerDoc ����/�Ҹ�

CX2EncryptFileViewerDoc::CX2EncryptFileViewerDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CX2EncryptFileViewerDoc::~CX2EncryptFileViewerDoc()
{
}

BOOL CX2EncryptFileViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	reinterpret_cast<CEditView*>(m_viewList.GetHead())->SetWindowText(NULL);

	// TODO: ���⿡ �ٽ� �ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CX2EncryptFileViewerDoc serialization

void CX2EncryptFileViewerDoc::Serialize(CArchive& ar)
{
	// CEditView���� ��� serialization�� ó���ϴ� edit ��Ʈ���� ��� �ֽ��ϴ�.
	reinterpret_cast<CEditView*>(m_viewList.GetHead())->SerializeRaw(ar);
}


// CX2EncryptFileViewerDoc ����

#ifdef _DEBUG
void CX2EncryptFileViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CX2EncryptFileViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CX2EncryptFileViewerDoc ���
