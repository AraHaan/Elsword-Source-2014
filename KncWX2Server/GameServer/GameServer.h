#pragma once
#include "BaseServer.h"

//korea nProtect ���
#pragma comment( lib, "./nProtect/ggsrvlib25.lib" )
#include "ggsrv25.h"

#ifdef SERV_NPROTECT_CS_AUTH_30
#pragma comment( lib, "./nProtect/ggsrv30lib.lib" )
#include "./nProtect/ggsrv30.h"
#endif SERV_NPROTECT_CS_AUTH_30

//korea HackShield ���
#pragma comment( lib, "./HackShield/AntiCpXSvr.lib" )
#include "HackShield/AntiCpXSvr.h"



class KGameServer : public KBaseServer
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclDump;
    DeclPfID;

public:
#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-07-02	// �ڼ���
	enum EXCHANGE_LIMIT_RESULT
	{
		ELR_NOT_INITED,
		ELR_EXHAUSTED,
		ELR_CHECK,
		ELR_PASS,
	};
#endif // SERV_ITEM_EXCHANGE_LIMIT

    KGameServer(void);
    virtual ~KGameServer(void);

    virtual bool Init();
    virtual void RegToLua();
    virtual void ShutDown();
    virtual bool DestroyAllSession();           // �������� ������ ����.

    virtual void ProcessEvent( const KEventPtr& spEvent );

    void Tick();

    //////////////////////////////////////////////////////////////////////////
	void SetHackShieldHSBFilePath( const char* pFilePath );

	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	void RequestRandomQuestInfo();
#endif SERV_RANDOM_DAY_QUEST
	//}}

	//{{ 2011. 12. 16	������	���� �ִ� �ο� �ǽð� ����
#ifdef SERV_REALTIME_MAX_USER
	int GetMaxUser() const;
#endif SERV_REALTIME_MAX_USER
	//}}

#ifdef SERV_KOG_HAVE_PCBANG_IP_LIST
	void SetPCBangIPList( const std::map< std::wstring, int >& mapPCBangIPList );
	int IsPCBangIP( const std::wstring& wstrIP );
#endif //SERV_KOG_HAVE_PCBANG_IP_LIST

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
	bool GetActiveLagCheck() { return m_bActiveLagCheck; }
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-28	// �ڼ���
	void	GetTimeEnchantEventUID( OUT int& iTimeEnchantEventUID ) const{	iTimeEnchantEventUID = m_iTimeEnchantEventUID;	}
	bool	PopFirstTimeEnchantEvent( void );
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-06-30	// �ڼ���
	void	GetExchangeLimitUID( OUT int& iExchangeLimitUID ) const{	iExchangeLimitUID = m_iExchangeLimitUID;	}
	int		DecreaseExchangeLimit( IN const int iDestItemID );
#endif // SERV_ITEM_EXCHANGE_LIMIT

protected:
	void ServerVersion();
    void WriteServerInfoToDB();
	virtual void OnServerReadyComplete();
	//{{ 2010. 10. 19	������	SMS�׽�Ʈ
#ifdef SERV_SMS_TEST
	void SMSTest_LUA();
#endif SERV_SMS_TEST
	//}}

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
	void InitTimeControlItemReleaseTick();
	bool TickCheckTimeControlItem();
	int GetTimeControlItemReleaseTick(TimeControl_Item_Release_Tick_Type _enum);
	void SetTimeControlItemReleaseTick(TimeControl_Item_Release_Tick_Type _enum ,std::map<int, int> &mapReleaseTick );
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
	void InitTimeControlCubeInItemMappingReleaseTick();
	bool TickCheckTimeControlCubeInItemMapping();
	int GetTimeControlCubeInItemMappingReleaseTick(TimeControl_CubeInItemMapping_Release_Tick_Type _enum);
	void SetTimeControlCubeInItemMappingReleaseTick(TimeControl_CubeInItemMapping_Release_Tick_Type _enum ,std::map<int, int> &mapReleaseTick );
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

    // packet send function
	template < class T > void SendToCnServer( unsigned short usEventID, const T& data );
	void SendToCnServer( unsigned short usEventID );

	template < class T > void SendToLoginServer( unsigned short usEventID, const T& data );
	void SendToLoginServer( unsigned short usEventID );

	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	template < class T > void SendToGlobalServer( unsigned short usEventID, const T& data );
	void SendToGlobalServer( unsigned short usEventID );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	template < class T > void SendToGameDB( unsigned short usEventID, const T& data );

	//{{ 2010. 02. 24  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT
	template < class T > void SendToNXWebDB( unsigned short usEventID, const T& data );
#endif SERV_WEB_POINT_EVENT
	//}}

#ifdef SERV_KOG_STATISTICS
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
#endif SERV_KOG_STATISTICS

#ifdef SERV_ENTRY_POINT
    template < class T > void SendToLogDB2nd( unsigned short usEventID, const T& data );
#endif SERV_ENTRY_POINT

	//{{ 2010. 10. 19	������	SMS�׽�Ʈ
#ifdef SERV_SMS_TEST
	template < class T > void SendToSMSDB( unsigned short usEventID, const T& data );
#endif SERV_SMS_TEST
	//}}
#ifdef SERV_CONTENT_MANAGER_INT
	template < class T > void SendToKOGBillingDB( unsigned short usEventID, const T& data );
#endif SERV_CONTENT_MANAGER_INT

#ifdef SERV_ADD_SCRIPT_DB
	template < class T > void SendToScriptDB( unsigned short usEventID, const T& data );
	void	SendToScriptDB( unsigned short usEventID ){ SendToScriptDB( usEventID, char() ); }
#endif //SERV_ADD_SCRIPT_DB

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-04	// �ڼ���
	void	CheckGSFieldBossSystemRequest( IN const CTime tCurrentTime );
	void	CheckGSFieldBossSystem( IN const CTime tCurrentTime );
#endif // SERV_BATTLE_FIELD_BOSS

   _DECL_ON_FUNC( DBE_SERVER_ON_ACK, KDBE_GAME_SERVER_ON_ACK );
   _DECL_ON_FUNC( DBE_UPDATE_SERVER_INFO_ACK, KServerList );
	//{{ 2009. 2. 13  ������	ä�θ���Ʈ
	DECL_ON_FUNC( DBE_CHANNEL_LIST_ACK );
	//}}
    DECL_ON_FUNC( ERM_ROOM_LIST_INFO_NOT );
	DECL_ON_FUNC( ERM_SQUARE_LIST_INFO_NOT );
	
   _DECL_ON_FUNC( ECN_NOTIFY_MSG_NOT, KEGS_NOTIFY_MSG_NOT );
	DECL_ON_FUNC( ENX_UPDATE_PRODUCT_LIST_NOT );
	DECL_ON_FUNC( DBE_EVENT_UPDATE_ACK );

	//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	DECL_ON_FUNC( DBE_RELEASE_TICK_UPDATE_ACK );
	DECL_ON_FUNC( DBE_EVENT_DATA_UPDATE_ACK );
	DECL_ON_FUNC( DBE_EVENT_MONSTER_UPDATE_ACK );
	DECL_ON_FUNC( DBE_EVENT_NOTIFY_MSG_SERVER_INFO_UPDATE_ACK );
	DECL_ON_FUNC( DBE_EVENT_REWARD_DATA_UPDATE_ACK );
	//{{ 2011. 08. 29	������	ĳ���� ������ �̺�Ʈ
#ifdef SERV_CHAR_LEVEL_UP_EVENT
	DECL_ON_FUNC( DBE_EVENT_REWARD_LEVEL_UP_UPDATE_ACK );
#endif SERV_CHAR_LEVEL_UP_EVENT
	//}}
	//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL
	DECL_ON_FUNC( DBE_EVENT_REWARD_RETAIN_USER_ITEM_UPDATE_ACK );
#endif SERV_NEXON_SESSION_PROTOCOL
	//}}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}

	//{{ 2011. 11. 3	������	��ϸ� �ð� ��ŷ ���� ������ ��ġ
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
#else
	//DECL_ON_FUNC( ELG_RANKING_TITLE_REWARD_NOT );
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
	//}}	
	DECL_ON_FUNC( DBE_INSERT_TITLE_ACK );
	DECL_ON_FUNC( DBE_INSERT_REWARD_TO_POST_ACK );
	//{{ 2008. 10. 31  ������	����� ����Ʈ
	DECL_ON_FUNC( DBE_ABUSER_LIST_ACK );
	//}}
	//{{ 2008. 11. 5  ������	���� ����
	DECL_ON_FUNC( ERM_UPDATE_FIELD_INFO_NOT );
	//}}
	//{{ 2009. 4. 22  ������	������������Ʈ
	DECL_ON_FUNC( ECN_NEW_PVP_USER_INFO_NOT );
	DECL_ON_FUNC( ECN_UPDATE_PVP_USER_INFO_NOT );
   _DECL_ON_FUNC( ECN_CHECK_INVALID_PVP_USER_NOT, UidType );
	//}}

	//{{ 2009. 6. 1  ������		ä���̵�
	DECL_ON_FUNC( ECN_CHECK_CHANNEL_CHANGE_REQ );
	//}}

	//{{ 2009. 6. 3  ������		Ȯ����
   _DECL_ON_FUNC( ECN_MEGAPHONE_MESSAGE_NOT, KEGS_MEGAPHONE_MESSAGE_NOT );
	//}}

	//{{ 2009. 7. 6  ������		��ŷ ����
	DECL_ON_FUNC( ELG_GET_RANKING_FOR_GS_RANKING_MANAGER_ACK );
	DECL_ON_FUNC( ELG_HENIR_RANKING_REFRESH_NOT );
	DECL_ON_FUNC( ELG_WEB_RANKING_REFRESH_NOT );
	//}}

	//{{ 2009. 7. 10  ������	��� �ý���
	DECL_ON_FUNC( ERM_INCREASE_PARTY_FEVER_NOT );
	//}}
	//{{ 2012. 05. 10	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	DECL_ON_FUNC( ERM_MAX_PARTY_FEVER_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2009. 10. 30  ������	��巹��
#ifdef GUILD_TEST
	DECL_ON_FUNC( DBE_JOIN_GUILD_ACK );

	DECL_ON_FUNC( ELG_KICK_RESULT_LETTER_NOT );

	DECL_ON_FUNC( DBE_EXPAND_GUILD_MAX_MEMBER_ACK );
	
	DECL_ON_FUNC( DBE_UPDATE_GUILD_EXP_ACK );

   _DECL_ON_FUNC( ELG_GUILD_LEVEL_UP_NOTIFY_NOT, KEGS_NOTIFY_MSG_NOT );
#endif GUILD_TEST
	//}}

	//{{ 2010. 02. 01  ������	��� �Խ���
#ifdef SERV_GUILD_AD
   _DECL_ON_FUNC( ELG_SEND_LETTER_JOIN_GUILD_NOT, KDBE_ACCEPT_JOIN_GUILD_ACK );
#endif SERV_GUILD_AD
	//}}

	//{{ 2010. 02. 16  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST
	DECL_ON_FUNC( DBE_CHECK_HACKING_TOOL_LIST_ACK );
#endif SERV_HACKING_TOOL_LIST
	//}}

	//{{ 2010. 02. 17  ������	ä����Ƽ����
#ifdef SERV_CHANNEL_PARTY	
	DECL_ON_FUNC( EPM_OPEN_PARTY_LIST_NOT );
	DECL_ON_FUNC( EPM_CLOSE_PARTY_LIST_NOT );
	DECL_ON_FUNC( EPM_ADD_PARTY_LIST_NOT );
	DECL_ON_FUNC( EPM_DEL_PARTY_LIST_NOT );	
	DECL_ON_FUNC( EPM_UPDATE_PARTY_IN_PARTY_LIST_NOT );
	DECL_ON_FUNC( EPM_ADD_PARTY_MEMBER_IN_PARTY_LIST_NOT );
	DECL_ON_FUNC( EPM_DEL_PARTY_MEMBER_IN_PARTY_LIST_NOT );
	DECL_ON_FUNC( EPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT );
#endif SERV_CHANNEL_PARTY
	//}}
	//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	DECL_ON_FUNC( EPM_UPDATE_PARTY_FEVER_IN_PARTY_LIST_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2010. 02. 23  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT
	DECL_ON_FUNC( DBE_INCREASE_WEB_POINT_ACK );
   _DECL_ON_FUNC( ELG_WEB_POINT_REWARD_INSERT_TO_POST_NOT, KDBE_GET_WEB_POINT_REWARD_ACK );
#endif SERV_WEB_POINT_EVENT
	//}}

#ifdef DEF_TRADE_BOARD
    DECL_ON_FUNC( ERM_PERSONAL_SHOP_INFO_NOT );
#endif DEF_TRADE_BOARD

	//{{ 2010. 06. 15  ������	�ǽð� ��ũ��Ʈ ����
#ifdef SERV_REALTIME_SCRIPT
   _DECL_ON_FUNC( ESR_ORDER_TO_GAME_SERVER_NOT, KESR_SCRIPT_REFRESH_ORDER_NOT );
   _DECL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT );
#endif SERV_REALTIME_SCRIPT
	//}}

#ifdef SERV_KOG_STATISTICS
   DECL_ON_FUNC( DBE_CCU_AGENT_ACK );
#endif SERV_KOG_STATISTICS
   
	//{{ 2010. 10. 12	������	���� ����͸�
#ifdef SERV_MORNITORING
	DECL_ON_FUNC( E_DISCONNECT_SERVER_REPORT_NOT );
   _DECL_ON_FUNC( DBE_SEND_PHONE_MSG_FOR_MORNITORING_ACK, KDBE_SEND_PHONE_MSG_NOT );
#endif SERV_MORNITORING
	//}}

   //{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	_DECL_ON_FUNC( EGB_UPDATE_MISSION_INFO_NOT, KEGB_GET_MISSION_INFO_ACK );
	DECL_ON_FUNC( EGB_GET_MISSION_INFO_ACK );
	DECL_ON_FUNC( EGB_UPDATE_WORLD_BUFF_NOT );
	DECL_ON_FUNC( EGB_UPDATE_MISSION_START_TIME_NOT );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	DECL_ON_FUNC( EGB_TODAY_RANDOM_QUEST_ACK );
#endif SERV_RANDOM_DAY_QUEST
	//}}
	//{{ 2011. 09. 29  ��μ�	�˴ٿ��� ����
#ifdef SERV_SHUTDOWN_SYSTEM
	_DECL_ON_FUNC( EGB_SHUT_DOWN_NOTIFY_NOT, KEGS_NOTIFY_MSG_NOT );
#endif SERV_SHUTDOWN_SYSTEM
	//}}

	//{{ 2012. 03. 23	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	DECL_ON_FUNC( EGB_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_NOT );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	//{{ 2012. 09. 02	�ڼ���	Merge ( �������� Ŭ���̾�Ʈ ���۵����� UDP ��Ŷ�� ���ƿ��� �ʴ� ������ �߰ߵǸ� �������� ű�Ѵ�. // 2012.06.11 lygan_������ )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	_DECL_ON_FUNC( ECN_UDP_KICK_GAMEEDIT_NOT, KEGS_UDP_KICK_GAMEEDIT_NOT );
#endif UDP_CAN_NOT_SEND_USER_KICK
	//}}

	//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
	DECL_ON_FUNC_NOPARAM( EGB_EARTH_QUAKE_NOT );
#endif SERV_EARTH_QUAKE_EFFECT
	//}}
	//{{ 2011. 11. 2	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	DECL_ON_FUNC( ERM_BATTLE_FIELD_ROOM_INFO_NOT );
	DECL_ON_FUNC( EGB_GET_AUTO_PARTY_BONUS_INFO_ACK );
	DECL_ON_FUNC( EGB_UPDATE_AUTO_PARTY_BONUS_INFO_NOT );
	DECL_ON_FUNC( ERM_END_GAME_REGROUP_PARTY_NOT );
	DECL_ON_FUNC( ERM_COMPLETE_RETURN_TO_FIELD_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 10. 16	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	DECL_ON_FUNC( DBE_GET_ED_AND_ITEM_MORNITOING_INFO_ACK );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}
	//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
	_DECL_ON_FUNC( EGB_JACKPOT_EVENT_NOTIFY_NOT, KEGS_NOTIFY_MSG_NOT );
	_DECL_ON_FUNC( EGB_JACKPOT_EVENT_POST_NOT, KDBE_INSERT_REWARD_TO_POST_REQ );
#endif SERV_EVENT_JACKPOT
	//}}
	
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	DECL_ON_FUNC( DBE_CHECK_EVENT_UPDATE_ACK );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK // 2012.12.11 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
	DECL_ON_FUNC( DBE_CHECK_EVENT_UPDATE_ACK );
#endif //SERV_REFRESH_EVENT_USING_RELEASE_TICK	
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
	DECL_ON_FUNC( ECN_SET_ACTIVE_LAGCHECK_NOT );
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	_DECL_ON_FUNC( ELG_WEDDING_HALL_INFO_UPDATE_NOT, KDBE_LOAD_WEDDING_HALL_INFO_ACK );
	DECL_ON_FUNC( ERM_WEDDING_HALL_INFO_NOT );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-28	// �ڼ���
	DECL_ON_FUNC( EGB_TIME_ENCHANT_EVENT_INFO_NOT );
	DECL_ON_FUNC( EGB_TIME_ENCHANT_EVENT_INFO_ACK );
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-06-30	// �ڼ���
	DECL_ON_FUNC( EGB_EXCHANGE_LIMIT_INFO_NOT );
	DECL_ON_FUNC( EGB_EXCHANGE_LIMIT_INFO_ACK );
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_CONTENT_MANAGER_INT
	DECL_ON_FUNC( DBE_GET_CASHSHOP_ON_OFF_INFO_ACK );
#endif SERV_CONTENT_MANAGER_INT

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	DECL_ON_FUNC( DBE_EVENT_DB_SCRIPT_ACK );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
	DECL_ON_FUNC( DBE_GET_ITEM_ONOFF_NPCSHOP_ACK );
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
	DECL_ON_FUNC( DBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_ACK );
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

	//{{ 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���
#ifdef SERV_GLOBAL_MISSION_MANAGER
	_DECL_ON_FUNC( EGB_UPDATE_GLOBAL_MISSION_INFO_NOT, KEGB_GET_GLOBAL_MISSION_INFO_ACK );
	DECL_ON_FUNC( EGB_GET_GLOBAL_MISSION_INFO_ACK );
	DECL_ON_FUNC( EGB_UPDATE_GLOBAL_MISSION_START_TIME_NOT );
#endif SERV_GLOBAL_MISSION_MANAGER
	//}} 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-29	// �ڼ���
	DECL_ON_FUNC( ERM_UPDATE_TOTAL_DANGEROUS_VALUE_NOT );
	DECL_ON_FUNC_NOPARAM( EGB_VERIFY_SERVER_CONNECT_NOT );
	DECL_ON_FUNC_NOPARAM( EGB_VERIFY_SERVER_DISCONNECT_NOT );
	DECL_ON_FUNC( EGB_BATTLE_FIELD_BOSS_INFO_NOT );
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_ENTRY_POINT
    _DECL_ON_FUNC( DBE_CHANNEL_LIST_2ND_ACK, KDBE_CHANNEL_LIST_ACK );
#endif SERV_ENTRY_POINT

#ifdef SERV_MODFIY_FLAG_REALTIME_PATCH
	_DECL_ON_FUNC( EGS_ADD_COMMON_FLAG_NOT, KEGS_ADD_COMMON_FLAG_NOT );
	_DECL_ON_FUNC( EGS_DEL_COMMON_FLAG_NOT, KEGS_DEL_COMMON_FLAG_NOT );
#endif // SERV_MODFIY_FLAG_REALTIME_PATCH

#ifdef SERV_STRING_FILTER_USING_DB
	DECL_ON_FUNC( DBE_CHECK_STRING_FILTER_UPDATE_ACK );
	DECL_ON_FUNC( DBE_STRING_FILTER_UPDATE_ACK );
#endif //SERV_STRING_FILTER_USING_DB

protected:

#ifdef SERV_KOG_HAVE_PCBANG_IP_LIST
	std::map< std::wstring, int >		m_mapPCBangIPList;
#endif //SERV_KOG_HAVE_PCBANG_IP_LIST

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	boost::timer					m_tTimeProcessCommunicationONOFF;
	bool							m_bServerRunningProcessCommunicationOnOff;
