#include "AutoPartyMakingManager.h"
#include "NetError.h"

#include "GlobalServer.h"
#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "X2Data/XSLRoom.h"
#include "AutoPartyFSM.h"
#include "AutoPartyUserFSM.h"
#include "CompareLimitList.h"
#include "GlobalScriptRefreshManager.h"
//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	#include "..\Common\AutoParty\AutoPartyScriptManager.h"
#else
	#include "AutoPartyScriptManager.h"
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
//}}
#include "X2Data/XSLDungeonManager.h"


//{{ 2012. 02. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


//////////////////////////////////////////////////////////////////////////
#define CLASS_TYPE KAutoPartyMakingManager
ImplPfID( KAutoPartyMakingManager, PI_GLOBAL_AUTO_PARTY );

ImplementSingleton( KAutoPartyMakingManager );


KAutoPartyMakingManager::KAutoPartyMakingManager()
{
	lua_tinker::class_add<KAutoPartyMakingManager>( g_pLua, "KAutoPartyMakingManager" );
	lua_tinker::class_def<KAutoPartyMakingManager>( g_pLua, "Command",					&KAutoPartyMakingManager::Command_LUA );
	lua_tinker::class_def<KAutoPartyMakingManager>( g_pLua, "dump",						&KAutoPartyMakingManager::Dump );

	lua_tinker::decl( g_pLua, "AutoPartyManager", this );
}

KAutoPartyMakingManager::~KAutoPartyMakingManager()
{
}

ImplToStringW( KAutoPartyMakingManager )
{
	stm_ << L"-------------- Auto Party Making Manager --------------" << std::endl
		<< TOSTRINGW( m_mapAutoParty.size() )
		<< TOSTRINGW( m_vecAutoParty.size() )
		<< TOSTRINGW( m_mapAutoPartyMakingProcess.size() )
		;

	return stm_;
}

void KAutoPartyMakingManager::Command_LUA( char cType )
{
    START_LOG( cout, L"�ڵ� ��Ƽ ����ŷ ������ ���� Ŀ�ǵ�!" )
		<< BUILD_LOGc( cType );

	KEGB_AUTO_PARTY_MAKING_ADMIN_COMMAND_NOT kPacket;
	kPacket.m_cType = cType;
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GLOBAL_MATCH_MAKING, NULL, EGB_AUTO_PARTY_MAKING_ADMIN_COMMAND_NOT, kPacket );
	QueueingEvent( spEvent );
}

void KAutoPartyMakingManager::Run()
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

void KAutoPartyMakingManager::ShutDown()
{
	End();
}

