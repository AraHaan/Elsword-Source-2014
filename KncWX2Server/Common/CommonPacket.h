#pragma once

// EffectTool ���带 ���� undef �ʿ�
#ifdef EFFECT_TOOL
#undef SERV_PET_SYSTEM
#undef SERV_SHIP_DUNGEON
#undef SERV_WORLD_TRIGGER_RELOCATION
#undef SERV_TOONILAND_CHANNELING_EVENT
#undef SERV_DLL_LIST_CHECK_BEFOR_LOADING
#undef SERV_MACHINE_ID_BLOCK
#undef SERV_MACHINE_ID_DUPLICATE_CHECK
#undef SERV_MACHINE_ID_BLOCK_UPDATE
#undef SERV_MACHINE_ID_CHECK_BEFOR_LOADI
#endif //EFFECT_TOOL

#include "ServerDefine.h"
#include <Serializer/SerializeMacro.h>
#include "KncUidType.h"
//#include "ServerDefine.h"

#pragma pack( push, 1 )

 // ���� ������ �ؿ��� �߰�
//{{ 2011.11.02     ��μ�    �������� SN �߱� �� ��ȿ�� �˻�
//#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
#define			SERVER_SN					12
//#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
//}}

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
DECL_DATA( KWeddingHallInfo )
{
	int				m_iWeddingUID;				// ��ȥ�� uid
	char			m_cWeddingHallType;			// ��ȥ���� Ÿ��
	char			m_cOfficiantNPC;			// �ַ�npc Ÿ��
	UidType			m_iGroom;					// �Ŷ�
	UidType			m_iBride;					// �ź�
	std::wstring	m_wstrWeddingDate;			// ��ȥ �Ͻ�
	__int64			m_tWeddingDate;				// ��ȥ �Ͻ�
	std::wstring	m_wstrWeddingMsg;			// �޽���
	UidType			m_iRoomUID;					// RoomUID
	bool			m_bSuccess;					// ��ȥ ���� ����
	bool			m_bDelete;					// ���� ����

	CON_COPYCON_ASSIGNOP( KWeddingHallInfo, right )
	{
		m_iWeddingUID = right.m_iWeddingUID;
		m_cWeddingHallType = right.m_cWeddingHallType;
		m_cOfficiantNPC = right.m_cOfficiantNPC;
		m_iGroom = right.m_iGroom;
		m_iBride = right.m_iBride;
		m_wstrWeddingDate = right.m_wstrWeddingDate;
		m_tWeddingDate = right.m_tWeddingDate;
		m_wstrWeddingMsg = right.m_wstrWeddingMsg;
		m_iRoomUID = right.m_iRoomUID;
		m_bSuccess = right.m_bSuccess;
		m_bDelete = right.m_bDelete;

		return *this;
	}
};

DECL_DATA( KWeddingItemInfo )
{
	UidType			m_iItemUID;					// item uid
	int				m_iWeddingUID;				// ��ȥ�� uid
	char			m_cWeddingHallType;			// ��ȥ���� Ÿ��
	char			m_cOfficiantNPC;			// �ַ�npc Ÿ��
	UidType			m_iGroom;					// �Ŷ�
	UidType			m_iBride;					// �ź�
	std::wstring	m_wstrGroom;				// �Ŷ�
	std::wstring	m_wstrBride;				// �ź�
	std::wstring	m_wstrWeddingDate;			// ��ȥ �Ͻ�
	std::wstring	m_wstrWeddingMsg;			// �޽���

	KWeddingItemInfo()
	{
		m_iItemUID = 0;
		m_iWeddingUID			= 0;
		m_cWeddingHallType		= 0;
		m_cOfficiantNPC			= 0;
		m_iGroom				= 0;
		m_iBride				= 0;
	}
};

DECL_DATA( KRelationshipInfo )
{
	char						m_cRelationshipType;
	UidType						m_iOtherUnitUID;
	std::wstring				m_wstrOtherNickName;
	std::wstring				m_wstrLoveWord;
	__int64						m_tDate;
	__int64						m_tLastReward;
	char						m_cRewardTitleType;

	KRelationshipInfo()
	{
		m_iOtherUnitUID = 0;
		m_cRelationshipType = 0;
		m_tDate  = 0;
		m_tLastReward = 0;
		m_cRewardTitleType = 0;
	}
};
#endif SERV_RELATIONSHIP_SYSTEM
//}

//{{ 2012. 1. 9	������		DB���� ��ȣ ����ȭ
#ifdef SERV_DB_CONNECTION_SECURITY
DECL_DATA( KDBConnectionInfo )
{
    int						m_iDBType;
	std::wstring			m_wstrConnectionString;
	int						m_iThreadCount;

	KDBConnectionInfo()
	{
		m_iDBType = 0;
		m_iThreadCount = 0;
	}
};
#endif SERV_DB_CONNECTION_SECURITY
//}}

DECL_DATA( KNetAddress )
{
    std::wstring    m_wstrIP;
    unsigned short  m_usPort;

	CON_COPYCON_ASSIGNOP( KNetAddress, right )
	{
		m_wstrIP = right.m_wstrIP;
		m_usPort = right.m_usPort;

        return *this;
	}
};

DECL_DATA( KNexonAccountInfo )
{
	//{{ 2011. 08. 09	������	���� ���� ä�θ� �̺�Ʈ
#ifdef SERV_TOONILAND_CHANNELING
	enum CHANNELING_ENUM
	{
		CE_NONE				 = -1,
		CE_NEXON_ACCOUNT	 = 0,
		CE_TOONILAND_ACCOUNT = 3,
        CE_NAVER_ACCOUNT     = 6, // NGM, SSO, ȸ��
        CE_NAVER_SESSION    = 14, // �˴ٿ�, PC��
	};
#endif SERV_TOONILAND_CHANNELING
	//}}

#ifdef SERV_CHANNELING_USER_MANAGER // SERV_JAPAN_CHANNELING
	enum JAPAN_CHANNELING_ENUM
	{
		JCE_NONE		= -1,
		JCE_NHN			= 0,
		JCE_MSN			= 1,
		JCE_NICO		= 2,
		JCE_NEXON		= 3,
		JCE_MAX			= 4,
	};
#endif SERV_CHANNELING_USER_MANAGER // SERV_JAPAN_CHANNELING

	//{{  2011.11.08     ��μ�    ���������� ���� ���θ�� �̺�Ʈ
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	enum BF_EVENT_TEAM
	{
		BFET_NONE				= 0,
		BFET_ELSWORD			= 1,
		BFET_BUBBLE_FIGHTER		= 2,
	};
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	//}}


    std::wstring            m_wstrID;
    std::wstring            m_wstrLoginIP;
    bool                    m_bSex;
    UINT32                  m_uiAge;
	UINT32                  m_uiNexonSN;
	UINT32                  m_uiPwdHash;
	UINT32                  m_uiSsnHash;
	UINT32                  m_uiFlag0;
	UINT32                  m_uiFlag1;
	//{{ 2012. 06. 28	��μ�       ���� ����� ATL 1.0 -> 2.0 ����
#ifdef SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
	bool					m_bNewMembership;
	UINT8					m_uMainAuthLevel;			// ATL ���� ����
	UINT8					m_uSubAuthLevel;			// ATL ���� ����
#endif SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
	//}}
    
	//{{ 2010. 06. 22  ������	OTP����
#ifdef SERV_OTP_AUTH
	std::wstring			m_wstrNationCode;
	std::wstring			m_wstrMeta;
	UINT8					m_uSecureCode;
	//{{ 2011. 07. 27    ��μ�    ���Ϸ��� ä�θ�
//#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
	UINT8					m_uChannelCode;
	std::wstring			m_wstrChannelUID;
//#else
//	UINT8					m_uRegisteredPC;
//#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
	//}}
#else
	UINT32					m_uiServerHash;
	std::wstring			m_wstrServerName;
#endif SERV_OTP_AUTH
	//}}
	byte					m_byteGuestUser;
	//{{  2011.11.08     ��μ�    ���������� ���� ���θ�� �̺�Ʈ
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	int						m_iBF_Team;
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	//}}


    KNexonAccountInfo()
    {
        m_uiNexonSN		= 0;
        m_bSex			= false;
        m_uiAge			= 0;
		m_uiPwdHash		= 0;
		m_uiSsnHash		= 0;
		m_uiFlag0		= 0;
		m_uiFlag1		= 0;
		//{{ 2012. 06. 28	��μ�       ���� ����� ATL 1.0 -> 2.0 ����
#ifdef SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
		m_bNewMembership = false;
		m_uMainAuthLevel = 0;			// ATL ���� ����
		m_uSubAuthLevel = 0;			// ATL ���� ����
#endif SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
		//}}
       
		//{{ 2010. 06. 22  ������	OTP����
#ifdef SERV_OTP_AUTH		
		m_uSecureCode	= 0;
		//{{ 2011. 07. 27    ��μ�    ���Ϸ��� ä�θ�
#ifdef SERV_TOONILAND_CHANNELING
//#ifdef SERV_JAPAN_CHANNELING 
		m_uChannelCode	= 0;		// ä�θ� ���� ���� �� 0
//#else // SERV_JAPAN_CHANNELING
//		m_uChannelCode	= CE_NEXON_ACCOUNT;
//#endif // SERV_JAPAN_CHANNELING
#else
		m_uRegisteredPC	= 0;
#endif SERV_TOONILAND_CHANNELING
		//}}
#else
		m_uiServerHash	= 0;
#endif SERV_OTP_AUTH
		//}}
		m_byteGuestUser = 0;
		//{{  2011.11.08     ��μ�    ���������� ���� ���θ�� �̺�Ʈ
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		m_iBF_Team		= 0;
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		//}}
    };
};

DECL_DATA( KUserAuthenticateReq )
{
    bool                    m_bDebugAuth;
    std::wstring            m_wstrUserID;
    std::wstring            m_wstrPasswd;
	//{{ 2010. 06. 22  ������	OTP����
#ifdef SERV_OTP_AUTH
	std::wstring			m_wstrHWID;
#endif SERV_OTP_AUTH
	//}}
	//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	std::string				m_strMachineID;
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}
	//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	int						m_kChannelRandomKey;
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}
	//{{  2012. 1. 16	��μ�		���Ӽ������� Server SN �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	unsigned char			m_charServerSN[SERVER_SN];
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	//}}
	//{{ 2012. 03. 27	��μ�		�ӽ� �ð��� �˻�����(50�� �̻� ���̳��� ����͸�)
#ifdef SERV_CHECK_MACHINE_LOCALTIME
	std::wstring			m_wstrClientTime;
#endif SERV_CHECK_MACHINE_LOCALTIME
	//}}

//#ifdef SERV_CHANNELING_USER_MANAGER // SERV_JAPAN_CHANNELING
	int					m_iChannelingCode;
//#endif SERV_CHANNELING_USER_MANAGER

	//{{ 2013.03.05 ��ȿ��		KOG_OTP ���� �� �籸��
#ifdef SERV_KOG_OTP_VERIFY
	bool					m_bManualLogin;
#endif SERV_KOG_OTP_VERIFY
	//}}

#ifdef SERV_COUNTRY_TH	
	std::wstring            m_wstrSocketID;
#endif // SERV_COUNTRY_TH

#ifdef SERV_STEAM
	bool					m_bSteamClient;
#endif //SERV_STEAM
#ifdef SERV_CHANNELING_AERIA
	bool					m_bAeriaClient;
#endif //SERV_CHANNELING_AERIA

	KUserAuthenticateReq()
	{
//#ifdef SERV_CHANNELING_USER_MANAGER // SERV_JAPAN_CHANNELING
		m_iChannelingCode = 0;	// ä�θ� ���� ���� Default �� 0
//#endif SERV_CHANNELING_USER_MANAGER // SERV_JAPAN_CHANNELING

#ifdef SERV_KOG_OTP_VERIFY
		m_bManualLogin = false;
#endif // SERV_KOG_OTP_VERIFY

#ifdef SERV_COUNTRY_TH	
		m_wstrSocketID = L"";
#endif // SERV_COUNTRY_TH

#ifdef SERV_STEAM
		m_bSteamClient = false;
#endif //SERV_STEAM
#ifdef SERV_CHANNELING_AERIA
		m_bAeriaClient = false;
#endif //SERV_CHANNELING_AERIA
	}
};

DECL_DATA( KAccountOption )
{
    bool                    m_bPlayGuide;
};

DECL_DATA( KAccountBlockInfo )
{
    std::wstring            m_wstrEndTime;
    std::wstring            m_wstrBlockReason;

#ifdef SERV_HACKING_USER_CHECK_COUNT// �۾���¥: 2013-06-02	// �ڼ���
	byte					m_byteBlockType;
	std::wstring			m_wstrBlockReason2;
	std::wstring			m_wstrBlockEndDate;

	KAccountBlockInfo()
		: m_byteBlockType( 0 )
	{
	}
#endif // SERV_HACKING_USER_CHECK_COUNT
};

DECL_DATA( KAccountInfo )
{
    UidType                 m_nUserUID;
    std::wstring            m_wstrID;
    std::wstring            m_wstrName;
    int				        m_iAuthLevel;
    bool                    m_bInternalUser;
    KAccountOption          m_kAccountOption;
    KAccountBlockInfo       m_kAccountBlockInfo;
	bool					m_bIsRecommend;
	bool					m_bIsGuestUser;
    std::wstring            m_wstrOTP;
	//{{ 2011. 02. 23	������	ĳ�� ��ǰ ����Ʈ
#ifdef SERV_CASH_ITEM_LIST
	std::wstring			m_wstrRegDate;
#endif SERV_CASH_ITEM_LIST
	//}}
	//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
	std::wstring			m_wstrLastLogin;
#endif SERV_SECOND_SECURITY
	//}}
	//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	int						m_iChannelRandomKey;
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}
};

DECL_DATA( KItemAttributeEnchantInfo )
{
	// �迭�� �ϸ� index�� �߸� ����ؼ� �޸� ���� ���ɼ��� �ֱ� ������ �����ϰ� ������ 3�� ������ ������� ��
	char					m_cAttribEnchant0;
	char					m_cAttribEnchant1;
	char					m_cAttribEnchant2;

	void Init()
	{
		m_cAttribEnchant0 = 0;
		m_cAttribEnchant1 = 0;
		m_cAttribEnchant2 = 0;
	}

	KItemAttributeEnchantInfo()
	{
		Init();
	}

	COPYCON_ASSIGNOP( KItemAttributeEnchantInfo, right )
	{
		m_cAttribEnchant0 = right.m_cAttribEnchant0;
		m_cAttribEnchant1 = right.m_cAttribEnchant1;
		m_cAttribEnchant2 = right.m_cAttribEnchant2;

		return *this;
	}

	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	bool Compare( const KItemAttributeEnchantInfo& kInfo ) const
	{
		if( m_cAttribEnchant0 == kInfo.m_cAttribEnchant0  &&
			m_cAttribEnchant1 == kInfo.m_cAttribEnchant1  &&
			m_cAttribEnchant2 == kInfo.m_cAttribEnchant2 )
		{
			return true;
		}

		return false;
	}
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

	//{{ 2011. 01. 05	������	�Ӽ� ����
#ifdef SERV_ATTRIBUTE_CHARM
	int GetAttribEnchantCount() const
	{
		int iAttribEnchantCount = 0;
		if( m_cAttribEnchant0 != 0 )
		{
			++iAttribEnchantCount;
		}
		if( m_cAttribEnchant1 != 0 )
		{
			++iAttribEnchantCount;
		}
		if( m_cAttribEnchant2 != 0 )
		{
			++iAttribEnchantCount;
		}
		return iAttribEnchantCount;
	}
#endif SERV_ATTRIBUTE_CHARM
	//}}
};

DECL_DATA( KItemInfo )
{
	//{{ 2013. 05. 09	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	enum ITEM_STATE
	{
		IS_INIT = 0,		// �ʱ�ȭ ����
		IS_NORMAL,			// �Ϲ� ������
		IS_NOT_EVALUATED,	// �������� ���� ����
	};
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

	// �⺻ ����
	int							m_iItemID;
    char						m_cUsageType;
	int							m_iQuantity;
	short						m_sEndurance;
	unsigned char				m_ucSealData;

	// ��ȭ ����
	char						m_cEnchantLevel;
	KItemAttributeEnchantInfo	m_kAttribEnchantInfo;

	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int >			m_vecItemSocket;
#else
	std::vector< short >		m_vecItemSocket;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	//{{ 2013. 05. 09	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	std::vector< int >			m_vecRandomSocket;
	char						m_cItemState;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

	// �Ⱓ ����
    short						m_sPeriod;
    std::wstring				m_wstrExpirationDate;

#ifdef SERV_GOLD_TICKET
	UidType						m_iGoldTicketKeyUID;
#endif //SERV_GOLD_TICKET

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
	byte						m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS

    KItemInfo()
	{
        m_iItemID			 = 0;
        m_cUsageType		 = 0;
        m_iQuantity			 = 1;
        m_sEndurance		 = 0;
		m_ucSealData		 = 0;
		m_cEnchantLevel		 = 0;
		//{{ 2013. 05. 09	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		m_cItemState		 = IS_INIT;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}
        m_sPeriod			 = 0;
		m_wstrExpirationDate = L"";

#ifdef SERV_GOLD_TICKET
		m_iGoldTicketKeyUID	= 0;
#endif //SERV_GOLD_TICKET

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
		m_byteExpandedSocketNum = 0;
#endif // SERV_BATTLE_FIELD_BOSS
	}

	COPYCON_ASSIGNOP( KItemInfo, right )
	{
		m_iItemID			 = right.m_iItemID;		
		m_cUsageType		 = right.m_cUsageType;
		m_iQuantity			 = right.m_iQuantity;
		m_sEndurance		 = right.m_sEndurance;
		m_ucSealData		 = right.m_ucSealData;
		m_cEnchantLevel		 = right.m_cEnchantLevel;
		m_kAttribEnchantInfo = right.m_kAttribEnchantInfo;
		m_vecItemSocket		 = right.m_vecItemSocket;
		//{{ 2013. 05. 09	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		m_vecRandomSocket	 = right.m_vecRandomSocket;
		m_cItemState		 = right.m_cItemState;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}
		m_sPeriod			 = right.m_sPeriod;
		m_wstrExpirationDate = right.m_wstrExpirationDate;
#ifdef SERV_GOLD_TICKET
		m_iGoldTicketKeyUID	 = right.m_iGoldTicketKeyUID;
#endif //SERV_GOLD_TICKET
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-18	// �ڼ���
		m_byteExpandedSocketNum = right.m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS

		return *this;
	}

    //{{ 2009. 8. 26  ������	�к�
	bool IsSealedItem() const	{ return ( m_ucSealData >= 100 ); } // 100�̻��̸� �к��� �����Դϴ�.
	void UnsealItem()			{ m_ucSealData -= 100; }			// 100�� ���� �к� ���� ���·� �ٲ۴�.
	unsigned char GetSealCount() const
	{
		if( m_ucSealData >= 100 )
		{
			return ( m_ucSealData - 100 );
		}
		else
		{
			return m_ucSealData;
		}
	}
	//}}
};

DECL_DATA( KBuyGPItemInfo )
{
    int                         m_iItemID;
    int                         m_iPrice;
    int                         m_iPvPPoint;
	char						m_cPeriodType;
	int							m_iQuantity;
	short						m_sEndurance;	
    short                       m_sPeriod;

    KBuyGPItemInfo()
    {
        m_iItemID = 0;
        m_iPrice = 0;
        m_iPvPPoint = 0;
        m_cPeriodType = 0;
        m_iQuantity = 1;
        m_sEndurance = 0;
        m_sPeriod = 0;
    }
};

//{{ 2010. 8. 3	������	�� �ý���
#ifdef SERV_PET_SYSTEM

DECL_DATA( KItemPosition )
{
	enum ITEM_POS_ENUM
	{
		IPE_INVALID_PET_UID = -1,
	};

	int							m_iSlotCategory;
	int							m_iSlotID;
	UidType						m_iPetUID;

	KItemPosition()
	{
		m_iSlotCategory = 0;
		m_iSlotID		= 0;
		m_iPetUID		= IPE_INVALID_PET_UID;
	}

	KItemPosition( int iSlotCategory, int iSlotID )
	{
		m_iSlotCategory = iSlotCategory;
		m_iSlotID		= iSlotID;
		m_iPetUID		= IPE_INVALID_PET_UID;
	}
};

#endif SERV_PET_SYSTEM
//}}

DECL_DATA( KInventoryItemSimpleInfo )
{
	UidType                     m_iItemUID;
	int							m_iItemID;
	char                        m_cSlotCategory;
	//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
	short						m_sSlotID;
#else
	char                        m_cSlotID;
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
	//}}
	char						m_EnchantLevel;	

	KItemAttributeEnchantInfo	m_kAttribEnchantInfo;
};

DECL_DATA( KInventoryItemInfo )
{
	UidType                     m_iItemUID;
	char						m_cSlotCategory;
	//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
	short						m_sSlotID;
#else
	char						m_cSlotID;
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
	//}}
	KItemInfo                   m_kItemInfo;

	CON_COPYCON_ASSIGNOP( KInventoryItemInfo, right )
	{
		m_iItemUID		= right.m_iItemUID;
		m_cSlotCategory = right.m_cSlotCategory;
		//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		m_sSlotID		= right.m_sSlotID;
#else
		m_cSlotID		= right.m_cSlotID;
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}		
		m_kItemInfo		= right.m_kItemInfo;

		return *this;
	}
};

//{{ 2009. 11. 15  ������	�����ۻ�������
DECL_DATA( KDeletedItemInfo )
{
	enum DELETE_REASON
	{
		DR_NOT_DELETE = 0,			// [0]  �������� ���� ����
		DR_NO_REASON,				// [1]  ���� ���� : �⺻��
		DR_DISCARD,					// [2]  ������ ������
		DR_SELL_TO_NPC,				// [3]  NPC���� ������ ���ȱ�
		DR_RESOLVE,					// [4]  ����
		DR_TRADE,					// [5]  ���ΰŷ�
		DR_PERSONAL_SHOP,			// [6]  ���λ���
		DR_LETTER,					// [7]  ���� ÷��
		DR_RANDOM_ITEM,				// [8]  ����ť��
		DR_EXCHANGE,				// [9]  NPC�κ��� ������ ��ȯ
		DR_EXPIRED,					// [10] �Ⱓ ����
		DR_QUEST_COMPLETE,			// [11] ����Ʈ �Ϸ�
		DR_MANUFACTURE,				// [12] ���� ���
		DR_ENCHANT_PLUOR,			// [13] ��ȭ������
		DR_RESTORE,					// [14] ��ȭ���� ������
		DR_SEAL,					// [15] �����ֹ���
		DR_ENCHANT_STONE,			// [16] ��ȭ�� ���
		DR_ATTACH,					// [17] ��ȭ�ο�
		DR_DUNGEON_REQUIRE,			// [18] �������������
		DR_USE_QUICK_SLOT,			// [19] ������ ���
		DR_CREATE_GUILD,			// [20] ���â���㰡��
		DR_RESET_GUILD_SKILL,		// [21] ��彺ų�ǵ�����
		DR_INIT_GUILD_SKILL,		// [22] ��彺ų�ʱ�ȭ
		DR_RESET_SKILL,				// [23] ������ų�ǵ�����
		DR_INIT_SKILL,				// [24] ������ų�ʱ�ȭ
		DR_USE_ITEM_IN_INVENTORY,	// [25] �κ��丮���� ������ ���
		DR_IDENTIFY,				// [26] �Ӽ��� ����
		DR_ATTRIB_ENCHANT,			// [27] �Ӽ���ȭ
		DR_SOCKET_MAGIC_STONE,		// [28] ���� ������
		DR_PREPARE_INSERT,			// [29] ������ ���� �غ�
		DR_MOVE_SLOT,				// [30] �κ��丮 ���� �̵�
		DR_DELETED_BY_SYSTEM,		// [31] �ý��� ������ ������ - DBA�� ���� ���� ����
		DR_UNIT_DELETE,				// [32] ĳ���� ���� - DB�� ���� ����
		//{{ 2010. 7. 30 ������	�� �ý���
//#ifdef SERV_PET_SYSTEM
		DR_PET_CREATE_ITEM,			// [33] �� ���� ������ ���
		DR_PET_FEED,				// [34] �� ���� �ֱ�
		DR_PET_AUTO_FEED,			// [35] �� �ڵ� ���� ���
//#endif SERV_PET_SYSTEM
		//}}
		//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
//#ifdef SERV_QUEST_CLEAR_EXPAND
		DR_QUEST_GIVE_UP,			// [36] ����Ʈ ����� �ڵ�����
//#endif SERV_QUEST_CLEAR_EXPAND
		//}}
		//{{ 2011. 01. 04	������	�Ӽ� ����
//#ifdef SERV_ATTRIBUTE_CHARM
		DR_ATTRIB_ATTACH,			// [37] �Ӽ��ο�
//#endif SERV_ATTRIBUTE_CHARM
		//}}
		DR_BUG_USER_DELETE,			// [38] ���� ���߷� ���� ������ ����
		//{{ 2011. 04. 08	������	�븮 ����
//#ifdef SERV_PSHOP_AGENCY
		DR_PSHOP_AGENCY,			// [39] �븮���ο� ������ ������� ���� ����
//#endif SERV_PSHOP_AGENCY
		//}}
		DR_SYNTHESIS,				// [40] �ӱԼ� �Ϻ� �߰�
		DR_BY_GM_TOOL,				// [41] GM������ ����
//#ifdef SERV_SHARING_BANK_TEST
		DR_PUT_SHARE_BANK,			// [42]	�������࿡ ������ �ֱ�
		DR_GET_SHARE_BANK,			// [43] �������࿡�� ������ ����
//#endif SERV_SHARING_BANK_TEST
//#ifdef SERV_NEW_RESULT
		DR_DUNGEON_REWARD,			// [44] ���� ����
//#endif SERV_NEW_RESULT
//#ifdef SERV_SHARING_BANK_TEST
		DR_ZERO_QUANTITY_SHARE_BANK,// [45] �������ణ ������ ������ �̵����� ����
//#endif SERV_SHARING_BANK_TEST 

//{{ 2012. 02. 22	�ڼ���	�� �̸� �����
//#ifdef SERV_PET_CHANGE_NAME
		DR_PET_NAME_CHANGE,			// [46] �� �̸� ����� ���
//#endif SERV_PET_CHANGE_NAME
//}}

//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
//#ifdef SERV_GUILD_CHANGE_NAME
		DR_GUILD_NAME_CHANGE,			// [47] ��� �̸� ����� ���
//#endif SERV_GUILD_CHANGE_NAME
//}}
//#ifdef SERV_FISHING_SYSTEM
		DR_FISHING_BAIT,				// [48] ���� �̳� ����
//#endif SERV_FISHING_SYSTEM
//{{
//#ifdef SERV_ENCHANTMENT_EXTRACTION_SYSTEM
		DR_EXTRACT_ENCHANT,                   // [49] ��ȭ �ɷ� ���⿡ ���� ��� ������ ����
//#endif SERV_ENCHANTMENT_EXTRACTION_SYSTEM
//}}
//{{ 2013. 04. 10	������	����� �� ����
//#ifdef SERV_NEW_DEFENCE_DUNGEON
		DR_DEFENCE_DUNGEON_DUMMY_ITEM,	// [50] ����� �� ������ ���� ������ ����
//#endif SERV_NEW_DEFENCE_DUNGEON
//}}
//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
//#ifdef SERV_RELATIONSHIP_SYSTEM
		DR_RELATIONSHIP,                   // [51] �ο� �ý��� ��� ������
//#endif SERV_RELATIONSHIP_SYSTEM
//}
//#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-20
		DR_RIDING_PET_CREATE_ITEM,		// [52] ���̵� �� ���� ������
//#endif	// SERV_RIDING_PET_SYSTM
//#ifdef SERV_FINALITY_SKILL_SYSTEM	// ���볯¥: 2013-08-01
		DR_ITEM_EXTRACT,			// [53] ������ ����
		DR_USE_FINALITY_SKILL,		// [54] �ñر� ��� ���
//#endif // SERV_FINALITY_SKILL_SYSTEM

//#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-17	// �ڼ���
		DR_EXPAND_SOCKET_SLOT,		// [55] ���� ���� Ȯ�� ���
//#endif // SERV_BATTLE_FIELD_BOSS
	};

	UidType					m_iItemUID;
	u_char					m_ucDeleteReason;

	KDeletedItemInfo()
	{
		m_iItemUID = 0;
		m_ucDeleteReason = DR_NO_REASON;
	}

	KDeletedItemInfo( UidType iItemUID )
	{
		m_iItemUID = iItemUID;
		m_ucDeleteReason = DR_NO_REASON;
	}

	KDeletedItemInfo( UidType iItemUID, DELETE_REASON eReason )
	{
		m_iItemUID = iItemUID;
		m_ucDeleteReason = static_cast<u_char>(eReason);
	}

	//{{ 2010. 01. 18  ������	�����˻�
	static bool IsAbnormalReason( DELETE_REASON eReason )
	{
		switch( eReason )
		{
		case DR_NOT_DELETE:
		case DR_NO_REASON:
			return true;
		}

		return false;
	}
	//}}
};
//}}

DECL_DATA( KItemQuantityUpdate )
{
    std::map< UidType, int >			m_mapQuantityChange;
	//{{ 2009. 12. 15  ������	������ ��������
#ifdef DELETE_ITEM
    std::vector< KDeletedItemInfo >		m_vecDeleted;
#else
	std::vector< UidType >				m_vecDeleted;
#endif DELETE_ITEM
	//}}
};

typedef std::map< UidType, int >                    KItemEnduranceUpdate;

//{{ 2010. 8. 3	������	�� �ý���
#ifdef SERV_PET_SYSTEM
typedef std::map< UidType, KItemPosition >			KItemPositionUpdate;
#else
typedef std::map< UidType, std::pair< int, int > >  KItemPositionUpdate;
#endif SERV_PET_SYSTEM
//}}

DECL_DATA( KDungeonClearInfo )
{
    int                     m_iDungeonID;
	int						m_iMaxScore;
	char					m_cMaxTotalRank;
    std::wstring            m_wstrClearTime;
    bool                    m_bNew;

	CON_COPYCON_ASSIGNOP( KDungeonClearInfo, right )
	{
		m_iDungeonID = right.m_iDungeonID;
		m_iMaxScore = right.m_iMaxScore;
		m_cMaxTotalRank = right.m_cMaxTotalRank;
		m_wstrClearTime = right.m_wstrClearTime;
		m_bNew = right.m_bNew;

		return *this;
	}
};

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
DECL_DATA( KDungeonPlayInfo )
{
	int                     m_iDungeonID;
	int						m_iPlayTimes;
	int						m_iClearTimes;
	bool                    m_bNew;

	CON_COPYCON_ASSIGNOP( KDungeonPlayInfo, right )
	{
		m_iDungeonID = right.m_iDungeonID;
		m_iPlayTimes = right.m_iPlayTimes;
		m_iClearTimes = right.m_iClearTimes;
		m_bNew = right.m_bNew;

		return *this;
	};
};
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

DECL_DATA( KTCClearInfo )
{
	int						m_iTCID;
	std::wstring			m_wstrClearTime;
	bool					m_bNew;

	CON_COPYCON_ASSIGNOP( KTCClearInfo, right )
	{
		m_iTCID = right.m_iTCID;
		m_wstrClearTime = right.m_wstrClearTime;
		m_bNew = right.m_bNew;
		return *this;
	};
};

//{{ 2010. 04. 02  ������	��������Ʈ
DECL_DATA( KCompleteQuestInfo )
{
    int						m_iQuestID;
	int						m_iCompleteCount;
	__int64					m_tCompleteDate;

	KCompleteQuestInfo()
	{
		m_iQuestID = 0;
		m_iCompleteCount = 0;
		m_tCompleteDate = 0;
	}
};
//}}

DECL_DATA( KSubQuestInstance )
{
	int				m_iID;

	UCHAR			m_ucClearData;
	bool			m_bIsSuccess;

	KSubQuestInstance()
	{
		m_iID			= 0;
		m_ucClearData	= 0;
		m_bIsSuccess	= false;
	}

	COPYCON_ASSIGNOP( KSubQuestInstance, right )
	{
		m_iID				= right.m_iID;			
		m_ucClearData		= right.m_ucClearData;
		m_bIsSuccess		= right.m_bIsSuccess;	

		return *this;
	}
};


DECL_DATA( KQuestInstance )
{
	int					m_iID;
	UidType				m_OwnorUnitUID;

	std::vector<KSubQuestInstance>	m_vecSubQuestInstance;

	KQuestInstance()
	{
		m_iID			= 0;
		m_OwnorUnitUID	= 0;
	}

	COPYCON_ASSIGNOP( KQuestInstance, right )
	{
		m_iID					= right.m_iID;
		m_OwnorUnitUID			= right.m_OwnorUnitUID;
		m_vecSubQuestInstance	= right.m_vecSubQuestInstance;

		return *this;
	}
};

DECL_DATA( KStat )
{	
	int			m_iBaseHP;
	int			m_iAtkPhysic;
	int			m_iAtkMagic;
	int			m_iDefPhysic;
	int			m_iDefMagic;

	KStat()
	{
		Init();
	}

	void Init()
	{
		m_iBaseHP			= 0;
		m_iAtkPhysic		= 0;
		m_iAtkMagic			= 0;
		m_iDefPhysic		= 0;
		m_iDefMagic			= 0;
	}

	void AddStat( KStat& stat )
	{
		m_iBaseHP			+= stat.m_iBaseHP;
		m_iAtkPhysic		+= stat.m_iAtkPhysic;
		m_iAtkMagic			+= stat.m_iAtkMagic;
		m_iDefPhysic		+= stat.m_iDefPhysic;
		m_iDefMagic			+= stat.m_iDefMagic;
	}

	void MultiplyStat( float fFactor )
	{
		m_iBaseHP			= (int)((float)m_iBaseHP		* fFactor);
		m_iAtkPhysic		= (int)((float)m_iAtkPhysic		* fFactor);
		m_iAtkMagic			= (int)((float)m_iAtkMagic		* fFactor);
		m_iDefPhysic		= (int)((float)m_iDefPhysic		* fFactor);
		m_iDefMagic			= (int)((float)m_iDefMagic		* fFactor);
	}

	void MultiplyBaseHP( float fFactor )
	{
		m_iBaseHP			= (int)((float)m_iBaseHP		* fFactor);
	}
	void MultiplyAttPhysic( float fFactor )
	{
		m_iAtkPhysic		= (int)((float)m_iAtkPhysic		* fFactor);
	}
	void MultiplyAtkMagic( float fFactor )
	{
		m_iAtkMagic			= (int)((float)m_iAtkMagic		* fFactor);
	}
	void MultiplyDefPhysic( float fFactor )
	{
		m_iDefPhysic		= (int)((float)m_iDefPhysic		* fFactor);
	}
	void MultiplyDefMagic( float fFactor )
	{
		m_iDefMagic			= (int)((float)m_iDefMagic		* fFactor);
	}
};

DECL_DATA( KSkillData )
{
	short m_iSkillID;
	UCHAR m_cSkillLevel;

	void Init()
	{
		m_iSkillID = 0;
		m_cSkillLevel = 0;
	}
};

DECL_DATA( KUnitSkillData )
{
	static const int EQUIPPED_SKILL_SLOT_COUNT = 4;				

	KSkillData 		m_aEquippedSkill[ EQUIPPED_SKILL_SLOT_COUNT ];
	KSkillData 		m_aEquippedSkillSlotB[ EQUIPPED_SKILL_SLOT_COUNT ];
	std::wstring	m_wstrSkillSlotBEndDate;														
	char			m_cSkillSlotBExpirationState;										// KUserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE		

	std::vector< KSkillData >	m_vecPassiveSkill;
	//{{ 2009. 11. 23  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	std::vector< KSkillData >	m_vecGuildPassiveSkill;
#endif GUILD_SKILL_TEST
	//}}
	//{{ 2010. 03. 13  ������	����� ��Ʈ
#ifdef SERV_SKILL_NOTE
	std::vector< int >		m_vecSkillNote;
#endif SERV_SKILL_NOTE
	//}}

#ifdef SERV_SKILL_PAGE_SYSTEM
	unsigned char	m_nActiveSkillPagesNumber;
	unsigned char	m_nTheNumberOfSkillPagesAvailable;
#endif // SERV_SKILL_PAGE_SYSTEM
	
	KUnitSkillData()
	{
		Init();
	}

	void Init()
	{
		m_vecPassiveSkill.resize(0);
		//{{ 2009. 11. 23  ������	��彺ų
#ifdef GUILD_SKILL_TEST
		m_vecGuildPassiveSkill.resize(0);
#endif GUILD_SKILL_TEST
		//}}
		//{{ 2010. 03. 13  ������	����� ��Ʈ
#ifdef SERV_SKILL_NOTE
		m_vecSkillNote.resize(0);
#endif SERV_SKILL_NOTE
		//}}
		m_wstrSkillSlotBEndDate = L"";
		m_cSkillSlotBExpirationState = 0;

		for( int i=0; i<EQUIPPED_SKILL_SLOT_COUNT; i++ )
		{
			m_aEquippedSkill[i].Init();
			m_aEquippedSkillSlotB[i].Init();
		}

#ifdef SERV_SKILL_PAGE_SYSTEM
		m_nActiveSkillPagesNumber					= 1;
		m_nTheNumberOfSkillPagesAvailable		= 1;
#endif // SERV_SKILL_PAGE_SYSTEM
	}
};

DECL_DATA( KFriendInfo )
{
    enum FRIEND_STATUS
    {
        FS_NORMAL,
        FS_REQUESTED,
        FS_WAITING,
        FS_BLOCKED,

        FS_NUM,
    };

    enum FRIEND_POSITION
    {
        FP_OFF_LINE,
        FP_ON_LINE,
    };

    UidType                             m_iUnitUID;
    std::wstring                        m_wstrNickName;
    char                                m_cGroupID;
    char                                m_cStatus;
    char                                m_cPosition;
};

DECL_DATA( KFriendMessageInfo )
{
    enum MESSAGE_TYPE
    {
        MT_REQUEST_FRIEND,
    };

    UidType                             m_iUnitUID;
    std::wstring                        m_wstrNickName;
    char                                m_cMessageType;
    std::wstring                        m_wstrMessage;
    std::wstring                        m_wstrRegDate;
};

