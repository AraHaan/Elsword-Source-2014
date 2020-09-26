//
// UserProperty.h	:	���� ������Ƽ Ŭ����
//

#pragma once

class CUserSliderPopup;

//	���� �׸��� �����̴�
class CUserPropertyGridSlider : public CMFCPropertyGridProperty
{
public:

	//	������
	CUserPropertyGridSlider( CWnd* pWnd, const CString& strName, int& varValue, DWORD_PTR dwData );
	CUserPropertyGridSlider( CWnd* pWnd, const CString& strName, float& varValue, DWORD_PTR dwData );
	~CUserPropertyGridSlider();

	//	���� ���Դϴ�.
	BOOL OnEdit(LPPOINT lptClick);

	//	������ ���� �Ǿ����ϴ�.
	BOOL OnEndEdit();

	//	�����̴� �˾� ����
	void SetPopup( CUserSliderPopup* pPopup );

	//	����Ʈ â�� Ȱ��ȭ �Ǿ� �ִ����� ����
	BOOL IsEnableEdit();

	//	�ּ�ġ�� �ִ�ġ
	void SetRange( int nMin, int nMax );

	//	�ε� �Ҽ���
	//	����� �� �ε� �Ҽ������� �����մϴ�.
	//	SetRange�� ���Ե� ���� 0.0~1.0���� �����ϰ�
	//	SetFloatValue�� ���Ե� ���� �����Ͽ� ���� ���� �����մϴ�.
	//	���� ���ڴ� ���� ���� ���� ���Դϴ�.
	void SetFloatValue( float fValue, float fAdd = 0.0F );	
	void UpdateSlider();

protected:
	friend class CUserSliderPopup;
	
	//	�����̴� �˾�
	CUserSliderPopup*	m_pPopup;
	CWnd*				m_pGridCtrl;

	//	����
	int					m_nMin;
	int					m_nMax;
	int					m_nValue;
	float				m_nFloatValue;

	float				m_fAddValue;
	float				m_fValue;
	float				m_fOffsetValue;
	
	//	�ε� �Ҽ������� ǥ�������� ����
	BOOL				m_bFloat;
}; //CUserPropertyGridSlider

//	������Ƽ �׸���
class CUserPropertyGrid	: public CMFCPropertyGridCtrl
{
public:
	DECLARE_DYNAMIC( CUserPropertyGrid )

	//	�����ڿ� ������
	CUserPropertyGrid()
	{
	}

	~CUserPropertyGrid()
	{
	} 
	
	void SetLeftColumnWidht(int iWidth) { m_nLeftColumnWidth = iWidth; }
protected:

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()
private:

}; //class CUserPropertyGrid


class CUserPropertyGridFile : public CMFCPropertyGridFileProperty
{
public:
	CUserPropertyGridFile( const CString& strName, BOOL bOpenFileDialog, const CString& strFileName, LPCTSTR lpszDefExt, DWORD dwFileFlags, LPCTSTR lpszFilter, LPCTSTR lpszDescr, DWORD_PTR dwData)
		: CMFCPropertyGridFileProperty(strName, m_bOpenFileDialog, strFileName, lpszDefExt, dwFileFlags, lpszFilter, lpszDescr, dwData)
	{
	}
	virtual ~CUserPropertyGridFile() {}

	virtual void OnClickButton(CPoint point);
};