#include "ActorManager.h"
#include "GiantRoleReg.h"

#ifdef SERV_COUNTRY_CN

#include "BaseServer.h"
#include "KncSend.h"
#include "GiantRoleRegManager.h"
#include "NetError.h"
#include "UserList.h"
#include "LoginSimLayer.h"

#define CLASS_TYPE  KGiantRoleReg

KGiantRoleReg::KGiantRoleReg( void )
{
}

KGiantRoleReg::~KGiantRoleReg( void )
{
	End( 15000 );
}

void KGiantRoleReg::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
	CASE( EGIANT_COMMON_NULL_SERVER );
	CASE( EGIANT_COMMON_NULL_CLIENT );

	CASE( EGIANT_ROLEREG_CREATE_UNIT_REQ );
	_CASE( EGIANT_ROLEREG_CREATE_UNIT_ACK, KEGIANT_ROLEREG );
	CASE( EGIANT_ROLEREG_DELETE_UNIT_REQ );
	_CASE( EGIANT_ROLEREG_DELETE_UNIT_ACK, KEGIANT_ROLEREG );

	CASE( EGIANT_ROLEREG_CREATE_GUILD_REQ );
	_CASE( EGIANT_ROLEREG_CREATE_GUILD_ACK, KEGIANT_ROLEREG );
	CASE( EGIANT_ROLEREG_DELETE_GUILD_REQ );
	_CASE( EGIANT_ROLEREG_DELETE_GUILD_ACK, KEGIANT_ROLEREG );

	default:
		START_LOG( cerr, L"�ڵ鷯�� �������� ���� �̺�Ʈ." )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG( cwarn, L"�̺�Ʈ ó�� �ҿ� �ð�." )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}

	return;
}

IMPL_ON_FUNC( EGIANT_COMMON_NULL_SERVER )
{
	//////////////////////////////////////////////////////////////////////////
	START_LOG( clog, L"RoleRegServer Null Server" )
		<< END_LOG;
	//////////////////////////////////////////////////////////////////////////		

	boost::shared_ptr< KGiantRoleRegPacket > spPacket( new KGiantRoleRegPacket );
	spPacket->Write( kPacket_ );
	SiKGiantRoleRegManager()->QueueingSendPacket( spPacket );
};

IMPL_ON_FUNC( EGIANT_COMMON_NULL_CLIENT )
{
	START_LOG( clog, L"RoleRegServer Null Client" )
		<< END_LOG;

	boost::shared_ptr< KGiantRoleRegPacket > spPacket( new KGiantRoleRegPacket );
	spPacket->Write( kPacket_ );
	SiKGiantRoleRegManager()->QueueingSendPacket( spPacket );
};

IMPL_ON_FUNC( EGIANT_ROLEREG_CREATE_UNIT_REQ )
{
	KEGIANT_ROLEREG kPacket;
	kPacket.m_usRegType = KGiantRoleRegPacket::GR_RT_UNIT;
	kPacket.m_usServerID = static_cast<unsigned short>(LAST_SENDER_UID);
	kPacket.m_uiGiantUID = kPacket_.m_uiGiantUID;
	kPacket.m_usZone = SiKGiantRoleRegManager()->GetServerInfo().m_usZone;
	kPacket.m_usGame = SiKGiantRoleRegManager()->GetServerInfo().m_usGame;
	kPacket.m_wstrName = kPacket_.m_CreateUnitReqPacket.m_wstrNickName;
	kPacket.m_uiRoleUID = 0;
	kPacket.m_usState = KGiantRoleRegPacket::GR_ST_MASK_INSERT_REQ;

	START_LOG( clog, L"EGIANT_ROLEREG_CREATE_UNIT_REQ" )
		<< BUILD_LOG( kPacket_.m_iActorUID )
		<< BUILD_LOG( kPacket.m_usRegType )
		<< BUILD_LOG( kPacket.m_usServerID )
		<< BUILD_LOG( kPacket.m_uiGiantUID )
		<< BUILD_LOG( kPacket.m_usZone )
		<< BUILD_LOG( kPacket.m_usGame )
		<< BUILD_LOG( kPacket.m_wstrName )
		<< BUILD_LOG( kPacket.m_uiRoleUID )
		<< BUILD_LOG( kPacket.m_usState )
		<< END_LOG;

	boost::shared_ptr< KGiantRoleRegPacket > spPacket( new KGiantRoleRegPacket );
	spPacket->Write( kPacket );
	SiKGiantRoleRegManager()->QueueingSendPacket( spPacket );
	SiKGiantRoleRegManager()->RegisterUserIDUnit( kPacket_.m_uiGiantUID, kPacket_ );
}

