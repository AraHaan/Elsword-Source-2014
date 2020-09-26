#include "ChannelUser.h"
#include <dbg/dbg.hpp>
//{{ 2012. 07. 02	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
	#include "CHFSM.h"
#else
	#include "defaultFSM.h"
#endif SERV_WAIT_QUEUE_SYSTEM
//}}

#include "DBLayer.h"
#include "NetError.h"
#include "ChannelSimLayer.h"
#include "ChannelServer.h"
//#include "GashAuthManager.h"
//{{ 2009. 12. 16  ������	������
#include "CCUToolManager.h"
//}}

//{{ 2011. 01. 20 ��μ�	���� ����͸� ��               ���Ŀ��������
#ifdef SERV_CCU_MONITORING_TOOL
#include "odbc/Odbc.h"
#endif SERV_CCU_MONITORING_TOOL
//}}

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
#include "ProxyManager.h"
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

#include <boost/format.hpp>

//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	#include "MachineBlockManager.h"
	#include "HackingProtectionManager.h"
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
//}}

//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	#include "GameSysVal/GameSysVal.h"
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
//}}

//{{ 2011.11.02     ��μ�    �������� SN �߱� �� ��ȿ�� �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	#include "ServerSNMaker.h"
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
//}}
//{{ 2012. 07. 02	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
	#include "WaitQueueManager.h"
#endif SERV_WAIT_QUEUE_SYSTEM
//}}
//{{ 2012. 07. 18	������		������ ����
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	#include "DDOSGuardSimulator.h"
#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
//}}

NiImplementRTTI( KChannelUser, KActor );
ImplPfID( KChannelUser, PI_CHANNEL_USER );

#define CLASS_TYPE  KChannelUser

KChannelUser::KChannelUser(void) 
{
	//{{ 2009. 10. 14  ������	����	
	InitRepeatEventFilter();
	//}}

	//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	m_bVerifyAccount = false;
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}
	//{{ 2012. 07. 04	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
	m_bPassedEnterChannel = false;
#endif SERV_WAIT_QUEUE_SYSTEM
	//}}
	//{{ 2012. 07. 18	������		������ ����
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	m_bPassedDDOSGuardAuth = false;
#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	//}}
#ifdef SERV_GLOBAL_AUTH
	m_wstrID = L"";
#ifdef SERV_PUBLISHER_MATRIXKEY
	m_uiMatrixKey = 0;
#endif //SERV_PUBLISHER_MATRIXKEY
#ifdef SERV_COUNTRY_TH
	m_wstrMasterID = L"";
	m_wstrSocketID = L"";
#endif //SERV_COUNTRY_TH
#endif //SERV_GLOBAL_AUTH
}

KChannelUser::~KChannelUser(void)
{
}

ImplToStringW( KChannelUser )
{
    return START_TOSTRING_PARENTW( KActor );
}

void KChannelUser::ProcessEvent( const KEventPtr& spEvent_ )
{
	//{{ 2012. 07. 18	������		������ ����
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	//static std::string strCheck = "192.168.66.17";
	//std::string strIP = GetIPStr();
	//if( strIP == strCheck )
	//{
	//	if( m_bPassedDDOSGuardAuth == false )
	//	{
	//		m_bPassedDDOSGuardAuth = SiKDDOSGuardSimulator()->IsRegUserSession( GetIPStr() );
	//		if( m_bPassedDDOSGuardAuth == false )
	//		{
	//			START_LOG( cout, L"[�׽�Ʈ�α�] DDOS ���� �ùķ����� ����!! ������ �����մϴ�!" )
	//				<< BUILD_LOG( GetUID() )
	//				<< BUILD_LOG( GetIPStr() );

	//			// ���� ���� ���!
	//			SiKDDOSGuardSimulator()->RegUserSession( GetIPStr() );

	//			// ���� ���� ó��!
	//			m_bDestroyReserved = true;
	//			return;
	//		}
	//	}
	//}	
#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	//}}

    if( RoutePacket( spEvent_.get() ) )
        return;

	//{{ 2008. 8. 28  ������	�̺�Ʈ ����
	if( !CheckReqEvent( spEvent_->m_usEventID ) )
		return;

    DWORD dwElapTime = ::GetTickCount();  // 040428. ��Ŷ ó�� �ҿ�ð��� ���Ѵ�.

    switch( spEvent_->m_usEventID )
    {
    CASE( ECH_VERIFY_ACCOUNT_REQ );
#ifndef SERV_GLOBAL_AUTH 
    CASE( ECH_VERIFY_ACCOUNT_ACK );	// �ؿܿ��� ������
#endif // SERV_GLOBAL_AUTH
    //CASE( DBE_GASH_USER_LOGIN_ACK );// �ؿܿ��� ���� ������
	CASE_NOPARAM( ECH_GET_CHANNEL_LIST_REQ );
	CASE_NOPARAM( ECH_DISCONNECT_REQ );

	//{{ 2009. 12. 16  ������	������
	CASE( E_TOOL_GET_CCU_INFO_REQ );
	CASE( DBE_GET_CONCURRENT_USER_INFO_ACK );
	//}}

	CASE_NOPARAM( E_RESERVE_DESTROY );

	//{{ 2011. 01. 20 ��μ�	���� ����͸� ��
#ifdef SERV_CCU_MONITORING_TOOL
	CASE( E_TOOL_CHECK_LOGIN_REQ );
	CASE_NOPARAM( E_TOOL_SERVER_LIST_REQ );
	CASE( DBE_GET_CONCURRENT_USER_LOGIN_ACK );
	CASE( DBE_GET_GAME_SERVER_LIST_ACK );
#endif SERV_CCU_MONITORING_TOOL
	//}}

	//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	CASE( ECH_HACKING_TOOL_LIST_ACK );
	CASE( DBE_UPDATE_CHANNEL_RANDOMKEY_ACK );
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}

	//{{  2011.11.08     ��μ�    �������� SN �߱� �� ��ȿ�� �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	CASE( DBE_CHECK_SERVER_SN_ACK );
	CASE( DBE_CREATE_SERVER_SN_ACK );
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	//}}
#ifdef SERVER_GROUP_UI_ADVANCED
	CASE_NOPARAM( ECH_GET_SERVERGROUP_LIST_REQ );
#endif SERVER_GROUP_UI_ADVANCED

#ifdef SERV_GLOBAL_AUTH
	CASE( EPUBLISHER_AUTHENTICATION_ACK );
	CASE( DBE_CH_USER_GENERAL_LOGIN_ACK );
	CASE( DBE_CH_USER_KOGOTP_LOGIN_ACK );

	CASE( ECH_SECURITY_AUTH_REQ );
	CASE( EPUBLISHER_SECURITY_AUTH_ACK );

	CASE( EPUBLISHER_SECURITY_INFO_NOT );
#endif // SERV_GLOBAL_AUTH

#ifdef SERV_SERVER_TIME_GET
	CASE_NOPARAM( ECH_GET_SERVER_TIME_REQ );
#endif SERV_SERVER_TIME_GET

#ifdef SERV_ID_NETMARBLE_PCBANG
	CASE( ECH_PCBANG_IP_AND_MAC_INFO_NOT );
#endif //SERV_ID_NETMARBLE_PCBANG
    default:
        dbg::cerr << L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr();
    }

    dwElapTime = ::GetTickCount() - dwElapTime;
    if( dwElapTime > 3000 )
    {
        START_LOG_WITH_NAME( cwarn )
            << BUILD_LOG( spEvent_->GetIDStr() )
            << BUILD_LOG( spEvent_->m_usEventID )
            << BUILD_LOG( dwElapTime );
    }
}

