#pragma once 

#include "RefreshSingleton.h"
#include "CommonPacket.h"


class KGameSysVal
{
	DeclareRefreshSingleton( KGameSysVal );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	enum GAME_SYS_VALUE_TYPE
	{
		GSVT_GB_EXP = 0,
		GSVT_GB_ED,
		GSVT_GB_VP,
		GSVT_GB_SPIRIT,
		GSVT_PREMIUM_EXP,
		//{{ 2012. 07. 02	��μ�       PC �� ���� �� �߰� ����ġ
#ifdef SERV_GAME_BANG_PET_ADD_EXP
		GSVT_GB_PET_EXP,
#endif SERV_GAME_BANG_PET_ADD_EXP
		//}}
//#ifdef SERV_VIP_SYSTEM
		GSVT_VIP_EXP,
//#endif //SERV_VIP_SYSTEM
		GSVT_TOTAL_NUM,
	};

#ifdef SERV_EVENT_CHECK_POWER
	float	m_fMultiplayer;
#endif SERV_EVENT_CHECK_POWER

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	enum COME_BACK_STEP_DURATION
	{
		CBSD_NONE		= 0,
		CBSD_ONE		= 15,
		CBSD_TWO		= 30,
		CBSD_THREE		= 60,
	};

	enum COME_BACK_STEP
	{
		CBS_NONE		= 0,
		CBS_ONE			= 1,
		CBS_TWO			= 2,
		CBS_THREE		= 3,
	};
#endif SERV_COME_BACK_USER_REWARD
	//}} 

	//{{ 2012. 08. 11	�ڼ���	SMS ���� �߼� ��ũ��Ʈ
#ifdef SERV_RUNTIME_SCRIPT_FOR_SMS
	enum SMS_TYPE
	{
		SMS_NONE								= -1,
		SMS_SURVEILLANCE_THE_LETTER_AND_TRADE	= 0,
		SMS_SURVEILLANCE_THE_CCU_DROP			= 1,
		SMS_MAX									= 2,
	};
#endif SERV_RUNTIME_SCRIPT_FOR_SMS
	//}}

#ifdef SERV_ATTRACTION_ITEM_TIME_EVENT_VER2// �۾���¥: 2013-05-13	// �ڼ���
	struct AttractionItemTimeEventInfo
	{
		int		m_iEventAttractionItem;
		CTime	m_tBeginDate;
		CTime	m_tEndDate;

		AttractionItemTimeEventInfo()
			: m_iEventAttractionItem( 0 )
		{
		}
	};
#endif // SERV_ATTRACTION_ITEM_TIME_EVENT_VER2

public:
	KGameSysVal();
	~KGameSysVal();

	//check func..

#ifdef SERV_PC_BANG_TYPE
	float	GetGBEXPRate( int iPcBangType = -1 );
	float	GetGBEDRate( int iPcBangType = -1 );
	float	GetGBVPRate( int iPcBangType = -1 );
	bool	IsSpiritFree( int iPcBangType = -1 );
	float	GetGBPetEXPRate( int iPcBangType = -1 );
	bool	IsSecretDungeonFree( int iPcBangType = -1 );
	int		GetAdditionalHenirRewardCount( int iPcBangType = -1 );
	bool	IsUsingPcBangTitle( int iPcBangType = -1 );
	KPcBangReward GetPcBangReward( int iPcBangType = -1 );
#else SERV_PC_BANG_TYPE
	float	GetGBEXPRate()				{ return m_fValue[GSVT_GB_EXP]; }
	float	GetGBEDRate()				{ return m_fValue[GSVT_GB_ED]; }
	float	GetGBVPRate()				{ return m_fValue[GSVT_GB_VP]; }
	float	GetGBSpiritRate()			{ return m_fValue[GSVT_GB_SPIRIT]; }
#endif SERV_PC_BANG_TYPE
	float	GetPremiumEXPRate()			{ return m_fValue[GSVT_PREMIUM_EXP]; }
#ifdef SERV_VIP_SYSTEM
	float	GetVIPEXPRate()			{ return m_fValue[GSVT_VIP_EXP]; }
#endif //SERV_VIP_SYSTEM

#ifdef SERV_NEW_EVENT_TYPES
	int		GetLimitsLevel();
#else SERV_NEW_EVENT_TYPES
	int		GetLimitsLevel()			{ return m_iLimitsMaxLevel; }
#endif SERV_NEW_EVENT_TYPES

	int		GetGuestUserLimitLevel()	{ return m_iGuestUserLimitLevel; }
	void	GetAdvertisementURL( std::wstring& wstrAdURL );
	//{{ 2010. 01. 05  ������	PC�� �����̾�
#ifdef SERV_PC_BANG_TYPE
	void	GetPcBangPremiumItem( IN char cUnitType, OUT std::vector< KInventoryItemInfo >& vecPcBangPremiumItem, int iPcBangType = 0 );
#else SERV_PC_BANG_TYPE

#ifdef SERV_PC_BANG_PRE
	void	GetPcBangPremiumItem( IN char cUnitType, OUT std::vector< KInventoryItemInfo >& vecPcBangPremiumItem );
#endif SERV_PC_BANG_PRE

#endif SERV_PC_BANG_TYPE
	//}}

#ifndef SERV_PC_BANG_TYPE
	//{{ 2012. 07. 02	��μ�       PC �� ���� �� �߰� ����ġ
#ifdef SERV_GAME_BANG_PET_ADD_EXP
	float	GetGBPetEXPRate()				{ return m_fValue[GSVT_GB_PET_EXP]; }
	void AddKNXGameBangPetEXP_LUA( float fEXPRate );
#endif SERV_GAME_BANG_PET_ADD_EXP
	//}}
#endif SERV_PC_BANG_TYPE

