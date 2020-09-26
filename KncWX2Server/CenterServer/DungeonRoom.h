#pragma once

#include "Room.h"
#include "X2Data/XSLDungeon.h"
#include "TimerManager.h"
//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	#include "EventDataRefresh/EventMonsterRefreshManager.h"
#else
	#include "GameEvent/GameEventScriptManager.h"
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
//{{ 2011. 10. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "DungeonMonsterManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2011. 10. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "DungeonSecurityManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
	#include "KDropTable.h"
#endif SERV_NEW_HENIR_TEST
//}}
//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	#include "BadAttitudeManager.h"
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}
//{{ 2013. 02. 01  �̺�Ʈ �߷�Ÿ�� ���� - ��μ�
#ifdef SERV_EVENT_VALENTINE_DUNGEON
	#include "ValentineManager.h"
#endif SERV_EVENT_VALENTINE_DUNGEON
//}
//{{ 2013. 04. 09	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	#include "DefenceDungeonManager.h"
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

SmartPointer( KDungeonRoom );

class KDungeonRoom : public KRoom
{
    NiDeclareRTTI;
    DeclToStringW;

public:
#   undef _ENUM
#   define _ENUM( id ) id,
    enum StatDungeon
    {
#   include "StatDungeon_def.h"
    };	

	struct ITEM_DATA
	{
		bool	m_bIsGet;
		int		m_iDropItemUID;

		int		m_iItemID;

		bool	m_bIsGP;
		int		m_iED;

		//{{ 2013. 1. 14	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
		char	m_cEnchantLevel;
		bool	m_bNotify;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
		//}}

		ITEM_DATA()
			: m_bIsGet( false )
			, m_iDropItemUID( 0 )
			, m_iItemID( 0 )
			, m_bIsGP( false )
			, m_iED( 0 )
			//{{ 2013. 1. 14	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
			, m_cEnchantLevel( 0 )
			, m_bNotify( false )
#endif SERV_ENCHANT_ITEM_DROP_EVENT
			//}}
		{
		}
	};

	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	enum DEFFENC_DUNGEON
	{
		MAX_SMALL_CRYSTAL = 4,
	};
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
	
	//{{ 2011. 10. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
#else
	typedef std::pair< int, int >	KSubStageKey;

	struct SUB_STAGE_NPC_INFO
	{
		std::set< int > m_setNormalNpcUID;
		std::set< int > m_setMiddleBossNpcUID;
		std::set< int > m_setBossNpcUID;
	};
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

public:
    KDungeonRoom();
    virtual ~KDungeonRoom();

    // override.
    virtual void	Tick();

protected:
    // Get Set
    virtual void	SetRoomInfo( const KRoomInfo& kInfo );

    // override.
    virtual void	ProcessNativeEvent( const KEventPtr& spEvent );
    virtual void	StartGame();
    virtual void	StartPlay();
    virtual void	EndPlay();
    virtual void	EndGame();
    virtual bool	CheckIfPlayEnd();
    virtual int		DecideWinTeam();
    virtual int		GetPlayMode();
    virtual void	HandleTimeOutUser( std::map< UidType, UidType >& mapUserCIDNGS );
	virtual void	CloseRoom();
	virtual void	CheckResultSuccess();
	virtual float	GetRemainingPlayTime(){ return static_cast<float>(m_fRemainingPlayTime + m_fLoadingTime); }
	virtual void	GetRoomInfo( OUT KRoomInfo& kInfo );
	
	void			SendDSSGoNextAllNot( KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT& kDSSGoNextAllNot );

	//{{ 2007. 10. 19  ������  ���� ��踦 ���� �Լ�
	void			BroadCastUpdateUnitInfoWithPartyCheck( 
					std::vector< KRoomUserPtr >& vecKRoomUserPtr, 
					unsigned short usEventID, 
					std::vector< KERM_UPDATE_DUNGEON_UNIT_INFO_NOT >& data,
					int iDungeonID
					);
	//}}