_IMPL_ON_FUNC( EGIANT_ROLEREG_CREATE_UNIT_ACK, KEGIANT_ROLEREG )
{
	// ������ REQ ��Ŷ ����
	KEGIANT_ROLEREG_CREATE_UNIT_REQ kPacketReq;
	SiKGiantRoleRegManager()->UnregisterUserIDUnit( kPacket_.m_uiGiantUID, kPacketReq );
	KEGIANT_ROLEREG_CREATE_UNIT_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
	kPacketAck.m_CreateUnitReqPacket = kPacketReq.m_CreateUnitReqPacket;

	START_LOG( clog, L"EGIANT_ROLEREG_CHARNAME_ACK" )
		<< BUILD_LOG( kPacketReq.m_iActorUID )
		<< BUILD_LOG( kPacket_.m_usRegType )
		<< BUILD_LOG( kPacket_.m_usServerID )
		<< BUILD_LOG( kPacket_.m_uiGiantUID )
		<< BUILD_LOG( kPacket_.m_usZone )
		<< BUILD_LOG( kPacket_.m_usGame )
		<< BUILD_LOG( kPacket_.m_wstrName )
		<< BUILD_LOG( kPacket_.m_uiRoleUID )
		<< BUILD_LOG( kPacket_.m_usState )
		<< END_LOG;

	if( kPacket_.m_usRegType == KGiantRoleRegPacket::GR_RT_UNIT && kPacket_.m_usState & KGiantRoleRegPacket::GR_ST_MASK_INSERT_REQ )
	{
		if( kPacket_.m_usState & KGiantRoleRegPacket::GR_ST_MASK_SUCCESS )
		{
			// �߰� ����
			kPacketAck.m_iOK = NetError::NET_OK;

			//KEventPtr spEvent( new KEvent );
			//spEvent->SetData( PI_GS_USER, NULL, EGIANT_ROLEREG_CREATE_UNIT_ACK, kPacketAck );
			//KActorManager::GetKObj()->QueueingTo( kPacketReq.m_iActorUID, spEvent );
			SiKGiantRoleRegManager()->SendToGSUser(kPacket_.m_usServerID, kPacketReq.m_iActorUID, EGIANT_ROLEREG_CREATE_UNIT_ACK, kPacketAck);

			return;
		}
		else
		{
			// �߰� ����
			START_LOG( clog, L"���̾�Ʈ ĳ���� ���ϼ� ���� �߰� ����." )
				<< BUILD_LOG( kPacketReq.m_uiGiantUID )
				<< BUILD_LOG( kPacketReq.m_CreateUnitReqPacket.m_wstrNickName )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_CREATE_UNIT_01;

			//KEventPtr spEvent( new KEvent );
			//spEvent->SetData( PI_GS_USER, NULL, EGIANT_ROLEREG_CREATE_UNIT_ACK, kPacketAck );
			//KActorManager::GetKObj()->QueueingTo( kPacketReq.m_iActorUID, spEvent );
			SiKGiantRoleRegManager()->SendToGSUser(kPacket_.m_usServerID, kPacketReq.m_iActorUID, EGIANT_ROLEREG_CREATE_UNIT_ACK, kPacketAck);

			return;
		}
	}

//end_proc:
	// ��Ŷ�� ���� �̻���.
	START_LOG( cerr, L"���̾�Ʈ RoleReg ��Ŷ ���� - �̺�Ʈ �ڵ鸵�� �̻���." )
		<< BUILD_LOG( kPacket_.m_usRegType )
		<< BUILD_LOG( kPacket_.m_usServerID )
		<< BUILD_LOG( kPacket_.m_uiGiantUID )
		<< BUILD_LOG( kPacket_.m_usZone )
		<< BUILD_LOG( kPacket_.m_usGame )
		<< BUILD_LOG( kPacket_.m_wstrName )
		<< BUILD_LOG( kPacket_.m_uiRoleUID )
		<< BUILD_LOG( kPacket_.m_usState )
		<< END_LOG;
}