	//{{ 2013. 04. 10	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	UidType GetTempItemUID()				{ return m_iTempItemUID++; }
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

	//{{ 2010. 7. 28  ������	������ ���� NPC
#ifdef SERV_AUTO_HACK_CHECK_NPC
	bool	IsHackUserRegRejected() const	{ return m_bIsHackUserRegRejected; }
	bool	IsHackUserRegMonitors() const	{ return m_bIsHackUserRegMonitors; }
	bool	IsHackUserKick() const			{ return m_bIsHackUserKick; }
#endif SERV_AUTO_HACK_CHECK_NPC
	//}}
	//{{ 2011. 03. 02	������	���� �ŷ� ����
#ifdef SERV_HACK_USER_TRADE_BLOCK
	bool	IsHackUserBlockTrade() const	{ return m_bIsHackUserBlockTrade; }
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}	

	//lua parsing func..
#ifdef SERV_PC_BANG_TYPE
	void	AddPCBang_LUA(	int iPcBangType, float fEXPRate, float fEDRate, float fVSPointRate, bool bSpiritFree, float fPetEXPRate,
							int iAdditionalHenirReward, bool bSecretDungeonFree, bool bAvatar, bool bTitle );
#else SERV_PC_BANG_TYPE
	void	AddKNXGameBang_LUA( float fEXPRate, float fEDRate, float fVSPointRate, float fSpiritRate );
#endif SERV_PC_BANG_TYPE

	void	AddLimitsMaxLevel_LUA( int iMaxLevel );
	void	AddPremiumEXP_LUA( float fPremiumEXP );
//#ifdef SERV_VIP_SYSTEM
	void	AddVIPEXP_LUA( float fVIPEXP );
//#endif //SERV_VIP_SYSTEM
	void	SetGuestUserLimitLevel_LUA( int iGuestUserLimitLevel );
	void	SetAdvertisementURL_LUA( const char* strURL );
	//{{ 2010. 01. 05  ������	PC�� �����̾�
#ifdef SERV_PC_BANG_PRE
	void	AddPcBangPremiumItem_LUA( char cUnitType, int iItemID );
#endif SERV_PC_BANG_PRE
	//}}
	//{{ 2010. 7. 28  ������	������ ���� NPC
#ifdef SERV_AUTO_HACK_CHECK_NPC	
	void	SetIsHackUserRegRejected_LUA( bool bVal );
	void	SetIsHackUserRegMonitors_LUA( bool bVal );
	void	SetIsHackUserKick_LUA( bool bVal );
#endif SERV_AUTO_HACK_CHECK_NPC
	//}}
#ifdef SERV_DESAGREE_HACK_USER_ON_OFF
	void	SetDisagreeHackUserFunc_LUA( bool bVal );
	bool	IsOnDisagreeHackUserFunc() const	{ return m_bDisagreeHackUserFunc; }
#endif //SERV_DESAGREE_HACK_USER_ON_OFF
	//{{ 2011. 03. 02	������	���� �ŷ� ����
#ifdef SERV_HACK_USER_TRADE_BLOCK
	void	SetIsHackUserBlockTrade_LUA( bool bVal );
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}

	//{{ 2011. 02. 23	������	ĳ�� ��ǰ ����Ʈ
#ifdef SERV_CASH_ITEM_LIST
	void	SetAccountStatisticsStartDate_LUA( const char* strDate );
	CTime	GetAccountStatisticsStartDate() { return m_tAccountStatisticsStartDate; }
#endif SERV_CASH_ITEM_LIST
	//}}
	//{{ 2011. 01. 18	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
	void	SetCharStatisticsStartDate_LUA( const char* strDate );
	CTime	GetCharStatisticsStartDate() { return m_tCharStatisticsStartDate; }
#endif SERV_CHAR_LOG
	//}}

	//{{ 2011. 02. 06	������	�ŷ� ���� ���
#ifdef SERV_BLOCK_TRADE
	void	SetBlockPersonalTrade_LUA( bool bVal );
	void	SetBlockPersonalShop_LUA( bool bVal );
	void	SetBlockSendLetter_LUA( bool bVal );

	bool	IsBlockPersonalTrade() const	{ return m_bBlockPersonalTrade; }
	bool	IsBlockPersonalShop() const		{ return m_bBlockPersonalShop; }
	bool	IsBlockSendLetter() const		{ return m_bBlockSendLetter; }
#endif SERV_BLOCK_TRADE
	//}}

	//{{ 2011. 08. 09	������	�ӽ�ID �����
#ifdef SERV_MACHINE_ID_BLOCK
	void	SetMachineIDBlock_LUA( bool bVal );
	bool	IsMachineIDBlock() const;
#endif SERV_MACHINE_ID_BLOCK
	//}}
	//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	void	SetMachineIDDuplicateCheck_LUA( bool bVal );
	bool	IsMachineIDDuplicateCheck() const;
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}
	//{{ 2011. 09. 08	������	������ ��Ŷ ����͸�
#ifdef SERV_AUTO_HACK_PACKET_MORNITOING
	void	SetHackUserPacketMornitor_LUA( bool bVal );
	bool	IsHackUserPacketMornitor() const							{ return m_bIsHackUserPacketMornitor; }
