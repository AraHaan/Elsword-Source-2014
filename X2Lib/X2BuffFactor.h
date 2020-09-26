/**@file : X2BuffFactor.h
   @breif : ������ �߻� ��Ű�� ������Ҹ� ��Ƴ��� ����
*/

#pragma once

#define STATIC_CAST_FLOAT_TO_ENUM( type_, arg ) \
	static_cast<type_>( static_cast<int>( arg ) )

enum BUFF_BEHAVIOR_TYPE
{
	BBT_NONE = 0,
	BBT_CHANGE_NOW_HP_PER_SECOND,		/// �ʴ� HP ����
   	BBT_CHANGE_NOW_MP_PER_SECOND,		/// �ʴ� MP ����
	BBT_REVERSE_LEFT_RIGHT,				/// Ư�� �������� �¿� ����
	BBT_DASH_IMPOSSIBLE,				/// ��� �Ұ�
	BBT_ATTACK_IMPOSSIBLE,				/// ���� �Ұ�
	BBT_CHANGE_MAX_HP,					/// �ִ�HP ����
	BBT_CHANGE_PHYSIC_ATTACK,			/// �������ݷ�
	BBT_CHANGE_MAGIC_ATTACK,			/// �������ݷ�
	BBT_CHANGE_PHYSIC_DEFENCE,			/// ��������
	BBT_CHANGE_MAGIC_DEFENCE,			/// ��������
	BBT_CHANGE_ANIM_SPEED,				/// ���ۼӵ�
	BBT_CHANGE_WALK_RUN_SPEED,			/// �̵��ӵ�
	BBT_CHANGE_JUMP_POWER,				/// ������
	BBT_CHANGE_ACCURACY,				/// ����
	BBT_CHANGE_AVOIDANCE,				///	ȸ��
	BBT_CHANGE_CRITICAL_RATE,			/// ũ��Ƽ�ð���Ȯ��
	BBT_CHANGE_CRITICAL_ATTACK,			/// ũ��Ƽ�ð��ݷ�(ũ��Ƽ�� �� ������ ����)
	BBT_CHANGE_ANTI_CRITICAL_RATE,		/// ũ��Ƽ�ù��Ȯ��
	BBT_CHANGE_CRITICAL_DEFENCE,		/// ũ��Ƽ�ù���(ũ��Ƽ�� �� ������ ����)
	BBT_CHANGE_ADDITIONAL_ATTACK,		/// �߰�������
	BBT_CHANGE_ADDITIONAL_DEFENCE,		/// ����������
   	BBT_CREATE_BUFF_FACTOR,				/// ������ ������ ���� ���� ���� ����
	BBT_ABSORB_EFFECT_ATTACK,			/// ����Ʈ ������ ����Ͽ� MP ȸ��
   	
	BBT_CHARGE_MP_HIT_MELEE_ATTACK,		/// ���� Ÿ�ݽ� �߰��� MP ȸ����
	BBT_CHARGE_MP_HIT_RANGE_ATTACK,		/// ���Ÿ� Ÿ�ݽ� �߰��� MP ȸ����
	BBT_CHARGE_MP_HITTED,				/// �ǰݽ� �߰��� MP ȸ����

	BBT_CHANGE_HYPER_MODE,				/// ����
	BBT_SUPER_ARMOR,					/// ���۾Ƹ�

	BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_MELEE_ATTACK,		/// �������� �ִ� ������ ����
	BBT_CHANGE_FINAL_DAMAGE_MULTIPLIER_RANGE_ATTACK,		/// ���Ÿ�(DAMAGE_EFFECT)���� �ִ� ������ ����

	BBT_CHANGE_FINAL_DEFENCE_MULTIPLIER,					///	�޴� ������ ����

	BBT_CHANGE_FORCE_DOWN_MULTIPLIER_MELEE_ATTACK,		/// �������� FORCE_DOWN ��ġ ����
	BBT_CHANGE_FORCE_DOWN_MULTIPLIER_RANGE_ATTACK,		/// ���Ÿ�(DAMAGE_EFFECT)���� FORCE_DOWN ��ġ ����

	BBT_RESET_SKILL_COOL_TIME,							/// ��ų�� ��Ÿ���� �ʱ�ȭ �����ִ� �ൿ
	BBT_CHANGE_UNIT_SCALE,								/// ������ ũ�⸦ �����ϴ� �ൿ

	BBT_CHANGE_ATTRIBUTE_BLAZE_DEFENCE,					/// �Ӽ� ����-��~���
	BBT_CHANGE_ATTRIBUTE_WATER_DEFENCE,
	BBT_CHANGE_ATTRIBUTE_NATURE_DEFENCE,
	BBT_CHANGE_ATTRIBUTE_WIND_DEFENCE,
	BBT_CHANGE_ATTRIBUTE_LIGHT_DEFENCE,
	BBT_CHANGE_ATTRIBUTE_DARK_DEFENCE,

	BBT_CHANGE_EVERY_ATTRIBUTE_DEFENCE,					/// ��� �Ӽ� ����

	BBT_CUSTOM_FUNCTION,								/// CommonStateStart, CommonFrameMove, CommonStateEnd ���� ó���Ǿ� �ϴ� �ϵ��ڵ� �Լ��� �����ϰ� �ϴ� �ൿ
	BBT_DRAIN_HP_NORMAL_ATTACK,							/// �Ϲ� ���ݽ� HP���(���� �׼� � �̿�)

	BBT_EFFECT_SET_WITH_DAMAGE,							/// �������� �ִ� ����Ʈ�� (������ ����� ������Ʈ��PowerRate�� ������ ������ ����Ʈ���� ���� ��ų�� ������ PowerRate�� �ٸ� ��찡 �����Ƿ� �������� �ִ� EffectSet�� ��� PowerRate�� ����ȭ�ϴ� Behavior ���)
	BBT_STUN,											/// ���� ������Ʈ�� ������Ű�� ���ø�
	BBT_FINISH_OTHER_BUFF_DEBUFF,						/// �ٸ� ����/������� ���� ��Ű�� �ൿ
	BBT_FROZEN,										/// ���� ������Ʈ�� ������Ű�� ���ø�
	BBT_CAN_PASS_UNIT,								/// ������ ����Ǵ� ���� ������ CanPassUnit �÷��׸� true �Ǵ� false �� ����(true �� �ٸ� ������ ���� ����ϰ���)
	BBT_ATTACK_ALL_TEAM,							/// ��� ���� �����ϴ� �ൿ
	BBT_NEVER_MOVE,									/// �����ϼ��������ϴ� �ൿ(�ǰݴ��ϰų� �ϴ���..)
	BBT_CHANGE_MAX_MP,								/// �ִ�MP ����
	BBT_CHANGE_ATTACK_BY_TYPE,						/// ���� Ÿ�Կ� ���� ���ݷ� ����(����� ��Ƽ�� ��ų, ��Ƽ�� ��ų, �Ϲ� )
	BBT_CHANGE_ENCHANT_ATTACK_RATE,					/// ���� �Ӽ� ���� �ߵ� Ȯ�� ����
	BBT_EMPTY,										/// �ƹ� �ൿ�� ���� ����
	BBT_DOUBLE_ATTACK,								/// ���� ����
	BBT_CHANGE_ATTACK_SPHERE_SCALE,					/// ĳ���Ϳ��� ���� ���ùڽ��� ũ�� ����

	BBT_DOWN_IMPOSSIBLE,							/// �ϴ��̵� �Ұ�
	BBT_ADD_SKILL_LEVEL,							/// ��ų������ ���� ��Ű�� �ൿ

