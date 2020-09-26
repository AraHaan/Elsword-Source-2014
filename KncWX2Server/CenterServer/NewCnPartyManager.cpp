#include "NewCnPartyManager.h"
#include "NetError.h"

#include "CnPartyFSM.h"
#include "CnServer.h"
#include "X2Data/XSLRoom.h"
#include "X2Data/XSLDungeonManager.h"
#include "X2Data/XSLPartyManager.h"
#include "Lottery.h"
#include "Enum/Enum.h"

//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
#include "Socket/NetCommon.h"		// ������ ����(����� �� ����) IP
#include "CnNetLayer.h"				// ������ ����(����� �� ����) Port
#endif SERV_CHOOSE_FASTEST_HOST
//}
//////////////////////////////////////////////////////////////////////////
//{{ 2011. 12. 05	������	��Ʋ�ʵ�
#ifdef SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////


#define CLASS_TYPE KCnPartyManager
ImplPfID( KCnPartyManager, PI_CN_PARTY );

ImplementSingleton( KCnPartyManager );


KCnPartyManager::KCnPartyManager() :
m_ciMaxParty(2000)
{
	lua_tinker::class_add<KCnPartyManager>( g_pLua, "KCnPartyManager" );
	lua_tinker::class_def<KCnPartyManager>( g_pLua, "dump",						&KCnPartyManager::Dump );

	lua_tinker::decl( g_pLua, "PartyManager", this );
}

KCnPartyManager::~KCnPartyManager()
{
}

ImplToStringW( KCnPartyManager )
{
	stm_ << L"-------------- Party Manager --------------" << std::endl
		 << TOSTRINGW( m_mapParty.size() )
		 << TOSTRINGW( m_setSpareUID.size() );

	//stm_ << L"-------------- Tick Dump --------------------" << std::endl;
	//stm_ << L"MaxTick : " << m_kPartyManagerTickDump.GetMaxTick() << std::endl;
	//stm_ << L"MaxTickRegDate : " << m_kPartyManagerTickDump.GetMaxTickRegDate() << std::endl;

	return stm_;
}

void KCnPartyManager::Init()
{
	for( int iIndex = 1; iIndex <= GetMaxParty(); ++iIndex )
	{
		UidType iPartyUID = 0;
		KncUid::SetPureUID( iPartyUID, static_cast<UidType>(iIndex) );
		KncUid::SetServerID( iPartyUID, GetKCnServer()->GetProxyID() );
		//{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
		KncUid::SetServerGroupID( iPartyUID, GetKCnServer()->GetServerGroupID() );
#endif SERV_INTEGRATION
		//}}

		m_setSpareUID.insert( iPartyUID );

		KCnPartyPtr spParty( new KCnParty( iPartyUID ) );
		if( spParty == NULL )
		{
			START_LOG( cerr, L"KCnPartyPtr new �Ҵ����." )
				<< BUILD_LOG( iPartyUID )
				<< END_LOG;

			return;
		}

		// FSM����
		FSMclassPtr spFSMClass = boost::shared_ptr< KCnPartyFSM >( new KCnPartyFSM );
		spParty->SetFSM( spFSMClass );

		// ��Ƽ ���� ����
		m_mapParty.insert( std::make_pair( iPartyUID, spParty ) );
	}
}

void KCnPartyManager::Tick()
{
	// �̺�Ʈ�� ���� ó���ѵ�..
	KPerformer::Tick();

	// ��Ƽ Tick�� ó������!
	std::map< UidType, KCnPartyPtr >::iterator mit;
	for( mit = m_mapParty.begin(); mit != m_mapParty.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"��Ƽ ������ �̻�" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			continue;
		}

		mit->second->Tick();
	}

	//{{ 2010. 03. 08  ������	TickDumper
//#ifdef SERV_ROOM_TICK
//	m_kPartyManagerTickDump.CheckOneTickLatency();
//#endif SERV_ROOM_TICK
	//}}
}

KCnPartyPtr KCnPartyManager::GetParty( IN UidType iPartyUID )
{
	std::map< UidType, KCnPartyPtr >::iterator mit = m_mapParty.find( iPartyUID );
	if( mit == m_mapParty.end() )
	{
		START_LOG( cwarn, L"ã�� ��Ƽ�� �����ϴ�.!" )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;

		return KCnPartyPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"��Ƽ ������ �̻�.!" )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;

		return KCnPartyPtr();
	}

	return mit->second;
}

//{{ 2010. 01. 27  ������	�� ����
#ifdef SERV_ROOM_COUNT

