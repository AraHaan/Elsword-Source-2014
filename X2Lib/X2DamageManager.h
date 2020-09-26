#pragma once

class CX2GameUnit;
typedef KObserverPtr<CX2GameUnit>   CX2GameUnitoPtr;

class CX2GUUser;

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

struct  CX2DamageEffect_CEffect_TAG {};
typedef KHandleType<int,CX2DamageEffect_CEffect_TAG>     CX2DamageEffect_CEffectHandle;
#define INVALID_DAMAGE_EFFECT_HANDLE    (CX2DamageEffect_CEffectHandle::invalid_handle())

#else   X2OPTIMIZE_HANDLE_VALIDITY_CHECK
typedef int CX2DamageEffect_CEffectHandle;
static const CX2DamageEffect_CEffectHandle INVALID_DAMAGE_EFFECT_HANDLE = CX2DamageEffect_CEffectHandle(-1);

#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK

#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
class CX2DamageEffectBasic;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE


class CX2DamageManager
{
	public:
#ifdef UPGRADE_RAVEN
		/// Ÿ�ݽ� Ʈ���� ID
		enum DAMAGE_TRIGGER_TYPE
		{
			DTT_NONE	= 0,

			DTT_RAVEN_RAGE_ATTACK,
#ifdef EVE_ELECTRA
			DTT_EVE_THOUSAND_STAR,
#endif EVE_ELECTRA
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
			DTT_MARK_OF_COMMANDER,
#endif SERV_ADD_CHUNG_SHELLING_GUARDIAN

#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
			// ���� �Ĺ� ������, ��ī�ο� �������� �Ϻ� ��Ƽ�� ��ų MP ȸ���� ������ �����ִ� Trigger Type
			DTT_SHARP_KICK_RELATE_ACTIVE_SKILL_INCREASE_MP_RATE,					
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����

#ifdef ADD_EVE_SYSTEM_2014		// ������, 2014 - �̺� �߰� �ý���, ���ҵ� �ھ�
			// �⵿ �ھ� ���ݿ� �������� �ʴ� AT_SPECIAL ������ Trigger Type ( �̺길 �ش� )
			DTT_EVE_MANEUVER_ATTACK,
#endif // ADD_EVE_SYSTEM_2014	// ������, 2014 - �̺� �߰� �ý���, ���ҵ� �ھ�
			DTT_END,
		};
#endif

		/// DAMAGE ����
		enum DAMAGE_TYPE
		{
			DT_PHYSIC = 0,			/// ���� Ÿ��
			DT_MAGIC,				/// ���� Ÿ��
			DT_MIX,					/// ����,���� ����

			DT_WEAPON_PHYSIC,		/// ������ �������ݷ�(��������)
			DT_WEAPON_MAGIC,		/// ������ �������ݷ�(��������)
			DT_WEAPON_MIX,			/// ������ ����, ���� ���ݷ�(��������)

			DT_BUFF,				/// ������. �� ����Ÿ���� ��*Ÿ�ݽ� ���� + ����ġ, ��*Ÿ�ݽ� ���� + ����ġ, ��� ��ų ���� ������ ���� �ʴ´�
		};

		/// ���� ����
		enum ATTACK_TYPE
		{
			AT_NORMAL = 0,
			AT_SPECIAL,
		};

		/// Ÿ�� ����
		enum HIT_TYPE
		{
			HT_NO_SOUND = -1,	/// ����
			HT_DEFAULT = 0,		/// �⺻
			HT_SWORD_SLASH,		/// Į ����
			HT_SWORD_SLASH2,	/// Į ����2
			HT_SWORD_HIT,		/// Į Ÿ��
			HT_SWORD_CRASH,		/// Į �浹

			HT_ROD_SLASH,		/// ������ ����
			HT_ROD_SLASH2,		/// ������ ����2
			HT_ROD_HIT,			/// ������ Ÿ��
			HT_ROD_CRASH,		/// ������ �浹

			HT_PUNCH_HIT,		/// �ָ� ġ��
			HT_PUNCH_HIT2,		/// �ָ� ġ��2
			HT_PUNCH_CRASH,		/// �ָ� �浹

			HT_KICK_SLASH,		/// �� ����
			HT_KICK_SLASH2,
			HT_KICK_HIT,
			HT_KICK_CRASH,
			
			HT_ARROW_HIT,
			HT_ARROW_HIT2,
			HT_ARROW_CRASH,
			
			HT_FIRE,
			HT_FIRE2,
			HT_BOMB,
			HT_WOOD,

			HT_LIGHTNING,

			HT_NASOD_KING,

			HT_METAL_PUNCH_HIT,
            
#ifdef GROUND_HIT
            HT_GROUND_HIT,
#endif
			HT_SLAP,				// �̺� �� ������
			HT_EVE_SLASH1,
			HT_EVE_SLASH2,

//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
//#ifdef	NEW_CHARACTER_CHUNG
			HT_CHUNG_SLASH1,
			HT_CHUNG_SLASH2,
			HT_CHUNG_SLASH3,
//#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

//{{ kimhc // 2011.1.21 // û 1�� ����
//#ifdef	CHUNG_FIRST_CLASS_CHANGE
			HT_HANDGUN_HIT1,
			HT_HANDGUN_HIT2,
//#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.21 // û 1�� ����
			
#ifdef AREA_HIT
			HT_AREA_HIT,
#endif
			HT_SPEAR_HIT,


#pragma region �ƶ� â ����� ����
#ifdef ARA_CHARACTER_BASE
			HT_POLE_BRANDISH_WEAK,		/// â ���� ����
			HT_POLE_BRANDISH_STRONG,	/// â ���� ����
			HT_SHORTSWORD_SLASH,		/// �ܰ� ������
			HT_SHORTSWORD_PIERCE,		/// �ܰ� ���
#endif
#pragma endregion 

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
			HT_ELECTRIC_1,				/// ������Ʈ 1
			HT_ELECTRIC_2,				/// ������Ʈ 2
			HT_PLASMA_1,				/// �ö����Ʈ 1
			HT_PLASMA_2,				/// �ö����Ʈ 2
			HT_PRESSURE_1,				/// ����/�������Ʈ 1
			HT_PRESSURE_2,				/// ����/�������Ʈ 2
			HT_WATER_1,					/// ��ü��Ʈ 1
			HT_WATER_2,					/// ��ü��Ʈ 2
			HT_WATER_3,					/// ��ü��Ʈ 3
			HT_ICE,						/// ������Ʈ
			HT_METAL,					/// ������Ʈ
			HT_WIND,					/// �ٶ���Ʈ
			HT_STONE,					/// ������ ��Ʈ
#endif //SERV_9TH_NEW_CHARACTER

			HIT_TYPE_COUNT,
		};		
		
		//�ǰ� ���� - ���� ����
		enum HITTED_TYPE
		{
			HTD_NO_SOUND = -1,
			HTD_DEFAULT	= 0,
			HTD_MEAT,
			HTD_WOOD,
			HTD_STONE,
			HTD_STONE2,
			HTD_METAL,
			HTD_METAL2,
			HTD_BEEHOUSE,
//{{ kimhc // 2011.1.21 // û 1�� ����
//#ifdef	CHUNG_FIRST_CLASS_CHANGE
			HTD_GUARD_1,
//#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.21 // û 1�� ����
//#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
			HTD_GUARD_2,
//#endif //UPGRADE_SKILL_SYSTEM_2013
			HITTED_TYPE_COUNT,
		};

		/// ���� ���� ����
		enum REACT_TYPE
		{
			RT_NO_DAMAGE = 0,			/// �����̰ų� ��� �ƹ��� ���׼ǵ� ���� ����			
			RT_SMALL_DAMAGE,			/// ���� ������
			RT_BIG_DAMAGE,				/// ū ������
			RT_DOWN,					/// ��������
			RT_UP,						/// ���� �Ŀø���
			RT_FLY,						/// ���ư��� �Ѿ�����
			RT_REVENGE,					/// �ݰݱ� ����
			RT_DUMMY_DAMAGE,			/// �´� ǥ�ð� �ȳ��� ������
			RT_GROGGY,					/// ��Ʋ�Ÿ��� ����
			RT_GUARD,					/// ���
            RT_SENSOR,          		/// ����Ÿ�� (NO ������)
			RT_CREATE_DAMAGE_EFFECT,	/// ���� ��ġ�� ���ο� ������ ����Ʈ�� �����Ѵ�
			RT_CREATE_EFFECT_SET,		/// 
			RT_GRAPPLE,					/// ��� ����
			RT_THROW,					/// ������
			RT_BOUNDING,				///
			RT_DRAG_UP,					/// kimhc // 2010.02.08 // ��ġ���� ���� ���� ��� ���� �ø��� ����
//#ifdef NEW_SKILL_2010_11
			RT_REFLECT,					// oasis907 : ����� [2010.11.1] // �� �����̾� - �������� ����
//#endif NEW_SKILL_2010_11