	BBT_SKILL_ATTACK_IMPOSSIBLE,					/// ��ų ���� �Ұ�
	BBT_ZX_ATTACK_IMPOSSIBLE,						/// ZX ���� �Ұ�

	BBT_REFLECT_MAGIC,								/// ���� �ݻ�

	BBT_CHANGE_CONSUME_MP_RATE,						/// ���� �Һ� ����
// #ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
// Z ���� �Ұ�, X ���� �Ұ� ���� Ÿ�� �߰�
	BBT_Z_ATTACK_IMPOSSIBLE,						///	Z ���� �Ұ�
	BBT_X_ATTACK_IMPOSSIBLE,						/// x ���� �Ұ�

// �� �Ӽ� �� ��æƮ Ȯ�� ����
	BBT_CHANGE_ATTRIBUTE_BLAZE_ATTACK_RATE,			/// �Ӽ� ���� Ȯ�� ���� (��)
	BBT_CHANGE_ATTRIBUTE_WATER_ATTACK_RATE,			///  �Ӽ� ���� Ȯ�� ���� (��)
	BBT_CHANGE_ATTRIBUTE_NATURE_ATTACK_RATE,		/// �Ӽ� ���� Ȯ�� ���� (�ڿ�)
	BBT_CHANGE_ATTRIBUTE_WIND_ATTACK_RATE,			/// �Ӽ� ���� Ȯ�� ���� (�ٶ�)
	BBT_CHANGE_ATTRIBUTE_LIGHT_ATTACK_RATE,			/// �Ӽ� ���� Ȯ�� ���� (��)
	BBT_CHANGE_ATTRIBUTE_DARK_ATTACK_RATE,			/// �Ӽ� ���� Ȯ�� ���� (���)
// #endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����

//#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
	BBT_CHANGE_HYPER_CHARGE_SPEED,						/// ���� ���� �ӵ� ����
//#endif //SERV_ADD_LUNATIC_PSYKER

};

enum BUFF_DURATION_TIME_TYPE
{
	BDTT_NONE = 0,
	BDTT_NORMAL_TIME,
	BDTT_RANDOM_TIME,
	BDTT_RELATION_HYPER_MODE_COUNT,			/// �������������� ������ �ð�
};

enum BUFF_USE_COUNT_TYPE		/// ������ ������ ������ ����ϴ� Ÿ�� ����
{
	BUCT_NOT_USE = 0,				/// ������ ����� ������ ������ ������� ����
	BUCT_MY_BUFF_COUNT,				/// �ڽſ��� �ɸ� ���� ������ ������ ���
	BUCT_MY_DEBUFF_COUNT,			/// �ڽſ��� �ɸ� ����� ������ ������ ���
	BUCT_ENEMY_BUFF_COUNT,			/// ������ �ɸ� ���� ������ ������ ���
	BUCT_ENEMY_DEBUFF_COUNT,		/// ������ �ɸ� ����� ������ ������ ���
};

enum BUFF_FINALIZER_TYPE
{
	BFT_NONE = 0,
	BFT_TIME,			/// �ð��� ���� ����
	BFT_HIT_COUNT,		/// ���� Ÿ��Ƚ���� �Ǹ� ����
	BFT_HITTED_COUNT,	/// ���� �ǰ�Ƚ���� �Ǹ� ����
	BFT_STATE_CHANGE,	/// ������Ʈ�� ����Ǵ� ��� ����
	BFT_DAMAGE_VALUE,	/// �Էµ� ��ġ�� ���������� �Ѿ�� ����
	BFT_PASS_HP,		/// HP�� ���� ��ġ�� ����ϸ� ����
	BFT_PASS_MP,		/// MP�� ���� ��ġ�� ����ϸ� ����
	BFT_IMMEDIATE,		/// ���� ��� ����(�����δ� 1���� FRAME_MOVE ���� �� ����)
	BFT_DUNGEON_STAGE_CHANGE,	/// �������������� ����Ǹ� ����
	BFT_LIVE_CREATOR,	/// ���� �����ڰ� ����ϰų� �������� ������ ����
	BFT_FINISH_HYPER,	/// ������ ������� ��
	BFT_RIDING_ON_OR_NOT,	/// ���̵� ���� Ÿ�ų� ������ ��
	BFT_X2STATE_CHANGE, /// ���� ������Ʈ ����Ǵ� ��� ����

	//BFT_BUFF,			/// �Էµ� ������ ������� �ɸ��� �Ǹ� ����
};

enum BUFF_CHANGE_TYPE
{
	BCT_SWAP_VALUE	= 0,	/// Ư�� ��ġ�� ġȯ
	BCT_FIX_VALUE,			/// ���� ��ġ
	BCT_RELATION_VALUE,		/// ���� ��ġ
	BCT_PERCENT,			/// % ���
	BCT_END,				/// enum�� ��, enum�� ����
};

enum BUFF_RELATION_TYPE		/// ���� Ÿ��
{
	BRT_MAX_HP,					/// �ִ�HP ����
	BRT_PHYSIC_ATTACK,			/// �������ݷ¿���
	BRT_MAGIC_ATTACK,			/// �������ݷ¿���
	BRT_MIX_ATTACK,				/// ��հ��ݷ¿���
	BRT_PHYSIC_DEFENCE,			/// �������¿���
	BRT_MAGIC_DEFENCE,			/// �������¿���
	BRT_MIX_DEFENCE,			/// ��չ��¿���
	BRT_END,					/// enum�� ��, enum�� ����
};

enum BUFF_DISPLAYER_TYPE
{
	BDT_NONE = 0,
	BDT_RENDER_PARAM,					/// CKTDGXRenderer::RenderParam ����
	BDT_RENDER_PARAM_BY_UNIT_TYPE,		/// ���� Ÿ�Ժ� CKTDGXRenderer::RenderParam ����(������ UT_NONE ���)
	BDT_EFFECT_SET,						/// ����Ʈ�� ���
	BDT_UNIT_SLASH_TRACE,				/// UnitSlashTrace ���
	BDT_EFFECT_SET_MELEE_IMPACT_POINT,	/// ���������� Ÿ������ ����Ʈ�� ���	( ����Ʈ �� �̸�, ����� ����, ���� ���� )
	BDT_EFFECT_SET_HITTED,				/// Ÿ�� �������� ����Ʈ�� �����ֱ�		( ����Ʈ �� �̸�, ����� ����, ���� ���� )
	BDT_WEAPON_PARTICLE,				/// ���⿡ ���̴� ��ƼŬ
	BDT_AFTER_IMAGE,					/// AfterImage ����
	BDT_SOUND,							/// ���� ���
//#ifdef ADD_DISPLAYER_WEAPON_AFTER_IMAGE
	BDT_WEAPON_AFTER_IMAGE,				/// ���� AfterImage ����
//#endif ADD_DISPLAYER_WEAPON_AFTER_IMAGE
//#ifdef SERV_NEW_DEFENCE_DUNGEON
	BDT_EFFECT_SET_CREATE_GAP,			/// ���� ���ݸ��� ����Ʈ�� �߻�			( ����Ʈ �� �̸�, ���� ���� )
	BDT_EFFECT_SET_NORMAL_ATTACK,		/// �Ϲ� ����  Ÿ�ݽ� ����Ʈ�� ����		( ����Ʈ �� �̸�, ����� ����, ���� ���� )
//#endif SERV_NEW_DEFENCE_DUNGEON
//#ifdef HAMEL_SECRET_DUNGEON // ����ȯ
	BDT_EFFECT_SET_ONLY_MY_CLASS		/// �� ������ Ŭ������ ���� �ٸ� ����Ʈ���� ����
//#endif // HAMEL_SECRET_DUNGEON
};

