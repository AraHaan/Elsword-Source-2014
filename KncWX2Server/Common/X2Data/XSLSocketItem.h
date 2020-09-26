#pragma once

#include "RefreshSingleton.h"
#include "Lottery.h"
#include "XSLStat.h"

class CXSLSocketItem
{
public:
	enum SOCKET_DATA_TYPE
	{
		SDT_NONE		 = 0,
		SDT_WEAPON_NORMAL,
		SDT_WEAPON_RARE,
		SDT_DEFENCE_NORMAL,
		SDT_DEFENCE_RARE,
		SDT_SET_ITEM,
        SDT_ACCESSORY,
		SDT_TITLE,
		SDT_WEAPON_HARMONY,
		SDT_DEFENCE_HARMONY,
		SDT_WEAPON_MYSTERY,
		SDT_DEFENCE_MYSTERY,
		SDT_WEAPON_SHINE,
		SDT_DEFENCE_SHINE,
		SDT_WEAPON_BATTLE,
		SDT_DEFENCE_BATTLE,
		SDT_WEAPON_INCREASE,
		SDT_DEFENCE_INCREASE,
		SDT_WEAPON_HIT,
		SDT_DEFENCE_HIT,
		SDT_WEAPON_ATK_PHYSIC,
		SDT_DEFENCE_ATK_PHYSIC,
		SDT_WEAPON_ATK_MAGIC,
		SDT_DEFENCE_ATK_MAGIC,
		SDT_WEAPON_DEF_PHYSIC,
		SDT_DEFENCE_DEF_PHYSIC,
		SDT_WEAPON_DEF_MAGIC,
		SDT_DEFENCE_DEF_MAGIC,
		SDT_WEAPON_HYPER_TIME,
		SDT_DEFENCE_HYPER_TIME,
		SDT_WEAPON_INCREASE_HP,
		SDT_DEFENCE_INCREASE_HP,
		SDT_WEAPON_HYPER_CHARGE,
		SDT_DEFENCE_HYPER_CHARGE,
		SDT_WEAPON_SPEED,
		SDT_DEFENCE_SPEED,
		SDT_WEAPON_CRITICAL,
		SDT_DEFENCE_CRITICAL,
		SDT_WEAPON_MOVE_SPEED,
		SDT_DEFENCE_MOVE_SPEED,
		SDT_WEAPON_STRIKE,
		SDT_DEFENCE_STRIKE,
		SDT_WEAPON_AVOID,
		SDT_DEFENCE_AVOID,
		SDT_WEAPON_JUMP_SPEED,
		SDT_DEFENCE_JUMP_SPEED,
//{{ 2011. 07. 27 ��μ�   ���� ������ �߰�
		SDT_WEAPON_ADD_DAMAGE,
		SDT_DEFENCE_ADD_DAMAGE,
		SDT_WEAPON_ADD_DAMAGE_REDUCE,
		SDT_DEFENCE_ADD_DAMAGE_REDUCE,
//}}
		//{{ 2011. 08. 30	������	��� ������ ����
//#ifdef SERV_NEW_DUAL_MAGIC_STONE
		SDT_WEAPON_DESTROY,
		SDT_DEFENCE_DESTROY,
		SDT_WEAPON_EXPLOSION,
		SDT_DEFENCE_EXPLOSION,
		SDT_WEAPON_BURN,
		SDT_DEFENCE_BURN,
		SDT_WEAPON_QUICK,
		SDT_DEFENCE_QUICK,
		SDT_WEAPON_FURY,
		SDT_DEFENCE_FURY,
//#endif SERV_NEW_DUAL_MAGIC_STONE
		//}}
		//{{ 2012. 07. 09	��μ�       ������ ������
//#ifdef SERV_DUNGEON_CLEAR_PAYMENT_STONE_EVENT
		SDT_WEAPON_SAGE,
		SDT_DEFENCE_SAGE,
//#endif SERV_DUNGEON_CLEAR_PAYMENT_STONE_EVENT
		//}}