DECL_DATA( KMessengerInfo )
{
    std::map< UidType, KFriendInfo >        m_mapFriendInfo;
    std::map< char, std::wstring >          m_mapGroup;
    std::vector< KFriendMessageInfo >       m_vecFriendMessageInfo;

    void Clear()
    {
        m_mapFriendInfo.clear();
        m_mapGroup.clear();
        m_vecFriendMessageInfo.clear();
    }
};

//{{ 2009. 10. 27  ������	��巹��
DECL_DATA( KUserGuildInfo )
{
	int									m_iGuildUID;
	std::wstring						m_wstrGuildName;
	u_char								m_ucMemberShipGrade;
	int									m_iHonorPoint;

	KUserGuildInfo()
	{
		m_iGuildUID				= 0;
		m_wstrGuildName			= L"";
		m_ucMemberShipGrade		= 0;
		m_iHonorPoint			= 0;
	}
};
//}}

//{{ 2012. 04. 20	�ڼ���	����� �� ����
#ifdef SERV_BATTLE_FIELD_SYSTEM
DECL_DATA( KLastPositionInfo )
{
	int                             m_iMapID;
	unsigned char 					m_ucLastTouchLineIndex;
	unsigned short 					m_usLastPosValue;
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-08	// �ڼ���
	bool							m_bIgnoreLastTouch;	// m_iMapID�� �ش��ϴ� �⺻ ��ġ�� ����ϵ��� �˸���.
#endif // SERV_BATTLE_FIELD_BOSS

	KLastPositionInfo()
	{
		Clear();
	}

	void Clear()
	{
		m_iMapID = 0;
		m_ucLastTouchLineIndex = 0;
		m_usLastPosValue = 0;
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-08	// �ڼ���
		m_bIgnoreLastTouch = false;
#endif // SERV_BATTLE_FIELD_BOSS
	}
};
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//////////////////////////////////////////////////////////////////////////
//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM

/////////////////////////////////////////////////////////////////////////////////////////////////////////
DECL_DATA( KBuffBehaviorFactor )
{
	UINT					m_uiType;
	std::vector< float >	m_vecValues;
	//vector<BUFF_BEHAVIOR_FACTOR> m_vecBuffBehaviorFactors;

	KBuffBehaviorFactor() : m_uiType( 0 )
	{
	}

	KBuffBehaviorFactor( UINT uiType_ ) : m_uiType( uiType_ )
	{
	}

	KBuffBehaviorFactor( const KBuffBehaviorFactor& rhs_ )
	{
		m_uiType = rhs_.m_uiType;
		m_vecValues = rhs_.m_vecValues;
	}

	float operator[]( const UINT uiIndex_ ) const        /// ���� ���µ��� ����� �� ����
	{
		if ( uiIndex_ < m_vecValues.size() )
			return m_vecValues[uiIndex_];
		else
			return 0.0f;   // ���� �α� ���ܾ� ��
	}
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////
DECL_DATA( KBuffFinalizerFactor )
{
	UINT					m_uiType;
	std::vector< float >	m_vecValues;

	KBuffFinalizerFactor() : m_uiType( 0 )
	{
	}

	KBuffFinalizerFactor( UINT uiType_ ) : m_uiType( uiType_ )
	{
	}

	KBuffFinalizerFactor( const KBuffFinalizerFactor& rhs_ )
	{
		m_uiType = rhs_.m_uiType;
		m_vecValues = rhs_.m_vecValues;
	}

	float operator[]( const UINT uiIndex_ ) const /// ���� ���µ��� ����� �� ����
	{
		if ( uiIndex_ < m_vecValues.size() )
			return m_vecValues[uiIndex_];
		else
			return 0.0f;   // ���� �α� ���ܾ� ��
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
DECL_DATA( KBuffIdentity )
{
	int					m_eBuffTempletID;
	UINT				m_uiUniqueNum;                /// ���� ���� ������ ������ �ִ� ���(�ߺ�) ������ �� �ִ� ��

	KBuffIdentity() : m_eBuffTempletID( 0 ), m_uiUniqueNum( 0 )
	{
	}

	KBuffIdentity( const int eBuffTempletID_, const UINT uiUniqueNum_ )
		: m_eBuffTempletID( eBuffTempletID_ ), m_uiUniqueNum( uiUniqueNum_ )
	{
	}

	KBuffIdentity( const KBuffIdentity& rhs_ )
		: m_eBuffTempletID( rhs_.m_eBuffTempletID ), m_uiUniqueNum( rhs_.m_uiUniqueNum )
	{
	}

	bool operator==( const KBuffIdentity& rhs_ ) const
	{
		if ( m_eBuffTempletID == rhs_.m_eBuffTempletID 
			&& m_uiUniqueNum == rhs_.m_uiUniqueNum )
			return true;
		else
			return false;
	}
};


//////////////////////////////////////////////////////////////////////////
DECL_DATA( KBuffFactor )
{
	std::vector< KBuffBehaviorFactor >		m_vecBuffBehaviorFactors;
	std::vector< KBuffFinalizerFactor >		m_vecBuffFinalizerFactors;
	KBuffIdentity							m_BuffIdentity;						/// �̰��� KBuffIdentity�� �� BUFF_FACTOR�� �߻� ��Ű�� ������ ���������� ������ �� �ְ���(Templet���κ��� ������ ���ͼ� �ٸ� �����鿡�� �˷��ٶ� ���)
	UidType									m_uidMesGameUnit;					/// ������UID
	float									m_fAccumulationMultiplier;			/// ��ø����
	UCHAR									m_ucAccumulationCountNow;			/// ���� ��ø Ƚ�� (255��������)
	bool									m_bIsMesGameUnitNpc;				/// �����ڰ� NPC�ΰ�?(true�� NPC)
	int										m_iFactorID;						/// ���� ���� ���̵� - �������� ���� �ߵ� �� �ʿ�

	KBuffFactor() : m_BuffIdentity(), m_uidMesGameUnit( 0 ), m_fAccumulationMultiplier( 0.0f ), m_ucAccumulationCountNow( 0 ), m_bIsMesGameUnitNpc( false ), m_iFactorID( 0 )
	{
	}

	KBuffFactor( const KBuffFactor& rhs_ ) : m_BuffIdentity( rhs_.m_BuffIdentity ),
		m_fAccumulationMultiplier( rhs_.m_fAccumulationMultiplier ), m_ucAccumulationCountNow( rhs_.m_ucAccumulationCountNow ), m_iFactorID( rhs_.m_iFactorID )
	{
		m_vecBuffBehaviorFactors = rhs_.m_vecBuffBehaviorFactors;
		m_vecBuffFinalizerFactors = rhs_.m_vecBuffFinalizerFactors;
	}
};

DECL_DATA( KBuffInfo )	// ���� �ϳ��� ������ ����
{
	KBuffFactor				m_kFactorInfo;
	__int64					m_iBuffStartTime;
	__int64					m_iBuffEndTime;

	KBuffInfo()
	{
		m_iBuffStartTime = 0;
		m_iBuffEndTime = 0;
	}
};

DECL_DATA( KUnitBuffInfo )	// ������ ������ ����(��) ����
{
	__int64						m_iInsertTime;		// ���� �Էµ� ���� �ð�
	std::map< int, KBuffInfo >	m_mapBuffInfo;

	KUnitBuffInfo()
	{
		m_iInsertTime = 0;
	}
};

DECL_DATA( KNpcUnitBuffInfo )
{
	int								m_iNpcUID;
	std::vector< KBuffFactor >		m_vecBuffFactor;

	KNpcUnitBuffInfo()
	{
		m_iNpcUID = 0;
	}
};

#endif SERV_SERVER_BUFF_SYSTEM
//}}
//////////////////////////////////////////////////////////////////////////

DECL_DATA( KUnitInfo )
{
    UidType                         	m_iOwnerUserUID;
    char                            	m_cAuthLevel;
	UidType                         	m_nUnitUID;
	u_int								m_uiKNMSerialNum;	//�ؽ��ڸ��� �޽����� ���� ������ �ѹ�
	char                            	m_cUnitClass;
	std::wstring                    	m_wstrNickName;

	std::wstring			        	m_wstrIP;
	USHORT                          	m_usPort;

	int                             	m_iED; 
	UCHAR                           	m_ucLevel;
	int                             	m_iEXP;
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	int									m_iOfficialMatchCnt;
	int							    	m_iRating;
	int							    	m_iMaxRating;
	int							    	m_iRPoint;
	int							    	m_iAPoint;
	bool								m_bIsWinBeforeMatch;

	//{{ 2012. 06. 20	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	char								m_cRank;
	//{{ 2012. 06. 25	�ڼ���	2012 ���� ����2
	float								m_fKFactor;
	bool								m_bIsRedistributionUser;
	int									m_iPastSeasonWin;
#endif SERV_2012_PVP_SEASON2
	//}}

#else
	int									m_iPVPEmblem;
	int							    	m_iVSPoint;
	int							    	m_iVSPointMax;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	
	int									m_iSPoint;						// ���� ������� ��ų �������� ��ų ����Ʈ
	int									m_iCSPoint;						// ������ �ִ� ĳ�� ��ų ����Ʈ									// fix!! data size ���̱�
	int									m_iMaxCSPoint;					// ĳ�� ��ų ������ ���Խ� �ʱ� ĳ�� ��ų ����Ʈ
	std::wstring						m_wstrCSPointEndDate;			// ĳ�ý�ų ������ ��� ����
	
	int							    	m_nNowBaseLevelEXP;
	int							    	m_nNextBaseLevelEXP;
	int                             	m_nStraightVictories; //short

    KStat								m_kStat;
	KStat								m_kGameStat;

	//{{ 2012. 02. 02	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KLastPositionInfo					m_kLastPos;	
#else
	int                             	m_nMapID;
	unsigned char 						m_ucLastTouchLineIndex;
	unsigned short 						m_usLastPosValue;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 04. 21	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	std::vector< KBuffInfo >			m_vecBuffInfo;
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	int							    	m_iWin;
	int							    	m_iLose;

    std::map< int, KDungeonClearInfo >	m_mapDungeonClear;
	std::map< int, KTCClearInfo >		m_mapTCClear;
#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
	std::map< int, KDungeonPlayInfo >	m_mapDungeonPlay;
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

    std::map< int, KInventoryItemInfo > m_mapEquippedItem;

	KUnitSkillData						m_UnitSkillData;

	bool								m_bIsParty;	//��Ƽ ���Կ���..
	//{{ 2008. 1. 7  ������  �ټ��� 	�ִ밪
	int									m_iSpiritMax;
	//}}
	int									m_iSpirit;
    bool                            	m_bIsGameBang;
#ifdef SERV_PC_BANG_TYPE
	int									m_iPcBangType;
#endif SERV_PC_BANG_TYPE
	//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
	int									m_iTitleID;
#else
	short								m_sTitleID;
#endif SERV_TITLE_DATA_SIZE
	//}}

	//{{ 2009. 9. 16  ������	���
#ifdef GUILD_TEST
	KUserGuildInfo						m_kUserGuildInfo;
#endif GUILD_TEST
	//}}
    std::wstring                        m_wstrLastLoginTime;
	
	//{{ 2012.02.20 ��ȿ��	ĳ���� ���� ���μ��� ���� (���� ��� �Ⱓ ����)
#ifdef SERV_UNIT_WAIT_DELETE
	std::wstring						m_wstrLastDate;			// ���� Logout Ÿ��
	bool								m_bDeleted;				// ���� ��� ���� ����
	__int64								m_trDelAbleDate;		// ���� ���� �Ͻ�
	__int64								m_trRestoreAbleDate;	// ���� ���� �Ͻ�
#endif SERV_UNIT_WAIT_DELETE
	//}}

#ifdef SERV_ADD_WARP_BUTTON
	__int64								m_trWarpVipEndData;
#endif // SERV_ADD_WARP_BUTTON

#ifdef SERV_GROW_UP_SOCKET
	int									m_iEventQuestClearCount;
	int									m_iExchangeCount;
#endif SERV_GROW_UP_SOCKET

#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
	int									m_iAccumulationSpirit;
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM

#ifdef SERV_CHINA_SPIRIT_EVENT
	int									m_arrChinaSpirit[6];
#endif SERV_CHINA_SPIRIT_EVENT

#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
	bool								m_bRecruit;
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER

#ifdef SERV_NEW_YEAR_EVENT_2014
	u_char								m_ucOldYearMissionRewardedLevel;
	int									m_iNewYearMissionStepID;
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_EVENT_CHECK_POWER
	unsigned char						m_ucCheckPowerCount;
	__int64								m_iCheckPowerTime;
	bool								m_bCheckPowerShowPopUp;
	unsigned char						m_ucCheckPowerScore;
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_4TH_ANNIVERSARY_EVENT
	unsigned int						m_iAccountPVPLoseCount;
#endif // SERV_4TH_ANNIVERSARY_EVENT

	KUnitInfo() 
	{
		Init();
	}
	
	void Init()
	{
		m_iOwnerUserUID             = 0;
		m_cAuthLevel                = 0;
		m_nUnitUID                  = 0;
		m_uiKNMSerialNum			= 0;
		m_cUnitClass                = 0;
		m_wstrNickName              = L"";
		m_wstrIP                    = L"";
		m_usPort					= 0;
		m_iED                       = 0;
		m_ucLevel                   = 0;
		m_iEXP                      = 0;
		//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		m_iRating					= 0;
		m_iRPoint				    = 0;
		m_iAPoint				    = 0;

		//{{ 2012. 06. 20	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		m_cRank						= 0;
		//{{ 2012. 06. 25	�ڼ���	2012 ���� ����2
		m_fKFactor					= 0.0;
		m_bIsRedistributionUser		= false;
#endif SERV_2012_PVP_SEASON2
		//}}
#else
		m_iPVPEmblem				= 0;
		m_iVSPoint				    = 0;
		m_iVSPointMax			    = 0;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		m_iSPoint					= 0;
		m_iCSPoint					= 0;
		m_iMaxCSPoint				= 0;
		m_wstrCSPointEndDate		= L"";

		m_nNowBaseLevelEXP		    = 0;
		m_nNextBaseLevelEXP		    = 0;
		m_nStraightVictories        = 0;

		//{{ 2012. 02. 02	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
#else
		m_nMapID                    = 0;
		m_ucLastTouchLineIndex		= 0;
		m_usLastPosValue			= 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		m_iWin					    = 0;
		m_iLose					    = 0;

		m_UnitSkillData.Init();

		m_bIsParty					= false;
		m_iSpiritMax				= 0;
		m_iSpirit					= 0;
        m_bIsGameBang               = false;
#ifdef SERV_PC_BANG_TYPE
		m_iPcBangType				= -1;
#endif SERV_PC_BANG_TYPE
		//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
		m_iTitleID					= 0;
#else
		m_sTitleID					= 0;
#endif SERV_TITLE_DATA_SIZE
		//}}
        m_wstrLastLoginTime = L"";
		
		//{{ 2012.02.20 ��ȿ��	ĳ���� ���� ���μ��� ���� (���� ��� �Ⱓ ����)
#ifdef SERV_UNIT_WAIT_DELETE
		m_wstrLastDate				= L"";		// ���� Logout Ÿ��
		m_bDeleted					= false;	// ���� ��� ���� ����
		m_trDelAbleDate				= 0LL;		// ���� ���� �Ͻ�
		m_trRestoreAbleDate			= 0LL;		// ���� ���� �Ͻ�
#endif SERV_UNIT_WAIT_DELETE
		//}}

#ifdef SERV_ADD_WARP_BUTTON
		m_trWarpVipEndData				= 0LL;
#endif // SERV_ADD_WARP_BUTTON

#ifdef SERV_GROW_UP_SOCKET
		m_iEventQuestClearCount		= 0;
		m_iExchangeCount			= 0;
#endif SERV_GROW_UP_SOCKET

#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
		m_iAccumulationSpirit		= 0;
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM

#ifdef SERV_CHINA_SPIRIT_EVENT
		m_arrChinaSpirit[0]			= 0;
		m_arrChinaSpirit[1]			= 0;
		m_arrChinaSpirit[2]			= 0;
		m_arrChinaSpirit[3]			= 0;
		m_arrChinaSpirit[4]			= 0;
		m_arrChinaSpirit[5]			= 0;
#endif SERV_CHINA_SPIRIT_EVENT

#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
		m_bRecruit					= false;
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER

#ifdef SERV_NEW_YEAR_EVENT_2014
		m_ucOldYearMissionRewardedLevel		= 0;
		m_iNewYearMissionStepID				= -1;
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_EVENT_CHECK_POWER
		m_ucCheckPowerCount			= 0;
		m_bCheckPowerShowPopUp		= false;
		m_ucCheckPowerScore			= 0;
		m_iCheckPowerTime			= 0;
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_4TH_ANNIVERSARY_EVENT
		m_iAccountPVPLoseCount		= 0;
#endif // SERV_4TH_ANNIVERSARY_EVENT
	}
};

//////////////////////////////////////////////////////////////////////////
// ������(D3DXVECTOR3 ���)
DECL_DATA( VECTOR3 )
{
	float x;
	float y;
	float z;

	VECTOR3(){ x = 0.0f; y = 0.0f; z = 0.0f; }
	VECTOR3( float x, float y, float z ){ this->x = x; this->y = y; this->z = z; }
	VECTOR3( const VECTOR3& src ){ *this = src; }

	const VECTOR3& operator = (const VECTOR3& right)
	{
		x = right.x;
		y = right.y;
		z = right.z;

		return *this;
	}
	bool operator == (const VECTOR3& right ) const
	{
		return ( x == right.x ) && ( y == right.y ) && ( z == right.z );
	}
	bool operator != (const VECTOR3& right ) const
	{
		return !( ( x == right.x ) && ( y == right.y ) && ( z == right.z ) );
	}
};


/////////////////////////////////////////////////////////////////////////
DECL_DATA( KRoomInfo )
{
    //���� ����Ÿ
    char					m_RoomType;
    UidType					m_RoomUID;
	UINT					m_uiRoomListID;
    std::wstring			m_RoomName;
    char					m_RoomState;

    bool					m_bPublic;
	bool					m_bTeamBalance;
    std::wstring            m_wstrPassword;

    char					m_MaxSlot;
    char					m_JoinSlot;

    std::wstring            m_wstrUDPRelayIP;
    USHORT                  m_usUDPRelayPort;

	//{{ 2011. 12. 13	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// �ڵ� ��Ƽ
	bool					m_bStartedByAutoParty;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//PVP ����Ÿ
    char					m_PVPGameType;
#ifdef DUNGEON_ITEM
	bool					m_bIsItemMode;
#endif DUNGEON_ITEM
	int						m_iPVPChannelClass;
    char					m_WinKillNum;
    bool					m_bCanIntrude;
    float					m_fPlayTime;
    short					m_WorldID;	
	short					m_ShowPvpMapWorldID;

	//���� ����Ÿ
    char				    m_DifficultyLevel;
    int					    m_iDungeonID;
	char					m_cGetItemType;
	char					m_cDungeonMode;	
	UidType					m_iPartyUID;

	//{{ 2011. 10. 31	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// ��Ʋ�ʵ� ����Ÿ
	int						m_iBattleFieldID;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
#ifdef SERV_PVP_REMATCH
	std::map< short, int >	m_mapAllPlayersSelectedMap;
#endif SERV_PVP_REMATCH
	//{{ 2013. 04. 10	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	bool					m_bDefenceDungeonOpen;
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

	KRoomInfo()
	{
		Initialize();
	}

	void Initialize()
	{
		//���� ����Ÿ
		m_RoomType			= -1;
		m_RoomUID			= 0;
		m_uiRoomListID		= 0;
		m_RoomState			= 0;

		m_bPublic			= true;
		m_bTeamBalance		= false;

		m_MaxSlot			= 0;
		m_JoinSlot			= 0;

		m_usUDPRelayPort	= 0;
		//{{ 2011. 12. 13	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_bStartedByAutoParty = false;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		//PVP ����Ÿ
		m_PVPGameType		= -1;
#ifdef DUNGEON_ITEM
		m_bIsItemMode		= false;	// �����̹� ����̸� ��������.
#endif DUNGEON_ITEM
		m_iPVPChannelClass	= -1;
		m_WinKillNum		= 0;
		m_bCanIntrude		= false;
		m_fPlayTime			= 0.0f;
		m_WorldID			= 0;
		m_ShowPvpMapWorldID = 0;

		//���� ����Ÿ
		m_DifficultyLevel	= 0;
		m_iDungeonID		= 0;
		m_cGetItemType		= 0;
		m_cDungeonMode		= 0;
		m_iPartyUID			= 0;

		//{{ 2011. 10. 31	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_iBattleFieldID	= 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2013. 04. 10	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
		m_bDefenceDungeonOpen = false;
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
	}
};

//�Ʒü� �⺻����
DECL_DATA( KTCInfo )
{
	int						m_iTCID;
	int						m_iDungeonID;
	UidType					m_RoomUID;
	char					m_cRoomType;

	std::wstring            m_wstrUDPRelayIP;
	USHORT                  m_usUDPRelayPort;

	float					m_fPlayTime;
};

//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
DECL_DATA( KSubQuestData )
{
	//08.03.10. hoons. ����Ʈ ������ ������ ���Ǹ�
	//���� �κ��� �ִ� ������ ����
	int					m_iInInventoryItemCount;
	// ����Ʈ ���� ����
	bool				m_bSussece;

	KSubQuestData()
	{
		m_iInInventoryItemCount = 0;
		m_bSussece = false;
	}

	COPYCON_ASSIGNOP( KSubQuestData, right )
	{
		m_iInInventoryItemCount = right.m_iInInventoryItemCount;
		m_bSussece = right.m_bSussece;
		return *this;
	}
};
#endif SERV_REFORM_QUEST
//}}

DECL_DATA( KSubQuestInfo )
{
	//08.03.10. hoons. ����Ʈ ������ ������ ���Ǹ�
	//##arg 1. ��������Ʈ ���̵�
	//##arg 2. ���� �κ��� �ִ� ������ ����.
	//{{ QUEST ���� - ��μ�
#ifdef SERV_REFORM_QUEST
	std::map<int,KSubQuestData>	m_mapSubQuestInfo;
#else
	std::map<int,int>	m_mapSubQuestInfo;
#endif SERV_REFORM_QUEST
	//}}

	CON_COPYCON_ASSIGNOP( KSubQuestInfo, right )
	{
		m_mapSubQuestInfo = right.m_mapSubQuestInfo;
		return *this;
	}
};

//{{ 2010. 07. 20  ������	�� �ý���
#ifdef SERV_PET_SYSTEM

DECL_DATA( KPetInfo )
{
	UidType							m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
	int								m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
	char							m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	std::wstring					m_wstrPetName;
	char							m_cEvolutionStep;		// ��ȭ �ܰ�
	short							m_sSatiety;				// ������
	int								m_iIntimacy;			// ģ�е�
	short							m_sExtroversion;		// ���⼺
	short							m_sEmotion;				// ������
	bool							m_bAutoFeed;			// �ڵ� ���� üũ
	std::wstring					m_wstrLastFeedDate;		// ������ ���� �� �ð�
	std::wstring					m_wstrLastSummonDate;	// ������ ��ȯ �ð�
	std::wstring					m_wstrRegDate;			// �� ���� �ð�
	//{{ 2012. 12. 24	�ڼ���	�� ���� ���� ��� �߰�
#ifdef SERV_PET_AUTO_LOOTING
	bool							m_bAutoLooting;			// ���� ���� ����
#endif SERV_PET_AUTO_LOOTING
	//}}
#ifdef SERV_EVENT_PET_INVENTORY
	//�̺�Ʈ �� ���� �߰� 
	bool							m_bEventFoodEat;      // �̺�Ʈ ������ ��� ���� 
	bool							m_bIsEventPetID;      // �̺�Ʈ �� ������ Ȯ�� �Ѵ�.
#endif SERV_EVENT_PET_INVENTORY

#ifdef SERV_FREE_AUTO_LOOTING
	bool							m_bFreeAutoLooting;
#endif SERV_FREE_AUTO_LOOTING

#ifdef SERV_PERIOD_PET
	std::wstring					m_wstrDestroyDate;		// ���� ��¥
#endif SERV_PERIOD_PET

	KPetInfo()
		: m_iPetUID( 0 )
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
		, m_iPetID( 0 )
#else //SERV_PETID_DATA_TYPE_CHANGE
		, m_cPetID( 0 )
#endif //SERV_PETID_DATA_TYPE_CHANGE
		, m_cEvolutionStep( 0 )
		, m_sSatiety( 0 )
		, m_iIntimacy( 0 )
		, m_sExtroversion( 0 )
		, m_sEmotion( 0 )
		, m_bAutoFeed( false )
		, m_bAutoLooting( false )
#ifdef SERV_EVENT_PET_INVENTORY
		, m_bEventFoodEat( false )
		, m_bIsEventPetID( false )
#endif SERV_EVENT_PET_INVENTORY
#ifdef SERV_FREE_AUTO_LOOTING
		, m_bFreeAutoLooting( false )
#endif SERV_FREE_AUTO_LOOTING
	{
	}
};

DECL_DATA( KFieldPetInfo )
{
	UidType							m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
	int								m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
	char							m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	std::wstring					m_wstrPetName;
	char							m_cEvolutionStep;
	int								m_iIntimacy;

	KFieldPetInfo()
	{
		m_iPetUID		 = 0;
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
		m_iPetID		 = 0;
#else //SERV_PETID_DATA_TYPE_CHANGE
		m_cPetID		 = 0;
#endif //SERV_PETID_DATA_TYPE_CHANGE
		m_cEvolutionStep = 0;
		m_iIntimacy		 = 0;
	}
};

#endif SERV_PET_SYSTEM
//}}

//{{ 2011. 12. 29	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM

DECL_DATA( KGamePlayStatus )
{
	enum CHAR_ABIL_TYPE
	{
		CAC_NONE = 0,
		CAC_WSP,
		CAC_CANNON_BALL_COUNT,
		CAC_FORCE_POWER,
	#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
		CAC_NP_AND_MUTATION_AND_FORMATION_MODE,
	#endif //SERV_9TH_NEW_CHARACTER
	#ifdef ADD_RENA_SYSTEM //��â�� // �ؿ��� ����
		CAC_NATURAL_FORCE,
	#endif //ADD_RENA_SYSTEM
	};

	enum COOL_TIME_TYPE
	{
        CTT_SKILL_COOL_TIME = 0,
		CTT_QUICK_SLOT_COOL_TIME,
#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
		CTT_RIDING_PET_COOL_TIME,
#endif	// SERV_RIDING_PET_SYSTM
	};

	int								m_iMaxHP;
    int								m_iCurHP;
	int								m_iMaxMP;
	int								m_iCurMP;
	int								m_iCurHyperGage;
	char							m_cCurHyperCount;
	char							m_cCharAbilType;
	int								m_iCharAbilCount;
	std::map< int, int >			m_mapSkillCoolTime;
	std::map< int, int >			m_mapQuickSlotCoolTime;
	std::set< int >					m_mapPetMP;
#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
	std::map< int, int >			m_mapRidingPetCoolTime;
#endif	// SERV_RIDING_PET_SYSTM

	KGamePlayStatus()
	{
		m_iCurHP = 0;
		m_iMaxMP = 0;
		m_iCurMP = 0;
		m_iCurHyperGage = 0;
		m_cCurHyperCount = 0;
		m_cCharAbilType = CAC_NONE;
		m_iCharAbilCount = 0;
	}

	void SetCharAbilType( IN const CHAR_ABIL_TYPE eType )
	{
		m_cCharAbilType = eType;
	}

	int GetWSP() const
	{
		if( m_cCharAbilType != CAC_WSP )
			return 0;

		return m_iCharAbilCount;
	}

	int GetCannonBallCount() const
	{
		if( m_cCharAbilType != CAC_CANNON_BALL_COUNT )
			return 0;

		return m_iCharAbilCount;
	}

	int GetForcePower() const
	{
		if( m_cCharAbilType != CAC_FORCE_POWER )
			return 0;

		return m_iCharAbilCount;
	}

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
	int GetNPAndMutationAndFormationMode() const
	{
		if( m_cCharAbilType != CAC_NP_AND_MUTATION_AND_FORMATION_MODE )
			return 0;

		return m_iCharAbilCount;
	}
#endif //SERV_9TH_NEW_CHARACTER

#ifdef ADD_RENA_SYSTEM //��â��
	int GetNaturalForce() const
	{
		if( m_cCharAbilType != CAC_NATURAL_FORCE )
			return 0;

		return m_iCharAbilCount;
	}
#endif //ADD_RENA_SYSTEM
};

DECL_DATA( KGamePlayStatusContainer )
{
	std::vector< KGamePlayStatus >		m_vecGamePlayStatus;

	void Clear()
	{
		m_vecGamePlayStatus.clear();
	}
	bool IsEmpty() const
	{
		return m_vecGamePlayStatus.empty();
	}
	void Set( const KGamePlayStatus& kStatus )
	{
        Clear();
		m_vecGamePlayStatus.push_back( kStatus );
	}
	void Set( const std::vector< KGamePlayStatus >& vecStatus )
	{
		m_vecGamePlayStatus = vecStatus;
	}
	bool Get( KGamePlayStatus& kInfo ) const
	{
        if( IsEmpty() )
			return false;

		kInfo = m_vecGamePlayStatus.front();
		return true;
	}
	bool Get( std::vector< KGamePlayStatus >& vecInfo ) const
	{
		if( IsEmpty() )
			return false;

		vecInfo = m_vecGamePlayStatus;
		return true;
	}
	bool AddSkillCoolTime( const int iSlotID, const int iCoolTime )
	{
		if( IsEmpty() )
			return false;

		std::vector< KGamePlayStatus >::iterator vit;
		for( vit = m_vecGamePlayStatus.begin(); vit != m_vecGamePlayStatus.end(); ++vit )
		{
			vit->m_mapSkillCoolTime.insert( std::make_pair( iSlotID, iCoolTime ) );
			break;
		}
		return true;
	}
	bool AddQuickSlotCoolTime( const int iSlotID, const int iCoolTime )
	{
		if( IsEmpty() )
			return false;

		std::vector< KGamePlayStatus >::iterator vit;
		for( vit = m_vecGamePlayStatus.begin(); vit != m_vecGamePlayStatus.end(); ++vit )
		{
			vit->m_mapQuickSlotCoolTime.insert( std::make_pair( iSlotID, iCoolTime ) );
			break;
		}
		return true;
	}

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
	bool AddRidingPetCoolTime( const int iSlotID, const int iCoolTime )
	{
		if( IsEmpty() )
			return false;

		std::vector< KGamePlayStatus >::iterator vit;
		for( vit = m_vecGamePlayStatus.begin(); vit != m_vecGamePlayStatus.end(); ++vit )
		{
			vit->m_mapRidingPetCoolTime.insert( std::map< int, int >::value_type( iSlotID, iCoolTime ) );
			break;
		}
		return true;
	}
#endif	// SERV_RIDING_PET_SYSTM
};

DECL_DATA( KPartyMemberStatus )
{
    float			m_fHpPercent;
	float			m_fMpPercent;

	KPartyMemberStatus()
	{
		m_fHpPercent = 0.f;
		m_fMpPercent = 0.f;
	}
};

#endif SERV_BATTLE_FIELD_SYSTEM
//}}

DECL_DATA( KRoomUserInfo )
{
	enum BONUS_TYPE
	{
		BT_PREMIUM_EXP_RATE = 0,		// �����̾� ����ġ ���ʽ� �޴�
		BT_SOCKET_OPT_EXP_RATE,			// ���� �ɼ� ����ġ ���ʽ�
		BT_CHANNEL_BONUS_EXP_RATE,		// ä�� ����ġ ���ʽ�
		BT_SOCKET_OPT_ED_RATE,			// ���� �ɼ� ED���ʽ�
		BT_GUILD_BONUS_EXP_RATE,		// ��� ����ġ ���ʽ�
		BT_GEV_EXP,						// �̺�Ʈ ����ġ ���ʽ�
		BT_GEV_VP,						// �̺�Ʈ VP ���ʽ�
		BT_GEV_ED,						// �̺�Ʈ ED ���ʽ�
		BT_GEV_PARTY_EXP,				// �̺�Ʈ ��Ƽ ����ġ ���ʽ�
		BT_GEV_PARTY_VP,				// �̺�Ʈ ��Ƽ VP ���ʽ�
		BT_GEV_PARTY_ED,				// �̺�Ʈ ��Ƽ ED ���ʽ�
		//{{ 2012. 12. 17	������	�ƶ� ��Ƽ �÷��� ���ʽ� ����ġ
//#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		BT_GEV_WITH_CHAR_EXP,			// �̺�Ʈ Ư�� ĳ���Ϳ� �Բ� ����ġ ���ʽ�
//#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		//}}
//#ifdef	SERV_RANKING_POINT_EVENT // ���볯¥: 2013-03-26
		BT_GEV_RANKING_POINT,			// �̺�Ʈ ��ŷ ����Ʈ ���ʽ�
		BT_GEV_PARTY_RANKING_POINT,		// �̺�Ʈ ��Ƽ ��ŷ ����Ʈ ���ʽ�
//#endif	// SERV_RANKING_POINT_EVENT
	};

    UidType                 m_iGSUID;
    UidType                 m_iOwnerUserUID;
	//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
	short					m_sServerGroupID;
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
	//}}
    char                    m_cAuthLevel;
    bool                    m_bMale; // ���� ���� �ʿ���ٸ� ���� ����..
    UCHAR                   m_ucAge; // ���� ���� �ʿ���ٸ� ���� ����..
    UidType                 m_nUnitUID;
    u_int					m_uiKNMSerialNum;	//�ؽ��ڸ��� �޽����� ���� ������ �ѹ�
    char                    m_cUnitClass;    
    std::wstring            m_wstrNickName;
	//{{ 2012. 04. 17	������	���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	int						m_iNumResurrectionStone;	// ��Ȱ�� ����
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}

    std::wstring            m_wstrIP;
    USHORT                  m_usPort;
	//{{ 2013. 1. 8	�ڼ���	Merge ����IP ���� ���н� ����IP�� �õ�( ������ )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
	std::wstring			m_wstrInternalIP;
	USHORT					m_usInternalPort;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
	//}}

	//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	UidType					m_iPartyUID;
	std::vector< KGamePlayStatus > m_vecGamePlayStatus;	// ���� �÷��� ���������� ����ֽ��ϴ�. ���ο� �濡 �����Ҷ� ������ ��� �ֽ��ϴ�.
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 04. 21	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	std::vector< KBuffInfo >		m_vecBuffInfo;
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

    UCHAR                   m_ucLevel;
    KStat                   m_kGameStat;
    std::map< int, KInventoryItemInfo > m_mapEquippedItem;
#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
	std::map< int, KInventoryItemInfo > m_mapSpecialItem;
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
	KUnitSkillData			m_UnitSkillData;
	//{{ 2011. 07. 11	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
	bool					m_bIsPvpNpc;
	int						m_iOfficialMatchCnt;
	int						m_iRating;
	int						m_iMaxRating;
	bool					m_bIsWinBeforeMatch;

	//{{ 2012. 06. 25	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	char					m_cRank;
	char					m_cRankForServer;
	float					m_fKFactor;
	bool					m_bIsRedistributionUser;
#endif SERV_2012_PVP_SEASON2
	//}}

//#else
//	int                     m_iPVPEmblem;
//#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2013. 03. 14	 ������� �ʴ� RoomUserInfo �� ������ ���� - ��μ�
#ifdef SERV_DELETE_ROOM_USER_INFO_DATA
#else
	int                     m_iSpiritMax;
	int                     m_iSpirit;
#endif SERV_DELETE_ROOM_USER_INFO_DATA
	//}

    bool                    m_bIsGameBang;
#ifdef SERV_PC_BANG_TYPE
	int						m_iPcBangType;
#endif SERV_PC_BANG_TYPE
	bool					m_bIsObserver;
	
	//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
//#ifdef SERV_ROOMUSERINFO_REFAC
	std::map< char, float > m_mapBonusRate;
//#endif SERV_ROOMUSERINFO_REFAC
	//}}
	bool					m_bIsRingofpvprebirth;
	bool					m_bIsGuestUser;

	//080310. hoons. �������� ����Ʈ�� ������ �ִ´�.(����� ���������� ���)
	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
//#ifdef SERV_QUEST_CLEAR_EXPAND
	std::map< int, KSubQuestInfo > m_mapOngoingQuest;
//#else
//	std::map< int, KSubQuestInfo > m_mapDropQuestItembyIngQuest;
//#endif SERV_QUEST_CLEAR_EXPAND
	//}}
	
	//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
//#ifdef SERV_TITLE_DATA_SIZE
	int						m_iTitleID;
//#else
//	short					m_sTitleID;
//#endif SERV_TITLE_DATA_SIZE
	//}}

	//{{ 2009. 9. 16  ������	���
#ifdef GUILD_TEST
	int						m_iGuildUID;
	std::wstring			m_wstrGuildName;
	u_char					m_ucMemberShipGrade;
#endif GUILD_TEST
	//}}
	//{{ 2010. 07. 21  ������	�� �ý���
#ifdef SERV_PET_SYSTEM
	std::vector< KPetInfo > m_vecPet;					// ���� ��ȯ�� �� 1������ ����� �ֽ��ϴ�.
#endif SERV_PET_SYSTEM
	//}}

	//{{ 2011. 05. 04  ��μ�	���� Ŭ����� ������ ���� ���� �߰�
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	std::set< int  >		m_setQuestInfo;
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
	bool					m_bUseItem;
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}
#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
	std::set< int  >		m_setGoingQuestInfo;
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	bool					m_bComeBackUser;
#endif SERV_COME_BACK_USER_REWARD
	//}} 

	//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
	bool					m_bHenirReward;
#endif SERV_NEW_HENIR_TEST
	//}}
	//{{ 2012. 05. 31	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	std::set< int >			m_setUseSkillBuffInPlay;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 09. 19   ��μ�   ĳ�ü� �湮 ����
#ifdef SERV_VISIT_CASH_SHOP
	bool					m_bEnterCashShop;
#endif SERV_VISIT_CASH_SHOP
	//}}

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
	UidType					m_iRidingPetUID;
	USHORT					m_usRidingPetID;
#endif	// SERV_RIDING_PET_SYSTM

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	char					m_cWeddingStatus;
	UidType					m_iLoverUnitUID;
#endif SERV_RELATIONSHIP_SYSTEM
	//}

#ifdef SERV_GROW_UP_SOCKET
	int						m_iEventQuestClearCount;
	int						m_iExchangeCount;
#endif // SERV_GROW_UP_SOCKET

#ifdef SERV_BLESS_OF_GODDESS_EVENT
	bool					m_bMaxLevelUnitInAccount;
#endif SERV_BLESS_OF_GODDESS_EVENT

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
	int						m_iGateOfDarknessSupportEventTime;
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
	bool					m_bCouple;
	UidType					m_iRelationTargetUserUid;
	std::wstring			m_wstrRelationTargetUserNickname;
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_RECRUIT_EVENT_BASE
	UidType					m_iRecruiterUnitUID;
	std::vector< UidType >	m_vecRecruitUnitUID;
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
	int						m_iValentineItemCount;
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM

#ifdef SERV_4TH_ANNIVERSARY_EVENT
	unsigned int						m_iAccountPVPLoseCount;
#endif // SERV_4TH_ANNIVERSARY_EVENT

	KRoomUserInfo()
	{
		m_iGSUID				= 0;
		m_iOwnerUserUID			= 0;
		//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
		m_sServerGroupID		= -1;
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
		//}}
		m_cAuthLevel			= 0;
		m_bMale					= false;
		m_ucAge					= 0;
		m_nUnitUID				= 0;
		m_uiKNMSerialNum		= 0;
		m_cUnitClass			= 0;
		//{{ 2012. 04. 17	������	���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		m_iNumResurrectionStone = 0;
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		//}}
		m_usPort				= 0;
		//{{ 2013. 1. 8	�ڼ���	Merge ����IP ���� ���н� ����IP�� �õ�( ������ )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
		m_usInternalPort		= 0;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
		//}}
		//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_iPartyUID				= 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		m_ucLevel				= 0;
		m_UnitSkillData.Init();
		//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		m_bIsPvpNpc				= false;
		m_iOfficialMatchCnt		= 0;
		m_iRating				= 0;
		m_iMaxRating			= 0;
		m_bIsWinBeforeMatch		= false;

		//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		m_cRank					= 0;
		m_cRankForServer		= 0;
		m_fKFactor				= 0.0;
		m_bIsRedistributionUser	= false;
#endif SERV_2012_PVP_SEASON2
		//}}

#else
		m_iPVPEmblem			= 0;
#endif SERV_PVP_NEW_SYSTEM
		//}}

		//{{ 2013. 03. 14	 ������� �ʴ� RoomUserInfo �� ������ ���� - ��μ�
#ifdef SERV_DELETE_ROOM_USER_INFO_DATA
#else
		m_iSpiritMax			= 0;
		m_iSpirit				= 0;
#endif SERV_DELETE_ROOM_USER_INFO_DATA
		//}
		m_bIsGameBang			= false;
#ifdef SERV_PC_BANG_TYPE
		m_iPcBangType			= -1;
#endif SERV_PC_BANG_TYPE
		m_bIsObserver			= false;
		m_bIsRingofpvprebirth	= false;
		m_bIsGuestUser			= false;
		//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
		m_iTitleID				= 0;
#else
		m_sTitleID				= 0;
#endif SERV_TITLE_DATA_SIZE
		//}}
		//{{ 2009. 9. 16  ������	���
#ifdef GUILD_TEST
		m_iGuildUID				= 0;
		m_ucMemberShipGrade		= 0;
#endif GUILD_TEST
		//}}
		//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
		m_bComeBackUser			= false;
#endif SERV_COME_BACK_USER_REWARD
		//}} 
		//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
		m_bHenirReward			= false;
#endif SERV_NEW_HENIR_TEST
		//}}
		//{{ 2012. 09. 19   ��μ�   ĳ�ü� �湮 ����
#ifdef SERV_VISIT_CASH_SHOP
		m_bEnterCashShop = false;
#endif SERV_VISIT_CASH_SHOP
		//}}

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
		m_iRidingPetUID			= 0;
		m_usRidingPetID			= 0;
#endif	// SERV_RIDING_PET_SYSTM
		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
		m_cWeddingStatus		= 0;
		m_iLoverUnitUID			= 0;
#endif SERV_RELATIONSHIP_SYSTEM
		//}
#ifdef SERV_GROW_UP_SOCKET
		m_iEventQuestClearCount		= 0;
		m_iExchangeCount			= 0;
#endif // SERV_GROW_UP_SOCKET

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
		m_bUseItem				= false;
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX

#ifdef SERV_BLESS_OF_GODDESS_EVENT
		m_bMaxLevelUnitInAccount		= false;
#endif SERV_BLESS_OF_GODDESS_EVENT

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		m_iGateOfDarknessSupportEventTime = 0;
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
		m_bCouple					= false;
        m_iRelationTargetUserUid	= 0;
        m_wstrRelationTargetUserNickname = L"";
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
		m_bUseItem				= false;
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX

#ifdef SERV_RECRUIT_EVENT_BASE
		m_iRecruiterUnitUID			= 0;
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		m_iValentineItemCount		= -1;
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM

#ifdef SERV_4TH_ANNIVERSARY_EVENT
		m_iAccountPVPLoseCount		= 0;
#endif // SERV_4TH_ANNIVERSARY_EVENT
	}

	//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
//#ifdef SERV_ROOMUSERINFO_REFAC
	float GetBonusRate( BONUS_TYPE eType ) const
	{
		std::map< char, float >::const_iterator mit;
		mit = m_mapBonusRate.find( static_cast<char>(eType) );
		if( mit == m_mapBonusRate.end() )
			return 0.0f;
		
		return mit->second;
	}
	void AddBonusRate( BONUS_TYPE eType, float fRate )
	{
		std::map< char, float >::iterator mit;
		mit = m_mapBonusRate.find( static_cast<char>(eType) );
		if( mit == m_mapBonusRate.end() )
		{
			m_mapBonusRate.insert( std::make_pair( eType, fRate ) );
		}
		else
		{
			//{{ 2012. 1. 15	�ڼ���	�̺�Ʈ ����ġ�� ��ø �Ұ� ( Max���� ó�� )
#ifdef SERV_EVENT_EXP_BONUS_CAN_NOT_ACCUMULATED
			if( eType == BT_GEV_EXP )
			{
				if( mit->second < fRate )
				{
					mit->second = fRate;
				}
				return;
			}
#endif SERV_EVENT_EXP_BONUS_CAN_NOT_ACCUMULATED
			//}}
			mit->second += fRate;
		}
	}
//#endif SERV_ROOMUSERINFO_REFAC
	//}}
};

DECL_DATA( KRoomSlotInfo )
{
    char					m_Index;
    char					m_SlotState;
    bool					m_bHost;
    bool					m_bReady;
	bool					m_bPitIn;
    bool                    m_bTrade;
    int						m_TeamNum;
    KRoomUserInfo           m_kRoomUserInfo;

//#ifdef PVP_BOSS_COMBAT_TEST
//	bool					m_bIsBoss;
//#endif PVP_BOSS_COMBAT_TEST
};

//{{ 2010. 05. 12  ������	���� �ڵ� ����
DECL_DATA( KDungeonPlayResultInfo )
{
	float							m_fHP;					// ���� HP
	int								m_nDamageCount;			// ����� ���� Ƚ��

	int								m_TotalScore;			// ��ü ����
	int								m_ComboScore;			// �޺� ����
	int								m_TechScore;			// ��ũ���� ����

	//{{ 2011. 01. 21	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
	int								m_iTotalGivenDamamge;	// ��ü Ÿ�� �����
	int								m_iTotalAttackedDamage;	// ��ü �ǰ� �����
	int								m_iTotalSumNpcHP;		// ��ü NPC�� HP��
	int								m_iTotalRecoveryHP;		// �Һ� ���������� ȸ���� �� HP
	int								m_iUsingKindOfSkill;	// ��ų ��� ����
	int								m_iCommandTechScore;	// Ŀ�ǵ� ��ũ���� ����
	int								m_iSkillTechScore;		// ��ų ��ũ���� ����
#endif SERV_DUNGEON_RANK_NEW
	//}}
	//{{ 2012. 03. 05	��μ�	���� ��� �α�
#ifdef SERV_DUNGEON_DASH_LOG
	int								m_iDashCount;			// ��� ��� Ƚ��
#endif SERV_DUNGEON_DASH_LOG
	//}}
	//{{ 2012. 04. 17	������	���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	int								m_iFrame;					// ���� ���ǵ��� ��� ������
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}
	//{{ 2012. 12. 12  ��ų ���� ��� Ÿ�� �α� - ��μ�
#ifdef SERV_USE_SKILL_SLOT_TYPE_LOG
	bool							m_bSkillSlotType;			// true = A , flase = B 
#endif SERV_USE_SKILL_SLOT_TYPE_LOG
	//}}

	KDungeonPlayResultInfo()
	{
		Clear();
	}

	void Clear()
	{
		m_fHP					= 0.0f;
		m_nDamageCount			= 0;
		m_TotalScore			= 0;
		m_ComboScore			= 0;
		m_TechScore				= 0;
		//{{ 2011. 01. 21	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW	
		m_iTotalGivenDamamge	= 0;	// ��ü Ÿ�� �����
		m_iTotalAttackedDamage	= 0;	// ��ü �ǰ� �����
		m_iTotalSumNpcHP		= 0;	// ��ü NPC�� HP��
		m_iTotalRecoveryHP		= 0;	// �Һ� ���������� ȸ���� �� HP
		m_iUsingKindOfSkill		= 0;	// ��ų ��� ����
		m_iCommandTechScore		= 0;	// Ŀ�ǵ� ��ũ���� ����
		m_iSkillTechScore		= 0;	// ��ų ��ũ���� ����
#endif SERV_DUNGEON_RANK_NEW
		//}}
		//{{ 2012. 03. 05	��μ�	���� ��� �α�
#ifdef SERV_DUNGEON_DASH_LOG
		m_iDashCount			= 0;	// ��� ��� Ƚ��
#endif SERV_DUNGEON_DASH_LOG
		//}}
		//{{ 2012. 04. 17	������	���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		m_iFrame				= 0;	// ���� ���ǵ��� ��� ������
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		//}}
		//{{ 2012. 12. 12  ��ų ���� ��� Ÿ�� �α� - ��μ�
#ifdef SERV_USE_SKILL_SLOT_TYPE_LOG
		m_bSkillSlotType		= true;			// true = A , flase = B 
#endif SERV_USE_SKILL_SLOT_TYPE_LOG
		//}}
	}
};
//}}

