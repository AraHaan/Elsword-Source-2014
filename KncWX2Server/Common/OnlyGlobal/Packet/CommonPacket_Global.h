#pragma once


#pragma pack( push, 1 )

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GLOBAL_BILLING

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
DECL_DATA( KGlobalCashInfo )
{
	// ���� ������ ĳ�ø� ������ �����ؾ� �� �� ���
	
	// ���� [EBP_BuyItem] SP ���� �Ķ���Ϳ� Enum�� ���� ���¶� �����ϸ� �ȵ�
	enum GLOBAL_CASH_TYPE
	{
		GCT_PUBLISHER_CASH			= 0,		// �Ϲ����� �ۺ��� ĳ�� (�������� ĳ�ø� ���� ��� �̰����� ����. ���� ĳ�� ������ �ٸ����� ���ҵ� ������ ĳ�� ���� �� �ʿ� ���� ��� �̰����� ó����)
		GCT_KOG_ELSWORD_CASH		= 1,		// KOG���� �����ϴ� ���ҵ�� ��ȯ�� ĳ�� (�߱� & �����ƽþ� ���)
		GCT_KOG_ELSWORD_BONUS_POINT	= 2,		// KOG���� �����ϴ� ���ҵ� �߰� ���ʽ� ����Ʈ (�߱��� ���)
		GCT_PUBLISHER_ELSWORD_CASH	= 3,		// �ۺ��� �� ���ҵ�� ��ȯ�� ĳ��
		GCT_TOTAL_NUM

		// �������� PUT, GET�� �����ؾ���
		//GCT_PUBLISHER_BONUS_CASH	= 4,		// �ۺ��� �� ���ʽ� ĳ�� (���� ��뱹�� ��� �ּ�ó��)
	};

	unsigned long				m_ulCash[GCT_TOTAL_NUM];

	KGlobalCashInfo()
	{
		for(int i=0; i < GCT_TOTAL_NUM; i++ )
		{
			m_ulCash[i]	= 0;
		}
	}
};
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES


DECL_DATA( KBillProductCategory )
{
	char                m_cCategoryNo;
	std::wstring        m_wstrCategoryName;
	char                m_cParentCategoryNo;
	char                m_cDisplayNo;
};

DECL_DATA( KBillProductInfo )
{
	int                 m_iProductNo;
	int                 m_iProductID;
	bool                m_bSale;
	bool                m_bEvent;
	bool                m_bShow;
	bool                m_bIsCashInvenSkip;
	std::wstring        m_wstrProductName;
	char                m_cCategoryNo;
	int                 m_iSalePrice;
	int                 m_iRealPrice;
	char                m_cPeriod;			// ��� �Ⱓ
	char                m_cQuantity;
	bool                m_bEnableGift;
	char                m_cLimitLevel;
	std::wstring        m_wstrEndDate;		// �Ǹ� �Ⱓ
#ifdef SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION
	std::set< std::pair< __int64, bool > >	m_setSalePeriod;
#endif SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION
};

DECL_DATA( KBillProductAttribute )
{
	int                 m_iProductNo;
	std::vector< int >  m_vecAttribute;
};

DECL_DATA( KBillPackageInfo )
{
	int                 m_iPackageNo;	// TranNo ��
	std::vector< int >  m_vecProductNo;

#ifdef SERV_GLOBAL_CASH_PACKAGE
	unsigned long       m_ulProductNo;
	int			        m_iProductID;
	std::wstring        m_wstrProductID;
	//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
	short				m_sProductExpire;
	//}}
	unsigned short      m_usProductPieces;
	std::vector< int >  m_vecSocketOption;
	//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
	short GetProductExpire() const { return ( m_sProductExpire == -1 ? 0 : m_sProductExpire ); }
	//}}
#endif //SERV_GLOBAL_CASH_PACKAGE
};

DECL_DATA( KBillBuyInfo )
{
	int                 m_iProductNo;
	int                 m_iPoint;
	int                 m_iProductID;
	unsigned short		m_usOrderQuantity;
	int                 m_iCategory;
	std::wstring        m_wstrMemo;


	std::wstring		m_wstrOrderID;


	KBillBuyInfo()
	{
		m_iProductNo		= 0;
		m_iPoint			= 0;
		m_iProductID		= 0;
		m_usOrderQuantity	= 0;
		m_iCategory			= 0;
		m_wstrMemo			= L"";

		m_wstrOrderID		= L"";
	}
};

DECL_DATA( KBillOrderInfo )
{
	//{{ ����� : [2010/2/10/] //	ĳ�� ������ ���޽� Sell Log �߰�
	enum BUY_CASH_ITEM_TYPE
	{
		BCIT_INVALID	= 0,
		BCIT_BUY_ITEM,			//	ĳ�� ������ ����
		BCIT_COUPON,			//	����
		BCIT_GIFT,				//	����
	};
	//}} ����� : [2010/2/10/] //	ĳ�� ������ ���޽� Sell Log �߰�

	UidType             m_iTransNo;
	std::wstring        m_wstrOrderID;
	KBillProductInfo    m_kBillProductInfo;
	std::vector< int >  m_vecSocketOption;

	byte				m_byteType;
	std::wstring		m_wstrSenderNickName;
	std::wstring		m_wstrSenderPresentMessage;
	//{{ ����� : [2010/8/19/] //	���� ��� ����
	UidType				m_iFromUnitUID;
	//}} ����� : [2010/8/19/] //	���� ��� ����
#ifdef SERV_GLOBAL_CASH_PACKAGE
	BYTE                m_byteProductKind;
#endif //SERV_GLOBAL_CASH_PACKAGE

	KBillOrderInfo()
	{
		m_iTransNo = -1;
		m_byteType = BCIT_INVALID;
#ifdef	SERV_GLOBAL_CASH_PACKAGE
		m_byteProductKind = 0;
#endif	SERV_GLOBAL_CASH_PACKAGE
	}
};
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
DECL_DATA( kDisCountItemInfo )
{
	int					m_DisCountProNo;
	int					m_BasicProNo;
	std::vector< int >	m_DisCountKeepItem;
};
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP
#endif SERV_GLOBAL_BILLING
//////////////////////////////////////////////////////////////////////////


#ifdef SERV_NEW_PVPROOM_PROCESS

DECL_PACKET( EGS_CHANGE_PVP_ROOM_PUBLIC_REQ )
{
	bool									m_bPublic;
	std::wstring							m_wstrPassword;
};

DECL_PACKET( EGS_CHANGE_PVP_ROOM_PUBLIC_ACK )
{
	bool									m_bPublic;
	int										m_iOK;
};

DECL_PACKET( EGS_CHANGE_PVP_ROOM_PUBLIC_NOT )
{
	bool									m_bPublic;
};

typedef KEGS_CHANGE_PVP_ROOM_PUBLIC_REQ KERM_CHANGE_PVP_ROOM_PUBLIC_REQ;
typedef KEGS_CHANGE_PVP_ROOM_PUBLIC_ACK KERM_CHANGE_PVP_ROOM_PUBLIC_ACK;

DECL_PACKET( EGS_CHANGE_PVP_ROOM_NAME_REQ )
{
	std::wstring							m_wstrRoomName;
};

DECL_PACKET( EGS_CHANGE_PVP_ROOM_NAME_ACK )
{
	std::wstring							m_wstrRoomName;
	int										m_iOK;
};

DECL_PACKET( EGS_CHANGE_PVP_ROOM_NAME_NOT )
{
	std::wstring							m_wstrRoomName;
};

typedef KEGS_CHANGE_PVP_ROOM_NAME_REQ KERM_CHANGE_PVP_ROOM_NAME_REQ;
typedef KEGS_CHANGE_PVP_ROOM_NAME_ACK KERM_CHANGE_PVP_ROOM_NAME_ACK;

#endif SERV_NEW_PVPROOM_PROCESS

