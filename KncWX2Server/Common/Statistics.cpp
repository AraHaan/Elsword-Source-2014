#include "Statistics.h"

#include <Serializer/Serializer.h>
#include <Serializer/SerMap.h>
#include "KncSend.h"

#include "X2Data/XSLItem.h"
#include "X2Data/XSLItemManager.h"
#include "GSFSM.h"

#include <iomanip>

#undef _ENUM
#define _ENUM( id ) L#id,
const wchar_t* KStatistics::ms_szStatistics[] =
{
#include "StatisticsID_def.h"
};

ImplToStringW( KStatistics )
{
    stm_ << L"-> " << GetIDStr( m_iStatisticsID ) << std::endl;

    KStatisticsData::const_iterator mit;
    for( mit = m_kStatisticsData.begin(); mit != m_kStatisticsData.end(); mit++ )
    {
        std::vector< int >::const_iterator vitInt;
        std::vector< std::wstring >::const_iterator vitStr;
		std::vector< __int64 >::const_iterator vitInt64;

        for( vitInt = mit->first.m_vecIntKey.begin(); vitInt != mit->first.m_vecIntKey.end(); vitInt++ )
        {
            stm_ << *vitInt << L" ";
        }
        for( vitStr = mit->first.m_vecStringKey.begin(); vitStr != mit->first.m_vecStringKey.end(); vitStr++ )
        {
            stm_ << *vitStr << L" ";
        }
        for( vitInt64 = mit->second.begin(); vitInt64 != mit->second.end(); vitInt64++ )
        {
            stm_ << *vitInt64 << L" ";
        }
        stm_ << std::endl;
    }

    return stm_;
}

KStatistics::KStatistics()
{
	//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
	m_bTimeFlush = false;
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}
}

KStatistics::~KStatistics()
{
}

const wchar_t* KStatistics::GetIDStr( int iStatistics )
{
    if( iStatistics < 0 || iStatistics > SI_SENTINEL )
    {
        return ms_szStatistics[SI_SENTINEL];
    }

    return ms_szStatistics[iStatistics];
}

void KStatistics::Init( int iStatisticsID, float fFlushGap, bool bWriteToDB )
{
    m_iStatisticsID = iStatisticsID;
    m_kStatisticsData.clear();
    m_kUpdateTimer.restart();
    m_fFlushGap = fFlushGap;
    m_bWriteToDB = bWriteToDB;
}

//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
void KStatistics::SetFlushTime( IN const int iHour )
{
	m_bTimeFlush = true;

	CTime tCurTime = CTime::GetCurrentTime();
	m_tFlushTime = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), iHour, 0, 0, 0 );
	if( m_tFlushTime < tCurTime )
	{
		m_tFlushTime += CTimeSpan( 1, 0, 0, 0 );
	}

	std::wstring wstrFlushTime = (const wchar_t*)m_tFlushTime.Format(_T("%Y-%m-%d %H:%M:%S"));

	START_LOG( cout, L"Ư�� �ð��� DB��� Flush �ð� ����!" )
		<< BUILD_LOG( m_iStatisticsID )
		<< BUILD_LOG( wstrFlushTime )
		<< BUILD_LOG( m_bTimeFlush );
}
#endif SERV_ITEM_STATISTICS_TO_DB
//}}

void KStatistics::Tick()
{
	//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
	if( m_bTimeFlush )
	{
		// 1�и��� Flush�ð��� üũ�մϴ�.
		if( m_kTimeFlushTimer.elapsed() < 60.0 )
			return;

		m_kTimeFlushTimer.restart();

		// Flush�ð��� üũ�غ��ϴ�.
		CTime tCurTime = CTime::GetCurrentTime();
		if( tCurTime < m_tFlushTime )
			return;

		// ������¥�� ����!
		m_tFlushTime += CTimeSpan( 1, 0, 0, 0 );
        
		// Flush
		FlushData();
		return;
	}
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

    if( m_fFlushGap >= 0.f )
    {
        if( m_kUpdateTimer.elapsed() >= m_fFlushGap )
        {
            FlushData();
        }
    }
}

