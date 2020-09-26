#pragma once

#include "XSLStat.h"


class CXSLInventory;
class CXSLUnit
{
public:
	enum UNIT_TYPE
	{
		UT_NONE	= 0,
		UT_ELSWORD,
		UT_ARME,
		UT_LIRE,
		UT_RAVEN,
		UT_EVE,
		UT_CHUNG,		// kimhc // 2010-12-23 �� �߰��� ��ĳ���� û
		UT_ARA,			/// JHKang / �� ĳ���� �ƶ�
#ifdef SERV_NEW_CHARACTER_EL // ���� ������ ���� �� ����
		UT_ELESIS,
#endif // SERV_NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
		UT_ADD,
#endif //SERV_9TH_NEW_CHARACTER
		UT_END,			//  �����ý��� �߰��Ǹ� END�� �����ý� �Ʒ���..
	};

	enum UNIT_CLASS
	{
		//�ʱ�����
		UC_NONE = 0,
		UC_ELSWORD_SWORDMAN,
		UC_ARME_VIOLET_MAGE,
		UC_LIRE_ELVEN_RANGER,
		UC_RAVEN_FIGHTER,
		UC_EVE_NASOD,
		UC_CHUNG_IRON_CANNON,
		UC_ARA_MARTIAL_ARTIST,		/// 7 �ƶ� ������, ������
#ifdef SERV_NEW_CHARACTER_EL // ���� ������ ���� �� ����
		UC_ELESIS_KNIGHT = 8,
#endif // SERV_NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
		UC_ADD_NASOD_RULER = 9,
#endif //SERV_9TH_NEW_CHARACTER


		//��������
		UC_ELSWORD_KNIGHT = 10,
		UC_ELSWORD_MAGIC_KNIGHT,
		UC_LIRE_COMBAT_RANGER,
		UC_LIRE_SNIPING_RANGER,
		UC_ARME_HIGH_MAGICIAN,
		UC_ARME_DARK_MAGICIAN,
		UC_RAVEN_SOUL_TAKER,
		UC_RAVEN_OVER_TAKER,
		UC_EVE_EXOTIC_GEAR,
		UC_EVE_ARCHITECTURE,
		UC_CHUNG_FURY_GUARDIAN,		// 20	// kimhc // û 1�� ����
		UC_CHUNG_SHOOTING_GUARDIAN,	// 21	// kimhc // û 1�� ����
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
		UC_ELSWORD_SHEATH_KNIGHT,	// 22
#endif SERV_ELSWORD_SHEATH_KNIGHT
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		UC_ARME_BATTLE_MAGICIAN,	// 23
#endif SERV_ADD_ARME_BATTLE_MAGICIAN
#ifdef SERV_TRAPPING_RANGER_TEST
		UC_LIRE_TRAPPING_RANGER			= 24,	// 24	���� Ʈ���� ������
#endif SERV_TRAPPING_RANGER_TEST
#ifdef SERV_RAVEN_WEAPON_TAKER
		UC_RAVEN_WEAPON_TAKER			= 25,
#endif SERV_RAVEN_WEAPON_TAKER
#ifdef SERV_EVE_ELECTRA
		UC_EVE_ELECTRA					= 26,
#endif SERV_EVE_ELECTRA
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
		UC_CHUNG_SHELLING_GUARDIAN		= 27,
#endif
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
		UC_ARA_LITTLE_HSIEN				= 28,	// �Ҽ� - �ƶ� 1�� ����
#endif //SERV_ARA_CHANGE_CLASS_FIRST

#ifdef SERV_NEW_CHARACTER_EL // ���� ������ ���� �� ����
		UC_ELESIS_SABER_KNIGHT			= 29,	// �����ý� 1-1�� ���̹� ����Ʈ
		UC_ELESIS_PYRO_KNIGHT			= 30,	// �����ý� 1-2�� ���̷� ����Ʈ
#endif // SERV_NEW_CHARACTER_EL

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
		UC_ARA_LITTLE_DEVIL				= 31,	// �Ҹ� - �ƶ� 1�� ����
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
		UC_ADD_PSYCHIC_TRACER			= 32,	// ����ű Ʈ���̼� - �ֵ� 1�� ����
#endif //SERV_9TH_NEW_CHARACTER



