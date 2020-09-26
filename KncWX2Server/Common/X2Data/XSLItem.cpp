#include ".\xslitem.h"
#include ".\XSLUnit.h"
//{{ 2011. 06. 01	������	��ȭ�� ������ ���� ť��
#ifdef SERV_ENCHANTED_ITEM_CUBE_REWARD
	#include ".\XSLAttribEnchantItem.h"
#endif SERV_ENCHANTED_ITEM_CUBE_REWARD
//}}

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	#include "Enum/Enum.h"
#endif SERV_RELATIONSHIP_SYSTEM
//}

//{{ 2012. 05. 23	��μ�	[�̺�Ʈ] ������ũ�� �����մϴ�!
//#ifdef SERV_EVENT_APINK
	#include ".\xslitemmanager.h"
//#endif SERV_EVENT_APINK
//}}

#include "XSLSquareUnit.h"

CXSLItem::CXSLItem( ItemData* pItemData )
{
	m_pItemData		= pItemData;
	m_bEqip			= false;
}

CXSLItem::~CXSLItem(void)
{
	SAFE_DELETE( m_pItemData );
}

//{{ 2009. 7. 28  ������	GM����
bool CXSLItem::IsGMweapon( const int iItemID )
{
	switch( iItemID )
	{
	case 128015: // ���ҵ�GM����
	case 128016: // ���ҵ�GM����
	case 128017: // ����GM����
	case 128018: // ����GM����
	case 128019: // ���̻�GM����
	case 128020: // ���̻�GM����
	case 128078: // ���̺�GM����
	case 128079: // ���̺�GM����
	case 28005: // �̺� GM����
	case 28006: // �̺� GM����
	case 183248: // �����ý�GM����
	case 183249: // �����ý�GM����
		return true;
	}

	return false;
}
//}}

//{{ 2009. 9. 22  ������	����ĳ��
char CXSLItem::GetCashItemChangeUnitClass( const int iItemID )
{
	switch( iItemID )
	{
	case CI_CHANGE_JOB_ELSWORD_KNIGHT:	return CXSLUnit::UC_ELSWORD_KNIGHT;
	case CI_CHANGE_JOB_MAGIC_KNIGHT:	return CXSLUnit::UC_ELSWORD_MAGIC_KNIGHT;
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
	case CI_CHANGE_JOB_SHEATH_KNIGHT:	return CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT;
#endif SERV_ELSWORD_SHEATH_KNIGHT
		//{{ 2009. 10. 24  ������	���ҵ�2������
	case CI_CHANGE_JOB_LORD_KNIGHT:		return CXSLUnit::UC_ELSWORD_LORD_KNIGHT;
	case CI_CHANGE_JOB_RUNE_SLAYER:		return CXSLUnit::UC_ELSWORD_RUNE_SLAYER;
		//}}
	case CI_CHANGE_JOB_COMBAT_RANGER:	return CXSLUnit::UC_LIRE_COMBAT_RANGER;
	case CI_CHANGE_JOB_SNIPING_RANGER:	return CXSLUnit::UC_LIRE_SNIPING_RANGER;
#ifdef SERV_TRAPPING_RANGER_TEST
	case CI_CHANGE_JOB_TRAPPING_RANGER:	return CXSLUnit::UC_LIRE_TRAPPING_RANGER;
#endif SERV_TRAPPING_RANGER_TEST
		//{{ 2009. 11. 23  ������	����2������
	case CI_CHANGE_JOB_WIND_SNEAKER:	return CXSLUnit::UC_LIRE_WIND_SNEAKER;
	case CI_CHANGE_JOB_GRAND_ARCHER:	return CXSLUnit::UC_LIRE_GRAND_ARCHER;
		//}}
	case CI_CHANGE_JOB_HIGH_MAGICIAN:	return CXSLUnit::UC_ARME_HIGH_MAGICIAN;
	case CI_CHANGE_JOB_DARK_MAGICIAN:	return CXSLUnit::UC_ARME_DARK_MAGICIAN;
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CI_CHANGE_JOB_BATTLE_MAGICIAN:	return CXSLUnit::UC_ARME_BATTLE_MAGICIAN;
#endif SERV_ADD_ARME_BATTLE_MAGICIAN
		//{{ 2009. 12. 11  ������	���̻� 2�� ����
	case CI_CHANGE_JOB_ELMENTAL_MASTER: return CXSLUnit::UC_ARME_ELEMENTAL_MASTER;
	case CI_CHANGE_JOB_VOID_PRINCESS:	return CXSLUnit::UC_ARME_VOID_PRINCESS;
		//}}
	case CI_CHANGE_JOB_SOUL_TAKER:		return CXSLUnit::UC_RAVEN_SOUL_TAKER;
	case CI_CHANGE_JOB_OVER_TAKER:		return CXSLUnit::UC_RAVEN_OVER_TAKER;
#ifdef SERV_RAVEN_WEAPON_TAKER
	case CI_CHANGE_JOB_WEAPON_TAKER:	return CXSLUnit::UC_RAVEN_WEAPON_TAKER;
#endif SERV_RAVEN_WEAPON_TAKER
		//{{ 2010. 1. 20  �տ���	���̺� 2�� ����
	case CI_CHANGE_JOB_BLADE_MASTER:	return CXSLUnit::UC_RAVEN_BLADE_MASTER;
	case CI_CHANGE_JOB_RECKLESS_FIST:	return CXSLUnit::UC_RAVEN_RECKLESS_FIST;
		//}}	
	case CI_CHANGE_JOB_EXOTIC_GEAR:		return CXSLUnit::UC_EVE_EXOTIC_GEAR;
	case CI_CHANGE_JOB_ARCHITECTURE:	return CXSLUnit::UC_EVE_ARCHITECTURE;
#ifdef SERV_EVE_ELECTRA
	case CI_CHANGE_JOB_EVE_ELECTRA:		return CXSLUnit::UC_EVE_ELECTRA;
#endif SERV_EVE_ELECTRA
		//{{ 2010. 1. 20  �տ���	�̺� 2�� ����
	case CI_CHANGE_JOB_NEMESIS:			return CXSLUnit::UC_EVE_CODE_NEMESIS;
	case CI_CHANGE_JOB_EMPRESS:			return CXSLUnit::UC_EVE_CODE_EMPRESS;
		//}}
		//{{ 2011. 01. 26	������	û 1�� ����
	case CI_CHANGE_JOB_FURY_GUARDIAN:	return CXSLUnit::UC_CHUNG_FURY_GUARDIAN;
	case CI_CHANGE_JOB_SHOOTER_GUARDIAN:return CXSLUnit::UC_CHUNG_SHOOTING_GUARDIAN;
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CI_CHANGE_JOB_SHELLING_GUARDIAN:	return CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN;
#endif
		//}}
		//{{ JHKang / ������ / 2011.6.13 / û 2�� ����
	case CI_CHANGE_JOB_IRON_PALADIN:	return CXSLUnit::UC_CHUNG_IRON_PALADIN;
	case CI_CHANGE_JOB_DEADLY_CHASER:	return CXSLUnit::UC_CHUNG_DEADLY_CHASER;
		//}}

#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CI_CHANGE_JOB_INFINITY_SWORD:	return CXSLUnit::UC_ELSWORD_INFINITY_SWORD;
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case CI_CHANGE_JOB_DIMENSION_WITCH:	return CXSLUnit::UC_ARME_DIMENSION_WITCH;
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
	case CI_CHANGE_JOB_NIGHT_WATCHER:	return CXSLUnit::UC_LIRE_NIGHT_WATCHER;
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CI_CHANGE_JOB_VETERAN_COMMANDER:	return CXSLUnit::UC_RAVEN_VETERAN_COMMANDER;
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
	case CI_CHANGE_JOB_BATTLE_SERAPH:	return CXSLUnit::UC_EVE_BATTLE_SERAPH;
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CI_CHANGE_JOB_TACTICAL_TROOPER:	return CXSLUnit::UC_CHUNG_TACTICAL_TROOPER;
#endif
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
	case CI_CHANGE_JOB_LITTLE_HSIEN:	return CXSLUnit::UC_ARA_LITTLE_HSIEN;		/// �Ҽ�
	case CI_CHANGE_JOB_SAKRA_DEVANAM:	return CXSLUnit::UC_ARA_SAKRA_DEVANAM;		/// ��õ
#endif //SERV_ARA_CHANGE_CLASS_FIRST
#ifdef SERV_NEW_CHARACTER_EL
	case CI_CHANGE_JOB_SABER_KNIGHT:	return CXSLUnit::UC_ELESIS_SABER_KNIGHT;	/// ���̹� ����Ʈ
	case CI_CHANGE_JOB_PYRO_KNIGHT:		return CXSLUnit::UC_ELESIS_PYRO_KNIGHT;		/// ���̷� ����Ʈ
#endif //SERV_NEW_CHARACTER_EL
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
	case CI_CHANGE_JOB_LITTLE_DEVIL:	return CXSLUnit::UC_ARA_LITTLE_DEVIL;		/// �Ҹ�
	case CI_CHANGE_JOB_YAMA_RAJA:			return CXSLUnit::UC_ARA_YAMA_RAJA;		/// ���
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
	case CI_CHANGE_JOB_GRAND_MASTER:	return CXSLUnit::UC_ELESIS_GRAND_MASTER;	/// �׷��� ������
	case CI_CHANGE_JOB_BLAZING_HEART:	return CXSLUnit::UC_ELESIS_BLAZING_HEART;	/// ����¡ ��Ʈ
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
	case CI_CHANGE_JOB_PSYCHIC_TRACER:	return CXSLUnit::UC_ADD_PSYCHIC_TRACER;		/// ����ű Ʈ���̼�
#endif //SERV_9TH_NEW_CHARACTER
#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
	case CI_CHANGE_JOB_LUNATIC_PSYKER:	return CXSLUnit::UC_ADD_LUNATIC_PSYKER;		/// �糪ƽ ����Ŀ
#endif //SERV_ADD_LUNATIC_PSYKER
	}

	return CXSLUnit::UC_NONE;
}