#endif SERV_AUTO_HACK_PACKET_MORNITOING
	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	void	SetIsNewUnitTradeBlock_LUA( bool bVal );
	bool	IsNewUnitTradeBlock() const								{ return m_bIsNewUnitTradeBlock; }
	void	SetNewUnitTradeBlockDay_LUA( int iVal );
	int		GetNewUnitTradeBlockDay() const							{ return m_iNewUnitTradeBlockDay; }
	void	SetNewUnitTradeBlockUnitClass_LUA( int iVal );
	int		GetNewUnitTradeBlockUnitClass() const					{ return m_iNewUnitTradeBlockUnitClass; }
#endif // SERV_NEW_UNIT_TRADE_LIMIT

	//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
	int		GetLimitsCountOfLetter()									{ return m_iLimitConutOfLetter; }
	int		GetLimitsCountOfTrade()										{ return m_iLimitConutOfTrade; }
	
	int		GetLetterMonitoringCheckTime()								{ return m_iLetterMonitoringCheckTime; }
	int		GetTradeMonitoringCheckTime()								{ return m_iTradeMonitoringCheckTime; }

	bool	GetLetterAndTradeMonitoringIsOn()							{ return m_bLetterAndTradeMonitoringIsOn; }
	void	SetLetterAndTradeMonitoringIsOn_Lua( bool bIsOn);

	void	SetLimitsCountOfLetter_Lua( int iCount );
	void	SetLetterMonitoringCheckTime_Lua( int iTime );

	void	SetLimitsCountOfTrade_Lua( int iCount );
	void	SetTradeMonitoringCheckTime_Lua( int iTime );

	void	SetMonitoringMinED_Lua( int iMin );
	void	SetMonitoringMaxED_Lua( int iMax );

	int		GetMonitoringMinED()										{ return m_iMonitoringMinED; }
	int		GetMonitoringMaxED()										{ return m_iMonitoringMaxED; }
#endif SERV_MONITORING_LETTER_AND_TRADE
	//}}

	//{{ 2011. 04. 26	������	deserialize ���� ����
#ifdef SERV_DESERIALIZE_FAIL_DISCONNECT
	void	SetDeserializeFailCheckCount_LUA( int iCount );
	int		GetDeserializeFailCheckCount() const					{ return m_iDeserializeFailCheckCount; }

	void	SetDeserializeFailCheck_LUA( bool bVal );
	bool	IsDeserializeFailCheck() const							{ return m_bDeserializeFailCheck; }
#endif SERV_DESERIALIZE_FAIL_DISCONNECT
	//}}

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	void	SetComeBackReward_LUA( int iStep, int iDays );
	void	GetComeBackRewardCondition( OUT std::map< int, int >& mapComeBackRewardCondition );
#endif SERV_COME_BACK_USER_REWARD
	//}}

	//{{ 2011. 09. 06  ��μ�	���� ���� ��ǰ ��� ���� ��
#ifdef SERV_CHECK_PERSONALSHOP_ITEM
	void	InsertItemPrice_LUA( int iItemID, int iPrice );
	void	DeleteItemPrice_LUA( int iItemID );
	int		GetItemPrice( IN int& iItemID );
#endif SERV_CHECK_PERSONALSHOP_ITEM
	//}}
	//{{ 2011. 06. 07    ��μ�    ���� �� �ŷ� ���� �ǽð� ���� ���
#ifdef SERV_MONITORING_LETTER_AND_TRADE_REALTIME_BLOCK
	void SetMonitoringLetterAndTradeBlockTime_LUA( int hh, int mm, int ss, int iDuration_s );
	bool IsMonitoringLetterAndTradeBlockTime( CTimeSpan& tSpan );
#endif SERV_MONITORING_LETTER_AND_TRADE_REALTIME_BLOCK
	//}} 
	//{{ 2011. 09. 07  ��μ�	���� �������� DIE NPC �ð� üũ ���
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
	void	SetSubStageNpcDieCheck_LUA( bool bCheck )		{ m_bSubStageNpcDieCheck = bCheck; }
	bool	GetSubStageNpcDieCheck()						{ return m_bSubStageNpcDieCheck; }
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
	//}}


#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX
	void	SetDungeonClearPaymentItemID_LUA( int iItemID );
	const int	GetDungeonClearPaymentItemID() const { return m_iPaymentItemID; }
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX

	//{{ 2012. 07. 09	��μ�       �̺�Ʈ ť�� ���� ���� �ð�
#ifdef SERV_EVENT_CUBE_OPEN_TIME
	void	SetEventCubeOpenStartTime_LUA( int iYear, int iMonth, int iDay, int iHour, int iMin, int iSec );
	void	SetEventCubeOpenEndTime_LUA( int iYear, int iMonth, int iDay, int iHour, int iMin, int iSec );
	CTime	GetEventCubeOpenEventStartTime()				{ return m_tEventCubeStart; }
	CTime	GetEventCubeOpenEventEndTime()				{ return m_tEventCubeEnd; }