IMPL_ON_FUNC( EGIANT_ROLEREG_DELETE_UNIT_REQ )
{
	KEGIANT_ROLEREG kPacket;
	kPacket.m_usRegType = KGiantRoleRegPacket::GR_RT_UNIT;
	kPacket.m_usServerID = static_cast<unsigned short>(LAST_SENDER_UID);
	kPacket.m_uiGiantUID = kPacket_.m_uiGiantUID;
	kPacket.m_usZone = SiKGiantRoleRegManager()->GetServerInfo().m_usZone;
	kPacket.m_usGame = SiKGiantRoleRegManager()->GetServerInfo().m_usGame;
	kPacket.m_wstrName = kPacket_.m_wstrNickName;
	kPacket.m_uiRoleUID = 0;
	kPacket.m_usState = KGiantRoleRegPacket::GR_ST_MASK_DELETE_REQ;

	//////////////////////////////////////////////////////////////////////////
	START_LOG( clog, L"EGIANT_ROLEREG_DELETE_UNIT_REQ" )
		<< BUILD_LOG( kPacket.m_usRegType )
		<< BUILD_LOG( kPacket.m_usServerID )
		<< BUILD_LOG( kPacket.m_uiGiantUID )
		<< BUILD_LOG( kPacket.m_usZone )
		<< BUILD_LOG( kPacket.m_usGame )
		<< BUILD_LOG( kPacket.m_wstrName )
		<< BUILD_LOG( kPacket.m_uiRoleUID )
		<< BUILD_LOG( kPacket.m_usState )
		<< END_LOG;
	//////////////////////////////////////////////////////////////////////////

	boost::shared_ptr< KGiantRoleRegPacket > spPacket( new KGiantRoleRegPacket );
	spPacket->Write( kPacket );
	SiKGiantRoleRegManager()->QueueingSendPacket( spPacket );
}

_IMPL_ON_FUNC( EGIANT_ROLEREG_DELETE_UNIT_ACK, KEGIANT_ROLEREG )
{
	//////////////////////////////////////////////////////////////////////////
	START_LOG( clog, L"EGIANT_ROLEREG_DELETE_UNIT_ACK" )
		<< BUILD_LOG( kPacket_.m_usRegType )
		<< BUILD_LOG( kPacket_.m_usServerID )
		<< BUILD_LOG( kPacket_.m_uiGiantUID )
		<< BUILD_LOG( kPacket_.m_usZone )
		<< BUILD_LOG( kPacket_.m_usGame )
		<< BUILD_LOG( kPacket_.m_wstrName )
		<< BUILD_LOG( kPacket_.m_uiRoleUID )
		<< BUILD_LOG( kPacket_.m_usState )
		<< END_LOG;
	//////////////////////////////////////////////////////////////////////////

	if( kPacket_.m_usRegType == KGiantRoleRegPacket::GR_RT_UNIT && kPacket_.m_usState & KGiantRoleRegPacket::GR_ST_MASK_DELETE_REQ )
	{
		if( kPacket_.m_usState & KGiantRoleRegPacket::GR_ST_MASK_SUCCESS )
		{
			// ���� ����
			START_LOG( clog, L"���̾�Ʈ ĳ���� ���ϼ� ���� ���� ����." )
				<< BUILD_LOG( kPacket_.m_uiGiantUID )
				<< BUILD_LOG( kPacket_.m_wstrName )
				<< END_LOG;

//{{ 2011.09.27 lygan_������ // �������ִٰ� ���� ����Ʈ�� �÷��� �μ��� �����ؼ� �����ϴ� ��Ŀ��� �ϰ� ���� �������� ����
			// DB���� ó�� �Ϸ� ó��
			//KDBE_UPDATE_GIANT_DELETE_UNIT_NOT kPacket;
			//kPacket.m_wstrNickName = kPacket_.m_wstrName;
			//kPacket.m_uiGiantUID = kPacket_.m_uiGiantUID;
			////KncSend( PI_LOGIN_USER, KBaseServer::GetKObj()->GetUID(), PI_LOGIN_GAME_DB, 0, NULL, DBE_UPDATE_GIANT_DELETE_UNIT_NOT, kPacket );
			//SiKGiantRoleRegManager()->SendToGameDB(DBE_UPDATE_GIANT_DELETE_UNIT_NOT, kPacket);
//}}

			return;
		}
		else
		{
			// ���� ����
			START_LOG( cerr, L"���̾�Ʈ ĳ���� ���ϼ� ���� ���� ����." )
				<< BUILD_LOG( kPacket_.m_uiGiantUID )
				<< BUILD_LOG( kPacket_.m_wstrName )
				<< END_LOG;

			return;
		}
	}

//end_proc:
	START_LOG( cerr, L"���̾�Ʈ �÷��� ��Ŷ ���� - �̺�Ʈ �ڵ鸵�� �̻���" )
		<< BUILD_LOG( kPacket_.m_usRegType )
		<< BUILD_LOG( kPacket_.m_usServerID )
		<< BUILD_LOG( kPacket_.m_uiGiantUID )
		<< BUILD_LOG( kPacket_.m_usZone )
		<< BUILD_LOG( kPacket_.m_usGame )
		<< BUILD_LOG( kPacket_.m_wstrName )
		<< BUILD_LOG( kPacket_.m_uiRoleUID )
		<< BUILD_LOG( kPacket_.m_usState )
		<< END_LOG;
}