#ifdef SERV_GLOBAL_EVENT_TABLE
DECL_DATA( KGlobalEventTableData )
{
	enum EVENT_ACCOUNT_ENUM
	{
		EAE_NONE				 = 0, // �Ϲ� - ��� ���� ���
		EAE_ACCOUNT_NORMAL		 = 1, // ���� - ���� ���� �� ĳ���͸�
		EAE_ACCOUNT_SUM			 = 2, // ���� - ���� ���� ��� ĳ���Ͱ� ������ ����
	};

	UidType				m_iUserUID;
	UidType				m_iUnitUID;
	int					m_iEventID;
	unsigned short		m_usCategory;
	std::vector<int>	m_veciParamData;
	int					m_iEventType;
	int					m_iEventScriptID;
	char				m_cEventAccountType;

	KGlobalEventTableData()
	{
		m_iUserUID = -1;
		m_iUnitUID = -1;
		m_iEventID = -1;
		m_usCategory = 0;
		m_iEventType = 0;
		m_iEventScriptID = 0;
		m_cEventAccountType = 0;
	}
};
#endif //SERV_GLOBAL_EVENT_TABLE

//{{ 2011. 07. 19	������	������ ���� �ڵ� �����丵
//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
DECL_PACKET( ECN_VERIFY_SERVER_CONNECT_ACK )
{
    int             m_nOK;
    UidType         m_nUID;
	//{{ 2010. 04. 22  ������	������ ��ü ����
#ifdef SERV_INTEGRATION
	int				m_iDBRegServerGroupID;
	int				m_iLocalServerGroupID;
#else
	short           m_sGroupID;
#endif SERV_INTEGRATION
	//}}
    UidType         m_iServerUID;
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	int				m_iServerType;
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY
    std::wstring    m_strName;
    short           m_sMaxNum;
    KNetAddress     m_kAddr;
    int             m_iVersion;
};
//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
//}}

//{{ 2012. 10. 16	������		P2P & Relay ���� ����
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
DECL_DATA( KGamePlayNetworkInfo )
{
	UidType							m_iOpponentUnitUID;			// ���� UnitUID
	int								m_iHolePunchingTryCount;	// Ȧ ��Ī �õ� Ƚ��
	float							m_fP2PPlayTime;				// P2P ��Ʈ��ũ�� �÷����� �ð� ( ms���� )
	float							m_fRelayPlayTime;			// ������ ������ ���� �÷����� �ð� ( ms���� )

	KGamePlayNetworkInfo()
	{
		m_iOpponentUnitUID = 0;
		m_iHolePunchingTryCount = 0;
		m_fP2PPlayTime = 0.f;
		m_fRelayPlayTime = 0.f;
	}
};

DECL_DATA( KGamePlayNetworkInfoPackage )
{
	std::vector< KGamePlayNetworkInfo > m_vecGamePlayNetworkInfo;
};
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
//}}

DECL_DATA( KPacketOK )
{
    int             m_iOK;
};

//#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
DECL_DATA( KPacketReason )
{
    int             m_iReason;
};
//#endif	SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX


// KERM_BAN_USER_NOT -> send id

DECL_DATA( KERM_OPEN_ROOM_REQ )
{
    bool                                m_bQuickJoin;
    KRoomInfo                           m_kRoomInfo;
    KRoomUserInfo                       m_kRoomUserInfo;

	std::vector< UidType >				m_vecStudentUnitUID;	//�����ý����� ���ڸ���Ʈ	
	//{{ 2012. 07. 03	������		���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	std::wstring						m_wstrChannelIP;
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}
	//{{ 2011. 12. 13	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	int									m_iCurEXP;
	int									m_iCurED;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	KERM_OPEN_ROOM_REQ()
	{
		m_bQuickJoin	= false;
		//{{ 2011. 12. 13	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_iCurEXP		= 0;
		m_iCurED		= 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}
};

DECL_DATA( KQuestUpdate )
{
	int					m_iQuestID;
	std::vector<UCHAR>	m_vecClearData;

	CON_COPYCON_ASSIGNOP( KQuestUpdate, right )
	{
		m_iQuestID = right.m_iQuestID;
		m_vecClearData = right.m_vecClearData;

		return *this;
	}
};

DECL_DATA( KMissionUpdate )
{
	int						m_iTitleID;
	std::vector< short >	m_vecClearData;

	CON_COPYCON_ASSIGNOP( KMissionUpdate, right )
	{
		m_iTitleID = right.m_iTitleID;
		m_vecClearData = right.m_vecClearData;

		return *this;
	}
};

//{{ 2007. 8. 16  ������  ���ܿɼ� ���� �߰�
DECL_DATA( KDenyOptions )
{
	enum DenyOptionState
	{
		DOS_ON = 0,
		DOS_OFF,
		DOS_ONLY_FRIEND,
	};

	char					m_cDenyFriendShip;
	char					m_cDenyInviteGuild;
	char					m_cDenyParty;
	char					m_cDenyPersonalTrade;
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	char					m_cDenyRequestCouple;
#endif SERV_RELATIONSHIP_SYSTEM
    char                    m_cDenyInvitePracticePVP;
	//}

    KDenyOptions() {}

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	KDenyOptions( char cDenyFriendShip, char cDenyInviteGuild, char cDenyParty, char cDenyPersonalTrade, char cDenyRequestCouple,
        char cDenyInvitePracticePVP )
	{
		m_cDenyFriendShip	 = cDenyFriendShip;
		m_cDenyInviteGuild	 = cDenyInviteGuild;
		m_cDenyParty		 = cDenyParty;
		m_cDenyPersonalTrade = cDenyPersonalTrade;
		m_cDenyRequestCouple = cDenyRequestCouple;
        m_cDenyInvitePracticePVP = cDenyInvitePracticePVP;
	}
#else
	KDenyOptions( char cDenyFriendShip, char cDenyInviteGuild, char cDenyParty, char cDenyPersonalTrade )
	{
		m_cDenyFriendShip	 = cDenyFriendShip;
		m_cDenyInviteGuild	 = cDenyInviteGuild;
		m_cDenyParty		 = cDenyParty;
		m_cDenyPersonalTrade = cDenyPersonalTrade;
	}
#endif SERV_RELATIONSHIP_SYSTEM
	//}
};

//{{ 2008. 1. 31  ������  
DECL_DATA( KChatBlackListUnit )
{
	UidType				m_iUnitUID;
	std::wstring		m_wstrNickName;	

	CON_COPYCON_ASSIGNOP( KChatBlackListUnit, right )
	{
		m_iUnitUID = right.m_iUnitUID;
		m_wstrNickName = right.m_wstrNickName;

		return *this;
	}
};
//}}

//{{ 2008. 7. 14  ������  ��ŷ
DECL_DATA( KRankerInfo )
{
	UidType			m_iUnitUID;
	std::wstring	m_wstrNickName;

	KRankerInfo()
	{
		m_iUnitUID = 0;
	}

	COPYCON_ASSIGNOP( KRankerInfo, right )
	{
		m_iUnitUID = right.m_iUnitUID;
		m_wstrNickName = right.m_wstrNickName;

		return *this;
	}
};

//{{ 2009. 7. 1  ������		��ϸ��� �ð�
DECL_DATA( KHenirRankingInfo )
{
	int							m_iRank;

	int							m_iStageCount;
	UINT						m_ulPlayTime;
	__int64						m_tRegDate;
	
	UidType						m_iUnitUID;
	std::wstring				m_wstrNickName;
	char						m_cUnitClass;
	UCHAR						m_ucLevel;
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-16	// �ڼ���
	byte						m_bytePlayStartedUserNum;	// ����� ���� üũ�ϸ� �ȴ�.
#endif // SERV_HENIR_RENEWAL_2013

	KHenirRankingInfo()
	{
		m_iRank				= 0;

		m_iStageCount		= 0;
		m_ulPlayTime	= 0;
		m_tRegDate			= 0;

		m_iUnitUID			= 0;
		m_cUnitClass		= 0;
		m_ucLevel			= 0;
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-16	// �ڼ���
		m_bytePlayStartedUserNum	= 0;
#endif // SERV_HENIR_RENEWAL_2013
	}

	COPYCON_ASSIGNOP( KHenirRankingInfo, right )
	{
		m_iRank				= right.m_iRank;
		m_iStageCount		= right.m_iStageCount;
		m_ulPlayTime		= right.m_ulPlayTime;
		m_tRegDate			= right.m_tRegDate;
		m_iUnitUID			= right.m_iUnitUID;
		m_wstrNickName		= right.m_wstrNickName;
		m_cUnitClass		= right.m_cUnitClass;
		m_ucLevel			= right.m_ucLevel;
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-16	// �ڼ���
		m_bytePlayStartedUserNum	= right.m_bytePlayStartedUserNum;
#endif // SERV_HENIR_RENEWAL_2013

		return *this;
	}

	static bool IsSameRank( const KHenirRankingInfo& kNewRank, const KHenirRankingInfo& kOldRank )
	{
		// 1. ��������
		if( kNewRank.m_iStageCount != kOldRank.m_iStageCount )		
			return false;

		// 2. �ҿ�ð�
		if( kNewRank.m_ulPlayTime != kOldRank.m_ulPlayTime )		
			return false;

		// 3. ��Ͻð� - ��Ͻð����� ���Ұ�� ���� ������ŷ�� ������ ����
		//if( kNewRank.m_tRegDate != kOldRank.m_tRegDate )
		//	return false;

		return true;
	}

	static bool RankCompare( const KHenirRankingInfo& kNewRank, const KHenirRankingInfo& kOldRank )
	{
		// 1. ��������
		if( kNewRank.m_iStageCount > kOldRank.m_iStageCount )
		{
			return true;
		}

		// 2. �ҿ�ð�
		if( kNewRank.m_ulPlayTime < kOldRank.m_ulPlayTime  &&  
			kNewRank.m_iStageCount == kOldRank.m_iStageCount )
		{
			return true;
		}

		// 3. ��Ͻð�
		if( kNewRank.m_tRegDate < kOldRank.m_tRegDate  &&  
			kNewRank.m_ulPlayTime == kOldRank.m_ulPlayTime  &&  
			kNewRank.m_iStageCount == kOldRank.m_iStageCount )
		{ 
			return true;
		}

		// 4. �г��� ( �����ټ� )
		if( kNewRank.m_wstrNickName < kOldRank.m_wstrNickName  &&
			kNewRank.m_tRegDate < kOldRank.m_tRegDate  &&
			kNewRank.m_ulPlayTime == kOldRank.m_ulPlayTime  &&
			kNewRank.m_iStageCount == kOldRank.m_iStageCount )
		{
			return true;
		}

		return false;
	}

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-17	// �ڼ���
	bool	QualificationForHeroRank( void ) const	{	return m_bytePlayStartedUserNum == 1;	}
#endif // SERV_HENIR_RENEWAL_2013
};
//}}

//{{ 2009. 7. 3  ������		������ŷ, ������ŷ
DECL_DATA( KDungeonRankingInfo )
{
	int									m_iRank;
	UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;
	char								m_cUnitClass;
	UCHAR								m_ucLevel;
	int									m_iEXP;

	KDungeonRankingInfo()
	{
		m_iRank		 = 0;		
		m_cUnitClass = 0;
		m_ucLevel	 = 0;
		m_iEXP		 = 0;
	}
};

DECL_DATA( KPvpRankingInfo )
{
	int									m_iRank;
	UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;
	char								m_cUnitClass;
	UCHAR								m_ucLevel;	
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	char								m_cRank;
#else
	int									m_iRating;
#endif SERV_2012_PVP_SEASON2
	//}}
	int									m_iRPoint;
#else
	char								m_cPvpEmblem;
	int									m_iLose;
#endif SERV_PVP_NEW_SYSTEM
	//}}
    int									m_iWin;	

	KPvpRankingInfo()
	{
		m_iRank			= 0;
		m_cUnitClass	= 0;
		m_ucLevel		= 0;		
		//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		m_cRank			= 0;
#else
		m_iRating		= 0;
#endif SERV_2012_PVP_SEASON2
		//}}
		m_iRPoint		= 0;
#else
		m_cPvpEmblem	= 0;
		m_iLose			= 0;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		m_iWin			= 0;
	}
};
//}}

//{{ 2008. 6. 10  ������  ������ �̺�Ʈ
DECL_DATA( KConnectTimeEventInfo )
{
	int				m_iEventUID;
	std::wstring	m_wstrEventTime;
	//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
	bool			m_bAccountEvent;
#endif SERV_ACC_TIME_EVENT
	//}}

	//{{ 2012. 08. 14	�ڼ���	��õ���� ��ȭ �̺�Ʈ ���̵� ���� ���
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
	int				m_iScriptID;
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
	//}}

	KConnectTimeEventInfo()
	{
		m_iEventUID		= 0;
		//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
		m_bAccountEvent = false;
#endif SERV_ACC_TIME_EVENT
		//}}

		//{{ 2012. 08. 14	�ڼ���	��õ���� ��ȭ �̺�Ʈ ���̵� ���� ���
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		m_iScriptID		= 0;
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		//}}
	}
};

//{{ 2009. 12. 7  ������	�����ð��̺�Ʈ
#ifdef CUMULATIVE_TIME_EVENT

DECL_DATA( KCumulativeTimeEventInfo )
{
	int				m_iEventUID;
	int				m_iCumulativeTime;
	//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
	bool			m_bAccountEvent;
#endif SERV_ACC_TIME_EVENT
	//}}

	KCumulativeTimeEventInfo()
		: m_iEventUID( 0 )
		, m_iCumulativeTime( 0 )
		//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
		, m_bAccountEvent( false )
#endif SERV_ACC_TIME_EVENT
		//}}
	{
	}
};

#endif CUMULATIVE_TIME_EVENT
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
DECL_PACKET( EGS_HENIR_REWARD_COUNT_NOT )
{
	bool		            m_bUnLimited;		// ������
	int			            m_iNormal;			// �Ϲ�
	int			            m_iPremium;			// PC��
	int			            m_iEvent;			// �̺�Ʈ
	// Ŭ�󿡼� UI ǥ�ÿ� �ʿ��� ������
	int			            m_iPremiumMAX;		// PC�� MAX
	int			            m_iEventMAX;		// �̺�Ʈ MAX

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
	int			            m_iChallengeNormal;		// ���� ��� �Ϲ�
	int			            m_iChallengePremium;	// ���� ��� PC��
	int			            m_iChallengeEvent;		// ���� ��� �̺�Ʈ
#endif // SERV_HENIR_RENEWAL_2013

	KEGS_HENIR_REWARD_COUNT_NOT()
	{
		m_bUnLimited			= false;
		m_iNormal				= 0;
		m_iPremium				= 0;
		m_iEvent				= 0;
		m_iPremiumMAX			= 0;
		m_iEventMAX				= 0;

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-24	// �ڼ���
		m_iChallengeNormal		= 0;
		m_iChallengePremium		= 0;
		m_iChallengeEvent		= 0;
#endif // SERV_HENIR_RENEWAL_2013
	}
};
#endif SERV_NEW_HENIR_TEST
//}}

//{{ 2012. 05. 6	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
DECL_DATA( KRecordBuffInfo )
{
	int				m_iBuffID;
	std::wstring	m_wstrStartTime;

	KRecordBuffInfo()
	{
		m_iBuffID = 0;
	}
};

#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

#ifdef SERV_RIDING_PET_SYSTM
DECL_DATA( KRidingPetInfo )
{
	UidType			m_iRidingPetUID;
	USHORT			m_usRindingPetID;
	float			m_fStamina;
	std::wstring	m_wstrDestroyDate;		// �Ҹ� ���� �ð�
	__int64			m_iLastUnSummonDate;	// ȸ���Ǿ�� �ϴ� ���¹̳� ���� ��� �ϱ� ���� �뵵

	KRidingPetInfo()
		: m_iRidingPetUID( 0 )
		, m_usRindingPetID( 0 )
		, m_fStamina( 0.0f )
		, m_iLastUnSummonDate( 0 )
	{
	}
};
#endif //SERV_RIDING_PET_SYSTM

DECL_PACKET( DBE_UPDATE_UNIT_INFO_REQ )
{
	bool                                m_bFinal;
	//{{ 2009. 3. 17  ������	ä���̵�
    int									m_iEventID;
	//}}
	UidType                             m_iUnitUID;
	int                                 m_iEXP;
    int                                 m_iLevel;
	int                                 m_iED;
	//{{ 2011. 09. 23	������	�α׿��� ���� EDüũ
#ifdef SERV_LOGOUT_ED_CHECK
	int									m_iRealDataED;
#endif SERV_LOGOUT_ED_CHECK
	//}}
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	int									m_iOfficialMatchCnt;
	int								    m_iRating;
	int								    m_iMaxRating;
	int								    m_iRPoint;
	int								    m_iAPoint;
	bool								m_bIsWinBeforeMatch;
	char								m_cEmblemEnum;

	//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	float								m_fKFactor;
#endif SERV_2012_PVP_SEASON2
	//}}

#else
	int								    m_iVSPoint;
	int									m_iVSPointMax;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	
#ifdef SERV_SKILL_PAGE_SYSTEM
#else // SERV_SKILL_PAGE_SYSTEM
	int									m_iSPoint;
#endif // SERV_SKILL_PAGE_SYSTEM

	int								    m_iWin;
	int								    m_iLose;
	//{{ 2012. 02. 02	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KLastPositionInfo					m_kLastPos;
#else
	int									m_iMapID;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	
    std::map< int, KDungeonClearInfo >  m_mapDungeonClear;
	std::map< int, KTCClearInfo >		m_mapTCClear;
#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
	std::map< int, KDungeonPlayInfo >	m_mapDungeonPlay;
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES
    KItemQuantityUpdate                 m_kItemQuantityUpdate;
	KItemEnduranceUpdate                m_kItemEnduranceUpdate;
    KItemPositionUpdate                 m_kItemPositionUpdate;
	std::vector< KQuestUpdate >			m_vecQuestData;
	std::vector< KMissionUpdate >		m_vecMissionData;
	//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
	int									m_iInitTitleID;
	int									m_iEquippedTitleID;
#else
	short								m_sInitTitleID;
	short								m_sEquippedTitleID;
#endif SERV_TITLE_DATA_SIZE
	//}}
	
#ifdef SERV_SKILL_PAGE_SYSTEM
	std::map< int, std::vector<int> >		m_mapSkillSlotVector;
	std::wstring							m_wstrSkillSlotBEndDate;
#else // SERV_SKILL_PAGE_SYSTEM
	std::vector<int>					m_vecSkillSlot;
#endif // SERV_SKILL_PAGE_SYSTEM

	KDenyOptions						m_kDenyOptions;
	int									m_iSpirit;
	bool								m_bIsSpiritUpdated;	
	int									m_iNumResurrectionStone;
	//{{ 2009. 10. 14  ������	���ʺ�Ȱ��
#ifdef AP_RESTONE
	int									m_iNumAutoPaymentResStone;
#endif AP_RESTONE
	//}}
	std::map< int, int >				m_mapWishList;
	//{{ 2009. 7. 7  ������		��ŷ ����
	std::vector< KHenirRankingInfo >	m_vecHenirRanking;
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-17	// �ڼ���
	std::vector< KHenirRankingInfo >	m_vecHenirHeroRanking;
#endif // SERV_HENIR_RENEWAL_2013
	//}}
	//{{ 2009. 10. 7  ������	���
#ifdef GUILD_TEST
	int									m_iGuildUID;
	int									m_iHonorPoint;
#endif GUILD_TEST
	//}}
	//{{ 2009. 12. 8  ������	�̺�Ʈ����
#ifdef CUMULATIVE_TIME_EVENT
	std::vector< KCumulativeTimeEventInfo > m_vecUpdateEventTime;
#endif CUMULATIVE_TIME_EVENT
	//}}	
	//{{ 2010. 02. 09  ������	���� �̺�Ʈ ����Ʈ
#ifdef SERV_DAY_QUEST
	std::vector< int >					m_vecGiveUpQuestList;
#endif SERV_DAY_QUEST
	//}}
	//{{ 2010. 8. 4	������	�� �ý���
#ifdef SERV_PET_SYSTEM
	std::vector< KPetInfo >				m_vecPet;
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
	std::map< int, int >				m_mapCharGameCount;
#endif SERV_CHAR_LOG
	//}}
	//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
	KEGS_HENIR_REWARD_COUNT_NOT			m_kHenirRewardCnt;
#endif SERV_NEW_HENIR_TEST
	//}}
	//{{ 2012. 02. 21	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KGamePlayStatusContainer			m_kGamePlayStatus;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 05. 6	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	std::vector< KRecordBuffInfo >		m_vecRecordBuffInfo;
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}
	//{{ 2013. 3. 17	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
	int									m_iChangedLocalRankingSpirit;
	int									m_iChangedLocalRankingAP;
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
	std::vector<KRidingPetInfo>			m_vecRidingPetList;
#endif	// SERV_RIDING_PET_SYSTM
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	char								m_cUnitRelationshipType;
#endif SERV_RELATIONSHIP_SYSTEM
		//}
#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
	int									m_iAccumultionSpirit;
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
	int									m_iGateOfDarknessSupportEventTime;
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_CHINA_SPIRIT_EVENT
	int									m_arrChinaSpirit[6];
#endif SERV_CHINA_SPIRIT_EVENT

#ifdef SERV_ELESIS_UPDATE_EVENT
	int									m_iNoteViewCount;
#endif SERV_ELESIS_UPDATE_EVENT
};

DECL_PACKET( DBE_UPDATE_UNIT_INFO_ACK )
{
	int									m_iOK;
	//{{ 2009. 3. 17  ������	ä���̵�
	int									m_iEventID;
	//}}
    int                                 m_iEXP;
    int                                 m_iED;
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	int									m_iOfficialMatchCnt;
	int								    m_iRating;
	int								    m_iMaxRating;
	int								    m_iRPoint;
	int								    m_iAPoint;
	int									m_iWin;
	int									m_iLose;

	//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	float								m_fKFactor;
#endif SERV_2012_PVP_SEASON2
	//}}

#else
	int								    m_iVSPoint;
	int								    m_iVSPointMax;
#endif SERV_PVP_NEW_SYSTEM
	//}}
    std::vector< int >                  m_vecDungeonClear;
	std::vector< int >                  m_vecTCClear;
    KItemQuantityUpdate                 m_kItemQuantityUpdate;
	KItemEnduranceUpdate                m_kItemEnduranceUpdate;
    KItemPositionUpdate                 m_kItemPositionUpdate;
    int									m_iNumResurrectionStone;
	//{{ 2009. 10. 14  ������	���ʺ�Ȱ��
#ifdef AP_RESTONE
	int									m_iNumAutoPaymentResStone;
#endif AP_RESTONE
	//}}
	//{{ 2010. 8. 4	������	�� �ý���
#ifdef SERV_PET_SYSTEM
	std::vector< KPetInfo >				m_vecPet;
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2013. 3. 17	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
	int									m_iChangedLocalRankingSpirit;
	int									m_iChangedLocalRankingAP;
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}
#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
	std::vector<UidType>				m_vecRidingPetUID;
#endif	// SERV_RIDING_PET_SYSTM
};

DECL_PACKET( ERM_UPDATE_PVP_UNIT_INFO_NOT )
{
	int		                        m_iEXP;
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	int								m_iCurrentRating;
	int								m_iCurrentMaxRating;
	int								m_iCurrentRPoint;
	int								m_iCurrentAPoint;
	bool							m_bIsOfficialMatch;
	int		                        m_iRating;
	int		                        m_iRPoint;
	int		                        m_iAPoint;

	//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	char							m_cRank;
	char							m_cRankForServer;
	float							m_fKFactorIncrement;
#endif SERV_2012_PVP_SEASON2
	//}}

#else
	int		                        m_iVSPoint;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	int		                        m_iWin;
	int		                        m_iLose;	
	bool							m_bOutRoom;
	//{{ 2009. 11. 16  ������	�����������Ʈ
	int								m_iMemberCount;
	int								m_iPlayTime;
	//}}

	std::map< int, KItemInfo >      m_mapItem;	//�̺�Ʈ�� �������� �ֱ�����

	KERM_UPDATE_PVP_UNIT_INFO_NOT()
	{
		m_iEXP				= 0;
		//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		m_iCurrentRating	= 0;
		m_iCurrentMaxRating = 0;
		m_iCurrentRPoint	= 0;
		m_iCurrentAPoint	= 0;
		m_bIsOfficialMatch	= false;
		m_iRating			= 0;
		m_iRPoint			= 0;
		m_iAPoint			= 0;
		
		//{{ 2012. 06. 22	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		m_cRank				= 0;
		m_cRankForServer	= 0;
		m_fKFactorIncrement	= 0.0f;
#endif SERV_2012_PVP_SEASON2
		//}}

#else
		m_iVSPoint			= 0;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		m_iWin				= 0;
		m_iLose				= 0;
		m_bOutRoom			= false;
		//{{ 2009. 11. 16  ������	�����������Ʈ
		m_iMemberCount		= 0;
		m_iPlayTime			= 0;
		//}}
	}
};

DECL_PACKET( ERM_UPDATE_DUNGEON_UNIT_INFO_NOT )
{
	int		                	m_iEXP;
	int		                	m_iED;
	int                     	m_iPvPGameType;
	int                     	m_iDungeonID;
	int							m_iUserCount;
	bool						m_bDungeonClear;
	std::map< int, int >		m_mapGetItem;
	//{{ 2009. 7. 28  ������	�������â����
	std::map< int, KItemInfo >	m_mapResultItem;
	int							m_iDropItemUID;
	//}}

	bool						m_bOutRoom;
	int							m_iPlayTime;

	int							m_iMaxScore;
	char						m_cMaxTotalRank;
	bool						m_bIsTutorialSuccess;

	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	bool						m_bGetWorldMissionReward;
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	//{{ 2009. 7. 1  ������		��ϸ� �ð�
	std::vector< KHenirRankingInfo > m_kHenirRankingInfo;
	//}}
	//{{ 2012. 04. 16	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool						m_bStartedByAutoParty;			// �ڵ���Ƽ�� ���� ���� ����
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	KERM_UPDATE_DUNGEON_UNIT_INFO_NOT()
	{
		m_iEXP					= 0;
		m_iED					= 0;
		m_iPvPGameType			= 0;
		m_iDungeonID			= 0;
		m_iUserCount			= 0;
		m_bDungeonClear			= false;
		m_iDropItemUID			= 0;
		m_bOutRoom				= false;
		m_iPlayTime				= 0;
		m_iMaxScore				= 0;
		m_cMaxTotalRank			= 0;
		m_bIsTutorialSuccess	= false;

		//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
		m_bGetWorldMissionReward = false;
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
		//{{ 2012. 04. 16	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_bStartedByAutoParty	= false;			// �ڵ���Ƽ�� ���� ���� ����
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}

	COPYCON_ASSIGNOP( KERM_UPDATE_DUNGEON_UNIT_INFO_NOT, t )
	{
        m_iEXP		    		= t.m_iEXP;
        m_iED		    		= t.m_iED;
        m_iPvPGameType  		= t.m_iPvPGameType;
        m_iDungeonID    		= t.m_iDungeonID;
        m_iUserCount			= t.m_iUserCount;
        m_bDungeonClear 		= t.m_bDungeonClear;
        m_mapGetItem       		= t.m_mapGetItem;
		m_mapResultItem			= t.m_mapResultItem;
		m_iDropItemUID			= t.m_iDropItemUID;
        m_bOutRoom				= t.m_bOutRoom;
        m_iPlayTime				= t.m_iPlayTime;

        m_iMaxScore				= t.m_iMaxScore;
        m_cMaxTotalRank			= t.m_cMaxTotalRank;
		m_bIsTutorialSuccess	= t.m_bIsTutorialSuccess;

		//{{ 2009. 7. 1  ������		��ϸ� �ð�
		m_kHenirRankingInfo		= t.m_kHenirRankingInfo;
		//}}		

		//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
		m_bGetWorldMissionReward = t.m_bGetWorldMissionReward;
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

		//{{ 2012. 04. 16	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_bStartedByAutoParty	= t.m_bStartedByAutoParty;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

        return *this;
	}
};

DECL_DATA( KStatisticsDetailData )
{
	int					m_iNum;
	std::wstring		m_wszString;
};

DECL_DATA( KErrorMsgInfo )
{
	int				m_iErrorMsgID;
	std::wstring	m_wstrErrorMsg;

	CON_COPYCON_ASSIGNOP( KErrorMsgInfo, right )
	{
		m_iErrorMsgID = right.m_iErrorMsgID;
		m_wstrErrorMsg = right.m_wstrErrorMsg;

		return *this;
	}
};

