#pragma once

#include "RefreshSingleton.h"
#include "X2Data/XSLMain.h"
#include <map>
#include <vector>


class CXSLBuffManager
{
	DeclareRefreshSingleton( CXSLBuffManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	/*
	���� ���̵�� DB�� ��ϵǴ� �����̱� ������, ������ ���� �� ���� �����ϼž� �մϴ�.
	*/
	enum BUFF_TEMPLET_ID
	{
		BTI_NONE						= 0,
		
		// ���
		BTI_BUFF_PROTECTION_OF_NUT 			= 109,	// ��Ʈ�� ��ȣ
		BTI_BUFF_PROTECTION_OF_PTAH 		= 110,	// ��Ÿ�� ��ȣ
		BTI_BUFF_PROTECTION_OF_LAHEL 		= 111,	// ������ ��ȣ
		BTI_BUFF_PROTECTION_OF_ZACHIEL 		= 112,	// ��Ű���� ��ȣ
		
		// ����
		BTI_BUFF_BELSSING_OF_SERAPHIM 		= 113,	// �������� �ູ
		BTI_BUFF_BELSSING_OF_GEB 			= 114,	// �Ժ��� �ູ
		BTI_BUFF_BELSSING_OF_AMON 			= 115,	// �Ƹ��� �ູ
		BTI_BUFF_BELSSING_OF_CRONOS 		= 116,	// ũ�γ뽺�� �ູ
		
		BTI_BUFF_STRENGTHENING_BODY			= 117,	// ��ȭ�� ��ü ( ��� ����� )
		
		BTI_BUFF_RALLY_OF_HERO_LEVEL_1		= 118,	// ������ ����
		BTI_BUFF_RALLY_OF_HERO_LEVEL_2		= 119,	// ������ ����
		BTI_BUFF_RALLY_OF_HERO_LEVEL_3		= 120,	// ������ ����
		
		BTI_DEBUFF_REST_OF_RELLY			= 121,	// �������� ���� �޽�
		
		BTI_BUFF_WARM_ENERGE_OF_EL			= 122,	// ����� �޽�( ���� )
		
		BTI_BUFF_THANKS_OF_RESIDENTS		= 123,	// �ֹ��� ����
		BTI_BUFF_BREATH_OF_DRAGON			= 124,	// �巡���� ����
		BTI_BUFF_BLESSING_OF_EL				= 125,	// ���� �ູ
		
		BTI_BUFF_HERO_OF_ELIOS_LEVEL_1		= 126,	// ���������� ��ȣ ���� ���� 1
		BTI_BUFF_HERO_OF_ELIOS_LEVEL_2		= 127,	// ���������� ��ȣ ���� ���� 2
		BTI_BUFF_HERO_OF_ELIOS_LEVEL_3		= 128,	// ���������� ��ȣ ���� ���� 3
		BTI_BUFF_HERO_OF_ELIOS_LEVEL_4		= 129,	// ���������� ��ȣ ���� ���� 4
		
		BTI_BUFF_PVP_REVENGE_MODE_LEVEL_1	= 130,	// ������ ��� ���� 1
		BTI_BUFF_PVP_REVENGE_MODE_LEVEL_2	= 131,	// ������ ��� ���� 2
		BTI_BUFF_PVP_REVENGE_MODE_LEVEL_3	= 132,	// ������ ��� ���� 3
		
		BTI_BUFF_RETURN_OF_HERO				= 133,	// ��ȯ�� ����

		BTI_BUFF_PREMIUM_PC_ROOM			= 138,	// PC �� ����

		BTI_BUFF_PET_AURA_SKILL 			= 139,	// �� �ݷ� ��ų
		BTI_BUFF_MECHANIZATION_SPEED_UP		= 140,	// ���ȭ ��Ʈ ���ǵ� ����

		BTI_SI_SA_EEG_ATOMIC_SHIELD			= 141,  // ����� ���� �����

		BTI_DEBUFF_POWERFUL_BOWSTRING		= 142,  // �׷������ ��ȭ�� ���� ��Ʈ���� �ǰ��� �����

		BTI_BUFF_SUPER_ARMOR				= 143, 	// ���� �Ƹ� ����

		BTI_BUFF_WALDO_TROCK				= 144, 	// �Ŵ��� �е��� ����ϴ� ����
		BTI_BUFF_BUGI_TROCK					= 145, 	// ���� Ʈ���� ����ϴ� ����
		BTI_DEBUFF_SPRIGGAN_ENTANGLE		= 146, 	// ����(���ʱ�) ���������� ����ϴ� ���ʱ�

		BTI_BUFF_HENIR_FIRE					= 147,	// ��ϸ� ���� - ���� ��
		BTI_BUFF_HENIR_WATER				= 148,	// ��ϸ� ���� - ���� ��
		BTI_BUFF_HENIR_NATURE				= 149,	// ��ϸ� ���� - ���� ��
		BTI_BUFF_HENIR_WIND					= 150,	// ��ϸ� ���� - �ٶ��� ��
		BTI_BUFF_HENIR_LIGHT				= 151,	// ��ϸ� ���� - ���� ��
		BTI_DEBUFF_SHADOW_JAIL				= 152,	// �׸��� ���� �����
		BTI_DEBUFF_SHADOW_KNOT				= 153,	// �׸��� �ŵ� �����

		BTI_EMPTY_EXP_BUFF				    = 154,	// �ƶ�� �Բ� �÷����ϸ� ����ġ ���ʽ�

		BTI_BUFF_FIELD_DEFENCE				= 187,	// ��Ʋ�ʵ� �̺�Ʈ ����(��ġ +30%, ED +30%)

		BTI_BUFF_APRIL_FOOLSDAY				= 188,	// ������ ���κ��� ����

		BTI_BUFF_GATE_DEFENCE				= 189,	// ����� �� ������ ������ ���� ���� ����
		BTI_BUFF_ENEMY_GATE_DEFENCE			= 190,	// ����� �� ������ ������ ���� ��ȯ�� ���� ���� ����

		BTI_BUFF_ELIXIR_GIANT_POTION		= 191,	// ����� �� ���� : ����ȭ ����
		BTI_BUFF_ELIXIR_BLAZING_BOMB		= 192,	// ����� �� ���� : �̱��̱� ����
		BTI_BUFF_ELIXIR_SPIRIT_OF_CHASER	= 193,	// ����� �� ���� : �������� ȥ
		BTI_BUFF_ELIXIR_CRADLE_OF_LITTLE_FAIRY	= 194,	// ����� �� ���� : �Ʊ� ���� ���
		BTI_BUFF_ELIXIR_ICE_BALL_OF_DENIPH	= 195,	// ����� �� ���� : �������� ���� ����
		BTI_BUFF_ELIXIR_FEATHER_OF_VENTUS	= 196,	// ����� �� ���� : �������� ������
		BTI_BUFF_ELIXIR_FLAME_RING_OF_ROSSO	= 197,	// ����� �� ���� : �ν��� ȭ����
		BTI_BUFF_ELIXIR_BIG_HAND_POTION		= 198,	// ����� �� ���� :�Ŵ�� ����

		BTI_BUFF_2013_DEFENSE_FAKE_BUFF		= 201,	// 2013 ����� �� ������ ��¥ ����
		BTI_BUFF_2013_DEFENSE_BUFF			= 202,	// 2013 ����� �� ������ ��¥ ����
		BTI_BUFF_2013_DEFENSE_DEBUFF		= 203,	// 2013 ����� �� ���� �����
		BTI_BUFF_2013_DEFENSE_BUFF_EVENT	= 204,	// �̺�Ʈ�� 2013 ����� �� ������ ����

		BTI_FESTIVAL_EVENT_BUFF				= 207,	// ������ ���� �̺�Ʈ ����,  (����,���� ���ݷ� 10% ����)

		BTI_BUFF_TRADE_BLOCK                = 213,   // �ؿ� ���� �ű� ĳ���� �ŷ� �� ����

#ifdef SERV_CRAYON_POP_EVENT_BUFF// �۾���¥: 2013-06-10	// �ڼ���
		BTI_BUFF_ELRIN						= 215,	// ũ������ ����
		BTI_BUFF_SOYUL						= 216,	// ũ������ ���� 
		BTI_BUFF_KUMI						= 217,	// ũ������ �ݹ�
		BTI_BUFF_CHOA						= 218,	// ũ������ �ʾ�
		BTI_BUFF_WEY						= 219,	// ũ������ ����
#endif // SERV_CRAYON_POP_EVENT_BUFF

//#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
		BTI_BUFF_2013_DEFENCE_ENTER_100_PERCENT_BUFF = 300,
//#endif //SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT

		BTI_DEBUFF_RAINCOAT_CAT_STEB1_ATTACK_A = 301,                
		BTI_DEBUFF_RAINCOAT_CAT_STEB2_ATTACK_A = 302,                 
		BTI_DEBUFF_RAINCOAT_CAT_STEB3_ATTACK_A = 303,                  

		BTI_DEBUFF_ANCIENT_FIRE							= 304,	

		BTI_BUFF_WHEN_RIDE_ON_PET_FOR_ONLY_MASTER_FIX		= 305,	

		BTI_BUFF_MADNESS_SOUP							= 306,  
		BTI_BUFF_BIG_HEAD_SOUP							= 307,
		BTI_BUFF_HP_UP_SOUP								= 308,
		BTI_BUFF_MP_UP_SOUP								= 309,
		BTI_BUFF_SPEED_UP_SOUP							= 310,
		BTI_BUFF_ATTACK_UP_SOUP							= 311,

//#ifdef SERV_BONUS_BUFF_SYSTEM
		BTI_BUFF_2YEARS_EVENT_CN_DAMAGE_UP			= 312,
		//BTI_BUFF_2YEARS_EVENT_CN_MOVE_JUMP_SPEED_UP = 313, // ���� �� ���� �̳� ���� �ʿ� // �ʵ� ���� ������ �Ұ� ������� ��ħ.
		BTI_BUFF_2YEARS_EVENT_CN_ACCURACY_AVOID_UP	= 314,
		BTI_BUFF_2YEARS_EVENT_CN_CRI_SPEED_UP		= 315,
		BTI_BUFF_2YEARS_EVENT_CN_DAMAGE_INC_DEC_UP	= 316,
		BTI_BUFF_2YEARS_EVENT_CN_ALL_IN_ONE			= 317,
//#endif //SERV_BONUS_BUFF_SYSTEM

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-11-05	// �ڼ���
		BTI_DEBUFF_REST_OF_RECHALLENGE		= 313,	// �ʵ� ���� �ý��� ������ �Ұ� �����
#endif // SERV_BATTLE_FIELD_BOSS
		BTI_SECRET_OF_THOUSAND_YEARS_FOX				= 327,  //SERV_BUFF_BONUSRATE_HAMEL_EVENT �ϸ� �̺�Ʈ ���� 1.2��
		
		BTI_BUFF_2013_CHRISTMAS				= 330,	// itexpertkim // ����ö // 2013-12-17
		BTI_BUFF_NAVER						= 339,	// ���̹�ä�θ�����, ����/�������ݷ� 3%, ����/������� 3%, MP �ִ�ġ 10% ����
//#ifdef SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC
		BTI_BUFF_THE_ESSENCE_OF_WEAK_HERETIC_POTION	= 1318,
		BTI_BUFF_THE_ESSENCE_OF_HERETIC_POTION		= 1319,
//#endif SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC

//#ifdef SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
		BTI_BUFF_RURIEL_MANA_ENERGIZE_POTION		= 1320,
//#endif SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
	};

	enum UNIT_BUFF_KEEP_TIME
	{
		UBKT_UNIT_RESET_TIME_HOUR = 6,				// 6�ð� ���� �ʱ�ȭ - �ð�
		UBKT_UNIT_RESET_TIME_SECOND = 21600,		// 6�ð� ���� �ʱ�ȭ - ��
	};

	enum BUFF_ATTRIBUTE
	{
		BA_MULTIPLY_ATTACK_STAT = 0,
		BA_MULTIPLY_DEFENCE_STAT,
		BA_MULTIPLY_HP,
	};

	struct BuffTemplet
	{
		BUFF_TEMPLET_ID			m_eBuffID;
		std::wstring			m_wstrBuffName;
		std::vector< int >		m_vecOption;
		double					m_fBuffPeriodSecond;

		//{{ 2012. 05. 6	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		bool					m_bEndBuffAtTheLeaveDungeonRoom;
		bool					m_bEndBuffAtTheEndDungeonGame;

		//{{ 2012. 07. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2_2
		bool					m_bEndBuffAtTheLeavePvpRoom;
		bool					m_bEndBuffAtTheEndPvpGame;
#endif SERV_2012_PVP_SEASON2_2
		//}}

		bool					m_bEndBuffAtTheUserDied;
		bool					m_bKeepBuffAtTheUserLogOut;
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}

#ifdef SERV_SERVER_BUFF_SYSTEM// �۾���¥: 2013-05-14	// �ڼ���
		bool					m_bEndBuffAtTheLeaveBattleFieldRoom;
#endif // SERV_SERVER_BUFF_SYSTEM
		
		//{{ 2012. 04. 17	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		std::vector< int >		m_vecRoomType;
		bool CheckRoomType( IN const int& iCheckRoomType ) const;
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}

