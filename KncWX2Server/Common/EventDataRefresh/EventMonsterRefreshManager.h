#pragma once
#include "ServerDefine.h"
#include "KncUtil.h"
#include "ServerPacket.h"

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#include <KNCSingleton.h>
#include <ToString.h>
#include <set>
#include <map>
#include "Lottery.h"

class KEventMonsterRefreshManager
{
	DeclareSingleton( KEventMonsterRefreshManager );
	DeclDump;
	DeclToStringW;

public:
	struct KAdventNpcEventInfo
	{
		int					m_iEventID;
		int					m_iMonsterDropCount;
		bool				m_bIsAdventInBossStage;
		KLottery			m_kAdventNpcLot;
		std::set< int >		m_setAdventDungeon;
		std::set< int >		m_setNoAdventDungeon;
		std::set< int >		m_setNoAdventStage;
		//{{ �̺�Ʈ ���� ���� ( �Ʊ� ����	) - ��μ�
#ifdef SERV_ALLY_EVENT_MONSTER
		bool				m_bAllyNPC;
#endif SERV_ALLY_EVENT_MONSTER
		//}}

		KAdventNpcEventInfo()
		{
			m_iEventID				= 0;
			m_iMonsterDropCount		= 0;
			m_bIsAdventInBossStage	= false;
			//{{ �̺�Ʈ ���� ���� ( �Ʊ� ����	) - ��μ�
#ifdef SERV_ALLY_EVENT_MONSTER
			m_bAllyNPC = false;
#endif SERV_ALLY_EVENT_MONSTER
			//}}
		}
	};

public:
	KEventMonsterRefreshManager();
	~KEventMonsterRefreshManager();

	// function
	bool				IsExistEvent( IN int iEventID )	{ return ( m_mapEventMonster.find( iEventID ) != m_mapEventMonster.end() ); }
	void				GetAdventNpcEventInfo( IN const std::vector< int >& vecEventID, OUT std::vector< KAdventNpcEventInfo >& vecInfo );
	void				UpdateEventMonster( void );
	// DB���� �̺�Ʈ ��Ŷ�� �ð��..
	void				SetEventMonsterData( IN const std::map< int, EVENT_MONSTER_DATA >& mapEventMonster );

private:
	std::map< int, KAdventNpcEventInfo >	m_mapEventMonster;
};

DefSingletonInline( KEventMonsterRefreshManager );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}