void KChannelUser::Tick()
{
    KActor::Tick();

	//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST
	CheckHackingToolList();
#endif SERV_HACKING_TOOL_LIST
	//}}

	//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	CheckVerifyAccountRecv();
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}

    switch( GetStateID() ) 
	{
	case KCHFSM::S_LOGINED:
		{
			// ���Ӱ��� IP ������ ����ؾ߸� ä�θ���Ʈ�� �ֵ�����.
			if( m_kTimer[TM_CHANNEL_LIST].elapsed() < 60.f )
				break;

			m_kTimer[TM_CHANNEL_LIST].restart();

#ifdef SERVER_GROUP_UI_ADVANCED
			KECH_GET_SERVERGROUP_LIST_NOT kPacketNot;
			kPacketNot.m_mapServerGroupList = GetKChannelServer()->GetServerGroupList();
			SendPacket( ECH_GET_SERVERGROUP_LIST_NOT, kPacketNot );
#endif SERVER_GROUP_UI_ADVANCED

			//{{ 2012. 07. 02	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
			if( m_bPassedEnterChannel == true )
			{
				SendChannelList();
			}
#else
			KECH_GET_CHANNEL_LIST_NOT kPacket;
			kPacket.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
			kPacket.m_mapChannelList = GetKChannelServer()->GetChannelList();
			//{{ 2012. 07. 2	�ڼ���	ä�� UI ���������� ���� ������ �ý��� ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
			kPacket.m_mapChannelBonusList = GetKChannelServer()->GetChannelBonusList();
#endif SERV_CHANNEL_LIST_RENEWAL
			//}}
			SendPacket( ECH_GET_CHANNEL_LIST_NOT, kPacket );
#endif SERV_WAIT_QUEUE_SYSTEM
			//}}
		}
		break;

		//{{ 2012. 07. 02	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
	case KCHFSM::S_WAIT_QUEUE:
		{
			// ���� �ο����Ѹ�尡 �ƴϸ� üũ���� ���� �ʴ´�.
			switch( SiKWaitQueueManager()->GetMode() )
			{
			case KWaitQueueManager::WQ_NONE:
				{
					// ���� �н�!
					m_bPassedEnterChannel = true;
					SiKWaitQueueManager()->RegPassedUser( GetUID() );

					// ���� �̵�!
					StateTransition( KCHFSM::I_TO_AUTHENTICATED );

					// �ش� �������� �˾��� ���� ���� ��Ŷ ����
					KECH_WAIT_QUEUE_MESSAGE_NOT kPacketNot;
					kPacketNot.m_bPassed = true;
					SendPacket( ECH_WAIT_QUEUE_MESSAGE_NOT, kPacketNot );

					// ä�� ����Ʈ ����!
					SendChannelList();
				}				
				break;

			case KWaitQueueManager::WQ_CCU_LIMIT:
				{
					// ���ť�� üũ����! �ڱ� ���ʰ� ���� ä�θ���Ʈ�� Ŭ���̾�Ʈ���� ������!
					if( SiKWaitQueueManager()->IsExist( GetUID() ) == true )
					{
						if( SiKWaitQueueManager()->IsNextEnterUser( GetUID() ) == true )
						{
							LIF( SiKWaitQueueManager()->UnRegWaitUser( GetUID() ) );

							// ���� �н�!
							m_bPassedEnterChannel = true;
                            SiKWaitQueueManager()->RegPassedUser( GetUID() );

							// ���� �̵�!
							StateTransition( KCHFSM::I_TO_AUTHENTICATED );

							// �ش� �������� �˾��� ���� ���� ��Ŷ ����
							KECH_WAIT_QUEUE_MESSAGE_NOT kPacketNot;
							kPacketNot.m_bPassed = true;
							SendPacket( ECH_WAIT_QUEUE_MESSAGE_NOT, kPacketNot );

							// ä�� ����Ʈ ����!
							SendChannelList();
						}
						else
						{
							if( m_kTimer[TM_WAIT_QUEUE].elapsed() > 5.0 )
							{
								m_kTimer[TM_WAIT_QUEUE].restart();

								// �ش� �������� �˾��� ���� ���� ��Ŷ ����
								KECH_WAIT_QUEUE_MESSAGE_NOT kPacketNot;
								LIF( SiKWaitQueueManager()->GetWaitUserCount( GetUID(), kPacketNot.m_iWaitUserCount ) );
								SendPacket( ECH_WAIT_QUEUE_MESSAGE_NOT, kPacketNot );
							}
						}
					}
				}
				break;

			case KWaitQueueManager::WQ_ENTRY_BAN:
				{
					// ������ �޽����� �����ϴ�.
					if( m_kTimer[TM_WAIT_QUEUE].elapsed() > 5.0 )
					{
						m_kTimer[TM_WAIT_QUEUE].restart();

						// �ش� �������� �˾��� ���� ���� ��Ŷ ����
						KECH_WAIT_QUEUE_MESSAGE_NOT kPacketNot;
						LIF( SiKWaitQueueManager()->GetWaitUserCount( GetUID(), kPacketNot.m_iWaitUserCount ) );
						SendPacket( ECH_WAIT_QUEUE_MESSAGE_NOT, kPacketNot );
					}
				}
				break;

			default:
				// �׿ܿ� �͵��� �ƹ��� ���۵� ���� �ʽ��ϴ�.
				break;
			}
		}
		break;
#endif SERV_WAIT_QUEUE_SYSTEM
		//}}

    case KCHFSM::S_EXIT:
        KActorManager::GetKObj()->ReserveDelete( m_nUID );    // �ڱ� �ڽ��� ������ ������ �� ����.
        break;
    }
}

void KChannelUser::OnDestroy()
{
    KSession::OnDestroy();

    switch( GetStateID() )
	{
    case KCHFSM::S_INIT:
    case KCHFSM::S_LOGINED:
		//{{ 2012. 07. 04	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
	case KCHFSM::S_WAIT_QUEUE:
#endif SERV_WAIT_QUEUE_SYSTEM
		//}}
		{
			//{{ 2012. 07. 10	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
			SiKWaitQueueManager()->UnRegWaitUser( GetUID() );
			SiKWaitQueueManager()->UnRegPassedUser( GetUID() );
#endif SERV_WAIT_QUEUE_SYSTEM
			//}}

			StateTransition( KCHFSM::I_TO_EXIT );    // OnDestroy()�� �Ҹ� ���� KSession::Tick ó�� ���̸�,
			// KLoginUser::Tick ó�� �ܰ迡�� ���� ����ܿ� ��ϵȴ�.
		}
        break;
    }
}

bool KChannelUser::RoutePacket( const KEvent* pkEvent_ )
{
    if( pkEvent_->m_kDestPerformer.m_dwPerformerID != PI_CHANNEL_USER )
    {
        int iEval = CompareServerLevel( pkEvent_->m_kDestPerformer.m_dwPerformerID, PI_CHANNEL_USER );
        if( iEval < 0 )
        {
            _JIF( !pkEvent_->IsEmptyTrace(), return true );
            KEvent kEvent( *pkEvent_ );
            kEvent.PopTrace();
            LIF( KSession::SendPacket( kEvent ) );
            return true;
        }
        else if( iEval > 0 )
        {
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
			KEvent kEvent( *pkEvent_ );
			kEvent.PushTrace( GetUID() );
			SiKProxyManager()->SendPacket( GetUID(), kEvent );

			return true;
#else
            START_LOG( cerr, L"����� ä�� ������ �ֻ��� �����̴�." )
                << BUILD_LOG( pkEvent_->m_kDestPerformer.m_dwPerformerID )
                << BUILD_LOG( pkEvent_->m_usEventID )
                << BUILD_LOG( pkEvent_->GetIDStr() )
                << END_LOG;

            return true;
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY
        }
        else
        {
            KEventPtr spEvent( pkEvent_->Clone() );
            spEvent->PushTrace( GetUID() );

            switch( GetPerformerClass( pkEvent_->m_kDestPerformer.m_dwPerformerID ) )
            {
            case PC_SERVER:
                KBaseServer::GetKObj()->QueueingEvent( spEvent );
                return true;
            case PC_LOG_DB:
                SiKDBLayer()->QueueingEvent( spEvent );
                return true;
            default:
                START_LOG( cerr, L"�������� �̻��� ��Ŷ." )
                    << BUILD_LOG( pkEvent_->m_kDestPerformer.m_dwPerformerID )
                    << END_LOG;
                return true;
            }
        }
    }

    return false;
}

bool KChannelUser::SendID( unsigned short usEventID )
{
	return KSession::SendID( PI_GS_PROXY, GetUID(), NULL, usEventID );
}

void KChannelUser::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}

//{{ 2008. 8. 28  ������	�̺�Ʈ ����
void KChannelUser::InitRepeatEventFilter()
{
	m_kRepeatEventFilter.RegRepeatEvent( ECH_VERIFY_ACCOUNT_REQ, ECH_VERIFY_ACCOUNT_ACK );
	m_kRepeatEventFilter.RegRepeatEvent( ECH_GET_CHANNEL_LIST_REQ, ECH_GET_CHANNEL_LIST_ACK );

	// ��� ī��Ʈ �˻�
	if( m_kRepeatEventFilter.GetRegCount() != m_kRepeatEventFilter.GetReqEventCount()  ||  
		m_kRepeatEventFilter.GetRegCount() != m_kRepeatEventFilter.GetAckEventCount() )
	{
		START_LOG( cerr, L"����� �̺�Ʈ ������ ������ Ʋ���ϴ�. ��Ͻ����� �̺�Ʈ�� ����!" )
			<< BUILD_LOG( m_kRepeatEventFilter.GetRegCount() )
			<< BUILD_LOG( m_kRepeatEventFilter.GetReqEventCount() )
			<< BUILD_LOG( m_kRepeatEventFilter.GetAckEventCount() )
			<< END_LOG;
	}
}

bool KChannelUser::CheckReqEvent( unsigned short usEventID )
{
	switch( usEventID )
	{
	case ECH_VERIFY_ACCOUNT_REQ:
	case ECH_GET_CHANNEL_LIST_REQ:
#ifdef SERV_PUBLISHER_MATRIXKEY
	case ECH_SECURITY_AUTH_REQ:
#endif SERV_PUBLISHER_MATRIXKEY
		{
			return m_kRepeatEventFilter.CheckReqEvent( usEventID );
		}
		break;

	default:
		break;
	}

	return true;
}

void KChannelUser::CheckAckEvent( unsigned short usEventID )
{
	switch( usEventID )
	{
	case ECH_VERIFY_ACCOUNT_ACK:
	case ECH_GET_CHANNEL_LIST_ACK:
		{
			m_kRepeatEventFilter.CheckAckEvent( usEventID );
		}
		break;

	default:
		break;
	}
}

//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST

void KChannelUser::CheckHackingToolList()
{
	if( m_kTimer[TM_HACKING_TOOL_LIST].elapsed() < m_kUserHackingManager.GetHackingToolCheckTime() )
		return;

	m_kTimer[TM_HACKING_TOOL_LIST].restart();

	KEGS_HACKING_TOOL_LIST_NOT kNot;
	m_kUserHackingManager.GetHackingToolList( kNot.m_iRandomKey, kNot.m_vecHackingToolList );
	SendPacket( EGS_HACKING_TOOL_LIST_NOT, kNot );
}

#endif SERV_HACKING_TOOL_LIST
//}}

//{{ 2013.03.19 ��ȿ�� �ؿ� ���� ���� �ý��� ���� (�ؿܴ� ���� ��Ŷ ó�� ������)
#ifdef SERV_GLOBAL_AUTH
IMPL_ON_FUNC( ECH_VERIFY_ACCOUNT_REQ )
{
    VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

    START_LOG( clog, L"���� ��û." );

    KECH_VERIFY_ACCOUNT_ACK kPacket;

    //{{ 2009. 6. 10  ������	����IP����
    if( GetKChannelSimLayer()->CheckIP( GetIPStr() ) == false )
    {
		if( GetKChannelSimLayer()->GetCheckIPMode() == KChannelSimLayer::CIM_SERVICE_MODE )
		{
			START_LOG( cout, L"[���� ���] ���� ���� ��� IP�� ������ ���ӽõ��� �Ͽ����ϴ� : " << GetIPStr() << L" ,  UserID : " << kPacket_.m_wstrID );
		}
		else
		{
            START_LOG( cout, L"[���� ���] ���� �߿� �Ϲ� ���� ���� : " << GetIPStr() << L" ,  UserID : " << kPacket_.m_wstrID );
		}

        kPacket.m_iOK = NetError::ERR_CONNECT_05;
        SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );

		// ���� ����
		ReserveDestroy();
		return;
	}
	//}}

	//{{ 2011. 1. 24 ��ȿ��		*�߿�* ������ ���� �� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrID ) )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
		return;
	}
	//}}

#ifdef SERV_USE_KOG_ACCOUNTDB_PASSWORD
	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrPassword ) )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
		return;
	}
#endif //SERV_USE_KOG_ACCOUNTDB_PASSWORD
#ifndef SERV_SERVER_TIME_GET
#ifdef SERV_MASSFILE_MAPPING_FUNCTION
	CTime tCurrent = CTime::GetCurrentTime();
	kPacket.m_wstrCurrentTime = ( CStringW )tCurrent.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
#endif SERV_MASSFILE_MAPPING_FUNCTION
#endif SERV_SERVER_TIME_GET

	DWORD dwAuthFlag = KSimLayer::GetKObj()->GetAuthFlag();
	if(KSimLayer::AF_INTERNAL == dwAuthFlag)
	{
		if( KSimLayer::GetKObj()->GetUseKogOTP() )
		{
			m_wstrID = kPacket_.m_wstrID;

			if (!kPacket_.m_bIDVerified)	// �ۺ��� ���� ���� �������� üũ
			{
				// ������ �ۺ����� ��û�ؾ����� �系 �׽�Ʈ������ ��ȸ��
				// �׷����� KOG OTP ��� �� OTP ���� ������ ���� 
				// Publisher ���� ����� �޾��� �� ��û�ϴ� DBE_CH_USER_GENERAL_LOGIN_REQ ��û�ؾ� ��

				KDBE_CH_USER_GENERAL_LOGIN_REQ kPacketReq;
				kPacketReq.m_wstrServiceAccountID = kPacket_.m_wstrID;
				kPacketReq.m_wstrIP = KncUtil::toWideString(KSession::GetIPStr());
				kPacketReq.m_iChannelingCode = kPacket_.m_iChannelingCode;
#ifdef SERV_PURCHASE_TOKEN
				kPacketReq.m_wstrPurchaseTok = L"";
#endif SERV_PURCHASE_TOKEN
				kPacketReq.m_bServerUseKogOTP = KSimLayer::GetKObj()->GetUseKogOTP();

#ifdef SERV_USE_KOG_ACCOUNTDB_PASSWORD
				kPacketReq.m_wstrPassword = kPacket_.m_wstrPassword;
#endif //SERV_USE_KOG_ACCOUNTDB_PASSWORD

				SendToAccountDB( DBE_CH_USER_GENERAL_LOGIN_REQ, kPacketReq );
			}
			else
			{
				// �ۺ����� ��������� KOG OTP ��û
				RequestKOGOTPLogin(kPacket_);
			}
		}
		else
		{
			StateTransition( KCHFSM::I_TO_AUTHENTICATED );
			kPacket.m_iOK = NetError::NET_OK;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			
		}

		return;
	}

	if( KSimLayer::GetKObj()->GetUseKogOTP() )
	{
		// - KOG OTP ��� ������ ���
		// KOG OTP ��� ������ ��� �ۺ��� ���� -> KOG OTP ���� 2���� �������� ��������. 

		if (!kPacket_.m_bIDVerified)
		{
			m_wstrID = kPacket_.m_wstrID;	// �ٸ� ���� ���μ��� ���� ���� ��� �ӽ÷� ID �����ؾ� �Ǵ� ��찡 ����

			// �ۺ����� ��û
			RequestPublisherLogin(kPacket_);
		}
		else
		{
			// �ۺ����� ��������� KOG OTP ��û
			RequestKOGOTPLogin(kPacket_);
		}
	}
	else
	{
		m_wstrID = kPacket_.m_wstrID;	// �ٸ� ���� ���μ��� ���� ���� ��� �ӽ÷� ID �����ؾ� �Ǵ� ��찡 ����

		// - KOG OTP �̻�� ������ ���
		// �ۺ��� ������ ������ ��
		RequestPublisherLogin(kPacket_);
	}
}

#else // SERV_GLOBAL_AUTH
// ���� ���� ó�� �κ�
IMPL_ON_FUNC( ECH_VERIFY_ACCOUNT_REQ )
{
    VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

    START_LOG( clog, L"���� ��û." );

    KECH_VERIFY_ACCOUNT_ACK kPacket;

    //{{ 2009. 6. 10  ������	����IP����
    if( GetKChannelSimLayer()->CheckIP( GetIPStr() ) == false )
    {
#ifndef SERV_PRIVACY_AGREEMENT
		if( GetKChannelSimLayer()->GetCheckIPMode() == KChannelSimLayer::CIM_SERVICE_MODE )
		{		
			START_LOG( cout, L"[���� ���] ���� ���� ��� IP�� ������ ���ӽõ��� �Ͽ����ϴ� : " << GetIPStr() << L" ,  UserID : " << kPacket_.m_wstrID );
		}
		else
		{
            START_LOG( cout, L"[���� ���] ���� �߿� �Ϲ� ���� ���� : " << GetIPStr() << L" ,  UserID : " << kPacket_.m_wstrID );
		}
#endif SERV_PRIVACY_AGREEMENT	

        kPacket.m_iOK = NetError::ERR_CONNECT_05;
        SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );

        // ���� ����
        ReserveDestroy();
        return;
    }
    //}}
#ifndef SERV_SERVER_TIME_GET
#ifdef SERV_MASSFILE_MAPPING_FUNCTION
	CTime tCurrent = CTime::GetCurrentTime();
	kPacket.m_wstrCurrentTime = ( CStringW )tCurrent.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
