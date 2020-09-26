#pragma once

#include <windows.h>
#include <RTTI.h>
//#include <ToString.h>
#include "RefreshSingleton.h"
//#include <KncLua.h>
//#include <lua_tinker.h>
//#include "Lua/KLuaManager.h"
#include "RoomUserManager.h"
//{{ 2011. 01. 20	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
	#include "RoomMonsterManager.h"
#endif SERV_DUNGEON_RANK_NEW
//}}
#include "DungeonMonsterManager.h"

#include "x2data/XSLDungeon.h"
#include "X2Data/XSLUnit.h"
#include "X2Data/XSLRoom.h"


class KResultProcess
{
	NiDeclareRootRTTI( KResultProcess );
	DeclareRefreshSingleton( KResultProcess );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	enum DUNGEON_PARTY_NUM
	{
		NUM_02	= 2,
		NUM_03	= 3,
		NUM_04	= 4,
		NUM_TOTAL,
	};

	enum GAME_EVENT_VAR
	{
		GEV_EXP = 0,
		GEV_VP,
		GEV_ED,

		GEV_PARTY_EXP,
		GEV_PARTY_VP,
		GEV_PARTY_ED,

		GEV_END,
	};

	struct KTeamData
	{
		int		m_nMaxAlive;
		float	m_fMaxHP;
		int		m_nAverageLevel;
		int		m_nUnitNum;

		void Init()
		{
			m_nMaxAlive		= 0;
			m_fMaxHP		= 0;
			m_nAverageLevel	= 0;
			m_nUnitNum		= 0;
		}
	};

public:
	KResultProcess(void);
	~KResultProcess(void);

	//{{ 2011. 07. 19	������	���� ����
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////
	bool		Result_TEAM( IN const float fTotalPlayTime, 
							 IN const float fRemainingPlayTime, 
							 IN const int iPVPChannelClass, 
							 IN const bool bOfficialPvpCancel, 
							 IN const SEnum::MATCH_TYPE eMatchType,
							 IN const SEnum::PVP_NPC_TYPE ePvpNpcType, 
							 IN OUT KRoomUserManagerPtr spRoomUserManager, 
							 OUT KEGS_END_GAME_PVP_RESULT_DATA_NOT& kPacketNot,
							 OUT KPvpMatchUserResultLog::END_TYPE& eEndType );

	bool		Result_SURVIVAL( IN const float fTotalPlayTime, 
								 IN const float fRemainingPlayTime, 
								 IN const int iPVPChannelClass, 
								 IN const bool bOfficialPvpCancel, 
								 IN const SEnum::MATCH_TYPE eMatchType,
								 IN const SEnum::PVP_NPC_TYPE ePvpNpcType, 
								 IN OUT KRoomUserManagerPtr spRoomUserManager, 
								 OUT KEGS_END_GAME_PVP_RESULT_DATA_NOT& kPacketNot,
								 OUT KPvpMatchUserResultLog::END_TYPE& eEndType );

	bool		Result_TEAM_DEATH( IN const float fTotalPlayTime, 
								   IN const float fRemainingPlayTime, 
								   IN const int iPVPChannelClass, 
								   IN const bool bOfficialPvpCancel, 
								   IN const SEnum::MATCH_TYPE eMatchType,
								   IN const SEnum::PVP_NPC_TYPE ePvpNpcType, 
								   IN OUT KRoomUserManagerPtr spRoomUserManager, 
								   OUT KEGS_END_GAME_PVP_RESULT_DATA_NOT& kPacketNot,
								   OUT KPvpMatchUserResultLog::END_TYPE& eEndType );
//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////
	bool		Result_TEAM( IN const float fTotalPlayTime, 
							 IN const float fRemainingPlayTime, 
							 IN const int iPVPChannelClass, 
							 IN OUT KRoomUserManagerPtr spRoomUserManager, 
							 OUT KEGS_END_GAME_PVP_RESULT_DATA_NOT& kPacketNot );

	bool		Result_SURVIVAL( IN const float fTotalPlayTime, 
								 IN const float fRemainingPlayTime, 
								 IN const int iPVPChannelClass, 
								 IN OUT KRoomUserManagerPtr spRoomUserManager, 
								 OUT KEGS_END_GAME_PVP_RESULT_DATA_NOT& kPacketNot );

	bool		Result_TEAM_DEATH( IN const float fTotalPlayTime, 
								   IN const float fRemainingPlayTime, 
								   IN const int iPVPChannelClass, 
								   IN OUT KRoomUserManagerPtr spRoomUserManager, 
								   OUT KEGS_END_GAME_PVP_RESULT_DATA_NOT& kPacketNot );
//////////////////////////////////////////////////////////////////////////
#endif SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////
	//}}	
	
