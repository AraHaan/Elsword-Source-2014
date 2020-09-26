#include ".\resultprocess.h"

#include "BaseServer.h"
#include "KAutoPath.h"
//{{ 2009. 7. 9  ������		��ϸ� �ð�
#include "HenirResultTable.h"
//}}
#include "Room/RoomUserFSM.h"
#include "GameEvent/GameEventManager.h"
#include "GameSysVal/GameSysVal.h"
#include "NetError.h"
//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
	#include "X2Data/XSLQuestManager.h"
#endif SERV_QUEST_CLEAR_EXPAND
//}}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
#include "KDropTable.h"
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
//{{ 2011. 07. 19	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatchResultTable.h"
#else
	#include "KPVPResultTable.h"
#endif SERV_PVP_NEW_SYSTEM
//}}
#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
#include "GameEvent/GameEventScriptManager.h"
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM

NiImplementRootRTTI( KResultProcess );
ImplementRefreshSingleton( KResultProcess );

ImplToStringW( KResultProcess )
{
	return START_TOSTRINGW;
}

KResultProcess::KResultProcess(void)
{
	for( int i = 0; i < NUM_TOTAL; ++i )
	{
		m_fPartyBonus[i] = 0.0f;
	}
}

KResultProcess::~KResultProcess(void)
{
}

ImplementLuaScriptParser( KResultProcess )
{
	lua_tinker::class_add<KResultProcess>( GetLuaState(), "KResultProcess" );
	lua_tinker::class_def<KResultProcess>( GetLuaState(), "AddDungeonPartyBonus",			&KResultProcess::AddDungeonPartyBonus_LUA );

	//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	lua_tinker::class_def<KResultProcess>( GetLuaState(), "AddDungeonPartyDifficultyBonus",	&KResultProcess::AddDungeonPartyDifficultyBonus_LUA );
	lua_tinker::class_def<KResultProcess>( GetLuaState(), "AddDungeonModePartyBonus",		&KResultProcess::AddDungeonModePartyBonus_LUA );
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

	lua_tinker::decl( GetLuaState(), "ResultProcess", this );

	lua_tinker::class_add<KRoomUser>( GetLuaState(), "KRoomUser" );
	lua_tinker::class_def<KRoomUser>( GetLuaState(), "GetComboScore",			&KRoomUser::GetComboScore_LUA );
	lua_tinker::class_def<KRoomUser>( GetLuaState(), "GetTechnicalScore",		&KRoomUser::GetTechnicalScore_LUA );
	lua_tinker::class_def<KRoomUser>( GetLuaState(), "GetUnitClass",			&KRoomUser::GetUnitClass_LUA );
	lua_tinker::class_def<KRoomUser>( GetLuaState(), "GetKillNPC",				&KRoomUser::GetKillNPC_LUA );
	lua_tinker::class_def<KRoomUser>( GetLuaState(), "GetDamageNum",			&KRoomUser::GetDamageNum_LUA );
	//{{ 2011. 01. 21	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
	lua_tinker::class_def<KRoomUser>( GetLuaState(), "GetBaseHP",				&KRoomUser::GetBaseHP_LUA );
	lua_tinker::class_def<KRoomUser>( GetLuaState(), "GetTotalGivenDamamge",	&KRoomUser::GetTotalGivenDamamge_LUA );
	lua_tinker::class_def<KRoomUser>( GetLuaState(), "GetTotalAttackedDamage",	&KRoomUser::GetTotalAttackedDamage_LUA );
	lua_tinker::class_def<KRoomUser>( GetLuaState(), "GetTotalSumNpcHP",		&KRoomUser::GetTotalSumNpcHP_LUA );
	lua_tinker::class_def<KRoomUser>( GetLuaState(), "GetTotalRecoveryHP",		&KRoomUser::GetTotalRecoveryHP_LUA );
	lua_tinker::class_def<KRoomUser>( GetLuaState(), "GetUsingKindOfSkill",		&KRoomUser::GetUsingKindOfSkill_LUA );
	lua_tinker::class_def<KRoomUser>( GetLuaState(), "GetCommandTechScore",		&KRoomUser::GetCommandTechScore_LUA );
	lua_tinker::class_def<KRoomUser>( GetLuaState(), "GetSkillTechScore",		&KRoomUser::GetSkillTechScore_LUA );
#endif SERV_DUNGEON_RANK_NEW
	//}}
	lua_tinker::class_def<KRoomUser>( GetLuaState(), "SetResultData",			&KRoomUser::SetResultData_LUA );

	//{{ 2011. 10. 26	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	lua_tinker::class_add<KDungeonMonsterManager>( GetLuaState(), "KRoomMonsterManager" );
	lua_tinker::class_def<KDungeonMonsterManager>( GetLuaState(), "GetNormalNpcDieCount",		&KDungeonMonsterManager::GetNormalNpcDieCount_LUA );
	lua_tinker::class_def<KDungeonMonsterManager>( GetLuaState(), "GetLowEliteNpcDieCount",		&KDungeonMonsterManager::GetLowEliteNpcDieCount_LUA );
	lua_tinker::class_def<KDungeonMonsterManager>( GetLuaState(), "GetHighEliteNpcDieCount",	&KDungeonMonsterManager::GetHighEliteNpcDieCount_LUA );
	lua_tinker::class_def<KDungeonMonsterManager>( GetLuaState(), "GetMiddleBossDieCount",		&KDungeonMonsterManager::GetMiddleBossDieCount_LUA );
	lua_tinker::class_def<KDungeonMonsterManager>( GetLuaState(), "GetBossDieCount",			&KDungeonMonsterManager::GetBossDieCount_LUA );
	//////////////////////////////////////////////////////////////////////////
	lua_tinker::class_def<KDungeonMonsterManager>( GetLuaState(), "PrintLuaLog",				&KDungeonMonsterManager::PrintLuaLog_LUA );
	//////////////////////////////////////////////////////////////////////////	
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
}

//{{ 2011. 07. 11	������	���� ����
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////
bool KResultProcess::Result_TEAM(IN const float fTotalPlayTime,
								 IN const float fRemainingPlayTime,
								 IN const int iPVPChannelClass,
								 IN const bool bOfficialPvpCancel, 
								 IN const SEnum::MATCH_TYPE eMatchType,
								 IN const SEnum::PVP_NPC_TYPE ePvpNpcType,
								 IN OUT KRoomUserManagerPtr spRoomUserManager,
								 OUT KEGS_END_GAME_PVP_RESULT_DATA_NOT& kPacketNot,
								 OUT KPvpMatchUserResultLog::END_TYPE& eEndType )
{
	kPacketNot.m_cWinTeam = static_cast<char>( DecidePVPWinTeam( fTotalPlayTime, fRemainingPlayTime, spRoomUserManager, CXSLRoom::PGT_TEAM, eEndType ) );
	kPacketNot.m_bIsAbnormalEndPlay = bOfficialPvpCancel;

	if( kPacketNot.m_cWinTeam == CXSLRoom::TN_NONE  ||  bOfficialPvpCancel == true )
		kPacketNot.m_bIsDrawn = true;
	else
		kPacketNot.m_bIsDrawn = false;

	//////////////////////////////////////////////////////////////////////////
	// ���� �����ð��� �̸� ���� ����!
	const int RED_TEAM_RATING = spRoomUserManager->GetRedTeamRating();
	const int BLUE_TEAM_RATING = spRoomUserManager->GetBlueTeamRating();
	//////////////////////////////////////////////////////////////////////////	

	//�¸����� ������ �¸�����, �ƴϸ� �й躸��
	for( int iIndex = 0; iIndex < spRoomUserManager->GetNumMember(); ++iIndex )
	{
		KRoomUserPtr pkRoomUser = spRoomUserManager->GetUser( iIndex );
		if( pkRoomUser == NULL )
			continue;

		//���â���� �������� ���� ������ ������� ����.
		if( pkRoomUser->GetStateID() != KRoomUserFSM::S_RESULT )
			continue;
		
		//{{ 2010. 05. 11  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
#else
		SetGameEvent( 0, pkRoomUser->GetLevel(), pkRoomUser->GetUnitClass_LUA() );
#endif SERV_INTEGRATION
		//}}

		KPVPUnitResultInfo	kUnitInfo;
		kUnitInfo.m_UnitUID		= pkRoomUser->GetCID();
		kUnitInfo.m_nKillNum	= pkRoomUser->GetNumKill();
		kUnitInfo.m_nMDKillNum	= pkRoomUser->GetNumMDKill();
		kUnitInfo.m_nDeathNum	= pkRoomUser->GetNumDie();
		kUnitInfo.m_bWin	= false;
		kUnitInfo.m_bIsMVP	= false;

		//drawn �̸� �⺻���ø� �ְ� ������.
		if( kPacketNot.m_bIsDrawn == true )
		{
			kPacketNot.m_vecPVPUnitInfo.push_back( kUnitInfo );
			continue;
		}

		// �¸������� üũ����!
		if( pkRoomUser->GetTeam() == kPacketNot.m_cWinTeam )
		{
			kUnitInfo.m_bWin = true;
		}

		//////////////////////////////////////////////////////////////////////////
		//{{ 2011. 07. 11	������	���� ����
		int iMyTeamRating = 0;
		int iEnemyTeamRating = 0;
		if( pkRoomUser->GetTeam() == CXSLRoom::TN_RED )
		{
			iMyTeamRating = RED_TEAM_RATING;
			iEnemyTeamRating = BLUE_TEAM_RATING;
		}
		else
		{
			iMyTeamRating = BLUE_TEAM_RATING;
			iEnemyTeamRating = RED_TEAM_RATING;
		}

		//{{ 2012. 06. 25	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		// ���� �̰�ų�, ���� ��ġ��Ŀ�̰ų�, ��밡 ��ġ��Ŀ�� �ƴϿ���
		// �Ϲ� ������ ��ġ ��Ŀ���� ��� ���� ���� ������ KFactor �� Rating ���Ұ� ������ �ϴ� ���� ����
		if( ( kUnitInfo.m_bWin == true ) || ( pkRoomUser->IsPlacementPvpUser() == true ) || ( spRoomUserManager->EnemyIsPlacementTeam( pkRoomUser->GetTeam() ) == false ) )
		{
			kUnitInfo.m_fKFactorIncrement = SiKPvpMatchResultTable()->GetKFactor( pkRoomUser->GetOfficialMatchCount(),
																				pkRoomUser->IsWinBeforeMatch(),
																				kUnitInfo.m_bWin,
																				pkRoomUser->GetRating(),
																				pkRoomUser->GetKFactor(),
																				pkRoomUser->IsRedistributionUser() );
			// 1. Rating�� ����!
			kUnitInfo.m_iRating = SiKPvpMatchResultTable()->GetRatingPvpResult( iMyTeamRating,
																				iEnemyTeamRating,
																				pkRoomUser->GetRating(),
																				kUnitInfo.m_bWin,
																				kUnitInfo.m_fKFactorIncrement + pkRoomUser->GetKFactor(),
																				spRoomUserManager->EnemyIsPlacementTeam( CXSLRoom::TN_RED ) == spRoomUserManager->EnemyIsPlacementTeam( CXSLRoom::TN_BLUE )
																				);
		}
#else
		// 1. Rating�� ����!
		kUnitInfo.m_iRating = SiKPvpMatchResultTable()->GetRatingPvpResult( iMyTeamRating,
																			iEnemyTeamRating,
																			pkRoomUser->GetRating(),
																			pkRoomUser->GetMaxRating(),
																			pkRoomUser->GetOfficialMatchCount(), 
																			kUnitInfo.m_bWin,
																			pkRoomUser->IsWinBeforeMatch(),
																			eMatchType,
																			ePvpNpcType );
#endif SERV_2012_PVP_SEASON2
		//}}

		// 2. RPoint�� ����!
#ifdef	SERV_RANKING_POINT_EVENT // ���볯¥: 2013-03-26
		const int iRankingPointReward = SiKPvpMatchResultTable()->GetRPointPvpResult( iMyTeamRating,
																					  iEnemyTeamRating,
																					  kUnitInfo.m_bWin,
																					  kUnitInfo.m_nKillNum,
																					  kUnitInfo.m_nMDKillNum,
																					  kUnitInfo.m_nDeathNum,
																					  eMatchType,
																					  ePvpNpcType
																					  );
		// �⺻ RankingPoint ����!
		kUnitInfo.m_iRPoint	= iRankingPointReward;
#else	// SERV_RANKING_POINT_EVENT
		kUnitInfo.m_iRPoint = SiKPvpMatchResultTable()->GetRPointPvpResult( iMyTeamRating,
																			iEnemyTeamRating,
																			kUnitInfo.m_bWin,
																			kUnitInfo.m_nKillNum,
																			kUnitInfo.m_nMDKillNum,
																			kUnitInfo.m_nDeathNum,
																			eMatchType,
																			ePvpNpcType );
#endif	// SERV_RANKING_POINT_EVENT

		// 3. APoint�� ����!
		const int iAPointReward = SiKPvpMatchResultTable()->GetAPointPvpResult( pkRoomUser->GetRating(), 
																			kUnitInfo.m_bWin, 
																			kUnitInfo.m_nKillNum, 
																			kUnitInfo.m_nMDKillNum,
																			eMatchType,
																			ePvpNpcType );

		// �⺻APoint ����!
		kUnitInfo.m_iAPoint = iAPointReward;

		// 4. ����ġ�� ����!
		const int iExpReward = SiKPvpMatchResultTable()->GetExpPvpResult( pkRoomUser->GetLevel(),
																		kUnitInfo.m_bWin,
																		kUnitInfo.m_nKillNum,
																		kUnitInfo.m_nMDKillNum,
																		eMatchType,
																		ePvpNpcType );

		// �⺻ ����ġ ����!
		kUnitInfo.m_nEXP = iExpReward;

		// PC�� ���ʽ�!
		if( pkRoomUser->IsGameBang() == true )
		{
#ifdef SERV_PC_BANG_TYPE
			kUnitInfo.m_iAPoint		+= (int)(iAPointReward * SiKGameSysVal()->GetGBVPRate( pkRoomUser->GetPcBangType() ));
			kUnitInfo.m_nEXP		+= (int)(iExpReward * SiKGameSysVal()->GetGBEXPRate( pkRoomUser->GetPcBangType() ));
#else SERV_PC_BANG_TYPE
			kUnitInfo.m_iAPoint		+= (int)(iAPointReward * SiKGameSysVal()->GetGBVPRate());
			kUnitInfo.m_nEXP		+= (int)(iExpReward * SiKGameSysVal()->GetGBEXPRate());
#endif SERV_PC_BANG_TYPE
		}

		//�̺�Ʈ ���� ����.!
		{
			//{{ 2010. 05. 11  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
			kUnitInfo.m_iAPoint		+= (int)(iAPointReward * pkRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_VP ) );
			kUnitInfo.m_nEXP		+= (int)(iExpReward * pkRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_EXP ) );
#ifdef	SERV_RANKING_POINT_EVENT // ���볯¥: 2013-03-26
			if( 0 < kUnitInfo.m_iRPoint )
			{
				kUnitInfo.m_iRPoint		+= (int)( iRankingPointReward * pkRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_RANKING_POINT ) );
			}
#endif	// SERV_RANKING_POINT_EVENT
#else
			//kUnitInfo.m_iAPoint		+= (int)(iAPointReward * m_fGameEvent[GEV_VP]);
			//kUnitInfo.m_nEXP		+= (int)(iExpReward  * m_fGameEvent[GEV_EXP]);
#endif SERV_INTEGRATION
			//}}
		}

		//080421.hoons.����ġ ���� ĳ���������� ������ ����ó��..
		//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
		//#ifdef SERV_ROOMUSERINFO_REFAC
		const float fAddPremiumExpRate = pkRoomUser->GetBonusRate( KRoomUserInfo::BT_PREMIUM_EXP_RATE );
		//#endif SERV_ROOMUSERINFO_REFAC
		//}}
		if( fAddPremiumExpRate > 0.0f )
		{
			kUnitInfo.m_nEXP += (int)(iExpReward * fAddPremiumExpRate);
		}

		//limit level
		if( pkRoomUser->GetLevel() >= SiKGameSysVal()->GetLimitsLevel() )
		{
			kUnitInfo.m_nEXP = 0;
		}

		//float fPlayTime = 0.0f; ---- ������ �ʹ� ���� ���ΰ��! �������� ������ �����?
		//if( spRoomUserManager->GetPlayTime( pkRoomUser->GetCID(), fPlayTime ) == true )
		//{
		//	if( fPlayTime < SEnum::ET_PVP_DRAWN_TIME )
		//	{
		//		kUnitInfo.m_iRating	= 0;
		//		kUnitInfo.m_iRPoint = 0;
		//		kUnitInfo.m_iAPoint = 0;
		//		kUnitInfo.m_nEXP	= 0;
		//	}
		//}

		// ��� ���� ���� ������ ������� ��������!
		kPacketNot.m_vecPVPUnitInfo.push_back( kUnitInfo );
	}

	if( kPacketNot.m_bIsDrawn == true )
		return true;

	//////////////////////////////////////////////////////////////////////////
	//���ӳ� ���ּ��� ����

	//�ƽ��������� ��������
	SortbyPVPMD( kPacketNot.m_vecPVPUnitInfo );

	//ų���� ��������
	SortbyPVPKill( kPacketNot.m_vecPVPUnitInfo );

	//�ƽ������� & ų���� ���� ��� HP�� ��������
	SortbyPVPHP( kPacketNot.m_vecPVPUnitInfo, spRoomUserManager, CXSLRoom::PGT_TEAM );

	// the same rank and level
	SortbyPVPLevel( kPacketNot.m_vecPVPUnitInfo, spRoomUserManager, CXSLRoom::PGT_TEAM );

	// and slotid..?
	SortbyPVPSlot( kPacketNot.m_vecPVPUnitInfo, spRoomUserManager, CXSLRoom::PGT_TEAM );

	bool bIsSelectionMVP = false;
	for( u_int ui = 0; ui < kPacketNot.m_vecPVPUnitInfo.size(); ++ui )
	{
		KPVPUnitResultInfo* pkUnitInfo = &kPacketNot.m_vecPVPUnitInfo[ui];
		pkUnitInfo->m_nRanking = ui+1;

		//Ǯ���϶� MVP�� �����Ѵ�.
		if( spRoomUserManager->GetNumTotalSlot() == spRoomUserManager->GetNumResultPlayer() )
		{
			//��ŷ�߿��� ���� ������ŷ�̰� �¸����̸� MVP
			if( bIsSelectionMVP == false && pkUnitInfo->m_bWin == true )
			{
				pkUnitInfo->m_bIsMVP = true;
				bIsSelectionMVP = true;
			}
		}
	}

	return true;
}

