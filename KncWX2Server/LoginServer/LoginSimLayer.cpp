#include "LoginSimLayer.h"
#include <iomanip>  // std::setiosflags, std::resetiosflags
#include "ActorManager.h"
#include "DBLayer.h"
#include "LoginServer.h"
#include "KAutoPath.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

//{{ 2011. 04. 29	������	�븮����
#ifdef SERV_PSHOP_AGENCY
	#include "LoginRoomManager.h"
#endif SERV_PSHOP_AGENCY
//}}

#include "LoginUser.h"
//{{ 2010. 06. 08  ������	�ؽ�PC�� ���� ���� ����
#ifdef SERV_PCBANG_AUTH_NEW
	#include "NexonAuthManager.h"
#else
	#include "NexonBillingAuthManager.h"
#endif SERV_PCBANG_AUTH_NEW
//}}

#ifdef SERV_GLOBAL_AUTH
#if defined (SERV_COUNTRY_TWHK)
#include "../Common/OnlyGlobal/AuthAndBilling/TW/GASHAuthManager.h"
#elif defined (SERV_COUNTRY_JP)
#include "../Common/OnlyGlobal/AuthAndBilling/JP/LoginNetCafeManager.h"
#endif // SERV_COUNTRY_XX
#endif // SERV_GLOBAL_AUTH

#ifdef SERV_GLOBAL_BILLING
#if defined (SERV_COUNTRY_TH)
#include "../Common/OnlyGlobal/AuthAndBilling/TH/AsiaSoftAuthManager.h"
#elif defined (SERV_COUNTRY_CN)
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantAuthManager.h"
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantRoleRegManager.h"
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantInfoManager.h"
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantBillingManager.h"
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantCouponManager.h"
#endif //SERV_COUNTRY_XX
#endif // SERV_GLOBAL_BILLING

//{{ 2008. 7. 17  ������  �����̵�
#include "RankingManager.h"
//}}
//{{ 2009. 9. 22  ������	���
#include "GuildManager.h"
//}}
//{{ 2009. 10. 27  ������	��巹��
#ifdef GUILD_TEST
	#include "X2Data/XSLGuildManager.h"
#endif GUILD_TEST
//}}
//{{ 2009. 11. 30  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	#include "X2Data/XSLSkillTree.h"
#endif GUILD_SKILL_TEST
//}}
//{{ 2010. 02. 02  ������	��� �Խ���
#ifdef SERV_GUILD_AD
	#include "GuildBoardManager.h"
#endif SERV_GUILD_AD
//}}
//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}
//{{ 2011. 11. 3	������	��ϸ� �ð� ��ŷ ���� ������ ��ġ
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
	#include "X2Data/XSLTitleManager.h"
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
//}}

//{{ 2010. 10. 13	������	DLL Manager
#ifdef SERV_DLL_MANAGER
	#include "DLLManager.h"
#endif SERV_DLL_MANAGER
//}}
//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	#include "GameSysVal/GameSysVal.h"
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
//}}
//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	#include "BuffManager.h"
#endif SERV_SERVER_BUFF_SYSTEM
//}

//{{ 2012. 11. 22	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}


#include "DBLayer.h"
#include "NetError.h"

#include <lua_tinker.h>
using namespace lua_tinker;

#include <boost/bind.hpp>

NiImplementRTTI( KLoginSimLayer, KSimLayer );

KLoginSimLayer::KLoginSimLayer(void) 
{
#ifdef SERV_GLOBAL_AUTH
	m_bPublisherCheckGameServerLogin = false;
#endif //SERV_GLOBAL_AUTH
#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
	SetEnableCNDirectCharge_LUA(true);
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH
}

KLoginSimLayer::~KLoginSimLayer(void)
{
}

ImplToStringW( KLoginSimLayer )
{
	//{{ 2011. 08. 11	������	���� ���� ä�θ� 
#ifdef SERV_TOONILAND_CHANNELING
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_LOGIN_USER, NULL, ELG_DUMP_LOGIN_USER_NOT, char() );
	KActorManager::GetKObj()->QueueingToAll( spEvent );
#endif SERV_TOONILAND_CHANNELING
	//}}
	
    return START_TOSTRING_PARENTW( KSimLayer );
}