#endif SERV_EVENT_CUBE_OPEN_TIME
	//}}

	//{{  2012. 1. 16	��μ�		���Ӽ������� Server SN �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	void SetCheckServerSN_LUA( bool bVal );
	bool GetCheckServerSN()										{ return m_bCheckServerSN; }
	void SetCheckServerSN_ERR_01_1_LUA( bool bVal );
	bool GetCheckServerSN_ERR_01_1()							{ return m_bCheckServerSN_ERR_01_1; }
	void SetCheckServerSN_ERR_01_2_LUA( bool bVal );
	bool GetCheckServerSN_ERR_01_2()							{ return m_bCheckServerSN_ERR_01_2; }
	void SetCheckServerSN_ERR_02_LUA( bool bVal );
	bool GetCheckServerSN_ERR_02()								{ return m_bCheckServerSN_ERR_02; }
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	//}}

	//{{ 2012. 01. 19	��μ�	�÷���� ���� ��ȭ �̺�Ʈ �ǽð� ����
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	void SetEnchantEventStartTime_LUA( int iYear, int iMonth, int iDay, int iHour, int iMin, int iSec );
	void SetEnchantEventEndTime_LUA( int iYear, int iMonth, int iDay, int iHour, int iMin, int iSec );
	bool IsEnchantEvent();
#endif SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	//}}

	//{{ 2012. 02. 06	�ڼ���	�߷�Ÿ�� ���� �̺�Ʈ
#ifdef SERV_VALENTINE_DAY_EVENT
	void	SetMenBuffEvent_LUA( bool bValue );
	bool	GetMenBuffEvent()							{	return m_bMenBuffEvent;	}
#endif SERV_VALENTINE_DAY_EVENT
	//}}

	//{{ 2012. 03. 13	��μ�		�ǽð� ���� �϶� üũ sms
#ifdef SERV_CHECK_DROP_CCU
	void	SetCheckDropCCU_LUA( bool bValue );
	bool	GetCheckDropCCU()							{	return m_bCheckDropCCU;	}
	void	SetDropCCUPercent_LUA( int iValue );
	float	GetDropCCUPercent();
#endif SERV_CHECK_DROP_CCU
	//}}


	//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	void	SetHackingUserKick_LUA( bool bValue );
	bool	GetHackingUserKick()							{	return m_bHackingUserKick;	}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	//}}

	//{{ 2012. 07. 12	������		���� ť�� ���� ������ �ý���
#ifdef SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
	void	SetEnableOpenRandomCubeDelaySystem_LUA( bool bValue );
	void	SetOpenRandomCubeDelayTime_LUA( double fTime );
	void	SetOpenRandomCubeDelayCount_LUA( int iCount );

	bool	IsEnableOpenRandomCubeDelaySystem() const			{ return m_bEnableOpenRandomCubeDelaySystem; }
	double	GetOpenRandomCubeDelayTime() const					{ return m_fOpenRandomCubeDelayTime; }
	int		GetOpenRandomCubeDelayCount() const					{ return m_iOpenRandomCubeDelayCount; }
#endif SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
	//}}

	//{{ 2012. 08. 11	�ڼ���	SMS ���� �߼� ��ũ��Ʈ
#ifdef SERV_RUNTIME_SCRIPT_FOR_SMS
	void	AddSMSRecipient_LUA( IN int iType, IN const char* pPhoneNumber );
	void	GetRecipientSMS( IN const int iType, OUT std::vector<std::wstring>& vecPhoneNum );
#endif SERV_RUNTIME_SCRIPT_FOR_SMS
	//}}

	//{{ 2012 ���� ����2 ���� ��Ī �̺�Ʈ	- ��μ�
//#ifdef SERV_2012_PVP_SEASON2_EVENT
//	void SetPvpEventTime_LUA( int iDayType, int iCountNum, int iStartHour, int iStartMin, int iEndtHour, int iEndMin );
//	bool IsPvpEventTime( CTime& tNow, int& iIndex, std::wstring& wstrLastTime, bool& bIsPossible );
//#endif SERV_2012_PVP_SEASON2_EVENT
	//}}
	//{{ 2012. 09. 18   ��μ�   2012 ���� ����2 �̺�Ʈ
#ifdef SERV_2012_PVP_SEASON2_NEW_EVENT
	void SetPvpEventTime_LUA( int iDayType, int iCountNum, int iStartHour, int iStartMin, int iEndtHour, int iEndMin );
	bool IsPvpEventTime( CTime& tNow );
#endif SERV_2012_PVP_SEASON2_NEW_EVENT
	//}}

	//{{ 2013. 2. 1	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	void	SetLanBugOutCheckOption_Lua( void );
	bool	GetLanBugOutCheck( void )			const {	return m_bLanBugOutCheck;	}
	byte	GetLanBugOutCheckMaxCount( void )	const {	return m_byteLanBugOutCheckMaxCount;	}
	float	GetLanBugOutCheckTime( void	)		const {	return m_fLanBugOutCheckTime;	}
	byte	GetLanBugOutCheckCount( void )		const {	return m_byteLanBugOutCheckCount;	}
	float	GetLanBugOutCheckTerm( void )		const {	return m_fLanBugOutCheckTermMin + ( m_fLanBugOutCheckTerm * ( rand() / static_cast<float>( RAND_MAX ) ) );	}
	float	GetLanBugOutCheckRepeatTerm( void )	const {	return m_fLanBugOutCheckRepeatTerm;	}

	void	SetLanBugOutVerifyOption_Lua( void );
	bool	GetLanBugOutVerify( void )			const {	return m_bLanBugOutVerify;	}
	byte	GetLanBugOutVerifyMaxCount( void )	const {	return m_byteLanBugOutVerifyMaxCount;	}
	float	GetLanBugOutVerifyTime( void )		const {	return m_fLanBugOutVerifyTime;	}
	byte	GetLanBugOutVerifyCount( void )		const {	return m_byteLanBugOutVerifyCount;	}
	float	GetLanBugOutVerifyTerm( void )		const {	return m_fLanBugOutVerifyTermMin + ( m_fLanBugOutVerifyTerm * ( rand() / static_cast<float>( RAND_MAX  ) ) );	}
	float	GetLanBugOutVerifyRepeatTerm( void )const {	return m_fLanBugOutVerifyRepeatTerm;	}

	
	void	SetLanBugOutJustLog_Lua( bool bJustLog ){	m_bLanBugOutJustLog = bJustLog;	}
	bool	GetLanBugOutJustLog( void )			const {	return m_bLanBugOutJustLog;	}
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}
	//{{ 2012. 10. 10	�ڼ���	ƨ�� ���� ������ ������
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	void	SetLanBugOut1_Lua( bool bOn );
	void	SetLanBugOut2_Lua( bool bOn );

	bool	GetLanBugOut1( void ){	return m_bLanBugOut1;	}
	bool	GetLanBugOut2( void ){	return m_bLanBugOut2;	}
