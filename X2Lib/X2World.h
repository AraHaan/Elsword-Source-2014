/** @file   X2World.h
    @desc   interface of X2World
    @date   jintaeks on 2008-10-23, 14:19
            - add KHelpSystem
            - ��ü�� �̸����� ��ü�� ã�� ��� �߰�
                -- ��ü �̸��� �޽� ���� �̸��� ���� ���� ���
*/

#pragma once

struct WORLDMESH_SYNC
{
	float fTime;
	float fFutureTime;
};

class CX2World
{
#ifdef KHELP_SYSTEM_TEST // 2008-10-23

public:
    DECLARE_HELP_SYSTEM( CX2World );

#endif // KHELP_SYSTEM_TEST // 2008-10-23

	public:
		enum WORLD_ID
		{
			WI_NONE = 0,
			WI_FOREST_OF_EL,				// ���� ElTreeDown�� �ش��ϴ� ��
			WI_TREE_OF_LIGHT_TOP,
			WI_EL_FOREST_MAIN_STAGE2,
			WI_EL_FOREST_MAIN_STAGE3,
			WI_EL_FOREST_MAIN_STAGE4,
			

			WI_TEMP0,						// note!! �׽�Ʈ�� ���� ���̵�
			WI_TEMP1,
			WI_TEMP2,
			WI_TEMP3,
			WI_TEMP4,
			WI_TEMP5,

			WI_EL_FOREST_WEST_STAGE1,
			WI_EL_FOREST_WEST_STAGE2,
			WI_EL_FOREST_WEST_STAGE3,
			WI_EL_FOREST_WEST_STAGE1_1,
			WI_EL_FOREST_WEST_STAGE2_1,
			WI_EL_FOREST_MAIN_STAGE2_1,
			WI_EL_FOREST_MAIN_STAGE3_1,

			WI_EL_FOREST_NORTH_STAGE1,
			WI_EL_FOREST_NORTH_STAGE1_1,
			WI_EL_FOREST_NORTH_STAGE1_1B,
			WI_EL_FOREST_NORTH_STAGE2,		
			WI_EL_FOREST_NORTH_STAGE2B,
			WI_EL_FOREST_NORTH_STAGE2_1,
			WI_EL_FOREST_NORTH_STAGE2_1B,
			WI_EL_FOREST_NORTH_STAGE3,
			
			WI_TUTORIAL,						/// Ʃ�丮��

			WI_NOVICE_FOREST_STAGE1,			/// �ʽ����� ��_1
			WI_NOVICE_FOREST_STAGE2,			/// �ʽ����� ��_2

			WI_ELDER_BENDERS_CAVE_STAGE1,		/// ����_������ ����_1
			WI_ELDER_BENDERS_CAVE_STAGE1_1,		/// ����_������ ����_1_1
			WI_ELDER_BENDERS_CAVE_STAGE2,		/// ����_������_����_2
			WI_ELDER_BENDERS_CAVE_STAGE2_1,		/// ����_������_����_2_1
			WI_ELDER_BENDERS_CAVE_STAGE3,		/// ����_������_����_3

			WI_ELDER_NATURE_CAVE_STAGE1,		/// ����_
			WI_ELDER_NATURE_CAVE_STAGE1_1,		/// ����_
			WI_ELDER_NATURE_CAVE_STAGE2,		/// ����_
			WI_ELDER_NATURE_CAVE_STAGE2_1,		/// ����_
			WI_ELDER_NATURE_CAVE_STAGE3,		/// ����_

			WI_EL_FOREST_EXTRA_STAGE1,			/// ���� ��_1
			WI_EL_FOREST_EXTRA_STAGE2,			/// ����_��_2
			WI_EL_FOREST_EXTRA_STAGE3,			/// ����_��_3
			WI_EL_FOREST_EXTRA_STAGE4,			/// ����_��_4

			WI_EL_FOREST_EXTRA_CAVE_STAGE1,		/// ����_��_����_1
			WI_EL_FOREST_EXTRA_CAVE_STAGE2,		/// ����_��_����_2
			WI_EL_FOREST_EXTRA_CAVE_STAGE3,		/// ����_��_����_3
			WI_EL_FOREST_EXTRA_CAVE_STAGE4,		/// ����_��_����_4
			WI_EL_FOREST_EXTRA_CAVE_STAGE5,		/// ����_��_����_5

			WI_SQUARE,							/// ����

			WI_ELDER_DRUID_FOREST_STAGE1,		/// ����_����̵�_��_1
			WI_ELDER_DRUID_FOREST_STAGE1_1A,	/// ����_����̵�_��_1A
			WI_ELDER_DRUID_FOREST_STAGE1_1B,	/// ����_����̵�_��_1B
			WI_ELDER_DRUID_FOREST_STAGE2A,		/// ����_����̵�_��_2A
			WI_ELDER_DRUID_FOREST_STAGE2B,		/// ����_����̵�_��_2B
			WI_ELDER_DRUID_FOREST_STAGE2_1A,	/// ����_����̵�_��_2_1A
			WI_ELDER_DRUID_FOREST_STAGE2_1B,	/// ����_����̵�_��_2_1B
			WI_ELDER_DRUID_FOREST_STAGE3,		/// ����_����̵�_��_3

			WI_ELDER_EXTRA_CAVE_STAGE1,			/// ����_����_1
			WI_ELDER_EXTRA_CAVE_STAGE2,			/// ����_����_2

			WI_EL_FOREST_NORTH_STAGE1_1B_MIRROR,	/// ����_��_����_1_1_
			WI_EL_FOREST_NORTH_STAGE2_1_MIRROR,		/// ����_��_����_2_1_

			WI_ELDER_WALLY_CASTLE_ROOF_STAGE1,		/// ����_������_��_�ܰ�_1
			WI_ELDER_WALLY_CASTLE_ROOF_STAGE1_1,	/// ����_������_��_�ܰ�_1_1
			WI_ELDER_WALLY_CASTLE_ROOF_STAGE2,		/// ����_������_��_�ܰ�_2
			WI_ELDER_WALLY_CASTLE_ROOF_STAGE2_1,	/// ����_������_��_�ܰ�_2_1
			WI_ELDER_WALLY_CASTLE_ROOF_STAGE3,		/// ����_������_��_�ܰ�_3
			WI_ELDER_WALLY_CASTLE_ROOF_STAGE4,		/// ����_������_��_�ܰ�_4


			WI_ELDER_WALLY_CASTLE_CENTER_STAGE1,	/// ����_������_��_1
			WI_ELDER_WALLY_CASTLE_CENTER_STAGE1_1,	/// ����_������_��_1_1
			WI_ELDER_WALLY_CASTLE_CENTER_STAGE2,	/// ����_������_��_2
			WI_ELDER_WALLY_CASTLE_CENTER_STAGE2_1,	/// ����_������_��_2_1
			WI_ELDER_WALLY_CASTLE_CENTER_STAGE3,	/// ����_������_��_3
			WI_ELDER_WALLY_CASTLE_CENTER_STAGE4,	/// ����_������-��_4


			WI_ELDER_BELOW_PASSAGE_STAGE1,			/// ����_���ϼ���_1
			WI_ELDER_BELOW_PASSAGE_STAGE2A,			/// ����_���ϼ���_2A
			WI_ELDER_BELOW_PASSAGE_STAGE2B,			/// ����_���ϼ���_2B
			WI_ELDER_BELOW_PASSAGE_STAGE3A,			/// ����_���ϼ���_3A
			WI_ELDER_BELOW_PASSAGE_STAGE3B,			/// ����_���ϼ���_3B
			WI_ELDER_BELOW_PASSAGE_STAGE4,			/// ����_���ϼ���_4
			WI_ELDER_BELOW_PASSAGE_STAGE2_1A,		/// ����_���ϼ���_2_1A
			WI_ELDER_BELOW_PASSAGE_STAGE2_1B,		/// ����_���ϼ���_2_1B
			WI_ELDER_BELOW_PASSAGE_STAGE3_1A,		/// ����_���ϼ���_3_1A
			WI_ELDER_BELOW_PASSAGE_STAGE3_1B,		/// ����_���ϼ���_3_1B

			WI_MONSTER_TEST_WORLD,					/// ����_�׽�Ʈ

			WI_BESMA_DRAGON_ROAD_STAGE1,			/// ������_����_��_1
			WI_BESMA_DRAGON_ROAD_STAGE2A,			/// ������_����_��_2A
			WI_BESMA_DRAGON_ROAD_STAGE2B,			/// ������_����_��_2B
			WI_BESMA_DRAGON_ROAD_STAGE2_1A,			/// ������_����_��_2_1A
			WI_BESMA_DRAGON_ROAD_STAGE2_1B,			/// ������_����_��_2_1B
			WI_BESMA_DRAGON_ROAD_STAGE3,			/// ������_����_��_3
			WI_BESMA_DRAGON_ROAD_STAGE4,			/// ������_����_��_4