		//{{ 2012. 09. 19	�ڼ���	2012 ���� ����2
//#ifdef SERV_2012_PVP_SEASON2
		SDT_WEAPON_HYPER_TIME_PVP,			/// ��ÿ� ������ : ���� ���� ���� �ð�
		SDT_DEFENCE_HYPER_TIME_PVP,			/// ��ÿ� ������ : �� ���� ���� �ð�
		SDT_WEAPON_INCREASE_HP_PVP,			/// ��ÿ� ������ : ���� ü��
		SDT_DEFENCE_INCREASE_HP_PVP,		/// ��ÿ� ������ : �� ü��
		SDT_WEAPON_HYPER_CHARGE_PVP,		/// ��ÿ� ������ : ���� ���� ���� �ӵ�
		SDT_DEFENCE_HYPER_CHARGE_PVP,		/// ��ÿ� ������ : �� ���� ���� �ӵ�
		SDT_WEAPON_SPEED_PVP,				/// ��ÿ� ������ : ���� ���ۼӵ�
		SDT_DEFENCE_SPEED_PVP,				/// ��ÿ� ������ : �� ���ۼӵ�
		SDT_WEAPON_CRITICAL_PVP,			/// ��ÿ� ������ : ���� ũ��Ƽ��
		SDT_DEFENCE_CRITICAL_PVP,			/// ��ÿ� ������ : �� ũ��Ƽ��
		SDT_WEAPON_MOVE_SPEED_PVP,			/// ��ÿ� ������ : ���� �̵���
		SDT_DEFENCE_MOVE_SPEED_PVP,			/// ��ÿ� ������ : �� �̵���
		SDT_WEAPON_STRIKE_PVP,				/// ��ÿ� ������ : ���� ����
		SDT_DEFENCE_STRIKE_PVP,				/// ��ÿ� ������ : �� ����
		SDT_WEAPON_AVOID_PVP,				/// ��ÿ� ������ : ���� ȸ��
		SDT_DEFENCE_AVOID_PVP,				/// ��ÿ� ������ : �� ȸ��
		SDT_WEAPON_JUMP_SPEED_PVP,			/// ��ÿ� ������ : ���� ������
		SDT_DEFENCE_JUMP_SPEED_PVP,			/// ��ÿ� ������ : �� ������
		SDT_WEAPON_ADD_DAMAGE_PVP,			/// ��ÿ� ������ : ���� �߰� ������
		SDT_DEFENCE_ADD_DAMAGE_PVP,			/// ��ÿ� ������ : �� �߰� ������
		SDT_WEAPON_ADD_DAMAGE_REDUCE_PVP,	/// ��ÿ� ������ : ���� ������ ����
		SDT_DEFENCE_ADD_DAMAGE_REDUCE_PVP,	/// ��ÿ� ������ : �� ������ ����
		SDT_WEAPON_DESTROY_PVP,				/// ��ÿ� ������ : ���� �ı�
		SDT_DEFENCE_DESTROY_PVP,			/// ��ÿ� ������ : �� �ı�
		SDT_WEAPON_EXPLOSION_PVP,			/// ��ÿ� ������ : ���� ����
		SDT_DEFENCE_EXPLOSION_PVP,			/// ��ÿ� ������ : �� ����
		SDT_WEAPON_BURN_PVP,				/// ��ÿ� ������ : ���� ����
		SDT_DEFENCE_BURN_PVP,				/// ��ÿ� ������ : �� ����
		SDT_WEAPON_QUICK_PVP,				/// ��ÿ� ������ : ���� �ż�
		SDT_DEFENCE_QUICK_PVP,				/// ��ÿ� ������ : �� �ż�
		SDT_WEAPON_FURY_PVP,				/// ��ÿ� ������ : ���� �ݳ�
		SDT_DEFENCE_FURY_PVP,				/// ��ÿ� ������ : �� �ݳ�
//#endif SERV_2012_PVP_SEASON2
		//}}
		SDT_PVP_TAG_POINT,					// ???
		//{{ 2012. 12. 11  ���� ȿ�� ������ �߰� - ��μ�
//#ifdef SERV_FIXED_EFFECTS_SOCKET_ITEM
		SDT_WEAPON_SPEED_2P5_FIXED,				// ���� ȿ�� ������ 2.5% : ���� �ӵ� 
		SDT_DEFFENCE_SPEED_2P5_FIXED,			// ���� ȿ�� ������ 2.5% : �� �ӵ�
		SDT_WEAPON_SPEED_5P_FIXED,				// ���� ȿ�� ������ 5% : ���� �ӵ�
		SDT_DEFFENCE_SPEED_5P_FIXED,			// ���� ȿ�� ������ 5% : �� �ӵ�
		SDT_WEAPON_CRITICAL_2P5_FIXED,			// ���� ȿ�� ������ 2.5% : ���� ũ��Ƽ��
		SDT_DEFFENCE_CRITICAL_2P5_FIXED,		// ���� ȿ�� ������ 2.5% : �� ũ��Ƽ��
		SDT_WEAPON_CRITICAL_5P_FIXED,			// ���� ȿ�� ������ 5% : ���� ũ��Ƽ��
		SDT_DEFFENCE_CRITICAL_5P_FIXED,			// ���� ȿ�� ������ 5% : �� ũ��Ƽ��
		SDT_WEAPON_AVOID_2P5_FIXED,				// ���� ȿ�� ������ 2.5% : ���� ȸ��
		SDT_DEFFECNE_AVOID_2P5_FIXED,			// ���� ȿ�� ������ 2.5% : �� ȸ��
		SDT_WEAPON_AVOID_5P_FIXED,				// ���� ȿ�� ������ 5% : ���� ȸ��
		SDT_DEFFENCE_AVOID_5P_FIXED,			// ���� ȿ�� ������ 5% : �� ȸ��
		SDT_WEAPON_ADD_DAMAGE_2P5_FIXED,		// ���� ȿ�� ������ 2.5% : ���� �߰� ������
		SDT_DEFFENCE_ADD_DAMAGE_2P5_FIXED,		// ���� ȿ�� ������ 2.5% : �� �߰� ������
		SDT_WEAPON_ADD_DAMAGE_5P_FIXED,			// ���� ȿ�� ������ 5% : ���� �߰� ������
		SDT_DEFFENCE_ADD_DAMAGE_5P_FIXED,		// ���� ȿ�� ������ 5% : �� �߰� ������
		SDT_WEAPON_DAMAGE_REDUCE_2P5_FIXED,		// ���� ȿ�� ������ 2.5% : ���� ������ ����
		SDT_DEFFECNE_DAMAGE_REDUCE_2P5_FIXED,	// ���� ȿ�� ������ 2.5% : �� ������ ����
		SDT_WEAPON_DAMAGE_REDUCE_5P_FIXED,		// ���� ȿ�� ������ 5% : ���� ������ ����
		SDT_DEFFENCE_DAMAGE_REDUCE_5P_FIXED,	// ���� ȿ�� ������ 5% : �� ������ ����
		SDT_WEAPON_HYPER_CHARGE_5P_FIXED,		// ���� ȿ�� ������ 5% : ���� ���� ����
		SDT_DEFFENCE_HYPER_CHARGE_5P_FIXED,		// ���� ȿ�� ������ 5% : �� ���� ����
		SDT_WEAPON_HYPER_CHARGE_10P_FIXED,		// ���� ȿ�� ������ 10% : ���� ���� ����
		SDT_DEFFENCE_HYPER_CHARGE_10P_FIXED,	// ���� ȿ�� ������ 10% : �� ���� ����
//#endif SERV_FIXED_EFFECTS_SOCKET_ITEM

