#include "GameSysVal.h"
//{{ 2010. 01. 05  ������	PC�� �����̾�
#include "X2Data/XSLItemManager.h"
//}}
//{{ 2012. 07. 12	������		���� ť�� ���� ������ �ý���
#ifdef SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
	#include "EventDelaySystemManager.h"
#endif SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
//}}

#ifdef SERV_NEW_EVENT_TYPES
#include "GameEvent/GameEventManager.h"
#endif SERV_NEW_EVENT_TYPES

ImplementRefreshSingleton( KGameSysVal );

KGameSysVal::KGameSysVal()
{
#ifndef SERV_PC_BANG_TYPE
	for( int i = 0; i < GSVT_TOTAL_NUM; ++i )
	{
		m_fValue[i] = 0.0f;
	}
#endif SERV_PC_BANG_TYPE

	m_iLimitsMaxLevel		= 0;
	m_iGuestUserLimitLevel	= 0;
	m_iTempItemUID			= 0;

	//{{ 2010. 01. 05  ������	PC�� �����̾�
	// DB���� �߱��ϴ� ItemUID�� ����� ���� �������� �ȵȴ�.
	// �׷��� �������� �ӽ÷� �߱��ϴ� TempUID�� ItemUID������ ����. ( �߱��Ҷ����� 1�� ���� )	
	KncUid::SetTempUID( m_iTempItemUID );
	KncUid::SetServerGroupID( m_iTempItemUID, KBaseServer::GetKObj()->GetServerGroupID() );
	KncUid::SetServerID( m_iTempItemUID, KBaseServer::GetKObj()->GetUID() );
	//}}

	//{{ 2010. 7. 28  ������	������ ���� NPC
#ifdef SERV_AUTO_HACK_CHECK_NPC
	m_bIsHackUserRegRejected = false;
	m_bIsHackUserRegMonitors = false;
	m_bIsHackUserKick = false;
#endif SERV_AUTO_HACK_CHECK_NPC
	//}}
#ifdef SERV_DESAGREE_HACK_USER_ON_OFF
	m_bDisagreeHackUserFunc = false; // �ؿ� �⺻�� false �̹Ƿ�...
#endif //SERV_DESAGREE_HACK_USER_ON_OFF
	//{{ 2011. 03. 02	������	���� �ŷ� ����
#ifdef SERV_HACK_USER_TRADE_BLOCK
	m_bIsHackUserBlockTrade = false;
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}
	//{{ 2011. 02. 06	������	�ŷ� ���� ���
#ifdef SERV_BLOCK_TRADE
	m_bBlockPersonalTrade = false;
	m_bBlockPersonalShop = false;
	m_bBlockSendLetter = false;
#endif SERV_BLOCK_TRADE
	//}}
	//{{ 2011. 09. 08	������	������ ��Ŷ ����͸�
#ifdef SERV_AUTO_HACK_PACKET_MORNITOING
	m_bIsHackUserPacketMornitor = false;
#endif SERV_AUTO_HACK_PACKET_MORNITOING
	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	m_bIsNewUnitTradeBlock = false;
	m_iNewUnitTradeBlockDay = 0;
	m_iNewUnitTradeBlockUnitClass = 0;
#endif // SERV_NEW_UNIT_TRADE_LIMIT

	//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
	m_iLimitConutOfLetter				= 15;
	m_iLimitConutOfTrade				= 15;
	m_iLetterMonitoringCheckTime		= 300;
	m_iTradeMonitoringCheckTime			= 300;
	m_bLetterAndTradeMonitoringIsOn		= false;
#endif SERV_MONITORING_LETTER_AND_TRADE
	//}}
	//{{ 2011. 04. 26	������	deserialize ���� ����
#ifdef SERV_DESERIALIZE_FAIL_DISCONNECT
	m_iDeserializeFailCheckCount = 9999;
	m_bDeserializeFailCheck = false;
#endif SERV_DESERIALIZE_FAIL_DISCONNECT
	//}}

	//{{ 2011. 09. 07  ��μ�	���� �������� DIE NPC �ð� üũ ���
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
	m_bSubStageNpcDieCheck		= true;
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
	//}}

	//{{  2012. 1. 16	��μ�		���Ӽ������� Server SN �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	m_bCheckServerSN				= false;
	m_bCheckServerSN_ERR_01_1		= false;
	m_bCheckServerSN_ERR_01_2		= false;
	m_bCheckServerSN_ERR_02			= false;
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	//}}

	//{{ 2012. 02. 06	�ڼ���	�߷�Ÿ�� ���� �̺�Ʈ
#ifdef SERV_VALENTINE_DAY_EVENT
	m_bMenBuffEvent = false;
#endif SERV_VALENTINE_DAY_EVENT
	//}}
	//{{ 2012. 03. 13	��μ�		�ǽð� ���� �϶� üũ sms
#ifdef SERV_CHECK_DROP_CCU
	m_bCheckDropCCU = false;
	m_fDropCCUPercent = 0.f;
#endif SERV_CHECK_DROP_CCU
	//}}
	//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	m_bHackingUserKick = false;
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	//}}
	//{{ 2012. 07. 12	������		���� ť�� ���� ������ �ý���
#ifdef SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
	m_bEnableOpenRandomCubeDelaySystem = false;
	m_fOpenRandomCubeDelayTime = 1.0;
	m_iOpenRandomCubeDelayCount = 100;
#endif SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
	//}}

	//{{ 2013. 2. 1	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	m_bLanBugOutCheck				= false;
	m_byteLanBugOutCheckMaxCount	= 0;
	m_fLanBugOutCheckTime			= 1.0f;
	m_byteLanBugOutCheckCount		= 3;
	m_fLanBugOutCheckTermMin		= 0.0f;
	m_fLanBugOutCheckTerm			= 5.0f;
	m_fLanBugOutCheckRepeatTerm		= 0.0f;
	
	m_bLanBugOutVerify				= false;
	m_byteLanBugOutVerifyMaxCount	= 1;
	m_fLanBugOutVerifyTime			= 1.0f;
	m_byteLanBugOutVerifyCount		= 1;
	m_fLanBugOutVerifyTermMin		= 0.0f;
	m_fLanBugOutVerifyTerm			= 5.0f;
	m_fLanBugOutVerifyRepeatTerm	= 0.0f;

	m_bLanBugOutJustLog				= false;
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-09	// �ڼ���
	m_bLanBugOutDisconnectCheckStartOnGameLoadingReq = true;
	m_fLanBugOutDisconnectCheckTerm	= 3.0f;
	m_iLanBugOutMinDisconnectTerm	= 10;
	m_iLanBugOutDisconnectTerm		= 50;
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

	//{{ 2012. 10. 10	�ڼ���	ƨ�� ���� ������ ������
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	m_bLanBugOut1	= false;
	m_bLanBugOut2	= false;
#endif UDP_CAN_NOT_SEND_USER_KICK
	//}}
	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	m_iCheckChangeHostTime	= 180;		// ���� 180 ��
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    m_iBattleFieldCheckChangeHostTime = 60;
    m_dwMaxPingScore		= 500;
#else   SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
	m_dwMaxPingScore		= 5000;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#endif SERV_CHOOSE_FASTEST_HOST
	//}
	//{{ 2010. 11. 12  ��ȿ��	GAMEFORGE ä�� �α� On-Off �����ϵ���
#ifdef SERV_RECORD_CHAT
	m_bRecordChat = false;
#endif SERV_RECORD_CHAT
	//}}	
	//{{ 2013. 2. 18	�ڼ���	���� ���� ������ ���� ���� �� ������ ó��
#ifdef SERV_EVENT_BINGO
	m_byteBingoRepeate	= 0;
#endif SERV_EVENT_BINGO
	//}}
	//{{ 2013. 3. 14	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
	m_iLocalRankingNumOfRanker	= 100;
	m_fLocalRankingElpUpdateGap	= 5.0f;
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX
	m_iPaymentItemID = 0;
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX

#ifdef	SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ// ���볯¥: 2013-05-03
	m_fHostCheckTerm	= 3.0;
#endif	// SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	m_bProcessCommunicationON = false;
#endif //SERV_PROCESS_COMMUNICATION_KSMS

#ifdef SERV_DUNGEON_STAGE_LOAD_LOG// �۾���¥: 2013-05-15	// �ڼ���
	m_bDungeonStageLoadLog	= true;
#endif // SERV_DUNGEON_STAGE_LOAD_LOG

#ifdef SERV_FIX_AFTER_WORK_STORAGE_CLASS// �۾���¥: 2013-12-21	// �ڼ���
	m_iAwsCriterionNum		= 100;
	m_iAwsUnderTerm			= 5;
	m_iAwsAboveOrEqualTerm	= 300;
#endif // SERV_FIX_AFTER_WORK_STORAGE_CLASS

#ifdef SERV_EVENT_CHECK_POWER
	m_fMultiplayer = 1.25f;
#endif SERV_EVENT_CHECK_POWER
}

KGameSysVal::~KGameSysVal()
{

}

ImplToStringW( KGameSysVal )
{
	stm_	<< L"----------[ Game Sys Value ]----------" << std::endl
#ifdef SERV_PC_BANG_TYPE
		;
	for( int iPcBangIndex = 0; iPcBangIndex < m_vecPcBangReward.size(); ++iPcBangIndex )
	{
		stm_	<< TOSTRINGW( m_vecPcBangReward[ iPcBangIndex ].m_iPcBangType )
				<< TOSTRINGW( m_vecPcBangReward[ iPcBangIndex ].m_fEXPRate )
				<< TOSTRINGW( m_vecPcBangReward[ iPcBangIndex ].m_fEDRate )
				<< TOSTRINGW( m_vecPcBangReward[ iPcBangIndex ].m_fVSPointRate )
				<< TOSTRINGW( m_vecPcBangReward[ iPcBangIndex ].m_bSpiritFree )
				<< TOSTRINGW( m_vecPcBangReward[ iPcBangIndex ].m_fPetEXPRate )
				<< TOSTRINGW( m_vecPcBangReward[ iPcBangIndex ].m_iAdditionalHenirReward )
				<< TOSTRINGW( m_vecPcBangReward[ iPcBangIndex ].m_bSecretDungeonFree )
				<< TOSTRINGW( m_vecPcBangReward[ iPcBangIndex ].m_bAvatar )
				<< TOSTRINGW( m_vecPcBangReward[ iPcBangIndex ].m_bTitle );
	}
	stm_
#else SERV_PC_BANG_TYPE
			<< TOSTRINGW( m_fValue[GSVT_GB_EXP] )
			<< TOSTRINGW( m_fValue[GSVT_GB_ED] )
			<< TOSTRINGW( m_fValue[GSVT_GB_VP] )
			<< TOSTRINGW( m_fValue[GSVT_GB_SPIRIT] )
			<< TOSTRINGW( m_fValue[GSVT_PREMIUM_EXP] )
			//{{ 2012. 07. 02	��μ�       PC �� ���� �� �߰� ����ġ
#ifdef SERV_GAME_BANG_PET_ADD_EXP
			<< TOSTRINGW( m_fValue[GSVT_GB_PET_EXP] )
#endif SERV_GAME_BANG_PET_ADD_EXP
			//}}

#endif SERV_PC_BANG_TYPE
//#ifdef SERV_VIP_SYSTEM
			<< TOSTRINGW( m_fValue[GSVT_VIP_EXP] )
//#endif //SERV_VIP_SYSTEM
			<< TOSTRINGW( m_iLimitsMaxLevel )
			<< TOSTRINGW( m_iGuestUserLimitLevel )
			<< TOSTRINGW( m_wstrAdvertisementURL )
			<< TOSTRINGW( m_mapPcBangPremiumItem.size() )
			<< TOSTRINGW( m_iTempItemUID )
			//{{ 2010. 7. 28  ������	������ ���� NPC
#ifdef SERV_AUTO_HACK_CHECK_NPC
			<< TOSTRINGW( m_bIsHackUserRegRejected )
			<< TOSTRINGW( m_bIsHackUserRegMonitors )
			<< TOSTRINGW( m_bIsHackUserKick )
#endif SERV_AUTO_HACK_CHECK_NPC
			//}}
			//{{ 2011. 02. 06	������	�ŷ� ���� ���
#ifdef SERV_BLOCK_TRADE
			<< TOSTRINGW( m_bBlockPersonalTrade )
			<< TOSTRINGW( m_bBlockPersonalShop )
			<< TOSTRINGW( m_bBlockSendLetter )
#endif SERV_BLOCK_TRADE
			//}}
			//{{ 2011. 08. 09	������	�ӽ�ID �����
#ifdef SERV_MACHINE_ID_BLOCK
			<< TOSTRINGW( m_bMachineIDBlock )
#endif SERV_MACHINE_ID_BLOCK
			//}}
			//{{ 2011. 09. 08	������	������ ��Ŷ ����͸�
#ifdef SERV_AUTO_HACK_PACKET_MORNITOING
			<< TOSTRINGW( m_bIsHackUserPacketMornitor )
#endif SERV_AUTO_HACK_PACKET_MORNITOING
			//}}
#ifdef SERV_NEW_UNIT_TRADE_LIMIT
			<< TOSTRINGW( m_bIsNewUnitTradeBlock )
			<< TOSTRINGW( m_iNewUnitTradeBlockDay )
			<< TOSTRINGW( m_iNewUnitTradeBlockUnitClass )
#endif // SERV_NEW_UNIT_TRADE_LIMIT
			//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
			<< TOSTRINGW( m_bMachineIDDuplicateCheck )
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
			//}}
			//{{ 2011. 04. 26	������	deserialize ���� ����
#ifdef SERV_DESERIALIZE_FAIL_DISCONNECT
			<< TOSTRINGW( m_iDeserializeFailCheckCount )
			<< TOSTRINGW( m_bDeserializeFailCheck )
#endif SERV_DESERIALIZE_FAIL_DISCONNECT
			//}}
			//{{ 2012. 07. 12	������		���� ť�� ���� ������ �ý���
#ifdef SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
			<< TOSTRINGW( m_bEnableOpenRandomCubeDelaySystem )
			<< TOSTRINGW( m_fOpenRandomCubeDelayTime )
			<< TOSTRINGW( m_iOpenRandomCubeDelayCount )
#endif SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
			//}}
			//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
			<< TOSTRINGW( m_iCheckChangeHostTime )
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
            << TOSTRINGW( m_iBattleFieldCheckChangeHostTime )
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#endif SERV_CHOOSE_FASTEST_HOST
			//}
#ifdef SERV_RECORD_CHAT
			<< TOSTRINGW( m_bRecordChat )
#endif SERV_RECORD_CHAT
			;

	return stm_;
}

