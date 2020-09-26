#ifndef _GCENUM_H_
#define _GCENUM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/// @enum EGCGameMode �׷���ü�̽� ���� ���
enum EGCGameMode // GC_GM_*
{
	GC_GM_INVALID				= -1,       ///< invalid game mode value

    // ����
    GC_GM_TEAM_MODE				= 0,        ///< �� ���
    GC_GM_SURVIVAL_MODE			= 1,        ///< �����̹� ���

    // �����
    GC_GM_GUILD_BATTLE			= 2,		///< ����� ���

    // ���� �̼�
    GC_GM_ANGEL_RESCUE			= 3,		///< õ�� ���� ����
    GC_GM_ARCHER_CONTEST		= 4,		///< ���չ� �ü� ��ȸ
    GC_GM_GADOSEN_DEFEAT		= 5,		///< ������ ��ġ ����

    // ���� ������
    GC_GM_MONSTER_CRUSADERS		= 6,		///< ���� ������

    // ���� ���
    GC_GM_MONSTER_HUNT			= 7,        ///< ���� ���

	GC_GM_ARENA_TEAM			= 8,		///< �Ʒ�����Ʋ �����
	GC_GM_ARENA_SURVIVAL		= 9,		///< �Ʒ�����Ʋ �����̹����

    // ����Ʈ
    GC_GM_QUEST1				= 10,       ///< ����Ʈ1
    GC_GM_QUEST2				= 11,       ///< ����Ʈ2	
    GC_GM_QUEST3				= 12,       ///< ����Ʈ3
    GC_GM_QUEST4				= 13,       ///< ����Ʈ4
    GC_GM_QUEST5				= 14,       ///< ����Ʈ5
	GC_GM_QUEST6				= 15,       ///< ����ƮFINAL

	GC_GM_QUEST1_1_VVVIC		= 16,		///< ����Ʈ1_1 ����
	GC_GM_QUEST1_2_VVVIC		= 17,		///< ����Ʈ1_2 ����
	GC_GM_QUEST2_1_VVVIC		= 18,		///< ����Ʈ2_1 ����
	GC_GM_QUEST2_2_VVVIC		= 19,		///< ����Ʈ2_2 ����
	GC_GM_QUEST3_1_VVVIC		= 20,
	GC_GM_QUEST3_2_VVVIC		= 21,
	GC_GM_QUEST4_1_VVVIC		= 22,
	GC_GM_QUEST4_2_VVVIC		= 23,
	GC_GM_QUEST6_1_VVVIC		= 24,
	GC_GM_QUEST6_2_VVVIC		= 25,

	GC_GM_QUEST_ZERO			= 26,		///< ����Ʈ_ZERO
    GC_GM_QUEST_ZERO_2			= 27,		///< ����Ʈ_ZERO_2

	GC_GM_TAG_TEAM				= 28,		///< �±׸�ġ �����
	GC_GM_TUTORIAL				= 29,		///< Ʃ�丮��

    NUM_GC_GAME_MODE,           ///< ���� ��� �� ����
};

/// @enum EGCGameMode �׷���ü�̽� ���� ��� ī�װ�
enum EGCGameModeCategory // GC_GMC_*
{
    GC_GMC_MATCH,             ///< ����             0
    GC_GMC_GUILD_BATTLE,      ///< �����           1
    GC_GMC_PROMOTION_MISSION, ///< ���� �̼�        2
    GC_GMC_MONSTER_CRUSADERS, ///< ���� ������    3
    GC_GMC_MONSTER_HUNT,      ///< ���� ���      4
    GC_GMC_QUEST,             ///< ����Ʈ           5
	GC_GMC_ARENABATTLE,       ///< �Ʒ�����Ʋ       6
	GC_GMC_TAG_MATCH,         ///< �±� ��ġ		7
	GC_GMC_TUTORIAL,		  ///< Ʃ�丮��			8
	
	GC_GMC_COUNT,			  ///< ���ī�װ� ����
    GC_GMC_INVALID           = -1,                      ///< invalid game mode category value
};

/// @enum EGCGameModeType �׷���ü�̽� ���� ��� Ÿ��
enum EGCGameModeType
{
    GC_GMT_ALONE_FIGHT,     ///< ���� �ο�� Ÿ��
    GC_GMT_TEAM_FIGHT,      ///< ���� �̷�� �ο�� Ÿ��
    GC_GMT_UNION_FIGHT,     ///< �����Ͽ� �ο�� Ÿ��

    NUM_GC_GAME_MODE_TYPE,  ///< ���� ��� Ÿ�� �� ����

    GC_GMT_INVALID  = -1,   ///< invalid game mode type value
};

/// @enum EGCSubGameMode �׷���ü�̽� ���� ���� ���
enum EGCSubGameMode // GC_SGM_*
{
    GC_SGM_NORMAL,          ///< �Ϲ� ���� ���(��������)
    GC_SGM_NO_ITEM,         ///< ��������� ���� ���

	GC_SGM_QUEST_LEVEL1,
	GC_SGM_QUEST_LEVEL2,
	GC_SGM_QUEST_LEVEL3,
	GC_SGM_QUEST_LEVEL4,
	GC_SGM_QUEST_LEVEL5,

    NUM_GC_SUB_GAME_MODE,   ///< ���� ���� ��� �� ����

    GC_SGM_INVALID  = -1,   ///< invalid sub game mode value
};
enum EGCObjType
{
    GC_OBJ_DEFAULT      = 0,
    GC_OBJ_MON          = 1,
    GC_OBJ_TARGET       = 2,        //�� Type�� �� �μž��� �Ѿ �� �ִٴ� Type�̴�.
    GC_OBJ_END,
};
enum EGCRenderLayer
{
    GC_LAYER_SKY                    = 0,
    GC_LAYER_BETWEEN_SKY_BLOCK      = 1,
    GC_LAYER_BLOCK                  = 2,
    GC_LAYER_BETWEEN_BLOCK_CHAR     = 3,
    GC_LAYER_CHAR                   = 4,
    GC_LAYER_BETWEEN_CHAR_FRONT     = 5,
    GC_LAYER_FRONT_MAP              = 6,
    GC_LAYER_FACE                   = 7,    // �ʻ�� ���� ������ ��.
    GC_LAYER_UI                     = 8,
    GC_LAYER_END,
};
/// @enum EGCMonster �׷���ü�̽� ����
enum EGCMonster // MON_*
{
    MON_GORGOS          = 0,    ///< ����
    MON_TROLL           = 1,    ///< Ʈ��
    MON_HARPY           = 2,    ///< ����
    MON_MUSHROOM        = 3,    ///< ������(Normal)
    MON_MUSHROOM_S      = 4,    ///< ������(Small)
    MON_DRILLMON        = 5,    ///< �帱��
    MON_SCARECROW       = 6,    ///< ����ƺ�
    MON_PARADOM         = 7,    ///< �Ķ�
    MON_ANGEL           = 8,    ///< õ��
    MON_TARGET          = 9,    ///< ȭ�����
    MON_GADOSEN         = 10,   ///< ������
    MON_DRAGON          = 11,   ///< �ٽǸ���ũ
	MON_CANNON          = 12,   ///< ����
    MON_WOOD_DOOR       = 13,   ///< ������
    MON_IRON_DOOR       = 14,   ///< ö��
    MON_TREASURE_BOX    = 15,   ///< ��������
    MON_SOLDIER1_A      = 16,   ///< ����1-A
    MON_SOLDIER1_B      = 17,   ///< ����1-B
    MON_SOLDIER2_A      = 18,   ///< ����2-A
    MON_SOLDIER2_B      = 19,   ///< ����2-B
    MON_NEW_GORGOS      = 20,   ///< �������
    MON_SANTA           = 21,   ///< ��Ÿ��
	MON_MONGBAN_A		= 22,	///< ����Ʈ2 ������A
	MON_KAIDO			= 23,	///< ����Ʈ2 ī�̵�(����)
	MON_MONGBAN_B		= 24,	///< ����Ʈ2 ������B
	MON_GAIKOZ			= 25,	///< ����Ʈ2 ��������
	//��������� ��ȣ�� ���� ���ʹ� ����� �߰����� �� ��!!!
	//������ ���� �߰��� ������ �� �޹�ȣ��... ���ϸ� ���δ�...
	MON_LAPSTER			= 26,	///< ����Ʈ3 �Ƹ�������
	MON_GHOST_FISH		= 27,	///< ����Ʈ3 �ͽžƱ�
	MON_GHOST_FISH_B	= 28,	///< ����Ʈ3 �ͽžƱ�B
	MON_PATUSEI_HEAD	= 29,	///< �������� �Ӹ�
	MON_PATUSEI_BODY	= 30,	///< �������� ��
	MON_PATUSEI_LEG		= 31,	///< �������� �ٸ�
	
	MON_ELLENA			= 32,	///< ������
	MON_KAMIKI			= 33,	///< ī��Ű
	MON_SANGSUN			= 34,	///< �󼱾�
	MON_KAZEAZE         = 35,   ///< ī������
	MON_KAKIRU_HEAD     = 36,	///< īŰ�̷� �Ӹ�
	MON_KAKIRU_LEFT     = 37,	///< īŰ�̷� ����
	MON_KAKIRU_RIGHT    = 38,	///< īŰ�̷� ������
	MON_KAKIRU_DOOR     = 39,   ///< īŰ�̷� ��
	MON_CASTLE_DOOR		= 40,	///< ����ƮFinal ����
	MON_TRANS_KAZEAZE   = 41,   ///< ����ī������
	MON_LAS			    = 42,	///< ��ĳ���� : ����(����Ʈ_FINAL)�� ó������ �����Ҷ� �����
	MON_NEW_SCARECROW   = 43,
	MON_RACOON			= 44,	///< �ʱ���
	MON_SLIM			= 45,	///< ������
	MON_ENT				= 46,	///< ��Ʈ
    MON_H_GOBLIN        = 47,   ///< ȩ ���
    MON_WENDY           = 48,   ///< ����
    MON_ORC             = 49,   ///< ��ũ
    MON_GOBLIN          = 50,   ///< ���
	MON_MINI_INSECTIVOROUS	= 51, ///< �̴� ���� �Ĺ�
	MON_INSECTIVOROUS_RIGHT	= 52, ///< �Ŵ� ���� �Ĺ� ( ������ )
	MON_INSECTIVOROUS_LEFT	= 53, ///< �Ŵ� ���� �Ĺ� ( ���� )
    MON_WOODPILLAR          = 54, ///< �������
    MON_CRYSTALGEM          = 55, ///< ��������
	MON_SOCCERBALL_PARADOM  = 56, ///< �౸�� �Ķ�
	MON_NEW_CANNON			= 57, ///< ����
	MONSTER_MODEL_NUM,          ///< ���� �� ����

    MON_INVALID         = -1,   ///< invalid monster value
};

/// @enum EGCBackgroundMusic �׷���ü�̽� ��� ����
enum EGCBackgroundMusic
{
    GC_BGM_MAIN_THEME           = 0,    ///< Track0     "���� �׸�"
    GC_BGM_NEW_WORLD            = 1,    ///< Track1     "�ż���"
    GC_BGM_MONSTER_ATTACK       = 2,    ///< Track2     "���� ����"
    GC_BGM_GAME_WIN             = 3,    ///< Track3     "���� �¸�"
    GC_BGM_GAME_LOSE            = 4,    ///< Track4     "���� �й�"
    GC_BGM_LETS_GO              = 5,    ///< Track5     "Let's go!"
    GC_BGM_LEGEND               = 6,    ///< Track6     "����"
    GC_BGM_CASTLE               = 7,    ///< Track7     "��"
    GC_BGM_X_MAS                = 8,    ///< Track8     "ũ��������"
    GC_BGM_MAIN_THEME_LYRICS    = 9,    ///< Track9     "���� �׸� (����)"
    GC_BGM_ANGEL                = 10,   ///< Track10    "õ��"
    GC_BGM_VILLAGE              = 11,   ///< Track11    "����"
    GC_BGM_INTO_THE_DARKNESS    = 12,   ///< Track12    "��� ������"
    GC_BGM_ALLIANCE             = 13,   ///< Track13    "����"
    GC_BGM_GAME_START           = 14,   ///< Track14    "���� ����"
    GC_BGM_LETS_DO_IT           = 15,   ///< Track15    "Let's do it!"
    GC_BGM_GORGOSLAIR           = 16,   ///< Track16    "������ ����"
    GC_BGM_XMAS_EVENT           = 17,   ///< Track17    "XMAS�̺�Ʈ"
	GC_BGM_RUN                  = 18,   ///< Track18    "����"
	GC_BGM_LOGO                 = 19,   ///< Track19    "�ΰ�"
	GC_BGM_QFINAL               = 20,   ///< Track20    "����Ʈ���̳�"
	GC_BGM_JAPAN_LOGO			= 21,	///< Track21	"�Ϻ��ΰ�"
	GC_BGM_FIRST_WAY			= 22,	///< Track22	"�ʺ����Ǳ�"
	GC_BGM_FIGHTING				= 23,	///< Track23	"��!��������"
};
enum EGCDamageTexture
{
	GC_TEX_START				= -1,
	GC_TEX_SWORD_LIGHT			= 0,	///Į�� ��
	GC_TEX_SWORD_FIRE_READY		= 1,	///��ǳ�� �������
	GC_TEX_SWORD_FIRE			= 2,	///��ǳ
	GC_TEX_MEGA_SLASH			= 3,	///�ް� ������
	GC_TEX_WAVE					= 4,	///�ĵ� ȿ��
	GC_TEX_ARROW				= 5,	///ȭ��
	GC_TEX_ARROW_UP				= 6,	///���� �ö󰡴� ȭ��
	GC_TEX_ARROWS				= 7,	///���� ���� ȭ��
	GC_TEX_ARROW_DEFENCE		= 8,	///ȭ�� ����� �� �ߴ� defence����
	GC_TEX_LIGHTNING_ARROW_SET	= 9,	///����ȭ�� ��Ʈ
	GC_TEX_CRITAICAL_ATTACK		= 10,	///critical attack �� �ߴ� ����
	GC_TEX_COUNTER_ATTACK		= 11,	///counter  attack �� �ߴ� ����
	GC_TEX_DOUBLE_ATTACK		= 12,	///double   attack �� �ߴ� ����
	GC_TEX_GORGOS_FIRE			= 13,	///������ ��
	GC_TEX_MAGIC_STAR_COMBO1	= 14,	///������ ���� �� �׸�1
	GC_TEX_MAGIC_STAR_COMBO2	= 15,	///������ ���� �� �׸�2
	GC_TEX_MAGIC_STAR_COMBO3	= 16,	///������ ���� �� �׸�3
	GC_TEX_WITCH_TELEPORT		= 17,	///������ �ڷ���Ʈ ������ �ܻ�
	GC_TEX_WITCH_CRASH_GADOSEN  = 18,	///������ �ڷ���Ʈ ���� �������� ������ ��
	GC_TEX_WITCH_RECHARGE		= 19,	///������ MP���� ��
	GC_TEX_WITCH_JUMPATTACK		= 20,	///������ ���� ����
	GC_TEX_STONE_CURSE			= 21,	///���� Ŀ�� �� �� �����̰� ���մ� ���
	GC_TEX_STONE_BODY			= 22,   ///���� �Ǵ� ��
	GC_TEX_LEFT_LEVER			= 23,	///���� Ŀ���¾��� �� �ߴ� ���� ����
	GC_TEX_RIGHT_LEVER			= 24,   ///���� Ŀ���¾��� �� �ߴ� ���� ����
	GC_TEX_LIGHTNING_BOLT		= 25,   ///����Ʈ�� ��Ʈ 
	GC_TEX_METEO				= 26,   ///���׿�	(������)
	GC_TEX_METEO_FIRE			= 27,	///���׿��� ���� ��	
	GC_TEX_METEO_CRASH_WAVE		= 28,	///���׿� ���� �浹�� �ĵ� ȿ��
	GC_TEX_LIGHTNING_GENERATE	= 29,	///����Ʈ�� ��Ʈ�� �����Ǿ��� �� generateȿ��
	GC_TEX_LIGHTNING_ARROW_UP	= 30,	///���οö󰡴� ����ȭ��
	GC_TEX_REVERSE_GRAVITY		= 31,	///������ �׷���Ƽ
	GC_TEX_CURE_HEART			= 32,   ///ť�� ��Ʈ
	GC_TEX_FIREBALL				= 33,   ///���̾
	GC_TEX_HARPY_FEATHER_ATTACK = 34,	///������ ���� ����
	//GC_TEX_ARCHER_REVENGE		= 35,	///�ü� �������鼭 ��� ȭ��
	GC_TEX_WITCH_GROUND			= 36,	///������ �� ���
	GC_TEX_SKILL_READY_CIRCLE	= 37,	///��� �������� �غ�	
	GC_TEX_POTALBIM				= 38,   ///�Ͱ� ��Ż�� ���
	GC_TEX_POTAL_CIRCLE			= 39,	///�Ͱ� ��Ż�� ��� ������ ���� ��ƼŬ ȿ��
	GC_TEX_DRILLMON_ROCK		= 40,	///�帱���� ����߸��� ��
	GC_TEX_WATER_POLE			= 41,	///������ ���ÿ��� ������ �����
	GC_TEX_PARADOM_MISSILE		= 42,	///�Ķ��� ��� �̻���
	GC_TEX_FORCE_STRIKE			= 43,   ///â�� 1�ܰ� ��� 
	GC_TEX_FIRE_LANCE			= 44,   ///â�� 2�ܰ� ���
	GC_TEX_LIGHTNING_ARROW		= 45,	///����ȭ��
	GC_TEX_ICE_CURSE			= 46,	///���̽� ����Ŀ��
	GC_TEX_KAZEAZE_FIRE			= 47,	///ī������ �ʻ��
	GC_TEX_SNIPER_BOLT			= 48,	///���� Ȱ ū��
	GC_TEX_HARPY_DOWN			= 49,	///���� 3�ܰ輭 ���� �������� ���
	GC_TEX_LIGHTNING_CB_ARROW	= 50,	///���ù���ȭ��
	GC_TEX_BLUE_LIGHTNING		= 51,	///������Ʈ��
	GC_TEX_BLUE_LIGHTNING_GEN	= 52,	///������Ʈ�� generate
	GC_TEX_BOMB					= 53,	///���� ��ź
	GC_TEX_ARCIMIST_GAS			= 54,	///���� ����
	GC_TEX_FLASK				= 55,	///�ö�ũ
	GC_TEX_BLOCK_BUSTER			= 56,	///������ ���������
	GC_TEX_MINE					= 57,	///������ ����
	GC_TEX_DARK_STONECURSE		= 58,	///���� ����Ŀ��
	GC_TEX_DEAD_SOUL			= 59,	///�������� ��ȥ ���� ��
	GC_TEX_ICE_METEO			= 60,	///���̽� ���׿�
	GC_TEX_ICE_WAVE				= 61,	///���̽� ���̺�
	GC_TEX_GAIKOZSLASH			= 62,	///�������� ������
	GC_TEX_WATER_CANNON			= 63,	///����Ÿ ����ĳ��
	GC_TEX_POISON_GAS			= 64,	///������
	GC_TEX_DARK_FIREBALL		= 65,	///�����ĺ�
	GC_TEX_FLYING_IMPACT		= 66,	///â�� �ű�� 1�ܰ� ����Ʈ ������
	GC_TEX_NARANTA_UP			= 67,	///���� �ű�� 1�ܰ� ����Ÿ ��
	GC_TEX_WIND_BLADE			= 68,	///�� 3�ܰ迡 ���� ���� ����Ʈ
	GC_TEX_LAS_JUMP_ATTACK	    = 69,	///�� ���� ���� �� �׸��� �ձ� ����
	GC_TEX_LAS_SPEAR			= 70,	///�� ǥâ. 2�� ���� �� �Ʒ��� ���� �� ����
	GC_TEX_MOTAL_BLOW			= 71,	///��Ż ��ο�
	GC_TEX_LAS_TELEPORT			= 72,	///���� �ڷ���Ʈ
	GC_TEX_NARANTA_DUST			= 73,	///���� �ű�� 1�ܰ� ����Ÿ ����
	GC_TEX_KAMIKI_DARK			= 74,	///ī��Ű ���渶��
	GC_TEX_INVISIBLE			= 75,	///���� �ؽ���!! �����ϴ�. �´� ������ ������δ� ó���� �� �� �� ����.
	GC_TEX_BIG_HAND0			= 76,	///���� �ű�� 1�ܰ� ���ڵ� 1.
	GC_TEX_BIG_HAND1			= 77,	///���� �ű�� 1�ܰ� ���ڵ� 2.
	GC_TEX_BIG_HAND2			= 78,	///���� �ű�� 1�ܰ� ���ڵ� 3.
	GC_TEX_BIG_HAND3			= 79,	///���� �ű�� 1�ܰ� ���ڵ� 4.
	GC_TEX_EGG_HERE				= 80,	///���� HERE
	GC_TEX_WITCH_SHIELD			= 81,	///���� ����
    GC_TEX_DEADLY_SHARP         = 82,   ///������ 3�ܰ� �ʻ��.
    GC_TEX_DASH_EFF             = 83,   ///�ڰ� �뽬 ����Ʈ
	GC_TEX_FRUIT				= 84,	///��Ʈ ����
	GC_TEX_GIGA_SLASH_READY 	= 85,	///�Ⱑ ������ �غ� ����Ʈ
	GC_TEX_SPIN_SHOCK			= 86,	///���ֻ� ȸ�� �����
	GC_TEX_TORNADO_SPIN			= 87,	///����̵� ���̵� ���� �ؽ���
	GC_TEX_TORNADO_IMPACT		= 88,	///����̵� ���̵尡 ������ ����Ű�� ���� ����Ʈ
	GC_TEX_STONE			 	= 89,	///����̵� ���̵� ���� �ڱ�ġ�� ����
	GC_TEX_ENT_LAND				= 90,	///��Ʈ �����ñ� ���̹���
	GC_TEX_EAGLE_SITE			= 91,	///�̱� ����Ʈ (��2)
	GC_TEX_CONFUSION			= 92,	///�������2�ܰ� �¿� Ű�ٲٱ� -- ������
	GC_TEX_SHOCK				= 93,	///�����
	GC_TEX_EXPLOSION_ARROWS     = 94,	///�ͽ��÷��� �ַο� �غ�(��3)
	GC_TEX_EXPLOSION_ARROW		= 95,	///�ͽ��÷��� �ַο� 
	GC_TEX_TORNADO_VOLCANIC2 	= 96,	///����̵� ���̵� ���� �ڱ�ġ�� ���� ���2(���3)
	GC_TEX_TORNADO_VOLCANIC3 	= 97,	///����̵� ���̵� ���� �ڱ�ġ�� ���� ���3
	GC_TEX_CURED_BY_BOOST_HEAL	= 98,	///�ν�Ʈ���� ġ����� ���� ȿ��
	GC_TEX_TORNADO_VOLCANIC1	= 99,	///����̵� ���̵� ���� �ڱ�ġ�� ���� ���1
	GC_TEX_SHOW_ATHENS_SWORD	= 100,	///���׳� �ҿ��� ȿ���� �������� �� �ߴ� ȿ��
	GC_TEX_BLUE_ADOGEN			= 101,  ///�Ķ��� ��ǳ
	GC_TEX_SKULL_BOMB			= 102,  ///�ذ���ź
	GC_TEX_DARK_BALL			= 103,	///��3�� BALL
	GC_TEX_BLOOD_BASH			= 104,  ///���庣�� 
    GC_TEX_GOBLIN_STONE         = 105,  ///��� �������°�.
	GC_TEX_ELESIS3_DUMMY1		= 106,
	GC_TEX_ELESIS3_DUMMY2		= 107,
	GC_TEX_ELESIS3_DUMMY3		= 108,
	GC_TEX_LIRE3_ARROW			= 109,	///����3 �Ŵ��� ȭ��
	GC_TEX_PARADOM_SOCCERBALL	= 110,	///�౸�� �Ķ� �̻���
	GC_TEX_PHOENIX_ARROW_FIRE	= 111,	///����3 �ʻ��3 �������� ȭ�쿡 �ٴ� ����Ʈ
	GC_TEX_PHOENIX_CRUSH1		= 112,	///����3 �ʻ��3 �浹1
	GC_TEX_PHOENIX_CRUSH2		= 113,	///����3 �ʻ��3 �浹2
	GC_TEX_PHOENIX_CRUSH3		= 114,	///����3 �ʻ��3 �浹3
	//GC_TEX_PHOENIX_CRUSH4		= 115,	///����3 �ʻ��3 �浹4
	//GC_TEX_PHOENIX_CRUSH5		= 116,	///����3 �ʻ��3 �浹5
	GC_TEX_RED_CANNON			= 117,	///���� ������
	//							  118
	//							  119
	//							  120
	GC_TEX_LAS3_TRACE			= 121,	///��3 �ܻ�

