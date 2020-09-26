#pragma once

#include "RefreshSingleton.h"
#include <set>
#include <map>
#include <KncUtil.h>
#include "Lottery.h"
#include "CommonPacket.h"


class KHenirResultTable
{
	DeclareRefreshSingleton( KHenirResultTable );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclToStringW;
	DeclDump;

public:

	// Henir Table Key
	typedef std::pair< int, char > KHenirTableKey;  // pair[ Stage Count, Henir Dungeon Mode ]

	// Henir Reward
	struct KHenirReward
	{        
		int			m_iGroupID;
		int			m_iRandomCount;		

		KHenirReward()
		{
			m_iGroupID		= 0;
			m_iRandomCount  = 0;
		}
	};

	//{{ 2011. 08. 22	������	��ϸ� �ð� ����
#ifdef SERV_NEW_HENIR_TEST
	struct SHenirChallangeReward
	{
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-23	// �ڼ���
		char					m_cHenirDungeonMode;
#endif // SERV_HENIR_RENEWAL_2013
		std::map< int, int >	m_mapReward;
		KLottery				m_kReward;
	};
#endif SERV_NEW_HENIR_TEST
	//}}

public:
	KHenirResultTable(void);
	~KHenirResultTable(void);

	// for lua
	bool AddHenirResultItemInfo_LUA( IN int iStageCount, IN char cHenirDungeonMode, IN int iItemGroupID, IN int iRandomCount );
	bool AddHenirResultItemGroup_LUA( IN int iItemGroupID, IN int iItemID, IN int iQuantity, IN float fRate );
	bool AddResurrectionStage_LUA( IN int iStageCount );
	//{{ 2009. 12. 14  ������	��ϸ��ӽ��κ�
	bool AddClearTempInventoryStage_LUA( IN int iStageID );
	//}}
	bool AddClearNotifyStage_LUA( IN int iStageCount );	
	
	// ��ϸ� �ð� ����
	//{{ 2011. 08. 23	������	��ϸ� �ð� ����
#ifdef SERV_NEW_HENIR_TEST
	bool GetHenirRewardItem( IN const int iStageID, IN const char cDungeonMode, IN OUT std::map< int, KItemInfo >& mapRewardItem );
#else
	bool GetHenirRewardItem( IN int iStageCount, IN char cDungeonMode, OUT std::map< int, KItemInfo >& mapRewardItem );
#endif SERV_NEW_HENIR_TEST
	//}}

	//{{ 2011. 08. 22	������	��ϸ� �ð� ����
#ifdef SERV_NEW_HENIR_TEST
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-23	// �ڼ���
	bool GetHenirChallangeRewardItem( IN char cHenirDungeonMode, IN const int iStageID, IN OUT std::map< int, KItemInfo >& mapRewardItem ) const;
#else // SERV_HENIR_RENEWAL_2013
	bool GetHenirChallangeRewardItem( IN const int iStageID, IN OUT std::map< int, KItemInfo >& mapRewardItem );
#endif // SERV_HENIR_RENEWAL_2013
#endif SERV_NEW_HENIR_TEST
	//}}

	// ��ϸ� �ð� �ڵ� ��Ȱ ��������
	bool IsResurrectionStage( int iStageCount )		{ return ( m_setResurrectionStage.find( iStageCount ) != m_setResurrectionStage.end() ); }

	//{{ 2009. 12. 14  ������	��ϸ��ӽ��κ�
	bool IsClearTempInventoryStage( IN int iStageID ) { return ( m_setClearTempInventoryStage.find( iStageID ) != m_setClearTempInventoryStage.end() ); }
	//}}

	// ��ϸ� �ð� Ŭ���� ���� ��������
	bool IsClearNotifyStage( int iStageCount )		{ return ( m_setClearNotifyStage.find( iStageCount ) != m_setClearNotifyStage.end() ); }

	//{{ 2011. 08. 22	������	��ϸ� �ð� ����
#ifdef SERV_NEW_HENIR_TEST
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-23	// �ڼ���
	bool AddHenirChallangeRewardInfo_LUA( IN char cHenirDungeonMode, IN int iStageID, IN int iItemID, IN int iQuantity );
#else // SERV_HENIR_RENEWAL_2013
	bool AddHenirChallangeRewardInfo_LUA( IN int iStageID, IN int iItemID, IN int iQuantity );
#endif // SERV_HENIR_RENEWAL_2013
#endif SERV_NEW_HENIR_TEST
	//}}

#ifdef SERV_HENIR_NO_NOTIFY
	bool SetEnbleNotify_LUA( IN bool bVal );
	bool GetEnbleNotify();
#endif // SERV_HENIR_NO_NOTIFY

private:
	std::map< KHenirTableKey, std::vector< KHenirReward > >	m_mapHenirReward;
	std::map< int, KLottery >								m_mapRewardGroup;
	//{{ 2011. 08. 22	������	��ϸ� �ð� ����
#ifdef SERV_NEW_HENIR_TEST

#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-23	// �ڼ���
	typedef std::map< int, std::vector< SHenirChallangeReward > >	TYPE_CHALLANGE_REWARD;
	TYPE_CHALLANGE_REWARD	m_mapChallangeReward;
#else // SERV_HENIR_RENEWAL_2013
	std::map< int, SHenirChallangeReward >					m_mapChallangeReward;
#endif // SERV_HENIR_RENEWAL_2013

#endif SERV_NEW_HENIR_TEST
	//}}

	std::set< int >											m_setResurrectionStage;
	//{{ 2009. 12. 14  ������	��ϸ��ӽ��κ�
	std::set< int >											m_setClearTempInventoryStage;
	//}}
	std::set< int >											m_setClearNotifyStage;

#ifdef SERV_HENIR_NO_NOTIFY
	bool													m_bNofity;
#endif // SERV_HENIR_NO_NOTIFY
};

DefRefreshSingletonInline( KHenirResultTable );



