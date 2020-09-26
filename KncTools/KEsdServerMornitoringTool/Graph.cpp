// Graph.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KEsdRemoteController_Console.h"
#include "Graph.h"
#include ".\graph.h"

#define PUSHGRAPH_SYSTEM_LINE    0
#define PUSHGRAPH_PROCESS_LINE   1
#define PUSHGRAPH_EX_LINE		 2
// CGraph ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CGraph, CDialog)
CGraph::CGraph(CWnd* pParent /*=NULL*/)
	: CDialog(CGraph::IDD, pParent)	
{
}

CGraph::~CGraph()
{
}

void CGraph::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGraph, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGraph �޽��� ó�����Դϴ�.
BOOL CGraph::OnInitDialog()
{
	m_PushGraph.CreateFromStatic(IDC_STATICGRAPH, this);
	m_PushGraph.ModifyStyle(0, WS_THICKFRAME);

	m_PushGraph.AddLine( PUSHGRAPH_SYSTEM_LINE,  RGB(0, 255, 0));
	m_PushGraph.AddLine( PUSHGRAPH_PROCESS_LINE, RGB(255, 255, 0));
	m_PushGraph.AddLine( PUSHGRAPH_EX_LINE, RGB(255, 0, 255));

	SetTimer( 1, 100, NULL );

	return CDialog::OnInitDialog();
}

void CGraph::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	/*int aData[1440] = {0, 10, 20, 3, 40};
	m_image.CreateImage_ForDay( dc.m_hDC, "��", aData, 150 );*/

	// �׸��� �޽����� ���ؼ��� CDialog::OnPaint()��(��) ȣ������ ���ʽÿ�.
}

void CGraph::OnTimer(UINT nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	static int dt, dt2, dt3;

	dt+=10;
	dt2+=20;
	dt3+=15;

	m_PushGraph.Push(dt, PUSHGRAPH_SYSTEM_LINE);
	m_PushGraph.Push(dt2, PUSHGRAPH_PROCESS_LINE);
	m_PushGraph.Push(dt3, PUSHGRAPH_EX_LINE);
	m_PushGraph.Update();

	CDialog::OnTimer(nIDEvent);
}