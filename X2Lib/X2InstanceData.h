#pragma once


#ifdef ITEM_CHEAT_POPUP_TEST
	//class CX2ItemCheatPopUp;
	#include "./X2ItemCheatPopUp.h"
#endif ITEM_CHEAT_POPUP_TEST

#ifdef SERV_CODE_EVENT
#include "ScriptID_Code.h"
#define IF_EVENT_ENABLED(id)		if( g_pInstanceData != NULL && g_pInstanceData->IsEnableCode( id ) == true )
#define ELSE_IF_EVENT_ENABLED(id)	else if( g_pInstanceData != NULL && g_pInstanceData->IsEnableCode( id ) == true )
#define ELSE						else
#endif SERV_CODE_EVENT

#ifdef UDP_CAN_NOT_SEND_USER_KICK
	#include <boost/timer.hpp>
#endif //UDP_CAN_NOT_SEND_USER_KICK

class CX2MiniMapUI;
class CX2InstanceData
{
	
public:

	CX2InstanceData(void);
	virtual ~CX2InstanceData(void);

	void OnFrameMove( double fTime, float fElapsedTime );


#ifdef ITEM_CHEAT_POPUP_TEST
	CX2ItemCheatPopUp& GetItemCheatPopup() { return m_ItemCheatPopup; }
#endif ITEM_CHEAT_POPUP_TEST


	void ResetMiniMapUI();
	CX2MiniMapUI* GetMiniMapUI(){ return m_pMiniMapUI; }
	void DeleteMiniMapUI(){ SAFE_DELETE(m_pMiniMapUI); }


	bool GetIsDungeonLounge() const { return m_bIsDungeonLounge; }
	void SetIsDungeonLounge(bool val) { m_bIsDungeonLounge = val; }

	bool GetLoadedDungeonLoungeUnits() const { return m_bLoadedDungeonLoungeUnits; }
	void SetLoadedDungeonLoungeUnits(bool val) { m_bLoadedDungeonLoungeUnits = val; }

	CX2World::WORLD_ID GetDungeonLoungeWorldID() const { return m_eDungeonLoungeWorldID; }
	void SetDungeonLoungeWorldID(CX2World::WORLD_ID val) { m_eDungeonLoungeWorldID = val; }


	const std::vector< KFieldUserInfo >& GetDungeonLoungeUnitInfo() const { return m_vecDungeonLoungeUnitInfo; }
	void AddDungeonLoungeUnitInfo( const KFieldUserInfo& val ) { m_vecDungeonLoungeUnitInfo.push_back(val); }
	void ClearDungeonLoungeUnitInfo() { m_vecDungeonLoungeUnitInfo.resize(0); }


	D3DXVECTOR3 GetLastDungeonGatePos() const { return m_vLastDungeonGatePos; }
	void SetLastDungeonGatePos(D3DXVECTOR3 val) { m_vLastDungeonGatePos = val; }

	int GetLastDungeonGatePosLineIndex() const { return m_iLastDungeonGatePosLineIndex; }
	void SetLastDungeonGatePosLineIndex(int val) { m_iLastDungeonGatePosLineIndex = val; }

	


	const wstring& GetUserID() const { return m_UserID; }
	void SetUserID(const wstring& val) { m_UserID = val; }

#ifdef SERV_CHANNELING_AERIA
	const wstring& GetUserSessionID() const { return m_UserSessionID; }
	void SetUserSessionID(const wstring& val) { m_UserSessionID = val; }
#endif //SERV_CHANNELING_AERIA

	const wstring& GetUserPassword() const { return m_UserPassword; }
	void SetUserPassword(const wstring& val) { m_UserPassword = val; }

#ifdef CLIENT_COUNTRY_TH
	const wstring& GetSocketID() const { return m_wstrSocketID; }
	void SetSocketID(const wstring& val) { m_wstrSocketID = val; }
#endif // CLIENT_COUNTRY_TH

	//{{ kimhc // 2010-06-24 // OTP �۾�
#ifdef SERV_OTP_AUTH
	const wstring& GetNxHWID() const { return m_wstrNxHWID; }
	void SetNxHWID(const wstring& val) { m_wstrNxHWID = val; }	
#endif	SERV_OTP_AUTH
	//}} kimhc // 2010-06-24 // OTP �۾�

	//{{ kimhc // 2009-12-18 // ������ �߰� �۾����� ������� ����
#ifndef	ADD_SERVER_GROUP
	int GetChannelIdFromListId(int listId);
	int GetListIdFromChannelId(int channelId);
#endif	ADD_SERVER_GROUP
	//}} kimhc // 2009-12-18 // ������ �߰� �۾����� ������� ����

	int GetConnectChannelServerID() const { return m_iConnectChannelServerID; }
	void SetConnectChannelServerID(int val) { m_iConnectChannelServerID = val; }

	bool GetFirstJoinVillage() { return m_bFirstJoinVillage; }
	void SetFirstJoinVillage(bool val) { m_bFirstJoinVillage = val; }
	
	wstring GetConnectChannelServerName();
#ifdef SERVER_INTEGRATION_CHANNEL_NAME_FIX
#ifdef EXTEND_SERVER_GROUP_MASK
	wstring GetChannelServerName( int ChannelID, int ServerID);
#else
	wstring GetChannelServerName( int ChannelID, SERVER_GROUP_ID ServerID);
#endif // EXTEND_SERVER_GROUP_MASK
#endif SERVER_INTEGRATION_CHANNEL_NAME_FIX
	wstring GetChannelServerName( int ChannelID );		// 09.04.20 �¿�

#if defined( _IN_HOUSE_ ) || defined( _OPEN_TEST_ ) || defined( _OPEN_TEST_2_ )
	bool GetFrameScale() const { return m_bFrameScale; }
	void SetFrameScale(bool val) { m_bFrameScale = val; }
#endif

