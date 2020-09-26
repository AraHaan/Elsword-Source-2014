#pragma once
#include "afxwin.h"


// CUncompressDlg ��ȭ �����Դϴ�.

class CUncompressDlg : public CDialog
{
	DECLARE_DYNAMIC(CUncompressDlg)

public:
	CUncompressDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUncompressDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_UNCOM_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	std::vector< std::wstring > m_vecSelectedFileList;		// ���������� ���� ���õ� ���� ����Ʈ
	std::map< std::wstring, std::wstring > m_mapKomList;
	UNCOMPRESS_MODE m_eMode;
	CListBox m_komList;
	afx_msg void OnBnClickedFile();		// ���õ� ���ҽ����ϸ� ��������
	afx_msg void OnBnClickedKom();		// ���õ� kom ���ϸ� ��������
	afx_msg void OnBnClickedAllKom();	// ���kom ���� ��������(kom ���Ϻ� ���� ����)
	afx_msg void OnBnClickedOneKom();	// ���kom ���� ��������(���� �ϳ��� ��� ����)

public:
	UNCOMPRESS_MODE GetCompressMode() { return m_eMode; }
	std::vector< std::wstring >& GetSeletedFileList() { return m_vecSelectedFileList; }
	void SetKomList( std::map< std::wstring, std::wstring >& mapKomList ) { m_mapKomList = mapKomList; }
};