			//{{ kimhc // 2011.1.8 // û 1�� ����
			RT_FLIP_SAME_DIR,			/// kimhc // 2011.01.08 // ������ ���� ������ �ٶ󺸵��� ȸ��(�̹� ���� ������ ���� �ִ� ��쿡�� ȸ������ ����, �׿ܿ��� �⺻������ Small_Damage_Type�� ���� ó��)
			//}} kimhc // 2011.1.8 // û 1�� ����

// #ifdef NEW_GRAPPLE_ATTACK
// 			RT_NEW_GRAPPLE,				/// lucidash :  ������. 
// #endif NEW_GRAPPLE_ATTACK
//#ifdef SEASON3_MONSTER_2011_7
			RT_ABSORB_MAGIC,
//#endif SEASON3_MONSTER_2011_7
//#ifdef ARA_CHANGE_CLASS_FIRST
			RT_AVOIDANCE,				/// ȸ��
//#endif //ARA_CHANGE_CLASS_FIRST
// #ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ
			RT_FLIP_DIFF_DIR,					/// kimjh, ������ �ٸ� ������ �ٶ󺸵��� ( ���� ���ֺ����� ) ȸ�� // ���߿� ���
			RT_FLIP_DIFF_DIR_DUMMY_DAMAGE,		/// kimjh, ������ �ٸ� ������ �ٶ󺸵��� ( ���� ���ֺ����� ) ȸ�� // ���߿� ���
// #endif // SERV_ELESIS_SECOND_CLASS_CHANGE // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ

		};

		enum REACTER_TYPE
		{
			AT_UNIT = 0,
			AT_EFFECT,
			AT_MESH,
			AT_MANUAL_COLLISION_DATA,
		};

		enum EXTRA_DAMAGE_TYPE
		{
			EDT_NONE = 0,
			EDT_POISON,
			EDT_SLOW,
			EDT_FIRE,
			EDT_CURSE,
			EDT_SILENCE,
			EDT_LIGHTNING,
			EDT_REVERSE_LEFT_RIGHT,
			EDT_COLD,
			EDT_Y_PRESSED,



			EDT_FIRE_SMASH,				/// �巡��� ������ ȭ����Ÿ, �ߺ� ������ extra_damage
			EDT_COLD_SMASH,				/// �ñ� ��Ÿ
			EDT_EVENT_SMASH,			/// �̺�Ʈ������ ����ϴ� ��Ÿ �迭 extra_damage
			EDT_LIGHTNING_SMASH,		/// kimhc // 2010-07-30 // ��Ʈȿ���� ���� ��Ÿ �߰�

			EDT_POISON_CLOUD,			/// ������
			EDT_DOUBLE_ATTACK,			/// �ι� ����

			EDT_MANA_DAMAGE,			/// MANA DAMAGE�� �ִ� Extra Damage, �������� ���� 

			EDT_FROZEN,					/// �����Ǽ� �������̰� ������ �Դ�
			EDT_LEG_WOUND,				/// ��ü ��ó, �������� ������, ��� �Ұ���, SI_A_RST_CUT_TENDON ����
			EDT_WEAPON_BREAK,			/// ���� �ı� 
			EDT_ARMOR_BREAK,			/// �� �ı� 

			EDT_ENTANGLE,				/// ������ �ӵ� 0, ���ڸ� ����. �ǰݽ� �ٿ� ���. ����, �ʻ�� ����
			EDT_DULL,					/// ��ȯ (SLOW�ʹ� �ٸ��� �Ӽ����� �������� ���� / ���ӵ����� ����)
			EDT_TIME_BOMB,				/// ���� ��ź
			EDT_STUN,					/// 
			EDT_AGING,					/// ��ȭ (����,�̼�,����,���ݷ� ����) 
			EDT_DAMAGE_RATE,			/// �ǰ��� ���� hp�� Ư�������� �������� ������.			

			EDT_MAGIC_DEFENCE_DEBUFF, 
			EDT_STIGMA_DEBUFF,			/// �ǰ��ڰ� �޴� ũ��Ƽ�� �������� ������Ų�� (����)

			EDT_DRYAD,					/// ���ʱ۰� ȿ���� �����ϳ�, �ڿ������� ����Ǹ�, ����̾�� ����Ʈ�� ���� �����			
//#ifdef EDT_EVENT_SMASH_SECOND
			EDT_EVENT_SMASH_2,
//#endif EDT_EVENT_SMASH_SECOND

//#ifdef NEW_SKILL_2010_11 // oasis907 : ����� [2010.11.8] // �ε� ����Ʈ - ���� ����
			EDT_BLIND,
//#endif NEW_SKILL_2010_11

			//{{ kimhc // 2010.12.29 // �ϸ� ���� �߰��� ���� ExtraDamage
			EDT_ICE_STING,
			EDT_FIRE_BALL,
			//}} kimhc // 2010.12.29 // �ϸ� ���� �߰��� ���� ExtraDamage

//#ifdef EDT_WATER_HOLD_TEST   // lucidash :: ����� [2010.1.4] 
			EDT_WATER_HOLD,
//#endif EDT_WATER_HOLD_TEST

			EDT_BIGHEAD,				// �������� ��� �Ұ� (user reform scale�� �浹��)
			//{{ 2011-04 �� ��ġ�� û ĳ�� ��ų
//#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
			EDT_PANIC,					/// �¿�Ű ������ ���� �������� �ɷȴ� Ǯ�ȴٸ� �ݺ� & ũ��Ƽ�ÿ� ���� Ȯ�� ����
			EDT_PAIN,					/// ���濡�� �ɷ��ִ� ExtraDamage�� ���� �����ϴ� Ƚ���� �޶����� ExtraDamage
			EDT_HEAD_SHOT,				/// ���� �����, ��弦 �޸� ���� �� �ߵ�
//#endif	CASH_SKILL_FOR_CHUNG_2011_04				
			//}} 2011-04 �� ��ġ�� û ĳ�� ��ų
			
			//{{ JHKang / ������ / 2011.6.19 / LEG_SHOT EDT
//#ifdef CHUNG_SECOND_CLASS_CHANGE
			EDT_LEG_SHOT,		/// �̵�, ���� �ӵ� ����
//#endif
			//}}
//#ifdef CHUNG_SECOND_CLASS_CHANGE // oasis907 : ����� [2011.6.16] û 2�� ���� - ���̾� �ȶ��
			EDT_TRANSIT_DEBUFF,
//#endif CHUNG_SECOND_CLASS_CHANGE			
//#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
			EDT_WIND_STORM,
			EDT_EARTH_QUAKE,
//#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE

//#ifdef VELDER_SECRET_DUNGEON
			EDT_CURSE_CLOUD,

			EDT_SCALE_CRITICAL_BUFF,
			EDT_BERSERKER_BUFF,
			EDT_MANA_BUFF,
			EDT_ATTACK_ALL_TEAM,
//#endif
//#ifdef UPGRADE_RAVEN
			EDT_RUNJUMP_SPEED,
//#endif
//#ifdef RAVEN_WEAPON_TAKER
			EDT_SMOKE_BLIND,
//#endif RAVEN_WEAPON_TAKER
//#ifdef EVE_ELECTRA
			EDT_FLASH_BANG,
//#endif EVE_ELECTRA
//#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
			EDT_MARK_OF_COMMANDER,
//#endif SERV_ADD_CHUNG_SHELLING_GUARDIAN

//#ifdef ADD_SOCKET_FOR_SECRET_SET
			EDT_FLAME_SMASH,		/// ���ùڽ��� ������ ȭ�� Ÿ��
			EDT_FIRE_WRAITH,		/// ����Ÿ ��Ʈ�� ���̽� �߻�
//#endif

//#ifdef SERV_RENA_NIGHT_WATCHER
			EDT_START_OF_DELAYED_FIRING,	///mauntain : ����ȯ [2012.05.21] ���� 2�� ���� ����Ʈ ���� - ������ ȭ��
//#endif SERV_RENA_NIGHT_WATCHER

//#ifdef SERV_RAVEN_VETERAN_COMMANDER
			EDT_BLEEDING,			///mauntain : ����ȯ [2012.07.16] ���̺� 3�� ���� ���׶� Ŀ�Ǵ� - ġ���
//#endif SERV_RAVEN_VETERAN_COMMANDER

//#ifdef STARLIGHT_SMASH
			EDT_STARLIGHT_SMASH,	/// ���� ��Ÿ ����
//#endif STARLIGHT_SMASH

//#ifdef CAMILLA_SMASH
			EDT_CAMILLA_SMASH,		/// �ؿ��� �߰� ����
//#endif
//#ifdef ADD_SOCKET_OPTION_SANDER_SET
			EDT_FALLING_ROCK,		/// ���� ��Ʈ ȿ�� - ���� ��ȯ 
			EDT_SANDSTORM,			/// ���� ��Ʈ ȿ�� - �𷡵�ǳ ��ȯ

