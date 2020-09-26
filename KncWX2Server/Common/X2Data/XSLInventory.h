#pragma once

//#include "X2Data/XSLItem.h"
class CXSLItem;

class CXSLInventory
{
public:
	enum SORT_TYPE
	{
		ST_NONE = 0,
		ST_EQUIP,		// [1]  ���
		ST_ACCESSORY,	// [2]  �Ǽ��縮
		ST_MATERIAL,	// [3]  ���
		ST_SPECIAL,		// [4]  Ư��
		ST_QUEST,		// [5]  ����Ʈ
		ST_QUICK_SLOT,	// [6]  �Һ�
		ST_AVARTA,		// [7]  �ƹ�Ÿ
		//{{ 2009. 8. 1  ������		����ī�װ�
		ST_BANK,		// [8]  ����
		//}}
		ST_E_EQUIP,		// [9]  �������� ���
		ST_E_SKILL,		// [10] �������� ��ų(�Ⱦ�)
		ST_E_QUICK_SLOT,// [11] �������� �Һ���
		ST_PC_BANG,		// [12] PC�� ���� �κ�
		//{{ 2010. 8. 2	������	�� �ý���
		ST_PET,			// [13] �� �κ��丮
		//}}
		//#ifdef	SERV_SHARING_BANK_TEST
		ST_SHARE_BANK,	// [14] ���� ����
		//#endif	SERV_SHARING_BANK_TEST
		//{{ 2013. 04. 10	������	����� �� ����
		//#ifdef SERV_NEW_DEFENCE_DUNGEON
		ST_E_DEFENCE_QUICK_SLOT, // [15] ����ǹ� ���� ������
		//#endif SERV_NEW_DEFENCE_DUNGEON
		//}}

		ST_END,
	};

	enum MEMBERSHIP_PRIVATE_BANK
	{
		MPB_NONE		= 0,
		MPB_NORMAL		= 8,
		MPB_SILVER		= 16,
		MPB_GOLD		= 24,
		MPB_EMERALD		= 32,
		MPB_DIAMOND		= 40,
		MPB_PLATINUM	= 48,
	};

	enum INVENTORY_INFO
	{
        SLOT_COUNT_ONE_LINE = 8,
		//{{ 2011. 12. 13  ��μ�	�� ���� Ȯ�� ������
#ifdef SERV_EXPAND_QUICK_SLOT
		EXPAND_QUICK_SLOT   = 3,
#endif SERV_EXPAND_QUICK_SLOT
		//}}
	};

#ifdef SERV_REMOVE_SEAL_COUNT_DECREASE
	enum SEAL_COUNT
	{
		SEAL_COUNT_MAX = 255,
	};
#endif //SERV_REMOVE_SEAL_COUNT_DECREASE

private:
	struct InvenSlotData
	{
		SORT_TYPE	m_SortType;
		int			m_SlotID;

		CXSLItem*	m_pItem;

		InvenSlotData()
		{
			m_SortType	= ST_NONE;
			m_SlotID	= -1;
			m_pItem		= 0;
		}
		~InvenSlotData()
		{
			//SAFE_DELETE( m_pItem );
		}
	};

public:
	CXSLInventory(){};
	~CXSLInventory(){};

	static bool IsTradePossibleCategory( char cSlotCategory );
	static bool IsValidBankMemberShip( MEMBERSHIP_PRIVATE_BANK eGrade );
	static bool IsUserInventoryCategory( char cSlotCategory );
	static MEMBERSHIP_PRIVATE_BANK GetNextBankMemberShip( MEMBERSHIP_PRIVATE_BANK eGrade );
};


