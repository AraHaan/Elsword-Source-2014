#pragma once

#include <boost/timer.hpp>
#include <ToString.h>
#include "KncSmartPtr.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"


#define KStatisticsPtr KStatistics*
#undef KStatisticsPtr
SmartPointer( KStatistics );

class KStatistics
{
    DeclToStringW;
    DeclDump;

public:
#   undef _ENUM
#   define _ENUM( id ) id,
    enum
    {
#   include "StatisticsID_def.h"
    };

	// ������ ���
	enum StatInventory
	{
		SI_ITEM_TRADE = 0,		// ���ΰŷ�
		SI_ITEM_PERSONAL_SHOP,	// ���λ���
		SI_ITEM_TRADE_BOARD,	// �ŷ��Խ���
		SI_ITEM_BUY,			// ������ ����
		SI_ITEM_SELL,			// ������ �Ǹ�
		SI_ITEM_USE,			// ������ ���
		SI_ITEM_MAKE,			// ����
		SI_ITEM_FAILED,			// ���� ����
		SI_ITEM_DROP,			// ������ ���
		SI_ITEM_QUEST,			// ����Ʈ ������
		SI_ITEM_EQUIP,			// ������ ����
		SI_ITEM_RANDOM,			// ���� ť�� ���
		SI_ITEM_RESOLVE,		// ������ ���� �õ� Ƚ��
		SI_ITEM_R_RESULT,		// ������ ���� �����
		SI_ITEM_MALL,			// ĳ�� ������		
		SI_ITEM_POST,			// ��ü�� ���� ÷�� ������
		SI_ITEM_IDENTIFY,		// �Ӽ��� ����
		SI_ITEM_EXCHANGE_S,		// ������ ��ȯ [������]
		SI_ITEM_EXCHANGE_R,		// ������ ��ȯ [�ٲ��]
		//{{ 2010. 05. 20  ������	������ ������ ���
		SI_ITEM_DISCARD,		// ������ ������
		//}}
		//{{ 2010. 9. 14	������	�� �ý���
//#ifdef SERV_PET_SYSTEM
		SI_ITEM_PET_FEED,		// �� ����
//#endif SERV_PET_SYSTEM
		//}}
		//{{ 2011. 04. 26	������	�븮 ����
//#ifdef SERV_PSHOP_AGENCY
		SI_ITEM_REG_PSHOP,		// �븮���� Reg Item
		SI_ITEM_BUY_PSHOP_AGENCY, // �븮���� ������ ����
		SI_ITEM_PICK_UP_PSHOP,	// �븮���� PickUp
//#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
//#ifdef SERV_NEXON_SESSION_PROTOCOL
		SI_ITEM_RETRANINING_REWARD,		// ��Ż ���� ���� ������ ����
//#endif SERV_NEXON_SESSION_PROTOCOL
		//}} 
//#ifdef SERV_FINALITY_SKILL_SYSTEM	// ���볯¥: 2013-08-01
		SI_ITEM_EXTRACT_S,		// ������ ���� [������]
		SI_ITEM_EXTRACT_R,		// ������ ���� [�ٲ��]
		SI_ITEM_USE_FINALITY_SKILL,		// �ñر� ���	
//#endif // SERV_FINALITY_SKILL_SYSTEM

		SI_ITEM_MAX,
	};