#ifdef SERV_PC_BANG_TYPE
DECL_DATA( KPcBangReward )
{
	int		m_iPcBangType;
	float	m_fEXPRate;
	float	m_fEDRate;
	float	m_fVSPointRate;
	bool	m_bSpiritFree;
	float	m_fPetEXPRate;
	int		m_iAdditionalHenirReward;
	bool	m_bSecretDungeonFree;
	bool	m_bAvatar;
	bool	m_bTitle;

	KPcBangReward()
	{
		m_iPcBangType				= -1;
		m_fEXPRate					= 0.0f;
		m_fEDRate					= 0.0f;
		m_fVSPointRate				= 0.0f;
		m_bSpiritFree				= false;
		m_fPetEXPRate				= 0.0f;
		m_iAdditionalHenirReward	= 0;
		m_bSecretDungeonFree		= false;
		m_bAvatar					= false;
		m_bTitle					= false;
	}

	COPYCON_ASSIGNOP( KPcBangReward, right )
	{
		m_iPcBangType				= right.m_iPcBangType;
		m_fEXPRate					= right.m_fEXPRate;
		m_fEDRate					= right.m_fEDRate;
		m_fVSPointRate				= right.m_fVSPointRate;
		m_bSpiritFree				= right.m_bSpiritFree;
		m_fPetEXPRate				= right.m_fPetEXPRate;
		m_iAdditionalHenirReward	= right.m_iAdditionalHenirReward;
		m_bSecretDungeonFree		= right.m_bSecretDungeonFree;
		m_bAvatar					= right.m_bAvatar;
		m_bTitle					= right.m_bTitle;

		return *this;
	}
};
#endif SERV_PC_BANG_TYPE

//{{ 2011. 12. 09	�ڱ���		������ ����Ʈ
#ifdef SERVER_GROUP_UI_ADVANCED
DECL_DATA( KServerGroupInfo )
{
	int							m_iOrder;
	int							m_iServerGroupUID;
	std::wstring				m_wstrServerGroupName;
	std::wstring				m_kServerIP;
	u_short						m_usMasterPort;
	u_short						m_usNCUDPPort;
	bool						m_bEnable;
};
#endif SERVER_GROUP_UI_ADVANCED
//}}

#ifdef SERV_CATCH_HACKUSER_INFO
DECL_PACKET( EGS_CATCH_HACKUSER_INFO_NOT )
{
	UidType				m_iUserUID;
	UidType				m_iUnitUID;
	int					m_iCrashType;
};
#endif SERV_CATCH_HACKUSER_INFO

#ifdef	SERV_SHARING_BANK_TEST
DECL_DATA( KTradeShareItemResult )
{
	UidType								m_iBeforeItemUID;
	UidType								m_iNewItemUID;

	std::wstring						m_wstrRegDate;

	KTradeShareItemResult()
	{
		m_iBeforeItemUID				= 0;
		m_iNewItemUID					= 0;

		m_wstrRegDate				= L"";
	}
};
#endif	SERV_SHARING_BANK_TEST

//{{ 2013. 05. 14	������	������ ����
#ifdef SERV_BLOCK_LIST
DECL_DATA( KBlockInfo )
{
	enum BLOCK_TYPE
	{
		BT_TRADE_BLOCK = 0,
		BT_ACCOUNT_BLOCK,
		BT_IP_BLOCK,
		BT_DLL_BLOCK,

		BT_MAX,
	};

    UidType							m_iBlockUID;
	char							m_cBlockType;
	UidType							m_iBlockTarget;
	std::wstring					m_wstrBlockTarget;	
	std::wstring					m_wstrStartDate;
	std::wstring					m_wstrEndDate;

	KBlockInfo()
	{
		m_iBlockUID = 0;
		m_cBlockType = 0;
		m_iBlockTarget = 0;
	}
};
#endif SERV_BLOCK_LIST
//}}

