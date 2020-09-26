#include "StdAfx.h"
#include ".\ktdguidialogmanager.h"


#define MA_

static inline int _CompareLayer( CKTDGUIDialogType pkIn1_, CKTDGUIDialogType pkIn2_ )
{
    ASSERT( pkIn1_ != NULL && pkIn2_ != NULL );
    if ( !pkIn1_->GetFront() )
    {
        if ( pkIn2_->GetFront() )
            return -1;
    }
    else
    {
        if ( !pkIn2_->GetFront() )
            return 1;
    }//if.. else..

    if ( pkIn1_->GetLayer() < pkIn2_->GetLayer() )
        return -1;
    else if ( pkIn1_->GetLayer() > pkIn2_->GetLayer() )
        return 1;
    return 0;
}


CKTDGUIDialogManager::CKTDGUIDialogManager(void) 
: m_iModalDialogCount( 0 )
, m_iDialogListChangeCount( 0 )
{
	m_bShow = true;

#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	m_mapFontToUKFont.clear();
#endif

#ifdef DIALOG_SHOW_TOGGLE
	m_bCanHide = false;
#endif
}

CKTDGUIDialogManager::~CKTDGUIDialogManager(void)
{
    ClearAllDlg();

#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	m_mapFontList.clear();
#endif
	m_mapUKFontList.clear();

#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	m_mapFontToUKFont.clear();
#endif

	RegisterLuaBind();

	CKTDGUIUniBuffer::Uninitialize();
	CKTDGUIIMEEditBox::Uninitialize();
}

void CKTDGUIDialogManager::RegisterLuaBind()
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pDialogManager", this );
}


void CKTDGUIDialogManager::_InsertIntoDialogListTop( CKTDGUIDialogType pkInDialog_ )
{
// �ܺο��� m_listDialog �� iteration �ϴ� �����̶� m_listDialog�� insert �ϴ� ���� �����ϴ�.

    ASSERT( pkInDialog_ != NULL && pkInDialog_->GetUIDialogManager() == this );

    CKTDGUIDialogList::iterator iter = m_listDialog.begin();
    while( iter != m_listDialog.end() )
    {
        ASSERT( *iter != NULL );
        ASSERT( *iter != pkInDialog_ );
        if ( _CompareLayer( pkInDialog_, (*iter) ) < 0 )
        {
			break;
        }//if
        iter++;
    }//for


    m_listDialog.insert( iter, pkInDialog_ );
	m_setDialog.insert( pkInDialog_ );
	ASSERT( m_listDialog.size() == m_setDialog.size() );

	IncreaseDialogListChangeCount();
	pkInDialog_->SetDialogListChangeCount( m_iDialogListChangeCount );

	CountModalDialog( pkInDialog_->Get() );

}//CKTDGUIDialogManager::_InsertIntoDialogList()


void CKTDGUIDialogManager::_InsertIntoDialogListBottom( CKTDGUIDialogType pkInDialog_ )
{
// �ܺο��� m_listDialog �� iteration �ϴ� �����̶� m_listDialog�� insert �ϴ� ���� �����ϴ�.

    ASSERT( pkInDialog_ != NULL && pkInDialog_->GetUIDialogManager() == this );

    CKTDGUIDialogList::iterator iter = m_listDialog.begin();
    while( iter != m_listDialog.end() )
    {
        ASSERT( *iter != NULL );
        ASSERT( *iter != pkInDialog_ );
        if ( _CompareLayer( pkInDialog_, (*iter) ) <= 0 )
        {
			break;
        }//if
        iter++;
    }//for

    m_listDialog.insert( iter, pkInDialog_ );
	m_setDialog.insert( pkInDialog_ );
	ASSERT( m_listDialog.size() == m_setDialog.size() );

	IncreaseDialogListChangeCount();
	pkInDialog_->SetDialogListChangeCount( -m_iDialogListChangeCount );

	CountModalDialog( pkInDialog_->Get() );


}//CKTDGUIDialogManager::_InsertIntoDialogList()


bool CKTDGUIDialogManager::_RemoveFromDialogList( CKTDGUIDialogType pkInDialog_ )
{
// �ܺο��� m_listDialog �� iteration �ϴ� ������ ��� m_vecIterator, m_vecReverseIterator ���� iterator���� �����ϰ� ������Ʈ�Ѵ�.

    ASSERT( pkInDialog_ != NULL && pkInDialog_->GetUIDialogManager() == this );

#ifdef _DEBUG // 2009-1-13
    CKTDGUIDialogList::reverse_iterator ritor = m_listDialog.rend();
    CKTDGUIDialogType  pritor = *ritor;
#endif // _DEBUG // 2009-1-13

    for( CKTDGUIDialogList::iterator iter = m_listDialog.begin();
        iter != m_listDialog.end(); iter++ )
    {
        ASSERT( *iter != NULL );
        if ( *iter == pkInDialog_ )
        {
            BOOST_TEST_FOREACH( CKTDGUIDialogList::iterator&, iterForward, m_vecIterator )
            {
                if ( iterForward != m_listDialog.end() && *iterForward == pkInDialog_ )
                    iterForward++;
            }//BOOST_TEST_FOREACH()

            BOOST_TEST_FOREACH( CKTDGUIDialogList::iterator&, iterReverse, m_vecReverseIterator )
            {
                if ( iterReverse != m_listDialog.end() && *iterReverse == pkInDialog_ )
                {
                    if ( iterReverse == m_listDialog.begin() )
                        iterReverse = m_listDialog.end();
                    else
                        iterReverse--;
                }//if
            }//BOOST_TEST_FOREACH()

            m_listDialog.erase( iter );
			m_setDialog.erase( pkInDialog_ );
			ASSERT( m_listDialog.size() == m_setDialog.size() );

			IncreaseDialogListChangeCount();
			pkInDialog_->SetDialogListChangeCount( 0 );

			CountModalDialog();
			
            return true;
        }//if
    }//for

    return false;
}//CKTDGUIDialogManager::_RemoveFromDialogList()


//{{ robobeg : 2008-01-07
bool CKTDGUIDialogManager::UnmanageDlg( CKTDGUIDialogType pDialog )
{
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.

    if ( !CheckDlg( pDialog ) )
        return false;

    bool bRemoved = _RemoveFromDialogList( pDialog );
    ASSERT( bRemoved );

    CKTDGUIDialog::CKTDGUIDialogManagerAccess::SetUIDialogManager( IN OUT pDialog, NULL );

    return true;
}//CKTDGUIDialogManager::UnmanageDlg()
//}} robobeg : 2008-01-07


bool CKTDGUIDialogManager::AddDlg( CKTDGUIDialogType pDialog )
{
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.

    if ( pDialog == NULL || pDialog->GetUIDialogManager() != NULL )
        return false;

    CKTDGUIDialog::CKTDGUIDialogManagerAccess::SetUIDialogManager( IN OUT pDialog, this );

//{{ robobeg : 2009-01-07
	// Setting Key ID
	//static int iDlgKeyID = 0;
	//iDlgKeyID++;

 //   ASSERT( m_mapDialogKey.find( iDlgKeyID ) == m_mapDialogKey.end() );
	//m_mapDialogKey[iDlgKeyID] = pDialog;
 //   CKTDGUIDialog::CKTDGUIDialogManagerAccess::SetKeyID( *pDialog, iDlgKeyID );
//}} robobeg : 2009-01-07

    _InsertIntoDialogListTop( pDialog );

//#ifdef HEAVY_LOG_TEST 
//	string dialogName;
//	ConvertWCHARToChar( dialogName, pDialog->GetName() );
//	dialogName += " - AddDlg�Ϸ�";
//	DialogLog( dialogName.c_str() );
//#endif HEAVY_LOG_TEST

	return true;
}


//{{ seojt // 2009-8-18, 18:11
//bool CKTDGUIDialogManager::AddDlg( CKTDGUIDialogHandle hDialog_ )
//{
//    if( hDialog_.IsValid() && CKTDGUIDialog::IsValidUID(hDialog_.GetHandle()) )
//    {
//        return AddDlg( CKTDGUIDialog::GetObjectByUID( hDialog_.GetHandle() ) );
//    }//if
//
//    return false;
//}//CKTDGUIDialogManager::AddDlg()
//}} seojt // 2009-8-18, 18:11


bool CKTDGUIDialogManager::DeleteDlg( CKTDGUIDialogType pDialog )
{
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.

    if ( !CheckDlg( pDialog ) )
        return false;


//#ifdef HEAVY_LOG_TEST
//	string dialogName;
//	ConvertWCHARToChar( dialogName, pDialog->GetName() );
//	dialogName += " - DeleteDlg�Ϸ�";
//	DialogLog( dialogName.c_str() );
//#endif HEAVY_LOG_TEST



    // CKTDGUIDialog destructor���� UnmanageDlg �� ȣ���ϹǷ� �Ʒ� �ڵ�� ���ʿ��ϴ�.
    // ������ �ڵ� ���� �� ���ظ� ���� ����... - robobeg
    UnmanageDlg( pDialog );

#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
    CKTDGUIDialog::Destroy( pDialog.GetHandle() );
    pDialog.Invalidate();
#else
    SAFE_DELETE( pDialog );
#endif // DIALOG_HANDLE_TEST // 2009-8-19

    return true;
}


//{{ seojt // 2009-8-18, 18:02, qff
//bool CKTDGUIDialogManager::DeleteDlg(CKTDGUIDialogHandle hDialog_)
//{
//    if( hDialog_.IsValid() && CKTDGUIDialog::IsValidUID(hDialog_.GetHandle()) )
//    {
//        return DeleteDlg( CKTDGUIDialog::GetObjectByUID( hDialog_.GetHandle() ) );
//    }//if
//
//    return false;
//}//CKTDGUIDialogManager::DeleteDlg
//}} seojt // 2009-8-18, 18:02


void CKTDGUIDialogManager::ClearAllDlg()
{
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.

    while( !m_listDialog.empty() )
    {
        CKTDGUIDialogType pkUIDialog = m_listDialog.front();

//      CKTDGUIDialog destructor���� UnmanageDlg �� ȣ���ϹǷ� �Ʒ� �ڵ�� ���ʿ��ϴ�.
//      ������ �ڵ� ���� �� ���ظ� ���� ����... - robobeg
        UnmanageDlg( pkUIDialog );

#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
        CKTDGUIDialog::Destroy( pkUIDialog.GetHandle() );
        pkUIDialog.Invalidate();
#else
        SAFE_DELETE( pkUIDialog );
#endif // DIALOG_HANDLE_TEST // 2009-8-19
    }//

//{{ robobeg : 2009-01-07
    //m_mapDialogKey.clear();
//}} robobeg : 2009-01-07

#ifdef HEAVY_LOG_TEST
	string dialogName = "- ClearAllDlg �Ҹ�";
	DialogLog( dialogName.c_str() );
#endif HEAVY_LOG_TEST
}

bool CKTDGUIDialogManager::ClearAllOKAndCancelMsgDlg()
{
    unsigned uSize = m_vecIterator.size();

    m_vecIterator.push_back( m_listDialog.begin() );

    while( m_vecIterator.back() != m_listDialog.end() )
    {
        CKTDGUIDialogType  pTempDialog = *m_vecIterator.back();
        ASSERT( pTempDialog != NULL );
        m_vecIterator.back()++;

        if ( pTempDialog->GetOKAndCancelMsgBox() == true )
        {
#ifdef HEAVY_LOG_TEST
			string dialogName;
			ConvertWCHARToChar( dialogName, pTempDialog->GetName() );
			dialogName += " - DeleteDlg�Ϸ�";
			DialogLog( dialogName.c_str() );
#endif HEAVY_LOG_TEST

//      CKTDGUIDialog destructor���� UnmanageDlg �� ȣ���ϹǷ� �Ʒ� �ڵ�� ���ʿ��ϴ�.
//      ������ �ڵ� ���� �� ���ظ� ���� ����... - robobeg
            UnmanageDlg( pTempDialog );

#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
            CKTDGUIDialog::Destroy( pTempDialog.GetHandle() );
            pTempDialog.Invalidate();
#else
            SAFE_DELETE( pTempDialog );
#endif // DIALOG_HANDLE_TEST // 2009-8-19
        }//if
    }//for

    m_vecIterator.pop_back();

    ASSERT( uSize == m_vecIterator.size() );

	return true;
}


bool CKTDGUIDialogManager::CheckDlg( CKTDGUIDialogType pDialogToCheck )
{
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.
	KTDXPROFILE();

    if ( pDialogToCheck == NULL )
        return false;	

	if( m_setDialog.find( pDialogToCheck ) == m_setDialog.end() )
		return false;

	
    //BOOST_TEST_FOREACH( CKTDGUIDialogType, pDialog, m_listDialog )
    //{
    //    if ( pDialog == pDialogToCheck )
    //    {
    //        ASSERT( pDialogToCheck->GetUIDialogManager() == this );
    //        return true;
    //    }
    //}

    if ( pDialogToCheck->GetUIDialogManager() == NULL )
    {
        ASSERT( !"CKTDGUIDialogManager::CheckDlg() : pDialogToCheck is not managed by this manager." );
        return false;
    }

	ASSERT( pDialogToCheck->GetUIDialogManager() == this );
	return true;

    //ASSERT( !"CKTDGUIDialogManager::CheckDlg() : pDialogToCheck may be an invalid pointer." );
    //return false;


}





bool CKTDGUIDialogManager::CheckModalDlg()
{
// ���⼭ ȣ���ϴ� CKTDGUIDialog �Լ����� ��� ����
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.

	KTDXPROFILE();
	
	if( m_iModalDialogCount > 0 )
		return true;

	return false;
}




bool CKTDGUIDialogManager::CheckFrontModalDlg( CKTDGUIDialogType pPivotDialog )
{
// ���⼭ ȣ���ϴ� CKTDGUIDialog �Լ����� ��� ����
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.

	KTDXPROFILE();

    if ( !CheckDlg( pPivotDialog ) )
        return false;

	for( CKTDGUIDialogList::reverse_iterator iter = m_listDialog.rbegin(); 
		iter != m_listDialog.rend(); iter++ )
	{
		CKTDGUIDialogType pDialog = *iter;
		ASSERT( pDialog != NULL );

		if( pDialog == NULL )
			continue;

		if ( pDialog == pPivotDialog )
			return false;

		if ( pDialog->GetShow() == true && 
			pDialog->GetEnable() == true && 
			pDialog->GetModal() == false )
			return true;
	}//for

	return false;
}




CKTDGUIDialogType CKTDGUIDialogManager::GetFirstFrontModalDlg()
{
// ���⼭ ȣ���ϴ� CKTDGUIDialog �Լ����� ��� ����
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.

	KTDXPROFILE();

    for( CKTDGUIDialogList::reverse_iterator iter = m_listDialog.rbegin();
        iter != m_listDialog.rend(); iter++ )
    {
		CKTDGUIDialogType pDialog = *iter;
        ASSERT( pDialog != NULL );
        if ( pDialog->GetShow() == true && pDialog->GetModal() == false )
            return pDialog;
    }//for

	return NULL;
}

void CKTDGUIDialogManager::CountModalDialog( CKTDGUIDialog* pDialog /*= NULL*/ )
{
	KTDXPROFILE();


	if( NULL == pDialog )
	{
		m_iModalDialogCount = 0;

		BOOST_TEST_FOREACH( CKTDGUIDialogType, pDialog, m_listDialog )
		{
			ASSERT( pDialog != NULL );
			CKTDGUIDialog* pDialogRawPointer = pDialog->Get();
			if( false == pDialogRawPointer->GetModal() && 
				true == pDialogRawPointer->GetShow() &&
				false == pDialogRawPointer->GetHasSlot() ) // slot ������ �ִ����� �� üũ ����?? �ϴ� ���� �ڵ��� ���ܵ�, ���߿� Ȯ���ϰ� �����ؾ���
			{
				m_iModalDialogCount += 1;
			}
		}
	}
	else
	{
		if( false == pDialog->GetModal() && 
			true == pDialog->GetShow() &&
			false == pDialog->GetHasSlot() ) // slot ������ �ִ����� �� üũ ����?? �ϴ� ���� �ڵ��� ���ܵ�, ���߿� Ȯ���ϰ� �����ؾ���
		{
			m_iModalDialogCount += 1;
		}
	}
}


bool CKTDGUIDialogManager::CheckMostFrontDlg( CKTDGUIDialogType pDialog )
{
// ���⼭ ȣ���ϴ� CKTDGUIDialog �Լ����� ��� ����
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.

	KTDXPROFILE();

    if ( !CheckDlg( pDialog ) )
        return false;

    ASSERT( !m_listDialog.empty() );

    if ( !m_listDialog.empty() )
    {
        if ( m_listDialog.back() == pDialog )
            return true;
        return false;
    }//if

	return false;
}

bool CKTDGUIDialogManager::ChangeLayer( CKTDGUIDialogType pDialog, int layerToChange )
{
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.
	KTDXPROFILE();

    if ( !CheckDlg( pDialog ) )
        return false;

    if ( pDialog->GetLayer() == layerToChange )
        return true;

    _RemoveFromDialogList( pDialog );

    CKTDGUIDialog::CKTDGUIDialogManagerAccess::SetLayer( IN OUT pDialog, layerToChange );

    _InsertIntoDialogListTop( pDialog );

    return true;
}


bool CKTDGUIDialogManager::ChangeFront( CKTDGUIDialogType pDialog, bool bFront )
{
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.

	KTDXPROFILE();

    if ( !CheckDlg( pDialog ) )
        return false;

    if ( pDialog->GetFront() == bFront )
        return true;

    _RemoveFromDialogList( pDialog );

    CKTDGUIDialog::CKTDGUIDialogManagerAccess::SetFront( IN OUT pDialog, bFront );

    _InsertIntoDialogListTop( pDialog );

    return true;
}//CKTDGUIDialogManager::ChangeFront()


bool CKTDGUIDialogManager::ChangeSequence( CKTDGUIDialogType pDialog, bool bTop )
{
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.

	KTDXPROFILE();


    if ( !CheckDlg( pDialog ) )
        return false;



	// �� ������ �� �Լ��� ȣ���ϴ� ��찡 �־ ���ɰ����� ���ؼ� �ϴ� �̷��� dialog list�� ��ȭ�� ���� ��쿡 dialog�� �ٽ� ���ٰ� �ִ� �۾��� ���ϵ��� �Ѵ�.
	if( abs( pDialog->GetDialogListChangeCount() ) == m_iDialogListChangeCount )
	{
		if( true == bTop ) 
		{
			if( pDialog->GetDialogListChangeCount() > 0 ) // dialog list�� �߰��� �� _InsertIntoDialogListTop()���� �߰��Ǿ���
				return true;
		}
		else
		{
			if( pDialog->GetDialogListChangeCount() < 0 ) // dialog list�� �߰��� �� _InsertIntoDialogListBottom()���� �߰��Ǿ���
				return true;
		}
	}



    bool bRet = _RemoveFromDialogList( pDialog );
    ASSERT( bRet );
    if ( bTop )
        _InsertIntoDialogListTop( pDialog );
    else
        _InsertIntoDialogListBottom( pDialog );

	return true;
}


//{{ seojt // 2009-8-18, 18:13
//bool CKTDGUIDialogManager::ChangeSequence(CKTDGUIDialogHandle hDialog_, bool bTop_)
//{
//    if( hDialog_.IsValid() && CKTDGUIDialog::IsValidUID(hDialog_.GetHandle()) )
//    {
//        return ChangeSequence( CKTDGUIDialog::GetObjectByUID( hDialog_.GetHandle() ), bTop_ );
//    }//if
//
//    return false;
//}//CKTDGUIDialogManager::ChangeSequence()
//}} seojt // 2009-8-18, 18:13


bool CKTDGUIDialogManager::ChangeSequence( CKTDGUIDialogType pDialogToChange, CKTDGUIDialogType pPivotDialog, bool bOver, bool bChangeLayer )
{
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.

	KTDXPROFILE();

	if ( CheckDlg( pDialogToChange ) == false || CheckDlg( pPivotDialog ) == false || pDialogToChange == pPivotDialog )
		return false;

    if ( !bChangeLayer && ( pDialogToChange->GetFront() != pPivotDialog->GetFront()
        || pDialogToChange->GetLayer() != pPivotDialog->GetLayer() ) )
        return false;

    bool bRet = _RemoveFromDialogList( pDialogToChange );
    ASSERT( bRet );

    CKTDGUIDialog::CKTDGUIDialogManagerAccess::SetFront( IN OUT pDialogToChange, pPivotDialog->GetFront() );
    CKTDGUIDialog::CKTDGUIDialogManagerAccess::SetLayer( IN OUT pDialogToChange, pPivotDialog->GetLayer() );

    for( CKTDGUIDialogList::iterator iter = m_listDialog.begin(); iter != m_listDialog.end(); iter++ )
    {
        CKTDGUIDialogType pDialog = *iter;
        ASSERT( pDialog != NULL );
        if ( pDialog == pPivotDialog )
        {
			if( true == bOver )
				iter++;

			m_listDialog.insert( iter, pDialogToChange );
			m_setDialog.insert( pDialogToChange );
			IncreaseDialogListChangeCount();
			pDialogToChange->SetDialogListChangeCount( m_iDialogListChangeCount );

            return true;
        }//if
    }//if


    ASSERT( 0 );	
    return false;
}

CKTDGUIDialog* CKTDGUIDialogManager::GetDialog_LUA( const char* pName )
{
	KTDXPROFILE();


// ���⼭ ȣ���ϴ� CKTDGUIDialog �Լ����� ��� ����
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.

	if( NULL == pName || pName[0] == NULL )
		return NULL;


	wstring wstrName = L"";
	ConvertUtf8ToWCHAR( wstrName, pName );

	if( true == wstrName.empty() )
		return NULL;

	CKTDGUIDialogType& pDialog = GetDialog( wstrName.c_str() );
	if( pDialog != NULL )
		return pDialog->Get();

	return NULL;



//    BOOST_TEST_FOREACH( CKTDGUIDialogType, pDialog, m_listDialog )
//    {
//        ASSERT( pDialog != NULL );
//		
//		if( 0 == wstrName.compare( pDialog->GetName() ) )
//        {
//#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
//            return CKTDGUIDialog::GetObjectByUID( pDialog.GetHandle() );
//#else
//			return pDialog;
//#endif // DIALOG_HANDLE_TEST // 2009-8-19
//        }//if
//    }//BOOST_TEST_FOREACH()
//
//
//	return NULL;
}



CKTDGUIDialogType CKTDGUIDialogManager::GetDialog( const WCHAR* pName )
{
	KTDXPROFILE();

// ���⼭ ȣ���ϴ� CKTDGUIDialog �Լ����� ��� ����
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.

	if( NULL == pName )
		return NULL;

	if( m_mapDialogListChangeCountWhenLastGetDialog[ pName ] == m_iDialogListChangeCount )
	{
		CKTDGUIDialogMap::iterator it = m_mapDialog.find( pName );
		if( it != m_mapDialog.end() )
		{
			return it->second;
		}
		else
		{
			return NULL;
		}
	}



    BOOST_TEST_FOREACH( CKTDGUIDialogType, pDialog, m_listDialog )
    {
        ASSERT( pDialog != NULL );
		if ( wcscmp( pName, pDialog->GetName() ) == 0 )
		{
			m_mapDialog[ pName ] = pDialog;
			m_mapDialogListChangeCountWhenLastGetDialog[ pName ] = m_iDialogListChangeCount;
			return pDialog;
		}
    }


	m_mapDialogListChangeCountWhenLastGetDialog[ pName ] = m_iDialogListChangeCount;
	return NULL;
}

#ifdef PRIORITY_EVERY_UI
bool CKTDGUIDialogManager::ExistDialogAtMousePos()
{
	KTDXPROFILE();

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	CKTDGUIDialogSet::iterator sitDialog;

	for( sitDialog = m_setDialog.begin(); sitDialog != m_setDialog.end(); ++sitDialog )
//#else
//	CKTDGUIDialogList::iterator sitDialog;
//
//	for( sitDialog = m_listDialog.begin(); sitDialog != m_listDialog.end(); ++sitDialog )
//#endif
	{
		if( *sitDialog == NULL )
			continue;

		if( ( *sitDialog )->GetControlMouseOver() != NULL )
			return true;
	}

	return false;
}
#endif PRIORITY_EVERY_UI




HRESULT CKTDGUIDialogManager::OnFrameMove( double fTime, float fElapsedTime )
{

	KTDXPROFILE();

	KTDXPROFILE_BEGIN( "DIALOG_DELETE" );
    
        unsigned uSize = m_vecIterator.size();
        
        m_vecIterator.push_back( m_listDialog.begin() );

        while( m_vecIterator.back() != m_listDialog.end() )
        {
            CKTDGUIDialogType pDialog = *m_vecIterator.back();
            ASSERT( pDialog != NULL );
            m_vecIterator.back()++;

            if ( pDialog->GetIsAutoDeleteEnd() == true )
            {
			    string dialogName;
			    ConvertWCHARToChar( dialogName, pDialog->GetName() );
			    dialogName += " - DeleteDlg���� by AutoDelete";
			    DialogLog( dialogName.c_str() );

//      CKTDGUIDialog destructor���� UnmanageDlg �� ȣ���ϹǷ� �Ʒ� �ڵ�� ���ʿ��ϴ�.
//      ������ �ڵ� ���� �� ���ظ� ���� ����... - robobeg
                UnmanageDlg( pDialog );

#ifdef DIALOG_HANDLE_TEST // 2009-8-19 by jintaeks
                CKTDGUIDialog::Destroy( pDialog.GetHandle() );
                pDialog.Invalidate();
#else
                SAFE_DELETE( pDialog );
#endif // DIALOG_HANDLE_TEST // 2009-8-19
            }//if
        }//for
    
        m_vecIterator.pop_back();    

        ASSERT( uSize == m_vecIterator.size() );
	
	KTDXPROFILE_END();

#ifdef DIALOG_SHOW_TOGGLE
	if( GetHideDialog() == true )
	{
		CKTDGUIDialogType pDialog = GetDialog( L"DLG_ChatBox_EditBox" );
		if( pDialog != NULL )
			pDialog->OnFrameMove( fTime, fElapsedTime );

		pDialog = GetDialog( L"DLG_MsgBox" );
		if( pDialog != NULL )
			pDialog->OnFrameMove( fTime, fElapsedTime );

		pDialog = GetDialog( L"DLG_OKMsgBox" );
		if( pDialog != NULL )
			pDialog->OnFrameMove( fTime, fElapsedTime );

		pDialog = GetDialog( L"OkAndCancelMsgBox" );
		if( pDialog != NULL )
			pDialog->OnFrameMove( fTime, fElapsedTime );

		return S_OK;
	}
#endif //DIALOG_SHOW_TOGGLE

	KTDXPROFILE_BEGIN( "DIALOG_FRAME_MOVE" );
        
	//���� ���ʿ� �ִ� ���̾�αװ� ���� ��ܿ� ��ġ�� ���̾�α� �̹Ƿ� ���ʺ��� ó���Ѵ�.

        if ( !m_listDialog.empty() )
        {
            unsigned uSize2 = m_vecReverseIterator.size();

            m_vecReverseIterator.push_back( m_listDialog.end() );
            m_vecReverseIterator.back()--;

            while( m_vecReverseIterator.back() != m_listDialog.end() )
            {
                CKTDGUIDialogType pDialog = *m_vecReverseIterator.back();
                ASSERT( pDialog != NULL );
                if ( m_vecReverseIterator.back() == m_listDialog.begin() )
                    m_vecReverseIterator.back() = m_listDialog.end();
                else
                    m_vecReverseIterator.back()--;

                if ( pDialog->GetEnable() == true && pDialog->GetShow() == true )
				{
					string dialogName;
					ConvertWCHARToChar( dialogName, pDialog->GetName() );
					LastErrorLog( dialogName.c_str() );

                    pDialog->OnFrameMove( fTime, fElapsedTime );
				}
/*	
		    if( pDialog->GetModal() == false )
			    break;
*/
            }//while

            m_vecReverseIterator.pop_back();

            ASSERT( uSize2 == m_vecReverseIterator.size() );

        }//if

	KTDXPROFILE_END();

	return S_OK;
}

HRESULT CKTDGUIDialogManager::OnFrameRender( bool bFront )
{


	KTDXPROFILE();

#ifdef DIALOG_SHOW_TOGGLE
	if( GetHideDialog() == true )
	{
		CKTDGUIDialogType pDialog = GetDialog( L"DLG_ChatBox_EditBox" );
		if( pDialog != NULL && pDialog->GetShow() )
			pDialog->OnFrameRender();

		pDialog = GetDialog( L"DLG_MsgBox" );
		if( pDialog != NULL && pDialog->GetShow() )
			pDialog->OnFrameRender();

		pDialog = GetDialog( L"DLG_OKMsgBox" );
		if( pDialog != NULL && pDialog->GetShow() )
			pDialog->OnFrameRender();

		pDialog = GetDialog( L"OkAndCancelMsgBox" );
		if( pDialog != NULL && pDialog->GetShow() )
			pDialog->OnFrameRender();

		return S_OK;
	}
#endif //DIALOG_SHOW_TOGGLE

	if ( m_bShow == false )
		return S_OK;
	


    unsigned uSize = m_vecIterator.size();

    m_vecIterator.push_back( m_listDialog.begin() );

    while( m_vecIterator.back() != m_listDialog.end() )
    {
        CKTDGUIDialogType pDialog = *m_vecIterator.back();
        ASSERT( pDialog != NULL );
        m_vecIterator.back()++;
        if ( pDialog->GetFront() != bFront )
            continue;


        if ( pDialog->GetShow() )
            pDialog->OnFrameRender();
    }//while

#ifdef LOG_LOAD_DLG_SCRIPT
	if ( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState(DIK_LCONTROL) == TRUE )
	{
		m_vecIterator.back() = m_listDialog.begin();

		int iNum = 0;

		while( m_vecIterator.back() != m_listDialog.end() )
		{
			CKTDGUIDialogType pDialog = *m_vecIterator.back();
			ASSERT( pDialog != NULL );
			m_vecIterator.back()++;
			if ( pDialog->GetFront() != bFront )
				continue;
			
			if ( pDialog->GetShow() && pDialog->GetShowScriptName() )
			{
				GetUKFont( 4 )->OutTextXY( 412, 100 + 100 * iNum, pDialog->GetScriptFileName().c_str(), D3DXCOLOR(1,0.6f,0,1),
					CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );
				GetUKFont( 4 )->OutTextXY( 412, 150 + 100 * iNum, pDialog->GetMouseOverStaticName().c_str(), D3DXCOLOR(1,0.6f,0,1),
					CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_CENTER );

				++iNum;
			}
		}//while
	}
#endif //LOG_LOAD_DLG_SCRIPT

    m_vecIterator.back() = m_listDialog.begin();

    while( m_vecIterator.back() != m_listDialog.end() )
    {
        CKTDGUIDialogType pDialog = *m_vecIterator.back();
        ASSERT( pDialog != NULL );
        m_vecIterator.back()++;
        if ( pDialog->GetFront() != bFront )
            continue;
        if ( pDialog->GetShow() && pDialog->GetGuideControl() != NULL )
		{
            pDialog->GetGuideControl()->OnFrameRender();
		}
    }//while

    m_vecIterator.pop_back();

    ASSERT( uSize == m_vecIterator.size() );

#ifdef NEW_SKILL_TREE_UI
	if(CKTDGUISlot::s_pDragSlot != NULL && bFront)
	{
		CKTDGUISlot::s_pDragSlot->RenderDragSlot();
	}
#endif

	return S_OK;
}

HRESULT CKTDGUIDialogManager::OnFrameRenderTopModal()
{

 #ifdef DIALOG_SHOW_TOGGLE
 	if( GetHideDialog() == true )
 		return S_OK;
 #else
 	if ( m_bShow == false )
 		return S_OK;
 #endif

// OnFrameRender ȣ�� �� �ٷ� �����ϹǷ� �ܼ� iteration �ص� ����

    for( CKTDGUIDialogList::reverse_iterator iter = m_listDialog.rbegin();
        iter != m_listDialog.rend(); iter++ )
    {
        CKTDGUIDialogType pDialog = *iter;
        ASSERT( pDialog != NULL );

		if( pDialog->GetEnable() && pDialog->GetShow() == true && pDialog->GetModal() == false )
		{
			pDialog->OnFrameRender();
            return S_OK;
		}//if
    }//for

	return S_OK;
}

HRESULT CKTDGUIDialogManager::OnResetDevice()
{

    unsigned uSize = m_vecIterator.size();
    
    m_vecIterator.push_back( m_listDialog.begin() );

    while(m_vecIterator.back() != m_listDialog.end() )
    {
        CKTDGUIDialogType pDialog = *m_vecIterator.back();
        m_vecIterator.back()++;
        ASSERT( pDialog != NULL );
		if ( pDialog != NULL )
			pDialog->OnResetDevice();
    }//for

    m_vecIterator.pop_back();

    ASSERT( uSize == m_vecIterator.size() );

	return S_OK;
}

HRESULT CKTDGUIDialogManager::OnLostDevice()
{

    unsigned uSize = m_vecIterator.size();

    m_vecIterator.push_back( m_listDialog.begin() );

    while( m_vecIterator.back() != m_listDialog.end() )
    {
        CKTDGUIDialogType pDialog = *m_vecIterator.back();
        m_vecIterator.back()++;
        ASSERT( pDialog != NULL );
		if ( pDialog != NULL )
			pDialog->OnLostDevice();
    }//for

    m_vecIterator.pop_back();

    ASSERT( uSize == m_vecIterator.size() );

	return S_OK;
}