	//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ED ȹ�� ����
#ifdef	REAL_TIME_ELSWORD
	int		GetInstanceED() const { return m_iED; }
	void	SetInstanceED( int iED ) { m_iED = iED; }
#endif	REAL_TIME_ELSWORD
	//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ED ȹ�� ����

	bool	GetShowCSPandSlotBDate() const { return m_bShowCSPandSlotBDate; }
	void	SetShowCSPandSlotBDate( bool bShow ) { m_bShowCSPandSlotBDate = bShow; }

#ifdef CHANGE_CHANNEL
	bool	IsChangeChannel() { return m_bIsChangeChannel; }
	void	SetChangeChannel(bool val) { m_bIsChangeChannel = val; }
#endif
#ifdef SERV_KOG_OTP_VERIFY
	bool	IsIDVerified(){ return m_bIDVerified; }
	void	SetIDVerified(bool val){ m_bIDVerified = val; }
#endif SERV_KOG_OTP_VERIFY

	//{{ 2010.12.03 ��ȿ��	ĳ�ü� ��ū ��Ʈ�� ����
#ifdef SERV_PURCHASE_TOKEN
	std::wstring	GetPurchaseToken(){ return m_wstrPurchaseTok; }
	void			SetPurchaseToken(std::wstring val){ m_wstrPurchaseTok = val; }
#endif SERV_PURCHASE_TOKEN
	//}}

#ifdef CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH
	bool GetInHouseAccount() { return m_bInHouseAccount; }
	void SetInHouseAccount( bool bInHouseAccount ) { m_bInHouseAccount = bInHouseAccount; }
#endif // CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH

#ifdef SERVER_NAME
	const wstring		GetServerGroupName();
#endif SERVER_NAME

	// 2009-07-29 // kimhc // ��ϸ� �ð��� ��ŷ�� ��ϵǾ��ٴ� �˸��� Ȯ���ϴ� bool��
	bool	IsRegisteredRank() const { return m_bRegisteredRank; }
	void	SetRegisteredRank( bool bRegistered ) { m_bRegisteredRank = bRegistered; }

	//{{ kimhc // �ڵ��������� �ֱ� �����ų� ���� ���������� ���������� �Ǵ� �ϴ� ���� �� �Լ� �߰�
#ifdef	AUTO_PAYMENT
	void	SetWebType( BYTE byWebType ) { m_byWebType = byWebType; } // �ֱ� �����ų� ���� ���������� �������� ����
	BYTE	GetWebType() const { return m_byWebType; }	// �ֱ� �����ų� ���� ���������� �������� �˼� �ִ� �Լ�
#endif	AUTO_PAYMENT
	//}} kimhc // �ڵ��������� �ֱ� �����ų� ���� ���������� ���������� �Ǵ� �ϴ� ���� �� �Լ� �߰�

	//{{ kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�
#ifdef	ADD_SERVER_GROUP

#ifdef SHOW_SERVERGROUP_NAME
	void				RefreshTitle();
#endif SHOW_SERVERGROUP_NAME

#ifdef EXTEND_SERVER_GROUP_MASK
	void				SetServerGroupID( int iServerGroupID );
	int					GetServerGroupID() const { return m_iServerGroupID; }
	void				SetServerGroupIDCashShop( int iServerGroupID ) { m_iServerGroupIDCashShop = iServerGroupID; }
	int					GetServerGroupIDCashShop() const { return m_iServerGroupIDCashShop; }
//#ifndef CHANNEL_CONGESTION_SCOPE_NO_CHEAT
	void				SetChannelDistributionByServerGroup( OUT float* pfChCongestionScopeTemp, IN const float* pfChCongestionScope, IN int iChannelIndex );
//#endif // CHANNEL_CONGESTION_SCOPE_NO_CHEAT
	void				SetServerGroupIDScript( int iServerGroupID ) { m_iServerGroupIDScript = iServerGroupID; }
	int					GetServerGroupIDScript() const { return m_iServerGroupIDScript; }
#else
	void				SetServerGroupID( SERVER_GROUP_ID eServerGroupID );
	SERVER_GROUP_ID		GetServerGroupID() const { return m_eServerGroupID; }
	void				SetServerGroupIDCashShop( SERVER_GROUP_ID eServerGroupID ) { m_eServerGroupIDCashShop = eServerGroupID; }
	SERVER_GROUP_ID		GetServerGroupIDCashShop() const { return m_eServerGroupIDCashShop; }
	void				SetChannelDistributionByServerGroup( OUT float* pfChCongestionScopeTemp, IN const float* pfChCongestionScope, IN int iChannelIndex );

	void				SetServerGroupIDScript( SERVER_GROUP_ID eServerGroupID ) { m_eServerGroupIDScript = eServerGroupID; }
	SERVER_GROUP_ID		GetServerGroupIDScript() const { return m_eServerGroupIDScript; }
#endif // EXTEND_SERVER_GROUP_MASK
#endif	ADD_SERVER_GROUP
	//}}  kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�

#ifdef SERV_STEAM
	// ä�� ���� ���� ���� �Ѿ�� ä�θ� �ڵ带 �����صд�
	void				SetChannelingCode( IN int iChannelingCode_ ) { m_iChannelingCode = iChannelingCode_; }
	int					GetChannelingCode() const { return m_iChannelingCode; }
#ifdef SERV_CHANNELING_AERIA
	const bool			IsAeriaChanneling() const { return ((m_iChannelingCode == 6) || (m_iChannelingCode == 7)); }
#endif //SERV_CHANNELING_AERIA
#endif //SERV_STEAM