DECL_DATA( KStatisticsKey )
{
    std::vector< int >				m_vecIntKey;
    std::vector< std::wstring >		m_vecStringKey;

	CON_COPYCON_ASSIGNOP( KStatisticsKey, right )
	{
		m_vecIntKey = right.m_vecIntKey;
		m_vecStringKey = right.m_vecStringKey;

		return *this;
	}

    bool operator<( const KStatisticsKey& right ) const
    {
        std::vector< int >::const_iterator vecIntIterLeft = m_vecIntKey.begin();
        std::vector< int >::const_iterator vecIntIterRight = right.m_vecIntKey.begin();
        std::vector< std::wstring >::const_iterator vecStringIterLeft = m_vecStringKey.begin();
        std::vector< std::wstring >::const_iterator vecStringIterRight = right.m_vecStringKey.begin();

        while( vecIntIterLeft != m_vecIntKey.end() )
        {
            if( vecIntIterRight == right.m_vecIntKey.end() )
            {
                return false;
            }

            if( ( *vecIntIterLeft ) < ( *vecIntIterRight ) )
            {
                return true;
            }
            else if( ( *vecIntIterRight ) < ( *vecIntIterLeft ) )
            {
                return false;
            }
            else
            {
                vecIntIterLeft++;
                vecIntIterRight++;
            }
        }

        if( vecIntIterRight != right.m_vecIntKey.end() )
        {
            return true;
        }

        while( vecStringIterLeft != m_vecStringKey.end() )
        {
            if( vecStringIterRight == right.m_vecStringKey.end() )
            {
                return false;
            }

            int iComp = ( *vecStringIterLeft ).compare( ( *vecStringIterRight ) );
            if( iComp < 0 )
            {
                return true;
            }
            else if( iComp > 0 )
            {
                return false;
            }
            else
            {
                vecStringIterLeft++;
                vecStringIterRight++;
            }
        }

        return false;
    }
};

//////////////////////////////////////////////////////////////////////////
DECL_DATA( KNPCUnitReq )
{
	int					m_UID;

	int					m_NPCID;	
	char				m_Level;
	VECTOR3				m_vPos;
	int					m_nStartPos;
	//{{ 2011. 11. 15	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM	
	std::vector< int >	m_vecPetrolLineIndex;
	std::vector< int >	m_vecPlayLineIndex;
	int					m_iGroupID;
	bool				m_bAggressive;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	bool				m_bActive;
	float				m_fDelayTime;

	char				m_KeyCode;
	bool				m_bFocusCamera;
	bool				m_bShowGage;
	VECTOR3				m_AddPos;
	bool				m_bHasBossGage;
	bool				m_bShowBossName;
	bool				m_bShowSubBossName;
	bool				m_bSiegeMode;
	bool				m_bIsRight;
	bool				m_bNoDrop;
	float				m_fUnitScale;

	//{{ dmlee 2008.11.13 PVP monster
	char				m_cTeamNum;			// ����ȣ
	char				m_cAIType;			// AI type: NormalAI, AllyAI
	UidType				m_iAllyUID;			// ���͸� ��ȯ�� ���� user unit uid
	//}} dmlee 2008.11.13 PVP monster

#ifdef SERV_TRAPPING_RANGER_TEST
	char			m_cAllyTeam;
#endif SERV_TRAPPING_RANGER_TEST

	//{{ 2009. 12. 18  ������	��������ġ����
	char				m_cMonsterGrade;
	//}}
	//{{ 2011. 01. 26	������	����Ʈ Ŭ���� ���� �߰�
#ifdef SERV_BOSS_GAUGE_HP_LINES
	u_short				m_usBossGaugeHPLines;
#endif SERV_BOSS_GAUGE_HP_LINES
	//}}
#ifdef SERV_ALLY_NPC
	bool				m_bAllyNpc;
#endif

	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	bool			m_bAttributeNpc;
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	//{{ 2012. 06. 20	�ڼ���	���� ��ȯ ī��
#ifdef SERV_MONSTER_SUMMON_CARD
	//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	char			m_cCreateMonsterType;
#else
	bool			m_bIsMonsterCard;
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
#endif SERV_MONSTER_SUMMON_CARD
	//}}

#ifdef SERV_MODIFY_DUNGEON_STAGING
	float			m_fShowBossNameDelayTime;
	bool			m_bStopAtStartState;
#endif //SERV_MODIFY_DUNGEON_STAGING
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	int					m_iBossGroupID;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

	KNPCUnitReq()
	{
		Init();
	}

	void Init()
	{
		m_UID				= -1;

		m_NPCID				= -1;	
		m_Level				= 0;
		m_vPos.x			= 0.0f;
		m_vPos.y			= 0.0f;
		m_vPos.z			= 0.0f;
		m_nStartPos			= 0;
		//{{ 2011. 11. 15	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_iGroupID			= 0;
		m_bAggressive		= true;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		m_bActive			= false;
		m_fDelayTime		= 0.0f;

		m_KeyCode			= -1;
		m_bFocusCamera		= false;
		m_bShowGage			= false;
		m_AddPos.x			= 0.0f;
		m_AddPos.y			= 0.0f;
		m_AddPos.z			= 0.0f;
		m_bHasBossGage		= false;
		m_bShowBossName		= false;
		m_bShowSubBossName	= false;
		m_bSiegeMode		= false;
		m_bIsRight			= false;
		m_bNoDrop			= false;
		m_fUnitScale		= -1.f;

		m_cTeamNum			= 2;		// default: CX2Room::TN_MONSTER
		m_cAIType			= 0;		// default: CX2NPCAI::NAT_NORMAL
		m_iAllyUID			= -1;

#ifdef SERV_TRAPPING_RANGER_TEST
		m_cAllyTeam		= 2;		// default: CX2Room::TN_MONSTER
#endif SERV_TRAPPING_RANGER_TEST

		//{{ 2009. 12. 18  ������	��������ġ����
		m_cMonsterGrade		= 0;		// default: CXSLUnitManager::MT_NORMAL_NPC
		//}}
		//{{ 2011. 01. 26	������	����Ʈ Ŭ���� ���� �߰�
#ifdef SERV_BOSS_GAUGE_HP_LINES
		m_usBossGaugeHPLines = 0;
#endif SERV_BOSS_GAUGE_HP_LINES
		//}}
#ifdef SERV_ALLY_NPC
		m_bAllyNpc			= false;
#endif
		//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
		m_bAttributeNpc		= false;
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

		//{{ 2012. 06. 20	�ڼ���	���� ��ȯ ī��
#ifdef SERV_MONSTER_SUMMON_CARD
		//{{ 2013. 04. 15	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
		m_cCreateMonsterType = 0;
#else
		m_bIsMonsterCard = false;
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}		
#endif SERV_MONSTER_SUMMON_CARD
		//}}
#ifdef SERV_MODIFY_DUNGEON_STAGING
		m_fShowBossNameDelayTime = -1.f;
		m_bStopAtStartState = false;
#endif //SERV_MODIFY_DUNGEON_STAGING
		//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		m_iBossGroupID			= 0;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}
	}
};

DECL_DATA( KNPCList )
{
	std::vector< KNPCUnitReq >	m_NPCList;

	CON_COPYCON_ASSIGNOP( KNPCList, right )
	{
		m_NPCList = right.m_NPCList;
		return *this;
	}
};

DECL_PACKET( ERM_NPC_UNIT_DIE_ACK )
{
	int		m_iOK;
	int		m_iNPCID;
};

DECL_DATA( KQuestReq )
{
	UidType		m_UnitUID;
	int			m_iQuestID;
};

DECL_DATA( KQuestAck )
{
	int									m_iOK;
	int									m_iQuestID;
	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >      m_mapItemInfo;
#endif SERV_QUEST_CLEAR_EXPAND
	//}}
};

DECL_PACKET( DBE_NEW_QUEST_REQ )
{
	UidType								m_UnitUID;
	int									m_iQuestID;
	int									m_iTalkNPCID;
	//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >            m_vecItemInfo;
#endif SERV_QUEST_CLEAR_EXPAND
	//}}
	//{{ 2012. 12. 14  ���� �̼� �ý��� ( �������� ����Ʈ ) - ��μ�
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	UidType								m_UserUID;
#endif SERV_ACCOUNT_MISSION_SYSTEM
	//}}
};
typedef KQuestAck	KDBE_NEW_QUEST_ACK;

typedef KQuestReq	KDBE_GIVE_UP_QUEST_REQ;

typedef KQuestAck	KDBE_GIVE_UP_QUEST_ACK;

DECL_PACKET( DBE_QUEST_COMPLETE_REQ )
{
	UidType								m_UnitUID;
	int									m_iQuestID;
	bool								m_bIsRepeat;

	bool								m_bIsChangeJob;	//08.02.20.hoons : ��������Ʈ�� ���� �����߰�.
	char								m_cChangeUnitClass;

	bool								m_bIsNew;
	std::map< int, KItemInfo >          m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >            m_vecItemInfo;
	//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	bool								m_bAutoComplete;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
	int									m_iNewDefaultSkill1;
	int									m_iNewDefaultSkill2;
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_SKILL_PAGE_SYSTEM
	int									m_iTheNumberOfSkillPagesAvailable;
#endif // SERV_SKILL_PAGE_SYSTEM

	KDBE_QUEST_COMPLETE_REQ()
	{
		m_UnitUID			= 0;
		m_iQuestID			= 0;
		m_bIsRepeat			= false;
		m_bIsChangeJob		= false;
		m_cChangeUnitClass	= 0;
		m_bIsNew			= false;
		//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		m_bAutoComplete		= false;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
		m_iNewDefaultSkill1 = 0;
		m_iNewDefaultSkill2 = 0;
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iTheNumberOfSkillPagesAvailable = 1;
#endif // SERV_SKILL_PAGE_SYSTEM
	}
};

DECL_PACKET( DBE_QUEST_COMPLETE_ACK )
{
	int									m_iOK;
	//{{ 2010. 04. 02  ������	��������Ʈ
#ifdef SERV_DAILY_QUEST
	KCompleteQuestInfo					m_kCompleteQuestInfo;
#else
	int									m_iQuestID;
#endif SERV_DAILY_QUEST
	//}}
	bool								m_bIsChangeJob;
	char								m_cChangeUnitClass;
	bool								m_bIsNew;

	std::map< int, KItemInfo >          m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >      m_mapItemInfo;
	//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	bool								m_bAutoComplete;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
	int									m_iNewDefaultSkill1;
	int									m_iNewDefaultSkill2;
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

	KDBE_QUEST_COMPLETE_ACK()
	{
		m_iOK				= 0;
		//{{ 2010. 04. 02  ������	��������Ʈ
#ifdef SERV_DAILY_QUEST
#else
		m_iQuestID			= 0;
#endif SERV_DAILY_QUEST
		//}}
		m_bIsChangeJob		= false;
		m_cChangeUnitClass	= 0;
		m_bIsNew			= false;
		//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		m_bAutoComplete		= false;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
		m_iNewDefaultSkill1 = 0;
		m_iNewDefaultSkill2 = 0;
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
	}
};

//{{ 2008. 10. 29  ������	8����Ʈ�� ��谪 ����
typedef std::map< KStatisticsKey, std::vector< __int64 > > KStatisticsData;
//}}

DECL_PACKET( ELG_REGISTER_UNIT_NOT )
{
	UidType				m_nUnitUID;
	u_int				m_uiKNMSerialNum;
	std::wstring		m_wstrUnitNickName;
	//{{ 2009. 4. 1  ������		ģ������
	bool				m_bDenyFriendShip;
	//}}
    std::set< UidType > m_setFriendUID;

	KELG_REGISTER_UNIT_NOT()
	{
		m_nUnitUID = 0;
		m_uiKNMSerialNum = 0;
		m_bDenyFriendShip = false;
	}
};

#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-03-31
DECL_DATA( KLocalRankingUserInfo )
{
	UidType			m_iUserUID;
	int				m_iCategory;
	__time64_t		m_iBirth;
	std::wstring	m_wstrProfile;
	byte			m_byteFilter;
	byte			m_byteGender;

	KLocalRankingUserInfo()
		: m_iUserUID( 0 )
		, m_iCategory( -1 )
		, m_iBirth( 0 )
		, m_byteFilter( 0 )
		, m_byteGender( 0 )
	{
	}

	KLocalRankingUserInfo&	operator=( const KLocalRankingUserInfo& rhs )
	{
		m_iUserUID		= rhs.m_iUserUID;
		m_iCategory		= rhs.m_iCategory;
		m_iBirth		= rhs.m_iBirth;
		m_wstrProfile	= rhs.m_wstrProfile;
		m_byteFilter	= rhs.m_byteFilter;
		m_byteGender	= rhs.m_byteGender;
		return *this;
	}

	bool	operator==( const KLocalRankingUserInfo& rhs ) const
	{
		if( m_iUserUID != rhs.m_iUserUID )		return false;
		if( m_iCategory != rhs.m_iCategory )	return false;
		if( m_iBirth != rhs.m_iBirth )			return false;
		if( m_byteFilter != rhs.m_byteFilter )	return false;
		if( m_byteGender != rhs.m_byteGender )	return false;
		return m_wstrProfile == rhs.m_wstrProfile;
	}

	bool	operator!=( const KLocalRankingUserInfo& rhs ) const
	{
		return !( *this == rhs );
	}

	void	clear( void )
	{
		m_iUserUID		= 0;
		m_iCategory		= -1;
		m_iBirth		= 0;
		m_wstrProfile.clear();
		m_byteFilter	= 0;
		m_byteGender	= 0;
	}
};
#endif	// SERV_LOCAL_RANKING_SYSTEM

DECL_DATA( KConnectionUnitInfo )
{
	UidType			m_iUnitUID;
	int				m_nKNMSN;
	char            m_cUnitClass;
	u_char			m_ucLevel;
	std::wstring	m_wstrCharName;
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	char			m_cRank;
#else
	int				m_iRating;
#endif SERV_2012_PVP_SEASON2
	//}}
	int				m_iRPoint;
#else
	int				m_iPVPEmblem;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	UINT			m_uiPVPRanking;
	int				m_iWin;
	int				m_iLose;
	UINT			m_uiKillNum;

	//{{ 2009. 4. 17  ������	Ȯ��
	int				m_iChannelID;
	//}}
	char			m_cState;
	int				m_iStateCode;	
	
	UidType			m_iPartyUID;
	UidType			m_iRoomUID;
	bool			m_bPublic;
	bool			m_bCanIntrude;
	int				m_iDungeonID;
	//{{ 2009. 4. 21  ������	Ȯ��
	char			m_iRoomListID;
	//}}
	//{{ 2009. 10. 23  ������	���
#ifdef GUILD_TEST
	std::wstring	m_wstrGuildName;
#endif GUILD_TEST
	//}}
#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-03-31
	KLocalRankingUserInfo	m_kLocalRankingUserInfo;
#endif	// SERV_LOCAL_RANKING_SYSTEM
};

DECL_PACKET( ELG_GET_UNIT_INFO_REQ )
{
	unsigned short	m_usEventID;
	UidType			m_iDemandUserUID;
	UidType			m_iDemandGSUID;
	int				m_nKNMSN;
	std::wstring	m_wstrNickName;
};

DECL_PACKET( ELG_GET_UNIT_INFO_ACK )
{
	unsigned short		m_usEventID;
	UidType				m_iDemandUserUID;
	UidType				m_iDemandGSUID;

	int					m_iOK;
	KConnectionUnitInfo	m_kCUnitInfo;
};

/////////////////////////////////////////////////////////////////////////
DECL_DATA( KSquareInfo )
{
	char					m_cSquareType;
	//{{ 2010. 11. 1	������	���� ����Ʈ ����
#ifdef SERV_SQUARE_LIST_SERV_UID
	char					m_cSortIndex;
#endif SERV_SQUARE_LIST_SERV_UID
	//}}
	UidType					m_iSquareUID;

	std::wstring			m_wstrRoomName;

	char					m_MaxSlot;
	char					m_JoinSlot;

	std::wstring			m_RelayIP;
	int						m_Port;	

	CON_COPYCON_ASSIGNOP( KSquareInfo, right )
	{
		m_cSquareType	= right.m_cSquareType;
		//{{ 2010. 11. 1	������	���� ����Ʈ ����
#ifdef SERV_SQUARE_LIST_SERV_UID
		m_cSortIndex	= right.m_cSortIndex;
#endif SERV_SQUARE_LIST_SERV_UID
		//}}
		m_iSquareUID	= right.m_iSquareUID;
		m_wstrRoomName	= right.m_wstrRoomName;
		m_MaxSlot		= right.m_MaxSlot;
		m_JoinSlot		= right.m_JoinSlot;
		m_RelayIP		= right.m_RelayIP;
		m_Port			= right.m_Port;

		return *this;
	}
};

DECL_DATA( KSquareUserInfo )
{
	//08.02.20. hoons :�̰��� ������ �߰��Ҷ����� KGSUser::GetSquareUserInfo()�Լ����� �ʱ�ȭ�� ���־�� �Ѵ�.
    UidType                             	m_iGSUID;    
    UidType                             	m_iUnitUID;
    std::wstring                        	m_wstrNickName;

    std::wstring                        	m_wstrIP;
    unsigned short                      	m_usPort;

    char                                	m_cUnitClass; 
    UCHAR                               	m_ucLevel;
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	char									m_cRank;
#else
	int										m_iRating;
#endif SERV_2012_PVP_SEASON2
	//}}
#else
	char									m_cPVPEmblem;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	bool									m_bIsParty;

	char									m_cPersonalShopState;
	char									m_cPersonalShopType;
	std::wstring							m_wstrPersonalShopName;

	std::vector< KInventoryItemSimpleInfo > m_vecEquippedItem;

	//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
	int										m_iTitleID;
#else
	short									m_sTitleID;
#endif SERV_TITLE_DATA_SIZE
	//}}

	//{{ 2009. 9. 16  ������	���
#ifdef GUILD_TEST
	std::wstring							m_wstrGuildName;
#endif GUILD_TEST
	//}}

	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
	int										m_iED;
#endif SERV_ED_MONITORING_IN_GAME
	//}}

#ifdef SERV_GROW_UP_SOCKET
	int										m_iEventQuestClearCount;
	int										m_iExchangeCount;
#endif // SERV_GROW_UP_SOCKET

#ifdef SERV_4TH_ANNIVERSARY_EVENT
	unsigned int							m_iAccountPVPLoseCount;
#endif SERV_4TH_ANNIVERSARY_EVENT

	CON_COPYCON_ASSIGNOP( KSquareUserInfo, right )
	{
		m_iGSUID				= right.m_iGSUID;		
		m_iUnitUID				= right.m_iUnitUID;
		m_wstrNickName			= right.m_wstrNickName;
		m_wstrIP				= right.m_wstrIP;
		m_usPort				= right.m_usPort;
		m_cUnitClass			= right.m_cUnitClass;
		m_ucLevel				= right.m_ucLevel;
		//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		m_cRank					= right.m_cRank;
#else
		m_iRating				= right.m_iRating;;
#endif SERV_2012_PVP_SEASON2
		//}}
#else
		m_cPVPEmblem			= right.m_cPVPEmblem;;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		m_bIsParty				= right.m_bIsParty;
		m_cPersonalShopState	= right.m_cPersonalShopState;
		m_cPersonalShopType		= right.m_cPersonalShopType;
		m_wstrPersonalShopName	= right.m_wstrPersonalShopName;
		m_vecEquippedItem		= right.m_vecEquippedItem;
		//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
		m_iTitleID				= right.m_iTitleID;
#else
		m_sTitleID				= right.m_sTitleID;
#endif SERV_TITLE_DATA_SIZE
		//}}

		//{{ 2009. 9. 16  ������	���
#ifdef GUILD_TEST
		m_wstrGuildName			= right.m_wstrGuildName;
#endif GUILD_TEST
		//}}

		//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
		m_iED					= right.m_iED;
#endif SERV_ED_MONITORING_IN_GAME
		//}}
#ifdef SERV_GROW_UP_SOCKET
		m_iEventQuestClearCount		= right.m_iEventQuestClearCount;
		m_iExchangeCount			= right.m_iExchangeCount;
#endif // SERV_GROW_UP_SOCKET

#ifdef SERV_4TH_ANNIVERSARY_EVENT
		m_iAccountPVPLoseCount		= right.m_iAccountPVPLoseCount;
#endif SERV_4TH_ANNIVERSARY_EVENT
		return *this;
	}
};

DECL_DATA( KPartyInfo )
{
	//{{ 2011. 06. 20	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	enum PARTY_TYPE
	{
		PT_NONE = 0,
		PT_DUNGEON_PARTY,
		PT_PVP_PARTY,
	};
#endif SERV_PVP_NEW_SYSTEM
	//}}
	
	// ��Ƽ �⺻ ����
	UidType			m_iPartyUID;
	//{{ 2011. 06. 20	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	char			m_cPartyType;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	std::wstring	m_wstrPartyName;
	char			m_JoinSlot;
	char			m_MaxSlot;
	bool			m_bPublic;

	// ���� ��Ƽ ����
	int				m_iDungeonID;
	char			m_DifficultyLevel;
	char			m_cGetItemType;
	char			m_cDungeonMode; // ��д��� ����
	//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	short			m_sPartyFever;
#else
	UCHAR			m_ucPartyFever;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	
	//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	bool			m_bCheckLowLevel;
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

	// ���� ��Ƽ ����
	//{{ 2011. 06. 20	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	char			m_cPVPGameType; // CXSLRoom::PVP_GAME_TYPE
#endif SERV_PVP_NEW_SYSTEM
	//}}

	// ���� ó�� ����
	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	bool			m_bComeBackBuff;
	std::wstring	m_wsrtComeBackBuffEnd;
#endif SERV_COME_BACK_USER_REWARD
	//}}

	KPartyInfo()
	{
		m_iPartyUID			= 0;
		//{{ 2011. 06. 20	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		m_cPartyType		= PT_NONE;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		m_JoinSlot			= 0;
		m_MaxSlot			= 0;
		m_bPublic			= true;
		m_iDungeonID		= 0;
		m_DifficultyLevel	= 0;
		m_cGetItemType		= 1;	//�����Դ� ������� �⺻������ �Ѵ�.
		m_cDungeonMode		= 0;
		//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_sPartyFever		= 0;
#else
		m_ucPartyFever		= 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}		
		//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
		m_bCheckLowLevel	= false;
#endif SERV_DUNGEON_DIFFICULTY_REVISION
		//}}		
		//{{ 2011. 06. 20	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		m_cPVPGameType		= 0;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
		m_bComeBackBuff		= false;
		m_wsrtComeBackBuffEnd = L"";
#endif SERV_COME_BACK_USER_REWARD
		//}}
	}
};


//{{ 2012. 01. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
DECL_DATA( KCreatePartyInfo )
{
	char							m_cPartyType;		// KPartyInfo::PARTY_TYPE
	int								m_iDungeonID;
	char							m_DifficultyLevel;	// CXSLDungeon::DIFFICULTY_LEVEL
	char							m_cGetItemType;		// CXSLRoom::DUNGEON_GET_ITEM_TYPE
	char							m_cDungeonMode;		// CXSLDungeon::DUNGEON_MODE
	bool							m_bCheckLowLevel;	// �ּ� ���� ����
	char							m_cPVPGameType;		// CXSLRoom::PVP_GAME_TYPE

	KCreatePartyInfo()
	{
		m_cPartyType = KPartyInfo::PT_NONE;
		m_iDungeonID = 0;
		m_DifficultyLevel = -1;
		m_cGetItemType = 0;
		m_cDungeonMode = 0;
		m_bCheckLowLevel = false;
		m_cPVPGameType = -1;
	}
};
#endif SERV_BATTLE_FIELD_SYSTEM
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 01. 26  ������	��Ƽ����
#ifdef SERV_CHANNEL_PARTY
//////////////////////////////////////////////////////////////////////////

DECL_DATA( KPartyUserInfo )
{
	UidType					m_iGSUID;
	int						m_iChannelID;
	UidType					m_iFieldUID;	
	UidType					m_iUnitUID;
	char					m_cUnitClass;
	std::wstring			m_wstrNickName;
	UCHAR					m_ucLevel;
	bool					m_bIsGuestUser;
	UCHAR					m_cState;
	int						m_iStateCode; // m_cState���� CX2Unit::CUS_FIELD_MAP �̸� mapID�̴�.
	bool					m_bReady;
	bool					m_bHost;
	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	bool					m_bComeBack;
	std::wstring			m_wsrtComeBackEnd;
#endif SERV_COME_BACK_USER_REWARD
	//}} 
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	char					m_cRank;
	int						m_iOfficialMatchCnt;
#endif SERV_2012_PVP_SEASON2
	//}}
	int						m_iRating;
	int						m_iRPoint;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2012. 05. 29	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KPartyMemberStatus		m_kGamePlayStatus;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	std::wstring	       	m_wstrIP;
	USHORT                 	m_usPort;
	DWORD					m_dwPingScore;
#endif SERV_CHOOSE_FASTEST_HOST
	//}
	//{{ 2013. 1. 8	�ڼ���	Merge ����IP ���� ���н� ����IP�� �õ�( ������ )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
	std::wstring			m_wstrInternalIP;				
	USHORT					m_usInternalPort;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
	//}}

	KPartyUserInfo()
	{
		m_iGSUID		= 0;
		m_iChannelID	= 0;
		m_iFieldUID		= 0;
		m_iUnitUID		= 0;
		m_cUnitClass	= 0;
		m_ucLevel		= 0;
		m_bIsGuestUser	= false;
		m_cState		= 0;
		m_iStateCode	= 0;
		m_bReady		= false;
		m_bHost			= false;
		//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
		m_bComeBack		= false;
		m_wsrtComeBackEnd	= L"";
#endif SERV_COME_BACK_USER_REWARD
		//}} 
		//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		m_cRank			= 0;
		m_iOfficialMatchCnt	= 0;
#endif SERV_2012_PVP_SEASON2
		//}}
		m_iRating		= 0;
		m_iRPoint		= 0;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
		m_wstrIP		= L"";
		m_usPort		= 0;
		m_dwPingScore	= 99999;
#endif SERV_CHOOSE_FASTEST_HOST
		//}
		//{{ 2013. 1. 8	�ڼ���	Merge ����IP ���� ���н� ����IP�� �õ�( ������ )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
		m_usInternalPort	= 0;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
		//}}
	}
};

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

DECL_DATA( KPartyUserInfo )
{
	UidType                 m_iUnitUID;
	std::wstring            m_wstrNickName;
	char                    m_cUnitClass;
	UCHAR                   m_ucLevel;
	UCHAR					m_cState;
	int                     m_iStateCode;
	bool					m_bReady;
	bool					m_bHost;
	
	KPartyUserInfo()
	{
		m_iUnitUID		= 0;
		m_wstrNickName.clear();
		m_cUnitClass	= 0;
		m_ucLevel		= 0;
		m_cState		= 0;
		m_iStateCode	= 0;
		m_bReady		= false;
		m_bHost			= false;
	}

};

//////////////////////////////////////////////////////////////////////////
#endif SERV_CHANNEL_PARTY
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2012. 05. 31	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
DECL_DATA( KOldPartyInfo )
{
	UidType					m_iOldPartyUID;			// �ڵ���Ƽ ��û ����� ��ƼUID
	//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	UidType					m_iOldHostUnitUID;		// �ڵ���Ƽ ��û ����� ��Ƽ�� UnitUID
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}
	std::set< UidType >		m_setOldPartyMember;	// �ڵ���Ƽ ��û ����� ��Ƽ ��� ����Ʈ
	short					m_sOldPartyFever;		// �ڵ���Ƽ ��û ����� ��Ƽ �ǹ�

	KOldPartyInfo()
	{
		m_iOldPartyUID = 0;
		//{{ 2012. 11. 13	������		��Ƽ ������ ��Ƽ�� ���� ����
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		m_iOldHostUnitUID = 0;
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		m_sOldPartyFever = 0;
	}
};
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

DECL_DATA( KPartyListUserInfo )
{
	int						m_iChannelID;
	UidType					m_iUnitUID;
	std::wstring            m_wstrNickName;
	char                    m_cUnitClass;
	UCHAR                   m_ucLevel;
	UCHAR					m_cState;
	int						m_iStateCode; // m_cState���� CX2Unit::CUS_FIELD_MAP �̸� mapID�̴�.
	bool					m_bHost;

	KPartyListUserInfo()
	{
		m_iChannelID	= 0;
		m_iUnitUID		= 0;
		m_wstrNickName.clear();
		m_cUnitClass	= 0;
		m_ucLevel		= 0;
		m_cState		= 0;
		m_iStateCode	= 0;
		m_bHost			= false;
	}

	COPYCON_ASSIGNOP( KPartyListUserInfo, right )
	{
		m_iChannelID	= right.m_iChannelID;
		m_iUnitUID		= right.m_iUnitUID;
		m_wstrNickName	= right.m_wstrNickName;
		m_cUnitClass	= right.m_cUnitClass;
		m_ucLevel		= right.m_ucLevel;
		m_cState		= right.m_cState;
		m_iStateCode	= right.m_iStateCode;
		m_bHost			= right.m_bHost;

		return *this;
	}
};

DECL_DATA( KPartyListInfo )
{
	KPartyInfo						m_kPartyInfo;
	std::vector<KPartyListUserInfo>	m_vecPartyListUserInfo;
};

DECL_DATA( KPartyRoomUserInfo )
{
	KRoomUserInfo						m_kRoomUserInfo;
	std::vector< UidType >				m_vecStudentUnitUID;
	//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KOldPartyInfo						m_kOldPartyInfo;
	bool								m_bAutoPartyBonus;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 07. 03	������		���� �α� �÷� �߰�
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	std::wstring						m_wstrChannelIP;
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}

	//{{ 2009. 6. 10  ������	���ӽ�������
	int									m_iCurEXP;
	int									m_iCurED;
	//}}
	//{{ �ڵ� ��Ƽ�� ���� ���� �� ��Ī ��� �ð� �α� �߰� - ��μ�
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
	int									m_iAutoPartyWaitTime;
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
	//}
	KPartyRoomUserInfo()
	{
		//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_bAutoPartyBonus = false;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		m_iCurEXP = 0;
		m_iCurED = 0;
		//{{ �ڵ� ��Ƽ�� ���� ���� �� ��Ī ��� �ð� �α� �߰� - ��μ�
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		m_iAutoPartyWaitTime = 0;
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		//}
	}
};

DECL_DATA( KTradeUserInfo )
{
	UidType					m_iGSUID;
	UidType					m_iUserUID;
	UidType					m_iUnitUID;
	std::wstring			m_wstrNickName;
	//{{ 2009. 4. 28  ������	�ִ� ED����
	int						m_iED;
	//}}

	CON_COPYCON_ASSIGNOP( KTradeUserInfo, right )
	{
		m_iGSUID = right.m_iGSUID;
		m_iUserUID = right.m_iUserUID;
		m_iUnitUID = right.m_iUnitUID;
		m_wstrNickName = right.m_wstrNickName;
		//{{ 2009. 4. 28  ������	�ִ� ED����
		m_iED = right.m_iED;
		//}}

		return *this;
	}
};

DECL_DATA( KPersonalShopUserInfo )
{
	UidType					m_iGSUID;
	UidType					m_iUserUID;
	UidType					m_iUnitUID;
	std::wstring			m_wstrNickName;
	bool					m_bIsSquare;

	KPersonalShopUserInfo()
	{
		m_iGSUID		= 0;
		m_iUserUID		= 0;
		m_iUnitUID		= 0;
		m_bIsSquare		= false;
	}
};

DECL_PACKET( ERM_SQUARE_LIST_INFO_NOT )
{
	int							m_iCode;
	KSquareInfo					m_kSquareInfo;
};

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
DECL_PACKET( DBE_INSERT_SKILL_REQ )
{
	UidType				m_iUnitUID;
	int					m_iSkillID;
	int					m_iSkillLevel;
	int					m_iSkillCSPoint;
	int					m_iCSPoint;
};

DECL_PACKET( DBE_INSERT_SKILL_ACK )
{
	int					m_iOK;
	int					m_iSkillID;
	int					m_iSkillLevel;
	int					m_iSkillCSPoint;
	int					m_iCSPoint;
};
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

typedef std::pair< UidType, UidType > KGSCID;

DECL_DATA( KNMUnitInfo )
{
	UidType                 m_iUnitUID;
	int                     m_iState;	
	int						m_iDungeonID;
	UidType					m_iRoomUID;
};

DECL_PACKET( ELG_KNM_TOGETHER_BY_UNIT_INFO_REQ )
{
	UidType			m_iDemandGSUID;
	UidType			m_iDemandUserUID;
};

DECL_PACKET( ELG_KNM_TOGETHER_BY_UNIT_INFO_ACK )
{
	int				m_iOK;

	UidType			m_iDemandGSUID;
	UidType			m_iDemandUserUID;

	KNMUnitInfo		m_kUnitInfo;
};

//{{ 2009. 12. 2  ������	�ڵ�����
//DECL_DATA( KNXShopOrder )
//{
//    int			order_no;
//    int			package_product_no;
//    int			product_no;
//    std::string product_name;
//    std::string product_dbid;
//    short		product_expireDay;
//    short 		product_pieces;
//    short 		order_quantity;
//    short 		receive_order_quantity;
//    short 		remain_order_quantity;
//    bool		is_order_package;
//    int			sender_game_no;
//    int			sender_game_server;
//    std::string sender_game_id;
//    std::string present_message;
//    std::string product_attribute0;
//    std::string product_attribute1;
//    std::string product_attribute2;
//    std::string product_attribute3;
//    std::string product_attribute4;
//    std::string register_date;
//    int			is_order_read;
//    std::string product_type;
//
//	CON_COPYCON_ASSIGNOP( KNXShopOrder, right )
//	{
//		order_no = right.order_no;
//		package_product_no = right.package_product_no;
//		product_no = right.product_no;
//		product_name = right.product_name;
//		product_dbid = right.product_dbid;
//		product_expireDay = right.product_expireDay;
//		product_pieces = right.product_pieces;
//		order_quantity = right.order_quantity;
//		receive_order_quantity = right.receive_order_quantity;
//		remain_order_quantity = right.remain_order_quantity;
//		is_order_package = right.is_order_package;
//		sender_game_no = right.sender_game_no;
//		sender_game_server = right.sender_game_server;
//		sender_game_id = right.sender_game_id;
//		present_message = right.present_message;
//		product_attribute0 = right.product_attribute0;
//		product_attribute1 = right.product_attribute1;
//		product_attribute2 = right.product_attribute2;
//		product_attribute3 = right.product_attribute3;
//		product_attribute4 = right.product_attribute4;
//		register_date = right.register_date;
//		is_order_read = right.is_order_read;
//		product_type = right.product_type;
//
//		return *this;
//	}
//};

//DECL_DATA( KNXShopPackage )
//{
//    int			order_no;
//    int			package_product_no;
//    int			product_no;
//    std::string product_name;
//    std::string product_dbid;
//    short		product_expireDay;
//    std::string product_attribute0;
//    std::string product_attribute1;
//    std::string product_attribute2;
//    std::string product_attribute3;
//    std::string product_attribute4;
//
//	CON_COPYCON_ASSIGNOP( KNXShopPackage, right )
//	{
//		order_no = right.order_no;
//		package_product_no = right.package_product_no;
//		product_no = right.product_no;
//		product_name = right.product_name;
//		product_dbid = right.product_dbid;
//		product_expireDay = right.product_expireDay;
//		product_attribute0 = right.product_attribute0;
//		product_attribute1 = right.product_attribute1;
//		product_attribute2 = right.product_attribute2;
//		product_attribute3 = right.product_attribute3;
//		product_attribute4 = right.product_attribute4;
//
//		return *this;
//	}
//};

//DECL_DATA( KNXShopLottery )
//{
//    int			order_no;
//    int			lottery_product_no;
//    int			product_no;
//    std::string product_name;
//    std::string product_dbid;
//    short		product_expireDay;
//    std::string product_attribute0;
//    std::string product_attribute1;
//    std::string product_attribute2;
//    std::string product_attribute3;
//    std::string product_attribute4;
//
//	CON_COPYCON_ASSIGNOP( KNXShopLottery, right )
//	{
//		order_no = right.order_no;
//		lottery_product_no = right.lottery_product_no;
//		product_no = right.product_no;
//		product_name = right.product_name;
//		product_dbid = right.product_dbid;
//		product_expireDay = right.product_expireDay;
//		product_attribute0 = right.product_attribute0;
//		product_attribute1 = right.product_attribute1;
//		product_attribute2 = right.product_attribute2;
//		product_attribute3 = right.product_attribute3;
//		product_attribute4 = right.product_attribute4;
//
//		return *this;
//	}
//};
//}}

DECL_DATA( KEXPData )
{
	int		m_iEXP;
	int		m_iPartyEXP;
	int		m_iSocketOptEXP;
	//{{ 2012. 12. 18	������	�ʵ忡���� ����ġ �̺�Ʈ ����
#ifdef SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
	int		m_iEventBonusEXP;
#endif SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
	//}}

	KEXPData()
	{
		m_iEXP		= 0;
		m_iPartyEXP	= 0;
		m_iSocketOptEXP = 0;
		//{{ 2012. 12. 18	������	�ʵ忡���� ����ġ �̺�Ʈ ����
#ifdef SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
		m_iEventBonusEXP = 0;
#endif SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
		//}}
	}

	int SumEXP() const
	{
		//{{ 2012. 12. 18	������	�ʵ忡���� ����ġ �̺�Ʈ ����
#ifdef SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
		return ( m_iEXP + m_iPartyEXP + m_iSocketOptEXP + m_iEventBonusEXP );
#else
		return ( m_iEXP + m_iPartyEXP + m_iSocketOptEXP );
#endif SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
		//}}
	}
};

DECL_DATA( KNXBTCategoryInfo )
{
    unsigned long       m_ulCategoryNo;
    std::wstring        m_wstrCategoryName;
    unsigned long       m_ulParentCategoryNo;
    unsigned long       m_ulDisplayNo;

	CON_COPYCON_ASSIGNOP( KNXBTCategoryInfo, right )
	{
		m_ulCategoryNo = right.m_ulCategoryNo;
		m_wstrCategoryName = right.m_wstrCategoryName;
		m_ulParentCategoryNo = right.m_ulParentCategoryNo;
		m_ulDisplayNo = right.m_ulDisplayNo;
		return *this;
	}
};

