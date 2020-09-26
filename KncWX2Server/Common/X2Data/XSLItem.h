#pragma once

#include "XSLUnit.h"

#define MAX_SPECIAL_ABILITY_NUM		 3
#define MAX_ENCHANT_CARD_NUM		10

class CXSLItem
{
	//{{ dmlee 2008.11.29 
	public: 
		static const int		MAX_MODEL_COUNT_A_ITEM = 6;
	//}} dmlee 2008.11.29 

	public:
		enum ITEM_TYPE
		{
			IT_NONE = 0,		//����
			IT_WEAPON,			//����
			IT_DEFENCE,			//��
			IT_ACCESSORY,		//�׼�����
			IT_SKILL,			//��ų
			IT_QICK_SLOT,		//������ ������(�Һ�)
			IT_MATERIAL,		//���
			IT_SPECIAL,			//Ư��			
			IT_QUEST,			//����Ʈ
			IT_OUTLAY,			//�����Һ�
			IT_ETC,				//��Ÿ
			//{{ 2010. 03. 23  ������	����� ��Ʈ
			IT_SKILL_MEMO,		//����ǳ�Ʈ
			//}}
			//#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
			IT_PET,				//��
			IT_RIDING,			//Ż��
			//#endif //SERV_UPGRADE_TRADE_SYSTEM
		};
		
		enum ITEM_GRADE
		{
			IG_NONE = 0,
			IG_UNIQUE,
			IG_ELITE,
			IG_RARE,
			IG_NORMAL,
			IG_LOW,
		};

		enum PERIOD_TYPE
		{
			PT_INFINITY = 0,	//������
			PT_ENDURANCE,		//������
			PT_QUANTITY,		//������
		};

		//{{ 2009. 8. 26  ������	�к� ������
		enum ITEM_STATE
		{
			IS_NORMAL = 0,
			IS_SEALED,
		};
		//}}

		enum SHOP_PRICE_TYPE
		{
			SPT_NONE = 0,		//�ȱ� �Ҵ�
			SPT_CASH,			//ĳ��
			SPT_GP,				//GP
		};

		enum USE_CONDITION
		{
			UC_NONE = 0,		// �ƹ��� ����� �� ����
			UC_ANYONE,			// �ƹ��� ����� �� ����
			UC_ONE_UNIT,		// ������ unit_type ��
			UC_ONE_CLASS,		// ������ unit_class ��
		};

		enum USE_TYPE
		{
			UT_NONE = 0,		// �����ϴ� ��� �ƴ�.
			UT_SKIN,			// �����ϴ� ���, ��Ų �ִϸ��̼�
			UT_ATTACH_ANIM,		// �����ϴ� ���, ����ġ �ִϸ��̼�
			UT_ATTACH_NORMAL,	// �����ϴ� ���, ����ġ �޽�
		};

		enum SPECIAL_SKILL_TYPE
		{
			SST_THROW_WOODEN_PIECE = 1,
			SST_MAGIC_SPEAR,
			SST_ICE_CRYSTAL,
			SST_FIRE_CRYSTAL,
			SST_MAGIC_BULLET,
			SST_POISON_POCKET,
		};

		enum SPECIAL_SKILL_MOTION_TYPE
		{
			SSMT_THROW = 1,		//������ ����
			SSMT_INSTALL,		//��ġ�ϴ� ����
		};

		enum SUPPLEMENT_ITEM_TYPE
		{
			SIT_PLUS_SUCCESS_RATE = 0,
			SIT_NOT_BROKEN,
		};

		//{{ 2010. 07. 01  ������	������ ����
		enum ENDURANCE
		{
			EN_MAX_ENDURANCE = 10000,
		};
		//}}

        enum SPECIAL_ABILITY_TYPE
        {
            SAT_NONE = 0,                    /// ����
            SAT_HP_UP,                        /// HP����
            SAT_MP_UP,                        /// MP����
            SAT_REMOVE_CURSE,                /// ��������
            SAT_REMOVE_SLOW,                /// ���ο�����
            SAT_REMOVE_FIRE,                /// ȭ������
            SAT_REMOVE_POISON,                /// ������
            SAT_RECIPE,                        /// ������ ������
            SAT_SUPPLEMENT_ITEM,            /// ������ ������
            SAT_HYPER_MODE,                    /// ����
            SAT_SOUL_GAGE_UP,                /// ���������� ���
            SAT_SHOW_OPPONENT_MP,            /// ����� MP ����
            SAT_UP_MP_AT_ATTACK_OR_DAMAGE,    /// �����ų� ������ MP ���
            SAT_HP_PERCENT_UP,                /// HP �ۼ�Ʈ ����
            SAT_TEAM_HP_UP,                    /// ������ ���� HP % ����
            SAT_TEAM_MP_UP,                    /// ������ ���� MP % ����
            SAT_PEPPER_RUN,                    /// ���߸԰� �ɸ�
            SAT_TRANSFORM_UNIT_SCALE,        /// ���� ������ ����
            SAT_SPECIAL_SKILL,                /// �Һ� �������� ����Կ� ���� Ư�� ���
            SAT_MP_PERCENT_UP,                /// MP �ۼ�Ʈ ����

            //#ifdef DUNGEON_ITEM
            SAT_POWER_RATE_UP,                /// ���ݷ� ���� (%)
            SAT_MOVE_SPEED_UP,                /// �̵� �ӵ� ���� (%)
            SAT_JUMP_SPEED_UP,                /// ���� �ӵ� ���� (%)
            SAT_MP_REGENERATION,            /// �ʴ� mp ȸ��
            SAT_USE_HYPER,                    /// �������� ���� ���� ����            
            SAT_PHYSIC_ATTACK_UP,            /// ���� ���ݷ� ���� (%)
            SAT_MAGIC_ATTACK_UP,            /// ���� ���ݷ� ���� (%)
            SAT_PHYSIC_DEFENCE_UP,            /// ���� ���� ���� (%)
            SAT_MAGIC_DEFENCE_UP,            /// ���� ���� ���� (%)
            SAT_SUPERARMOR,                    /// ���۾Ƹ�    
            SAT_SUMMON_SPIRIT,                /// ���ɼ�ȯ
            //#endif //DUNGEON_ITEM
            SAT_REMOVE_FROZEN,                // �ñ�����
            SAT_TRANSFORM_MONSTER,            // ���� ����            
            SAT_WAY_OF_SWORD_GAUGE_UP,        // ���Ǳ� ������ ����                
            //#ifdef SUMMON_MONSTER_CARD_SYSTEM
            SAT_SUMMON_MONSTER,                // ���� ��ȯ ���
            //#endif //SUMMON_MONSTER_CARD_SYSTEM
            SAT_FORCE_CRITICAL_MAX,            // ���� Ÿ�� ũ�� 100%
            SAT_ENABLE_ATTACK_MONSTER,         // �ؿ� �ҷ��� �̺�Ʈ
            //#ifdef ARA_FORCE_POWER_ITEM
            SAT_ARA_FORCE_POWER_PERCENT_UP,    // �ؿ� �ƶ� ��� ���� (%)
            //#endif ARA_FORCE_POWER_ITEM
			//#ifdef RETURN_TO_BASE_AT_PVE
			SAT_RETURN_TO_BASE_AT_PVE,			// PVE ��ȯ��
			//#endif // RETURN_TO_BASE_AT_PVE
			//#ifdef RIDINGPET_STAMINA_ITEM
			SAT_RIDINGPET_STAMINA_PERCENT_UP,    // �ؿ� ���̵��� ���׹̳� ���� (%)
			//#endif RIDINGPET_STAMINA_ITEM
            SAT_END,
        };

		//{{ 2008. 4. 22  ������  ĳ�� ������
		enum CASH_ITEM
		{
			CI_RESURRECTION_STONE 			= 127030, // ��Ȱ��
			CI_EXPAND_INVENTORY	  			= 200750, // ����ǰ Ȯ��
			CI_EXPAND_SKILL_SLOT  			= 200880, // ��ų����BȮ�� �Ⱓ��
			CI_EXPAND_SKILL_SLOT_PERMANENT  = 200885, // ��ų����BȮ�� ����������
			CI_EXPAND_SKILL_SLOT_ARA		= 261630, // ��ų����BȮ�� �ƶ� ���� �Ǹ� ĳ����
#ifdef SERV_CASH_ITEM_FOR_ELESIS	// ���볯¥: 2013-07-11
			CI_EXPAND_SKILL_SLOT_ELESIS		= 261661, // ��ų����BȮ�� �����ý� ���� �Ǹ� ĳ����
#endif	// SERV_CASH_ITEM_FOR_ELESIS

#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
#ifdef SERV_CASH_SKILL_POINT_TW
			CI_CASH_SKILL_POINT_30_7		= 60007810,	// 30SP 7�ϱ�
			CI_CASH_SKILL_POINT_60_7		= 60007811,	// 60SP 7�ϱ�
#endif SERV_CASH_SKILL_POINT_TW
			CI_CASH_SKILL_POINT_30_15		= 271076,	// 30SP 15�ϱ�
			CI_CASH_SKILL_POINT_60_15		= 271077,	// 60SP 15�ϱ�
			CI_CASH_SKILL_POINT_30_30		= 271078,	// 30SP 30�ϱ�
			CI_CASH_SKILL_POINT_60_30		= 271079,	// 60SP 30�ϱ�
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
			CI_CASH_SKILL_POINT_5			= 211340, // ĳ�� ��ų ����Ʈ 5
			CI_CASH_SKILL_POINT_10			= 211330, // ĳ�� ��ų ����Ʈ 10
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

			CI_SKILL_RESET_ITEM				= 200730, // ��ų �Ѱ� �ʱ�ȭ
			CI_SKILL_RESET_EVENT_ITEM		= 130808, // ��ų �Ѱ� �ʱ�ȭ
			CI_SKILL_RESET_EVENT_ITEM2		= 160947, // ��ų �Ѱ� �ʱ�ȭ
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP	
			EI_SKILL_RESET_EVENT_ITEM2		= 67003592, // ������ �˾�(��ų �Ѱ� �ʱ�ȭ)
			EI_SKILL_RESET_EVENT_ITEM3		= 67006745, // ������ �˾�(��ų �Ѱ� �ʱ�ȭ)
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP

			CI_SKILL_INIT_ITEM				= 203800, // ��ų ��ü �ʱ�ȭ
			CI_SKILL_INIT_EVENT_ITEM		= 130633, // ��ų ��ü �ʱ�ȭ �̺�Ʈ ������

#ifdef SERV_NEW_SKILL_INIT_EVENT_ITEM// �۾���¥: 2013-06-25	// �ڼ���
			CI_SKILL_INIT_EVENT_ITEM2		= 135416, // ��ų ��ü �ʱ�ȭ �̺�Ʈ ������2
			CI_SKILL_INIT_EVENT_ITEM3		= 135417, // ��ų ��ü �ʱ�ȭ �̺�Ʈ ������3
			CI_SKILL_INIT_EVENT_ITEM4		= 160946, // ��ų ��ü �ʱ�ȭ �̺�Ʈ ������4
#endif // SERV_NEW_SKILL_INIT_EVENT_ITEM

#ifdef SERV_GLOBAL_DEFINE
			CI_SKILL_INIT_EVENT_ITEM_JP		= 65000430, // ��ų ��ü �ʱ�ȭ �̺�Ʈ ������ ���� 130633
#endif //SERV_GLOBAL_DEFINE

#ifdef SERV_UNLIMITED_SKILL_RESET_ITEM
			CI_UNLIMITED_SKILL_RESET_ITEM	= 60008069, // ������ ���� ��ų �ʱ�ȭ �޴�
#endif //SERV_UNLIMITED_SKILL_RESET_ITEM

			CI_EXPAND_CHAR_SLOT_1 			= 206820, // ĳ���� ���� Ȯ�� 1
			CI_EXPAND_CHAR_SLOT_2 			= 206830, // ĳ���� ���� Ȯ�� 2
			CI_CHANGE_NICKNAME	  			= 209660, // ĳ���� �г��� ���� ī��
			CI_PREMIUM_SHOP		  			= 200740, // �����̾� ���λ���

			//{{ 2009. 11. 24  ������	��彺ų
			CI_GUILD_SKILL_RESET_ITEM		= 110857, // ������ �Ǹ�
			CI_GUILD_SKILL_INIT_ITEM		= 110858, // ������ ���Ǹ�
			//}}

			//{{ 2009. 8. 7  ������		����
			CI_BANK_MEMBERSHIP_SILVER		= 213460, // �ǹ� ȸ����
			CI_BANK_MEMBERSHIP_GOLD			= 213470, // ��� ȸ����
			CI_BANK_MEMBERSHIP_EMERALD		= 213480, // ���޶��� ȸ����
			CI_BANK_MEMBERSHIP_DIAMOND		= 213490, // ���̾Ƹ�� ȸ����
			CI_BANK_MEMBERSHIP_PLATINUM		= 213500, // �÷�Ƽ�� ȸ����
			CI_BANK_MEMBERSHIP_ONE_PACKAGE	= 213610, // �ں� �ſ� ������
			//}}

			//{{ 2011. 12. 13  ��μ�	�� ���� Ȯ�� ������
#ifdef SERV_EXPAND_QUICK_SLOT
#if defined(SERV_COUNTRY_TWHK) || defined(SERV_COUNTRY_JP)
			CI_EXPAND_QUICK_SLOT	        = 70002820, // ������ Ȯ���(�븸, �Ϻ�)
#else
			CI_EXPAND_QUICK_SLOT			= 244560, // ������ Ȯ���
#endif SERV_COUNTRY_TWHK
			CI_EXPAND_QUICK_SLOT_ARA		= 261650,	// ������ Ȯ���(�ƶ� ����)
#endif SERV_EXPAND_QUICK_SLOT
			//}}
#ifdef SERV_CASH_ITEM_FOR_ELESIS	// ���볯¥: 2013-07-11
			CI_EXPAND_QUICK_SLOT_ELESIS		= 261663,	// ������ Ȯ���(�����ý� ����)
#endif	// SERV_CASH_ITEM_FOR_ELESIS

			CI_NASOD_MEGAPHONE				= 212350, // �ް���
			CI_HIGH_NASOD_MEGAPHONE			= 212360, // ���� �ް���

			//{{ 2009. 9. 1  ������		�к� ������
			CI_SEAL_ITEM_NORMAL				= 214370, // �Ƿ��� �� ����(�Ͼ��)
			CI_SEAL_ITEM_RARE				= 214380, // �Ƿ��� �� ����(�����)
			CI_SEAL_ITEM_ELITE				= 214390, // �Ƿ��� �� ����(�����)
			CI_SEAL_ITEM_UNIQUE				= 214400, // �Ƿ��� �� ����(Ȳ�ݻ�)
			//}}

			//{{  2011.12.21     ��μ�    �ں� �ߵ��� �̺�Ʈ
#ifdef SERV_COBO_SEAL_ITEM_EVENT
#ifdef SERV_COUNTRY_JP
			CI_EVENT_SEAL_ITEM_NORMAL			= 70001900, // �Ƿ��� �� ����(�Ͼ��) ���� 160372
			CI_EVENT_SEAL_ITEM_RARE				= 70001901, // �Ƿ��� �� ����(�����)			
			CI_EVENT_SEAL_ITEM_ELITE			= 70001902, // �Ƿ��� �� ����(�����) ���� 160373
			CI_EVENT_SEAL_ITEM_UNIQUE			= 70001903, // �Ƿ��� �� ����(Ȳ�ݻ�)			
#else //SERV_COUNTRY_JP
			CI_EVENT_SEAL_ITEM_NORMAL				= 160372, // �ں� �Ƿ��� �� ����(�Ͼ��)
			CI_EVENT_SEAL_ITEM_ELITE				= 160373, // �ں� �Ƿ��� �� ����(�����)

#ifdef SERV_EVENT_SEAL_ITEM_UNIQUE// �۾���¥: 2013-05-08	// �ڼ���
			CI_EVENT_SEAL_ITEM_UNIQUE		= 160794, // [�ں�]�Ƿ��� �� ���� (Ȳ�ݻ�)
#endif // SERV_EVENT_SEAL_ITEM_UNIQUE
#endif //SERV_COUNTRY_JP

#endif SERV_COBO_SEAL_ITEM_EVENT
			//}}

			//{{ 2009. 9. 22  ������	����ĳ��
			CI_CHANGE_JOB_ELSWORD_KNIGHT	= 214410, // �ҵ� ����Ʈ�� ����
			CI_CHANGE_JOB_MAGIC_KNIGHT		= 214420, // ���� ����Ʈ�� ����
			CI_CHANGE_JOB_COMBAT_RANGER		= 214450, // �Ĺ� �������� ��ȥ
			CI_CHANGE_JOB_SNIPING_RANGER	= 214460, // �������� �������� ��ȥ
			CI_CHANGE_JOB_HIGH_MAGICIAN		= 214430, // ���� �������� �޸������
			CI_CHANGE_JOB_DARK_MAGICIAN		= 214440, // ��ũ �������� �޸������
			CI_CHANGE_JOB_SOUL_TAKER		= 214470, // �ҵ� ����Ŀ�� ���
			CI_CHANGE_JOB_OVER_TAKER		= 214480, // ���� ����Ŀ�� ���
			CI_CHANGE_JOB_EXOTIC_GEAR		= 214490, // �ڵ� ����ƽ�� �޸�
			CI_CHANGE_JOB_ARCHITECTURE		= 214500, // �ڵ� ��Ű��ó�� �޸�
			//}}
			//{{ 2009. 10. 24  ������	���ҵ�2������
			CI_CHANGE_JOB_LORD_KNIGHT		= 216420, // �ε� ����Ʈ �Ӹ���
			CI_CHANGE_JOB_RUNE_SLAYER		= 216430, // �� �����̾� ¡ǥ
			//}}
			//{{ 2009. 11. 23  ������	����2������
			CI_CHANGE_JOB_WIND_SNEAKER		= 217850, // ���� ����Ŀ ���� ĳ�� ������
			CI_CHANGE_JOB_GRAND_ARCHER		= 217860, // �׷��� ��ó ���� ĳ�� ������
			//}}
			//{{ 2009. 12. 11  ������	���̻� 2�� ����
			CI_CHANGE_JOB_ELMENTAL_MASTER	= 217920, // ������Ż ������ ���� ������
			CI_CHANGE_JOB_VOID_PRINCESS		= 217930, // ���̵� �������� ���� ������
			//}}
			//{{ 2010. 1. 20  �տ���	���̺�/�̺� 2�� ����
			CI_CHANGE_JOB_BLADE_MASTER		= 217950, // ���̵� ������ ���� ������
			CI_CHANGE_JOB_RECKLESS_FIST		= 217960, // ��ũ���� �ǽ�Ʈ ���� ������
			CI_CHANGE_JOB_NEMESIS			= 217970, // �ڵ� �׹̽ý� ���� ������
			CI_CHANGE_JOB_EMPRESS			= 217980, // �ڵ� �������� ���� ������
			//}}
			//{{ 2010. 04. 26  ������	����� ��Ʈ			
			CI_CASH_SKILL_NOTE_ITEM			= 221600,  // ĳ�� ����� ��Ʈ ������
			//}}

			//{{ kimhc // 2011.1.3 // û 1�� ����
			CI_CHANGE_JOB_FURY_GUARDIAN		= 229460,	// ǻ�� ������� ����
			CI_CHANGE_JOB_SHOOTER_GUARDIAN	= 229470,	// ���� ������� ����
			//}} kimhc // 2011.1.3 // û 1�� ����

			//{{ JHKang / ������ / 2011.6.13 / û 2�� ����
			CI_CHANGE_JOB_IRON_PALADIN		= 235120,	/// ���̾� �ȶ����
			CI_CHANGE_JOB_DEADLY_CHASER		= 235130,	/// ���鸮 ü�̼���
			//}}
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
			CI_UNLIMITED_CHANGE_SECOND_JOB			= 153000168,	//������ 2�� ���� ������
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

			//{{ 2011. 04. 14	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
			CI_PERSONAL_SHOP_AGENCY			= 233210,	// �븮 �Ǹ� ������
#endif SERV_PSHOP_AGENCY
			//}}

#ifdef SERV_ELSWORD_SHEATH_KNIGHT
			CI_CHANGE_JOB_SHEATH_KNIGHT		= 242310,	// �ý� ����Ʈ ���� ĳ�� ������
#endif SERV_ELSWORD_SHEATH_KNIGHT
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
			CI_CHANGE_JOB_BATTLE_MAGICIAN	= 242320,	// ��Ʋ ������ ���� ĳ�� ������
#endif SERV_ADD_ARME_BATTLE_MAGICIAN
#ifdef SERV_TRAPPING_RANGER_TEST
			CI_CHANGE_JOB_TRAPPING_RANGER	= 242330,	// Ʈ���� ������ ���� ĳ�� ������
#endif SERV_TRAPPING_RANGER_TEST
#ifdef SERV_RAVEN_WEAPON_TAKER
			CI_CHANGE_JOB_WEAPON_TAKER		= 242340,	// ���� ����Ŀ ���� ĳ�� ������
#endif SERV_RAVEN_WEAPON_TAKER
#ifdef SERV_EVE_ELECTRA
			CI_CHANGE_JOB_EVE_ELECTRA		= 242350,	// �Ϸ�Ʈ�� ���� ĳ�� ������
#endif SERV_EVE_ELECTRA
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
			CI_CHANGE_JOB_SHELLING_GUARDIAN	= 242360,	// ���� �����
#endif

#ifdef SERV_ELSWORD_INFINITY_SWORD
			CI_CHANGE_JOB_INFINITY_SWORD	= 252640,
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
			CI_CHANGE_JOB_DIMENSION_WITCH	= 252650,
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
			CI_CHANGE_JOB_NIGHT_WATCHER		= 252660,
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
			CI_CHANGE_JOB_VETERAN_COMMANDER	= 252670,
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
			CI_CHANGE_JOB_BATTLE_SERAPH		= 252680,
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
			CI_CHANGE_JOB_TACTICAL_TROOPER	= 252690,
#endif
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
			CI_CHANGE_JOB_LITTLE_HSIEN		= 264380,			/// �ƶ� �Ҽ� ���� ������
			CI_CHANGE_JOB_SAKRA_DEVANAM		= 264390,			/// �ƶ� ��õ ���� ������
#endif //SERV_ARA_CHANGE_CLASS_FIRST

#ifdef SERV_NEW_CHARACTER_EL
			CI_CHANGE_JOB_SABER_KNIGHT		= 264391,			/// �����ý� ���̹�����Ʈ ���� ������
			CI_CHANGE_JOB_PYRO_KNIGHT		= 264392,			/// �����ý� ���̷γ���Ʈ ���� ������
#endif //SERV_NEW_CHARACTER_EL

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
			CI_CHANGE_JOB_LITTLE_DEVIL		= 264393,			/// �ƶ� �Ҹ� ���� ������
			CI_CHANGE_JOB_YAMA_RAJA			= 264394,			/// �ƶ� ��� ���� ������
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â�� 
			CI_CHANGE_JOB_GRAND_MASTER		= 272169,			/// �����ý� �׷��� ������ ���� ������
			CI_CHANGE_JOB_BLAZING_HEART		= 272170,			/// �����ý� ����¡ ��Ʈ ���� ������
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
			CI_CHANGE_JOB_PSYCHIC_TRACER	= 272810,			/// �ֵ� ����ű Ʈ���̼� ���� ������
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
			CI_CHANGE_JOB_LUNATIC_PSYKER	= 273232,			/// �ֵ� �糪ƽ ����Ŀ ���� ������
#endif //SERV_ADD_LUNATIC_PSYKER


