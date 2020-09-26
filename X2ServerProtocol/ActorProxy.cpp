#include "StdAfx.h"

#include "ActorProxy.h"

NiImplementRTTI( KActorProxy, KSession );

KActorProxy::KActorProxy() 
:
    KSession( true ), 
    m_bThrowWinMsg( false ),
    m_nAckOK( -1 ),
    m_iClassID( 0 ),
    m_iLoad( 0 )
{
	m_spEventAck.reset( CreateEvent( NULL, false, false, NULL ), CloseHandle );
	m_pSendGameMessage = NULL;
}

ImplToStringW( KActorProxy )
{
	return START_TOSTRING_PARENTW( KSession );
}

void KActorProxy::PostEvent( KEvent* pkEvent_ )
{
	// SendMessage�ϸ� �޼����� ó���ɶ����� ��ٸ�����, PostMessage�̹Ƿ� ť�׸� �Ѵ�.
	// cmd : WM_APP + class ID
	// wParam : Event ID
	// lParam : data �� ������

	if( m_bThrowWinMsg )
	{
		if( g_bIsSERVICE == false )
		{
			//X2�޼��� ó��
			dbg::clog << L"post message : " << pkEvent_->GetIDStr() << dbg::endl;
		}	

		KSerBuffer* pBuff = new KSerBuffer( pkEvent_->m_kbuff );

		if( m_pSendGameMessage != NULL )
			m_pSendGameMessage( 
			m_ServerMsgID, 
			static_cast<WPARAM>(pkEvent_->m_usEventID), 
			reinterpret_cast<LPARAM>(pBuff), 
			false
			);
		//::PostMessage( m_hWnd, m_ServerMsgID, 
		//				static_cast<WPARAM>(pkEvent_->m_usEventID),
		//				reinterpret_cast<LPARAM>(pBuff) );
	}
	else
	{
        KEventPtr spEvent( new KEvent( *pkEvent_ ) );

		KLocker lock( m_csRecvMsg );
		m_queRecvMsg.push( spEvent );
	}
}

void KActorProxy::SetHwnd( HWND hWnd_ )
{
	m_hWnd = hWnd_;

	if( ::IsWindow( m_hWnd ) )
		m_bThrowWinMsg = true;
}

void KActorProxy::SetThrowWinMsg( bool bThrowWinMsg_ )
{
	m_bThrowWinMsg = bThrowWinMsg_;

	if( m_bThrowWinMsg && ::IsWindow( m_hWnd ) == FALSE )
	{
		START_LOG( cerr, L"Invalid Window Handle" << END_LOG );
		assert( !"KUserProxy�� �ùٸ� HWND�� �����ϼ���" );
	}
}

int KActorProxy::ConnectNAuth( IN const WCHAR* pIP, 
							  IN const int port, 
							  IN const KEvent& kEvent_, 
							  IN DWORD dwTimeOut_ )
{
	std::wstring wIP = pIP;
	std::string IP = KncUtil::toNarrowString( wIP );

	if( !KSession::Connect( IP.c_str(), port ) )
	{
		START_LOG( cerr, L"game server connect failed. IP : " 
			<< pIP
			<< ":" << port );
		return -1;
	}

	if( ! KSession::SendPacket( kEvent_ ) )
	{
		START_LOG( cerr, L"sending verify account req packet failed." );
		return -2;
	}

	m_nAckOK = -99; // unused-error code.

	if( m_bUseIocp )    // IOCP�� ����ϴ� ��� ��ü �����尡 ����.
	{
		DWORD dwTick = ::GetTickCount();
		while( GetTickCount() - dwTick < dwTimeOut_ && m_nAckOK == -99 )
		{
			Sleep( 30 );
			Tick();
		}

		if( m_nAckOK != -99 )   // ack�� ����� ���� ���.
			return m_nAckOK;
	}
	else
	{
		switch( ::WaitForSingleObject( m_spEventAck.get(), dwTimeOut_ ) ) {
		case WAIT_OBJECT_0: // ack�� ����� ���� ���.
			return m_nAckOK;
			break;
		case WAIT_TIMEOUT: // �ð� �ʰ�
			START_LOG( cerr, L"verify account timeout." );
			break;
		default:
			START_LOG( cerr, L"invalide event object." );
		}
	}

	return -3;
}