int CXSLItem::GetCashItemByUnitClass( const char cUnitClass )
{
	switch( cUnitClass )
	{
	case CXSLUnit::UC_ELSWORD_KNIGHT:		return CI_CHANGE_JOB_ELSWORD_KNIGHT;
	case CXSLUnit::UC_ELSWORD_MAGIC_KNIGHT:	return CI_CHANGE_JOB_MAGIC_KNIGHT;
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
	case CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT:return CI_CHANGE_JOB_SHEATH_KNIGHT;
#endif SERV_ELSWORD_SHEATH_KNIGHT
		//{{ 2009. 10. 24  ������	���ҵ�2������
	case CXSLUnit::UC_ELSWORD_LORD_KNIGHT:	return CI_CHANGE_JOB_LORD_KNIGHT;
	case CXSLUnit::UC_ELSWORD_RUNE_SLAYER:	return CI_CHANGE_JOB_RUNE_SLAYER;
		//}}
	case CXSLUnit::UC_LIRE_COMBAT_RANGER:	return CI_CHANGE_JOB_COMBAT_RANGER;
	case CXSLUnit::UC_LIRE_SNIPING_RANGER:	return CI_CHANGE_JOB_SNIPING_RANGER;
#ifdef SERV_TRAPPING_RANGER_TEST
	case CXSLUnit::UC_LIRE_TRAPPING_RANGER:	return CI_CHANGE_JOB_TRAPPING_RANGER;
#endif SERV_TRAPPING_RANGER_TEST
		//{{ 2009. 11. 23  ������	����2������
	case CXSLUnit::UC_LIRE_WIND_SNEAKER:	return CI_CHANGE_JOB_WIND_SNEAKER;
	case CXSLUnit::UC_LIRE_GRAND_ARCHER:	return CI_CHANGE_JOB_GRAND_ARCHER;
		//}}
	case CXSLUnit::UC_ARME_HIGH_MAGICIAN:	return CI_CHANGE_JOB_HIGH_MAGICIAN;
	case CXSLUnit::UC_ARME_DARK_MAGICIAN:	return CI_CHANGE_JOB_DARK_MAGICIAN;
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CXSLUnit::UC_ARME_BATTLE_MAGICIAN:	return CI_CHANGE_JOB_BATTLE_MAGICIAN;
#endif SERV_ADD_ARME_BATTLE_MAGICIAN
		//{{ 2009. 12. 11  ������	���̻� 2�� ����
	case CXSLUnit::UC_ARME_ELEMENTAL_MASTER:	return CI_CHANGE_JOB_ELMENTAL_MASTER;
	case CXSLUnit::UC_ARME_VOID_PRINCESS:	return CI_CHANGE_JOB_VOID_PRINCESS;
		//}}
	case CXSLUnit::UC_RAVEN_SOUL_TAKER:		return CI_CHANGE_JOB_SOUL_TAKER;
	case CXSLUnit::UC_RAVEN_OVER_TAKER:		return CI_CHANGE_JOB_OVER_TAKER;
#ifdef SERV_RAVEN_WEAPON_TAKER
	case CXSLUnit::UC_RAVEN_WEAPON_TAKER:	return CI_CHANGE_JOB_WEAPON_TAKER;
#endif SERV_RAVEN_WEAPON_TAKER
		//{{ 2010. 1. 20  �տ���	���̺� 2�� ����
	case CXSLUnit::UC_RAVEN_BLADE_MASTER:	return CI_CHANGE_JOB_BLADE_MASTER;
	case CXSLUnit::UC_RAVEN_RECKLESS_FIST:	return CI_CHANGE_JOB_RECKLESS_FIST;
		//}}	
	case CXSLUnit::UC_EVE_EXOTIC_GEAR:		return CI_CHANGE_JOB_EXOTIC_GEAR;
	case CXSLUnit::UC_EVE_ARCHITECTURE:		return CI_CHANGE_JOB_ARCHITECTURE;
#ifdef SERV_EVE_ELECTRA
	case CXSLUnit::UC_EVE_ELECTRA:			return CI_CHANGE_JOB_EVE_ELECTRA;
#endif SERV_EVE_ELECTRA
		//{{ 2010. 1. 20  �տ���	�̺� 2�� ����
	case CXSLUnit::UC_EVE_CODE_NEMESIS:		return CI_CHANGE_JOB_NEMESIS;
	case CXSLUnit::UC_EVE_CODE_EMPRESS:		return CI_CHANGE_JOB_EMPRESS;
		//}}	
		//{{ 2011. 01. 26	������	û 1�� ����
	case CXSLUnit::UC_CHUNG_FURY_GUARDIAN:	return CI_CHANGE_JOB_FURY_GUARDIAN;
	case CXSLUnit::UC_CHUNG_SHOOTING_GUARDIAN: return CI_CHANGE_JOB_SHOOTER_GUARDIAN;
		//}}
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN:	return CI_CHANGE_JOB_SHELLING_GUARDIAN;
#endif
		//{{ JHKang / ������ / 2011.6.13 / û 2�� ����
	case CXSLUnit::UC_CHUNG_IRON_PALADIN:	return CI_CHANGE_JOB_IRON_PALADIN;
	case CXSLUnit::UC_CHUNG_DEADLY_CHASER:	return CI_CHANGE_JOB_DEADLY_CHASER;
		//}}

#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CXSLUnit::UC_ELSWORD_INFINITY_SWORD:	return CI_CHANGE_JOB_INFINITY_SWORD;
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case CXSLUnit::UC_ARME_DIMENSION_WITCH:		return CI_CHANGE_JOB_DIMENSION_WITCH;
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
	case CXSLUnit::UC_LIRE_NIGHT_WATCHER:		return CI_CHANGE_JOB_NIGHT_WATCHER;
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CXSLUnit::UC_RAVEN_VETERAN_COMMANDER:	return CI_CHANGE_JOB_VETERAN_COMMANDER;
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
	case CXSLUnit::UC_EVE_BATTLE_SERAPH:	return CI_CHANGE_JOB_BATTLE_SERAPH;
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CXSLUnit::UC_CHUNG_TACTICAL_TROOPER:	return CI_CHANGE_JOB_TACTICAL_TROOPER;
#endif
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
	case CXSLUnit::UC_ARA_LITTLE_HSIEN: return CI_CHANGE_JOB_LITTLE_HSIEN;				/// �Ҽ�
	case CXSLUnit::UC_ARA_SAKRA_DEVANAM: return CI_CHANGE_JOB_SAKRA_DEVANAM;			/// ��õ
#endif //SERV_ARA_CHANGE_CLASS_FIRST
#ifdef SERV_NEW_CHARACTER_EL
	case CXSLUnit::UC_ELESIS_SABER_KNIGHT:		return CI_CHANGE_JOB_SABER_KNIGHT;		/// ���̹� ����Ʈ
	case CXSLUnit::UC_ELESIS_PYRO_KNIGHT:		return CI_CHANGE_JOB_PYRO_KNIGHT;		/// ���̷� ����Ʈ
#endif //SERV_NEW_CHARACTER_EL
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
	case CXSLUnit::UC_ARA_LITTLE_DEVIL:			return CI_CHANGE_JOB_LITTLE_DEVIL;		/// �Ҹ�
	case CXSLUnit::UC_ARA_YAMA_RAJA:			return CI_CHANGE_JOB_YAMA_RAJA;			/// ���
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
	case CXSLUnit::UC_ELESIS_GRAND_MASTER:		return CI_CHANGE_JOB_GRAND_MASTER;		/// �׷��� ������
	case CXSLUnit::UC_ELESIS_BLAZING_HEART:		return CI_CHANGE_JOB_BLAZING_HEART;		/// ����¡ ��Ʈ
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
	case CXSLUnit::UC_ADD_PSYCHIC_TRACER:		return CI_CHANGE_JOB_PSYCHIC_TRACER;	/// �ֵ�
#endif //SERV_9TH_NEW_CHARACTER
#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
	case CXSLUnit::UC_ADD_LUNATIC_PSYKER:		return CI_CHANGE_JOB_LUNATIC_PSYKER;	/// �ֵ�
#endif //SERV_ADD_LUNATIC_PSYKER
	}

	return 0;
}
//}}