ImplementLuaScriptParser( KGameSysVal )
{
	lua_tinker::class_add<KGameSysVal>( GetLuaState(), "KGameSysVal" );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "dump",							&KGameSysVal::Dump );

#ifdef SERV_PC_BANG_TYPE
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "AddPCBang",						&KGameSysVal::AddPCBang_LUA );
#else SERV_PC_BANG_TYPE
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "AddKNXGameBang",				&KGameSysVal::AddKNXGameBang_LUA );
	//{{ 2012. 07. 02	��μ�       PC �� ���� �� �߰� ����ġ
#ifdef SERV_GAME_BANG_PET_ADD_EXP
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "AddKNXGameBangPetEXP",				&KGameSysVal::AddKNXGameBangPetEXP_LUA );
#endif SERV_GAME_BANG_PET_ADD_EXP
	//}}
#endif SERV_PC_BANG_TYPE

	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "AddLimitsMaxLevel",				&KGameSysVal::AddLimitsMaxLevel_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "AddPremiumEXP",					&KGameSysVal::AddPremiumEXP_LUA );
//#ifdef SERV_VIP_SYSTEM
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "AddVIPEXP",						&KGameSysVal::AddVIPEXP_LUA );
//#endif //SERV_VIP_SYSTEM
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetGuestUserLimitLevel",		&KGameSysVal::SetGuestUserLimitLevel_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetAdvertisementURL",			&KGameSysVal::SetAdvertisementURL_LUA );
	//{{ 2010. 01. 05  ������	PC�� �����̾�
#ifdef SERV_PC_BANG_PRE
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "AddPcBangPremiumItem",			&KGameSysVal::AddPcBangPremiumItem_LUA );
#endif SERV_PC_BANG_PRE
	//}}
	//{{ 2010. 7. 28  ������	������ ���� NPC
#ifdef SERV_AUTO_HACK_CHECK_NPC
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetIsHackUserRegRejected",		&KGameSysVal::SetIsHackUserRegRejected_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetIsHackUserRegMonitors",		&KGameSysVal::SetIsHackUserRegMonitors_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetIsHackUserKick",				&KGameSysVal::SetIsHackUserKick_LUA );
#endif SERV_AUTO_HACK_CHECK_NPC
	//}}

#ifdef SERV_DESAGREE_HACK_USER_ON_OFF
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetDisagreeHackUserFunc",			&KGameSysVal::SetDisagreeHackUserFunc_LUA );
#endif //SERV_DESAGREE_HACK_USER_ON_OFF

	//{{ 2011. 03. 02	������	���� �ŷ� ����
#ifdef SERV_HACK_USER_TRADE_BLOCK
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetIsHackUserBlockTrade",		&KGameSysVal::SetIsHackUserBlockTrade_LUA );
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}
	//{{ 2011. 02. 23	������	ĳ�� ��ǰ ����Ʈ
#ifdef SERV_CASH_ITEM_LIST
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetAccountStatisticsStartDate",	&KGameSysVal::SetAccountStatisticsStartDate_LUA );
#endif SERV_CASH_ITEM_LIST
	//}}
	//{{ 2011. 01. 18	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetCharStatisticsStartDate",	&KGameSysVal::SetCharStatisticsStartDate_LUA );
#endif SERV_CHAR_LOG
	//}}
	//{{ 2011. 02. 06	������	�ŷ� ���� ���
#ifdef SERV_BLOCK_TRADE
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetBlockPersonalTrade",			&KGameSysVal::SetBlockPersonalTrade_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetBlockPersonalShop",			&KGameSysVal::SetBlockPersonalShop_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetBlockSendLetter",			&KGameSysVal::SetBlockSendLetter_LUA );
#endif SERV_BLOCK_TRADE
	//}}
	//{{ 2011. 08. 09	������	�ӽ�ID �����
#ifdef SERV_MACHINE_ID_BLOCK
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetMachineIDBlock",				&KGameSysVal::SetMachineIDBlock_LUA );
#endif SERV_MACHINE_ID_BLOCK
	//}}
	//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetMachineIDDuplicateCheck",	&KGameSysVal::SetMachineIDDuplicateCheck_LUA );
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}
	//{{ 2011. 09. 08	������	������ ��Ŷ ����͸�
#ifdef SERV_AUTO_HACK_PACKET_MORNITOING
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetHackUserPacketMornitor",		&KGameSysVal::SetHackUserPacketMornitor_LUA );
#endif SERV_AUTO_HACK_PACKET_MORNITOING
	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetIsNewUnitTradeBlock",		&KGameSysVal::SetIsNewUnitTradeBlock_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetNewUnitTradeBlockDay",		&KGameSysVal::SetNewUnitTradeBlockDay_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetNewUnitTradeBlockUnitClass",		&KGameSysVal::SetNewUnitTradeBlockUnitClass_LUA );
#endif // SERV_NEW_UNIT_TRADE_LIMIT

	//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetLetterAndTradeMonitoringIsOn",	&KGameSysVal::SetLetterAndTradeMonitoringIsOn_Lua );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetLimitsCountOfLetter",			&KGameSysVal::SetLimitsCountOfLetter_Lua );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetLetterMonitoringCheckTime",		&KGameSysVal::SetLetterMonitoringCheckTime_Lua );
	
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetLimitsCountOfTrade",				&KGameSysVal::SetLimitsCountOfTrade_Lua );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetTradeMonitoringCheckTime",		&KGameSysVal::SetTradeMonitoringCheckTime_Lua );

	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetMonitoringMinED",				&KGameSysVal::SetMonitoringMinED_Lua );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetMonitoringMaxED",				&KGameSysVal::SetMonitoringMaxED_Lua );
	//{{ 2011. 06. 07    ��μ�    ���� �� �ŷ� ���� �ǽð� ���� ���
#ifdef SERV_MONITORING_LETTER_AND_TRADE_REALTIME_BLOCK
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetMonitoringLetterAndTradeBlockTime",				&KGameSysVal::SetMonitoringLetterAndTradeBlockTime_LUA );
#endif SERV_MONITORING_LETTER_AND_TRADE_REALTIME_BLOCK
	//}} 
#endif SERV_MONITORING_LETTER_AND_TRADE
	//}}
	//{{ 2011. 04. 26	������	deserialize ���� ����
#ifdef SERV_DESERIALIZE_FAIL_DISCONNECT
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetDeserializeFailCheckCount",		&KGameSysVal::SetDeserializeFailCheckCount_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetDeserializeFailCheck",			&KGameSysVal::SetDeserializeFailCheck_LUA );
#endif SERV_DESERIALIZE_FAIL_DISCONNECT
	//}}

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetComeBackReward",					&KGameSysVal::SetComeBackReward_LUA );
#endif SERV_COME_BACK_USER_REWARD
	//}} 

	//{{ 2011. 09. 06  ��μ�	���� ���� ��ǰ ��� ���� ��
#ifdef SERV_CHECK_PERSONALSHOP_ITEM
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "InsertItemPrice",					&KGameSysVal::InsertItemPrice_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "DeleteItemPrice",					&KGameSysVal::DeleteItemPrice_LUA );
#endif SERV_CHECK_PERSONALSHOP_ITEM
	//}}

	//{{ 2011. 09. 07  ��μ�	���� �������� DIE NPC �ð� üũ ���
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetSubStageNpcDieCheck",					&KGameSysVal::SetSubStageNpcDieCheck_LUA );
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
	//}}

	//{{ 2012. 07. 09	��μ�       �̺�Ʈ ť�� ���� ���� �ð�
#ifdef SERV_EVENT_CUBE_OPEN_TIME
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetEventCubeOpenStartTime",					&KGameSysVal::SetEventCubeOpenStartTime_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetEventCubeOpenEndTime",					&KGameSysVal::SetEventCubeOpenEndTime_LUA );
#endif SERV_EVENT_CUBE_OPEN_TIME
	//}}

	//{{  2012. 1. 16	��μ�		���Ӽ������� Server SN �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetCheckServerSN_IN_GS",					&KGameSysVal::SetCheckServerSN_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetCheckServerSN_ERR_01_1_IN_GS",			&KGameSysVal::SetCheckServerSN_ERR_01_1_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetCheckServerSN_ERR_01_2_IN_GS",			&KGameSysVal::SetCheckServerSN_ERR_01_2_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetCheckServerSN_ERR_02_IN_GS",				&KGameSysVal::SetCheckServerSN_ERR_02_LUA );
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	//}}

	//{{ 2012. 01. 19	��μ�	�÷���� ���� ��ȭ �̺�Ʈ �ǽð� ����
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetEnchantEventStartTime",					&KGameSysVal::SetEnchantEventStartTime_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetEnchantEventEndTime",					&KGameSysVal::SetEnchantEventEndTime_LUA );
#endif SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	//}}

	//{{ 2012. 02. 06	�ڼ���	�߷�Ÿ�� ���� �̺�Ʈ
#ifdef SERV_VALENTINE_DAY_EVENT
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetMenBuffEvent",					&KGameSysVal::SetMenBuffEvent_LUA );
#endif SERV_VALENTINE_DAY_EVENT
	//}}

	//{{ 2012. 03. 13	��μ�		�ǽð� ���� �϶� üũ sms
#ifdef SERV_CHECK_DROP_CCU
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetCheckDropCCU",					&KGameSysVal::SetCheckDropCCU_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetDropCCUPercent",					&KGameSysVal::SetDropCCUPercent_LUA );
#endif SERV_CHECK_DROP_CCU
	//}}

	//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetHackingUserKick",					&KGameSysVal::SetHackingUserKick_LUA );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
	//}}
	//{{ 2012. 07. 12	������		���� ť�� ���� ������ �ý���
#ifdef SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetEnableOpenRandomCubeDelaySystem",	&KGameSysVal::SetEnableOpenRandomCubeDelaySystem_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetOpenRandomCubeDelayTime",			&KGameSysVal::SetOpenRandomCubeDelayTime_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetOpenRandomCubeDelayCount",			&KGameSysVal::SetOpenRandomCubeDelayCount_LUA );
#endif SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
	//}}

	//{{ 2012. 08. 11	�ڼ���	SMS ���� �߼� ��ũ��Ʈ
#ifdef SERV_RUNTIME_SCRIPT_FOR_SMS
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "AddSMSRecipient",	&KGameSysVal::AddSMSRecipient_LUA );
#endif SERV_RUNTIME_SCRIPT_FOR_SMS
	//}}

	//{{ 2012 ���� ����2 ���� ��Ī �̺�Ʈ	- ��μ�
//#ifdef SERV_2012_PVP_SEASON2_EVENT
//	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetPvpEventTime",	&KGameSysVal::SetPvpEventTime_LUA );
//#endif SERV_2012_PVP_SEASON2_EVENT
//}}

	//{{ 2012. 09. 18   ��μ�   2012 ���� ����2 �̺�Ʈ
#ifdef SERV_2012_PVP_SEASON2_NEW_EVENT
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetPvpEventTime",	&KGameSysVal::SetPvpEventTime_LUA );
#endif SERV_2012_PVP_SEASON2_NEW_EVENT
	//}}

	//{{ 2013. 2. 1	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetLanBugOutCheckOption",	&KGameSysVal::SetLanBugOutCheckOption_Lua );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetLanBugOutVerifyOption",	&KGameSysVal::SetLanBugOutVerifyOption_Lua );
	
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetLanBugOutJustLog",		&KGameSysVal::SetLanBugOutJustLog_Lua );
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-09	// �ڼ���
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetLanBugOutDisconnectCheckStartOnGameLoadingReq",		&KGameSysVal::SetLanBugOutDisconnectCheckStartOnGameLoadingReq_Lua );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetLanBugOutDisconnectCheckTerm",		&KGameSysVal::SetLanBugOutDisconnectCheckTerm_Lua );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetLanBugOutDisconnectTerm",		&KGameSysVal::SetLanBugOutDisconnectTerm_Lua );
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

	//{{ 2012. 10. 10	�ڼ���	ƨ�� ���� ������ ������
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetLanBugOut1",	&KGameSysVal::SetLanBugOut1_Lua );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetLanBugOut2",	&KGameSysVal::SetLanBugOut2_Lua );
#endif UDP_CAN_NOT_SEND_USER_KICK
	//}}
	//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetCheckChangeHostTime",	&KGameSysVal::SetCheckChangeHostTime_LUA );
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetBattleFieldCheckChangeHostTime",	&KGameSysVal::SetBattleFieldCheckChangeHostTime_LUA );
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetMaxPingScore",		&KGameSysVal::SetMaxPingScore_Lua );
#endif SERV_CHOOSE_FASTEST_HOST
	//}
	//{{ 2013. 02. 04  Ư���ð� ������ Ȯ�� ���� - ��μ�
#ifdef SERV_ATTRACTION_ITEM_TIME_EVENT
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetRandomCubeEventStartTime",	&KGameSysVal::SetRandomCubeEventStartTime_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetRandomCubeEventEndTime",		&KGameSysVal::SetRandomCubeEventEndTime_LUA );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetTimeRandomItem",				&KGameSysVal::SetTimeRandomItem_LUA );
#endif SERV_ATTRACTION_ITEM_TIME_EVENT
	//}

#ifdef SERV_RECORD_CHAT
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetIsRecordChat",			&KGameSysVal::SetIsRecordChat_LUA );
#endif SERV_RECORD_CHAT

	//{{ 2013. 2. 18	�ڼ���	���� ���� ������ ���� ���� �� ������ ó��
#ifdef SERV_EVENT_BINGO
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetBingoEventPayoutRepeateUse",	&KGameSysVal::SetBingoEventPayoutRepeateUse_Lua );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetBingoEventPayoutRepeateUse2",	&KGameSysVal::SetBingoEventPayoutRepeateUse2_Lua );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetBingoEventPayoutRepeate",	&KGameSysVal::SetBingoEventPayoutRepeate_Lua );
#endif SERV_EVENT_BINGO
	//}}
	//{{ 2013. 3. 14	�ڼ���	 ���� ��ŷ �ý���
#ifdef SERV_LOCAL_RANKING_SYSTEM
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetLocalRankingNumOfRanker",	&KGameSysVal::SetLocalRankingNumOfRanker_Lua );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetLocalRankingElpUpdateGap",	&KGameSysVal::SetLocalRankingElpUpdateGap_Lua );
	
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "ResetLocalRankingSystem",		&KGameSysVal::ResetLocalRankingSystem_Lua );
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "CheckLocalRankingSystem",		&KGameSysVal::CheckLocalRankingSystem_Lua );
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}

#ifdef	SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ// ���볯¥: 2013-05-03
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetHostCheckTerm",				&KGameSysVal::SetHostCheckTerm_Lua );
#endif	// SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetProcessCommunication",				&KGameSysVal::SetProcessCommunication_Lua );
#endif //SERV_PROCESS_COMMUNICATION_KSMS

#ifdef SERV_ATTRACTION_ITEM_TIME_EVENT_VER2// �۾���¥: 2013-05-13	// �ڼ���
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetAttractionItemTimeEvent",	&KGameSysVal::SetAttractionItemTimeEvent_Lua );
#endif // SERV_ATTRACTION_ITEM_TIME_EVENT_VER2

#ifdef SERV_DUNGEON_STAGE_LOAD_LOG// �۾���¥: 2013-05-15	// �ڼ���
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "SetDungeonStageLoadLog",	&KGameSysVal::SetDungeonStageLoadLog_Lua );
#endif // SERV_DUNGEON_STAGE_LOAD_LOG

#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-30	// �ڼ���
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "ResetTimeEnchantEventInfo",		&KGameSysVal::ResetTimeEnchantEventInfo_Lua );
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-07-03	// �ڼ���
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "ResetExchangeLimitInfo",		&KGameSysVal::ResetExchangeLimitInfo_Lua );
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_FIX_AFTER_WORK_STORAGE_CLASS// �۾���¥: 2013-12-21	// �ڼ���
	lua_tinker::class_def<KGameSysVal>( GetLuaState(), "AfterWorkStorageMessageDeleteTerm_Second",		&KGameSysVal::AfterWorkStorageMessageDeleteTerm_Second_LUA );
#endif // SERV_FIX_AFTER_WORK_STORAGE_CLASS

	lua_tinker::decl( GetLuaState(), "GameSysVal", this );
}

#ifdef SERV_PC_BANG_TYPE
void KGameSysVal::AddPCBang_LUA( int iPcBangType, float fEXPRate, float fEDRate, float fVSPointRate, bool bSpiritFree, float fPetEXPRate,
								 int iAdditionalHenirReward, bool bSecretDungeonFree, bool bAvatar, bool bTitle )
{
	if( iPcBangType < 0 )
	{
		START_LOG( cerr, L"�Ǿ��� Ÿ�� �̻�!" )
			<< BUILD_LOG( iPcBangType )
			<< END_LOG;

		return;
	}

	m_vecPcBangReward.resize( iPcBangType + 1 );
	KPcBangReward& kPcBangReward = m_vecPcBangReward[ iPcBangType ];

	if( fEXPRate < 0.0f )
	{
		START_LOG( cerr, L"�Ǿ��� EXP ����Ȯ�� �̻�.!" )
			<< BUILD_LOG( fEXPRate )
			<< END_LOG;

		fEXPRate = 0.0f;
	}

	if( fEDRate < 0.0f )
	{
		START_LOG( cerr, L"�Ǿ��� ED ����Ȯ�� �̻�.!" )
			<< BUILD_LOG( fEDRate )
			<< END_LOG;

		fEDRate = 0.0f;
	}

	if( fVSPointRate < 0.0f )
	{
		START_LOG( cerr, L"�Ǿ��� VSPoint ����Ȯ�� �̻�.!" )
			<< BUILD_LOG( fVSPointRate )
			<< END_LOG;

		fVSPointRate = 0.0f;
	}

	if( fPetEXPRate < 0.0f )
	{
		START_LOG( cerr, L"�Ǿ��� Pet ����Ȯ�� �̻�.!" )
			<< BUILD_LOG( fPetEXPRate )
			<< END_LOG;

		fPetEXPRate = 0.0f;
	}

	if( iAdditionalHenirReward < 0 )
	{
		START_LOG( cerr, L"��ϸ� �߰� ���� Ƚ���̻�.!" )
			<< BUILD_LOG( iAdditionalHenirReward )
			<< END_LOG;

		iAdditionalHenirReward = 0;
	}

	kPcBangReward.m_iPcBangType				= iPcBangType;
	kPcBangReward.m_fEXPRate				= fEXPRate;
	kPcBangReward.m_fEDRate					= fEDRate;
	kPcBangReward.m_fVSPointRate			= fVSPointRate;
	kPcBangReward.m_bSpiritFree				= bSpiritFree;
	kPcBangReward.m_fPetEXPRate				= fPetEXPRate;
	kPcBangReward.m_iAdditionalHenirReward	= iAdditionalHenirReward;
	kPcBangReward.m_bSecretDungeonFree		= bSecretDungeonFree;
	kPcBangReward.m_bAvatar					= bAvatar;
	kPcBangReward.m_bTitle					= bTitle;


	START_LOG( cout2, L"�Ǿ��� Ÿ�Ժ� ���ʽ� �߰�!" )
		<< BUILD_LOG( kPcBangReward.m_iPcBangType )
		<< BUILD_LOG( kPcBangReward.m_fEXPRate )
		<< BUILD_LOG( kPcBangReward.m_fEDRate )
		<< BUILD_LOG( kPcBangReward.m_fVSPointRate )
		<< BUILD_LOG( kPcBangReward.m_bSpiritFree )
		<< BUILD_LOG( kPcBangReward.m_fPetEXPRate )
		<< BUILD_LOG( kPcBangReward.m_iAdditionalHenirReward )
		<< BUILD_LOG( kPcBangReward.m_bSecretDungeonFree )
		<< BUILD_LOG( kPcBangReward.m_bAvatar )
		<< BUILD_LOG( kPcBangReward.m_bTitle )
		;

}
#else SERV_PC_BANG_TYPE
void KGameSysVal::AddKNXGameBang_LUA( float fEXPRate, float fEDRate, float fVSPointRate, float fSpiritRate )
{
	if( fEXPRate < 0.0f )
	{
		START_LOG( cerr, L"�ؽ� �Ǿ��� EXP ����Ȯ�� �̻�.!" )
			<< BUILD_LOG( fEXPRate )
			<< END_LOG;

		fEXPRate = 0.0f;
	}

	if( fEDRate < 0.0f )
	{
		START_LOG( cerr, L"�ؽ� �Ǿ��� ED ����Ȯ�� �̻�.!" )
			<< BUILD_LOG( fEDRate )
			<< END_LOG;

		fEDRate = 0.0f;
	}

	if( fVSPointRate < 0.0f )
	{
		START_LOG( cerr, L"�ؽ� �Ǿ��� VSPoint ����Ȯ�� �̻�.!" )
			<< BUILD_LOG( fVSPointRate )
			<< END_LOG;

		fVSPointRate = 0.0f;
	}

	if( fSpiritRate < 0.0f )
	{
		START_LOG( cerr, L"�ؽ� �Ǿ��� Spirit ����Ȯ�� �̻�.!" )
			<< BUILD_LOG( fSpiritRate )
			<< END_LOG;

		fSpiritRate = 0.0f;
	}

	m_fValue[GSVT_GB_EXP] = fEXPRate;
	m_fValue[GSVT_GB_ED] = fEDRate;
	m_fValue[GSVT_GB_VP] = fVSPointRate;
	m_fValue[GSVT_GB_SPIRIT] = fSpiritRate;

	START_LOG( cout2, L"�ؽ� �Ǿ��� ���ʽ� Ȯ��.!" )
		<< BUILD_LOG( m_fValue[GSVT_GB_EXP] )
		<< BUILD_LOG( m_fValue[GSVT_GB_ED] )
		<< BUILD_LOG( m_fValue[GSVT_GB_VP] )
		<< BUILD_LOG( m_fValue[GSVT_GB_SPIRIT] )
		;

}

//{{ 2012. 07. 02	��μ�       PC �� ���� �� �߰� ����ġ
#ifdef SERV_GAME_BANG_PET_ADD_EXP
void KGameSysVal::AddKNXGameBangPetEXP_LUA( float fEXPRate )
{
	if( fEXPRate < 0.0f )
	{
		START_LOG( cerr, L"�ؽ� �Ǿ��� Pet ����Ȯ�� �̻�.!" )
			<< BUILD_LOG( fEXPRate )
			<< END_LOG;

		fEXPRate = 0.0f;
	}

	m_fValue[GSVT_GB_PET_EXP] = fEXPRate;

	START_LOG( cout2, L"�ؽ� �Ǿ��� Pet ���ʽ� Ȯ��.!" )
		<< BUILD_LOG( m_fValue[GSVT_GB_PET_EXP] )
		;
}
#endif SERV_GAME_BANG_PET_ADD_EXP
//}}

#endif SERV_PC_BANG_TYPE

void KGameSysVal::AddLimitsMaxLevel_LUA( int iMaxLevel )
{
	if( iMaxLevel < 1 || iMaxLevel > 80  )
	{
		START_LOG( cerr, L"���� ���� ��ġ�̻�.!" )
			<< BUILD_LOG( iMaxLevel )
			<< END_LOG;

		iMaxLevel = 1;
	}

	m_iLimitsMaxLevel = iMaxLevel;

	START_LOG( cout2, L"���ѷ��� ����.!" )
		<< BUILD_LOG( m_iLimitsMaxLevel )
		;
}

void KGameSysVal::AddPremiumEXP_LUA( float fPremiumEXP )
{
	if( fPremiumEXP < 0.0f )
	{
		START_LOG( cerr, L"�����̾� ����ġ ������ EXP ����Ȯ�� �̻�.!" )
			<< BUILD_LOG( fPremiumEXP )
			<< END_LOG;

		fPremiumEXP = 0.0f;
	}

	m_fValue[GSVT_PREMIUM_EXP] = fPremiumEXP;

	START_LOG( cout2, L"�����̾� ������ EXP ���ʽ� Ȯ��.!" )
		<< BUILD_LOG( m_fValue[GSVT_PREMIUM_EXP] )
		;

}

//#ifdef SERV_VIP_SYSTEM
void KGameSysVal::AddVIPEXP_LUA( float fVIPEXP )
{
	if( fVIPEXP < 0.00f )
	{
		START_LOG( cerr, L"VIP ������ EXP ����Ȯ�� �̻�.!" )
			<< BUILD_LOG( fVIPEXP )
			<< END_LOG;
	
		fVIPEXP = 0.00f;
	}
	m_fValue[GSVT_VIP_EXP] = fVIPEXP;

	START_LOG( cout2, L"VIP EXP ���ʽ� Ȯ��.!" )
		<< BUILD_LOG( m_fValue[GSVT_VIP_EXP] )
		;

}
//#endif //SERV_VIP_SYSTEM

//{{ 2008. 5. 16  ������  ü��ID ����
void KGameSysVal::SetGuestUserLimitLevel_LUA( int iGuestUserLimitLevel )
{
	if( iGuestUserLimitLevel < 1 )
	{
		START_LOG( cerr, L"���� ��ġ�̻�.!" )
			<< BUILD_LOG( iGuestUserLimitLevel )
			<< END_LOG;

		iGuestUserLimitLevel = 19;
	}

	m_iGuestUserLimitLevel = iGuestUserLimitLevel;

	START_LOG( cout2, L"ü��ID ���ѷ��� ����.!" )
		<< BUILD_LOG( m_iGuestUserLimitLevel )
		;
}
//}}

//{{ 2008. 9. 2  ������		���� URL
void KGameSysVal::SetAdvertisementURL_LUA( const char* strURL )
{
    m_wstrAdvertisementURL = KncUtil::toWideString( strURL );
}

