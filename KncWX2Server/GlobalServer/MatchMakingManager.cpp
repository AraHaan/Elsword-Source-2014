#include "MatchMakingManager.h"
#include "NetError.h"

#include "GlobalServer.h"
#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "X2Data/XSLRoom.h"
#include "MatchFSM.h"
#include "MatchUserFSM.h"
#include "GlobalScriptRefreshManager.h"
#include "CompareLimitList.h"
//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch/PvpMatchManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}


//{{ 2011. 06. 14	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM



//////////////////////////////////////////////////////////////////////////
//{{ 2012. 02. 21	��μ�	���� ���� ��� �ð� ǥ�� ����
#ifdef SERV_PVP_MATCH_WAIT_TIME
KMatchCompleteTimeManager::KMatchCompleteTimeManager()
{
}

KMatchCompleteTimeManager::~KMatchCompleteTimeManager()
{
}

void KMatchCompleteTimeManager::AddCompleteTime( IN const int iCompleteTime )
{
	KMatchTime kCompleteTime;
	kCompleteTime.m_tMatchTime = CTime::GetCurrentTime();
	kCompleteTime.m_iWatiTime = iCompleteTime;

	m_listCompleteTime.push_back( kCompleteTime );

	// 5���� �ɶ����� �������� �����.
	while( CTE_MAX_COUNT < m_listCompleteTime.size() )
	{
		m_listCompleteTime.pop_front();
	}
}

int	KMatchCompleteTimeManager::GetMatchMakingEstimatedTime() const
{
	// �ѹ��� ��ġ�� ���������� ����
	if( m_listCompleteTime.empty() )
		return 2000; // Ŭ���̾�Ʈ���� 30�� �̻��̸� '�˼� ����'���� ǥ���Ѵ�

	int iSumCompleteTime = 0;												// �ֱ� ���� ��� �ð� �� ��
	int iEstimatedTime	 = 0;												// �ֱ� ���� ��� �ð� ���
	int	iSumCount		 = 0;												// ��� �߰��� Ƚ��
	CTime tCurr = CTime::GetCurrentTime();									// ���� �ð�
	CTime tTwoHourAgo = tCurr - CTimeSpan( 0, 2, 0, 0 );					// ���� �ð����� �νð� �� �ð�

	BOOST_TEST_FOREACH( const KMatchTime, kCompleteTime, m_listCompleteTime )
	{
		if( tTwoHourAgo <= kCompleteTime.m_tMatchTime )	// �ֱ� 2�ð� �̳� ���
		{
			iSumCompleteTime += kCompleteTime.m_iWatiTime;
			++iSumCount;
		}
	}

	if( iSumCount <= 0 )	// ��꿡 �߰� �� Ƚ���� ���� -> ��Ī ���� ������ ������ 2�ð��� ���� �����̹Ƿ� ������
	{
		return 2000; // Ŭ���̾�Ʈ���� 30�� �̻��̸� '�˼� ����'���� ǥ���Ѵ�
	}

	iEstimatedTime = ( iSumCompleteTime / iSumCount );
	const int iMiddleResult = iEstimatedTime % 10; // 10�ʴ��� �ø�
	if( iMiddleResult > 0 )
	{
		iEstimatedTime = ( iEstimatedTime - iMiddleResult ) + 10;			// ��հ��� �ø��Ѵ�.
	}

	return iEstimatedTime;
}
#else
KMatchCompleteTimeManager::KMatchCompleteTimeManager()
{
}

KMatchCompleteTimeManager::~KMatchCompleteTimeManager()
{
}

void KMatchCompleteTimeManager::AddCompleteTime( IN const int iCompleteTime )
{
	m_listCompleteTime.push_back( iCompleteTime );

	// 5���� �ɶ����� �������� �����.
	while( COMPLETE_TIME_ENUM::CTE_MAX_COUNT < m_listCompleteTime.size() )
	{
		m_listCompleteTime.pop_front();
	}
}

int	KMatchCompleteTimeManager::GetMatchMakingEstimatedTime() const
{
	// �ѹ��� ��ġ�� ���������� ����
	if( m_listCompleteTime.empty() )
		return 10; // ����Ʈ�� 10��

	const int iCompleteCount = static_cast<int>(m_listCompleteTime.size());
	int iSumCompleteTime = 0;

	BOOST_TEST_FOREACH( const int, iTime, m_listCompleteTime )
	{
		iSumCompleteTime += iTime;
	}

	int iEstimatedTime = ( iSumCompleteTime / iCompleteCount );
	const int iMiddleResult = iEstimatedTime % 10; // 10�ʴ��� �ø�
	if( iMiddleResult > 0 )
	{
		iEstimatedTime = ( iEstimatedTime - iMiddleResult ) + 10;
	}

	return iEstimatedTime;
}
#endif SERV_PVP_MATCH_WAIT_TIME
//}}
//////////////////////////////////////////////////////////////////////////


#define CLASS_TYPE KMatchMakingManager
ImplPfID( KMatchMakingManager, PI_CN_PARTY );

ImplementSingleton( KMatchMakingManager );


FSMclassPtr KMatchMakingManager::ms_spUserFSM = KMatchUserFSMPtr( new KMatchUserFSM );


KMatchMakingManager::KMatchMakingManager() :
m_iLastMatchUID( 0 ),
m_iLastWaitNumber( 0 ),
m_iRematchCount( 0 )
{
	lua_tinker::class_add<KMatchMakingManager>( g_pLua, "KMatchMakingManager" );
	lua_tinker::class_def<KMatchMakingManager>( g_pLua, "Command",					&KMatchMakingManager::Command_LUA );
	lua_tinker::class_def<KMatchMakingManager>( g_pLua, "dump",						&KMatchMakingManager::Dump );

	lua_tinker::decl( g_pLua, "MatchManager", this );
}

KMatchMakingManager::~KMatchMakingManager()
{
}

ImplToStringW( KMatchMakingManager )
{
	stm_ << L"-------------- Match Making Manager --------------" << std::endl
		<< TOSTRINGW( m_iLastMatchUID )
		<< TOSTRINGW( m_mapMatch.size() )
		<< TOSTRINGW( m_iLastWaitNumber )
		<< TOSTRINGW( m_mapMatchWaitUser.size() )
		<< TOSTRINGW( m_mapMatchWaitList.size() )
		<< TOSTRINGW( m_mapMatchWaitParty.size() )
		<< TOSTRINGW( m_iRematchCount )
		;

	for( int iEnum = 0; iEnum < SEnum::MT_MAX; ++iEnum )
	{
		stm_ << L"------------------------------------------" << std::endl
			<< TOSTRINGW( SEnum::GetMatchTypeName( iEnum ) )
			<< TOSTRINGW( m_listMatchWaitList[iEnum].size() )
			<< TOSTRINGW( m_kCompleteTimeManager[iEnum].GetListSize() )
			;
	}

	return stm_;
}

void KMatchMakingManager::Command_LUA( char cType )
{
    START_LOG( cout, L"��ġ ����ŷ ������ ���� Ŀ�ǵ�!" )
		<< BUILD_LOGc( cType );

	KEGB_MATCH_MAKING_ADMIN_COMMAND_NOT kPacket;
	kPacket.m_cType = cType;
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GLOBAL_MATCH_MAKING, NULL, EGB_MATCH_MAKING_ADMIN_COMMAND_NOT, kPacket );
	QueueingEventToMatch( spEvent );
}

void KMatchMakingManager::Run()
{
	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 20 );       // 0.02s ����

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Tick();

		else std::cout << "*** WaitForSingleObject() - return : " << ret << std::endl;

	}

	return;
}

void KMatchMakingManager::ShutDown()
{
	End();
}

bool KMatchMakingManager::Init()
{
	return KThread::Begin();
}