// #ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
/// ���� �Ӽ� ���� ��ȭ�� Ÿ�� �߰�
enum BUFF_ENCHANT_ATTRIBUTE_TYPE
{
	BEAT_ALL = 0,						/// ��� �Ӽ� Ȯ�� ��ȭ
	BEAT_BLAZE,							/// �� �Ӽ� Ȯ�� ��ȭ
	BEAT_FROZEN,						/// �� �Ӽ� Ȯ�� ��ȭ
	BEAT_POSION,						/// �� �Ӽ� Ȯ�� ��ȭ	
	BEAT_PIERCING,						/// �� �Ӽ� Ȯ�� ��ȭ
	BEAT_SHOCK,							/// �� �Ӽ� Ȯ�� ��ȭ
	BEAT_SNATCH,						/// �� �Ӽ� Ȯ�� ��ȭ
};
// #endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
enum BUFF_TEMPLET_ID
{
	BTI_NONE									= 0,
	BTI_SI_SA_EMK_SWORD_FIRE					= 3,	// �ҵ� ���̾�
	BTI_SI_SA_EMK_PHOENIX_TALON					= 4,	// �Ǵн� �ڷ�
	BTI_HYPER_MODE								= 5,	// ����
	BTI_SI_P_ETK_BRUTAL_SLAYER					= 19,	// �ı��� �˻�
	BTI_DEBUFF_PRESS							= 25,	// ����
	BTI_SI_SA_ETK_PHANTOM_SWORD					= 29,	// ���� �ҵ�
	BTI_BLOODY_WEAPON							= 30,	// ���� �׼�
	BTI_DEBUFF_WOUND							= 31,	// ��ó(���ٴ�)
	BTI_SI_SA_ABM_MAGICAL_MAKEUP				= 32,	// ������ ����ũ ��
	BTI_BUFF_SI_A_AHM_MEDITATION				= 35,	// �ŵ����̼�
	BTI_DEBUFF_FIRE								= 37,	// ȭ��
	BTI_DEBUFF_BLIND							= 38,	// �þ� ����(���߷� ����, ���� ���� ����)
	BTI_DEBUFF_POISON							= 39,	// �ߵ�
	BTI_DEBUFF_AGING							= 40,	// ��ȭ
	BTI_DEBUFF_CURSE							= 41,	// ����
	BTI_SI_SA_CSG_SHARPSHOOTER_SYNDROME			= 45,   // �������� �ŵ��
	BTI_DEBUFF_STIGMA							= 46,	// ������ ���
	BTI_DEBUFF_WEAPON_BREAK						= 48,	// ���� �ı�
	BTI_BUFF_AMPLIFICATION_PLACE				= 50,	// ������
	BTI_BUFF_SPECTRUM_PLACE						= 51,	// �б���
	BTI_DEBUFF_STUN								= 52,	// ����
	BTI_DEBUFF_PANIC							= 53,	// ��Ȳ (�ǵ���Ͽ�)
	BTI_BUFF_LAND_DIMOLISHER					= 54,	// ���� �������
	BTI_BUFF_UNFIXED_CLIP						= 56,	// ���� Ŭ�� ����
	BTI_DEBUFF_MOVE_JUMP_SLOWDOWN				= 58,	// �̵� ��ȭ (���׼� ����)
	BTI_DEBUFF_LEG_SPASM						= 59,	// �ٸ� �λ�
	BTI_DEBUFF_COLD								= 60,	// ����
	BTI_DEBUFF_CONFUSION						= 61,	// ȥ��
	BTI_DEBUFF_MARK_OF_COMMANDER				= 62,	// ���ְ��� ǥ��
	BTI_DEBUFF_SIDE_EFFECT_MEDICINE				= 75,	// ��� ( ���� ���ۿ� )
	BTI_DEBUFF_SIDE_EFFECT_CREST				= 76,	// ����� ���� ( ���� ���ۿ� )
	BTI_DEBUFF_ENTANGLE							= 77,	// ���ʱ�
	BTI_DEBUFF_ARMOR_BREAK						= 78,	// �� �ı�
	BTI_DEBUFF_WATER_HOLD						= 79,	// ���� ��
	BTI_DEBUFF_FROZEN							= 80,	// ����
	BTI_DEBUFF_MAGIC_DEFFENCE_DROP				= 81,	// ������ �罽
	BTI_DEBUFF_BIND								= 82,	// ���� ����
	BTI_DEBUFF_DEATH_SENTENCE					= 84,	// ������ ����
	BTI_DEBUFF_MOTION_SLOWDOWN					= 85,	// ���� ��ȭ(�ο�ű ����)
	BTI_DEBUFF_BLIND_SMOKE						= 87,	// ���� (ȸ�� ����)
	BTI_DEBUFF_ANGER_OF_DRYAD					= 88,	// ����̾���� �г�
	BTI_CALM_REST								= 89,	// ����� �޽�
	BTI_DEBUFF_DEADLY_POISON					= 96,	// �͵� (�ϴ��� ��ũ Ŭ���� �޸𿡸� ����)
	BTI_DEBUFF_BLEEDING							= 99,	// ����
	BTI_DEBUFF_FLASH_BANG						= 100,	// ����ź (�Ϸ�Ʈ�� ���� �÷���)
	BTI_BUFF_INDUCTION_PLACE					= 105,	// ������
	BTI_BUFF_SI_SA_EBS_ENERGETIC_HEART			= 106,	// �ֳ���ƽ ��Ʈ
	BTI_DEBUF_SI_A_AHM_MEDITATION				= 107,	// �ŵ����̼� �����
	BTI_BUFF_SI_SA_CTT_TACTICAL_FIELD			= 108,	// ��Ƽ�� �ʵ�
	BTI_BUFF_THANKS_OF_RESIDENTS				= 123,	// �ֹ��� ����
	BTI_SI_SA_EEG_ATOMIC_SHIELD					= 124,	// ����� �ǵ�
//#ifdef BTI_BUFF_PREMIUM_PC_ROOM
	BTI_BUFF_PREMIUM_PC_ROOM					= 138,	//PC �� ����
//#endif BTI_BUFF_PREMIUM_PC_ROOM
	BTI_BUFF_PET_AURA_SKILL						= 139,	// �� �ݷ�
//#ifdef ADD_DISPLAYER_WEAPON_AFTER_IMAGE
	BTI_BUFF_HENIR_FIRE							= 147,	// ��ϸ� ���� - ���� ��
	BTI_BUFF_HENIR_WATER						= 148,	// ��ϸ� ���� - ���� ��
	BTI_BUFF_HENIR_NATURE						= 149,	// ��ϸ� ���� - ���� ��
	BTI_BUFF_HENIR_WIND							= 150,	// ��ϸ� ���� - �ٶ��� ��
	BTI_BUFF_HENIR_LIGHT						= 151,	// ��ϸ� ���� - ���� ��
//#endif ADD_DISPLAYER_WEAPON_AFTER_IMAGE
//#ifdef ARA_CHARACTER_BASE
	BTI_DEBUFF_SHADOW_JAIL						= 152,	// �׸��� ���� �����
	BTI_DEBUFF_SHADOW_KNOT						= 153,	// �׸��� �ŵ� �����
	BTI_EMPTY_EXP_BUFF							= 154,  // Ư�� ĳ���Ϳ� ���� ��Ƽ�ϸ� ����ġ ����
//#endif
	BTI_DEBUFF_SHOCK							= 156,	// ����� ��ũ
	BTI_ARA_FULL_HYPER_MODE						= 157,	// �ƶ� 3����
	BTI_DEBUFF_SUPPRESSION						= 161,	// ����
	BTI_BUFF_SWORD_ENCHANT_ICE					= 163,	// �ҵ� ��þƮ ���̽�
	BTI_BUFF_SWORD_ENCHANT_FIRE					= 164,	// �ҵ� ��þƮ ���̾�
	BTI_BUFF_SWORD_ENCHANT_POISON				= 165,	// �ҵ� ��þƮ ������
	BTI_DEBUFF_EVENT_BIG_HEAD					= 179,	// ���� ��ī���� ���б� �̺�Ʈ - ���� ��� ����
//#ifdef SERV_NEW_DEFENCE_DUNGEON
	BTI_BUFF_ELIXIR_BIG_HAND_POTION				= 198,	// ���� : �Ŵ�� ����
	BTI_BUFF_2013_DEFENSE_BUFF					= 202,	// 2013 ����� �� ������ ��¥ ����
	BTI_BUFF_2013_DEFENSE_DEBUFF				= 203,	// 2013 ����� �� ���� �����
	BTI_BUFF_2013_DEFENSE_BUFF_EVENT			= 204,	// �̺�Ʈ�� 2013 ����� �� ������ ����
//#endif SERV_NEW_DEFENCE_DUNGEON

//#ifdef SERV_NEW_UNIT_TRADE_LIMIT	// �ؿ� ����! �������� ����
	BTI_BUFF_TRADE_BLOCK						= 213,	// �ؿ� ���� �ű� ĳ���� �ŷ� �� ����
//#endif // SERV_NEW_UNIT_TRADE_LIMIT

#ifdef RIDING_SYSTEM
	BTI_RIDING_SCORPION_ENERGY_SWORD			= 214,	// �����ǿ� Ż �� ����� ����
#endif RIDING_SYSTEM