ImplOstmOperatorW2A( KLoginSimLayer );

void KLoginSimLayer::RegToLua()
{
    KSimLayer::RegToLua();

    //class_<KLoginSimLayer>( "KLoginSimLayer" )
    //    .inh<KSimLayer>()
    //    .def( "dump", KLoginSimLayer::Dump );

    //decl( "SimLayer", this );
	lua_tinker::class_add<KLoginSimLayer>( g_pLua, "KLoginSimLayer" );
	lua_tinker::class_inh<KLoginSimLayer, KSimLayer>( g_pLua );
	//{{ 2012. 09. 03	������		�ߺ� ���� ���� ����
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
	lua_tinker::class_def<KLoginSimLayer>( g_pLua, "SetEnableAuthWait",	&KLoginSimLayer::SetEnableAuthWait_LUA );
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
	//}}
#ifdef SERV_GLOBAL_AUTH
	lua_tinker::class_def<KLoginSimLayer>( g_pLua, "SetPublisherCheckGameServerLogin",	&KLoginSimLayer::SetPublisherCheckGameServerLogin_LUA );
#endif //SERV_GLOBAL_AUTH
	lua_tinker::class_def<KLoginSimLayer>( g_pLua, "dump",				&KLoginSimLayer::Dump );

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
	lua_tinker::class_def<KLoginSimLayer>( g_pLua, "SetEnableCNDirectCharge", &KLoginSimLayer::SetEnableCNDirectCharge_LUA );
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

	lua_tinker::decl( g_pLua, "SimLayer", this );

	//{{ 2011. 04. 29	������	�븮����
#ifdef SERV_PSHOP_AGENCY
	GetKLoginRoomManager()->RegToLua();
#endif SERV_PSHOP_AGENCY
	//}}
	
	//{{ 2010. 06. 08  ������	�ؽ�PC�� ���� ���� ����
#ifdef SERV_PCBANG_AUTH_NEW
	SiKNexonAuthManager()->RegToLua();
#else
	SiKNexonBillingAuthManager()->RegToLua();
#endif SERV_PCBANG_AUTH_NEW
	//}}	

#ifdef SERV_GLOBAL_AUTH
#if defined (SERV_COUNTRY_TWHK)
    SiKGASHAuthManager()->RegToLua();
#elif defined (SERV_COUNTRY_JP)
	SiKLoginNetCafeManager()->RegToLua();
#endif // SERV_COUNTRY_XX
#endif // SERV_GLOBAL_AUTH

#ifdef SERV_GLOBAL_BILLING
#if defined (SERV_COUNTRY_TH)
	SiKAsiaSoftAuthManager()->RegToLua();
#elif defined (SERV_COUNTRY_CN)
	KGiantBaseManager::RegToLuaBase();
	SiKGiantAuthManager()->RegToLua();
	SiKGiantInfoManager()->RegToLua();
	SiKGiantRoleRegManager()->RegToLua();
	SiKGiantBillingManager()->RegToLua();
	SiKGiantCouponManager()->RegToLua();
#endif SERV_COUNTRY_XX
#endif // SERV_GLOBAL_BILLING
}

//{{ 2012. 09. 03	������		�ߺ� ���� ���� ����
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
void KLoginSimLayer::SetEnableAuthWait_LUA( bool bEnable )
{
	START_LOG( cout, L"PC�� ���� ��� ��� ���� ���� : " << bEnable );

	m_kUserList.SetEnableAuthWait( bEnable );
}
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
//}}

bool KLoginSimLayer::GetGSUserInfoByUserUID( UidType iUserUID, KUserList::KGSUserInfo& kInfo )
{
    return m_kUserList.GetGSUserInfoByUserUID( iUserUID, kInfo );
}

bool KLoginSimLayer::GetGSUserInfoByUserID( const std::wstring& wstrUserID, KUserList::KGSUserInfo& kInfo )
{
    return m_kUserList.GetGSUserInfoByUserID( wstrUserID, kInfo );
}

bool KLoginSimLayer::GetGSUserInfoByUnitUID( UidType iUnitUID, KUserList::KGSUserInfo& kInfo )
{
    return m_kUserList.GetGSUserInfoByUnitUID( iUnitUID, kInfo );
}