	// ���� ���� ���
	enum StatLocalPvp
	{
		SLP_CREATE_COUNT = 0,
		SLP_UNITCLASS_ELSWORD,
		SLP_UNITCLASS_AISHA,
		SLP_UNITCLASS_RENA,
		SLP_UNITCLASS_RAVEN_FIGHTER,		
		SLP_UNITCLASS_EVE_NASOD,
		SLP_UNITCLASS_SWORD_KNIGHT,
		SLP_UNITCLASS_MAGIC_KNIGHT,
		SLP_UNITCLASS_COMBAT_RANGER,
		SLP_UNITCLASS_SNIPING_RANGER,
		SLP_UNITCLASS_HIGH_MAGICIAN,
		SLP_UNITCLASS_DARK_MAGICIAN,
		SLP_UNITCLASS_SOUL_TAKER,
		SLP_UNITCLASS_OVER_TAKER,		
		SLP_UNITCLASS_EXOTIC_GEAR,
		SLP_UNITCLASS_ARCHITECTURE,
		SLP_INTRUDER,
		SLP_GIVE_UP,
		SLP_GET_VP,
		SLP_GET_EXP,
		SLP_TEAM_MATCH_MC,
		SLP_TEAM_MATCH_PT,
		SLP_TEAM_DEATH_MC,
		SLP_TEAM_DEATH_PT,
		SLP_SURVIVAL_MC,
		SLP_SURVIVAL_PT,
//#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
		SLP_UNITCLASS_ARA,
//#endif // SERV_ARA_CHANGE_CLASS_SECOND

		SLP_MAX
	};

	// ���� �� ���
	enum ESIColumn_PvpMap
	{
		eSIColPvpMap_PlayCount = 0,

		eSIColPvpMap_Max,
	};

	// FPS ���
	enum ESIColumn_Fps
	{
		eSIColFps_0_20 = 0,
		eSIColFps_21_30,
		eSIColFps_31_40,
		eSIColFps_41_50,
		eSIColFps_51_60,
		eSIColFps_61_70,
		eSIColFps_71_80,
		eSIColFps_81_90,
		eSIColFps_91_unlimited,

		eSIColFps_Max,
	};

	// �ټ��� ���
	enum ESIColumn_Spirit
	{
		eSIColSpirit_Play = 0,
		eSIColSpirit_UseSpirit,
		eSIColSpirit_SpiritZero,

		eSIColSpirit_Max,
	};

	// ���� ��ȭ ���
	enum ESIColumn_Enchant
	{
		eSIColEnchant_Lv_1_Succ = 0,
		eSIColEnchant_Lv_1_Fail,
		eSIColEnchant_Lv_2_Succ,
		eSIColEnchant_Lv_2_Fail,
		eSIColEnchant_Lv_3_Succ,
		eSIColEnchant_Lv_3_Fail,
		eSIColEnchant_Lv_4_Succ,
		eSIColEnchant_Lv_4_Fail,
		eSIColEnchant_Lv_5_Succ,
		eSIColEnchant_Lv_5_Fail,
		eSIColEnchant_Lv_6_Succ,
		eSIColEnchant_Lv_6_Fail,
		eSIColEnchant_Lv_7_Succ,
		eSIColEnchant_Lv_7_Fail,
		eSIColEnchant_Lv_8_Succ,
		eSIColEnchant_Lv_8_Fail,
		eSIColEnchant_Lv_9_Succ,
		eSIColEnchant_Lv_9_Fail,

		eSIColEnchant_Max,
	};

	// ��Ȱ�� ��� [DB]
	enum ESIColumn_DB_ResStone
	{
		eSIColResStone_PlayerCount = 0,
		eSIColResStone_UseCount,

		eSIColResStone_Max,
	};