			EDT_WIND_VIBRATION,		/// ���� Īȣ 
			EDT_BREEZE_ACCEL,
			EDT_EVIL_SWORD,			
			EDT_SPIRIT_SWORD,		
			EDT_GHOST_SWORD,
//#endif //ADD_SOCKET_OPTION_SANDER_SET

//#ifdef HAMEL_SECRET_DUNGEON // ����ȯ
			EDT_ACTIVE_DEBUFF,		/// Ÿ�ݽ� Ư�� ������� ����
//#endif // HAMEL_SECRET_DUNGEON

//#ifdef EDT_EVENT_SMASH_THIRD
			EDT_EVENT_SMASH_3,
//#endif EDT_EVENT_SMASH_THIRD

			//////////////////////////////////////////////////////////////////////////
			// 1�� �Ӽ� ��æƮ
			EDT_ENCHANT_BLAZE			= 100,
			EDT_ENCHANT_FROZEN,
			EDT_ENCHANT_POISON,
			EDT_ENCHANT_PIERCING,
			EDT_ENCHANT_SHOCK,
			EDT_ENCHANT_SNATCH,


			
			// 2�� �Ӽ� ��æƮ
			EDT_ENCHANT_BLAZE_MASTER,
			EDT_ENCHANT_FROZEN_MASTER,
			EDT_ENCHANT_POISON_MASTER,
			EDT_ENCHANT_MASTER_PIERCING,
			EDT_ENCHANT_MASTER_SHOCK,
			EDT_ENCHANT_MASTER_SNATCH,

			EDT_ENCHANT_BLAZE_PIERCING,
			EDT_ENCHANT_BLAZE_SHOCK,
			EDT_ENCHANT_BLAZE_SNATCH,

			EDT_ENCHANT_FROZEN_PIERCING,
			EDT_ENCHANT_FROZEN_SHOCK,
			EDT_ENCHANT_FROZEN_SNATCH,

			EDT_ENCHANT_POISON_PIERCING,
			EDT_ENCHANT_POISON_SHOCK,
			EDT_ENCHANT_POISON_SNATCH,


			// 3�� �Ӽ� ENCHANT
			EDT_ENCHANT_BLAZE_DOUBLE_MASTER,
			EDT_ENCHANT_FROZEN_DOUBLE_MASTER,
			EDT_ENCHANT_POISON_DOUBLE_MASTER,
			EDT_ENCHANT_DOUBLE_MASTER_PIERCING,
			EDT_ENCHANT_DOUBLE_MASTER_SHOCK,
			EDT_ENCHANT_DOUBLE_MASTER_SNATCH,
			EDT_ENCHANT_DOUBLE_BLAZE_PIERCING,
			EDT_ENCHANT_DOUBLE_BLAZE_SHOCK,
			EDT_ENCHANT_DOUBLE_BLAZE_SNATCH,
			EDT_ENCHANT_DOUBLE_FROZEN_PIERCING,
			EDT_ENCHANT_DOUBLE_FROZEN_SHOCK,
			EDT_ENCHANT_DOUBLE_FROZEN_SNATCH,
			EDT_ENCHANT_DOUBLE_POISON_PIERCING,
			EDT_ENCHANT_DOUBLE_POISON_SHOCK,
			EDT_ENCHANT_DOUBLE_POISON_SNATCH,
			EDT_ENCHANT_BLAZE_DOUBLE_PIERCING,
			EDT_ENCHANT_BLAZE_DOUBLE_SHOCK,
			EDT_ENCHANT_BLAZE_DOUBLE_SNATCH,
			EDT_ENCHANT_FROZEN_DOUBLE_PIERCING,
			EDT_ENCHANT_FROZEN_DOUBLE_SHOCK,
			EDT_ENCHANT_FROZEN_DOUBLE_SNATCH,
			EDT_ENCHANT_POISON_DOUBLE_PIERCING,
			EDT_ENCHANT_POISON_DOUBLE_SHOCK,
			EDT_ENCHANT_POISON_DOUBLE_SNATCH,

		};

#ifdef ELSWORD_WAY_OF_SWORD
		enum WAY_OF_SWORD_TYPE
		{
			WST_NONE,
			WST_VIGOR_NORMAL,
			WST_VIGOR_ACTIVE,
			WST_VIGOR_SPECIAL,
			WST_DESTRUCTION_NORMAL,
			WST_DESTRUCTION_ACTIVE,
			WST_DESTRUCTION_SPECIAL,			
		};
#endif ELSWORD_WAY_OF_SWORD

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
		/// ���� Ÿ�Կ� ���� DP ���� ��ġ
		/// �ش� Ÿ���� Enum.lua�� ���� �Ǿ� �ְ�, Add_NasodRuler.lua�� ���� ��Ī �Ǿ� �ִ�.
		/// �ڵ�� CX2GUAdd::GetDPRateByType �Լ� ������ �������̴�.
		enum DYNAMO_PARTICLE_RATE_TYPE
		{
			DPRT_NONE				= -1,
			DPRT_SPECIAL_ACTIVE		= 0,	/// ����� ��Ƽ��
			DPRT_ACTIVE				= 1,	/// ��Ƽ�� ����
			DPRT_COMMAND			= 2,	/// Ŀ�ǵ� ����
			DPRT_NASOD_ARMOR		= 3,	/// ���ҵ� �Ƹ� ����
		};
#endif //SERV_9TH_NEW_CHARACTER

#ifdef ADD_RENA_SYSTEM //��â��
		enum NATURAL_FORCE_TYPE
		{
			NFT_NONE			= -1,	
			NFT_COMBO			= 0,	// ��Ÿ ����
			NFT_COMBO_BOMB		= 1,	// ��Ÿ ��Ÿ ���� ����
			NFT_ACTIVE			= 2,	// ��Ƽ�� ����
			NFT_SPECIAL_ACTIVE	= 3,	// ����� ��Ƽ�� ����
			NFT_RESULT_BOMB		= 4,	// NF�� �Ҹ��Ͽ� �߻��� ����
		};

		enum FIRST_ATTACK_CHECK
		{
			FAC_NOT_CHECK		= -1,	// ù��° �����̶�� ���� üũ ���� ����
			FAC_NONE			= 0,	// ���� ù��° ������ ���� ����.
			FAC_FIRST_ATTACK	= 1,	// ù��° ���� ������ �̷�� ��.
		};
#endif //ADD_RENA_SYSTEM

#ifdef AREA_HIT
		enum DAMAGE_AREA_TYPE
		{
			DAT_ALL_UNIT = 0,
			DAT_NEAR_UNIT,
			DAT_FAR_UNIT,
		};

		struct sAreaHitUnit
		{
			int unitIndex;
			float fDistance;
		};
#endif

#ifdef SERV_RENA_NIGHT_WATCHER

		struct StartOfDelayedFiringData
		{
			int			m_iType;				/// ȭ�� Ÿ��( ������ ȭ�� ���� ������ ��ȣź ���� )
			int			m_iAttackerIndex;		/// �ش� ȭ�� �߻���
			int			m_iArrowOrderNumber;	/// ȭ�� �߻� ����
			int			m_iArrowIndex;			/// ���� ���� ȭ���� �ε���
			float		m_fCoolTime;			/// EDT ���� �ð�
			float		m_fIncreaseDamage;		/// �ش� ��ü�� �߰� ������
			float		m_fSkillPowerRate;
			D3DXVECTOR3 m_vExplosionOffSet;		/// ���ֿ��� ���� �� ����Ʈ ��ġ

