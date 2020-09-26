#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
#include "GameSysVal/GameSysVal.h"

#include "Enum/Enum.h"

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	#include "EventDataRefresh/EventRewardID.h"
#else
//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
	#include ".\rewardtable.h"
#endif SERV_SECOND_SECURITY
//}}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}

//{{ 2011. 08. 09	������	�ӽ�ID �����
#ifdef SERV_MACHINE_ID_BLOCK
	#include "odbc/Odbc.h"
#endif SERV_MACHINE_ID_BLOCK
//}}

//{{  2011.12.20     ��μ�    2�� ���� �е� ��й�ȣ ��ȣȭ
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	#include "ServerSNMaker.h"
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
//}}

//{{ 2012. 03. 29	��μ�		x2.exe ��� ���� �˻�
#ifdef SERV_CHECK_X2_EXE_MODULE
#include "Mornitoring/MornitoringManager.h"
#endif SERV_CHECK_X2_EXE_MODULE
//}}

#ifdef SERV_NPROTECT_CS_AUTH_30
#include <boost/foreach.hpp>
#endif SERV_NPROTECT_CS_AUTH_30

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp ���� ������ �˴ϴ�!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC( EGS_KNPROTECT_AUTH_CHECK_NOT )
{
	//if( m_kGGAuth.CheckAuthAnswer( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	//{
	//	SetDisconnectReason( KStatistics::eSIColDR_NProtect_Wrong_Ack );
	//	ReserveDestroy();

	//	START_LOG( cerr, L"nProtect ���� ���� ����.!" )
	//		<< END_LOG;
	//	return;
	//}
}