bool KAutoPartyMakingManager::Init()
{
	// ��ũ��Ʈ �Ľ��� ����� �ȵǾ��ٸ� ���� ��Ű�� ����!
	const std::vector< KAutoPartyMakingProcessCondition >& vecAutoPartyDungeonInfo = SiKAutoPartyScriptManager()->GetAutoPartyDungeonInfo();
	if( vecAutoPartyDungeonInfo.empty() == true )
	{
		START_LOG( cerr, L"�ڵ� ��Ƽ ��ũ��Ʈ�� ����� �Ľ̵��� �ʾҽ��ϴ�! �ڵ���Ƽ �Ŵ����� ���۽�Ű�� �ʽ��ϴ�!" )
			<< END_LOG;
		return false;
	}

	// ���ÿ� ���� AutoPartyMakingProcess�� ����!
	BOOST_TEST_FOREACH( const KAutoPartyMakingProcessCondition&, kInfo, vecAutoPartyDungeonInfo )
	{
		KMakingProcessKey kKey( kInfo.m_iDungeonMode, kInfo.m_iDungeonIDWithDif );
		std::map< KMakingProcessKey, KAutoPartyMakingProcess >::iterator mit;
		mit = m_mapAutoPartyMakingProcess.find( kKey );
		if( mit == m_mapAutoPartyMakingProcess.end() )
		{
			m_mapAutoPartyMakingProcess.insert( std::make_pair( kKey, KAutoPartyMakingProcess( kInfo ) ) );
		}
		else
		{
			mit->second.AddLevelCondition( kInfo.m_iBeginLevel, kInfo.m_iEndLevel );
		}
	}

	// DungeonData.lua�� ��ϵǾ��ִ� �Ϲ� ���� ������ ����� �Ͽ� AutoPartyMakingProcess�� ��������!
	{
		std::vector< int > vecNormalDungeonIDList;
		SiCXSLDungeonManager()->GetNormalDungeonIDList( vecNormalDungeonIDList );

		BOOST_TEST_FOREACH( const int, iDungeonIDWithDif, vecNormalDungeonIDList )
		{
			// Ʃ�丮�� ����
			if( CXSLDungeon::IsTutorialDungeon( iDungeonIDWithDif ) == true )
				continue;

			//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
			const int iDifficultyLevel = iDungeonIDWithDif % 10;
			if( iDifficultyLevel == CXSLDungeon::DL_HARD  ||  iDifficultyLevel == CXSLDungeon::DL_EXPERT )
				continue;
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
			//}}

			KMakingProcessKey kKey( CXSLDungeon::DM_INVALID, iDungeonIDWithDif );
			std::map< KMakingProcessKey, KAutoPartyMakingProcess >::iterator mit;
			mit = m_mapAutoPartyMakingProcess.find( kKey );
			if( mit == m_mapAutoPartyMakingProcess.end() )
			{
				KAutoPartyMakingProcessCondition kInfo;
				kInfo.m_iDungeonMode = CXSLDungeon::DM_INVALID;
				kInfo.m_iDungeonIDWithDif = iDungeonIDWithDif;
				kInfo.m_iBeginLevel = 1;
				kInfo.m_iEndLevel = 60; // �ϴ��� �ϵ��ڵ������� ���� ��ũ��Ʈ ������ ���ܿ;���.
				m_mapAutoPartyMakingProcess.insert( std::make_pair( kKey, KAutoPartyMakingProcess( kInfo ) ) );
			}
			else
			{
				START_LOG( cerr, L"�Ϲݴ����ε� ������ DungeonID���� �����ؼ��� �ȵ˴ϴ�!" )
					<< BUILD_LOG( iDungeonIDWithDif )
					<< END_LOG;
			}
		}
	}

	return KThread::Begin();
}

void KAutoPartyMakingManager::ClearAllAutoPartyInfo()
{
	//////////////////////////////////////////////////////////////////////////
	START_LOG( cout2, L"[���!] ��� �ڵ� ��Ƽ ������ �ʱ�ȭ ��ŵ�ϴ�!" );
	//////////////////////////////////////////////////////////////////////////	

	// �ڵ� ��Ƽ ���� ó��!
	std::vector< UidType > vecAutoPartyList;
	BOOST_TEST_FOREACH( KAutoPartyPtr, spAutoParty, m_vecAutoParty )
	{
		if( !spAutoParty )
			continue;

		vecAutoPartyList.push_back( spAutoParty->GetAutoPartyUID() );
	}

	BOOST_TEST_FOREACH( const UidType, iAutoPartyUID, vecAutoPartyList )
	{
		DestroyAutoParty( iAutoPartyUID );
	}

	// ����� ����Ʈ ���� ó��
	std::map< KMakingProcessKey, KAutoPartyMakingProcess >::iterator mit;
	for( mit = m_mapAutoPartyMakingProcess.begin(); mit != m_mapAutoPartyMakingProcess.end(); ++mit )
	{
		mit->second.ClearAllAutoPartyInfo();
	}
}