	bool		Result_DUNGEON( IN OUT KRoomUserManagerPtr spRoomUserManager, 
								IN OUT KDungeonMonsterManager& kDungeonMonsterManager, 
								IN const bool bIsWin, 
								IN const CXSLDungeon::DUNGEON_TYPE eDungeonType,
								IN const char cDungeonMode, 
								IN const char cDifficulty, 
								IN const int iDungeonID, 
								//{{ 2011. 08. 22	������	��ϸ� �ð� ����
#ifdef SERV_NEW_HENIR_TEST
								IN const int iStageID,
#endif SERV_NEW_HENIR_TEST
								//}}
								IN const int nTotalPlayTime, 
								IN const int nRemainingTime, 
								IN const int nDropItemUID, 
								IN const int iStartedNumMember, 
								//{{ 2012. 03. 15	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
								IN const bool bStartedByAutoParty,
#endif SERV_BATTLE_FIELD_SYSTEM
								//}}
								OUT int& iClearStageCount,
								OUT KEGS_END_GAME_DUNGEON_RESULT_DATA_NOT& kUnitInfoNot,
								OUT std::vector< KRoomUserPtr >& vecKRoomUserPtr,
								OUT std::vector< KERM_UPDATE_DUNGEON_UNIT_INFO_NOT >& vecUpdateUnitInfo
#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
								,IN const int iTotalValentineTIme
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
								);

	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
	bool		Result_DefenceDUNGEON( IN OUT KRoomUserManagerPtr spRoomUserManager, 
									   IN OUT KDungeonMonsterManager& kDungeonMonsterManager, 
									   IN const bool bIsWin, 
									   IN const CXSLDungeon::DUNGEON_TYPE eDungeonType,
									   IN const char cDungeonMode,
									   IN const char cDifficulty, 
									   IN const int iDungeonID, 
									   IN const int nTotalPlayTime, 
									   IN const int nRemainingTime, 
									   IN const int nDropItemUID, 
									   IN const int iStartedNumMember, 
									   //{{ 2012. 03. 15	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
									   IN const bool bStartedByAutoParty,
#endif SERV_BATTLE_FIELD_SYSTEM
									   //}}
									   OUT int& iClearStageCount,
									   OUT KEGS_END_GAME_DUNGEON_RESULT_DATA_NOT& kUnitInfoNot,
									   OUT std::vector< KRoomUserPtr >& vecKRoomUserPtr,
									   OUT std::vector< KERM_UPDATE_DUNGEON_UNIT_INFO_NOT >& vecUpdateUnitInfo,
									   IN int iProtectedCrystalCount );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
/*																// �򰥷��� �ּ� ó��
	
*/	

	//{{ 2011. 07. 25	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	bool		Result_TEAM_BREAK(			IN KRoomUserManagerPtr spRoomUserManager, IN KRoomUserPtr spRoomUser, IN const int iReason, IN const SEnum::MATCH_TYPE eMatchType, IN const SEnum::PVP_NPC_TYPE ePvpNpcType, OUT KERM_UPDATE_PVP_UNIT_INFO_NOT& kInfo );
	bool		Result_TEAM_DEATH_BREAK(	IN KRoomUserManagerPtr spRoomUserManager, IN KRoomUserPtr spRoomUser, IN const int iReason, IN const SEnum::MATCH_TYPE eMatchType, IN const SEnum::PVP_NPC_TYPE ePvpNpcType, OUT KERM_UPDATE_PVP_UNIT_INFO_NOT& kInfo );
	bool		Result_SURVIVAL_BREAK(		IN KRoomUserManagerPtr spRoomUserManager, IN KRoomUserPtr spRoomUser, IN const int iReason, IN const SEnum::MATCH_TYPE eMatchType, IN const SEnum::PVP_NPC_TYPE ePvpNpcType, OUT KERM_UPDATE_PVP_UNIT_INFO_NOT& kInfo );
#else
	bool		Result_TEAM_BREAK(			IN KRoomUserManagerPtr spRoomUserManager, IN KRoomUserPtr spRoomUser, OUT KERM_UPDATE_PVP_UNIT_INFO_NOT& kInfo );
	bool		Result_TEAM_DEATH_BREAK(	IN KRoomUserManagerPtr spRoomUserManager, IN KRoomUserPtr spRoomUser, OUT KERM_UPDATE_PVP_UNIT_INFO_NOT& kInfo );
	bool		Result_SURVIVAL_BREAK(		IN KRoomUserManagerPtr spRoomUserManager, IN KRoomUserPtr spRoomUser, OUT KERM_UPDATE_PVP_UNIT_INFO_NOT& kInfo );
#endif SERV_PVP_NEW_SYSTEM
	//}}	
	bool		Result_DUNGEON_BREAK(		IN KRoomUserPtr spRoomUser, OUT KERM_UPDATE_DUNGEON_UNIT_INFO_NOT& kInfo );