		SDT_SKILL_LV_GROUP,						/// ������ ���� �ٸ� ��ų�� �ο��ϱ� ���� ��ų �׷�
		//}}

		SDT_QUEST_CLEAR_COUNT 					= 120, /// �ؿ��� ���� �۾�. 2013-09-03 ��â��
		SDT_ITEM_EXCHANGE_COUNT 				= 121, /// �ؿ��� ���� �۾�. 2013-09-03 ��â��
	};

	enum MAGIC_STONE_ITEM_ID
	{
		MSI_NONE = 0,
		MSI_MAGIC_STONE				= 109945,
		MSI_RARE_MAGIC_STONE		= 109955,
		//{{ 2010. 04. 22  ������	���� ����
		MSI_HARMONY_MAGIC_STONE		= 132110,
		MSI_MYSTERY_MAGIC_STONE		= 132111,
		MSI_SHINE_MAGIC_STONE		= 132112,
		MSI_BATTLE_MAGIC_STONE		= 132113,
		MSI_INCREASE_MAGIC_STONE	= 132114,
		MSI_HIT_MAGIC_STONE			= 132115,

		MSI_ATK_PHYSIC_MAGIC_STONE	= 132193, // ���� ���ݷ��� ������
		MSI_ATK_MAGIC_MAGIC_STONE	= 132194, // ���� ���ݷ��� ������
		MSI_DEF_PHYSIC_MAGIC_STONE	= 132195, // ���� ������ ������
		MSI_DEF_MAGIC_MAGIC_STONE	= 132196, // ���� ������ ������
		MSI_HYPER_TIME_MAGIC_STONE	= 132197, // ���� ������ ������
		MSI_INCREASE_HP_MAGIC_STONE	= 132198, // ü���� ������
		MSI_HYPER_CHARGE_MAGIC_STONE= 132199, // ���� ������ ������
		MSI_SPEED_MAGIC_STONE		= 132200, // �ӵ��� ������
		MSI_CRITICAL_MAGIC_STONE	= 132201, // ũ��Ƽ���� ������
		MSI_MOVE_SPEED_MAGIC_STONE	= 132202, // �̵� �ӵ��� ������
		MSI_STRIKE_MAGIC_STONE		= 132203, // ������ ������
		MSI_AVOID_MAGIC_STONE		= 132204, // ȸ���� ������0
		MSI_JUMP_SPEED_MAGIC_STONE	= 132205, // ���� �ӵ��� ������
		//}}
		//{{ 2011. 07. 27 ��μ�   ���� ������ �߰�
		MSI_INCREASE_DAMEGE_STONE	= 72510,  // �߰� �������� ������
		MSI_DECREASE_DAMEGE_STONE	= 72520,  // ������ ������ ������
		//}}
		//{{ 2011. 08. 30	������	��� ������ ����
#ifdef SERV_NEW_DUAL_MAGIC_STONE
		MSI_DESTROY_MAGIC_STONE		= 111701, // �ı��� ������
		MSI_EXPLOSION_MAGIC_STONE	= 111702, // ������ ������
		MSI_BURN_MAGIC_STONE		= 111703, // ��Ÿ�� ������
		MSI_QUICK_MAGIC_STONE		= 111704, // ������ ������
		MSI_FURY_MAGIC_STONE		= 111705, // �г��� ������
#endif SERV_NEW_DUAL_MAGIC_STONE
		//}}
		//{{ 2012. 07. 09	��μ�       ������ ������
//#ifdef SERV_DUNGEON_CLEAR_PAYMENT_STONE_EVENT
		MSI_WISE_MAN_STONE_ITEM		= 135184, // ������ ������
//#endif SERV_DUNGEON_CLEAR_PAYMENT_STONE_EVENT
		//}}

