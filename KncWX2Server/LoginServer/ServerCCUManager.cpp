#include "LoginSimLayer.h"
#include "BaseServer.h"

#include ".\serverccumanager.h"



#ifdef SERV_CCU_NEW


ImplementSingleton( KServerCCUManager );

KServerCCUManager::KServerCCUManager(void)
{
	//m_tNextUpdateTime = CTime::GetCurrentTime();
	
	//{{ 2011.10.12 ��ȿ�� ���� ���� ���� ���� �� ��ü ���� �������� ���⵵�� ����
#ifdef SERV_FIX_CCU_REPORT
	m_tLastUpdateTime = CTime::GetCurrentTime();
#endif SERV_FIX_CCU_REPORT
	//}}

#ifdef SERV_COUNTRY_CN
	m_iUpdatedMinute = -1;
#endif SERV_COUNTRY_CN
}

KServerCCUManager::~KServerCCUManager(void)
{
}

ImplToStringW( KServerCCUManager )
{
	return stm_;
}

void KServerCCUManager::Tick()
{
#ifdef SERV_FIX_CCU_REPORT
	//{{ 2011.10.12 ��ȿ�� ���� ���� ���� ���� �� ��ü ���� �������� ���⵵�� ���� ( �ӱԼ� �Ϻ� ���� )
	if( m_kTimer[TM_CCU_UPDATE].elapsed() <= 40.0 )	// �ð� ���� ���̴°� �߿�
		return;	
	//}}
	// DB�� ���� ���� ������Ʈ
	DBUpdateCCUInfo();
#else SERV_FIX_CCU_REPORT
	if( m_kTimer[TM_CCU_UPDATE].elapsed() > 60.0 )
	{
		// DB�� ���� ���� ������Ʈ
		DBUpdateCCUInfo();

		// Ÿ�̸� �ʱ�ȭ
		m_kTimer[TM_CCU_UPDATE].restart();
	}
#endif SERV_FIX_CCU_REPORT

	//{{ 2012. 07. 10	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
	if( m_kTimer[TM_FOR_WAIT_QUEUE].elapsed() > 1.0 )
	{
		CTime tCurrentTime = CTime::GetCurrentTime();
		KDBE_UPDATE_CCU_INFO_NOT kPacket;
		std::map< UidType, KUpdateCCUInfo >::const_iterator mit;
		for( mit = m_mapCCUInfo.begin(); mit != m_mapCCUInfo.end(); ++mit )
		{
			kPacket.m_vecCCUInfo.push_back( mit->second );
		}
		kPacket.m_wstrRegDate = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M" ) ) );
		SendToLogDB( DBE_UPDATE_CCU_INFO_NOT, kPacket );

        m_kTimer[TM_FOR_WAIT_QUEUE].restart();
	}
#endif SERV_WAIT_QUEUE_SYSTEM
	//}}
}