#endif SERV_MASSFILE_MAPPING_FUNCTION
#endif SERV_SERVER_TIME_GET

    DWORD dwAuthFlag = KSimLayer::GetKObj()->GetAuthFlag();
    switch( dwAuthFlag )
    {
    case KSimLayer::AF_INTERNAL:
		{
			StateTransition( KCHFSM::I_TO_AUTHENTICATED );
	     
		//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
			if( kPacket_.m_bUseVerify == true )
			{
				SetUserID( kPacket_.m_wstrID );

				//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
				if( SetMachineID( kPacket_.m_strMachineID ) == true )
				{
					if( KSimLayer::GetKObj()->GetVersionFlag() != KSimLayer::VF_OPEN_TEST )		// ���� �׼������� �˻� ����
					{
						if( SiKGameSysVal()->IsMachineIDDuplicateCheck() == true )
						{
							// �������� MachineID �̹Ƿ� MachineID Block �˻縦 �Ѵ�.
							if( SiKMachineBlockManager()->IsBlockedMachineID( kPacket_.m_strMachineID ) == true )
							{
								// ���� Machine ID �Դϴ�.
								kPacket.m_iOK = NetError::ERR_VERIFY_17;
								SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );

								// ���� ����
								ReserveDestroy();
								return;
							}
						}
					}
				}
				else
				{
					// ���������� MachineID �̹Ƿ� ���� ó���Ѵ�.
					kPacket.m_iOK = NetError::ERR_VERIFY_17;
					SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
					// ���� ����
					ReserveDestroy();
					return;
				}
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
				//}}
				//{{ 2011.11.02     ��μ�    �������� SN �߱� �� ��ȿ�� �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
				if( SiKGameSysVal()->GetCheckServerSN() == true )
				{
					// Ŭ�� �������� sn �� �ִ�
					if( KServerSNMaker::IsEmptyServerSN( kPacket_.m_charServerSN ) == false )
					{
						// �˻��� ����?
						// ������ ��ȿ�� �˻�
						__int64 iServerSN = 0;
						if( KServerSNMaker::CheckServerSN( kPacket_.m_charServerSN, iServerSN ) == false )
						{
							START_LOG( cerr, L"��ȿ���� ���� ServerSN�� Ŭ�� ���!" )
								<< BUILD_LOG( kPacket_.m_wstrID )
								<< BUILD_LOG( iServerSN );

							// Ŭ�� ���Ͽ��� ���� 12����Ʈ ������ ���� �Ǿ���.
							//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
							KEGS_SERVER_CHECK_HACK_USER_REQ kHack;
							kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_CHECK_SUM_SEND_MAIL;
							SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kHack );
#else
							KEGS_SERVER_CHECK_HACK_USER_NOT kHack;
							kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_CHECK_SUM_SEND_MAIL;
							SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kHack );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
							//}}
							
							KDBE_CREATE_SERVER_SN_REQ kCreateSN;
							kCreateSN.m_strMachineID = kPacket_.m_strMachineID;
							SendToAccountDB( DBE_CREATE_SERVER_SN_REQ, kCreateSN );
							return;
						}

						// DB�� �����ϴ� ServerSN ���� Ȯ������
						KDBE_CHECK_SERVER_SN_REQ kCheckSN;
						kCheckSN.m_iServerSN = iServerSN;
						kCheckSN.m_strMachineID = kPacket_.m_strMachineID;
						kCheckSN.m_wstrID = kPacket_.m_wstrID;
						SendToAccountDB( DBE_CHECK_SERVER_SN_REQ, kCheckSN );

						START_LOG( clog, L"ServerSN ��ȣ Ȯ�ο�" )
							<< BUILD_LOG( kPacket_.m_wstrID )
							<< BUILD_LOG( iServerSN );

						return;
						
					}
					else // Ŭ�� �������� sn �� ����
					{
						// ���� sn�� ���� ����
						KDBE_CREATE_SERVER_SN_REQ kCreateSN;
						kCreateSN.m_strMachineID = kPacket_.m_strMachineID;
						SendToAccountDB( DBE_CREATE_SERVER_SN_REQ, kCreateSN );
						return;
					}
				}
				else
				{
					KECH_HACKING_TOOL_LIST_REQ kReq;
					SiKHackingProtectionManager()->GetHackingToolList( kReq.m_iRandomKey, kReq.m_vecHackingToolList );
					SendPacket( ECH_HACKING_TOOL_LIST_REQ, kReq );

					m_bVerifyAccount = true;
					m_tVerifyAccount = CTime::GetCurrentTime();

					START_LOG( clog, L"��ŷ�� ����Ʈ ���� �Ϸ�!!" )
						<< BUILD_LOG( kReq.m_iRandomKey )
						<< BUILD_LOG( kReq.m_vecHackingToolList.size() )
						<< END_LOG;
				}
#else
				KECH_HACKING_TOOL_LIST_REQ kReq;
				SiKHackingProtectionManager()->GetHackingToolList( kReq.m_iRandomKey, kReq.m_vecHackingToolList );
				SendPacket( ECH_HACKING_TOOL_LIST_REQ, kReq );

				m_bVerifyAccount = true;
				m_tVerifyAccount = CTime::GetCurrentTime();

				START_LOG( clog, L"��ŷ�� ����Ʈ ���� �Ϸ�!!" )
					<< BUILD_LOG( kReq.m_iRandomKey )
					<< BUILD_LOG( kReq.m_vecHackingToolList.size() )
					<< END_LOG;
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
				//}}
			}
			else
			{
				if( m_bVerifyAccount )
				{
					kPacket.m_iOK = NetError::NET_OK;
					SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
				}
				else
				{
					kPacket.m_iOK = NetError::NET_OK;
					SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
				}
			}
			return;
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
		//}}
			kPacket.m_iOK = NetError::NET_OK;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			break;
		}
    case KSimLayer::AF_NEXON_KOREA:
		{
			StateTransition( KCHFSM::I_TO_AUTHENTICATED );

		//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
			if( kPacket_.m_bUseVerify == true )
			{
				SetUserID( kPacket_.m_wstrID );

				//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
				if( SetMachineID( kPacket_.m_strMachineID ) == true )
				{
					if( KSimLayer::GetKObj()->GetVersionFlag() != KSimLayer::VF_OPEN_TEST )		// ���� �׼������� �˻� ����
					{
						if( SiKGameSysVal()->IsMachineIDDuplicateCheck() == true )
						{
							// �������� MachineID �̹Ƿ� Account Block �˻縦 �Ѵ�.
							if( SiKMachineBlockManager()->IsBlockedMachineID( kPacket_.m_strMachineID ) == true )
							{
								// ���� Machine ID �Դϴ�.
								kPacket.m_iOK = NetError::ERR_VERIFY_17;
								SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );

								// ���� ����
								ReserveDestroy();
								return;
							}
						}
					}
				}
				else
				{
					// ���������� MachineID �̹Ƿ� ���� ó���Ѵ�.
					kPacket.m_iOK = NetError::ERR_VERIFY_17;
					SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
					// ���� ����
					ReserveDestroy();
					return;
				}
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
				//}}
				//{{ 2011.11.02     ��μ�    �������� SN �߱� �� ��ȿ�� �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
				if( SiKGameSysVal()->GetCheckServerSN() == true )
				{
					// Ŭ�� �������� sn �� �ִ�
					if( KServerSNMaker::IsEmptyServerSN( kPacket_.m_charServerSN ) == false )
					{
						// �˻��� ����?
						// ������ ��ȿ�� �˻�
						__int64 iServerSN = 0;
						if( KServerSNMaker::CheckServerSN( kPacket_.m_charServerSN, iServerSN ) == false )
						{
							START_LOG( cerr, L"��ȿ���� ���� ServerSN�� Ŭ�� ���!" )
								<< BUILD_LOG( kPacket_.m_wstrID )
								<< BUILD_LOG( iServerSN );

							// Ŭ�� ���Ͽ��� ���� 12����Ʈ ������ ���� �Ǿ���.
							//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
							KEGS_SERVER_CHECK_HACK_USER_REQ kHack;
							kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_CHECK_SUM_SEND_MAIL;
							SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kHack );
#else
							KEGS_SERVER_CHECK_HACK_USER_NOT kHack;
							kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_CHECK_SUM_SEND_MAIL;
							SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kHack );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
							//}}
							
							KDBE_CREATE_SERVER_SN_REQ kCreateSN;
							kCreateSN.m_strMachineID = kPacket_.m_strMachineID;
							SendToAccountDB( DBE_CREATE_SERVER_SN_REQ, kCreateSN );
							return;
						}

						// DB�� �����ϴ� ServerSN ���� Ȯ������
						KDBE_CHECK_SERVER_SN_REQ kCheckSN;
						kCheckSN.m_iServerSN = iServerSN;
						kCheckSN.m_strMachineID = kPacket_.m_strMachineID;
						kCheckSN.m_wstrID = kPacket_.m_wstrID;
						SendToAccountDB( DBE_CHECK_SERVER_SN_REQ, kCheckSN );
						return;

					}
					else // Ŭ�� �������� sn �� ����
					{
						// ���� sn�� ���� ����
						KDBE_CREATE_SERVER_SN_REQ kCreateSN;
						kCreateSN.m_strMachineID = kPacket_.m_strMachineID;
						SendToAccountDB( DBE_CREATE_SERVER_SN_REQ, kCreateSN );
						return;
					}
				}
				else
				{
					KECH_HACKING_TOOL_LIST_REQ kReq;
					SiKHackingProtectionManager()->GetHackingToolList( kReq.m_iRandomKey, kReq.m_vecHackingToolList );
					SendPacket( ECH_HACKING_TOOL_LIST_REQ, kReq );

					m_bVerifyAccount = true;
					m_tVerifyAccount = CTime::GetCurrentTime();

					START_LOG( clog, L"��ŷ�� ����Ʈ ���� �Ϸ�!!" )
						<< BUILD_LOG( kReq.m_iRandomKey )
						<< BUILD_LOG( kReq.m_vecHackingToolList.size() )
						<< END_LOG;
				}
#else
				KECH_HACKING_TOOL_LIST_REQ kReq;
				SiKHackingProtectionManager()->GetHackingToolList( kReq.m_iRandomKey, kReq.m_vecHackingToolList );
				SendPacket( ECH_HACKING_TOOL_LIST_REQ, kReq );

				m_bVerifyAccount = true;
				m_tVerifyAccount = CTime::GetCurrentTime();

				START_LOG( clog, L"��ŷ�� ����Ʈ ���� �Ϸ�!!" )
					<< BUILD_LOG( kReq.m_iRandomKey )
					<< BUILD_LOG( kReq.m_vecHackingToolList.size() )
					<< END_LOG;
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
				//}}
			}
			else
			{
				kPacket.m_iOK = NetError::NET_OK;
				SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			}
			return;
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
		//}}
			kPacket.m_iOK = NetError::NET_OK;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			break;
		}
    //case KSimLayer::AF_GAMANIA_TAIWAN:
    //    if( kPacket_.m_bLogin )
    //    {
    //        // GASH ����
    //        KEGASH_AUTHORIZATION_REQ kPacketReq;
    //        kPacketReq.m_iActorUID = GetUID();
    //        kPacketReq.m_strServiceCode = SiKGASHAuthManager()->GetServiceCode();
    //        kPacketReq.m_strServiceRegion = SiKGASHAuthManager()->GetServiceRegion();
    //        kPacketReq.m_strServiceAccountID = KncUtil::toNarrowString( kPacket_.m_wstrID );
    //        kPacketReq.m_strServicePassword = KncUtil::toNarrowString( kPacket_.m_wstrPassword );
    //        kPacketReq.m_strAuthenticationFlag = "1";
    //        kPacketReq.m_strType = "I";
    //        kPacketReq.m_strDuplicateCheck = "0";
    //        kPacketReq.m_strIPAddress = KSession::GetIPStr();
    //        kPacketReq.m_strPacketID = KncUtil::toNarrowString( kPacket_.m_wstrID );

    //        KEventPtr spEvent( new KEvent );
    //        spEvent->SetData(PI_NULL, NULL, EGASH_AUTHORIZATION_REQ, kPacketReq );
    //        SiKGASHAuthManager()->QueueingEvent( spEvent );
    //    }
    //    else
    //    {
    //        // ��ü passport ����
    //        KDBE_GASH_USER_LOGIN_REQ kPacketReq;
    //        kPacketReq.m_wstrServiceAccountID = kPacket_.m_wstrID;
    //        kPacketReq.m_wstrOTP = kPacket_.m_wstrPassword;

    //        SendToAccountDB( DBE_GASH_USER_LOGIN_REQ, kPacketReq );
    //    }
    //    break;
    default:
		{
			START_LOG( cerr, L"���� �÷��� �̻�." )
				<< BUILD_LOG( dwAuthFlag )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_VERIFY_17;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			break;
		}
    }
}
#endif // SERV_GLOBAL_AUTH
//}}