#ifdef SERV_ANTI_ADDICTION_SYSTEM
DECL_DATA( KGiantAccountType )
{
	enum TYPE_MASK
	{ 
		NORMAL_USER			= 0,		// 00: �Ϲ� ����
		SPECIAL_USER		= 2,		// 01: Ư�� ����
		VIP_USER			= 4,		// 02: VIP ����
		EVENT_PLAYER		= 8,		// 03: �̺�Ʈ �÷��̾�
		ADULT_PLAYER		= 16,		// 04: ���� �÷��̾�
		BLACKLIST_USER		= 32,		// 05: ������Ʈ ����

		DEALER				= 512,		// 10: ����?
		WEB_DEALER			= 1024,		// 11: ������?
	};

	KGiantAccountType()
	{
		m_uiType = 0;
	}

	bool			IsType(TYPE_MASK mask)			{ return ( m_uiType & mask ) ? true : false; }

	unsigned int	m_uiType;
};
#endif SERV_ANTI_ADDICTION_SYSTEM

#ifdef SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG
DECL_DATA( KRandomItemResult )
{
	int		m_iItemCount;
	bool	m_bIsSealItem;
	double  m_dRate;
	double  m_dItemTotalCount;
	bool	m_bIsNotifyMsgAttration;

};
#endif//SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG

#ifdef SERV_ID_NETMARBLE_PCBANG
DECL_PACKET( ECH_PCBANG_IP_AND_MAC_INFO_NOT )
{
	std::wstring							m_wstrPublicIP;
	std::wstring							m_wstrMacInfo;
};

typedef KECH_PCBANG_IP_AND_MAC_INFO_NOT			 KDBE_PCBANG_IP_AND_MAC_INFO_NOT;
#endif //SERV_ID_NETMARBLE_PCBANG

#ifdef SERV_COUNTRY_PH
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef std::vector<uint8> dataBuffer_t;
typedef std::string string_t;
DECL_DATA( kGarenaUserTokInfo_t ) // 2013.07.31 lygan_������ // �����ƽþ� �ۺ��ſ��� ���� �������� �������� ����ü
{
	uint32				m_Uid;
	std::string			m_Username;
	uint32				m_Timestamp;
	uint16				m_GcaType;
	uint8				m_GoldMemberType;
	uint8				m_IpBonus;
	std::string			m_Reserved;
};
#endif //SERV_COUNTRY_PH

//{{ 2013. 09. 23	������	�Ϻ� �̺�Ʈ �߰�DB�۾�
//#ifdef SERV_RELAY_DB_CONNECTION
DECL_DATA( KJPEventRewardInfo )
{
	__int64								m_iRID;
	std::wstring						m_wstrUserID;
	UidType								m_iUserUID;
	UidType								m_iUnitUID;
	int									m_iRewardID;

	KJPEventRewardInfo()
	{
		m_iRID = 0;
		m_iUserUID = 0;
		m_iUnitUID = 0;
		m_iRewardID = 0;
	}
};
//#endif SERV_RELAY_DB_CONNECTION
//}}

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
DECL_DATA( KEventCharacterRanking )
{
	// �����϶� 1�� ���ҵ� , 2�� ���̺� , 3�� û
	// �����϶� 1�� ���̻� , 2�� ���� , 3�� �̺� , 4�� �ƶ�
	int								m_iDungeonCharacter1;
	int								m_iDungeonCharacter2;
	int								m_iDungeonCharacter3;
	int								m_iDungeonCharacter4;
	int								m_iPVPCharacter1;
	int								m_iPVPCharacter2;
	int								m_iPVPCharacter3;
	int								m_iPVPCharacter4;
	int								m_iFieldCharacter1;
	int								m_iFieldCharacter2;
	int								m_iFieldCharacter3;
	int								m_iFieldCharacter4;

	KEventCharacterRanking()
	{
		m_iDungeonCharacter1 = 0;
		m_iDungeonCharacter2 = 0;
		m_iDungeonCharacter3 = 0;
		m_iDungeonCharacter4 = 0;
		m_iPVPCharacter1 = 0;
		m_iPVPCharacter2 = 0;
		m_iPVPCharacter3 = 0;
		m_iPVPCharacter4 = 0;
		m_iFieldCharacter1 = 0;
		m_iFieldCharacter2 = 0;
		m_iFieldCharacter3 = 0;
		m_iFieldCharacter4 = 0;
	}
};
#endif //SERV_EVENT_CHARACTER_QUEST_RANKING