		// 2�� ����
		UC_ELSWORD_LORD_KNIGHT		= 100,	
		UC_ELSWORD_RUNE_SLAYER,		// 101
		UC_LIRE_WIND_SNEAKER,		// 102
		UC_LIRE_GRAND_ARCHER,		// 103
		UC_ARME_ELEMENTAL_MASTER,	// 104
		UC_ARME_VOID_PRINCESS,		// 105
		UC_RAVEN_BLADE_MASTER,		// 106
		UC_RAVEN_RECKLESS_FIST,		// 107
		UC_EVE_CODE_NEMESIS,		// 108
		UC_EVE_CODE_EMPRESS,		// 109
		UC_CHUNG_IRON_PALADIN,		// 110
		UC_CHUNG_DEADLY_CHASER,		// 111

#ifdef SERV_ELSWORD_INFINITY_SWORD
		UC_ELSWORD_INFINITY_SWORD,	// 112
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
		UC_ARME_DIMENSION_WITCH,	// 113
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
		UC_LIRE_NIGHT_WATCHER,		// 114
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
		UC_RAVEN_VETERAN_COMMANDER,	// 115
#endif

#ifdef SERV_EVE_BATTLE_SERAPH
		UC_EVE_BATTLE_SERAPH,		// 116
#endif

#ifdef SERV_CHUNG_TACTICAL_TROOPER
		UC_CHUNG_TACTICAL_TROOPER,	// 117
#endif

#ifdef SERV_ARA_CHANGE_CLASS_FIRST
		UC_ARA_SAKRA_DEVANAM		= 118,	// ��õ - �ƶ� 2�� ����
#endif //SERV_ARA_CHANGE_CLASS_FIRST

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
		UC_ARA_YAMA_RAJA			= 119,	// ��� - �ƶ� 2�� ����
#endif // SERV_ARA_CHANGE_CLASS_SECOND


#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	// ������
		UC_ELESIS_GRAND_MASTER		= 120,	// ������ // 2013-09-04 // �����ý� 1-2 �׷��� ������
		UC_ELESIS_BLAZING_HEART		= 121,	// ������ // 2013-09-04 // �����ý� 2-2 ����¡ ��Ʈ
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE

#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
		UC_ADD_LUNATIC_PSYKER		= 122,	// �糪ƽ ����Ŀ - �ֵ� 2�� ����
#endif //SERV_ADD_LUNATIC_PSYKER


		//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	#ifndef SERV_ELSWORD_INFINITY_SWORD
		UC_ELSWORD_SHEATH_KNIGHT_2,   // ���ҵ� �ý� 2�� ���߿� name ���� �ʿ�
	#endif //SERV_ELSWORD_INFINITY_SWORD
	#ifndef SERV_ARME_DIMENSION_WITCH
		UC_ARME_BATTLE_MAGICIAN_2,    // ���̻� ��Ʋ 2�� ���߿� name ���� �ʿ�
	#endif //SERV_ARME_DIMENSION_WITCH
	#ifndef SERV_RENA_NIGHT_WATCHER
		UC_LIRE_TRAPPING_RANGER_2,    // ���� Ʈ���� 2�� ���߿� name ���� �ʿ�
	#endif //SERV_RENA_NIGHT_WATCHER
	#ifndef SERV_RAVEN_VETERAN_COMMANDER
		UC_RAVEN_WEAPON_TAKER_2,	  // ���̺� 2�� ���߿� name ���� �ʿ�
	#endif //SERV_RAVEN_VETERAN_COMMANDER
	#ifndef SERV_EVE_BATTLE_SERAPH
		UC_EVE_ELECTRA_2,			// �̺� 2�� ���߿� name ���� �ʿ�
	#endif
	#ifndef SERV_CHUNG_TACTICAL_TROOPER
		UC_CHUNG_SHELLING_GUARDIAN_2, // û 2�� ���߿� name ���� �ʿ�
	#endif
		UC_MAX,		// ������ ��
#endif SERV_UNIT_CLASS_CHANGE_ITEM
		//}}
	};

