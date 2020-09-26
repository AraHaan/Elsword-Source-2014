#pragma once

#include "X2Data/XSLMain.h"
#include <RTTI.h>

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// �۾���¥: 2013-08-12	// �ڼ���
#include "RefreshSingleton.h"
#else // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
#include <ToString.h>
#include <KNCSingleton.h>
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2



class CXSLSkillTree
{
#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// �۾���¥: 2013-08-12	// �ڼ���
	DeclareRefreshSingleton( CXSLSkillTree );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
#else // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
	DeclareSingleton( CXSLSkillTree );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
	DeclToStringW;
	DeclDump;

public:
	enum SKILL_TYPE
	{
		ST_NONE = 0,
		ST_PASSIVE_PHYSIC_ATTACK,
		ST_PASSIVE_MAGIC_ATTACK,
		ST_PASSIVE_MAGIC_DEFENCE,
		ST_PASSIVE_PHYSIC_DEFENCE,
		ST_BUFF,
		ST_ACTIVE,
		ST_SPECIAL_ACTIVE,
		ST_PASSIVE,

		//#ifdef GUILD_SKILL
		ST_GUILD_PASSIVE_PHYSIC_ATTACK_MAGIC_DEFENCE, 
		ST_GUILD_PASSIVE_MAGIC_ATTCK_PHYSIC_DEFENCE,
		ST_GUILD_PASSIVE,
		//#endif GUILD_SKILL

		//#ifdef ADDED_RELATIONSHIP_SYSTEM
		ST_RELATIONSHIP_SKILL,
		//#endif // ADDED_RELATIONSHIP_SYSTEM
	};

	enum SKILL_ID
	{
		SI_INVALID = -1,
		SI_NONE,
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013
		// ���� �нú�
		SI_P_COMMON_PHYSIC_ATTACK_BEGINNER = 100,		/// ���� �ٷ´ܷ�
		SI_P_COMMON_MAGIC_ATTACK_BEGINNER,				/// ���� ��������
		SI_P_COMMON_PHYSIC_ATTACK_INTERMEDIATE,			/// �߱� �ٷ´ܷ�
		SI_P_COMMON_MAGIC_ATTACK_INTERMEDIATE,			/// �߱� ��������
		SI_P_COMMON_GET_CHANCE,							/// ��ȸ�� ����
		SI_P_COMMON_SUPPORT_AURA = 105,					/// �ݷ��� ���

		// ���� ����
		SI_SA_COMMON_AURA_POWER_ACCEL = 200,			/// ���� ����
		SI_SA_COMMON_AURA_MAGIC_ACCEL,					/// ������ ����
		SI_SA_COMMON_AURA_SHIELD_ACCEL,					/// �γ��� ����
		SI_SA_COMMON_AURA_SPEED_ACCEL,					/// ������ ����
		SI_SA_COMMON_AURA_EL_DEFENCE_ACCEL,				///	��ȣ�� ����
		SI_SA_COMMON_AURA_CRITICAL_ACCEL = 205,			/// �¸��� ����
		SI_SA_COMMON_AURA_ADDATK_ACCEL,					/// ������ ����

		// ���, ��ȥ
		SI_GP_COMMON_BALANCED_BODY					= 500,		/// �������� ��ü
		SI_GP_COMMON_HARMONIC_SPIRIT				= 501,		/// ��ȭ�ο� ��ȥ
		SI_GP_COMMON_CHEERING_OF_TEAM				= 502,		/// ������ ����
		SI_GP_COMMON_MASTER_OF_ALCHEMIST			= 503,		/// ���ݼ��� ����
		SI_GP_COMMON_CONCENTRATION_OF_SPIRIT		= 504,		/// ���� ����
		SI_GP_COMMON_CONCENTRATION_MAGICAL_POWER	= 505,		/// ���ߵ� ����
		SI_GP_COMMON_VIOLENT						= 506,		/// �ݳ�
		SI_GP_COMMON_LIBERATION_OF_ANGER			= 507,		/// �г� �ع�
		// ������� 2Ʈ��
		SI_GP_COMMON_INFINITE_STRENGTH				= 508,		/// ������ ü��
		SI_GP_COMMON_CHANCE_TO_REVERSE				= 509,		/// ������ ��ȸ
		SI_GP_COMMON_SACRIFICE_OF_HERO				= 510,		/// ������ ���
		SI_GP_COMMON_MIDAS							= 511,		/// ���̴����� ��

		SI_ETC_WS_COMMON_LOVE						= 900,		/// Ŀ�� ��ų õ������

		// Elsword
		SI_SA_ES_FATAL_FURY = 1000,						/// ����Ż ǻ��
		SI_SA_ES_FLAME_GEYSER,							/// �÷��� ������
		SI_SA_ES_UNLIMITED_BLADE,						/// �𸮹�Ƽ�� ���̵�
		SI_SA_ES_TRIPLE_GEYSER,							/// Ʈ���� ������
		SI_SA_ESK_SPIRAL_BLAST,							/// �����̷� ����Ʈ
		SI_SA_ESK_ARMAGEDON_BLADE = 1005,				/// �Ƹ��ٵ� ���̵�
		SI_SA_ESK_DOUBLE_SLASH,							/// ���� ������
		SI_SA_ELK_WINDMILL,								/// �����
		SI_SA_ELK_SAND_STORM,							/// ���� ����
		SI_SA_EMK_RISING_SLASH,							/// ����¡ ������
		SI_SA_EMK_RISING_WAVE = 1010,					/// ����¡ ���̺�
		SI_SA_EMK_SWORD_FIRE,							/// �ҵ� ���̾�
		SI_SA_EMK_PHOENIX_TALON,						/// �Ǵн� �ŷ�
		SI_SA_ERS_STORM_BLADE,							/// ���� ���̵�
		SI_SA_ERS_LUNA_BLADE,							/// �糪 ���̵�
		SI_SA_ETK_FINAL_STRIKE = 1015,					/// ���̳� ��Ʈ����ũ
		SI_SA_ETK_PHANTOM_SWORD,						/// ���� �ҵ�
		SI_SA_EIS_MAELSTORM_RAGE,						/// ���Ͻ��� ������
		SI_SA_EIS_SWORD_FALL,							/// �ҵ���
		SI_SA_EIS_RAGE_CUTTER,							/// ������ Ŀ��

		SI_A_ES_MEGASLASH = 1200,						/// �ް� ������
		SI_A_ES_ASSAULT_SLASH,							/// �Ʈ ������
		SI_A_ES_COUNTER,								/// �ݰ�
		SI_A_ES_ENDURANCE,								/// �ر�
		SI_A_ES_KICK,									/// ������
		SI_A_ES_ROLLING = 1205,							/// ������
		SI_A_ES_SWORD_WAVE,								/// �ҵ� ���̺�
		SI_A_ES_DEFENCE,								/// ����
		SI_A_ESK_AIR_SLASH,								/// ���� ������
		SI_A_ESK_ARMOR_BREAK,							/// �Ƹ� �극��ũ
		SI_A_ESK_WEAPON_BREAK = 1210,					/// ���� �극��ũ
		SI_A_ELK_SONIC_BLADE,							/// �Ҵ� ���̵�
		SI_A_ELK_ROLLING_SMASH,							/// �Ѹ� ���Ž�
		SI_A_ELK_IMPACT_SMASH,							/// ����Ʈ ���Ž�
		SI_A_EMK_BIG_BURST,								/// ������
		SI_A_EMK_WIND_BLADE = 1215,						/// ���� ���̵�
		SI_A_ERS_SPLASH_EXPLOSION,						/// ���÷��� �ͽ��÷���
		SI_A_ERS_SWORD_ENCHANT,							/// �ҵ� ��æƮ
		SI_A_ERS_CRITICAL_SWORD,						///	ũ��Ƽ�� �ҵ�
		SI_A_ERS_RUNE_OF_FIRE,							/// �ʿ��� ��
		SI_A_ERS_RUNE_OF_ICE = 1220,					/// �Ѻ��� ��
		SI_A_ETK_CRESCENT_CUT,							/// ũ����Ʈ ��
		SI_A_ETK_SWORD_SHIELD,							/// �ҵ� �ǵ�
		SI_A_ETK_HARSH_CHASER,							/// �Ͻ� ü�̼�
		SI_A_ETK_PIERCING_SWORD,						/// �Ǿ�� �ҵ�
		SI_A_EIS_SWORD_BLASTING = 1225,					/// �ҵ� ������
		SI_A_EIS_MIRAGE_STING,							/// �̶��� ����
		SI_A_EIS_FATAL_SLAP,							/// ����Ż ����

		SI_P_ES_POWERFUL_VITAL = 1400,					/// ������ ��ü
		SI_P_ESK_GET_MANA_UP,							/// Ȱ��
		SI_P_ESK_AUTO_GUARD,							/// ���� ����
		SI_P_ELK_STRONG_BONE,							/// �ٰ� ü��
		SI_P_ELK_REDUCE_PAIN,							/// �������
		SI_P_ELK_INTENSIVE_ATTACK = 1405,				/// ������ �ϰ�
		SI_P_ELK_INDURANCE_OF_REVENGE,					/// ������ �γ�
		SI_P_ES_POWERFUL_FIREBALL,						/// ���̾ ��ȭ
		SI_P_ERS_SPELL_CHAIN,							/// ������ �罽
		SI_P_ERS_MAGIC_RESISTANCE,						/// �������� ����
		SI_P_ERS_RUNE_MASTERY = 1410,					/// �� �����͸�
		SI_P_ETK_BRUTAL_SLAYER,							/// ��Ȥ�� �л���
		SI_P_ETK_HARSH_SLAYER,							/// ��Ȥ�� �л���
		SI_P_EIS_COMPACT_COUNTER,						/// ��ƴ ���� �ݰ�
		SI_P_EIS_MIND_OF_FIGHTER,						/// �ºλ��� ����
		SI_P_EIS_LIGHTNING_STEP = 1415,					/// ���� ��ȭ
		SI_P_EIS_CUTTING_SWORD,							/// ��ī�ο� ��

		SI_HA_ELK_GIGANTIC_SLASH = 1600,				/// �Ⱓƽ ������
		SI_HA_ERS_SHINING_RUNE_BUSTER,					/// ���̴� �������
		SI_HA_EIS_BLADE_RAIN,							/// ���̵� ����

		// Aisha
		SI_SA_AV_GUST_SCREW = 2000,						/// �Ž�Ʈ ��ũ��
		SI_SA_AV_LIGHTNING_BOLT,						/// ����Ʈ�� ��Ʈ
		SI_SA_AV_GUST_STORM,							/// �Ž�Ʈ ����
		SI_SA_AV_ICE_STORM,								/// ���̽� ����
		SI_SA_AHM_MAGIC_MISSILE,						/// ���� �̻���
		SI_SA_AHM_BLIZZARDSHOWER = 2005,				/// ���ڵ� ����
		SI_SA_AHM_CHAIN_LIGHTNING,						/// ü�� ����Ʈ��
		SI_SA_AHM_BLAZE_STEP,							/// ������ ����
		SI_SA_AEM_METEOR_SHOWER,						/// ���׿� ����
		SI_SA_AEM_LIGHTNING_SHOWER,						/// ����Ʈ�� ����
		SI_SA_ADM_DARK_CLOUD = 2010,					/// ��ũ Ŭ����
		SI_SA_ADM_PLASMA_CUTTER,						/// �ö�� Ŀ��
		SI_SA_ADM_DEATHFIELD,							/// �����ʵ�
		SI_SA_ADM_AGING,								/// ����¡
		SI_SA_AVP_PHANTOM_BREATHING_DARK_FALL,			/// ���� �긮�� - ��ũ��
		SI_SA_AVP_PHANTOM_BREATHING_DARK_HOLE = 2015,	/// ���� �긮�� - ��ũ Ȧ
		SI_SA_ABM_ENERGY_SPURT,							/// ������ ����Ʈ
		SI_SA_ABM_SUPER_NOVA,							/// ���� ���
		SI_SA_ABM_GUILLOTINE_PRESS,						/// ���ƾ ������
		SI_SA_ABM_MAGICAL_MAKEUP,						/// ������ ����ũ��
		SI_SA_ADW_SCREWDRIVER_TORNADO = 2020,			/// ��ũ�� ����̹� - ����̵�
		SI_SA_ADW_SCREWDRIVER_DRILLER,					/// ��ũ�� ����̹� - �帱��

		SI_A_AV_CHAIN_FIRE_BALL = 2200,					/// ü�� ���̾
		SI_A_AV_BINDING_CIRCLE,							/// ���ε� ��Ŭ
		SI_A_AV_TELEPORT,								/// �ڷ���Ʈ
		SI_A_AV_MANA_SHIELD,							/// ���� �ǵ�
		SI_A_AV_CIRCLE_FLAME,							/// ��Ŭ �÷���
		SI_A_AV_FIRE_ROAD = 2205,						/// ���̾� �ε�
		SI_A_AV_ICICLE_WAVE,							/// ���̽�Ŭ ���̺�
		SI_A_AHM_CYCLONE,								/// ����Ŭ��
		SI_A_AHM_MEDITATION,							/// ���
		SI_A_AEM_CHAIN_BURST,							/// ü�� ����Ʈ
		SI_A_AEM_STONE_WALL = 2210,						/// ����
		SI_A_AEM_AQUA_SHOWER,							/// ����� ����
		SI_A_AEM_SHINING_BODY,							/// ���̴� �ٵ�
		SI_A_ADM_HELL_STONE,							/// �ｺ��
		SI_A_ADM_HELL_DROP,								/// �� ���
		SI_A_ADM_MANA_INTAKE = 2215,					/// ���� ���
		SI_A_AVP_SUMMON_BAT_TIME_BOMB,					/// ���� ��ȯ - Ÿ�� ��
		SI_A_AVP_SUMMON_BAT_HEAVY_DOLL,					/// �ڶ� �Ӱ� - ��� ��
		SI_A_AVP_SHADOW_BODY,							/// ������ �ٵ�
		SI_A_ABM_VITAL_DRAIN,							/// ����Ż �巹��
		SI_A_ABM_ENERGY_DRAIN = 2220,					/// ������ �巹��
		SI_A_ADW_DISTORTION,							/// ���� �ְ�
		SI_A_ADW_ENERGETIC_BODY,						/// ������ƽ �ٵ�
		SI_A_ADW_HEAVY_PRESS,							/// ��� ������
		SI_A_ADW_IMPACT_HAMMER,							/// ����Ʈ �ظ�
		SI_A_ADW_WORM_HOLE = 2225,						/// ��Ȧ

		SI_P_AV_FIREBALL_UP = 2400,						/// ���̾ ����
		SI_P_AHM_FREEZING_POINT_RESEARCH,				/// ���� ����
		SI_P_AHM_UNLIMIT_MANA,							/// ������ ����
		SI_P_AEM_ELEMENTAL_FRIENDSHIP,					/// ���ҿ��� ģȭ
		SI_P_AEM_ENERGY_OF_THE_PLENTY,					/// ��ȭ�� ���
		SI_P_AEM_ELEMENTAL_RESEARCH = 2405,				/// ������ ����
		SI_P_ADM_SHADOW_DATH,							/// ������ �׸���
		SI_P_ADM_SHADOW_OF_DISASTER,					/// ����� �׸���
		SI_P_AVP_LIMITED_MANA_MANAGEMENT,				/// ������ ���� ���
		SI_P_AVP_STRONG_MIND,							/// ����� ���
		SI_P_AVP_SPRINTER = 2410,						/// ��������
		SI_P_AVP_MAGIC_AMPLIFICATION,					/// ���� ����
		SI_P_AVP_GRAVITY_BALL_RESEARCH,					/// �׶��Ƽ �� ����
		SI_P_ABM_FITNESS,								/// ��ü �ܷ�
		SI_P_ABM_MANAFLOW_OVERDRIVE,					/// ���� ����
		SI_P_ADW_SPIRIT_ACCELERATION  = 2415,			/// ��� ����
		SI_P_ADW_ADVANCED_TELEPORTATION,				/// ���� ����
		SI_P_ADW_MAGICAL_STAFF,							/// ������ ������

		SI_HA_AEM_ELEMENTAL_STORM = 2600,				/// ������Ż ����
		SI_HA_AVP_ABYSS_ANGOR,							/// ��� �Ӱ�
		SI_HA_ADW_FATE_SPACE,							/// ����Ʈ �����̽�

		// Lena
		SI_SA_LE_PERFECT_STORM = 3000,					/// ����Ʈ ����
		SI_SA_LE_MULTIPLE_STINGER,						/// ��Ƽ�� ���ð�
		SI_SA_LE_AERO_TORNADO,							/// ����� ����̵�
		SI_SA_LE_PHOENIX_STRIKE,						/// �Ǵн� ��Ʈ����ũ
		SI_SA_LCR_CRESCENT_KICK,						/// ũ����Ʈ ű
		SI_SA_LCR_DIVE_KICK_BOMBING = 3005,				/// ���̺� ű ����
		SI_SA_LCR_VIOLENT_ATTACK,						/// ���̿÷�Ʈ ����
		SI_SA_LWS_SHARPFALL,							/// ���� ��
		SI_SA_LWS_AIRELINNA_NYMPH,						/// ���̷�����- ����
		SI_SA_LWS_AIRELINNA_SYLPH,						/// ���̷����� - ����
		SI_SA_LSR_GUIDED_ARROW = 3010,					/// ���̵� �ַο�
		SI_SA_LSR_CRAZY_SHOT,							/// ũ������ ��
		SI_SA_LSR_WINDWARD,								/// ����͵�
		SI_SA_LSR_GUNGNIR,								/// �ñ״�
		SI_SA_LGA_AERO_STRAFE,							/// ����� ��Ʈ������
		SI_SA_LTR_FATALITY = 3015,						///	����Ż��Ƽ
		SI_SA_LTR_TRAPPING_ARROW_BURST,					///	Ʈ���� �ַο� - ����Ʈ
		SI_SA_LTR_TRAPPING_ARROW_FUNGUS,				/// Ʈ���� �ַο� - ��Ž�
		SI_SA_LTR_CALL_OF_RUIN,							///	�� ���� ����
		SI_SA_LTR_KARMA,								///	ī����
		SI_SA_LNW_GLIDING_STRIKE = 3020,				///	�۶��̵� ��Ʈ����ũ

		SI_A_LE_RAIL_STINGER = 3200,					/// ���� ���ð�
		SI_A_LE_ASSAULT_KICK,							/// �Ʈ ű
		SI_A_LE_BACKJUMP_SHOT,							/// ����� ��
		SI_A_LE_REFLEX_MAGIC,							/// �����ݻ�
		SI_A_LE_RISING_FALCON,							///	����¡ ����
		SI_A_LE_SIEGEMODE = 3205,						/// ���� ���
		SI_A_LCR_SPINNING_KICK,							/// ���Ǵ� ű
		SI_A_LCR_LOW_KICK,								/// �ο�ű
		SI_A_LCR_MIDDLE_KICK,							/// �̵�ű
		SI_A_LWS_NATURE_FORCE,							/// �ڿ��� ��
		SI_A_LWS_SLIDE_KICK = 3210,						/// �����̵� ���� ű
		SI_A_LWS_ASSAULT_IMPACT,						/// �Ʈ ����Ʈ
		SI_A_LWS_BACK_KICK,								/// ��ű
		SI_A_LWS_HIGH_KICK,								/// ����ű
		SI_A_LSR_ENTANGLE,								/// ���ʱ� - ��Ÿ���� ������
		SI_A_LSR_SHOOTING_MAGNUM = 3215,				/// ���� �ű׳�
		SI_A_LSR_HUMMING_WIND,							/// ��� ����
		SI_A_LGA_FREEZING_ARROW,						/// ����¡ �ַο�
		SI_A_LGA_FREEZING_BOLT,							/// ����¡ ��Ʈ
		SI_A_LGA_ARC_SHOT,								/// ��ũ ��
		SI_A_LGA_RAPID_SHOT = 3220,						/// ���ǵ� ��
		SI_A_LGA_WIND_BLAST,							/// ���� ����Ʈ
		SI_A_LGA_STIGMA_ARROW,							/// ������ ���
		SI_A_LTR_EVOKE,									///	�̺�ũ
		SI_A_LTR_ENTANGLE,								/// ���ʱ� - Ʈ����
		SI_A_LTR_EXPLOSION_TRAP = 3225,					///	������ ��
		SI_A_LNW_FURIOUS_ENGAGE,						///	ǻ��� �ΰ�����
		SI_A_LNW_DELAYED_FIRING,						///	������ ��ȣź
		SI_A_RNW_THORNS_TRAP,							/// ���õ��� ��
		SI_A_RNW_ROSEBUSH_TRAP,							/// �� ��
		SI_A_RNW_SEED_OF_RUIN = 3230,					/// ������ ����
		SI_A_RNW_ANGER_OF_ELF,							/// ��������� �г�

		SI_P_LCR_SHARP_KICK = 3400,						/// ������ ������
		SI_P_LCR_AGILE_MOVEMENT,						/// ����� ���
		SI_P_LCR_SOFTBODY,								/// �����Ѹ��
		SI_P_LWS_POWERFUL_SHOT,							/// �� �ִ� ���
		SI_P_LWS_FRIENDSHIP_OF_NATURE,					/// �ڿ����� ģȭ
		SI_P_LSR_POWERFUL_BOWSTRING = 3405,				/// ��ȭ�� ���콺Ʈ��
		SI_P_LSR_HAWK_EYE,								/// ȣũ����
		SI_P_LSR_SHARP_ARROW,							/// �ٵ���� ȭ��
		SI_P_LGA_COMMUNE_OF_NATURE,						/// �ڿ��� ����
		SI_P_LGA_VIBRATION_SHOOTING,					/// ���� ���
		SI_P_LTR_HUNTERS_ABILITY = 3410,				///	��ɲ��� ���
		SI_P_LTR_SHARPEN_ARROW,							///	��ī�ο� ȭ��
		SI_P_LNW_VITALPOINT_PIERCING,					///	�޼� ���
		SI_P_LNW_PRIOR_PLANNED_BLOW,					///	���� �ϰ�

		SI_HA_LWS_SPIRAL_STRIKE = 3600,					/// �����̷� ��Ʈ����ũ
		SI_HA_LGA_CRYOTRON_BOLT,						/// ũ���ƿ�Ʈ�� ��Ʈ
		SI_HA_LNW_INNOCENT,								/// �̳뼾Ʈ

		// Raven
		SI_SA_RF_POWER_ASSAULT = 4000,					/// �Ŀ� �Ʈ
		SI_SA_RF_CANNON_BLADE,							/// ĳ�� ���̵�
		SI_SA_RF_SEVEN_BURST,							/// ���� ����Ʈ
		SI_SA_RST_HYPER_SONIC_STAB,						/// ������ �Ҵн���
		SI_SA_RST_SHOCKWAVE,							/// ��ũ ���̺� - ��
		SI_SA_RST_SHOCKWAVE_FRONT = 4005,				/// ��ũ ���̺� - ��
		SI_SA_RST_BLOODY_ACCEL,							/// ���� �׼�
		SI_SA_RBM_FLYING_IMPACT,						/// �ö��� ����Ʈ
		SI_SA_RBM_GIGA_DRIVE_LIMITER,					/// �Ⱑ ����̺�- ������
		SI_SA_RBM_GIGA_DRIVE_SEISMIC_TREMOR,			/// �Ⱑ ����̺� - ������� Ʈ����
		SI_SA_ROT_ARC_ENEMY = 4010,						/// ��ũ �ֳʹ�
		SI_SA_ROT_GUARDIAN_STRIKE,						/// ����� ��Ʈ����ũ
		SI_SA_ROT_VALKYRIESJAVELIN,						/// ��Ű���� �ں���
		SI_SA_ROT_NUCLEAR,								/// ��Ŭ����
		SI_SA_RRF_WILD_CHARGE,							/// ���ϵ� ����
		SI_SA_RWT_BURSTING_BLADE = 4015,				/// ������ ���̵�
		SI_SA_RWT_REVOLVER_CANNON_ORDNANCE_PENETRATOR,	/// ������ ĳ�� - OPź
		SI_SA_RWT_REVOLVER_CANNON_HIGH_EXPLOSIVE_SHELL,	/// ������ ĳ�� - HEź
		SI_SA_RWT_HARPOON_SPEAR,						/// ��Ǭ ���Ǿ�
		SI_SA_RWT_HELLFIRE_GATLING,						/// �����̾� ��Ʋ��
		SI_SA_RWT_GIGA_PROMINENCE = 4020,				/// �Ⱑ ���ι̳ͽ�
		SI_SA_RVC_DEADLY_RAID,							/// ���鸮 ���̵�
		SI_SA_RVC_IGNITION_CROW_NAPALM,					/// �̱״ϼ� ũ�ο� - ������
		SI_SA_RVC_IGNITION_CROW_INCINERATION,			/// �̱״ϼ� ũ�ο� - �νóʷ��̼�

		SI_A_RF_MAXIMUM_CANNON = 4200,					/// �ƽø� ĳ��
		SI_A_RF_EARTH_BREAKER,							/// � �극��ũ
		SI_A_RF_MEGADRILL_BREAK,						/// �ް� �帱 �극��ũ
		SI_A_RF_BURNING_RUSH,							/// ���׷���
		SI_A_RF_GROUND_IMPACT,							/// �׶��� ����Ʈ
		SI_A_RF_SHADOW_STEP = 4205,						/// �����콺��
		SI_A_RF_BREAKING_FIST,							/// �극��ŷ �ǽ�Ʈ
		SI_A_RST_BERSERKER_BLADE,						/// ����Ŀ ���̵�
		SI_A_RST_CUT_TENDON,							/// ���ٴ�
		SI_A_RBM_ONE_FLASH,								/// �ϼ�
		SI_A_RBM_SONIC_SLASH = 4210,					/// �Ҵ� ������
		SI_A_RBM_WOFL_FANG,								/// ������
		SI_A_ROT_CHARGED_BOLT_HEDGHOG,					/// ������ ��Ʈ - ����ȣ��
		SI_A_ROT_CHARGED_BOLT_BLOOD,					/// ������ ��Ʈ - ���� ����
		SI_A_RRF_HELL_DIVE,								/// �� ���̺�
		SI_A_ROT_WEAPON_BREAK = 4215,					/// ���� ����
		SI_A_ROT_ARMOR_BREAK,							/// �� ����
		SI_A_RRF_X_CRASH,								/// ���� ũ����
		SI_A_RRF_LIMIT_CRUSHER,							/// ����Ʈ ũ����
		SI_A_RWT_BARRAGE_ATTACK,						/// ���� ����
		SI_A_RWT_FLAME_SWORD = 4220,					/// ȭ����
		SI_A_RVC_BLEEDING_SLICER,						/// ġ���
		SI_A_RVC_NAPALM_GRENADE,						/// ������ �׷����̵�

		SI_P_RF_SHADOW_PIERCING = 4400,					/// �������Ǿ��
		SI_P_RF_HARDEN_BODY,							/// �ܷõȽ�ü
		SI_P_RF_INDUCE_DESIRE_WINNING,					/// ȣ�½�����
		SI_P_RST_BLADE_MASTERY,							/// ���õ� �˼�
		SI_P_RST_EXQUISITE_SWORDMANSHIP,				/// ������ �˼�
		SI_P_RBM_REVERSAL_SOLDIER = 4405,				/// �����ǿ��
		SI_P_RBM_SHADOW_THRUST,							/// ������ ������Ʈ
		SI_P_RBM_EMERGENCY_ESCAPE,						/// ��� Ż��
		SI_P_RRF_BURNING_NASOD_HAND,					/// Ÿ������ ���ҵ� �ڵ�
		SI_P_RRF_SPIRITUALIZED_FURY,					/// ��ȭ�� �г�
		SI_P_RRF_EXPLOSIVE_NASOD_HAND = 4410,			/// �����ϴ� ���ҵ� �ڵ�
		SI_P_RRF_SHADOW_PUNISHER,						/// ������ �۴ϼ�
		SI_P_RRF_CONFIDENCE_VICTORY,					/// �¸����ڽŰ�		
		SI_P_RWT_BEHAVIOR_OF_MERCENARY,					/// �뺴�� ó�ż�
		SI_P_RVC_SHADOW_BACK_SLIDE,						/// ������ �� �����̵�
		SI_P_RVC_SURVIVAL_TECHNIQUE_OF_MERCENARY = 4415,/// �뺴�� ���� ����
		SI_P_RVC_WINNABLE_TECHNIQUE_OF_MERCENARY,		/// �뺴�� �¸�����
		SI_P_RVC_AIR_SLICER,							/// ���� �����̼�
		SI_P_RVC_OVERHEAT_MODE,							/// ������� �۵�!

		SI_HA_RBM_EXTREM_BLADE = 4600,					/// �ͽ�Ʈ�� ���̵�
		SI_HA_RRF_INFERNAL_ARM,							/// ���丣�� ����
		SI_HA_RVC_BURNING_BUSTER,						/// ���� ������

		// Eve
		SI_SA_EN_ILLUSION_STINGER = 5000,				/// �Ϸ��� ���þ�
		SI_SA_EN_DIMENSION_LINK_BLADER,					/// ���� ��ũ - ���̴�
		SI_SA_EN_GENERATE_BLACKHOLE,					/// ���׷����� ��Ȧ
		SI_SA_EN_DIMENSION_LINK_GUARDIAN,				/// ���� ��ũ - �����
		SI_SA_EEG_EXPLOSION_IMPACT,						/// �ͽ��÷��� ����Ʈ
		SI_SA_EEG_QUEENS_THRONE = 5005,					/// ���� ����
		SI_SA_EEG_JUNK_BREAK,							/// ��ũ �극��ũ
		SI_SA_ENS_IRONSCRAPS,							/// ���̾� ��ũ����
		SI_SA_ENS_ATOMIC_SHIELD,						/// ����� ����
		SI_SA_ENS_ASSUALT_SPEAR_ANNIHILATOR,			/// �Ʈ ���Ǿ� - ��̾�����
		SI_SA_EAT_SONIC_WAVE = 5010,					/// �Ҵ� ���̺�
		SI_SA_EAT_GENOCIDE_RIPPER,						/// ������̵� ����
		SI_SA_EAT_HEAVENS_FIST_PRESSURE,				/// �غ��� �ǽ�Ʈ - ������
		SI_SA_EAT_HEAVENS_FIST_SWEEPER,					/// ����� �ǽ�Ʈ - ������
		SI_SA_EEP_SPACE_WRENCH,							/// ������ ����ó
		SI_SA_EEP_ASSUALT_SPEAR_BURST = 5015,			/// �Ʈ ���Ǿ� - ������
		SI_SA_EEL_PARTICLE_RAY,							/// ��ƼŬ ����
		SI_SA_EEL_SWEEP_ROLLING_OVER_CHARGE,			/// ������ �Ѹ� - ��������
		SI_SA_EEL_SWEEP_ROLLING_TRI_VULCAN,				/// ������ �Ѹ� - Ʈ���� ��ĭ
		SI_SA_EEL_PHOTON_FLARE,							/// ���� �÷���
		SI_SA_EEL_GIGA_STREAM = 5020,					/// �Ⱑ ��Ʈ��
		SI_SA_EEL_THOUSANDS_OF_STARS,					/// ������� ��Ÿ
		SI_SA_EBS_ENERGY_NEEDLES,						/// ������ �ϵ齺
		SI_SA_EBS_ENERGETIC_HEART,						/// ������ƽ ��Ʈ
		SI_SA_EEL_DREAD_SHOCK,							/// �巹�� ��ũ