			WI_BESMA_MINE_STAGE1,					/// ������_��ġ����_1
			WI_BESMA_MINE_STAGE2A,					/// ������_��ġ����_2A
			WI_BESMA_MINE_STAGE2_1A,				/// ������_��ġ����_2_1A
			WI_BESMA_MINE_STAGE2_2A,				/// ������_��ġ����_2_2A
			WI_BESMA_MINE_STAGE3A,					/// ������_��ġ����_3A
			WI_BESMA_MINE_STAGE4,					/// ������_��ġ����_4

			WI_TRAINING_CENTER_STAGE1,				/// �Ʒü�_1
			WI_TRAINING_CENTER_STAGE2,				/// �Ʒü�_2
			WI_TRAINING_CENTER_STAGE3,				/// �Ʒü�_3
			WI_TRAINING_CENTER_STAGE4,				/// �Ʒü�_4

			WI_BESMA_LAKE_STAGE1,					/// ������_ȣ��_1
			WI_BESMA_LAKE_STAGE2A,					/// ������_ȣ��_2A
			WI_BESMA_LAKE_STAGE2_1A,				/// ������_ȣ��_2_1A
			WI_BESMA_LAKE_STAGE3A,					/// ������_ȣ��_3A
			WI_BESMA_LAKE_STAGE3_1A,				/// ������_ȣ��_3_1A
			WI_BESMA_LAKE_STAGE4,					/// ������_ȣ��_4


			WI_BESMA_LAKE_NIGHT_STAGE1,				/// ������_ȣ��_��_1
			WI_BESMA_LAKE_NIGHT_STAGE2A,			/// ������_ȣ��_��_2A
			WI_BESMA_LAKE_NIGHT_STAGE2_1A,			/// ������_ȣ��_��_2_1A
			WI_BESMA_LAKE_NIGHT_STAGE3A,			/// ������_ȣ��_��_3A
			WI_BESMA_LAKE_NIGHT_STAGE3_1A,			/// ������_ȣ��_��_3_1A
			WI_BESMA_LAKE_NIGHT_STAGE4,				/// ������_ȣ��_��_4


			WI_BESMA_DRAGON_NEST_STAGE1,			/// ������_����_����_1
			WI_BESMA_DRAGON_NEST_STAGE2,			/// ������_����_����_2
			WI_BESMA_DRAGON_NEST_STAGE2_1,			/// ������_����_����_2_1
			WI_BESMA_DRAGON_NEST_STAGE2_1B,			/// ������_����_����_2_1B
			WI_BESMA_DRAGON_NEST_STAGE3,			/// ������_����_����_3
			WI_BESMA_DRAGON_NEST_STAGE4,			/// ������_����_����_4

			WI_BESMA_AIRSHIP_STAGE1,				/// ������_���ۺ����_1
			WI_BESMA_AIRSHIP_STAGE2A,				/// ������_���ۺ����_2A
			WI_BESMA_AIRSHIP_STAGE2B,				/// ������_���ۺ����_2B
			WI_BESMA_AIRSHIP_STAGE2_1A,				/// ������_���ۺ����_2_1A
			WI_BESMA_AIRSHIP_STAGE2_1B,				/// ������_���ۺ����_2_1B
			WI_BESMA_AIRSHIP_STAGE3A,				/// ������_���ۺ����_3A
			WI_BESMA_AIRSHIP_STAGE4,				/// ������_���ۺ����_4
			WI_BESMA_AIRSHIP_STAGE5,				/// ������_���ۺ����_5

			WI_BESMA_MINE2_STAGE1,					/// ������_��ġ����(����)_1
			WI_BESMA_MINE2_STAGE2A,					/// ������_��ġ����(����)_2A
			WI_BESMA_MINE2_STAGE2_1A,				/// ������_��ġ����(����)_2_1A
			WI_BESMA_MINE2_STAGE2_2A,				/// ������_��ġ����(����)_2_2A
			WI_BESMA_MINE2_STAGE3A,					/// ������_��ġ����(����)_3A
			WI_BESMA_MINE2_STAGE4,					/// ������_��ġ����(����)_4

			WI_BESMA_LAKE_CANYON_STAGE3_1A,			/// ������_ȣ��_����_3_1A
			WI_BESMA_LAKE_CANYON_STAGE3A,			/// ������_ȣ��_����_3A
			WI_BESMA_LAKE_CANYON_STAGE4A,			/// ������_ȣ��_����_4A
			WI_BESMA_DRAGON_ROAD_CANYON_STAGE2_1A,	/// ������_����_��_����_2_1A
			WI_BESMA_DRAGON_ROAD_CANYON_STAGE2_1B,	/// ������_����_��_����_2_1B
			WI_BESMA_DRAGON_ROAD_CANYON_STAGE2B,	/// ������_����_��_����_2B
			WI_BESMA_DRAGON_ROAD_CANYON_STAGE3,		/// ������_����_��_����_3
			WI_BESMA_DRAGON_ROAD_CANYON_STAGE4,		/// ������_����_��_����_4


			
			WI_ALTERA_PLAIN_STAGE1,					/// ���׶�_���_1
			WI_ALTERA_PLAIN_STAGE2,					/// ���׶�_���_2
			WI_ALTERA_PLAIN_STAGE2B,				/// ���׶�_���_2B
			WI_ALTERA_PLAIN_STAGE2_1A,				/// ���׶�_���_2_1A
			WI_ALTERA_PLAIN_STAGE2_1B,				/// ���׶�_���_2_1B
			WI_ALTERA_PLAIN_STAGE3,					/// ���׶�_���_3
			WI_ALTERA_PLAIN_STAGE4,					/// ���׶�_���_4


			WI_ALTERA_BATTLE_AIR_SHIP_STAGE1,		/// ���׶�_��ũ�ο�ȣ_1
			WI_ALTERA_BATTLE_AIR_SHIP_STAGE2,		/// ���׶�_��ũ�ο�ȣ_2
			WI_ALTERA_BATTLE_AIR_SHIP_STAGE3,		/// ���׶�_��ũ�ο�ȣ_3
			WI_ALTERA_BATTLE_AIR_SHIP_STAGE4,		/// ���׶�_��ũ�ο�ȣ_4
			WI_ALTERA_BATTLE_AIR_SHIP_STAGE5,		/// ���׶�_��ũ�ο�ȣ_5
			WI_ALTERA_BATTLE_AIR_SHIP_STAGE6,		/// ���׶�_��ũ�ο�ȣ_6

			WI_WORLDTOOL_DEFAULT,						// worldtool���� ���� default world


			WI_ALTERA_BELOW_TUNNEL_STAGE1,			/// ���׶�_���_�ͳ�_1
			WI_ALTERA_BELOW_TUNNEL_STAGE2A,			/// ���׶�_���_�ͳ�_2A
			WI_ALTERA_BELOW_TUNNEL_STAGE2_1A,		/// ���׶�_���_�ͳ�_2_1A
			WI_ALTERA_BELOW_TUNNEL_STAGE2_2A,		/// ���׶�_���_�ͳ�_2_2A
			WI_ALTERA_BELOW_TUNNEL_STAGE3A,			/// ���׶�_���_�ͳ�_3A
			WI_ALTERA_BELOW_TUNNEL_STAGE4,			/// ���׶�_���_�ͳ�_4

            WI_BESMA_BESMA_LAKE_PVPMAP,				/// ������_ȣ��_����

            WI_SQUARE_MARKET,						/// ����_����

			WI_ELDER_WALLY_CASTLE_LAB_STAGE1,		/// ����_������_��_���Ͽ�����_1
			WI_ELDER_WALLY_CASTLE_LAB_STAGE2,		/// ����_������_��_���Ͽ�����_2
			WI_ELDER_WALLY_CASTLE_LAB_STAGE3,		/// ����_������_��_���Ͽ�����_3
			WI_ELDER_WALLY_CASTLE_LAB_STAGE4,		/// ����_������_��_���Ͽ�����_4
            WI_ELDER_WALLY_CASTLE_LAB_STAGE5,		/// ����_������_��_���Ͽ�����_5

            WI_BESMA_AIR_SHIP_PVPMAP,				/// ������_���ۺ����_����

            WI_EL_TREE_MAIN_TEST_DUNGEON,			/// ����_����_��_�׽�Ʈ_����


			WI_ALTERA_PLAIN_RECYCLE_STAGE1,			/// ���׶�_ȸ����_���_1
			WI_ALTERA_PLAIN_RECYCLE_STAGE2,			/// ���׶�_ȸ����_���_2
			WI_ALTERA_PLAIN_RECYCLE_STAGE2B,		/// ���׶�_ȸ����_���_2B
			WI_ALTERA_PLAIN_RECYCLE_STAGE2_1A,		/// ���׶�_ȸ����_���_2_1A
			WI_ALTERA_PLAIN_RECYCLE_STAGE2_1B,		/// ���׶�_ȸ����_���_2_1B
			WI_ALTERA_PLAIN_RECYCLE_STAGE3,			/// ���׶�_ȸ����_���_3
			WI_ALTERA_PLAIN_RECYCLE_STAGE4,			/// ���׶�_ȸ����_���_4