	BTI_DEBUFF_PANDEMONIUM_FEAR					= 223,	// �ǵ���Ͽ� - �Ǿ�
	BTI_BUFF_SI_SA_CTT_TACTICAL_FIELD_RAID		= 224,  // ��Ƽ�� �ʵ�

	BTI_BUFF_CHIVALRY_ATTACK					= 229,	// �����ý� �нú� ��ų ����� ���� - ����, ���� ���ݷ�
	BTI_BUFF_CHIVALRY_DEFENCE					= 230,	// �����ý� �нú� ��ų ����� ���� - ����, ���� ����, ����, �Ӽ� ����

	BTI_BUFF_INDURANCE_POWER					= 238,	// �����ý� �ر�-��(˭)
	BTI_BUFF_VICTORIOUS_SWORD					= 239,	// �����ý� �¸��� ��

	BTI_BUFF_GALE_WAY_OF_SWORD					= 240,	// �����ý� ���� �� ��ǳ
	BTI_BUFF_ANNIHILATION_WAY_OF_SWORD			= 247,	// �����ý� ���� �� ���� ( �Ӽ� ���� Ȯ�� ���� )

	BTI_DEBUFF_NEVER_MOVE						= 257,	// �ƶ� ���� 1�� - ��ȥ
	BTI_DEBUFF_SELF_PROTECTION_FORTITUDE		= 258,	// ȸ������ ��� �Ұ� �ȳ� �����
	BTI_BUFF_WATER_SHIELD						= 259,	// �ϸ� ��� ����, ���� ��ȣ��
	BTI_HEALTH_POINT_IMMEDIATELY_CHANGE_ONCE	= 260,	// HP ��� ����, 1����

//#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
	BTI_DEBUFF_STIGMA_OF_FIRE					= 290,	// �����ý�(����¡ ��Ʈ) - �Ҳ��� ���� �����
	BTI_CHANGE_PHYSIC_DEFENCE_AND_PHYSIC_DAMAGE = 292,	// �����ý�(�׷��� ������) - ���� �����, ���� ���ݷ� ���� / ���� ���� ���� 
	BTI_DEBUFF_UNEXTINGUISHABLE_FIRE			= 293,	// �����ý�(����¡ ��Ʈ) - ������ �ʴ� �Ҳ� �����
	BTI_BUFF_FIRE_BLOSSOMS						= 294,	// �����ý�(����¡ ��Ʈ) - �Ҳ� ��ȭ
	BTI_BUFF_FLAME_SWORD						= 298,
	BTI_DEBUFF_HIGH_FEVER						= 299,	// �ҼӼ� ������ ���ҽ�Ű�� �����.
//#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

//#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
	BTI_BUFF_2013_DEFENCE_ENTER_100_PERCENT_BUFF = 300,
//#endif //SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
	BTI_DEBUFF_RAINCOAT_CAT_STEB1_ATTACK_A	= 301,		// ������� ����� ����
	BTI_DEBUFF_RAINCOAT_CAT_STEB2_ATTACK_A	= 302,		// ������� ����� ����
	BTI_DEBUFF_RAINCOAT_CAT_STEB3_ATTACK_A	= 303,		// ������� ����� ����
	// �ҷ��� ������ ���� ���� ����
//#ifdef INT_HALLOWEEN_EVENT_BUF
	BTI_BUFF_BIG_HEAD_SOUP				= 307,
	BTI_BUFF_HP_UP_SOUP					= 308,
	BTI_BUFF_MP_UP_SOUP					= 309,
//#endif INT_HALLOWEEN_EVENT_BUF
//#ifdef BALANCE_PATCH_20131107							// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	BTI_BUFF_SA_ESK_WINDMILL					= 318,	// ���ҵ�(�ε� ����Ʈ) - 2013�� �Ϲݱ� �뷱�� �۾�, ���� ���� ���� ���� ���� �߰�
//#endif // BALANCE_PATCH_20131107						// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	BTI_FORMATION_MODE							= 320,	// �ֵ� ���� ���
	BTI_BUFF_SGM_VICTORIOUS_SWORD				= 326,	// �����ý�(�׷��帶����) - �¸��� ��. ����Ʈ �ٲ�.
	BTI_BUFF_LIBRARY_OF_LIMITLESS_MANA_RECOVERY = 327,	// �ֵ� - ������ ������ '����ȸ���ӵ��� X�ʰ� Y�� ����' �ϴ� ����
	BTI_NASOD_ARMOR								= 331,	// �ֵ� - ���ҵ� �Ƹ�

//#ifdef ADD_RENA_SYSTEM //��â��
	BTI_BUFF_NATURAL_FORCE						= 333,	// ���� �߰� �ý��� - NF ����
//#endif //ADD_RENA_SYSTEM

//#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
	BTI_BUFF_POWER_EXCHANGER_HALF_UP			= 334,	// �ֵ� - ��� ��ȯ�� ���� ���ݷ�, ũ��Ƽ�� ��� ����
	BTI_BUFF_POWER_EXCHANGER_HALF_DOWN			= 335,	// �ֵ� - ��� ��ȯ�� ���� ���� �ӵ� ��� ����
	BTI_DEBUFF_PLASMA_LINK						= 336,	// �ֵ� - ������ ������ �̵���, �Ӽ� ���� ���� �����
	BTI_BUFF_QUICKSILVER_FRENZY					= 337,	// �ֵ� - ���ǹ� ������ ���ۼӵ�, �̵���, ������ ����
//#endif //SERV_ADD_LUNATIC_PSYKER