	// ���� ED ��� [DB]
	enum ESIColumn_DB_ED
	{
		eSIColDB_ED_PDungeon				= 0,
		eSIColDB_ED_PTraining				= 1,
		eSIColDB_ED_PQuest					= 2,
		eSIColDB_ED_PItemSell				= 3,
		eSIColDB_ED_PItemTrade				= 4,
		eSIColDB_ED_PPersonalShop			= 5,
		eSIColDB_ED_PPostLetter				= 6,
		eSIColDB_ED_MItemBuy				= 7,
		eSIColDB_ED_MItemTrade				= 8,
		eSIColDB_ED_MItemRepair				= 9,
		eSIColDB_ED_MManufacture			= 10,
		eSIColDB_ED_MDunPenalty				= 11,
		eSIColDB_ED_MPersonalShop			= 12,
		eSIColDB_ED_MOpenPShop				= 13,
		eSIColDB_ED_MEnchant				= 14,
		eSIColDB_ED_MSocket					= 15,
		eSIColDB_ED_MPostLetter				= 16,
		eSIColDB_ED_MSendLetterBasic		= 17,
		eSIColDB_ED_MSendLetterItem			= 18,
		eSIColDB_ED_MAttribEnchant			= 19,
		eSIColDB_ED_MSellTaxPShop			= 20,
		eSIColDB_ED_MBuyTaxPShop			= 21,
		//{{ 2011. 05. 11	������	�븮����
//#ifdef SERV_PSHOP_AGENCY
		eSIColDB_ED_MPShopAgency			= 22,
		eSIColDB_ED_MBuyTaxPShopAgency		= 23,
		eSIColDB_ED_PPShopPickUp			= 24,
//#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2012. 11. 26 ť�� ED ���� ���� ��� �߰� - ��μ�
//#ifdef SERV_CUBE_OPEN_ED_CONDITION
		eSIColDB_ED_POpenCube				= 25,
//#endif SERV_CUBE_OPEN_ED_CONDITION
		//}

//#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// �۾���¥: 2013-05-08	// �ڼ���
		eSIColDB_ED_PBattleField			= 26,		// ��Ʋ �ʵ忡�� ȹ���� ED
		eSIColDB_ED_MCoboExpressTicketED	= 27,		// ���� �̵����� ���� �Ҹ��� ED
//#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG
//#ifdef SERV_MULTI_RESOLVE // �ؿ��� �߰� ���� ������
		eSIColDB_ED_MItemResolve              = 28,
//#endif SERV_MULTI_RESOLVE

//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05// �۾���¥: 2013-06-17	// �ڼ���
		eSIColDB_ED_MItemEvaluate			= 29,		// ������ �������� �Ҹ��� ED
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05
        
//#ifdef SERV_GOOD_ELSWORD // �۾���¥ 2013-11-08 , ED�� �κ��丮 Ȯ��, ED �� ���� Ȯ��
        eSIColDB_ED_ExpandInventory         = 30, // �κ��丮
        eSIColDB_ED_ExpandBank              = 31, // ����
//#endif SERV_GOOD_ELSWORD

//#ifdef SERV_SKILL_PAGE_SYSTEM
		eSIColDB_ED_ExpandSkillPage         = 32, // ��ų ������
//#endif // SERV_SKILL_PAGE_SYSTEM

		eSIColDB_ED_Max,
	};

	// ����Ʈ ��� [DB]
	enum ESIColumn_DB_Quest
	{
		eSIColQuest_Accept = 0,
		eSIColQuest_GiveUp,
		eSIColQuest_Complete,
		eSIColQuest_AcceptLevel,
		eSIColQuest_GiveUpLevel,
		eSIColQuest_CompleteLevel,

		eSIColQuest_Max,
	};	

	// ���� ���� ���
	enum ESIColumn_Disconnect_Reason
	{
		eSIColDR_No_Reason							=  0,	// [0]  ���� ����
		eSIColDR_Unknown							=  1,	// [1]  Unknown
		eSIColDR_HackShield_Auth_Fail				=  2,	// [2]  HackShield ���� ���� (Ŭ���̾�Ʈ ���� ����)
		eSIColDR_PCBang_Auth_Delay					=  3,	// [3]  PC�� ���� ����
		eSIColDR_Diff_Main_Ver						=  4,	// [4]  ���ι��� �ٸ�
		eSIColDR_Limit_User_Num						=  5,	// [5]  ���� �ο� �ʰ�
		eSIColDR_Nexon_Auth_Fail					=  6,	// [6]  �ؽ� ���� ����
		eSIColDR_Same_Server_Connect				=  7,	// [7]  ���� ���� ���� ����
		eSIColDR_Account_Block						=  8,	// [8]  ���� ���
		eSIColDR_IP_Block							=  9,	// [9]  IP ����
		eSIColDR_NProtect_Auth_First				= 10,	// [10] NProtect ó�� ����
		eSIColDR_PCBang_Auth_Fail					= 11,	// [11] PC�� ���� ����
		eSIColDR_PCBang_Auth_Wrong					= 12,	// [12] PC�� ���� ��� �̻�
		eSIColDR_Internal_Account					= 13,	// [13] �系 ���� ����
		eSIColDR_Normal_Disconnect					= 14,	// [14] ���� ����
		eSIColDR_Client_Crash						= 15,	// [15] Ŭ���̾�Ʈ ũ����
		eSIColDR_Client_Hacking						= 16,	// [16] Ŭ���̾�Ʈ ��ŷ
		eSIColDR_HackShield_Wrong_Ack				= 17,	// [17] HackShield ���� ���� (���� Ʋ��)
		eSIColDR_PCBang_Auth_Server					= 18,	// [18] PC�� ���� ������ ���� ��û
		eSIColDR_Server_Shutdown					= 19,	// [19] ���� Shutdown
		eSIColDR_Session_Close						= 20,	// [20] ���� ���� ���� (Session)
		eSIColDR_WSAECONNRESET						= 21,	// [21] WSAECONNRESET (Socket Object)
		eSIColDR_WSARecvFailed						= 22,	// [22] WSARecvFailed (Socket Object)
		eSIColDR_WSASendFailed_OnSendComp			= 23,	// [23] WSASendFailed (Socket Object) - OnSendCompleted()
		eSIColDR_SendBufferFull						= 24,	// [24] SendBufferFull (Socket Object)
		eSIColDR_WSASendFailed						= 25,	// [25] WSASendFailed (Socket Object) - SendData()
		eSIColDR_GM_Kick_User						= 26,	// [26] ��� ���� ���� ����
		eSIColDR_Diff_Server_Connect				= 27,	// [27] Ÿ ������ ��������
		eSIColDR_User_Reg_Error						= 28,	// [28] ���� ��� �� ����
		eSIColDR_Heart_Beat							= 29,	// [29] ��Ʈ ��Ʈ
		eSIColDR_IOCP_GetStatus						= 30,	// [30] IOCP_GetStatus Closed by local machine
		eSIColDR_nProtect_Hacking					= 31,	// [31] nProtect ��ŷ ����
		eSIColDR_Channel_Change						= 32,	// [32] ä���̵����� ���� ��������
		eSIColDR_Channel_Change_Fail				= 33,	// [33] ä���̵� ���з� ���� ��������
		//{{ 2009. 10. 22  ������	Ʈ���� ���� ����
		eSIColDR_Traffic_Attack						= 34,	// [34] Ʈ���� ���߷� ���� ��������
		//}}
		//{{ 2010. 06. 22  ������	���������
		eSIColDR_GameServer_Account_Block			= 35,	// [35] ���Ӽ��� ���� ���
		//}}
		//{{ 2011. 08. 09	������	�ӽ�ID �����
//#ifdef SERV_MACHINE_ID_BLOCK
		eSIColDR_MachineID_Block					= 36,	// [36] �ӽ�ID ��
//#endif SERV_MACHINE_ID_BLOCK
		//}}
		//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
//#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
		eSIColDR_Same_MachineID_Connect				= 37,	// [37] �ӽ�ID �ߺ� ����
//#endif SERV_MACHINE_ID_DUPLICATE_CHECK
		//}}
		//{{ 2011. 09. 29  ��μ�	�˴ٿ��� ����
//#ifdef SERV_SHUTDOWN_SYSTEM
		eSIColDR_Policy_Shutdown_Disconnect			= 38,	// [38] �˴ٿ����� ���� ���� �Ұ�
//#endif SERV_SHUTDOWN_SYSTEM
		//}}
		eSlColDR_Wrong_Account_or_Password			= 39,	// [39] ���̵� �Ǵ� ��й�ȣ Ʋ�� (TW)
		//{{ ����� : [2011/2/16/] //	SHA üũ��
		//#ifdef SERV_USE_CHECKSUM_WITH_SHA
		eSlColDR_Wrong_SHA_Checksum					= 40,	// [40] SHA üũ�� Ʋ�� (TW)
		//#endif SERV_USE_CHECKSUM_WITH_SHA
		//}} ����� : [2011/2/16/] //	SHA üũ��

		eSIColDR_Account_Invalid_Selling_Kill		= 41,	// [41] �ȱ�Ҵ� �������� �Ǵٰ� ��Ŷ������ ������ ���� ���� (TW) 
		eSIColDR_Clear_Time_Hack_User				= 42,	// [42] Ŭ����Ÿ�� 5������ �� �ǽ� ������ �Ǻ� (TW)

		//{{ 2011.03.20 lygan_������ // HProtect
		//#ifdef SERV_USE_HPROTECT
		eSIColDR_Error_Check_Alive1					= 43,	// [43] A type heartbeat validation error
		eSIColDR_Error_Check_Alive2					= 44,	// [44] B type heartbeat validation error
		eSIColDR_Error_Wait_Check_Time_Out			= 45,	// [45] The client did not respond to packet too long
		eSIColDR_Error_Client_Speed					= 46,	// [46] Client speed abnormally
		eSIColDR_Error_Client_Scan_Result			= 47,	// [47] wParam point to TOOLSSCANRESULT
		eSIColDR_Error_Client_Version				= 48,	// [48] unsigned int wParam type allows for the smallest version of the type unsigned int lParam is the client version number
		eSIColDR_Error_Server_Lost_File				= 49,	// [49] wParam point to file path
		eSIColDR_Error_Client_Protocol				= 50,	// [50] Agreement send data packets when checking failure
		eSIColDR_Error_Client_Sign_Hash				= 51,	// [51] Ŭ���̾�Ʈ Ư¡ ���Ͽ� ����������
		eSIColDR_Error_Wait_Client_Version_Time_Out = 52,	// [52] In 2 minutes, fail to get the client version, unable to initialize the protection system
		//#endif SERV_USE_HPROTECT
		//}}
		//{{  2012. 1. 16	��μ�		���Ӽ������� Server SN �˻�
		//#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		eSIColDR_Check_ServerSN_In_GameServer		= 53,	// [53] ���Ӽ������� ServerSN �˻�� ���Ͽ� ���� ����
		//#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		//}}
		//{{ 2012. 02. 01	������	recv buffer full
//#ifdef SERV_CHECK_RECV_BUFFER_FULL
		eSIColDR_RecvBufferFull						= 54,	// [54] ���ŵ� ��Ŷ�� ���̰� recv bufferũ�⸦ �Ѿ�� ���
//#endif SERV_CHECK_RECV_BUFFER_FULL
		//}}		
		//{{ 2012. 02. 14    ��μ�   �ؽ� �Ƚ� �α��� - ��ǥ ��ġ��� �α���
//#ifdef SERV_NEXON_PEACE_OF_MIND_LOGIN
		eSIColDR_PeaceOfMindLogin					= 55,	// [55] �ؽ� �Ƚ� �α������� �� ó���� ����
//#endif SERV_NEXON_PEACE_OF_MIND_LOGIN
		//}}
		//{{ 2012. 05. 23	��μ�       �Ǹ� �Ұ� ������ �Ǹ� �õ� ��ŷ ������ �Ǵ�
//#ifdef SERV_IMPOSSIBLE_SELL_ITEM
		eSIColDR_ImpossibleSellItem					= 56,	// [56] �Ǹ� �Ұ� ������ �ȱ�
//#endif SERV_IMPOSSIBLE_SELL_ITEM
		//}}
//#ifdef SERV_NPROTECT_CS_AUTH_30
		eSIColDR_nProtect_Error                     = 57,    // [57] nProtect ����
//#endif SERV_NPROTECT_CS_AUTH_30
//{{
//#ifdef SERV_MOVE_DATA_QUE2BUF_FAIL  
		eSIColDR_MovDataQue2Buf_Failed              = 58,    // [58] MovDataQue2Buf �κп��� ���������� ���� ũ��� ���� ���Ӽ��� ũ���� ���� �κ� ����
//#endif //SERV_MOVE_DATA_QUE2BUF_FAIL
//}}
//#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-06	// �ڼ���
		eSIColDR_LanBugOutDisconnect				= 59,    // [59] ������ ���ÿ� �ɷ������� �̸� �����Ͽ� ���� ���� ó���� ���
//#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

//#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-06	// �ڼ���
		eSIColDR_FieldWorkingCheck					= 60,    // [60] �ʵ� �۾��� üũ ���
//#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

//#ifdef SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT // �۾���¥: 2013-09-04 // ������
		eSIColDR_InvlideCheckKomScript				= 61,    // [61] CheckKom.xml ��ũ��Ʈ �Ľ� ����
//#endif // SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT

//#ifdef SERV_KOM_FILE_CHECK_ADVANCED		// �۾���¥: 2013-11-06 // ��ȫ��
		eSIColDR_InvaildKomDetect					= 62,   // [62] Kom ���� ����
//#endif SERV_KOM_FILE_CHECK_ADVANCED