void KServerCCUManager::DBUpdateCCUInfo()
{
	// ���� �ð� -> DB�� ���� �ð� ������ ����!
	// ���� �����Ǵ� ��Ȳ���δ� CTime�� ���װ� �־ ����� �ð� ������ ���־��ְ� �ִٴ����� ���� �����ѵ�..
#ifdef SERV_FIX_CCU_REPORT
	m_kTimer[TM_CCU_UPDATE].restart(); // -- Ÿ�̸ӿ� ���װ� �ִٰ� �����Ͽ����� Ÿ�̸Ӵ� �����ΰ� ����.
#endif SERV_FIX_CCU_REPORT

	CTime tCurrentTime = CTime::GetCurrentTime();

#ifdef SERV_FIX_CCU_REPORT
	//{{ 2011.10.12 ��ȿ�� ���� ���� ���� ���� �� ��ü ���� �������� ���⵵�� ����
	// ���� �ð��� �ߺ����� �Է��ϴ� ���� ���� ���ؼ�
	if( tCurrentTime.GetMinute() == m_tLastUpdateTime.GetMinute() ){
		return;
	}

	m_tLastUpdateTime = tCurrentTime;
	//}}
#endif SERV_FIX_CCU_REPORT

#ifdef SERV_COUNTRY_CN
	//     �̹� DB�� ������Ʈ �ߴ��� �˻�
	if( m_iUpdatedMinute == tCurrentTime.GetMinute() )
		return;
#endif SERV_COUNTRY_CN

	// ������Ʈ ��Ŷ �����
	KDBE_UPDATE_CCU_INFO_NOT kPacket;

#ifdef SERV_FIX_CCU_REPORT
	//{{ 2011.10.12 ��ȿ�� ���� ���� ���� ���� �� ��ü ���� �������� ���⵵�� ����
	KUpdateCCUInfo kTotalCCUInfo;
	kTotalCCUInfo.m_iServerUID = 0;
	//}}
#else SERV_FIX_CCU_REPORT
	//KUpdateCCUInfo kTotalCCUInfo;
	//kTotalCCUInfo.m_iServerUID = 0;
#endif SERV_FIX_CCU_REPORT

	std::map< UidType, KUpdateCCUInfo >::const_iterator mit;
	for( mit = m_mapCCUInfo.begin(); mit != m_mapCCUInfo.end(); ++mit )
	{
		kPacket.m_vecCCUInfo.push_back( mit->second );

#ifdef SERV_FIX_CCU_REPORT
		//{{ 2011.10.12 ��ȿ�� ���� ���� ���� ���� �� ��ü ���� �������� ���⵵�� ����
		kTotalCCUInfo.m_iConcurrentUser		+= mit->second.m_iConcurrentUser;
		kTotalCCUInfo.m_iCurPartyCount		+= mit->second.m_iCurPartyCount;
		kTotalCCUInfo.m_iPlayGamePartyCount += mit->second.m_iPlayGamePartyCount;
		//}}
#else SERV_FIX_CCU_REPORT
		//kTotalCCUInfo.m_iConcurrentUser		+= mit->second.m_iConcurrentUser;
		//kTotalCCUInfo.m_iCurPartyCount		+= mit->second.m_iCurPartyCount;
		//kTotalCCUInfo.m_iPlayGamePartyCount += mit->second.m_iPlayGamePartyCount;
#endif SERV_FIX_CCU_REPORT

#ifdef SERV_CHANNELING_USER_MANAGER
		if( !mit->second.m_mapChannelingUserList.empty() )
			UpdateChannelingUserInfo( mit->second.m_mapChannelingUserList , kTotalCCUInfo );
#endif SERV_CHANNELING_USER_MANAGER
	}

#ifdef SERV_FIX_CCU_REPORT
	//{{ 2011.10.12 ��ȿ�� ���� ���� ���� ���� �� ��ü ���� �������� ���⵵�� ����
	kPacket.m_vecCCUInfo.push_back( kTotalCCUInfo );
	//}}
#else SERV_FIX_CCU_REPORT
	//kPacket.m_vecCCUInfo.push_back( kTotalCCUInfo );
#endif SERV_FIX_CCU_REPORT

	// ü��ID ����
	if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_NEXON_KOREA )
	{
		KUpdateCCUInfo kGuestCCUInfo;
		kGuestCCUInfo.m_iServerUID			= KBaseServer::GetKObj()->GetUID();
		kGuestCCUInfo.m_iConcurrentUser		= GetKLoginSimLayer()->GetGuestConcurrentUserCount(); // ü��ID ����
		kGuestCCUInfo.m_iCurPartyCount		= 0;
		kGuestCCUInfo.m_iPlayGamePartyCount = 0;
		kPacket.m_vecCCUInfo.push_back( kGuestCCUInfo );
	}

	// ���� ������Ʈ �ð��� ���� �ð��� ���̰� ���ٸ� �������� ����!
	//if( tCurrentTime.GetMinute() != m_tNextUpdateTime.GetMinute() )
	//{
	//	kPacket.m_wstrRegDate = ( CStringW )( m_tNextUpdateTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	//	SendToLogDB( DBE_UPDATE_CCU_INFO_NOT, kPacket );

	//	START_LOG( cout, L"���� ���� ������Ʈ Ÿ�� 1�� ���� �߻�! �������� ������Ʈ �մϴ�!" )
	//		<< BUILD_LOG( tCurrentTime.GetMinute() )
	//		<< BUILD_LOG( m_tNextUpdateTime.GetMinute() )
	//		<< BUILD_LOG( kPacket.m_wstrRegDate );
	//}
	
	// ������Ʈ ��Ŷ
	kPacket.m_wstrRegDate = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M" ) ) );
	SendToLogDB( DBE_UPDATE_CCU_INFO_NOT, kPacket );

	// ���� ������ ����.
	m_mapCCUInfo.clear();

	// ���� ������Ʈ �ð� ����
	//m_tNextUpdateTime = tCurrentTime += CTimeSpan( 0, 0, 1, 0 );

#ifdef SERV_COUNTRY_CN
	// ���� ������Ʈ�� �ð� ����
	m_iUpdatedMinute = tCurrentTime.GetMinute();
#endif SERV_COUNTRY_CN
}

void KServerCCUManager::UpdateCCUInfo( IN const KUpdateCCUInfo& kInfo )
{
	std::map< UidType, KUpdateCCUInfo >::iterator mit;
	mit = m_mapCCUInfo.find( kInfo.m_iServerUID );
	if( mit == m_mapCCUInfo.end() )
	{
		m_mapCCUInfo.insert( std::make_pair( kInfo.m_iServerUID, kInfo ) );
#ifdef SERV_COUNTRY_CN
		m_mapLastCCUInfo[kInfo.m_iServerUID] = kInfo;
#endif SERV_COUNTRY_CN
	}
	else
	{
        mit->second = kInfo;
	}
}

#ifdef SERV_CHANNELING_USER_MANAGER
void KServerCCUManager::UpdateChannelingUserInfo( IN std::map<int,int> m_mapChannelingUserList , OUT KUpdateCCUInfo& kTotalCCUInfo )
{
	std::map<int,int>::const_iterator mitCUL;
	for( mitCUL = m_mapChannelingUserList.begin(); mitCUL != m_mapChannelingUserList.end(); ++mitCUL )
	{
		std::map<int,int>::iterator mitTotal;
		mitTotal = kTotalCCUInfo.m_mapChannelingUserList.find( mitCUL->first );

		if( mitTotal ==  kTotalCCUInfo.m_mapChannelingUserList.end() )
		{
			kTotalCCUInfo.m_mapChannelingUserList.insert( std::make_pair( mitCUL->first, mitCUL->second ) );
		}
		else
		{
			mitTotal->second += mitCUL->second;
		}		
	}

}
#endif SERV_CHANNELING_USER_MANAGER

void KServerCCUManager::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}


#endif SERV_CCU_NEW