void KStatistics::IncreaseCount( const KStatisticsKey& kKey, int iIndex, int iCount )
{
    std::vector< __int64 > vecCount;
    for( int i = 0; i < iIndex; i++ )
    {
        vecCount.push_back( 0 );
    }
    vecCount.push_back( iCount );

    IncreaseCount( kKey, vecCount );
}

void KStatistics::IncreaseCount( const KStatisticsKey& kKey, const std::vector< __int64 >& vecCount )
{
    KStatisticsData::iterator mit = m_kStatisticsData.find( kKey );
    if( mit == m_kStatisticsData.end() )
    {
        m_kStatisticsData.insert( std::make_pair( kKey, vecCount ) );
    }
    else
    {
        std::vector< __int64 >::const_iterator vitSrc = vecCount.begin();
        std::vector< __int64 >::iterator vitDest = mit->second.begin();

        while( vitSrc != vecCount.end() )
        {
            if( vitDest == mit->second.end() )
            {
                while( vitSrc != vecCount.end() )
                {
                    mit->second.push_back( *vitSrc );
                    vitSrc++;
                }
            }
            else
            {
                ( *vitDest ) += ( *vitSrc );
                vitSrc++;
                vitDest++;
            }
        }
    }
}

void KStatistics::FlushData()
{
	switch ( m_iStatisticsID )
	{
	case KStatistics::SI_LOC_PVP:
	case KStatistics::SI_LOC_ITEM:
	case KStatistics::SI_LOC_DUNGEON:
	case KStatistics::SI_LOC_SPIRIT:
	case KStatistics::SI_LOC_ENCHANT:		
	case KStatistics::SI_LOC_PVP_VICTORY_PER:		
	case KStatistics::SI_LOC_PVP_ROOM:
	case KStatistics::SI_LOC_DUNGEON_ROOM:
	case KStatistics::SI_LOC_HENIR_RANKING:
	case KStatistics::SI_LOC_TITLE:
		//{{ 2010. 9. 14	������	�� �ý���
#ifdef SERV_PET_SYSTEM
	case KStatistics::SI_LOC_PET_SUMMON:
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2010. 10. 11	������	������ ���� ���� �α�
#ifdef SERV_SERVER_DISCONNECT_LOG
	case KStatistics::SI_LOC_SERV_DISCONNECT:
#endif SERV_SERVER_DISCONNECT_LOG
		//}}
		//{{ 2010. 10. 11	������	�ؽ� ���� ��Ŷ �α�
#ifdef SERV_BILLING_PACKET_LOG
	case KStatistics::SI_LOC_BILLING_PACKET:
#endif SERV_BILLING_PACKET_LOG
		//}}
		//{{ 2010. 11. 17	������	������ ���� ���
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	case KStatistics::SI_LOC_ABUSER_MORNITORING:
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
		//}}
#ifdef SERV_LOG_UNDEFINED_QUEST_TEMPLET
	case KStatistics::SI_LOC_UNDEFINED_QUEST_TEMPLET:
#endif // SERV_LOG_UNDEFINED_QUEST_TEMPLET

		return;
	}

	m_kUpdateTimer.restart();
	if( !m_kStatisticsData.empty() )
	{
		if( m_bWriteToDB )
		{
			KncSend( PC_SERVER, 0, PC_LOG_DB, 0, NULL, GetEventID(), m_kStatisticsData );
		}

		m_kStatisticsData.clear();
	}
}

void KStatistics::ClearLocalLogData()
{
	switch ( m_iStatisticsID )
	{
	case KStatistics::SI_LOC_PVP:
	case KStatistics::SI_LOC_ITEM:
	case KStatistics::SI_LOC_SPIRIT:		
	case KStatistics::SI_LOC_ENCHANT:
		{
			m_kUpdateTimer.restart();
			m_kStatisticsData.clear();
		}
		break;
	}
}