		//{{ 2012. 09. 19	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		MSI_MOVE_SPEED_PVP			= 98004,	/// ��ÿ� ������ : �̵���
		MSI_JUMP_SPEED_PVP			= 98005,	/// ��ÿ� ������ : ������
		MSI_INCREASE_HP_PVP			= 98006,	/// ��ÿ� ������ : ü��
		MSI_STRIKE_PVP				= 98007,	/// ��ÿ� ������ : ����
		MSI_AVOID_PVP				= 98008,	/// ��ÿ� ������ : ȸ��
		MSI_HYPER_CHARGE_PVP		= 98009,	/// ��ÿ� ������ : ���� ���� �ӵ�
		MSI_HYPER_TIME_PVP			= 98010,	/// ��ÿ� ������ : ���� ���� �ð�
		MSI_CRITICAL_PVP			= 98011,	/// ��ÿ� ������ : ũ��Ƽ��
		MSI_SPEED_PVP				= 98012,	/// ��ÿ� ������ : ���ۼӵ�
		MSI_ADD_DAMAGE_PVP			= 98013,	/// ��ÿ� ������ : �߰� ������
		MSI_ADD_DAMAGE_REDUCE_PVP	= 98014,	/// ��ÿ� ������ : ������ ����
		MSI_EXPLOSION_PVP			= 98015,	/// ��ÿ� ������ : ����
		MSI_DESTROY_PVP				= 98016,	/// ��ÿ� ������ : �ı�
		MSI_QUICK_PVP				= 98017,	/// ��ÿ� ������ : �ż�
		MSI_BURN_PVP				= 98018,	/// ��ÿ� ������ : ����
		MSI_FURY_PVP				= 98019,	/// ��ÿ� ������ : �ݳ�
#endif SERV_2012_PVP_SEASON2
		//}}
		//{{ 2012. 12. 11  ���� ȿ�� ������ �߰� - ��μ�
#ifdef SERV_FIXED_EFFECTS_SOCKET_ITEM
		MSI_SPEED_MAGIC_STONE_FIXED_EFFECT			= 98028,	/// ���� �ӵ� ���� ������
		MSI_CRITICAL_MAGIC_STONE_FIXED_EFFECT		= 98029,	/// ũ��Ƽ�� ���� ������
		MSI_INCREASE_DAMEGE_STONE_FIXED_EFFECT		= 98030,	/// �߰� ������ ���� ������
		MSI_HYPER_CHARGE_MAGIC_STONE_FIXED_EFFECT	= 98031,	/// ���� ���� �ӵ� ���� ������
		MSI_AVOID_MAGIC_STONE_FIXED_EFFECT			= 98032,	/// ȸ�� ���� ������
		MSI_DECREASE_DAMEGE_STONE_FIXED_EFFECT		= 98033,	/// ������ ���� ���� ������
#endif SERV_FIXED_EFFECTS_SOCKET_ITEM
		//}}

//#ifdef SERV_MISS_ELIOS_STONE_ITEM
		MSI_MISS_ELIOS_STONE_ITEM				= 67003750, // �̽� �������� Ư�� ������
//#endif SERV_MISS_ELIOS_STONE_ITEM

//#ifdef SERV_BC_EVENT
		MSI_VC_EVENT_STONE_ITEM					= 60005473, // �Ҳ��� ������
//#endif SERV_BC_EVENT

//#ifdef SERV_FISHING_HOLE_MAGIC_STONE
		MSI_EVENT_MAGIC_STONE_1					= 60004121,
		MSI_EVENT_MAGIC_STONE_2					= 60004122,
		MSI_EVENT_MAGIC_STONE_3					= 60004123,
		MSI_EVENT_MAGIC_STONE_4					= 60004124,
		MSI_EVENT_MAGIC_STONE_5					= 60004125,
//#endif SERV_FISHING_HOLE_MAGIC_STONE

//#ifdef SERV_LURIEL_MAGIC_STONE
		MSI_LURIEL_INCREASE_DAMEGE_STONE		= 85003840, /// �縮���� �߰� �������� ������
		MSI_LURIEL_DECREASE_DAMEGE_STONE		= 85003841, /// �縮���� ������ ������ ������
		MSI_LURIEL_HYPER_TIME_MAGIC_STONE		= 85003842, /// �縮���� ���� ������ ������
		MSI_LURIEL_INCREASE_HP_MAGIC_STONE		= 85003843, /// �縮���� ü���� ������
		MSI_LURIEL_HYPER_CHARGE_MAGIC_STONE		= 85003844, /// �縮���� ���� ������ ������
		MSI_LURIEL_SPEED_MAGIC_STONE			= 85003845, /// �縮���� �ӵ��� ������
		MSI_LURIEL_CRITICAL_MAGIC_STONE			= 85003846, /// �縮���� ũ��Ƽ���� ������
		MSI_LURIEL_MOVE_SPEED_MAGIC_STONE		= 85003847, /// �縮���� �̵� �ӵ��� ������
		MSI_LURIEL_STRIKE_MAGIC_STONE			= 85003848, /// �縮���� ������ ������
		MSI_LURIEL_AVOID_MAGIC_STONE			= 85003849, /// �縮���� ȸ���� ������
		MSI_LURIEL_JUMP_SPEED_MAGIC_STONE		= 85003850, /// �縮���� ���� �ӵ��� ������
		MSI_LURIEL_DESTROY_MAGIC_STONE			= 152000699, // �縮���� �ı��� ������(�ı�)
		MSI_LURIEL_EXPLOSION_MAGIC_STONE		= 152000700, // �縮���� ������ ������(����)
		MSI_LURIEL_BURN_MAGIC_STONE				= 152000701, // �縮���� ��Ÿ�� ������(����)
		MSI_LURIEL_QUICK_MAGIC_STONE			= 152000702, // �縮���� ������ ������(�ż�)
		MSI_LURIEL_FURY_MAGIC_STONE				= 152000703, // �縮���� �г��� ������(�ݳ�)
//#endif SERV_LURIEL_MAGIC_STONE
	};

