#pragma once

#include "GameEvent.h"

//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
#ifdef SERV_QUEST_ITEM_DROP_EVENT
class KGameEventQuestItemDrop : public KGameEvent
{
public:
	KGameEventQuestItemDrop(void);
	virtual ~KGameEventQuestItemDrop(void);

	void	SetQuestItemDropRate( float fQuestItemDropRate )			{ m_fQuestItemDropRate = fQuestItemDropRate; }
	float	GetQuestItemDropRate()										{ return m_fQuestItemDropRate; }

private:
	float	m_fQuestItemDropRate;
};
#endif SERV_QUEST_ITEM_DROP_EVENT
//}