unsigned short KStatistics::GetEventID()
{
	switch( m_iStatisticsID )
	{
	case SI_PVP_MAP:
		return ELOG_STAT_PVP_MAP;
	case SI_FPS:
		return ELOG_STAT_FPS;
	case SI_RES_STONE:
		return ELOG_STAT_RES_STONE;
	case SI_ED:
		return ELOG_STAT_ED;
	case SI_QUEST:
		return ELOG_STAT_QUEST;
	case SI_DISCONNECT_REASON:
		return ELOG_STAT_DISCONNECT_REASON;
	case SI_DISCONNECT_USERFSM:
		return ELOG_STAT_DISCONNECT_USERFSM;
		//{{ 2011. 02. 28	������	�系 ���� ���
#ifdef SERV_CHAR_CREATE_DEL_STAT
	case SI_CHARACTER:
		return ELOG_STAT_CHARACTER;
#endif SERV_CHAR_CREATE_DEL_STAT
		//}}
		//{{ 2011. 04. 13	������	������ ��� DB���
#ifdef SERV_ITEM_STATISTICS_TO_DB
	case SI_ITEM_DB:
		return ELOG_STAT_ITEM_DB;
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}	

	default:
		START_LOG( cerr, L"��� ID�� ���� event id�� �������� �ʾ���." )
			<< BUILD_LOG( m_iStatisticsID )
			<< END_LOG;
		return EVENT_EVENTID_SENTINEL;
	}
}

void KStatistics::WriteLocalLogData( std::wfstream& fout )
{
	// �ִ� �÷���
	int iMaxColNum = 0;
	int iCurColNum = 0;

	KStatisticsData::const_iterator mit;
	for( mit = m_kStatisticsData.begin(); mit != m_kStatisticsData.end(); ++mit )
	{
		int iColCnt = 0;

		std::vector< __int64 >::const_iterator vitInt;
		for( vitInt = mit->second.begin(); vitInt != mit->second.end(); ++vitInt )
		{
			iColCnt++;
		}

		iMaxColNum = max( iMaxColNum, iColCnt );
	} 

	for( mit = m_kStatisticsData.begin(); mit != m_kStatisticsData.end(); ++mit )
	{
		std::vector< int >::const_iterator vitInt;
		std::vector< std::wstring >::const_iterator vitStr;

		fout << L"\n";

		// Key ���
		for( vitInt = mit->first.m_vecIntKey.begin(); vitInt != mit->first.m_vecIntKey.end(); ++vitInt )
		{
			fout << std::setw(8) << std::setiosflags( std::ios::left ) << *vitInt << L" ";
		}
		for( vitStr = mit->first.m_vecStringKey.begin(); vitStr != mit->first.m_vecStringKey.end(); ++vitStr )
		{
			fout << std::setw(8) << std::setiosflags( std::ios::left ) << *vitStr << L" ";
		}

		fout << L"\t";

		// Data ���
		iCurColNum = iMaxColNum;

		std::vector< __int64 >::const_iterator vitInt64;

		for( vitInt64 = mit->second.begin(); vitInt64 != mit->second.end(); ++vitInt64 )
		{
			fout << std::setw(8) << std::setiosflags( std::ios::left ) << *vitInt64 << L"\t";
			iCurColNum--;
		}

		for( int iC = 0; iC < iCurColNum; ++iC )
		{
			fout << std::setw(8) << std::setiosflags( std::ios::left ) << 0 << L"\t";
		}
	}
}

void KStatistics::InitLocalLogKeys()
{
	if ( m_iStatisticsID == KStatistics::SI_LOC_ITEM )
	{
		const std::map< int, CXSLItem::ItemTemplet >& mapItemTemplet = SiCXSLItemManager()->GetItemTempletContainer();
		std::map< int, CXSLItem::ItemTemplet >::const_iterator mit;
		for ( mit = mapItemTemplet.begin(); mit != mapItemTemplet.end(); ++mit )
		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( mit->first );
			IncreaseCount( kKey, KStatistics::SI_ITEM_MAX - 1, 0 );
		}
	}	
}

int  KStatistics::GetDisconnectUserFSM( int iStateID )
{
	switch( iStateID ) 
	{
	case KGSFSM::S_INIT:
		return eSIColDU_S_INIT;

	case KGSFSM::S_UID_UPDATED:
		return eSIColDU_S_UID_UPDATED;

	case KGSFSM::S_LOGINED:
		return eSIColDU_S_LOGINED;

	case KGSFSM::S_SERVER_SELECT:
		return eSIColDU_S_SERVER_SELECT;

	case KGSFSM::S_CHECK_SECOND_PW:
		return eSIColDU_S_CHECK_SECOND_PW;

	case KGSFSM::S_FIELD_MAP:
		return eSIColDU_S_VILLAGE_MAP; // �̰� ���߿� �����ؾߵ�

	case KGSFSM::S_ROOM:
		return eSIColDU_S_ROOM;
	}

	START_LOG( cerr, L"�������� �ʴ� GSFSM���Դϴ�" )
		<< BUILD_LOG( iStateID )
		<< END_LOG;

	return eSIColDU_Num;
}

bool KStatistics::GetUnitClassPvpStatColumn( char cUnitClass, int& iColumn )
{
	switch( cUnitClass )
	{
	case CXSLUnit::UC_ELSWORD_SWORDMAN:		iColumn = KStatistics::SLP_UNITCLASS_ELSWORD;			return true;
	case CXSLUnit::UC_ARME_VIOLET_MAGE:		iColumn = KStatistics::SLP_UNITCLASS_AISHA;				return true;
	case CXSLUnit::UC_LIRE_ELVEN_RANGER:	iColumn = KStatistics::SLP_UNITCLASS_RENA;				return true;
	case CXSLUnit::UC_RAVEN_FIGHTER:		iColumn = KStatistics::SLP_UNITCLASS_RAVEN_FIGHTER;		return true;			
	case CXSLUnit::UC_EVE_NASOD:			iColumn = KStatistics::SLP_UNITCLASS_EVE_NASOD;			return true;
	case CXSLUnit::UC_ELSWORD_KNIGHT:		iColumn = KStatistics::SLP_UNITCLASS_SWORD_KNIGHT;		return true;
	case CXSLUnit::UC_ELSWORD_MAGIC_KNIGHT:	iColumn = KStatistics::SLP_UNITCLASS_MAGIC_KNIGHT;		return true;
	case CXSLUnit::UC_LIRE_COMBAT_RANGER:	iColumn = KStatistics::SLP_UNITCLASS_COMBAT_RANGER;		return true;
	case CXSLUnit::UC_LIRE_SNIPING_RANGER:	iColumn = KStatistics::SLP_UNITCLASS_SNIPING_RANGER;	return true;
	case CXSLUnit::UC_ARME_HIGH_MAGICIAN:	iColumn = KStatistics::SLP_UNITCLASS_HIGH_MAGICIAN;		return true;
	case CXSLUnit::UC_ARME_DARK_MAGICIAN:	iColumn = KStatistics::SLP_UNITCLASS_DARK_MAGICIAN;		return true;
	case CXSLUnit::UC_RAVEN_SOUL_TAKER:		iColumn = KStatistics::SLP_UNITCLASS_SOUL_TAKER;		return true;
	case CXSLUnit::UC_RAVEN_OVER_TAKER:		iColumn = KStatistics::SLP_UNITCLASS_OVER_TAKER;		return true;		
	case CXSLUnit::UC_EVE_EXOTIC_GEAR:		iColumn = KStatistics::SLP_UNITCLASS_EXOTIC_GEAR;		return true;
	case CXSLUnit::UC_EVE_ARCHITECTURE:		iColumn = KStatistics::SLP_UNITCLASS_ARCHITECTURE;		return true;
		//{{ 2009. 10. 24  ������	���ҵ�2������
	case CXSLUnit::UC_ELSWORD_LORD_KNIGHT:	iColumn = KStatistics::SLP_UNITCLASS_ELSWORD;			return true;
	case CXSLUnit::UC_ELSWORD_RUNE_SLAYER:	iColumn = KStatistics::SLP_UNITCLASS_ELSWORD;			return true;
		//}}
		//{{ 2009. 11. 23  ������	����2������
	case CXSLUnit::UC_LIRE_WIND_SNEAKER:	iColumn = KStatistics::SLP_UNITCLASS_RENA;				return true;
	case CXSLUnit::UC_LIRE_GRAND_ARCHER:	iColumn = KStatistics::SLP_UNITCLASS_RENA;				return true;
		//}}
		//{{ 2009. 12. 11  ������	���̻� 2�� ����
	case CXSLUnit::UC_ARME_ELEMENTAL_MASTER:	iColumn = KStatistics::SLP_UNITCLASS_AISHA;			return true;
	case CXSLUnit::UC_ARME_VOID_PRINCESS:	iColumn = KStatistics::SLP_UNITCLASS_AISHA;				return true;
		//}}
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CXSLUnit::UC_ELSWORD_INFINITY_SWORD:	iColumn = KStatistics::SLP_UNITCLASS_ELSWORD;		return true;
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case CXSLUnit::UC_ARME_DIMENSION_WITCH:	iColumn = KStatistics::SLP_UNITCLASS_AISHA;				return true;
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
	case CXSLUnit::UC_LIRE_NIGHT_WATCHER:	iColumn = KStatistics::SLP_UNITCLASS_RENA;				return true;
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CXSLUnit::UC_RAVEN_VETERAN_COMMANDER:	iColumn = KStatistics::SLP_UNITCLASS_RAVEN_FIGHTER;	return true;
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
	case CXSLUnit::UC_EVE_BATTLE_SERAPH:	iColumn = KStatistics::SLP_UNITCLASS_EVE_NASOD;		return true;
#endif
	}

	return false;
}