			StartOfDelayedFiringData( int iType, int iAttackerIndex, int iArrowOrderNumber, int iArrowIndex = -1, float fCoolTime = 10.f, float fIncreaseDamage = 1.f )
			{
				m_iType				= iType;
				m_iAttackerIndex	= iAttackerIndex;
				m_iArrowOrderNumber = iArrowOrderNumber;
				m_iArrowIndex		= iArrowIndex;
				m_fCoolTime			= fCoolTime;
				m_fIncreaseDamage	= fIncreaseDamage;
				m_fSkillPowerRate	= 1.f;
				m_vExplosionOffSet  = D3DXVECTOR3( 0.f, 0.f, 0.f );
			}
		};

		struct StartOfDelayedFiringDataSort

		{
			bool operator()( const CX2DamageManager::StartOfDelayedFiringData* _Left, const CX2DamageManager::StartOfDelayedFiringData* _Right ) const
			{
				if( _Left->m_iArrowOrderNumber < _Right->m_iArrowOrderNumber )
				{
					return true;
				}
				else
				{
					return false;
				}
			};
		};

#endif SERV_RENA_NIGHT_WATCHER

		struct ExtraDamageData
		{
			EXTRA_DAMAGE_TYPE		m_ExtraDamageType;
			KProtectedType<float>	m_fTime;
			KProtectedType<float>	m_DamagePerSec;
			KProtectedType<int>		m_Accumulation;
			KProtectedType<float>	m_fElapsedTimeToEffectDamage;
			KProtectedType<float>	m_fRate;			/// �� extra damage�� �۵��� Ȯ��
#ifdef EXTRADAMAGE_RATE_BUG_FIX			
			KProtectedType<float>	m_fPureRate;		/// Rate ���� ������ ���� ������ ���� Rate
#endif EXTRADAMAGE_RATE_BUG_FIX
			KProtectedType<float>	m_Damage;			/// ������

#ifdef NEW_EXTRA_DAMAGE
			KProtectedType<int>		m_iLevel;			/// ����
			KProtectedType<int>		m_iAddLevel;		/// �߰� ����
			KProtectedType<bool>	m_bIgnoreResist;	/// ����
#endif
#ifdef ADDITIONAL_MEMO
			bool					m_bDefenderLevel;
#endif //def ADDITIONAL_MEMO

#ifdef SKILL_CASH_10_TEST
			KProtectedType<float>	m_fDamageRate;		/// ������
			KProtectedType<float>	m_fRunJumpRate;		/// �̵�, ���� �ӵ�
			KProtectedType<float>	m_fAnimSpeedRate;	/// �ִϸ��̼� �ӵ�
#endif

			bool					m_bFixedDamage;

			//{{ 2011-04 �� ��ġ�� û ĳ�� ��ų
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
			/// ExtraDamage�� Map���� �ٲٰ� �Ǹ� ����
			bool					m_bCorrectDirection;		/// Panic���� Ű���� �Է¿� ���� ���������� �����̴��� ���θ� �����ϴ� �÷���
			KProtectedType<float>	m_fCriticalRateToDefender;	/// �ǰ��ڰ� ũ��Ƽ�ÿ� ���� Ȯ�� ����
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
			//}} 2011-04 �� ��ġ�� û ĳ�� ��ų

			//{{ JHKang / ������ / 2011.6.20 / ���� �� ���� �̵�, ���� �ӵ� ���� ������ ���� ���� �߰�
#ifdef CHUNG_SECOND_CLASS_CHANGE
			KProtectedType<float>	m_fJumpRate;		/// ���� �ӵ�(LEG_SHOT���� m_fRunJumpRate�� �̵� �ӵ��� ���)
#endif
			//}}

			//{{ kimhc // 2011-07-29 // Į�󺼱� ���� ���� ����
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
			// ����� EDT_PAIN���� ������ ���� EDT������ �����ϱ� ���� �뵵�� ���
			CX2GameUnitoPtr			m_optrGameUnit;
#endif	CASH_SKILL_FOR_CHUNG_2011_04
			//}} kimhc // 2011-07-29 // Į�󺼱� ���� ���� ����
#ifdef ELSWORD_WAY_OF_SWORD			// CureExtraDamage - ���� ����, ����� ����, �ӹ� ������ ExtraDamage ������ ������ ���� �� ����.
			bool					m_bCured;
#endif ELSWORD_WAY_OF_SWORD

#ifdef SERV_RENA_NIGHT_WATCHER
			vector<CX2DamageManager::StartOfDelayedFiringData*> m_vecStartOfDelayedFiringData;		/// ������ ��ȣź ���� ����
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
			vector<float>				m_vecOverlapDamaage;	/// ġ��� ��ø ������ ����
#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef HAMEL_SECRET_DUNGEON // ����ȯ
			CX2BuffFactorPtr			m_ptrBuffFactor;		/// ������ ���� ����
#endif // HAMEL_SECRET_DUNGEON


#ifdef  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
            bool                    m_bAttackedByMyUnit;        /// �� ���ֿ� ���� ���ݴ��ߴ���
#endif  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