IMPL_ON_FUNC_NOPARAM( EGS_AGREE_HACK_USER_REQ )
{
	//Ư���� ������Ʈ ������ ����.
	if( m_cHackingUserType == SEnum::HUT_DISAGREE_HACK_USER )
	{
		SendToAccountDB( DBE_AGREE_HACK_USER_REQ, GetUID() );

		START_LOG( clog, L"��ŷ���� ����.!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_kNexonAccountInfo.m_wstrID )
			<< END_LOG;

		return;
	}

	KEGS_AGREE_HACK_USER_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_HACK_USER_00;
	SendPacket( EGS_AGREE_HACK_USER_ACK, kPacket );

	START_LOG( cerr, L"��ŷ���� ����Ʈ�� ��ϵ� ������ �ƴѵ� ��Ŷ�� ���ƿ�.!" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( m_kNexonAccountInfo.m_wstrID )
		<< END_LOG;
}

_IMPL_ON_FUNC( DBE_AGREE_HACK_USER_ACK, KEGS_AGREE_HACK_USER_ACK )
{
	//Ư���� ������Ʈ ������ ����..��~
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_cHackingUserType = SEnum::HUT_AGREE_HACK_USER;
		kPacket_.m_cHackingUserType = m_cHackingUserType;

		SendPacket( EGS_AGREE_HACK_USER_ACK, kPacket_ );
		return;
	}

	START_LOG( cerr, L"��ŷ���� ���� ��� ������Ʈ ����.!" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( m_kNexonAccountInfo.m_wstrID )
		<< END_LOG;

	SendPacket( EGS_AGREE_HACK_USER_ACK, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_REPORT_HACK_USER_NOT )
{
	//Ư���� ������Ʈ ������ ����. ��!!
	if( m_cHackingUserType == SEnum::HUT_NORMAL )
	{
		START_LOG( clog, L"��ŷ ���� �Ű�.!" )
			<< BUILD_LOG( GetUID() )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( m_cHackingUserType );

		// �������� ���� �������� ���¸� �ٲ���!
		m_cHackingUserType = SEnum::HUT_DISAGREE_HACK_USER;

		SendToAccountDB( DBE_REPORT_HACK_USER_NOT, GetUID() );		
	}
	else
	{
		START_LOG( cwarn, L"�̹� �Ű�� ������.!" )
			<< BUILD_LOG( GetUID() )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( m_cHackingUserType )
			<< END_LOG;
	}
}

IMPL_ON_FUNC_NOPARAM( EGS_NPROTECT_HACKING_USER_NOT )
{
	// nProtect ��ŷ ������ ���� ��輳�� - �����۾��� Ŭ���̾�Ʈ���� ó����.
	SetDisconnectReason( KStatistics::eSIColDR_nProtect_Hacking );
}

//{{ 2009. 8. 18  ������	hack shield
IMPL_ON_FUNC( EGS_HACKSHIELD_CHECK_ACK )
{
	if( m_kHSAuth.OnHackShieldCheckAck( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		SetDisconnectReason( KStatistics::eSIColDR_HackShield_Wrong_Ack );
		ReserveDestroy();

		START_LOG( cerr, L"hack shield ���� ������ ���� ����! UserUID : " << GetUID() );
		return;
	}
}
//}}

//{{ 2009. 12. 29  ������	NPCKILLALL��ŷ����
IMPL_ON_FUNC_NOPARAM( EGS_DUNGEON_KILLALLNPC_CHECK_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() <= 0 )
	{
		START_LOG( cerr, L"���� �ƴѵ� �� ��Ŷ�� ���ȳ�?" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		return;
	}

	if( CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_DUNGEON )
	{
		START_LOG( cerr, L"�������� �ƴѵ� �� ��Ŷ�� ���ȳ�?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
			<< END_LOG;

		return;
	}

	// ���������� ������!
	SendToCnRoom( ERM_DUNGEON_KILLALLNPC_CHECK_REQ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_KILLALLNPC_CHECK_ACK, KEGS_DUNGEON_KILLALLNPC_CHECK_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_KILLALLNPC_CHECK_ACK, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_REQUEST_HACKSHIELD_CHECK_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	START_LOG( cout, L"Ŭ���̾�Ʈ���� �ٽ��� üũ ��Ŷ�� ��û�Ͽ����ϴ�." )
#ifdef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetCharUID() );
#else
		<< BUILD_LOG( GetUserName() )
		<< BUILD_LOG( GetCharName() );
#endif SERV_PRIVACY_AGREEMENT

	// �ٽ��� ��û ��Ŷ Ŭ��� ����!
	if( m_kHSAuth.Tick( GetThisPtr<KGSUser>(), true ) == false )
	{
		SetDisconnectReason( KStatistics::eSIColDR_HackShield_Auth_Fail );
		ReserveDestroy();

		START_LOG( cerr, L"hack shield ���� ����.!" )
			<< END_LOG;
		return;
	}
}
//}}

//{{ 2010. 05. 20  ������	��ũ��Ʈ �Ľ�
#ifdef SERV_CLIENT_SCRIPT

IMPL_ON_FUNC( EGS_GET_CLIENT_SCRIPT_REQ )
{
	// ���� ���� �ȸ��Ƶ� �ɵ�..

	KEGS_GET_CLIENT_SCRIPT_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;	

	//////////////////////////////////////////////////////////////////////////
	// ��ũ��Ʈ�� �ҷ�����!

	if( SiKClientScriptManager()->GetClientScriptInfo( 0, kPacket.m_wstrScriptString ) == false )
	{
		kPacket.m_iOK = NetError::ERR_UNKNOWN;
	}

	//////////////////////////////////////////////////////////////////////////

	SendPacket( EGS_GET_CLIENT_SCRIPT_ACK, kPacket );
}

#endif SERV_CLIENT_SCRIPT
//}}

//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//{{ 2010. 06. 17  ������	������ ����
#ifdef SERV_PROTECT_AUTO_HACK
_IMPL_ON_FUNC( ERM_SERVER_CHECK_HACK_USER_NOT, KEGS_SERVER_CHECK_HACK_USER_REQ )
{
	// �̷� ��Ŷ�� ���� ���� ����..	

	// ��ڸ� ��ŷ ������ ó�� ���� �ʴ´�.
	if( GetAuthLevel() >= SEnum::UAL_GM )
	{
		START_LOG( cout, L"��ڰ� ��ŷ������ �ǴܵǾ����� ��ŷ������� �� ���� ������ ������ ����." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< BUILD_LOGc( kPacket_.m_cHackingCheckType )
			<< END_LOG;
		return;
	}

	// üũ �׸� �°� ���� ó��
	switch( kPacket_.m_cHackingCheckType )
	{		
	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_NPC_UNIT_DIE:			// �������� ���͸� ���� ������ �̻��� ���
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;

	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_GET_ITEM:				// �߱޵��� ���� �����۰� �̻��� �������� ȹ���Ϸ��� ���
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;

	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_CANT_KILL_NPC_FIND:
		break; // �ƹ��͵� ���� �ʴ´�!

	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_CANT_KILL_NPC_KICK:	// ������ �ʴ� ���͸� �������� ��Ҵ�!
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;
	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_PLAY_START:			// �̹� �÷��� ���� �ߴµ� �� �÷��� �����Ϸ��� ���!
		break;
		//{{ 2010. 10. 05	������	���ͼ��� ��ŷüũ
#ifdef SERV_CN_SERVER_HACKING_CHECK
	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_CLEAR_RESULT:			// ���� Ŭ���� ����� ���������� ���!
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;
#endif SERV_CN_SERVER_HACKING_CHECK
		//}}
		//{{ 2010. 11. 18	������	���� �� �������� �ε� üũ
#ifdef SERV_AUTO_HACK_CHECK_STAGE_LOAD
	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_STAGE_LOADING:		// �̹� �ε��� ���������� �ٽ� �ε��Ϸ��� ���!
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;
#endif SERV_AUTO_HACK_CHECK_STAGE_LOAD
		//}}
		//{{ 2011. 01. 26	������	���� Ŭ���� Ÿ�� ������ üũ
#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_CLEAR_TIME:			// ���� Ŭ���� Ÿ���� ���������� ���!
		{
//			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
//			HackUserRegTradeBlock( kPacket_.m_cHackingCheckType );
//			//{{ 2011. 09. 08	������	������ ��Ŷ ����͸�
//#ifdef SERV_AUTO_HACK_PACKET_MORNITOING
//			HackUserRegPacketMornitor();
//#endif SERV_AUTO_HACK_PACKET_MORNITOING
//			//}}
//	
//			m_kUserHackingManager.SetHackingUserSendType( kPacket_.m_cHackingCheckType );
//
//			// ��Ŷ�� ������!
//			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket_ );
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType
#ifdef SERV_PERIOD_ACCOUNT_BLOCK				
				, SEnum::UAL_BLOCKED_3, L"���å�� ���� ������ ���ѵǾ����ϴ�. (Ȩ������>�����ͷ� ���� �ֽñ� �ٶ��ϴ�.)"
#endif //SERV_PERIOD_ACCOUNT_BLOCK				
				);
			SetRandomTimeKick( KStatistics::eSIColDR_Client_Hacking, 10, 50 );
		}
		break;
#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
		//}}
		//{{ 2011. 09. 07  ��μ�	���� �������� DIE NPC �ð� üũ ���
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_NPC_DIE_TIME:			// ���� ���� ������������ ��� ��Ƽ�� ���� ��� �ð��� ª�� ���!!(1��)
		{
			//HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );

			m_kUserHackingManager.SetHackingUserSendType( kPacket_.m_cHackingCheckType );

			// ��Ŷ�� ������!
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket_ );
		}
		break;
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
		//}}
		//{{ 2012. 02. 06    ��μ�    ���� ���� UID �߱� ��� ����
#ifdef SERV_MAKE_MONSTER_UID_CHANGE
	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_VERIFY_DUNGEON_NPC_UID:			// ���� ���� ���� NPC UID �� Ŭ���̾�Ʈ�� ������ ������(��ŷ����� �ǽ�)
		{
			m_kUserHackingManager.SetHackingUserSendType( kPacket_.m_cHackingCheckType );

			// ��Ŷ�� ������!
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket_ );
		}
		break;
#endif SERV_MAKE_MONSTER_UID_CHANGE	
		//}}
	default:
		{
			START_LOG( cerr, L"��ϵ��� �ʴ� ������ üũ Ÿ�� �Դϴ�!" )
				<< BUILD_LOGc( kPacket_.m_cHackingCheckType )
				<< END_LOG;
		}
		break;
	}
}
#endif SERV_PROTECT_AUTO_HACK
//}}
#else
//{{ 2010. 06. 17  ������	������ ����
#ifdef SERV_PROTECT_AUTO_HACK
_IMPL_ON_FUNC( ERM_SERVER_CHECK_HACK_USER_NOT, KEGS_SERVER_CHECK_HACK_USER_NOT )
{
	// �̷� ��Ŷ�� ���� ���� ����..	

	// ��ڸ� ��ŷ ������ ó�� ���� �ʴ´�.
	if( GetAuthLevel() >= SEnum::UAL_GM )
	{
		START_LOG( cout, L"��ڰ� ��ŷ������ �ǴܵǾ����� ��ŷ������� �� ���� ������ ������ ����." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< BUILD_LOGc( kPacket_.m_cHackingCheckType )
			<< END_LOG;
		return;
	}

	// üũ �׸� �°� ���� ó��
	switch( kPacket_.m_cHackingCheckType )
	{		
	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_NPC_UNIT_DIE:			// �������� ���͸� ���� ������ �̻��� ���
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;

	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_GET_ITEM:				// �߱޵��� ���� �����۰� �̻��� �������� ȹ���Ϸ��� ���
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;

	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_CANT_KILL_NPC_FIND:
		break; // �ƹ��͵� ���� �ʴ´�!

	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_CANT_KILL_NPC_KICK:	// ������ �ʴ� ���͸� �������� ��Ҵ�!
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;
	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_PLAY_START:			// �̹� �÷��� ���� �ߴµ� �� �÷��� �����Ϸ��� ���!
		break;
		//{{ 2010. 10. 05	������	���ͼ��� ��ŷüũ
#ifdef SERV_CN_SERVER_HACKING_CHECK
	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_CLEAR_RESULT:			// ���� Ŭ���� ����� ���������� ���!
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;
#endif SERV_CN_SERVER_HACKING_CHECK
		//}}
		//{{ 2010. 11. 18	������	���� �� �������� �ε� üũ
#ifdef SERV_AUTO_HACK_CHECK_STAGE_LOAD
	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_STAGE_LOADING:		// �̹� �ε��� ���������� �ٽ� �ε��Ϸ��� ���!
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;
#endif SERV_AUTO_HACK_CHECK_STAGE_LOAD
		//}}
		//{{ 2011. 01. 26	������	���� Ŭ���� Ÿ�� ������ üũ
#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_CLEAR_TIME:			// ���� Ŭ���� Ÿ���� ���������� ���!
		{
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserRegTradeBlock( kPacket_.m_cHackingCheckType );
			//{{ 2011. 09. 08	������	������ ��Ŷ ����͸�
#ifdef SERV_AUTO_HACK_PACKET_MORNITOING
			//HackUserRegPacketMornitor();
#endif SERV_AUTO_HACK_PACKET_MORNITOING
			//}}

			// ��Ŷ�� ������!
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket_ );
		}
		break;
#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
		//}}
		//{{ 2011. 09. 07  ��μ�	���� �������� DIE NPC �ð� üũ ���
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_NPC_DIE_TIME:			// ���� ���� ������������ ��� ��Ƽ�� ���� ��� �ð��� ª�� ���!!(1��)
		{
			//HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );

			// ��Ŷ�� ������!
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket_ );
		}
		break;
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
		//}}
		//{{ 2012. 02. 06    ��μ�    ���� ���� UID �߱� ��� ����
#ifdef SERV_MAKE_MONSTER_UID_CHANGE
	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_VERIFY_DUNGEON_NPC_UID:			// ���� ���� ���� NPC UID �� Ŭ���̾�Ʈ�� ������ ������(��ŷ����� �ǽ�)
		{
			// ��Ŷ�� ������!
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket_ );
		}
		break;
#endif SERV_MAKE_MONSTER_UID_CHANGE	
		//}}
	default:
		{
			START_LOG( cerr, L"��ϵ��� �ʴ� ������ üũ Ÿ�� �Դϴ�!" )
				<< BUILD_LOGc( kPacket_.m_cHackingCheckType )
				<< END_LOG;
		}
		break;
	}
}
#endif SERV_PROTECT_AUTO_HACK
//}}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//}}

//{{ 2010. 10. 13	������	DLL Manager
#ifdef SERV_DLL_MANAGER

IMPL_ON_FUNC( EGS_MODULE_INFO_UPDATE_NOT )
{
	// ���� ���� ����

	//{{ 2013. 06. 12	������	���� ����Ʈ ����
#ifdef SERV_BLOCK_LIST
	SendToGlobalServer( EGB_MODULE_INFO_UPDATE_NOT, kPacket_ );
#else
	SendToLoginServer( ELG_MODULE_INFO_UPDATE_NOT, kPacket_ );
#endif SERV_BLOCK_LIST
	//}}

	//{{ 2012. 03. 29	��μ�		x2.exe ��� ���� �˻�
#ifdef SERV_CHECK_X2_EXE_MODULE
	if( SiKMornitoringManager()->CheckModuleList_X2Module() == true )
	{
		// �α� ���
		KDBE_MODULE_EXIST_LOG_NOT kLog;

		if( CheckModuleList_X2Module( kPacket_.m_vecModuleName, kLog.m_mapModule ) == false )
		{
			//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			// x2.exe �� �������� �ʴ´ٸ� ������ �ִ�.
			KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
			kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_X2_EXE_MODULE;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );

			m_kUserHackingManager.SetHackingUserSendType( KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_X2_EXE_MODULE );

#else
			// x2.exe �� �������� �ʴ´ٸ� ������ �ִ�.
			KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
			kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_X2_EXE_MODULE;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			//}}

			CTime tCurr = CTime::GetCurrentTime();
			m_kUserSecurityManager.GetServerSN( kLog.m_iServerSN );
			kLog.m_iUserUID = GetUID();
			kLog.m_wstrRegDate = tCurr.Format( _T("%Y-%m-%d %H:%M:%S") );
			SendToLogDB( DBE_MODULE_EXIST_LOG_NOT, kLog );
		}
	}
#endif SERV_CHECK_X2_EXE_MODULE
	//}}	
}

#endif SERV_DLL_MANAGER
//}}

//{{ 2011. 03. 02	������	���� �ŷ� ����
#ifdef SERV_HACK_USER_TRADE_BLOCK
IMPL_ON_FUNC( ELG_TRADE_BLOCK_NOT )
{
	// ���� ���� ����.

	m_kUserAbuserManager.SetTradeBlockUser( kPacket_.m_bIsTradeBlock );

	if( kPacket_.m_bIsTradeBlock )
	{
		START_LOG( cout, L"��ŷ �ǽ����� ���� ���� �ŷ��� ���ܵǾ����ϴ�!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() );
#else
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() );
#endif SERV_PRIVACY_AGREEMENT
	}
	else
	{
		START_LOG( cout, L"���� �ŷ� ������ �����Ǿ����ϴ�!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() );
#else
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() );
#endif SERV_PRIVACY_AGREEMENT
	}	
}
#endif SERV_HACK_USER_TRADE_BLOCK
//}}

//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
IMPL_ON_FUNC( DBE_GET_SECOND_SECURITY_INFO_ACK )
{
#ifdef SERV_ENTRY_POINT
    VERIFY_STATE_ACK( ( 2, KGSFSM::S_LOGINED, KGSFSM::S_SERVER_SELECT ), EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK );
#endif SERV_ENTRY_POINT
        
	KEGS_SECOND_SECURITY_INFO_NOT kNot;
	kNot.m_iOK = kPacket_.m_iOK;
	kNot.m_bUseSecondPW = kPacket_.m_bUseSecondPW;

	if( kPacket_.m_iOK == NetError::ERR_SECOND_SECURITY_AUTH_FAILED_LIMIT ) // ��� ��ȣ �Է� ���а� 10ȸ �̻��� �� 
	{
		SendPacket( EGS_SECOND_SECURITY_INFO_NOT, kNot );

		// Ŭ���̼� ���� ��Ű��� �Ͽ� �ּ� ��
		//SetDisconnectReason( KStatistics::eSIColDR_Normal_Disconnect );
		//ReserveDestroy();															// ���� ����
		return;
	}

	//{{ 2013. 01. 21  ��Ը� �̺�Ʈ ���� �� ���� ���� ĳ���Ϳ��� �����ϴ� �ý��� - ��μ�
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	if( kPacket_.m_vecFirstSelectUnitReward.empty() == false )
	{
		m_kUserUnitSelectRewardManager.SetFirstSelectUnitRewardInfo( kPacket_.m_vecFirstSelectUnitReward );

		BOOST_TEST_FOREACH( KFirstSelectUnitReward ,kInfo , kPacket_.m_vecFirstSelectUnitReward )
		{
			KEGS_EXIST_FIRST_SELECT_UNIT_REWARD_NOT kReward;
			kReward = kInfo.m_cRewardType;
			SendPacket( EGS_EXIST_FIRST_SELECT_UNIT_REWARD_NOT, kReward );
		}
	}
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	//}

	// DB ���� �޾ƿ� ������ ���� (���� �� ��)
	CTime tLasstAuthDate;
	if( kPacket_.m_bUseSecondPW == true )
	{
		if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrLastAuthDate, tLasstAuthDate ) == false )
		{
			START_LOG( cerr, L"���ڿ� �ð� ��ȯ ����." )
				<< BUILD_LOG( kPacket_.m_wstrLastAuthDate )
				<< END_LOG;
		}

		//{{ 2011. 06. 23	������	2�� ��� ����ó��
#ifdef SERV_CHECK_SECOND_PW_STATE
		StateTransition( KGSFSM::I_TO_CHECK_SECOND_PW );
#endif SERV_CHECK_SECOND_PW_STATE
		//}}
	}
	SetIsUseSecondPW( kPacket_.m_bUseSecondPW );
	SetSecondPWFailedCount( kPacket_.m_iFailedCount );
	SetSecondPW( kPacket_.m_wstrSecondPW );
	SetLastAuthDate( tLasstAuthDate );

	SendPacket( EGS_SECOND_SECURITY_INFO_NOT, kNot );

	if( m_kUserStatistics.IsFirstUserLoginToday( GetUID(), GetLastLoginDate() ) == true )
	{
		// ���� ��� ���� �α� ������!
		SendFirstLoginSecurityInfo();
	}

	//{{  2011.11.08     ��μ�    ���������� ���� ���θ�� �̺�Ʈ
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	SetBF_Team( kPacket_.m_iBF_Team );
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	//}}
	//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
	m_wstrWinterVacationEventRegDate	= kPacket_.m_wstrWinterVacationEventRegDate;
	m_iWinterVacationEventCount			= kPacket_.m_iWinterVacationEventCount;
