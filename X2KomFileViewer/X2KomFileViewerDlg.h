// X2KomFileViewerDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CX2KomFileViewerDlg ��ȭ ����
class CX2KomFileViewerDlg : public CDialog
{
// ����
public:
	CX2KomFileViewerDlg(CWnd* pParent = NULL);	// ǥ�� ������

// ��ȭ ���� ������
	enum { IDD = IDD_X2KOMFILEVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ����


// ����
protected:
	HICON m_hIcon;

	// �޽��� �� �Լ��� �����߽��ϴ�.
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	bool m_bIsKomParsingOnly;
	void InitListControl();
	void InsertKomList( IN const std::wstring& wstrKomName, IN const std::map< std::wstring, std::vector< MemberFileInfo > >& mapMemberFile );
	void InsertMemberList( IN const std::wstring& wstrKomName, IN const std::vector< MemberFileInfo >& vecMemberFileList );
	void InsertInsertedFileList( IN const std::vector< MemberFileInfo >& vecMemberFileList );
	void InsertDeletedFileList( IN std::vector< MemberFileInfo >& vecMemberFileList );
	bool BrowseFolder( HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR* szFolder );

public:	
	CImageList m_imglist;
	CListCtrl m_memberList;
	CListCtrl m_insertedListCtrl;
	CListCtrl m_deletedListCtrl;
	CEdit m_memberCount;
	void SetKomParsingOnly( bool bIsKomParsingOnly ) { m_bIsKomParsingOnly = bIsKomParsingOnly;	}
	afx_msg void OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_komSizeEdit;
	afx_msg void OnBnClickedCreateKom();	// �����ϱ�
	CListCtrl m_komList;
	afx_msg void OnLvnItemActivateKomList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();		// ���Ͽ���
	afx_msg void OnBnClickedFindFile();		// ����ã��
	CEdit m_editFileFind;
	afx_msg void OnBnClickedUncompress();	// ����Ǯ��
	CButton m_checkMT;
	afx_msg void OnBnClickedModifyKom();	// ���ҽ� ���Ƴ����
};
