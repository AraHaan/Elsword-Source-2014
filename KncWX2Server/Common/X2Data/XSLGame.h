#pragma once

class CXSLGame
{
public:
	CXSLGame(void);
	~CXSLGame(void);

	enum GAME_TYPE
	{
		GT_NONE = 0,
		GT_PVP,
		GT_DUNGEON,
		//{{ 2012. 04. 04	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM
		GT_BATTLE_FIELD,
//#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	};
};
