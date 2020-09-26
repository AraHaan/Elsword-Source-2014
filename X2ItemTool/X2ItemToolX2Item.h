#pragma once

#define MAX_SPECIAL_ABILITY_NUM	 3
#define MAX_ENCHANT_CARD_NUM	10



class CX2Stat
{
public:
	enum ATTRIBUTE
	{
		ATT_NONE,
		ATT_PHYSIC,
		ATT_FIRE,
		ATT_ICE,
		ATT_LIGHTNING,
		ATT_EARTH,
		ATT_LIGHT,
		ATT_DARK,
		ATT_UNIVERSAL,
	};

public:
	struct Stat
	{
		float	m_fBaseHP;

		//����
		float	m_fAtkPhysic;
		float	m_fAtkMagic;

		float	m_fAtkFire;
		float	m_fAtkIce;
		float	m_fAtkLightning;
		float	m_fAtkEarth;
		float	m_fAtkLight;
		float	m_fAtkDark;
		float	m_fAtkUniversal;

		//���
		float	m_fDefPhysic;
		float	m_fDefMagic;

		float	m_fDefFire;
		float	m_fDefIce;
		float	m_fDefLightning;
		float	m_fDefEarth;
		float	m_fDefLight;
		float	m_fDefDark;
		float	m_fDefUniversal;

		Stat()
		{
			Init();
		}
		void Init()
		{
			m_fBaseHP			= 0.0f;

			m_fAtkPhysic		= 0.0f;
			m_fAtkMagic			= 0.0f;

			m_fAtkFire			= 0.0f;
			m_fAtkIce			= 0.0f;
			m_fAtkLightning		= 0.0f;
			m_fAtkEarth			= 0.0f;
			m_fAtkLight			= 0.0f;
			m_fAtkDark			= 0.0f;
			m_fAtkUniversal		= 0.0f;

			m_fDefPhysic		= 0.0f;
			m_fDefMagic			= 0.0f;

			m_fDefFire			= 0.0f;
			m_fDefIce			= 0.0f;
			m_fDefLightning		= 0.0f;
			m_fDefEarth			= 0.0f;
			m_fDefLight			= 0.0f;
			m_fDefDark			= 0.0f;
			m_fDefUniversal		= 0.0f;
		}
		void AddStat( const Stat& baseStat )
		{
			m_fBaseHP			+= baseStat.m_fBaseHP;

			m_fAtkPhysic		+= baseStat.m_fAtkPhysic;
			m_fAtkMagic			+= baseStat.m_fAtkMagic;

			m_fAtkFire			+= baseStat.m_fAtkFire;
			m_fAtkIce			+= baseStat.m_fAtkIce;
			m_fAtkLightning		+= baseStat.m_fAtkLightning;
			m_fAtkEarth			+= baseStat.m_fAtkEarth;
			m_fAtkLight			+= baseStat.m_fAtkLight;
			m_fAtkDark			+= baseStat.m_fAtkDark;
			m_fAtkUniversal		+= baseStat.m_fAtkUniversal;

			m_fDefPhysic		+= baseStat.m_fDefPhysic;
			m_fDefMagic			+= baseStat.m_fDefMagic;

			m_fDefFire			+= baseStat.m_fDefFire;
			m_fDefIce			+= baseStat.m_fDefIce;
			m_fDefLightning		+= baseStat.m_fDefLightning;
			m_fDefEarth			+= baseStat.m_fDefEarth;
			m_fDefLight			+= baseStat.m_fDefLight;
			m_fDefDark			+= baseStat.m_fDefDark;
			m_fDefUniversal		+= baseStat.m_fDefUniversal;
		}

		void Verify();
	};

public:
	CX2Stat(void) {};
	~CX2Stat(void) {};

	void	InitStat(){ m_Stat.Init(); }
	Stat*	GetStat(){ return &m_Stat; }
	void	AddStat( const Stat& baseStat ){ m_Stat.AddStat( baseStat ); }

private:
	Stat		m_Stat;
};


class CX2Item;
class CX2Inventory;
class CX2Eqip;
class CX2Unit
{
public:
	enum UNIT_TYPE
	{
		UT_NONE	= 0,
		UT_ELSWORD,
		UT_ARME,
		UT_LIRE,
		UT_RAVEN,
		UT_END,			//  �����ý��� �߰��Ǹ� END�� �����ý� �Ʒ���..
		UT_ELISIS,
	};

