#include "StdAfx.h"
#include ".\x2informer.h"

CX2Informer::CX2Informer(void)
{
	m_pTalkBoxManager = CX2TalkBoxManager::CreateTalkBoxManger();
	g_pKTDXApp->GetDGManager()->AddObjectChain( m_pTalkBoxManager );
	m_pTalkBoxManager->SetOverUI( true );

	m_vTalkBoxPos				= D3DXVECTOR2(0,0);
	m_bCheckInfo				= false;

	m_bMouseDown				= false;

	m_pSoundAlarm				= g_pKTDXApp->GetDeviceManager()->OpenSound( L"MessageAlarm.ogg" );

#ifdef EVENT_SCENE
	m_bSetShowAfterEventSceneEnd		= false;
#endif EVENT_SCENE
	m_vTalkBoxOffsetPos			= D3DXVECTOR2(0,0);

	SetNeedCheckInfo( true );

	m_InformerType = XIT_SKILL;
}

CX2Informer::~CX2Informer(void)
{
	CX2TalkBoxManager::DeleteKTDGObject( m_pTalkBoxManager );
	SAFE_CLOSE( m_pSoundAlarm );
}

bool CX2Informer::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( m_pTalkBoxManager->MsgProc( hWnd, uMsg, wParam, lParam) == true )
		return true;

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
		{
			UidType unitUID = 0;

			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			if ( m_pTalkBoxManager->CheckMousePointInTalkbox( mousePos, unitUID ) == true && m_pTalkBoxManager->GetTalkBoxNum() != 0 )
			{
				m_bMouseDown = true;
				return true;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			UidType unitUID = 0;
			bool bCheck = false;

			POINT mousePoint = { short(LOWORD(lParam)), short(HIWORD(lParam)) };
			D3DXVECTOR2 mousePos = D3DXVECTOR2( (float)mousePoint.x, (float)mousePoint.y );
			if ( m_pTalkBoxManager->CheckMousePointInTalkbox( mousePos, unitUID ) == true && m_bMouseDown == true )
			{
				InvalidInfo();
				bCheck = true;
			}

			m_bMouseDown = false;

			if ( bCheck == true )
				return true;
		}
		break;
	}
	
	return false;
}

HRESULT CX2Informer::OnFrameMove( double fTime, float fElapsedTime )
{
	if ( GetNeedCheckInfo() )
	{
		CheckInfo( false );
		SetNeedCheckInfo( false );	/// üũ ������ �ٽ� üũ �� �ʿ� ���ٰ� ����
	}

#ifdef EVENT_SCENE
	if(g_pData->GetEventScene()->GetIsPlayingEventScene())
	{
		if(m_pTalkBoxManager->GetShowTalk())
		{
			m_pTalkBoxManager->SetShowTalk( false );
			m_bSetShowAfterEventSceneEnd = true;
		}
	}
	else if(m_bSetShowAfterEventSceneEnd)
	{
		m_pTalkBoxManager->SetShowTalk( true );
		m_bSetShowAfterEventSceneEnd = false;
	}
#endif EVENT_SCENE

	return m_pTalkBoxManager->OnFrameMove( fTime, fElapsedTime );
}

void CX2Informer::Reset()
{
	m_bCheckInfo = false;
}

void CX2Informer::CheckInfo( bool bStateChange )
{
	if ( bStateChange == true ) 
	{
		if ( m_bCheckInfo == true ) //������ ������ Ȱ��ȭ�� �־���.
		{
			//Ȥ�� ���� �ٲ��� ���� �𸣴ϱ� üũ
			if ( IsNowInfoChange() == true )
			{
				//�ѵ�ä�� �״�� �д�.
				SetNowInfo();
				SetShow( true );
			}
			else
			{
				InvalidInfo();
			}
		}
		else
		{
			if ( IsNowInfoChange() == true )
			{
				m_bCheckInfo = true;
				SetNowInfo();
				SetShow( true );
			}
			else
			{
				InvalidInfo();
			}
		}
	}
	else
	{
		if ( IsNowInfoChange() == true )
		{
			//�ѵ�ä�� �״�� �д�.
			SetShow( true );
			m_bCheckInfo = true;
			SetNowInfo();
		}
	}
}

void CX2Informer::InvalidInfo()
{
	SetShow( false );
}

void CX2Informer::SetShow( bool bShow )
{
	
	
}

