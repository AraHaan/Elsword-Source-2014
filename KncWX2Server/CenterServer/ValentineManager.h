#pragma once


#include "Event.h"
#include "CommonPacket.h"
#include <boost/timer.hpp>

//{{ 2013. 02. 01  �̺�Ʈ �߷�Ÿ�� ���� - ��μ�
#ifdef SERV_EVENT_VALENTINE_DUNGEON

class KValentineManager
{

public:
	KValentineManager(void);
	~KValentineManager(void);

	enum ENUM_ETC_TIMER
	{
		TEM_AFTER_DUNGEON_START,			// ���� ���� �� 'START' UI ����� ���ÿ� �귯���� �ð�
		TEM_AFTER_DUNGEON_START_5_SEC,		// ���� ���� �� 'START' UI ����� ���ÿ� �귯���� �ð� ���� 5��
		TEM_TOTAL_NUM,
	};

	enum ENUM_LIMITE_DUNGEON_TIME
	{
		LDT_VALENTINE = 99,			// ���� ���� �ð�
	};
	
	enum ENUM_TIME_TYPE
	{
		TT_NONE,
		TT_SYNC_TIME,
		TT_END_GAME,
	};

	void Init();
	bool SetStartTimer( IN int iEnum, IN bool bStart );
	bool IsOnTimer( IN int iEnum );
	void CheckTimer( IN int iEnum, OUT int& iType );
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
	void CheckTimerForInt( IN int iEnum, OUT int& iType );		// �ؿ��� �߷�Ÿ�� �̺�Ʈ ������ �Լ� �߰�
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
	int GetRemainTime( IN int iEnum );

private:
	boost::timer		m_kTimer[TEM_TOTAL_NUM];
	bool				m_bOnTimer[TEM_TOTAL_NUM];
};

#endif SERV_EVENT_VALENTINE_DUNGEON
//}