	GC_DAMAGE_TEX_NUM,
};

enum EGCGameStage
{
	GC_GS_FOREST_OF_ELF     = 0,    // ������ ��
	GC_GS_SWAMP_OF_OBLIVION = 1,    // ������ ��
	GC_GS_FLYING_SHIP       = 2,    // �����
	GC_GS_VALLEY_OF_OATH    = 3,    // �ͼ��� ���
	GC_GS_FOGOTTEN_CITY     = 4,    // ������ ����
	GC_GS_BABEL_OF_X_MAS    = 5,    // X-������ �ٺ�
	GC_GS_TEMPLE_OF_FIRE    = 6,    // ���� ���
	GC_GS_SHOOTING_FIELD    = 7,    // ��� ��ȸ��
	GC_GS_HELL_BRIDGE       = 8,    // �� �긴��
	GC_GS_TEMPLE_OF_ORC     = 9,    // ��ũ ���
	GC_GS_GUILD				= 10,   // ����(������ �� �ܰ�)
	GC_GS_CARRIBEACH		= 11,   // ĳ����ġ

	GC_GS_QUEST1_A          = 12,   // ����Ʈ1 A��
	GC_GS_QUEST1_B          = 13,   // ����Ʈ1 B��
	GC_GS_QUEST1_C          = 14,   // ����Ʈ1 C��
	GC_GS_QUEST1_D          = 15,   // ����Ʈ1 D��

	GC_GS_QUEST1_X_MAS      = 16,   // ����Ʈ1 ũ�������� ��

	GC_GS_QUEST2_A          = 17,   // ����Ʈ2 A��
	GC_GS_QUEST2_B          = 18,   // ����Ʈ2 B��
	GC_GS_QUEST2_C          = 19,   // ����Ʈ2 C��
	GC_GS_QUEST2_D          = 20,   // ����Ʈ2 D��
	GC_GS_QUEST2_E          = 21,   // ����Ʈ2 E��

	GC_GS_QUEST3_A          = 22,   // ����Ʈ3 A��
	GC_GS_QUEST3_B          = 23,   // ����Ʈ3 B��
	GC_GS_QUEST3_C          = 24,   // ����Ʈ3 C��
	GC_GS_QUEST3_D          = 25,   // ����Ʈ3 D��
	GC_GS_QUEST3_E          = 26,   // ����Ʈ3 E��

	GC_GS_QUEST4_A          = 27,   // ����Ʈ4 A��
	GC_GS_QUEST4_B          = 28,   // ����Ʈ4 B��
	GC_GS_QUEST4_C          = 29,   // ����Ʈ4 C��
	GC_GS_QUEST4_D          = 30,   // ����Ʈ4 D��
	GC_GS_QUEST4_E          = 31,   // ����Ʈ4 E��
	GC_GS_QUEST4_F          = 32,   // ����Ʈ4 E��
	GC_GS_QUEST6_A          = 33,   // ����Ʈ FINAL A��
	GC_GS_QUEST6_B          = 34,   // ����Ʈ FINAL B��
	GC_GS_QUEST6_C          = 35,   // ����Ʈ FINAL C��
	GC_GS_QUEST6_D          = 36,   // ����Ʈ FINAL D��
	GC_GS_QUEST6_E          = 37,   // ����Ʈ FINAL E��
	GC_GS_QUEST6_F          = 38,   // ����Ʈ FINAL F��
	GC_GS_QUEST6_G          = 39,   // ����Ʈ FINAL G��

	GC_GS_QUEST_ZERO_A		= 40,
	GC_GS_QUEST_ZERO_B		= 41,
	GC_GS_QUEST_ZERO_C		= 42,

	GC_GS_FOREST_OF_TRIAL   = 43,
    GC_GS_QUEST_ZERO_2_A    = 44,
    GC_GS_QUEST_ZERO_2_B    = 45,
    GC_GS_QUEST_ZERO_2_C    = 46,

	NUM_TOTAL_STAGE,                // ���� �������� ����
};

enum EGCSOLDIER_MOTION
{
    EGCSM_WAIT              = 0,
    EGCSM_WALK              = 1,
    EGCSM_RUN               = 2,
    EGCSM_EVASION           = 3,
    EGCSM_JUMP              = 4,
    EGCSM_DAMAGE            = 5,
    EGCSM_STANDUP           = 6,
    EGCSM_DOWN              = 7,			//�¾Ƽ� �Ѿ����ų� �׾ �Ѿ����� ���...
    EGCSM_JUMPATTACK        = 8,
    EGCSM_ATTACK1           = 9,
    EGCSM_ATTACK2           = 10,
    EGCSM_ATTACK3           = 11,
    EGCSM_ATTACK4           = 12,
    EGCSM_ATTACK5           = 13,
    EGCSM_SAN_DIE           = 14,
	EGCSM_KAMIKI_RISE       = 15,
	EGCSM_CASTLEDOOR_OPEN   = 16,		//������ īŰ�繮 �Ѵ� 16�� ����մϴ�.
	EGCSM_TRANS_KAZEAZE_DIE = 17,
	EGCSM_SLIM_DIE			= 18,

	MONSTER_MOTION_NUM      = 19,
};

enum PET_MOTION
{
    PM_FLY1          = 0,
	PM_FLY2,
	PM_FLY3,
	PM_FLY4,
	PM_MASTER_ATTACK,
	PM_MASTER_ATTACKED,
	PM_MAGIC1,
	PM_MAGIC2,
	PM_MAGIC3,
	PM_MAGIC4,
	PM_MAGIC5,
	PM_END,
};

enum EGCCharType
{
    GC_CHAR_ELESIS = 0,     ///< �����ý�
    GC_CHAR_LIRE,			///< ����
    GC_CHAR_ARME,			///< �Ƹ���
	GC_CHAR_LAS,			///< ��
	GC_CHAR_RYAN,			///< ���̾�

    GC_CHAR_NUM,
};

enum EGCGrade
{
	GC_GRADE_VAGABOND = 0,
	GC_GRADE_MERCENARY,
	GC_GRADE_ROYAL,
	GC_GRADE_HOLY,
	
	GC_GRADE_NUM,
};

enum CHAR_IDENTY
{
	CID_COMMON  = -1,//����Ʈ�� ����
	CID_KNIGHT  = 0,//���
	CID_ARCHER,     //�ü�
	CID_WITCH,      //����
	CID_THIEF,		//����
	CID_DRUID,		//���̾�-����̵�

	CID_LANCER,     //â��
	CID_SNIPER,     //���ú�
	CID_ARCHIMIST,  //���ݼ���
    CID_HASSASSIN,  //�ڰ�
	CID_DRUID_1ST,	//���̾� - ����

	CID_SWORD_MASTER,	// �����ý� 2������ �ҵ帶����
	CID_LIRE3,			// ���� 2������
	CID_WARLOCK,		// �Ƹ��� 2������ ����
	CID_DASSASSIN,		// �� 2������ ��ũ��ؽ�
	CID_DRUID_2ST,	//���̾� - ����
};

enum EGCGameItemType
{
    GC_GAME_ITEM_HPUP       = 0,    ///< ü�º���
    GC_GAME_ITEM_SUPER      = 1,    ///< ����
    GC_GAME_ITEM_WING       = 2,    ///< õ���� ����
    GC_GAME_ITEM_HALLOW     = 3,    ///< ����
    GC_GAME_ITEM_REFLECTION = 4,    ///< �ݻ�
    GC_GAME_ITEM_MPUP       = 5,    ///< ��º���
    GC_GAME_ITEM_GP         = 6,    ///< ���ӸӴ�	
	GC_GAME_ITEM_HASTE      = 7,    ///< ���̽�Ʈ
    GC_GAME_ITEM_BONUS      = 8,    ///< ���ʽ�
	GC_GAME_ITEM_CHICKEN    = 9,    ///< �߰��
    GC_GAME_ITEM_GRAPE      = 10,   ///< ����
    GC_GAME_ITEM_WATERMELON = 11,   ///< ����
    GC_GAME_ITEM_SANTA_GIFT = 12,   ///< ��Ÿ����
    GC_GAME_ITEM_RICECAKE   = 13,   ///< ����
	GC_GAME_ITEM_EGGCOOKIE	= 14,	///< �������
	GC_GAME_ITEM_PIGHEAD	= 15,	///< �����Ӹ�
    GC_GAME_ITEM_NUM,
};

enum STATE_BLOCK_ID
{
	DISABLE_STATE_ID  = -1,
	SB_INIT_STATE = 0,
	PARTICLE_STATE_ID,
};

enum KT_TIMER_ID
{
    KTIMER_QUEST_STAGE_FIRE1 = 0,
    KTIMER_QUEST_STAGE_FIRE2,
    KTIMER_QUEST_STAGE_FIRE3,
	KTIMER_QUEST2_STAGE4_FIRE1,
	KTIMER_QUEST2_STAGE4_FIRE2,
	KTIMER_QUEST3_STAGE3_FIRE1,
	KTIMER_QUEST3_STAGE4_FIRE1,
	KTIMER_QUEST3_STAGE4_FIRE2,
	KTIMER_QUEST3_STAGE4_FIRE3,
	KTIMER_QUEST3_STAGE5_FIRE1,
	

    KTIMER_MONSTER_AWAKE,
    KTIMER_EARTHQUAKE_DELAY,
};


enum GC_MON_EXTRA_TEX
{
	GMET_NO_USE = -1,
	GMET_MONGBANA,
	GMET_MONGBANB,
	GMET_PATUSEI,
	
	GMET_MIDNIGHT_REDANT,
	GMET_MIDNIGHT_BLUEANT,
	GMET_MIDNIGHT_GORGOS,
	GMET_MIDNIGHT_TROLL,
	GMET_MIDNIGHT_REDGORGOS,
	GMET_MIDNIGHT_KAIDO,
	GMET_MIDNIGHT_MONGBAN,
	GMET_MIDNIGHT_GAIKOZ,
	GMET_MIDNIGHT_LAPSTER,
	GMET_MIDNIGHT_GHOSTFISH,

	GMET_MIDNIGHT_BASILISK,

	GMET_TOTAL_NUM,
};

