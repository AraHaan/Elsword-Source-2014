#pragma once

#include "GameEvent.h"

class KGameEventSecretDungeon : public KGameEvent
{
public:
	KGameEventSecretDungeon();
	virtual ~KGameEventSecretDungeon();

	//{{ 2010. 10. 19	������	��д��� �̺�Ʈ ������Ʈ
//#ifdef SERV_SECRET_DUNGEON_EVENT
	void	SetDungeonID( int iDungeonID )			{ m_iDungeonID = iDungeonID; }
	int		GetDungeonID()							{ return m_iDungeonID; }

	void	SetDungeonMode( int iDungeonMode )		{ m_iDungeonMode = iDungeonMode; }
	int		GetDungeonMode()						{ return m_iDungeonMode; }

private:
	int		m_iDungeonID;
	int		m_iDungeonMode;
//#endif SERV_SECRET_DUNGEON_EVENT
	//}}
};