            WI_ALTERA_NASOD_FACTORY_STAGE1, 		/// ���׶�_���ҵ�_�������_1
            WI_ALTERA_NASOD_FACTORY_STAGE2, 		/// ���׶�_���ҵ�_�������_2
            WI_ALTERA_NASOD_FACTORY_STAGE3, 		/// ���׶�_���ҵ�_�������_3
            WI_ALTERA_NASOD_FACTORY_STAGE4, 		/// ���׶�_���ҵ�_�������_4
            WI_ALTERA_NASOD_FACTORY_STAGE5, 		/// ���׶�_���ҵ�_�������_5
            WI_ALTERA_NASOD_FACTORY_BOSS, 			/// ���׶�_���ҵ�_�������_����
			
			WI_BESMA_SECRET_STAGE1,					/// ������_����_����_����_1
			WI_BESMA_SECRET_STAGE2,					/// ������_����_����_����_2
			WI_BESMA_SECRET_STAGE2_1,				/// ������_����_����_����_2_1
			WI_BESMA_SECRET_STAGE3,					/// ������_����_����_����_3
			WI_BESMA_SECRET_STAGE3_1,				/// ������_����_����_����_3_1
			WI_BESMA_SECRET_STAGE4,					/// ������_����_����_����_4
			WI_BESMA_SECRET_STAGE5,					/// ������_����_����_����_5
			WI_BESMA_SECRET_STAGE6,					/// ������_����_����_����_6
			WI_BESMA_SECRET_STAGE7,					/// ������_����_����_����_7
			WI_BESMA_SECRET_BOSS,					/// ������_����_����_����_����


			WI_ARCADE_RUBEN_STAGE1,					/// �����̵�_�纥_1
			WI_ARCADE_RUBEN_STAGE1_1,				/// �����̵�_�纥_1_1
			WI_ARCADE_RUBEN_STAGE1_2,				/// �����̵�_�纥_1_2
			WI_ARCADE_RUBEN_STAGE2,					/// �����̵�_�纥_2
			WI_ARCADE_RUBEN_STAGE3,					/// �����̵�_�纥_3
			WI_ARCADE_RUBEN_STAGE4,					/// �����̵�_�纥_4
			WI_ARCADE_RUBEN_STAGE5,					/// �����̵�_�纥_5
			WI_ARCADE_RUBEN_STAGE6,					/// �����̵�_�纥_6
			WI_ARCADE_RUBEN_STAGE7,					/// �����̵�_�纥_7
			WI_ARCADE_RUBEN_STAGE8,					/// �����̵�_�纥_8
			WI_ARCADE_RUBEN_STAGE8_1,				/// �����̵�_�纥_8_1
			WI_ARCADE_RUBEN_STAGE9,					/// �����̵�_�纥_9
			WI_ARCADE_RUBEN_STAGE10,				/// �����̵�_�纥_10
			WI_ARCADE_RUBEN_STAGE11,				/// �����̵�_�纥_11
			WI_ARCADE_RUBEN_STAGE12,				/// �����̵�_�纥_12
			WI_ARCADE_RUBEN_STAGE13,				/// �����̵�_�纥_13
			WI_ARCADE_RUBEN_STAGE14,				/// �����̵�_�纥_14
			WI_ARCADE_RUBEN_STAGE15,				/// �����̵�_�纥_15
			WI_ARCADE_RUBEN_STAGE16,				/// �����̵�_�纥_16
			WI_ARCADE_RUBEN_STAGE17,				/// �����̵�_�纥_17
			WI_ARCADE_RUBEN_STAGE18,				/// �����̵�_�纥_18
			WI_ARCADE_RUBEN_STAGE19,				/// �����̵�_�纥_19
			WI_ARCADE_RUBEN_STAGE20,				/// �����̵�_�纥_20
			WI_ARCADE_RUBEN_STAGE21,				/// �����̵�_�纥_21
			WI_ARCADE_RUBEN_STAGE22,				/// �����̵�_�纥_22
			WI_ARCADE_RUBEN_STAGE23,				/// �����̵�_�纥_23


			WI_ARCADE_ELDER_STAGE1,					/// �����̵�_����_1
			WI_ARCADE_ELDER_STAGE2,					/// �����̵�_����_2
			WI_ARCADE_ELDER_STAGE3,					/// �����̵�_����_3
			WI_ARCADE_ELDER_STAGE3_1,				/// �����̵�_����_3_1
			WI_ARCADE_ELDER_STAGE4,					/// �����̵�_����_4
			WI_ARCADE_ELDER_STAGE5,					/// �����̵�_����_5
			WI_ARCADE_ELDER_STAGE5_1,				/// �����̵�_����_5_1
			WI_ARCADE_ELDER_STAGE6,					/// �����̵�_����_6
			WI_ARCADE_ELDER_STAGE7,					/// �����̵�_����_7
			WI_ARCADE_ELDER_STAGE8,					/// �����̵�_����_8
			WI_ARCADE_ELDER_STAGE9,					/// �����̵�_����_9
			WI_ARCADE_ELDER_STAGE10,				/// �����̵�_����_10
			WI_ARCADE_ELDER_STAGE11,				/// �����̵�_����_11
			WI_ARCADE_ELDER_STAGE12,				/// �����̵�_����_12
			WI_ARCADE_ELDER_STAGE13,				/// �����̵�_����_13
			WI_ARCADE_ELDER_STAGE14,				/// �����̵�_����_14
			WI_ARCADE_ELDER_STAGE15,				/// �����̵�_����_15

			WI_ARCADE_ELDER_STAGE30,				/// �����̵�_����_30
			WI_ARCADE_ELDER_STAGE31,				/// �����̵�_����_31
			WI_ARCADE_ELDER_STAGE32,				/// �����̵�_����_32
			WI_ARCADE_ELDER_STAGE33,				/// �����̵�_����_33
			WI_ARCADE_ELDER_STAGE34,				/// �����̵�_����_34
			WI_ARCADE_ELDER_STAGE35,				/// �����̵�_����_35


			WI_SUMMER_2008_PVP,						/// ����_2008_����


			WI_ALTERA_CORE_STAGE1,					/// ���׶�_�ھ�_1
			WI_ALTERA_CORE_STAGE1_MIDDLE,			/// ���׶�_�ھ�_1_�̵�
			WI_ALTERA_CORE_STAGE2,					/// ���׶�_�ھ�_2
			WI_ALTERA_CORE_STAGE2_MIDDLE,			/// ���׶�_�ھ�_2_�̵�
			WI_ALTERA_CORE_STAGE3,					/// ���׶�_�ھ�_3
			WI_ALTERA_CORE_STAGE3_MIDDLE,			/// ���׶�_�ھ�_3_�̵�
			WI_ALTERA_CORE_STAGE4,					/// ���׶�_�ھ�_4
			WI_ALTERA_CORE_STAGE4_MIDDLE,			/// ���׶�_�ھ�_4_�̵�
			WI_ALTERA_CORE_BOSS_STAGE,				/// ���׶�_�ھ�_����

			WI_EL_FOREST_MAIN_STAGE1,				/// ����_����_��_1

			WI_TRAINING_CENTER_FREESTAGE,			/// �Ʒü�_����

			WI_EL_FOREST_MONKEY_STAGE1,				/// ����_����_������_1
			WI_EL_FOREST_MONKEY_STAGE2,				/// ����_����_������_2
			WI_EL_FOREST_MONKEY_STAGE3,				/// ����_����_������_3

	
			WI_VILLAGE_RUBEN,						/// �纥_����
			WI_DUNGEON_GATE_RUBEN,					/// �纥_����_����Ʈ


			WI_ELDER_HALLOWEEN_CARNIVAL_STAGE1,		/// ����_�ҷ���_����_1
			WI_ELDER_HALLOWEEN_CARNIVAL_STAGE2,		/// ����_�ҷ���_����_2


			WI_VILLAGE_BESMA,						/// ������_����
			WI_VILLAGE_ELDER,						/// ����_����
			WI_DUNGEON_GATE_ELDER,					/// ����_����_����Ʈ

			WI_VILLAGE_ALTERA,						/// ���׶�_����
			WI_DUNGEON_GATE_BESMA,					/// ������_����_����Ʈ
			WI_DUNGEON_GATE_ALTERA,					/// ���׶�_����_����Ʈ
			
			WI_VILLAGE_PEITA,						/// ����Ÿ_����

			WI_PEITA_OFFERINGS_STAGE1,				/// ����Ÿ_�����ǽ���_1
			WI_PEITA_OFFERINGS_STAGE2,				/// ����Ÿ_�����ǽ���_2
			WI_PEITA_OFFERINGS_STAGE3,				/// ����Ÿ_�����ǽ���_3
			WI_PEITA_OFFERINGS_STAGE4,				/// ����Ÿ_�����ǽ���_4
			WI_PEITA_OFFERINGS_STAGE_BOSS,			/// ����Ÿ_�����ǽ���_����

			WI_PEITA_SPIRAL_CORRIDOR_STAGE1,		/// ����Ÿ_����_ȸ��_1
			WI_PEITA_SPIRAL_CORRIDOR_STAGE2,		/// ����Ÿ_����_ȸ��_2
			WI_PEITA_SPIRAL_CORRIDOR_STAGE3,		/// ����Ÿ_����_ȸ��_3
			WI_PEITA_SPIRAL_CORRIDOR_STAGE4,		/// ����Ÿ_����_ȸ��_4
			WI_PEITA_SPIRAL_CORRIDOR_STAGE_BOSS,	/// ����Ÿ_����_ȸ��_����


			WI_EVE_TUTORIAL,						/// �̺�_Ʃ�丮��


			WI_DUNGEON_LOUNGE_RUBEN,				/// �纥_����_�ްԽ�
			WI_DUNGEON_LOUNGE_ELDER,				/// ����_����-�ްԽ�
			WI_DUNGEON_LOUNGE_BESMA,				/// ������_����_�ްԽ�
			WI_DUNGEON_LOUNGE_ALTERA,				/// ���׶�_����_�ްԽ�
			WI_DUNGEON_LOUNGE_PEITA,				/// ����Ÿ_����_�ްԽ�


			WI_ALTERA_SECRET_STAGE1,				/// ���׶�_����ͳ�_��������_1
			WI_ALTERA_SECRET_STAGE2,				/// ���׶�_����ͳ�_��������_2
			WI_ALTERA_SECRET_STAGE3,				/// ���׶�_����ͳ�_��������_3
			WI_ALTERA_SECRET_STAGE4,				/// ���׶�_����ͳ�_��������_4
			WI_ALTERA_SECRET_BOSS,					/// ���׶�_����ͳ�_��������_����


			WI_2009_NEW_YEAR_EVENT_STAGE1,			/// 2009_����_�̺�Ʈ_1
			WI_2009_NEW_YEAR_EVENT_BOSS,			/// 2009_����_�̺�Ʈ_����


			WI_PEITA_OFFERING_PVP_STAGE,			/// ����Ÿ_�����ǽ���_����
			WI_PEITA_SPIRAL_CORRIDOR_PVP_STAGE,		/// ����Ÿ_����_ȸ��_����

			WI_PEITA_CHAPEL_STAGE1,
			WI_PEITA_CHAPEL_STAGE2_1,
			WI_PEITA_CHAPEL_STAGE2_2,
			WI_PEITA_CHAPEL_STAGE2_3,
			WI_PEITA_CHAPEL_STAGE3,
			WI_PEITA_CHAPEL_STAGE4,
			WI_PEITA_CHAPEL_STAGE5,
			WI_PEITA_CHAPEL_STAGE_BOSS,

			WI_PEITA_UNDER_GARDEN_STAGE1,
			WI_PEITA_UNDER_GARDEN_STAGE2,
			WI_PEITA_UNDER_GARDEN_STAGE_BOSS,

			WI_PEITA_TOWER_HEART_STAGE1,
			WI_PEITA_TOWER_HEART_STAGE2,
			WI_PEITA_TOWER_HEART_STAGE3,
			WI_PEITA_TOWER_HEART_STAGE_BOSS,

			WI_PEITA_OFFERINGS_ALTER_STAGE1,
			WI_PEITA_OFFERINGS_ALTER_STAGE2,
			WI_PEITA_OFFERINGS_ALTER_STAGE3,
			WI_PEITA_OFFERINGS_ALTER_STAGE4,
			WI_PEITA_OFFERINGS_ALTER_STAGE_BOSS,

			WI_ARCADE_NEW_STAGE_REST,	

			WI_VELDER_THIRD_DWELLING_STAGE1,
			WI_VELDER_THIRD_DWELLING_STAGE2,
			WI_VELDER_THIRD_DWELLING_STAGE3,
			WI_VELDER_THIRD_DWELLING_STAGE_BOSS,

			WI_VELDER_BRIDGE_HOPE_STAGE1,	
			WI_VELDER_BRIDGE_HOPE_STAGE2,	
			WI_VELDER_BRIDGE_HOPE_STAGE2_1,	
			WI_VELDER_BRIDGE_HOPE_STAGE3,	
			WI_VELDER_BRIDGE_HOPE_STAGE3_1,	
			WI_VELDER_BRIDGE_HOPE_STAGE4,	
			WI_VELDER_BRIDGE_HOPE_STAGE_BOSS,	

			WI_VILLAGE_VELDER,
			WI_DUNGEON_GATE_VELDER,
			WI_DUNGEON_LOUNGE_VELDER,

			WI_NEW_ELDER_BELOW_PASSAGE_STAGE3_1B,
			WI_NEW_ELDER_WALLY_CASTLE_ROOF_STAGE1_1,
			WI_NEW_ELDER_WALLY_CASTLE_ROOF_STAGE2,
			WI_NEW_ELDER_WALLY_CASTLE_ROOF_STAGE2_1,			
			WI_NEW_ELDER_WALLY_CASTLE_ROOF_STAGE3,

			WI_NEW_ELDER_WALLY_CASTLE_CENTER_STAGE2_1,
			WI_NEW_ELDER_WALLY_CASTLE_CENTER_STAGE1_1, 

			WI_NEW_BESMA_DRAGON_ROAD_STAGE1,
			WI_NEW_BESMA_DRAGON_ROAD_STAGE2A,
			WI_NEW_BESMA_DRAGON_ROAD_STAGE2_1B,
			WI_NEW_BESMA_DRAGON_ROAD_STAGE4,
			WI_NEW_BESMA_DRAGON_ROAD_STAGE2B,
			WI_NEW_BESMA_DRAGON_ROAD_STAGE2_1A,
			WI_NEW_BESMA_LAKE_STAGE1,
			WI_NEW_BESMA_LAKE_STAGE2A,

			WI_NEW_BESMA_DRAGON_NEST_STAGE1,
			WI_NEW_BESMA_DRAGON_NEST_STAGE2_1B,

			WI_ELDER_BRIDGE_HOPE_PVPMAP,
			WI_WINTER_PVPMAP,

			WI_NEW_BESMA_LAKE_NIGHT_STAGE1,
			WI_NEW_BESMA_LAKE_NIGHT_STAGE2A,

			WI_NEW_BESMA_AIRSHIP_STAGE2A,
			WI_NEW_BESMA_MINE_STAGE2_2A,
			WI_NEW_BESMA_MINE_STAGE2A,			

			WI_NEW_BESMA_AIRSHIP_STAGE1,

			WI_EVENT_TREE_DAY_STAGE1,
			WI_EVENT_TREE_DAY_STAGE2,
			WI_EVENT_TREE_DAY_STAGE3,
			WI_EVENT_TREE_DAY_STAGE_BOSS,

			// ��� ���� �߰� ����
			WI_BESMA_HELL_EXTRA_BOSS,
			WI_ELDER_SECRET_STAGE4,
			WI_ELDER_SECRET_BOSS,
			WI_BESMA_HELL_STAGE1,
			WI_BESMA_HELL_STAGE2,
			WI_BESMA_HELL_STAGE3_1,
			WI_BESMA_HELL_BOSS,

			// ��� ���� ����Ʈ�� ��� ����
			WI_BESMA_HELL_EXTRA_STAGE1,
			WI_BESMA_HELL_EXTRA_STAGE2,
			WI_BESMA_HELL_EXTRA_STAGE3,
			WI_BESMA_HELL_EXTRA_STAGE4,
			WI_BESMA_HELL_EXTRA_STAGE5,

			// ���� ����Ʈ�� ����
			WI_ELDER_HELL_EXTRA_STAGE1,
			WI_ELDER_HELL_EXTRA_STAGE2,
			WI_ELDER_HELL_EXTRA_STAGE3,
			WI_ELDER_HELL_EXTRA_STAGE4,
			WI_ELDER_HELL_EXTRA_STAGE5, 

			WI_ALTERA_HELL_EXTRA_STAGE1,
			WI_ALTERA_HELL_EXTRA_STAGE2,
			WI_ALTERA_HELL_EXTRA_STAGE3,
			WI_ALTERA_HELL_EXTRA_STAGE4,
			WI_ALTERA_HELL_EXTRA_STAGE5, 

            // �纥���� ���ǳ���
			WI_RUBEN_EL_TREE_STAGE1,
			WI_RUBEN_EL_TREE_STAGE2,
			WI_RUBEN_EL_TREE_BOSS_STAGE,

			WI_RUBEN_RUIN_OF_ELF_STAGE1,
			WI_RUBEN_RUIN_OF_ELF_STAGE2,
			WI_RUBEN_RUIN_OF_ELF_BOSS_STAGE,