	//{{ �̺�Ʈ ���� ���� ( �Ʊ� ����	) - ��μ�
#ifdef SERV_ALLY_EVENT_MONSTER
	enum NPC_AI_TYPE
	{
		NAT_NORMAL,			// 
		NAT_ALLY,			// ��ȯ�� ģ�� NPC
		NAT_NO_BRAIN,
	};
#endif SERV_ALLY_EVENT_MONSTER
	//}}

	enum EQIP_POSITION
	{
		EP_NONE		= 0,
		EP_QUICK_SLOT,		//������ ������

		//����
		EP_WEAPON_HAND,			//����
		EP_WEAPON_TEMP1,		//�ӽ�1
		EP_WEAPON_TEMP2,		//�ӽ�2
		EP_WEAPON_TEMP3,		//�ӽ�3

		//��
		EP_DEFENCE_HAIR,		//��Ÿ��
		EP_DEFENCE_FACE,		//��
		EP_DEFENCE_BODY,		//����
		EP_DEFENCE_LEG,			//����
		EP_DEFENCE_HAND,		//�尩
		EP_DEFENCE_FOOT,		//�Ź�
		EP_DEFENCE_TEMP1,		//�ӽ�1
		EP_DEFENCE_TEMP2,		//�ӽ�2
		EP_DEFENCE_TEMP3,		//�ӽ�3

		//�׼�����
		EP_AC_TITLE,			//Īȣ
		EP_AC_HAIR,				//���
		EP_AC_FACE1,			//��(��)
		EP_AC_FACE2,			//��(��)
		EP_AC_FACE3,			//��(��)
		EP_AC_BODY,				//����
		EP_AC_LEG,				//�ٸ�
		EP_AC_ARM,				//��
		EP_AC_RING,				//����
		EP_AC_NECKLESS,			//�����
		EP_AC_WEAPON,			//���� �Ǽ��縮
		EP_AC_TEMP2,			//�ӽ�2
		EP_AC_TEMP3,			//�ӽ�3
		EP_AC_TEMP4,			//�ӽ�4
		EP_AC_TEMP5,			//�ӽ�5

		//�ʻ��
		EP_SKILL_1,				//1�ܰ� �ʻ��
		EP_SKILL_2,				//2�ܰ� �ʻ��
		EP_SKILL_3,				//3�ܰ� �ʻ��
		EP_SKILL_TEMP1,			//�ӽ�1
		EP_SKILL_TEMP2,			//�ӽ�2
		EP_SKILL_TEMP3,			//�ӽ�3

		EP_RAVEN_LEFT_ARM,		// dmlee 2008.07.31 - ���̺� ����, ������ �����鼭	
		EP_WEAPON_SECOND,		

#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
		EP_ONEPIECE_FASHION,	// ���ǽ� �ƹ�Ÿ
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT

		EP_END,
	};

	enum NOW_EQIP_SLOT_ID
	{
		NESI_DEFENCE_HAIR_FASHION = 0,		

		NESI_DEFENCE_BODY,				
		NESI_DEFENCE_BODY_FASHION,		

		NESI_DEFENCE_LEG,				
		NESI_DEFENCE_LEG_FASHION,

		NESI_DEFENCE_HAND,				
		NESI_DEFENCE_HAND_FASHION,	

		NESI_DEFENCE_FOOT,				
		NESI_DEFENCE_FOOT_FASHION,		

		NESI_WEAPON_HAND,				
		NESI_WEAPON_HAND_FASHION,		

		NESI_AC_FACE1,				//��(��)
		NESI_AC_FACE2,				//��(��)
		NESI_AC_FACE3,				//��(��)
		NESI_AC_BODY,				//����
		NESI_AC_LEG,				//�ٸ�
		NESI_AC_ARM,				//��
		NESI_AC_RING,				//����
		NESI_AC_NECKLESS,			//�����
		NESI_AC_WEAPON,				//���� �Ǽ��縮

#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
		NESI_ONEPIECE_FASHION,		//���ǽ� �ƹ�Ÿ
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT

		NESI_END,
	};

	enum NOW_SKILL_SLOT_ID
	{
		NSSI_SKILL_1 = 0,			//1�ܰ� �ʻ��
		NSSI_SKILL_2,				//2�ܰ� �ʻ��
		NSSI_SKILL_3,				//3�ܰ� �ʻ��

		NSSI_END,
	};

	enum NOW_QUICK_SLOT_ID
	{
		NQSI_QUICK_SLOT_1 = 0,
		NQSI_QUICK_SLOT_2,
		NQSI_QUICK_SLOT_3,
		NQSI_QUICK_SLOT_4,
		NQSI_QUICK_SLOT_5,
		NQSI_QUICK_SLOT_6,

		NQSI_END,
	};

	enum CONNECT_UNIT_STATE
	{
		CUS_NONE		= 0,
		CUS_FIELD_MAP,
		CUS_MARKET,
		CUS_PVP_LOBBY,
		CUS_PVP_WAIT,
		CUS_PVP_PLAY,
		CUS_TC_PLAY,
		CUS_DUNGEON_PLAY,
	};

	//{{ 2011. 06. 20	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		//{{ 2012. 06. 19	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	enum PVP_EMBLEM
	{
		PE_NONE			= -1,
		PE_RANK_E		= 0,
		PE_RANK_D		= 251,
		PE_RANK_C		= 551,
		PE_RANK_B		= 951,
		PE_RANK_A		= 1351,
		PE_RANK_S		= 1851,
		PE_RANK_SS		= 2351,
		PE_RANK_SSS		= 2951,
		PE_END			= 3300,
	};

	enum PVP_RANK
	{
		PVPRANK_NONE = 0,
		PVPRANK_RANK_ARRANGE,
		PVPRANK_RANK_E,
		PVPRANK_RANK_D,
		PVPRANK_RANK_C,
		PVPRANK_RANK_B,
		PVPRANK_RANK_A,
		PVPRANK_RANK_S,
		PVPRANK_RANK_SS,
		PVPRANK_RANK_SSS,

		PVPRANK_END,
	};
#else
	enum PVP_EMBLEM
	{
		PE_NONE			= -1,
		PE_RANK_F		= 0,
		PE_RANK_E		= 251,
		PE_RANK_D		= 501,
		PE_RANK_C		= 1001,
		PE_RANK_B		= 1501,
		PE_RANK_A		= 2001,
		PE_RANK_S		= 2501,
		PE_RANK_SS		= 2701,
		PE_END			= 100000000,
	};

	enum PVP_RANK
	{
		PVPRANK_NONE = 0,
		PVPRANK_RANK_F,
		PVPRANK_RANK_E,
		PVPRANK_RANK_D,
		PVPRANK_RANK_C,
		PVPRANK_RANK_B,
		PVPRANK_RANK_A,
		PVPRANK_RANK_S,
		PVPRANK_RANK_SS,

		PVPRANK_END,
	};
#endif SERV_2012_PVP_SEASON2
		//}}
#else
	enum PVP_EMBLEM
	{
		PE_NONE			= -1,
		PE_RECRUIT		=         0,		//�Ʒú�
		PE_GUARDS		=      1500,		//�����
		PE_MERCENARY	=     10000,		//�뺴��
		PE_KNIGHTS		=    125000,		//����
		PE_COMMANDER	=    615000,		//�屺
		PE_EMPEROR		=   1835000,		//Ȳ��
		PE_HERO			=   4300000,		//����
		PE_GOTOWAR		=   9500000,		//����
		PE_END			= 100000000,		//100000(�����ƽø�)
	};

	enum PVP_EMBLEM_ENUM
	{
		PEE_NONE	= -1,
		PEE_RECRUIT = 0,	//�Ʒú�
		PEE_GUARDS,			//�����
		PEE_MERCENARY,		//�뺴��
		PEE_KNIGHTS,		//����
		PEE_COMMANDER,		//�屺
		PEE_EMPEROR,		//Ȳ��
		PEE_HERO,			//����
		PEE_GOTOWAR,		//����
		PEE_END,			//100000(�����ƽø�)
	};
#endif SERV_PVP_NEW_SYSTEM
	//}}

