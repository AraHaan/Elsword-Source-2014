
#include "stdafx.h"
#include "UserProperty.h"
#include "afxcontrolbarutil.h"
#include "afxshellmanager.h"

#define FOCUS_CHECK_EVENT_ID		0x850408
#define FOCUS_CHECK_TIME			100
#define USER_SLIDER_PROPERTY_ID		0x850409

// �����̴� �˾�
class CUserSliderPopup	:	public CSliderCtrl
{
public:
	DECLARE_DYNAMIC( CUserSliderPopup )

	CUserSliderPopup();
	~CUserSliderPopup();

	//	���� �����մϴ�.
	void UpdateValue();

protected:
	//	�޽��� ����
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

	//	�޽��� ó����
	BOOL PreTranslateMessage( MSG* pMsg );

	//	�˾�â�� ���� �մϴ�.
	void ClosePopup();

protected:
	friend class CUserPropertyGridSlider;

	//	������Ƽ
	CUserPropertyGridSlider*	m_pProperty;
}; //class CUserSliderPopup

IMPLEMENT_DYNAMIC( CUserSliderPopup, CSliderCtrl )

//{{AFX_MSG_MAP( CUserSliderPopup )
BEGIN_MESSAGE_MAP( CUserSliderPopup, CSliderCtrl )
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

CUserSliderPopup::CUserSliderPopup()
:	m_pProperty		( NULL )
{
} //CUserSliderPopup::CUserSliderPopup

CUserSliderPopup::~CUserSliderPopup()
{
} //CUserSliderPopup::~CUserSliderPopup

//	�޽��� ó����
BOOL CUserSliderPopup::PreTranslateMessage( MSG* pMsg )
{
	switch( pMsg->message )
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		//	���콺 ��ư Ŭ��
		{
			CRect rect;
			GetClientRect(rect);

			//	���콺�� Ŭ�� ��ġ
			CPoint pt( AFX_GET_X_LPARAM( pMsg->lParam ), AFX_GET_Y_LPARAM( pMsg->lParam ) );

			if( !rect.PtInRect( pt ) )
			{	//	������ ���� �����ϴ�.
				ClosePopup();
				return TRUE;
			} //if
		} // ~���콺 ��ư Ŭ��
		break;
	case WM_KILLFOCUS:
	case WM_SYSKEYDOWN:
	case WM_CONTEXTMENU:
		ClosePopup();
		return TRUE;
	case WM_TIMER:
		{
			KillTimer( FOCUS_CHECK_EVENT_ID );

			//	���� Ȱ��ȭ �� ������
			CWnd* pFocusWnd = GetFocus();

			if( pFocusWnd && m_pProperty )
			{
				if( pFocusWnd != this && !m_pProperty->IsEnableEdit() )
				{	//	�θ� ������� ���簡 ��Ŀ���� ������ ���� �ʴٸ� ���� �մϴ�.
					ClosePopup();
				} 
				else
				{	//	��Ŀ���� �ִٸ� Ư�� �ð� ���� �˻��մϴ�.
					SetTimer( FOCUS_CHECK_EVENT_ID, FOCUS_CHECK_TIME, NULL );
				} //if..else..			
			} 
			else
			{	//	��Ŀ���� ���ٸ� ���� �մϴ�.
				ClosePopup();
			} //if..else..

			return TRUE;
		}
		break;
	case WM_KEYDOWN:
		if( pMsg->wParam == VK_ESCAPE )
		{	//	���� ��� ó���� �մϴ�.
			ClosePopup();
			return TRUE;
		} //if
		break;
	}; //switch

	return CSliderCtrl::PreTranslateMessage( pMsg );
} //CUserSliderPopup::PreTranslateMessage

//	�˾�â�� ���� �մϴ�.
void CUserSliderPopup::ClosePopup()
{	//	���� ó��
	OnDestroy();

	//	�ڽ��� ����ϴ�.
	//	�̰����� �ٸ� ���� ������ �ʾƵ� �ڵ������� �޸𸮰� ���� �ǵ��� �մϴ�.
	delete this;
} //CUserSliderPopup::ClosePopup

int CUserSliderPopup::OnCreate( LPCREATESTRUCT lpCreateStruct )
{	
	if( CSliderCtrl::OnCreate( lpCreateStruct ) == -1 )
	{	//	���� ����
		return FALSE;
	} //if

	return TRUE;
} //CUserSliderPopup::OnCreate

void CUserSliderPopup::OnDestroy()
{
	if( m_pProperty )
	{
		ShowWindow( FALSE );
		m_pProperty->SetPopup( NULL );
		m_pProperty = NULL;
	} //if

	CSliderCtrl::OnDestroy();
} //CUserSliderPopup::OnDestroy

