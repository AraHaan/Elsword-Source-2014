#pragma once

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ������
// ��ųID, ��ų������ ��ġ
typedef std::pair< CX2SkillTree::SKILL_ID, int> SkillLevelUpIDAndNum;
#endif // UPGRADE_SKILL_SYSTEM_2013

class CX2SocketItem
{
	public:
		enum SOCKET_DATA_TYPE
		{
			SDT_NONE			= 0,
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
			SDT_PVP_TAG_POINT,
			SDT_WEAPON_SPEED_2P5_FIXED,				/// ���� ȿ�� ������ 2.5% : ���� �ӵ� 
			SDT_DEFFENCE_SPEED_2P5_FIXED,			/// ���� ȿ�� ������ 2.5% : �� �ӵ� 
			SDT_WEAPON_SPEED_5P_FIXED,				/// ���� ȿ�� ������ 5% : ���� �ӵ� 
			SDT_DEFFENCE_SPEED_5P_FIXED,			/// ���� ȿ�� ������ 5% : �� �ӵ� 
			SDT_WEAPON_CRITICAL_2P5_FIXED,			/// ���� ȿ�� ������ 2.5% : ���� ũ��Ƽ�� 
			SDT_DEFFENCE_CRITICAL_2P5_FIXED,		/// ���� ȿ�� ������ 2.5% : �� ũ��Ƽ�� 
			SDT_WEAPON_CRITICAL_5P_FIXED,			/// ���� ȿ�� ������ 5% : ���� ũ��Ƽ�� 
			SDT_DEFFENCE_CRITICAL_5P_FIXED,			/// ���� ȿ�� ������ 5% : �� ũ��Ƽ�� 
			SDT_WEAPON_AVOID_2P5_FIXED,				/// ���� ȿ�� ������ 2.5% : ���� ȸ�� 
			SDT_DEFFECNE_AVOID_2P5_FIXED,			/// ���� ȿ�� ������ 2.5% : �� ȸ�� 
			SDT_WEAPON_AVOID_5P_FIXED,				/// ���� ȿ�� ������ 5% : ���� ȸ�� 
			SDT_DEFFENCE_AVOID_5P_FIXED,			/// ���� ȿ�� ������ 5% : �� ȸ��
			SDT_WEAPON_ADD_DAMAGE_2P5_FIXED,		/// ���� ȿ�� ������ 2.5% : ���� �߰� ������ 
			SDT_DEFFENCE_ADD_DAMAGE_2P5_FIXED,		/// ���� ȿ�� ������ 2.5% : �� �߰� ������ 
			SDT_WEAPON_ADD_DAMAGE_5P_FIXED,			/// ���� ȿ�� ������ 5% : ���� �߰� ������ 
			SDT_DEFFENCE_ADD_DAMAGE_5P_FIXED,		/// ���� ȿ�� ������ 5% : �� �߰� ������ 
			SDT_WEAPON_DAMAGE_REDUCE_2P5_FIXED,		/// ���� ȿ�� ������ 2.5% : ���� ������ ���� 
			SDT_DEFFECNE_DAMAGE_REDUCE_2P5_FIXED,	/// ���� ȿ�� ������ 2.5% : �� ������ ���� 
			SDT_WEAPON_DAMAGE_REDUCE_5P_FIXED,		/// ���� ȿ�� ������ 5% : ���� ������ ���� 
			SDT_DEFFENCE_DAMAGE_REDUCE_5P_FIXED,	/// ���� ȿ�� ������ 5% : �� ������ ���� 
			SDT_WEAPON_HYPER_CHARGE_5P_FIXED,		/// ���� ȿ�� ������ 5% : ���� ���� ���� 
			SDT_DEFFENCE_HYPER_CHARGE_5P_FIXED,		/// ���� ȿ�� ������ 5% : �� ���� ���� 
			SDT_WEAPON_HYPER_CHARGE_10P_FIXED,		/// ���� ȿ�� ������ 10% : ���� ���� ���� 
			SDT_DEFFENCE_HYPER_CHARGE_10P_FIXED,	/// ���� ȿ�� ������ 10% : �� ���� ����

			SDT_SKILL_LV_GROUP,						/// ������ ���� �ٸ� ��ų�� �ο��ϱ� ���� ��ų �׷�

//#endif SERV_2012_PVP_SEASON2
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
			//}}
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

		//{{ kimhc // 2010.3.31 // ��д��� �۾�(��Ʈ������ȿ��)
#ifdef SERV_SECRET_HELL
		enum SOCKET_ITEM_EFFECT_EX				// 2010�� 05�� ��� ��ġ ���ĺ��� �߰��� ���� ȿ�� ���� ENUM��
		{
			SIEE_NONE							= 0,	/// �߰�ȿ�� ����
			SIEE_STAT_UP,						/// �����̻� �ɸ��� X% Ȯ���� ��� ������ Y�ʰ� Z% ��� �Ǵ� ȿ��
			SIEE_UP_MANA_RECOVERY_VALUE,		/// �����̻� �ɸ��� ����ȸ���ӵ��� X�ʰ� Y�� �����Ѵ�.
			SIEE_CURE_DEBUFF,					/// �ǰݽ� x % Ȯ���� �ڽſ��� �ɸ� �����̻� y���� �����Ѵ�. (z�ʾȿ� �ٽ� �ߵ� ����)
			SIEE_DEBUFF_ATTACKER,				/// �ǰݽ� �ڽ��� Ÿ���� ����� X�ʰ� Y% Ȯ���� Z ���·θ����.
			SIEE_BONE_SHIELD,					/// �ǰݽ� x%Ȯ���� ���ظ� ����ϴ� �����и� ���� �Ͽ� Ÿ���� z%���(w�� ����� �����)				
			SIEE_ATTACK_SPEED_UP,				/// ���� ���� �� x% Ȯ���� �ִϸ��̼Ǽӵ�(���ݼӵ�)�� Y% ���� ������

			//{{ kimhc // 2010.12.29 // �ϸ� ���� �߰��� ���� ��Ʈ ������ ȿ��
			SIEE_ICE_NOVA,						/// �ǰݽ� ICE_NOVA ����Ʈ �� ����
			//}} kimhc // 2010.12.29 // �ϸ� ���� �߰��� ���� ��Ʈ ������ ȿ��
#ifdef ADD_SOCKET_FOR_SECRET_SET
			SIEE_POISON_BURST,					/// �ǰ� �� Poison ��Ʈ ȿ��
			SIEE_ABSORB_HP,						/// �ǰ� �� HP ��� ��Ʈ ȿ��
#endif
#ifdef HAMEL_SECRET_DUNGEON // ����ȯ
			SIEE_ABSORB_HP_NO_REACT,			/// �ǰ� �� HP ��� ��Ʈ ȿ�� ( ���� ���� )
			SIEE_ACTIVE_BUFF,					/// �ǰ� �� ���� ���� ȿ��
#endif // HAMEL_SECRET_DUNGEON

		};

		enum STAT_UP_TYPE	// �����̻� �ɸ��� % UP �� �� �ִ� ������ ����
		{
			SUT_NONE			= 0,
			SUT_PHYSIC_ATT,			// ����
			SUT_MAGIC_ATT,			// ����
			SUT_PHYSIC_DEF,			// ����
			SUT_MAGIC_DEF,			// ����
			SUT_BOTH_ATT,			// ����, ����
			SUT_BOTH_DEF,			// ����, ����
			SUT_ALL,				// HP�� ������ ��� ����
		};
#endif SERV_SECRET_HELL

#ifdef HAMEL_SECRET_DUNGEON // ����ȯ
		enum POSSIBLE_USE_PLACE			/// ��� ������ ��ġ
		{
			PUP_NONE							= -1,
			PUP_ALL								= 0,	/// ����
			PUP_DUNGEON_ONLY					= 1,	/// ���� ����
			PUP_PVP_ONLY						= 2,	/// ���� ����
			PUP_END								= 3,
		};

