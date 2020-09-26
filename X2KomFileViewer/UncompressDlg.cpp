// UncompressDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "X2KomFileViewer.h"
#include "X2KomFileManager.h"
#include "UncompressDlg.h"
#include ".\uncompressdlg.h"


// CUncompressDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUncompressDlg, CDialog)
CUncompressDlg::CUncompressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUncompressDlg::IDD, pParent)
{
}

CUncompressDlg::~CUncompressDlg()
{
}

void CUncompressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_KOM_LIST, m_komList);
}


BEGIN_MESSAGE_MAP(CUncompressDlg, CDialog)
	ON_BN_CLICKED(IDSEL_FILE, OnBnClickedFile)
	ON_BN_CLICKED(IDSEL_KOM, OnBnClickedKom)
	ON_BN_CLICKED(ID_ALL_KOM, OnBnClickedAllKom)
	ON_BN_CLICKED(ID_ONE_KOM, OnBnClickedOneKom)
END_MESSAGE_MAP()


// CUncompressDlg �޽��� ó�����Դϴ�.
BOOL CUncompressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ʱ�ȭ
	std::map< std::wstring, std::wstring >::const_iterator mit;
	for( mit = m_mapKomList.begin(); mit != m_mapKomList.end(); ++mit )
	{
		m_komList.InsertString( 0, mit->first.c_str() );
	}

	return TRUE;
}
void CUncompressDlg::OnBnClickedFile()	// ���õ� ���ҽ����ϸ� ��������
{
	m_eMode = UM_SEL_FILE;
	EndDialog( 0 );
}

void CUncompressDlg::OnBnClickedKom()
{	
	// ������ kom�� ����Ǯ��
	std::wstring wstrCurKomName;
	m_vecSelectedFileList.clear();
	int iSelCount = m_komList.GetSelCount();

	if( iSelCount == 0 )
	{
		MessageBox( L"���������� ������ �����ϼ���!" );
		return;
	}
	else
	{
		int* pArray = new int[iSelCount];
		m_komList.GetSelItems( iSelCount, pArray );		// pArray�� ���õ� ������ ��ȣ���� ����

		for( int iIndex = 0; iIndex < iSelCount; ++iIndex )
		{			
			CString strTemp;
			m_komList.GetText( pArray[iIndex], strTemp );
			
			std::wstring wstrTemp = strTemp.GetBuffer();
			m_vecSelectedFileList.push_back( wstrTemp );
		}
		delete []pArray;
	}

	m_eMode = UM_SEL_KOM;
	EndDialog( 0 );	
}

void CUncompressDlg::OnBnClickedAllKom()
{
	m_eMode = UM_ALL_KOM;
	EndDialog( 0 );	
}

void CUncompressDlg::OnBnClickedOneKom()
{
	m_eMode = UM_ONE_KOM;
	EndDialog( 0 );	
}
