#pragma once

class   CX2Unit;
class   CX2Unit_PreHeader
{
public:

	enum EQIP_POSITION
	{
		EP_NONE		= 0,
		EP_QUICK_SLOT,			//������ ������

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
		EP_AC_FACE1,			//��(��)
		EP_AC_FACE2,			//��(��)
		EP_AC_FACE3,			//��(��)
		EP_AC_BODY,				//����
		EP_AC_LEG,				//�ٸ�
		EP_AC_ARM,				//��
		EP_AC_RING,				//����
		EP_AC_NECKLESS,			//�����
		EP_AC_WEAPON,			// ���� �Ǽ��縮
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
			
		EP_RAVEN_LEFT_ARM,		// dmlee 2008.07.31 - ���̺� ����, ������ �����鼭	
		EP_WEAPON_SECOND,		// dmlee 2008.12.12 - �� ��° ����

#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
		EP_ONEPIECE_FASHION,	// ���ǽ� �ƹ�Ÿ
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT

		EP_END,
	};
};