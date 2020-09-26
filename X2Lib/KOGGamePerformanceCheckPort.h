#pragma once

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK

class KOGGamePerformanceCheckPort_Function
{
public:
	KOGGamePerformanceCheckPort_Function();
	~KOGGamePerformanceCheckPort_Function();

	//��� ������, �� ���� ����
	static int GetGameSpace();

	static bool IsGameSpaceVillage( int iGameSpace );
	static bool IsGameSpaceField( int iGameSpace );
	static bool IsGameSpaceDungeon( int iGameSpace );
	static bool IsGameSpacePvP( int iGameSpace );

	static KOGGamePerformanceCheck::EPartyType GetVillagePartyType();
	static int GetVillagePartyUserNum();
	static int GetVillageMapID();
	static int GetVillageUserNum();

	static KOGGamePerformanceCheck::EPartyType GetFieldPartyType();
	static int GetFieldPartyUserNum();
	static int GetFieldMapID();
	static int GetFieldUserNum();

	static KOGGamePerformanceCheck::EPartyType GetDungeonPartyType();
	static int GetDungeonPartyUserNum();
	static int GetDungeonMapID();
	static int GetDungeonDifficulty();
	static int GetDungeonStage();
	static bool IsDungeonEscape();
	static int GetDungeonBlock();

	static int GetPvPMapID();
	static int GetPvPUserNum();
	static int GetPvPRuleType();
	static bool IsPvPEscape();
	static int GetPvPRound();
	//static int GetPvPObserverNum();

	static float GetFramePerSecond();
	static void ResetFramePerSecond();

	static bool IsGameHost();

	static void SendServer_Village( const KOGGamePerformanceCheck::KVillageInfo& kVillageInfo );
	static void SendServer_Field( const KOGGamePerformanceCheck::KFieldInfo& kFieldInfo );
	static void SendServer_Dungeon( const KOGGamePerformanceCheck::KDungeonInfo& kDungeonInfo );
	static void SendServer_PvP( const KOGGamePerformanceCheck::KPvPInfo& kPvPInfo );

	//��� ������ ��
	static bool IsValidGameSpace_AvgFps( int iGameSpace );

	//�� ���� ��
	static bool IsUnKnownLagType_Lag( int iLagType );
	static bool IsValidGameSpace_Lag( int iGameSpace );

	//ping ���� ��
	static bool IsValidGameSpace_Ping( int iGameSpace );

	//UDP �������� ��
	static bool IsValidGameSpace_UDPTrans( int iGameSpace );

	//��ȣ��Ʈ ��� NPC ���׼� ���� ��
	static bool IsValidGameSpace_NonHostNpcReaction( int iGameSpace );

	//memory usage ���� ��
	static bool IsValidGameSpace_MemUsage( int iGameSpace );
};

extern KOGGamePerformanceCheckPort_Function g_KOGGamePerformanceCheckPort_Function;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK