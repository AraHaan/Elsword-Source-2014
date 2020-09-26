#include "StdAfx.h"
#include ".\x2instancedata.h"

#ifdef SERVER_NAME
#include <boost/assign.hpp>
#endif SERVER_NAME

CX2InstanceData::CX2InstanceData(void) :
m_pMiniMapUI( NULL ),
m_bIsDungeonLounge( false ),
m_bLoadedDungeonLoungeUnits( false ),
m_eDungeonLoungeWorldID( CX2World::WI_NONE ),
m_vLastDungeonGatePos( 0, 0, 0 ),
m_UserID( L"" ),
#ifdef SERV_CHANNELING_AERIA
m_UserSessionID( L"" ),
#endif //SERV_CHANNELING_AERIA
m_UserPassword( L"" ),
#ifdef CLIENT_COUNTRY_TH
m_wstrSocketID( L"" ),
#endif // CLIENT_COUNTRY_TH
#ifdef SERV_KOG_OTP_VERIFY
m_bIDVerified(false),
#endif SERV_KOG_OTP_VERIFY
#ifdef SERV_PURCHASE_TOKEN
m_wstrPurchaseTok( L"" ),
#endif SERV_PURCHASE_TOKEN
#ifdef CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH
m_bInHouseAccount(false),
#endif CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH
m_iConnectChannelServerID( -1 ),
m_bFirstJoinVillage(false)
//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ED ȹ�� ����
#ifdef	REAL_TIME_ELSWORD
, m_iED( 0 )
#endif	REAL_TIME_ELSWORD
//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ED ȹ�� ����
, m_bShowCSPandSlotBDate( false )
, m_bIsChangeChannel(false)
, m_bRegisteredRank( false )
//{{ kimhc // �ڵ��������� �ֱ� �����ų� ���� ���������� ���������� �Ǵ� �ϴ� ���� �� �Լ� �߰�
#ifdef	AUTO_PAYMENT
, m_byWebType( 0 )	// �ֱ� �����ų� ���� ���� ����
#endif	AUTO_PAYMENT
//}} kimhc // �ڵ��������� �ֱ� �����ų� ���� ���������� ���������� �Ǵ� �ϴ� ���� �� �Լ� �߰�

//{{ kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�
#ifdef	ADD_SERVER_GROUP
#ifdef EXTEND_SERVER_GROUP_MASK
, m_iServerGroupID( -1 )	// ���� ������ �ȵ��ִ� ��� �ַ����� �ʱ�ȭ 
, m_iServerGroupIDCashShop( -1 )	// ���� ������ �ִ� ĳ�þ����� ����Ʈ�� �޾ƿ� ������ ID
, m_iServerGroupIDScript( -1 )
#else
, m_eServerGroupID( SGI_INVALID )	// ���� ������ �ȵ��ִ� ��� �ַ����� �ʱ�ȭ 
, m_eServerGroupIDCashShop( SGI_INVALID )	// ���� ������ �ִ� ĳ�þ����� ����Ʈ�� �޾ƿ� ������ ID
, m_eServerGroupIDScript( SGI_INVALID )
#endif // EXTEND_SERVER_GROUP_MASK
#endif	ADD_SERVER_GROUP
//}}  kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�
#ifdef DAMAGE_HISTORY
, m_bSendedHackUser(false)
, m_bSendedHackMail(false)
#endif
#ifdef SERV_PROTECT_AUTO_HACK
, m_bSendedHackMailAutoHack(false)
#endif // SERV_PROTECT_AUTO_HACK
, m_bFirstSelect(false)	// kimhc // �α��� �� ó�� ĳ���� �����ΰ�? // PC �� �κ��丮 ����Ʈ �Ǽ��� �۾�
, m_bReceiveGameStartNotPacket( false ) // kimhc // 2010-03-23 // STATE_CHANGE_GAME_START_NOT�� �޾Ҵ°�?	
, m_bSendGameStartReqPacket( false ) // kimhc // 2010-03-25 // Change_state_Game_start_req�� �����ϸ� ���Ŀ��� ���� ������ ���� �� �� ���� ��ų������ �ٲܼ� ������ ��
, m_fRemainedTimeByForceQuitGame(0.f)
#ifdef SERV_PSHOP_AGENCY
, m_bIsPShopOpen(false)
, m_wstrAgencyShopExpirationDate(L"")
#endif
#ifdef MACHINE_ID
, m_strMachineId("")
#endif
, m_TimerForSendingPlayStatus( 3.0f )
#ifdef SERV_NEW_EVENT_TYPES
, m_iMaxLevel( _CONST_X2GAME_::g_iMaxLevel )
#endif SERV_NEW_EVENT_TYPES
#ifdef UDP_CAN_NOT_SEND_USER_KICK // 2012.06.19 lygan_������ // �������� UDP ��Ŷ�� ���� �ʴ� ������ ű�ϴ� �ڵ� Ȱ��ȭ ����
, m_bSetUDPKickOff(false)
#endif //UDP_CAN_NOT_SEND_USER_KICK
#ifdef	USE_ACCOUNT_DOMAIN
, m_eDomain( ACCOUNT_DOMAIN_TYPE::ADT_PLAYID )
#endif	USE_ACCOUNT_DOMAIN
#ifdef CLIENT_PORT_CHANGE_REQUEST
, m_bStartPortChangeRequest(false)
, m_iUDPPortSuccessType(0)
#endif //CLIENT_PORT_CHANGE_REQUEST
#ifdef SERV_ID_NETMARBLE_PCBANG
, m_bPublicIP(false)
#endif //SERV_ID_NETMARBLE_PCBANG
#ifdef SERV_COUNTRY_PH
, m_strAuthToken("")
#endif //SERV_COUNTRY_PH
#ifdef SERV_STEAM
, m_iChannelingCode( 0 )
#endif //SERV_STEAM
#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
, m_bRecruit( false )
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
,m_bUIChungShow(false)
,m_cGetCharClass(0)
#endif SERV_EVENT_CHUNG_GIVE_ITEM
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
,m_bAdamsShopShow(false)
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
#ifdef ALWAYS_SCREEN_SHOT_TEST
,m_bScreenShotTest(false)
#endif ALWAYS_SCREEN_SHOT_TEST
#ifdef SERV_ELISIS_PREVIOUS_SIS_EVENT
,m_iPreEventElesisUID(0)
#endif // SERV_ELISIS_PREVIOUS_SIS_EVENT
{

#if defined( _IN_HOUSE_ ) || defined( _OPEN_TEST_ ) || defined( _OPEN_TEST_2_ )
	m_bFrameScale = false;
#endif

#if defined( SERV_HACKING_TOOL_LIST )

	THEMIDA_VM_START

#if defined( _SERVICE_ ) 
	// checked
	ELSWORD_VIRTUALIZER_START

#endif


#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
    m_vecHackList_MainThread.resize( 0 );
    {
        CSLock  lock( m_csHackList );
	    m_bChangeHacklist = false;
        m_vecHackList.clear();
    }
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX

#if 0
	KHackingToolInfo hackInfo;
	hackInfo.m_cFlag = 2;
	hackInfo.m_wstrProcessName = L"OllyDbg";
	hackInfo.m_wstrWindowClassName = L"";
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
    m_vecHackList_MainThread.push_back( hackInfo );
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
	PushHackList(hackInfo);
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
	hackInfo.m_cFlag = 2;
	hackInfo.m_wstrProcessName = L"Ollyice";
	hackInfo.m_wstrWindowClassName = L"";
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
    m_vecHackList_MainThread.push_back( hackInfo );
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
	PushHackList(hackInfo);
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
	hackInfo.m_cFlag = 7;
	hackInfo.m_wstrProcessName = L"HanAier.dll";
	hackInfo.m_wstrWindowClassName = L"";
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
    m_vecHackList_MainThread.push_back( hackInfo );
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
	PushHackList(hackInfo);
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
	hackInfo.m_cFlag = 7;
	hackInfo.m_wstrProcessName = L"ppd.dll";
	hackInfo.m_wstrWindowClassName = L"";
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
    m_vecHackList_MainThread.push_back( hackInfo );
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
	PushHackList(hackInfo);
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
#endif


#ifdef VERIFY_NPC_HP
	m_bVerifyNpcHp = false;
#endif

#ifdef EXTEND_SERVER_GROUP_MASK
#else EXTEND_SERVER_GROUP_MASK
#ifdef SERVER_NAME
	using namespace boost::assign;
	insert( m_mapServerGroupName )
		( SGI_INVALID, L"Invalid ServerGroup" )
		( SGI_SOLES, GET_STRING( STR_ID_4836 ) ) 
		( SGI_GAIA, GET_STRING( STR_ID_4835 ) )
		;
#endif SERVER_NAME
#endif EXTEND_SERVER_GROUP_MASK

#ifdef SERV_EPAY_SYSTEM
	m_usGiantZone = 0;
	m_usGiantGame = 0;
#endif //SERV_EPAY_SYSTEM

#ifdef ADD_KPROTECTEDTYPE_VALUE
	m_nVerifyNpcStateId = 0;
#endif

#ifdef SERV_DLL_MANAGER
	m_bSendedLoadedModuleList = false;
#endif

#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	m_iChannelRandomKey = -1;
#endif
#ifdef SERV_ED_MONITORING_IN_GAME
	m_bMoneyMonitoring = false;
#endif
#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_END
#endif

#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	memset(m_cServerSN, 0, sizeof(unsigned char) * SERVER_SN);
	memset(m_cOldServerSN, 0, sizeof(unsigned char) * SERVER_SN);
#endif

#ifdef ADD_COLLECT_CLIENT_INFO
	memset( g_AntiHackingList, 0, sizeof(unsigned int) * ANTIHACKING_ID::ANTIHACKING_ID_END );
#endif ADD_COLLECT_CLIENT_INFO

	THEMIDA_VM_END

#endif // defined( SERV_HACKING_TOOL_LIST )

	m_iLastDungeonGatePosLineIndex = 0;

#ifdef X2TOOL
	m_bIsNpcLoad = false;
	m_bCanDungeonPlay = false;
	m_bChangeStage = false;
#endif

#ifdef SERV_NAVER_CHANNELING
	m_strNaverAccessToken.clear();
#endif // SERV_NAVER_CHANNELING

#ifdef FIX_REFORM_ENTRY_POINT_10TH			//	kimjh,  ĳ���� ����Ʈ �������� ������ ����
	m_vecTryConnectChannelID.clear();
#endif // FIX_REFORM_ENTRY_POINT_10TH		//	kimjh,  ĳ���� ����Ʈ �������� ������ ����

#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
	m_bChoicedItem = false;
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	m_bStartUI = false;
	m_DungeonCountUI = false;
	m_FieldCountUI = false;
	m_DungeonCount = 0;
	m_FieldMonsterKillCount = 0;
	m_iRemaindTime = -1;
	m_iTimeSecond = 0;
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
}

CX2InstanceData::~CX2InstanceData(void)
{
	SAFE_DELETE( m_pMiniMapUI );

}