//	���� �����մϴ�.
void CUserSliderPopup::UpdateValue()
{
	if( NULL == m_pProperty )	return ;
	
	//	���� ���� �����մϴ�.
	m_pProperty->m_nValue = GetPos();

	//	�ε� �Ҽ����� ��� Ư�� ó���� �մϴ�.
	if( m_pProperty->m_bFloat )
	{		
		int nMin = m_pProperty->m_nMin;
		int nMax = m_pProperty->m_nMax;

		//	�Ÿ�
		int nDist = nMax - nMin;

		//	�����°��� �����մϴ�.
		float fOffset = ( GetPos() - nMin ) / (float)nDist;

		//	���� ���� �����մϴ�.
		m_pProperty->m_fValue = fOffset * m_pProperty->m_fOffsetValue;
		m_pProperty->m_fValue += m_pProperty->m_fAddValue;

		//	ȭ�鿡 ����� ���� �����մϴ�.
		m_pProperty->SetValue( (float)m_pProperty->m_fValue );
	} 
	else
	{
		m_pProperty->SetValue( (long)GetPos() );
	} //if

	//	�׸���
	CMFCPropertyGridCtrl* pGrid = m_pProperty->m_pWndList;

	if( pGrid )
	{	//	���� ����Ǿ��ٰ� �����մϴ�.
		pGrid->OnPropertyChanged( m_pProperty );
	} //if
} //CUserSliderPopup::UpdateValue

CUserPropertyGridSlider::CUserPropertyGridSlider( CWnd* pWnd, const CString& strName, int& varValue, DWORD_PTR dwData )
:	CMFCPropertyGridProperty( strName, (_variant_t)(long)varValue, NULL, dwData )
{
	m_pPopup = NULL;
	m_pGridCtrl = pWnd;

	//	����
	m_nMin = 0;
	m_nMax = 100;
	m_nValue = varValue;
	m_fValue = 0.0F;
	m_nFloatValue = -1.f;
	m_fAddValue = 0.0F;
	m_fOffsetValue = 0.0F;
	m_bFloat = 0;
} //CUserPropertyGridSlider::CUserPropertyGridSlider
CUserPropertyGridSlider::CUserPropertyGridSlider( CWnd* pWnd, const CString& strName, float& varValue, DWORD_PTR dwData )
	:	CMFCPropertyGridProperty( strName, (_variant_t)(float)varValue, NULL, dwData )
{
	m_pPopup = NULL;
	m_pGridCtrl = pWnd;

	//	����
	m_nMin = 0;
	m_nMax = 100;
	m_nValue = 0;
	m_fValue = 0.0F;
	m_nFloatValue = varValue;
	m_fAddValue = 0.0F;
	m_fOffsetValue = 0.0F;
	m_bFloat = 0;
		
} //CUserPropertyGridSlider::CUserPropertyGridSlider

CUserPropertyGridSlider::~CUserPropertyGridSlider()
{
	SAFE_DELETE( m_pPopup );
} //CUserPropertyGridSlider::~CUserPropertyGridSlider

//	���� �����մϴ�.
BOOL CUserPropertyGridSlider::OnEdit( LPPOINT lptClick )
{
	if( NULL == m_pPopup )
	{	//	�����̴� ��Ʈ���� �߰��մϴ�.
		m_pPopup = new CUserSliderPopup;
		m_pPopup->Create( TBS_BOTH | TBS_NOTICKS, m_Rect, m_pGridCtrl, USER_SLIDER_PROPERTY_ID );
		m_pPopup->m_pProperty = this;
	} //if

	//	������Ƽ�� �Ӽ��� �����մϴ�.
	m_pPopup->SetRange( m_nMin, m_nMax );
	m_pPopup->SetPos( m_nValue );

	//	���� �÷��� ���̸� ���մϴ�.
	int nAddX = m_pWndList->GetLeftColumnWidth();

	//	��Ȯ�ϰ� Valueâ�� �� �κп� ��� ��Ű�� ���� ���� ���� ���մϴ�.
	int nParentWidth = m_pWndList->GetListRect().Width();

	//	������Ƽ�׸����� ���� ���̿� ������Ƽ�� ���� ������ ���� �߻��� �� �ֱ⿡
	//	�̰��� ���մϴ�.
	int nPropertyWidth = nParentWidth - m_Rect.Width();

	//	Valueâ�� ũ�⿡ �µ��� �����մϴ�. �ǵ��� ���̴� ������ ������ �����մϴ�.
	m_pPopup->MoveWindow( m_Rect.left + nAddX - nPropertyWidth, m_Rect.top + m_Rect.Height(), m_Rect.Width() - nAddX + nPropertyWidth, 25 );
	m_pPopup->ShowWindow( TRUE );

	//	������Ƽ�� �ٽ� �׸��ϴ�.
	Redraw();

	return CMFCPropertyGridProperty::OnEdit( lptClick );
} //CUserPropertyGridSlider::OnEdit