DECL_DATA( KNXBTBonusProductInfo )
{
    unsigned long       m_ulProductNo;
    std::wstring        m_wstrExtend;

	CON_COPYCON_ASSIGNOP( KNXBTBonusProductInfo, right )
	{
		m_ulProductNo = right.m_ulProductNo;
		m_wstrExtend = right.m_wstrExtend;
		return *this;
	}
};

//{{ 2008. 7. 10  ������  ������Ŷ ����
DECL_DATA( KNXBTBonusProductInfoPickUp )
{
    unsigned long		m_ulBonusProduct;
	unsigned long		m_ulBonusAmount;
};
//}}

DECL_DATA( KNXBTProductInfo )
{
    unsigned long                           m_ulProductNo;
    unsigned long                           m_ulRelationProductNo;
	//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���	
    short									m_sProductExpire;
	//}}
    unsigned short                          m_usProductPieces;
    std::wstring                            m_wstrProductID;
    std::wstring                            m_wstrProductGUID;
    unsigned long                           m_ulPaymentType;
    std::wstring                            m_wstrProductType;
    unsigned long                           m_ulSalePrice;
    unsigned long                           m_ulCategoryNo;	
    std::vector< KNXBTBonusProductInfo >    m_vecBonusProductInfo;

	CON_COPYCON_ASSIGNOP( KNXBTProductInfo, right )
	{
		m_ulProductNo = right.m_ulProductNo;
		m_ulRelationProductNo = right.m_ulRelationProductNo;
		//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���		
		m_sProductExpire = right.m_sProductExpire;
		//}}
		m_usProductPieces = right.m_usProductPieces;
		m_wstrProductID = right.m_wstrProductID;
		m_wstrProductGUID = right.m_wstrProductGUID;
		m_ulPaymentType = right.m_ulPaymentType;
		m_wstrProductType = right.m_wstrProductType;
		m_ulSalePrice = right.m_ulSalePrice;
		m_ulCategoryNo = right.m_ulCategoryNo;		
		m_vecBonusProductInfo = right.m_vecBonusProductInfo;

		return *this;
	}

	//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
	bool IsInfinityProduct() const { return ( m_sProductExpire == 0  ||  m_sProductExpire == -1 ); }
	short GetProductExpire() const { return ( m_sProductExpire == -1 ? 0 : m_sProductExpire ); }
	//}}
};

DECL_DATA( KNXBTPurchaseReqInfo )
{
	unsigned long   m_ulProductNo;
	unsigned short	m_usOrderQuantity;
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-23	// �ڼ���
	int				m_iCouponCardNo;
#endif // SERV_NEXON_COUPON_SYSTEM

	KNXBTPurchaseReqInfo()
		: m_ulProductNo( 0 )
		, m_usOrderQuantity( 0 )
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-23	// �ڼ���
		, m_iCouponCardNo( 0 )
#endif // SERV_NEXON_COUPON_SYSTEM
	{
	}
};

DECL_DATA( KNXBTPurchaseAckInfo )
{
    unsigned long	m_ulProductNo;
	unsigned short	m_usOrderQuantity;
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-23	// �ڼ���
	unsigned long	m_ulCouponUseApproval;
#endif // SERV_NEXON_COUPON_SYSTEM
	std::wstring	m_wstrExtendValue;

	CON_COPYCON_ASSIGNOP( KNXBTPurchaseAckInfo, right )
	{
		m_ulProductNo = right.m_ulProductNo;
		m_usOrderQuantity = right.m_usOrderQuantity;
#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-23	// �ڼ���
		m_ulCouponUseApproval	= right.m_ulCouponUseApproval;
#endif // SERV_NEXON_COUPON_SYSTEM
		m_wstrExtendValue = right.m_wstrExtendValue;

		return *this;
	}
};

DECL_DATA( KNXBTOrderInfo )
{
    unsigned long       m_ulOrderNo;
    unsigned long       m_ulProductNo;
    BYTE                m_byteProductKind;
    std::wstring        m_wstrProductName;
    std::wstring        m_wstrProductID;
	//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
    short				m_sProductExpire;
	//}}
    unsigned short      m_usProductPieces;
    unsigned short      m_usOrderQuantity;
    unsigned short      m_usRemainOrderQuantity;
    BYTE                m_byteIsPresent;
    BYTE                m_byteIsRead;
    BYTE                m_byteSenderServerNo;
    std::wstring        m_wstrSenderGameID;
    std::wstring        m_wstrSenderPresentMessage;
    std::wstring        m_wstrProductAttribute0;
    std::wstring        m_wstrProductAttribute1;
    std::wstring        m_wstrProductAttribute2;
    std::wstring        m_wstrProductAttribute3;
    std::wstring        m_wstrProductAttribute4;
    std::wstring        m_wstrExtendValue;

	CON_COPYCON_ASSIGNOP( KNXBTOrderInfo, right )
	{
		m_ulOrderNo = right.m_ulOrderNo;
		m_ulProductNo = right.m_ulProductNo;
		m_byteProductKind = right.m_byteProductKind;
		m_wstrProductName = right.m_wstrProductName;
		m_wstrProductID = right.m_wstrProductID;
		//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
		m_sProductExpire = right.m_sProductExpire;
		//}}
		m_usProductPieces = right.m_usProductPieces;
		m_usOrderQuantity = right.m_usOrderQuantity;
		m_usRemainOrderQuantity = right.m_usRemainOrderQuantity;
		m_byteIsPresent = right.m_byteIsPresent;
		m_byteIsRead = right.m_byteIsRead;
		m_byteSenderServerNo = right.m_byteSenderServerNo;
		m_wstrSenderGameID = right.m_wstrSenderGameID;
		m_wstrSenderPresentMessage = right.m_wstrSenderPresentMessage;
		m_wstrProductAttribute0 = right.m_wstrProductAttribute0;
		m_wstrProductAttribute1 = right.m_wstrProductAttribute1;
		m_wstrProductAttribute2 = right.m_wstrProductAttribute2;
		m_wstrProductAttribute3 = right.m_wstrProductAttribute3;
		m_wstrProductAttribute4 = right.m_wstrProductAttribute4;
		m_wstrExtendValue = right.m_wstrExtendValue;

		return *this;
	}

	//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
	short GetProductExpire() const { return ( m_sProductExpire == -1 ? 0 : m_sProductExpire ); }
	//}}
};

DECL_DATA( KNXBTPackageInfo )
{
    unsigned long       m_ulOrderNo;
    unsigned long       m_ulPackageProductNo;
    unsigned long       m_ulProductNo;
    std::wstring        m_wstrProductName;
    std::wstring        m_wstrProductID;
	//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���	
    short				m_sProductExpire;
	//}}
    unsigned short      m_usProductPieces;
    std::wstring        m_wstrProductAttribute0;
    std::wstring        m_wstrProductAttribute1;
    std::wstring        m_wstrProductAttribute2;
    std::wstring        m_wstrProductAttribute3;
    std::wstring        m_wstrProductAttribute4;

	CON_COPYCON_ASSIGNOP( KNXBTPackageInfo, right )
	{
		m_ulOrderNo = right.m_ulOrderNo;
		m_ulPackageProductNo = right.m_ulPackageProductNo;
		m_ulProductNo = right.m_ulProductNo;
		m_wstrProductName = right.m_wstrProductName;
		m_wstrProductID = right.m_wstrProductID;
		//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���		
		m_sProductExpire = right.m_sProductExpire;
		//}}
		m_usProductPieces = right.m_usProductPieces;
		m_wstrProductAttribute0 = right.m_wstrProductAttribute0;
		m_wstrProductAttribute1 = right.m_wstrProductAttribute1;
		m_wstrProductAttribute2 = right.m_wstrProductAttribute2;
		m_wstrProductAttribute3 = right.m_wstrProductAttribute3;
		m_wstrProductAttribute4 = right.m_wstrProductAttribute4;

		return *this;
	}

	//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
	short GetProductExpire() const { return ( m_sProductExpire == -1 ? 0 : m_sProductExpire ); }
	//}}
};

//{{ 2008. 7. 28  ������	��Ű�� 
DECL_DATA( KNXBTPickUpPackageInfo )
{
	unsigned long       m_ulProductNo;
	std::wstring		m_wstrProductID;
	//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���	
	short				m_sProductExpire;
	//}}
	unsigned short		m_usProductPieces;

#ifdef SERV_GLOBAL_CASH_PACKAGE
	int					m_iSubProductNo;
#endif //SERV_GLOBAL_CASH_PACKAGE

	CON_COPYCON_ASSIGNOP( KNXBTPickUpPackageInfo, right )
	{
		m_ulProductNo = right.m_ulProductNo;
		m_wstrProductID = right.m_wstrProductID;
		//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���		
		m_sProductExpire = right.m_sProductExpire;
		//}}
		m_usProductPieces = right.m_usProductPieces;

#ifdef SERV_GLOBAL_CASH_PACKAGE
		m_iSubProductNo = right.m_iSubProductNo;
#endif //SERV_GLOBAL_CASH_PACKAGE

		return *this;
	}

	//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
	short GetProductExpire() const { return ( m_sProductExpire == -1 ? 0 : m_sProductExpire ); }
	//}}
};
//}}

DECL_DATA( KNXBTLotteryInfo )
{
    unsigned long       m_ulOrderNo;
    unsigned long       m_ulLotteryProductNo;
    unsigned long       m_ulProductNo;
    std::wstring        m_wstrProductName;
    std::wstring        m_wstrProductID;
	//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���	
    short				m_sProductExpire;
	//}}
    unsigned short      m_usProductPieces;
    std::wstring        m_wstrProductAttribute0;
    std::wstring        m_wstrProductAttribute1;
    std::wstring        m_wstrProductAttribute2;
    std::wstring        m_wstrProductAttribute3;
    std::wstring        m_wstrProductAttribute4;

	CON_COPYCON_ASSIGNOP( KNXBTLotteryInfo, right )
	{
		m_ulOrderNo = right.m_ulOrderNo;
		m_ulLotteryProductNo = right.m_ulLotteryProductNo;
		m_ulProductNo = right.m_ulProductNo;
		m_wstrProductName = right.m_wstrProductName;
		m_wstrProductID = right.m_wstrProductID;
		//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���		
		m_sProductExpire = right.m_sProductExpire;
		//}}
		m_usProductPieces = right.m_usProductPieces;
		m_wstrProductAttribute0 = right.m_wstrProductAttribute0;
		m_wstrProductAttribute1 = right.m_wstrProductAttribute1;
		m_wstrProductAttribute2 = right.m_wstrProductAttribute2;
		m_wstrProductAttribute3 = right.m_wstrProductAttribute3;
		m_wstrProductAttribute4 = right.m_wstrProductAttribute4;

		return *this;
	}
};

DECL_DATA( KNXBTCouponProductInfo )
{
    unsigned long       m_ulProductNo;
    std::wstring        m_wstrExtendValue;

	CON_COPYCON_ASSIGNOP( KNXBTCouponProductInfo, right )
	{
		m_ulProductNo = right.m_ulProductNo;
		m_wstrExtendValue = right.m_wstrExtendValue;
		return *this;
	}
};

DECL_DATA( KNXBTSubProductInfo )
{
    unsigned long       m_ulProductNo;
    std::wstring        m_wstrProductName;
    std::wstring        m_wstrProductID;
	//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���	
    short				m_sProductExpire;
	//}}
    unsigned short      m_usProductPieces;
    std::wstring        m_wstrProductAttribute0;
    std::wstring        m_wstrProductAttribute1;
    std::wstring        m_wstrProductAttribute2;
    std::wstring        m_wstrProductAttribute3;
    std::wstring        m_wstrProductAttribute4;

	CON_COPYCON_ASSIGNOP( KNXBTSubProductInfo, right )
	{
		m_ulProductNo = right.m_ulProductNo;
		m_wstrProductName = right.m_wstrProductName;
		m_wstrProductID = right.m_wstrProductID;
		//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���		
		m_sProductExpire = right.m_sProductExpire;
		//}}
		m_usProductPieces = right.m_usProductPieces;
		m_wstrProductAttribute0 = right.m_wstrProductAttribute0;
		m_wstrProductAttribute1 = right.m_wstrProductAttribute1;
		m_wstrProductAttribute2 = right.m_wstrProductAttribute2;
		m_wstrProductAttribute3 = right.m_wstrProductAttribute3;
		m_wstrProductAttribute4 = right.m_wstrProductAttribute4;

		return *this;
	}

	//{{ 2009. 12. 2  ������	ĳ���۱ⰣŸ�Ժ���
	short GetProductExpire() const { return ( m_sProductExpire == -1 ? 0 : m_sProductExpire ); }
	//}}
};

#ifndef SERV_GLOBAL_BILLING
/*
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
#ifdef SERV_GLOBAL_BILLING
	bool                m_bShow;
	bool                m_bNotForSale;
	bool                m_bIsCashInvenSkip;
#endif // SERV_GLOBAL_BILLING
    std::wstring        m_wstrProductName;
    char                m_cCategoryNo;
    int                 m_iSalePrice;
    int                 m_iRealPrice;
    char                m_cPeriod;
    char                m_cQuantity;
    bool                m_bEnableGift;
    char                m_cLimitLevel;
    std::wstring        m_wstrEndDate;
};

// #ifdef SERV_GLOBAL_BILLING
DECL_DATA( KBillProductAttribute )
{
	int                 m_iProductNo;
	std::vector< int >  m_vecAttribute;
};
// #endif SERV_GLOBAL_BILLING

DECL_DATA( KBillPackageInfo )
{
    int                 m_iPackageNo;
    int                 m_iProductNo;
};

// #ifdef SERV_GLOBAL_BILLING
DECL_DATA( KBillBuyInfo )
{
	std::wstring		m_wstrOrderID;
	int                 m_iProductNo;
	int                 m_iPoint;
	int                 m_iProductID;
	unsigned short		m_usOrderQuantity;
	int                 m_iCategory;
	std::wstring        m_wstrMemo;

	KBillBuyInfo()
	{
		m_iProductNo = 0;
		m_iPoint = 0;
		m_iProductID = 0;
		m_usOrderQuantity = 0;
		m_iCategory = 0;
	}
};
// #endif SERV_GLOBAL_BILLING

DECL_DATA( KBillOrderInfo )
{
    int                 m_iOrderNo;
    UidType             m_iUserUID;
    int                 m_iProductNo;
};
//*/
#endif // SERV_GLOBAL_BILLING

DECL_DATA( KSquarePersonalShopInfo )
{
	enum UPDATE_TYPE
	{
		UT_ALL = 0,
		UT_STATE_NAME,

		UT_DATA_CLEAR,
	};

	char					m_cUpdateType;

	char					m_cPersonalShopState;
	char					m_cPersonalShopType;
	UidType					m_iPersonalShopUID;
	std::wstring			m_wstrPersonalShopName;

	KSquarePersonalShopInfo()
	{
		m_cPersonalShopState = 0;
		m_iPersonalShopUID = 0;
	}
};

DECL_DATA( KRegPersonalShopItemInfo )
{
	UidType         m_iItemUID;
	int				m_iQuantity;
	int				m_iPricePerOne;	//���簡��
};

DECL_DATA( KPersonalShopItemInfo )
{
	UidType         m_iItemUID;
	int				m_iQuantity;
};

DECL_DATA( KSellPersonalShopItemInfo )
{
	//{{ 2011. 05. 09	������	�븮����
#ifdef SERV_PSHOP_AGENCY
	enum SELL_PSHOP_ITEM_TYPE
	{
        SPIT_PERSONAL_SHOP = 0,
		SPIT_PSHOP_AGENCY,
	};
#endif SERV_PSHOP_AGENCY
	//}}

	KInventoryItemInfo	m_kInventoryItemInfo;	//����������
	int					m_iPricePerOne;			//���簡��
	int					m_iFeePerOne;			//���������
	//{{ 2011. 04. 18	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	char				m_cPShopItemType;
	int					m_iTotalSellEDIn;
	int					m_iTotalSoldItemQuantity;
#endif SERV_PSHOP_AGENCY
	//}}
	//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	UidType				m_iInventoryItemUID;
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}

	int TotalPricePerOne() const
	{
		return (m_iPricePerOne + m_iFeePerOne);
	}

	KSellPersonalShopItemInfo()
	{
		m_iPricePerOne = 0;
		m_iFeePerOne = 0;
		//{{ 2011. 04. 18	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
		m_cPShopItemType = SPIT_PERSONAL_SHOP;
		m_iTotalSellEDIn = 0;
		m_iTotalSoldItemQuantity = 0;
#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2012. 05. 31	��μ�       �븮 ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		m_iInventoryItemUID = 0;
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}
	}
};

//{{ 2011. 04. 22	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
DECL_DATA( KSellPShopItemDBUpdateInfo )
{
	UidType						m_iItemUID;
	int							m_iQuantity;
	int							m_iTotalSellEDIn;
	int							m_iTotalSoldItemQuantity;
	int							m_iTotalSellCommissionED;

	KSellPShopItemDBUpdateInfo()
	{
		m_iItemUID = 0;
		m_iQuantity = 0;
		m_iTotalSellEDIn = 0;
		m_iTotalSoldItemQuantity = 0;
		m_iTotalSellCommissionED = 0;
	}
};

DECL_DATA( KSellPShopItemBackupData )
{
	KSellPersonalShopItemInfo	m_kSellPShopItemInfo;
	int							m_iTotalSoldItemQuantity;
	int							m_iTotalSellCommissionED;

	KSellPShopItemBackupData()
	{
		m_iTotalSoldItemQuantity = 0;
		m_iTotalSellCommissionED = 0;
	}
};
#endif SERV_PSHOP_AGENCY
//}}

DECL_DATA( KLogData )
{
    BYTE                        m_byteLogLevel;
    std::wstring                m_wstrRegDate;
    std::wstring                m_wstrFunctionName;
    std::vector< std::wstring > m_vecContent;
    std::wstring                m_wstrFileName;
    int                         m_iFileLine;
};

//{{ 2008. 3. 30  ������  ��ü��
DECL_DATA( KPostItemInfo )
{
	enum LETTER_TYPE
	{
		LT_NONE = 0,

		LT_POST_OFFICE,
		LT_RECOMMEND,
		LT_TUTOL,
		LT_EVENT,
		LT_MESSAGE,
		LT_RANKING_TITLE,
		//{{ 2008. 12. 23  ������	���� ���� ����
		LT_BROKEN_ITEM,
		//}}
		//{{ 2009. 11. 2  ������	���
		LT_GUILD_DISBAND,
		LT_GUILD_KICK,
		//}}
		//{{ 2010. 02. 01  ������	��� �Խ���
		LT_GUILD_JOIN,
		//}}
		//{{ 2010. 02. 24  ������	�� ����Ʈ �̺�Ʈ
		LT_WEB_POINT_EVENT,
		//}}
		//{{ 2011. 05. 11	������	�븮 ����
//#ifdef SERV_PSHOP_AGENCY
		LT_STOP_SALE_PSHOP,
		LT_SELL_ITEM_PSHOP,
//#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
//#ifdef SERV_COME_BACK_USER_REWARD
		LT_COME_BACK_USER_NOTIFY,
//#endif SERV_COME_BACK_USER_REWARD
		//}} 
		//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
//#ifdef SERV_NEXON_SESSION_PROTOCOL
		LT_OUT_USER_RETANING_REWARD,		// ��Ż ���� ����
//#endif SERV_NEXON_SESSION_PROTOCOL
		//}}
//#define SERV_DUNGEON_REWARD_ITEM_MAIL
		LT_DUNGEON_REWARD_ITEM,                           // �߱�, �븸 ���� ����â ���� ��� ����
//#endif SERV_DUNGEON_REWARD_ITEM_MAIL
		//}}
		//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
//#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		LT_QUEST_REWARD,                           // ����Ʈ ����
//#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
		// 2013.02.05 lygan_������ // ������ �߻��Ͽ� ������ �Ҷ� DB���� �������� �������� �����ϴ� ���� Ÿ�� ( �ڵ忡���� �ش� Ÿ�� ������� �ʰ�, ����ؼ��� �ȵ� )
//#ifdef SERV_INDEMNIFICATION_POST_DB_ONLY
		LT_INDEMNIFICATION_DB_ONLY                                   = 18,                                  
//#endif SERV_INDEMNIFICATION_POST_DB_ONLY
		//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
//#ifdef SERV_EVENT_JACKPOT
		LT_JACKPOT_EVENT_REWARD,	// ���� �̺�Ʈ ����
//#endif SERV_EVENT_JACKPOT
		//}}
		//{{ 2013. 01. 21  ��Ը� �̺�Ʈ ���� �� ���� ���� ĳ���Ϳ��� �����ϴ� �ý��� - ��μ�
//#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		LT_DB_REWARD,				// DB���� �� �̺�Ʈ ����
		LT_SYSTEM_ERROR_REWARD,		// �ý��� ��� ���� ����
//#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		//}
		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
//#ifdef SERV_RELATIONSHIP_SYSTEM
		LT_WEDDING_REWARD,				// ��ȥ�� ����
		LT_WEDDING_INVITATION,			// ��ȥ�� ûø��
		LT_WEDDING_RESERVE,				// ��ȥ�� ����
		LT_BREAK_UP,					// �����
//#endif SERV_RELATIONSHIP_SYSTEM
		//}
	};

	UidType			m_iPostNo;
	UidType			m_iFromUnitUID;
	UidType			m_iToUnitUID;

	char			m_cScriptType;		// ������ ����
	int				m_iScriptIndex;		// ������ ��� rewardID, ������ ��� ÷�� ItemID
	int				m_iQuantity;		// ÷�ι��� �������� ��� ����, ED�� ��� ED��
	char			m_cEnchantLevel;

	KItemAttributeEnchantInfo m_kAttribEnchantInfo;
	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int >	m_vecItemSocket;
#else
	std::vector< short > m_vecItemSocket;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	//{{ 2013. 05. 09	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	std::vector< int >	m_vecRandomSocket;
	char				m_cItemState;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
	unsigned char	m_ucSealData;

	std::wstring	m_wstrRegDate;
	std::wstring	m_wstrFromNickName;
	std::wstring	m_wstrTitle;
	std::wstring	m_wstrMessage;	
	bool			m_bRead;	

	//{{ 2012. 08. 21	�ڼ���	���� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
	UidType					m_iItemUID;
	byte					m_iUsageType;
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
	//}}
	//{{ 2011.09.16 ��ȿ��  �� ���� ��Ʈ���� Ŭ���̾�Ʈ�ʿ��� �����ͼ� ���
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
	bool			m_bSenderDeleted;
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
	//}}	
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-20	// �ڼ���
	byte			m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS

	KPostItemInfo()
		: m_iPostNo( 0 )
		, m_iFromUnitUID( 0 )
		, m_iToUnitUID( 0 )
		, m_cScriptType( 0 )
		, m_iScriptIndex( 0 )
		, m_iQuantity( 0 )
		, m_cEnchantLevel( 0 )
		, m_ucSealData( 0 )
		, m_bRead( false )
		//{{ 2013. 05. 09	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		, m_cItemState( 0 )
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}
		//{{ 2012. 08. 21	�ڼ���	���� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
		, m_iItemUID( 0 )
		, m_iUsageType( 0 )
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
		//}}
		//{{ 2011.09.16 ��ȿ��  �� ���� ��Ʈ���� Ŭ���̾�Ʈ�ʿ��� �����ͼ� ���
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
		, m_bSenderDeleted( false )
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
		//}}
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-20	// �ڼ���
		, m_byteExpandedSocketNum( 0 )
#endif // SERV_BATTLE_FIELD_BOSS
	{
	}

	COPYCON_ASSIGNOP( KPostItemInfo, right )
	{
		m_iPostNo			 = right.m_iPostNo;
		m_iFromUnitUID		 = right.m_iFromUnitUID;
		m_iToUnitUID		 = right.m_iToUnitUID;
		m_cScriptType		 = right.m_cScriptType;
		m_iScriptIndex		 = right.m_iScriptIndex;
		m_iQuantity			 = right.m_iQuantity;
		m_cEnchantLevel		 = right.m_cEnchantLevel;
		m_kAttribEnchantInfo = right.m_kAttribEnchantInfo;
		m_vecItemSocket		 = right.m_vecItemSocket;		
		//{{ 2013. 05. 09	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		m_vecRandomSocket	 = right.m_vecRandomSocket;
		m_cItemState		 = right.m_cItemState;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}
		m_ucSealData		 = right.m_ucSealData;
		m_wstrRegDate		 = right.m_wstrRegDate;
		m_wstrFromNickName	 = right.m_wstrFromNickName;
		m_wstrTitle			 = right.m_wstrTitle;
		m_wstrMessage		 = right.m_wstrMessage;
		m_bRead				 = right.m_bRead;
		//{{ 2012. 08. 21	�ڼ���	���� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
		m_iItemUID			 = right.m_iItemUID;
		m_iUsageType		 = right.m_iUsageType;
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
		//}}
		//{{ 2011.09.16 ��ȿ��  �� ���� ��Ʈ���� Ŭ���̾�Ʈ�ʿ��� �����ͼ� ���
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
		m_bSenderDeleted	= right.m_bSenderDeleted;
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
		//}}
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-20	// �ڼ���
		m_byteExpandedSocketNum	= right.m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS

		return *this;
	}

	//{{ 2009. 11. 2  ������	�����ڵ�����
	bool IsSystemLetter() const { return ( m_cScriptType != KPostItemInfo::LT_POST_OFFICE ); }
	//}}
	//{{ 2012. 04. 17	������	����Ʈ ���� ���� ����
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR	
	static bool IsItemIDLetter( const char cScriptType )
	{
		// ScriptIndext���� ItemID���� ����
		switch( cScriptType )
		{
		case KPostItemInfo::LT_POST_OFFICE:
		case KPostItemInfo::LT_MESSAGE:
		case KPostItemInfo::LT_BROKEN_ITEM:
		case KPostItemInfo::LT_WEB_POINT_EVENT:
		case KPostItemInfo::LT_QUEST_REWARD:
			//{{ 2013. 01. 21  ��Ը� �̺�Ʈ ���� �� ���� ���� ĳ���Ϳ��� �����ϴ� �ý��� - ��μ�
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		case KPostItemInfo::LT_DB_REWARD:
		case KPostItemInfo::LT_SYSTEM_ERROR_REWARD:
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
			//}
			//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
		case KPostItemInfo::LT_WEDDING_REWARD:
		case KPostItemInfo::LT_WEDDING_INVITATION:
		case KPostItemInfo::LT_WEDDING_RESERVE:
#endif SERV_RELATIONSHIP_SYSTEM
			//}
			return true;
		}

		return false;
	}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}

	//{{ 2009. 8. 26  ������	�к�
	bool IsSealedItem() const { return ( m_ucSealData >= 100 ); }
	unsigned char GetSealCount() const
	{
		if( m_ucSealData >= 100 )
		{
			return ( m_ucSealData - 100 );
		}
		else
		{
			return m_ucSealData;
		}
	}
	//}}

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	char GetLetterType() const { return m_cScriptType; }
#endif SERV_RELATIONSHIP_SYSTEM
	//}
};

DECL_DATA( KPostItemTitleInfo )
{
	UidType			m_iPostNo;	
	char			m_cScriptType;		// ������ ����
	int				m_iScriptIndex;		// ������ ��� rewardID, ������ ��� ÷�� ItemID
	char			m_cEnchantLevel;
	std::wstring	m_wstrRegDate;
	std::wstring	m_wstrFromNickName;
	std::wstring	m_wstrTitle;
	bool			m_bRead;

	//{{ 2011.09.16 ��ȿ��  �� ���� ��Ʈ���� Ŭ���̾�Ʈ�ʿ��� �����ͼ� ���
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
	bool			m_bSenderDeleted;
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
	//}}

	KPostItemTitleInfo()
	{
		m_iPostNo = 0;
		m_cScriptType = 0;
		m_iScriptIndex = 0;
		m_cEnchantLevel = 0;
		m_bRead = false;

		//{{ 2011.09.16 ��ȿ��  �� ���� ��Ʈ���� Ŭ���̾�Ʈ�ʿ��� �����ͼ� ���
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
		m_bSenderDeleted = false;
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
		//}}
	}

	COPYCON_ASSIGNOP( KPostItemTitleInfo, right )
	{
		m_iPostNo = right.m_iPostNo;
		m_cScriptType = right.m_cScriptType;
		m_iScriptIndex = right.m_iScriptIndex;		
		m_cEnchantLevel = right.m_cEnchantLevel;
		m_wstrRegDate = right.m_wstrRegDate;
		m_wstrFromNickName = right.m_wstrFromNickName;
		m_wstrTitle = right.m_wstrTitle;
		m_bRead = right.m_bRead;

		//{{ 2011.09.16 ��ȿ��  �� ���� ��Ʈ���� Ŭ���̾�Ʈ�ʿ��� �����ͼ� ���
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
		m_bSenderDeleted = right.m_bSenderDeleted;
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
		//}}

		return *this;
	}
};
//}}

DECL_DATA( KPostSmallTitleInfo )
{

	char		 m_cScriptType;
	std::wstring m_wstrLetterTitle;
};

DECL_DATA( KTutorialUnitInfo )
{
	UidType			m_iUnitUID;
	u_char			m_ucLevel;
	std::wstring	m_wstrNickName;
	bool			m_bIsOnline;

	KTutorialUnitInfo()
	{
		m_iUnitUID = 0;
		m_ucLevel = 0;
		m_bIsOnline = false;
	}

	COPYCON_ASSIGNOP( KTutorialUnitInfo, right )
	{
		m_iUnitUID = right.m_iUnitUID;
		m_ucLevel = right.m_ucLevel;
		m_wstrNickName = right.m_wstrNickName;
		m_bIsOnline = right.m_bIsOnline;

		return *this;
	}
};

DECL_DATA( KTutorialDBUnitInfo )
{
	KTutorialUnitInfo		m_kTutorialUnitInfo;
	std::wstring			m_wstrLastDate;

	CON_COPYCON_ASSIGNOP( KTutorialDBUnitInfo, right )
	{
		m_kTutorialUnitInfo = right.m_kTutorialUnitInfo;
		m_wstrLastDate = right.m_wstrLastDate;

		return *this;
	}
};

//{{ 2008. 5. 21  ������  ������ ����� �α�
DECL_DATA( KItemAbuserLogInfo )
{
	int				m_iRewardState;
	int				m_iItemID;
	int				m_iQuantity;
};
//}}

//{{ 2008. 9. 23  ������	ĳ���� ����
DECL_DATA( KCashItemResolveInfo )
{
	int				m_iItemID;
    int				m_iSalePrice;
};
//}}

//{{ 2008. 10. 1  ������	�Ӽ�����
DECL_DATA( KAttribEnchantNpcInfo )
{
	char			m_cAttribEnchant1;
	char			m_cAttribEnchant2;
	char			m_cExtraSpecialAbility1;
	char			m_cExtraSpecialAbility2;

	KAttribEnchantNpcInfo()
	{
		m_cAttribEnchant1 = 0;
		m_cAttribEnchant2 = 0;
		m_cExtraSpecialAbility1 = 0;
		m_cExtraSpecialAbility2 = 0;
	}
//{{ robobeg : 2012-11-15
	bool			IsValidInfo() const
	{
		return m_cAttribEnchant1 != 0
			|| m_cAttribEnchant2 != 0
			|| m_cExtraSpecialAbility1 != 0
			|| m_cExtraSpecialAbility2 != 0;
	}
//}} robobeg : 2012-11-15
};
//}}

//{{ 2008. 10. 6  ������	Ÿ��Ʋ
DECL_DATA( KSubMissionInstance )
{
	int				m_iID;

	short			m_sClearData;
	bool			m_bIsSuccess;

	KSubMissionInstance()
	{
		m_iID			= 0;
		m_sClearData	= 0;
		m_bIsSuccess	= false;
	}

	COPYCON_ASSIGNOP( KSubMissionInstance, right )
	{
		m_iID				= right.m_iID;
		m_sClearData		= right.m_sClearData;
		m_bIsSuccess		= right.m_bIsSuccess;

		return *this;
	}
};


DECL_DATA( KMissionInstance )
{
	int				m_iID;

	std::vector< KSubMissionInstance > m_vecSubMissionInstance;

	KMissionInstance()
	{
		m_iID			= 0;
	}

	COPYCON_ASSIGNOP( KMissionInstance, right )
	{
		m_iID					= right.m_iID;
		m_vecSubMissionInstance	= right.m_vecSubMissionInstance;

		return *this;
	}
};

DECL_DATA( KTitleInfo )
{
    int				m_iTitleID;
	std::wstring	m_wstrEndDate;
	__int64			m_kCTime;
	bool			m_bInfinity;

	KTitleInfo()
	{
		m_iTitleID		 = 0;
		m_kCTime		 = 0;
		m_bInfinity		 = false;
	}

	COPYCON_ASSIGNOP( KTitleInfo, right )
	{
		m_iTitleID = right.m_iTitleID;
		m_wstrEndDate = right.m_wstrEndDate;
		m_kCTime = right.m_kCTime;
		m_bInfinity = right.m_bInfinity;

		return *this;
	}
};

DECL_DATA( KTitleReward )
{
	int				m_iTitleID;
	short			m_sPeriod;

	KTitleReward()
	{
		m_iTitleID		 = 0;
		m_sPeriod		 = 0;
	}

	COPYCON_ASSIGNOP( KTitleReward, right )
	{
		m_iTitleID = right.m_iTitleID;
		m_sPeriod = right.m_sPeriod;

		return *this;
	}
};

DECL_DATA( KRankingTitleReward )
{
	UidType						m_iUnitUID;
	std::vector< KTitleReward > m_vecReward;
};
//}}

//{{ 2008. 10. 30  ������	����� ����Ʈ
DECL_DATA( KAbuserList )
{
	int							m_iReleaseTick;
	std::vector< UidType >		m_vecAbuserList;
};
//}}

DECL_DATA( KFieldUserInfo )
{
	UidType                             	m_iUnitUID;
	std::wstring                        	m_wstrNickName;
	char                                	m_cUnitClass; 
	UCHAR                               	m_ucLevel;
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	char									m_cRank;
#else
	int										m_iRating;
#endif SERV_2012_PVP_SEASON2
	//}}
#else
	char									m_cPVPEmblem;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	std::vector< KInventoryItemSimpleInfo > m_vecEquippedItem;
	//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
	int										m_iTitleID;
#else
	short									m_sTitleID;
#endif SERV_TITLE_DATA_SIZE
	//}}
	VECTOR3									m_vPos;	
	UidType									m_iPartyUID;

	//{{ 2009. 9. 16  ������	���
#ifdef GUILD_TEST
	std::wstring							m_wstrGuildName;
#endif GUILD_TEST
	//}}

	//{{ 2010. 07. 21  ������	�� �ý���
#ifdef SERV_PET_SYSTEM
	std::vector< KFieldPetInfo >			m_vecPet;	// ���� ��ȯ�� �� 1������ ����� �ֽ��ϴ�.
#endif SERV_PET_SYSTEM
	//}}

	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
	int										m_iED;
#endif SERV_ED_MONITORING_IN_GAME
	//}}

#ifdef SERV_INVISIBLE_GM
	bool									m_bInvisible;
#endif SERV_INVISIBLE_GM

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
	UidType									m_iRidingPetUID;
	USHORT									m_usRidingPetID;
#endif	// SERV_RIDING_PET_SYSTM

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	UidType									m_iLoverUnitUID;
#endif SERV_RELATIONSHIP_SYSTEM
	//}

#ifdef SERV_GROW_UP_SOCKET
	int										m_iEventQuestClearCount;
	int										m_iExchangeCount;
#endif SERV_GROW_UP_SOCKET
#ifdef SERV_4TH_ANNIVERSARY_EVENT
	unsigned int							m_iAccountPVPLoseCount;
#endif SERV_4TH_ANNIVERSARY_EVENT

	CON_COPYCON_ASSIGNOP( KFieldUserInfo, right )
	{
		m_iUnitUID			= right.m_iUnitUID;
		m_wstrNickName		= right.m_wstrNickName;
		m_cUnitClass		= right.m_cUnitClass;
		m_ucLevel			= right.m_ucLevel;
		//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		m_cRank				= right.m_cRank;
#else
		m_iRating			= right.m_iRating;
#endif SERV_2012_PVP_SEASON2
		//}}
#else
		m_cPVPEmblem		= right.m_cPVPEmblem;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		m_vecEquippedItem	= right.m_vecEquippedItem;
		//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
		m_iTitleID			= right.m_iTitleID;
#else
		m_sTitleID			= right.m_sTitleID;
#endif SERV_TITLE_DATA_SIZE
		//}}
		m_vPos				= right.m_vPos;
		//{{ 2009. 2. 24  ������	��Ƽ��ǳ��
		m_iPartyUID			= right.m_iPartyUID;
		//}}
		//{{ 2009. 9. 16  ������	���
#ifdef GUILD_TEST
		m_wstrGuildName		= right.m_wstrGuildName;
#endif GUILD_TEST
		//}}
		//{{ 2010. 07. 21  ������	�� �ý���
#ifdef SERV_PET_SYSTEM
		m_vecPet			= right.m_vecPet;
#endif SERV_PET_SYSTEM
		//}}		
		//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
		m_iED				= right.m_iED;
#endif SERV_ED_MONITORING_IN_GAME
		//}}
#ifdef SERV_INVISIBLE_GM
		m_bInvisible	= right.m_bInvisible;
#endif SERV_INVISIBLE_GM

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
		m_iRidingPetUID		= right.m_iRidingPetUID;
		m_usRidingPetID		= right.m_usRidingPetID;
#endif	// SERV_RIDING_PET_SYSTM
		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
		m_iLoverUnitUID		= right.m_iLoverUnitUID;
#endif SERV_RELATIONSHIP_SYSTEM
		//}

#ifdef SERV_GROW_UP_SOCKET
		m_iEventQuestClearCount		= right.m_iEventQuestClearCount;
		m_iExchangeCount			= right.m_iExchangeCount;
#endif SERV_GROW_UP_SOCKET
#ifdef SERV_4TH_ANNIVERSARY_EVENT
		m_iAccountPVPLoseCount		= right.m_iAccountPVPLoseCount;
#endif SERV_4TH_ANNIVERSARY_EVENT
		return *this;
	}
};

//{{ 2009. 1. 13  ������	�ʵ� ���� ����Ʈ
DECL_DATA( KCommunityUserInfo )
{
	enum USER_STATE
	{
		US_NONE = 0,
        US_FIELD,			// statecode : PartyUID
		US_DUNGEON_LOUNGE,	// statecode : PartyUID
		US_PVP_LOBBY,		// statecode : RoomListID
		US_PVP_WAIT,		// statecode : RoomListID
		US_PVP_PLAY,		// statecode : RoomListID
		US_ENTER_LOBBY,		// �κ� ����
		US_LEAVE_LOBBY,		// �κ� ��Ż
	};

	UidType                             	m_iUnitUID;
	std::wstring                        	m_wstrNickName;
	char                                	m_cUnitClass;
	UCHAR                               	m_ucLevel;
	char									m_cState;	
	UidType									m_iStateCode;
	UidType									m_iRoomUID;
	//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
	int										m_iED;
#endif SERV_ED_MONITORING_IN_GAME
	//}}


	KCommunityUserInfo()
	{
		m_iUnitUID		= 0;
		m_cUnitClass	= 0;
		m_ucLevel		= 0;
		m_cState		= US_NONE;
		m_iStateCode	= 0;
		m_iRoomUID		= 0;
	}

	COPYCON_ASSIGNOP( KCommunityUserInfo, right )
	{
		m_iUnitUID			= right.m_iUnitUID;
		m_wstrNickName		= right.m_wstrNickName;
		m_cUnitClass		= right.m_cUnitClass;
		m_ucLevel			= right.m_ucLevel;
		m_cState			= right.m_cState;
		m_iStateCode		= right.m_iStateCode;
		m_iRoomUID			= right.m_iRoomUID;
		//{{ 2011. 09. 20  ��μ�	���ӳ� ED ���� ����͸�
#ifdef SERV_ED_MONITORING_IN_GAME
		m_iED				= right.m_iED;
#endif SERV_ED_MONITORING_IN_GAME
		//}}
		return *this;
	}

	// Ŭ���̾�Ʈ���� ������ �������̽�	
	UidType GetPartyUID()	{ return m_iStateCode; }
	int GetRoomListID()		{ return static_cast<int>(m_iStateCode); }
};
//}}

//�Ʒ� struct �� size ������ ���氡�ɼ��� ���� ���������� ������ ����.
DECL_DATA( KTileInfo )
{
	//Ÿ�ϰ���(3�����迭�� �ε���)
	int							m_NumX;
	int							m_NumY;
	int							m_NumZ;

	//Ÿ��ũ��
	short						m_SizeX;
	short						m_SizeY;
	short						m_SizeZ;
};

DECL_DATA( KMapInfo )
{
	short						m_MinX;
	short						m_MinY;
	short						m_MinZ;

	short						m_MaxX;
	short						m_MaxY;
	short						m_MaxZ;
};

DECL_DATA( KFieldCoordinates )
{
	short						m_PosX;
	short						m_PosY;
	short						m_PosZ;
};

DECL_DATA( KFieldFrameInfo )
{
	u_char						m_StateID;
	u_char						m_FrameCount;
};

DECL_DATA( KFieldOption )
{
	enum LIMIT_OPTION
	{
		LO_LOW			= 10,
		LO_MIDDLE		= 25,			// �ؿ��� �ɼ� ���� ( �� ȭ�� �ִ� ĳ���� �� ) 30
		LO_HIGH			= 35,			// �ؿ��� �ɼ� ���� ( �� ȭ�� �ִ� ĳ���� �� ) 40
	};

	short						m_sLimitOption;	

	KFieldOption()
	{
		m_sLimitOption	= LO_HIGH;
	}

	static bool VerifyLimitOption( short sOption )
	{
		switch( sOption )
		{
		case LO_LOW:
		case LO_MIDDLE:
		case LO_HIGH:
			return true;
		}

		return false;
	}
};

//{{ 2009. 1. 16  ������	ä�θ���Ʈ
DECL_DATA( KChannelBonusInfo )
{
	bool						m_bEnable;
	int							m_iBeginLv;
	int							m_iEndLv;
	int							m_iPerExp;
	int							m_iPerED;

	KChannelBonusInfo()
	{
		m_bEnable	= false;
		m_iBeginLv	= 0;
		m_iEndLv	= 0;
		m_iPerExp	= 0;
		m_iPerED	= 0;
	}
};

DECL_DATA( KChannelInfo )
{
	int							m_iServerUID;
	int							m_iChannelID;
	std::wstring				m_wstrChannelName;
	int							m_iServerGroupID;
	std::wstring				m_wstrIP;
	u_short						m_usMasterPort;
	u_short						m_usNCUDPPort;
	int							m_iMaxUser;
	int							m_iCurrentUser;
	//{{ 2009. 4. 1  ������		��Ƽ����
	int							m_iCurPartyCount;
	int							m_iPlayGamePartyCount;
	//}}
	
	//{{ 2012. 05. 16	�ڼ���	ä�� ����Ʈ ���� ����
#ifdef SERV_INTEGRATE_THE_CHANNEL_INFO
	KChannelBonusInfo			m_kBonusInfo;
#endif SERV_INTEGRATE_THE_CHANNEL_INFO
	//}}
};

//{{ 2009. 6. 15  ������	ä�θ���Ʈ
DECL_DATA( KChannelSimpleInfo )
{
	int							m_iChannelID;
	int							m_iMaxUser;
	int							m_iCurrentUser;
};
//}}
//}}

//{{ 2009. 4. 23  ������	������������Ʈ
DECL_DATA( KPVPUserSimpleInfo )
{
	UidType                             m_iUnitUID;
	char								m_cState;
	int									m_iStateCode;
	UidType								m_iRoomUID;
};
//}}

// ȹ���� ��ų ����
DECL_DATA( KUserSkillData )
{
	short	m_iSkillID;
	UCHAR	m_cSkillLevel;
	UCHAR	m_cSkillCSPoint;

	KUserSkillData()
	{
		m_iSkillID = 0;
		m_cSkillLevel = 0;
		m_cSkillCSPoint = 0;
	}

	KUserSkillData( short iSkillID, UCHAR cSkillLevel, UCHAR cSkillCSPoint )
	{
		m_iSkillID		= iSkillID;
		m_cSkillLevel	= cSkillLevel;
		m_cSkillCSPoint	= cSkillCSPoint;
	}
};

#define EQUIPPED_SKILL_SLOT_COUNT 4

#ifdef SERV_SKILL_PAGE_SYSTEM
DECL_DATA( KRetrievedSkillPageData )
{
	int								m_iRetrievedSPoint;
	std::vector< KUserSkillData >	m_vecUserSkillData;		// DB�� �����ؾ��� ��ų ����

	KRetrievedSkillPageData()
		: m_iRetrievedSPoint(0)
	{
		m_vecUserSkillData.clear();
	}
};

DECL_DATA( KUserSkillPageData )
{
	KSkillData 					m_aEquippedSkill[ EQUIPPED_SKILL_SLOT_COUNT ];
	KSkillData 					m_aEquippedSkillSlotB[ EQUIPPED_SKILL_SLOT_COUNT ];
	
	USHORT						m_usSkillPoint;
	USHORT						m_usCashSkillPoint;

	std::vector<KUserSkillData> m_vecUserSkillData;
	

	KUserSkillPageData()
	{
		Init();
	}

	void Init()
	{
		for ( int i = 0; i < EQUIPPED_SKILL_SLOT_COUNT; i++ )
		{
			m_aEquippedSkill[i].Init();
			m_aEquippedSkillSlotB[i].Init();
		}

		m_usSkillPoint			= 0;
		m_usCashSkillPoint		= 0;
		m_vecUserSkillData.clear();
	}	
};
#endif // SERV_SKILL_PAGE_SYSTEM

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 5. 11  ������	�ǽð�������
DECL_DATA( KDungeonRewardED )
{
	int							m_iED;
	int							m_iBonusED;

	KDungeonRewardED()
	{
		m_iED = 0;
		m_iBonusED = 0;
	}

	int SumED() const
	{
		return ( m_iED + m_iBonusED );
	}
};

DECL_PACKET( EGS_GET_ITEM_NOT )
{
	enum GET_ITEM_TYPE
	{
		GIT_DROP_ITEM = 0,
        GIT_QUEST_ITEM,
		GIT_RESULT_ITEM,
		GIT_DROP_EVENT_ITEM,
	};

	UidType                     m_GetUnitUID;

	// ������ ȹ��
	int							m_iItemID;
	int							m_iDropItemUID;
	char						m_cGetItemType;

#ifdef DUNGEON_ITEM
	bool						m_bIsItemSuccess;	// ����,���� �� ������ ���漺�� ����.
#endif DUNGEON_ITEM

	// EDȹ��
	int							m_iTotalED;
	std::map< UidType, KDungeonRewardED > m_mapGetED;
	//{{ 2013. 1. 14	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
	char						m_cEnchantLevel;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
	//}}

	KEGS_GET_ITEM_NOT()
		: m_GetUnitUID( 0 )
		, m_iItemID( 0 )
		, m_iDropItemUID( 0 )
		, m_cGetItemType( GIT_DROP_ITEM )	// �⺻������ ��Ӿ�����
		, m_iTotalED( 0 )
#ifdef DUNGEON_ITEM
		, m_bIsItemSuccess( false )
#endif DUNGEON_ITEM
		//{{ 2013. 1. 14	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
		, m_cEnchantLevel( 0 )
#endif SERV_ENCHANT_ITEM_DROP_EVENT
		//}}
	{
	}

	//{{ 2009. 11. 18  ������	Ư���ð�����̺�Ʈ	
	bool IsDungeonDropItem()
	{
		return ( m_cGetItemType == GIT_DROP_ITEM  ||  m_cGetItemType == GIT_DROP_EVENT_ITEM );
	}
	//}}
};
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2009. 5. 14  ������	������ �Ҹ� ����
DECL_DATA( KDecreaseEnduranceInfo )
{
    UidType						m_iItemUID;
	int							m_iEndurance;
};
//}}

//{{ 2009. 5. 23  ������	�ð����
DECL_DATA( KTimeDropInfo )
{
    int							m_iTimeDropID;
	int							m_iDropItemID;
	std::wstring				m_wstrDropTime;
	std::wstring				m_wstrServerIP;
	bool						m_bNoticeMsg;
};
//}}

//{{ 2011. 04. 18	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
DECL_DATA( KUserPShopAgencyInfo )
{
    bool						m_bIsPShopOpen;
	std::wstring				m_wstrAgencyExpirationDate;
    char                        m_cShopType;
    std::vector< KSellPShopItemBackupData >		m_vecSellItemInfo;

    UidType					m_iUserUID;
    UidType					m_iUnitUID;
    std::wstring			m_wstrNickName;
    std::wstring			m_wstrAgencyOpenDate;
    std::wstring			m_wstrPersonalShopName;
    bool					m_bOnSale;

	KUserPShopAgencyInfo()
	{		
		m_bIsPShopOpen = false;
	}
};

DECL_DATA( KPersonalShopAgencyInfo )
{
	UidType										m_iHostUnitUID;
	UidType										m_iPShopAgencyUID;
	std::wstring								m_wstrAgencyOpenDate;
	std::wstring								m_wstrAgencyExpirationDate;
    bool										m_bOnSale;
	std::vector< KSellPersonalShopItemInfo >	m_vecSellPShopAgencyItem;

	KPersonalShopAgencyInfo()
	{
		m_iHostUnitUID = 0;
		m_iPShopAgencyUID = 0;
		m_bOnSale = false;
	}
};

#endif SERV_PSHOP_AGENCY
//}}

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_CONNECT_REQ )
{
	std::wstring			m_wstrVersion;
};

DECL_PACKET( EGS_CONNECT_ACK )
{
	int						m_iOK;
	int						m_UDPPort;
	//{{ 2009. 4. 17  ������	ä������
	int						m_iChannelID;
	//}}

	//{{ 2012. 09. 02	�ڼ���	Merge ( �������� Ŭ���̾�Ʈ ���۵����� UDP ��Ŷ�� ���ƿ��� �ʴ� ������ �߰ߵǸ� �������� ű�Ѵ�. // 2012.06.11 lygan_������ )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	bool					m_bUDPKickOff;
#endif UDP_CAN_NOT_SEND_USER_KICK
	//}}
};


/////////////////////////////////////////////////////////////////////////
typedef KUserAuthenticateReq KEGS_VERIFY_ACCOUNT_REQ;

DECL_PACKET( EGS_VERIFY_ACCOUNT_ACK )
{
	int                     m_iOK;
	KAccountInfo            m_kAccountInfo;
	char					m_cHackingUserType;
	//{{ 2008. 9. 2  ������		������ ����
	std::wstring			m_wstrAdURL;
	//}}
	//{{ 2010. 8. 3	������	���� ����
	bool                    m_bSex;
	UINT32                  m_uiAge;
	//}}
	//{{ 2010. 12. 1	������	NexonSN�߰�
	UINT32                  m_uiNexonSN;
	//}}
	//{{ 2010. 01. 11	��μ�	IP�߰�
#ifdef SERV_CLIENT_PUBLIC_IP
	std::wstring			m_wstrClientIP;
#endif SERV_CLIENT_PUBLIC_IP
	//}}
	//{{ 2011. 07. 27    ��μ�    ���Ϸ��� ä�θ�
//#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
	UINT8					m_uChannelCode;			// 0 �ؽ�, 3 ���Ϸ���
	std::wstring			m_wstrChannelUserID;	// ä�θ��� ������ ID
//#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
	//}}
#ifdef SERV_EVENT_MONEY	// ��μ� // ���볯¥: 2013-07-04
	int						m_iEventMoney;
#endif // SERV_EVENT_MONEY
	
	KEGS_VERIFY_ACCOUNT_ACK()
	{
//#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		m_uChannelCode			= 0;	//ä�θ� ���� ���� Default �� 0
		m_wstrChannelUserID		= L"";
//#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
	}
};





//////////////////////////////////////////////////////////////////////////
//{{ 2009. 5. 30  ������	�ӽ� �κ�
DECL_DATA( KTempInventoryItemInfo )
{
	int				m_iTempItemUID;
	int				m_iItemID;
	int				m_iQuantity;
	unsigned char	m_ucSealData;
	//{{ 2012. 1. 16	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
	char			m_cEnchantLevel;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
	//}}

	KTempInventoryItemInfo()
		: m_iTempItemUID( 0 )
		, m_iItemID( 0 )
		, m_iQuantity( 0 )
		, m_ucSealData( 0 )
		//{{ 2012. 1. 16	�ڼ���	���� �� �ʵ� ��� �����ۿ� ��ȭ �ο�
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
		, m_cEnchantLevel( 0 )
#endif SERV_ENCHANT_ITEM_DROP_EVENT
		//}}
	{
	}
};
//}}

DECL_PACKET( EGS_REQUEST_FRIEND_REQ )
{
    std::wstring                        m_wstrNickName;
    std::wstring                        m_wstrMessage;
};

DECL_PACKET( EGS_REQUEST_FRIEND_ACK )
{
    int                                 m_iOK;
    UidType                             m_iUnitUID;
    std::wstring                        m_wstrNickName;
};

DECL_PACKET( EGS_REQUEST_FRIEND_NOT )
{
    UidType                             m_iUnitUID;
    std::wstring                        m_wstrNickName;
    std::wstring                        m_wstrMessage;
};

typedef                 UidType         KEGS_ACCEPT_FRIEND_REQ;

DECL_PACKET( EGS_ACCEPT_FRIEND_ACK )
{
    int                                 m_iOK;
    UidType                             m_iUnitUID;
};

DECL_PACKET( EGS_ACCEPT_FRIEND_NOT )
{
    UidType                             m_iUnitUID;
};

typedef                 UidType         KEGS_DENY_FRIEND_REQ;

DECL_PACKET( EGS_DENY_FRIEND_ACK )
{
    int                                 m_iOK;
    UidType                             m_iUnitUID;
};

DECL_PACKET( EGS_DENY_FRIEND_NOT )
{
    UidType                             m_iUnitUID;
    std::wstring                        m_wstrNickName;
};

typedef                 UidType         KEGS_BLOCK_FRIEND_REQ;

DECL_PACKET( EGS_BLOCK_FRIEND_ACK )
{
    int                                 m_iOK;
    UidType                             m_iUnitUID;
};

DECL_PACKET( EGS_BLOCK_FRIEND_NOT )
{
    UidType                             m_iUnitUID;
};

typedef                 UidType         KEGS_UNBLOCK_FRIEND_REQ;

DECL_PACKET( EGS_UNBLOCK_FRIEND_ACK )
{
    int                                 m_iOK;
    UidType                             m_iUnitUID;
};

DECL_PACKET( EGS_UNBLOCK_FRIEND_NOT )
{
    UidType                             m_iUnitUID;
};

typedef                 UidType         KEGS_DELETE_FRIEND_REQ;

DECL_PACKET( EGS_DELETE_FRIEND_ACK )
{
    int                                 m_iOK;
    UidType                             m_iUnitUID;
};

DECL_PACKET( EGS_DELETE_FRIEND_NOT )
{
    UidType                             m_iUnitUID;
};

DECL_PACKET( EGS_MOVE_FRIEND_REQ )
{
    UidType                             m_iUnitUID;
    char                                m_cTargetGroupID;
};

DECL_PACKET( EGS_MOVE_FRIEND_ACK )
{
    int                                 m_iOK;
    UidType                             m_iUnitUID;
    char                                m_cTargetGroupID;
};

DECL_PACKET( EGS_MAKE_FRIEND_GROUP_REQ )
{
    std::wstring                        m_wstrGroupName;
};

DECL_PACKET( EGS_MAKE_FRIEND_GROUP_ACK )
{
    int                                 m_iOK;
    char                                m_cGroupID;
    std::wstring                        m_wstrGroupName;
};

DECL_PACKET( EGS_RENAME_FRIEND_GROUP_REQ )
{
    char                                m_cGroupID;
    std::wstring                        m_wstrGroupName;
};

DECL_PACKET( EGS_RENAME_FRIEND_GROUP_ACK )
{
    int                                 m_iOK;
    char                                m_cGroupID;
    std::wstring                        m_wstrGroupName;
};

DECL_PACKET( EGS_DELETE_FRIEND_GROUP_REQ )
{
    char                                m_cGroupID;
};

DECL_PACKET( EGS_DELETE_FRIEND_GROUP_ACK )
{
    int                                 m_iOK;
    char                                m_cGroupID;
};

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 9. 16  ������	���
#ifdef GUILD_TEST

DECL_DATA( KGuildInfo )
{
	enum GUILD_INFO
	{
		GUILD_MAX_MEMBER_LIMIT = 100,
	};

    int									m_iGuildUID;
	std::wstring						m_wstrGuildName;
	u_short								m_usMaxNumMember;	
	u_char								m_ucGuildLevel;
	int									m_iGuildEXP;
	std::wstring						m_wstrGuildMessage;
	std::wstring						m_wstrFoundingDay;

	KGuildInfo()
	{
		m_iGuildUID		 = 0;
		m_usMaxNumMember = 0;
		m_ucGuildLevel	 = 0;
		m_iGuildEXP		 = 0;
	}
};

DECL_DATA( KGuildMemberInfo )
{
	UidType								m_iGSUID;
	UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;
	char                            	m_cUnitClass;
	UCHAR                           	m_ucLevel;
	u_char								m_ucMemberShipGrade;
	int									m_iChannelID;
	int									m_iMapID;
	char								m_cPosition;
	__int64								m_tLogInTime;
	__int64								m_tLogOutTime;
	std::wstring						m_wstrMessage;

	KGuildMemberInfo()
	{
		m_iGSUID			= 0;
		m_iUnitUID			= 0;		
		m_cUnitClass		= 0;
		m_ucLevel			= 0;
		m_ucMemberShipGrade = 0;
		m_iChannelID		= 0;
		m_iMapID			= 0;
		m_cPosition			= -1;
		m_tLogInTime		= 0;
		m_tLogOutTime		= 0;
	}

	bool IsLogined() { return ( m_tLogInTime != 0 ); }
};

DECL_DATA( KGuildMemberMessageInfo )
{
	UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;
	u_char								m_ucMemberShipGrade;
	char                            	m_cUnitClass;
	UCHAR                           	m_ucLevel;
	std::wstring						m_wstrMessage;

	KGuildMemberMessageInfo()
	{
		m_iUnitUID			= 0;
		m_ucMemberShipGrade = 0;		
		m_cUnitClass		= 0;
		m_ucLevel			= 0;
	}
};

#endif GUILD_TEST
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 11. 23  ������	��彺ų
#ifdef GUILD_SKILL_TEST

DECL_DATA( KGuildSkillData )
{
	short	m_iSkillID;
	u_char	m_cSkillLevel;
	u_char	m_cSkillCSPoint;

	KGuildSkillData()
	{
		m_iSkillID = 0;
		m_cSkillLevel = 0;
		m_cSkillCSPoint = 0;
	}

	KGuildSkillData( short iSkillID, UCHAR cSkillLevel, UCHAR cSkillCSPoint )
	{
		m_iSkillID		= iSkillID;
		m_cSkillLevel	= cSkillLevel;
		m_cSkillCSPoint	= cSkillCSPoint;
	}
};

DECL_DATA( KGuildSkillInfo )
{
	std::vector< KGuildSkillData >		m_vecGuildSkill;
	int									m_iGuildSPoint;
	int									m_iGuildCSPoint;
	int									m_iMaxGuildCSPoint;
	std::wstring						m_wstrGuildCSPointEndDate;

	KGuildSkillInfo()
	{
		m_iGuildSPoint = 0;
		m_iGuildCSPoint = 0;
		m_iMaxGuildCSPoint = 0;
	}
};

#endif GUILD_SKILL_TEST
//}}
//////////////////////////////////////////////////////////////////////////

DECL_PACKET( EGS_GUILD_INFO_NOT )
{
	KGuildInfo							m_kGuildInfo;
	std::wstring						m_wstrGuildMasterName;
	std::vector< UidType >				m_vecMemberUnitUID;

	//{{ 2009. 11. 23  ������	��彺ų
#ifdef GUILD_SKILL_TEST
	KGuildSkillInfo						m_kGuildSkillInfo;
#endif GUILD_SKILL_TEST
	//}}
};

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 12. 4  ������	���ӽð��̺�Ʈ�ý��۰���

DECL_DATA( KUserConnectTimeEventInfo )
{
	int				m_iEventID;
	float			m_fEventTime;
	__int64			m_kCTime;
	std::wstring	m_wstrEventName;
	bool			m_bInit;
	bool			m_bGiveOnce;
	//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
	bool			m_bAccountEvent;
#endif SERV_ACC_TIME_EVENT
	//}}
	//{{ 2010. 7. 27  ������	���� �ð� �̺�Ʈ ���� ����
#ifdef SERV_TIME_EVENT_BUG_FIX
	double			m_fBackUpTime;
#endif SERV_TIME_EVENT_BUG_FIX
	//}}
	//{{ 2012. 06. 29	��μ�       ���� �ð��� ���� ������ �ݺ� ����
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
	bool			m_bRepeatEvent;
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
	//}}

	//{{ 2012. 08. 14	�ڼ���	��õ���� ��ȭ �̺�Ʈ ���̵� ���� ���
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
	int				m_iScriptID;
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
	//}}

#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
	bool					m_bRepeatRewardEvent;
	int						m_iBeforeEventID;
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
	bool					m_bNewUnit;
	bool					m_bNewUnit2;
	bool					m_bCurrentUnit;
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
	int						 m_iEventItemID;
#endif

#ifdef SERV_CUSTOM_CONNECT_EVENT
	int					m_iCustomEventID;
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_STEAM_USER_CONNECT_EVENT
	bool				m_bOnlySteamUser;
	bool				m_bOnlyNotSteamUser;
#endif //SERV_STEAM_USER_CONNECT_EVENT

	KUserConnectTimeEventInfo()
	{
		m_iEventID		= 0;
		m_fEventTime	= 0.0f;
		m_kCTime		= 0;		
		m_bInit			= false;
		m_bGiveOnce		= true;
		//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
		m_bAccountEvent	= false;
#endif SERV_ACC_TIME_EVENT
		//}}
		//{{ 2010. 7. 27  ������	���� �ð� �̺�Ʈ ���� ����
#ifdef SERV_TIME_EVENT_BUG_FIX
		m_fBackUpTime	= 0.0;
#endif SERV_TIME_EVENT_BUG_FIX
		//}}
		//{{ 2012. 06. 29	��μ�       ���� �ð��� ���� ������ �ݺ� ����
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
		m_bRepeatEvent = false;
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
		//}}

		//{{ 2012. 08. 14	�ڼ���	��õ���� ��ȭ �̺�Ʈ ���̵� ���� ���
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		m_iScriptID		= 0;
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		//}}
#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
		m_bRepeatRewardEvent = false;
		m_iBeforeEventID = -1;
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		m_bNewUnit		= false;
		m_bNewUnit2		= false;
		m_bCurrentUnit	= false;
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
		m_iEventItemID	= 0;
#endif

#ifdef SERV_CUSTOM_CONNECT_EVENT
		m_iCustomEventID = 0;
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_STEAM_USER_CONNECT_EVENT
		m_bOnlySteamUser = false;
		m_bOnlyNotSteamUser = false;
#endif //SERV_STEAM_USER_CONNECT_EVENT
	}
};

DECL_DATA( KUserCumulativeTimeEventInfo )
{
	int				m_iEventUID;
	int				m_iEventID;
	bool			m_bInit;
	std::wstring	m_wstrEventName;
	float			m_fEventTime;	
	bool			m_bDirectReward;
	//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
	bool			m_bAccountEvent;
#endif SERV_ACC_TIME_EVENT
	//}}
#ifdef SERV_CUSTOM_CONNECT_EVENT
	int					m_iCustomEventID;
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
	bool					m_bNewUnitEvent;
	bool					m_bNewUnitEvent2;
	bool					m_bCurrentUnitEvent;
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

	KUserCumulativeTimeEventInfo()
		: m_iEventUID( 0 )
		, m_iEventID( 0 )
		, m_bInit( false )
		, m_fEventTime( 0.0f )
		, m_bDirectReward( false )
		//{{ 2010. 06. 11  ������	�������� ���ӽð� �̺�Ʈ
#ifdef SERV_ACC_TIME_EVENT
		, m_bAccountEvent( false )
#endif SERV_ACC_TIME_EVENT
		//}}
#ifdef SERV_CUSTOM_CONNECT_EVENT
		, m_iCustomEventID( 0 )
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		, m_bNewUnitEvent( false )
		, m_bNewUnitEvent2( false )
		, m_bCurrentUnitEvent( false )
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
	{
	}
};

DECL_DATA( KTimeEventSimpleInfo )
{
	int									m_iEventUID;
	int									m_iEventID;
	std::wstring						m_wstrEventName;
	float								m_fEventTime;
#ifdef SERV_ADVERTISEMENT_EVENT
	std::wstring						m_wstrAdvertisementURL;
#endif SERV_ADVERTISEMENT_EVENT

	KTimeEventSimpleInfo()
	{
		m_iEventUID	 = 0;
		m_iEventID	 = 0;
		m_fEventTime = 0.0f;
#ifdef SERV_ADVERTISEMENT_EVENT
		m_wstrAdvertisementURL = L"";
#endif SERV_ADVERTISEMENT_EVENT
	}

	KTimeEventSimpleInfo( int iEventUID, int iEventID )
	{
		m_iEventUID	 = iEventUID;
		m_iEventID	 = iEventID;
		m_fEventTime = 0.0f;
#ifdef SERV_ADVERTISEMENT_EVENT
		m_wstrAdvertisementURL = L"";
#endif SERV_ADVERTISEMENT_EVENT
	}
};

//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2012. 06. 13	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
DECL_DATA( KDailyGiftBoxInfo )
{
	UidType			m_iUnitUID;
	byte			m_iState;
	std::wstring	m_wstrRegDate;

	KDailyGiftBoxInfo()
	{
		m_iUnitUID = 0;
		m_iState = 0;
	}
};
#endif SERV_EVENT_DAILY_GIFT_BOX
//}}

//{{ 2009. 6. 10  ������	ä���̵�����
DECL_DATA( KChannelChangeInfo )
{
	//{{ 2010. 7. 27  ������	���� �ð� �̺�Ʈ ���� ����
#ifdef SERV_TIME_EVENT_BUG_FIX
	std::map< int, KUserConnectTimeEventInfo >	m_mapEventDurationTime;
#else
	std::map< int, double >						m_mapEventDurationTime;
#endif SERV_TIME_EVENT_BUG_FIX
	//}}
	//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
	int											m_iEquippedTitleID;
#else
	short										m_sEquippedTitleID;
#endif SERV_TITLE_DATA_SIZE
	//}}
	bool										m_bIsPcBang;
#ifdef SERV_PC_BANG_TYPE
	int											m_iPcBangType;
#endif SERV_PC_BANG_TYPE
	std::vector< KInventoryItemInfo >			m_vecPcBangItemInfo;

	//{{ 2012. 06. 14	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	std::multimap<int /*iItemID*/, KDailyGiftBoxInfo>	m_mmapDailyGiftBoxList;
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}
	//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
	std::wstring								m_wstrWinterVacationEventRegDate;
	int											m_iWinterVacationEventCount;
#endif SERV_2012_WINTER_VACATION_EVENT
	//}}
	//{{ 2012. 1. 2		Merge �ڼ���	( 2013.1.2 ���� : �̴ϸ� ä�� �̵� �� �������� ���� �߰� )
#ifdef SERV_CHANNEL_CHANGE_BUG_FIX
	int											m_nUnitSlot;
#endif SERV_CHANNEL_CHANGE_BUG_FIX
	//}}

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-30
	UidType										m_iSummonedRidingPetUID;
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	bool										m_ButtonStartUI;
	bool										m_DungeonClearUI;
	bool										m_FieldCountUI;
	int											m_DungeonCount;
	int											m_FieldMonsterKillCount;
	std::wstring								m_wstrButtonClickTime;
	int											m_RemaindTime;
	bool										m_bCoboItemGive;
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_GLOBAL_EVENT_TABLE
	std::map< int, KGlobalEventTableData >		m_mapGlobalEventData;
#endif //SERV_GLOBAL_EVENT_TABLE

	KChannelChangeInfo()
		: m_bIsPcBang( false )
#ifdef SERV_PC_BANG_TYPE
		, m_iPcBangType( -1 )
#endif SERV_PC_BANG_TYPE
		//{{ 2010. 11. 17	������	Īȣ ������ ũ�� ���̱�
#ifdef SERV_TITLE_DATA_SIZE
		, m_iEquippedTitleID( 0 )
#else
		, m_sEquippedTitleID( 0 )
#endif SERV_TITLE_DATA_SIZE
		//}}
		//{{ 2012. 12. 12	�ڼ���	�ܿ� ���� ���� �̺�Ʈ( �ӽ�, �ϵ� �ڵ� )
#ifdef SERV_2012_WINTER_VACATION_EVENT
		, m_iWinterVacationEventCount( -1 )
#endif SERV_2012_WINTER_VACATION_EVENT
		//}}
		//{{ 2012. 1. 2		Merge �ڼ���	( 2013.1.2 ���� : �̴ϸ� ä�� �̵� �� �������� ���� �߰� )
#ifdef SERV_CHANNEL_CHANGE_BUG_FIX
		, m_nUnitSlot( 0 )
#endif SERV_CHANNEL_CHANGE_BUG_FIX
		//}}
#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-30
		, m_iSummonedRidingPetUID( 0 )
#endif	// SERV_RIDING_PET_SYSTM
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		, m_ButtonStartUI( false )
		, m_DungeonClearUI( false )
		, m_FieldCountUI( false )
		, m_DungeonCount( 0 )
		, m_FieldMonsterKillCount( 0 )
		, m_wstrButtonClickTime( L"1900-01-01 00:00:00" )
		, m_RemaindTime( 0 )
		, m_bCoboItemGive( false )
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
	{
	}
};
//}}

//{{ 2009. 12. 16  ������	������
DECL_DATA( KCCUInfo )
{
	__int64								m_iCCUTime;
	int									m_iCCUCount;
};
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 12. 16  ������	������
DECL_PACKET( E_TOOL_GET_CCU_INFO_REQ )
{
	enum CCU_TYPE
	{
		CT_TODAY = 0,
		CT_YESTERDAY,
		CT_DATE,
		CT_NOWTIME,
	};

	char								m_cCCUType;
	UidType								m_iServerUID;
	std::wstring						m_wstrDate;
};

DECL_PACKET( E_TOOL_GET_CCU_INFO_ACK )
{
	int									m_iOK;
	char								m_cCCUType;
	UidType								m_iServerUID;
	std::wstring						m_wstrDate;
	std::vector< KCCUInfo >				m_vecCCUInfo;
};
//}}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 12. 31  ������	��� ����
DECL_DATA( KGuildAdInfo )
{
    int									m_iGuildUID;
	std::wstring						m_wstrGuildName;
	u_char								m_ucGuildLevel;
	int									m_iGuildEXP;
	std::wstring						m_wstrMasterNickName;
	u_short								m_usCurNumMember;
	std::wstring						m_wstrAdMessage;
	//{{ 2010. 02. 04  ������	��� ���� ���� ����
	std::wstring						m_wstrFoundingDay;
	//}}
	std::wstring						m_wstrAdRegDate;
	std::wstring						m_wstrAdEndDate;
};

DECL_DATA( KApplyJoinGuildInfo )
{
	int									m_iApplyJoinGuildUID;
    UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;
	char                            	m_cUnitClass;
	u_char                           	m_ucLevel;
	std::wstring						m_wstrMessage;
	std::wstring						m_wstrRegDate;
	__int64								m_tRegDate;
};

DECL_DATA( KApplyDeleteInfo )
{
	int									m_iGuildUID;
	UidType								m_iUnitUID;

	KApplyDeleteInfo()
	{
		m_iGuildUID = 0;
		m_iUnitUID	= 0;
	}

	KApplyDeleteInfo( int iGuildUID, UidType iUnitUID )
	{
		m_iGuildUID = iGuildUID;
		m_iUnitUID	= iUnitUID;
	}
};
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2010. 01. 07  ������	��õ�θ���Ʈ
DECL_DATA( KRecommendUserInfo )
{
	enum RECOMMEND_USER_INFO_TYPE
	{
		RUIT_LOGIN = 0,
		RUIT_LOGOUT,
		RUIT_LEVEL_UP,
	};

    UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;
	u_char                           	m_ucLevel;
	std::wstring						m_wstrRecommendDate;
	bool								m_bIsOnline;
#ifdef SERV_RECRUIT_EVENT_BASE
	char								m_cUnitClass;
#endif SERV_RECRUIT_EVENT_BASE

	KRecommendUserInfo()
	{
		m_iUnitUID	= 0;
		m_ucLevel	= 0;
		m_bIsOnline = false;
#ifdef SERV_RECRUIT_EVENT_BASE
		m_cUnitClass = 0;
#endif SERV_RECRUIT_EVENT_BASE
	}
};
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 02. 06  ������	���� �̺�Ʈ ����Ʈ
#ifdef SERV_DAY_QUEST

DECL_DATA( KEventQuestInfo )
{
	bool								m_bInit;
	int									m_iEventUID;
	int									m_iQuestID;

	KEventQuestInfo()
	{
		m_bInit		= false;
		m_iEventUID = 0;
		m_iQuestID	= 0;
	}
};

#endif SERV_DAY_QUEST
//}}


//{{ 2010. 02. 12  ������	��ŷ�� ����Ʈ
#ifdef SERV_HACKING_TOOL_LIST 

DECL_DATA( KHackingToolInfo )
{
	std::wstring						m_wstrProcessName;
	std::wstring						m_wstrWindowClassName;
	char								m_cFlag;    

	KHackingToolInfo() : m_wstrProcessName(), m_wstrWindowClassName(), m_cFlag( 0 )
	{}
};

#endif SERV_HACKING_TOOL_LIST
//}}

//{{ 2010. 02. 23  ������	�� ����Ʈ �̺�Ʈ
#ifdef SERV_WEB_POINT_EVENT

DECL_DATA( KWebPointRewardInfo )
{
	int									m_iApplySN;
	std::wstring						m_wstrUserID;
	std::wstring						m_wstrNickName;
    UidType								m_iUnitUID;
	int									m_iRewardItemID;

	KWebPointRewardInfo()
	{
		m_iApplySN = 0;		
		m_iUnitUID = 0;
		m_iRewardItemID = 0;
	}
};

#endif SERV_WEB_POINT_EVENT
//}}

#ifdef DEF_TRADE_BOARD
DECL_DATA( KTradeBoardItemInfo )
{	
	UidType						m_iPersonalShopUID;
	//{{ 2011. 05. 03	������	�븮����
#ifdef SERV_PSHOP_AGENCY
	UidType						m_iHostUnitUID;
#endif SERV_PSHOP_AGENCY
	//}}
	std::wstring				m_wstrSellerNickName;
	KSellPersonalShopItemInfo	m_kSellItemInfo;

	KTradeBoardItemInfo()
	{		
		m_iPersonalShopUID = 0;
		//{{ 2011. 05. 03	������	�븮����
#ifdef SERV_PSHOP_AGENCY
		m_iHostUnitUID = 0;
#endif SERV_PSHOP_AGENCY
		//}}
	}
};
#endif DEF_TRADE_BOARD


//{{ 2010. 05. 31  ������	���� ���� ����
#ifdef SERV_CCU_NEW

DECL_DATA( KUpdateCCUInfo )
{
	UidType                     m_iServerUID;
	int                         m_iConcurrentUser;
	int							m_iCurPartyCount;
	int							m_iPlayGamePartyCount;
#ifdef SERV_CHANNELING_USER_MANAGER
	std::map<int,int>			m_mapChannelingUserList;
#endif SERV_CHANNELING_USER_MANAGER

	KUpdateCCUInfo()
	{
		m_iServerUID		  = 0;
		m_iConcurrentUser	  = 0;
		m_iCurPartyCount	  = 0;
		m_iPlayGamePartyCount = 0;
	}
};

#endif SERV_CCU_NEW
//}}

//{{ 2011. 09. 29  ��μ�	�˴ٿ��� ����
#ifdef SERV_SHUTDOWN_SYSTEM
DECL_DATA( KRolicyResults )
{
	BYTE					m_byteRolicyCount;
	BYTE					m_byteRolicyListNo;
	std::wstring			m_wstrRolicyName;
	BYTE					m_byteRolicyResult;
	std::wstring			m_wstrOption;

	KRolicyResults()
	{
		m_byteRolicyCount = 0;
		m_byteRolicyListNo = 0;
		m_byteRolicyResult = 0;
	}
};
#endif SERV_SHUTDOWN_SYSTEM
//}}

//{{ 2010. 06. 07  ������	�ؽ�PC�� ���� ���� ����
#ifdef SERV_PCBANG_AUTH_NEW