		SI_A_EN_ILLUSION_STRIKE = 5200,					/// �Ϸ��� ��Ʈ����ũ
		SI_A_EN_DIMENSION_LINK,							/// ���� ��ũ - ��Ʈ��
		SI_A_EN_CLOAKING,								/// Ŭ��ŷ
		SI_A_EN_CONVERSION,								/// ���� ��ȯ
		SI_A_EN_MEGA_ELECTRONBALL,						/// �ް� �Ϸ�Ʈ�к�
		SI_A_EN_ILLUSION_RAY = 5205,					/// �Ϸ��� ����
		SI_A_EN_TESLA_SHOCK,							/// �׽��� ��ũ
		SI_A_EN_PHOTON_BLINK,							/// ���� ����
		SI_A_EEG_HORNET_STING_EXPLOSION,				/// ȣ�� ���� - �ͽ�������
		SI_A_EEG_HORNET_STING_SHAPED_CHARGE,			/// ȣ�� ���� - ������Ʈ ����
		SI_A_EEG_METAL_DUST_AURA = 5210,				/// ��Ż ����Ʈ �ƿ��
		SI_A_ENS_ATOMIC_BLASTER_PLASMAGUN,				/// ����� ������ - �ö�� ��
		SI_A_ENS_ATOMIC_BLASTER_SONICGUN,				/// ����� ������ - �Ҵ� ��
		SI_A_ENS_SPEAR_BURST,							/// ���Ǿ� ����Ʈ
		SI_A_EAT_SURFACE_CUTING,						/// ���ǽ� Ŀ��
		SI_A_EAT_OBERON_GUARD = 5215,					/// ������ ����
		SI_A_EEP_SPIT_FIRE_FLUSH,						/// ���� ���̾� - �÷���
		SI_A_EEP_SPIT_FIRE_GRENADE,						/// ���� ���̾� - �׸����̵�
		SI_SA_EEP_ELECTRONIC_FIELD,						/// �Ϸ�Ʈ�δ� �ʵ�
		SI_A_EEL_SPECTRO_EL_CRYSTAL,					/// �� �б� ����
		SI_A_EBS_TASER_PILUM = 5220,					/// ������ �ʶ� (������ ����)
		SI_A_EBS_KUGELBLITZ,							/// ��ۺ���

		SI_P_EN_ELECTRONBALL_UP = 5400,					/// �Ϸ�Ʈ�к� ����
		SI_P_EN_QUEENS_CONTROL,							/// ������ �����
		SI_P_EN_QUEENS_POTENTIAL,						/// ������ �����
		SI_P_EN_REFINED_STEP,							/// ����ѹ߰���
		SI_P_EN_PHOTON_BOOSTER,							/// ���� ������
		SI_P_ENS_NASOD_WEAPON = 5405,					/// ���ҵ� ���� ��ȭ
		SI_P_ENS_ENERGY_OF_CONCENTRATION,				/// ������ ���
		SI_P_EEP_ELECTRONBALL_MASTER,					/// �Ϸ�Ʈ�� ��������
		SI_P_EEP_CHARGING_BOOSTER,						/// ����������
		SI_P_EEL_SPECTRUM_DYNAMICS,						/// �б� ����
		SI_P_EEL_ADVANCED_EL_ENERGY_REACTOR = 5410,		/// ��ȭ �� ������ ������
		SI_P_EBS_HYPER_OPTICAL_RESEARCH,				/// �ʱ��� ����
		SI_P_EBS_HIGHPOWERED_ELECTRICS,					/// ����� ����ȸ��
		SI_P_EBS_AMPLIFIED_EL_ENERGY_REACTOR,			/// ���� �� ������ ������

		SI_HA_ENS_LUNATIC_SCUD = 5600,					/// �糪ƽ ��Ŀ��
		SI_HA_EEP_LINK_OVERCHARGE_ILLUSION,				/// ��ũ �������� �Ϸ���
		SI_HA_EBS_PSYCHIC_ARTILLERRY,					/// ����ű ��ƿ����( �̷����� ĳ�� )

		// Chung
		SI_SA_CC_LUNATIC_BLOW = 6000,					/// �糪ƽ ��ο�
		SI_SA_CC_SCARE_CHASE,							/// ���ɾ� ü�̽�
		SI_SA_CC_GIGANTIC_IMPACT,						/// �Ⱓƽ ����Ʈ
		SI_SA_CC_ACELDAMA,								/// �ưִٸ�
		SI_SA_CFG_LUNATIC_FURY,							/// �糪ƽ ǻ��
		SI_SA_CFG_PANDEMONIUM_CHAOS = 6005,				/// �ǵ���Ͽ� - ī����
		SI_SA_CFG_PANDEMONIUM_FEAR,						/// �ǵ���Ͽ� - �Ǿ�
		SI_SA_CFG_PAIN_OF_CALADBOLG,					/// Į�󺼱� ����
		SI_SA_CIP_IRON_HOWLING,							/// ���̾� �Ͽ︵
		SI_SA_CIP_LAND_DEMOLISHIER_HEAVY_ARMS,			/// ���� ������� - ��� ����
		SI_SA_CIP_LAND_DEMOLISHER_EARTHQUAKE = 6010,	/// ���� ������� - � ����ũ
		SI_SA_CSG_ARTILLERY_STRIKE_QUANTUM_BALLISTA,	/// ��ƿ���� ��Ʈ����ũ - ���� �߸���Ÿ
		SI_SA_CSG_HEAVY_RAILGUN,						/// ����ϰ�
		SI_SA_CSG_SHOOTING_STAR,						/// ���� ��Ÿ
		SI_SA_CDC_COMET_CRASHER,						/// ī�� ũ����
		SI_SA_CDC_SHARPSHOOTER_SYNDROME = 6015,			/// �������� ��Ʈ��
		SI_SA_CHG_BIGBANG_STREAM,						/// ��� ��Ʈ��
		SI_SA_CHG_CHAOS_CANNON,							/// ī���� ĳ��
		SI_SA_CHG_CARPET_BOMBING,						/// ī�� �ٹ�
		SI_SA_CTT_ARTILLERY_STRIKE,						/// ��ƿ���� ��Ʈ����ũ - �̽� ����
		SI_SA_CTT_DREAD_CHASE = 6020,					/// �巹�� ü�̽�
		SI_SA_CTT_TACTICAL_FIELD_INDURANCE,				/// ��Ƽ�� �ʵ� - �ε෱��
		SI_SA_CTT_TACTICAL_FIELD_RAID,					/// ��Ƽ�� �ʵ� - ���̵�

		SI_A_CC_BRUTAL_SWING = 6200,					/// ���Ż ����
		SI_A_CC_DETONATION,								/// ������̼�
		SI_A_CC_IMPACT_DETONATION,						/// ����Ʈ ������̼�
		SI_A_CC_RELOAD_CANNON,							/// ź��
		SI_A_CC_AIMMING_SHOT,							/// ���̹� ��
		SI_A_CC_SIEGE_SHELLING = 6205,					/// ���� ����
		SI_A_CFG_STEEL_EDGE,							/// ��ƿ ����
		SI_A_CFG_GUARD,									/// ����
		SI_A_CFG_LEAP_ATTACK,							/// ��������
		SI_A_CIP_SUDDEN_BURSTER,						/// ���� ������
		SI_A_CIP_ARTILLERY_NOVA = 6210,					/// ��ƿ���� ���
		SI_A_CIP_BURST_WOLF,							/// ����Ʈ ����
		SI_A_CSG_DOUBLE_FIRE,							/// ���� ���̾�
		SI_A_CSG_DUAL_BUSTER,							/// ��� ������
		SI_A_CSG_HEAD_SHOT,								/// ��弦
		SI_A_CSG_MAGNUM_SHOT = 6215,					/// �ű׳� ��
		SI_A_CDC_RUMBLE_SHOT,							/// ����
		SI_A_CDC_BULLET_BLITZ,							/// �ҷ� ����
		SI_A_CDC_GATLING_SHOT,							/// ��Ʋ�� ��
		SI_A_CDC_LEG_SHOT,								/// ���׼�
		SI_A_CHG_CANNON_STRIKE = 6220,					/// ĳ�� ��Ʈ����ũ
		SI_A_CHG_ELASTIC_BOMB_GRENADE,					/// �Ϸ���ƽ ��
		SI_A_CHG_ELASTIC_BOMB_LAUNCHER,					/// �Ϸ���ƽ �� - �׸����̵� ��ó
		SI_A_CHG_MARK_OF_COMMANDER,						/// ���ְ��� ǥ��
		SI_A_CTT_WONDER_WALL,							/// ���� ��
		SI_A_CTT_AUTOMATIC_MORTAR = 6225,				/// �ڰ���
		SI_A_CTT_BOMBARD_SERVICE,						/// ���� ����

		SI_P_CFG_METABOLISM_BOOST = 6400,				/// ������� ����
		SI_P_CFG_GUARD_MASTERY,							/// ������
		SI_P_CFG_ELEMENTAL_DEFENCE_A,					/// �Ұ� ���� �ڿ�
		SI_P_CFG_ELEMENTAL_DEFENCE_B,					/// �ٶ��� �ؿ� ��
		SI_P_CIP_ADVANCED_METABOLISM_BOOST,				/// ������� ���� ��ȭ
		SI_P_CIP_IRON_WILL = 6405,						/// ������ ����
		SI_P_CSG_REMODEL_MAGAZINE,						/// źâ/��� ����
		SI_P_CSG_REMODEL_RIFLE,							/// ���� ����
		SI_P_CDC_TUSSLE_TECHNIQUE,						/// ������ ���
		SI_P_CDC_ACCURATE_MARKMANSHIP,					/// ������ ��ݼ�
		SI_P_CDC_INTENSE_SHOWTIME = 6410,				/// ������ ��Ÿ��
		SI_P_CDC_GAS_PISTON_SYSTEM,						/// ���� �ǽ��� �ý���
		SI_P_CHG_RELOAD_MASTERY,						/// ���ε� �����͸�
		SI_P_CHG_CONVERTED_WARHEAD,						/// ź�� ����
		SI_P_CHG_CANNONEER_WITHSTANDING,				/// ������ ��Ƽ��
		SI_P_CTT_MOBILE_ARMORED_MODE = 6415,			/// �̵� ����
		SI_P_CTT_REACTIVE_ARMOR,						/// ���� �尩

		SI_HA_CIP_DOOM_STRIKER = 6600,					/// �� ��Ʈ����ũ
		SI_HA_CDC_OUTRAGE_STRIKE,						/// �ƿ������� ��Ʈ����ũ
		SI_HA_CTT_SATELITE_RAIN,						/// ��Ʋ����Ʈ ����

		// Ara
		SI_SA_AM_PULLING_PIERCE = 7000,					/// õ��_���� �α�
		SI_SA_AM_TEMPEST_DANCE,							/// ��ǳ_�ָ�
		SI_SA_AM_FALLING_DRAGON,						/// ������_���� ������
		SI_SA_AM_SHADOW_KNOT,							/// �῵_�׸��� �ŵ�_�ͻ� 3��
		SI_SA_ALH_MOONLIGHT_SLASH,						/// ��� 4�� - �޺� ����
		SI_SA_ALH_PEERLESSNESS_SLASH = 7005,			/// ������
		SI_SA_ALH_WHITE_TIGER,							/// ��ȣ��
		SI_SA_ALH_SWALLOW_ASSAULT,						/// ��
		SI_SA_ASD_SUPPRESSION,							/// ����
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
		SI_SA_ALD_FINGER_BULLET,						/// ���� 1�� ��ȥ
		SI_SA_ALD_WOLF_SPEAR = 7010,					/// ���� 4�� ���� �̻�
		SI_SA_ALD_ENERGY_WAVE,							/// �����
		SI_SA_AYR_SUPPRESSION_ENERGY,					/// ���� : ��
		SI_SA_ALD_ENERGY_BEAD,							/// ���� 2�� ��ȯ��
		SI_SA_ALD_HELL_OF_TORNADO,						/// ��ȯ����
		SI_SA_ALD_PRISON_SPEAR = 7015,					/// ���� 3�� â�차��
		SI_SA_AYR_ENERGY_VOID,							/// ��������
#endif // SERV_ARA_CHANGE_CLASS_SECOND

		SI_A_AM_WIND_WEDGE = 7200,						/// ��ǳ��_�ٶ� ����_���� 2��
		SI_A_AM_SHADOW_RISING,							/// ���·�_õ�� �ɸ���
		SI_A_AM_TIGER_CLAW,								/// ȣ����_����_��ȣ 2��
		SI_A_AM_FORCE_SPEAR,							/// ���â
		SI_A_AM_GAIN_FORCE,								/// ���
		SI_A_AM_QUICK_PIERCING = 7205,					/// �ż�
		SI_A_AM_SHOULDER_BLOW,							/// ö���
		SI_A_AM_QUICK_STAB,								/// õ��
		SI_A_ALH_ROCK_CRASH,							/// ��ȣ 1�� - ��������
		SI_A_ALH_BREAKING,								/// ��ȭ����
		SI_A_ALH_DOUBLE_COLLISION = 7210,				/// ��� 3�� - �ȱ���
		SI_A_ASD_DRAGON_FALLING,						/// ����
		SI_A_ASD_TURBULENT_WAVE,						/// ��ȣ 4�� - �뵵
		SI_A_ASD_LOW_BRANDISH,							/// ��ȣ 3�� - ����
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
		SI_A_ALD_WOLF_CLAW,								/// ���� 3�� ���� ����
		SI_A_ALD_CONVERSION_HEALTH = 7215,				/// ������
		SI_A_AYR_HIGH_SPEED,							/// ���
		SI_A_ALD_CONVERSION_MANA,						/// �̱��
		SI_A_AYR_STEAL_SOUL,							/// ���� 4�� Żȥ
		SI_A_ALD_REFLECTION,							/// ��ź��
#endif // SERV_ARA_CHANGE_CLASS_SECOND

		SI_P_ALH_QUICK_STAND = 7400,					/// ����� ���
		SI_P_ALH_VIGOR_ENERGIZE,						/// ��� Ȱ��ȭ
		SI_P_ALH_FILLED_POWER,							/// �游�� ��
		SI_P_ASD_QUICKEN_GUARD,							/// �����̴� ����
		SI_P_ASD_BRUTAL_PIERCING,						/// â���� ����
		SI_P_ASD_FULLMOON_SLASH = 7405,					/// ���� ����
		SI_P_ASD_POWER_OF_TIGER,						/// ��ȣ�� ��
		SI_P_ASD_SELF_PROTECTION_FORTITUDE,				/// ȣ�Ű���
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
		SI_P_AYR_ENERGY_BULLET_REINFORCE,				/// ��ź ��ȭ
		SI_P_AYR_HOWLING,								/// ������ ���¢��
		SI_P_AYR_RESURRECTION = 7410,					/// ȸ������
		SI_P_ALD_ENERGY_REINFORCE,						/// ��� ��ȭ
		SI_P_AYR_HELL_GATE_OPEN,						/// ���� �ʷ�
		
#endif // SERV_ARA_CHANGE_CLASS_SECOND