		//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
		int						m_iFactorID;
#endif SERV_SERVER_BUFF_SYSTEM
		//}}
		
		BuffTemplet()
		{
			m_eBuffID = BTI_NONE;
			m_fBuffPeriodSecond = 0.0f;

			//{{ 2012. 05. 6	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
			m_bEndBuffAtTheLeaveDungeonRoom	= false;
			m_bEndBuffAtTheEndDungeonGame	= false;

			//{{ 2012. 07. 11	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2_2
			m_bEndBuffAtTheLeavePvpRoom		= false;
			m_bEndBuffAtTheEndPvpGame		= false;
#endif SERV_2012_PVP_SEASON2_2
			//}}

			m_bEndBuffAtTheUserDied			= false;
			m_bKeepBuffAtTheUserLogOut		= false;
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
			//}}

#ifdef SERV_SERVER_BUFF_SYSTEM// �۾���¥: 2013-05-14	// �ڼ���
			m_bEndBuffAtTheLeaveBattleFieldRoom	= false;
#endif // SERV_SERVER_BUFF_SYSTEM

			//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
			m_iFactorID						= 0;
#endif SERV_SERVER_BUFF_SYSTEM
			//}
		}
	};

public:
	CXSLBuffManager(void);
	~CXSLBuffManager(void);

	// for lua
	bool AddBuffInfo_LUA();

	const BuffTemplet* GetBuffTemplet( IN const int iBuffID ) const;
	bool IsExistTitleID( IN const int iBuffID ) const		{ return ( GetBuffTemplet( iBuffID ) != NULL ); }

private:
	std::map< BUFF_TEMPLET_ID, BuffTemplet >		m_mapBuffList;

};

DefRefreshSingletonInline( CXSLBuffManager );