#ifndef SERV_GLOBAL_AUTH 
IMPL_ON_FUNC( ECH_VERIFY_ACCOUNT_ACK )
{
    VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

    SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket_ );

    if( kPacket_.m_iOK == NetError::NET_OK )
    {
        StateTransition( KCHFSM::I_TO_AUTHENTICATED );
    }
    else
    {
        ReserveDestroy();
    }
}
#endif // SERV_GLOBAL_AUTH

/*
IMPL_ON_FUNC( DBE_GASH_USER_LOGIN_ACK )
{
    VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

    KECH_VERIFY_ACCOUNT_ACK kPacket;
    kPacket.m_iOK = kPacket_.m_iOK;
    kPacket.m_iUserUID = kPacket_.m_iUserUID;
    kPacket.m_wstrPassport = kPacket_.m_wstrOTP;

    SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );

    if( kPacket_.m_iOK == NetError::NET_OK )
    {
        StateTransition( KCHFSM::I_TO_AUTHENTICATED );
    }
    else
    {
        ReserveDestroy();
    }

}
//*/

//{{ 2012. 07. 02	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC_NOPARAM( ECH_GET_CHANNEL_LIST_REQ )
{
	switch( GetStateID() )
	{
	case KCHFSM::S_LOGINED:
		break;

	case KCHFSM::S_WAIT_QUEUE:
		{
			KECH_GET_CHANNEL_LIST_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::NET_OK;
			SendPacket( ECH_GET_CHANNEL_LIST_ACK, kPacketAck );
		}
		break;

	default:
		{
			START_LOG( cerr, L"���� ����. Name : " << m_strName )
				<< L"    ���� ���� : " << GetStateIDString() << dbg::endl
				<< L"    ��� ���� : KCHFSM::S_LOGINED" << END_LOG;
			return;
		}
		break;
	}

	KECH_GET_CHANNEL_LIST_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::NET_OK;
	SendPacket( ECH_GET_CHANNEL_LIST_ACK, kPacketAck );

	// ���� ��忡 ���� ����
	switch( SiKWaitQueueManager()->GetMode() )
	{
	case KWaitQueueManager::WQ_NONE:
		{
			if( m_bPassedEnterChannel == false )
			{
				// �׳� �ٷ� ���!
				m_bPassedEnterChannel = true;
				SiKWaitQueueManager()->RegPassedUser( GetUID() );
			}
		}
		break;

	case KWaitQueueManager::WQ_CCU_LIMIT:
		{
			// ä�� ����Ʈ�� ������ �ֵ��� ������ �޾Ҵ°�?
			if( m_bPassedEnterChannel == false )
			{
				// �ϴ� ���� ������ ��ŭ���� ä���� �����Ҹ�ŭ �ο��� ���ִ��� ����!	
				m_bPassedEnterChannel = SiKWaitQueueManager()->IsCanEnterChannel();
				if( m_bPassedEnterChannel == false )
				{
					// �ش� ������ ��⿭�� ����Ѵ�!
					LIF( SiKWaitQueueManager()->RegWaitUser( GetUID() ) );

					// �ش� �������� �˾��� ���� ���� ��Ŷ ����
					KECH_WAIT_QUEUE_MESSAGE_NOT kPacketNot;
					LIF( SiKWaitQueueManager()->GetWaitUserCount( GetUID(), kPacketNot.m_iWaitUserCount ) );
					SendPacket( ECH_WAIT_QUEUE_MESSAGE_NOT, kPacketNot );

					// ���� �̵�
					StateTransition( KCHFSM::I_TO_WAIT_QUEUE );
					return;
				}
				else
				{
					// ����� ���� ���!
					SiKWaitQueueManager()->RegPassedUser( GetUID() );
				}
			}
		}
		break;

	case KWaitQueueManager::WQ_ENTRY_BAN:
		{
			// �ش� ������ ��⿭�� ����Ѵ�!
			LIF( SiKWaitQueueManager()->RegWaitUser( GetUID() ) );

			// �ش� �������� �˾��� ���� ���� ��Ŷ ����
			KECH_WAIT_QUEUE_MESSAGE_NOT kPacketNot;
			LIF( SiKWaitQueueManager()->GetWaitUserCount( GetUID(), kPacketNot.m_iWaitUserCount ) );
			SendPacket( ECH_WAIT_QUEUE_MESSAGE_NOT, kPacketNot );

			// ���� �̵�
			StateTransition( KCHFSM::I_TO_WAIT_QUEUE );
			return;
		}
		break;

	default:
		{
			START_LOG( cerr, L"���� ���� �ʴ� ����Դϴ�!" )
				<< BUILD_LOG( SiKWaitQueueManager()->GetMode() )
				<< END_LOG;
		}
		return;
	}
	
	// ä�� ����Ʈ ����
	SendChannelList();
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC_NOPARAM( ECH_GET_CHANNEL_LIST_REQ )
{
	VERIFY_STATE( ( 1, KCHFSM::S_LOGINED ) );

	KECH_GET_CHANNEL_LIST_ACK kPacketAck;	
	kPacketAck.m_iOK = NetError::NET_OK;
	SendPacket( ECH_GET_CHANNEL_LIST_ACK, kPacketAck );

	KECH_GET_CHANNEL_LIST_NOT kPacketNot;
	kPacketNot.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
	kPacketNot.m_mapChannelList = GetKChannelServer()->GetChannelList();
	//{{ 2012. 07. 2	�ڼ���	ä�� UI ���������� ���� ������ �ý��� ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
	kPacketNot.m_mapChannelBonusList = GetKChannelServer()->GetChannelBonusList();
#endif SERV_CHANNEL_LIST_RENEWAL
	//}}
	SendPacket( ECH_GET_CHANNEL_LIST_NOT, kPacketNot );
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_WAIT_QUEUE_SYSTEM
//}}

IMPL_ON_FUNC_NOPARAM( ECH_DISCONNECT_REQ )
{
	//{{ 2012. 07. 02	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
	VERIFY_STATE( ( 3, KCHFSM::S_INIT, KCHFSM::S_LOGINED, KCHFSM::S_WAIT_QUEUE ) );
#else
	VERIFY_STATE( ( 2, KCHFSM::S_INIT, KCHFSM::S_LOGINED ) );
#endif SERV_WAIT_QUEUE_SYSTEM
	//}}

	//{{ 2012. 07. 02	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
	if( GetStateID() == KCHFSM::S_WAIT_QUEUE )
	{
		if( SiKWaitQueueManager()->IsExist( GetUID() ) == true )
		{
			LIF( SiKWaitQueueManager()->UnRegWaitUser( GetUID() ) );
		}
	}
#endif SERV_WAIT_QUEUE_SYSTEM
	//}}

	SendID( ECH_DISCONNECT_ACK );

	ReserveDestroy();
}

//{{ 2009. 12. 16  ������	������
IMPL_ON_FUNC( E_TOOL_GET_CCU_INFO_REQ )
{
	VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

	// ĳ�̵����� Ȯ��
	KE_TOOL_GET_CCU_INFO_ACK kPacketAck;
	if( SiKCCUToolManager()->GetExistCCU( kPacket_, kPacketAck ) == true )	// ��û�� ��¥�� CCU ����Ÿ�� �ִ�
	{
		kPacketAck.m_iOK = NetError::NET_OK;
		SendPacket( E_TOOL_GET_CCU_INFO_ACK, kPacketAck );
		return;
	}

	KDBE_GET_CONCURRENT_USER_INFO_REQ kPacketToDB;
	kPacketToDB.m_cCCUType = kPacket_.m_cCCUType;
	kPacketToDB.m_iServerUID = kPacket_.m_iServerUID;
	kPacketToDB.m_wstrDate = kPacket_.m_wstrDate;
	SendToLogDB( DBE_GET_CONCURRENT_USER_INFO_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_GET_CONCURRENT_USER_INFO_ACK )
{
	VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

	KE_TOOL_GET_CCU_INFO_ACK kAck;
	kAck.m_iOK = kPacket_.m_iOK;
	kAck.m_cCCUType = kPacket_.m_cCCUType;
	kAck.m_iServerUID = kPacket_.m_iServerUID;
	kAck.m_wstrDate = kPacket_.m_wstrDate;
	kAck.m_vecCCUInfo = kPacket_.m_vecCCUInfo;

	CTime tCCUDate;

	if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrDate, tCCUDate ) == false )
	{
		START_LOG( cerr, L"��¥ ���� ����!" )
			<< BUILD_LOG( kPacket_.m_iServerUID )
			<< BUILD_LOG( kPacket_.m_cCCUType )
			<< BUILD_LOG( kPacket_.m_wstrDate )
			<< END_LOG;
		return;
	}

	const KCCUToolManager::CCUDateData* mapCCUData = SiKCCUToolManager()->GetCCUData( tCCUDate );
	if( mapCCUData == NULL ) 
	{
		if( SiKCCUToolManager()->NewCCUData( kPacket_.m_iServerUID, tCCUDate.GetTime(), kPacket_.m_vecCCUInfo ) == false )
		{
			//[������] �α� ����
			START_LOG( cerr, L"���� ���� �߰� ����! ���� ���� �ִµ�?" )
				<< BUILD_LOG( kPacket_.m_iServerUID )
				<< BUILD_LOG( kPacket_.m_cCCUType )
				<< BUILD_LOG( kPacket_.m_wstrDate )
				<< END_LOG;

			return;
		}
	}
	else
	{
		if( SiKCCUToolManager()->SetCCUData( tCCUDate.GetTime(), kPacket_.m_iServerUID, kPacket_.m_vecCCUInfo, kPacket_.m_cCCUType ) == false ) 
		{
			return;
		}
	}

    SendPacket( E_TOOL_GET_CCU_INFO_ACK, kAck );
}
//}}

IMPL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY )
{
	m_bDestroyReserved = true;
}