void KAutoPartyMakingManager::Tick()
{
	//////////////////////////////////////////////////////////////////////////	
	// AutoPartyMakingManager�̺�Ʈ�� ���� ó���ѵ�..
	KPerformer::Tick();

	
	//////////////////////////////////////////////////////////////////////////	
	// �ڵ� ��Ƽ Tick�� ó������!	
	std::vector< UidType > vecDeleteAutoPartyUIDList;
	BOOST_TEST_FOREACH( KAutoPartyPtr, spAutoParty, m_vecAutoParty )
	{
		if( spAutoParty == NULL )
			continue;

		spAutoParty->Tick();

		// ������ ���°� S_CLOSE��� ���� ����Ʈ�� AutoPartyUID�� ����!
		if( spAutoParty->GetStateID() == KAutoPartyFSM::S_CLOSE )
		{
			vecDeleteAutoPartyUIDList.push_back( spAutoParty->GetUID() );
		}
	}


	//////////////////////////////////////////////////////////////////////////	
	// �ڵ� ��Ƽ ���� ó��!
	BOOST_TEST_FOREACH( const UidType, iAutoPartyUID, vecDeleteAutoPartyUIDList )
	{
        DestroyAutoParty( iAutoPartyUID );
	}


	//////////////////////////////////////////////////////////////////////////	
	// �ڵ� ��Ƽ ����ŷ ���μ���!
	std::map< KMakingProcessKey, KAutoPartyMakingProcess >::iterator mit;
	for( mit = m_mapAutoPartyMakingProcess.begin(); mit != m_mapAutoPartyMakingProcess.end(); ++mit )
	{
		// tick
		mit->second.Tick();

		// �ڵ� ��Ƽ ���� �۾�!
		std::vector< KAutoPartyPtr > vecMadeAutoParty;
		mit->second.MakingAutoPartyProcessing( vecMadeAutoParty );

		// ������ �ڵ� ��Ƽ�� ����Ʈ�� �߰�!
		BOOST_TEST_FOREACH( KAutoPartyPtr, spAutoParty, vecMadeAutoParty )
		{
			LIF( AddAutoParty( spAutoParty ) );
		}
	}
}

KAutoPartyMakingProcess* KAutoPartyMakingManager::GetAutoPartyMakingProcess( IN const int iDungeonMode, IN const int iDungeonIDWithDif )
{
	switch( iDungeonMode )
	{
		// ��ϸ� �ð�
	case CXSLDungeon::DM_HENIR_PRACTICE:
	case CXSLDungeon::DM_HENIR_CHALLENGE:
		{
			KMakingProcessKey kKey( iDungeonMode, SEnum::DI_ELDER_HENIR_SPACE );

			std::map< KMakingProcessKey, KAutoPartyMakingProcess >::iterator mit;
			mit	= m_mapAutoPartyMakingProcess.find( kKey );
			if( mit	== m_mapAutoPartyMakingProcess.end() )
			{
				START_LOG( cerr, L"�ش� ����ID�� �ڵ� ��Ī �Ҽ� �����ϴ�! " )
					<< BUILD_LOGc( iDungeonMode )
					<< BUILD_LOG( iDungeonIDWithDif )
					<< END_LOG;

				return NULL;
			}

			return &mit->second;
		}
		break;

		// ��д���
	case CXSLDungeon::DM_SECRET_NORMAL:
	case CXSLDungeon::DM_SECRET_HELL:
		{
			KMakingProcessKey kKey( iDungeonMode, iDungeonIDWithDif );

			std::map< KMakingProcessKey, KAutoPartyMakingProcess >::iterator mit;
			mit	= m_mapAutoPartyMakingProcess.find( kKey );
			if( mit	== m_mapAutoPartyMakingProcess.end() )
			{
				START_LOG( cerr, L"�ش� ����ID�� �ڵ� ��Ī �Ҽ� �����ϴ�! " )
					<< BUILD_LOGc( iDungeonMode )
					<< BUILD_LOG( iDungeonIDWithDif )
					<< END_LOG;

				return NULL;
			}

			return &mit->second;
		}
		break;
	}

	// �Ϲ� ����
	KMakingProcessKey kKey( CXSLDungeon::DM_INVALID, iDungeonIDWithDif );

	std::map< KMakingProcessKey, KAutoPartyMakingProcess >::iterator mit;
	mit	= m_mapAutoPartyMakingProcess.find( kKey );
	if( mit	== m_mapAutoPartyMakingProcess.end() )
	{
		START_LOG( cerr, L"�ش� ����ID�� �ڵ� ��Ī �Ҽ� �����ϴ�! " )
			<< BUILD_LOGc( iDungeonMode )
			<< BUILD_LOG( iDungeonIDWithDif )
			<< END_LOG;

		return NULL;
	}

	return &mit->second;
}

KAutoPartyMakingProcess* KAutoPartyMakingManager::GetAutoPartyMakingProcess( IN const UidType iAutoPartyWaitNumber )
{
	const int iDungeonID = static_cast<int>(KAutoPartyMakingProcess::ExtractDungeonIDAtWaitNumber( iAutoPartyWaitNumber ));
	const int iDungeonMode = static_cast<char>(KAutoPartyMakingProcess::ExtractDungeonModeAtWaitNumber( iAutoPartyWaitNumber ));
	return GetAutoPartyMakingProcess( iDungeonMode, iDungeonID );
}