	//{{ 2012. 02. 06    ��μ�    ���� ���� UID �߱� ��� ����
//#ifdef SERV_MAKE_MONSTER_UID_CHANGE
//	int GetDungeonNpcRandomInitUid();
//#endif SERV_MAKE_MONSTER_UID_CHANGE	
	//}}
	//{{ 2012. 10. 16	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	virtual	void	LogToDB_GamePlayNetWorkInfo( IN const UidType iUnitUID, IN const KGamePlayNetworkInfoPackage& kInfo, IN const bool bLeaveRoom );
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	//}}	
	//{{ 2012. 11. 9	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM
	//{{ 2012. 12. 12	������	��Ʋ�ʵ� �ý���
#ifdef SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
	void			LogToDB_DungeonLog( IN KRoomUserPtr spRoomUser, IN const int iClearValue, IN const KERM_UPDATE_DUNGEON_UNIT_INFO_NOT& kUpdateUnitInfo, IN const KDungeonUnitResultInfo& kDungeonUnitInfo, IN const int iLeaveReason = 0 );
#else
	void			LogToDB_DungeonLog( IN KRoomUserPtr spRoomUser, IN const int iClearValue, IN const KERM_UPDATE_DUNGEON_UNIT_INFO_NOT& kUpdateUnitInfo, IN const KDungeonUnitResultInfo& kDungeonUnitInfo );
#endif SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
	//}}	
//#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

   _DECL_ON_FUNC( ERM_OPEN_DUNGEON_ROOM_REQ, KERM_OPEN_ROOM_REQ );
	DECL_ON_FUNC( ERM_PARTY_OPEN_DUNGEON_ROOM_REQ );
   //{{ 2011. 12. 09	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
   _DECL_ON_FUNC( ERM_SINGLE_OPEN_DUNGEON_ROOM_REQ, KERM_OPEN_ROOM_REQ );
#endif SERV_BATTLE_FIELD_SYSTEM
   //}}