	//{{ 2008. 12. 14  ������	ĳ���� ����
	enum CHAR_SLOT
	{
#if defined( SERV_COUNTRY_TWHK )
		CHAR_SLOT_MAX = 11,
#elif defined (SERV_COUNTRY_EU)
		CHAR_SLOT_MAX = 9,
#elif defined (SERV_COUNTRY_US)
		CHAR_SLOT_MAX = 10,
#elif defined (SERV_COUNTRY_JP)
		CHAR_SLOT_MAX = 12,
#elif defined (SERV_COUNTRY_CN)
		CHAR_SLOT_MAX = 10,
#elif defined (SERV_COUNTRY_TH)
		CHAR_SLOT_MAX = 8,
#elif defined (SERV_COUNTRY_ID)
		CHAR_SLOT_MAX = 6,
#elif defined (SERV_COUNTRY_BR)
		CHAR_SLOT_MAX = 12,
#elif defined (SERV_COUNTRY_PH)
		CHAR_SLOT_MAX = 6,
#elif defined (SERV_COUNTRY_IN)
		CHAR_SLOT_MAX = 6,
#else
		CHAR_SLOT_MAX = 10, // 20313.06.04 lygan_������ // �ִ�ġ 30���� ���°� ���� �ȵ� Ȥ�ó� �� ���縦 ���� 10���� ��������
		//CHAR_SLOT_MAX = 30,
#endif 
		
	};
	//}}

	//{{ 2011. 07. 08    ��μ�    �ɼ� ��ġȭ
//#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
	enum UNIT_PERCENT_HP
	{
		UPHP_INIT				= 100,    // ���� 1�� 1 ���� ��
		UPHP_INCREASE_ONE_POINT = 15,     // ������ �����Ǵ� ���� ���а�
	};
//#endif SERV_USE_PERCENT_IN_OPTION_DATA
	//}} 

	//{{ 2011. 05. 16  ��μ�	Īȣ ȹ�� ���� �߰�
//#ifdef SERV_ADD_TITLE_CONDITION
	enum USER_UNIT_DIE_REASON			// ������ ���� ����
	{
		UUDR_EMPTY_OXYGEN = 1,			// ��� �������� ���� ���	
	};
//#endif SERV_ADD_TITLE_CONDITION
	//}}

//#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
	enum UNIT_CLASS_TYPE
	{
		UCT_NONE			= -1,
		UCT_BASIC_CLASS		= 0,	// �⺻ ����
		UCT_FIRST_CLASS		= 1,	// 1�� ����
		UCT_SECOND_CLASS	= 2,	// 2�� ����
		UCT_END				= 3,
	};
//#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

	struct UnitTemplet
	{
		UNIT_TYPE			m_UnitType;
		UNIT_CLASS			m_UnitClass;

		std::wstring		m_Name;
		std::wstring		m_Description;

		std::wstring		m_MotionFile;

		CXSLStat::Stat		m_Stat;

		int					m_BasicWeaponItemID;
		int					m_BasicHairItemID;
		int					m_BasicFaceItemID;
		int					m_BasicBodyItemID;
		int					m_BasicLegItemID;
		int					m_BasicHandItemID;
		int					m_BasicFootItemID;

		int					m_RavenLeftArmItemID;	// dmlee 2008.07.31 - ���̺� ����, ������ �����鼭
		int					m_SecondWeaponItemID;
		UnitTemplet()
		{
			m_UnitType				= UT_NONE;
			m_UnitClass				= UC_NONE;

			m_BasicWeaponItemID		= 0;
			m_BasicHairItemID		= 0;
			m_BasicFaceItemID		= 0;
			m_BasicBodyItemID		= 0;
			m_BasicLegItemID		= 0;
			m_BasicHandItemID		= 0;
			m_BasicFootItemID		= 0;

			m_RavenLeftArmItemID	= 0;
			m_SecondWeaponItemID	= 0;
		}
	};

