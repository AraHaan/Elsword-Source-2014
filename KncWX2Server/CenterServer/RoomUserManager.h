#pragma once

#include "RoomSlot.h"
#include "RoomUser.h"
#include "X2Data/XSLRoom.h"

//////////////////////////////////////////////////////////////////////////
//NOTE : �����帧
//data reset : �����߰��߰� ���Ǵ� �����͸� ������ �ѹ��� �����÷��̿� ���Ǵ� ���ֵ����ʹ� ��� StartGame���� �̷������

//#define MAX_ROOM_MEMBER 8       // ���߿� ������ ��ġ�� �ű���.

SmartPointer( KRoomSlotFSM );
SmartPointer( KRoomUserFSM );


#define KRoomUserManagerPtr KRoomUserManager*
#undef KRoomUserManagerPtr
SmartPointer( KRoomUserManager );


class KRoomUserManager
{
public:
    enum ENUM_USERLIST
    {
        UL_ALL,
        UL_RED_TEAM,
        UL_BLUE_TEAM,
        UL_PLAY,
        UL_RESULT,
		//{{ 2011. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		UL_NPC_LOAD_USER,
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

        UL_TOTAL_NUM,
    };

	enum USERLIST_TYPE
	{
		UT_NONE = -1,
		UT_GAME,		//���� ���� ��������Ʈ
		UT_OBSERVER,	//������ ��������Ʈ
		UT_TOTAL_NUM,
	};

	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	enum PING_VALUE
	{
		PV_HIGH		= 60,	 // 0~60 �̸�
		PV_NORMAL	= 120,   // 60~120 �̸�
		PV_LOW		= 10000, // 120~10000 �̸�
	};
#endif SERV_CHOOSE_FASTEST_HOST
	//}

	struct KLeaveUserInfo
	{
		UidType		m_iUnitUID;
		UidType		m_iPartyUID;

		KLeaveUserInfo()
		{
			m_iUnitUID = 0;
			m_iPartyUID = 0;
		}
	};

    KRoomUserManager();
    virtual ~KRoomUserManager();

	void		 Init( IN const CXSLRoom::ROOM_TYPE eRoomType, IN const int nRoomSlot, IN const int nRoomObserverSlot = 3 );		//��ü������ �ѹ��� ȣ���Ѵ�.
    virtual void Reset( USERLIST_TYPE eType = UT_GAME );

    KRoomSlotPtr GetSlot( int iSlotID, USERLIST_TYPE eType = UT_GAME );	
    KRoomUserPtr GetUser( UidType nCID, USERLIST_TYPE eType = UT_GAME );
	KRoomUserPtr GetUser( int nIndex, USERLIST_TYPE eType = UT_GAME );		//for���� ���� �Լ� �ε����� 0����
	//{{ 2011. 11. 2	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool IsExist( IN const UidType iUnitUID ) { return ( GetUser( iUnitUID ) != NULL ); }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	CXSLRoom::ROOM_TYPE GetRoomType() const { return m_eRoomType; }

    bool AddUser( KRoomUserPtr spRoomUser, USERLIST_TYPE eType = UT_GAME );
    bool DeleteUser( UidType nCID, USERLIST_TYPE eType = UT_GAME );
	int DeleteUserByGsUID( IN UidType nGsUID, OUT std::vector<KLeaveUserInfo>& vecDelUserUID, USERLIST_TYPE eType = UT_GAME );   // �ش� ���Ӽ����� ���ӵ� ������ ��� �����. (���� �������� ����)

	bool GetGSUID( IN UidType nCID, OUT UidType& nGSUID, USERLIST_TYPE eType = UT_GAME );

    int		GetNumTotalSlot( USERLIST_TYPE eType = UT_GAME );
    int		GetNumOpenedSlot( USERLIST_TYPE eType = UT_GAME );
    int		GetNumOccupiedSlot( USERLIST_TYPE eType = UT_GAME );
    int		GetNumMember( USERLIST_TYPE eType = UT_GAME );
    int		GetNumPlaying( USERLIST_TYPE eType = UT_GAME );
	int		GetTeamNumPlaying( CXSLRoom::TEAM_NUM eTeamNum, USERLIST_TYPE eType = UT_GAME );
    int		GetNumResultPlayer( USERLIST_TYPE eType = UT_GAME );
    int		GetNumReadyPlayer( USERLIST_TYPE eType = UT_GAME );
	int		GetLiveMember( USERLIST_TYPE eType = UT_GAME );
	//{{ 2013. 04. 10	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	int		GetNumMemberOverLevel( IN const char cLevel, IN USERLIST_TYPE eType = UT_GAME );
	void	GetUnitLevelList( OUT std::map< UidType, int >& mapUnitLevelList );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	//{{ 2012. 09. 08	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	GetUnitUIDByPartyUIDList( OUT std::map< UidType, UidType  >& mapUnitUIDPartyUID );
#endif SERV_BATTLE_FIELD_SYSTEM
	//{{ 2012. 09. 08	������	��Ʋ�ʵ� �ý���
	//{{ 2012. 11. 20	������		���� ��Ʋ�ʵ� üũ �� ����
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	int		GetHostCount() const;
	bool	ReselectOnlyOneHost( IN const UidType iOldHostUID );
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	//}}
    