	DECL_ON_FUNC( ERM_GAME_START_REQ );
	DECL_ON_FUNC( ERM_LEAVE_ROOM_REQ );
	//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
   _DECL_ON_FUNC( ERM_LEAVE_ROOM_FOR_JOIN_BATTLE_FIELD_REQ, KERM_LEAVE_ROOM_REQ );
   _DECL_ON_FUNC( ERM_LEAVE_ROOM_FOR_ENTER_THE_VILLAGE_REQ, KERM_LEAVE_ROOM_REQ );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
   _DECL_ON_FUNC( ERM_DUNGEON_STAGE_LOAD_REQ, KEGS_DUNGEON_STAGE_LOAD_REQ );
   _DECL_ON_FUNC( ERM_DUNGEON_STAGE_LOAD_COMPLETE_REQ, KEGS_DUNGEON_STAGE_LOAD_COMPLETE_REQ );
   _DECL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_OPEN_REQ,			KEGS_DUNGEON_SUB_STAGE_OPEN_REQ );
   _DECL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_GO_NEXT_REQ,		KEGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ );
   _DECL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ,	KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ );

	//{{ 2009. 7. 20  ������	��н�������
	DECL_ON_FUNC_NOPARAM( ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ );
   _DECL_ON_FUNC( ERM_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT, KEGS_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT );
	//}}   

   _DECL_ON_FUNC( ERM_NPC_UNIT_CREATE_REQ, KEGS_NPC_UNIT_CREATE_REQ );
   _DECL_ON_FUNC( ERM_NPC_UNIT_DIE_REQ, KEGS_NPC_UNIT_DIE_REQ );
   _DECL_ON_FUNC( ERM_USER_UNIT_DIE_REQ, KEGS_USER_UNIT_DIE_REQ );
    DECL_ON_FUNC_NOPARAM( ERM_USER_UNIT_DIE_COMPLETE_REQ );
   
   _DECL_ON_FUNC( ERM_GET_ITEM_REQ, KEGS_GET_ITEM_REQ );
	//{{ 2009. 5. 11  ������	�ǽð�������
	DECL_ON_FUNC( ERM_GET_ITEM_COMPLETE_NOT );
	//}}	
	DECL_ON_FUNC( ERM_MY_USER_UNIT_INFO_TO_SERVER_REQ );
   _DECL_ON_FUNC( ERM_DUNGEON_PLAY_INFO_TO_SERVER_NOT, KEGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT ); // ���

   _DECL_ON_FUNC( ERM_END_GAME_REQ, KEGS_END_GAME_REQ );
   _DECL_ON_FUNC( ERM_USE_QUICK_SLOT_NOT, KEGS_USE_QUICK_SLOT_NOT );    

   _DECL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType );

   _DECL_ON_FUNC( ERM_RESURRECT_TO_CONTINUE_DUNGEON_REQ, KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ );
	DECL_ON_FUNC_NOPARAM( ERM_SKIP_CONTINUE_DUNGEON_NOT );

    DECL_ON_FUNC( ERM_TUTORIAL_DUNGEON_ROOM_UPDATE_NOT );
	
   _DECL_ON_FUNC( ERM_STOP_DUNGEON_CONTINUE_TIME_REQ, KEGS_STOP_DUNGEON_CONTINUE_TIME_REQ ); // �ǽð� ��Ȱ��
	
   _DECL_ON_FUNC( ERM_FIELD_UNIT_SYNC_DATA_NOT, KEGS_FIELD_UNIT_SYNC_DATA_NOT );			 // ���� ����� ��ũ
	
	DECL_ON_FUNC_NOPARAM( ERM_PARTY_BAN_USER_REQ );											 // ��Ƽ������	

	//{{ 2009. 12. 29  ������	NPCKILLALL��ŷ����
	DECL_ON_FUNC_NOPARAM( ERM_DUNGEON_KILLALLNPC_CHECK_REQ );
	//}}
	//{{ 2010. 11. 18	������	������ �Ǹ� ����
#ifdef SERV_AUTO_HACK_CHECK_SELL_ITEM
   _DECL_ON_FUNC( ERM_CHECK_SELL_ED_ITEM_REQ, KEGS_SELL_ED_ITEM_REQ );
#endif SERV_AUTO_HACK_CHECK_SELL_ITEM
	//}}
#ifdef SERV_CREATE_ELITE_FOR_ADMIN
   _DECL_ON_FUNC( ERM_ADMIN_NPC_UNIT_CREATE_REQ, KEGS_ADMIN_NPC_UNIT_CREATE_REQ );
#endif SERV_CREATE_ELITE_FOR_ADMIN

   //{{ 2011. 01. 25  ��μ�  ���� Ʈ���� ����(������ ������ - ���� PVP Ȯ��)
#ifdef SERV_WORLD_TRIGGER_RELOCATION
   _DECL_ON_FUNC( ECN_WORLD_TRIGGER_RELOCATION_REQ, KEGS_WORLD_TRIGGER_RELOCATION_REQ );
#endif SERV_WORLD_TRIGGER_RELOCATION
   //}}
   //{{ 2012. 09. 02	�ڼ���	Merge ( �������� Ŭ���̾�Ʈ ���۵����� UDP ��Ŷ�� ���ƿ��� �ʴ� ������ �߰ߵǸ� �������� ű�Ѵ�. // 2012.06.11 lygan_������ )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
   _DECL_ON_FUNC( ERM_UDP_CHECK_KICK_USER_NOT, KEGS_UDP_CHECK_KICK_USER_NOT );