			ExtraDamageData()
			{
				Init();
			}
			void Init()
			{
				m_fRate							= 0.f;
#ifdef EXTRADAMAGE_RATE_BUG_FIX					
				m_fPureRate						= 0.f;
#endif EXTRADAMAGE_RATE_BUG_FIX					
				m_Damage						= 0.0f;
				m_ExtraDamageType				= EDT_NONE;
				m_fTime							= 0.f;
				m_DamagePerSec					= 0.f;
				m_Accumulation					= 0;
				m_fElapsedTimeToEffectDamage	= 0.f;

#ifdef NEW_EXTRA_DAMAGE
				m_iLevel						= -1;
				m_iAddLevel						= 0;
				m_bIgnoreResist					= false;
#endif
#ifdef ADDITIONAL_MEMO
				m_bDefenderLevel				= false;
#endif

#ifdef SKILL_CASH_10_TEST
				m_fDamageRate					= 0.f;
				m_fRunJumpRate					= 0.f;
				m_fAnimSpeedRate				= 0.f;
#endif
				m_bFixedDamage					= false;
				//{{ 2011-04 �� ��ġ�� û ĳ�� ��ų
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
				/// ExtraDamage�� Map���� �ٲٰ� �Ǹ� ����
				m_fCriticalRateToDefender		= 0.0f;
				m_bCorrectDirection				= true;	/// Panic���� Ű���� �Է¿� ���� ���������� �����̴��� ���θ� �����ϴ� �÷���
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
				//}} 2011-04 �� ��ġ�� û ĳ�� ��ų

				//{{ JHKang / ������ / 2011.6.20 / ���� �� ���� �̵�, ���� �ӵ� ���� ������ ���� ���� �߰�
#ifdef CHUNG_SECOND_CLASS_CHANGE
				m_fJumpRate						= 0.f;
#endif
				//}}

				//{{ kimhc // 2011-07-29 // Į�󺼱� ���� ���� ����
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
				m_optrGameUnit.Reset();
#endif	CASH_SKILL_FOR_CHUNG_2011_04
				//}} kimhc // 2011-07-29 // Į�󺼱� ���� ���� ����
#ifdef ELSWORD_WAY_OF_SWORD
				m_bCured						= false;
#endif ELSWORD_WAY_OF_SWORD

#ifdef SERV_RENA_NIGHT_WATCHER
				for( UINT i = 0; i < m_vecStartOfDelayedFiringData.size(); ++i )		/// ������ ��ȣź ���� ����
				{
					CX2DamageManager::StartOfDelayedFiringData* pData = m_vecStartOfDelayedFiringData[i];
					SAFE_DELETE( pData );
				}
				m_vecStartOfDelayedFiringData.erase( m_vecStartOfDelayedFiringData.begin(), m_vecStartOfDelayedFiringData.end() );
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
				m_vecOverlapDamaage.erase( m_vecOverlapDamaage.begin(), m_vecOverlapDamaage.end() );	/// ġ��� ��ø ������ ���� ����
#endif SERV_RAVEN_VETERAN_COMMANDER

#ifdef  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
                m_bAttackedByMyUnit = false;
#endif  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

			}
			void Copy( const ExtraDamageData& data )
			{
				m_fRate							= data.m_fRate;
#ifdef EXTRADAMAGE_RATE_BUG_FIX					
				m_fPureRate						= data.m_fPureRate;
#endif EXTRADAMAGE_RATE_BUG_FIX					
				m_Damage						= data.m_Damage;
				m_ExtraDamageType				= data.m_ExtraDamageType;
				m_fTime							= data.m_fTime;
				m_DamagePerSec					= data.m_DamagePerSec;
				m_Accumulation					= data.m_Accumulation;
				m_fElapsedTimeToEffectDamage	= data.m_fElapsedTimeToEffectDamage;

#ifdef NEW_EXTRA_DAMAGE
				m_iLevel						= data.m_iLevel;
				m_iAddLevel						= data.m_iAddLevel;
				m_bIgnoreResist					= data.m_bIgnoreResist;
#endif
#ifdef ADDITIONAL_MEMO
				m_bDefenderLevel				= data.m_bDefenderLevel;
#endif
#ifdef SKILL_CASH_10_TEST
				m_fDamageRate					= data.m_fDamageRate;
				m_fRunJumpRate					= data.m_fRunJumpRate;
				m_fAnimSpeedRate				= data.m_fAnimSpeedRate;
#endif
				m_bFixedDamage					= data.m_bFixedDamage;
				//{{ 2011-04 �� ��ġ�� û ĳ�� ��ų
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
				/// ExtraDamage�� Map���� �ٲٰ� �Ǹ� ����
				m_fCriticalRateToDefender		= data.m_fCriticalRateToDefender;
				m_bCorrectDirection				= data.m_bCorrectDirection;	/// Panic���� Ű���� �Է¿� ���� ���������� �����̴��� ���θ� �����ϴ� �÷���
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
				//}} 2011-04 �� ��ġ�� û ĳ�� ��ų

				//{{ JHKang / ������ / 2011.6.20 / ���� �� ���� �̵�, ���� �ӵ� ���� ������ ���� ���� �߰�
#ifdef CHUNG_SECOND_CLASS_CHANGE
				m_fJumpRate						= data.m_fJumpRate;
#endif
				//}}

				//{{ kimhc // 2011-07-29 // Į�󺼱� ���� ���� ����
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
				m_optrGameUnit					= data.m_optrGameUnit;		// swallow copy
#endif	CASH_SKILL_FOR_CHUNG_2011_04
				//}} kimhc // 2011-07-29 // Į�󺼱� ���� ���� ����
#ifdef ELSWORD_WAY_OF_SWORD
				m_bCured						= data.m_bCured;
#endif ELSWORD_WAY_OF_SWORD

#ifdef HAMEL_SECRET_DUNGEON // ����ȯ
				m_ptrBuffFactor					= data.m_ptrBuffFactor;
#endif // HAMEL_SECRET_DUNGEON

#ifdef  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
                m_bAttackedByMyUnit = data.m_bAttackedByMyUnit;
#endif  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION

			}
			bool Verify() const
			{
				if( m_fTime.Verify() == false
					|| m_DamagePerSec.Verify() == false 
					|| m_Accumulation.Verify() == false
					|| m_fElapsedTimeToEffectDamage.Verify() == false 
					|| m_fRate.Verify() == false 
#ifdef EXTRADAMAGE_RATE_BUG_FIX						
					|| m_fPureRate.Verify() == false
#endif EXTRADAMAGE_RATE_BUG_FIX		
					|| m_Damage.Verify() == false
#ifdef NEW_EXTRA_DAMAGE
					|| m_iLevel.Verify() == false
					|| m_iAddLevel.Verify() == false
					|| m_bIgnoreResist.Verify() == false
#endif
#ifdef SKILL_CASH_10_TEST
					|| m_fDamageRate.Verify() == false
					|| m_fRunJumpRate.Verify() == false
					|| m_fAnimSpeedRate.Verify() == false
#endif
					//{{ 2011-04 �� ��ġ�� û ĳ�� ��ų
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
					/// ExtraDamage�� Map���� �ٲٰ� �Ǹ� ����
					|| m_fCriticalRateToDefender.Verify() == false
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
				//}} 2011-04 �� ��ġ�� û ĳ�� ��ų
#ifdef CHUNG_SECOND_CLASS_CHANGE
					|| m_fJumpRate.Verify() == false
#endif
					)
				{
					return false;
				}
				return true;
			}

#ifdef NEW_EXTRA_DAMAGE
			float GetCalcPerDamage(int iUnitLevel, float fResist) const;
#endif

		};

		struct HitUnit
		{
			float			fRemainGap; //0���� �۾����� ���� ���ݰ���
			CX2GameUnitoPtr	optrHitGameUnit;

            HitUnit()
                : fRemainGap( 0.f )
                , optrHitGameUnit()
            {
            }
		};
        typedef std::vector<HitUnit>    HitUnitVector;
#ifdef  X2OPTIMIZE_STL_CONTAINER_USAGE
        static inline void swap( HitUnit& lhs_, HitUnit& rhs_ )
        {
            std::swap( lhs_.fRemainGap, rhs_.fRemainGap );
            lhs_.optrHitGameUnit.Swap( rhs_.optrHitGameUnit );
        }
#endif  X2OPTIMIZE_STL_CONTAINER_USAGE

		struct Damage
		{
			float	fPhysic;
			float	fMagic;

			Damage()
			{
				Init();
			}
			void Init()
			{
				fPhysic		= 0.0f;
				fMagic		= 0.0f;
			}
			float GetAllDamage()
			{
				return fPhysic + fMagic;
			}
		};

		struct AttackerGet
		{
			float m_fHP;
			float m_fMP;

			AttackerGet()
			{
				Init();
			}

			void Init()
			{
				m_fHP = 0.f;
				m_fMP = 0.f;
			}
		};

#ifdef ADD_RENA_SYSTEM //��â��
		struct DamageRelateSkillData
		{
			Byte					m_byteSkillIndex;	// ��ų ������ ���� index
			Byte					m_byteRelateData;	//�ִ�� �Ҹ��ϴ� NF������ ���� ����

			DamageRelateSkillData():m_byteSkillIndex(0), m_byteRelateData(0) {}

			void Init()
			{
				m_byteSkillIndex = 0;
				m_byteRelateData = 0;
			}
		};
#endif //ADD_RENA_SYSTEM

		struct DamageData
		{
			DAMAGE_TYPE						damageType;
			REACTER_TYPE					attackerType;
			ATTACK_TYPE						attackType;
			CX2GameUnitoPtr					optrAttackerGameUnit;	/// �̰͵� �̰�����.. UID�� �ʿ��� ����..
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            CX2DamageEffect_CEffectHandle   hAttackerEffect;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CX2DamageEffectBasic*			pAttackerEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			//{{ seojt // 2009-1-12, 16:15
			CKTDGXSkinAnim*	                pAttackerAnim;			/// �����...
			//CKTDGXSkinAnimPtr	            pAttackerAnim;
			//}} seojt // 2009-1-12, 16:15
			CKTDXCollision::CollisionData*	pCollisionData;
			

			HIT_TYPE				hitType;
			REACT_TYPE				reActType;
			REACT_TYPE				reActResult;            
			bool					m_bMeleeAttack;		/// ���������ΰ�?(�⺻������ ������ Ÿ�ݹڽ��� true, ����������Ʈ�� false �̳�, ��ũ��Ʈ�� DAMAGE_DATA���� ����Ʈ���� �������� ���氡��)
			bool					m_bDrain;
			bool					m_bDrainMana;
			bool					m_bAttackAllTeam;
			float					m_fDungeonRate;
						
			int						techPoint;
			Damage					damage;							/// ����, �������� �ٲٴ°�...

			vector<CX2BuffFactorPtr>	m_vecBuffFactorPtr;			/// ���濡�� ����/�����

			ExtraDamageData			m_ExtraDamage;					/// ������ ��ũ��Ʈ�� �ִ� extra_damage
			ExtraDamageData			m_BufExtraDamage;				/// ������ ������ 
			ExtraDamageData			m_SocketExtraDamage;			/// ������ ���� �ɼǿ� ���� extra_damage, �ߺ� �ȵǴ� ��
			vector<ExtraDamageData>	m_MultipleExtraDamage;			/// ������ ���� �ɼǿ� ���� extra_damage, �ߺ� �Ǵ� ��


			AttackerGet				attackerGet;
			D3DXVECTOR2				backSpeed;
			float					fForceDownValue;
			float					fGroggyTime;

			float					fHitAddMP;
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
			float					fHitAddHPPer;					/// ���� ������ �����ڿ��� ȸ����ų ü�� ����
#endif // SERV_ARA_CHANGE_CLASS_SECOND

			float					fStopTimeAtt;
			float					fStopTimeDef;