    bool EnterRoom( IN const KRoomUserInfo& kInfo, IN const bool bConsiderTeam );
	//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	bool EnterRoom( IN const std::vector< KRoomUserInfo >& vecRedTeam, IN const std::vector< KRoomUserInfo >& vecBlueTeam, IN const std::map< UidType, KPvpNpcInfo >& mapPvpNpcInfo, IN const std::map< UidType, int >& mapMatchWaitTime );
#endif SERV_PVP_NEW_SYSTEM
	//}}
    virtual bool LeaveRoom( IN const UidType iUnitUID );
    bool LeaveGame( UidType nCID );
	void LeaveAllUnit();

	bool SetRoomUserInfo( UidType nCID, const KRoomUserInfo& kInfo );

    bool ChangeTeam( UidType nCID, int iDestTeam );
	bool CheckTeamChange( UidType nCID );

	bool DeathTeamBalance( std::vector<KRoomUserPtr>& vecChangeUnit );

	bool GetTeamNum( int& nRedNum, int& nBlueNum );
	int  GetTeamReadyNum( CXSLRoom::TEAM_NUM eTeamNum );

    bool IsReady( UidType nCID, bool& bReady );
	//{{ 2012. 12. 5	������		������ ���� ���� ���
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
	bool SetReady( IN const UidType nCID, IN const bool bReady, IN const bool bObserver = false );
#else
	bool SetReady( IN const UidType nCID, IN const bool bReady );
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
	//}}    
    void SetAllReady( bool bReady );

	bool SetPitIn( UidType nCID, bool bPitIn );

    bool IsInTrade( UidType nCID, bool& bTrade );
    bool SetTrade( UidType nCID, bool bTrade );
    
    bool SetLoadingProgress( UidType nCID, int iLoadingProgress );

    bool SetStageLoaded( UidType nCID, bool bStageLoaded );
    void ResetStageLoaded();

    bool GetTeam( UidType nCID, int& iTeam );

    bool IncreaseTeamNumKill( UidType nCID );
	bool IncreaseTeamNumKillByTeamNum( CXSLRoom::TEAM_NUM teamNum );

    bool SetStageID( UidType nCID, int iStageID );
    void ResetStageID();

	//{{ 2012. 04. 17	������	���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	bool SetSubStageID( IN const UidType iUnitUID, IN const int iSubStageID );
	void ResetSubStageID();
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}

    bool GetNumKill( UidType nCID, int& nKill );
    bool IncreaseNumKill( UidType nCID );
	bool IncreaseNumMDKill( UidType nCID );

    bool GetNumDie( UidType nCID, int& nDie );
    bool IncreaseNumDie( UidType nCID );

	bool IsDie( UidType nCID, bool& bDie );
    bool SetDie( UidType nCID, bool bDie );

    bool SetHP( UidType nCID, float fHP );

	bool IsObserver( UidType nCID );

    bool GetRebirthPos( UidType nCID, int& iRebirthPos );
    bool SetRebirthPos( UidType nCID, int iRebirthPos );

    void GetCurrentKillScore( std::map< UidType, std::pair< int, int > >& mapKillScore, std::map< UidType, int >& mapMDKillScore );    

    void GetLoadingTimeOutUser( OUT std::map< UidType, UidType >& mapUserCIDNGS );

    bool GetPlayTime( UidType nCID, float& fPlayTime );
    void GetPlayTime( OUT std::map< UidType , float >& mapPlayTime );

	//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
#else
	bool GetPVPEmblem( IN UidType nCID, OUT CXSLUnit::PVP_EMBLEM& eEmblem );