void KMatchMakingManager::ClearAllMatchInfo()
{
	//////////////////////////////////////////////////////////////////////////
	START_LOG( cout2, L"[���!] ��� ��ġ ������ �ʱ�ȭ ��ŵ�ϴ�!" );
	//////////////////////////////////////////////////////////////////////////	
	// ��ġ ���� ó��!
	std::vector< UidType > vecMatchList;
	for( u_int uiMatchType = 0; uiMatchType < SEnum::MT_MAX; ++uiMatchType )
	{
		BOOST_TEST_FOREACH( KMatchPtr, spMatch, m_vecMatch[uiMatchType] )
		{
			if( !spMatch )
				continue;

			vecMatchList.push_back( spMatch->GetMatchUID() );
		}
	}

	BOOST_TEST_FOREACH( const UidType, iMatchUID, vecMatchList )
	{
		DestroyMatch( iMatchUID );
	}


	//////////////////////////////////////////////////////////////////////////
	// ����� ����Ʈ ���� ó��
	std::vector< UidType > vecUnitUIDLIst;
	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMatchWaitUser.begin(); mit != m_mapMatchWaitUser.end(); ++mit )
	{
		vecUnitUIDLIst.push_back( mit->first );
	}

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUnitUIDLIst )
	{
		UnRegMatchWaitListByUnitUID( iUnitUID, NetError::NOT_LEAVE_MATCH_WAIT_REASON_01 );
	}
}

void KMatchMakingManager::Tick()
{
	// MatchMakingManager�̺�Ʈ�� ���� ó���ѵ�..
	KPerformer::Tick();

	std::vector< UidType > vecDeleteMatchUIDList;

	// ��ġ Tick�� ó������!	
	for( u_int uiMatchType = 0; uiMatchType < SEnum::MT_MAX; ++uiMatchType )
	{
		BOOST_TEST_FOREACH( KMatchPtr, spMatch, m_vecMatch[uiMatchType] )
		{
			if( spMatch == NULL )
				continue;

			spMatch->Tick();

			// ������ ���°� S_CLOSE��� ���� ����Ʈ�� MatchUID�� ����!
			if( spMatch->GetStateID() == KMatchFSM::S_CLOSE )
			{
				vecDeleteMatchUIDList.push_back( spMatch->GetUID() );
			}
		}
	}

	// ��ġ ���� ó��!
	BOOST_TEST_FOREACH( const UidType, iMatchUID, vecDeleteMatchUIDList )
	{
        DestroyMatch( iMatchUID );
	}

	// ��ġ ����ŷ
	for( u_int uiMatchType = 0; uiMatchType < SEnum::MT_MAX; ++uiMatchType )
	{
		//{{ 2012. 09. 17	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
#else
		// �̹� ������ ��ġ���� ���� ��ġ ����ŷ
		MatchMakingForCreatedMatch( static_cast<SEnum::MATCH_TYPE>(uiMatchType) );
#endif SERV_2012_PVP_SEASON2
		//}}

		// ������� ����ڵ��� ���� ��ġ ����ŷ
		MatchMakingForWaitUser( static_cast<SEnum::MATCH_TYPE>(uiMatchType) );
	}
}

KMatchPtr KMatchMakingManager::MakeMatch( IN const SEnum::MATCH_TYPE eMatchType )
{
	KMatchPtr spMatch( new KMatch( ++m_iLastMatchUID, eMatchType ) );
	if( spMatch == NULL )
	{
		START_LOG( cerr, L"KCnPartyPtr new �Ҵ����." )
			<< BUILD_LOG( m_iLastMatchUID )
			<< END_LOG;

		return KMatchPtr();
	}

	// FSM����
	FSMclassPtr spFSMClass = boost::shared_ptr< KMatchFSM >( new KMatchFSM );
	spMatch->SetFSM( spFSMClass );
	return spMatch;
}

KMatchUserPtr KMatchMakingManager::MakeUser( IN const SEnum::MATCH_TYPE eMatchType, IN const KMatchUserInfo& kInfo )
{
	KMatchUserPtr spMatchUser( new KMatchUser( kInfo ) );
	if( spMatchUser == NULL )
	{
		START_LOG( cerr, L"��ġ ���� ���� ����!" )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;

		return KMatchUserPtr();
	}

	spMatchUser->SetFSM( ms_spUserFSM );
	spMatchUser->SetMatchType( eMatchType );
	return spMatchUser;
}

KMatchUserPtr KMatchMakingManager::GetMatchWaitUserByUnitUID( IN const UidType iUnitUID )
{
	std::map< UidType, KMatchUserPtr >::iterator mit = m_mapMatchWaitUser.find( iUnitUID );
	if( mit == m_mapMatchWaitUser.end() )
	{
		START_LOG( cwarn, L"ã�� ��ġ������ �����ϴ�.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KMatchUserPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"��ġ ���� ������ �̻�.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KMatchUserPtr();
	}

	return mit->second;
}

KMatchUserWaitListPtr KMatchMakingManager::GetMatchWaitListByWaitNumber( IN const int iWaitNumber )
{
	std::map< int, KMatchUserWaitListPtr >::iterator mit = m_mapMatchWaitList.find( iWaitNumber );
	if( mit == m_mapMatchWaitList.end() )
	{
		START_LOG( cwarn, L"ã�� ��ġ ����� ����Ʈ�� �����ϴ�.!" )
			<< BUILD_LOG( iWaitNumber )
			<< END_LOG;

		return KMatchUserWaitListPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"��ġ ��ġ ����� ����Ʈ ������ �̻�.!" )
			<< BUILD_LOG( iWaitNumber )
			<< END_LOG;

		return KMatchUserWaitListPtr();
	}

	return mit->second;
}

KMatchUserWaitListPtr KMatchMakingManager::GetMatchWaitListByPartyUID( IN const UidType iPartyUID )
{
	std::map< UidType, KMatchUserWaitListPtr >::iterator mit = m_mapMatchWaitParty.find( iPartyUID );
	if( mit == m_mapMatchWaitParty.end() )
	{
		START_LOG( cwarn, L"ã�� ��ġ ����� ����Ʈ�� �����ϴ�.!" )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;

		return KMatchUserWaitListPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"��ġ ��ġ ����� ����Ʈ ������ �̻�.!" )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;

		return KMatchUserWaitListPtr();
	}

	return mit->second;
}

KMatchPtr KMatchMakingManager::GetMatch( IN UidType iMatchUID )
{
	std::map< UidType, KMatchPtr >::iterator mit = m_mapMatch.find( iMatchUID );
	if( mit == m_mapMatch.end() )
	{
		START_LOG( cwarn, L"ã�� ��ġ�� �����ϴ�.!" )
			<< BUILD_LOG( iMatchUID )
			<< END_LOG;

		return KMatchPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"��ġ ������ �̻�.!" )
			<< BUILD_LOG( iMatchUID )
			<< END_LOG;

		return KMatchPtr();
	}

	return mit->second;
}

KMatchPtr KMatchMakingManager::CreateMatch( IN const SEnum::MATCH_TYPE eMatchType, IN const std::vector< KMatchUserInfo >& vecMatchUserInfo )
{
	KMatchPtr spMatch = MakeMatch( eMatchType );
	if( spMatch == NULL )
	{
		START_LOG( cerr, L"��ġ ���� ����!" )
			<< BUILD_LOG( eMatchType )
			<< END_LOG;

		return KMatchPtr();
	}

	// ��ġ ���� �ʱ�ȭ
	spMatch->Enter( vecMatchUserInfo );

	// ��ġ ���
	m_mapMatch.insert( std::make_pair( spMatch->GetUID(), spMatch ) );
	m_vecMatch[eMatchType].push_back( spMatch );

	// ��ġ�� ó�� �����Ǹ� ��ġ ����ŷ �����̴�!
	spMatch->StateTransition( KMatchFSM::I_TO_MATCH_MAKING );
    return spMatch;
}