bool KLoginSimLayer::GetGSUserInfoByUnitNickName( std::wstring& wstrUnitNickName, KUserList::KGSUserInfo& kInfo )
{
    return m_kUserList.GetGSUserInfoByUnitNickName( wstrUnitNickName, kInfo );
}
#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
bool KLoginSimLayer::GetGSUserInfoByPublisherUID( unsigned int uiPublisherUID, KUserList::KGSUserInfo& kInfo )
{
	return m_kUserList.GetGSUserInfoByPublisherUID( uiPublisherUID, kInfo );
}
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID

//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
bool KLoginSimLayer::GetGSUserInfoByMachineID( IN const std::string& strMachineID, KUserList::KGSUserInfo& kInfo )
{
    return m_kUserList.GetGSUserInfoByMachineID( strMachineID, kInfo );
}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
//}}

//{{ 2010. 06. 08  ������	�ؽ�PC�� ���� ���� ����
#ifdef SERV_PCBANG_AUTH_NEW

void KLoginSimLayer::CheckAuthSynchronize( const KENX_AUTH_SYNCHRONIZE_REQ& kInfo )
{
    m_kUserList.CheckAuthSynchronize( kInfo );
}

bool KLoginSimLayer::UpdateSessionNoByUserID( const std::wstring& wstrUserID, __int64 iSessionNo )
{
	return m_kUserList.UpdateSessionNoByUserID( wstrUserID, iSessionNo );
}

void KLoginSimLayer::LoginTimeOut( const KELG_NEXON_USER_LOGIN_TIME_OUT_NOT& kInfo )
{
    m_kUserList.LoginTimeOut( kInfo );
}

#endif SERV_PCBANG_AUTH_NEW
//}}

#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
void KLoginSimLayer::LogoutPcBang( const KELG_PCBANG_LOGOUT_NOT& kInfo )
{
	m_kUserList.LogoutPCBang( kInfo );
}
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER

int KLoginSimLayer::RegUser( UidType iUserUID, UidType iGSUID, const KELG_REGISTER_USER_REQ& kInfo_, bool bChannelChanged )
{
    return m_kUserList.RegUser( iUserUID, iGSUID, kInfo_, bChannelChanged );
}

void KLoginSimLayer::UnRegUser( UidType iUserUID )
{
    m_kUserList.UnRegUser( iUserUID );
}

void KLoginSimLayer::UnRegAllUserOfThisGS( UidType iGSUID )
{
    m_kUserList.UnRegAllUserOfThisGS( iGSUID );
}

int KLoginSimLayer::RegUnit( UidType iUserUID, UidType iUnitUID, std::wstring& wstrUnitNickName, u_int& uiKNMSerialNum, bool bDenyFriendShip )
{
    return m_kUserList.RegUnit( iUserUID, iUnitUID, wstrUnitNickName, uiKNMSerialNum, bDenyFriendShip );
}

void KLoginSimLayer::UnRegUnit( UidType iUserUID )
{
    m_kUserList.UnRegUnit( iUserUID );
}