#endif SERV_2012_PVP_SEASON2
	//}}
	
	//bool AddItemWithRandom( IN int iItemID, OUT UidType& iGetUnitUID );
	//{{ 2009. 5. 11  ������	�ǽð� ������
	//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
//#ifdef SERV_NEW_HENIR_TEST
	bool GetItemWithRandom( IN const int iItemID, IN const int iDungeonID,  OUT UidType& iGetUnitUID, OUT UidType& iGSUID );
//#else
//	bool GetItemWithRandom( IN const int iItemID, OUT UidType& iGetUnitUID, OUT UidType& iGSUID );
//#endif SERV_NEW_HENIR_TEST
	//}}
	//}}
	bool AddItem( UidType nCID, IN int iItemID );

	bool GetItemList( UidType nCID, std::map< int, int >& mapitemList );

	bool AddKillNPC( UidType nCID );	

	//{{ 2010. 05. 12  ������	���� �ڵ� ����
	bool SetDungeonUnitInfo( IN UidType nCID, IN const KDungeonPlayResultInfo& kDungeonUnitInfo );
	//}}
	void ResetDungeonUnitInfo();

	//{{ 2011. 08. 03	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	bool SetPvpPlayResultInfo( IN const UidType iUnitUID, IN const KPvpPlayResultInfo& kPvpUnitInfo );
#endif SERV_PVP_NEW_SYSTEM
	//}}

	//{{ 2008. 9. 19  ������	���
	bool SetDungeonPlayInfo( UidType nCID, KEGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT& kDungeonPlayInfo );
	//}}

	bool SetSuccessResult( UidType nCID, bool bResult );

    void StartGame();
    void StartPlay();
	void StartResult();
    void EndPlay();
    void EndGame();
	void EndResult();
	//{{ 2012. 12. 5	������		������ ���� ���� ���
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
	bool StartGame( IN const UidType nCID, IN const bool bObserver = false );
	bool StartPlay( IN const UidType nCID, IN const bool bObserver = false );
#else
	bool StartGame( IN const UidType nCID );
	bool StartPlay( IN const UidType nCID );
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
	//}}
	bool StartResult( UidType nCID );
    bool EndPlay( UidType nCID );
    bool EndGame( UidType nCID );
	bool EndResult( UidType nCID );


    bool IsHost( UidType nCID );
    bool IsPlaying( UidType nCID );
    bool IsAllPlayerReady();
    bool IsAllPlayerFinishLoading();
    bool IsAllPlayerStageLoaded();
	//{{ 2009. 7. 20  ������	��� ��������
	bool IsAllPlayerAlive();
	//}}
	bool IsAllPlayerDie();
    bool IsAllPlayerHPReported();
	bool IsAllPlayerStageID();
	bool IsAllPlayerDungeonUnitInfoSet();	
	//{{ 2007. 8. 27  ������  
	bool CheckAuthLevel( UidType nCID );
	//}}    
	bool IsAllPlayerSuccessResult();

	//{{ 2009. 12. 4  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	bool IsAllPlayerSameGuildMember();
#endif GUILD_SKILL_TEST
	//}}

    void AssignTeam( int iGameMode );
    bool OpenSlot( int iSlotID );
    bool CloseSlot( int iSlotID );
	bool OpenSlotTeam( IN int iSlotID, OUT int& iTeamBalanceSlotID );
	bool CloseSlotTeam( IN int iSlotID, OUT int& iTeamBalanceSlotID );
    bool ToggleOpenClose( int iSlotID );
    bool GetRoomSlotInfo( int iSlotID, KRoomSlotInfo& kInfo, USERLIST_TYPE eType = UT_GAME );
    bool GetRoomSlotInfo( UidType nCID, KRoomSlotInfo& kInfo, USERLIST_TYPE eType = UT_GAME );
    void GetRoomSlotInfo( std::vector< KRoomSlotInfo >& vecRoomSlotInfo, USERLIST_TYPE eType = UT_GAME );
    bool GetRoomUserGS( UidType nCID, UidType& nGS );
	//{{ 2010. 05. 13  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	bool GetRoomUserGS( IN const std::wstring& wstrNickName, OUT UidType& nGS, OUT UidType& iUnitUID );
#endif SERV_INTEGRATION
	//}}
	void GetUserList( IN const int iFlag, OUT std::map< UidType, std::set< UidType > >& mapUidList, IN const USERLIST_TYPE eType = UT_GAME ); // Server UID ���� ���� User UID.
	//{{ 2011. 05. 18	������	���� ȹ�� ���� ����
#ifdef SERV_GET_RANDOM_ITEM_LOGIC_FIX
	void GetUnitUIDList( OUT std::vector< UidType >& vecUidList );
#endif SERV_GET_RANDOM_ITEM_LOGIC_FIX
	//}}
    void DecideWinTeam( unsigned char ucGameType, std::vector< int >& vecWinTeam );
    bool IsAnyTeamEliminated();
    bool IsOneTeamExist();
    bool IsAnyoneReachObjectiveNumKill( int nKill );
    bool IsAnyTeamReachObjectiveNumKill( int nKill );

//#ifdef PVP_BOSS_COMBAT_TEST
//	bool GetIsBoss( UidType nCID, bool& bIsBoss );
//	bool SetIsBoss( UidType nCID, bool bIsBoss );
//
//	bool PickRandomBoss();
//	bool IsAnyTeamBossDead( OUT bool& bRedTeamBossDead, OUT bool& bBlueTeamBossDead );
//#endif PVP_BOSS_COMBAT_TEST

	//{{ 2012. 11. 21	������		�����ڿ��� ĳ���� ���� ���� ������
#ifdef SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
	void	GetEnterCashShopUserList( OUT std::vector< UidType >& vecEnterCashShopUser ) const;
#endif SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
	//}}

    int		GetMaxKillUnit();
	int		GetMaxKillTeam();

    int		GetTeamScore( int iTeam );
	
	//������ UNIT�� EXP������ ó���ϱ� ����..
	bool	AddRewardEXP( IN UidType nCID, IN int iEXP );

	//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
	bool	AddDungeonRewardEXP( IN const UidType iUnitUID,
								 IN const int iEXP,
								 IN const int iDungeonID,
								 IN const char cDifficultyLevel,
								 IN float fLevelFactor,
								 OUT KERM_NPC_UNIT_DIE_NOT& kInfo );
#else
	bool	AddDungeonRewardEXP( IN const int iEXP, 
								 IN const int iDungeonID, 
								 IN const int iNpcLv, 
								 OUT KERM_NPC_UNIT_DIE_NOT& kInfo, 
								 IN const char cDifficultyLevel );
#endif SERV_REFORM_ITEM_DROP
	//}}	

	bool	GetRewardEXP( IN UidType nCID, OUT int& iEXP );
	bool	GetRewardPartyEXP( IN UidType nCID, OUT int& iPartyEXP );
	


	//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
	bool	AddDungeonRewardED( IN int iED, IN UidType iUserUID, IN float fPartyBonusRate, IN int iDungeonID, IN char cDifficultyLevel, OUT KEGS_GET_ITEM_NOT& kInfo );
#else
	//{{ 2009. 5. 12  ������	�ǽð� ED����
	//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
	//#ifdef SERV_NEW_HENIR_TEST
	bool	AddDungeonRewardED( IN int iED, IN int iDungeonID, OUT KEGS_GET_ITEM_NOT& kInfo );
	//#else
	//	bool	AddDungeonRewardED( IN int iED, OUT KEGS_GET_ITEM_NOT& kInfo );
	//#endif SERV_NEW_HENIR_TEST
	//}}
	//}}
#endif SERV_REFORM_ITEM_DROP
	//}}

	//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	void	SetAllPrepareForDefenceDungeon( IN const bool bVal );
	bool	SetPrepareForDefenceDungeon( IN const UidType iUnitUID, IN const bool bVal );
	bool	IsAllPlayerPrepareForDefenceDungeon();
	void	GetUnitUIDListNotPrepareForDefenceDungeon( std::map< UidType, UidType >& mapUserList );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	//{{ 2013. 04. 22	������	����� �� ���� ���� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	void	SetAllEnterDefenceDungeon();
	bool	SetEnterDefenceDungeon( IN const UidType iUnitUID, IN const bool bAgree );
	bool	IsAllPlayerPrepareForDefenceDungeon( OUT std::vector< UidType >& vecLeaveUnitUID );
	void	GetUnitUIDListDisagreeEnterDefenceDungeon( OUT std::vector< UidType >& vecLeaveUnitUID );
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	//}}
    
    void SetUserEquipItem( IN UidType nCID, IN std::vector< KInventoryItemInfo >& vecInventorySlotInfo );
	
	//{{ 2007. 10. 18  ������  	
	int GetKillNumber( UidType nCID );	
	//}}
	//{{ 2012. 12. 5	������		������ ���� ���� ���
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
	void SetIsIntrude( IN const UidType nCID, IN const bool bIsIntrude, IN const bool bObserver = false );