//{{ 2010. 04. 26  ������	PC�� ���� ĳ����
bool CXSLItem::IsPcBangOnlyCashItem( const int iItemID )
{
	switch( iItemID )
	{
#ifdef SERV_NET_CAFE_CASHSHOP
	case 70007272: // �ο� ��(����)
	case 70007277: // ������ 2�� ť�� 100��
#else //SERV_NET_CAFE_CASHSHOP
	case 221560: // PC�� ���� �Ǹ�[���� ������ ������]
	case 221570: // PC�� ���� �Ǹ�[�Ƿ��� �ߵ���(�����))
	case 221580: // PC�� ���� �Ǹ�[ȫ��]
	case 221590: // PC�� ���� �Ǹ�[���� ����]		
#endif //SERV_NET_CAFE_CASHSHOP
		return true;
	}

	return false;
}
//}}

#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
bool CXSLItem::IsKeepItemShowItem( const int iItemID )
{
	switch( iItemID )
	{
		case EI_SERVICE_COUPON_TYPE_A_PACKAGE:	 // ���� ���� ���� Type A ��Ű�� 
		case EI_SERVICE_COUPON_TYPE_B_PACKAGE: 	 // ���� ���� ���� Type B ��Ű��
		case EI_SERVICE_COUPON_TYPE_C_PACKAGE: 	 // ���� ���� ���� Type C ��Ű��
			return true;
	}

	return false;
}

bool CXSLItem::IsKeepItemShowItem2( const int iItemID )
{
	switch( iItemID )
	{
	case EI_ARA_SPECIAL_ATTRACTION_PACKAGE:		// �ƶ� ����� ������ ��Ű��
	case EI_ARA_SPECIAL_AVATAR1_PACKAGE:		// �ƶ� ����� �ƹ�Ÿ ��Ű��1
	case EI_ARA_SPECIAL_AVATAR2_PACKAGE:		// �ƶ� ����� �ƹ�Ÿ ��Ű��2
	case EI_ARA_SPECIAL_PET_PACKAGE:			// �ƶ� ����� �� ��Ű��
		return true;
	}

	return false;
}

bool CXSLItem::IsKeepItemShowItem3( const int iItemID )
{
	switch( iItemID )
	{
	case EI_SPECIAL_PRESENT_COPPER_KEY:
		return true;
	}

	return false;
}

bool CXSLItem::IsKeepItemShowItem4( const int iItemID )
{
	switch( iItemID )
	{
	case EI_SPECIAL_PRESENT_SILVER_KEY:
		return true;
	}

	return false;
}

bool CXSLItem::IsKeepItemShowItem5( const int iItemID )
{
	switch( iItemID )
	{
	case EI_SPECIAL_PRESENT_GOLD_KEY:
		return true;
	}

	return false;
}

bool CXSLItem::IsKeepItemShowItem6( const int iItemID )
{
	switch( iItemID )
	{
	case EI_SPECIAL_HEAVENLY_BOSS_RARE:
		return true;
	}

	return false;
}
#endif //SERV_KEEP_ITEM_SHOW_CASHSHOP