			WI_RUBEN_SWAMP_STAGE1,
			WI_RUBEN_SWAMP_STAGE2,
			WI_RUBEN_SWAMP_STAGE3,
			WI_RUBEN_SWAMP_STAGE3_1,
			WI_RUBEN_SWAMP_BOSS_STAGE,

			WI_VELDER_PALACE_ENTRANCE_STAGE1,
			WI_VELDER_PALACE_ENTRANCE_STAGE2,
			WI_VELDER_PALACE_ENTRANCE_STAGE3,
			WI_VELDER_PALACE_ENTRANCE_STAGE_BOSS,

			WI_VELDER_BRIDGE_BURNING_STAGE1,
			WI_VELDER_BRIDGE_BURNING_STAGE2,

			WI_VELDER_BRIDGE_BURNING_STAGE2_1,
			WI_VELDER_BRIDGE_BURNING_STAGE3,
			WI_VELDER_BRIDGE_BURNING_BOSS_STAGE, 

			WI_VELDER_MARKET_STAGE1,
			WI_VELDER_MARKET_STAGE2,
			WI_VELDER_MARKET_STAGE3,
			WI_VELDER_MARKET_BOSS_STAGE,
			WI_VELDER_MARKET_STAGE2_1,

			WI_VELDER_GATE_STAGE1,
			WI_VELDER_GATE_STAGE2,
			WI_VELDER_GATE_STAGE3,
			WI_VELDER_GATE_BOSS_STAGE,
			WI_VELDER_GATE_STAGE2_1,
			WI_VELDER_GATE_LOUNGE,

			//{{ JHKang / ������ / 2010.10.19 / �ҷ��� �̺�Ʈ ���� �߰� Stage
			WI_EVENT_HALLOWEEN_STAGE1,
			WI_EVENT_HALLOWEEN_STAGE2,
			WI_EVENT_HALLOWEEN_STAGE3,
			WI_EVENT_HALLOWEEN_STAGE4,
			WI_EVENT_HALLOWEEN_STAGE5,
			WI_EVENT_HALLOWEEN_STAGE_BOSS,
			//}} JHKang / ������ / 2010.10.19 / �ҷ��� �̺�Ʈ ���� �߰� Stage

			WI_VILLAGE_HAMEL,				/// �ϸ� ����
			WI_DUNGEON_GATE_HAMEL,			/// �ϸ� ���� ����Ʈ
			WI_DUNGEON_LOUNGE_HAMEL,		/// �ϸ� ���� �����

			WI_BATTLE_FIELD_VELDER_SHIP_STAGE,	/// ���� --> �ϸ� ������
			WI_BATTLE_FIELD_HAMEL_SHIP_STAGE,		/// �ϸ� --> ���� ������

			//{{ JHKang / ������ / 2011.1.10 / ���þ� �ٱ� ����
			WI_HAMEL_RESIAM_STAGE1,
			WI_HAMEL_RESIAM_STAGE2,
			WI_HAMEL_RESIAM_STAGE3,
			WI_HAMEL_RESIAM_BOSS_STAGE,
			//}} JHKang / ������ / 2011.1.10 / ���þ� �ٱ� ����

			//{{ JHKang / ������ / 2011.1.12 / ��� ����
			WI_HAMEL_WATERWAY_STAGE1,
			WI_HAMEL_WATERWAY_STAGE2,
			WI_HAMEL_WATERWAY_STAGE3,
			WI_HAMEL_WATERWAY_STAGE4,
			WI_HAMEL_WATERWAY_BOSS_STAGE,
			//{{ JHKang / ������ / 2011.1.12 / ��� ����

			//{{ JHKang / ������ / 2011.1.14 / ������� ���þ�
			WI_HAMEL_RESIAM_UNDERWATER_STAGE1,
			WI_HAMEL_RESIAM_UNDERWATER_STAGE2,
			WI_HAMEL_RESIAM_UNDERWATER_STAGE3,
			WI_HAMEL_RESIAM_UNDERWATER_STAGE4,
			WI_HAMEL_RESIAM_UNDERWATER_BOSS_STAGE,
			//}} JHKang / ������ / 2011.1.14 / ������� ���þ�

			//{{ JHKang / ������ / 2011.1.20 / ��� ���� �߽ɺ�
			WI_HAMEL_WATERWAY_CORE_STAGE1,
			WI_HAMEL_WATERWAY_CORE_STAGE2,
			WI_HAMEL_WATERWAY_CORE_STAGE3,
			WI_HAMEL_WATERWAY_CORE_STAGE4,
			WI_HAMEL_WATERWAY_CORE_STAGE4_1,
			WI_HAMEL_WATERWAY_CORE_STAGE5,
			WI_HAMEL_WATERWAY_CORE_BOSS_STAGE,
			//}} JHKang / ������ / 2011.1.20 / ��� ���� �߽ɺ�

			WI_HAMEL_RESIAM_STAGE3_1,		/// ���þ� �������� ���п� ����
			
			//{{ JHKang / ������ / 2011.04.18 / ���潺 ���� ��������
			WI_DEFENCE_DUNGEON_NORMAL_STAGE,	/// �Ϲ�
			WI_DEFENCE_DUNGEON_HARD_STAGE,		/// �����
			WI_DEFENCE_DUNGEON_EXPERT_STAGE,	/// �ſ� �����
			//}}

			//{{ JHKang / ������ / 2011.05.17 / ���׸�Ÿ�� ����(5�� ����)
			WI_HAMEL_CAVE_OF_MAGMANTA_STAGE1,
			WI_HAMEL_CAVE_OF_MAGMANTA_STAGE2,
			WI_HAMEL_CAVE_OF_MAGMANTA_STAGE3,
			WI_HAMEL_CAVE_OF_MAGMANTA_STAGE4,
			WI_HAMEL_CAVE_OF_MAGMANTA_BOSS_STAGE,
			//}}
			WI_HAMEL_CAVE_OF_MAGMANTA_STAGE3_1,

			//{{ JHKang / ������ / 2011.6.14 / ������ ���� ���
			WI_HAMEL_FROZEN_WATER_TEMPLE_STAGE1,
			WI_HAMEL_FROZEN_WATER_TEMPLE_STAGE2,
			WI_HAMEL_FROZEN_WATER_TEMPLE_STAGE2_1,
			WI_HAMEL_FROZEN_WATER_TEMPLE_STAGE3,
			WI_HAMEL_FROZEN_WATER_TEMPLE_STAGE3_1,
			WI_HAMEL_FROZEN_WATER_TEMPLE_STAGE4,
			WI_HAMEL_FROZEN_WATER_TEMPLE_B0SS_STAGE,
			//}}

			//{{ JHKang / ������ / 2011.6.29 / ������ ���� ����
			WI_HAMEL_FROZEN_WATER_HALL_STAGE1,
			WI_HAMEL_FROZEN_WATER_HALL_STAGE2,
			WI_HAMEL_FROZEN_WATER_HALL_STAGE3,
			WI_HAMEL_FROZEN_WATER_HALL_STAGE4,
			WI_HAMEL_FROZEN_WATER_HALL_STAGE5,
			WI_HAMEL_FROZEN_WATER_HALL_BOSS_STAGE,
			//}}

			//{{ JHKang / ������ / 2011.8.24 / ���� ��� ����
			WI_VELDER_SECRET_STAGE1,
			WI_VELDER_SECRET_STAGE2,
			WI_VELDER_SECRET_STAGE3,
			WI_VELDER_SECRET_STAGE4, 
			WI_VELDER_SECRET_STAGE5,
			WI_VELDER_SECRET_BOSS_STAGE,

			WI_VELDER_SECRET_EXTRA_STAGE1,
			WI_VELDER_SECRET_EXTRA_STAGE2,
			WI_VELDER_SECRET_EXTRA_STAGE3, 
			WI_VELDER_SECRET_EXTRA_STAGE4,
			WI_VELDER_SECRET_EXTRA_STAGE5,
			WI_VELDER_SECRET_EXTRA_BOSS_STAGE,
			//}}

			WI_EVENT_HALLOWEEN_STAGE6,		/// �ؿ��� �ҷ��� �̺�Ʈ �������� �߰�
			WI_FISHING_HOLE,				/// �ؿ��� �̺�Ʈ ����(������ �߰�) 2012.03.14 �Ӽ���
			WI_EVENT_DARKDOOR_ABYSS,		/// �ؿ��� �̺�Ʈ ���� �߰� 2012.02.29 �Ӽ���
			
			WI_EVENT_TREE_DAY_NEW_STAGE2,
			WI_EVENT_TREE_DAY_NEW_STAGE3,

			WI_TAG_PVPMAP,