#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
DECL_DATA( EVENT_DATA ) // 2013.10.31 darkstarbt_������ // �������� ������ CommonPacket.h �� ����� �����ſ� �ֽ�ȭ ���ϰ� ���� ���� �����, ���� ���� ���Ⲩ �״�ε����
{
	float	m_fEXPRate;
	float	m_fVPRate;
	float	m_fEDRate;

	float	m_fRankingPointRate;


	int		m_iDungeonID;
	int		m_iDungeonMode;
	int		m_iUnitLevel;		
	char	m_cUnitType;
	char    m_cUnitClass;
	int		m_iPartyNum;

	// ������ �̺�Ʈ
	float	m_fEventTime;
	int		m_iEventReward;
	std::map< std::pair< int, int >, int >	m_mapMinMaxReward;

	bool	m_bGiveOnce;
	bool	m_bDelEndEvent;
	bool	m_bPcBangEvent;
	bool	m_bDirectReward;
	bool	m_bAccountEvent;
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
		float	m_fDropRate;
#else // SERV_DROP_EVENT_RENEWAL	
	int		m_iDropCount;
#endif // SERV_DROP_EVENT_RENEWAL
	bool	m_bWithPlayPcBang;

	int		m_iChannelCode;
	int		m_iBF_Team;

	int		m_iHenirRewardEventCount;
	bool	m_bHenirRewardUnLimited;


	int		m_iServerGroup;

	bool	m_bAccumulateEXP;

	//{{ 2012. 03. 28	�ڼ���	��ӷ� ���� �̺�Ʈ ���� Merge �۾� ( 2012.03.20 lygan_������ )
#ifdef DROPEVENT_RENEWAL
	int		m_iStartLevel;
	int		m_iEndLevel;
#endif DROPEVENT_RENEWAL
	//}}

	bool	m_bRepeatEvent;

	bool	m_bRepeatRewardEvent;
	int		m_iBeforeEventID;

	//{{ 2012. 12. 17	������	�ƶ� ��Ƽ �÷��� ���ʽ� ����ġ
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	bool	m_bPlayWithCharEXP;
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	//}}

	int		m_iEventGroupID;

	float	m_fQuestItemDropRate;

#ifdef SERV_ADVERTISEMENT_EVENT
	std::wstring	m_wstrAdvertisementURL;
#endif SERV_ADVERTISEMENT_EVENT


	int		m_iEventItemID;

	int		m_iCustomEventID;

#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// ���볯¥: 2013-04-11
	float	m_fDefenseDungeonOpenRate;
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT

#ifdef	SERV_CRITERION_DATE_EVENT// ���볯¥: 2013-04-11
	byte	m_byteCriterionUserType;
	byte	m_byteCriterionPresentUserType;
	CTime	m_tCriterionDateBegin;
	CTime	m_tCriterionDateEnd;
#endif	// SERV_CRITERION_DATE_EVENT

#ifdef SERV_STEAM_USER_CONNECT_EVENT
	bool			m_bOnlySteamUser;
	bool			m_bOnlyNotSteamUser;
#endif //SERV_STEAM_USER_CONNECT_EVENT

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
	bool	m_bNewUnitEvent;
	bool	m_bNewUnitEvent2;
	bool	m_bCurrentUnitEvent;
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
		int		m_iUnitClassLevel;
#endif SERV_UNIT_CLASS_LEVEL_EVENT

#ifdef SERV_HENIR_REWARD_EVENT// �۾���¥: 2013-09-09	// �ڼ���
		bool	m_bAccumulate;
#endif // SERV_HENIR_REWARD_EVENT

	EVENT_DATA()
		: m_fEXPRate(0.f)
		, m_fVPRate(0.f)
		, m_fEDRate(0.f)
		, m_fRankingPointRate(0.f)
		, m_iDungeonID(0)
		, m_iDungeonMode(0)
		, m_iUnitLevel(0)
		, m_cUnitType(0)
		, m_cUnitClass(0)
		, m_iPartyNum(0)
		, m_fEventTime(0.f)
		, m_iEventReward(0)
		, m_bGiveOnce(true)
		, m_bDelEndEvent(false)
		, m_bPcBangEvent(false)
		, m_bDirectReward(false)
		, m_bAccountEvent(false)
#ifdef SERV_DROP_EVENT_RENEWAL// �۾���¥: 2013-09-09	// �ڼ���
		, m_fDropRate(0.0f)
#else // SERV_DROP_EVENT_RENEWAL
		, m_iDropCount(0)
#endif // SERV_DROP_EVENT_RENEWAL
		, m_bWithPlayPcBang(false)
		, m_iChannelCode(0)
		, m_iBF_Team(0)
		, m_iHenirRewardEventCount(0)
		, m_bHenirRewardUnLimited(false)
		, m_iServerGroup(-1)
		, m_bAccumulateEXP(false)
		//{{ 2012. 03. 28	�ڼ���	��ӷ� ���� �̺�Ʈ ���� Merge �۾� ( 2012.03.20 lygan_������ )
#ifdef DROPEVENT_RENEWAL
		, m_iStartLevel(0)
		, m_iEndLevel(0)
#endif DROPEVENT_RENEWAL
		//}}
		, m_bRepeatEvent(false)
		, m_bRepeatRewardEvent(false)
		, m_iBeforeEventID(-1)
		//{{ 2012. 12. 17	������	�ƶ� ��Ƽ �÷��� ���ʽ� ����ġ
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		, m_bPlayWithCharEXP(false)
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		//}}
		, m_iEventGroupID(-1)
		, m_fQuestItemDropRate(-1.f)
		, m_iEventItemID(0)
		, m_iCustomEventID(0)
#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// ���볯¥: 2013-04-11
		, m_fDefenseDungeonOpenRate(1.0f)
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT
#ifdef	SERV_CRITERION_DATE_EVENT// ���볯¥: 2013-04-11
		, m_byteCriterionUserType(0)
		, m_byteCriterionPresentUserType(0)
#endif	// SERV_CRITERION_DATE_EVENT
#ifdef SERV_STEAM_USER_CONNECT_EVENT
		, m_bOnlySteamUser(false)
		, m_bOnlyNotSteamUser(false)
#endif //SERV_STEAM_USER_CONNECT_EVENT
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		, m_bNewUnitEvent(false)
		, m_bNewUnitEvent2(false)
		, m_bCurrentUnitEvent(false)
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
		, m_iUnitClassLevel(-1)
#endif SERV_UNIT_CLASS_LEVEL_EVENT
#ifdef SERV_HENIR_REWARD_EVENT// �۾���¥: 2013-09-09	// �ڼ���
		, m_bAccumulate(false)
#endif // SERV_HENIR_REWARD_EVENT
	{
	}

};