//{{ 2010. 01. 05  ������	PC�� �����̾�
#ifdef SERV_PC_BANG_PRE

void KGameSysVal::AddPcBangPremiumItem_LUA( char cUnitType, int iItemID )
{
	//const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	//if( pItemTemplet == NULL )
	//{
	//	START_LOG( cerr, L"�������� �ʴ� �������� PC�� �����̾� ���������� �����Ͽ����ϴ�!" )
	//		<< BUILD_LOG( iItemID )
	//		<< END_LOG;
	//	return;
	//}

	//if( pItemTemplet->m_bIsPcBang == false )
	//{
	//	START_LOG( cerr, L"PC�� ���� �������� �ƴմϴ�." )
	//		<< BUILD_LOG( iItemID )
	//		<< END_LOG;
	//	return;
	//}

	//if( pItemTemplet->m_PeriodType != CXSLItem::PT_INFINITY )
	//{
	//	START_LOG( cerr, L"�Ⱓ �������� ��츸 PC�� �����̾� �Դϴ�." )
	//		<< BUILD_LOG( iItemID )
	//		<< BUILD_LOG( pItemTemplet->m_PeriodType )
	//		<< END_LOG;
	//	return;
	//}

	std::map< char, std::vector< int > >::iterator mit = m_mapPcBangPremiumItem.find( cUnitType );
	if( mit == m_mapPcBangPremiumItem.end() )
	{
		std::vector< int > vecInfo;
		vecInfo.push_back( iItemID );
		m_mapPcBangPremiumItem.insert( std::make_pair( cUnitType, vecInfo ) );
	}
	else
	{
        mit->second.push_back( iItemID );
	}

	START_LOG( clog2, L"PC�� �����̾� ������" )
		<< BUILD_LOGc( cUnitType )
		<< BUILD_LOGc( iItemID );
}

#endif SERV_PC_BANG_PRE
//}}

//{{ 2010. 7. 28  ������	������ ���� NPC
#ifdef SERV_AUTO_HACK_CHECK_NPC

void KGameSysVal::SetIsHackUserRegRejected_LUA( bool bVal )
{
	m_bIsHackUserRegRejected = bVal;

	START_LOG( cout2, L"�� ���� �߽߰� DB�� ���� ���� ��� ����!" )
		<< BUILD_LOG( m_bIsHackUserRegRejected );
}	

void KGameSysVal::SetIsHackUserRegMonitors_LUA( bool bVal )
{
	m_bIsHackUserRegMonitors = bVal;

	START_LOG( cout2, L"�� ���� �߽߰� DB�� ��ŷ ���� ��� ���� ��� ����!" )
		<< BUILD_LOG( m_bIsHackUserRegMonitors );
}

void KGameSysVal::SetIsHackUserKick_LUA( bool bVal )
{
	m_bIsHackUserKick = bVal;

	START_LOG( cout2, L"�� ���� �߽߰� ��� ���� ���� ���� ���� ����!" )
		<< BUILD_LOG( m_bIsHackUserKick );
}

#endif SERV_AUTO_HACK_CHECK_NPC
//}}

#ifdef SERV_DESAGREE_HACK_USER_ON_OFF
void KGameSysVal::SetDisagreeHackUserFunc_LUA( bool bVal )
{
	m_bDisagreeHackUserFunc = bVal;

	START_LOG( cout2, L"���� ���� �� ������ ���� ó�� ����!" )
		<< BUILD_LOG( m_bDisagreeHackUserFunc );
}
#endif //SERV_DESAGREE_HACK_USER_ON_OFF

//{{ 2011. 03. 02	������	���� �ŷ� ����
#ifdef SERV_HACK_USER_TRADE_BLOCK
void KGameSysVal::SetIsHackUserBlockTrade_LUA( bool bVal )
{
	m_bIsHackUserBlockTrade = bVal;

	START_LOG( cout2, L"�� ���� �߽߰� ��� �ŷ� ���� ó�� ����!" )
		<< BUILD_LOG( m_bIsHackUserBlockTrade );
}
#endif SERV_HACK_USER_TRADE_BLOCK
//}}

//{{ 2011. 02. 23	������	ĳ�� ��ǰ ����Ʈ
#ifdef SERV_CASH_ITEM_LIST
void KGameSysVal::SetAccountStatisticsStartDate_LUA( const char* strDate )
{
	std::wstring wstrAccountStatisticsStartDate = KncUtil::toWideString( std::string( strDate ) );
	LIF( KncUtil::ConvertStringToCTime( wstrAccountStatisticsStartDate, m_tAccountStatisticsStartDate ) );	

	START_LOG( cout2, L"���� ��� ����� ���� ��¥!" )
		<< BUILD_LOG( wstrAccountStatisticsStartDate );
}
#endif SERV_CASH_ITEM_LIST
//}}

//{{ 2011. 01. 18	������	ĳ���� ī��Ʈ ����
#ifdef SERV_CHAR_LOG
void KGameSysVal::SetCharStatisticsStartDate_LUA( const char* strDate )
{
	std::wstring wstrCharStatisticsStartDate = KncUtil::toWideString( std::string( strDate ) );
	LIF( KncUtil::ConvertStringToCTime( wstrCharStatisticsStartDate, m_tCharStatisticsStartDate ) );	

	START_LOG( cout2, L"ĳ���� ��� ����� ���� ��¥!" )
		<< BUILD_LOG( wstrCharStatisticsStartDate );
}
#endif SERV_CHAR_LOG
//}}

void KGameSysVal::GetAdvertisementURL( std::wstring& wstrAdURL )
{
	wstrAdURL = m_wstrAdvertisementURL;
}
//}}

//{{ 2010. 01. 05  ������	PC�� �����̾�
#ifdef SERV_PC_BANG_PRE

#ifdef SERV_PC_BANG_TYPE
void KGameSysVal::GetPcBangPremiumItem( IN char cUnitType, OUT std::vector< KInventoryItemInfo >& vecPcBangPremiumItem, int iPcBangType )
#else SERV_PC_BANG_TYPE
void KGameSysVal::GetPcBangPremiumItem( IN char cUnitType, OUT std::vector< KInventoryItemInfo >& vecPcBangPremiumItem )
#endif SERV_PC_BANG_TYPE
{
	vecPcBangPremiumItem.clear();

#ifdef SERV_PC_BANG_TYPE
	if( iPcBangType < 0 || m_vecPcBangReward.size() <= iPcBangType ||
		m_vecPcBangReward[iPcBangType].m_bAvatar == false )
	{
		START_LOG( clog, L"PC���� �ƴϰų� �ƹ�Ÿ�� ���� PC��" );
		return;
	}
#endif SERV_PC_BANG_TYPE

	if( m_mapPcBangPremiumItem.empty() )
	{
		START_LOG( clog, L"��ϵ� PC�� ���� �������� ����." );
		return;
	}

	std::map< char, std::vector< int > >::const_iterator mit = m_mapPcBangPremiumItem.find( cUnitType );
	if( mit == m_mapPcBangPremiumItem.end() )
	{
		START_LOG( cwarn, L"��ϵǾ����� ���� UnitType�Դϴ�." )
			<< BUILD_LOGc( cUnitType )
			<< END_LOG;
		return;
	}

	//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
	short sSlotID = 0;
#else
	char cSlotID = 0;
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
	//}}	
	std::vector< int >::const_iterator vit;
	for( vit = mit->second.begin(); vit != mit->second.end(); ++vit )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( *vit );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"�������� �ʴ� �������� PC�� �����̾� ���������� �����Ͽ����ϴ�!" )
				<< BUILD_LOG( *vit )
				<< END_LOG;
			continue;
		}

		if( pItemTemplet->m_bIsPcBang == false )
		{
			START_LOG( cerr, L"PC�� ���� �������� �ƴմϴ�." )
				<< BUILD_LOG( *vit )
				<< END_LOG;
			continue;
		}

		// �ӽ� ItemUID�߱ް� ���ÿ� ������ ȹ�� ó��
		KInventoryItemInfo kInventoryItemInfo;
		//{{ 2013. 04. 10	������	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
		kInventoryItemInfo.m_iItemUID = GetTempItemUID();
#else
		kInventoryItemInfo.m_iItemUID = m_iTempItemUID++;
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}		
		kInventoryItemInfo.m_cSlotCategory = CXSLInventory::ST_PC_BANG;
		//{{ 2011. 12. 15	������	�κ��丮 ����IDũ�� �ø���
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		kInventoryItemInfo.m_sSlotID = sSlotID++;
#else
		kInventoryItemInfo.m_cSlotID = cSlotID++;
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}		

		kInventoryItemInfo.m_kItemInfo.m_iItemID	= *vit;
		kInventoryItemInfo.m_kItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
		kInventoryItemInfo.m_kItemInfo.m_iQuantity	= 1;
		
		vecPcBangPremiumItem.push_back( kInventoryItemInfo );
	}
}

#endif SERV_PC_BANG_PRE
//}}

//{{ 2011. 02. 06	������	�ŷ� ���� ���
#ifdef SERV_BLOCK_TRADE
void KGameSysVal::SetBlockPersonalTrade_LUA( bool bVal )
{
	m_bBlockPersonalTrade = bVal;
	
	START_LOG( cout, L"���� �ŷ� ���� ���� : " << m_bBlockPersonalTrade );
}

void KGameSysVal::SetBlockPersonalShop_LUA( bool bVal )
{
	m_bBlockPersonalShop = bVal;

	START_LOG( cout, L"���� ���� ���� ���� : " << m_bBlockPersonalShop );
}

void KGameSysVal::SetBlockSendLetter_LUA( bool bVal )
{	
	m_bBlockSendLetter = bVal;

	START_LOG( cout, L"���� ���� ���� ���� : " << m_bBlockSendLetter );
}
#endif SERV_BLOCK_TRADE
//}}

//{{ 2011. 08. 09	������	�ӽ�ID �����
#ifdef SERV_MACHINE_ID_BLOCK
void KGameSysVal::SetMachineIDBlock_LUA( bool bVal )
{
	m_bMachineIDBlock = bVal;

	START_LOG( cout, L"�ӽ� ID �� ��� ���� ���� : " << m_bMachineIDBlock );
}

bool KGameSysVal::IsMachineIDBlock() const
{
    return m_bMachineIDBlock;
}
#endif SERV_MACHINE_ID_BLOCK
//}}

//{{ 2011. 09. 08	������	������ ��Ŷ ����͸�
#ifdef SERV_AUTO_HACK_PACKET_MORNITOING
void KGameSysVal::SetHackUserPacketMornitor_LUA( bool bVal )
{
	m_bIsHackUserPacketMornitor = bVal;

	START_LOG( cout, L"������ ���� ��Ŷ ����͸� ���� : " << m_bIsHackUserPacketMornitor );
}
#endif SERV_AUTO_HACK_PACKET_MORNITOING
//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
void KGameSysVal::SetIsNewUnitTradeBlock_LUA( bool bVal )
{
	m_bIsNewUnitTradeBlock = bVal;

	START_LOG( cout, L"�ű� ���� �ŷ� ���� ���� : " << m_bIsNewUnitTradeBlock );
}
void KGameSysVal::SetNewUnitTradeBlockDay_LUA( int iVal )
{
	m_iNewUnitTradeBlockDay = iVal;

	START_LOG( cout, L"�ű� ���� �ŷ� ���� ���� : " << m_iNewUnitTradeBlockDay );
}
void KGameSysVal::SetNewUnitTradeBlockUnitClass_LUA( int iVal )
{
	m_iNewUnitTradeBlockUnitClass = iVal;

	START_LOG( cout, L"�ű� ���� �ŷ� ���� ���� : " << m_iNewUnitTradeBlockUnitClass );
}
#endif // SERV_NEW_UNIT_TRADE_LIMIT

//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
void KGameSysVal::SetMachineIDDuplicateCheck_LUA( bool bVal )
{
	m_bMachineIDDuplicateCheck = bVal;

	START_LOG( cout, L"�ӽ� ID �ߺ� ���� üũ ��� ���� ���� : " << m_bMachineIDDuplicateCheck );
}

bool KGameSysVal::IsMachineIDDuplicateCheck() const
{
	return m_bMachineIDDuplicateCheck;
}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
//}}

//{{ 2011. 04. 05  ��μ�	���� �� �ŷ� ����
#ifdef SERV_MONITORING_LETTER_AND_TRADE
void KGameSysVal::SetLimitsCountOfLetter_Lua( int iCount )
{ 
	m_iLimitConutOfLetter = iCount ;

	START_LOG( cout, L"���� ���� Ƚ�� ���� ��� - ���� Ƚ�� ���� : " << m_iLimitConutOfLetter );
}
void KGameSysVal::SetLetterMonitoringCheckTime_Lua( int iTime )
{
	m_iLetterMonitoringCheckTime = iTime * 60;

	START_LOG( cout, L"���� ���� Ƚ�� ���� ��� - �ð� ����(��) : " << m_iLetterMonitoringCheckTime );
}

void KGameSysVal::SetLetterAndTradeMonitoringIsOn_Lua( bool bIsOn )		
{
	m_bLetterAndTradeMonitoringIsOn = bIsOn;

	START_LOG( cout, L"���� and �ŷ� ���� ��� : " << m_bLetterAndTradeMonitoringIsOn );
}

void KGameSysVal::SetLimitsCountOfTrade_Lua( int iCount )			
{
	m_iLimitConutOfTrade = iCount;

	START_LOG( cout, L"�ŷ� Ƚ�� ���� ��� - �ŷ� Ƚ�� ���� : " << m_iLimitConutOfTrade );
}