	CKTDXTimer& GetTimerPacketTiming() { return m_TimerPacketTiming; }
	bool IsElapsedPacketTiming( float fTime = 1.f ) { return m_TimerPacketTiming.elapsed() > fTime; }

#ifdef DAMAGE_HISTORY
	void SetSendHackUser(bool bVal) { m_bSendedHackUser = bVal; }
	bool GetSendHackUser() { return m_bSendedHackUser; }
	void SetSendHackMail(bool bVal) { m_bSendedHackMail = bVal; }
	bool GetSendHackMail() { return m_bSendedHackMail; }
#endif

#ifdef SERV_PROTECT_AUTO_HACK
	void SetSendHackMail_AutoHack(bool bVal) { m_bSendedHackMailAutoHack = bVal; }
	bool GetSendHackMail_AutoHack() { return m_bSendedHackMailAutoHack; }
		
#endif

#if defined( SERV_HACKING_TOOL_LIST )

#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX

    void    SetHackList_MainThread( const std::vector<KHackingToolInfo>& vecHackList )
    {
        m_vecHackList_MainThread = vecHackList;
        ReSetHackList_MainThread();
    }
    void    ReSetHackList_MainThread()
    {
        std::vector<KHackingToolInfo> vecTemp = m_vecHackList_MainThread;
        {
            CSLock  lock(m_csHackList);
            m_vecHackList.swap( vecTemp );
            m_bChangeHacklist = true;
        }
    }
    bool    GetChangedHackList_ThreadSafe( OUT std::vector<KHackingToolInfo>& vecHackList )
    {
        bool    bChangeHackList = false;
        vecHackList.resize( 0 );
        if ( m_bChangeHacklist == true )
        {
            CSLock  lock(m_csHackList);
            bChangeHackList = m_bChangeHacklist;
            vecHackList.swap( m_vecHackList );
            m_bChangeHacklist = false;
        }
        if ( bChangeHackList == false )
        {
            vecHackList.resize( 0 );
        }
        return bChangeHackList;
    }
    bool    VerifyChangeHackList_ThreadSafe()
    {
        CSLock  lock(m_csHackList);
        return  m_bChangeHacklist.Verify();
    }
	int     GetHackListSize_MainThread() { return (int)m_vecHackList_MainThread.size(); }

#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX

	void ClearHackList() { m_vecHackList.clear(); }
	void PushHackList(KHackingToolInfo hackInfo) { m_vecHackList.push_back(hackInfo); }
	int GetHackListSize() { return (int)m_vecHackList.size(); }
	KHackingToolInfo GetHackInfo(int i) 
	{ 
		if(i >= 0 && i < (int)m_vecHackList.size() )
			return m_vecHackList[i];

		KHackingToolInfo ret;
		ret.m_cFlag = 0;
		return ret;
	}

	void SetChangeHackList(bool bVal) 
	{ 	
		m_bChangeHacklist = bVal; 		
	}
	bool GetChangeHackList() { return m_bChangeHacklist; }

#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX

#endif

//{{ kimhc // 2010.3.26 // ���� ��ų ���� ����
#ifdef MODIFY_INFINITE_SKILL_BUG
	// STATE_CHANGE_GAME_START_NOT�� �޾Ҵ°�?
	void SetReceiveGameStartNotPacket( bool bValue ) { m_bReceiveGameStartNotPacket = bValue; }		
	bool GetReceiveGameStartNotPacket() const { return m_bReceiveGameStartNotPacket; }
	// STATE_CHANGE_GAME_START_REQ�� �����ߴ°�?
	void SetSendGameStartReqPacket( bool bValue ) { m_bSendGameStartReqPacket = bValue; }		
	bool GetSendGameStartReqPacket() const { return m_bSendGameStartReqPacket; }
#endif MODIFY_INFINITE_SKILL_BUG
//}} kimhc // 2010.3.26 // ���� ��ų ���� ����
	
	//{{ kimhc // 2010.4.9 // ��д��� �۾�(��������Ʈ �ý���)
#ifdef SERV_SECRET_HELL
	void SetPreCheckTimeForDailyQuest( __time64_t t ) { m_t64PreCheckedTimeForDailyQuest = t; }
	const __time64_t GetPreCheckTimeForDailyQuest() const { return m_t64PreCheckedTimeForDailyQuest; }
#endif SERV_SECRET_HELL
	//}} kimhc // 2010.4.9 // ��д��� �۾�(��������Ʈ �ý���)

#ifdef VERIFY_NPC_HP
	bool GetVerifyNpcHp() { return m_bVerifyNpcHp; }
	void SetVerifyNpcHp(bool bVal) { m_bVerifyNpcHp = bVal; }
#endif

	float GetVerifyGageManagerTimer() { return m_fRemainedTimeByForceQuitGame; }
	void  SetRemainedTimeByForceQuitGame(float fVal) { m_fRemainedTimeByForceQuitGame = fVal; }

#ifdef ADD_KPROTECTEDTYPE_VALUE
	int GetVerifyNpcStateIdCnt() { return m_nVerifyNpcStateId; }
	void SetVerifyNpcStateIdCnt( int iVal ) { m_nVerifyNpcStateId = iVal; }
#endif

#ifdef SERV_DLL_MANAGER
	void SetSendedModuleList(bool bVal) { m_bSendedLoadedModuleList = bVal; }
	bool GetSendedModuleList() { return m_bSendedLoadedModuleList; }
#endif

#ifdef SERV_PSHOP_AGENCY	
	wstring GetAgencyShopExpirationDate() { return m_wstrAgencyShopExpirationDate; }
	bool GetIsPShopOpen() { return m_bIsPShopOpen; }

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	const SEnum::AGENCY_SHOP_TYPE GetAgencyShopType() { return m_eAgencyShopType; }

	void SetPShopAgencyInfo( IN const bool bIsPShopOpen_, IN const wstring wstrExpirationDate_, IN const SEnum::AGENCY_SHOP_TYPE eAgencyShopType_)
	{	
		m_bIsPShopOpen					= bIsPShopOpen_;
		m_wstrAgencyShopExpirationDate	= wstrExpirationDate_;
		m_eAgencyShopType				= eAgencyShopType_;
	}
#else //SERV_UPGRADE_TRADE_SYSTEM
	void SetPShopAgencyInfo(bool bIsPShopOpen, wstring wstrExpirationDate)
	{	
		m_bIsPShopOpen = bIsPShopOpen;
		m_wstrAgencyShopExpirationDate = wstrExpirationDate;
	}
#endif //SERV_UPGRADE_TRADE_SYSTEM
	