#endif UDP_CAN_NOT_SEND_USER_KICK
   //}}
	//{{ 2012. 11. 9	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	DECL_ON_FUNC_NOPARAM( ERM_UPDATE_HENIR_REWARD_USER_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	_DECL_ON_FUNC( ERM_BAD_ATTITUDE_USER_CHECK_INFO_NOT, KEGS_BAD_ATTITUDE_USER_CHECK_INFO_NOT );
	_DECL_ON_FUNC( ERM_DEFENCE_DUNGEON_WAVE_ID_NOT, KEGS_DEFENCE_DUNGEON_WAVE_ID_NOT );
	_DECL_ON_FUNC( ERM_FORCED_EXIT_VOTE_REQ, KEGS_FORCED_EXIT_VOTE_REQ );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}
	//{{ 2013. 02. 01  �̺�Ʈ �߷�Ÿ�� ���� - ��μ�
#ifdef SERV_EVENT_VALENTINE_DUNGEON
	DECL_ON_FUNC_NOPARAM( ERM_SYNC_DUNGEON_TIMER_NOT );
#endif SERV_EVENT_VALENTINE_DUNGEON
	//}

	//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	DECL_ON_FUNC( ERM_PREPARE_FOR_DEFENCE_DUNGEON_ACK );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	//{{ 2013. 04. 22	������	����� �� ���� ���� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	_DECL_ON_FUNC( ERM_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT, KEGS_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT );
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	//}}

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    //{{ 2013.09.03 ������  ���� �뿡 ��Ʋ�ʵ� ��Ŷ ���۽� ����
	DECL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_ZOMBIE_ALERT_NOT );
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

#ifdef SERV_STAGE_CLEAR_IN_SERVER// �۾���¥: 2013-10-30	// �ڼ���
	_DECL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_CLEAR_REQ, KEGS_DUNGEON_SUB_STAGE_CLEAR_REQ );
	_DECL_ON_FUNC( ERM_SECRET_STAGE_LOAD_REQ, KEGS_SECRET_STAGE_LOAD_REQ );
#endif // SERV_STAGE_CLEAR_IN_SERVER

protected:
	void			SetStageID( IN int iStageID )		{ m_iStageID = iStageID; }
	int				GetStageID()						{ return m_iStageID; }
	void			SetSubStageID( IN int iSubStageID ) { m_iSubStageID = iSubStageID; }
	int				GetSubStageID()						{ return m_iSubStageID; }
	//{{ 2012. 11. 12	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CXSLDungeon::DUNGEON_TYPE GetDungeonType() const	{ return m_eDungeonType; }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	int				GetHighestUserLevelAtDungeonStart() const { return m_iHighestUserLevelAtDungeonStart; }
	//{{ 2012. 05. 10	������	���� ���� ���� ����
#ifdef SERV_DUNGEON_NPC_LEVEL
	int				GetAverageUserLevelAtDungeonStart() const { return m_iAverageUserLevelAtDungeonStart; }
#endif SERV_DUNGEON_NPC_LEVEL
	//}}

	KDropItemData	CreateItemProcess( IN int iItemID, IN bool bIsGP = false, IN int iED = 0 );	
	void			CheckLoadingSuccess();			//�Ѹ��� ������ ���涧 �ε�ó���� ���ؼ�
	void			CheckDungeonUnitInfoSuccess();	//���â���� �Ѿ�� �ٷ����� ������ �ñ��찡 �߻��Ͽ� ���������� ����ϱ�����
	//{{ 2009. 7. 27  ������	��ϸ� �ð�
	void			CheckEnterSecretStageSuccess();
	//}}
	//{{ 2010. 8. 16	������	���� �ڵ� ����
	bool			DungeonSubStageStartProcess( IN int iSubStageID );
	//}}

	//{{ 2010. 03. 24  ������	��д��� ����
#ifdef SERV_SECRET_HELL
    void			NormalDungeonAttribMonsterDrop( IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot );
	void			SecretDungeonExtraStageAttribMonsterDrop( IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot );
#endif SERV_SECRET_HELL
	//}}
	//{{ �̺�Ʈ ���� ���� ( �Ʊ� ����	) - ��μ�
#ifdef SERV_ALLY_EVENT_MONSTER
#ifdef SERV_ACTIVE_EVENT_MONSTER
	bool			EventDropMonster( IN int iNpcID, IN bool bAllyNpc, IN int iStageID, IN bool bActiveNpc, IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot );
#else
	bool			EventDropMonster( IN int iNpcID, IN bool bAllyNpc, IN int iStageID, IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot );
#endif SERV_ACTIVE_EVENT_MONSTER
#else
	bool			EventDropMonster( IN int iNpcID, IN int iStageID, IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot );
#endif SERV_ALLY_EVENT_MONSTER
	//}}
	//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void			SecretDungeonExtraStageMonster( IN const int iStageID, IN const short sPartyFever, IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot );
#else
	void			SecretDungeonExtraStageMonster( IN int iStageID, IN char cPartyFever, IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	bool			IsAttribNpc( IN int iNpcUID, OUT int& iNpcLv );

	//{{ 2009. 5. 11  ������	npc��������
	bool			IsDropDieState( IN char cState );
	//}}
	//{{ 2009. 7. 26  ������	ù��° ���� ��������
	bool			IsFirstSubStage() { return ( m_iStageID == 0  &&  m_iSubStageID == 0 ); }
	//}}
	//{{ 2010. 05. 02  ������	��д��� ����
#ifdef SERV_SECRET_HELL
	bool			IsEnteredExtraStage()	{ return ( m_iExtraStageLoadCount > 0 ); }	
#endif SERV_SECRET_HELL
	//}}
	//{{ 2009. 12. 14  ������	��ϸ��ӽ��κ�
	void			CheckTempInventoryClear();
	//}}
	//{{ 2010. 01. 05  ������	����������
	short			GetSubStageExpNpcNum( IN int iSubStageID ) const;
	//}}
#ifdef SERV_FIX_DUNGEON_TOTAL_PLAY_TIME_AT_LEAVE// �۾���¥: 2013-09-04	// �ڼ���
	void			ComputeTotalPlayTime( void );
#endif // SERV_FIX_DUNGEON_TOTAL_PLAY_TIME_AT_LEAVE
	//{{ 2010. 8. 16	������	���� �ڵ� ����
	double			GetTotalPlayTime() const	{ return m_fTotalPlayTime; }
	double			GetLoadingTime() const		{ return m_fLoadingTime; }
	//}}
	//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void			IncreasePartyFever();
	void			MaxPartyFever();
	bool			IsStartedByAutoParty() const		{ return m_bStartedByAutoParty; }
	virtual void	ReservedRegroupOldParty();
	virtual void	CheckReserveRegroupOldParty();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	int				GetProtectedSmallCrystalCount();
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
	void			CheckDeleteHenirDropItem( IN OUT KDropTable::DROP_DATA& sDropDataNpc, 
						  					IN OUT KDropTable::DROP_DATA& sDropDataStatic, 
											IN OUT KDropTable::DROP_DATA& sDropDataEvent, 
											IN OUT KDropTable::DROP_DATA& sDropDataAttribNpc,
											IN OUT KDropTable::DROP_DATA& sDropDataHenir );
#endif SERV_NEW_HENIR_TEST
	//}}

	//{{ 2012. 02. 06    ��μ�    ���� ���� UID �߱� ��� ����
#ifdef SERV_MAKE_MONSTER_UID_CHANGE
	bool			GetHackingNpcUid()								{ return m_bHackingNpcUid; }
	void			SetHackingNpcUid( IN bool bHacking )			{ m_bHackingNpcUid = bHacking; }
#endif SERV_MAKE_MONSTER_UID_CHANGE

	//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void			OnLeaveRoom( IN const UidType iLAST_SENDER_UID, IN const UidType iFIRST_SENDER_UID, IN const KERM_LEAVE_ROOM_REQ& kPacket_, IN const u_short usEventIDAck );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2012. 07. 04	������		��⿭ �ý���
#ifdef SERV_AUTO_RESURRECTION_BUG_FIX
	void			HenirAutoResurrection( IN const UidType iUnitUID );
#endif SERV_AUTO_RESURRECTION_BUG_FIX
	//}}
	//{{ 2012. 02. 29	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool			AutoPartyBuff( IN OUT std::map< UidType, std::vector< KBuffInfo > >& mapActivateBuffList );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 12. 17	������	�ƶ� ��Ƽ �÷��� ���ʽ� ����ġ
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	bool			PlayWithCharBuff( IN OUT std::map< UidType, std::vector< KBuffInfo > >& mapActivateBuffList );
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	//}}
#ifdef SERV_BUFF_BONUSRATE_HAMEL_EVENT
	void			PlayHamelEventBuff(IN OUT std::map< UidType, std::vector< KBuffInfo > >& mapActivateBuffList );
#endif SERV_BUFF_BONUSRATE_HAMEL_EVENT

#ifdef SERV_CRAYON_POP_EVENT_BUFF// �۾���¥: 2013-06-10	// �ڼ���
	void			CrayonPopEventBuff( IN OUT std::map< UidType, std::vector< KBuffInfo > >& mapActivateBuffList ) const;
#endif // SERV_CRAYON_POP_EVENT_BUFF

#ifdef SERV_BONUS_BUFF_SYSTEM
	bool			PlayWithBonusBuff( IN OUT std::map< UidType, std::vector< KBuffInfo > >& mapActivateBuffList );
#endif SERV_BONUS_BUFF_SYSTEM

	//{{ 2012. 05. 23	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	virtual void	TimeCountFinishProcess( IN const u_short usEventID );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	//{{ �ʵ� ��� ���� - ��μ�
#ifdef SERV_REFORM_ITEM_DROP
	float			GetDungeonPartyBonusRate();
#endif SERV_REFORM_ITEM_DROP
	//}}
	//{{ 2013. 04. 09	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	void			PrepareForEnterDefenceDungeon();
	//{{ 2013. 04. 22	������	����� �� ���� ���� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	void			EnterDefenceDungeon( IN const std::vector< UidType >& vecLeaveUnitUID );
#else
	//void			EnterDefenceDungeon();
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	//}}	
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

protected:
	KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT	m_kDSSGoNextAllNot;

	//{{ 2011. 06. 13	������	�������� �α�
#ifdef SERV_DUNGEON_STAGE_LOG
	int							m_iLoadingCompleteStageID;
#endif SERV_DUNGEON_STAGE_LOG
	//}}
	int							m_iStageID;
	int							m_iSubStageID;
	double						m_fTotalPlayTime; // ����&�������� �ε� �ð��� �� ���� �÷��� �ð��Դϴ�.
	double						m_fLoadingTime;
	bool						m_bIsWin;
	
	int							m_nDropItemUID;	// Drop item UID �߱�	
	//{{ 2011. 10. 25	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
#else
	int							m_nNPCUID;		// NPC Unit UID �߱�
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	u_short						m_usLoadingState;	//�ε����� üũ

	//{{ 2010. 07. 16  ������	�̺�Ʈ ���� �ý���
#ifdef SERV_EVENT_MONSTER
	//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	std::vector< KEventMonsterRefreshManager::KAdventNpcEventInfo > m_vecEventMonster;
#else
	std::vector< KGameEventScriptManager::KAdventNpcEventInfo > m_vecEventMonster;
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}
#endif SERV_EVENT_MONSTER
	//}}

	std::map< int, ITEM_DATA >	m_mapDropItem;			 // �ӽ÷� �߱��� item uid�� �����ߴ����� ���� üũ	[key:DropUID,	 value:ITEM_DATA]	
	//{{ 2011. 10. 25	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