IMPL_ON_FUNC( EGIANT_ROLEREG_CREATE_GUILD_REQ )
{
	KEGIANT_ROLEREG kPacket;
	kPacket.m_usRegType = KGiantRoleRegPacket::GR_RT_GUILD;
	kPacket.m_usServerID = static_cast<unsigned short>(LAST_SENDER_UID);
	kPacket.m_uiGiantUID = kPacket_.m_uiGiantUID;
	kPacket.m_usZone = SiKGiantRoleRegManager()->GetServerInfo().m_usZone;
	kPacket.m_usGame = SiKGiantRoleRegManager()->GetServerInfo().m_usGame;
	kPacket.m_wstrName = kPacket_.m_CreateGuildReqPacket.m_wstrGuildName;
	kPacket.m_uiRoleUID = 0;
	kPacket.m_usState = KGiantRoleRegPacket::GR_ST_MASK_INSERT_REQ;


	//////////////////////////////////////////////////////////////////////////
	START_LOG( clog, L"EGIANT_ROLEREG_CREATE_GUILD_REQ" )
		<< BUILD_LOG( kPacket_.m_iActorUID )
		<< BUILD_LOG( kPacket.m_usRegType )
		<< BUILD_LOG( kPacket.m_usServerID )
		<< BUILD_LOG( kPacket.m_uiGiantUID )
		<< BUILD_LOG( kPacket.m_usZone )
		<< BUILD_LOG( kPacket.m_usGame )
		<< BUILD_LOG( kPacket.m_wstrName )
		<< BUILD_LOG( kPacket.m_uiRoleUID )
		<< BUILD_LOG( kPacket.m_usState )
		<< END_LOG;
	//////////////////////////////////////////////////////////////////////////

	boost::shared_ptr< KGiantRoleRegPacket > spPacket( new KGiantRoleRegPacket );
	spPacket->Write( kPacket );
	SiKGiantRoleRegManager()->QueueingSendPacket( spPacket );
	SiKGiantRoleRegManager()->RegisterUserIDGuild( kPacket_.m_uiGiantUID, kPacket_ );
}