void KLoginSimLayer::Init()
{
    switch( GetAuthFlag() )
    {
    case KSimLayer::AF_NEXON_KOREA:
		//{{ 2010. 06. 08  ������	�ؽ�PC�� ���� ���� ����
#ifdef SERV_PCBANG_AUTH_NEW
		m_vecpThreadMgr.push_back( SiKNexonAuthManager()->GetInstance() );
#else
		m_vecpThreadMgr.push_back( SiKNexonBillingAuthManager()->GetInstance() );
#endif SERV_PCBANG_AUTH_NEW
		//}}	    
        break;
    default:
        break;
    }

#ifdef SERV_GLOBAL_AUTH
#if defined (SERV_COUNTRY_TWHK)
	m_vecpThreadMgr.push_back( SiKGASHAuthManager()->GetInstance() );
#elif defined (SERV_COUNTRY_JP)
	m_vecpThreadMgr.push_back( SiKLoginNetCafeManager()->GetInstance() );
#elif defined (SERV_COUNTRY_CN)
	m_vecpThreadMgr.push_back( SiKGiantAuthManager()->GetInstance() );
	m_vecpThreadMgr.push_back( SiKGiantRoleRegManager()->GetInstance() );
	m_vecpThreadMgr.push_back( SiKGiantInfoManager()->GetInstance() );
#endif // SERV_COUNTRY_XX
#endif // SERV_GLOBAL_AUTH

#ifdef SERV_GLOBAL_BILLING
#if defined (SERV_COUNTRY_TH)
	m_vecpThreadMgr.push_back( SiKAsiaSoftAuthManager()->GetInstance() );
#elif defined (SERV_COUNTRY_CN)
	m_vecpThreadMgr.push_back( SiKGiantBillingManager()->GetInstance() );
	m_vecpThreadMgr.push_back( SiKGiantCouponManager()->GetInstance() );
#endif // SERV_COUNTRY_XX
#endif // SERV_GLOBAL_BILLING

    KSimLayer::Init();

	KAutoPath kAutoPath;
	std::string		strFile;

	{
		LoadingTimer lt( L"Enum.lua" );

		//���� �鿡�� �̷�� ���� ������ �ε��� �����Ұ�� ������ ���� ���Ѿ��Ѵ�.
		strFile = "Enum.lua";
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( g_pLua, strFile.c_str() ) )
		{
			START_LOG( cerr, L"Enum ���� �ε� ����.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );

			//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"Enum.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"Enum ���� �ε� ����.!" );
		}
	}

	{
		LoadingTimer lt( L"DungeonEnum.lua" );

		//���� �鿡�� �̷�� ���� ������ �ε��� �����Ұ�� ������ ���� ���Ѿ��Ѵ�.
		strFile = "DungeonEnum.lua";
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( g_pLua, strFile.c_str() ) )
		{
			START_LOG( cerr, L"DungeonEnum ���� �ε� ����.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );

			//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
			KBaseServer::GetKObj()->AddFailScriptFileName( L"DungeonEnum.lua" );
			//}}
		}
		else
		{
			START_LOG( cout, L"DungeonEnum ���� �ε� ����.!" );
		}
	}

	//{{ 2009. 7. 6  ������		��ŷ����
	{
		LoadingTimer lt(L"RankingData.lua");
		
		strFile = "RankingData.lua";
		kAutoPath.GetPullPath( strFile );
		if( SiKRankingManager()->OpenScriptFile( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"Ranking Data ���� �ε� ����.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );

			//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"RankingData.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"Ranking Data ���� �ε� ����.!" );
		}
	}
	//}}

	//{{ 2009. 10. 27  ������	��巹��
#ifdef GUILD_TEST
	{
		CXSLGuildManager::RegScriptName( "GuildData.lua" );
		OPEN_SCRIPT_FILE( CXSLGuildManager );
	}
#endif GUILD_TEST
	//}}

	//{{ 2009. 11. 30  ������	��彺ų
#ifdef GUILD_SKILL_TEST
#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// �۾���¥: 2013-08-12	// �ڼ���
	CXSLSkillTree::RegScriptName( "NewSkillTempletVer2.lua" );
	OPEN_SCRIPT_FILE( CXSLSkillTree );
#else // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
	{
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
		LoadingTimer lt( L"NewSkillTempletVer2.lua" );

		strFile = "NewSkillTempletVer2.lua";
		kAutoPath.GetPullPath( strFile );
		if( SiCXSLSkillTree()->OpenScriptFile( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"SKILL TEMPLET ���� �ε� ����.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
			KBaseServer::GetKObj()->AddFailScriptFileName( L"NewSkillTempletVer2.lua" );

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
		LoadingTimer lt( L"NewSkillTemplet.lua" );

		strFile = "NewSkillTemplet.lua";
		kAutoPath.GetPullPath( strFile );
		if( SiCXSLSkillTree()->OpenScriptFile( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"SKILL TEMPLET ���� �ε� ����.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	������	��ũ��Ʈ �Ľ� ���� ����Ʈ
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"NewSkillTemplet.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
		}
		else
		{
			START_LOG( cout, L"SKILL TEMPLET ���� �ε� ����.!" );
		}
	}
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
#endif GUILD_SKILL_TEST
	//}}

	//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	{
		KMornitoringManager::RegScriptName( "ServerMornitoringConfig.lua" );
		OPEN_SCRIPT_FILE( KMornitoringManager );
	}
#endif SERV_MORNITORING
	//}}

	//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	{
		KGameSysVal::RegScriptName( "GameSysValTable.lua" );
		OPEN_SCRIPT_FILE( KGameSysVal );
	}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}	

	//{{ 2011. 11. 3	������	��ϸ� �ð� ��ŷ ���� ������ ��ġ
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
	{
		CXSLTitleManager::RegScriptName( "TitleTable.lua" );
		CXSLTitleManager::RegScriptName( "TitleMission.lua" );
		CXSLTitleManager::RegScriptName( "SubTitleMission.lua" );
#ifdef SEPARATION_SUB_TITLE_SCRIPT
		CXSLTitleManager::RegScriptName( "SubTitleOpenMission.lua" );
#endif // SEPARATION_SUB_TITLE_SCRIPT
		OPEN_SCRIPT_FILE( CXSLTitleManager );
	}
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
	//}}

	//{{ 2012. 11. 22	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	{
		KSMSPhoneNumberManager::RegScriptName( "SMSPhoneNumberList.lua" );
		OPEN_SCRIPT_FILE( KSMSPhoneNumberManager );
	}
#endif SERV_SMS_TOTAL_MANAGER
	//}}

	//{{ 2009. 9. 22  ������	���
#ifdef GUILD_TEST
	SiKGuildManager(); // �̱��� ����
#endif GUILD_TEST
	//}}

	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	SiKBuffManager(); // �̱��� ����
#endif SERV_SERVER_BUFF_SYSTEM
	//}
}

