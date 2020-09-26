#pragma once
#include "ServerDefine.h"
#include "KncUtil.h"

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB

//{{ 2011. 08. 29	������	ĳ���� ������ �̺�Ʈ
#ifdef SERV_CHAR_LEVEL_UP_EVENT

#include <KNCSingleton.h>
#include <ToString.h>
#include <map>
#include <vector>

class KEventRewardLevelUpRefreshManager
{
	DeclareSingleton( KEventRewardLevelUpRefreshManager );
	DeclToStringW;
	DeclDump;

public:
	KEventRewardLevelUpRefreshManager(void);
	~KEventRewardLevelUpRefreshManager(void);

	bool GetLevelUpRewardItem( IN u_char& ucLevel, OUT std::vector< int >& vecRewardID );
	void UpdateRewardLevelUp( void );
	void SetRewardLevelUp( IN const std::map< u_char, std::vector< int > >& m_mapLevelUpRewardItem );

private:
	std::map< u_char, std::vector< int > > m_mapLevelUpRewardItem;
};

DefSingletonInline( KEventRewardLevelUpRefreshManager );

#endif SERV_CHAR_LEVEL_UP_EVENT
//}}

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}