DECL_DATA( KPropertyValue )
{
	BYTE							m_bytePropertyValue;
	unsigned int					m_uintPropertyValue;
	unsigned long					m_ulongPropertyValue;
	std::wstring					m_wstrPropertyValue;
	std::vector< unsigned long >	m_vecPropertyValue;
	//{{ 2011. 09. 29  ��μ�	�˴ٿ��� ����
#ifdef SERV_SHUTDOWN_SYSTEM
	std::map< BYTE, KRolicyResults >		m_mapRolicyResult;
	__int64							m_int64PropertyValue;
#endif SERV_SHUTDOWN_SYSTEM
	//}}
};

#endif SERV_PCBANG_AUTH_NEW
//}}


//{{ 2010. 06. 20  ������	��� ������
#ifdef SERV_STATISTICS_THREAD

DECL_DATA( KStatisticsInfo )
{
	int								m_iStatisticsID;
	KStatisticsKey					m_kKey;
	int								m_iIndex;
	int								m_iCount;

	KStatisticsInfo()
	{
		m_iStatisticsID	= 0;
		m_iIndex		= 0;
		m_iCount		= 0;
	}
};

#endif SERV_STATISTICS_THREAD
//}}


//{{ 2010. 8. 16	������	�Ⱓ ���� ������ �̺�Ʈ
#ifdef SERV_RESET_PERIOD_EVENT

DECL_DATA( KItemPeriodInfo )
{
	UidType							m_iItemUID;
	short							m_sPeriod;
	std::wstring					m_wstrExpirationDate;
};

#endif SERV_RESET_PERIOD_EVENT
//}}


//{{ 2010. 10. 13	������	DLL Manager
#ifdef SERV_DLL_MANAGER

DECL_DATA( KModuleInfo )
{
	//{{  2012. 1. 30	��μ�		�ѹ��� ���(��ŷ ���μ��� �˻�)
#ifdef SERV_HACKING_DLL_CHECK
	enum MODULE_INFO_ALLOW
	{
		MIA_NONE			= 0,
		MIA_BLACK_LIST		= 1,
		MIA_MONITORING		= 2,
	};
#endif SERV_HACKING_DLL_CHECK
	//}}

	int								m_iModuleUID;
	std::wstring					m_wstrModuleName;
	__int64							m_iCount;
	int								m_iFlag;
	//{{  2012. 1. 30	��μ�		�ѹ��� ���(��ŷ ���μ��� �˻�)
#ifdef SERV_HACKING_DLL_CHECK
	int								m_iAllow;
#else
	bool							m_bAllow;
#endif SERV_HACKING_DLL_CHECK
	//}}
	bool							m_bReserveDBUpdate;

	KModuleInfo()
	{
		m_iModuleUID		= 0;
		m_iCount			= 0;
		m_iFlag				= 0;
		//{{  2012. 1. 30	��μ�		�ѹ��� ���(��ŷ ���μ��� �˻�)
#ifdef SERV_HACKING_DLL_CHECK
		m_iAllow			= MIA_NONE;
#else
		m_bAllow			= false;
#endif SERV_HACKING_DLL_CHECK
		//}}
		m_bReserveDBUpdate	= false;
	}
};

#endif SERV_DLL_MANAGER
//}}


//{{ 2010. 10. 27	������	�� ���� ��ų
#ifdef SERV_PET_AURA_SKILL

DECL_DATA( KStatIncreaseRate )
{
	float							m_fIncreaseAtkPhysicRate;
	float							m_fIncreaseAtkMagicRate;
	float							m_fIncreaseDefPhysicRate;
	float							m_fIncreaseDefMagicRate;

	KStatIncreaseRate()
	{
		m_fIncreaseAtkPhysicRate	= 0.f;
		m_fIncreaseAtkMagicRate		= 0.f;
		m_fIncreaseDefPhysicRate	= 0.f;
		m_fIncreaseDefMagicRate		= 0.f;
	}
};

#endif SERV_PET_AURA_SKILL
//}}


//{{ 2010. 11. 17	������	������ ���� ���
//#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM

DECL_DATA( KAbuserEventLog )
{
	UidType							m_iUserUID;
	UidType							m_iUnitUID;
	int								m_iED;
	int								m_iGetItemCount;
	//{{ 2013. 05. 29	������	��Ŷ ����͸� �ټ��� �׸� �߰�
#ifdef SERV_PACKET_MORNITORING_SPIRIT_COL
	int								m_iSpirit;
#endif SERV_PACKET_MORNITORING_SPIRIT_COL
	//}}
	__int64							m_iRegDate;
	unsigned short					m_usEventID;

	KAbuserEventLog()
	{
		m_iUserUID = 0;
		m_iUnitUID = 0;
		m_iED = 0;
		m_iGetItemCount = 0;
		//{{ 2013. 05. 29	������	��Ŷ ����͸� �ټ��� �׸� �߰�
#ifdef SERV_PACKET_MORNITORING_SPIRIT_COL
		m_iSpirit = 0;
#endif SERV_PACKET_MORNITORING_SPIRIT_COL
		//}}
		m_iRegDate = 0;
		m_usEventID = 0;
	}
};

//#endif SERV_AUTO_HACK_CHECK_GET_ITEM
//}}


//{{ 2010. 11. 20	������	Ư�� �ð� ���� ����
#ifdef SERV_TIME_DROP_MONSTER_EVENT

DECL_DATA( KDungeonDropEventNpcInfo )
{
    int								m_iEventUID;
	int								m_iNpcID;
	__int64							m_tDropDate;

	KDungeonDropEventNpcInfo()
	{
		m_iEventUID		= 0;
		m_iNpcID		= 0;
		m_tDropDate		= 0;
	}
};

#endif SERV_TIME_DROP_MONSTER_EVENT
//}}


//{{ 2010. 12. 8	������	���� ���� ��� ���
#ifdef SERV_APPROPRIATE_LEVEL_STAT

DECL_DATA( KUserAppropriateLevelInfo )
{
    UidType							m_iUnitUID;
	u_char							m_ucLevel;
	int								m_iWeaponLevel;
	int								m_iWeaponGrade;
	int								m_iStartedAtkPhysic;
	int								m_iStartedAtkMagic;

	KUserAppropriateLevelInfo()
	{
		Clear();		
	}

	void Clear()
	{
		m_iUnitUID			= 0;
		m_ucLevel			= 0;
		m_iWeaponLevel		= 0;
		m_iWeaponGrade		= 0;
		m_iStartedAtkPhysic	= 0;
		m_iStartedAtkMagic	= 0;
	}
};

#endif SERV_APPROPRIATE_LEVEL_STAT
//}}


//{{ 2010. 12. 20	������	Ư�� �ð� ť�� ����
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT

DECL_DATA( KTimeOpenRandomItemEventInfo )
{
	int								m_iID;
	int								m_iItemID;
	std::wstring					m_wstrDropTime;
	//{{ 2011. 07. 04    ��μ�    ���̹� ���θ��
#ifdef SERV_NAVER_PROMOTIONS
	std::wstring					m_wstrDropEndTime;
#endif SERV_NAVER_PROMOTIONS
	//}} 
	//{{ 2011. 12.13    ��μ�	���� �̺�Ʈ ��� �߰� (�ݺ� ����)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
	int								m_iOpenItemID;
#endif SERV_RANDOM_CUBE_GOLD_REWARD
	//}}


	KTimeOpenRandomItemEventInfo()
	{
		m_iID		= 0;
		m_iItemID	= 0;
	}
};

DECL_DATA( KTimeOpenRandomItemEventDBUPdate )
{
	int								m_iID;
	__int64							m_tUpdateTime;
	UidType							m_iGetUserUID;
	UidType							m_iGetUnitUID;

	KTimeOpenRandomItemEventDBUPdate()
	{
		m_iID			= 0;
		m_iGetUserUID	= 0;
		m_iGetUnitUID	= 0;
	}
};

#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
//}}

//{{ 2011. 01. 13 ��μ�	���� ����͸� ��
#ifdef SERV_CCU_MONITORING_TOOL

DECL_DATA( KToolServerList )
{
	int				m_iServerUID;
	int				m_iServerSetID;
	int				m_iServerType;
	int				m_iChannelID;
	std::wstring	m_wstrChannelName;
	std::wstring	m_wstrIP;
	int				m_iPort;
};

DECL_PACKET( E_TOOL_CHECK_LOGIN_REQ )
{
	std::wstring						m_wstrID;
	std::wstring						m_wstrPW;
};

DECL_PACKET( E_TOOL_CHECK_LOGIN_ACK )
{
	int									m_iAuthLevel;
};

DECL_PACKET( E_TOOL_SERVER_LIST_ACK )
{
	int									m_iServerCount;
	std::vector< KToolServerList >		m_vecServerList;
};
#endif SERV_CCU_MONITORING_TOOL
//}}


//{{ 2011. 01. 25  ��μ�  ���� Ʈ���� ����(������ ������ - ���� PVP Ȯ��)
#ifdef SERV_WORLD_TRIGGER_RELOCATION
DECL_PACKET( EGS_WORLD_TRIGGER_RELOCATION_REQ )
{
	int									m_iWorldTrigger;
};

typedef KPacketOK								KEGS_WORLD_TRIGGER_RELOCATION_ACK;

typedef KEGS_WORLD_TRIGGER_RELOCATION_REQ		KEGS_WORLD_TRIGGER_RELOCATION_NOT;

#endif SERV_WORLD_TRIGGER_RELOCATION
//}}


//{{ 2011. 02. 23	������	ĳ�� ��ǰ ����Ʈ
#ifdef SERV_CASH_ITEM_LIST
DECL_DATA( KCashPruductInfo )
{
	unsigned long                           m_ulProductNo;
	short									m_sProductExpire;
	u_short									m_usProductPieces;
	int										m_iItemID;
	unsigned long                           m_ulSalePrice;

	KCashPruductInfo()
	{
		m_ulProductNo = 0;
		m_sProductExpire = 0;
		m_usProductPieces = 0;
		m_iItemID = 0;
		m_ulSalePrice = 0;
	}
};
#endif SERV_CASH_ITEM_LIST
//}}

//{{ 2011. 03. 26  ��μ�	û 1�� �߰� ��ų �߰� - ����
#ifdef SERV_ADD_BUFF_SKILL_INFO
DECL_DATA( KBuffSkillInfo )
{
	UidType									m_iCaughtUnitUID;		// �ɸ� ���
	int										m_iBuffEnum;			// ���� ����
	UidType									m_iCastingUnitUID;		// ������(�Ǵ� ĵ��)
	bool									m_bIsBuffCasting;		// ���� = true , ĵ�� = false
	char									m_cBuffLevel;			// ���� ����
};

DECL_PACKET( EGS_BUFF_SKILL_INFO_CONVEY_REQ )
{
	int										m_iBuffEnum;			// ���� ����
	UidType									m_iCastingUnitUID;		// ������(�Ǵ� ĵ��)
	float									m_fBuffTime;			// ���ӽð�
	bool									m_bIsBuffCasting;		// ���� = true , ĵ�� = false
	char									m_cBuffLevel;			// ���� ����
	std::vector< UidType >					m_vecCaughtUIDList;		// �ɸ������
};

typedef KPacketOK								KEGS_BUFF_SKILL_INFO_CONVEY_ACK;

DECL_PACKET( EGS_BUFF_SKILL_INFO_CONVEY_NOT )
{
	// key : BuffEnum
	std::map< int, std::vector< KBuffSkillInfo > >		m_mapCaughtUIDList;		// �ɸ������ 
};
#endif SERV_ADD_BUFF_SKILL_INFO
//}}

//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
DECL_PACKET( DBE_WRITE_LETTER_AND_TRADE_MOINTORING_REQ )
{
	char									m_cType;
	UidType									m_iRecvUnitUID;
	std::wstring							m_wstrRegDate;
};

typedef KPacketOK								KDBE_WRITE_LETTER_AND_TRADE_MOINTORING_ACK;

#endif SERV_MONITORING_LETTER_AND_TRADE
//}}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
DECL_DATA( KDefenseOpenTime )
{
	int										m_iStartHour;		// ���潺 ���� ���� ��
	int										m_iStartMinute;		// ���潺 ���� ���� ��
	int										m_iDurationTime;	// ���潺 ���� ���� �ð�
};

DECL_DATA( KAttribNpcUnitInfo )
{
	KNPCUnitReq								m_sNPCUnitReq;
	KAttribEnchantNpcInfo					m_sAttribEnchantNpcInfo;
};
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

//{{ 2011. 06. 23	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM

DECL_DATA( KMatchUserInfo )
{
	UidType					m_iGSUID;	
	UidType					m_iUnitUID;
	UidType					m_iPartyUID;
	std::wstring			m_wstrNickName;
	char					m_cUnitClass;
	UCHAR					m_ucLevel;

	//{{ 2012. 06. 20	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	char					m_cCurrentRank;
	int						m_iOfficialMatchCnt;
#endif SERV_2012_PVP_SEASON2
	//}}

	int						m_iRating;
	int						m_iRPoint;
	int						m_iMatchWaitTime;

	KMatchUserInfo()
	{
		m_iGSUID			= 0;
		m_iUnitUID			= 0;
		m_iPartyUID			= 0;
		m_cUnitClass		= 0;
		m_ucLevel			= 0;

		//{{ 2012. 06. 20	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		m_cCurrentRank		= 0;
		m_iOfficialMatchCnt	= 0;
#endif SERV_2012_PVP_SEASON2
		//}}

		m_iRating			= 0;
		m_iRPoint			= 0;
		m_iMatchWaitTime	= 0;
	}
};

DECL_DATA( KPvpNpcInfo )
{
    int						m_iNpcID;

	KPvpNpcInfo()
	{
		m_iNpcID = 0;
	}
};

DECL_DATA( KPvpMatchUserResultLog )
{
	enum END_TYPE
	{
		ET_EALRY_LEAVE = 0,
		ET_ALL_KILL,
		ET_TIME_OUT,
		ET_LEAVE_ALL,
		ET_LEAVE_SELF,
		ET_ETC,
	};

	UidType					m_iUnitUID;
	std::wstring			m_wstrNickName;
	u_char					m_ucLevel;
	char					m_cUnitClass;
	int						m_iRating;
	char					m_cPvpRank;
    int						m_iRPoint;
	int						m_iAPoint;
	int						m_iOfficialMatchCnt;
	char					m_cTeam;
	bool					m_bPartyApp;
	int						m_iWaitTime;
	int						m_iWin;	
	int						m_iKill;
	int						m_iAssist;
	int						m_iDeath;
	int						m_iNetRatingInc;
	int						m_iNetRPInc;
	int						m_iNetAPInc;
	int						m_iEXPearned;
	char					m_cEndType;

	KPvpMatchUserResultLog()
	{
		m_iUnitUID = 0;
		m_ucLevel = 0;
		m_cUnitClass = 0;
		m_iRating = 0;
		m_cPvpRank = 0;
		m_iRPoint = 0;
		m_iAPoint = 0;
		m_iOfficialMatchCnt = 0;
		m_cTeam = 0;
		m_bPartyApp = false;
		m_iWaitTime = 0;
		m_iWin = 0;		
		m_iKill = 0;
		m_iAssist = 0;
		m_iDeath = 0;
		m_iNetRatingInc = 0;
		m_iNetRPInc = 0;
		m_iNetAPInc = 0;
		m_iEXPearned = 0;
		m_cEndType = ET_ETC;
	}
};

DECL_DATA( KPvpPlayResultInfo )
{
	int						m_iRPoint;
	int						m_iAPoint;
	int						m_iWinCount;
	bool					m_bIsPlayWithParty;

	KPvpPlayResultInfo()
	{
		m_iRPoint			= 0;
		m_iAPoint			= 0;
		m_iWinCount			= 0;
		m_bIsPlayWithParty	= false;
	}
};

#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL
DECL_DATA( KRewardItem )
{
	int             m_iItemID;
	int             m_iQuantity;

	KRewardItem()
	{
		m_iItemID	= 0;
		m_iQuantity	= 0;
	}
};
#else
//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
DECL_DATA( KRewardItem )
{
	int             m_iItemID;
	int             m_iQuantity;

	KRewardItem()
	{
		m_iItemID	= 0;
		m_iQuantity	= 0;
	}
};
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#endif SERV_NEXON_SESSION_PROTOCOL
//}} 

//{{ 2012. 10. 10	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
DECL_PACKET( DBE_BINGO_EVENT_INFO_READ_ACK )		// ���� ���� �̺�Ʈ ����
{
	int										m_iOK;
	int										m_iBalance;
	int										m_iLotteryChance;	// �̱� ���� Ƚ��
	int										m_iLotteryRecord;	// ���� Ƚ��
	int										m_iMixChance;		// �ڼ��� ���� Ƚ��
	int										m_iResetChance;		// �ʱ�ȭ ���� Ƚ��
	std::vector<byte>						m_vecBingoBoard;	// ���� ���� ������
	std::vector< std::pair<int, bool> >		m_vecPresentInfo;	// ���� ���� ����

	KDBE_BINGO_EVENT_INFO_READ_ACK()
	{
		clear();
	}

	void clear( void )
	{
		m_iOK				= 0;
		m_iBalance			= 0;
		m_iLotteryChance	= 0;
		m_iLotteryRecord	= 0;
		m_iMixChance		= 0;
		m_iResetChance		= 0;
		m_vecBingoBoard.clear();
		m_vecPresentInfo.clear();
	}
};
#endif SERV_EVENT_BINGO
//}}

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EGS_SELECT_UNIT_REQ )
{
	UidType                 m_iUnitUID;
#ifdef SERV_CODE_EVENT
	std::set< int >			m_setCodeEventScriptID;
#endif SERV_CODE_EVENT

#ifdef SERV_GLOBAL_EVENT_TABLE
	std::map< int, KGlobalEventTableData >	m_mapGlobalEventData;
#endif //SERV_GLOBAL_EVENT_TABLE

	KEGS_SELECT_UNIT_REQ()
	{
		m_iUnitUID = 0;
#ifdef SERV_CODE_EVENT
		m_setCodeEventScriptID.clear();
#endif SERV_CODE_EVENT

#ifdef SERV_GLOBAL_EVENT_TABLE
		m_mapGlobalEventData.clear();
#endif //SERV_GLOBAL_EVENT_TABLE

	}
};


//////////////////////////////////////////////////////////////////////////
//{{ 2012. 12. 10  ĳ���� ���� ��Ŷ ���� - ��μ�
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
DECL_PACKET( EGS_SELECT_UNIT_1_NOT )	// ������ ����, �κ��丮, ��ų
{
	int																m_iOK;
	KUnitInfo														m_kUnitInfo;
	//{{ 2011. 01. 18	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
	std::wstring													m_wstrUnitCreateDate;
#endif SERV_CHAR_LOG
	//}}
	//{{ 2011. 09. 23	������	�α׿��� ���� EDüũ
#ifdef SERV_LOGOUT_ED_CHECK
	int																m_iRealDataED;
#endif SERV_LOGOUT_ED_CHECK
	//}}
	//{{ 2011. 03. 22	������	ĳ���� ù ���� �α�
#ifdef SERV_DAILY_CHAR_FIRST_SELECT
	std::wstring													m_wstrUnitLastLoginDate;
#endif SERV_DAILY_CHAR_FIRST_SELECT
	//}}

	// ī�װ�, ���� ����
	std::map< int, int >											m_mapInventorySlotSize;
	std::map< UidType, KInventoryItemInfo >							m_mapItem;

	int																m_iNumResurrectionStone;	// ��Ȱ��
	//{{ 2009. 10. 14  ������	���ʺ�Ȱ��
#ifdef AP_RESTONE
	int																m_iNumAutoPaymentResStone;
	std::wstring													m_wstrAutoPayResStoneLastDate;
#endif AP_RESTONE
	//}}

#ifdef SERV_SKILL_PAGE_SYSTEM
	std::vector< KUserSkillPageData >								m_vecUserSkillPageData;		// ȹ���� ��ų ������
#else //SERV_SKILL_PAGE_SYSTEM
	std::vector< KUserSkillData >									m_vecSkillAcquired;		// ȹ���� ��ų ����Ʈ
#endif // SERV_SKILL_PAGE_SYSTEM

	std::vector< short >											m_vecSkillUnsealed;		// ���������� ��ų ����Ʈ

	//{{ 2010. 03. 13  ������	����� ��Ʈ
#ifdef SERV_SKILL_NOTE
	char															m_cSkillNoteMaxPageNum;
	std::map< char, int >											m_mapSkillNote;			// [key:SlotID, value:MemoID]
#endif SERV_SKILL_NOTE
	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	bool															m_bIsNewUnitTradeBlock;
	int																m_iNewUnitTradeBlockDay;
	int																m_iNewUnitTradeBlockUnitClass;
#endif // SERV_NEW_UNIT_TRADE_LIMIT

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	KRelationshipInfo												m_kRelationshipInfo;
#endif SERV_RELATIONSHIP_SYSTEM
	//}

	KEGS_SELECT_UNIT_1_NOT()
	{
		m_iOK = 0;
		//{{ 2011. 09. 23	������	�α׿��� ���� EDüũ
#ifdef SERV_LOGOUT_ED_CHECK
		m_iRealDataED					= 0;
#endif SERV_LOGOUT_ED_CHECK
		//}}
		//{{ 2009. 10. 14  ������	���ʺ�Ȱ��
#ifdef AP_RESTONE
		m_iNumResurrectionStone			= 0;
		m_iNumAutoPaymentResStone		= 0;
#endif AP_RESTONE
		//}}
		//{{ 2010. 03. 13  ������	����� ��Ʈ
#ifdef SERV_SKILL_NOTE
		m_cSkillNoteMaxPageNum			= 0;
#endif SERV_SKILL_NOTE
		//}}
#ifdef SERV_NEW_UNIT_TRADE_LIMIT
		m_bIsNewUnitTradeBlock			= false;
		m_iNewUnitTradeBlockDay			= 0;
		m_iNewUnitTradeBlockUnitClass	= 0;
#endif // SERV_NEW_UNIT_TRADE_LIMIT
	}
};

DECL_PACKET( EGS_SELECT_UNIT_2_NOT )	// ����Ʈ
{
	int													m_iOK;
	std::vector< KQuestInstance >						m_vecQuest;
	//{{ 2010. 04. 02  ������	��������Ʈ
#ifdef SERV_DAILY_QUEST
	std::vector< KCompleteQuestInfo >					m_vecCompletQuest;
#else
	std::vector< int >									m_vecCompletQuest;
#endif SERV_DAILY_QUEST
	//}}
	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	std::map< int, int >								m_mapRandomQuestList;
#endif SERV_RANDOM_DAY_QUEST
	//}}

	KEGS_SELECT_UNIT_2_NOT()
	{
		m_iOK				= 0;
	}
};

DECL_PACKET( EGS_SELECT_UNIT_3_NOT )	// Ÿ��Ʋ, ��
{
	int													m_iOK;
	//{{ 2008. 10. 6  ������	Ÿ��Ʋ
	std::vector< KMissionInstance >						m_vecMission;
	std::vector< KTitleInfo >							m_vecTitle;

	//{{ 2010. 7. 21  ������	�� �ý���
#ifdef SERV_PET_SYSTEM
	UidType															m_iSummonedPetUID;
	std::vector< KPetInfo >											m_vecPetList;
	std::vector< UidType >											m_vecCanEvolutionPetList;
	std::vector< UidType >											m_vecNeverSummonPetList;
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2010. 8. 2	������	�� �ý���
#ifdef SERV_PET_SYSTEM
	std::map< UidType, std::map< UidType, KInventoryItemInfo > >	m_mapPetItem; // ���� ����
#endif SERV_PET_SYSTEM
	//}}

	KEGS_SELECT_UNIT_3_NOT()
	{
		m_iOK							= 0;
		//{{ 2010. 7. 21  ������	�� �ý���
#ifdef SERV_PET_SYSTEM
		m_iSummonedPetUID				= 0;
#endif SERV_PET_SYSTEM
		//}}
	}
};

DECL_PACKET( EGS_SELECT_UNIT_4_NOT )	// �����̺�Ʈ, ����, ����, ��ŷ
{
	int														m_iOK;
	std::vector< KChatBlackListUnit >						m_vecChatBlackList;
	KDenyOptions											m_kDenyOptions;

	//{{ 2008. 3. 31  ������  ��õ��
	bool													m_bIsRecommend;
	UidType													m_iRecommendUnitUID;
#ifdef SERV_RECRUIT_EVENT_BASE
	std::vector< KRecommendUserInfo >						m_vecRecruiterUnitInfo;
	std::vector< KRecommendUserInfo >						m_vecRecruitUnitInfo;
#endif SERV_RECRUIT_EVENT_BASE

	//080405. hoons. ��������Ʈ ������
	std::vector< KTutorialDBUnitInfo >						m_vecTutorialDBUnitInfo;
	//{{ 2008. 5. 19  ������  ���� �ð� �̺�Ʈ
	std::vector< KConnectTimeEventInfo >					m_vecConnectTimeEvent;
	//{{ 2009. 12. 7  ������	�����ð��̺�Ʈ
#ifdef CUMULATIVE_TIME_EVENT
	std::vector< KCumulativeTimeEventInfo >					m_vecCumulativeTimeEvent;
#endif CUMULATIVE_TIME_EVENT
	//}}
	//{{ 2009. 7. 7  ������		��ŷ����
	std::vector< KHenirRankingInfo >						m_vecHenirRanking;

	KMessengerInfo											m_kMessengerInfo;
	//{{ 2012. 02. 21	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KGamePlayStatusContainer								m_kGamePlayStatus;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	KEGS_SELECT_UNIT_4_NOT()
	{
		m_iOK					= 0;
		m_bIsRecommend			= false;
		m_iRecommendUnitUID		= 0;
	}
};

DECL_PACKET( EGS_SELECT_UNIT_5_NOT )	// 1ȸ�� ������, ETC
{
	int											m_iOK;

	//{{ 2008. 5. 7  ������  ���
	bool										m_bIsSpiritUpdated;
	//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
	std::map< int, int >						m_mapCharGameCount;
#endif SERV_CHAR_LOG
	//}}
	//{{ 2011. 04. 18	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	KUserPShopAgencyInfo						m_kPShopAgencyInfo;
#endif SERV_PSHOP_AGENCY
	//}}
	//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
	KEGS_HENIR_REWARD_COUNT_NOT					m_PacketHenirRewardCount;
#endif SERV_NEW_HENIR_TEST
	//}}
	//{{ 2011. 10. 13	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	bool										m_bCashShopOpen;
#endif SERV_CONTENT_MANAGER
	//}}
#ifdef SERV_EVENT_MONEY	// ��μ� // ���볯¥: 2013-07-04
	int											m_iEventMoney;
#endif // SERV_EVENT_MONEY
	//{{ 2012 ���� ����2 ���� ��Ī �̺�Ʈ	- ��μ�
#ifdef SERV_2012_PVP_SEASON2_EVENT
	int											m_iPvpEventIndex;
	std::wstring								m_wstrLastPvpEventDate;
#endif SERV_2012_PVP_SEASON2_EVENT
	//}}
	//{{ 2012. 10. 10	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	KDBE_BINGO_EVENT_INFO_READ_ACK				m_kBingoEvent;
#endif SERV_EVENT_BINGO
	//}}
	//{{ 2012. 12. 20	������	�ƶ� ù ���� Ʃ�丮��
#ifdef SERV_ARA_FIRST_SELECT_TUTORIAL
	bool										m_bFirstSelect;
#endif SERV_ARA_FIRST_SELECT_TUTORIAL
	//}}
#ifdef SERV_CUSTOM_CONNECT_EVENT
	bool									m_iCustomEventID;
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-03-31
	bool										m_bLocalRankingUser;
#endif	// SERV_LOCAL_RANKING_SYSTEM

#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
	int						m_iConnectExperienceAck;
	int						m_iReward7DaysItem;
#endif //SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
	int											m_iGateOfDarknessSupportEventTime;
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
	bool										m_bCouple;
	UidType										m_iRelationTargetUserUid;
	std::wstring								m_wstrRelationTargetUserNickname;
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_STEAM_USER_CONNECT_EVENT
	bool				m_bOnlySteamUser;
	bool				m_bOnlyNotSteamUser;
#endif //SERV_STEAM_USER_CONNECT_EVENT

#ifdef SERV_ELESIS_UPDATE_EVENT
	int					m_iNoteViewCount;
#endif SERV_ELESIS_UPDATE_EVENT

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
	std::wstring							m_wstrGiveMeTheItemTime_One;
	std::wstring							m_wstrGiveMeTheItemTime_Two;
	std::wstring							m_wstrGiveMeTheItemTime_Tree;
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_4TH_ANNIVERSARY_EVENT
	std::vector<bool>					m_vec4ThAnnivEventRewardInfo;		// 4�ֳ� �̺�Ʈ ���� ���� ����
	__int64								m_tLastRewardTime;	// ���������� ���� ���� �ð�
#endif //SERV_4TH_ANNIVERSARY_EVENT

	KEGS_SELECT_UNIT_5_NOT()
	{
		m_iOK = 0;

		m_bIsSpiritUpdated		= false;
		//{{ 2011. 10. 13	������	������ ������
#ifdef SERV_CONTENT_MANAGER
		m_bCashShopOpen			= false;
#endif SERV_CONTENT_MANAGER
		//}}
#ifdef SERV_EVENT_MONEY	// ��μ� // ���볯¥: 2013-07-04
		m_iEventMoney			= 0;
#endif // SERV_EVENT_MONEY
		//{{ 2012 ���� ����2 ���� ��Ī �̺�Ʈ	- ��μ�
#ifdef SERV_2012_PVP_SEASON2_EVENT
		m_iPvpEventIndex		= 0;
#endif SERV_2012_PVP_SEASON2_EVENT
		//}}
		//{{ 2012. 12. 20	������	�ƶ� ù ���� Ʃ�丮��
#ifdef SERV_ARA_FIRST_SELECT_TUTORIAL
		m_bFirstSelect			= false;
#endif SERV_ARA_FIRST_SELECT_TUTORIAL
		//}}
#ifdef SERV_CUSTOM_CONNECT_EVENT
		m_iCustomEventID		= 0;
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-03-31
		m_bLocalRankingUser		= false;
#endif	// SERV_LOCAL_RANKING_SYSTEM

#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
		m_iConnectExperienceAck	= -1;
		m_iReward7DaysItem		= -1;
#endif // SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		m_iGateOfDarknessSupportEventTime = 0;
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
		m_bCouple				= false;
		m_iRelationTargetUserUid = 0;
		m_wstrRelationTargetUserNickname = L"";
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_STEAM_USER_CONNECT_EVENT
		m_bOnlySteamUser	= false;
		m_bOnlyNotSteamUser	 = false;
#endif //SERV_STEAM_USER_CONNECT_EVENT

#ifdef SERV_ELESIS_UPDATE_EVENT
		m_iNoteViewCount	= 0;
#endif SERV_ELESIS_UPDATE_EVENT

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
		m_wstrGiveMeTheItemTime_One	= L"1900-01-01 00:00:00";
		m_wstrGiveMeTheItemTime_Two	= L"1900-01-01 00:00:00";
		m_wstrGiveMeTheItemTime_Tree = L"1900-01-01 00:00:00";
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_4TH_ANNIVERSARY_EVENT
		m_tLastRewardTime = 0;
#endif //SERV_4TH_ANNIVERSARY_EVENT
	}
};

DECL_PACKET( EGS_SELECT_UNIT_ACK )
{
	int                                     m_iOK;

	KEGS_SELECT_UNIT_ACK()
	{
		m_iOK = 0;
	}
};
#else
DECL_PACKET( EGS_SELECT_UNIT_ACK )
{
	int                                     m_iOK;
	KUnitInfo                               m_kUnitInfo;
	//{{ 2011. 01. 18	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
	std::wstring							m_wstrUnitCreateDate;
#endif SERV_CHAR_LOG
	//}}
	//{{ 2011. 09. 23	������	�α׿��� ���� EDüũ
#ifdef SERV_LOGOUT_ED_CHECK
	int										m_iRealDataED;
#endif SERV_LOGOUT_ED_CHECK
	//}}
	//{{ 2011. 03. 22	������	ĳ���� ù ���� �α�
#ifdef SERV_DAILY_CHAR_FIRST_SELECT
	std::wstring							m_wstrUnitLastLoginDate;
#endif SERV_DAILY_CHAR_FIRST_SELECT
	//}}

	// ī�װ�, ���� ����
	std::map< int, int >                    m_mapInventorySlotSize;
	std::map< UidType, KInventoryItemInfo > m_mapItem;
	//{{ 2010. 8. 2	������	�� �ý���
#ifdef SERV_PET_SYSTEM
	std::map< UidType, std::map< UidType, KInventoryItemInfo > > m_mapPetItem; // ���� ����
#endif SERV_PET_SYSTEM
	//}}

	int										m_iNumResurrectionStone; // ��Ȱ��
	//{{ 2009. 10. 14  ������	���ʺ�Ȱ��
#ifdef AP_RESTONE
	int										m_iNumAutoPaymentResStone;
	std::wstring							m_wstrAutoPayResStoneLastDate;
#endif AP_RESTONE
	//}}

	std::vector< KQuestInstance >			m_vecQuest;
	//{{ 2010. 04. 02  ������	��������Ʈ
#ifdef SERV_DAILY_QUEST
	std::vector< KCompleteQuestInfo >		m_vecCompletQuest;
#else
	std::vector< int >						m_vecCompletQuest;
#endif SERV_DAILY_QUEST
	//}}	

	//{{ 2008. 10. 6  ������	Ÿ��Ʋ
	std::vector< KMissionInstance >			m_vecMission;
	std::vector< KTitleInfo >				m_vecTitle;
	//}}

	std::vector< KUserSkillData >			m_vecSkillAcquired;			// ȹ���� ��ų ����Ʈ
	std::vector< short >					m_vecSkillUnsealed;			// ���������� ��ų ����Ʈ

	//{{ 2010. 03. 13  ������	����� ��Ʈ
#ifdef SERV_SKILL_NOTE
	char									m_cSkillNoteMaxPageNum;
	std::map< char, int >					m_mapSkillNote;				// [key:SlotID, value:MemoID]
#endif SERV_SKILL_NOTE
	//}}

	std::vector< KChatBlackListUnit >		m_vecChatBlackList;
	KDenyOptions							m_kDenyOptions;

	//{{ 2008. 3. 31  ������  ��õ��
	bool									m_bIsRecommend;
	UidType									m_iRecommendUnitUID;
	//}}

	//080405. hoons. ��������Ʈ ������
	std::vector< KTutorialDBUnitInfo >		m_vecTutorialDBUnitInfo;

	//{{ 2008. 5. 7  ������  ���
	bool									m_bIsSpiritUpdated;
	//}}

	//{{ 2008. 5. 19  ������  ���� �ð� �̺�Ʈ
	std::vector< KConnectTimeEventInfo >	m_vecConnectTimeEvent;
	//}}
	//{{ 2009. 12. 7  ������	�����ð��̺�Ʈ
#ifdef CUMULATIVE_TIME_EVENT
	std::vector< KCumulativeTimeEventInfo > m_vecCumulativeTimeEvent;
#endif CUMULATIVE_TIME_EVENT
	//}}

	//{{ 2009. 7. 7  ������		��ŷ����
	std::vector< KHenirRankingInfo >		m_vecHenirRanking;
	//}}

	KMessengerInfo                          m_kMessengerInfo;

	//{{ 2010. 7. 21  ������	�� �ý���
#ifdef SERV_PET_SYSTEM
	UidType									m_iSummonedPetUID;
	std::vector< KPetInfo >					m_vecPetList;
	std::vector< UidType >					m_vecCanEvolutionPetList;
	std::vector< UidType >					m_vecNeverSummonPetList;
#endif SERV_PET_SYSTEM
	//}}

	//{{ 2011. 01. 17	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
	std::map< int, int >					m_mapCharGameCount;
#endif SERV_CHAR_LOG
	//}}
	//{{ 2011. 04. 18	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	KUserPShopAgencyInfo					m_kPShopAgencyInfo;
#endif SERV_PSHOP_AGENCY
	//}}

	//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
	KEGS_HENIR_REWARD_COUNT_NOT				m_PacketHenirRewardCount;
#endif SERV_NEW_HENIR_TEST
	//}}

	//{{ 2011. 08. 29	��μ�       ���� ���� ����Ʈ
#ifdef SERV_RANDOM_DAY_QUEST
	std::map< int, int >					m_mapRandomQuestList;
#endif SERV_RANDOM_DAY_QUEST
	//}}
	//{{ 2011. 10. 13	������	������ ������
#ifdef SERV_CONTENT_MANAGER
	bool									m_bCashShopOpen;
#endif SERV_CONTENT_MANAGER
	//}}
	//{{ 2012. 02. 21	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KGamePlayStatusContainer				m_kGamePlayStatus;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
#ifdef SERV_EVENT_MONEY	// ��μ� // ���볯¥: 2013-07-04
	int										m_iEventMoney;
#endif // SERV_EVENT_MONEY

	//{{ 2012 ���� ����2 ���� ��Ī �̺�Ʈ	- ��μ�
#ifdef SERV_2012_PVP_SEASON2_EVENT
	int										m_iPvpEventIndex;
	std::wstring							m_wstrLastPvpEventDate;
#endif SERV_2012_PVP_SEASON2_EVENT
	//}}

	//{{ 2012. 10. 10	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
	KDBE_BINGO_EVENT_INFO_READ_ACK			m_kBingoEvent;
#endif SERV_EVENT_BINGO
	//}}

#ifdef SERV_CUSTOM_CONNECT_EVENT
	bool									m_iCustomEventID;
#endif //SERV_CUSTOM_CONNECT_EVENT

	KEGS_SELECT_UNIT_ACK()
		: m_iOK( 0 )
#ifdef SERV_CUSTOM_CONNECT_EVENT
		, m_iCustomEventID( 0 )
#endif //SERV_CUSTOM_CONNECT_EVENT
	{
	}
};
#endif SERV_SELECT_UNIT_PACKET_DIVISION
//}

//{{ 2011. 12. 14	������	��Ŷ ó�� ���� �ð� üũ
#ifdef SERV_PACKET_PROCESSING_LATENCY_CHECK
DECL_DATA( KPacketProcessingLatencyInfo )
{
	u_short						m_usEventID;
	UidType						m_iUnitUID;
	DWORD						m_dwMaxLatency;
	std::wstring				m_wstrMaxLatencyTime;

	KPacketProcessingLatencyInfo()
	{
		m_usEventID = 0;
		m_iUnitUID = 0;
		m_dwMaxLatency = 0;
	}
};