	enum SOCKET_COUNT
	{
		SC_NONE           = -1,

		SC_WEAPON_LOW     = 0,
		SC_WEAPON_NORMAL  = 1,
		SC_WEAPON_RARE    = 2,
		SC_WEAPON_ELITE   = 3,
		SC_WEAPON_UNIQUE  = 4,

		SC_DEFENCE_LOW    = 0,
		SC_DEFENCE_NORMAL = 1,
		SC_DEFENCE_RARE   = 1,
		SC_DEFENCE_ELITE  = 2,
		SC_DEFENCE_UNIQUE = 3,
	};	

	//{{ 2013. 05. 15	������	������ ����
	//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	enum RANDOM_SOCKET_ENUM
	{
		RSC_MAX			  = 5,
	};
	//#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

	//struct SocketExtraDamage
	//{
	//	float								m_fRate;
	//	CX2DamageManager::ExtraDamageData	m_ExtraDamageData;

	//	SocketExtraDamage()
	//	{
	//		m_fRate = 0.0f;
	//	}
	//};

public:

	struct SocketData
	{
		int					m_ID;
		SOCKET_DATA_TYPE	m_Type;						//�ɼ�Ÿ��
		CXSLStat::Stat		m_Stat;						//�ɷ�ġ ����
		//float				m_fMoveSpeed;				//�޸��� �ӵ� ����(%)(�ִ� 30%)
		//float				m_fJumpSpeed;				//���� �ӵ� ����(%)(�ִ� 20%)
		//float				m_fHyperModeChargeRate;		//�ҿ� ������ ��¡ �ӵ� ����(%)(�ִ� 50%)
		//float				m_fHyperModeTime;			//���� �ð� ����(�ִ� 20��)
		float				m_fRepairPriceSale;			//������ ������ ����(%)(�ִ�90%)
		//SocketExtraDamage	m_SocketExtraDamage;