	bool IsActiveAgencyShop() 
	{
		if( m_wstrAgencyShopExpirationDate.empty() )
			return false;

		CTime	tCurrentTime	= CTime::GetCurrentTime();
		CTime	cAgencyTime;
		KncUtil::ConvertStringToCTime( m_wstrAgencyShopExpirationDate, cAgencyTime );
		if( tCurrentTime < cAgencyTime )
		{
			return true;
		}
		return false;
	}
#endif

#ifdef MACHINE_ID
	void SetMachineId(std::string &strMachineId) { m_strMachineId = strMachineId; }
	std::string &GetMachineId() { return m_strMachineId; }
#endif

#ifdef SERV_EPAY_SYSTEM
	int GetGiantZone() const						{ return m_usGiantZone; }
	int GetGiantGame() const						{ return m_usGiantGame; }
	void SetGiantZone(unsigned short usGiantZone)			{ m_usGiantZone = usGiantZone; }
	void SetGiantGame(unsigned short usGiantGame)				{ m_usGiantGame = usGiantGame; }
#endif //SERV_EPAY_SYSTEM

#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	int GetChannelKey() { return m_iChannelRandomKey; }
	void SetChannelKey(int iVal) { m_iChannelRandomKey = iVal; }
#endif

#ifdef SERV_ED_MONITORING_IN_GAME
	bool GetMoneyMonitoring() { return m_bMoneyMonitoring; }
	void SetMoneyMonitoring(bool bVal) { m_bMoneyMonitoring = bVal; }
#endif

#ifdef UDP_CAN_NOT_SEND_USER_KICK
	bool GetUDPKickOff() { return m_bSetUDPKickOff; }
	void SetUDPKickOff( bool _bSetUDPKickOff ) { m_bSetUDPKickOff = _bSetUDPKickOff; }
#endif //UDP_CAN_NOT_SEND_USER_KICK

#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK	
	unsigned char *GetSN() { return m_cServerSN; }
	unsigned char *GetOldSN() { return m_cOldServerSN; }
	void SetSN( unsigned char *cSn ) 
	{
		memcpy(m_cServerSN, cSn, sizeof(unsigned char) * SERVER_SN);

		if( cSn != NULL )
		{
			std::string strSn = base64_encode(cSn, SERVER_SN);
			ErrorLogMsg( XEM_ERROR1, strSn.c_str() );
		}
		else
		{
			ErrorLogMsg( XEM_ERROR1, "SNULL" );
		}
	}
	void SetOldSn( unsigned char *cSn ) 
	{
		memcpy(m_cOldServerSN, cSn, sizeof(unsigned char) * SERVER_SN);

		if( cSn != NULL )
		{
			std::string strSn = base64_encode(cSn, SERVER_SN);
			ErrorLogMsg( XEM_ERROR1, strSn.c_str() );
		}
	}	
#endif

	CKTDXCheckElapsedTime&		GetTimerForSendingPlayStatus() { return m_TimerForSendingPlayStatus; }

#ifdef	USE_ACCOUNT_DOMAIN
	void SetDomain( ACCOUNT_DOMAIN_TYPE eDomain ) { m_eDomain = eDomain; }
	ACCOUNT_DOMAIN_TYPE GetDomain() { return m_eDomain; }
	std::wstring GetDomainName() { return ACCOUNT_DOMAIN_NAME[ static_cast<int>(m_eDomain) ]; }
#endif	USE_ACCOUNT_DOMAIN

#ifdef SERV_NEW_EVENT_TYPES
	void SetMaxLevel( int iMaxLevel ) { m_iMaxLevel = iMaxLevel; }
	int GetMaxLevel() { return m_iMaxLevel; }
#endif SERV_NEW_EVENT_TYPES

#ifdef SERV_CODE_EVENT
	void SetCodeEvent( std::map< int, bool >& mapEnableCodeEnum ) { m_mapEnableCodeEnum = mapEnableCodeEnum; }
	bool IsEnableCode( IN int iEventID )
	{
		std::map< int, bool >::iterator mitEnableCodeEnum = m_mapEnableCodeEnum.find( iEventID );
		
		if( mitEnableCodeEnum == m_mapEnableCodeEnum.end() )
			return false;

		return mitEnableCodeEnum->second;
	}
#endif SERV_CODE_EVENT

#ifdef CLIENT_COUNTRY_ID
	CNMRunParam& GetNetMarbleRunParam() { return m_kNMRunParam; }
#endif //AUTH_CJ_ID

#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
	void SetNowSubCategoryList( IN const vector< CX2CashShop::CashShopCategory* >& m_vecCashShopCateList, IN const int iCategory_ );
	bool IsCurrentSubCategoryInNowCatagory( IN const int iSubCategory_ );
	vector< KBillProductInfo > GetCurrentProductInfoListInNowCatagory( vector< KBillProductInfo >& vecBillInfo_ );
	void SetChoicedItem(IN bool bVal_ );
	bool IsChoicedItem();
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2

#ifdef CLIENT_PORT_CHANGE_REQUEST
	void SetStartPortChangeRequest( bool _bStart ) { m_bStartPortChangeRequest = _bStart;  }
	bool GetStartPortChangeRequest() { return m_bStartPortChangeRequest; }

	void SetUDPPortSuccessType( int _itype ) { m_iUDPPortSuccessType = _itype; }
	int GetUDPPortSuccessType() { return m_iUDPPortSuccessType; }
#endif //CLIENT_PORT_CHANGE_REQUEST

#ifdef SERV_ID_NETMARBLE_PCBANG
	void SetPublicIPCheck( bool _bPublicIP  ) { m_bPublicIP = _bPublicIP; }
	bool GetPublicIPCheck() { return m_bPublicIP; }