void KLoginSimLayer::Tick()
{
    KSimLayer::Tick();

    switch( GetAuthFlag() )
    {
    case KSimLayer::AF_NEXON_KOREA:
		//{{ 2010. 06. 08  ������	�ؽ�PC�� ���� ���� ����
#ifdef SERV_PCBANG_AUTH_NEW
		SiKNexonAuthManager()->Tick();
#else
		SiKNexonBillingAuthManager()->Tick();
#endif SERV_PCBANG_AUTH_NEW
		//}}		
        break;
    default:
        break;
    }

#ifdef SERV_GLOBAL_AUTH
#if defined (SERV_COUNTRY_TWHK)
	if (GetAuthFlag() == KSimLayer::AF_GLOBAL_SERVICE)
		SiKGASHAuthManager()->Tick();
#elif defined (SERV_COUNTRY_JP)
	if (GetAuthFlag() == KSimLayer::AF_GLOBAL_SERVICE)
		SiKLoginNetCafeManager()->Tick();
#elif defined (SERV_COUNTRY_CN)
	if (GetAuthFlag() == KSimLayer::AF_GLOBAL_SERVICE)
	{
		SiKGiantAuthManager()->Tick();
		SiKGiantRoleRegManager()->Tick();
		SiKGiantInfoManager()->Tick();
	}
#endif // SERV_COUNTRY_XX
#endif // SERV_GLOBAL_AUTH

#ifdef SERV_GLOBAL_BILLING
#if defined (SERV_COUNTRY_TH)
	if (GetBillingFlag() == KSimLayer::BF_GLOBAL_SERVICE)
		SiKAsiaSoftAuthManager()->Tick();
#elif defined (SERV_COUNTRY_CN)
	if (GetBillingFlag() == KSimLayer::BF_GLOBAL_SERVICE)
	{
		SiKGiantBillingManager()->Tick();
		SiKGiantCouponManager()->Tick();
	}
#endif // SERV_COUNTRY_XX
#endif // SERV_GLOBAL_BILLING


	//{{ 2009. 7. 6  ������		��ŷ ����
	SiKRankingManager()->Tick();
	//}}

	//{{ 2009. 11. 26  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	SiKGuildManager()->Tick();
#endif GUILD_SKILL_TEST
	//}}

	//{{ 2010. 02. 02  ������	��� �Խ���
#ifdef SERV_GUILD_AD
	SiKGuildBoardManager()->Tick();
#endif SERV_GUILD_AD
	//}}	

	//{{ 2010. 10. 13	������	DLL Manager
#ifdef SERV_DLL_MANAGER
	SiKDLLManager()->Tick();
#endif SERV_DLL_MANAGER
	//}}

	//{{ 2010. 02. 24  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT
	if( m_tWebPointEventTimer.elapsed() > 3600.0 )
	{
		m_tWebPointEventTimer.restart();

		KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_LOGIN_NX_WEB_DB, 0, NULL, DBE_GET_WEB_POINT_REWARD_REQ, char() );
	}