		SI_HA_ASD_THOUSANDS_BLOSSOMS = 7600,			/// �б��õ - õ��ȭ
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
		SI_SA_AYR_ENERGY_CANNON,						/// ���ȯ����
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#else //SERV_UPGRADE_SKILL_SYSTEM_2013
/*
		// �нú� �ʻ��, ĳ���� ����	1~299
		SI_P_COMMON_PHYSIC_ATTACK_BEGINNER = 1,			//���ʱٷ´ܷ�
		SI_P_COMMON_MAGIC_ATTACK_BEGINNER,				//���ʸ�������
		SI_P_COMMON_PHYSIC_DEFENCE_BEGINNER,			//���ʹ�������
		SI_P_COMMON_MAGIC_DEFENCE_BEGINNER,				//���ʸ�������
		SI_P_COMMON_PHYSIC_ATTACK_INTERMEDIATE,			//�߱ޱٷ´ܷ�
		SI_P_COMMON_MAGIC_ATTACK_INTERMEDIATE,			//�߱޸�������
		SI_P_COMMON_PHYSIC_DEFENCE_INTERMEDIATE,		//�߱޹�������
		SI_P_COMMON_MAGIC_DEFENCE_INTERMEDIATE,			//�߱޸�������

		SI_P_COMMON_GET_CHANCE,							//��ȸ������				
		SI_P_COMMON_GET_MANA_UP,						//Ȱ��						
		SI_P_COMMON_POWERFUL_VITAL,						//�����ѽ�ü				
		SI_P_COMMON_POWERFUL_SOUL,						//���������ŷ�				
		SI_P_COMMON_SUPPORT_AURA,						//�ݷ��Ǳ��				
		SI_P_COMMON_UNLIMIT_MANA,						//�����¸���				
		SI_P_COMMON_SHADOW_DATH,						//�����Ǳ׸���				
		SI_P_COMMON_PHYSIC_HARMONY,						//��ü����������ȭ			
		SI_P_COMMON_SPRINTER,							//��������					
		SI_P_COMMON_SOFTBODY,							//�����Ѹ��				
		SI_P_COMMON_FIGHTER_SOUL,						//��������ȥ				
		SI_P_COMMON_CONFIDENCE_VICTORY,					//�¸����ڽŰ�
		SI_P_COMMON_HARDEN_BODY,						//�ܷõȽ�ü				
		SI_P_COMMON_REVERSAL_SOLDIER,					//�����ǿ��
		SI_P_COMMON_INDUCE_DESIRE_WINNING,				//ȣ�½�����				
		SI_P_COMMON_REFINED_STEP,						//����ѹ߰���				


		// ���� �ʻ��, ĳ���� ����		300~599
		SI_SA_COMMON_POWER_ACCEL = 300,					//�Ŀ�����������
		SI_SA_COMMON_MAGIC_ACCEL,						//��������������
		SI_SA_COMMON_SHIELD_ACCEL,						//�ǵ忢��������
		SI_SA_COMMON_AURA_POWER_ACCEL,					//�����Ŀ�����������
		SI_SA_COMMON_AURA_MAGIC_ACCEL,					//������������������
		SI_SA_COMMON_AURA_SHIELD_ACCEL,					//�����ǵ忢��������
		SI_SA_COMMON_POWER_ADRENALIN,					//�Ŀ��Ƶ巹����
		SI_SA_COMMON_MAGIC_ADRENALIN,					//�����Ƶ巹����
		SI_SA_COMMON_SHIELD_ADRENALIN,					//�ǵ�Ƶ巹����
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		SI_SA_COMMON_AURA_SPEED_ACCEL			= 309,	// ���� ���ǵ� �׼���������
#endif






		// ��Ƽ�� �ʻ��, ĳ���� ��������, ĳ���� �ϳ��� skill id�� ������ 500
		// elsword
		SI_SA_ES_MEGASLASH = 1000,						//�ް�������
		SI_SA_ES_ASSAULT_SLASH,							//�Ʈ������
		SI_SA_ES_FATAL_FURY,							//����Żǻ��
		SI_SA_ES_FLAME_GEYSER,							//�����Ӱ�����
		SI_SA_ES_UNLIMITED_BLADE,						//�𸮹�Ƽ����̵�
		SI_SA_ES_TRIPLE_GEYSER,							//Ʈ���ð�����
		SI_SA_ESK_AIR_SLASH,							//�������
		SI_SA_ESK_SPIRAL_BLAST,							//�����̷�����Ʈ
		SI_SA_ESK_ARMAGEDON_BLADE,						//�Ƹ��ٵ����̵�
		SI_SA_EMK_WIND_BLADE,							//������̵�
		SI_SA_EMK_RISING_SLASH,							//����¡������
		SI_SA_EMK_STORM_BLADE,							//������̵�
		SI_SA_ESK_WINDMILL,								
		SI_SA_EMK_SWORD_FIRE,
		SI_SA_ESK_DOUBLE_SLASH,							//���� ������
		SI_SA_EMK_PHOENIX_TALON,						//�Ǵн� �ŷ�				
		SI_SA_ELK_SONIC_BLADE,	
		SI_SA_ERS_SPLASH_EXPLOSION,
		SI_SA_ELK_SAND_STORM,							/// ���� ����				// oasis907
		SI_SA_ERS_LUNA_BLADE,							/// �糪 ���̵�			// JHKang
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
		SI_SA_ETK_CRESCENT_CUT,							/// ũ����Ʈ ��
		SI_SA_ETK_SWORD_FALL,							/// �ҵ���	
		SI_SA_ETK_FINAL_STRIKE,							/// ���̳� ��Ʈ����ũ
		SI_SA_ETK_MAELSTORM_RAGE,						/// ���Ͻ��� ������
		SI_SA_ETK_PHANTOM_SWORD,						/// ���� �ҵ�
#endif SERV_ELSWORD_SHEATH_KNIGHT
#ifdef SERV_ELSWORD_INFINITY_SWORD
		SI_SA_EIS_SWORD_BLASTING	= 1025,				//  �ҵ� ������
		SI_SA_EIS_RAGE_CUTTER,							//  ������ Ŀ��
#endif //SERV_ELSWORD_INFINITY_SWORD
#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
		SI_SA_EMK_RISING_SLASH_WAVE	= 1026,					//  ����¡ ���̺�	
#endif //UPGRADE_SKILL_SYSTEM_2013

		SI_A_ES_COUNTER = 1200,							//�ݰ�						
		SI_A_ES_ENDURANCE,								//�ر�						
		SI_A_ES_KICK,									//������					
		SI_A_ESK_ARMOR_BREAK,														
		SI_A_EMK_BIG_BURST,															
		SI_A_ELK_ROLLING_SMASH,
		SI_A_ERS_SWORD_ENCHANT,
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
		SI_A_ES_ROLLING,								/// ������
		SI_A_ETK_HARSH_CHASER,							/// �Ͻ� ü�̼�
#endif SERV_ELSWORD_SHEATH_KNIGHT
#ifdef SERV_ELSWORD_INFINITY_SWORD
		SI_A_EIS_MIRAGE_STING	= 1209,					// �̶��� ����
#endif //SERV_ELSWORD_INFINITY_SWORD
#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
		SI_A_ELK_IMPACT_SMASH,							//  ����Ʈ ���Ž�
		SI_A_ES_SWORD_WAVE,								//  �ҵ� ���̺�
		SI_A_ES_DEFENCE,								//  ����			
		SI_A_ERS_CRITICAL_SWORD,						//	ũ��Ƽ�� �ҵ�
		SI_A_ERS_RUNE_OF_FIRE,							//  �ʿ��� ��
		SI_A_ERS_RUNE_OF_ICE,							//  �Ѻ��� ��
		SI_A_ETK_PIERCING_SWORD,						//  �Ǿ�� �ҵ�
		SI_A_EIS_FATAL_SLAP,							//  ����Ż ����
		SI_A_ETK_SWORD_SHIELD,							//  �ҵ� �ǵ�
#endif //UPGRADE_SKILL_SYSTEM_2013


		SI_P_ES_COUNTER_UP	= 1400,						//�ݰݿ���					
		SI_P_ES_POWERFUL_FIREBALL,						//��ȭ�����̾			
		SI_P_ELK_STRONG_BONE,			
		SI_P_ERS_SPELL_CHAIN,
		SI_P_ELK_REDUCE_PAIN,							/// �������				// kimhc // 2010-11-15
		SI_P_ERS_MAGIC_RESISTANCE,						/// �������� ����			// oasis907
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
		SI_P_ETK_COMPACT_COUNTER,
		SI_P_ETK_BRUTAL_SLAYER,							/// ��Ȥ�� �л���								
#endif SERV_ELSWORD_SHEATH_KNIGHT
#ifdef SERV_ELSWORD_INFINITY_SWORD
		SI_P_EIS_MIND_OF_FIGHTER	= 1408,				// �ºλ��� ����
		SI_P_EIS_LIGHTNING_STEP,						// ���� ��ȭ
#endif //SERV_ELSWORD_INFINITY_SWORD
#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
		SI_P_ESK_AUTO_GUARD,							//  ���� ����
		SI_P_ERS_RUNE_MASTERY,							//  �� �����͸�
		SI_P_ESK_INTENSIVE_ATTACK,						//  ������ �ϰ�
		SI_P_ELK_INDURANCE_OF_REVENGE,					//  ������ �γ�
		SI_P_EIS_CUTTING_SWORD,							//  ��ī�ο� ��
#endif //UPGRADE_SKILL_SYSTEM_2013


		// aisha
		SI_SA_AV_CHAIN_FIRE_BALL = 1500,				//ü�����̾
		SI_SA_AV_BINDING_CIRCLE,						//���ε���Ŭ
		SI_SA_AV_GUST_SCREW,							//�Ž�Ʈ��ũ��
		SI_SA_AV_LIGHTNING_BOLT,						//����Ʈ�׺�Ʈ
		SI_SA_AV_GUST_STORM,							//�Ž�Ʈ����
		SI_SA_AV_METEO_CALL,							//���׿���
		SI_SA_AHM_CHAIN_BURST,							//ü�ι���Ʈ
		SI_SA_AHM_MAGIC_MISSILE,						//�����̻���
		SI_SA_AHM_INFERNAL_WAVE,						//���۳ο��̺�
		SI_SA_ADM_HELL_STONE,							//�ｺ��
		SI_SA_ADM_DARK_CLOUD,							//��ũŬ����
		SI_SA_ADM_PLASMA_CUTTER,						//�ö��Ŀ��
		SI_SA_AHM_BLIZZARDSHOWER,
		SI_SA_ADM_DEATHFIELD,
		SI_SA_AHM_BLAZE_STEP,							//�������
		SI_SA_ADM_AGING,								//����¡
		SI_SA_AEM_CYCLONE,
		SI_SA_AVP_HELL_DROP,

		SI_SA_AEM_METEOR_SHOWER					= 1518,
		SI_SA_AVP_PHANTOM_BREATHING				= 1519,

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		SI_SA_ABM_HEAVY_PRESS					= 1520,	// ��� ������
		SI_SA_ABM_ENERGY_SPURT,							// ������ ����Ʈ
		SI_SA_ABM_SUPER_NOVA,							// ���� ���
		SI_SA_ABM_GUILLOTINE_PRESS,						// ���ƾ ������
		SI_SA_ABM_MAGICAL_MAKEUP,						// ������ ����ũ��
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
		SI_SA_ADW_MORNING_STAR	= 1525,					// ��� ��Ÿ
		SI_SA_ADW_SCREWDRIVER,							// ��ũ�� ����̹�
#endif


		SI_A_AV_TELEPORT = 1700,						//�ڷ���Ʈ					
		SI_A_AV_MANA_SHIELD,							//�����ǵ�					
		SI_A_AV_CIRCLE_FLAME,							//��Ŭ�÷���				
		SI_A_AHM_MEDITATION,														
		SI_A_ADM_MANA_INTAKE,							//����������ũ				
		SI_A_AEM_STONE_WALL,
		SI_A_AVP_SUMMON_BAT,
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		SI_A_AV_FIRE_ROAD,								// ���̾� �ε�
		SI_A_ABM_ENERGY_DRAIN,							// ������ �巹��
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
		SI_A_ADW_DISTORTION	= 1709,						// ���� �ְ�
#endif


		SI_P_AV_TELEPORT_UP = 1900,						//�ڷ���Ʈ����				
		SI_P_AV_FIREBALL_UP,							//���̾����				
		SI_P_AEM_ELEMENTAL_FRIENDSHIP,
		SI_P_AVP_STRONG_MIND, 
		SI_P_AEM_ENERGY_OF_THE_PLENTY,					/// ǳ���� ���				// oasis907

		SI_P_AVP_LIMITED_MANA_MANAGEMENT		= 1905,
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
		SI_P_ABM_FITNESS						= 1906,	// ��ü �ܷ�
		SI_P_ABM_MANAFLOW_OVERDRIVE,					// ���� ����
		SI_P_ABM_SPIRIT_ACCELERATION,					// ��� ����
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
		SI_P_ADW_ADVANCED_TELEPORTATION	= 1909,			// ���� ����
		SI_P_ADW_MAGICAL_STAFF,							// ������ ������

#endif

		// rena
		SI_SA_LE_RAIL_STINGER = 2000,					//���Ͻ��ð�
		SI_SA_LE_ASSAULT_KICK,							//�Ʈű
		SI_SA_LE_PERFECT_STORM,							//����Ʈ����
		SI_SA_LE_MULTIPLE_STINGER,						//��Ƽ�ý��ð�
		SI_SA_LE_AERO_TORNADO,							//���������̵�
		SI_SA_LE_PHOENIX_STRIKE,						//�Ǵн���Ʈ����ũ
		SI_SA_LCR_SPINNING_KICK,						//���Ǵ�ű
		SI_SA_LCR_CRESCENT_KICK,						//ũ����Ʈű
		SI_SA_LCR_DIVE_KICK_BOMBING,					//���̺�ű����
		SI_SA_LSR_HUMMING_WIND,							//�������
		SI_SA_LSR_GUIDED_ARROW,							//���̵�ַο�
		SI_SA_LSR_CRAZY_SHOT,							//ũ��������
		SI_SA_LCR_SHARPFALL,
		SI_SA_LSR_WINDWARD,
		SI_SA_LCR_VIOLENT_ATTACK,						//���̿÷�Ʈ ����
		SI_SA_LSR_GUNGNIR,								//�ñ״�
		SI_SA_LWS_SLIDE_KICK, 
		SI_SA_LGA_FREEZING_ARROW, 
		SI_SA_LWS_AIRELINNA,							/// ���̷�����				// oasis907

		SI_SA_LGA_AERO_STRAFE					= 2019,

#ifdef	SERV_TRAPPING_RANGER_TEST
		SI_SA_RTR_EVOKE							= 2020,	///	�̺�ũ
		SI_SA_RTR_FATALITY						= 2021,	///	����Ż��Ƽ
		SI_SA_RTR_TRAPING_ARROW					= 2022,	///	Ʈ���� �ַο�
		SI_SA_RTR_CALL_OF_RUIN					= 2023,	///	�� ���� ����
		SI_SA_RTR_KARMA							= 2024,	///	ī����
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
		SI_SA_RNW_FURIOUS_ENGAGE				= 2025,	///	ǻ��� �ΰ�����
		SI_SA_RNW_GLIDING_STRIKE				= 2026,	///	�۶��̵� ��Ʈ����ũ
#endif

#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
		SI_SA_RTR_TRAPPING_ARROW_FUNGUS,				/// Ʈ���� �ַο� - ��Ž�
		SI_SA_LWS_AIRELINNA_SYLPH,						/// ���̷����� - ����
#endif //UPGRADE_SKILL_SYSTEM_2013


		SI_A_LE_SIEGEMODE = 2200,						//������					
		SI_A_LE_BACKJUMP_SHOT,							//�������
		SI_A_LE_REFLEX_MAGIC,							//�����ݻ�
		SI_A_LCR_LOW_KICK,								//�ο�ű					// wonpok. �۾��Ϸ�
		SI_A_LSR_ENTANGLE,															// dmlee �۾���
		SI_A_LWS_NATURE_FORCE, 
		SI_A_LGA_STIGMA_ARROW, 

#ifdef	SERV_TRAPPING_RANGER_TEST
		SI_A_LE_RISING_FALCON					= 2207,	///	����¡ ����
		SI_A_RTR_EXPLOSION_TRAP					= 2208,	///	������ ��
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
		SI_A_RNW_START_OF_DELAYED_FIRING		= 2209,	///	������ ��ȣź
#endif

#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
		SI_A_LCR_MIDDLE_KICK,							/// �̵�ű
		SI_A_LWS_BACK_KICK,								/// ��ű
		SI_A_LWS_HIGH_KICK,								/// ����ű
		SI_A_LSR_SHOOTING_MAGNUM,						/// ���� �ű׳�
		SI_A_LGA_ARC_SHOT,								/// ��ũ ��
		SI_A_LGA_RAPID_SHOT,							/// ���ǵ� ��
		SI_A_LGA_FREEZING_BOLT,							/// ����¡ ��Ʈ
		SI_A_LGA_WIND_BLAST,							/// ���� ����Ʈ
		SI_A_RNW_THORNS_TRAP,							/// ���õ��� ��
		SI_P_RNW_SEED_OF_RUIN,							/// ������ ����
		SI_A_RNW_ANGER_OF_ELF,							/// ��������� �г�
		SI_A_RNW_ROSEBUSH_TRAP,							/// �� ��
		SI_A_LWS_ASSAULT_IMPACT,						/// �Ʈ ����Ʈ
#endif //UPGRADE_SKILL_SYSTEM_2013


		SI_P_LE_SIEGEMODE_UP = 2400,					//�����忬��				
		SI_P_LE_SHARP_KICK,								//�����ѹ�����				
		SI_P_LE_POWERFUL_BOWSTRING,						//��ȭ�Ⱥ��콺Ʈ��			
		SI_P_LE_HAWK_EYE,								//ȣũ����					
		SI_P_LE_SHARP_ARROW,							//�ٵ����ȭ��				
		SI_P_LWS_POWERFUL_SHOT, 
		SI_P_LGA_COMMUNE_OF_NATURE,
		SI_P_LWS_FRIENDSHIP_OF_NATURE,					/// �ڿ����� ģȭ			// oasis907
		SI_P_LGA_VIBRATION_SHOOTING,					/// ���� ���				// oasis907

#ifdef	SERV_TRAPPING_RANGER_TEST
		SI_P_RTR_HUNTERS_ABILITY				= 2409,	///	��ɲ��� ���(�нú�)
		SI_P_RTR_SHARPEN_ARROW					= 2410,	///	��ī�ο� ȭ��(�нú�)
		SI_P_RTR_VITALPOINT_PIERCING			= 2411,	///	�޼� ���(�нú�)
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
		SI_P_RNW_PRIOR_PLANNED_BLOW				= 2412,	///	���� �ϰ�
		SI_P_RNW_GUARDIAN_OF_ELF,						/// ������ ��ȣ��
#endif


		// raven
		SI_SA_RF_MAXIMUM_CANNON = 2500,					//�ƽø�ĳ��
		SI_SA_RF_EARTH_BREAKER,							//��극��ũ
		SI_SA_RF_POWER_ASSAULT,							//�Ŀ��Ʈ
		SI_SA_RF_CANNON_BLADE,							//ĳ����̵�
		SI_SA_RF_MAGNUM_BLASTER,						//�ű׳Ѻ�����
		SI_SA_RF_SEVEN_BURST,							//�������Ʈ
		SI_SA_RST_BERSERKER_BLADE,						//����Ŀ���̵�
		SI_SA_RST_FLYING_IMPACT,						//�ö�������Ʈ
		SI_SA_RST_HYPER_SONIC_STAB,						//�����ۼҴн���
		SI_SA_ROT_CHARGED_BOLT,							//�����庼Ʈ
		SI_SA_ROT_ARC_ENEMY,							//��ũ�ֳʹ�
		SI_SA_ROT_GUARDIAN_STRIKE,						//�����Ʈ����ũ
		SI_SA_RST_SHOCKWAVE,
		SI_SA_ROT_VALKYRIESJAVELIN,
		SI_SA_RST_BLOODY_ACCEL,							//���𿢼�
		SI_SA_ROT_NUCLEAR,								//��Ŭ����	
		SI_SA_RBM_WOFL_FANG,							//������
		SI_SA_RRF_X_CRASH,								//���� ũ����

		SI_SA_RBM_GIGA_DRIVE,							/// �Ⱑ ����̺�			// kimhc // 2010-11-15
		SI_SA_RRF_WILD_CHARGE,							/// ���ϵ� ����				// kimhc // 2010-11-15

#ifdef SERV_RAVEN_WEAPON_TAKER
		SI_SA_RWT_BURSTING_BLADE,						// ������ ���̵�
		SI_SA_RWT_REVOLVER_CANNON,						// ������ ĳ��
		SI_SA_RWT_HARPOON_SPEAR,						// ��Ǭ ���Ǿ�
		SI_SA_RWT_HELLFIRE_GATLING,						// �����̾� ��Ʋ��
		SI_SA_RWT_GIGA_PROMINENCE,						// �Ⱑ ���ι̳ͽ�
#endif SERV_RAVEN_WEAPON_TAKER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		SI_SA_RVC_DEADLY_RAID = 2525,					// ���鸮 ���̵�
		SI_SA_RVC_IGNITION_CROW,						// �̱״ϼ� ũ�ο�
#endif SERV_RAVEN_VETERAN_COMMANDER


		SI_A_RF_BURNING_RUSH = 2700,					//���׷���					
		SI_A_RF_SHADOW_STEP,							//�����콺��
		SI_A_RST_CUT_TENDON,							//���ٵ�		//�����淡 �߰�. 2011/12/19 ������
		SI_A_ROT_WEAPON_BREAK,							//���� ����		//�����淡 �߰�. 2011/12/19 ������
		SI_A_RBM_ONE_FLASH,								//�ϼ�
		SI_A_RRF_GROUND_IMPACT,							//�׶��� ����Ʈ
#ifdef SERV_RAVEN_WEAPON_TAKER
		SI_A_RF_BREAKING_FIST,							// �극��ŷ �ǽ�Ʈ
		SI_A_RWT_BARRAGE_ATTACK,						// ���� ����
#endif SERV_RAVEN_WEAPON_TAKER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		SI_A_RVC_BLEEDING_SLICER = 2708,				// ġ���
#endif SERV_RAVEN_VETERAN_COMMANDER


		SI_P_RF_THRUST = 2900,							//�������Ǿ��
		SI_P_RBM_BLADE_MASTERY,							//���õ� �˼�
		SI_P_RRF_BURNING_NASOD_HAND,					//Ÿ������ ���ҵ� �ڵ�
		SI_P_RBM_EMERGENCY_ESCAPE,						/// ��� Ż��
		SI_P_RRF_SPIRITUALIZED_FURY,					/// ��ȭ�� �г�
#ifdef SERV_RAVEN_WEAPON_TAKER
		SI_P_RWT_AIR_SLICER,							// ���� �����̼�
		SI_P_RWT_BEHAVIOR_OF_MERCENARY,					// �뺴�� ó�ż�
#endif SERV_RAVEN_WEAPON_TAKER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
		SI_P_RVC_SHADOW_BACK_SLIDE = 2907,				// ������ �� �����̵�
		SI_P_RVC_SURVIVAL_TECHNIQUE_OF_MERCENARY,		// �뺴�� ������
#endif SERV_RAVEN_VETERAN_COMMANDER


		// eve
		SI_SA_EN_ILLUSION_STRIKE = 3000,				//�Ϸ�����Ʈ����ũ
		SI_SA_EN_DIMENSION_LINK,						//���Ǹ�ũ
		SI_SA_EN_ILLUSION_STINGER,						//�Ϸ������þ�
		SI_SA_EN_DIMENSION_LINK_BLADER,					//���Ǹ�ũ���̴�
		SI_SA_EN_GENERATE_BLACKHOLE,					//���׷����ͺ�Ȧ
		SI_SA_EN_DIMENSION_LINK_GUARDIAN,				//���Ǹ�ũ�����
		SI_SA_EEG_HORNET_STING,							//ȣ�ݽ���
		SI_SA_EEG_EXPLOSION_IMPACT,						//�ͽ��÷�������Ʈ
		SI_SA_EEG_QUEENS_THRONE,						//��������
		SI_SA_EAT_SURFACE_CUTING,						//���ǽ�Ŀ��
		SI_SA_EAT_SONIC_WAVE,							//�Ҵп��̺�
		SI_SA_EAT_GENOCIDE_RIPPER,						//������̵帮��
		SI_SA_EEG_IRONSCRAPS,
		SI_SA_EAT_SPACE_WRENCH,
		SI_SA_EEG_JUNK_BREAK,							//��ũ�극��ũ
		SI_SA_EAT_HEAVENS_FIST,							//�غ����ǽ�Ʈ		
		SI_SA_ENS_ATOMIC_BLASTER,						//����ͺ�����
		SI_SA_EEP_ELECTRONIC_FIELD,						//�Ϸ�Ʈ�δ��ʵ�
		SI_SA_ENS_ATOMIC_SHIELD,						/// ����� ����
		SI_SA_EPP_ASSUALT_SPEAR,						/// �Ʈ ���Ǿ�
#ifdef SERV_EVE_ELECTRA
		SI_SA_EEL_PARTICLE_RAY,							// ��ƼŬ ����
		SI_SA_EEL_SWEEP_ROLLING,						// ������ �Ѹ�
		SI_SA_EEL_PHOTON_FLARE,							// ���� �÷���
		SI_SA_EEL_GIGA_STREAM,							// �Ⱑ ��Ʈ��
		SI_SA_EEL_THOUSANDS_OF_STARS,					// ������� ��Ÿ
#endif SERV_EVE_ELECTRA
#ifdef SERV_EVE_BATTLE_SERAPH
		SI_SA_EBS_ENERGY_NEEDLES,						/// ������ �ϵ齺
		SI_SA_EBS_ENERGETIC_HEART,						/// ������ƽ ��Ʈ
#endif

		SI_A_EN_CLOAKING = 3200,						//Ŭ��ŷ					
		SI_A_EN_CONVERSION,								//������ȯ					
		SI_A_EN_MEGA_ELECTRONBALL,						//�ް��Ϸ�Ʈ�к�
		SI_A_EEG_METAL_DUST_AURA,						//��Ż����Ʈ�ƿ��
		SI_A_EAT_OBERON_GUARD,							//�����а����
		SI_A_ENS_SPEAR_BURST,							//���Ǿ����Ʈ
		SI_A_EEP_SPIT_FIRE,								//�������̾�
#ifdef SERV_EVE_ELECTRA
		SI_A_EVE_PHOTON_BLINK,							// ���� ����
		SI_A_EEL_SPECTRO_EL_CRYSTAL,					// �� �б� ����
#endif SERV_EVE_ELECTRA
#ifdef SERV_EVE_BATTLE_SERAPH
		SI_A_EBS_TASER_PILUM,							/// ������ �ʶ� (������ ����)
#endif

		SI_P_EN_CLOAKING_UP = 3400,						//Ŭ��ŷ����				
		SI_P_EN_ELECTRONBALL_UP,						//�Ϸ�Ʈ�к�����
		SI_P_EN_QUEENS_CONTROL,							//�����������				
		SI_P_EN_QUEENS_POTENTIAL,						//�����������
		SI_P_ENS_NASOD_WEAPON,							//���ҵ� ���� ��ȭ
		SI_P_EEP_ELECTRONBALL_MASTER,					//�Ϸ�Ʈ�к�������
		SI_P_ENS_ENERGY_OF_CONCENTRATION,				/// ������ ���			// oasis907
		SI_P_EEP_CHARGING_BOOSTER,						/// ����������			// kimhc // 2010-11-
#ifdef SERV_EVE_ELECTRA
		SI_P_EEL_SPECTRUM_DYNAMICS,						// �б� ����
		SI_P_EEL_PHOTON_BOOSTER,						// ���� ������
		SI_P_EEL_ADVANCED_EL_ENERGY_REACTOR,			// ��ȭ �� ������ ������
#endif SERV_EVE_ELECTRA
#ifdef SERV_EVE_BATTLE_SERAPH
		SI_P_EBS_HYPER_OPTICAL_RESEARCH,				/// �ʱ��� ����
		SI_P_EBS_HIGHPOWERED_ELECTRICS,					/// ����� ����ȸ�� (������ ��ų, ������ ��� �Ͷ�!)
#endif

		
		///////////////////////////// CHUNG //////////////////////////////////////////////////////////////
		//////////////////////////// ����� ��Ƽ��
		SI_SA_CHUNG_BRUTAL_SWING	= 4000,				/// ���Ż ����
		SI_SA_CHUNG_DETONATION,							/// ������̼�
		SI_SA_CHUNG_LUNATIC_BLOW,						/// �糪ƽ ��ο�
		SI_SA_CHUNG_SCARE_CHASE,						/// ���ɾ� ü�̽�
		SI_SA_CHUNG_GIGANTIC_IMPACT,					/// �Ⱓƽ ����Ʈ
		SI_SA_CHUNG_ACELDAMA,							/// �ưִٸ�

		//{{ kimhc // 2011.1.8 // û 1�� ����
		SI_SA_CFG_STEEL_EDGE,							/// ��ƿ���� (ǻ�������)
		SI_SA_CFG_IRON_HOWLING,							/// ���̾� �Ͽ︵ (ǻ�������)
		SI_SA_CFG_LUNATIC_FURY,							/// �糪ƽǻ�� (ǻ�������)

		SI_SA_CSG_DUAL_BUSTER,							/// �������� (���ð����)
		SI_SA_CSG_HEAVY_RAILGUN,						/// ����ϰ� (���ð����)
		SI_SA_CSG_SHOOTING_STAR,						/// ���ý�Ÿ (���ð����)
		//}} kimhc // 2011.1.8 // û 1�� ����

		//{{ kimhc // 2011-3-16 // 2011-04 �� ��ġ�� û ĳ�� ��ų
		SI_SA_CFG_PANDEMONIUM,							/// �ǵ���Ͽ� (ǻ�������)
		SI_SA_CFG_PAIN_OF_CALADBOLG,						/// Į�󺼱����� (ǻ�������)

		SI_SA_CSG_COMET_CRASHER,						/// ī��ũ���� (���ð����)
		SI_SA_CSG_SHARPSHOOTER_SYNDROME,				/// �������ͽ�Ʈ�� (���ð����)
		//}} kimhc // 2011-3-16 // 2011-04 �� ��ġ�� û ĳ�� ��ų

		SI_SA_CIP_BURST_WOLF,							/// ����Ʈ ���� (���̾� �ȶ��)
		SI_SA_CIP_LAND_DEMOLISHIER,						/// ���� ������� (���̾� �ȶ��)
		SI_SA_CDC_BULLET_BLITZ,							/// �ҷ� ���� (���鸮 ü�̼�)
		SI_SA_CDC_ARTILLERY_STRIKE,						/// ��ƿ���� ��Ʈ����ũ (���鸮 ü�̼�)

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
		SI_SA_CHG_WONDER_WALL			= 4020,			// ���� ��
		SI_SA_CHG_DREAD_CHASE,							// �巹�� ü�̽�
		SI_SA_CHG_BIGBANG_STREAM,						// ��� ��Ʈ��
		SI_SA_CHG_CHAOS_CANNON,							// ī���� ĳ��
		SI_SA_CHG_CARPET_BOMBING		= 4024,			// ī�� �ٹ�
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN

#ifdef SERV_CHUNG_TACTICAL_TROOPER
		SI_SA_CTT_CROSS_FIRE			= 4025,			// ũ�ν� ���̾�
		SI_SA_CTT_TACTICAL_FIELD		= 4026,			// ��Ƽ�� �ʵ�
#endif //SERV_CHUNG_TACTICAL_TROOPER

		/////////////////////////////// ��Ƽ��
		SI_A_CHUNG_RELOAD_CANNON	= 4200,				/// ź��
		SI_A_CHUNG_AIMMING_SHOT,						/// ���̹� ��
		//{{ kimhc // 2011.1.10 // û 1�� ����
		SI_A_CFG_GUARD,									/// ���� (ǻ�������)
		SI_A_CFG_LEAP_ATTACK,							/// �������� (ǻ�������)

		SI_A_CSG_HEAD_SHOT,								/// ��弦 (���ð����)
		SI_A_CSG_RUMBLE_SHOT,							/// ���ü� (���ð����)
		//}} kimhc // 2011.1.10 // û 1�� ����

		SI_A_CIP_SUDDEN_BURSTER,						/// ���� ������ (���̾� �ȶ��)
		SI_A_CDC_LEG_SHOT,								/// ���׼� (���鸮 ü�̼�)

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
		SI_A_CHUNG_SIEGE_SHELLING	= 4208,				// ���� ����
		SI_A_CHG_ELASTIC_BOMB,							// �Ϸ���ƽ ��
		SI_A_CHG_MARK_OF_COMMANDER	= 4210,				// ���ְ��� ǥ��
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN

#ifdef SERV_CHUNG_TACTICAL_TROOPER
		SI_A_CTT_AUTOMATIC_MORTAR		= 4211,				// �ڰ���
#endif //SERV_CHUNG_TACTICAL_TROOPER

		//////////////////////////////// �нú�
		SI_P_CHUNG_RELOAD_CANNON	= 4400,				/// ���õ� ź��
		//{{ kimhc // 2011.1.10 // û 1�� ����
		SI_P_CFG_METABOLISM_BOOST,						/// ������� ���� (ǻ�������)
		SI_P_CFG_GUARD_MASTERY,							/// ������ (ǻ�������)
		SI_P_CFG_ELEMENTAL_DEFENCE_A,					/// �Ұ� ���� �ڿ� (ǻ�������)
		SI_P_CFG_ELEMENTAL_DEFENCE_B,					/// �ٶ��� �ؿ� �� (ǻ�������)

		SI_P_CSG_REMODEL_MAGAZINE,						/// źâ/��� ���� (���ð����)
		SI_P_CSG_REMODEL_RIFLE,							/// ���� ���� (���ð����)
		SI_P_CSG_TUSSLE_TECHNIQUE,						/// ������ ��� (���ð����)
		//}} kimhc // 2011.1.10 // û 1�� ����

		SI_P_CIP_ADVANCED_METABOLISM_BOOST,				/// ������� ���� ��ȭ (���̾� �ȶ��)
		SI_P_CIP_IRON_WILL,								/// ������ ���� (���̾� �ȶ��)
		SI_P_CDC_ACCURATE_MARKMANSHIP,					/// ������ ��ݼ� (���鸮 ü�̼�)
		SI_P_CDC_INTENSE_SHOWTIME,						/// ������ ��Ÿ�� (���鸮 ü�̼�)

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
		SI_P_CHUNG_ABILITY_CANNONEER	= 4412,			// ���õ� ������ �ɷ�
		SI_P_CHG_RELOAD_MASTERY,						// ���ε� �����͸�
		SI_P_CHG_CONVERTED_WARHEAD,						// ź�� ����
		SI_P_CHG_CANNONEER_WITHSTANDING	= 4415,			// ������ ��Ƽ��
#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN

#ifdef SERV_CHUNG_TACTICAL_TROOPER
		SI_P_CTT_MOBILE_ARMORED_MODE	= 4416,			// �̵� ����
		SI_P_CTT_REACTIVE_ARMOR			= 4417,			// ���� �尩
#endif //SERV_CHUNG_TACTICAL_TROOPER

#ifdef SERV_ARA_MARTIAL_ARTIST
		SI_SA_ARA_WIND_WEDGE			= 4500,		/// ��ǳ��_�ٶ� ����_���� 2��
		SI_SA_ARA_SHADOW_RISING_JUMP,				/// ���·�_õ�� �ɸ���
		SI_SA_ARA_PULLING_PIERCE,					/// õ��_���� �α�
		SI_SA_ARA_TEMPEST_DANCE,					/// ��ǳ_�ָ�
		SI_SA_ARA_FALLING_DRAGON,					/// ������_���� ������
		SI_SA_ARA_SHADOW_KNOT,						/// �῵_�׸��� �ŵ�_�ͻ� 3��
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
		SI_SA_ALH_DOUBLE_COLLISION,					/// ��� 3�� - �ȱ���
		SI_SA_ALH_MOONLIGHT_SLASH,					/// ��� 4�� - �޺� ����
		SI_SA_ALH_PEERLESSNESS_SLASH,				/// ������
		SI_SA_ALH_WHITE,TIGER,						/// ��ȣ�� ��
		SI_SA_ALH_SWALLOW_ASSAULT,					/// ��
		SI_SA_ASD_TURBULENT_WAVE,					/// ��ȣ 4�� - �뵵
		SI_SA_ASD_SUPPRESSION,						/// ����
#endif
		
		SI_A_ARA_TIGER_CLAW				= 4700,		/// ȣ����_����_��ȣ 2��
		SI_A_ARA_FORCE_SPEAR,						/// ���â
		SI_A_ARA_GAIN_FORCE,						/// ���
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
		SI_A_ALH_ROCK_CRASH,						/// ��ȣ 1�� - ��������
		SI_A_ALH_BREAKING,							/// ��ȭ����
		SI_A_ASD_LOW_BRANDISH,						/// ��ȣ 3�� - ����
#endif

		SI_P_ARA_FORCE_SPEAR_MASTERY	= 4900,		/// ���â ����
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
		SI_P_ALH_QUICK_STAND,						/// ����� ���
		SI_P_ALH_VIGOR_ENERGIZE,					/// ��� Ȱ��ȭ
		SI_P_ALH_FILLED_POWER,						/// �游�� ��
		SI_P_ASD_QUICKEN_GUARD,						/// �����̴� ����
		SI_P_ASD_BRUTAL_PIERCING,					/// â���� ����
#endif

#endif

//#ifdef GUILD_SKILL
		SI_GP_COMMON_BALANCED_BODY					= 5000,		//�������� ��ü
		SI_GP_COMMON_HARMONIC_SPIRIT				= 5010,		//��ȭ�ο� ��ȥ
		SI_GP_COMMON_CHEERING_OF_TEAM				= 5020,		//������ ����
		SI_GP_COMMON_MASTER_OF_ALCHEMIST			= 5030,		//���ݼ��� ����
		SI_GP_COMMON_CONCENTRATION_OF_SPIRIT		= 5040,		//���� ����
		SI_GP_COMMON_CONCENTRATION_MAGICAL_POWER	= 5050,		//���ߵ� ����
		SI_GP_COMMON_VIOLENT						= 5060,		//�ݳ�
		SI_GP_COMMON_LIBERATION_OF_ANGER			= 5070,		//�г� �ع�
		// ������� 2Ʈ��
		SI_GP_COMMON_INFINITE_STRENGTH				= 5080,		//������ ü��
		SI_GP_COMMON_CHANCE_TO_REVERSE				= 5090,		//������ ��ȸ
		SI_GP_COMMON_SACRIFICE_OF_HERO				= 5100,		//������ ���
		SI_GP_COMMON_MIDAS							= 5110,		//���̴����� ��
		SI_GP_COMMON_SUPPORT_AURA					= 5120,		//��� ȸ��
		SI_GP_COMMON_IMPROVE_CHANCE					= 5130,		//��ȸ ����
		SI_GP_COMMON_ESCAPE_FROM_CRISIS				= 5140, 	//���� Ż��
		SI_GP_COMMON_INSTRUCTION_OF_WISEMAN			= 5150,		//������ ����ħ
//#endif GUILD_SKILL

//#ifdef ADDED_RELATIONSHIP_SYSTEM
		SI_ETC_WS_COMMON_LOVE						= 6001,		/// Ŀ�� ��ų õ������
//#endif // ADDED_RELATIONSHIP_SYSTEM
*/
#endif //SERV_UPGRADE_SKILL_SYSTEM_2013

		// Elesis
		SI_SA_EL_WILD_SHOCK							= 8000,		/// ���ϵ� ��ũ
		SI_SA_EL_POWER_BURSTER						= 8001,		/// �Ŀ� ������
		SI_SA_EL_MEGA_BURSTER						= 8002,		/// �ް� ������
		SI_SA_EL_UNLIMITED_BLADE					= 8003,		/// �𸮹�Ƽ�� ���̵�

		SI_SA_SSK_SPIRAL_BLAST						= 8004,		/// �����̷� ����Ʈ
		SI_SA_SSK_EXTINCTION						= 8005,		/// ���̹� - �ͽ��ü�
		SI_SA_SSK_VICTORIOUS_SWORD					= 8006,		/// �¸��� ��
		SI_SA_SSK_JUDGEMENT_DRIVE					= 8007,		/// ������Ʈ ����̺�

		SI_SA_SPK_BURST_RISING						= 8008,		/// ����Ʈ ����¡
		SI_SA_SPK_BURST_WAVE						= 8009,		/// ����Ʈ ���̺�
		SI_SA_SPK_SWORDFIRE							= 8010,		/// �ҵ� ���̾�
		SI_SA_SPK_INFERNAL_BLADE					= 8011,		/// ���丣�� ���̵�
		SI_SA_SPK_ETERNAL_FIRE						= 8012,		/// ���ͳ� ���̾�
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
		SI_SA_SGM_SONIC_BLADE						= 8013,		/// �Ҵ� ���̵�
		SI_SA_SGM_CRITICAL_DIVE						= 8014,		/// ũ��Ƽ�� ���̺�
		SI_SA_SBH_SCALDIC_SWORD						= 8015,		/// ��Į�� �ҵ�
		SI_SA_SBH_WALL_OF_PROMINENCE				= 8016,		/// ���̾� ��
		SI_SA_SGM_JUGGERNAUT_BUSTER					= 8017,		/// ���ų� ������
		SI_SA_SGM_WAR_PRELUDE						= 8018,		/// �� ��������
		SI_SA_SBH_BLAZE_WING						= 8019,		/// ������ ��	
		SI_SA_SBH_BLAZING_DANCE						= 8020,		/// ����¡ ��
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