enum MOTIONID
{
	MID_ELESIS_WAIT = 0,				//0//�����ý�//�⺻�ڼ�
	MID_ELESIS_WALK,					//1//�����ý�//�ȱ�
	MID_ELESIS_JUMPREADY,				//2//�����ý�//�����غ�
	MID_ELESIS_JUMP,					//3//�����ý�//����
	MID_ELESIS_ATK1_1,					//4//�����ý�//�⺻����1_1
	MID_ELESIS_ATK1_2,					//5//�����ý�//�⺻����1_2
	MID_ELESIS_ATK1_3,					//6//�����ý�//�⺻����1_3
	MID_ELESIS_ATK1_4,					//7//�����ý�//�⺻����1_4
	MID_ELESIS_ATK1_5,					//8//�����ý�//�⺻����1_5
	MID_COMMON_SMALLDAMAGE,				//9//����//���ϰ� �¾�����
	MID_COMMON_BIGDAMAGE,				//10//����//���ϰ� �¾�����
	MID_COMMON_DOWN_AND_STANDUP,		//11//����//�Ѿ����� �Ͼ��
	MID_ELESIS_DASH,					//12//�����ý�//����
	MID_ELESIS_DASHATK,					//13//�����ý�//��������
	MID_ELESIS_JUMPATK,					//14//�����ý�//��������
	MID_ELESIS_SPECIAL1,				//15//�����ý�//�ʻ��1
	MID_ELESIS_SPECIAL2,				//16//�����ý�//�ʻ��2
	MID_ELESIS_SPECIAL3,				//17//�����ý�//�ʻ��3
	MID_COMMON_DOWN_IN_SKY,				//18//����//���߿��� �¾Ƽ� ��������.. �������� 11������ �����
	MID_ELESIS_WINPOS,					//19//�����ý�//�¸�����
	MID_LIRE_LEGATK,					//20//����//�ٸ����
	MID_COMMON_ARENA,					//21//����//�Ʒ���
	MID_COMMON_DROP,					//22//����//����
	MID_ELESIS_START,					//23//�����ý�//����
	MID_LIRE_WAIT,						//24//����//�⺻�ڼ�
	MID_LIRE_WALK,						//25//����//�ȱ�
	MID_LIRE_DASH,						//26//����//����
	MID_LIRE_JUMPREADY,					//27//����//�����غ�
	MID_LIRE_JUMP,						//28//����//����
	MID_LIRE_DOUBLEJUMP,				//29//����//2������
	MID_LIRE_ATK1_1,					//30//����//�⺻����1_1
	MID_LIRE_ATK1_2,					//31//����//�⺻����1_2
	MID_LIRE_ATK1_3,					//32//����//�⺻����1_3
	MID_LIRE_ATK1_4,					//33//����//�⺻����1_4
	MID_LIRE_ATK1_5,					//34//����//�⺻����1_5
	MID_LIRE_SPECIAL2,					//35//����//�ʻ��2
	MID_LIRE_SPECIAL3,					//36//����//�ʻ��3
	MID_LIRE_SPECIAL1,					//37//����//�ʻ��1
	MID_LIRE_JUMPATK,					//38//����//��������
	MID_ELE_ELE2_ARROWDEF_ON_GROUND,	//39//����,����(2)//ȭ�츷��(����1)
	MID_ELE_ELE2_ARROWDEF_IN_SKY,		//40//����,����(2)//ȭ�츷��(����)
	MID_LIRE_DASHATK,					//41//����//��������
	MID_ELE_ELE2_ARROWDEF_ON_GROUND2,	//42//����,����(2)//ȭ�츷��(����2)
	MID_COMMON_FRONTDOWN_AND_STANDUP,	//43//����//�ڷγ��ư��� ������ �������� �Ͼ��
	MID_LIRE_START,						//44//����//����
	MID_ARME_EARTHQUAKE,				//45//�Ƹ���//����� ����(�Ƹ��� �޺��� UpKey)
	MID_LIRE_WINPOS,					//46//����//�¸�����
	MID_ARME_WAIT_AND_FATAL,			//47//�Ƹ���//�⺻�ڼ�,�⺻�ڼ�(Fatal)
	MID_ARME_START,						//48//�Ƹ���//����
	MID_ARME_WALK,						//49//�Ƹ���//�ȱ�
	MID_ARME_ATK1_1,					//50//�Ƹ���//�⺻����1_1
	MID_ARME_ATK1_2,					//51//�Ƹ���//�⺻����1_2
	MID_ARME_ATK1_3,					//52//�Ƹ���//�⺻����1_3
	MID_ARME_ATK1_4,					//53//�Ƹ���//�⺻����1_4			
	MID_ARME_ARME2_DASH,				//54//�Ƹ�,�Ƹ�(2)//����(�����̵�)
	MID_ARME_JUMPREADY,					//55//�Ƹ���//�����غ�
	MID_ARME_JUMP,						//56//�Ƹ���//����
	MID_ARME_JUMPATK,					//57//�Ƹ���//��������
	MID_ARME_SPECIAL1_BLACK,			//58//�Ƹ���//�渶��1�ܰ�
	MID_COMMON_STONECURSE,				//59//����//����Ŀ��
	MID_ARME_SPECIAL2_BLACK,			//60//�Ƹ���//�渶��2�ܰ�
	MID_ARME_SPECIAL3_BLACK,			//61//�Ƹ���//�渶��3�ܰ�
	MID_ARME_SPECIAL1,					//62//�Ƹ���//�ʻ��1
	MID_ARME_SPECIAL2,					//63//�Ƹ���//�ʻ��2
	MID_COMMON_DAMAGED_BY_FLYATK,		//64//����//���� ������ �¾�����
	MID_ARME_SPECIAL3,					//65//�Ƹ���//�ʻ��3
	MID_ARME_DASHATK,					//66//�Ƹ���//��������
	MID_ARME_WINPOS,					//67//�Ƹ���//�¸�����
	MID_ELESIS_CATCH,					//68//�����ý�//���
	MID_COMMON_CATCH_BY_ELESIS,			//69//����//�����ֱ�
	MID_LIRE_CATCH,						//70//����//���
	MID_COMMON_CATCH_BY_LIRE,			//71//����//�����ֱ�
	MID_ARME_CATCH,						//72//�Ƹ���//���
	MID_COMMON_CATCH_BY_ARME,			//73//����//�����ֱ�
	MID_ELESIS_STANDUP_AND_ATK,			//74//�����ý�//�Ͼ�鼭 ġ��
	MID_LIRE_ARME_ARME2_STANDUP_AND_ATK,//75//����,�Ƹ���,�Ƹ���(2)//�Ͼ�鼭 ġ��
	MID_ELESIS_RUN,						//76//�����ý� ���Option:134//�޸���
	MID_ELESIS_SELECT,					//77//����//����
	MID_LIRE_SELECT,					//78//����//����
	MID_ARME_SELECT,					//79//�Ƹ���//����
	MID_ELESIS_FLYATK,					//80//�����ý�//�޺��� UpKey�����顦��븦 ����
	MID_NOUSE_81,						//81//������(SLOT)//XXXXXXXXXXXXXXX
	MID_NOUSE_82,						//82//������(SLOT)//XXXXXXXXXXXXXXX
	MID_NOUSE_83,						//83//������(SLOT)//XXXXXXXXXXXXXXX
	MID_ELE_ELE2_RESULT_WIN,			//84//����,����(2)//���â �ȿ��� �̰��� �� �׷����� ���
	MID_LIRE_LIRE2_RESULT_WIN,			//85//����,����(2)//���â �ȿ��� �̰��� �� �׷����� ���
	MID_ARME_ARME2_RESULT_WIN,			//86//�Ƹ�,�Ƹ�(2)//���â �ȿ��� �̰��� �� �׷����� ���
	MID_ELE_ELE2_RESULT_LOSE,			//87//����,����(2)//���â �ȿ��� ���� �� �׷����� ���
	MID_LIRE_LIRE2_RESULT_LOSE,			//88//����,����(2)//���â �ȿ��� ���� �� �׷����� ���
	MID_ARME_ARME2_RESULT_LOSE,			//89//�Ƹ�,�Ƹ�(2)//���â �ȿ��� ���� �� �׷����� ���
	MID_COMMON_SHOPPOS,					//90//����//�޿� ������ ĳ���� ���
	MID_COMMON_INTO_THE_TELEPORT,		//91//����//�����̵�����
	MID_ELESIS_ARROW_REFLECTION,		//92//�����ý�//ȭ�츷��(�ĳ��� ���)
	MID_ELESIS2_SELECT,					//93//����(2)//����
	MID_ELESIS2_WAIT,					//94//����(2)//�⺻�ڼ�
	MID_ELESIS2_WALK,					//95//����(2)//�ȱ�
	MID_ELESIS2_DASH,					//96//����(2)//����
	MID_ELESIS2_JUMP,					//97//����(2)//����
	MID_ELESIS2_JUMPREADY,				//98//����(2)//�����غ�
	MID_ELESIS2_JUMPATK,				//99//����(2)//��������
	MID_ELESIS2_DASHATK,				//100//����(2)//��������
	MID_ELESIS2_ATK1_1,					//101//����(2)//�⺻����1_1
	MID_ELESIS2_ATK1_2,					//102//����(2)//�⺻����1_2
	MID_ELESIS2_ATK1_3,					//103//����(2)//�⺻����1_3
	MID_ELESIS2_ATK1_4,					//104//����(2)//�⺻����1_4
	MID_ELESIS2_ATK1_5,					//105//����(2)//�⺻����1_5
	MID_ELESIS2_FASTATK_ON_COMBO1,		//106//����(2)//Z��Ÿ�� �ſ춧���� ����
	MID_ELESIS2_START,					//107//����(2)//����
	MID_NOUSE_108,						//108//������(SLOT)//XXXXXXXXXXXXXXX
	MID_ELESIS2_SPECIAL1,				//109//����(2)//�ʻ��1
	MID_ELESIS2_SPECIAL2,				//110//����(2)//�ʻ��2
	MID_ELESIS2_SPECIAL3,				//111//����(2)//�ʻ��3
	MID_ELESIS2_CATCH,					//112//����(2)//���
	MID_COMMON_CATCH_BY_ELESIS2,		//113//����//�����ֱ�
	MID_ELESIS2_STANDUP_AND_ATK,		//114//����(2)//�Ͼ�鼭 ġ��
	MID_ELESIS2_WINPOS,					//115//����(2)//�¸�����
	MID_ELE_ELE2_FATAL,					//116//����,����(2)//�⺻�ڼ�(Fatal)
	MID_LIRE_LIRE2_FATAL,				//117//����,����(2)//�⺻�ڼ�(Fatal)
	MID_ELE_ELE2_JOKE,					//118//����,����(2)//����
	MID_LIRE_LIRE2_JOKE,				//119//����,����(2)//����
	MID_ARME_ARME2_JOKE,				//120//�Ƹ�,�Ƹ�(2)//����
	MID_LIRE2_WAIT,						//121//����(2)//�⺻�ڼ�
	MID_LIRE2_WALK,						//122//����(2)//�ȱ�
	MID_LIRE2_DASH,						//123//����(2)//����
	MID_NOUSE_124,						//124//������(SLOT)//XXXXXXXXXXXXXXX
	MID_LIRE2_SELECT,					//125//����(2)//����
	MID_LIRE2_START,					//126//����(2)//����
	MID_LIRE2_DASHATK,					//127//����(2)//��������
	MID_LIRE2_JUMPREADY,				//128//����(2)//�����غ�
	MID_LIRE2_JUMP,						//129//����(2)//����
	MID_LIRE2_ATK1_1,					//130//����(2)//�⺻����1_1(���� ����)
	MID_LIRE2_ATK1_2,					//131//����(2)//�⺻����1_2
	MID_LIRE2_ATK1_3,					//132//����(2)//�⺻����1_3
	MID_LIRE2_ATK1_4,					//133//����(2)//�⺻����1_4
	MID_LIRE2_ATK1_5,					//134//����(2)//�⺻����1_5
	MID_LIRE2_ATK2_1,					//135//����(2)//�⺻����2_1(��� ����)
	MID_LIRE2_ATK2_2,					//136//����(2)//�⺻����2_2
	MID_LIRE2_ATK2_3,					//137//����(2)//�⺻����2_3
	MID_LIRE2_ATK2_4,					//138//����(2)//�⺻����2_4
	MID_LIRE2_ATK2_5,					//139//����(2)//�⺻����2_5
	MID_LIRE2_ATK3_1,					//140//����(2)//�⺻����3_1(�Ϲ� ����)
	MID_LIRE2_ATK3_2,					//141//����(2)//�⺻����3_2
	MID_LIRE2_ATK3_3,					//142//����(2)//�⺻����3_3
	MID_LIRE2_ATK3_4,					//143//����(2)//�⺻����3_4
	MID_LIRE2_ATK3_5,					//144//����(2)//�⺻����3_5
	MID_LIRE2_JUMPATK,					//145//����(2)//��������
	MID_LIRE2_ATK1_STOP,				//146//����(2)//�⺻����1_�޺� �ߴ�
	MID_LIRE2_ATK2_STOP,				//147//����(2)//�⺻����2_�޺� �ߴ�
	MID_LIRE2_ATK3_STOP,				//148//����(2)//�⺻����3_�޺� �ߴ�
	MID_LIRE2_CATCH,					//149//����(2)//���
	MID_COMMON_CATCH_BY_LIRE2,			//150//����//�����ֱ�
	MID_LIRE2_STANDUP_AND_ATK,			//151//����(2)//�Ͼ�鼭 ġ��
	MID_LIRE2_SPECIAL1_DOWN,			//152//����(2)//�ʻ��1(�Ϲ�)
	MID_LIRE2_SPECIAL1_UP,				//153//����(2)//�ʻ��1(���)
	MID_LIRE2_SPECIAL3,					//154//����(2)//�ʻ��3
	MID_LIRE2_SPECIAL2,					//155//����(2)//�ʻ��2
	MID_LIRE2_JUMPPING_DOWN_ARROW_ATK,	//156//����(2)//���߿��� ������ ȭ����
	MID_LIRE2_LAND_IN_DOWN_ARROW_ATK,	//157//����(2)//156�������� ����
	MID_LIRE2_WINPOS,					//158//����(2)//�¸�����
	MID_ELESIS123_BERSERK,				//159//����,����(2)//����ũ!!!
	MID_ARME2_WAIT_AND_FATAL,			//160//�Ƹ���(2)//�⺻�ڼ�,Fatal
	MID_ARME2_WALK,						//161//�Ƹ���(2)//�ȱ�
	MID_ARME2_DASHATK,					//162//�Ƹ���(2)//��������
	MID_ARME2_ATK1_1,					//163//�Ƹ���(2)//�⺻����1(��ź����)
	MID_ARME2_ATK1_2,					//164//�Ƹ���(2)//�⺻����1(��ź����)
	MID_ARME2_ATK1_3,					//165//�Ƹ���(2)//�⺻����1(��ź����)
	MID_ARME2_ATK1_4,					//166//�Ƹ���(2)//�⺻����1(��ź����)
	MID_ARME2_ATK1_5,					//167//�Ƹ���(2)//�⺻����1(��ź����)
	MID_ARME2_JUMPREADY,				//168//�Ƹ���(2)//�����غ�
	MID_ARME2_JUMP,						//169//�Ƹ���(2)//����
	MID_ARME2_JUMPATK,					//170//�Ƹ���(2)//��������
	MID_ARME2_SPECIAL2_BLACK,			//171//�Ƹ���(2)//�渶��2�ܰ�
	MID_ARME2_SPECIAL3,					//172//�Ƹ���(2)//�ʻ��3
	MID_ARME2_SPECIAL3_BLACK,			//173//�Ƹ���(2)//�渶��3�ܰ�
	MID_ARME2_SPECIAL1_BLACK,			//174//�Ƹ���(2)//�渶��1�ܰ�
	MID_ARME2_CATCH,					//175//�Ƹ���(2)//���
	MID_COMMON_CATCH_BY_ARME2,			//176//����//�����ֱ�
	MID_ARME2_SPECIAL1,					//177//�Ƹ���(2)//�ʻ��1
	MID_ARME2_SPECIAL2,					//178//�Ƹ���(2)//�ʻ��2
	MID_NOUSE_179,						//179//������(SLOT)//XXXXXXXXXXXXXXX
	MID_ARME2_SELECT,					//180//�Ƹ���(2)//����
	MID_ARME2_START,					//181//�Ƹ���(2)//����(�׾Ƹ����� ���´�)
	MID_ARME2_WINPOS,					//182//�Ƹ���(2)//�¸�����
	MID_ARME_ARME2_PUSHED_BY_BOSSMONSTER,//183//�Ƹ���,�Ƹ���(2)//DASH�߿� �������� �и��� ���..
	MID_COMMON_INTROPOS,				//184//����//���� ��Ÿ�ÿ� ������ ĳ���� ���
	MID_COMMON_TIGER_ROLLING_ACTION,	//185//����//Ÿ�̰ŷѸ��׼�
	MID_COMMON_INTO_BASILISK,			//186//����//�ٽǿ��� ������
//=================================================================================================//
	MID_LAS_WAIT,						//187//��//�⺻�ڼ�
	MID_LAS_FATAL,						//188//��//�⺻�ڼ�(Fatal)
	MID_LAS_WALK,						//189//��//�ȱ�
	MID_LAS_DASH,						//190//��//�뽬
	MID_LAS_RUN,						//191//��//�޸���
	MID_LAS_JUMPREADY,					//192//��//�����غ�
	MID_LAS_JUMP,						//193//��//����
	MID_LAS_JUMPATK,					//194//��//��������
	MID_LAS_DOUBLEJUMP,					//195//��//2������
	MID_LAS_DOUBLEJUMP_ATK,				//196//��//2����������
	MID_LAS_START,						//197//��//����
	MID_LAS_ATK1_1,						//198//��//�⺻����1_1
	MID_LAS_FASTATK_ON_COMBO1,			//199//��//�⺻���� ������ ��Ÿ
	MID_LAS_DASHATK1,					//200//��//�뽬����1_1
	MID_LAS_DASHATK2,					//201//��//�뽬�޺�1_2
	MID_LAS_DASHATK3,					//202//��//�뽬�޺�1_3
	MID_LAS_DASHATK4,					//203//��//�뽬�޺�1_4
	MID_LAS_WINPOS,						//204//��//�¸�����
	MID_LAS_STANDUP_AND_ATK,			//205//��//�Ͼ�鼭 ����
	MID_LAS_ARROWDEF_ON_GROUND1,		//206//��//ȭ�츷��1
	MID_LAS_ARROWDEF_ON_GROUND2,		//207//��//ȭ�츷��2
	MID_LAS_ARROWDEF_IN_SKY,			//208//��//����ȭ�츷��
	MID_LAS_SPECIAL1,					//209//��//�ʻ��1
	MID_LAS_SPECIAL2,					//210//��//�ʻ��2
	MID_LAS_SPECIAL3,					//211//��//�ʻ��3
	MID_LAS_EVASION_ATK,				//212//��//ȸ�ǰ���
	MID_LAS_JOKE,						//213//��//����
	MID_LAS_CATCH,						//214//��//���
	MID_COMMON_CATCH_BY_LAS,			//215//����//�󽺿��� ������
	MID_LAS_SELECT,						//216//��//����
	MID_LAS_RESULT_WIN,					//217//��//���â �¸�����
	MID_LAS_RESULT_LOSE,				//218//��//���â �й�����
	MID_LAS_DOUBLEJUMP_LAND,			//219//��//2������ ����
	MID_LAS_CRITICAL_ATK,				//220//��//ũ��Ƽ�� ����
	MID_LAS_DOUBLE_ATK,					//221//��//���� ����
	MID_LAS_SPECIAL2_FAIL,				//222//��//��Ż��ο� ����
	MID_LAS_EVASION,					//223//��//ȸ�ǰ���
	MID_LAS_ATK1_2,						//224//��//�⺻����1_2
	MID_LAS_ATK1_3,						//225//��//�⺻����1_3
//========================================================================================
//                                      ��    ��
//========================================================================================
	MID_ELESIS_SPECIAL_MOONBLADE,		//226//�����ý�//�ʻ��.. �����̵�....
	MID_ARME_SPECIAL_SHINING_ARROW,		//227//�Ƹ���//�ʻ��..���̴׾ַο�...
	MID_ELESIS2_SPECIAL_FLYING_IMPACT,  //228//�����ý�2//�ʻ�� �ö��� ����Ʈ
	MID_ARME2_SPECIAL_BIG_HAND,			//229//�Ƹ���2//�ʻ�� ���ڵ�
	MID_LIRE_WIND_WARD,					//230//����//�ʻ�� �������
	MID_LIRE2_NARANTA,					//231//����2//�ʻ�� ����Ÿ
//========================================================================================
//                                      �� ����
//========================================================================================
    MID_LAS2_WAIT,                      //232//��2//�⺻�ڼ�
    MID_LAS2_FATAL,                     //233//��2//�⺻�ڼ�(����Ż)
    MID_LAS2_WALK,                      //234//��2//�ȱ�
    MID_LAS2_DASH_1,                    //235//��2//�뽬1
    MID_LAS2_DASH_2,                    //236//��2//�뽬2
    MID_LAS2_JUMPREADY,                 //237//��2//�����غ�
    MID_LAS2_JUMP,                      //238//��2//����
    MID_LAS2_JUMPATK,                   //239//��2//��������
    MID_LAS2_START,                     //240//��2//����
    MID_LAS2_ATK1_1,                    //241//��2//�⺻�޺�1
    MID_LAS2_ATK1_2,                    //242//��2//�⺻�޺�2
    MID_LAS2_ATK1_3,                    //243//��2//�⺻�޺�3
    MID_LAS2_ATK1_4,                    //244//��2//�⺻�޺�4
    MID_LAS2_RESULT_WIN,                //245//��2//���â �¸� ����
    MID_LAS2_RESULT_LOSE,               //246//��2//���â �й� ����
    MID_LAS2_JUMPDASH,                  //247//��2//�����뽬
    MID_LAS2_JUMPDASH_ATK,              //248//��2//�����뽬 ����
    MID_LAS2_ARROW_EVASION_SKY,         //249//��2//���� ȭ�� ȸ��
    MID_LAS2_STANDUP_AND_ATK,           //250//��2//��� ����
    MID_LAS2_SELECT,                    //251//��2//����
    MID_LAS2_DASH_ATK,                  //252//��2//�뽬����
    MID_LAS2_JOKE,                      //253//��2//����
    MID_LAS2_WINPOS,                    //254//��2//�¸�����
    MID_LAS2_CATCH,                     //255//��2//���
    MID_COMMON_CATCH_BY_LAS2,           //256//����//��2�� ������
    MID_LAS2_ARROWDEF_IN_SKY,           //257//��2//���� ȭ�� ����
    MID_LAS2_ARROWDEF_ON_GROUND1,       //258//��2//���� ȭ�� ����1
    MID_LAS2_ARROWDEF_ON_GROUND2,       //259//��2//���� ȭ�� ����2
    MID_LAS2_ARROW_EVASION_GROUND,      //260//��2//���� ȭ�� ȸ��
    MID_LAS2_SPECIAL1,                  //261//��2//�ʻ��1
    MID_LAS2_SPECIAL2,                  //262//��2//�ʻ��2
    MID_LAS2_SPECIAL3,                  //263//��2//�ʻ��3
    MID_LAS2_CRITICAL_ATK,              //264//��2//ũ��Ƽ�� ����
    MID_LAS2_DOUBLE_ATK,                //265//��2//�������
//====================================================================================//
	MID_ELESIS_GIGA_SLASH,				//266//�����ý�//�ʻ�� �Ⱑ ������
	MID_ELESIS_TORNADO_BLADE,			//267//�����ý�//�ʻ�� ����̵� ���̵�
	MID_LIRE_EAGLE_SITE,				//268//����//�̱� ����Ʈ
	MID_LIRE_EXPLOSION_ARROW,			//269//����//�ͽ��÷��� �ַο�
	MID_ARME_SPECIAL_CONFUSION,			//270//�Ƹ���//��ǻ��
	MID_ARME_LAVA_FLOW,					//271//�Ƹ���//����÷ο�
	MID_ARME_BOOST_HEAL,				//272//�Ƹ���//�ν�Ʈ ��
	MID_ARME_DARK_SWORM,				//273//�Ƹ���//��ũ ����
	MID_COMMON_ENT_TRUNK,				//274//����//��Ʈ�ٱ⿡ ������
	MID_ARME_ATHENS_SWORD,				//275//�Ƹ���//���׳� �ҿ���
	MID_LAS_DOUBLE_SLASH,				//276//��//��������
	MID_LAS_BLOOD_BASH,					//277//��//����转(���ڵ�ȸ���ȵ�)
	MID_LAS_GUARDIAN_BALL,				//278//��//��ũ��(�þ�������)
//================================2�� ���� ����=======================================//
	MID_ELESIS3_WAIT,					//279//����(3)//�⺻�ڼ�
	MID_ELESIS3_FATAL,					//280//����(3)//�⺻�ڼ�(����Ż)
	MID_ELESIS3_WALK,					//281//����(3)//�ȱ�
	MID_ELESIS3_JOKE,					//282//����(3)//����
	MID_ELESIS3_DASH,					//283//����(3)//�뽬
	MID_ELESIS3_RUN,					//284//����(3)//�޸���
	MID_ELESIS3_DASHATK,				//285//����(3)//�뽬 ����
	MID_ELESIS3_JUMPREADY,				//286//����(3)//�����غ�
	MID_ELESIS3_JUMP,					//287//����(3)//����
	MID_ELESIS3_JUMPATK,				//288//����(3)//��������_�븻
	MID_ELESIS3_JUMPATK_UP,				//289//����(3)//��������_�购���� ����
	MID_ELESIS3_JUMPATK_DOWN,			//290//����(3)//��������_�鴭���� ����
	MID_ELESIS3_ATK1_1,					//291//����(3)//�⺻�޺�1-1
	MID_ELESIS3_ATK1_2,					//292//����(3)//�⺻�޺�1-2
	MID_ELESIS3_ATK1_3,					//293//����(3)//�⺻�޺�1-3
	MID_ELESIS3_SHADOW_ATK,				//294//����(3)//�׸��� ����
	MID_ELESIS3_CRITICAL_ATK,			//295//����(3)//ũ��Ƽ�� ����
	MID_ELESIS3_DOUBLE_ATK,				//296//����(3)//���� ����
	MID_ELESIS3_CATCH,					//297//����(3)//���
	MID_COMMON_CATCH_BY_ELE3,			//298//����	//����(3)���� ������
	MID_ELESIS3_STANDUP_AND_ATK,		//299//����(3)//��� ����
	MID_ELESIS3_SPECIAL1,				//300//����(3)//�ʻ��1
	MID_ELESIS3_SPECIAL2,				//301//����(3)//�ʻ��2
	MID_ELESIS3_SPECIAL3,				//302//����(3)//�ʻ��3
	MID_ELESIS3_START,					//303//����(3)//����
	MID_ELESIS3_WINPOS,					//304//����(3)//�¸�����
	MID_ELESIS3_SELECT,					//305//����(3)//����
	MID_ELESIS3_RESULT_WIN,				//306//����(3)//���â �¸� ����
	MID_ELESIS3_RESULT_LOSE,			//307//����(3)//���â �й� ����
	MID_ELESIS3_ARROWDEF_ON_GROUND,		//308//����(3)//ȭ�� ����1
	MID_ELESIS3_ARROWDEF_ON_GROUND2,	//309//����(3)//ȭ�� ����2
	MID_ELESIS3_ARROWDEF_IN_SKY,		//310//����(3)//���߿��� ȭ�� ����
	MID_ELESIS3_ARROW_REFLECTION,		//311//����(3)//ȭ�� �ĳ���
	MID_ELESIS3_JUMPATK_DOWN_LAND,		//312//����(3)//�������� ��� ������ ��������!
//================================���� 2�� ����=======================================//
	MID_LIRE3_WAIT,						//313//����(3)//�⺻�ڼ�
	MID_LIRE3_WALK,						//314//����(3)//�ȱ�
	MID_LIRE3_DASH,						//315//����(3)//�뽬
	MID_LIRE3_RUN,						//316//����(3)//�޸���
	MID_LIRE3_DASHATK,					//317//����(3)//�뽬 ����
	MID_LIRE3_JUMPREADY,				//318//����(3)//�����غ�
	MID_LIRE3_JUMP,						//319//����(3)//����
	MID_LIRE3_ATK1_1,					//320//����(3)//�⺻�޺�1-1
	MID_LIRE3_ATK1_2,					//321//����(3)//�⺻�޺�1-2
	MID_LIRE3_ATK1_3,					//322//����(3)//�⺻�޺�1-3
	MID_LIRE3_MELEEATK,					//323//����(3)//��������
	MID_LIRE3_JUMPATK1,					//324//����(3)//��������01
	MID_LIRE3_JUMPATK2,					//325//����(3)//��������02
	MID_LIRE3_LANDING,					//326//����(3)//���� �� ����
	MID_LIRE3_CATCH,					//327//����(3)//���
	MID_COMMON_CATCH_BY_LIRE3,			//328//����//����(3)���� ������
	MID_LIRE3_SPECIAL1,					//329//����(3)//�ʻ��1
	MID_LIRE3_SPECIAL2,					//330//����(3)//�ʻ��2
	MID_LIRE3_SPECIAL3,					//331//����(3)//�ʻ��3
	MID_LIRE3_JOKE,						//332//����(3)//����
	MID_LIRE3_SELECT,					//333//����(3)//����
	MID_LIRE3_START,					//334//����(3)//����
	MID_LIRE3_WINPOS,					//335//����(3)//����
	MID_LIRE3_RESULT_LOSE,				//336//����(3)//���â�й�
	MID_LIRE3_RESULT_WIN,				//337//����(3)//���â�¸�	
	MID_LIRE3_FATAL,					//338//����(3)//�⺻�ڼ�(����Ż)
	MID_LIRE3_JUMPATK3,					//339//����(3)//��������03
	MID_LIRE3_DASHATK2,					//340//����(3)//�뽬����02	
	MID_LIRE3_CRITICALATK,				//341//����(3)//ũ��Ƽ��ATK
	MID_LIRE3_DOUBLEATK,				//342//����(3)//����ATK	
	MID_LIRE3_CRITICAL_MELEEATK,		//343//����(3)//ũ��Ƽ�ùи�����
	MID_LIRE3_CRITICAL_DASHATK2,		//344//����(3)//ũ��Ƽ�ô뽬����02
	MID_LIRE3_JUMP_DASH,				//345//����(3)//�����뽬
//================================���� 2�� ���� ��=======================================//
//================================�Ƹ��� 2�� ����=======================================//
	MID_ARME3_WAIT,						//346//�Ƹ���(3)//�⺻�ڼ�
	MID_ARME3_FATAL,					//347//�Ƹ���(3)//����Ż
	MID_ARME3_WALK,						//348//�Ƹ���(3)//�ȱ�
	MID_NOUSE_349,						//349//������(SLOT)--XXXXXXXXXXXXXXX
	MID_NOUSE_350,						//350//������(SLOT)--XXXXXXXXXXXXXXX
	MID_ARME3_JUMPREADY,				//351//�Ƹ���(3)//�����غ�
	MID_ARME3_JUMP,						//352//�Ƹ���(3)//����
	MID_ARME3_SUMMON_MOVE,				//353//�Ƹ���(3)//���� �غ� �� �̵�
	MID_ARME3_SUMMON_WAIT,				//354//�Ƹ���(3)//���� ��ȯ �̵� �� ���
	MID_ARME3_UNSUMMON,					//355//�Ƹ���(3)//��ȯ ����
	MID_ARME3_DASHATK1,					//356//�Ƹ���(3)//�뽬 ���� 1
	MID_ARME3_DASHATK2,					//357//�Ƹ���(3)//�뽬 ���� 2
	MID_NOUSE_358,						//358//������(SLOT)//XXXXXXXXXXXXXXX
	MID_ARME3_JUMPATK1,					//359//�Ƹ���(3)//���� ���� 1
	MID_ARME3_JUMPATK2,					//360//�Ƹ���(3)//���� ���� 2
	MID_ARME3_ATK1READY,				//361//�Ƹ���(3)//�����غ� 1
	MID_ARME3_ATK2READY,				//362//�Ƹ���(3)//�����غ� 2
	MID_ARME3_SUMMON_ENDINE,			//363//�Ƹ���(3)//���� ��ȯ
	MID_ARME3_DASHATK3,					//364//�Ƹ���(3)//�����Ǵ� ��ȯ -> �뽬����3
	MID_ARME3_SUMMON_SHYLPH,			//365//�Ƹ���(3)//���� ��ȯ
	MID_ARME3_SUMMON_UNDINE,			//366//�Ƹ���(3)//����� ��ȯ
	MID_ARME3_SUMMON_SALIST,			//367//�Ƹ���(3)//������Ʈ ��ȯ -> ö�� �̻��ص� ���� X
	MID_NOUSE_368,						//368//������(SLOT)//XXXXXXXXXXXXXXX
	MID_ARME3_SPECIAL1,					//369//�Ƹ���(3)//�ʻ�� 1
	MID_ARME3_SPECIAL2,					//370//�Ƹ���(3)//�ʻ�� 2
	MID_ARME3_SPECIAL3,					//371//�Ƹ���(3)//�ʻ�� 3
	MID_ARME3_JOKE,						//372//�Ƹ���(3)//����
	MID_ARME3_SELECT,					//373//�Ƹ���(3)//����
	MID_ARME3_START,					//374//�Ƹ���(3)//����
	MID_ARME3_WINPOS,					//375//�Ƹ���(3)//����
	MID_NOUSE376,						//376//������(SLOT)//XXXXXXXXXXXXXXX
	MID_NOUSE377,						//377//������(SLOT)//XXXXXXXXXXXXXXX
	MID_ARME3_SUMMON_JUMP,				//378//�Ƹ���(3)//��ȯ�غ� �� ���� �غ�
//================================�Ƹ��� 2�� ���� ��=======================================//
//================================�� 2�� ���� ����=======================================//
	MID_LAS3_WAIT,						//379//��(3)//�⺻�ڼ�
	MID_LAS3_FATAL,						//380//��(3)//����Ż
	MID_LAS3_WALK,						//381//��(3)//�ȱ�
	MID_LAS3_DASH,						//382//��(3)//�뽬
	MID_LAS3_DASH_ATK2,					//383//��(3)//�뽬 ����2
	MID_LAS3_JUMPREADY,					//384//��(3))//���� �غ�
	MID_LAS3_JUMP,						//385//��(3)//����
	MID_LAS3_UP_JUMPDASH,				//386//��(3)//���� �뽬 ����
	MID_LAS3_FRONT_JUMPDASH,			//387//��(3)//���� �뽬 ������
	MID_LAS3_DASH_ATK3,					//388//��(3)//�뽬 ����2
	MID_LAS3_DOWN_JUMPDASH,				//389//��(3)//���� �뽬 �Ʒ���
	MID_LAS3_LANDING,					//390//��(3)//���� �뽬 ����
	MID_LAS3_ATK1,						//391//��(3)//�޺� ���� 1
	MID_LAS3_ATK2,						//392//��(3)//�޺� ���� 2
	MID_LAS3_ATK3,						//393//��(3)//�޺� ���� 3
	MID_LAS3_ATK4,						//394//��(3)//�޺� ���� 4
	MID_LAS3_ATK3_2,					//395//��(3)//�޺� ���� 3-2
	MID_LAS3_DASH_ATK,					//396//��(3)//�뽬 ����
	MID_LAS3_JUMP_ATK,					//397//��(3)//���� ����
	MID_LAS3_CATCH,						//398//��(3)//���
	MID_COMMON_CATCH_BY_LAS3,			//399//��(3)//������
	MID_LAS3_SPECIAL1,					//400//��(3)//�ʻ�� 1
	MID_LAS3_SPECIAL2,					//401//��(3)//�ʻ�� 2
	MID_LAS3_SPECIAL3,					//402//��(3)//�ʻ�� 3
	MID_LAS3_JOKE,						//403//��(3)//����
	MID_LAS3_SELECT,					//404//��(3)//����
//================================�� 2�� ���� ��=========================================//
//==================================���̾� ����============================================//
	MID_RYAN_WAIT,						//405//���̾�//�⺻�ڼ�
	MID_RYAN_FATAL,						//406//���̾�//����Ż
	MID_RYAN_JOKE,						//407//���̾�//����
	MID_RYAN_WALK,						//408//���̾�//�ȱ�
	MID_RYAN_DASH,						//409//���̾�//�뽬
	MID_RYAN_JUMP_READY,				//410//���̾�//�����غ�
	MID_RYAN_JUMP,						//411//���̾�//����
	MID_RYAN_CATCH,						//412//���̾�//���
	MID_COMMON_CATCH_BY_RYAN,			//413//���̾�//������
	MID_RYAN_ATK1,						//414//���̾�//����1
	MID_RYAN_ATK2,						//415//���̾�//����2
	MID_RYAN_ATK3,						//416//���̾�//����3
	MID_RYAN_DASH_ATK1,					//417//���̾�//�뽬����1
	MID_RYAN_DASH_ATK2,					//418//���̾�//�뽬����2
	MID_RYAN_JUMP_ATK1,					//419//���̾�//��������1
	MID_RYAN_JUMP_ATK2,					//420//���̾�//��������2
	MID_RYAN_JUMP_ATK2_LAND,			//421//���̾�//��������2 ����
	MID_RYAN_SPECIAL1,					//422//���̾�//�ʻ��1
	MID_RYAN_SPECIAL2,					//423//���̾�//�ʻ��2
	MID_RYAN_SPECIAL3,					//424//���̾�//�ʻ��3
	MID_RYAN_SELECT,					//425//���̾�//����
	MID_RYAN_START,						//426//���̾�//����
	MID_RYAN_WINPOS,					//427//���̾�//����
	MID_RYAN_RESULT_WIN,				//428//���̾�//���â �¸�
	MID_RYAN_RESULT_LOSE,				//429//���̾�//���â �й�
//===================================���̾� ��=============================================//