void KAutoPartyMakingManager::RefreshTimerAutoPartyGameStart( IN const int iDungeonMode, IN const int iDungeonIDWithDif )
{
	KAutoPartyMakingProcess* pAutoPartyMakingProcess = GetAutoPartyMakingProcess( iDungeonMode, iDungeonIDWithDif );
	if( IS_NULL( pAutoPartyMakingProcess ) )
	{
		START_LOG( cerr, L"���� ���� �ʴ� �ڵ���Ƽ ���� �����Դϴ�!" )
			<< BUILD_LOG( iDungeonMode )
			<< BUILD_LOG( iDungeonIDWithDif )
			<< END_LOG;
		return;
	}
	
	pAutoPartyMakingProcess->RestartGameStartLastTime();
}

KAutoPartyPtr KAutoPartyMakingManager::GetAutoParty( IN const UidType iAutoPartyUID )
{
	std::map< UidType, KAutoPartyPtr >::iterator mit = m_mapAutoParty.find( iAutoPartyUID );
	if( mit == m_mapAutoParty.end() )
	{
		START_LOG( cwarn, L"ã�� ��ġ�� �����ϴ�.!" )
			<< BUILD_LOG( iAutoPartyUID )
			<< END_LOG;

		return KAutoPartyPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"��ġ ������ �̻�.!" )
			<< BUILD_LOG( iAutoPartyUID )
			<< END_LOG;

		return KAutoPartyPtr();
	}

	return mit->second;
}

bool KAutoPartyMakingManager::AddAutoParty( IN KAutoPartyPtr spAutoParty )
{
	if( spAutoParty == NULL )
	{
		START_LOG( cerr, L"�߰� �Ϸ��� �ڵ� ��Ƽ ������ �̻��մϴ�!" )
			<< END_LOG;
		return false;
	}

	if( GetAutoParty( spAutoParty->GetAutoPartyUID() ) != NULL )
	{
		START_LOG( cerr, L"�̹� �ش� AutoPartyUID���� ����Ʈ�� �����մϴ�. ������ ��Ƽ���� ���� �����ϰڽ��ϴ�." )
			<< BUILD_LOG( spAutoParty->GetAutoPartyUID() )
			<< END_LOG;
		
		// ������ ���� ����ó��
		DestroyAutoParty( spAutoParty->GetAutoPartyUID() );
	}

	m_mapAutoParty.insert( std::make_pair( spAutoParty->GetAutoPartyUID(), spAutoParty ) );
	m_vecAutoParty.push_back( spAutoParty );
	return true;
}

void KAutoPartyMakingManager::DestroyAutoParty( IN const UidType iAutoPartyUID )
{
	KAutoPartyPtr spAutoParty = GetAutoParty( iAutoPartyUID );
	if( spAutoParty == NULL )
	{
		START_LOG( cerr, L"�����Ϸ��� ��ġ ������ �����ϴ�!" )
			<< BUILD_LOG( iAutoPartyUID )
			<< END_LOG;
		return;
	}

	std::vector< KAutoPartyPtr >::iterator vit;
	for( vit = m_vecAutoParty.begin(); vit != m_vecAutoParty.end(); ++vit )
	{
		KAutoPartyPtr spAutoParty = *vit;
		if( spAutoParty == NULL )
			continue;

		if( spAutoParty->GetUID() == iAutoPartyUID )
			break;
	}

	if( vit != m_vecAutoParty.end() )
	{
		m_vecAutoParty.erase( vit );
	}

	m_mapAutoParty.erase( iAutoPartyUID );
}