			/// �ؿ��� �̼� ���� �������� ���� �߰�
			WI_EVENT_MISSION_IMPOSSIBLE_STAGE1,
			WI_EVENT_MISSION_IMPOSSIBLE_STAGE1B,
			WI_EVENT_MISSION_IMPOSSIBLE_STAGE2,
			WI_EVENT_MISSION_IMPOSSIBLE_STAGE3,
			WI_EVENT_MISSION_IMPOSSIBLE_STAGE4,
			WI_EVENT_MISSION_IMPOSSIBLE_STAGE5,
			WI_EVENT_MISSION_IMPOSSIBLE_STAGE6,

			WI_VILLAGE_SANDER,				// ��������

			WI_BATTLE_FIELD_RUBEN_FIELD_00,		/// ��Ʋ�ʵ� �纥 00
  
			WI_BATTLE_FIELD_ELDER_FIELD_00,		/// ���� 00
			WI_BATTLE_FIELD_ELDER_FIELD_REST_00, /// ���� �޽�ó
			WI_BATTLE_FIELD_ELDER_FIELD_01,		/// ���� 01 
  
			WI_BATTLE_FIELD_BESMA_FIELD_00,		/// ��Ʋ�ʵ� ������ 00
			WI_BATTLE_FIELD_BESMA_FIELD_REST_00, /// �轺�� �޽���
			WI_BATTLE_FIELD_BESMA_FIELD_01,		/// ��Ʋ�ʵ� ������ 01
			WI_BATTLE_FIELD_BESMA_FIELD_02,		/// ��Ʋ�ʵ� ������ 02

			WI_BATTLE_FIELD_ALTERA_FIELD_00,			/// ��Ʋ�ʵ� ���׶� 00
			WI_BATTLE_FIELD_ALTERA_FIELD_01,			/// ��Ʋ�ʵ� ���׶� 01
			WI_BATTLE_FIELD_ALTERA_FIELD_REST_00,	/// ��Ʋ�ʵ� ���׶� ���� 00
			WI_BATTLE_FIELD_ALTERA_FIELD_02,			/// ��Ʋ�ʵ� ���׶� 02

			WI_BATTLE_FIELD_PEITA_FIELD_00,			/// ��Ʋ�ʵ� ����Ÿ 00 
			WI_BATTLE_FIELD_PEITA_FIELD_01,			/// ��Ʋ�ʵ� ����Ÿ 01 
			WI_BATTLE_FIELD_PEITA_FIELD_02,			/// ��Ʋ�ʵ� ����Ÿ 02 
			WI_BATTLE_FIELD_PEITA_FIELD_REST_00,	/// ��Ʋ�ʵ� ����Ÿ ���� 

			WI_BATTLE_FIELD_VELDER_FIELD_00, /// ��Ʋ�ʵ� ���� 00
			WI_BATTLE_FIELD_VELDER_FIELD_01, /// ��Ʋ�ʵ� ���� 01
			WI_BATTLE_FIELD_VELDER_FIELD_02, /// ��Ʋ�ʵ� ���� 02
			WI_BATTLE_FIELD_VELDER_FIELD_03, /// ��Ʋ�ʵ� ���� 03
			WI_BATTLE_FIELD_VELDER_FIELD_REST_00, /// ��Ʋ�ʵ� ���� 04

			WI_BATTLE_FIELD_HAMEL_FIELD_00, /// ��Ʋ�ʵ� ���� 00
			WI_BATTLE_FIELD_HAMEL_FIELD_01, /// ��Ʋ�ʵ� ���� 01
			WI_BATTLE_FIELD_HAMEL_FIELD_02, /// ��Ʋ�ʵ� ���� 02
			WI_BATTLE_FIELD_HAMEL_FIELD_03, /// ��Ʋ�ʵ� ���� 03
			WI_BATTLE_FIELD_HAMEL_FIELD_REST_00, /// ��Ʋ�ʵ� �޽�ó

			WI_BATTLE_FIELD_TUTORIAL,		/// ��Ʋ�ʵ� Ʃ�丮��

			//�޸��� ����
			WI_SANDER_DRY_SANDER_STAGE0,		
			WI_SANDER_DRY_SANDER_STAGE1,		
			WI_SANDER_DRY_SANDER_STAGE1_1,	
			WI_SANDER_DRY_SANDER_STAGE2,	
			WI_SANDER_DRY_SANDER_B0SS_STAGE,	

			//�������� �������
			WI_SANDER_GARPAI_ROCK_STAGE0,	
			WI_SANDER_GARPAI_ROCK_STAGE1,	
			WI_SANDER_GARPAI_ROCK_STAGE1_1,	
			WI_SANDER_GARPAI_ROCK_STAGE2,	
			WI_SANDER_GARPAI_ROCK_STAGE3,	
			WI_SANDER_GARPAI_ROCK_BOSS_STAGE,

			WI_BATTLE_FIELD_SANDER_FIELD_00, //���� �ʵ� 1
			WI_BATTLE_FIELD_SANDER_FIELD_01, //���� �ʵ� 2		
			WI_BATTLE_FIELD_SANDER_FIELD_02, //���� �ʵ� 3
			WI_BATTLE_FIELD_SANDER_FIELD_03, //���� �ʵ� 4
			WI_BATTLE_FIELD_SANDER_FIELD_REST_00,//���� �޽�ó

			//Ʈ�� �ұ�
			WI_SANDER_DEN_OF_TROCK_STAGE0,
			WI_SANDER_DEN_OF_TROCK_STAGE1,
			WI_SANDER_DEN_OF_TROCK_STAGE2_1,	
			WI_SANDER_DEN_OF_TROCK_STAGE2,
			WI_SANDER_DEN_OF_TROCK_STAGE3,
			WI_SANDER_DEN_OF_TROCK_BOSS_STAGE,

			//Į��� ���� ����
			WI_SANDER_KARUSO_VILLAGE_STAGE0,
			WI_SANDER_KARUSO_VILLAGE_STAGE1,
			WI_SANDER_KARUSO_VILLAGE_STAGE2,
			WI_SANDER_KARUSO_VILLAGE_BOSS_STAGE,

			WI_EVENT_VALENTINE_DAY_STAGE0			= 545,	// �߷�Ÿ�� �̺�Ʈ ����

			//���� PVP ��
			WI_SANDER_GARPAI_ROCK_PVPMAP				= 546,	
			WI_SANDER_VILLIAGE_PVPMAP					= 547,							WI_RUBEN_WEDDING							= 548,			WI_SANDER_WEDDING							= 549,

			// ���� 5�� ����, ��ƿ���� ȣ
			WI_SANDER_SANTILUS_SHIP_STAGE0			 	= 550,
			WI_SANDER_SANTILUS_SHIP_STAGE1,
			WI_SANDER_SANTILUS_SHIP_STAGE2,
			WI_SANDER_SANTILUS_SHIP_STAGE3,
			WI_SANDER_SANTILUS_SHIP_BOSS_STAGE,

			// ���� 6�� ����, ������ �����

			WI_SANDER_BEHIMOSS_HEART_STAGE0,
			WI_SANDER_BEHIMOSS_HEART_STAGE1,
			WI_SANDER_BEHIMOSS_HEART_STAGE2,
			WI_SANDER_BEHIMOSS_HEART_STAGE3,
			WI_SANDER_BEHIMOSS_HEART_BOSS_STAGE,

			//�ؿ��� ����Ʈ���� �̺�Ʈ ���� 2013.07.03 ��â��
			WI_EVENT_NIGHT_WATCHER_STAGE1 = 560,

			//�ϸ� ���
			WI_HAMEL_SECRET_STAGE0					 	= 561,			WI_HAMEL_SECRET_STAGE1					 	= 562,			WI_HAMEL_SECRET_STAGE2					 	= 563,			WI_HAMEL_SECRET_STAGE3					 	= 564,			WI_HAMEL_SECRET_BOSS_STAGE					= 565,			WI_HAMEL_SECRET_EXTRA_STAGE1				= 566,			WI_HAMEL_SECRET_EXTRA_STAGE2				= 567,			WI_HAMEL_SECRET_EXTRA_STAGE3				= 568,			WI_HAMEL_SECRET_EXTRA_BOSS_STAGE0			= 569,

			WI_FIELD_BOSSRAID_KINGDOM_OF_NASOD			= 570,	// ���ҵ� �ձ�
			WI_FIELD_BOSSRAID_VALLEY_OF_ABYSS			= 571,	// �ɿ��� ���
			WI_END,
		};

		struct WorldData
		{
			WORLD_ID	worldID;
			wstring		worldName;
			wstring		dataFileName;
		};

		enum SQUARE_CAMERA_TYPE
		{
			SCT_DEFAULT,
			SCT_PRESET,			// ���庰�� ���� ������ lookat, eye height���� ���
			SCT_LINE_DEPENDENT,	// CKTDGLineMap::CameraData�� ����ϴ� 
		};

		struct SquareCameraData
		{
			SQUARE_CAMERA_TYPE	m_eSquareCameraType;
			
			float 				m_fCameraDist;
			float 				m_fLookAtHeight;
			float 				m_fEyeHeight;