bool KResultProcess::Result_SURVIVAL( IN const float fTotalPlayTime,
									  IN const float fRemainingPlayTime, 
									  IN const int iPVPChannelClass, 
									  IN const bool bOfficialPvpCancel, 
									  IN const SEnum::MATCH_TYPE eMatchType,
									  IN const SEnum::PVP_NPC_TYPE ePvpNpcType,
									  IN OUT KRoomUserManagerPtr spRoomUserManager, 
									  OUT KEGS_END_GAME_PVP_RESULT_DATA_NOT& kPacketNot,
									  OUT KPvpMatchUserResultLog::END_TYPE& eEndType )
{
	KRoomUserPtr		pkRoomUser;

	kPacketNot.m_bIsDrawn = false;
	kPacketNot.m_cWinTeam = CXSLRoom::TN_NONE;

	//���º� ó��
	if( spRoomUserManager->GetNumResultPlayer() <= 1 )
	{
		if( (fTotalPlayTime - fRemainingPlayTime) < SEnum::ET_PVP_DRAWN_TIME )
		{
			kPacketNot.m_bIsDrawn = true;
		}
	}

	//����ó���� ���������� �̾Ƴ���.
	for( int iIndex = 0; iIndex < spRoomUserManager->GetNumMember(); ++iIndex )
	{
		pkRoomUser = spRoomUserManager->GetUser( iIndex );

		//���â���� �������� ���� ������ ������� ����.
		if( pkRoomUser != NULL && pkRoomUser->GetStateID() == KRoomUserFSM::S_RESULT )
		{			
			KPVPUnitResultInfo	kUnitInfo;

			kUnitInfo.m_UnitUID		= pkRoomUser->GetCID();
			kUnitInfo.m_nKillNum	= pkRoomUser->GetNumKill();
			kUnitInfo.m_nMDKillNum	= pkRoomUser->GetNumMDKill();
			kUnitInfo.m_nDeathNum	= pkRoomUser->GetNumDie();

			if( kPacketNot.m_bIsDrawn == true )
			{
				kPacketNot.m_vecPVPUnitInfo.push_back( kUnitInfo );
				continue;
			}

			// [����] �����̹� ���� ���� ���̻� ������ �����ϴ�!
			kUnitInfo.m_iRating = 0;
			kUnitInfo.m_iRPoint = 0;
			kUnitInfo.m_iAPoint = 0;
			kUnitInfo.m_nEXP	= 0;

			//float fPlayTime = 0;
			//if( spRoomUserManager->GetPlayTime( pkRoomUser->GetCID(), fPlayTime ) == true )
			//{
			//	if( fPlayTime < SEnum::ET_PVP_DRAWN_TIME )
			//	{
			//		//kUnitInfo.m_nVSPoint	= 0;
			//		kUnitInfo.m_nEXP		= 0;
			//	}
			//}

			kPacketNot.m_vecPVPUnitInfo.push_back( kUnitInfo );
		}
	}

	if( kPacketNot.m_bIsDrawn == true )
		return true;

	//////////////////////////////////////////////////////////////////////////
	//���ӳ� ���ּ��� ����

	//�ƽ��������� ��������
	SortbyPVPMD( kPacketNot.m_vecPVPUnitInfo );

	//ų���� ��������
	SortbyPVPKill( kPacketNot.m_vecPVPUnitInfo );

	//�ƽ������� & ų���� ���� ��� HP�� ��������
	SortbyPVPHP( kPacketNot.m_vecPVPUnitInfo, spRoomUserManager, CXSLRoom::PGT_SURVIVAL );

	// the same rank and level
	SortbyPVPLevel( kPacketNot.m_vecPVPUnitInfo, spRoomUserManager, CXSLRoom::PGT_SURVIVAL );

	// and slotid..?
	SortbyPVPSlot( kPacketNot.m_vecPVPUnitInfo, spRoomUserManager, CXSLRoom::PGT_SURVIVAL );

	//mvp &  rank & is win
	//const KPvpMatchResultTable::SResultData* pRDwin		= SiKPvpMatchResultTable()->GetTeamMatchWin( iPVPChannelClass );
	//const KPvpMatchResultTable::SResultData* pRDLose	= SiKPvpMatchResultTable()->GetTeamMatchLose( iPVPChannelClass );

	//if( pRDwin == NULL || pRDLose == NULL )
	//{
	//	START_LOG( cerr, L"SURVIVAL.. Team ��������� ���� result data ������ �̻�.!" )
	//		<< BUILD_LOG( iPVPChannelClass )
	//		<< END_LOG;
	//}

	for( int i = 0; i < (int)kPacketNot.m_vecPVPUnitInfo.size(); ++i )
	{
		KPVPUnitResultInfo* pkUnitInfo = &kPacketNot.m_vecPVPUnitInfo[i];
		pkUnitInfo->m_nRanking = i+1;
		if( i == 0 )
		{
			//Ǯ���϶� MVP�� �����Ѵ�.
			if( spRoomUserManager->GetNumTotalSlot() == spRoomUserManager->GetNumResultPlayer() )
				pkUnitInfo->m_bIsMVP = true;
		}
		else
		{
			pkUnitInfo->m_bIsMVP = false;
		}

		int tempMiddleValue = (int)(kPacketNot.m_vecPVPUnitInfo.size() / 2);
		if ( tempMiddleValue <= 0 )
			tempMiddleValue = 1;
		if( pkUnitInfo->m_nRanking <= tempMiddleValue )
		{
			pkUnitInfo->m_bWin = true;

			//if( pRDwin != NULL )
			//{
			//	pkUnitInfo->m_nVSPoint	+= pRDwin->m_VP;
			//	pkUnitInfo->m_nEXP		+= pRDwin->m_EXP;
			//}
		}
		else
		{
			pkUnitInfo->m_bWin = false;

			//if( pRDLose != NULL )
			//{
			//	pkUnitInfo->m_nVSPoint	+= pRDLose->m_VP;
			//	pkUnitInfo->m_nEXP		+= pRDLose->m_EXP;
			//}
		}

		//int iVSPoint = pkUnitInfo->m_nVSPoint;
		//int iEXP = pkUnitInfo->m_nEXP;
		//if( spRoomUserManager->IsGameBang( pkUnitInfo->m_UnitUID ) == true )
		//{
		//	//pkUnitInfo->m_nVSPoint	+= (int)(iVSPoint * SiKGameSysVal()->GetGBVPRate());
		//	pkUnitInfo->m_nEXP		+= (int)(iEXP * SiKGameSysVal()->GetGBEXPRate());
		//}

		pkRoomUser = spRoomUserManager->GetUser( pkUnitInfo->m_UnitUID );
		int iUnitLevel = 0;
		if( pkRoomUser != NULL )
			iUnitLevel = pkRoomUser->GetLevel();

		//�̺�Ʈ ���� ����.!
//		{
//			//{{ 2010. 05. 11  ������	���� ���� ������ ����
//#ifdef SERV_INTEGRATION
//			//pkUnitInfo->m_nVSPoint	+= (int)(iVSPoint * pkRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_VP ) );
//			pkUnitInfo->m_nEXP		+= (int)(iEXP * pkRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_EXP ) );
//#else
//			SetGameEvent( 0, pkRoomUser->GetLevel(), pkRoomUser->GetUnitClass_LUA() );
//
//			//pkUnitInfo->m_nVSPoint	+= (int)(iVSPoint * m_fGameEvent[GEV_VP]);
//			pkUnitInfo->m_nEXP		+= (int)(iEXP * m_fGameEvent[GEV_EXP]);
//#endif SERV_INTEGRATION
//			//}}			
//		}

		//080421.hoons.����ġ ���� ĳ���������� ������ ����ó��..
		//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
		//#ifdef SERV_ROOMUSERINFO_REFAC
		//const float fAddPremiumExpRate = pkRoomUser->GetBonusRate( KRoomUserInfo::BT_PREMIUM_EXP_RATE );
		//#endif SERV_ROOMUSERINFO_REFAC
		//}}
		//if( fAddPremiumExpRate > 0.0f )
		//{
		//	pkUnitInfo->m_nEXP += (int)(iEXP * fAddPremiumExpRate);
		//}

		//limit level
		if( iUnitLevel >= SiKGameSysVal()->GetLimitsLevel() )
		{
			pkUnitInfo->m_nEXP = 0;
		}
	}

	return true;
}

bool KResultProcess::Result_TEAM_DEATH(	IN const float fTotalPlayTime, 
										IN const float fRemainingPlayTime, 
										IN const int iPVPChannelClass, 
										IN const bool bOfficialPvpCancel, 
										IN const SEnum::MATCH_TYPE eMatchType,
										IN const SEnum::PVP_NPC_TYPE ePvpNpcType,
										IN OUT KRoomUserManagerPtr spRoomUserManager, 
										OUT KEGS_END_GAME_PVP_RESULT_DATA_NOT& kPacketNot,
										OUT KPvpMatchUserResultLog::END_TYPE& eEndType )
{
	kPacketNot.m_cWinTeam = static_cast<char>( DecidePVPWinTeam( fTotalPlayTime, fRemainingPlayTime, spRoomUserManager, CXSLRoom::PGT_TEAM_DEATH, eEndType ) );
	kPacketNot.m_bIsAbnormalEndPlay = bOfficialPvpCancel;

	if( kPacketNot.m_cWinTeam == CXSLRoom::TN_NONE  ||  bOfficialPvpCancel == true )
		kPacketNot.m_bIsDrawn = true;
	else
		kPacketNot.m_bIsDrawn = false;

	//////////////////////////////////////////////////////////////////////////
	// ���� �����ð��� �̸� ���� ����!
	const int RED_TEAM_RATING = spRoomUserManager->GetRedTeamRating();
	const int BLUE_TEAM_RATING = spRoomUserManager->GetBlueTeamRating();
	//////////////////////////////////////////////////////////////////////////	

	//�¸����� ������ �¸�����, �ƴϸ� �й躸��
	for( int iIndex = 0; iIndex < spRoomUserManager->GetNumMember(); ++iIndex )
	{
		KRoomUserPtr pkRoomUser = spRoomUserManager->GetUser( iIndex );
		if( pkRoomUser == NULL )
			continue;

		//���â���� �������� ���� ������ ������� ����.
		if( pkRoomUser->GetStateID() != KRoomUserFSM::S_RESULT )
			continue;
		
		//{{ 2010. 05. 11  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
#else
		SetGameEvent( 0, pkRoomUser->GetLevel(), pkRoomUser->GetUnitClass_LUA() );
#endif SERV_INTEGRATION
		//}}

		KPVPUnitResultInfo	kUnitInfo;
		kUnitInfo.m_UnitUID		= pkRoomUser->GetCID();
		kUnitInfo.m_nKillNum	= pkRoomUser->GetNumKill();
		kUnitInfo.m_nMDKillNum	= pkRoomUser->GetNumMDKill();
		kUnitInfo.m_nDeathNum	= pkRoomUser->GetNumDie();
		kUnitInfo.m_bWin	= false;
		kUnitInfo.m_bIsMVP	= false;

		if( kPacketNot.m_bIsDrawn == true )
		{
			kPacketNot.m_vecPVPUnitInfo.push_back( kUnitInfo );
			continue;
		}

		// �¸������� üũ����!
		if( pkRoomUser->GetTeam() == kPacketNot.m_cWinTeam )
		{
			kUnitInfo.m_bWin = true;
		}

		//////////////////////////////////////////////////////////////////////////
		//{{ 2011. 07. 11	������	���� ����
		int iMyTeamRating = 0;
		int iEnemyTeamRating = 0;
		if( pkRoomUser->GetTeam() == CXSLRoom::TN_RED )
		{
			iMyTeamRating = RED_TEAM_RATING;
			iEnemyTeamRating = BLUE_TEAM_RATING;
		}
		else
		{
			iMyTeamRating = BLUE_TEAM_RATING;
			iEnemyTeamRating = RED_TEAM_RATING;
		}

		//{{ 2012. 06. 21	�ڼ���	2012 ���� ����2
#ifdef SERV_2012_PVP_SEASON2
		// ���� �̰�ų�, ���� ��ġ��Ŀ�̰ų�, ��밡 ��ġ��Ŀ�� �ƴϿ���
		// �Ϲ� ������ ��ġ ��Ŀ���� ��� ���� ���� ������ KFactor �� Rating ���Ұ� ������ �ϴ� ���� ����
		if( ( kUnitInfo.m_bWin == true ) || ( pkRoomUser->IsPlacementPvpUser() == true ) || ( spRoomUserManager->EnemyIsPlacementTeam( pkRoomUser->GetTeam() ) == false ) )
		{
			kUnitInfo.m_fKFactorIncrement = SiKPvpMatchResultTable()->GetKFactor( pkRoomUser->GetOfficialMatchCount(),
																				pkRoomUser->IsWinBeforeMatch(),
																				kUnitInfo.m_bWin,
																				pkRoomUser->GetRating(),
																				pkRoomUser->GetKFactor(),
																				pkRoomUser->IsRedistributionUser() );

			// 1. Rating�� ����!
			kUnitInfo.m_iRating = SiKPvpMatchResultTable()->GetRatingPvpResult( iMyTeamRating,
																				iEnemyTeamRating,
																				pkRoomUser->GetRating(),
																				kUnitInfo.m_bWin,
																				kUnitInfo.m_fKFactorIncrement + pkRoomUser->GetKFactor(),
																				spRoomUserManager->EnemyIsPlacementTeam( CXSLRoom::TN_RED ) == spRoomUserManager->EnemyIsPlacementTeam( CXSLRoom::TN_BLUE )
																				);
		}
#else
		// 1. Rating�� ����!
		kUnitInfo.m_iRating = SiKPvpMatchResultTable()->GetRatingPvpResult( iMyTeamRating,
																			iEnemyTeamRating,
																			pkRoomUser->GetRating(),
																			pkRoomUser->GetMaxRating(),
																			pkRoomUser->GetOfficialMatchCount(), 
																			kUnitInfo.m_bWin,
																			pkRoomUser->IsWinBeforeMatch(),
																			eMatchType,
																			ePvpNpcType );
#endif SERV_2012_PVP_SEASON2
		//}}

		// 2. RPoint�� ����!
#ifdef	SERV_RANKING_POINT_EVENT // ���볯¥: 2013-03-26
		const int iRankingPointReward = SiKPvpMatchResultTable()->GetRPointPvpResult( iMyTeamRating,
																					  iEnemyTeamRating,
																					  kUnitInfo.m_bWin,
																					  kUnitInfo.m_nKillNum,
																					  kUnitInfo.m_nMDKillNum,
																					  kUnitInfo.m_nDeathNum,
																					  eMatchType,
																					  ePvpNpcType
																					  );
		kUnitInfo.m_iRPoint	= iRankingPointReward;
#else	// SERV_RANKING_POINT_EVENT
		kUnitInfo.m_iRPoint = SiKPvpMatchResultTable()->GetRPointPvpResult( iMyTeamRating,
																			iEnemyTeamRating,
																			kUnitInfo.m_bWin,
																			kUnitInfo.m_nKillNum,
																			kUnitInfo.m_nMDKillNum,
																			kUnitInfo.m_nDeathNum,
																			eMatchType,
																			ePvpNpcType );
#endif	// SERV_RANKING_POINT_EVENT

		// 3. APoint�� ����!
		const int iAPointReward = SiKPvpMatchResultTable()->GetAPointPvpResult( pkRoomUser->GetRating(), 
																			kUnitInfo.m_bWin, 
																			kUnitInfo.m_nKillNum, 
																			kUnitInfo.m_nMDKillNum,
																			eMatchType,
																			ePvpNpcType );

		// �⺻APoint ����!
		kUnitInfo.m_iAPoint = iAPointReward;

		// 4. ����ġ�� ����!
		const int iExpReward = SiKPvpMatchResultTable()->GetExpPvpResult( pkRoomUser->GetLevel(),
																			kUnitInfo.m_bWin,
																			kUnitInfo.m_nKillNum,
																			kUnitInfo.m_nMDKillNum,
																			eMatchType,
																			ePvpNpcType );

		// �⺻ ����ġ ����!
		kUnitInfo.m_nEXP = iExpReward;

		// PC�� ���ʽ�!
		if( pkRoomUser->IsGameBang() == true )
		{
#ifdef SERV_PC_BANG_TYPE
			kUnitInfo.m_iAPoint		+= (int)(iAPointReward * SiKGameSysVal()->GetGBVPRate( pkRoomUser->GetPcBangType() ));
			kUnitInfo.m_nEXP		+= (int)(iExpReward * SiKGameSysVal()->GetGBEXPRate( pkRoomUser->GetPcBangType() ));
#else SERV_PC_BANG_TYPE
			kUnitInfo.m_iAPoint		+= (int)(iAPointReward * SiKGameSysVal()->GetGBVPRate());
			kUnitInfo.m_nEXP		+= (int)(iExpReward * SiKGameSysVal()->GetGBEXPRate());
#endif SERV_PC_BANG_TYPE
		}

		//�̺�Ʈ ���� ����.!
		{
			//{{ 2010. 05. 11  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
			kUnitInfo.m_iAPoint		+= (int)(iAPointReward * pkRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_VP ) );
			kUnitInfo.m_nEXP		+= (int)(iExpReward * pkRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_EXP ) );
#ifdef	SERV_RANKING_POINT_EVENT // ���볯¥: 2013-03-26
			if( 0 < kUnitInfo.m_iRPoint )
			{
				kUnitInfo.m_iRPoint		+= (int)( iRankingPointReward * pkRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_RANKING_POINT ) );
			}
#endif	// SERV_RANKING_POINT_EVENT
#else
			//kUnitInfo.m_iAPoint		+= (int)(iAPointReward * m_fGameEvent[GEV_VP]);
			//kUnitInfo.m_nEXP		+= (int)(iExpReward * m_fGameEvent[GEV_EXP]);