#endif SERV_2012_WINTER_VACATION_EVENT
	//}}

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	if( kPacket_.m_bIsComeBackUser == true )	// �޸� ���� �����̴�
	{
		m_kComeBackUserInfo.SetIsComeBackFirst( kPacket_.m_bIsFirst );
		m_kComeBackUserInfo.SetComeBackRewardLevel( kPacket_.m_iRewardStep );
		m_kComeBackUserInfo.SetComeBackBuffEndTime( kPacket_.m_wstrComeBackBuffEndDate );

		m_kTimer[TM_COME_BACK_USER].restart();		// ���� �ð� ����ȭ ��

		if( kPacket_.m_bIsFirst == true )	// ���� ������ �� Ŭ�󿡰� �˷�����
		{
			KEGS_COME_BACK_USER_NOT kNot;
			kNot = true;
			SendPacket( EGS_COME_BACK_USER_NOT, kNot );
		}
	}

	m_kComeBackUserInfo.SetIsComeBackUser( kPacket_.m_bIsComeBackUser );	// �޸� ���� �������� �ƴ��� ����

	//{{ 2012. 05. 16	�ڼ���	ù ���� �� ���̵� ���� ����ֱ�
#ifdef SERV_EVENT_GUIDELINE_POPUP
	CTime tLastConnectDate;
	if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrLastConnectDate, tLastConnectDate ) == false )
	{
		if( kPacket_.m_wstrLastConnectDate.size() != 0 )
		{
			START_LOG( cerr, L"���ڿ� �ð� ��ȯ ����." )
				<< BUILD_LOG( kPacket_.m_wstrLastConnectDate )
				<< END_LOG;
		}
	}
	else
	{
		if( tLastConnectDate < CTime( 2012, 5, 17, 6, 0, 0 ) )
		{
			m_bPopupTheGuideLine = true;
		}
	}