void KAutoPartyMakingManager::QueueingToAll( IN const KEventPtr& spEvent )
{
	std::map< UidType, KAutoPartyPtr >::iterator mit;
	for( mit = m_mapAutoParty.begin(); mit != m_mapAutoParty.end(); ++mit )
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

void KAutoPartyMakingManager::ProcessEvent( IN const KEventPtr& spEvent_ )
{
	switch( spEvent_->m_usEventID )
	{
		CASE_NOPARAM( EGB_GET_AUTO_PARTY_BONUS_INFO_REQ );
		CASE( EGB_REQUEST_AUTO_PARTY_MAKING_REQ );
		CASE( EGB_CANCEL_AUTO_PARTY_MAKING_REQ );
		CASE_NOPARAM( EGB_CHECK_INVALID_AUTO_PARTY_NOT );
		CASE( EGB_AUTO_PARTY_MAKING_ADMIN_COMMAND_NOT );
	   _CASE( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT );
		//{{ 2012. 11. 13	������		�ڵ���Ƽ ��⿭ ī��Ʈ ǥ��
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
		CASE( EGB_REQUEST_AUTO_PARTY_WAIT_USER_COUNT_NOT );
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
		//}}

	default:
		{
			const UidType iAutoPartyUID = spEvent_->m_kDestPerformer.GetFirstUID();
			KAutoPartyPtr spAutoParty = GetAutoParty( iAutoPartyUID );
			if( spAutoParty == NULL )
			{
				START_LOG( cwarn, L"��ġ �˻� ����." )
					<< BUILD_LOG( iAutoPartyUID )
					<< BUILD_LOG( spEvent_->GetIDStr() )
					<< END_LOG;
				break;
			}

			// �ڵ���Ƽ�� ���� event����
			spAutoParty->QueueingEvent( spEvent_ );
		}
		break;
	}
}

IMPL_ON_FUNC_NOPARAM( EGB_GET_AUTO_PARTY_BONUS_INFO_REQ )
{
    // ���� �ڵ� ��Ƽ �̺�Ʈ ���θ� ���Ӽ����� �����ش�!
	KEGB_GET_AUTO_PARTY_BONUS_INFO_ACK kPacket;

	std::map< KMakingProcessKey, KAutoPartyMakingProcess >::iterator mit;
	for( mit = m_mapAutoPartyMakingProcess.begin(); mit != m_mapAutoPartyMakingProcess.end(); ++mit )
	{
		KAutoPartyBonusInfo kInfo;
		kInfo.m_cDungeonMode = mit->second.GetDungeonMode();
		kInfo.m_iDungeonIDWithDif = mit->second.GetDungeonIDWithDif();
		kInfo.m_bActivatedBonus = mit->second.IsActivatedAutoPartyBonus();
        kPacket.m_vecAutoPartyBonusInfo.push_back( kInfo );
	}
	SendToGameServer( LAST_SENDER_UID, EGB_GET_AUTO_PARTY_BONUS_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( EGB_REQUEST_AUTO_PARTY_MAKING_REQ )
{
	// ��ġ ����ŷ!
	KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	const int iDungeonMode = kPacket_.m_kAutoPartyCondition.m_cDungeonMode;
	//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	int iDungeonIDWithDif = kPacket_.m_kAutoPartyCondition.m_iDungeonID + static_cast<int>(kPacket_.m_kAutoPartyCondition.m_cDifficultyLevel);
	switch( iDungeonMode )
	{
	case CXSLDungeon::DM_HENIR_PRACTICE:
	case CXSLDungeon::DM_HENIR_CHALLENGE:
	case CXSLDungeon::DM_SECRET_NORMAL:
	case CXSLDungeon::DM_SECRET_HELL:
		break;

	default:
		{
			iDungeonIDWithDif = iDungeonIDWithDif - ( iDungeonIDWithDif % 10 ); // ���̵��� ����
		}
		break;
	}
#else
	const int iDungeonIDWithDif = kPacket_.m_kAutoPartyCondition.m_iDungeonID + static_cast<int>(kPacket_.m_kAutoPartyCondition.m_cDifficultyLevel);
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}	
	
	KAutoPartyMakingProcess* pAutoPartyMakingProcess = GetAutoPartyMakingProcess( iDungeonMode, iDungeonIDWithDif );
	if( pAutoPartyMakingProcess == NULL )
	{
		START_LOG( cerr, L"�ش� �ڵ� ��Ƽ ��û �����δ� �ڵ� ��Ƽ ��û�� �Ҽ� �����ϴ�! " )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( iDungeonMode )
			<< BUILD_LOG( iDungeonIDWithDif )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_AUTO_PARTY_04;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_REQUEST_AUTO_PARTY_MAKING_ACK, kPacket );
		return;
	}

	// ��ġ ����� ����Ʈ�� ���!
	UidType iAutoPartyWaitNumber = 0;
	//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	if( pAutoPartyMakingProcess->RegAutoPartyWaitList( kPacket_.m_vecAutoPartyUser, kPacket_.m_kAutoPartyCondition, iAutoPartyWaitNumber ) == false )
#else
	if( pAutoPartyMakingProcess->RegAutoPartyWaitList( kPacket_.m_vecAutoPartyUser, iAutoPartyWaitNumber ) == false )
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}
	{
		START_LOG( cerr, L"�ڵ� ��Ƽ ����� ����Ʈ�� ����� �����Ͽ����ϴ�!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_vecAutoPartyUser.size() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_AUTO_PARTY_05;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_REQUEST_AUTO_PARTY_MAKING_ACK, kPacket );
		return;
	}

	// ��� ����
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_REQUEST_AUTO_PARTY_MAKING_ACK, kPacket );

	// �ڵ� ��Ƽ ����� ��� ������ ���ؼ� �˸�
	BOOST_TEST_FOREACH( const KAutoPartyUserInfo&, kInfo, kPacket_.m_vecAutoPartyUser )
	{
		KEGS_REG_AUTO_PARTY_WAIT_LIST_SUCCESS_NOT kPacketNot;
		kPacketNot.m_bRemakingAutoParty = kPacket_.m_bRemakingAutoParty; // ���� ����!
		kPacketNot.m_iWaitNumber = iAutoPartyWaitNumber;
		kPacketNot.m_iEstimatedTime = pAutoPartyMakingProcess->GetAutoPartyMakingEstimatedTime();
		//{{ 2012. 11. 13	������		�ڵ���Ƽ ��⿭ ī��Ʈ ǥ��
//#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
		//{{ 2012. 11. 21	������		����ο� ǥ�� ���� �������� ����
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
		kPacketNot.m_iAutoPartyWaitUserCount = pAutoPartyMakingProcess->GetWaitUserCountByWaitNumber( iAutoPartyWaitNumber );
#else
		kPacketNot.m_iAutoPartyWaitUserCount = pAutoPartyMakingProcess->GetWaitUserCount();
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
		//}}		
//#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
		//}}
		SendToGSCharacter( kInfo.m_iGSUID, kInfo.m_iUnitUID, EGB_REG_AUTO_PARTY_WAIT_LIST_SUCCESS_NOT, kPacketNot );
	}
}