#else
	//std::map< int, NPC_DATA >	m_mapNPCData;			 // ���� ������ NPC
	//std::map< KSubStageKey, SUB_STAGE_NPC_INFO > m_mapSubStageNpcInfo; // ���꽺���������� �����ϴ� npc uid����
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	std::map< int, int >		m_mapSubStageExpNpcNum;	 // ���꽺�������� exp npc��						[key:SubStageID, value:NpcCount]
	//{{ 2010. 11. 20	������	Ư�� �ð� ���� ����
#ifdef SERV_TIME_DROP_MONSTER_EVENT
	std::map< int, int >		m_mapTimeDropEventMonster; // Ư�� �ð��� �����ϴ� �̺�Ʈ ����			[key:DropUID, value:EventUID]
#endif SERV_TIME_DROP_MONSTER_EVENT
	//}}

	LOADING_TIMER				m_LoadingTimer;

	//{{ 2008. 10. 20  ������	�Ӽ� ���� ī��Ʈ
	std::map< int, int >		m_mapAttribNpcData;		// 1. npcUID    2. npcLv
	int							m_iAttribNpcStageCnt;
	int							m_iAttribNpcDropCnt;
	//}}

	//{{ 2010. 04. 07  ������	��д��� ����
#ifdef SERV_SECRET_HELL
	int							m_iExtraStageLoadCount;
	bool						m_bAdventExtraNpc;	
	float						m_fPoisonTimeLimit;