#endif SERV_INTEGRATION
			//}}
		}

		//080421.hoons.����ġ ���� ĳ���������� ������ ����ó��..
		//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
		//#ifdef SERV_ROOMUSERINFO_REFAC
		const float fAddPremiumExpRate = pkRoomUser->GetBonusRate( KRoomUserInfo::BT_PREMIUM_EXP_RATE );
		//#endif SERV_ROOMUSERINFO_REFAC
		//}}
		if( fAddPremiumExpRate > 0.0f )
		{
			kUnitInfo.m_nEXP += (int)(iExpReward * fAddPremiumExpRate);
		}

		//limit level
		if( pkRoomUser->GetLevel() >= SiKGameSysVal()->GetLimitsLevel() )
		{
			kUnitInfo.m_nEXP = 0;
		}

		//float fPlayTime = 0.0f; -- ������ �ʹ� ���� ���ΰ��! �������� ������ �����?
		//if( spRoomUserManager->GetPlayTime( pkRoomUser->GetCID(), fPlayTime ) == true )
		//{
		//	if( fPlayTime < SEnum::ET_PVP_DRAWN_TIME )
		//	{
		//		kUnitInfo.m_iRating	= 0;
		//		kUnitInfo.m_iRPoint = 0;
		//		kUnitInfo.m_iAPoint = 0;
		//		kUnitInfo.m_nEXP	= 0;
		//	}
		//}	

		// ��� ���� ���� ������ ������� ��������!
		kPacketNot.m_vecPVPUnitInfo.push_back( kUnitInfo );
	}

	if( kPacketNot.m_bIsDrawn == true )
		return true;

	//�ƽ��������� ��������
	SortbyPVPMD( kPacketNot.m_vecPVPUnitInfo );

	//ų���� ��������
	SortbyPVPKill( kPacketNot.m_vecPVPUnitInfo );

	//�ƽ������� & ų���� ���� ��� HP�� ��������
	SortbyPVPHP( kPacketNot.m_vecPVPUnitInfo, spRoomUserManager, CXSLRoom::PGT_TEAM_DEATH );

	// the same rank and level
	SortbyPVPLevel( kPacketNot.m_vecPVPUnitInfo, spRoomUserManager, CXSLRoom::PGT_TEAM_DEATH );

	// and slotid..?
	SortbyPVPSlot( kPacketNot.m_vecPVPUnitInfo, spRoomUserManager, CXSLRoom::PGT_TEAM_DEATH );

	bool bIsSelectionMVP = false;
	for( int i = 0; i < (int)kPacketNot.m_vecPVPUnitInfo.size(); ++i )
	{
		KPVPUnitResultInfo* pkUnitInfo = &kPacketNot.m_vecPVPUnitInfo[i];
		pkUnitInfo->m_nRanking = i+1;

		//Ǯ���϶� MVP�� �����Ѵ�.
		if( spRoomUserManager->GetNumTotalSlot() == spRoomUserManager->GetNumResultPlayer() )
		{
			//�¸����� ������ŷ�� �̾� MVP
			if( bIsSelectionMVP == false && pkUnitInfo->m_bWin == true )
			{
				pkUnitInfo->m_bIsMVP = true;
				bIsSelectionMVP = true;
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////
bool KResultProcess::Result_TEAM(	IN const float fTotalPlayTime,
									IN const float fRemainingPlayTime,
									IN const int iPVPChannelClass,
									IN OUT KRoomUserManagerPtr spRoomUserManager,
									OUT KEGS_END_GAME_PVP_RESULT_DATA_NOT& kPacketNot )
{
	KRoomUserPtr pkRoomUser;

	kPacketNot.m_cWinTeam = static_cast<char>( DecidePVPWinTeam( fTotalPlayTime, fRemainingPlayTime, spRoomUserManager, CXSLRoom::PGT_TEAM ) );

	if( kPacketNot.m_cWinTeam == CXSLRoom::TN_NONE )
		kPacketNot.m_bIsDrawn = true;
	else
		kPacketNot.m_bIsDrawn = false;

	//�¸����� ������ �¸�����, �ƴϸ� �й躸��
	for( int i = 0; i < spRoomUserManager->GetNumMember(); ++i )
	{
		pkRoomUser = spRoomUserManager->GetUser( i );

		//���â���� �������� ���� ������ ������� ����.
        if( pkRoomUser != NULL && pkRoomUser->GetStateID() == KRoomUserFSM::S_RESULT )
		{
			//{{ 2010. 05. 11  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
#else
			SetGameEvent( 0, pkRoomUser->GetLevel(), pkRoomUser->GetUnitClass_LUA() );
#endif SERV_INTEGRATION
			//}}

			KPVPUnitResultInfo	kUnitInfo;

			kUnitInfo.m_UnitUID		= pkRoomUser->GetCID();
			kUnitInfo.m_nKillNum	= pkRoomUser->GetNumKill();
			kUnitInfo.m_nMDKillNum	= pkRoomUser->GetNumMDKill();
			kUnitInfo.m_nDeathNum	= pkRoomUser->GetNumDie();
			kUnitInfo.m_bWin	= false;
			kUnitInfo.m_bIsMVP	= false;

			//drawn �̸� �⺻���ø� �ְ� ������.
			if( kPacketNot.m_bIsDrawn == true )
			{
				kPacketNot.m_vecPVPUnitInfo.push_back( kUnitInfo );
				continue;
			}

			const KPVPResultTable::ResultData* pResultData = NULL; 
			if( pkRoomUser->GetTeam() == kPacketNot.m_cWinTeam )
			{
				kUnitInfo.m_bWin = true;
				//�¸����� �¸�����				
				pResultData = SiKPVPResultTable()->GetTeamMatchWin( iPVPChannelClass );
			}
			else
			{
				//�й����� �й躸��
				pResultData = SiKPVPResultTable()->GetTeamMatchLose( iPVPChannelClass );
			}

			if( pResultData != NULL )
			{
				int teamScore = spRoomUserManager->GetTeamScore( pkRoomUser->GetTeam() );
				//if( teamScore <= 0 )
				{
					teamScore = 1;
				}

				kUnitInfo.m_nVSPoint	= pResultData->m_VP		* teamScore;
				kUnitInfo.m_nEXP		= pResultData->m_EXP	* teamScore;

				int iVSPoint = kUnitInfo.m_nVSPoint;
				int iEXP = kUnitInfo.m_nEXP;
				if( pkRoomUser->IsGameBang() == true )
				{
#ifdef SERV_PC_BANG_TYPE
					kUnitInfo.m_nVSPoint	+= (int)(iVSPoint * SiKGameSysVal()->GetGBVPRate( pkRoomUser->GetPcBangType() ));
					kUnitInfo.m_nEXP		+= (int)(iEXP * SiKGameSysVal()->GetGBEXPRate( pkRoomUser->GetPcBangType() ));
#else SERV_PC_BANG_TYPE
					kUnitInfo.m_nVSPoint	+= (int)(iVSPoint * SiKGameSysVal()->GetGBVPRate());
					kUnitInfo.m_nEXP		+= (int)(iEXP * SiKGameSysVal()->GetGBEXPRate());
#endif SERV_PC_BANG_TYPE
				}

				//�̺�Ʈ ���� ����.!
				{
					//{{ 2010. 05. 11  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
					kUnitInfo.m_nVSPoint	+= (int)(iVSPoint * pkRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_VP ) );
					kUnitInfo.m_nEXP		+= (int)(iEXP * pkRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_EXP ) );
#else
					//kUnitInfo.m_nVSPoint	+= (int)(iVSPoint * m_fGameEvent[GEV_VP]);
					//kUnitInfo.m_nEXP		+= (int)(iEXP * m_fGameEvent[GEV_EXP]);
#endif SERV_INTEGRATION
					//}}					
				}

				//080421.hoons.����ġ ���� ĳ���������� ������ ����ó��..
				//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
//#ifdef SERV_ROOMUSERINFO_REFAC
				const float fAddPremiumExpRate = pkRoomUser->GetBonusRate( KRoomUserInfo::BT_PREMIUM_EXP_RATE );
//#endif SERV_ROOMUSERINFO_REFAC
				//}}
				if( fAddPremiumExpRate > 0.0f )
				{
					kUnitInfo.m_nEXP += (int)(iEXP * fAddPremiumExpRate);
				}

				//limit level
				if( pkRoomUser->GetLevel() >= SiKGameSysVal()->GetLimitsLevel() )
				{
					kUnitInfo.m_nEXP = 0;
				}
			}
			else
			{
				kUnitInfo.m_nVSPoint	= 0;
				kUnitInfo.m_nEXP		= 0;
			}

			float fPlayTime = 0.0f;
			if( spRoomUserManager->GetPlayTime( pkRoomUser->GetCID(), fPlayTime ) == true )
			{
				if( fPlayTime < SEnum::ET_PVP_DRAWN_TIME )
				{
					kUnitInfo.m_nVSPoint	= 0;
					kUnitInfo.m_nEXP		= 0;
				}
			}

			kPacketNot.m_vecPVPUnitInfo.push_back( kUnitInfo );
		}
	}

	if( kPacketNot.m_bIsDrawn == true )
		return true;

	//////////////////////////////////////////////////////////////////////////
	//���ӳ� ���ּ��� ����

	//�ƽ��������� ��������
	SortbyPVPMD( kPacketNot.m_vecPVPUnitInfo );

	//ų���� ��������
	SortbyPVPKill( kPacketNot.m_vecPVPUnitInfo );

	//�ƽ������� & ų���� ���� ��� HP�� ��������
	SortbyPVPHP( kPacketNot.m_vecPVPUnitInfo, spRoomUserManager, CXSLRoom::PGT_TEAM );

	// the same rank and level
	SortbyPVPLevel( kPacketNot.m_vecPVPUnitInfo, spRoomUserManager, CXSLRoom::PGT_TEAM );

	// and slotid..?
	SortbyPVPSlot( kPacketNot.m_vecPVPUnitInfo, spRoomUserManager, CXSLRoom::PGT_TEAM );

	bool bIsSelectionMVP = false;
	for( int i = 0; i < (int)kPacketNot.m_vecPVPUnitInfo.size(); i++ )
	{
		KPVPUnitResultInfo* pkUnitInfo = &kPacketNot.m_vecPVPUnitInfo[i];
		pkUnitInfo->m_nRanking = i+1;
		
		//Ǯ���϶� MVP�� �����Ѵ�.
		if( spRoomUserManager->GetNumTotalSlot() == spRoomUserManager->GetNumResultPlayer() )
		{
			//��ŷ�߿��� ���� ������ŷ�̰� �¸����̸� MVP
			if( bIsSelectionMVP == false && pkUnitInfo->m_bWin == true )
			{
				pkUnitInfo->m_bIsMVP = true;
				bIsSelectionMVP = true;
			}
		}
	}

	return true;
}

bool KResultProcess::Result_SURVIVAL(	IN const float fTotalPlayTime,
										IN const float fRemainingPlayTime, 
										IN const int iPVPChannelClass, 
										IN OUT KRoomUserManagerPtr spRoomUserManager, 
										OUT KEGS_END_GAME_PVP_RESULT_DATA_NOT& kPacketNot )
{
	KRoomUserPtr		pkRoomUser;

	kPacketNot.m_bIsDrawn = false;
	kPacketNot.m_cWinTeam = CXSLRoom::TN_NONE;

	//���º� ó��
	if( spRoomUserManager->GetNumResultPlayer() <= 1 )
	{
		if( (fTotalPlayTime - fRemainingPlayTime) < SEnum::ET_PVP_DRAWN_TIME )
		{
			kPacketNot.m_bIsDrawn = true;
		}
	}

	//����ó���� ���������� �̾Ƴ���.
	for( int i = 0; i < spRoomUserManager->GetNumMember(); i++ )
	{
		pkRoomUser = spRoomUserManager->GetUser( i );

		//���â���� �������� ���� ������ ������� ����.
		if( pkRoomUser != NULL && pkRoomUser->GetStateID() == KRoomUserFSM::S_RESULT )
		{			
			KPVPUnitResultInfo	kUnitInfo;

			kUnitInfo.m_UnitUID		= pkRoomUser->GetCID();
			kUnitInfo.m_nKillNum	= pkRoomUser->GetNumKill();
			kUnitInfo.m_nMDKillNum	= pkRoomUser->GetNumMDKill();
			kUnitInfo.m_nDeathNum	= pkRoomUser->GetNumDie();

			if( kPacketNot.m_bIsDrawn == true )
			{
				kPacketNot.m_vecPVPUnitInfo.push_back( kUnitInfo );
				continue;
			}

			kUnitInfo.m_nVSPoint	= pkRoomUser->GetRewardVP();
			kUnitInfo.m_nEXP		= pkRoomUser->GetRewardEXP();

			float fPlayTime = 0;
			if( spRoomUserManager->GetPlayTime( pkRoomUser->GetCID(), fPlayTime ) == true )
			{
				if( fPlayTime < SEnum::ET_PVP_DRAWN_TIME )
				{
					kUnitInfo.m_nVSPoint	= 0;
					kUnitInfo.m_nEXP		= 0;
				}
			}

			kPacketNot.m_vecPVPUnitInfo.push_back( kUnitInfo );
		}
	}

	if( kPacketNot.m_bIsDrawn == true )
		return true;

	//////////////////////////////////////////////////////////////////////////
	//���ӳ� ���ּ��� ����

	//�ƽ��������� ��������
	SortbyPVPMD( kPacketNot.m_vecPVPUnitInfo );

	//ų���� ��������
	SortbyPVPKill( kPacketNot.m_vecPVPUnitInfo );

	//�ƽ������� & ų���� ���� ��� HP�� ��������
	SortbyPVPHP( kPacketNot.m_vecPVPUnitInfo, kRoomUserManager, CXSLRoom::PGT_SURVIVAL );

	// the same rank and level
	SortbyPVPLevel( kPacketNot.m_vecPVPUnitInfo, kRoomUserManager, CXSLRoom::PGT_SURVIVAL );

	// and slotid..?
	SortbyPVPSlot( kPacketNot.m_vecPVPUnitInfo, kRoomUserManager, CXSLRoom::PGT_SURVIVAL );

	//mvp &  rank & is win
	const KPVPResultTable::ResultData* pRDwin	= SiKPVPResultTable()->GetTeamMatchWin( iPVPChannelClass );
	const KPVPResultTable::ResultData* pRDLose	= SiKPVPResultTable()->GetTeamMatchLose( iPVPChannelClass );

	if( pRDwin == NULL || pRDLose == NULL )
	{
		START_LOG( cerr, L"SURVIVAL.. Team ��������� ���� result data ������ �̻�.!" )
			<< BUILD_LOG( iPVPChannelClass )
			<< END_LOG;
	}

	for( int i = 0; i < (int)kPacketNot.m_vecPVPUnitInfo.size(); i++ )
	{
		KPVPUnitResultInfo* pkUnitInfo = &kPacketNot.m_vecPVPUnitInfo[i];
		pkUnitInfo->m_nRanking = i+1;
		if( i == 0 )
		{
			//Ǯ���϶� MVP�� �����Ѵ�.
			if( spRoomUserManager->GetNumTotalSlot() == spRoomUserManager->GetNumResultPlayer() )
				pkUnitInfo->m_bIsMVP = true;
		}
		else
		{
			pkUnitInfo->m_bIsMVP = false;
		}

		int tempMiddleValue = (int)(kPacketNot.m_vecPVPUnitInfo.size() / 2);
		if ( tempMiddleValue <= 0 )
			tempMiddleValue = 1;
		if( pkUnitInfo->m_nRanking <= tempMiddleValue )
		{
			pkUnitInfo->m_bWin = true;

			if( pRDwin != NULL )
			{
				pkUnitInfo->m_nVSPoint	+= pRDwin->m_VP;
				pkUnitInfo->m_nEXP		+= pRDwin->m_EXP;
			}
		}
		else
		{
			pkUnitInfo->m_bWin = false;

			if( pRDLose != NULL )
			{
				pkUnitInfo->m_nVSPoint	+= pRDLose->m_VP;
				pkUnitInfo->m_nEXP		+= pRDLose->m_EXP;
			}
		}

		int iVSPoint = pkUnitInfo->m_nVSPoint;
		int iEXP = pkUnitInfo->m_nEXP;
		if( spRoomUserManager->IsGameBang( pkUnitInfo->m_UnitUID ) == true )
		{
#ifdef SERV_PC_BANG_TYPE
			pkUnitInfo->m_nVSPoint	+= (int)(iVSPoint * SiKGameSysVal()->GetGBVPRate( spRoomUserManager->GetPcBangType( pkUnitInfo->m_UnitUID ) ));
			pkUnitInfo->m_nEXP		+= (int)(iEXP * SiKGameSysVal()->GetGBEXPRate( spRoomUserManager->GetPcBangType( pkUnitInfo->m_UnitUID ) ));
#else SERV_PC_BANG_TYPE
			pkUnitInfo->m_nVSPoint	+= (int)(iVSPoint * SiKGameSysVal()->GetGBVPRate());
			pkUnitInfo->m_nEXP		+= (int)(iEXP * SiKGameSysVal()->GetGBEXPRate());
#endif SERV_PC_BANG_TYPE
		}

		pkRoomUser = spRoomUserManager->GetUser( pkUnitInfo->m_UnitUID );
		int iUnitLevel = 0;
		if( pkRoomUser != NULL )
			iUnitLevel = pkRoomUser->GetLevel();

		//�̺�Ʈ ���� ����.!
		{
			//{{ 2010. 05. 11  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
			pkUnitInfo->m_nVSPoint	+= (int)(iVSPoint * pkRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_VP ) );
			pkUnitInfo->m_nEXP		+= (int)(iEXP * pkRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_EXP ) );
#else
			//SetGameEvent( 0, pkRoomUser->GetLevel(), pkRoomUser->GetUnitClass_LUA() );

			//pkUnitInfo->m_nVSPoint	+= (int)(iVSPoint * m_fGameEvent[GEV_VP]);
			//pkUnitInfo->m_nEXP		+= (int)(iEXP * m_fGameEvent[GEV_EXP]);
#endif SERV_INTEGRATION
			//}}			
		}

		//080421.hoons.����ġ ���� ĳ���������� ������ ����ó��..
		//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
//#ifdef SERV_ROOMUSERINFO_REFAC
		const float fAddPremiumExpRate = pkRoomUser->GetBonusRate( KRoomUserInfo::BT_PREMIUM_EXP_RATE );
//#endif SERV_ROOMUSERINFO_REFAC
		//}}
		if( fAddPremiumExpRate > 0.0f )
		{
			pkUnitInfo->m_nEXP += (int)(iEXP * fAddPremiumExpRate);
		}

		//limit level
		if( iUnitLevel >= SiKGameSysVal()->GetLimitsLevel() )
		{
			pkUnitInfo->m_nEXP = 0;
		}
	}

	return true;
}