	//{{ 2011. 08. 03	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	int			DecidePVPWinTeam( IN const float fTotalPlayTime, IN const float fRemainingPlayTime, IN OUT KRoomUserManagerPtr spRoomUserManager, IN const CXSLRoom::PVP_GAME_TYPE eGameType, OUT KPvpMatchUserResultLog::END_TYPE& eEndType );
#else
	int			DecidePVPWinTeam( IN const float fTotalPlayTime, IN const float fRemainingPlayTime, IN OUT KRoomUserManagerPtr spRoomUserManager, IN const CXSLRoom::PVP_GAME_TYPE eGameType );
#endif SERV_PVP_NEW_SYSTEM
	//}}	

	void		AddDungeonPartyBonus_LUA( float fTwo, float fThree, float fFour );	//���� ��Ƽ���ʽ� Ȯ��.!
	
	float		GetDungeonPartyBonusRate( int iNumber );	//���� �ο��� Ȯ�� ����.	

	//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	float		GetDungeonPartyDifficultyBonusRate( char cDifficulty );	//���� ���̵��� Ȯ�� ����.
	void		AddDungeonPartyDifficultyBonus_LUA( float fNormal, float fHard, float fExpert );
	float		GetDungeonModePartyBonusRate( char cDungeonMode );
	void		AddDungeonModePartyBonus_LUA( float fNormal, float fHenir, float fSecret );
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

	//{{ 2013. 02. 25   ���� ��Ż �� ��ũ ��� - ��μ�
#ifdef SERV_LEAVE_ROOM_DUNGEON_RANK
	void		LeaveRoomResultRank( IN int							iDungeonID, 
									 IN char						cDifficultyLevel, 
									 KRoomUserPtr					spRoomUser, 
									 IN OUT KDungeonMonsterManager& kDungeonMonsterManager,
									 IN int							iEndNumMember, 
									 IN int							iPartyTotalGivenDamage,
									 IN int							iTotalPlayTime,
									 KERM_UPDATE_DUNGEON_UNIT_INFO_NOT& kUpdateUnitInfoDummy, 
									 KDungeonUnitResultInfo&		kDungeonUnitInfoDummy
									);
#endif SERV_LEAVE_ROOM_DUNGEON_RANK
	//}

	//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	char		GetSubStageComboResult( IN int iDungeonIDDiff, IN int iEndNumMember, IN int iCombo, IN int iMonsterPoint );
	char		GetSubStageTechResult( IN int iDungeonIDDiff, IN int iEndNumMember, IN int iTech, IN int iMonsterPoint );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}

protected:
	//�Ʒ��� ���� �Լ��� �����ϰ�� ������� ����ؾ� ��.
	void		SortbyPVPMD( std::vector<KPVPUnitResultInfo>& vecData );
	void		SortbyPVPKill( std::vector<KPVPUnitResultInfo>& vecData );
	void		SortbyPVPHP( std::vector<KPVPUnitResultInfo>& vecData, KRoomUserManagerPtr spRoomUserManager, CXSLRoom::PVP_GAME_TYPE eGameType );
	void		SortbyPVPLevel( std::vector<KPVPUnitResultInfo>& vecData, KRoomUserManagerPtr spRoomUserManager, CXSLRoom::PVP_GAME_TYPE eGameType );
	void		SortbyPVPSlot( std::vector<KPVPUnitResultInfo>& vecData, KRoomUserManagerPtr spRoomUserManager, CXSLRoom::PVP_GAME_TYPE eGameType );

	//{{ 2012. 12. 17	������	�̺�Ʈ ���ʽ� �ڵ� �����丵	
	//void		SetGameEvent( int iDungeonID, int iUnitLevel, char cUnitClass, int iPartyNum = 0 ); -- �Ⱦ��� �ڵ�
	//}}

protected:
	float		m_fPartyBonus[NUM_TOTAL];
	//{{ 2012. 12. 17	������	�̺�Ʈ ���ʽ� �ڵ� �����丵	
	//float		m_fGameEvent[GEV_END]; -- �Ⱦ��� �ڵ�
	//}}
	//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	float		m_fDungeonModePartyBonus[CXSLDungeon::DT_NUM];
	float		m_fPartyDifficultyBonus[CXSLDungeon::DL_NUM];
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}
};

DefRefreshSingletonInline( KResultProcess );



