// X2ItemToolDoc.cpp : CX2ItemToolDoc Ŭ������ ����
//

#include "stdafx.h"
#include "X2ItemTool.h"

#include "X2ItemToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CX2ItemToolDoc

IMPLEMENT_DYNCREATE(CX2ItemToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CX2ItemToolDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CX2ItemToolDoc, CDocument)
END_DISPATCH_MAP()

// ����: VBA������ ���� ���� ���ε��� �����ϱ� ���� IID_IX2ItemTool�� ���� ������ �߰��߽��ϴ�.
// �� IID�� .IDL ������ dispinterface�� ÷�ε� 
// GUID�� ��ġ�ؾ� �մϴ�.

// {551A743D-ABB1-4F0C-88D1-0E24EC0CAD00}
static const IID IID_IX2ItemTool =
{ 0x551A743D, 0xABB1, 0x4F0C, { 0x88, 0xD1, 0xE, 0x24, 0xEC, 0xC, 0xAD, 0x0 } };

BEGIN_INTERFACE_MAP(CX2ItemToolDoc, CDocument)
	INTERFACE_PART(CX2ItemToolDoc, IID_IX2ItemTool, Dispatch)
END_INTERFACE_MAP()


// CX2ItemToolDoc ����/�Ҹ�

CX2ItemToolDoc::CX2ItemToolDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

	EnableAutomation();

	AfxOleLockApp();
}

CX2ItemToolDoc::~CX2ItemToolDoc()
{
	AfxOleUnlockApp();
}

BOOL CX2ItemToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ �ٽ� �ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CX2ItemToolDoc serialization

void CX2ItemToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CX2ItemToolDoc ����

#ifdef _DEBUG
void CX2ItemToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CX2ItemToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CX2ItemToolDoc ���
