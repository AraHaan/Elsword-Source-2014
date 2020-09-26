#include "StdAfx.h"
#include "X2PopupUIBase.h"


/** @function 	: CX2PopupUIBase
	@brief 		: ������
*/
#pragma region CX2PopupUIBase
CX2PopupUIBase::CX2PopupUIBase(void)
	: m_bShow ( false )
	, m_pDlgPopup( NULL )
	, m_pNowState( NULL )
	, m_pPicLeftTop( NULL )
	, m_pPicCenterTop ( NULL )
	, m_pPicRightTop( NULL )
	, m_pPicLeftMiddle( NULL )
	, m_pPicCenterMiddle( NULL )
	, m_pPicRightMiddle( NULL )
	, m_pPicLeftBottom( NULL )
	, m_pPicCenterBottom( NULL )
	, m_pPicRightBottom( NULL )
	, m_nMenuCount( 0 )
{
	// ���� �� �Ҵ�
	m_pDlgPopup = new CKTDGUIDialog( g_pMain->GetNowState(), L"DLG_UI_PopupSkin.lua" );

	if ( NULL != m_pDlgPopup )
	{
		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDlgPopup );    
		m_pDlgPopup->SetShowEnable( false, false );

		CKTDGUIStatic *pStaticDlg = m_pDlgPopup->GetStatic_LUA( "PopupSkin" );

		if ( NULL != pStaticDlg )
		{
			m_pPicLeftTop		= pStaticDlg->GetPicture( 0 );
			m_pPicCenterTop		= pStaticDlg->GetPicture( 1 );
			m_pPicRightTop		= pStaticDlg->GetPicture( 2 );
			m_pPicLeftMiddle	= pStaticDlg->GetPicture( 3 );
			m_pPicCenterMiddle	= pStaticDlg->GetPicture( 4 );
			m_pPicRightMiddle	= pStaticDlg->GetPicture( 5 );
			m_pPicLeftBottom	= pStaticDlg->GetPicture( 6 );
			m_pPicCenterBottom	= pStaticDlg->GetPicture( 7 );
			m_pPicRightBottom	= pStaticDlg->GetPicture( 8 );
		}
	}
}
#pragma endregion ()

/** @function 	: ~CX2PopupUIBase
	@brief 		: �Ҹ���
*/
#pragma region ~CX2PopupUIBase
CX2PopupUIBase::~CX2PopupUIBase(void)
{
	SAFE_DELETE_DIALOG( m_pDlgPopup );
	m_pNowState = NULL;
}
#pragma endregion ()

/** @function 	: ClosePopupMenu
	@brief 		: �˾� �޴��� ��Ȱ��ȭ ��Ų��.
*/
#pragma region ClosePopupMenu
void CX2PopupUIBase::ClosePopupMenu()
{
	m_pDlgPopup->SetShowEnable(false, false);

	m_bShow = false;
	m_nMenuCount = 0;
}
#pragma endregion ()

/** @function 	: SetStage
	@brief 		: ���� ���������� �����͸� ����
*/
#pragma region SetStage
void CX2PopupUIBase::SetStage( CKTDXStage* pNowState )
{
	m_pNowState = (CX2State*) pNowState;
	m_pDlgPopup->SetStage(m_pNowState);
}
#pragma endregion ( CKTDXStage* pNowState )

/** @function 	: ChangeTopImage
	@brief 		: �˾� �޴��� ��� �̹����� ��ü�Ѵ�.( true : ���� �̹���, false : ū �̹��� )
*/
#pragma region ChangeTopImage
void CX2PopupUIBase::ChangeTopImage( IN bool bChange_ )
{
	CKTDGUIStatic *pStaticDlg = m_pDlgPopup->GetStatic_LUA( "PopupSkin" );

	if ( NULL != pStaticDlg )
	{
		if ( true == bChange_ )
		{
			m_pPicLeftTop->SetShow( false );
			m_pPicCenterTop->SetShow( false );
			m_pPicRightTop->SetShow( false );

			m_pPicLeftTop		= pStaticDlg->GetPicture( 9 );
			m_pPicCenterTop		= pStaticDlg->GetPicture( 10 );
			m_pPicRightTop		= pStaticDlg->GetPicture( 11 );
		}
		else
		{
			m_pPicLeftTop->SetShow( false );
			m_pPicCenterTop->SetShow( false );
			m_pPicRightTop->SetShow( false );

			m_pPicLeftTop		= pStaticDlg->GetPicture( 0 );
			m_pPicCenterTop		= pStaticDlg->GetPicture( 1 );
			m_pPicRightTop		= pStaticDlg->GetPicture( 2 );
		}

		m_pPicLeftTop->SetShow( true );
		m_pPicCenterTop->SetShow( true );
		m_pPicRightTop->SetShow( true );
	}
}
#pragma endregion ( IN bool bChange_ )