#ifndef DELETE_EXCEPTION_WISHLIST_CHANGEJOBITEM //ĳ�Ø� ���ϱ⿡�� ���� �����ۿ� ���� ����ó�� ����.
//{{ ĳ�ü� ���ϱ� ���� �ӽ� ó��
//{{ 2010. 03. 30  ������ �������������� Ȯ���ϴ� �Լ�
bool CXSLItem::IsChangeJobItem( const int iItemID )
{
	switch( iItemID )
	{
		// 1�� ����
	case CI_CHANGE_JOB_ELSWORD_KNIGHT:
	case CI_CHANGE_JOB_MAGIC_KNIGHT:
	case CI_CHANGE_JOB_HIGH_MAGICIAN:
	case CI_CHANGE_JOB_DARK_MAGICIAN:
	case CI_CHANGE_JOB_COMBAT_RANGER:
	case CI_CHANGE_JOB_SNIPING_RANGER:
	case CI_CHANGE_JOB_SOUL_TAKER:
	case CI_CHANGE_JOB_OVER_TAKER:
	case CI_CHANGE_JOB_EXOTIC_GEAR:
	case CI_CHANGE_JOB_ARCHITECTURE:
		//{{ kimhc // 2011.1.3 // û 1�� ����
	case CI_CHANGE_JOB_FURY_GUARDIAN:
	case CI_CHANGE_JOB_SHOOTER_GUARDIAN:
		//}} kimhc // 2011.1.3 // û 1�� ����

		//{{ Ʈ������ 1��
#ifdef SERV_ELSWORD_SHEATH_KNIGHT
	case CI_CHANGE_JOB_SHEATH_KNIGHT:
#endif SERV_ELSWORD_SHEATH_KNIGHT
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CI_CHANGE_JOB_BATTLE_MAGICIAN:
#endif SERV_ADD_ARME_BATTLE_MAGICIAN
#ifdef SERV_TRAPPING_RANGER_TEST
	case CI_CHANGE_JOB_TRAPPING_RANGER:
#endif SERV_TRAPPING_RANGER_TEST
#ifdef SERV_RAVEN_WEAPON_TAKER
	case CI_CHANGE_JOB_WEAPON_TAKER:
#endif SERV_RAVEN_WEAPON_TAKER
#ifdef SERV_EVE_ELECTRA
	case CI_CHANGE_JOB_EVE_ELECTRA:
#endif SERV_EVE_ELECTRA
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CI_CHANGE_JOB_SHELLING_GUARDIAN:
#endif
		//}} Ʈ������ 1��

		//{{ �Ҽ�
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
	case CI_CHANGE_JOB_LITTLE_HSIEN:
#endif //SERV_ARA_CHANGE_CLASS_FIRST
		//}} �Ҽ�

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
	case CI_CHANGE_JOB_LITTLE_DEVIL:
#endif // SERV_ARA_CHANGE_CLASS_SECOND

		//{{ ����1��
#ifdef SERV_NEW_CHARACTER_EL
	case CI_CHANGE_JOB_SABER_KNIGHT:
	case CI_CHANGE_JOB_PYRO_KNIGHT:
#endif //SERV_NEW_CHARACTER_EL
		//}} ����1��
		
#ifdef SERV_9TH_NEW_CHARACTER	// �ؿ��� �߰�
	case CI_CHANGE_JOB_PSYCHIC_TRACER:	// �ֵ� 1�� ���� ����ű Ʈ����
#endif SERV_9TH_NEW_CHARACTER	// �ؿ��� �߰�

		// 2�� ����
	case CI_CHANGE_JOB_LORD_KNIGHT:
	case CI_CHANGE_JOB_RUNE_SLAYER:
	case CI_CHANGE_JOB_WIND_SNEAKER:
	case CI_CHANGE_JOB_GRAND_ARCHER:
	case CI_CHANGE_JOB_ELMENTAL_MASTER:
	case CI_CHANGE_JOB_VOID_PRINCESS:
	case CI_CHANGE_JOB_BLADE_MASTER:
	case CI_CHANGE_JOB_RECKLESS_FIST:
	case CI_CHANGE_JOB_NEMESIS:
	case CI_CHANGE_JOB_EMPRESS:
//{{ JHKang / ������ / 2011.6.13 / û 2�� ����
	case CI_CHANGE_JOB_IRON_PALADIN:
	case CI_CHANGE_JOB_DEADLY_CHASER:
//}}
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CI_CHANGE_JOB_INFINITY_SWORD:
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case CI_CHANGE_JOB_DIMENSION_WITCH:
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
	case CI_CHANGE_JOB_NIGHT_WATCHER:
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CI_CHANGE_JOB_VETERAN_COMMANDER:
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
	case CI_CHANGE_JOB_BATTLE_SERAPH:
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CI_CHANGE_JOB_TACTICAL_TROOPER:
#endif
		//{{ ��õ
#ifdef SERV_ARA_CHANGE_CLASS_FIRST
	case CI_CHANGE_JOB_SAKRA_DEVANAM:
#endif //SERV_ARA_CHANGE_CLASS_FIRST
		//}} ��õ
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
	case CI_CHANGE_JOB_YAMA_RAJA:
#endif // SERV_ARA_CHANGE_CLASS_SECOND
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
	case CI_CHANGE_JOB_GRAND_MASTER:
	case CI_CHANGE_JOB_BLAZING_HEART:
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
	case CI_CHANGE_JOB_LUNATIC_PSYKER:
#endif //SERV_ADD_LUNATIC_PSYKER
		return true;
	}

	return false;
}
#endif //DELETE_EXCEPTION_WISHLIST_CHANGEJOBITEM

bool CXSLItem::IsBankExpandItem( const int iItemID )
{
	switch( iItemID )
	{
	case CI_BANK_MEMBERSHIP_SILVER:
	case CI_BANK_MEMBERSHIP_GOLD:
	case CI_BANK_MEMBERSHIP_EMERALD:
	case CI_BANK_MEMBERSHIP_DIAMOND:
	case CI_BANK_MEMBERSHIP_PLATINUM:
	case CI_BANK_MEMBERSHIP_ONE_PACKAGE:
		//{{ 2011. 12. 13  ��μ�	�� ���� Ȯ�� ������
#ifdef SERV_EXPAND_QUICK_SLOT
	case CI_EXPAND_QUICK_SLOT:
	case CI_EXPAND_QUICK_SLOT_ARA:
#endif SERV_EXPAND_QUICK_SLOT
		//}}
#ifdef SERV_SHARING_BANK_QUEST_CASH
	case CI_CASH_SHARING_BACK_OPEN:
#endif SERV_SHARING_BANK_QUEST_CASH
#ifdef SERV_CASH_ITEM_FOR_ELESIS	// ���볯¥: 2013-07-11
	case CI_EXPAND_QUICK_SLOT_ELESIS:
#endif	// SERV_CASH_ITEM_FOR_ELESIS
		return true;
	}
    
	return false;
}
//}}

//{{ 2011. 05. 11	������	�븮����
#ifdef SERV_PSHOP_AGENCY
int CXSLItem::GetPShopAgencyDays( IN const int iItemID )
{
	switch( iItemID )
	{
	case CXSLItem::SI_PSHOP_AGENCY_1_DAY:	return 1;
	case CXSLItem::SI_PSHOP_AGENCY_2_DAYS:	return 2;
	case CXSLItem::SI_PSHOP_AGENCY_3_DAYS:	return 3;
	case CXSLItem::SI_PSHOP_AGENCY_5_DAYS:	return 5;
	case CXSLItem::SI_PSHOP_AGENCY_7_DAYS:	return 7;
	case CXSLItem::SI_PSHOP_AGENCY_10_DAYS:	return 10;
	case CXSLItem::SI_PSHOP_AGENCY_15_DAYS: return 15;
	case CXSLItem::SI_PSHOP_AGENCY_30_DAYS:	return 30;
    case CXSLItem::SI_PSHOP_AGENCY_7_DAYS_ED: return 7;
    case CXSLItem::SI_PSHOP_AGENCY_15_DAYS_ED: return 15;
    case CXSLItem::SI_PSHOP_AGENCY_30_DAYS_ED: return 30;
	default: return 0;
	}
	
    return 0;
}

int CXSLItem::GetPShopAgencyType( IN const int iItemID_ )
{
    int ret = 0;
    switch( iItemID_ )
    {
    case CXSLItem::SI_PSHOP_AGENCY_1_DAY:	
    case CXSLItem::SI_PSHOP_AGENCY_2_DAYS:	
    case CXSLItem::SI_PSHOP_AGENCY_3_DAYS:	
    case CXSLItem::SI_PSHOP_AGENCY_5_DAYS:	
    case CXSLItem::SI_PSHOP_AGENCY_7_DAYS:	
    case CXSLItem::SI_PSHOP_AGENCY_10_DAYS:	
    case CXSLItem::SI_PSHOP_AGENCY_15_DAYS: 
    case CXSLItem::SI_PSHOP_AGENCY_30_DAYS:	
#ifdef SERV_UPGRADE_TRADE_SYSTEM
        ret = SEnum::AST_PREMIUM;
#else //SERV_UPGRADE_TRADE_SYSTEM
		ret = CXSLSquareUnit::PST_PREMIUM;
#endif //SERV_UPGRADE_TRADE_SYSTEM
        break;
    case CXSLItem::SI_PSHOP_AGENCY_7_DAYS_ED:
    case CXSLItem::SI_PSHOP_AGENCY_15_DAYS_ED:
    case CXSLItem::SI_PSHOP_AGENCY_30_DAYS_ED:
#ifdef SERV_UPGRADE_TRADE_SYSTEM
        ret = SEnum::AST_NORMAL;
#else //SERV_UPGRADE_TRADE_SYSTEM
		ret = CXSLSquareUnit::PST_NORMAL;
#endif //SERV_UPGRADE_TRADE_SYSTEM
        break;

    default: 
        ret = 0;
    }

    return ret;
}

#endif SERV_PSHOP_AGENCY
//}}

