#pragma once

#include "RefreshSingleton.h"
#include "DBLayer.h"
#include <KncSend.h>
#include <boost/timer.hpp>
#include <set>
#include "CommonPacket.h"
//{{ 2011. 12. 14	������	SimLayer Tick�����ս� üũ
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	#include "TickDumper.h"
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
//}}

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	#include "EventDataRefresh/ReleaseTickTypeDefine.h"
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}

class KAbuserLogManager
{
	DeclareRefreshSingleton( KAbuserLogManager );
	DeclareLuaScriptParser;		
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;
	//{{ 2011. 12. 14	������	SimLayer Tick�����ս� üũ
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	DeclareTickDumper;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

public:
	KAbuserLogManager(void);
	~KAbuserLogManager(void);	

	//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	enum ABUSER_TYPE
	{
		AT_PVP_ABUSER			= RTT_ABUSER_TYPE_PVP,				// [0] ���� �����
		AT_ITEM_ABUSER			= RTT_ABUSER_TYPE_ITEM,				// [1] ������ ȹ�� �����
		AT_ENCHANT_ABUSER		= RTT_ABUSER_TYPE_ENCHANT,			// [2] ��ȭ �����
		AT_ED_ABUSER			= RTT_ABUSER_TYPE_ED,				// [3] EDȹ�� �����
		AT_ABUSER_MONITORING	= RTT_ABUSER_TYPE_MONITORING,		// [4] ����� �ǽ� ���� ��Ŷ ����͸�
		//{{ 2010. 9. 8	������	��Ʈ��ũ Ʈ���� ���� ���� ����͸�
#ifdef SERV_TRAFFIC_USER_MONITORING
		AT_TRAFFIC_ABUSER		= RTT_ABUSER_TYPE_TRAFFIC,			// [5] Ʈ���� �߻���Ű�� ���� ����
#endif SERV_TRAFFIC_USER_MONITORING
		//}}

		AT_MAX/*					= ( AT_TRAFFIC_ABUSER + 1 )*/
	};
#else
	enum ABUSER_TYPE
	{
		AT_PVP_ABUSER = 0,			// [0] ���� �����
		AT_ITEM_ABUSER,				// [1] ������ ȹ�� �����
		AT_ENCHANT_ABUSER,			// [2] ��ȭ �����
		AT_ED_ABUSER,				// [3] EDȹ�� �����
		AT_ABUSER_MONITORING,		// [4] ����� �ǽ� ���� ��Ŷ ����͸�
		//{{ 2010. 9. 8	������	��Ʈ��ũ Ʈ���� ���� ���� ����͸�
#ifdef SERV_TRAFFIC_USER_MONITORING
		AT_TRAFFIC_ABUSER,			// [5] Ʈ���� �߻���Ű�� ���� ����
#endif SERV_TRAFFIC_USER_MONITORING
		//}}

		AT_MAX
	};
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}

	enum REWARD_STATE
	{
		RS_NONE = 0,

		RS_BUY_SHOP,		// [1]
		RS_BUY_CHEAT,		// [2]
		RS_DUNGEON,			// [3]
		RS_PVP,				// [4]
		RS_QUEST,			// [5]
		RS_MANUFACTURE,		// [6]
		RS_RESOLVE,			// [7]
		RS_RANDOM,			// [8]
		RS_TRADE,			// [9]
		RS_PERSONAL_SHOP,	// [10]
		RS_RECOMMEND,		// [11]
		RS_TUTOL,			// [12]
		RS_EVENT,			// [13]	
		RS_IDENTIFY,		// [14]
		RS_LETTER,			// [15]
		RS_ARCADE,			// [16]
		RS_TRADE_ROLLBACK,	// [17]
		RS_CASH_ITEM,		// [18]
		RS_ITEM_EXCHANGE,	// [19]
		//{{ 2011. 04. 26	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
		RS_PICK_UP_PSHOP,	// [20]
#endif SERV_PSHOP_AGENCY
		//}}
#ifdef SERV_FINALITY_SKILL_SYSTEM	// ���볯¥: 2013-08-01
		RS_ITEM_EXTRACT,	// [21]
#endif // SERV_FINALITY_SKILL_SYSTEM
		RS_MAX,
	};

	enum ENCHANT_RESULT_STATE
	{
		ERS_NONE = 0,

		ERS_SUCCESS,		// [1]
		ERS_NO_CHANGE,		// [2]
		ERS_LEVEL_DOWN,		// [3]
		ERS_RESET,			// [4]
		ERS_BROKEN,			// [5]
	};

	//{{ 2012. 10. 02	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
#else
	enum ED_LOG_STATE
	{
		ELS_NONE = 0,

		ELS_DUNGEON_REWARD					= 1,	// [1]
		ELS_BUY_ED_ITEM						= 2,	// [2]
		ELS_BUY_ED_ITEM_ROLLBACK			= 3,	// [3]
		ELS_SELL_ED_ITEM					= 4,	// [4]
		ELS_OUT_TRADE						= 5,	// [5]
		ELS_IN_TRADE						= 6,	// [6]
		ELS_ITEM_REPAIR						= 7,	// [7]
		ELS_TRAINING_REWARD					= 8,	// [8]
		ELS_MANUFACTURE						= 9,	// [9]
		ELS_PERSONAL_SHOP_OPEN				= 10,	// [10]
		ELS_IN_PERSONAL_SHOP				= 11,	// [11]
		ELS_OUT_PERSONAL_SHOP				= 12,	// [12]
		ELS_ITEM_ENCHANT					= 13,	// [13]
		ELS_ITEM_SOCKET						= 14,	// [14]
		ELS_QUEST_REWARD					= 15,	// [15]
		ELS_ARCADE_REWARD					= 16,	// [16]
		ELS_LETTER_COST						= 17,	// [17]
		ELS_LETTER_SEND_ED					= 18,	// [18]
		ELS_LETTER_COST_ROLLBACK			= 19,	// [19]
		ELS_LETTER_ED_ROLLBACK				= 20,	// [20]
		ELS_LETTER_RECV_ED					= 21,	// [21]
		ELS_ATTRIB_ENCHANT					= 22,	// [22]
		ELS_DUNGEON_REALTIME				= 23,	// [23]
		//{{ 2011. 05. 11	������	�븮����
//#ifdef SERV_PSHOP_AGENCY
		ELS_OUT_PSHOP_AGENCY				= 24,	// [24]
		ELS_PSHOP_PICK_UP					= 25,	// [25]
//#endif SERV_PSHOP_AGENCY
		//}}
		ELS_GUILD_AD_COST					= 26,	// [26]
		ELS_GUILD_AD_COST_ROLLBACK			= 27,	// [27]
		//{{ 2012. 11. 26 ť�� ED ���� ���� ��� �߰� - ��μ�
//#ifdef SERV_CUBE_OPEN_ED_CONDITION
		ELS_CUBE_OPEN_ED_COST				= 28,	// [28]
//#endif SERV_CUBE_OPEN_ED_CONDITION
		//}
		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
//#ifdef SERV_RELATIONSHIP_SYSTEM
		ELS_BREAK_UP_RELATIONSHIP_ED_COST	= 29,	// [29]
//#endif SERV_RELATIONSHIP_SYSTEM
		//}

//#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// �۾���¥: 2013-06-17	// �ڼ���
		ELS_BATTLEFIELD_REWARD				= 30,		// ��Ʋ �ʵ忡�� ȹ���� ED
		ELS_COBO_EXPRESS_TICKET_COST		= 31,		// ���� �̵����� ���� �Ҹ��� ED
//#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG

//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05// �۾���¥: 2013-06-17	// �ڼ���
		ELS_ITEM_EVALUATE_COST				= 32,
		ELS_ITEM_EVALUATE_COST_ROLLBACK		= 33,
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05
	};
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

#ifdef SERV_HACKING_USER_CHECK_COUNT// �۾���¥: 2013-06-01	// �ڼ���
	enum ENUMHACKING_USER_BLOCK_NOTIFY_TYPE
	{
		HUCBNT_NONE		= 0,
		HUCBNT_RESON	= 1,
		HUCBNT_NOTIFY	= 2,
		HUCBNT_DEFAULT	= 3,
	};

	enum HackingUserCheckResultType
	{
		HUCRT_PACKETMORNITORING	= ( 1 << 0 ),
		HUCRT_ACCOUNTBLOCK		= ( 1 << 1 ),
		HUCRT_KICK				= ( 1 << 2 ),
	};

	struct KHackingUserCheckInfo
	{
		byte			m_bytePacketMornitoring;

		byte			m_byteAccountBlock;
		unsigned short	m_usAccountBlockTermDay;
		unsigned short	m_usAccountBlockTermHour;
		unsigned short	m_usAccountBlockTermMinute;
		byte			m_byteAccountBlockNotifyType;
		std::wstring	m_wstrAccountBlockNotify;

		byte			m_byteKick;
		byte			m_byteRandomKickMin;
		byte			m_byteRandomKickTerm;

		byte			m_byteReset;
		bool			m_bResetWeekly;

		KHackingUserCheckInfo()
			: m_bytePacketMornitoring( 0 )
			, m_byteAccountBlock( 0 )
			, m_usAccountBlockTermDay( 0 )
			, m_usAccountBlockTermHour( 0 )
			, m_usAccountBlockTermMinute( 0 )
			, m_byteAccountBlockNotifyType( 0 )
			, m_byteKick( 0 )
			, m_byteRandomKickMin( 0 )
			, m_byteRandomKickTerm( 0 )
			, m_byteReset( 0 )
			, m_bResetWeekly( false )
		{
		}
	};
	typedef	std::map<byte, KHackingUserCheckInfo>	TYPE_HACKING_USER_CHECK_INFO;
#endif // SERV_HACKING_USER_CHECK_COUNT

public:
	// for lua
	void SetAbuserLogPath_LUA( const char* pLogPath );
	void SetAbuserEventLogEnable_LUA( bool bEnable );
	void AddAbuserNotifyPhoneNum_LUA( const char* pPhoneNum );
	//{{ 2010. 11. 2	������	IP�ּ� ���� ���� �α�
#ifdef SERV_IP_ACCOUNT_LOG
	void AddJoinAccountCheckIP_LUA( const char* pIP );
#endif SERV_IP_ACCOUNT_LOG
	//}}
	//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	void SetGetItemCountMinMax_LUA( int iGetItemCountMin, int iGetItemCountMax );
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}
	//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_LEVEL
	void SetAbuserCheckLevelUp_LUA( bool bEnable );
#endif SERV_AUTO_HACK_CHECK_LEVEL
	//}}

#ifdef SERV_HACKING_USER_CHECK_COUNT// �۾���¥: 2013-06-01	// �ڼ���
	void	SetHackingUserCheckDefaultNotify_LUA( void );
	void	GetHackingUserCheckDefaultNotify( OUT std::wstring& wstrHackingUserCheckDefaultNotify ) const{	wstrHackingUserCheckDefaultNotify = m_wstrHackingUserCheckDefaultNotify;	}
	void	SetHackingUserCheckInfo_LUA( void );
	bool	GetHackingUserCheckInfo( IN const byte byteHackingUserCheckType, OUT KHackingUserCheckInfo& kInfo ) const;
#endif // SERV_HACKING_USER_CHECK_COUNT

	// function
	void GetAbuserNotifyPhoneNumList( std::vector< std::wstring >& vecPhoneNumList ) const;

	//{{ 2008. 10. 31  ������	�ǽð� ����Ʈ ����
	virtual void Tick();
	//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	void UpdateAllAbuserList( void );
	void UpdateAbuserList( int rtt );
	void SetAbuserList( int iRtt, const std::vector< UidType >& vecAbuserList );
#else
	void SendAbuserListReq();
	void UpdateAbuserList( const std::map< int, KAbuserList >& mapAbuserList );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}
	//}}
	
	bool IsPvpAbuser( UidType iUnitUID ) const;
	bool IsItemAbuser( UidType iUnitUID ) const;
	bool IsEnchantAbuser( UidType iUnitUID ) const;
	bool IsEDAbuser( UidType iUnitUID ) const;
	bool IsMornitoringAbuser( UidType iUserUID ) const;
	//{{ 2010. 9. 8	������	��Ʈ��ũ Ʈ���� ���� ���� ����͸�
#ifdef SERV_TRAFFIC_USER_MONITORING
	bool IsTrafficAbuser( UidType iUnitUID ) const;
#endif SERV_TRAFFIC_USER_MONITORING
	//}}

	//{{ 2010. 11. 2	������	IP�ּ� ���� ���� �α�
#ifdef SERV_IP_ACCOUNT_LOG
	bool JoinAccountCheckIP( const std::wstring& wstrIP ) const;
#endif SERV_IP_ACCOUNT_LOG
	//}}

	int  ChangeEnchantAbuserResult( int iEnchantResult );

	//{{ 2008. 11. 12  ������	���ηα�	
	//{{ 2013. 05. 29	������	��Ŷ ����͸� �ټ��� �׸� �߰�
#ifdef SERV_PACKET_MORNITORING_SPIRIT_COL
	void InsertAbuserEventLog( IN const UidType iUserUID, IN const UidType iUnitUID, IN const int iED, IN const int iGetItemCount, IN const int iSpirit, IN const unsigned short usEventID );
#else
	void InsertAbuserEventLog( IN const UidType iUserUID, IN const UidType iUnitUID, IN const int iED, IN const int iGetItemCount, IN const unsigned short usEventID );
#endif SERV_PACKET_MORNITORING_SPIRIT_COL
	//}}
	void FlushAbuserEventLog();
	//}}

	//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	int GetItemCountMin() const { return m_iGetItemCountMin; }
	int GetItemCountMax() const { return m_iGetItemCountMax; }
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}	

	//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_LEVEL
	bool IsAbuserCheckLevelUp() const { return m_bAbuserCheckLevelUp; } 
#endif SERV_AUTO_HACK_CHECK_LEVEL
	//}}

protected:
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void SendToLogDB( unsigned short usEventID );

private:
	std::set< UidType >				m_setAbuserList[AT_MAX];
	//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#else
	int								m_arrReleaseTick[AT_MAX];
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}
	boost::timer					m_tRefreshTimer;

	std::vector< std::wstring >		m_vecPhoneNumList;

	//{{ 2008. 11. 12  ������	���ηα�
	bool							m_bAbuserEventLogEnable;
	std::vector< KAbuserEventLog >	m_vecAbuserEventLog;
	std::wfstream					m_wfsAbuserEventLog;
	//}}

	//{{ 2009. 7. 14  ������	�ڵ�����
	std::string						m_strAbuserLogPath;
	//}}

	//{{ 2010. 11. 2	������	IP�ּ� ���� ���� �α�
#ifdef SERV_IP_ACCOUNT_LOG
	std::vector< std::string >		m_vecIP;
#endif SERV_IP_ACCOUNT_LOG
	//}}

	//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	int								m_iGetItemCountMin;
	int								m_iGetItemCountMax;
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}
	//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_LEVEL
	bool							m_bAbuserCheckLevelUp;
#endif SERV_AUTO_HACK_CHECK_LEVEL
	//}}

#ifdef SERV_HACKING_USER_CHECK_COUNT// �۾���¥: 2013-06-01	// �ڼ���
	std::wstring					m_wstrHackingUserCheckDefaultNotify;
	TYPE_HACKING_USER_CHECK_INFO	m_mapHackingUserCheckInfo;
#endif // SERV_HACKING_USER_CHECK_COUNT
};

DefRefreshSingletonInline( KAbuserLogManager );

template < class T >
void KAbuserLogManager::SendToLogDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { 0, -1 };

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PC_LOG_DB, anTrace, usEventID, data );
	LIF( spEvent->m_kDestPerformer.AddUID( 0 ) );

	SiKDBLayer()->QueueingEvent( spEvent );
}