void KCnPartyManager::SendPartyCountInfo( bool bOpen )
{
	KERM_ROOM_COUNT_INFO_NOT kNot;
	kNot.m_cRoomType = static_cast<char>(CXSLPartyManager::PE_PARTY_COUNT_TYPE);
	kNot.m_bIsOpen = bOpen;

	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_PARTY, 0, PI_CN_PROXY, 0, anTrace, ERM_ROOM_COUNT_INFO_NOT, kNot );
}

#endif SERV_ROOM_COUNT
//}}

void KCnPartyManager::QueueingEventToParty( const KEventPtr& spEvent_ )
{
	UidType iPartyUID = 0;
	switch( spEvent_->m_usEventID )
	{
	case EPM_CREATE_PARTY_BY_INVITE_REQ:
	case EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ:
	case EPM_REQUEST_REGROUP_PARTY_NOT:
	case EPM_CHECK_INVALID_USER_NOT:
		QueueingEvent( spEvent_ ); // ���� ������ ���õ� ó���� RoomManager Thread���� ó���ؾ� �ϹǷ� ���⼭ �ٷ� ó�� ���ϰ� event queueing�Ѵ�.
		return;

	default:
		// ��Ƽ �ϳ����� �̺�Ʈ�� ���޵ȴٰ� ������. GetFirstUID
		iPartyUID = spEvent_->m_kDestPerformer.GetFirstUID();
		break;
	}

	KCnPartyPtr spParty = GetParty( iPartyUID );
	if( spParty == NULL )
	{
		START_LOG( cerr, L"��Ƽ �˻� ����." )
			<< BUILD_LOG( iPartyUID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
		return;
	}

	// ��Ƽ�� event����
	spParty->QueueingEvent( spEvent_ );
}

void KCnPartyManager::QueueingToAll( const KEventPtr& spEvent )
{
	std::map< UidType, KCnPartyPtr >::iterator mit;
	for( mit = m_mapParty.begin(); mit != m_mapParty.end(); ++mit )
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

void KCnPartyManager::ProcessEvent( const KEventPtr& spEvent_ )
{
	switch( spEvent_->m_usEventID )
	{
        CASE( EPM_CREATE_PARTY_BY_INVITE_REQ );
		CASE( EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ );
		CASE( EPM_REQUEST_REGROUP_PARTY_NOT );
	   _CASE( EPM_CHECK_INVALID_USER_NOT, UidType );

	default:
		START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
	}
}

IMPL_ON_FUNC( EPM_CREATE_PARTY_BY_INVITE_REQ )
{
	// �ϴ� ��Ƽ ��ü�� �ϳ� ���ɴϴ�.
	KCnPartyPtr spParty = OpenParty();
	if( spParty == NULL )
	{
		START_LOG( cerr, L"������ ����Ƽ ��ü�� ����." )
			<< BUILD_LOG( kPacket_.m_kHostPartyUserInfo.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_kHostPartyUserInfo.m_wstrNickName )
			<< END_LOG;

		KEPM_CREATE_PARTY_BY_INVITE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_PARTY_00;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CREATE_PARTY_BY_INVITE_ACK, kPacket );
		return;
	}

	// ����� ���� �� �Է�
	UidType anTrace[2] = { FIRST_SENDER_UID, LAST_SENDER_UID };

	// �ش� ��Ƽ�� �̺�Ʈ ť��
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_PARTY, anTrace, EPM_CREATE_PARTY_BY_INVITE_REQ, kPacket_ );
	spParty->QueueingEvent( spEvent );
}

IMPL_ON_FUNC( EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ )
{
	// �ϴ� ��Ƽ ��ü�� �ϳ� ���ɴϴ�.
	KCnPartyPtr spParty = OpenParty();
	if( spParty == NULL )
	{
		START_LOG( cerr, L"������ ����Ƽ ��ü�� ����." )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_PARTY_00;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacket );
		return;
	}

	// ����� ���� �� �Է�
	UidType anTrace[2] = { FIRST_SENDER_UID, LAST_SENDER_UID };

	// �ش� ��Ƽ�� �̺�Ʈ ť��
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_PARTY, anTrace, EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ, kPacket_ );
	spParty->QueueingEvent( spEvent );
}

IMPL_ON_FUNC( EPM_REQUEST_REGROUP_PARTY_NOT )
{
	// �ϴ� ��Ƽ ��ü�� �ϳ� ���ɴϴ�.
	KCnPartyPtr spParty = OpenParty();
	if( spParty == NULL )
	{
		START_LOG( cerr, L"������ ����Ƽ ��ü�� ����." )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	// ����� ���� �� �Է�
	UidType anTrace[2] = { FIRST_SENDER_UID, LAST_SENDER_UID };

	// �ش� ��Ƽ�� �̺�Ʈ ť��
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_PARTY, anTrace, EPM_REQUEST_REGROUP_PARTY_NOT, kPacket_ );
	spParty->QueueingEvent( spEvent );
}