			SquareCameraData()
			{
				m_eSquareCameraType = SCT_DEFAULT;
				m_fCameraDist		= 0.f;
				m_fLookAtHeight		= 0.f;
				m_fEyeHeight		= 0.f;
			}
		};


#ifdef MARIO_LIKE_BLOCK_TEST

		struct WorldMonsterData
		{
			int m_iTeamNumber;
			int m_iNPCID;
			int m_iLevel;
			D3DXVECTOR3 m_vPosition;

			WorldMonsterData()
				: m_iTeamNumber( 0 )
				, m_iNPCID( 0 )
				, m_iLevel( 0 )
				, m_vPosition( 0, 0, 0 )
			{
			}
		};

#endif MARIO_LIKE_BLOCK_TEST

	public:
		CX2World( WorldData* pWorldData, const bool bBackgroundLoad_ = false );
		virtual ~CX2World(void);

		virtual HRESULT		OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT		OnFrameRender();
		virtual HRESULT		OnResetDevice();
		virtual HRESULT		OnLostDevice();

		void				SetMapDetail( CX2GameOption::OptionLevel optionLevel );

		void				PlayBGM();
		void				StopBGM();

		bool GetRenderLineMap() const { return m_bRenderLineMap; }
		void SetRenderLineMap(bool val) { m_bRenderLineMap = val; }

		void				SetWorldColor( D3DXCOLOR color )
		{
			m_NowColor		= color;
			m_FadeColor		= color;
			m_fFadeTime		= 0.0f;
		}

		void				FadeOriginWorldColor( D3DXCOLOR color, float fTime )
		{
			m_FadeOriginColor = color;	
			m_fFadeOriginTime = fTime;	
		}

		void				FadeWorldColor( D3DXCOLOR color, float fTime )
		{
			m_FadeColor		= color;
			m_fFadeTime		= fTime;
		}

		void				FadeLightColor( D3DXCOLOR color, float fTime )
		{
			m_FadeLightColor	= color;
			m_fFadeLightTime	= fTime;
		}

		void				FadeWorldLayerColor( D3DXCOLOR color, float fTime )
		{
			for( UINT i=0; i<m_LayerList.size(); i++ )
			{
				CX2WorldLayer* pWorldLayer = m_LayerList[i];
				if( NULL == pWorldLayer )
					continue;

				pWorldLayer->FadeColor( color, fTime );
			}
		}


		



		bool				RayCheck( D3DXVECTOR3 rayStart, D3DXVECTOR3 rayEnd );
		
		void				SetShowObject( bool bShow );
#ifdef X2TOOL
		void				SetShowObjectByTool( bool bShow );
#endif
		CKTDGLineMap*		GetLineMap(){ return m_pLineMap; }
		CKTDGLineMap*		GetLineMapFuture() { return m_pLineMapFuture; }

		D3DXVECTOR3			GetLightPos(){ return m_LightPos; }
		D3DXCOLOR			GetLightColor(){ return m_LightColor; }

		

        const WorldData* GetWorldData() const { return m_pWorldData; }

		//{{ seojt // 2008-10-24, 11:02
		//vector<CX2WorldObject*>& GetWorldObjectList() { return m_ObjectList; }
        // GetWorldObjectList()�� ȣ���� �ʿ��� container�� �������� �ʴ� �ٴ� ����
        // �����ϱ� ����, const&�� �����ϵ��� ���� - jintaeks on 2008-10-24, 11:03
		const vector<CX2WorldObjectPtr>& GetWorldObjectList() { return m_ObjectList; }
		//}} seojt // 2008-10-24, 11:02

#if defined(WORLD_TOOL) || defined(X2TOOL)
		//{{ 2009.02.25 ���¿� : GetWorldObjectList���� ���� vector�� �����ų �� ���� �����
		//World���� Ư�� ������Ʈ�� �����ϴ� ��� �߰� : ������ ��
		void RemoveWorldObject(CX2WorldObject* pTargetWorldObject);
		//}}
		vector<CX2WorldObjectPtr> GetWorldObjectList4Tool() { return m_ObjectList; }
		void AddWorldObject4Undo(CX2WorldObjectPtr pTargetWorldObjectPtr);
		void DeleteWorldObject4Undo(CX2WorldObject* pTargetWorldObject);
#endif

//{{ seojt // 2008-10-24, 11:11
        /// ��ü �̸����� m_ObjectList[]�� �����Ѵ�.
        void SortOnetime();

        /// �̸����� CX2WorldObject*�� ã�´�.
        /// @param  wstrName_ : CX2WorldObject:m_Name�� �ش��ϴ� �̸��� ����Ѵ�.
        /// @return CX2WorldObject* : �̸��� �ش��ϴ� ��ü�� ���� ���, NULL�� �����Ѵ�.
        /// @note   �� �Լ��� �ð� ���⵵�� O(n)�̴�. ���ɽ����� �������.
        /// @date   jintaeks on 2008-10-21, 17:26
        CX2WorldObject*     FindObjectByName( const std::wstring& wstrName_ );
//}} seojt // 2008-10-24, 11:11

		vector<CKTDGSkyDome*>& GetSkyDomeList() { return m_SkyDomeList; }
#ifdef WORLD_LAYER
        vector<CX2WorldLayer*>& GetWorldLayerList() { return m_LayerList; }
#ifdef X2TOOL
		void SwapWorldLayerId(int iOld, int iNew)
		{
			CX2WorldLayer *pTemp = m_LayerList[iOld];
			m_LayerList[iOld] = m_LayerList[iNew];
			m_LayerList[iNew] = pTemp;
		}
#endif
#endif
		bool				OpenScriptFile( const WCHAR* pFileName, KLuabinder* pKLuabinder = NULL );

		D3DXCOLOR			GetOriginColor(){ return m_OriginColor; }

		//LUA
		void				SetOriginColor_LUA( float fRed, float fGreen, float fBlue );	// ���� ��ü ����
		void				SetLightColor_LUA( float fRed, float fGreen, float fBlue );		// ĳ���� ����
		void				SetLightPos( float x, float y, float z );
		bool				SetLineMap_LUA( const char* pFileName );
		
#ifdef X2TOOL
		bool				GetLineMapByTool( wstring wstrFileName );
#endif

		void				SetBGM_LUA( const char* pFileName );
		void				SetBGMName( const wstring& BGMName_){ m_BGMName = BGMName_; } 
		wstring				GetBGMName() const {return m_BGMName;}
		void				AddEffectBGM_LUA( const char* pFileName );
		void				Add3DEffectBGM_LUA( const char* pFileName, float x, float y, float z, float fMax3DDistance );
		void				StopAllEffectBGM();
		void				PlayAllEffectBGM();
		

		// oasis907 : ����� [2010.12.13] // ���� �Ľ� ���� ������ ���� ����
		bool				SetWorldCamera_LUA( const char* pFileName );
#ifdef KEYFRAME_CAMERA
		void				UnLoadWorldCamera();
#endif KEYFRAME_CAMERA

		void				SetSquareCameraSetting( float fCameraDist, float fLookAtHeight, float fEyeHeight )
		{
			m_SquareCameraData.m_fCameraDist		= fCameraDist;
			m_SquareCameraData.m_fLookAtHeight		= fLookAtHeight;
			m_SquareCameraData.m_fEyeHeight			= fEyeHeight;
		}

		void				SetSquareCameraType( int iType )
		{
			m_SquareCameraData.m_eSquareCameraType = (SQUARE_CAMERA_TYPE) iType;
		}

		CX2WorldObjectParticle*		CreateObjectParticle( CKTDGParticleSystem* pParticleSystem, const char* pszSequenceName );
		CX2WorldObjectParticle*		CreateObjectParticleDelay( CKTDGParticleSystem* pParticleSystem, const char* pszSequenceName, float fDelay );
		
		CX2WorldObjectLightFlow*	CreateObjectLightFlow();
		CX2WorldObjectSkinMesh*		CreateObjectSkinMesh();
		CX2WorldObjectMesh*         CreateObjectMesh();
		CKTDGSkyDome*				CreateSkyDome();
		CKTDGLensFlare*				CreateLensFlare();
#ifdef WORLD_LAYER
        CX2WorldLayer*              CreateWorldLayer();
        void                        RemoveWordLayer(int index);
#endif
	
        void                        ClearObjectMesh();

		void						SetLOD( int lodPercent );
		int							GetLOD() { return m_LodPercent; }

#ifdef FOG_WORLD
        void                        SetFogWorld(float fNearX, float fFarX, float fNearY, float fFarY, float fDensity, D3DXCOLOR fogColor);
        void                        ToggleFog(bool flag = false);
                    