void KGameSysVal::SetTradeMonitoringCheckTime_Lua( int iTime )
{
	m_iTradeMonitoringCheckTime = iTime * 60;

	START_LOG( cout, L"�ŷ� Ƚ�� ���� ��� - �ð� ����(��) : " << m_iTradeMonitoringCheckTime );
}

void KGameSysVal::SetMonitoringMinED_Lua( int iMin )
{
	m_iMonitoringMinED = iMin;
	START_LOG( cout, L"�ŷ� �ּ� �ݾ� ����(ED) : " << m_iMonitoringMinED );
}

void KGameSysVal::SetMonitoringMaxED_Lua( int iMax )
{
	m_iMonitoringMaxED = iMax;
	START_LOG( cout, L"�ŷ� �ִ� �ݾ� ����(ED) : " << m_iMonitoringMaxED );
}
#endif SERV_MONITORING_LETTER_AND_TRADE
//}}


//{{ 2011. 04. 26	������	deserialize ���� ����
#ifdef SERV_DESERIALIZE_FAIL_DISCONNECT
void KGameSysVal::SetDeserializeFailCheckCount_LUA( int iCount )
{
	m_iDeserializeFailCheckCount = iCount;

	START_LOG( cout, L"Deserialize Fail Check Count ����" )
		<< BUILD_LOG( m_iDeserializeFailCheckCount );
}

void KGameSysVal::SetDeserializeFailCheck_LUA( bool bVal )
{
	m_bDeserializeFailCheck = bVal;

	START_LOG( cout, L"Deserialize Fail Check ����" )
		<< BUILD_LOG( m_bDeserializeFailCheck );
}
#endif SERV_DESERIALIZE_FAIL_DISCONNECT
//}}

//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
void KGameSysVal::SetComeBackReward_LUA( int iStep, int iDays )
{
	if( iStep <= 0 || iDays <= 0 )
	{
		START_LOG( cerr, L"�޸� ���� ���� �ܰ躰 ��¥ ������ �̻��մϴ�." )
			<< BUILD_LOG( iStep )
			<< BUILD_LOG( iDays )
			<< END_LOG;
		return;
	}
	
	std::map< int, int >::iterator	mit = m_mapComeBackRewardCondition.find( iStep );
	if( mit != m_mapComeBackRewardCondition.end() )
	{
		START_LOG( cerr, L"�޸� ���� ���� �ܰ躰 ��¥ ������ �̹� ���� �մϴ�." )
			<< BUILD_LOG( iStep )
			<< BUILD_LOG( iDays )
			<< END_LOG;
		return;
	}
	m_mapComeBackRewardCondition.insert( std::make_pair( iStep, iDays ) );

	START_LOG( clog, L"�޸� ���� ���� �ܰ躰 ��¥ ������ �߰� �Ǿ����ϴ�." )
		<< BUILD_LOG( iStep )
		<< BUILD_LOG( iDays )
		<< END_LOG;
}

void KGameSysVal::GetComeBackRewardCondition( OUT std::map< int, int >& mapComeBackRewardCondition )
{
	mapComeBackRewardCondition = m_mapComeBackRewardCondition;
}
#endif SERV_COME_BACK_USER_REWARD
//}}

//{{ 2011. 09. 06  ��μ�	���� ���� ��ǰ ��� ���� ��
#ifdef SERV_CHECK_PERSONALSHOP_ITEM
void KGameSysVal::InsertItemPrice_LUA( int iItemID, int iPrice )
{
	std::map< int, int >::iterator mit = m_mapItemPrice.find( iItemID );
	if( mit!= m_mapItemPrice.end() )
	{
		START_LOG( cerr, L"�̹� ��� �Ǿ� �ִ� ItemID �Դϴ�. ���� �Ǿ����ϴ�." )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( mit->second )
			<< END_LOG;

		mit->second = iPrice;

		START_LOG( cerr, L"���� �Ǿ����ϴ�." )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( mit->second )
			<< END_LOG;
	}
	else
	{
		m_mapItemPrice.insert( std::make_pair( iItemID, iPrice ) );
		START_LOG( cout, L"��� �Ǿ����ϴ�." )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( iPrice )
			<< END_LOG;
	}
}

void KGameSysVal::DeleteItemPrice_LUA( int iItemID )
{
	std::map< int, int >::iterator mit = m_mapItemPrice.find( iItemID );
	if( mit!= m_mapItemPrice.end() )
	{
		m_mapItemPrice.erase( mit );
		
		std::map< int, int >::iterator tempmit = m_mapItemPrice.find( iItemID );
		if( tempmit == m_mapItemPrice.end() )
		{
			START_LOG( cerr, L"�������� �����Ͽ����ϴ�." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
		}
	}
	else
	{
		START_LOG( cerr, L"��� ���� ���� �������� �����Ϸ��� �մϴ�" )
			<< BUILD_LOG( iItemID )
			<< END_LOG;
	}
}

int KGameSysVal::GetItemPrice( IN int& iItemID )
{
	std::map< int, int >::iterator mit = m_mapItemPrice.find( iItemID );
	if( mit!= m_mapItemPrice.end() )
	{
		return mit->second;
	}

	return 0;
}
#endif SERV_CHECK_PERSONALSHOP_ITEM
//}}

//{{ 2011. 12.13    ��μ�	���� Ŭ���� �� ������ ���� �̺�Ʈ - ������ �ֹ���(�ߺ� ���� ����)
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT
void KGameSysVal::SetDungeonClearStartTime_LUA( int iYear, int iMonth, int iDay, int iHour, int iMin, int iSec )
{
	m_tDungeonClearStart = CTime( iYear, iMonth, iDay, iHour, iMin, iSec );

	START_LOG( cout, L"������ �ֹ��� �̺�Ʈ ���� �ð�" )
		<< BUILD_LOG( iYear )
		<< BUILD_LOG( iMonth )
		<< BUILD_LOG( iDay )
		<< BUILD_LOG( iHour )
		<< BUILD_LOG( iMin )
		<< BUILD_LOG( iSec )
		<< END_LOG;
}

void KGameSysVal::SetDungeonClearEndTime_LUA( int iYear, int iMonth, int iDay, int iHour, int iMin, int iSec )
{
	m_tDungeonClearEnd = CTime( iYear, iMonth, iDay, iHour, iMin, iSec );

	START_LOG( cout, L"������ �ֹ��� �̺�Ʈ ���� �ð�" )
		<< BUILD_LOG( iYear )
		<< BUILD_LOG( iMonth )
		<< BUILD_LOG( iDay )
		<< BUILD_LOG( iHour )
		<< BUILD_LOG( iMin )
		<< BUILD_LOG( iSec )
		<< END_LOG;
}
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT
//}}

//{{ 2012. 07. 09	��μ�       �̺�Ʈ ť�� ���� ���� �ð�
#ifdef SERV_EVENT_CUBE_OPEN_TIME
void KGameSysVal::SetEventCubeOpenStartTime_LUA( int iYear, int iMonth, int iDay, int iHour, int iMin, int iSec )
{
	m_tEventCubeStart = CTime( iYear, iMonth, iDay, iHour, iMin, iSec );

	START_LOG( cout, L"������ �ֹ��� �̺�Ʈ ���� �ð�" )
		<< BUILD_LOG( iYear )
		<< BUILD_LOG( iMonth )
		<< BUILD_LOG( iDay )
		<< BUILD_LOG( iHour )
		<< BUILD_LOG( iMin )
		<< BUILD_LOG( iSec )
		<< END_LOG;
}

void KGameSysVal::SetEventCubeOpenEndTime_LUA( int iYear, int iMonth, int iDay, int iHour, int iMin, int iSec )
{
	m_tEventCubeEnd = CTime( iYear, iMonth, iDay, iHour, iMin, iSec );

	START_LOG( cout, L"������ �ֹ��� �̺�Ʈ ���� �ð�" )
		<< BUILD_LOG( iYear )
		<< BUILD_LOG( iMonth )
		<< BUILD_LOG( iDay )
		<< BUILD_LOG( iHour )
		<< BUILD_LOG( iMin )
		<< BUILD_LOG( iSec )
		<< END_LOG;
}
#endif SERV_EVENT_CUBE_OPEN_TIME
//}}

//{{  2012. 1. 16	��μ�		���Ӽ������� Server SN �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
void KGameSysVal::SetCheckServerSN_LUA( bool bVal )
{
	m_bCheckServerSN = bVal;

	START_LOG( cout, L"GameServer SNüũ ���� : " << bVal );
}
void KGameSysVal::SetCheckServerSN_ERR_01_1_LUA( bool bVal )
{
	m_bCheckServerSN_ERR_01_1 = bVal;

	START_LOG( cout, L"GameServer SNüũ ���� ERR_01_1 : " << bVal );
}

void KGameSysVal::SetCheckServerSN_ERR_01_2_LUA( bool bVal )
{
	m_bCheckServerSN_ERR_01_2 = bVal;

	START_LOG( cout, L"GameServer SNüũ ���� ERR_01_2 : " << bVal );
}

void KGameSysVal::SetCheckServerSN_ERR_02_LUA( bool bVal )
{
	m_bCheckServerSN_ERR_02 = bVal;

	START_LOG( cout, L"GameServer SNüũ ���� ERR_02 : " << bVal );
}
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
//}}

//{{ 2012. 01. 19	��μ�	�÷���� ���� ��ȭ �̺�Ʈ �ǽð� ����
#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
void KGameSysVal::SetEnchantEventStartTime_LUA( int iYear, int iMonth, int iDay, int iHour, int iMin, int iSec )
{
	m_tEventEnchantStart = CTime( iYear, iMonth, iDay, iHour, iMin, iSec );

	// ������ // 2014-01-27 // ���� �ð��� ���� �α� �߰�
	{
		std::wstring wstrBeginTime	= ( m_tEventEnchantStart.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) ).GetString();

		START_LOG( cout, L"��ȭ �̺�Ʈ ���� �ð�" )
			<< BUILD_LOG( wstrBeginTime )
			<< END_LOG;
	}
}

void KGameSysVal::SetEnchantEventEndTime_LUA( int iYear, int iMonth, int iDay, int iHour, int iMin, int iSec )
{
	m_tEventEnchantEnd = CTime( iYear, iMonth, iDay, iHour, iMin, iSec );

	// ������ // 2014-01-27 // ���� �ð��� ���� �α� �߰�
	{
		std::wstring wstrEndTime	= ( m_tEventEnchantEnd.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) ).GetString();

		START_LOG( cout, L"��ȭ �̺�Ʈ ���� �ð�" )
			<< BUILD_LOG( wstrEndTime )
			<< END_LOG;
	}
}

bool KGameSysVal::IsEnchantEvent()
{
	CTime tNow = CTime::GetCurrentTime();

	if( m_tEventEnchantStart <= tNow && tNow <= m_tEventEnchantEnd )
	{
		START_LOG( clog, L"������ �̺�Ʈ ��ȭ �ð�!!!" )
			<< END_LOG;

		return true;
	}

	START_LOG( clog, L"������ �̺�Ʈ ��ȭ �ð��� �ƴϴ�!!!" )
		<< END_LOG;
	return false;
}
#endif SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
//}}

//{{ 2012. 02. 06	�ڼ���	�߷�Ÿ�� ���� �̺�Ʈ
#ifdef SERV_VALENTINE_DAY_EVENT
void KGameSysVal::SetMenBuffEvent_LUA( bool bValue )
{	
	m_bMenBuffEvent = bValue;	
	START_LOG( clog, L"�߷�Ÿ�� ���� ���� ���ΰ� ����Ǿ����ϴ�.")
		<< BUILD_LOG( m_bMenBuffEvent );
}
#endif SERV_VALENTINE_DAY_EVENT
//}}

//{{ 2012. 03. 13	��μ�		�ǽð� ���� �϶� üũ sms
#ifdef SERV_CHECK_DROP_CCU
void KGameSysVal::SetCheckDropCCU_LUA( bool bValue )
{
	m_bCheckDropCCU = bValue;	
	START_LOG( clog, L"�ǽð� ���� �϶� üũ sms ����Ǿ����ϴ�.")
		<< BUILD_LOG( m_bCheckDropCCU );
}

void KGameSysVal::SetDropCCUPercent_LUA( int iValue )
{
	m_fDropCCUPercent = static_cast<float>(iValue * 0.01f);	
	START_LOG( clog, L"�ǽð� ���� �϶� üũ Percent ����Ǿ����ϴ�.")
		<< BUILD_LOG( m_fDropCCUPercent );
}

float KGameSysVal::GetDropCCUPercent()
{
	// 0 �̶�� ������ ���ǿ� �ɸ��� �� ���̴�.
	if( m_fDropCCUPercent <= 0.f )
	{
		m_fDropCCUPercent = 1.f;
	}

	return m_fDropCCUPercent;
}
#endif SERV_CHECK_DROP_CCU
//}}

//{{ 2012. 06. 04	��μ�       ���� ��ŷ ���� üũ ��û �������� ����
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
void KGameSysVal::SetHackingUserKick_LUA( bool bValue )
{
	m_bHackingUserKick = bValue;

	START_LOG( cout, L"��ŷ ���� ű ����" )
		<< BUILD_LOG( m_bHackingUserKick )
		<< END_LOG;
}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//}}