_IMPL_ON_FUNC( EPM_CHECK_INVALID_USER_NOT, UidType )
{
	std::map< UidType, KCnPartyPtr >::iterator mit;
	for( mit = m_mapParty.begin(); mit != m_mapParty.end(); ++mit )
	{
		KCnPartyPtr spParty = mit->second;
		if( spParty == NULL )
		{
			START_LOG( cerr, L"��Ƽ ������ �̻�" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// ����� ���� �� �Է�
		UidType anTrace[2] = { FIRST_SENDER_UID, LAST_SENDER_UID };

		// �ش� ��Ƽ�� �̺�Ʈ ť��
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CN_PARTY, anTrace, EPM_CHECK_INVALID_USER_NOT, kPacket_ );
		spParty->QueueingEvent( spEvent );
	}
}

KCnPartyPtr KCnPartyManager::OpenParty()
{
	std::set< UidType >::iterator sit = m_setSpareUID.begin();
	if( sit == m_setSpareUID.end() )
	{
		START_LOG( cerr, L"��Ƽ�� ���� ����� �ϴµ� �󸮽�Ʈ�� ����.?" )
			<< END_LOG;

		return KCnPartyPtr();
	}

	KCnPartyPtr spParty = GetParty( *sit );
	if( spParty == NULL )
	{
		START_LOG( cerr, L"���� ������ ��Ƽ ��ü�� ����." )
			<< END_LOG;

		return KCnPartyPtr();
	}

	m_setSpareUID.erase( sit );
	spParty->Reset();

	//{{ 2010. 01. 27  ������	�� ����
#ifdef SERV_ROOM_COUNT
	SendPartyCountInfo( true );
#endif SERV_ROOM_COUNT
	//}}

	return spParty;
}

void KCnPartyManager::CloseParty( IN KCnPartyPtr spParty, bool bClosePartyList /*= true*/ )
{
	const UidType iPartyUID = spParty->GetUID();

	std::set< UidType >::iterator sit = m_setSpareUID.find( spParty->GetUID() );
	if( sit != m_setSpareUID.end() )
	{
		START_LOG( cerr, L"��Ƽ�� �������µ� �̹� �����ֵ�..?" )
			<< BUILD_LOG( spParty->GetUID() )
			<< END_LOG;
		return;
	}

	m_setSpareUID.insert( spParty->GetUID() );

	// ��Ƽ�� ���� �ǹǷ� ��� ���Ӽ����� GSPartyManager�� �����Ǵ� ��Ƽ�� �˸���.
	spParty->DelPartyInfoInPartyList();

	//�׸��� ��Ƽ �ʱ�ȭ.
	spParty->Reset();

	// ���� üũ
	switch( spParty->GetStateID() )
	{
	case KCnPartyFSM::S_INIT:
		// ��Ƽ ������� ���ϰ� �ٽ� �ݴ±���!
		break;

	case KCnPartyFSM::S_MAKING_PARTY:
	case KCnPartyFSM::S_WAIT:
	case KCnPartyFSM::S_ACCEPT_FOR_PLAY:
	case KCnPartyFSM::S_PREPARE_FOR_PLAY:
		{
			// ��Ƽ ���� CLOSE���·� ����
			spParty->StateTransition( KCnPartyFSM::I_TO_CLOSE );
		}

	case KCnPartyFSM::S_CLOSE:
		{
			// ��Ƽ ���� INIT���� ����
			spParty->StateTransition( KCnPartyFSM::I_TO_INIT );
		}
		break;

	default:
		{
			START_LOG( cerr, L"��Ƽ�� �������µ� ���°� �̻���. ������ ���� ����!" )
				<< BUILD_LOG( spParty->GetStateIDString() )
				<< END_LOG;
		}
	}	

	if( bClosePartyList )
	{
		KEPM_CLOSE_PARTY_LIST_NOT kPacket;
		kPacket.m_iPartyUID = iPartyUID;
		BroadCastGameServer( EPM_CLOSE_PARTY_LIST_NOT, kPacket );
	}

	//{{ 2010. 01. 27  ������	�� ����
#ifdef SERV_ROOM_COUNT
	SendPartyCountInfo( false );
#endif SERV_ROOM_COUNT
	//}}
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}