	MID_TOTAL,
};

// �÷��̾ �ɷ��ִ� ��������
enum EGCPlayerMagicEffect
{
	EGC_EFFECT_DARKBALL		= 0,
	EGC_EFFECT_ENT_TRUNK,
	EGC_EFFECT_EAGLE_SITE,
	EGC_EFFECT_CONFUSION,
	EGC_EFFECT_BOOST_HEAL,
	EGC_EFFECT_ATHENS_SWORD,
	EGC_EFFECT_DARK_SWORM,
	EGC_EFFECT_BLOOD_BASH,
	EGC_EFFECT_FIRE_SHIELD,
	EGC_EFFECT_BACKSTEP,
	EGC_EFFECT_BACKSTEP_APPEAR,
	EGC_EFFECT_ANKH_OF_RESURRECTION,

	EGC_EFFECT_NUM,
};

enum EMIT_POSITION_TYPE
{
	EPT_BODY_CENTER,
	EPT_BODY_CENTER_MIRROR,
	EPT_SWORD_TIP,
	EPT_SWORD_BODY,
	EPT_SWORD_CENTER,
	EPT_BOW,
	EPT_CROSSBOW_ONE,
	EPT_CROSSBOW_TWO,
	EPT_LASKNIFE_ONE,
	EPT_LASKNIFE_TWO,
	EPT_RSHOULDER,
	EPT_LSHOULDER,
	EPT_RHAND,
	EPT_LHAND,
	EPT_RFOOT,
	EPT_LFOOT,
};

