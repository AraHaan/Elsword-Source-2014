// X2EncryptFileViewerDoc.h : CX2EncryptFileViewerDoc Ŭ������ �������̽�
//


#pragma once

class CX2EncryptFileViewerDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CX2EncryptFileViewerDoc();
	DECLARE_DYNCREATE(CX2EncryptFileViewerDoc)

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
	virtual ~CX2EncryptFileViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};