//{{ 2011. 01. 20 ��μ�	���� ����͸� ��
#ifdef SERV_CCU_MONITORING_TOOL
IMPL_ON_FUNC( E_TOOL_CHECK_LOGIN_REQ )
{
	// ID, PW �� �� �ϳ��� Ư�� ���ڰ� ���� �Ǿ� �ִٸ� 
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrID ) || KODBC::IsInvalidMarkIn( kPacket_.m_wstrPW ) )
	{
		START_LOG( cout, L"Ư�� ���� ����� ID, PW �Է����� ���� ����!")
			<< BUILD_LOG( kPacket_.m_wstrID )
			<< BUILD_LOG( kPacket_.m_wstrPW )
			<< END_LOG;

		ReserveDestroy();
		return;
	}

	KDBE_GET_CONCURRENT_USER_LOGIN_REQ kPacketReq;

	kPacketReq.m_wstrID = kPacket_.m_wstrID;
	kPacketReq.m_wstrPW	= kPacket_.m_wstrPW;

 	SendToAccountDB(DBE_GET_CONCURRENT_USER_LOGIN_REQ, kPacketReq );
}

IMPL_ON_FUNC( DBE_GET_CONCURRENT_USER_LOGIN_ACK )
{
	KE_TOOL_CHECK_LOGIN_ACK Login_ACK;

	if( kPacket_.m_iAuthLevel < 0 )
	{
		START_LOG( cout, L"�α��� ���з� ���� ���� ����!")
		<< BUILD_LOG( kPacket_.m_iAuthLevel )
			<< END_LOG;
		ReserveDestroy();
		return;
	}

	Login_ACK.m_iAuthLevel = kPacket_.m_iAuthLevel;
	
	SendPacket( E_TOOL_CHECK_LOGIN_ACK, Login_ACK );	
}

IMPL_ON_FUNC_NOPARAM( E_TOOL_SERVER_LIST_REQ )
{
	SendToLogDB( DBE_GET_GAME_SERVER_LIST_REQ );
}

IMPL_ON_FUNC( DBE_GET_GAME_SERVER_LIST_ACK )
{
	SendPacket( E_TOOL_SERVER_LIST_ACK, kPacket_ );
}

#endif SERV_CCU_MONITORING_TOOL
//}}

//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
IMPL_ON_FUNC( ECH_HACKING_TOOL_LIST_ACK )
{
	KDBE_UPDATE_CHANNEL_RANDOMKEY_REQ kReq;
	m_bVerifyAccount = false;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// ���� ������ ����Ű �߱�
		kReq.m_wstrUserID = GetUserID();

		// ü�� ���̵��� ��� �˻����� �����Ƿ� DB ��� ���� �ʴ´�.
		if( kReq.m_wstrUserID.empty() == true )  // ü�� ���̵�� �Ǵ��Ѵ�.  
		{										 // ä�� ���������� UserID�� Ŭ���̾�Ʈ���� �޴µ� Ŭ���̾�Ʈ�� ü�� ���̵� �϶� ���� ���Ѵ�.
			KECH_VERIFY_ACCOUNT_ACK kPacket;
			DWORD dwAuthFlag = KSimLayer::GetKObj()->GetAuthFlag();
			kPacket.m_iOK = kPacket_.m_iOK;
			kPacket.m_iChannelRandomKey = -58566;
			kPacket.m_bUseVerify = true;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			return;
		}

		SendToAccountDB( DBE_UPDATE_CHANNEL_RANDOMKEY_REQ, kReq );
	}
}

IMPL_ON_FUNC( DBE_UPDATE_CHANNEL_RANDOMKEY_ACK )
{
	KECH_VERIFY_ACCOUNT_ACK kPacket;
	DWORD dwAuthFlag = KSimLayer::GetKObj()->GetAuthFlag();

	switch( dwAuthFlag )
	{
	case KSimLayer::AF_INTERNAL:
	case KSimLayer::AF_NEXON_KOREA:
		{
			kPacket.m_iOK = kPacket_.m_iOK;
			kPacket.m_iChannelRandomKey = kPacket_.m_iRandomKey;
			kPacket.m_bUseVerify = true;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			break;
		}
	default:
		{
			START_LOG( cerr, L"���� �÷��� �̻�." )
				<< BUILD_LOG( dwAuthFlag )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_VERIFY_17;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			break;
		}
	}
}

void KChannelUser::CheckVerifyAccountRecv()
{
	if( m_bVerifyAccount == true )
	{
		CTime tNow = CTime::GetCurrentTime();
		if( m_tVerifyAccount + 10 < tNow )
		{
			KECH_VERIFY_ACCOUNT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );

			m_bVerifyAccount = false;

			// ���� ����
			ReserveDestroy();
		}
	}
}

void KChannelUser::SetUserID( IN std::wstring& wstrUserID )
{
	// ������ ���� �ȵ����� ���ڿ��̹Ƿ� injection ��� �˻縦 �Ѵ�.
	if( KODBC::IsInvalidMarkIn( wstrUserID ) )
	{
		START_LOG( cerr, L"ChannelUserID �� ���������� ���� ���ڰ� �ִ�." )
			<< BUILD_LOG( wstrUserID );
		return;
	}

	m_wstrUserID = wstrUserID;
}
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
//}}

//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
bool KChannelUser::SetMachineID( IN const std::string& strMachineID )
{
	if( m_kUserSecurityManager.CheckValidMachineID( strMachineID ) == true )
	{
		m_kUserSecurityManager.SetMachineID( strMachineID );
		return true;
	}

	return false;
}

std::string KChannelUser::GetMachineID()
{
	return m_kUserSecurityManager.GetStringMachineID();
}
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
//}}

//{{ 2012. 07. 02	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
void KChannelUser::SendChannelList()
{
	KECH_GET_CHANNEL_LIST_NOT kPacketNot;
	kPacketNot.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
	kPacketNot.m_mapChannelList = GetKChannelServer()->GetChannelList();
	//{{ 2012. 07. 2	�ڼ���	ä�� UI ���������� ���� ������ �ý��� ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
	kPacketNot.m_mapChannelBonusList = GetKChannelServer()->GetChannelBonusList();
#endif SERV_CHANNEL_LIST_RENEWAL
	//}}
	SendPacket( ECH_GET_CHANNEL_LIST_NOT, kPacketNot );
}
#endif SERV_WAIT_QUEUE_SYSTEM
//}}

//{{ 2011.11.02     ��μ�    �������� SN �߱� �� ��ȿ�� �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
IMPL_ON_FUNC( DBE_CHECK_SERVER_SN_ACK )
{
	// DB�� �����ϴ� SN�̴�.
	switch( kPacket_.m_iOK )
	{
	case NetError::NET_OK:
		{
			
		}break;
	case NetError::ERR_SERVERSN_01:
		{
			// ��߱�
			KECH_NEW_SERVER_SN_NOT kNot;
			if( kPacket_.m_iServerSN > 0 && KServerSNMaker::MakeServerSN( kPacket_.m_iServerSN, kNot.m_charServerSN ) == true )
			{
				SendPacket( ECH_NEW_SERVER_SN_NOT, kNot );
			}
			
			// ����
			//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			KEGS_SERVER_CHECK_HACK_USER_REQ kHack;
			kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_SERVER_SN_SEND_MAIL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kHack );
#else
			KEGS_SERVER_CHECK_HACK_USER_NOT kHack;
			kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_SERVER_SN_SEND_MAIL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kHack );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			//}}
		}break;
	case NetError::ERR_SERVERSN_02:
		{
			// ����
			//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			KEGS_SERVER_CHECK_HACK_USER_REQ kHack;
			kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_SN_AND_MID_SEND_MAIL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kHack );