const wchar_t* KStatistics::GetDissconnectReasonString( int iReason )
{
	switch( iReason )
	{
	case eSIColDR_No_Reason:							return L"No_Reason [���� ����]";
	case eSIColDR_Unknown:								return L"Unknown [Unknown]";
	case eSIColDR_HackShield_Auth_Fail:					return L"HackShield_Auth_Fail [HackShield ���� ���� (Ŭ���̾�Ʈ ���� ����)]";
	case eSIColDR_PCBang_Auth_Delay:					return L"PCBang_Auth_Delay [PC�� ���� ����]";
	case eSIColDR_Diff_Main_Ver:						return L"Diff_Main_Ver [���ι��� �ٸ�]";
	case eSIColDR_Limit_User_Num:						return L"Limit_User_Num [���� �ο� �ʰ�]";
	case eSIColDR_Nexon_Auth_Fail:						return L"Nexon_Auth_Fail [�ؽ� ���� ����]";
	case eSIColDR_Same_Server_Connect:					return L"Same_Server_Connect [���� ���� ���� ����]";
	case eSIColDR_Account_Block:						return L"Account_Block [���� ���]";
	case eSIColDR_IP_Block:								return L"IP_Block [IP ����]";
	case eSIColDR_NProtect_Auth_First:					return L"NProtect_Auth_First [NProtect ó�� ����]";
	case eSIColDR_PCBang_Auth_Fail:						return L"PCBang_Auth_Fail [PC�� ���� ����]";
	case eSIColDR_PCBang_Auth_Wrong:					return L"PCBang_Auth_Wrong [PC�� ���� ��� �̻�]";
	case eSIColDR_Internal_Account:						return L"Internal_Account [�系 ���� ����]";
	case eSIColDR_Normal_Disconnect:					return L"Normal_Disconnect [���� ����]";
	case eSIColDR_Client_Crash:							return L"Client_Crash [Ŭ���̾�Ʈ ũ����]";
	case eSIColDR_Client_Hacking:						return L"Client_Hacking [Ŭ���̾�Ʈ ��ŷ]";
	case eSIColDR_HackShield_Wrong_Ack:					return L"HackShield_Wrong_Ack [HackShield ���� ���� (���� Ʋ��)]";
	case eSIColDR_PCBang_Auth_Server:					return L"PCBang_Auth_Server [PC�� ���� ������ ���� ��û]";
	case eSIColDR_Server_Shutdown:						return L"Server_Shutdown [���� Shutdown]";
	case eSIColDR_Session_Close:						return L"Session_Close [���� ���� ���� (Session)]";
	case eSIColDR_WSAECONNRESET:						return L"WSAECONNRESET [WSAECONNRESET (Socket Object)]";
	case eSIColDR_WSARecvFailed:						return L"WSARecvFailed [WSARecvFailed (Socket Object)]";
	case eSIColDR_WSASendFailed_OnSendComp:				return L"WSASendFailed_OnSendComp [WSASendFailed (Socket Object) - OnSendCompleted()]";
	case eSIColDR_SendBufferFull:						return L"SendBufferFull [SendBufferFull (Socket Object)]";
	case eSIColDR_WSASendFailed:						return L"WSASendFailed [WSASendFailed (Socket Object) - SendData()]";
	case eSIColDR_GM_Kick_User:							return L"GM_Kick_User [��� ���� ���� ����]";
	case eSIColDR_Diff_Server_Connect:					return L"Diff_Server_Connect [Ÿ ������ ��������]";
	case eSIColDR_User_Reg_Error:						return L"User_Reg_Error [���� ��� �� ����]";
	case eSIColDR_Heart_Beat:							return L"Heart_Beat [��Ʈ ��Ʈ]";
	case eSIColDR_IOCP_GetStatus:						return L"IOCP_GetStatus [IOCP_GetStatus Closed by local machine]";
	case eSIColDR_nProtect_Hacking:						return L"nProtect_Hacking [nProtect ��ŷ ����]";
	case eSIColDR_Channel_Change:						return L"Channel_Change [ä���̵����� ���� ��������]";
	case eSIColDR_Channel_Change_Fail:					return L"Channel_Change_Fail [ä���̵� ���з� ���� ��������]";
		//{{ 2009. 10. 22  ������	Ʈ���� ���� ����
	case eSIColDR_Traffic_Attack:						return L"Traffic_Attack [Ʈ���� ���߷� ���� ��������]";
		//}}
		//{{ 2010. 06. 22  ������	���������
	case eSIColDR_GameServer_Account_Block:				return L"GameServer_Account_Block [���Ӽ��� ���� ���]";
		//}}
		//{{ 2011. 08. 09	������	�ӽ�ID �����
		//#ifdef SERV_MACHINE_ID_BLOCK
	case eSIColDR_MachineID_Block:						return L"MachineID_Block [�ӽ�ID ��]";
		//#endif SERV_MACHINE_ID_BLOCK
		//}}
		//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
		//#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	case eSIColDR_Same_MachineID_Connect:				return L"Same_MachineID_Connect [�ӽ�ID �ߺ� ����]";
		//#endif SERV_MACHINE_ID_DUPLICATE_CHECK
		//}}
		//{{ 2011. 09. 29  ��μ�	�˴ٿ��� ����
		//#ifdef SERV_SHUTDOWN_SYSTEM
	case eSIColDR_Policy_Shutdown_Disconnect:			return L"Policy_Shutdown_Disconnect [�˴ٿ����� ���� ���� �Ұ�]";
		//#endif SERV_SHUTDOWN_SYSTEM
		//}}
	case eSlColDR_Wrong_Account_or_Password:			return L"Wrong_Account_or_Password [���̵� �Ǵ� ��й�ȣ Ʋ�� (TW)]";
		//{{ ����� : [2011/2/16/] //	SHA üũ��
		//#ifdef SERV_USE_CHECKSUM_WITH_SHA
	case eSlColDR_Wrong_SHA_Checksum:					return L"Wrong_SHA_Checksum [SHA üũ�� Ʋ�� (TW)]";
		//#endif SERV_USE_CHECKSUM_WITH_SHA
		//}} ����� : [2011/2/16/] //	SHA üũ��

	case eSIColDR_Account_Invalid_Selling_Kill:			return L"Account_Invalid_Selling_Kill [�ȱ�Ҵ� �������� �Ǵٰ� ��Ŷ������ ������ ���� ���� (TW)]";
	case eSIColDR_Clear_Time_Hack_User:					return L"Clear_Time_Hack_User [Ŭ����Ÿ�� 5������ �� �ǽ� ������ �Ǻ� (TW)]";

		//{{ 2011.03.20 lygan_������ // HProtect
		//#ifdef SERV_USE_HPROTECT
	case eSIColDR_Error_Check_Alive1:					return L"Error_Check_Alive1 [A type heartbeat validation error]";
	case eSIColDR_Error_Check_Alive2:					return L"Error_Check_Alive2 [B type heartbeat validation error]";
	case eSIColDR_Error_Wait_Check_Time_Out:			return L"Error_Wait_Check_Time_Out [The client did not respond to packet too long]";
	case eSIColDR_Error_Client_Speed:					return L"Error_Client_Speed [Client speed abnormally]";
	case eSIColDR_Error_Client_Scan_Result:				return L"Error_Client_Scan_Result [wParam point to TOOLSSCANRESULT]";
	case eSIColDR_Error_Client_Version:					return L"Error_Client_Version [unsigned int wParam type allows for the smallest version of the type unsigned int lParam is the client version number]";
	case eSIColDR_Error_Server_Lost_File:				return L"Error_Server_Lost_File [wParam point to file path]";
	case eSIColDR_Error_Client_Protocol:				return L"Error_Client_Protocol [Agreement send data packets when checking failure]";
	case eSIColDR_Error_Client_Sign_Hash:				return L"Error_Client_Sign_Hash [Ŭ���̾�Ʈ Ư¡ ���Ͽ� ����������]";
	case eSIColDR_Error_Wait_Client_Version_Time_Out:	return L"Error_Wait_Client_Version_Time_Out [In 2 minutes, fail to get the client version, unable to initialize the protection system]";
		//#endif SERV_USE_HPROTECT
		//}}
		//{{  2012. 1. 16	��μ�		���Ӽ������� Server SN �˻�
		//#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	case eSIColDR_Check_ServerSN_In_GameServer:			return L"Check_ServerSN_In_GameServer [���Ӽ������� ServerSN �˻�� ���Ͽ� ���� ����]";
		//#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		//}}
		//{{ 2012. 02. 01	������	recv buffer full
		//#ifdef SERV_CHECK_RECV_BUFFER_FULL
	case eSIColDR_RecvBufferFull:						return L"RecvBufferFull [���ŵ� ��Ŷ�� ���̰� recv bufferũ�⸦ �Ѿ�� ���]";
		//#endif SERV_CHECK_RECV_BUFFER_FULL
		//}}		
		//{{ 2012. 02. 14    ��μ�   �ؽ� �Ƚ� �α��� - ��ǥ ��ġ��� �α���
		//#ifdef SERV_NEXON_PEACE_OF_MIND_LOGIN
	case eSIColDR_PeaceOfMindLogin:						return L"PeaceOfMindLogin [�ؽ� �Ƚ� �α������� �� ó���� ����]";
		//#endif SERV_NEXON_PEACE_OF_MIND_LOGIN
		//}}
		//{{ 2012. 05. 23	��μ�       �Ǹ� �Ұ� ������ �Ǹ� �õ� ��ŷ ������ �Ǵ�
		//#ifdef SERV_IMPOSSIBLE_SELL_ITEM
	case eSIColDR_ImpossibleSellItem:					return L"ImpossibleSellItem [�Ǹ� �Ұ� ������ �ȱ�]";
		//#endif SERV_IMPOSSIBLE_SELL_ITEM
		//}}
		//#ifdef SERV_NPROTECT_CS_AUTH_30
	case eSIColDR_nProtect_Error:						return L"nProtect_Error [nProtect ����]";
		//#endif SERV_NPROTECT_CS_AUTH_30
		//{{
		//#ifdef SERV_MOVE_DATA_QUE2BUF_FAIL  
	case eSIColDR_MovDataQue2Buf_Failed:				return L"MovDataQue2Buf_Failed [MovDataQue2Buf �κп��� ���������� ���� ũ��� ���� ���Ӽ��� ũ���� ���� �κ� ����]";
		//#endif //SERV_MOVE_DATA_QUE2BUF_FAIL
		//}}
		//#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-06	// �ڼ���
	case eSIColDR_LanBugOutDisconnect:					return L"LanBugOutDisconnect [������ ���ÿ� �ɷ������� �̸� �����Ͽ� ���� ���� ó���� ���]";
		//#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

		//#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// �۾���¥: 2013-05-06	// �ڼ���
	case eSIColDR_FieldWorkingCheck:					return L"FieldWorkingCheck [�ʵ� �۾��� üũ ���]";
		//#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

		//#ifdef SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT // �۾���¥: 2013-09-04 // ������
	case eSIColDR_InvlideCheckKomScript:				return L"InvlideCheckKomScript [CheckKom.xml ��ũ��Ʈ �Ľ� ����]";
		//#endif // SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT

		//#ifdef SERV_KOM_FILE_CHECK_ADVANCED		// �۾���¥: 2013-11-06 // ��ȫ��
	case eSIColDR_InvaildKomDetect:						return L"InvaildKomDetect [Kom ���� ����]";
		//#endif SERV_KOM_FILE_CHECK_ADVANCED

	default:
		return L"���� ���� ���� ���� �����Դϴ�.";
	}
}


