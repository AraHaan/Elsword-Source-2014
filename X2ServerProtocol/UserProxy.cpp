#include "StdAfx.h"

#include "UserProxy.h"

NiImplementRTTI( KUserProxy, KActorProxy );

#define CLASS_TYPE KUserProxy

KUserProxy::KUserProxy( USER_PROXY_TYPE eType ) :
m_eProxyType( eType ),
m_bIntendedDisconnect( false )
{
}

KUserProxy::~KUserProxy(void)
{
	End( INFINITE );
}

ImplToStringW( KUserProxy )
{
    return START_TOSTRING_PARENTW( KActorProxy );
}

void KUserProxy::OnSocketError()
{    
	KSession::OnSocketError();


	//{{ dmlee 2009.3.11 �ǵ����� ���� ���ᰡ �ƴ϶�� ���� event�� �߻��Ѵ�.
	switch( m_eProxyType )
	{
	default:
	case UPT_INVALID:
		{
			assert( !"invalid proxy type" );
		} break;

	case UPT_GAME_SERVER:
	case UPT_CHANNEL_SERVER:
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
	case UPT_COLLECT_SERVER:
#endif
		{
			if( false == m_bIntendedDisconnect )
			{
				// ���� Ŭ���̾�Ʈ���� ������ ���������� �˷��� �Ѵ�.
				KEvent kEvent;
				kEvent.m_usEventID = E_CONNECTION_LOST_NOT;

				// windows msg sendmessage
				PostEvent( &kEvent );
			}
		} break;

	}
	//}} dmlee 2009.3.11 �ǵ����� ���� ���ᰡ �ƴ϶�� ����ó�� event�� �߻��Ѵ�.

}

void KUserProxy::OnAuthenticFailed()
{
	KSession::OnAuthenticFailed(); // �ƹ��� ������ ���� ������ �ϴ� ȣ��

	// proxy type�� gameserver�϶��� �����ϵ��� ó��
	if( m_eProxyType == UPT_GAME_SERVER )
	{
		char cDummy = 0;
		SendPacket( PI_GS_USER, 0, NULL, E_CHECK_SEQUENCE_COUNT_NOT, cDummy, false, false );
	}
}

void KUserProxy::ProcessEvent( IN KEvent* pkEvent_ )
{
	PostEvent( pkEvent_ );

	KSerializer     ks;
	ks.BeginReading( &pkEvent_->m_kbuff );
}

//template < typename T >
//bool KUserProxy::SendPacket( unsigned short usEventID, T& data, bool bLogging, bool bCompress )
//{
//    return KSession::SendPacket( PI_GS_USER, GetUID(), std::vector< UidType >(), usEventID, data, bLogging, bCompress );
//}
//
//bool KUserProxy::SendID( unsigned short usEvenID )
//{
//    return KSession::SendID( PI_GS_USER, GetUID(), std::vector< UidType >(), usEvenID );
//}