	enum UNIT_CLASS
	{
		//�ʱ�����
		UC_NONE = 0,
		UC_ELSWORD_SWORDMAN,
		UC_ARME_VIOLET_MAGE,
		UC_LIRE_ELVEN_RANGER,
		UC_RAVEN_FIGHTER,
		UC_ELISIS_KNIGHT,

		//��������
		UC_ELSWORD_KNIGHT = 10,
		UC_ELSWORD_MAGIC_KNIGHT,
		UC_LIRE_COMBAT_RANGER,
		UC_LIRE_SNIPING_RANGER,
		UC_ARME_HIGH_MAGICIAN,
		UC_ARME_DARK_MAGICIAN,


		//�ӽ�
		UC_ELSWORD_SWORD_EXPERT,
		UC_ELSWORD_SQUERD_LEADER,

		UC_ELISIS_ARCANA_KNIGHT,
		UC_ELISIS_CENTURION_KNIGHT,

		UC_ELISIS_KNIGHT_MASTER,
		UC_ELISIS_LEGION_COMMENDER,

		UC_ARME_ARCH_MAGE,
		UC_ARME_ARCHIMIST,
		UC_ARME_ARCANE_MASTER,
	};

	enum WEAPON_TYPE
	{
		WT_NONE = 0,
		WT_GREATE_SWORD,
		WT_LONG_BOW,
		WT_WAND,
	};

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
		EP_AC_FACE1,				//��
		EP_AC_FACE2,			//��
		EP_AC_FACE3,				//��
		EP_AC_BODY,				//����
		EP_AC_ARM,				//��
		EP_AC_LEG,				//�ٸ�
		EP_AC_RING,				//����
		EP_AC_NECKLESS,			//�����
		EP_AC_TEMP1,			//�ӽ�1
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

		EP_END,
	};


	enum EQIP_SLOT_ID
	{
		ESI_NONE		= 0,
		ESI_QUICK_SLOT,		//������ ������

		ESI_WEAPON_HAND,			//����

		ESI_DEFENCE_BODY,		//����
		ESI_DEFENCE_LEG,			//����
		ESI_DEFENCE_HAND,		//�尩
		ESI_DEFENCE_FOOT,		//�Ź�

		ESI_DEFENCE_HAIR_FASHION,		//��Ÿ��

		ESI_DEFENCE_BODY_FASHION,		//����
		ESI_DEFENCE_LEG_FASHION,			//����
		ESI_DEFENCE_HAND_FASHION,		//�尩
		ESI_DEFENCE_FOOT_FASHION,		//�Ź�

		ESI_SKILL_1,				//1�ܰ� �ʻ��
		ESI_SKILL_2,				//2�ܰ� �ʻ��
		ESI_SKILL_3,				//3�ܰ� �ʻ��

		ESI_END,
	};

	struct UnitTemplet
	{
		UNIT_TYPE			m_UnitType;
		UNIT_CLASS			m_UnitClass;

		wstring				m_Name;
		wstring				m_Description;

		wstring				m_GameMotionFile;
		wstring				m_UIMotionFile;

		wstring				m_GameAniXETName;
		wstring				m_UIAniXETName;

		wstring				m_BasicAniXETName;
		wstring				m_BasicChangeTexXETName;

		CX2Stat::Stat		m_Stat;

		int					m_BasicWeaponItemID;
		int					m_BasicHairItemID;
		int					m_BasicFaceItemID;
		int					m_BasicBodyItemID;
		int					m_BasicLegItemID;
		int					m_BasicHandItemID;
		int					m_BasicFootItemID;
	};

	struct UnitData
	{
		UidType					m_UnitUID;
		UidType					m_UserUID;
		UNIT_CLASS				m_UnitClass;

		wstring				    m_IP;
		int						m_Port;

		wstring					m_NickName;

		int		m_GP;
		int		m_VSPoint;
		int		m_Level;
		int		m_EXP;

		int						m_Win;
		int						m_Lose;
		int						m_Seceder;

		int		m_NowBaseLevelEXP;
		int		m_NextBaseLevelEXP;

		int		m_EquipInventorySize;		
		int		m_SkillInventorySize;		
		int		m_MaterialInventorySize;	
		int		m_SpecialInventorySize;	
		int		m_CardInventorySize;		
		int		m_QuickSlotInventorySize;	
		int		m_AvartaInventorySize;	

		int		m_QuickSlotSize;

		CX2Stat::Stat 			m_Stat;
		int		m_RemainStatPoint;

		vector<UidType>			m_NowEqipItemUIDList;
		CX2Inventory*			m_pInventory;

		int		m_nStraightVictories;
		int		m_nMapID;

		//UnitData& operator=( const KUnitInfo& data );

		//UnitData( CX2Unit* pOwnerUnit, const KUnitInfo& data );
		~UnitData();

		void Verify();
	};