DECL_DATA( KRewardData )
{	
	int				m_iRewardItemID;
	int				m_iQuantity;
	short			m_sPeriod;

	KRewardData(){};
	KRewardData( const KRewardData& t ) { *this = t; }    // copy constructor
	KRewardData& operator=( const KRewardData& t )       // assign operator
	{			
		m_iRewardItemID	= t.m_iRewardItemID;
		m_iQuantity		= t.m_iQuantity;
		m_sPeriod		= t.m_sPeriod;

		return *this;
	}
};

DECL_PACKET( ESG_REWARD_DB_DATA_NOT )
{
	std::map< int, std::vector< KRewardData > > m_mapTotalRewardData;
};


#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#ifdef SERV_CUBE_IN_ITEM_MAPPING
DECL_DATA( kRandomItemMappingToInfoClient )
{
	int		 m_iBeforeGroupID;
	int		 m_iAfterGroupID;
	bool	 m_bCheckMapping;
	std::wstring	 m_tStartTime;
	std::wstring	 m_tEndTime;

	kRandomItemMappingToInfoClient()
	{
		m_iBeforeGroupID = 0;
		m_iAfterGroupID = 0;
		m_bCheckMapping = false;
	}
};

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
DECL_DATA( KRandomItemMappingToInfoServer )
{
	int		 m_iIndex;
	int		 m_iCubeID;
	int		 m_iKeyItemID;
	int		 m_iBeforeGroupID;
	int		 m_iAfterGroupID;
	std::wstring	 m_tStartTime;
	std::wstring	 m_tEndTime;

	KRandomItemMappingToInfoServer()
	{
		m_iIndex			= 0;
		m_iCubeID			= 0;
		m_iKeyItemID		= 0;
		m_iBeforeGroupID	= 0;
		m_iAfterGroupID		= 0;
		m_tStartTime		= L"";
		m_tEndTime			= L"";
	}
};
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
#endif //SERV_CUBE_IN_ITEM_MAPPING