#else
	void SetIsIntrude( IN const UidType nCID, IN const bool bIsIntrude );
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
	//}}
	bool GetIsIntrude( UidType nCID );
	//{{ 2007. 12. 18  ������  ��Ȱ�� ��� ����[for statistics]
	int GetUsedRessurectionStoneCount( UidType nCID );
	int GetUsedRessurectionStonePlayerCount();
	//}}

	bool IsGameBang( UidType nCID );
#ifdef SERV_PC_BANG_TYPE
	int GetPcBangType( UidType nCID );
#endif SERV_PC_BANG_TYPE

	bool IsRingofpvprebirth( UidType nCID );

	//Zombie unit funtion
	bool ZU_IsCheckZU() const						{ return m_bIsCheckZU; }
	void ZU_CheckStart();	//call endgame()
	void ZU_CheckEnd();		//call startgame()



	//{{ 2012. 09. 08	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool ZU_Tick( IN const CXSLRoom::ROOM_TYPE eRoomType, OUT std::vector< UidType >& vecZombieUnit );	//Tick()
#else
	bool ZU_Tick( OUT std::vector< UidType >& vecZombieUnit );	//Tick()
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	
	void ZU_Refresh( UidType nCID, u_short usEventID );		//ProcessEvent()

	//{{ 2012. 10. 22	�ڼ���	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	virtual bool RequestTradeTo( UidType nCID, UidType nCIDTo );
	virtual bool TradeAcceptedBy( UidType nCID, UidType nCIDAcceptor );
#else
    bool RequestTradeTo( UidType nCID, UidType nCIDTo );
	bool TradeAcceptedBy( UidType nCID, UidType nCIDAcceptor );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
    bool TradeRejectedBy( UidType nCID, UidType nCIDRejector );
    void ExpireTradeRequest( std::vector< UidType >& vecTradeExpireUnit );

	//{{ 2012. 11. 9	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
	bool GetQuestDropItem( IN const UidType iUnitUID, 
							IN const int iDungeonID, IN const char cDifficulty, 
							IN const char cDungeonMode, 
							IN const int iNPCID, 
							OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo, 
							IN float fQuestItemDropRate );
#else
	bool GetQuestDropItem( IN const UidType iUnitUID, IN const int iDungeonID, IN const char cDifficulty, IN const char cDungeonMode, IN const int iNPCID, OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo );
#endif SERV_QUEST_ITEM_DROP_EVENT
	//}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	
	//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
	bool GetQuestDropItem( IN const int iDungeonID, 
						   IN const char cDifficulty, 
						   IN const char cDungeonMode, 
						   IN const int iNPCID, 
						   OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo,
						   IN float fQuestItemDropRate );
#else
	//{{ 2011. 08. 31  ��μ�	��д��� ����
	//#ifdef SERV_RANDOM_DAY_QUEST
	bool GetQuestDropItem( IN const int iDungeonID, IN const char cDifficulty, IN const char cDungeonMode, IN const int iNPCID, OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo );
	//#endif SERV_RANDOM_DAY_QUEST
	//}}
#endif SERV_QUEST_ITEM_DROP_EVENT
	//}

	bool SetUnitQuestInvenInfo( UidType nCID, std::map<int, KSubQuestInfo>& mapInvenInfo );

	bool SetTutorialUIDList( UidType nCID, const std::vector<UidType>& vecTutorUIDList );
	//{{ 2009. 6. 10  ������	���� ������ ��� EXP,ED	
	bool UpdateetOldExpED( UidType nCID, int iOldEXP, int iOldED );
	//}}
	
	//{{ 2012. 07. 03	������		���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	bool SetGameChannelIP( IN const UidType iUnitUID, IN const std::wstring& wstrGameChannelIP );
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}

	//{{ 2008. 6. 17  ������  �ǽð� ��Ȱ��
	void SetEndPlay( UidType nCID, bool bEndPlay );
	void SetCashContinueReady( UidType nCID, bool bCashContinueReady );
	void ReserveEndPlay( UidType nCID );
	bool CheckEndPlay();
	bool CheckEndPlayAnotherMember( UidType nCID );
	void StopDungeonContinueTime( UidType nCID, bool bIsStop );
	//}}

	//{{ 2008. 7. 24  ������	�����̵�
    bool IsRankFail( IN int iDungeonID );
	//}}

	int GetHighestUserLevel() const;
	//{{ 2012. 05. 10	������	���� ���� ���� ����
#ifdef SERV_DUNGEON_NPC_LEVEL
	int	GetAverageUserLevel( IN const int iIncludeLevelGap = 0 ) const;
#endif SERV_DUNGEON_NPC_LEVEL
	//}}

	//{{ 2009. 7. 21  ������	��� ��������
	void ResetAgreeEnterSecretStage();
	bool AgreeEnterSecretStage( UidType nCID, CXSLRoom::ENTER_SECRET_STAGE eAgreeValue );
	bool IsAllPlayerAgreed();
    bool IsEnterSecretStage();
	//}}

	//{{ 2010. 07. 09  ������	��ӷ� �̺�Ʈ Ȯ��
#ifdef SERV_PC_BANG_DROP_EVENT
	bool IsExistPcBangPlayer();
#endif SERV_PC_BANG_DROP_EVENT
	//}}

	//{{ 2010. 8. 3	������	�� �ý���
#ifdef SERV_PET_SYSTEM
	bool HavePet( IN UidType nCID );
	void GetTeamMemberList( IN CXSLRoom::TEAM_NUM eTeamNum, IN bool bIsPlayingMember, OUT std::vector< UidType >& vecUnitUIDList );
	bool UpdatePetInfo( IN UidType nCID, IN const std::vector< KPetInfo >& vecPetInfo );
	bool UpdatePetInfo( IN UidType nCID, IN const KPetInfo& kPetInfo );
	bool UpdatePetSatiety( IN UidType nCID, IN UidType iPetUID, IN short sSatiety );
	bool UpdatePetIntimacy( IN UidType nCID, IN UidType iPetUID, IN int iIntimacy );
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2012. 03. 06	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool UpdateGameStat( IN const UidType iUnitUID, IN const KStat& kGameStat );
	bool UpdateBuffInfo( IN const UidType iUnitUID, IN const std::vector< KBuffInfo >& vecBuffInfo );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	void CheckItemEquipQuestCondition( IN bool bIsGameStart );
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

	//{{ 2010. 8. 25	������	���� ������ ���
	KRoomUserPtr GetHostUser();
	//}}

	//{{ 2010. 12. 7	������	���� ���� ��� ���
#ifdef SERV_APPROPRIATE_LEVEL_STAT
	void GetAppropriateLevelStatistics( OUT KELOG_APPROPRIATE_LEVEL_STATISTICS_NOT& kNot );
#endif SERV_APPROPRIATE_LEVEL_STAT
	//}}

	//{{ 2011. 03. 08	��μ� ����ġ�� ȹ���� ������ ����Ʈ �Ϸ� ���� ����
#ifdef SERV_EXP_ACQUISITION_CONDITION_CHANGE
	const std::map< UidType, bool >& GetHaveExpInDungeon() const { return m_mapHaveExpInDungeon; }
	void						AddHaveExpInDungeon( UidType UnitUid, bool bState );
#endif SERV_EXP_ACQUISITION_CONDITION_CHANGE
	//}}

	//{{ 2011. 03. 17	��μ�	���� �÷��� �α� ��û���� 
#ifdef SERV_DUNGEON_TEMP_LOG_DB
	int				GetTotalPatyDamage_LUA();
#endif SERV_DUNGEON_TEMP_LOG_DB
	//}}

	//{{ 2011. 03. 26  ��μ�	û 1�� �߰� ��ų �߰� - ����
#ifdef SERV_ADD_BUFF_SKILL_INFO
	bool			ProcessBuffSkill( IN KEGS_BUFF_SKILL_INFO_CONVEY_REQ& kPacket, OUT KEGS_BUFF_SKILL_INFO_CONVEY_NOT& KNot );
	bool			CheckCaughtBuffSkill( OUT KEGS_BUFF_SKILL_INFO_CONVEY_NOT& KNot );