#endif SERV_EVENT_GUIDELINE_POPUP
	//}}

	SendToGameDB( DBE_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ, GetName() );
#else
	SendToGameDB( DBE_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ, GetName() );
#endif SERV_COME_BACK_USER_REWARD
	//}}

	//{{ 2012. 06. 07	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	m_mmapDailyGiftBoxList = kPacket_.m_mmapDailyGiftBoxList;
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}
}

IMPL_ON_FUNC( EGS_AUTH_SECOND_SECURITY_REQ )
{
	//{{ 2011. 06. 23	������	2�� ��� ����ó��
#ifdef SERV_CHECK_SECOND_PW_STATE
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_CHECK_SECOND_PW, KGSFSM::S_SERVER_SELECT ), EGS_AUTH_SECOND_SECURITY_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_AUTH_SECOND_SECURITY_ACK );
#endif SERV_CHECK_SECOND_PW_STATE
	//}}

	//{{  2011.12.20     ��μ�    2�� ���� �е� ��й�ȣ ��ȣȭ
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	// ��ȣȭ �Ͽ� DB�� ����
	std::string strPW = KncUtil::toNarrowString( kPacket_.m_wstrSecondPW );
	std::string strChangePW = KServerSNMaker::GetCRCFromString( strPW );
	kPacket_.m_wstrSecondPW = KncUtil::toWideString( strChangePW.c_str() );
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
	//}}

	if( GetSecondPWFailedCount() < 10 && IsSameSecondSecurityPW( kPacket_.m_wstrSecondPW ) == true )
	{
		KDBE_SEUCCESS_SECOND_SECURITY_REQ kReq;
		CTime tCurr = CTime::GetCurrentTime();
		SetLastAuthDate( tCurr );

		kReq.m_iUserID = GetUID();
		kReq.m_wstrCurrTime = tCurr.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

		START_LOG( clog, L"2�� ���� ���� ����!!" )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;
		SendToAccountDB( DBE_SEUCCESS_SECOND_SECURITY_REQ, kReq );
	}
	else
	{
		START_LOG( clog, L"2�� ���� ���� ����!!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_iFailedCount )
			<< END_LOG;
		SendToAccountDB( DBE_FAILED_SECOND_SECURITY_REQ, GetUID() );
	}
}

IMPL_ON_FUNC( DBE_SEUCCESS_SECOND_SECURITY_ACK )
{
	//{{ 2011. 06. 23	������	2�� ��� ����ó��
#ifdef SERV_CHECK_SECOND_PW_STATE
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_CHECK_SECOND_PW, KGSFSM::S_SERVER_SELECT ), EGS_AUTH_SECOND_SECURITY_ACK );
#endif SERV_CHECK_SECOND_PW_STATE
	//}}

	if( kPacket_.m_iOK == NetError::NET_OK )	// ���� ���� ����� �Ϸ� �Ǿ���
	{
		//{{ 2011. 06. 23	������	2�� ��� ����ó��
#ifdef SERV_CHECK_SECOND_PW_STATE
		StateTransition( KGSFSM::I_TO_SERVER_SELECT );
#endif SERV_CHECK_SECOND_PW_STATE
		//}}

		// ���� ������ �ϰ� �Ǹ� ������ �ʱ�ȭ �ȴ�.
		SetSuccessSecondSecurity( true );
		SetSecondPWFailedCount( 0 );

		KEGS_AUTH_SECOND_SECURITY_ACK kAck;

		kAck.m_iOK = NetError::NET_OK;
		kAck.m_iFailedCount = 0;

		SendPacket( EGS_AUTH_SECOND_SECURITY_ACK, kAck );

#ifdef SERV_SECOND_SECURITY_REWARD_ID
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		if( ERI_SECOND_SECURITY == 0 )
#else
		if( KRewardTable::ERI_SECOND_SECURITY == 0 )
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		return;
#endif // SERV_SECOND_SECURITY_REWARD_ID

		// ���� 6 ���� ���� �����̸�
		CTime tLastLogin;
		CTime tNow = CTime::GetCurrentTime();
		CTime tRestart = CTime( tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), 6, 0, 0 );		// �ʱ�ȭ ����
		CTimeSpan tOneDay = CTimeSpan( 1, 0, 0, 0 );
		CTime tYesterRestart = tRestart - tOneDay;		// ���� �ʱ�ȭ ����

		if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrCurrTime, tLastLogin ) == false )
		{
			START_LOG( cerr, L"�ð��� ���ڿ��� ��ȯ ����!!!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( kAck.m_iOK )
				<< BUILD_LOG( kPacket_.m_wstrCurrTime )
				<< END_LOG;
			return;
		}
		
		if( tLastLogin >= tYesterRestart )	// ���� �޾Ҵ�
		{
			if( tLastLogin < tRestart )	// ���� ���� ������ �ȴ�
			{
				if( tRestart <= tNow )
				{
					//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
					m_kUserUnitSelectRewardManager.AddRewardItemInfo( ERI_SECOND_SECURITY );
#else
					m_kUserUnitSelectRewardManager.AddRewardItemInfo( KRewardTable::ERI_SECOND_SECURITY );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
					//}}
				}
			}
		}
		else
		{
			//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
			m_kUserUnitSelectRewardManager.AddRewardItemInfo( ERI_SECOND_SECURITY );
#else
			m_kUserUnitSelectRewardManager.AddRewardItemInfo( KRewardTable::ERI_SECOND_SECURITY );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
			//}}
		}
	}
}

_IMPL_ON_FUNC( DBE_FAILED_SECOND_SECURITY_ACK, int )
{
	// ���� ���� ����� �Ϸ� �Ǿ���  // ���� ī��Ʈ�� ��� �´�
	KEGS_AUTH_SECOND_SECURITY_ACK kAck;

	SetSuccessSecondSecurity( false );
	SetSecondPWFailedCount( kPacket_ );
	kAck.m_iFailedCount = kPacket_;

	if( kPacket_ < 10 )
	{
		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_AUTH_FAILED;
	}
	else
	{
		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_AUTH_FAILED_LIMIT;
	}

	SendPacket( EGS_AUTH_SECOND_SECURITY_ACK, kAck );
}