//{{ 2011. 06. 07    ��μ�    ���� �� �ŷ� ���� �ǽð� ���� ���
#ifdef SERV_MONITORING_LETTER_AND_TRADE_REALTIME_BLOCK
void KGameSysVal::SetMonitoringLetterAndTradeBlockTime_LUA( int hh, int mm, int ss, int iDuration_s )
{
	m_tLetterAndTradeBlockStartTime = CTimeSpan( 0, hh, mm, ss );
	m_tLetterAndTradeBlockEndTime = CTimeSpan( 0, hh, mm, ss );

	CTimeSpan tDuration = CTimeSpan( 0, 0, 0, iDuration_s );
	m_tLetterAndTradeBlockEndTime += tDuration;

	START_LOG( cout, L"���� �� �ŷ� ���� �ǽð� ���� ��� �ð� ����" )
		<< BUILD_LOG( hh )
		<< BUILD_LOG( mm )
		<< BUILD_LOG( ss )
		<< BUILD_LOG( iDuration_s )
		<< BUILD_LOG( m_tLetterAndTradeBlockStartTime.GetTimeSpan() )
		<< BUILD_LOG( m_tLetterAndTradeBlockEndTime.GetTimeSpan() )
		<< END_LOG;
	
}

bool KGameSysVal::IsMonitoringLetterAndTradeBlockTime( CTimeSpan& tSpan )
{
	if( m_tLetterAndTradeBlockStartTime == m_tLetterAndTradeBlockEndTime )
	{
		return false;
	}

	if( m_tLetterAndTradeBlockStartTime <= tSpan &&  tSpan <= m_tLetterAndTradeBlockEndTime )
	{
		return true;
	}

	return false;
}
#endif SERV_MONITORING_LETTER_AND_TRADE_REALTIME_BLOCK
//}}

//{{ 2012. 07. 12	������		���� ť�� ���� ������ �ý���
#ifdef SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
void KGameSysVal::SetEnableOpenRandomCubeDelaySystem_LUA( bool bValue )
{
	m_bEnableOpenRandomCubeDelaySystem = bValue;

	START_LOG( cout, L"���� ť�� ���� ������ �ý��� : Ȱ��ȭ ����" )
		<< BUILD_LOG( m_bEnableOpenRandomCubeDelaySystem );
}

void KGameSysVal::SetOpenRandomCubeDelayTime_LUA( double fTime )
{
	m_fOpenRandomCubeDelayTime = fTime;

	START_LOG( cout, L"���� ť�� ���� ������ �ý��� : ������ �ð�" )
		<< BUILD_LOG( m_fOpenRandomCubeDelayTime );
}

void KGameSysVal::SetOpenRandomCubeDelayCount_LUA( int iCount )
{
	m_iOpenRandomCubeDelayCount = iCount;

	START_LOG( cout, L"���� ť�� ���� ������ �ý��� : �ð��� ���� ī��Ʈ" )
		<< BUILD_LOG( m_iOpenRandomCubeDelayCount );
}
#endif SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
//}}

//{{ 2012. 08. 11	�ڼ���	SMS ���� �߼� ��ũ��Ʈ
#ifdef SERV_RUNTIME_SCRIPT_FOR_SMS
void KGameSysVal::AddSMSRecipient_LUA( IN int iType, IN const char* pPhoneNumber )
{
	if( ( iType < 0 ) || ( SMS_MAX <= iType) )
	{
		START_LOG( cerr, L"�߸��� SMS Type �Դϴ�.")
			<< BUILD_LOG( iType )
			<< BUILD_LOG( pPhoneNumber )
			<< END_LOG;
		return;
	}
	
	if( m_setSMS[iType].insert( std::wstring( pPhoneNumber, pPhoneNumber + strlen( pPhoneNumber ) ) ).second == false )
	{
		START_LOG( cerr, L"�̹� ��ϵǾ� �ִ� �������Դϴ�.")
			<< BUILD_LOG( iType )
			<< BUILD_LOG( pPhoneNumber )
			<< END_LOG;
		return;
	}

	START_LOG( cout, L"SMS ������ �߰�" )
		<< BUILD_LOG( iType )
		<< BUILD_LOG( pPhoneNumber )
		<< END_LOG;
}

void KGameSysVal::GetRecipientSMS( IN const int iType, OUT std::vector<std::wstring>& vecPhoneNum )
{
	if( ( iType < 0 ) || ( SMS_MAX <= iType) )
	{
		START_LOG( cerr, L"�߸��� SMS Type �Դϴ�.")
			<< BUILD_LOG( iType )
			<< END_LOG;
		return;
	}

	vecPhoneNum.clear();

	BOOST_TEST_FOREACH( const std::wstring&, wstrPhoneNumber, m_setSMS[iType] )
	{
		vecPhoneNum.push_back( wstrPhoneNumber );
	}
}
#endif SERV_RUNTIME_SCRIPT_FOR_SMS
//}}

/*
//{{ 2012 ���� ����2 ���� ��Ī �̺�Ʈ	- ��μ�
#ifdef SERV_2012_PVP_SEASON2_EVENT
void KGameSysVal::SetPvpEventTime_LUA( int iDayType, int iCountNum, int iStartHour, int iStartMin, int iEndtHour, int iEndMin )
{
	if( iDayType < 0 || iCountNum < 0 || iStartHour < 0 || iStartMin < 0 || iEndtHour < 0 || iEndMin < 0 )
	{
		START_LOG( cerr, L"PVP EVENT TIME SETTING ����!" )
			<< BUILD_LOG( iDayType )
			<< BUILD_LOG( iCountNum )
			<< BUILD_LOG( iStartHour )
			<< BUILD_LOG( iStartMin )
			<< BUILD_LOG( iEndtHour )
			<< BUILD_LOG( iEndMin )
			<< END_LOG;
		return;
	}

	// �ָ�: iDayType = 1
	// ���� : iDayType = 2

	CTimeSpan tStartPvpEvent = CTimeSpan( 0, iStartHour, iStartMin, 0 );
	CTimeSpan tEndPvpEvent = CTimeSpan( 0, iEndtHour, iEndMin, 0 );

	std::map< int, std::map< int, CTimeSpan > >::iterator mitS = m_mapStartPvpEvent.find( iDayType );
	std::map< int, std::map< int, CTimeSpan > >::iterator mitE = m_mapEndPvpEvent.find( iDayType );

	// ����
	if( mitS == m_mapStartPvpEvent.end() )
	{
		std::map< int, CTimeSpan > mapNew;
		mapNew.insert( std::make_pair( iCountNum, tStartPvpEvent ) );
		m_mapStartPvpEvent.insert( std::make_pair( iDayType, mapNew ) );
	}
	else // �ִ�
	{
		std::map< int, CTimeSpan >::iterator mit = mitS->second.find( iCountNum );
		if( mit == mitS->second.end() )
		{
			mitS->second.insert( std::make_pair( iCountNum, tStartPvpEvent ) );
		}
		else
		{
			START_LOG( cerr, L"PVP EVENT TIME SETTING �ߺ� ��û!" )
				<< BUILD_LOG( iDayType )
				<< BUILD_LOG( iCountNum )
				<< BUILD_LOG( iStartHour )
				<< BUILD_LOG( iStartMin )
				<< BUILD_LOG( iEndtHour )
				<< BUILD_LOG( iEndMin )
				<< END_LOG;
		}
	}

	// ����
	if( mitE == m_mapEndPvpEvent.end() )
	{
		std::map< int, CTimeSpan > mapNew;
		mapNew.insert( std::make_pair( iCountNum, tEndPvpEvent ) );
		m_mapEndPvpEvent.insert( std::make_pair( iDayType, mapNew ) );
	}
	else // �ִ�
	{
		std::map< int, CTimeSpan >::iterator mit = mitE->second.find( iCountNum );
		if( mit == mitE->second.end() )
		{
			mitE->second.insert( std::make_pair( iCountNum, tEndPvpEvent ) );
		}
		else
		{
			START_LOG( cerr, L"PVP EVENT TIME SETTING �ߺ� ��û!" )
				<< BUILD_LOG( iDayType )
				<< BUILD_LOG( iCountNum )
				<< BUILD_LOG( iStartHour )
				<< BUILD_LOG( iStartMin )
				<< BUILD_LOG( iEndtHour )
				<< BUILD_LOG( iEndMin )
				<< END_LOG;
		}
	}

	START_LOG( cout, L"PVP EVENT START TIME SETTING" )
		<< BUILD_LOG( iDayType )
		<< BUILD_LOG( iCountNum )
		<< BUILD_LOG( iStartHour )
		<< BUILD_LOG( iStartMin )
		<< BUILD_LOG( iEndtHour )
		<< BUILD_LOG( iEndMin )
		<< END_LOG;
}

bool KGameSysVal::IsPvpEventTime( CTime& tNow, int& iIndex, std::wstring& wstrLastTime, bool& bIsPossible )
{
	bIsPossible = false;

	// ������ ���� �ð� ��ȯ
	CTime tLastTime;
	if( wstrLastTime.empty() == false )
	{
		if( KncUtil::ConvertStringToCTime( wstrLastTime, tLastTime ) == false )
		{
			START_LOG( clog, L"�ð� ��ȯ ����!" )
				<< BUILD_LOG( wstrLastTime )
				<< END_LOG;
		}
	}

	// ���� �ð��� pvp �̺�Ʈ �ð����� Ȯ������.
	int iDayOfWeek = tNow.GetDayOfWeek();
	int iDayType = 0;

	switch( iDayOfWeek )
	{
	case 2:	case 3:	case 4:	case 5:	case 6: //(������~�ݿ���)
		{
			iDayType = 2;		// ����
		}
		break;
	case 1: case 7:	//(�����,�Ͽ���)
		{
			iDayType = 1;		// �ָ�
		}break;
	default:
		{
			START_LOG( cerr, L"�������� �ʴ� �����Դϴ�." )
				<< BUILD_LOG( iDayOfWeek )
				<< END_LOG;

			return false;
		}break;
	}

	std::map< int, std::map< int, CTimeSpan > >::iterator mitS = m_mapStartPvpEvent.find( iDayType );
	if( mitS == m_mapStartPvpEvent.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� �������Դϴ�." )
			<< BUILD_LOG( iDayOfWeek )
			<< BUILD_LOG( iDayType )
			<< END_LOG;

		return false;
	}

	std::map< int, std::map< int, CTimeSpan > >::iterator mitE = m_mapEndPvpEvent.find( iDayType );
	if( mitE == m_mapEndPvpEvent.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� �������Դϴ�." )
			<< BUILD_LOG( iDayOfWeek )
			<< BUILD_LOG( iDayType )
			<< END_LOG;

		return false;
	}

	std::map< int, CTimeSpan >::iterator mitStart = mitS->second.begin();
	std::map< int, CTimeSpan >::iterator mitEnd = mitE->second.begin();
	for( ; mitStart != mitS->second.end(), mitEnd != mitE->second.end() ; ++mitStart, ++mitEnd )
	{
		// ���� ���������� Ȯ���� index �� ���´�.
		iIndex = mitStart->first;

		CTime tTempStart = CTime( tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), mitStart->second.GetHours(), mitStart->second.GetMinutes(), 0 );
		CTime tTempEnd = CTime( tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), mitEnd->second.GetHours(), mitEnd->second.GetMinutes(), 0 );

		if( tTempStart <= tNow && tNow <= tTempEnd )
		{
			if( mitStart->first == mitEnd->first )
			{
				if( tLastTime < tTempStart )
				{
					bIsPossible = true;
				}
				return true;
			}
			else
			{
				START_LOG( cerr, L"�������� �ʴ� �������Դϴ�." )
					<< BUILD_LOG( iDayOfWeek )
					<< BUILD_LOG( iDayType )
					<< BUILD_LOG( mitStart->first )
					<< BUILD_LOG( mitEnd->first )
					<< END_LOG;
			}
		}
	}

	return false;
}
#endif SERV_2012_PVP_SEASON2_EVENT
//}}
*/