#endif SERV_ADD_BUFF_SKILL_INFO
	//}}

	//{{ 2011. 05. 04  ��μ�	���� Ŭ����� ������ ���� ���� �߰�
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	bool	IsRoomUserGetExp( IN UidType iUnitUID );
	bool	SetUnitAllQuestInfo( UidType nCID, std::set< int >& setQuestInfo );
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	bool	IsComeBackUserInRoom();
#endif SERV_COME_BACK_USER_REWARD
	//}} 

	//{{ 2011. 07. 21	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	int		GetRatingByTeam( IN const CXSLRoom::TEAM_NUM eTeamNum );
	bool	SetPvpNpcInfo( IN const UidType iUnitUID, IN const int iNpcID );
	bool	IsPvpNpc( IN const UidType iUnitUID );
	int		GetRedTeamRating() const					{ return m_iRedTeamRating; }
	int		GetBlueTeamRating() const					{ return m_iBlueTeamRating; }
	bool	IsOnlyPvpNpcInRoom( OUT std::vector< UidType >& vecPvpNpcUnitUIDList ) const;
	bool	SetMatchWaitTime( IN const UidType iUnitUID, IN const int iMatchWaitTime );
#endif SERV_PVP_NEW_SYSTEM
	//}}

	//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
	int GetHenirRewardUserNum();
	void GetHenirRewardUserList( OUT std::map< UidType, UidType >& mapList );
#endif SERV_NEW_HENIR_TEST
	//}}
	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	bool	SetPingScore( UidType nCID, bool bForceHost, DWORD dwPingScore );
#else//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	bool	SetPingScore( UidType nCID, DWORD dwPingScore );
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
#endif SERV_CHOOSE_FASTEST_HOST
	//}
	//{{ 2011. 11. 7	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool	SetActivatedAutoPartyBonus( IN const UidType iUnitUID, IN const bool bActivatedAutoPartyBonus );
	bool	SetBattleFieldNpcLoad( IN const UidType iUnitUID, IN const bool bVal );
	bool	IsBattleFieldNpcLoad( IN const UidType iUnitUID );
	bool	SetBattleFieldNpcSyncSubjects( IN const UidType iUnitUID, IN const bool bVal );
	bool	IsBattleFieldNpcSyncSubjects( IN const UidType iUnitUID );
	void	GetBattleFieldNpcSyncSubjectsList( OUT std::vector< UidType >& vecUnitUIDList );
	void	GetUnitUIDList( OUT std::vector< UidType >& vecUnitUIDList, IN const UidType iNotIncludeUnitUID = 0 ) const;
	bool	SetOldPartyInfo( IN const UidType iUnitUID, IN const KOldPartyInfo& kInfo );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 06. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	PrepareForReturnToFieldProcess( IN const UidType iRoomUID, OUT UidType& iGSUID, OUT UidType& iUnitUID, OUT KReturnToFieldInfo& kInfo );
	bool	GetNextReturnToFieldUnitInfo( IN const UidType iRoomUID, OUT UidType& iGSUID, OUT UidType& iUnitUID, OUT KReturnToFieldInfo& kInfo );
	UidType GetSendReturnToFieldUnitUID() const		{ return m_iSendReturnToFieldUnitUID; }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	bool	EnemyIsPlacementTeam( IN const int& iMyTeam );
#endif SERV_2012_PVP_SEASON2
	//}}

	//{{ 2012. 07. 13	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2_2
	int		GetMyTeamKillNum( IN const UidType& iMyUnitUID );
	int		GetEnemyTeamKillNum( IN const UidType& iMyUnitUID );
#endif SERV_2012_PVP_SEASON2_2
	//}}

	//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
	int		GetUserLevel( IN const UidType& iMyUnitUID );
	float	GetPartyBonusRate( IN const int& iPartyMember );
	void	GetLiveMemberList( OUT std::vector< UidType >& vecLiveMember, IN USERLIST_TYPE eType = UT_GAME );
#endif SERV_REFORM_ITEM_DROP
	//}}

	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	//{{ 2013. 03. 06   ���� ���� ���� ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST_NEW
	KRoomUserPtr	ChooseBetterHost( UidType iHostUID );
#else
	KRoomUserPtr	ChooseBetterHost( DWORD dwHostPingScore
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
        , bool bHostIsPlaying, bool bHostZombieAlert
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
        );
#endif SERV_CHOOSE_FASTEST_HOST_NEW
	//}
	void	ClearPingScore();
#endif SERV_CHOOSE_FASTEST_HOST
	//}

	//{{ �ڵ� ��Ƽ�� ���� ���� �� ��Ī ��� �ð� �α� �߰� - ��μ�
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
	bool	SetAutoPartyWaitTime( IN const UidType iUnitUID, IN const int iAutoPartyWaitTime );
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
	//}
	//{{ 2012. 12. 17	������	�ƶ� ��Ƽ �÷��� ���ʽ� ����ġ
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	bool	IsExistCharType( IN const CXSLUnit::UNIT_TYPE eUnitType ) const;
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	//}}
	//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	void SetUnitLevelBeforGameStart( IN const UidType iUnitUID, IN const bool bSuitableLevel );
	const std::map< UidType, bool >& GetSuitableLevelInfo() const { return m_mapSuitableLevelInfo; }
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	//}}