public:
	CX2Unit() {} ;
	virtual ~CX2Unit(void) {};


};


class CX2Item
{
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
			IT_CARD,			//ī��
			IT_OUTLAY,			//�����Һ�
			IT_ETC,				//��Ÿ]
			IT_END,
		};

		enum EQIP_TYPE
		{
			ET_NONE = 0,		//��� �ƴ�
			ET_SKIN,			//��Ų �ִϸ��̼�
			ET_ATTACH_ANIM,		//����ġ �ִϸ��̼�
			ET_ATTACH_NORMAL,	//����ġ �޽�
		};

		enum PERIOD_TYPE
		{
			PT_INFINITY = 0,	//������
			PT_ENDURANCE,		//������(�Ⱓ��)
			PT_QUANTITY,		//������
		};

		enum SHOP_PRICE_TYPE
		{
			SPT_NONE = 0,		//�ȱ� �Ҵ�
			SPT_CASH,			//ĳ��
			SPT_GP,				//GP
		};

		enum EQIP_CONDITION
		{
			EC_NONE = 0,		//��� �ƴ�
			EC_EVERYONE,		//��δ�
			EC_ONE_UNIT,		//�� ���ָ�
			EC_ONE_CLASS,		//�� Ŭ������
		};

		enum SPECIAL_ABILITY_TYPE
		{
			SAT_NONE = 0,		//����
			SAT_HP_UP,			//HP����
			SAT_MP_UP,			//MP����
			SAT_REMOVE_CURSE,	//��������
			SAT_REMOVE_SLOW,	//���ο�����
			SAT_REMOVE_FIRE,	//ȭ������
			SAT_REMOVE_POISON,	//������
			SAT_HYPER_MODE,			//����
			SAT_SOUL_GAGE_UP,		//���������� ���
			SAT_END,
		};

		struct SpecialAbility
		{
			SPECIAL_ABILITY_TYPE		m_Type;
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

		struct ItemTemplet
		{
			int							m_ItemID;					//������ ID
			std::wstring				m_Name;						//������ �̸�
			std::wstring				m_Description;				//������ ����

			std::wstring				m_ModelName;				//������ ���� _Hyper �� ���δ�
			std::wstring				m_TextureChangeXETName;		//������ ���� _Hyper �� ���δ�
			std::wstring				m_AniXETName;				//������ ���� _Hyper �� ���δ�
			std::wstring				m_AniName;					//������ ���� _Hyper �� ���δ�
			std::wstring				m_ShopImage;
			std::wstring				m_DropViewer;				//��ӽ� ����� ��ƼŬ �ý��� �̸�

			ITEM_TYPE					m_ItemType;
			bool						m_bFashion;					//�м� ������
			bool						m_bVested;					//�ͼ� ������(�絵�Ҵ�/�ǸźҴ�)
			bool						m_bRare;					//����������ΰ�?
			bool						m_bCanEnchant;				//��ȭ���� �������ΰ�

			EQIP_TYPE					m_EqipType;
			std::wstring				m_AttachFrameName;			//��� ���� �ٴ°�
			bool						m_bCanHyperMode;			//������ ��� ���� �������ΰ�
			
			PERIOD_TYPE					m_PeriodType;				//������ ����Ⱓ
			int							m_Endurance;				//�ʱ⳻����
			int							m_Quantity;					//1��������
			
			bool						m_bSale;					//�Ǹ��ϴ°�?
			SHOP_PRICE_TYPE				m_PriceType;				//����Ÿ��
			int							m_Price;					//����
			int							m_PricePvPPoint;			//���Ŵ�������Ʈ

			bool						m_bUseInUI;					//ui â���� ��� ��ư�� �ٴ°�?

			EQIP_CONDITION				m_EqipConditon;				//���� ����
			CX2Unit::UNIT_TYPE			m_UnitType;					//���� ����
			CX2Unit::UNIT_CLASS			m_UnitClass;				//���� Ŭ����
			int							m_EqipLevel;				//���� ����

			CX2Unit::EQIP_POSITION		m_EqipPosition;				//���� ��ġ
			CX2Stat::Stat				m_Stat;						//������ ����

			std::vector<SpecialAbility>	m_SpecialAbilityList;

			ItemTemplet()
			{
				m_ItemID			= -1;				//������ ID

				m_ItemType			= IT_NONE;
				m_bFashion			= false;			//�м� ������
				m_bVested			= false;			//�ͼ� ������(�絵�Ҵ�/�ǸźҴ�)
				m_bRare				= false;			//����������ΰ�?
				m_bCanEnchant		= false;			//��ȭ���� �������ΰ�

				m_EqipType			= ET_NONE;
				m_bCanHyperMode		= false;			//������ ��� ���� �������ΰ�

				m_PeriodType		= PT_INFINITY;		//������ ����Ⱓ
				m_Endurance			= 0;				//�ʱ⳻����
				m_Quantity			= 0;				//1��������

				m_bSale				= false;			//�Ǹ��ϴ°�?
				m_PriceType			= SPT_NONE;			//����Ÿ��
				m_Price				= 0;				//����
				m_PricePvPPoint		= 0;				//���Ŵ�������Ʈ

				m_bUseInUI			= false;			//ui â���� ��� ��ư�� �ٴ°�?

				m_EqipConditon		= EC_NONE;			//���� ����
				m_UnitType			= CX2Unit::UT_NONE;	//���� ����
				m_UnitClass			= CX2Unit::UC_NONE;	//���� Ŭ����
				m_EqipLevel			= 0;				//���� ����

				m_EqipPosition		= CX2Unit::EP_NONE;	//���� ��ġ
			}
		};

		struct ItemData
		{
			UidType				m_ItemUID;				//UID
			int					m_ItemID;

            PERIOD_TYPE         m_PeriodType;
			int					m_Endurance;			//������
			int					m_Quantity;				//����

			int					m_EnchantAttribute;
			int					m_EnchantLevel;

			ItemData()
			{
				m_ItemUID				= 0;				//UID
				m_ItemID				= 0;

                m_PeriodType            = PT_INFINITY;
				m_Endurance				= 0;				//������
				m_Quantity				= 0;				//����

				m_EnchantAttribute		= 0;
				m_EnchantLevel			= 0;
			}
			//ItemData& operator=( const KItemInfo& data )
			//{
			//	m_ItemUID				= data.m_iItemUID;					//UID
			//	m_ItemID				= data.m_iItemID;

   //             m_PeriodType            = (CX2Item::PERIOD_TYPE)data.m_iUsageType;
			//	m_Endurance				= data.m_iEndurance;				//������
			//	m_Quantity				= data.m_iQuantity;					//����

			//	m_EnchantAttribute		= data.m_iEType;
			//	m_EnchantLevel			= data.m_iELevel;

			//	return *this;
			//}

			//void MakePacket( KItemInfo& itemInfo )
			//{
			//	itemInfo.m_iItemUID				= m_ItemUID;				//UID
			//	itemInfo.m_iItemID				= m_ItemID;

   //             itemInfo.m_iUsageType           = m_PeriodType;
			//	itemInfo.m_iEndurance			= m_Endurance;			//������
			//	itemInfo.m_iQuantity			= m_Quantity;			//����

			//	itemInfo.m_iEType				= m_EnchantAttribute;
			//	itemInfo.m_iELevel				= m_EnchantLevel;
			//}
		};

	public:
		CX2Item( ItemData* pItemData );
		~CX2Item(void);

		ItemData*		GetItemData()			{ return m_pItemData; }
		ItemTemplet*	GetItemTemplet()		{ return m_pItemTemplet; }

		UidType			GetUID()				{ return m_pItemData->m_ItemUID; }
		void			SetEqip( bool bEqip )	{ m_bEqip = bEqip; }
		bool			GetEqip()				{ return m_bEqip; }

	private:
		ItemData*		m_pItemData;
		ItemTemplet*	m_pItemTemplet;

		bool			m_bEqip;
};