	BTI_BUFF_NAVER								= 339,	// ���̹�ä�θ�����, ����/�������ݷ� 3%, ����/������� 3%, MP �ִ�ġ 10% ����
//#ifdef SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC
	BTI_BUFF_THE_ESSENCE_OF_WEAK_HERETIC_POTION	= 1318,
	BTI_BUFF_THE_ESSENCE_OF_HERETIC_POTION		= 1319,
//#endif SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC
//#ifdef SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
	BTI_BUFF_RURIEL_MANA_ENERGIZE_POTION		= 1320,
//#endif SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
};

enum BUFF_FACTOR_ID
{
	BFI_NONE									= 0,
 	BFI_HYPER_MODE								= 1,
	BFI_DEBUFF_SIDE_EFFECT_MEDICINE				= 67,
	BFI_DEBUFF_SIDE_EFFECT_CREST				= 68,
	BFI_DEBUFF_BIND								= 73,
	BFI_DEBUFF_BIND_WITH_MEMO					= 74,
	BFI_DEBUFF_DEATH_SENTENCE					= 75,
	BFI_CALM_REST								= 78,
//#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��	
	BFI_DEBUFF_SI_P_EIS_MIND_OF_FIGHTER			= 80,
//#endif //UPGRADE_SKILL_SYSTEM_2013
	BFI_BUFF_OVER_HEAT							= 84,

#ifdef FIX_REFORM_ENTRY_POINT_7TH						// ������, ��ȯ�� ���� ��� �κ� Emblem Manager �� ����
	BFI_BUFF_RETURN_OF_HERO						= 114,		// ��ȯ�� ����
#endif // FIX_REFORM_ENTRY_POINT_7TH					// ������, ��ȯ�� ���� ��� �κ� Emblem Manager �� ����

	BFI_BUFF_PET_AURA_SKILL_LEVEL_1_DEFAULT		= 125,
	BFI_BUFF_PET_AURA_SKILL_LEVEL_2_DEFAULT		= 130,
	BFI_BUFF_PET_AURA_SKILL_LEVEL_3_DEFAULT		= 135,	
	BFI_BUFF_MECHANIZATION_SPEED_UP				= 141,	// ���ȭ ��Ʈ ���ǵ� ����
//#ifdef ADD_DISPLAYER_WEAPON_AFTER_IMAGE
	BFI_BUFF_HENIR_FIRE							= 147,	// ��ϸ� ���� - ���� ��
	BFI_BUFF_HENIR_WATER						= 148,	// ��ϸ� ���� - ���� ��
	BFI_BUFF_HENIR_NATURE						= 149,	// ��ϸ� ���� - ���� ��
	BFI_BUFF_HENIR_WIND							= 150,	// ��ϸ� ���� - �ٶ��� ��
	BFI_BUFF_HENIR_LIGHT						= 151,	// ��ϸ� ���� - ���� ��
//#endif ADD_DISPLAYER_WEAPON_AFTER_IMAGE

	BFI_EMPTY_EXP_BUFF							= 154,	// ����ġ ���� ����
	BFI_ARA_FULL_HYPER_MODE						= 156,	// �ƶ� 3���� ���� - ȣ����
	BFI_BUFF_BREAKING_MEMO						= 181,	// �ƶ� �޸� : �� �帣����( ��ȭ���� ) - ȸ���� ����

#ifdef RIDING_SYSTEM
	BFI_BUFF_SUPER_AMOR_FOR_RIDING				= 199,	// Ż ���� Ÿ�� ���� �� Ư���� ��Ȳ(DOWN, UP, FLY ����)�� ���� �� Super Armor �ߵ�
#endif //RIDING_SYSTEM
	BFI_BUFF_TRADE_BLOCK						= 205,	// SERV_NEW_UNIT_TRADE_LIMIT �ؿ� ���� �ű� ĳ���� �ŷ� ��� ����
#ifdef RIDING_SYSTEM
	BFI_RIDING_SCORPION_ENERGY_SWORD			= 206,	// �����ǿ� Ż �� ����� ����
#endif //RIDING_SYSTEM
// #ifdef UPGRADE_SKILL_SYSTEM_2013 // Raven ��ų ����, ������
	BFI_BUFF_OVER_HEAT_ENHANCE					= 217,	// ������� �۵�! �нú� ����, 
// #endif // UPGRADE_SKILL_SYSTEM_2013 // Raven ��ų ����, ������
	BFI_BUFF_VICTORIOUS_SWORD					= 233,	// �����ý� �¸��� �� ���̹�����Ʈ ��.
	BFI_BUFF_GALE_WAY_OF_SWORD					= 237,	// �����ý� ���� �� ��ǳ ( �̼�, ���� ���� )
	BFI_BUFF_GALE_MP_GAIN_INCREASE				= 245,	// �����ý� ���� �� ��ǳ ( MP ȸ���� ���� ) 
	BFI_BUFF_ANNIHILATION_WAY_OF_SWORD			= 246,	// �����ý� ���� �� ���� ( �Ӽ� ���� Ȯ�� ���� )
	BFI_BUFF_ANNIHILATION_WAY_OF_SWORD_PVP		= 247,	// �����ý� ���� �� ����_����( �Ӽ� ���� Ȯ�� ���� )
	BFI_BUFF_ANNIHILATION_SUPER_ARMOR			= 248,	// �����ý� ���� �� ���� ������ ���� ���۾Ƹ�

	BFI_BUFF_SI_A_ALD_REFLECTION				= 259,	// �ƶ� ��ź��
	BFI_DEBUFF_SELF_PROTECTION_FORTITUDE		= 264,	// ȸ������ ��� �Ұ� �ȳ� �����
#ifdef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
	BFI_DEBUFF_TELEPORT_CONSUME_MP				= 290,	// ���̻� �ڷ���Ʈ �����
#endif //FIX_SKILL_BALANCE_AISHA_LENA
//#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
	BFI_DEBUFF_STIGMA_OF_FIRE					= 291,	// �����ý�(����¡ ��Ʈ) - �Ҳ��� ���� �����
	BFI_DEBUFF_JUDGEMENT_FIRE					= 292,	// �����ý�(����¡ ��Ʈ) - �Ҳ��� ���� ���� �����
	BFI_BUFF_FIRE_BLOSSOMS						= 295,	// �����ý�(����¡ ��Ʈ) - �Ҳ� ��ȭ
	BFI_DEBUFF_HIGH_FEVER						= 300,	// �ҼӼ� ������ ���ҽ�Ű�� �����.
	BFI_DEBUFF_FLAME_SWORD						= 301,	// �����ý�(����¡ ��Ʈ) - ȫ���� �� �����(ȭ��ȿ��)
// #ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
	BFI_BUFF_2013_DEFENCE_ENTER_100_PERCENT_BUFF = 302,
// #endif //SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
	BFI_DEBUFF_JUDGEMENT_FIRE2					= 307,	// �����ý�(����¡ ��Ʈ) - �Ҳ��� ���� ���� �����2
//#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
	BFI_FORMATION_MODE							= 315,	// �ֵ� ���� ���
//#ifdef ADD_MEMO_1ST_CLASS //��â��
	BFI_BUFF_SI_A_ALD_REFLECTION_MEMO			= 334,	// �ƶ�(�Ҹ�) - ��ź��(�޸�)
	BFI_BUFF_SWORD_ENCHANT_CHARGE_MP			= 338,	// ���ҵ�(��������Ʈ) - �ҵ� ��æƮ (�޸�)
//#endif //ADD_MEMO_1ST_CLASS
	BFI_BUFF_SGM_VICTORIOUS_SWORD				= 345,	// �����ý� �¸��� �� �׷��� ������ ��.
	BFI_BUFF_LIBRARY_OF_LIMITLESS_MANA_RECOVERY	= 346,	// �ֵ� - ������ ������
	BFI_DEBUFF_MIND_BREAK						= 347,	// �ֵ� - ���� �ر�
	BFI_DEBUFF_EMP_SHOCK_NEAR					= 348,	// �ֵ� - EMP ��ũ ���� �ǰ�
	BFI_DEBUFF_EMP_SHOCK_FAR					= 349,	// �ֵ� - EMP ��ũ ���� �ǰ�
	BFI_NASOD_ARMOR 							= 353,	// �ֵ� - ���ҵ� �Ƹ�

//#ifdef ADD_RENA_SYSTEM //��â��
	BFI_BUFF_NATURAL_FORCE						= 355,	// ���� �߰� �ý��� - NF ����
//#endif //ADD_RENA_SYSTEM

//#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
	BFI_BUFF_POWER_EXCHANGER_HALF_UP			= 356,	// �ֵ� - ��� ��ȯ�� ���� ���ݷ�, ũ��Ƽ�� ��� ����
	BFI_BUFF_POWER_EXCHANGER_HALF_DOWN			= 357,	// �ֵ� - ��� ��ȯ�� ���� ���� �ӵ� ��� ����
	BFI_DEBUFF_PLASMA_LINK						= 358,	// �ֵ� - ������ ������ �̵���, �Ӽ� ���� ���� �����
	BFI_BUFF_QUICKSILVER_FRENZY					= 359,	// �ֵ� - ���ǹ� ������ ���ۼӵ�, �̵���, ������ ����
//#endif //SERV_ADD_LUNATIC_PSYKER