    public:
        bool                        m_bFog;    
        bool                        m_bFogShow;
        float                       m_fNearX;
        float                       m_fFarX;
        float                       m_fNearY;
        float                       m_fFarY;
        float                       m_fDensity;
        D3DXCOLOR                   m_FogColor;
#endif


#ifdef BUBBLE_BOBBLE_TEST
		D3DXVECTOR3 GetLookAt() const { return m_vLookAt; }
		void SetLookAt(D3DXVECTOR3 val) { m_vLookAt = val; }
		D3DXVECTOR3 GetEye() const { return m_vEye; }
		void SetEye(D3DXVECTOR3 val) { m_vEye = val; }
#endif BUBBLE_BOBBLE_TEST

        bool                        GetDistanceColor() { return m_bDistanceColor; }
        void                        SetDistanceColor(bool flag) { m_bDistanceColor = flag; }
        void                        AddSyncObject(CX2WorldObject *object) { m_vecSyncObject.push_back(object); }
        void                        FrameMoveSyncObject(float fx, float fy);
        CX2WorldObject*             GetSyncObject(int index);
		
		void PlayEffectBGM( bool bPlay );
		void Play3DEffectBGM( bool bPlay );
		void SetEffectBGMVolume( float fSoundVolume );


		const CX2World::SquareCameraData& GetSquareCameraData() const { return m_SquareCameraData; }




#ifdef DEEP_WATER_DUNGEON_TEST
		float GetGravityAndSpeedRate() const { return m_fGravityAndSpeedRate; }
		void SetGravityAndSpeedRate(float val) { m_fGravityAndSpeedRate = val; }

		float GetUnitAnimSpeedRate() const { return m_fUnitAnimSpeedRate; }
		void SetUnitAnimSpeedRate(float val) { m_fUnitAnimSpeedRate = val; }
#endif DEEP_WATER_DUNGEON_TEST




		void AddWorldMonster_LUA();
#ifdef MARIO_LIKE_BLOCK_TEST
		std::vector< WorldMonsterData >& GetVecWorldMonsterData() { return m_vecWorldMonsterData; }
#endif MARIO_LIKE_BLOCK_TEST



#ifdef WORLD_TRIGGER
		bool CanActiveTrigger(int iVal)
		{
			if( GetLineMap() == NULL || iVal < 0 )
				return false;

			if( iVal >= (int)GetLineMap()->GetLineMapTrigger()->m_vecTriggerSeg.size() )
				return false;

			CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg = GetLineMap()->GetLineMapTrigger()->m_vecTriggerSeg[iVal];
			if( triggerSeg.m_iTriggerDataIndex < 0 )
				return false;

			for(UINT i=0; i<GetLineMap()->GetLineMapTrigger()->m_vecNowTrigger.size(); ++i)
			{
				int iNowIndex = GetLineMap()->GetLineMapTrigger()->m_vecNowTrigger[i];
				if( iNowIndex == iVal )
					return false;		
			}

			return true;
		}
		void ActiveTrigger(int iVal)
		{
			if( CanActiveTrigger(iVal) == true )
				GetLineMap()->GetLineMapTrigger()->m_vecNowTrigger.push_back(iVal);
		}
		void InactiveTrigger(int iVal)
		{
			if( GetLineMap() == NULL )
				return;

			for(UINT i=0; i<GetLineMap()->GetLineMapTrigger()->m_vecNowTrigger.size(); ++i)
			{
				if( GetLineMap()->GetLineMapTrigger()->m_vecNowTrigger[i] == iVal )
				{
					GetLineMap()->GetLineMapTrigger()->m_vecNowTrigger.erase(GetLineMap()->GetLineMapTrigger()->m_vecNowTrigger.begin()+i);
					return;
				}

			}
		}
		void InitTriggerDone(CKTDGLineMap::TRIGGER_DATA &triggerData, bool bDone = false);
		CX2WorldObjectSkinMesh *GetTriggerMesh(const wstring &wstrMeshName);

		void DoTriggerMesh(CKTDGLineMap::TRIGGER_DATA_MESH &triggerMesh);
		void DoTriggerLine(CKTDGLineMap::TRIGGER_DATA_LINE &triggerLine);
		void DoTriggerEtc(CKTDGLineMap::TRIGGER_DATA_ETC &triggerEtc);
		void DoTriggerFrameMove( double fTime, float fElapsedTime );

		void SetTriggerCamera(int iVal, float fVal) { m_iTriggerCameraIndex = iVal; m_fDurationTriggerCamera = fVal; }
		int	GetTriggerCamera()
		{
			if( m_fDurationTriggerCamera > 0.f && m_iTriggerCameraIndex >= 0 )
			{
				return m_iTriggerCameraIndex;
			}

			return -1;
		}
#endif

#ifdef MODIFY_FRUSTUM
		void SetProjection( float fNear, float fFar, float fPerspective, float fBaseDistance, float fStepDist1, float fStepDist2, float fStepDist3 );
		float GetBaseCameraDistance() { return m_fBaseCameraDistance; }
		float GetCameraStepDistacne(int iStep) { return m_fCameraDistanceStep[iStep]; }
		void SetCameraDistance( float fBaseDist, float fStepDist1, float fStepDist2, float fStepDist3 )
		{
			m_fBaseCameraDistance = fBaseDist;
			m_fCameraDistanceStep[0] = fStepDist1;
			m_fCameraDistanceStep[1] = fStepDist2;
			m_fCameraDistanceStep[2] = fStepDist3;
		}
#endif

#if defined(WORLD_TOOL) || defined(X2TOOL)
		wstring GetBGMName() { return m_BGMName; }
		wstring GetWorldName() { return m_wstrWorldName; }
		void RemoveSkyDome(int index);
		wstring GetWorldCamera() { return m_wstrWorldCamera; }
#endif

    //{{ seojt // 2008-10-24, 14:06
    public:
        void Debug_RenderHelp( int ix_, int iy_ );
    //}} seojt // 2008-10-24, 14:06




	private:
		WorldData*					m_pWorldData; //�⺻ ���� ����Ÿ

		bool						m_bBackgroundLoad;
		CKTDGLineMap*				m_pLineMap;
		CKTDGLineMap*				m_pLineMapFuture;			
		bool						m_bFirstLineMapFrameMove;


		wstring						m_BGMName;

		D3DXVECTOR3					m_LightPos;
		D3DXCOLOR					m_LightColor;			// ĳ���� ���� ����Ǵ� color
		D3DXCOLOR					m_FadeLightColor;		
		float						m_fFadeLightTime;


		D3DXCOLOR					m_OriginColor;			// �������� ��� object color
		D3DXCOLOR					m_FadeOriginColor;		
		float						m_fFadeOriginTime;		


		D3DXCOLOR					m_NowColor;				// ��� object�� ����Ǵ� color, ĳ���͵��� �ʻ�⸦ ����ϰų� ���°� �ٲ� �� �Ź� �� ������ m_OriginColor�� ���µȴ�. ��, instance color
		D3DXCOLOR					m_FadeColor;			
		float						m_fFadeTime;

		


		

		vector<CKTDGSkyDome*>		m_SkyDomeList;
		vector<CX2WorldObjectPtr>   m_ObjectList;
        bool                        m_bObjectListSorted;
		vector<CKTDGLensFlare*>		m_pLensFlareList;
#ifdef WORLD_LAYER
        vector<CX2WorldLayer*>      m_LayerList;
#endif
		vector<CKTDXDeviceSound*>	m_vecEffectBGM;
		vector<CKTDXDeviceSound*>	m_vec3DEffectBGM;


		bool						m_bRenderLineMap;

		int							m_LodPercent;


#ifdef BUBBLE_BOBBLE_TEST
		D3DXVECTOR3					m_vLookAt;	// when it's fixed camera mode
		D3DXVECTOR3					m_vEye;
#endif BUBBLE_BOBBLE_TEST

        bool                        m_bDistanceColor;
        vector<CX2WorldObject*>     m_vecSyncObject;

		KLuabinder*					m_pKLuabinder;



		SquareCameraData			m_SquareCameraData;


#ifdef DEEP_WATER_DUNGEON_TEST
		float						m_fGravityAndSpeedRate; // �߷� �� ĳ���� �����̴� �ӵ��� �������� ��, ���� ���� ����� ���ؼ� ó�� �߰�
		float						m_fUnitAnimSpeedRate;		// ĳ���� �ִϸ��̼� �ӵ��� �������� ��
#endif DEEP_WATER_DUNGEON_TEST


#ifdef MARIO_LIKE_BLOCK_TEST

		std::vector< WorldMonsterData > m_vecWorldMonsterData;		

#endif MARIO_LIKE_BLOCK_TEST


#ifdef WORLD_TRIGGER
		int								m_iTriggerCameraIndex;
		float							m_fDurationTriggerCamera;
#endif

#ifdef MODIFY_FRUSTUM
		float m_fBaseCameraDistance;
		float m_fCameraDistanceStep[3];
#endif

#if defined(WORLD_TOOL) || defined(X2TOOL)
		wstring m_wstrWorldName;
		wstring m_wstrWorldCamera;
#endif
};//class CX2World