		eSIColDR_Num,
	};

	// ���� ���� ���
	enum ESIColumn_Disconnect_UserFSM
	{		
		eSIColDU_S_INIT = 0,
		eSIColDU_S_UID_UPDATED,
		eSIColDU_S_LOGINED,
		eSIColDU_S_SERVER_SELECT,
		eSIColDU_S_UNIT_SELECT,
		eSIColDU_S_VILLAGE_MAP,
		eSIColDU_S_LOCAL_MAP,
		eSIColDU_S_WORLD_MAP,
		eSIColDU_S_CHANNEL,
		eSIColDU_S_ROOM,
		eSIColDU_S_CHECK_SECOND_PW,

		eSIColDU_Num,
	};

	//{{ 2011. 02. 28	������	�系 ���� ���
#ifdef SERV_CHAR_CREATE_DEL_STAT
	// ĳ���� ���
	enum ESIColumn_DB_Character
	{
		eSIColDB_Char_Create_Count = 0,
		eSIColDB_Char_Delete_Count,

		eSIColDB_Num,
	};
#endif SERV_CHAR_CREATE_DEL_STAT
	//}}

	// - - - - - - - - - - - - - - - - - - - - - - - - -

    KStatistics();
    virtual ~KStatistics();

    float GetFlushGap()             { return m_fFlushGap; }
    bool  IsWriteToDB()             { return m_bWriteToDB; }
    static const wchar_t* GetIDStr( int iStatisticsID );

	void Init( int iStatisticsID, float fFlushGap, bool bWriteToDB );
	//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
	void SetFlushTime( IN const int iHour );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

    void Tick();

    void IncreaseCount( const KStatisticsKey& kKey, int iIndex, int iCount );
    void IncreaseCount( const KStatisticsKey& kKey, const std::vector< __int64 >& vecCount );
    void FlushData();
	void WriteLocalLogData( std::wfstream& fout );	
	void ClearLocalLogData();

	void InitLocalLogKeys();

	static int GetDisconnectUserFSM( int iStateID );
	static bool GetUnitClassPvpStatColumn( char cUnitClass, int& iColumn );
	static const wchar_t* GetDissconnectReasonString( int iReason );

protected:
	unsigned short GetEventID();

protected:
    int m_iStatisticsID;
    KStatisticsData m_kStatisticsData;
    boost::timer m_kUpdateTimer;
    float m_fFlushGap;
    bool m_bWriteToDB;
	//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
	bool m_bTimeFlush;
	CTime m_tFlushTime;
	boost::timer m_kTimeFlushTimer;
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

	static const wchar_t* ms_szStatistics[];
};