#endif UDP_CAN_NOT_SEND_USER_KICK
	//}}

	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	void	SetCheckChangeHostTime_LUA( int iCheckTime );
	int		GetCheckChangeHostTime()					{	return m_iCheckChangeHostTime;	}
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
	void	SetBattleFieldCheckChangeHostTime_LUA( int iCheckTime );
	int		GetBattleFieldCheckChangeHostTime()					{	return m_iBattleFieldCheckChangeHostTime;	}
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
	void	SetMaxPingScore_Lua( DWORD dwMaxPingScore ){	m_dwMaxPingScore = dwMaxPingScore;	}
	DWORD	GetMaxPingScore( void ) const{	return m_dwMaxPingScore;	}
#endif SERV_CHOOSE_FASTEST_HOST
	//}
	//{{ 2013. 02. 04  Ư���ð� ������ Ȯ�� ���� - ��μ�
#ifdef SERV_ATTRACTION_ITEM_TIME_EVENT
	void SetRandomCubeEventStartTime_LUA( int iYear, int iMonth, int iDay, int iHour, int iMin, int iSec );
	void SetRandomCubeEventEndTime_LUA( int iYear, int iMonth, int iDay, int iHour, int iMin, int iSec );
	void SetTimeRandomItem_LUA( int iItemID, int iEventItemID );
	CTime GetRandomCubeEventStartTime()					{ return m_tRandomCubeEventStart; }
	CTime GetRandomCubeEventEndTime()					{ return m_tRandomCubeEventEnd; }
	int IsTimeEventItemID( int iItemID );
#endif SERV_ATTRACTION_ITEM_TIME_EVENT
	//}

	//{{ 2010. 11. 12  ��ȿ��	GAMEFORGE ä�� �α� On-Off �����ϵ���
#ifdef SERV_RECORD_CHAT
	void	SetIsRecordChat_LUA( bool bVal );
	bool	IsRecordChat()		{ return m_bRecordChat; }
#endif SERV_RECORD_CHAT
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	void	SetProcessCommunication_Lua( IN bool bProcessCommunicationON );
	bool	GetProcessCommunication( void ) const{	return m_bProcessCommunicationON;	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS

	//{{ 2013. 2. 18	�ڼ���	���� ���� ������ ���� ���� �� ������ ó��
#ifdef SERV_EVENT_BINGO
	void	SetBingoEventPayoutRepeateUse_Lua( IN bool bUse ){	m_bUse = bUse;	}
	bool	GetBingoEventPayoutRepeateUse( void ){	return m_bUse;	}
	void	SetBingoEventPayoutRepeateUse2_Lua( IN bool bUse ){	m_bUse2 = bUse;	}
	bool	GetBingoEventPayoutRepeateUse2( void ){	return m_bUse2;	}

	void	SetBingoEventPayoutRepeate_Lua( IN byte byteRepeate ){	m_byteBingoRepeate = byteRepeate;	}
	float	GetBingoEventPayoutRepeate( void ){	return m_byteBingoRepeate;	}
#endif SERV_EVENT_BINGO
	//}}
	//{{ 2013. 3. 14	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
	void	SetLocalRankingNumOfRanker_Lua( IN int iNumOfRanker ){	m_iLocalRankingNumOfRanker = iNumOfRanker;	}
	int		GetLocalRankingNumOfRanker( void ) const{	return m_iLocalRankingNumOfRanker;	}

	void	SetLocalRankingElpUpdateGap_Lua( IN float fLocalRankingElpUpdateGap ){	m_fLocalRankingElpUpdateGap = fLocalRankingElpUpdateGap;	}
	float	GetLocalRankingElpUpdateGap( void ) const{	return m_fLocalRankingElpUpdateGap;	}

	void	ResetLocalRankingSystem_Lua( void ) const;
	void	CheckLocalRankingSystem_Lua( int iMainTabIndex, int iSubTabIndex ) const;
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}

#ifdef	SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ// ���볯¥: 2013-05-03
	void	SetHostCheckTerm_Lua( IN float bHostCheckTerm ){	m_fHostCheckTerm = bHostCheckTerm;	}
	float	GetHostCheckTerm( void ) const{	return m_fHostCheckTerm;	}