		enum HYPER_MODE_EFFECTIVE_TYPE	/// ������ ����� ȿ�� Ÿ��
		{
			HMET_NONE									= -1,
			HMET_USE_BUFF								= 0,	/// ���� ���
			HMET_ERASE_DEBUFF_AND_ADD_MP				= 1,	/// (define ADJUST_SECRET_ITEM_OPTION) ����� ����, ���� ������ ������� mp ȸ������ ���� <- ����� ���� �� ���� ������ MP ȸ��
			HMET_ADD_HP_IMMEDIATELY						= 2,	/// HP ��� ȸ��
			HMET_END									= 3,
		};

		struct InfoByUseHyperMode
		{
			InfoByUseHyperMode()
			{
				m_ePossibleUsePlace			= PUP_ALL;
				m_eHyperModeEffectiveType	= HMET_NONE;
				m_fEffectiveValue			= 0.f;
#ifdef ADJUST_SECRET_ITEM_OPTION //��â��
				m_fCoolTime					= 0.f;
#endif //ADJUST_SECRET_ITEM_OPTION
			}

			POSSIBLE_USE_PLACE				m_ePossibleUsePlace;		/// ��� ������ ����
			HYPER_MODE_EFFECTIVE_TYPE		m_eHyperModeEffectiveType;	/// ������ ����� ȿ�� Ÿ��
			CX2BuffFactorPtr				m_pBuffFactorPtr;			/// ����� ����
			float							m_fEffectiveValue;			/// ���� ��
#ifdef ADJUST_SECRET_ITEM_OPTION //��â��
			float							m_fCoolTime;				/// ���� �� ���� �����Ҷ������� �ð�
#endif //ADJUST_SECRET_ITEM_OPTION
		};
		typedef std::pair<POSSIBLE_USE_PLACE, CX2BuffFactorPtr> BuffInfoByUseHyperMode;
#endif // HAMEL_SECRET_DUNGEON

