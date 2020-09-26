#include "StdAfx.h"
#include "x2book.h"

CX2Book::CX2Book() :
m_bShow(false),
m_pDLGBook(NULL),
m_pDLGBookCoverDefault(NULL),
m_iCurrentPage(0)
//m_mapPageData;
{
    
}

CX2Book::CX2Book( const char* bookTableName )
{
	CX2Book::CX2Book();
	SetBookTable( bookTableName );
	m_bShow = false;
}

CX2Book::~CX2Book(void)
{
	SAFE_DELETE_DIALOG(m_pDLGBook);
	SAFE_DELETE_DIALOG(m_pDLGBookCoverDefault);
	m_mapPageData.clear();
}

bool CX2Book::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case BUCM_EXIT:
		{
			SetShow(false);
			return true;
		} break;
	case BUCM_PREV_PAGE:
		{
			PrevPage();
			return true;
		} break;
	case BUCM_NEXT_PAGE:
		{
			NextPage();
			return true;
		} break;
	case BUCM_GOTO_PAGE:
		{
			CKTDGUIButton* pButton = (CKTDGUIButton*) lParam;
			int iTargetPage = pButton->GetDummyInt(0);
			GotoPage(iTargetPage);
			return true;
		} break;
	default:
		{
		} break;
	}
	return false;
}

bool CX2Book::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_F12) == TRUE )
	{
		SetShow(false);
	}
	return false;
}

bool CX2Book::OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return false;
}

void CX2Book::SetShow(bool bOpen )
{
	m_bShow = bOpen;
	if(NULL != m_pDLGBook)
	{
		m_pDLGBook->SetShow(m_bShow);
	}
	if(NULL != m_pDLGBookCoverDefault)
	{
		m_pDLGBookCoverDefault->SetShow(m_bShow);
	}

	if( bOpen )
	{
		if(false == m_mapPageData.empty())
		{
			PageDataIterator it = m_mapPageData.begin();
			GotoPage( it->first );
		}
	}
	else
	{

	}

}

bool CX2Book::SetBookTable( const char* bookTableName )
{
	// Show ���ζ��� �������, å�� �о���� ù �������� ����δ� ����.

	KLuaManager kLuamanager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );

    if ( g_pKTDXApp->LoadAndDoMemory( &kLuamanager, L"Book_Table.lua" ) == false )
    {
		// �����޽����� ���� �Ŷ� �̹��ϰ� �ٲ�θ� ���еǰ���? (.. )
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_3 ), g_pMain->GetNowState() );
		return false;
    }

	wstring wstrDefaultBookCoverFileName;
	if ( kLuamanager.BeginTable( bookTableName ) == true )
	{
		// �ش� å�̸� ���̺��� �ִ�. ������ ������.
		m_mapPageData.clear();
		
		LUA_GET_VALUE( kLuamanager, "DEFAULT_COVER", wstrDefaultBookCoverFileName, L"" );

		if( kLuamanager.BeginTable( "PAGE_TABLE" ) == true )
		{
			int index = 1;
			while( kLuamanager.BeginTable( index ) == true )
			{
				int iPageNum;
				PageData data;
				
				// PageData�� ���⼭ �о�´�.
				LUA_GET_VALUE( kLuamanager, 1,	iPageNum, -1 );
				LUA_GET_VALUE( kLuamanager, 2,	data.m_PageLuaFileName, L"" );
							
				kLuamanager.EndTable(); // index
				m_mapPageData.insert(std::make_pair(iPageNum, data));
				index++;
			}
			kLuamanager.EndTable();		// PageTable
		}

		kLuamanager.EndTable();			// BookTable
	}
	else
	{
		g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_STRING( STR_ID_4 ), g_pMain->GetNowState() );
		return false;
	}

	// �����ߴ�! or �������� ����
	if(m_mapPageData.empty())
	{
		return false;
	}
	
	// å ���̺� �ε� ����
	// ù ������ ����
	PageDataIterator it = m_mapPageData.begin();
	m_iCurrentPage = it->first;

	// �⺻ ��Ŀ�� ����
	m_pDLGBookCoverDefault = new CKTDGUIDialog( g_pMain->GetNowState(), wstrDefaultBookCoverFileName.c_str() );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGBookCoverDefault );	
	//m_pDLGBookCoverDefault->SetDisableUnderWindow(true);
	m_pDLGBookCoverDefault->SetShow(m_bShow);

	return true;
}

void CX2Book::NextPage()
{
	PageDataIterator it = m_mapPageData.upper_bound(m_iCurrentPage);
	if( it != m_mapPageData.end() )
	{
		GotoPage(it->first);
	}
// 	if( it != m_mapPageData.end() )
// 	{
// 		advance(it, 1);
// 		if( it != m_mapPageData.end() )
// 		{
// 			GotoPage(it->first);
// 		}
// 		
// 	}
	
}
void CX2Book::PrevPage()
{
	PageDataIterator it = m_mapPageData.lower_bound(m_iCurrentPage);
	if( it != m_mapPageData.begin() )
	{
		--it;
		GotoPage(it->first);
	}

// 	std::map<int, PageData>::reverse_iterator it( m_mapPageData.find(m_iCurrentPage) );
// 	if( it != m_mapPageData.rend() )
// 	{
// 		advance(it, 1);
// 		if( it != m_mapPageData.rend() )
// 		{
// 			GotoPage(it->first);
// 		}
// 		
// 	}
}

void CX2Book::GotoPage(int iTargetPage)
{
	// ������ġ�� �° �ɾ���°��� ����-_-
	if(m_mapPageData.empty())
		return;

	// iTargetPage�� �´� �������� �˻��Ѵ�.
	PageDataIterator it = m_mapPageData.find(iTargetPage);

	if(it == m_mapPageData.end())
	{
		// ã�� �������� ������ ���. ��� ���ѿ����� �����޽��� �˾�..
		if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
		{		
			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), GET_REPLACED_STRING( ( STR_ID_5, "i", iTargetPage ) ), g_pMain->GetNowState() );
		}
		return;
	}
	
	// ���� ���⼭ ���� it�� valid �ϴٰ� �����Ѵ� (empty���� �����Ƿ�)
	PageData& refPageData = it->second;

	// ������ �ִ� �������� �������ְ�
	if(m_pDLGBook != NULL)
	{
		g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGBook, NULL, false );
	}

	// ���ο� �������� ����
	m_pDLGBook = new CKTDGUIDialog( g_pMain->GetNowState(), refPageData.m_PageLuaFileName.c_str() );
	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGBook );

	m_iCurrentPage = iTargetPage;

}