		SI_A_EL_DODGE_AND_SLASH						= 8200,		/// ����&������
		SI_A_EL_LEAP_CRASH							= 8201,		/// ����
		SI_A_EL_RUSHING_SWORD						= 8202,		/// ���� �ҵ�
		SI_A_EL_KICK								= 8203,		/// ������
		SI_A_EL_MEGA_SLASH							= 8204,		/// �ް� ������
		SI_A_EL_SONIC_ASSAULT_STAB					= 8205,		/// �Ҵ� �Ʈ - ����
		SI_A_EL_SONIC_ASSAULT_STING					= 8206,		/// �Ҵ� �Ʈ - ����

		SI_A_EL_ENDURANCE_POWER						= 8207,		/// �ر� - ��

		SI_A_SSK_CROSS_SLASH						= 8208,		/// ũ�ν� ������
		SI_A_SSK_POWER_BREAK						= 8209,		/// �Ŀ� �극��ũ
		SI_A_SSK_HEAVY_STUNNER						= 8210,		/// ��� ���ͳ�

		SI_A_SPK_SWORD_ERUPTION						= 8211,		/// �ҵ� �̷���
		SI_A_SPK_SCALDIC_SWORD						= 8212,		/// �Ҳ��� ��
		SI_A_SPK_BIG_BURST							= 8213,		/// ������
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
		SI_A_SGM_PROVOKE							= 8214,		/// ����
		SI_A_SGM_DRASTIC_CHARGE						= 8215,		/// �巡��ƽ ����
		SI_A_SGM_SNAP_CRASHER						= 8216,		/// ���� ũ����
		SI_A_SBH_FLAME_SWORD						= 8217,		///	ȫ���� ��
		SI_A_SBH_FIREWORKS							= 8218,		/// �ҳ���
		SI_A_SBH_SCALET_ROSE						= 8219,		/// �Ҳ� ���
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

		SI_P_SSK_AGILITY_MASTERY					= 8400,		/// ���߷� ���
		SI_P_SSK_CHIVALRY							= 8401,		/// ����� ����
		SI_P_SPK_CONCENTRATION_MASTERY				= 8402,		/// ���߷� ���
		SI_P_SPK_BODY_OF_FIRE						= 8403,		/// ���� ��ü
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
		SI_P_SGM_ADVANCED_KNIGHT_MASTERY			= 8404,		/// ��� ��� ����
		SI_P_SGM_WALTZ_OF_SWORD						= 8405,		/// ���� ����
		SI_P_SGM_FIRST_STRIKE						= 8406,		/// �����ʽ�
		SI_P_SBH_FIRE_BLOSSOMS						= 8407,		/// �Ҳ� ��ȭ
		SI_P_SBH_UNEXTINGUISHABLE_FIRE				= 8408,		/// ������ �ʴ� �Ҳ�
		SI_P_SBH_STIGMA_OF_FIRE						= 8409,		/// �Ҳ��� ����	

		SI_HA_SGM_SWORD_OF_RELICS					= 8600,		/// �ҵ� ���� ������
		SI_HA_SBH_ANCIENT_FIRE						= 8601,		/// ���μ�Ʈ ���̾�
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE	


		// Add
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
		SI_SA_AN_VOID_BREAKER						= 9000,		/// ���̵� �극��Ŀ
		SI_SA_AN_PARTICLE_PRISM						= 9001,		/// ��ƼŬ ������
		SI_SA_AN_ENERGY_BOOM						= 9002,		/// ������ ��
		SI_SA_AN_PARTICLE_ACCELERATOR				= 9003,		/// ��ƼŬ �׼���������
		SI_SA_AN_NEUTRON_BOMB						= 9004,		/// ��Ʈ�� ��

		SI_SA_APT_PULSE_CANNON						= 9005,		/// �޽� ĳ��
		SI_SA_APT_EMP_SHOCK							= 9006,		/// EMP ��ũ
		SI_SA_APT_QUAKE_BUSTER						= 9007,		/// ����ũ ������
		SI_SA_APT_STASIS_FIELD						= 9008,		/// �����̽ý� �ʵ�
		SI_SA_APT_CONQUEROR							= 9009,		/// ��Ŀ��

	#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
		SI_SA_ALP_REVERSE_REACTOR					= 9010,		/// ������ ������
		SI_SA_ALP_QUICKSILVER_ACCEL					= 9011,		/// ���ǹ� �׼�
		SI_SA_ALP_QUICKSILVER_FRENZY				= 9012,		/// ���ǹ� ������
		SI_SA_ALP_DUST_ANATOMY						= 9013,		/// ����Ʈ �Ƴ����
		SI_SA_ALP_PSYCHIC_STORM						= 9014,		/// ����ű ����
	#endif //SERV_ADD_LUNATIC_PSYKER


		SI_A_AN_PULSE_BULLET						= 9200,		/// ����ź
		SI_A_AN_MIND_BREAK							= 9201,		/// ���� �ر�
		SI_A_AN_PHASE_SHIFT							= 9202,		/// ���� ��ȭ

		SI_A_APT_MAGNETRON_DYNAMO					= 9203,		/// ���̳ʸ� ���� - ���׳�Ʈ��
		SI_A_APT_DESOLVER_DYNAMO					= 9204,		/// ���̳ʸ� ���� - ������
		SI_A_APT_PYLON_DYNAMO						= 9205,		/// ���̳ʸ� ���� - ���Ϸ�

	#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
		SI_A_ALP_PSIONIC_BEAT						= 9206,		/// ���̿��� ��Ʈ
		SI_A_ALP_TWIRL_RUSH							= 9207,		/// Ʈ�� ����
	#endif //SERV_ADD_LUNATIC_PSYKER


		SI_P_AN_CHARGED_IMPULSAR					= 9400,		/// ���̳ʸ� ���� - ��������
		SI_P_AN_RESEARCH_DYNAMO_DISSOLUTION			= 9401,		/// ���̳ʸ� ���� - ��ü
		SI_P_AN_RESEARCH_DYNAMO_COMPOSITION			= 9402,		/// ���̳ʸ� ���� - ����
		SI_P_APT_NASOD_ARMOR_MODE					= 9403,		/// ���ҵ� �Ƹ� ���

		SI_P_APT_LIBRARY_OF_LIMITLESS				= 9404,		/// ������ ������
		SI_P_APT_DUST_STORM							= 9405,		/// �̸��� ��ǳ

	#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
		SI_P_ALP_BODY_OF_TRANSCENDENCE				= 9406,		/// �ʿ����� ��ü
		SI_P_ALP_POWER_EXCHANGER					= 9407,		/// ��� ��ȯ��
		SI_P_ALP_FANTASY_TRACER						= 9408,		/// ȯ�� ������
	#endif //SERV_ADD_LUNATIC_PSYKER


	#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
		SI_HA_ALP_DOOMS_DAY							= 9600,		/// �ҽ� ����
	#endif //SERV_ADD_LUNATIC_PSYKER
#endif //SERV_9TH_NEW_CHARACTER

	}; 

	enum SKILL_ABILITY_TYPE
	{
		SA_INVALID,															
		SA_EFFECTIVE_TIME,					/// ���� �ð�												
		SA_EFFECTIVE_COUNT,					/// ���� Ƚ��
		SA_SKILL_ID,						/// Ư�� ��ų ����
		SA_COMBO_ID,						/// Ư�� �޺� ����
		SA_EFFECTIVE_RATE,					/// �ߵ� Ȯ��
		SA_ATK_REL,							/// ���ݷ�(����,����) ���� ����
		SA_ATK_PHYSIC_REL,					/// ���� ���ݷ� ���� ����
		SA_ATK_MAGIC_REL,					/// ���� ���ݷ� ���� ����
		SA_DEF_PHYSIC_REL,					/// ���� ���� ���� ����
		SA_DEF_MAGIC_REL,					/// ���� ���� ���� ����
		SA_TEAM_ATK_PHYSIC_REL,				/// ���� ���ݷ� ���� ���� (��)
		SA_TEAM_ATK_MAGIC_REL,				/// ���� ���ݷ� ���� ���� (��)
		SA_TEAM_DEF_PHYSIC_REL,				/// ���� ���� ���� ���� (��)
		SA_TEAM_DEF_MAGIC_REL,				/// ���� ���� ���� ���� (��)
		SA_DAMAGE_REL,						/// ������ ���� ����(�нú� ��ų���� ���� ���)
		SA_DAMAGED_REL,						/// �޴� ������ ����
		SA_UNFIXED_DEFENSE_REL,				/// �� ����
		SA_SUMMON_ATK_REL,					/// ��ȯ�� ���ݷ� ����
		SA_MAX_HP_ABS,						/// �ִ� HP ���� (��ġ)
		SA_MAX_HP_REL,						/// �ִ� HP ���� (%)
		SA_MAX_MP_ABS,						/// �ִ� MP ���� (��ġ)
		SA_MAX_MP_REL,						/// �ִ� MP ���� (%)
		SA_TELEPORT_RANGE_ABS,				/// ĳ���� �̵���ų �Ÿ� ����
		SA_CHARGING_TIME,					/// ������ �ð�
		SA_HP_REL_TO_MP_ABS_1,				/// ������ȯ ù��° param (HP�� %)
		SA_HP_REL_TO_MP_ABS_2,				/// ���� ��ȯ �ι�° param (MP�� ��ġ)
		SA_MP_CONSUME_REL,					/// ���� �Ҹ� ����
		SA_MP_GAIN_ON_HIT_REL,				/// (Ÿ�ݽ�)���� ȹ�淮 ����
		SA_SCOPE_ABS,						/// ��ų���� ���� (�ݰ�,��ġ)
		SA_PROJECTILE_FIRE_COUNT,			/// �߻�ü ���� ����
		SA_PROJECTILE_RANGE_REL,			/// �߻�ü �Ÿ� ����
		SA_PROJECTILE_SIZE,					/// �߻�ü ������ ����
		SA_PROJECTILE_PIERCING,				/// �߻�ü ��Ʈ �� (�����) ����
		SA_MOVE_SPEED,						/// �̵� �ӵ� ����
		SA_JUMP_SPEED,						/// ���� �ӵ� ����
		SA_ATK_PHYSIC_TO_ATK_MAGIC,			/// ���� ���ݷ��� ������ �������ݷ�
		SA_ATK_MAGIC_TO_ATK_PHYSIC,			/// ���� ���ݷ��� ������ �������ݷ�
		SA_READY_TIME_REL,					/// ������ �غ� �ð� ����
		SA_FORCE_DOWN_REL,					/// �����ٿ� ��ġ ���� (%)
		SA_FORCE_DOWN_MELEE_REL,			/// �������� �����ٿ� ��ġ ���� (%)
		SA_FORCE_DOWN_RANGE_REL,			/// �߻�ü���� �����ٿ� ��ġ ���� (%)
		SA_FORCE_DOWN_ABS,					/// ��ųŸ�ݽ� �����ٿ� ��ġ ���� (��ġ)
		SA_MP_REGENERATION_ABS,				/// MP ȸ�� ������
		SA_MP_INTAKE_ABS,					/// MP ��Ż��
		SA_TARGET_ANI_SPEED,				/// ��� �ִ� �ӵ� ����
		SA_TARGET_MOVE_SPEED,				/// ��� �̵� �ӵ� ����
		SA_TARGET_JUMP_SPEED,				/// ��� ���� �ӵ� ����
		SA_TARGET_ATK_REL,					/// ��� ���ݷ� ����
		SA_HP_INTAKE_REL_DAMAGE,			/// �� ������ ��� HP �����(%)
		SA_ATK_CRITICAL_DAM_ABS,			/// �����ڰ� �ִ� ũ��Ƽ�� ������ ����
		SA_ATK_CRITICAL_RATE_ABS,			/// �����ڰ� �ִ� ũ��Ƽ�� ����
		SA_DEF_CRITICAL_DAM_ABS,			/// �ǰ��ڰ� �޴� ũ��Ƽ�� ������ ����
		SA_DEF_CRITICAL_RATE_ABS,			/// �ǰ��ڰ� �޴� ũ��Ƽ�� ����		// kimhc // 2011-04 �� ��ġ�� û ĳ�� ��ų
		SA_BACK_SPEED_X,					/// ���ݿ� ���� x�������� �з����� ��
		SA_ITEM_SPECIAL_ABILITY_REL,		/// HP/MP ������ ����� ȸ���� ����(%)
		SA_MP_GAIN_GET_HIT_REL,				/// (�ǰݽ�)���� ȹ�淮 ���� (%) 
		SA_SOUL_GAIN_ON_HIT_REL,			/// (Ÿ�ݽ�)�ҿ� ȹ�淮 ���� (%) 
		SA_SOUL_GAIN_GET_HIT_REL,			/// (�ǰݽ�)�ҿ� ȹ�淮 ���� (%) 
		SA_HP_GAIN_REL_MAX_HP,				/// ĳ������ MAX HP�� %�� �ش��ϴ� HP ȸ��
		SA_ED_GAIN_REL,						/// ED ���淮 ����
		SA_ATK_PHYSIC_PLUS_ATK_MAGIC_REL,	/// (���� ���ݷ� + ���� ���ݷ�)�� %������
		SA_EXP_GAIN_REL,					/// EXP ���淮 ����
		SA_DAMAGE_ABSORB_REL_MAX_HP,		/// ĳ������ MAX HP�� %�� �ش��ϴ� ������ ���


		SA_STRONG_MIND_ACTIVE_REL,			/// ??
		SA_STRONG_MIND_SPECIAL_ACTIVE_REL,	/// ??
		SA_MP_RETAKE_ABS,					/// ���� ȸ��

		SA_RESIST_FIRE_ABS,					/// �� ����
		SA_RESIST_WATER_ABS,				/// �� ����
		SA_RESIST_GREEN_ABS,				/// �ڿ� ����
		SA_RESIST_WIND_ABS,					/// �ٶ� ����
		SA_RESIST_LIGHT_ABS,				/// �� ����
		SA_RESIST_DARK_ABS,					/// ��� ����

		SA_ATK_MAGIC_FIRST_REL,				/// ù��° ����
		SA_ATK_MAGIC_SECOND_REL,			/// �ι�° ����
		SA_ATK_MAGIC_THIRD_REL,				/// ����° ����

		SA_REMOTE_SPEED_ABS,				/// �ӵ� ����

		SA_DAMAGE_TYPE_CHANGE,				/// ������ ���� ����
		SA_DAMAGE_TIME,						/// ������ �ð�

//{{ ����� : 2010.11.12
//#ifdef NEW_SKILL_2010_11
		SA_RECOVER_MP_ABS, 
		SA_TARGET_MANA,
//#endif NEW_SKILL_2010_11
//}} ����� : 2010.11.12		
		SA_TARGET_MANA_REL,					/// Ÿ�� ���� ���� ����, JHKang
		SA_SIZE_ABS,						/// ���� ũ�� ��, JHKang
		SA_REFLECT_REL,						/// ���� �ݻ� Ȯ��
		SA_EL_DEFENCE_ABS,					/// ��� �Ӽ� ����
		SA_TARGET_HP_REL,					/// ��� HP �ۼ�Ʈ ȸ��
		SA_TARGET_HITRATE_REL,				/// EDT BLIND�� ����
		//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
		SA_CHARGE_CANNON_BALL,				/// ĳ�� ������
		//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
		
		//{{ kimhc // 2011.1.14 // û 1�� ����
		SA_FIRE_DEFENCE_ABS,				/// ���� â�� ��
		SA_WATER_DEFENCE_ABS,				/// ���� â�� ��
		SA_NATURE_DEFENCE_ABS,				/// ���� â�� �ڿ�(�׸�)
		SA_WIND_DEFENCE_ABS,				/// ���� â�� �ٶ�
		SA_LIGHT_DEFENCE_ABS,				/// ���� â�� ��
		SA_DARK_DEFENCE_ABS,				/// ���� â�� ���

		SA_EDT_FIRE_RESIST_REL,				/// ȭ�� ���� ����
		SA_EDT_FROZEN_RESIST_REL,			/// ���� ���� ����
		SA_EDT_ICE_RESIST_REL,				/// ���� ���� ����
		SA_EDT_POISON_RESIST_REL,			/// �� ���� ����
		SA_EDT_Y_PRESSED_RESIST_REL,		/// ������ ���� ����
		SA_EDT_LEG_WOUND_RESIST_REL,		/// ��ó ���� ����
		SA_EDT_STUN_RESIST_REL,				/// ���� ���� ����
		SA_EDT_CURSE_RESIST_REL,			/// ���� ���� ����

		SA_EXPAND_CB,						/// ĳ�� Ȯ�� ��
		SA_ANI_SPEED,						/// �ִϸ��̼� �ӵ� ����
		//}} kimhc // 2011.1.14 // û 1�� ����

		//{{ kimhc // 2011-04-04 // 2011-04 �� ��ġ�� û ĳ�� ��ų
		SA_ACCURACY_PERCENT_ABS,			/// ���߷�
		//}} kimhc // 2011-04-04 // 2011-04 �� ��ġ�� û ĳ�� ��ų
		SA_CRITICAL_EVASION_REL,
//#ifdef CHUNG_SECOND_CLASS_CHANGE
		SA_DECREASE_REL,					/// ���� ����
//#endif
//#ifdef ELSWORD_WAY_OF_SWORD
		SA_WAY_OF_SWORD_TYPE,				/// ���ҵ� ����, �˻��� ��		
//#endif ELSWORD_WAY_OF_SWORD
//#ifdef ELSWORD_SHEATH_KNIGHT
		SA_ATK_ADD_DAMAGE_FIRST_ABS,
		SA_ATK_ADD_DAMAGE_SECOND_ABS,
		SA_ATK_ADD_DAMAGE_THIRD_ABS,
		SA_ATK_ADD_DAMAGE_FOURTH_ABS,
		SA_ATK_ADD_DAMAGE_FIFTH_ABS,
		SA_TELEPORT_SPEED_REL,
		HP_CONSUME_REL,
		SA_MP_BURN_ABS,
//#endif ELSWORD_SHEATH_KNIGHT
//#ifdef	SERV_TRAPPING_RANGER_TEST	//	����Ż��Ƽ �������� Ȯ��
		SA_SKILLDAMAGE_MULTIPLE_01,
		SA_SKILLDAMAGE_MULTIPLE_02,
		SA_SKILLDAMAGE_MULTIPLE_03,
		SA_SKILLDAMAGE_MULTIPLE_04,

		SA_SKILLDAMAGE_MULTIPLE_01_RATE,
		SA_SKILLDAMAGE_MULTIPLE_02_RATE,
		SA_SKILLDAMAGE_MULTIPLE_03_RATE,
		SA_SKILLDAMAGE_MULTIPLE_04_RATE,
//#endif	SERV_TRAPPING_RANGER_TEST
//#ifdef SERV_RAVEN_WEAPON_TAKER
		SA_EVASION_REL,
		SA_REDUCE_EDT_TIME_REL,
		SA_OVERHEAT_HP,
//#endif SERV_RAVEN_WEAPON_TAKER
//#ifdef EVE_ELECTRA
		SA_SCOPE_ABS_HYPER,
		SA_EFFECTIVE_TIME_HYPER,
		SA_AIR_MOVE_COUNT,
//#endif EVE_ELECTRA
//#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
		SA_NOT_CANNONBALL_REL,
		SA_INSERT_CANNONBALL_REL,
		SA_BE_SHOT_CANNONBALL_REL,
		SA_CANNONBALL_DAMAGE_UP_REL,
		SA_FORCE_DOWN_REL_DAMAGE_RATE_BASE,
		SA_SIEGE_SHELLING_REL,
		SA_EFFECTIVE_DAMAGED_ATTACK_BASE,
//#endif //SERV_ADD_CHUNG_SHELLING_GUARDIAN
//#ifdef SERV_CHUNG_TACTICAL_TROOPER
		SA_FIRST_HYPER_EFFECTIVE,
		SA_SECOND_HYPER_EFFECTIVE,
		SA_THIRD_HYPER_EFFECTIVE,
//#endif SERV_CHUNG_TACTICAL_TROOPER
//#ifdef BALANCE_CODE_NEMESIS_20121213
		SA_ATK_CRITICAL_RATE_ATK_BASE,			/// ���ݷ��� % ��ŭ �߰��Ǵ� ũ��Ƽ�� ����
//#endif //BALANCE_CODE_NEMESIS_20121213
		// X2SkillTree.h���� �߰����ּ���~
	};			
	
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
	struct SkillTemplet
	{
		enum SKILL_LEVEL_VALUE
		{
			SLV_MASTER_LEVEL	= 20,
			SLV_MAX_LEVEL		= 30,
		};

		SKILL_TYPE				m_eType;						// ��ų Ÿ��
		int						m_iSkillID;						// ��ų ID
		int						m_iRequireLearnSkillPoint;		// ��ų ���ʽ��� �� �ʿ� sp
		int						m_iRequireUpgradeSkillPoint;	// ��ų ��ȭ �� �ʿ� sp
		bool					m_bBornSealed;					// ������ �ִ� ��ų����
		bool					m_bGuildSkill;
		
		// <��ų ������ ���� �ϱ� ���ϰ� �����̳ʷ� ������>		
		// ****�����̳ʰ� ��� ���� �ʴٸ� ������ 30��(Lv30)�� �Ǿ�� �Ѵ�.***
		std::vector< int >										m_vecRequireCharactorLevel;		// ��ų �ܰ躰 �ʿ� ĳ���� ����
		std::vector< CXSLStat::Stat >							m_vecStat;						// ��ų �ܰ躰 stat ����ġ
		
		std::map< SKILL_ABILITY_TYPE, std::vector< float > >	m_mapSkillAbility;				// ��ų �ܰ躰 SA ȿ��

		const float GetSkillAbilityValue( IN CXSLSkillTree::SKILL_ABILITY_TYPE eType, IN int iSkillLevel ) const 
		{
			if( true == m_mapSkillAbility.empty() )
				return 0.f;

			std::map< SKILL_ABILITY_TYPE, std::vector< float > >::const_iterator it = m_mapSkillAbility.find( eType );
			if( m_mapSkillAbility.end() == it )
				return 0.f;

			if( it->second.size() != SLV_MAX_LEVEL )
				return 0.f;

			if( iSkillLevel > SLV_MAX_LEVEL )
				iSkillLevel = SLV_MAX_LEVEL;

			return (float)it->second[iSkillLevel-1];
		}

		SkillTemplet()
		{
			m_eType						= ST_NONE;
			m_iSkillID					= 0;
			m_iRequireLearnSkillPoint	= 0;
			m_iRequireUpgradeSkillPoint	= 0;
			m_bBornSealed				= false;
			m_bGuildSkill				= false;
			m_vecRequireCharactorLevel.clear();
			m_vecStat.clear();
			m_mapSkillAbility.clear();
		}
	};

	struct SkillTreeTemplet
	{
		int						m_iUnitClass;
		int						m_iMasterSkillLevel;		// ������ ��� �� �ִ� �ְ� ����
		int						m_iTier;					// ��ųƮ������ ������ ��ġ, ���η� �� ��° row���� 0���� ����
		int						m_iColumn;						// ��ųƮ������ ������ ��ġ, ���η� �� ��° column���� 0���� ����
		int						m_iPrecedingSkill;			// ���� ��ų ID

		std::vector<int>		m_vecFollowingSkill;		// ��ų �ǵ����� �� �� �ʿ�

		SkillTreeTemplet() :
		m_iUnitClass( CXSLUnit::UC_NONE ),
		m_iMasterSkillLevel( 0 ), 
		m_iTier( 0 ),
		m_iColumn( 0 ),
		m_iPrecedingSkill( 0 )
		{
		}
	};
	typedef std::map< int, SkillTreeTemplet >		SkillTreeTempletMap;			// < skill_id, SkillTreeTemplet >
	typedef std::map< int, SkillTreeTempletMap >	UnitSkillTreeTempletMap;		// < unit_class, SkillTreeTempletMap >
	//{{ 2009. 11. 26  ������	��彺ų
	typedef SkillTreeTempletMap						GuildSkillTreeTempletMap;		// < skill_id, SkillTreeTemplet >
	//}}

	private:
		CXSLSkillTree();

public:
	~CXSLSkillTree();

	bool OpenScriptFile( const char* pFileName );
	void OpenScriptFilePostProcess();

	// �Ϲݽ�ų
	bool AddSkillTemplet_LUA();
	bool AddSkillTreeTemplet_LUA();

	// ��彺ų
	bool AddGuildSkillTreeTemplet_LUA();

	// ���ν�ų
	bool AddSealSkillInfo_LUA();

	void AddSkillPowerRate_LUA();
	bool AddDefaultSkill_LUA();			/// ������ �⺻ ��ų ���̺� �Ľ� �Լ�

	bool IsExsitSkillID( int iSkillID ) { return ( m_setSkillIDList.find( iSkillID ) != m_setSkillIDList.end() ); }

	const SkillTreeTemplet* GetSkillTreeTemplet( const int iUnitClass, const int iSkillID );

	const SkillTemplet* GetSkillTemplet( const int iSkillID );
	int GetMasterSkillLevel( int iUnitClass, int iSkillID );
	int GetMaxSkillLevel( int iUnitClass, int iSkillID );
	int GetMasterGuildSkillLevel( int iGuildSkillID );
	bool LoadSkillTemplet_RequireCharactorLevel( KLuaManager& luaManager, SkillTemplet& skillTemplet );
	bool LoadSkillTemplet_Stat( KLuaManager& luaManager, SkillTemplet& skillTemplet );
	bool LoadSkillTemplet_SA( KLuaManager& luaManager, SKILL_ABILITY_TYPE eType, SkillTemplet& skillTemplet );


	// ��彺ų
	const SkillTreeTemplet* GetGuildSkillTreeTemplet( const int iGuildSkillID );

	//////////////////////////////////////////////////////////////////////////
	//BEGIN : SERVER ������ ���Ǵ� �Լ�(�������� ��ų����Ʈ�� �ֱ�����..)
	bool					GetCalcLevelUpIncreaseSkillPoint( IN u_char ucLevel, OUT int& iSkillPoint );	
	bool					GetCalcInitSkillPoint( IN u_char ucLevel, OUT int& iSkillPoint ) const;	
	bool					IsUnitTypeDefaultSkill( IN int iSkillID );
	bool					GetUnitClassDefaultSkill( IN char cUnitClass, OUT int& iSkillID1, OUT int& iSkillID2, OUT int& iSkillID3, OUT int& iSkillID4, OUT int& iSkillID5, OUT int& iSkillID6 );	
	//END
	//////////////////////////////////////////////////////////////////////////


	int TokenizeInt( WCHAR** ppStart, WCHAR* pEnd );

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 8. 4  ������		���ν�ų
	bool GetUnsealItemInfo( IN int iItemID, IN int iUnitClass, OUT std::vector<int>& vecSkillID );
	bool GetUnsealSkillItemList( IN int iSkillID, OUT std::vector< int >& vecItemList );
	bool IsUnsealItemID( int iItemID );
	//}}
	//////////////////////////////////////////////////////////////////////////

	//{{ 2009. 11. 9  ������	Ʈ���üӼ�
	static bool CheckUnitClassForUnsealSkill( IN char cUnitClass, IN char cUnsealSkillUnitClass );	
	//}}

	static float CalulateIncreaseingRate( float fRate_ );	/// kimhc // 1.05 (5%), 0.95 (-5%) ���� �������� 0.05, -0.05 ������ ��ȯ ���ִ� �Լ�

	//{{ 2012. 07. 24	�ڼ���	�ش� ĳ������ ��� ��ų�� �� ��� ġƮ
	const SkillTreeTempletMap* GetSkillTreeTemplet( const int iUnitClass );
	bool IsSealSkill( const int iSkillID );

	// Ŭ������, ��ų ����Ʈ
	void GetUnitClassSkillList( IN char cUnitClass, OUT std::map< int, SkillTreeTemplet >& mapSkillTreeList );
	bool GetCashSkillItemUnitClassType( IN int iItemID, OUT int& iUnitClassType );

private:

	std::set< int >						m_setSkillIDList;
	std::map<int, SkillTemplet>			m_mapSkillTemplet;

	UnitSkillTreeTempletMap				m_mapUnitSkillTreeTemplet;
	//{{ 2009. 11. 26  ������	��彺ų
	GuildSkillTreeTempletMap			m_mapGuildSkillTreeTemplet;
	//}}

	//{{ 2009. 8. 4  ������		���ν�ų����
	mutable	KncCriticalSection			m_csUnsealSkillItem;
	std::set< int >						m_setUnsealSkillItem;

	std::map< int, int >				m_mapUnsealSkillItem_UnitClassType;		// key : ItemID, value : UnitClassType (1�������̳�, 2�� �����̳�)

	std::map< int, std::set< int > >	m_mapUnsealSkillItem;	// key : ItemID, value : skillIDList
	std::map< int, std::vector< int > > m_mapUnsealSkill;		// key : SkillID, value : Unseal Item List
	//}}

	std::map< int, std::vector< int > > m_mapDefaultSkill;		// key : unitclass, value : skill list
	std::set< int >						m_setDefaultSkill;

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
	struct SkillTemplet
	{
		//{{ 2009. 11. 25  ������	��彺ų
		bool					m_bGuildSkill;
		//}}
		SKILL_TYPE				m_eType;
		int						m_iSkillID; 
		int						m_iSkillLevel;				// �� ��ų���ø��� ��ų ����
		float					m_fSkillCoolTime;
		float					m_fMPConsumption;			// MP �Ҹ�

		//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		USHORT					m_usCBConsumption;			/// ĳ�� �Ҹ�
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

		std::wstring			m_Name;
		std::wstring			m_Desc;

		CXSLStat::Stat			m_Stat;						// �����ʻ���� ��� stat ����ġ
		bool					m_bBornSealed;				// ������ �ִ� ��ų����

		//std::wstring			m_wstrTextureName;			// ��ų ������
		//std::wstring			m_wstrTexturePieceName;		// ��ų ������

		std::map< SKILL_ABILITY_TYPE, float > m_mapSkillAbility;

		const float GetSkillAbilityValue( CXSLSkillTree::SKILL_ABILITY_TYPE eType ) const 
		{
			//ASSERT( false == m_mapSkillAbility.empty() );
			if( true == m_mapSkillAbility.empty() )
				return 0.f;

			std::map< SKILL_ABILITY_TYPE, float >::const_iterator it = m_mapSkillAbility.find( eType );
			//ASSERT( m_mapSkillAbility.end() != it );
			if( m_mapSkillAbility.end() == it )
				return 0.f;

			return (float)it->second;
		}



		SkillTemplet() :
		//{{ 2009. 11. 25  ������	��彺ų
		m_bGuildSkill(false),
			//}}
			m_eType(ST_NONE ),
			m_iSkillID( 0 ),
			m_iSkillLevel( 0 ),
			m_fSkillCoolTime( 0.f ),
			m_fMPConsumption( 0.f ),
			//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
			m_usCBConsumption(0),			/// ĳ�� �Ҹ�
#endif	NEW_CHARACTER_CHUNG
			//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
			m_Name( L"" ),
			m_Desc( L"" ),
			m_bBornSealed( false )//,
			//m_wstrTextureName( L"" ),
			//m_wstrTexturePieceName( L"" )
		{
		}
	}; // struct SkillTemplet


	struct SkillKey
	{
		int 			m_iSkillID;			// skill id
		int				m_iSkillLevel;		// skill level

		SkillKey() :
		m_iSkillID( 0 ),
		m_iSkillLevel( 0 )
		{
		}

		SkillKey( int iSkillID, int iSkillLevel )
		{
			m_iSkillID = iSkillID;
			m_iSkillLevel = iSkillLevel;
		}

		bool operator < ( const SkillKey& rhs ) const 
		{
			if( (int)m_iSkillID < (int)rhs.m_iSkillID )
				return true;
			else if( (int)m_iSkillID > (int)rhs.m_iSkillID )
				return false;

			if( m_iSkillLevel < rhs.m_iSkillLevel )
				return true;

			return false;
		}
	};
	typedef std::map<SkillKey, SkillTemplet> SkillTempletMap;

	struct SkillTreeTemplet
	{
		int				m_iMaxSkillLevel;
		int				m_iTier;					// ��ųƮ������ ������ ��ġ, ���η� �� ��° row���� 0���� ����
		int				m_iIndexInTier;				// ��ųƮ������ ������ ��ġ, ���η� �� ��° column���� 0���� ����

		std::vector<int>		m_vecPrecedingSkill;		// ���� ��ų ID
		std::vector<int>		m_vecFollowingSkill;		// ��ų �ǵ����� �� �� �ʿ�

		SkillTreeTemplet() :
		m_iMaxSkillLevel( 0 ), 
		m_iTier( 0 ),
		m_iIndexInTier( 0 )
		{
		}
	};
	typedef std::map< int, SkillTreeTemplet >		SkillTreeTempletMap;			// < skill_id, SkillTreeTemplet >
	typedef std::map< int, SkillTreeTempletMap >	UnitSkillTreeTempletMap;		// < unit_class, SkillTreeTempletMap >
	//{{ 2009. 11. 26  ������	��彺ų
	typedef SkillTreeTempletMap						GuildSkillTreeTempletMap;		// < skill_id, SkillTreeTemplet >
	//}}

	//{{ 2009. 8. 4  ������		���ν�ų
	struct KUnsealItemInfo
	{
        int		m_iSkillID;
		char	m_cUnitClass;

		KUnsealItemInfo()		
		{
			m_iSkillID	 = 0;
			m_cUnitClass = CXSLUnit::UC_NONE;
		}
	};

private:
	CXSLSkillTree();

public:
	~CXSLSkillTree();

	bool OpenScriptFile( const char* pFileName );
	void OpenScriptFilePostProcess();

	// �Ϲݽ�ų
	bool AddSkillTemplet_LUA();
	bool AddSkillTreeTemplet_LUA();

	// ��彺ų
	bool AddGuildSkillTreeTemplet_LUA();

	// ���ν�ų
	bool AddSealSkillInfo_LUA( int iUnsealItemID, int iSkillID, char cUnitClass );

	bool IsExsitSkillID( int iSkillID ) { return ( m_setSkillIDList.find( iSkillID ) != m_setSkillIDList.end() ); }

	const SkillTreeTemplet* GetSkillTreeTemplet( const int iUnitClass, const int iSkillID );

	const SkillTempletMap& GetSkillTempletMap() const { return m_mapSkillTemplet; }
	const SkillTemplet* GetSkillTemplet( const int iSkillID, const int iSkillLevel );
	int GetMaxSkillLevel( int iUnitClass, int iSkillID );
	int GetMaxGuildSkillLevel( int iGuildSkillID );

	// ��彺ų
	const SkillTreeTemplet* GetGuildSkillTreeTemplet( const int iGuildSkillID );

	//////////////////////////////////////////////////////////////////////////
	//BEGIN : SERVER ������ ���Ǵ� �Լ�(�������� ��ų����Ʈ�� �ֱ�����..)
	bool					GetIncSkillPoint( IN u_char ucLevel, OUT int& iSkillPoint );	
	bool					IsUnitTypeDefaultSkill( IN int iSkillID );
	bool					GetUnitTypeDefaultSkill( IN char cUnitType, OUT int& iSkillID, OUT int& iSkillID2 );	
	//END
	//////////////////////////////////////////////////////////////////////////


	int TokenizeInt( WCHAR** ppStart, WCHAR* pEnd );

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 8. 4  ������		���ν�ų
	bool GetUnsealItemInfo( IN int iItemID, OUT int& iSkillID, OUT char& cUnitClass );
	bool GetUnsealSkillItemList( IN int iSkillID, OUT std::vector< int >& vecItemList );
	bool IsUnsealItemID( int iItemID );
	//}}
	//////////////////////////////////////////////////////////////////////////

	//{{ 2009. 11. 9  ������	Ʈ���üӼ�
	static bool CheckUnitClassForUnsealSkill( IN char cUnitClass, IN char cUnsealSkillUnitClass );	
	//}}

	static float CalulateIncreaseingRate( float fRate_ );	/// kimhc // 1.05 (5%), 0.95 (-5%) ���� �������� 0.05, -0.05 ������ ��ȯ ���ִ� �Լ�

	//{{ 2012. 07. 24	�ڼ���	�ش� ĳ������ ��� ��ų�� �� ��� ġƮ
#ifdef SERV_ADMIN_CHEAT_GET_ALL_SKILL
	const SkillTreeTempletMap* GetSkillTreeTemplet( const int iUnitClass );
	bool IsSealSkill( const int iSkillID );
#endif SERV_ADMIN_CHEAT_GET_ALL_SKILL
	//}}

private:
	
	std::set< int >						m_setSkillIDList;
	SkillTempletMap						m_mapSkillTemplet;

	UnitSkillTreeTempletMap				m_mapUnitSkillTreeTemplet;
	//{{ 2009. 11. 26  ������	��彺ų
	GuildSkillTreeTempletMap			m_mapGuildSkillTreeTemplet;
	//}}

	//{{ 2009. 8. 4  ������		���ν�ų����
	mutable	KncCriticalSection			m_csUnsealSkillItem;
	std::set< int >						m_setUnsealSkillItem;

	std::map< int, KUnsealItemInfo >	m_mapUnsealSkillItem;	// key : ItemID, value : UnsealItemInfo
	std::map< int, std::vector< int > > m_mapUnsealSkill;		// key : SkillID, value : Unseal Item List
	//}}

*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

};

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// �۾���¥: 2013-08-12	// �ڼ���
DefRefreshSingletonInline( CXSLSkillTree );
#else // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
DefSingletonInline( CXSLSkillTree );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2