#include "BaseServer.h"
#include "HackingProtectionManager.h"


//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST


ImplementSingleton( KHackingProtectionManager );

KHackingProtectionManager::KHackingProtectionManager() :
m_iReleaseTick( 0 )
{
}

KHackingProtectionManager::~KHackingProtectionManager()
{
}

ImplToStringW( KHackingProtectionManager )
{
	return stm_;
}

void KHackingProtectionManager::Tick()
{
	if( m_tRefreshTimer.elapsed() <= 60.0 )
		return;

	m_tRefreshTimer.restart();

	KDBE_CHECK_HACKING_TOOL_LIST_REQ kPacketToDB;
	kPacketToDB.m_iReleaseTick = m_iReleaseTick;
	SendToLogDB( DBE_CHECK_HACKING_TOOL_LIST_REQ, kPacketToDB );
}

void KHackingProtectionManager::Init()
{
	KDBE_CHECK_HACKING_TOOL_LIST_REQ kPacketToDB;
	kPacketToDB.m_iReleaseTick = m_iReleaseTick;
	SendToLogDB( DBE_CHECK_HACKING_TOOL_LIST_REQ, kPacketToDB );
}

void KHackingProtectionManager::UpdateHackingToolList( IN const KDBE_CHECK_HACKING_TOOL_LIST_ACK& kInfo )
{
	// ReleaseTick�� ���ٸ� ����Ʈ�� ������� �������̹Ƿ� �ƹ��͵� ó������ �ʴ´�.
	if( m_iReleaseTick == kInfo.m_iReleaseTick )
	{
		return;
	}
	else if( m_iReleaseTick > kInfo.m_iReleaseTick )
	{
		START_LOG( cerr, L"ReleaseTick���� �� �۾�����. �Ͼ�� ���� ����!" )
			<< BUILD_LOG( m_iReleaseTick )
			<< BUILD_LOG( kInfo.m_iReleaseTick )
			<< END_LOG;

		return;
	}

	// ReleaseTick�� ������Ʈ�Ѵ�!
	m_iReleaseTick = kInfo.m_iReleaseTick;

	// ��ŷ �� ����Ʈ�� ������Ʈ �Ѵ�!
	m_vecHackingToolList.clear();
	m_vecHackingToolList = kInfo.m_vecHackingToolList;

	START_LOG( cout, L"��ŷ �� ����Ʈ�� ������Ʈ �Ǿ����ϴ�!" )
		<< BUILD_LOG( m_vecHackingToolList.size() );
}

void KHackingProtectionManager::GetHackingToolList( OUT int& iRandomKey, OUT std::vector< KHackingToolInfo >& vecHackingToolList )
{
	vecHackingToolList.clear();

	vecHackingToolList = m_vecHackingToolList;
	iRandomKey = rand(); // ������ Ű�� ����
}

void KHackingProtectionManager::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}


#endif SERV_HACKING_TOOL_LIST
//}}