#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
	bool	SetUnitGoingQuestInfo( UidType nCID, std::set< int >& setQuestInfo );
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT
	bool	IsAraEvent();
#endif SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT

#ifdef SERV_PVP_REMATCH
	bool			SetRematch( UidType nCID, bool bAcceptRematch );
	bool			SetAllRematch( bool bAcceptRematch );
	bool			IsAllPlayerWantRematch();
#endif SERV_PVP_REMATCH

#ifdef SERV_PAYMENT_ITEM_WITH_ALLY_NPC
	bool			AddAllyNPC( IN const UidType iUnitUID, IN const KNPCUnitReq& kNPCUnit );
	void			DeleteAllyNPC( IN const UidType iNPCUID );
#endif SERV_PAYMENT_ITEM_WITH_ALLY_NPC

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
	bool SetRidingPetInfo( IN const UidType iUnitUID, IN const UidType iRidingPetUID, IN const USHORT usRidingPetID );
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-16	// �ڼ���
	byte	GetPlayStartedUserNum( void ) const{	return m_byteStartPlayMemberNum;	}
#endif // SERV_HENIR_RENEWAL_2013

#ifdef SERV_RELATIONSHIP_EVENT_INT
	bool	CheckCouplePVP();
#endif //SERV_RELATIONSHIP_EVENT_INT

protected:

    void			MakeSlot( int iSlotID, USERLIST_TYPE eType = UT_GAME );
    KRoomUserPtr	MakeUser( const KRoomUserInfo& kInfo );
    void			HandleInvalidSlotPointer( int iSlotID, USERLIST_TYPE eType = UT_GAME );

    KRoomSlotPtr	FindEmptySlot( bool bConsiderTeam, USERLIST_TYPE eType = UT_GAME );
    UidType			AppointNewHost();
	//���������� ���� ȹ�� EXP �뷱��	
	float			CheckDungeonBalRate( int iUnitLevel, int nDungeonID );
	//{{ 2010. 04. 30  ������	��д��� ����
#ifdef SERV_SECRET_HELL 
	float			CheckDungeonBalRateByNpcLv( int iUnitLevel, int iNpcLv );
#endif SERV_SECRET_HELL
	//}}

	bool			CheckUserlistType( USERLIST_TYPE eType ) const;

protected:

	CXSLRoom::ROOM_TYPE					m_eRoomType;

    int                                 m_nRoomSlot[UT_TOTAL_NUM];
    std::vector< KRoomSlotPtr >         m_vecRoomSlot[UT_TOTAL_NUM];
    std::map< UidType, KRoomUserPtr >   m_mapRoomUser[UT_TOTAL_NUM];

    static FSMclassPtr                  ms_spSlotFSM;       // RoomSlot FSM
    static FSMclassPtr                  ms_spUserFSM;       // RoomUser FSM

    std::map< int, int >                m_mapTeamNumKill;

	bool								m_bIsCheckZU;	//zombie user check

	//{{ 2011. 03. 08	��μ� ����ġ�� ȹ���� ������ ����Ʈ �Ϸ� ���� ����
#ifdef SERV_EXP_ACQUISITION_CONDITION_CHANGE
	std::map< UidType, bool >			m_mapHaveExpInDungeon;		// �������� ����ġ�� ���� ���� �ִ�.
#endif SERV_EXP_ACQUISITION_CONDITION_CHANGE
	//}}
	//{{ 2011. 07. 25	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	int									m_iRedTeamRating;
	int									m_iBlueTeamRating;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2012. 06. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	std::map< UidType, UidType >		m_mapReturnToFieldRemainUnitUID;		// key : UnitUID, value : GSUID
	UidType								m_iSendReturnToFieldUnitUID;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	std::map< UidType, bool >			m_mapSuitableLevelInfo;		// �������� �� ���������̾��°�?.
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	//}}

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
public:
	//bool IsForceHost() { return m_bForceHost; }
    UidType GetForceHostCID(); // { return m_uidForceHostCID; }
private:
	//bool m_bForceHost;
    UidType m_uidForceHostCID;
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-16	// �ڼ���
	byte	m_byteStartPlayMemberNum;
#endif // SERV_HENIR_RENEWAL_2013
};