			CKTDGCamera::SHAKE_TYPE	m_eCameraShakeType;
			float					fCameraCrashGap;
			float					fCameraCrashTime;
			int						clearScreen;
			D3DXCOLOR				clearScreenColor;
#ifdef ARRANGED_FLY_DAMAGE_TEST
			//{{ oasis907 : ����� [2010.2.11] // 
			bool					bArrangedFly;
			//}}
#endif ARRANGED_FLY_DAMAGE_TEST
			bool					bForceFly;
			bool					bCanRevenge;
			bool					bCanReflexMagic;
			bool					bReAttack;
			float					fHitGap;
			vector<HitUnit>			hitUnitList;

			D3DXVECTOR3				impactPoint;

			REACTER_TYPE			defenderType;
			CX2GameUnitoPtr			optrDefenderGameUnit;
#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			CX2DamageEffectBasic*	pDefenderEffect;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

            bool                    m_bGuard;
            bool                    m_bWBackSpeed;

			std::wstring			m_wstrCreateDamageEffectName;
			std::wstring			m_wstrCreateEffectSetName;
			bool					m_bCreateEffectSetTraceUnit;			// ����Ʈ �����ϴ� damage effect �� �� ������ effect set�� �ǰݴ��� ������ trace ���� ����
#ifdef TRACE_UNIT_DIE_DELETE_EFFECTSET
			bool					m_bTraceUnitDieDeleteEffectSet;			// trace ������ ������ �ش� ����Ʈ ���� ��������, true�� ����
#endif //TRACE_UNIT_DIE_DELETE_EFFECTSET
			bool					m_bPenetrate;
            
            bool                    DamageLineGroup;
			float					DamageDistance;

			bool					m_bFlipDirOnHit;		// Ÿ���� ���� ����� ������ �ݴ�� ��������

#ifdef AREA_HIT
			DAMAGE_AREA_TYPE		m_eDamageAreaType;
			int						m_iDamageUnitNum;
#endif
#ifdef SKILL_30_TEST
			bool					m_bNoDamage;
#endif
			bool					m_bHitedManaDamage;

			float					m_fDrainHpRate;		// Ÿ�ݽ� �������� �󸶸� hp�� ȯ��

			std::vector<std::wstring>	m_vecHitParticleName;

#ifdef NO_DETONATION
			bool					m_bNoDetonation;	// �������� ���ϵ��� �Ѵ�.
			bool					m_bDrainHpParticle; // �����...����׼���...HP�� ����� ����Ʈ�� ���� ������..
#endif
			bool					m_bApplyExtraDamage;

#ifdef DUNGEON_ITEM
			int						m_ResistType;
			bool					m_bAttackOurTeam;
			float					m_fHpPercentUp;
			float					m_fMpPercentUp;
#endif

#ifdef	APPLY_MOTION_OFFSET
			bool					m_bApplyMotionOffset;
#endif	APPLY_MOTION_OFFSET

#ifdef SERV_SKILL_NOTE
			float					m_fCriticalRate;
			float					m_fDecreaseForceDown;
#endif

			bool					m_bNoBuff;

#ifdef DAMAGEDATA_RATE_MODIFIER
			float					m_fRateModifier;
#endif DAMAGEDATA_RATE_MODIFIER

			// ���� DamageEffect.lua �ѹ����� �ʰ� �ݿ���Ű�� ���� �ּ� ����
			// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
			float					m_fHitDamageEffectInvokeRate;
			float					m_fHitDamageEffectDamageRate;

			bool					m_bIgnoreDefence;

#ifdef WIDE_BUFF_ANI_SPEED_UP
			bool	m_bAnimationSpeedUp;
			float	m_fAnimationSpeedUpRate;
#endif
#ifdef SKILL_BALANCE_20110728
			float	m_fPvpRate;
#endif

#ifdef UPGRADE_RAVEN
			DAMAGE_TRIGGER_TYPE		m_eDamageTrigger;	//
			bool					m_bOnlyAttackBasic;
			bool					m_bIgnoreStateDefence;
			bool					m_bNoMiss;
#endif

#ifdef ELSWORD_WAY_OF_SWORD
			WAY_OF_SWORD_TYPE		m_WayofSwordType;
#endif ELSWORD_WAY_OF_SWORD

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
			DYNAMO_PARTICLE_RATE_TYPE	m_DPRateType;		///	DP ���� ����
#endif //SERV_9TH_NEW_CHARACTER

#ifdef ADD_RENA_SYSTEM //��â��
			NATURAL_FORCE_TYPE		m_NaturalForceType;
#endif //ADD_RENA_SYSTEM

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
			float					m_fDrainMpByReact;
			float					m_fDrainMpRate;
			bool					m_bLvDamage;
#endif

#ifdef EVE_ELECTRA
			int						m_iLaserGroupID;
#endif EVE_ELECTRA

#ifdef ADD_BACKATTACK
			bool					m_bApplyBackAttack;
#endif

#ifdef SERV_ARME_DIMENSION_WITCH
			float					m_fCreateDamageEffectRate;
#endif

#ifdef SERV_CHUNG_TACTICAL_TROOPER		/// û 2�� ���� ��Ƽ�� Ʈ���� - ��Ƽ�� �ʵ�
			float					m_fIncreaseMP;				/// ���� ���Ƿ�
#endif SERV_CHUNG_TACTICAL_TROOPER

#ifdef PLAY_EMOTION_BY_DAMAGE_EFFECT
			bool					m_bIsLaughterDamage;
#endif // PLAY_EMOTION_BY_DAMAGE_EFFECT

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
			int							m_iHitAddForcePower;		/// �ǰݽ� �������� ��� ȸ��
			CX2SkillTree::SKILL_ID		m_eUsedSkillID;				/// �ش� ������ �����͸� ������ ��ų ���̵� ( ���� ������ ��� �ؿ�... )
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
            bool                    m_bRelaxNPCReactionStateCheck;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

#ifdef ADD_MEMO_1ST_CLASS //��â��
			float					m_fApplyCriticalDamage;			/// ũ��Ƽ�� ������ ������ ������Ų��. ex> ���� ũ��Ƽ�� ������ 1.5�� �ش� ���� ������ �������� �������� ���ȴ�.
			float					fHitAddHPbyAttackPower;			/// ���� ������ �����ڿ��� ȸ����ų ü�� ���� (������ �������� ��հ��� ����Ͽ� ����)
#endif //ADD_MEMO_1ST_CLASS

#ifdef ADD_RENA_SYSTEM //��â��
			FIRST_ATTACK_CHECK		m_eFirstAttack;					/// ù��° ������ �����ߴ°�?
			DamageRelateSkillData	m_RelateSkillData;
#endif //ADD_RENA_SYSTEM

