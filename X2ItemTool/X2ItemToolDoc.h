// X2ItemToolDoc.h : CX2ItemToolDoc Ŭ������ �������̽�
//


#pragma once

class CX2ItemToolDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CX2ItemToolDoc();
	DECLARE_DYNCREATE(CX2ItemToolDoc)

// Ư��
public:

// �۾�
public:

// ������
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ����
public:
	virtual ~CX2ItemToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()

	// OLE ����ġ �� �Լ��� �����߽��ϴ�.

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