		float				m_IncreaseHPRate;

		bool				m_bDungeonOnly;
		bool				m_bPVPOnly;
		bool				m_bOverlap;

		float				m_fIncreaseEDPer;
		float				m_fIncreaseExpPer;

		//{{ 2008. 11. 15  ������	�Ӽ���ȭ
		float				m_fResistBlaze;
		float				m_fResistWater;
		float				m_fResistNature;
		float				m_fResistWind;
		float				m_fResistLight;
		float				m_fResistDark;
		//}}
		//{{ 2010. 10. 18	������	�� ���� ��ų
#ifdef SERV_PET_AURA_SKILL
		float				m_fAttackBlaze;
		float				m_fAttackWater;
		float				m_fAttackNature;
		float				m_fAttackWind;
		float				m_fAttackLight;
		float				m_fAttackDark;

		float				m_fIncreaseMPRate;
		float				m_fIncreaseAtkPhysicRate;
		float				m_fIncreaseAtkMagicRate;
		float				m_fIncreaseDefPhysicRate;
		float				m_fIncreaseDefMagicRate;
#endif SERV_PET_AURA_SKILL
		//}}

		//{{ 2012. 06. 27	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2_1
		bool				m_bIsLevelLinkedStat;
		char				m_cAddLevel;
#endif SERV_2012_PVP_SEASON2_1
		//}}
		//{{ 2013. 06. 04	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		KStatRelationLevel	m_kStatRelLVData;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}

