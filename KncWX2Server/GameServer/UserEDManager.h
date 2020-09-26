#pragma once
#include "CacheData.h"


//{{ 2012. 09. 28	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM


class KUserEDManager
{
public:
	enum ED_REASON
	{
		ER_NO_REASON = 0,

		// set
		ER_SET_ED_SELECT_UNIT,							// ĳ���� ���ý� �����Ǵ� ED��
		ER_SET_ED_SYNC_FOR_SEND_LETTER,					// ���� ������ ���� ����ȭ �ϴ� ED��
		ER_SET_ED_SYNC_FOR_SEND_LEETER_ROLLBACK,		// ���� ������ ���� ����ȭ �ϴ� ED �ѹ� ��
		ER_SET_ED_SEND_LETTER_ATTACH_ROLLBACK,			// ���� ���� ���з� ���� ÷�� ED �ѹ� ��
		ER_SET_ED_PERSONAL_SHOP_SELL_ITEM,				// ���� ���� ������ �Ǹ�
		ER_SET_ED_PERSONAL_SHOP_BUY_ITEM,				// ���� ���� ������ ����
		ER_SET_ED_PSHOP_AGENCY_SELL_ITEM,				// �븮 ���� ������ �Ǹ�
		ER_SET_ED_PSHOP_AGENCY_BUY_ITEM,				// �븮 ���� ������ ����
		ER_SET_ED_IN_TRADE,								// ���ΰŷ��� �޴� ED
		ER_SET_ED_OUT_TRADE,							// ���ΰŷ��� ������ ED

		// in
		ER_IN_ED_DUNGEON_DROP,							// ���� ED���
		ER_IN_ED_SELL_ITEM_NPC_SHOP,					// NPC������ ������ ���ȱ�
		ER_IN_ED_DUNGEON_CLEAR_BONUS,					// ���� Ŭ���� ���ʽ� ED
		ER_IN_ED_BUY_ITEM_NPC_SHOP_ROLLBACK,			// NPC�������� ������ ���� ���н� ED �ѹ� ��
		ER_IN_ED_REG_GUILD_AD_COMMISSION_ROLLBACK,
		ER_IN_ED_MODIFY_GUILD_AD_COMMISSION_ROLLBACK,
		ER_IN_ED_SEND_LETTER_COMMISSION_ROLLBACK,
		ER_IN_ED_SEND_LETTER_ATTACH_ROLLBACK,
		ER_IN_ED_SYNC_FOR_SEND_LEETER_ROLLBACK,
		ER_IN_ED_GET_ED_FROM_LETTER,
		ER_IN_ED_PERSONAL_SHOP_SELL_ITEM,
		ER_IN_ED_PSHOP_AGENCY_SELL_ITEM,		
		ER_IN_ED_TRAINING_REWARD,
		ER_IN_ED_QUEST_REWARD,

		// out
		ER_OUT_ED_BUY_ITEM_NPC_SHOP,
		ER_OUT_ED_ITEM_REPAIR,
		ER_OUT_ED_ITEM_MANUFACTURE,
		ER_OUT_ED_ITEM_ENCHANT,
		ER_OUT_ED_ITEM_SOCKET,
		ER_OUT_ED_ITEM_ATTRIBUTE,
		ER_OUT_ED_SEND_LETTER_COMMISSION,
		ER_OUT_ED_SEND_LETTER_ATTACH,
		ER_OUT_ED_REG_GUILD_AD_COMMISSION,
		ER_OUT_ED_MODIFY_GUILD_AD_COMMISSION,
		ER_OUT_ED_PERSONAL_SHOP_OPEN_COMMISSION,
		ER_OUT_ED_PERSONAL_SHOP_BUY_ITEM,
		ER_OUT_ED_PSHOP_AGENCY_BUY_ITEM,
		ER_OUT_ED_USE_COBO_EXPRESS_TICKET,
		//{{ 2012. 11. 26 ť�� ED ���� ���� ��� �߰� - ��μ�
//#ifdef SERV_CUBE_OPEN_ED_CONDITION
		ER_OUT_ED_USE_CUBE_OPEN,
//#endif SERV_CUBE_OPEN_ED_CONDITION
		//}
		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
//#ifdef SERV_RELATIONSHIP_SYSTEM
		ER_OUT_ED_BREAK_UP_RELATIONSHIP,
//#endif SERV_RELATIONSHIP_SYSTEM
		//}
		ER_MAX,
	};

public:
	KUserEDManager(void);
	~KUserEDManager(void);

	// �ʱ�ȭ
	void	Clear();
	void	ClearStatistics();
	void	Init( IN const int iED, IN const ED_REASON eReason );

	// ���� ����
	int		GetED() const			{ return m_iED; }
	int		GetChangeValue() const	{ return m_iED.GetChangeValue(); }
	const std::map< int, __int64 >& GetEDStatistics() { return m_mapEdStatistics; }

	// DB������Ʈ
	void	GetDBUpdateInfo( OUT int& iED );
	void	RollBackDBUpdateInfo( IN const int iED );

	// ED���� & ����
	bool	IncreaseMyED( IN const int iIncreaseED, IN const ED_REASON eReason );
	bool	DecreaseMyED( IN const int iDecreaseED, IN const ED_REASON eReason );

private:
	void	AddEdStatistics( IN const int iED, IN const ED_REASON eReason );

private:
	KCacheDataMaxLimit< int, __int64 >	m_iED;
	std::map< int, __int64 >			m_mapEdStatistics;
};


#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
//}}