void CX2InstanceData::OnFrameMove( double fTime, float fElapsedTime )
{
	//{{ 09.03.23 �¿�
	if( NULL != m_pMiniMapUI )
	{
		m_pMiniMapUI->OnFrameMove( fTime, fElapsedTime );
	}
	//}}

#ifdef FIX_GAGEMANAGER01
	// ���� ���� ���� ���� �ð� �κ��� �̻��ϰ� ���� ��ٸ� �ٷ� ���� ����
	if ( !m_fRemainedTimeByForceQuitGame.Verify() )
	{
		g_pMain->SetQuitType( NetError::ERR_CLIENT_QUIT_01 );
		PostQuitMessage(0);
	}

	if ( GetVerifyGageManagerTimer() > 0.0f )
	{
		m_fRemainedTimeByForceQuitGame -= fElapsedTime;
		if( m_fRemainedTimeByForceQuitGame <= 0.f )
		{
			m_fRemainedTimeByForceQuitGame = 0.f;
			if ( NULL != g_pKTDXApp )
			{
				// kimhc // ����ö // �� �������� ���⼭ ���� ó�� ����
				// ��ü ��ŷ Ž�� �� �ڵ� �����丵 �� �ؾ߰ڴ�...
				g_pMain->SetQuitType( NetError::ERR_CLIENT_QUIT_01 );
				PostQuitMessage(0);
			}
		}
	}
#endif
}

void CX2InstanceData::ResetMiniMapUI()
{
	SAFE_DELETE( m_pMiniMapUI );
	m_pMiniMapUI = new CX2MiniMapUI;
}

//{{ kimhc // 2009-12-18 // ������ �߰� �۾����� ������� ����
#ifndef	ADD_SERVER_GROUP

int CX2InstanceData::GetChannelIdFromListId(int listId)
{
	// ä���߰�
	switch(listId)
	{
	case 1:			// �纥1
		return 1;
	case 2:			// �纥2
		return 3;
	case 3:			// �纥3
		return 5;
	case 4:			// ����1
		return 2;
	case 5:			// ����2
		return 4;	
	case 6:			// ����3
		return 6;
	case 7:			// ������1
		return 7;
	case 8:			// ������2
		return 9;
	case 9:			// ������3
		return 11;
	case 10:		// ������4
		return 13;
	case 11:		// ������5
		return 15;
	case 12:		// ���׶�1
		return 8;
	case 13:		// ���׶�2
		return 10;
	case 14:		// ���׶�3
		return 12;
	case 15:		// ���׶�4
		return 14;
	case 16:		// ���׶�5
		return 16;
	}
	return 0;
}

int CX2InstanceData::GetListIdFromChannelId(int channelId)
{
	// ä���߰�
	switch(channelId)
	{
	case 1:			// �纥1
		return 1;
	case 2:			// ����1
		return 4;
	case 3:			// �纥2
		return 2;
	case 4:			// ����2
		return 5;
	case 5:			// �纥3
		return 3;
	case 6:			// ����3
		return 6;
	case 7:			// ������1
		return 7;
	case 8:			// ���׶�1
		return 12;
	case 9:			// ������2
		return 8;
	case 10:		// ���׶�2
		return 13;
	case 11:		// ������3
		return 9;
	case 12:		// ���׶�3
		return 14;
	case 13:		// ������4
		return 10;
	case 14:		// ���׶�4
		return 15;
	case 15:		// ������5
		return 11;
	case 16:		// ���׶�5
		return 16;
	}

	return 0;
}
#endif	ADD_SERVER_GROUP
//}} kimhc // 2009-12-18 // ������ �߰� �۾����� ������� ����

wstring CX2InstanceData::GetConnectChannelServerName()
{
	return GetChannelServerName( m_iConnectChannelServerID );
}
#ifdef SERVER_INTEGRATION_CHANNEL_NAME_FIX
#ifdef EXTEND_SERVER_GROUP_MASK
wstring CX2InstanceData::GetChannelServerName( int ChannelID, int ServerID)
{
	wstring wstrChannelName = L"";
#ifdef SERV_CHANNEL_LIST_RENEWAL
	if ( m_vecChannelInfo.empty() == false )
	{
		for(int i = 0; i < static_cast<signed>( m_vecChannelInfo.size() ); i ++)
		{
			if(m_vecChannelInfo[i].m_iChannelID == ChannelID)
			{
				wstrChannelName = m_vecChannelInfo[i].m_wstrChannelName;
			}
		}
	}
#else //SERV_CHANNEL_LIST_RENEWAL
	if(ServerID == 1 || ServerID == 3)
	{
		switch(ChannelID)
		{
		case 1:		// �纥1
			wstrChannelName = GET_STRING( STR_ID_232 );
			break;
		case 2:		// �纥2
			wstrChannelName = GET_STRING( STR_ID_233 );
			break;
		case 3:		// �纥3
			wstrChannelName = GET_STRING( STR_ID_236 );
			// ����1
			//wstrChannelName = GET_STRING( STR_ID_236 );
			break;
		case 4:		// �纥4
			//wstrChannelName = GET_STRING( STR_ID_235 );
			// ����1
			wstrChannelName = GET_STRING( STR_ID_237 );
			break;
		case 5:		// ����1
			//wstrChannelName = GET_STRING( STR_ID_236 );
			// ����2
			wstrChannelName = GET_STRING( STR_ID_240 );
			break;
		case 6:		// ����2
			//wstrChannelName = GET_STRING( STR_ID_237 );
			// ����3
			wstrChannelName = GET_STRING( STR_ID_241 );
			break;
		case 7:		// ����3
			//wstrChannelName = GET_STRING( STR_ID_238 );
			// ������1
			wstrChannelName = GET_STRING( STR_ID_4751 );
			break;
		case 8:		// ����4
			//wstrChannelName = GET_STRING( STR_ID_239 );
			// ������2
			wstrChannelName = GET_STRING( STR_ID_4773 );
			break;
		case 9:		// ������1
			//wstrChannelName = GET_STRING( STR_ID_240 );
			// ������3
			wstrChannelName = GET_STRING( STR_ID_15168 );
			break;
		case 10:	// ������2
			//wstrChannelName = GET_STRING( STR_ID_241 );
			// ������4
			//wstrChannelName = GET_STRING( STR_ID_243 );
			// ���׶�1
			wstrChannelName = GET_STRING( STR_ID_15227 );
			break;
		case 11:	// ������3
			//wstrChannelName = GET_STRING( STR_ID_242 );
			// ���׶�1
			//wstrChannelName = GET_STRING( STR_ID_4751 );
			// ���׶�2
			wstrChannelName = GET_STRING( STR_ID_15228 );
			break;
		case 12:	// ������4
			//wstrChannelName = GET_STRING( STR_ID_243 );
			// ���׶�2
			wstrChannelName = GET_STRING( STR_ID_15385 );
			break;
		case 13:	// ������4
			wstrChannelName = GET_STRING( STR_ID_244 );
			break;
		case 14:	// ���׶�4
			wstrChannelName = GET_STRING( STR_ID_4751 );
			break;
		case 15:	// ������5
			wstrChannelName = GET_STRING( STR_ID_4780 );	
			break;
		case 16:	// ���׶�5
			wstrChannelName = GET_STRING( STR_ID_4775 );		
			break;
		default:
			break;
		}
	}
	else if(ServerID == 2)
	{
		switch(ChannelID)
		{
		case 1:		// �纥1
			wstrChannelName = GET_STRING( STR_ID_232 );
			break;
		case 2:		// �纥2
			wstrChannelName = GET_STRING( STR_ID_236 );
			break;
		case 3:		// ����1
			wstrChannelName = GET_STRING( STR_ID_240 );
			break;
		case 4:		// ����2
			wstrChannelName = GET_STRING( STR_ID_4751 );
			break;
		case 5:		// ������1
			wstrChannelName = GET_STRING( STR_ID_4773 );
			break;
		case 6:		// ������2
			wstrChannelName = GET_STRING( STR_ID_15227 );
			break;
		case 7:		// ���׶�1
			wstrChannelName = GET_STRING( STR_ID_15228 );
			break;
		case 8:		// ���׶�2
			wstrChannelName = GET_STRING( STR_ID_15385 );
			break;
		case 9:		// ������2
			wstrChannelName = GET_STRING( STR_ID_240 );
			break;
		case 10:	// ���׶�2
			wstrChannelName = GET_STRING( STR_ID_241 );
			break;
		case 11:	// ������3
			wstrChannelName = GET_STRING( STR_ID_242 );
			break;
		case 12:	// ���׶�3
			wstrChannelName = GET_STRING( STR_ID_243 );
			break;
		case 13:	// ������4
			wstrChannelName = GET_STRING( STR_ID_244 );
			break;
		case 14:	// ���׶�4
			wstrChannelName = GET_STRING( STR_ID_4751 );
			break;
		case 15:	// ������5
			wstrChannelName = GET_STRING( STR_ID_4780 );	
			break;
		case 16:	// ���׶�5
			wstrChannelName = GET_STRING( STR_ID_4775 );		
			break;
		default:
			break;
		}
	}
	else
	{
		switch(ChannelID)
		{
		case 1:		// �纥1
			wstrChannelName = GET_STRING( STR_ID_232 );
			break;
		case 2:		// �纥2
			wstrChannelName = GET_STRING( STR_ID_233 );
			break;
		case 3:		// ����1
			wstrChannelName = GET_STRING( STR_ID_234 );
			break;
		case 4:		// ����2
			wstrChannelName = GET_STRING( STR_ID_236 );
			break;
		case 5:		// ������1
			wstrChannelName = GET_STRING( STR_ID_237 );
			break;
		case 6:		// ������2
			wstrChannelName = GET_STRING( STR_ID_238 );
			break;
		case 7:		// ���׶�1
			wstrChannelName = GET_STRING( STR_ID_240 );
			break;
		case 8:		// ���׶�2
			wstrChannelName = GET_STRING( STR_ID_241 );
			break;
		case 9:		// ������2
			wstrChannelName = GET_STRING( STR_ID_242 );
			break;
		case 10:	// ���׶�2
			wstrChannelName = GET_STRING( STR_ID_4751 );
			break;
		case 11:	// ������3
			wstrChannelName = GET_STRING( STR_ID_4773 );
			break;
		case 12:	// ���׶�3
			wstrChannelName = GET_STRING( STR_ID_15168 );
			break;
		case 13:	// ������4
			wstrChannelName = GET_STRING( STR_ID_15169 );
			break;
		case 14:	// ���׶�4
			wstrChannelName = GET_STRING( STR_ID_15227 );
			break;
		case 15:	// ������5
			wstrChannelName = GET_STRING( STR_ID_15228 );	
			break;
		case 16:	// ���׶�5
			wstrChannelName = GET_STRING( STR_ID_15385 );		
			break;
		default:
			break;
		}
	}
#endif //SERV_CHANNEL_LIST_RENEWAL

	return wstrChannelName;
}
#else
wstring CX2InstanceData::GetChannelServerName( int ChannelID, SERVER_GROUP_ID ServerID)
{
	wstring wstrChannelName = L"";

#ifdef SERV_CHANNEL_LIST_RENEWAL
	if ( m_vecChannelInfo.empty() == false )
	{
		for( UINT i=0; i < m_vecChannelInfo.size(); ++i )
		{
			if(m_vecChannelInfo[i].m_iChannelID == ChannelID)
			{
				wstrChannelName = m_vecChannelInfo[i].m_wstrChannelName;
			}
		}
	}
#else //SERV_CHANNEL_LIST_RENEWAL
	switch (ServerID)
	{
	case SGI_SOLES:
		{
			switch(ChannelID)
			{
			case 1:		// �纥1
				wstrChannelName = GET_STRING( STR_ID_232 );
				break;
			case 2:		// ���׶�2 
				wstrChannelName = GET_STRING( STR_ID_241 );
				break;
			case 3:		// �纥2
				wstrChannelName = GET_STRING( STR_ID_234 );
				break;
			case 4:		// ����1
				wstrChannelName = GET_STRING( STR_ID_15227 );
				break;
			case 5:		// ����1
				wstrChannelName = GET_STRING( STR_ID_233 );
				break;
			case 6:		// ����2
				wstrChannelName = GET_STRING( STR_ID_15228 );
				break;
			case 7:		// ����2
				wstrChannelName = GET_STRING( STR_ID_235 );
				break;
			case 8:		// ����3
				wstrChannelName = GET_STRING( STR_ID_15385 );
				break;
			case 9:		// ������1
				wstrChannelName = GET_STRING( STR_ID_238 );
				break;
			case 10:	// �ϸ�1
				wstrChannelName = GET_STRING( STR_ID_15229 );
				break;
			case 11:	// ������2
				wstrChannelName = GET_STRING( STR_ID_240 );
				break;
			case 12:	// �ϸ�2
				wstrChannelName = GET_STRING( STR_ID_15230 );
				break;
			case 13:	// ���׶�1
				wstrChannelName = GET_STRING( STR_ID_239 );
				break;
			case 14:	// �ϸ�3
				wstrChannelName = GET_STRING( STR_ID_15386 );
				break;
			default:
				break;
			}
		}
		break;
	case SGI_GAIA:
		{
			switch(ChannelID)
			{
			case 1:		// �纥1
				wstrChannelName = GET_STRING( STR_ID_232 );
				break;
			case 2:		// ���׶�1
				wstrChannelName = GET_STRING( STR_ID_239 );
				break;
			case 3:		// �纥2
				wstrChannelName = GET_STRING( STR_ID_234 );
				break;
			case 4:		// ���׶�2
				wstrChannelName = GET_STRING( STR_ID_241 );
				break;
			case 5:		// �纥3
				wstrChannelName = GET_STRING( STR_ID_236 );
				break;
			case 6:		// ����1
				wstrChannelName = GET_STRING( STR_ID_15227 );
				break;
			case 7:		// ����1
				wstrChannelName = GET_STRING( STR_ID_233 );
				break;
			case 8:		// ����2
				wstrChannelName = GET_STRING( STR_ID_15228 );
				break;
			case 9:		// ����2
				wstrChannelName = GET_STRING( STR_ID_235 );
				break;
			case 10:	// �ϸ�1
				wstrChannelName = GET_STRING( STR_ID_15229 );
				break;
			case 11:	// ������1
				wstrChannelName = GET_STRING( STR_ID_238 );
				break;
			case 12:	// �ϸ�2
				wstrChannelName = GET_STRING( STR_ID_15230 );
				break;
			case 13:	// ������2
				wstrChannelName = GET_STRING( STR_ID_240 );
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
#endif
	return wstrChannelName;
}
#endif EXTEND_SERVER_GROUP_MASK
#endif SERVER_INTEGRATION_CHANNEL_NAME_FIX

wstring CX2InstanceData::GetChannelServerName( int ChannelID )
{
	//{{ kimhc // 2009-12-15 // ������ �߰� �۾�
#ifdef	ADD_SERVER_GROUP
	if( 0 == ChannelID )
	{
		ErrorLogMsg(XEM_ERROR143,"ChannelID == 0");
		return L"";
	}
	
	wstring wstrChannelName = L"";
	if ( false == m_vecChannelInfo.empty() && (ChannelID-1) < static_cast<int>(m_vecChannelInfo.size()) )
	{
#ifdef	SERV_CHANNEL_LIST_RENEWAL
		for( UINT i = 0; i < m_vecChannelInfo.size(); ++i )
		{
			if(m_vecChannelInfo[i].m_iChannelID == ChannelID)
			{
				wstrChannelName = m_vecChannelInfo[i].m_wstrChannelName;
			}
		}

		return wstrChannelName;
#else	//SERV_CHANNEL_LIST_RENEWAL
		return m_vecChannelInfo[ChannelID - 1].m_wstrChannelName;
#endif //SERV_CHANNEL_LIST_RENEWAL
	}
	else
	{
		ErrorLog(XEM_ERROR143);
		return wstrChannelName;
	}
#else	ADD_SERVER_GROUP
	// ä���߰�
	// ���� ������ ä�� �̸��� �����Ѵ�.
	wstring wstrChannelName = L"";

	switch(ChannelID)
	{
	case 1:		// �纥1
		wstrChannelName = GET_STRING( STR_ID_232 );
		break;
	case 2:		// ����1
		wstrChannelName = GET_STRING( STR_ID_233 );
		break;
	case 3:		// �纥2
		wstrChannelName = GET_STRING( STR_ID_234 );
		break;
	case 4:		// ����2
		wstrChannelName = GET_STRING( STR_ID_235 );
		break;
	case 5:		// �纥3
		wstrChannelName = GET_STRING( STR_ID_236 );
		break;
	case 6:		// ����3
		wstrChannelName = GET_STRING( STR_ID_237 );
		break;
	case 7:		// ������1
		wstrChannelName = GET_STRING( STR_ID_238 );
		break;
	case 8:		// ���׶�1
		wstrChannelName = GET_STRING( STR_ID_239 );
		break;
	case 9:		// ������2
		wstrChannelName = GET_STRING( STR_ID_240 );
		break;
	case 10:	// ���׶�2
		wstrChannelName = GET_STRING( STR_ID_241 );
		break;
	case 11:	// ������3
		wstrChannelName = GET_STRING( STR_ID_242 );
		break;
	case 12:	// ���׶�3
		wstrChannelName = GET_STRING( STR_ID_243 );
		break;
	case 13:	// ������4
		wstrChannelName = GET_STRING( STR_ID_244 );
		break;
	case 14:	// ���׶�4
		wstrChannelName = GET_STRING( STR_ID_4751 );
		break;
	case 15:	// ������5
		wstrChannelName = GET_STRING( STR_ID_4780 );	
		break;
	case 16:	// ���׶�5
		wstrChannelName = GET_STRING( STR_ID_4775 );		
		break;
	default:
		break;
	}

	return wstrChannelName;
#endif	ADD_SERVER_GROUP
	//}}  kimhc // 2009-12-15 // ������ �߰� �۾�
}

//{{ kimhc // 2009-12-15 // ������ �߰� �۾�
#ifdef	ADD_SERVER_GROUP

void CX2InstanceData::SetChannelDistributionByServerGroup( OUT float* pfChCongestionScopeTemp, IN const float* pfChCongestionScope, IN int iChannelIndex )
{
	// ä�� �׷쿡�� ù��° ä�������� �Ǵ��ϴ� �÷��� ex) �纥01, 02, 03 ä���� �纥01�� ù��° ä��
	bool bIsFirstChannelInChannelGroup = false;

	switch ( g_pInstanceData->GetServerGroupID() )
	{
	case SGI_GAIA:
		{

			switch ( iChannelIndex )
			{
			case 0:
			case 3:
			case 6:
			case 7:
			case 11:
			case 12:
				{
					bIsFirstChannelInChannelGroup = true;
				}
				break;
			}
		}
		break;

	case SGI_SOLES:
	default:
		{
			switch ( iChannelIndex )
			{
			case 0:
			case 5:
			case 6:
			case 11:
			case 12:
			case 18:
				{
					bIsFirstChannelInChannelGroup = true;
				}
			}
		}
		break;
	}


	if ( bIsFirstChannelInChannelGroup == false )	// ä�� �׷쿡�� ù��° ä���� �ƴϸ�
	{
		std::copy( pfChCongestionScope, pfChCongestionScope + 4, pfChCongestionScopeTemp );
	}
	else
	{
		// ä�α׷��� ùä���� ������ �ٸ��� ǥ��
		pfChCongestionScopeTemp[0] = 0.1f;
		pfChCongestionScopeTemp[1] = 0.2f;
		pfChCongestionScopeTemp[2] = 0.3f;
		pfChCongestionScopeTemp[3] = pfChCongestionScope[3];
	}
}

#ifdef SHOW_SERVERGROUP_NAME
void CX2InstanceData::RefreshTitle()
{
#ifdef _SERVICE_
	std::wstring wstrTitle( L"Elsword" );
	switch( m_iServerGroupID )
	{
	case 1:
		// ȭ��1��
		wstrTitle += std::wstring( L" - " ) + GET_STRING( STR_ID_26141 );
		break;
	case 6:
		// ȭ��2��
		wstrTitle += std::wstring( L" - " ) + GET_STRING( STR_ID_26142 );
		break;
	case 4:
		// ȭ��1��
		wstrTitle += std::wstring( L" - " ) + GET_STRING( STR_ID_26143 );
		break;
	case 101:
		// ȭ��1��
		wstrTitle += std::wstring( L" - " ) + GET_STRING( STR_ID_26144 );
		break;
	default:
		break;
	}

	SetWindowText( g_pKTDXApp->GetHWND(), wstrTitle.c_str() );
#endif _SERVICE_
}
#endif SHOW_SERVERGROUP_NAME

#ifdef EXTEND_SERVER_GROUP_MASK
void CX2InstanceData::SetServerGroupID( int iServerGroupID ) 
{ 
	m_iServerGroupID = iServerGroupID;
#ifdef SHOW_SERVERGROUP_NAME
	RefreshTitle();
#endif SHOW_SERVERGROUP_NAME
}
#else
void CX2InstanceData::SetServerGroupID( SERVER_GROUP_ID eServerGroupID ) 
{ 
	m_eServerGroupID = eServerGroupID; 
#ifdef SHOW_SERVERGROUP_NAME
	RefreshTitle();
#endif SHOW_SERVERGROUP_NAME
}
#endif // EXTEND_SERVER_GROUP_MASK
#endif	ADD_SERVER_GROUP
//}}  kimhc // 2009-12-15 // ������ �߰� �۾�

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
// ä�� �̸��� �޾Ƽ� DLG_UI_Channel_Selection_New.lua �� ���̴� �� ä�� ��ư �̸��� ��ȯ��
wstring CX2InstanceData::GetChannelButtonNameByChannelName( wstring wstrChannelName ) 
{ 
	if ( wstrChannelName.find ( GET_STRING(STR_ID_17731) ) == 0 )	//�纥
	{
		return L"Ruben";
	}
	else if ( wstrChannelName.find ( GET_STRING(STR_ID_17732) ) == 0)	//����
	{
		return L"Elder";
	}
	else if ( wstrChannelName.find ( GET_STRING(STR_ID_17733) ) == 0)	//������
	{
		return L"Besma";
	}
	else if ( wstrChannelName.find ( GET_STRING(STR_ID_17734) ) == 0)	//���׶�
	{
		return L"Altera";
	}
	else if ( wstrChannelName.find ( GET_STRING(STR_ID_23098) ) == 0 )	//����Ÿ  
	{
		return L"Peita";
	}
	else if ( wstrChannelName.find ( GET_STRING(STR_ID_17735) ) == 0 )	//����
	{
		return L"Velder";
	}
	else if ( wstrChannelName.find ( GET_STRING(STR_ID_17736) ) == 0 )	//�ϸ�
	{
		return L"Hamel";
	}

	return L"Ruben";
}
void CX2InstanceData::AddUserUnitDataInServer( SEnum::SERVER_GROUP_ID eServerGroupID, int iNowUserUnitManyInServer, int iMaxUserUnitManyInServer )
{
	m_mapUserUnitDataInServer.insert ( std::make_pair( eServerGroupID, std::make_pair ( iNowUserUnitManyInServer, iMaxUserUnitManyInServer) ) );
}

void CX2InstanceData::ClearUserUnitDataInServer( ) 
{
	m_mapUserUnitDataInServer.clear();
}


int CX2InstanceData::GetMaxUserUnitManyInServer( SEnum::SERVER_GROUP_ID eServerGroupID ) 
{
#ifdef FIX_REFORM_ENTRY_POINT_CREATE_UNIT_UNLIMITED
	return 500;
#else // FIX_REFORM_ENTRY_POINT_CREATE_UNIT_UNLIMITED
	map < SEnum::SERVER_GROUP_ID, pair< int, int > >::const_iterator cit = m_mapUserUnitDataInServer.find ( eServerGroupID );
	if ( cit != m_mapUserUnitDataInServer.end() )
	{
		return cit->second.second;
	}

#ifdef FORCE_SERVER_GROUP_ID_SETTING_WHEN_ERROR
	{
		eServerGroupID = SEnum::SGI_SOLES;
		map < SEnum::SERVER_GROUP_ID, pair< int, int > >::const_iterator cit = m_mapUserUnitDataInServer.find ( eServerGroupID );
		if ( cit != m_mapUserUnitDataInServer.end() )
		{
			return cit->second.second;
		}
	}
#else
	ASSERT ( ! L"Can't Find Server Group ID ! " );
#endif // FORCE_SERVER_GROUP_ID_SETTING_WHEN_ERROR

#endif // FIX_REFORM_ENTRY_POINT_CREATE_UNIT_UNLIMITED 
	return 0;
}


int CX2InstanceData::GetNowUserUnitManyInServer( SEnum::SERVER_GROUP_ID eServerGroupID ) 
{
	map < SEnum::SERVER_GROUP_ID, pair< int, int > >::const_iterator cit = m_mapUserUnitDataInServer.find ( eServerGroupID );
	if ( cit != m_mapUserUnitDataInServer.end() )
	{
		return cit->second.first;
	}

#ifdef FORCE_SERVER_GROUP_ID_SETTING_WHEN_ERROR
	{
		eServerGroupID = SEnum::SGI_SOLES;
		map < SEnum::SERVER_GROUP_ID, pair< int, int > >::const_iterator cit = m_mapUserUnitDataInServer.find ( eServerGroupID );
		if ( cit != m_mapUserUnitDataInServer.end() )
		{
			return cit->second.first;
		}
	}
#else
	ASSERT ( ! L"Can't Find Server Group ID ! " );	
#endif // FORCE_SERVER_GROUP_ID_SETTING_WHEN_ERROR

	return 0;
}

#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifdef	ADD_SERVER_GROUP
//{{ robobeg : 2013-12-19
    // ���� ���� �ߺ��Ǿ� �ִ� �ڵ带 �� ������ �����ϴ�.
bool CX2InstanceData::OpenScriptServerGroupFile()			// ������ �÷��� �ߴ� ������ �б�
{
	string			strFileName;
	SERVER_GROUP_ID eServerGroupID	= SGI_INVALID;
	bool			bParsingOK		= false;

#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    KLuaManagerProxy    luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	if( g_pKTDXApp->LoadAndDoMemory_LocalFile( &luaManager, g_pData->GetSavedServerGroupFileName().c_str() ) == true )
    {
        LUA_GET_VALUE_ENUM( luaManager, "SERVER_GROUP", 			eServerGroupID,			SERVER_GROUP_ID,		SGI_INVALID	);
    }
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	ConvertWCHARToChar( strFileName, g_pData->GetSavedServerGroupFileName() );
	ConvertFileAnsiToUTF8( strFileName, strFileName );

	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );

	if( g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->IsValidFile_LocalFile( g_pData->GetSavedServerGroupFileName() ) == true )
	{
		if( true == g_pKTDXApp->LoadLuaTinker_LocalFile( g_pData->GetSavedServerGroupFileName().c_str() ) )
		{
			if( true == g_pKTDXApp->LoadAndDoMemory_LocalFile( &luaManager, g_pData->GetSavedServerGroupFileName().c_str() ) )
			{
				LUA_GET_VALUE_ENUM( luaManager, "SERVER_GROUP", 			eServerGroupID,			SERVER_GROUP_ID,		SGI_INVALID	);
			}
		}
	}
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	switch ( eServerGroupID )
	{
	case SGI_SOLES:
	case SGI_GAIA:
		{
			g_pInstanceData->SetServerGroupID( eServerGroupID );
			bParsingOK = true;
		}
		break;

	default:
		{
#ifdef RANDOM_SERVER
			if( g_pMain->GetDefaultChannelServerIPIndex() == SGI_INVALID )
			{
				g_pInstanceData->SetServerGroupID( static_cast<SERVER_GROUP_ID>( (rand() % 2) ) );
			}
			else
			{
				g_pInstanceData->SetServerGroupID( static_cast<SERVER_GROUP_ID>( g_pMain->GetDefaultChannelServerIPIndex() ) );
			}
#else
			g_pInstanceData->SetServerGroupID( static_cast<SERVER_GROUP_ID>( g_pMain->GetDefaultChannelServerIPIndex() ) );
#endif
		}
		break;

	}

	return bParsingOK;
}
//}} robobeg : 2013-12-19

//{{ kimhc // 2009-12-15 // ������ �÷��� �ߴ� ������ ����
bool    CX2InstanceData::SaveScriptServerGroupFile()
{
	string strFileName;

	ConvertWCHARToChar( strFileName, g_pData->GetSavedServerGroupFileName() );


	FILE* file = NULL;
	file = fopen( strFileName.c_str(), "w" );		
	
	if( NULL == file )
	{
		ErrorLogMsg( XEM_ERROR7, strFileName.c_str() );
		return false;
	}

	fputc( 0xEF, file );
	fputc( 0xBB, file );
	fputc( 0xBF, file );

	fwprintf( file, L"  \n" );
	fwprintf( file, L"  \n" );
	fwprintf( file, L"SERVER_GROUP = " );

	switch( g_pInstanceData->GetServerGroupID() )
	{
	case SGI_GAIA:
		fwprintf( file, L"SERVER_GROUP_ID[\"SGI_GAIA\"]" );
		break;

	case SGI_SOLES:
	default:
		fwprintf( file, L"SERVER_GROUP_ID[\"SGI_SOLES\"]" );
		break;
	}

	fclose( file );

	return true;
}

#endif  ADD_SERVER_GROUP
//}}  kimhc // 2009-12-15 // ������ �÷��� �ߴ� ������ ����		

#ifdef FIX_REFORM_ENTRY_POINT_10TH		//	kimjh,  ĳ���� ����Ʈ �������� ������ ����
bool CX2InstanceData::IsConnectedChannel ( int iChannelID_ )
{
	// ä�ο� �����߾����� Ȯ���Ѵ�.
	BOOST_FOREACH ( int iChannelID, m_vecTryConnectChannelID )
	{
		if ( iChannelID == iChannelID_ )
			return true;
	}
	return false;
}
void CX2InstanceData::SetConnectedChannelID ( int iChannelID_ )
{
	// ���� �õ��� ä���� ���̵� �����.
	m_vecTryConnectChannelID.push_back ( iChannelID_ );
}
void CX2InstanceData::ResetConnectedChannelID ()
{
	// ���� �õ��� ä���� ���̵� �ʱ�ȭ �Ѵ�.
	m_vecTryConnectChannelID.clear();
}

#endif // FIX_REFORM_ENTRY_POINT_10TH	//	kimjh,  ĳ���� ����Ʈ �������� ������ ����

#ifdef SERVER_NAME
const wstring CX2InstanceData::GetServerGroupName()
{
#ifdef EXTEND_SERVER_GROUP_MASK
#ifdef SERVER_GROUP_UI_ADVANCED
	map<int, KServerGroupInfo>::iterator iter = m_mapServerGroupInfo.find(GetServerGroupID());
	if( iter != m_mapServerGroupInfo.end() )
		return iter->second.m_wstrServerGroupName;
	else
		return std::wstring();
#else
	if( m_mapServerGroupName.end() != m_mapServerGroupName.find(GetServerGroupID()) )
		return m_mapServerGroupName[m_iServerGroupID];
	else
		return std::wstring();
#endif SERVER_GROUP_UI_ADVANCED
#else
	if( m_mapServerGroupName.end() != m_mapServerGroupName.find(GetServerGroupID()) )
		return m_mapServerGroupName[m_eServerGroupID];
	else
		return std::wstring();
#endif // EXTEND_SERVER_GROUP_MASK
}
#endif SERVER_NAME

#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
void CX2InstanceData::SetNowSubCategoryList( IN const vector< CX2CashShop::CashShopCategory* >& m_vecCashShopCateList, IN const int iCategory_ )
{
	m_vecNowSubCategoryList.clear();

	for ( int cateIndex = 0; cateIndex < (int)m_vecCashShopCateList.size(); cateIndex++ )
	{
		CX2CashShop::CashShopCategory* pCashShopCategory = m_vecCashShopCateList[cateIndex];
		if ( pCashShopCategory != NULL )
		{
			if ( (int)pCashShopCategory->m_CategoryID.x == iCategory_ )
			{
				m_vecNowSubCategoryList = pCashShopCategory->m_vecSubCategoryID;
			}
		}
	}
}

bool CX2InstanceData::IsCurrentSubCategoryInNowCatagory( IN const int iSubCategory_ )
{
	if( true == m_vecNowSubCategoryList.empty() )
		// �� ī�װ� ����ִ°���, ���� ������! �� ��������.
		return true;

	BOOST_TEST_FOREACH( const D3DXVECTOR2, v2Val, m_vecNowSubCategoryList)
	{
		if( v2Val.y == iSubCategory_ )
			return true;
	}

	return false;
}

vector< KBillProductInfo > CX2InstanceData::GetCurrentProductInfoListInNowCatagory( vector< KBillProductInfo >& vecBillInfo_ )
{
	vector< KBillProductInfo > vecBillInfoTemp;

	BOOST_TEST_FOREACH( const KBillProductInfo, kInfo, vecBillInfo_)
	{
		if( true == IsCurrentSubCategoryInNowCatagory(kInfo.m_cCategoryNo) )
			vecBillInfoTemp.push_back(kInfo);
	}

	return vecBillInfoTemp;
}

void CX2InstanceData::SetChoicedItem(IN bool bVal_ )
{
	m_bChoicedItem = bVal_;
}

bool CX2InstanceData::IsChoicedItem()
{
	return m_bChoicedItem;
}
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2