	struct UnitData
	{
		UidType					m_UnitUID;
		UidType					m_UserUID;
		UNIT_CLASS				m_UnitClass;

		std::wstring			m_IP;
		int						m_Port;

		std::wstring			m_NickName;

		int						m_ED;
		//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
		int						m_Rating;
		int						m_RPoint;
		int						m_APoint;
#else
		int						m_VSPoint;
#endif SERV_PVP_NEW_SYSTEM
		//}}		
		int						m_Level;
		int						m_EXP;

		int						m_Win;
		int						m_Lose;
		int						m_Seceder;

		int						m_NowBaseLevelEXP;
		int						m_NextBaseLevelEXP;

		int						m_EquipInventorySize;		
		int						m_SkillInventorySize;		
		int						m_MaterialInventorySize;	
		int						m_SpecialInventorySize;	
		int						m_CardInventorySize;		
		int						m_QuickSlotInventorySize;	
		int						m_AvartaInventorySize;	

		int						m_QuickSlotSize;

		CXSLStat::Stat 			m_Stat;
		int						m_RemainStatPoint;

		std::vector<UidType>	m_NowEqipItemUIDList;
		CXSLInventory*			m_pInventory;

		int						m_nStraightVictories;
		int						m_nMapID;

		bool					m_bIsGameBang;

		UnitData& operator=( const KUnitInfo& data );

		UnitData();
		~UnitData();
	};

public:
	CXSLUnit(void);
	~CXSLUnit(void);
	
	//{{ 2012. 09. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
	static PVP_RANK			ComputeNewPVPRank( IN PVP_RANK eCurrentPvpRank, IN const int iNowRating );
#else
	//{{ 2011. 07. 11	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	static bool				IsValidPvpEmblem( const PVP_EMBLEM eEmblem ) { return ( PvpEmblemToPvpRank( eEmblem ) != PVPRANK_NONE ); }
	static PVP_EMBLEM		GetPVPEmblem( const int iRating );
	static PVP_RANK			PvpEmblemToPvpRank( const PVP_EMBLEM eEmblem );
	static PVP_EMBLEM		PvpRankToPvpEmblem( const PVP_RANK eEmblemEnum );
#else
	static PVP_EMBLEM		GetPVPEmblem( const int iMaxVP );
	static PVP_EMBLEM_ENUM	PvpEmblemToPvpEmblemEnum( const PVP_EMBLEM eEmblem );
	static PVP_EMBLEM		PvpEmblemEnumToPvpEmblem( const PVP_EMBLEM_ENUM eEmblemEnum );
#endif SERV_PVP_NEW_SYSTEM
	//}}
#endif SERV_2012_PVP_SEASON2
	//}}

	static bool				IsValidUnitClass( UNIT_CLASS eUnitClass ) { return ( GetUnitClassToUnitType( eUnitClass ) != UT_NONE ); }
	static UNIT_CLASS		GetUnitClassDownGrade( UNIT_CLASS eUnitClass );
	static UNIT_CLASS		GetUnitClassBaseGrade( UNIT_CLASS eUnitClass );
	static UNIT_CLASS		GetUnitClassUpGrade( UNIT_CLASS eUnitClass );
	static UNIT_TYPE		GetUnitClassToUnitType( UNIT_CLASS eUnitClass );
	//}}

	//{{ 2009. 11. 10  ������	2������
	static bool				IsFirstChangeJob( CXSLUnit::UNIT_CLASS unitClass );
	static bool				IsSecondChangeJob( CXSLUnit::UNIT_CLASS unitClass );
	//}}

	//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	static bool				IsInitNormalJob( CXSLUnit::UNIT_CLASS unitClass );					// �ʱ� �����ΰ�?
	static bool				IsUnBalanceUnitClass( IN char cUnitType, IN char cUnitClass, IN char cItemUnitClass, OUT int& iNewUnitClass ); 
#endif SERV_UNIT_CLASS_CHANGE_ITEM
	//}}

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	static bool				IsMan_UnitType( UNIT_TYPE eUnitType );
#endif SERV_RELATIONSHIP_SYSTEM
	//}
};