	void SetPublicIP( std::wstring _wstrPublicIP  ) { m_wstrPublicIP = _wstrPublicIP; }
	std::wstring GetPublicIP() { return m_wstrPublicIP; }

	void SetMacAdress( std::wstring _wstrMacAdress  ) { m_wstrMacAdress = _wstrMacAdress; }
	std::wstring GetMacAdress() { return m_wstrMacAdress; }
#endif //SERV_ID_NETMARBLE_PCBANG

#ifdef SERV_COUNTRY_PH
	void SetAuthToken( std::string strtoken ) { m_strAuthToken = strtoken; }
	std::string GetAuthToken() { return m_strAuthToken; }
#endif //SERV_COUNTRY_PH

#ifdef SERV_FIELD_WORKINGS_BLOCK_LOG
	void SetNowX2StateToBlockLog( const char cX2State_ ) { m_packetFieldWorkingsBlockLog.m_cX2State = cX2State_; }
	void SetPhysicalMemoryToBlockLog( const int iPhysicalMemory_ ) { m_packetFieldWorkingsBlockLog.m_iPhysicalMemory = iPhysicalMemory_; }
	void SetVirtualMemoryToBlockLog( const int iVirtualMemory_ ) { m_packetFieldWorkingsBlockLog.m_iVirtualMemory = iVirtualMemory_; }
	void SetProcessTerm( const char cProcessTerm ) { m_packetFieldWorkingsBlockLog.m_cProcessTerm = cProcessTerm; }	/// JoinBattleFieldReq�� ������ ���� �� GameLoadingReq�� ���� �Ҷ� ���� �ɸ� �ð�(��)
	void SetFieldUserNum( const char cFieldUserNum_ ) { m_packetFieldWorkingsBlockLog.m_cFieldUserNum = cFieldUserNum_; }
	void SetFieldNpcNum( const char cFieldNpcNum_ ) { m_packetFieldWorkingsBlockLog.m_cFieldNpcNum = cFieldNpcNum_; }
	void InitFieldWorkingSetBlockLog()
	{
		SetNowX2StateToBlockLog( 0 );
		SetPhysicalMemoryToBlockLog( 0 );
		SetVirtualMemoryToBlockLog( 0 );
		SetProcessTerm( 0 );
		SetFieldUserNum( 0 );
		SetFieldNpcNum( 0 );
	}

	void RestartTimerFromJoinBattleToGameLoading() { m_timerFromJoinBattleToGameLoading.restart(); }
	double GetElapsedTimerFromJoinBattleToGameLoading() { return m_timerFromJoinBattleToGameLoading.elapsed(); }