void KMatchMakingManager::DestroyMatch( IN const UidType iMatchUID )
{
	KMatchPtr spMatch = GetMatch( iMatchUID );
	if( spMatch == NULL )
	{
		START_LOG( cerr, L"�����Ϸ��� ��ġ ������ �����ϴ�!" )
			<< BUILD_LOG( iMatchUID )
			<< END_LOG;
		return;
	}

	const SEnum::MATCH_TYPE eMatchType = spMatch->GetMatchType();	

	std::vector< KMatchPtr >::iterator vit;
	for( vit = m_vecMatch[eMatchType].begin(); vit != m_vecMatch[eMatchType].end(); ++vit )
	{
		KMatchPtr spMatch = *vit;
		if( spMatch == NULL )
			continue;

		if( spMatch->GetUID() == iMatchUID )
			break;
	}

	if( vit != m_vecMatch[eMatchType].end() )
	{
		m_vecMatch[eMatchType].erase( vit );
	}

	m_mapMatch.erase( iMatchUID );
}

void KMatchMakingManager::QueueingEventToMatch( IN const KEventPtr& spEvent_ )
{
	UidType iMatchUID = 0;
	switch( spEvent_->m_usEventID )
	{
	case EGB_REQUEST_MATCH_MAKING_REQ:
	case EGB_CANCEL_MATCH_MAKING_REQ:
	case EPM_CHECK_INVALID_USER_NOT:
	case ESR_ORDER_TO_REFRESH_MANAGER_REQ:
		QueueingEvent( spEvent_ ); // ���� ������ ���õ� ó���� RoomManager Thread���� ó���ؾ� �ϹǷ� ���⼭ �ٷ� ó�� ���ϰ� event queueing�Ѵ�.
		return;

	default:
		iMatchUID = spEvent_->m_kDestPerformer.GetFirstUID();
		break;
	}

	KMatchPtr spMatch = GetMatch( iMatchUID );
	if( spMatch == NULL )
	{
		START_LOG( cwarn, L"��ġ �˻� ����." )
			<< BUILD_LOG( iMatchUID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
		return;
	}

	// ��ġ�� event����
	spMatch->QueueingEvent( spEvent_ );
}

void KMatchMakingManager::QueueingToAll( IN const KEventPtr& spEvent )
{
	std::map< UidType, KMatchPtr >::iterator mit;
	for( mit = m_mapMatch.begin(); mit != m_mapMatch.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"��Ƽ ������ �̻�" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		mit->second->QueueingEvent( spEvent );
	}
}

bool KMatchMakingManager::RegMatchWaitList( IN const SEnum::MATCH_TYPE eType, IN const std::vector< KMatchUserInfo >& vecMatchUserList, IN const bool bRegFrontOfList )
{
	// 1. ��ġ �ο� ���� �´��� üũ!
	if( KMatch::GetMatchPlayerCount( eType ) < (int)vecMatchUserList.size() )
	{
		START_LOG( cerr, L"��ġ ����� ����Ʈ ���� ����!" )
			<< BUILD_LOG( eType )
			<< BUILD_LOG( KMatch::GetMatchPlayerCount( eType ) )
			<< BUILD_LOG( vecMatchUserList.size() )
			<< END_LOG;
		return false;
	}

	// 2. ��ġ ����� ����Ʈ�� ���� �մϴ�.
	KMatchUserWaitListPtr spWaitList( new KMatchUserWaitList( ++m_iLastWaitNumber ) );
	if( spWaitList == NULL )
	{
		START_LOG( cerr, L"��ġ ����� ����Ʈ ���� ����!" )
			<< BUILD_LOG( eType )
			<< END_LOG;
		return false;
	}
	
	// 3. ��ġ ����� ����Ʈ�� ��û�ڵ��� �ϳ��� �ֽ��ϴ�!
	BOOST_TEST_FOREACH( const KMatchUserInfo, kInfo, vecMatchUserList )
	{
		// �̹� ��ϵ� ĳ���� ���� �˻�
		if( IsExistInWaitListByUnitUID( kInfo.m_iUnitUID ) == true )
		{
			START_LOG( cerr, L"�̹� ��ġ�� ��ϵǾ��ִ� ����!" )
				<< BUILD_LOG( eType )
				<< BUILD_LOG( kInfo.m_iUnitUID )
				<< END_LOG;
			return false;
		}

		// �̹� ��ϵ� ��Ƽ���� �˻�
		if( kInfo.m_iPartyUID != 0 )
		{
			if( IsExistInWaitListByPartyUID( kInfo.m_iPartyUID ) == true )
			{
				START_LOG( cerr, L"�̹� ��ϵ� ��Ƽ ���� �Դϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
					<< BUILD_LOG( eType )
					<< BUILD_LOG( kInfo.m_iPartyUID )
					<< END_LOG;
				return false;
			}
		}

        KMatchUserPtr spMatchUser = MakeUser( eType, kInfo );
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"����� ����Ʈ ���� �� ��ġ ���� ���� ����!" )
				<< BUILD_LOG( eType )
				<< BUILD_LOG( kInfo.m_iUnitUID )
				<< END_LOG;
			return false;
		}

		// ����� ����Ʈ�� ��ġ ������ �������!
		spWaitList->AddMatchUser( spMatchUser );

		// ��ü ���� ����Ʈ���� �������!
		m_mapMatchWaitUser.insert( std::make_pair( spMatchUser->GetCharUID(), spMatchUser ) );
	}

	if( spWaitList->GetNumMember() == 0 )
	{
		START_LOG( cerr, L"����� ����Ʈ�� �������� ���Ͽ����ϴ�." )
			<< BUILD_LOG( eType )
			<< END_LOG;
		return false;
	}

	// 4. �ش� Ÿ���� ����ڸ���Ʈ�� �ֽ��ϴ�!
	if( bRegFrontOfList )
	{
		// ���û�̶�� ����Ʈ �ֻ����� ����!
		m_listMatchWaitList[eType].push_front( spWaitList );
	}
	else
	{
		// ���ο� ��û�̶�� ����Ʈ �������� ����!
		m_listMatchWaitList[eType].push_back( spWaitList );
	}

	// 5. ����ȣ�� key���ϴ� �����̳ʿ��� �ֽ��ϴ�!
	m_mapMatchWaitList.insert( std::make_pair( spWaitList->GetMatchWaitNumber(), spWaitList ) );

	// 6. PartyUID�� �����Ѵٸ�?
	const UidType iPartyUID = spWaitList->GetPartyUID();
	if( iPartyUID != 0 )
	{
		// �̹� ��ϵ� ��Ƽ������ ������ �̹� �� �˻� �ߴ�!
		m_mapMatchWaitParty.insert( std::make_pair( iPartyUID, spWaitList ) );
	}
	
	return true;
}