			//{{ 2011. 11. 30	������	��Ű�� ��ǰ �߰�
#ifdef SERV_ADD_PACKAGE_PRODUCT
#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
			CI_SKILL_POINT_30_USE_INVEN		= 241780,	// �κ��丮���� Ŭ���ؼ� ����ϴ� �׳�ý� �ູ
			CI_SKILL_POINT_60_USE_INVEN		= 241790,	// �κ��丮���� Ŭ���ؼ� ����ϴ� �׳�ý� �ູ
			CI_SKILL_POINT_60_USE_INVEN_ARA	= 261660,	// �׳�ý� �ູ(�ƶ�����)
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
			/*
			CI_SKILL_POINT_5_USE_INVEN		= 241780,	// �κ��丮���� Ŭ���ؼ� ����ϴ� �׳�ý� �ູ
			CI_SKILL_POINT_10_USE_INVEN		= 241790,	// �κ��丮���� Ŭ���ؼ� ����ϴ� �׳�ý� �ູ
			CI_SKILL_POINT_10_USE_INVEN_ARA	= 261660,	// �׳�ý� �ູ(�ƶ�����)
			*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013			
			CI_EXPAND_SKILL_SLOT_IN_PACKAGE	= 241800,	// ��ų ���� ü���� �޴�
#endif SERV_ADD_PACKAGE_PRODUCT
			//}}

			//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
			CI_CLASS_CHANGE_ELSWORD_KNIGHT			= 241950,
			CI_CLASS_CHANGE_ELSWORD_MAGIC_KNIGHT	= 241960,
			CI_CLASS_CHANGE_ELSWORD_SHEATH_KNIGHT	= 241970,

			CI_CLASS_CHANGE_AISHA_HIGH_MAGICIAN		= 241980,
			CI_CLASS_CHANGE_AISHA_DARK_MAGICIAN		= 241990,
			CI_CLASS_CHANGE_AISHA_BATTLE_MAGICIAN	= 242000,

			CI_CLASS_CHANGE_RENA_COMBAT_RANGER		= 242010,
			CI_CLASS_CHANGE_RENA_SNIPING_RANGER		= 242020,
			CI_CLASS_CHANGE_RENA_TRAPPING_RANGER	= 242030,

			CI_CLASS_CHANGE_REVEN_SOUL_TAKER		= 242040,
			CI_CLASS_CHANGE_REVEN_OVER_TAKER		= 242050,
			CI_CLASS_CHANGE_RAVEN_WEAPON_TAKER		= 242060,

			CI_CLASS_CHANGE_EVE_EXOTIC_GEAR			= 242070,
			CI_CLASS_CHANGE_EVE_ARCHITECTURE		= 242080,
			CI_CLASS_CHANGE_EVE_ELECTRA				= 242090,

			CI_CLASS_CHANGE_CHUNG_FURY_GUARDIAN		= 242100,
			CI_CLASS_CHANGE_CHUNG_SHOOTER_GUARDIAN	= 242110,
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
			CI_CLASS_CHANGE_CHUNG_SHELLING_GUARDIAN	= 242120,
#endif

			CI_CLASS_CHANGE_ELSWORD_LORD_KNIGHT		= 242130,
			CI_CLASS_CHANGE_ELSWORD_RUNE_SLAYER		= 242140,
#ifdef SERV_ELSWORD_INFINITY_SWORD
			CI_CLASS_CHANGE_ELSWORD_INFINITY_SWORD	= 242150,
#else
			CI_CLASS_CHANGE_ELSWORD_SHEATH_KNIGHT_2	= 242150,
#endif //SERV_ELSWORD_INFINITY_SWORD
			CI_CLASS_CHANGE_AISHA_VOID_PRINCESS		= 242160,
			CI_CLASS_CHANGE_AISHA_ELMENTAL_MASTER	= 242170,
#ifdef SERV_ARME_DIMENSION_WITCH
			CI_CLASS_CHANGE_AISHA_DIMENSION_WITCH	= 242180,
#else
			CI_CLASS_CHANGE_AISHA_BATTLE_MAGICIAN_2	= 242180,
#endif

			CI_CLASS_CHANGE_RENA_WIND_SNEAKER		= 242190,			
			CI_CLASS_CHANGE_RENA_GRAND_ARCHER		= 242200,			
#ifdef SERV_RENA_NIGHT_WATCHER
			CI_CLASS_CHANGE_RENA_NIGHT_WATCHER		= 242210,
#else
			CI_CLASS_CHANGE_RENA_TRAPPING_RANGER_2	= 242210,
#endif
			
			CI_CLASS_CHANGE_REVEN_BLADE_MASTER		= 242220,
			CI_CLASS_CHANGE_REVEN_RECKLESS_FIST		= 242230,
#ifdef SERV_RAVEN_VETERAN_COMMANDER
			CI_CLASS_CHANGE_RAVEN_VETERAN_COMMANDER	= 242240,
#else
			CI_CLASS_CHANGE_RAVEN_WEAPON_TAKER_2	= 242240,
#endif
			
			CI_CLASS_CHANGE_EVE_NEMESIS				= 242250,			
			CI_CLASS_CHANGE_EVE_EMPRESS				= 242260,
#ifdef SERV_EVE_BATTLE_SERAPH
			CI_CLASS_CHANGE_EVE_BATTLE_SERAPH		= 242270,
#else
			CI_CLASS_CHANGE_EVE_ELECTRA_2			= 242270,
#endif
			
			CI_CLASS_CHANGE_CHUNG_IRON_PALADIN		= 242280,
			CI_CLASS_CHANGE_CHUNG_DEADLY_CHASER		= 242290,
#ifdef SERV_CHUNG_TACTICAL_TROOPER
			CI_CLASS_CHANGE_CHUNG_TACTICAL_TROOPER	= 242300,
#else
			CI_CLASS_CHANGE_CHUNG_SHELLING_GUARDIAN_2	= 242300,
#endif

#ifdef SERV_NEW_CHARACTER_EL	// ���볯¥: 2013-07-18
			CI_CLASS_CHANGE_ELESIS_SABER_KNIGHT		= 252754,
			CI_CLASS_CHANGE_ELESIS_PYRO_KNIGHT		= 252755,
#endif	// SERV_NEW_CHARACTER_EL

#endif SERV_UNIT_CLASS_CHANGE_ITEM

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
			CI_CLASS_CHANGE_UC_ARA_LITTLE_HSIEN		= 252757,	/// �ƶ� 1�� ���� �Ҽ� ���� �����
			CI_CLASS_CHANGE_ARA_LITTLE_DEVIL		= 252756,	/// �ƶ� 1�� ���� �Ҹ� ���� �����
			
			CI_CLASS_CHANGE_ARA_SAKRA_DEVANAM		= 252751,	/// �ƶ� 2�� ���� ��õ ���� �����
			CI_CLASS_CHANGE_ARA_YAMA_RAJA			= 252752,	/// �ƶ� 2�� ���� ��� ���� �����
#endif // SERV_ARA_CHANGE_CLASS_SECOND
			//}}
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
			CI_CLASS_CHANGE_ELESIS_GRAND_MASTER		= 272171,	/// �����ý� 2�� ���� �׷��� ������ ���� �����
			CI_CLASS_CHANGE_ELESIS_BLAZING_HEART	= 272172,	/// �����ý� 2�� ���� ����¡ ��Ʈ ���� �����
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
			CI_CLASS_CHANGE_PSYCHIC_TRACER			= 999992,	/// �ֵ� 1�� ���� ����ű Ʈ���̼� ���� �����
#endif //SERV_9TH_NEW_CHARACTER
#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
			CI_CLASS_CHANGE_LUNATIC_PSYKER			= 999993,	/// �ֵ� 2�� ���� �糪ƽ ����Ŀ ���� �����
#endif //SERV_ADD_LUNATIC_PSYKER

			//{{ 2012. 02. 22	�ڼ���	�� �̸� �����
#ifdef SERV_PET_CHANGE_NAME
			CI_PET_NAME_CHANGE						= 248390,
#endif SERV_PET_CHANGE_NAME
			//}}

			//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
			CI_GUILD_NAME_CHANGE					= 248400,
#endif SERV_GUILD_CHANGE_NAME
			//}}

			//{{ 2012. 04. 26	�ڼ���	�� �峭��
#ifdef SERV_PET_TOY
			CI_PET_TOY								= 500580,
#endif SERV_PET_TOY
			//}}
			//{{ 2013. 2. 28	�ڼ���	 ���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
			CI_BINGO_MIX_ITEM						= 257280,
			CI_BINGO_RESET_ITEM						= 257290,
#endif SERV_EVENT_BINGO
			//}}

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT// �۾���¥: 2013-05-12	// �ڼ���
			CI_REQUEST_OF_LOVE						= 135404,	// ����� ������Ʈ
#endif // SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

#ifdef SERV_SKILL_PAGE_SYSTEM
			CI_EXPAND_SKILL_PAGE					= 127273,	// ��ų ������ Ȯ���
#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_CASH_ITEM_FOR_ADD
			CI_EXPAND_SKILL_SLOT_ADD				= 272738,	// ��ų ���� BȮ�� �����ý� ���� �Ǹ� ĳ����
#endif // SERV_CASH_ITEM_FOR_ADD

//----------------------------------------------------------------------------------//
#ifdef EVENT_GUILD_ITEM
			CI_EVENT_GUILD_SKILL_RESET_ITEM			= 80001182,	//(�߱�) �̺�Ʈ�� �Ƹ����� ������ �Ǹ� // 2012.06.05 lygan_������
#endif //EVENT_GUILD_ITEM
#ifdef SERV_EVENT_SKILL_POINT
			CI_SKILL_5_POINT_30						= 60005091,	// ������ �̼Ǵ����� ���� ������
			CI_SKILL_10_POINT_60					= 60005092, // ������ �̼Ǵ����� ���� ������
#endif

#ifdef SERV_REFORM_INVENTORY_INT_CASH_ITEM
			// �κ��丮 ī�װ��� Ȯ��
			CI_EXPAND_INVENTORY_EQUIP				= 200751, // ���
			CI_EXPAND_INVENTORY_ACCESSORY			= 200752, // �Ǽ��縮
			CI_EXPAND_INVENTORY_QUICK_SLOT			= 200753, // �Һ�
			CI_EXPAND_INVENTORY_MATERIAL			= 200754, // ���
			CI_EXPAND_INVENTORY_QUEST				= 200755, // ����Ʈ
			CI_EXPAND_INVENTORY_SPECIAL				= 200756, // Ư��
#endif  SERV_REFORM_INVENTORY_INT_CASH_ITEM

			//{{ ���� - ���� ���� ����Ʈ, ĳ�� �۾� 
#ifdef SERV_SHARING_BANK_QUEST_CASH
#ifdef SERV_COUNTRY_TWHK
			CI_CASH_SHARING_BACK_OPEN				= 60003460,	// ���� ���� Ȱ��ȭ ������(�븸)
#else
			CI_CASH_SHARING_BACK_OPEN				= 65000701,
#endif SERV_COUNTRY_TWHK
#endif

#ifdef SERV_SYNTHESIS_AVATAR
			CI_SYNTHESIS_ITEM						= 65000550,
#endif SERV_SYNTHESIS_AVATAR

#ifdef SERV_VIP_SYSTEM
			CI_EVENT_NASOD_MEGAPHONE				= 80000119, //(�߱�) �̺�Ʈ �� ���ҵ� �ް��� //2011.09.17 lygan_������
			CI_EVENT_HIGH_NASOD_MEGAPHONE			= 80000120, //(�߱�) �̺�Ʈ �� ���� ���ҵ� �ް��� //2011.09.17 lygan_������
#endif //SERV_VIP_SYSTEM
#ifdef SERV_CN_GNOSIS
			CI_EVENT_SKILL_POINT_10_USE_INVEN_7_DAY	= 80001150,	//(�߱�) �̺�Ʈ�� �׳�ý��� �ູ ( 10 SP, 7�ϱ�) // 2012.05.24 lygan_������
			CI_EVENT_SKILL_POINT_5_USE_INVEN_1_DAY	= 80001327, //(�߱�) �̺�Ʈ�� �׳�ý��� �ູ ( 5 SP, 1�ϱ�) // 2012.06.25 lygan_������

			CI_EVENT_SKILL_POINT_5_USE_INVEN_7_DAY		 = 80001355, //(�߱�) �̺�Ʈ�� �׳�ý��� �ູ ( 5 SP, 7�ϱ�) // 2012.07.02 lygan_������
			CI_EVENT_SKILL_POINT_5_USE_INVEN_7_DAY_TYPE2 = 80001372, //(�߱�) �̺�Ʈ�� �׳�ý��� �ູ ( 5 SP, 7�ϱ�) // 2012.07.02 lygan_������ // �̰� ���� ���� ���� ���� ���� �ʴ� ������
			CI_EVENT_SKILL_POINT_10_USE_INVEN_15_DAY	 = 80001367, //(�߱�) �̺�Ʈ�� �׳�ý��� �ູ ( 10 SP, 15�ϱ�) // 2012.07.02 lygan_������
#endif SERV_CN_GNOSIS
#ifdef SERV_GNOSIS_BR
			CI_EVENT_SKILL_POINT_5_USE_INVEN_7_DAY = 90000690, // �׳�ý��� �ູ (5 SP, 7�ϱ�)
			CI_EVENT_SKILL_POINT_5_USE_INVEN_15_DAY = 90000691, // �׳�ý��� �ູ (5 SP, 15�ϱ�)
#endif SERV_GNOSIS_BR

#ifdef SERV_EVENT_VIP_SYSTEM			
			CI_CASH_REWARD_EVENT_VIP_OPEN			= 83000170,	// (�߱�) VIP ���������� //2012.11.06 lygan_������ // �߱� ���� ���� ����â 2�� �� ���¿� (�̺�Ʈ)
#endif //SERV_EVENT_VIP_SYSTEM
#ifdef SERV_US_GNOSIS
			CI_EVENT_SKILL_POINT_5_USE_INVEN_15_DAY	= 90000747, // �̱� �̺�Ʈ �׳�ý�
#endif SERV_US_GNOSIS

#ifdef SERV_CASHITEM_PURCHASE_RESTRICTION
			CI_ICE_BURNER							= 207500,
			CI_OFFICER_EVE_ICE_BURNER				= 120003200,
			CI_OFFICER_CHUNG_ICE_BURNER				= 90001070,
			CI_MAGIC_NECKLACE						= 75000600,
			CI_ICE_BURNER_LURIAL					= 210000226,
#endif SERV_CASHITEM_PURCHASE_RESTRICTION

//----------------------------------------------------------------------------------//
// �ؿ��� ���� �̺�Ʈ ������
#ifdef SERV_GLOBAL_EVENT_ITEM

#ifdef SERV_EVENT_INIT_SKILL_TREE_ITEM
			CI_INIT_SKILL_TREE_ITEM					= 60001727,	// ������ �帵ũ
#endif SERV_EVENT_INIT_SKILL_TREE_ITEM
#ifdef SERV_EVENT_RESET_A_SKILL_ITEM
			CI_RESET_A_SKILL_ITEM					= 60001726, // ������ �˾�
#endif SERV_EVENT_RESET_A_SKILL_ITEM
#ifdef SERV_ADD_WARP_BUTTON
			CI_WARP_VIP_ITEM						= 215660,	// ���ο� ���������� �����ؾ� �ұ�?
#endif // SERV_ADD_WARP_BUTTON

#ifdef SERV_QUEST_SKILL_RESET
			CI_SKILL_RESET_QUEST_ITEM				= 141000000, // ��ų �Ѱ� �ʱ�ȭ
#endif SERV_QUEST_SKILL_RESET
#ifdef SERV_2ND_CLASS_SKILL_RESET
			CI_SKILL_RESET_2ND_CLASS_EVENT_ITEM		= 141000036, // ��ų �Ѱ� �ʱ�ȭ
#endif SERV_2ND_CLASS_SKILL_RESET

#ifdef SERV_EVENT_BUY_FAKE_ITEM
			CI_BUY_FAKE_ITEM						= 67005900, //������ �̺�Ʈ�� ��ũ���� ��Ű��
#endif //SERV_EVENT_BUY_FAKE_ITEM

#ifdef SERV_EVENT_RURIEL_RESET_SKILL_ITEM
			EI_RURIEL_RESET_A_SKILL_ITEM			= 67006453, // �縮���� ���� ��ų �ʱ�ȭ �޴�
			EI_RURIEL_RESET_SKILL_ITEM				= 67006454, // �縮���� ��ų �ʱ�ȭ �޴�
			EI_RURIEL_RESET_A_SKILL_EVENT_ITEM		= 67006451, // �縮���� ������ �˾�
#endif SERV_EVENT_RURIEL_RESET_SKILL_ITEM

#ifdef SERV_RURIEL_EVENT_ITEM
			CI_RURIEL_EVENT_SEAL_ITEM_NORMAL		= 83000290,	 //�縮���� �Ƿ��� �� ���� (�Ͼ��)
			CI_RURIEL_EVENT_SEAL_ITEM_RARE			= 83000291,	 //�縮���� �Ƿ��� �� ���� (�����)
			CI_RURIEL_EVENT_SEAL_ITEM_ELITE			= 83000292,	 //�縮���� �Ƿ��� �� ���� (�����)
			CI_RURIEL_EVENT_SEAL_ITEM_UNIQUE		= 83000293,	 //�縮���� �Ƿ��� �� ���� (Ȳ�ݻ�)
#endif //SERV_RURIEL_EVENT_ITEM

#ifdef SERV_ARIEL_EVENT_ITEM
			CI_ARIEL_EVENT_SEAL_ITEM_NORMAL			= 82214370,	 //�Ƹ����� �Ƿ��� �� ���� (�Ͼ��)
			CI_ARIEL_EVENT_SEAL_ITEM_RARE			= 82214380,	 //�Ƹ����� �Ƿ��� �� ���� (�����)
			CI_ARIEL_EVENT_SEAL_ITEM_ELITE			= 82214390,	 //�Ƹ����� �Ƿ��� �� ���� (�����)
			CI_ARIEL_EVENT_SEAL_ITEM_UNIQUE			= 82214400,	 //�Ƹ����� �Ƿ��� �� ���� (Ȳ�ݻ�)
#endif //SERV_ARIEL_EVENT_ITEM

#endif //SERV_GLOBAL_EVENT_ITEM
//----------------------------------------------------------------------------------//
		};
		//}}

		//{{ 2009. 9. 7  ������		�к� ī��Ʈ
		enum SEAL_ITEM_INFO
		{
            SEAL_ITEM_DECREASE_COUNT = 1,
		};
		//}}

		//{{ 2009. 2. 23  ������	�̺�Ʈ ������
		enum EVENT_ITEM
		{
			EI_GOLD_MEDAL				= 130002, // ��ȸä�ο� Ȳ�ݸ޴�
#ifdef SERV_COUNTRY_JP
			EI_XMAS_EVENT_ITEM			= 65000390, //��Ÿ�Ƹ��� ���� // �ӱԼ� �Ϻ� ���� 131354 ũ�������� Ʈ�� ���
#else //SERV_COUNTRY_JP
			EI_XMAS_EVENT_ITEM			= 131354, // ũ�������� Ʈ�� ���
#endif //SERV_COUNTRY_JP
			EI_CAMILLAS_COIN			= 130715, // ī�ж��� ����
			EI_CAMILLAS_COIN_2010		= 132701, // ī�ж��� ���� ( 2010�� �������� )
			EI_PRELIMINARY_TICKET		= 130843, // ���� ��ȸ ������
			EI_OFFLINE_TICKET			= 135304, // �¶��� ��ʸ�Ʈ ���� ��û�� 130844, // �������� ��ȸ ������
			EI_TREE_SUMMON_ITEM			= 131857, // �ڿ��� ���� ( ������ȣ�� ��ȯ )

			//{{ 2010. 12. 8	������	�̺�Ʈ�� �׳�ý� �ູ
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
			//{{ 2010. 12. 8	������	�̺�Ʈ�� �׳�ý� �ູ
			EI_SKILL_POINT_30_USE_INVEN	= 133131, // �̺�Ʈ�� �׳�ý� �ູ
#ifdef SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
			EI_SKILL_POINT_130_1DAY_USE_INVEN = 67006450,	// �̺�Ʈ�� �׳�ý� �ູ 
#endif SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
#ifdef SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
			EI_SKILL_POINT_30_1DAY_USE_INVEN		= 67006500, // �̺�Ʈ�� �׳�ý� �ູ 30sp 1��
			EI_SKILL_POINT_60_1DAY_USE_INVEN		= 67006501, // �̺�Ʈ�� �׳�ý� �ູ 60sp 1��
#endif SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
			EI_SKILL_POINT_30_USE_INVEN_JP			= 67000382,	// �̺�Ʈ�� �׳�ý� �ູ(2��)	133131
			EI_SKILL_POINT_10_30DAY_USE_INVEN_JP	= 67005710, // �̺�Ʈ�� �׳�ý� �ູ 10sp 30��
			EI_SKILL_POINT_10_15DAY_USE_INVEN_JP	= 70007020, // �̺�Ʈ�� �׳�ý� �ູ 10sp 15��
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
			EI_SKILL_POINT_30_7DAY_USE_INVEN	= 60003758, // �̺�Ʈ�� �׳�ý� �ູ 30SP 7��
			EI_SKILL_POINT_60_7DAY_USE_INVEN	= 60003759, // �̺�Ʈ�� �׳�ý� �ູ 60SP 7��
			EI_SKILL_POINT_60_15DAY_USE_INVEN	= 60004280, // �̺�Ʈ�� �׳�ý� �ູ 60SP 15��
			EI_SKILL_POINT_60_30DAY_USE_INVEN	= 60005011, // �̺�Ʈ�� �׳�ý� �ູ 60SP 30��
			EI_SKILL_POINT_60_7DAY_USE_INVEN_2	= 60008007, // �̺�Ʈ�� �׳�ý� �ູ 60SP 7��	
			EI_SKILL_POINT_30_7DAY_USE_INVEN_2	= 70007030, // �̺�Ʈ�� �׳�ý� �ູ 30SP 7��
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
			/*
			EI_SKILL_POINT_5_USE_INVEN	= 133131, // �̺�Ʈ�� �׳�ý� �ູ
			*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
			EI_SKILL_POINT_30_7DAY_USE_INVEN_INT	= 70007030, // �̺�Ʈ�� �׳�ý� �ູ 30SP 7��
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
			//}}

			//{{ 2010. 12. 20	������	Ư�� �ð� ť�� ����
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
			EI_XMAS_RANDOM_CUBE			= 133223, // ũ�������� �̺�Ʈ ť�� ( ��Ÿ �縻 )
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
			//}}

			//{{ 2011. 07. 04    ��μ�    ���̹� ���θ��
#ifdef SERV_NAVER_PROMOTIONS
			EI_SUMMER_RANDOM_CUBE		= 160227,	 // �� ���� ��� ť��
			EI_WINTER_RANDOM_CUBE		= 160326,	 // �� �ܿ� ��� ť��
			EI_WINTER_BIG_RANDOM_CUBE	= 160374,	 // �� �ܿ� �մ�� ť��
			EI_SUMMER_TEN_ENCHANT_CHARM = 130152,	 // 10 �� ����
			EI_SUMMER_RANDOM_NDS		= 160230, // ���ٵ� ���� ��ȣ
			EI_SUMMER_NEXON_CASH		= 160229, // �ؽ� ĳ�� 10000
			EI_SUMMER_NEXON_CASH_50000	= 135105, // �ؽ� ĳ�� 50000
			//{{ 2011. 12.13    ��μ�	���� �̺�Ʈ ��� �߰� (�ݺ� ����)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
			EI_RANDOM_CUBE_GOLD			= 135115, // ���ε� ���� ������(Gold)
#endif SERV_RANDOM_CUBE_GOLD_REWARD
			//}}
#endif SERV_NAVER_PROMOTIONS
			//}} 

			//{{ 2011. 01. 06  ��μ�  ��ų����ü���� üũ(�κ��丮-�Ⱓ��) ��� ����
#ifdef SERV_SKILL_SLOT_CHANGE_INVENTORY
			EI_EXPAND_SKILL_SLOT_EVENT	= 200880, // �̺�Ʈ ��ų ����B (�Ⱓ��)
#endif SERV_SKILL_SLOT_CHANGE_INVENTORY
			//}}
			//{{ 2011. 08. 26	������	ĳ���� ������ ������
#ifdef SERV_CHAR_LEVEL_UP_ITEM
			EI_CHAR_LEVEL_UP_ITEM		= 160267, // ������ �ֹ���
#endif SERV_CHAR_LEVEL_UP_ITEM
			//}}
			//{{ 2011. 12. 05	������	�ູ�� �޴� ť�� üũ
#ifdef SERV_CHECK_BLESSING_MEDAL_CUBE
			EI_BLESSING_MEDAL_CUBE		= 132698, // �ູ�� �޴�(1�ϱ�) ť��
#endif SERV_CHECK_BLESSING_MEDAL_CUBE
			//}}
			//{{ 2011. 12. 06	������	Ư�� ������ �̺�Ʈ �ý���
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
			EI_COBO_PC_BANG_COUPON		= 135093, // �ں� PC�� ����
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
			//}}

#ifdef SERV_EVENT_MONEY	// ��μ� // ���볯¥: 2013-07-04
			EI_EVENT_MONEY_ITEM		= 135465, // ���� �⼮����
#endif // SERV_EVENT_MONEY

			//{{ 2012. 07. 09	��μ�       ������ ������
//#ifdef SERV_DUNGEON_CLEAR_PAYMENT_STONE_EVENT
			EI_WISE_MAN_STONE_ITEM		= 135184, // ������ ������
//#endif SERV_DUNGEON_CLEAR_PAYMENT_STONE_EVENT
			//}}
			//{{ 2012. 07. 09	��μ�       �̺�Ʈ ť�� ���� ���� �ð�
#ifdef SERV_EVENT_CUBE_OPEN_TIME
			EI_EVENT_LOTTO_CUBE				= 160533, // ��� ť��
			EI_EVENT_GOLD_CUBE				= 160534, // Ȳ�� ť��
			EI_EVENT_CASH_CUBE				= 160535, // �ؽ� ĳ�� ť��

			EI_NEXON_CASH_100				= 160536, // �ؽ� ĳ�� 100
			EI_NEXON_CASH_1000				= 160537, // �ؽ� ĳ�� 1000
			EI_NEXON_CASH_5000				= 160538, // �ؽ� ĳ�� 5000
			EI_NEXON_CASH_10000				= 160539, // �ؽ� ĳ�� 10000
#endif SERV_EVENT_CUBE_OPEN_TIME
			//}}
			//{{ 2012. 12. 19  ��ȯ(���� ����) - ��μ�
#ifdef SERV_ITEM_EXCHANGE_LIMIT_COUNT
			EI_ARA_EVENT_NEXON_CASH_1000		= 135318, // õ�� ���� �ؽ� ĳ�� 1000�� ���� ����
#endif SERV_ITEM_EXCHANGE_LIMIT_COUNT
			//}}

			//{{ 2013. 03. 27	 �׳�ý��� �ູ 5point 7�� �߰� - ��μ�
#ifdef SERV_EVENT_CASH_SKILL_5_POINT_7_DAY
#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
			EI_SKILL_POINT_30_DAY_7_USE_INVEN		= 268562,	// �κ��丮���� Ŭ���ؼ� ����ϴ� �׳�ý� �ູ 30����Ʈ 7����
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
			/*
			EI_SKILL_POINT_5_DAY_7_USE_INVEN		= 268562,	// �κ��丮���� Ŭ���ؼ� ����ϴ� �׳�ý� �ູ 5����Ʈ 7����
			*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
#endif SERV_EVENT_CASH_SKILL_5_POINT_7_DAY
			//}

#ifdef SERV_NEW_ITEM_COBO_SKILL_RESET_ITEM// �۾���¥: 2013-07-02	// �ڼ���
			CI_COBO_SKILL_RESET_ITEM		= 135433, // ��ų �Ѱ� �ʱ�ȭ
#endif // SERV_NEW_ITEM_COBO_SKILL_RESET_ITEM

//----------------------------------------------------------------------------------//
// �ؿ��� ���� �̺�Ʈ ������
#ifdef SERV_GLOBAL_EVENT_ITEM
		// ���� �߰��Ǵ� ��� �̰��� �־��ֽñ� �ٶ��ϴ�.

#ifdef SERV_CHAR_LEVEL_UP_ITEM_EVENT_RENA
			EL_CHAR_LEVEL_UP_ITEM2		= 60004276,	// �ð����� �ֹ���
#endif SERV_CHAR_LEVEL_UP_ITEM_EVENT_RENA
		
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
		// ���� ������ ���� Ư�� ��Ű�� �Ǹ�(7.4)
		EI_SERVICE_COUPON_TYPE_A				= 60007075,	// ���� ����� ���� Type A
		EI_SERVICE_COUPON_TYPE_A_PACKAGE		= 60007078,	// ���� ����� ���� Type A ��Ű��

		EI_SERVICE_COUPON_TYPE_B				= 60007076,	// ���� ����� ���� Type B
		EI_SERVICE_COUPON_TYPE_B_PACKAGE		= 60007079,	// ���� ����� ���� Type B ��Ű��

		EI_SERVICE_COUPON_TYPE_C				= 60007077,	// ���� ����� ���� Type C
		EI_SERVICE_COUPON_TYPE_C_PACKAGE		= 60007080,	// ���� ����� ���� Type C ��Ű��

		EI_ARA_SPECIAL_COUPON					= 60007091,	// �ƶ� ����� ����
		EI_ARA_SPECIAL_ATTRACTION_PACKAGE		= 60007113,	// �ƶ� ����� ������ ��Ű��
		EI_ARA_SPECIAL_AVATAR1_PACKAGE			= 60007114,	// �ƶ� ����� �ƹ�Ÿ ��Ű��1
		EI_ARA_SPECIAL_AVATAR2_PACKAGE			= 60007115,	// �ƶ� ����� �ƹ�Ÿ ��Ű��2
		EI_ARA_SPECIAL_PET_PACKAGE				= 60007116,	// �ƶ� ����� �� ��Ű��

		EI_ARIEL_SPECIAL_PRESENT_CUBE_LV12		= 60007062, // �Ƹ��� Ư�� ���� ť�� Lv.12
		EI_ARIEL_SPECIAL_PRESENT_CUBE_LV18		= 60007064, // �Ƹ��� Ư�� ���� ť�� Lv.18
		EI_ARIEL_SPECIAL_PRESENT_CUBE_LV20		= 60007066, // �Ƹ��� Ư�� ���� ť�� Lv.20
		EI_SPECIAL_PRESENT_COPPER_KEY			= 60007069, // ����� ���� ����(��)
		EI_SPECIAL_PRESENT_SILVER_KEY			= 60007070, // ����� ���� ����(��)
		EI_SPECIAL_PRESENT_GOLD_KEY				= 60007071, // ����� ���� ����(��)

		EI_SPECIAL_HEAVENLY_BOSS_RARE			= 70007203, // õ���� ������(����)
#endif //SERV_KEEP_ITEM_SHOW_CASHSHOP

#ifdef SERV_READY_TO_SOSUN_EVENT
		EI_READY_TO_SOSUN_EVENT_ITEM			= 60007092,	// �ƶ� 1�� ���� �̸����߱� �����
		EI_READY_TO_SOSUN_EVENT_WIN_ITEM		= 60007112,	// �ƶ� 1�� ���� �̸����߱� ��÷ ����
#endif SERV_READY_TO_SOSUN_EVENT
#ifdef SERV_RELATIONSHIP_EVENT_INT
		EI_EVENT_PROPOSE_ITEM					= 60007791,	// ���� ��ȥ ��û��
		EI_EVENT_DIVORCE_ITEM					= 60007792, // ���� ��ȥ ��༭
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_RECRUIT_EVENT_BASE
		EI_RECRUIT_CUBE							= 90002350, // �ű����� ���� ť��
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
		EI_INFINITY_EL_ESSENCE_ITEM_EVENT		= 60008064,		// ���� ���� ���ޱ�
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
#ifdef SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
		EI_SKILL_POINT_30_14DAY_USE_INVEN	= 141000896, //���� �׳�ý� 
#endif SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
#ifdef SERV_LURIEL_GNOSIS
		EI_LURIEL_GNOSIS_30_15DAY				= 210000270, // �縮���� �׳�ý��� �ູ (30SP/15��)
		EI_LURIEL_GNOSIS_30_30DAY				= 210000271, // �縮���� �׳�ý��� �ູ (30SP/30��)
		EI_LURIEL_GNOSIS_30_60DAY				= 210000272, // �縮���� �׳�ý��� �ູ (30SP/60��)
		EI_LURIEL_GNOSIS_30_7DAY				= 210000273, // �縮���� �׳�ý��� �ູ (30SP/7��)
		EI_LURIEL_GNOSIS_60_15DAY				= 210000280, // �縮���� �׳�ý��� �ູ (60SP/15��)
		EI_LURIEL_GNOSIS_60_30DAY				= 210000281, // �縮���� �׳�ý��� �ູ (60SP/30��)
		EI_LURIEL_GNOSIS_60_60DAY				= 210000282, // �縮���� �׳�ý��� �ູ (60SP/60��)
		EI_LURIEL_GNOSIS_60_7DAY				= 210000283, // �縮���� �׳�ý��� �ູ (60SP/7��)
#endif //SERV_LURIEL_GNOSIS
#ifdef SERV_EVENT_TEAR_OF_ELWOMAN
		EI_TEAR_OF_ELWOMAN		= 85001550	, // ���� ������ ����
#endif SERV_EVENT_TEAR_OF_ELWOMAN

#endif //SERV_GLOBAL_EVENT_ITEM
//----------------------------------------------------------------------------------//
		};
		//}}

		//{{ 2012. 03. 15	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		enum REWARD_ITEM
		{
            RI_AUTO_PARTY_BONUS			= 135289, // ������ ��ȭ
		};
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		enum DEFENCE_DUNGEON_REWARD
		{
			DDR_DRAGON_PIECE			= 180034, // ����� ��� ����
		};

		//{{ 2009. 12. 15  ������	ED������
		enum ED_ITEM
		{
            EDI_BRONZE_ED				= 127000, // �����ED
			EDI_SILVER_ED				= 127010, // �ǹ�ED
			EDI_GOLD_ED					= 127020, // ���ED
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-15	// �ڼ���
			EDI_GOLD_BAR				= 127041, // �ݱ�
#endif // SERV_BATTLE_FIELD_BOSS
		};
		//}}

		//{{ 2009. 3. 9  ������		�����̵�������
		enum USE_IN_INVENTORY_ITEM
		{
			SI_WARP_RUBEN				= 109995, // ������ Ư�� �̵� Ƽ�� (�纥 ����)
			SI_WARP_ELDER				= 109996, // ������ Ư�� �̵� Ƽ�� (���� ����)
			SI_WARP_BESMA				= 109997, // ������ Ư�� �̵� Ƽ�� (������ ����)
			SI_WARP_ALTERA				= 109998, // ������ Ư�� �̵� Ƽ�� (���׶� ����)
			SI_WARP_PEITA				= 110832, // ����Ÿ Ư�� �̵� Ƽ�� (����Ÿ ��������Ʈ)
			SI_WARP_VELDER				= 110847, // ���� �ٱ� Ư�� �̵� Ƽ�� (�����ٱ� ����)

			//{{ 2009. 8. 7  ������		����
			SI_BANK_MEMBERSHIP_UPGRADE	= 99380, // �縮���� �ſ� ������
			//}}

			//{{ 2009. 9. 22  ������	���
			SI_GUILD_FOUNDATION			= 110845, // ��� â�� �㰡��
			//}}

			//{{ 2009. 9. 23  ������	1������ĳ����
			SI_FIRST_CHANGEJOB_CUBE		= 109999, // 1�� ���� ���θ�� ť��
			//}}
			//{{ 2009. 11. 10  ������	2������
			SI_SECOND_CHANGE_JOB_CUBE	= 110850, // 2�� ���� ���θ�� ť��
			//}}

			//{{ 2009. 10. 9  ������	���
			SI_GUILD_MAX_MEMBER_UPGRADE	= 110846, // ��� �ο� Ȯ���
			//}}

			SI_RING_OF_PVP_REBIRTH		= 202410, // ������ ����
			SI_BLESSING_MEDAL			= 200720, // �ູ�� �޴�
			SI_EXP_15_PERCENT_MEDAL		= 129940, // ����ġ 15% �޴�
			SI_EXP_100_PERCENT_MEDAL	= 130529, // ����ġ 100% �޴�

			//{{ 2011. 10. 19    ��μ�    200% ����ġ �߰� �޴�(�̺�Ʈ ������)
#ifdef SERV_ADD_EXP_200_MEDAL
			SI_EXP_200_PERCENT_MEDAL	= 160299, // ������ �޴�  ����ġ 200% �޴�
#endif SERV_ADD_EXP_200_MEDAL
			//}} 

			SI_USE_FREE_BY_FIELD		= 215660, // �ڵ����� �����̵� ������.

			//{{ 2012. 10. 31	�ڼ���	�ں� �ͽ������� Ƽ�� �߰�
#ifdef SERV_ADD_COBO_EXPRESS_TICKET
			SI_USE_COBO_EXPRESS_TICKET	= 112323, // ED �Ҹ� �����̵� ������.
#endif SERV_ADD_COBO_EXPRESS_TICKET
			//}}

			//{{ 2009. 11. 27  ������	��彺ų
			SI_GUILD_CASH_SKILL_POINT	= 110859, // �׳�ý��� ���ູ
			//}}
			//{{ 2010. 04. 09  ������	����� ��Ʈ
			SI_SKILL_NOTE_ITEM			= 99600,  // ����� ��Ʈ ������
			//}}
			//{{ 2011. 05. 11	������	�븮����
#ifdef SERV_PSHOP_AGENCY
			SI_PSHOP_AGENCY_1_DAY		= 160060, // �븮 �Ǹ� ������(1�ϱ�)
			SI_PSHOP_AGENCY_2_DAYS		= 160061, // �븮 �Ǹ� ������(2�ϱ�)
			SI_PSHOP_AGENCY_3_DAYS		= 160062, // �븮 �Ǹ� ������(3�ϱ�)
			SI_PSHOP_AGENCY_5_DAYS		= 160063, // �븮 �Ǹ� ������(5�ϱ�)
			SI_PSHOP_AGENCY_7_DAYS		= 160064, // �븮 �Ǹ� ������(7�ϱ�)
			SI_PSHOP_AGENCY_10_DAYS		= 160065, // �븮 �Ǹ� ������(10�ϱ�)
			SI_PSHOP_AGENCY_15_DAYS		= 160066, // �븮 �Ǹ� ������(15�ϱ�)
			SI_PSHOP_AGENCY_30_DAYS		= 160067, // �븮 �Ǹ� ������(30�ϱ�)

            SI_PSHOP_AGENCY_7_DAYS_ED   = 160904, // �븮 �Ǹ� ������ (7�ϱ�, ED)
            SI_PSHOP_AGENCY_15_DAYS_ED  = 160905,//             �븮 �Ǹ� ������
            SI_PSHOP_AGENCY_30_DAYS_ED  = 160906,//             �븮 �Ǹ� ������
#endif SERV_PSHOP_AGENCY
			//}}
			
			//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
			SI_COME_BACK_REWARD_SKILL_30_POINT_15_DAY		= 127180, // �׳�ý��� �ູ(15�ϱ�)
			SI_COME_BACK_REWARD_SKILL_30_POINT_30_DAY		= 127190, // �׳�ý��� �ູ(30�ϱ�)
			SI_COME_BACK_REWARD_SKILL_30_POINT_60_DAY		= 127200, // �׳�ý��� �ູ(60�ϱ�)
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
			SI_COME_BACK_REWARD_SKILL_5_POINT_15_DAY		= 127180, // �׳�ý��� �ູ(15�ϱ�)
			SI_COME_BACK_REWARD_SKILL_5_POINT_30_DAY		= 127190, // �׳�ý��� �ູ(30�ϱ�)
			SI_COME_BACK_REWARD_SKILL_5_POINT_60_DAY		= 127200, // �׳�ý��� �ູ(60�ϱ�)
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
#endif SERV_COME_BACK_USER_REWARD
//}} 

#ifdef SERV_GOLD_TICKET
			SI_GOLD_TICKET_10_ID			= 75000700,// Gold Ticket (10)
			SI_GOLD_TICKET_50_ID			= 75000701,// Gold Ticket (50)
			SI_GOLD_TICKET_100_ID			= 75000702,// Gold Ticket (100)
			SI_GOLD_TICKET_500_ID			= 75000703,// Gold Ticket (500)
			SI_GOLD_TICKET_1000_ID			= 75000704,// Gold Ticket (1000)
			SI_GOLD_TICKET_2000_ID			= 75000705,// Gold Ticket (2000)
#endif SERV_GOLD_TICKET

			//{{ 2012. 03. 25	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
			SI_THE_GATE_OF_DARKNESS_ARCANUM_NUT		= 78520,	// ��Ʈ�� ��ȣ
			SI_THE_GATE_OF_DARKNESS_ARCANUM_PTAH	= 78530,	// ��Ÿ�� ��ȣ
			SI_THE_GATE_OF_DARKNESS_ARCANUM_RACHEL	= 78540,	// ������ ��ȣ
			SI_THE_GATE_OF_DARKNESS_ARCANUM_ZADKIEL	= 78550,	// ��Ű���� ��ȣ

			// ����
			SI_THE_GATE_OF_DARKNESS_ELIXIR_SERAPHIM	= 78560,	// ���������� �ູ
			SI_THE_GATE_OF_DARKNESS_ELIXIR_GEB		= 78570,	// �Ժ��� �ູ
			SI_THE_GATE_OF_DARKNESS_ELIXIR_AMON		= 78580,	// �Ƹ��� �ູ
			SI_THE_GATE_OF_DARKNESS_ELIXIR_CRONOS	= 78590,	// ũ�γ뽺�� �ູ
			
			//{{ 2013. 04. 12	�ڼ���	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
			// ����
			SI_THE_GATE_OF_DARKNESS_ELIXIR_GIANT_POTION				= 78894,	// ����� �� ���� : ����ȭ ����
			SI_THE_GATE_OF_DARKNESS_ELIXIR_BLAZING_BOMB				= 78895,	// ����� �� ���� : �̱��̱� ����
			SI_THE_GATE_OF_DARKNESS_ELIXIR_SPIRIT_OF_CHASER			= 78896,	// ����� �� ���� : �������� ȥ
			SI_THE_GATE_OF_DARKNESS_ELIXIR_CRADLE_OF_LITTLE_FAIRY	= 78897,	// ����� �� ���� : �Ʊ� ���� ���
			SI_THE_GATE_OF_DARKNESS_ELIXIR_ICE_BALL_OF_DENIPH		= 78898,	// ����� �� ���� : �������� ���� ����
			SI_THE_GATE_OF_DARKNESS_ELIXIR_FEATHER_OF_VENTUS		= 78899,	// ����� �� ���� : �������� ������
			SI_THE_GATE_OF_DARKNESS_ELIXIR_FLAME_RING_OF_ROSSO		= 78900,	// ����� �� ���� : �ν��� ȭ����
			SI_THE_GATE_OF_DARKNESS_ELIXIR_BIG_HAND_POTION			= 78901,	// ����� �� ���� : �Ŵ�� ����
#endif SERV_NEW_DEFENCE_DUNGEON
			//}}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
			//}}

			//{{ 2012. 12. 24	�ڼ���	�� ���� ���� ��� �߰�
#ifdef SERV_PET_AUTO_LOOTING
			SI_PET_AUTO_LOOTING_ITEM				= 500720,	// �� ���� ����
#ifdef SERV_PET_AUTO_LOOTING_ITEM_CN
			SI_PET_AUTO_LOOTING_ITEM_CN				= 100000120,	// �� ���� ���� �߱���(���� �����۵� ����մϴ�)
#endif // SERV_PET_AUTO_LOOTING_ITEM_CN
#endif SERV_PET_AUTO_LOOTING
			//}}

			//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
			SI_COUPLE_RING_ITEM						= 269710,	// Ŀ�ø�
			SI_WEDDING_RING_ITEM					= 269711,	// ��ȥ����
			SI_WEDDING_HALL_A_ITEM					= 269712,	// ��ȥ���� ĳ�� - A type
			SI_WEDDING_HALL_B_ITEM					= 269713,	// ��ȥ���� ĳ�� - B type
			SI_WEDDING_INVITE_TICKET_ITEM			= 269714,	// ��ȥ �ʴ��
			SI_WEDDING_RESERVATION_ITEM				= 269715,	// ��ȥ�� �����
			SI_WEDDING_INVITATION_ITEM				= 269716,	// ûø��
			SI_WEDDING_DRESS_ITEM					= 160765,	// �ǻ� ť��
			SI_LOVE_WORD_CHANGE_ITEM_ITEM			= 269718,	// ��Ī �����
			SI_COUPLE_ANNIVERSARY_DAY_REWARD_ITEM	= 160762,	// Ŀ�� ����� ť��
			SI_WEDDING_ANNIVERSARY_DAY_REWARD_ITEM	= 160763,	// ��ȥ ����� ť��
#endif SERV_RELATIONSHIP_SYSTEM
			//}

#ifdef SERV_FINALITY_SKILL_SYSTEM	// ���볯¥: 2013-08-01
			SI_FINALITY_SKILL_ITEM					= 271310,	// �ñ��� ������
#endif // SERV_FINALITY_SKILL_SYSTEM

#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
			SI_FINALITY_SKILL_BOOK					= 271311,	// �ñ��� ������
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT

//----------------------------------------------------------------------------------//
// �ؿ��� ���� �̺�Ʈ ������
#ifdef SERV_GLOBAL_EVENT_ITEM

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
			SI_HALLOWEEN_TRANSFORM_POSION_ITEM		= 90002300, // �� ���� ����
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#ifdef SERV_EVENT_GUILD_SKILL_GLOBAL
			SI_EVENT_GUILD_SKILL_5_POINT_15_DAY	= 67005930, // �Ϻ� �׳�ý� 15��,5SP, �ŷ��Ұ�
#endif //SERV_EVENT_GUILD_SKILL_GLOBAL

#ifdef NEW_YEAR_2013_EVENT
			SI_EXP_10_PERCENT_MEDAL					= 60005596,	// �Ƹ����� ����ġ 10% �޴�
#endif
#ifdef SERV_EXP_MEDAL_JP
			SI_EXP_30_PERCENT_MEDAL			= 65000720, // ����ġ 30% �޴�
			SI_EXP_10_PERCENT_MEDAL365		= 67004384, // ����ġ 10% �޴� ( 365 �� )
			SI_EXP_10_PERCENT_MEDAL1		= 67004385, // ����ġ 10% �޴� ( 1�� )
			SI_EXP_50_PERCENT_MEDAL_JP		= 60007081, // ����ġ 50% �޴�
			SI_EXP_30_PERCENT_MEDAL_JP		= 67006203, // ����ġ 30% �޴�(�̺�Ʈ��)
#endif
#ifdef EVENT_GUILD_ITEM
			SI_EVENT_GUILD_MAX_MEMBER_UPGRADE		= 80001183, // �߱� ���� �̺�Ʈ�� ��� �ο� Ȯ��� // 2012.06.04 lygan_������
			SI_EVENT_GUILD_CASH_SKILL_POINT			= 80001181, // �߱� ���� �̺�Ʈ�� �Ƹ����� �׳�ý� ���ູ 15�� 5GSP // 2012.06.05 lygan_������
#endif //EVENT_GUILD_ITEM
#ifdef EVENT_REBIRTH_RING_CN
			SI_RING_OF_PVP_REBIRTH_EVENT = 83000001, // �߱� ���� ������ ����
#endif //EVENT_REBIRTH_RING_CN

#ifdef SERV_VIP_SYSTEM
			SI_VIP_TICKET				= 80000115, //VIP Ƽ���� �������� 5%�� �߰� ����ġ
			SI_USE_FREE_BY_FIELD_VIP	= 80000117, //VIP ���� �����̵� ������
#endif //SERV_VIP_SYSTEM
#ifdef SERV_EVENT_VIP_SYSTEM
			SI_EVENT_VIP_TICKET				= 83000170, // �̺�Ʈ�� VIP Ƽ�� // 2012.11.06 lygan_������
#endif //SERV_EVENT_VIP_SYSTEM
#ifdef SERV_EVENT_ITEM_US
			SI_STARTER_EXP_15_PERCENT_MEDAL	= 120000100,// ����Ÿ�� ��Ű�� ����ġ 15% �޴�
			SI_EXP_50_PERCENT_MEDAL			= 120002740,// ����ġ 50% �޴�
			SI_EXP_30_PERCENT_MEDAL_1DAY	= 85001942, // ����ġ 30% �޴�(1�ϱ�)
			SI_EXP_10_PERCENT_MEDAL_US		= 85001972, // ����ġ 10% �޴�
#endif SERV_EVENT_ITEM_US
#ifdef SERV_EVENT_CHARACTER_SLOT_EXPAND_ITEM
			SI_CHARACTER_SLOT_EXPAND_ITEM1	= 60000410,	// �κ��丮 ��� �̺�Ʈ�� ĳ���� ���� Ȯ���1
			SI_CHARACTER_SLOT_EXPAND_ITEM2	= 60000825,	// �κ��丮 ��� �̺�Ʈ�� ĳ���� ���� Ȯ���2
			SI_CHARACTER_SLOT_EXPAND_ITEM3	= 60001965,	// �κ��丮 ��� �̺�Ʈ�� ĳ���� ���� Ȯ���3
#endif SERV_EVENT_CHARACTER_SLOT_EXPAND_ITEM
#ifdef SERV_SKILL_5_POINT_7_DAY_EU
			SI_EVENT_SKILL_5_POINT_7_DAY = 90000690,
#endif SERV_SKILL_5_POINT_7_DAY_EU
#ifdef SERV_EXP_MEDAL_TW
			SI_EXP_200_PERCENT_MEDAL_TWHK	= 64000012, // ������ �޴�  ����ġ 200% �޴�(�븸)
			SI_EXP_50_PERCENT_MEDAL_TWHK	= 60007081,	// ����ġ 50% �޴�
#endif SERV_EXP_MEDAL_TW
#ifdef SERV_TOUR_TICKET_EVENT
			SI_TOUR_TICKET_ITEM				= 60007447,
#endif SERV_TOUR_TICKET_EVENT
#ifdef SERV_EVENT_VC
			SI_EVENT_VC_BATTLE_FOOD			= 85003041,	// ���� �ķ�. ����ϸ� �뺴 �Ƿ� ģ�е� ���
#endif //SERV_EVENT_VC
#ifdef SERV_RECRUIT_EVENT_BASE
			SI_RECRUIT_TICKET				= 90000600,
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EXPAND_INVENTORY_BY_EVENT_ITEM
			SI_INVENTORY_SLOT_ADD_ITEM_EQUIP_EVENT			= 141000900,// �κ� Ȯ�� �̺�Ʈ��
			SI_INVENTORY_SLOT_ADD_ITEM_ACCESSORY_EVENT		= 141000901,
			SI_INVENTORY_SLOT_ADD_ITEM_QUICK_SLOT_EVENT		= 141000902,
			SI_INVENTORY_SLOT_ADD_ITEM_MATERIAL_EVENT		= 141000903,
			SI_INVENTORY_SLOT_ADD_ITEM_QUEST_EVENT			= 141000904,
			SI_INVENTORY_SLOT_ADD_ITEM_SPECIAL_EVENT		= 141000905,
#endif //SERV_EXPAND_INVENTORY_BY_EVENT_ITEM

#ifdef SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC
			EI_THE_ESSENCE_OF_WEAK_HERETIC_POTION			= 85003830,	// �̺�Ʈ ���� : �̾��� ������ ����
			EI_THE_ESSENCE_OF_HERETIC_POTION				= 85003831,	// �̺�Ʈ ���� : ������ ����
#endif SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC
#ifdef SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
			EI_BUFF_RURIEL_MANA_ENERGIZE_POTION				= 60008058,	// �̺�Ʈ ���� : �縮���� ���� Ȱ��ȭ ����
#endif SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
#endif //SERV_GLOBAL_EVENT_ITEM
//----------------------------------------------------------------------------------//
		};
		//}}

		//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
		enum HENIR_DUNGEON_REWARD
		{
			HDR_TIMESPACE_PIECE			= 61205, // �ð����� ���� 
		};
#endif SERV_NEW_HENIR_TEST
		//}}

		//{{ 2012. 04. 04	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
#ifdef	SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP 	// ���볯¥: 2013-06-20
		enum COOLTIME_ITEM_GROUP
		{
			CIG_NONE				= 0,
			CIG_HP_POTION,
			CIG_FOOD,
			CIG_AP_POTION,
			CIG_MP_POTION,
			CIG_DRINK,
			CIG_THROW_ITEM,
			CIG_MONSTER_CARD,
			CIG_DEFENCE_MONSTER,
			CIG_DEFENCE_MONSTER_SPECIAL,

			CIG_EVENT_APPLE,
			CIG_EVENT_RECOVER_ITEM,
			CIG_SPECIA_FOOD,
			CIG_SPECIAL_RECOVER_ITEM,
			CIG_ANTI_PITION,

			CIG_FULL_RECOVER_ITEM,
			CIG_SHERBET_ITEM,
			CIG_COBO_FULL_RECOVER_ITEM,
			CIG_PILL_ITEM,
			CIG_MP_ELIXIR_ITEM,
			CIG_WEDDING_CAKE_ITEM,
			CIG_WEDDING_DRINK_ITEM,

			CIG_MAX,
		};
#else
		enum COOLTIME_ITEM_GROUP
		{
			CIG_NONE				= 0,
			CIG_HP_POTION			= 1,
			CIG_FOOD				= 2,
			CIG_AP_POTION			= 3,
			CIG_MP_POTION			= 21,
			CIG_DRINK				= 22,
			CIG_THROW_ITEM			= 41,
			CIG_MONSTER_CARD		= 101,
			CIG_DEFENCE_MONSTER		= 111,
			CIG_DEFENCE_MONSTER_SPECIAL		= 112,
		};
#endif	// SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-20
		enum RIDING_PET_CREATE_ITEM
		{
			RPCI_NONE	= 0,
			RPCI_1		= 1,
			RPCI_2		= 2,
			RPCI_3		= 3,
			RPCI_MAX,
		};
#endif	// SERV_RIDING_PET_SYSTM

#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
		enum CASH_SKILL_POINT
		{
			CSP_NONE = 0,
			CSP_30_POINT = 30,
			CSP_60_POINT = 60,
#ifdef SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
			CSP_130_POINT = 130,
#endif SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
		};
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

		struct SpecialAbility
		{
			DWORD		m_Type;
			int							m_CoolTime;
			int							m_Value1;
			int							m_Value2;
			int							m_Value3;

			SpecialAbility()
			{
				m_Type			= SAT_NONE;
				m_CoolTime		= 0;
				m_Value1		= 0;
				m_Value2		= 0;
				m_Value3		= 0;
			}
		};

		//{{ 2013. 05. 10	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		struct SStatRelationLevel
		{
			bool		m_bIsCommonItem;
			BYTE		m_byBaseHPRelLV;
			BYTE		m_byAtkPhysicRelLV;
			BYTE		m_byAtkMagicRelLV;
			BYTE		m_byDefPhysicRelLV;
			BYTE		m_byDefMagicRelLV;

			SStatRelationLevel()
			{
				m_bIsCommonItem		= false;
				m_byBaseHPRelLV		= 0;
				m_byAtkPhysicRelLV	= 0;
				m_byAtkMagicRelLV	= 0;
				m_byDefPhysicRelLV	= 0;
				m_byDefMagicRelLV	= 0;
			}
		};
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}

		struct ItemTemplet
		{
			USE_TYPE					m_UseType;					// ��� ��� ����������� �ƴ���, ��������� ������� �����ϴ���
			USE_CONDITION				m_UseCondition;				// ��� ����
			int							m_UseLevel;					// ��� ���� ����
			CXSLUnit::UNIT_TYPE			m_UnitType;					// ��� unittype
			CXSLUnit::UNIT_CLASS		m_UnitClass;				// ��� unit Ŭ����
			//{{ 2011. 07. 27	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
			CXSLUnit::PVP_RANK			m_BuyPvpRankCondition;		// ��� ��ũ
#endif SERV_PVP_NEW_SYSTEM
			//}}

			int							m_ItemID;					//������ ID
			std::wstring				m_Name;						//������ �̸�
			std::wstring				m_Description;				//������ ����
			std::wstring				m_DescriptionInShop;		//������ ������ ���������� ���̴� ����


			//{{ dmlee 2008.11.29 
			//std::wstring				m_ModelName;				//������ ���� _Hyper �� ���δ�
			std::wstring				m_ModelName[ MAX_MODEL_COUNT_A_ITEM ];	//������ ���� _Hyper �� ���δ�
			//}} dmlee 2008.11.29 

			std::wstring				m_TextureChangeXETName;		//������ ���� _Hyper �� ���δ�
			std::wstring				m_AniXETName;				//������ ���� _Hyper �� ���δ�
			std::wstring				m_AniName;					//������ ���� _Hyper �� ���δ�
			std::wstring				m_ShopImage;
			std::wstring				m_DropViewer;				//��ӽ� ����� ��ƼŬ �ý��� �̸�

			ITEM_TYPE					m_ItemType;
			//{{ 2007. 10. 31  ������  
			ITEM_GRADE					m_ItemGrade;				//������ ���
			//}}
			bool						m_bFashion;					//�м� ������
			bool						m_bVested;					//�ͼ� ������(�絵�Ҵ�/�ǸźҴ�)			
			bool						m_bCanEnchant;				//��ȭ���� �������ΰ�
			bool						m_bCanUseInventory;			//�κ��丮���� ��밡���� �������ΰ�
			//{{ 2009. 7. 30  ������	�����Ұ�
			bool						m_bNoEquip;					//�����Ҽ� ���� ����ΰ�(��ȯ�� ���)
			//}}
			//{{ 2010. 01. 05  ������	PC�� �����̾�
			bool						m_bIsPcBang;				//PC�� ���� ������
			//}}
			//{{ 2011. 10. 14	������	��Ʋ�ʵ� ������ ���� �ý���
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
			int							m_iItemLevel;				// ������ ���� ����
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
			//}}

			//{{ dmlee 2008.11.29 
			//std::wstring				m_AttachFrameName;			//��� ���� �ٴ°�
			std::wstring				m_AttachFrameName[ MAX_MODEL_COUNT_A_ITEM ];	// ��� ���� �ٴ°�
			//}} dmlee 2008.11.29 
			bool						m_bCanHyperMode;			//������ ��� ���� �������ΰ�

			PERIOD_TYPE					m_PeriodType;				//������ ����Ⱓ
			short						m_Endurance;				//�ʱ⳻����
			short						m_EnduranceDamageMin;		//1���������÷��̴系���� ���ҷ� �ּ�
			short						m_EnduranceDamageMax;		//1���������÷��̴系���� ���ҷ� �ִ�
			//{{ 2007. 11. 16  ������  
			float						m_RepairED;					//������ 1 ������ ED
			int							m_RepairVP;					//������ 1 ������ VP
			//}}
			int							m_Quantity;					//1��������
			
			SHOP_PRICE_TYPE				m_PriceType;				//����Ÿ��
			int							m_Price;					//����
			int							m_PricePvPPoint;			//���Ŵ�������Ʈ


			CXSLUnit::EQIP_POSITION		m_EqipPosition;				//���� ��ġ
			CXSLStat::Stat				m_Stat;						//������ ����

            std::vector<SpecialAbility>    m_SpecialAbilityList;
			//{{ dmlee 2008.11.14 ������ �����Ӽ��� ���Ͽɼ� �߰�
			//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
			std::vector< int >				m_vecSocketOption;
#else
			std::vector< short >			m_vecSocketOption;
#endif SERV_ITEM_OPTION_DATA_SIZE
			//}} 
			//}} dmlee 2008.11.14 ������ �����Ӽ��� ���Ͽɼ� �߰�

			//{{ 2013. 05. 10	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			std::vector< int >			m_vecRandomSocketGroupID;
			SStatRelationLevel			m_kStatRelationLevel;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}

			int							m_CoolTime;
			int							m_SetID;

			//{{ 2009. 9. 2  ������		����
			BYTE						m_ucMaxSealCount;			// �ִ�� ������ �� �ִ� Ƚ��
			//}}
			//{{ 2009. 11. 17  ������	�Ӽ���ȭ���ɰ���
			int							m_iMaxAttribEnchantCount;
			//}}

			//{{ 2012. 06. 28	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2_1
			short						m_iAttributeLevel;			// �䱸 �Ӽ� ����
#endif SERV_2012_PVP_SEASON2_1
			//}}

			//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
			int							m_iBuffFactorID;			// ���� ���� id
#endif SERV_SERVER_BUFF_SYSTEM
			//}

#ifdef SERV_RESTRICTED_TO_MOVE_TO_BANK
			bool						m_bInventoryOnly;			// ����, ���κ� �̵�����
#endif SERV_RESTRICTED_TO_MOVE_TO_BANK

			ItemTemplet()
			{
				m_UseType			= UT_NONE;		// ��� ��� ����������� �ƴ���, ��������� ������� �����ϴ���
				m_UseCondition		= UC_NONE;	// ��� ����
				m_UseLevel			= 0;						// ��� ���� ����
				m_UnitType			= CXSLUnit::UT_NONE;		// ��� unittype
				m_UnitClass			= CXSLUnit::UC_NONE;		// ��� unit Ŭ����
				//{{ 2011. 07. 27	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
				m_BuyPvpRankCondition = CXSLUnit::PVPRANK_NONE; // ��� ��ũ
#endif SERV_PVP_NEW_SYSTEM
				//}}
				
				m_ItemID			= -1;						//������ ID

				m_ItemType			= IT_NONE;
				//{{ 2007. 10. 31  ������  
				m_ItemGrade			= IG_NONE;					//������ ���
				//}}
				m_bFashion			= false;					//�м� ������
				m_bVested			= false;					//�ͼ� ������(�絵�Ҵ�/�ǸźҴ�)				
				m_bCanEnchant		= false;					//��ȭ���� �������ΰ�
				m_bCanUseInventory	= false;					//�κ��丮���� ��밡���� �������ΰ�
				m_bNoEquip			= false;					//�����Ұ�
				//{{ 2010. 01. 05  ������	PC�� �����̾�
				m_bIsPcBang			= false;					//PC�� ���� ������
				//}}
				//{{ 2011. 10. 14	������	��Ʋ�ʵ� ������ ���� �ý���
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
				m_iItemLevel		= 0;						// ������ ���� ����
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
				//}}
				
				m_bCanHyperMode		= false;					//������ ��� ���� �������ΰ�

				m_PeriodType		= PT_INFINITY;				//������ ����Ⱓ
				m_Endurance			= -1;						//�ʱ⳻����
				m_EnduranceDamageMin	= 0;					//1���������÷��̴系���� ���ҷ�
				m_EnduranceDamageMax	= 0;					//1���������÷��̴系���� ���ҷ�
				//{{ 2007. 11. 16  ������  
				m_RepairED			= 0.0f;						//������ ���� ED
				m_RepairVP			= 0;						//������ ���� VP
				//}}
				m_Quantity			= 0;						//1��������
				
				m_PriceType			= SPT_NONE;					//����Ÿ��
				m_Price				= 0;						//����
				m_PricePvPPoint		= 0;						//���Ŵ�������Ʈ
				
				m_UnitType			= CXSLUnit::UT_NONE;		//���� ����
				m_UnitClass			= CXSLUnit::UC_NONE;		//���� Ŭ����

				m_EqipPosition		= CXSLUnit::EP_NONE;		//���� ��ġ

				m_CoolTime			= 0;
				m_SetID				= 0;

				//{{ 2009. 9. 2  ������		����
				m_ucMaxSealCount	= 0;						// �ִ�� ������ �� �ִ� Ƚ��
				//}}
				//{{ 2009. 11. 17  ������	�Ӽ���ȭ���ɰ���
				m_iMaxAttribEnchantCount = 0;
				//}}		

				//{{ 2012. 06. 28	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2_1
				m_iAttributeLevel	= 0;						// �䱸 �Ӽ� ����
#endif SERV_2012_PVP_SEASON2_1
				//}}

				//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
				m_iBuffFactorID		= 0;
#endif SERV_SERVER_BUFF_SYSTEM
				//}
#ifdef SERV_RESTRICTED_TO_MOVE_TO_BANK
				m_bInventoryOnly	= false;
#endif SERV_RESTRICTED_TO_MOVE_TO_BANK
			}

			//{{ 2013. 05. 20	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			bool IsCommonItem() const		{ return m_kStatRelationLevel.m_bIsCommonItem; }
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}
		};

		struct ItemData
		{
			UidType				m_ItemUID;				//UID
			int					m_ItemID;

			PERIOD_TYPE         m_PeriodType;
			short				m_Endurance;			//������
			int					m_Quantity;				//����

			int					m_EnchantAttribute;
			int					m_EnchantLevel;

			ItemData()
			{
				m_ItemUID				= 0;				//UID
				m_ItemID				= 0;

				m_PeriodType            = PT_INFINITY;
				m_Endurance				= -1;				//������
				m_Quantity				= 0;				//����

				m_EnchantAttribute		= 0;
				m_EnchantLevel			= 0;
			}
			ItemData& operator=( const KItemInfo& data )
			{
//				m_ItemUID				= data.m_iItemUID;					//UID
				m_ItemID				= data.m_iItemID;

				m_PeriodType            = (CXSLItem::PERIOD_TYPE)data.m_cUsageType;
				//m_Endurance				= data.m_iEndurance;				//������
				m_Quantity				= data.m_iQuantity;					//����

				//m_EnchantAttribute		= data.m_iEType;
				//m_EnchantLevel			= data.m_iELevel;

				return *this;
			}

			void MakePacket( KItemInfo& itemInfo )
			{
//				itemInfo.m_iItemUID				= m_ItemUID;				//UID
				itemInfo.m_iItemID				= m_ItemID;

				itemInfo.m_cUsageType           = m_PeriodType;
				//itemInfo.m_iEndurance			= m_Endurance;			//������
				itemInfo.m_iQuantity			= m_Quantity;			//����

				//itemInfo.m_iEType				= m_EnchantAttribute;
				//itemInfo.m_iELevel				= m_EnchantLevel;
			}
		};

	public:
		CXSLItem( ItemData* pItemData );
		~CXSLItem(void);

		ItemData*		GetItemData()			{ return m_pItemData; }
		ItemTemplet*	GetItemTemplet()		{ return m_pItemTemplet; }

		UidType			GetUID()				{ return m_pItemData->m_ItemUID; }
		void			SetEqip( bool bEqip )	{ m_bEqip = bEqip; }
		bool			GetEqip()				{ return m_bEqip; }

	public:
		//{{ 2009. 7. 28  ������	GM����
		static	bool	IsGMweapon( const int iItemID );
		//}}
		//{{ 2009. 9. 22  ������	����ĳ��
		static	char	GetCashItemChangeUnitClass( const int iItemID );
		static  int		GetCashItemByUnitClass( const char cUnitClass );
		//}}
		//{{ 2009. 12. 14  ������	�����ڵ�����
		static	bool	IsJobChangeCashItem( const int iItemID ) { return ( GetCashItemChangeUnitClass( iItemID ) != CXSLUnit::UC_NONE ); }
		//}}
		//{{ 2010. 04. 26  ������	PC�� ���� ĳ����
		static	bool	IsPcBangOnlyCashItem( const int iItemID );
		//}}

#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
		static	bool	IsKeepItemShowItem( const int iItemID );
		static	bool	IsKeepItemShowItem2( const int iItemID );
		static	bool	IsKeepItemShowItem3( const int iItemID );
		static	bool	IsKeepItemShowItem4( const int iItemID );
		static	bool	IsKeepItemShowItem5( const int iItemID );
		static	bool	IsKeepItemShowItem6( const int iItemID );
#endif //SERV_KEEP_ITEM_SHOW_CASHSHOP

#ifndef DELETE_EXCEPTION_WISHLIST_CHANGEJOBITEM //ĳ�Ø� ���ϱ⿡�� ���� �����ۿ� ���� ����ó�� ����.
		//{{ 2010. 03. 30  ������ �������������� Ȯ���ϴ� �Լ�
		static	bool	IsChangeJobItem( const int iItemID );
#endif //DELETE_EXCEPTION_WISHLIST_CHANGEJOBITEM

		static	bool	IsBankExpandItem( const int iItemID );
		//}}

#ifdef SERV_SKILL_PAGE_SYSTEM
		static	bool	IsItemToMakeSkillPageExpanded( const int iItemID_ );
#endif // SERV_SKILL_PAGE_SYSTEM


		//{{ 2011. 05. 11	������	�븮����
#ifdef SERV_PSHOP_AGENCY
		static	int		GetPShopAgencyDays( IN const int iItemID );
        static  int     GetPShopAgencyType( IN const int iItemID_ );

#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2011. 06. 01	������	��ȭ�� ������ ���� ť��
#ifdef SERV_ENCHANTED_ITEM_CUBE_REWARD
		//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		static	bool	UpdateEnchantedItemCubeReward( IN const int iCubeItemID, IN OUT KItemInfo& kInsertItemInfo );
#else
		static	void	UpdateEnchantedItemCubeReward( IN const int iCubeItemID, IN OUT KItemInfo& kInsertItemInfo );
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}
#endif SERV_ENCHANTED_ITEM_CUBE_REWARD
		//}}

#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
		//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
		static	int		GetComeBackRewardSkillPointDays( IN const int iItemID );
#endif SERV_COME_BACK_USER_REWARD
		//}}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
		
		//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
		static	int		GetClassChangeCashItem( const int iItemID );
		static	bool	IsClassChangeCashItem( const int iItemID ) { return ( GetClassChangeCashItem( iItemID ) != CXSLUnit::UC_NONE ); }
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM_POST_MESSEGE_FIX
		static int		GetClassChangeCashItemIDByUnitClass( const int iUnitClass_ );
#endif SERV_UNIT_CLASS_CHANGE_ITEM_POST_MESSEGE_FIX
#endif SERV_UNIT_CLASS_CHANGE_ITEM
		//}}

		//{{ 2012. 02. 22	�ڼ���	�� �̸� �����
#ifdef SERV_PET_CHANGE_NAME
		static	bool	IsPetNameChangeCashItem( const int iItemID ) { return iItemID == CXSLItem::CI_PET_NAME_CHANGE; }
#endif SERV_PET_CHANGE_NAME
		//}}
#ifdef SERV_WISH_ITEM_DISABLE
		static const bool	IsPossibleCart( const int iItemID_ );
#endif SERV_WISH_ITEM_DISABLE

#ifdef SERV_GOLD_TICKET_SEAL_COUNT
		static bool		IsGoldTicket( const int iItemID );
#endif SERV_GOLD_TICKET_SEAL_COUNT

	private:
		ItemData*		m_pItemData;
		ItemTemplet*	m_pItemTemplet;

		bool			m_bEqip;
};