		SocketData()
		{
			m_Type						= SDT_NONE;
			//m_fMoveSpeed				= 0.0f;
			//m_fJumpSpeed				= 0.0f;
			//m_fHyperModeChargeRate	= 0.0f;
			//m_fHyperModeTime			= 0.0f;
			m_fRepairPriceSale			= 0.0f;

			m_bDungeonOnly				= false;
			m_bPVPOnly					= false;

			m_IncreaseHPRate			= 0.f;

			m_bOverlap					= false;

			m_fIncreaseEDPer			= 0.0f;
			m_fIncreaseExpPer			= 0.0f;

			//{{ 2008. 11. 15  ������	�Ӽ���ȭ
			m_fResistBlaze				= 0.0f;
			m_fResistWater				= 0.0f;
			m_fResistNature				= 0.0f;
			m_fResistWind				= 0.0f;
			m_fResistLight				= 0.0f;
			m_fResistDark				= 0.0f;
			//}}
			//{{ 2010. 10. 18	������	�� ���� ��ų
#ifdef SERV_PET_AURA_SKILL
			m_fAttackBlaze				= 0.0f;
			m_fAttackWater				= 0.0f;
			m_fAttackNature				= 0.0f;
			m_fAttackWind				= 0.0f;
			m_fAttackLight				= 0.0f;
			m_fAttackDark				= 0.0f;

			m_fIncreaseMPRate			= 0.0f;
			m_fIncreaseAtkPhysicRate	= 0.0f;
			m_fIncreaseAtkMagicRate		= 0.0f;
			m_fIncreaseDefPhysicRate	= 0.0f;
			m_fIncreaseDefMagicRate		= 0.0f;
#endif SERV_PET_AURA_SKILL
			//}}

			//{{ 2012. 06. 27	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2_1
			m_bIsLevelLinkedStat		= false;
			m_cAddLevel					= 0;
#endif SERV_2012_PVP_SEASON2_1
			//}}
		}
	};

	typedef std::pair< int, int > KBeginEndEnchantLv;
	typedef std::map< KBeginEndEnchantLv, KLottery > KSocketGroup;

	struct SocketOptionInfo
	{
		int					m_iSocketOptionID;
		int					m_iRandomCount;
		int					m_iEnchantLevel;
		float				m_fRepairPriceSale;
		bool				m_bOverLap;

		SocketOptionInfo()
		{
			m_iSocketOptionID	= 0;
			m_iRandomCount		= 0;
			m_iEnchantLevel		= 0;
			m_fRepairPriceSale	= 0;
			m_bOverLap			= false;
		}
	};

	//{{ 2008. 8. 18  ������	��Ʈ ������
	struct SetItemData
	{
		int										m_SetID;
		std::wstring							m_SetName;
		//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
		std::map< int, std::vector< int > >	m_mapNeedPartsNumNOptions;
#else
		std::map< int, std::vector< short > >	m_mapNeedPartsNumNOptions;
#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 

		SetItemData()
		{
			m_SetID = 0;
		}
	};
	//}}

	//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	struct SMiracleCubeLevel
	{
        int				m_iMinLevel;
		int				m_iMaxLevel;

		SMiracleCubeLevel()
		{
			m_iMinLevel = 0;
			m_iMaxLevel = 0;
		}
	};
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

private:
	DeclareRefreshSingleton( CXSLSocketItem );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	CXSLSocketItem(void);
	~CXSLSocketItem(void);

	// ���� ����
	bool AddSocketData_LUA();
	bool AddSocketRandomData_LUA( int iSocketDataType, int iGroupID, float fRate );
	bool AddSocketGroup_LUA( int iGroupID, int iBeginEnchantLv, int iEndEnchantLv, int iOptionID, float fRate );

	// ��Ʈ ������ ����
	bool AddSetItemData_LUA();

	//{{ 2012. 11. 28 ĳ�� ������ ���� �ɼ� ��� ���� - ��μ�
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
	void AddSocektGroupDataForCashAvatar_LUA();
	bool CheckCashItemSocketID( IN int iType, IN int iSocketID );
	bool CheckCashItemGroupID( IN int iSocketID, IN int iGroupID  );
#endif SERV_CASH_ITEM_SOCKET_OPTION
	//}

	// ����
	const SocketData* GetSocketData( IN int iSocketOptionID ) const;
	int  GetSocketCount( IN int iItemGrade, IN int iItemType ) const;
	bool VerifySocketCount( IN int iItemGrade, IN bool bIsWeapon, IN int iSocketCount ) const;
	SOCKET_DATA_TYPE GetSocketDataType( IN int iItemID, IN int iItemType ) const;
	int  GetMagicStoneItemID( IN bool bIsRare ) const;