//{{ 2012. 09. 18   ��μ�   2012 ���� ����2 �̺�Ʈ
#ifdef SERV_2012_PVP_SEASON2_NEW_EVENT
void KGameSysVal::SetPvpEventTime_LUA( int iDayType, int iCountNum, int iStartHour, int iStartMin, int iEndtHour, int iEndMin )
{
	if( iDayType < 0 || iCountNum < 0 || iStartHour < 0 || iStartMin < 0 || iEndtHour < 0 || iEndMin < 0 )
	{
		START_LOG( cerr, L"PVP EVENT TIME SETTING ����!" )
			<< BUILD_LOG( iDayType )
			<< BUILD_LOG( iCountNum )
			<< BUILD_LOG( iStartHour )
			<< BUILD_LOG( iStartMin )
			<< BUILD_LOG( iEndtHour )
			<< BUILD_LOG( iEndMin )
			<< END_LOG;
		return;
	}

	// �ָ�: iDayType = 1
	// ���� : iDayType = 2

	CTimeSpan tStartPvpEvent = CTimeSpan( 0, iStartHour, iStartMin, 0 );
	CTimeSpan tEndPvpEvent = CTimeSpan( 0, iEndtHour, iEndMin, 0 );

	std::map< int, std::map< int, CTimeSpan > >::iterator mitS = m_mapStartPvpEvent.find( iDayType );
	std::map< int, std::map< int, CTimeSpan > >::iterator mitE = m_mapEndPvpEvent.find( iDayType );

	// ����
	if( mitS == m_mapStartPvpEvent.end() )
	{
		std::map< int, CTimeSpan > mapNew;
		mapNew.insert( std::make_pair( iCountNum, tStartPvpEvent ) );
		m_mapStartPvpEvent.insert( std::make_pair( iDayType, mapNew ) );
	}
	else // �ִ�
	{
		std::map< int, CTimeSpan >::iterator mit = mitS->second.find( iCountNum );
		if( mit == mitS->second.end() )
		{
			mitS->second.insert( std::make_pair( iCountNum, tStartPvpEvent ) );
		}
		else
		{
			START_LOG( cerr, L"PVP EVENT TIME SETTING �ߺ� ��û!" )
				<< BUILD_LOG( iDayType )
				<< BUILD_LOG( iCountNum )
				<< BUILD_LOG( iStartHour )
				<< BUILD_LOG( iStartMin )
				<< BUILD_LOG( iEndtHour )
				<< BUILD_LOG( iEndMin )
				<< END_LOG;
		}
	}

	// ����
	if( mitE == m_mapEndPvpEvent.end() )
	{
		std::map< int, CTimeSpan > mapNew;
		mapNew.insert( std::make_pair( iCountNum, tEndPvpEvent ) );
		m_mapEndPvpEvent.insert( std::make_pair( iDayType, mapNew ) );
	}
	else // �ִ�
	{
		std::map< int, CTimeSpan >::iterator mit = mitE->second.find( iCountNum );
		if( mit == mitE->second.end() )
		{
			mitE->second.insert( std::make_pair( iCountNum, tEndPvpEvent ) );
		}
		else
		{
			START_LOG( cerr, L"PVP EVENT TIME SETTING �ߺ� ��û!" )
				<< BUILD_LOG( iDayType )
				<< BUILD_LOG( iCountNum )
				<< BUILD_LOG( iStartHour )
				<< BUILD_LOG( iStartMin )
				<< BUILD_LOG( iEndtHour )
				<< BUILD_LOG( iEndMin )
				<< END_LOG;
		}
	}

	START_LOG( cout, L"PVP EVENT START TIME SETTING" )
		<< BUILD_LOG( iDayType )
		<< BUILD_LOG( iCountNum )
		<< BUILD_LOG( iStartHour )
		<< BUILD_LOG( iStartMin )
		<< BUILD_LOG( iEndtHour )
		<< BUILD_LOG( iEndMin )
		<< END_LOG;
}

bool KGameSysVal::IsPvpEventTime( CTime& tNow )
{	
	// ���� �ð��� pvp �̺�Ʈ �ð����� Ȯ������.
	int iDayOfWeek = tNow.GetDayOfWeek();
	int iDayType = 0;

	switch( iDayOfWeek )
	{
	case 2:	case 3:	case 4:	case 5:	case 6: //(������~�ݿ���)
		{
			iDayType = 2;		// ����
		}
		break;
	case 1: case 7:	//(�����,�Ͽ���)
		{
			iDayType = 1;		// �ָ�
		}break;
	default:
		{
			START_LOG( cerr, L"�������� �ʴ� �����Դϴ�." )
				<< BUILD_LOG( iDayOfWeek )
				<< END_LOG;

			return false;
		}break;
	}

	std::map< int, std::map< int, CTimeSpan > >::iterator mitS = m_mapStartPvpEvent.find( iDayType );
	if( mitS == m_mapStartPvpEvent.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� �������Դϴ�." )
			<< BUILD_LOG( iDayOfWeek )
			<< BUILD_LOG( iDayType )
			<< END_LOG;

		return false;
	}

	std::map< int, std::map< int, CTimeSpan > >::iterator mitE = m_mapEndPvpEvent.find( iDayType );
	if( mitE == m_mapEndPvpEvent.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� �������Դϴ�." )
			<< BUILD_LOG( iDayOfWeek )
			<< BUILD_LOG( iDayType )
			<< END_LOG;

		return false;
	}

	std::map< int, CTimeSpan >::iterator mitStart = mitS->second.begin();
	std::map< int, CTimeSpan >::iterator mitEnd = mitE->second.begin();
	for( ; mitStart != mitS->second.end(), mitEnd != mitE->second.end() ; ++mitStart, ++mitEnd )
	{
		CTime tTempStart = CTime( tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), mitStart->second.GetHours(), mitStart->second.GetMinutes(), 0 );
		CTime tTempEnd = CTime( tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), mitEnd->second.GetHours(), mitEnd->second.GetMinutes(), 0 );

		if( tTempStart <= tNow && tNow <= tTempEnd )
		{
				return true;
		}
	}

	return false;
}
#endif SERV_2012_PVP_SEASON2_NEW_EVENT
//}}

//{{ 2012. 10. 10	�ڼ���	ƨ�� ���� ������ ������
#ifdef UDP_CAN_NOT_SEND_USER_KICK
void KGameSysVal::SetLanBugOut1_Lua( bool bOn )
{
	m_bLanBugOut1 = bOn;
}

void KGameSysVal::SetLanBugOut2_Lua( bool bOn )
{
	m_bLanBugOut2 = bOn;
}
#endif UDP_CAN_NOT_SEND_USER_KICK
//}}

//{{ �� ����üũ ȣ��Ʈ ���� - ��μ�
#ifdef SERV_CHOOSE_FASTEST_HOST
void KGameSysVal::SetCheckChangeHostTime_LUA( int iCheckTime )
{
	m_iCheckChangeHostTime = iCheckTime;

	START_LOG( cout2, L"�� üũ�Ͽ� ���� ���� �ð��� ���� �Ǿ����ϴ�." )
		<< BUILD_LOG( m_iCheckChangeHostTime )
		<< END_LOG;
}

#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
void KGameSysVal::SetBattleFieldCheckChangeHostTime_LUA( int iCheckTime )
{
	m_iBattleFieldCheckChangeHostTime = iCheckTime;

	START_LOG( cout2, L"�� üũ�Ͽ� �ʵ� ���� ���� �ð��� ���� �Ǿ����ϴ�." )
		<< BUILD_LOG( m_iBattleFieldCheckChangeHostTime )
		<< END_LOG;
}
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE

#endif SERV_CHOOSE_FASTEST_HOST
//}

//{{ 2013. 02. 04  Ư���ð� ������ Ȯ�� ���� - ��μ�
#ifdef SERV_ATTRACTION_ITEM_TIME_EVENT
void KGameSysVal::SetRandomCubeEventStartTime_LUA( int iYear, int iMonth, int iDay, int iHour, int iMin, int iSec )
{
	m_tRandomCubeEventStart = CTime( iYear, iMonth, iDay, iHour, iMin, iSec );

	START_LOG( cout, L"���� ť�� �̺�Ʈ ���� �ð�" )
		<< BUILD_LOG( iYear )
		<< BUILD_LOG( iMonth )
		<< BUILD_LOG( iDay )
		<< BUILD_LOG( iHour )
		<< BUILD_LOG( iMin )
		<< BUILD_LOG( iSec )
		<< END_LOG;
}

void KGameSysVal::SetRandomCubeEventEndTime_LUA( int iYear, int iMonth, int iDay, int iHour, int iMin, int iSec )
{
	m_tRandomCubeEventEnd = CTime( iYear, iMonth, iDay, iHour, iMin, iSec );

	START_LOG( cout, L"���� ť�� �̺�Ʈ ���� �ð�" )
		<< BUILD_LOG( iYear )
		<< BUILD_LOG( iMonth )
		<< BUILD_LOG( iDay )
		<< BUILD_LOG( iHour )
		<< BUILD_LOG( iMin )
		<< BUILD_LOG( iSec )
		<< END_LOG;
}

void KGameSysVal::SetTimeRandomItem_LUA( int iItemID, int iEventItemID )
{
	m_mapTimeRandomItem.insert( std::make_pair( iItemID, iEventItemID ) );
}

int KGameSysVal::IsTimeEventItemID( int iItemID )
{
	std::map<int,int>::iterator mit = m_mapTimeRandomItem.find( iItemID );
	if( mit != m_mapTimeRandomItem.end() )
	{
		return mit->second;
	}
	return -1;
}
#endif SERV_ATTRACTION_ITEM_TIME_EVENT
//}

//{{ 2010. 11. 12  ��ȿ��	GAMEFORGE ä�� �α� On-Off �����ϵ���
#ifdef SERV_RECORD_CHAT
void KGameSysVal::SetIsRecordChat_LUA( bool bVal )
{
	m_bRecordChat = bVal;

	START_LOG( cout2, L"ä�� �α� On-Off ����!" )
		<< BUILD_LOG( m_bRecordChat );
}
#endif SERV_RECORD_CHAT
//}}

#ifdef SERV_PC_BANG_TYPE
float KGameSysVal::GetGBEXPRate( int iPcBangType )
{
	if( iPcBangType < 0 || m_vecPcBangReward.size() <= iPcBangType )
	{
		return 0.0f;
	}

	return m_vecPcBangReward[iPcBangType].m_fEXPRate;
}

float KGameSysVal::GetGBEDRate( int iPcBangType )
{
	if( iPcBangType < 0 || m_vecPcBangReward.size() <= iPcBangType )
	{
		return 0.0f;
	}

	return m_vecPcBangReward[iPcBangType].m_fEDRate;
}

float KGameSysVal::GetGBVPRate( int iPcBangType )
{
	if( iPcBangType < 0 || m_vecPcBangReward.size() <= iPcBangType )
	{
		return 0.0f;
	}

	return m_vecPcBangReward[iPcBangType].m_fVSPointRate;
}

bool KGameSysVal::IsSpiritFree( int iPcBangType )
{
	if( iPcBangType < 0 || m_vecPcBangReward.size() <= iPcBangType )
	{
		return false;
	}

	return m_vecPcBangReward[iPcBangType].m_bSpiritFree;
}

float KGameSysVal::GetGBPetEXPRate( int iPcBangType )
{
	if( iPcBangType < 0 || m_vecPcBangReward.size() <= iPcBangType )
	{
		return 0.0f;
	}

	return m_vecPcBangReward[iPcBangType].m_fPetEXPRate;
}

bool KGameSysVal::IsSecretDungeonFree( int iPcBangType )
{
	if( iPcBangType < 0 || m_vecPcBangReward.size() <= iPcBangType )
	{
		return false;
	}

	return m_vecPcBangReward[iPcBangType].m_bSecretDungeonFree;
}

int KGameSysVal::GetAdditionalHenirRewardCount( int iPcBangType )
{
	if( iPcBangType < 0 || m_vecPcBangReward.size() <= iPcBangType )
	{
		return false;
	}

	return m_vecPcBangReward[iPcBangType].m_iAdditionalHenirReward;
}

bool KGameSysVal::IsUsingPcBangTitle( int iPcBangType )
{
	if( iPcBangType < 0 || m_vecPcBangReward.size() <= iPcBangType )
	{
		return false;
	}

	return m_vecPcBangReward[iPcBangType].m_bTitle;
}

KPcBangReward KGameSysVal::GetPcBangReward( int iPcBangType )
{
	if( iPcBangType < 0 || m_vecPcBangReward.size() <= iPcBangType )
	{
		// �Ǿ����� �ƴ� ��� iPcBangType ������ -1�� �����µ�, �������� ���̹Ƿ� �����α׸� �����մϴ�.

		//START_LOG( cerr, L"�Ǿ��� Ÿ�� �̻�!" )
		//	<< BUILD_LOG( iPcBangType )
		//	<< BUILD_LOG( m_vecPcBangReward.size() )
		//	<< END_LOG;

		KPcBangReward kPcBangReward;
		kPcBangReward.m_iPcBangType				= -1;
		kPcBangReward.m_fEXPRate				= 0.0f;
		kPcBangReward.m_fEDRate					= 0.0f;
		kPcBangReward.m_fVSPointRate			= 0.0f;
		kPcBangReward.m_bSpiritFree				= false;
		kPcBangReward.m_fPetEXPRate				= 0.0f;
		kPcBangReward.m_iAdditionalHenirReward	= 0;
		kPcBangReward.m_bSecretDungeonFree		= false;
		kPcBangReward.m_bAvatar					= false;
		kPcBangReward.m_bTitle					= false;		
		return kPcBangReward;
	}

	return m_vecPcBangReward[iPcBangType];
}
#endif SERV_PC_BANG_TYPE

#ifdef SERV_NEW_EVENT_TYPES
int KGameSysVal::GetLimitsLevel()
{
	// �̺�Ʈ�� �� ���� ���� ���� ��� �̿�
	return max( SiKGameEventManager()->GetMaxLevel(), m_iLimitsMaxLevel );
}
#endif SERV_NEW_EVENT_TYPES

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX
void KGameSysVal::SetDungeonClearPaymentItemID_LUA( int iItemID )
{
	m_iPaymentItemID = iItemID; 
	START_LOG( cout, L"���� Ŭ���� ���� �ߺ� ���� ������ ���!" )
		<< BUILD_LOG( m_iPaymentItemID );
}
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
void KGameSysVal::SetProcessCommunication_Lua( IN bool bProcessCommunicationON )
{
	m_bProcessCommunicationON = bProcessCommunicationON; 
	START_LOG( cout, L"���� ���������� ��� ���" )
		<< BUILD_LOG( m_bProcessCommunicationON );
}
#endif //SERV_PROCESS_COMMUNICATION_KSMS