// FRAME������ 3���� �� �ۿ� ���� �� �����Ƿ�.. �̷� ������ ó���մϴ�.
// PHYSICATKTYPE, PHYSICATKREACTION, POWER �� �־���
enum ePhysicAtkType
{
	PAT_SWORD = 1,
	PAT_LEG_LEFT = 2,
	PAT_LEG_RIGHT = 3,
	PAT_SWORD_LEFT = 4,
};

//���̿� �ٸ� ���� �߰��Ǹ� ����� ��� �����Ǿ�� �ϹǷ�...
//���� �߰��� ���� �����־�� �ȵ�
enum ePhysicAtkReaction
{
	PAR_DOWN			= 0,

	PAR_SMALL_DAMAGE	= 1,		//��¦ �´� �������� ��ȯ
	PAR_BIG_DAMAGE		= 2,				//ũ�� �´� �������� ��ȯ

	PAR_FRONTDOWN_0		= 3,				//��������... ���̴� N * FACTOR ���� PAR_FRONTDOWN_4�� ���� ���� �߰Ե�
	PAR_FRONTDOWN_1		= 4,				//�������鼭 ������ ����
	PAR_FRONTDOWN_2		= 5,
	PAR_FRONTDOWN_3		= 6,
	PAR_FRONTDOWN_LAST	= 7,

