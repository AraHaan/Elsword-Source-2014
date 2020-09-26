// KAccountDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KEsdRemoteController_Console.h"
#include "KAccountDlg.h"
#include ".\kaccountdlg.h"


// CKAccountDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CKAccountDlg, CDialog)
CKAccountDlg::CKAccountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKAccountDlg::IDD, pParent)
{
}

CKAccountDlg::~CKAccountDlg()
{
}

void CKAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NEW_ID_EDIT, m_newIdEdit);
	DDX_Control(pDX, IDC_NEW_PW_EDIT, m_newPwEdit);
	DDX_Control(pDX, IDC_RE_PW_EDIT, m_rePwEdit);
	DDX_Control(pDX, IDC_ACCOUNT_LIST, m_accList);
	DDX_Control(pDX, IDC_AUTH_COMBO, m_authLevel);
}

BOOL CKAccountDlg::OnInitDialog()
{
    // ����Ʈ �ڽ��� ���� ����Ʈ�� �߰�
	CListCtrl* pkListEsd = (CListCtrl*) GetDlgItem( IDC_ACCOUNT_LIST );		
	ListView_SetExtendedListViewStyle( pkListEsd->GetSafeHwnd(),
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 80;
		kCol.pszText = L"ID";

		pkListEsd->InsertColumn(0, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 90;
		kCol.pszText = L"PASSWORD";

		pkListEsd->InsertColumn(1, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 110;
		kCol.pszText = L"Description";

		pkListEsd->InsertColumn(2, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 85;
		kCol.pszText = L"Auth Level";

		pkListEsd->InsertColumn(3, &kCol);
	}
	
	int iIdx = pkListEsd->GetItemCount();

	vector<AccountList> acList;
	//theApp.m_kImp.DoQueryAccountList( acList );

	vector<AccountList>::iterator mit = acList.begin();
	for( ; mit != acList.end(); mit++ )
	{
		{
			LV_ITEM kItem;
			ZeroMemory( &kItem, sizeof(kItem) );
			kItem.mask = LVIF_TEXT; 
			kItem.iItem = iIdx;
			kItem.iSubItem = 0;
			kItem.pszText = const_cast<wchar_t*>(mit->m_id.c_str());
			;

			pkListEsd->InsertItem( &kItem );
		}

		{
			LV_ITEM kItem;
			ZeroMemory( &kItem, sizeof(kItem) );
			kItem.mask = LVIF_TEXT; 
			kItem.iItem = iIdx;
			kItem.iSubItem = 1;
			kItem.pszText = const_cast<wchar_t*>(mit->m_pw.c_str());
			;

			pkListEsd->SetItem( &kItem );
		}

		{
			LV_ITEM kItem;
			ZeroMemory( &kItem, sizeof(kItem) );
			kItem.mask = LVIF_TEXT; 
			kItem.iItem = iIdx;
			kItem.iSubItem = 2;
			kItem.pszText = const_cast<wchar_t*>(mit->m_description.c_str());

			pkListEsd->SetItem( &kItem );
		}

		{
			LV_ITEM kItem;
			ZeroMemory( &kItem, sizeof(kItem) );
			kItem.mask = LVIF_TEXT; 
			kItem.iItem = iIdx;
			kItem.iSubItem = 3;			
			kItem.pszText = const_cast<wchar_t*>( GetAuthString( mit->m_authLevel ) );

			pkListEsd->SetItem( &kItem );
		}
	}

	return CDialog::OnInitDialog();
}

void CKAccountDlg::UpdateAccountList()
{
	CListCtrl* pkListEsd = (CListCtrl*) GetDlgItem( IDC_ACCOUNT_LIST );
	pkListEsd->DeleteAllItems();
	int iIdx = pkListEsd->GetItemCount();

	CString temp;
	vector<AccountList> acList;
	//theApp.m_kImp.DoQueryAccountList( acList );

	vector<AccountList>::iterator mit = acList.begin();
	for( ; mit != acList.end(); mit++ )
	{
		{
			LV_ITEM kItem;
			ZeroMemory( &kItem, sizeof(kItem) );
			kItem.mask = LVIF_TEXT; 
			kItem.iItem = iIdx;
			kItem.iSubItem = 0;
			kItem.pszText = const_cast<wchar_t*>(mit->m_id.c_str());
			;

			pkListEsd->InsertItem( &kItem );
		}

		{
			LV_ITEM kItem;
			ZeroMemory( &kItem, sizeof(kItem) );
			kItem.mask = LVIF_TEXT; 
			kItem.iItem = iIdx;
			kItem.iSubItem = 1;
			kItem.pszText = const_cast<wchar_t*>(mit->m_pw.c_str());

			pkListEsd->SetItem( &kItem );
		}

		{
			LV_ITEM kItem;
			ZeroMemory( &kItem, sizeof(kItem) );
			kItem.mask = LVIF_TEXT; 
			kItem.iItem = iIdx;
			kItem.iSubItem = 2;
			kItem.pszText = const_cast<wchar_t*>(mit->m_description.c_str());

			pkListEsd->SetItem( &kItem );
		}

		{
			LV_ITEM kItem;
			ZeroMemory( &kItem, sizeof(kItem) );
			kItem.mask = LVIF_TEXT; 
			kItem.iItem = iIdx;
			kItem.iSubItem = 3;
			kItem.pszText = const_cast<wchar_t*>( GetAuthString( mit->m_authLevel ) );

			pkListEsd->SetItem( &kItem );
		}
	}
}


BEGIN_MESSAGE_MAP(CKAccountDlg, CDialog)
ON_BN_CLICKED(IDC_NEW_ACCOUNT, OnBnClickedNewAccount)
ON_BN_CLICKED(IDC_MODIFY, OnBnClickedModify)
ON_BN_CLICKED(IDC_DEL_ACCOUNT, OnBnClickedDelAccount)
ON_NOTIFY(LVN_ITEMACTIVATE, IDC_ACCOUNT_LIST, OnLvnItemActivateAccountList)
END_MESSAGE_MAP()


// CKAccountDlg �޽��� ó�����Դϴ�.
void CKAccountDlg::OnBnClickedNewAccount()
{
	CString newID, newPW, rePW, message;
	m_newIdEdit.GetWindowText( newID );
	m_newPwEdit.GetWindowText( newPW );
	m_rePwEdit.GetWindowText( rePW );
	int authIndex = m_authLevel.GetCurSel();

	if( newID.GetLength() > ID_SIZE )
	{
		message.Format( L"ID���̰� %d�� �̻� �Ѿ �� �����ϴ�!", ID_SIZE );
		MessageBox( message.GetBuffer(), L"�˸�", MB_OK );
		return;
	}

	if( newPW.GetLength() > PW_SIZE || rePW.GetLength() > PW_SIZE )
	{
		message.Format( L"��ȣ���̰� %d�� �̻� �Ѿ �� �����ϴ�!", PW_SIZE );
		MessageBox( message.GetBuffer(), L"�˸�", MB_OK );
		return;
	}

	if( newID.IsEmpty() )
	{
		MessageBox( L"ID�� �Է��ϼ���!", L"�˸�", MB_OK );
		return;
	}

	if( newPW.IsEmpty() || rePW.IsEmpty() )
	{
		MessageBox( L"��ȣ�� �Է��ϼ���!", L"�˸�", MB_OK );
		return;
	}

	if( newPW.Compare( rePW.GetBuffer() ) != 0 )
	{
		MessageBox( L"��ȣ�� �߸� �Է��Ͽ����ϴ�!", L"�˸�", MB_OK );
		return;
	}

	if( authIndex == -1 )
	{
		MessageBox( L"���������� ������ �ּ���!", L"�˸�", MB_OK );
		return;
	}


	//theApp.m_kImp.DoQueryNewAccount( newID.GetBuffer(), newPW.GetBuffer(), authIndex );
	UpdateAccountList();
}

void CKAccountDlg::OnBnClickedModify()
{
	CString newID, newPW, rePW, message;
	m_newIdEdit.GetWindowText( newID );
	m_newPwEdit.GetWindowText( newPW );
	m_rePwEdit.GetWindowText( rePW );
	int authIndex = m_authLevel.GetCurSel();

	if( newID.GetLength() > ID_SIZE )
	{
		message.Format( L"ID���̰� %d�� �̻� �Ѿ �� �����ϴ�!", ID_SIZE );
		MessageBox( message.GetBuffer(), L"�˸�", MB_OK );
		return;
	}

	if( newPW.GetLength() > PW_SIZE || rePW.GetLength() > PW_SIZE )
	{
		message.Format( L"��ȣ���̰� %d�� �̻� �Ѿ �� �����ϴ�!", PW_SIZE );
		MessageBox( message.GetBuffer(), L"�˸�", MB_OK );
		return;
	}

	if( newID.IsEmpty() )
	{
		MessageBox( L"ID�� �Է��ϼ���!", L"�˸�", MB_OK );
		return;
	}

	if( newPW.IsEmpty() || rePW.IsEmpty() )
	{
		MessageBox( L"��ȣ�� �Է��ϼ���!", L"�˸�", MB_OK );
		return;
	}

	if( newPW.Compare( rePW.GetBuffer() ) != 0 )
	{
		MessageBox( L"��ȣ�� �߸� �Է��Ͽ����ϴ�!", L"�˸�", MB_OK );
		return;
	}

	if( authIndex == -1 )
	{
		MessageBox( L"���������� ������ �ּ���!", L"�˸�", MB_OK );
		return;
	}

	//theApp.m_kImp.DoQueryModifyAccount( newID.GetBuffer(), newPW.GetBuffer(), authIndex );
	UpdateAccountList();
}

void CKAccountDlg::OnBnClickedDelAccount()
{
	CString newID, message;
	m_newIdEdit.GetWindowText( newID );

	if( newID.IsEmpty() )
	{
		MessageBox( L"������ ������ ������ �ּ���!", L"�˸�", MB_OK );
		return;
	}

	message.Format( L"[%s]������ ���� �����Ͻðڽ��ϱ�?", newID.GetBuffer() );
	if( MessageBox( message.GetBuffer(), L"�˸�", MB_YESNO ) == IDNO )
		return;

	//theApp.m_kImp.DoQueryDelAccount( newID.GetBuffer() );
	UpdateAccountList();
}

void CKAccountDlg::OnLvnItemActivateAccountList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.	
	CListCtrl* pkListEsd = (CListCtrl*) GetDlgItem( IDC_ACCOUNT_LIST );	
	CString id, pw;
	id = pkListEsd->GetItemText( pkListEsd->GetSelectionMark(), 0 );
	pw = pkListEsd->GetItemText( pkListEsd->GetSelectionMark(), 1 );
	int authLevel = theApp.m_kImp.GetAccountInfo( (pkListEsd->GetItemCount()-1) - pkListEsd->GetSelectionMark() ).m_authLevel;
	m_newIdEdit.SetWindowText( id.GetBuffer() );
	m_newPwEdit.SetWindowText( pw.GetBuffer() );
	m_rePwEdit.SetWindowText( L"" );
	m_authLevel.SetCurSel( authLevel );
	*pResult = 0;
}