	const KEGS_FIELD_WORKINGS_BLOCK_LOG_ACK& GetFieldWorkingsBlockLog() const { return m_packetFieldWorkingsBlockLog; }	
#endif // SERV_FIELD_WORKINGS_BLOCK_LOG

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	wstring GetChannelButtonNameByChannelName( wstring wstrChannelName );
	void AddUserUnitDataInServer( SEnum::SERVER_GROUP_ID eServerGroupID, int iNowUserUnitManyInServer, int iMaxUserUnitManyInServer );
	void ClearUserUnitDataInServer ();
	int	GetMaxUserUnitManyInServer ( SEnum::SERVER_GROUP_ID eServerGroupID );
	int	GetNowUserUnitManyInServer ( SEnum::SERVER_GROUP_ID eServerGroupID );	

#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifdef SERV_NAVER_CHANNELING
	const std::string& GetNaverAccessToken() const { return m_strNaverAccessToken; }
	void SetNaverAccessToken( const std::string& val ) { m_strNaverAccessToken = val; }
#endif // SERV_NAVER_CHANNELING

#ifdef	ADD_SERVER_GROUP
//{{ robobeg : 2013-12-19
    // ���� ���� �ߺ��Ǿ� �ִ� �ڵ带 �� ������ �����ϴ�.
    bool OpenScriptServerGroupFile();			// ������ �÷��� �ߴ� ������ �б�
    bool SaveScriptServerGroupFile();
//}} robobeg : 2013-12-19
#endif  ADD_SERVER_GROUP


#ifdef FIX_REFORM_ENTRY_POINT_10TH		//	kimjh,  ĳ���� ����Ʈ �������� ������ ����
	bool IsConnectedChannel ( int iChannelID_ );
	void SetConnectedChannelID ( int iChannelID_ );
	void ResetConnectedChannelID ();
#endif // FIX_REFORM_ENTRY_POINT_10TH	//	kimjh,  ĳ���� ����Ʈ �������� ������ ����

#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
	bool IsRecruit() { return m_bRecruit; }
	void SetRecruit( bool bRecruit ) { m_bRecruit = bRecruit; }
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
	void SetChungUIShow(bool bShow) 			{ m_bUIChungShow = bShow; }
	bool GetChungUIShow(void)					{ return m_bUIChungShow; }
	void SetChungUIClass(char cChungClass)		{ m_cGetCharClass = cChungClass; }
	char GetChungUIClass(void)					{ return m_cGetCharClass; }
	void SetNextGiveItem(bool bNext)			{ m_bNextGiveItem = bNext; }
	bool GetNextGiveItem(void)					{ return m_bNextGiveItem; }
	void SetToolTipTime(std::wstring Temptime)	{ m_wstrTootipTiptime = Temptime; }
	std::wstring GetToolTipTime(void)			{ return m_wstrTootipTiptime; }
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
public:
	void SetStartUI( bool TempStart )				{ m_bStartUI = TempStart; }
	bool GetStartUI( void ) 						{ return m_bStartUI; }
	void SetDungeonCountUI(bool TempDungeon)		{ m_DungeonCountUI = TempDungeon; }
	bool GetDungeonCountUI(void) 					{ return m_DungeonCountUI; }
	void SetFieldCountUI(bool TempField)			{ m_FieldCountUI = TempField; }
	bool GetFieldCountUI(void)						{ return m_FieldCountUI; }
	void SetDungeonCount(int TempCount)				{ m_DungeonCount = TempCount; }
	int GetDungeonCount(void)						{ return m_DungeonCount; }
	void SetFieldCount(int TempFieldCount)			{ m_FieldMonsterKillCount = TempFieldCount; }
	int GetFieldCount(void)							{ return m_FieldMonsterKillCount; }
	void SetRemaindTime(int TempTime)				{ m_iRemaindTime = TempTime; }
	int GetRemaindTime(void)						{ return m_iRemaindTime; }
	void SetSecondTime(int SecondTime)				{ m_iTimeSecond = SecondTime; }
	int GetSecondTime(void)							{ return m_iTimeSecond; }
	void SetButtonPushTime(__time64_t TempPushTime)	{ m_tButtonPushTime = TempPushTime; }
	__time64_t GetButtonPushTime(void)				{ return m_tButtonPushTime; }
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	void SetAdamsEventShopUIShow(bool bShow)		{ m_bAdamsShopShow = bShow; }
	bool GetAdamsEventShopUIShow(void)				{ return m_bAdamsShopShow; }
#endif ALWAYS_EVENT_ADAMS_UI_SHOP

#ifdef ALWAYS_SCREEN_SHOT_TEST
	void SetScreenShotTest( bool bTest )			{ m_bScreenShotTest = bTest; }
	bool GetScreenShotTest(void)					{ return m_bScreenShotTest; }
#endif ALWAYS_SCREEN_SHOT_TEST

#ifdef SERV_ELISIS_PREVIOUS_SIS_EVENT
		void SetPreEventElesisUID( UidType iVal_ ){ m_iPreEventElesisUID = iVal_; }
		const UidType GetPreEventElesisUID() const { return m_iPreEventElesisUID; }
#endif // SERV_ELISIS_PREVIOUS_SIS_EVENT

#ifdef SERV_4TH_ANNIVERSARY_EVENT
	void SetK4ThAnnivEventInfo( IN const K4ThAnnivEventInfo& k4ThAnnivEventInfo )
	{
		m_k4ThAnnivEventInfo = k4ThAnnivEventInfo;
	}
	void GetK4ThAnnivEventInfo( OUT K4ThAnnivEventInfo& k4ThAnnivEventInfo )
	{
		k4ThAnnivEventInfo = m_k4ThAnnivEventInfo;
	}
	void Set4ThAnnivEventRewardInfo( IN const std::vector< bool >& vec4ThAnnivEventRewardInfo )
	{
		m_vec4ThAnnivEventRewardInfo = vec4ThAnnivEventRewardInfo;
	}
	void Set4thRewarded( IN int iIndex )
	{
		if( m_vec4ThAnnivEventRewardInfo.size() <= iIndex )
			m_vec4ThAnnivEventRewardInfo.resize( iIndex + 1 );

		m_vec4ThAnnivEventRewardInfo[iIndex] = true;
	}
	bool Is4thRewarded( IN int iIndex )
	{
		if( iIndex >= m_vec4ThAnnivEventRewardInfo.size() || iIndex < 0 )
			return false;

		return m_vec4ThAnnivEventRewardInfo[iIndex];
	}
#endif //SERV_4TH_ANNIVERSARY_EVENT
private:
	CX2MiniMapUI*	m_pMiniMapUI;


	//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ED ȹ�� ����
#ifdef	REAL_TIME_ELSWORD
	int		m_iED;		// ���� ��Ż�� ED�� �г�Ƽ�� ����� �޴� ȿ���� �����ֱ����� �뵵�� ����
#endif	REAL_TIME_ELSWORD
	//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ED ȹ�� ����

	bool	m_bShowCSPandSlotBDate;		// kimhc // ĳ���� ���� �� �׳�ý��� �ູ�� ��ų����B�� ��� �˸�

	bool m_bIsDungeonLounge;
	bool m_bLoadedDungeonLoungeUnits;
	std::vector< KFieldUserInfo > m_vecDungeonLoungeUnitInfo;

	CX2World::WORLD_ID m_eDungeonLoungeWorldID;
	D3DXVECTOR3 m_vLastDungeonGatePos;				// ���� ��������� ���� ����Ʈ�� �������� �� ����� ��ġ 
	int			m_iLastDungeonGatePosLineIndex;		// ���� ��������� ���� ����Ʈ�� �������� �� ����� ��ġ�� line index
	
	wstring m_UserID;				// �α��ν� id/password
#ifdef SERV_CHANNELING_AERIA
	wstring m_UserSessionID;				// �α��ν� Aeria Games ���� �Ѱ��� Sesion ID
#endif //SERV_CHANNELING_AERIA
	wstring m_UserPassword;
#ifdef CLIENT_COUNTRY_TH
	wstring m_wstrSocketID;
#endif // CLIENT_COUNTRY_TH

#ifdef SERV_KOG_OTP_VERIFY
	bool	m_bIDVerified;
#endif SERV_KOG_OTP_VERIFY

#ifdef SERV_PURCHASE_TOKEN
	std::wstring m_wstrPurchaseTok;
#endif SERV_PURCHASE_TOKEN

#ifdef CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH
	bool m_bInHouseAccount;
#endif // CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH

	int		m_iConnectChannelServerID;		// ���� ����� ���Ӽ���, �� ä���� ID

	bool	m_bFirstJoinVillage;					// ĳ���� ����â���� ������ ����



#ifdef ITEM_CHEAT_POPUP_TEST

	CX2ItemCheatPopUp m_ItemCheatPopup;

#endif ITEM_CHEAT_POPUP_TEST





#if defined( _IN_HOUSE_ ) || defined( _OPEN_TEST_ ) || defined( _OPEN_TEST_2_ )
	bool	m_bFrameScale;
#endif