//{{ 2011. 06. 01	������	��ȭ�� ������ ���� ť��
#ifdef SERV_ENCHANTED_ITEM_CUBE_REWARD
//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
bool CXSLItem::UpdateEnchantedItemCubeReward( IN const int iCubeItemID, IN OUT KItemInfo& kInsertItemInfo )
#else
void CXSLItem::UpdateEnchantedItemCubeReward( IN const int iCubeItemID, IN OUT KItemInfo& kInsertItemInfo )
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}
{
	//{{ 2012. 05. 23	��μ�	[�̺�Ʈ] ������ũ�� �����մϴ�!
//#ifdef SERV_EVENT_APINK
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInsertItemInfo.m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"������ ���� �� �����ϴ�." )
			<< BUILD_LOG( iCubeItemID )
			<< BUILD_LOG( kInsertItemInfo.m_iItemID )
			<< END_LOG;

		//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		return false;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}
	}
//#endif SERV_EVENT_APINK
	//}}

	switch( iCubeItemID )
	{
#ifdef SERV_GLOBAL_EVENT_ITEM
	case 60007610: // 10�� ���� ť�� Lv.20
	case 60007611: // 10�� ���� ť�� Lv.32
	case 60007612: // 10�� ���� ť�� Lv.44
	case 60007613: // 10�� ���� ť�� Lv.52
	case 60007614: // 10�� ���� ť�� Lv.60
		{
			kInsertItemInfo.m_cEnchantLevel = 10;
		}
		break;
	case 67003610: // ����� ���� ���� ��� ť�� 1��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
#endif //SERV_GLOBAL_EVENT_ITEM
	case 160134: // ���� ��ȣ�� ���� ť�� 1��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
		}
		break;
	case 160135:	// �ù��� ��ȣ�� ���� ť�� 1��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_LIGHT;
		}
		break;
	case 160136: // �ĸ��� �뺴 ���� ť�� 1��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
		}
		break;
	case 160137: // �г��� ȭ�� ���� ť�� 1��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_BLAZE;
		}
		break;
	case 160138: // ������ ������ ���� ť�� 1��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WIND;
		}
		break;
	case 160139: // �������� ����� ���� ť�� 1��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
		}
		break;
	case 160140: // �����̽� ���� ���� ť�� 1��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_LIGHT;
		}
		break;
	case 160141: // ������ �뷡 ���� ť�� 1��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_BLAZE;
		}
		break;
	case 160142: // ���� ��ȣ�� ���� ť�� 1��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WIND;
		}
		break;
		////////////////////////////////////////////////////////////////////////// �߰� : 2011�� 9�� 14��
	case 160276: // �밨�� ���� ��ȣ�� ���� ť��(����Ʈ)
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
		}
		break;
	case 160277: // ���Ƿο� �ù��� ��ȣ�� ���� ť��(����Ʈ)
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_LIGHT;
		}
		break;
	case 160278: // �г��� �ĸ��� �뺴 ���� ť��(����Ʈ)
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
		}
		break;
	case 160279: // �ż��� �г��� ȭ�� ���� ť��(����Ʈ)
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_BLAZE;
		}
		break;
	case 160280: // ������ ������ ������ ���� ť��(����Ʈ)
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WIND;
		}
		break;
	case 160281: // ����� �������� ����� ���� ť��(����Ʈ)
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
		}
		break;
	case 160282: // ��н��� �����̽� ���� ���� ť��(����Ʈ)
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_LIGHT;
		}
		break;
	case 160283: // �ǲ��� ������ �뷡 ���� ť��(����Ʈ)
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_BLAZE;
		}
		break;
	case 160284: // 2�� ���� ����Ʈ ���� ť��
		{
			kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
		}
		break;
		////////////////////////////////////////////////////////////////////////// �߰� : 2011�� 9�� 14��

		////////////////////////////////////////////////////////////////////////// �߰� : 2012�� 3�� 28��
	case 160424: // +6 �밨�� ���� ��ȣ�� Ǯ��Ʈ ť��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160425: // +6 ���Ƿο� �ù��� ��ȣ�� Ǯ��Ʈ ť��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160426: // +6 �г��� �ĸ��� �뺴 Ǯ��Ʈ ť��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160427: // +6 �ż��� �г��� ȭ�� Ǯ��Ʈ ť��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160428: // +6 ������ ������ ������ Ǯ��Ʈ ť��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160429: // +6 ����� �������� ��� Ǯ��Ʈ ť��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160430: // +6 �����̽� ���� Ǯ��Ʈ ť��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160431: // +6 �ǲ��� ������ �뷡 Ǯ��Ʈ ť��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160432: // +6 ���������� ���� Ǯ��Ʈ ť��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160433: // +6 ������ ������ Ǯ��Ʈ ť��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160434: // +6 ���� ���ְ� Ǯ��Ʈ ť��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
		////////////////////////////////////////////////////////////////////////// �߰� : 2012�� 3�� 28��

	case 135098: // +10��ȭ �� Ǯ��Ʈ ť��
		{
			kInsertItemInfo.m_cEnchantLevel = 10;
		}
		break;
	case 161048: // +6��ȭ [�ں�]������ ���� ����
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		} break;
		//{{ 2012. 05. 23	��μ�	[�̺�Ʈ] ������ũ�� �����մϴ�!
//#ifdef SERV_EVENT_APINK
	case 181456:   // +6 ������ũ�� ��ȣ�� Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_BLAZE;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181457:   // +6 ���Ƿο� ������ũ Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181458:   // +6 �г��� ������ũ�� Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181459:   // +6 ����� ���̵� ������ũ Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_BLAZE;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181460:   // +6 ���� ������ ������ũ Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181461:   // +6 �밨�� ������ũ ��� Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_NATURE;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181462:   // +6 ������ũ ���� Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181463:   // +6 ���� ���̵� ������ũ Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181464:   // +6 ������ũ�� ���� Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181465:   // +6 ������ũ�� ������ Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 181466:   // +6 ������ũ�� ����� ��� Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
//#endif SERV_EVENT_APINK
		//}}

	case 160619:	// +6 õ�� ���� ��ȣ�� Ǯ��Ʈ ť��
	case 160732:	// �������� �߽� ���谡�� Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_BLAZE;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160620:	// +6 ���ε� õ�� ���� Ǯ��Ʈ ť��
	case 160733:	// �������� �ʺ� ���谡�� Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160621:	// +6 �г��� õ�� ���� Ǯ��Ʈ ť��
	case 160734:	// �������� �ʱ� ���谡�� Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160622:	// +6 �Ұ������� õ�� ���� Ǯ��Ʈ ť��
	case 160735:	// �������� �߱� ���谡�� Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_BLAZE;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160623:	// +6 ���� ������ õ�� ���� Ǯ��Ʈ ť��
	case 160736:	// �������� ��� ���谡�� Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160624:	// +6 ��Ȱ�� õ�� ���� Ǯ��Ʈ ť��
	case 160737:	// �������� �ֻ�� ���谡�� Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_NATURE;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160625:	// +6 ������ õ�� ���� Ǯ��Ʈ ť��
	case 160738:	// �������� ����� ���谡�� Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160626:	// +6 ��̿� õ�� ���� Ǯ��Ʈ ť��
	case 160739:	// �������� ���õ� ���谡�� Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160627:	// +6 ���� ġ�� õ�� ���� Ǯ��Ʈ ť��
	case 160740:	// �������� ���õ� ���谡�� Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160628:	// +6 �����ϴ� õ�� ���� Ǯ��Ʈ ť��
	case 160741:	// �������� ������ ���谡 Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_WATER;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160629:    // +6 ���� �� õ�� ���� Ǯ��Ʈ ť��
	case 160742:	// �������� �Ϻ��� ���谡 Ǯ��Ʈ ť��
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
	case 160641:   // õ�� ������ �� 30���� 10��ȭ ���� ť��
		{
			kInsertItemInfo.m_cEnchantLevel = 10;
		}
		break;

	case 8655943:   // +7 [�ں�] ��ŷ ���� ��� ť�� (Lv.30)
	case 8655944:   // +7 [�ں�] ��ŷ ���� ��� ť�� (Lv.40)
	case 8655945:   // +7 [�ں�] ��ŷ ���� ��� ť�� (Lv.50)
	case 8655946:   // +7 [�ں�] ��ŷ ���� ��� ť�� (Lv.60)
	case 8655947:   // +7 [�ں�] ��ŷ ���� ��� ť�� (Lv.65)
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON )
			{
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = CXSLAttribEnchantItem::ET_DARK;
				kInsertItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 = CXSLAttribEnchantItem::ET_DARK;
			}
			kInsertItemInfo.m_cEnchantLevel = 7;
		}
		break;