_IMPL_ON_FUNC( EGIANT_ROLEREG_CREATE_GUILD_ACK, KEGIANT_ROLEREG )
{
	// ������ REQ ��Ŷ ����
	KEGIANT_ROLEREG_CREATE_GUILD_REQ kPacketReq;
	SiKGiantRoleRegManager()->UnregisterUserIDGuild( kPacket_.m_uiGiantUID, kPacketReq );

	//////////////////////////////////////////////////////////////////////////
	START_LOG( clog, L"EGIANT_ROLEREG_CREATE_GUILD_ACK" )
		<< BUILD_LOG( kPacketReq.m_iActorUID )
		<< BUILD_LOG( kPacket_.m_usRegType )
		<< BUILD_LOG( kPacket_.m_usServerID )
		<< BUILD_LOG( kPacket_.m_uiGiantUID )
		<< BUILD_LOG( kPacket_.m_usZone )
		<< BUILD_LOG( kPacket_.m_usGame )
		<< BUILD_LOG( kPacket_.m_wstrName )
		<< BUILD_LOG( kPacket_.m_uiRoleUID )
		<< BUILD_LOG( kPacket_.m_usState )
		<< END_LOG;
	//////////////////////////////////////////////////////////////////////////

	if( kPacket_.m_usState & KGiantRoleRegPacket::GR_ST_MASK_INSERT_REQ )
	{
		KUserList::KGSUserInfo kInfo;
		if( false == GetKLoginSimLayer()->GetGSUserInfoByPublisherUID( kPacket_.m_uiGiantUID, kInfo ) )
		{
			// �ð����� �����?
			START_LOG( cerr, L"�ش� ������ �α��� �Ǿ� ���� ����" )
				<< BUILD_LOG( kPacket_.m_uiGiantUID )
				<< BUILD_LOG( kInfo.m_nGSUID)
				<< BUILD_LOG( kInfo.m_nUserUID)
				<< END_LOG;

			return;
		}
		if( kPacket_.m_usState & KGiantRoleRegPacket::GR_ST_MASK_SUCCESS )
		{
			// �߰� ����
			KEGIANT_ROLEREG_CREATE_GUILD_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::NET_OK;
			kPacketAck.m_CreateGuildReqPacket = kPacketReq.m_CreateGuildReqPacket;

			//KEventPtr spEvent( new KEvent );
			//spEvent->SetData( PI_GS_USER, NULL, EGIANT_ROLEREG_CREATE_GUILD_ACK, kPacketAck );
			//KActorManager::GetKObj()->QueueingTo( kPacketReq.m_iActorUID, spEvent );
			SiKGiantRoleRegManager()->SendToGSUser(kInfo.m_nGSUID, kInfo.m_nUserUID, EGIANT_ROLEREG_CREATE_GUILD_ACK, kPacketAck);

			return;
		}
		else
		{
			// �߰� ����
			START_LOG( cerr, L"���̾�Ʈ ��� ���ϼ� ���� �߰� ����." )
				<< BUILD_LOG( kPacket_.m_uiGiantUID )
				<< BUILD_LOG( kPacket_.m_wstrName )
				<< END_LOG;

			KEGIANT_ROLEREG_CREATE_GUILD_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_GUILD_01;
			kPacketAck.m_CreateGuildReqPacket = kPacketReq.m_CreateGuildReqPacket;

			SiKGiantRoleRegManager()->SendToGSUser(kInfo.m_nGSUID, kInfo.m_nUserUID, EGIANT_ROLEREG_CREATE_GUILD_ACK, kPacketAck);

			return;
		}
	}
}

IMPL_ON_FUNC( EGIANT_ROLEREG_DELETE_GUILD_REQ )
{
	KEGIANT_ROLEREG kPacket;
	kPacket.m_usRegType = KGiantRoleRegPacket::GR_RT_GUILD;
	kPacket.m_usServerID = static_cast<unsigned short>(LAST_SENDER_UID);
	kPacket.m_uiGiantUID = kPacket_.m_uiGiantUID;
	kPacket.m_usZone = SiKGiantRoleRegManager()->GetServerInfo().m_usZone;
	kPacket.m_usGame = SiKGiantRoleRegManager()->GetServerInfo().m_usGame;
	kPacket.m_wstrName = kPacket_.m_wstrGuildName;
	kPacket.m_uiRoleUID = 0;
	kPacket.m_usState = KGiantRoleRegPacket::GR_ST_MASK_DELETE_REQ;

	//////////////////////////////////////////////////////////////////////////
	START_LOG( clog, L"EGIANT_ROLEREG_DELETE_GUILD_REQ" )
		<< BUILD_LOG( kPacket.m_usRegType )
		<< BUILD_LOG( kPacket.m_usServerID )
		<< BUILD_LOG( kPacket.m_uiGiantUID )
		<< BUILD_LOG( kPacket.m_usZone )
		<< BUILD_LOG( kPacket.m_usGame )
		<< BUILD_LOG( kPacket.m_wstrName )
		<< BUILD_LOG( kPacket.m_uiRoleUID )
		<< BUILD_LOG( kPacket.m_usState )
		<< END_LOG;
	//////////////////////////////////////////////////////////////////////////

	boost::shared_ptr< KGiantRoleRegPacket > spPacket( new KGiantRoleRegPacket );
	spPacket->Write( kPacket );
	SiKGiantRoleRegManager()->QueueingSendPacket( spPacket );
}

