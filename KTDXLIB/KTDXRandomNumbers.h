#pragma once

class CKTDXRandomNumbers
{
public:
	// ���ÿ� ���Ǵ� Ȯ���� ��쿡 ���� �ٸ� random number�� ����ؾ� �ϱ� ������ random table�� Ž���� �� ������ offset�� �����ֵ��� �Ѵ�. 
	// ���� ��� blaze 5%�� ũ��Ƽ�� 5%�̸� blaze�� ũ��Ƽ���� �׻� ���ÿ� ������ �Ǵµ� �̸� ���� ���ؼ�
	enum SPECIAL_RANDOM_OFFSET
	{

		SRO_ATTACK_CRITICAL = 1,
		SRO_HIT_DODGE,
		SRO_ATTACK_EXTRA_DAMAGE_ENCHANT,
		SRO_ATTACK_EXTRA_DAMAGE_MULTIPLE,	// ��ø ������ extra damage Ȯ�� ��꿡 ���
		SRO_IMMUNE_EXTRA_DAMAGE_SCRIPTED,
		SRO_RESIST_EXTRA_DAMAGE_ENCHANT,
		SRO_SUPER_ARMOR_WHEN_ATTACKED,

		SRO_ADD_MANA_ON_ATTACK,
		SRO_ADD_MANA_WHEN_ATTACKED,
		SRO_STAT_PERCENT_UP,
		SRO_CURE_DEBUFF,
		SRO_DEBUFF_ATTACKER,
		SRO_BONE_SHIELD,
		SRO_ATTACK_SPEED_UP,
		//{{ kimhc // 2010.12.29 // �ϸ� ���� �߰��� ���� ��Ʈ ������ ȿ��
		SRO_ICE_NOVA,
		//}} kimhc // 2010.12.29 // �ϸ� ���� �߰��� ���� ��Ʈ ������ ȿ��
#ifdef	SERV_TRAPPING_RANGER_TEST
		SRO_VITALPOINT_PIERCING,
#endif	SERV_TRAPPING_RANGER_TEST
#ifdef ADD_SOCKET_FOR_SECRET_SET
		SRO_POISON_BURST,
		SRO_ABSORB_HP,
#endif
#ifdef HAMEL_SECRET_DUNGEON // ����ȯ
		SRO_ACTIVE_BUFF,
#endif // HAMEL_SECRET_DUNGEON
	};


public:
	CKTDXRandomNumbers(void);
	~CKTDXRandomNumbers(void);

	static float GetRandomFloat( int iIndex );
	static int GetRandomInt( int iIndex );

public:
	static const int RANDOM_NUMBER_COUNT = 32000;
	static const float MIN_PROBABILITY_RATE;
	static const USHORT s_RandomTable[RANDOM_NUMBER_COUNT];

};