#ifdef SERV_EN_OPEN_ENCHANT_ITEM_EVENT
	case 85003011:   // ���� ���ȭ ������ ���谡�� ť��
		{
			kInsertItemInfo.m_cEnchantLevel = 5;
		}
		break;
#endif //SERV_EN_OPEN_ENCHANT_ITEM_EVENT

#ifdef SERV_PLAY_TOGETHER_EVENT
	case 90003000:	// �ű� ���� ���� ��� ť�� (Lv.16)
	case 90003001:	// �ű� ���� ���� ��� ť�� (Lv.22)
	case 90003002:	// �ű� ���� ���� ��� ť�� (Lv.26)
	case 90003003:	// �ű� ���� ���� ��� ť�� (Lv.34)
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
#endif SERV_PLAY_TOGETHER_EVENT

#ifdef SERV_BR_ENCHANT_CUBE_EVENT
	case 152000450:
	case 152000451:
	case 152000452:
	case 152000453:
	case 152000454:
	case 152000455:
		{
			kInsertItemInfo.m_cEnchantLevel = 5;
		}
		break;
#endif //SERV_CHUNG_EVENT_WEAPON_ENCHANT

#ifdef SERV_NEW_YEAR_EVENT_2014
	case 153000102:	// 6�� ��� Ǯ��Ʈ ť��(22����)
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
#endif //SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_EVENT_ENCHANT_LEVEL
	case 67003609:	// 6�� �ż��� �г��� ȭ�� Ǯ��Ʈ ť��
		{
			kInsertItemInfo.m_cEnchantLevel = 6;
		}
		break;
#endif SERV_EVENT_ENCHANT_LEVEL

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
	case 141000632:   // ������ ������ ��� ť��
		{
			kInsertItemInfo.m_cEnchantLevel = 6; //��ȭ ������ ������ �ش�.
		}
		break;
#endif SERV_EVENT_CHUNG_GIVE_ITEM

		//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	default:
		{
			return false;
		}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}
	}

	//{{ 2013. 05. 15	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	return true;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
}
#endif SERV_ENCHANTED_ITEM_CUBE_REWARD
//}}

#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // ���볯¥: 2013-06-27
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
int	CXSLItem::GetComeBackRewardSkillPointDays( IN const int iItemID )
{
	switch( iItemID )
	{
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_5_POINT_15_DAY:	return 15;
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_5_POINT_30_DAY:	return 30;
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_5_POINT_60_DAY:	return 60;

	default: return 0;
	}

	return 0;
}
#endif SERV_COME_BACK_USER_REWARD
//}}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

//{{ 2011. 11. 21  ��μ�	���� ���� ������
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
int CXSLItem::GetClassChangeCashItem( const int iItemID )
{
	switch( iItemID )
	{
////////// ���ҵ� ////////////////////////////////////////////////////////////////////////
	case CI_CLASS_CHANGE_ELSWORD_KNIGHT:			return CXSLUnit::UC_ELSWORD_KNIGHT;
	case CI_CLASS_CHANGE_ELSWORD_LORD_KNIGHT:		return CXSLUnit::UC_ELSWORD_LORD_KNIGHT;

	case CI_CLASS_CHANGE_ELSWORD_MAGIC_KNIGHT:		return CXSLUnit::UC_ELSWORD_MAGIC_KNIGHT;
	case CI_CLASS_CHANGE_ELSWORD_RUNE_SLAYER:		return CXSLUnit::UC_ELSWORD_RUNE_SLAYER;

	case CI_CLASS_CHANGE_ELSWORD_SHEATH_KNIGHT:		return CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT;
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CI_CLASS_CHANGE_ELSWORD_INFINITY_SWORD:	return CXSLUnit::UC_ELSWORD_INFINITY_SWORD;
#else
	case CI_CLASS_CHANGE_ELSWORD_SHEATH_KNIGHT_2:	return CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT_2;
#endif
////////// ���̻� ////////////////////////////////////////////////////////////////////////
	case CI_CLASS_CHANGE_AISHA_HIGH_MAGICIAN:		return CXSLUnit::UC_ARME_HIGH_MAGICIAN;
	case CI_CLASS_CHANGE_AISHA_ELMENTAL_MASTER:		return CXSLUnit::UC_ARME_ELEMENTAL_MASTER;

	case CI_CLASS_CHANGE_AISHA_DARK_MAGICIAN:		return CXSLUnit::UC_ARME_DARK_MAGICIAN;
	case CI_CLASS_CHANGE_AISHA_VOID_PRINCESS:		return CXSLUnit::UC_ARME_VOID_PRINCESS;

	case CI_CLASS_CHANGE_AISHA_BATTLE_MAGICIAN:		return CXSLUnit::UC_ARME_BATTLE_MAGICIAN;
#ifdef SERV_ARME_DIMENSION_WITCH
	case CI_CLASS_CHANGE_AISHA_DIMENSION_WITCH:		return CXSLUnit::UC_ARME_DIMENSION_WITCH;
#else
	case CI_CLASS_CHANGE_AISHA_BATTLE_MAGICIAN_2:	return CXSLUnit::UC_ARME_BATTLE_MAGICIAN_2;
#endif
////////// ���� ////////////////////////////////////////////////////////////////////////
	case CI_CLASS_CHANGE_RENA_COMBAT_RANGER:		return CXSLUnit::UC_LIRE_COMBAT_RANGER;
	case CI_CLASS_CHANGE_RENA_WIND_SNEAKER:			return CXSLUnit::UC_LIRE_WIND_SNEAKER;

	case CI_CLASS_CHANGE_RENA_SNIPING_RANGER:		return CXSLUnit::UC_LIRE_SNIPING_RANGER;
	case CI_CLASS_CHANGE_RENA_GRAND_ARCHER:			return CXSLUnit::UC_LIRE_GRAND_ARCHER;

	case CI_CLASS_CHANGE_RENA_TRAPPING_RANGER:		return CXSLUnit::UC_LIRE_TRAPPING_RANGER;
#ifdef SERV_RENA_NIGHT_WATCHER
	case CI_CLASS_CHANGE_RENA_NIGHT_WATCHER:		return CXSLUnit::UC_LIRE_NIGHT_WATCHER;
#else
	case CI_CLASS_CHANGE_RENA_TRAPPING_RANGER_2:	return CXSLUnit::UC_LIRE_TRAPPING_RANGER_2;
#endif
////////// ���̺� ////////////////////////////////////////////////////////////////////////
	case CI_CLASS_CHANGE_REVEN_SOUL_TAKER:			return CXSLUnit::UC_RAVEN_SOUL_TAKER;
	case CI_CLASS_CHANGE_REVEN_BLADE_MASTER:		return CXSLUnit::UC_RAVEN_BLADE_MASTER;

	case CI_CLASS_CHANGE_REVEN_OVER_TAKER:			return CXSLUnit::UC_RAVEN_OVER_TAKER;
	case CI_CLASS_CHANGE_REVEN_RECKLESS_FIST:		return CXSLUnit::UC_RAVEN_RECKLESS_FIST;

	case CI_CLASS_CHANGE_RAVEN_WEAPON_TAKER:		return CXSLUnit::UC_RAVEN_WEAPON_TAKER;
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CI_CLASS_CHANGE_RAVEN_VETERAN_COMMANDER:	return CXSLUnit::UC_RAVEN_VETERAN_COMMANDER;
#else
	case CI_CLASS_CHANGE_RAVEN_WEAPON_TAKER_2:		return CXSLUnit::UC_RAVEN_WEAPON_TAKER_2;
#endif
////////// �̺� ////////////////////////////////////////////////////////////////////////
	case CI_CLASS_CHANGE_EVE_EXOTIC_GEAR:			return CXSLUnit::UC_EVE_EXOTIC_GEAR;
	case CI_CLASS_CHANGE_EVE_NEMESIS:				return CXSLUnit::UC_EVE_CODE_NEMESIS;

	case CI_CLASS_CHANGE_EVE_ARCHITECTURE:			return CXSLUnit::UC_EVE_ARCHITECTURE;
	case CI_CLASS_CHANGE_EVE_EMPRESS:				return CXSLUnit::UC_EVE_CODE_EMPRESS;

	case CI_CLASS_CHANGE_EVE_ELECTRA:				return CXSLUnit::UC_EVE_ELECTRA;
#ifdef SERV_EVE_BATTLE_SERAPH
	case CI_CLASS_CHANGE_EVE_BATTLE_SERAPH:			return CXSLUnit::UC_EVE_BATTLE_SERAPH;
#else
	case CI_CLASS_CHANGE_EVE_ELECTRA_2:				return CXSLUnit::UC_EVE_ELECTRA_2;
#endif
////////// û ////////////////////////////////////////////////////////////////////////
	case CI_CLASS_CHANGE_CHUNG_FURY_GUARDIAN:		return CXSLUnit::UC_CHUNG_FURY_GUARDIAN;
	case CI_CLASS_CHANGE_CHUNG_IRON_PALADIN:		return CXSLUnit::UC_CHUNG_IRON_PALADIN;

	case CI_CLASS_CHANGE_CHUNG_SHOOTER_GUARDIAN:	return CXSLUnit::UC_CHUNG_SHOOTING_GUARDIAN;
	case CI_CLASS_CHANGE_CHUNG_DEADLY_CHASER:		return CXSLUnit::UC_CHUNG_DEADLY_CHASER;

	case CI_CLASS_CHANGE_CHUNG_SHELLING_GUARDIAN:		return CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN;
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CI_CLASS_CHANGE_CHUNG_TACTICAL_TROOPER:		return CXSLUnit::UC_CHUNG_TACTICAL_TROOPER;
#else
	case CI_CLASS_CHANGE_CHUNG_SHELLING_GUARDIAN_2:		return CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN_2;
#endif
////////// �����ý� ////////////////////////////////////////////////////////////////////////
#ifdef SERV_NEW_CHARACTER_EL	// ���볯¥: 2013-07-18
	case CI_CLASS_CHANGE_ELESIS_SABER_KNIGHT:		return CXSLUnit::UC_ELESIS_SABER_KNIGHT;
	case CI_CLASS_CHANGE_ELESIS_PYRO_KNIGHT:		return CXSLUnit::UC_ELESIS_PYRO_KNIGHT;
#endif	// SERV_NEW_CHARACTER_EL
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
	case CI_CLASS_CHANGE_ELESIS_GRAND_MASTER:		return CXSLUnit::UC_ELESIS_GRAND_MASTER;
	case CI_CLASS_CHANGE_ELESIS_BLAZING_HEART:		return CXSLUnit::UC_ELESIS_BLAZING_HEART;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
////////// �ƶ� ////////////////////////////////////////////////////////////////////////
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
	case CI_CLASS_CHANGE_UC_ARA_LITTLE_HSIEN:		return CXSLUnit::UC_ARA_LITTLE_HSIEN;
	case CI_CLASS_CHANGE_ARA_LITTLE_DEVIL:			return CXSLUnit::UC_ARA_LITTLE_DEVIL;

	case CI_CLASS_CHANGE_ARA_SAKRA_DEVANAM:			return CXSLUnit::UC_ARA_SAKRA_DEVANAM;
	case CI_CLASS_CHANGE_ARA_YAMA_RAJA:				return CXSLUnit::UC_ARA_YAMA_RAJA;
#endif // SERV_ARA_CHANGE_CLASS_SECOND
////////// �ֵ� ////////////////////////////////////////////////////////////////////////
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
	case CI_CLASS_CHANGE_PSYCHIC_TRACER:			return CXSLUnit::UC_ADD_PSYCHIC_TRACER;
#endif //SERV_9TH_NEW_CHARACTER
#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
	case CI_CLASS_CHANGE_LUNATIC_PSYKER:			return CXSLUnit::UC_ADD_LUNATIC_PSYKER;
#endif //SERV_ADD_LUNATIC_PSYKER

	}

	return CXSLUnit::UC_NONE;
}