#endif	// SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-09	// �ڼ���
	void	SetLanBugOutDisconnectCheckStartOnGameLoadingReq_Lua( IN bool bLanBugOutDisconnectCheckStartOnGameLoadingReq ){	m_bLanBugOutDisconnectCheckStartOnGameLoadingReq = bLanBugOutDisconnectCheckStartOnGameLoadingReq;	}
	bool	GetLanBugOutDisconnectCheckStartOnGameLoadingReq( void )	const{	return m_bLanBugOutDisconnectCheckStartOnGameLoadingReq;	}
	
	void	SetLanBugOutDisconnectCheckTerm_Lua( float fLanBugOutDisconnectCheckTerm ){	m_fLanBugOutDisconnectCheckTerm = fLanBugOutDisconnectCheckTerm;	}
	float	GetLanBugOutDisconnectCheckTerm( void )	const{	return m_fLanBugOutDisconnectCheckTerm;	}

	void	SetLanBugOutDisconnectTerm_Lua( int iLanBugOutMinDisconnectTerm, int iLanBugOutDisconnectTerm )
	{
		m_iLanBugOutMinDisconnectTerm	= iLanBugOutMinDisconnectTerm;
		m_iLanBugOutDisconnectTerm		= iLanBugOutDisconnectTerm;
	}

	void	GetLanBugOutDisconnectTerm( OUT int& iLanBugOutMinDisconnectTerm, OUT int& iLanBugOutDisconnectTerm ) const
	{
		iLanBugOutMinDisconnectTerm	= m_iLanBugOutMinDisconnectTerm;
		iLanBugOutDisconnectTerm	= m_iLanBugOutDisconnectTerm;
	}
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_ATTRACTION_ITEM_TIME_EVENT_VER2// �۾���¥: 2013-05-13	// �ڼ���
	void	SetAttractionItemTimeEvent_Lua( IN int iSrcItemID, IN int iDesItemID );
	void	CheckAttractionItemTimeEvent( IN OUT int& iItemID );
#endif // SERV_ATTRACTION_ITEM_TIME_EVENT_VER2

#ifdef SERV_DUNGEON_STAGE_LOAD_LOG// �۾���¥: 2013-05-15	// �ڼ���
	void	SetDungeonStageLoadLog_Lua( IN bool bDungeonStageLoadLog ){	m_bDungeonStageLoadLog = bDungeonStageLoadLog;	}
	bool	GetDungeonStageLoadLog( void ) const{	return m_bDungeonStageLoadLog;	}
#endif // SERV_DUNGEON_STAGE_LOAD_LOG

#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-30	// �ڼ���
	void	ResetTimeEnchantEventInfo_Lua( void ) const;
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-07-03	// �ڼ���
	void	ResetExchangeLimitInfo_Lua( void ) const;
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_FIX_AFTER_WORK_STORAGE_CLASS// �۾���¥: 2013-12-21	// �ڼ���
	void	AfterWorkStorageMessageDeleteTerm_Second_LUA( IN int iAwsCriterionNum, IN int iAwsUnderTerm, IN int iAwsAboveOrEqualTerm );
	int		GetAwsCriterionNum( void )		const{	return m_iAwsCriterionNum;	}
	int		GetAwsUnderTerm( void )			const{	return m_iAwsUnderTerm;	}
	int		GetAwsAboveOrEqualTerm( void )	const{	return m_iAwsAboveOrEqualTerm;	}
#endif // SERV_FIX_AFTER_WORK_STORAGE_CLASS

protected:
	//## Game System value..
	//{{

#ifdef SERV_PC_BANG_TYPE
	std::vector< KPcBangReward > m_vecPcBangReward;
#endif SERV_PC_BANG_TYPE
	// KOREA NEXON GAME BANG BONUS..
	float			m_fValue[GSVT_TOTAL_NUM];
	//}}

	int				m_iLimitsMaxLevel;		// ���� ����..
	int				m_iGuestUserLimitLevel; // ü��ID ���� ����

	std::wstring	m_wstrAdvertisementURL; // Ŭ���̾�Ʈ ���� ���� URL

	//{{ 2010. 01. 05  ������	PC�� �����̾�
	std::map< char, std::vector< int > > m_mapPcBangPremiumItem; // PC�� �����̾� ������
	UidType			m_iTempItemUID;			// PC�� ���� �ӽ� �߱� ItemUID
	//}}
	//{{ 2010. 7. 28  ������	������ ���� NPC
#ifdef SERV_AUTO_HACK_CHECK_NPC
	bool			m_bIsHackUserRegRejected;
	bool			m_bIsHackUserRegMonitors;
	bool			m_bIsHackUserKick;
#endif SERV_AUTO_HACK_CHECK_NPC
	//}}
#ifdef SERV_DESAGREE_HACK_USER_ON_OFF
	bool			m_bDisagreeHackUserFunc;	// ���� ���� �� ������ ���� ó�� ��/��
#endif //SERV_DESAGREE_HACK_USER_ON_OFF
	//{{ 2011. 03. 02	������	���� �ŷ� ����
#ifdef SERV_HACK_USER_TRADE_BLOCK
	bool			m_bIsHackUserBlockTrade;
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}
	//{{ 2011. 02. 23	������	ĳ�� ��ǰ ����Ʈ
#ifdef SERV_CASH_ITEM_LIST
	CTime			m_tAccountStatisticsStartDate;
#endif SERV_CASH_ITEM_LIST
	//}}
	//{{ 2011. 01. 18	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
	CTime			m_tCharStatisticsStartDate;