bool KResultProcess::Result_TEAM_DEATH(	IN const float fTotalPlayTime, 
										IN const float fRemainingPlayTime, 
										IN const int iPVPChannelClass, 
										IN OUT KRoomUserManagerPtr spRoomUserManager, 
										OUT KEGS_END_GAME_PVP_RESULT_DATA_NOT& kPacketNot )
{
	KRoomUserPtr pkRoomUser;

	kPacketNot.m_cWinTeam = static_cast<char>( DecidePVPWinTeam( fTotalPlayTime, fRemainingPlayTime, kRoomUserManager, CXSLRoom::PGT_TEAM_DEATH ) );

	if( kPacketNot.m_cWinTeam == CXSLRoom::TN_NONE )
		kPacketNot.m_bIsDrawn = true;
	else
		kPacketNot.m_bIsDrawn = false;

	//�¸����� ������ �¸�����, �ƴϸ� �й躸��
	for( int i = 0; i < spRoomUserManager->GetNumMember(); ++i )
	{
		pkRoomUser = spRoomUserManager->GetUser( i );

		//���â���� �������� ���� ������ ������� ����.
		if( pkRoomUser != NULL && pkRoomUser->GetStateID() == KRoomUserFSM::S_RESULT )
		{			
			//{{ 2010. 05. 11  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
#else
			SetGameEvent( 0, pkRoomUser->GetLevel(), pkRoomUser->GetUnitClass_LUA() );
#endif SERV_INTEGRATION
			//}}

			KPVPUnitResultInfo	kUnitInfo;

			kUnitInfo.m_UnitUID		= pkRoomUser->GetCID();
			kUnitInfo.m_nKillNum	= pkRoomUser->GetNumKill();
			kUnitInfo.m_nMDKillNum	= pkRoomUser->GetNumMDKill();
			kUnitInfo.m_nDeathNum	= pkRoomUser->GetNumDie();
			kUnitInfo.m_bWin	= false;
			kUnitInfo.m_bIsMVP	= false;

			if( kPacketNot.m_bIsDrawn == true )
			{
				kPacketNot.m_vecPVPUnitInfo.push_back( kUnitInfo );
				continue;
			}

			const KPVPResultTable::ResultData* pResultData = NULL; 
			if( pkRoomUser->GetTeam() == kPacketNot.m_cWinTeam )
			{
				kUnitInfo.m_bWin = true;

				//�¸����� �¸�����				
				pResultData = SiKPVPResultTable()->GetTeamMatchWin( iPVPChannelClass );
			}
			else
			{
				//�й����� �й躸��
				pResultData = SiKPVPResultTable()->GetTeamMatchLose( iPVPChannelClass );
			}

			if( pResultData != NULL )
			{
				kUnitInfo.m_nVSPoint	= pResultData->m_VP		+ pkRoomUser->GetRewardVP();
				kUnitInfo.m_nEXP		= pResultData->m_EXP	+ pkRoomUser->GetRewardEXP();
			}
			else
			{
				START_LOG( cerr, L"������ ��������� ���� result data ������ �̻�.!" )
					<< BUILD_LOG( iPVPChannelClass )
					<< END_LOG;


				kUnitInfo.m_nVSPoint	= pkRoomUser->GetRewardVP();
				kUnitInfo.m_nEXP		= pkRoomUser->GetRewardEXP();
			}

			float fPlayTime = 0.0f;
			if( spRoomUserManager->GetPlayTime( pkRoomUser->GetCID(), fPlayTime ) == true )
			{
				if( fPlayTime < SEnum::ET_PVP_DRAWN_TIME )
				{
					kUnitInfo.m_nVSPoint	= 0;
					kUnitInfo.m_nEXP		= 0;
				}
			}

			int iVSPoint = kUnitInfo.m_nVSPoint;
			int iEXP = kUnitInfo.m_nEXP;
			if( pkRoomUser->IsGameBang() == true )
			{
#ifdef SERV_PC_BANG_TYPE
				kUnitInfo.m_nVSPoint	+= (int)(iVSPoint * SiKGameSysVal()->GetGBVPRate( pkRoomUser->GetPcBangType() ));
				kUnitInfo.m_nEXP		+= (int)(iEXP * SiKGameSysVal()->GetGBEXPRate( pkRoomUser->GetPcBangType() ));
#else SERV_PC_BANG_TYPE
				kUnitInfo.m_nVSPoint	+= (int)(iVSPoint * SiKGameSysVal()->GetGBVPRate());
				kUnitInfo.m_nEXP		+= (int)(iEXP * SiKGameSysVal()->GetGBEXPRate());
#endif SERV_PC_BANG_TYPE
			}

			//�̺�Ʈ ���� ����.!
			{
				//{{ 2010. 05. 11  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
				kUnitInfo.m_nVSPoint	+= (int)(iVSPoint * pkRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_VP ) );
				kUnitInfo.m_nEXP		+= (int)(iEXP * pkRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_EXP ) );
#else
				//kUnitInfo.m_nVSPoint	+= (int)(iVSPoint * m_fGameEvent[GEV_VP]);
				//kUnitInfo.m_nEXP		+= (int)(iEXP * m_fGameEvent[GEV_EXP]);
#endif SERV_INTEGRATION
				//}}			
			}

			//080421.hoons.����ġ ���� ĳ���������� ������ ����ó��..
			//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
//#ifdef SERV_ROOMUSERINFO_REFAC
			const float fAddPremiumExpRate = pkRoomUser->GetBonusRate( KRoomUserInfo::BT_PREMIUM_EXP_RATE );
//#endif SERV_ROOMUSERINFO_REFAC
			//}}
			if( fAddPremiumExpRate > 0.0f )
			{
				kUnitInfo.m_nEXP += (int)(iEXP * fAddPremiumExpRate);
			}

			//limit level
			if( pkRoomUser->GetLevel() >= SiKGameSysVal()->GetLimitsLevel() )
			{
				kUnitInfo.m_nEXP = 0;
			}

			kPacketNot.m_vecPVPUnitInfo.push_back( kUnitInfo );
		}
	}

	if( kPacketNot.m_bIsDrawn == true )
		return true;

	//�ƽ��������� ��������
	SortbyPVPMD( kPacketNot.m_vecPVPUnitInfo );

	//ų���� ��������
	SortbyPVPKill( kPacketNot.m_vecPVPUnitInfo );

	//�ƽ������� & ų���� ���� ��� HP�� ��������
	SortbyPVPHP( kPacketNot.m_vecPVPUnitInfo, kRoomUserManager, CXSLRoom::PGT_TEAM_DEATH );

	// the same rank and level
	SortbyPVPLevel( kPacketNot.m_vecPVPUnitInfo, kRoomUserManager, CXSLRoom::PGT_TEAM_DEATH );

	// and slotid..?
	SortbyPVPSlot( kPacketNot.m_vecPVPUnitInfo, kRoomUserManager, CXSLRoom::PGT_TEAM_DEATH );

	bool bIsSelectionMVP = false;
	for( int i = 0; i < (int)kPacketNot.m_vecPVPUnitInfo.size(); i++ )
	{
		KPVPUnitResultInfo* pkUnitInfo = &kPacketNot.m_vecPVPUnitInfo[i];
		pkUnitInfo->m_nRanking = i+1;
		
		//Ǯ���϶� MVP�� �����Ѵ�.
		if( spRoomUserManager->GetNumTotalSlot() == spRoomUserManager->GetNumResultPlayer() )
		{
			//�¸����� ������ŷ�� �̾� MVP
			if( bIsSelectionMVP == false && pkUnitInfo->m_bWin == true )
			{
				pkUnitInfo->m_bIsMVP = true;
				bIsSelectionMVP = true;
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}

bool KResultProcess::Result_DUNGEON( IN OUT KRoomUserManagerPtr spRoomUserManager,
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
									)
{
	//�������� �� ��������� �����Ѵ�..
	kUnitInfoNot.m_bIsWin		= bIsWin;
	kUnitInfoNot.m_nSec			= nTotalPlayTime;
	kUnitInfoNot.m_iDungeonID	= iDungeonID;
	kUnitInfoNot.m_cDifficulty	= cDifficulty;
	//{{ 2010. 05. 01  ������	��д��� ����
#ifdef SERV_SECRET_HELL
	kUnitInfoNot.m_cDungeonMode	= cDungeonMode;
#endif SERV_SECRET_HELL
	//}}

	//������ ���� ����ó���� ������ ��찡 �־� �߰���..
	const int iEndNumMember = spRoomUserManager->GetNumMember();
	if( iEndNumMember <= 0 )
	{
		START_LOG( cerr, L"���� ������ ���µ� ���ó���� �����.!" )
			<< BUILD_LOG( iEndNumMember )
			<< END_LOG;

		return false;
	}

	//�� UNIT �� ���ó�� ����...........

	//1. mvp ����
	bool			bIsSelectMVP = false;
	KRoomUserPtr	pkMVPRoomuser;

	//{{ 2013. 03. 05	���� MVP ���� ���� ���� - ��μ�
#ifdef SERV_DUNGEON_MVP
	//Ǯ��Ƽ �ϰ�츸 MVP�� �����Ѵ�.
	if( spRoomUserManager->GetNumMember() >= 3 )
	{
		bIsSelectMVP = true;

		int iTotalDamage = 0;

		for( int idx = 0; idx < spRoomUserManager->GetNumMember(); ++idx )
		{
			KRoomUserPtr pkRoomUser = spRoomUserManager->GetUser( idx );
			if( pkRoomUser == NULL )
				continue;

			if( bIsSelectMVP == true && iTotalDamage < pkRoomUser->GetTotalGivenDamamge_LUA() )
			{
				pkMVPRoomuser	= pkRoomUser;
				iTotalDamage	= pkRoomUser->GetTotalGivenDamamge_LUA();
			}
		}
	}
#else
	//Ǯ��Ƽ �ϰ�츸 MVP�� �����Ѵ�.
	if( spRoomUserManager->GetNumMember() == spRoomUserManager->GetNumTotalSlot() )
	{
		bIsSelectMVP = true;

		int iMaxKill = 0;

		for( int idx = 0; idx < spRoomUserManager->GetNumMember(); ++idx )
		{
			KRoomUserPtr pkRoomUser = spRoomUserManager->GetUser( idx );
			if( pkRoomUser == NULL )
				continue;

			if( bIsSelectMVP == true && iMaxKill < pkRoomUser->GetKillNPC_LUA() )
			{
				pkMVPRoomuser = pkRoomUser;
				iMaxKill	  = pkRoomUser->GetKillNPC_LUA();
			}
		}
	}
#endif SERV_DUNGEON_MVP
	//}

	//2. �� UNIT �� �ð���ũ ��꿡 ���� ��� ���� / ų�� ����ϱ�
	int nAverageLevel		= 0;
	int	nTotalKillNPCNum	= 0;
	int iMaxKill = 0;
	//{{ 2011. 02. 22	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
	int iPartyTotalGivenDamage = 0;
#endif SERV_DUNGEON_RANK_NEW
	//}}
	for( int idx = 0; idx < spRoomUserManager->GetNumMember(); ++idx )
	{
		KRoomUserPtr pkRoomUser  = spRoomUserManager->GetUser( idx );
		_JIF( pkRoomUser != NULL, continue; );

		nAverageLevel			+= pkRoomUser->GetLevel();
		nTotalKillNPCNum		+= pkRoomUser->GetKillNPC_LUA();
		//{{ 2011. 02. 22	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
		iPartyTotalGivenDamage	+= pkRoomUser->GetTotalGivenDamamge_LUA();
#endif SERV_DUNGEON_RANK_NEW
		//}}

		// ������ �������� ī��Ʈ ���ϱ�
		if( iClearStageCount < pkRoomUser->GetPassedStageCount() )
			iClearStageCount = pkRoomUser->GetPassedStageCount();
	}

	// ��շ����� ���� ������ ��� ���Ѵ�.
	nAverageLevel = nAverageLevel / spRoomUserManager->GetNumMember(); // ������ 0���� üũ�մϴ�

	//3. �� UNIT�� ������� ����..
	for( int i = 0; i < spRoomUserManager->GetNumMember(); ++i )
	{
		KDungeonUnitResultInfo					kDungeonUnitInfo;
		KERM_UPDATE_DUNGEON_UNIT_INFO_NOT		kUpdateUnitInfo;

		KRoomUserPtr spRoomUser = spRoomUserManager->GetUser( i );
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"������ ������ �̻�.!" )
				<< BUILD_LOG( i )
				<< END_LOG;
			continue;
		}

		//{{ 2010. 05. 11  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
#else
		SetGameEvent( iDungeonID, spRoomUser->GetLevel(), spRoomUser->GetUnitClass_LUA(), spRoomUserManager->GetLiveMember() );
#endif SERV_INTEGRATION
		//}}

		vecKRoomUserPtr.push_back( spRoomUser );

		//3.1 �÷��� �⺻��� ����..
		kDungeonUnitInfo.m_bIsMVP			= false;
		kDungeonUnitInfo.m_UnitUID			= spRoomUser->GetCID();
		kDungeonUnitInfo.m_nTotalScore		= spRoomUser->GetTotalScore();
		kDungeonUnitInfo.m_nComboScore		= spRoomUser->GetComboScore_LUA();
		kDungeonUnitInfo.m_nTechnicalScore	= spRoomUser->GetTechnicalScore_LUA();
		kDungeonUnitInfo.m_nDamaged			= spRoomUser->GetDamageNum_LUA();
		kDungeonUnitInfo.m_nKillNPCNum		= spRoomUser->GetKillNPC_LUA();
		kDungeonUnitInfo.m_bIsDie			= spRoomUser->IsDie();
		//{{ 2009. 6. 10  ������	���� ���۽� ����
		kDungeonUnitInfo.m_nOldEXP			= spRoomUser->GetOldEXP();
		kDungeonUnitInfo.m_nOldED			= spRoomUser->GetOldED();
		//}}		

		//3.2 ����Ŭ���� ��������
		kUpdateUnitInfo.m_bDungeonClear	= bIsWin;
		kUpdateUnitInfo.m_iMaxScore		= kDungeonUnitInfo.m_nTotalScore;		//����Ŭ���� ������ �ְ������� ����ϱ� ����
		kUpdateUnitInfo.m_iDungeonID	= iDungeonID + static_cast<int>(cDifficulty);
		kUpdateUnitInfo.m_iUserCount	= spRoomUserManager->GetNumMember();
		kUpdateUnitInfo.m_iPlayTime		= nTotalPlayTime; // ���� ��踦 ���� �÷��� Ÿ�� ���
		kUpdateUnitInfo.m_iDropItemUID	= nDropItemUID; // �ӽ� �κ��丮�� ���� DropItemUID
		//{{ 2012. 04. 16	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		kUpdateUnitInfo.m_bStartedByAutoParty = bStartedByAutoParty;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
		std::map< int, KSubQuestInfo > mapOngoingQuest;
		spRoomUser->GetOngoingQuest( CXSLQuestManager::SQT_ITEM_EQUIP_DUNGEON_CLEAR, mapOngoingQuest );
		kUnitInfoNot.m_mapOngoingQuest.insert( std::make_pair( spRoomUser->GetCID(), mapOngoingQuest ) );
#endif SERV_QUEST_CLEAR_EXPAND
		//}}

		//////////////////////////////////////////////////////////////////////////
		//{{ 2011. 08. 22	������	��ϸ� �ð� ����
#ifdef SERV_NEW_HENIR_TEST
#ifndef SERV_HENIR_NO_REWARD
		if( eDungeonType == CXSLDungeon::DT_HENIR )
		{
			if( spRoomUser->IsHenirRewardUser() == true )
			{
				// �����ߵ� �����ߵ� ������� ��������! ���� ����!!
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-23	// �ڼ���
				if( SiKHenirResultTable()->GetHenirChallangeRewardItem( cDungeonMode, iStageID, kUpdateUnitInfo.m_mapResultItem ) == true )
#else // SERV_HENIR_RENEWAL_2013
				if( SiKHenirResultTable()->GetHenirChallangeRewardItem( iStageID, kUpdateUnitInfo.m_mapResultItem ) == true )
#endif // SERV_HENIR_RENEWAL_2013
				{
					std::map< int, KItemInfo >::const_iterator mitHR;
					for( mitHR = kUpdateUnitInfo.m_mapResultItem.begin(); mitHR != kUpdateUnitInfo.m_mapResultItem.end(); ++mitHR )
					{
						spRoomUser->AddItem( mitHR->second.m_iItemID, mitHR->second.m_iQuantity );

						//////////////////////////////////////////////////////////////////////////
						START_LOG( clog, L"���� ����!" )
#ifdef SERV_PRIVACY_AGREEMENT
							<< BUILD_LOG( spRoomUser->GetCID() )
#else
							<< BUILD_LOG( spRoomUser->GetNickName() )
#endif SERV_PRIVACY_AGREEMENT
							<< BUILD_LOG( mitHR->second.m_iItemID )
							<< BUILD_LOG( mitHR->second.m_iQuantity );
						//////////////////////////////////////////////////////////////////////////
					}
				}
				else
				{
					START_LOG( cerr, L"��ϸ��� �ð� ���� ���� ������ ã�� ���Ͽ����ϴ�." )
						<< BUILD_LOG( kDungeonUnitInfo.m_UnitUID )
						<< BUILD_LOG( iStageID )
						<< END_LOG;
				}
			}
		}
#endif //SERV_HENIR_NO_REWARD
#endif SERV_NEW_HENIR_TEST
		//}}
		//////////////////////////////////////////////////////////////////////////

		//{{ 2011. 12. 06	������	PC�� ���� ���� Ŭ���� �� �߰� ������ ���� �̺�Ʈ
#ifdef SERV_PC_BANG_USER_DUNGEON_CLEAR_PAYMENT_ITEM
		if( bIsWin == true )
		{
			// PC�� ������ ��� �޴� ����
			if( spRoomUser->IsGameBang() == true  &&  spRoomUserManager->IsRoomUserGetExp( spRoomUser->GetCID() ) == true )
			{
				const CXSLItem::ItemTemplet *pItemTemplet = SiCXSLItemManager()->GetItemTemplet( CXSLItem::EI_COBO_PC_BANG_COUPON );
				if( pItemTemplet == NULL )
				{
					START_LOG( cerr, L"������ ������ �������� �ʽ��ϴ�!" )
						<< BUILD_LOG( CXSLItem::EI_COBO_PC_BANG_COUPON )
						<< END_LOG;
				}
				else
				{
					KItemInfo kInfo;
					kInfo.m_iItemID		  = CXSLItem::EI_COBO_PC_BANG_COUPON; // �ں� PC�� ����
					kInfo.m_cUsageType	  = pItemTemplet->m_PeriodType;
					kInfo.m_iQuantity	  = 1;
					kInfo.m_sEndurance	  = pItemTemplet->m_Endurance;
					kUpdateUnitInfo.m_mapResultItem.insert( std::make_pair( CXSLItem::EI_COBO_PC_BANG_COUPON, kInfo ) );

					// �ش� �������� ���� ���� ������ �߰��Ѵ�. (���â���� ǥ���ϱ� ���ؼ�)
					spRoomUser->AddItem( CXSLItem::EI_COBO_PC_BANG_COUPON, 1 );
				}
			}
		}
#endif SERV_PC_BANG_USER_DUNGEON_CLEAR_PAYMENT_ITEM
		//}}
#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		if( bIsWin == true )
		{
			//���� Ŭ����� �ش� ���� �ΰ� üũ�ؼ� ���⼭ ���� ��������
			//�߷�Ÿ�� �����϶��� üũ�ؼ� ���� ����
			if( iDungeonID == SEnum::DI_EVENT_VALENTINE_DUNGEON_INT )
			{	
				KRoomUserInfo kInfo;
				spRoomUser->GetRoomUserInfo(kInfo);
				START_LOG( clog, L"�߷�Ÿ�� ���� ���� ī��Ʈ" )
					<< BUILD_LOG( kInfo.m_iValentineItemCount )
					<< END_LOG;
				if( kInfo.m_iValentineItemCount < SiKGameEventScriptManager()->GetValenTinePlayCount() )
				{
					//���� ���� �� �ִ�.�ð� ��� ����
					const CXSLItem::ItemTemplet *pItemTemplet = SiCXSLItemManager()->GetItemTemplet( SiKGameEventScriptManager()->GetValenItemID() );
					if( pItemTemplet == NULL )
					{
						START_LOG( cerr, L"������ ������ �������� �ʽ��ϴ�!" )
							<< BUILD_LOG( SiKGameEventScriptManager()->GetValenItemID() )
							<< END_LOG;
					}
					else
					{
						if( iTotalValentineTIme >= SiKGameEventScriptManager()->GetIntermediateTime() ) ///�ִ� �ð� ��ġ 
						{
							KItemInfo kInfo;
							kInfo.m_iItemID		  = pItemTemplet->m_ItemID; //�ﷻ�� ���� ������
							kInfo.m_cUsageType	  = pItemTemplet->m_PeriodType;
							kInfo.m_iQuantity	  = SiKGameEventScriptManager()->GetValenItemNum_1();
							kInfo.m_sEndurance	  = pItemTemplet->m_Endurance;
							kUpdateUnitInfo.m_mapResultItem.insert( std::make_pair( pItemTemplet->m_ItemID, kInfo ) );
							// �ش� �������� ���� ���� ������ �߰��Ѵ�. (���â���� ǥ���ϱ� ���ؼ�)
							spRoomUser->AddItem( pItemTemplet->m_ItemID, kInfo.m_iQuantity );
						}
						else if( iTotalValentineTIme < SiKGameEventScriptManager()->GetIntermediateTime() && iTotalValentineTIme >= SiKGameEventScriptManager()->GetExpertTime() )
						{
							KItemInfo kInfo;
							kInfo.m_iItemID		  = pItemTemplet->m_ItemID; //�ﷻ�� ���� ������
							kInfo.m_cUsageType	  = pItemTemplet->m_PeriodType;
							kInfo.m_iQuantity	  = SiKGameEventScriptManager()->GetValenItemNum_2();
							kInfo.m_sEndurance	  = pItemTemplet->m_Endurance;
							kUpdateUnitInfo.m_mapResultItem.insert( std::make_pair( pItemTemplet->m_ItemID, kInfo ) );
							// �ش� �������� ���� ���� ������ �߰��Ѵ�. (���â���� ǥ���ϱ� ���ؼ�)
							spRoomUser->AddItem( pItemTemplet->m_ItemID, kInfo.m_iQuantity );
						}
						else if( iTotalValentineTIme < SiKGameEventScriptManager()->GetExpertTime() )
						{
							KItemInfo kInfo;
							kInfo.m_iItemID		  = pItemTemplet->m_ItemID; //�ﷻ�� ���� ������
							kInfo.m_cUsageType	  = pItemTemplet->m_PeriodType;
							kInfo.m_iQuantity	  = SiKGameEventScriptManager()->GetValenItemNum_3();
							kInfo.m_sEndurance	  = pItemTemplet->m_Endurance;
							kUpdateUnitInfo.m_mapResultItem.insert( std::make_pair( pItemTemplet->m_ItemID, kInfo ) );
							// �ش� �������� ���� ���� ������ �߰��Ѵ�. (���â���� ǥ���ϱ� ���ؼ�)
							spRoomUser->AddItem( pItemTemplet->m_ItemID, kInfo.m_iQuantity );
						}
					}
				}
				START_LOG( clog, L" �߷�Ÿ�� ���� �ޱ� ���� ����" )
					<< BUILD_LOG( iTotalValentineTIme )
					<< BUILD_LOG( SiKGameEventScriptManager()->GetBeginnerTime() )
					<< BUILD_LOG( SiKGameEventScriptManager()->GetIntermediateTime() )
					<< BUILD_LOG( SiKGameEventScriptManager()->GetExpertTime() )
					<< BUILD_LOG( SiKGameEventScriptManager()->GetValenItemID() )
					<< END_LOG;
			}
		}
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		//////////////////////////////////////////////////////////////////////////
		// �׾��� ��  ����     ��ϸ��� �ð� ���� ó��
		//////////////////////////////////////////////////////////////////////////
		if( spRoomUser->IsDie() == true )
		{
			//�װ����� ������� �� ����ó���� db���� ���ְ� ���â���� �������� �ʴ´�.
			kDungeonUnitInfo.m_cTotalRank			= CXSLDungeon::RT_F;
			kDungeonUnitInfo.m_cComboRank			= CXSLDungeon::RT_F;
			kDungeonUnitInfo.m_cDamagedRank			= CXSLDungeon::RT_F;
			kDungeonUnitInfo.m_cTechnicalRank		= CXSLDungeon::RT_F;
			kDungeonUnitInfo.m_cTimeRank			= CXSLDungeon::RT_F;

			kUpdateUnitInfo.m_bDungeonClear			= false;	//��������� ��������͵� Ŭ�����Ѱ� �ƴϴ�.
			kUpdateUnitInfo.m_cMaxTotalRank			= kDungeonUnitInfo.m_cTotalRank;

			//////////////////////////////////////////////////////////////////////////
			// ����ġ ����

			// 1. �⺻ ����ġ
			kDungeonUnitInfo.m_nEXP				= spRoomUser->GetRewardEXP();
			
			// 2. ��Ƽ ���ʽ�
			kDungeonUnitInfo.m_nPartyBonusEXP	= spRoomUser->GetRewardPartyEXP();

			// 3. ���ӹ� ���ʽ�
			if( spRoomUser->IsGameBang() == true )
			{
#ifdef SERV_PC_BANG_TYPE
				kDungeonUnitInfo.m_nGameBangBonusEXP	= (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * SiKGameSysVal()->GetGBEXPRate( spRoomUser->GetPcBangType() ));
#else SERV_PC_BANG_TYPE
				kDungeonUnitInfo.m_nGameBangBonusEXP	= (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * SiKGameSysVal()->GetGBEXPRate());
#endif SERV_PC_BANG_TYPE
			}

			// 4. �̺�Ʈ ���� ����.!
			{
				//{{ 2010. 05. 11  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
				kDungeonUnitInfo.m_nEventEXP = (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * spRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_EXP ) );
				kDungeonUnitInfo.m_nEventEXP += (int)(kDungeonUnitInfo.m_nPartyBonusEXP * spRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_PARTY_EXP ) );
	#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
				kDungeonUnitInfo.m_nEventEXP += (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * spRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_WITH_CHAR_EXP ) );
	#endif // SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP

#else
				//kDungeonUnitInfo.m_nEventEXP = (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * m_fGameEvent[GEV_EXP]);
				//kDungeonUnitInfo.m_nEventEXP += (int)(kDungeonUnitInfo.m_nPartyBonusEXP * m_fGameEvent[GEV_PARTY_EXP]);
#endif SERV_INTEGRATION
				//}}
			}

			// 5. ä�κ��ʽ�
			{
				//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
//#ifdef SERV_ROOMUSERINFO_REFAC
				kDungeonUnitInfo.m_nChannelBonusEXP = (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * spRoomUser->GetBonusRate( KRoomUserInfo::BT_CHANNEL_BONUS_EXP_RATE ));
//#endif SERV_ROOMUSERINFO_REFAC				
			}

			// 6. �����̾� ����
			//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
//#ifdef SERV_ROOMUSERINFO_REFAC
			float fAddPremiumExpRate = spRoomUser->GetBonusRate( KRoomUserInfo::BT_PREMIUM_EXP_RATE );
//#endif SERV_ROOMUSERINFO_REFAC
			//}}

			if( fAddPremiumExpRate > 0.0f )
			{
				kDungeonUnitInfo.m_nPremiumBonusEXP += (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * fAddPremiumExpRate);
			}

			// 7. ���� ����
			if( spRoomUser->GetLevel() >= SiKGameSysVal()->GetLimitsLevel() )
			{
				kDungeonUnitInfo.ResetEXP();
			}

			//db process
			//{{ 2009. 5. 8  ������		�ǽð� ����ġ
			kUpdateUnitInfo.m_iEXP = kDungeonUnitInfo.SumEXP() - ( kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP );
			//}}

			// ������ ����
			spRoomUserManager->GetItemList( spRoomUser->GetCID(), kUpdateUnitInfo.m_mapGetItem );

			kUnitInfoNot.m_vecDungeonUnitInfo.push_back( kDungeonUnitInfo );

			//db�� ������Ʈ �� ������
			vecUpdateUnitInfo.push_back( kUpdateUnitInfo );
			continue;
		}

		//{{ 2012. 03. 15	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		// �ڵ� ��Ƽ ���ʽ�
#ifdef SERV_DO_NOT_SUPPLY_AUTO_PARTY_BONUS_AT_EVENT_DUNGEON// �۾���¥: 2013-10-23	// �ڼ���
		if( ( bIsWin == true )
			&& ( spRoomUser->IsActivatedAutoPartyBonus() == true )
			&& ( CXSLDungeon::IsEventDungeon( iDungeonID ) == false )
			)
#else
		if( bIsWin  &&  spRoomUser->IsActivatedAutoPartyBonus() )
#endif // SERV_DO_NOT_SUPPLY_AUTO_PARTY_BONUS_AT_EVENT_DUNGEON
		{
			bool bCheckLevelForAutoPartyBonus = false;
			switch( eDungeonType )
			{
			case CXSLDungeon::DT_HENIR:
			case CXSLDungeon::DT_SECRET:
				{
					bCheckLevelForAutoPartyBonus = true;
				}
				break;

			default:
				{
					bCheckLevelForAutoPartyBonus = SiCXSLDungeonManager()->CheckLevelForDungeonAutoPartyBonus( iDungeonID + static_cast<int>(cDifficulty), spRoomUser->GetLevel() );
				}
				break;
			}

			if( bCheckLevelForAutoPartyBonus )
			{
				const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( CXSLItem::RI_AUTO_PARTY_BONUS );
				if( pItemTemplet == NULL )
				{
					START_LOG( cerr, L"������ ������ �������� �ʽ��ϴ�! ���� ���� ����!" )
						<< BUILD_LOG( CXSLItem::RI_AUTO_PARTY_BONUS )
						<< END_LOG;
				}
				else
				{
					KItemInfo kResultItemInfo;
					kResultItemInfo.m_iItemID	 = CXSLItem::RI_AUTO_PARTY_BONUS;
					kResultItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
					kResultItemInfo.m_iQuantity	 = 1;
					kResultItemInfo.m_sEndurance = pItemTemplet->m_Endurance;

					kUpdateUnitInfo.m_mapResultItem.insert( std::make_pair( kResultItemInfo.m_iItemID, kResultItemInfo ) );

					spRoomUser->AddItem( kResultItemInfo.m_iItemID, kResultItemInfo.m_iQuantity );

				}
			}
		}
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		//////////////////////////////////////////////////////////////////////////
		// ��� ���� �� ����     ��ϸ��� �ð� ���� ó��
		//////////////////////////////////////////////////////////////////////////
		if( eDungeonType == CXSLDungeon::DT_HENIR  && bIsWin == true )
		{
			//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
			if( spRoomUser->IsHenirRewardUser() == true )
#endif SERV_NEW_HENIR_TEST
			//}}
			{
				// ���� ���â ����
				//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
#ifdef SERV_NEW_HENIR_TEST
				std::map< int, KItemInfo > mapResultItem;
				if( SiKHenirResultTable()->GetHenirRewardItem( iStageID, cDungeonMode, mapResultItem ) == true )
				{
					std::map< int, KItemInfo >::const_iterator mitHR;
					for( mitHR = mapResultItem.begin(); mitHR != mapResultItem.end(); ++mitHR )
					{
						// �ش� �������� ���� ���� ������ �߰��Ѵ�. (���â���� ǥ���ϱ� ���ؼ�)
						spRoomUser->AddItem( mitHR->second.m_iItemID, mitHR->second.m_iQuantity );

						// ���� ������ ���� ���� �������� �߰��Ѵ�.
						std::map< int, KItemInfo >::iterator mitRI;
						mitRI = kUpdateUnitInfo.m_mapResultItem.find( mitHR->second.m_iItemID );
						if( mitRI == kUpdateUnitInfo.m_mapResultItem.end() )
						{
							kUpdateUnitInfo.m_mapResultItem.insert( std::make_pair( mitHR->first, mitHR->second ) );
						}
						else
						{
                            mitRI->second.m_iQuantity += mitHR->second.m_iQuantity;
						}
					}
				}
#else
				if( SiKHenirResultTable()->GetHenirRewardItem( iClearStageCount, cDungeonMode, kUpdateUnitInfo.m_mapResultItem ) == true )
				{
					std::map< int, KItemInfo >::const_iterator mitHR;
					for( mitHR = kUpdateUnitInfo.m_mapResultItem.begin(); mitHR != kUpdateUnitInfo.m_mapResultItem.end(); ++mitHR )
					{
						spRoomUser->AddItem( mitHR->second.m_iItemID, mitHR->second.m_iQuantity );
					}
				}
#endif SERV_NEW_HENIR_TEST
				//}}				
				else
				{
					START_LOG( cerr, L"��ϸ��� �ð� ���� ������ ã�� ���Ͽ����ϴ�." )
						<< BUILD_LOG( iClearStageCount )
						<< BUILD_LOGc( cDungeonMode )
						<< END_LOG;
				}
			}

			// ��ϸ��� �ð� ��ŷ ���� ���� - ������忡���� ��ŷ ����
			if( cDungeonMode == CXSLDungeon::DM_HENIR_CHALLENGE )
			{
				KHenirRankingInfo kRankingInfo;
				kRankingInfo.m_iRank		= 0;
				kRankingInfo.m_iUnitUID		= spRoomUser->GetCID();
				kRankingInfo.m_wstrNickName = spRoomUser->GetNickName();
				kRankingInfo.m_iStageCount	= iClearStageCount;
				kRankingInfo.m_cUnitClass	= spRoomUser->GetUnitClass_LUA();
				kRankingInfo.m_ucLevel		= spRoomUser->GetLevel();
				kRankingInfo.m_ulPlayTime	= nTotalPlayTime; // nTotalPlayTime : �ε� �ð� ���ɷ� ��ŷ �ð� �־����!
				kRankingInfo.m_tRegDate		= static_cast<__int64>( CTime::GetCurrentTime().GetTime() );
#ifdef SERV_HENIR_RENEWAL_2013// �۾���¥: 2013-09-16	// �ڼ���
				kRankingInfo.m_bytePlayStartedUserNum	= spRoomUserManager->GetPlayStartedUserNum();
#endif // SERV_HENIR_RENEWAL_2013

				kUpdateUnitInfo.m_kHenirRankingInfo.push_back( kRankingInfo );
			}
		}

		//{{ 2011. 05. 04  ��μ�	���� Ŭ����� ������ ���� ���� �߰�
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		if( bIsWin == true )
		{
			bool bGetExp = spRoomUserManager->IsRoomUserGetExp( spRoomUser->GetCID() );
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT

			bool bAraEvent = spRoomUserManager->IsAraEvent();
			SiKDropTable()->CheckDungeonClearPaymentItem( iDungeonID, cDifficulty, bGetExp, spRoomUser,
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
				spRoomUserManager,
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
				kUpdateUnitInfo.m_mapResultItem, bAraEvent  );

#else //SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT

			SiKDropTable()->CheckDungeonClearPaymentItem( iDungeonID, cDifficulty, bGetExp, spRoomUser,
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
				spRoomUserManager,
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
				kUpdateUnitInfo.m_mapResultItem );
#endif //SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT
			
		}
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		//}}

		//////////////////////////////////////////////////////////////////////////
		// ��ũ ���ó��
		//CXSLDungeon::RANK_TYPE	eCombo, eTechnical, eDamage, eTime;
		//float���� exp�� �߰��� ���� %
		float					fComboBonus, fTechBonus, fDamageBonus, fTimeBonus;
		//////////////////////////////////////////////////////////////////////////		
		//COMBO RESULT
		{
			//{{ 2011. 01. 21	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
			lua_tinker::call<double>( GetLuaState(), "DUNGEON_COMBO_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficulty), &kDungeonMonsterManager, iEndNumMember, iPartyTotalGivenDamage );
#else
			lua_tinker::call<double>( GetLuaState(), "DUNGEON_COMBO_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficulty), iStartedNumMember, nTotalKillNPCNum );
#endif SERV_DUNGEON_RANK_NEW
			//}}

			fComboBonus	= spRoomUser->GetPercent();
			kDungeonUnitInfo.m_cComboRank	= (CXSLDungeon::RANK_TYPE)spRoomUser->GetRank();
		}
		
		//////////////////////////////////////////////////////////////////////////
		//TECHNICAL RESULT
		{
			//{{ 2011. 01. 21	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
			lua_tinker::call<double>( GetLuaState(), "DUNGEON_TECHNICAL_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficulty), &kDungeonMonsterManager, iEndNumMember, iPartyTotalGivenDamage );
#else
			lua_tinker::call<double>( GetLuaState(), "DUNGEON_TECHNICAL_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficulty), iStartedNumMember, nTotalKillNPCNum );
#endif SERV_DUNGEON_RANK_NEW
			//}}

			fTechBonus	= spRoomUser->GetPercent();
			kDungeonUnitInfo.m_cTechnicalRank = (CXSLDungeon::RANK_TYPE)spRoomUser->GetRank();
		}

		//////////////////////////////////////////////////////////////////////////		
		//DAMAGE RESULT
		{
			//{{ 2011. 01. 21	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
			lua_tinker::call<double>( GetLuaState(), "DUNGEON_DAMAGE_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficulty), &kDungeonMonsterManager );
#else
			lua_tinker::call<double>( GetLuaState(), "DUNGEON_DAMAGE_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficulty) );
#endif SERV_DUNGEON_RANK_NEW
			//}}			

			fDamageBonus	= spRoomUser->GetPercent();
			kDungeonUnitInfo.m_cDamagedRank	= (CXSLDungeon::RANK_TYPE)spRoomUser->GetRank();
		}

		//////////////////////////////////////////////////////////////////////////		
		//TIME RESULT
		{
			//{{ 2011. 01. 20	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
			(CXSLDungeon::RANK_TYPE)lua_tinker::call<int>( GetLuaState(), "DUNGEON_TIME_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficulty), &kDungeonMonsterManager, kUnitInfoNot.m_nSec );
#else
			(CXSLDungeon::RANK_TYPE)lua_tinker::call<int>( GetLuaState(), "DUNGEON_TIME_RESULT", spRoomUser.get(), nTotalKillNPCNum, nAverageLevel, kUnitInfoNot.m_nSec, (int)(iDungeonID + cDifficulty) );
#endif SERV_DUNGEON_RANK_NEW
			//}}

			fTimeBonus	= spRoomUser->GetPercent();
			kDungeonUnitInfo.m_cTimeRank	= (CXSLDungeon::RANK_TYPE)spRoomUser->GetRank();
		}

		//////////////////////////////////////////////////////////////////////////		
		//::: RANK AVERAGE :::
		{
			//{{ 2011. 02. 22	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
            kDungeonUnitInfo.m_cTotalRank = (char)lua_tinker::call<int>( GetLuaState(), "DUNGEON_TOTAL_RANK_RESULT", kDungeonUnitInfo.m_cComboRank, kDungeonUnitInfo.m_cTechnicalRank, kDungeonUnitInfo.m_cDamagedRank, kDungeonUnitInfo.m_cTimeRank, &kDungeonMonsterManager );

			if( (char)CXSLDungeon::RT_NONE >= kDungeonUnitInfo.m_cTotalRank )
			{
				kDungeonUnitInfo.m_cTotalRank = CXSLDungeon::RT_F;
			}
			else if( (char)CXSLDungeon::RT_SS < kDungeonUnitInfo.m_cTotalRank )
			{
				kDungeonUnitInfo.m_cTotalRank = CXSLDungeon::RT_SS;
			}
#else
			kDungeonUnitInfo.m_cTotalRank = (char)2*kDungeonUnitInfo.m_cComboRank + (char)2*kDungeonUnitInfo.m_cTechnicalRank + (char)kDungeonUnitInfo.m_cDamagedRank + (char)kDungeonUnitInfo.m_cTimeRank;
			kDungeonUnitInfo.m_cTotalRank = kDungeonUnitInfo.m_cTotalRank/6;

			if( (char)CXSLDungeon::RT_NONE >= kDungeonUnitInfo.m_cTotalRank )
			{
				kDungeonUnitInfo.m_cTotalRank = CXSLDungeon::RT_F;
			}
			else if( (char)CXSLDungeon::RT_S < kDungeonUnitInfo.m_cTotalRank )
			{
				kDungeonUnitInfo.m_cTotalRank = CXSLDungeon::RT_S;
			}
#endif SERV_DUNGEON_RANK_NEW
			//}}
			
			//����Ŭ���� ������ ����
			kUpdateUnitInfo.m_cMaxTotalRank			= kDungeonUnitInfo.m_cTotalRank;
		}

		//////////////////////////////////////////////////////////////////////////		
		//Rank Bonus
		//kDungeonUnitInfo.m_fRankBonus	= ( fComboBonus + fTechBonus + fDamageBonus + fTimeBonus );
		float fRankBonusRate = ( fComboBonus + fTechBonus + fDamageBonus + fTimeBonus );
		if( fRankBonusRate > 0.15f )
			fRankBonusRate = 0.15f;

		//////////////////////////////////////////////////////////////////////////
		// ����ġ �� ED����		
		// 1. �⺻ ����
		kDungeonUnitInfo.m_nED	= spRoomUser->GetRewardED();
		kDungeonUnitInfo.m_nEXP	= spRoomUser->GetRewardEXP();

		// 2. ��Ƽ ���ʽ�.
		kDungeonUnitInfo.m_nPartyBonusEXP = spRoomUser->GetRewardPartyEXP();
		
		// 3. ���� Ŭ���� ���ʽ�
		kDungeonUnitInfo.m_nClearBonusEXP = static_cast<int>(( kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP ) * 0.3f);

		// 4. ��ũ ���ʽ�
		kDungeonUnitInfo.m_nRankBonusEXP = static_cast<int>(( kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP ) * fRankBonusRate);

		// 5. MVP ���ʽ�
		if( bIsSelectMVP == true && pkMVPRoomuser == spRoomUser )
		{
			kDungeonUnitInfo.m_nMvpBonusEXP += static_cast<int>((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * 0.1f);
			kDungeonUnitInfo.m_bIsMVP = true;
		}

		// 6. ���ӹ� ���ʽ�
		if( spRoomUser->IsGameBang() == true )
		{
#ifdef SERV_PC_BANG_TYPE
			kDungeonUnitInfo.m_nED					+= (int)(spRoomUser->GetRewardED() * SiKGameSysVal()->GetGBEDRate( spRoomUser->GetPcBangType() ));
			kDungeonUnitInfo.m_nGameBangBonusEXP	 = (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * SiKGameSysVal()->GetGBEXPRate( spRoomUser->GetPcBangType() ));
			// PC�� �����ε� ���� ��� ���̴�.
			if( spRoomUser->GetPetID() > 0 )
			{
				kDungeonUnitInfo.m_nGameBangBonusEXP	 += (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * SiKGameSysVal()->GetGBPetEXPRate( spRoomUser->GetPcBangType() ));
			}
#else SERV_PC_BANG_TYPE
			kDungeonUnitInfo.m_nED					+= (int)(spRoomUser->GetRewardED() * SiKGameSysVal()->GetGBEDRate());
			kDungeonUnitInfo.m_nGameBangBonusEXP	 = (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * SiKGameSysVal()->GetGBEXPRate());

			//{{ 2012. 07. 02	��μ�       PC �� ���� �� �߰� ����ġ
#ifdef SERV_GAME_BANG_PET_ADD_EXP
			// PC�� �����ε� ���� ��� ���̴�.
			if( spRoomUser->GetPetID() > 0 )
			{
				kDungeonUnitInfo.m_nGameBangBonusEXP	 += (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * SiKGameSysVal()->GetGBPetEXPRate());

			}
#endif SERV_GAME_BANG_PET_ADD_EXP
			//}}			
#endif SERV_PC_BANG_TYPE
		}

		// 7. �̺�Ʈ ����		
		{
			//{{ 2010. 05. 11  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
			kDungeonUnitInfo.m_nED		+= (int)(spRoomUser->GetRewardED() * spRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_ED ) );
			kDungeonUnitInfo.m_nEventEXP = (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * spRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_EXP ) );

			kDungeonUnitInfo.m_nEventEXP += (int)(kDungeonUnitInfo.m_nPartyBonusEXP * spRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_PARTY_EXP ) );
	#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
			kDungeonUnitInfo.m_nEventEXP += (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * spRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_WITH_CHAR_EXP ) );
	#endif // SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP

#else
			//kDungeonUnitInfo.m_nED		+= (int)(spRoomUser->GetRewardED() * m_fGameEvent[GEV_ED]);
			//kDungeonUnitInfo.m_nEventEXP = (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * m_fGameEvent[GEV_EXP]);

			//kDungeonUnitInfo.m_nEventEXP += (int)(kDungeonUnitInfo.m_nPartyBonusEXP * m_fGameEvent[GEV_PARTY_EXP]);
#endif SERV_INTEGRATION
			//}}
		}

		// 8. ä�κ��ʽ�
		{
			//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
//#ifdef SERV_ROOMUSERINFO_REFAC
			kDungeonUnitInfo.m_nChannelBonusEXP = (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * spRoomUser->GetBonusRate( KRoomUserInfo::BT_CHANNEL_BONUS_EXP_RATE ));
//#endif SERV_ROOMUSERINFO_REFAC
			//}}
		}

		// 9. �����̾� ���ʽ�
		//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
//#ifdef SERV_ROOMUSERINFO_REFAC
		float fAddPremiumExpRate = spRoomUser->GetBonusRate( KRoomUserInfo::BT_PREMIUM_EXP_RATE );
//#endif SERV_ROOMUSERINFO_REFAC
		//}}

		if( fAddPremiumExpRate > 0.0f )
		{
			kDungeonUnitInfo.m_nPremiumBonusEXP += (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * fAddPremiumExpRate);
		}

		// 10. ���� ����
		if( spRoomUser->GetLevel() >= SiKGameSysVal()->GetLimitsLevel() )
		{
			kDungeonUnitInfo.ResetEXP();
		}

		// 11. �������� üũ
		{
			if( kUpdateUnitInfo.m_bDungeonClear == true )
			{
				std::vector< UidType > vecTemp;
				std::vector< UidType >::const_iterator vitTemp;
				spRoomUser->GetTutorialUIDList( vecTemp );

				for( vitTemp = vecTemp.begin(); vitTemp != vecTemp.end(); ++vitTemp )
				{
					KRoomUserPtr spRU = spRoomUserManager->GetUser( *vitTemp );

					if( spRU != NULL )
					{
						kUpdateUnitInfo.m_bIsTutorialSuccess = true;
						break;
					}
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		//DB�� �� ���ֵ����� ó��
		//{{ 2009. 5. 8  ������		�ǽð� ����ġ ( ��ü���� - �⺻���� )
		kUpdateUnitInfo.m_iEXP	= kDungeonUnitInfo.SumEXP() - ( kDungeonUnitInfo.m_nEXP + kDungeonUnitInfo.m_nPartyBonusEXP ); // ���ʽ� ����ġ�� ����
		//}}
		kUpdateUnitInfo.m_iED	= kDungeonUnitInfo.m_nED - spRoomUser->GetRewardED(); // ���ʽ� ED�� ����
		
		// ������ ����
		spRoomUserManager->GetItemList( spRoomUser->GetCID(), kUpdateUnitInfo.m_mapGetItem );

		//client by result process
		kUnitInfoNot.m_vecDungeonUnitInfo.push_back( kDungeonUnitInfo );

		//db process
		vecUpdateUnitInfo.push_back( kUpdateUnitInfo );
	}

	//{{ 2011. 03. 08	��μ� ����ġ�� ȹ���� ������ ����Ʈ �Ϸ� ���� ����
#ifdef SERV_EXP_ACQUISITION_CONDITION_CHANGE
	kUnitInfoNot.m_mapHaveExpInDungeon = spRoomUserManager->GetHaveExpInDungeon();
#endif SERV_EXP_ACQUISITION_CONDITION_CHANGE
	//}}
	//{{ 2012. 12. 21  ���� ����Ʈ Ÿ�� �߰� (���� ���� ���� Ŭ����) - ��μ�
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	kUnitInfoNot.m_mapSuitableLevelInfo = spRoomUserManager->GetSuitableLevelInfo();
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	//}}
	return true;
}

//{{ 2011. 07. 11	������	���� ����
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////

bool KResultProcess::Result_TEAM_BREAK(	IN KRoomUserManagerPtr spRoomUserManager, IN KRoomUserPtr spRoomUser, IN const int iReason, IN const SEnum::MATCH_TYPE eMatchType, IN const SEnum::PVP_NPC_TYPE ePvpNpcType, OUT KERM_UPDATE_PVP_UNIT_INFO_NOT& kInfo )
{
	// ��Ż ���� �߿��� ũ������ ����
	kInfo.m_iEXP = 0;
	kInfo.m_iRating = 0;
	kInfo.m_iRPoint = 0;
	kInfo.m_iAPoint = 0;

	if( iReason == NetError::NOT_LEAVE_ROOM_REASON_23 )
		return true;

	//////////////////////////////////////////////////////////////////////////	
	// Ranking Point ���Ƽ

	// �ڱ� ��ũ���� �¸��� ���� �� �ִ� RP*5�� �������� �Ʒ��� ���� ����
	//= ( -12*���·� ) * 5       = ( -12*100% ) * 5 
	kInfo.m_iRPoint = SiKPvpMatchResultTable()->GetRPointPvpPenalty();


	//////////////////////////////////////////////////////////////////////////
	// Arena Point���Ƽ

	// �ڱ� ��ũ���� �¸��� ���� �� �ִ� AP*5��� ���� ( ��, kill=0, assist = 0 ���� )
	// ( 4 * int( rating * 0.01 + 0.5 ) ) * 5
	kInfo.m_iAPoint = SiKPvpMatchResultTable()->GetAPointPvpPenalty( spRoomUser->GetRating(), eMatchType, ePvpNpcType );
	return true;
}

bool KResultProcess::Result_TEAM_DEATH_BREAK( IN KRoomUserManagerPtr spRoomUserManager, IN KRoomUserPtr spRoomUser, IN const int iReason, IN const SEnum::MATCH_TYPE eMatchType, IN const SEnum::PVP_NPC_TYPE ePvpNpcType, OUT KERM_UPDATE_PVP_UNIT_INFO_NOT& kInfo )
{
	// ��Ż ���� �߿��� ũ������ ����
	kInfo.m_iEXP = 0;
	kInfo.m_iRating = 0;
	kInfo.m_iRPoint = 0;
	kInfo.m_iAPoint = 0;

	if( iReason == NetError::NOT_LEAVE_ROOM_REASON_23 )
		return true;

	//////////////////////////////////////////////////////////////////////////	
	// Ranking Point ���Ƽ

	// �ڱ� ��ũ���� �¸��� ���� �� �ִ� RP*5�� �������� �Ʒ��� ���� ����
	//= ( -12*���·� ) * 5       = ( -12*100% ) * 5 
	kInfo.m_iRPoint = SiKPvpMatchResultTable()->GetRPointPvpPenalty();


	//////////////////////////////////////////////////////////////////////////
	// Arena Point���Ƽ

	// �ڱ� ��ũ���� �¸��� ���� �� �ִ� AP*5��� ���� ( ��, kill=0, assist = 0 ���� )
	// ( 4 * int( rating * 0.01 + 0.5 ) ) * 5
	kInfo.m_iAPoint = SiKPvpMatchResultTable()->GetAPointPvpPenalty( spRoomUser->GetRating(), eMatchType, ePvpNpcType );
	return true;
}

bool KResultProcess::Result_SURVIVAL_BREAK( IN KRoomUserManagerPtr spRoomUserManager, IN KRoomUserPtr spRoomUser, IN const int iReason, IN const SEnum::MATCH_TYPE eMatchType, IN const SEnum::PVP_NPC_TYPE ePvpNpcType, OUT KERM_UPDATE_PVP_UNIT_INFO_NOT& kInfo )
{
	// ��Ż ���� �߿��� ũ������ ����
	kInfo.m_iEXP = 0;
	kInfo.m_iRating = 0;
	kInfo.m_iRPoint = 0;
	kInfo.m_iAPoint = 0;

	if( iReason == NetError::NOT_LEAVE_ROOM_REASON_23 )
		return true;

	//////////////////////////////////////////////////////////////////////////	
	// Ranking Point ���Ƽ

	// �ڱ� ��ũ���� �¸��� ���� �� �ִ� RP*5�� �������� �Ʒ��� ���� ����
	//= ( -12*���·� ) * 5       = ( -12*100% ) * 5 
	kInfo.m_iRPoint = SiKPvpMatchResultTable()->GetRPointPvpPenalty();


	//////////////////////////////////////////////////////////////////////////
	// Arena Point���Ƽ

	// �ڱ� ��ũ���� �¸��� ���� �� �ִ� AP*5��� ���� ( ��, kill=0, assist = 0 ���� )
	// ( 4 * int( rating * 0.01 + 0.5 ) ) * 5
	kInfo.m_iAPoint = SiKPvpMatchResultTable()->GetAPointPvpPenalty( spRoomUser->GetRating(), eMatchType, ePvpNpcType );
	return true;
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

bool KResultProcess::Result_TEAM_BREAK(	IN KRoomUserManagerPtr spRoomUserManager, IN KRoomUserPtr spRoomUser, OUT KERM_UPDATE_PVP_UNIT_INFO_NOT& kInfo )
{
	const KPVPResultTable::ResultData* pResultData = NULL; 
	pResultData = SiKPVPResultTable()->GetGameOutPenalty( spRoomUser->GetPVPEmblem() );/*(CXSLUnit::UNIT_CLASS)spRoomUser->GetUnitClass_LUA(), spRoomUser->GetLevel() );*/
	if( pResultData != NULL )
	{
		kInfo.m_iVSPoint	-= pResultData->m_VP;
	}
	else
	{
		kInfo.m_iVSPoint	= 0;
	}
	float fPlayTime = 0.0f;
	if( spRoomUserManager->GetPlayTime( spRoomUser->GetCID(), fPlayTime ) == true )
	{
		if( fPlayTime < 30.0f )
		{
			kInfo.m_iVSPoint	= 0;
		}
	}

	return true;
}

bool KResultProcess::Result_TEAM_DEATH_BREAK( IN KRoomUserManagerPtr spRoomUserManager, IN KRoomUserPtr spRoomUser, OUT KERM_UPDATE_PVP_UNIT_INFO_NOT& kInfo )
{
	const KPVPResultTable::ResultData* pResultData = NULL; 
	pResultData = SiKPVPResultTable()->GetGameOutPenalty( spRoomUser->GetPVPEmblem() );/*(CXSLUnit::UNIT_CLASS)spRoomUser->GetUnitClass_LUA(), spRoomUser->GetLevel() );*/
	if( pResultData != NULL )
	{
		kInfo.m_iVSPoint	-= pResultData->m_VP;
	}
	else
	{
		kInfo.m_iVSPoint	= 0;
	}
	float fPlayTime = 0.0f;
	if( spRoomUserManager->GetPlayTime( spRoomUser->GetCID(), fPlayTime ) == true )
	{
		if( fPlayTime < 30.0f )
		{
			kInfo.m_iVSPoint	= 0;
		}
	}

	return true;
}

bool KResultProcess::Result_SURVIVAL_BREAK( IN KRoomUserManagerPtr spRoomUserManager, IN KRoomUserPtr spRoomUser, OUT KERM_UPDATE_PVP_UNIT_INFO_NOT& kInfo )
{
	const KPVPResultTable::ResultData* pResultData = NULL; 
	pResultData = SiKPVPResultTable()->GetGameOutPenalty( spRoomUser->GetPVPEmblem() );/*(CXSLUnit::UNIT_CLASS)spRoomUser->GetUnitClass_LUA(), spRoomUser->GetLevel() );*/
	if( pResultData != NULL )
	{
		kInfo.m_iVSPoint	-= pResultData->m_VP;
	}
	else
	{
		kInfo.m_iVSPoint	= 0;
	}
	float fPlayTime = 0.0f;
	if( spRoomUserManager->GetPlayTime( spRoomUser->GetCID(), fPlayTime ) == true )
	{
		if( fPlayTime < 30.0f )
		{
			kInfo.m_iVSPoint	= 0;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}

bool KResultProcess::Result_DUNGEON_BREAK( IN KRoomUserPtr spRoomUser, OUT KERM_UPDATE_DUNGEON_UNIT_INFO_NOT& kInfo )
{
	//const KPVPResultTable::ResultData* pResultData = NULL; 
	//pResultData = SiKPVPResultTable()->GetGameOutPenalty( spRoomUser->GetPVPEmblem() );/*(CXSLUnit::UNIT_CLASS)spRoomUser->GetUnitClass_LUA(), spRoomUser->GetLevel() );*/
	//if( pResultData != NULL )
	//{
	//	kInfo.m_iED			= pResultData->m_ED;
	//}
	//else
	if( spRoomUser != NULL )
	{
		kInfo.m_iED		-= spRoomUser->GetLevel() * 100;
	}
	else
	{
		kInfo.m_iED		= 0;
	}

	return true;
}

//{{ 2011. 08. 03	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
int KResultProcess::DecidePVPWinTeam( IN const float fTotalPlayTime, IN const float fRemainingPlayTime, IN OUT KRoomUserManagerPtr spRoomUserManager, IN const CXSLRoom::PVP_GAME_TYPE eGameType, OUT KPvpMatchUserResultLog::END_TYPE& eEndType )
#else
int KResultProcess::DecidePVPWinTeam( IN const float fTotalPlayTime, IN const float fRemainingPlayTime, IN OUT KRoomUserManagerPtr spRoomUserManager, IN const CXSLRoom::PVP_GAME_TYPE eGameType )
#endif SERV_PVP_NEW_SYSTEM
//}}
{
	std::map<int, KTeamData>			mapTeamInfo;
	std::map<int, KTeamData>::iterator	mit;
	KTeamData		sTeamData;
	KRoomUserPtr	pkRoomUser;
	//{{ 2011. 08. 03	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	eEndType = KPvpMatchUserResultLog::ET_ETC;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	//��������� �˻��ϸ鼭 ����� �� ������ ������ ����
	for( int i = 0; i < spRoomUserManager->GetNumMember(); ++i )
	{
		pkRoomUser = spRoomUserManager->GetUser( i );

		if( !pkRoomUser )
		{
			START_LOG( cerr, L"�� ���� ������ �̻�." )
				<< BUILD_LOG( i )
				<< END_LOG;
		}

		if( pkRoomUser == NULL || pkRoomUser->GetStateID() != KRoomUserFSM::S_RESULT )
			continue;

		mit = mapTeamInfo.find( pkRoomUser->GetTeam() );
		if( mit == mapTeamInfo.end() )
		{
			sTeamData.Init();

			//sTeamData.m_nMaxAlive		+= pkRoomUser->GetNumKill();
			//{{ 2009. 2. 19  ������	�¸��� ���Ҷ� HP���� Ȯ���� ���
			//sTeamData.m_fMaxHP			+= pkRoomUser->GetHP();
			if( eGameType == CXSLRoom::PGT_TEAM_DEATH )
			{
				sTeamData.m_fMaxHP		+= ( pkRoomUser->IsDie() ? 100 : pkRoomUser->GetPercentHP() );
			}
			else
			{
				sTeamData.m_fMaxHP		+= pkRoomUser->GetPercentHP();
			}
			//}}
			sTeamData.m_nAverageLevel	+= pkRoomUser->GetLevel();
			sTeamData.m_nUnitNum++;

			mapTeamInfo.insert( std::make_pair( pkRoomUser->GetTeam(), sTeamData ) );
		}
		else
		{
			sTeamData = mit->second;

			//sTeamData.m_nMaxAlive		+= pkRoomUser->GetNumKill();
			//{{ 2009. 2. 19  ������	�¸��� ���Ҷ� HP���� Ȯ���� ���
			//sTeamData.m_fMaxHP			+= pkRoomUser->GetHP();
			if( eGameType == CXSLRoom::PGT_TEAM_DEATH )
			{
				sTeamData.m_fMaxHP		+= ( pkRoomUser->IsDie() ? 100 : pkRoomUser->GetPercentHP() );
			}
			else
			{
				sTeamData.m_fMaxHP		+= pkRoomUser->GetPercentHP();
			}
			//}}
			sTeamData.m_nAverageLevel	+= pkRoomUser->GetLevel();
			sTeamData.m_nUnitNum++;

			mit->second = sTeamData;
		}
	}

	//������ ������ ��� 
	// 1) ���º� ó��
	// 2) ������ �¸�ó��
	if( mapTeamInfo.size() <= 1 )
	{
		//���º�
		if( (fTotalPlayTime - fRemainingPlayTime) < SEnum::ET_PVP_DRAWN_TIME )
		{
			//{{ 2011. 08. 03	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
			eEndType = KPvpMatchUserResultLog::ET_EALRY_LEAVE;
#endif SERV_PVP_NEW_SYSTEM
			//}}
			return CXSLRoom::TN_NONE;
		}
		//������ �¸�
		else
		{
			mit = mapTeamInfo.begin();

			if( mit != mapTeamInfo.end() )
			{
				//{{ 2011. 08. 03	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
				eEndType = KPvpMatchUserResultLog::ET_LEAVE_ALL;
#endif SERV_PVP_NEW_SYSTEM
				//}}
				return mit->first;
			}
			else
			{
				START_LOG( cwarn, L"�¸��� �ĺ��� �������� ������ ����.!" )
					<< BUILD_LOG( spRoomUserManager->GetNumMember() )
					<< END_LOG;

				//{{ 2011. 08. 03	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
				eEndType = KPvpMatchUserResultLog::ET_LEAVE_ALL;
#endif SERV_PVP_NEW_SYSTEM
				//}}
				return CXSLRoom::TN_NONE;
			}
		}
	}

	for( mit = mapTeamInfo.begin(); mit != mapTeamInfo.end(); ++mit )
	{
		mit->second.m_nMaxAlive = spRoomUserManager->GetTeamScore( mit->first );
	}

	//�¸��� �ĺ�
	int nWinTeam = CXSLRoom::TN_NONE;
	sTeamData.Init();

	mit = mapTeamInfo.begin();
	if( mit != mapTeamInfo.end() )
	{
		mit->second.m_nAverageLevel = mit->second.m_nAverageLevel/mit->second.m_nUnitNum;
		sTeamData	= mit->second;
		nWinTeam	= mit->first;
		++mit;
	}

	for( ; mit != mapTeamInfo.end(); mit++ )
	{
		//��շ��� ���ϱ�
		mit->second.m_nAverageLevel = mit->second.m_nAverageLevel/mit->second.m_nUnitNum;

		//ù° : ų���� �������� �¸�
		if( mit->second.m_nMaxAlive > sTeamData.m_nMaxAlive )
		{
			sTeamData = mit->second;
			nWinTeam = mit->first;
		}
		else if( mit->second.m_nMaxAlive == sTeamData.m_nMaxAlive )
		{
			if( mit->second.m_fMaxHP > sTeamData.m_fMaxHP )
			{
				sTeamData = mit->second;
				nWinTeam = mit->first;
			}
			else if( mit->second.m_fMaxHP == sTeamData.m_fMaxHP )
			{
				//����� ������ ������ �¸�
				if( mit->second.m_nAverageLevel < sTeamData.m_nAverageLevel )
				{
					sTeamData = mit->second;
					nWinTeam = mit->first;
				}
			}
		}
	}

#ifdef PVP_BOSS_COMBAT_TEST

	if( eGameType == CXSLRoom::PGT_TEAM_DEATH )
	{
		bool bRedTeamBossDead = false;
		bool bBlueTeamBossDead = false;
		spRoomUserManager->IsAnyTeamBossDead( bRedTeamBossDead, bBlueTeamBossDead );
		
		if( true == bRedTeamBossDead )
		{
			nWinTeam = (int) CXSLRoom::TN_BLUE;
		}
		else
		{
			nWinTeam = (int) CXSLRoom::TN_RED;
		}
	}

#endif PVP_BOSS_COMBAT_TEST


	//{{ 2011. 08. 03	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	if( fRemainingPlayTime <= 0.f )
	{
		eEndType = KPvpMatchUserResultLog::ET_TIME_OUT;
	}
	else
	{
		eEndType = KPvpMatchUserResultLog::ET_ALL_KILL;
	}	
#endif SERV_PVP_NEW_SYSTEM
	//}}
	return nWinTeam;
}

void KResultProcess::AddDungeonPartyBonus_LUA( float fTwo, float fThree, float fFour )
{
	m_fPartyBonus[NUM_02]	= fTwo;
	m_fPartyBonus[NUM_03]	= fThree;
	m_fPartyBonus[NUM_04]	= fFour;

	START_LOG( cout2, L"�������� ��Ƽ ���ʽ� Ȯ��.!" )
		<< BUILD_LOG( m_fPartyBonus[NUM_02] )
		<< BUILD_LOG( m_fPartyBonus[NUM_03] )
		<< BUILD_LOG( m_fPartyBonus[NUM_04] )
		;
}

float KResultProcess::GetDungeonPartyBonusRate( int iNumber )
{
	if( iNumber >= NUM_02 && iNumber <= NUM_04 )
	{
		return m_fPartyBonus[iNumber];
	}

	START_LOG( cerr, L"�������� ��Ƽ �ο����� �߸��Ǿ���." )
		<< BUILD_LOG( iNumber )
		<< END_LOG;

	return 0.0f;
}

void KResultProcess::SortbyPVPMD(std::vector<KPVPUnitResultInfo>& vecData )
{
	//�ƽ��������� ��������
	for( int i = 0; i < (int)vecData.size(); i++ )
	{
		for( int j = 0; j < (int)vecData.size() - i - 1; j++ )
		{
			KPVPUnitResultInfo kUnitInfo1 = vecData[j];
			KPVPUnitResultInfo kUnitInfo2 = vecData[j+1];
			if( kUnitInfo1.m_nMDKillNum < kUnitInfo2.m_nMDKillNum )
			{
				vecData[j+1]	= kUnitInfo1;
				vecData[j]		= kUnitInfo2;
			}
		}
	}
}

void KResultProcess::SortbyPVPKill(std::vector<KPVPUnitResultInfo>& vecData )
{
	//ų���� ��������
	for( int i = 0; i < (int)vecData.size(); i++ )
	{
		for( int j = 0; j < (int)vecData.size() - i - 1; j++ )
		{
			KPVPUnitResultInfo kUnitInfo1 = vecData[j];
			KPVPUnitResultInfo kUnitInfo2 = vecData[j+1];
			if( kUnitInfo1.m_nKillNum < kUnitInfo2.m_nKillNum )
			{
				vecData[j+1]	= kUnitInfo1;
				vecData[j]		= kUnitInfo2;
			}
		}
	}
}

void KResultProcess::SortbyPVPHP(std::vector<KPVPUnitResultInfo>& vecData, KRoomUserManagerPtr spRoomUserManager, CXSLRoom::PVP_GAME_TYPE eGameType )
{
	int iUser1PerHP = 0;
	int iUser2PerHP = 0;

	//�ƽ������� & ų���� ���� ��� HP�� ��������
	for( int i = 0; i < (int)vecData.size(); i++ )
	{
		for( int j = 0; j < (int)vecData.size() - i - 1; j++ )
		{
			KPVPUnitResultInfo kUnitInfo1 = vecData[j];
			KPVPUnitResultInfo kUnitInfo2 = vecData[j+1];
			if( kUnitInfo1.m_nKillNum == kUnitInfo2.m_nKillNum &&
				kUnitInfo1.m_nMDKillNum == kUnitInfo2.m_nMDKillNum )
			{
				KRoomUserPtr kRoomUser1 = spRoomUserManager->GetUser( kUnitInfo1.m_UnitUID );
				KRoomUserPtr kRoomUser2 = spRoomUserManager->GetUser( kUnitInfo2.m_UnitUID );

				//{{ 2009. 2. 25  ������	���� ������ ��� hp�� 100%�� ���
				if( eGameType == CXSLRoom::PGT_TEAM )
				{
					iUser1PerHP = kRoomUser1->GetPercentHP();
					iUser2PerHP = kRoomUser2->GetPercentHP();
				}
				else
				{
					iUser1PerHP = ( kRoomUser1->IsDie() ? 100 : kRoomUser1->GetPercentHP() );
					iUser2PerHP = ( kRoomUser2->IsDie() ? 100 : kRoomUser2->GetPercentHP() );
				}

				if( iUser1PerHP < iUser2PerHP )
				{
					vecData[j+1]	= kUnitInfo1;
					vecData[j]		= kUnitInfo2;
				}
				//}}
			}
		}
	}
}

void KResultProcess::SortbyPVPLevel(std::vector<KPVPUnitResultInfo>& vecData, KRoomUserManagerPtr spRoomUserManager, CXSLRoom::PVP_GAME_TYPE eGameType )
{
	int iUser1PerHP = 0;
	int iUser2PerHP = 0;

	// the same rank and level
	for( int i = 0; i < (int)vecData.size(); i++ )
	{
		for( int j = 0; j < (int)vecData.size() - i - 1; j++ )
		{
			KPVPUnitResultInfo kUnitInfo1 = vecData[j];
			KPVPUnitResultInfo kUnitInfo2 = vecData[j+1];
			if( kUnitInfo1.m_nKillNum == kUnitInfo2.m_nKillNum &&
				kUnitInfo1.m_nMDKillNum == kUnitInfo2.m_nMDKillNum )
			{
				KRoomUserPtr kRoomUser1 = spRoomUserManager->GetUser( kUnitInfo1.m_UnitUID );
				KRoomUserPtr kRoomUser2 = spRoomUserManager->GetUser( kUnitInfo2.m_UnitUID );

				//{{ 2009. 3. 2  ������		���� ������ ��� hp�� 100%�� ���
				if( eGameType == CXSLRoom::PGT_TEAM )
				{
					iUser1PerHP = kRoomUser1->GetPercentHP();
					iUser2PerHP = kRoomUser2->GetPercentHP();
				}
				else
				{
					iUser1PerHP = ( kRoomUser1->IsDie() ? 100 : kRoomUser1->GetPercentHP() );
					iUser2PerHP = ( kRoomUser2->IsDie() ? 100 : kRoomUser2->GetPercentHP() );
				}

				if ( iUser1PerHP == iUser2PerHP )
				{
					if ( kRoomUser1->GetLevel() > kRoomUser2->GetLevel() )
					{
						vecData[j+1]	= kUnitInfo1;
						vecData[j]		= kUnitInfo2;
					}
				}
				//}}
			}
		}
	}
}

void KResultProcess::SortbyPVPSlot(std::vector<KPVPUnitResultInfo>& vecData, KRoomUserManagerPtr spRoomUserManager, CXSLRoom::PVP_GAME_TYPE eGameType )
{
	int iUser1PerHP = 0;
	int iUser2PerHP = 0;

	// and slotid..?
	for( int i = 0; i < (int)vecData.size(); i++ )
	{
		for( int j = 0; j < (int)vecData.size() - i - 1; j++ )
		{
			KPVPUnitResultInfo kUnitInfo1 = vecData[j];
			KPVPUnitResultInfo kUnitInfo2 = vecData[j+1];
			if( kUnitInfo1.m_nKillNum == kUnitInfo2.m_nKillNum &&
				kUnitInfo1.m_nMDKillNum == kUnitInfo2.m_nMDKillNum )
			{
				KRoomUserPtr kRoomUser1 = spRoomUserManager->GetUser( kUnitInfo1.m_UnitUID );
				KRoomUserPtr kRoomUser2 = spRoomUserManager->GetUser( kUnitInfo2.m_UnitUID );

				//{{ 2009. 3. 2  ������		���� ������ ��� hp�� 100%�� ���
				if( eGameType == CXSLRoom::PGT_TEAM )
				{
					iUser1PerHP = kRoomUser1->GetPercentHP();
					iUser2PerHP = kRoomUser2->GetPercentHP();
				}
				else
				{
					iUser1PerHP = ( kRoomUser1->IsDie() ? 100 : kRoomUser1->GetPercentHP() );
					iUser2PerHP = ( kRoomUser2->IsDie() ? 100 : kRoomUser2->GetPercentHP() );
				}

				if ( iUser1PerHP == iUser2PerHP )
				{
					if ( kRoomUser1->GetLevel() == kRoomUser2->GetLevel() )
					{
						if ( kRoomUser1->GetSlotID() > kRoomUser2->GetSlotID() )
						{
							vecData[j+1]	= kUnitInfo1;
							vecData[j]		= kUnitInfo2;
						}
					}
				}
				//}}
			}
		}
	}
}

//{{ 2012. 12. 17	������	�̺�Ʈ ���ʽ� �ڵ� �����丵
//void KResultProcess::SetGameEvent( int iDungeonID, int iUnitLevel, char cUnitClass, int iPartyNum ) -- �Ⱦ��� �ڵ�
//{
//	SiKGameEventManager()->GetRewardEvent( m_fGameEvent[GEV_EXP], 
//										   m_fGameEvent[GEV_VP], 
//										   m_fGameEvent[GEV_ED], 
//										   m_fGameEvent[GEV_PARTY_EXP],
//										   m_fGameEvent[GEV_PARTY_VP],
//										   m_fGameEvent[GEV_PARTY_ED], 
//										   iDungeonID, 
//										   iUnitLevel, 
//										   cUnitClass, 
//										   iPartyNum );
//
//	//if( iPartyNum > 1 )
//	//	SiKGameEventManager()->GetRewardPartyEvent( m_fGameEvent[GEV_PARTY_EXP], m_fGameEvent[GEV_PARTY_VP], m_fGameEvent[GEV_PARTY_ED], iPartyNum );
//	//else
//	//{
//	//	m_fGameEvent[GEV_PARTY_EXP] = 0.0f;
//	//	m_fGameEvent[GEV_PARTY_VP] = 0.0f;
//	//	m_fGameEvent[GEV_PARTY_ED] = 0.0f;
//	//}
//}
//}}


//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
bool KResultProcess::Result_DefenceDUNGEON( IN OUT KRoomUserManagerPtr spRoomUserManager,
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
											IN int iProtectedCrystalCount )
{
	//�������� �� ��������� �����Ѵ�..
	kUnitInfoNot.m_bIsWin		= bIsWin;
	kUnitInfoNot.m_nSec			= nTotalPlayTime;
	kUnitInfoNot.m_iDungeonID	= iDungeonID;
	kUnitInfoNot.m_cDifficulty	= cDifficulty;
	//{{ 2010. 05. 01  ������	��д��� ����
#ifdef SERV_SECRET_HELL
	kUnitInfoNot.m_cDungeonMode	= cDungeonMode;
#endif SERV_SECRET_HELL
	//}}

	kUnitInfoNot.m_bIsClearDefenceDungeon = false;
	kUnitInfoNot.m_iProtectedSmallCrystal= 0;

	//������ ���� ����ó���� ������ ��찡 �־� �߰���..
	const int iEndNumMember = spRoomUserManager->GetNumMember();
	if( iEndNumMember <= 0 )
	{
		START_LOG( cerr, L"���� ������ ���µ� ���ó���� �����.!" )
			<< BUILD_LOG( iEndNumMember )
			<< END_LOG;

		return false;
	}

	//�� UNIT �� ���ó�� ����...........

	//1. mvp ����
	bool			bIsSelectMVP = false;
	KRoomUserPtr	pkMVPRoomuser;

	//{{ 2013. 03. 05	���� MVP ���� ���� ���� - ��μ�
#ifdef SERV_DUNGEON_MVP
	// 3�� ��Ƽ �̻󿡼� MVP�� �����Ѵ�.
	if( spRoomUserManager->GetNumMember() >= 3 )
	{
		bIsSelectMVP = true;

		int iTotalDamage = 0;

		for( int idx = 0; idx < spRoomUserManager->GetNumMember(); ++idx )
		{
			KRoomUserPtr pkRoomUser = spRoomUserManager->GetUser( idx );
			if( pkRoomUser == NULL )
				continue;

			if( bIsSelectMVP == true && iTotalDamage < pkRoomUser->GetTotalGivenDamamge_LUA() )
			{
				pkMVPRoomuser	= pkRoomUser;
				iTotalDamage	= pkRoomUser->GetTotalGivenDamamge_LUA();
			}
		}
	}
#else
	//Ǯ��Ƽ �ϰ�츸 MVP�� �����Ѵ�.
	if( spRoomUserManager->GetNumMember() == spRoomUserManager->GetNumTotalSlot() )
	{
		bIsSelectMVP = true;

		int iMaxKill = 0;

		for( int idx = 0; idx < spRoomUserManager->GetNumMember(); ++idx )
		{
			KRoomUserPtr pkRoomUser = spRoomUserManager->GetUser( idx );
			if( pkRoomUser == NULL )
				continue;

			if( bIsSelectMVP == true && iMaxKill < pkRoomUser->GetKillNPC_LUA() )
			{
				pkMVPRoomuser = pkRoomUser;
				iMaxKill	  = pkRoomUser->GetKillNPC_LUA();
			}
		}
	}
#endif SERV_DUNGEON_MVP
	//}

	//2. �� UNIT �� �ð���ũ ��꿡 ���� ��� ���� / ų�� ����ϱ�
	int nAverageLevel		= 0;
	int	nTotalKillNPCNum	= 0;
	int iMaxKill = 0;
	//{{ 2011. 02. 22	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
	int iPartyTotalGivenDamage = 0;
#endif SERV_DUNGEON_RANK_NEW
	//}}
	for( int idx = 0; idx < spRoomUserManager->GetNumMember(); ++idx )
	{
		KRoomUserPtr pkRoomUser  = spRoomUserManager->GetUser( idx );
		_JIF( pkRoomUser != NULL, continue; );

		nAverageLevel			+= pkRoomUser->GetLevel();
		nTotalKillNPCNum		+= pkRoomUser->GetKillNPC_LUA();
		//{{ 2011. 02. 22	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
		iPartyTotalGivenDamage	+= pkRoomUser->GetTotalGivenDamamge_LUA();
#endif SERV_DUNGEON_RANK_NEW
		//}}

		// ������ �������� ī��Ʈ ���ϱ�
		if( iClearStageCount < pkRoomUser->GetPassedStageCount() )
			iClearStageCount = pkRoomUser->GetPassedStageCount();
	}

	// ��շ����� ���� ������ ��� ���Ѵ�.
	nAverageLevel = nAverageLevel / spRoomUserManager->GetNumMember(); // ������ 0���� üũ�մϴ�

	//3. �� UNIT�� ������� ����..
	for( int i = 0; i < spRoomUserManager->GetNumMember(); ++i )
	{
		KDungeonUnitResultInfo					kDungeonUnitInfo;
		KERM_UPDATE_DUNGEON_UNIT_INFO_NOT		kUpdateUnitInfo;

		KRoomUserPtr spRoomUser = spRoomUserManager->GetUser( i );
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"������ ������ �̻�.!" )
				<< BUILD_LOG( i )
				<< END_LOG;
			continue;
		}

		//{{ 2010. 05. 11  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION

#else
		SetGameEvent( iDungeonID, spRoomUser->GetLevel(), spRoomUser->GetUnitClass_LUA(), spRoomUserManager->GetLiveMember() );
#endif SERV_INTEGRATION
		//}}

		vecKRoomUserPtr.push_back( spRoomUser );

		//3.1 �÷��� �⺻��� ����..
		kDungeonUnitInfo.m_bIsMVP			= false;
		kDungeonUnitInfo.m_UnitUID			= spRoomUser->GetCID();
		kDungeonUnitInfo.m_nTotalScore		= spRoomUser->GetTotalScore();
		kDungeonUnitInfo.m_nComboScore		= spRoomUser->GetComboScore_LUA();
		kDungeonUnitInfo.m_nTechnicalScore	= spRoomUser->GetTechnicalScore_LUA();
		kDungeonUnitInfo.m_nDamaged			= spRoomUser->GetDamageNum_LUA();
		kDungeonUnitInfo.m_nKillNPCNum		= spRoomUser->GetKillNPC_LUA();
		kDungeonUnitInfo.m_bIsDie			= spRoomUser->IsDie();
		//{{ 2009. 6. 10  ������	���� ���۽� ����
		kDungeonUnitInfo.m_nOldEXP			= spRoomUser->GetOldEXP();
		kDungeonUnitInfo.m_nOldED			= spRoomUser->GetOldED();
		//}}		

		//3.2 ����Ŭ���� ��������
		kUpdateUnitInfo.m_bDungeonClear	= bIsWin;
		kUpdateUnitInfo.m_iMaxScore		= kDungeonUnitInfo.m_nTotalScore;		//����Ŭ���� ������ �ְ������� ����ϱ� ����
		kUpdateUnitInfo.m_iDungeonID	= iDungeonID + static_cast<int>(cDifficulty);
		kUpdateUnitInfo.m_iUserCount	= spRoomUserManager->GetNumMember();
		kUpdateUnitInfo.m_iPlayTime		= nTotalPlayTime; // ���� ��踦 ���� �÷��� Ÿ�� ���
		kUpdateUnitInfo.m_iDropItemUID	= nDropItemUID; // �ӽ� �κ��丮�� ���� DropItemUID
		//{{ 2012. 04. 16	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		kUpdateUnitInfo.m_bStartedByAutoParty = bStartedByAutoParty;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		//{{ 2010. 10. 26	������	����Ʈ ���� �߰�
#ifdef SERV_QUEST_CLEAR_EXPAND
		std::map< int, KSubQuestInfo > mapOngoingQuest;
		spRoomUser->GetOngoingQuest( CXSLQuestManager::SQT_ITEM_EQUIP_DUNGEON_CLEAR, mapOngoingQuest );
		kUnitInfoNot.m_mapOngoingQuest.insert( std::make_pair( spRoomUser->GetCID(), mapOngoingQuest ) );
#endif SERV_QUEST_CLEAR_EXPAND
		//}}

		//{{ 2011. 03. 08	��μ� ����ġ�� ȹ���� ������ ����Ʈ �Ϸ� ���� ����
#ifdef SERV_EXP_ACQUISITION_CONDITION_CHANGE
		spRoomUserManager->AddHaveExpInDungeon( spRoomUser->GetCID(), bIsWin );
#endif SERV_EXP_ACQUISITION_CONDITION_CHANGE
		//}}

		// ���潺 ���� �й� ���� ��
		if( bIsWin == false )
		{
			//�װ����� ������� �� ����ó���� db���� ���ְ� ���â���� �������� �ʴ´�.			
			kDungeonUnitInfo.m_cTotalRank			= CXSLDungeon::RT_F;
			kDungeonUnitInfo.m_cComboRank			= CXSLDungeon::RT_F;
			kDungeonUnitInfo.m_cDamagedRank			= CXSLDungeon::RT_F;
			kDungeonUnitInfo.m_cTechnicalRank		= CXSLDungeon::RT_F;
			kDungeonUnitInfo.m_cTimeRank			= CXSLDungeon::RT_F;

			kUpdateUnitInfo.m_bDungeonClear			= false;	//��������� ��������͵� Ŭ�����Ѱ� �ƴϴ�.
			kUpdateUnitInfo.m_cMaxTotalRank			= kDungeonUnitInfo.m_cTotalRank;

			//////////////////////////////////////////////////////////////////////////
			// ����ġ ����

			// 1. �⺻ ����ġ
			kDungeonUnitInfo.m_nEXP				= 0;

			// 2. ��Ƽ ���ʽ�
			kDungeonUnitInfo.m_nPartyBonusEXP	= 0;

			// 3. ���ӹ� ���ʽ�
			kDungeonUnitInfo.m_nGameBangBonusEXP	= 0;

			// 4. �̺�Ʈ ���� ����.!
			kDungeonUnitInfo.m_nEventEXP = 0;

			// 5. ä�κ��ʽ�
			kDungeonUnitInfo.m_nChannelBonusEXP = 0;


			// 6. �����̾� ����
			float fAddPremiumExpRate = spRoomUser->GetBonusRate( KRoomUserInfo::BT_PREMIUM_EXP_RATE );
			if( fAddPremiumExpRate > 0.0f )
			{
				kDungeonUnitInfo.m_nPremiumBonusEXP += (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * fAddPremiumExpRate);
			}

			// 7. ���� ����
			if( spRoomUser->GetLevel() >= SiKGameSysVal()->GetLimitsLevel() )
			{
				kDungeonUnitInfo.ResetEXP();
			}

			//db process
			//{{ 2009. 5. 8  ������		�ǽð� ����ġ
			kUpdateUnitInfo.m_iEXP = 0;
			//}}

			// ������ ����
			spRoomUserManager->GetItemList( spRoomUser->GetCID(), kUpdateUnitInfo.m_mapGetItem );

			kUnitInfoNot.m_vecDungeonUnitInfo.push_back( kDungeonUnitInfo );

			//db�� ������Ʈ �� ������
			vecUpdateUnitInfo.push_back( kUpdateUnitInfo );
			continue;
		}
		//////////////////////////////////////////////////////////////////////////
		// ���潺 ���� �¸� ���� ��
		//////////////////////////////////////////////////////////////////////////

		//{{ 2012. 03. 15	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
		// �ڵ� ��Ƽ ���ʽ�
		if( bIsWin  &&  spRoomUser->IsActivatedAutoPartyBonus() )
		{
			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( CXSLItem::RI_AUTO_PARTY_BONUS );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"������ ������ �������� �ʽ��ϴ�! ���� ���� ����!" )
					<< BUILD_LOG( CXSLItem::RI_AUTO_PARTY_BONUS )
					<< END_LOG;
			}
			else
			{
				KItemInfo kResultItemInfo;
				kResultItemInfo.m_iItemID	 = CXSLItem::RI_AUTO_PARTY_BONUS;
				kResultItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
				kResultItemInfo.m_iQuantity	 = 1;
				kResultItemInfo.m_sEndurance = pItemTemplet->m_Endurance;
				kUpdateUnitInfo.m_mapResultItem.insert( std::make_pair( kResultItemInfo.m_iItemID, kResultItemInfo ) );

				std::map< int, KItemInfo >::const_iterator mitHR;
				for( mitHR = kUpdateUnitInfo.m_mapResultItem.begin(); mitHR != kUpdateUnitInfo.m_mapResultItem.end(); ++mitHR )
				{
					spRoomUser->AddItem( mitHR->second.m_iItemID, mitHR->second.m_iQuantity );
				}
			}
		}
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		//{{ 2011. 04. 27	������	���潺 ���� �̺�Ʈ ���� ( 2011�� 5�� 12�� ���� )
#ifdef SERV_DEFENCE_DUNGEON_EVENT_REWARD
		{
			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( CXSLItem::DDR_DRAGON_PIECE );
			if( pItemTemplet == NULL )
			{
				START_LOG( cerr, L"������ ������ �������� �ʽ��ϴ�! ���� ���� ����!" )
					<< BUILD_LOG( CXSLItem::DDR_DRAGON_PIECE )
					<< END_LOG;
			}
			else
			{
				KItemInfo kResultItemInfo;
				kResultItemInfo.m_iItemID	 = CXSLItem::DDR_DRAGON_PIECE;
				kResultItemInfo.m_cUsageType = pItemTemplet->m_PeriodType;
				kResultItemInfo.m_iQuantity	 = 1;
				kResultItemInfo.m_sEndurance = pItemTemplet->m_Endurance;
				kUpdateUnitInfo.m_mapResultItem.insert( std::make_pair( kResultItemInfo.m_iItemID, kResultItemInfo ) );

				std::map< int, KItemInfo >::const_iterator mitHR;
				for( mitHR = kUpdateUnitInfo.m_mapResultItem.begin(); mitHR != kUpdateUnitInfo.m_mapResultItem.end(); ++mitHR )
				{
					spRoomUser->AddItem( mitHR->second.m_iItemID, mitHR->second.m_iQuantity );
				}
			}
		}
#endif SERV_DEFENCE_DUNGEON_EVENT_REWARD
		//}}

		//{{ 2011. 05. 04  ��μ�	���� Ŭ����� ������ ���� ���� �߰�
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		if( bIsWin == true )
		{
			bool bGetExp = spRoomUserManager->IsRoomUserGetExp( spRoomUser->GetCID() );
			SiKDropTable()->CheckDungeonClearPaymentItem( iDungeonID, cDifficulty, bGetExp, spRoomUser,
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
				spRoomUserManager,
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
				kUpdateUnitInfo.m_mapResultItem );
		}
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		//}}
	
		//////////////////////////////////////////////////////////////////////////
		// ��ũ ���ó��
		//CXSLDungeon::RANK_TYPE	eCombo, eTechnical, eDamage, eTime;
		//float���� exp�� �߰��� ���� %
		float					fComboBonus, fTechBonus, fDamageBonus, fTimeBonus;
		//////////////////////////////////////////////////////////////////////////		
		//COMBO RESULT
		{
			//{{ 2011. 01. 21	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
			lua_tinker::call<double>( GetLuaState(), "DUNGEON_COMBO_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficulty), &kDungeonMonsterManager, iEndNumMember, iPartyTotalGivenDamage );
#else
			lua_tinker::call<double>( GetLuaState(), "DUNGEON_COMBO_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficulty), iStartedNumMember, nTotalKillNPCNum );
#endif SERV_DUNGEON_RANK_NEW
			//}}

			fComboBonus	= spRoomUser->GetPercent();
			kDungeonUnitInfo.m_cComboRank	= (CXSLDungeon::RANK_TYPE)spRoomUser->GetRank();
		}

		//////////////////////////////////////////////////////////////////////////
		//TECHNICAL RESULT
		{
			//{{ 2011. 01. 21	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
			lua_tinker::call<double>( GetLuaState(), "DUNGEON_TECHNICAL_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficulty), &kDungeonMonsterManager, iEndNumMember, iPartyTotalGivenDamage );
#else
			lua_tinker::call<double>( GetLuaState(), "DUNGEON_TECHNICAL_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficulty), iStartedNumMember, nTotalKillNPCNum );
#endif SERV_DUNGEON_RANK_NEW
			//}}

			fTechBonus	= spRoomUser->GetPercent();
			kDungeonUnitInfo.m_cTechnicalRank = (CXSLDungeon::RANK_TYPE)spRoomUser->GetRank();
		}

		//////////////////////////////////////////////////////////////////////////		
		//DAMAGE RESULT
		{
			//{{ 2011. 01. 21	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
			lua_tinker::call<double>( GetLuaState(), "DUNGEON_DAMAGE_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficulty), &kDungeonMonsterManager );
#else
			lua_tinker::call<double>( GetLuaState(), "DUNGEON_DAMAGE_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficulty) );
#endif SERV_DUNGEON_RANK_NEW
			//}}			

			fDamageBonus	= spRoomUser->GetPercent();
			kDungeonUnitInfo.m_cDamagedRank	= (CXSLDungeon::RANK_TYPE)spRoomUser->GetRank();
		}

		//////////////////////////////////////////////////////////////////////////		
		//TIME RESULT
		{
			//{{ 2011. 01. 20	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
			(CXSLDungeon::RANK_TYPE)lua_tinker::call<int>( GetLuaState(), "DUNGEON_TIME_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficulty), &kDungeonMonsterManager, kUnitInfoNot.m_nSec );
#else
			(CXSLDungeon::RANK_TYPE)lua_tinker::call<int>( GetLuaState(), "DUNGEON_TIME_RESULT", spRoomUser.get(), nTotalKillNPCNum, nAverageLevel, kUnitInfoNot.m_nSec, (int)(iDungeonID + cDifficulty) );
#endif SERV_DUNGEON_RANK_NEW
			//}}

			fTimeBonus	= spRoomUser->GetPercent();
			kDungeonUnitInfo.m_cTimeRank	= (CXSLDungeon::RANK_TYPE)spRoomUser->GetRank();
		}

		//////////////////////////////////////////////////////////////////////////		
		//::: RANK AVERAGE :::
		{
			//{{ 2011. 02. 22	������	���� ��ũ ����
#ifdef SERV_DUNGEON_RANK_NEW
			kDungeonUnitInfo.m_cTotalRank = (char)lua_tinker::call<int>( GetLuaState(), "DUNGEON_TOTAL_RANK_RESULT", kDungeonUnitInfo.m_cComboRank, kDungeonUnitInfo.m_cTechnicalRank, kDungeonUnitInfo.m_cDamagedRank, kDungeonUnitInfo.m_cTimeRank, &kDungeonMonsterManager );

			if( (char)CXSLDungeon::RT_NONE >= kDungeonUnitInfo.m_cTotalRank )
			{
				kDungeonUnitInfo.m_cTotalRank = CXSLDungeon::RT_F;
			}
			else if( (char)CXSLDungeon::RT_SS < kDungeonUnitInfo.m_cTotalRank )
			{
				kDungeonUnitInfo.m_cTotalRank = CXSLDungeon::RT_SS;
			}
#else
			kDungeonUnitInfo.m_cTotalRank = (char)2*kDungeonUnitInfo.m_cComboRank + (char)2*kDungeonUnitInfo.m_cTechnicalRank + (char)kDungeonUnitInfo.m_cDamagedRank + (char)kDungeonUnitInfo.m_cTimeRank;
			kDungeonUnitInfo.m_cTotalRank = kDungeonUnitInfo.m_cTotalRank/6;

			if( (char)CXSLDungeon::RT_NONE >= kDungeonUnitInfo.m_cTotalRank )
			{
				kDungeonUnitInfo.m_cTotalRank = CXSLDungeon::RT_F;
			}
			else if( (char)CXSLDungeon::RT_S < kDungeonUnitInfo.m_cTotalRank )
			{
				kDungeonUnitInfo.m_cTotalRank = CXSLDungeon::RT_S;
			}
#endif SERV_DUNGEON_RANK_NEW
			//}}

			//����Ŭ���� ������ ����
			kUpdateUnitInfo.m_cMaxTotalRank			= kDungeonUnitInfo.m_cTotalRank;
		}

		//////////////////////////////////////////////////////////////////////////		
		//Rank Bonus
		//kDungeonUnitInfo.m_fRankBonus	= ( fComboBonus + fTechBonus + fDamageBonus + fTimeBonus );
		float fRankBonusRate = ( fComboBonus + fTechBonus + fDamageBonus + fTimeBonus );
		if( fRankBonusRate > 0.15f )
			fRankBonusRate = 0.15f;

		//////////////////////////////////////////////////////////////////////////
		// ����ġ �� ED����		
		// 1. �⺻ ����
		kDungeonUnitInfo.m_nED	= spRoomUser->GetRewardED();
		//{{ 2012. 03. 03	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		switch( cDifficulty )
		{
		/// itexpertkim // ����ö // 2014-01-14 
		/// ��ȹ��(�����)�� ��û�� ���� ���� ����
		/// �븻 ��忡�� �ͽ���Ʈ ��忡���� ����ġ ������ ����ϵ��� ����
		/// ����, ���� ����� ���� �븻 ��� �ۿ� ����
// 		case CXSLDungeon::DL_NORMAL:	kDungeonUnitInfo.m_nEXP	= static_cast<int>( SiKDropTable()->GetNpcExp( spRoomUser->GetLevel() ) * 47 * 1.5f );	break;
// 		case CXSLDungeon::DL_HARD:		kDungeonUnitInfo.m_nEXP	= static_cast<int>( SiKDropTable()->GetNpcExp( spRoomUser->GetLevel() ) * 47 * 1.8f );	break;
// 		case CXSLDungeon::DL_EXPERT:	kDungeonUnitInfo.m_nEXP	= static_cast<int>( SiKDropTable()->GetNpcExp( spRoomUser->GetLevel() ) * 47 * 3.0f );	break;
		case CXSLDungeon::DL_NORMAL:
		case CXSLDungeon::DL_HARD:
		case CXSLDungeon::DL_EXPERT:
			kDungeonUnitInfo.m_nEXP	= static_cast<int>( SiKDropTable()->GetNpcExp( spRoomUser->GetLevel() ) * 47 * 3.0f );
			break;
		}
#else
		kDungeonUnitInfo.m_nEXP	= SiKDropTable()->GetNpcExp( spRoomUser->GetLevel() ) * 47 * iProtectedCrystalCount;
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}

		// 2. ��Ƽ ���ʽ�.
		kDungeonUnitInfo.m_nPartyBonusEXP = spRoomUser->GetRewardPartyEXP();

		// 3. ���� Ŭ���� ���ʽ�
		//{{ 2012. 04. 24	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		kDungeonUnitInfo.m_nClearBonusEXP = static_cast<int>( ( kDungeonUnitInfo.m_nEXP + kDungeonUnitInfo.m_nPartyBonusEXP ) * 0.3f );
		kDungeonUnitInfo.m_nClearBonusEXP += static_cast<int>( kDungeonUnitInfo.m_nEXP * spRoomUser->GetBonusRate( KRoomUserInfo::BT_SOCKET_OPT_EXP_RATE ) );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}

		// 4. ��ũ ���ʽ�
		kDungeonUnitInfo.m_nRankBonusEXP = static_cast<int>(( kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP ) * fRankBonusRate);

		// 5. MVP ���ʽ�
		if( bIsSelectMVP == true && pkMVPRoomuser == spRoomUser )
		{
			kDungeonUnitInfo.m_nMvpBonusEXP += static_cast<int>((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * 0.1f);
			kDungeonUnitInfo.m_bIsMVP = true;
		}

		// 6. ���ӹ� ���ʽ�
		if( spRoomUser->IsGameBang() == true )
		{
#ifdef SERV_PC_BANG_TYPE
			kDungeonUnitInfo.m_nED					+= (int)(spRoomUser->GetRewardED() * SiKGameSysVal()->GetGBEDRate( spRoomUser->GetPcBangType() ));
			kDungeonUnitInfo.m_nGameBangBonusEXP	 = (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * SiKGameSysVal()->GetGBEXPRate( spRoomUser->GetPcBangType() ));
#else SERV_PC_BANG_TYPE
			kDungeonUnitInfo.m_nED					+= (int)(spRoomUser->GetRewardED() * SiKGameSysVal()->GetGBEDRate());
			kDungeonUnitInfo.m_nGameBangBonusEXP	 = (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * SiKGameSysVal()->GetGBEXPRate());
#endif SERV_PC_BANG_TYPE
		}

		// 7. �̺�Ʈ ����		
		{
			//{{ 2010. 05. 11  ������	���� ���� ������ ����
#ifdef SERV_INTEGRATION
			kDungeonUnitInfo.m_nED		+= (int)(spRoomUser->GetRewardED() * spRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_ED ) );
			kDungeonUnitInfo.m_nEventEXP = (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * spRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_EXP ) );

			kDungeonUnitInfo.m_nEventEXP += (int)(kDungeonUnitInfo.m_nPartyBonusEXP * spRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_PARTY_EXP ) );
	#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
			kDungeonUnitInfo.m_nEventEXP += (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * spRoomUser->GetBonusRate( KRoomUserInfo::BT_GEV_WITH_CHAR_EXP ) );
	#endif // SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
#else
			//kDungeonUnitInfo.m_nED		+= (int)(spRoomUser->GetRewardED() * m_fGameEvent[GEV_ED]);
			//kDungeonUnitInfo.m_nEventEXP = (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * m_fGameEvent[GEV_EXP]);

			//kDungeonUnitInfo.m_nEventEXP += (int)(kDungeonUnitInfo.m_nPartyBonusEXP * m_fGameEvent[GEV_PARTY_EXP]);
#endif SERV_INTEGRATION
			//}}
		}

		// 8. ä�κ��ʽ�
		{
			//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
			//#ifdef SERV_ROOMUSERINFO_REFAC
			kDungeonUnitInfo.m_nChannelBonusEXP = (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * spRoomUser->GetBonusRate( KRoomUserInfo::BT_CHANNEL_BONUS_EXP_RATE ));
			//#endif SERV_ROOMUSERINFO_REFAC
			//}}
		}

		// 9. �����̾� ���ʽ�
		//{{ 2010. 05. 11  ������	�̺�Ʈ ���� �ڵ� ����
		//#ifdef SERV_ROOMUSERINFO_REFAC
		float fAddPremiumExpRate = spRoomUser->GetBonusRate( KRoomUserInfo::BT_PREMIUM_EXP_RATE );
		//#endif SERV_ROOMUSERINFO_REFAC
		//}}

		if( fAddPremiumExpRate > 0.0f )
		{
			kDungeonUnitInfo.m_nPremiumBonusEXP += (int)((kDungeonUnitInfo.m_nEXP+kDungeonUnitInfo.m_nPartyBonusEXP) * fAddPremiumExpRate);
		}

		// 10. ���� ����
		if( spRoomUser->GetLevel() >= SiKGameSysVal()->GetLimitsLevel() )
		{
			kDungeonUnitInfo.ResetEXP();
		}

		// 11. �������� üũ
		{
			if( kUpdateUnitInfo.m_bDungeonClear == true )
			{
				std::vector< UidType > vecTemp;
				std::vector< UidType >::const_iterator vitTemp;
				spRoomUser->GetTutorialUIDList( vecTemp );

				for( vitTemp = vecTemp.begin(); vitTemp != vecTemp.end(); ++vitTemp )
				{
					KRoomUserPtr spRU = spRoomUserManager->GetUser( *vitTemp );

					if( spRU != NULL )
					{
						kUpdateUnitInfo.m_bIsTutorialSuccess = true;
						break;
					}
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		//DB�� �� ���ֵ����� ó��
		//{{ 2009. 5. 8  ������		�ǽð� ����ġ ( ��ü���� - �⺻���� )
		kUpdateUnitInfo.m_iEXP	= kDungeonUnitInfo.SumEXP() - ( kDungeonUnitInfo.m_nEXP + kDungeonUnitInfo.m_nPartyBonusEXP ); // ���ʽ� ����ġ�� ����
		//}}
		kUpdateUnitInfo.m_iED	= kDungeonUnitInfo.m_nED - spRoomUser->GetRewardED(); // ���ʽ� ED�� ����

		kUpdateUnitInfo.m_iEXP  = kDungeonUnitInfo.SumEXP();

		// ������ ����
		spRoomUserManager->GetItemList( spRoomUser->GetCID(), kUpdateUnitInfo.m_mapGetItem );

		//client by result process
		kUnitInfoNot.m_vecDungeonUnitInfo.push_back( kDungeonUnitInfo );

		//db process
		vecUpdateUnitInfo.push_back( kUpdateUnitInfo );
	}

	//{{ 2011. 03. 08	��μ� ����ġ�� ȹ���� ������ ����Ʈ �Ϸ� ���� ����
#ifdef SERV_EXP_ACQUISITION_CONDITION_CHANGE
	kUnitInfoNot.m_mapHaveExpInDungeon = spRoomUserManager->GetHaveExpInDungeon();
#endif SERV_EXP_ACQUISITION_CONDITION_CHANGE
	//}}

	return true;
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
float KResultProcess::GetDungeonModePartyBonusRate( char cDungeonMode )
{
	switch( cDungeonMode )
	{
	case CXSLDungeon::DT_NORMAL:
	case CXSLDungeon::DT_HENIR:
	case CXSLDungeon::DT_SECRET:
		{
			return m_fDungeonModePartyBonus[cDungeonMode];
		}break;
	default:
		{
			START_LOG( cerr, L"�������� ��尡 �߸��Ǿ���." )
				<< BUILD_LOG( cDungeonMode )
				<< END_LOG;
		}
	}

	return 0.0f;
}

float KResultProcess::GetDungeonPartyDifficultyBonusRate( char cDifficulty )
{
	switch( cDifficulty )
	{
	case CXSLDungeon::DL_NORMAL:
	case CXSLDungeon::DL_HARD:
	case CXSLDungeon::DL_EXPERT:
		{
			return m_fPartyDifficultyBonus[cDifficulty];
		}break;
	default:
		{
			START_LOG( cerr, L"�������� ���̵��� �߸��Ǿ���." )
				<< BUILD_LOG( cDifficulty )
				<< END_LOG;
		}
	}

	return 1.0f;
}

void KResultProcess::AddDungeonPartyDifficultyBonus_LUA( float fNormal, float fHard, float fExpert )
{
	m_fPartyDifficultyBonus[CXSLDungeon::DL_NORMAL]	= fNormal;
	m_fPartyDifficultyBonus[CXSLDungeon::DL_HARD]	= fHard;
	m_fPartyDifficultyBonus[CXSLDungeon::DL_EXPERT]	= fExpert;

	START_LOG( cout2, L"�������� ��Ƽ ���̵� ���ʽ� Ȯ��.!" )
		<< BUILD_LOG( m_fPartyDifficultyBonus[CXSLDungeon::DL_NORMAL] )
		<< BUILD_LOG( m_fPartyDifficultyBonus[CXSLDungeon::DL_HARD] )
		<< BUILD_LOG( m_fPartyDifficultyBonus[CXSLDungeon::DL_EXPERT] )
		<< END_LOG;
}

void KResultProcess::AddDungeonModePartyBonus_LUA( float fNormal, float fHenir, float fSecret )
{
	m_fDungeonModePartyBonus[CXSLDungeon::DT_NORMAL]	= fNormal;
	m_fDungeonModePartyBonus[CXSLDungeon::DT_HENIR]		= fHenir;
	m_fDungeonModePartyBonus[CXSLDungeon::DT_SECRET]	= fSecret;

	START_LOG( cout2, L"�������� ��� ���̵� ���ʽ� Ȯ��.!" )
		<< BUILD_LOG( m_fDungeonModePartyBonus[CXSLDungeon::DT_NORMAL] )
		<< BUILD_LOG( m_fDungeonModePartyBonus[CXSLDungeon::DT_HENIR] )
		<< BUILD_LOG( m_fDungeonModePartyBonus[CXSLDungeon::DT_SECRET] )
		<< END_LOG;
}
#endif SERV_DUNGEON_DIFFICULTY_REVISION
//}}

//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
char KResultProcess::GetSubStageComboResult( IN int iDungeonIDDiff, IN int iEndNumMember, IN int iCombo, IN int iMonsterPoint )
{
	return (char)lua_tinker::call<double>( GetLuaState(), "SUB_STAGE_COMBO_RESULT",     iDungeonIDDiff, iEndNumMember, iCombo, iMonsterPoint );
}

char KResultProcess::GetSubStageTechResult( IN int iDungeonIDDiff, IN int iEndNumMember, IN int iTech, IN int iMonsterPoint )
{
	return (char)lua_tinker::call<double>( GetLuaState(), "SUB_STAGE_TECHNICAL_RESULT", iDungeonIDDiff, iEndNumMember, iTech, iMonsterPoint );
}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}

//{{ 2013. 02. 25   ���� ��Ż �� ��ũ ��� - ��μ�
#ifdef SERV_LEAVE_ROOM_DUNGEON_RANK
void KResultProcess::LeaveRoomResultRank( IN int						iDungeonID, 
										 IN char						cDifficultyLevel, 
										 KRoomUserPtr					spRoomUser, 
										 IN OUT KDungeonMonsterManager& kDungeonMonsterManager,
										 IN int							iEndNumMember, 
										 IN int							iPartyTotalGivenDamage, 
										 IN int							iTotalPlayTime,
										 KERM_UPDATE_DUNGEON_UNIT_INFO_NOT& kUpdateUnitInfoDummy, 
										 KDungeonUnitResultInfo&		kDungeonUnitInfoDummy
										 )
{
	//////////////////////////////////////////////////////////////////////////
	// ��ũ ���ó��
	//CXSLDungeon::RANK_TYPE	eCombo, eTechnical, eDamage, eTime;
	//float���� exp�� �߰��� ���� %
	float					fComboBonus, fTechBonus, fDamageBonus, fTimeBonus;
	//////////////////////////////////////////////////////////////////////////		
	//COMBO RESULT
	{
		//{{ 2011. 01. 21	������	���� ��ũ ����
		lua_tinker::call<double>( GetLuaState(), "DUNGEON_COMBO_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficultyLevel), &kDungeonMonsterManager, iEndNumMember, iPartyTotalGivenDamage );

		fComboBonus	= spRoomUser->GetPercent();
		kDungeonUnitInfoDummy.m_cComboRank	= (CXSLDungeon::RANK_TYPE)spRoomUser->GetRank();
	}

	//////////////////////////////////////////////////////////////////////////
	//TECHNICAL RESULT
	{
		//{{ 2011. 01. 21	������	���� ��ũ ����

		lua_tinker::call<double>( GetLuaState(), "DUNGEON_TECHNICAL_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficultyLevel), &kDungeonMonsterManager, iEndNumMember, iPartyTotalGivenDamage );

		fTechBonus	= spRoomUser->GetPercent();
		kDungeonUnitInfoDummy.m_cTechnicalRank = (CXSLDungeon::RANK_TYPE)spRoomUser->GetRank();
	}

	//////////////////////////////////////////////////////////////////////////		
	//DAMAGE RESULT
	{
		//{{ 2011. 01. 21	������	���� ��ũ ����
		lua_tinker::call<double>( GetLuaState(), "DUNGEON_DAMAGE_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficultyLevel), &kDungeonMonsterManager );

		fDamageBonus	= spRoomUser->GetPercent();
		kDungeonUnitInfoDummy.m_cDamagedRank	= (CXSLDungeon::RANK_TYPE)spRoomUser->GetRank();
	}

	//////////////////////////////////////////////////////////////////////////		
	//TIME RESULT
	{
		//{{ 2011. 01. 20	������	���� ��ũ ����
		(CXSLDungeon::RANK_TYPE)lua_tinker::call<int>( GetLuaState(), "DUNGEON_TIME_RESULT", spRoomUser.get(), (int)(iDungeonID + cDifficultyLevel), &kDungeonMonsterManager, iTotalPlayTime );

		fTimeBonus	= spRoomUser->GetPercent();
		kDungeonUnitInfoDummy.m_cTimeRank	= (CXSLDungeon::RANK_TYPE)spRoomUser->GetRank();
	}

	//////////////////////////////////////////////////////////////////////////		
	//::: RANK AVERAGE :::
	{
		//{{ 2011. 02. 22	������	���� ��ũ ����
		kDungeonUnitInfoDummy.m_cTotalRank = (char)lua_tinker::call<int>( GetLuaState(), "DUNGEON_TOTAL_RANK_RESULT", kDungeonUnitInfoDummy.m_cComboRank, kDungeonUnitInfoDummy.m_cTechnicalRank, kDungeonUnitInfoDummy.m_cDamagedRank, kDungeonUnitInfoDummy.m_cTimeRank, &kDungeonMonsterManager );

		if( (char)CXSLDungeon::RT_NONE >= kDungeonUnitInfoDummy.m_cTotalRank )
		{
			kDungeonUnitInfoDummy.m_cTotalRank = CXSLDungeon::RT_F;
		}
		else if( (char)CXSLDungeon::RT_SS < kDungeonUnitInfoDummy.m_cTotalRank )
		{
			kDungeonUnitInfoDummy.m_cTotalRank = CXSLDungeon::RT_SS;
		}

		//����Ŭ���� ������ ����
		kUpdateUnitInfoDummy.m_cMaxTotalRank			= kDungeonUnitInfoDummy.m_cTotalRank;
	}
}
#endif SERV_LEAVE_ROOM_DUNGEON_RANK
//}