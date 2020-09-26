#pragma once

#include "RefreshSingleton.h"
#include "X2Data/XSLRoom.h"

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
#include "Lottery.h"
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

class CXSLPetManager
{
	DeclareRefreshSingleton( CXSLPetManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	enum PET_UNIT_ID
	{
		PUI_NONE = 0,
		PUI_TEST,
		PUI_PETTE_PPORU,
		PUI_PETTE_ANCIENT_PPORU,
		PUI_TREE_KNIGHT,
		PUI_PETTE_PPORU_EVENT,
		PUI_PETTE_ANCIENT_PPORU_F,
		PUI_TREE_KNIGHT_F,
		PUI_HOLLO_WITCH,
		PUI_HOLLO_WITCH_F,
		PUI_XMAS2010_PPORU,
		PUI_MYSTIC_NORM,
		PUI_MYSTIC_BLK,
		PUI_MYSTIC_WHT,
		PUI_MYSTIC_RED,
		PUI_HATCHLING_NORM,
		PUI_HATCHLING_DARK,
		PUI_HATCHLING_FIRE,
		PUI_HATCHLING_LIGHT,
		PUI_PETIT_ANCIENT_HALLOW_DARK,
		PUI_PETIT_ANCIENT_HALLOW_FLAME,
		PUI_PETIT_ANCIENT_HALLOW_SPOOK,
		PUI_PETIT_ANCIENT_HALLOW_SPRITE,
		PUI_PETIT_PPORU_BOOMHILL,
		PUI_TREE_KNIGHT_XMAS2011,
		PUI_TREE_KNIGHT_SANTA2011,
		PUI_PETIT_LIGHTBRINGER_UNIEL,
		PUI_PETIT_DARKBRINGER_UNIRING,
		PUI_PET_SILVERFOX_MIHO,			/// ������ ��ȣ
		PUI_PET_FIREFOX_SHIHO,			/// �ҿ��� ��ȣ
		PUI_PET_DARKFOX_NAHO,			/// ��ҿ��� ��ȣ
		PUI_PET_BLOOD_EATER,			/// ���� ����
		PUI_PET_MANA_EATER,				/// ���� ����
		PUI_PET_MILLENNIAFOX_EUN,		/// õ�⿩�� ��

		PUI_PETIT_PPORU_HATS_ON,	/// �ڶ� �Ƿ� ������
		PUI_STINKO_HATS_ON,			/// ������ ������
		PUI_EBONY_HATS_ON,			/// ������ ������

		PUI_PPORU_HOLLOWEEN_EVENT,		/// �ҷ��� �̺�Ʈ �� ���� �Ƿ�

		PUI_GRIM_REAPER_DEATH				= 38, //�׸� ����-����(��� ��)
		PUI_GRIM_REAPER_REBIRTH				= 39, //�׸� ����-������(��� ��)
		PUI_RAINCOAT_CAT_BLACK_KR           = 40, /// ��� ���� ����� ��
		PUI_RAINCOAT_CAT_YELLOW_KR          = 41, /// ��� ���� ����� ���ο�
		PUI_PPORU_NAVER_EVENT				= 42, /// ���̹� ä�θ� �̺�Ʈ �Ƿ�

		// �ؿ��� pet
		PUI_PANDA_STICK_RED					= 96,
		PUI_PANDA_STICK_BLUE				= 97,
		PUI_PANDA_STICK_RED_F				= 98,
		PUI_PANDA_STICK_BLUE_F				= 99,
		PUI_PETTE_ANCIENT_PPORU_SHADOW		= 100, 
		PUI_GAMANIA_PETTE_PPORU				= 101,
		PUI_PETTE_ANCIENT_PPORU_SHADOW_F	= 102,
		PUI_TREE_KNIGHT_SPRING				= 110,
		PUI_TREE_KNIGHT_FALL				= 111,
		PUI_TREE_KNIGHT_WINTER				= 112,
		PUI_TREE_KNIGHT_SPRING_F			= 113,
		PUI_TREE_KNIGHT_FALL_F				= 114,
		PUI_TREE_KNIGHT_WINTER_F			= 115,
		PUI_HATCHLING_FIRE_F				= 116,
		PUI_HATCHLING_LIGHT_F				= 117,
		PUI_HATCHLING_STAR_F				= 118,
		PUI_HATCHLING_NORM_F				= 119,
		PUI_HATCHLING_DARK_F				= 120,
		PUI_CHINAJOY_PPORU_EVENT			= 121,
		PUI_HATCHLING_BLUE					= 122,
		PUI_DEATH_SCYTHE					= 123,
		PUI_DEATH_SCYTHE_F					= 124,
		PUI_PANDA_ROYAL_RED					= 125,
		PUI_PANDA_ROYAL_WHITE				= 126,
		PUI_MYSTIC_NORM_F					= 127,
		PUI_MYSTIC_BLK_F					= 128,
		PUI_MYSTIC_WHT_F					= 129,
		PUI_MYSTIC_RED_F					= 130,	
		PUI_MYSTIC_BLUE                     = 131,  /// �̽�ƽ ���
		PUI_MYSTIC_BLUE_F                   = 132,	/// �̽�ƽ ���(����ü)
		PUI_PETIT_LIGHTBRINGER_UNIEL_F      = 133,	/// ���Ͽ� ����ü ��
		PUI_SHERIFF_NORM                    = 134,	/// �̱� ���Ȱ� �� ( ���� )
		PUI_SHERIFF_WHITE                   = 135,	/// �̱� ���Ȱ� �� ( ��� )
		PUI_SHERIFF_NORM_F                  = 136,	/// �̱� ���Ȱ� �� ( ���� ����ü )
		PUI_SHERIFF_WHITE_F                 = 137,	/// �̱� ���Ȱ� �� ( ��� ����ü )
		PUI_PETIT_DARKBRINGER_UNIRING_F		= 138,	/// ���ϸ�(����ü)
		PUI_PET_BLOOD_EATER_EVENT			= 139,	/// �븸 �̺�Ʈ�� ���� ����
		PUI_HATCHLING_BLUE_F                = 140,	///	���� ���� CBT��� �߻� ������ ����ü
		PUI_PETTE_BERTHE					= 141,  /// ������ Jr.		
		PUI_PETTE_BERTHE_F					= 142,  /// ������ Jr. ����ü

		PUI_PET_SILVERFOX_MIHO_F            = 30000, /// ������ ��ȣ ����ü
		PUI_PET_FIREFOX_SHIHO_F             = 30001, /// �ҿ��� ��ȣ ����ü
		PUI_PET_DARKFOX_NAHO_F              = 30002, /// ��ҿ��� ��ȣ ����ü
		PUI_PET_MANA_EATER_EVENT_INT        = 30003, /// �븸 ���� ����
		PUI_PET_MERCENARY_PPORU_EVENT_INT   = 30004, /// �̱� �뺴 �Ƿ�
		PUI_RAINCOAT_CAT_BLACK              = 30005, /// ��� ���� ����� ��
		PUI_RAINCOAT_CAT_YELLOW             = 30006, /// ��� ���� ����� ���ο�
		PUI_RAINCOAT_CAT_BLACK_F            = 30007, /// ��� ���� ����� �� ����ü
		PUI_RAINCOAT_CAT_YELLOW_F           = 30008, /// ��� ���� ����� ���ο� ����ü
		PUI_EVENT_EAT_FOOD_PPORU_F          = 30009, /// �±� �ε��׽þ� �ڶ� �Ƿ�
		PUI_HOLLO_WITCH_EVENT_F				= 30010, /// �̺�Ʈ�� �ҷ���ġ ����ü
		PUI_PETTE_PPORU_EVENT_BRAZIL		= 30011, /// �̺�Ʈ�� ����� �Ƿ�
		PUI_HALLOWEEN_PUMPKIN_TANYA			= 30012, /// �ҷ��� ȣ�� ���� Ÿ��
		PUI_HALLOWEEN_PUMPKIN_LAEL			= 30013, /// �ҷ��� ȣ�� ���� ��
		PUI_HALLOWEEN_PUMPKIN_LOJETA		= 30014, /// �ҷ��� ȣ�� ���� ����Ÿ
		PUI_HALLOWEEN_PUMPKIN_NARENEA		= 30015, /// �ҷ��� ȣ�� ���� �����׾�
		PUI_HALLOWEEN_PUMPKIN_ELDENA		= 30016, /// �ҷ��� ȣ�� ���� ������ (����)
		PUI_HALLOWEEN_PUMPKIN_TANYA_F		= 30017, /// �ҷ��� ȣ�� ���� Ÿ�� ����ü
		PUI_HALLOWEEN_PUMPKIN_LAEL_F		= 30018, /// �ҷ��� ȣ�� ���� �� ����ü
		PUI_HALLOWEEN_PUMPKIN_LOJETA_F		= 30019, /// �ҷ��� ȣ�� ���� ����Ÿ ����ü
		PUI_HALLOWEEN_PUMPKIN_NARENEA_F		= 30020, /// �ҷ��� ȣ�� ���� �����׾� ����ü
		PUI_HALLOWEEN_PUMPKIN_ELDENA_F		= 30021, /// �ҷ��� ȣ�� ���� ������ (����) ����ü
		PUI_SONOKONG						= 30022, /// �տ���
		PUI_SONOKONG_F						= 30023, /// �տ��� ����ü
		PUI_FIREWORK_PPORU					= 30024, /// ���� �̺�Ʈ �Ҳ� �Ƿ�
		PUI_PET_SNOWBUMP					= 30025, /// �����
		PUI_PET_HUNTER_PENGUIN				= 30026, /// �ر�
		PUI_TREE_KNIGHT_EVENT_F				= 30027, /// Ʈ�� ����Ʈ ����ü �Ⱓ�� ( 7�� )
		PUI_HATCHLING_NORM_EVENT_F			= 30028, /// ������ ����ü 15��
		PUI_MYSTIC_EVENT_NORM_F				= 30029, /// �̽�ƽ ����ü 15��

	};

	//{{ 2010. 7. 26  ������	�� �ý���
	enum PET_ACTION_COMMAND
	{
		PAC_NONE = 0,

		PAC_DONOTKNOW,	// 1. ����
		PAC_HUNGRY,		// 2. �����	- 100%
		PAC_HELLO,		// 3. �λ�
		PAC_SIT,		// 4. �ɾ�!
		PAC_SIT_END,	// 5. �Ͼ!	- 100%
		PAC_CUTE,		// 6. �ֱ�!  
		PAC_AFRAID,		// 7. ������!
		PAC_SLEEP,		// 8. ����!
		PAC_SLEEP_END,	// 9. ��!		- 100%
		PAC_HAPPY,		// 10. ������!
		PAC_EAT,		// 11. �Ա�		- 100%
		PAC_STUFFED,	// 12. ��θ�	- 100%
		PAC_FORCE_SLEEP,// 13. �������� - 100%
		PAC_SPECIAL,	// 14. �����!

		PAC_END,
	}; 
	//}}

	enum PET_ENUM
	{
		PE_MAX_SATIETY		= 4800,

		PE_MIN_EXTROVERSION = -4800,
		PE_MAX_EXTROVERSION	= 4800,

		PE_MIN_EMOTION		= -4800,
		PE_MAX_EMOTION		= 4800,

		PE_PET_FEED_ITEM_LEVEL = 11,
	};

	enum AURA_SKILL_ENUM
	{
		ASE_OPTION_1 = 0,
		ASE_OPTION_2,
		ASE_OPTION_3,

		ASE_MAX,
	};

	struct PetTemplet
	{
		PET_UNIT_ID					m_PetId;
		std::wstring				m_wstrCommonName;				// ��ǥ�̸�
		float						m_caprice;						// ����
		float						m_appetite;						// �Ļ緮
		int							m_Evolution_All_Step;			// ��ȭ�ܰ�
		//{{ 2010. 10. 27	������	�� ���� ��ų
#ifdef SERV_PET_AURA_SKILL
		std::vector< int >			m_PetStatus;					// ��ȭ�ܰ���°�
#endif SERV_PET_AURA_SKILL
		//}}
		std::vector< int >			m_InvenSlot_Width_Size;			// �κ��丮 ���� ����
		std::vector< int >			m_InvenSlot_Height_Size;		// �κ��丮 ���� ����

		std::vector< float >		m_Evolution_Step_Max;			// �� ��ȭ���� max ģ�е�
		std::vector< std::wstring >	m_Evolution_Step_ScriptName;	// �� ��ȭ���¿� ���� ��ũ��Ʈ �̸�
		//{{ 2010. 10. 18	������	�� ���� ��ų
#ifdef SERV_PET_AURA_SKILL		
		//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
		std::vector< int >		m_vecAuraSkillOption[ASE_MAX];	// ��ų ���Ͽɼ�
#else
		std::vector< short >		m_vecAuraSkillOption[ASE_MAX];	// ��ų ���Ͽɼ�
#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 
#endif SERV_PET_AURA_SKILL
		//}}
#ifdef SERV_FREE_AUTO_LOOTING
		bool						m_bFreeAutoLooting;
#endif SERV_FREE_AUTO_LOOTING
	};

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-20
	enum RIDING_PET_ID
	{
		RPI_NONE	= 0,
		RPI_TEMP1	= 1,
		RPI_TEMP2	= 2,
		RPI_TEMP3	= 3,
		RPI_MAX,
	};

	enum RIDING_PET_ENUM
	{
		RPE_MAX_PET_COUNT	= 30,
		RPE_PAGE_PER_COUNT	= 6,
	};
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
	struct TransformPetInfo
	{
		int							m_iPetCreateItemID;
		PET_UNIT_ID					m_PetId;
		float						m_fRate;
	};
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

	typedef std::pair< int, int >	KCashPetFeedKey;

public:
	CXSLPetManager(void);
	~CXSLPetManager(void);

	// for lua
	void	SetNewPetInfo_LUA( char cEvolutionStep, short sSatiety, int iIntimacy, short sExtroversion, short sEmotion );
	bool	AddPetTemplet_LUA();
#ifdef SERV_PERIOD_PET
	bool	AddPetCreateItemInfo_LUA( int iItemID, int iPetID, short sPeriod );
#else SERV_PERIOD_PET
	bool	AddPetCreateItemInfo_LUA( int iItemID, int iPetID );
#endif SERV_PERIOD_PET
	bool	AddPetCashFeedItemInfo_LUA( int iItemID, short sIncreaseSatiety );
	bool	AddSpecialFeedItemID_LUA( int iItemID );
	//{{ 2011. 04. 25	������	�� ������ �Ҹ� ����
#ifdef SERV_PET_SATIETY_NEW
	bool	AddPetSatietyDecreaseFactor_LUA( int iEvolutionStep, double fFactor );
#endif SERV_PET_SATIETY_NEW
	//}}

	// function
	bool	IsExistPetID( IN int iPetID ) const			{ return ( GetPetTemplet( iPetID ) != NULL ); }
	bool	IsSpecialFeedItemID( IN int iItemID ) const { return ( m_setSpecialFeedItemID.find( iItemID ) != m_setSpecialFeedItemID.end() ); }
	const PetTemplet* GetPetTemplet( IN int iPetID ) const;
	int		GetPetInventorySlotSize( IN int iPetID, IN char cEvolutionStep ) const;
#ifdef SERV_PERIOD_PET
	void	GetPetIDByItemID( IN const int iItemID, OUT PET_UNIT_ID& ePetID, OUT short& sPeriod );
	bool	HasPetInventory( IN int iPetID ) const;
#else SERV_PERIOD_PET
	PET_UNIT_ID GetPetIDByItemID( IN int iItemID );
#endif SERV_PERIOD_PET
	bool	GetCashPetFeedItemInfo( IN int iItemID, OUT short& sIncreaseSatiety );
	bool	GetEvolutionStepMaxIntimacy( IN int iPetID, IN char cEvolutionStep, OUT int& iMaxIntimacy ) const;
	bool	CheckNextEvolution( IN int iPetID, IN char cCurrentEvolutionStep, OUT char& cNextEvolutionStep ) const;
	bool	CalcIncreaseSatietyForFeed( IN int iPetID, IN int iFeedItemID, IN char cLevel, OUT short& sIncreaseSatiety ) const;
	bool	CheckExtroversion( IN int iPetID, IN short sCurExtroversion, IN int iCumulativeIntimacy, OUT short& sChangeExtroversion ) const;
	bool	CheckEmotion( IN int iPetID, IN short sCurEmotion, IN int iCumulativeSatiety, IN int iFeedCount, OUT short& sChangeEmotion ) const;

	bool	MakeNewPetInfo( IN PET_UNIT_ID ePetID, IN const std::wstring& wstrPetName, OUT KPetInfo& kNewPetInfo ) const;
#ifdef SERV_EVENT_PET_INVENTORY
	bool    IsEventFoodPetID( IN int iPetID ) const;
#endif SERV_EVENT_PET_INVENTORY
	//{{ 2010. 10. 18	������	�� ���� ��ų
#ifdef SERV_PET_AURA_SKILL		
	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	bool	GetAuraSkillOption( IN PET_UNIT_ID ePetID, IN char cEvolutionStep, OUT std::vector< int >& vecAuraSkillOption );
#else
	bool	GetAuraSkillOption( IN PET_UNIT_ID ePetID, IN char cEvolutionStep, OUT std::vector< short >& vecAuraSkillOption );
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
#endif SERV_PET_AURA_SKILL
	//}}
	//{{ 2011. 04. 25	������	�� ������ �Ҹ� ����
#ifdef SERV_PET_SATIETY_NEW
	short	CalcDecreaseSatietyValueOnDungeonRoom( IN const PET_UNIT_ID ePetID, IN const char cEvolutionStep, IN const short sSubStageExpNpcNum ) const;
	short	CalcDecreaseSatietyValueOnPvpRoom( IN const PET_UNIT_ID ePetID, IN const char cEvolutionStep ) const;
#endif SERV_PET_SATIETY_NEW
	//}}
	
	//{{ 2011. 05. 23  ��μ�	ť�ǿ� ���� ģ�е� ����
#ifdef SERV_ADD_KEWPIEL_JELLY
	void	IncreaseSpecialFeedIntimacyRate_LUA( IN float fIncreaseRate );
	void	DecreaseSpecialFeedIntimacyRate_LUA( IN float fDecreaseRate );
	float	GetSpecialFeedIncreaseIntimacyRate()						   { return m_fSpecialFeedIncreaseIntimacyRate; }
	float	GetSpecialFeedDecreaseIntimacyRate()						   { return m_fSpecialFeedDecreaseIntimacyRate; }
#endif SERV_ADD_KEWPIEL_JELLY
	//}}
	//{{ 2012. 12. 26	�ڼ���	�� ���� ���� ��� �߰�
#ifdef SERV_PET_AUTO_LOOTING
	bool	IsEvolutionExceptionPet( IN const int iPetID ) const;
#endif SERV_PET_AUTO_LOOTING
	//}}

	// static	
	static bool IsValidCommand( IN PET_ACTION_COMMAND eCommand );
	static bool IsUnconditionalCommand( IN PET_ACTION_COMMAND eCommand );

	//{{ 2012. 04. 26	�ڼ���	�� �峭��
#ifdef SERV_PET_TOY
	static int CalcChangeIntimacyValue( IN CXSLRoom::ROOM_TYPE eRoomType, IN char cUserCount, IN short sSatiety, IN const bool& bHaveToy );
#else
	static int CalcChangeIntimacyValue( IN CXSLRoom::ROOM_TYPE eRoomType, IN char cUserCount, IN short sSatiety );
#endif SERV_PET_TOY
	//}}

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
	bool	AddRandomPetCreateItemInfo_LUA( int iItemID, int iPetID, float fRate );
	bool	AddTransformPetItemInfo_LUA( int iTransformID, int iPetCreateItemID, int iPetID, float fRate );
	CXSLPetManager::PET_UNIT_ID GetRandomPetIDByItemID( IN int iItemID );
	CXSLPetManager::PET_UNIT_ID GetTransformPetID( IN const int iUsedItemID, IN const int iBeforePetID );
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#ifdef SERV_FREE_AUTO_LOOTING
	bool IsFreeAutoLootingPet( IN const int iPetID_ ) const;
#endif SERV_FREE_AUTO_LOOTING

private:
	std::map< PET_UNIT_ID, PetTemplet >		m_mapPetTemplet;			// �� ������
#ifdef SERV_PERIOD_PET
	std::map< int, std::pair<PET_UNIT_ID, short> >	m_mapPetCreateItem;	// ���̵� �� ���� ������ ����Ʈ
#else SERV_PERIOD_PET
	std::map< int, PET_UNIT_ID >			m_mapPetCreateItem;			// �� ���� ������ ����Ʈ
#endif SERV_PERIOD_PET
	std::map< int, short >					m_mapPetCashFeedItem;		// �� ĳ�� ���� ������ ����Ʈ
	std::set< int >							m_setSpecialFeedItemID;		// Ư���� ������ �� ���� ����Ʈ
	KPetInfo								m_kNewPetInfo;				// �� ������ �ʱⰪ ����
	//{{ 2011. 04. 25	������	�� ������ �Ҹ� ����
#ifdef SERV_PET_SATIETY_NEW
	std::map< int, double >					m_mapPetSatietyDecreaseFactor; // �� ������ �Ҹ� ���
#endif SERV_PET_SATIETY_NEW
	//}}

	//{{ 2011. 05. 23  ��μ�	ť�ǿ� ���� ģ�е� ����
#ifdef SERV_ADD_KEWPIEL_JELLY
	float									m_fSpecialFeedIncreaseIntimacyRate;
	float									m_fSpecialFeedDecreaseIntimacyRate;
#endif SERV_ADD_KEWPIEL_JELLY
	//}}

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
	std::map< int, KLottery >				m_mapRandomPetCreateItem;
	std::map< int, TransformPetInfo >		m_mapTransformPetItem;
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
};

DefRefreshSingletonInline( CXSLPetManager );