			//}} kimhc // 2010.3.31 // ��д��� �۾�(��Ʈ������ȿ��)
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		struct KItemStatRelLVData
		{
			// CX2SocketItem Ŭ�������� CX2ItemŬ������ ���� �� �� ��� �ۼ� �� ����ü
			float m_fBaseHPRelLV;
			float m_fAtkPhysicRelLV;
			float m_fAtkMagicRelLV;
			float m_fDefPhysicRelLV;
			float m_fDefMagicRelLV;

			KItemStatRelLVData()
			{
				Init();
			}

			inline bool IsValideCheck() const
			{
				return 0.f != (m_fBaseHPRelLV + m_fAtkPhysicRelLV + m_fAtkMagicRelLV + m_fDefPhysicRelLV + m_fDefMagicRelLV);
			}

			void AddStat( const KItemStatRelLVData& sStatRelLVData_)
			{
				this->m_fBaseHPRelLV += sStatRelLVData_.m_fBaseHPRelLV;
				this->m_fAtkPhysicRelLV += sStatRelLVData_.m_fAtkPhysicRelLV;
				this->m_fAtkMagicRelLV += sStatRelLVData_.m_fAtkMagicRelLV;
				this->m_fDefPhysicRelLV += sStatRelLVData_.m_fDefPhysicRelLV;
				this->m_fDefMagicRelLV += sStatRelLVData_.m_fDefMagicRelLV;
			}

			void Init()
			{
				m_fBaseHPRelLV = 0.f;
				m_fAtkPhysicRelLV = 0.f;
				m_fAtkMagicRelLV = 0.f;
				m_fDefPhysicRelLV = 0.f;
				m_fDefMagicRelLV = 0.f;
			}

			bool operator < ( const KItemStatRelLVData& rhs ) const 
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


#ifdef UPGRADE_SKILL_SYSTEM_2013 // ������
		struct SkillLvUpSocketOption
		{
			CX2Unit::UNIT_CLASS m_eUnitClass;
			CX2SkillTree::SKILL_ID m_eSkillID;
			int m_iSkillLvUPNum;

			SkillLvUpSocketOption( CX2Unit::UNIT_CLASS eUnitClass_, CX2SkillTree::SKILL_ID eSkillID_, int iSkillLvUPNum_ ):
			m_eUnitClass(eUnitClass_), m_eSkillID(eSkillID_), m_iSkillLvUPNum(iSkillLvUPNum_)
			{}

		};
#endif // UPGRADE_SKILL_SYSTEM_2013

#endif // SERV_NEW_ITEM_SYSTEM_2013_05

		struct SocketData
		{
		public:
//{{ kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
			//}} kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�

			static const float		MAX_HYPERMODETIME;
			static const float 		MAX_REPAIR_PRICE_DISCOUNT;
			static const float		MAX_SUPER_ARMOR_RATE;
			static const float		MAX_SUPER_ARMOR_TIME;
			static const float		MAX_DUBLE_ATTACK_RATE;
			
		public:
			int						m_ID;
			SOCKET_DATA_TYPE		m_Type;							//�ɼ�Ÿ��
			CX2Stat::Stat			m_Stat;							//�ɷ�ġ����
			float					m_fMoveSpeed;					//�޸���ӵ�����(%)(�ִ� 30%)
			float					m_fJumpSpeed;					//�����ӵ�����(%)(�ִ� 30%)

			float 					m_fAnimationSpeedRate;			//[0,100]Ÿ��, �ǰ�, �ǰ��� ȸ�� ���۽� �ִϸ��̼� �ӵ� ���

			float					m_fHyperModeChargeRate;			//�ҿ��������¡�ӵ� ����(%)(�ִ� 64.2%)
			float					m_fHyperModeTime;				//�����ð�����(40%)
			float					m_fRepairPriceSale;				//�����������񰨼�(%)(�ִ�90%)
			
			CX2BuffFactorPtr							m_ptrBuffFactor;
			//vector<CX2BuffFactorPtr>					m_vecBuffFactorPtr;
			CX2DamageManager::ExtraDamageData			m_SocketExtraDamage;				// �ߺ��ȵǴ� extra damage
			vector<CX2DamageManager::ExtraDamageData>	m_MultipleSocketExtraDamage;		// �ߺ� �Ǵ�, ��, ������ ���ÿ� ����� �� �ִ� extra damage
			
			
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 // ������
			// ����ȭ �����ۿ� ����, �������� ������ ������ ���� �ٸ� ��ų�� ���� �ֱ� ���� �׷� �߰�
			vector<int> m_vecSkillLevelUpGroupID;		//��ų�����׷� ID
			vector< CX2SocketItem::SkillLevelUpIDAndNum > m_vecSkillLevelUpID;		//��ų����
#else
			vector<CX2SkillTree::SKILL_ID> m_vecSkillLevelUpID;		//��ų����
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

			float					m_fDamageReduce;
			float					m_fPercentDamageReduce;

			std::wstring			m_DescEvadeByMonsterAttack;		// Ư�����Ϳ� ���� ������ ������ ������ ������ ��ũ��Ʈ���� ���� �ۼ��ϴ� ��쿡 ����
			set< int >				m_setMonsterIDDamageReduced;	//�ѽ����� �����δ�. //Ư�� ���� ����Ʈ 
			float					m_fDamageReduceByMonsterAttack; //Ư�� ���͵�� ���� ���ҵ� ������
			float					m_fPercentDRByMonsterAttack;	//Ư�����Ϳ��� ������ ���ҵ� Ȯ��

			float					m_fEvadePercent;				// [0,1] ȸ����

			float					m_fAntiEvadePercent;			// [0,1] ������


			float					m_fPercentCritical;				//ũ��Ƽ�� ������ Ȯ��
			float					m_fCritical;					//ũ��Ƽ�� ������ �� ���?


			//{{ kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
			float					m_fAdditionalDefenceValue;	/// �������� �������� ���� ������ ���� ��ġ (�� ��ġ�� ������ % ������ ��)
			float					m_fAdditionalAttackValue;	/// �������� �������� ���� �߰� ������
			//}} kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�

			vector<D3DXVECTOR2>		m_vecHpUpByAttack;				// ������ ���� �� �����ϴ� HP %, x: �ߵ�Ȯ��, y: �����ϴ� HP %
			vector<D3DXVECTOR2>		m_vecHpUpByAttacked;			// ������ ������ �� �����ϴ� HP % x: �ߵ�Ȯ��, y: �����ϴ� HP %

			//{{ kimhc // 2011-05-25 // SetOption, TitleOption���� ���� �Ǵ� option ��� itemOption�� ���� �ǵ��� ����
			vector<D3DXVECTOR2>		m_vecMpUpByAttack;				// ������ ���� �� �����ϴ� MP��, x: �ߵ�Ȯ��, y: �����ϴ� MP��

#ifdef ADJUST_SECRET_ITEM_OPTION //��â��
			vector<D3DXVECTOR4>		m_vecMpUpByAttacked;			// ������ ������ �� �����ϴ� MP�� x: �ߵ�Ȯ��, y: �����ϴ� MP��, z: ���� ��Ÿ��, w: ���� ��Ÿ��.
#else //ADJUST_SECRET_ITEM_OPTION
			vector<D3DXVECTOR2>		m_vecMpUpByAttacked;			// ������ ������ �� �����ϴ� MP�� x: �ߵ�Ȯ��, y: �����ϴ� MP��
#endif //ADJUST_SECRET_ITEM_OPTION

			//}} kimhc // 2011-05-25 // SetOption, TitleOption���� ���� �Ǵ� option ��� itemOption�� ���� �ǵ��� ����

			//{{ 2011.09.05 / ������ / �븸/ȫ�� / ������ ����� ���� / ���� : ������
#ifdef MAGIC_NEC_FIX
			float					m_fMPMultiplyByAll;				// ���� ȸ���� ����ġ( ���� ȿ�� )
#endif
			//}}

			float					m_fDamageUpByAMADS;				//m_fDamageUpByAttackMonsterAtDonwState �ٿ������ ���� ���ݽ� ������ ������, ����ΰ�

			CX2DamageManager::EXTRA_DAMAGE_TYPE		m_ResistExtraDamageType;		//Ư�� ������ ���� Ÿ��
			float									m_fResistExtraDamage;			//���ۼ�Ʈ ���ҽ�Ű�°�?

			CX2DamageManager::EXTRA_DAMAGE_TYPE		m_ImmuneExtraDamageType;		//Ư�������� ���� �̹� Ÿ��
			float									m_fPercentImmuneExtraDamage;	//���ۼ�Ʈ�� �̹� ����..


			int						m_AllSkillLevelUp;
	
			set<int>				m_setMonsterIDForDamageUp;
			float					m_fDamageUpPerAtSpecificMonster;	//Ư�� ���Ϳ��� ������ ����

			set<int>				m_setMonsterIDForEvade;
			float					m_fEvadePercentByMonsterAttack;

			float					m_fSpeedUpManaGather;

			float					m_fIncreaseEDPercent;
			float					m_fIncreaseDCBExpPercent;			// dungeonClearBonusExp

			bool					m_bDungeonOnly;
			bool					m_bPVPOnly;
			bool					m_bOverlap;

			float					m_fPercentSuperArmor;				// [0,1] �ǰݽ� ����Ȯ���� ���۾Ƹ�
			float					m_fTimeSuperArmor;					// ���۾Ƹ� �ð�

			int						m_aiResistEnchant[ CX2EnchantItem::ET_END ];
#ifdef PET_AURA_SKILL
			float					m_afAttackEnchant[ CX2EnchantItem::ET_END ];
#endif

//{{ kimhc // 2010.4.1 // ��д��� �۾�(��Ʈ������ȿ��)
#ifdef SERV_SECRET_HELL
			SOCKET_ITEM_EFFECT_EX	m_eTypeSocketItemEffectEx;	// ����� �߰� ���� ȿ��			
			STAT_UP_TYPE			m_eStatUpType;			// ���� ��ų ���� Ÿ��
			float					m_fDurationTime;		// ���ӽð�
			float					m_fProbability;			// �߻�Ȯ��
			float					m_fIncOrDecRate;		// ���� or ���� ��
			float					m_fIncOrDecVal;			// ���� or ���� ��			
			int						m_iCount;				// ���� (������ �����̻� or 5�� �ǰ� ���ϸ� ����� � �����)
			float					m_fIntervalTime;		// �߻��ɼ��ִ� �ð� ����(�ߵ��� �Ŀ� Inverval �ð� ��ŭ�� �ٽ� �ߵ� �� �� ����)
			vector<wstring>			m_vecEffectName;		// �ش� ȿ���� ���� ����Ʈ ��
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.1 // ��д��� �۾�(��Ʈ������ȿ��)

#ifdef SUPER_SOCKET_IN_TITLE
			float					m_fPercentSuperArmorDungeon;		/// ������ ��� �ְ� �� Ȯ��
			float					m_fTimeSuperArmorDungeon;			/// ������ ��� �ְ� �� �ð�
			float					m_fPercentSuperArmorPVP;			/// ������ ��� �ְ� �� Ȯ��
			float					m_fTimeSuperArmorPVP;				/// ������ ��� �ְ� �� �ð�
#endif

#ifdef BUFF_TEMPLET_SYSTEM 
			int						m_iAddMPValue;						// MP ������
			int						m_iMPChangeValue;					// MP �ڿ� ȸ����
			float					m_fHPRelativeChangeRate;			// 10�ʴ� HP ȸ��
			float					m_fSkillDamageUpRate;				// ��ų ���� ���� ��������
			float					m_fAllAttackEnchantRate;			// �Ӽ� �ߵ� Ȯ�� % ����
#endif BUFF_TEMPLET_SYSTEM

			int						m_iSummonNpcId;			// ��ȯ�� npc id
			float					m_fSummonNpcRate;		// Ÿ�ݽ� ��ȯȮ��
			float					m_fSummonNpcCoolTime;	// ���� ��Ÿ��

#ifdef PVP_SEASON2_SOCKET
			bool					m_bIsLevelLinkedStat;	/// ���� ���� ������ ����
			int						m_iAddLevel;			/// �߰� ���� ����
#endif
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			KItemStatRelLVData		m_kStatRelLVData;	/// ���� �� �߰� ����
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef SKILL_LEVEL_UP_BY_POWER_RATE_TYPE
			// POWER_RATE_TYPE ��� ���� ���ؼ� ���������ִ� ��ų ������ ���� �ɼ�
			int						m_iAllSkillLevelUpEx;
#endif // SKILL_LEVEL_UP_BY_POWER_RATE_TYPE

#ifdef HAMEL_SECRET_DUNGEON // ����ȯ
			CX2BuffFactorPtr			m_ptrBuffFactorCustomUse;		/// Ư���� �������� ���Ǵ� ���� ���� ���� �����̳�
																		/// ( Ÿ�ݽ� Ư�� ����� ����, �ǰݽ� Ư�� ���� ����, ������ Ư�� ���� ���� )
			HYPER_MODE_EFFECTIVE_TYPE	m_eHyperModeEffectiveType;		/// ������ ����ϴ� ȿ�� ������
			vector<InfoByUseHyperMode>	m_vecInfoByUseHyperMode;		/// ������ ����ϴ� ���� ���� ���� �����̳�
			float						m_fEffectiveValue;				/// ȿ�� ��
#endif // HAMEL_SECRET_DUNGEON

#ifdef SERV_GROW_UP_SOCKET
			map<int, vector<int>>	m_mapGrowUpSocketID;		// ���� ���� �׷�
#endif //SERV_GROW_UP_SOCKET

			SocketData()
			{
				m_ID								= 0;
				m_Type								= SDT_NONE;//zz
				m_fMoveSpeed						= 0.0f;
				m_fJumpSpeed						= 0.0f;

				m_fAnimationSpeedRate				= 0.0f;
				m_fHyperModeChargeRate				= 0.0f;
				m_fHyperModeTime					= 0.0f;
				m_fRepairPriceSale					= 0.0f;

				m_fDamageReduce						= 0.0f;
				m_fPercentDamageReduce				= 0.0f;

				m_DescEvadeByMonsterAttack			= L"";//zz
				m_fDamageReduceByMonsterAttack		= 0.0f;//zz
				m_fPercentDRByMonsterAttack			= 0.0f;//zz

				m_fEvadePercent						= 0.0f;

				m_fAntiEvadePercent					= 0.0f;



				m_fPercentCritical					= 0.0f;	
				m_fCritical							= 0.0f;		

				//{{ kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
				m_fAdditionalDefenceValue	= 0.0f;	// �������� �������� ���� ������ ���� ��ġ (�� ��ġ�� ������ % ������ ��)
				m_fAdditionalAttackValue	= 0.0f;	// �������� �������� ���� �߰� ������
				//}} kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�

		//{{ kimhc // 2011-05-25 // SetOption, TitleOption���� ���� �Ǵ� option ��� itemOption�� ���� �ǵ��� ����
		//}} kimhc // 2011-05-25 // SetOption, TitleOption���� ���� �Ǵ� option ��� itemOption�� ���� �ǵ��� ����

				//{{ 2011.09.05 / ������ / �븸/ȫ�� / ������ ����� ���� / ���� : ������
#ifdef MAGIC_NEC_FIX
				m_fMPMultiplyByAll					= 0.0f;
#endif
				//}}
				

				m_fDamageUpByAMADS					= 0.0f;			

				m_ResistExtraDamageType				= CX2DamageManager::EDT_NONE;//zz
				m_fResistExtraDamage				= 0.0f;//zz

				m_ImmuneExtraDamageType				= CX2DamageManager::EDT_NONE;//zz			
				m_fPercentImmuneExtraDamage			= 0.0f;		//zz

				m_AllSkillLevelUp					= 0;
				m_fDamageUpPerAtSpecificMonster		= 0.0f;//zz
				
				m_fEvadePercentByMonsterAttack		= 0.0f;//zz


				m_fSpeedUpManaGather				= 0.0f;

				m_fIncreaseEDPercent				= 0.0f;
				m_fIncreaseDCBExpPercent			= 0.0f;

				m_bDungeonOnly						= false;//zz
				m_bPVPOnly							= false;
				m_bOverlap							= false;//zz

				m_fPercentSuperArmor				= 0.0f;
				m_fTimeSuperArmor					= 0.0f;

				for( int i=0; i< (int)CX2EnchantItem::ET_END ; i++ )
				{
					m_aiResistEnchant[i] = 0;
#ifdef PET_AURA_SKILL
					m_afAttackEnchant[i] = 0.f;
#endif
				}

				//{{ kimhc // 2010.4.1 // ��д��� �۾�(��Ʈ������ȿ��)
#ifdef SERV_SECRET_HELL
				m_eTypeSocketItemEffectEx			= SIEE_NONE;	// ����� �߰� ��Ʈ������ ȿ��
				m_eStatUpType				= SUT_NONE;	// ���� ��ų ���� Ÿ��
				m_fDurationTime						= 0.0f;			// ���ӽð�
				m_fProbability						= 0.0f;			// �߻�Ȯ��
				m_fIncOrDecRate						= 0.0f;			// ���� or ���� ��
				m_fIncOrDecVal						= 0.0f;			// ���� or ���� ��			
				m_iCount							= 0;			// ���� (������ �����̻� or 5�� �ǰ� ���ϸ� ����� � �����)
				m_fIntervalTime						= 0.0f;			// �߻��ɼ��ִ� �ð� ����(�ߵ��� �Ŀ� Inverval �ð� ��ŭ�� �ٽ� �ߵ� �� �� ����)
#endif SERV_SECRET_HELL
				//}} kimhc // 2010.4.1 // ��д��� �۾�(��Ʈ������ȿ��)

#ifdef SUPER_SOCKET_IN_TITLE
				m_fPercentSuperArmorDungeon			= 0.0f;
				m_fTimeSuperArmorDungeon			= 0.0f;
				m_fPercentSuperArmorPVP				= 0.0f;
				m_fTimeSuperArmorPVP				= 0.0f;
#endif

#ifdef BUFF_TEMPLET_SYSTEM
				m_iAddMPValue						= 0;			// MP ������
				m_iMPChangeValue					= 0;			// MP �ڿ� ȸ����
				m_fHPRelativeChangeRate				= 0.0f;			// 10�ʴ� HP ȸ��
				m_fSkillDamageUpRate				= 0.0f;			// ��ų ���� ���� ��������
				m_fAllAttackEnchantRate				= 1.0f;			// �Ӽ� �ߵ� Ȯ�� % ����
#endif BUFF_TEMPLET_SYSTEM

				m_iSummonNpcId						= 0;			// ��ȯ�� npc id
				m_fSummonNpcRate					= 0.f;		// Ÿ�ݽ� ��ȯȮ��
				m_fSummonNpcCoolTime				= 0.f;

#ifdef PVP_SEASON2_SOCKET
				m_bIsLevelLinkedStat	= false;
				m_iAddLevel				= 0;
#endif

#ifdef SKILL_LEVEL_UP_BY_POWER_RATE_TYPE
				m_iAllSkillLevelUpEx = 0;
#endif // SKILL_LEVEL_UP_BY_POWER_RATE_TYPE

#ifdef HAMEL_SECRET_DUNGEON // ����ȯ
				m_eHyperModeEffectiveType	= HMET_NONE;
				m_fEffectiveValue			= 0.f;
#endif // HAMEL_SECRET_DUNGEON
			}

			//{{ // kimhc // �ǽð� ���ҵ� �� �ǽð� �ƹ�Ÿ �� ��� ��ü �� ���� �ɼ�
#ifdef REAL_TIME_ELSWORD
			void ReInit()
			{
				m_Stat.Init();

				m_fMoveSpeed				= 0.0f;
				m_fJumpSpeed				= 0.0f;

				m_fAnimationSpeedRate		= 0.0f;
				m_fHyperModeChargeRate		= 0.0f;
				m_fHyperModeTime			= 0.0f;
				m_fRepairPriceSale			= 0.0f;

				m_fDamageReduce				= 0.0f;
				m_fPercentDamageReduce		= 0.0f;

				m_fEvadePercent				= 0.0f;

				m_fAntiEvadePercent			= 0.0f;

				m_fPercentCritical			= 0.0f;
				m_fCritical					= 0.0f;

				//{{ kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
				m_fAdditionalDefenceValue	= 0.0f;	// �������� �������� ���� ������ ���� ��ġ (�� ��ġ�� ������ % ������ ��)
				m_fAdditionalAttackValue	= 0.0f;	// �������� �������� ���� �߰� ������
				//}} kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�

				//{{ kimhc // 2011-05-25 // SetOption, TitleOption���� ���� �Ǵ� option ��� itemOption�� ���� �ǵ��� ����
				m_vecHpUpByAttack.resize( 0  );
				m_vecHpUpByAttacked.resize( 0 );

				m_vecMpUpByAttack.resize( 0 );				// ������ ���� �� �����ϴ� MP��, x: �ߵ�Ȯ��, y: �����ϴ� MP��
				m_vecMpUpByAttacked.resize( 0 );			// ������ ������ �� �����ϴ� MP�� x: �ߵ�Ȯ��, y: �����ϴ� MP��
				//}} kimhc // 2011-05-25 // SetOption, TitleOption���� ���� �Ǵ� option ��� itemOption�� ���� �ǵ��� ����

				//{{ 2011.09.05 / ������ / �븸/ȫ�� / ������ ����� ���� / ���� : ������
#ifdef MAGIC_NEC_FIX
				m_fMPMultiplyByAll			= 0.0f;
#endif
				//}}

				m_fDamageUpByAMADS			= 0.0f;

				m_fSpeedUpManaGather		= 0.0f;
				m_fIncreaseEDPercent		= 0.0f;
				m_fIncreaseDCBExpPercent	= 0.0f;

				m_fPercentSuperArmor		= 0.0f;
				m_fTimeSuperArmor			= 0.0f;

				m_AllSkillLevelUp			= 0;

				m_vecSkillLevelUpID.resize( 0 );

				m_SocketExtraDamage.Init();
				m_MultipleSocketExtraDamage.resize( 0 );

				for( int i=0; i< (int)CX2EnchantItem::ET_END ; i++ )
				{
					m_aiResistEnchant[i] = 0;
#ifdef PET_AURA_SKILL
					m_afAttackEnchant[i] = 0.f;
#endif
				}

				//{{ kimhc // 2010.4.1 // ��д��� �۾�(��Ʈ������ȿ��)
#ifdef SERV_SECRET_HELL
				m_eTypeSocketItemEffectEx			= SIEE_NONE;	// ����� �߰� ��Ʈ������ ȿ��
				m_eStatUpType				= SUT_NONE;	// ���� ��ų ���� Ÿ��
				m_fDurationTime						= 0.0f;			// ���ӽð�
				m_fProbability						= 0.0f;			// �߻�Ȯ��
				m_fIncOrDecRate						= 0.0f;			// ���� or ���� ��
				m_fIncOrDecVal						= 0.0f;			// ���� or ���� ��			
				m_iCount							= 0;			// ���� (������ �����̻� or 5�� �ǰ� ���ϸ� ����� � �����)
				m_fIntervalTime						= 0.0f;			// �߻��ɼ��ִ� �ð� ����(�ߵ��� �Ŀ� Inverval �ð� ��ŭ�� �ٽ� �ߵ� �� �� ����)
				m_vecEffectName.resize(0);
#endif SERV_SECRET_HELL
				//}} kimhc // 2010.4.1 // ��д��� �۾�(��Ʈ������ȿ��)

#ifdef SUPER_SOCKET_IN_TITLE
				m_fPercentSuperArmorDungeon			= 0.0f;
				m_fTimeSuperArmorDungeon			= 0.0f;
				m_fPercentSuperArmorPVP				= 0.0f;
				m_fTimeSuperArmorPVP				= 0.0f;
#endif

#ifdef BUFF_TEMPLET_SYSTEM
				m_iAddMPValue						= 0;			// MP ������
				m_iMPChangeValue					= 0;			// MP �ڿ� ȸ����
				m_fHPRelativeChangeRate				= 0.0f;			// 10�ʴ� HP ȸ��
				m_fSkillDamageUpRate				= 0.0f;			// ��ų ���� ���� ��������
				m_fAllAttackEnchantRate				= 1.0f;			// �Ӽ� �ߵ� Ȯ�� % ����
#endif BUFF_TEMPLET_SYSTEM

				m_iSummonNpcId						= 0;			// ��ȯ�� npc id
				m_fSummonNpcRate					= 0.f;		// Ÿ�ݽ� ��ȯȮ��
				m_fSummonNpcCoolTime				= 0.f;

#ifdef PVP_SEASON2_SOCKET
				m_bIsLevelLinkedStat	= false;
				m_iAddLevel				= 0;
#endif

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				m_kStatRelLVData.Init();	/// ���� �� �߰� ����
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef SKILL_LEVEL_UP_BY_POWER_RATE_TYPE
				m_iAllSkillLevelUpEx = 0;
#endif // SKILL_LEVEL_UP_BY_POWER_RATE_TYPE

#ifdef HAMEL_SECRET_DUNGEON // ����ȯ
				m_vecInfoByUseHyperMode.clear();	/// ������ ����ϴ� ȿ�� ���� ���� �����̳�
#endif // HAMEL_SECRET_DUNGEON
			}
#endif REAL_TIME_ELSWORD
			//}} // kimhc // �ǽð� ���ҵ� �� �ǽð� �ƹ�Ÿ �� ��� ��ü �� ���� �ɼ�

			void Add( const SocketData& rhs )
			{
				m_Stat.AddStat( rhs.m_Stat, true );			// socket option�� stat�� extra_stat ������ default�� �����ؼ� ����ϵ��� �Ѵ�.


				m_fMoveSpeed += rhs.m_fMoveSpeed;
				m_fJumpSpeed += rhs.m_fJumpSpeed;

				m_fAnimationSpeedRate += rhs.m_fAnimationSpeedRate;
				m_fHyperModeChargeRate += rhs.m_fHyperModeChargeRate;
				m_fHyperModeTime += rhs.m_fHyperModeTime;
				m_fRepairPriceSale += rhs.m_fRepairPriceSale;
				
			
				m_fDamageReduce += rhs.m_fDamageReduce;
				m_fPercentDamageReduce += rhs.m_fPercentDamageReduce;

				m_fEvadePercent += rhs.m_fEvadePercent;

				m_fAntiEvadePercent += rhs.m_fAntiEvadePercent;


				m_fPercentCritical += rhs.m_fPercentCritical;
				m_fCritical += rhs.m_fCritical;

				//{{ kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
				m_fAdditionalDefenceValue = rhs.m_fAdditionalDefenceValue;	// �������� �������� ���� ������ ���� ��ġ (�� ��ġ�� ������ % ������ ��)
				m_fAdditionalDefenceValue = rhs.m_fAdditionalAttackValue;	// �������� �������� ���� �߰� ������
				// �������� �������� ���� ������ ���� ��ġ (�� ��ġ�� ������ % ������ ��)
				//}} kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�

				if ( false == rhs.m_vecHpUpByAttack.empty() )
					m_vecHpUpByAttack.insert( m_vecHpUpByAttack.end(), rhs.m_vecHpUpByAttack.begin(), rhs.m_vecHpUpByAttack.end() );

				if ( false == rhs.m_vecHpUpByAttacked.empty() )
					m_vecHpUpByAttacked.insert( m_vecHpUpByAttacked.end(), rhs.m_vecHpUpByAttacked.begin(), rhs.m_vecHpUpByAttacked.end() );

				//{{ kimhc // 2011-05-25 // SetOption, TitleOption���� ���� �Ǵ� option ��� itemOption�� ���� �ǵ��� ����
				if ( false == rhs.m_vecMpUpByAttack.empty() )
					m_vecMpUpByAttack.insert( m_vecMpUpByAttack.end(), rhs.m_vecMpUpByAttack.begin(), rhs.m_vecMpUpByAttack.end() );

				if ( false == rhs.m_vecMpUpByAttacked.empty() )
					m_vecMpUpByAttacked.insert( m_vecMpUpByAttacked.end(), rhs.m_vecMpUpByAttacked.begin(), rhs.m_vecMpUpByAttacked.end() );
				//}} kimhc // 2011-05-25 // SetOption, TitleOption���� ���� �Ǵ� option ��� itemOption�� ���� �ǵ��� ����

				//{{ 2011.09.05 / ������ / �븸/ȫ�� / ������ ����� ���� / ���� : ������
#ifdef MAGIC_NEC_FIX
				m_fMPMultiplyByAll += rhs.m_fMPMultiplyByAll;
#endif
				//}}

#ifdef BUFF_TEMPLET_SYSTEM
				m_iAddMPValue					+= rhs.m_iAddMPValue;					// MP ������
				m_iMPChangeValue				+= rhs.m_iMPChangeValue;				// MP �ڿ� ȸ����
				m_fHPRelativeChangeRate			+= rhs.m_fHPRelativeChangeRate;			// 10�ʴ� HP ȸ��
				m_fSkillDamageUpRate			+= rhs.m_fSkillDamageUpRate;			// ��ų ���� ���� ��������
				m_fAllAttackEnchantRate			*= rhs.m_fAllAttackEnchantRate;			// �Ӽ� �ߵ� Ȯ�� % ����
#endif BUFF_TEMPLET_SYSTEM

				m_fDamageUpByAMADS += rhs.m_fDamageUpByAMADS;

				m_fSpeedUpManaGather += rhs.m_fSpeedUpManaGather;
				m_fIncreaseEDPercent += rhs.m_fIncreaseEDPercent;
				m_fIncreaseDCBExpPercent += rhs.m_fIncreaseDCBExpPercent;

				m_fPercentSuperArmor += rhs.m_fPercentSuperArmor;
				m_fTimeSuperArmor += rhs.m_fTimeSuperArmor;
					
				m_AllSkillLevelUp += rhs.m_AllSkillLevelUp;

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				m_kStatRelLVData.AddStat( rhs.m_kStatRelLVData );	/// ���� �� �߰� ����
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef SKILL_LEVEL_UP_BY_POWER_RATE_TYPE
				// POWER_RATE_TYPE ��� ���� ���ؼ� ���������ִ� ��ų ������ ���� �ɼ�
				m_iAllSkillLevelUpEx += rhs.m_iAllSkillLevelUpEx;
#endif // SKILL_LEVEL_UP_BY_POWER_RATE_TYPE
			}

			//{{ kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 // ������
			wstring GetSocketDesc( const int iSocketLevel_, bool bCompact = false, CX2Unit::UNIT_CLASS eUnitClass_ = CX2Unit::UC_NONE ) const;
	#else
			wstring GetSocketDesc( const int iSocketLevel_, bool bCompact = false ) const;
	#endif // SERV_NEW_ITEM_SYSTEM_2013_05
			//}} kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�

//{{ kimhc // 2010.4.9 // ��д��� �۾�(��Ʈ������ȿ��)
#ifdef SERV_SECRET_HELL
			wstring GetSocketDescEx() const;			// SOCKET_ITEM_EFFECT_EX�� NONE�� �ƴ� Ÿ���� ��ũ����
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.9 // ��д��� �۾�(��Ʈ������ȿ��)

			// @bExcludeInternalLimit: UI�� ǥ�õǴ� �κп��� ������ ������ ���������� ������ �ִ� ��쿡, ���Ѽ�ġ ����� ���� �ʱ� ����
			void LimitMaximum( bool bExcludeInternalLimit = false )
			{
				m_Stat.LimitMaximum();

//{{ kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
				//}} kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�

				// �ٸ� ��ġ�� ������ SocketOptionForm.lua���� ����
				if( m_fHyperModeTime > CX2SocketItem::SocketData::MAX_HYPERMODETIME )
					m_fHyperModeTime = CX2SocketItem::SocketData::MAX_HYPERMODETIME;				

				if( m_fPercentSuperArmor > CX2SocketItem::SocketData::MAX_SUPER_ARMOR_RATE )
					m_fPercentSuperArmor = CX2SocketItem::SocketData::MAX_SUPER_ARMOR_RATE;
				
				if( m_fTimeSuperArmor > CX2SocketItem::SocketData::MAX_SUPER_ARMOR_TIME )
					m_fTimeSuperArmor = CX2SocketItem::SocketData::MAX_SUPER_ARMOR_TIME;

			}

			//{{ kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
			// ������ ��ȯ �� �ݿø��� ���ؼ� 0.5f�� �����ش�.
			int GetLinearCriticalValue( const int iSocketLevel_ ) const
			{
				return static_cast<int>( m_fPercentCritical * lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateLinearCriticalValue", iSocketLevel_ ) + 0.5f );
			}

			int GetLinearMoveSpeedValue( const int iSocketLevel_ ) const
			{
				return static_cast<int>( m_fMoveSpeed * lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateLinearMoveSpeedValue", iSocketLevel_ ) + 0.5f );
			}
	
			int GetLinearJumpSpeedValue( const int iSocketLevel_ ) const
			{
				return static_cast<int>( m_fJumpSpeed * lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateLinearJumpSpeedValue", iSocketLevel_ ) + 0.5f );
			}

			int GetLinearAnimationSpeedValue( const int iSocketLevel_ ) const
			{
				return static_cast<int>( m_fAnimationSpeedRate * lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateLinearAnimationSpeedValue", iSocketLevel_ ) + 0.5f );
			}
		
			int GetLinearAntiEvadeValue( const int iSocketLevel_ ) const
			{
				return static_cast<int>( m_fAntiEvadePercent * lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateLinearAntiEvadeValue", iSocketLevel_ ) + 0.5f );
			}

			int GetLinearEvadeValue( const int iSocketLevel_ ) const
			{
				return static_cast<int>( m_fEvadePercent * lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateLinearEvadeValue", iSocketLevel_ ) + 0.5f );
			}
			
			int GetHpIncrementValue( const int iSocketLevel_ ) const
			{
				return lua_tinker::call<int>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateHpIncrementValue", iSocketLevel_ );
			}

			int GetLinearHyperGageChargeSpeedValue( const int iSocketLevel_ ) const
			{
				return static_cast<int>( m_fHyperModeChargeRate * lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateLinearHyperGageChargeSpeedValue", iSocketLevel_ ) + 0.5f );
			}

			int GetLinearAdditionalDefenceValue( const int iSocketLevel_ ) const
			{
				return static_cast<int>( m_fAdditionalDefenceValue * lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateLinearAdditionalDefenceValue", iSocketLevel_ ) + 0.5f );
			}

			int GetLinearAdditionalAttackValue( const int iSocketLevel_ ) const
			{
				return static_cast<int>( m_fAdditionalAttackValue * lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateLinearAdditionalAttackValue", iSocketLevel_ ) + 0.5f );
			}

	#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
			void ConvertAndAdd( const SocketData& rhs_, const int iSocketLevel_, const CX2Unit::UNIT_TYPE eUnitType_ )
	#else // SERV_9TH_NEW_CHARACTER
			void ConvertAndAdd( const SocketData& rhs_, const int iSocketLevel_ )
	#endif // SERV_9TH_NEW_CHARACTER
			{

				// 2011-07-07 ��������� Stat�߿��� m_fIncreaseHPRate �� �ɼ� ��ġȭ�� ������ �޴´�
				m_Stat.ConvertAndAddStat( rhs_.m_Stat, GetHpIncrementValue( iSocketLevel_ ) );			// socket option�� stat�� extra_stat ������ default�� �����ؼ� ����ϵ��� �Ѵ�.


				m_fMoveSpeed += rhs_.GetLinearMoveSpeedValue( iSocketLevel_ );
				m_fJumpSpeed += rhs_.GetLinearJumpSpeedValue( iSocketLevel_ );

				m_fAnimationSpeedRate += rhs_.GetLinearAnimationSpeedValue( iSocketLevel_ );


	#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
				/// �ֵ��� ��, ���ݸ� ��������
				const float fReformValue = ( CX2Unit::UT_ADD == eUnitType_ ) ? 0.5f : 1.f;

				m_fHyperModeChargeRate	+= ( rhs_.GetLinearHyperGageChargeSpeedValue( iSocketLevel_ ) * fReformValue );
				m_fHyperModeTime		+= rhs_.m_fHyperModeTime * fReformValue;
	#else // SERV_9TH_NEW_CHARACTER
				m_fHyperModeChargeRate += rhs_.GetLinearHyperGageChargeSpeedValue( iSocketLevel_ );
				m_fHyperModeTime += rhs_.m_fHyperModeTime;
	#endif // SERV_9TH_NEW_CHARACTER
				
				m_fRepairPriceSale += rhs_.m_fRepairPriceSale;
				
			
				m_fDamageReduce += rhs_.m_fDamageReduce;
				m_fPercentDamageReduce += rhs_.m_fPercentDamageReduce;

				m_fEvadePercent += rhs_.GetLinearEvadeValue( iSocketLevel_ );

				m_fAntiEvadePercent += rhs_.GetLinearAntiEvadeValue( iSocketLevel_ );


				m_fPercentCritical += rhs_.GetLinearCriticalValue( iSocketLevel_ );
				m_fCritical += rhs_.m_fCritical;

				m_fAdditionalDefenceValue += rhs_.GetLinearAdditionalDefenceValue( iSocketLevel_ );
				m_fAdditionalAttackValue += rhs_.GetLinearAdditionalAttackValue( iSocketLevel_ );

				if ( false == rhs_.m_vecHpUpByAttack.empty() )
					m_vecHpUpByAttack.insert( m_vecHpUpByAttack.end(), rhs_.m_vecHpUpByAttack.begin(), rhs_.m_vecHpUpByAttack.end() );

				if ( false == rhs_.m_vecHpUpByAttacked.empty() )
					m_vecHpUpByAttacked.insert( m_vecHpUpByAttacked.end(), rhs_.m_vecHpUpByAttacked.begin(), rhs_.m_vecHpUpByAttacked.end() );

				//{{ kimhc // 2011-05-25 // SetOption, TitleOption���� ���� �Ǵ� option ��� itemOption�� ���� �ǵ��� ����
				if ( false == rhs_.m_vecMpUpByAttack.empty() )
					m_vecMpUpByAttack.insert( m_vecMpUpByAttack.end(), rhs_.m_vecMpUpByAttack.begin(), rhs_.m_vecMpUpByAttack.end() );

				if ( false == rhs_.m_vecMpUpByAttacked.empty() )
					m_vecMpUpByAttacked.insert( m_vecMpUpByAttacked.end(), rhs_.m_vecMpUpByAttacked.begin(), rhs_.m_vecMpUpByAttacked.end() );
				//}} kimhc // 2011-05-25 // SetOption, TitleOption���� ���� �Ǵ� option ��� itemOption�� ���� �ǵ��� ����

				m_fDamageUpByAMADS += rhs_.m_fDamageUpByAMADS;

				m_fSpeedUpManaGather += rhs_.m_fSpeedUpManaGather;
				m_fIncreaseEDPercent += rhs_.m_fIncreaseEDPercent;
				m_fIncreaseDCBExpPercent += rhs_.m_fIncreaseDCBExpPercent;

#ifdef SUPER_SOCKET_IN_TITLE
				if ( true == rhs_.m_bDungeonOnly )
				{
					m_fPercentSuperArmorDungeon = max( m_fPercentSuperArmorDungeon, rhs_.m_fPercentSuperArmor );
					m_fTimeSuperArmorDungeon = max( m_fTimeSuperArmorDungeon, rhs_.m_fTimeSuperArmor );
				}
				else if ( true == rhs_.m_bPVPOnly )
				{
					m_fPercentSuperArmorPVP = max( m_fPercentSuperArmorPVP, rhs_.m_fPercentSuperArmor );
					m_fTimeSuperArmorPVP = max( m_fTimeSuperArmorPVP, rhs_.m_fTimeSuperArmor );
				}
				else
				{
					m_fPercentSuperArmorDungeon = max( m_fPercentSuperArmorDungeon, rhs_.m_fPercentSuperArmor );
					m_fTimeSuperArmorDungeon = max( m_fTimeSuperArmorDungeon, rhs_.m_fTimeSuperArmor );

					m_fPercentSuperArmorPVP = max( m_fPercentSuperArmorPVP, rhs_.m_fPercentSuperArmor );
	m_fTimeSuperArmorPVP = max( m_fTimeSuperArmorPVP, rhs_.m_fTimeSuperArmor );
				}
				
#else
				m_fPercentSuperArmor += rhs_.m_fPercentSuperArmor;
				m_fTimeSuperArmor += rhs_.m_fTimeSuperArmor;
#endif
					
				m_AllSkillLevelUp += rhs_.m_AllSkillLevelUp;

				//{{ 2011.09.05 / ������ / �븸/ȫ�� / ������ ����� ���� / ���� : ������
#ifdef MAGIC_NEC_FIX
				m_fMPMultiplyByAll += rhs_.m_fMPMultiplyByAll;
#endif
				//}}
#ifdef BUFF_TEMPLET_SYSTEM
				m_iAddMPValue					+= rhs_.m_iAddMPValue;					// MP ������
				m_iMPChangeValue				+= rhs_.m_iMPChangeValue;				// MP �ڿ� ȸ����
				m_fHPRelativeChangeRate			+= rhs_.m_fHPRelativeChangeRate;		// 10�ʴ� HP ȸ��
				m_fSkillDamageUpRate			+= rhs_.m_fSkillDamageUpRate;			// ��ų ���� ���� ��������
				m_fAllAttackEnchantRate			*= rhs_.m_fAllAttackEnchantRate;		// �Ӽ� �ߵ� Ȯ�� % ����
#endif BUFF_TEMPLET_SYSTEM

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				m_kStatRelLVData.AddStat( rhs_.m_kStatRelLVData );	/// ���� �� �߰� ����
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef SKILL_LEVEL_UP_BY_POWER_RATE_TYPE
				// POWER_RATE_TYPE ��� ���� ���ؼ� ���������ִ� ��ų ������ ���� �ɼ�
				m_iAllSkillLevelUpEx += rhs_.m_iAllSkillLevelUpEx;
#endif // SKILL_LEVEL_UP_BY_POWER_RATE_TYPE
			}
			//}} kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
#ifdef VIEW_SPECIAL_OPTIONDATA	
			/** @function : LevelLinkedStatAdd
				@brief : �⺻������ �ɼǼ�ġȭ ����
				@param : �����ų ���� ��ü(rhs_), ���Ϸ���(iSocketLevel_)
			*/
			void LevelLinkedStatAdd( const SocketData& rhs_, const int iSocketLevel_ )
			{
				m_Stat.m_fBaseHP += rhs_.m_Stat.m_fBaseHP * iSocketLevel_;
				m_Stat.m_fAtkMagic += rhs_.m_Stat.m_fAtkMagic * iSocketLevel_;
				m_Stat.m_fAtkPhysic += rhs_.m_Stat.m_fAtkPhysic * iSocketLevel_;
				m_Stat.m_fDefMagic += rhs_.m_Stat.m_fDefMagic * iSocketLevel_;
				m_Stat.m_fDefPhysic += rhs_.m_Stat.m_fDefPhysic * iSocketLevel_;
			}
#endif //VIEW_SPECIAL_OPTIONDATA
		};
	public:
		CX2SocketItem(void);
		~CX2SocketItem(void);

		//{{ kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�
		void			OpenScriptFile( const WCHAR* pFileName, const WCHAR* pFormulaFileName_ );

		static float GetFinalCriticalPercent( const float fSumValue_, const int iUserLevel_ )
		{
			return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateFinalCriticalPercent", fSumValue_, iUserLevel_ );
		}

		static float GetFinalMoveSpeedPercent( const float fSumValue_, const int iUserLevel_ )
		{
			return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateFinalMoveSpeedPercent", fSumValue_, iUserLevel_ );
		}

		static float GetFinalJumpSpeedPercent( const float fSumValue_, const int iUserLevel_ )
		{
			return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateFinalJumpSpeedPercent", fSumValue_, iUserLevel_ );
		}

		static float GetFinalAnimationSpeedPercent( const float fSumValue_, const int iUserLevel_ )
		{
			return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateFinalAnimationSpeedPercent", fSumValue_, iUserLevel_ );
		}

		static float GetFinalAnitEvadePercent( const float fSumValue_, const int iUserLevel_ )
		{
			return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateFinalAnitEvadePercent", fSumValue_, iUserLevel_ );
		}

		static float GetFinalEvadePercent( const float fSumValue_, const int iUserLevel_ )
		{
			return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateFinalEvadePercent", fSumValue_, iUserLevel_ );
		}
		
		static float GetFinalHyperGageChargeSpeedPercent( const float fSumValue_, const int iUserLevel_ )
		{
			return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateFinalHyperGageChargeSpeedPercent", fSumValue_, iUserLevel_ );
		}
		
		static float GetFinalAdditionalDefencePercent( const float fSumValue_, const int iUserLevel_ )
		{
			return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateFinalAdditionalDefencePercent", fSumValue_, iUserLevel_ );
		}

		static float GetFinalAdditionalAttackValue( const float fSumValue_, const int iUserLevel_ )
		{
			// fPower_ �� �� ������Ʈ�� ���ݹ���*�Ŀ�����Ʈ �̴�.
			return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateFinalAdditionalAttackValue", fSumValue_, iUserLevel_ );
		}

		// 1���� ���� ���� ��
		static float GetFinalAdditionalAttackValueInDamageFreeGame( const float fSumValue_, const int iUserLevel_ )
		{
			// fPower_ �� �� ������Ʈ�� ���ݹ���*�Ŀ�����Ʈ �̴�.
			return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateFinalAdditionalAttackValueInDamageFreeGame", fSumValue_, iUserLevel_ );
		}
#ifdef ELSWORD_WAY_OF_SWORD 
		// �ɼ� ������
		static float GetOptionRateCorrection( const float fOptionRate_ );
#endif ELSWORD_WAY_OF_SWORD
		//}} kimhc // 2011-07-05 // �ɼǵ���Ÿ ��ġȭ �۾�

		const SocketData*   GetSocketData( int key ) const;
		
		bool			AddSocketData_LUA();
#ifdef LUA_TRANS_DEVIDE
		bool			AddSocketDataTrans_LUA();
#endif LUA_TRANS_DEVIDE

#ifdef SERV_CASH_ITEM_SOCKET_OPTION
		void			AddSocketGroupDataForCashAvatar_LUA();		/// ĳ�þƹ�Ÿ���� ������ �� �ִ� �ɼ� �׷� �Ľ�
		int				GetSocketIdListForCashAvatar( const int iGroupID_, OUT vector<int>& vecSocketGroupData_, const int iItemType_ );
#endif // SERV_CASH_ITEM_SOCKET_OPTION

		bool			GetIsPossibleSocketItemByOnlyItemType( int itemID );

		//{{ �ֹ�ö [2013/1/4]  ���ӳ� ���� ��Ʈ���� �������Ϸ� ���
#ifdef PRINT_INGAMEINFO_TO_EXCEL
		void PrintOptionInfo_ToExcel(KProtectedType<int> iLevel);
#endif PRINT_INGAMEINFO_TO_EXCEL
		//}} �ֹ�ö [2013/1/4]  ���ӳ� ���� ��Ʈ���� �������Ϸ� ���

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		int	 GetSocketPrefixStrID( int iSocketID_ ) const;
		void GetSocketPrefix( IN int iSocketID_, OUT wstring& wstrItemDesc_ ) const;
#ifdef EU_NEW_ITEM_SYSTEM_PREFIX_TO_POSTFIX
		void GetSocketPostfix( IN int iSocketID_, OUT wstring& wstrItemDesc_ ) const;
#endif // EU_NEW_ITEM_SYSTEM_PREFIX_TO_POSTFIX
		void AddSkillLevelUpSocketGroup_LUA( int iGroupID_, CX2Unit::UNIT_CLASS eUnicClass_, int iSkillID_
			#ifdef UPGRADE_SKILL_SYSTEM_2013// ������
			, int iSkillLvUP_ 
			#endif // UPGRADE_SKILL_SYSTEM_2013
			);

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ������ 
		bool GetSkillIDByUnicClassAndGropID( OUT vector<SkillLevelUpIDAndNum>& vecSkillLevelUPIDAndNumList_, IN CX2Unit::UNIT_CLASS eUnitClass_, IN int iGroupID_ ) const;

		void GetSkillLevelIncreaseValue( IN const SocketData* pSocketData, IN const CX2SkillTree::SKILL_ID eSkillID, OUT int& iIncreaseSkillLevelBySocket, IN CX2Unit::UNIT_CLASS eUnitClass_) const;
#else
		CX2SkillTree::SKILL_ID GetSkillIDByUnicClassAndGropID( CX2Unit::UNIT_CLASS eUnitClass_, int iGroupID_ ) const;
#endif //UPGRADE_SKILL_SYSTEM_2013
		const CX2SocketItem::KItemStatRelLVData& GetStatRelLVDataBySocktID( int iSocketID_ ) const;

		static KItemStatRelLVData	ms_KDummyItemStatRelLVData;
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef SERV_GROW_UP_SOCKET
		bool			SetGrowUpSocketData( IN const wstring& wstrGrowUpSocketID_, OUT map<int, vector<int>>& mapGrowUpSocketID );
		// ���� ���� ���� ��������
		bool			GetGrowUpSocketData(IN const vector<int>& vecOldSocketOption_, OUT vector<int>& vecNewSocketOption_ );
#endif //SERV_GROW_UP_SOCKET

	private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
        typedef std::map<int,SocketData>    SocketDataMap;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
        typedef std::map<int,SocketData*>   SocketDataMap;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
		SocketDataMap                       m_mapSocketOptionPool;

#ifdef SERV_CASH_ITEM_SOCKET_OPTION
		typedef std::map<int, vector<int>> MapSocketGroupDataForCashAvartar;
		MapSocketGroupDataForCashAvartar m_mapSocketGroupDataForCashAvartar;		/// ĳ�þƹ�Ÿ���� ������ �� �ִ� ���Ͽɼ� �׷�
#endif // SERV_CASH_ITEM_SOCKET_OPTION

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		// ���� ID �� ���ξ� String ID // map<SocketID , StringID >
		std::map< int, int >			m_mapSocketDescStrID;

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ������
		// �׷�ID, < ����Ŭ����, ��ųID, ��ų������ ��ġ>
		typedef std::map< int, std::vector< SkillLvUpSocketOption >> mapUnitClassSkillGroup;
		mapUnitClassSkillGroup m_mapSkillUpSocketGroup;
#else
		// <����Ŭ����, vcetor< �׷�ID, ��ųID > //  
		typedef	std::pair<int, CX2SkillTree::SKILL_ID> pairSkillGroup;
		typedef std::map< CX2Unit::UNIT_CLASS, std::vector< pairSkillGroup >> mapUnitClassSkillGroup;
		mapUnitClassSkillGroup m_mapSkillUpSocketGroup;
#endif //UPGRADE_SKILL_SYSTEM_2013 

#endif //SERV_NEW_ITEM_SYSTEM_2013_05
};