			void SimpleInit()
			{
				damageType			= DT_PHYSIC;
				attackerType		= AT_UNIT;
				attackType			= AT_NORMAL;
				optrAttackerGameUnit.Reset();
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                hAttackerEffect     = INVALID_DAMAGE_EFFECT_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				pAttackerEffect		= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				//{{ seojt // 2009-1-12, 16:15
				pAttackerAnim		= NULL;
                //pAttackerAnim.reset();
				//}} seojt // 2009-1-12, 16:15
				pCollisionData		= NULL;
				m_bMeleeAttack		= false;
				m_bDrain			= false;
				m_bDrainMana		= false;
				m_bAttackAllTeam	= false;
				m_fDungeonRate		= 1.0f;

				techPoint			= 0;
				hitType				= HT_DEFAULT;
				reActType			= RT_NO_DAMAGE;
				reActResult			= RT_NO_DAMAGE;

				damage.Init();

				m_vecBuffFactorPtr.resize( 0 );
				m_ExtraDamage.Init();
				m_BufExtraDamage.Init();


				backSpeed			= D3DXVECTOR2( 0.0f, 0.0f );
				fForceDownValue		= 0.0f;
				fGroggyTime			= 0.0f;
				
				fHitAddMP			= 6.0f;
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
				fHitAddHPPer		= 0.0f;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

				fStopTimeAtt		= 0.0f;
				fStopTimeDef		= 0.0f;
				m_eCameraShakeType	= CKTDGCamera::DECT_UP_DOWN;
				fCameraCrashGap		= 0.0f;
				fCameraCrashTime	= 0.0f;
				clearScreen			= 0;
				clearScreenColor	= D3DXCOLOR(0x00ffffff);
#ifdef ARRANGED_FLY_DAMAGE_TEST
				//{{ oasis907 : ����� [2010.2.11] // 
				bArrangedFly		= false;
				//}
#endif ARRANGED_FLY_DAMAGE_TEST
				bForceFly			= false;
				bCanRevenge			= true;
				bCanReflexMagic		= false;
				bReAttack			= false;
				fHitGap				= 0.0f;
				
				hitUnitList.resize(0);
			
                m_bGuard = false;
                m_bWBackSpeed = false;

				m_wstrCreateDamageEffectName = L"";
				m_wstrCreateEffectSetName = L"";
				m_bCreateEffectSetTraceUnit = false;
#ifdef TRACE_UNIT_DIE_DELETE_EFFECTSET
				m_bTraceUnitDieDeleteEffectSet = true;
#endif //TRACE_UNIT_DIE_DELETE_EFFECTSET
				m_bPenetrate		= false;

                DamageLineGroup     = false;
				DamageDistance		= 99999.f;

				m_bFlipDirOnHit		= false;

#ifdef AREA_HIT
				m_eDamageAreaType	= DAT_ALL_UNIT;
				m_iDamageUnitNum	= 0;				
#endif
#ifdef SKILL_30_TEST
				m_bNoDamage			= false;
#endif
				m_bHitedManaDamage	= false;
				m_fDrainHpRate		= 0.f;

				m_vecHitParticleName.clear();

#ifdef NO_DETONATION
				m_bNoDetonation		= false;
#endif
#ifdef SKILL_CASH_10_TEST
				m_bDrainHpParticle	= false;
#endif
				m_bApplyExtraDamage = true;
#ifdef DUNGEON_ITEM
				m_ResistType = 0;
				m_fHpPercentUp = 0.f;
				m_fMpPercentUp = 0.f;
				m_bAttackOurTeam = false;
#endif
#ifdef	APPLY_MOTION_OFFSET
				m_bApplyMotionOffset	= true;
#endif	APPLY_MOTION_OFFSET
				
#ifdef SERV_SKILL_NOTE
				m_fCriticalRate		= 0.f;
				m_fDecreaseForceDown = 0.f;
#endif

				m_bNoBuff			= false;

#ifdef DAMAGEDATA_RATE_MODIFIER
				m_fRateModifier		= 1.f;
#endif DAMAGEDATA_RATE_MODIFIER

				// ���� DamageEffect.lua �ѹ����� �ʰ� �ݿ���Ű�� ���� �ּ� ����
				// oasis907 : ����� [2010.11.4] // �׷��� ��ó - ���� ���(�нú�)
				m_fHitDamageEffectInvokeRate = 1.f;
				m_fHitDamageEffectDamageRate = 1.f;

				m_bIgnoreDefence	= false;

#ifdef WIDE_BUFF_ANI_SPEED_UP
				m_bAnimationSpeedUp = false;
				m_fAnimationSpeedUpRate = 0.f;
#endif
#ifdef SKILL_BALANCE_20110728
				m_fPvpRate = 1.f;
#endif

#ifdef UPGRADE_RAVEN
				m_eDamageTrigger = DTT_NONE;
				m_bOnlyAttackBasic = false;
				m_bIgnoreStateDefence = false;
				m_bNoMiss = false;
#endif

#ifdef ELSWORD_WAY_OF_SWORD
				m_WayofSwordType	= WST_NONE;
#endif ELSWORD_WAY_OF_SWORD

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
				m_DPRateType		= DPRT_NONE;
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
				m_fDrainMpByReact = 0.f;
				m_fDrainMpRate = 1.f;
				m_bLvDamage = false;
#endif

#ifdef EVE_ELECTRA
				m_iLaserGroupID			= 0;
#endif EVE_ELECTRA

#ifdef ADD_BACKATTACK
				m_bApplyBackAttack		= true;
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
				m_fCreateDamageEffectRate	= 0.f;
#endif

#ifdef SERV_CHUNG_TACTICAL_TROOPER		/// û 2�� ���� ��Ƽ�� Ʈ���� - ��Ƽ�� �ʵ�
				m_fIncreaseMP				= 0.f;				/// ���� ���Ƿ�
#endif SERV_CHUNG_TACTICAL_TROOPER

#ifdef PLAY_EMOTION_BY_DAMAGE_EFFECT
				m_bIsLaughterDamage = false;
#endif // PLAY_EMOTION_BY_DAMAGE_EFFECT

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
				m_iHitAddForcePower = 0;
				m_eUsedSkillID		= CX2SkillTree::SI_NONE;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
                m_bRelaxNPCReactionStateCheck = false;
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

#ifdef ADD_MEMO_1ST_CLASS //��â��
				m_fApplyCriticalDamage = 0.f;
				fHitAddHPbyAttackPower = 0.f;
#endif //ADD_MEMO_1ST_CLASS

#ifdef ADD_RENA_SYSTEM //��â��
				m_eFirstAttack = FAC_NONE;
				m_RelateSkillData.Init();
#endif //ADD_RENA_SYSTEM
			}

			void Init()
			{
				SimpleInit();

				impactPoint		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
				defenderType	= AT_UNIT;
				optrDefenderGameUnit.Reset();
#ifndef X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				pDefenderEffect	= NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				
				m_SocketExtraDamage.Init();
				m_MultipleExtraDamage.resize(0);
			}

			void PushBuffFactor( CX2BuffFactorPtr ptrBuffFactor_, const bool bClearExistedFactor_ = true )
			{
				if ( bClearExistedFactor_ )
				{
					m_vecBuffFactorPtr.clear();
					PushSocketAndTemporaryBuffFactor( this );	/// ���Ͽɼ�, �������� �Ͻ������� ���� ���͵��� �ٽ� ����
				}

				if ( NULL != ptrBuffFactor_ )
				{
					CX2BuffFactorPtr ptrBuffFactorClone = ptrBuffFactor_->GetClonePtr();
					if ( NULL != ptrBuffFactorClone )
					{
						ptrBuffFactorClone->SetGameUnitBuffFactor( optrAttackerGameUnit.GetObservable() );
						m_vecBuffFactorPtr.push_back( ptrBuffFactorClone );
					}
				}
			}

			~DamageData()
			{
				hitUnitList.clear();
			}

			DamageData()
			{
				Init();
			}

			DamageData( const DamageData& rhs )
			{					
				*this = rhs;
			}

//{{ robobeg : 2013-09-17
            bool    IsAttackedByMyUnit();
//}} robobeg : 2013-09-17

		};

#ifdef ELSWORD_WAY_OF_SWORD
		struct WayOfSwordConstTable
		{
			float m_fSkillTypeConst_VigorCommand;
			float m_fSkillTypeConst_VigorActive;
			float m_fSkillTypeConst_VigorSpecialActive;
			float m_fSkillTypeConst_DestCommand;
			float m_fSkillTypeConst_DestActive;
			float m_fSkillTypeConst_DestSpecialActive;
			float m_fSkillTypeConst_DamageReact;

			float m_fStateConst_DestSkill_TowardVigorState;
			float m_fStateConst_VigorSkill_TowardVigorState;
			float m_fStateConst_DestSkill_TowrdDestState;
			float m_fStateConst_VigorSkill_TowrdDestState;
			float m_fStateConst_DestSkill_InVigorState;
			float m_fStateConst_VigorSkill_InVigorState;
			float m_fStateConst_DestSkill_InDestState;
			float m_fStateConst_VigorSkill_InDestState;
			float m_fStateConst_DestSkill_CenterState;
			float m_fStateConst_VigorSkill_CenterState;
			
			float m_fStateConst_DamageReact_TowardVigorState;
			float m_fStateConst_DamageReact_TowrdDestState;
			float m_fStateConst_DamageReact_InVigorState;
			float m_fStateConst_DamageReact_InDestState;
			float m_fStateConst_DamageReact_CenterState;


			float m_fGameModeConst_NPC_Hit;
			float m_fGameModeConst_PVP_Hit;

			float m_fConst_VigorStateThreshold;
			float m_fConst_DestStateThreshold;
			float m_fConst_DestRageWSPGain;

			// ���� ������ ����/���� ���� ���� ����
			float m_fConst_DestRageWSPGain_Elesis;