#endif SERV_SECRET_HELL
	//}}

	//{{ 2010. 07. 09  ������	��ӷ� �̺�Ʈ Ȯ��
#ifdef SERV_PC_BANG_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
	float						m_fItemDropEventProbRate;
#else // SERV_DROP_EVENT_RENEWAL
	int							m_iItemDropEventProbCount;
#endif // SERV_DROP_EVENT_RENEWAL
	bool						m_bWithPlayPcBangEvent;
#endif SERV_PC_BANG_DROP_EVENT
	//}}
	
	int							m_iHighestUserLevelAtDungeonStart;	// ���� ���۽ÿ� �����߿��� �ְ� ����
	//{{ 2012. 05. 10	������	���� ���� ���� ����
#ifdef SERV_DUNGEON_NPC_LEVEL
	int							m_iAverageUserLevelAtDungeonStart;	// ���� ���۽ÿ� �����߿��� ��� ����
#endif SERV_DUNGEON_NPC_LEVEL
	//}}
	CXSLDungeon::DUNGEON_TYPE	m_eDungeonType;						// ��ϸ� �ð�

	UidType						m_iPartyUID;

	//{{ 2010. 06. 17  ������	������ ����
#ifdef SERV_PROTECT_AUTO_HACK
	int							m_iHackUserCheckCount;
	int							m_iNpcDieHackCount;
	int							m_iGetItemHackCount;
	int							m_iStartedNumMember;
#endif SERV_PROTECT_AUTO_HACK
	//}}	
	//{{ 2010. 7. 28  ������	������ ���� NPC
#ifdef SERV_AUTO_HACK_CHECK_NPC
	std::set< UidType >			m_setFindedHackUserUnitUID;
#endif SERV_AUTO_HACK_CHECK_NPC
	//}}
	//{{ 2010. 11. 18	������	���� �� �������� �ε� üũ
#ifdef SERV_AUTO_HACK_CHECK_STAGE_LOAD
	std::map< int, int >		m_mapStageLoadingCheck;
	UidType						m_iLastLoadingUnitUID;
