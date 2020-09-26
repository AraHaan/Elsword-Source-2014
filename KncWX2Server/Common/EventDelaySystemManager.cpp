#include "BaseServer.h"
#include "EventDelaySystemManager.h"
#include "GameSysVal/GameSysVal.h"


ImplementSingleton( KEventDelaySystemManager );

KEventDelaySystemManager::KEventDelaySystemManager()
{
}

KEventDelaySystemManager::~KEventDelaySystemManager()
{
}

ImplToStringW( KEventDelaySystemManager )
{
	return stm_;
}

void KEventDelaySystemManager::Tick()
{
	if( SiKGameSysVal()->IsEnableOpenRandomCubeDelaySystem() == false )
		return;

	// ��ϵ� ���� �ð���ŭ ť�� ������ ������Ų��.
	if( m_tTimer.elapsed() < SiKGameSysVal()->GetOpenRandomCubeDelayTime() )
		return;

	m_tTimer.restart();

	// ��ϵ� ī��Ʈ ��ŭ ���� ť�� ������ �õ��Ѵ�!
	for( int iCnt = 0; iCnt < SiKGameSysVal()->GetOpenRandomCubeDelayCount(); ++iCnt )
	{
		if( m_deqOpenRandomCubeEvent.empty() )
			break;

		// ����Ʈ���� �ϳ��� ������.
		KOpenRandomCubeEvent kEvent = m_deqOpenRandomCubeEvent.front();
		m_deqOpenRandomCubeEvent.pop_front();
		m_setRegUnitUID.erase( kEvent.m_iUnitUID );

		// ������ ��Ŷ OKó��
		kEvent.m_kEvent.m_bDelayedOK = true;

		// GSUser���� �ٽ� ������!
		SendToGSUser( kEvent.m_iUnitUID, EGS_OPEN_RANDOM_ITEM_REQ, kEvent.m_kEvent );
	}
}

void KEventDelaySystemManager::RegOpenRandomCubeEvent( IN const UidType iUnitUID, IN const KEGS_OPEN_RANDOM_ITEM_REQ& kEvent )
{
	if( IsRegRandomCubeEvent( iUnitUID ) )
	{
		// �̹� ��ϵǾ������� ���� �ִ��� ����� �ٽ� �������!
		UnRegOpenRandomCubeEvent( iUnitUID );
	}

	m_setRegUnitUID.insert( iUnitUID );
	m_deqOpenRandomCubeEvent.push_back( KOpenRandomCubeEvent( iUnitUID, kEvent ) );
}

void KEventDelaySystemManager::UnRegOpenRandomCubeEvent( IN const UidType iUnitUID )
{
    if( IsRegRandomCubeEvent( iUnitUID ) == false )
		return;

	m_setRegUnitUID.erase( iUnitUID );

	std::deque< KOpenRandomCubeEvent >::iterator dit;
	for( dit = m_deqOpenRandomCubeEvent.begin(); dit != m_deqOpenRandomCubeEvent.end(); ++dit )
	{
		if( dit->m_iUnitUID == iUnitUID )
			break;
	}

	if( dit != m_deqOpenRandomCubeEvent.end() )
	{
		m_deqOpenRandomCubeEvent.erase( dit );
	}	
}