			WayOfSwordConstTable()
			{
				m_fSkillTypeConst_VigorCommand				= 0.f;
				m_fSkillTypeConst_VigorActive				= 0.f;
				m_fSkillTypeConst_VigorSpecialActive		= 0.f;
				m_fSkillTypeConst_DestCommand				= 0.f;
				m_fSkillTypeConst_DestActive				= 0.f;
				m_fSkillTypeConst_DestSpecialActive			= 0.f;
				m_fSkillTypeConst_DamageReact				= 0.f;

				m_fStateConst_DestSkill_TowardVigorState	= 0.f;
				m_fStateConst_VigorSkill_TowardVigorState	= 0.f;
				m_fStateConst_DestSkill_TowrdDestState		= 0.f;
				m_fStateConst_VigorSkill_TowrdDestState		= 0.f;
				m_fStateConst_DestSkill_InVigorState		= 0.f;
				m_fStateConst_VigorSkill_InVigorState		= 0.f;
				m_fStateConst_DestSkill_InDestState			= 0.f;
				m_fStateConst_VigorSkill_InDestState		= 0.f;
				m_fStateConst_DestSkill_CenterState			= 0.f;
				m_fStateConst_VigorSkill_CenterState		= 0.f;

				m_fStateConst_DamageReact_TowardVigorState	= 0.f;
				m_fStateConst_DamageReact_TowrdDestState	= 0.f;
				m_fStateConst_DamageReact_InVigorState		= 0.f;
				m_fStateConst_DamageReact_InDestState		= 0.f;
				m_fStateConst_DamageReact_CenterState		= 0.f;

				m_fGameModeConst_NPC_Hit					= 0.f;
				m_fGameModeConst_PVP_Hit					= 0.f;

				m_fConst_VigorStateThreshold				= 0.f;
				m_fConst_DestStateThreshold					= 0.f;
				m_fConst_DestRageWSPGain					= 0.f;

				// ���� ������ ����/���� ���� ���� ����
				m_fConst_DestRageWSPGain_Elesis				= 0.f;
			}
		};

#endif ELSWORD_WAY_OF_SWORD


	public:
		CX2DamageManager(void);
		~CX2DamageManager(void);

		static void PushSocketAndTemporaryBuffFactor( DamageData* pDamageData_ );	/// ���� �ɼ�, �Ͻ������� �ɸ� ������ ���� BuffFactor ����
		bool OpenDamageSoundScript( const WCHAR* wszFileName );
		bool ParseDamageSoundBlock( KLuaManager& luaManager, HIT_TYPE eHitType, const char* pszTableNameUTF8 );
		const WCHAR* GetDamageSound( HIT_TYPE hitType, HITTED_TYPE hittedType );		

		void ParsingBuffFactor( KLuaManager& luaManager_, DamageData* pDamageData_ );

#ifdef ADD_MEMO_1ST_CLASS
		bool SetDamageDataFromLUA( DamageData* pDamageData, KLuaManager& luaManager, const char* pTableName = "DAMAGE_DATA", float fPowerRate = 1.f, const bool IsEqippedMemo_ = false );
#else //ADD_MEMO_1ST_CLASS
		bool SetDamageDataFromLUA( DamageData* pDamageData, KLuaManager& luaManager, const char* pTableName = "DAMAGE_DATA", float fPowerRate = 1.f );
#endif //ADD_MEMO_1ST_CLASS

		bool DamageCheck( DamageData* pDamageData, bool bAttackOnlyThisUnit = false, UidType attackUnitUID = -1 );

		wstring GetExtraDamageName( CX2DamageManager::EXTRA_DAMAGE_TYPE extraDamageType, bool bTwoLine = false );

#ifdef SKILL_30_TEST
		void CX2DamageManager::SetExtraDamageData( DamageData* pDamageData, bool bApplyEffect,
			CX2DamageManager::EXTRA_DAMAGE_TYPE edtType, float fRate, float fTime, float fDamage, 
			float fExtraDamage = 0.f, int iLevel = -1, bool bIgnoreResist = false,
			float fDamageRate = 0.f, float fRunJumpRate = 0.f, float fAnimSpeedRate = 0.f );
#endif
		
#ifdef ELSWORD_WAY_OF_SWORD
		float	CalcWayOfSwordPoint( WAY_OF_SWORD_TYPE wayOfSwordType_, float fDamageRate_, int iWayOfSwordState );
		float	CalcWayOfSwordPointOnDamageReact( float fDamageRate_, int iWayOfSwordState );
		float	CalcVigorMpUpOnAttack( WAY_OF_SWORD_TYPE wayOfSwordType_, int iWayOfSwordState );
		float	CalcVigorMpUpOnDamageReact( int iWayOfSwordState );
		bool	OpenWayOfSwordScript( const WCHAR* wszFileName );
		float	GetVigorStateThreshold(){	return m_WayOfSwordConstTable.m_fConst_VigorStateThreshold; }
		float	GetDestStateThreshold(){	return m_WayOfSwordConstTable.m_fConst_DestStateThreshold;	}
		float	GetDestRageWSPGain();

		// �����ý� ���� ��
		float	GetDestRageWSPGain_Elesis();
		float	CalcVigorMpUpOnAttack_Elesis( WAY_OF_SWORD_TYPE wayOfSwordType_, int iWayOfSwordState );
#endif ELSWORD_WAY_OF_SWORD

#ifdef EVE_ELECTRA
		void	AddHitUnit( DamageData& kDamageData, HitUnit _HitUnit ){ kDamageData.hitUnitList.push_back(_HitUnit); }
#endif EVE_ELECTRA
		

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ
		bool	GetIsPossibleProvokeExtraDamage ( DamageData * pDamageData );
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ
	private:
		bool CollisionDataCheckFromUnitToUnit( CX2GameUnit* pAttackerGameUnit_, 
			const CKTDXCollision::CollisionDataListSet& setDefenderCollisionDataList_,
			CKTDXCollision::CollisionType& collisionType_, D3DXVECTOR3* pImpactPos_ );

		//bool CollisionDataCheckFromUnitToEffect( CX2GameUnit* pAttackerGameUnit_, 
		//	const CKTDXCollision::CollisionDataList& listDefenderCollisionData_,
		//	CKTDXCollision::CollisionType& collisionType_, D3DXVECTOR3* pImpactPos_ );

		bool UnitToUnit(DamageData* pDamageData, bool bAttackOnlyThisUnit = false, UidType attackUnitUID = -1 );	//������ ���� ����
		bool MeshToUnit(DamageData* pDamageData );  //�޽÷� ���� ����
		bool CollisionDataToUnit(DamageData* pDamageData );
		bool UnitToEffect( DamageData* pDamageData );
		bool EffectToUnit( DamageData* pDamageData );
		bool EffectToEffect( DamageData* pDamageData );
		
		float CalcHitDodgePercent( CX2GameUnit* pAttacker, CX2GameUnit* pDefender );

#ifdef GROUND_HIT
        bool GroundHit( DamageData* pDamageData, bool bAttackOnlyThisUnit, UidType attackUnitUID );
#endif
#ifdef AREA_HIT
		bool AreaHit( DamageData* pDamageData, bool bAttackOnlyThisUnit, UidType attackUnitUID );
#endif

#ifdef DAMAGE_EFFECT_GROUP_DAMAGE
		/// �׷� ������ Ȱ���� �˻�
		bool CheckActiveGroupDamage( const wstring& wstrDamageEffectName_, CX2DamageManager::DamageData* pDamageData_ );
		/// �׷� ������ ����
		void SetGroupDamage( const wstring& wstrDamageEffectName_, CX2DamageManager::DamageData* pDamageData_ );
#endif DAMAGE_EFFECT_GROUP_DAMAGE

#ifdef ADD_VELDER_ACADEMY_EVENT_MONSTER
		bool GetPossibleAttackOurTeamID( CX2GameUnit* pNPCUnit_ );	/// ���� ���� ���� �� �ִ� NPC���� �˻�
#endif ADD_VELDER_ACADEMY_EVENT_MONSTER


#ifdef DAMAGE_DATA_BUFF_FACTOR_RELATIVE_SKILL_LEVEL
		bool SetBuffFactorToDamageDataByBuffFactorID( OUT DamageData* pDamageData_, IN const UINT uiBuffFactorId_, IN const UINT uiLevel_ = 1 );

#ifdef ADD_MEMO_1ST_CLASS
		void ParsingBuffFactorID( KLuaManager& luaManager_, DamageData* pDamageData_, const bool IsEqippedMemo_ = false );
#else //ADD_MEMO_1ST_CLASS
		void ParsingBuffFactorID( KLuaManager& luaManager_, DamageData* pDamageData_ );
#endif //ADD_MEMO_1ST_CLASS

#endif // DAMAGE_DATA_BUFF_FACTOR_RELATIVE_SKILL_LEVEL

#ifdef ELSWORD_WAY_OF_SWORD
		WayOfSwordConstTable	m_WayOfSwordConstTable;
#endif ELSWORD_WAY_OF_SWORD

		CKTDXCollision*			m_pCollision;

		std::wstring			m_DamageSoundName[HIT_TYPE_COUNT][HITTED_TYPE_COUNT];
		vector<CKTDXDeviceSound*>	m_pvecReadySound;

};