#endif SERV_CHAR_LOG
	//}}

	//{{ 2011. 02. 06	������	�ŷ� ���� ���
#ifdef SERV_BLOCK_TRADE
	bool			m_bBlockPersonalTrade;
	bool			m_bBlockPersonalShop;
	bool			m_bBlockSendLetter;
#endif SERV_BLOCK_TRADE
	//}}
	//{{ 2011. 08. 09	������	�ӽ�ID �����
#ifdef SERV_MACHINE_ID_BLOCK
	bool			m_bMachineIDBlock;
#endif SERV_MACHINE_ID_BLOCK
	//}}
	//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	bool			m_bMachineIDDuplicateCheck;
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}
	//{{ 2011. 09. 08	������	������ ��Ŷ ����͸�
#ifdef SERV_AUTO_HACK_PACKET_MORNITOING
	bool			m_bIsHackUserPacketMornitor;
#endif SERV_AUTO_HACK_PACKET_MORNITOING
	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	bool			m_bIsNewUnitTradeBlock;
	int				m_iNewUnitTradeBlockDay;
	int				m_iNewUnitTradeBlockUnitClass;
#endif // SERV_NEW_UNIT_TRADE_LIMIT

	//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
	int				m_iLimitConutOfLetter;
	int				m_iLetterMonitoringCheckTime;

	int				m_iLimitConutOfTrade;
	int				m_iTradeMonitoringCheckTime;

	bool			m_bLetterAndTradeMonitoringIsOn;

	int				m_iMonitoringMinED;
	int				m_iMonitoringMaxED;
#endif SERV_MONITORING_LETTER_AND_TRADE
	//}}
	//{{ 2011. 04. 26	������	deserialize ���� ����
#ifdef SERV_DESERIALIZE_FAIL_DISCONNECT
	int				m_iDeserializeFailCheckCount;
	bool			m_bDeserializeFailCheck;
#endif SERV_DESERIALIZE_FAIL_DISCONNECT
	//}}

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	std::map< int, int >			m_mapComeBackRewardCondition;		// ���� ���� (�ܰ�, �Ⱓ)
#endif SERV_COME_BACK_USER_REWARD
	//}} 

	//{{ 2011. 09. 06  ��μ�	���� ���� ��ǰ ��� ���� ��
#ifdef SERV_CHECK_PERSONALSHOP_ITEM
	std::map< int, int >			m_mapItemPrice;					// ������ ����(�۾����)
#endif SERV_CHECK_PERSONALSHOP_ITEM
	//}}

	//{{ 2011. 09. 07  ��μ�	���� �������� DIE NPC �ð� üũ ���
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
	bool							m_bSubStageNpcDieCheck;
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
	//}}


#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX
	int								m_iPaymentItemID;
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX

	//{{ 2012. 07. 09	��μ�       �̺�Ʈ ť�� ���� ���� �ð�
#ifdef SERV_EVENT_CUBE_OPEN_TIME
	CTime							m_tEventCubeStart;
	CTime							m_tEventCubeEnd;
#endif SERV_EVENT_CUBE_OPEN_TIME
	//}}

	//{{  2012. 1. 16	��μ�		���Ӽ������� Server SN �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	bool						m_bCheckServerSN;					// ���Ӽ������� ServerSN üũ ���
	bool						m_bCheckServerSN_ERR_01_1;			// ���Ӽ������� ServerSN üũ ��� - ERR_SERVERSN_01-1    ex) A/2, B/1
	bool						m_bCheckServerSN_ERR_01_2;			// ���Ӽ������� ServerSN üũ ��� - ERR_SERVERSN_01-2    ex) A/0, A/3
	bool						m_bCheckServerSN_ERR_02;			// ���Ӽ������� ServerSN üũ ��� - ERR_SERVERSN_02	  ex) C/0, C/3
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	//}}
	//{{ 2012. 01. 19	��μ�	�÷���� ���� ��ȭ �̺�Ʈ �ǽð� ����
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	CTime						m_tEventEnchantStart;
	CTime						m_tEventEnchantEnd;
#endif SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	//}}

	//{{ 2012. 02. 06	�ڼ���	�߷�Ÿ�� ���� �̺�Ʈ
#ifdef SERV_VALENTINE_DAY_EVENT
	bool						m_bMenBuffEvent;
#endif SERV_VALENTINE_DAY_EVENT
	//}}

	//{{ 2012. 03. 13	��μ�		�ǽð� ���� �϶� üũ sms
#ifdef SERV_CHECK_DROP_CCU
	bool						m_bCheckDropCCU;
	float						m_fDropCCUPercent;
#endif SERV_CHECK_DROP_CCU
	//}}

	//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	bool						m_bHackingUserKick;
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	//}}

	//{{ 2011. 06. 07    ��μ�    ���� �� �ŷ� ���� �ǽð� ���� ���
#ifdef SERV_MONITORING_LETTER_AND_TRADE_REALTIME_BLOCK
	CTimeSpan					m_tLetterAndTradeBlockStartTime;
	CTimeSpan					m_tLetterAndTradeBlockEndTime;
#endif SERV_MONITORING_LETTER_AND_TRADE_REALTIME_BLOCK
	//}}
	//{{ 2012. 07. 12	������		���� ť�� ���� ������ �ý���
#ifdef SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
	double						m_fOpenRandomCubeDelayTime;
	int							m_iOpenRandomCubeDelayCount;
	bool						m_bEnableOpenRandomCubeDelaySystem;