	BFI_BUFF_NAVER								= 361,	// ���̹�ä�θ�����, ����/�������ݷ� 3%, ����/������� 3%, MP �ִ�ġ 10% ����
// #ifdef SERV_BONUS_BUFF_SYSTEM �̳� ��ģ��ƾƾƾ�
//	BFI_BUFF_2YEARS_EVENT_CN_DAMAGE_UP				= 307,	
//	BFI_BUFF_2YEARS_EVENT_CN_MOVE_JUMP_SPEED_UP 	= 308,
//	BFI_BUFF_2YEARS_EVENT_CN_ACCURACY_AVOID_UP		= 309,
//	BFI_BUFF_2YEARS_EVENT_CN_CRI_SPEED_UP			= 310,
//	BFI_BUFF_2YEARS_EVENT_CN_DAMAGE_INC_DEC_UP		= 311,
//	BFI_BUFF_2YEARS_EVENT_CN_ALL_IN_ONE				= 312,
// #endif //SERV_BONUS_BUFF_SYSTEM	
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////

struct BUFF_FACTOR
{
public:
	BUFF_FACTOR() : m_uidGameUnit( 0 ), m_bIgnoreRegistForRate( true )
#ifdef ADD_LIVE_CREATOR_FINALIZER
		, m_bIsUserUID( true )
#endif ADD_LIVE_CREATOR_FINALIZER
	{
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		m_vecRate.clear();
#else //UPGRADE_SKILL_SYSTEM_2013
		m_fRate = 0.f;
#endif //UPGRADE_SKILL_SYSTEM_2013
	}
		
	BUFF_FACTOR( const BUFF_FACTOR& rhs_ ) : m_BuffIdentity( rhs_.m_BuffIdentity ),
		m_uidGameUnit( rhs_.m_uidGameUnit ), m_bIgnoreRegistForRate( rhs_.m_bIgnoreRegistForRate )
#ifdef ADD_LIVE_CREATOR_FINALIZER
		, m_bIsUserUID( rhs_.m_bIsUserUID )
#endif ADD_LIVE_CREATOR_FINALIZER
	{
		m_vecBuffBehaviorFactors = rhs_.m_vecBuffBehaviorFactors;
		m_vecBuffFinalizerFactors = rhs_.m_vecBuffFinalizerFactors;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		m_vecRate = rhs_.m_vecRate;
#else //UPGRADE_SKILL_SYSTEM_2013
		 m_fRate = rhs_.m_fRate;
#endif //UPGRADE_SKILL_SYSTEM_2013
	}

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	BUFF_FACTOR( const BUFF_FACTOR& rhs_, IN const UINT uiLevel_ ) : m_BuffIdentity( rhs_.m_BuffIdentity ),
		m_uidGameUnit( rhs_.m_uidGameUnit ), m_bIgnoreRegistForRate( rhs_.m_bIgnoreRegistForRate )
#ifdef ADD_LIVE_CREATOR_FINALIZER
		, m_bIsUserUID( rhs_.m_bIsUserUID )
#endif ADD_LIVE_CREATOR_FINALIZER
	{
		if ( uiLevel_ >= 1 )
		{
			m_vecRate.push_back( rhs_.m_vecRate[uiLevel_] );
			GetBehaviorFactorToLevel( rhs_.m_vecBuffBehaviorFactors, m_vecBuffBehaviorFactors, uiLevel_ );
			GetFinalizerFactorToLevel( rhs_.m_vecBuffFinalizerFactors, m_vecBuffFinalizerFactors, uiLevel_ );
		}
		else
		{
			m_vecRate.push_back( rhs_.m_vecRate[0] );
			GetBehaviorFactorToLevel( rhs_.m_vecBuffBehaviorFactors, m_vecBuffBehaviorFactors, 1 );
			GetFinalizerFactorToLevel( rhs_.m_vecBuffFinalizerFactors, m_vecBuffFinalizerFactors, 1 );
		}
	}

	void GetBehaviorFactorToLevel( IN const vector<KBuffBehaviorFactor>& vecInBehaviorFactorList ,
		OUT vector<KBuffBehaviorFactor>& vecOutBehaviorFactorList, IN const UINT uiLevel_ );

	void GetFinalizerFactorToLevel( IN const vector<KBuffFinalizerFactor>& vecInFinalizerFactorList ,
		OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, IN const UINT uiLevel_ );

	void PushFinalizerTimeToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, IN const KBuffFinalizerFactor& factor_,
		IN const UINT uiLevel_ );
	void PushFinalizerHitToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, IN const KBuffFinalizerFactor& factor_,
		IN const UINT uiLevel_ );
	void PushFinalizerHittedToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, IN const KBuffFinalizerFactor& factor_,
		IN const UINT uiLevel_ );
	void PushFinalizerDamageValueToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, IN const KBuffFinalizerFactor& factor_,
		IN const UINT uiLevel_ );
	void PushFinalizerHpToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, IN const KBuffFinalizerFactor& factor_,
		IN const UINT uiLevel_ );
	void PushFinalizerMpToLevel( OUT vector<KBuffFinalizerFactor>& vecOutFinalizerFactorList, IN const KBuffFinalizerFactor& factor_,
		IN const UINT uiLevel_ );
#endif //UPGRADE_SKILL_SYSTEM_2013

public:
	vector<KBuffBehaviorFactor>		m_vecBuffBehaviorFactors;
	vector<KBuffFinalizerFactor>	m_vecBuffFinalizerFactors;
	KBuffIdentity					m_BuffIdentity;				/// �̰��� KBuffIdentity�� �� BUFF_FACTOR�� �߻� ��Ű�� ������ ���������� ������ �� �ְ���(Templet���κ��� ������ ���ͼ� �ٸ� �����鿡�� �˷��ٶ� ���)
	UidType							m_uidGameUnit;