#else
			KEGS_SERVER_CHECK_HACK_USER_NOT kHack;
			kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_SN_AND_MID_SEND_MAIL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kHack );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			//}}
			
			// ����
			KDBE_CREATE_SERVER_SN_REQ kCreateSN;
			kCreateSN.m_strMachineID = GetMachineID();;
			SendToAccountDB( DBE_CREATE_SERVER_SN_REQ, kCreateSN );
			return;
		}break;
	case NetError::ERR_SERVERSN_03:
		{
			// ����
			//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			KEGS_SERVER_CHECK_HACK_USER_REQ kHack;
			kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_MACHINE_ID_SEND_MAIL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kHack );
#else
			KEGS_SERVER_CHECK_HACK_USER_NOT kHack;
			kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_MACHINE_ID_SEND_MAIL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kHack );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			//}}
		}break;
	case NetError::ERR_SERVERSN_07:   // ����� sn
		{
			// ����
			KECH_VERIFY_ACCOUNT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_VERIFY_17;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			// ���� ����
			ReserveDestroy();
		}break;
	default:
		{
			// ������ ������ �����ΰ�?
			START_LOG( cerr, L"SN & MID üũ���� ������ ������ �����ΰ�?" )
				<< BUILD_LOG( kPacket_.m_iOK );

			KECH_VERIFY_ACCOUNT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_VERIFY_17;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			// ���� ����
			ReserveDestroy();
		}break;
	}

	KECH_HACKING_TOOL_LIST_REQ kReq;
	SiKHackingProtectionManager()->GetHackingToolList( kReq.m_iRandomKey, kReq.m_vecHackingToolList );
	SendPacket( ECH_HACKING_TOOL_LIST_REQ, kReq );

	m_bVerifyAccount = true;
	m_tVerifyAccount = CTime::GetCurrentTime();
}

IMPL_ON_FUNC( DBE_CREATE_SERVER_SN_ACK )
{
	// DB���� �����Ͽ���.
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// send ECH_NEW_SERVER_SN_NOT
		KECH_NEW_SERVER_SN_NOT kNot;
		if( KServerSNMaker::MakeServerSN( kPacket_.m_iServerSN, kNot.m_charServerSN ) == true )
		{
			SendPacket( ECH_NEW_SERVER_SN_NOT, kNot );
		}
		else
		{
			KECH_VERIFY_ACCOUNT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_VERIFY_17;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			// ���� ����
			ReserveDestroy();
			return;
		}

		// send ECH_HACKING_TOOL_LIST_REQ
		KECH_HACKING_TOOL_LIST_REQ kReq;
		SiKHackingProtectionManager()->GetHackingToolList( kReq.m_iRandomKey, kReq.m_vecHackingToolList );
		SendPacket( ECH_HACKING_TOOL_LIST_REQ, kReq );

		m_bVerifyAccount = true;
		m_tVerifyAccount = CTime::GetCurrentTime();
	}
	else if( kPacket_.m_iOK == NetError::ERR_SERVERSN_05 )
	{
		// send ECH_NEW_SERVER_SN_NOT
		KECH_NEW_SERVER_SN_NOT kNot;
		if( KServerSNMaker::MakeServerSN( kPacket_.m_iServerSN, kNot.m_charServerSN ) == true )
		{
			SendPacket( ECH_NEW_SERVER_SN_NOT, kNot );
		}
		else
		{
			KECH_VERIFY_ACCOUNT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_VERIFY_17;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			// ���� ����
			ReserveDestroy();
			return;
		}

		// send ECH_HACKING_TOOL_LIST_REQ
		KECH_HACKING_TOOL_LIST_REQ kReq;
		SiKHackingProtectionManager()->GetHackingToolList( kReq.m_iRandomKey, kReq.m_vecHackingToolList );
		SendPacket( ECH_HACKING_TOOL_LIST_REQ, kReq );

		m_bVerifyAccount = true;
		m_tVerifyAccount = CTime::GetCurrentTime();

		// ���� ��� �ߵ�
		//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		KEGS_SERVER_CHECK_HACK_USER_REQ kHack;
		kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_SERVER_SN_SEND_MAIL;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kHack );
#else
		KEGS_SERVER_CHECK_HACK_USER_NOT kHack;
		kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_SERVER_SN_SEND_MAIL;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kHack );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//}}
	}
	else
	{
		// DB���� �������� ���ߴ�.
		KECH_VERIFY_ACCOUNT_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_17;
		SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
		// ���� ����
		ReserveDestroy();
	}
}
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
//}}

#ifdef SERV_GLOBAL_AUTH
IMPL_ON_FUNC( EPUBLISHER_AUTHENTICATION_ACK )
{
    VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

	// ������ Ư�� ó�� �߰� (������ �ݿ����� ó��)//
	bool bDoNotDisconnect = false;

#ifdef SERV_LOGIN_RESULT_INFO
	ProcessLoginResultCount( kPacket_.m_iOK );			// �α��� ���� ��� ó��( �ۺ��� ������ ��ġ�� ������ ���⿡�� ó�� )
#endif SERV_LOGIN_RESULT_INFO

	HandlePublisherLoginAck(kPacket_, bDoNotDisconnect);
		
	if (kPacket_.m_iOK == NetError::NET_OK)				// �ۺ��� ���� ����
	{
		KDBE_CH_USER_GENERAL_LOGIN_REQ kPacketReq;
		kPacketReq.m_wstrServiceAccountID = kPacket_.m_wstrID;
		kPacketReq.m_uiPublisherUID = kPacket_.m_uiPublisherUID;
		kPacketReq.m_wstrIP = KncUtil::toWideString(KSession::GetIPStr());
		kPacketReq.m_iChannelingCode = kPacket_.m_iChannelingCode;
#ifdef SERV_ANTI_ADDICTION_SYSTEM
		kPacketReq.m_uiAccountType = kPacket_.m_uiAccountType;
#endif SERV_ANTI_ADDICTION_SYSTEM

#ifdef SERV_PURCHASE_TOKEN
		kPacketReq.m_wstrPurchaseTok = kPacket_.wstrPurchTok;
#endif SERV_PURCHASE_TOKEN
		kPacketReq.m_bServerUseKogOTP = KSimLayer::GetKObj()->GetUseKogOTP();
#ifdef SERV_COUNTRY_TH
		SetMasterID( kPacket_.m_wstrMasterID );
		SetSocketID( kPacket_.m_wstrSocketID );
		kPacketReq.m_wstrMasterID = kPacket_.m_wstrMasterID;
		kPacketReq.m_wstrSocketID = kPacket_.m_wstrSocketID;
#endif // SERV_COUNTRY_TH

		SendToAccountDB( DBE_CH_USER_GENERAL_LOGIN_REQ, kPacketReq );
	}
	else												// �ۺ��� ���� ����
	{
		KECH_VERIFY_ACCOUNT_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		kPacketAck.m_wstrUserID = L"";
		//	kPacketAck.m_iUserUID = kPacket_.m_iUserUID;
		kPacketAck.m_wstrPassport = L"";
#ifdef SERV_PURCHASE_TOKEN
		kPacketAck.m_wstrPurchaseTok = L"";
#endif SERV_PURCHASE_TOKEN

#ifdef SERV_LOGIN_TOU // ��� �̵��Ƿ� �α��� ���� �ÿ� �����
		kPacketAck.m_strAgreementURL = kPacket_.m_strAgreementURL;
#endif //SERV_LOGIN_TOU

		// Client ACK ���� //
		SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacketAck );
		
		if (bDoNotDisconnect)
		{
			// �н����� �߸� �Է����� ������ ���� �ʴ´�.
			START_LOG( clog2, L"���̵� �Ǵ� �н����� �ٸ� �Ǵ� GF ���� ���� �ȵ� ����. (���� �� ����)" );
		}
		else
		{
			ReserveDestroy();
		}
	}	
}

IMPL_ON_FUNC( DBE_CH_USER_GENERAL_LOGIN_ACK )
{
	VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );
	
	KECH_VERIFY_ACCOUNT_ACK kPacketAck;

	kPacketAck.m_iOK = kPacket_.m_iOK;
	kPacketAck.m_wstrUserID = kPacket_.m_wstrUserID;
	kPacketAck.m_iUserUID = kPacket_.m_iUserUID;
	kPacketAck.m_wstrPassport = kPacket_.m_wstrOTP;
	kPacketAck.m_iChannelingCode = kPacket_.m_iChannelingCode;
#ifdef SERV_PURCHASE_TOKEN
	kPacketAck.m_wstrPurchaseTok = kPacket_.m_wstrPurchaseTok;
#endif SERV_PURCHASE_TOKEN
#ifndef SERV_SERVER_TIME_GET
#ifdef SERV_MASSFILE_MAPPING_FUNCTION
	CTime tCurrent = CTime::GetCurrentTime();
	kPacketAck.m_wstrCurrentTime = ( CStringW )tCurrent.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
#endif SERV_MASSFILE_MAPPING_FUNCTION
#endif SERV_SERVER_TIME_GET

#ifdef SERV_COUNTRY_TH
	kPacketAck.m_wstrSocketID = GetSocketID();
#endif //SERV_COUNTRY_TH
	
	START_LOG( clog, L"[�׽�Ʈ �α�] Send Packet Token ���� Ȯ��" )
		<< BUILD_LOG( kPacketAck.m_iOK )
#ifndef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( kPacketAck.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( kPacketAck.m_iUserUID )
		<< BUILD_LOG( kPacketAck.m_wstrPassport )
		<< BUILD_LOG( kPacketAck.m_wstrPurchaseTok )
		<< END_LOG;

	SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacketAck );

#ifdef SERV_LOGIN_RESULT_INFO
	// �系 ���� �Ǵ� �ۺ��� ���� �����κ��� �������� �ʰ� ��ü �����ϴ� ���(�ε��׽þ�, �ʸ���)�� ���⼭ �α��� ī��Ʈ �ø�
	// �ε��׽þ�, �ʸ����� EPUBLISHER_AUTHENTICATION_ACK �̺�Ʈ�� ��ġ�� �ʱ⿡ ���⼭ �α��� ī��Ʈ �ø�
	DWORD dwAuthFlag = KSimLayer::GetKObj()->GetAuthFlag();

	if( KSimLayer::AF_INTERNAL == dwAuthFlag )							// �系 �α��� ���� ��� ó��
	{
		ProcessLoginResultCount( kPacket_.m_iOK );
	}
#if defined( SERV_COUNTRY_ID ) || defined( SERV_COUNTRY_PH )
	else																// �ε��׽þ�, �ʸ��� �α��� ���� ��� ó��
	{
		ProcessLoginResultCount( kPacket_.m_iOK );
	}
#endif // defined( SERV_COUNTRY_ID ) || defined( SERV_COUNTRY_PH )
#endif SERV_LOGIN_RESULT_INFO

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		StateTransition( KCHFSM::I_TO_AUTHENTICATED );
		
		//{{ 2011.03.16  �ӱԼ� IP ��� �ŷ� ����
#ifdef SERV_IP_TRADE_BLOCK
		KDBE_IP_CHECK_LIST_NOT kPacketNot;
		kPacketNot.m_wstrID = kPacket_.m_wstrUserID;
		//kPacketNot.iUserUID = kPacket_.m_iUserUID;
		kPacketNot.strIP = GetIPStr();

		START_LOG( clog, L"[�׽�Ʈ �α�] ���� UID" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUID() )
#else
			<< BUILD_LOG( kPacketNot.m_wstrID )
			<< BUILD_LOG( GetIPStr() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		SendToAccountDB(DBE_IP_CHECK_LIST_NOT, kPacketNot);
#endif SERV_IP_TRADE_BLOCK
		//}}
	}
	else
	{
		ReserveDestroy();
	}
}

IMPL_ON_FUNC( DBE_CH_USER_KOGOTP_LOGIN_ACK )
{
	VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

	// ������ Ư�� ó�� �ִ� ��� ���
	HandleKOGOTPLoginAck();


	KECH_VERIFY_ACCOUNT_ACK kPacketAck;

	kPacketAck.m_iOK = kPacket_.m_iOK;
	kPacketAck.m_wstrUserID = kPacket_.m_wstrUserID;
	kPacketAck.m_iUserUID = kPacket_.m_iUserUID;
	kPacketAck.m_wstrPassport = kPacket_.m_wstrOTP;
#ifdef SERV_PURCHASE_TOKEN
	kPacketAck.m_wstrPurchaseTok = L"";	// ��ū ���� GENERAL_LOGIN���� ����. ���⼱ NULL ���� 
#endif SERV_PURCHASE_TOKEN

	START_LOG( clog, L"[�׽�Ʈ �α�] Send Packet Token ���� Ȯ��" )
		<< BUILD_LOG( kPacketAck.m_iOK )
		<< BUILD_LOG( kPacketAck.m_wstrUserID )
		<< BUILD_LOG( kPacketAck.m_iUserUID )
		<< BUILD_LOG( kPacketAck.m_wstrPassport )
		<< BUILD_LOG( kPacketAck.m_wstrPurchaseTok )
		<< END_LOG;

	SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacketAck );

    if( kPacket_.m_iOK == NetError::NET_OK )
    {
        StateTransition( KCHFSM::I_TO_AUTHENTICATED );
    }
    else
    {
        ReserveDestroy();
    }
}

IMPL_ON_FUNC( ECH_SECURITY_AUTH_REQ )
{
	VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

	KECH_SECURITY_AUTH_ACK kPacket;

	DWORD dwAuthFlag = KSimLayer::GetKObj()->GetAuthFlag();
	if (KSimLayer::AF_INTERNAL == dwAuthFlag)
	{
		StateTransition( KCHFSM::I_TO_AUTHENTICATED );
		kPacket.m_iOK = NetError::NET_OK;
		SendPacket( ECH_SECURITY_AUTH_ACK, kPacket );
	}

	RequestPublisherSecurityAuth(kPacket_);
}

IMPL_ON_FUNC( EPUBLISHER_SECURITY_AUTH_ACK )
{
	VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );	// �������� ��û�Ǳ⿡ S_INIT��

	HandlePublisherSecurityAuthAck(kPacket_);
}

IMPL_ON_FUNC( EPUBLISHER_SECURITY_INFO_NOT )
{
	VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );	// �������� ��û�Ǳ⿡ S_INIT��

	HandlePublisherSecurityInfoNot(kPacket_);
}

#endif // SERV_GLOBAL_AUTH

#ifdef SERVER_GROUP_UI_ADVANCED
IMPL_ON_FUNC_NOPARAM( ECH_GET_SERVERGROUP_LIST_REQ )
{
	VERIFY_STATE( ( 1, KCHFSM::S_LOGINED ) );

	KECH_GET_SERVERGROUP_LIST_ACK kPacketAck;

	kPacketAck.m_iOK = NetError::NET_OK;
	SendPacket( ECH_GET_SERVERGROUP_LIST_ACK, kPacketAck );

	KECH_GET_SERVERGROUP_LIST_NOT kPacketNot;
	kPacketNot.m_mapServerGroupList = GetKChannelServer()->GetServerGroupList();
	SendPacket( ECH_GET_SERVERGROUP_LIST_NOT, kPacketNot );
}
#endif SERVER_GROUP_UI_ADVANCED
//{{����ȯ �����ð� �޾ƿ���
#ifdef	SERV_SERVER_TIME_GET
IMPL_ON_FUNC_NOPARAM(ECH_GET_SERVER_TIME_REQ)
{
	VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );
	KECH_GET_SERVER_TIME_ACK kPacket;
	CTime tCurrent = CTime::GetCurrentTime();
	kPacket.m_wstrCurrentTime = ( CStringW )tCurrent.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
	SendPacket( ECH_GET_SERVER_TIME_ACK, kPacket );
}
#endif  SERV_SERVER_TIME_GET

#ifdef SERV_ID_NETMARBLE_PCBANG
IMPL_ON_FUNC(ECH_PCBANG_IP_AND_MAC_INFO_NOT)
{

	KDBE_PCBANG_IP_AND_MAC_INFO_NOT kPacketToDB;
	kPacketToDB.m_wstrPublicIP = kPacket_.m_wstrPublicIP;
	kPacketToDB.m_wstrMacInfo = kPacket_.m_wstrMacInfo;
	SendToLogDB( DBE_PCBANG_IP_AND_MAC_INFO_NOT, kPacketToDB );
}
#endif //SERV_ID_NETMARBLE_PCBANG

#ifdef SERV_LOGIN_RESULT_INFO
void KChannelUser::ProcessLoginResultCount( IN const int iResult )
{
	switch( iResult )
	{
	case NetError::NET_OK:
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_SUCCESS);
		break;
	case NetError::ERR_VERIFY_21:	// ���� ���� ����		
	case NetError::ERR_GIANT_VERIFY_07:
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_SYSTEM_ERROR);
		break;
	case NetError::ERR_GAMEFORGE_01:		
	case NetError::ERR_GIANT_VERIFY_06:
#ifdef SERV_COUNTRY_US
	case NetError::ERR_BUY_CASH_ITEM_25:
#endif //SERV_COUNTRY_US
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_ACCOUNT_NOT_EXIST);
		break;
	case NetError::ERR_GAMEFORGE_00:
	case NetError::ERR_GIANT_VERIFY_05:
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_PASSWORD_ERROR);
		break;
	case NetError::ERR_GIANT_VERIFY_02:
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_OTP_PASSWORD_ERROR);
		break;
	case NetError::ERR_K3RCOMBO_AUTH_01:
	case NetError::ERR_GIANT_VERIFY_04:
	case NetError::ERR_GASH_17:	
	case NetError::ERR_VERIFY_11:	
#ifdef SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
	case NetError::ERR_ACCOUNT_BLOCK_01:	
	case NetError::ERR_ACCOUNT_BLOCK_02:	
	case NetError::ERR_ACCOUNT_BLOCK_03:	
	case NetError::ERR_ACCOUNT_BLOCK_04:	
#endif //SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_ACCOUNT_BLOCK);
		break;
	case NetError::ERR_GIANT_VERIFY_03:
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_MATRIX_NUMBER_ERROR);
		break;
	case NetError::ERR_GASH_12:	
	case NetError::ERR_GASH_13:
	case NetError::ERR_GASH_16:	
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_REJECT);
		break;		
	case NetError::ERR_GAMEFORGE_02:
	case NetError::ERR_K3RCOMBO_AUTH_02:	
	case NetError::ERR_UNKNOWN:
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_ACCOUNT_NOT_ACTIVATE);
		break;
	default:
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_UNKNOWN);
		break;
	}
}
#endif SERV_LOGIN_RESULT_INFO