	PAR_FRONTDOWN_DROPDAMAGE_0		= 8,		//��������... ���̴� N * FACTOR ���� PAR_FRONTDOWN_4�� ���� ���� �߰Ե�
	PAR_FRONTDOWN_DROPDAMAGE_1		= 9,		//�������鼭 ������ ����.. ���̿� ����...
	PAR_FRONTDOWN_DROPDAMAGE_2		= 10,
	PAR_FRONTDOWN_DROPDAMAGE_3		= 11,
	PAR_FRONTDOWN_DROPDAMAGE_LAST	= 12,

	PAR_SMALL_DAMAGE_NODOWN			= 13,		//���߿��� �¾Ƶ� �������� �ʴ� ���� ������ ����
	PAR_BIG_DAMAGE_NODOWN			= 14,		//���߿��� �¾Ƶ� �������� �ʴ� ū ������ ����
	PAR_REVERSE_DOWN				= 15,
};

// ��������
enum EGCUserLevel
{
	USERLEVEL_BAD = -1,		// �ҷ�����
	USERLEVEL_NOMAL = 0,	// �Ϲ�����
	USERLEVEL_HERO = 1,		// ����Ǽ� �����
	USERLEVEL_HIGH = 2,		// �ʰ��, ���������
	USERLEVEL_ADMIN = 3,	// ���
};
// �ۺ��� Ÿ��
enum PublisherAuthType
{
	AT_NETMARBLE    = 0x00000001,   // �ݸ���   - �ѱ�
	AT_NEXON        = 0x00000002,   // �ؽ����� - �Ϻ�
	AT_GASH         = 0x00000004,   // �����Ͼ� - �븸, ȫ��
	AT_SSO          = 0x00000008,   // ����     - ����, �ϰ�
	AT_LEVELUP      = 0x00000010,   // ������   - �����
	AT_NMJ          = 0x00000020,   // �ݸ��� ����
};

enum EGCEventEnum
{
	EVENT_ID_VALENTINE      = 0,
	EVENT_ID_GP_EXP         = 1,
	EVENT_ID_CARNATION      = 2,
	EVENT_ID_RICECAKE       = 3,//���� �̺�Ʈ
	EVENT_ID_WORLDCUP		= 4,//������ �̺�Ʈ
	EVENT_ID_FRIENDSHIP		= 5,//�Ϻ� �����ǹ��� �̺�Ʈ
	EVENT_ID_SUMMER			= 6,//�ϴϾȼ�Ʈ
	EVENT_ID_EGGCOOKIE		= 7,//�븸 ��������̺�Ʈ
	EVENT_ID_GP2_PvPMode	= 8,//�Ϻ� �������� GP2��
	EVENT_ID_PIGHEAD		= 9,//�븸 ���ӳ� ü��ȸ���������� �����Ӹ��� ����
	EVENT_ID_GHOSTMONTH		= 10,//�븸 �ͽ��� �� �̺�Ʈ
	EVENT_ID_END,
};

#ifdef _ENUM
#undef _ENUM
#define             _ENUM( ID, strError_ )                  ID,
#else
#define             _ENUM( ID, strError_ )                  ID,
#endif

enum GC_CONNECTION_ERROR
{
#include "FailRate_def.h"
};

#endif // _GCENUM_H_