#ifdef ADD_LIVE_CREATOR_FINALIZER
	bool							m_bIsUserUID;				/// ���� ������ UID���� ����( ������ ����� �Ҹ� ��Ű�� ���̳ζ������� ���� ���� )
#endif ADD_LIVE_CREATOR_FINALIZER

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	vector<float>	m_vecRate;					/// �ش� ���ο� ���� ���� �Ǵ� ������� �ɸ� Ȯ��
#else //UPGRADE_SKILL_SYSTEM_2013
	float							m_fRate;					/// �ش� ���ο� ���� ���� �Ǵ� ������� �ɸ� Ȯ��
#endif //UPGRADE_SKILL_SYSTEM_2013
	bool							m_bIgnoreRegistForRate;		/// ���� Ȯ���� ���׿� ���ؼ� ����Ǵ��� ����(true �ΰ�� ������ ������ �ɸ� Ȯ���� ������)
	/// ��ø�� �ʿ��ҵ�...
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
class CX2GameUnit;
class CX2BuffFactor;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2BuffFactor> CX2BuffFactorPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2BuffFactor> CX2BuffFactorPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

/** @class : CX2BuffFactor
	@brief : ������ �߻���Ű�� ������ ������ �ִ� Ŭ����
	@date : 2012/7/16/
*/
class CX2BuffFactor
{
public:
	static CX2BuffFactorPtr CreateBuffFactorPtr() { return CX2BuffFactorPtr( new CX2BuffFactor ); }

	~CX2BuffFactor() { SAFE_DELETE( m_pDataBuffFactor ); }

	CX2BuffFactor( const CX2BuffFactor& rhs_ ) 
		: m_pDataBuffFactor( new BUFF_FACTOR( *(rhs_.m_pDataBuffFactor) ) ), m_DataBuffIdentity( rhs_.m_DataBuffIdentity )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{}

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	CX2BuffFactor( const CX2BuffFactor& rhs_, IN const UINT uiLevel_ ) 
		: m_pDataBuffFactor( new BUFF_FACTOR( *(rhs_.m_pDataBuffFactor), uiLevel_ ) ), m_DataBuffIdentity( rhs_.m_DataBuffIdentity )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{}
#endif //UPGRADE_SKILL_SYSTEM_2013

private:
	CX2BuffFactor() : m_pDataBuffFactor( new BUFF_FACTOR )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR    
    {}
	
public:
	CX2BuffFactorPtr	GetClonePtr() const { return CX2BuffFactorPtr( new CX2BuffFactor(*this) ); }
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	CX2BuffFactorPtr	GetClonePtr( IN const UINT uiLevel_ ) const { return CX2BuffFactorPtr( new CX2BuffFactor(*this, uiLevel_) ); }
#endif //UPGRADE_SKILL_SYSTEM_2013

	bool GetBehaviorFactor( const BUFF_BEHAVIOR_TYPE eType_, OUT const KBuffBehaviorFactor** ppFactor_ ) const;
	bool GetFinalizerFactor( const BUFF_FINALIZER_TYPE eType_, OUT const KBuffFinalizerFactor** ppFactor_ ) const;

	UidType GetUidGameUnit() const { return m_pDataBuffFactor->m_uidGameUnit; }
	void SetUidGameUnit(UidType val) { m_pDataBuffFactor->m_uidGameUnit = val; }

#ifdef ADD_LIVE_CREATOR_FINALIZER
	bool GetIsUserUID() const {  return m_pDataBuffFactor->m_bIsUserUID; }
	void SetIsUserUID( bool val ) { m_pDataBuffFactor->m_bIsUserUID = val; }
#endif ADD_LIVE_CREATOR_FINALIZER

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	float GetRate( IN const UINT iLevel_ ) const
	{
		if ( true == m_pDataBuffFactor->m_vecRate.empty() )
			DISPLAY_ERROR( L"Rate is Empty!" );

		if ( m_pDataBuffFactor->m_vecRate.size() >= iLevel_ )
			return m_pDataBuffFactor->m_vecRate.at( iLevel_ - 1 );
		else
			return m_pDataBuffFactor->m_vecRate.at( 0 );
	}

	void SetRate( float val, IN const UINT iLevel_ )
	{
		if ( true == m_pDataBuffFactor->m_vecRate.empty() )
			DISPLAY_ERROR( L"Rate is Empty!" );

		if ( m_pDataBuffFactor->m_vecRate.size() >= iLevel_ )
			m_pDataBuffFactor->m_vecRate.at( iLevel_ - 1 ) = val;
		else
			m_pDataBuffFactor->m_vecRate.at( 0 ) = val;
	}

	void ParsingTableForWhile( IN KLuaManager& luaManager_, IN const char* pszTableName_, OUT vector<float>& vecValues_, IN bool bUpdateCount_ = true );
	void ParsingTableMinMaxForWhile( IN KLuaManager& luaManager_, IN const char* pszTableName_, OUT vector<float>& vecValues_, IN bool bUpdateCount_ = true );
#else //UPGRADE_SKILL_SYSTEM_2013
	float GetRate() const { return m_pDataBuffFactor->m_fRate; }
	void SetRate(float val) { m_pDataBuffFactor->m_fRate = val; }
#endif //UPGRADE_SKILL_SYSTEM_2013

	bool GetIgnoreRegistForRate() const { return m_pDataBuffFactor->m_bIgnoreRegistForRate; }
	void SetIgnoreRegistForRate(bool val) { m_pDataBuffFactor->m_bIgnoreRegistForRate = val; }

	BUFF_TEMPLET_ID GetBuffTempletID() const { return static_cast<BUFF_TEMPLET_ID>( m_pDataBuffFactor->m_BuffIdentity.m_eBuffTempletID ); }
	void SetBuffTempletID(BUFF_TEMPLET_ID val) { m_pDataBuffFactor->m_BuffIdentity.m_eBuffTempletID = val; }

	void SetBuffIdentity( const KBuffIdentity& BuffIdentity_ ) { m_DataBuffIdentity = BuffIdentity_; }
	const KBuffIdentity& GetBuffIdentity() const { return m_DataBuffIdentity; }

	/// ������ġ�� ��� �� �Լ��� �̿��Ͽ� ���� ������
	void UpdateValueByRelationType( const BUFF_RELATION_TYPE eRelationType_, OUT float& fValue_, CX2GameUnit* pGameUnit_ );

	////////////////////////////////////////////////////////////////////////////////////////////////
	// PushFactor
	////////////////////////////////////////////////////////////////////////////////////////////////
	void PushBehaviorFactor( const KBuffBehaviorFactor& factor_ ) { m_pDataBuffFactor->m_vecBuffBehaviorFactors.push_back( factor_ ); }
	void PushFinalizerFactor( const KBuffFinalizerFactor& factor_ ) { m_pDataBuffFactor->m_vecBuffFinalizerFactors.push_back( factor_ ); }

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Parsing
	////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	bool ParsingBuffFactor( KLuaManager& luaManager_, bool bIsSkill = true );
	bool ParsingBehavior( KLuaManager& luaManager_, bool bIsSkill = true );
	bool ParsingCombinationBehavior( KLuaManager& luaManager_, bool bIsSkill = true );
	bool ParsingFinalizer( KLuaManager& luaManager_, bool bIsSkill = true );
	bool ParsingCombinationFinalizer( KLuaManager& luaManager_, bool bIsSkill = true );
#else //UPGRADE_SKILL_SYSTEM_2013
	bool ParsingBuffFactor( KLuaManager& luaManager_ );
	bool ParsingBehavior( KLuaManager& luaManager_ );
	bool ParsingCombinationBehavior( KLuaManager& luaManager_ );
	bool ParsingFinalizer( KLuaManager& luaManager_ );
	bool ParsingCombinationFinalizer( KLuaManager& luaManager_ );
#endif //UPGRADE_SKILL_SYSTEM_2013