bool CKTDGUIDialogManager::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	KTDXPROFILE();

	//���� ���ʿ� �ִ� ���̾�αװ� ���� ��ܿ� ��ġ�� ���̾�α� �̹Ƿ� ���ʺ��� ó���Ѵ�.

    if ( m_listDialog.empty() )
        return false;

    unsigned uSize = m_vecReverseIterator.size();

    m_vecReverseIterator.push_back( m_listDialog.end() );
    m_vecReverseIterator.back()--;

    bool    bRet = false;

    while( m_vecReverseIterator.back() != m_listDialog.end() )
    {
        CKTDGUIDialogType pDialog = *m_vecReverseIterator.back();
        if ( m_vecReverseIterator.back() == m_listDialog.begin() )
            m_vecReverseIterator.back() = m_listDialog.end();
        else
            m_vecReverseIterator.back()--;
        ASSERT( pDialog != NULL );

		if( pDialog->MsgProc( hWnd, uMsg, wParam, lParam ) == true )
        {
            bRet = true;
            break;
        }//if

		if( pDialog->GetEnable() == true && pDialog->GetShow() == true &&
			pDialog->GetModal() == false )
        {
            bRet = false;
            break;
        }//if
    }//for

    m_vecReverseIterator.pop_back();

    ASSERT( uSize == m_vecReverseIterator.size() );

#ifdef NEW_SKILL_TREE_UI
	if(CKTDGUISlot::s_pDragSlot != NULL)
	{
		switch( uMsg )
		{
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
			{
				CKTDGUISlot::ReturnDraggingItem();
			} break;
		default:
			break;
		}		
	}
#endif

	return bRet;
}

#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
void CKTDGUIDialogManager::SetFont( int uiFontID, CKTDGFontManager::CKTDGFont* pFont )
{
	//m_FontList.push_back( pFont );

	if ( m_mapFontList.find( uiFontID ) != m_mapFontList.end() )
		return;

	m_mapFontList.insert( map< int, CKTDGFontManager::CKTDGFont* >::value_type( uiFontID, pFont ) );
}
#endif


void CKTDGUIDialogManager::SetUKFont( int uiUKFontID, CKTDGFontManager::CUKFont* pFont )
{ 
	//m_UKFontList.push_back( pFont ); 

	if ( m_mapUKFontList.find( uiUKFontID ) != m_mapUKFontList.end() )
		return;


	m_mapUKFontList.insert( map< int, CKTDGFontManager::CUKFont* >::value_type( uiUKFontID, pFont ) );
}



bool CKTDGUIDialogManager::IsDraggingDialog()
{
// ���⼭ ȣ���ϴ� CKTDGUIDialog �Լ����� ��� ����
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.

	KTDXPROFILE();

    BOOST_TEST_FOREACH( CKTDGUIDialogType, pDialog, m_listDialog )
    {
        ASSERT( pDialog != NULL );
        if ( pDialog->GetShow() == true && pDialog->GetEnable() == true
            && pDialog->GetEnableMoveByDrag() == true && pDialog->GetMouseDownForDrag() == true )
            return true;
    }//BOOST_TEST_FOREACH()

	return false;
}

CKTDGUIDialogType CKTDGUIDialogManager::GetCloseDialogByESC()
{
// ���⼭ ȣ���ϴ� CKTDGUIDialog �Լ����� ��� ����
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.

	KTDXPROFILE();

    for( CKTDGUIDialogList::reverse_iterator iter = m_listDialog.rbegin();
        iter != m_listDialog.rend(); iter++ )
    {
		CKTDGUIDialogType pDialog = *iter;
        ASSERT( pDialog != NULL );
		if ( wcscmp( pDialog->GetName(), L"DLG_MsgBox" ) == 0 )
			continue;

		if( (pDialog->GetOKAndCancelMsgBox() == true ||	wcscmp( pDialog->GetName(), L"DLG_OKMsgBox" ) == 0) && pDialog->GetShow() == true )
		{
			g_pKTDXApp->GetDIManager()->SetEnable(true);

			return pDialog;
		}

		if ( pDialog->GetShow() == true && pDialog->GetEnable() == true && pDialog->GetCloseCustomUIEventID() != -1 )
		{
			return pDialog;
		}

		if ( pDialog->GetShow() == true && pDialog->GetEnable() == true && pDialog->GetModal() == false )
			return NULL;
    }//for

	return NULL;
}


void CKTDGUIDialogManager::SetStage( CKTDXStage* pStage )
{
// ���⼭ ȣ���ϴ� CKTDGUIDialog �Լ����� ��� ����
// �� �Լ��� �ܺ� iteration �߿� ȣ��Ǿ �����ϴ�. ���������ε� ������ iteration�� �������� �ʴ´�.

    BOOST_TEST_FOREACH( CKTDGUIDialogType, pDialog, m_listDialog )
    {
        ASSERT( pDialog != NULL );
        pDialog->SetStage( pStage );
    }//BOOST_TEST_FOREACH()

}

bool CKTDGUIDialogManager::GetFocusInEditBox()
{
	CKTDGUIControl* pControl = CKTDGUIDialog::GetControlFocus();
	if ( pControl != NULL )
	{
		if ( pControl->GetType() == CKTDGUIControl::UCT_IME_EDITBOX ||
			pControl->GetType() == CKTDGUIControl::UCT_EDITBOX )
		{
			return true;
		}
	}

	return false;
}

#ifdef DIALOG_SHOW_TOGGLE
bool CKTDGUIDialogManager::GetHideDialog()
{
	if( m_bCanHide == true && m_bShow == false )
		return true;

	return false;
}
#endif