#ifdef	SERV_LOCAL_RANKING_SYSTEM // ���볯¥: 2013-04-07
void KGameSysVal::ResetLocalRankingSystem_Lua( void ) const
{
	const int iPfID = KBaseServer::GetKObj()->GetPfID();

	if( iPfID != PI_LOGIN_SERVER )
	{
		START_LOG( cerr, L"�α��� ���������� ����� �� �ֽ��ϴ�." )
			<< BUILD_LOG( iPfID )
			<< END_LOG;
		return;
	}

	KncSend( iPfID, 0, iPfID, KBaseServer::GetKObj()->GetUID(), NULL, ELG_LOCAL_RANKING_RESET_NOT, char() );
}

void KGameSysVal::CheckLocalRankingSystem_Lua( int iMainTabIndex, int iSubTabIndex ) const
{
	const int iPfID = KBaseServer::GetKObj()->GetPfID();
	if( ( iPfID != PI_LOGIN_SERVER ) && ( iPfID != PI_GS_SERVER ) )
	{
		START_LOG( cerr, L"�α��� Ȥ�� ���� ���������� ����� �� �ֽ��ϴ�." )
			<< BUILD_LOG( iPfID )
			<< END_LOG;
		return;
	}

	KELG_LOCAL_RANKING_SYSTEM_CHECK_NOT kPacket;
	kPacket.m_iMainTabIndex	= iMainTabIndex;
	kPacket.m_iSubTabIndex	= iSubTabIndex;
	KncSend( iPfID, 0, iPfID, KBaseServer::GetKObj()->GetUID(), NULL, ELG_LOCAL_RANKING_SYSTEM_CHECK_NOT, kPacket );
}
#endif	// SERV_LOCAL_RANKING_SYSTEM

//{{ 2013. 2. 1	�ڼ���	������ ���� �ڵ�2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
void KGameSysVal::SetLanBugOutCheckOption_Lua( void )
{
	KLuaManager luaMgr( GetLuaState() );

	char temp;

	LUA_GET_VALUE( luaMgr,	"bLanBugOutCheck",				m_bLanBugOutCheck,				false	);
	LUA_GET_VALUE( luaMgr,	"byteLanBugOutCheckMaxCount",	temp,							0		);
	m_byteLanBugOutCheckMaxCount = temp;

	LUA_GET_VALUE( luaMgr,	"fLanBugOutCheckTime",			m_fLanBugOutCheckTime,			1.0f	);
	LUA_GET_VALUE( luaMgr,	"byteLanBugOutCheckCount",		temp,							10		);
	m_byteLanBugOutCheckCount = temp;

	LUA_GET_VALUE( luaMgr,	"fLanBugOutCheckTermMin",		m_fLanBugOutCheckTermMin,		1.0f	);
	LUA_GET_VALUE( luaMgr,	"fLanBugOutCheckTerm",			m_fLanBugOutCheckTerm,			0.0f	);
	LUA_GET_VALUE( luaMgr,	"fLanBugOutCheckRepeatTerm",	m_fLanBugOutCheckRepeatTerm,	0.5f	);

	START_LOG( cout, L"������ ���� �ڵ�: ���� ���� ����" )
		<< BUILD_LOG( m_bLanBugOutCheck )
		<< BUILD_LOG( m_byteLanBugOutCheckMaxCount )
		<< BUILD_LOG( m_fLanBugOutCheckTime )
		<< BUILD_LOG( m_byteLanBugOutCheckCount )
		<< BUILD_LOG( m_fLanBugOutCheckTermMin )
		<< BUILD_LOG( m_fLanBugOutCheckTerm )
		<< BUILD_LOG( m_fLanBugOutCheckRepeatTerm );
}

void KGameSysVal::SetLanBugOutVerifyOption_Lua( void )
{
	KLuaManager luaMgr( GetLuaState() );

	char temp;

	LUA_GET_VALUE( luaMgr,	"bLanBugOutVerify",				m_bLanBugOutVerify,				false	);
	LUA_GET_VALUE( luaMgr,	"byteLanBugOutVerifyMaxCount",	temp,							10		);
	m_byteLanBugOutVerifyMaxCount = temp;

	LUA_GET_VALUE( luaMgr,	"fLanBugOutVerifyTime",			m_fLanBugOutVerifyTime,			1.0f	);
	LUA_GET_VALUE( luaMgr,	"byteLanBugOutVerifyCount",		temp,							10		);
	m_byteLanBugOutVerifyCount = temp;

	LUA_GET_VALUE( luaMgr,	"fLanBugOutVerifyTermMin",		m_fLanBugOutVerifyTermMin,		1.0f	);
	LUA_GET_VALUE( luaMgr,	"fLanBugOutVerifyTerm",			m_fLanBugOutVerifyTerm,			0.0f	);
	LUA_GET_VALUE( luaMgr,	"fLanBugOutVerifyRepeatTerm",	m_fLanBugOutVerifyRepeatTerm,	0.5f	);

	START_LOG( cout, L"������ ���� �ڵ�: ���� ���� ����" )
		<< BUILD_LOG( m_bLanBugOutVerify )
		<< BUILD_LOG( m_byteLanBugOutVerifyMaxCount )
		<< BUILD_LOG( m_fLanBugOutVerifyTime )
		<< BUILD_LOG( m_byteLanBugOutVerifyCount )
		<< BUILD_LOG( m_fLanBugOutVerifyTermMin )
		<< BUILD_LOG( m_fLanBugOutVerifyTerm )
		<< BUILD_LOG( m_fLanBugOutVerifyRepeatTerm );
}
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

#ifdef SERV_ATTRACTION_ITEM_TIME_EVENT_VER2// �۾���¥: 2013-05-13	// �ڼ���
void KGameSysVal::SetAttractionItemTimeEvent_Lua( IN int iSrcItemID, IN int iDesItemID )
{
	KLuaManager luaMgr( GetLuaState() );
	
	AttractionItemTimeEventInfo sData;
	std::wstring wstrDate;

	// ������
	LUA_GET_VALUE( luaMgr,	L"wstrBeginDate",	wstrDate,	L"" );
	if( wstrDate.empty() == false )
	{
		if( KncUtil::ConvertStringToCTime( wstrDate, sData.m_tBeginDate ) == false )
		{
			START_LOG( cerr, L"AttractionItemTimeEvent ���� �ð� ��ȯ�� ���� �Ͽ����ϴ�." )
				<< BUILD_LOG( iSrcItemID )
				<< BUILD_LOG( iDesItemID )
				<< BUILD_LOG( wstrDate )
				<< END_LOG;
			return;
		}
	}

	// ������
	LUA_GET_VALUE( luaMgr,	L"wstrEndDate",	wstrDate,	L"" );
	if( wstrDate.empty() == false )
	{
		if( KncUtil::ConvertStringToCTime( wstrDate, sData.m_tEndDate ) == false )
		{
			START_LOG( cerr, L"AttractionItemTimeEvent ���� �ð� ��ȯ�� ���� �Ͽ����ϴ�." )
				<< BUILD_LOG( iSrcItemID )
				<< BUILD_LOG( iDesItemID )
				<< BUILD_LOG( wstrDate )
				<< END_LOG;
			return;
		}
	}

	// ���� �ð��� ���� �ð� ������ �ùٸ��� üũ!
	if( ( 0 < sData.m_tBeginDate.GetTime() ) &&
		( 0 < sData.m_tEndDate.GetTime() ) &&
		( sData.m_tEndDate < sData.m_tBeginDate )
		)
	{
		START_LOG( cerr, L"AttractionItemTimeEvent ����, ���� �ð� ������ �̻��մϴ�." )
			<< BUILD_LOG( iSrcItemID )
			<< BUILD_LOG( iDesItemID )
			<< END_LOG;
		return;
	}

	std::pair<std::multimap<int, AttractionItemTimeEventInfo>::const_iterator, std::multimap<int, AttractionItemTimeEventInfo>::const_iterator> pairFindResult = m_mmapAttractionItemTimeEventInfo.equal_range( iSrcItemID );
	for( ; pairFindResult.first != pairFindResult.second; ++pairFindResult.first )
	{
		if( ( 0 < pairFindResult.first->second.m_tBeginDate.GetTime() ) &&
			( 0 < sData.m_tEndDate.GetTime() ) &&
			( sData.m_tEndDate.GetTime() < pairFindResult.first->second.m_tBeginDate.GetTime() )
			)
		{
			continue;
		}

		if( ( 0 < pairFindResult.first->second.m_tEndDate.GetTime() ) &&
			( 0 < sData.m_tBeginDate.GetTime() ) &&
			( pairFindResult.first->second.m_tEndDate.GetTime() < sData.m_tBeginDate.GetTime() )
			)
		{
			continue;
		}

		break;
	}

	if( pairFindResult.first != pairFindResult.second )
	{
		START_LOG( cerr, L"�Ⱓ�� ��ġ�� AttractionItemTimeEvent ������ �����մϴ�." )
			<< BUILD_LOG( iSrcItemID )
			<< BUILD_LOG( iDesItemID )
			<< END_LOG;
		return;
	}

	sData.m_iEventAttractionItem = iDesItemID;
	m_mmapAttractionItemTimeEventInfo.insert( std::multimap<int, AttractionItemTimeEventInfo>::value_type( iSrcItemID, sData ) );


	// ������ // 2014-01-27 // ���� �ð��� ���� �α� �߰�
	{
		std::wstring wstrBeginTime	= ( sData.m_tBeginDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) ).GetString();
		std::wstring wstrEndTime	= ( sData.m_tEndDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) ).GetString();

		START_LOG( cout, L"������ Ȯ�� ���� ���� �ð�" )
			<< BUILD_LOG( iSrcItemID )
			<< BUILD_LOG( sData.m_iEventAttractionItem )
			<< BUILD_LOG( wstrBeginTime )
			<< END_LOG;

		START_LOG( cout, L"������ Ȯ�� ���� ���� �ð�" )
			<< BUILD_LOG( wstrEndTime )
			<< END_LOG;
	}
}

void KGameSysVal::CheckAttractionItemTimeEvent( IN OUT int& iItemID )
{
	CTime tCurrentTime = CTime::GetCurrentTime();

	std::pair<std::multimap<int, AttractionItemTimeEventInfo>::iterator, std::multimap<int, AttractionItemTimeEventInfo>::const_iterator> pairFindResult = m_mmapAttractionItemTimeEventInfo.equal_range( iItemID );
	while( pairFindResult.first != pairFindResult.second )
	{
		if( ( 0 < pairFindResult.first->second.m_tBeginDate.GetTime() ) &&
			( tCurrentTime.GetTime() < pairFindResult.first->second.m_tBeginDate.GetTime() )
			)
		{
			++pairFindResult.first;
			continue;
		}

		if( ( 0 < pairFindResult.first->second.m_tEndDate.GetTime() ) &&
			( pairFindResult.first->second.m_tEndDate.GetTime() <= tCurrentTime.GetTime() )
			)
		{
			// �̹� �ð��� ������ �̺�Ʈ�� ���� �ع�����
			//pairFindResult.first = m_mmapAttractionItemTimeEventInfo.erase( pairFindResult.first );
			// ������ ����... ���� �ð��� �����ϰų� �ϴ� ��찡 �߻������� �𸥴�.
			++pairFindResult.first;
			continue;
		}

		iItemID = pairFindResult.first->second.m_iEventAttractionItem;
		break;
	}
}
#endif // SERV_ATTRACTION_ITEM_TIME_EVENT_VER2

#ifdef SERV_TIME_ENCHANT_EVENT// �۾���¥: 2013-05-30	// �ڼ���
void KGameSysVal::ResetTimeEnchantEventInfo_Lua( void ) const
{
	const int iPfID = KBaseServer::GetKObj()->GetPfID();

	if( iPfID != PI_GLOBAL_SERVER )
	{
		START_LOG( cerr, L"�۷ι� ���������� ����� �� �ֽ��ϴ�." )
			<< BUILD_LOG( iPfID )
			<< END_LOG;
		return;
	}

	KncSend( iPfID, KBaseServer::GetKObj()->GetUID(), PI_LOG_DB, 0, NULL, DBE_TIME_ENCHANT_EVENT_INFO_REQ, char() );
}
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// �۾���¥: 2013-07-03	// �ڼ���
void KGameSysVal::ResetExchangeLimitInfo_Lua( void ) const
{
	const int iPfID = KBaseServer::GetKObj()->GetPfID();

	if( iPfID != PI_GLOBAL_SERVER )
	{
		START_LOG( cerr, L"�۷ι� ���������� ����� �� �ֽ��ϴ�." )
			<< BUILD_LOG( iPfID )
			<< END_LOG;
		return;
	}

	KncSend( iPfID, KBaseServer::GetKObj()->GetUID(), PI_LOG_DB, 0, NULL, DBE_EXCHANGE_LIMIT_INFO_REQ, char() );
}
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_FIX_AFTER_WORK_STORAGE_CLASS// �۾���¥: 2013-12-21	// �ڼ���
void KGameSysVal::AfterWorkStorageMessageDeleteTerm_Second_LUA( IN int iAwsCriterionNum, IN int iAwsUnderTerm, IN int iAwsAboveOrEqualTerm )
{
	m_iAwsCriterionNum		= iAwsCriterionNum;
	m_iAwsUnderTerm			= iAwsUnderTerm;
	m_iAwsAboveOrEqualTerm	= iAwsAboveOrEqualTerm;

	START_LOG( cout, L"AfterWorkStorage ���� ���� �Ϸ�" )
		<< BUILD_LOG( m_iAwsCriterionNum )
		<< BUILD_LOG( m_iAwsUnderTerm )
		<< BUILD_LOG( m_iAwsAboveOrEqualTerm )
		;
}
#endif // SERV_FIX_AFTER_WORK_STORAGE_CLASS