#endif SERV_AUTO_HACK_CHECK_STAGE_LOAD
	//}}
	//{{ 2011. 10. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KDungeonMonsterManager		m_kDungeonMonsterManager;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2011. 10. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KDungeonSecurityManager		m_kDungeonSecurityManager;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	int							m_iDefenseMainCrystalNpcUID;			// ū ���� NPCUID
	std::set< int >				m_setDefenseSmallCrystalNpcUID;			// ���� ���� NPCUID
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�


	//{{ 2011. 09. 07  ��μ�	���� �������� DIE NPC �ð� üũ ���
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
	std::map< int, int >		m_mapSubStageActiveNpcNum;		// ���꽺�������� Active npc��						[key:SubStageID, value:NpcCount]
	int							m_iSubStageActiveNpc;			// ���� ���� ���������� ��Ƽ�� ���� ��
	int							m_iSubStageKillActiveNpc;		// ���� ������������ ���� ���� ��
	CTime						m_tSubStageFirstKillTime;		// ���� ������������ ó�� ���� ���� �ð�
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
	//}}
	//{{ 2012. 03. 15	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool						m_bStartedByAutoParty;			// �ڵ� ��Ƽ�� ���� �������� ����
	std::vector< KERM_END_GAME_REGROUP_PARTY_NOT > m_vecReservedRegroupOldParty;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2012. 02. 06    ��μ�    ���� ���� UID �߱� ��� ����
#ifdef SERV_MAKE_MONSTER_UID_CHANGE
	bool						m_bHackingNpcUid;				// �߱޵��� �ʴ� NPC UID �� Ŭ���̾�Ʈ�� �������� �˻� - �ʱⰪ false
#endif SERV_MAKE_MONSTER_UID_CHANGE	
	//}}

	//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	KBadAttitudeManager			m_kBadAttitudeManager;
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}
	//{{ 2013. 02. 01  �̺�Ʈ �߷�Ÿ�� ���� - ��μ�
#ifdef SERV_EVENT_VALENTINE_DUNGEON
	KValentineManager			m_kValentineManager;			// �߷�Ÿ�� ���� �Ŵ���(�̺�Ʈ ����)
#endif SERV_EVENT_VALENTINE_DUNGEON
	//}

	//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
	float m_fQuestItemDropEventProbRate;;
#endif SERV_QUEST_ITEM_DROP_EVENT
	//}
	//{{ 2013. 04. 09	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	KDefenceDungeonManager		m_kDefenceDungeonManager;
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

#ifdef SERV_DUNGEON_STAGE_LOAD_LOG// �۾���¥: 2013-05-15	// �ڼ���
	std::vector<DungonStageLoadLog>		m_vecDungonStageLoadLog;
#endif // SERV_DUNGEON_STAGE_LOAD_LOG

#ifdef SERV_CREATED_NPC_LIMITED_DROPS
	struct NPC_INFO_QUADRUPLE
	{
		bool operator < ( const NPC_INFO_QUADRUPLE& right ) const
		{
			if( iDungeonID == right.iDungeonID )
			{
				if( iStageID == right.iStageID )
				{
					if( iSubStageID == right.iSubStageID )
					{
						if( iNpcID < right.iNpcID )
							return true;
					}
					else if( iSubStageID < right.iSubStageID )
					{
						return true;
					}
				}
				else if( iStageID < right.iStageID )
				{
					return true;
				}
			}
			else if( iDungeonID < right.iDungeonID )
			{
				return true;
			}

			return false;
		}

		int iDungeonID;
		int iStageID;
		int iSubStageID;
		int iNpcID;
	};
	std::map< NPC_INFO_QUADRUPLE, int > m_mapDroppedTimes;
#endif SERV_CREATED_NPC_LIMITED_DROPS

#ifdef  SERV_CHOOSE_FASTEST_HOST
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    int m_iSubStageCount;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#endif  SERV_CHOOSE_FASTEST_HOST

#ifdef SERV_STAGE_CLEAR_IN_SERVER// �۾���¥: 2013-10-30	// �ڼ���
	CXSLDungeonSubStage::NextStageData	m_kNextStageData;
	bool								m_bFirstStage;
	bool								m_bFirstSubStage;
	int									m_iClearConditionIndex;
#endif // SERV_STAGE_CLEAR_IN_SERVER
};