_IMPL_ON_FUNC( EGIANT_ROLEREG_DELETE_GUILD_ACK, KEGIANT_ROLEREG )
{
	//////////////////////////////////////////////////////////////////////////
	START_LOG( clog, L"EGIANT_ROLEREG_DELETE_GUILD_ACK" )
		<< BUILD_LOG( kPacket_.m_usRegType )
		<< BUILD_LOG( kPacket_.m_usServerID )
		<< BUILD_LOG( kPacket_.m_uiGiantUID )
		<< BUILD_LOG( kPacket_.m_usZone )
		<< BUILD_LOG( kPacket_.m_usGame )
		<< BUILD_LOG( kPacket_.m_wstrName )
		<< BUILD_LOG( kPacket_.m_uiRoleUID )
		<< BUILD_LOG( kPacket_.m_usState )
		<< END_LOG;
	//////////////////////////////////////////////////////////////////////////

	if( kPacket_.m_usRegType == KGiantRoleRegPacket::GR_RT_GUILD && kPacket_.m_usState & KGiantRoleRegPacket::GR_ST_MASK_DELETE_REQ )
	{
		if( kPacket_.m_usState & KGiantRoleRegPacket::GR_ST_MASK_SUCCESS )
		{
			// ���� ����
			START_LOG( clog, L"���̾�Ʈ ��� ���ϼ� ���� ���� ����." )
				<< BUILD_LOG( kPacket_.m_uiGiantUID )
				<< BUILD_LOG( kPacket_.m_wstrName )
				<< END_LOG;

			//// DB���� ó�� �Ϸ� ó��
			//KDBE_UPDATE_GIANT_DELETE_UNIT_NOT kPacket;
			//kPacket.m_wstrNickName = kPacket_.m_wstrName;
			//kPacket.m_uiGiantUID = kPacket_.m_uiGiantUID;
//			KncSend( PI_LOGIN_USER, KBaseServer::GetKObj()->GetUID(), PI_LOGIN_GAME_DB, 0, NULL, DBE_UPDATE_GIANT_DELETE_UNIT_NOT, kPacket );
//			SendToGameDB(DBE_UPDATE_GIANT_DELETE_UNIT_NOT,kPacket);
			return;
		}
		else
		{
			// ���� ����
			START_LOG( cerr, L"���̾�Ʈ ��� ���ϼ� ���� ���� ����." )
				<< BUILD_LOG( kPacket_.m_uiGiantUID )
				<< BUILD_LOG( kPacket_.m_wstrName )
				<< END_LOG;

			return;
		}
	}
	else
	{
		START_LOG( cerr, L"���̾�Ʈ �÷��� ��Ŷ ���� - �̺�Ʈ �ڵ鸵�� �̻���" )
			<< BUILD_LOG( kPacket_.m_usRegType )
			<< BUILD_LOG( kPacket_.m_usServerID )
			<< BUILD_LOG( kPacket_.m_uiGiantUID )
			<< BUILD_LOG( kPacket_.m_usZone )
			<< BUILD_LOG( kPacket_.m_usGame )
			<< BUILD_LOG( kPacket_.m_wstrName )
			<< BUILD_LOG( kPacket_.m_uiRoleUID )
			<< BUILD_LOG( kPacket_.m_usState )
			<< END_LOG;
	}
}

#endif // SERV_COUNTRY_CN