#pragma once

#include "CommonPacket.h"

#pragma pack( push, 1 )

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_NEW_USER_JOIN_REQ )
{
	std::wstring            m_wstrID;
	std::wstring            m_wstrPassword;
	std::wstring            m_wstrName;
	byte					m_byteGuestUser;

	KEGS_NEW_USER_JOIN_REQ()
	{
		m_byteGuestUser = 0;
	};
};

DECL_PACKET( EGS_NEW_USER_JOIN_ACK )
{
	int                     m_iOK;
	UidType                 m_iUserUID;

	KEGS_NEW_USER_JOIN_ACK()
	{
		m_iOK = 0;
		m_iUserUID = 0;
	};
};


////////////////////////////////////////////////////////////////////////////
// KEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ -> send id
DECL_PACKET( EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK )
{
	int                     m_iOK;
	int                     m_nUnitSlot;
	std::vector<KUnitInfo>  m_vecUnitInfo;
#ifdef SERV_SHARING_BANK_QUEST_CASH
	bool					m_bSharingBank;
#endif SERV_SHARING_BANK_QUEST_CASH
#ifdef SERV_4TH_ANNIVERSARY_EVENT
	K4ThAnnivEventInfo		m_4ThAnnivEventInfo;
#endif // SERV_4TH_ANNIVERSARY_EVENT

	KEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK()
		: m_iOK( 0 )
        ,m_nUnitSlot(0)
	{
        m_vecUnitInfo.clear();
	}
};

////////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CREATE_UNIT_REQ )
{
	std::wstring            m_wstrNickName;
	int                     m_iClass;
};

DECL_PACKET( EGS_CREATE_UNIT_ACK )
{
	int                     m_iOK;
	KUnitInfo               m_kUnitInfo;
	std::vector<int>		m_vecSkillSlot;
	//{{ 2008. 4. 8  ������  ������ �г��� ��� ���� ��¥
	std::wstring			m_wstrEnableDate;
	//}}	
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
	//{{ 2012.02.21 ��ȿ��  ���� �̸� ���� ���μ��� �ָ��Ѱ� ���� 
	bool					m_bReqGiant_DeleteName;
	std::wstring            m_wstrNickName;
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

	KEGS_CREATE_UNIT_ACK()
	{
#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN	
		m_bReqGiant_DeleteName	= false;
		m_wstrNickName			= L"";
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
	}
#else //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
	//{{ 2012.02.21 ��ȿ��  ���� �̸� ���� ���μ��� �ָ��Ѱ� ���� 
	bool					m_bReqGiant_DeleteName;
	std::wstring            m_wstrNickName;

	KEGS_CREATE_UNIT_ACK()
	{
		m_bReqGiant_DeleteName	= false;
		m_wstrNickName			= L"";
	}
	//}}
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
};

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_DELETE_UNIT_REQ )
{
	UidType                 m_iUnitUID;
	//{{ 2012. 02. 21	��μ�	ĳ���� ���� �� ��� Ż�� ����ó�� ����
#ifdef SERV_UNIT_DELETE_EXCEPTION_MODIFY
	UidType                 m_iUserUID;
#endif SERV_UNIT_DELETE_EXCEPTION_MODIFY
	//}}

	KEGS_DELETE_UNIT_REQ()
	{
		m_iUnitUID		= 0;
		//{{ 2012. 02. 21	��μ�	ĳ���� ���� �� ��� Ż�� ����ó�� ����
#ifdef SERV_UNIT_DELETE_EXCEPTION_MODIFY
		m_iUserUID		= 0;
#endif SERV_UNIT_DELETE_EXCEPTION_MODIFY
		//}}
	}
};

DECL_PACKET( EGS_DELETE_UNIT_ACK )
{
	int                     m_iOK;
    UidType                 m_iUnitUID;
	u_int					m_uiKNMSerialNum;
	//{{ 2009. 10. 8  ������	���
#ifdef GUILD_TEST
	int						m_iGuildUID;
#endif GUILD_TEST
	//}}
	//{{ 2011. 02. 23	������	ĳ���� �α�
#ifdef SERV_CHAR_LOG
	std::wstring			m_wstrNickName;
	u_char					m_ucLevel;
	char					m_cUnitClass;
#endif SERV_CHAR_LOG
	//}}
	//{{ 2012.02.20 ��ȿ��	ĳ���� ���� ���μ��� ���� (���� ��� �Ⱓ ����)
#ifdef SERV_UNIT_WAIT_DELETE
	//{{ // 2012.03.06 lygan_������ // ���� ���ڿ��ð��� �״�� �޴� �κ��� �������� ���ڷ� �����ؼ� Ŭ��� ���� ������ ����
	__int64					m_tReDelAbleDate;
	__int64					m_tDelAbleDate; //2012.03.05 lygan_������ // ĳ���� ���� �Ҷ� ���� ���� ������ ���� �˱� ���� 
	//}}
#endif SERV_UNIT_WAIT_DELETE
	//}}
};


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 01. 04  ������	�κ��丮 ��ɰ���
DECL_PACKET( EGS_GET_MY_INVENTORY_ACK )
{
	int										m_iOK;
	bool									m_bIsRecommend;
#ifdef SERV_SHARING_BANK_QUEST_CASH
	bool									m_bSharingBank;
#endif SERV_SHARING_BANK_QUEST_CASH

	KEGS_GET_MY_INVENTORY_ACK()
	{
	};
};

DECL_PACKET( EGS_SELECT_UNIT_INVENTORY_INFO_NOT )
{
	int										m_iSlotCategory;
	std::map< UidType, KInventoryItemInfo > m_mapItem;
};
//}}


//////////////////////////////////////////////////////////////////////////
//EGS_STATE_CHANGE_SERVER_SELECT_REQ
DECL_PACKET( EGS_STATE_CHANGE_SERVER_SELECT_ACK )
{
	int			m_iOK;
};


//////////////////////////////////////////////////////////////////////////
DECL_DATA( KServerSetData )
{
	enum USER_COUNT_LEVEL
	{
		UCL_INVALID = 0,
		UCL_FREE,		//30%�̸�
		UCL_NORMAL,		//70%�̸�
		UCL_BUSY,		//100%�̸�
		UCL_FULL,		//���ӺҰ�
	};

	int					serverSetID;
	std::wstring		serverSetName;
	char				userCountLevel;

	CON_COPYCON_ASSIGNOP( KServerSetData, right )
	{
		serverSetID = right.serverSetID;
		serverSetName = right.serverSetName;
		userCountLevel = right.userCountLevel;

		return *this;
	}
};


//////////////////////////////////////////////////////////////////////////
//EGS_GET_SERVER_SET_DATA_REQ

DECL_PACKET( EGS_GET_SERVER_SET_DATA_ACK )
{
	int							m_iOK;
	std::vector<KServerSetData>	m_ServerSetList;
};

DECL_PACKET( EGS_SELECT_SERVER_SET_REQ )
{
	int							m_ServerSetID;
};

DECL_PACKET( EGS_SELECT_SERVER_SET_ACK )
{
    int							m_iOK;
    std::wstring				m_wstrIP;
    unsigned short				m_usPort;

	KAccountBlockInfo			m_kAccountBlockInfo;
};

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CHECK_CHANNEL_CHANGE_REQ )
{
	int							m_iChannelID;
};

typedef KPacketOK		KEGS_CHECK_CHANNEL_CHANGE_ACK;


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 5. 26  ������	ä���̵�
DECL_PACKET( EGS_CONNECT_CHANNEL_CHANGE_REQ )
{
    KEGS_CONNECT_REQ			m_kConnectReq;
	KEGS_VERIFY_ACCOUNT_REQ		m_kVerifyAccountReq;
	KEGS_SELECT_UNIT_REQ		m_kSelectUnitReq;
};

DECL_PACKET( EGS_CONNECT_CHANNEL_CHANGE_ACK )
{
	int							m_iOK;
	KEGS_CONNECT_ACK			m_kConnectAck;
	KEGS_VERIFY_ACCOUNT_ACK		m_kVerifyAccountAck;
	KChannelChangeInfo			m_kChangeUserInfo;
};

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_JOIN_FIELD_CHANNEL_CHANGE_REQ )
{
	int									m_iMapID;
	VECTOR3								m_vPos;
};

typedef KPacketOK		KEGS_JOIN_FIELD_CHANNEL_CHANGE_ACK;
//}}

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_ENTER_NPC_HOUSE_REQ )
{
	int							m_iNPCID;
};

//{{ 2009. 2. 13  ������	ä�� ����Ʈ
typedef KPacketOK		KECH_GET_CHANNEL_LIST_ACK;

DECL_PACKET( ECH_GET_CHANNEL_LIST_NOT )
{
	int								m_iServerGroupID;
	std::map< int, KChannelInfo >	m_mapChannelList;

	//{{ 2012. 06. 29	�ڼ���	ä�� UI ���������� ���� ������ �ý��� ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
	std::map< int, KChannelBonusInfo >	m_mapChannelBonusList;
#endif SERV_CHANNEL_LIST_RENEWAL
	//}}
};

DECL_PACKET( EGS_GET_CHANNEL_LIST_ACK )
{
	int									m_iServerGroupID;
#ifdef SERV_CHANNEL_LIST_RENEWAL
	std::vector< KChannelInfo >			m_vecChannelInfo;
	std::map< int, KChannelBonusInfo >	m_mapChannelBonusInfo;
#else //SERV_CHANNEL_LIST_RENEWAL
    std::vector< KChannelSimpleInfo >	m_vecChannelList;
#endif //SERV_CHANNEL_LIST_RENEWAL
};
//}}

DECL_PACKET( EGS_ENTRY_POINT_GET_CHANNEL_LIST_ACK )
{
    std::map< int, KChannelInfo >	m_mapSolesChannelList; // ���� �������� ä�� ����Ʈ. �ַ���/���̾� ������ �ƴϹǷ� ��������.
    std::map< int, KChannelInfo >	m_mapGaiaChannelList; // �ٸ� �������� ä�� ����Ʈ. 
    std::map< int, KChannelBonusInfo >	m_mapSolesChannelBonusList;
    std::map< int, KChannelBonusInfo >	m_mapGaiaChannelBonusList;

    KEGS_ENTRY_POINT_GET_CHANNEL_LIST_ACK()
    {
        m_mapSolesChannelList.clear();
        m_mapGaiaChannelList.clear();
        m_mapSolesChannelBonusList.clear();
        m_mapGaiaChannelBonusList.clear();
    }
};

//////////////////////////////////////////////////////////////////////////
DECL_DATA( KStateChangeReq )
{
	UINT							m_nMapID;
};

DECL_DATA( KStateChangeAck )
{
	int								m_iOK;
	UINT							m_nMapID;
};


/////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_ROOM_LIST_REQ )
{
	UINT							m_nViewPage;
	char							m_cPVPGameType;
};

DECL_PACKET( EGS_ROOM_LIST_ACK )
{
	int								m_iOK;
	UINT							m_nTotalPage;
	UINT							m_nViewPage;

	std::vector< KRoomInfo >		m_vRoomInfo;
};


/////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CREATE_ROOM_REQ )
{
	bool							m_bObserver;
	KRoomInfo						m_RoomInfo;

	KEGS_CREATE_ROOM_REQ()
	{
		m_bObserver = false;
	}
};

DECL_PACKET( EGS_CREATE_ROOM_ACK )
{
	int                                 m_iOK;
	KRoomInfo						    m_RoomInfo;
	std::vector< KRoomSlotInfo >        m_vecSlot;
	std::vector< KRoomSlotInfo >		m_vecObserverSlot;

	std::wstring						m_wstrCNIP;
};


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 12. 4  ������	Ʃ�丮�� ����
DECL_PACKET( EGS_CREATE_TUTORIAL_ROOM_REQ )
{
	KRoomInfo							m_RoomInfo;
};

DECL_PACKET( EGS_CREATE_TUTORIAL_ROOM_ACK )
{
	int                                 m_iOK;
	KRoomInfo						    m_RoomInfo;
	std::vector< KRoomSlotInfo >        m_vecSlot;
	std::wstring						m_wstrCNIP;
};
//}}


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CREATE_TC_ROOM_REQ )
{
	int									m_iTCID;
};

DECL_PACKET( EGS_CREATE_TC_ROOM_ACK )
{
	int									m_iOK;	
	KTCInfo								m_kTCInfo;
	KRoomUserInfo						m_kRoomUserInfo;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_QUICK_JOIN_REQ )
{
    int		m_iOption;
#ifdef DUNGEON_ITEM
	bool	m_bIsItemMode;

	KEGS_QUICK_JOIN_REQ()
	{
		m_bIsItemMode = false;
	}
#endif DUNGEON_ITEM
};

DECL_PACKET( EGS_QUICK_JOIN_ACK )
{
    int                                 m_iOK;
    KRoomInfo						    m_RoomInfo;
    std::vector< KRoomSlotInfo >		m_vecSlot;	
};


/////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_JOIN_ROOM_REQ )
{
	UidType								m_iRoomUID;
	std::wstring						m_wstrPassword;
	bool								m_bIsObserver;
#ifdef SERV_NEW_PVPROOM_PROCESS
	bool								m_bInvitation;
#endif SERV_NEW_PVPROOM_PROCESS
};

DECL_PACKET( EGS_JOIN_ROOM_ACK )
{
	int                                 m_iOK;
	KRoomInfo						    m_RoomInfo;
	std::vector< KRoomSlotInfo >        m_vecSlot;
	std::vector< KRoomSlotInfo >		m_vecObserverSlot;

	std::wstring						m_wstrCNIP;
};

DECL_PACKET( EGS_JOIN_ROOM_NOT )
{
    KRoomSlotInfo						m_JoinSlot;
	//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	int									m_StartPosIndex;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	KEGS_JOIN_ROOM_NOT()
	{
		//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_StartPosIndex = 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_LEAVE_ROOM_REQ )
{
	int									m_iReason;
	//{{ 2012. 04. 17	������	���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	KDungeonPlayResultInfo				m_kResultInfo;
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}
	//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	int									m_iBattleFieldID;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 10. 16	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	KGamePlayNetworkInfoPackage			m_kGamePlayNetworkInfo;
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	//}}

	KEGS_LEAVE_ROOM_REQ()
	{
		m_iReason = 0;
		//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_iBattleFieldID = 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}
};

//{{ 2011. 02. 21  ��μ�	Ư�� ���� ���� ������ ���� ����
//#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
DECL_PACKET( EGS_LEAVE_ROOM_ACK )
{
	int									m_iOK;		// �� ��Ż ���� ����
	int									m_iReason;	// �� ��Ż ����
	int									m_iItemID;
	//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool								m_bNotLeaveParty;
	KBattleFieldJoinInfo				m_kBattleFieldJoinInfo;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	KEGS_LEAVE_ROOM_ACK()
	{
		m_iOK = 0;
		m_iReason = 0;
		m_iItemID = 0;
		//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_bNotLeaveParty = false;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}
};
//#else
//typedef KPacketOK					KEGS_LEAVE_ROOM_ACK;
//#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
//}}

typedef KEGS_LEAVE_ROOM_REQ	KEGS_LEAVE_SQUARE_REQ;
typedef KPacketOK			KEGS_LEAVE_SQUARE_ACK;

DECL_PACKET( EGS_LEAVE_ROOM_NOT )
{
	char                            m_cRoomState;
    UidType                         m_iUnitUID;
	int                             m_iReason;
    std::vector< KRoomSlotInfo >    m_vecSlot;
	std::vector< KRoomSlotInfo >    m_vecObserverSlot;
};

DECL_PACKET( EGS_LEAVE_SQUARE_NOT )
{
	UidType                         m_iUnitUID;
	int                             m_iReason;
};

//////////////////////////////////////////////////////////////////////////
//						KEGS_LEAVE_GAME_REQ -> send id
typedef KPacketOK		KEGS_LEAVE_GAME_ACK;

DECL_PACKET( EGS_LEAVE_GAME_NOT )
{
    char                            m_cRoomState;
    UidType                         m_iUnitUID;
    std::vector< KRoomSlotInfo >    m_vecSlot;
	std::vector< KRoomSlotInfo >    m_vecObserverSlot;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_BAN_USER_REQ )
{
	UidType						    m_iUnitUID;
};

// KEGS_BAN_USER_ACK -> KPacketOK


/////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CHANGE_TEAM_REQ )
{
	char							m_TeamNum;
};

DECL_PACKET( EGS_CHANGE_TEAM_ACK )
{
	int								m_iOK;
};

DECL_PACKET( EGS_CHANGE_TEAM_NOT )
{
	UidType							m_UnitUID;
	char							m_TeamNum;
	char							m_SwapSlotIndex;
};


/////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CHANGE_READY_REQ )
{
	bool							m_bReady;
};

DECL_PACKET( EGS_CHANGE_READY_ACK )
{
	int								m_iOK;
};

DECL_PACKET( EGS_CHANGE_READY_NOT )
{
	UidType							m_UnitUID;
	bool							m_bReady;
};

DECL_PACKET( EGS_CHANGE_PITIN_REQ )
{
	bool							m_bPitIn;
};

typedef KPacketOK	KEGS_CHANGE_PITIN_ACK;

DECL_PACKET( EGS_CHANGE_PITIN_NOT )
{
	UidType							m_UnitUID;
	bool							m_bPitIn;
};


/////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CHANGE_MAP_REQ )
{
	short							m_WorldID;
};

DECL_PACKET( EGS_CHANGE_MAP_ACK )
{
	int								m_iOK;
};

DECL_PACKET( EGS_CHANGE_MAP_NOT )
{
	short							m_WorldID;
};


/////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CHANGE_SLOT_OPEN_REQ )
{
	char							m_SlotIndex;
	char							m_SlotState;
};

DECL_PACKET( EGS_CHANGE_SLOT_OPEN_ACK )
{
	int								m_iOK;
};

DECL_PACKET( EGS_CHANGE_SLOT_OPEN_NOT )
{
	char							m_SlotIndex;
	char							m_TeamBalanceSlotIndex;
	char							m_SlotState;
};


/////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CHANGE_PLAY_TIME_LIMIT_REQ )
{
	float							m_fPlayTime;
};

typedef KPacketOK   KEGS_CHANGE_PLAY_TIME_LIMIT_ACK;

DECL_PACKET( EGS_CHANGE_PLAY_TIME_LIMIT_NOT )
{
	float							m_fPlayTime;
};


/////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CHANGE_WINNING_NUM_KILL_REQ )
{
	int								m_iWinKillNum;
};

typedef KPacketOK   KEGS_CHANGE_WINNING_NUM_KILL_ACK;

DECL_PACKET( EGS_CHANGE_WINNING_NUM_KILL_NOT )
{
	int								m_iWinKillNum;
};

/////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CHAT_REQ )
{
	enum CHAT_PACKET_TYPE
	{
		CPT_TOTAL,
		CPT_PARTY,
		CPT_SYSTEM,
		//{{ 2009. 9. 17  ������	���
		CPT_GUILD,
		//}}		
		CPT_WHISPER,
        CPT_MESSENGER,
		CPT_MEGAPHONE,
		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
		CPT_RELATIONSHIP,		// �ο� ä��
#endif SERV_RELATIONSHIP_SYSTEM
		//}
	};

	char							m_cChatPacketType;
	char							m_cRoomChatType;
	UidType							m_ToUnitUID;
	std::wstring					m_wstrRecieverUnitNickName;
	std::wstring					m_wstrMsg;
};

typedef KPacketOK	KEGS_CHAT_ACK;

DECL_PACKET( EGS_CHAT_NOT )
{
	char							m_cChatPacketType;
	UidType							m_SenderUnitUID;
	std::wstring					m_wstrSenderUnitNickName;
	std::wstring					m_wstrRecieverUnitNickName;
	UidType							m_RecieverUnitUID;
	char							m_cRoomChatType;
	std::wstring					m_wstrMsg;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CHANGE_MOTION_REQ )
{
	char							m_cMotionID;
};

DECL_PACKET( EGS_CHANGE_MOTION_ACK )
{
	int								m_iOK;
};

DECL_PACKET( EGS_CHANGE_MOTION_NOT )
{
	UidType							m_UnitUID;
	char							m_cMotionID;
};


/////////////////////////////////////////////////////////////////////////
//{{ 2008. 5. 27  ������  ������
DECL_PACKET( EGS_STATE_CHANGE_GAME_START_REQ )
{
	short							m_sWorldID;
	//{{ 2009. 1. 21  ������	������ ���ӽ��۽� ��������� üũ
	int								m_iDungeonID;
	//}}
};
//}}

typedef KPacketOK						KEGS_STATE_CHANGE_GAME_START_ACK;

DECL_PACKET( EGS_STATE_CHANGE_GAME_START_NOT )
{
    char								m_cRoomState;
	int									m_iGameType;
	int									m_iSpirit;
	//{{ 2008. 5. 27  ������  ������
	short								m_sWorldID;
	//}}
    std::vector< KRoomSlotInfo >		m_vecSlot;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};


/////////////////////////////////////////////////////////////////////////
//										KEGS_STATE_CHANGE_GAME_INTRUDE_REQ -> send id
typedef KPacketOK						KEGS_STATE_CHANGE_GAME_INTRUDE_ACK;

DECL_PACKET( EGS_STATE_CHANGE_GAME_INTRUDE_NOT )
{
	char							m_RoomState;
	KRoomSlotInfo					m_IntruderSlot;
};


/////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_GAME_LOADING_REQ )
{
	int                             m_iLoadingProgress;
	//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	int                             m_iUnitLevel;			// Ŭ���̾�Ʈ�� ��� ����

	KEGS_GAME_LOADING_REQ()
	{
		m_iLoadingProgress  = 0 ;
		m_iUnitLevel  = 0 ;
	}
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	//}}
};

typedef KPacketOK	KEGS_GAME_LOADING_ACK;

DECL_PACKET( EGS_GAME_LOADING_NOT )
{
	UidType                         m_iUnitUID;
	int                             m_iLoadingProgress;
};
// KEGS_GAME_LOADING_ALL_UNIT_OK_NOT -> send id


/////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_GAME_P2P_CONNECT_RESULT_REQ )
{
	bool							m_bConnectSuccess;
	UidType							m_UnitUID;
};

DECL_PACKET( EGS_GAME_P2P_CONNECT_RESULT_ACK )
{
	int								m_iOK;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_PLAY_START_NOT )
{
	char							m_RoomState;
	std::vector< KRoomSlotInfo >    m_vecSlot;
	//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
	int								m_iDungeonID;
#endif SERV_CHAR_LOG
	//}}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_INTRUDE_START_REQ )
{
	int								m_StartPosIndex;
};

DECL_PACKET( EGS_INTRUDE_START_ACK )
{
	int								m_iOK;
};

DECL_PACKET( EGS_INTRUDE_START_NOT )
{
	char							m_RoomState;
	int								m_StartPosIndex;
	KRoomSlotInfo					m_IntruderSlot;
	//{{ 2012. 12. 5	������		������ ���� ���� ���
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
	bool							m_bObserver;
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
	//}}

	KEGS_INTRUDE_START_NOT()
	{
		m_RoomState = 0;
		m_StartPosIndex = 0;
		//{{ 2012. 12. 5	������		������ ���� ���� ���
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
		m_bObserver = false;
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
		//}}
	}
};

/////////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CURRENT_KILL_SCORE_NOT )
{
	std::map< UidType, std::pair< int, int > >      m_mapKillScore;
	std::map< UidType, int >						m_mapMDKillScore;
};

DECL_PACKET( EGS_CURRENT_TEAM_KILL_SCORE_NOT )
{
	char											m_RedTeamKill;
	char											m_BlueTeamKill;
};

typedef float KEGS_REMAINING_PLAY_TIME_NOT;

// KEGS_PLAY_TIME_OUT_NOT -> send id


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_MY_USER_UNIT_INFO_TO_SERVER_REQ )
{
	//{{ 2011. 01. 21	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
	KDungeonPlayResultInfo			m_kMyPlayResult;
#else
	float							m_fHP;
	int								m_nDamageCount;

	int								m_TotalScore;
	int								m_ComboScore;
	int								m_TechScore;
#endif SERV_DUNGEON_RANK_NEW
	//}}
};

// EGS_MY_USER_UNIT_INFO_TO_SERVER_ACK -> KPacketOK

//{{ 2008. 9. 19  ������	���
DECL_PACKET( EGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT )
{
	int								m_ComboScore;
	int								m_TechScore;
	int								m_nDamageCount;
	//{{ 2011. 01. 21	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
	int								m_iTotalAttackedDamage;
	int								m_iTotalRecoveryHP;
	int								m_iUsingKindOfSkill;
#endif SERV_DUNGEON_RANK_NEW
	//}}
};
//}}

DECL_DATA( KDungeonUnitResultInfo )	//���ó���� ���� unit info
{
	UidType			m_UnitUID;
	bool			m_bIsDie;

	int				m_nTotalScore;
	int				m_nComboScore;
	int				m_nTechnicalScore;
	int				m_nDamaged;
	//{{ 2009. 6. 10  ������	���ӽ��۽� EXP, ED
	int				m_nOldED;
	int				m_nOldEXP;
	//}}
	int				m_nED;
	int				m_nEXP;

	int				m_nClearBonusEXP;
	int				m_nRankBonusEXP;
	int				m_nPartyBonusEXP;
	int				m_nMvpBonusEXP;
	int				m_nGameBangBonusEXP;
	int				m_nPremiumBonusEXP;
	int				m_nEventEXP;
	//{{ 2009. 5. 6  ������		ä�κ��ʽ�
	int				m_nChannelBonusEXP;
	//}}

	int				m_nKillNPCNum;
	char			m_cComboRank;
	char			m_cTechnicalRank;
	char			m_cDamagedRank;
	char			m_cTimeRank;
	char			m_cTotalRank;
	bool			m_bIsMVP;

	//������� UNIT�� ���������� ���Ǿ� �����ʱ�ȭ�� �����д�.
	KDungeonUnitResultInfo()
	{
		m_UnitUID			= 0;
		m_bIsDie			= false;

		m_nTotalScore		= 0;
		m_nComboScore		= 0;
		m_nTechnicalScore	= 0;
		m_nDamaged			= 0;
		//{{ 2009. 6. 10  ������	���ӽ��۽� EXP, ED
		m_nOldED			= 0;
		m_nOldEXP			= 0;
		//}}
		m_nED				= 0;		
		m_nKillNPCNum		= 0;
		m_cComboRank		= 1;
		m_cTechnicalRank	= 1;
		m_cDamagedRank		= 1;
		m_cTimeRank			= 1;
		m_cTotalRank		= 1;
		m_bIsMVP			= 1;

		ResetEXP();
	}

	COPYCON_ASSIGNOP( KDungeonUnitResultInfo, right )
	{
		m_UnitUID = right.m_UnitUID;
		m_bIsDie = right.m_bIsDie;
		m_nTotalScore = right.m_nTotalScore;
		m_nComboScore = right.m_nComboScore;
		m_nTechnicalScore = right.m_nTechnicalScore;
		m_nDamaged = right.m_nDamaged;
		//{{ 2009. 6. 10  ������	���ӽ��۽� EXP, ED
		m_nOldED = right.m_nOldED;
		m_nOldEXP = right.m_nOldEXP;
		//}}
		m_nED = right.m_nED;
		m_nEXP = right.m_nEXP;
		m_nClearBonusEXP = right.m_nClearBonusEXP;
		m_nRankBonusEXP = right.m_nRankBonusEXP;
		m_nPartyBonusEXP = right.m_nPartyBonusEXP;
		m_nMvpBonusEXP = right.m_nMvpBonusEXP;
		m_nGameBangBonusEXP = right.m_nGameBangBonusEXP;
		m_nPremiumBonusEXP = right.m_nPremiumBonusEXP;
		m_nEventEXP = right.m_nEventEXP;
		//{{ 2009. 5. 6  ������		ä�κ��ʽ�
		m_nChannelBonusEXP = right.m_nChannelBonusEXP;
		//}}
		m_nKillNPCNum = right.m_nKillNPCNum;
		m_cComboRank = right.m_cComboRank;
		m_cTechnicalRank = right.m_cTechnicalRank;
		m_cDamagedRank = right.m_cDamagedRank;
		m_cTimeRank = right.m_cTimeRank;
		m_cTotalRank = right.m_cTotalRank;
		m_bIsMVP = right.m_bIsMVP;

		return *this;
	}

	void ResetEXP()
	{
		m_nEXP				= 0;
		m_nClearBonusEXP	= 0;
		m_nRankBonusEXP		= 0;
		m_nPartyBonusEXP	= 0;
		m_nMvpBonusEXP		= 0;
		m_nGameBangBonusEXP	= 0;
		m_nPremiumBonusEXP	= 0;
		m_nEventEXP			= 0;
		//{{ 2009. 5. 6  ������		ä�κ��ʽ�
		m_nChannelBonusEXP	= 0;
		//}}
	}

	int SumEXP()
	{
		return (m_nEXP + 
				m_nClearBonusEXP + 
				m_nRankBonusEXP + 
				m_nPartyBonusEXP + 
				m_nMvpBonusEXP + 
				m_nGameBangBonusEXP + 
				m_nPremiumBonusEXP +
				m_nEventEXP +
				//{{ 2009. 5. 6  ������		ä�κ��ʽ�
				m_nChannelBonusEXP
				//}}
				);
	}
};

DECL_PACKET( EGS_END_GAME_DUNGEON_RESULT_DATA_NOT )
{
	int										m_nSec;
	bool									m_bIsWin;
	int										m_iDungeonID;
	char									m_cDifficulty;
	//{{ 2010. 05. 01  ������	��д��� ����
#ifdef SERV_SECRET_HELL
	char									m_cDungeonMode;
#endif SERV_SECRET_HELL
	//}}
	std::vector< KDungeonUnitResultInfo >	m_vecDungeonUnitInfo;
	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	std::map< UidType, std::map< int, KSubQuestInfo > > m_mapOngoingQuest;
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

	//{{ 2009. 8. 25  ������	���б� �̺�Ʈ
	bool									m_bIsHenirDungeon;
	int										m_iDungeonEventPoint;
	int										m_iHenirEventPoint;
	//}}

	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	bool GetOngoingQuestByUnitUID( UidType iUnitUID, std::map< int, KSubQuestInfo >& mapOngoingQuest )
	{
		mapOngoingQuest.clear();

		std::map< UidType, std::map< int, KSubQuestInfo > >::const_iterator mit;
		mit = m_mapOngoingQuest.find( iUnitUID );
		if( mit == m_mapOngoingQuest.end() )
			return false;

		mapOngoingQuest = mit->second;
		return true;
	}
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

	//{{ 2011. 03. 08	��μ� ����ġ�� ȹ���� ������ ����Ʈ �Ϸ� ���� ����
#ifdef SERV_EXP_ACQUISITION_CONDITION_CHANGE
	std::map< UidType, bool >				m_mapHaveExpInDungeon;			// �������� ����ġ�� �������� �ִ°�?
#endif SERV_EXP_ACQUISITION_CONDITION_CHANGE
	//}}
	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	bool									m_bIsClearDefenceDungeon;		// ���潺 ���� ��� ����?
	int										m_iProtectedSmallCrystal;			// ��ȣ�� ũ����Ż ��
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM		
	bool									m_bComeBackUserInParty;
#endif SERV_SERVER_BUFF_SYSTEM
	//}

	//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	std::map< UidType, bool >				m_mapSuitableLevelInfo;			// ���� ������ ���� Ŭ���� �Ѱ��ΰ�?
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	//}}
#ifdef SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
	bool									m_bSpecChar1;
	bool									m_bSpecChar2;
#endif SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT

#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
	std::vector<UidType>					m_vecRelation;
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST

#ifdef SERV_THREE_COLOR_EVENT
	std::set< int >							m_setEquippedTitle;
#endif SERV_THREE_COLOR_EVENT

#ifdef SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
	KEGS_END_GAME_DUNGEON_RESULT_DATA_NOT()
	{
		m_bSpecChar1 = false;
		m_bSpecChar2 = false;
	}
#endif SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
};

DECL_DATA( KPVPUnitResultInfo )
{
	UidType			m_UnitUID;

	int				m_nRanking;
	int				m_nKillNum;
	int				m_nMDKillNum;
	int				m_nDeathNum;
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	int				m_iRating;
	int				m_iRPoint;
	int				m_iAPoint;

	//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	float			m_fKFactorIncrement;
#endif SERV_2012_PVP_SEASON2
	//}}

#else
	int				m_nVSPoint;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	int				m_nEXP;

	bool			m_bIsMVP;
	bool			m_bWin;

	KPVPUnitResultInfo()
	{
		m_UnitUID		= 0;

		m_nRanking		= 0;
		m_nKillNum		= 0;
		m_nMDKillNum	= 0;
		m_nDeathNum		= 0;
		//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		m_iRating		= 0;
		m_iRPoint		= 0;
		m_iAPoint		= 0;

		//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		m_fKFactorIncrement	= 0.0f;
#endif SERV_2012_PVP_SEASON2
		//}}

#else
		m_nVSPoint		= 0;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		m_nEXP			= 0;

		m_bIsMVP		= false;
		m_bWin			= false;
	}

	KPVPUnitResultInfo( const KPVPUnitResultInfo& src ){ *this = src; }

	const KPVPUnitResultInfo& operator = (const KPVPUnitResultInfo& right)
	{
		m_UnitUID		= right.m_UnitUID;
		m_nRanking		= right.m_nRanking;		
		m_nKillNum		= right.m_nKillNum;
		m_nMDKillNum	= right.m_nMDKillNum;
		m_nDeathNum		= right.m_nDeathNum;
		//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		m_iRating		= right.m_iRating;
		m_iRPoint		= right.m_iRPoint;
		m_iAPoint		= right.m_iAPoint;

		//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		m_fKFactorIncrement	= right.m_fKFactorIncrement;
#endif SERV_2012_PVP_SEASON2
		//}}

#else
		m_nVSPoint		= right.m_nVSPoint;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		m_nEXP			= right.m_nEXP;
		m_bIsMVP		= right.m_bIsMVP;
		m_bWin			= right.m_bWin;

		return *this;
	}
};

DECL_PACKET( EGS_END_GAME_PVP_RESULT_DATA_NOT )
{
	char								m_cWinTeam;
	bool								m_bIsDrawn;
	//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	bool								m_bIsAbnormalEndPlay;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	int									m_iPlayTime;
	int									m_iGameType;
	int									m_iSpirit;
	UINT								m_uiChannelID;

	std::vector< KPVPUnitResultInfo >	m_vecPVPUnitInfo;

	//{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
//{{ 2012. 02. 22	��μ�	���� ����Ʈ, PVP NPC ���� ����Ʈ	���� ����
#ifdef SERV_NEW_PVP_QUEST_ERROR_MODIFY
#else
	bool								m_bMatchNPC;
#endif SERV_NEW_PVP_QUEST_ERROR_MODIFY
//}}	
	bool								m_bOfficialMatch;
#endif SERV_NEW_PVP_QUEST
	//}} 
#ifdef PVP_QUEST_HERO_KILL_COUNT
	bool								m_bIsHeroNPC;
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_PVP_REMATCH
	int									m_iRematchCount;
#endif SERV_PVP_REMATCH

	KEGS_END_GAME_PVP_RESULT_DATA_NOT()
	{
		m_cWinTeam			 = 0;
		m_bIsDrawn			 = false;
		//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		m_bIsAbnormalEndPlay = false;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		m_iPlayTime			 = 0;
		m_iGameType			 = 0;
		m_iSpirit			 = 0;
		m_uiChannelID		 = 0;
		//{{ 2011. 07. 25    ��μ�    ���� ����Ʈ ���� �߰�
#ifdef SERV_NEW_PVP_QUEST
//{{ 2012. 02. 22	��μ�	���� ����Ʈ, PVP NPC ���� ����Ʈ	���� ����
#ifdef SERV_NEW_PVP_QUEST_ERROR_MODIFY
#else
		m_bMatchNPC			 = false;
#endif SERV_NEW_PVP_QUEST_ERROR_MODIFY
//}}		
		m_bOfficialMatch	 = false;
#endif SERV_NEW_PVP_QUEST
		//}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
		m_bIsHeroNPC		= false;
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_PVP_REMATCH
		m_iRematchCount		= 0;
#endif SERV_PVP_REMATCH
	}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_USER_UNIT_RE_BIRTH_POS_REQ )
{
	int								m_StartPosIndex;
};

// KEGS_USER_UNIT_RE_BIRTH_POS_ACK

// KEGS_USER_UNIT_RE_BIRTH_REQ -> send id
// KEGS_USER_UNIT_RE_BIRTH_ACK -> KPacket

DECL_PACKET( EGS_USER_UNIT_RE_BIRTH_NOT )
{
	UidType                         m_UserUnitUID;
	int								m_StartPosIndex;
};


//////////////////////////////////////////////////////////////////////////
DECL_DATA( KPVPChannelInfo )
{
	enum PVP_CHANNEL_CLASS
	{
		PCC_NONE		= -1,
		PCC_RECRUIT		=  0,
		PCC_E1,
		PCC_E2,
		PCC_E3,
		PCC_E4,
		PCC_E5,
		PCC_E6,
		PCC_E7,
		PCC_E8,
		PCC_FREE,			/// ����ä��
		PCC_TOURNAMENT,		/// ��ȸ��..
		PCC_PLAY,			/// ���ä��..
		//{{ 2011. 07. 04	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		PCC_OFFICIAL,
#endif SERV_PVP_NEW_SYSTEM
		//}}

		PCC_END,
	};

	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	int				m_iMinRating;
	int				m_iMaxRating;
#else
	int				m_nMinVSPoint;
	int				m_nMaxVSPoint;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	UINT			m_iChannelID;
	char			m_cPVPChannelClass;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_OPEN_PVP_ROOM_LIST_ACK )
{
	int				m_iOK;
	UINT			m_iConnectChannelID;

	std::vector<KPVPChannelInfo>	m_vecChannel;
};

typedef UINT		KEGS_CHANGE_PVP_ROOM_LIST_REQ;
typedef KPacketOK	KEGS_CHANGE_PVP_ROOM_LIST_ACK;


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 12. 1  ������	������ �ʴ�
DECL_PACKET( EGS_INVITE_PVP_ROOM_REQ )
{
    UidType			m_iUnitUID;
};

DECL_PACKET( EGS_INVITE_PVP_ROOM_ACK )
{
    int				m_iOK;
	std::wstring	m_wstrNickName;
};

DECL_PACKET( EGS_INVITE_PVP_ROOM_NOT )
{
	std::wstring	m_wstrNickName;
	KRoomInfo		m_kRoomInfo;
};
//}}


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_USER_UNIT_DIE_REQ )
{
	int			m_KillerNPCUID;
	UidType		m_KillerUserUnitUID;
	UidType		m_MaxDamageKillerUserUnitUID;
	//{{ 2011. 09. 19  ��μ�	Īȣ ȹ�� ���� �߰�
#ifdef SERV_ADD_TITLE_CONDITION
	unsigned char		m_ucDieReason;

	enum USER_UNIT_DIE_REASON								// enum.lua ���� �߰� ������ �ʿ��Ѵ�
	{
		UUDR_UNKNOWN		= 0,
		UUDR_LACK_OF_OXYGEN,			// ��� ����

		UUDR_END,						// ������
	};
#endif SERV_ADD_TITLE_CONDITION
	//}}
};

DECL_PACKET( EGS_USER_UNIT_DIE_ACK )
{
	float		m_fReBirthTime;
	int			m_iOK;

	short		m_VP;							//���� VP
	//short		m_ED;							//���� ED
	short		m_EXP;
	
	KEGS_USER_UNIT_DIE_ACK()
	{
		m_fReBirthTime		= 0.f;
		m_iOK				= 0;
		m_VP				= 0;
		//m_ED				= 0;
		m_EXP				= 0;
	}	
};

DECL_PACKET( EGS_USER_UNIT_DIE_NOT )
{
	int			m_KillerNPCUID;
	UidType		m_KillerUserUnitUID;			//���� Unit
	UidType		m_MaxDamageKillerUserUnitUID;	//���� Unit
	UidType		m_KilledUserUnitUID;			//���� Unit

	KEGS_USER_UNIT_DIE_NOT()
	{
		m_KillerNPCUID					= -1;
		m_KillerUserUnitUID				= -1;
		m_MaxDamageKillerUserUnitUID	= -1;
		m_KilledUserUnitUID				= -1;
	}
};

typedef KPacketOK	KEGS_USER_UNIT_DIE_COMPLETE_ACK;

DECL_PACKET( EGS_USER_UNIT_DIE_COMPLETE_NOT )
{
	UidType		m_UnitUID;			//���� Unit

	KEGS_USER_UNIT_DIE_COMPLETE_NOT()
	{
		m_UnitUID					= -1;
	}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_NPC_UNIT_CREATE_REQ )
{
	std::vector< KNPCUnitReq >	m_vecNPCUnitReq;
};

// KEGS_NPC_UNIT_CREATE_ACK -> KPacketOK

DECL_DATA( KNPCUnitNot )
{
	KNPCUnitReq					m_kNPCUnitReq;
};

DECL_PACKET( EGS_NPC_UNIT_CREATE_NOT )
{
	std::vector< KNPCUnitNot >				m_vecNPCUnitAck;
	//{{ 2011. 11. 18	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	std::map< int, KAttribEnchantNpcInfo >	m_mapAttribNpcInfo;
	int										m_iBattleFieldID;

	KEGS_NPC_UNIT_CREATE_NOT()
	{
		m_iBattleFieldID = 0;
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
};

DECL_PACKET( EGS_NPC_UNIT_DIE_REQ )
{
	enum NPC_DIE_STATE
	{
		NDS_KILL_BY_USER = 0,
		NDS_KILL_SELF,
        NDS_ALLY,
		NDS_ESCAPE,
		NDS_BONUS_DROP,
	};

	int							m_nDieNPCUID;

	char						m_cAttUnitType;
	UidType						m_uiAttUnit;
	UidType						m_uiMDAttUnit;

	VECTOR3						m_DiePos;

	//{{ 2009. 5. 11  ������	npc die state
	char						m_cNpcDieState;
	//}}
	//{{ 2011. 11. 22	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	int							m_iNpcMaxHP;			// monster hp
	std::map< UidType, float >	m_mapDamageByUser;		// key : UnitUID,  value : damage
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

#ifdef SERV_DROP_FOR_FINISHER_ONLY
	UidType						m_iFinisherUID;
#endif SERV_DROP_FOR_FINISHER_ONLY

	KEGS_NPC_UNIT_DIE_REQ()
	{
		m_nDieNPCUID	= 0; // npcuid�� 1���� ����
		m_cAttUnitType	= -1;
		m_uiAttUnit		= -1;
		m_uiMDAttUnit	= -1;
		m_cNpcDieState	= NDS_KILL_BY_USER;
		//{{ 2011. 11. 22	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_iNpcMaxHP		= 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
#ifdef SERV_DROP_FOR_FINISHER_ONLY
		m_iFinisherUID	= 0;
#endif SERV_DROP_FOR_FINISHER_ONLY
	}

	KEGS_NPC_UNIT_DIE_REQ( const int nDieNPCUID_, const char cAttUnitType_, const UidType uiAttUnit_, 
		const UidType uiMDAttUnit_, const char cNpcDieState_, const int iNpcMaxHP_ ) 
		: m_nDieNPCUID( nDieNPCUID_ ), m_cAttUnitType( cAttUnitType_ ), m_uiAttUnit( uiAttUnit_ ),
		m_uiMDAttUnit( uiMDAttUnit_ ), /*m_DiePos*/ m_cNpcDieState( cNpcDieState_ ), 
		m_iNpcMaxHP( iNpcMaxHP_ )
	{}
};

DECL_PACKET( EGS_NPC_UNIT_DIE_ACK )
{
	int		m_iOK;

	KEGS_NPC_UNIT_DIE_ACK()
	{
		m_iOK	= 0;
	}
};

DECL_PACKET( EGS_NPC_UNIT_DIE_NOT )
{
	int							m_nDieNPCUID;
	int							m_iNPCID;

	char						m_cAttUnitType;
	UidType						m_uiAttUnit;

	std::map< UidType, KEXPData > m_EXPList;

	KEGS_NPC_UNIT_DIE_NOT()
	{
		m_nDieNPCUID	= 0;
		m_iNPCID		= 0;
		m_cAttUnitType	= 0;
		m_uiAttUnit		= 0;
	}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_DUNGEON_STAGE_LOAD_REQ )
{
	int		m_iStageID;
	//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	short	m_sPartyFever;
#else
	char	m_cPartyFever;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

#ifdef SERV_DUNGEON_STAGE_LOAD_LOG// �۾���¥: 2013-05-15	// �ڼ���
	char	m_cNormalStage;
	char	m_cSecretStage;
	char	m_cEliteStage;
	char	m_cClearConditionCheckCount;
#endif // SERV_DUNGEON_STAGE_LOAD_LOG

	KEGS_DUNGEON_STAGE_LOAD_REQ()
		: m_iStageID( 0 )
		//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		, m_sPartyFever( 0 )
#else
		, m_cPartyFever( 0 )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
#ifdef SERV_DUNGEON_STAGE_LOAD_LOG// �۾���¥: 2013-05-15	// �ڼ���
		, m_cNormalStage( 0 )
		, m_cSecretStage( 0 )
		, m_cEliteStage( 0 )
		, m_cClearConditionCheckCount( 0 )
#endif // SERV_DUNGEON_STAGE_LOAD_LOG
	{
	}
};

typedef KPacketOK		KEGS_DUNGEON_STAGE_LOAD_ACK;

DECL_PACKET( EGS_DUNGEON_STAGE_LOAD_NOT )
{
	int							m_iStageID;
	//{{ 2008. 7. 21  ������	�����̵�
	int							m_iNumMember;
	//}}
	std::map< int, KNPCList >	m_mapNPCData;
	//{{ 2008. 10. 1  ������	�Ӽ�����
	std::map< int, KAttribEnchantNpcInfo > m_mapAttribNpcInfo;
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
#ifdef SERV_TEST_LOG_FOR_SUB_STAGE_NPC_GROUP_ID// �۾���¥: 2013-09-03	// �ڼ���
	std::map< int, int >		m_mapSubStageNpcGroupID;
#endif // SERV_TEST_LOG_FOR_SUB_STAGE_NPC_GROUP_ID
};

//{{ 2009. 7. 21  ������	��� ��������
DECL_PACKET( EGS_DUNGEON_STAGE_LOAD_COMPLETE_REQ )
{
	bool						m_bIsSecretStage;
	//{{ 2011. 06. 13	������	�������� �α�
#ifdef SERV_DUNGEON_STAGE_LOG
	int							m_iDamage;
#endif SERV_DUNGEON_STAGE_LOG
	//}}

	KEGS_DUNGEON_STAGE_LOAD_COMPLETE_REQ()
	{
		m_bIsSecretStage = false;
		//{{ 2011. 06. 13	������	�������� �α�
#ifdef SERV_DUNGEON_STAGE_LOG
		m_iDamage = 0;
#endif SERV_DUNGEON_STAGE_LOG
		//}}
	}
};
//}}

// KEGS_DUNGEON_STAGE_LOAD_COMPLETE_ACK -> KPacketOK

DECL_PACKET( EGS_DUNGEON_STAGE_LOAD_COMPLETE_NOT )
{
	int		m_iSlotID;
#ifdef  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
    UidType m_iUnitUID;
#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

};

DECL_PACKET( EGS_DUNGEON_STAGE_START_NOT )
{
	int		m_iStageID;
};


/////////////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_DUNGEON_SUB_STAGE_OPEN_REQ )
{
	char		m_iSubStageID;
};

typedef KPacketOK	KEGS_DUNGEON_SUB_STAGE_OPEN_ACK;

DECL_PACKET( EGS_DUNGEON_SUB_STAGE_OPEN_NOT )
{
	char		m_iSubStageID;
};


/////////////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ )
{
	char		m_iBeforeStageID;
	char		m_iNextStageID;
};

typedef KPacketOK	KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK;

DECL_PACKET( EGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT )
{
	char		m_iBeforeStageID;
	char		m_iNextStageID;
};


/////////////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ )
{
	char		m_iSubStageID;
};

typedef KPacketOK	KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK;


/////////////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_DUNGEON_SUB_STAGE_START_NOT )
{
	char		m_iSubStageID;
};



DECL_DATA( KDungeonNextStageData )
{
	std::wstring	m_wstrStageID;
	int				m_nSubStageIndex;
};

// EGS_DUNGEON_LOADING_NEXT_STAGE_ACK				-> KPacketOK
// EGS_DUNGEON_LOADING_NEXT_STAGE_ALL_UNIT_OK_NOT	-> KPacketOK


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_END_GAME_REQ )
{
    int                             m_iWinTeam;
};

// KEGS_END_GAME_ACK -> KPacketOK

DECL_PACKET( EGS_END_GAME_NOT )
{
    char                            m_cRoomState;
	int                             m_iWinTeam;
    std::vector< KRoomSlotInfo >    m_vecSlot;

	KEGS_END_GAME_NOT()
	{
		m_cRoomState			= 0;
		m_iWinTeam				= 0;
	}
};


/////////////////////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_STATE_CHANGE_RESULT_REQ )
{
	int                             m_Dummy;
};

DECL_PACKET( EGS_STATE_CHANGE_RESULT_ACK )
{
	int								m_iOK;
};

DECL_PACKET( EGS_STATE_CHANGE_RESULT_NOT )
{
	char                            m_cRoomState;
	std::vector< KRoomSlotInfo >    m_vecSlot;
};


//////////////////////////////////////////////////////////////////////////
//EGS_START_REWARD_BOX_SELECT_NOT ID_SEND
DECL_PACKET( EGS_SELECT_REWARD_BOX_REQ )
{
	char			m_BoxNum;
};

DECL_PACKET( EGS_SELECT_REWARD_BOX_ACK )
{
	int				m_iOK;
};

DECL_PACKET( EGS_SELECT_REWARD_BOX_NOT )
{
	char			m_BoxNum;
	UidType			m_UnitUID;
};



typedef int         KEGS_CHANGE_DUNGEON_DIFFICULTY_REQ;
typedef KPacketOK   KEGS_CHANGE_DUNGEON_DIFFICULTY_ACK;
typedef int         KEGS_CHANGE_DUNGEON_DIFFICULTY_NOT;


////////////////////////////////////////////////////////////////////////////////////////
DECL_DATA( KDropItemData )
{
	int					m_iItemID;
	int					m_iDropItemUID;
	bool				m_bLeft;
	float				m_fSpeed;
	//{{ 2012. 02. 27	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	std::set< UidType >	m_setGetPossibleUnitUID;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2013. 1. 14	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
	char				m_cEnchantLevel;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
	//}}

	KDropItemData()
		: m_iItemID( 0 )
		, m_iDropItemUID( 0 )
		, m_bLeft( false )
		, m_fSpeed( 0.f )
		//{{ 2013. 1. 14	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
		, m_cEnchantLevel( 0 )
#endif SERV_ENCHANT_ITEM_DROP_EVENT
		//}}
	{
	}
};

DECL_PACKET( EGS_DROP_ITEM_NOT )
{
	std::vector< KDropItemData >		m_DropItemDataList;
	VECTOR3								m_CreatePos;
};


////////////////////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_GET_ITEM_REQ )
{
    int									m_iDropItemUID;
	KItemInfo                           m_kItemInfo;
};

typedef KPacketOK	KEGS_GET_ITEM_ACK;

//{{ 2009. 5. 11  ������	�ǽð�������
DECL_PACKET( EGS_GET_ITEM_REALTIME_NOT )
{
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
};

DECL_PACKET( EGS_GET_ITEM_TEMP_INVENTORY_NOT )
{
	KTempInventoryItemInfo				m_kTempInvenItem;
};

DECL_PACKET( EGS_GET_TEMP_ITEM_REQ )
{
    int									m_iTempItemUID;
};

DECL_PACKET( EGS_GET_TEMP_ITEM_ACK )
{
	int									m_iOK;
	int									m_iItemID;
	int									m_iTempItemUID;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};

typedef KEGS_GET_TEMP_ITEM_REQ		KEGS_DELETE_TEMP_ITEM_REQ;

DECL_PACKET( EGS_DELETE_TEMP_ITEM_ACK )
{
	int									m_iOK;
	int									m_iTempItemUID;
};
//}}

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_UPDATE_UNIT_INFO_NOT )
{
	KUnitInfo							m_kUnitInfo;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
	std::map< int, int >                m_mapItemObtained;
	bool								m_bGameEnd;

	KEGS_UPDATE_UNIT_INFO_NOT()
	{
		m_bGameEnd = false;
	}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_REFRESH_ROOM_ACK )
{
    int                                 m_iOK;
    KRoomInfo						    m_RoomInfo;
    std::vector<KRoomSlotInfo>	        m_vecSlot;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_NOTIFY_MSG_REQ )
{
	enum NOTIFY_TYPE
	{
		NT_GM_NOTIFY_ALL_SERVERS = 0,
		NT_GM_NOTIFY_SERVER_GROUP,
		NT_RANDOM_GET_ITEM,
		NT_DROP_GET_ITEM,
		//{{ 2009. 6. 29  ������	�����޼�����
		NT_MAX_LEVEL_UP,
		//}}
		//{{ 2009. 6. 29  ������	10�� ��ȭ ����
		NT_ENCHANT_SUCCESS,
		//}}
		//{{ 2009. 7. 27  ������	��ϸ� �ð�Ŭ���� ����
		NT_HENIR_CLEAR,
		//}}
		//{{ 2009. 11. 2  ������	���â�� ����
		NT_GUILD_CREATION,
		NT_GUILD_LEVEL_UP,
		//}}
		//{{ 2011. 07. 04    ��μ�    ���̹� ���θ��
//#ifdef SERV_NAVER_PROMOTIONS
		NT_RANDOM_GET_ITEM_ALL_NOTIFY,  // ������ ��� ���� ����
//#endif SERV_NAVER_PROMOTIONS
		//}} 
		//{{ 2011. 09. 29  ��μ�	�˴ٿ��� ����
//#ifdef SERV_SHUTDOWN_SYSTEM
		NT_SHUT_DOWN_ALL_NOTIFY,		// �˴ٿ��� �̸� �˸�
//#endif SERV_SHUTDOWN_SYSTEM
		//}}
		//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
		NT_GUILD_NAME_CHANGED,			// ��� �̸� ���� �˸�
#endif SERV_GUILD_CHANGE_NAME
		//}}
		//{{ 2012. 05. 3	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		NT_THE_GATE_OF_DARKNESS,	// ����� �� ���̵� ���� �˸�
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}
		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
		NT_WEDDING_SUCCESS,	// ��ȥ ���� �˸�
#endif SERV_RELATIONSHIP_SYSTEM
		//}
	};
	
	//{{ 2009. 6. 23  ������	����Ÿ��
	char								m_cNotifyType;
	//}}
	char								m_Count;
	std::wstring						m_wstrMSG;	
	
	//{{ ������ ȹ�� ������ ť�� �̸� ��� - ��μ�
#ifdef SERV_GET_ITEM_NOTIFY
	int									m_iRandomItem;
#endif SERV_GET_ITEM_NOTIFY
	//}}

	KEGS_NOTIFY_MSG_REQ()
	{
		m_cNotifyType = NT_GM_NOTIFY_ALL_SERVERS;
		m_Count = 1;

		//{{ ������ ȹ�� ������ ť�� �̸� ��� - ��μ�
#ifdef SERV_GET_ITEM_NOTIFY
		m_iRandomItem = 0;
#endif SERV_GET_ITEM_NOTIFY
		//}}
	}
};

typedef KEGS_NOTIFY_MSG_REQ		KEGS_NOTIFY_MSG_NOT;


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_ADMIN_SERVER_STATUS_ACK )
{
    int									m_iOK;
    int							        m_iCCU;
    std::map< int, int>				    m_mapNumPvPRoom;
	//{{ 2009. 4. 1  ������		��Ƽ����
	int									m_iCurPartyCount;
	int									m_iPlayGamePartyCount;
	//}}
#ifdef SERV_CHANNELING_USER_MANAGER
	std::map<int,int>					m_mapChannelingUserList;
#endif SERV_CHANNELING_USER_MANAGER
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CLIENT_QUIT_REQ )
{
    int								    m_iReason;
	std::vector< KErrorMsgInfo >		m_ErrorMsgInfoList;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_ADMIN_MODIFY_UNIT_LEVEL_REQ )
{
	std::wstring                        m_wstrUnitNickName;
    int                                 m_iLevel;
	int									m_iEXP;

	KEGS_ADMIN_MODIFY_UNIT_LEVEL_REQ()
	{
		m_iLevel = 1;
		m_iEXP = 0;
	}
};

DECL_PACKET( EGS_ADMIN_MODIFY_UNIT_LEVEL_ACK )
{
	int                                 m_iOK;
	KUnitInfo                           m_kUnitInfo;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CHANGE_OPTION_PLAY_GUIDE_REQ )
{
    bool                                m_bOn;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_BUY_ED_ITEM_REQ )
{
	bool                                m_bDebug;
	//{{ 2008. 4. 24  ������  ������ ġƮ �Ⱓ��
	short								m_sPeriod;
	//}}
    std::map< int, int >                m_mapItem;
};

DECL_PACKET( EGS_BUY_ED_ITEM_ACK )
{
    int                                 m_iOK;
    int                                 m_iED;
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	int                                 m_iAPoint;
#else
	int                                 m_iVSPoint;
#endif SERV_PVP_NEW_SYSTEM
	//}}
    std::vector< KInventoryItemInfo >   m_vecInventorySlotInfo;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_SELL_ED_ITEM_REQ )
{
    UidType                             m_iItemUID;	
    int									m_iQuantity;
};

DECL_PACKET( EGS_SELL_ED_ITEM_ACK )
{
    int                                 m_iOK;
    int                                 m_iED;
    KInventoryItemInfo                  m_kInventorySlotInfo;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_NEW_QUEST_REQ )
{
	int									m_iQuestID;
	int									m_iTalkNPCID;
};

typedef KPacketOK		KEGS_NEW_QUEST_ACK;


DECL_PACKET( EGS_NEW_QUEST_NOT )
{
	KQuestInstance						m_QuestInst;
	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
#endif SERV_QUEST_CLEAR_EXPAND
	//}}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_UPDATE_QUEST_REQ )
{
	KQuestInstance						m_QuestInst;
};

DECL_PACKET( EGS_UPDATE_QUEST_ACK )
{
	int									m_iOK;
};

DECL_PACKET( EGS_UPDATE_QUEST_NOT )
{
	std::vector< KQuestInstance >		m_vecQuestInst;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_QUEST_COMPLETE_REQ )
{
	int									m_iQuestID;
	int									m_iTalkNPCID;
	std::map< int, int >				m_mapSelectItemID;
	//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	bool								m_bAutoComplete;		// �ټ��� ����Ʈ�� ���ÿ� �Ϸ�ó���Ҷ� ���, �Ϲ������δ� false�� �д�.
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}

	KEGS_QUEST_COMPLETE_REQ()
	{
		m_iQuestID		= 0;
		m_iTalkNPCID	= 0;
		//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		m_bAutoComplete = false;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
	}
};

DECL_PACKET( EGS_QUEST_COMPLETE_ACK )
{
	int									m_iOK;	
	//{{ 2010. 04. 05  ������	��������Ʈ
#ifdef SERV_DAILY_QUEST
	KCompleteQuestInfo					m_kCompleteQuestInfo;
#else
	int									m_iQuestID;
#endif SERV_DAILY_QUEST
	//}}

	KEGS_UPDATE_UNIT_INFO_NOT			m_kUpdateUnitInfo;
};


//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
DECL_PACKET( EGS_ALL_COMPLETED_QUEST_COMPLETE_REQ )
{
	std::vector< KEGS_QUEST_COMPLETE_REQ >	m_vecQuestCompleteReq;
};

DECL_PACKET( EGS_ALL_COMPLETED_QUEST_COMPLETE_ACK )
{
	int										m_iOK;
	std::vector< KEGS_QUEST_COMPLETE_ACK >	m_vecQuestCompleteAck;

	KEGS_ALL_COMPLETED_QUEST_COMPLETE_ACK()
	{
		m_iOK = 0;
	}
};
#endif SERV_ENTER_FIELD_QUEST_CLEAR
//}}


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_GIVE_UP_QUEST_REQ )
{
	int									m_iQuestID;
};

#ifdef SERV_GATHER_GIVE_UP_QUEST
DECL_PACKET( EGS_GATHER_GIVE_UP_QUEST_REQ )
{
	std::vector< int >					m_vecGatherQuestID;
};
#endif // SERV_GATHER_GIVE_UP_QUEST

typedef KQuestAck	KEGS_GIVE_UP_QUEST_ACK;


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_TALK_WITH_NPC_REQ )
{
	int									m_iNPCID;
};

typedef KQuestAck	KEGS_TALK_WITH_NPC_ACK;


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CHANGE_INVENTORY_SLOT_ITEM_REQ )
{
	char								m_cFromSlotType;
	int									m_iFromSlotID;

	char								m_cToSlotType;
	int									m_iToSlotID;

#ifdef SERV_SHARING_BANK_TEST
	UidType								m_iShareUnitUID;
#endif //SERV_SHARING_BANK_TEST
};

DECL_PACKET( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK )
{
    std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};

DECL_PACKET( EGS_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT )
{
    UidType                             m_UnitUID;
    std::vector< KInventoryItemInfo >   m_vecInventorySlotInfo;
	//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
	int									m_iEquippedTitleID;
#else
	short								m_sEquippedTitleID;
#endif SERV_TITLE_DATA_SIZE
	//}}
};

DECL_PACKET( EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT )
{
	//{{ 2010. 12. 23	������	��� ���� ��Ŷ Ÿ��
#ifdef SERV_CHANGE_EQUIPPED_PACKET_REFAC
	enum CHANGE_EQUIPPED_PACKET_TYPE
	{
		CEPT_NONE = 0,
		CEPT_CHANGE_EQUIP,
		CEPT_CHANGE_TITLE,
		CEPT_CHANGE_PET,
	};
	
	char								m_cChangeEquippedPacketType;
#endif SERV_CHANGE_EQUIPPED_PACKET_REFAC
	//}}
    UidType                             m_UnitUID;
    std::vector< KInventoryItemInfo >   m_vecInventorySlotInfo;
    KStat                               m_kGameStat;
	//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
	int									m_iEquippedTitleID;
#else
	short								m_sEquippedTitleID;
#endif SERV_TITLE_DATA_SIZE
	//}}

	KEGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT()
	{
		//{{ 2010. 12. 23	������	��� ���� ��Ŷ Ÿ��
#ifdef SERV_CHANGE_EQUIPPED_PACKET_REFAC
		m_cChangeEquippedPacketType = CEPT_NONE;
#endif SERV_CHANGE_EQUIPPED_PACKET_REFAC
		//}}
		m_UnitUID = 0;
	}
};

//{{ 2008. 11. 30  ������	�ʵ忡�� ��� ��ü
DECL_PACKET( EGS_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT )
{
	UidType								m_iUnitUID;
	std::vector< KInventoryItemInfo >   m_vecInventorySlotInfo;
	//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
	int									m_iEquippedTitleID;
#else
	short								m_sEquippedTitleID;
#endif SERV_TITLE_DATA_SIZE
	//}}
};
//}}


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_DELETE_ITEM_REQ )
{
    UidType						        m_iItemUID;	
	int									m_iQuantity;
};

DECL_PACKET( EGS_DELETE_ITEM_ACK )
{
	int									m_iOK;
	KInventoryItemInfo					m_kInventoryItemInfo;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_ADMIN_CHANGE_AUTH_LEVEL_REQ )
{
	std::wstring						m_wstrUserID;
	char								m_cAuthLevel;
};

typedef KPacketOK	KEGS_ADMIN_CHANGE_AUTH_LEVEL_ACK;


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_ADMIN_GET_AUTH_LEVEL_LIST_REQ )
{
	char								m_cAuthLevel;
};

DECL_PACKET( EGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK )
{
	int									m_iOK;
	std::vector< std::wstring >			m_vecUserIDList;
};

DECL_PACKET( EGS_CHECK_INVALID_ROOM_NOT )
{
	int									m_iOK;
	int									m_iMapID;
	
	KEGS_CHECK_INVALID_ROOM_NOT()
	{
		m_iOK = 0;
		m_iMapID = 0;
	}
};


//////////////////////////////////////////////////////////////////////////

//{{ 2012. 10. 16	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
DECL_PACKET( EGS_RESULT_SUCCESS_REQ )
{
	KGamePlayNetworkInfoPackage		m_kGamePlayNetworkInfo;
};
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
//}}

//{{ 2009. 4. 28  ������	����id
DECL_PACKET( EGS_RESULT_SUCCESS_ACK )
{
	int									m_iOK;
	int									m_iDungeonID;
};
//}}

DECL_PACKET( EGS_RESULT_ALL_UNIT_SUCCESS_NOT )
{
	KRoomInfo						    m_RoomInfo;
	std::vector< KRoomSlotInfo >		m_vecSlot;
	//{{ 2012. 06. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KReturnToFieldInfo					m_kReturnToFieldInfo;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_USE_QUICK_SLOT_REQ )
{
	//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
	short								m_sSlotID;
#else
	char								m_cSlotID;
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
	//}}
};

DECL_PACKET( EGS_USE_QUICK_SLOT_ACK )
{
	int									m_iOK;
	KInventoryItemInfo					m_InventorySlotInfo;
	int									m_ItemID;
};

DECL_PACKET( EGS_USE_QUICK_SLOT_NOT )
{
	UidType								m_UnitUID;
	int									m_ItemID;
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
	bool								m_bUseItemNeedPayment;
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_ADMIN_CHANGE_ED_REQ )
{
	std::wstring						m_wstrUnitNickName;
	int									m_nED;
};

typedef KPacketOK		KEGS_ADMIN_CHANGE_ED_ACK;


//////////////////////////////////////////////////////////////////////////
typedef std::wstring	KEGS_SEARCH_UNIT_REQ;

DECL_PACKET( EGS_SEARCH_UNIT_ACK )
{
	int									m_iOK;
	KConnectionUnitInfo					m_kCUnitInfo;
};

//{{ 2009. 4. 17  ������	�������� �������� ���
typedef KEGS_SEARCH_UNIT_REQ		KEGS_GET_CONNECTION_UNIT_INFO_REQ;
typedef KEGS_SEARCH_UNIT_ACK		KEGS_GET_CONNECTION_UNIT_INFO_ACK;
//}}

//{{ 2009. 6. 23  ������	������
DECL_PACKET( EGS_WATCH_UNIT_REQ )
{
    UidType								m_iUnitUID;
};

typedef KEGS_SEARCH_UNIT_ACK		KEGS_WATCH_UNIT_ACK;
//}}


//////////////////////////////////////////////////////////////////////////
typedef KPacketOK	KEGS_LEAVE_SQUARE_ACK;

DECL_PACKET( EGS_SQUARE_LIST_REQ )
{
	UINT								m_nViewPage;

	char								m_cSquareType;
};

DECL_PACKET( EGS_SQUARE_LIST_ACK )
{
	int									m_iOK;

	UINT								m_nTotalPage;
	UINT								m_nViewPage;
	std::vector< KSquareInfo >			m_vecSquareInfo;
};

DECL_PACKET( EGS_SQUARE_UNIT_SYNC_DATA_REQ )
{
	char								m_StateID;
	char								m_StateChangeNum;
	VECTOR3								m_vPos;
	unsigned char						m_LastTouchLineIndex;
	bool								m_bIsRight;

	KEGS_SQUARE_UNIT_SYNC_DATA_REQ()
	{
		m_StateID				= 0;
		m_StateChangeNum		= 0;
		m_LastTouchLineIndex	= 0;
		m_bIsRight				= true;
	}
};

DECL_PACKET( EGS_SQUARE_UNIT_SYNC_DATA_NOT )
{
	UidType								m_UnitUID;
	char								m_StateID;
	char								m_StateChangeNum;
	VECTOR3								m_vPos;
	unsigned char						m_LastTouchLineIndex;
	bool								m_bIsRight;
	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
	int									m_iED;
#endif SERV_ED_MONITORING_IN_GAME
	//}}

	KEGS_SQUARE_UNIT_SYNC_DATA_NOT()
	{
		m_UnitUID				= 0;
		m_StateID				= 0;
		m_StateChangeNum		= 0;
		m_LastTouchLineIndex	= 0;
		m_bIsRight				= true;
		//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
		m_iED					= 0;
#endif SERV_ED_MONITORING_IN_GAME
		//}}
	}
};

/////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_JOIN_SQUARE_REQ )
{
	UidType								m_iSquareUID;
};

DECL_PACKET( EGS_JOIN_SQUARE_ACK )
{
	int                                 m_iOK;
	KSquareInfo						    m_kSquareInfo;
	std::vector< KSquareUserInfo >		m_vecUserInfo;
};

DECL_PACKET( EGS_JOIN_SQUARE_NOT )
{
	KSquareUserInfo						m_kUserInfo;
};


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
DECL_PACKET( EGS_GET_SKILL_REQ )
{
#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
	std::vector<int>						m_vecNowLearnSkill;	// ���� ��� ��ų ��
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL
#ifdef SERV_SKILL_PAGE_SYSTEM
	int										m_iActiveSkillPageNumber;	/// Ȱ��ȭ ���� ������
#endif // SERV_SKILL_PAGE_SYSTEM

	std::map< int, KGetSkillInfo >			m_mapSkillList;	// ���� ���ų� ��ȭ �ϴ� ��ų
	
	KEGS_GET_SKILL_REQ()
#ifdef SERV_SKILL_PAGE_SYSTEM
		: m_iActiveSkillPageNumber(0)	
#endif // SERV_SKILL_PAGE_SYSTEM

	{}
};

DECL_PACKET( EGS_GET_SKILL_ACK )
{
	int										m_iOK;
#ifdef SERV_SKILL_PAGE_SYSTEM
	int										m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	std::map< int, KGetSkillInfo >			m_mapSkillList;			// ����� skill list
	int										m_iRemainSP;			// ���� sp
	int										m_iRemainCSP;			// ���� Cash sp

	KEGS_GET_SKILL_ACK()
	{
		m_iOK = 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iActiveSkillPageNumber = 0;
		m_mapSkillList.clear();
#endif // SERV_SKILL_PAGE_SYSTEM
		m_iRemainSP = 0;
		m_iRemainCSP = 0;
	}
};
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
DECL_PACKET( EGS_GET_SKILL_REQ )
{
	int									m_iSkillID;
};

DECL_PACKET( EGS_GET_SKILL_ACK )
{
	int									m_iOK;
	int									m_iSkillID;
	int									m_iSkillLevel;
	int									m_iSkillCSPoint;
	int									m_iSPoint;
	int									m_iCSPoint;
};
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CHANGE_SKILL_SLOT_REQ )
{
	int									m_iSlotID;
	int									m_iSkillID;

	KEGS_CHANGE_SKILL_SLOT_REQ()
		: m_iSlotID( -1 )
		, m_iSkillID( 0 )
	{
	}
};

DECL_PACKET( EGS_CHANGE_SKILL_SLOT_ACK )
{
	int									m_iOK;
	int									m_iSlotID;
	int									m_iSkillID;
	//{{ 2012. 12. 3	�ڼ���	��ų ���� ü���� ��Ŷ ����
#ifdef SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
	int									m_iSlotID2;
	int									m_iSkillID2;
#endif SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
	//}}

	KEGS_CHANGE_SKILL_SLOT_ACK()
		: m_iOK( 0 )
		, m_iSlotID( -1 )
		, m_iSkillID( 0 )
		//{{ 2012. 12. 3	�ڼ���	��ų ���� ü���� ��Ŷ ����
#ifdef SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
		, m_iSlotID2( -1 )
		, m_iSkillID2( 0 )
#endif SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
		//}}
	{
	}
};

DECL_PACKET( EGS_CHANGE_SKILL_SLOT_NOT )
{
	int									m_iSlotID;
	int									m_iSkillID;
	//{{ 2012. 12. 3	�ڼ���	��ų ���� ü���� ��Ŷ ����
#ifdef SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
	int									m_iSlotID2;
	int									m_iSkillID2;
#endif SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
	//}}

	UidType								m_iUnitUID;

	KEGS_CHANGE_SKILL_SLOT_NOT()
		:  m_iSlotID( -1 )
		, m_iSkillID( 0 )
		//{{ 2012. 12. 3	�ڼ���	��ų ���� ü���� ��Ŷ ����
#ifdef SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
		, m_iSlotID2( -1 )
		, m_iSkillID2( 0 )
#endif SERV_SKILL_SLOT_CHANGE_PACKET_INTEGRATE
		//}}
		, m_iUnitUID( 0 )
	{
	}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_ADMIN_CHANGE_SKILL_POINT_REQ )
{
	int									m_iSPoint;
#ifdef SERV_SKILL_PAGE_SYSTEM
	int									m_iActiveSkillPageNumber;

	KEGS_ADMIN_CHANGE_SKILL_POINT_REQ()
		: m_iSPoint(0), m_iActiveSkillPageNumber(0)
	{}
#endif // SERV_SKILL_PAGE_SYSTEM
};

DECL_PACKET( EGS_ADMIN_CHANGE_SKILL_POINT_ACK )
{
	int								    m_iOK;
	int								    m_iSPoint;
#ifdef SERV_SKILL_PAGE_SYSTEM
	int									m_iActiveSkillPageNumber;

	KEGS_ADMIN_CHANGE_SKILL_POINT_ACK()
		: m_iOK(0), m_iSPoint(0), m_iActiveSkillPageNumber(0)
	{}
#endif // SERV_SKILL_PAGE_SYSTEM
};

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27

#ifdef SERV_SKILL_PAGE_SYSTEM
DECL_PACKET( EGS_ADMIN_INIT_SKILL_TREE_REQ )
{
	int									m_iActiveSkillPageNumber;

	KEGS_ADMIN_INIT_SKILL_TREE_REQ()
		: m_iActiveSkillPageNumber(0)
	{}
};
#endif // SERV_SKILL_PAGE_SYSTEM

DECL_PACKET( EGS_ADMIN_INIT_SKILL_TREE_ACK )
{
	int								    m_iOK;
#ifdef SERV_SKILL_PAGE_SYSTEM
	int									m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM
	int								    m_iSPoint;
	int								    m_iCSPoint;

	KEGS_ADMIN_INIT_SKILL_TREE_ACK()
	{
		m_iOK = 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iActiveSkillPageNumber = 0;
#endif // SERV_SKILL_PAGE_SYSTEM
		m_iSPoint = 0;
		m_iCSPoint = 0;
	}
};
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
typedef KPacketOK	KEGS_ADMIN_INIT_SKILL_TREE_ACK;
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_ADMIN_KICK_USER_REQ )
{
    std::wstring						m_wstrUnitNickName;
	bool								m_bIsUserID;
};

typedef KPacketOK   KEGS_ADMIN_KICK_USER_ACK;


//{{ 2011. 12. 22	������	��Ʋ�ʵ� �ý���
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_INVITE_PARTY_REQ )
{
	UidType							m_iReceiverUID;
	std::wstring					m_wstrNickName;
	int								m_iServerGroupID;

	// ��Ƽ ������ �ʿ��� ����
	KCreatePartyInfo				m_kCreatePartyInfo;

	KEGS_INVITE_PARTY_REQ()
	{
		m_iReceiverUID = 0;
		m_iServerGroupID = -1;
	}
};

DECL_PACKET( EGS_INVITE_PARTY_ACK )
{
	int								m_iOK;
	UidType							m_iReceiverUID;
	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	std::wstring					m_wstrUDPRelayIP;
	USHORT							m_usUDPRelayPort;
#endif SERV_CHOOSE_FASTEST_HOST
	//}
};

DECL_PACKET( EGS_INVITE_PARTY_NOT )
{
	UidType							m_iPartyUID;
	UidType							m_iHostUnitUID;
	std::wstring					m_wstrHostNickName;
	//{{ 2011. 07. 08	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	char							m_cPartyType;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	std::wstring					m_wstrUDPRelayIP;
	USHORT							m_usUDPRelayPort;
#endif SERV_CHOOSE_FASTEST_HOST
	//}

	KEGS_INVITE_PARTY_NOT()
	{
		m_iPartyUID			= 0;
		m_iHostUnitUID		= 0;
		//{{ 2011. 07. 08	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		m_cPartyType		= 0;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
		m_wstrUDPRelayIP = L"";
		m_usUDPRelayPort = 0;
#endif SERV_CHOOSE_FASTEST_HOST
		//}
	}
};

DECL_PACKET( EGS_INVITE_PARTY_REPLY_NOT )			// ��Ƽ �ʴ� ���� ����� ������ ������ ���� ��Ŷ
{
	enum REPLY_REASON
	{
		RR_ACCEPT = 0,
		RR_NOT_ACCEPT,
		RR_BUSY,
	};

	UidType							m_iPartyUID;
	UidType							m_iHostUnitUID;
	char							m_cReplyReason;

	KEGS_INVITE_PARTY_REPLY_NOT()
	{
		m_iPartyUID = 0;
		m_iHostUnitUID = 0;
		m_cReplyReason = RR_ACCEPT;
	}
};

DECL_PACKET( EGS_INVITE_PARTY_DENY_NOT )			// ��Ƽ �ʴ��� ����� timeout �Ŀ� �޴� packet
{
	int								m_iDenyReasonNetError;
	UidType							m_iUnitUID;
};

DECL_PACKET( EGS_JOIN_PARTY_NOT )
{
	int								m_iOK;
	std::vector< KPartyInfo >		m_vecPartyInfo;	// ���������ϴ� �������Ը� �����ֱ� ����
	std::vector< KPartyUserInfo >	m_vecPartyUserInfo;
	//{{ 2012. 02. 22	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	std::vector< UidType >			m_vecCurPartyUnitUIDList;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	std::wstring					m_wstrUDPRelayIP;
	USHORT							m_usUDPRelayPort;
#endif SERV_CHOOSE_FASTEST_HOST
	//}
};

DECL_PACKET( EGS_CLOSE_PARTY_NOT )
{
	UidType							m_iPartyUID;
    int								m_iReason;

	KEGS_CLOSE_PARTY_NOT()
	{
		m_iPartyUID = 0;
		m_iReason = 0;
	}
};

DECL_PACKET( EGS_JOIN_PARTY_INVITE_DENY_NOT )		// ��Ƽ���� �ƴ� ����� ��Ƽ ��û�ߴٰ� timeout �Ǿ��� ��쿡 ��Ƽ���� �ް� �Ǵ� ��Ŷ
{
	int								m_iOK;
	UidType							m_iUnitUID;
};


DECL_PACKET( EGS_UPDATE_PARTY_USER_INFO_NOT )
{
	UidType							m_iUnitUID;
	UCHAR							m_cState;
	int								m_iStateCode;
	char						    m_cUnitClass;
	UCHAR							m_ucLevel;
};

//{{ 2009. 3. 31  ������	��Ƽ����
DECL_PACKET( EGS_UPDATE_PARTY_UID_NOT )
{
	UidType							m_iUnitUID;	
	UidType							m_iPartyUID;
};
//}}


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_LEAVE_PARTY_REQ )
{
	int								m_iReason;

	KEGS_LEAVE_PARTY_REQ()
	{
		m_iReason = 0;
	}
};

typedef KPacketOK	KEGS_LEAVE_PARTY_ACK;

DECL_PACKET( EGS_LEAVE_PARTY_NOT )
{
	UidType							m_iLeaveUnitUID;
	UidType							m_iNewHostUID;
	//{{ 2012. 02. 21	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	UidType							m_iPartyUID;
	std::vector< UidType >			m_vecPartyUnitUIDList;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	bool							m_bExistComeBackUser;
#endif SERV_SERVER_BUFF_SYSTEM
	//}

#ifdef LOG_PARTY_BREAK
	int								m_iReason;
#endif // LOG_PARTY_BREAK

	KEGS_LEAVE_PARTY_NOT()
	{
		m_iLeaveUnitUID = 0;
		m_iNewHostUID = 0;
		//{{ 2012. 02. 21	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_iPartyUID = 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
		m_bExistComeBackUser = false;
#endif SERV_SERVER_BUFF_SYSTEM
		//}
#ifdef LOG_PARTY_BREAK
		m_iReason = -1;
#endif // LOG_PARTY_BREAK
	}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_PARTY_CHANGE_READY_REQ )
{
	bool							m_bReady;
};

typedef KPacketOK	KEGS_PARTY_CHANGE_READY_ACK;

DECL_PACKET( EGS_PARTY_CHANGE_READY_NOT )
{
	UidType							m_UnitUID;
	bool							m_bReady;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_PARTY_CHANGE_DUNGEON_REQ )
{
	int								m_iDungeonID;
	char							m_DifficultyLevel;
	char							m_cDungeonMode;
};

typedef KPacketOK	KEGS_PARTY_CHANGE_DUNGEON_ACK;

typedef KEGS_PARTY_CHANGE_DUNGEON_REQ	KEGS_PARTY_CHANGE_DUNGEON_NOT;


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_PARTY_GAME_START_REQ )
{
	short							m_sWorldID;
};

typedef KPacketOK	KEGS_PARTY_GAME_START_ACK;

DECL_PACKET( EGS_PARTY_GAME_START_NOT )
{
	short								m_sWorldID;
	KRoomInfo						    m_RoomInfo;
	std::vector< KRoomSlotInfo >        m_vecSlot;
	std::vector< KRoomSlotInfo >		m_vecObserverSlot; // ���������ε� �������� �ʿ��ֳ�?

	std::wstring						m_wstrCNIP;

	//{{ 2009. 2. 4  ������		���� �����
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
	//}}
};

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT )
{
	bool								m_bAccept;	// ��������
	int									m_iReason;	// �������ο� ���� �ڼ��� ������ �ʿ��Ҷ� ���, ����� ������ ���� ����

	KEGS_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT()
	{
		m_bAccept = false;
		m_iReason = 0;
	}
};

DECL_PACKET( EGS_PARTY_GAME_START_FAILED_NOT )
{
	int									m_iReason;
	UidType								m_iFailUnitUID;

	KEGS_PARTY_GAME_START_FAILED_NOT()
	{
		m_iReason = 0;
		m_iFailUnitUID = 0;
	}
};

//////////////////////////////////////////////////////////////////////////
typedef UidType			KEGS_PARTY_CHANGE_HOST_REQ;
typedef KPacketOK		KEGS_PARTY_CHANGE_HOST_ACK;

DECL_PACKET( EGS_PARTY_CHANGE_HOST_NOT )
{
	UidType								m_iBeforeUID;
	UidType								m_iAfterUID;
	//{{ 2012. 11. 13	������	��Ʋ�ʵ� �ý���
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	bool								m_bRegroupParty;
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}

	KEGS_PARTY_CHANGE_HOST_NOT()
	{
		m_iBeforeUID = 0;
		m_iAfterUID = 0;
		//{{ 2012. 11. 13	������	��Ʋ�ʵ� �ý���
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		m_bRegroupParty = false;
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
	}
};


//////////////////////////////////////////////////////////////////////////
typedef UidType			KEGS_PARTY_BAN_USER_REQ;
typedef KPacketOK		KEGS_PARTY_BAN_USER_ACK;


//////////////////////////////////////////////////////////////////////////
typedef char			KEGS_PARTY_CHANGE_GET_ITEM_TYPE_REQ;
typedef KPacketOK		KEGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK;
typedef char			KEGS_PARTY_CHANGE_GET_ITEM_TYPE_NOT;


//////////////////////////////////////////////////////////////////////////
typedef char			KEGS_PARTY_CHANGE_NUM_OF_PER_REQ;
typedef KPacketOK		KEGS_PARTY_CHANGE_NUM_OF_PER_ACK;
typedef char			KEGS_PARTY_CHANGE_NUM_OF_PER_NOT;


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 4. 1  ������		��Ƽ����
typedef bool			KEGS_PARTY_LIMIT_LEVEL_REQ;
typedef KPacketOK		KEGS_PARTY_LIMIT_LEVEL_ACK;
typedef bool			KEGS_PARTY_LIMIT_LEVEL_NOT;
//}}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//{{ 2008. 11. 26  ������	��Ƽ ����Ʈ
//DECL_PACKET( EGS_PARTY_LIST_REQ )
//{
//	enum PARTY_LIST_TYPE
//	{
//		PLT_DUNGEON = 0,
//		PLT_GATE_TOTAL,
//		//{{ 2010. 03. 26  ������	��д��� ����
//		PLT_SECRET_DUNGEON,
//		//}}
//
//		PLT_END,
//	};
//
//	char							m_cListType;
//	UINT							m_nViewPage;
//	int								m_iID;	// Ÿ�ӿ� ���� ���� ���̵� & ���� ���̵�
//
//	KEGS_PARTY_LIST_REQ()
//	{
//		m_cListType		= PLT_DUNGEON;
//		m_nViewPage		= 1;
//		m_iID			= 0;
//	}
//};
//
//DECL_PACKET( EGS_PARTY_LIST_ACK )
//{
//	int								m_iOK;
//	UINT							m_nTotalPage;
//	UINT							m_nViewPage;
//
//	std::vector< KPartyListInfo >	m_vecPartyInfo;
//};
////}}
//
//
////////////////////////////////////////////////////////////////////////////
//typedef KPartyInfo	KEGS_CREATE_PARTY_REQ;
//
//DECL_PACKET( EGS_CREATE_PARTY_ACK )
//{
//	int								m_iOK;
//	KPartyInfo						m_kPartyInfo;
//	KPartyUserInfo					m_kPartyUserInfo;
//};
//
//
////////////////////////////////////////////////////////////////////////////
//DECL_PACKET( EGS_JOIN_PARTY_REQ )
//{
//	UidType							m_iPartyUID;
//};
//
//typedef KPacketOK	KEGS_JOIN_PARTY_ACK;
//
//DECL_PACKET( EGS_JOIN_PARTY_INVITE_NOT )
//{
//	UidType							m_iUnitUID;
//	std::wstring					m_wstrNickName;
//	char							m_cUnitClass;
//	UCHAR							m_ucLevel;
//	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
//#ifdef SERV_COME_BACK_USER_REWARD
//	bool							m_bComeBack;
//	std::wstring					m_wsrtComeBackEnd;
//#endif SERV_COME_BACK_USER_REWARD
//	//}}
//
//	CON_COPYCON_ASSIGNOP( KEGS_JOIN_PARTY_INVITE_NOT, right )
//	{
//		m_iUnitUID		= right.m_iUnitUID;
//		m_wstrNickName	= right.m_wstrNickName;
//		m_cUnitClass	= right.m_cUnitClass;
//		m_ucLevel		= right.m_ucLevel;
//		//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
//#ifdef SERV_COME_BACK_USER_REWARD
//		m_bComeBack		= right.m_bComeBack;
//		m_wsrtComeBackEnd = right.m_wsrtComeBackEnd;
//#endif SERV_COME_BACK_USER_REWARD
//		//}}
//	}
//};
//
//DECL_PACKET( EGS_JOIN_PARTY_INVITE_REPLY_NOT )
//{
//	//{{ 2010. 10. 04	������	��Ƽ �ʴ� ���� ����
//#ifdef SERV_PARTY_INVITE_REPLY_FIX
//	enum REPLY_REASON
//	{
//		RR_ACCEPT = 0,
//		RR_NOT_ACCEPT,
//		RR_BUSY,
//	};
//#endif SERV_PARTY_INVITE_REPLY_FIX
//	//}}
//
//	UidType							m_iUnitUID;
//	//{{ 2011. 02. 18	������	��Ƽ �ʴ� ����
//#ifdef SERV_PARTY_INVITE_REPLY_FIX
//	char							m_cReplyReason;
//#else
//	bool							m_bAccept;
//#endif SERV_PARTY_INVITE_REPLY_FIX
//	//}}
//};
//
//DECL_PACKET( EGS_JOIN_PARTY_NOT )
//{
//	int								m_iOK;
//	std::vector< KPartyInfo >		m_vecPartyInfo;	// ���������ϴ� �������Ը� �����ֱ� ����
//	std::vector< KPartyUserInfo >	m_vecPartyUserInfo;
//};
//
////typedef KPacketOK	KEGS_JOIN_PARTY_INVITE_DENY_NOT;
//DECL_PACKET( EGS_JOIN_PARTY_INVITE_DENY_NOT )		// ��Ƽ���� �ƴ� ����� ��Ƽ ��û�ߴٰ� timeout �Ǿ��� ��쿡 ��Ƽ���� �ް� �Ǵ� ��Ŷ
//{
//	int								m_iOK;
//	UidType							m_iUnitUID;
//};
//
//
////////////////////////////////////////////////////////////////////////////
//DECL_PACKET( EGS_LEAVE_PARTY_REQ )
//{
//	int								m_iReason;
//};
//
//typedef KPacketOK	KEGS_LEAVE_PARTY_ACK;
//
//DECL_PACKET( EGS_LEAVE_PARTY_NOT )
//{
//	UidType							m_iLeaveUnitUID;
//	UidType							m_iNewHostUID;
//};
//
//
////////////////////////////////////////////////////////////////////////////
//DECL_PACKET( EGS_INVITE_PARTY_REQ )
//{
//	UidType							m_iReceiverUID;
//	//{{ 2011. 12. 05	������	��Ʋ�ʵ�
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//	std::wstring					m_wstrNickName;
//#endif SERV_BATTLE_FIELD_SYSTEM
//	//}}
//};
//
//DECL_PACKET( EGS_INVITE_PARTY_ACK )
//{
//	int								m_iOK;
//	UidType							m_iReceiverUID;
//};
//
//DECL_PACKET( EGS_INVITE_PARTY_NOT )
//{
//	UidType							m_iPartyUID;
//	std::wstring					m_wstrNickName;
//	int								m_iDungeonID;
//	char							m_cDifficultyLevel;
//	char							m_cDungeonMode;
//	//{{ 2011. 07. 08	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
//	char							m_cPartyType;
//#endif SERV_PVP_NEW_SYSTEM
//	//}}
//
//	KEGS_INVITE_PARTY_NOT()
//	{
//		m_iPartyUID			= 0;
//		m_iDungeonID		= 0;
//		m_cDifficultyLevel	= 0;
//		m_cDungeonMode		= 0;
//		//{{ 2011. 07. 08	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
//		m_cPartyType		= 0;
//#endif SERV_PVP_NEW_SYSTEM
//		//}}
//	}
//};
//
//DECL_PACKET( EGS_INVITE_PARTY_REPLY_NOT )			// ��Ƽ �ʴ� ���� ����� ������ ������ ���� ��Ŷ
//{
//	//{{ 2010. 10. 04	������	��Ƽ �ʴ� ���� ����
//#ifdef SERV_INVITE_PARTY_REPLY_REASON
//	enum REPLY_REASON
//	{
//		RR_ACCEPT = 0,
//		RR_NOT_ACCEPT,
//		RR_BUSY,
//	};
//#endif SERV_INVITE_PARTY_REPLY_REASON
//	//}}
//
//	UidType							m_iPartyUID;
//	//{{ 2010. 10. 04	������	��Ƽ �ʴ� ���� ����
//#ifdef SERV_INVITE_PARTY_REPLY_REASON
//	char							m_cReplyReason;
//#else
//	bool							m_bAccept;
//#endif SERV_INVITE_PARTY_REPLY_REASON
//	//}}
//};
//
////typedef KPacketOK	KEGS_INVITE_PARTY_DENY_NOT;
//DECL_PACKET( EGS_INVITE_PARTY_DENY_NOT )			// ��Ƽ �ʴ��� ����� timeout �Ŀ� �޴� packet
//{
//	//{{ 2010. 10. 04	������	��Ƽ �ʴ� ���� ����
//#ifdef SERV_INVITE_PARTY_REPLY_REASON
//	int								m_iDenyReasonNetError;
//#else
//	int								m_iOK;
//#endif SERV_INVITE_PARTY_REPLY_REASON
//	//}}
//	UidType							m_iUnitUID;
//};
//
//DECL_PACKET( EGS_UPDATE_PARTY_USER_INFO_NOT )
//{
//	UidType							m_iUnitUID;
//	UCHAR							m_cState;
//	int								m_iStateCode;
//	char						    m_cUnitClass;
//	UCHAR							m_ucLevel;
//	//{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
//#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
//	int								m_iEquippedTitleID;
//#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
//	//}}
//};
//
////{{ 2009. 3. 31  ������	��Ƽ����
//DECL_PACKET( EGS_UPDATE_PARTY_UID_NOT )
//{
//	UidType							m_iUnitUID;	
//	UidType							m_iPartyUID;
//};
////}}
//
//
////////////////////////////////////////////////////////////////////////////
//DECL_PACKET( EGS_PARTY_CHANGE_READY_REQ )
//{
//	bool							m_bReady;
//};
//
//typedef KPacketOK	KEGS_PARTY_CHANGE_READY_ACK;
//
//DECL_PACKET( EGS_PARTY_CHANGE_READY_NOT )
//{
//	UidType							m_UnitUID;
//	bool							m_bReady;
//};
//
//
////////////////////////////////////////////////////////////////////////////
//DECL_PACKET( EGS_PARTY_CHANGE_DUNGEON_REQ )
//{
//	int								m_iDungeonID;
//	char							m_DifficultyLevel;
//	char							m_cDungeonMode;
//};
//
//typedef KPacketOK	KEGS_PARTY_CHANGE_DUNGEON_ACK;
//
//typedef KEGS_PARTY_CHANGE_DUNGEON_REQ	KEGS_PARTY_CHANGE_DUNGEON_NOT;
//
//
////////////////////////////////////////////////////////////////////////////
//DECL_PACKET( EGS_PARTY_GAME_START_REQ )
//{
//	short							m_sWorldID;
//};
//
//typedef KPacketOK	KEGS_PARTY_GAME_START_ACK;
//
//DECL_PACKET( EGS_PARTY_GAME_START_NOT )
//{
//	short								m_sWorldID;
//	KRoomInfo						    m_RoomInfo;
//	std::vector< KRoomSlotInfo >        m_vecSlot;
//	std::vector< KRoomSlotInfo >		m_vecObserverSlot; // ���������ε� �������� �ʿ��ֳ�?
//
//	std::wstring						m_wstrCNIP;
//
//	//{{ 2009. 2. 4  ������		���� �����
//	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
//	//}}
//};
//
////{{ 2010. 12. 28	������	�ϸ� ���� �߰�
//#ifdef SERV_SHIP_DUNGEON
//
//DECL_PACKET( EGS_PARTY_TO_FIELD_MAP_REQ )
//{
//	enum PARTY_TO_FIELD_TYPE
//	{
//		PTFT_NONE = 0,
//		PTFT_LOUNGE_TO_FIELD,
//		PTFT_SHIP_DUNGEON_TO_FIELD,
//	};
//
//	char								m_cPartyToFieldType;
//	int									m_iFieldStartPos;
//
//	KEGS_PARTY_TO_FIELD_MAP_REQ()
//	{
//		m_cPartyToFieldType = PTFT_LOUNGE_TO_FIELD;
//		m_iFieldStartPos = -1;
//	}
//};
//
//DECL_PACKET( EGS_PARTY_TO_FIELD_MAP_ACK )
//{
//	int									m_iOK;
//	char								m_cPartyToFieldType;
//	int									m_iFieldStartPos;
//
//	KEGS_PARTY_TO_FIELD_MAP_ACK()
//	{
//		m_cPartyToFieldType = 0;
//		m_iFieldStartPos = -1;
//	}
//};
//
//DECL_PACKET( EGS_PARTY_TO_FIELD_MAP_NOT )
//{
//	int									m_iFieldStartPos;
//};
//
//#else
//typedef KPacketOK		KEGS_PARTY_TO_FIELD_MAP_ACK;
//#endif SERV_SHIP_DUNGEON
////}}
//
////////////////////////////////////////////////////////////////////////////
//typedef std::wstring	KEGS_PARTY_CHANGE_NAME_REQ;
//typedef KPacketOK		KEGS_PARTY_CHANGE_NAME_ACK;
//typedef std::wstring	KEGS_PARTY_CHANGE_NAME_NOT;
//
//
////////////////////////////////////////////////////////////////////////////
//typedef bool			KEGS_PARTY_CHANGE_PUBLIC_REQ;
//typedef KPacketOK		KEGS_PARTY_CHANGE_PUBLIC_ACK;
//typedef bool			KEGS_PARTY_CHANGE_PUBLIC_NOT;
//
//
////////////////////////////////////////////////////////////////////////////
//typedef UidType			KEGS_PARTY_CHANGE_HOST_REQ;
//typedef KPacketOK		KEGS_PARTY_CHANGE_HOST_ACK;
//
//DECL_PACKET( EGS_PARTY_CHANGE_HOST_NOT )
//{
//	UidType								m_iBeforeUID;
//	UidType								m_iAfterUID;
//};
//
//
////////////////////////////////////////////////////////////////////////////
//typedef UidType			KEGS_PARTY_BAN_USER_REQ;
//typedef KPacketOK		KEGS_PARTY_BAN_USER_ACK;
//
//
////////////////////////////////////////////////////////////////////////////
//typedef char			KEGS_PARTY_CHANGE_GET_ITEM_TYPE_REQ;
//typedef KPacketOK		KEGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK;
//typedef char			KEGS_PARTY_CHANGE_GET_ITEM_TYPE_NOT;
//
//
////////////////////////////////////////////////////////////////////////////
//typedef char			KEGS_PARTY_CHANGE_NUM_OF_PER_REQ;
//typedef KPacketOK		KEGS_PARTY_CHANGE_NUM_OF_PER_ACK;
//typedef char			KEGS_PARTY_CHANGE_NUM_OF_PER_NOT;
//
//
////////////////////////////////////////////////////////////////////////////
////{{ 2009. 4. 1  ������		��Ƽ����
//typedef bool			KEGS_PARTY_LIMIT_LEVEL_REQ;
//typedef KPacketOK		KEGS_PARTY_LIMIT_LEVEL_ACK;
//typedef bool			KEGS_PARTY_LIMIT_LEVEL_NOT;
////}}
//
//
////////////////////////////////////////////////////////////////////////////
////{{ 2009. 5. 1  ������		��Ƽ ������
//DECL_PACKET( EGS_PARTY_QUICK_JOIN_REQ )
//{
//	int						m_iDungeonID;
//	char					m_cDifficulty;
//	char					m_cDungeonMode;
//	//{{ 2011. 05. 27  ��μ�	���� ���̵� ���� ��� ����
//#ifdef SERV_DUNGEON_DIFFICULTY_REVISION_ADD_OPTION
//	bool					m_bCheckLowLevel;
//#endif SERV_DUNGEON_DIFFICULTY_REVISION_ADD_OPTION
//	//}}
//};
//
//DECL_PACKET( EGS_PARTY_QUICK_JOIN_ACK )
//{
//	int						m_iOK;	
//	UidType					m_iPartyUID;
//};
////}}

//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK )
{
	int						m_iOK;
	int						m_iDungeonID;
	int						m_iNumResurrectionStone;
	//{{ 2009. 10. 14  ������	�ڵ����� ��Ȱ��
#ifdef AP_RESTONE
	int						m_iNumAutoPaymentResStone;
#endif AP_RESTONE
	//}}
	//{{ 2008. 6. 16  ������  �ǽð� ��Ȱ��
	bool					m_bUseCash;
	//}}

	KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK()
	{
		m_bUseCash = false;
	}
};

DECL_PACKET( EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT )
{
	UidType					m_iUsedByUnitUID;
	UidType					m_iUnitUID;
	int						m_iStartPosIndex;
};


//////////////////////////////////////////////////////////////////////////
//{{ 2007. 8. 9  ������  ������Ʈ ��Ŷ����ü
DECL_PACKET( EGS_NEW_BLACKLIST_USER_REQ )
{
	std::wstring			m_wstrNickName;
};

DECL_PACKET( EGS_NEW_BLACKLIST_USER_ACK )
{
	int                     m_iOK;
	KChatBlackListUnit		m_kChatBlackListUnit;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_DEL_BLACKLIST_USER_REQ )
{
	UidType					m_iUnitUID;
};

typedef KEGS_NEW_BLACKLIST_USER_ACK		KEGS_DEL_BLACKLIST_USER_ACK;


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_UPDATE_COMMUNITY_OPTION_REQ )
{
	KDenyOptions			m_kDenyOptions;
};

DECL_PACKET( EGS_UPDATE_COMMUNITY_OPTION_ACK )
{
	int                     m_iOK;
	KDenyOptions			m_kDenyOptions;
};
//}}


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CHECK_SUM_REQ )
{
	std::map< std::wstring, std::wstring >	m_mapCheckSum;
	DWORD									m_ProtectTableCSVal;	//CheckSumValue
};

DECL_PACKET( EGS_CHECK_SUM_ACK )
{
	int                     m_iOK;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_KNM_REQUEST_NEW_FRIEND_INFO_REQ )
{
	std::wstring			m_wstrUnitNickName;
	//{{ 2010. 05. 18  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
	UidType					m_iUnitUID;

	KEGS_KNM_REQUEST_NEW_FRIEND_INFO_REQ()
	{
		m_iUnitUID = 0;
	}
#endif SERV_INTEGRATION
	//}}
};

DECL_PACKET( EGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK )
{
	int						m_iOK;
	u_int					m_uiKNMSerialNum;
	std::wstring			m_wstrUnitNickName;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_REQUEST_TRADE_REQ )
{
	UidType					m_iUnitUID;
};

DECL_PACKET( EGS_REQUEST_TRADE_ACK )
{
	int						m_iOK;
	UidType					m_iUnitUID;
	std::wstring			m_wstrNickName;
};

DECL_PACKET( EGS_REQUEST_TRADE_NOT )
{
	UidType                         m_iSenderUID;
	std::wstring                    m_wstrSenderNickName;
};

DECL_PACKET( EGS_REQUEST_TRADE_REPLY_NOT )
{
	UidType                         m_iReceiverUID;
	bool                            m_bAccept;
};

DECL_PACKET( EGS_REQUEST_TRADE_DENY_NOT )
{
	int                             m_iOK;
	UidType                         m_iUnitUID;
};

DECL_PACKET( EGS_CREATE_TRADE_NOT )
{
	UidType							m_iTradeUID;
};

DECL_PACKET( EGS_CREATE_PERSONAL_SHOP_ACK )
{
	int								m_iOK;
	UidType							m_iPersonalShopUID;
};


//////////////////////////////////////////////////////////////////////////
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
typedef KPacketReason               KEGS_BREAK_TRADE_REQ;
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
DECL_PACKET( EGS_BREAK_TRADE_REQ )
{
	int								m_iReason;
};
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

typedef KPacketOK		KEGS_BREAK_TRADE_ACK;

#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
typedef KPacketReason   KEGS_BREAK_TRADE_NOT;
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
DECL_PACKET( EGS_BREAK_TRADE_NOT )
{
	int								m_iReason;
};
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

DECL_PACKET( EGS_TRADE_USER_INFO_NOT )
{
	std::vector< KTradeUserInfo >	m_vecTradeUserInfo;
};

DECL_PACKET( EGS_JOIN_TRADE_NOT )
{
	KTradeUserInfo					m_kTradeUserInfo;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_UPDATE_TRADE_ITEM_REQ )
{
	UidType							m_iItemUID;	
	int								m_iQuantity;
	//{{ 2009. 1. 7  ������		�ŷ��� 10���� Ȯ��
	bool							m_bDelete;
	//}}
};

typedef KPacketOK		KEGS_UPDATE_TRADE_ITEM_ACK;

DECL_PACKET( EGS_UPDATE_TRADE_ITEM_NOT )
{
	UidType							m_iUnitUID;
	//{{ 2009. 1. 7  ������		�ŷ��� 10�� Ȯ��
	UidType							m_iItemUID;
	bool							m_bDelete;
	//}}
	KItemInfo						m_kItemInfo;
    std::map< UidType, bool >       m_mapOfferState;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_UPDATE_TRADE_ED_REQ )
{
	int								m_iED;
};

typedef KPacketOK		KEGS_UPDATE_TRADE_ED_ACK;

DECL_PACKET( EGS_UPDATE_TRADE_ED_NOT )
{
	UidType							m_iUnitUID;
	int								m_iED;
    std::map< UidType, bool >       m_mapOfferState;
};

typedef KPacketOK		KEGS_OFFER_TRADE_ACK;
typedef UidType			KEGS_OFFER_TRADE_NOT;

typedef KPacketOK		KEGS_APPROVE_TRADE_ACK;
typedef UidType			KEGS_APPROVE_TRADE_NOT;

DECL_PACKET( EGS_TRADE_COMPLETE_NOT )
{
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
	int									m_iED;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_REPAIR_ITEM_REQ )
{
	std::vector< UidType >				m_vecItemUID;
	UidType								m_iItemUID;
};

DECL_PACKET( EGS_REPAIR_ITEM_ACK )
{
	int									m_iOK;
	int									m_iED;
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	int                                 m_iAPoint;
#else
	int                                 m_iVP;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};

typedef KEGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT	KEGS_REPAIR_ITEM_NOT;


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_END_TC_GAME_REQ )
{
	bool		m_bIsSuccess;
};

DECL_PACKET( EGS_END_TC_GAME_ACK )
{
	int			m_iOK;
	
	bool		m_bIsSuccess;
	KUnitInfo	m_kUnitInfo;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_SET_TC_REMAINING_TIME_REQ )
{
	bool		m_bEnable;
};

typedef KPacketOK						KEGS_SET_TC_REMAINING_TIME_ACK;

//{{ 2007. 10. 23  ������  
typedef KEGS_LEAVE_ROOM_ACK				KEGS_LEAVE_TC_ROOM_ACK;
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2007. 10. 4  ������  ���� ������ ��Ŷ
DECL_PACKET( EGS_OPEN_RANDOM_ITEM_REQ )
{
	UidType                             m_iItemUID;
	//{{ 2012. 07. 12	������		���� ť�� ���� ������ �ý���
#ifdef SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
	bool								m_bDelayedOK;
#endif SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
	//}}
#ifdef SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
	bool								m_bNotifyGetItem;
#endif // SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
	
	KEGS_OPEN_RANDOM_ITEM_REQ()
	{
		m_iItemUID = 0;
		//{{ 2012. 07. 12	������		���� ť�� ���� ������ �ý���
#ifdef SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
		m_bDelayedOK = false;
#endif SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
		//}}
#ifdef SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
		m_bNotifyGetItem = true;
#endif // SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
	}
};

DECL_PACKET( EGS_OPEN_RANDOM_ITEM_ACK )
{
	int									m_iOK;	
	int									m_iRessurectionCount;
	int									m_iRestoreSpirit;
    std::map< int, int >                m_mapInsertedItem;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;

	//{{ 2012. 11. 26 ť�� ED ���� ���� ��� �߰� - ��μ�
#ifdef SERV_CUBE_OPEN_ED_CONDITION
	int									m_iED;
#endif SERV_CUBE_OPEN_ED_CONDITION
	//}

	KEGS_OPEN_RANDOM_ITEM_ACK()
	{		
		m_iRessurectionCount  = 0;
		//{{ 2012. 11. 26 ť�� ED ���� ���� ��� �߰� - ��μ�
#ifdef SERV_CUBE_OPEN_ED_CONDITION
		m_iED = 0;
#endif SERV_CUBE_OPEN_ED_CONDITION
		//}
	}
};
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2007. 10. 10  ������  ����
DECL_PACKET( EGS_ITEM_MANUFACTURE_REQ )
{
	int									m_iManufactureID;
#ifdef SERV_MANUFACTURE_FIX
	int									m_iQuantity;
#endif
};

DECL_PACKET( EGS_ITEM_MANUFACTURE_ACK )
{
	int									m_iOK;
    std::map< int, int >				m_mapInsertedItem;
	int									m_iED;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
};
//}}

DECL_PACKET( EGS_KNPROTECT_AUTH_CHECK_NOT )
{
	unsigned long						dwIndex;
	unsigned long						dwValue1;
	unsigned long						dwValue2;
	unsigned long						dwValue3;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_ADD_ON_STAT_REQ )
{
	enum ADD_ON_STAT
	{
		AOS_NONE = 0,
		AOS_ATK_PHYSIC,
		AOS_ATK_MAGIC,
		AOS_DEF_PHYSIC,
		AOS_DEF_MAGIC,

		AOS_END,
	};

	UidType								m_UID;
	bool								m_bIsUnit;
	char								m_cStatType;
	float								m_fStatVal;
	float								m_fTime;
};

typedef KPacketOK				KEGS_ADD_ON_STAT_ACK;

DECL_PACKET( EGS_ADD_ON_STAT_NOT )
{
	UidType								m_UID;
	bool								m_bIsUnit;
	char								m_cStatType;
	float								m_fStatVal;
	float								m_fTime;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_APPLY_COUPON_REQ )
{
    std::wstring                        m_wstrCouponSerial;

#ifdef SERV_GLOBAL_BILLING
	enum COUPON_TYPE
	{
		CT_ITEM_CARD = 0,
		CT_ZONE_CARD,

		CT_END
	};

	int									m_iCouponType;
#endif // SERV_GLOBAL_BILLING

	KEGS_APPLY_COUPON_REQ()
	{
		m_wstrCouponSerial	=	L"";

#ifdef SERV_GLOBAL_BILLING
		m_iCouponType		=	0;
#endif // SERV_GLOBAL_BILLING

	}
};

typedef KPacketOK		KEGS_APPLY_COUPON_ACK;


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_PURCHASED_CASH_ITEM_LIST_REQ )
{
    int                                 m_nItemPerPage;
    int                                 m_iCurrentPage;
};

DECL_PACKET( EGS_PURCHASED_CASH_ITEM_LIST_ACK )
{
    int                                 m_iOK;
    std::vector< KNXBTOrderInfo >       m_vecOrderInfo;
	std::vector< KNXBTPackageInfo >     m_vecPackageInfo;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_GET_PURCHASED_CASH_ITEM_REQ )
{
	// �ؿܿ����� EGS_BILL_GET_PURCHASED_CASH_ITEM_REQ ���

    unsigned long                       m_ulOrderNo;
    unsigned long                       m_ulProductNo;
	//{{ 2008. 4. 29  ������  ��ǰ �Ӽ�
	std::wstring						m_wstrProductID;
	unsigned short						m_usProductExpire;
	unsigned short						m_usProductPieces;
	BYTE								m_byteProductKind;
	//}}
	int									m_iQuantity;
    std::map< int, int >                m_mapSocketForCash;
	//{{ 2012. 11. 28 ĳ�� ������ ���� �ɼ� ��� ���� - ��μ�
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
	std::map< int, int >                m_mapSocketGroupIDForCash;		// <ItemID,GroupID>
#endif SERV_CASH_ITEM_SOCKET_OPTION
	//}
};

DECL_PACKET( EGS_GET_PURCHASED_CASH_ITEM_ACK )
{
    int                                 m_iOK;
    int                                 m_iNumResurrectionStone;
    std::map< int, int >                m_mapExpandedCategorySlot;
    std::vector< KInventoryItemInfo >   m_vecInventorySlotInfo;
	
	//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
	int									m_iExceptionProcessItemID;	// ����ó���� Item ID

	KEGS_GET_PURCHASED_CASH_ITEM_ACK()
	{
		m_iExceptionProcessItemID = 0;
	}
#endif SERV_GUILD_CHANGE_NAME
	//}}
};


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 6. 16  ������  �ؽ����� ��Ű�� �Ⱦ�
DECL_PACKET( EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ )
{
	// �ؿܿ����� �ش� ��Ŷ �״�� �� (SERV_GLOBAL_CASH_PACKAGE)

	unsigned long                       m_ulOrderNo;
	unsigned long                       m_ulPackageProductNo;
	std::wstring						m_wstrPackageProductID;
	BYTE								m_byteProductKind;

	//{{ 2008. 7. 28  ������	��Ű��
	std::vector< KNXBTPickUpPackageInfo > m_vecPickUpPackageInfo;
	//}}
	
	std::map< int, int >                m_mapSocketForCash;
	//{{ 2012. 11. 28 ĳ�� ������ ���� �ɼ� ��� ���� - ��μ�
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
	std::map< int, int >                m_mapSocketGroupIDForCash;		// <SocketID,GroupID>
#endif SERV_CASH_ITEM_SOCKET_OPTION
	//}
#ifdef SERV_GLOBAL_CASH_PACKAGE
	int									m_iProductID;
#endif // SERV_GLOBAL_CASH_PACKAGE
};

typedef KEGS_GET_PURCHASED_CASH_ITEM_ACK		KEGS_GET_PURCHASED_PACKAGE_CASH_ITEM_ACK;
//}}


//////////////////////////////////////////////////////////////////////////
typedef	std::wstring	KEGS_KNM_INVITE_REQ;
typedef	std::wstring	KEGS_KNM_TOGETHER_REQ;

typedef	KPacketOK		KEGS_KNM_INVITE_ACK;
DECL_PACKET( EGS_KNM_INVITE_NOT )
{
	std::wstring		m_wstrNickName;
	KNMUnitInfo			m_kUnitInfo;
};

typedef KPacketOK		KEGS_KNM_INVITE_RESULT_NOT;

DECL_PACKET( EGS_KNM_TOGETHER_ACK )
{
	int					m_iOK;
	KNMUnitInfo			m_kUnitInfo;
};


//////////////////////////////////////////////////////////////////////////
//{{ 2007. 11. 6  ������  ������ ���
DECL_PACKET( EGS_FRAME_AVERAGE_REQ )
{
	char				m_cGameType;
	int					m_iTotalFrame;
	int					m_iFrameAverage;
};

typedef KPacketOK		KEGS_FRAME_AVERAGE_ACK;
//}}

DECL_PACKET( ENX_USER_LOGIN_NOT )
{
	bool		m_bIsGameBang;
#ifdef SERV_PC_BANG_TYPE
	KPcBangReward
				m_kPcBangReward;
#endif SERV_PC_BANG_TYPE
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ )
{
	//{{ 2008. 6. 10  ������  �ǽð� ��Ȱ��
	bool				m_bUseCash;
	//}}
	UidType				m_iUsedByUnitUID;
    UidType             m_iUnitUID;

	KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ()
	{
		m_bUseCash = false;
		m_iUsedByUnitUID = 0;
		m_iUnitUID = 0;
	}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CHANGE_NICK_NAME_REQ )
{
    UidType             m_iUnitUID;
    std::wstring        m_wstrNickName;
	bool				m_bCheckOnly;
};

DECL_PACKET( EGS_CHANGE_NICK_NAME_ACK )
{
    int                 m_iOK;
    UidType             m_iUnitUID;
	std::wstring		m_wstrNickName;
	bool				m_bCheckOnly;
};


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 1. 2  ������  
DECL_PACKET( EGS_ADMIN_CHANGE_SPIRIT_REQ )
{
	int					m_iSpirit;
};

DECL_PACKET( EGS_ADMIN_CHANGE_SPIRIT_ACK )
{
	int                 m_iOK;
	int					m_iChangedSpirit;
};
//}}


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_ADMIN_SET_PC_BANG_REQ )
{
    bool                m_bOn;
#ifdef SERV_PC_BANG_TYPE
	int					m_iPcBangType;
#endif //SERV_PC_BANG_TYPE

	KEGS_ADMIN_SET_PC_BANG_REQ()
	{
#ifdef SERV_PC_BANG_TYPE
		m_iPcBangType		= 0;
#endif //SERV_PC_BANG_TYPE
	}
};

DECL_PACKET( EGS_ADMIN_SET_PC_BANG_ACK )
{
    int                 m_iOK;
    bool                m_bOn;
#ifdef SERV_PC_BANG_TYPE
	int					m_iPcBangType;
	KPcBangReward		m_kPcBangReward;
#endif //SERV_PC_BANG_TYPE

	KEGS_ADMIN_SET_PC_BANG_ACK()
	{
#ifdef SERV_PC_BANG_TYPE
		m_iPcBangType		= 0;
#endif //SERV_PC_BANG_TYPE
	}
};


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 1. 25  ������  
DECL_PACKET( EGS_ADMIN_CHANGE_VSPOINT_REQ )
{
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	int					m_iRating;
	int					m_iRPoint;
	int					m_iAPoint;
#else
	int					m_iVSPoint;
	int					m_iVSPointMax;
#endif SERV_PVP_NEW_SYSTEM
	//}}
};

DECL_PACKET( EGS_ADMIN_CHANGE_VSPOINT_ACK )
{
	int                 m_iOK;
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	char				m_cRank;
#else
	int					m_iChangedRating;
#endif SERV_2012_PVP_SEASON2
	//}}
	int					m_iChangedRPoint;
	int					m_iChangedAPoint;
#else
	int					m_iChangedVSPoint;
	int					m_iChangedVSPointMax;
#endif SERV_PVP_NEW_SYSTEM
	//}}
};
//}}


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CHECK_BALANCE_ACK )
{
    int                 m_iOK;
    unsigned long       m_ulBalance;	// �����ڵ� ȣȯ �� �ϳ��� ĳ�ø� ���� �������� ���� �״�ε�

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	bool				m_bOnlyType;		// �ܾ� ��ȸ�� �� �Ѱ��� Ÿ������ ���� (�Ѱ��� Ÿ���� ��� �ٸ� Type���� �ǵ��̸� �ȵ�)
	int					m_iCashType;		// �ܾ� ��ȸ�� �� �Ѱ��� Ÿ���� �� � ĳ��Ÿ������ ���θ� ���� ���
	KGlobalCashInfo     m_GlobalCashInfo;
#endif // SERV_SUPPORT_SEVERAL_CASH_TYPES

	KEGS_CHECK_BALANCE_ACK()
	{
		m_ulBalance = 0;

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		m_bOnlyType = false;
		m_iCashType = -1;
#endif // SERV_SUPPORT_SEVERAL_CASH_TYPES
	}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_REG_PERSONAL_SHOP_ITEM_REQ )
{
	std::wstring								m_wstrPersonalShopName;
	std::vector< KRegPersonalShopItemInfo >		m_vecRegItem;
};


DECL_PACKET( EGS_REG_PERSONAL_SHOP_ITEM_ACK )
{
	int											m_iOK;
	int											m_iED;

	std::wstring								m_wstrPersonalShopName;
	std::vector< KSellPersonalShopItemInfo >	m_vecSellItemInfo;
	//{{ 2011. 04. 18	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	std::vector< KInventoryItemInfo >			m_vecInventorySlotInfo;
#endif SERV_PSHOP_AGENCY
	//}}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_BUY_PERSONAL_SHOP_ITEM_REQ )
{
	UidType								m_iItemUID;
	int									m_iItemID;	
	int									m_iQuantity;
};

typedef KPacketOK	KEGS_BUY_PERSONAL_SHOP_ITEM_ACK;

DECL_PACKET( EGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT )
{
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
	int									m_iED;
};


typedef KPersonalShopItemInfo KEGS_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT;


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_BREAK_PERSONAL_SHOP_REQ )
{
	int									m_iReason;
};

typedef KPacketOK		KEGS_BREAK_PERSONAL_SHOP_ACK;

DECL_PACKET( EGS_BREAK_PERSONAL_SHOP_NOT )
{
	int									m_iReason;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_JOIN_PERSONAL_SHOP_REQ )
{
#ifdef DEF_TRADE_BOARD
	enum STAGE_KEY
	{
		SK_SQUARE = 0,
		SK_BOARD,
		//{{ 2011. 04. 18	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
		SK_AGENCY,
#endif SERV_PSHOP_AGENCY
		//}}
	};
#endif DEF_TRADE_BOARD
	//#Stage key ���� ���� uid �� ��� ������ Ʋ����.
	// SK_SQUARE : ���� Ű
	// SK_FIELD : ���λ��� Ű
	UidType		m_iUnitUID;

#ifdef DEF_TRADE_BOARD
	char	m_cStageKey;
#endif DEF_TRADE_BOARD
};

DECL_PACKET( EGS_JOIN_PERSONAL_SHOP_ACK )
{
	int											m_iOK;
	//{{ 2011. 07. 19	������	�븮���� ���� ������ ���� �̺�Ʈ
#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	bool										m_bIsPShopAgency;
#endif SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	//}}
	std::wstring								m_wstrPersonalShopName;
	std::wstring								m_wstrNickName;
	std::vector< KSellPersonalShopItemInfo >	m_vecSellItemInfo;

	KEGS_JOIN_PERSONAL_SHOP_ACK()
	{
		m_iOK = 0;
		//{{ 2011. 07. 19	������	�븮���� ���� ������ ���� �̺�Ʈ
#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
		m_bIsPShopAgency = false;
#endif SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
		//}}
	}
};

DECL_PACKET( EGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT )
{
	UidType								m_iUnitUID;
	char								m_cPersonalShopState;
	char								m_cPersonalShopType;
	std::wstring						m_wstrPersonalShopName;
};

typedef KPacketOK		KEGS_LEAVE_PERSONAL_SHOP_ACK;


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 2. 18  ������  
DECL_PACKET( EGS_ADMIN_CHANGE_UNIT_CLASS_REQ )
{
	char								m_cUnitClass;
};

DECL_PACKET( EGS_ADMIN_CHANGE_UNIT_CLASS_ACK )
{
	int									m_iOK;
	char							 	m_cUnitClass;

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
	int									m_iSPoint;
	int									m_iCSPoint;
	int									m_iDefaultSkill1;
	int									m_iDefaultSkill2;
	int									m_iDefaultSkill3;
	int									m_iDefaultSkill4;
	int									m_iDefaultSkill5;
	int									m_iDefaultSkill6;
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
};
//}}


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CHANGE_TRADE_NOT )
{
    UidType						        m_iUnitUID;
    bool								m_bTrade;
};


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 2. 18  ������  ����
typedef KEGS_OPEN_RANDOM_ITEM_REQ			KEGS_RESOLVE_ITEM_REQ;

DECL_PACKET( EGS_RESOLVE_ITEM_ACK )
{
	int									m_iOK;
	bool								m_bJackpot;
	std::map< int, int >                m_mapInsertedItem;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
	//{{ 2011.03.16   �ӱԼ� �ƹ�Ÿ ���� �ý���
#ifdef SERV_MULTI_RESOLVE
	int									m_iED;
#endif SERV_MULTI_RESOLVE
	//}}
};
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 2. 20  ������  ��ȭ
DECL_PACKET( EGS_ENCHANT_ITEM_REQ )
{
	UidType                             m_iItemUID;
    bool	                            m_bIsRareEnchantStone;
	bool								m_bIsNewEnchant;
	bool								m_bIsSupportMaterial;
#ifdef SERV_ENCHANT_PLUS_ITEM
	bool								m_bIsEnchantPlus;
#endif
#ifdef SERV_DESTROY_GUARD_ITEM
	bool								m_bIsDestroyGuard;
#endif
	bool								m_bDebug;
};

DECL_PACKET( EGS_ENCHANT_ITEM_ACK )
{
	int									m_iOK;
	int									m_iED;
	int									m_iEnchantResult;
	UidType								m_iEnchantedItemUID;
	int									m_iEnchantedItemID;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 3. 5  ������  ����

//////////////////////////////////////////////////////////////////////////
//{{ 2010. 04. 06  ������	������ ����
#ifdef SERV_SOCKET_NEW
//////////////////////////////////////////////////////////////////////////

DECL_PACKET( EGS_SOCKET_ITEM_REQ )
{
	UidType								m_iItemUID;
	std::map< int, UidType >			m_mapSocketInfo;
	bool								m_bCheat;
};

DECL_PACKET( EGS_SOCKET_ITEM_ACK )
{
	int									m_iOK;
	int									m_iED;
	UidType								m_iItemUID;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

DECL_PACKET( EGS_SOCKET_ITEM_REQ )
{
	UidType								m_iItemUID;
	int									m_iSocketIndex;
	bool								m_bIsRareMagicStone;
	bool								m_bIsRemoveSocketOption;
	bool								m_bDebug;
	short								m_sDebugSocketOption;
};

DECL_PACKET( EGS_SOCKET_ITEM_ACK )
{
	int									m_iOK;
	int									m_iED;
	int									m_iSocketResult;
	UidType								m_iSocketItemUID;
	int									m_iSocketItemID;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};

//////////////////////////////////////////////////////////////////////////
#endif SERV_SOCKET_NEW
//////////////////////////////////////////////////////////////////////////
//}}




//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_ITEM_EXPIRATION_NOT )
{
    std::vector< UidType >              m_vecItemUID;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_INIT_SKILL_TREE_REQ )
{
	UidType			m_iItemUID;
#ifdef SERV_SKILL_PAGE_SYSTEM
	int									m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM
};

DECL_PACKET( EGS_INIT_SKILL_TREE_ACK )
{
	int			m_iOK;
#ifdef SERV_SKILL_PAGE_SYSTEM
	int									m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM
	int			m_iSPoint;
	int			m_iCSPoint;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_RECOMMEND_USER_REQ )
{
	std::wstring						m_wstrNickName;
	bool								m_bCancel;
};

DECL_PACKET( EGS_RECOMMEND_USER_ACK )
{
	int									m_iOK;
	int									m_iRewardResurrectionStone;

	KEGS_RECOMMEND_USER_ACK()
	{
		m_iRewardResurrectionStone = 0;
	}
};

DECL_PACKET( EGS_RECOMMEND_USER_RESULT_NOT )
{
	UidType								m_iRecommendedUnitUID;
	UidType								m_iRecommendUnitUID;
	std::wstring						m_wstrFromNickName;
	//{{ 2010. 01. 11  ������	��õ�θ���Ʈ
	u_char								m_ucLevel;
	std::wstring						m_wstrRecommendDate;
	//}}
};

DECL_PACKET( EGS_REWARD_TO_RECOMMEND_USER_NOT )
{
	std::wstring						m_wstrNickName;
	char								m_cLevel;
	std::map< int, int >				m_mapRewardItem;
};

DECL_PACKET( EGS_REWARD_TO_TUTORIAL_USER_NOT )
{
	std::wstring						m_wstrNickName;
	char								m_cLevel;
	std::map< int, int >				m_mapRewardItem;
	bool								m_bGraduation;
};


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 9. 8  ������		��ü�� Ȯ��
DECL_PACKET( EGS_GET_POST_LETTER_LIST_REQ )
{
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	enum LETTER_LIST_TYPE
	{
		LLT_SYSTEM = 0,
		LLT_NORMAL,
		LLT_WEDDING,
	};
	
	char								m_cLetterListType;
#else
	bool								m_bSystemLetter;
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	int									m_iPostPageNo;
};

DECL_PACKET( EGS_GET_POST_LETTER_LIST_ACK )
{
	int									m_iOK;
	int									m_iTotalLetterCount;
	std::vector< KPostItemTitleInfo >	m_vecPostItemTitle;	
};

DECL_PACKET( EGS_NEW_POST_LETTER_INFO_NOT )
{
	int									m_iNewUserLetterCount;
	int									m_iNewSystemLetterCount;
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	int									m_iNewRelationshipLetterCount;
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	int									m_iTotalLetterCount;
	//{{ 2009. 5. 18  ������	���������� ����
	// revision his.hoons.2009.05.20.
	//std::vector< std::wstring >			m_vecNewLetterTitle;
	std::vector< KPostSmallTitleInfo >	m_vecNewLetterTitle;
	//}}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_SEND_LETTER_REQ )
{
	bool								m_bIsGmMessage;
	std::wstring						m_wstrToNickName;
	std::wstring						m_wstrTitle;
	std::wstring						m_wstrMessage;
	int									m_iED;
	UidType								m_iItemUID;	
	int									m_iQuantity;

	KEGS_SEND_LETTER_REQ()
	{
		m_bIsGmMessage	= false;
		m_iED			= 0;
		m_iItemUID		= 0;
		m_iQuantity		= 0;
	}
};

DECL_PACKET( EGS_SEND_LETTER_ACK )
{
	int									m_iOK;
	int									m_iED;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
};

DECL_PACKET( EGS_RECEIVE_LETTER_NOT )
{
	std::wstring						m_wstrNickName;
	//{{ 2009. 5. 18  ������	��������
	std::wstring						m_wstrLetterTitle;
	//}}
	char								m_cScriptType;
	int									m_iScriptIndex;
	//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
	UidType								m_iFromUnitUID;
#endif SERV_MONITORING_LETTER_AND_TRADE
	//}}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_READ_LETTER_REQ )
{
	UidType								m_iPostNo;
};

DECL_PACKET( EGS_READ_LETTER_ACK )
{
    int									m_iOK;
	KPostItemInfo						m_kLetter;
};

typedef KEGS_NEW_POST_LETTER_INFO_NOT KEGS_RENEWAL_LETTER_ACK;


//////////////////////////////////////////////////////////////////////////
typedef KEGS_READ_LETTER_REQ		KEGS_GET_ITEM_FROM_LETTER_REQ;

//{{ 2012. 08. 21	�ڼ���	���� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
#else
DECL_PACKET( EGS_GET_ITEM_FROM_LETTER_ACK )
{
	int									m_iOK;
	UidType								m_iPostNo;
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	char								m_cLetterType;
#else
	bool								m_bSystemLetter;
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	std::map< int, int >				m_mapResultItem;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
	int									m_iED;
};
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
//}}


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_DELETE_LETTER_REQ )
{
	std::vector< UidType >				m_vecPostNo;
};

DECL_PACKET( EGS_DELETE_LETTER_ACK )
{
	int									m_iOK;
	std::vector< UidType >				m_vecPostNo;
};
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 4. 2  ������  �ټ��� ȸ�� ������
DECL_PACKET( EGS_RESTORE_SPIRIT_NOT )
{
    UidType								m_iUnitUID;
	int									m_iSpirit;
};
//}}


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CASH_PRODUCT_INFO_REQ )
{
    int                                 m_iPage;
};

DECL_PACKET( EGS_CASH_PRODUCT_INFO_ACK )
{
    bool                                m_bFinal;
    std::vector< KNXBTProductInfo >     m_vecProductInfo;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_BUY_CASH_ITEM_REQ )
{
    std::vector< KNXBTPurchaseReqInfo > m_vecPurchaseReqInfo;
	bool								m_bIsWishList;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	int									m_iUseCashType;//KGlobalCashInfo�� enum GLOBAL_CASH_TYPE�� �� �ϳ�
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES	
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-23	// �ڼ���
	bool								m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM

	KEGS_BUY_CASH_ITEM_REQ()
		: m_bIsWishList( false )
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-23	// �ڼ���
		, m_bUseCoupon( false )
#endif // SERV_NEXON_COUPON_SYSTEM
	{
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		m_iUseCashType					= -1;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES	
	}
};

DECL_PACKET( EGS_BUY_CASH_ITEM_ACK )
{
    int                                 m_iOK;
    std::vector< KNXBTPurchaseReqInfo > m_vecPurchaseReqInfo;	
	bool								m_bIsWishList;
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-23	// �ڼ���
	bool								m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM

	KEGS_BUY_CASH_ITEM_ACK()
		: m_iOK( 0 )
		, m_bIsWishList( false )
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-23	// �ڼ���
		, m_bUseCoupon( false )
#endif // SERV_NEXON_COUPON_SYSTEM
	{
	}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_PRESENT_CASH_ITEM_REQ )
{
	std::wstring						m_wstrPassword;
    std::wstring                        m_wstrReceiverNickName;
    std::wstring                        m_wstrMessage;
    std::vector< KNXBTPurchaseReqInfo > m_vecPurchaseReqInfo;
	bool								m_bIsWishList;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	int									m_iUseCashType;	//KGlobalCashInfo�� enum GLOBAL_CASH_TYPE�� �� �ϳ�
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-07-29	// �ڼ���
	bool								m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM

	KEGS_PRESENT_CASH_ITEM_REQ()
		: m_bIsWishList( false )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		, m_iUseCashType( -1 )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES		
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-07-29	// �ڼ���
		, m_bUseCoupon( false )
#endif // SERV_NEXON_COUPON_SYSTEM
	{
	}
};

typedef KEGS_BUY_CASH_ITEM_ACK		KEGS_PRESENT_CASH_ITEM_ACK;


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 10. 6  ������	�ڵ�����
DECL_PACKET( EGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_REQ )
{
	unsigned long						m_ulProductNo;
	unsigned short						m_usOrderQuantity;
};

DECL_PACKET( EGS_BUY_CASH_ITEM_AUTOMATIC_PAYMENT_ACK )
{
	int									m_iOK;
	UINT								m_uiUserSN;
	BYTE								m_byDomainType;
	std::wstring						m_wstrTransactionID;
	unsigned long						m_ulProductNo;
	unsigned long						m_ulSalePrice;
	unsigned short						m_usOrderQuantity;
};

DECL_PACKET( EGS_AUTO_PAYMENT_RESURRECTION_STONE_NOT )
{
    int									m_iNumAutoPaymentResStone;
};
//}}


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_DEL_TUTORIAL_REQ )
{
	UidType								m_iUnitUID;
};

typedef KPacketOK					KEGS_DEL_TUTORIAL_ACK;

DECL_PACKET( EGS_DEL_TUTORIAL_NOT )
{
	UidType								m_iUnitUID;
	//{{ 2011.09.16 ��ȿ��  �� ���� ��Ʈ���� Ŭ���̾�Ʈ�ʿ��� �����ͼ� ���
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
	int									m_iReason;
#else SERV_NETERROR_STR_GET_FROM_CLIENT
	std::wstring						m_wstrReason;
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
	//}}

	KEGS_DEL_TUTORIAL_NOT()
	{
		m_iUnitUID = 0;
		//{{ 2011.09.16 ��ȿ��  �� ���� ��Ʈ���� Ŭ���̾�Ʈ�ʿ��� �����ͼ� ���
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
		m_iReason = 0;
#else SERV_NETERROR_STR_GET_FROM_CLIENT
		m_wstrReason = L"";
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
		//}}
	}
};

DECL_PACKET( EGS_TUTORIAL_UPDATE_UNIT_INFO_NOT )
{
	bool								m_bIsReply;				//�ٽ� ��������Ʈ�� �������Ѵ���.
	UidType								m_iReceiverUID;
	KTutorialUnitInfo					m_kTutorialUnitInfo;	//������Ʈ�� ����.
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_REQUEST_TUTORIAL_REQ )
{
	UidType								m_iUnitUID;
};

typedef KPacketOK	KEGS_REQUEST_TUTORIAL_ACK;

typedef KTutorialUnitInfo	KEGS_REQUEST_TUTORIAL_NOT;

DECL_PACKET( EGS_REQUEST_TUTORIAL_REPLY_NOT )
{
	UidType								m_iReceiverUnitUID;
	KTutorialUnitInfo					m_kTutorialUnitInfo;
	bool							    m_bAccept;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_TUTORIAL_STUDENT_LIST_REQ )
{
	int									m_iViewPage;
};

DECL_PACKET( EGS_TUTORIAL_STUDENT_LIST_ACK )
{
	int									m_iOK;
	int									m_iTotalPage;
	int									m_iViewPage;

	std::vector< KTutorialUnitInfo >	m_vecTutorialStudentList;
};


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 4. 14  ������  ��ų �ʱ�ȭ
DECL_PACKET( EGS_RESET_SKILL_REQ )
{
	int									m_iSkillID;
#ifdef SERV_SKILL_PAGE_SYSTEM
	int									m_iActiveSkillPageNumber;	/// Ȱ��ȭ ���� ������

	KEGS_RESET_SKILL_REQ()
		: m_iSkillID(0), m_iActiveSkillPageNumber(0)
	{}
#endif // SERV_SKILL_PAGE_SYSTEM
};

DECL_PACKET( EGS_RESET_SKILL_ACK )
{
	int									m_iOK;
	int									m_iDelSkillID;

#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
	int									m_iSPoint;
	int									m_iCSPoint;
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
	int									m_iRestoreSPoint;
	int									m_iRestoreCSPoint;
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;

#ifdef SERV_SKILL_PAGE_SYSTEM
	int									m_iActiveSkillPageNumber;	/// Ȱ��ȭ ���� ������

	KEGS_RESET_SKILL_ACK()
		: m_iOK(0), m_iActiveSkillPageNumber(0), m_iDelSkillID(0),
		m_iSPoint(0), m_iCSPoint(0)
	{
		m_vecInventorySlotInfo.clear();
	}
#endif // SERV_SKILL_PAGE_SYSTEM
};

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CURRENT_TIME_ACK )
{
    std::wstring                        m_wstrCurrentTime;
#ifdef SERVER_CURRENT_TIME_INT64
	__int64								m_tCurrentTime;
#endif //SERVER_CURRENT_TIME_INT64
};


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 5. 5  ������  ��ٱ���
DECL_PACKET( EGS_GET_WISH_LIST_ACK )
{
	std::map< int, int >				m_mapWishList;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_MODIFY_WISH_LIST_REQ )
{
	std::vector< int >					m_vecWishListItemID;
	bool								m_bIsDelete;
};

DECL_PACKET( EGS_MODIFY_WISH_LIST_ACK )
{
	int									m_iOK;
	std::vector< int > 					m_vecWishListItemID;
	bool								m_bIsDelete;
};
//}}


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_AGREE_HACK_USER_ACK )
{
	int									m_iOK;
	char								m_cHackingUserType;
};


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 6. 16  ������  ���� ��Ƽ�� �ð� ���߱�
DECL_PACKET( EGS_STOP_DUNGEON_CONTINUE_TIME_REQ )
{
	bool								m_bIsStop;
};

DECL_PACKET( EGS_STOP_DUNGEON_CONTINUE_TIME_ACK )
{
    int									m_iOK;
	bool								m_bIsStop;
};
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 6. 20  ������  ĳ�� �κ� ���� Ȯ��
DECL_PACKET( EGS_NEW_PRESENT_CASH_INVENTORY_NOT )
{
	int									m_iPresentCount;
};

DECL_PACKET( EGS_CHECK_PRESENT_CASH_INVENTORY_NOT )
{
	unsigned long                       m_ulOrderNo;
	unsigned long                       m_ulProductNo;
};

DECL_PACKET( EGS_PRESENT_MESSAGE_TO_RECEIVER_NOT )
{
	std::wstring						m_wstrSenderNickName;
	std::wstring                        m_wstrReceiverUserID;
	std::wstring                        m_wstrMessage;
#ifdef SERV_GLOBAL_BILLING
	KBillProductInfo					m_kBillProductInfo;		// SERV_GLOBAL_BILLING
#else
	KNXBTProductInfo					m_kNXBTProductInfo;
#endif SERV_GLOBAL_BILLING
};
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 7. 6  ������		��ŷ ����
DECL_PACKET( EGS_GET_RANKING_INFO_REQ )
{
    int									m_iRankingType;
	UINT								m_uiViewPage;
};

DECL_PACKET( EGS_GET_RANKING_INFO_ACK )
{
	int									m_iRankingType;
	UINT								m_uiViewPage;
	UINT								m_uiTotalPage;

	std::vector< KHenirRankingInfo >	m_vecHenirRankingPage;
	std::vector< KDungeonRankingInfo >	m_vecDungeonRankingPage;
	std::vector< KPvpRankingInfo >		m_vecPvpRankingPage;

	std::vector< KHenirRankingInfo >	m_vecMyHenirRanking;
	std::vector< KDungeonRankingInfo >	m_vecMyDungeonRanking;
	std::vector< KPvpRankingInfo >		m_vecMyPvpRanking;
};
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 9. 3  ������		�Ӽ���ȭ
DECL_PACKET( EGS_ATTRIB_ENCHANT_ITEM_REQ )
{
	UidType                             m_iItemUID;
	char								m_cAttribEnchantSlotNo;
	char								m_cAttribEnchantID;
};

DECL_PACKET( EGS_ATTRIB_ENCHANT_ITEM_ACK )
{
	int									m_iOK;
	int									m_iED;
	UidType								m_iEnchantedItemUID;
	int									m_iEnchantedItemID;
	char								m_cAttribEnchantSlotNo;
	char								m_cAttribEnchantID;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_IDENTIFY_ITEM_REQ )
{
	UidType								m_iItemUID;	
	int									m_iQuantity;
};

DECL_PACKET( EGS_IDENTIFY_ITEM_ACK )
{
	int									m_iOK;
	std::map< int, int >                m_mapInsertedItem;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
};
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 9. 26  ������	��ü�� ������Ʈ
DECL_PACKET( EGS_GET_POST_BLACK_LIST_ACK )
{
	std::vector< std::wstring >			m_vecBlackList;
};

DECL_PACKET( EGS_NEW_POST_BLACK_LIST_REQ )
{
	std::wstring						m_wstrNickName;
};

DECL_PACKET( EGS_NEW_POST_BLACK_LIST_ACK )
{
	int									m_iOK;
	std::wstring						m_wstrNickName;
};

typedef KEGS_NEW_POST_BLACK_LIST_REQ		KEGS_DEL_POST_BLACK_LIST_REQ;
typedef KEGS_NEW_POST_BLACK_LIST_ACK		KEGS_DEL_POST_BLACK_LIST_ACK;
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 10. 6  ������	Ÿ��Ʋ
DECL_PACKET( EGS_EQUIP_TITLE_REQ )
{
	int									m_iTitleID;
};

DECL_PACKET( EGS_EQUIP_TITLE_ACK )
{
	int									m_iOK;
	int									m_iTitleID;
};

DECL_PACKET( EGS_TITLE_EXPIRATION_NOT )
{
    int									m_iTitleID;
};

DECL_PACKET( EGS_NEW_MISSION_NOT )
{
	bool								m_bFirst;
	std::vector< KMissionInstance >		m_vecNewMission;

	KEGS_NEW_MISSION_NOT()
	{
		m_bFirst = false;
	}
};

DECL_PACKET( EGS_UPDATE_MISSION_NOT )
{
	std::vector< KMissionInstance >		m_vecMissionInst;
};

DECL_PACKET( EGS_REWARD_TITLE_NOT )
{
    KTitleInfo							m_kTitle;
};
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 10. 9  ������	�����ۻ��
DECL_PACKET( EGS_USE_ITEM_IN_INVENTORY_REQ )
{
	UidType		m_iItemUID;

	//# ������ �� �߰� �����Ͱ� ������� ����ϱ�����
	//1. �������� 131134(�ڵ����� �����̵� �������� ���)�̸� �̵��� �ʾ��̵� ��������..
	//{{ 2012. 12. 24	�ڼ���	�� ���� ���� ��� �߰�
//#ifdef SERV_PET_AUTO_LOOTING
	__int64		m_iTempCode;
//#else
//	int			m_iTempCode;
//#endif SERV_PET_AUTO_LOOTING
	//}}

	KEGS_USE_ITEM_IN_INVENTORY_REQ()
		: m_iItemUID( 0 )
		, m_iTempCode( 0 )
	{
	}
};

DECL_PACKET( EGS_USE_ITEM_IN_INVENTORY_ACK )
{
	int									m_iOK;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
	//{{ 2009. 3. 9  ������		��Ż
	int									m_iUsedItemID;
	int									m_iWarpPointMapID;
	//}}
	//{{ 2012. 10. 25	�ڼ���	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	int									m_iED;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 12. 24	�ڼ���	�� ���� ���� ��� �߰�
//#ifdef SERV_PET_AUTO_LOOTING
	__int64								m_iTempCode;
//#endif SERV_PET_AUTO_LOOTING
	//}}

	KEGS_USE_ITEM_IN_INVENTORY_ACK()
		: m_iOK( 0 )
		//{{ 2012. 10. 25	�ڼ���	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		, m_iED( 0 )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2012. 12. 24	�ڼ���	�� ���� ���� ��� �߰�
//#ifdef SERV_PET_AUTO_LOOTING
		, m_iTempCode( 0 )
//#endif SERV_PET_AUTO_LOOTING
		//}}
	{
	}
};
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 10. 24  ������	Ÿ��Ʋ ġƮ
DECL_PACKET( EGS_ADMIN_GET_TITLE_CHEAT_REQ )
{
    int									m_iTitleID;
	int									m_iPeriod;
};

typedef KPacketOK		KEGS_ADMIN_GET_TITLE_CHEAT_ACK;
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2008. 10. 29  ������	��������Ұ�
typedef KPacketOK		KEGS_DUNGEON_REQUIRE_ITEM_MESSAGE_NOT;
//}}

//{{ 2008. 12. 14  ������	ĳ���� ���� Ȯ��
DECL_PACKET( EGS_EXPAND_CHAR_SLOT_NOT )
{
    int									m_iCharSlotSize;
};
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 11. 19  ������	������ ����
DECL_PACKET( EGS_RESTORE_ITEM_REQ )
{
	UidType                             m_iItemUID;
};

DECL_PACKET( EGS_RESTORE_ITEM_ACK )
{
	int									m_iOK;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
};
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 12. 25  ������	�ο�
DECL_PACKET( EGS_ENCHANT_ATTACH_ITEM_REQ )
{
	UidType                             m_iDestItemUID;
	UidType                             m_iAttachItemUID;
};

DECL_PACKET( EGS_ENCHANT_ATTACH_ITEM_ACK )
{
	int									m_iOK;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
};
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2008. 11. 4  ������	���� ����
DECL_PACKET( EGS_STATE_CHANGE_FIELD_REQ )
{
	int									m_iMapID;
	//{{ 2012. 10. 18	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool								m_bUseWarpItem;	// ���� �̵� ������ ���� true��
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	KEGS_STATE_CHANGE_FIELD_REQ()
	{
		m_iMapID = 0;
		//{{ 2012. 10. 18	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_bUseWarpItem = false;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}
};

//{{ 2012. 09. 21   ��μ�   ���� ���� ���� �޽��� ����
#ifdef SERV_ENTER_VILLAGE_ERROR
DECL_PACKET( EGS_STATE_CHANGE_FIELD_ACK )
{
	int									m_iOK;
	int									m_iLimitLevel;
	int									m_iRequireDungeonID;

	KEGS_STATE_CHANGE_FIELD_ACK()
	{
		m_iOK = 0;
		m_iLimitLevel = 0;
		m_iRequireDungeonID = 0;
	};
};
#else
typedef KPacketOK		KEGS_STATE_CHANGE_FIELD_ACK;
#endif SERV_ENTER_VILLAGE_ERROR
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 12. 22  ������	�ʵ� �ε�
DECL_PACKET( EGS_FIELD_LOADING_COMPLETE_REQ )
{	
	VECTOR3								m_vPos;
	unsigned char						m_ucLastTouchLineIndex;
	unsigned short						m_usLastPosValue;
};

//{{ 2012. 05. 16	�ڼ���	ù ���� �� ���̵� ���� ����ֱ�
#ifdef SERV_EVENT_GUIDELINE_POPUP
DECL_PACKET( EGS_FIELD_LOADING_COMPLETE_ACK )
{
	int									m_iOK;
	bool								m_bPopupTheGuideLine;
};
#else
typedef KPacketOK		KEGS_FIELD_LOADING_COMPLETE_ACK;
#endif SERV_EVENT_GUIDELINE_POPUP
//}}

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CHANGE_USER_LIST_IN_SECTOR_NOT )
{
	std::vector< KFieldUserInfo >		m_vecNewUserList;
    std::vector< UidType >				m_vecDelUserList;
};
//}}


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_FIELD_UNIT_SYNC_DATA_NOT )
{
	VECTOR3								m_vPos;
	std::vector< KFieldFrameInfo >		m_vecFieldFrame;
	bool								m_bIsRight;

	unsigned char						m_ucLastTouchLineIndex;
	unsigned short						m_usLastPosValue;	
};

DECL_PACKET( EGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT )
{
	UidType								m_UnitUID;
	VECTOR3								m_vPos;
	std::vector< KFieldFrameInfo >		m_vecFieldFrame;
	bool								m_bIsRight;	
};


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 1. 13  ������	�ʵ� ���� ����Ʈ
DECL_PACKET( EGS_COMMUNITY_USER_LIST_REQ )
{
	UINT								m_uiViewPage;
	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
	bool								m_bMode;
	KEGS_COMMUNITY_USER_LIST_REQ()
	{
		m_bMode = false;
	};
#endif SERV_ED_MONITORING_IN_GAME
	//}}
};

DECL_PACKET( EGS_COMMUNITY_USER_LIST_ACK )
{
	int									m_iOK;
	UINT								m_uiTotalPage;
	UINT								m_uiViewPage;
	std::vector< KCommunityUserInfo >	m_vecUserList;
};
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2008. 12. 9  ������	ģ�� ����Ʈ
DECL_PACKET( EGS_OPTION_UPDATE_REQ )
{
    KFieldOption						m_kFieldOption;
};

typedef KPacketOK		KEGS_OPTION_UPDATE_ACK;


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 2. 13  ������	ä�� �̵�
//DECL_PACKET( EGS_CHANNEL_CHANGE_REQ )
//{
//	int									m_iChannelID;
//};
//
//DECL_PACKET( EGS_CHANNEL_CHANGE_ACK )
//{
//	int									m_iOK;
//	UidType								m_iServerUID;
//	UidType								m_iAccountAuthKey;
//	UidType								m_iUnitUID;
//};


//////////////////////////////////////////////////////////////////////////
//DECL_PACKET( EGS_CONNECT_CHANNEL_CHANGE_REQ )
//{
//	std::wstring						m_wstrVersion;
//	UidType								m_iServerUID;
//	UidType								m_iAccountAuthKey;
//	UidType								m_iUnitUID;
//};
//
//DECL_PACKET( EGS_CONNECT_CHANNEL_CHANGE_ACK )
//{
//	int									m_iOK;
//	int									m_UDPPort;
//};
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 4. 7  ������		ģ������
DECL_PACKET( EGS_UPDATE_FRIEND_INFO_REQ )
{
    bool								m_bDelete;
	std::wstring						m_wstrFriendNickName;
};

DECL_PACKET( EGS_UPDATE_FRIEND_INFO_ACK )
{
	int									m_iOK;
	std::wstring						m_wstrFriendNickName;
	UidType								m_iFriendUnitUID;
	//{{ 2009. 4. 17  ������	ä��id
	int									m_iChannelID;
	//}}
};
//}}

//////////////////////////////////////////////////////////////////////////
typedef UidType	KEGS_GET_MY_MESSENGER_SN_REQ;

DECL_PACKET( EGS_GET_MY_MESSENGER_SN_ACK )
{
	int									m_iOK;
	u_int								m_uiKNMSerialNum;	//�ؽ��ڸ��� �޽����� ���� ������ �ѹ�
};
//////////////////////////////////////////////////////////////////////////




//{{ 2009. 5. 8  ������		�ǽð� ������
DECL_PACKET( EGS_CHAR_LEVEL_UP_NOT )
{
    UidType								m_iUnitUID;
	UCHAR                           	m_ucLevel;
	KStat								m_kBaseStat;
	KStat                               m_kGameStat;
#ifdef SERV_ELESIS_UPDATE_EVENT
	int									m_iNoteViewCount;
#endif SERV_ELESIS_UPDATE_EVENT

	KEGS_CHAR_LEVEL_UP_NOT()
	{
		m_iUnitUID	= 0;
		m_ucLevel	= 0;
#ifdef SERV_ELESIS_UPDATE_EVENT
		m_iNoteViewCount = 0;
#endif SERV_ELESIS_UPDATE_EVENT
	}
};
//}}

//{{ 2009. 5. 14  ������	������ �Ҹ� ����
DECL_PACKET( EGS_DECREASE_ENDURANCE_NOT )
{
	std::vector< KDecreaseEnduranceInfo > m_vecEnduranceUpdate;
};

DECL_PACKET( EGS_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT )
{
	UidType                             m_UnitUID;
	std::vector< KInventoryItemInfo >   m_vecInventorySlotInfo;
	KStat                               m_kGameStat;
};

DECL_PACKET( EGS_DECREASE_SPIRIT_NOT )
{
    int									m_iSpirit;
	//{{ 2009. 10. 29  ������	��巹��
#ifdef GUILD_TEST
	int									m_iHonorPoint;
#endif GUILD_TEST
	//}}
#ifdef SERV_CHINA_SPIRIT_EVENT
	int									m_arrChinaSpirit[6];
#endif SERV_CHINA_SPIRIT_EVENT
};
//}}



//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_UPDATE_CASH_SKILL_POINT_NOT )
{
	int				m_iOK;
	UidType			m_iUnitUID;
	int				m_iCSPoint;
	std::wstring	m_wstrCSPointEndDate;
	bool			m_bUpdateEndDateOnly;
	//{{ 2010. 12. 8	������	�̺�Ʈ�� �׳�ý� �ູ
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
	int				m_iSkillPointItemID;
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
	//}}
};


DECL_PACKET( EGS_EXPIRE_CASH_SKILL_POINT_NOT )
{
	UidType							m_iUnitUID;

#ifdef SERV_SKILL_PAGE_SYSTEM
	std::vector<int>				m_vecRetrievedSPoint;

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
	std::vector< std::map<int,int> >	m_vecMapHaveSKill;
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#else // SERV_SKILL_PAGE_SYSTEM
	int								m_iRetrievedSPoint;

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
	std::map<int, int>				m_mapHaveSKill;
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#endif // SERV_SKILL_PAGE_SYSTEM
};


DECL_PACKET( EGS_EXPAND_SKILL_SLOT_NOT )
{
	UidType							m_iUnitUID;
	char							m_cSkillSlotBExpirationState;		// KUserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE
	std::wstring					m_wstrSkillSlotBEndDate;
};

//{{ 2009. 8. 4  ������		���ν�ų
DECL_PACKET( EGS_UNSEAL_SKILL_NOT )
{
	int								m_iSkillID;
};
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 6. 3  ������		Ȯ����
DECL_PACKET( EGS_USE_MEGAPHONE_REQ )
{
	UidType								m_iItemUID;
	std::wstring						m_wstrMessage;
};

DECL_PACKET( EGS_USE_MEGAPHONE_ACK )
{
	int									m_iOK;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};

DECL_PACKET( EGS_MEGAPHONE_MESSAGE_NOT )
{
	std::wstring						m_wstrSenderNickName;
	int									m_iSenderChannelID;
	int									m_iSenderLocation;
	u_char								m_cMessageColor;	
	std::wstring						m_wstrMessage;
};
//}}

//{{ 2009. 7. 10  ������	��� �ý���
DECL_PACKET( EGS_PARTY_FEVER_POINT_NOT )
{
	int									m_iPartyFever;
};

DECL_PACKET( EGS_CHANGE_PARTY_FEVER_STATE_NOT )
{
	bool								m_bIsStart;
	int									m_iPartyFever;

	KEGS_CHANGE_PARTY_FEVER_STATE_NOT()
	{
		m_bIsStart	  = false;
		m_iPartyFever = 0;
	}
};
//}}

DECL_PACKET( EGS_FRIEND_POSITION_UPDATE_NOT )
{
    std::map< UidType, char >           m_mapFriendPosition;
};





//{{ dmlee 2009/07/17 ��ϸ� �ð� ��� �������� ���� ����
DECL_PACKET( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK )
{
	int									m_iOK;
	bool								m_bCanEnterSecretStage;
};


DECL_PACKET( EGS_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT )
{
	bool m_bWantToEnterSecretStage;
};


DECL_PACKET( EGS_DUNGEON_SECRET_STAGE_ENTER_RESULT_NOT )
{
	bool m_bProceedToEnterSecretStage;
};
//}} dmlee 2009/07/17 ��ϸ� �ð� ��� �������� ���� ����


//{{ 2009. 8. 7  ������		����
DECL_PACKET( EGS_GET_MY_BANK_INFO_ACK )
{
	std::map< UidType, KInventoryItemInfo >	m_mapItem;
};

DECL_PACKET( EGS_EXPAND_BANK_SLOT_NOT )
{
	std::map< int, int >					m_mapExpandedCategorySlot;
    int                                     m_iOK;
    int                                     m_iED;
};
//}}

//{{ 2009. 8. 18  ������	HackShield
typedef std::vector< unsigned char >		KEGS_HACKSHIELD_CHECK_REQ;
typedef std::vector< unsigned char >		KEGS_HACKSHIELD_CHECK_ACK;
//}}

//{{ 2009. 8. 20  ������	���б� �̺�Ʈ
DECL_PACKET( EGS_PVP_KILL_DIE_INFO_NOT )
{
	bool									m_bShowKillCount;
    int										m_iKillCount;
	int										m_iDieCount;
};
//}}

//{{ 2009. 8. 27  ������	�к�
DECL_PACKET( EGS_SEAL_ITEM_REQ )
{
    UidType									m_iDestItemUID;
	UidType									m_iSealItemUID;
};

DECL_PACKET( EGS_SEAL_ITEM_ACK )
{
	int										m_iOK;
	std::vector< KInventoryItemInfo >		m_vecInventorySlotInfo;
};

DECL_PACKET( EGS_UNSEAL_ITEM_REQ )
{
	UidType									m_iDestItemUID;
};

typedef KEGS_SEAL_ITEM_ACK		KEGS_UNSEAL_ITEM_ACK;
//}}

DECL_PACKET( ECH_VERIFY_ACCOUNT_REQ )
{
    bool                                m_bLogin;
    std::wstring                        m_wstrID;
    std::wstring                        m_wstrPassword;
#ifdef SERV_GLOBAL_AUTH
	int									m_iChannelingCode;	// �ؿ� �������� ����
#endif // SERV_GLOBAL_AUTH

#ifdef SERV_USE_PASSWORD_TOKEN
	std::string							m_strUserToken;
#endif // SERV_USE_PASSWORD_TOKEN

	//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	bool								m_bUseVerify;
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}
	//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	std::string							m_strMachineID;
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	//}}
	//{{ 2011.11.02     ��μ�    �������� SN �߱� �� ��ȿ�� �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	unsigned char						m_charServerSN[SERVER_SN];
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	//}}

	//{{ 2013.03.05 ��ȿ��		KOG_OTP ���� �� �籸��
#ifdef SERV_KOG_OTP_VERIFY
	bool								m_bIDVerified;
#endif SERV_KOG_OTP_VERIFY
	//}}
#ifdef SERV_LOGIN_MAC_ADDRESS
	std::wstring						m_wstrMACAddress;
#endif // SERV_LOGIN_MAC_ADDRESS

#ifdef SERV_PUBLISHER_OTP
	std::wstring						m_wstrPublisherOTP;
#endif // SERV_PUBLISHER_OTP

#ifdef SERV_USE_DOMAIN_ACCOUNT
	std::wstring						m_wstrDomain;
#endif // SERV_USE_DOMAIN_ACCOUNT

#ifdef SERV_STEAM
	bool								m_bSteamClient;// ���� Ŭ���̾�Ʈ�� ����Ǿ���
#endif //SERV_STEAM

#ifdef SERV_CHANNELING_AERIA
	bool								m_bAeriaClient;
#endif //SERV_CHANNELING_AERIA
	
	KECH_VERIFY_ACCOUNT_REQ()
	{
#ifdef SERV_GLOBAL_AUTH
		m_iChannelingCode = 0;		// �ؿ� �������� ����
#endif //SERV_GLOBAL_AUTH

#ifdef SERV_USE_PASSWORD_TOKEN
		m_strUserToken	= "";
#endif // SERV_USE_PASSWORD_TOKEN

		//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
		m_bUseVerify = false;
		//{{ 2011.11.02     ��μ�    �������� SN �߱� �� ��ȿ�� �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
		for( int i = 0 ; i < SERVER_SN ; ++i )
		{
			m_charServerSN[i] = 0;
		}
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
		//}}

#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
		//}}

		//{{ 2013.03.05 ��ȿ��		KOG_OTP ���� �� �籸��
#ifdef SERV_KOG_OTP_VERIFY
		m_bIDVerified	= false;
#endif SERV_KOG_OTP_VERIFY
		//}}
#ifdef	SERV_LOGIN_MAC_ADDRESS
		m_wstrMACAddress = L"";
#endif	// SERV_LOGIN_MAC_ADDRESS

#ifdef SERV_PUBLISHER_OTP
		m_wstrPublisherOTP = L"";
#endif // SERV_PUBLISHER_OTP
#ifdef SERV_USE_DOMAIN_ACCOUNT
		m_wstrDomain = L"";
#endif // SERV_USE_DOMAIN_ACCOUNT

#ifdef SERV_STEAM
		m_bSteamClient = false;// ���� Ŭ���̾�Ʈ
#endif //SERV_STEAM
#ifdef SERV_CHANNELING_AERIA
		m_bAeriaClient = false;
#endif //SERV_CHANNELING_AERIA
	}
};

DECL_PACKET( ECH_VERIFY_ACCOUNT_ACK )
{
    int                                 m_iOK;
    UidType                             m_iUserUID;
    std::wstring                        m_wstrPassport;
#ifdef SERV_GLOBAL_AUTH
	int									m_iChannelingCode;
#endif //SERV_GLOBAL_AUTH

//���� �ð� ���� ������ �ٲٱ�
#ifndef SERV_SERVER_TIME_GET
#ifdef SERV_MASSFILE_MAPPING_FUNCTION
	std::wstring                        m_wstrCurrentTime;
#endif SERV_MASSFILE_MAPPING_FUNCTION
#endif SERV_SERVER_TIME_GET

	//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	bool								m_bUseVerify;
	int									m_iChannelRandomKey;
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}
	//{{ 2013.03.05 ��ȿ��		KOG_OTP ���� �� �籸��
#ifdef SERV_KOG_OTP_VERIFY
	std::wstring                        m_wstrUserID;
#endif SERV_KOG_OTP_VERIFY
	//}}
	//{{ 2010. 12. 02 ��ȿ��	ĳ���� ���� ��, �α��� ���� ������ ���� ���ڿ�
#ifdef SERV_PURCHASE_TOKEN
	std::wstring						m_wstrPurchaseTok;
#endif SERV_PURCHASE_TOKEN
	//}}

#ifdef SERV_LOGIN_TOU
	std::string							m_strAgreementURL;
#endif //SERV_LOGIN_TOU

#ifdef SERV_COUNTRY_TH
	std::wstring						m_wstrSocketID;
#endif //SERV_COUNTRY_TH

#ifdef SERV_SOUTH_AMERICA
	int									m_iServerGourpID; 
#endif // SERV_SOUTH_AMERICA

	KECH_VERIFY_ACCOUNT_ACK()
	{
#ifdef SERV_GLOBAL_AUTH
		m_iChannelingCode = 0;
#endif //SERV_GLOBAL_AUTH

		//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
		m_bUseVerify = false;
		m_iChannelRandomKey = 0;
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
		//}}
		//���� �ð� ���� ������ �ٲٱ�
#ifndef SERV_SERVER_TIME_GET
#ifdef SERV_MASSFILE_MAPPING_FUNCTION
		m_wstrCurrentTime = L"";
#endif SERV_MASSFILE_MAPPING_FUNCTION
#endif SERV_SERVER_TIME_GET

		//{{ 2013.03.05 ��ȿ��		KOG_OTP ���� �� �籸��
#ifdef SERV_KOG_OTP_VERIFY
		m_wstrUserID		= L"";
#endif SERV_KOG_OTP_VERIFY
		//}}
		//{{ 2010. 12. 02 ��ȿ��	ĳ���� ���� ��, �α��� ���� ������ ���� ���ڿ�
#ifdef SERV_PURCHASE_TOKEN
		m_wstrPurchaseTok	= L"";
#endif SERV_PURCHASE_TOKEN
		//}}
#ifdef SERV_LOGIN_TOU
		m_strAgreementURL	= "";
#endif //SERV_LOGIN_TOU

#ifdef SERV_COUNTRY_TH
		m_wstrSocketID		= L"";
#endif //SERV_COUNTRY_TH

#ifdef SERV_SOUTH_AMERICA
		m_iServerGourpID	= 0; 
#endif // SERV_SOUTH_AMERICA
	}
};

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 9. 17  ������	���
#ifdef GUILD_TEST

DECL_PACKET( EGS_CREATE_GUILD_REQ )
{
    UidType								m_iItemUID;
	std::wstring						m_wstrGuildName;
	std::wstring						m_wstrGuildMessage;
};

DECL_PACKET( EGS_CREATE_GUILD_ACK )
{
	int									m_iOK;
	KGuildInfo							m_kGuildInfo;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};

DECL_PACKET( EGS_GET_GUILD_USER_LIST_REQ )
{
	enum USER_LIST_TYPE
	{
		ULT_POSITION = 0,
		ULT_GRADE,
		ULT_MESSAGE,
	};

	UINT								m_uiViewPage;
	char								m_cGuildUserListType;
};

DECL_PACKET( EGS_GET_GUILD_USER_LIST_ACK )
{
	int										m_iOK;
	UINT									m_uiTotalPage;
	UINT									m_uiViewPage;
	char									m_cGuildUserListType;
	int										m_iLoginedMemberCount;

	// GuildUserListType�� ���� ��� vector�� ������ ����������� ������
	std::vector< KGuildMemberInfo >			m_vecMemberPositionGradeList;
	std::vector< KGuildMemberMessageInfo >	m_vecMemberMessageList;
};

DECL_PACKET( EGS_GUILD_MEMBER_INFO_NOT )
{	
	KGuildMemberInfo					m_kMemberInfo;
};

DECL_PACKET( EGS_GUILD_MESSAGE_NOT )
{
	enum MESSAGE_TYPE
	{
        MT_JOIN_MEMBER = 0,			// ��忡 ���� ������ ��������
		MT_CHANGE_MEMBER_GRADE,		// ����� �ٲ� ���� ����
		MT_KICK_MEMBER,				// Ż���� ���� ����
		MT_CONNECT_MEMBER,			// �α����� ���� ����
		MT_DISCONNECT_MEMBER,		// �α׾ƿ��� ���� ����
		MT_CHANGE_MEMBER_MESSAGE,	// ���� �λ縻 ������ ���� ����
		MT_UPDATE_MEMBER_INFO,		// ���� ���°� ����� ���� ���� ( ������, ����, �����̵�, ���̵� )
		MT_KICK_BY_MASTER,			// ������� ���� ����
		MT_GUILD_MESSAGE,			// ��� �޽���
		MT_GUILD_EXP,				// ��� ����ġ
		MT_GUILD_LEVEL_UP,			// ��� ���� ��
		MT_EXPAND_MAX_MEMBER,		// �ִ��ο�Ȯ��
		MT_INSERT_GUILD_CSPOINT,	// ��� ��ų ����Ʈ ȹ��
		MT_GUILD_AD_PERIOD_INFO,	// ��� ���� �Ⱓ �˸�
		MT_REG_GUILD_AD,			// ��� ���� ��� ��� �˸�
		
		//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
		MT_CHANGE_GUILD_NAME,		// ��� �̸� ���� ��� �˸�
#endif SERV_GUILD_CHANGE_NAME
		//}}
	};

	char								m_cMessageType;
	UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;
	std::wstring						m_wstrMessage;
	int									m_iMessageData;  // ���뵥����
	int									m_iMessageData2; // ���뵥����
};

DECL_PACKET( EGS_INVITE_GUILD_REQ )
{
	std::wstring						m_wstrNickName;
	UidType								m_iUnitUID;

	KEGS_INVITE_GUILD_REQ()
	{
		m_iUnitUID = 0;
	}
};

DECL_PACKET( EGS_INVITE_GUILD_ACK )
{
	int									m_iOK;
	std::wstring						m_wstrReceiverNickName;
};

DECL_PACKET( EGS_INVITE_GUILD_NOT )
{
	KGuildInfo							m_kGuildInfo;
	std::wstring						m_wstrSenderNickName;
	//{{ 2010. 01. 18  ������	���Խ���
	bool								m_bAutoJoinGuild;
	//}}

	KEGS_INVITE_GUILD_NOT()
	{
		m_bAutoJoinGuild = false;
	}
};

DECL_PACKET( EGS_INVITE_GUILD_REPLY_REQ )
{
	int									m_iGuildUID;
	std::wstring						m_wstrSenderNickName;
    int									m_iReason;
};

typedef KPacketOK		KEGS_INVITE_GUILD_REPLY_ACK;

DECL_PACKET( EGS_INVITE_GUILD_REPLY_DENY_NOT )
{
	std::wstring						m_wstrNickName;
	int									m_iReason;
};

typedef KEGS_GUILD_INFO_NOT		KEGS_JOIN_GUILD_SUCCESS_NOT;

DECL_PACKET( EGS_CHANGE_GUILD_MEMBER_GRADE_REQ )
{
    UidType								m_iUnitUID;
	u_char								m_ucMemberShipGrade;
};

DECL_PACKET( EGS_CHANGE_GUILD_MEMBER_GRADE_ACK )
{
    int									m_iOK;
	std::wstring						m_wstrTargetNickName;
	u_char								m_ucMemberShipGrade;
};

DECL_PACKET( EGS_CHANGE_GUILD_MESSAGE_REQ )
{
	std::wstring						m_wstrMessage;
};

typedef KPacketOK		KEGS_CHANGE_GUILD_MESSAGE_ACK;

DECL_PACKET( EGS_KICK_GUILD_MEMBER_REQ )
{
	UidType								m_iUnitUID;
};

DECL_PACKET( EGS_KICK_GUILD_MEMBER_ACK )
{
    int									m_iOK;
	bool								m_bKickBySysop;
};

DECL_PACKET( EGS_CHANGE_GUILD_MEMBER_MESSAGE_REQ )
{
	UidType								m_iUnitUID;
	std::wstring						m_wstrMessage;
};

DECL_PACKET( EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK )
{
    int									m_iOK;
	bool								m_bDelete;
};

typedef KPacketOK		KEGS_DISBAND_GUILD_ACK;

//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
DECL_PACKET( EGS_CHANGE_GUILD_NAME_CHECK_REQ )
{
	std::wstring						m_wstrGuildName;
};

typedef KPacketOK KEGS_CHANGE_GUILD_NAME_CHECK_ACK;

DECL_PACKET( EGS_CHANGE_GUILD_NAME_REQ )
{
	int									m_iOK;
	unsigned long                       m_ulOrderNo;
	unsigned long                       m_ulProductNo;
	int									m_iQuantity;
};

DECL_PACKET( EGS_CHANGE_GUILD_NAME_ACK )
{
	int									m_iOK;
	KGuildInfo							m_kGuildInfo;
};

#endif SERV_GUILD_CHANGE_NAME
//}}

#endif GUILD_TEST
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2009. 9. 22  ������	����ĳ��
DECL_PACKET( EGS_CHANGE_JOB_CASH_ITEM_NOT )
{
    KUnitInfo							m_kUnitInfo;
};

DECL_PACKET( EGS_CHANGE_USER_INFO_NOT )
{
    UidType								m_iUnitUID;
	char								m_cUnitClass;
	//{{ 2009. 10. 9  ������	���
#ifdef GUILD_TEST
	std::wstring						m_wstrGuildName;
#endif GUILD_TEST
	//}}	
};
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 11. 23  ������	��彺ų
#ifdef GUILD_SKILL_TEST

DECL_PACKET( EGS_GET_GUILD_SKILL_REQ )
{
    int									m_iGuildSkillID;
};

typedef KPacketOK		KEGS_GET_GUILD_SKILL_ACK;

DECL_PACKET( EGS_GET_GUILD_SKILL_NOT )
{
    int									m_iGuildSkillID;
	int									m_iGuildSkillLv;
	int									m_iGuildSkillCSPoint;
	int									m_iGuildSPoint;
	int									m_iGuildCSPoint;
};

DECL_PACKET( EGS_RESET_GUILD_SKILL_REQ )
{
    int									m_iGuildSkillID;
};

DECL_PACKET( EGS_RESET_GUILD_SKILL_ACK )
{
    int									m_iOK;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};

DECL_PACKET( EGS_RESET_GUILD_SKILL_NOT )
{
    int									m_iDelGuildSkillID;
	int									m_iRestoreGuildSPoint;
	int									m_iRestoreGuildCSPoint;
};

DECL_PACKET( EGS_INIT_GUILD_SKILL_REQ )
{
	UidType								m_iItemUID;
};

DECL_PACKET( EGS_INIT_GUILD_SKILL_ACK )
{
    int									m_iOK;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};

DECL_PACKET( EGS_INIT_GUILD_SKILL_NOT )
{
    int									m_iGuildSPoint;
	int									m_iGuildCSPoint;
};

DECL_PACKET( EGS_EXPIRE_GUILD_CASH_SKILL_POINT_NOT )
{
	int									m_iGuildSPoint;
	int									m_iGuildCSPoint;
    std::vector< KGuildSkillData >		m_vecGuildSkillData;
};

DECL_PACKET( EGS_CHANGE_SKILL_INFO_IN_ROOM_NOT )
{
    UidType								m_iUnitUID;
	u_char								m_ucMemberShipGrade;
	KStat								m_kGameStat;
	KUnitSkillData						m_UnitSkillData;
};

DECL_PACKET( EGS_ADMIN_GET_GUILD_SKILL_POINT_REQ )
{
	int									m_iGuildSPoint;
};

DECL_PACKET( EGS_ADMIN_GET_GUILD_SKILL_POINT_ACK )
{
	int									m_iOK;
	int									m_iCurGuildSPoint;
};

#endif GUILD_SKILL_TEST
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2009. 12. 4  ������	���ӽð��̺�Ʈ�ý��۰���
DECL_PACKET( EGS_TIME_EVENT_INFO_NOT )
{
	enum TIME_EVENT_NOTIFY_TYPE
	{
		TENT_INIT = 0,
		TENT_BEGIN,
		TENT_END,
	};
	
	char								m_cNotifyType;
	std::vector< KTimeEventSimpleInfo > m_vecTimeEvent;

	KEGS_TIME_EVENT_INFO_NOT()
	{
		m_cNotifyType = TENT_INIT;
	}
};
//}}

#ifdef DUNGEON_ITEM
DECL_PACKET( EGS_CREATE_PVP_ITEM_REQ )
{
	int		m_iPositionIndex;
};

typedef KPacketOK KEGS_CREATE_PVP_ITEM_ACK;

DECL_PACKET( EGS_CREATE_PVP_ITEM_NOT )
{
	int				m_iPositionIndex;
	KDropItemData	m_kDropItemData;
};

DECL_PACKET( EGS_CHANGE_PVP_ITEM_NOT )
{
	int		m_iDropItemUID;
	int		m_iItemID;
};

typedef KPacketOK	KEGS_CHANGE_PVP_ITEMMODE_ACK;
typedef bool		KEGS_CHANGE_PVP_ITEMMODE_NOT;
#endif DUNGEON_ITEM

//{{ 2009. 12. 8  ������	ũ���������̺�Ʈ
DECL_PACKET( EGS_CHECK_TIME_EVENT_COMPLETE_ACK )
{
    int									m_iOK;
	int									m_iRewardCount;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};
//}}







#ifdef SERVER_PVP_BASE_DEFENCE_TEST

	DECL_PACKET( EGS_CHANGE_PVP_GAME_OPTION_REQ )
	{
		bool m_bBaseDefence;
		bool m_bBossCombat;
	};

	typedef KPacketOK KEGS_CHANGE_PVP_GAME_OPTION_ACK;

	//DECL_PACKET( EGS_CHANGE_PVP_GAME_OPTION_ACK )
	//{
	//	int m_iOK;
	//};

	typedef KEGS_CHANGE_PVP_GAME_OPTION_REQ KEGS_CHANGE_PVP_GAME_OPTION_NOT;


#endif SERVER_PVP_BASE_DEFENCE_TEST


//{{ 2009. 12. 29  ������	NPCKILLALL��ŷ����	
DECL_PACKET( EGS_DUNGEON_KILLALLNPC_CHECK_ACK )
{
	bool								m_bResult;
};
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 12. 31  ������	��� ����
#ifdef SERV_GUILD_AD

DECL_PACKET( EGS_GET_GUILD_AD_LIST_REQ )
{
	enum AD_SORT_TYPE
	{
		AST_REG_DATE = 0,
		AST_NEW_GUILD,
		AST_HOT_GUILD,
		AST_MEMBER_COUNT,
		AST_GUILD_LEVEL,

		AST_MAX,
	};

	char								m_cSortType;
	u_int								m_uiViewPage;
};

DECL_PACKET( EGS_GET_GUILD_AD_LIST_ACK )
{	
	char									m_cSortType;
	u_int									m_uiTotalPage;
	u_int									m_uiViewPage;
	std::vector< KGuildAdInfo >				m_vecGuildAdList;
	bool									m_bCanShowApplyList;
	bool									m_bRegMyGuildAd;
};

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_GET_GUILD_SKILL_IN_BOARD_REQ )
{
    int										m_iGuildUID;
};

DECL_PACKET( EGS_GET_GUILD_SKILL_IN_BOARD_ACK )
{
	int										m_iOK;
    KGuildSkillInfo							m_kGuildSkillInfo;
};

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_REGISTRATION_GUILD_AD_REQ )
{
    short									m_sPeriod;
	std::wstring							m_wstrAdMessage;
};

DECL_PACKET( EGS_REGISTRATION_GUILD_AD_ACK )
{
	int										m_iOK;
	int										m_iED;
	short									m_sPeriod;
	std::wstring							m_wstrAdMessage;
};

typedef KEGS_REGISTRATION_GUILD_AD_REQ		KEGS_MODIFY_REG_GUILD_AD_REQ;
typedef KEGS_REGISTRATION_GUILD_AD_ACK		KEGS_MODIFY_REG_GUILD_AD_ACK;

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_APPLY_JOIN_GUILD_REQ )
{
    int										m_iGuildUID;
	std::wstring							m_wstrMessage;
	bool									m_bOldApplyDelete;
};

typedef KPacketOK		KEGS_APPLY_JOIN_GUILD_ACK;

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_GET_APPLY_JOIN_GUILD_LIST_REQ )
{
	u_int									m_uiViewPage;
};

DECL_PACKET( EGS_GET_APPLY_JOIN_GUILD_LIST_ACK )
{
    int										m_iOK;
	std::wstring							m_wstrGuildName;
	u_int									m_uiTotalPage;
	u_int									m_uiViewPage;
	std::vector< KApplyJoinGuildInfo >		m_vecApplyJoinGuildList;
};

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_ACCEPT_JOIN_GUILD_REQ )
{
    UidType									m_iUnitUID;
};

DECL_PACKET( EGS_ACCEPT_JOIN_GUILD_ACK )
{
	int										m_iOK;
	std::wstring							m_wstrReceiverNickName;
};

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_DELETE_APPLY_JOIN_GUILD_REQ )
{
	UidType									m_iUnitUID;
};

typedef KPacketOK		KEGS_DELETE_APPLY_JOIN_GUILD_ACK;

#endif SERV_GUILD_AD
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2010. 01. 07  ������	��õ�θ���Ʈ
DECL_PACKET( EGS_GET_RECOMMEND_USER_LIST_REQ )
{
	u_int									m_uiViewPage;
};

DECL_PACKET( EGS_GET_RECOMMEND_USER_LIST_ACK )
{
	int										m_iOK;
	u_int									m_uiTotalPage;
	u_int									m_uiViewPage;
	std::vector< KRecommendUserInfo >		m_vecRecommendUserList;
};

DECL_PACKET( EGS_RECOMMEND_USER_INFO_NOT )
{
	UidType									m_iRecommendUnitUID;
    char									m_cType;
	int										m_iData;

    KEGS_RECOMMEND_USER_INFO_NOT()
	{
		m_iRecommendUnitUID = 0;
		m_cType				= 0;
		m_iData				= 0;
	}
};
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2010. 01. 29  ������	PC�� ���ȭ
DECL_PACKET( EGS_PC_BANG_AUTH_RESULT_NOT )
{
	enum PC_BANG_RESULT_TYPE
	{
		PBRT_FORBIDDEN = 0, // �����̾� ���� PC���� �ƴѰ��.
		PBRT_ALLOWED_1,		// �ؽ� ���� ������
		PBRT_ALLOWED_2,		// �ؽ� ���� ������
		PBRT_TERMINATE_1,	// PC�� ���� �Ⱓ ����
		PBRT_TERMINATE_2,	// PC�� ���� �ð� ����
		PBRT_TERMINATE_3,	// IP�� ��� ������ �� �ʰ�
		PBRT_MESSAGE,		// PC�� ������ �ܿ� �ð� �޽���

		// �������� ������ ���� �޽���
		PBRT_RECONNECT,		// �������� ���������� ���� �޽���
		//{{ 2012. 02. 14    ��μ�   �ؽ� �Ƚ� �α��� - ��ǥ ��ġ��� �α���
//#ifdef SERV_NEXON_PEACE_OF_MIND_LOGIN
		PBRT_PEACE_OF_MIND_LOGIN,		// �Ƚ� �α���
//#endif SERV_NEXON_PEACE_OF_MIND_LOGIN
		//}}
		//{{ 2012. 06. 13	��μ�       ������ �˴ٿ���
//#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
		PBRT_SELECTIVE_SHUTDOWN_SYSTEM,		// ������ �˴ٿ��� - ���� �̿� ���� ��å
//#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
		//}}
	};

	char									m_cType;
	u_int									m_uiData;
	
	KEGS_PC_BANG_AUTH_RESULT_NOT()
	{
		m_cType	 = 0;
		m_uiData = 0;;
	}
};
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2010. 02. 06  ������	���� �̺�Ʈ ����Ʈ
#ifdef SERV_DAY_QUEST

DECL_PACKET( EGS_EVENT_QUEST_INFO_NOT )
{
	enum EVENT_QUEST_INFO_TYPE
	{
        EQIT_BEGIN = 0,
		EQIT_END,
	};

	char									m_cType;
	std::vector< int >						m_vecQuestID;
};

#endif SERV_DAY_QUEST
//}}


//{{ 2010. 02. 12  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST

DECL_PACKET( EGS_HACKING_TOOL_LIST_NOT )
{
	int										m_iRandomKey;
	std::vector< KHackingToolInfo >			m_vecHackingToolList;
};

#endif SERV_HACKING_TOOL_LIST
//}}


//{{ 2010. 02. 23  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT

typedef KPacketOK		KEGS_ATTENDANCE_CHECK_ACK;

#endif SERV_WEB_POINT_EVENT
//}}

#ifdef DEF_TRADE_BOARD
DECL_PACKET( EGS_SEARCH_TRADE_BOARD_REQ )
{
	std::wstring	m_wstrFinder;	// ���ڿ� �˻� Ű

	char	m_cSlotCategory;	// �ʼ� ���� �׸�
	char	m_cEqipPosition;	// ���� ��ġ
	char	m_cUnitType;		// ���� Ÿ��
	char	m_cUnitClass;		// ���� Ŭ����

	char	m_cMinLevel;		// �ּ� ���� ����
	char	m_cMaxLevel;		// �ִ� ���� ����

	//char	m_cItemGrade;		// ������ ���
	std::vector<char>	m_vecItemGrade;
    bool    m_bStrictSearchOption; // ��ġ�ϴ� Ű���� �˻�
};

DECL_PACKET( EGS_SEARCH_TRADE_BOARD_ACK )
{
	int			m_iOK;

	std::vector<KTradeBoardItemInfo>	m_vecItemInfo;	// �˻��� ������ ����Ʈ
};
#endif DEF_TRADE_BOARD


//{{ 2010. 03. 22  ������	����� ��Ʈ
#ifdef SERV_SKILL_NOTE

DECL_PACKET( EGS_EXPAND_SKILL_NOTE_PAGE_NUM_NOT )
{
    char								m_cExpandedPageNum;
};

DECL_PACKET( EGS_REG_SKILL_NOTE_MEMO_REQ )
{
	char								m_cSkillNotePageNum;
    UidType								m_iItemUID;	
};

DECL_PACKET( EGS_REG_SKILL_NOTE_MEMO_ACK )
{
    int									m_iOK;
	char								m_cSkillNotePageNum;
	int									m_iMemoID;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
	//{{ 2012. 10. 29	�ڼ���	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	std::vector<int>					m_vecSkillNote;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	KEGS_REG_SKILL_NOTE_MEMO_ACK()
		: m_iOK( 0 )
	{
    }
};

#endif SERV_SKILL_NOTE
//}}


//{{ 2010. 05. 06  ������	��д��� ����
DECL_PACKET( EGS_DUNGEON_EFFECT_REMAINING_TIME_NOT )
{
    float								m_fRemainingTime;
};

DECL_PACKET( EGS_DUNGEON_EFFECT_TIME_OUT_NOT )
{
	enum DUNGEON_DFFECT_TYPE
	{
		DDT_NONE = 0,
		DDT_POISON,
	};

    char								m_cType;

	KEGS_DUNGEON_EFFECT_TIME_OUT_NOT()
	{
		m_cType = DDT_NONE;
	}
};
//}}

//{{ 2010. 05. 06  ������	���� �˾�
DECL_PACKET( EGS_ERROR_POPUP_NOT )
{
    int									m_iOK;
	int									m_iErrorData1;
	int									m_iErrorData2;
	//{{ ���� ����� ���� �˾� ����
#ifdef SERV_CHANGE_ERROR_POPUP
	bool								m_bPartyMember;

	KEGS_ERROR_POPUP_NOT()
	{
		m_iOK							= 0;
		m_iErrorData1					= 0;
		m_iErrorData2					= 0;
		m_bPartyMember					= false;
	}
#endif SERV_CHANGE_ERROR_POPUP
	//}}
};
//}}


//{{ 2010. 05. 20  ������	��ũ��Ʈ �Ľ�
#ifdef SERV_CLIENT_SCRIPT

DECL_PACKET( EGS_GET_CLIENT_SCRIPT_REQ )
{
	std::wstring						m_wstrScriptName;
};

DECL_PACKET( EGS_GET_CLIENT_SCRIPT_ACK )
{
    int									m_iOK;
	std::wstring						m_wstrScriptString;
};

#endif SERV_CLIENT_SCRIPT
//}}


//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//}}

//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//{{ 2010. 06. 17  ������	������ ����
#ifdef SERV_PROTECT_AUTO_HACK
DECL_PACKET( EGS_SERVER_CHECK_HACK_USER_REQ )
{
	enum HACKING_CHECK_TYPE
	{
		HCT_NONE = 0,
		HCT_DUNGEON_NPC_UNIT_DIE,
		HCT_DUNGEON_GET_ITEM,
		HCT_DUNGEON_CANT_KILL_NPC_FIND,
		HCT_DUNGEON_CANT_KILL_NPC_KICK,
		HCT_DUNGEON_PLAY_START,
		HCT_DUNGEON_CLEAR_RESULT,
		HCT_DUNGEON_STAGE_LOADING,
		//{{ 2011. 01. 26	������	���� Ŭ���� Ÿ�� ������ üũ
		//#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
		HCT_DUNGEON_CLEAR_TIME,
		//#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
		//}}
		//{{ 2011. 04. 26	������	deserialize ���� ����
		//#ifdef SERV_DESERIALIZE_FAIL_DISCONNECT
		HCT_DESERIALIZE_FAIL,
		//#endif SERV_DESERIALIZE_FAIL_DISCONNECT
		//}}
		//{{ 2011. 06. 07    ��μ�    ���� �� �ŷ� ���� �ǽð� ���� ���
		//#ifdef SERV_MONITORING_LETTER_AND_TRADE_REALTIME_BLOCK
		HCT_MONITORING_LETTER_AND_TRADE,
		//#endif SERV_MONITORING_LETTER_AND_TRADE_REALTIME_BLOCK
		//}} 
		//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
		//#ifdef SERV_SECOND_SECURITY
		HCT_SECOND_SECURITY_NOT_AUTH,	// 2�� �����е� ������
		//#endif SERV_SECOND_SECURITY
		//}}
		HCT_MONITORING,				// �ӽ÷� ���캸��    // 2011-06-28
		//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
		//#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
		HCT_CHECK_MACHINE_ID,
		//#endif SERV_MACHINE_ID_DUPLICATE_CHECK
		//}}
		//{{ 2011. 09. 07  ��μ�	���� �������� DIE NPC �ð� üũ ���
		//#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
		HCT_CHECK_NPC_DIE_TIME,
		//#endif SERV_SUB_STAGE_NPC_DIE_CHECK
		//}}
		//{{ 2011. 09. 20	��μ�	������ 5000�� ����� �ڵ� ���� ��
		//#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM_AUTO_BLOCK
		HCT_CHECK_GET_ITEM_COUNT,							//������ �ִ� ���� ���� �ʰ�(5000��)
		//#endif SERV_AUTO_HACK_CHECK_GET_ITEM_AUTO_BLOCK
		//}}
		//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
		//#ifdef SERV_ED_MONITORING_IN_GAME
		HCT_ED_MONITORING_SEND_MAIL,
		HCT_ED_MONITORING_ACCOUNT_BLOCK,
		HCT_ED_MONITORING_MACHINE_ID_BLOCK,
		//#endif SERV_ED_MONITORING_IN_GAME
		//}}
		//{{ 2011. 09. 23	������	�α׿��� ���� EDüũ
		//#ifdef SERV_LOGOUT_ED_CHECK
		HCT_LOGOUT_ED_CHECK,
		//#endif SERV_LOGOUT_ED_CHECK
		//}}
		//{{ 2011.11.02     ��μ�    �������� SN �߱� �� ��ȿ�� �˻�
		//#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
		HCT_CHANGE_CHECK_SUM_SEND_MAIL,			// SN üũ�� ����
		HCT_CHANGE_SERVER_SN_SEND_MAIL,			// SN ����
		HCT_CHANGE_MACHINE_ID_SEND_MAIL,		// MID ����
		HCT_CHANGE_SN_AND_MID_SEND_MAIL,		// SN, MID ����
		//#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
		//}}
		//{{  2012. 1. 16	��μ�		���Ӽ������� Server SN �˻�
		//#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		HCT_CHANGE_SERVER_SN_IN_GAMESERVER_SEND_MAIL,			// ���Ӽ������� SN üũ�� ����
		//#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		//}}
		//{{ 2012. 01. 27  ��μ�	��ŷ ���μ��� �˻� ���
		//#ifdef SERV_HACKING_DLL_CHECK
		HCT_CHECK_CLIENT_DLL,					// Ŭ���̾�Ʈ dll �˻� �� ����͸�����Ʈ dll ���
		//#endif SERV_HACKING_DLL_CHECK
		//}}
		//{{ 2012. 02. 06    ��μ�    ���� ���� UID �߱� ��� ����
		//#ifdef SERV_MAKE_MONSTER_UID_CHANGE
		HCT_VERIFY_DUNGEON_NPC_UID,				// ���� ���� ���� NPC UID �� Ŭ���̾�Ʈ�� ������ ������(��ŷ����� �ǽ�)
		//#endif SERV_MAKE_MONSTER_UID_CHANGE	
		//}}
		//{{ 2012. 03. 14	��μ�		��Ŷ ������ ī��Ʈ üũ
		//#ifdef SERV_CHECK_SEQUENCE_COUNT
		HCT_CHECK_SEQUENCE_COUNT,				
		//#endif SERV_CHECK_SEQUENCE_COUNT
		//}}
		//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
		//#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
		HCT_CHECK_GAME_VERSION,					// ���� ���� �� ����
		HCT_CHECK_OVERLAP_ACCOUNT,				// ���� ����
		HCT_CHECK_FAST_LEVEL_UP,				// ������ ������ 1�� �̸�
		HCT_CHECK_CLIENT_BLOCK_DLL,				// Ŭ���̾�Ʈ dll �˻� �� ��ϸ���Ʈ dll ���
		HCT_CHECK_BLOCK_MACHINE_ID,				// Machine ID ���
		HCT_KICK_BY_GM,							// ��ڿ� ���� ���� ���� ����
		HCT_CHANGE_SERVER_SN_IN_GAMESERVER_EMPTY, // ServerSN�� ��� ���� ���(���Ӽ���)
		//#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
		//}}

		//{{ 2012. 04. 02	��μ�		sn ����͸� ��� �и�
		//#ifdef SERV_SERIAL_NUMBER_MORNITORING
		HCT_SERVER_SN_MONITORING,					// ����͸��� ��ϵ� sn - 
		//#endif SERV_SERIAL_NUMBER_MORNITORING
		//}}
		//{{ 2012. 03. 29	��μ�		�ߺ� sn ���� ����
		//#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
		HCT_SERVER_SN_OVERLAP_CONNECT,				// sn �ߺ� ����
		//#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
		//}}
		//{{ 2012. 03. 27	��μ�		�ӽ� �ð��� �˻�����(50�� �̻� ���̳��� ����͸�)
		//#ifdef SERV_CHECK_MACHINE_LOCALTIME
		HCT_CHECK_MACHINE_LOCALTIME,					// �ӽ� �ð��� �˻� 
		//#endif SERV_CHECK_MACHINE_LOCALTIME
		//}}
		//{{ 2012. 03. 29	��μ�		x2.exe ��� ���� �˻�
		//#ifdef SERV_CHECK_X2_EXE_MODULE
		HCT_CHECK_X2_EXE_MODULE,						// x2.exe ��� ���� �˻� 
		//#endif SERV_CHECK_X2_EXE_MODULE
		//}}
		//{{ 2012. 05. 23	��μ�       �Ǹ� �Ұ� ������ �Ǹ� �õ� ��ŷ ������ �Ǵ�
		//#ifdef SERV_IMPOSSIBLE_SELL_ITEM
		HCT_IMPOSSIBLE_SELL_ITEM,						// �Ǹ� �Ұ� ������ �Ǹ� �õ� 
		//#endif SERV_IMPOSSIBLE_SELL_ITEM
		//}}
		//{{ ����� : [2012/5/24] //	��ϸ� �ð� ������ üũ
		//#ifdef	SERV_CHECK_HENIR_SPACE_DAMAGE
		HCT_CHECK_HENIR_SPACE_DAMAGE,					//	��ϸ� �ð����� ��� ������ �������� Ŭ���� ���� ���
		//#endif	SERV_CHECK_HENIR_SPACE_DAMAGE
		//}} ����� : [2012/5/24] //	��ϸ� �ð� ������ üũ
		//{{ 2012. 10. 12	������		2�� ���� ���� ����
		//#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX_COMPLETE
		HCT_CHECK_MINUS_ED,
		//#endif SERV_DUPLICATE_CONNECT_BUG_FIX_COMPLETE
		//}}
		//#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-07	// �ڼ���
		HCT_LANBUGOUT_DISCONNECT,
		//#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK
	};

	char								m_cHackingCheckType;

	KEGS_SERVER_CHECK_HACK_USER_REQ()
	{
		m_cHackingCheckType = HCT_NONE;
	}
};
DECL_PACKET( EGS_SERVER_CHECK_HACK_USER_ACK )
{
	char								m_cHackingCheckType;

	KEGS_SERVER_CHECK_HACK_USER_ACK()
	{
		m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_NONE;
	}
};
#endif SERV_PROTECT_AUTO_HACK
//}}
#else
//{{ 2010. 06. 17  ������	������ ����
#ifdef SERV_PROTECT_AUTO_HACK
DECL_PACKET( EGS_SERVER_CHECK_HACK_USER_NOT )
{
	enum HACKING_CHECK_TYPE
	{
		HCT_NONE = 0,
		HCT_DUNGEON_NPC_UNIT_DIE,
		HCT_DUNGEON_GET_ITEM,
		HCT_DUNGEON_CANT_KILL_NPC_FIND,
		HCT_DUNGEON_CANT_KILL_NPC_KICK,
		HCT_DUNGEON_PLAY_START,
		HCT_DUNGEON_CLEAR_RESULT,
		HCT_DUNGEON_STAGE_LOADING,
		//{{ 2011. 01. 26	������	���� Ŭ���� Ÿ�� ������ üũ
		//#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
		HCT_DUNGEON_CLEAR_TIME,
		//#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
		//}}
		//{{ 2011. 04. 26	������	deserialize ���� ����
		//#ifdef SERV_DESERIALIZE_FAIL_DISCONNECT
		HCT_DESERIALIZE_FAIL,
		//#endif SERV_DESERIALIZE_FAIL_DISCONNECT
		//}}
		//{{ 2011. 06. 07    ��μ�    ���� �� �ŷ� ���� �ǽð� ���� ���
		//#ifdef SERV_MONITORING_LETTER_AND_TRADE_REALTIME_BLOCK
		HCT_MONITORING_LETTER_AND_TRADE,
		//#endif SERV_MONITORING_LETTER_AND_TRADE_REALTIME_BLOCK
		//}} 
		//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
		//#ifdef SERV_SECOND_SECURITY
		HCT_SECOND_SECURITY_NOT_AUTH,	// 2�� �����е� ������
		//#endif SERV_SECOND_SECURITY
		//}}
		HCT_MONITORING,				// �ӽ÷� ���캸��    // 2011-06-28
		//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
		//#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
		HCT_CHECK_MACHINE_ID,
		//#endif SERV_MACHINE_ID_DUPLICATE_CHECK
		//}}
		//{{ 2011. 09. 07  ��μ�	���� �������� DIE NPC �ð� üũ ���
		//#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
		HCT_CHECK_NPC_DIE_TIME,
		//#endif SERV_SUB_STAGE_NPC_DIE_CHECK
		//}}
		//{{ 2011. 09. 20	��μ�	������ 5000�� ����� �ڵ� ���� ��
		//#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM_AUTO_BLOCK
		HCT_CHECK_GET_ITEM_COUNT,							//������ �ִ� ���� ���� �ʰ�(5000��)
		//#endif SERV_AUTO_HACK_CHECK_GET_ITEM_AUTO_BLOCK
		//}}
		//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
		//#ifdef SERV_ED_MONITORING_IN_GAME
		HCT_ED_MONITORING_SEND_MAIL,
		HCT_ED_MONITORING_ACCOUNT_BLOCK,
		HCT_ED_MONITORING_MACHINE_ID_BLOCK,
		//#endif SERV_ED_MONITORING_IN_GAME
		//}}
		//{{ 2011. 09. 23	������	�α׿��� ���� EDüũ
		//#ifdef SERV_LOGOUT_ED_CHECK
		HCT_LOGOUT_ED_CHECK,
		//#endif SERV_LOGOUT_ED_CHECK
		//}}
		//{{ 2011.11.02     ��μ�    �������� SN �߱� �� ��ȿ�� �˻�
		//#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
		HCT_CHANGE_CHECK_SUM_SEND_MAIL,			// SN üũ�� ����
		HCT_CHANGE_SERVER_SN_SEND_MAIL,			// SN ����
		HCT_CHANGE_MACHINE_ID_SEND_MAIL,		// MID ����
		HCT_CHANGE_SN_AND_MID_SEND_MAIL,		// SN, MID ����
		//#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
		//}}
		//{{  2012. 1. 16	��μ�		���Ӽ������� Server SN �˻�
		//#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		HCT_CHANGE_SERVER_SN_IN_GAMESERVER_SEND_MAIL,			// ���Ӽ������� SN üũ�� ����
		//#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		//}}
		//{{ 2012. 01. 27  ��μ�	��ŷ ���μ��� �˻� ���
		//#ifdef SERV_HACKING_DLL_CHECK
		HCT_CHECK_CLIENT_DLL,					// Ŭ���̾�Ʈ dll �˻� �� ����͸�����Ʈ dll ���
		//#endif SERV_HACKING_DLL_CHECK
		//}}
		//{{ 2012. 02. 06    ��μ�    ���� ���� UID �߱� ��� ����
		//#ifdef SERV_MAKE_MONSTER_UID_CHANGE
		HCT_VERIFY_DUNGEON_NPC_UID,				// ���� ���� ���� NPC UID �� Ŭ���̾�Ʈ�� ������ ������(��ŷ����� �ǽ�)
		//#endif SERV_MAKE_MONSTER_UID_CHANGE	
		//}}
		//{{ 2012. 03. 14	��μ�		��Ŷ ������ ī��Ʈ üũ
		//#ifdef SERV_CHECK_SEQUENCE_COUNT
		HCT_CHECK_SEQUENCE_COUNT,				
		//#endif SERV_CHECK_SEQUENCE_COUNT
		//}}
		//{{ 2012. 03. 20	��μ�		Game Server ���� ���� ������ ��ŷ ���� üũ
		//#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
		HCT_CHECK_GAME_VERSION,					// ���� ���� �� ����
		HCT_CHECK_OVERLAP_ACCOUNT,				// ���� ����
		HCT_CHECK_FAST_LEVEL_UP,				// ������ ������ 1�� �̸�
		HCT_CHECK_CLIENT_BLOCK_DLL,				// Ŭ���̾�Ʈ dll �˻� �� ��ϸ���Ʈ dll ���
		HCT_CHECK_BLOCK_MACHINE_ID,				// Machine ID ���
		HCT_KICK_BY_GM,							// ��ڿ� ���� ���� ���� ����
		HCT_CHANGE_SERVER_SN_IN_GAMESERVER_EMPTY, // ServerSN�� ��� ���� ���(���Ӽ���)
		//#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
		//}}

		//{{ 2012. 04. 02	��μ�		sn ����͸� ��� �и�
		//#ifdef SERV_SERIAL_NUMBER_MORNITORING
		HCT_SERVER_SN_MONITORING,					// ����͸��� ��ϵ� sn - 
		//#endif SERV_SERIAL_NUMBER_MORNITORING
		//}}
		//{{ 2012. 03. 29	��μ�		�ߺ� sn ���� ����
		//#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
		HCT_SERVER_SN_OVERLAP_CONNECT,				// sn �ߺ� ����
		//#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
		//}}
		//{{ 2012. 03. 27	��μ�		�ӽ� �ð��� �˻�����(50�� �̻� ���̳��� ����͸�)
		//#ifdef SERV_CHECK_MACHINE_LOCALTIME
		HCT_CHECK_MACHINE_LOCALTIME,					// �ӽ� �ð��� �˻� 
		//#endif SERV_CHECK_MACHINE_LOCALTIME
		//}}
		//{{ 2012. 03. 29	��μ�		x2.exe ��� ���� �˻�
		//#ifdef SERV_CHECK_X2_EXE_MODULE
		HCT_CHECK_X2_EXE_MODULE,						// x2.exe ��� ���� �˻� 
		//#endif SERV_CHECK_X2_EXE_MODULE
		//}}
		//{{ 2012. 05. 23	��μ�       �Ǹ� �Ұ� ������ �Ǹ� �õ� ��ŷ ������ �Ǵ�
		//#ifdef SERV_IMPOSSIBLE_SELL_ITEM
		HCT_IMPOSSIBLE_SELL_ITEM,						// �Ǹ� �Ұ� ������ �Ǹ� �õ� 
		//#endif SERV_IMPOSSIBLE_SELL_ITEM
		//}}
		//{{ ����� : [2012/5/24] //	��ϸ� �ð� ������ üũ
		//#ifdef	SERV_CHECK_HENIR_SPACE_DAMAGE
		HCT_CHECK_HENIR_SPACE_DAMAGE,					//	��ϸ� �ð����� ��� ������ �������� Ŭ���� ���� ���
		//#endif	SERV_CHECK_HENIR_SPACE_DAMAGE
		//}} ����� : [2012/5/24] //	��ϸ� �ð� ������ üũ
	};

	char								m_cHackingCheckType;

	KEGS_SERVER_CHECK_HACK_USER_NOT()
	{
		m_cHackingCheckType = HCT_NONE;
	}
};
#endif SERV_PROTECT_AUTO_HACK
//}}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2010. 7. 21  ������	�� �ý���
#ifdef SERV_PET_SYSTEM

DECL_PACKET( EGS_CREATE_PET_REQ )
{
	UidType								m_iItemUID;
	std::wstring						m_wstrPetName;
};

DECL_PACKET( EGS_CREATE_PET_ACK )
{
	int									m_iOK;
	KPetInfo							m_kCreatedPetInfo;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
};

DECL_PACKET( EGS_GET_PET_LIST_REQ )
{
	UINT								m_nViewPage;
};

DECL_PACKET( EGS_GET_PET_LIST_ACK )
{
	int									m_iOK;
	UINT								m_nTotalPage;
	UINT								m_nViewPage;
	std::vector< KPetInfo >				m_vecPetList;
};

DECL_PACKET( EGS_SUMMON_PET_REQ )
{
    UidType								m_iSummonPetUID;
};

DECL_PACKET( EGS_SUMMON_PET_ACK )
{
    int									m_iOK;
	KPetInfo							m_kSummonedPetInfo;
	std::vector< KInventoryItemInfo >	m_vecPetInventorySlotInfo; // �� �κ��丮 ������
};

DECL_PACKET( EGS_SUMMON_PET_NOT )
{
	UidType								m_iUnitUID;
	std::vector< KPetInfo >				m_vecPetInfo;
};

DECL_PACKET( EGS_COMMANDS_FOR_PETS_REQ )
{
	char								m_cActionType;
};

typedef KPacketOK		KEGS_COMMANDS_FOR_PETS_ACK;

DECL_PACKET( EGS_PET_ACTION_NOT )
{
	UidType								m_iUnitUID;
    char								m_cActionType;
};

DECL_PACKET( EGS_CHANGED_PET_SATIETY_NOT )
{
	UidType								m_iUnitUID;
	UidType								m_iPetUID;
    short								m_sCurrentSatiety;	
};

DECL_PACKET( EGS_CHANGED_PET_INTIMACY_NOT )
{
	UidType								m_iUnitUID;
	UidType								m_iPetUID;
	int									m_iCurrentIntimacy;	
};

DECL_PACKET( EGS_CHANGED_PET_SATIETY_INTIMACY_NOT )
{
	UidType								m_iUnitUID;
	UidType								m_iPetUID;
	short								m_sCurrentSatiety;
	int									m_iCurrentIntimacy;
};

DECL_PACKET( EGS_FEED_PETS_REQ )
{
	UidType								m_iItemUID;
};

DECL_PACKET( EGS_FEED_PETS_ACK )
{
    int									m_iOK;
	short								m_sSatiety;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
#ifdef SERV_EVENT_PET_INVENTORY
	//�̺�Ʈ �� ���� ��� ���� üũ ���� 
	bool								m_EventFoodEat;
	KEGS_FEED_PETS_ACK()
	{
		m_EventFoodEat = false;
	}
#endif SERV_EVENT_PET_INVENTORY
};

DECL_PACKET( EGS_FIRST_FEED_PETS_NOT )
{
    bool								m_bAutoFeed;
};

DECL_PACKET( EGS_SET_AUTO_FEED_PETS_REQ )
{
	bool								m_bAutoFeed;
};

DECL_PACKET( EGS_SET_AUTO_FEED_PETS_ACK )
{
	int									m_iOK;
	bool								m_bAutoFeed;
};

DECL_PACKET( EGS_AUTO_FEED_NOT )
{
	int									m_iFeedItemID;
	std::vector< KInventoryItemInfo >	m_vecPetInventorySlotInfo;
};

DECL_PACKET( EGS_PET_EVOLUTION_REQ )
{
	UidType								m_iPetUID;
};

DECL_PACKET( EGS_PET_EVOLUTION_ACK )
{
	int									m_iOK;
	KPetInfo							m_kEvolutionResultPetInfo;
};

DECL_PACKET( EGS_PET_EVOLUTION_NOT )
{
    UidType								m_iUnitUID;
	KPetInfo							m_kPetInfo;
};

DECL_PACKET( EGS_ADMIN_CHANGE_PET_INFO_NOT )
{
	enum PET_CHEAT_TYPE
	{
		PCT_EVOLUTION_STEP = 0,
		PCT_SATIETY,
		PCT_INTIMACY,
		PCT_EXTROVERSION,
		PCT_EMOTION,
	};

    int									m_iCheatType;
	int									m_iValue;
};

//{{ 2012. 02. 22	�ڼ���	�� �̸� �����
#ifdef SERV_PET_CHANGE_NAME
DECL_PACKET( EGS_CHANGE_PET_NAME_REQ )
{
	UidType								m_iItemUID;
	std::wstring						m_wstrPetName;
};

DECL_PACKET( EGS_CHANGE_PET_NAME_ACK )
{
	int									m_iOK;
	KPetInfo							m_kChangedPetInfo;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
};
#endif SERV_PET_CHANGE_NAME
//}}

#endif SERV_PET_SYSTEM
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2010. 8. 16	������	�Ⱓ ���� ������ �̺�Ʈ
#ifdef SERV_RESET_PERIOD_EVENT

DECL_PACKET( EGS_RESET_PERIOD_ITEM_NOT )
{
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};

#endif SERV_RESET_PERIOD_EVENT
//}}


//{{ 2010. 9. 29	������	ED ����͸� ġƮ
#ifdef SERV_ADMIN_SHOW_ED

DECL_PACKET( EGS_ADMIN_SHOW_ED_CHEAT_REQ )
{
	std::wstring						m_wstrNickName;
};

DECL_PACKET( EGS_ADMIN_SHOW_ED_CHEAT_ACK )
{
	int									m_iOK;
	std::wstring						m_wstrNickName;
	int									m_iED;
};

#endif SERV_ADMIN_SHOW_ED
//}}


//{{ 2010. 10. 13	������	DLL Manager
#ifdef SERV_DLL_MANAGER

DECL_PACKET( EGS_MODULE_INFO_UPDATE_NOT )
{
	std::vector< std::wstring >			m_vecModuleName;
};

#endif SERV_DLL_MANAGER
//}}


//{{ 2010. 11. 17	������	�ؽ� ���� û�� öȸ
#ifdef SERV_NX_BILLING_REFUND

DECL_PACKET( EGS_CASH_ITEM_REFUND_REQ )
{
	unsigned long                       m_ulOrderNo;
	unsigned long                       m_ulProductNo;
	int									m_iQuantity;
};

typedef KPacketOK		KEGS_CASH_ITEM_REFUND_ACK;

#endif SERV_NX_BILLING_REFUND
//}}

//{{ 2011. 01. 04  ��μ�	�Ӽ� ����
#ifdef SERV_ATTRIBUTE_CHARM

DECL_PACKET( EGS_ATTRIB_ATTACH_ITEM_REQ )
{
	UidType                             m_iDestItemUID;			// ����
	UidType                             m_iAttachItemUID;		// �Ӽ� ����
};

DECL_PACKET( EGS_ATTRIB_ATTACH_ITEM_ACK )
{
	int									m_iOK;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};

#endif SERV_ATTRIBUTE_CHARM
//}}

//{{ 2011. 01. 06  ��μ�  ��ų����ü���� üũ(�κ��丮-�Ⱓ��) ��� ����
#ifdef SERV_SKILL_SLOT_CHANGE_INVENTORY
DECL_PACKET( EGS_SKILL_SLOT_CHANGE_ITEM_NOT )
{
	char								m_cSkillSlotBExpirationState;
	std::wstring						m_wstrSkillSlotBEndDate;
};
#endif SERV_SKILL_SLOT_CHANGE_INVENTORY
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2011. 04. 12	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY

typedef KEGS_CREATE_PERSONAL_SHOP_ACK		KEGS_CREATE_PSHOP_AGENCY_ACK;

DECL_PACKET( EGS_CHANGE_PSHOP_AGENCY_EXPIRATION_NOT )
{
    char                    m_cShopType;
	std::wstring			m_wstrAgencyExpirationDate;
};

typedef KEGS_REG_PERSONAL_SHOP_ITEM_REQ		KEGS_REG_PSHOP_AGENCY_ITEM_REQ;
typedef KEGS_REG_PERSONAL_SHOP_ITEM_ACK		KEGS_REG_PSHOP_AGENCY_ITEM_ACK;

DECL_PACKET( EGS_JOIN_MY_PSHOP_AGENCY_REQ )
{
	enum JOIN_TYPE
	{
		JT_SHOW_MY_ITEM_LIST = 0,
		JT_FOR_PICK_UP,
	};

    char										m_cJoinType;

	KEGS_JOIN_MY_PSHOP_AGENCY_REQ()
	{
		m_cJoinType = JT_SHOW_MY_ITEM_LIST;
	}
};

DECL_PACKET( EGS_JOIN_MY_PSHOP_AGENCY_ACK )
{
	int											m_iOK;
	char										m_cJoinType;
	std::wstring								m_wstrPersonalShopName;
	std::wstring								m_wstrNickName;
	std::vector< KSellPersonalShopItemInfo >	m_vecSellItemInfo;
#ifdef SERV_UPGRADE_TRADE_SYSTEM
    int					                        m_iUsedItemID; // ���� ���׷��̵� ������(������)
#endif SERV_UPGRADE_TRADE_SYSTEM
	KEGS_JOIN_MY_PSHOP_AGENCY_ACK()
	{
		m_iOK = 0;
		m_cJoinType = 0;

#ifdef SERV_UPGRADE_TRADE_SYSTEM
        m_iUsedItemID = 0;
#endif SERV_UPGRADE_TRADE_SYSTEM
	}
};

typedef KEGS_LEAVE_PERSONAL_SHOP_ACK		KEGS_LEAVE_PSHOP_AGENCY_ACK;

DECL_PACKET( EGS_PSHOP_AGENCY_MESSAGE_NOT )
{
	enum PSHOP_AGENCY_MESSAGE_TYPE
	{
		PAMT_SELL_ITEM = 0,
		PAMT_SOLD_OUT,
		PAMT_EXPIRATION,
		PAMT_OVER_24_HOURS,
	};

	char										m_cMessageType;
	UidType										m_iHostUnitUID;
	int											m_iSellItemID;
	int											m_iSellQuantity;
	int											m_iSellED;

	KEGS_PSHOP_AGENCY_MESSAGE_NOT()
	{
		m_cMessageType = 0;
		m_iHostUnitUID = 0;
		m_iSellItemID = 0;
		m_iSellQuantity = 0;
		m_iSellED = 0;
	}
};

typedef KPacketOK							KEGS_STOP_SALE_PSHOP_AGENCY_ACK;

DECL_PACKET( EGS_PICK_UP_FROM_PSHOP_AGENCY_REQ )
{
	std::vector< UidType >						m_vecPickUpItemList;
};

DECL_PACKET( EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK )
{
	int											m_iOK;
	int											m_iED;
	std::vector< KSellPersonalShopItemInfo >	m_vecSellItemInfo;
	std::vector< KInventoryItemInfo >			m_vecInventorySlotInfo;
    bool                                        m_bRemainSellItem;
};

#endif SERV_PSHOP_AGENCY
//}}

DECL_PACKET( EGS_ADMIN_PSHOP_AGENCY_CHEAT_REQ )
{
	short								m_sPeriod;
};

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
DECL_PACKET( EGS_WORLD_MISSION_UPDATE_NOT )
{
	bool								m_bActive;
	int									m_iRemainTime;
	int									m_iWorldCrystalCount;
	//{{ 2013. 04. 15	�ڼ���	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	int									m_iWorldMaxCrystalCount;
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	int									m_iMissionStartRemainTime;

	KEGS_WORLD_MISSION_UPDATE_NOT()
		: m_bActive( false )
		, m_iRemainTime( 0 )
		, m_iWorldCrystalCount( 0 )
		//{{ 2013. 04. 15	�ڼ���	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
		, m_iWorldMaxCrystalCount( 0 )
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
		, m_iMissionStartRemainTime( 0 )
	{
	}
};

DECL_PACKET( EGS_CREATE_ATTRIB_NPC_NOT )
{
	std::vector< KAttribNpcUnitInfo >			m_vecAttribNPCUnit;
};

DECL_PACKET( EGS_UPDATE_WORLD_BUFF_NOT )
{
	u_short										m_usSuccessWorldBuff;
	u_short										m_usWorldBuffDurationTime;
};

#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
DECL_PACKET( EGS_SECOND_SECURITY_INFO_NOT )
{
	int											m_iOK;
	bool										m_bUseSecondPW;
};

typedef KEGS_SECOND_SECURITY_INFO_NOT KEGS_GET_SECOND_SECURITY_INFO_ACK;

DECL_PACKET( EGS_AUTH_SECOND_SECURITY_REQ )
{
	UidType										m_iUserUID;
	std::wstring								m_wstrSecondPW;
};

DECL_PACKET( EGS_AUTH_SECOND_SECURITY_ACK )
{
	int											m_iOK;
	int											m_iFailedCount;
};

DECL_PACKET( EGS_CREATE_SECOND_SECURITY_REQ )
{
	std::wstring								m_wstrSecondPW;
};
typedef KPacketOK							KEGS_CREATE_SECOND_SECURITY_ACK;

typedef KEGS_CREATE_SECOND_SECURITY_REQ		KEGS_DELETE_SECOND_SECURITY_REQ;
typedef KPacketOK							KEGS_DELETE_SECOND_SECURITY_ACK;

DECL_PACKET( EGS_DELETE_SECOND_SECURITY_VERIFY_REQ )
{
	std::wstring								m_wstrSecondPW;
    bool										m_bDelete;
};
typedef KPacketOK							KEGS_DELETE_SECOND_SECURITY_VERIFY_ACK;

DECL_PACKET( EGS_CHANGE_SECOND_SECURITY_PW_REQ )
{
	std::wstring								m_wstrOldSecondPW;
	std::wstring								m_wstrNewSecondPW;
};
typedef KPacketOK							KEGS_CHANGE_SECOND_SECURITY_PW_ACK;

typedef std::wstring						KEGS_COMPARE_SECOND_SECURITY_PW_REQ;
typedef KPacketOK							KEGS_COMPARE_SECOND_SECURITY_PW_ACK;
typedef std::wstring                        KEGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_REQ;
typedef KPacketOK                           KEGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_ACK;
#endif SERV_SECOND_SECURITY
//}}

//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
typedef bool			KEGS_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ;
typedef KPacketOK		KEGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK;
typedef bool			KEGS_PARTY_CHANGE_CHECK_MIN_LEVEL_NOT;
#endif SERV_DUNGEON_DIFFICULTY_REVISION
//}}

//{{ 2011. 05. 23  ��μ�	ť�ǿ� ���� ģ�е� ����
#ifdef SERV_ADD_KEWPIEL_JELLY
DECL_PACKET( EGS_EAT_SPECIAL_FEED_NOT )
{
	UidType				m_iUnitUID;
	UidType				m_iPetUID;
	short				m_sPercent;
	int					m_iCurrentIntimacy;
};
#endif SERV_ADD_KEWPIEL_JELLY
//}}

//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
typedef bool			KEGS_COME_BACK_USER_NOT;

DECL_PACKET( EGS_COME_BACK_BUFF_NOT )
{
	bool						m_bIsFinish;
	__int64						m_iComeBackBuffEnd;
};
#endif SERV_COME_BACK_USER_REWARD
//}} 

//{{ 2011. 06. 21	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM

typedef KPartyInfo					KEGS_CHANGE_PARTY_TYPE_REQ;
typedef KPacketOK					KEGS_CHANGE_PARTY_TYPE_ACK;

DECL_PACKET( EGS_CHANGE_PARTY_TYPE_NOT )
{
    KPartyInfo						m_kPartyInfo;
};

DECL_PACKET( EGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ )
{
	char							m_cPvpGameType;

	KEGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ()
	{
		m_cPvpGameType = 0;
	}
};

typedef KPacketOK									KEGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK;
typedef KEGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ		KEGS_PVP_PARTY_CHANGE_MATCH_INFO_NOT;

DECL_PACKET( EGS_REQUEST_MATCH_MAKING_REQ )
{
	char							m_cPvpPlayerCount;
    char							m_cPvpGameType;

	KEGS_REQUEST_MATCH_MAKING_REQ()
	{
		m_cPvpPlayerCount = 0;
		m_cPvpGameType = 0;
	}
};

typedef KPacketOK					KEGS_REQUEST_MATCH_MAKING_ACK;

DECL_PACKET( EGS_REG_MATCH_WAIT_LIST_SUCCESS_NOT )
{
    int								m_iEstimatedTime;

	KEGS_REG_MATCH_WAIT_LIST_SUCCESS_NOT()
	{
		m_iEstimatedTime = 0;
	}
};

typedef KPacketOK					KEGS_CANCEL_MATCH_MAKING_ACK;

DECL_PACKET( EGS_MATCH_MAKING_SUCCESS_NOT )
{
    UidType							m_iMatchUID;

	KEGS_MATCH_MAKING_SUCCESS_NOT()
	{
		m_iMatchUID = 0;
	}
};

DECL_PACKET( EGS_MATCH_MAKING_SUCCESS_REPLY_NOT )
{
	bool							m_bGameStartAgree;
	bool							m_bPvpPowerMode;
	bool							m_bPvpItemMode;
	std::set< short >				m_setSelectedPvpMap;

	KEGS_MATCH_MAKING_SUCCESS_REPLY_NOT()
	{
		m_bGameStartAgree = false;
		m_bPvpPowerMode = false;
		m_bPvpItemMode = false;
	}
};

DECL_PACKET( EGS_UNREG_MATCH_WAIT_LIST_NOT )
{
	int								m_iReason;
	std::wstring					m_wstrCancelUnitNickName;

	KEGS_UNREG_MATCH_WAIT_LIST_NOT()
	{
		m_iReason = 0;
	};
};

DECL_PACKET( EGS_MATCH_CLOSE_NOT )
{
    int								m_iReason;
	UidType							m_iCancelUnitUID;

	KEGS_MATCH_CLOSE_NOT()
	{
		m_iReason = 0;
		m_iCancelUnitUID = 0;
	}
};

DECL_PACKET( EGS_GAME_START_PVP_MATCH_NOT )
{
	KRoomInfo							m_RoomInfo;
	std::vector< KRoomSlotInfo >		m_vecSlot;
	std::vector< KRoomSlotInfo >		m_vecObserverSlot;
	std::map< UidType, KPvpNpcInfo >	m_mapPvpNpcInfo;
};

DECL_PACKET( EGS_PVP_NPC_UNIT_RE_BIRTH_POS_REQ )
{
	UidType							m_iPvpNpcUnitUID;
	int								m_iStartPosIndex;

	KEGS_PVP_NPC_UNIT_RE_BIRTH_POS_REQ()
	{
		m_iPvpNpcUnitUID = 0;
		m_iStartPosIndex = 0;
	}
};

typedef	KPacketOK		KEGS_PVP_NPC_UNIT_RE_BIRTH_POS_ACK;

DECL_PACKET( EGS_PVP_NPC_UNIT_INFO_TO_SERVER_REQ )
{
	std::map< UidType, float >		m_mapPvpNpcInfo;
};

typedef KPacketOK		KEGS_PVP_NPC_UNIT_INFO_TO_SERVER_ACK;

#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL
DECL_PACKET( EGS_OUT_USER_RETAINING_REQ )			// ���� ��Ŷ ��� �̰� ���� �����ּ���
{
	UidType								m_iUnitUID; // �ɸ��� uid	, �ɸ��� ����â�̶�� 0��
};

DECL_PACKET( EGS_OUT_USER_RETAINING_ACK )
{
	bool								m_bDepartureUser;				// ��Ż �����ΰ�?   // false �̸� �׳� ���� ��Ű�� �˴ϴ�.
	std::map< int, KRewardItem >		m_mapSlotItem;					// < ���Թ�ȣ, ����������>
};

DECL_PACKET( EGS_RETAINING_SELECT_REWARD_REQ )
{
	int		m_iSlotID;						// ������ ����ID
};

DECL_PACKET( EGS_RETAINING_SELECT_REWARD_ACK )
{
	int									m_iOK;
	bool								m_bIsPost;				 // �������� ���� �Ǿ��°�?
	KRewardItem							m_kRewardItem;			 // ȭ�� �˾� ��¿�
	int									m_iResurrectionStone;	 // ���� ��Ȱ�� ���� ����
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo; // ���ŵ� �κ� ���� ����

	KEGS_RETAINING_SELECT_REWARD_ACK()
	{
		m_iOK = 0;
		m_bIsPost = false;
		m_iResurrectionStone = 0;
	}
};
#endif SERV_NEXON_SESSION_PROTOCOL
//}} 

//{{ 2011. 08. 08	������	mac�ּ� �� ���
#ifdef SERV_MACHINE_ID_BLOCK
DECL_PACKET( EGS_CHECK_MACHINE_ID_REQ )
{
	std::string							m_strMachineID;
};

DECL_PACKET( EGS_CHECK_MACHINE_ID_ACK )
{
	int									m_iOK;
	std::string							m_strMachineID;
};
#endif SERV_MACHINE_ID_BLOCK
//}}

//{{ 2011. 08. 10    ��μ�    ä�θ� SOAP
#ifdef SERV_CHANNELING_SOAP
DECL_PACKET( EGS_TOONILAND_USER_ID_NOT )
{
	std::wstring						m_iTooniLandID;
};
#endif SERV_CHANNELING_SOAP
//}}

//{{ 2011. 08. 11	������	���� ���� ä�θ� 
#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
DECL_PACKET( EGS_ADMIN_CHANNELING_CODE_CHEAT_NOT )
{
	int									m_iChannelingCode;

	KEGS_ADMIN_CHANNELING_CODE_CHEAT_NOT()
	{
		m_iChannelingCode = 0;
	}
};
#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
//}}

//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
DECL_PACKET( EGS_ADMIN_HENIR_REWARD_CHEAT_NOT )
{
	int									m_iHenirRewardNormalCnt;

	KEGS_ADMIN_HENIR_REWARD_CHEAT_NOT()
	{
		m_iHenirRewardNormalCnt = 0;
	}
};
#endif SERV_NEW_HENIR_TEST
//}}

//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
DECL_PACKET( EGS_CHANGE_RANDOM_QUEST_NOT )
{
	std::map< int, int >					m_mapRandomQuestList;
};
#endif SERV_RANDOM_DAY_QUEST
//}}

//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
DECL_PACKET( ECH_HACKING_TOOL_LIST_REQ )
{
	int										m_iRandomKey;
	std::vector< KHackingToolInfo >			m_vecHackingToolList;
};

DECL_PACKET( ECH_HACKING_TOOL_LIST_ACK )
{
	int										m_iOK;
	std::string								m_strMachineID;
};
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
//}}

//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
DECL_PACKET( EGS_ADMIN_ED_MONITORING_CHEAT_NOT )
{
	bool						m_bEDMonitoringMode;
	int							m_iMinED;

	KEGS_ADMIN_ED_MONITORING_CHEAT_NOT()
	{
		m_bEDMonitoringMode = false;
		m_iMinED			= 100000000;
	}
};

DECL_PACKET( EGS_ED_MONITORING_BLOCK_REQ )
{
	enum ED_MONITORING_BLOCK_TYPE
	{
		EMBT_NONE = 0,
		EMBT_SEND_MAIL,
		EMBT_ACCOUNT_BLOCK,
		EMBT_MACHINE_ID_BLOCK,
	};

	UidType						m_iUnitUID;
	unsigned char				m_ucType;

	KEGS_ED_MONITORING_BLOCK_REQ()
	{
		m_iUnitUID = 0;
		m_ucType = 0;
	}
};

DECL_PACKET( EGS_ED_MONITORING_BLOCK_ACK )
{
	int						m_iOK;

	KEGS_ED_MONITORING_BLOCK_ACK()
	{
		m_iOK = 0;
	}
};

DECL_PACKET( EGS_COMMUNITY_USER_LIST_MONITORING_MODE_REQ )
{
	UINT								m_uiViewPage;
	std::vector< UidType >				m_vecUnitUIDList;

	KEGS_COMMUNITY_USER_LIST_MONITORING_MODE_REQ()
	{
		m_uiViewPage = 0;
	};
};
#endif SERV_ED_MONITORING_IN_GAME
//}}

//{{ 2011. 10. 13	������	������ ������
#ifdef SERV_CONTENT_MANAGER
DECL_PACKET( EGS_CASH_SHOP_OPEN_NOT )
{
    bool					m_bCashShopOpen;

	KEGS_CASH_SHOP_OPEN_NOT()
	{
		m_bCashShopOpen = true;
	}
};
#endif SERV_CONTENT_MANAGER
//}}

//{{ 2011. 09. 29  ��μ�	�˴ٿ��� ����
#ifdef SERV_SHUTDOWN_SYSTEM
DECL_PACKET( EGS_SHUT_DOWN_MESSAGE_NOT )
{
	int								m_iNetErrorID;

	KEGS_SHUT_DOWN_MESSAGE_NOT()
	{
		m_iNetErrorID = 0;
	}
};
#endif SERV_SHUTDOWN_SYSTEM
//}}

//{{ 2011.11.02     ��μ�    �������� SN �߱� �� ��ȿ�� �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
DECL_PACKET( ECH_NEW_SERVER_SN_NOT )
{
	unsigned char						m_charServerSN[SERVER_SN];

	KECH_NEW_SERVER_SN_NOT()
	{
		for( int i = 0 ; i < SERVER_SN ; ++i )
		{
			m_charServerSN[i] = 0;
		}
	}
};
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
//}}

//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
DECL_PACKET( EGS_BUY_UNIT_CLASS_CHANGE_NOT )
{
	UidType									m_iUnitUID;
	int										m_iNewUnitClass;
	std::vector< short >					m_vecSkillUnsealed;   // ���������� ��ų ����Ʈ
	
#ifdef SERV_REFORM_SKILL_NOTE
#else // KDBE_BUY_UNIT_CLASS_CHANGE_REQ
	std::map< char, int >					m_mapChangeMemo;  // <����MemoSlot, �ٲ�MemoID>
#endif // SERV_REFORM_SKILL_NOTE
	
	std::vector< KInventoryItemInfo >		m_vecChangeItem;
	std::vector< KCompleteQuestInfo >		m_vecChangeCompleteQuest;
	std::vector< KQuestInstance >			m_vecChangeInProgressQuest;
	KUnitInfo								m_kUnitInfo;
};
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

#ifdef SERV_CREATE_ELITE_FOR_ADMIN
DECL_PACKET( EGS_ADMIN_NPC_UNIT_CREATE_REQ )
{
	KNPCUnitReq			m_kNPCUnitReq;
	char				m_cAttrib1;
	char				m_cAttrib2;
	char				m_cSA1;
	char				m_cSA2;

	KEGS_ADMIN_NPC_UNIT_CREATE_REQ()
	{
		m_cAttrib1 = 0;
		m_cAttrib2 = 0;
		m_cSA1 = 0;
		m_cSA2 = 0;
	}
};

DECL_PACKET( EGS_ADMIN_NPC_UNIT_CREATE_NOT )
{
	KNPCUnitReq			m_kNPCUnitNot;
	char				m_cAttrib1;
	char				m_cAttrib2;
	char				m_cSA1;
	char				m_cSA2;
};
#endif SERV_CREATE_ELITE_FOR_ADMIN

//{{ 2012. 02. 14    ��μ�   PVP ���� Item ���� ��ġ ����
#ifdef SERV_PVP_ITEM_CHANGE_POS
DECL_PACKET( EGS_CHANGE_PVP_ITEM_POS_REQ )
{
	UidType									m_iItemUID;
	int										m_iPos;

	KEGS_CHANGE_PVP_ITEM_POS_REQ()
	{
		m_iItemUID	= 0;
		m_iPos		= 0;
	}
};

typedef		KPacketOK						KEGS_CHANGE_PVP_ITEM_POS_ACK;

typedef		KEGS_CHANGE_PVP_ITEM_POS_REQ	KEGS_CHANGE_PVP_ITEM_POS_NOT;
#endif SERV_PVP_ITEM_CHANGE_POS
//}}

//{{ 2012. 02. 21	��μ�	�κ��丮 ����
#ifdef SERV_SORT_CATEGORY_ITEM
DECL_PACKET( EGS_SORT_CATEGORY_ITEM_REQ )
{
	int						m_iCategoryType;
};

DECL_PACKET( EGS_SORT_CATEGORY_ITEM_ACK )
{
	int						m_iCategoryType;
	std::vector< UidType >	m_vecUpdatedInventorySlot;
	int						m_iOK;
};
#endif SERV_SORT_CATEGORY_ITEM 
//}}


//{{ 2011. 10. 27	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
DECL_PACKET( EGS_JOIN_BATTLE_FIELD_REQ )
{
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	KBattleFieldJoinInfo        m_kBattleFieldJoinInfo;
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	int							m_iBattleFieldID;
	int							m_StartPosIndex;
	bool						m_bMoveForMyParty;		// ���� ��Ƽ���� �ִ� �ʵ�� �̵��Ҷ��� true��
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

	KEGS_JOIN_BATTLE_FIELD_REQ()
	{
#ifndef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		m_iBattleFieldID = 0;
		m_StartPosIndex = 0;
		m_bMoveForMyParty = false;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	}
};

DECL_PACKET( EGS_JOIN_BATTLE_FIELD_ACK )
{
	int                                 m_iOK;
	KBattleFieldJoinInfo				m_kBattleFieldJoinInfo;
	KRoomInfo						    m_RoomInfo;
	std::vector< KRoomSlotInfo >        m_vecSlot;

	std::wstring						m_wstrCNIP;
#ifndef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	int									m_iLastTouchIndex;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

	int									m_iRequireLevel;
	int									m_iRequireDungeonID;

	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	std::wstring						m_wstrUDPRelayIP;
	USHORT								m_usUDPRelayPort;
#endif SERV_CHOOSE_FASTEST_HOST
	//}
	//{{ 2012. 11. 21	������		�����ڿ��� ĳ���� ���� ���� ������
#ifdef SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
	std::vector< UidType >				m_vecEnterCashShopUser;
#endif SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-05	// �ڼ���
	__time64_t							m_tRemainFieldHoldingTime;	// ���� �ʵ尡 ���� ������ ���� �ð�
#endif // SERV_BATTLE_FIELD_BOSS


	KEGS_JOIN_BATTLE_FIELD_ACK()
	{
		Initialize();
	}

	void Initialize()
	{
		m_iOK = 0;
		m_kBattleFieldJoinInfo.Initialize();
		m_RoomInfo.Initialize();
		m_vecSlot.clear();
		m_wstrCNIP.clear();
#ifndef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		m_iLastTouchIndex = 0;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		m_iRequireLevel = 0;
		m_iRequireDungeonID = 0;

		//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
		m_wstrUDPRelayIP.clear();
		m_usUDPRelayPort = 0;
#endif SERV_CHOOSE_FASTEST_HOST
		
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-05	// �ڼ���
		m_tRemainFieldHoldingTime = 0;
#endif // SERV_BATTLE_FIELD_BOSS
	}
};

typedef KEGS_JOIN_ROOM_NOT		KEGS_JOIN_BATTLE_FIELD_NOT;

DECL_PACKET( EGS_LEAVE_BATTLE_FIELD_NOT )
{
	char                            m_cRoomState;
	UidType                         m_iUnitUID;
	UidType							m_iPartyUID;
	int                             m_iReason;
	std::vector< KRoomSlotInfo >    m_vecSlot;
	std::vector< KRoomSlotInfo >    m_vecObserverSlot;

	KEGS_LEAVE_BATTLE_FIELD_NOT()
	{
		m_cRoomState = 0;
		m_iUnitUID = 0;
		m_iPartyUID = 0;
		m_iReason = 0;
	}
};

DECL_PACKET( EGS_BATTLE_FIELD_JOIN_USER_LOAD_COMPLETE_NOT )
{
    UidType								m_iLoadedUnitUID;

	KEGS_BATTLE_FIELD_JOIN_USER_LOAD_COMPLETE_NOT()
	{
		m_iLoadedUnitUID = 0;
	}
};

DECL_PACKET( EGS_BATTLE_FIELD_NPC_LOAD_NOT )
{
	std::vector< KNPCUnitReq >				m_vecNPCData;
	std::map< int, KAttribEnchantNpcInfo >	m_mapAttribNpcInfo;
	int										m_iBattleFieldID;

	KEGS_BATTLE_FIELD_NPC_LOAD_NOT()
	{
		m_iBattleFieldID = 0;
	}
};

typedef KPacketOK		KEGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK;

DECL_PACKET( EGS_BATTLE_FIELD_NPC_P2P_SYNC_NOT )
{
	std::vector< UidType >				m_vecNonNpcSyncUserList;
};

DECL_PACKET( EGS_BATTLE_FIELD_EVENT_MESSAGE_NOT )
{
	enum BF_EVENT_TYPE
	{
		BET_NONE = 0,
		BET_WARNING_MESSAGE,
	};

	char								m_cEventType;

	KEGS_BATTLE_FIELD_EVENT_MESSAGE_NOT()
	{
		m_cEventType = BET_NONE;
	}
};

DECL_PACKET( EGS_UPDATE_BATTLE_FIELD_USER_POS_NOT )
{
    KLastPositionInfo					m_kLastPos;
};

DECL_PACKET( EGS_QUICK_START_DUNGEON_GAME_REQ )
{
	int				m_iDungeonID;
	char			m_DifficultyLevel;
	char			m_cGetItemType;
	char			m_cDungeonMode;

	KEGS_QUICK_START_DUNGEON_GAME_REQ()
	{
		m_iDungeonID = 0;
		m_DifficultyLevel = 0;
		m_cGetItemType = 0;
		m_cDungeonMode = 0;
	}
};

DECL_PACKET( EGS_QUICK_START_DUNGEON_GAME_ACK )
{
	int									m_iOK;
	UidType								m_iFailUnitUID;				// ���� ����ŸƮ�� �����ϰ��� �庻�� UnitUID
	std::wstring						m_wstrFailUserNickName;		// ���� ����ŸƮ�� �����ϰ��� �庻�� �г���

	KEGS_QUICK_START_DUNGEON_GAME_ACK()
	{
		m_iOK = 0;
		m_iFailUnitUID = 0;
	}
};

typedef KEGS_QUICK_START_DUNGEON_GAME_REQ		KEGS_AUTO_PARTY_DUNGEON_GAME_REQ;

DECL_PACKET( EGS_AUTO_PARTY_DUNGEON_GAME_ACK )
{
	int									m_iOK;
	std::wstring						m_wstrFailUserNickName;

	KEGS_AUTO_PARTY_DUNGEON_GAME_ACK()
	{
		m_iOK = 0;
	}
};

DECL_PACKET( EGS_REG_AUTO_PARTY_WAIT_LIST_SUCCESS_NOT )
{
	UidType							m_iWaitNumber;			// ��� ��ȣǥ ( ������ ������ )
	int								m_iEstimatedTime;		// �ش� ������ ���� ��� ��� �ð�
	bool							m_bRemakingAutoParty;	// �ڵ� ��Ƽ ���� ����
	bool							m_bAutoPartyBonus;		// �ڵ���Ƽ ���ʽ� ����
	//{{ 2012. 11. 13	������		�ڵ���Ƽ ��⿭ ī��Ʈ ǥ��
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
	int								m_iAutoPartyWaitUserCount;
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
	//}}

	KEGS_REG_AUTO_PARTY_WAIT_LIST_SUCCESS_NOT()
	{
		m_iWaitNumber = 0;
		m_iEstimatedTime = 0;
		m_bRemakingAutoParty = false;
		m_bAutoPartyBonus = false;
		//{{ 2012. 11. 13	������		�ڵ���Ƽ ��⿭ ī��Ʈ ǥ��
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
		m_iAutoPartyWaitUserCount = 0;
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
		//}}
	}
};

DECL_PACKET( EGS_AUTO_PARTY_MAKING_SUCCESS_NOT )
{
	UidType							m_iAutoPartyUID;	// �ڵ� ��Ī UID ( ������ ������ )
	int								m_iMemberCount;		// �ڵ� ��Ī�� ��� ��
    int								m_iDungeonID;		// �ڵ� ��Ī ����
	char							m_cDifficultyLevel;	// �ڵ� ��Ī ���̵�
	//{{ 2012. 12. 17	�ڼ���	���� ��Ƽ �� ���� �˾� â ���� ���� ����
#ifdef SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
	char							m_cDungeonMode;		// ��д��� ����
#endif SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
	//}}

	KEGS_AUTO_PARTY_MAKING_SUCCESS_NOT()
		: m_iAutoPartyUID( 0 )
		, m_iMemberCount( 0 )
		, m_iDungeonID( 0 )
		, m_cDifficultyLevel( 0 )
		//{{ 2012. 12. 17	�ڼ���	���� ��Ƽ �� ���� �˾� â ���� ���� ����
#ifdef SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
		, m_cDungeonMode( 0 )
#endif SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
		//}}
	{
	}
};

DECL_PACKET( EGS_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT )
{
	bool							m_bGameStartAgree;	// �ڵ� ��Ī�� ���� ���� ���� ���� ����

	KEGS_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT()
	{
		m_bGameStartAgree = false;
	}
};

DECL_PACKET( EGS_AUTO_PARTY_CLOSE_NOT )
{
	int								m_iReason;			// �ڵ� ��Ī ��� ���� NetError
	UidType							m_iCancelUnitUID;	// �ڵ� ��Ī�� ��ҽ�Ų �庻��

	KEGS_AUTO_PARTY_CLOSE_NOT()
	{
		m_iReason = 0;
		m_iCancelUnitUID = 0;
	}
};

DECL_PACKET( EGS_UNREG_AUTO_PARTY_WAIT_LIST_NOT )
{
	int								m_iReason;			// �ڵ� ��Ī ��û ��� ���� NetError
	std::wstring					m_wstrCancelUnitNickName;	// �ڵ� ��Ī ����� ĳ���� �̸�

	KEGS_UNREG_AUTO_PARTY_WAIT_LIST_NOT()
	{
		m_iReason = 0;
	}
};

typedef KPacketOK								KEGS_CANCEL_AUTO_PARTY_MAKING_ACK;

DECL_PACKET( EGS_REGROUP_PARTY_NOT )
{
	KPartyInfo						m_kPartyInfo;
	std::vector< KPartyUserInfo >	m_vecPartyUserInfo;
};

DECL_PACKET( EGS_REQUEST_GET_AUTO_PARTY_BONUS_INFO_NOT )
{
	int											m_iVillageMapID;
	std::vector< KDungeonGameSimpleInfo >		m_vecDungeonList;

	KEGS_REQUEST_GET_AUTO_PARTY_BONUS_INFO_NOT()
	{
		m_iVillageMapID = 0;
	}
};

DECL_PACKET( EGS_UPDATE_AUTO_PARTY_BONUS_INFO_NOT )
{
	std::map< int, KAutoPartyDungeonBonusInfo >	m_mapDungeonBonusList;
};

DECL_PACKET( EGS_UPDATE_PLAY_STATUS_NOT )
{
	KGamePlayStatus					m_kGamePlayStatus;
	//{{ 2012. 11. 20	�ڼ���	Field Frame �α� �߰�
#ifdef SERV_FIELD_FRAME_LOG
	int								m_iFrame;
	
	KEGS_UPDATE_PLAY_STATUS_NOT()
		: m_iFrame( 0 )
	{
	}
#endif SERV_FIELD_FRAME_LOG
	//}}
};

DECL_PACKET( EGS_UPDATE_MOVE_TO_PARTY_MEMBER_INFO_NOT )
{
	bool							m_bIsOn;

	KEGS_UPDATE_MOVE_TO_PARTY_MEMBER_INFO_NOT()
	{
		m_bIsOn = false;
	}
};

DECL_PACKET( EGS_UPDATE_PARTY_MEMBER_STATUS_NOT )
{
    UidType							m_iUnitUID;
	KPartyMemberStatus				m_kStatus;

	KEGS_UPDATE_PARTY_MEMBER_STATUS_NOT()
	{
		m_iUnitUID = 0;
	}
};

DECL_PACKET( EGS_GAME_START_TIME_COUNT_NOT )
{
	UidType							m_iRoomUID;
	char							m_cSecondCount;
	bool							m_bIsDungeonRoom;

	KEGS_GAME_START_TIME_COUNT_NOT()
	{
		m_iRoomUID = 0;
		m_cSecondCount = 0;
		m_bIsDungeonRoom = false;
	}
};

DECL_PACKET( EGS_USE_SKILL_BUFF_IN_PLAY_NOT )
{
	int								m_iSkillID;		// buffid�� ����ɼ� ����
	bool							m_bOn;			// ��� & ������� ����

	KEGS_USE_SKILL_BUFF_IN_PLAY_NOT()
	{
		m_iSkillID = 0;
		m_bOn = false;
	}
};

DECL_PACKET( EGS_USE_SKILL_BUFF_IN_PLAY_BROAD_NOT )
{
	UidType							m_iUnitUID;
	int								m_iSkillID;		// buffid�� ����ɼ� ����
	bool							m_bOn;			// ��� & ������� ����

	KEGS_USE_SKILL_BUFF_IN_PLAY_BROAD_NOT()
	{
		m_iUnitUID = 0;
		m_iSkillID = 0;
		m_bOn = false;
	}
};

#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
DECL_PACKET( EGS_UPDATE_BUFF_INFO_NOT )
{
	std::vector< KBuffInfo >		m_vecActivateBuffList;
};

DECL_PACKET( EGS_UPDATE_BUFF_INFO_IN_ROOM_NOT )
{
	UidType							m_iUnitUID;	
	std::vector< KBuffInfo >		m_vecActivateBuffList;
	KStat							m_kGameStat;
	//{{ 2012. 12. 18	������	�ƶ� ��Ƽ �÷��� ���ʽ� ����ġ
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	std::map< char, float >			m_mapBonusRate;
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	//}}

	KEGS_UPDATE_BUFF_INFO_IN_ROOM_NOT()
	{
		m_iUnitUID = 0;
	}
};
#endif SERV_SERVER_BUFF_SYSTEM
//}

//{{ 2012. 06. 13	��μ�       ������ �˴ٿ���
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
DECL_PACKET( EGS_BEFORE_SHUT_DOWN_MESSAGE_NOT )
{
	int				m_iNetErrorID;		// ���� Ÿ��
	std::wstring	m_wstrTime;			// ���� ���� �ð�

	KEGS_BEFORE_SHUT_DOWN_MESSAGE_NOT()
	{
		m_iNetErrorID = 0;
	}
};
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
//}}

#ifdef SERV_EVENT_MONEY	// ��μ� // ���볯¥: 2013-07-04
DECL_PACKET( EGS_UPDATE_EVENT_MONEY_NOT )
{
	int				m_iEventMoney; // �̺�Ʈ ��ȭ

	KEGS_UPDATE_EVENT_MONEY_NOT()
	{
		m_iEventMoney = 0;
	}
};
#endif // SERV_EVENT_MONEY

//{{ 2012. 07. 02	������		��⿭ �ý���
#ifdef SERV_WAIT_QUEUE_SYSTEM
DECL_PACKET( ECH_WAIT_QUEUE_MESSAGE_NOT )
{
    int				m_iWaitUserCount;
	bool			m_bPassed;

	KECH_WAIT_QUEUE_MESSAGE_NOT()
	{
		m_iWaitUserCount = 0;
		m_bPassed = false;
	}
};
#endif SERV_WAIT_QUEUE_SYSTEM
//}}
//{{ 2012. 05. 17	�ڼ���	Ű���� ���� ������ DB���� �а� ����
#ifdef SERV_KEYBOARD_MAPPING_INFO_RW
typedef KKeyboardMappingInfo					KEGS_KEYBOARD_MAPPING_INFO_WRITE_REQ;
typedef KPacketOK								KEGS_KEYBOARD_MAPPING_INFO_WRITE_ACK;
DECL_PACKET( EGS_KEYBOARD_MAPPING_INFO_NOT )
{
	int								m_iOK;
	KKeyboardMappingInfo			m_kKeyboardMappingInfo;
};
#endif SERV_KEYBOARD_MAPPING_INFO_RW
//}}

//{{ 2012. 05. 25	�ڼ���	ä�� �ɼ� ������ DB���� �а� ����
#ifdef SERV_CHAT_OPTION_INFO_RW
DECL_PACKET( EGS_CHAT_OPTION_INFO_WRITE_REQ )
{
	byte							m_cIndex;
	bool							m_bValue;
};

typedef KPacketOK								KEGS_CHAT_OPTION_INFO_WRITE_ACK;
DECL_PACKET( EGS_CHAT_OPTION_INFO_NOT )
{
	int								m_iOK;
	std::map<byte, bool>			m_mapChatOptionSetInfo;
};
#endif SERV_CHAT_OPTION_INFO_RW
//}}

//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
DECL_PACKET( EGS_UPDATE_USER_UNIT_BUFF_INFO_NOT )
{
	UidType							m_iToUnitUID;		// ���� 0�̸� ��ε�ĳ����
	std::vector< KBuffFactor >		m_vecBuffFactor;

	KEGS_UPDATE_USER_UNIT_BUFF_INFO_NOT()
	{
		m_iToUnitUID = 0;
	}
};

DECL_PACKET( EGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT )
{
	UidType							m_iUnitUID;
	std::vector< KBuffFactor >		m_vecBuffFactor;

	KEGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT()
	{
		m_iUnitUID = 0;
	}
};

DECL_PACKET( EGS_UPDATE_NPC_UNIT_BUFF_INFO_NOT )
{
//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
    std::vector<UidType>            m_vecToUnitUID;
//#else   SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//	UidType							m_iToUnitUID;		// ���� 0�̸� ��ε�ĳ����
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
	std::vector< KNpcUnitBuffInfo >	m_vecNpcUnitBuff;

	KEGS_UPDATE_NPC_UNIT_BUFF_INFO_NOT()
	{
//#ifndef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//		m_iToUnitUID = 0;
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
	}
};

DECL_PACKET( EGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT )
{
	std::vector< KNpcUnitBuffInfo >	m_vecNpcUnitBuff;

	KEGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT()
	{
	}
};

//typedef KEGS_UPDATE_NPC_UNIT_BUFF_INFO_NOT		KEGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT;
#endif SERV_SERVER_BUFF_SYSTEM
//}}

//{{ 2012. 05. 30	������	�׳�ý� �Ⱓ ġƮ
#ifdef SERV_CASH_SKILL_POINT_DATE_CHANGE
DECL_PACKET( EGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ )
{
    int								m_iYear;
	int								m_iMonth;
	int								m_iDay;
	int								m_iHour;
	int								m_iMinute;

	KEGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ()
	{
		m_iYear = 0;
		m_iMonth = 0;
		m_iDay = 0;
		m_iHour = 0;
		m_iMinute = 0;
	}
};

DECL_PACKET( EGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_ACK )
{
	int								m_iOK;
	std::wstring					m_wstrCspEndDate;

	KEGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_ACK()
	{
		m_iOK = 0;
	}
};
#endif SERV_CASH_SKILL_POINT_DATE_CHANGE
//}}

//{{ 2012. 09. 19   ��μ�   ĳ�ü� �湮 ����
#ifdef SERV_VISIT_CASH_SHOP
DECL_PACKET( EGS_VISIT_CASH_SHOP_NOT )
{
	bool						m_bEnterCashShop;
	UidType						m_iUnitUID;

	KEGS_VISIT_CASH_SHOP_NOT()
	{
		m_bEnterCashShop = false;
		m_iUnitUID = 0;
	}
};
#endif SERV_VISIT_CASH_SHOP
//}}

//{{ 2012. 09. 02	�ڼ���	Merge ( �������� Ŭ���̾�Ʈ ���۵����� UDP ��Ŷ�� ���ƿ��� �ʴ� ������ �߰ߵǸ� �������� ű�Ѵ�. // 2012.06.11 lygan_������ )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
DECL_PACKET( EGS_UDP_CHECK_KICK_USER_NOT )
{
	UidType		m_iMyUnitUID;
	UidType		m_iAccusationUnitUID;	
	bool		m_bNowKickOut;
	//{{ 2012. 10. 7	�ڼ���	������ �α� �߰�
#ifdef UDP_CAN_NOT_SEND_USER_KICK_LOG
	byte		m_byteNum;
#endif UDP_CAN_NOT_SEND_USER_KICK_LOG
	//}}

	KEGS_UDP_CHECK_KICK_USER_NOT()
	{
		m_iMyUnitUID			= 0;
		m_iAccusationUnitUID	= 0;
		m_bNowKickOut			= false;

		//{{ 2012. 10. 7	�ڼ���	������ �α� �߰�
#ifdef UDP_CAN_NOT_SEND_USER_KICK_LOG
		m_byteNum				= 0;
#endif UDP_CAN_NOT_SEND_USER_KICK_LOG
		//}}
	};
};

DECL_PACKET( EGS_UDP_KICK_GAMEEDIT_NOT )
{
	bool											m_bUDPSetKick;
};
#endif UDP_CAN_NOT_SEND_USER_KICK
//}}

//{{ 2012 ���� ����2 ���� ��Ī �̺�Ʈ	- ��μ�
#ifdef SERV_2012_PVP_SEASON2_EVENT
DECL_PACKET( EGS_PVP_EVENT_INFO_NOT )
{
	bool											m_bEventInfo;

	KEGS_PVP_EVENT_INFO_NOT()
	{
		m_bEventInfo = false;
	}
};
#endif SERV_2012_PVP_SEASON2_EVENT
//}}

//{{ 2012. 09. 21	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
DECL_PACKET( EGS_BINGO_EVENT_INFO_ACK )		// ���� ���� �̺�Ʈ ����
{
	int										m_iOK;
	int										m_iLotteryChance;	// �̱� ���� Ƚ��
	int										m_iLotteryRecord;	// ���� Ƚ��
	int										m_iMixChance;		// �ڼ��� ���� Ƚ��
	int										m_iResetChance;		// �ʱ�ȭ ���� Ƚ��
	std::vector<byte>						m_vecBingoBoard;	// ���� ���� ������
	std::vector< std::pair<int, bool> >		m_vecPresentInfo;	// ���� ���� ����

	KEGS_BINGO_EVENT_INFO_ACK()
	{
		m_iOK				= 0;
		m_iLotteryChance	= 0;
		m_iLotteryRecord	= 0;
		m_iMixChance		= 0;
		m_iResetChance		= 0;
	}
};

DECL_PACKET( EGS_BINGO_EVENT_LOTTERY_ACK )	// ���� �̱�
{
	int					m_iOK;
	byte				m_byteNum;				// ���� ����
	std::vector<byte>	m_vecAcquiredPresent;	// ȹ���� ������ ��ġ ���� ������.
	int					m_iLotteryChance;
	int					m_iLotteryRecord;

	KEGS_BINGO_EVENT_LOTTERY_ACK()
	{
		clear();
	}

	void	clear( void )
	{
		m_iOK				= 0;
		m_byteNum			= 0;
		m_vecAcquiredPresent.clear();
		m_iLotteryChance	= 0;
		m_iLotteryRecord	= 0;
	}
};

DECL_PACKET( EGS_BINGO_EVENT_MIX_ACK )		// ���� �ڼ���
{
	int					m_iOK;
	std::vector<byte>	m_vecBingoBoard;		// �ڼ��� ������
	std::vector<byte>	m_vecAcquiredPresent;	// ȹ���� ������ ��ġ ���� ������.
	int					m_iMixChance;

	KEGS_BINGO_EVENT_MIX_ACK()
	{
		clear();
	}
	void	clear( void )
	{
		m_iOK			= 0;
		m_vecBingoBoard.clear();
		m_vecAcquiredPresent.clear();
		m_iMixChance	= 0;
	}

};

DECL_PACKET( EGS_BINGO_EVENT_RESET_ACK )	// �� �ʱ�ȭ
{
	int					m_iOK;
	std::vector<byte>	m_vecBingoBoard;	// �ڼ��� ������
	std::vector<int>	m_vecPresentInfo;	// �ڼ��� ���� ����
	int					m_iResetChance;

	KEGS_BINGO_EVENT_RESET_ACK()
	{
		clear();
	}

	void	clear( void )
	{
		m_iOK			= 0;
		m_vecBingoBoard.clear();
		m_vecPresentInfo.clear();
		m_iResetChance	= 0;
	}
};

typedef KEGS_BINGO_EVENT_RESET_ACK	KEGS_BINGO_EVENT_INIT_ACK;
#endif SERV_EVENT_BINGO
//}}

//{{ ��Ʋ�ʵ� ���赵 Ȯ�� ġƮ - ��μ�
#ifdef SERV_BATTLE_FIELD_DANGEROUS_CHEAT
DECL_PACKET( EGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT )
{
	int								m_iDangerousValue;

	KEGS_BATTLE_FIELD_DANGEROUS_VALUE_NOT()
	{
		m_iDangerousValue = 0;
	}
};
#endif SERV_BATTLE_FIELD_DANGEROUS_CHEAT
//}}

//{{ 2013. 2. 1	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
DECL_PACKET( EGS_FORCE_RELAY_NOT )
{
	UidType		m_iTargetUnitUID;

	KEGS_FORCE_RELAY_NOT()
		: m_iTargetUnitUID( 0 )
	{
	}
};

typedef	KPacketOK	KEGS_UDP_KICK_STATE_CHANGE_FIELD_NOT;
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

//{{ 2012. 10. 10	�ڼ���	ƨ�� ���� ������ ������
#ifdef UDP_CAN_NOT_SEND_USER_KICK
DECL_PACKET( EGS_UDP_KICK_STATE_CHANGE_FIELD_NOT )
{
	UidType	m_iAccusationUnitUID;
	
	KEGS_UDP_KICK_STATE_CHANGE_FIELD_NOT()
	{
		m_iAccusationUnitUID = 0;
	}
};
#endif UDP_CAN_NOT_SEND_USER_KICK
//}}

//{{ 2012. 11. 13	������		�ڵ���Ƽ ��⿭ ī��Ʈ ǥ��
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
DECL_PACKET( EGS_AUTO_PARTY_WAIT_USER_COUNT_NOT )
{
	int									m_iAutoPartyWaitUserCount;

	KEGS_AUTO_PARTY_WAIT_USER_COUNT_NOT()
	{
		m_iAutoPartyWaitUserCount = 0;
	}
};
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
//}}

//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
DECL_PACKET( EGS_UDP_PING_SCORE_NOT ) // client -> server
{
#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	bool m_bForceHost;
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	DWORD	m_dwPingScore;

	KEGS_UDP_PING_SCORE_NOT()
	{
#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
		m_bForceHost = false;
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
		m_dwPingScore = 99999;
	}
};

DECL_PACKET( EGS_CHECK_PING_SCORE_CHANGE_HOST_NOT ) // server -> client
{
	UidType					m_iOldHostUID;
	UidType					m_iNewHostUID;

	KEGS_CHECK_PING_SCORE_CHANGE_HOST_NOT()
	{
		m_iOldHostUID = 0;
		m_iNewHostUID = 0;
	}
};
#endif SERV_CHOOSE_FASTEST_HOST
//}

//{{ 2012. 12. 5	������		������ ���� ���� ���
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
DECL_PACKET( EGS_ADMIN_OBSERVER_MODE_REQ )
{
	bool					m_bObserverMode;

	KEGS_ADMIN_OBSERVER_MODE_REQ()
	{
		m_bObserverMode = false;
	}
};

typedef KPacketOK		KEGS_ADMIN_OBSERVER_MODE_ACK;
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
//}}

//{{ 2012. 11. 21	�ڼ���	���� ���� ������ ������ ����Ʈ
#ifdef SERV_HERO_PVP_MANAGE_LIST
DECL_PACKET( EGS_ADMIN_HERO_PVP_USER_LIST_ACK )
{
	std::vector< KHeroPvpUserInfo >	m_vecUserList;

	KEGS_ADMIN_HERO_PVP_USER_LIST_ACK()
	{
	}
};

DECL_PACKET( EGS_ADMIN_HERO_PVP_USER_ADD_REQ )
{
	std::vector< std::pair< UidType, std::wstring > >	m_vecUserList;

	KEGS_ADMIN_HERO_PVP_USER_ADD_REQ()
	{
	}
};

typedef	KEGS_ADMIN_HERO_PVP_USER_ADD_REQ	KEGS_ADMIN_HERO_PVP_USER_ADD_ACK;

typedef	KEGS_ADMIN_HERO_PVP_USER_ADD_REQ	KEGS_ADMIN_HERO_PVP_USER_REMOVE_REQ;
typedef	KEGS_ADMIN_HERO_PVP_USER_ADD_REQ	KEGS_ADMIN_HERO_PVP_USER_REMOVE_ACK;

DECL_PACKET( EGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ )
{
	UidType			m_iServerGroupID;
	std::wstring	m_wstrNickName;
	std::wstring	m_wstrMessage;

	KEGS_ADMIN_NOTIFY_HERO_PVP_USER_REQ()
		: m_iServerGroupID( -1 )
	{
	}
};

DECL_PACKET( EGS_ADMIN_HERO_PVP_USER_CONNECTION_NOT )
{
	typedef std::pair< bool, KHeroPvpUserInfo > PairHeroPvpUserInfo;
	std::vector< PairHeroPvpUserInfo >	m_vecUserList;

	KEGS_ADMIN_HERO_PVP_USER_CONNECTION_NOT()
	{
	}
};
#endif SERV_HERO_PVP_MANAGE_LIST
//}}

//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
DECL_PACKET( EGS_BAD_ATTITUDE_USER_CHECK_INFO_NOT )
{
	int m_iStageID;
	int m_iSubStageID;
	int m_iComboScore;
	int m_iTechScore;

	KEGS_BAD_ATTITUDE_USER_CHECK_INFO_NOT()
	{
		m_iStageID				= 0;
		m_iSubStageID			= 0;
		m_iComboScore			= 0;
		m_iTechScore			= 0;
	}
};

DECL_PACKET( EGS_BAD_ATTITUDE_USER_NOT )
{
	UidType	m_iUserUID;			// �ҷ�����

	KEGS_BAD_ATTITUDE_USER_NOT()
	{
		m_iUserUID				= 0;
	}
};

DECL_PACKET( EGS_FORCED_EXIT_VOTE_REQ )
{
	UidType	m_iUserUID;			// �ҷ�����
	bool	m_bWantForceExit;	// ���� ���ϴ°�?

	KEGS_FORCED_EXIT_VOTE_REQ()
	{
		m_iUserUID				= 0;
		m_bWantForceExit		= false;
	}
};

DECL_PACKET( EGS_FORCED_EXIT_VOTE_ACK )
{
	UidType		m_iUserUID;			// ��ǥ�� ���
	int			m_iOK;				// ���� ���ϴ°�?

	KEGS_FORCED_EXIT_VOTE_ACK()
	{
		m_iUserUID				= 0;
		m_iOK					= 0;
	}
};

DECL_PACKET( EGS_BAD_ATTITUDE_USER_MSG_NOT )
{
	enum MGS_TYPE
	{
		MT_NONE,								// ����(�ʱⰪ)
		MT_BAD_ATTITUDE_USER_WARNING,			// �ҷ��������� ��� �޽��� ����
		MT_BAD_ATTITUDE_USER_EXIT,				// �ҷ������� ���� �� �˸� �޽��� ����
		MT_FORCED_EXIT,							// �ҷ��������� ���� �˸� �޽��� ����

	};

	UidType				m_iUserUID;				// ��� ����
	char				m_MsgType;				// �޽��� Ÿ��

	KEGS_BAD_ATTITUDE_USER_MSG_NOT()
	{
		m_MsgType			= MT_NONE;
		m_iUserUID			= 0;
	}
};

DECL_PACKET( EGS_DEFENCE_DUNGEON_WAVE_ID_NOT )
{
	int						m_iWaveID;

	KEGS_DEFENCE_DUNGEON_WAVE_ID_NOT()
	{
		m_iWaveID			= 0;
	}
};
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}

//{{ 2013. 01. 21  ��Ը� �̺�Ʈ ���� �� ���� ���� ĳ���Ϳ��� �����ϴ� �ý��� - ��μ�
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
typedef u_short		KEGS_EXIST_FIRST_SELECT_UNIT_REWARD_NOT;
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
//}

//{{ 2013. 02. 01  �̺�Ʈ �߷�Ÿ�� ���� - ��μ�
#ifdef SERV_EVENT_VALENTINE_DUNGEON
DECL_PACKET( EGS_SYNC_DUNGEON_TIMER_NOT )
{
	int						m_iRemainTime;				// ���� �ð� (�����ð�)

	KEGS_SYNC_DUNGEON_TIMER_NOT()
	{
		m_iRemainTime			= 0;
	}
};
#endif SERV_EVENT_VALENTINE_DUNGEON
//}

//{{ 2013. 02. 07   �÷���� ���� ��ȭ �̺�Ʈ �ǽð� ���� - ��μ�
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
typedef bool		KEGS_SUPPORT_MATERIAL_EVENT_TIME_ACK;
#endif SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
//}

//{{ 2013. 03. 12	������	���� ��ȿ �˻�
#ifdef SERV_CHECK_EXIST_MONSTER_UID
DECL_PACKET( EGS_CHECK_EXIST_MONSTER_UID_REQ )
{
	std::vector< int >					m_vecNpcUID;
};

DECL_PACKET( EGS_CHECK_EXIST_MONSTER_UID_ACK )
{
	int									m_iOK;
	std::vector< int >					m_vecNotExistNpcUID;

	KEGS_CHECK_EXIST_MONSTER_UID_ACK()
	{
		m_iOK = 0;
	}
};
#endif SERV_CHECK_EXIST_MONSTER_UID
//}}

//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
DECL_PACKET( EGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT )
{
	KEGS_NPC_UNIT_CREATE_NOT			m_kCreatePacket;

	KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT()
	{
	}
};

DECL_PACKET( EGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_REQ )
{
	int									m_iDanger;

	KEGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_REQ()
	{
		m_iDanger = 0;
	}
};

DECL_PACKET( EGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK )
{
	int									m_iOK;
	int									m_iDanger;

	KEGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK()
	{
		m_iOK	  = 0;
		m_iDanger = 0;
	}
};

DECL_PACKET( EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_REQ )			// ������ -> server
{
    int									m_iNPCUID;				// ��� npc

	KEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_REQ()
	{
		m_iNPCUID = 0;
	}
};

typedef		KPacketOK									KEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_ACK;

DECL_PACKET( EGS_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ )			// ������ -> server
{
	UidType								m_iUnitUID;				// ��� unit
	int									m_iNPCUID;				// ��� npc

	KEGS_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ()
	{
		m_iNPCUID = 0;
	}
};

DECL_PACKET( EGS_BATTLE_FIELD_NPC_DAMAGED_MAP_ACK )			// host -> server
{
	UidType								m_iUnitUID;			// ��� unit
	int									m_iNPCUID;			// ��� npc
	std::map< UidType, float >			m_mapDamagedMap;

	KEGS_BATTLE_FIELD_NPC_DAMAGED_MAP_ACK()
	{
		m_iUnitUID		= 0;
		m_iNPCUID		= 0;
	}
};

typedef KEGS_BATTLE_FIELD_NPC_DAMAGED_MAP_ACK	KEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
//}

//{{ 2013. 3. 4	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
DECL_PACKET( EGS_LOCAL_RANKING_USER_INFO_READ_REQ )
{
	std::wstring m_wstrPassword;
};

DECL_PACKET( EGS_LOCAL_RANKING_USER_INFO_READ_ACK )
{
	int						m_iOK;
	KLocalRankingUserInfo	m_kInfo;
	std::wstring			m_wstrUpdated;

	KEGS_LOCAL_RANKING_USER_INFO_READ_ACK()
		: m_iOK( 0 )
	{
	}
};

DECL_PACKET( EGS_LOCAL_RANKING_USER_INFO_WRITE_REQ )
{
	std::wstring			m_wstrPassword;
	KLocalRankingUserInfo	m_kInfo;

	KEGS_LOCAL_RANKING_USER_INFO_WRITE_REQ()
	{
	}
};

typedef	KPacketOK	KEGS_LOCAL_RANKING_USER_INFO_WRITE_ACK;

DECL_PACKET( EGS_LOCAL_RANKING_INQUIRY_REQ )
{
	UINT	m_nViewPage;
	char	m_cMainTabIndex;
	char	m_cSubTabIndex;
	byte	m_byteFilter;
#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// �۾���¥: 2013-05-15	// �ڼ���
	bool	m_bRankingButtonClick;
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG

	KEGS_LOCAL_RANKING_INQUIRY_REQ()
		: m_nViewPage( 0 )
		, m_cMainTabIndex( -1 )
		, m_cSubTabIndex( -1 )
		, m_byteFilter( 0 )
#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// �۾���¥: 2013-05-15	// �ڼ���
		, m_bRankingButtonClick( false )
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
	{
	}
};

DECL_PACKET( EGS_LOCAL_RANKING_INQUIRY_ACK )
{
	int										m_iOK;
	UINT									m_nTotalPage;
	UINT									m_nViewPage;
	char									m_cMainTabIndex;
	char									m_cSubTabIndex;
	byte									m_byteFilter;
	std::vector<KLocalRankingRankerInfo>	m_vecRankerList;
	KLocalRankingRankerInfo					m_kPrevRankerInfo;
	KLocalRankingRankerInfo					m_kMyRankInfo;

	KEGS_LOCAL_RANKING_INQUIRY_ACK()
		: m_iOK( 0 )
		, m_nTotalPage( 0 )
		, m_nViewPage( 0 )
		, m_cMainTabIndex( -1 )
		, m_cSubTabIndex( -1 )
		, m_byteFilter( 0 )
	{
	}
};
#endif SERV_LOCAL_RANKING_SYSTEM
//}}

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
DECL_PACKET( EGS_SYSTEM_INFO )
{
	UidType m_iAccountUID;
	INT64 m_iSystemKey;
	std::wstring m_wstrCPU;
	std::wstring m_wstrMemory;
	std::wstring m_wstrGPU;
	std::wstring m_wstrOS;

	KEGS_SYSTEM_INFO() {
		m_iAccountUID = -1;
		m_iSystemKey= -1;
	}
};

DECL_PACKET( EGS_TCP_PING )
{
	unsigned int m_uiSequence;
	DWORD m_dwStartTime;

	KEGS_TCP_PING()
		: m_uiSequence( 0 )
		, m_dwStartTime( 0L )
	{
	}
};

DECL_PACKET( EGS_KOG_GAME_PERFORMANCE_VILLAGE_LOG )
{
	//�ĺ���
	UidType m_iCharUID;
	INT64 m_iSystemKey;
	//���������
	float m_fAvgFps;
	float m_fAvgFps_StdDev;
	//�޸� ��뷮
	bool m_bFirstMemUsageCheck;
	float m_fMinWorkingSetSize;
	float m_fMaxWorkingSetSize;
	float m_fMinPagefileUsage;
	float m_fMaxPagefileUsage;
	//�ΰ�����
	int m_iPartyType;
	int m_iPartyUserNum;
	int m_iMapID;
	int m_iVillageUserNum;

	//�˷����� ���� �� ����
	KVillageLagInfo m_kUnKnownLagInfo;
	//�˷��� ���� �� ����
	KVillageLagInfo m_kKnownLagInfo;

	KEGS_KOG_GAME_PERFORMANCE_VILLAGE_LOG()
	{
		//�ĺ���
		m_iCharUID = -1;
		m_iSystemKey = 0;
		//���������
		m_fAvgFps = -1.0f;
		m_fAvgFps_StdDev = -1.0f;
		//�޸� ��뷮
		m_bFirstMemUsageCheck = true;
		m_fMinWorkingSetSize = -1.0f;
		m_fMaxWorkingSetSize = -1.0f;
		m_fMinPagefileUsage = -1.0f;
		m_fMaxPagefileUsage = -1.0f;
		//�ΰ�����
		m_iPartyType = -1;
		m_iPartyUserNum = -1;
		m_iMapID = -1;
		m_iVillageUserNum = -1;
	}
};

DECL_PACKET( EGS_KOG_GAME_PERFORMANCE_FIELD_LOG )
{
	//�ĺ���
	UidType m_iCharUID;
	INT64 m_iSystemKey;
	//���������
	float m_fAvgFps;
	float m_fAvgFps_StdDev;
	//UDP Ping
	float m_fP2PMinPingTime;
	float m_fP2PMaxPingTime;
	float m_fP2PAvgPingTime;
	float m_fP2PAvgPingTime_StdDev;
	float m_fP2PAvgPingLossRate;
	float m_fRelayMinPingTime;
	float m_fRelayMaxPingTime;
	float m_fRelayAvgPingTime;
	float m_fRelayAvgPingTime_StdDev;
	float m_fRelayAvgPingLossRate;
	float m_fUdpMinPingTime;
	float m_fUdpMaxPingTime;
	float m_fUdpAvgPingTime;
	float m_fUdpAvgPingTime_StdDev;
	float m_fUdpAvgPingLossRate;
	//TCP Ping
	float m_fTcpMinPingTime;
	float m_fTcpMaxPingTime;
	float m_fTcpAvgPingTime;
	float m_fTcpAvgPingTime_StdDev;
	//UDP ��������
	float m_fUdpSendCountPerSec_Host;
	float m_fUdpSendAmountPerSec_Host;
	float m_fUdpRecvCountPerSec_Host;
	float m_fUdpRecvAmountPerSec_Host;
	float m_fUdpSendCountPerSec_NonHost;
	float m_fUdpSendAmountPerSec_NonHost;
	float m_fUdpRecvCountPerSec_NonHost;
	float m_fUdpRecvAmountPerSec_NonHost;
	//UDP ���ֽ�ũ �ν���
	float m_fUdpUserSyncPacketLossRate;
	float m_fUdpNpcSyncPacketLossRate;
	//��ȣ��Ʈ ��� NPC ���׼� delay ����
	float m_fNonHostNpcReaction_HostUdpAvgPingTime;	
	float m_fNonHostNpcReaction_MinDelayTime_SimulOff;
	float m_fNonHostNpcReaction_MaxDelayTime_SimulOff;
	float m_fNonHostNpcReaction_AvgDelayTime_SimulOff;
	float m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOff;
	float m_fNonHostNpcReaction_MinDelayTime_SimulOn;
	float m_fNonHostNpcReaction_MaxDelayTime_SimulOn;
	float m_fNonHostNpcReaction_AvgDelayTime_SimulOn;
	float m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOn;
	//�޸� ��뷮
	bool m_bFirstMemUsageCheck;
	float m_fMinWorkingSetSize;
	float m_fMaxWorkingSetSize;
	float m_fMinPagefileUsage;
	float m_fMaxPagefileUsage;
	//�ΰ�����
	int m_iPartyType;
	int m_iPartyUserNum;
	int m_iMapID;
	int m_iFieldUserNum;

	//�˷����� ���� �� ����
	KFieldLagInfo m_kUnKnownLagInfo;
	//�˷��� ���� �� ����
	KFieldLagInfo m_kKnownLagInfo;

	KEGS_KOG_GAME_PERFORMANCE_FIELD_LOG()
	{
		//�ĺ���
		m_iCharUID = -1;
		m_iSystemKey = 0;
		//���������
		m_fAvgFps = -1.0f;
		m_fAvgFps_StdDev = -1.0f;
		//UDP Ping
		m_fP2PMinPingTime = -1.0f;
		m_fP2PMaxPingTime = -1.0f;
		m_fP2PAvgPingTime = -1.0f;
		m_fP2PAvgPingTime_StdDev = -1.0f;
		m_fP2PAvgPingLossRate = -1.0f;
		m_fRelayMinPingTime = -1.0f;
		m_fRelayMaxPingTime = -1.0f;
		m_fRelayAvgPingTime = -1.0f;
		m_fRelayAvgPingTime_StdDev = -1.0f;
		m_fRelayAvgPingLossRate = -1.0f;
		m_fUdpMinPingTime = -1.0f;
		m_fUdpMaxPingTime = -1.0f;
		m_fUdpAvgPingTime = -1.0f;
		m_fUdpAvgPingTime_StdDev = -1.0f;
		m_fUdpAvgPingLossRate = -1.0f;
		//TCP Ping
		m_fTcpMinPingTime = -1.0f;
		m_fTcpMaxPingTime = -1.0f;
		m_fTcpAvgPingTime = -1.0f;
		m_fTcpAvgPingTime_StdDev = -1.0f;
		//UDP ��������
		m_fUdpSendCountPerSec_Host = -1.0f;
		m_fUdpSendAmountPerSec_Host = -1.0f;
		m_fUdpRecvCountPerSec_Host = -1.0f;
		m_fUdpRecvAmountPerSec_Host = -1.0f;
		m_fUdpSendCountPerSec_NonHost = -1.0f;
		m_fUdpSendAmountPerSec_NonHost = -1.0f;
		m_fUdpRecvCountPerSec_NonHost = -1.0f;
		m_fUdpRecvAmountPerSec_NonHost = -1.0f;
		//UDP ���ֽ�ũ �ν���
		m_fUdpUserSyncPacketLossRate = -1.0f;
		m_fUdpNpcSyncPacketLossRate = -1.0f;
		//��ȣ��Ʈ ��� NPC ���׼� delay ����
		m_fNonHostNpcReaction_HostUdpAvgPingTime = -1.0f;		
		m_fNonHostNpcReaction_MinDelayTime_SimulOff = -1.0f;
		m_fNonHostNpcReaction_MaxDelayTime_SimulOff = -1.0f;
		m_fNonHostNpcReaction_AvgDelayTime_SimulOff = -1.0f;
		m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOff = -1.0f;
		m_fNonHostNpcReaction_MinDelayTime_SimulOn = -1.0f;
		m_fNonHostNpcReaction_MaxDelayTime_SimulOn = -1.0f;
		m_fNonHostNpcReaction_AvgDelayTime_SimulOn = -1.0f;
		m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOn = -1.0f;
		//�޸� ��뷮
		m_bFirstMemUsageCheck = true;
		m_fMinWorkingSetSize = -1.0f;
		m_fMaxWorkingSetSize = -1.0f;
		m_fMinPagefileUsage = -1.0f;
		m_fMaxPagefileUsage = -1.0f;
		//�ΰ�����
		m_iPartyType = -1;
		m_iPartyUserNum = -1;
		m_iMapID = -1;
		m_iFieldUserNum = -1;
	}
};

DECL_PACKET( EGS_KOG_GAME_PERFORMANCE_DUNGEON_LOG )
{
	//�ĺ���
	UidType m_iCharUID;
	INT64 m_iSystemKey;
	//���������
	float m_fAvgFps;
	float m_fAvgFps_StdDev;
	//UDP Ping
	float m_fP2PMinPingTime;
	float m_fP2PMaxPingTime;
	float m_fP2PAvgPingTime;
	float m_fP2PAvgPingTime_StdDev;
	float m_fP2PAvgPingLossRate;
	float m_fRelayMinPingTime;
	float m_fRelayMaxPingTime;
	float m_fRelayAvgPingTime;
	float m_fRelayAvgPingTime_StdDev;
	float m_fRelayAvgPingLossRate;
	float m_fUdpMinPingTime;
	float m_fUdpMaxPingTime;
	float m_fUdpAvgPingTime;
	float m_fUdpAvgPingTime_StdDev;
	float m_fUdpAvgPingLossRate;
	//TCP Ping
	float m_fTcpMinPingTime;
	float m_fTcpMaxPingTime;
	float m_fTcpAvgPingTime;
	float m_fTcpAvgPingTime_StdDev;
	//UDP ��������
	float m_fUdpSendCountPerSec_Host;
	float m_fUdpSendAmountPerSec_Host;
	float m_fUdpRecvCountPerSec_Host;
	float m_fUdpRecvAmountPerSec_Host;
	float m_fUdpSendCountPerSec_NonHost;
	float m_fUdpSendAmountPerSec_NonHost;
	float m_fUdpRecvCountPerSec_NonHost;
	float m_fUdpRecvAmountPerSec_NonHost;
	//UDP ���ֽ�ũ �ν���
	float m_fUdpUserSyncPacketLossRate;
	float m_fUdpNpcSyncPacketLossRate;
	//��ȣ��Ʈ ��� NPC ���׼� delay ����
	float m_fNonHostNpcReaction_HostUdpAvgPingTime;	
	float m_fNonHostNpcReaction_MinDelayTime_SimulOff;
	float m_fNonHostNpcReaction_MaxDelayTime_SimulOff;
	float m_fNonHostNpcReaction_AvgDelayTime_SimulOff;
	float m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOff;
	float m_fNonHostNpcReaction_MinDelayTime_SimulOn;
	float m_fNonHostNpcReaction_MaxDelayTime_SimulOn;
	float m_fNonHostNpcReaction_AvgDelayTime_SimulOn;
	float m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOn;
	//�޸� ��뷮
	bool m_bFirstMemUsageCheck;
	float m_fMinWorkingSetSize;
	float m_fMaxWorkingSetSize;
	float m_fMinPagefileUsage;
	float m_fMaxPagefileUsage;
	//�ΰ�����
	int m_iPartyType;
	int m_iPartyUserNum;
	int m_iMapID;
	int m_iDifficulty;
	bool m_bEscape;
	int m_iStage;
	int m_iBlock;

	//�˷����� ���� �� ����
	KDungeonLagInfo m_kUnKnownLagInfo;
	//�˷��� ���� �� ����
	KDungeonLagInfo m_kKnownLagInfo;

	KEGS_KOG_GAME_PERFORMANCE_DUNGEON_LOG()
	{
		//�ĺ���
		m_iCharUID = -1;
		m_iSystemKey = 0;
		//���������
		m_fAvgFps = -1.0f;
		m_fAvgFps_StdDev = -1.0f;
		//UDP Ping
		m_fP2PMinPingTime = -1.0f;
		m_fP2PMaxPingTime = -1.0f;
		m_fP2PAvgPingTime = -1.0f;
		m_fP2PAvgPingTime_StdDev = -1.0f;
		m_fP2PAvgPingLossRate = -1.0f;
		m_fRelayMinPingTime = -1.0f;
		m_fRelayMaxPingTime = -1.0f;
		m_fRelayAvgPingTime = -1.0f;
		m_fRelayAvgPingTime_StdDev = -1.0f;
		m_fRelayAvgPingLossRate = -1.0f;		
		m_fUdpMinPingTime = -1.0f;
		m_fUdpMaxPingTime = -1.0f;
		m_fUdpAvgPingTime = -1.0f;
		m_fUdpAvgPingTime_StdDev = -1.0f;
		m_fUdpAvgPingLossRate = -1.0f;
		//TCP Ping
		m_fTcpMinPingTime = -1.0f;
		m_fTcpMaxPingTime = -1.0f;
		m_fTcpAvgPingTime = -1.0f;
		m_fTcpAvgPingTime_StdDev = -1.0f;
		//UDP ��������
		m_fUdpSendCountPerSec_Host = -1.0f;
		m_fUdpSendAmountPerSec_Host = -1.0f;
		m_fUdpRecvCountPerSec_Host = -1.0f;
		m_fUdpRecvAmountPerSec_Host = -1.0f;
		m_fUdpSendCountPerSec_NonHost = -1.0f;
		m_fUdpSendAmountPerSec_NonHost = -1.0f;
		m_fUdpRecvCountPerSec_NonHost = -1.0f;
		m_fUdpRecvAmountPerSec_NonHost = -1.0f;
		//UDP ���ֽ�ũ �ν���
		m_fUdpUserSyncPacketLossRate = -1.0f;
		m_fUdpNpcSyncPacketLossRate = -1.0f;
		//��ȣ��Ʈ ��� NPC ���׼� delay ����
		m_fNonHostNpcReaction_HostUdpAvgPingTime = -1.0f;		
		m_fNonHostNpcReaction_MinDelayTime_SimulOff = -1.0f;
		m_fNonHostNpcReaction_MaxDelayTime_SimulOff = -1.0f;
		m_fNonHostNpcReaction_AvgDelayTime_SimulOff = -1.0f;
		m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOff = -1.0f;
		m_fNonHostNpcReaction_MinDelayTime_SimulOn = -1.0f;
		m_fNonHostNpcReaction_MaxDelayTime_SimulOn = -1.0f;
		m_fNonHostNpcReaction_AvgDelayTime_SimulOn = -1.0f;
		m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOn = -1.0f;
		//�޸� ��뷮
		m_bFirstMemUsageCheck = true;
		m_fMinWorkingSetSize = -1.0f;
		m_fMaxWorkingSetSize = -1.0f;
		m_fMinPagefileUsage = -1.0f;
		m_fMaxPagefileUsage = -1.0f;
		//�ΰ�����
		m_iPartyType = -1;
		m_iPartyUserNum = -1;
		m_iMapID = -1;
		m_iDifficulty = -1;
		m_bEscape = true;
		m_iStage = -1;
		m_iBlock = -1;
	}
};

DECL_PACKET( EGS_KOG_GAME_PERFORMANCE_PVP_LOG )
{
	//�ĺ���
	UidType m_iCharUID;
	INT64 m_iSystemKey;
	//���������
	float m_fAvgFps;
	float m_fAvgFps_StdDev;
	//UDP Ping
	float m_fP2PMinPingTime;
	float m_fP2PMaxPingTime;
	float m_fP2PAvgPingTime;
	float m_fP2PAvgPingTime_StdDev;
	float m_fP2PAvgPingLossRate;
	float m_fRelayMinPingTime;
	float m_fRelayMaxPingTime;
	float m_fRelayAvgPingTime;
	float m_fRelayAvgPingTime_StdDev;
	float m_fRelayAvgPingLossRate;
	float m_fUdpMinPingTime;
	float m_fUdpMaxPingTime;
	float m_fUdpAvgPingTime;
	float m_fUdpAvgPingTime_StdDev;
	float m_fUdpAvgPingLossRate;
	//TCP Ping
	float m_fTcpMinPingTime;
	float m_fTcpMaxPingTime;
	float m_fTcpAvgPingTime;
	float m_fTcpAvgPingTime_StdDev;
	//UDP ��������
	float m_fUdpSendCountPerSec_Host;
	float m_fUdpSendAmountPerSec_Host;
	float m_fUdpRecvCountPerSec_Host;
	float m_fUdpRecvAmountPerSec_Host;
	float m_fUdpSendCountPerSec_NonHost;
	float m_fUdpSendAmountPerSec_NonHost;
	float m_fUdpRecvCountPerSec_NonHost;
	float m_fUdpRecvAmountPerSec_NonHost;
	//UDP ���ֽ�ũ �ν���
	float m_fUdpUserSyncPacketLossRate;
	float m_fUdpNpcSyncPacketLossRate;
	//�޸� ��뷮
	bool m_bFirstMemUsageCheck;
	float m_fMinWorkingSetSize;
	float m_fMaxWorkingSetSize;
	float m_fMinPagefileUsage;
	float m_fMaxPagefileUsage;
	//�ΰ�����
	int m_iMapID;
	int m_iPvPUserNum;
	int m_iRuleType;
	bool m_bEscape;
	int m_iRound;

	//�˷����� ���� �� ����
	KPvPLagInfo m_kUnKnownLagInfo;
	//�˷��� ���� �� ����
	KPvPLagInfo m_kKnownLagInfo;

	KEGS_KOG_GAME_PERFORMANCE_PVP_LOG()
	{
		//�ĺ���
		m_iCharUID = -1;
		m_iSystemKey = 0;
		//���������
		m_fAvgFps = -1.0f;
		m_fAvgFps_StdDev = -1.0f;
		//UDP Ping
		m_fP2PMinPingTime = -1.0f;
		m_fP2PMaxPingTime = -1.0f;
		m_fP2PAvgPingTime = -1.0f;
		m_fP2PAvgPingTime_StdDev = -1.0f;
		m_fP2PAvgPingLossRate = -1.0f;
		m_fRelayMinPingTime = -1.0f;
		m_fRelayMaxPingTime = -1.0f;
		m_fRelayAvgPingTime = -1.0f;
		m_fRelayAvgPingTime_StdDev = -1.0f;
		m_fRelayAvgPingLossRate = -1.0f;		
		m_fUdpMinPingTime = -1.0f;
		m_fUdpMaxPingTime = -1.0f;
		m_fUdpAvgPingTime = -1.0f;
		m_fUdpAvgPingTime_StdDev = -1.0f;
		m_fUdpAvgPingLossRate = -1.0f;
		//TCP Ping
		m_fTcpMinPingTime = -1.0f;
		m_fTcpMaxPingTime = -1.0f;
		m_fTcpAvgPingTime = -1.0f;
		m_fTcpAvgPingTime_StdDev = -1.0f;
		//UDP ��������
		m_fUdpSendCountPerSec_Host = -1.0f;
		m_fUdpSendAmountPerSec_Host = -1.0f;
		m_fUdpRecvCountPerSec_Host = -1.0f;
		m_fUdpRecvAmountPerSec_Host = -1.0f;
		m_fUdpSendCountPerSec_NonHost = -1.0f;
		m_fUdpSendAmountPerSec_NonHost = -1.0f;
		m_fUdpRecvCountPerSec_NonHost = -1.0f;
		m_fUdpRecvAmountPerSec_NonHost = -1.0f;
		//UDP ���ֽ�ũ �ν���
		m_fUdpUserSyncPacketLossRate = -1.0f;
		m_fUdpNpcSyncPacketLossRate = -1.0f;
		//�޸� ��뷮
		m_bFirstMemUsageCheck = true;
		m_fMinWorkingSetSize = -1.0f;
		m_fMaxWorkingSetSize = -1.0f;
		m_fMinPagefileUsage = -1.0f;
		m_fMaxPagefileUsage = -1.0f;
		//�ΰ�����
		m_iMapID = -1;
		m_iPvPUserNum = -1;
		m_iRuleType = -1;
		m_bEscape = true;
		m_iRound = -1;
	}
};
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
DECL_PACKET( EGS_GET_ACTIVE_LAGCHECK_REQ )
{
	KEGS_GET_ACTIVE_LAGCHECK_REQ()
	{
	}
};

DECL_PACKET( EGS_GET_ACTIVE_LAGCHECK_ACK )
{
	bool m_bActiveLagCheck;
	KEGS_GET_ACTIVE_LAGCHECK_ACK()
		: m_bActiveLagCheck( false )
	{
	}
};
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

//{{ 2013. 04. 05	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
DECL_PACKET( EGS_DECISION_ENTER_DEFENCE_DUNGEON_NOT )
{
	VECTOR3								m_DieBossPos;
};

DECL_PACKET( EGS_DEFENCE_DUNGEON_GAME_START_NOT )
{
	short								m_sWorldID;
	KRoomInfo						    m_RoomInfo;
	std::vector< KRoomSlotInfo >        m_vecSlot;
	std::vector< KRoomSlotInfo >		m_vecObserverSlot; // ���������ε� �������� �ʿ��ֳ�?

	std::wstring						m_wstrCNIP;

	//{{ 2009. 2. 4  ������		���� �����
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
	//}}
};

DECL_PACKET( EGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT )
{
	UidType								m_iUnitUID;
	KStat								m_kGameStat;

	KEGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT()
	{
		m_iUnitUID = 0;
	}
};

DECL_PACKET( EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_REQ )
{
	short								m_sSlotID;

	KEGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_REQ()
	{
		m_sSlotID = 0;
	}
};

DECL_PACKET( EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_ACK )
{
	int									m_iOK;
	KInventoryItemInfo					m_InventorySlotInfo;
	int									m_ItemID;

	KEGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_ACK()
	{
		m_iOK = 0;
		m_ItemID = 0;
	}
};
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

#if defined( SERV_NEW_DEFENCE_DUNGEON ) || defined( SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM )
DECL_PACKET( EGS_UPDATE_INVENTORY_SLOT_INFO_NOT )
{
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};
#endif //defined( SERV_NEW_DEFENCE_DUNGEON ) || defined( SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM )



#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-04-05
DECL_PACKET( EGS_LOCAL_RANKING_WATCH_UNIT_REQ )
{
	UidType	m_iUnitUID;
	
	KEGS_LOCAL_RANKING_WATCH_UNIT_REQ()
		: m_iUnitUID( 0 )
	{
	}
};

DECL_PACKET( EGS_LOCAL_RANKING_WATCH_UNIT_ACK )
{
	int						m_iOK;
	KLocalRankingUserInfo	m_kUserInfo;
	KLocalRankingUnitInfo	m_kUnitInfo;

	KEGS_LOCAL_RANKING_WATCH_UNIT_ACK()
		: m_iOK( -1 )	// ��� �����غôµ� �����̳� Unknown���� �޸� �ʱ�ȭ�� ���ķ� �������� ���� ��츦 ����غ��� �ϴ�.
	{
	}
};
#endif	// SERV_LOCAL_RANKING_SYSTEM

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
DECL_PACKET( EGS_COUPLE_PROPOSE_REQ )
{
	std::wstring					m_wstrUnitName;

	KEGS_COUPLE_PROPOSE_REQ()
	{
	}
};

DECL_PACKET( EGS_COUPLE_PROPOSE_ACK )
{
	int								m_iOK;
	std::wstring					m_wstrNickName;

	KEGS_COUPLE_PROPOSE_ACK()
	{
		m_iOK					= 0;
	}
};

DECL_PACKET( EGS_COUPLE_PROPOSE_NOT )
{
	std::wstring				m_wstrRequestUnitName;
	UidType						m_iRequestUnitUID;

	KEGS_COUPLE_PROPOSE_NOT()
	{
		m_iRequestUnitUID					= 0;
	}
};

DECL_PACKET( EGS_COUPLE_PROPOSE_AGREE_NOT )
{
	enum COUPLE_RESULT_TYPE
	{
		CRT_YES				= 0,
		CRT_NO				= 1,
		CRT_NOT_ANSWER		= 2,
	};

	char						m_cAnswer;			// 0 = ����, 1 = ����, 2 = ���� ����
	UidType						m_iRequestUnitUID;
	std::wstring				m_wstrRequestUnitName;
	UidType						m_iAcceptUserUID;
	u_char						m_ucAcceptUnitLevel;
	char						m_cAcceptUnitClass;
	UidType						m_iAcceptUnitUID;
	std::wstring				m_wstrAcceptUnitName;

	KEGS_COUPLE_PROPOSE_AGREE_NOT()
	{
		m_cAnswer				= CRT_YES;
		m_iRequestUnitUID		= 0;
		m_iAcceptUserUID		= 0;
		m_ucAcceptUnitLevel		= 0;
		m_cAcceptUnitClass		= 0;
		m_iAcceptUnitUID		= 0;
	}
};

DECL_PACKET( EGS_COUPLE_PROPOSE_RESULT_NOT )
{
	int							m_iOK;
	char						m_cResult;
	UidType						m_iUnitUID;	// ���� 
	std::wstring				m_wstrUnitName;	// ���� 
	__int64						m_tDate;

	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KEGS_COUPLE_PROPOSE_RESULT_NOT()
	{
		m_iOK						= 0;
		m_cResult					= 0;
		m_iUnitUID					= 0;
		m_tDate						= 0;
	}
};

DECL_PACKET( EGS_COUPLE_MAKING_SUCCESS_NOT )
{
	std::wstring				m_wstrRequestUnitName;	// Ŀ�ý�ûĳ��
	std::wstring				m_wstrAcceptUnitName;	// Ŀ�ü�������ĳ��

	KEGS_COUPLE_MAKING_SUCCESS_NOT()
	{
	}
};

DECL_PACKET( EGS_RELATIONSHIP_INFO_REQ )
{
	UidType						m_iAcceptUnitUID;

	KEGS_RELATIONSHIP_INFO_REQ()
	{
		m_iAcceptUnitUID = 0;
	}
};

DECL_PACKET( EGS_RELATIONSHIP_INFO_ACK )
{
	int									m_iOK;

	UidType								m_iRequestGSUID;
	UidType								m_iRequestUnitUID;
	UidType								m_iAcceptUnitUID;

	u_char								m_ucMyLevel;			// ����
	char								m_cMyUnitClass;			// Ŭ����
	int									m_iMyMapID;				// ��ġ
	std::wstring						m_wstrMyChannelName;	// ä�� �̸�
	char								m_cMyPosition;

	std::map< int, KInventoryItemInfo > m_mapOtherUnitEquippedItem;
	u_char								m_ucLevel;			// ����
	char								m_cUnitClass;		// Ŭ����
	int									m_iMapID;			// ��ġ
	__int64								m_tLastLogOutTime;	// ������ ���� �ð�
	bool								m_bIsConnect;		// ���� ���� ����
	std::wstring						m_wstrChannelName;	// ä�� �̸�
	char								m_cPosition;

	KEGS_RELATIONSHIP_INFO_ACK()
	{
		m_iOK = 0;

		m_iRequestGSUID = 0;
		m_iRequestUnitUID = 0;
		m_iAcceptUnitUID = 0;

		m_ucMyLevel = 0;
		m_cMyUnitClass = 0;
		m_iMyMapID = 0;
		m_cMyPosition = 0;
        
		m_ucLevel = 0;
		m_cUnitClass = 0;
		m_iMapID = 0;
		m_bIsConnect = false;
		m_tLastLogOutTime = 0;
		m_cPosition = 0;
	}
};

DECL_PACKET( EGS_RELATIONSHIP_EFFECT_NOT )
{
	UidType						m_iManUID;					// ��
	UidType						m_iGirlUID;					// ��

	int							m_iEffectIndex;

	std::wstring				m_wstrLoveWord;	// ��Ī

	KEGS_RELATIONSHIP_EFFECT_NOT()
	{
		m_iManUID = 0;
		m_iGirlUID = 0;
		m_iEffectIndex = 0;
	}
};

DECL_PACKET( EGS_WEDDING_PROPOSE_REQ )
{
	char						m_cWeddingHallType;			// ��ȥ���� Ÿ��
	char						m_cOfficiantNPC;			// �ַ�npc Ÿ��
	__int64						m_tWeddingDate;				// ��ȥ �Ͻ�
	std::wstring				m_wstrWeddingMsg;
	std::vector<UidType>		m_vecInviteUnitList;
	
	KEGS_WEDDING_PROPOSE_REQ()
	{
		m_cWeddingHallType = 0;
		m_cOfficiantNPC = 0;
		m_tWeddingDate = 0;
	}
};

DECL_PACKET( EGS_WEDDING_PROPOSE_ACK )
{
	int								m_iOK;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KEGS_WEDDING_PROPOSE_ACK()
	{
		m_iOK					= 0;
	}
};

DECL_PACKET( EGS_CHECK_NICK_NAME_REQ )
{
	std::wstring					m_wstrNickName;

	KEGS_CHECK_NICK_NAME_REQ()
	{
	}
};

DECL_PACKET( EGS_CHECK_NICK_NAME_ACK )
{
	int								m_iOK;
	UidType							m_iUnitUID;
	std::wstring					m_wstrNickName;

	KEGS_CHECK_NICK_NAME_ACK()
	{
		m_iOK					= 0;
		m_iUnitUID				= 0;
	}
};


DECL_PACKET( EGS_WEDDING_ITEM_INFO_REQ )
{
	std::vector< UidType >						m_vecWeddingItemList;

	KEGS_WEDDING_ITEM_INFO_REQ()
	{
	}
};

DECL_PACKET( EGS_WEDDING_ITEM_INFO_ACK )
{
	int											m_iOK;
	std::map< UidType, KWeddingItemInfo >		m_mapWeddingItemInfo;

	std::vector< KInventoryItemInfo >			m_vecUpdatedInventorySlot;

	KEGS_WEDDING_ITEM_INFO_ACK()
	{
		m_iOK = 0;
	}
};

DECL_PACKET( EGS_CALL_MY_LOVER_REQ )
{
	int							m_iMapID;
	VECTOR3						m_vPos;
	unsigned char				m_LastTouchLineIndex;

	KEGS_CALL_MY_LOVER_REQ()
	{
		m_iMapID = 0;
		m_LastTouchLineIndex = 0;
	}
};

typedef KPacketOK KEGS_CALL_MY_LOVER_ACK;

DECL_PACKET( EGS_CALL_MY_LOVER_VILLAGE_NOT )
{
	enum CALL_TYPE
	{
		CT_SAME_CHANNEL		= 0,		// ���� ä�� ����
		CT_OTHER_CHANNEL,				// �ٸ� ä�� ����
	};

	int							m_iMapID;
	VECTOR3						m_vPos;
	unsigned char				m_LastTouchLineIndex;

	UidType						m_iCalledUserUID;
	UidType						m_iCalledGSUID;

	char						m_cCallType;

	KEGS_CALL_MY_LOVER_VILLAGE_NOT()
	{
		m_iMapID = 0;
		m_LastTouchLineIndex = 0;
		m_iCalledUserUID = 0;
		m_iCalledGSUID = 0;
		m_cCallType = CT_SAME_CHANNEL;
	}
};

DECL_PACKET( EGS_CALL_MY_LOVER_AGREE_VILLAGE_NOT )
{
	bool						m_bAgree;		// ���� ���

	int							m_iMapID;
	VECTOR3						m_vPos;
	unsigned char				m_LastTouchLineIndex;

	UidType						m_iCalledUserUID;
	UidType						m_iCalledGSUID;

	char						m_cCallType;

	KEGS_CALL_MY_LOVER_AGREE_VILLAGE_NOT()
	{
		m_bAgree = false;
		m_iMapID = 0;
		m_LastTouchLineIndex = 0;
		m_iCalledUserUID = 0;
		m_iCalledGSUID = 0;
		m_cCallType = 0;
	}
};

DECL_PACKET( EGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT )
{
	int							m_iOK;
	int							m_iMapID;
	VECTOR3						m_vPos;
	unsigned char				m_LastTouchLineIndex;

	int							m_iLimitLevel;
	int							m_iRequireDungeonID;

	KEGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT()
	{
		m_iOK = 0;
		m_iMapID = 0;
		m_LastTouchLineIndex = 0;
		m_iLimitLevel = 0;
		m_iRequireDungeonID = 0;
	}
};

DECL_PACKET( EGS_CALL_MY_LOVER_ROOM_NOT )
{
	enum CALL_TYPE
	{
		CT_SAME_CHANNEL		= 0,		// ���� ä�� �ʵ�
		CT_OTHER_CHANNEL,				// �ٸ� ä�� �ʵ�
	};

	int							m_iBattleFieldID;
	UidType						m_RoomUID;

	VECTOR3						m_vPos;
	unsigned char				m_LastTouchLineIndex;

	UidType						m_iCalledUserUID;
	UidType						m_iCalledGSUID;

	char						m_cCallType;

	KEGS_CALL_MY_LOVER_ROOM_NOT()
	{
		m_iBattleFieldID = 0;
		m_RoomUID = 0;
		m_LastTouchLineIndex = 0;
		m_iCalledUserUID = 0;
		m_iCalledGSUID = 0;
		m_cCallType = CT_SAME_CHANNEL;
	}
};

DECL_PACKET( EGS_CALL_MY_LOVER_AGREE_ROOM_NOT )
{
	bool						m_bAgree;		// ���� ���
	int							m_iBattleFieldID;
	UidType						m_RoomUID;

	VECTOR3						m_vPos;
	unsigned char				m_LastTouchLineIndex;

	UidType						m_iCalledUserUID;
	UidType						m_iCalledGSUID;

	char						m_cCallType;

	KEGS_CALL_MY_LOVER_AGREE_ROOM_NOT()
	{
		m_bAgree = false;
		m_iBattleFieldID = 0;
		m_RoomUID = 0;
		m_LastTouchLineIndex = 0;
		m_iCalledUserUID = 0;
		m_iCalledGSUID = 0;
		m_cCallType = KEGS_CALL_MY_LOVER_ROOM_NOT::CT_SAME_CHANNEL;
	}
};

DECL_PACKET( EGS_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT )
{
	int                                 m_iOK;
	KBattleFieldJoinInfo				m_kBattleFieldJoinInfo;
	KRoomInfo						    m_RoomInfo;
	std::vector< KRoomSlotInfo >        m_vecSlot;

	std::wstring						m_wstrCNIP;
#ifndef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	int									m_iLastTouchIndex;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	VECTOR3								m_vPos;

	int									m_iRequireLevel;
	int									m_iRequireDungeonID;

	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
	std::wstring						m_wstrUDPRelayIP;
	USHORT								m_usUDPRelayPort;
	//{{ 2012. 11. 21	������		�����ڿ��� ĳ���� ���� ���� ������
	std::vector< UidType >				m_vecEnterCashShopUser;

	UidType								m_iCalledUserUID;
	UidType								m_iCalledGSUID;

	KEGS_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT()
	{
		m_iOK = 0;
		m_kBattleFieldJoinInfo.Initialize();
		m_RoomInfo.Initialize();
		m_vecSlot.clear();
		m_wstrCNIP.clear();
#ifndef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		m_iLastTouchIndex = 0;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		m_iRequireLevel = 0;
		m_iRequireDungeonID = 0;

		//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
		m_wstrUDPRelayIP.clear();
		m_usUDPRelayPort = 0;

		m_iCalledUserUID = 0;
		m_iCalledGSUID = 0;
	}
};

typedef bool KEGS_CALL_MY_LOVER_AGREE_RESULT_NOT;

typedef KPacketOK KEGS_CALL_MY_LOVER_JOIN_BATTLE_FIELD_FAILD_NOT;

DECL_PACKET( EGS_JOIN_WEDDING_HALL_REQ )
{
	UidType						m_iItemUID;
	int							m_iWeddingUID;

	KEGS_JOIN_WEDDING_HALL_REQ()
	{
		m_iItemUID = 0;
		m_iWeddingUID = 0;
	}
};

DECL_PACKET( EGS_JOIN_WEDDING_HALL_ACK )
{
	int                                 m_iOK;
	KWeddingHallInfo					m_kWeddingInfo;
	KRoomInfo						    m_RoomInfo;
	std::vector< KRoomSlotInfo >        m_vecSlot;

	std::wstring						m_wstrCNIP;

	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
	std::wstring						m_wstrUDPRelayIP;
	USHORT								m_usUDPRelayPort;

	KEGS_JOIN_WEDDING_HALL_ACK()
	{
		Initialize();
	}

	void Initialize()
	{
		m_iOK = 0;
		m_RoomInfo.Initialize();
		m_vecSlot.clear();
		m_wstrCNIP.clear();

		m_wstrUDPRelayIP.clear();
		m_usUDPRelayPort = 0;
	}
};

typedef KEGS_JOIN_ROOM_NOT		KEGS_JOIN_WEDDING_HALL_NOT;

#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
typedef KPacketReason           KEGS_LEAVE_WEDDING_HALL_REQ;
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
typedef KEGS_LEAVE_ROOM_REQ		KEGS_LEAVE_WEDDING_HALL_REQ;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
typedef KPacketOK				KEGS_LEAVE_WEDDING_HALL_ACK;

DECL_PACKET( EGS_LEAVE_WEDDING_HALL_NOT )
{
	int								m_iReason;
	char							m_cRoomState;
	UidType							m_iUnitUID;
	std::vector< KRoomSlotInfo >	m_vecSlot;

	KEGS_LEAVE_WEDDING_HALL_NOT()
	{
		m_iReason = 0;
		m_cRoomState = 0;
		m_iUnitUID = 0;
	}
};

DECL_PACKET( EGS_CHECK_BREAK_UP_ACK )
{
	int						m_iOK;
	int						m_iCost;
	char					m_cRelationshipType;
	std::wstring			m_wstrLoverNickName;

	KEGS_CHECK_BREAK_UP_ACK()
	{
		m_iOK = 0;
		m_iCost = 0;
		m_cRelationshipType = 0;
	}
};

typedef KPacketOK	KEGS_BREAK_UP_ACK;

DECL_PACKET( EGS_BREAK_UP_NOT )
{
	int			m_iCost;

	KEGS_BREAK_UP_NOT()
	{
		m_iCost = 0;
	}
};

typedef	KPacketOK		KEGS_START_WEDDING_ACK;

DECL_PACKET( EGS_START_WEDDING_ASK_ACK )
{
	bool				m_bAgree;

	KEGS_START_WEDDING_ASK_ACK()
	{
		m_bAgree = false;
	}
};

typedef KEGS_START_WEDDING_ASK_ACK	KEGS_START_WEDDING_AGREE_NOT;

DECL_PACKET( EGS_START_WEDDING_NOT )
{
	int									m_iOK;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KEGS_START_WEDDING_NOT()
	{
		m_iOK = 0;
	}
};

DECL_PACKET( EGS_CHANGE_LOVE_WORD_REQ )
{
	UidType						m_iItemUID;
	std::wstring				m_wstrNewLoveWord;

	KEGS_CHANGE_LOVE_WORD_REQ()
	{
		m_iItemUID = 0;
	}
};

typedef	KPacketOK		KEGS_CHANGE_LOVE_WORD_ACK;

DECL_PACKET( EGS_CHANGE_LOVE_WORD_NOT )
{
	UidType						m_iUseItemUnitUID;		// ���� ���� ��û�� ����
	std::wstring				m_wstrNewLoveWord;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KEGS_CHANGE_LOVE_WORD_NOT()
	{
		m_iUseItemUnitUID = 0;
	}
};

DECL_PACKET( EGS_ADMIN_CHANGE_COUPLE_DATE_REQ )
{
	int								m_iAddHour;

	KEGS_ADMIN_CHANGE_COUPLE_DATE_REQ()
	{
		m_iAddHour = 0;
	}
};

typedef	KPacketOK KEGS_ADMIN_CHANGE_COUPLE_DATE_ACK;
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-20
DECL_PACKET( EGS_GET_RIDING_PET_LIST_REQ )
{
	byte	m_byteViewPage;			// page ��ȣ�� 0�̸� ��ü ����Ʈ

	KEGS_GET_RIDING_PET_LIST_REQ()
		: m_byteViewPage( 0 )
	{
	}
};

DECL_PACKET( EGS_GET_RIDING_PET_LIST_ACK )
{
	int							m_iOK;
	std::vector<KRidingPetInfo>	m_vecRidingPetList;

	KEGS_GET_RIDING_PET_LIST_ACK()
		: m_iOK( 0 )
	{
	}
};

DECL_PACKET( EGS_SUMMON_RIDING_PET_REQ )
{
	UidType m_iRidingPetUID;
	bool	m_bNeedAck;				// ä�� �̵� �� ��ȯ�Ǿ� �ִ� ���̵� ���� �״�� ��ȯ���ֱ� ���� ���� ���������� ����Ѵ�.

	KEGS_SUMMON_RIDING_PET_REQ()
		: m_iRidingPetUID( 0 )
		, m_bNeedAck( true )
	{
	}
};

DECL_PACKET( EGS_SUMMON_RIDING_PET_ACK )
{
	int				m_iOK;
	KRidingPetInfo	m_kRidingPetInfo;

	KEGS_SUMMON_RIDING_PET_ACK()
		: m_iOK( 0 )
	{
	}
};

DECL_PACKET( EGS_SUMMON_RIDING_PET_NOT )
{
	UidType	m_iUnitUID;
	UidType	m_iRidingPetUID;
	USHORT	m_usRidingPetID;

	KEGS_SUMMON_RIDING_PET_NOT()
		: m_iUnitUID( 0 )
		, m_iRidingPetUID( 0 )
		, m_usRidingPetID( 0 )
	{
	}
};

DECL_PACKET( EGS_UNSUMMON_RIDING_PET_REQ )
{
	UidType m_iRidingPetUID;
	float	m_fStamina;

	KEGS_UNSUMMON_RIDING_PET_REQ()
		: m_iRidingPetUID( 0 )
		, m_fStamina( 0.0f )
	{
	}
};

DECL_PACKET( EGS_UNSUMMON_RIDING_PET_ACK )
{
	int			m_iOK;
	UidType		m_iRidingPetUID;

	KEGS_UNSUMMON_RIDING_PET_ACK()
		: m_iOK( 0 )
		, m_iRidingPetUID( 0 )
	{
	}
};

DECL_PACKET( EGS_UNSUMMON_RIDING_PET_NOT )
{
	UidType	m_iUnitUID;

	KEGS_UNSUMMON_RIDING_PET_NOT()
		: m_iUnitUID( 0 )
	{
	}
};

DECL_PACKET( EGS_CREATE_RIDING_PET_REQ )
{
	UidType	m_iItemUID;

	KEGS_CREATE_RIDING_PET_REQ()
		: m_iItemUID( 0 )
	{
	}
};

DECL_PACKET( EGS_CREATE_RIDING_PET_ACK )
{
	int									m_iOK;
	KRidingPetInfo						m_kCreatedPetInfo;
	std::vector<KInventoryItemInfo>		m_vecKInventorySlotInfo;

	KEGS_CREATE_RIDING_PET_ACK()
		: m_iOK( 0 )
	{
	}
};

DECL_PACKET( EGS_RELEASE_RIDING_PET_REQ )
{
	UidType		m_iRidingPetUID;
#ifdef SERV_EVENT_RIDING_WITH_SUB_QUEST
	bool		m_bCEventQuest;
#endif //SERV_EVENT_RIDING_WITH_SUB_QUEST

	KEGS_RELEASE_RIDING_PET_REQ()
		: m_iRidingPetUID( 0 )
	{
#ifdef SERV_EVENT_RIDING_WITH_SUB_QUEST
		m_bCEventQuest = false;
#endif //SERV_EVENT_RIDING_WITH_SUB_QUEST
	}
};

DECL_PACKET( EGS_RELEASE_RIDING_PET_ACK )
{
	int			m_iOK;
	UidType		m_iRidingPetUID;

	KEGS_RELEASE_RIDING_PET_ACK()
		: m_iOK( 0 )
		, m_iRidingPetUID( 0 )
	{
	}
};

DECL_PACKET( EGS_RELEASE_RIDING_PET_NOT )
{
	UidType		m_iRidingPetUID;
	USHORT		m_usRindingPetID;

	KEGS_RELEASE_RIDING_PET_NOT()
		: m_iRidingPetUID( 0 )
		, m_usRindingPetID( 0 )
	{
	}
};

DECL_PACKET( EGS_UPDATE_RIDING_PET_NOT )
{
	UidType		m_iRidingPetUID;
	float		m_fStamina;

	KEGS_UPDATE_RIDING_PET_NOT()
		: m_iRidingPetUID( 0 )
		, m_fStamina( 0.0f )
	{
	}
};
#endif	// SERV_RIDING_PET_SYSTM


//{{ 2013. 05. 10	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
DECL_PACKET( EGS_ITEM_EVALUATE_REQ )
{
	UidType								m_iItemUID;

	KEGS_ITEM_EVALUATE_REQ()
	{
		m_iItemUID = 0;
	}
};

DECL_PACKET( EGS_ITEM_EVALUATE_ACK )
{
	int									m_iOK;
	int									m_iED;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;

	KEGS_ITEM_EVALUATE_ACK()
	{
		m_iOK = 0;
		m_iED = 0;
	}
};

DECL_PACKET( EGS_RESTORE_ITEM_EVALUATE_REQ )
{
	UidType								m_iSupportItemUID;
	UidType								m_iTargetItemUID;

	KEGS_RESTORE_ITEM_EVALUATE_REQ()
	{
		m_iSupportItemUID = 0;
		m_iTargetItemUID = 0;
	}
};

DECL_PACKET( EGS_RESTORE_ITEM_EVALUATE_ACK )
{
	int									m_iOK;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;

	KEGS_RESTORE_ITEM_EVALUATE_ACK()
	{
		m_iOK = 0;
	}
};

DECL_PACKET( EGS_ITEM_CONVERT_REQ )
{
    UidType								m_iItemUID;
    int                                 m_iQuantity;
	KEGS_ITEM_CONVERT_REQ()
	{
		m_iItemUID = 0;
        m_iQuantity = 0;
	}
};

DECL_PACKET( EGS_ITEM_CONVERT_ACK )
{
	int									m_iOK;
	int									m_iED;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;

	KEGS_ITEM_CONVERT_ACK()
	{
		m_iOK = 0;
		m_iED = 0;
	}
};
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}

//{{ 2013. 04. 22	������	����� �� ���� ���� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
DECL_PACKET( EGS_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT )
{
	bool								m_bAgree;

	KEGS_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT()
	{
		// �⺻������ �����Ѱ����� �ʱ�ȭ
		m_bAgree = true;
	}
};
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
//}}

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT// �۾���¥: 2013-05-10	// �ڼ���
DECL_PACKET( EGS_SEND_LOVE_LETTER_EVENT_REQ )
{
	std::wstring	m_wstrMessage;

	KEGS_SEND_LOVE_LETTER_EVENT_REQ()
	{
	}
};

DECL_PACKET( EGS_SEND_LOVE_LETTER_EVENT_ACK )
{
	int									m_iOK;
	int									m_iED;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KEGS_SEND_LOVE_LETTER_EVENT_ACK()
		: m_iOK( 0 )
		, m_iED( 0 )
	{
	}
};
#endif // SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

#ifdef SERV_FIELD_WORKINGS_BLOCK_LOG// �۾���¥: 2013-05-15	// �ڼ���
DECL_PACKET( EGS_FIELD_WORKINGS_BLOCK_LOG_ACK )
{
	char	m_cX2State;
	int		m_iPhysicalMemory;
	int		m_iVirtualMemory;
	char	m_cProcessTerm;
	char	m_cFieldUserNum;
	char	m_cFieldNpcNum;

	KEGS_FIELD_WORKINGS_BLOCK_LOG_ACK()
		: m_cX2State( 0 )
		, m_iPhysicalMemory( 0 )
		, m_iVirtualMemory( 0 )
		, m_cProcessTerm( 0 )
		, m_cFieldUserNum( 0 )
		, m_cFieldNpcNum( 0 )
	{
	}
};
#endif // SERV_FIELD_WORKINGS_BLOCK_LOG

#ifdef SERV_HACKING_USER_PROCESS_NAME_RECORD// �۾���¥: 2013-06-03	// �ڼ���
DECL_PACKET( EGS_HACKING_USER_PROCESS_NAME_RECORD_NOT )
{
	byte			m_byteType;
	std::wstring	m_wstrReason;

	KEGS_HACKING_USER_PROCESS_NAME_RECORD_NOT()
		: m_byteType( 0 )
	{
	}
};
#endif // SERV_HACKING_USER_PROCESS_NAME_RECORD

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
DECL_PACKET( EGS_CHANGE_MY_UNIT_INFO_NOT )
{
	char			m_cUnitClass;
	int				m_iNewDefaultSkill_1;
	int				m_iNewDefaultSkill_2;

	KEGS_CHANGE_MY_UNIT_INFO_NOT()
	{
		m_cUnitClass = 0;
		m_iNewDefaultSkill_1 = 0;
		m_iNewDefaultSkill_2 = 0;
	}
};

#ifdef SERV_SKILL_PAGE_SYSTEM
DECL_PACKET( EGS_ADMIN_AUTO_GET_ALL_SKILL_REQ )
{
	int				m_iActiveSkillPageNumber;

	KEGS_ADMIN_AUTO_GET_ALL_SKILL_REQ()
		: m_iActiveSkillPageNumber(0)
	{}
};
#endif // SERV_SKILL_PAGE_SYSTEM

DECL_PACKET( EGS_ADMIN_AUTO_GET_ALL_SKILL_ACK )
{
	int						m_iOK;
#ifdef SERV_SKILL_PAGE_SYSTEM
	int						m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM
	int						m_iSPoint;
	int						m_iCSPoint;
	std::map< int, int>		m_mapSkillList;
	std::vector< short >	m_vecUnsealedSkillID;

	KEGS_ADMIN_AUTO_GET_ALL_SKILL_ACK()
	{
		m_iOK = 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iActiveSkillPageNumber = 0;
#endif // SERV_SKILL_PAGE_SYSTEM
		m_iSPoint = 0;
		m_iCSPoint = 0;
	}
};

DECL_PACKET( EGS_ADMIN_GET_SKILL_REQ )
{
	int						m_iSkillID;
	int						m_iSkillLevel;
#ifdef SERV_SKILL_PAGE_SYSTEM
	int						m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	KEGS_ADMIN_GET_SKILL_REQ()
	{
		m_iSkillID = 0;
		m_iSkillLevel = 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iActiveSkillPageNumber = 0;
#endif // SERV_SKILL_PAGE_SYSTEM
	}
};

DECL_PACKET( EGS_ADMIN_GET_SKILL_ACK )
{
	int						m_iOK;
	int						m_iSkillID;
	int						m_iSkillLevel;
#ifdef SERV_SKILL_PAGE_SYSTEM
	int						m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM
	int						m_iCSPoint;
	bool					m_bUnsealed;

	KEGS_ADMIN_GET_SKILL_ACK()
	{
		m_iOK = 0;
		m_iSkillID = 0;
		m_iSkillLevel = 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iActiveSkillPageNumber = 0;
#endif // SERV_SKILL_PAGE_SYSTEM
		m_iCSPoint = 0;
		m_bUnsealed = false;
	}
};

#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-15	// �ڼ���
DECL_DATA( KNXBTPurchaseCouponAckInfo )
{
	unsigned long	m_ulProductNo;
	unsigned short	m_usOrderQuantity;
	int				m_iCouponUseApproval;
	std::wstring	m_wstrExtendValue;

	KNXBTPurchaseCouponAckInfo()
		: m_ulProductNo( 0 )
		, m_usOrderQuantity( 0 )
		, m_iCouponUseApproval( 0 )
	{
	}
};

DECL_PACKET( EGS_COUPON_LIST_REQ )
{
	bool			m_bUsed;
	short			m_sCouponBoxType;
	unsigned short	m_usPageNum;

	KEGS_COUPON_LIST_REQ()
		: m_bUsed( false )
		, m_sCouponBoxType( 0 )
		, m_usPageNum( 0 )
	{
	}
};

DECL_PACKET( EGS_COUPON_LIST_ACK )
{
	int								m_iOK;
	bool							m_bUsed;
	short							m_sCouponBoxType;
	unsigned short					m_usPageNum;
	int								m_iCouponCount;
	std::vector<KClientCouponBox>	m_vecCouponList;

	KEGS_COUPON_LIST_ACK()
		: m_iOK( 0 )
		, m_bUsed( false )
		, m_sCouponBoxType( 0 )
		, m_usPageNum( 0 )
		, m_iCouponCount( 0 )
	{
	}
};

DECL_PACKET( EGS_REGIST_COUPON_REQ )
{
	std::string	m_strCouponPin;

	KEGS_REGIST_COUPON_REQ()
	{
	}
};

typedef	KPacketOK	KEGS_REGIST_COUPON_ACK;

DECL_PACKET( EGS_USE_ITEM_COUPON_REQ )
{
	int	m_iCouponCardNo;

	KEGS_USE_ITEM_COUPON_REQ()
		: m_iCouponCardNo( 0 )
	{
	}
};

typedef	KPacketOK	KEGS_USE_ITEM_COUPON_ACK;

DECL_PACKET( EGS_COUPON_TARGET_ITEM_REQ )
{
	int				m_iCouponCardNo;
	unsigned short	m_usPageNum;

	KEGS_COUPON_TARGET_ITEM_REQ()
		: m_iCouponCardNo( 0 )
		, m_usPageNum( 0 )
	{
	}
};

DECL_PACKET( EGS_COUPON_TARGET_ITEM_ACK )
{
	int									m_iOK;
	int									m_iCouponCardNo;
	unsigned short						m_usPageNum;
	int									m_iTargetItemCount;
	std::vector<KCouponBoxTargetItem>	m_vecTargetItem;

	KEGS_COUPON_TARGET_ITEM_ACK()
		: m_iOK( 0 )
		, m_iCouponCardNo( 0 )
		, m_usPageNum( 0 )
		, m_iTargetItemCount( 0 )
	{
	}
};

typedef	KDiscountCouponInquriyInfo	KEGS_DISCOUNT_COUPON_INQUIRY_REQ;

DECL_PACKET( EGS_DISCOUNT_COUPON_INQUIRY_ACK )
{
	int								m_iOK;
	std::vector<KDiscountCoupon>	m_vecDiscountCoupon;

	KEGS_DISCOUNT_COUPON_INQUIRY_ACK()
		: m_iOK( 0 )
	{
	}
};

DECL_PACKET( EGS_DISCOUNT_COUPON_LIST_INQUIRY_REQ )
{
	std::vector<KDiscountCouponInquriyInfo>	m_vecDiscountCouponInquiryInfo;
};

typedef	KEGS_DISCOUNT_COUPON_INQUIRY_ACK	KEGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK;

DECL_PACKET( EGS_BUY_CASH_ITEM_COUPON_ACK )
{
	int										m_iOK;
	std::vector<KNXBTPurchaseCouponAckInfo>	m_vecPurchaseCouponAckInfo;
	bool									m_bIsWishList;

	KEGS_BUY_CASH_ITEM_COUPON_ACK()
		: m_iOK( 0 )
		, m_bIsWishList( false )
	{
	}
};

typedef	KEGS_BUY_CASH_ITEM_COUPON_ACK	KEGS_PRESENT_CASH_ITEM_COUPON_ACK;
#endif // SERV_NEXON_COUPON_SYSTEM

#ifdef SERV_JUMPING_CHARACTER// �۾���¥: 2013-07-11	// �ڼ���
DECL_PACKET( EGS_JUMPING_CHARACTER_REQ )
{
	int	m_iClass;

	KEGS_JUMPING_CHARACTER_REQ()
		: m_iClass( 0 )
	{
	}
};

DECL_PACKET( EGS_JUMPING_CHARACTER_ACK )
{
	int					m_iOK;
	KUnitInfo			m_kUnitInfo;
	char				m_cExpandedMaxPageNum;
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
	std::vector<int>	m_vecNewDefaultSkill;
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
	std::vector<short>	m_vecUnSealedSkill;

	KEGS_JUMPING_CHARACTER_ACK()
		: m_iOK( 0 )
		, m_cExpandedMaxPageNum( 0 )
	{
	}
};
#endif // SERV_JUMPING_CHARACTER

#ifdef SERV_FINALITY_SKILL_SYSTEM	// ���볯¥: 2013-08-01
DECL_PACKET( EGS_ITEM_EXTRACT_REQ )
{
	UidType							m_iItemUID;
	int								m_iQuantity;

	KEGS_ITEM_EXTRACT_REQ()
	{
		m_iItemUID			= 0;
		m_iQuantity			= 0;
	}
};


DECL_PACKET( EGS_ITEM_EXTRACT_ACK )
{
	int									m_iOK;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
	std::map< int, int >                m_mapInsertedItem;

	KEGS_ITEM_EXTRACT_ACK()
	{
		m_iOK			= 0;
	}
};

DECL_PACKET( EGS_USE_FINALITY_SKILL_REQ )
{
	UidType							m_iItemUID;
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
	bool							m_bNoConsume;
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT

	KEGS_USE_FINALITY_SKILL_REQ()
	{
		m_iItemUID			= 0;
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
		m_bNoConsume		= false;
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
	}
};


DECL_PACKET( EGS_USE_FINALITY_SKILL_ACK )
{
	int									m_iOK;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
	bool								m_bNoConsume;
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT

	KEGS_USE_FINALITY_SKILL_ACK()
	{
		m_iOK			= 0;
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
		m_bNoConsume		= false;
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
	}
};
#endif // SERV_FINALITY_SKILL_SYSTEM

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-28	// �ڼ���
DECL_PACKET( EGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT )
{
	int		m_iBattleFieldID;	// ����Ʈ�� ������ ��Ʋ �ʵ� ID
	int		m_iBossFieldID;		// ����Ʈ�� ����� ���� �ʵ� ID

	KEGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT()
		: m_iBattleFieldID( 0 )
		, m_iBossFieldID( 0 )
	{
	}
};

DECL_PACKET( EGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT )
{
	int		m_iBattleFieldID;	// ����Ʈ�� �ı��� ��Ʋ �ʵ� ID

	KEGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT()
		: m_iBattleFieldID( 0 )
	{
	}
};

typedef	KBossFieldJoinInfo	KEGS_BOSS_FIELD_RETURN_PROCESS_NOT;

DECL_PACKET( EGS_SOCKET_EXPAND_ITEM_REQ )
{
	UidType	m_iMaterialItemUID;
	UidType	m_iTargetItemUID;

	KEGS_SOCKET_EXPAND_ITEM_REQ()
		: m_iMaterialItemUID( 0 )
		, m_iTargetItemUID( 0 )
	{
	}
};

DECL_PACKET( EGS_SOCKET_EXPAND_ITEM_ACK )
{
	int								m_iOK;
	std::vector<KInventoryItemInfo>	m_vecUpdatedInventorySlot;

	KEGS_SOCKET_EXPAND_ITEM_ACK()
		: m_iOK( 0 )
	{
	}
};
#endif // SERV_BATTLE_FIELD_BOS

#ifdef SERV_STAGE_CLEAR_IN_SERVER// �۾���¥: 2013-10-30	// �ڼ���
DECL_PACKET( EGS_DUNGEON_SUB_STAGE_CLEAR_REQ )
{
	int		m_iClearConditionIndex;

	KEGS_DUNGEON_SUB_STAGE_CLEAR_REQ()
		: m_iClearConditionIndex( 0 )
	{
	}
};
DECL_PACKET( EGS_DUNGEON_SUB_STAGE_CLEAR_ACK )
{
	int		m_iOK;
	int		m_iClearType;
	int		m_iStageIndex;
	int		m_iSubStageIndex;

	KEGS_DUNGEON_SUB_STAGE_CLEAR_ACK()
		: m_iOK( 0 )
		, m_iClearType( 0 )
		, m_iStageIndex( 0 )
		, m_iSubStageIndex( 0 )
	{
	}
};
DECL_PACKET( EGS_SECRET_STAGE_LOAD_REQ )
{
	int		m_iPadID;

	KEGS_SECRET_STAGE_LOAD_REQ()
		: m_iPadID( 0 )
	{
	}
};
DECL_PACKET( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT )
{
	int		m_iStartSecretStageEnteringEvent;

	KEGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT()
		: m_iStartSecretStageEnteringEvent( 0 )
	{
	}
};
#endif // SERV_STAGE_CLEAR_IN_SERVER

#ifdef SERV_GOOD_ELSWORD
DECL_PACKET( EGS_GET_NEXT_BANK_ED_ACK )
{
    int m_iOK;
    int m_iED;

    KEGS_GET_NEXT_BANK_ED_ACK()
        :m_iOK(0)
        ,m_iED(0)
    {}
};

typedef KEGS_GET_NEXT_BANK_ED_ACK    KEGS_GET_NEXT_INVENTORY_ED_ACK;

DECL_PACKET( EGS_EXPAND_BANK_SLOT_ED_ACK )
{
    int m_iOK;
    int m_iED;
    std::map< int, int >					m_mapExpandedCategorySlot;

    KEGS_EXPAND_BANK_SLOT_ED_ACK()
        :m_iOK(0)
        ,m_iED(0)
    {
        m_mapExpandedCategorySlot.clear();
    }
};



typedef KEGS_EXPAND_BANK_SLOT_ED_ACK KEGS_EXPAND_INVENTORY_ED_ACK;


#endif // SERV_GOOD_ELSWORD

#ifdef SERV_KOM_FILE_CHECK_ADVANCED
DECL_PACKET( EGS_KOM_FILE_CHECK_LOG_REQ )
{
	std::wstring						m_wstrInvalidKomName;
};

DECL_PACKET( EGS_KOM_FILE_CHECK_LOG_ACK )
{
	std::wstring						m_wstrInvalidKomName;
	int									m_iOK;
};
#endif // SERV_KOM_FILE_CHECK_ADVANCED

#ifdef SERV_SKILL_PAGE_SYSTEM

typedef KEGS_GET_NEXT_BANK_ED_ACK    KEGS_GET_NEXT_SKILL_PAGE_ED_ACK;

DECL_PACKET( EGS_EXPAND_SKILL_PAGE_ACK )
{
	int						m_iOK;
	int						m_iED;		/// ��ų������ Ȯ�� �� ������ ������ �־�� �ϴ� ED
	int						m_iTheNumberOfSkillPagesAvailable;
	int						m_iSPointAvailable;
	int						m_iCSPointAvailable;
	std::vector<int>		m_vecSkillListLearned;
	std::vector< short >	m_vecUnsealedSkillID;
	bool					m_bPayWithED;

	KEGS_EXPAND_SKILL_PAGE_ACK()
		: m_iOK(0), 
		m_iED(0),
		m_iTheNumberOfSkillPagesAvailable(0),
		m_iSPointAvailable(0),
		m_iCSPointAvailable(0),
		m_bPayWithED( true )
	{
		m_vecSkillListLearned.clear();
		m_vecUnsealedSkillID.clear();
	}
};

DECL_PACKET( EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ )
{
	int	m_iSkillPagesNumberDecidedToUse;

	KEGS_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ()
		: m_iSkillPagesNumberDecidedToUse(0)
	{}
};

DECL_PACKET( EGS_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK )
{
	int						m_iOK;
	int						m_iSkillPagesNumberDecidedToUse;
	int						m_iSPointAvailable;
	int						m_iCSPointAvailable;
	KUserSkillPageData		m_kUserSkillPageData;
	//std::map< int, int>		m_mapSkillListLearned;

	KEGS_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK()
		: m_iOK(0),
		m_iSkillPagesNumberDecidedToUse(0), 
		m_iSPointAvailable(0),
		m_iCSPointAvailable(0)
	{}
};

#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_ENTRY_POINT
DECL_PACKET( EGS_CHARACTER_LIST_ACK )
{
    int                     m_iOK;
    std::map< int, int >    m_mapServerGroupUnitSlot; // first = ������( SEnum::SGI_SOLES or SGI_GAIA, second = ���� ��
    std::map< int, std::vector<KUnitInfo> > m_mapServerGroupUnitInfo; // first = ������
    std::wstring m_strUserID;
    KEGS_CHARACTER_LIST_ACK()
        :m_iOK(0)        
    {
        m_mapServerGroupUnitSlot.clear();
        m_mapServerGroupUnitInfo.clear();
        m_strUserID.clear();
    }
};

typedef std::wstring KECH_CHANNEL_CHARACTER_LIST_REQ;
typedef KEGS_CHARACTER_LIST_ACK KEGS_CHARACTER_LIST_1ST_ACK;

DECL_PACKET( EGS_CREATE_NEW_UNIT_REQ )
{
    std::wstring            m_wstrNickName;
    int                     m_iClass;
    int                     m_iServerGroup;

    KEGS_CREATE_NEW_UNIT_REQ()
        :m_wstrNickName(L"")
        ,m_iClass(0)
        ,m_iServerGroup(0)
    {}

};


DECL_PACKET( EGS_ENTRY_POINT_DELETE_UNIT_REQ )
{
    UidType                 m_iUnitUID;
    UidType                 m_iUserUID;
    int                     m_iServerGroup;

    KEGS_ENTRY_POINT_DELETE_UNIT_REQ()
        
        :m_iServerGroup(0)
    {
        m_iUnitUID		= 0;
        m_iUserUID		= 0;
    }
};

DECL_PACKET( EGS_ENTRY_POINT_CHECK_NICK_NAME_REQ )
{
    int                     m_iServerGroup;    
    std::wstring			m_wstrNickName;

    KEGS_ENTRY_POINT_CHECK_NICK_NAME_REQ():
    m_iServerGroup(0),
    m_wstrNickName(L"")
    {
    }
};

typedef KEGS_CHECK_NICK_NAME_ACK KEGS_ENTRY_POINT_CHECK_NICK_NAME_ACK;

DECL_PACKET( EGS_GET_CREATE_UNIT_TODAY_COUNT_ACK )
{
    UidType	             m_iUserUID;
    std::map< int, int > m_mapCreateCharCountToday; // first : server group , second : ���� ĳ���� ���� ��

    KEGS_GET_CREATE_UNIT_TODAY_COUNT_ACK():
    m_iUserUID(0)
    {
        m_mapCreateCharCountToday.clear();
    }


};

DECL_PACKET( EGS_ENTRY_POINT_CHANGE_NICK_NAME_REQ )
{
    int                 m_iServerGroup;    
    UidType             m_iUnitUID;
    std::wstring        m_wstrNickName;
    bool				m_bCheckOnly;

    KEGS_ENTRY_POINT_CHANGE_NICK_NAME_REQ():
        m_iServerGroup(0),
        m_iUnitUID(0),
        m_wstrNickName(L""),
        m_bCheckOnly(false)
    {
    }
};

//DECL_PACKET( EGS_CHANGE_NICK_NAME_ACK )
//{
//    int                 m_iOK;
//    UidType             m_iUnitUID;
//    std::wstring		m_wstrNickName;
//    bool				m_bCheckOnly;
//};


#endif SERV_ENTRY_POINT

#ifdef SERV_NAVER_CHANNELING
//typedef std::string KEGS_GET_NAVER_ACCESS_TOKEN_ACK;
DECL_PACKET( EGS_GET_NAVER_ACCESS_TOKEN_ACK )
{
    int         m_iOK;
    std::string m_strAccessToken;

    KEGS_GET_NAVER_ACCESS_TOKEN_ACK():m_iOK(0),
        m_strAccessToken(std::string())
    {}
};
#endif SERV_NAVER_CHANNELING

#pragma pack( pop )

//{{ 2013.02.27 ��ȿ��	�ؿ� �⺻ ���� �۾� (SERV_GLOBAL_BASE)
#   include "OnlyGlobal/Packet/ClientPacket_Global.h"
//}}