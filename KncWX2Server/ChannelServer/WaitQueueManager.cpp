#include "ChannelSimLayer.h"
#include "BaseServer.h"

#include "WaitQueueManager.h"



//{{ 2012. 07. 02	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM


ImplementRefreshSingleton( KWaitQueueManager );

KWaitQueueManager::KWaitQueueManager(void) : 
m_eMode( WQ_NONE ),
m_iTotalCCU( 0 ),
m_iPassCount( 0 ),
m_iTotalCCULimit( 0 ),
m_fSendTimeGap( 0.0 ),
m_bSendGetCCU( false )
{
}

KWaitQueueManager::~KWaitQueueManager(void)
{
}

ImplToStringW( KWaitQueueManager )
{
	return stm_;
}

ImplementLuaScriptParser( KWaitQueueManager )
{
	lua_tinker::class_add<KWaitQueueManager>( GetLuaState(), "KWaitQueueManager" );
	lua_tinker::class_def<KWaitQueueManager>( GetLuaState(), "SetMode",					&KWaitQueueManager::SetMode_LUA );
	lua_tinker::class_def<KWaitQueueManager>( GetLuaState(), "SetSendTimeGap",			&KWaitQueueManager::SetSendTimeGap_LUA );
	lua_tinker::class_def<KWaitQueueManager>( GetLuaState(), "SetTotalCCULimit",		&KWaitQueueManager::SetTotalCCULimit_LUA );
	lua_tinker::class_def<KWaitQueueManager>( GetLuaState(), "dump",					&KWaitQueueManager::Dump );

	lua_tinker::decl( GetLuaState(), "WQ_NONE",			KWaitQueueManager::WQ_NONE );
	lua_tinker::decl( GetLuaState(), "WQ_CCU_LIMIT",	KWaitQueueManager::WQ_CCU_LIMIT );
	lua_tinker::decl( GetLuaState(), "WQ_ENTRY_BAN",	KWaitQueueManager::WQ_ENTRY_BAN );

	lua_tinker::decl( GetLuaState(), "WaitQueueManager", this );
}

void KWaitQueueManager::SetMode_LUA( int iMode )
{
    m_eMode = static_cast<WQ_ENUM>(iMode);

	START_LOG( cout, L"��⿭ ���" )
		<< BUILD_LOG( m_eMode );
}

void KWaitQueueManager::SetSendTimeGap_LUA( double fTimeGap )
{
	m_fSendTimeGap = fTimeGap;

	START_LOG( cout, L"���� ���� �������� �ð� ����" )
		<< BUILD_LOG( fTimeGap );
}

void KWaitQueueManager::SetTotalCCULimit_LUA( int iServerGroupID, int iCCU )
{
	if( iServerGroupID != KBaseServer::GetKObj()->GetServerGroupID() )
		return;

	m_iTotalCCULimit = iCCU;

	START_LOG( cout, L"��⿭ ���� : �ִ� ���� ���� �ο� ����!" )
		<< BUILD_LOG( iServerGroupID )
		<< BUILD_LOG( m_iTotalCCULimit );
}

void KWaitQueueManager::Tick()
{
	if( m_kTimer.elapsed() < m_fSendTimeGap )
		return;

	m_kTimer.restart();

	// �̹� ���� ��û�� ���³�?
	if( m_bSendGetCCU == true )
	{
		START_LOG( cout, L"���� ���� ��û�ð��� 1�ʸ� �Ѿ���!" );
		return;
	}

	// 1�ʸ��� ��ü ���� ��������
	SendToLogDB( DBE_GET_TOTAL_CCU_FOR_WAIT_QUEUE_REQ );

	m_bSendGetCCU = true;
}

void KWaitQueueManager::SetTotalCCU( IN const int iTotalCCU )
{
	m_bSendGetCCU = false;	

    m_iTotalCCU = iTotalCCU;

	// ��� ������ �ο��� ����!
	const int iPassCountCalc = GetTotalCCULimit() - ( GetTotalCCU() + GetCountPassedUser() );
	if( iPassCountCalc > 0 )
	{
        m_iPassCount = iPassCountCalc;
	}
}

bool KWaitQueueManager::IsNextEnterUser( IN const UidType iUID )
{
	if( m_deqWaitUserList.empty() )
		return false;

	// ���� ��� ������ ���� �ο����� ����Ѱ�?
	if( m_iPassCount <= 0 )
		return false;

	const bool bRet = ( m_deqWaitUserList.front() == iUID );
	if( bRet )
	{
		// ��� ������ ���� ���� �Ѹ� ���δ�.
		--m_iPassCount;
	}

    return bRet;
}

bool KWaitQueueManager::RegWaitUser( IN const UidType iUID )
{
    if( IsExist( iUID ) == true )
	{
		START_LOG( cerr, L"�̹� ��ϵ� ���� �Դϴ�!" )
			<< BUILD_LOG( iUID )
			<< END_LOG;
		return false;
	}

	// ���� ���
	m_deqWaitUserList.push_back( iUID );
	m_setWaitUserList.insert( iUID );
	return true;
}

bool KWaitQueueManager::UnRegWaitUser( IN const UidType iUID )
{
    if( IsExist( iUID ) == false )
	{
		START_LOG( clog, L"��ϵ��� ���� ���� �Դϴ�!" )
			<< BUILD_LOG( iUID )
			<< END_LOG;
		return false;
	}

	std::deque< UidType >::iterator dit;
	for( dit = m_deqWaitUserList.begin(); dit != m_deqWaitUserList.end(); ++dit )
	{
		if( *dit == iUID )
			break;
	}
 
	if( dit != m_deqWaitUserList.end() )
	{
		m_deqWaitUserList.erase( dit );
	}
	m_setWaitUserList.erase( iUID );
	return true;
}

bool KWaitQueueManager::GetWaitUserCount( IN const UidType iUID, OUT int& iWaitUserCount ) const
{
	iWaitUserCount = 0;

	if( IsExist( iUID ) == false )
		return false;

	BOOST_TEST_FOREACH( const UidType, iRegUID, m_deqWaitUserList )
	{
		if( iRegUID == iUID )
			break;

		++iWaitUserCount;
	}
	
	return true;
}

void KWaitQueueManager::SendToLogDB( IN const unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}

#endif SERV_WAIT_QUEUE_SYSTEM
//}}