#endif //SERV_PROCESS_COMMUNICATION_KSMS

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
	bool m_bActiveLagCheck;
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_������ // ������ �۾� ������ ( DB���� �ǽð� �� �ݿ�, ��ȯ, ���� �ʵ� ���� )
	boost::timer					m_tTimeControlItemCheckTimer; // 2012.12.11 lygan_������ // ��� ��� Ư���ð����� �Ǹ� �ϴ� ������ ����Ʈ�� ���� ������ ƽ ī��Ʈ üũ��
	std::map<int, int>				m_mapTimeControlItem_StaticDBReleaseTick;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
	boost::timer					m_tTimeControlCubeInItemMappingCheckTimer; // 2014.01.10 86red_�輮�� // ��� ��� ���������� ���� ������ ƽ ī��Ʈ üũ��
	std::map<int, int>				m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick;
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

private:
#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-28	// �ڼ���
	boost::timer	m_kTimeEnchantEventTimer;
	int				m_iTimeEnchantEventUID;
	int				m_iNumOfTimeEnchantEvent;
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-06-30	// �ڼ���
	int						m_iExchangeLimitUID;
	std::map< int, int >	m_mapExchangeLimitInfo;	// <m_iDestItemID, m_iQuantity>
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_CONTENT_MANAGER_INT
	boost::timer			m_tCashshopOnOffCheckTimer;
	bool					m_bFirstCashshopOnOffCheck;
#endif SERV_CONTENT_MANAGER_INT
};

DefKObjectInline( KGameServer, KBaseServer );

template < class T >
void KGameServer::SendToCnServer( unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_SERVER, GetUID(), PI_CN_SERVER, 0, NULL, usEventID, data );
}

template < class T >
void KGameServer::SendToLoginServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_SERVER, GetUID(), PI_LOGIN_USER, 0, NULL, usEventID, data );
}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
template < class T >
void KGameServer::SendToGlobalServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_SERVER, GetUID(), PI_GLOBAL_USER, 0, NULL, usEventID, data );
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�


template < class T >
void KGameServer::SendToGameDB( unsigned short usEventID, const T& data )
{
	KncSend( GetPfID(), GetUID(), PI_GS_GAME_DB, 0, NULL, usEventID, data );
}

//{{ 2010. 02. 24  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT

template < class T >
void KGameServer::SendToNXWebDB( unsigned short usEventID, const T& data )
{
	KncSend( GetPfID(), GetUID(), PI_GS_NX_WEB_DB, 0, NULL, usEventID, data );
}

#endif SERV_WEB_POINT_EVENT
//}}

#ifdef SERV_KOG_STATISTICS
template < class T >
void KGameServer::SendToLogDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( GetPfID(), GetUID(), PI_LOG_DB, 0, anTrace, usEventID, data );
}
#endif SERV_KOG_STATISTICS

#ifdef SERV_ENTRY_POINT
template < class T >
void KGameServer::SendToLogDB2nd( unsigned short usEventID, const T& data )
{
    UidType anTrace[2] = { GetUID(), -1 };
    KncSend( GetPfID(), GetUID(), PI_GS_LOG_DB_2ND, 0, anTrace, usEventID, data );
}
#endif SERV_ENTRY_POINT

//{{ 2010. 10. 19	������	SMS�׽�Ʈ
#ifdef SERV_SMS_TEST
template < class T >
void KGameServer::SendToSMSDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( GetPfID(), GetUID(), PI_GS_SMS_DB, 0, anTrace, usEventID, data );
}
#endif SERV_SMS_TEST
//}}

#ifdef SERV_CONTENT_MANAGER_INT
template < class T >
void KGameServer::SendToKOGBillingDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_GS_SERVER, GetUID(), PI_GS_KOG_BILLING_DB, 0, anTrace, usEventID, data );
}
#endif SERV_CONTENT_MANAGER_INT

#ifdef SERV_ADD_SCRIPT_DB
template < class T >
void KGameServer::SendToScriptDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( GetPfID(), GetUID(), PI_SCRIPT_DB, 0, anTrace, usEventID, data );
}
#endif //SERV_ADD_SCRIPT_DB