	//{{ 2012. 09. 26  ��μ�  ���� ��� ���� ���� ��ȭ ����
#ifdef SERV_2012_PVP_SEASON2_1
	bool GetResultItem( IN int iMagicStoneItemID, IN int iItemType, IN int iPeriodType, IN int iUseLevel, IN bool bFashion, IN int iEnchantLevel, OUT int& iResultSocketOption ) const;
#else
	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
//#ifdef SERV_ITEM_OPTION_DATA_SIZE
//	bool GetResultItem( IN int iMagicStoneItemID, IN int iItemType, IN int iPeriodType, IN bool bFashion, IN int iEnchantLevel, OUT int& iResultSocketOption ) const;
//#else
//	bool GetResultItem( IN int iMagicStoneItemID, IN int iItemType, IN int iPeriodType, IN bool bFashion, IN int iEnchantLevel, OUT short& sResultSocketOption ) const;
//#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
#endif SERV_2012_PVP_SEASON2_1
	//}}

	//{{ 2010. 04. 15  ������	���� ����
#ifdef SERV_SOCKET_NEW
	bool IsMagicStoneItemID( IN int iItemID );
	bool CalcInsertSocketCost( IN int iItemID, IN int iMagicStoneItemID, IN int iAssignedSocketCount, IN bool bEmptySlot, OUT int& iCost );
#endif SERV_SOCKET_NEW
	//}}

//	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
//	#ifdef SERV_ITEM_OPTION_DATA_SIZE
	bool GetSocketOptionStat(	IN const std::vector< int >& vecSocketOption, 
								IN bool bIsDungeon,
								IN const KStat& kBaseStat,
								IN OUT float& fTotalIncHPRate,
								OUT KStat& kAddStat,
								IN unsigned int& uiHP_OnePoint,
								IN unsigned int& iItemLevel
								//{{ 2012. 09. 25	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2_1
								, IN const bool bLevelLinkedStat = false
#endif SERV_2012_PVP_SEASON2_1
								//}}
								);
//	#else
//	void GetSocketOptionStat( IN const std::vector< short >& vecSocketOption, IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat, IN unsigned int& uiHP_OnePoint, IN unsigned int& iItemLevel );
//	#endif SERV_ITEM_OPTION_DATA_SIZE
//	//}}

	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	void GetSocketOptionIncreaseRate( IN const std::vector< int >& vecSocketOption, IN bool bIsDungeon, IN OUT KStatIncreaseRate& kStatIncreaseRateInfo );
#else
	void GetSocketOptionIncreaseRate( IN const std::vector< short >& vecSocketOption, IN bool bIsDungeon, IN OUT KStatIncreaseRate& kStatIncreaseRateInfo );
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}}

	// ��Ʈ ������	
	const SetItemData* GetSetItem( int setID );
	//{{ 2011. 07. 25    ��μ�    ������ �ɼ�ID ������ ������ ����
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	void GetSetItemOptions( int setID, int partsNum, std::vector< int >& vecOptions );
#else
	void GetSetItemOptions( int setID, int partsNum, std::vector< short >& vecOptions );
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 

	//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	bool AddRandomSocketGroupInfo_LUA( int iGroupID, int iOptionID, float fRate );	
	bool EvaluateItemRandomSocket( IN const int iItemID, OUT std::vector< int >& vecRandomSocket );

	bool AddMiracleCubeInfo_LUA( int iItemID, int iMinLevel, int iMaxLevel );
	bool IsCanUseMiracleCube( IN const int iMiracleCubeItemID, IN const int iItemLevel ) const;

	void GetAndAddStatRelationLevel( IN const std::vector< int >& vecSocketOption, IN OUT KStatRelationLevel& kAddStatRelLevel ) const;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

private:
	//{{ 2008. 11. 17  ������	������ ����
	std::map< int, KLottery >										m_mapSocketRandom;
	std::map< int, KSocketGroup >									m_mapSocketGroup;
	std::map< int, SocketData >										m_mapSocketData;
	//}}

	//{{ 2008. 8. 18  ������	
	std::map< int, SetItemData >									m_mapSetItemData;	
	//}}

	//{{ 2012. 11. 28 ĳ�� ������ ���� �ɼ� ��� ���� - ��μ�
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
	std::map< int, std::set< int > >								m_mapItemTypeSocketList;				// ������ Ÿ��, ����ID��
	std::map< int, std::set< int > >								m_mapItemGroupSocketList;				// GroupID, ����ID��
#endif SERV_CASH_ITEM_SOCKET_OPTION
	//}
	//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	std::map< int, KLottery >										m_mapRandomSocketGroup;
	std::map< int, SMiracleCubeLevel >								m_mapMiracleCubeTable;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
};

DefRefreshSingletonInline( CXSLSocketItem );