bool KMatchMakingManager::UnRegMatchWaitListByUnitUID( IN const UidType iUnitUID, IN const int iUnRegReason, IN const std::wstring& wstrCancelUnitNickName /*= L""*/ )
{
	KMatchUserPtr spMatchUser = GetMatchWaitUserByUnitUID( iUnitUID );
	if( spMatchUser == NULL )
	{
		START_LOG( cerr, L"����� ����Ʈ ��������� �Ϸ��� �ϴµ� ������ ã�� ���Ͽ����ϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	// ��ġ Ÿ���� ���� ����!	
	const SEnum::MATCH_TYPE eMatchType = static_cast<SEnum::MATCH_TYPE>(spMatchUser->GetMatchType());
	if( eMatchType < 0  ||  eMatchType > SEnum::MT_MAX )
	{
		START_LOG( cerr, L"��ġ Ÿ���� �̻��մϴ�! ���� �Ͼ�� ���� ����!" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( eMatchType )
			<< BUILD_LOG( iUnRegReason )
			<< END_LOG;
		return false;
	}

	// �ش� ��ġ ���� ����
    m_mapMatchWaitUser.erase( iUnitUID );

	// ����� ����Ʈ������ ����
	std::list< KMatchUserWaitListPtr >::iterator lit = m_listMatchWaitList[eMatchType].begin();
	for( ; lit != m_listMatchWaitList[eMatchType].end(); ++lit )
	{
		if( (*lit) == NULL )
		{
			START_LOG( cerr, L"����� ����Ʈ �����Ͱ��� �̻��մϴ�" )
				<< END_LOG;
			continue;
		}

        if( (*lit)->IsExistMatchUser( iUnitUID ) )
		{
            (*lit)->DelMatchUser( iUnitUID );
			break;
		}
	}

	if( lit != m_listMatchWaitList[eMatchType].end() )
	{
		if( (*lit)->GetNumMember() == 0 )
		{
			const int iMatchWaitNumber = (*lit)->GetMatchWaitNumber();
			m_listMatchWaitList[eMatchType].erase( lit );
            m_mapMatchWaitList.erase( iMatchWaitNumber );
		}
	}
	else
	{
		START_LOG( cerr, L"����� ����Ʈ�� ��ġ������ ����! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
	}

	// ��� ó���� ������!
	KEGS_UNREG_MATCH_WAIT_LIST_NOT kPacketNot;
	kPacketNot.m_iReason = iUnRegReason;
	kPacketNot.m_wstrCancelUnitNickName = wstrCancelUnitNickName;
	spMatchUser->SendToGSCharacter( EGB_UNREG_MATCH_WAIT_LIST_NOT, kPacketNot );

	// ��ġ�� �����ؼ� ����� ����Ʈ���� �����°Ŷ�� ��ġ ���� ��踦 ������!
	if( iUnRegReason == NetError::NOT_LEAVE_MATCH_WAIT_REASON_00 )
	{
		m_kCompleteTimeManager[eMatchType].AddCompleteTime( static_cast<int>(spMatchUser->GetMatchCompleteSeconds()) );
	}
	return true;
}

bool KMatchMakingManager::UnRegMatchWaitListByWaitNumber( IN const int iWaitNumber, IN const int iUnRegReason, IN const std::wstring& wstrCancelUnitNickName /*= L""*/ )
{
	KMatchUserWaitListPtr spMatchWaitList = GetMatchWaitListByWaitNumber( iWaitNumber );
	if( spMatchWaitList == NULL )
	{
		START_LOG( cerr, L"����� ����Ʈ ��������� �Ϸ��� �ϴµ� ����Ʈ�� ã�� ���Ͽ����ϴ�." )
			<< BUILD_LOG( iWaitNumber )
			<< END_LOG;
		return false;
	}

	// ��ƼUID���� ���� ��Ƽ�����̳ʺ��� ��������!
	const UidType iPartyUID = spMatchWaitList->GetPartyUID();
	if( iPartyUID != 0 )
	{
		if( IsExistInWaitListByPartyUID( iPartyUID ) == false )
		{
			START_LOG( cerr, L"PartyUID�� ����ڸ���Ʈ�� ã�� ���ߴ�? ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( iWaitNumber )
				<< BUILD_LOG( NetError::GetErrStr( iUnRegReason ) )
				<< BUILD_LOG( wstrCancelUnitNickName )
				<< BUILD_LOG( iPartyUID )
				<< END_LOG;
			// ���� ó���Ұ� ���ڱ�..
		}
		else
		{
			// ��Ƽ �����̳ʿ��� ����!
			m_mapMatchWaitParty.erase( iPartyUID );
		}
	}

	// ĳ���� ����Ʈ�� �� �ϳ��� �����������!
	std::vector< UidType > vecUnitUIDList;
	spMatchWaitList->GetUnitUIDList( vecUnitUIDList );

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUnitUIDList )
	{
		UnRegMatchWaitListByUnitUID( iUnitUID, iUnRegReason, wstrCancelUnitNickName );
	}
	
	return true;
}

bool KMatchMakingManager::UnRegMatchWaitListByPartyUID( IN const UidType iPartyUID, IN const int iUnRegReason, IN const UidType iRequestCancelUnitUID )
{
	KMatchUserWaitListPtr spWaitList = GetMatchWaitListByPartyUID( iPartyUID );
	if( spWaitList == NULL )
	{
		START_LOG( cerr, L"��Ż ��û�� PartyUID�� ��ϵǾ� ���� �ʳ�?" )
			<< BUILD_LOG( iPartyUID )
			<< BUILD_LOG( NetError::GetErrStr( iUnRegReason ) )
			<< BUILD_LOG( iRequestCancelUnitUID )
			<< END_LOG;
		return false;
	}

	// �ش� PartyUID�� �´� ĳ���Ͱ� �־����� ����!
	KMatchUserPtr spCancelMatchUser = spWaitList->GetMatchUser( iRequestCancelUnitUID );
	if( spCancelMatchUser == NULL )
	{
		START_LOG( cerr, L"��Ż ��û�� PartyUID�� �ش�Ǵ� ĳ���Ͱ� ����? �̰� ó���ϸ� �ȵǰڴ�." )
			<< BUILD_LOG( iPartyUID )
			<< BUILD_LOG( NetError::GetErrStr( iUnRegReason ) )
			<< BUILD_LOG( iRequestCancelUnitUID )
			<< END_LOG;
		return false;
	}
	
	return UnRegMatchWaitListByWaitNumber( spWaitList->GetMatchWaitNumber(), iUnRegReason, spCancelMatchUser->GetNickName() );
}

//{{ 2012. 09. 15	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
bool KMatchMakingManager::FindTheOpponent( IN const SEnum::MATCH_TYPE& eType,
										   IN const KMatchUserWaitListPtr& spFrontMatchWaitUser,
										   IN const std::list<KMatchUserWaitListPtr>& listMatchMakingWaitList,
										   OUT std::vector<int>& vecSelectedWaitUserList )
{
	const int CHECK_WAIT_USER_COUNT = SiKPvpMatchManager()->GetCheckWaitUserCount();
	int iCheckListIndex = 0;

	vecSelectedWaitUserList.clear();

	// listSortedWaitUserList�� +- 200, +- 400, ArrangeUser�� ����
	std::list<KMatchUserWaitListPtr> listUnderTheRatingGab200;
	std::list<KMatchUserWaitListPtr> listUnderTheRatingGab400;
	std::list<KMatchUserWaitListPtr> listUnderTheRatingGabArrange;

	const int iUpperRating200 = 200;
	const int iUpeerRating400 = 400;
	
	// ��ġ ����� ����Ʈ�� ����� ��� F��ũ���� �˻�!
	const bool bIsAllMemberBeginner = spFrontMatchWaitUser->IsAllPlayersBeginner();

	BOOST_TEST_FOREACH( KMatchUserWaitListPtr, spTargetUserList, listMatchMakingWaitList )
	{
		// �丸ŭ�� üũ����!
		if( iCheckListIndex > CHECK_WAIT_USER_COUNT )
			break;

		++iCheckListIndex;

		if( spTargetUserList == NULL )
		{
			START_LOG( cerr, L"����� ����Ʈ �����Ͱ��� �̻��մϴ�." )
				<< END_LOG;
			continue;
		}

		// ��ġ ����ŷ �غ� �Ǿ����� Ȯ���Ѵ�!
		if( spTargetUserList->IsPossibleMatchMakingBegin() == false )
			continue;
		
		int iOptimalMatchPoint = 0;

		// ������ ��ġ �������� üũ!
		if( spFrontMatchWaitUser->CheckOptimalMatch( spTargetUserList, iOptimalMatchPoint ) == false )
			continue;

		// ��밡 ��ġ ��Ŀ���,
		if(	spTargetUserList->IsAllPlayersArrangeRank() == true )
		{
			// Rating 400 �̳���
			if( iOptimalMatchPoint <= iUpeerRating400 )
			{
				listUnderTheRatingGabArrange.push_back( spTargetUserList );
			}
		}
		else
		{
			if( iOptimalMatchPoint <= iUpperRating200 )
			{
				listUnderTheRatingGab200.push_back( spTargetUserList );
			}
			else if( iOptimalMatchPoint <= iUpeerRating400 )
			{
				listUnderTheRatingGab400.push_back( spTargetUserList );
			}
		}
	}

	// üũ�� ��ġ �ο� ��
	int iSumMatchPlayerCount = spFrontMatchWaitUser->GetNumMember();
	int iMinRating = spFrontMatchWaitUser->GetRatingWeightAverage();
	int iMaxRating = spFrontMatchWaitUser->GetRatingWeightAverage();

	int i2NumPartyCount = ( spFrontMatchWaitUser->GetNumMember() == 2 ) ? 1 : 0;
	if( _CheckTheMemberNum( eType, bIsAllMemberBeginner, listUnderTheRatingGab200, iSumMatchPlayerCount, iMinRating, iMaxRating, i2NumPartyCount, vecSelectedWaitUserList ) == true )
	{
		return true;
	}

	if( _CheckTheMemberNum( eType, bIsAllMemberBeginner, listUnderTheRatingGab400, iSumMatchPlayerCount, iMinRating, iMaxRating, i2NumPartyCount, vecSelectedWaitUserList ) == true )
	{
		return true;
	}

	if( _CheckTheMemberNum( eType, bIsAllMemberBeginner, listUnderTheRatingGabArrange, iSumMatchPlayerCount, iMinRating, iMaxRating, i2NumPartyCount, vecSelectedWaitUserList ) == true )
	{
		return true;
	}

	return false;
}

bool KMatchMakingManager::_CheckTheMemberNum( IN const SEnum::MATCH_TYPE& eType,
											  IN const bool bIsAllMemberBeginner,
											  IN std::list<KMatchUserWaitListPtr>& listMatchMakingWaitList,
											  IN OUT int& iSumMatchPlayerCount,
											  IN OUT int& iMinRating,
											  IN OUT int& iMaxRating,
											  IN OUT int& i2NumPartyCount,
											  OUT std::vector<int>& vecSelectedWaitUserList )
{
	const int MATCH_TOTAL_PLAYER_COUNT = KMatch::GetMatchTotalPlayerCount( eType );
	const int GAP_MIN_MAX_RATING = SiKPvpMatchManager()->GetGapMinMaxRating();

	// ������ ����Ʈ�� �������� ��ġ �ο��� ���߾� �߷�����!
	while( listMatchMakingWaitList.empty() == false )
	{
		// ���� ���� ��Ī
		std::list<KMatchUserWaitListPtr>::iterator it = listMatchMakingWaitList.begin();
		for( int i=0, iMax = ( rand() % listMatchMakingWaitList.size() ); i < iMax; ++i )
		{
			++it;
		}

		KMatchUserWaitListPtr spTargetUserList = *it;
		listMatchMakingWaitList.erase( it );
		if( spTargetUserList == NULL )
		{
			START_LOG( cerr, L"����� ����Ʈ �����Ͱ��� �̻��մϴ�." )
				<< END_LOG;
			continue;
		}

		// ������� �´��� üũ!
		if( ( iSumMatchPlayerCount + spTargetUserList->GetNumMember() ) > MATCH_TOTAL_PLAYER_COUNT )
			continue;

		// 2������ ��û�� �������� �ݵ�� ���� ������ �����־�� �ϱ� ������
		// �ϳ��� ��Ī���� 2�� ��Ƽ�� �ִ� 2��Ƽ���� �����ϴ�.
		// 3�� ��Ƽ�� ��쿡�� ��ü ��Ī �ο� üũ���� �˻��ϰ� ������,
		// 1�� ��Ƽ�� ��쿡�� ������ ����� �Ѵ�.
		if( spTargetUserList->GetNumMember() == 2 )
		{
			if( i2NumPartyCount == 2 )
			{
				continue;
			}
			else if( 2 < i2NumPartyCount )
			{
				START_LOG( cerr, L"�ϳ��� ��Ī���� 2�� ��Ƽ�� ���� �̹� 2���� �Ѿ���ϴ�.")
					<< BUILD_LOG( i2NumPartyCount )
					<< END_LOG;
				return false;
			}
			++i2NumPartyCount;
		}

		// ��ġ ����� ��� F��ũ�� �ٸ� ������ F��ũ�θ� ������ ����� ����Ʈ�� ��ġ ��Ų��!
		if( bIsAllMemberBeginner != spTargetUserList->IsAllPlayersBeginner() )
		{
			continue;
		}

		// ��ġ ����ڵ��� �ּ� rating�� �ִ� rating���� ���̰� �ʹ� ū�� �ƴ��� üũ�Ѵ�!
		int iCheckMinRating = iMinRating;
		int iCheckMaxRating = iMaxRating;

		if( iMinRating > spTargetUserList->GetRatingWeightAverage() )
		{
			iCheckMinRating = spTargetUserList->GetRatingWeightAverage();
		}
		else if( iMaxRating < spTargetUserList->GetRatingWeightAverage() )
		{
			iCheckMaxRating = spTargetUserList->GetRatingWeightAverage();
		}

		if( abs( iCheckMaxRating - iCheckMinRating ) > GAP_MIN_MAX_RATING )
		{
			continue;
		}

		iMinRating = iCheckMinRating;
		iMaxRating = iCheckMaxRating;

		iSumMatchPlayerCount += spTargetUserList->GetNumMember();
		vecSelectedWaitUserList.push_back( spTargetUserList->GetMatchWaitNumber() );

		// ��� �ο��� �� ä�������� ��Ż!
		if( iSumMatchPlayerCount == MATCH_TOTAL_PLAYER_COUNT )
			return true;
	}

	// ��� �ο��� �غ�Ǿ����� �ο����� üũ����!
	if( iSumMatchPlayerCount == MATCH_TOTAL_PLAYER_COUNT )
	{
		return true;
	}
	return false;
}
#else
void KMatchMakingManager::MatchMakingForCreatedMatch( IN const SEnum::MATCH_TYPE eType )
{
	const int MATCH_TOTAL_PLAYER_COUNT = KMatch::GetMatchTotalPlayerCount( eType );

	//////////////////////////////////////////////////////////////////////////	
    // ���� ��ġ���� ������ ����ִ� ��ġ�� ä��� ��ġ�� ���۽�Ű��!
	BOOST_TEST_FOREACH( KMatchPtr, spMatch, m_vecMatch[eType] )
	{
		if( spMatch == NULL )
		{
			START_LOG( cerr, L"��ġ �����Ͱ��� �̻��մϴ�." )
				<< END_LOG;
			continue;
		}

		// 1. ��ġ ����ŷ ������ ��ġ�� üũ�Ѵ�!
		if( spMatch->GetStateID() != KMatchFSM::S_MATCH_MAKING )
			continue;

		// 2. ��� ������� F��ũ���� �˻�
		const bool bIsAllMemberBeginner = spMatch->IsAllPlayersSameRank( CXSLUnit::PVPRANK_RANK_F );

		int iBestOptimalMatchPoint = INT_MAX; // ������ ��ġ ���� ���
		KMatchUserWaitListPtr spSelectedWaitList;

		// 3. �ش� ��ġ�� ������ ������ ����ڸ� ã�Ƽ� ���� ����!
		BOOST_TEST_FOREACH( KMatchUserWaitListPtr, spWaitList, m_listMatchWaitList[eType] )
		{
			if( spWaitList == NULL )
			{
				START_LOG( cerr, L"����� ����Ʈ �����Ͱ��� �̻��մϴ�." )
					<< END_LOG;
				continue;
			}

			// ��ġ ����ŷ �غ� �Ǿ����� Ȯ���Ѵ�!
			if( spWaitList->IsPossibleMatchMakingBegin() == false )
				continue;

			// �ο����� �´��� üũ����!
			if( ( spMatch->GetNumMember() + spWaitList->GetNumMember() ) > MATCH_TOTAL_PLAYER_COUNT )
				continue;

			// ��ġ�� ������� ��� F��ũ�� �ٸ� ����� F��ũ���߸� �Ѵ�.
			if( bIsAllMemberBeginner )
			{
				if( spWaitList->IsAllPlayersSameRank( CXSLUnit::PVPRANK_RANK_F ) == false )
					continue;
			}
			else
			{
				if( spWaitList->IsAllPlayersSameRank( CXSLUnit::PVPRANK_RANK_F ) == true )
					continue;
			}

			int iOptimalMatchPoint = 0;

			// ������ ��ġ �������� üũ!
			if( spMatch->CheckOptimalMatch( spWaitList, iOptimalMatchPoint ) == false )
				continue;

			// �׷��ٸ� ����!
			if( iOptimalMatchPoint < iBestOptimalMatchPoint )
			{
				iBestOptimalMatchPoint = iOptimalMatchPoint;
                spSelectedWaitList = spWaitList;
			}
		}

		// ���õ� ������ ��ġ ���� ������ ���ٸ� �н�!
		if( spSelectedWaitList == NULL )
			continue;

		// ��ġ�� ������ ���� ���� ����!
		std::vector< KMatchUserInfo > vecEnterMatchUser;
		spSelectedWaitList->GetMatchUserInfoList( vecEnterMatchUser );

		// ����� ����Ʈ���� ����!
		LIF( UnRegMatchWaitListByWaitNumber( spSelectedWaitList->GetMatchWaitNumber(), NetError::NOT_LEAVE_MATCH_WAIT_REASON_00 ) );

		// ��ġ�� ����!
		if( spMatch->Enter( vecEnterMatchUser ) == false )
		{
			START_LOG( cerr, L"������� �Դµ� ���� ���еƴ�. �հ� �߸��Ǿ���.." )
				<< BUILD_LOG( spMatch->GetMatchUID() )
				<< BUILD_LOG( vecEnterMatchUser.size() )
				<< END_LOG;
			continue;
		}

		// ���������� ��ġ�� ����Ǿ����� Ȯ������!
		if( spMatch->CheckMatchMakingSuccess() == false )
		{
            START_LOG( clog, L"��ġ ���� ����!" )
				<< BUILD_LOG( spMatch->GetMatchUID() )
				<< END_LOG;
		}
		else
		{
			// �� ��ġ ���� ī��Ʈ!
			++m_iRematchCount;
		}
	}
}
#endif SERV_2012_PVP_SEASON2
//}}

void KMatchMakingManager::MatchMakingForWaitUser( IN const SEnum::MATCH_TYPE eType )
{
	//{{ 2012. 09. 17	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	// �ʿ�� �ϴ� �Լ� ���ο��� ����Ѵ�.
	const int MATCH_MAKING_BEGIN_COUNT = SiKPvpMatchManager()->GetMatchMakingBeginCount();
#else
	//////////////////////////////////////////////////////////////////////////	
	// �״����� ���� ����ڵ��� ������ ���ο� ��ġ�� ������!	
	const int MATCH_TOTAL_PLAYER_COUNT = KMatch::GetMatchTotalPlayerCount( eType );
	const int MATCH_MAKING_BEGIN_COUNT = SiKPvpMatchManager()->GetMatchMakingBeginCount();
	const int CHECK_WAIT_USER_COUNT = SiKPvpMatchManager()->GetCheckWaitUserCount();
	const int GAP_MIN_MAX_RATING = SiKPvpMatchManager()->GetGapMinMaxRating();
#endif SERV_2012_PVP_SEASON2
	//}}

	// ����� ����Ʈ�� �����ؿ���!
	std::list< KMatchUserWaitListPtr > listMatchMakingWaitList = m_listMatchWaitList[eType];	

	// ��� ������ ��ġ ����ŷ�� �Ѵ�!
	const u_int uiTotalListSize = m_listMatchWaitList[eType].size();
	u_int uiLoopCount = 0;

	while( listMatchMakingWaitList.empty() == false )
	{
		++uiLoopCount;
		if( uiLoopCount > uiTotalListSize )
		{
			START_LOG( cerr, L"�߸��� ������ �Ͽ����ϴ�! ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( uiLoopCount )
				<< BUILD_LOG( uiTotalListSize )
				<< END_LOG;
			break;
		}


		//////////////////////////////////////////////////////////////////////////
		// 1. ��ġ�� ����� �ִ� �ο������� ����ڼ��� ���� ��� ���̻� üũ�� �ʿ䰡 ����!
		//////////////////////////////////////////////////////////////////////////
		if( (int)listMatchMakingWaitList.size() < MATCH_MAKING_BEGIN_COUNT )
			break;


		//////////////////////////////////////////////////////////////////////////		
		// 2. ����� ����Ʈ���� �� ó������ �ϳ� ������.
		//////////////////////////////////////////////////////////////////////////		
		KMatchUserWaitListPtr spFrontMatchWaitUser = listMatchMakingWaitList.front();
		listMatchMakingWaitList.pop_front();
		if( spFrontMatchWaitUser == NULL )
		{
			START_LOG( cerr, L"��ġ ����� ����Ʈ �����Ͱ��� �̻��մϴ�." )
				<< END_LOG;
			continue;
		}


		//////////////////////////////////////////////////////////////////////////		
		// 3. ��ġ ����ŷ�� �����ص� �Ǵ��� �˻�!
		//////////////////////////////////////////////////////////////////////////		
		if( spFrontMatchWaitUser->IsPossibleMatchMakingBegin() == false )
			continue;

		//{{ 2012. 09. 16	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		// üũ�� ��ġ �ο� ��
		/*
		�ּ� ��Ī �ο� ���� 4������ ������ �� 4���� �ο��� ��Ī�� ��û�ϸ�, �翬�� 4�� �߿��� ��Ī�� �̷���� �Ŷ��
		����������, ������ ���� �κ� ������ ����� ���� �������� ��û�� ������ �����ϰ� ��Ī�ϵ��� �����ϰ� �־���.

		// ��ġ ����ŷ �غ� �Ǿ����� Ȯ���Ѵ�!
		if( spTargetUserList->IsPossibleMatchMakingBegin() == false )
			continue;

		��ġ ����ŷ ��û �� ���� �ð��� �����߸� ��Ī �ĺ��� �� �� �ִٴ� �����̴�.
		*/
		std::vector< int > vecSelectedWaitUserList;
		if( FindTheOpponent( eType, spFrontMatchWaitUser, listMatchMakingWaitList, vecSelectedWaitUserList ) == false )
		{
			continue;
		}
#else
		//////////////////////////////////////////////////////////////////////////		
		// 4. ��ġ ����� ����Ʈ�� ����� ��� F��ũ���� �˻�!
		//////////////////////////////////////////////////////////////////////////		
		const bool bIsAllMemberBeginner = spFrontMatchWaitUser->IsAllPlayersSameRank( CXSLUnit::PVPRANK_RANK_F );


		//////////////////////////////////////////////////////////////////////////
		// 4. ������ ��ġ ���� ������ �����Ѵ�.
		//////////////////////////////////////////////////////////////////////////
		KCompareLimitList< KMatchUserWaitListPtr > listOptimalMatchWaitList( CHECK_WAIT_USER_COUNT ); // ������ ����� ����Ʈ�� ����!
		int iCheckListIndex = 0;
		
		BOOST_TEST_FOREACH( KMatchUserWaitListPtr, spTargetUserList, listMatchMakingWaitList )
		{
			// �丸ŭ�� üũ����!
			if( iCheckListIndex > CHECK_WAIT_USER_COUNT )
				break;

			++iCheckListIndex;

			if( spTargetUserList == NULL )
			{
				START_LOG( cerr, L"����� ����Ʈ �����Ͱ��� �̻��մϴ�." )
					<< END_LOG;
				continue;
			}

			// ��ġ ����ŷ �غ� �Ǿ����� Ȯ���Ѵ�!
			if( spTargetUserList->IsPossibleMatchMakingBegin() == false )
				continue;

			// ��ġ ����� ��� F��ũ�� �ٸ� ������ F��ũ�θ� ������ ����� ����Ʈ�� ��ġ ��Ų��!
			if( bIsAllMemberBeginner )
			{
				if( spTargetUserList->IsAllPlayersSameRank( CXSLUnit::PVPRANK_RANK_F ) == false )
                    continue;
			}
			else
			{
				if( spTargetUserList->IsAllPlayersSameRank( CXSLUnit::PVPRANK_RANK_F ) == true )
					continue;
			}

			int iOptimalMatchPoint = 0;

			// ������ ��ġ �������� üũ!
			if( spFrontMatchWaitUser->CheckOptimalMatch( spTargetUserList, iOptimalMatchPoint ) == false )
				continue;

            // �׷��ٸ� ����!
			listOptimalMatchWaitList.IfUpperValuePushBack( iOptimalMatchPoint, spTargetUserList );
		}


		//////////////////////////////////////////////////////////////////////////		
		// 5. ���ĵ� ����Ʈ�� �������� ��ġ �ο��� ���߾� �߷�����!
		//////////////////////////////////////////////////////////////////////////		
		std::list< KMatchUserWaitListPtr > listSortedWaitUserList;
		listOptimalMatchWaitList.GetValueList( listSortedWaitUserList );


		// üũ�� ��ġ �ο� ��		
		std::vector< int > vecSelectedWaitUserList;
		int iSumMatchPlayerCount = spFrontMatchWaitUser->GetNumMember();
		int iMinRating = spFrontMatchWaitUser->GetRatingAverage();
		int iMaxRating = spFrontMatchWaitUser->GetRatingAverage();
		
		BOOST_TEST_FOREACH( KMatchUserWaitListPtr, spTargetUserList, listSortedWaitUserList )
		{
			if( spTargetUserList == NULL )
			{
				START_LOG( cerr, L"����� ����Ʈ �����Ͱ��� �̻��մϴ�." )
					<< END_LOG;
				continue;
			}

			// ������� �´��� üũ!
			if( ( iSumMatchPlayerCount + spTargetUserList->GetNumMember() ) > MATCH_TOTAL_PLAYER_COUNT )
				continue;

			iSumMatchPlayerCount += spTargetUserList->GetNumMember();
			vecSelectedWaitUserList.push_back( spTargetUserList->GetMatchWaitNumber() );

			if( iMinRating > spTargetUserList->GetRatingAverage() )
				iMinRating = spTargetUserList->GetRatingAverage();

			if( iMaxRating < spTargetUserList->GetRatingAverage() )
				iMaxRating = spTargetUserList->GetRatingAverage();

			// ��� �ο��� �� ä�������� ��Ż!
			if( iSumMatchPlayerCount == MATCH_TOTAL_PLAYER_COUNT )
				break;
		}


		//////////////////////////////////////////////////////////////////////////
		// 6. ��� �ο��� �غ�Ǿ����� �ο����� üũ����!
		//////////////////////////////////////////////////////////////////////////
		if( iSumMatchPlayerCount < MATCH_TOTAL_PLAYER_COUNT )
		{
			continue;
		}


		//////////////////////////////////////////////////////////////////////////
		// 7. ��ġ ����ڵ��� �ּ� rating�� �ִ� rating���� ���̰� �ʹ� ū�� �ƴ��� üũ�Ѵ�!
		//////////////////////////////////////////////////////////////////////////
		const int iGapMinMaxRating = ::abs( iMaxRating - iMinRating );
		if( iGapMinMaxRating > GAP_MIN_MAX_RATING )
		{
			continue;
		}
#endif SERV_2012_PVP_SEASON2
		//}}

		//////////////////////////////////////////////////////////////////////////
		// 8. ������ ��ġ ���� ������ �ο����� ���߾� �ϳ��� ��ġ�� �ϼ���Ų��.
		//////////////////////////////////////////////////////////////////////////
		{
			std::vector< KMatchUserInfo > vecSourceUserInfo;
			spFrontMatchWaitUser->GetMatchUserInfoList( vecSourceUserInfo );

			// ����� ����Ʈ���� ��ġ ��û�� ����!
			if( UnRegMatchWaitListByWaitNumber( spFrontMatchWaitUser->GetMatchWaitNumber(), NetError::NOT_LEAVE_MATCH_WAIT_REASON_00 ) == false )
			{
				START_LOG( cerr, L"����� ����Ʈ���� ��ġ ��û�� ���� ����! �Ͼ� ������ �ȵǴ� ����!" )
					<< BUILD_LOG( eType )
					<< BUILD_LOG( spFrontMatchWaitUser->GetMatchWaitNumber() )
					<< END_LOG;
				break;
			}

			// ���ο� ��ġ ����!
			KMatchPtr spNewMatch = CreateMatch( eType, vecSourceUserInfo );
			if( spNewMatch == NULL )
			{
				START_LOG( cerr, L"���ο� ��ġ ���� ����!" )
					<< BUILD_LOG( eType )
					<< END_LOG;
				break;
			}

			// ������ ������ ��ġ�� ���� ó��!
			BOOST_TEST_FOREACH( const int, iMatchWaitNumber, vecSelectedWaitUserList )
			{
                KMatchUserWaitListPtr spWaitList = GetMatchWaitListByWaitNumber( iMatchWaitNumber );
				if( spWaitList == NULL )
				{
					START_LOG( cerr, L"����� ����Ʈ�� �������� �ʴ´�! ����ͼ� �̷��� ���.. �Ͼ�� �ȵǴ� ����!" )
						<< BUILD_LOG( eType )
						<< BUILD_LOG( iMatchWaitNumber )
						<< END_LOG;
					break;
				}

				std::vector< KMatchUserInfo > vecNewEnterUser;
				spWaitList->GetMatchUserInfoList( vecNewEnterUser );

				// ����� ����Ʈ���� ��������!
				if( UnRegMatchWaitListByWaitNumber( iMatchWaitNumber, NetError::NOT_LEAVE_MATCH_WAIT_REASON_00 ) == false )
				{
					START_LOG( cerr, L"����� ����Ʈ���� ��ġ ��û�� ���� ����!" )
						<< BUILD_LOG( eType )
						<< BUILD_LOG( spWaitList->GetMatchWaitNumber() )
						<< END_LOG;
					break;
				}

				// ��ġ�� ���� ��Ű��!
				if( spNewMatch->Enter( vecNewEnterUser ) == false )
				{
					START_LOG( cerr, L"�߰��ο� ���� ����! ���󿡷��ΰ�?" )
						<< BUILD_LOG( vecNewEnterUser.size() )
						<< END_LOG;
					break;
				}

				// �̾Ƴ��� ����Ʈ���� ����
				std::list< KMatchUserWaitListPtr >::iterator litD = listMatchMakingWaitList.begin();
				for( ; litD != listMatchMakingWaitList.end(); ++litD )
				{
					if( (*litD) == NULL )
					{
						START_LOG( cerr, L"�����Ͱ��� �̻��մϴ�." )
							<< END_LOG;
						continue;
					}

					if( (*litD)->GetMatchWaitNumber() == iMatchWaitNumber )
						break;
				}

				if( litD != listMatchMakingWaitList.end() ) 
				{
					listMatchMakingWaitList.erase( litD );
				}
			}

			// ���������� ��ġ�� ����Ǿ����� Ȯ������!
			if( spNewMatch->CheckMatchMakingSuccess() == false )
			{
				START_LOG( clog, L"��Ī�� �����ߴ�!" )
					<< BUILD_LOG( spNewMatch->GetMatchUID() )
					<< END_LOG;
			}
		}
	}
}

void KMatchMakingManager::ProcessEvent( IN const KEventPtr& spEvent_ )
{
	switch( spEvent_->m_usEventID )
	{
        CASE( EGB_REQUEST_MATCH_MAKING_REQ );
		CASE( EGB_CANCEL_MATCH_MAKING_REQ );	   
	   _CASE( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT );
		CASE_NOPARAM( EGB_CHECK_INVALID_MATCH_NOT );
		CASE( EGB_MATCH_MAKING_ADMIN_COMMAND_NOT );

	default:
		START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
	}
}

IMPL_ON_FUNC( EGB_REQUEST_MATCH_MAKING_REQ )
{
	// ��ġ ����ŷ!
	KEGS_REQUEST_MATCH_MAKING_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	SEnum::MATCH_TYPE eMatchType;
	if( KMatch::GetMatchTypeByMatchInfo( kPacket_.m_cPvpGameType, kPacket_.m_cPvpPlayerCount, eMatchType ) == false )
	{
		START_LOG( cerr, L"�߸��� ��ġ ��û �����Դϴ�!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_MATCH_MAKING_15;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_REQUEST_MATCH_MAKING_ACK, kPacket );
		return;
	}

	// ��ġ ����� ����Ʈ�� ���!
	if( RegMatchWaitList( eMatchType, kPacket_.m_vecMatchUserInfo, kPacket_.m_bRegFrontOfList ) == false )
	{
		START_LOG( cerr, L"��ġ ����� ����Ʈ�� ����� �����Ͽ����ϴ�!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( eMatchType )
			<< BUILD_LOG( kPacket_.m_vecMatchUserInfo.size() )
			<< BUILD_LOG( kPacket_.m_bRegFrontOfList )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_MATCH_MAKING_15;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_REQUEST_MATCH_MAKING_ACK, kPacket );
		return;
	}

	// ��� ����
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_REQUEST_MATCH_MAKING_ACK, kPacket );

	// ��ġ ������ ���ؼ� �˸�
	BOOST_TEST_FOREACH( const KMatchUserInfo&, kInfo, kPacket_.m_vecMatchUserInfo )
	{
		KEGS_REG_MATCH_WAIT_LIST_SUCCESS_NOT kPacketNot;
		kPacketNot.m_iEstimatedTime = m_kCompleteTimeManager[eMatchType].GetMatchMakingEstimatedTime();
		SendToGSCharacter( kInfo.m_iGSUID, kInfo.m_iUnitUID, EGB_REG_MATCH_WAIT_LIST_SUCCESS_NOT, kPacketNot );
	}
}

IMPL_ON_FUNC( EGB_CANCEL_MATCH_MAKING_REQ )
{	
	// ��Ƽ �Ҽ��̸� ���� ��Ƽ���� �Բ� ��� ����!
	if( kPacket_.m_iPartyUID != 0 )
	{
		if( UnRegMatchWaitListByPartyUID( kPacket_.m_iPartyUID, kPacket_.m_iReason, FIRST_SENDER_UID ) == false )
		{
			START_LOG( cerr, L"PartyUID�� ����� ����Ʈ�� ���� ����!" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iPartyUID )
				<< END_LOG;
		}
	}

	// ����� ����Ʈ�� ��ϵǾ� �ִ��� Ȯ���Ѵ�!
	const bool bExistInWaitList = IsExistInWaitListByUnitUID( FIRST_SENDER_UID );
	if( bExistInWaitList == true )
	{
        if( UnRegMatchWaitListByUnitUID( FIRST_SENDER_UID, kPacket_.m_iReason ) == false )
		{
			START_LOG( cerr, L"����� ����Ʈ�� ���� ����!" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
		}

		if( kPacket_.m_iPartyUID != 0 )
		{
			START_LOG( cerr, L"�̹� PartyUID�� ���� �����ٵ�.. ���� �ֳ�? ����?" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iPartyUID )
				<< END_LOG;
		}
		
		if( kPacket_.m_iMatchUID != 0 )
		{
			START_LOG( cerr, L"����� ����Ʈ�� ��ϵǾ��ִµ� MatchUID���� ��ȿ�ϴ�! �����ε�?" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iMatchUID )
				<< END_LOG;
		}
	}

	// ��ġ ������ �������� �ʴ´ٸ�
	if( kPacket_.m_iMatchUID == 0 )
	{
		// �ý����� ���� ��Ŷ�� ��� ACK ���� ����.
		if( kPacket_.m_bSendBySystem == true )
			return;

		KEGS_CANCEL_MATCH_MAKING_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_CANCEL_MATCH_MAKING_ACK, kPacket );
		return;
	}

	KMatchPtr spMatch = GetMatch( kPacket_.m_iMatchUID );
	if( spMatch != NULL )
	{
		// ��ġ�� event����
		KEventPtr spEvent( new KEvent );
		UidType anTrace[2] = { FIRST_SENDER_UID, LAST_SENDER_UID };
		spEvent->SetData( PI_GLOBAL_MATCH_MAKING, anTrace, EGB_CANCEL_MATCH_MAKING_REQ, kPacket_ );
		spMatch->QueueingEvent( spEvent );
	}
}

_IMPL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	switch( kPacket_.m_iOrderType )
	{
		CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GB_PVP_MATCH_MANAGER, KPvpMatchManager );

	default:
		{
			START_LOG( cerr, L"�������� ���� �ȵǴ� Ÿ���ε�?" )
				<< BUILD_LOG( kPacket_.m_iOrderType )
				<< END_LOG;
		}
		break;
	}
}

IMPL_ON_FUNC_NOPARAM( EGB_CHECK_INVALID_MATCH_NOT )
{
    START_LOG( cout, L"GlobalUser��ü�� �Ҹ�Ǿ� ���� ��ġ ��û �Ǵ� ��ġ���� ��� ����ó���մϴ�!" )	
		<< END_LOG;

	ClearAllMatchInfo();
}

IMPL_ON_FUNC( EGB_MATCH_MAKING_ADMIN_COMMAND_NOT )
{
    START_LOG( cout, L"������ Ŀ�ǵ忡 ���� ���� ����!" )
		<< BUILD_LOGc( kPacket_.m_cType );

	switch( kPacket_.m_cType )
	{
	case KEGB_MATCH_MAKING_ADMIN_COMMAND_NOT::CT_DUMP:		
		{

		}
		break;

	case KEGB_MATCH_MAKING_ADMIN_COMMAND_NOT::CT_CLEAR:
		{
            ClearAllMatchInfo();
		}
		break;

	default:
		{
			START_LOG( cerr, L"�߸��� Ŀ�ǵ� �Դϴ�!" )
				<< BUILD_LOGc( kPacket_.m_cType )
				<< END_LOG;
		}
		break;
	}
}

#endif SERV_PVP_NEW_SYSTEM
//}}