#endif SERV_WEB_POINT_EVENT
	//}}

	//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	if( GetKLoginServer()->IsConnectionMornitoringEnable() == true )
	{
		SiKMornitoringManager()->Tick_LoginSImLayer();
	}
#endif SERV_MORNITORING
	//}}

	//{{ 2012. 09. 03	������		�ߺ� ���� ���� ����
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
	m_kUserList.Tick();
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
	//}}
}

void KLoginSimLayer::ShutDown()
{
    KSimLayer::ShutDown();

    switch( GetAuthFlag() )
    {
    case KSimLayer::AF_NEXON_KOREA:
		//{{ 2010. 06. 08  ������	�ؽ�PC�� ���� ���� ����
#ifdef SERV_PCBANG_AUTH_NEW
		SiKNexonAuthManager()->ReleaseInstance();
#else
		SiKNexonBillingAuthManager()->ReleaseInstance();
#endif SERV_PCBANG_AUTH_NEW
		//}}        
        break;
    default:
        break;
    }

#ifdef SERV_GLOBAL_AUTH
#if defined (SERV_COUNTRY_TWHK)
	SiKGASHAuthManager()->ReleaseInstance(); 
#elif defined (SERV_COUNTRY_JP)
	SiKLoginNetCafeManager()->ReleaseInstance();
#elif defined (SERV_COUNTRY_CN)
	SiKGiantAuthManager()->ReleaseInstance();
	SiKGiantRoleRegManager()->ReleaseInstance();
	SiKGiantInfoManager()->ReleaseInstance();
#endif // SERV_COUNTRY_XX
#endif // SERV_GLOBAL_AUTH

#ifdef SERV_GLOBAL_BILLING
#if defined (SERV_COUNTRY_TH)
	SiKAsiaSoftAuthManager()->ReleaseInstance();
#elif defined (SERV_COUNTRY_CN)
	SiKGiantBillingManager()->ReleaseInstance();
	SiKGiantCouponManager()->ReleaseInstance();
#endif // SERV_COUNTRY_TH
#endif // SERV_GLOBAL_BILLING

	//{{ 2009. 7. 6  ������		��ŷ ����
	KRankingManager::ReleaseInstance();
	//}}
	//{{ 2009. 9. 22  ������	���
#ifdef GUILD_TEST
	KGuildManager::ReleaseInstance();
#endif GUILD_TEST
	//}}
	//{{ 2009. 10. 27  ������	��巹��
#ifdef GUILD_TEST
	CXSLGuildManager::ReleaseInstance();
#endif GUILD_TEST
	//}}
	//{{ 2009. 11. 30  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	CXSLSkillTree::ReleaseInstance();
#endif GUILD_SKILL_TEST
	//}}
	//{{ 2011. 11. 3	������	��ϸ� �ð� ��ŷ ���� ������ ��ġ
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
	CXSLTitleManager::ReleaseInstance();
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
	//}}
	//{{ 2012. 11. 22	�ڼ���	SMS ��ȭ��ȣ ���� ����
#ifdef SERV_SMS_TOTAL_MANAGER
	KSMSPhoneNumberManager::ReleaseInstance();
#endif SERV_SMS_TOTAL_MANAGER
	//}}
}

//{{ 2009. 4. 1  ������		ģ�� ����
bool KLoginSimLayer::SetDenyFriendShip( UidType iUnitUID, bool bDenyFriendShip )
{
	return m_kUserList.SetDenyFriendShip( iUnitUID, bDenyFriendShip );
}
//}}

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
void KLoginSimLayer::SetEnableCNDirectCharge_LUA(bool bValue)
{
	m_bEnableCNDirectCharge = bValue;

	START_LOG( cout, L"[CN] Direct Charge Enable Setting" )
		<< BUILD_LOG( m_bEnableCNDirectCharge );
}
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