IMPL_ON_FUNC( EGB_CANCEL_AUTO_PARTY_MAKING_REQ )
{
	// WaitNumber�� ���� ó��!
	if( kPacket_.m_iAutoPartyWaitNumber != 0 )
	{
		// �ش� �Ŵ����� ã�Ƽ� ��� �����϶�!
		KAutoPartyMakingProcess* pAutoPartyMakingProcess = GetAutoPartyMakingProcess( kPacket_.m_iAutoPartyWaitNumber );
		if( pAutoPartyMakingProcess != NULL )
		{
			// �ý����� ���� ��Ŷ�� �ƴѰ�� ACK����
			if( kPacket_.m_bSendBySystem == false )
			{
				KEGS_CANCEL_AUTO_PARTY_MAKING_ACK kPacket;
				kPacket.m_iOK = NetError::NET_OK;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_CANCEL_AUTO_PARTY_MAKING_ACK, kPacket );
			}

			if( pAutoPartyMakingProcess->UnRegAutoPartyWaitListByWaitNumber( kPacket_.m_iAutoPartyWaitNumber, kPacket_.m_iReason ) == false )
			{
				START_LOG( cerr, L"�ڵ� ��Ƽ ��û ���� ����!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( kPacket_.m_iAutoPartyWaitNumber )
					<< BUILD_LOG( kPacket_.m_iReason )
					<< END_LOG;
			}
		}
		else
		{
			START_LOG( cerr, L"�ڵ� ��Ƽ ��û ���� �Ϸ��µ� �ڵ� ��Ƽ ��� ������ ����? �ð��� �����ΰ�?" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iAutoPartyWaitNumber )
				<< BUILD_LOG( kPacket_.m_iReason )
				<< END_LOG;
		}

		if( kPacket_.m_iAutoPartyUID != 0 )
		{
			START_LOG( cerr, L"����ο��ε� �ڵ���Ƽ�� �ҼӵǾ��ֳ�? ���� �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iAutoPartyWaitNumber )
				<< BUILD_LOG( kPacket_.m_iAutoPartyUID )
				<< END_LOG;
			// �α׸� ����
		}
	}

	// AutoPartyUID�� ���� ó��!
	if( kPacket_.m_iAutoPartyUID != 0 )
	{
		KAutoPartyPtr spAutoParty = GetAutoParty( kPacket_.m_iAutoPartyUID );
		if( spAutoParty != NULL )
		{
			// ��ġ�� event����
			KEventPtr spEvent( new KEvent );
			UidType anTrace[2] = { FIRST_SENDER_UID, LAST_SENDER_UID };
			spEvent->SetData( PI_GLOBAL_MATCH_MAKING, anTrace, EGB_CANCEL_AUTO_PARTY_MAKING_REQ, kPacket_ );
			spAutoParty->QueueingEvent( spEvent );
			return;
		}
		else
		{
			START_LOG( cerr, L"�ڵ� ��Ƽ ��Ż ó�� �Ϸ��µ� �ڵ� ��Ƽ ������ ����? �ð��� �����ΰ�?" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iAutoPartyUID )
				<< END_LOG;
		}
	}
}

IMPL_ON_FUNC_NOPARAM( EGB_CHECK_INVALID_AUTO_PARTY_NOT )
{
    START_LOG( cout, L"GlobalUser��ü�� �Ҹ�Ǿ� ��� �ڵ� ��Ƽ ������ �����մϴ�!" )
		<< END_LOG;

	ClearAllAutoPartyInfo();
}

IMPL_ON_FUNC( EGB_AUTO_PARTY_MAKING_ADMIN_COMMAND_NOT )
{
    START_LOG( cout, L"������ Ŀ�ǵ忡 ���� ���� ����!" )
		<< BUILD_LOGc( kPacket_.m_cType );

	switch( kPacket_.m_cType )
	{
	case KEGB_AUTO_PARTY_MAKING_ADMIN_COMMAND_NOT::CT_DUMP:
		{

		}
		break;

	case KEGB_AUTO_PARTY_MAKING_ADMIN_COMMAND_NOT::CT_CLEAR:
		{
            ClearAllAutoPartyInfo();
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

_IMPL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	switch( kPacket_.m_iOrderType )
	{
		CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GB_AUTO_PARTY_SCRIPT_MANAGER, KAutoPartyScriptManager );

	default:
		{
			START_LOG( cerr, L"�������� ���� �ȵǴ� Ÿ���ε�?" )
				<< BUILD_LOG( kPacket_.m_iOrderType )
				<< END_LOG;
		}
		break;
	}
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 11. 13	������		�ڵ���Ƽ ��⿭ ī��Ʈ ǥ��
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
IMPL_ON_FUNC( EGB_REQUEST_AUTO_PARTY_WAIT_USER_COUNT_NOT )
{
	// �������� WaitNumber���� üũ
	if( kPacket_.m_iAutoPartyWaitNumber == 0 )
	{
		START_LOG( cerr, L"WaitNumber�� 0�ε� ��⿭ ī��Ʈ ������ �Գ�?" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iAutoPartyWaitNumber )
			<< END_LOG;
		return;
	}

	// �ش� �Ŵ����� ã�Ƽ� ��� �����϶�!
	KAutoPartyMakingProcess* pAutoPartyMakingProcess = GetAutoPartyMakingProcess( kPacket_.m_iAutoPartyWaitNumber );
	if( IS_NULL( pAutoPartyMakingProcess ) )
	{
		START_LOG( cerr, L"��⿭ ī��Ʈ ��� ����!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iAutoPartyWaitNumber )
			<< END_LOG;
		return;
	}

	// �ý����� ���� ��Ŷ�� �ƴѰ�� ACK����
	KEGS_AUTO_PARTY_WAIT_USER_COUNT_NOT kPacket;
	//{{ 2012. 11. 21	������		����ο� ǥ�� ���� �������� ����
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	kPacket.m_iAutoPartyWaitUserCount = pAutoPartyMakingProcess->GetWaitUserCountByWaitNumber( kPacket_.m_iAutoPartyWaitNumber );
#else
	kPacket.m_iAutoPartyWaitUserCount = pAutoPartyMakingProcess->GetWaitUserCount();
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	//}}	
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_AUTO_PARTY_WAIT_USER_COUNT_NOT, kPacket );
}
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
//}}