	//{{ kimhc // �ڵ��������� �ֱ� �����ų� ���� ���������� ���������� �Ǵ� �ϴ� ���� �� �Լ� �߰�
#ifdef	AUTO_PAYMENT
	BYTE	m_byWebType;	// �ֱ� �����ų� ���� ���� ����
#endif	AUTO_PAYMENT
	//}} kimhc // �ڵ��������� �ֱ� �����ų� ���� ���������� ���������� �Ǵ� �ϴ� ���� �� �Լ� �߰�

#ifdef CHANGE_CHANNEL
	bool	m_bIsChangeChannel;

	bool	m_bRegisteredRank;

public:
	std::vector< KChannelInfo > m_vecChannelInfo;

#ifdef SERV_CHANNEL_LIST_RENEWAL
	std::map< int, KChannelBonusInfo > m_mapChannelBonusInfo;
#endif //SERV_CHANNEL_LIST_RENEWAL

#endif CHANGE_CHANNEL


	//{{ kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�
#ifdef	ADD_SERVER_GROUP
#ifdef EXTEND_SERVER_GROUP_MASK
	int					m_iServerGroupID;	
	int					m_iServerGroupIDCashShop;	// ���� ������ �ִ� ĳ�þ����� ����Ʈ�� �޾ƿ� ������ ID
	int					m_iServerGroupIDScript;
#else
	SERVER_GROUP_ID		m_eServerGroupID;	
	SERVER_GROUP_ID		m_eServerGroupIDCashShop;	// ���� ������ �ִ� ĳ�þ����� ����Ʈ�� �޾ƿ� ������ ID
	SERVER_GROUP_ID		m_eServerGroupIDScript;
#endif EXTEND_SERVER_GROUP_MASK
#endif	ADD_SERVER_GROUP
	//}}  kimhc // 2009-12-15 // ������ �߰� �۾��� ���� ä�� ���̾�α� �и�

#ifdef SERVER_GROUP_UI_ADVANCED
	std::map< int, KServerGroupInfo > m_mapServerGroupInfo;
#endif SERVER_GROUP_UI_ADVANCED

#ifdef SERVER_NAME
#ifdef EXTEND_SERVER_GROUP_MASK
	std::map<int, std::wstring>					m_mapServerGroupName;
#else
	std::map<SERVER_GROUP_ID, std::wstring>		m_mapServerGroupName;
#endif // EXTEND_SERVER_GROUP_MASK
#endif SERVER_NAME

#ifdef SERV_EPAY_SYSTEM
	unsigned short						m_usGiantZone;
	unsigned short						m_usGiantGame;
#endif //SERV_EPAY_SYSTEM
	CKTDXTimer m_TimerPacketTiming;		// Ư�� ��Ŷ���� �����ð� �������� ���޾� �������� �� ������ ����� ���� �������ؼ� �� Ÿ�̸Ӹ� �����ؼ� ��Ŷ�� �����ų� �ش� ����� �������� ������ �����Ѵ�
	
#ifdef DAMAGE_HISTORY
	bool m_bSendedHackUser;
	bool m_bSendedHackMail;
#endif
#ifdef SERV_PROTECT_AUTO_HACK
	bool m_bSendedHackMailAutoHack;
#endif
	bool m_bFirstSelect;		// kimhc // �α��� �� ó�� ĳ���� �����ΰ�? // PC �� �κ��丮 ����Ʈ �Ǽ��� �۾�

#if defined( SERV_HACKING_TOOL_LIST )
private:
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
    std::vector<KHackingToolInfo>   m_vecHackList_MainThread;
    MemberCriticalSection           m_csHackList;
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
	std::vector<KHackingToolInfo>   m_vecHackList;
	KProtectedType<bool>            m_bChangeHacklist;
public:
#endif

	//{{ kimhc // 2010-03-23 // ���� ��ų ���� ����
	bool m_bReceiveGameStartNotPacket;		// STATE_CHANGE_GAME_START_NOT�� �޾Ҵ°�?
	bool m_bSendGameStartReqPacket;			// STATE_CHANGE_GAME_START_REQ�� �����ߴ°�?
	//}} kimhc // 2010-03-23 // ���� ��ų ���� ����

	//{{ kimhc // 2010.4.9 // ��д��� �۾�(��������Ʈ �ý���)
#ifdef SERV_SECRET_HELL
	__time64_t		m_t64PreCheckedTimeForDailyQuest;		// ������ üũ�� �ð� (��������Ʈ��)
#endif SERV_SECRET_HELL
	//}} kimhc // 2010.4.9 // ��д��� �۾�(��������Ʈ �ý���)

#ifdef VERIFY_NPC_HP
	bool m_bVerifyNpcHp;
#endif

	//{{ kimhc // 2010-06-24 // OTP �۾�
#ifdef SERV_OTP_AUTH
	wstring m_wstrNxHWID;		// �ϵ���� ID
#endif	SERV_OTP_AUTH
	//}} kimhc // 2010-06-24 // OTP �۾�

	KProtectedType<float>	m_fRemainedTimeByForceQuitGame;

#ifdef ADD_KPROTECTEDTYPE_VALUE
	int		m_nVerifyNpcStateId;
#endif

#ifdef SERV_DLL_MANAGER
	KProtectedType<bool>	m_bSendedLoadedModuleList;
#endif

#ifdef SERV_PSHOP_AGENCY	
	bool						m_bIsPShopOpen;
	wstring						m_wstrAgencyShopExpirationDate;
#endif

#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
	SEnum::AGENCY_SHOP_TYPE		m_eAgencyShopType;		/// �븮 ���� Ÿ�� ( Free, ED, Cash )
#endif //SERV_UPGRADE_TRADE_SYSTEM

#ifdef MACHINE_ID
	std::string					m_strMachineId;
#endif

#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	KProtectedType<int>			m_iChannelRandomKey;
#endif
#ifdef SERV_ED_MONITORING_IN_GAME
	KProtectedType<bool>		m_bMoneyMonitoring;
#endif

#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	unsigned char m_cServerSN[SERVER_SN];
	unsigned char m_cOldServerSN[SERVER_SN];
#endif

#ifdef ADD_COLLECT_CLIENT_INFO
	unsigned int g_AntiHackingList[ANTIHACKING_ID::ANTIHACKING_ID_END];
#endif ADD_COLLECT_CLIENT_INFO