	////////////////////////////////////////////////////////////////////////////////////////////////
	/// ApplyExternalFactor
	////////////////////////////////////////////////////////////////////////////////////////////////
	//void ApplyExternalFactorToBehavior( const BUFF_BEHAVIOR_TYPE eType_, const float fValue_ );
	void ApplyExternalFactorToFinalizer( const BUFF_FINALIZER_TYPE eType_, const float fValue_ );

	////////////////////////////////////////////////////////////////////////////////////////////////
	// SetGameUnit
	////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ADD_LIVE_CREATOR_FINALIZER
	void SetGameUnitBuffFactor( CX2GameUnit* pGameUnit_, UidType iCreatorUID = -1, bool bIsUserUID = true  );
#else  ADD_LIVE_CREATOR_FINALIZER
	void SetGameUnitBuffFactor( CX2GameUnit* pGameUnit_ );
#endif ADD_LIVE_CREATOR_FINALIZER
	void SetGameUnitBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );
	void SetGameUnitFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );
	

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Behavior
	////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	bool ParsingChangeNowHpPerSecondBehavior( KLuaManager& luaManager_, const char* pszTableNameUTF8_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );
	void SetGameUnitChangeNowHpPerSecondBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );

	bool ParsingChangeNowMpPerSecondBehavior( KLuaManager& luaManager_, const char* pszTableNameUTF8_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );

	bool ParsingChangeStatBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );
	void SetGameUnitChangeStatBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );

	bool ParsingCreateBuffFactorBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	bool ParsingAbsorbEffectAttackBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true  );

	bool ParsingChargeMpHitBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );
	bool ParsingChargeMpHittedBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );

	bool ParsingGameUnitChangeHyperModeBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	void SetGameUnitChangeHyperModeBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );

	bool ParsingEmptyBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	bool ParsingCustomBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingEffectSetWithDamageBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	void SetGameUnitEffectSetWithDamageBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );

	bool ParsingChangeAttackByTypeBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );

	bool ParsingChangeEnchantAttackRateBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );

	bool ParsingDoubleAttackByBuffBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );

	bool ParsingAddSkillLevelByBuffBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingResetSkillCoolTimeByBuffBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
	bool ParsingReflectMagicBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
	bool ParsingChangConsumeMpRateBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_, bool bIsSkill = true );
#endif //FIX_SKILL_BALANCE_AISHA_LENA

#else //UPGRADE_SKILL_SYSTEM_2013
	bool ParsingChangeNowHpPerSecondBehavior( KLuaManager& luaManager_, const char* pszTableNameUTF8_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	void SetGameUnitChangeNowHpPerSecondBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );

	bool ParsingChangeNowMpPerSecondBehavior( KLuaManager& luaManager_, const char* pszTableNameUTF8_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingChangeStatBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	void SetGameUnitChangeStatBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );

	bool ParsingCreateBuffFactorBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	bool ParsingAbsorbEffectAttackBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingChargeMpHitBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	bool ParsingChargeMpHittedBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingGameUnitChangeHyperModeBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	void SetGameUnitChangeHyperModeBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );

	bool ParsingEmptyBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	bool ParsingCustomBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingEffectSetWithDamageBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
	void SetGameUnitEffectSetWithDamageBehavior( KBuffBehaviorFactor& factor_, CX2GameUnit* pGameUnit_ );

	bool ParsingChangeAttackByTypeBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingChangeEnchantAttackRateBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingDoubleAttackByBuffBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

	bool ParsingAddSkillLevelByBuffBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
	bool ParsingResetSkillCoolTimeByBuffBehavior( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_BEHAVIOR_TYPE eBehaviorType_ );
#endif // UPGRADE_SKILL_SYSTEM_2013
#endif //UPGRADE_SKILL_SYSTEM_2013

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Finalizer
	////////////////////////////////////////////////////////////////////////////////////////////////
	/// �ð��� ���� ���� (����, ����)
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	bool ParsingTimeFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_, bool bIsSkill = true );
	void SetGameUnitTimeFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );
	void ApplyExternalFactorToTimeFinalizer( KBuffFinalizerFactor& factor_, const float fValue_ );

	/// Ÿ�� ī��Ʈ�� ���� ����(����, ����� ���� ��� ����)
	bool ParsingHitCountFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eBehaviorType_, bool bIsSkill = true );
	void SetGameUnitHitCountFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );

	/// �ǰ� ī��Ʈ�� ���� ����(����, ����� ���� ��� ����)
	bool ParsingHittedCountFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_, bool bIsSkill = true );
	void SetGameUnitHittedCountFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );
	
	/// ���� �������� ������ ����
	bool ParsingDamageValueFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_, bool bIsSkill = true );
	void SetGameUnitDamageValueFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );

	/// ���� HP�� ���� �Ǵ� ���� ���� �ϸ� ����
	bool ParsingPassHpFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_, bool bIsSkill = true );

	/// ���� MP�� ���� �Ǵ� ���� ���� �ϸ� ���� (HP�� ���� ������ ��������� ���� ���� �� ������ �ΰ��� ���� ����� ���� �ֱ� ����)
	bool ParsingPassMpFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_, bool bIsSkill = true );

	bool ParsingEmptyFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ );
#else //UPGRADE_SKILL_SYSTEM_2013
	bool ParsingTimeFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ );
	void SetGameUnitTimeFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );
	void ApplyExternalFactorToTimeFinalizer( KBuffFinalizerFactor& factor_, const float fValue_ );

	/// Ÿ�� ī��Ʈ�� ���� ����(����, ����� ���� ��� ����)
	bool ParsingHitCountFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eBehaviorType_ );
	void SetGameUnitHitCountFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );

	/// �ǰ� ī��Ʈ�� ���� ����(����, ����� ���� ��� ����)
	bool ParsingHittedCountFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ );
	void SetGameUnitHittedCountFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );

	/// ������Ʈ�� ����Ǹ� ����
	bool ParsingStateChangeFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ );

	/// ���� �������� ������ ����
	bool ParsingDamageValueFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ );
	void SetGameUnitDamageValueFinalizer( KBuffFinalizerFactor& factor_, CX2GameUnit* pGameUnit_ );

	/// ���� HP�� ���� �Ǵ� ���� ���� �ϸ� ����
	bool ParsingPassHpFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_);

	/// ���� MP�� ���� �Ǵ� ���� ���� �ϸ� ���� (HP�� ���� ������ ��������� ���� ���� �� ������ �ΰ��� ���� ����� ���� �ֱ� ����)
	bool ParsingPassMpFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ );

	bool ParsingEmptyFinalizer( KLuaManager& luaManager_, const char* pszTableName_, const BUFF_FINALIZER_TYPE eFinalizerType_ );
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

private:

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    CX2BuffFactor& operator = ( const CX2BuffFactor& );
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned            m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	BUFF_FACTOR*		m_pDataBuffFactor;		/// �����ͷ� �� ���ΰ�...?
	KBuffIdentity		m_DataBuffIdentity;		/// � ������ ���� �߻��� BuffFactor������ �� �� �ְ� ��

};


IMPLEMENT_INTRUSIVE_PTR( CX2BuffFactor );