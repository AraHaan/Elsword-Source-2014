#pragma once


// CGraph ��ȭ �����Դϴ�.
#include "Image.h"
#include "2DPushGraph.h"

class CGraph : public CDialog
{
	DECLARE_DYNAMIC(CGraph)

public:
	CGraph(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGraph();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

protected:	
	virtual BOOL OnInitDialog();
	

	DECLARE_MESSAGE_MAP()
private:
	//CImage m_image;
	C2DPushGraph m_PushGraph;
public:
	afx_msg void OnPaint();	
	afx_msg void OnTimer(UINT nIDEvent);
};