BOOL CUserPropertyGridSlider::OnEndEdit()
{
	if( m_pPopup && ::GetFocus() != m_pPopup->GetSafeHwnd() )
	{
		m_pPopup->KillTimer( FOCUS_CHECK_EVENT_ID );
		m_pPopup->SetTimer( FOCUS_CHECK_EVENT_ID, 1, NULL );
	} //if

	return CMFCPropertyGridProperty::OnEndEdit();
} //CUserPropertyGridSlider::OnEndEdit

//	�����̴� �˾� ����
void CUserPropertyGridSlider::SetPopup( CUserSliderPopup* pPopup )
{	
	m_pPopup = pPopup;
} //CUserPropertyGridSlider::SetPopup

//	����Ʈ â�� Ȱ��ȭ �Ǿ� �ִ����� ����
BOOL CUserPropertyGridSlider::IsEnableEdit()
{
	if( m_pWndInPlace )	return TRUE;
	return FALSE;
} //CUserPropertyGridSlider::IsEnableEdit

//	�ּ�ġ�� �ִ�ġ
void CUserPropertyGridSlider::SetRange( int nMin, int nMax )
{
	m_nMin = nMin;
	m_nMax = nMax;
	//m_nValue = nMin;	
	m_fValue = 0.0F;
	m_bFloat = FALSE;
	
} //CUserPropertyGridSlider::SetRange

//	�ε� �Ҽ���
//	����� �� �ε� �Ҽ������� �����մϴ�.
//	SetRange�� ���Ե� ���� 0.0~1.0���� �����ϰ�
//	SetFloatValue�� ���Ե� ���� �����Ͽ� ���� ���� �����մϴ�.
//	���� ���ڴ� ���� ���� ���� ���Դϴ�.
void CUserPropertyGridSlider::SetFloatValue( float fValue, float fAdd )
{
	m_fOffsetValue = fValue;
	m_fAddValue = fAdd;
	m_fValue = fAdd;
	m_bFloat = TRUE;

	m_nValue = (m_nMax - m_nMin) * static_cast<int>( (m_nFloatValue - m_fAddValue) / m_fOffsetValue ) + m_nMin;
} //CUserPropertyGridSlider::SetFloatValue

void CUserPropertyGridSlider::UpdateSlider()
{
	if( m_nFloatValue < 0.f )
	{
		m_nValue = (int)GetValue().intVal;
	}
	else
	{
		float fTemp = (float)GetValue().fltVal;
		m_nValue = (m_nMax - m_nMin) * static_cast<int>( (fTemp - m_fAddValue) / m_fOffsetValue) + m_nMin;
	}
}

IMPLEMENT_DYNAMIC(CUserPropertyGrid, CMFCPropertyGridCtrl)

//{{AFX_MSG_MAP(CMFCPropertyGridCtrl)
BEGIN_MESSAGE_MAP(CUserPropertyGrid, CMFCPropertyGridCtrl)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CUserPropertyGrid::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{	//	���ҽ� ���̵� ��� �ɴϴ�.
	int nID = pScrollBar->GetDlgCtrlID();

	if( nID == USER_SLIDER_PROPERTY_ID )
	{	//	���� �����̴��Դϴ�.
		CUserSliderPopup* pUserSlider = (CUserSliderPopup*)pScrollBar;
		pUserSlider->UpdateValue();
	} //if

	CMFCPropertyGridCtrl::OnHScroll( nSBCode, nPos, pScrollBar );
} //CUserPropertyGrid::OnHScroll


void CUserPropertyGridFile::OnClickButton(CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	m_bButtonIsDown = TRUE;
	Redraw();

	CString strPath = m_varValue.bstrVal;
	BOOL bUpdate = FALSE;

	
	CFileDialog dlg(m_bOpenFileDialog, m_strDefExt, strPath, m_dwFileOpenFlags, m_strFilter, m_pWndList);
	if (dlg.DoModal() == IDOK)
	{
		bUpdate = TRUE;
		strPath = dlg.GetPathName();
	}

	if (bUpdate)
	{
		CString cstrFileName = strPath;
		wstring wstrFileName = strPath;
		WCHAR wszFilePathOnly[512] = {NULL};
		WCHAR wszFileNameOnly[256] = {NULL};
		DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, wstrFileName.c_str() );

		if (m_pWndInPlace != NULL)
		{
			m_pWndInPlace->SetWindowText(wszFileNameOnly);
		}

		cstrFileName = wszFileNameOnly;
		m_varValue = (LPCTSTR) cstrFileName;
	}

	m_bButtonIsDown = FALSE;
	Redraw();

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus();
	}
	else
	{
		m_pWndList->SetFocus();
	}
}