#ifdef SERV_UNIT_CLASS_CHANGE_ITEM_POST_MESSEGE_FIX
int CXSLItem::GetClassChangeCashItemIDByUnitClass( const int iUnitClass_ )
{
	switch( iUnitClass_ )
	{
		////////// ���ҵ� ////////////////////////////////////////////////////////////////////////
	case CXSLUnit::UC_ELSWORD_KNIGHT:			return CI_CLASS_CHANGE_ELSWORD_KNIGHT;
	case CXSLUnit::UC_ELSWORD_LORD_KNIGHT:		return CI_CLASS_CHANGE_ELSWORD_LORD_KNIGHT;

	case CXSLUnit::UC_ELSWORD_MAGIC_KNIGHT:		return CI_CLASS_CHANGE_ELSWORD_MAGIC_KNIGHT;
	case CXSLUnit::UC_ELSWORD_RUNE_SLAYER:		return CI_CLASS_CHANGE_ELSWORD_RUNE_SLAYER;

	case CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT:		return CI_CLASS_CHANGE_ELSWORD_SHEATH_KNIGHT;
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CXSLUnit::UC_ELSWORD_INFINITY_SWORD:	return CI_CLASS_CHANGE_ELSWORD_INFINITY_SWORD;
#else
	case CXSLUnit::UC_ELSWORD_SHEATH_KNIGHT_2:	return CI_CLASS_CHANGE_ELSWORD_SHEATH_KNIGHT_2;
#endif
		////////// ���̻� ////////////////////////////////////////////////////////////////////////
	case CXSLUnit::UC_ARME_HIGH_MAGICIAN:		return CI_CLASS_CHANGE_AISHA_HIGH_MAGICIAN;
	case CXSLUnit::UC_ARME_ELEMENTAL_MASTER:		return CI_CLASS_CHANGE_AISHA_ELMENTAL_MASTER;

	case CXSLUnit::UC_ARME_DARK_MAGICIAN:		return CI_CLASS_CHANGE_AISHA_DARK_MAGICIAN;
	case CXSLUnit::UC_ARME_VOID_PRINCESS:		return CI_CLASS_CHANGE_AISHA_VOID_PRINCESS;

	case CXSLUnit::UC_ARME_BATTLE_MAGICIAN:		return CI_CLASS_CHANGE_AISHA_BATTLE_MAGICIAN;
#ifdef SERV_ARME_DIMENSION_WITCH
	case CXSLUnit::UC_ARME_DIMENSION_WITCH:		return CI_CLASS_CHANGE_AISHA_DIMENSION_WITCH;
#else
	case CXSLUnit::UC_ARME_BATTLE_MAGICIAN_2:	return CI_CLASS_CHANGE_AISHA_BATTLE_MAGICIAN_2;
#endif
		////////// ���� ////////////////////////////////////////////////////////////////////////
	case CXSLUnit::UC_LIRE_COMBAT_RANGER:		return CI_CLASS_CHANGE_RENA_COMBAT_RANGER;
	case CXSLUnit::UC_LIRE_WIND_SNEAKER:			return CI_CLASS_CHANGE_RENA_WIND_SNEAKER;

	case CXSLUnit::UC_LIRE_SNIPING_RANGER:		return CI_CLASS_CHANGE_RENA_SNIPING_RANGER;
	case CXSLUnit::UC_LIRE_GRAND_ARCHER:			return CI_CLASS_CHANGE_RENA_GRAND_ARCHER;

	case CXSLUnit::UC_LIRE_TRAPPING_RANGER:		return CI_CLASS_CHANGE_RENA_TRAPPING_RANGER;
#ifdef SERV_RENA_NIGHT_WATCHER
	case CXSLUnit::UC_LIRE_NIGHT_WATCHER:		return CI_CLASS_CHANGE_RENA_NIGHT_WATCHER;
#else
	case CXSLUnit::UC_LIRE_TRAPPING_RANGER_2:	return CI_CLASS_CHANGE_RENA_TRAPPING_RANGER_2;
#endif
		////////// ���̺� ////////////////////////////////////////////////////////////////////////
	case CXSLUnit::UC_RAVEN_SOUL_TAKER:			return CI_CLASS_CHANGE_REVEN_SOUL_TAKER;
	case CXSLUnit::UC_RAVEN_BLADE_MASTER:		return CI_CLASS_CHANGE_REVEN_BLADE_MASTER;

	case CXSLUnit::UC_RAVEN_OVER_TAKER:			return CI_CLASS_CHANGE_REVEN_OVER_TAKER;
	case CXSLUnit::UC_RAVEN_RECKLESS_FIST:		return CI_CLASS_CHANGE_REVEN_RECKLESS_FIST;

	case CXSLUnit::UC_RAVEN_WEAPON_TAKER:		return CI_CLASS_CHANGE_RAVEN_WEAPON_TAKER;
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CXSLUnit::UC_RAVEN_VETERAN_COMMANDER:	return CI_CLASS_CHANGE_RAVEN_VETERAN_COMMANDER;
#else
	case CXSLUnit::UC_RAVEN_WEAPON_TAKER_2:		return CI_CLASS_CHANGE_RAVEN_WEAPON_TAKER_2;
#endif
		////////// �̺� ////////////////////////////////////////////////////////////////////////
	case CXSLUnit::UC_EVE_EXOTIC_GEAR:			return CI_CLASS_CHANGE_EVE_EXOTIC_GEAR;
	case CXSLUnit::UC_EVE_CODE_NEMESIS:				return CI_CLASS_CHANGE_EVE_NEMESIS;

	case CXSLUnit::UC_EVE_ARCHITECTURE:			return CI_CLASS_CHANGE_EVE_ARCHITECTURE;
	case CXSLUnit::UC_EVE_CODE_EMPRESS:				return CI_CLASS_CHANGE_EVE_EMPRESS;

	case CXSLUnit::UC_EVE_ELECTRA:				return CI_CLASS_CHANGE_EVE_ELECTRA;
#ifdef SERV_EVE_BATTLE_SERAPH
	case CXSLUnit::UC_EVE_BATTLE_SERAPH:			return CI_CLASS_CHANGE_EVE_BATTLE_SERAPH;
#else
	case CXSLUnit::UC_EVE_ELECTRA_2:				return CI_CLASS_CHANGE_EVE_ELECTRA_2;
#endif
		////////// û ////////////////////////////////////////////////////////////////////////
	case CXSLUnit::UC_CHUNG_FURY_GUARDIAN:		return CI_CLASS_CHANGE_CHUNG_FURY_GUARDIAN;
	case CXSLUnit::UC_CHUNG_IRON_PALADIN:		return CI_CLASS_CHANGE_CHUNG_IRON_PALADIN;

	case CXSLUnit::UC_CHUNG_SHOOTING_GUARDIAN:	return CI_CLASS_CHANGE_CHUNG_SHOOTER_GUARDIAN;
	case CXSLUnit::UC_CHUNG_DEADLY_CHASER:		return CI_CLASS_CHANGE_CHUNG_DEADLY_CHASER;

	case CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN:		return CI_CLASS_CHANGE_CHUNG_SHELLING_GUARDIAN;
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CXSLUnit::UC_CHUNG_TACTICAL_TROOPER:		return CI_CLASS_CHANGE_CHUNG_TACTICAL_TROOPER;
#else
	case CXSLUnit::UC_CHUNG_SHELLING_GUARDIAN_2:		return CI_CLASS_CHANGE_CHUNG_SHELLING_GUARDIAN_2;
#endif
		////////// �ƶ� ////////////////////////////////////////////////////////////////////////
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
	case CXSLUnit::UC_ARA_LITTLE_HSIEN:				return CI_CLASS_CHANGE_UC_ARA_LITTLE_HSIEN;
	case CXSLUnit::UC_ARA_LITTLE_DEVIL:				return CI_CLASS_CHANGE_ARA_LITTLE_DEVIL;

	case CXSLUnit::UC_ARA_SAKRA_DEVANAM:			return CI_CLASS_CHANGE_ARA_SAKRA_DEVANAM;
	case CXSLUnit::UC_ARA_YAMA_RAJA:				return CI_CLASS_CHANGE_ARA_YAMA_RAJA;
#endif // SERV_ARA_CHANGE_CLASS_SECOND
		////////// �����ý� ////////////////////////////////////////////////////////////////////////
#ifdef SERV_NEW_CHARACTER_EL // ���� ������ ���� �� ����
	case CXSLUnit::UC_ELESIS_SABER_KNIGHT:			return CI_CLASS_CHANGE_ELESIS_SABER_KNIGHT;
	case CXSLUnit::UC_ELESIS_PYRO_KNIGHT:			return CI_CLASS_CHANGE_ELESIS_PYRO_KNIGHT;
#endif // SERV_NEW_CHARACTER_EL
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
	case CXSLUnit::UC_ELESIS_GRAND_MASTER:			return CI_CLASS_CHANGE_ELESIS_GRAND_MASTER;
	case CXSLUnit::UC_ELESIS_BLAZING_HEART:			return CI_CLASS_CHANGE_ELESIS_BLAZING_HEART;
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE
		////////// �ֵ� ////////////////////////////////////////////////////////////////////////
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
	case CXSLUnit::UC_ADD_PSYCHIC_TRACER:			return CI_CLASS_CHANGE_PSYCHIC_TRACER;
#endif //SERV_9TH_NEW_CHARACTER
#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
	case CXSLUnit::UC_ADD_LUNATIC_PSYKER:			return CI_CLASS_CHANGE_LUNATIC_PSYKER;
#endif //SERV_ADD_LUNATIC_PSYKER
	}

	return 0;
}
#endif SERV_UNIT_CLASS_CHANGE_ITEM_POST_MESSEGE_FIX
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