DECL_DATA( KPacketProcessingCountInfo )
{
	u_short						m_usEventID;
	DWORD						m_dwCount;

	KPacketProcessingCountInfo()
	{
		m_usEventID = 0;
		m_dwCount = 0;
	}
};
#endif SERV_PACKET_PROCESSING_LATENCY_CHECK
//}}

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
DECL_DATA( EVENT_DATA )
{
	float	m_fEXPRate;
	float	m_fVPRate;
	float	m_fEDRate;

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

	int		m_iDropCount;
	bool	m_bWithPlayPcBang;
	int		m_iChannelCode;
	int		m_iBF_Team;

	int		m_iHenirRewardEventCount;
	bool	m_bHenirRewardUnLimited;

	int		m_iServerGroup;
};

DECL_DATA( EVENT_MONSTER_DATA )
{
	int						m_iEventID;
	int						m_iMonsterDropCount;
	bool					m_bIsAdventInBossStage;
	std::map<int, float>	m_mapNpcIDAndRate;
	std::set<int>			m_setAdventDungeon;
	std::set<int>			m_setNoAdventDungeon;
	std::set<int>			m_setNoAdventStage;
	//{{ �̺�Ʈ ���� ���� ( �Ʊ� ����	) - ��μ�
#ifdef SERV_ALLY_EVENT_MONSTER
	bool					m_bAllyNPC;
#endif SERV_ALLY_EVENT_MONSTER
	//}}

	EVENT_MONSTER_DATA()
	{
		m_iEventID				= 0;
		m_iMonsterDropCount		= 0;
		m_bIsAdventInBossStage	= false;
		//{{ �̺�Ʈ ���� ���� ( �Ʊ� ����	) - ��μ�
#ifdef SERV_ALLY_EVENT_MONSTER
		m_bAllyNPC				= false;
#endif SERV_ALLY_EVENT_MONSTER
		//}}
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
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM

DECL_DATA( KBattleFieldJoinInfo )
{
	int							m_iBattleFieldID;
	int							m_iStartPosIndex;
	std::vector< UidType >		m_vecPartyMemberList;
	bool						m_bMoveForMyParty;
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
    bool                        m_bNowBattleFieldPositionInfoStartPosition;
    unsigned short              m_usBattleFieldPositionValue;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

	KBattleFieldJoinInfo()
	{
		Initialize();
	}

	void Initialize()
	{
		m_iBattleFieldID = 0;
		m_iStartPosIndex = 0;
		m_bMoveForMyParty = false;
		m_vecPartyMemberList.clear();
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
        m_bNowBattleFieldPositionInfoStartPosition = false;
        m_usBattleFieldPositionValue = 0;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	}

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-08	// �ڼ���
	COPYCON_ASSIGNOP( KBattleFieldJoinInfo, right )
	{
		m_iBattleFieldID		= right.m_iBattleFieldID;
		m_iStartPosIndex		= right.m_iStartPosIndex;
		m_vecPartyMemberList	= right.m_vecPartyMemberList;
		m_bMoveForMyParty		= right.m_bMoveForMyParty;
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
        m_bNowBattleFieldPositionInfoStartPosition = right.m_bNowBattleFieldPositionInfoStartPosition;
        m_usBattleFieldPositionValue = right.m_usBattleFieldPositionValue;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		return *this;
	}
#endif // SERV_BATTLE_FIELD_BOSS
};

DECL_DATA( KBattleFieldRoomInfo )
{
	int											m_iBattleFieldID;
	UidType										m_RoomUID;
	char										m_MaxSlot;
	std::map< UidType, UidType >				m_mapUnitUIDPartyUID;	// key : UnitUID, value : PartyUID
	std::map< UidType, std::set< UidType > >	m_mapPartyList;			// key : PartyUID, value : Party Member UnitUID List
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-06	// �ڼ���
	byte										m_byteBossFieldState;
#endif // SERV_BATTLE_FIELD_BOSS

	KBattleFieldRoomInfo()
		: m_iBattleFieldID( 0 )
		, m_RoomUID( 0 )
		, m_MaxSlot( 0 )
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-06	// �ڼ���
		, m_byteBossFieldState( 0 )
#endif // SERV_BATTLE_FIELD_BOSS
	{
	}
};

DECL_DATA( KDangerousEventInfo )
{
	enum DANGEROUS_EVENT
	{
		DE_WARNING_MESSAGE = 0,
		DE_ELITE_MONSTER_DROP,
		DE_BOSS_MONSTER_DROP,
		//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		DE_MIDDLE_BOSS_MONSTER_DROP,
#endif SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
		DE_EVENT_BOSS_MONSTER_DROP,
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
		//}
	};

	std::set< int >			m_setEventInfo;

	void Clear()
	{
		m_setEventInfo.clear();
	}
	bool IsEventReserved( IN const DANGEROUS_EVENT eEnum )
	{
		return ( m_setEventInfo.find( eEnum ) != m_setEventInfo.end() );
	}
	void ReserveEvent( IN const DANGEROUS_EVENT eEnum )
	{
		m_setEventInfo.insert( eEnum );
	}
	void DeleteEvent( IN const DANGEROUS_EVENT eEnum )
	{
		m_setEventInfo.erase( eEnum );
	}
};

DECL_DATA( KAutoPartyMakingProcessCondition )
{
	int		m_iDungeonMode;
	int		m_iDungeonIDWithDif;
	int		m_iBeginLevel;
	int		m_iEndLevel;

	KAutoPartyMakingProcessCondition()
	{
		m_iDungeonMode = 0;
		m_iDungeonIDWithDif = 0;
		m_iBeginLevel = 0;
		m_iEndLevel = 0;
	}
};

DECL_DATA( KAutoPartyConditionInfo )
{	
	int						m_iDungeonID;
	char					m_cDifficultyLevel;
	char					m_cGetItemType;
	char					m_cDungeonMode;
	char					m_cLimitPlayerCount;

	KAutoPartyConditionInfo()
	{
		m_iDungeonID = 0;
		m_cDifficultyLevel = -1;
		m_cGetItemType = 0;
		m_cDungeonMode = 0;
		m_cLimitPlayerCount = 0;
	}
};

DECL_DATA( KAutoPartyUserInfo )
{
	UidType					m_iGSUID;
	UidType					m_iUnitUID;
	KOldPartyInfo			m_kOldPartyInfo;
	std::wstring			m_wstrNickName;
	char					m_cUnitClass;
	UCHAR					m_ucLevel;
	int						m_iAutoPartyWaitTime;
	bool					m_bAutoPartyBonus;

	KAutoPartyUserInfo()
	{
		m_iGSUID				= 0;
		m_iUnitUID				= 0;
		m_cUnitClass			= 0;
		m_ucLevel				= 0;
		m_iAutoPartyWaitTime	= 0;
		m_bAutoPartyBonus		= false;
	}
};

DECL_DATA( KRegroupPartyUserInfo )
{
	KPartyUserInfo			m_kPartyUserInfo;
	KPartyRoomUserInfo		m_kPartyRoomUserInfo;
	KOldPartyInfo			m_kOldPartyInfo;
};

DECL_DATA( KAutoPartyBonusInfo )
{
	char					m_cDungeonMode;
	int						m_iDungeonIDWithDif;
	bool					m_bActivatedBonus;

	KAutoPartyBonusInfo()
	{
		m_cDungeonMode = 0;
		m_iDungeonIDWithDif = 0;
		m_bActivatedBonus = false;
	}
};

DECL_DATA( KPartyUserJoinRoomInfo )
{
	UidType					m_iRoomUID;
	int						m_iBattleFieldID;

	KPartyUserJoinRoomInfo()
	{
		m_iRoomUID = 0;
		m_iBattleFieldID = 0;
	}
};

DECL_DATA( KDungeonGameInfo )
{	
    int						m_iDungeonID;
	char					m_cDungeonMode;
	char					m_cDifficulty;

	KDungeonGameInfo()
	{
		Clear();
	}

	void Clear()
	{
		m_iDungeonID	= 0;
		m_cDungeonMode	= 0;
		m_cDifficulty	= 0;
	}

	int GetDungeonIDWithDif() const
	{
		return ( m_iDungeonID + static_cast<int>(m_cDifficulty) );
	}
};

DECL_DATA( KDungeonGameSimpleInfo )
{
	int						m_iDungeonID;
	char					m_cDungeonMode;

	KDungeonGameSimpleInfo()
	{
		m_iDungeonID = 0;
		m_cDungeonMode = 0;
	}

	KDungeonGameSimpleInfo( const char cDungeonMode, const int iDungeonID )
	{
		m_iDungeonID = iDungeonID;
		m_cDungeonMode = cDungeonMode;
	}
};

DECL_DATA( KAutoPartyDungeonBonusInfo )
{
	int						m_iDungeonID;
	char					m_cDungeonMode;
	bool					m_bActivatedBonus;

	KAutoPartyDungeonBonusInfo()
	{
		m_iDungeonID = 0;
		m_cDungeonMode = 0;
		m_bActivatedBonus = false;
	}
};

DECL_DATA( KReturnToFieldInfo )
{
	UidType					m_iReturnToFieldUnitUID;
	UidType					m_iRoomUID;
	std::set< UidType >		m_setRemainUnitUID;

	KReturnToFieldInfo()
	{
		m_iReturnToFieldUnitUID = 0;
		m_iRoomUID = 0;
	}
};

#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2012. 04. 24	������	UDP������ �����ս� üũ
#ifdef SERV_UDP_RELAY_CHECKER
DECL_PACKET( E_CONNECT_RELAY_ACK )
{
	std::wstring		m_wstrRelayServerIP;
};

DECL_PACKET( E_UDP_RELAY_SERVER_CHECK_PACKET_NOT )
{
	std::wstring		m_wstrRelayServerIP;
	DWORD				m_dwEndTickCount;
	DWORD				m_dwEndTimeGetTime;

	KE_UDP_RELAY_SERVER_CHECK_PACKET_NOT()
	{
		m_dwEndTickCount = 0;
		m_dwEndTimeGetTime = 0;
	}
};

DECL_DATA( KUdpRelayCheckLog )
{
	UidType								m_iServerUID;
	std::wstring						m_wstrServerIP;
	DWORD								m_dwAvgDelayTime;
	DWORD								m_dwMaxDelayTime;
	std::wstring						m_wstrRegDate;

	KUdpRelayCheckLog()
	{
		m_iServerUID = 0;
		m_dwAvgDelayTime = 0;
		m_dwMaxDelayTime = 0;
	}
};
#endif SERV_UDP_RELAY_CHECKER
//}}

//{{ 2012. 05. 17	�ڼ���	Ű���� ���� ������ DB���� �а� ����
#ifdef SERV_KEYBOARD_MAPPING_INFO_RW
DECL_DATA( KKeyboardMappingInfo )
{
	std::map<short, short>	m_mapKeyboardMappingInfo;
#ifdef SERV_KEY_MAPPING_INT
	std::map<short, short>	m_mapGamePadMappingInfo;
#endif //SERV_KEY_MAPPING_INT
};
#endif SERV_KEYBOARD_MAPPING_INFO_RW
//}}

//{{ 2012. 07. 24	�ڼ���	�ش� ĳ������ ��� ��ų�� �� ��� ġƮ
#ifdef SERV_ADMIN_CHEAT_GET_ALL_SKILL
DECL_DATA( KAdminCheatSkill )
{
	int									m_iSkillLevel;
	int									m_iSkillCSPoint;
};

#ifdef SERV_SKILL_PAGE_SYSTEM
DECL_PACKET( EGS_ADMIN_CHEAT_GET_ALL_SKILL_REQ )
{
	int									m_iActiveSkillPageNumber;

	KEGS_ADMIN_CHEAT_GET_ALL_SKILL_REQ()
		: m_iActiveSkillPageNumber(0)
	{}
};
#endif // SERV_SKILL_PAGE_SYSTEM

DECL_PACKET( EGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK )
{
	int								m_iOK;
#ifdef SERV_SKILL_PAGE_SYSTEM
	int								m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM
	std::vector<short>				m_vecUnSealedSkill;
	std::map<int, KAdminCheatSkill>	m_mapSkillInfo;	// m_iSkillID, KAdminCheatSkill

#ifdef SERV_SKILL_PAGE_SYSTEM
	KEGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK()
		: m_iOK(0), m_iActiveSkillPageNumber(0)
	{
		m_vecUnSealedSkill.clear();
		m_mapSkillInfo.clear();
	}
#endif // SERV_SKILL_PAGE_SYSTEM
};
#endif SERV_ADMIN_CHEAT_GET_ALL_SKILL
//}}

//{{ 2012. 10. 31	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
DECL_DATA( KRegUnitInfo )
{
	UidType							m_iUserUID;
	UidType							m_iUnitUID;
	std::wstring					m_wstrUnitNickName;
	UidType							m_iGSUID;
	int								m_iChannelID;
	std::wstring					m_wstrIP;
	char							m_cAuthLevel;
	//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
	int								m_iChannelCode;
#endif SERV_EVENT_JACKPOT
	//}}

	KRegUnitInfo()
		: m_iUserUID( 0 )
		, m_iUnitUID( 0 )
		, m_iGSUID( 0 )
		, m_iChannelID( 0 )
		, m_cAuthLevel( 0 )
		//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
		, m_iChannelCode( 0 )
#endif SERV_EVENT_JACKPOT
		//}}
	{
	}
};
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 08. 14	�ڼ���	��õ���� ��ȭ �̺�Ʈ ���̵� ���� ���
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
DECL_PACKET( DBE_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK )
{
	int		m_iOK;
	bool	m_bReceiveTheLetter;
	
	KDBE_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK()
		: m_iOK( 0 ), m_bReceiveTheLetter( false )
	{
	}
};
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
//}}

//{{ 2012. 08. 21	�ڼ���	���� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
DECL_PACKET( EGS_GET_ITEM_FROM_LETTER_ACK )
{
	int									m_iOK;
	UidType								m_iPostNo;
	bool								m_bSystemLetter;
	std::map< int, int >				m_mapResultItem;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
	int									m_iED;
	UidType								m_iUnitUID;
	KItemQuantityUpdate					m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >	m_vecMoveItemSlotInfo;

	KEGS_GET_ITEM_FROM_LETTER_ACK()
	{
		m_iUnitUID = 0;
	}
};
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
//}}

//{{ 2012. 12. 17	�ڼ���	���� ��Ƽ �� ���� �˾� â ���� ���� ����
#ifdef SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
DECL_PACKET( EGS_PARTY_GAME_START_OPINION_CHECK_NOT )
{
	int		m_iDungeonID;		// ����
	char	m_cDifficultyLevel;	// ���̵�
	char	m_cDungeonMode;		// ��д��� ����

	KEGS_PARTY_GAME_START_OPINION_CHECK_NOT()
		: m_iDungeonID( 0 )
		, m_cDifficultyLevel( 0 )
		, m_cDungeonMode( 0 )
	{
	}
};
#endif SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2008. 11. 18  ������	������ ��ȯ
DECL_PACKET( EGS_ITEM_EXCHANGE_REQ )
{
	int									m_iHouseID;
	//{{ 2011. 08. 16	������	������ ��ȯ ����
#ifdef SERV_ITEM_EXCHANGE_NEW
	UidType								m_iSourceItemUID;
#endif SERV_ITEM_EXCHANGE_NEW
	//}}
	int									m_iSourceItemID;
	//{{ 2011. 08. 16	������	������ ��ȯ ����
#ifdef SERV_ITEM_EXCHANGE_NEW
	int									m_iSourceQuantity;
#endif SERV_ITEM_EXCHANGE_NEW
	//}}
	int									m_iDestItemID;

	KEGS_ITEM_EXCHANGE_REQ()
	{
		m_iHouseID			= 0;
		//{{ 2011. 08. 16	������	������ ��ȯ ����
#ifdef SERV_ITEM_EXCHANGE_NEW
		m_iSourceItemUID	= 0;
#endif SERV_ITEM_EXCHANGE_NEW
		//}}
		m_iSourceItemID		= 0;
		//{{ 2011. 08. 16	������	������ ��ȯ ����
#ifdef SERV_ITEM_EXCHANGE_NEW
		m_iSourceQuantity	= 0;
#endif SERV_ITEM_EXCHANGE_NEW
		//}}
		m_iDestItemID		= 0;
	}
};

DECL_PACKET( EGS_ITEM_EXCHANGE_ACK )
{
	int									m_iOK;	
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
	//{{ 2012. 03. 05	��μ�	������ȯ
#ifdef SERV_RANDOM_EXCHANGE_RESULT_VIEW
	std::map< int, int >				m_mapResultItem;
#endif SERV_RANDOM_EXCHANGE_RESULT_VIEW
	//}}

#ifdef SERV_GROW_UP_SOCKET
	int									m_iExchangeCount;
#endif SERV_GROW_UP_SOCKET

	KEGS_ITEM_EXCHANGE_ACK()
		: m_iOK( 0 )
	{
	}
};

//{{ 2013. 02. 20	������	�α� �ý��� ����
#ifdef SERV_LOG_SYSTEM_NEW
DECL_DATA( KLogStatInfo )
{
	std::wstring	m_wstrFunctionName;
	std::wstring	m_wstrFileName;
	std::wstring	m_wstrLineNum;
	int				m_iCount;

	KLogStatInfo()
	{
		m_iCount = 0;
	}
};
#endif SERV_LOG_SYSTEM_NEW
//}}

//{{ 2013. 01. 21  ��Ը� �̺�Ʈ ���� �� ���� ���� ĳ���Ϳ��� �����ϴ� �ý��� - ��μ�
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
DECL_DATA( KFirstSelectUnitReward )
{
	enum FIRST_SELECT_UNIT_REWARD
	{
		FSUR_NONE			= 0,
		FSUR_WEB_EVENT,						// �� �̺�Ʈ ����
		FSUR_SYSTEM_ERROR,					// �ý��� ���� ����
		
		FSUR_END,
	};

	enum GIVE_GROUP_TYPE
	{
		GGT_ONLY_ONE			= -1,		// ��� �׷� �� �ϳ��� �ش�.
		GGT_SOLES				= 0,		// �ַ��� ĳ���͸� �ش�
		GGT_GAIA				= 1,		// ���̾� ĳ���͸� �ش�
	};

	UidType			m_iUnitUID;			// ���� ���� ����UID
	char			m_cRewardType;		// ���� Ÿ��
	char			m_cGiveType;		// ���� �׷챺 �ִ� Ÿ��
	int				m_iItemID;			// ���� ������ ID
	int				m_iQuantity;		// ���� ������ ����
	int				m_iDBIndexID;		// DB �÷� id
	

	KFirstSelectUnitReward()
	{
		m_iUnitUID		= 0;
		m_cRewardType	= FSUR_NONE;
		m_cGiveType		= GGT_ONLY_ONE;
		m_iItemID		= 0;
		m_iQuantity		= 0;
		m_iDBIndexID	= 0;
	}

	COPYCON_ASSIGNOP( KFirstSelectUnitReward, right )
	{
		m_iUnitUID		= right.m_iUnitUID;
		m_cRewardType	= right.m_cRewardType;
		m_cGiveType	= right.m_cGiveType;
		m_iItemID		= right.m_iItemID;
		m_iQuantity		= right.m_iQuantity;
		m_iDBIndexID	= right.m_iDBIndexID;

		return *this;
	}
};
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
//}

//{{ 2013. 3. 4	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
DECL_DATA( KLocalRankingUnitInfo )
{
	UidType			m_iUnitUID;
	std::wstring	m_wstrNickName;
	u_char			m_ucLevel;
	char			m_cUnitClass;
	int				m_iPoint[2];

	KLocalRankingUnitInfo()
		: m_iUnitUID( 0 )
		, m_ucLevel( 0 )
		, m_cUnitClass( 0 )
	{
		memset( m_iPoint, NULL, sizeof( int ) * 2 );
	}

	KLocalRankingUnitInfo&	operator=( const KLocalRankingUnitInfo& rhs )
	{
		m_iUnitUID				= rhs.m_iUnitUID;
		m_wstrNickName			= rhs.m_wstrNickName;
		m_ucLevel				= rhs.m_ucLevel;
		m_cUnitClass			= rhs.m_cUnitClass;
		memcpy( m_iPoint, rhs.m_iPoint, sizeof( int ) * 2 );
		return *this;
	}

	bool	operator==( const KLocalRankingUnitInfo& rhs ) const
	{
		if( m_iUnitUID				!= rhs.m_iUnitUID			)	return false;
		if( m_wstrNickName			!= rhs.m_wstrNickName		)	return false;
		if( m_ucLevel				!= rhs.m_ucLevel			)	return false;
		if( m_cUnitClass			!= rhs.m_cUnitClass			)	return false;
		return memcmp( m_iPoint, rhs.m_iPoint, sizeof( int ) * 2 ) == 0;
	}

	void	clear( void )
	{
		m_iUnitUID				= 0;
		m_wstrNickName.clear();
		m_ucLevel				= 0;
		m_cUnitClass			= 0;
		memset( m_iPoint, NULL, sizeof( int ) * 2 );
	}
};

DECL_DATA( KLocalRankingUserInfoCounter )
{
	KLocalRankingUserInfo	m_kInfo;
	int						m_iCounter;

	KLocalRankingUserInfoCounter()
		: m_iCounter( 0 )
	{
	}
};

DECL_DATA( KLocalRankingUnitInfoCounter )
{
	UidType					m_iUserUID;
	int						m_iCounter;
	KLocalRankingUnitInfo	m_kInfo;

	KLocalRankingUnitInfoCounter()
		: m_iUserUID( 0 )
		, m_iCounter( 0 )
	{
	}
};

DECL_DATA( KLocalRankingRankerInfo )
{
	int						m_iRank;
	KLocalRankingUserInfo	m_kUserInfo;
	KLocalRankingUnitInfo	m_kUnitInfo;

	KLocalRankingRankerInfo()
		: m_iRank( 0 )
	{
	}
	
	KLocalRankingRankerInfo&	operator=( const KLocalRankingRankerInfo& rhs )
	{
		m_iRank		= rhs.m_iRank;
		m_kUserInfo	= rhs.m_kUserInfo;
		m_kUnitInfo	= rhs.m_kUnitInfo;
		return *this;
	}
};

DECL_DATA( KLocalRankingInquiryData )
{
	UINT					m_nViewPage;
	char					m_cMainTabIndex;
	char					m_cSubTabIndex;
	byte					m_byteFilter;
	KLocalRankingRankerInfo	m_kMyRankInfo;
	std::set<UidType>		m_setFriendList;
	int						m_iGuildUID;

	KLocalRankingInquiryData()
		: m_nViewPage( 0 )
		, m_cMainTabIndex( -1 )
		, m_cSubTabIndex( -1 )
		, m_byteFilter( 0 )
		, m_iGuildUID( 0 )
	{
	}
};

DECL_PACKET( EGS_LOCAL_RANKING_RANK_UP_NOT )
{
	int						m_iMyRank;
	char					m_cMainTabIndex;
	char					m_cSubTabIndex;
	KLocalRankingUnitInfo	m_kMyUnitInfo;
	KLocalRankingUnitInfo	m_kNextUnitInfo;

	KEGS_LOCAL_RANKING_RANK_UP_NOT()
		: m_iMyRank( 0 )
		, m_cMainTabIndex( -1 )
		, m_cSubTabIndex( -1 )
	{
	}
};
#endif SERV_LOCAL_RANKING_SYSTEM
//}}

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
DECL_DATA( KVillageLagInfo )
{
	int m_iLagType;
	int m_iPartyType;
	int m_iPartyUserNum;
	int m_iMapID;
	int m_iVillageUserNum;
	float m_fLagTime;
	float m_fPlayTime;
	int m_iLagTotalNum;
	float m_fLagTotalTime;

	KVillageLagInfo()
	{
		m_iLagType = -1;
		m_iPartyType = -1;
		m_iPartyUserNum = -1;
		m_iMapID = -1;
		m_iVillageUserNum = -1;
		m_fLagTime = -1.0f;
		m_fPlayTime = -1.0f;
		m_iLagTotalNum = -1;
		m_fLagTotalTime = -1.0f;
	}
};

DECL_DATA( KFieldLagInfo )
{
	int m_iLagType;
	int m_iPartyType;
	int m_iPartyUserNum;
	int m_iMapID;
	int m_iFieldUserNum;
	float m_fLagTime;
	float m_fPlayTime;
	int m_iLagTotalNum;
	float m_fLagTotalTime;

	KFieldLagInfo()
	{
		m_iLagType = -1;
		m_iPartyType = -1;
		m_iPartyUserNum = -1;
		m_iMapID = -1;
		m_iFieldUserNum = -1;
		m_fLagTime = -1.0f;
		m_fPlayTime = -1.0f;
		m_iLagTotalNum = -1;
		m_fLagTotalTime = -1.0f;
	}
};

DECL_DATA( KDungeonLagInfo )
{
	int m_iLagType;
	int m_iPartyType;
	int m_iPartyUserNum;
	int m_iMapID;
	int m_iDifficulty;
	int m_iStage;
	int m_iBlock;
	float m_fLagTime;
	float m_fPlayTime;
	int m_iLagTotalNum;
	float m_fLagTotalTime;

	KDungeonLagInfo()
	{
		m_iLagType = -1;
		m_iPartyType = -1;
		m_iPartyUserNum = -1;
		m_iMapID = -1;
		m_iDifficulty = -1;
		m_iStage = -1;
		m_iBlock = -1;
		m_fLagTime = -1.0f;
		m_fPlayTime = -1.0f;
		m_iLagTotalNum = -1;
		m_fLagTotalTime = -1.0f;
	}
};

DECL_DATA( KPvPLagInfo )
{
	int m_iLagType;
	int m_iMapID;
	int m_iPvPUserNum;
	int m_iRuleType;
	int m_iRound;
	float m_fLagTime;
	float m_fPlayTime;
	int m_iLagTotalNum;
	float m_fLagTotalTime;

	KPvPLagInfo()
	{
		m_iLagType = -1;
		m_iMapID = -1;
		m_iPvPUserNum = -1;
		m_iRuleType = -1;
		m_iRound = -1;
		m_fLagTime = -1.0f;
		m_fPlayTime = -1.0f;
		m_iLagTotalNum = -1;
		m_fLagTotalTime = -1.0f;
	}
};
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK

//{{ 2013. 06. 04	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
DECL_DATA( KStatRelationLevel )
{
	float			m_fBaseHPRelLV;
	float			m_fAtkPhysicRelLV;
	float			m_fAtkMagicRelLV;
	float			m_fDefPhysicRelLV;
	float			m_fDefMagicRelLV;

	KStatRelationLevel()
	{
		Init();
	}

	void Init()
	{
		m_fBaseHPRelLV		= 0.f;
		m_fAtkPhysicRelLV	= 0.f;
		m_fAtkMagicRelLV	= 0.f;
		m_fDefPhysicRelLV	= 0.f;
		m_fDefMagicRelLV	= 0.f;
	}

	inline bool IsValideCheck() const
	{
		return 0.f != (m_fBaseHPRelLV + m_fAtkPhysicRelLV + m_fAtkMagicRelLV + m_fDefPhysicRelLV + m_fDefMagicRelLV);
	}

	void AddStat( const KStatRelationLevel& sStatRelLVData_ )
	{
		this->m_fBaseHPRelLV += sStatRelLVData_.m_fBaseHPRelLV;
		this->m_fAtkPhysicRelLV += sStatRelLVData_.m_fAtkPhysicRelLV;
		this->m_fAtkMagicRelLV += sStatRelLVData_.m_fAtkMagicRelLV;
		this->m_fDefPhysicRelLV += sStatRelLVData_.m_fDefPhysicRelLV;
		this->m_fDefMagicRelLV += sStatRelLVData_.m_fDefMagicRelLV;
	}

	bool operator < ( const KStatRelationLevel& rhs ) const 
	{
		if( m_fBaseHPRelLV < rhs.m_fBaseHPRelLV )
			return true;
		else if( m_fBaseHPRelLV > rhs.m_fBaseHPRelLV )
			return false;

		if( m_fAtkPhysicRelLV < rhs.m_fAtkPhysicRelLV )
			return true;
		else if( m_fAtkPhysicRelLV > rhs.m_fAtkPhysicRelLV )
			return false;

		if( m_fAtkMagicRelLV < rhs.m_fAtkMagicRelLV )
			return true;
		else if( m_fAtkMagicRelLV > rhs.m_fAtkMagicRelLV )
			return false;

		if( m_fDefPhysicRelLV < rhs.m_fDefPhysicRelLV )
			return true;
		else if( m_fDefPhysicRelLV > rhs.m_fDefPhysicRelLV )
			return false;

		if( m_fDefMagicRelLV < rhs.m_fDefMagicRelLV )
			return true;

		return false;
	}
};
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}

#ifdef	SERV_HERO_PVP_MANAGE_LIST // ���볯¥: 2013-04-04
DECL_DATA( KHeroPvpUserInfo )
{
	UidType			m_iServerGroupID;
	std::wstring	m_wstrNickName;
	bool			m_bOn;
	bool			m_bConnect;

	KHeroPvpUserInfo()
		: m_iServerGroupID( -1 )
		, m_bOn( false )
		, m_bConnect( false )
	{
	}
};
#endif	// SERV_HERO_PVP_MANAGE_LIST

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
DECL_DATA( KGetSkillInfo )
{
	int				m_iSkillID;
	int				m_iBeforeSkillLevel;
	int				m_iSkillLevel;
	int				m_iSpendSkillCSPoint;

	KGetSkillInfo()
	{
		m_iSkillID				= 0;
		m_iBeforeSkillLevel		= 0;
		m_iSkillLevel			= 0;
		m_iSpendSkillCSPoint	= 0;
	}
};
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_NEXON_COUPON_SYSTEM// �۾���¥: 2013-06-18	// �ڼ���
DECL_DATA( KCouponBoxTargetItem )
{
	int				m_iItemQuantity;
	short			m_sItemUseDuration;
	std::wstring	m_wstrItemCode;
	std::wstring	m_wstrItemName;

	KCouponBoxTargetItem()
		: m_iItemQuantity( 0 )
		, m_sItemUseDuration( 0 )
	{
	}
};

DECL_DATA( KClientCouponBox )
{
	std::wstring	m_wstrContractDetailName;
	int				m_iContractDetailNo;
	std::wstring	m_wstrCouponBoxType;
	std::wstring	m_wstrCouponBoxTypeText;
	int				m_iCouponCardNo;
	short			m_iCouponCardStatus;
	std::wstring	m_wstrCouponCardStatusText;
	std::wstring	m_wstrDiscountValue;
	std::wstring	m_wstrExpireDateTime;
	std::wstring	m_wstrObtainDateTime;
	std::wstring	m_wstrValidDateRange;
	std::wstring	m_wstrUsedDateTime;
	std::vector<KCouponBoxTargetItem>	m_vecTargetItem;

	KClientCouponBox()
		: m_iContractDetailNo( 0 )
		, m_iCouponCardNo( 0 )
		, m_iCouponCardStatus( 0 )
	{
	}
};

DECL_DATA( KDiscountCoupon )
{
	int				m_iCouponCardNo;
	std::wstring	m_wstrContractDetailName;
	std::wstring	m_wstrDiscountValue;
	std::wstring	m_wstrExpireDateTime;

	KDiscountCoupon()
		: m_iCouponCardNo( 0 )
	{
	}
};

DECL_DATA( KDiscountCouponInquriyInfo )
{
	unsigned long	m_ulProductNo;
	unsigned short	m_usOrderQuantity;

	KDiscountCouponInquriyInfo()
		: m_ulProductNo( 0 )
		, m_usOrderQuantity( 0 )
	{
	}
};
#endif // SERV_NEXON_COUPON_SYSTEM

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-08	// �ڼ���
DECL_DATA( KBossFieldCreateInfo )
{
	// ���� �ʵ�� ���� �ð� ���� ������ �����ٴ� ���� �Ͽ� ������.

	__time64_t	m_tFieldHoldingTime;
	__time64_t	m_tPortalOpenTime;
	int			m_iBossFieldID;
	int			m_iPortalMapID;			// ��Ż�� ������ ��Ʋ �ʵ� ID
	bool		m_bBossField;

	KBossFieldCreateInfo()
	{
		clear();
	}

	void	clear( void )
	{
		m_tFieldHoldingTime	= 0;
		m_tPortalOpenTime	= 0;
		m_iBossFieldID		= 0;
		m_iPortalMapID		= 0;
		m_bBossField		= false;
	}
};

DECL_DATA( KBossFieldJoinInfo )
{
	int				m_iReturnMapID;			// ���� �ִ� �ʵ�� ���ư��� ����.
	unsigned char 	m_ucLastTouchLineIndex;
	unsigned short 	m_usLastPosValue;
	bool			m_bIgnoreLastTouch;		// m_iMapID�� �ش��ϴ� �⺻ ��ġ�� ����ϵ��� �˸���.

	KBossFieldJoinInfo()
	{
		clear();
	}

	COPYCON_ASSIGNOP( KBossFieldJoinInfo, right )
	{
		m_iReturnMapID			= right.m_iReturnMapID;
		m_ucLastTouchLineIndex	= right.m_ucLastTouchLineIndex;
		m_usLastPosValue		= right.m_usLastPosValue;
		m_bIgnoreLastTouch		= right.m_bIgnoreLastTouch;
		
		return *this;
	}

	void	clear( void )
	{
		m_iReturnMapID			= 0;
		m_ucLastTouchLineIndex	= 0;
		m_usLastPosValue		= 0;
		m_bIgnoreLastTouch		= false;
	}
};

DECL_PACKET( EGS_ADMIN_BOSS_FIELD_GATE_OPEN_REQ )
{
	int		m_iBattleFieldID;

	KEGS_ADMIN_BOSS_FIELD_GATE_OPEN_REQ()
		: m_iBattleFieldID( 0 )
	{
	}
};

typedef	KPacketOK	KEGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK;

DECL_PACKET( EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ )
{
	int		m_iBattleFieldID;

	KEGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ()
		: m_iBattleFieldID( 0 )
	{
	}
};

DECL_PACKET( EGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK )
{
	int		m_iOK;
	int		m_iBattleFieldID;
	byte	m_byteTotalDangerousValue;

	KEGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK()
		: m_iOK( 0 )
		, m_iBattleFieldID( 0 )
		, m_byteTotalDangerousValue( 0 )
	{
	}
};

DECL_PACKET( EGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ )
{
	int		m_iBattleFieldID;
	int		m_iTotalDangerousValue;
	bool	m_bSet;

	KEGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ()
		: m_iBattleFieldID( 0 )
		, m_iTotalDangerousValue( 0 )
		, m_bSet( true )
	{
	}
};

typedef	KPacketOK	KEGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK;

typedef	KPacketOK	KEGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK;

DECL_DATA( KBossFieldLog )
{
	int				m_iBossFieldID;
	int				m_iPortalMapID;			// ��Ż�� ������ ��Ʋ �ʵ� ID
	int				m_iPlayTime;
	byte			m_byteRemainUserCount;
	bool			m_bSucceed;
	std::wstring	m_wstrPortalOpenTime;

	KBossFieldLog()
	{
		clear();
	}

	void	clear( void )
	{
		m_iBossFieldID			= 0;
		m_iPortalMapID			= 0;			// ��Ż�� ������ ��Ʋ �ʵ� ID
		m_iPlayTime				= 0;
		m_byteRemainUserCount	= 0;
		m_bSucceed				= false;
		m_wstrPortalOpenTime.clear();
	}
};

DECL_DATA( KBossFieldUserLog )
{
	UidType			m_iUnitUID;
	__int64			m_iGivenDamage;
	__int64			m_iAttackDamage;
	int				m_iEXP;
	int				m_iED;
	short			m_sNumResurrectionStone;
	byte			m_byteLevel;
	byte			m_byteClass;
	byte			m_byteCompletionType;
	byte			m_byteContributionRank;
	std::wstring	m_wstrNickName;

	KBossFieldUserLog()
		: m_iUnitUID( 0 )
		, m_iGivenDamage( 0 )
		, m_iAttackDamage( 0 )
		, m_iEXP( 0 )
		, m_iED( 0 )
		, m_sNumResurrectionStone( 0 )
		, m_byteLevel( 0 )
		, m_byteClass( 0 )
		, m_byteCompletionType( 0 )
		, m_byteContributionRank( 0 )
	{
	}
};

DECL_PACKET( EGS_BOSS_FIELD_LOG_NOT )
{
	std::map<UidType, __int64>	m_mapGivenDamage;
	std::map<UidType, __int64>	m_mapAttackDamage;
};
#endif // SERV_BATTLE_FIELD_BOSS


#ifdef SERV_4TH_ANNIVERSARY_EVENT
DECL_DATA( K4ThAnnivEventInfo )
{
	__int64 m_tTimeFirstPlay;			// 	ù ���ҵ� �÷��� �� ��
	__int64 m_tTimeFirstPet;			// 	ù ���� ����� ��
	__int64 m_tTimeFirstHenir;		// 	��ϸ��� �ð� ù �÷��� �� ��
	__int64 m_tTimeFirstDeleteChar;	// 	ù ĳ���� ������
	int m_iItemIDFirstBuy;			// 	ù ���� ������
	int m_iLongestConnectTime;	// 	���� ���� �ð�( 1 = 1��)
	int m_iCountQuestComplete;		// 	����Ʈ ���ݱ��� �Ϸ��� Ƚ��
	int m_iCountReceivedPost;		// 	���� ���� Ƚ��
	int m_iDayTotalConnect;			// 	���� �� ���� �� ��
	int	m_iCountPvpLose;			// 	���� �� �й� Ƚ��
	int m_iCountResurrect;			// 	�� ���� ��Ȱ Ƚ��
	int m_iCountDungeonClear;		// 	�� ���� Ŭ���� Ƚ��

	K4ThAnnivEventInfo()
	{
		m_tTimeFirstPlay = 0;
		m_tTimeFirstPet = 0;
		m_tTimeFirstHenir = 0;
		m_tTimeFirstDeleteChar = 0;
		m_iItemIDFirstBuy = 0;
		m_iLongestConnectTime = 0;
		m_iCountQuestComplete = 0;		
		m_iCountReceivedPost = 0;		
		m_iDayTotalConnect = 0;			
		m_iCountPvpLose = 0;			
		m_iCountResurrect = 0;			
		m_iCountDungeonClear = 0;		
	}
};
#endif SERV_4TH_ANNIVERSARY_EVENT

#pragma pack( pop )

//{{ 2013.02.27 ��ȿ��	�ؿ� �⺻ ���� �۾� (SERV_GLOBAL_BASE)
#   include "OnlyGlobal/Packet/CommonPacket_Global.h"
//}}