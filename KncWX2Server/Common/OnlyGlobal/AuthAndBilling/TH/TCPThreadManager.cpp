#include "TCPThreadManager.h"
#include "KncSend.h" 


//{{ ������ : [2012/10/9] //	�±� OTP ���� ��� ���
#ifdef SERV_AUTH_TCP_THREAD_MANAGER


ImplPfID( KTCPThreadManager, PI_NULL );    // ������ ��ü���� �ٸ� ���� ������ �Ѵ�. �׳� ��� x.

NiImplementRTTI( KTCPThreadManager, KThreadManager );
ImplToStringW( KTCPThreadManager )
{
	stm_ << ", Thread : " << GetThreadNum() << ", Event : " << GetQueueSize() << std::endl;
	//{{ 2009. 9. 16  ������	����

	KMaxQueueSize kInfo = GetMaxQueueSize();
	std::wstring wstrRegDate = ( CStringW )( kInfo.m_tRegTime.Format( _T( "%Y-%m-%d_%H%M%S" ) ) );
	stm_ << L"  MaxQueueSize : " << kInfo.m_QueueSize << ", RegDate : " << wstrRegDate << std::endl;
	//}}

    return stm_;
}

extern KThread* CreateTCPThread( const std::wstring& wstrIP, const short sPort );

KThread* KTCPThreadManager::CreateThread()
{
	return CreateTCPThread( m_wstrIP, m_sPort );
}

void KTCPThreadManager::SetTCPConnectionInfo( const std::wstring& wstrIP, const short sPort )
{
	m_wstrIP = wstrIP;
	m_sPort = sPort;
}

ImplOstmOperatorW( KTCPThreadManager );


#endif SERV_AUTH_TCP_THREAD_MANAGER
//}}