#endif SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
	//}}

	//{{ 2012. 08. 11	�ڼ���	SMS ���� �߼� ��ũ��Ʈ
#ifdef SERV_RUNTIME_SCRIPT_FOR_SMS
	std::set<std::wstring>	m_setSMS[SMS_MAX];
#endif SERV_RUNTIME_SCRIPT_FOR_SMS
	//}}

	//{{ 2012 ���� ����2 ���� ��Ī �̺�Ʈ	- ��μ�
//#ifdef SERV_2012_PVP_SEASON2_EVENT
	//{{ 2012. 09. 18   ��μ�   2012 ���� ����2 �̺�Ʈ
#ifdef SERV_2012_PVP_SEASON2_NEW_EVENT
	std::map< int, std::map< int, CTimeSpan > >						m_mapStartPvpEvent;		// [����/�ָ� , [��°, �ð�]]
	std::map< int, std::map< int, CTimeSpan > >						m_mapEndPvpEvent;		// [����/�ָ� , [��°, �ð�]]
#endif SERV_2012_PVP_SEASON2_NEW_EVENT
	//}}
//#endif SERV_2012_PVP_SEASON2_EVENT
	//}}

	//{{ 2013. 2. 1	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	bool						m_bLanBugOutCheck;
	byte						m_byteLanBugOutCheckMaxCount;
	float						m_fLanBugOutCheckTime;
	byte						m_byteLanBugOutCheckCount;
	float						m_fLanBugOutCheckTermMin;
	float						m_fLanBugOutCheckTerm;
	float						m_fLanBugOutCheckRepeatTerm;

	bool						m_bLanBugOutVerify;
	byte						m_byteLanBugOutVerifyMaxCount;
	float						m_fLanBugOutVerifyTime;
	byte						m_byteLanBugOutVerifyCount;
	float						m_fLanBugOutVerifyTermMin;
	float						m_fLanBugOutVerifyTerm;
	float						m_fLanBugOutVerifyRepeatTerm;

	bool						m_bLanBugOutJustLog;
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}
	//{{ 2012. 10. 10	�ڼ���	ƨ�� ���� ������ ������
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	bool						m_bLanBugOut1;
	bool						m_bLanBugOut2;
#endif UDP_CAN_NOT_SEND_USER_KICK
	//}}
	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	int							m_iCheckChangeHostTime;
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    int							m_iBattleFieldCheckChangeHostTime;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
	DWORD						m_dwMaxPingScore;
#endif SERV_CHOOSE_FASTEST_HOST
	//}
	//{{ 2013. 02. 04  Ư���ð� ������ Ȯ�� ���� - ��μ�
#ifdef SERV_ATTRACTION_ITEM_TIME_EVENT
	CTime						m_tRandomCubeEventStart;
	CTime						m_tRandomCubeEventEnd;
	std::map< int, int >		m_mapTimeRandomItem;
#endif SERV_ATTRACTION_ITEM_TIME_EVENT
	//}

	//{{ 2010. 11. 12  ��ȿ��	GAMEFORGE ä�� �α� On-Off �����ϵ���
#ifdef SERV_RECORD_CHAT
	bool						m_bRecordChat;
#endif SERV_RECORD_CHAT
	//}}
	//{{ 2013. 2. 18	�ڼ���	���� ���� ������ ���� ���� �� ������ ó��
#ifdef SERV_EVENT_BINGO
	bool						m_bUse;
	bool						m_bUse2;
	byte						m_byteBingoRepeate;
#endif SERV_EVENT_BINGO
	//}}
	//{{ 2013. 3. 14	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
	int							m_iLocalRankingNumOfRanker;
	float						m_fLocalRankingElpUpdateGap;
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}

#ifdef	SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ// ���볯¥: 2013-05-03
	float						m_fHostCheckTerm;
#endif	// SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	bool						m_bProcessCommunicationON;
#endif //SERV_PROCESS_COMMUNICATION_KSMS

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-09	// �ڼ���
	bool						m_bLanBugOutDisconnectCheckStartOnGameLoadingReq;
	float						m_fLanBugOutDisconnectCheckTerm;
	int							m_iLanBugOutMinDisconnectTerm;
	int							m_iLanBugOutDisconnectTerm;
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_ATTRACTION_ITEM_TIME_EVENT_VER2// �۾���¥: 2013-05-13	// �ڼ���
	std::multimap<int, AttractionItemTimeEventInfo>		m_mmapAttractionItemTimeEventInfo;
#endif // SERV_ATTRACTION_ITEM_TIME_EVENT_VER2

#ifdef SERV_DUNGEON_STAGE_LOAD_LOG// �۾���¥: 2013-05-15	// �ڼ���
	bool						m_bDungeonStageLoadLog;
#endif // SERV_DUNGEON_STAGE_LOAD_LOG

#ifdef SERV_FIX_AFTER_WORK_STORAGE_CLASS// �۾���¥: 2013-12-21	// �ڼ���
	int		m_iAwsCriterionNum;		// ó�� �ð��� ������ ���� ����
	int		m_iAwsUnderTerm;			// ���� ���� �̸��� ����� ó�� ����
	int		m_iAwsAboveOrEqualTerm;	// ���� ���� �̻��� ����� ó�� ����
#endif // SERV_FIX_AFTER_WORK_STORAGE_CLASS
};

DefRefreshSingletonInline( KGameSysVal );



