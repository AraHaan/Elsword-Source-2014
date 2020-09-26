#pragma once

#include "RefreshSingleton.h"
#include "XSLDungeon.h"

class CXSLDungeonManager
{
	DeclareRefreshSingleton( CXSLDungeonManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	~CXSLDungeonManager();
	CXSLDungeonManager();

	bool	OpenScriptFile_AllDungeonScriptLoad( IN lua_State* pLuaState );

	bool	AddDungeonData_LUA();
	bool	AddDefaultRoomTitle_LUA( int iStringIndex ) { return true; } // Ŭ���̾�Ʈ������ �ʿ��� �Լ�, ��ũ��Ʈ �Ľ��� ���ؼ� ������

#ifdef SERV_CREATED_NPC_LIMITED_DROPS
	int		GetNPCDropTime( IN const int nDungeonID, IN const int iStageIndex, IN const int iSubStageIndex, IN const int iNPCID );
#endif SERV_CREATED_NPC_LIMITED_DROPS

#ifdef SERV_DUNGEON_NPC_DATA_EXP_RATE		// ���볯¥: 2013-08-13
	float	GetNPCExpRate( IN const int nDungeonID, IN const int iStageIndex, IN const int iSubStageIndex, IN const int iNPCID );
#endif // SERV_DUNGEON_NPC_DATA_EXP_RATE

	//{{ 2012. 05. 10	������	���� ���� ���� ����
#ifdef SERV_DUNGEON_NPC_LEVEL
	bool	GetNPCData( IN const int nDungeonID, IN const int nStageNum, IN const int iRelativeMonsterLevel, OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot, OUT bool& bIsBossStage );
#else
	bool	GetNPCData( IN const int nDungeonID, IN const int nStageNum, IN const int iHighestUserLevel, OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot, OUT bool& bIsBossStage );
#endif SERV_DUNGEON_NPC_LEVEL
	//}}

	bool	GetDungeonName( IN int nDungeonID, OUT std::wstring& strDungeonName );
	int		GetDungeonMinLevel( IN int nDungeonID ) const;
	int		GetDungeonMaxLevel( IN int nDungeonID ) const;
	//{{ 2012. 05. 10	������	��Ʋ�ʵ� �ý���
#ifdef SERV_DUNGEON_NPC_LEVEL
	int		GetDungeonNpcLevel( IN const int iDungeonID ) const;
#endif SERV_DUNGEON_NPC_LEVEL
	//}}
	int		GetRequireDungeonID( IN int nDungeonID );
	//{{ 2012. 05. 22	������	���� ���� ������ ����
#ifdef SERV_DUNGEON_REQUIRE_ITEM_LEVEL
	int		GetRequireItemLevel( IN const int iDungeonID ) const;
#endif SERV_DUNGEON_REQUIRE_ITEM_LEVEL
	//}}

	//{{ 2010. 03. 24  ������	��д��� ����
	CXSLDungeonStage::STAGE_TYPE GetStageType( IN int iDungeonID, IN int iStageNum );
	//}}

#ifdef SERV_ADDITIONAL_TICKET
	bool	GetTicketInformation( IN int nDungeonID, OUT std::vector< std::pair< int, int > >& vecTicketInformation );
#endif SERV_ADDITIONAL_TICKET
	//{{ 2010. 03. 18  ������	��д��� ����	
	bool	GetRequireItemIDAndCount( IN int nDungeonID, IN CXSLDungeon::DUNGEON_MODE eDungeonMode, OUT int& iItemID, OUT int& iItemCount );
	//}}

	//{{ 2007. 10. 4  ������  �ټ��� ��� �Լ�
	bool	GetRequireSpiritCount( IN int nDungeonID, OUT int& nSpiritByDungeon );
	//}}
	
	int		GetStartPosByRandom( IN int nDungeonID, IN int iStageIndex, IN int iSubStageIndex );
	//{{ 2010. 04. 26  ������	��д��� ����
#ifdef SERV_SECRET_HELL
	void	GetExtraNpcInfo( IN int nDungeonID, IN int iStageIndex, IN int iSubStageIndex, OUT int& iStartPos, OUT bool& bIsRight );
	float	GetPoisonTimeLimit( IN int nDungeonID );
	float	GetExtraStagePoisonTimeLimit( IN int nDungeonID );
#endif SERV_SECRET_HELL
	//}}
	float	GetPlayTimeLimit( IN int nDungeonID );

	//{{ 2009. 4. 28  ������	����id�˻�
	bool	VerifyDungeonID( IN int nDungeonID ) { return ( m_mapDungeonData.find( static_cast<SEnum::DUNGEON_ID>(nDungeonID) ) != m_mapDungeonData.end() ); }
	//}}

	//{{ 2009. 7. 3  ������		��ϸ�����
	CXSLDungeon::DUNGEON_TYPE GetDungeonType( IN int nDungeonID );
	//}}

	// ��д��� ����	
	void	SetSecretDungeonExtraStageNpcLevel_LUA( int iLevel );
	void	AddExtraStageCondition_LUA( int iBeginLevel, int iEndLevel, float fFactor );
	//{{ 2012. 11. 9	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	SetExtraStageFactor_LUA( float fFactor );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	int		GetSecretDungeonExtraStageNpcLevel()					{ return m_iSecretDungeonExtraStageNpcLevel; }
	float	GetExtraStageConditionFactor( IN char cLevel );
	//{{ 2012. 11. 9	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	float	GetExtraStageFactor() const								{ return m_fExtraStageFactor; }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

#ifdef SERV_DUNGEON_OPTION_IN_LUA
	bool	IsFixedMembers( IN int nDungeonID );
	short	GetFixedMembers( IN int nDungeonID );
	bool	IsEventDungeon( IN int nDungeonID );
#endif SERV_DUNGEON_OPTION_IN_LUA
	
#ifdef SERV_NEW_EVENT_TYPES
	bool	IsSwitchingWithEventInfo( IN int nDungeonID );
	void	GetEventDungeons( OUT std::map< int, bool >& mapEventDungeons ) { mapEventDungeons = m_mapEventDungeonEnable; }
#endif SERV_NEW_EVENT_TYPES	

	//{{ 2011. 11. 1	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool	GetCanBeEnteredDungeonID( IN const std::map< int, KDungeonClearInfo >& mapDungeonClear, OUT int& iDungeonID );
	void	GetNormalDungeonIDList( OUT std::vector< int >& vecNormalDungeonIDList );
	void	GetDefenceDungeonIDList( OUT std::vector< int >& vecDefenceDungeonIDList );
	bool	CheckMinLevelForEnterDungeon( IN const int iDungeonID, IN const u_char ucLevel ) const;
	bool	CheckLevelForDungeonAutoPartyBonus( IN const int iDungeonID, IN const u_char ucLevel ) const;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

#ifdef SERV_STAGE_CLEAR_IN_SERVER// �۾���¥: 2013-10-30	// �ڼ���
	int		GetSecretStageEnteringEvent( IN const int nDungeonID, IN const int iStageIndex, IN const int iSubStageIndex, IN int iClearConditionIndex );
	bool	GetNextStage( OUT CXSLDungeonSubStage::NextStageData& kNextStageData, IN const int nDungeonID, IN const int iStageIndex, IN const int iSubStageIndex, IN int iClearConditionIndex, IN int iSecretPadIndex = 0 );
#endif // SERV_STAGE_CLEAR_IN_SERVER

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
	bool	GetLimitedPlayTimes( IN const int nDungeonID, OUT int& iPlayTimes );
	bool	GetLimitedClearTimes( IN const int nDungeonID, OUT int& iClearTimes );
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

private:
	std::map< SEnum::DUNGEON_ID, CXSLDungeon >				m_mapDungeon;
	std::map< SEnum::DUNGEON_ID, CXSLDungeon::DungeonData >	m_mapDungeonData;

#ifdef SERV_NEW_EVENT_TYPES
	std::map< int, bool >						m_mapEventDungeonEnable;
#endif SERV_NEW_EVENT_TYPES

	// ��д��� ����
	int																m_iSecretDungeonExtraStageNpcLevel;
	std::map< char, float >											m_mapExtraStageConditionFactor;
	//{{ 2012. 11. 9	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	float															m_fExtraStageFactor;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
};

DefRefreshSingletonInline( CXSLDungeonManager );