#ifdef SERV_USE_GM_TOOL_INFO
DECL_DATA( KItemName )
{
	std::wstring				m_wstrKRItemName;
	std::wstring				m_wstrTransItemName;
};
#endif //SERV_USE_GM_TOOL_INFO

//{{ 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���
#ifdef SERV_GLOBAL_MISSION_MANAGER
DECL_DATA( KGlobalMissionReward )
{
	int									m_iRewardType;					// ���� Ÿ��(����, �����̺�Ʈ ��)
	int									m_iRewardDurationTime;			// ���� ���� �ð�(��)
	int									m_iScriptID;					// ���� ��ũ��Ʈ ID

	KGlobalMissionReward()
	{
		m_iRewardType = 0;
		m_iRewardDurationTime = 0;
		m_iScriptID = 0;
	}
};

DECL_DATA( KGlobalMissionInfo )
{
	bool									m_bActive;						// ���� ������ ����
	std::wstring							m_wstrMissionStartTime;			// ���� �۷ι� �̼� ���� �ð�
	int										m_iDurationMinute;				// �۷ι� �̼� ���� �ð�(��)
	int										m_iRemainSecond;				// �۷ι� �̼� ���� �ð�(��)

	int										m_iMissionType;					// �۷ι� �̼� Ÿ��
	int										m_iClearCondition;				// �۷ι� �̼� Ŭ���� ����
	int										m_iClearCount;					// ��ǥ Ŭ���� ��ġ
	int										m_iNowCount;					// ���� Ŭ���� ��ġ


	KGlobalMissionInfo()
	{
		m_bActive = false;
		m_wstrMissionStartTime = L"";
		m_iDurationMinute = 0;
		m_iRemainSecond = 0;

		m_iMissionType = 0;
		m_iClearCondition = 0;
		m_iClearCount = 0;
		m_iNowCount = 0;
	}
};
#endif SERV_GLOBAL_MISSION_MANAGER
//}} 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING
DECL_DATA( KPacketGetItemOnOff )
{
	int						m_iType;
	int						m_iItemID;
	std::wstring			m_wstrStartTime;
	std::wstring			m_wstrEndTime;

	KPacketGetItemOnOff()
	{
		m_iType = 0;
		m_iItemID = 0;
		m_wstrStartTime = L"";
		m_wstrEndTime = L"";
	}
};

typedef KPacketOK								KDBE_GET_TIME_CONTROL_ITME_LIST_NOT;
typedef KPacketOK								KEGS_GET_TIME_CONTROL_ITME_LIST_NOT;
#endif SERV_ITEM_ACTION_BY_DBTIME_SETTING


#ifdef SERV_STRING_FILTER_USING_DB
DECL_DATA( KStringFilterInfo )
{
	short			m_sStringFilterType;
	std::wstring	m_wstrFilter;

	KStringFilterInfo()
	{
		m_sStringFilterType = 0;
		m_wstrFilter = L"";
	}
};
typedef std::vector< KStringFilterInfo > StringFilterVector;
#endif //SERV_STRING_FILTER_USING_DB

#pragma pack( pop )