	CKTDXCheckElapsedTime		m_TimerForSendingPlayStatus;	/// �ڽ��� HP, MP �� �÷��� ����Ÿ�� ������ �����ϴ� ����	

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
	// Key, pair < ���� ���� ��, �ִ� ���� ������ ���� �� > 
	map < SEnum::SERVER_GROUP_ID, pair< int, int > > m_mapUserUnitDataInServer;
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifdef UDP_CAN_NOT_SEND_USER_KICK
	std::map< UidType, boost::timer >		m_mapCheckUDPTimer;
	std::map< UidType, __int64 >			m_mapCheckUDPTDelayCount;
	bool									m_bSetUDPKickOff;
	//#ifdef UDP_DOWNLOAD_BLOCK_CHECK
	//std::set< UidType >						m_setAbuseUnitUid;	//�Ű� ������� UID
	//#endif //UDP_DOWNLOAD_BLOCK_CHECK
#endif//UDP_CAN_NOT_SEND_USER_KICK

#ifdef LOG_UDP_CHECK
	//�α׸� ���� �ӽ÷� UDPüũ �ϴ� ���� 1�� �� �߰�.
	std::map< UidType, boost::timer >		m_mapCheckUDPTimerTemp;
	std::map< UidType, __int64 >			m_mapCheckUDPTDelayCountTemp;
#endif //LOG_UDP_CHECK
#ifdef	USE_ACCOUNT_DOMAIN
	ACCOUNT_DOMAIN_TYPE			m_eDomain;
#endif	USE_ACCOUNT_DOMAIN

#ifdef SERV_NEW_EVENT_TYPES
	int							m_iMaxLevel;
#endif SERV_NEW_EVENT_TYPES

#ifdef SERV_CODE_EVENT
	std::map< int, bool >		m_mapEnableCodeEnum;
#endif SERV_CODE_EVENT

#ifdef SERV_ELISIS_PREVIOUS_SIS_EVENT
	UidType					m_iPreEventElesisUID;	// �����ý� ���� �Ϸ� �ߴ���?
#endif // SERV_ELISIS_PREVIOUS_SIS_EVENT

#ifdef X2TOOL
	public:
		bool m_bIsNpcLoad;
		bool m_bCanDungeonPlay;
		bool m_bChangeStage;
#endif X2TOOL

#ifdef CLIENT_COUNTRY_ID
	CNMRunParam m_kNMRunParam;
#endif //AUTH_CJ_ID

#ifdef ADD_CASH_SHOP_CATEGORY_EVENT_2
	vector< D3DXVECTOR2 >		m_vecNowSubCategoryList;
	bool						m_bChoicedItem;
#endif //ADD_CASH_SHOP_CATEGORY_EVENT_2

#ifdef CLIENT_PORT_CHANGE_REQUEST
	int							m_iUDPPortSuccessType;
	bool						m_bStartPortChangeRequest;
#endif //CLIENT_PORT_CHANGE_REQUEST

#ifdef SERV_ID_NETMARBLE_PCBANG
	bool						m_bPublicIP;
	std::wstring				m_wstrPublicIP;
	std::wstring				m_wstrMacAdress;
#endif //SERV_ID_NETMARBLE_PCBANG

#ifdef SERV_COUNTRY_PH
	std::string					m_strAuthToken;
#endif //SERV_COUNTRY_PH

#ifdef SERV_FIELD_WORKINGS_BLOCK_LOG
	KEGS_FIELD_WORKINGS_BLOCK_LOG_ACK	m_packetFieldWorkingsBlockLog;
	CKTDXTimer							m_timerFromJoinBattleToGameLoading;
#endif // SERV_FIELD_WORKINGS_BLOCK_LOG

#ifdef SERV_NAVER_CHANNELING
		std::string							m_strNaverAccessToken;
#endif // SERV_NAVER_CHANNELING

#ifdef FIX_REFORM_ENTRY_POINT_10TH		//	kimjh,  ĳ���� ����Ʈ �������� ������ ����
		vector<int>							m_vecTryConnectChannelID;
#endif // FIX_REFORM_ENTRY_POINT_10TH	//	kimjh,  ĳ���� ����Ʈ �������� ������ ����

#ifdef SERV_STEAM
	int							m_iChannelingCode;
#endif //SERV_STEAM

#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
	bool						m_bRecruit;
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
	bool						m_bUIChungShow;
	char						m_cGetCharClass;
	bool						m_bNextGiveItem;
	std::wstring				m_wstrTootipTiptime;
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		bool								m_bStartUI;
		bool								m_DungeonCountUI;
		bool								m_FieldCountUI;
		int									m_DungeonCount;
		int									m_FieldMonsterKillCount;
		int									m_iRemaindTime;
		__time64_t							m_tButtonPushTime;
		int									m_iTimeSecond;///�ʴ��� ���
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
		bool								m_bAdamsShopShow;
#endif ALWAYS_EVENT_ADAMS_UI_SHOP

#ifdef ALWAYS_SCREEN_SHOT_TEST
		bool								m_bScreenShotTest;
#endif ALWAYS_SCREEN_SHOT_TEST

#ifdef SERV_4TH_ANNIVERSARY_EVENT
		K4ThAnnivEventInfo					m_k4ThAnnivEventInfo;
		std::vector< bool >					m_vec4ThAnnivEventRewardInfo;
#endif //SERV_4TH_ANNIVERSARY_EVENT
};