IMPL_ON_FUNC( EGS_CREATE_SECOND_SECURITY_REQ )
{
	KEGS_CHANGE_SECOND_SECURITY_PW_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

#ifdef SERV_DISABLE_SECOND_SECURITY
	START_LOG( clog, L"2�� �����е带 ���Ҵµ� Ŭ����ŷ���� ���Գ� ��" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( kPacket_.m_wstrSecondPW )
		<< END_LOG;

	SendPacket( EGS_CREATE_SECOND_SECURITY_ACK, kAck );
	return;
#endif SERV_DISABLE_SECOND_SECURITY

	// ���ڿ� ���Ἲ �˻�( ���ڷθ�, 4~6�ڸ� �ΰ�? )
	if( CheckIntegritySecondSecurityPW( kPacket_.m_wstrSecondPW, kAck.m_iOK ) == false )
	{
		START_LOG( clog, L"2�� ���� ��й�ȣ�� ���� �̻��մϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_wstrSecondPW )
			<< END_LOG;

		SendPacket( EGS_CREATE_SECOND_SECURITY_ACK, kAck );
		return;
	}

	KDBE_CREATE_SECOND_SECURITY_REQ kReq;
	kReq.m_iUserID = GetUID();
	//{{  2011.12.20     ��μ�    2�� ���� �е� ��й�ȣ ��ȣȭ
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	// ��ȣȭ �Ͽ� DB�� ����
	std::string strPW = KncUtil::toNarrowString( kPacket_.m_wstrSecondPW );
	START_LOG( clog, L"2�� ���� ��й�ȣ�� ���� �̻��մϴ�." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( strPW )
		<< END_LOG;
	std::string strChangePW = KServerSNMaker::GetCRCFromString( strPW );
	START_LOG( clog, L"2�� ���� ��й�ȣ�� ���� �̻��մϴ�." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( strChangePW )
		<< END_LOG;
	kReq.m_wstrSecondPW = KncUtil::toWideString( strChangePW.c_str() );
	START_LOG( clog, L"2�� ���� ��й�ȣ�� ���� �̻��մϴ�." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( kReq.m_wstrSecondPW )
		<< END_LOG;
#else
	kReq.m_wstrSecondPW = kPacket_.m_wstrSecondPW;
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
	//}}
	SendToAccountDB( DBE_CREATE_SECOND_SECURITY_REQ, kReq );
}

IMPL_ON_FUNC( DBE_CREATE_SECOND_SECURITY_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetIsUseSecondPW( true );
		SetSecondPWFailedCount( 0 );
		SetSecondPW( kPacket_.m_wstrSecondPW );
		SetSuccessSecondSecurity( true );

		START_LOG( clog, L"2�� ���� ���� ����!!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_wstrSecondPW )
			<< END_LOG;
	}

	//{{  2011.12.20     ��μ�    2�� ���� �е� ��й�ȣ ��ȣȭ
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	// �˷��ָ� �ȵǴϱ� ������
	kPacket_.m_wstrSecondPW = L"";
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
	//}}

	SendPacket( EGS_CREATE_SECOND_SECURITY_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_DELETE_SECOND_SECURITY_REQ )
{
	KEGS_DELETE_SECOND_SECURITY_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	if( kPacket_.m_wstrSecondPW.empty() == true )
	{
		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DEFECT_PW;
		SendPacket( EGS_DELETE_SECOND_SECURITY_ACK, kAck );
		return;
	}

	// ���� ��й�ȣ�� ��
	//{{  2011.12.20     ��μ�    2�� ���� �е� ��й�ȣ ��ȣȭ
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	// ��ȣȭ
	std::string strPW = KncUtil::toNarrowString( kPacket_.m_wstrSecondPW );
	START_LOG( clog, L"2�� ���� ��й�ȣ�� ���� �̻��մϴ�." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( strPW )
		<< END_LOG;
	std::string strChangePW = KServerSNMaker::GetCRCFromString( strPW );
	START_LOG( clog, L"2�� ���� ��й�ȣ�� ���� �̻��մϴ�." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( strChangePW )
		<< END_LOG;
	kPacket_.m_wstrSecondPW = KncUtil::toWideString( strChangePW.c_str() );
	START_LOG( clog, L"2�� ���� ��й�ȣ�� ���� �̻��մϴ�." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( kPacket_.m_wstrSecondPW )
		<< END_LOG;
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
	//}}

	if( IsSameSecondSecurityPW( kPacket_.m_wstrSecondPW ) == false )
	{
		START_LOG( clog, L"2�� ���� ��й�ȣ�� ���� Ʋ�Ƚ��ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_wstrSecondPW )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DIFFER_PW;
	}

	SendPacket( EGS_DELETE_SECOND_SECURITY_ACK, kAck );
}

IMPL_ON_FUNC( EGS_DELETE_SECOND_SECURITY_VERIFY_REQ )
{
	KEGS_DELETE_SECOND_SECURITY_VERIFY_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	if( kPacket_.m_wstrSecondPW.empty() == true )
	{
		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DEFECT_PW;
		SendPacket( EGS_DELETE_SECOND_SECURITY_ACK, kAck );
		return;
	}

	// ���� ��й�ȣ�� ��
	//{{  2011.12.20     ��μ�    2�� ���� �е� ��й�ȣ ��ȣȭ
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	// ��ȣȭ
	std::string strPW = KncUtil::toNarrowString( kPacket_.m_wstrSecondPW );
	START_LOG( clog, L"2�� ���� ��й�ȣ�� ���� �̻��մϴ�." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( strPW )
		<< END_LOG;
	std::string strChangePW = KServerSNMaker::GetCRCFromString( strPW );
	START_LOG( clog, L"2�� ���� ��й�ȣ�� ���� �̻��մϴ�." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( strChangePW )
		<< END_LOG;
	kPacket_.m_wstrSecondPW = KncUtil::toWideString( strChangePW.c_str() );
	START_LOG( clog, L"2�� ���� ��й�ȣ�� ���� �̻��մϴ�." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( kPacket_.m_wstrSecondPW )
		<< END_LOG;
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
	//}}
	if( IsSameSecondSecurityPW( kPacket_.m_wstrSecondPW ) == false )
	{
		START_LOG( clog, L"2�� ���� ��й�ȣ�� ���� Ʋ�Ƚ��ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_wstrSecondPW )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DIFFER_PW;
		SendPacket( EGS_DELETE_SECOND_SECURITY_ACK, kAck );
		return;
	}

	if( kPacket_.m_bDelete )
	{
		KDBE_DELETE_SECOND_SECURITY_REQ kReq;
		kReq.m_iUserID = GetUID();
		GetSecondPW( kReq.m_wstrSecondPW );

		SendToAccountDB( DBE_DELETE_SECOND_SECURITY_REQ, kReq );
		return;
	}

	kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DELETE_FAILE;
	SendPacket( EGS_DELETE_SECOND_SECURITY_VERIFY_ACK, kAck );
}

IMPL_ON_FUNC( DBE_DELETE_SECOND_SECURITY_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetIsUseSecondPW( false );
		SetSecondPWFailedCount( 0 );
		SetSuccessSecondSecurity( false );

		START_LOG( clog, L"2�� ���� ���� ����!!" )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;
	}

	KEGS_DELETE_SECOND_SECURITY_VERIFY_ACK kAck;
	kAck.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_DELETE_SECOND_SECURITY_VERIFY_ACK, kAck );
}

IMPL_ON_FUNC( EGS_CHANGE_SECOND_SECURITY_PW_REQ )
{
	KEGS_CHANGE_SECOND_SECURITY_PW_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	// ���ڿ� ���Ἲ �˻�( ���ڷθ�, 4~6�ڸ� �ΰ�? )
	if( CheckIntegritySecondSecurityPW( kPacket_.m_wstrNewSecondPW, kAck.m_iOK ) == false || 
		CheckIntegritySecondSecurityPW( kPacket_.m_wstrOldSecondPW, kAck.m_iOK ) == false )
	{
		START_LOG( clog, L"2�� ���� ��й�ȣ�� ���� �̻��մϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_wstrNewSecondPW )
			<< BUILD_LOG( kPacket_.m_wstrOldSecondPW )
			<< END_LOG;

		SendPacket( EGS_CHANGE_SECOND_SECURITY_PW_ACK, kAck );
		return;
	}

	// ���� ��й�ȣ�� ��
	//{{  2011.12.20     ��μ�    2�� ���� �е� ��й�ȣ ��ȣȭ
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	// ��ȣȭ
	std::string strOldPW = KncUtil::toNarrowString( kPacket_.m_wstrOldSecondPW );
	std::string strChangeOldPW = KServerSNMaker::GetCRCFromString( strOldPW );
	kPacket_.m_wstrOldSecondPW = KncUtil::toWideString( strChangeOldPW.c_str() );
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
	//}}
	if( IsSameSecondSecurityPW( kPacket_.m_wstrOldSecondPW ) == false )
	{
		START_LOG( clog, L"2�� ���� ��й�ȣ�� ���� Ʋ�Ƚ��ϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_wstrNewSecondPW )
			<< BUILD_LOG( kPacket_.m_wstrOldSecondPW )
			<< END_LOG;

		KEGS_CHANGE_SECOND_SECURITY_PW_ACK kAck;
		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DIFFER_PW;

		SendPacket( EGS_CHANGE_SECOND_SECURITY_PW_ACK, kAck );
		return;
	}

	// ���� ��й�ȣ�� ������ ������� ���� �Ϸ� �� ��
	//{{  2011.12.20     ��μ�    2�� ���� �е� ��й�ȣ ��ȣȭ
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	// ��ȣȭ
	std::string strNewPW = KncUtil::toNarrowString( kPacket_.m_wstrNewSecondPW );
	std::string strChangeNewPW = KServerSNMaker::GetCRCFromString( strNewPW );
	kPacket_.m_wstrNewSecondPW = KncUtil::toWideString( strChangeNewPW.c_str() );
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
	//}}
	if( IsSameSecondSecurityPW( kPacket_.m_wstrNewSecondPW ) == true )
	{
		START_LOG( clog, L"2�� ������ ����й�ȣ�� ���� ��й�ȣ�� �����մϴ�." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_wstrNewSecondPW )
			<< BUILD_LOG( kPacket_.m_wstrOldSecondPW )
			<< END_LOG;

		KEGS_CHANGE_SECOND_SECURITY_PW_ACK kAck;
		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DEFECT_PW_3;

		SendPacket( EGS_CHANGE_SECOND_SECURITY_PW_ACK, kAck );
		return;
	}

	KDBE_CHANGE_SECOND_SECURITY_PW_REQ kReq;
	kReq.m_iUserID = GetUID();
	kReq.m_wstrOldSecondPW = kPacket_.m_wstrOldSecondPW;
	kReq.m_wstrNewSecondPW = kPacket_.m_wstrNewSecondPW;

	SendToAccountDB( DBE_CHANGE_SECOND_SECURITY_PW_REQ, kReq );
}

IMPL_ON_FUNC( DBE_CHANGE_SECOND_SECURITY_PW_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetSecondPW( kPacket_.m_wstrSecondPW );
		SetSecondPWFailedCount( 0 );

		START_LOG( clog, L"2�� ���� ��й�ȣ ���� ����!!" )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;
	}

	KEGS_CHANGE_SECOND_SECURITY_PW_ACK kAck;
	kAck.m_iOK = kPacket_.m_iOK;

	SendPacket( EGS_CHANGE_SECOND_SECURITY_PW_ACK, kAck );
}

IMPL_ON_FUNC( EGS_COMPARE_SECOND_SECURITY_PW_REQ )
{
	KEGS_COMPARE_SECOND_SECURITY_PW_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	//{{  2011.12.20     ��μ�    2�� ���� �е� ��й�ȣ ��ȣȭ
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	// ��ȣȭ
	std::string strPW = KncUtil::toNarrowString( kPacket_ );
	std::string strChangePW = KServerSNMaker::GetCRCFromString( strPW );
	kPacket_ = KncUtil::toWideString( strChangePW.c_str() );
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
	//}}

	// 2�� ���� �е� ��� ���̰� ��й�ȣ�� ���� �� ��
	if( GetIsUseSecondPW() == false )
	{
		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_NO_USE;
	}
	else if( IsSameSecondSecurityPW( kPacket_ ) == false )
	{
		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DIFFER_PW;
	}

	SendPacket( EGS_COMPARE_SECOND_SECURITY_PW_ACK, kAck );
}

IMPL_ON_FUNC( EGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_REQ )
{
    KEGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_ACK kAck;
    kAck.m_iOK = NetError::NET_OK;

    // 2011.12.20     ��μ�    2�� ���� �е� ��й�ȣ ��ȣȭ
    std::string strPW = KncUtil::toNarrowString( kPacket_ );
    std::string strChangePW = KServerSNMaker::GetCRCFromString( strPW );
    kPacket_ = KncUtil::toWideString( strChangePW.c_str() );

    // 2�� ���� �е� ��� ���̰� ��й�ȣ�� ���� �� �� ���
    if( GetIsUseSecondPW() == false )
    {
        kAck.m_iOK = NetError::ERR_SECOND_SECURITY_NO_USE;
    }
    else if( IsSameSecondSecurityPW( kPacket_ ) == false )
    {
        kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DIFFER_PW;
    }
    else 
    {
#ifdef SERV_CHECK_SECOND_PW_STATE
        StateTransition( KGSFSM::I_TO_SERVER_SELECT );
#endif SERV_CHECK_SECOND_PW_STATE
    }

    SendPacket( EGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_ACK, kAck );
}

#endif SERV_SECOND_SECURITY
//}}

//{{ 2011. 08. 09	������	�ӽ�ID �����
#ifdef SERV_MACHINE_ID_BLOCK
IMPL_ON_FUNC( EGS_CHECK_MACHINE_ID_REQ )
{
	// ���� ������ �����ϴ�!

	// ���� ��� ���� ����
	if( SiKGameSysVal()->IsMachineIDBlock() == false )
	{
		KEGS_CHECK_MACHINE_ID_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::NET_OK;
		kPacketAck.m_strMachineID = kPacket_.m_strMachineID;
        SendPacket( EGS_CHECK_MACHINE_ID_ACK, kPacketAck );
		return;
	}

	// ���ڿ� ���� üũ!
	if( kPacket_.m_strMachineID.empty()  ||  kPacket_.m_strMachineID.size() > 24 )
	{
		START_LOG( cerr, L"�ӽ�ID���� �̻��մϴ�! ���� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( kPacket_.m_strMachineID )
			<< END_LOG;

		//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
		KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_MACHINE_ID;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
		//}}
#else
		//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
		KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_MACHINE_ID;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
		//}}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//}}

		Kick( NetError::ERR_CHECK_MACHINE_ID_00 );
		return;
	}

	// ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	std::wstring wstrMachineID = KncUtil::toWideString( kPacket_.m_strMachineID );
	if( KODBC::IsInvalidMarkInForMachineID( wstrMachineID ) == true )
	{
		START_LOG( cerr, L"�ӽ�ID���� �̻��մϴ�! ���� ����!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( kPacket_.m_strMachineID )
			<< END_LOG;

		//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
		KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_MACHINE_ID;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
		//}}
#else
		//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
		KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_MACHINE_ID;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
		//}}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//}}

		Kick( NetError::ERR_CHECK_MACHINE_ID_00 );
		return;
	}

	//{{ 2011. 09. 07	������	�ӽ� ID ��� ������Ʈ
#ifdef SERV_MACHINE_ID_BLOCK_UPDATE
	if( m_kUserSecurityManager.CheckValidMachineID( kPacket_.m_strMachineID ) == false )
	{
		START_LOG( cout, L"[�˸�] �ӽ�ID�� ��ȿ���� �ʴ�! ���� ���� ��Ű��!" )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_strMachineID );

		//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_MACHINE_ID;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );
#else
		KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_MACHINE_ID;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//}}
		
		// �ӽ�ID�� ��������� ���� ����!
		Kick( NetError::ERR_CHECK_MACHINE_ID_00 );
		return;
	}
#endif SERV_MACHINE_ID_BLOCK_UPDATE
	//}}

	SendToLoginServer( ELG_CHECK_MACHINE_ID_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ELG_CHECK_MACHINE_ID_ACK, KEGS_CHECK_MACHINE_ID_ACK )
{
	// ���� ������ �����ϴ�!

	// �ϴ� �����ص���!
	m_kUserSecurityManager.SetMachineID( kPacket_.m_strMachineID );

	// ���� �����ߴٸ� ���� ����!
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
		KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_BLOCK_MACHINE_ID;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
		//}}
#else
		//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
		KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_BLOCK_MACHINE_ID;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
		//}}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//}}

		Kick( NetError::ERR_CHECK_MACHINE_ID_00 );
		return;
	}

	SendPacket( EGS_CHECK_MACHINE_ID_ACK, kPacket_ );
}
#endif SERV_MACHINE_ID_BLOCK
//}}

//{{ 2012. 01. 27  ��μ�	��ŷ ���μ��� �˻� ���
#ifdef SERV_HACKING_DLL_CHECK
IMPL_ON_FUNC( ELG_CHECK_MODULE_INFO_UPDATE_NOT )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// Ŭ���̾�Ʈ�� ������ ������
		//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_CLIENT_DLL;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );

		// ��ŷ �ǽ������� ����Ѵ�.
		HackUserRegMonitorsUser( KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_CLIENT_DLL );
#else
		KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_CLIENT_DLL;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );

		// ��ŷ �ǽ������� ����Ѵ�.
		HackUserRegMonitorsUser( KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_CLIENT_DLL );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//}}
	
		// �α� ���
		bool bWrite = false;
		KDBE_CHECK_MODULE_INFO_LOG_NOT kLog;
		
		std::map< std::wstring, KModuleInfo >::iterator mit = kPacket_.m_mapModuleName.begin();
		for(  ; mit != kPacket_.m_mapModuleName.end() ; ++mit )
		{
			if( mit->second.m_iAllow == KModuleInfo::MIA_MONITORING )
			{
				kLog.m_vecMonitoringDLL.push_back( mit->second );
			}
			else if( mit->second.m_iAllow == KModuleInfo::MIA_BLACK_LIST )
			{
				kLog.m_vecBlackListDLL.push_back( mit->second );
			}	
		}

		if( kLog.m_vecMonitoringDLL.empty() == false )  // ����͸��� �켱�����̴�.
		{
			// TradeBlock�� ����ϱ�!
			//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			HackUserRegTradeBlock( KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_CLIENT_DLL );

			m_kUserHackingManager.SetHackingUserSendType( KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_CLIENT_DLL );
#else
			HackUserRegTradeBlock( KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_CLIENT_DLL );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			//}}

			bWrite = true;

			START_LOG( clog, L"DLL üũ���� �ɷȱ���~�ŷ� ����" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( KModuleInfo::MIA_MONITORING )
				<< END_LOG;
		}
		else if( kLog.m_vecBlackListDLL.empty() == false )
		{
			//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
			kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_CLIENT_BLOCK_DLL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
#else
			//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
			kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_CLIENT_BLOCK_DLL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			//}}
			
			SetDisconnectReason( KStatistics::eSIColDR_Client_Hacking );
			ReserveDestroy();

			bWrite = true;

			START_LOG( clog, L"DLL üũ���� �ɷȱ���~ű ����" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( KModuleInfo::MIA_BLACK_LIST )
				<< END_LOG;
		}

		if( bWrite == true )
		{
			// �α� �������
			kLog.m_iUserUID = GetUID();
			m_kUserSecurityManager.GetServerSN( kLog.m_iServerSN );

			SendToLogDB( DBE_CHECK_MODULE_INFO_LOG_NOT, kLog );

			START_LOG( clog, L"�α� ����Ϸ� ����" )
				<< BUILD_LOG( kLog.m_iUserUID )
				<< BUILD_LOG( kLog.m_iServerSN )
				<< END_LOG;
		}
	}
}
#endif SERV_HACKING_DLL_CHECK
//}}

//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
IMPL_ON_FUNC( EGS_SERVER_CHECK_HACK_USER_ACK )
{
	char cLastSendType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_NONE;
	if( m_kUserHackingManager.SetHackingUserReciveType( kPacket_.m_cHackingCheckType, cLastSendType ) == false )
	{
		START_LOG( cerr, L"��ŷ ���� ��Ŷ�� ���´µ� �߸��� �����̴�." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOGc( cLastSendType )
			<< BUILD_LOGc( kPacket_.m_cHackingCheckType )
			<<END_LOG;

		// Ŭ�󿡰� ��Ŷ�� ����
		SendID( EGS_CHECK_HACK_USER_PACKET_REQUEST_NOT );

		// ű����
		if( SiKGameSysVal()->GetHackingUserKick() == true )
		{
			SetDisconnectReason( KStatistics::eSIColDR_Client_Hacking );
			ReserveDestroy();
		}
	}
}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//}}

#ifdef SERV_NPROTECT_CS_AUTH_30
IMPL_ON_FUNC( EGS_NPROTECT_CSAUTH3_RESPONE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	static const size_t MaxBufferSize = 4096;

	size_t ReceivedSize = min(kPacket_.m_deqCSAuth3.size(), MaxBufferSize);
	size_t index = 0;
	BOOST_FOREACH(BYTE b, kPacket_.m_deqCSAuth3)
	{
		m_CSAuth3.packet[index] = b;
		++index;
		if(index >= ReceivedSize)
			break;
	}

	UINT32 uReturn = m_CSAuth3.Check( ReceivedSize );

	if( uReturn >= 3000 )
	{
		START_LOG( cerr, L"nProtect CS ����3 ���� ����" )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( uReturn )
			<< END_LOG;
		SetDisconnectReason( KStatistics::eSIColDR_nProtect_Error );
		ReserveDestroy();

		return;
	}
}

IMPL_ON_FUNC( EGS_NPROTECT_CSAUTH3_GAMEHACK_REPORT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// �������� �߰ߵǾ����� ���� �����մϴ�. 
	// ����Ŭ���̾�Ʈ�� ���޹��� �����͸� ������ �����ϰ� ������ �մϴ�.

	const size_t MaxBufferSize = 4096;

	// Hack report �� Ŭ���̾�Ʈ���� ���޵� ���
	size_t ReceivedSize = max(kPacket_.m_deqCSAuth3.size(), MaxBufferSize);
	size_t index = 0;
	BOOST_FOREACH(BYTE b, kPacket_.m_deqCSAuth3)
	{
		m_CSAuth3.packet[index] = b;
		++index;
		if(index >= ReceivedSize)
			break;
	}

	// TODO: �׼����� ����Ű�� �ٲܼ� �ֵ��� ���Ǳ׷� ������

	// �ؿ��� ������ ���� ������
#if defined( SERV_COUNTRY_JP )
	UINT32 uReturn = DecryptHackData( "ElswordJP", m_CSAuth3.packet, ReceivedSize );
#elif defined( SERV_COUNTRY_CN )
	UINT32 uReturn = DecryptHackData( "ElswordCN", m_CSAuth3.packet, ReceivedSize );
#elif defined( SERV_COUNTRY_TH )
	UINT32 uReturn = DecryptHackData( "ElswordTH", m_CSAuth3.packet, ReceivedSize );
#elif defined( SERV_COUNTRY_ID )
	UINT32 uReturn = DecryptHackData( "ElswordID", m_CSAuth3.packet, ReceivedSize );
#elif defined( SERV_COUNTRY_BR )
	UINT32 uReturn = DecryptHackData( "ElswordBR", m_CSAuth3.packet, ReceivedSize );
#elif defined( SERV_COUNTRY_PH )
	UINT32 uReturn = DecryptHackData( "ElswordPH", m_CSAuth3.packet, ReceivedSize );
#endif

	if( uReturn == -1 )
	{
		// ����	
		START_LOG( cerr, L"nProtect ������ ����Ʈ ��Ŷ ��ȣȭ ����." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< END_LOG;
	}

	START_LOG( cout, L"nProtect ������ ����Ʈ." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetName() )
		<< BUILD_LOG( uReturn )
		<< END_LOG;
}

#endif SERV_NPROTECT_CS_AUTH_30

//-- ���� : XTRAP ���� - Ŭ�� ���� ��Ŷ ó���κ�(Step3)
#ifdef SERV_USE_XTRAP
IMPL_ON_FUNC( EGS_XTRAP_ACK )
{
	if(m_cXTrapCSM.CSMStep3( &kPacket_ ) == false)
	{
		// ����3���� ���� �ʴ°� �����Դϴ�.
		//SetDisconnectReason( KStatistics::eSIColDR_nProtect_Hacking );
		//ReserveDestroy();

		START_LOG( cerr, L"XTRAP ��ŷ Ŭ���̾�Ʈ  UserUID : " << GetUID() );
		return;
	}
}
#endif SERV_USE_XTRAP

#ifdef SERV_FIX_NONE_NPC_DUNGEON_LINES// �۾���¥: 2013-05-22	// �ڼ���
IMPL_ON_FUNC_NOPARAM( ERM_DUNGEON_KILLALLNPC_CHECK_NOT )
{
	SendID( EGS_DUNGEON_KILLALLNPC_CHECK_NOT );
}
#endif // SERV_FIX_NONE_NPC_DUNGEON_LINES

#ifdef SERV_HACKING_USER_CHECK_COUNT// �۾���¥: 2013-06-02	// �ڼ���
IMPL_ON_FUNC( DBE_HACKING_USER_CHECK_COUNT_INFO_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"��ŷ ���� üũ ī��Ʈ�� �ʱ�ȭ �� �� �����ϴ�." )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_byteType )
			<< BUILD_LOG( kPacket_.m_wstrReason )
			<< END_LOG;
		return;
	}
	
	m_kUserAbuserManager.InitCheckCount( kPacket_.m_byteType, kPacket_.m_byteBlockCheckCount, kPacket_.m_wstrLastResetDate );
	IncreaseCheckCount( kPacket_.m_byteType, kPacket_.m_wstrReason, true );
}
#endif // SERV_HACKING_USER_CHECK_COUNT

#ifdef SERV_HACKING_USER_PROCESS_NAME_RECORD// �۾���¥: 2013-06-03	// �ڼ���
IMPL_ON_FUNC( EGS_HACKING_USER_PROCESS_NAME_RECORD_NOT )
{
	switch( kPacket_.m_byteType )
	{
	case SEnum::HUCT_CLIENT_HACKING_PROCESS:
	case SEnum::HUCT_CLIENT_HACKING_TEXT:
	case SEnum::HUCT_CLIENT_HACKING_MODULE:
	case SEnum::HUCT_CLIENT_HACKING_DLL_MANAGER:
	case SEnum::HUCT_CLIENT_HACKING_ZOMBIE_PROCESS:
	case SEnum::HUCT_CLIENT_HACKING_THREAD_TITLE:
		IncreaseCheckCount( kPacket_.m_byteType, kPacket_.m_wstrReason );
		break;

	default:
		START_LOG( cerr, L"����� �� ���� Ŭ���̾�Ʈ ��ŷ Ÿ���� ���Խ��ϴ�." )
			<< BUILD_LOG( kPacket_.m_byteType )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
	}
}
#endif // SERV_HACKING_USER_PROCESS_NAME_RECORD

#ifdef SERV_ENTRY_POINT

IMPL_ON_FUNC_NOPARAM( EGS_GET_SECOND_SECURITY_INFO_REQ )
{
    KDBE_GET_PURE_SECOND_SECURITY_INFO_REQ kReq;
    kReq.m_iUserUID = GetUID();
    //SiKGameSysVal()->GetComeBackRewardCondition( kReq.m_mapComeBackRewardCondition );
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
    kReq.m_wstrID = GetName();
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT

    SendToAccountDB( DBE_GET_PURE_SECOND_SECURITY_INFO_REQ, kReq );	// AccountDB�� �ֱ� ���� ���� ���� ���
}

IMPL_ON_FUNC( DBE_GET_PURE_SECOND_SECURITY_INFO_ACK )
{
    KEGS_SECOND_SECURITY_INFO_NOT kNot;
    kNot.m_iOK = kPacket_.m_iOK;
    kNot.m_bUseSecondPW = kPacket_.m_bUseSecondPW;

    if( kPacket_.m_iOK == NetError::ERR_SECOND_SECURITY_AUTH_FAILED_LIMIT ) // ��� ��ȣ �Է� ���а� 10ȸ �̻��� �� 
    {
        SendPacket( EGS_GET_SECOND_SECURITY_INFO_ACK, kNot );

        // Ŭ���̼� ���� ��Ű��� �Ͽ� �ּ� ��
        //SetDisconnectReason( KStatistics::eSIColDR_Normal_Disconnect );
        //ReserveDestroy();															// ���� ����
        return;
    }

    // DB ���� �޾ƿ� ������ ���� (���� �� ��)
    CTime tLastAuthDate;
    if( kPacket_.m_bUseSecondPW == true )
    {
        if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrLastAuthDate, tLastAuthDate ) == false )
        {
            START_LOG( cerr, L"���ڿ� �ð� ��ȯ ����." )
                << BUILD_LOG( kPacket_.m_wstrLastAuthDate )
                << END_LOG;
        }

        //{{ 2011. 06. 23	������	2�� ��� ����ó��
#ifdef SERV_CHECK_SECOND_PW_STATE
        //StateTransition( KGSFSM::I_TO_CHECK_SECOND_PW );
#endif SERV_CHECK_SECOND_PW_STATE
        //}}
    }
    SetIsUseSecondPW( kPacket_.m_bUseSecondPW );
    SetSecondPWFailedCount( kPacket_.m_iFailedCount );
    SetSecondPW( kPacket_.m_wstrSecondPW );
    SetLastAuthDate( tLastAuthDate );

    SendPacket( EGS_GET_SECOND_SECURITY_INFO_ACK, kNot );

    if( m_kUserStatistics.IsFirstUserLoginToday( GetUID(), GetLastLoginDate() ) == true )
    {  // ���� ��� ���� �α� ������!
        SendFirstLoginSecurityInfo();
    }
}

#endif SERV_ENTRY_POINT

//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