#ifdef SERV_WISH_ITEM_DISABLE
const bool	CXSLItem::IsPossibleCart( const int iItemID_ )
{
	if( CXSLItem::GetClassChangeCashItem( iItemID_ ) != CXSLUnit::UC_NONE )
		return false;

	return true;
}
#endif // SERV_WISH_ITEM_DISABLE

#ifdef SERV_GOLD_TICKET_SEAL_COUNT
bool CXSLItem::IsGoldTicket( const int iItemID )
{
	switch( iItemID )
	{
	case CXSLItem::SI_GOLD_TICKET_10_ID:
	case CXSLItem::SI_GOLD_TICKET_50_ID:
	case CXSLItem::SI_GOLD_TICKET_100_ID:
	case CXSLItem::SI_GOLD_TICKET_500_ID:
	case CXSLItem::SI_GOLD_TICKET_1000_ID:
	case CXSLItem::SI_GOLD_TICKET_2000_ID:
		return true;
	}

	return false;
}
#endif SERV_GOLD_TICKET_SEAL_COUNT

#ifdef SERV_SKILL_PAGE_SYSTEM
bool CXSLItem::IsItemToMakeSkillPageExpanded( const int iItemID_ )
{
	switch ( iItemID_ )
	{
	case CI_EXPAND_SKILL_PAGE:
		return true;
		break;

	default:
		break;
	}

	return false;	
}
#endif // SERV_SKILL_PAGE_SYSTEM